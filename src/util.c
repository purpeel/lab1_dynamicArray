#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/util.h"


double power( const double base, int power ) {
    double res = 1.0;
    if ( power == 0 ) { return res; } 
    if ( power > 0 ) {

        while ( power != 0 ) {
            res *= base;
            power--;
        }

    } else { 

        while ( power != -1 ) {
            res /= base;
            power++;
        }
    }
    return res;
}