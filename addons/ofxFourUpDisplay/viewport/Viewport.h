/*
 *  Viewport.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */
#pragma once
#include "ofMain.h"



/** 
 * \brief The generic viewport, inherits from rectangle, to describe where in screen coords to draw it.
 */
class Viewport: public ofRectangle, public ofNode {
public:
	string name;
	Viewport(string name, float x, float y, float width, float height): ofRectangle(x, y, width, height) {
		this->name = name;
	}
	
	virtual void begin() = 0;
	virtual void end() = 0;
};
