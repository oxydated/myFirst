//
//  textureLoader.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 15/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_textureLoader_h
#define myFirst_textureLoader_h

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void getTextureImageFromFile( char* filename, char* ext, unsigned char* buffer );

EXTERN_C unsigned long int getTextureSizeInBytesFromFile( char* filename, char* ext );

#endif
