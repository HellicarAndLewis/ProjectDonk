/*
 *  ofAppPezWindow.cpp
 *  graphicsExample
 *
 *  Created by theo on 09/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#import <pez.h>
//#import <glew.h>
#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>
#import <mach/mach_time.h>

#include "ofAppPezWindow.h"
#include "ofBaseApp.h"
#include "ofMain.h"

int requestedWidth  = 1024;
int requestedHeight = 768;
ofBaseApp *		ofAppPtr = NULL;
ofAppPezWindow * myWindow = NULL;
bool bEnableSetupScreen = true;
bool bFullScreen = false;
bool bFullScreenRequested = false;
int nFrameCount = 0;
int nFramesSinceWindowResized = 0;

@class View;

@interface View : NSOpenGLView <NSWindowDelegate>{
    NSRect m_frameRect;
    BOOL m_didInit;
    uint64_t m_previousTime;
    NSTimer* m_timer;
}

- (void) animate;

@end

@implementation View

-(void)windowWillClose:(NSNotification *)note {
    [[NSApplication sharedApplication] terminate:self];
}

- (void) timerFired:(NSTimer*) timer
{
    [self animate];     
}

- (id) initWithFrame: (NSRect) frame
{
    m_didInit = FALSE;
    
    int attribs[] = {
        NSOpenGLPFAAccelerated,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFAColorSize, 32,
        NSOpenGLPFANoRecovery,
#if PEZ_ENABLE_MULTISAMPLING
        kCGLPFASampleBuffers, 1,
        kCGLPFASamples, 4,
#endif
        0
    };

    NSOpenGLPixelFormat *fmt = [[NSOpenGLPixelFormat alloc]
                            initWithAttributes:(NSOpenGLPixelFormatAttribute*) attribs];

    self = [self initWithFrame:frame pixelFormat:fmt];

    [fmt release];

    m_frameRect = frame;
    m_previousTime = mach_absolute_time();

    m_timer = [[NSTimer
                       scheduledTimerWithTimeInterval:1.0f/120.0f
                       target:self 
                       selector:@selector(timerFired:)
                       userInfo:nil
                       repeats:YES] retain];

    return self;
}

- (void) drawRect:(NSRect) theRect
{
    if (!m_didInit) {
            
        int TransparentWindow = 0;
        if (TransparentWindow) {
            int opaque = NO;
            [[self openGLContext]
                setValues:&opaque
                forParameter:NSOpenGLCPSurfaceOpacity];
    
            [[self window] setOpaque:NO];
            [[self window] setAlphaValue:0.2];
        }
        
        //glewInit();
        const char* szTitle = PezInitialize(requestedWidth, requestedHeight);
        m_didInit = YES;
        
        [[self window] setLevel: NSNormalWindowLevel];
        [[self window] makeKeyAndOrderFront: self];
        [[self window] setTitle: [NSString stringWithUTF8String: szTitle]];

		[[self window] setAcceptsMouseMovedEvents:true];
		
		[self registerForDraggedTypes:[NSArray arrayWithObjects:NSTIFFPboardType, 
								   NSFilenamesPboardType, nil]];
		
    }

    PezRender();
    [[self openGLContext] flushBuffer]; 
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender
{
    if ((NSDragOperationGeneric & [sender draggingSourceOperationMask]) 
		== NSDragOperationGeneric) {
		
        return NSDragOperationGeneric;
		
    } // end if
	
    // not a drag we can use
	return NSDragOperationNone;	
	
} // end draggingEntered

- (BOOL)prepareForDragOperation:(id <NSDraggingInfo>)sender {
	printf("prepareForDragOperation! \n");
    return YES;
} // end prepareForDragOperation


- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {
	printf("performDragOperation! \n");


    NSPasteboard *zPasteboard	= [sender draggingPasteboard];
	NSArray *zFileNamesAry		= [zPasteboard propertyListForType:@"NSFilenamesPboardType"];
	
	
	int numFiles				= [zFileNamesAry count];
	vector <string> fileList;
		
	for(int k = 0; k < numFiles; k++){
		NSString *zPath = [zFileNamesAry objectAtIndex:k];
		fileList.push_back( [zPath UTF8String] );
	}
	
	
	ofPoint pt( [sender draggingLocation].x, ofGetHeight() - [sender draggingLocation].y );
	ofAppPtr->filesDragged(fileList, pt);
	
	return YES;
		 		
} // end performDragOperation


- (void)concludeDragOperation:(id <NSDraggingInfo>)sender {
    [self setNeedsDisplay:YES];
} // end concludeDragOperation


- (void) mouseMoved: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    PezHandleMouse(curPoint.x, m_frameRect.size.height - curPoint.y, PEZ_MOVE);
}

- (void) mouseDragged: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    PezHandleMouse(curPoint.x, m_frameRect.size.height - curPoint.y, PEZ_MOVE);
}

- (void) mouseUp: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    PezHandleMouse(curPoint.x, m_frameRect.size.height - curPoint.y, PEZ_UP);
}

- (void) mouseDown: (NSEvent*) theEvent
{
    NSPoint curPoint = [theEvent locationInWindow];
    PezHandleMouse(curPoint.x, m_frameRect.size.height - curPoint.y, PEZ_DOWN);
}

- (void) animate
{
    uint64_t currentTime = mach_absolute_time();
    uint64_t elapsedTime = currentTime - m_previousTime;
    m_previousTime = currentTime;
    
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);
    
    elapsedTime *= info.numer;
    elapsedTime /= info.denom;
    
    float timeStep = elapsedTime / 1000000.0f;

    PezUpdate(timeStep);
    
    [self display];
}

- (void) onKey: (unichar) character downEvent: (BOOL) flag
{
    switch(character) {
        case 27:
        case 'q':
            [[NSApplication sharedApplication] terminate:self];
            break;
    }
}

- (void) keyDown:(NSEvent *)theEvent
{
	printf("keyDown\n");

    NSString *characters;
    unsigned int characterIndex, characterCount;
    
    characters = [theEvent charactersIgnoringModifiers];
    characterCount = [characters length];

    for (characterIndex = 0; characterIndex < characterCount; characterIndex++){
		ofAppPtr->keyPressed([characters characterAtIndex:characterIndex]);
		[self onKey:[characters characterAtIndex:characterIndex] downEvent:YES];
	}
}

@end


void PezDebugStringW(const wchar_t* pStr, ...)
{
    va_list a;
    va_start(a, pStr);

    wchar_t msg[1024] = {0};
    vswprintf(msg, countof(msg), pStr, a);
    fputws(msg, stderr);
}

void PezDebugString(const char* pStr, ...)
{
    va_list a;
    va_start(a, pStr);

    char msg[1024] = {0};
    vsnprintf(msg, countof(msg), pStr, a);
    fputs(msg, stderr);
}

void PezFatalErrorW(const wchar_t* pStr, ...)
{
    fwide(stderr, 1);

    va_list a;
    va_start(a, pStr);

    wchar_t msg[1024] = {0};
    vswprintf(msg, countof(msg), pStr, a);
    fputws(msg, stderr);
    exit(1);
}

void PezFatalError(const char* pStr, ...)
{
    va_list a;
    va_start(a, pStr);

    char msg[1024] = {0};
    vsnprintf(msg, countof(msg), pStr, a);
    fputs(msg, stderr);
    exit(1);
}

//---------------------------------------------------------------
void ofAppPezWindow::setupOpenGL(int w, int h, int screenMode){
	requestedWidth	= w;
	requestedHeight = h;
	
	if( screenMode == OF_FULLSCREEN ){
		bFullScreenRequested = true;
	}
}

//---------------------------------------------------------------
void ofAppPezWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr){
	myWindow = this;
	ofAppPtr = appPtr;
	
	NSAutoreleasePool *pool = [NSAutoreleasePool new];
    NSApplication *NSApp = [NSApplication sharedApplication];
    NSRect frame = NSMakeRect( 100., 100., 300., 300. );
    
    NSRect screenBounds = [[NSScreen mainScreen] frame];
    NSRect viewBounds;
	
	viewBounds = NSMakeRect(0, 0, requestedWidth, requestedHeight);
	if( bFullScreenRequested ){
		viewBounds = screenBounds;
	}
	
	unsigned int mask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
    
    View* view = [[View alloc] initWithFrame:viewBounds];
    
	
//    NSRect centered = NSMakeRect(NSMidX(screenBounds) - NSMidX(viewBounds),
//                                 NSMidY(screenBounds) - NSMidY(viewBounds),
//                                 viewBounds.size.width, viewBounds.size.height);

    NSRect centered = NSMakeRect(40, 40, viewBounds.size.width, viewBounds.size.height);
    
	if( bFullScreenRequested ){
		centered = viewBounds;
		mask	 = NSBorderlessWindowMask;

		#ifdef TARGET_OSX
			SetSystemUIMode(kUIModeAllHidden,NULL);
		#endif
	}
	
    NSWindow *window = [[NSWindow alloc]
        initWithContentRect:centered
        styleMask:mask
        backing:NSBackingStoreBuffered
        defer:NO];

    [window setContentView:view];
    [window setDelegate:view];
	[view release];
    
    [NSApp run];
    
    [pool release];
    return;
}

//---------------------------------------------------------------
const char* PezInitialize(int width, int height){
	myWindow->screen_width = width;
	myWindow->screen_height = height;
	
	ofAppPtr->setup();
	
	nFrameCount = 0;
	
	return "openFrameworks app";
}

//---------------------------------------------------------------
void PezRender(){

int width, height;

	width  = ofGetWidth();
	height = ofGetHeight();

	height = height > 0 ? height : 1;
	// set viewport, clear the screen
	glViewport( 0, 0, width, height );
	float * bgPtr = ofBgColorPtr();
	bool bClearAuto = ofbClearBg();

    // to do non auto clear on PC for now - we do something like "single" buffering --
    // it's not that pretty but it work for the most part

    #ifdef TARGET_WIN32
    if (bClearAuto == false){
        glDrawBuffer (GL_FRONT);
    }
    #endif

	if ( bClearAuto == true || nFrameCount < 3){
		glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	if( bEnableSetupScreen )ofSetupScreen();

	ofNotifyDraw();

    #ifdef TARGET_WIN32
    if (bClearAuto == false){
        // on a PC resizing a window with this method of accumulation (essentially single buffering)
        // is BAD, so we clear on resize events.
        if (nFramesSinceWindowResized < 3){
            glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        } else {
            if (nFrameCount < 3 || nFramesSinceWindowResized < 3)    glutSwapBuffers();
            else                                                     glFlush();
        }
    } else {
        glutSwapBuffers();
    }
    #else
		if (bClearAuto == false){
			// in accum mode resizing a window is BAD, so we clear on resize events.
			if (nFramesSinceWindowResized < 3){
				glClearColor(bgPtr[0],bgPtr[1],bgPtr[2], bgPtr[3]);
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
		}
        glutSwapBuffers();
    #endif

    nFramesSinceWindowResized++;

	//fps calculation moved to idle_cb as we were having fps speedups when heavy drawing was occuring
	//wasn't reflecting on the actual app fps which was in reality slower. 
	//could be caused by some sort of deferred drawing? 

	nFrameCount++;		// increase the overall frame count

}

//---------------------------------------------------------------
void PezUpdate(unsigned int milliseconds){
	ofAppPtr->update();
}

//---------------------------------------------------------------
bool bMouseDown = false;

// handle mouse action: PEZ_DOWN, PEZ_UP, or PEZ_MOVE
void PezHandleMouse(int x, int y, int action){
	if (nFrameCount > 0){
		if(ofAppPtr){
			ofAppPtr->mouseX = x;
			ofAppPtr->mouseY = y;
		}

		if (action == PEZ_DOWN) {
			ofNotifyMousePressed(x, y, 0);
			bMouseDown = true;
		} else if (action == PEZ_UP) {
			ofNotifyMouseReleased(x, y, 0);
			bMouseDown = false;
		}
		
		if( action == PEZ_MOVE ){
			if( bMouseDown = false ){
				ofNotifyMouseMoved(x, y);
			}else{
				ofNotifyMouseDragged(x, y, 0);		
			}		
		}
		
	}

}


