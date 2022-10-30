#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "serial_dei.c"

#define CLOUD_SIZE 150
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

void cloud_generator_testing(vector *v){
    
    point *test_array = (point*)malloc(sizeof(point)*CLOUD_SIZE);

    //point p;
    
    int i;
    for(i = 0; i < CLOUD_SIZE; i++){

        int x_c = rand()%CLOUD_WIDTH-(CLOUD_WIDTH/2);     // Returns a pseudo-random integer between 0 and RAND_MAX.
        int y_c = rand()%CLOUD_HEIGHT-(CLOUD_HEIGHT/2);     // Returns a pseudo-random integer between 0 and RAND_MAX.
        
       
        //printf("x: %d, y: %d\n", x_c, y_c);
        //point p = (point){.x = x_c, .y = y_c};
        //v->pfVectorAdd(v, &p);

        test_array[i] = (point){.x = x_c, .y = y_c}; 
        v->pfVectorAdd(v, &test_array[i]);

        //printf("x: %d, y: %d\n", x_c, y_c);
        //p.x = x_c;
        //p.y = y_c;
        //v->pfVectorAdd(v, &p);

        //p = (point){.x = x_c, .y = y_c};
        //v->pfVectorAdd(v, &p);

        //point p = (point){.x = x_c, .y = y_c};
        //v->pfVectorSet(v, i, &p);




    }

    /*
    int x_test = 663;
    int y_test = 8764;   
    point test = (point){.x = x_test, .y = y_test}; 
    v->pfVectorSet(v, 70, &test);

    int x_test2 = 456;
    int y_test2 = 777;
    point test2 = (point){.x = x_test2, .y = y_test2};
    v->pfVectorSet(v, 65, &test2);

  
    test_array[0] = (point){.x = 3429, .y = 643};
    test_array[1] = (point){.x = 109, .y = 176};

    v->pfVectorAdd(v, &test_array[0]);
    v->pfVectorAdd(v, &test_array[1]);

    */


}

int main(int argc, char *argv[]){

    srand(time(NULL));   // Initialization, should only be called once.
    
    FILE *fptr_1;
    FILE *fptr_2;
    fptr_1 = fopen("cloud.txt","w");
    fptr_2 = fopen("convex_hull.txt","w");

    
    
    point cloud_array[CLOUD_SIZE];
    VECTOR_INIT(cloud);
    VECTOR_INIT(hull);
    cloud_generator(&cloud, cloud_array);

    //cloud_generator_testing(&cloud);
    //print_cloud(&cloud, NULL);

    
    fprintf(fptr_1,"Copy the point as are in the following webapp:\nhttps://planetcalc.com/8576/\n\n");
    print_cloud(&cloud, fptr_1);

    //the sorting works, already checked
	qsort(cloud.vectorList.items, cloud.vectorList.total, sizeof(*cloud.vectorList.items), compareX);
    
    hull = divide(&cloud);
    fprintf(fptr_2,"Convex Hull:\n\n");
    print_cloud(&hull, fptr_2);

    // for(int i = 0; i < CLOUD_SIZE; i++){
    //     fprintf(fptr, "%d;%d\n", cloud_array[i].x, cloud_array[i].y);
    // }
    fclose(fptr_1);
    fclose(fptr_2);

    return 0;
}