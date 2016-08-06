//
//  myFirst_AppDelegate.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 19/09/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MyView;

@interface myFirst_AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) MyView    *view;
@property (strong, nonatomic) CADisplayLink *myLink;

@end
