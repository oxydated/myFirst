//
//  setTexture.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 07/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_setTexture_h
#define myFirst_setTexture_h

#ifdef _WIN32
#include <GL/glew.h>

#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

#include <math.h>
#include <stdio.h>

void setTexture(int program);
void setSampler( int theSampler, int program );

#endif
