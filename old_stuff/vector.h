// Reference for vector implementation in C: https://aticleworld.com/implement-vector-in-c/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VECTOR_INIT_CAPACITY 100
#define UNDEFINE  -1
#define SUCCESS 0
#define VECTOR_INIT(vec) vector vec;\
 vector_init(&vec)

//Store and track the stored data
typedef struct sVectorList
{
    void **items;
    int capacity;
    int total;
} sVectorList;

//structure contain the function pointer
typedef struct sVector vector;
struct sVector
{
    sVectorList vectorList;
    //function pointers
    int (*pfVectorTotal)(vector *);
    int (*pfVectorResize)(vector *, int);
    int (*pfVectorAdd)(vector *, void *);
    int (*pfVectorSet)(vector *, int, void *);
    void *(*pfVectorGet)(vector *, int);
    int (*pfVectorDelete)(vector *, int);
    int (*pfVectorFree)(vector *);
};

int vectorTotal(vector *v){
    int totalCount = UNDEFINE;
    if(v)
    {
        totalCount = v->vectorList.total;
    }
    return totalCount;
}

int vectorResize(vector *v, int capacity){
    int status = UNDEFINE;
    if(v)
    {
        void **items = realloc(v->vectorList.items, sizeof(void *) * capacity);
        if (items)
        {
            v->vectorList.items = items;
            v->vectorList.capacity = capacity;
            status = SUCCESS;
        }
    }
    return status;
}

int vectorPushBack(vector *v, void *item){
    int status = UNDEFINE;
    if(v)
    {
        if (v->vectorList.capacity == v->vectorList.total)
        {
            status = vectorResize(v, v->vectorList.capacity * 2);
            if(status != UNDEFINE)
            {
                v->vectorList.items[v->vectorList.total++] = item;
            }
        }
        else
        {
            v->vectorList.items[v->vectorList.total++] = item;
            status = SUCCESS;
        }
    }
    return status;
}

int vectorSet(vector *v, int index, void *item){
    int status = UNDEFINE;
    if(v)
    {
        if ((index >= 0) && (index < v->vectorList.total))
        {
            v->vectorList.items[index] = item;
            status = SUCCESS;
        }
    }
    return status;
}

void *vectorGet(vector *v, int index){
    void *readData = NULL;
    if(v)
    {
        if ((index >= 0) && (index < v->vectorList.total))
        {
            readData = v->vectorList.items[index];
        }
    }
    return readData;
}

int vectorDelete(vector *v, int index){
    int status = UNDEFINE;
    int i = 0;
    if(v)
    {
        if ((index < 0) || (index >= v->vectorList.total))
            return status;
        v->vectorList.items[index] = NULL;
        for (i = index; (i < v->vectorList.total - 1); ++i)
        {
            v->vectorList.items[i] = v->vectorList.items[i + 1];
            v->vectorList.items[i + 1] = NULL;
        }
        v->vectorList.total--;
        if ((v->vectorList.total > 0) && ((v->vectorList.total) == (v->vectorList.capacity / 4)))
        {
            vectorResize(v, v->vectorList.capacity / 2);
        }
        status = SUCCESS;
    }
    return status;
}

int vectorFree(vector *v){
    int status = UNDEFINE;
    if(v)
    {
        free(v->vectorList.items);
        v->vectorList.items = NULL;
        status = SUCCESS;
    }
    return status;
}

void vector_init(vector *v){
    //init function pointers
    v->pfVectorTotal = vectorTotal;
    v->pfVectorResize = vectorResize;
    v->pfVectorAdd = vectorPushBack;
    v->pfVectorSet = vectorSet;
    v->pfVectorGet = vectorGet;
    v->pfVectorFree = vectorFree;
    v->pfVectorDelete = vectorDelete;
    //initialize the capacity and allocate the memory
    v->vectorList.capacity = VECTOR_INIT_CAPACITY;
    v->vectorList.total = 0;
    v->vectorList.items = malloc(sizeof(void *) * v->vectorList.capacity);
}
