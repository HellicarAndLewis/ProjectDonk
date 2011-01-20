/*
 *  Model.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#pragma once
#include "ofxOBJModel.h"
#include "ofMain.h"
#include "Settings.h"

/**
 * \brief Represents the physical surfaces to project onto in the scene
 */
class Model: public ofxOBJModel { //: public ofNode {
public:
	ofxOBJModel model;
	Model() {
		load(settings.get("model file", "scene.obj"));
	}
};