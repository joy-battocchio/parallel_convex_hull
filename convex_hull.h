#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>
#include "vector.h"

#define CLOUD_SIZE 150
#define CLOUD_WIDTH 1000
#define CLOUD_HEIGHT 1000

typedef struct {
    int x;
    int y;
} point;

point mid;

point get_point(vector *v, int i){ return *(point*)((v->vectorList.items[i]));}


/*

######DONE######

*/
// check if vector has element (point) and returns its position, -1 otherwise
int hasElement(point *cloud, int size, point elem){
    int i;
    if(v){
        for(i=0; i<size; i++){
            if(cloud[i] == elem){
                return i;
            }
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

// returns the orientatation value (-1 ,0, 1) of 3 given points 
int orientation(point a, point b, point c){
	int res = (b.y-a.y)*(c.x-b.x) - (c.y-b.y)*(b.x-a.x);
	if (res == 0) return 0;
	if (res > 0) return 1;
	return -1;
}


/*

######DONE######

*/
//compare the points for the sort function by polar angle
int compare(const void *p1,const void *q1){

	point p = {.x = (p1->x - mid.x, 
			   .y = (p1->y - mid.y};
    point q = {.x = (q1->x - mid.x, 
			   .y = (q1->y - mid.y};
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
	return p1->x - q1->x;
}


/*

######DONE######

*/
int merger(point *a,int a_sz, point *b, int b_sz, point *cx_hull){
	// n1 -> number of points in polygon a
	// n2 -> number of points in polygon b
    // point* a = (point*)aV;
    // point* b = (point*)bV;

	int ia = 0, ib = 0;

    // ia -> rightmost point of a
	for (int i=1; i<a_sz; i++)
		if(a[i].x > a[ia].x)
			ia = i;
	

	// ib -> leftmost point of b
	for (int i=1; i<b_sz; i++)
		if (b[i].x < b[ib].x)
			ib=i;
	

	// finding the upper tangent
	int inda = ia, indb = ib;
	bool done = 0;
	while (!done){
		done = 1;
		while (orientation(b[indb], a[inda], a[(inda+1)%n1]) >=0)
			inda = (inda + 1) % n1;
		

		while (orientation(a[inda], b[indb], b[(n2+indb-1)%n2]) <=0){
			indb = (n2+indb-1)%n2;
			done = 0;
		}
	}

	int uppera = inda, upperb = indb;
	inda = ia, indb=ib;
	done = 0;
	//finding the lower tangent
	while (!done){
		done = 1;
		while (orientation(a[inda], b[indb], b[(indb+1)%n2])>=0)
			indb=(indb+1)%n2;

		while (orientation(b[indb], a[inda], a[(n1+inda-1)%n1])<=0){
			inda=(n1+inda-1)%n1;
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
		ind = (ind+1)%n1;
        cx_hull[hull_size] = a[ind];
        hull_size++;
	}

	ind = lowerb;
    cx_hull[hull_size] = b[lowerb];
    hull_size++; 
	while (ind != upperb){
		ind = (ind+1)%n2;
        cx_hull[hull_size] = b[ind];
        hull_size++;
	}
	return hull_size;
}


/*

######DONE######

*/
int bruteHull(point *cloud, int size, point *cx_hull){
	// Take any pair of points from the set and check
	// whether it is the edge of the convex hull or not.
	// if all the remaining points are on the same side
	// of the line then the line is the edge of convex
	// hull otherwise not
    //point* a = (point*)aV;
	int hull_size = 0;
	for (int i=0; i<size; i++){
		for (int j=i+1; j<size; j++){
			int x1 = cloud[i].x, x2 = cloud[j].x;
			int y1 = cloud[i].y, y2 = cloud[j].y;

			int a1 = y1-y2;
			int b1 = x2-x1;
			int c1 = x1*y2-y1*x2;
			int pos = 0, neg = 0;
			for (int k=0; k<size; k++){
				if (a1*a[k].x+b1*a[k].y+c1 <= 0)
					neg++;
				if (a1*a[k].x+b1*a[k].y+c1 >= 0)
					pos++;
			}
			if (pos == size || neg == size){
				if(hasElement(cx_hull, hull_size, a[i]) == -1){
					cx_hull[hull_size] = a[i];
                    hull_size++;
				}
				if(hasElement(cx_hull, hull_size, a[j]) == -1){
                    cx_hull[hull_size] = a[j];
                    hull_size++;
				}
			}
		}
	}
	// Sorting the points in the anti-clockwise order
	mid.x = 0;
	mid.y = 0;
	for (int i=0; i<hull_size; i++){
		mid.x += cx_hull[i].x;
		mid.y += cx_hull[i].y;
        cx_hull[i].x *= n;
        cx_hull[i].y *= n;
	}
	qsort(cx_hull, hull_size, sizeof(point), compare);
	for (int i=0; i<n; i++){
        cx_hull[i].x /= n;
        cx_hull[i].y /= n;
	}
	return hull_size;
}

/*

######DONE######

*/
//already translated into only array for parallel implementation
int divide(point *cloud, int size, point *cx_hull){
	// If the number of points is less than 6 then the
	// function uses the brute algorithm to find the
	// convex hull
    int lh_size = 0;
    int rh_size = 0;
	if (size <= 5) return bruteHull(cloud, size, cx_hull);

	// left contains the left half points
	// right contains the right half points
    point left[size/2];
    point right[size/2 + size%2];
	for (int i=0; i<size/2; i++)
		left[i] = cloud[i];
	for (int i=size/2; i<size; i++)
		right[i] = cloud[i];

	// convex hull for the left and right sets
    point left_hull[size/2];
    point right_hull[size/2 + size%2];
	lh_size = divide(left,size/2, left_hull);
	rh_size = divide(right, size/2 + size%2, right_hull);

	// merging the convex hulls
	return merger(left_hull, lh_size, right_hull, rh_size, cx_hull);
}

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


/*

######DONE######

*/
void cloud_generator(){
    point *cloud = malloc(sizeof(point)*CLOUD_SIZE); 
    int i;
    for(i = 0; i < CLOUD_SIZE; i++){
        int x_c = rand()%CLOUD_WIDTH-(CLOUD_WIDTH/2);      // Returns a pseudo-random integer between 0 and RAND_MAX.
        int y_c = rand()%CLOUD_HEIGHT-(CLOUD_HEIGHT/2);      // Returns a pseudo-random integer between 0 and RAND_MAX.
        cloud[i] = (point){.x = x_c, .y = y_c};
    }
    return cloud;
}

void defineStruct(MPI_Datatype *tstype) {
    const int count = 2;
    int          blocklens[count] = {1,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_INT};
    MPI_Aint     disps[count] = {offsetof(point,x), offsetof(point,y)};

    MPI_Type_create_struct(count, blocklens, disps, types, tstype);
    MPI_Type_commit(tstype);
}