#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	httpLoader = new ofxAsyncHttpLoader();
	httpLoader->get("http://upload.wikimedia.org/wikipedia/en/5/5f/Van_Gogh_-_Portrait_of_Pere_Tanguy_1887-8.JPG","");
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update(){
	if(httpLoader != NULL){
		cout << "httpLoader->status = " << httpLoader->status << endl;

		if(httpLoader->status==2){
			cout << "data size: " << httpLoader->data.size() << " bytes" << endl;
			cout << "error: " << httpLoader->errorString << endl;
		}
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);
	ofSetColor(255,255,255);
	ofRotate(ofGetFrameNum(),0,0,1);
	ofRect(-100,-100,200,200);
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

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

