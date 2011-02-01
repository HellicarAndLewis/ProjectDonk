/*
 *  ofxBlobTracker.h
 *  ofxBlobTrackerExample
 *
 *  This is largely taken from https://code.google.com/p/tuiokinect/
 * 
 *  Created by Marek Bereza on 31/01/2011.
 *
 */
#include "ofMain.h"
#include "ofxCvKalman.h"
#pragma once
/**
 * Implement this class to listen for blobs.
 */
class ofxBlobListener {
public:
	virtual void blobEntered(ofVec3f pos, int blobId) {}
	virtual void blobMoved(ofVec3f pos, int blobId) {}
	virtual void blobExited(ofVec3f pos, int blobId) {}
	
};
#define NUM_KALMAN_POINTS 32

class ofxBlob: public ofVec3f {
public:
	int id;
	bool touched;
};

/**
 * This is the actual blob tracker.
 */
class ofxBlobTracker: public ofBaseDraws {
public:
	
	ofxBlobTracker();
	
	// input: give the blob tracker the coordinates of your blobs
	void track(vector<ofVec3f> &blobs);

	// output: add yourself as a listener
	void addListener(ofxBlobListener *listener);
	
	
	void setVerbose(bool bVerbose = true);
	// ofBaseDraws stuff
	void draw(float x,float y);
	void draw(float x,float y,float w, float h);
	float getWidth();
	float getHeight();
	
private:
	bool bVerbose;
	vector<ofxBlobListener*> listeners;
	
	enum ofxBlobEventType { ofxBlobTracker_entered, ofxBlobTracker_moved, ofxBlobTracker_exited };
	void notifyAllListeners(ofVec3f pos, int id, ofxBlobEventType type);
	
	vector<ofxBlob*> lastBlobs;
	
	ofxCvKalman *tuioPointSmoothed[NUM_KALMAN_POINTS];
	ofxBlob *getClosestBlob(ofVec3f &blob);
	void clearKalman(int id);
	ofxBlob *updateKalman(int id, ofxBlob *blob);
	
	ofxBlob *newBlob(ofVec3f coords);
	void updateBlob(int id, ofVec3f *blob);

	void untouchLastBlobs();
	int getNextAvailableBlobId();
};