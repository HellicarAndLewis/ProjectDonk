/*
 *  OrthoViewport.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */

#include "OrthoViewport.h"


OrthoViewport::OrthoViewport(float x, float y, float width, float height, ViewportType type): Viewport("", x, y, width, height) {
	this->type = type;
	this->name = getName();
	rotation = ofPoint(45, 45);
	scale = 10.f;

	
	
	switch(type) {
		case Viewport_FRONT: break;
		case Viewport_BACK:
			rotate(180, 0, 1, 0);
			break;
		case Viewport_TOP: 
			rotate(90, 1, 0, 0);
			break;
		case Viewport_BOTTOM: 
			rotate(-90, 1, 0, 0);
			break;
		case Viewport_LEFT:
			rotate(90, 0, 1, 0);
			break;
		case Viewport_RIGHT:
			rotate(-90, 0, 1, 0);
			break;
		case Viewport_ROTATABLE:
			rotate(rotation.x, 1, 0, 0);
			rotate(rotation.y, 0, 1, 0);
			break;
	}
	
	setPosition(0, 0, 10);//width/2, height/2, 10);
	//setScale(0.1, 0.1, 0.1);
}



void OrthoViewport::mouse(ofPoint delta, int button) {
	if(type==Viewport_ROTATABLE) {
		if(button==0) {
			rotation.x += delta.y;
			rotation.y += delta.x;
			
		} else {
			scale += delta.y/scale;
			// limit the scale so as not to reverse
			//if(scale<0.001) scale = 0.001;
		}
		this->name = getName();
	} else {
		if(button==0) {
			float aspect = width/height;
			translation.x += delta.x/scale/2;
			translation.y -= delta.y*aspect/scale/2;

		} else {
			
			scale += delta.y/scale;
			// limit the scale so as not to reverse
			//if(scale<0.001) scale = 0.001;
		}
	}
}
string OrthoViewport::getName() {
	switch (type) {
			
		case Viewport_FRONT:  return "Front";
		case Viewport_BACK:   return "Back";
		case Viewport_TOP:    return "Top";
		case Viewport_BOTTOM: return "Bottom";
		case Viewport_LEFT:   return "Left";
		case Viewport_RIGHT:  return "Right";
		case Viewport_ROTATABLE: return "angle: "+ofToString(rotation.x, 1)+" / "+ofToString(rotation.y, 1);// + " zoom: "+ofToString(scale);
	}
	return "no name";
}
void OrthoViewport::begin() {
	glMatrixMode (GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity ();							// Reset The Projection Matrix
	float aspect = width/height;
	float size = scale;
	glOrtho(-size/2, size/2, -size/(2*aspect), size/(2*aspect), -10000, 10000);
	
	//glScalef(scale, scale, scale);
	glTranslatef(translation.x, translation.y, 0);
	switch(type) {
		case Viewport_FRONT: break;
		case Viewport_BACK:
			glRotatef(180, 0, 1, 0);
			break;
		case Viewport_TOP: 
			glRotatef(90, 1, 0, 0);
			break;
		case Viewport_BOTTOM: 
			glRotatef(-90, 1, 0, 0);
			break;
		case Viewport_LEFT:
			glRotatef(90, 0, 1, 0);
			break;
		case Viewport_RIGHT:
			glRotatef(-90, 0, 1, 0);
			break;
		case Viewport_ROTATABLE:
			glRotatef(rotation.x, 1, 0, 0);
			glRotatef(rotation.y, 0, 1, 0);
			break;
	}
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glLoadMatrixf(ofMatrix4x4::getInverseOf(getGlobalTransformMatrix()).getPtr());
	glViewport(x, ofGetHeight() - (height+y), width, height);
	
}
void OrthoViewport::end() {
	
}
