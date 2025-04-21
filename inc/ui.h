#ifndef UI_H
#define UI_H

#include "util.h"
#include "collection.h"


typedef void printMenu;
typedef void printMessage;
typedef void printData;


printMessage printError( const Exception );
printMessage printExit();
printMessage printKboardInputMenu();
printMessage printDoubleIsSet();

printMenu printStringIsSet();
printMenu printMapMenu();
printMenu printWhereMenu();
printMenu printArrayManagingMenu();
printMenu printMainMenu();
printMenu printSortingMenu();
printMenu printConcatMenu();

printData printArrayContents( const DynamicArray * );
printData printArrayStorage( ArrayStorage * );


#endif