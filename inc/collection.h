#ifndef H_COLLECTION
#define H_COLLECTION


#include "stringTypeinfo.h"
#include "doubleTypeinfo.h"
#include "typeinfo.h"
#include "util.h"


typedef struct _dynamicArray {
    int capacity;
    int size;
    elemPtr *begin;
    elemPtr *head;
    elemPtr *tail;
    TypeInfo *typeInfo;
} DynamicArray;


typedef struct ArrayStorage {
    DynamicArray **arrayPtrs;
    int count;
} ArrayStorage;


ArrayStorage *getStorage();


typedef enum {
    EXTEND,
    SHRINK
} resizeType;


Exception init( DynamicArray **, const TypeInfo *, const int ); 
Exception delete( DynamicArray * );
Exception readFromInput( DynamicArray *, const char *, const int );
int pop( DynamicArray *, const int, elemPtr );
int slice( DynamicArray *, const DynamicArray *, const int, const int );
Exception concatenate( DynamicArray **, const DynamicArray *, const DynamicArray * );
Exception map( DynamicArray *, unaryOperator );
Exception where( DynamicArray *, predicate );
Exception prepend( DynamicArray *, const elemPtr * );                //^ maybe shouldn't be here
Exception pushIndex( DynamicArray *, const elemPtr *, const int );   //^ maybe shouldn't be here
Exception copyArray( DynamicArray *, const DynamicArray * );         //^ maybe shouldn't be here 
Exception append( DynamicArray *, const elemPtr * );                 //^ maybe shouldn't be here
Exception addArrayToStorage( DynamicArray *, ArrayStorage * );       //^ maybe shouldn't be here


#endif