#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/input.h"


const unsigned MAX_SIZE = 1024 * 1024;
const unsigned CHUNK_SIZE = 128;


Exception receiver( char **buffer, int *length, FILE *stream ) {
    *buffer = malloc( CHUNK_SIZE );

    if ( *buffer == NULL )
        return MEMORY_ALLOCATION_ERROR;
    char c;
    unsigned index = 0, currentLength = 0;

    while ( 1 ) {
        c = getc( stream );
        *( *buffer + index ) = c;

        if ( index >= MAX_SIZE ) {
            free( *buffer );
            return INPUT_OVERFLOW_ERROR;
        }
        
        if ( index > currentLength ) {
            currentLength += CHUNK_SIZE;
            *buffer = realloc( *buffer, currentLength );
        }

        index++;
        if ( c == '\n' || c == EOF ) {
            break;
        }
    }

    *length = index + 2; // +1 since the indices start with 0 and +1 to remember null-terminator char
    *( *buffer + index + 1 ) = '\0';
    
    return SUCCESSFUL_EXECUTION;
}


Exception cmdInput( int *cmd, const int context ) {
    *cmd = 0;
    char *input = malloc( 48 );
    if ( input == NULL ) {
        return MEMORY_ALLOCATION_ERROR;
    }

    printf( ">> " );
    fgets( input, 48, stdin );
    fflush( stdin );
    
    Exception exeCode = charToInt( input, cmd );
    free(input);

    switch ( exeCode )
    {
    case SUCCESSFUL_EXECUTION:

        if ( *cmd > context ) { return CMD_OUT_OF_CONTEXT_ERROR; }
        else { return SUCCESSFUL_EXECUTION; }

    default:

        return exeCode;

    }
}


// Exception kboardInput( Arr *array, const TypeInfo *TI ) {

//     char *input = NULL;
//     int length;

//     Exception receiverRes = receiver( &input, &length, stdin );
//     if ( receiverRes != SUCCESSFUL_EXECUTION ) {

//         return receiverRes;

//     }

//     init( array, TI );
//     readFromInput( array, input, length );
//     free(input);

//     return SUCCESSFUL_EXECUTION;
// }