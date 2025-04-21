#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/stringTypeinfo.h"


size_t stringGetSize() {

    return sizeof( char *);

}


Exception deleteString( ElemPtr arg ) {

    free( arg );

    return SUCCESSFUL_EXECUTION;
}


Exception copyString( ElemPtr *destination, const ElemPtr source ) {
    if ( source == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    char **destinationStr = ( char ** ) destination;
    char    *sourceStr   = ( char * ) source;
    
    int length = strlen( ( char * ) source ) + 1;

    *destinationStr = ( char * ) malloc( length );
    if ( *destinationStr == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for ( int i = 0; i < length; i++ ) {
        *( *destinationStr + i ) = *( sourceStr + i );
    }

    return SUCCESSFUL_EXECUTION;
}


ComparisonResult compareString( const ElemPtr elem1, const ElemPtr elem2 ) {
    int equalFlag = 1;
    int len1 = strlen( (char *) elem1 );
    int len2 = strlen( (char *) elem2 );
    int minLength;  

    if ( len1 == len2 ) {

        for ( int index = 0; index < len1; index++ ) {

            if ( *( (char *) elem1 + index ) != *( (char *) elem2 + index ) ) {
                equalFlag = 0;

                if ( (unsigned) *( (char *) elem1 + index ) < (unsigned) *( (char *) elem2 + index ) ) {
                    return LESS;
                }
                else {
                    return GREATER;
                }
            }
        }

    } else { 
        minLength = (len1 < len2) ? len1 : len2;

        for ( int index = 0; index < minLength; index++ ) {

            if ( *( (char *) elem1 + index ) != *( (char *) elem2 + index ) ) {

                if ( (unsigned) *( (char *) elem1 + index ) < (unsigned) *( (char *) elem2 + index ) ) {
                    return LESS;
                }
                else {
                    return GREATER;
                }

            }
        }
    }

    if ( equalFlag == 1 ) { return EQUAL ;}
    if ( minLength == len1 ) { return LESS; }
    return GREATER;
}


void printString( const ElemPtr output ) {

    printf( "%s", ( char *) output );

}


Exception inputString( ElemPtr *arg, const char *input ) {

    return copyString( arg, ( ElemPtr ) input );
}


bool firstIsLower( const ElemPtr arg ) {
    unsigned asciiCode = ( unsigned ) *( ( char * ) arg );

    if ( asciiCode >= 'a' && asciiCode <= 'z' ) {
        return true;
    }

    return false;
}


bool isDigit( const ElemPtr arg ) {
    int len = strlen( ( char * ) arg );
    bool result = true;

    for ( short index = 0; index < len; index++ ) {
        unsigned asciiCode = ( unsigned ) *( ( char * ) arg + index );
        if ( asciiCode < '0' || asciiCode > '9' ) {
            result = false;
        }
    }

    return result;
}


bool isUpperCase( const ElemPtr arg ) {
    int len = strlen( ( char * ) arg );
    bool result = true;

    for ( short index = 0; index < len; index++ ) {
        unsigned asciiCode = ( unsigned ) *( ( char * ) arg + index );
        if ( asciiCode >= 'a' && asciiCode <= 'z' ) {
            result = false;
        }
    }

    return result;
}


Exception invertString( ElemPtr arg ) {
    int len = strlen( (char *) arg );
    ElemPtr temp = malloc( len + 1 );
    if ( temp == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    copyString( &temp, arg );

    for ( short index = 0; index < len; index++ ) {
        *( ( char * ) arg + index ) = *( ( char * ) temp + len - 1 - index );
    }

    free( temp );
    return SUCCESSFUL_EXECUTION;
}


Exception toLowerCase( ElemPtr arg ) {
    int len = strlen( (char *) arg );

    for ( short index = 0; index < len; index++ ) {
        if ( *( ( char * ) arg + index ) >= 'A' && *( ( char * ) arg + index ) <= 'Z' ) {
            *( ( char * ) arg + index ) += 32;
        }
    }

    return SUCCESSFUL_EXECUTION;
}


Exception getFirstLiteral( ElemPtr arg ) {
    char *argStr = ( char * ) arg;

    argStr = realloc( argStr, 2 );
    if ( argStr == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }
    
    argStr[1] = '\0';

    return SUCCESSFUL_EXECUTION;
}


ElemPtr maxString( const ElemPtr arg1, const ElemPtr arg2 ) {
    ElemPtr result = ( compareString( arg1, arg2 ) == LESS ) ? arg2 : arg1;

    return result;
}


ElemPtr minString( const ElemPtr arg1, const ElemPtr arg2 ) {
    ElemPtr result = ( compareString( arg1, arg2 ) == LESS ) ? arg1 : arg2;

    return result;
}

Exception swapString( ElemPtr *elem1, ElemPtr *elem2 ) {
    ElemPtr temp;
    temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;

    return SUCCESSFUL_EXECUTION;
}


static TypeInfo *stringTypeInfo = NULL;


const TypeInfo *getStringTI() {
    if ( stringTypeInfo == NULL ) {
        stringTypeInfo = malloc( sizeof( TypeInfo ) );
        if ( stringTypeInfo == NULL ) {
            return NULL;
        }
        stringTypeInfo->typeName = "string";
        stringTypeInfo->swap = swapString;

        stringTypeInfo->input = inputString;
        stringTypeInfo->assign = copyString;

        stringTypeInfo->compare = compareString;
        stringTypeInfo->maximum = maxString;
        stringTypeInfo->minimum = minString;
        
        stringTypeInfo->getSize = stringGetSize;
        stringTypeInfo->destruct = deleteString;

        stringTypeInfo->print = printString;

        stringTypeInfo->setForMap[0] = invertString;
        stringTypeInfo->setForMap[1] = toLowerCase;
        stringTypeInfo->setForMap[2] = getFirstLiteral;

        stringTypeInfo->setForWhere[0] = firstIsLower;
        stringTypeInfo->setForWhere[1] = isDigit;
        stringTypeInfo->setForWhere[2] = isUpperCase;
    }
    return stringTypeInfo;    
}