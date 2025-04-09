#ifndef H_TYPEINFO
#define H_TYPEINFO


#include <stdbool.h>
#include "util.h"


typedef void *elemPtr;  


typedef Exception ( *binaryOperator )( const elemPtr, const elemPtr, elemPtr );
typedef Exception ( *compareOperator )( const elemPtr, const elemPtr );
typedef Exception ( *unaryOperator )( elemPtr );
typedef Exception ( *copyConstructor )( elemPtr *, const elemPtr );
typedef size_t ( *memoryOperator )();
typedef void ( *printOperator )( const elemPtr );
typedef Exception ( *inputOperator )( elemPtr *, const char * );
typedef bool ( *predicate )( const elemPtr );


typedef struct {
    const char *typeName;
    inputOperator input;

    copyConstructor assign;
    compareOperator compare;

    memoryOperator getSize;
    unaryOperator destruct;

    printOperator print;

    predicate setForWhere[3];
    unaryOperator setForMap[3];
} TypeInfo;


#endif