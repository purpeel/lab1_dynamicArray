#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/stringTypeinfo.h"


size_t stringGetSize() {
    size_t size;

    size = sizeof( char * );

    return size;
}


int stringDelete( elemPtr arg ) {
    int res;

    free( arg );

    return res;
}


int stringCopy( elemPtr *destination, const elemPtr source ) {
    int length = strlen( (char *) source ) + 1;

    if ( *( destination ) == NULL ) {
        return 1;
    }

    for ( unsigned i = 0; i < length; i++ ) {
        *( ( char * ) *( destination ) + i ) = *( ( char * ) source + i );
    }

    return 0;
}


ComparisonResult stringCompare( const elemPtr elem1, const elemPtr elem2 ) {
    int equalFlag = 1;
    int len1 = strlen( (char *) elem1 );
    int len2 = strlen( (char *) elem2 );
    int minLength;

    if ( len1 == len2 ) {

        for ( unsigned index = 0; index < len1; index++ ) {

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
        minLength = (len1 <= len2) ? len1 : len2;

        for ( unsigned index = 0; index < minLength; index++ ) {

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


void stringPrint( const elemPtr output ) {
    printf( "%s\n", ( char *) output );
}


int stringInput( elemPtr *arg, const char *input ) {
    if ( *( arg )== NULL ) {
        return 1;
    }

    if ( stringCopy( arg, ( elemPtr ) input ) == 0 ) {
        return 0;
    } else {
        return 1;
    }

}


static TypeInfo *stringTI = NULL;


const TypeInfo *getStringTI() {
    if ( stringTI == NULL ) {
        stringTI = malloc( sizeof( TypeInfo ) );
        if ( stringTI == NULL ) {
            return NULL;
        }

        stringTI->input = stringInput;
        stringTI->assign = stringCopy;

        stringTI->compare = stringCompare;
        
        stringTI->getSize = stringGetSize;
        stringTI->destruct = stringDelete;

        stringTI->print = stringPrint;
    }
    return stringTI;    
}