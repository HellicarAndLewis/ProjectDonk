/*
 *  testApp.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */


#include "testApp.h"




void testApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60.f);
	ofBackground(0, 0, 0);
	bubbles.setup(1024,1024);
	/*for(int i = 0; i < scene->kinects.size(); i++) {
		scene->kinects[i]->addListener(this);
	}*/
}

void testApp::update() {
	bubbles.update();
}


void testApp::render() {
	bubbles.render();
}


void testApp::drawView() {

	glColor3f(1, 1, 1);
	// setting the parameter to false just draws a wireframe
	bubbles.drawOnModel(scene->getModel());
}
/*
void testApp::blobMoved(ofVec3f centroid, int blobId) {
	bubbles.spawn(ofVec2f(centroid.x*bubbles.getWidth(),centroid.y*bubbles.getHeight()));
}*/


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
