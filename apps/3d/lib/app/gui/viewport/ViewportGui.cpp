/*
 *  ViewportGui.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#include "ViewportGui.h"
#include "Settings.h"

// this is just to get the default background color
#include "ofxXmlGui.h"

ViewportGui::ViewportGui(Scene *scene): Enableable() {
	this->scene = scene;
	x = GUI_PADDING*2 + CAMERA_GUI_WIDTH;
	y = GUI_PADDING;
	width = settings.getInt("projector width") - x - GUI_PADDING;
	height = ofGetHeight() - GUI_PADDING*2;
	movingCentre = false;
	
	// create 4 viewports
	viewports.push_back(new OrthoViewport(x, y, width/2, height/2, Viewport_ROTATABLE));
	viewports.push_back(new OrthoViewport(width/2, x, width/2, height/2, Viewport_TOP));
	viewports.push_back(new OrthoViewport(x, y + height/2, width/2, height/2, Viewport_FRONT));
	viewports.push_back(new OrthoViewport(width/2, y + height/2, width/2, height/2, Viewport_LEFT));
	
	setCentre(x + width/2, y + height/2);
	
	prevViewport = NULL;
	overCentre = false;
}
ViewportGui::~ViewportGui() {
	for(int i = 0; i < viewports.size(); i++) {
		delete viewports[i];
	}
}
void ViewportGui::draw() {
	if(!enabled) return;
	ofSetupScreen();
	ofViewport();
	for(int i = 0; i < viewports.size(); i++) {
		ofFill();
		ofSetHexColor(OFXXMLGUI_DEFAULT_BG_COLOR);
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
		// position the viewport in opengl space.
		viewports[i]->begin();
		
		// draw each projector in the scene where it's supposed to be
		for(int j = 0; j < scene->projectors.size(); j++) {
			if(scene->projectors[j]->enabled) {
				scene->projectors[j]->draw();
			}
		}

		for(int j = 0; j < scene->kinects.size(); j++) {
			if(scene->kinects[j]->enabled) {
				scene->kinects[j]->draw();
			}
		}
		// draw the actual model
		glColor3f(1, 1, 1);
		scene->getModel()->draw(false);
		viewports[i]->end();
	}
}





bool ViewportGui::inRect(ofPoint point, ofRectangle &rect) {
	return point.x>=rect.x && point.y>=rect.y && point.x<=rect.x+rect.width && point.y<=rect.y+rect.height;
}

void ViewportGui::setCentre(float x, float y) {
	
	// resize all the viewports
	
	if(!inRect(ofPoint(x, y), *this)) return;
	
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
void ViewportGui::mouseMoved(float x, float y) {
	if(!enabled) return;
	if(ofDist(x, y, centre.x, centre.y)<10) {
		overCentre = true;
	} else {
		overCentre = false;
	}
}

void ViewportGui::mousePressed(float x, float y, int button) {
	if(!enabled) return;
	prevMouse = ofPoint(x, y);
	if(ofDist(x, y, centre.x, centre.y)<10) {
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

void ViewportGui::mouseDragged(float x, float y, int button) {
	if(!enabled) return;
	ofPoint currMouse = ofPoint(x, y);
	if(movingCentre) {
		setCentre(x, y);
		return;
	}
	
	if(prevViewport!=NULL) {
		ofPoint delta = currMouse - prevMouse;
		prevViewport->mouse (delta, button);
	}
	prevMouse = currMouse;
	
}
void ViewportGui::mouseReleased(float x, float y, int button) {
	if(!enabled) return;
	prevViewport = NULL;
	movingCentre = false;	
}




