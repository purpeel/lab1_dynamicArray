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


typedef struct ArrayStorage {
    Arr **arrayPtrs;
    int count;
} ArrayStorage;


ArrayStorage *getStorage();


typedef enum {
    EXTEND,
    SHRINK
} resizeType;


Exception init( Arr **, const TypeInfo * ); 
Exception readFromInput( Arr *, const char *, const int );
Exception append( Arr *, const elemPtr );
Exception prepend( Arr *, const elemPtr );
Exception indexPush( Arr *, const elemPtr, const int );
int pop( Arr *, const int, elemPtr );
int slice( const Arr *, Arr *, const int, const int );
int delete( Arr * );
Exception map( Arr *, unaryOperator );
Exception where( Arr *, predicate );
Exception getElem( Arr, const int );
Exception addArrayToStorage( Arr *, ArrayStorage * );


#endif