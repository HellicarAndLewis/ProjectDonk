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
	light.setDirectional(false);
	light.setPosition(ofVec3f(2, 4, 5));
	light.setAmbientColor(ofColor(0));
	//light.disable();
}

void testApp::update() {
	
}

void testApp::render() {
}
void testApp::drawView() {
	glColor3f(1, 1, 1);
	
	ofEnableLighting();
	light.enable();
	// setting the parameter to false just draws a wireframe
	glShadeModel(GL_FLAT);
	scene->getModel()->drawSolid();
	ofDisableLighting();
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
