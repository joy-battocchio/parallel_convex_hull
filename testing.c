#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "serial_dei.c"

#define CLOUD_SIZE 100
#define CLOUD_WIDTH 1000
#define CLOUD_HEIGHT 1000

// void print_cloud(vector *v){
//     int i;
//     for(i = 0; i < v->vectorList.total; i++){
//         point elem = get_point(v, i);
//         printf("point %d: ( %d ; %d )\n", i, elem.x, elem.y);
//     }
// }

void cloud_generator(vector *v, point *c_array){
    int i;
    for(i = 0; i < CLOUD_SIZE; i++){
        int x_c = rand()%CLOUD_WIDTH-(CLOUD_WIDTH/2);      // Returns a pseudo-random integer between 0 and RAND_MAX.
        int y_c = rand()%CLOUD_HEIGHT-(CLOUD_HEIGHT/2);      // Returns a pseudo-random integer between 0 and RAND_MAX.
        c_array[i] = (point){.x = x_c, .y = y_c};
        v->pfVectorAdd(v, &c_array[i]);
    }
}

int main(int argc, char *argv[]){
    
    FILE *fptr;
    fptr = fopen("cloud.txt","w");

    srand(time(NULL));   // Initialization, should only be called once.
    point cloud_array[CLOUD_SIZE];
    VECTOR_INIT(cloud);
    VECTOR_INIT(hull);
    cloud_generator(&cloud, cloud_array);
    printf("Initial cloud: \n");
    print_cloud(&cloud);

    //the sorting works, already checked
	qsort(cloud.vectorList.items, cloud.vectorList.total,sizeof(*cloud.vectorList.items), compareX);
    
    hull = divide(&cloud);
    printf("Convex hull: \n");
    print_cloud(&hull);

    fprintf(fptr,"Copy the point as are in the following webapp:\nhttps://planetcalc.com/8576/\n\n");
    for(int i = 0; i < CLOUD_SIZE; i++){
        fprintf(fptr, "%d;%d\n", cloud_array[i].x, cloud_array[i].y);
    }
    fclose(fptr);
    return 0;
}