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
		
		float middle = mode->getValue("Gradient Centre");
		
		ofColor middleColor = (topColor + bottomColor) / 2.f;
		
		glBegin(GL_TRIANGLE_STRIP);
		
		ofSetColor(topColor);
		glVertex2f(0, 0);
		glVertex2f(getWidth(), 0);
		
		ofSetColor(middleColor);
		glVertex2f(0, getHeight()*middle);
		glVertex2f(getWidth(), getHeight()*middle);
		
		ofSetColor(bottomColor);
		glVertex2f(0, getHeight());
		glVertex2f(getWidth(), getHeight());
		
		glEnd();
	}
};