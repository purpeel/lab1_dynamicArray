#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/util.h"


double power( const double base, int power ) {
    double res = 1.0;
    if ( power == 0 ) { return res; } 
    if ( power > 0 ) {

        while ( power != 0 ) {
            res *= base;
            power--;
        }

    } else { 

        while ( power != -1 ) {
            res /= base;
            power++;
        }
    }
    return res;
}


Exception charToInt( const char *source, int *value ) {
    int len = 0;
    
    for ( short index = 0; *( source + index ) != '\0' || *( source + index ) != '\n'; index++ ) {
        int asciiCode = ( int ) *( source + index );
        
        if ( asciiCode == '\0' || asciiCode == '\n' ) { 
            break;
        } else {
            len++;
        }
    }

    if ( len == 0 ) {
        return ZERO_LENGTH_INPUT_ERROR;
    }

    for ( short index = 0; *( source + index ) != '\0' || *( source + index ) != '\n'; index++ ) {
        int asciiCode = ( int ) *( source + index );
        if ( asciiCode == '\0' || asciiCode == '\n' ) { 
            break;
        }

        if ( asciiCode < '0' || asciiCode > '9' ) {
            return UNEXPECTED_ALPHA_ERROR;
        } else {
            *value += ( asciiCode - '0') * power( 10, len - index - 1);
        }
    }

    return SUCCESSFUL_EXECUTION;
}


Exception charToDouble( const char *source, double *value ) {
    *value = 0.0;

    int len = strlen( source );
    int integerPartLen = 0, pointCount = 0;
    int isNegative = 0;

    for ( short index = 0; index < len; index++ ) {
        int asciiCode = ( int ) *( source + index );

        if ( asciiCode < '0' || asciiCode > '9' ) {
            if ( asciiCode == '-' && index == 0 ) {
                isNegative = 1;
            } else if ( asciiCode != '.'  ) {
                return UNEXPECTED_ALPHA_ERROR;
            }
        }

        if ( asciiCode == '.' ) {

            integerPartLen = index;
            pointCount ++;

            if ( pointCount > 1 ) {
                return DOUBLE_INPUT_ERROR;
            }
        }
    }
    
    integerPartLen = ( integerPartLen == 0) ? len : integerPartLen;

    for ( short index = 0; index < len; index++ ) {
        int asciiCode = ( int ) *( source + index );

        if ( asciiCode != '.' && asciiCode != '-' ) {
            *value += ( asciiCode - '0') * power( 10, integerPartLen - index - 1 );
        }
    }
    
    if ( isNegative == 1 ) {
        *value *= -1;
    }

    return SUCCESSFUL_EXECUTION;
}