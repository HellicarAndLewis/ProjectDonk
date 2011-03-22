/*
 *  BubbleTouch.h
 *  Render
 *
 *  Created by Todd Vanderlin on 3/10/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxBullet.h"
#include "BubbleData.h"
#include "ofxCubeMap.h"

//--------------------------------------------------------------
class BubbleTouch {
	
public:
	
	ofVec2f pos;
	ofVec2f	touchPos;
	int		id;
	float   touchAlpha, touchAlphaDes;
	ofColor	color, colorDes;
	float	radius, radiusDes;
	bool	bDraw, bRemove, bChildOfGesture;
	BubbleTouch * secondTouch;
	
	BubbleTouch() {
		bChildOfGesture = false;
		bRemove			= false;
		secondTouch	    = NULL;
		bDraw			= true;
		id			    = -1;
		touchAlpha      = 0;
		touchAlphaDes   = 255.0;
		radius		    = 0;
		radiusDes	    = 40.0;
		
		colorDes.set(255, 255, 255);
		color.set(0, 0, 0);
	}
	
	void update() {
		if(!bDraw) return;
		
		
		// the of color class is weird clamp crap! (ill do it myself?)
		color.r		 += (colorDes.r-color.r) * 0.2;
		color.g		 += (colorDes.g-color.g) * 0.2;
		color.b		 += (colorDes.b-color.b) * 0.2;
		
		touchAlpha   += (touchAlphaDes-touchAlpha) * 0.1;
		radius       += (radiusDes-radius) * 0.1;
	}
	
	void enableGesture() {
		bDraw = true;
		touchAlphaDes = 255;
		colorDes.g = 0;
		colorDes.b = 0;
		printf("gesture enabled\n");
	}
	void disableGesture() {
		bDraw = true;
		colorDes.g = 255;
		colorDes.b = 255;
		touchAlphaDes = 255;
	}
	
	void setPosition(ofVec2f &p) {
		if(secondTouch != NULL) {
			touchPos = (p+secondTouch->getPosition())/2;
		} else touchPos = p;
	}
	
	ofVec2f getPosition() {
		return touchPos;		
	}
	
	void drawTouch(ofVec2f &p) {
		if(!bDraw) return;
		ofEnableAlphaBlending();
		glPushMatrix();
		glTranslatef(p.x, p.y, 0);
		//draw circular hailo
		glBegin(GL_TRIANGLE_STRIP);
		int steps = 30;
		float inc = TWO_PI/steps;
		for(int i=0; i<steps+1; i++) { 
			float x = cos(i*inc);
			float y = sin(i*inc);
			glColor4f(color.r/255.0, color.g/255.0, color.b/255.0, touchAlpha/255.0);
			glVertex2f(0, 0);
			glColor4f(color.r/255.0, color.g/255.0, color.b/255.0, 0);
			glVertex2f(x*radius*2, y*radius*2);
		}
		glEnd();
		
		
		glPopMatrix();
		
		
	}
	
};