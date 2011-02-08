/**
 * ofxPlanarKinectGui.cpp
 * KinectTracker
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */


#include "ofxPlanarKinect.h"

void ofxPlanarKinect::draw() {
	draw(x, y, width, height);
}

void ofxPlanarKinect::draw(float x,float y) {
	draw(x, y, getWidth(), getHeight());
}



// these return the dims of the actual drawable, not the kinect itself
float ofxPlanarKinect::getHeight() {
	return height;
}
float ofxPlanarKinect::getWidth() {
	return width;
}

void ofxPlanarKinect::draw(float x,float y,float w, float h) {
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	
	ofSetHexColor(0xFFFFFF);
	if(guiMode==SLICE_SELECTION) {
		camImg.draw(x,y,width,height);
		
		ofSetHexColor(0xFF0000);
		ofNoFill();
		ofRect(x, y + (sliceY-1)*height/kinectHeight, width, 3);
		ofFill();
	} else if(guiMode==THRESHOLD_SETTING) {
		ofSetHexColor(0xFFFFFF);
		sliceImg.draw(x,y,width,height);
		ofSetHexColor(0xFF0000);
		ofDrawBitmapString("Click mouse to choose a slice,\nright click to set threshold", x+3,y+14);
		glBegin(GL_LINE_STRIP);
		for(int i = 0 ; i < numDepthGraphPoints; i++)
			glVertex2f(depthGraph[i].x, depthGraph[i].y);
		glEnd();
		ofSetHexColor(0x00FF00);
		float t = ofMap(threshold, 0, 255, y, y+height);
		ofLine(x, t, x+width, t);
		for(int i = 0; i < rawBlobs.size(); i++) {
			ofCircle(
					 x + ofMap(rawBlobs[i].x, 0, kinectWidth, 0, width),
					 y + ofMap(rawBlobs[i].y, 0, 255, 0, height), 7);
		}
	}
}


void ofxPlanarKinect::mousePressed(float x, float y, int button) {
	mouseIsDown = inside(x,y);
	if(mouseIsDown) {
		if(guiMode==SLICE_SELECTION) {
			sliceY = (y - this->y)*kinectHeight/height;
		} else {
			threshold = (y - this->y)*255.f/height;
		}
	}
}

void ofxPlanarKinect::mouseReleased(float x, float y, int button) {
	mouseIsDown = false;
	saveSettings();
}

void ofxPlanarKinect::mouseDragged(float x, float y, int button) {
	mousePressed(x, y, button);
}
