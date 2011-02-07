#ifndef _TEST_APP
#define _TEST_APP
#include "ofxKinect.h"
#include "ofxPlanarKinect.h"
#include "ofMain.h"
#include "ofxBlobTracker.h"
#include "ofxTuioServer.h"

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
	
	void blobEntered(ofVec3f pos, int blobId);
	void blobMoved(ofVec3f pos, int blobId);
	void blobExited(ofVec3f pos, int blobId);
	
	ofxKinect			kinect;
	ofxPlanarKinect		planarKinect; 
	ofxBlobTracker		blobTracker;
	ofxTuioServer		tuioServer;
	bool clearFbo;
	ofFbo fbo;
	map<int,TuioCursor*> blobs;
	vector<pair<ofVec3f,ofVec3f> > lines;
};

#endif	
