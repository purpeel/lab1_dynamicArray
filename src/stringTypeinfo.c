#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/stringTypeinfo.h"


size_t stringGetSize() {

    return sizeof( char *);

}


Exception deleteString( elemPtr arg ) {

    free( arg );

    return SUCCESSFUL_EXECUTION;
}


Exception copyString( elemPtr *destination, const elemPtr source ) {
    char **destinationStr = ( char ** ) destination;
    char    *sourceStr   = ( char * ) source;
    
    int length = strlen( source ) + 1;

    *destinationStr = malloc( length );
    if ( *destinationStr == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for ( int i = 0; i < length; i++ ) {
        *( *destinationStr + i ) = *( sourceStr + i );
    }

    return SUCCESSFUL_EXECUTION;
}


ComparisonResult compareString( const elemPtr elem1, const elemPtr elem2 ) {
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


void printString( const elemPtr output ) {

    printf( "%s", ( char *) output );

}


Exception inputString( elemPtr *arg, const char *input ) {

    return copyString( arg, ( elemPtr ) input );
}


bool firstIsLower( const elemPtr arg ) {
    unsigned asciiCode = ( unsigned ) *( ( char * ) arg );

    if ( asciiCode >= 'a' && asciiCode <= 'z' ) {
        return true;
    }

    return false;
}


bool isDigit( const elemPtr arg ) {
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


bool isUpperCase( const elemPtr arg ) {
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


Exception invertString( elemPtr arg ) {
    int len = strlen( (char *) arg );
    elemPtr temp = malloc( len + 1 );
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


Exception toLowerCase( elemPtr arg ) {
    int len = strlen( (char *) arg );

    for ( short index = 0; index < len; index++ ) {
        if ( *( ( char * ) arg + index ) >= 'A' && *( ( char * ) arg + index ) <= 'Z' ) {
            *( ( char * ) arg + index ) += 32;
        }
    }

    return SUCCESSFUL_EXECUTION;
}


Exception getFirstLiteral( elemPtr arg ) {
    char *argStr = ( char * ) arg;

    argStr = realloc( argStr, 2 );
    if ( argStr == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }
    
    argStr[1] = '\0';

    return SUCCESSFUL_EXECUTION;
}


elemPtr maxString( const elemPtr arg1, const elemPtr arg2 ) {
    elemPtr result = ( compareString( arg1, arg2 ) == LESS ) ? arg1 : arg2;

    return result;
}


elemPtr minString( const elemPtr arg1, const elemPtr arg2 ) {
    elemPtr result = ( compareString( arg1, arg2 ) == LESS ) ? arg1 : arg2;

    return result;
}

Exception swapString( elemPtr *elem1, elemPtr *elem2 ) {
    elemPtr temp;
    temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;

    return SUCCESSFUL_EXECUTION;
}


static TypeInfo *stringTypeInfo = NULL;
static Exception ( *stringMapSet[3] )( elemPtr ) = { invertString, toLowerCase, getFirstLiteral };
static bool ( *stringWhereSet[3] )( elemPtr ) = { firstIsLower, isDigit, isUpperCase };


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

        stringTypeInfo->setForMap[0] = stringMapSet[0];
        stringTypeInfo->setForMap[1] = stringMapSet[1];
        stringTypeInfo->setForMap[2] = stringMapSet[2];

        stringTypeInfo->setForWhere[0] = stringWhereSet[0];
        stringTypeInfo->setForWhere[1] = stringWhereSet[1];
        stringTypeInfo->setForWhere[2] = stringWhereSet[2];
    }
    return stringTypeInfo;    
}