/*
 *  SceneGui.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */

#include "SceneGui.h"
#include "ofxSettings.h"
#include "constants.h"

float a = 0;
bool show4Up = false;
SceneGui::SceneGui(Scene *scene): Enableable() {
	this->scene = scene;

	gui.setup(GUI_PADDING, GUI_PADDING, CAMERA_GUI_WIDTH);	
	
	float x = GUI_PADDING*2 + CAMERA_GUI_WIDTH;
	
	viewports	= new ofxFourUpDisplay(scene, ofRectangle(x, GUI_PADDING, 
														  settings.getInt("projector width") - x - GUI_PADDING,
														  ofGetHeight() - GUI_PADDING*2));
	
	
	for(int i = 0; i < scene->projectors.size(); i++) {
		
		gui.addPage(scene->projectors[i]->name);
		
		// add this to every page. It won't kill it!
		gui.addToggle("Show 4-up", show4Up);
		gui.addToggle("enabled", scene->projectors[i]->enabled);
		gui.addSlider("fov", scene->projectors[i]->fov, 0, 180);

		gui.addSlider2D("xy position", scene->projectors[i]->pos, -10, 10, -10, 10);
		gui.addPanner("z position", scene->projectors[i]->pos.z, -10, 10);
		gui.addSlider2D("xy rotation", scene->projectors[i]->rot, -180, 180, -180, 180);
		gui.addPanner("z rotation", scene->projectors[i]->rot.z, -180, 180);
		gui.addToggle("Rotate then position", scene->projectors[i]->rotateThenPosition);
	}
	
	gui.setPage(0);
	
	gui.enableAutoSave("settings/cameras.xml");
	// we want events to come through
	// from the app, not through the oF events mechanism.
	gui.disable();
	
	// disable() hides the elements as well as disabling the oF events
	// so we need to call show() to show the elements if we draw them.
	gui.show();
	viewports->setEnabled(gui.getControlById("Show 4-up")->boolValue());
	if(enabled) {
		gui.enable();
	} else {
		gui.disable();
	}
	gui.addListener(this);
}

void SceneGui::controlChanged(GuiControl *control) {
	if(control->controlId=="Show 4-up") {
		viewports->setEnabled(control->boolValue());
	}
}


void SceneGui::setEnabled(bool enabled) {
	this->enabled = enabled;
	if(enabled) {
		gui.enable();
	} else {
		gui.disable();
	}
	if(enabled && gui.getControlById("Show 4-up")->boolValue()) {
		viewports->setEnabled(true);
	} else {
		viewports->setEnabled(false);
	}
}