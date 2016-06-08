//
//  popMenuView.m
//  popmenu
//
//  Created by ks on 16/1/6.
//  Copyright (c) 2016年 ks. All rights reserved.
//

#import "popMenuView.h"

@interface popMenuView()
@property(weak) IBOutlet NSMenuItem* popMenu;
@end

@implementation popMenuView


- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}

- (void)rightMouseDown:(NSEvent *)theEvent {
    NSPoint location = [self convertPoint:[theEvent locationInWindow] fromView:nil];
    [[self menu] popUpMenuPositioningItem:nil atLocation:location inView:self];
}
@end
