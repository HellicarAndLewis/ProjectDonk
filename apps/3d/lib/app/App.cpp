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

App::App() {
	ofSetLogLevel(OF_LOG_NOTICE);
	show4Up = false;
	whichGui = 0;
	blendEnabled = false;

	for(int i = 0; i < 8; i++) {
		blendEditingEnabled[i] = false;
	}
	float x = GUI_PADDING*2 + CAMERA_GUI_WIDTH;
	
	ofAddListener(ofEvents.keyPressed, this, &App::_keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &App::_keyReleased);
	ofAddListener(ofEvents.draw, this, &App::_draw);
	ofAddListener(ofEvents.update, this, &App::_update);
	screenFbo.setup(settings.getFloat("projector width"), settings.getFloat("projector height"), GL_RGB, 4);
	maskMode = false;

	currProjectorId = 0;
	scene		= new Scene();

	sceneGui	= new SceneGui(scene);
	
	blendPower = 1;
	blendGamma = 1;
	blendLuminance = 1;
	
	
	guiEnabled = false;
	if(settings.getBool("using first screen for gui only", false)) {
		guiEnabled = true;
	}
	
	guiChooser.setup(10, 10, 200);
	guiChooser.addSegmentedControl(" ", whichGui, "Project|Mode|Calibrate|Blend");
	guiChooser.addToggle("Show 4-up", show4Up);
	guiChooser.disable();
	guiChooser.height = 25;
	guiChooser.addListener(this);
	
	sceneGui->setEnabled(false);
	modeGui = Mode::getInstance()->getGui();
	modeGui->disable();
	calibrationGui = new ofxXmlGui();
	calibrationGui->setup(10, 60, 200);
	calibrationGui->disable();
	
	
	// setup masks - one for each projector
	for(int i = 0; i < scene->projectors.size(); i++) {
		masks.push_back(new ofxPolygonMask());
		masks.back()->setup(string("mask") + ofToString(i) + ".xml");
		masks.back()->setPosition(scene->projectors[i]->x, scene->projectors[i]->y);
	}
	
	// projector blend/editing gui.

	blendGui = new ofxXmlGui();
	blendGui->setup(10, 60, 200);
	blendGui->addToggle("enable blend", blendEnabled);
	for(int i = 0; i < masks.size(); i++) {
		blendGui->addToggle(string("enable editing ")+ofToString(i+1), blendEditingEnabled[i]);
	}
	
	blendGui->addSlider("Blend power", blendPower, 0, 4);
	blendGui->addSlider("gamma", blendGamma, 0, 4);
	blendGui->addSlider("luminance", blendLuminance, 0, 4);
	
	blendGui->disable();
	blendGui->enableAutoSave("settings/blend.xml");
	
	
	
	viewports	= new ofxFourUpDisplay(scene, ofRectangle(x, GUI_PADDING, 
														  settings.getInt("projector width") - x - GUI_PADDING,
														  ofGetHeight() - GUI_PADDING*2));
	
	viewports->setEnabled(show4Up);//gui.getControlById("Show 4-up")->boolValue());
	ofSetFullscreen(true);
	
}

void App::controlChanged(GuiControl *control) {
	if(control->controlId==" ") { // it's the gui chooser! - fire a mouse press
		ofKeyEventArgs e;
		e.key = '1' + whichGui;
		_keyPressed(e);
	}
}

void App::drawAllProjectors() {
	
	

	for(int i = 0; i < scene->projectors.size(); i++) {
		if(scene->projectors[i]->enabled) {

			screenFbo.begin();
			ofClear(0, 0, 0, 0);
			scene->projectors[i]->begin();
			// don't know if we need this (it was originally inside drawView()
			// for non-projector blending. it was breaking the projector blend.
			//glViewport(0, 0, ofGetWidth(), ofGetHeight());
			
			this->drawView();
			
			scene->projectors[i]->end();
			screenFbo.end();
			
			ofSetupScreen();
			if(blendEnabled) {
				
				masks[i]->setTexture(&screenFbo.getTexture(0));
				ofEventArgs e;
				masks[i]->draw(e);
			} else {
				
				glPushMatrix();
			
				screenFbo.draw(scene->projectors[i]->x, scene->projectors[i]->y);
				glPopMatrix();
			}
		}
	}
	
	
	
	
}

void App::_update(ofEventArgs &e) {
	
	if(guiEnabled) {
		ofShowCursor();
	} else {
		ofHideCursor();
	}
	
	for(int i = 0; i < masks.size(); i++) {
		masks[i]->blendPower = blendPower;
		masks[i]->gamma = blendGamma;
		masks[i]->luminance = blendLuminance;
		masks[i]->setEnableEditing(blendEditingEnabled[i]);
	}
	
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
		else if(whichGui==3) blendGui->draw();
	}
}


int lastGui = 1;
bool zedDown = false;
bool exDown = false;
bool zooming = false;
bool aDown = false;
bool vDown = false;

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
		case 'v':
		case 'V':
			vDown = false;
			break;
		case 'a':
		case 'A':
			aDown = false;
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
				blendGui->disable();
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
			blendGui->disable();
			break;
			
		case '2':
			guiChooser.enable();
			whichGui = 1;
			lastGui = '2';
			guiEnabled = true;
			sceneGui->setEnabled(false);
			modeGui->enable();
			calibrationGui->disable();
			blendGui->disable();
			break;
			
		case '3':
			guiChooser.enable();
			whichGui = 2;
			lastGui = '3';
			guiEnabled = true;
			sceneGui->setEnabled(false);
			modeGui->disable();
			calibrationGui->enable();
			blendGui->disable();
			break;
			
		case '4':
			guiChooser.enable();
			whichGui = 3;
			lastGui = '4';
			guiEnabled = true;
			sceneGui->setEnabled(false);
			modeGui->disable();
			calibrationGui->disable();
			blendGui->enable();
			break;
		case 'm':
			maskMode ^= true;
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
		case 'v':
		case 'V':
			vDown = true;
			break;
		case 'a':
		case 'A':
			aDown = true;
			break;
			
		case OF_KEY_UP:
			if(zedDown) scene->projectors[currProjectorId]->pos.z+=increment;
			else if(exDown) scene->projectors[currProjectorId]->pos.y-=increment;
			else scene->projectors[currProjectorId]->rot.y -= increment;
			sceneGui->save();
			break;
			
		case OF_KEY_DOWN:
			if(zedDown) scene->projectors[currProjectorId]->pos.z-=increment;
			else if(exDown) scene->projectors[currProjectorId]->pos.y+=increment;
			else if(vDown) scene->projectors[currProjectorId]->fov += increment;
			else scene->projectors[currProjectorId]->rot.y += increment;
			sceneGui->save();
			break;
		case OF_KEY_LEFT:
			if(exDown) {
				// pan left
				scene->projectors[currProjectorId]->pos.x+=increment;
			} else if(zedDown) {
				scene->projectors[currProjectorId]->rot.z+=increment;
			} else {
				// rotate left
				scene->projectors[currProjectorId]->rot.x+=increment;
			}

			sceneGui->save();
			break;
			
		case OF_KEY_RIGHT:
			if(exDown) {
				// pan right
				scene->projectors[currProjectorId]->pos.x-=increment;
			} else if(zedDown) {
				scene->projectors[currProjectorId]->rot.z-=increment;
			} else {
				// rotate right
				scene->projectors[currProjectorId]->rot.x-=increment;
			}
			sceneGui->save();
			break;
		case '[':
			currProjectorId--;
			if(currProjectorId<0) currProjectorId = 0;
			break;
		case ']':
			currProjectorId++;
			if(currProjectorId>=scene->projectors.size()) currProjectorId = scene->projectors.size()-1;
			break;
	}
}

ofxXmlGui *App::getCalibrationGui() {
	return calibrationGui;
}