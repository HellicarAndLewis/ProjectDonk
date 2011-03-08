#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofxSettings.h"

//========================================================================
int main( ){
	
	ofSetDataPathRoot("../../../data/");
	settings.load("KinectTrackerSettings.xml");
	
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 670,725, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
