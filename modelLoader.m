//
//  modelLoader.m
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 18/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>

void loadModel(int *numFaces, int *numVerts, unsigned short *faces, float *vertices, float *texcoord, float *normals ){
    //NSString *thePath = [[NSBundle mainBundle] pathForResource:@"thePlane_32_final" ofType:@"OXY"];
    NSString *thePath = [[NSBundle mainBundle] pathForResource:@"theFace_33_final" ofType:@"OXY"];

    FILE *theFile = fopen([thePath UTF8String], "r");
    if( theFile == NULL ){
        printf("DID NOT WORK!\n");
    }else{
        printf("************it did work\n");
        char theLine[100] = "";
        
        /*
         node name: Scene Root
         node name: Box001
         /////////////////////////////////////////////////////////////////
         //////////////////////////////////////////////      vertexBuilder
        */
        
        for( int i = 0; i < 4; i++){
            fgets( theLine, 100, theFile);
            printf("%s\n", theLine);   
        }  
            
        /*
        faces: 
        1256
        */
                
        fgets( theLine, 100, theFile);
        printf("%s\n", theLine);        
            
        fscanf( theFile, "%i\n", numFaces);
        printf("numFaces: %i\n", numFaces[0]); 
            
        for( int i=0; i < numFaces[0]; i++ ){
            int index = 0;        
            if( (faces != NULL) && (vertices != NULL)){ 
                
                /*
                 0		0 1 2
                 */
                fscanf( theFile, "%i %i %i %i\n", &index, &faces[i*3], &faces[i*3+1], &faces[i*3+2] );
                printf("face[%i]: %i %i %i\n", index, faces[i*3], faces[i*3+1], faces[i*3+2]);
            } else {
                fgets( theLine, 100, theFile);
            }
        } 

        /*
        vertices: 
        681
        */            
        fgets( theLine, 100, theFile);
        printf("%s\n", theLine);
            
        fscanf( theFile, "%i\n", numVerts);
        printf("numVerts: %i\n", numVerts[0]); 
        
            
        for( int i=0; i < numVerts[0]; i++ ){
            int index = 0;
            if( (faces != NULL) && (vertices != NULL)){ 
                
                /*
                 0		51		-11,514627, -12,970696, -0,928399 
                */    
                int theVert = -1;
                
                fgets(theLine, 100, theFile );
                char* theChar = NULL;
                do{
                    theChar = strrchr(theLine, ',');
                    if(theChar != NULL) theChar[0] = '.';
                }while( theChar != 0 );
                //printf("%s", theLine);   
                
                
                //            fscanf( theFile, "%i", &index );
                //printf("%s\n", theLine);

                sscanf( theLine, "%i %i %f %f %f\n", &index, &theVert, &vertices[i*3], &vertices[i*3+1], &vertices[i*3+2] );
                printf("%i  vertex[%i]: %f %f %f\n", index, theVert, vertices[i*3], vertices[i*3+1], vertices[i*3+2]);
                
                /*
                        51		0,698728, 0,139680 
                */
                int theTexCoord = -1;
                
                fgets(theLine, 100, theFile );
                theChar = NULL;
                do{
                    theChar = strrchr(theLine, ',');
                    if(theChar != NULL) theChar[0] = '.';
                }while( theChar != 0 );
                
                printf("%s\n", theLine);                
                sscanf( theLine, "%i %f %f\n", &theTexCoord, &texcoord[i*2], &texcoord[i*2+1] );
                printf("TexCoord[%i]: %f %f\n", theTexCoord, texcoord[i*2], texcoord[i*2+1] );

                
                /*
                        0		-0,585541, -0,757004, -0,289976 
                */
                int theNormal = -1;
                
                fgets(theLine, 100, theFile );
                theChar = NULL;
                do{
                    theChar = strrchr(theLine, ',');
                    if(theChar != NULL) theChar[0] = '.';
                }while( theChar != 0 );
                sscanf( theLine, "%i %f %f %f\n", &theNormal, &normals[i*3], &normals[i*3+1], &normals[i*3+2] );
                printf("Normal[%i]: %f %f %f\n", theNormal, normals[i*3], normals[i*3+1], normals[i*3+2]);
                
            } else {
                fgets( theLine, 100, theFile);
                fgets( theLine, 100, theFile);
                fgets( theLine, 100, theFile);
            }
        }
        
        fclose(theFile);
    }
}