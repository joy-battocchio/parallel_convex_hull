#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"

// int main(int argc, char *argv[])
// {
//     int i =0;
//     //init vector
//     VECTOR_INIT(v);
//     //Add data in vector
//     v.pfVectorAdd(&v,"aticleworld.com\n");
//     v.pfVectorAdd(&v,"amlendra\n");
//     v.pfVectorAdd(&v,"Pooja\n");
//     //print the data and type cast it
//     for (i = 0; i < v.pfVectorTotal(&v); i++)
//     {
//         printf("%s", (char*)v.pfVectorGet(&v, i));
//     }
//     //Set the data at index 0
//     v.pfVectorSet(&v,0,"Apoorv\n");
//     printf("\n\n\nVector list after changes\n\n\n");
//     //print the data and type cast it
//     for (i = 0; i < v.pfVectorTotal(&v); i++)
//     {
//         printf("%s", (char*)v.pfVectorGet(&v, i));
//     }
//     return 0;
// }



typedef struct {
    int x;
    int y;
} point;

point mid;

point get_point(vector *v, int i){ return *(point*)((v->vectorList.items[i]));}

// check if vector has element (point) and returns its position, -1 otherwise
int hasElement(vector *v, point elem){ 
    int total = v->vectorList.total;
    int i;
    if(v){
        for(i=0; i<total; i++){
			point vec_elem = *(point*)(v->vectorList.items[i]);
            if((vec_elem.x == elem.x) && (vec_elem.y == elem.y)){
                return i;
            }
        }
        return -1;
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

//compare the points for the sort function by polar angle
int compare(const void *p1,const void *q1){
	point* p_pointer = *(point**)p1;
	point* q_pointer = *(point**)q1;

	point p = {.x = ((point*)p_pointer)->x - mid.x, 
			   .y = ((point*)p_pointer)->y - mid.y};
    point q = {.x = ((point*)q_pointer)->x - mid.x, 
			   .y = ((point*)q_pointer)->y - mid.y};
	int one = quad(p);
	int two = quad(q);

	if (one != two) return (one > two);
	return (p.y*q.x > q.y*p.x);
}

//compare the points to sort them by x 
int compareX(const void *p1, const void *q1){
	point* p_pointer = *(point**)p1;
	point* q_pointer = *(point**)q1;

	point p = {.x = ((point*)p_pointer)->x, 
			   .y = ((point*)p_pointer)->y};
    point q = {.x = ((point*)q_pointer)->x, 
			   .y = ((point*)q_pointer)->y};
	return p.x - q.x;
}

vector merger(vector *a, vector *b){
	// n1 -> number of points in polygon a
	// n2 -> number of points in polygon b
    // point* a = (point*)aV;
    // point* b = (point*)bV;

	int n1 = a->vectorList.total, n2 = b->vectorList.total;

	int ia = 0, ib = 0;

    // ia -> rightmost point of a
	for (int i=1; i<n1; i++)
		if(get_point(a, i).x > get_point(a, ia).x)
			ia = i;
	

	// ib -> leftmost point of b
	for (int i=1; i<n2; i++)
		if (get_point(b,i).x < get_point(b,ib).x)
			ib=i;
	

	// finding the upper tangent
	int inda = ia, indb = ib;
	bool done = 0;
	while (!done){
		done = 1;
		while (orientation(get_point(b, indb), get_point(a, inda), get_point(a, (inda+1)%n1)) >=0)
			inda = (inda + 1) % n1;
		

		while (orientation(get_point(a, inda), get_point(b, indb), get_point(b, (n2+indb-1)%n2)) <=0){
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
		while (orientation(get_point(a, inda), get_point(b, indb), get_point(b, (indb+1)%n2))>=0)
			indb=(indb+1)%n2;

		while (orientation(get_point(b, indb), get_point(a, inda), get_point(a, (n1+inda-1)%n1))<=0){
			inda=(n1+inda-1)%n1;
			done=0;
		}
	}


	int lowera = inda, lowerb = indb;
	VECTOR_INIT(ret);
    //point* ret = (point*)retV;
	//ret contains the convex hull after merging the two convex hulls
	//with the points sorted in anti-clockwise order
	int ind = uppera;
	ret.pfVectorAdd(&ret, a->vectorList.items[uppera]);
	while (ind != lowera){
		ind = (ind+1)%n1;
		ret.pfVectorAdd(&ret, a->vectorList.items[ind]);
	}

	ind = lowerb;
	ret.pfVectorAdd(&ret, b->vectorList.items[lowerb]);
	while (ind != upperb){
		ind = (ind+1)%n2;
		ret.pfVectorAdd(&ret, b->vectorList.items[ind]);
	}
	return ret;
}

vector bruteHull(vector *a)
{
	// Take any pair of points from the set and check
	// whether it is the edge of the convex hull or not.
	// if all the remaining points are on the same side
	// of the line then the line is the edge of convex
	// hull otherwise not
    //point* a = (point*)aV;
	VECTOR_INIT(ret);
	for (int i=0; i<a->vectorList.total; i++){
		for (int j=i+1; j<a->vectorList.total; j++){
			int x1 = get_point(a,i).x, x2 = get_point(a,j).x;
			int y1 = get_point(a,i).y, y2 = get_point(a,j).y;

			int a1 = y1-y2;
			int b1 = x2-x1;
			int c1 = x1*y2-y1*x2;
			int pos = 0, neg = 0;
			for (int k=0; k<a->vectorList.total; k++){
				if (a1*get_point(a,k).x+b1*get_point(a,k).y+c1 <= 0)
					neg++;
				if (a1*get_point(a,k).x+b1*get_point(a,k).y+c1 >= 0)
					pos++;
			}
			if (pos == a->vectorList.total || neg == a->vectorList.total){
				if(hasElement(&ret, get_point(a,i)) == -1){
					ret.pfVectorAdd(&ret,a->vectorList.items[i]);
				}
				if(hasElement(&ret, get_point(a,j)) == -1){
					ret.pfVectorAdd(&ret,a->vectorList.items[j]);
				}
				
			}
		}
	}
	// Sorting the points in the anti-clockwise order
	mid.x = 0;
	mid.y = 0;
	int n = ret.vectorList.total;
	for (int i=0; i<n; i++){
		mid.x += get_point(&ret,i).x;
		mid.y += get_point(&ret,i).y;
		(*(point*)ret.vectorList.items[i]).x = get_point(&ret,i).x * n;
		(*(point*)ret.vectorList.items[i]).y = get_point(&ret,i).y * n;
	}
	qsort(ret.vectorList.items, ret.vectorList.total, sizeof(*ret.vectorList.items), compare);
	for (int i=0; i<n; i++){
		(*(point*)ret.vectorList.items[i]).x = get_point(&ret,i).x / n;
		(*(point*)ret.vectorList.items[i]).y = get_point(&ret,i).y / n;
	}

	return ret;
}

vector divide(vector *a){
	// If the number of points is less than 6 then the
	// function uses the brute algorithm to find the
	// convex hull
	if (a->vectorList.total <= 5) return bruteHull(a);

	// left contains the left half points
	// right contains the right half points
	VECTOR_INIT(left);
	VECTOR_INIT(right);
	for (int i=0; i<a->vectorList.total/2; i++)
		left.pfVectorAdd(&left,a->vectorList.items[i]);
	for (int i=a->vectorList.total/2; i<a->vectorList.total; i++)
		right.pfVectorAdd(&right,a->vectorList.items[i]);

	// convex hull for the left and right sets
	VECTOR_INIT(left_hull);
	VECTOR_INIT(right_hull);
	left_hull = divide(&left);
	right_hull = divide(&right);

	// merging the convex hulls
	return merger(&left_hull, &right_hull);
}




