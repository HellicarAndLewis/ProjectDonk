/*
 *  BrownianObject.h
 *  Simple object w/Brownian-ish motion
 */

#include "ofMain.h"

class BrownianObject {
	
	
public:
	
	BrownianObject(ofVec2f initialLocation, float intialRadius, bool limitXMovement = false);
	bool isDead() { return age < 0.01; }
	void update();
	void draw();
	
	float minSize;
	ofVec2f velocity;
	ofVec2f position;
	float age, radius;
	bool xmoveLimited;
	
};