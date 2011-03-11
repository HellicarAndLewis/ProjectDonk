/*
 *  ofxFourUpDisplay.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */

#include "ofxFourUpDisplay.h"

ofxFourUpDisplay::ofxFourUpDisplay(ofScene3d *scene, ofRectangle rect) {
	this->scene = scene;
	
	movingCentre = false;
	x = rect.x;
	y = rect.y;
	width = rect.width;
	height = rect.height;
	
	// create 4 viewports
	viewports.push_back(new OrthoViewport(x, y, width/2, height/2, Viewport_ROTATABLE));
	viewports.push_back(new OrthoViewport(width/2, x, width/2, height/2, Viewport_TOP));
	viewports.push_back(new OrthoViewport(x, y + height/2, width/2, height/2, Viewport_FRONT));
	viewports.push_back(new OrthoViewport(width/2, y + height/2, width/2, height/2, Viewport_LEFT));
	
	setCentre(x + width/2, y + height/2);
	
	prevViewport = NULL;
	overCentre = false;
	enabled = false;
}
ofxFourUpDisplay::~ofxFourUpDisplay() {
	for(int i = 0; i < viewports.size(); i++) {
		delete viewports[i];
	}
}


void ofxFourUpDisplay::toggle() {
	setEnabled(!enabled);
}

void ofxFourUpDisplay::setEnabled(bool enabled) {
	if(this->enabled!=enabled) {
		this->enabled = enabled;
		if(this->enabled) {
		
			ofAddListener(ofEvents.mousePressed, this, &ofxFourUpDisplay::mousePressed);
			ofAddListener(ofEvents.mouseMoved, this, &ofxFourUpDisplay::mouseMoved);
			ofAddListener(ofEvents.mouseDragged, this, &ofxFourUpDisplay::mouseDragged);
			ofAddListener(ofEvents.mouseReleased, this, &ofxFourUpDisplay::mouseReleased);
			ofAddListener(ofEvents.draw, this, &ofxFourUpDisplay::draw);
			
		} else {
			ofRemoveListener(ofEvents.mousePressed, this, &ofxFourUpDisplay::mousePressed);
			ofRemoveListener(ofEvents.mouseMoved, this, &ofxFourUpDisplay::mouseMoved);
			ofRemoveListener(ofEvents.mouseDragged, this, &ofxFourUpDisplay::mouseDragged);
			ofRemoveListener(ofEvents.mouseReleased, this, &ofxFourUpDisplay::mouseReleased);
			ofRemoveListener(ofEvents.draw, this, &ofxFourUpDisplay::draw);
		}
	}
	
	
	
}


void ofxFourUpDisplay::draw(ofEventArgs &e) {
	if(!enabled) return;
	ofSetupScreen();
	//ofViewport();
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	glLineWidth(1);
	glDisable(GL_DEPTH_TEST);
	for(int i = 0; i < viewports.size(); i++) {
		ofFill();
		ofSetHexColor(0x1E2832);
		ofRect(*viewports[i]);
		ofNoFill();
		ofSetColor(255, 255, 255);
		
		ofRect(*viewports[i]);
		ofDrawBitmapString(viewports[i]->getName(), viewports[i]->x+4, viewports[i]->y+15);
	}
	if(overCentre) {
		ofRect(centre.x - 3, centre.y - 3, 6, 6);
	}
	
	ofFill();
	
	for(int i = 0; i < viewports.size(); i++) {
		// ignore any really small viewports
		if(viewports[i]->width<5 || viewports[i]->height<5) continue;
		// position the viewport in opengl space.
		viewports[i]->begin();
		
		for(int j = 0; j < scene->getNumNodes(); j++) {
			scene->getNode(j).draw();
		}
		
		viewports[i]->end();
	}
}





bool ofxFourUpDisplay::inRect(ofPoint point, ofRectangle &rect) {
	return point.x>=rect.x && point.y>=rect.y && point.x<=rect.x+rect.width && point.y<=rect.y+rect.height;
}

void ofxFourUpDisplay::setCentre(float x, float y) {
	
	// resize all the viewports
	ofRectangle r = ofRectangle(this->x, this->y, this->width, this->height);
	if(!inRect(ofPoint(x, y), r)) return;
	
	viewports[0]->width = x - this->x;
	viewports[0]->height = y - this->y;
	
	viewports[1]->x = x;
	viewports[1]->y = viewports[0]->y;
	viewports[1]->width = width - (x - this->x);
	viewports[1]->height = y - this->y;
	
	
	viewports[2]->width = x - this->x;
	viewports[2]->y = y;
	viewports[2]->height = height - viewports[0]->height;
	
	viewports[3]->x = x;
	viewports[3]->y = y;
	viewports[3]->height = viewports[2]->height;
	viewports[3]->width = viewports[1]->width;
	centre = ofPoint(viewports[3]->x, viewports[3]->y);
	
}
void ofxFourUpDisplay::mouseMoved(ofMouseEventArgs &m) {
	if(!enabled) return;
	if(ofDist(m.x, m.y, centre.x, centre.y)<10) {
		overCentre = true;
	} else {
		overCentre = false;
	}
}

void ofxFourUpDisplay::mousePressed(ofMouseEventArgs &m) {
	if(!enabled) return;
	prevMouse = ofPoint(m.x, m.y);
	if(ofDist(m.x, m.y, centre.x, centre.y)<10) {
		movingCentre = true;
		return;
	}
	
	for(int i = 0; i < viewports.size(); i++) {
		if(inRect(prevMouse, *viewports[i])) {
			prevViewport = viewports[i];
			return;
		}
	}
	prevViewport = NULL;
	
}

void ofxFourUpDisplay::mouseDragged(ofMouseEventArgs &m) {
	if(!enabled) return;
	ofPoint currMouse = ofPoint(m.x, m.y);
	if(movingCentre) {
		setCentre(m.x, m.y);
		return;
	}
	
	if(prevViewport!=NULL) {
		ofPoint delta = currMouse - prevMouse;
		prevViewport->mouse (delta, m.button);
	}
	prevMouse = currMouse;
	
}
void ofxFourUpDisplay::mouseReleased(ofMouseEventArgs &m) {
	if(!enabled) return;
	prevViewport = NULL;
	movingCentre = false;	
}




bool ofxFourUpDisplay::isEnabled() { return enabled; }