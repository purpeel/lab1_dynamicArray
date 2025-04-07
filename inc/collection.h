#ifndef H_COLLECTION
#define H_COLLECTION


#include "stringTypeinfo.h"
#include "doubleTypeinfo.h"
#include "typeinfo.h"
#include "util.h"


typedef struct _dynamicArray {
    int capacity;
    int size;
    elemPtr head;
    elemPtr tail; //should always point to the first byte after last element's end
    TypeInfo *TI;
} Arr;


typedef enum {
    EXTEND,
    SHRINK
} resizeType;


Exception init( Arr **, const TypeInfo * ); 
int readFromInput( Arr *, const char *, const int );
Exception append( Arr *, const elemPtr );
int prepend( Arr *, const elemPtr );
int pop( Arr *, const int, elemPtr );
int slice( const Arr *, Arr *, const int, const int );
int delete( Arr * );
int map( Arr *, unaryOperator );
int where( Arr *, predicate );
int getElem( Arr, const int );
Exception addArrayToStorage( Arr * );


#endif