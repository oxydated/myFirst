//
//  textureLoader.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 15/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//#include <iostream>
#include <stdio.h>
#include "aImage.h"

unsigned long int getTextureSizeInBytesFromFile( char* filename, char* ext ){
    NSString* theFileName = [[NSString alloc]initWithCString:filename encoding:NSASCIIStringEncoding];
    NSString* theExtension = [[NSString alloc]initWithCString:ext encoding:NSASCIIStringEncoding];
    aImage* myInstance = [aImage imageFromBundleContentsWithName:theFileName forExtension:theExtension];
    unsigned long int retValue =  [myInstance imageDataSize];
    [myInstance release];
    return retValue;
}

void getTextureImageFromFile( char* filename, char* ext, unsigned char* buffer ){
    NSString* theFileName = [[NSString alloc]initWithCString:filename encoding:NSASCIIStringEncoding];
    NSString* theExtension = [[NSString alloc]initWithCString:ext encoding:NSASCIIStringEncoding];
    aImage* myInstance = [aImage imageFromBundleContentsWithName:theFileName forExtension:theExtension];
    [myInstance copyImageDataIn:(UInt8*)buffer];
}
