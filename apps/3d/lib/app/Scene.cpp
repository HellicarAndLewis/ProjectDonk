/*
 *  Scene.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#include "Scene.h"
#include "Settings.h"

Scene::Scene() {
	
	
	
	bool usingFirstScreenForGuiOnly = settings.getBool("using first screen for gui only", false);
	int numProjectors	= settings.getInt("num projectors", 1);
	int numKinects		= settings.getInt("num kinects", 1);
	int projectorWidth	= settings.getInt("projector width", 1024);
	int projectorHeight = settings.getInt("projector height", 768);
	
	
	for(int i = 0; i < numKinects; i++) {
		kinects.push_back(new Kinect("kinect " + ofToString(i+1)));
	}
	
	for(int i = 0; i < numProjectors; i++) {
		int offset = 0;
		if(usingFirstScreenForGuiOnly) offset++;
		projectors.push_back(new Projector("proj " + ofToString(i+1), projectorWidth*(i+offset), 0, projectorWidth, projectorHeight));

		printf("projector %d is (%f %f %f %f)\n", i, projectors.back()->x, projectors.back()->y, projectors.back()->width, projectors.back()->height);
	}
	model = new Model();
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