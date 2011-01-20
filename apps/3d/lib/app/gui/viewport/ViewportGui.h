/*
 *  ViewportGui.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 05/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
#pragma once
#include "Scene.h"
#include "OrthoViewport.h"
#include "Enableable.h"

/**
 * \brief This provides a 4-up view of a given Model.
 *  Through mouse events, it lets you manipulate the views.
 *  Left mouse button moves or rotates the view, right mouse
 *  button zooms.
 * 
 */
class ViewportGui: public ofRectangle, public Enableable {
public:
	ViewportGui(Scene *scene);
	~ViewportGui();
	
	/** Draws all 4 viewports */
	void draw();
	
	
	/** mouse events */
	void mouseMoved(float x, float y);
	void mousePressed(float x, float y, int button);
	void mouseDragged(float x, float y, int button);
	void mouseReleased(float x, float y, int button);
	
	
	
	
private:
	/** The model the viewport gui is looking at */
	Scene *scene;
	
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



