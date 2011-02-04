#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	
	kinect.init();
	kinect.open();
	planarKinect.set(20, 20, 320, 240);
	planarKinect.setup();
	ofSetFrameRate(30);
	ofBackground(0,0,0);
	
}


//--------------------------------------------------------------
void testApp::update(){
	
	
	kinect.update();
	


	planarKinect.update(kinect.getDepthPixels());
	

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
	planarKinect.draw();

	ofEnableAlphaBlending();
	ofSetColor(0, 0, 255, 140);
	for(int i = 0; i < planarKinect.blobs.size(); i++) {
		
		ofCircle(planarKinect.blobs[i].x*ofGetWidth(),
				 planarKinect.blobs[i].y*ofGetHeight(),
				 50
				 );
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	planarKinect.keyPressed(key);
	
	
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
