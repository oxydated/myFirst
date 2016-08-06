//
//  lights.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 28/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_lights_h
#define myFirst_lights_h

#include <iostream>

#ifdef _WIN32
#include <glew.h>

#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

#include "shaders.h"



void setLightPosition( GLuint program, float x, float y, float z );



#endif
