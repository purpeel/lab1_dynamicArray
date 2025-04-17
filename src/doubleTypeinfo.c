#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../inc/doubleTypeinfo.h"


size_t doubleGetSize() {

    return sizeof( double );

}


Exception doubleDelete( elemPtr arg ) {
    free ( arg );

    return SUCCESSFUL_EXECUTION;
}


Exception assignDouble( elemPtr *destination, const elemPtr source ) {
    double **destinationDbl = ( double ** ) destination;
    double *sourceDbl       = ( double * ) source;

    *destinationDbl = malloc( doubleGetSize() );
    if ( *destinationDbl == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    **destinationDbl = *sourceDbl;
    
    return SUCCESSFUL_EXECUTION;
}


ComparisonResult compareDouble( const elemPtr elem1, const elemPtr elem2 ) {
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


void printDouble( const elemPtr output ) {

    printf( "%lf", *( ( double * ) output ) );

}


Exception inputDouble( elemPtr *arg, const char *input ) {
    double value = 0.0;

    Exception conversionStatus = charToDouble( input, &value );

    if ( conversionStatus != SUCCESSFUL_EXECUTION ) {
        return conversionStatus;
    }

    return assignDouble( arg, ( elemPtr ) &value );
}


bool isNormalized( const elemPtr arg1 ) {
    double argDouble = fabs( *( ( double * ) arg1 ) );

    return ( argDouble >= 0.0 && argDouble <= 1.0 );
}


bool isRound( const elemPtr arg1 ) {
    double argDouble = fabs( *( ( double * ) arg1 ) );

    return ( argDouble - ( int ) argDouble == 0.0 );
}


bool isPositive( const elemPtr arg1 ) {
    bool res = ( *( ( double * ) arg1 ) > 0.0 ) ? true : false;

    return res;
}


Exception oppose( elemPtr arg ) {
    printf("oppose\n");
    double *argDouble = ( double * ) arg;

    *argDouble *= ( -1 );

    return SUCCESSFUL_EXECUTION;
}


Exception logarithm( elemPtr arg ) {
    printf("logarithm\n");
    double *argDouble = ( double * ) arg;
    if ( *argDouble == 0.0 ) {
        return SUCCESSFUL_EXECUTION;
    }
    if ( *argDouble < 0.0 ) {
        *argDouble *= ( -1 );
    }

    *argDouble = log10( *argDouble );

    return SUCCESSFUL_EXECUTION;
}


Exception sign( elemPtr arg ) {
    printf("sign\n");
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


elemPtr maxDouble( const elemPtr arg1, const elemPtr arg2 ) {
    elemPtr result = ( compareDouble( arg1, arg2 ) == LESS ) ? arg1 : arg2;

    return result;
}


elemPtr minDouble( const elemPtr arg1, const elemPtr arg2 ) {
    elemPtr result = ( compareDouble( arg1, arg2 ) == GREATER ) ? arg1 : arg2;

    return result;
}


Exception swapDouble( elemPtr *elem1, elemPtr *elem2 ) {
    elemPtr temp;
    temp = *elem1;
    *elem1 = *elem2;
    *elem2 = temp;

    return SUCCESSFUL_EXECUTION;
}


static TypeInfo *doubleTypeInfo = NULL;
static Exception ( *doubleMapSet[3] )( elemPtr ) = { oppose, logarithm, sign };
static bool ( *doubleWhereSet[3] )( elemPtr ) = { isNormalized, isRound, isPositive };


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
        doubleTypeInfo->destruct = doubleDelete;

        doubleTypeInfo->print = printDouble;

        doubleTypeInfo->setForMap[0] = doubleMapSet[0];
        doubleTypeInfo->setForMap[1] = doubleMapSet[1];
        doubleTypeInfo->setForMap[2] = doubleMapSet[2];

        doubleTypeInfo->setForWhere[0] = doubleWhereSet[0];
        doubleTypeInfo->setForWhere[1] = doubleWhereSet[1];
        doubleTypeInfo->setForWhere[2] = doubleWhereSet[2];
    }
    return doubleTypeInfo;  
}