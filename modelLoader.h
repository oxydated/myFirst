//
//  modelLoader.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 18/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef myFirst_modelLoader_h
#define myFirst_modelLoader_h

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void loadModel(int *numFaces, int *numVerts, unsigned short *faces, float *vertices, float *texcoord, float *normals );

#endif
