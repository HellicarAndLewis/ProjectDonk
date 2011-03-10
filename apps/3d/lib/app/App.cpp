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
#include "ofxMacKeys.h"

using namespace util;
using namespace Donk;
float f = 0;
bool show4Up = false;
int whichGui = 0;

App::App() {
	ofSetLogLevel(OF_LOG_NOTICE);
	
	float x = GUI_PADDING*2 + CAMERA_GUI_WIDTH;
	
	ofAddListener(ofEvents.keyPressed, this, &App::_keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &App::_keyReleased);
	ofAddListener(ofEvents.draw, this, &App::_draw);
	ofAddListener(ofEvents.update, this, &App::_update);
	
	usingProjectorBlend = settings.getBool("using projector blending");

	
	
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
		projectorBlend.setWindowToDisplaySize();
		scene		= new Scene(projectorBlend.getCanvasWidth(), projectorBlend.getCanvasHeight());
	} else {
		scene		= new Scene();
	}
	sceneGui	= new SceneGui(scene);
	
	
	
	guiEnabled = true;
	if(settings.getBool("using first screen for gui only", false)) {
		guiEnabled = true;
	}
	
	guiChooser.setup(10, 10, 200);
	guiChooser.addSegmentedControl(" ", whichGui, "Project|Mode|Calibrate|Blend");
	guiChooser.addToggle("Show 4-up", show4Up);
	guiChooser.enable();
	guiChooser.height = 25;
	guiChooser.addListener(this);
	
	sceneGui->setEnabled(true);
	modeGui = Mode::getInstance()->getGui();
	modeGui->disable();
	calibrationGui = new ofxXmlGui();
	calibrationGui->setup(10, 60, 200);
	calibrationGui->disable();
	
	projectorBlendGui = new ofxXmlGui();
	projectorBlendGui->setup(10, 60, 200);
	projectorBlendGui->disable();
	projectorBlendGui->addToggle("Show blend", projectorBlend.showBlend);
	projectorBlendGui->addSlider("Blend Power", projectorBlend.blendPower, 0, 4);
	projectorBlendGui->addSlider("Gamma", projectorBlend.gamma, 0, 4);
	projectorBlendGui->addSlider("Luminance", projectorBlend.luminance, 0, 4);
	projectorBlendGui->enableAutoSave("settings/projectorBlending.xml");
	viewports	= new ofxFourUpDisplay(scene, ofRectangle(x, GUI_PADDING, 
														  settings.getInt("projector width") - x - GUI_PADDING,
														  ofGetHeight() - GUI_PADDING*2));
	
	viewports->setEnabled(show4Up);//gui.getControlById("Show 4-up")->boolValue());
}

void App::controlChanged(GuiControl *control) {
	if(control->controlId==" ") { // it's the gui chooser! - fire a mouse press
		ofKeyEventArgs e;
		e.key = '1' + whichGui;
		_keyPressed(e);
	}
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
			if(usingProjectorBlend) {
				glViewport(0, 0, projectorBlend.getCanvasWidth(), projectorBlend.getCanvasHeight());
			} else {
				// don't know if we need this (it was originally inside drawView()
				// for non-projector blending. it was breaking the projector blend.
				glViewport(0, 0, ofGetWidth(), ofGetHeight());
			}
			
			
			this->drawView();
			
			scene->projectors[i]->end();
		}
	}
	
	if(usingProjectorBlend) {
		
		projectorBlend.end();
		
		//ofViewport(0, 0, ofGetWidth(), ofGetHeight());
		
		
		
		// turn everything upside down!?
		//glPushMatrix();
		//glTranslatef(0, projectorBlend.getCanvasHeight(), 0);
		//glScalef(1, -1, 1);
		
		projectorBlend.draw(0, 0);
		//glPopMatrix();
	} else {
		ofViewport(0, 0, ofGetWidth(), ofGetHeight());
		ofSetupScreen();
	}
}

void App::_update(ofEventArgs &e) {
	scene->update();	
	// [en|dis]able 4up if needed
	if(show4Up!=viewports->isEnabled()) {
		viewports->setEnabled(show4Up);
	}
}

void App::_draw(ofEventArgs &e) {
	// just have to have this here for some weird reason
	// - I think it's a bug in 007, maybe it's me. meh.
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	
	
	
	this->render();
	drawAllProjectors();

	if(guiEnabled) {
		guiChooser.draw();
		if(whichGui==0) sceneGui->draw();
		else if(whichGui==1) modeGui->draw();
		else if(whichGui==2) calibrationGui->draw();
		else if(whichGui==3) projectorBlendGui->draw();
	}
}


int lastGui = 1;
bool zedDown = false;
bool exDown = false;
bool zooming = false;

void App::_keyReleased(ofKeyEventArgs &e) {
	switch(e.key) {
		case '/':
		case '`':
			zooming = false;
			break;
		case 'x':
		case 'X':
			exDown = false;
			break;
			
		case 'z':
		case 'Z':
			zedDown = false;
			break;
	}
}


void App::_keyPressed(ofKeyEventArgs &e) {
	
	float increment = ofxMacShiftKeyDown()?0.1:0.005;
	
	// printf("Key pressed %c\n", e.key);
	switch(e.key) {
		case ' ':
			guiEnabled ^= true;
			if(!guiEnabled) {
				sceneGui->setEnabled(false);
				modeGui->disable();
				calibrationGui->disable();
				guiChooser.disable();
				projectorBlendGui->disable();
			} else {
				e.key = lastGui;
				_keyPressed(e);
			}
			break;
			
		case '1':
			guiChooser.enable();
			whichGui = 0;
			lastGui = '1';
			guiEnabled = true;
			sceneGui->setEnabled(true);
			modeGui->disable();
			calibrationGui->disable();
			projectorBlendGui->disable();
			break;
			
		case '2':
			guiChooser.enable();
			whichGui = 1;
			lastGui = '2';
			guiEnabled = true;
			sceneGui->setEnabled(false);
			modeGui->enable();
			calibrationGui->disable();
			projectorBlendGui->disable();
			break;
			
		case '3':
			guiChooser.enable();
			whichGui = 2;
			lastGui = '3';
			guiEnabled = true;
			sceneGui->setEnabled(false);
			modeGui->disable();
			calibrationGui->enable();
			projectorBlendGui->disable();
			break;
			
		case '4':
			guiChooser.enable();
			whichGui = 3;
			lastGui = '4';
			guiEnabled = true;
			sceneGui->setEnabled(false);
			modeGui->disable();
			calibrationGui->disable();
			projectorBlendGui->enable();
			break;
			
		case 'f':
		case 'F':
			ofToggleFullscreen();
			break;
		case 'z':
		case 'Z':
			zedDown = true;
			break;
		case 'x':
		case 'X':
			exDown = true;
			break;
			
		case OF_KEY_UP:
			if(zedDown) scene->projectors[0]->pos.z+=increment;
			else if(exDown) scene->projectors[0]->pos.y-=increment;
			else scene->projectors[0]->rot.y -= increment;
			sceneGui->save();
			break;
			
		case OF_KEY_DOWN:
			if(zedDown) scene->projectors[0]->pos.z-=increment;
			else if(exDown) scene->projectors[0]->pos.y+=increment;
			else scene->projectors[0]->rot.y += increment;
			sceneGui->save();
			break;
		case OF_KEY_LEFT:
			if(exDown) {
				// pan left
				scene->projectors[0]->pos.x+=increment;
			} else if(!zedDown) {
				// rotate left
				scene->projectors[0]->rot.x+=increment;
			}
			sceneGui->save();
			break;
			
		case OF_KEY_RIGHT:
			if(exDown) {
				// pan right
				scene->projectors[0]->pos.x-=increment;
			} else if(!zedDown) {
				// rotate right
				scene->projectors[0]->rot.x-=increment;
			}
			sceneGui->save();
			break;
	}
}

ofxXmlGui *App::getCalibrationGui() {
	return calibrationGui;
}