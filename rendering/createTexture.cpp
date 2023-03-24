//
//  createTexture.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 07/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "../stdafx.h"
#endif

#include "createTexture.h"
#include "../files/textureLoader.h"

unsigned char *theData = NULL;

unsigned char* generateTextureData( int width, int height ){
    theData = new unsigned char[4*width*height];
    for ( int bx = 0; bx < width; bx++){
        for( int by = 0; by < height; by++){
            bool blackX = false;
            bool blackY = false;
            blackX = (bx < (width/2))? true: false;
            blackY = (by < (height/2))? true: false;
            
            unsigned char color = (blackX == blackY)? 0xff: 0x00;
            
            int index = (bx+by*width)*4;
            theData[index+0] = color;
            theData[index+1] = 0x00;
            theData[index+2] = 0xff-color;
            theData[index+3] = 0xff;
        }
    }
    
    return theData;
}

unsigned char* generateTextureDataFromFile( char* fileName, char* extension ){
	int dataSize = getTextureSizeInBytesFromFile( fileName, extension );
    theData = new unsigned char[dataSize];
    getTextureImageFromFile( fileName, extension, theData );
    return theData;
}

void destroyTextureData(){
    if( theData != NULL){
        delete theData;
        theData = NULL;
    }
}
