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
}

bool Kinect::open() {
	bool opened = kinect.init();
	if(!opened) {
		ofLog(OF_LOG_ERROR, "Couldn't init() %s\n", name.c_str());
	}
	running = kinect.open();
	if(running) {
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
	applyTransformation();
	if(running) {
		kinect.update();
		
		// all this should be in update()!!!
		
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
}

void Kinect::customDraw() {
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

