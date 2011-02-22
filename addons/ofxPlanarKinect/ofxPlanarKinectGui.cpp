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
		
		// correct English
		if(blobs.size()!=1) {
			ofDrawBitmapString(ofToString(blobs.size())+" blobs", x+3,y+14);
		} else {
			ofDrawBitmapString("1 blob", x+3,y+14);
		}
		glBegin(GL_LINE_STRIP);
		for(int i = 0 ; i < numDepthGraphPoints; i++)
			glVertex2f(depthGraph[i].x, depthGraph[i].y);
		glEnd();
		ofSetHexColor(0x00FF00);
		
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < kinectWidth; i++) {
			float yy = ofMap(threshold[i], 0, 255, y, y+height);
			float xx = ofMap(i, 0, kinectWidth, x, x+width);
			glVertex2f(xx,yy);
			
		}
		glEnd();
		
		
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
			int fromOffset = ofMap(x, this->x, this->x+this->width, 0, kinectWidth);
			float fromThreshold = (y - this->y)*255.f/height;
			if((lastMouse.x==-1 && lastMouse.y==-1) || lastMouse.x==x) {
				threshold[fromOffset] = fromThreshold;
			} else {
				int toOffset = ofMap(lastMouse.x, this->x, this->x+this->width, 0, kinectWidth);
				float toThreshold = (lastMouse.y - this->y)*255.f/height;
				// order them correctly if needed
				if(toOffset<fromOffset) {
					// swap offset
					int t = toOffset;
					toOffset = fromOffset;
					fromOffset = t;
					
					float f = toThreshold;
					toThreshold = fromThreshold;
					fromThreshold = f;
				}
				
				// helps with the left and right edges
				if(toOffset>kinectWidth-4) toOffset = kinectWidth - 1;
				if(fromOffset<3) fromOffset = 0;
				
				
				// draw a line of thresholds going from previous mouse to curr mouse
				for(int i = fromOffset; i <= toOffset; i++) {
					threshold[i] = ofMap(i, fromOffset, toOffset, fromThreshold, toThreshold);
				}
			}
			
		}
	}
	lastMouse = ofVec2f(x, y);
}

void ofxPlanarKinect::mouseReleased(float x, float y, int button) {
	mouseIsDown = false;
	saveSettings();
	lastMouse = ofVec2f(-1, -1);
}

void ofxPlanarKinect::mouseDragged(float x, float y, int button) {
	mousePressed(x, y, button);
	lastMouse = ofVec2f(x, y);
}