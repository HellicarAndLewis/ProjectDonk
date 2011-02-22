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
	bool disableWhite;
	Model() {
		disableWhite = false;
		solid = false;
		model.load(settings.get("model file", "scene.obj"));
	}
	
	void drawSolid() {
		solid = true;
		disableWhite = true;
		draw();
		disableWhite = false;
		solid = false;
	}
	void customDraw() {
		if(!disableWhite) glColor3f(1,1,1);
		model.draw(solid);
	}
	void drawModel() {
		model.draw(true);
	}
};