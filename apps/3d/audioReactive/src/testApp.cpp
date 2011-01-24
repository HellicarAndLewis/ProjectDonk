/*
 *  testApp.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */


#include "testApp.h"



void testApp::setup() {
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60.f);
	
	ofBackground(0, 0, 0);
	
	
	bubbles.setup(500, 1000);
	
	
	ofSoundStreamSetup(0, 1, this, 44100, 256, 1);
}

void testApp::update() {
	bubbles.update();
	if(volume>0.7) {
		bubbles.spawn(ofVec2f(bubbles.getWidth()/2, bubbles.getHeight()/2));
	}
}

void testApp::render() {
	glDisable(GL_DEPTH_TEST);
	bubbles.render();
}

void testApp::drawView() {
	bubbles.drawOnModel(scene->getModel());
	ofSetHexColor(0xFFFFFF);
	scene->getModel()->draw(false);
	ofFill();
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
