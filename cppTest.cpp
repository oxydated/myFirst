//
//  cppTest.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 28/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "cppTest.h"
#include "classCppTest.h"

void printMyClassName(Class cls){
//    fprintf(stderr, "found at last! class name: %s\n",  class_getName(cls));
    myTestClass myObject;
    myObject.PrintTheClassName(class_getName(cls));
} 

