#ifndef H_COLLECTION
#define H_COLLECTION

#include "typeinfo.h"
#include "util.h"


typedef struct _dynamicArray {
    int capacity;
    int size;
    ElemPtr *beginOfReserved; //rename
    ElemPtr *data;
    ElemPtr *endOfReserved;
    TypeInfo *typeInfo;
} DynamicArray;


typedef struct ArrayStorage {
    DynamicArray **arrays;
    int count;
} ArrayStorage;
ArrayStorage *getStorage();
Exception deleteStorage( ArrayStorage * );

Exception init( DynamicArray **, const TypeInfo *, const int ); 
Exception deleteArray( DynamicArray * );
Exception extend( DynamicArray *, const int );
Exception shrink( DynamicArray *, const int );
Exception readFromInput( DynamicArray *, const char *, const int );
Exception concatenate( DynamicArray **, const DynamicArray *, const DynamicArray * );
Exception map( DynamicArray *, unaryTransform );
Exception where( DynamicArray *, predicate );
Exception prepend( DynamicArray *, const ElemPtr * );               
Exception insertAt( DynamicArray *, const ElemPtr *, const int ); 
Exception getElem( DynamicArray *, const int, ElemPtr * ); 
Exception copyArray( DynamicArray *, const DynamicArray * );         
Exception append( DynamicArray *, const ElemPtr * );                
Exception addArrayToStorage( DynamicArray *, ArrayStorage * );      
Exception removeArrayFromStorage(DynamicArray *, ArrayStorage *);   


#endif