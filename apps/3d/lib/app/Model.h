/*
 *  Model.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */

#pragma once
#include "ofxOBJModel.h"
#include "ofMain.h"
#include "ofxSettings.h"

/**
 * \brief Represents the physical surfaces to project onto in the scene
 */
class Model: public ofNode { //: public ofNode {
public:
	ofxOBJModel model;
	bool solid;
	
	Model() {
		solid = false;
		model.load(settings.get("model file", "scene.obj"));
	}
	
	void drawSolid() {
		solid = true;
		draw();
		solid = false;
	}
	void customDraw() {
		glColor3f(1,1,1);
		model.draw(solid);
	}
	void drawModel() {
		glColor3f(1,1,1);
		model.draw(true);
	}
};