#include <stdio.h>
#include "../inc/sort.h"


Exception bubbleSort( Arr *array, sortOrder order) {
    int disorderCount = 0;
    elemPtr *element1, *element2; 

    for ( short iterCount = 0; iterCount < array->size; iterCount++ ) {
        for ( short elemIndex = 0; elemIndex < array->size - iterCount; elemIndex++ ) { 
            element1 = ( ( char * ) array->head + elemIndex * array->typeInfo->getSize() );
            element2 = ( ( char * ) array->head + ( elemIndex + 1 ) * array->typeInfo->getSize() );
            printf( "before swap:\n");
            array->typeInfo->print( *element1 );
            array->typeInfo->print( *element2 );

            if        ( array->typeInfo->compare( *element1, *element2 ) == LESS    && order == DESCENDING_ORDER ) {
                disorderCount++;
                array->typeInfo->swap( element1, element2 );
                printf( "after swap:\n");
                array->typeInfo->print( *element1 );
                array->typeInfo->print( *element2 );
            } else if ( array->typeInfo->compare( *element1, *element2 ) == GREATER && order == ASCENDING_ORDER ) {
                disorderCount++;
                array->typeInfo->swap( element1, element2 );
                printf( "after swap:\n");
                array->typeInfo->print( *element1 );
                array->typeInfo->print( *element2 );
            }
        }
        if ( disorderCount == 0 ) {
            break;
        }
    }

    return 0;
}


Exception heapify( Arr *array, int heapSize, sortOrder order )
{
    elemPtr root, left, right;
    
    for ( short index = heapSize / 2; index > 0; index-- ) {

        root = ( ( char * ) array->head +   index   * array->typeInfo->getSize() );
        left = ( ( char * ) array->head + 2 * index * array->typeInfo->getSize() );

        if ( 2 * index + 1 <= heapSize ) {

            right =( ( char * ) array->head + ( 2 * index  + 1 ) * array->typeInfo->getSize());

            if ( array->typeInfo->compare( root, array->typeInfo->maximum( left, right ) ) == GREATER && order == DESCENDING_ORDER ) {

                if ( array->typeInfo->compare( left, right ) == LESS || array->typeInfo->compare( left, right ) == EQUAL ) {
                    array->typeInfo->swap( &root, &left );
                }
                else {
                    array->typeInfo->swap( &root, &right );
                }
            } else if ( array->typeInfo->compare( root, array->typeInfo->maximum( left, right ) ) == LESS && order == ASCENDING_ORDER ) {

                if ( array->typeInfo->compare( left, right ) == GREATER || array->typeInfo->compare( left, right ) == EQUAL ) {
                    array->typeInfo->swap( &root, &left );
                }
                else {
                    array->typeInfo->swap( &root, &right );
                }
            }
        } else {

            if ( array->typeInfo->compare( root, left ) == GREATER && order == DESCENDING_ORDER ||
                 array->typeInfo->compare( root, left ) == LESS    && order == ASCENDING_ORDER ) {
                array->typeInfo->swap( &root, &left );
            }
        }
    }
    return SUCCESSFUL_EXECUTION;
}


Exception heapSort( Arr *array, sortOrder order ) {
    int heapSize = array->size;
    elemPtr newRoot, sifted;

    for ( short index = 0; index < array->size; index++ ) {
        heapify( array, heapSize, order );
        newRoot = ( char * ) array->head;
        sifted  = ( char * ) array->head + ( heapSize - 1 ) * array->typeInfo->getSize();

        array->typeInfo->swap( &newRoot, &sifted );
        heapSize--;
    }
    
    return SUCCESSFUL_EXECUTION;
}