/*
 *  testApp.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */


#include "testApp.h"
ofFbo test;


void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60.f);
	
	ofBackground(0, 0, 0);
	img.loadImage("of.jpg");
	 
	//glEnable(GL_LIGHT0);
 
	ofSoundStreamSetup(0, 1, this, 44100, 256, 1);
	
	bubbles.setup(1000, 1000);
	test.setup(1280, 720);

}

void testApp::update() {
	bubbles.update();
	if(volume>0.7) {
		bubbles.spawn(ofVec2f(ofGetWidth()/2, ofGetHeight()));
	}
}

void testApp::render() {
	glDisable(GL_DEPTH_TEST);
	bubbles.render();
}

void testApp::drawView() {
	bubbles.drawOnModel(scene->getModel());
}


void testApp::drawFace(vector<ofPoint> &points) {
	
	ProjectedRect *tex = &bubbles;
	tex->bind();
	if(points.size()==4) {
		glBegin(GL_QUADS);
	} else {
		glBegin(GL_TRIANGLE_FAN);
	}
	for(int i = 0; i < points.size(); i++) {
		if(i==0) glTexCoord2f(0, 0);
		else if(i==1) glTexCoord2f(tex->getWidth(), 0);
		else if(i==2) glTexCoord2f(tex->getWidth(), tex->getHeight());
		else glTexCoord2f(0, tex->getHeight());
		
		glVertex3f(points[i].x, points[i].y, points[i].z);
	}
	glEnd();
	
	tex->unbind();
}

void testApp::audioReceived(float *input, int bufferSize, int numChannels) {
	float max = volume;
	for(int i = 0; i < bufferSize*numChannels; i++) {
		if(ABS(input[i])>max) max = ABS(input[i]);
		else max *= 0.99995;
	}
	volume = max;
}

void testApp::keyPressed  (int key) {
}
void testApp::keyReleased(int key) {
}
void testApp::mouseMoved(int x, int y ) {
}
void testApp::mouseDragged(int x, int y, int button) {

}
void testApp::mousePressed(int x, int y, int button) {
	

}
void testApp::mouseReleased(int x, int y, int button) {

}
void testApp::windowResized(int w, int h) {}
