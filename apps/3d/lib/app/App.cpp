/*
 *  App.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */
#include "App.h"
#include "ofxSettings.h"
#include "GLHelpers.h"

using namespace util;

App::App() {
	ofSetLogLevel(OF_LOG_NOTICE);

	scene		= new Scene();
	float x = GUI_PADDING*2 + CAMERA_GUI_WIDTH;
	viewports	= new ofxFourUpDisplay(scene, ofRectangle(x, GUI_PADDING, 
														  settings.getInt("projector width") - x - GUI_PADDING,
														  ofGetHeight() - GUI_PADDING*2));
	sceneGui	= new SceneGui(scene);
	kinectGui	= new KinectGui(scene);
	
	ofAddListener(ofEvents.mousePressed, this, &App::_mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &App::_mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &App::_mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &App::_mouseReleased);
	ofAddListener(ofEvents.keyPressed, this, &App::_keyPressed);
	ofAddListener(ofEvents.draw, this, &App::_draw);
	ofAddListener(ofEvents.update, this, &App::_update);
	
	guiEnabled = false;
	if(settings.getBool("using first screen for gui only", false)) {
		guiEnabled = true;
	}

	viewports->setEnabled(true);
	sceneGui->setEnabled(true);
	kinectGui->setEnabled(true);
	
}

void App::drawAllProjectors() {
	for(int i = 0; i < scene->projectors.size(); i++) {
		if(scene->projectors[i]->enabled) {
			scene->projectors[i]->begin();
			this->drawView();
			scene->projectors[i]->end();
		}
	}
}
void App::_update(ofEventArgs &e) {
	scene->update();
}
void App::_draw(ofEventArgs &e) {
	this->render();
	drawAllProjectors();
	if(guiEnabled) {
		ScopedGLCapability depth(GL_DEPTH_TEST, false);
		glViewport(0, 0, ofGetWidth(), ofGetHeight());
		ofSetupScreen();
		glViewport(0, 0, ofGetWidth(), ofGetHeight());
		ofSetupScreen();
		sceneGui->draw();
		kinectGui->draw();
		
	}
}
ofPoint lastMouse;
void App::_mousePressed(ofMouseEventArgs &e) {
	if(guiEnabled) {
		//viewports->mousePressed(e.x, e.y, e.button);
		sceneGui->mousePressed(e.x, e.y, e.button);
		kinectGui->mousePressed(e.x, e.y, e.button);
	}
	lastMouse = ofPoint(e.x, e.y);
}
void App::_mouseMoved(ofMouseEventArgs &e) {
	if(guiEnabled) {
		//viewports->mouseMoved(e.x, e.y);
		sceneGui->mouseMoved(e.x, e.y);
		kinectGui->mouseMoved(e.x, e.y);
	}
}

void App::_mouseDragged(ofMouseEventArgs &e) {
	ofPoint currMouse = ofPoint(e.x, e.y);
	if(guiEnabled) {
		//viewports->mouseDragged(e.x, e.y, e.button);
		sceneGui->mouseDragged(e.x, e.y, e.button);
		kinectGui->mouseDragged(e.x, e.y, e.button);
	} else {
	}
	
	lastMouse = currMouse;
}
void App::_mouseReleased(ofMouseEventArgs &e) {
	if(guiEnabled) {
		//viewports->mouseReleased(e.x, e.y, e.button);
		sceneGui->mouseReleased(e.x, e.y, e.button);
		kinectGui->mouseReleased(e.x, e.y, e.button);
	}
}

void App::_keyPressed(ofKeyEventArgs &e) {
	switch(e.key) {
		case ' ':
			guiEnabled ^= true;

			break;
		case '2':
			viewports->toggle();
			kinectGui->setEnabled(false);
			break;
		case '1':
			sceneGui->toggle();
			kinectGui->setEnabled(false);
			break;
		case '3':
			viewports->setEnabled(false);
			sceneGui->setEnabled(false);
			kinectGui->setEnabled(true);
			break;
		case 'f':
		case 'F':
			ofToggleFullscreen();
			break;
	}
}