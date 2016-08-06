//
//  MyView.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 19/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/CAEAGLLayer.h>
#import <OpenGLES/EAGL.h>

@interface MyView : UIView {
    NSString *message;
}

@property( strong, nonatomic ) NSString *message;
@property( strong, nonatomic ) EAGLContext *myContext;

- (void) drawSceneCallback: (CADisplayLink*) sender;
@end
