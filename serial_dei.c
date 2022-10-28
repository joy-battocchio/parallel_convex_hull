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

int quad(point p){
    if(p.x >= 0 && p.y >= 0) return 1;
    if(p.x <= 0 && p.y >= 0) return 2;
	if(p.x <= 0 && p.y <= 0) return 3;
    return 4;
}

int orientation(point a, point b, point c){
	int res = (b.y-a.y)*(c.x-b.x) - (c.y-b.y)*(b.x-a.x);
	if (res == 0) return 0;
	if (res > 0) return 1;
	return -1;
}

//compare the points for the sort function by polar angle
bool compare(point p1, point q1)
{
	point p = {.x = p1.x - mid.x, .y = p1.y - mid.y};
    point q = {.x = q1.x - mid.x, .y = q1.y - mid.y};

	int one = quad(p);
	int two = quad(q);

	if (one != two) return (one < two);
	return (p.y*q.x < q.y*p.x);
}

vector merger(vector *aV, vector *bV)
{
	// n1 -> number of points in polygon a
	// n2 -> number of points in polygon b
    point* a = (point*)aV;
    point* b = (point*)bV;

	int n1 = aV->pfVectorTotal(&aV), n2 = bV->pfVectorTotal(&bV);

	int ia = 0, ib = 0;

    // ia -> rightmost point of a
	for (int i=1; i<n1; i++)
		if (a[i].x > a[ia].x)
			ia = i;

	// ib -> leftmost point of b
	for (int i=1; i<n2; i++)
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
	int g = 0;
	while (!done)//finding the lower tangent
	{
		done = 1;
		while (orientation(a[inda], b[indb], b[(indb+1)%n2])>=0)
			indb=(indb+1)%n2;

		while (orientation(b[indb], a[inda], a[(n1+inda-1)%n1])<=0)
		{
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
	ret.pfVectorAdd(&ret, &a[uppera]);
	while (ind != lowera)
	{
		ind = (ind+1)%n1;
		ret.pfVectorAdd(&ret, &a[ind]);
	}

	ind = lowerb;
	ret.pfVectorAdd(&ret, &b[lowerb]);
	while (ind != upperb)
	{
		ind = (ind+1)%n2;
		ret.pfVectorAdd(&ret, &b[ind]);
	}
	return ret;

}

// vector bruteHull(vector *aV)
// {
// 	// Take any pair of points from the set and check
// 	// whether it is the edge of the convex hull or not.
// 	// if all the remaining points are on the same side
// 	// of the line then the line is the edge of convex
// 	// hull otherwise not
//     point* a = (point*)aV;
// 	set<pair<int, int> >s;

// 	for (int i=0; i<a.size(); i++)
// 	{
// 		for (int j=i+1; j<a.size(); j++)
// 		{
// 			int x1 = a[i].first, x2 = a[j].first;
// 			int y1 = a[i].second, y2 = a[j].second;

// 			int a1 = y1-y2;
// 			int b1 = x2-x1;
// 			int c1 = x1*y2-y1*x2;
// 			int pos = 0, neg = 0;
// 			for (int k=0; k<a.size(); k++)
// 			{
// 				if (a1*a[k].first+b1*a[k].second+c1 <= 0)
// 					neg++;
// 				if (a1*a[k].first+b1*a[k].second+c1 >= 0)
// 					pos++;
// 			}
// 			if (pos == a.size() || neg == a.size())
// 			{
// 				s.insert(a[i]);
// 				s.insert(a[j]);
// 			}
// 		}
// 	}

// 	vector<pair<int, int>>ret;
// 	for (auto e:s)
// 		ret.push_back(e);

// 	// Sorting the points in the anti-clockwise order
// 	mid = {0, 0};
// 	int n = ret.size();
// 	for (int i=0; i<n; i++)
// 	{
// 		mid.first += ret[i].first;
// 		mid.second += ret[i].second;
// 		ret[i].first *= n;
// 		ret[i].second *= n;
// 	}
// 	sort(ret.begin(), ret.end(), compare);
// 	for (int i=0; i<n; i++)
// 		ret[i] = make_pair(ret[i].first/n, ret[i].second/n);

// 	return ret;
// }


int main(int argc, char *argv[]){
    


    return 0;
}