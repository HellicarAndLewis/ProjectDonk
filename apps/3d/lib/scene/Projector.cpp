/*
 *  Projector.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */

#include "Projector.h"
#include "constants.h"

Projector::Projector(string name, float x, float y, float width, float height): Viewport(name, x, y, width, height) {
	fov = 45;
	zNear = (0.01);
	zFar = (1000);
	this->name = name;
	enabled = true;
	isActive = false;
	rotateThenPosition = false;
}

void Projector::customDraw() {
//	glScalef(0.1, 0.1, 0.1);
	glColor3f(1, 0, 0);
	ofBox(0, 0, 0, 0.2);
	glBegin(GL_LINES);
	
	
	float h = 2;
	float w = h*width/height;
	
	w *= 0.4;
	h *= 0.4;
	
	float z = -1;
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

void Projector::begin() {
	
	ofSetCoordHandedness(OF_RIGHT_HANDED);
	
	// autocalculate near/far clip planes if not set by user
	float nc = zNear, fc = zFar;
	if(zNear == 0 || zFar == 0) {
		float dist = height * 0.5f / tanf(PI * fov / 360.0f);
		nc = (zNear == 0) ? dist / 100.0f : zNear;
		fc = (zFar == 0) ? dist * 10.0f : zFar;
	}
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(fov, width/height, nc, fc);
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(ofMatrix4x4::getInverseOf(getGlobalTransformMatrix()).getPtr());
	glViewport(x, ofGetHeight() - (height+y), width, height);
	
}

void Projector::end() {
	ofPopView();
	isActive = false;
}