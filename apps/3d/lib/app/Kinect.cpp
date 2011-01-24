/*
 *  Kinect.cpp
 *  KinectExample
 *
 *  Created by Marek Bereza on 24/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */


#include "Kinect.h"


Kinect::Kinect(string name): ofNode() {
	numKinectCoords = 0;
	enabled = true;
	this->name = name;
	running = false;
	kinectCoords = NULL;
	kinectColours = NULL;
	nearClip = 0;
	farClip = 255;
	calibrationImage = NULL;
	flipX = false;
	flipY = false;
}


void Kinect::addListener(BlobListener *listener) {
	blobTracker.addListener(listener);
}



bool Kinect::open() {

	bool opened = kinect.init();
	if(!opened) {
		ofLog(OF_LOG_ERROR, "Couldn't init() %s\n", name.c_str());
	}
	running = kinect.open();
	if(running) {
		int numPx = kinect.getWidth()*kinect.getHeight();
		calibrationImage = new unsigned char[numPx];
		thresholded.allocate(kinect.getWidth(), kinect.getHeight());
		numKinectCoords = kinect.getWidth()*kinect.getHeight()/(KINECT_RESOLUTION*KINECT_RESOLUTION);
		kinectCoords = new ofVec3f[numKinectCoords];
		kinectColours = new ofVec3f[numKinectCoords];

	}
	return running;
}

void Kinect::close() {
	if(running) {
		kinect.close();
		running = false;
		delete [] kinectCoords;
		delete [] kinectColours;
		kinectCoords = NULL;
		kinectColours = NULL;
	}
}

void Kinect::update() {
	
	// do the ofNode stuff
	applyTransformation();
	
	
	// actual processing
	if(running) {
		kinect.update();
		
		hasCreatedVertexArrays = false;
		


		// get depth pixels
		int numPx = kinect.getWidth()*kinect.getHeight();
		memcpy(calibrationImage, kinect.getDepthPixels(), numPx);
		
		
		unsigned char minValue = nearClip;
		unsigned char maxValue = farClip;
		if(nearClip>farClip) {
			minValue = farClip;
			maxValue = nearClip;
		} else if(minValue==maxValue) {
			minValue = maxValue - 1;
		}
		for(int i = 0; i < numPx; i++) {
			// clamp and normalize
			if(calibrationImage[i]>minValue && calibrationImage[i]<maxValue) {
				calibrationImage[i] = 255;
			} else {
				calibrationImage[i] = 0;
			}
		}
		thresholded.setFromPixels(calibrationImage, kinect.getWidth(), kinect.getHeight());
		if(flipX || flipY) thresholded.mirror(flipY, flipX);
		
		contourFinder.findContours( thresholded,
					 100, getWidth()*getHeight()/4,
								   5, false);
		// this is when the notifictations will fire.
		blobTracker.update(contourFinder);
		
		
	}
}

void Kinect::createVertexArrays() {
	float *depths = kinect.getDistancePixels();
	unsigned char *colours = kinect.getCalibratedRGBPixels();
	
	int w = kinect.getWidth();
	// copy the pixels into the vertex arrays
	for(int i = 0; i < w; i+=KINECT_RESOLUTION) {
		for(int j = 0; j < kinect.getHeight(); j+=KINECT_RESOLUTION) {
			int index = w*j + i;
			int resIndex = w*j/(KINECT_RESOLUTION*KINECT_RESOLUTION) + i/KINECT_RESOLUTION;
			kinectCoords[resIndex] = ofVec3f(i*0.02, j*0.02, depths[index]*0.02);
			kinectColours[resIndex] = ofVec3f((float)colours[index*3+0]/255.f, (float)colours[index*3+1]/255.f, (float)colours[index*3+2]/255.f);
		}
	}
}

void Kinect::customDraw() {
	
	// this will only generate vertex arrays once per frame!
	if(!hasCreatedVertexArrays) {
		createVertexArrays();
		hasCreatedVertexArrays = true;
	}
	ofSetHexColor(0x00FF00);
	ofBox(0, 0, 0, 0.2);
	
	if(kinectCoords==NULL) return;
	
	
	glColorPointer(3, GL_FLOAT, 0, kinectColours);
	glVertexPointer(3, GL_FLOAT, 0, kinectCoords);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_POINTS, 0, numKinectCoords);
	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableClientState(GL_COLOR_ARRAY);
}


float Kinect::getWidth() {
	return kinect.getWidth();
}

float Kinect::getHeight() {
	return kinect.getHeight();
}
void Kinect::drawCalibration(ofRectangle rect) {
	if(calibrationImage!=NULL) {
		kinect.draw(rect.x, rect.y, rect.width/2, rect.height/2);
		thresholded.draw(rect.x+rect.width/2, rect.y, rect.width/2, rect.height/2);
		contourFinder.draw(rect.x+rect.width/2, rect.y, rect.width/2, rect.height/2);
//		calibrationTexture.loadData(calibrationImage, kinect.getWidth(), kinect.getHeight(), GL_LUMINANCE);
	
//		calibrationTexture.draw(rect);
	}
}