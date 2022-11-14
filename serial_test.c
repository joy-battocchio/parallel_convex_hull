#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "convex_hull.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));   // Initialization, should only be called once.

    int cloud_size = atoi(argv[1]);

    FILE *fptr_1;
    FILE *fptr_2;
    FILE *fptr;
    fptr_1 = fopen("cloud.txt","w");
    fptr_2 = fopen("convex_hull.txt","w");
    fptr = fopen("steps.txt", "w");

    point cloud[cloud_size];
    point convex_hull[cloud_size];
    cloud_generator(cloud, cloud_size);
    //fprintf(fptr_1,"Copy the point as are in the following webapp:\nhttps://planetcalc.com/8576/\n\n");

    print_cloud(cloud,cloud_size, fptr_1);
    print_cloud(cloud,cloud_size, NULL);
    qsort(cloud, cloud_size, sizeof(point), compareX);
    
    int hull_size = divide(cloud,cloud_size,convex_hull, fptr);
    print_cloud(convex_hull, hull_size, fptr_2);
    print_cloud(convex_hull, hull_size, NULL);
    fclose(fptr_1);
    fclose(fptr_2);
    return 0;
}