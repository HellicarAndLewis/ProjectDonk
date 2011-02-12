/*
 *  Kinect.cpp
 *  KinectExample
 *
 *  Created by Marek Bereza on 24/01/2011.
 *
 */


#include "Kinect.h"


Kinect::Kinect(string name): ofNode() {
	enabled = true;
	this->name = name;
}




bool Kinect::open() {

	return true;
}

void Kinect::close() {
}

void Kinect::update() {
}

void Kinect::customDraw() {
	
	
	ofSetHexColor(0x00FF00);

	ofBox(0, 0, 0, 0.2);
	glBegin(GL_LINES);
	
	
	float h = 2;
	float w = h*640.f/480.f;
	
	w *= 0.4;
	h *= 0.4;
	
	float z = 1;
	glVertex3f(0, 0, 0);
	glVertex3f(-w, -h, z);
	
	glVertex3f(-w, -h, z);
	glVertex3f(-w, h, z);
	
	glVertex3f(-w, h, z);
	glVertex3f(0, 0, 0);
	
	
	
	glVertex3f(0, 0, 0);
	glVertex3f(w, h, z);
	
	glVertex3f(w, h, z);
	glVertex3f(w, -h, z);
	
	glVertex3f(w, -h, z);
	glVertex3f(0, 0, 0);
	
	glVertex3f(w, -h, z);
	glVertex3f(-w, -h, z);
	
	glVertex3f(-w, h, z);
	glVertex3f(w, h, z);
	
	
	
	glEnd();
}
