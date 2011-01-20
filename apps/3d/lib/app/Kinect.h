

#pragma once
#include "ofMain.h"

/**
 * \brief Wrapper for a kinect device and its position in the real world.
 */
class Kinect: public ofNode {
public:
	Kinect(string name): ofNode() {
		enabled = true;
		this->name = name;
	}
	void customDraw() {
		ofSetHexColor(0x00FF00);
		ofBox(0, 0, 0, 0.2);
	}
	
	
	void update() {
		applyTransformation();
	}
	
	
	// these are for the gui
	// and get put into the 
	// matrix when applyTransformation()
	// is called, (usually in update())
	ofVec3f pos;
	ofVec3f rot;
	
	void applyTransformation() {
		resetTransform();
		setPosition(pos);
		rotate(rot.x, 1, 0, 0);
		rotate(rot.y, 0, 1, 0);
		rotate(rot.z, 0, 0, 1);
	}
	
	
	bool enabled;
	string name;
};