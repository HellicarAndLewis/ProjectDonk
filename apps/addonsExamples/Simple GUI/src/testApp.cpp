#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {	
	
	ofSetVerticalSync(true);
	ofBackground(20, 20, 20);
	
	r				= 0;
	g				= 0;
	b				= 0;
	
	doTail			= false;
	maxPoints		= 40;
	tailPoint		= new ofPoint[maxPoints];
	
	doRanAmp		= false;
	radius			= 3;
	theta			= 0;
	amplitude		= 31.0;
	pos.set(0, 0);
	
	
	
	GUI.setTitle("The Simpilist GUI Ever");
	
	GUI.addToggle("Random Amplitude", &doRanAmp, 0);
	GUI.addToggle("Draw Tail", &doTail, 1);
	
	GUI.addSlider("Radius", &radius, 0.0, 100.0, 2);
	GUI.addSlider("Amplitude", &amplitude, 1.0, 200.0, 3);
	
	GUI.addSlider("Red", &r, 0, 255, 4);
	GUI.addSlider("Green", &g, 0, 255, 5);
	GUI.addSlider("Blue", &b, 0, 255, 6);

	GUI.setFromXML("gui.xml");
	//set from xml
}

//--------------------------------------------------------------
void testApp::update() {
	
	//update cirle
	theta += .1;
	pos.x = cos(theta) * amplitude;
	pos.y = sin(theta) * amplitude;
	
	//random radius
	if(doRanAmp) {
		amplitude = ofRandom(10, 120); 
	}
	//update the tail points
	if(doTail) {
		for(int i=maxPoints-1; i>0; i--) {
			tailPoint[i] = tailPoint[i-1];
		}
		tailPoint[0] = pos;
	}
}

//--------------------------------------------------------------
void testApp::draw() {
	
	
	//Circle
	glPushMatrix();
	glTranslatef(400, 200, 0);
	
	ofSetColor(r, g, b);
	ofFill();
	ofCircle(pos.x, pos.y, radius);
	
	//draw tail
	if(doTail) {
		ofNoFill();
		ofBeginShape();	
		for(int i=0; i<maxPoints; i++) {
			ofVertex(tailPoint[i].x, tailPoint[i].y);
		}
		ofEndShape();
	}
	glPopMatrix();
	
	ofDrawFPS();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

	if(key == 's') GUI.saveToXML("gui.xml");
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) { 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased() {
	
}
