#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "serial_dei.c"

#define CLOUD_SIZE 100
#define CLOUD_WIDTH 1000
#define CLOUD_HEIGHT 1000

void print_cloud(vector *v, FILE *ptr){
    int i;
    if(ptr != NULL){
        for(i = 0; i < v->vectorList.total; i++){
            point elem = get_point(v, i);
            fprintf(ptr,"%d;%d\n",elem.x, elem.y);
        }
    }else{
        for(i = 0; i < v->vectorList.total; i++){
            point elem = get_point(v, i);
            printf("point %d: ( %d ; %d )\n", i, elem.x, elem.y);
        }
    }
}

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
    
    FILE *fptr_1;
    FILE *fptr_2;
    fptr_1 = fopen("cloud.txt","w");
    fptr_2 = fopen("convex_hull.txt","w");

    srand(time(NULL));   // Initialization, should only be called once.
    point cloud_array[CLOUD_SIZE];
    VECTOR_INIT(cloud);
    VECTOR_INIT(hull);
    cloud_generator(&cloud, cloud_array);
    fprintf(fptr_1,"Copy the point as are in the following webapp:\nhttps://planetcalc.com/8576/\n\n");
    print_cloud(&cloud, fptr_1);

    //the sorting works, already checked
	qsort(cloud.vectorList.items, cloud.vectorList.total,sizeof(*cloud.vectorList.items), compareX);
    
    hull = divide(&cloud);
    fprintf(fptr_1,"Convex Hull:\n\n");
    print_cloud(&hull, fptr_2);

    // for(int i = 0; i < CLOUD_SIZE; i++){
    //     fprintf(fptr, "%d;%d\n", cloud_array[i].x, cloud_array[i].y);
    // }
    fclose(fptr_1);
    fclose(fptr_2);
    return 0;
}