#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stddef.h>
#include <math.h>
#include <mpi.h>
#include "convex_hull.h"

bool flag;

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

    char *path = argv[1];
    int cloud_size = atoi(argv[2]);
    flag = (atoi(argv[3]) == 1) ? true : false;

    int n_threads;
    if(argv[4]==NULL){
        n_threads = 0;
    }else{
        n_threads = atoi(argv[4]);
        printf("n_threads: %d", n_threads);
    }
    
    FILE *fptr;
    //FILE *fptr_cloud;
    char buf[strlen(path)+30];     

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    //printf("%soutput/output_%d.txt\n", path, my_rank);
    snprintf(buf, strlen(path)+30, "%soutput/output_%d.txt", path, my_rank);
    //printf("buf: %s", buf);
    fptr = fopen(buf,"w");
    
    MPI_Datatype MPI_point;
    defineStruct(&MPI_point);
    
    //printf("[BEFORE SCATTER]: RANK: %d\n", my_rank);
    
    int fragment_sz = cloud_size/(comm_sz);
    printf("my_rank: %d, fragment_sz: %d\n", my_rank, fragment_sz);
    
    //point *cloud;
    point *convex_hull = (point*)malloc(cloud_size * sizeof(point));
    point *cloud_fragment = (point*)malloc(fragment_sz * sizeof(point));
    cloud_load(cloud_fragment, fragment_sz, path, my_rank);
    
    if(my_rank == 0){
        //cloud = (point*)malloc(cloud_size * sizeof(point));
        //char buf_cloud[strlen(path)+30]; 
        // snprintf(buf_cloud, strlen(path)+30, "%soutput/output_cloud.txt", path);
        // fptr_cloud = fopen(buf_cloud,"w");
        //srand(time(NULL));
        //cloud_generator(cloud, cloud_size);
        //qsort(cloud, cloud_size, sizeof(point), compareX);
        //print_cloud(cloud, cloud_size, fptr_cloud);
    }
    if(my_rank == comm_sz-1){
        start_time = MPI_Wtime();
    }
    //printf("[BEFORE BARRIER]: RANK: %d\n", my_rank);
    //MPI_Barrier(MPI_COMM_WORLD);
    //MPI_Scatter(cloud, fragment_sz, MPI_point, cloud_fragment, fragment_sz, MPI_point, 0, MPI_COMM_WORLD);
    
    int hull_size;
    hull_size = divide(cloud_fragment, fragment_sz, convex_hull, fptr, n_threads);
    !flag ? true : fprintf(fptr, "###\n");
    int step = (int)log2(comm_sz);
    int i;
    for(i = 1; i <= step; i++){
        //receiver
        // 1 - 3 - 5 - 7
        //     3   -   7
        //             7
        if((my_rank+1) % (int)pow(2,i) == 0){
            
            /*
            int ready = 43;
            if(i > 1){
                MPI_Send(&ready, 1, MPI_INT, my_rank-(int)pow(2,i-1), 0, MPI_COMM_WORLD);
            }
            */

            //convex hull recieved from process #
            point convex_hull_rcvd[fragment_sz+1];
            //convex hull where to save the result of merging
            point convex_hull_merged[fragment_sz*2];
            MPI_Recv(convex_hull_rcvd, fragment_sz+1, MPI_point, my_rank-(int)pow(2,i-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("step: %d    receiver: %d from %d\n",i, my_rank,  my_rank-(int)pow(2,i-1));
            !flag ? true : fprintf(fptr, "START_PROCESS_MERGER\n");
            hull_size = merger(convex_hull_rcvd, convex_hull_rcvd[fragment_sz].x, convex_hull, hull_size, convex_hull_merged, fptr, n_threads);
            !flag ? true : fprintf(fptr, "###\n");
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
            
            /*
            int ready;
            if(i > 1){
                MPI_Recv(&ready, 1, MPI_INT, my_rank+(int)pow(2,i-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            */

            MPI_Send(convex_hull, fragment_sz+1, MPI_point, my_rank+(int)pow(2,i-1), 0, MPI_COMM_WORLD);
            printf("step: %d    sender: %d to %d hull of size %lld\n",i, my_rank, my_rank+(int)pow(2,i-1), convex_hull[fragment_sz].x);
            
            if(my_rank==0){
                //free(cloud);
            }
            free(convex_hull);
            free(cloud_fragment);

            break;
        }
        fragment_sz *= 2;
    }
    if(my_rank == comm_sz-1){
        /*
        FILE *fTime;
        char bufTime[strlen(path)+30]; 
        snprintf(bufTime, strlen(path)+30, "%soutput/output_time.txt", path);
        fTime = fopen(bufTime, "a");
        */
        print_cloud(convex_hull, hull_size, NULL);
        end_time = MPI_Wtime();
        interval = end_time - start_time;
        printf("Finished in time: %lf\n", interval);
        //fprintf(fTime, "%lf\n", interval);
    }
    
    printf("[FINAL] My rank: %d\n", my_rank);
    MPI_Finalize();
    
    return 0;
}