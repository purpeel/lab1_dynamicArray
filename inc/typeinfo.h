#ifndef H_TYPEINFO
#define H_TYPEINFO

#include <stdbool.h>
#include "util.h"

typedef enum _comparisonResult {
    GREATER = -1,
    EQUAL = 0,
    LESS = 1
} ComparisonResult;

typedef void *ElemPtr;

typedef Exception ( *binaryOperator )( const ElemPtr, const ElemPtr, ElemPtr );
typedef Exception ( *swapOperator )( ElemPtr *, ElemPtr * );
typedef Exception ( *unaryTransform )( ElemPtr );
typedef Exception ( *copyConstructor )( ElemPtr *, const ElemPtr );

typedef ComparisonResult ( *compareOperator )( const ElemPtr, const ElemPtr );
typedef ElemPtr ( *selector )( const ElemPtr, const ElemPtr );

typedef size_t ( *sizeGetter )();

typedef void ( *printer )( const ElemPtr );
typedef Exception ( *charToElem )( ElemPtr *, const char * );

typedef bool ( *predicate )( const ElemPtr );

typedef struct {
    const char *typeName;
    swapOperator swap;

    copyConstructor assign;
    compareOperator compare;
    selector maximum;
    selector minimum;

    sizeGetter getSize;
    unaryTransform destruct; // разделить на деструктор элемента и высвобождение памяти под него 
// free(arg) необязателен внутри реализации типа, этим можно озаботить внешние конструкторы
    printer print;
    charToElem input;

    predicate setForWhere[3];
    unaryTransform setForMap[3];
} TypeInfo;


#endif