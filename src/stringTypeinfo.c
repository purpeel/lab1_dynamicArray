#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/stringTypeinfo.h"


size_t stringGetSize() {
    size_t size;

    size = sizeof( char * );

    return size;
}


Exception stringDelete( elemPtr arg ) {
    free( arg );

    return SUCCESSFUL_EXECUTION;
}


Exception stringCopy( elemPtr *destination, const elemPtr source ) {
    int length = strlen( (char *) source ) + 1;

    if ( *( destination ) == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    for ( unsigned i = 0; i < length; i++ ) {
        *( ( char * ) *( destination ) + i ) = *( ( char * ) source + i );
    }

    return SUCCESSFUL_EXECUTION;
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


Exception stringInput( elemPtr *arg, const char *input ) {
    if ( *( arg )== NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    if ( stringCopy( arg, ( elemPtr ) input ) == 0 ) {
        return SUCCESSFUL_EXECUTION;
    } else {
        return MEMORY_ALLOCATION_ERROR;
    }

}


// bool isAlpha( const elemPtr arg ) {
//     int len = strlen( (char *) arg );
//     bool result = true;

//     for ( short index = 0; index < len; index++ ) {
//         unsigned asciiCode = (unsigned) *( (char *) arg + index );
//         if ( ( asciiCode < 'A' || asciiCode > 'Z' ) && ( asciiCode < 'a' || asciiCode > 'z' ) ) {
//             result = false;
//         }
//     }

//     return result;
// }


// bool isDigit( const elemPtr arg ) {
//     int len = strlen( (char *) arg );
//     bool result = true;

//     for ( short index = 0; index < len; index++ ) {
//         unsigned asciiCode = (unsigned) *( (char *) arg + index );
//         if ( asciiCode < '0' || asciiCode > '9' ) {
//             result = false;
//         }
//     }

//     return result;
// }


// bool isUpperCase( const elemPtr arg ) {
//     int len = strlen( (char *) arg );
//     bool result = true;

//     for ( short index = 0; index < len; index++ ) {
//         unsigned asciiCode = ( unsigned ) *( ( char * ) arg + index );
//         if ( asciiCode < 'A' || asciiCode > 'Z' ) {
//             result = false;
//         }
//     }

//     return result;
// }


// bool isLowerCase( const elemPtr arg ) {
//     int len = strlen( (char *) arg );
//     bool result = true;

//     for ( short index = 0; index < len; index++ ) {
//         unsigned asciiCode = (unsigned) *( (char *) arg + index );
//         if ( asciiCode < 'a' || asciiCode > 'z' ) {
//             result = false;
//         }
//     }

//     return result;
// }


Exception invertString( elemPtr arg ) {
    int len = strlen( (char *) arg );
    char *temp = malloc( len + 1 );
    if ( temp == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    stringCopy( &temp, arg );

    for ( short index = 0; index < len; index++ ) {
        *( ( char * ) arg + index ) = *( temp + len - 1 - index );
    }

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


Exception toUpperCase( elemPtr arg ) {
    int len = strlen( (char *) arg );

    for ( short index = 0; index < len; index++ ) {
        if ( *( ( char * ) arg + index ) >= 'a' && *( ( char * ) arg + index ) <= 'z' ) {
            *( ( char * ) arg + index ) -= 32;
        }
    }

    return SUCCESSFUL_EXECUTION;
}


static TypeInfo *stringTI = NULL;


const TypeInfo *getStringTI() {
    if ( stringTI == NULL ) {
        stringTI = malloc( sizeof( TypeInfo ) );
        if ( stringTI == NULL ) {
            return NULL;
        }
        stringTI->typeName = "string";

        stringTI->input = stringInput;
        stringTI->assign = stringCopy;

        stringTI->compare = stringCompare;
        
        stringTI->getSize = stringGetSize;
        stringTI->destruct = stringDelete;

        stringTI->print = stringPrint;
    }
    return stringTI;    
}