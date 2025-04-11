#include <stdio.h>
#include "../inc/ui.h"


printUI printMainMenu() {
    // system( "clear" );
    printf( "Choose one of the listed functions and enter a number of chosen function.\n" );
    printf( "Each function is followed by its number.\n\t\t-----\n" );
    printf( "1 - Enter array contents via keyboard and process them.\n" );
    printf( "2 - Provide array contents through reading from a .txt file.\n" );
    printf( "0 - Stop the execution.\n" );
}


printUI printError( const Exception exeCode ) {
    system( "clear" );
    switch ( exeCode )
    {
    case MEMORY_ALLOCATION_ERROR:
        printf( "Error. Unable to allocate memory.\n" );
        break;
    case INPUT_OVERFLOW_ERROR:
        printf( "Provided input exceeded allowed limit.\n");
        break;
    case UNEXPECTED_ALPHA_ERROR:
        printf( "Error. Detected alphabet symbols when digits were expected.\n" );
        break;
    case DOUBLE_INPUT_ERROR:
        printf( "Error. Detected two or more decimal points when a real number was expected.\n" );
        break;
    case ARRAY_DATA_ALLOCATION_ERROR:
        printf( "Error. Unable to allocate memory for keeping array contents.\n" );
        break;
    case CMD_OUT_OF_CONTEXT_ERROR:
        printf( "Error. Provided command is out of range of available commands.\n" );
        break;
    case ZERO_LENGTH_INPUT_ERROR:
        printf( "Error. Provided input is empty.\n" );
        break;
    default:
        break;
    }
    printf( "Try again.\n" );
}


printUI printExit( const menuDirective context ) {
    // system( "clear" );
    switch ( context )
    {
    case 0:

        printf( "Exexcution successfully finished.\n" );
        break;
    
    case 1:
        
        printf( "Execution terminated due to multiple cases of providing an incorrect value.\n" );
        break;

    }
}


printUI printKboardInputMenu() {
    system( "clear" );
    printf( "Confirm to procceed to keyboard input.\n\t\t-----\n" );
    printf( "1 - confirm and proceed to strings input. Can consist of symbols from standard 7 bit ASCII table.\n" );
    printf( "2 - confirm and proceed to input real numbers. Can be provided in decimal or exponential format.\n");
    printf( "0 - return to main menu.\n");
}


printUI printDoubleIsSet() {
    printf( "Contents type and operations set to double (real numbers).\n");
}


printUI printStringIsSet() {
    printf( "Contents type and operations set to strings.\n");
}


printUI printArrayStorage( ArrayStorage *storage ) {
    printf( "Enter a number of array you want to work with.\n" );
    printf( "Current array storage:\n" );
    printf( "%p\n", storage->arrayPtrs[0]);
    printf("%d\n", storage->arrayPtrs[0]->size);

    for ( short index = 0; index < storage->count; index++ ) {
        printf("%d. Of type %s, containing %d elements.\n", index + 1, storage->arrayPtrs[index]->typeInfo->typeName, storage->arrayPtrs[index]->size );
    }
}


printUI printArrayContents( const Arr *array ) {
    printf( "Current content of provided array:\n");

    size_t shift = 0;
    for ( int index = 0; index < array->size; index++, shift += array->typeInfo->getSize() ) {
        array->typeInfo->print( ( char * ) array->head + shift );
        if ( index < array->size - 1 ) { printf( ", " ); }
        else { printf(".\n"); }
    }
    printf("\n");
}


printUI printSortingMenu() {
    system( "clear" );
    printf( "Choose sorting method. Again, enter a number of chosen method.\n" );
    printf( "0. Return to main menu.\n" );
    printf( "1. Ascending bubble sort.\n" );
    printf( "2. Descending bubble sort.\n" );
    printf( "3. Ascending heap sort.\n" );
    printf( "4. Descending heap sort.\n" );
}


printUI printArrayManagingMenu() {
    system( "clear" );
    printf( "Choose operation to perform on arrays. Again, enter a number of chosen operation.\n" );
    printf( "0. Return to main menu.\n" );
    printf( "1. Sort one of the arrays available in the storage.\n" );
    printf( "2. Concatenate two of the arrays available in the storage.\n" );
    printf( "3. Perform map() operation on one of the arrays available in the storage.\n" );
    printf( "4. Perform where() operation on one of the arrays available in the storage.\n" );
}