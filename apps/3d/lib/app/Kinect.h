

#pragma once
#include "ofMain.h"
#include "ofxKinect.h"
#include "Plane.h"

// the downsample factor for drawing the vertex arrays.
#define KINECT_RESOLUTION 2

/**
 * \brief Wrapper for a kinect device and its position in the real world.
 */
class Kinect: public ofNode {
public:

	
	// give the kinect a decent name
	Kinect(string name);	
	
	// start kinect communication
	bool open();	
	
	// stop kinect communication
	void close();
	
	
	// read the kinect and preprocess
	void update();
	
	// for ofNode
	void customDraw();
	
	
	// dimensions of kinect output
	float getWidth();
	float getHeight();	
	
	
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
	
private:
	
	// the kinect object, may subclass this instead in future.
	ofxKinect kinect;
	
	// this is the plane that the kinect clips to 
	Plane plane;
	
	// whether the kinect is running 
	bool running;
	
	// for drawing vertex arrays
	ofVec3f *kinectCoords;
	ofVec3f *kinectColours;
	int numKinectCoords;	
};