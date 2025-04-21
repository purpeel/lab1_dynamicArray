#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/input.h"


const unsigned MAX_SIZE = 1024 * 1024;
const unsigned CHUNK_SIZE = 128;


Exception charToInt( const char *source, int *value ) {
    int len = 0;
    
    for ( short index = 0; *( source + index ) != '\0' || *( source + index ) != '\n'; index++ ) {
        int asciiCode = ( int ) *( source + index );
        
        if ( asciiCode == '\0' || asciiCode == '\n' ) { 
            break;
        } else {
            len++;
        }
    }

    if ( len == 0 ) {
        return ZERO_LENGTH_INPUT_ERROR;
    }

    for ( short index = 0; *( source + index ) != '\0' || *( source + index ) != '\n'; index++ ) {
        int asciiCode = ( int ) *( source + index );
        if ( asciiCode == '\0' || asciiCode == '\n' ) { 
            break;
        }

        if ( asciiCode < '0' || asciiCode > '9' ) {
            return UNEXPECTED_CHAR_ERROR;
        } else {
            *value += ( asciiCode - '0') * power( 10, len - index - 1);
        }
    }

    return SUCCESSFUL_EXECUTION;
}


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


Exception cmdInput( int *cmd, const int minContext, const int maxContext ) {
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

        if ( *cmd < minContext || *cmd > maxContext ) { return CMD_OUT_OF_CONTEXT_ERROR; }
        else { return SUCCESSFUL_EXECUTION; }

    default:

        return exeCode;

    }
}