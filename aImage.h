//
//  aImage.h
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 15/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface aImage : UIImage

+(aImage*)imageFromBundleContentsWithName:(NSString*)fileName forExtension:(NSString*)ext;

-(unsigned long int)imageDataSize;

-(void)copyImageDataIn:(UInt8*)buffer;

@end
