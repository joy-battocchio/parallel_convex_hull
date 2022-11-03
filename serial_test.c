#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "vector.h"
#include "convex_hull.h"

int main(void) {
    point cloud[CLOUD_SIZE];
    point convex_hull[CLOUD_SIZE];
    srand(time(NULL));   // Initialization, should only be called once.
    cloud = cloud_generator();
    qsort(cloud, CLOUD_SIZE, sizeof(point), compareX);
    int hull_size = divide(cloud,CLOUD_SIZE,convex_hull);
}