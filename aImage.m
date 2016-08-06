//
//  aImage.m
//  myFirst
//
//  Created by LEANDRO A DE ARAUJO on 15/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "aImage.h"

@implementation aImage

+(aImage*)imageFromBundleContentsWithName:(NSString*)fileName forExtension:(NSString*)ext{
    
//    self.miImage = nil;
//    NSBundle* myBundle = [NSBundle mainBundle];
//    NSString* myPath = [myBundle pathForResource:@"c88c978ebe85" ofType:@"png"];
//    UIImage* myImage = [UIImage imageWithContentsOfFile:myPath];
//    if(myImage == nil){
//        printf("image failed\n");
//    }else{
//        self.miImage = myImage.CGImage;
//        CFDataRef myData = CGDataProviderCopyData(CGImageGetDataProvider(self.miImage));
//        printf("data size of miImage: %i bytes\n", CFDataGetLength(myData));
//    }
    NSBundle* myBundle = [NSBundle mainBundle]; 
    NSString* myPath = [myBundle pathForResource:fileName ofType:ext];
    UIImage* myImage = [UIImage imageWithContentsOfFile:myPath];
    aImage* myInstance = [[aImage alloc] initWithCGImage:myImage.CGImage];
    //myInstance = [super init];
    //[myInstance [super copy:myImage]];
    //myInstance = (aImage*) myImage;
    return myInstance;
}

-(unsigned long int)imageDataSize{
    CFDataRef myData = CGDataProviderCopyData(CGImageGetDataProvider(self.CGImage)); 
    return CFDataGetLength(myData);
}

-(void)copyImageDataIn:(UInt8*)buffer{
    CFDataRef myData = CGDataProviderCopyData(CGImageGetDataProvider(self.CGImage)); 
    printf("data size of miImage: %ld bytes\n", CFDataGetLength(myData));
    CFRange theRange = CFRangeMake(0,CFDataGetLength(myData));
    CFDataGetBytes( myData, theRange, buffer);
}
@end
