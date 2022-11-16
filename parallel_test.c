#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stddef.h>
#include <math.h>
#include <mpi.h>
#include "convex_hull.h"

void defineStruct(MPI_Datatype *tstype) {
    //const int count = 2;
    int blocklens[2] = {1,1};
    MPI_Datatype types[2] = {MPI_LONG_LONG, MPI_LONG_LONG};
    MPI_Aint     disps[2] = {offsetof(point,x), offsetof(point,y)};

    MPI_Type_create_struct(2, blocklens, disps, types, tstype);
    MPI_Type_commit(tstype);
}

int main(int argc, char *argv[]) {

    int comm_sz;
    int my_rank;
    
    double start_time;
    double end_time;
    double interval;

    int cloud_size = atoi(argv[2]);
    char *path = argv[1];
    FILE *fptr;
    char buf[strlen(path)+30];  


    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //type point to use in mpi communications
    
    printf("%soutput/output_%d.txt\n", path, my_rank);
    snprintf(buf, strlen(path)+30, "%soutput/output_%d.txt", path, my_rank);
    printf("buf: %s", buf);
    fptr = fopen(buf,"w");
    
    MPI_Datatype MPI_point;
    defineStruct(&MPI_point);
    
    printf("comm_sz: %d\n", comm_sz);
    
    int fragment_sz = cloud_size/(comm_sz);
    printf("my_rank: %d, fragment_sz: %d\n", my_rank, fragment_sz);
    point cloud[cloud_size];
    //cloud_size is big, find another way?
    point convex_hull[cloud_size];
    point cloud_fragment[fragment_sz];
    if(my_rank == 0){
        FILE *fptr_cloud;
        char buf_cloud[strlen(path)+30]; 
        snprintf(buf_cloud, strlen(path)+30, "%soutput/output_cloud.txt", path);
        fptr_cloud = fopen(buf_cloud,"w");
        srand(time(NULL));   // Initialization, should only be called once.
        //cloud_generator(cloud, cloud_size);
        cloud_load(cloud, cloud_size, path);
        qsort(cloud, cloud_size, sizeof(point), compareX);
        print_cloud(cloud, cloud_size, fptr_cloud);
    }
    if(my_rank == comm_sz-1){
        start_time = MPI_Wtime();
    }
    MPI_Scatter( cloud , fragment_sz , MPI_point, cloud_fragment , fragment_sz , MPI_point , 0 , MPI_COMM_WORLD);
    int hull_size;
    hull_size = divide(cloud_fragment, fragment_sz,convex_hull, fptr);
    fprintf(fptr, "###\n");
    int step = (int)log2(comm_sz);
    int i;
    for(i = 1; i <= step; i++){
        //receiver
        // 1 - 3 - 5 - 7
        //     3   -   7
        //             7
        if((my_rank+1) % (int)pow(2,i) == 0){

            //convex hull recieved from process #
            point convex_hull_rcvd[fragment_sz+1];
            //convex hull where to save the result of merging
            point convex_hull_merged[fragment_sz*2];
            MPI_Recv(convex_hull_rcvd, fragment_sz+1, MPI_point, my_rank-(int)pow(2,i-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            fprintf(fptr, "START_PROCESS_MERGER\n");
            hull_size = merger(convex_hull_rcvd, convex_hull_rcvd[fragment_sz].x, convex_hull, hull_size, convex_hull_merged, fptr);
            fprintf(fptr, "###\n");
            //save the merged one to the normal one in order to send or merge again in next step
            memcpy(convex_hull, convex_hull_merged, hull_size*sizeof(point));
        }
        //sender
        // 0 - 2 - 4 - 6
        //     1   -   5
        //             3
        else{
            //Best way I could find to send the actual size in the same message
            //Problem is I need to send a much bigger object then hull_size
            convex_hull[fragment_sz] = (point){.x = hull_size, .y = 0};

            MPI_Ssend(convex_hull, fragment_sz+1, MPI_point, my_rank+(int)pow(2,i-1), 0, MPI_COMM_WORLD);
            printf("step: %d    sender: %d to %d hull of size %lld\n",i, my_rank, my_rank+(int)pow(2,i-1), convex_hull[fragment_sz].x);
            break;
        }
        fragment_sz *= 2;
    }
    if(my_rank == comm_sz-1){
        print_cloud(convex_hull, hull_size, NULL);
        end_time = MPI_Wtime();
        interval = end_time - start_time;
        printf("Finished in time: %lf", interval);
    }
}