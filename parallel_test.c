#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>
#include "vector.h"
#include "convex_hull.h"

int main(void) {

    int comm_sz;
    int my_rank;
    int lenght_array[2] = {sizeof(int), sizeof(int)};
    
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    //type point to use in mpi communications
    MPI_Datatype MPI_point;
    defineStruct(&MPI_point);
     
    int fragment_sz = CLOUD_SIZE/comm_sz
    point cloud[CLOUD_SIZE];
    point cloud_fragment[fragment_sz];
    if(my_rank == 0){
        srand(time(NULL));   // Initialization, should only be called once.
        cloud = cloud_generator();
        qsort(cloud, CLOUD_SIZE, sizeof(point), compareX);
        MPI_Scatter( cloud , fragment_sz , MPI_point, cloud_fragment , fragment_sz , MPI_point , 0 , MPI_COMM_WORLD);
    }else{
        int hull_size;
        MPI_Scatter( cloud , fragment_sz , MPI_point, cloud_fragment , fragment_sz , MPI_point , 0 , MPI_COMM_WORLD);
        point convex_hull[fragment_sz];
        hull_size = divide(cloud_fragment, fragment_sz,convex_hull);
        //
        int step = log2(comm_sz);
        int i;
        for(i = 1; i <= step; i++){
            if(my_rank % pow(2,step) == 0){
                //convex hull recieved from process #
                point convex_hull_rcvd[fragment_sz];
                //convex hull where to save the result of merging
                point convex_hull_merged[fragment_sz*2];
                MPI_recv(convex_hull_rcvd, fragment_sz, MPI_point, my_rank-pow(2,step-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                hull_size = merger(convex_hull, hull_size, convex_hull_rcvd, convex_hull_rcvd[fragment_sz], convex_hull_merged);
                //save the merged one to the normal one in order to send or merge again in next step
                convex_hull = convex_hull_merged;
            }else{
                //Best way I could find to send the actual size in the same message
                //Problem is I need to send a much bigger object then hull_size
                convex_hull[fragment_sz] = hull_size;
                MPI_Send(convex_hull, fragment_sz, MPI_point, my_rank+pow(2,step-1), 0, my_rank, MPI_COMM_WORLD);
            }
            fragment_sz *= 2;
        }   
    }
}