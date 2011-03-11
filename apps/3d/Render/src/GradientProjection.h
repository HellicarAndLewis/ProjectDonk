/*
 *  GradientProjection.h
 *  Render
 *
 *  Created by Joel Lewis on 11/03/2011.
 *  Copyright 2011 Hellicar & Lewis. All rights reserved.
 *
 */
#include "ProjectedRect.h"
#include "ofMain.h"
#include "Mode.h"
class GradientProjection: public ProjectedRect {
public:
	ofColor topColor;
	ofColor bottomColor;
	
	void draw() {
		
		Donk::Mode *mode = Donk::Mode::getInstance();
		topColor.set(mode->getValue("Top BG Red"), 
				   mode->getValue("Top BG Green"), 
				   mode->getValue("Top BG Blue"));
		bottomColor.set(mode->getValue("Bottom BG Red"), 
					 mode->getValue("Bottom BG Green"), 
					 mode->getValue("Bottom BG Blue"));
		
		glBegin(GL_QUADS);
		
		ofSetColor(topColor);
		glVertex2f(0, 0);
		glVertex2f(getWidth(), 0);
		ofSetColor(bottomColor);
		glVertex2f(getWidth(), getHeight());
		glVertex2f(0, getHeight());
		glEnd();
	}
};