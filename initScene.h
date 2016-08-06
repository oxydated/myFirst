//
//  initScene.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 06/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_initScene_h
#define myFirst_initScene_h

#ifdef _WIN32
#include "stdafx.h"
#include <glew.h>

#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

//#include <iostream>


#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void initGLES(int x, int y, int width, int height);

#endif
