#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/collection.h"


static ArrayStorage *storage = NULL;


ArrayStorage *getStorage() {
    if ( storage == NULL ) {
        storage = malloc( sizeof( ArrayStorage ) );
        if ( storage == NULL ) {
            return NULL;
        }
        storage->count = 0;
        storage->arrayPtrs = NULL;
    }
    return storage;
}


Exception delete( DynamicArray *array ) {
    if ( array == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for ( unsigned long index = 0; index < array->size; index++ ) {
        array->typeInfo->destruct( ( char * ) array->begin + index * array->typeInfo->getSize() );
    }

    free( array->begin );
    free( array );

    return SUCCESSFUL_EXECUTION;

}


Exception addArrayToStorage( DynamicArray *array, ArrayStorage *storage ) {
    storage->count++;
    storage->arrayPtrs = realloc( storage->arrayPtrs, storage->count * sizeof( DynamicArray * ) );
    if ( storage->arrayPtrs == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    storage->arrayPtrs[storage->count - 1] = array;
    return SUCCESSFUL_EXECUTION;
}


Exception init( DynamicArray **array, const TypeInfo *TI, const int initialCapacity ) {
    ( *array ) = malloc( sizeof( DynamicArray ) );
    if ( ( *array ) == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    ( *array )->typeInfo = TI;


    ( *array )->capacity = initialCapacity;
    ( *array )->size = 0;

    ( *array )->begin = malloc( ( *array )->typeInfo->getSize() * ( *array )->capacity );
    if ( ( *array )->begin == NULL ) {
        free( ( *array ) );
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    ( *array )->head = ( elemPtr * ) ( ( char * ) ( *array )->begin + ( *array )->typeInfo->getSize() );
    ( *array )->tail = ( elemPtr * ) ( ( char * ) ( *array )->begin + ( *array )->capacity * ( *array )->typeInfo->getSize() );

    Exception storingStatus = addArrayToStorage( *array, getStorage() );
    if ( storingStatus != SUCCESSFUL_EXECUTION ) {
        return storingStatus;
    }
    return SUCCESSFUL_EXECUTION;
}


Exception resize( DynamicArray *array, resizeType directive ) {
    elemPtr *buffer;

    switch ( directive )  
    {
    case EXTEND:
        if ( array->size >= ( array->capacity * 0.75 ) && array->capacity < 10000 ) {
            buffer = ( elemPtr *) malloc( ( array->size - 1 ) * array->typeInfo->getSize() );
            if ( buffer == NULL ) {
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->capacity = array->capacity * 2 + 1;
                memmove( buffer, array->head, array->typeInfo->getSize() * ( array->size - 1 ) );
                array->begin = ( elemPtr * ) realloc( array->begin, array->capacity * array->typeInfo->getSize() );
                if ( array->begin == NULL ) {
                    free( buffer );
                    return ARRAY_DATA_ALLOCATION_ERROR;
                }
                array->head = ( elemPtr * ) ( ( char * ) array->begin + ( 1 + array->capacity / 4 ) * array->typeInfo->getSize() );
                array->tail = ( elemPtr * ) ( ( char * ) array->begin +       array->capacity       * array->typeInfo->getSize() );
                memmove( array->head, buffer, array->typeInfo->getSize() * ( array->size - 1 ) );
            }
        } else if ( array->size > ( array->capacity * 0.25 ) && array->capacity > 10000 ) {
            buffer = ( elemPtr * ) malloc( ( array->size - 1 ) * array->typeInfo->getSize() );
            if ( buffer == NULL ) {
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->capacity += 1000;
                memmove( buffer, array->head, array->typeInfo->getSize() * ( array->size - 1 ) );
                array->begin = ( elemPtr * ) realloc( array->begin, array->capacity * array->typeInfo->getSize() );
                if ( array->begin == NULL ) {
                    free( buffer );
                    return ARRAY_DATA_ALLOCATION_ERROR;
                }
                array->head = ( elemPtr * ) ( ( char * ) array->begin +      500        * array->typeInfo->getSize() );
                array->tail = ( elemPtr * ) ( ( char * ) array->begin + array->capacity * array->typeInfo->getSize() );
                memmove( array->head, buffer, array->typeInfo->getSize() * ( array->size - 1 ) );
            }
        }
        break;
    case SHRINK:
        if ( array->size < ( array->capacity * 0.25 ) && array->capacity < 10000 ) {
            buffer = ( elemPtr * ) malloc( ( array->size + 1 ) * array->typeInfo->getSize() );
            if ( buffer == NULL ) {
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                printf( "%p\n", buffer );
                array->capacity = array->capacity / 2 + 1;
                memmove( buffer, array->head, array->typeInfo->getSize() * ( array->size + 1 ) );
                array->begin = ( elemPtr * ) realloc( array->begin, array->capacity * array->typeInfo->getSize() );
                if ( array->begin == NULL ) {
                    free( buffer );
                    return ARRAY_DATA_ALLOCATION_ERROR;
                }
                array->head = ( elemPtr * ) ( ( char * ) array->begin + ( 1 + array->capacity / 8 ) * array->typeInfo->getSize() );
                array->tail = ( elemPtr * ) ( ( char * ) array->begin +       array->capacity       * array->typeInfo->getSize() );
                memmove( array->head, buffer, array->typeInfo->getSize() * ( array->size + 1 ) );
            }
        } else if ( array->size < ( array->capacity * 0.75 ) && array->capacity > 10000 ) {
            buffer = ( elemPtr * ) malloc( ( array->size + 1 ) * array->typeInfo->getSize() );
            if ( buffer == NULL ) {
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->capacity -= 1000;
                memmove( buffer, array->head, array->typeInfo->getSize() * ( array->size + 1 ) );
                array->begin = ( elemPtr * ) realloc( array->begin, array->capacity * array->typeInfo->getSize() );
                if ( array->begin == NULL ) {
                    free( buffer );
                    return ARRAY_DATA_ALLOCATION_ERROR;
                }
                array->head = ( elemPtr * ) ( ( char * ) array->begin +       500       * array->typeInfo->getSize() );
                array->tail = ( elemPtr * ) ( ( char * ) array->begin + array->capacity * array->typeInfo->getSize() );
                memmove( array->head, buffer, array->typeInfo->getSize() * ( array->size + 1 ) );
            }
        }
        break;
    }

    printf("size: %d\t capacity: %d\n", array->size, array->capacity );
    printf( "begin: %p\t head: %p\t tail: %p\n", array->begin, array->head, array->tail );
    return SUCCESSFUL_EXECUTION;
}



Exception append( DynamicArray *array, const elemPtr *element ) {

    array->size++;

    if ( resize( array, EXTEND ) == SUCCESSFUL_EXECUTION ) {
        if ( array->head == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }

        elemPtr *placing = ( elemPtr * ) ( ( char * ) array->head + ( array->size - 1 ) * array->typeInfo->getSize() );
        Exception assigningStatus = array->typeInfo->assign( placing, *element );
        if ( assigningStatus != SUCCESSFUL_EXECUTION ) {
            return assigningStatus;
        }

    } else { 
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    return SUCCESSFUL_EXECUTION;
}


Exception prepend( DynamicArray *array, const elemPtr *element ) {
    array->size++;

    if ( resize( array, EXTEND ) == SUCCESSFUL_EXECUTION ) {
        printf( "resize done.\n");
        if ( array->head == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }
        array->head = ( elemPtr * ) ( ( char * ) array->head - array->typeInfo->getSize() );

        elemPtr *placing = array->head;
        Exception assigningStatus = array->typeInfo->assign( placing, *element );
        if ( assigningStatus != SUCCESSFUL_EXECUTION ) {
            return assigningStatus;
        }

    } else { 
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    return SUCCESSFUL_EXECUTION;
}


Exception pushIndex( DynamicArray *array, const elemPtr *element, const int index ) {
    array->size++;

    if ( resize( array, EXTEND ) == SUCCESSFUL_EXECUTION ) {
        if ( array->head == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }
        array->tail = ( elemPtr * ) ( ( char * ) array->head + array->size * array->typeInfo->getSize() );


        elemPtr *placing = ( elemPtr * ) ( ( char * ) array->head + index * array->typeInfo->getSize() );
        memmove( placing + array->typeInfo->getSize(), placing, array->size - index + 1 );
        Exception assigningStatus = array->typeInfo->assign( placing, *element );
        if ( assigningStatus != SUCCESSFUL_EXECUTION ) {
            return assigningStatus;
        }
        

    } else {
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    return SUCCESSFUL_EXECUTION;
}


Exception copyArray( DynamicArray *destination, const DynamicArray *source ) {
    elemPtr *sourceElem;

    for ( short index = 0; index < source->size; index++ ) {
        sourceElem = ( elemPtr * ) ( ( char * ) source->head + index * source->typeInfo->getSize() );
        source->typeInfo->print( *sourceElem );
        
        Exception appendStatus = append( destination, sourceElem );
        if ( appendStatus != SUCCESSFUL_EXECUTION ) {
            return appendStatus;
        }
    }

    return SUCCESSFUL_EXECUTION;
}


Exception concatenate( DynamicArray **result, const DynamicArray *array1, const DynamicArray *array2 ) {
    if ( array1->typeInfo != array2->typeInfo ) {
        return ARRAYS_TYPEINFO_MISMATCH_ERROR;
    }

    Exception initStatus = init( result, array1->typeInfo, array1->capacity + array2->capacity );
    if ( initStatus != SUCCESSFUL_EXECUTION ) {
        return initStatus;
    }
    
    Exception copyStatus = copyArray( *result, array1 );    
    if ( copyStatus != SUCCESSFUL_EXECUTION ) {
        return copyStatus;
    }
    copyStatus = copyArray( *result, array2 );
    if ( copyStatus != SUCCESSFUL_EXECUTION ) {
        return copyStatus;
    }

    return SUCCESSFUL_EXECUTION;
}


Exception map( DynamicArray *array, unaryOperator func ) {
    for ( short index = 0; index < array->size; index++ ) {
        elemPtr *elem = ( elemPtr * ) ( ( char * ) array->head + index * array->typeInfo->getSize() );
        func( *elem );
    }
    
    return SUCCESSFUL_EXECUTION;
}


Exception readFromInput( DynamicArray *array, const char *input, const int length ) {
    char *buffer = NULL;
    int prevIsSpace = 1, bufferLength = 0, resBufferLength = 0, bufferRecordedCount = 0, readElementsCount = 0, isInQuotes = 0;
    elemPtr *newElem;

    for ( short index = 0; index < length; index++ ) {

        if ( ( *( input + index ) == ' '  ||  *( input + index ) == '\n' ) && isInQuotes == 0 ) {
            if ( prevIsSpace == 0 && readElementsCount != 0 ) {
                *( buffer + resBufferLength ) = '\0';

                newElem = malloc( array->typeInfo->getSize() );
                Exception inputStatus = array->typeInfo->input( newElem, buffer ); 
                if ( inputStatus != SUCCESSFUL_EXECUTION ) {
                    free( newElem );
                    free( buffer );
                    return inputStatus;
                }
                Exception appendStatus = append( array, newElem );
                if ( appendStatus != SUCCESSFUL_EXECUTION ) {
                    free( newElem );
                    free( buffer );
                    return appendStatus;
                }

                free( newElem );
                free( buffer );

                bufferLength = 0;
                bufferRecordedCount = 0;
                isInQuotes = 0;
            }
            prevIsSpace = 1;
            continue;

        } else if ( *( input + index ) == '\"' && isInQuotes == 0 && prevIsSpace == 1 ) {
            isInQuotes = 1;
            for ( short innerIndex = index + 1; innerIndex < length; innerIndex++ ) {

                if ( *( input + innerIndex ) == '\"' || *( input + innerIndex ) == '\n' ) {
                    break;
                }
                bufferLength++;
            }

        } else {

            if ( prevIsSpace == 1 ) {
                prevIsSpace = 0;

                if ( isInQuotes == 0 ) {
                    for ( short innerIndex = index; innerIndex < length; innerIndex++ ) {
                        
                        if ( *( input + innerIndex ) == ' ' || *( input + innerIndex ) == '\n' ) {
                            break;
                        }

                        bufferLength++;
                    }
                }

                buffer = malloc( bufferLength + 1 );
                resBufferLength = bufferLength;
                if ( buffer == NULL ) {
                    return MEMORY_ALLOCATION_ERROR;
                }
            }

            *( buffer + bufferRecordedCount ) = *( input + index );
            bufferRecordedCount++;
            readElementsCount++;

            if ( bufferLength == 0 && isInQuotes == 1 ) {
                isInQuotes = 0;
            }
            bufferLength--;
        }
    }
    if ( buffer ) {
        free( buffer );
    }

    return SUCCESSFUL_EXECUTION;
}