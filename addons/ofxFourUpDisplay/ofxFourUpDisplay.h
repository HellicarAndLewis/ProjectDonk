/*
 *  ofxFourUpDisplay.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 05/01/2011.
 *
 *  TODO:
 *   - Make an example
 *   - get rid of Viewport/OrthoViewport when oF 007 has settled down a bit.
 *
 */
#pragma once
#include "OrthoViewport.h"
#include "ofScene3d.h"

/**
 * \brief This provides a 4-up view of a given ofScene3d.
 *  Through mouse events, it lets you manipulate the views.
 *  Left mouse button moves or rotates the view, right mouse
 *  button zooms.
 * 
 */
class ofxFourUpDisplay {
public:
	ofxFourUpDisplay(ofScene3d *scene, ofRectangle rect = ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));
	~ofxFourUpDisplay();
	
	/** Draws all 4 viewports */
	void draw(ofEventArgs &e);
	
	
	/** mouse events */
	void mouseMoved(ofMouseEventArgs &m);//float x, float y);
	void mousePressed(ofMouseEventArgs &m);//float x, float y, int button);
	void mouseDragged(ofMouseEventArgs &m);//float x, float y, int button);
	void mouseReleased(ofMouseEventArgs &m);//float x, float y, int button);
	
	
	void toggle();
	void setEnabled(bool enabled);
	bool isEnabled();
	
private:
	
	float width;
	float height;
	float x;
	float y;
	
	bool enabled;
	
	/** The model the viewport gui is looking at */
	ofScene3d *scene;
	
	/** Sets the centre of the 4-up display */
	void setCentre(float x, float y);
	
	
	/** The (4) viewports */
	vector<OrthoViewport*> viewports;
	
	/** last mouse position */
	ofPoint prevMouse;
	
	/** last viewport entered */
	OrthoViewport *prevViewport;
	
	/** if the current mouse click is moving the centre of the 4-up display. */
	bool movingCentre;
	
	
	/** true if the mouse is currently over the centre - for display purposes; */
	bool overCentre;
	
	/** the central point of the 4-up display. */
	ofPoint centre;
	
	/** Helper, says if point is inside rect */
	bool inRect(ofPoint point, ofRectangle &rect);
};



