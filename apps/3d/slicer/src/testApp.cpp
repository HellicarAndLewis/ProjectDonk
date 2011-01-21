#include "testApp.h"
#include "Plane.h"
#include "ofxXmlGui.h"

Plane plane;
ofxXmlGui gui;
//--------------------------------------------------------------
void testApp::setup(){
	gui.setup();
	//gui.addPanner("A", plane.A, -1, 1);
	//gui.addPanner("B", plane.B, -1, 1);
	//gui.addPanner("C", plane.C, -1, 1);
	//gui.addPanner("D", plane.D, -1, 1);
	gui.enable();
	
	ofBackground(0,0,0);
	//ofDisableSetupScreen();
	//plane.setFrom3Points(ofVec3f(-1, 1, 1), );
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){

}

void drawQuad(ofPoint *p) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &p[0]);
	glDrawArrays(GL_QUADS, 0, 4);
}
//--------------------------------------------------------------
void testApp::draw(){
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glScalef(ofGetWidth(), ofGetWidth(), ofGetWidth());
	glTranslatef(0.5, 0.5, 0);
	glScalef(0.2, 0.2, 0.2);
	glRotatef(mouseX, 0, 1, 0);
	glRotatef(mouseY, 1, 0, 0);
	
	
	ofPoint face[4];
	face[0] = ofPoint(-1, -1, 0);
	face[1] = ofPoint(-1, 1, 0);
	face[2] = ofPoint(1, 1, 1);
	face[3] = ofPoint(1, -1, 1);
	
	// draw the face
	glColor4f(1, 0, 0, 0.5);
	drawQuad(face);
	
	plane.setFrom3Points(face[0], face[1], face[2]);
	
	plane.draw();

	glPointSize(4);
	
	glBegin(GL_POINTS);
	for(int i = 0; i < 1000; i++) {
		ofVec3f pos = ofVec3f(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10));
	float dist = plane.distanceToPoint(pos);	
	
	if(dist>0) {
		glColor3f(0, 1, 1);
	} else {
		glColor3f(1, 0, 1);
	}
	
	glVertex3f(pos.x, pos.y, pos.z);
	}
	glEnd();
	
	
	 glPopMatrix();
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

