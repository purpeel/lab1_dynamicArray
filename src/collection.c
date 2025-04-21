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
        storage->arrays = NULL;
    }
    return storage;
}

/// @brief function clearing all data related to storage
/// @param storage current pointer to global storage
/// @return status of execution depending on inner calls
Exception deleteStorage( ArrayStorage *storage ) {
    Exception status;
    
    if ( storage->count == 0 ) {
        storage->arrays = NULL;
        return SUCCESSFUL_EXECUTION;
    }
    for ( short index = 0; index < storage->count; index++ ) {
        status = removeArrayFromStorage( storage->arrays[index], storage );
        if ( status != SUCCESSFUL_EXECUTION ) {
            return status;
        }
    }
    // free( storage );
    return SUCCESSFUL_EXECUTION;
}

/// @brief removing array from storage by consecutively swapping its adress with the next one until the end and then reallocating data without last array's address
/// @param array pointer to array ought to be removed
/// @param storage current pointer to global storage
/// @return status of execution depending on inner calls
Exception removeArrayFromStorage( DynamicArray *array, ArrayStorage *storage ) {
    DynamicArray *temp;
    for ( short index = 0; index < storage->count - 1; index++ ) {
        if ( storage->arrays[index] == array ) {
            temp = storage->arrays[index + 1];
            storage->arrays[index + 1] = storage->arrays[index];
            storage->arrays[index] = temp;
        }
    }
    storage->count--;
    if ( storage->count == 0 ) {
        free(storage->arrays);
        storage->arrays = NULL;
        return SUCCESSFUL_EXECUTION;
    } else {
        storage->arrays = realloc( storage->arrays, storage->count * sizeof( DynamicArray * ) );
        if ( storage->arrays == NULL ) {
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
    storage->arrays = realloc( storage->arrays, storage->count * sizeof( DynamicArray * ) );
    if ( storage->arrays == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }
    
    storage->arrays[storage->count - 1] = array;
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
    } if ( TI == NULL ) {
        return NULL_TYPEINFO_ERROR;
    }    
    ( *array )->typeInfo = TI;

    ( *array )->capacity = initialCapacity > 2 ? initialCapacity : 2;
    ( *array )->size = 0;
    ( *array )->beginOfReserved = malloc( ( *array )->typeInfo->getSize() * ( *array )->capacity );
    if ( ( *array )->beginOfReserved == NULL ) {
        free( ( *array ) );
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    ( *array )->data = ( ElemPtr * ) ( (char *) (*array)->beginOfReserved + ((*array)->capacity / 4 + 1 ) * (*array)->typeInfo->getSize() );
    ( *array )->endOfReserved = ( ElemPtr * ) ( (char *) (*array)->beginOfReserved +     (*array)->capacity        * (*array)->typeInfo->getSize() );

    Exception storingStatus = addArrayToStorage( *array, getStorage() );

    if ( storingStatus != SUCCESSFUL_EXECUTION ) {
        free( ( *array )->beginOfReserved );
        free( *array );
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
            array->typeInfo->destruct( ( ElemPtr ) ( ( char * ) array->beginOfReserved + index * array->typeInfo->getSize() ) );
        }
    }

    if ( removeArrayFromStorage( array, getStorage() ) != SUCCESSFUL_EXECUTION ) {

        return MEMORY_ALLOCATION_ERROR;
    }

    return SUCCESSFUL_EXECUTION;

}

/// @brief adadptive resize function ensuring allocation for all possible future manipulations with array's elements
/// @param array pointer to array
/// @param directive enumerator giving context of resize call: extend for adding elements (if needed) and shrink for removing elements (if possible)
/// @return status of execution depending on inner calls

Exception extend( DynamicArray *array, const int sizeDiff ) {
    if ( array->beginOfReserved == NULL ) {
        return ARRAY_DATA_ALLOCATION_ERROR;
    } 
    array->size += sizeDiff;
    ElemPtr *buffer = NULL;

    if ( array->size >= ( array->capacity * 0.75 ) && array->capacity < 10000 ) {
        buffer = ( ElemPtr *) malloc( ( array->size - sizeDiff ) * array->typeInfo->getSize() );
        if ( buffer == NULL ) {
            return ARRAY_DATA_ALLOCATION_ERROR;
        }
        memmove( buffer, array->data, array->typeInfo->getSize() * ( array->size - sizeDiff ) );
        array->capacity = array->capacity * 2 + 1;
        array->beginOfReserved = ( ElemPtr * ) realloc( array->beginOfReserved, array->capacity * array->typeInfo->getSize() );
        if ( array->beginOfReserved == NULL ) {
            free( buffer );
            return ARRAY_DATA_ALLOCATION_ERROR;
        }
        array->data = ( ElemPtr * ) ( ( char * ) array->beginOfReserved + ( 1 + array->capacity / 4 ) * array->typeInfo->getSize() );
        memmove( array->data, buffer, array->typeInfo->getSize() * ( array->size - sizeDiff ) );
    } else if ( array->size > ( array->capacity * 0.25 ) && array->capacity > 10000 ) {
        array->capacity += 1000;
        array->beginOfReserved = ( ElemPtr * ) realloc( array->beginOfReserved, array->capacity * array->typeInfo->getSize() );
        if ( array->beginOfReserved == NULL ) {
            free( buffer );
            return ARRAY_DATA_ALLOCATION_ERROR;
        }
        array->data = ( ElemPtr * ) ( ( char * ) array->beginOfReserved +      500        * array->typeInfo->getSize() );
        memmove( array->data, buffer, array->typeInfo->getSize() * ( array->size - sizeDiff ) );
    }
    array->endOfReserved = ( ElemPtr * ) ( ( char * ) array->beginOfReserved + array->capacity * array->typeInfo->getSize() );
    free( buffer );

    return SUCCESSFUL_EXECUTION;
}


Exception shrink( DynamicArray *array, const int sizeDiff ) {
    if ( array->beginOfReserved == NULL ) {
        return ARRAY_DATA_ALLOCATION_ERROR;
    } 
    if ( sizeDiff > array->size ) {
        return INVALID_SIZEDIFF_ERROR;  
    }    
    array->size -= sizeDiff;
    ElemPtr *buffer = NULL;
    if ( array->size < ( array->capacity * 0.25 ) && array->capacity < 10000 ) {
        buffer = ( ElemPtr *) malloc( ( array->size + sizeDiff ) * array->typeInfo->getSize() );
        if ( buffer == NULL ) {
            return ARRAY_DATA_ALLOCATION_ERROR;
        }
        memmove( buffer, array->data, array->typeInfo->getSize() * ( array->size + sizeDiff ) );
        array->capacity = array->capacity / 2 + 1;
        array->beginOfReserved = ( ElemPtr * ) realloc( array->beginOfReserved, array->capacity * array->typeInfo->getSize() );
        if ( array->beginOfReserved == NULL ) {
            free( buffer );
            return ARRAY_DATA_ALLOCATION_ERROR;
        }
        array->data = ( ElemPtr * ) ( ( char * ) array->beginOfReserved + ( 1 + array->capacity / 8 ) * array->typeInfo->getSize() );
        memmove( array->data, buffer, array->typeInfo->getSize() * ( array->size + sizeDiff ) );
    } else if ( array->size < ( array->capacity * 0.75 ) && array->capacity > 10000 ) {
        buffer = ( ElemPtr *) malloc( ( array->size + sizeDiff ) * array->typeInfo->getSize() );
        if ( buffer == NULL ) {
            return ARRAY_DATA_ALLOCATION_ERROR;
        }
        memmove( buffer, array->data, array->typeInfo->getSize() * ( array->size + sizeDiff ) );
        array->capacity -= 1000;
        array->beginOfReserved = ( ElemPtr * ) realloc( array->beginOfReserved, array->capacity * array->typeInfo->getSize() );
        if ( array->beginOfReserved == NULL ) {
            free( buffer );
            return ARRAY_DATA_ALLOCATION_ERROR;
        }
        array->data = ( ElemPtr * ) ( ( char * ) array->beginOfReserved +       500       * array->typeInfo->getSize() );
        memmove( array->data, buffer, array->typeInfo->getSize() * ( array->size + sizeDiff ) );
    }
    array->endOfReserved = ( ElemPtr * ) ( ( char * ) array->beginOfReserved +       array->capacity       * array->typeInfo->getSize() );
    free( buffer );

    return SUCCESSFUL_EXECUTION;
}

/// @brief appending new element to the array
/// @param array pointer to array where the element should be stored
/// @param element pointer to new element
/// @return status of execution depending on inner calls
Exception append( DynamicArray *array, const ElemPtr *element ) {
    if ( array == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    Exception extendStatus = extend( array, 1 );

    if ( extendStatus == SUCCESSFUL_EXECUTION ) {
        if ( array->data == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }

        ElemPtr *placing = ( ElemPtr * ) ( ( char * ) array->data + ( array->size - 1 ) * array->typeInfo->getSize() );
        Exception assigningStatus = array->typeInfo->assign( placing, *element );
        if ( assigningStatus != SUCCESSFUL_EXECUTION ) {
            return assigningStatus;
        }

    } else { 
        return extendStatus;
    }

    return SUCCESSFUL_EXECUTION;
}

/// @brief appending new element to the array
/// @param array pointer to array where the element should be stored
/// @param element pointer to new element
/// @return status of execution depending on inner calls
Exception prepend( DynamicArray *array, const ElemPtr *element ) {
    if ( array == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    if ( extend( array, 1 ) == SUCCESSFUL_EXECUTION ) {
        if ( array->data == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }
        array->data = ( ElemPtr * ) ( ( char * ) array->data - array->typeInfo->getSize() );

        ElemPtr *placing = array->data;
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
Exception insertAt( DynamicArray *array, const ElemPtr *element, const int index ) {
    if ( array == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    if ( index < 0 || index > array->size ) {
        return INVALID_INDEX_ERROR;
    }
    
    if ( extend(array, 1) == SUCCESSFUL_EXECUTION ) {
        if ( array->data == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }
        array->endOfReserved = ( ElemPtr * ) ( ( char * ) array->data + array->size * array->typeInfo->getSize() );


        ElemPtr *placing = ( ElemPtr * ) ( ( char * ) array->data + index * array->typeInfo->getSize() );
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

/// @brief function for getting an element from the array by index
/// @param array pointer to the array
/// @param index 
/// @param destination pointer to where the extracted element should be stored
/// @return status of execution depending on inner calls
Exception getElem( DynamicArray *array, const int index, ElemPtr *destination ) {
    if ( array == NULL || destination == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    if ( array->size == 0 )  {
        return EMPTY_ARRAY_ERROR;
    }

    if ( index < 0 || index >= array->size ) {
        return INVALID_INDEX_ERROR;
    }

    ElemPtr *elem = ( ElemPtr * ) ( ( char * ) array->data + index * array->typeInfo->getSize() );
    Exception assigningStatus = array->typeInfo->assign( destination, *elem );
    if ( assigningStatus != SUCCESSFUL_EXECUTION ) {
        return assigningStatus;
    }

    return SUCCESSFUL_EXECUTION;

}


/// @brief function that copying arrays data by appending from one array to another (could be memmove but since we're storing char*, append() is a more reliable option
/// @param destination pointer to target array to which we're copying
/// @param source pointer to source array
/// @return status of execution depending on inner calls
Exception copyArray( DynamicArray *destination, const DynamicArray *source ) {
    if ( destination->typeInfo != source->typeInfo ) {
        return ARRAYS_TYPEINFO_MISMATCH_ERROR;
    }
    ElemPtr *sourceElem;

    for ( short index = 0; index < source->size; index++ ) {
        sourceElem = ( ElemPtr * ) ( ( char * ) source->data + index * source->typeInfo->getSize() );
        
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
        deleteArray( *result );
        return copyStatus;
    }
    copyStatus = copyArray( *result, array2 );
    if ( copyStatus != SUCCESSFUL_EXECUTION ) {
        deleteArray( *result );
        return copyStatus;
    }

    return SUCCESSFUL_EXECUTION;
}

/// @brief map(l, f) where l = [a1, a2, ... , an] is a set of elements of type <T> and f(): T -> T returns l' = [f(a1), f(a2), ... , f(an)] - applying some 
/// @brief unary operator to every element of the array
/// @param array - pointer to array ought to be mapped
/// @param func - pointer to mapping function
/// @return all unary operators are type-safe so we aint returning anything besides successful execution
Exception map( DynamicArray *array, unaryTransform func ) {
    for ( short index = 0; index < array->size; index++ ) {
        ElemPtr *elem = ( ElemPtr * ) ( ( char * ) array->data + index * array->typeInfo->getSize() );
        func( *elem );
    }
    
    return SUCCESSFUL_EXECUTION;
}

/// @brief where(l, f) where l = [a1, a2, ... , an] is a set of elements of type <T> and p() is an unary predicate returns l' = [ai, aj, ... , ak],
/// @brief and l' consists of elements which, when substituted into the predicate return True. new array is automatically added to the storage
/// @param array 
/// @param func 
/// @return status of execution depending on inner calls
Exception where( DynamicArray *array, predicate func ) {
    DynamicArray *result = NULL;

    Exception initStatus = init( &result, array->typeInfo, 2 );
    if ( initStatus != SUCCESSFUL_EXECUTION ) {
        return initStatus;
    }

    for ( short index = 0; index < array->size; index++ ) {
        ElemPtr *elem = ( ElemPtr * ) ( ( char * ) array->data + index * array->typeInfo->getSize() );
        if ( func( *elem ) ) {
            Exception appendStatus = append( result, elem );
            if ( appendStatus != SUCCESSFUL_EXECUTION ) {
                return appendStatus;
            }
        }
    }

    return SUCCESSFUL_EXECUTION;
}

/// @brief function splitting user input by spaces and consecutively appending provivded elements to the array
/// @param array pointer to the array ought to be filled
/// @param input string of user input
/// @param length length of user input
/// @return status of execution depending on inner calls
Exception readFromInput( DynamicArray *array, const char *input, const int length ) {
    char *buffer = NULL;
    int prevIsSpace = 1, bufferLength = 0, resBufferLength = 0, bufferRecordedCount = 0, readElementsCount = 0, isInQuotes = 0;
    ElemPtr *newElem = ( ElemPtr * ) malloc( array->typeInfo->getSize() );

    for ( short index = 0; index < length; index++ ) {

        if ( ( *( input + index ) == ' '  ||  *( input + index ) == '\n' ) && isInQuotes == 0 ) {
            if ( prevIsSpace == 0 && readElementsCount != 0 ) {
                *( buffer + resBufferLength ) = '\0';

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

    free( buffer );
    free( newElem );

    return SUCCESSFUL_EXECUTION;
}