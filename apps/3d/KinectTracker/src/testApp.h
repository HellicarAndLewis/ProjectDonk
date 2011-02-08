#ifndef _TEST_APP
#define _TEST_APP
#include "ofxKinect.h"
#include "ofxPlanarKinect.h"
#include "ofMain.h"
#include "ofxBlobTracker.h"
#include "ofxTuioServer.h"
#include "ofxXmlGui.h"
class testApp : public ofBaseApp, public ofxBlobListener {
	
public:
	
	void setup();
	void update();
	void draw();
	
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
};

#endif	
