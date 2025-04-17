#ifndef H_SORT
#define H_SORT


#include "collection.h"
#include "util.h"

typedef enum {
    ASCENDING_ORDER,
    DESCENDING_ORDER
} sortOrder;


Exception bubbleSort( DynamicArray *, sortOrder );
Exception heapSort( DynamicArray *, sortOrder );


#endif