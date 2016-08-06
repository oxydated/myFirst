//
//  textureLoader.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 15/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//#include <iostream>
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#endif
#include <stdio.h>
#include <Windows.h>
#include <cstring>

//#include "aImage.h"

//unsigned long int getTextureSizeInBytesFromFile( char* filename, char* ext ){
//    NSString* theFileName = [[NSString alloc]initWithCString:filename encoding:NSASCIIStringEncoding];
//    NSString* theExtension = [[NSString alloc]initWithCString:ext encoding:NSASCIIStringEncoding];
//    aImage* myInstance = [aImage imageFromBundleContentsWithName:theFileName forExtension:theExtension];
//    unsigned long int retValue =  [myInstance imageDataSize];
//    [myInstance release];
//    return retValue;
//}
//
//void getTextureImageFromFile( char* filename, char* ext, unsigned char* buffer ){
//    NSString* theFileName = [[NSString alloc]initWithCString:filename encoding:NSASCIIStringEncoding];
//    NSString* theExtension = [[NSString alloc]initWithCString:ext encoding:NSASCIIStringEncoding];
//    aImage* myInstance = [aImage imageFromBundleContentsWithName:theFileName forExtension:theExtension];
//    [myInstance copyImageDataIn:(UInt8*)buffer];
//}

EXTERN_C unsigned long int getTextureSizeInBytesFromFile( char* filename, char* ext ){
	char thePath[100] = "\n";
	strcpy( thePath, filename );
	strcat( thePath, "." );
	strcat( thePath, ext );
	//strcat( thePath, "\n" );
	//LoadImageA( NULL, (LPCSTR)thePath, 0, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE  ); 
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	char theCurrentDirectory[50];
	GetCurrentDirectoryA( 50, (LPSTR)theCurrentDirectory);

	
	FILE* theFile  = NULL;
	errno = 0;
	//system("pwd");
	theFile = fopen( (const char*)thePath, "r");
	//theFile = fopen("G:\\myProjects\\myFirst\\Debug\\CMan0002-M3-Head-D-small.bmp", "r");
	//theFile = fopen("G:\\myProjects\\myFirst\\Debug\\CMan0002-M3-Head-D-small.bmp", "r");
	//theFile = fopen( "CMan0002.bmp", "r");
	//char testChar[100];
	//int A = errno;
	char *testChar = strerror(errno);

	//int testErr = ferror(theFile);


	int retSize = 0;

	int byteRead = fread( &bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, theFile );
	byteRead = fread( &bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, theFile );
	bool allStructureLoaded = byteRead*sizeof(BITMAPINFOHEADER) == bmpInfoHeader.biSize;
	if( allStructureLoaded){
		retSize = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 4;
	}

	int B = fclose(theFile);
	//return 1048576;
	return retSize;
}

EXTERN_C void getTextureImageFromFile( char* filename, char* ext, unsigned char* buffer ){
	char thePath[100] = "\n";
	strcpy( thePath, filename );
	strcat( thePath, "." );
	strcat( thePath, ext );
	//strcat( thePath, "\n" );
	//LoadImageA( NULL, (LPCSTR)thePath, 0, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE  ); 
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;

	char theCurrentDirectory[50];
	GetCurrentDirectoryA( 50, (LPSTR)theCurrentDirectory);

	
	FILE* theFile  = NULL;
	theFile = fopen( (const char*)thePath, "rb");
	//theFile = fopen( "CMan0002.bmp", "r");
	int A = errno;

	int retSize = 0;

	int byteRead = fread( &bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, theFile );
	byteRead = fread( &bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, theFile );
	bool allStructureLoaded = byteRead*sizeof(BITMAPINFOHEADER) == bmpInfoHeader.biSize;
	if( allStructureLoaded){
		//retSize = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 4;

		unsigned char* tempBuffer = new unsigned char[bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3];
		int byteRead = fread( tempBuffer, sizeof(unsigned char), bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3, theFile );
		int pixelSize = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight;
		for( int i = 0; i < pixelSize; i++ ){
			int j = pixelSize - i;
			buffer[j*4+2] = tempBuffer[i*3+0];
			buffer[j*4+1] = tempBuffer[i*3+1];
			buffer[j*4+0] = tempBuffer[i*3+2];
			buffer[j*4+3] = 0x00;
		}
	}

	int B = fclose(theFile);
	//return 1048576;
	//return retSize;
}
