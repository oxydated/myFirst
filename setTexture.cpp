//
//  setTexture.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 07/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "setTexture.h"
#include "createTexture.h"
#include "shaders.h"

#define SPIT_ERROR     if(glGetError()!= GL_NO_ERROR){ printf("error up to line %i\n", __LINE__ );} else {printf("ok in %i\n", __LINE__ );}

void setTexture(int program){
    int width = 512;
    int height = 512;
    int theGLerror = 0;
//    unsigned char* theImage = generateTextureData( width, height );
	//unsigned char* theImage = generateTextureDataFromFile( "CMan0002-M3-Head-D-small", "bmp" );
	unsigned char* theImage = generateTextureDataFromFile("CMan0002-M3-Head-D-small", "bmp");
    unsigned int textureName = 0;
    glGenTextures( 1, &textureName);
    printf("textureName: %i\n", textureName );
    glBindTexture( GL_TEXTURE_2D, textureName );
    
    SPIT_ERROR
    
    glActiveTexture(GL_TEXTURE0);
    
    SPIT_ERROR

    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	SPIT_ERROR

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SPIT_ERROR

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	SPIT_ERROR

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	SPIT_ERROR

    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)theImage);
    
    SPIT_ERROR
	if (glGetError() != GL_NO_ERROR){
		printf("error up to line %i\n", __LINE__);
	}
	else {
		printf("ok in %i\n", __LINE__);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
    
	SPIT_ERROR

	setSampler(0, program);

}

void setSampler( int theSampler, int program ){
    GLint samplerLocation = glGetUniformLocation( program, "theSampler");
    
    SPIT_ERROR

    printf("location of theSampler: %i\n", samplerLocation);
    //glUniformMatrix4fv( persLocation, 1, GL_FALSE, pers );
    glUniform1i( samplerLocation, theSampler );
    
    SPIT_ERROR

}

