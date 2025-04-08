#include <stdio.h>
#include <stdlib.h>
#include "../inc/collection.h"


Exception memCopy( elemPtr destination, const elemPtr source, const int size ) {
    if ( source == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }
    for ( unsigned long index = 0; index < size; index++ ) {
        *( ( unsigned char * ) destination + index ) = *( ( unsigned char * ) source + index );
    }
    return SUCCESSFUL_EXECUTION;
}


Exception init( Arr **array, const TypeInfo *TI ) {
    ( *array ) = malloc( sizeof( Arr ) );
    if ( ( *array ) == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    ( *array )->typeInfo = TI;


    ( *array )->capacity = 2;
    ( *array )->size = 0;

    ( *array )->begin = malloc( ( *array )->typeInfo->getSize() * ( *array )->capacity );
    if ( ( *array )->begin == NULL ) {
        free( ( *array ) );
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    ( *array )->head = ( char * ) ( *array )->begin + ( *array )->typeInfo->getSize();
    ( *array )->tail = ( char * ) ( *array )->begin + ( *array )->capacity * ( *array )->typeInfo->getSize();
    
    return SUCCESSFUL_EXECUTION;
}


Exception resize( Arr *array, resizeType directive ) {
    void *buffer;

    switch ( directive )  
    {
    case EXTEND:
        if ( array->size >= ( array->capacity * 0.75 ) && array->capacity < 10000 ) {
            buffer = malloc( array->size * array->typeInfo->getSize() - 1 );
            if ( buffer == NULL ) {
                free( buffer );
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->capacity = array->capacity * 2 + 1;
                memCopy( buffer, array->head, array->typeInfo->getSize() * ( array->size - 1 ) );
                array->begin = realloc( array->begin, array->capacity * array->typeInfo->getSize() );
                if ( array->begin == NULL ) {
                    free( buffer );
                    return ARRAY_DATA_ALLOCATION_ERROR;
                }
                array->head = array->begin + ( 1 + array->capacity / 4 ) * array->typeInfo->getSize();
                array->tail = ( char * ) array->begin + ( array->capacity * array->typeInfo->getSize() );
                memCopy( array->head, buffer, array->typeInfo->getSize() * ( array->size - 1 ) );
            }
        } else if ( array->size > ( array->capacity * 0.25 ) && array->capacity > 10000 ) {
            buffer = malloc( array->size * array->typeInfo->getSize() - 1 );
            if ( buffer == NULL ) {
                free( buffer );
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->capacity += 1000;
                memCopy( buffer, array->head, array->typeInfo->getSize() * ( array->size - 1 ) );
                array->begin = realloc( array->begin, array->capacity * array->typeInfo->getSize() );
                if ( array->begin == NULL ) {
                    free( buffer );
                    return ARRAY_DATA_ALLOCATION_ERROR;
                }
                array->head = array->begin + 500;
                array->tail = ( char * ) array->begin + ( array->capacity * array->typeInfo->getSize() );
                memCopy( array->head, buffer, array->typeInfo->getSize() * ( array->size - 1 ) );
            }
        }
        break;
    case SHRINK:
        if ( array->size < ( array->capacity * 0.25 ) && array->capacity < 10000 ) {
            buffer = malloc( array->size * array->typeInfo->getSize() - 1 );
            if ( buffer == NULL ) {
                free( buffer );
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->capacity = array->capacity / 2 + 1;
                memCopy( buffer, array->head, array->typeInfo->getSize() * ( array->size - 1 ) );
                array->begin = realloc( array->begin, array->capacity * array->typeInfo->getSize() );
                if ( array->begin == NULL ) {
                    free( buffer );
                    return ARRAY_DATA_ALLOCATION_ERROR;
                }
                array->head = array->begin + ( 1 + array->capacity / 8 ) * array->typeInfo->getSize();
                array->tail = ( char * ) array->begin + ( array->capacity * array->typeInfo->getSize() );
                memCopy( array->head, buffer, array->typeInfo->getSize() * ( array->size - 1 ) );
            }
        } else if ( array->size < ( array->capacity * 0.75 ) && array->capacity > 10000 ) {
            buffer = malloc( array->size * array->typeInfo->getSize() - 1 );
            if ( buffer == NULL ) {
                free( buffer );
                return ARRAY_DATA_ALLOCATION_ERROR;
            } else {
                array->capacity -= 1000;
                memCopy( buffer, array->head, array->typeInfo->getSize() * ( array->size - 1 ) );
                array->begin = realloc( array->begin, array->capacity * array->typeInfo->getSize() );
                if ( array->begin == NULL ) {
                    free( buffer );
                    return ARRAY_DATA_ALLOCATION_ERROR;
                }
                array->head = array->begin + 500;
                array->tail = ( char * ) array->begin + ( array->capacity * array->typeInfo->getSize() );
                memCopy( array->head, buffer, array->typeInfo->getSize() * ( array->size - 1 ) );
            }
        }
        break;

    }
    return SUCCESSFUL_EXECUTION;
}



Exception append( Arr *array, const elemPtr element ) {
    array->size++;

    if ( resize( array, EXTEND ) == SUCCESSFUL_EXECUTION ) {
        if ( array->head == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }
        array->tail = ( char * ) array->begin + ( array->typeInfo->getSize() * array->capacity );

        elemPtr newElem = ( char * ) array->head + ( array->size - 1 ) * array->typeInfo->getSize();
        array->typeInfo->assign( &newElem, element );
    } else { 
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    return SUCCESSFUL_EXECUTION;
}


Exception prepend( Arr *array, const elemPtr element ) {
    array->size++;

    if ( resize( array, EXTEND ) == SUCCESSFUL_EXECUTION ) {
        if ( array->head == NULL ) {
            return MEMORY_ALLOCATION_ERROR;
        }
        array->head = ( char * ) array->head - array->typeInfo->getSize();


        elemPtr newElem = array->head;
        array->typeInfo->assign( &newElem, element );

    } else { 
        return ARRAY_DATA_ALLOCATION_ERROR;
    }

    return SUCCESSFUL_EXECUTION;
}


Exception readFromInput( Arr *array, const char *input, const int length ) {
    char *buffer = NULL;
    int prevIsSpace = 1, bufferLength = 0, resBufferLength = 0, bufferRecordedCount = 0, readElementsCount = 0, isInQuotes = 0;
    elemPtr newElem = NULL;

    for ( short index = 0; index < length; index++ ) {

        if ( ( *( input + index ) == ' '  ||  *( input + index ) == '\n' ) && isInQuotes == 0 ) {
            if ( prevIsSpace == 0 && readElementsCount != 0 ) {
                *( buffer + resBufferLength ) = '\0';

                newElem = malloc( array->typeInfo->getSize() );
                if ( newElem == NULL ) {
                    free( newElem );
                    return MEMORY_ALLOCATION_ERROR;
                }
                array->typeInfo->input( &newElem, buffer );
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
                    free( resBufferLength );
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

    return SUCCESSFUL_EXECUTION;
}
