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
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Aint     disps[2] = {offsetof(point,x), offsetof(point,y)};

    MPI_Type_create_struct(2, blocklens, disps, types, tstype);
    MPI_Type_commit(tstype);
}

int main(void) {

    int comm_sz;
    int my_rank;
    
    double start_time;
    double end_time;
    double interval;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //type point to use in mpi communications
    MPI_Datatype MPI_point;
    defineStruct(&MPI_point);
     
    int fragment_sz = CLOUD_SIZE/(comm_sz);
    point cloud[CLOUD_SIZE];
    //CLOUD_SIZE is big, find another way?
    point convex_hull[CLOUD_SIZE];
    point cloud_fragment[fragment_sz];
    if(my_rank == 0){
        srand(time(NULL));   // Initialization, should only be called once.
        cloud_generator(cloud);
        qsort(cloud, CLOUD_SIZE, sizeof(point), compareX);
        print_cloud(cloud,CLOUD_SIZE,NULL);
    }
    if(my_rank == comm_sz-1){
        start_time = MPI_Wtime();
    }
    MPI_Scatter( cloud , fragment_sz , MPI_point, cloud_fragment , fragment_sz , MPI_point , 0 , MPI_COMM_WORLD);
    int hull_size;
    hull_size = divide(cloud_fragment, fragment_sz,convex_hull);
    int step = (int)log2(comm_sz);
    int i;
    for(i = 1; i <= step; i++){
        //receiver
        // 1 - 3 - 5 - 7
        //     3   -   7
        //             7
        if((my_rank+1) % (int)pow(2,i) == 0){
            int ready = 43;
            if(i > 1){
                MPI_Send(&ready, 1, MPI_INT, my_rank-(int)pow(2,i-1), 0, MPI_COMM_WORLD);
            }
            //convex hull recieved from process #
            point convex_hull_rcvd[fragment_sz+1];
            //convex hull where to save the result of merging
            point convex_hull_merged[fragment_sz*2];
            MPI_Recv(convex_hull_rcvd, fragment_sz+1, MPI_point, my_rank-(int)pow(2,i-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            hull_size = merger(convex_hull_rcvd, convex_hull_rcvd[fragment_sz].x, convex_hull, hull_size, convex_hull_merged);
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
            int ready;
            if(i > 1){
                MPI_Recv(&ready, 1, MPI_INT, my_rank+(int)pow(2,i-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            MPI_Send(convex_hull, fragment_sz+1, MPI_point, my_rank+(int)pow(2,i-1), 0, MPI_COMM_WORLD);
            //printf("step: %d    sender: %d to %d hull of size %d\n",i, my_rank, my_rank+(int)pow(2,i-1), convex_hull[fragment_sz].x);
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