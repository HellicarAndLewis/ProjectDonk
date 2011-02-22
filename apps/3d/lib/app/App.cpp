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
#include "Mode.h"

using namespace util;
using namespace Donk;
App::App() {
	ofSetLogLevel(OF_LOG_NOTICE);

	usingProjectorBlend = settings.getBool("using projector blending");

	scene		= new Scene();
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
	
	ofAddListener(ofEvents.keyPressed, this, &App::_keyPressed);
	ofAddListener(ofEvents.draw, this, &App::_draw);
	ofAddListener(ofEvents.update, this, &App::_update);
	
	guiEnabled = true;
	if(settings.getBool("using first screen for gui only", false)) {
		guiEnabled = true;
	}

	sceneGui->setEnabled(false);
	modeGui = Mode::getInstance()->getGui();
	modeGui->enable();
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

		
/*	ScopedGLCapability depth(GL_DEPTH_TEST, false);*/
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	ofSetupScreen();
	glViewport(0, 0, ofGetWidth(), ofGetHeight());
	ofSetupScreen();
}


int lastGui = 1;
void App::_keyPressed(ofKeyEventArgs &e) {
	switch(e.key) {
		case ' ':
			guiEnabled ^= true;
			if(!guiEnabled) {
				sceneGui->setEnabled(false);
				modeGui->disable();
			} else {
				e.key = lastGui;
				_keyPressed(e);
			}
			break;
		case '1':
			lastGui = '1';
			guiEnabled = true;
			sceneGui->setEnabled(true);
			modeGui->disable();
			break;
		case '2':
			lastGui = '2';
			guiEnabled = true;
			sceneGui->setEnabled(false);
			modeGui->enable();
			break;
			
		case 'f':
		case 'F':
			ofToggleFullscreen();
			break;
	}
}