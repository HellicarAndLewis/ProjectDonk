#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	tuioClient.connect(3333);
	ofAddListener(ofEvents.touchDown, this, &testApp::touchDown);
	ofAddListener(ofEvents.touchUp, this, &testApp::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &testApp::touchMoved);

}


void testApp::touchDown(ofTouchEventArgs &touch) {

}

void testApp::touchMoved(ofTouchEventArgs &touch) {
	// e.g.
	// touches are normalized
	ofVec2f pos(touch.x, touch.y);
	
	// now scaled for screen coords
	pos *= ofGetWindowSize();
	printf("pos: %f %f\n", pos.x, pos.y);
}

void testApp::touchUp(ofTouchEventArgs &touch) {

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

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
// MOUSE EVENTS
//
// They simulate touch events - use the left/right mouse buttons
// to simulate touches with different id's
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	ofTouchEventArgs t;
	t.x = x;
	t.y = y;
	t.id = button;
	touchMoved(t);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	ofTouchEventArgs t;
	t.x = x;
	t.y = y;
	t.id = button;
	touchDown(t);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	ofTouchEventArgs t;
	t.x = x;
	t.y = y;
	t.id = button;
	touchUp(t);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

