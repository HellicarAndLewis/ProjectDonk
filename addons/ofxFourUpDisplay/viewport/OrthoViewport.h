/*
 *  OrthoViewport.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */
#pragma once
#include "Viewport.h"


enum ViewportType {
	Viewport_FRONT,
	Viewport_TOP,
	Viewport_LEFT,
	Viewport_ROTATABLE,
	Viewport_BACK,
	Viewport_BOTTOM,
	Viewport_RIGHT,
	Viewport_NUM_VIEWPORT_TYPES
};


/**
 * \brief Basic orthographic viewport like the ones you see in 3d programs
 */
class OrthoViewport: public Viewport {

public:
	
	/**
	 * Specify a viewport type to set where you want to look.
	 */
	OrthoViewport(float x, float y, float width, float height, ViewportType type = Viewport_FRONT);
	
	/**
	 * Receive mouse dragging events - supply it with how 
	 * far the mouse has moved and with which button.
	 */
	void mouse(ofPoint delta, int button);
	
	/**
	 * This returns the name of the kind of view it is 
	 * e.g. Front, Left, Top, etc.
	 */
	string getName();
	
	void begin();
	void end();
private:
	/** How far to translate the view */
	ofPoint translation;
	
	/** 
	 * If we're in 3/4 view, this is the amount of rotation in 
	 * x and y axes - it starts off as 45 degrees.
	 */
	ofPoint rotation;
	
	/** What type of viewport */
	ViewportType type;
	
	/** The zoom of the viewport */
	float scale;
	
};