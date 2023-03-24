//
//  createTexture.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 07/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_createTexture_h
#define myFirst_createTexture_h

#include <math.h>
#include <stdio.h>

unsigned char* generateTextureData( int width, int height );

unsigned char* generateTextureDataFromFile( char* fileName, char* extension );

void destroyTextureData();

#endif
