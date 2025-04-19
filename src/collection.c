#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/collection.h"


static ArrayStorage *storage = NULL;

/// @brief function that instantiate global const object storage at the first time it's being called
/// @return pointer to storage
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

/// @brief function clearing all data related to storage
/// @param storage current pointer to global storage
/// @return status of execution depending on inner calls
Exception deleteStorage( ArrayStorage *storage ) {
    if ( storage->count == 0 ) {
        free( storage->arrayPtrs );
        free( storage );
        return SUCCESSFUL_EXECUTION;
    }
    for ( short index = 0; index < storage->count; index++ ) {
        Exception removalStatus = removeArrayFromStorage( storage->arrayPtrs[index], storage );
        if ( removalStatus != SUCCESSFUL_EXECUTION ) {
            return removalStatus;
        }
    }
    free( storage );
    return SUCCESSFUL_EXECUTION;
}

/// @brief removing array from storage by consecutively swapping its adress with the next one until the end and then reallocating data without last array's address
/// @param array pointer to array ought to be removed
/// @param storage current pointer to global storage
/// @return status of execution depending on inner calls
Exception removeArrayFromStorage( DynamicArray *array, ArrayStorage *storage ) {
    DynamicArray *temp;
    for ( short index = 0; index < storage->count - 1; index++ ) {
        if ( storage->arrayPtrs[index] == array ) {
            temp = storage->arrayPtrs[index + 1];
            storage->arrayPtrs[index + 1] = storage->arrayPtrs[index];
            storage->arrayPtrs[index] = temp;
        }
    }
    storage->count--;
    if ( storage->count == 0 ) {
        free(storage->arrayPtrs);
        storage->arrayPtrs = NULL;
        return SUCCESSFUL_EXECUTION;
    } else {
        storage->arrayPtrs = realloc( storage->arrayPtrs, storage->count * sizeof( DynamicArray * ) );
        if ( storage->arrayPtrs == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }
    }
    return SUCCESSFUL_EXECUTION;
}

/// @brief adding new array pointer to list of array pointers in global storage 
/// @param array pointer to array ought to be removed
/// @param storage current pointer to global storage
/// @return status of execution depending on inner calls
Exception addArrayToStorage( DynamicArray *array, ArrayStorage *storage ) {
    storage->count++;
    storage->arrayPtrs = realloc( storage->arrayPtrs, storage->count * sizeof( DynamicArray * ) );
    if ( storage->arrayPtrs == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }
    
    storage->arrayPtrs[storage->count - 1] = array;
    return SUCCESSFUL_EXECUTION;
}

/// @brief initializing array at given address with provided typeinfo and initial capacity to prevent multiple resize calls
/// @param array pointer to address where array's address should be stored
/// @param TI provided typeInfo interface
/// @param initialCapacity pretty self-explaining
/// @return status of execution depending on inner calls
Exception init( DynamicArray **array, const TypeInfo *TI, const int initialCapacity ) {
    ( *array ) = malloc( sizeof( DynamicArray ) );
    if ( ( *array ) == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    if ( TI == NULL ) {
        return NULL_TYPEINFO_ERROR;
    }    
    ( *array )->typeInfo = TI;
    
    ( *array )->capacity = abs( initialCapacity );
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

/// @brief removing array and cleaning up all data related to it including removal from storage
/// @param array pointer to array ought to be removed
/// @return status of execution depending on inner calls
Exception deleteArray( DynamicArray *array ) {
    if ( array == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }
    
    if ( array->size > 0 ) {
        for ( int index = 0; index < array->size; index++ ) {
            array->typeInfo->destruct( ( char * ) array->begin + index * array->typeInfo->getSize() );
        }
    }

    if ( removeArrayFromStorage( array, getStorage() ) != SUCCESSFUL_EXECUTION ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    free( array );

    return SUCCESSFUL_EXECUTION;

}

/// @brief adadptive resize function ensuring allocation for all possible future manipulations with array's elements
/// @param array pointer to array
/// @param directive enumerator giving context of resize call: extend for adding elements (if needed) and shrink for removing elements (if possible)
/// @return status of execution depending on inner calls
Exception resize( DynamicArray *array, const resizeType directive ) {
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

    return SUCCESSFUL_EXECUTION;
}

/// @brief appending new element to the array
/// @param array pointer to array where the element should be stored
/// @param element pointer to new element
/// @return status of execution depending on inner calls
Exception append( DynamicArray *array, const elemPtr *element ) {
    if ( array == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

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

/// @brief appending new element to the array
/// @param array pointer to array where the element should be stored
/// @param element pointer to new element
/// @return status of execution depending on inner calls
Exception prepend( DynamicArray *array, const elemPtr *element ) {
    if ( array == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }
    array->size++;

    if ( resize( array, EXTEND ) == SUCCESSFUL_EXECUTION ) {
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

/// @brief appending new element to the array
/// @param array pointer to array where the element should be stored
/// @param element pointer to new element
/// @param index value of index at which new element should be stored
/// @return status of execution depending on inner calls
Exception pushIndex( DynamicArray *array, const elemPtr *element, const int index ) {
    if ( array == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }
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

/// @brief function that copying arrays data by appending from one array to another (could be memmove but since we're storing char*, append() is a more reliable option
/// @param destination pointer to target array to which we're copying
/// @param source pointer to source array
/// @return status of execution depending on inner calls
Exception copyArray( DynamicArray *destination, const DynamicArray *source ) {
    elemPtr *sourceElem;

    for ( short index = 0; index < source->size; index++ ) {
        sourceElem = ( elemPtr * ) ( ( char * ) source->head + index * source->typeInfo->getSize() );
        
        Exception appendStatus = append( destination, sourceElem );
        if ( appendStatus != SUCCESSFUL_EXECUTION ) {
            return appendStatus;
        }
    }

    return SUCCESSFUL_EXECUTION;
}

/// @brief function for arrays' concatenation consecutively copying data from given arrays to target
/// @param result pointer to pointer to target array (for initialization)
/// @param array1 pointer to first array
/// @param array2 pointer to second array
/// @return status of execution depending on inner calls
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

/// @brief map(l, f) where l = [a1, a2, ... , an] is a set of elements of type <T> and f(): T -> T returns l' = [f(a1), f(a2), ... , f(an)] - applying some 
/// @brief unary operator to every element of the array
/// @param array - pointer to array ought to be mapped
/// @param func - pointer to mapping function
/// @return all unary operators are type-safe so we aint returning anything besides successful execution
Exception map( DynamicArray *array, unaryOperator func ) {
    for ( short index = 0; index < array->size; index++ ) {
        elemPtr *elem = ( elemPtr * ) ( ( char * ) array->head + index * array->typeInfo->getSize() );
        func( *elem );
    }
    
    return SUCCESSFUL_EXECUTION;
}


Exception where( DynamicArray *array, predicate func ) {
    DynamicArray *result = NULL;

    Exception initStatus = init( &result, array->typeInfo, 2 );
    if ( initStatus != SUCCESSFUL_EXECUTION ) {
        return initStatus;
    }

    for ( short index = 0; index < array->size; index++ ) {
        elemPtr *elem = ( elemPtr * ) ( ( char * ) array->head + index * array->typeInfo->getSize() );
        if ( func( *elem ) ) {
            Exception appendStatus = append( result, elem );
            if ( appendStatus != SUCCESSFUL_EXECUTION ) {
                return appendStatus;
            }
        }
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