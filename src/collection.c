#include <stdio.h>
#include <stdlib.h>
#include "../inc/collection.h"


static struct ArrayStorage {
    Arr **arrayPtrs;
    int quantity;
} ArrayStorage = {NULL, 0};



Exception addArrayToStorage( Arr *array ) {
    ArrayStorage.arrayPtrs = realloc( ArrayStorage.arrayPtrs, ( ArrayStorage.quantity + 1 ) * sizeof( Arr * ) );
    if ( ArrayStorage.arrayPtrs == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    *( ArrayStorage.arrayPtrs + ArrayStorage.quantity ) = array;
    ArrayStorage.quantity++;

    return SUCCESSFUL_EXECUTION;
}


Exception init( Arr **array, const TypeInfo *TI ) {
    ( *array ) = malloc( sizeof( Arr ) );
    if ( ( *array ) == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    ( *array )->TI = TI;


    ( *array )->capacity = 1;
    ( *array )->size = 0;

    ( *array )->head = malloc( ( *array )->TI->getSize() );
    if ( ( *array )->head == NULL ) {
        free( ( *array ) );
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    ( *array )->tail = ( *array )->head;
    
    Exception isStoringSuccessfull = addArrayToStorage( *array );
    return isStoringSuccessfull;
}


Exception resize( Arr *array, resizeType directive ) {
    void *buffer;

    switch ( directive )  
    {
    case EXTEND:
        if ( array->size > ( array->capacity * 0.75 ) && array->capacity < 10000 ) {
            buffer = realloc( array->head, array->capacity * array->TI->getSize() * 2 );
            if ( buffer == NULL ) {
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->head = buffer;
                array->capacity *= 2;
                array->tail = ( char * ) array->head + ( array->capacity * array->TI->getSize() / sizeof( char * ));
            }
        } else if ( array->size > ( array->capacity * 0.25 ) && array->capacity > 10000 ) {
            buffer = realloc( array->head, (array->capacity + 1000) * array->TI->getSize()  );
            if ( buffer == NULL ) {
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->head = buffer;
                array->capacity += 1000;
                array->tail = ( char * ) array->head + ( array->capacity * array->TI->getSize() );
            }
        }
        break;
    case SHRINK:
        if ( array->size < ( array->capacity * 0.25 ) && array->capacity < 10000 ) {
            buffer = realloc( array->head, array->capacity * array->TI->getSize() / 2 );
            if ( buffer == NULL ) {
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->head = buffer;
                array->capacity /= 2;
                array->tail = ( char * ) array->head + ( array->capacity * array->TI->getSize() / sizeof( char * ));  
            }
        } else if ( array->size < ( array->capacity * 0.75 ) && array->capacity > 10000 ) {
            buffer = realloc( array->head, ( array->capacity - 1000 )* array->TI->getSize() );
            if ( buffer == NULL ) {
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->head = buffer;
                array->capacity -= 1000;
                array->tail = ( char * ) array->head + ( array->capacity * array->TI->getSize() / sizeof( char * ));  
            }
        }
        break;

    }
    printf("size: %d, capacity: %d\n", array->size, array->capacity );
    return SUCCESSFUL_EXECUTION;
}



Exception append( Arr *array, const elemPtr element ) {
    array->size++;

    if ( resize( array, EXTEND ) == SUCCESSFUL_EXECUTION ) {
        if ( array->head == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }
        array->tail = ( char * ) array->head + ( array->TI->getSize() * array->size );

        elemPtr newElem = ( char * ) array->head + ( array->size - 1 ) * array->TI->getSize();
        array->TI->assign( &newElem, element );
    } else { 
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    return SUCCESSFUL_EXECUTION;
}


int readFromInput( Arr *array, const char *input, const int length ) {
    char *buffer = NULL;
    int prevIsSpace = 1, bufferLength = 0, resBufferLength = 0, bufferRecordedCount = 0, readElementsCount = 0, isInQuotes = 0;
    elemPtr newElem = NULL;

    for ( short index = 0; index < length; index++ ) {

        if ( ( *( input + index ) == ' '  ||  *( input + index ) == '\n' ) && isInQuotes == 0 ) {
            if ( prevIsSpace == 0 && readElementsCount != 0 ) {
                *( buffer + resBufferLength ) = '\0';

                newElem = malloc( array->TI->getSize() );
                if ( newElem == NULL ) {
                    return 1;
                }
                array->TI->input( &newElem, buffer );
                append( array, newElem );

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

                buffer = malloc( bufferLength );
                resBufferLength = bufferLength;
                if ( buffer == NULL ) {
                    return 1;
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

    return 0;
}
