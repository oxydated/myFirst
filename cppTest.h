//
//  cppTest.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 28/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_cppTest_h
#define myFirst_cppTest_h

#include <stdio.h>
#include <objc/runtime.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void printMyClassName(Class cls); 
    

#endif
