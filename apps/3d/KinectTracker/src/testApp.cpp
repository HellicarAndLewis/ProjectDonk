#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	tuioHost = "localhost";
	tuioPort = 3333;
	planarKinect.set(20, 20, 320, 240);
	planarKinect.setup();
	kinect.init();
	kinect.open(planarKinect.deviceId);
	ofSetFrameRate(30);
	ofBackground(0,0,0);
	blobTracker.addListener(this);
	tuioServer.start((char*)tuioHost.c_str(), tuioPort);
	tuioServer.setVerbose(true);
}



//--------------------------------------------------------------
void testApp::update(){
	
	kinect.update();

	planarKinect.update(kinect.getDepthPixels());
	blobTracker.track(planarKinect.blobs);
	tuioServer.run();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
	planarKinect.draw();

	string msg = "";
	msg += "Sending tuio to "+tuioHost+":"+ofToString(tuioPort) + "\n";
	msg += "Press 'h' for instructions";
	ofDrawBitmapString(msg, 20, ofGetHeight()-35);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	planarKinect.keyPressed(key);
	if(key=='h') {
		system((string("open ")+ofToDataPath("instructions.gif", true)).c_str());
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
	
	// ofxTuioServer assumes you're giving screen coordinates, so multiply by ofGetWidth()/Height()
	blobs[blobId] = tuioServer.addCursor(pos.x*ofGetWidth(), pos.y*ofGetHeight());
}
void testApp::blobMoved(ofVec3f pos, int blobId) {
	tuioServer.updateCursor(blobs[blobId], pos.x*ofGetWidth(), pos.y*ofGetHeight());
}

void testApp::blobExited(ofVec3f pos, int blobId) {
	tuioServer.removeCursor(blobs[blobId]);
	blobs.erase(blobId);
}