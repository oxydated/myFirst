//
//  shaders.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 06/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#ifdef _WIN32
#include "../stdafx.h"
#endif

#include "shaders.h"
#include "string.h"
#include "stdio.h"
#include "vertexArrays.h"
#include "linearAlg.h"
#include "../macroUtilities.h"

GLint loadAndCompileShader(){
    GLchar** tester = new GLchar*[2];
	int tester_0_size = 0;
	int tester_1_size = 0;

	//loadShaders((char*)tester[0], tester_0_size, (char*)tester[1], tester_1_size);
    loadShaders(tester[0], tester_0_size, tester[1], tester_1_size);
   
    GLboolean shaderCompiler = GL_FALSE;
    glGetBooleanv(GL_SHADER_COMPILER, &shaderCompiler);
    if(shaderCompiler == GL_TRUE){
        printf("it can compile!\n");
    }
    
    GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
    
    GLint vShaderLen[] = { strlen(tester[0]) };
    
    glShaderSource( vShader, 1, (const GLchar**)tester, (const GLint*) vShaderLen );
    
    glCompileShader(vShader);
    
    GLint isCompiled = GL_FALSE;
    
    glGetShaderiv( vShader, GL_COMPILE_STATUS, (GLint*) &isCompiled );
    
    if(isCompiled == GL_FALSE){
		PRINTIT("compilation failed!\n");
    }else{
		PRINTIT("vShader compilation went fine\n");
    }
    
    GLint infoLogLength = 0;
    glGetShaderiv( vShader, GL_INFO_LOG_LENGTH, (GLint*) &infoLogLength );
    GLchar* shaderInfoLog = new GLchar[infoLogLength];
	glGetShaderInfoLog(vShader, infoLogLength, NULL, shaderInfoLog);

	char vshaderInfoString[200];
	sprintf_s(vshaderInfoString, "For vShader: %s\n", shaderInfoLog);
	OutputDebugStringA(vshaderInfoString);

    //delete shaderInfoLog;

    
    GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER ); 
    
    GLint fShaderLen[] = { strlen(tester[1]) };
    
    glShaderSource( fShader, 1, (const GLchar**)&(tester[1]), (const GLint*) fShaderLen );
    
    glCompileShader(fShader);
    
    isCompiled = GL_FALSE;
    
    glGetShaderiv( fShader, GL_COMPILE_STATUS, (GLint*) &isCompiled );
    
    if(isCompiled == GL_FALSE){
		PRINTIT("fShader compilation FAILED!\n");
    }else{
		PRINTIT("fShader compilation went fine\n");
    }
    
    glGetShaderiv( fShader, GL_INFO_LOG_LENGTH, (GLint*) &infoLogLength );
    shaderInfoLog = new GLchar[infoLogLength];
    glGetShaderInfoLog( fShader, infoLogLength, NULL, shaderInfoLog);

	char shaderInfoString[200];
	sprintf_s(shaderInfoString, "For fShader: %s\n", shaderInfoLog);
	OutputDebugStringA(shaderInfoString);

    delete shaderInfoLog;
    
    GLuint program = glCreateProgram();
    printf("program %i created\n", program);
    
    glAttachShader( program, vShader );
    glAttachShader( program, fShader );
    
    glBindAttribLocation(program, VERTEX_POSITION_ATT, "vPosition" );
    
    glBindAttribLocation(program, VERTEX_TEXCOORD_ATT, "vTexCoord" );
    //
    glBindAttribLocation(program, VERTEX_NORMAL_ATT, "vNormal" );
	//
	glBindAttribLocation(program, VERTEX_BONE_NUM_ATT, "boneNumVertAttrib" );

	glBindAttribLocation(program, VERTEX_BONE_OFFSET_ATT, "boneOffsetVertAttrib" );

	//////////////////// uniform array test
    
	//glBindAttribLocation(program, VERTEX_WEIGHT_ATT, "vIndices");

	///////////////////////////////////////
    
    glLinkProgram( program );
    
    GLint isLinked = GL_FALSE;
    GLuint programStatus = program;
    
    printf("address of program: %0x, sizeof program: %i, address of isLinked: %0x, sizeof isLinked: %i\n",  &program, sizeof(program),
           &isLinked, sizeof(isLinked));
    
    glGetProgramiv( programStatus, GL_LINK_STATUS, (GLint*) &isLinked );
    
    if(isLinked == GL_FALSE){
		PRINTIT("Program link FAILED!\n");
    }else{
		PRINTIT("Program link went fine\n");
    }
    
    glGetProgramiv( programStatus, GL_INFO_LOG_LENGTH, (GLint*)&infoLogLength );
    GLchar* programInfoLog = new GLchar[infoLogLength];
    glGetProgramInfoLog( program, infoLogLength, NULL, programInfoLog );
    printf("For program %i linking: %s\n", program, programInfoLog);
    delete programInfoLog;
    
    glUseProgram(program);
    
    printf("Validating program\n");
    glValidateProgram(program);
    GLint isValid = GL_FALSE;
    glGetProgramiv( program, GL_VALIDATE_STATUS, (GLint*)&isValid);
    
    if(isValid == GL_FALSE){
		PRINTIT("program is invalid!\n");
    }else{
		PRINTIT("program is valid...\n");
    }

    return program;
}

void linkShaders(){
    
}
