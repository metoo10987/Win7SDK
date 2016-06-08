//
//  CircularIndicator.h
//  ActivityIndicator
//
//  Created by ks on 16/5/24.
//  Copyright (c) 2016年 ks. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface CircularIndicator : NSView

@property BOOL isAnimating;

-(void)setColor:(NSColor*)color;
-(void)setBackgroundColor:(NSColor*)color;

-(void)startAnimation:(id)sender;
-(void)stopAnimation:(id)sender;
@end
