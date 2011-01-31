#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxBlobTracker.h"

// inherit from ofxBlobListener!
class testApp : public ofBaseApp, public ofxBlobListener {

public:
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	// inherited methods
	void blobEntered(float x, float y, int blobId); 
	void blobMoved(float x, float y, int blobId);
	void blobExited(float x, float y, int blobId);
	
	// the blob tracker
	ofxBlobTracker tracker;
	
	// our local representation of the blobs
	map<int, ofVec2f> blobs;	
};

#endif
