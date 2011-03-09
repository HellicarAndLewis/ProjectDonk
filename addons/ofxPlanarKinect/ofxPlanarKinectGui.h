/*
 *  ofxPlanarKinectGui.h
 *  KinectTracker
 *
 *  Created by Joel Lewis on 08/03/2011.
 *  Copyright 2011 Hellicar & Lewis. All rights reserved.
 *
 */
#pragma once

/**
 * this class is a wrapper for ofBaseDraws using triangle to make it 
 * simpler to use. It also provides overridable mouse events.
 */

class ofxPlanarKinectGuiElement: public ofRectangle, public ofBaseDraws {
public:
	virtual void draw() = 0;
	
	virtual void mousePressed(float x, float y, int button) {}
	virtual void mouseReleased(float x, float y, int button) {}
	virtual void mouseDragged(float x, float y, int button) {}
	
	ofxPlanarKinectGuiElement();
	void draw(float x, float y);
	
	void draw(float x,float y,float w, float h);
	float getWidth();
	float getHeight();
	
};

class SliceChooser: public ofxPlanarKinectGuiElement {
public:
	void draw();
	
	void mousePressed(float x, float y, int button);
	void mouseReleased(float x, float y, int button);
	void mouseDragged(float x, float y, int button);
	
	void setup(int *sliceY, ofTexture *camImg, float kinectHeight, int *interactionDepth);
	// pointer to where we take the slice from
	int *sliceY;
	int *interactionDepth;
	
	// pointer to the camera's image
	ofTexture *camImg;
	float kinectHeight;
	
};

class ThresholdControl: public ofxPlanarKinectGuiElement {
public:
	void draw();
	void mousePressed(float x, float y, int button);
	void mouseReleased(float x, float y, int button);
	void mouseDragged(float x, float y, int button);
	
	void setup(float kinectWidth, vector<ofVec2f> *blobs, ofTexture *sliceImg, ofVec2f *depthGraph, int numDepthGraphPoints, vector<ofVec2f> *rawBlobs, float *threshold) {
		this->kinectWidth = kinectWidth;
		this->blobs = blobs;
		this->sliceImg = sliceImg;
		this->depthGraph = depthGraph;
		this->rawBlobs = rawBlobs;
		this->threshold = threshold;
		this->numDepthGraphPoints = numDepthGraphPoints;
		lastMouse = ofVec2f(-1,-1);
		mouseIsDown = false;
		
	}
	int numDepthGraphPoints;
	float *threshold;
	ofVec2f *depthGraph;
	float kinectWidth;
	ofTexture *sliceImg;
	vector<ofVec2f> *blobs;
	vector<ofVec2f> *rawBlobs;
	ofVec2f lastMouse;
	bool mouseIsDown;
};