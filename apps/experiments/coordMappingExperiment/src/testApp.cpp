#include "testApp.h"
#include "ofxCoordMapper.h"
ofxCoordMapper coordMapper;
//--------------------------------------------------------------
void testApp::setup(){
	coordMapper.setup();
	glEnable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	
	
	
	coordMapper.beginCapture();
	glPushMatrix();
	glTranslatef(10, 10, 0);
	glRotatef(45, 1, 1, 0);
	ofBox(100, 100, 0, 100);
	glPopMatrix();
	coordMapper.endCapture();
	
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

	ofVec2f point((float)x/640.f, (float)y/480.f);
	point = coordMapper.lookUp(point);
	ofLogNotice() << point << endl;
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

