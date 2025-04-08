#include <stdio.h>
#include <stdlib.h>
#include "../inc/menu.h"


// // int isArgvCorrectCtrl( int argc, const char **argv ) {
// //     if ( argc == 1 || containsFilename( argv ) == 0 ) {
// //         return 11;
// //     } else {
// //         return 0;
// //     }
// // }


execMenu arrayManaging( Arr *array ) {
    printArrayManagingMenu( array );
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

            arrayManaging( array );

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