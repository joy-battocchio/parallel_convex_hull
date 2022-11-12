#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "convex_hull.h"

int main(void) {
    srand(time(NULL));   // Initialization, should only be called once.

    FILE *fptr_1;
    FILE *fptr_2;
    fptr_1 = fopen("cloud.txt","w");
    fptr_2 = fopen("convex_hull.txt","w");

    point cloud[CLOUD_SIZE];
    point convex_hull[CLOUD_SIZE];
    cloud_generator(cloud);
    //fprintf(fptr_1,"Copy the point as are in the following webapp:\nhttps://planetcalc.com/8576/\n\n");

    //print_cloud(cloud,CLOUD_SIZE, fptr_1);
    print_cloud(cloud,CLOUD_SIZE, NULL);
    qsort(cloud, CLOUD_SIZE, sizeof(point), compareX);
    
    int hull_size = divide(cloud,CLOUD_SIZE,convex_hull);
    //print_cloud(convex_hull, hull_size, fptr_2);
    print_cloud(convex_hull, hull_size, NULL);
    fclose(fptr_1);
    fclose(fptr_2);
    return 0;
}