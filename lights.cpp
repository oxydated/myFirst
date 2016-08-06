//
//  lights.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 28/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include <iostream>
#include "lights.h"


void setLightPosition( GLuint program, float x, float y, float z ){

    GLuint lightPosLocation = glGetUniformLocation( program, (const GLchar*)"vLightPos");
    printf("lightPosLocation: %i\n", lightPosLocation);
    glUniform3f(lightPosLocation, x, y, z);
}