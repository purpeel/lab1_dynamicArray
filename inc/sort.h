#ifndef H_SORT
#define H_SORT


#include "collection.h"
#include "util.h"


typedef enum {
    ASCENDING_ORDER,
    DESCENDING_ORDER
} sortOrder;


Exception bubbleSort( Arr *, sortOrder );
Exception heapSort( Arr *, sortOrder );


#endif