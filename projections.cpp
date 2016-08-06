//
//  projections.cpp
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 04/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifdef _WIN32
#include "stdafx.h"
#endif

#include "projections.h"

static float tm[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

void perspectiveMatrix( float l, float r, float n, float f, float b, float t, float *m, float *res ){
    tm[0]= 	2*n/(r-l);      tm[4]= 	0.0;            tm[8]=  (r+l)/(r-l);        tm[12]= 	0.0;		
    tm[1]= 	0.0;            tm[5]= 	2*n/(t-b);      tm[9]=  (t+b)/(t-b);        tm[13]= 	0.0;		
    tm[2]= 	0.0;            tm[6]= 	0.0;            tm[10]= (-f-n)/(f-n);		tm[14]= 	-2*(f*n)/(f-n);		
    tm[3]= 	0.0;            tm[7]= 	0.0;            tm[11]= -1.0;               tm[15]= 	0.0;
    
    //multiplyMatrices( tm, m, res );   
	
    multiplyMatrices( m, tm, res );   
}

void viewportMatrix( float x0, float y0, float w, float h, float *m, float *res ){
    tm[0]= 	1.0;			tm[4]= 	0.0;            tm[8]=      0.0;        tm[12]= 	0;		
    tm[1]= 	0.0;            tm[5]=	w/h;			tm[9]=      0.0;        tm[13]= 	0;		
    tm[2]= 	0.0;            tm[6]= 	0.0;            tm[10]= 	0.0;		tm[14]= 	1.0;		
    tm[3]= 	0.0;            tm[7]= 	0.0;            tm[11]= 	0.0;		tm[15]= 	1.0;
    
    //multiplyMatrices( tm, m, res );   
	
    multiplyMatrices( m, tm, res );   
}
