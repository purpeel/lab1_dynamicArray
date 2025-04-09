#ifndef UI_H
#define UI_H


#include "util.h"
#include "collection.h"


typedef void printUI;


printUI printMainMenu();
printUI printError( const Exception );
printUI printExit( const menuDirective );
printUI printKboardInputMenu();
printUI printDoubleIsSet();
printUI printStringIsSet();
printUI printArrayContents( const Arr * );
printUI printArrayManagingMenu();
printUI printArrayStorage( ArrayStorage * );
printUI printSortingMenu();


#endif