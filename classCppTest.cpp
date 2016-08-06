//
//  classCppTest.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 29/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include "classCppTest.h"

int myTestClass::inc = 0;

void myTestClass::PrintTheClassName( const char* theName ){    
    fprintf(stderr, "found at last! the class name: %s .. inc: %i\n",  theName, inc++);   
}