/**
 * ofxPlanarKinectGui.cpp
 * KinectTracker
 *
 * Created by Marek Bereza on 08/02/2011.
 *
 */


#include "ofxPlanarKinect.h"



///////////////////////////////////////////////////////////////////////////////////////////
// ofxPlanarKinectGuiElement
///////////////////////////////////////////////////////////////////////////////////////////

ofxPlanarKinectGuiElement::ofxPlanarKinectGuiElement() {
	x = y = 0;
	width = 320; height = 240;
}
void ofxPlanarKinectGuiElement::draw(float x, float y) { draw(x, y, width, height); }

void ofxPlanarKinectGuiElement::draw(float x,float y,float w, float h) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	draw();
}	
float ofxPlanarKinectGuiElement::getWidth() { return width; }
float ofxPlanarKinectGuiElement::getHeight() { return height; }






///////////////////////////////////////////////////////////////////////////////////////////
// SliceChooser
///////////////////////////////////////////////////////////////////////////////////////////




void SliceChooser::draw() {
	camImg->draw(x,y,width,height);
	
	ofNoFill();
	ofSetHexColor(0x00FF00);
	float bottom = y + ((*sliceY)+2)*height/kinectHeight;
	float top = bottom - (*interactionDepth)*height/kinectHeight;
	ofRect(x, top, width, bottom - top);
	ofSetHexColor(0xFF0000);
	
	ofRect(x, y + ((*sliceY)-1)*height/kinectHeight, width, 3);
	ofFill();
}



void SliceChooser::setup(int *sliceY, ofTexture *camImg, float kinectHeight, int *interactionDepth) {
	this->interactionDepth = interactionDepth;
	this->sliceY = sliceY;
	this->camImg = camImg;
	this->kinectHeight = kinectHeight;
}

void SliceChooser::mousePressed(float x, float y, int button) {
	if(inside(x,y)) {
		(*sliceY) = (y - this->y)*kinectHeight/height;
	}
}

void SliceChooser::mouseDragged(float x, float y, int button) {
	mousePressed(x, y, button);
}


void SliceChooser::mouseReleased(float x, float y, int button) {}









///////////////////////////////////////////////////////////////////////////////////////////
// ThresholdControl
///////////////////////////////////////////////////////////////////////////////////////////



// kinectWidth, blobs, sliceImg, depthGraph, rawBlobs, threshold

void ThresholdControl::draw() {
	ofSetHexColor(0xFFFFFF);
	sliceImg->draw(x,y,width,height);
	ofSetHexColor(0xFF0000);
	
	// correct English
	if(blobs->size()!=1) {
		ofDrawBitmapString(ofToString(blobs->size())+" blobs", x+3,y+14);
	} else {
		ofDrawBitmapString("1 blob", x+3,y+14);
	}
	glBegin(GL_LINE_STRIP);
	for(int i = 0 ; i < numDepthGraphPoints; i++) {
		//glVertex2f(depthGraph[i].x, depthGraph[i].y);
		float yy = ofMap(depthGraph[i].y, 0, 1, y, y+height);
		float xx = ofMap(i, 0, numDepthGraphPoints, x, x+width);
		glVertex2f(xx,yy);
	}
	glEnd();
	ofSetHexColor(0x00FF00);
	
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < kinectWidth; i++) {
		float yy = ofMap(threshold[i], 0, 1, y, y+height);
		float xx = ofMap(i, 0, kinectWidth, x, x+width);
		glVertex2f(xx,yy);
		
	}
	glEnd();
	
	
	for(int i = 0; i < rawBlobs->size(); i++) {
		ofCircle(
				 x + ofMap(rawBlobs->at(i).x, 0, kinectWidth, 0, width),
				 y + ofMap(rawBlobs->at(i).y, 0, 1, 0, height), 7);
	}
}


void ThresholdControl::mousePressed(float x, float y, int button) {
	mouseIsDown = inside(x,y);
	if(mouseIsDown) {
		int fromOffset = ofMap(x, this->x, this->x+this->width, 0, kinectWidth);
		float fromThreshold = (y - this->y)*1.f/height;
		if((lastMouse.x==-1 && lastMouse.y==-1) || lastMouse.x==x) {
			threshold[fromOffset] = fromThreshold;
		} else {
			int toOffset = ofMap(lastMouse.x, this->x, this->x+this->width, 0, kinectWidth);
			float toThreshold = (lastMouse.y - this->y)*1.f/height;
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

void ThresholdControl::mouseDragged(float x, float y, int button) {
	mousePressed(x, y, button);
	lastMouse = ofVec2f(x, y);
}


void ThresholdControl::mouseReleased(float x, float y, int button) {
	mouseIsDown = false;
	lastMouse = ofVec2f(-1, -1);
}













///////////////////////////////////////////////////////////////////////////////////////////
// ofxPlanarKinect
///////////////////////////////////////////////////////////////////////////////////////////




void ofxPlanarKinect::draw(float x,float y,float w, float h) {
	this->x = x;
	this->y = y;
	width = w;
	height = h;
	
	ofSetHexColor(0xFFFFFF);
	if(guiMode==SLICE_SELECTION) {
		
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
			float yy = ofMap(threshold[i], 0, 1, y, y+height);
			float xx = ofMap(i, 0, kinectWidth, x, x+width);
			glVertex2f(xx,yy);
			
		}
		glEnd();
		
		
		for(int i = 0; i < rawBlobs.size(); i++) {
			ofCircle(
					 x + ofMap(rawBlobs[i].x, 0, kinectWidth, 0, width),
					 y + ofMap(rawBlobs[i].y, 0, 1, 0, height), 7);
		}
	}
}


void ofxPlanarKinect::mousePressed(float x, float y, int button) {
	sliceChooser.mousePressed(x, y, button);
	thresholdControl.mousePressed(x, y, button);
}

void ofxPlanarKinect::mouseReleased(float x, float y, int button) {
	sliceChooser.mouseReleased(x, y, button);
	thresholdControl.mouseReleased(x, y, button);
	saveSettings();

}

void ofxPlanarKinect::mouseDragged(float x, float y, int button) {
	sliceChooser.mouseDragged(x, y, button);
	thresholdControl.mouseDragged(x, y, button);
}
