/*
 *  Scene.cpp
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */

#include "Scene.h"
#include "ofxSettings.h"

Scene::Scene(int w, int h) {
	
	
	
	bool usingFirstScreenForGuiOnly = settings.getBool("using first screen for gui only", false);
	int numProjectors	= settings.getInt("num projectors", 1);
	int projectorWidth	= settings.getInt("projector width", 1024);
	int projectorHeight = settings.getInt("projector height", 768);

	bool doingProjectorBlend = settings.getBool("using projector blending", false);
	
	// if we're doing projector blending
	if(doingProjectorBlend) {
		numProjectors = 1;

		projectorWidth = w;
		projectorHeight = h;
		//printf("%d %d\n", projectorWidth, projectorHeight);
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
	light.setDirectional(false);
	addNode(light);
}


Scene::~Scene() {
	
}
Model *Scene::getModel() {
	return model;
}
void Scene::update() {
		
	for(int i = 0; i < projectors.size(); i++) {
		projectors[i]->update();
	}
}

Light *Scene::getLight() {
	return &light;
}