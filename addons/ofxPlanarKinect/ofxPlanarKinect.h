/*
 *  ofxPlanarKinect.h
 *
 *  Created by Marek Bereza on 31/01/2011.
 * 
 *  With this class you can track on a 2d plane putting the kinect parallel to the plane.
 *  Effectively it turns a surface into a multitouch input device.
 *
 */
#pragma once
#include "ofMain.h"
#include "ofxPlanarKinectWarper.h"
#include "ofxPlanarKinectGui.h"
#define SLICE_SELECTION 1
#define THRESHOLD_SETTING 0

#define TOP_LEFT_CORNER 0
#define TOP_RIGHT_CORNER 1
#define BOTTOM_RIGHT_CORNER 2
#define BOTTOM_LEFT_CORNER 3



class ofxPlanarKinect {

public:
	ofxPlanarKinect();	
	~ofxPlanarKinect();	
	
	void setup();	
	
	void update(unsigned char *pixels, float *distances);	
	
	
	// add these as drawables to your gui.
	ofxPlanarKinectGuiElement &getSliceChooser();
	ofxPlanarKinectGuiElement &getThresholdControl();
	
	
	void draw();
	void draw(float x,float y);	
	void draw(float x,float y,float w, float h);
	void mousePressed(float x, float y, int button);
	void mouseReleased(float x, float y, int button);
	void mouseDragged(float x, float y, int button);
	
	// delete this stuff
	float x, y, width, height;
	/** calibrated blobs */
	vector<ofVec2f> blobs;
	
	void saveSettings();
	void loadSettings();
	
	int deviceId;
	
	// can either be SLICE_SELECTION or THRESHOLD_SETTING
	int guiMode;
	
	// call this when you're making a blob in a particular corner
	void calibrateCorner(int whichCorner);
	
	
	void moveThreshold(float increment);
	
	void captureThreshold();
	float inset;
	
	float lpf;
	float timeFilter;
	
	bool fillHoles;
	float cropLeft, cropRight;
private:
	
	/** uncalibrated blobs */
	vector<ofVec2f> rawBlobs;
	
	ofVec2f inputQuad[4];
	ofVec2f outputQuad[4];
	
	/** machine vision */
	void preprocessSlice();
	void findBlobs();
	void calibrateBlobs();
	
	/** number of pixels in 'pixels' */
	int numPixels;	
	
	/** current frame (this gets dynamically allocated and copied to every frame */
	unsigned char *pixels;
	
	/** the start point in 'pixels' of the row of interest */
	float *slice;
	float *lastSlice;
	
	/** This is which row of pixels we slice from */
	int sliceY;

	/** This is the graph that gets drawn to the screen */
	ofVec2f *depthGraph;
	int numDepthGraphPoints;
	
	/** depth threshold at which to stop ignoring points - there's a threshold for each x-position*/
	float *threshold;
	
	/** texture for the whole camera view */
	ofTexture camImg;
	
	/** texture for just the slice in question */
	ofTexture sliceImg;
	
	/** dimensions of the kinect camera */
	float kinectWidth;
	float kinectHeight;
	
	ofxPlanarKinectWarper warper;
	
	SliceChooser sliceChooser;
	ThresholdControl thresholdControl;
	

};