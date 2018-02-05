#include "shaders.h"

int getFileSize( const char* fileName){
	int fileSize = 0;
	FILE* f = fopen(fileName, "r");
	if (f != NULL){
		fseek(f, 0, SEEK_END);
		fileSize = ftell(f);
		fclose(f);
	}
	return fileSize;
}

int loadFileIntoString(const char* fileName, char* &theString, int &stringSize){
	int fileError = 0;
	int fileSize = getFileSize(fileName);
	theString = new char[fileSize + 1];
	memset((void*)theString, '\0', fileSize + 1);
	FILE* f = fopen(fileName, "r");
	fileError = errno;
	if (f != NULL){
		stringSize = fread((void*)theString, fileSize*sizeof(char), 1, f);
		stringSize++;
		fclose(f);
	}
	return fileError;
}

void loadShaders( char* &vertexShader, int &vsSize, char* &fragmentShader, int &fsSize){
	loadFileIntoString("vertexShader_per_pixel_lighting_4_3.vs", vertexShader, vsSize);
	loadFileIntoString("fragmentShader_per_pixel_lighting_4_3.fs", fragmentShader, fsSize);
}