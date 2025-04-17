#include <stdio.h>
#include <stdbool.h>
#include "../inc/sort.h"


Exception bubbleSort( DynamicArray *array, sortOrder order) {
    int disorderCount;
    elemPtr *element1, *element2; 

    for ( short iterCount = 0; iterCount < array->size; iterCount++ ) {
        disorderCount = 0;
        for ( short elemIndex = 0; elemIndex < array->size - iterCount - 1; elemIndex++ ) { 
            element1 = ( elemPtr * ) ( ( char * ) array->head +     elemIndex     * array->typeInfo->getSize() );
            element2 = ( elemPtr * ) ( ( char * ) array->head + ( elemIndex + 1 ) * array->typeInfo->getSize() );

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


Exception sift( DynamicArray *array, int heapSize, int index, sortOrder order ) {

    elemPtr *root, *left, *right, *extr;
    int extrIndex = index;
    bool InOrder;

    root = ( elemPtr * ) ( ( char * ) array->head + index * array->typeInfo->getSize() );
    extr = ( elemPtr * ) ( ( char * ) array->head + extrIndex * array->typeInfo->getSize() );

    if ( 2 * index + 1 < heapSize ) {
        left = ( elemPtr * ) ( ( char * ) array->head + ( 2 * index + 1 ) * array->typeInfo->getSize() );
        
        InOrder = ( order == ASCENDING_ORDER ) 
            ? ( array->typeInfo->compare( *left, *extr ) == LESS )
            : ( array->typeInfo->compare( *left, *extr ) == GREATER );
        
        if ( !InOrder ) {
            extrIndex = 2 * index + 1;
            extr = ( elemPtr * ) ( ( char * ) array->head + extrIndex * array->typeInfo->getSize() );
        }
    }

    if ( 2 * index + 2 < heapSize ) {
        right = ( elemPtr * ) ( ( char * ) array->head + ( 2 * index + 2 ) * array->typeInfo->getSize() );

        InOrder = ( order == ASCENDING_ORDER ) 
            ? ( array->typeInfo->compare( *right, *extr ) == LESS )
            : ( array->typeInfo->compare( *right, *extr ) == GREATER );

        if ( !InOrder ) {
            extrIndex = 2 * index + 2;
            extr = ( elemPtr * ) ( ( char * ) array->head + extrIndex * array->typeInfo->getSize() );
        }
    }

    if ( extrIndex != index ) {
        extr = ( elemPtr * ) ( ( char * ) array->head + extrIndex * array->typeInfo->getSize() );
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
    elemPtr *newRoot, *lastLeaf;

    buildHeap( array, heapSize, order );

    for ( short index = 0; index < array->size; index++ ) {
        newRoot = array->head;
        lastLeaf = ( elemPtr * ) ( ( char * ) array->head + ( heapSize - 1 ) * array->typeInfo->getSize() );
        array->typeInfo->swap( newRoot, lastLeaf );
        heapSize--;
        sift( array, heapSize, 0, order );
    }
    
    return SUCCESSFUL_EXECUTION;
}
