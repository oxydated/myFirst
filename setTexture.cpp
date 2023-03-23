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

#include <vector>
#include "setTexture.h"
#include "createTexture.h"
#include "shaders.h"
#include "imageLoader.h"

#define SPIT_ERROR     if(glGetError()!= GL_NO_ERROR){ printf("error up to line %i\n", __LINE__ );} else {printf("ok in %i\n", __LINE__ );}

void setTexture(int program){
    unsigned int width = 512;
	unsigned int height = 512;
    int theGLerror = 0;

	//std::vector<unsigned char> &&imageData = oxyde::utility::loadImageFromFile(L"wireframegirl_3.png", width, height);
	std::vector<unsigned char> &&imageData = oxyde::utility::loadImageFromFile(L"CMan0002-M3-Head-D-small.jpg", width, height);
	
    unsigned int textureName = 0;
    //glGenTextures( 1, &textureName);
    printf("textureName: %i\n", textureName );
    //glBindTexture( GL_TEXTURE_2D, textureName );
    
 //   SPIT_ERROR
 //   
 //   glActiveTexture(GL_TEXTURE0);
 //   
	//SPIT_ERROR

	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//SPIT_ERROR

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//SPIT_ERROR

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	//SPIT_ERROR

	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//SPIT_ERROR

	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)imageData.data());

	//SPIT_ERROR
	//	if (glGetError() != GL_NO_ERROR) {
	//	printf("error up to line %i\n", __LINE__);
	//}
	//else {
	//	printf("ok in %i\n", __LINE__);
	//}

	//glGenerateMipmap(GL_TEXTURE_2D);
 //   
	//SPIT_ERROR

	setSampler(0, program);

}

void setSampler( int theSampler, int program ){
    //GLint samplerLocation = glGetUniformLocation( program, "theSampler");
    //
    //SPIT_ERROR

    //printf("location of theSampler: %i\n", samplerLocation);

    //glUniform1i( samplerLocation, theSampler );
    //
    //SPIT_ERROR

}


