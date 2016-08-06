//
//  MyView.m
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 19/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#import "MyView.h"
#include "cppTest.h"
#include "initScene.h"
#include "vertexArrays.h"

@implementation MyView

@synthesize message;
@synthesize myContext;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
//        self.contentScaleFactor = [[UIScreen mainScreen] scale];        
        //self.contentScaleFactor = 1.0;   
        self.layer.contentsScale = 2.0;
        myContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        printMyClassName([myContext class]);
        if(myContext != nil){
            fprintf(stderr, "it actually worked!\n");
            if([EAGLContext setCurrentContext: myContext]){
                fprintf(stderr, "context set as current\n");
                GLuint framebuffer;
                glGenFramebuffers(1, &framebuffer);
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                GLuint colorRenderBuffer;
                glGenRenderbuffers( 1, &colorRenderBuffer);
                glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
                if([myContext renderbufferStorage: GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer] ){
                    fprintf(stderr, "renderbufferStorage set succesfully\n");
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
                    
                    GLint backingWidth;
                    GLint backingHeight;
                    
                    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
                    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
                    
                    GLuint depthRenderBuffer;
                    glGenRenderbuffers( 1, &depthRenderBuffer );                    
                    glBindRenderbuffer( GL_RENDERBUFFER, depthRenderBuffer );
                    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight);
                    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer );
                    
                    
                    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
                    
                    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER) ;
                    if(status != GL_FRAMEBUFFER_COMPLETE) {
                        printf("failed to make complete framebuffer object %i", status);
                    }
                    
                    printf("renderbuffer width:%i, height:%i\n", backingWidth, backingHeight );
                    GLenum myError =  glGetError();
                    if( myError != GL_NO_ERROR ){
                        fprintf(stderr, "glError: %i\n", myError );
                    } else {
                        fprintf(stderr, "it's ok\n");
                        initGLES(0, 0, backingWidth, backingHeight);
//                        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
                        [myContext presentRenderbuffer:GL_RENDERBUFFER];
                    }
                }
            }
        }
    }
    return self;
}

-(void)drawSceneCallback: (CADisplayLink*) sender{
    drawVertexArray();
    [myContext presentRenderbuffer:GL_RENDERBUFFER];
}

-(void)touchesEnded:(NSSet *) touches withEvent:(UIEvent *)event{
    
    reorient();
    if([(UITouch*)[touches anyObject] tapCount] == 2){
        //handle a double-tap
        message = @"SHIT!!!";

//        [self setNeedsDisplay];
        printMyClassName([myContext class]);

    }
}

//-(void) drawRect:(CGRect)rect{
//    [message drawAtPoint:CGPointMake( 60, 180)
//                withFont:[UIFont systemFontOfSize:32]];
//}

-(void) dealloc{
    self.message = nil;
    [super dealloc];
}

+(Class)layerClass{
    return [CAEAGLLayer class];
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
