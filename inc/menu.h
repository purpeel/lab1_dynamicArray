#ifndef CONTROLS_H
#define CONTROLS_H


#include "stringTypeinfo.h"
#include "doubleTypeinfo.h"
#include "sort.h"
#include "ui.h"
#include "input.h"
#include "collection.h"


typedef enum {
    USER_EXIT,
    USER_CONTINUE
} menuDirective;


typedef menuDirective execMenu;

execMenu mainMenu();


#endif