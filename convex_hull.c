#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif

extern bool flag;

//this seems to be the highest value for the cloud width and height
// #define CLOUD_WIDTH 10000000
// #define CLOUD_HEIGHT 10000000
const int CLOUD_WIDTH = 1000000000;
const int CLOUD_HEIGHT = 1000000000;
const int MAX_CLOUD_SIZE = 4194304;

typedef struct {
    long long x;
    long long y;
} point;

point mid;

/*

######DONE######

*/
// check if vector has element (point) and returns its position, -1 otherwise
int hasElement(point *cloud, int size, point elem){
    int i;
	for(i=0; i<size; i++){
		if(cloud[i].x == elem.x && cloud[i].y == elem.y){
			return i;
		}
	}
	return -1;
}

// returns the correspondent quadrant (I, II, III, IV) of a given point 
int quad(point p){
    if(p.x >= 0 && p.y >= 0) return 1;
    if(p.x <= 0 && p.y >= 0) return 2;
	if(p.x <= 0 && p.y <= 0) return 3;
    return 4;
}

// returns the orientation value (-1 ,0, 1) of 3 given points 
int orientation(point a, point b, point c){
	long long res = (b.y-a.y)*(c.x-b.x) - (c.y-b.y)*(b.x-a.x);
	if (res == 0) return 0;
	if (res > 0) return 1;
	return -1;
}


/*

######DONE######

*/
//compare the points for the sort function by polar angle
int compare(const void *p1,const void *q1){

	point p = {.x = ((point*)p1)->x - mid.x, 
			   .y = ((point*)p1)->y - mid.y};
    point q = {.x = ((point*)q1)->x - mid.x, 
			   .y = ((point*)q1)->y - mid.y};
	int one = quad(p);
	int two = quad(q);

	if (one != two) return (one > two);
	return (p.y*q.x > q.y*p.x);
}



/*

######DONE######

*/
//compare the points to sort them by x 
int compareX(const void *p1, const void *q1){
	return ((point*)p1)->x - ((point*)q1)->x;
}


/*

######DONE######

*/
int merger(point *a,int a_sz, point *b, int b_sz, point *cx_hull, FILE *fptr, int n_threads){
	// n1 -> number of points in polygon a
	// n2 -> number of points in polygon b
    // point* a = (point*)aV;
    // point* b = (point*)bV;
	//qsort(a, a_sz, sizeof(point), compareX);
	//qsort(b, b_sz, sizeof(point), compareX);

	

	!flag ? true : fprintf(fptr, "START_MERGER\n");
	!flag ? true : fprintf(fptr, "# %d\n",a_sz+b_sz);
		
	int ia = 0, ib = 0;
	// ia -> rightmost point of a
	for (int i=1; i<a_sz; i++){
		if(a[i].x > a[ia].x){ 
			ia = i;
		}		
	}
	
	// ib -> leftmost point of b
	for (int i=1; i<b_sz; i++){
		if (b[i].x < b[ib].x){
			ib=i;
		}	
	}
	
	// finding the upper tangent
	int inda = ia, indb = ib;
	bool done = 0;
	while (!done){
		done = 1;
		while (orientation(b[indb], a[inda], a[(inda+1)%a_sz]) >=0)
			inda = (inda + 1) % a_sz;
		

		while (orientation(a[inda], b[indb], b[(b_sz+indb-1)%b_sz]) <=0){
			indb = (b_sz+indb-1)%b_sz;
			done = 0;
		}
	}

	int uppera = inda, upperb = indb;
	inda = ia, indb=ib;
	done = 0;
	//finding the lower tangent
	while (!done){
		done = 1;
		while (orientation(a[inda], b[indb], b[(indb+1)%b_sz])>=0){
			indb=(indb+1)%b_sz;
		}
		while (orientation(b[indb], a[inda], a[(a_sz+inda-1)%a_sz])<=0){
			inda=(a_sz+inda-1)%a_sz;
			done=0;
		}
	}


	int lowera = inda, lowerb = indb;
    int hull_size = 0;
    //point* ret = (point*)retV;
	//ret contains the convex hull after merging the two convex hulls
	//with the points sorted in anti-clockwise order
	int ind = uppera;
    cx_hull[hull_size] = a[uppera];
    hull_size++; 
	while (ind != lowera){
		ind = (ind+1)%a_sz;
        cx_hull[hull_size] = a[ind];
		!flag ? true : fprintf(fptr, "%lld;%lld %lld;%lld\n", cx_hull[hull_size-1].x, cx_hull[hull_size-1].y, a[ind].x, a[ind].y);
        hull_size++;
	}

	ind = lowerb;
    cx_hull[hull_size] = b[lowerb];
	!flag ? true : fprintf(fptr, "%lld;%lld %lld;%lld\n", cx_hull[hull_size-1].x, cx_hull[hull_size-1].y, cx_hull[hull_size].x, cx_hull[hull_size].y);
    hull_size++; 
	while (ind != upperb){
		ind = (ind+1)%b_sz;
        cx_hull[hull_size] = b[ind];
		!flag ? true : fprintf(fptr, "%lld;%lld %lld;%lld\n", cx_hull[hull_size-1].x, cx_hull[hull_size-1].y, b[ind].x, b[ind].y);
        hull_size++;
	}
	!flag ? true : fprintf(fptr, "%lld;%lld %lld;%lld\n", cx_hull[hull_size-1].x, cx_hull[hull_size-1].y, cx_hull[0].x, cx_hull[0].y);
	!flag ? true : fprintf(fptr, "END_MERGER\n");
	return hull_size;
}


/*

######DONE######

*/
int bruteHull(point *cloud, int size, point *cx_hull, FILE *fptr){
	// Take any pair of points from the set and check
	// whether it is the edge of the convex hull or not.
	// if all the remaining points are on the same side
	// of the line then the line is the edge of convex
	// hull otherwise not
    //point* a = (point*)aV;
	!flag ? true : fprintf(fptr, "START_BH\n");
	int hull_size = 0;
	for (int i=0; i<size; i++){
		for (int j=i+1; j<size; j++){
			long long x1 = cloud[i].x, x2 = cloud[j].x;
			long long y1 = cloud[i].y, y2 = cloud[j].y;
			
			long long a1 = y1-y2;
			long long b1 = x2-x1;
			//HERE IS THE PROBLEM
			long long c1 = x1*y2-y1*x2;
			int pos = 0, neg = 0;
			for (int k=0; k<size; k++){
				if (a1*cloud[k].x+b1*cloud[k].y+c1 <= 0)
					neg++;
				if (a1*cloud[k].x+b1*cloud[k].y+c1 >= 0)
					pos++;
			}
			if (pos == size || neg == size){
				if(hasElement(cx_hull, hull_size, cloud[i]) == -1){
					cx_hull[hull_size] = cloud[i];
					hull_size++;
				}
				if(hasElement(cx_hull, hull_size, cloud[j]) == -1){
                    cx_hull[hull_size] = cloud[j];
					hull_size++;
				}
				!flag ? true : fprintf(fptr, "%lld;%lld ",cloud[i].x, cloud[i].y);
				!flag ? true : fprintf(fptr, "%lld;%lld\n",cloud[j].x, cloud[j].y);
			}
		}
	}
	// Sorting the points in the anti-clockwise order
	mid.x = 0;
	mid.y = 0;
	for (int i=0; i<hull_size; i++){
		mid.x += cx_hull[i].x;
		mid.y += cx_hull[i].y;
        cx_hull[i].x *= hull_size;
        cx_hull[i].y *= hull_size;
	}
	qsort(cx_hull, hull_size, sizeof(point), compare);
	for (int i=0; i<hull_size; i++){
        cx_hull[i].x /= hull_size;
        cx_hull[i].y /= hull_size;
	}
	!flag ? true : fprintf(fptr, "END_BH\n");
	return hull_size;
}

/*

######DONE######

*/
//already translated into only array for parallel implementation
int divide(point *cloud, int size, point *cx_hull, FILE *fptr, int n_threads){
	// If the number of points is less than 6 then the
	// function uses the brute algorithm to find the
	// convex hull

    int lh_size = size/2;
    int rh_size = size/2 + size%2;
	if (size <= 5) return bruteHull(cloud, size, cx_hull, fptr);

	// left contains the left half points
	// right contains the right half points
    point *left = (point*)malloc(lh_size * sizeof(point));
    point *right = (point*)malloc(rh_size * sizeof(point));

	bool multithreading = false;
	//printf("size: %d", size);
	(n_threads>1 && size/n_threads > 2000) ? (multithreading=true) : (multithreading=false);
	//printf("multithreading: %d\n", multithreading);
	//printf("n_threads: %d\n", n_threads);

	
	if(multithreading){
		#ifdef _OPENMP
		//double start = omp_get_wtime();
		# pragma omp parallel num_threads(n_threads) if(multithreading)
		{
			//int thread_num = omp_get_thread_num();
			//int real_thread_count = omp_get_num_threads();
			//int cpu_num = sched_getcpu();
			//printf("[DIVIDE] Thread n:%d of (%d) of %d CPU, is starting to work [in divide]\n", thread_num, real_thread_count, cpu_num);

			//printf("[lh size]: %d \n", lh_size);
			//printf("[rh size]: %d \n", rh_size);

			# pragma omp for nowait schedule(static)
			for (int i=0; i<lh_size; i++){
				//printf("[DIVIDE] Thread n:%d of (%d) of %d CPU, is in first for [in divide] \n", thread_num, real_thread_count, cpu_num);
				left[i] = cloud[i];	
				//sleep(0.01);		
			}
			

			//# pragma omp barrier

			# pragma omp for nowait schedule(static)
			for (int i=0; i<rh_size; i++){
				//printf("[DIVIDE] Thread n:%d of (%d) of %d CPU, is in second for [in divide] \n", thread_num, real_thread_count, cpu_num);
				right[i] = cloud[i+lh_size];	
			}	
		}
		//double end = omp_get_wtime();
		
		# else //no openmp support, no threads
			//printf("inelse: %d\n", size);
			for (int i=0; i<lh_size; i++){
				//printf("[DIVIDE] first assignment, without thread\n");
				left[i] = cloud[i];
			}
			for (int i=0; i<rh_size; i++){
				//printf("[DIVIDE] second assignment, without thread\n");
				right[i] = cloud[i+lh_size];
			}		
		# endif
	}else{ //no multithreading
		//printf("in second else: %d\n", size);
		for (int i=0; i<lh_size; i++){
			//printf("[DIVIDE] first assignment, without thread\n");
			left[i] = cloud[i];
		}
		for (int i=0; i<rh_size; i++){
			//printf("[DIVIDE] second assignment, without thread\n");
			right[i] = cloud[i+lh_size];
		}
	}
	

	// convex hull for the left and right sets 
    point left_hull[lh_size];
	point right_hull[rh_size];

	lh_size = divide(left,lh_size, left_hull, fptr, n_threads);
	rh_size = divide(right, rh_size, right_hull, fptr, n_threads);
	
	free(left);
	free(right);

	// merging the convex hulls
	return merger(left_hull, lh_size, right_hull, rh_size, cx_hull, fptr, n_threads);	
		
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


/*

######DONE######

*/
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

void cloud_load(point *cloud, int cloud_size, char* path, int rank){
    int start_pos = cloud_size*rank;   
	char buf_cloud_to_load[strlen(path)+30]; 
    snprintf(buf_cloud_to_load, strlen(path)+30, "%scloud_to_load.txt", path);
	
	FILE *fptr = fopen(buf_cloud_to_load, "r");
	if (fptr == NULL){
        printf ("Error opening the file\n\n'");
	}
	for(int i = 0; i< MAX_CLOUD_SIZE; i++){
		char line[256];
		if(i < start_pos){
			fgets(line, sizeof(line), fptr);
			fflush(stdin);
		}else{
			for(int j = 0; j < cloud_size; j++){
				long long x_c;
				long long y_c;
				fscanf(fptr, "%lld;%lld\n", &x_c, &y_c);
				cloud[j] = (point){.x = x_c, .y = y_c};	
			}

			fclose(fptr);
			return;
		}
	}
}


