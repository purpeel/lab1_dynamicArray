#include <stdio.h>
#include <stdlib.h>
#include "../inc/menu.h"


Exception getArray( DynamicArray **array ) {
    int cmd;
    
    printArrayStorage( getStorage() );

    Exception cmdStatus = cmdInput( &cmd, 1, getStorage()->count );

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        *array = getStorage()->arrays[cmd - 1];
    } else {
        return cmdStatus;
    }

    return SUCCESSFUL_EXECUTION;
} 


execMenu mapMenu() {
    int cmd;
    DynamicArray *array;

    Exception choiceStatus = getArray( &array );
    if ( choiceStatus != SUCCESSFUL_EXECUTION ) {
        printError( choiceStatus );
        return USER_CONTINUE;
    }
    printMapMenu();

    Exception cmdStatus = cmdInput(&cmd, 0, 3 );
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

    return USER_CONTINUE;
}


execMenu whereMenu() {
    int cmd;
    DynamicArray *array;
    Exception whereStatus;

    Exception choiceStatus = getArray( &array );
    if ( choiceStatus != SUCCESSFUL_EXECUTION ) {
        printError( choiceStatus );
        return USER_CONTINUE;
    }
    printWhereMenu();

    Exception cmdStatus = cmdInput(&cmd, 0, 3);
    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        switch ( cmd )
        {
        case 1:

            whereStatus = where(array, array->typeInfo->setForWhere[0]);
            if ( whereStatus != SUCCESSFUL_EXECUTION ) {
                printError( whereStatus );
                return USER_CONTINUE;
            }
            break;

        case 2:

            whereStatus = where(array, array->typeInfo->setForWhere[1]);
            if ( whereStatus != SUCCESSFUL_EXECUTION ) {
                printError( whereStatus );
                return USER_CONTINUE;
            }
            break;

        case 3:

            whereStatus = where(array, array->typeInfo->setForWhere[2]);
            if ( whereStatus != SUCCESSFUL_EXECUTION ) {
                printError( whereStatus );
                return USER_CONTINUE;
            }
            break;

        default:
            break;
        }

        printArrayContents( getStorage()->arrays[getStorage()->count - 1] );

    } else {
        printError( cmdStatus );
    }

    return USER_CONTINUE;
}


execMenu sortingMenu() {
    int cmd;
    DynamicArray *array;

    Exception choiceStatus = getArray( &array );
    if ( choiceStatus != SUCCESSFUL_EXECUTION ) {
        printError( choiceStatus );
        return USER_CONTINUE;
    }  
    printSortingMenu();

    Exception cmdStatus = cmdInput( &cmd, 0, 4 );
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

    return USER_CONTINUE;
}


execMenu concatMenu() {
    int cmd;
    DynamicArray *array1, *array2, *resArray;

    printConcatMenu();
    Exception cmdStatus = cmdInput( &cmd, 0, 1 );

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        switch ( cmd )
        {
        case 0:

            break;

        case 1:

            Exception choiceStatus = getArray( &array1 );
            if ( choiceStatus != SUCCESSFUL_EXECUTION ) {
                printError( choiceStatus );
                return USER_CONTINUE;
            }  
            choiceStatus = getArray( &array2 );
            if ( choiceStatus != SUCCESSFUL_EXECUTION ) {
                printError( choiceStatus );
                return USER_CONTINUE;
            }  

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

    return USER_CONTINUE;
}


execMenu arrayManaging() {
    int cmd;

    printArrayManagingMenu();
    Exception cmdStatus = cmdInput( &cmd, 0, 4 );

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

            if ( getStorage()->count == 0 ) {
                printError( EMPTY_STORAGE_ERROR );
                break;
            }

            whereMenu();
            break;
        }
    } else {
        printError( cmdStatus );
    }

    return 0;

    return USER_CONTINUE;
}


execMenu kboardInputMenu() {
    int cmd, length;
    char *input = NULL;
    DynamicArray *array;

    printKboardInputMenu();
    Exception cmdStatus = cmdInput( &cmd, 0, 3 );
    Exception status;

    if ( cmdStatus == SUCCESSFUL_EXECUTION ) {
        
        switch ( cmd )
        {
        case 0:

            break;
        
        case 1:

            printStringIsSet();

            status = receiver( &input, &length, stdin );
            if ( status != SUCCESSFUL_EXECUTION ) {
                printError( status );
                free( input );
                break;
            }

            status = init( &array, getStringTI(), 2 );
            if ( status != SUCCESSFUL_EXECUTION ) {
                printError( status );
                free( input );
                deleteArray( array );
                break;
            }

            status = readFromInput(array, input, length);
            if ( status != SUCCESSFUL_EXECUTION ) {
                printError( status );
                free( input );
                deleteArray( array );
                break;
            }
            free( input );

            arrayManaging();

            break;
            
        case 2:

            printDoubleIsSet();
            status = receiver( &input, &length, stdin );
            if ( status != SUCCESSFUL_EXECUTION ) {
                printError( status );
                free( input );
                break;
            }

            status = init( &array, getDoubleTI(), 2 );
            if ( status != SUCCESSFUL_EXECUTION ) {
                printError( status );
                free( input );
                deleteArray( array );
                break;
            }

            status = readFromInput(array, input, length);
            if ( status != SUCCESSFUL_EXECUTION ) {
                printError( status );
                free( input );
                deleteArray( array );
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

    return USER_CONTINUE;
}


execMenu mainMenu() {
    int cmd;
    menuDirective directive = USER_CONTINUE;

    while ( directive == USER_CONTINUE ) {
        printMainMenu();
        Exception cmdStatus = cmdInput( &cmd, 0, 2 );
        if ( cmdStatus != SUCCESSFUL_EXECUTION ) {
            printError( cmdStatus );
            continue;
        }
        switch ( cmd )
        {
        case 0:

            deleteStorage( getStorage() );
            printExit();
            directive = USER_EXIT;
            break;

        case 1:
            
            kboardInputMenu();
            break;
        
        case 2:

            arrayManaging();
            break;
        
        default:
            break;
        }
    }

    return USER_EXIT;
}