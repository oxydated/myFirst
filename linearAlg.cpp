//
//  linearAlg.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 14/10/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "linearAlg.h"

void rotationMatrix( float cx, float cy, float teta, int program ){
    GLfloat mat[4] = { cos(teta), sin(teta), -sin(teta), cos(teta) };
    GLint location = glGetUniformLocation( program, "mRotation");
    printf("location of mRotation: %i\n", location);
    glUniformMatrix2fv( location, 1, GL_FALSE, mat );
}

void setPerspectiveMatrix( float *pers, int program ){
    GLint persLocation = glGetUniformLocation( program, "Proj");
    printf("location of Proj: %i\n", persLocation);
    glUniformMatrix4fv( persLocation, 1, GL_FALSE, pers );
}

void setViewportMatrix( float *view, int program ){
    GLint viewLocation = glGetUniformLocation( program, "View" );
    printf("location of View: %i\n", viewLocation);
    if( viewLocation >= 0 ){
		glUniformMatrix4fv( viewLocation, 1, GL_FALSE, view );
	}
}
