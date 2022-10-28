#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "serial_dei.c"

#define CLOUD_SIZE 1000
#define CLOUD_WIDTH 20000
#define CLOUD_HEIGHT 20000

void print_cloud(vector *v){
    int i;
    for(i = 0; i < v->vectorList.total; i++){
        point elem = get_point(v, i);
        printf("point %d: ( %d ; %d )\n", i, elem.x, elem.y);
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
    
    srand(time(NULL));   // Initialization, should only be called once.
    point cloud_array[CLOUD_SIZE];
    VECTOR_INIT(cloud);
    cloud_generator(&cloud, cloud_array);
    print_cloud(&cloud);

    ///// TO TRANSLATE
    ///// OCHHIO AL SORT, CERCA QSORT IN serial_dei.c PER INFO
    // sort(a.begin(), a.end());
	// vector<pair<int, int> >ans = divide(a);

	// cout << "convex hull:\n";
	// for (auto e:ans)
	// cout << e.first << " "
	// 		<< e.second << endl;

    return 0;
}