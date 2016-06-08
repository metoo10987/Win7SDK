//
//  StatusIndicator.m
//  ActivityIndicator
//
//  Created by ks on 16/5/23.
//  Copyright (c) 2016年 ks. All rights reserved.
//

#import "PopupActivityIndicator.h"
#import "CircularIndicator.h"

@interface PopupActivityIndicator()
{
    NSView* parentView;
    
    NSButton* backgroundMask;
    NSView*   mainView;
    CircularIndicator* circularIndicator;
}
@end

@implementation PopupActivityIndicator

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
}

- (NSRect)getCenterWithInRect:(NSRect)rect scale:(CGFloat)scale
{
    NSRect re;
    CGFloat newWidth = rect.size.width*scale;
    CGFloat newHeight = rect.size.height*scale;
    
    re.origin.x = rect.size.width/2 - newWidth/2;
    re.origin.y = rect.size.height/2 - newHeight/2;
    re.size.width = newWidth;
    re.size.height = newHeight;
    return re;
}

- (void)addMask
{
    backgroundMask = [[NSButton alloc] init];
    [backgroundMask removeFromSuperview];
    NSRect rect = parentView.frame;
    [backgroundMask setFrame:rect];
    [backgroundMask setEnabled:YES];
    if(!backgroundMask.wantsLayer)
    {
        CALayer* layer = [CALayer layer];
        [backgroundMask setLayer:layer];
        
        CGColorRef bgColor = [NSColor blackColor].CGColor;
        [backgroundMask.layer setBackgroundColor:bgColor];
    }
    [backgroundMask.layer setOpacity:0.5];
    backgroundMask.autoresizingMask = NSViewMaxXMargin | NSViewMaxYMargin | NSViewMinXMargin | NSViewMinYMargin | NSViewWidthSizable | NSViewHeightSizable;
    [parentView addSubview:backgroundMask positioned:NSWindowAbove relativeTo:self];
}

- (void)runIndicator
{
    NSRect frame = mainView.frame;
    frame.origin.x = 0;
    frame.origin.y = 0;
    circularIndicator = [[CircularIndicator alloc] initWithFrame:frame];
    [mainView addSubview:circularIndicator];
    [circularIndicator startAnimation:nil];
}

- (void)showViewAnimated
{
    if(![parentView wantsLayer])
    {
        [parentView setWantsLayer:TRUE];
        [parentView setLayer:[CALayer layer]];
    }
    
    if(!self.superview)
    {
        [parentView addSubview:self];
    }
    [self runIndicator];
}

- (void)setMainViewBackgroundColor
{
    CGColorRef bgColor = CGColorCreateGenericRGB(0.5, 0.5, 0.5, 0.9);
    if(![mainView wantsLayer])
    {
        CALayer* bgLayer = [CALayer layer];
        [bgLayer setBackgroundColor:bgColor];
        [bgLayer setCornerRadius:15.0];
        [mainView setWantsLayer:TRUE];
        [mainView setLayer:bgLayer];
    }else
    {
        [mainView.layer setBackgroundColor:bgColor];
        [mainView.layer setCornerRadius:15.0];
    }
}

- (void)initSelfView
{
    self.autoresizingMask = NSViewMaxXMargin | NSViewMaxYMargin | NSViewMinXMargin | NSViewMinYMargin;
    NSRect rect = [parentView frame];
    [self setFrame:rect];
    if(![self layer])
    {
        CALayer* bgLayer = [CALayer layer];
        [self setLayer:bgLayer];
        [self setWantsLayer:TRUE];
    }
    
    [self.layer setFrame:rect];
}

- (void)initMainView
{
    mainView = [[NSView alloc] init];
    mainView.autoresizingMask = NSViewMaxXMargin | NSViewMaxYMargin | NSViewMinXMargin | NSViewMinYMargin;
    NSRect minViewRect = [self getCenterWithInRect:parentView.frame scale:0.2];
    [mainView setFrame:minViewRect];
    [self setMainViewBackgroundColor];
}

- (void)popupCircularIndicatorFromView:(NSView*)view
{
    parentView = view;
    
    [self initSelfView];
    [self initMainView];
    
    [self addSubview:mainView];
    
    [self addMask];
    
    [self showViewAnimated];
}


- (void)stop
{
    [circularIndicator stopAnimation:nil];
    [backgroundMask removeFromSuperview];
    [self removeFromSuperview];
}

@end


































