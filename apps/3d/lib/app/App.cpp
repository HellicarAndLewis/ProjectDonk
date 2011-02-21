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
#include "constants.h"
using namespace util;
using namespace Donk;
App::App() {
	ofSetLogLevel(OF_LOG_NOTICE);

	usingProjectorBlend = settings.getBool("using projector blending");

	scene		= new Scene();
	float x = GUI_PADDING*2 + CAMERA_GUI_WIDTH;
		
	viewports	= new ofxFourUpDisplay(scene, ofRectangle(x, GUI_PADDING, 
														  settings.getInt("projector width") - x - GUI_PADDING,
														  ofGetHeight() - GUI_PADDING*2));
	sceneGui	= new SceneGui(scene);
	if(usingProjectorBlend) {	
		projectorBlend.setup(
						 settings.getInt("projector width"), 
						 settings.getInt("projector height"), 
						 settings.getInt("num projectors"),
						 settings.getInt("projector overlap"),
						 settings.get("layout")=="vertical"?ofxProjectorBlend_Vertical:ofxProjectorBlend_Horizontal,
						settings.get("orientation")=="left"
							 ?
							 ofxProjectorBlend_RotatedLeft:settings.get("orientation")=="right"
								?
								ofxProjectorBlend_RotatedRight:ofxProjectorBlend_NoRotation
						
		);
		ofSetWindowShape(projectorBlend.getCanvasWidth(), projectorBlend.getCanvasHeight());
	}
	
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
	
}

void App::drawAllProjectors() {
	
	if(usingProjectorBlend) {
		// if we're using projector blending, there 
		// will be only one projector.
		projectorBlend.begin();
	}

	for(int i = 0; i < scene->projectors.size(); i++) {
		if(scene->projectors[i]->enabled) {
			scene->projectors[i]->begin();
			this->drawView();
			scene->projectors[i]->end();
		}
	}
	if(usingProjectorBlend) {
		projectorBlend.end();
		projectorBlend.draw(0, 0);
	}
}
void App::_update(ofEventArgs &e) {
	scene->update();
}
void App::_draw(ofEventArgs &e) {
	// just have to have this here for some weird reason
	// - I think it's a bug in 007, maybe it's me. meh.
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	this->render();
	drawAllProjectors();
	if(guiEnabled) {
		ScopedGLCapability depth(GL_DEPTH_TEST, false);
		glViewport(0, 0, ofGetWidth(), ofGetHeight());
		ofSetupScreen();
		glViewport(0, 0, ofGetWidth(), ofGetHeight());
		ofSetupScreen();
		sceneGui->draw();
	}
}
ofPoint lastMouse;
void App::_mousePressed(ofMouseEventArgs &e) {
	if(guiEnabled) {
		//viewports->mousePressed(e.x, e.y, e.button);
		sceneGui->mousePressed(e.x, e.y, e.button);
	}
	lastMouse = ofPoint(e.x, e.y);
}
void App::_mouseMoved(ofMouseEventArgs &e) {
	if(guiEnabled) {
		//viewports->mouseMoved(e.x, e.y);
		sceneGui->mouseMoved(e.x, e.y);
	}
}

void App::_mouseDragged(ofMouseEventArgs &e) {
	ofPoint currMouse = ofPoint(e.x, e.y);
	if(guiEnabled) {
		//viewports->mouseDragged(e.x, e.y, e.button);
		sceneGui->mouseDragged(e.x, e.y, e.button);
	} else {
	}
	
	lastMouse = currMouse;
}
void App::_mouseReleased(ofMouseEventArgs &e) {
	if(guiEnabled) {
		//viewports->mouseReleased(e.x, e.y, e.button);
		sceneGui->mouseReleased(e.x, e.y, e.button);
	}
}

void App::_keyPressed(ofKeyEventArgs &e) {
	switch(e.key) {
		case ' ':
			guiEnabled ^= true;

			break;
		case '2':
			viewports->toggle();
			break;
		case '1':
			sceneGui->toggle();
			break;
		case 'f':
		case 'F':
			ofToggleFullscreen();
			break;
	}
}