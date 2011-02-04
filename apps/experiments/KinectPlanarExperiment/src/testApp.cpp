#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	clearFbo = true;
	fbo.setup(ofGetWidth(), ofGetHeight(), GL_RGBA);
	kinect.init();
	kinect.open();
	planarKinect.set(20, 20, 320, 240);
	planarKinect.setup();
	ofSetFrameRate(30);
	ofBackground(0,0,0);
	blobTracker.addListener(this);
	
}


//--------------------------------------------------------------
void testApp::update(){
	
	
	kinect.update();
	


	planarKinect.update(kinect.getDepthPixels());
	
	blobTracker.track(planarKinect.blobs);

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
	planarKinect.draw();


	
	fbo.begin();
	if(clearFbo) {
		ofClear(0,0,0,0);
		clearFbo = false;
	}
	ofSetColor(0, 0, 255, 255);
	ofVec3f d = ofVec3f(ofGetWidth(), ofGetHeight());
	for(int i = 0; i < lines.size(); i++) {
		ofLine(lines[i].first*d, lines[i].second*d);
	}
	lines.clear();
	fbo.end();
	fbo.draw(0,0);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	planarKinect.keyPressed(key);
	if(key==' ') {
		clearFbo = true;
	}
	
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	planarKinect.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	planarKinect.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	planarKinect.mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

void testApp::blobEntered(ofVec3f pos, int blobId) {
	blobs[blobId] = pos;
}
void testApp::blobMoved(ofVec3f pos, int blobId) {
	lines.push_back(make_pair(pos, blobs[blobId]));
	blobs[blobId] = pos;
}
void testApp::blobExited(ofVec3f pos, int blobId) {
	blobs.erase(blobId);
}