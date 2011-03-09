/*
 *  Champagne.h
 *  motionFluid
 *
 *  Created by Josh Nimoy on 3/8/11.
 *
 */

#pragma once

#include "BrownianObject.h"

class Champagne{
public:

	Champagne();
	void setup();
	void update();
	void draw();
	
	void createObjectAtPoint(ofVec3f point);
	
	vector<BrownianObject*> particles;
	
	ofCamera cam;
	ofLight	light;
	bool drawSphere;
	
	vector<ofVec3f> disturbingObjects;
	
	ofVec3f emitter;
	

};