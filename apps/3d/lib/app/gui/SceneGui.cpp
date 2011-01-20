/*
 *  SceneGui.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#include "SceneGui.h"
#include "Settings.h"

float a = 0;
SceneGui::SceneGui(Scene *scene): Enableable() {
	this->scene = scene;

	gui.setup(GUI_PADDING, GUI_PADDING, CAMERA_GUI_WIDTH);	
	
	
	for(int i = 0; i < scene->projectors.size(); i++) {
		gui.addPage(scene->projectors[i]->name);
		gui.addToggle("enabled", scene->projectors[i]->enabled);
		gui.addSlider("fov", scene->projectors[i]->fov, 0, 180);

		gui.addSlider2D("xy position", scene->projectors[i]->pos, -10, 10, -10, 10);
		gui.addPanner("z position", scene->projectors[i]->pos.z, -10, 10);
		gui.addSlider2D("xy rotation", scene->projectors[i]->rot, -180, 180, -180, 180);
		gui.addPanner("z rotation", scene->projectors[i]->rot.z, -180, 180);
		gui.addToggle("Rotate then position", scene->projectors[i]->rotateThenPosition);
	}


	for(int i = 0; i < scene->kinects.size(); i++) {
		gui.addPage(scene->kinects[i]->name);
		gui.addToggle("enabled", scene->kinects[i]->enabled);
		gui.addPanner("z", scene->kinects[i]->pos.z, -10, 10);
		gui.addSlider2D("position", scene->kinects[i]->pos, -10, 10, -10, 10);
	}
	gui.setPage(0);
	
	gui.enableAutoSave("settings/cameras.xml");
	// we want events to come through
	// from the app, not through the oF events mechanism.
	gui.disable();
	
	// disable() hides the elements as well as disabling the oF events
	// so we need to call show() to show the elements if we draw them.
	gui.show();
}



void SceneGui::mousePressed(float x, float y, int button) {
	if(enabled) gui.touchDown(x, y, button);
}
void SceneGui::mouseReleased(float x, float y, int button) {
	if(enabled) gui.touchUp(x, y, button);
}
void SceneGui::mouseDragged(float x, float y, int button) {
	if(enabled) gui.touchMoved(x, y, button);
}
void SceneGui::mouseMoved(float x, float y) {
	if(enabled) gui.touchOver(x, y, 0);
}

void SceneGui::draw() {
	if(enabled) gui.draw();

}