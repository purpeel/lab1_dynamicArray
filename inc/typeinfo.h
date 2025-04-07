#ifndef H_TYPEINFO
#define H_TYPEINFO


#include <stdbool.h>
#include "util.h"


typedef void *elemPtr;  


typedef int ( *binaryOperator )( const elemPtr, const elemPtr, elemPtr );
typedef int ( *compareOperator )( const elemPtr, const elemPtr );
typedef int ( *unaryOperator )( elemPtr );
typedef int ( *copyConstructor )( elemPtr *, const elemPtr );
typedef size_t ( *memoryOperator )();
typedef void ( *printOperator )( const elemPtr );
typedef int ( *inputOperator )( elemPtr *, const char * );
typedef bool predicate( const elemPtr );


typedef struct {
    inputOperator input;

    copyConstructor assign;
    compareOperator compare;

    memoryOperator getSize;
    unaryOperator destruct;

    printOperator print;
} TypeInfo;


#endif