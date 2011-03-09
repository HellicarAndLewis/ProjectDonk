/*
 *  BrownianObject.h
 *  emptyExample
 *
 *  Created by base on 24/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofMain.h"

class BrownianObject {
	
	
public:
	
	BrownianObject(ofVec2f initialLocation, float intialRadius, bool limitXMovement = false);
	bool isDead() { return age < 0.001; }
	void update();
	void draw();
	
	float minSize;
	ofVec2f velocity;
	ofVec2f position;
	float age, radius;
	bool xmoveLimited;
	
};