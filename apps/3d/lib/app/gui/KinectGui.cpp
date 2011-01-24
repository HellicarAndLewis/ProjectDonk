/*
 *  KinectGui.cpp
 *  KinectExample
 *
 *  Created by Marek Bereza on 24/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#include "KinectGui.h"

KinectGui::KinectGui(Scene *scene) {
	this->scene = scene;
	setKinects(scene->kinects);
	currKinectIndex = 0;
	kinectSelector.setup(10, 10, 250);
	vector<string> kinectNameList;

	//kinectSelector.addTitle("Kinect Calibrator");

	for(int i = 0; i < kinects.size(); i++) {
		kinectSelector.addPage(kinects[i]->name);
		kinectSelector.addSlider("near clip", kinects[i]->nearClip, 0, 255);
		kinectSelector.addSlider("far clip", kinects[i]->farClip, 0, 255);
		kinectSelector.addToggle("flip x", kinects[i]->flipX);
		kinectSelector.addToggle("flip y", kinects[i]->flipY);
		
	}
	
	kinectSelector.enableAutoSave("kinects.xml");
	// we want events to come through
	// from the app, not through the oF events mechanism.
	kinectSelector.disable();
	
	// disable() hides the elements as well as disabling the oF events
	// so we need to call show() to show the elements if we draw them.
	kinectSelector.show();
}
void KinectGui::mousePressed(float x, float y, int button) {
	if(!enabled) return;
	kinectSelector.touchDown(x, y, button);
}
void KinectGui::mouseReleased(float x, float y, int button) {
	if(!enabled) return;
	kinectSelector.touchUp(x, y, button);
}
void KinectGui::mouseDragged(float x, float y, int button) {
	if(!enabled) return;
	kinectSelector.touchMoved(x, y, button);
}
void KinectGui::mouseMoved(float x, float y) {
	if(!enabled) return;
	kinectSelector.touchOver(x, y, 0);
}

void KinectGui::setKinects(vector<Kinect*> &kinects) {
	
	// copies over the kinects.
	for(int i = 0; i < kinects.size(); i++) {
		this->kinects.push_back(kinects[i]);
	}
}


void KinectGui::draw() {
	if(!enabled) return;
	kinectSelector.draw();
	
	// draw the background - make it the same aspect as the kinect
	float x = kinectSelector.x + kinectSelector.width+20;
	ofSetHexColor(OFXXMLGUI_DEFAULT_BG_COLOR);
	ofRectangle rect(x, kinectSelector.y, WIDTH - x - 10, HEIGHT - 10*2);
	
	// maintain aspect
	rect.height = rect.width * kinects[currKinectIndex]->getHeight() / kinects[currKinectIndex]->getWidth();
	
	ofRect(rect); 
	
	// white border
	ofSetHexColor(0xFFFFFF);
	ofNoFill();
	ofRect(rect);
	
	ofFill();
	
	// inset the rect
	rect.x += 10;
	rect.y += 10;
	rect.width -= 20;
	rect.height -= 20;
	kinects[currKinectIndex]->drawCalibration(rect);
	
}