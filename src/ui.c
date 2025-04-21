#include <stdio.h>
#include "../inc/ui.h"


printMessage printDoubleIsSet() {
    printf( "Contents type and operations set to double (real numbers).\n");
}


printMessage printStringIsSet() {
    printf( "Contents type and operations set to strings.\n");
}


printMessage printError( const Exception exeCode ) {
    // system( "clear" );
    switch ( exeCode )
    {
    case MEMORY_ALLOCATION_ERROR:
        printf( "Error. Unable to allocate memory.\n" );
        break;
    case INPUT_OVERFLOW_ERROR:
        printf( "Provided input exceeded allowed limit.\n");
        break;
    case UNEXPECTED_CHAR_ERROR:
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
    case EMPTY_STORAGE_ERROR:
        printf( "Error. Storage is currently empty, provide an array to process it.\n" );
        break;
    case ARRAYS_TYPEINFO_MISMATCH_ERROR:
        printf( "Error. Provided arrays are of different types.\n" );
        break;
    case TOO_FEW_ARRAYS_ERROR:
        printf( "Error. There are not enough arrays in the storage to perform concatenation.\n" );
        break;
    case NULL_TYPEINFO_ERROR:
        printf( "Error. Unable to initialize an array without chosing its type.\n" );
        break;
    default:
        break;
    }
    printf( "Try again.\n" );
}


printMessage printExit() {
    // system( "clear" );
    printf( "Exexcution successfully finished.\n" );
}


printMenu printMainMenu() {
    // system( "clear" );
    printf( "Choose one of the listed functions and enter a number of chosen function.\n" );
    printf( "Each function is followed by its number.\n\t\t-----\n" );
    printf( "1 - Enter array contents via keyboard and process them.\n" );
    printf( "2 - procceed to array storage without providing any new arrays.\n");
    printf( "0 - Stop the execution.\n" );
}


printMenu printKboardInputMenu() {
    // system( "clear" );
    printf( "Confirm to procceed to keyboard input.\n\t\t-----\n" );
    printf( "1 - confirm and proceed to strings input. Can consist of symbols from standard 7 bit ASCII table.\n" );
    printf( "2 - confirm and proceed to input real numbers. Can be provided in decimal format.\n");
    printf( "0 - return to main menu.\n");
}


printMenu printMapMenu() {
    // system( "clear" );
    printf( "Choose one of the following functions to perform on a chosen array. Enter a number of chosen function.\n" );
    printf( "0 - Return to main menu.\n" );
    printf( "1 - Invert all elements: returns opposite element for real numbers and inverts the strings\n" );
    printf( "2 - Normalize: returns common logarithm for real numbers and applies toLowerCase() to strings\n" );
    printf( "3 - Sign: return for real numbers is similar to sign(x) function and returns first literal for the strings\n" );
}


printMenu printWhereMenu() {
    // system( "clear" );
    printf( "Choose one of the following functions to perform on a chosen array. Enter a number of chosen function.\n" );
    printf( "0 - Return to main menu.\n" );
    printf( "1 - Find all elements that are \"normalized\": a string is considered normalized when it's beginning with a" 
            "lowercase literal and a real number is considered normalized when it's between 0 and 1.\n" );
    printf( "2 - Perform isRound(): a string consisting of digits is considered round and the"
            "real number is considered round when it has zero fractional part.\n" );
    printf( "3 - Find all positive numbers or all uppercase-provided strings.\n" );
}


printMenu printArrayManagingMenu() {
    // system( "clear" );
    printf( "Choose operation to perform on arrays. Again, enter a number of chosen operation.\n" );
    printf( "0. Return to main menu.\n" );
    printf( "1. Sort one of the arrays available in the storage.\n" );
    printf( "2. Concatenate two of the arrays available in the storage.\n" );
    printf( "3. Perform map() operation on one of the arrays available in the storage.\n" );
    printf( "4. Perform where() operation on one of the arrays available in the storage.\n" );
}


printMenu printConcatMenu() {
    // system( "clear" );
    printf( "0 - Return to main menu.\n" );
    printf( "1 - Choose two arrays to concatenate. Consecutively enter numbers of chosen arrays.\n" );
}


printMenu printSortingMenu() {
    // system( "clear" );
    printf( "Choose sorting method. Again, enter a number of chosen method.\n" );
    printf( "0. Return to main menu.\n" );
    printf( "1. Ascending bubble sort.\n" );
    printf( "2. Descending bubble sort.\n" );
    printf( "3. Ascending heap sort.\n" );
    printf( "4. Descending heap sort.\n" );
}


printData printArrayStorage( ArrayStorage *storage ) {
    if ( storage->count > 0 ) {
        printf( "Enter a number of array you want to work with.\n" );
        printf( "Current array storage:\n" );;

        for ( short index = 0; index < storage->count; index++ ) {
            printf("%d. Of type %s, containing %d elements.\n", index + 1, storage->arrays[index]->typeInfo->typeName, storage->arrays[index]->size );
        }
    }
}


printData printArrayContents( const DynamicArray *array ) {
    printf( "Current content of provided array:\n");

    size_t offset = 0;
    for ( int index = 0; index < array->size; index++, offset += array->typeInfo->getSize() ) {
        array->typeInfo->print( *( ( ElemPtr * ) ( ( char * ) array->data + offset ) ) );
        if ( index < array->size - 1 ) { printf( ", " ); }
        else { printf(".\n"); }
    }
    printf("\n");
}