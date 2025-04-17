#include <stdio.h>
#include <stdlib.h>
#include "../inc/menu.h"


// int isArgvCorrectCtrl( int argc, const char **argv ) {
//     if ( argc == 1 || containsFilename( argv ) == 0 ) {
//         return 11;
//     } else {
//         return 0;
//     }
// }


execMenu chooseArray( DynamicArray **array ) {
    int cmd;
    
    printArrayStorage( getStorage() );

    Exception cmdStatus = cmdInput( &cmd, getStorage()->count );

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        *array = getStorage()->arrayPtrs[cmd - 1];
    } else {
        printError( cmdStatus );
    }
} 


execMenu mapMenu() {
    int cmd;
    DynamicArray *array;

    chooseArray( &array );
    printMapMenu();

    Exception cmdStatus = cmdInput(&cmd, 3 );
    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        switch ( cmd )
        {
        case 1:

            map( array, array->typeInfo->setForMap[0] );
            break;
        
        case 2:

            map( array, array->typeInfo->setForMap[1] );
            break;
        
        case 3: 

            map( array, array->typeInfo->setForMap[2] );
            break;
        
        default:
            break;
        }

        printArrayContents( array );

    } else {
        printError( cmdStatus );
    }

}


execMenu sortingMenu() {
    int cmd;
    DynamicArray *array;

    chooseArray( &array );     
    printSortingMenu();

    Exception cmdStatus = cmdInput( &cmd, 4 );
    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        switch ( cmd )
        {
        case 0:
            
            break;
        
        case 1:
            
            printArrayContents( array );
            bubbleSort( array, ASCENDING_ORDER );
            printArrayContents( array );
            break;
        
        case 2:

            printArrayContents( array );
            bubbleSort( array, DESCENDING_ORDER );
            printArrayContents( array );
            break;

        case 3:
            
            printArrayContents( array );
            heapSort( array, ASCENDING_ORDER );
            printArrayContents( array );
            break;
        
        case 4:

            printArrayContents( array );
            heapSort( array, DESCENDING_ORDER );
            printArrayContents( array );
            break;

            
        default:

            break;
        }
    } else {
        printError( cmdStatus );
    }
}


execMenu concatMenu() {
    int cmd;
    DynamicArray *array1, *array2, *resArray;

    printConcatMenu();
    Exception cmdStatus = cmdInput( &cmd, 1 );

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        switch ( cmd )
        {
        case 0:

            break;

        case 1:

            chooseArray( &array1 );
            chooseArray( &array2 );

            Exception concatStatus = concatenate( &resArray, array1, array2 );
            if ( concatStatus != SUCCESSFUL_EXECUTION ) {
                printError( concatStatus );
                break;
            }
            printArrayContents( resArray );
            break;

        default:

            break;

        }
    } else {
        printError( cmdStatus );
    }
}


execMenu arrayManaging() {
    int cmd;

    printArrayManagingMenu();
    Exception cmdStatus = cmdInput( &cmd, 4 );

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {

        switch ( cmd )
        {
        case 0:

            break;
            
        case 1:

            if ( getStorage()->count == 0 ) {
                printError( EMPTY_STORAGE_ERROR );
                break;
            }
            
            sortingMenu();
            break;

        case 2:

            if ( getStorage()->count < 1 ) {
                printError( TOO_FEW_ARRAYS_ERROR );
                break;
            }

            concatMenu();
            break;

        case 3:
            
            if ( getStorage()->count == 0 ) {
                printError( EMPTY_STORAGE_ERROR );
                break;
            }

            mapMenu();
            break;
        
        default:
            break;
        }
    } else {
        printError( cmdStatus );
    }

    return 0;
}


execMenu kboardInputMenu() {
    int cmd, length;
    char *input = NULL;
    DynamicArray *array;

    printKboardInputMenu();
    Exception cmdStatus = cmdInput( &cmd, 3 );
    Exception receiverStatus, initStatus, readStatus;

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        
        switch ( cmd )
        {
        case 0:

            // delete( array );
            break;
        
        case 1:

            printStringIsSet();

            receiverStatus = receiver( &input, &length, stdin );
            if ( receiverStatus != SUCCESSFUL_EXECUTION ) {
                printError( receiverStatus );
                free( input );
                break;
            }

            initStatus = init( &array, getStringTI(), 2 );
            if ( initStatus != SUCCESSFUL_EXECUTION ) {
                printError( initStatus );
                free( input );
                delete( array );
                break;
            }

            readStatus = readFromInput(array, input, length);
            if ( readStatus != SUCCESSFUL_EXECUTION ) {
                printError( readStatus );
                free( input );
                delete( array );
                break;
            }

            free( input );

            arrayManaging();

            break;
            
        case 2:

            printDoubleIsSet();
            receiverStatus = receiver( &input, &length, stdin );
            if ( receiverStatus != SUCCESSFUL_EXECUTION ) {
                printError( receiverStatus );
                free( input );
                break;
            }

            initStatus = init( &array, getDoubleTI(), 2 );
            if ( initStatus != SUCCESSFUL_EXECUTION ) {
                printError( initStatus );
                free( input );
                delete( array );
                break;
            }

            readStatus = readFromInput(array, input, length);
            if ( readStatus != SUCCESSFUL_EXECUTION ) {
                printError( readStatus );
                free( input );
                delete( array );
                break;
            }

            free( input );

            arrayManaging();
            break;
            
        default:
            break;
        }
    } else {
        printError( cmdStatus );
    }
    return 0;
}


execMenu mainMenu( int argc, const char **argv ) {
    int cmd;
    menuDirective isStopSignal = USER_CONTINUE;

    while ( isStopSignal == USER_CONTINUE ) {
        printMainMenu();
        Exception cmdStatus = cmdInput( &cmd, 3 );
        if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
            switch ( cmd )
            {
            case 0:

                printExit( USER_EXIT );
                isStopSignal = USER_EXIT;
                break;
    
            case 1:
                
                kboardInputMenu();
                break;
            
            case 2:
                
                // kboardInputMenu();
                break;
            
            case 3:

                arrayManaging();
                break;
            
            default:
    
                break;
            }
        } else {
            printError( cmdStatus );

            continue;
        }
    }
}