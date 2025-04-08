#ifndef H_COLLECTION
#define H_COLLECTION


#include "stringTypeinfo.h"
#include "doubleTypeinfo.h"
#include "typeinfo.h"
#include "util.h"


typedef struct _dynamicArray {
    int capacity;
    int size;
    elemPtr begin;
    elemPtr head;
    elemPtr tail;
    TypeInfo *typeInfo;
} Arr;


typedef enum {
    EXTEND,
    SHRINK
} resizeType;


Exception init( Arr **, const TypeInfo * ); 
Exception readFromInput( Arr *, const char *, const int );
Exception append( Arr *, const elemPtr );
Exception prepend( Arr *, const elemPtr );
int pop( Arr *, const int, elemPtr );
int slice( const Arr *, Arr *, const int, const int );
int delete( Arr * );
int map( Arr *, unaryOperator );
int where( Arr *, predicate );
int getElem( Arr, const int );
Exception addArrayToStorage( Arr * );


#endif