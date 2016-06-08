//
//  CircularIndicator.m
//  ActivityIndicator
//
//  Created by ks on 16/5/24.
//  Copyright (c) 2016年 ks. All rights reserved.
//

#import "CircularIndicator.h"

#define ALPHA_WHEN_STOPPED  0.15
#define FADE_MULTIPLIER     0.85

@interface CircularIndicator()
{
    int position;
    NSMutableArray* finColors;
    
    BOOL isFadingOut;
    NSTimer* animationTimer;
    
    NSColor* foreColor;
    NSColor* backColor;
}
@end

@implementation CircularIndicator

- (void)awakeFromNib
{
    NSRect rect = [self frame];
    [self initWithFrame:rect];
}

- (id)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    if(self)
    {
        position = 0;
        int numFins = 12;
        finColors = [[NSMutableArray alloc] initWithCapacity:numFins];
        
        _isAnimating = NO;
        isFadingOut = NO;
        
        foreColor = [NSColor blackColor];
        backColor = [NSColor clearColor];
        
        for(int i = 0; i < numFins; ++i)
        {
            [finColors addObject:foreColor];
        }
        
        [self setBoundsOrigin:NSMakePoint(0, 0)];
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    NSRect frame = self.frame;
    NSRect bframe = [self bounds];
    NSSize size = [self bounds].size;
    CGFloat theMaxSize = (size.width > size.height)?size.height:size.width;
    
    //在当前上下文设置填充和画笔颜色
    [backColor set];
    [NSBezierPath fillRect:frame];
    
    CGContextRef currentContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    [NSGraphicsContext saveGraphicsState];
    
    //坐标系转换
    CGContextTranslateCTM(currentContext,[self bounds].size.width/2,[self bounds].size.height/2);
    
    NSBezierPath* path  = [[NSBezierPath alloc] init];
    CGFloat lineWidth   = 0.0859375 * theMaxSize;
    CGFloat lineStart   = 0.234375 * theMaxSize;
    CGFloat lineEnd     = 0.421875 * theMaxSize;
    [path setLineWidth:lineWidth];
    [path setLineCapStyle:NSRoundLineCapStyle];
    [path moveToPoint:NSMakePoint(0, lineStart)];
    [path lineToPoint:NSMakePoint(0, lineEnd)];
    
    for(int i=0; i<finColors.count; ++i)
    {
        if(_isAnimating)
        {
            [(NSColor*)finColors[i]set];
        }else
        {
            [[foreColor colorWithAlphaComponent:ALPHA_WHEN_STOPPED] set];
        }
        [path stroke];
        
        //在上下文中旋转坐标系
        CGContextRotateCTM(currentContext, 6.282185/finColors.count);
    }
    
    path = nil;
    [NSGraphicsContext restoreGraphicsState];
}

-(void)setColor:(NSColor *)color
{
    if(foreColor != color)
    {
        foreColor = nil;
        foreColor = color;
        for(int i = 0; i < finColors.count; ++i)
        {
            CGFloat alpha = [finColors[i] alphaComponent];
            [finColors setObject:[foreColor colorWithAlphaComponent:alpha] atIndexedSubscript:i];
        }
        
        //设置一个bool值，指示显示前是否需要重绘
        [self setNeedsDisplay:YES];
    }
}

-(void)setBackgroundColor:(NSColor *)color
{
    if(backColor != color)
    {
        backColor = nil;
        backColor = color;
        [self setNeedsDisplay:YES];
    }
}


- (void)updateFrame:(NSTimer*)timer
{
    if(position > 0)
        -- position;
    else
        position = (int)finColors.count - 1;
    
    CGFloat minAlpha = ALPHA_WHEN_STOPPED;
    for(int i = 0; i < finColors.count; ++i)
    {
        CGFloat newAlpha = [finColors[i] alphaComponent] * FADE_MULTIPLIER;
        if(newAlpha < minAlpha)
            newAlpha = minAlpha;
        finColors[i] = [foreColor colorWithAlphaComponent:newAlpha];
    }
    
    if(isFadingOut)
    {
        BOOL done = YES;
        for(int i = 0; i < finColors.count; ++i)
        {
            if(fabs([finColors[i] alphaComponent] - minAlpha) > 0.01)
            {
                done = NO;
                break;
            }
        }
        if(done)
        {
            [self actuallyStopAnimation];
        }
    }else
    {
        finColors[position] = foreColor;
    }
    
    [self setNeedsDisplay:YES];
}

- (void)actuallyStartAnimation
{
    [self actuallyStopAnimation];
    
    _isAnimating = YES;
    isFadingOut  = NO;
    
    position = 1;
    
    animationTimer = [NSTimer timerWithTimeInterval:(NSTimeInterval)0.05 target:self selector:@selector(updateFrame:) userInfo:nil repeats:YES];
    
    [[NSRunLoop currentRunLoop] addTimer:animationTimer forMode:NSRunLoopCommonModes];
    [[NSRunLoop currentRunLoop] addTimer:animationTimer forMode:NSDefaultRunLoopMode];
    [[NSRunLoop currentRunLoop] addTimer:animationTimer forMode:NSEventTrackingRunLoopMode];
}

- (void)actuallyStopAnimation
{
    _isAnimating = NO;
    isFadingOut  = NO;
    
    if(animationTimer)
    {
        [animationTimer invalidate];
        animationTimer = nil;
    }
}

- (void)startAnimation:(id)sender
{
    if(_isAnimating && !isFadingOut)
        return;
    
    [self actuallyStartAnimation];
}

- (void)stopAnimation:(id)sender
{
    isFadingOut = YES;
}

@end















