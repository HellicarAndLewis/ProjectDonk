

#pragma once
#include "ofMain.h"
#include "ofxKinect.h"
#include "Plane.h"
#include "constants.h"
#include "ofxOpenCv.h"
#include "BlobTracker.h"

// the downsample factor for drawing the vertex arrays.
#define KINECT_RESOLUTION 2

/**
 * \brief Wrapper for a kinect device and its position in the real world.
 *
 * The current state of this is a bit messy - work in progress - needs
 * to be pulled out into several classes.
 *
 * First step would be to get all the computer vision out! - put it in the vision/ dir
 */
class Kinect: public ofNode {
public:

	
	/** give the kinect a decent name */
	Kinect(string name);	
	
	/** start kinect communication */
	bool open();	
	
	/** stop kinect communication */
	void close();
	
	
	/** read the kinect and preprocess */
	void update();
	
	/** for ofNode */
	void customDraw();
	
	
	void addListener(BlobListener *listener);
	
	
	/** dimensions of kinect output */
	float getWidth();
	float getHeight();	
	
	/** So you can access the raw kinect object */
	ofxKinect *getKinect();
	
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
	
	// this is for calibration
	float nearClip;
	float farClip;
	
	
	float minBlobArea;
	
	bool flipX;
	bool flipY;
	
	// draws the image for calibrating
	void drawCalibration(ofRectangle rect = ofRectangle(0, 0, WIDTH, HEIGHT));
	

private:
	
	BlobTracker blobTracker;
	
	unsigned char *calibrationImage;
	
	ofxCvGrayscaleImage thresholded;
	ofxCvContourFinder  contourFinder;
	
	
	/** the kinect object, may subclass this instead in future. */
	ofxKinect kinect;
	
	/** this is the plane that the kinect clips to */
	Plane plane;
	
	/** whether the kinect is running */
	bool running;
	
	
	// vertex arrays are only for debug at present.
	
	// for drawing vertex arrays
	ofVec3f *kinectCoords;
	ofVec3f *kinectColours;
	int numKinectCoords;	
	
	// this is a flag that ensures 
	// vertex arrays are only constructed
	// once per frame
	bool hasCreatedVertexArrays;
	void createVertexArrays();
};