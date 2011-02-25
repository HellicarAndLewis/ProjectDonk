/**
 * BubbleProjection.h
 * Render
 *
 * Created by Marek Bereza on 24/02/2011.
 *
 */
#pragma once
#include "ofxCubeMap.h"


#include "ProjectedRect.h"

class BubbleProjection: public ProjectedRect {
public:
	BubbleProjection();
	void setup();
	void update();
	void draw();
	
	/** 
	 * Touches come in normalized to the interactive area.
	 * i.e. (0,0) is the top left, (1,1) is bottom right.
	 */
	void touchDown(float x, float y, int touchId);
	void touchMoved(float x, float y, int touchId);
	void touchUp(float x, float y, int touchId);

	/** Maps a normalized point to coordinates in the interactive area */
	ofVec2f mapToInteractiveArea(ofVec2f inPoint);
	
	/** This is the rectangle where the interaction is going to happen */
	ofRectangle &getInteractiveArea();
	
private:
	ofRectangle interactiveArea;

	/** this gets called (from touchDown) when there is a detected double touch */
	void doubleTouchGesture(int touch1Id, int touch2Id);
	
	/** This is a local copy of the current touches */
	map<int,ofVec2f> touches;
	
	/** Convenience so you don't have to keep typing it */
	map<int,ofVec2f>::iterator tIt;
	
	
	
	
	
	ofShader cubeshader;
	ofxCubeMap cubeMap;
	
	ofVec3f sphereCenter;
	ofEasyCam cam;
	
	ofImage permTex;
	ofImage glossTex;
	
	bool hasDrawnTex;
	
	ofFbo first;
	ofFbo second;
};