#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void testApp::update(){
	champ.update();	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofClear(0, 0, 0, 255);
	champ.draw();
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
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	ofVec3f vec(x, y, ofRandom(-10));
	champ.createObjectAtPoint(vec);
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

