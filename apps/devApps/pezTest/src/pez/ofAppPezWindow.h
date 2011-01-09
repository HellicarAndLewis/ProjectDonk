/*
 *  ofAppPezWindow.h
 *  graphicsExample
 *
 *  Created by theo on 09/01/2011.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofEvents.h"

class ofPoint;
class ofBaseApp;

class ofAppPezWindow : public ofAppBaseWindow {

public:

	ofAppPezWindow(){}

	void setupOpenGL(int w, int h, int screenMode);
	void initializeWindow(){}
	void runAppViaInfiniteLoop(ofBaseApp * appPtr);
	
	void hideCursor(){ }
	void showCursor(){ }
	
	void setFullscreen(bool fullScreen){ }
	void toggleFullscreen(){ }
	
	static void exitApp(){ }

	void setWindowTitle(string title){ }
	void setWindowPosition(int x, int y){ }
	void setWindowShape(int w, int h){ }

	ofPoint		getWindowPosition(){return ofPoint();}
	ofPoint		getWindowSize(){return ofPoint(screen_width, screen_height);}
	ofPoint		getScreenSize(){return ofPoint();}

	int			getWindowMode(){return 0;}

	int			getFrameNum(){return 0;}
	float		getFrameRate(){return 60;}
	double		getLastFrameTime(){return 0.1;}
	void		setFrameRate(float targetRate){}

	void		enableSetupScreen(){}
	void		disableSetupScreen(){}
	
	float screen_width, screen_height;
	 
};

