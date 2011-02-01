/*
 *  Scene.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */

#include "Scene.h"
#include "ofxSettings.h"

Scene::Scene() {
	
	
	
	bool usingFirstScreenForGuiOnly = settings.getBool("using first screen for gui only", false);
	int numProjectors	= settings.getInt("num projectors", 1);
	int numKinects		= settings.getInt("num kinects", 1);
	int projectorWidth	= settings.getInt("projector width", 1024);
	int projectorHeight = settings.getInt("projector height", 768);
	
	
	for(int i = 0; i < numKinects; i++) {
		
		// add it to our local list.
		kinects.push_back(new Kinect("kinect " + ofToString(i+1)));
		
		// add it to the scene list
		addNode(*kinects.back());
		if(!kinects.back()->open()) {
			ofLog(OF_LOG_ERROR, "Failed to open %s\n", kinects.back()->name.c_str());
		} else {
			ofLog(OF_LOG_NOTICE, "Successfully %s\n", kinects.back()->name.c_str());
		}
	}
	
	for(int i = 0; i < numProjectors; i++) {
		int offset = 0;
		if(usingFirstScreenForGuiOnly) offset++;

		// add it to our local list
		projectors.push_back(new Projector("proj " + ofToString(i+1), projectorWidth*(i+offset), 0, projectorWidth, projectorHeight));

		// add it to the scene list
		addNode(*projectors.back());
		ofLog(OF_LOG_NOTICE, "projector %d is (%f %f %f %f)\n", i, projectors.back()->x, projectors.back()->y, projectors.back()->width, projectors.back()->height);
	}
	// create a new model and add it to the scene
	model = new Model();
	addNode(*model);
}


Scene::~Scene() {
	for(int i = 0; i < kinects.size(); i++) {
		kinects[i]->close();
	}
}
Model *Scene::getModel() {
	return model;
}
void Scene::update() {
	for(int i = 0; i < kinects.size(); i++) {
		kinects[i]->update();
	}
	
	for(int i = 0; i < projectors.size(); i++) {
		projectors[i]->update();
	}
}