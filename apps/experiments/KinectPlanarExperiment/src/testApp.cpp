#include "testApp.h"

string tuioHost = "localhost";
int tuioPort = 3333;
//--------------------------------------------------------------
void testApp::setup(){
	clearFbo = true;
	fbo.setup(ofGetWidth(), ofGetHeight(), GL_RGBA);


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

	ofSetHexColor(0xFFFFFF);
	string msg = "";
	msg += "Sending tuio to "+tuioHost+":"+ofToString(tuioPort) + "\n";
	msg += "Press 'h' for instructions";
	ofDrawBitmapString(msg, 20, ofGetHeight()-35);
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	planarKinect.keyPressed(key);
	if(key==' ') {
		clearFbo = true;
	} else if(key=='h') {
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
	
	/*lines.push_back(make_pair(pos, 
							  ofVec3f(blobs[blobId]->getPosition().getX(), blobs[blobId]->getPosition().getY())
							  ));*/
	
	tuioServer.updateCursor(blobs[blobId], pos.x*ofGetWidth(), pos.y*ofGetHeight());
}
void testApp::blobExited(ofVec3f pos, int blobId) {
	tuioServer.removeCursor(blobs[blobId]);
	blobs.erase(blobId);
}