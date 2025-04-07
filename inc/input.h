#ifndef H_INPUT
#define H_INPUT


#include "util.h"
#include "collection.h"
#include "typeinfo.h"


Exception receiver(char **, int *, FILE *);
Exception cmdInput( int *, const int ); 
Exception kboardInput( Arr *, const TypeInfo * );

#endif