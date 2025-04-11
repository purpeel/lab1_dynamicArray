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


execMenu chooseArray( Arr **array ) {
    int cmd;
    char *input = NULL;

    printArrayStorage( getStorage() );

    Exception cmdStatus = cmdInput( &cmd, getStorage()->count );

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        *array = getStorage()->arrayPtrs[cmd - 1];
    }
} 


execMenu sortingMenu() {
    int cmd;
    char *input = NULL;
    Arr *array;

    chooseArray( &array );     
    printSortingMenu();

    Exception cmdStatus = cmdInput( &cmd, 4 );
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


}


execMenu arrayManaging() {
    int cmd;
    char *input = NULL;

    printArrayManagingMenu();
    Exception cmdStatus = cmdInput( &cmd, 4 );

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {

        switch ( cmd )
        {
        case 0:

            break;
            
        case 1:
            
            sortingMenu();

            break;
        
        default:
            break;
        }
    }
}


execMenu kboardInputMenu() {
    int cmd, length;
    char *input = NULL;
    Arr *array;

    printKboardInputMenu();
    Exception cmdStatus = cmdInput( &cmd, 2 );

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        
        switch ( cmd )
        {
        case 0:

            break;
        
        case 1:

            printStringIsSet();

            Exception receiverRes = receiver( &input, &length, stdin );
            if ( receiverRes != SUCCESSFUL_EXECUTION ) {
                free( input );
                printError( receiverRes );
                break;
            }

            init( &array, getStringTI() );
            readFromInput( array, input, length );
            free( input );

            printf( "%p\n", array );

            arrayManaging();

            break;
            
        case 2:

            printDoubleIsSet();
            // kboardInput( &array, getDoubleTI() );
            break;
            
        default:
            break;
        }
    } else {
        printError( cmdStatus );
    }
    return 0;
}



// execMenu mainMenu( const int argc, const char **argv ) {
//     int cmd = 0;
//     char *input;
    
//     menuDirective isStopSignal = USER_CONTINUE;

//     while ( isStopSignal == USER_CONTINUE ) {
//         printMainMenu();
//         isStopSignal = gateway( cmdInput( &cmd, input, 2 ), 0 );
        
//         switch ( cmd )
//         {
//         case 0:
    
//             printExit( USER_EXIT );
//             isStopSignal = USER_EXIT;
//             break;
    
//         case 1:
    
//             isStopSignal = kboardInputMenu();
//             break;
    
//         case 2:
    
//             // fileInputVw( argc );
//             // gatewayCtrl( isArgvCorrectCtrl( argc, argv ) );
//             // gatewayCtrl( fileInputCtrl( argc, argv ) );
    
//             break;
    
//         default:
    
            
//             break;
        
//         }
//     }
// }


execMenu mainMenu( int argc, const char **argv ) {
    int cmd;
    menuDirective isStopSignal = USER_CONTINUE;

    while ( isStopSignal == USER_CONTINUE ) {
        printMainMenu();
        Exception cmdStatus = cmdInput( &cmd, 2 );
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