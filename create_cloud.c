#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stddef.h>
#include <math.h>

const int CLOUD_WIDTH = 1000000000;
const int CLOUD_HEIGHT = 1000000000;

typedef struct {
    long long x;
    long long y;
} point;

void cloud_generator(point *cloud, int cloud_size){
    int i;
    for(i = 0; i < cloud_size; i++){
		// int x_c = rand()%CLOUD_WIDTH-(CLOUD_WIDTH/2);      // Returns a pseudo-random integer between 0 and RAND_MAX.
        // int y_c = rand()%CLOUD_HEIGHT-(CLOUD_HEIGHT/2);      // Returns a pseudo-random integer between 0 and RAND_MAX.
        // cloud[i] = (point){.x = x_c, .y = y_c};
		long long max_width = rand()%CLOUD_WIDTH+1;
		long long max_height = rand()%CLOUD_HEIGHT+1;
        //printf("i: %d    %d, %d\n",i, max_width, max_height);

        long long x_c = rand()%max_width*(rand()%2*2-1);      // Returns a pseudo-random integer between 0 and RAND_MAX.
        long long y_c = rand()%max_height*(rand()%2*2-1);      // Returns a pseudo-random integer between 0 and RAND_MAX.
		cloud[i] = (point){.x = x_c, .y = y_c};
    }
}

void print_cloud(point *cloud,int size, FILE *ptr){
    int i;
    if(ptr != NULL){
        for(i = 0; i < size; i++){
            point elem = cloud[i];
            fprintf(ptr,"%lld;%lld\n",elem.x, elem.y);
        }
    }else{
        for(i = 0; i < size; i++){
            point elem = cloud[i];
			printf("%lld;%lld\n",elem.x, elem.y);
        }
		printf("____________________________\n\n");
    }
}

int compareX(const void *p1, const void *q1){
	return ((point*)p1)->x - ((point*)q1)->x;
}

int main(int argc, char *argv[]) {
    int cloud_size = atoi(argv[1]);
    FILE *fptr;
    fptr = fopen("cloud_to_load.txt","w");
    point *cloud;
    
    cloud = (point*)malloc(cloud_size * sizeof(point));
    
    cloud_generator(cloud,cloud_size);
    qsort(cloud, cloud_size, sizeof(point), compareX);
    print_cloud(cloud,cloud_size,fptr);
}
