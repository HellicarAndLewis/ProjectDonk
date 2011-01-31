#include "testApp.h"

bool mouseIsDown = false;


//--------------------------------------------------------------
void testApp::setup(){
	// make this class listen to the tracker.
	tracker.addListener(this);
	ofSetFrameRate(30);
}




//--------------------------------------------------------------
void testApp::update(){
	vector<ofVec3f> points;
	if(mouseIsDown) points.push_back(ofVec3f((float)mouseX/(float)ofGetWidth(), (float)mouseY/(float)ofGetHeight(), 0));
	blobs.clear();
	
	// every time this is called, the tracker will call
	// blobXXX on this object depending on what it finds.
	// then we store the result in our local blob array
	// for drawing.
	tracker.track(points);
}

//--------------------------------------------------------------
void testApp::draw() {
	ofNoFill();
	ofSetColor(0, 0, 0);
	map<int, ofVec2f>::iterator it;
	for(it = blobs.begin(); it != blobs.end(); it++) {
		ofCircle((*it).second.x*ofGetWidth(), (*it).second.y*ofGetHeight(), 20);
		ofDrawBitmapString(ofToString((*it).first), (*it).second*ofVec3f(ofGetWidth(), ofGetHeight()));
	}
}

void testApp::blobEntered(float x, float y, int blobId) {
	printf("blob %d entered (%f, %f)\n", blobId, x, y);
	blobs[blobId] = ofVec2f(x, y);
}
void testApp::blobMoved(float x, float y, int blobId) {
	printf("blob %d moved (%f, %f)\n", blobId, x, y);
	blobs[blobId] = ofVec2f(x, y);
}
void testApp::blobExited(float x, float y, int blobId) {
	printf("blob %d exited (%f, %f)\n", blobId, x, y);
	if(blobs.find(blobId)!=blobs.end()) {
		blobs.erase(blobId);
	}
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	mouseIsDown = true;
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	mouseIsDown = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

