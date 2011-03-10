#ifndef _TEST_APP
#define _TEST_APP
#include "ofxKinect.h"
#include "ofxPlanarKinect.h"
#include "ofMain.h"
#include "ofxBlobTracker.h"
#include "ofxTuioServer.h"
#include "ofxXmlGui.h"

class testApp : public ofBaseApp, public ofxBlobListener, public GuiListener {
	
public:
	
	void setup();
	void update();
	
	// instead of using the normal draw() we have to use
	// a custom event draw() because we need it to draw
	// ontop of the gui.
	void _draw(ofEventArgs &args);
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	// for ofxBlobTracker
	void blobEntered(ofVec3f pos, int blobId);
	void blobMoved(ofVec3f pos, int blobId);
	void blobExited(ofVec3f pos, int blobId);
	
	// for gui
	void controlChanged(GuiControl *control);
	
	
	
	/** the actual kinect device */
	ofxKinect			kinect;
	
	/** the thing that converts the kinect info to blobs */
	ofxPlanarKinect		planarKinect; 
	
	/** the tracker for the output of planarKinect */
	ofxBlobTracker		blobTracker;
	
	/** Output to any hosts. */
	ofxTuioServer		tuioServer;
	
	
	map<int,TuioCursor*> blobs;
	ofxXmlGui gui;
	
	string tuioHost;
	int tuioPort;
	
	void setupGui();
	
	/** 
	 * This is the location of the gui element
	 * that draws the tuio points. It's used
	 * for drawing the offset
	 */
	ofRectangle tuioRect;
	float updateTime;
	bool fullscreen;
	bool isConnected;
};

#endif	
