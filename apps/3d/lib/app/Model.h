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
#include "Settings.h"

/**
 * \brief Represents the physical surfaces to project onto in the scene
 */
class Model: public ofNode { //: public ofNode {
public:
	ofxOBJModel model;
	Model() {
		model.load(settings.get("model file", "scene.obj"));
	}
	void customDraw() {
		ofSetColor(255, 255, 255);
		model.draw(false);
	}
};