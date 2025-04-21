#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../inc/doubleTypeinfo.h"


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
                return UNEXPECTED_CHAR_ERROR;
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


size_t doubleGetSize() {

    return sizeof( double );

}


Exception deleteDouble( ElemPtr arg ) {

    return SUCCESSFUL_EXECUTION;
}


Exception assignDouble( ElemPtr *destination, const ElemPtr source ) {
    double **destinationDbl = ( double ** ) destination;
    double *sourceDbl       = ( double * ) source;

    *destinationDbl = malloc( doubleGetSize() );
    if ( *destinationDbl == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    **destinationDbl = *sourceDbl;
    
    return SUCCESSFUL_EXECUTION;
}


ComparisonResult compareDouble( const ElemPtr elem1, const ElemPtr elem2 ) {
    ComparisonResult result;
    double elem1Double = *( ( double * ) elem1 );
    double elem2Double = *( ( double * ) elem2 );

    if ( elem1Double > elem2Double ) {

        result = GREATER;

    } else if ( elem1Double < elem2Double ) {

        result = LESS;

    } else {
        result = EQUAL;
    }

    return result;
}


void printDouble( const ElemPtr output ) {

    printf( "%lf", *( ( double * ) output ) );

}


Exception inputDouble( ElemPtr *arg, const char *input ) {
    double value = 0.0;

    Exception conversionStatus = charToDouble( input, &value );

    if ( conversionStatus != SUCCESSFUL_EXECUTION ) {
        return conversionStatus;
    }

    return assignDouble( arg, ( ElemPtr ) &value );
}


bool isNormalized( const ElemPtr arg1 ) {
    double argDouble = fabs( *( ( double * ) arg1 ) );

    return argDouble >= 0.0 && argDouble <= 1.0;
}


bool isRound( const ElemPtr arg1 ) {
    double argDouble = fabs( *( ( double * ) arg1 ) );

    return argDouble - ( int ) argDouble == 0.0;
}


bool isPositive( const ElemPtr arg1 ) {
    bool res = *(double*)arg1 > 0.0 ? true : false;

    return res;
}


Exception oppose( ElemPtr arg ) {
    double *argDouble = ( double * ) arg;

    *argDouble *= ( -1 );

    return SUCCESSFUL_EXECUTION;
}


Exception logarithm( ElemPtr arg ) {
    double *argDouble = ( double * ) arg;
    if ( *argDouble == 0.0 ) {
        return SUCCESSFUL_EXECUTION;
    }
    if ( *argDouble < 0.0 ) {
        *argDouble *= ( -1 );
    }

    *argDouble = log10( fabs( *argDouble ) );

    return SUCCESSFUL_EXECUTION;
}


Exception sign( ElemPtr arg ) {
    double *argDouble = ( double * ) arg;
    if ( *argDouble < 0 ) {
        *argDouble = -1;
    } else if ( *argDouble > 0 ) {
        *argDouble = 1;
    } else {
        *argDouble = 0;
    }

    return SUCCESSFUL_EXECUTION;
}


ElemPtr maxDouble( const ElemPtr arg1, const ElemPtr arg2 ) {
    ElemPtr result = ( compareDouble( arg1, arg2 ) == LESS ) ? arg2 : arg1;

    return result;
}


ElemPtr minDouble( const ElemPtr arg1, const ElemPtr arg2 ) {
    ElemPtr result = ( compareDouble( arg1, arg2 ) == GREATER ) ? arg2 : arg1;

    return result;
}


Exception swapDouble( ElemPtr *elem1, ElemPtr *elem2 ) {
    ElemPtr temp;
    temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;

    return SUCCESSFUL_EXECUTION;
}


static TypeInfo *doubleTypeInfo = NULL;


const TypeInfo *getDoubleTI() {
    if ( doubleTypeInfo == NULL ) {
        doubleTypeInfo = malloc( sizeof( TypeInfo ) );
        if ( doubleTypeInfo == NULL ) {
            return NULL;
        }
        doubleTypeInfo->typeName = "double";
        doubleTypeInfo->swap = swapDouble;

        doubleTypeInfo->input = inputDouble;
        doubleTypeInfo->assign = assignDouble;

        doubleTypeInfo->compare = compareDouble;
        doubleTypeInfo->maximum = maxDouble;
        doubleTypeInfo->minimum = minDouble;
        
        doubleTypeInfo->getSize = doubleGetSize;
        doubleTypeInfo->destruct = deleteDouble;

        doubleTypeInfo->print = printDouble;

        doubleTypeInfo->setForMap[0] = oppose;
        doubleTypeInfo->setForMap[1] = logarithm;
        doubleTypeInfo->setForMap[2] = sign;

        doubleTypeInfo->setForWhere[0] = isNormalized;
        doubleTypeInfo->setForWhere[1] = isRound;
        doubleTypeInfo->setForWhere[2] = isPositive;
    }
    return doubleTypeInfo;  
}