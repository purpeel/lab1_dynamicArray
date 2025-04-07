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


printUI printArrayContents( const Arr *array ) {
    // system( "clear" );
    printf( "Current content of provided array:\n");
    printf( "%p\n", array->TI);

    size_t shift = 0;
    for ( int index = 0; index < array->size; index++, shift +=array->TI->getSize() ) {
        array->TI->print( ( char * ) array->head + shift );

        printf( ", " );
    }
    printf("\n");
}


printUI printArrayManagingMenu( const Arr *array ) {
    // system( "clear" );
    printArrayContents( array );

}