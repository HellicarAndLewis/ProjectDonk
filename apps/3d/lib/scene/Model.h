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
	
	void drawOutline() {
		disableWhite = true;
		draw();
		disableWhite = false;
		
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
		
		/*
		ofVec3f *verts = model.getPoints();
		int numVerts = model.getNumPoints();
		
		for(int i = 0; i < numVerts; i++) {
			ofSetHexColor(i*2);
			ofCircle(verts[i], 0.1);
		}
		*/
		
	}
	void drawModel() {
		model.draw(true);
	}
};