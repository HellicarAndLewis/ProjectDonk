/*
 *  BaseInteraction.h
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "BubbleData.h"
#include "ofxBullet.h"
#include "ContentBubble.h"
#include "BubbleData.h"
#include "BubbleShader.h"
#include "TouchedConstraint.h"

enum {
	MODE_BUZZ		 = 0,
	MODE_INSPIRATION = 1,
	MODE_INTERVIEW	 = 2,
	MODE_CHOICE		 = 3,
	MODE_PERFORMANCE = 4
};

// --------------------------------------------
class BaseInteraction {

public:
	
	vector <ContentBubble*>		bubbles;
	ofxBullet * bullet;
	int			mode;
	ofRectangle	interactiveRect;
	bool		bDoneAnimatingOut, bDoneAnimatingIn;
	bool		bAnimateOut, bAnimateIn;
	int			nTouches;
	
	BaseInteraction()  {
		bullet			  = NULL;
		mode			  = -1;
		nTouches		  = 0;
		bDoneAnimatingOut = false;
		bDoneAnimatingIn  = false;
		bAnimateOut		  = false;
		bAnimateIn		  = false;
	}
	
	~BaseInteraction() {}
	
	virtual void setup()  {}
	virtual void draw()   {};
	virtual void doubleTouched(ofVec2f touchpos) {}
	
	// required!
	virtual void update()								    = 0;
	virtual void drawContent()								= 0;
	virtual void drawSphere(BubbleShader * shader)			= 0;
	virtual void newBubbleRecieved(Donk::BubbleData * data) = 0;
	virtual void animatedOut()								= 0;
	virtual void animatedIn()								= 0;
	
};