//
//  vertexArrays.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 06/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_vertexArrays_h
#define myFirst_vertexArrays_h

//#include <iostream>

#ifdef _WIN32
#include <glew.h>

#else
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

#define VERTEX_POSITION_ATT 0
#define VERTEX_POSITION_SIZE 3

#define VERTEX_TEXCOORD_ATT 1
#define VERTEX_TEXCOORD_SIZE 2

#define VERTEX_NORMAL_ATT 2
#define VERTEX_NORMAL_SIZE 3

#define VERTEX_BONE_NUM_ATT	3	// int boneNumVertAttrib;

#define VERTEX_BONE_OFFSET_ATT 4 // int boneOffsetVertAttrib;

///////////////////// uniform array test

#define VERTEX_WEIGHT_ATT 3
#define VERTEX_WEIGHT_SIZE 1

////////////////////////////////////////

void createVertexBuffer();

void bindVertexBuffer();

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void drawVertexArray();

EXTERN_C void reorient();

void setProgram( GLint program );

#endif
