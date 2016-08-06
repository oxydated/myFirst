//
//  shaders.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 06/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_shaders_h
#define myFirst_shaders_h

#include <iostream>
#include <cstdio>

#ifdef _WIN32
#include <glew.h>

#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

void loadShaders(char* &vertexShader, int &vsSize, char* &fragmentShader, int &fsSize);

GLint loadAndCompileShader();

void linkShaders();

#endif
