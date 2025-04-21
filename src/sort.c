#include <stdio.h>
#include <stdbool.h>
#include "../inc/sort.h"


Exception bubbleSort( DynamicArray *array, sortOrder order) {
    int disorderCount;
    ElemPtr *element1, *element2; 

    for ( short iterCount = 0; iterCount < array->size; iterCount++ ) {
        disorderCount = 0;
        for ( short elemIndex = 0; elemIndex < array->size - iterCount - 1; elemIndex++ ) { 
            element1 = ( ElemPtr * ) ( ( char * ) array->data +     elemIndex     * array->typeInfo->getSize() );
            element2 = ( ElemPtr * ) ( ( char * ) array->data + ( elemIndex + 1 ) * array->typeInfo->getSize() );

            if        ( array->typeInfo->compare( *element1, *element2 ) == LESS    && order == DESCENDING_ORDER ) {
                disorderCount++;
                array->typeInfo->swap( element1, element2 );
            } else if ( array->typeInfo->compare( *element1, *element2 ) == GREATER && order == ASCENDING_ORDER ) {
                disorderCount++;
                array->typeInfo->swap( element1, element2 );
            }
        }
        if ( disorderCount == 0 ) {
            break;
        }
    }

    return 0;
}


bool inOrder( DynamicArray *array, ElemPtr elem1, ElemPtr elem2, sortOrder order ) {
    return ( order == ASCENDING_ORDER )
        ? ( array->typeInfo->compare( elem1, elem2 ) == LESS )
        : ( array->typeInfo->compare( elem1, elem2 ) == GREATER );
}


Exception sift( DynamicArray *array, int heapSize, int index, sortOrder order ) {

    ElemPtr *root, *left, *right, *extr;
    int extrIndex = index;
    bool InOrder;

    root = ( ElemPtr * ) ( ( char * ) array->data + index * array->typeInfo->getSize() );
    extr = ( ElemPtr * ) ( ( char * ) array->data + extrIndex * array->typeInfo->getSize() );

    if ( 2 * index + 1 < heapSize ) {
        left = ( ElemPtr * ) ( ( char * ) array->data + ( 2 * index + 1 ) * array->typeInfo->getSize() );
        
        if ( !inOrder(array, *left, *extr, order) ) { 
            extrIndex = 2 * index + 1;
            extr = ( ElemPtr * ) ( ( char * ) array->data + extrIndex * array->typeInfo->getSize() );
        }
    }

    if ( 2 * index + 2 < heapSize ) {
        right = ( ElemPtr * ) ( ( char * ) array->data + ( 2 * index + 2 ) * array->typeInfo->getSize() );

        if ( !inOrder(array, *right, *extr, order) ) {
            extrIndex = 2 * index + 2;
            extr = ( ElemPtr * ) ( ( char * ) array->data + extrIndex * array->typeInfo->getSize() );
        }
    }

    if ( extrIndex != index ) {
        extr = ( ElemPtr * ) ( ( char * ) array->data + extrIndex * array->typeInfo->getSize() );
        array->typeInfo->swap( root, extr );
        sift( array, heapSize, extrIndex, order );
    }

    return SUCCESSFUL_EXECUTION;
}


Exception buildHeap( DynamicArray *array, int heapSize, sortOrder order )
{

    for ( short int index = heapSize / 2 - 1; index >= 0; index-- ) {
        sift( array, heapSize, index, order );
    }
    
    return SUCCESSFUL_EXECUTION;
}


Exception heapSort( DynamicArray *array, sortOrder order ) {
    int heapSize = array->size;
    ElemPtr *newRoot, *lastLeaf;

    buildHeap( array, heapSize, order );

    for ( short index = 0; index < array->size; index++ ) {
        newRoot = array->data;
        lastLeaf = ( ElemPtr * ) ( ( char * ) array->data + ( heapSize - 1 ) * array->typeInfo->getSize() );
        array->typeInfo->swap( newRoot, lastLeaf );
        heapSize--;
        sift( array, heapSize, 0, order );
    }
    
    return SUCCESSFUL_EXECUTION;
}
