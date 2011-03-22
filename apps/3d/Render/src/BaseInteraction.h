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

#define MAX_ANIMATION_TIME 3.0 // seconds

enum {
	MODE_BUZZ		 = 0,
	MODE_INSPIRATION = 1,
	MODE_INTERVIEW	 = 2,
	MODE_VOTE		 = 3,
	MODE_PERFORMANCE = 4
};

static bool shouldRemoveBubble(ContentBubble * b) {

	bool bRemove = false;
	if(b != NULL) {
		bRemove = b->bRemove;
		if(bRemove) {
			b->destroy();
			delete b;
			b = NULL;
		}
	}
	return bRemove;
	
}

// --------------------------------------------
class BaseInteraction {

public:
	
	vector <ContentBubble*>		bubbles;
	ofxBullet * bullet;
	int			mode;
	ofRectangle	interactiveRect;
	bool		bDoneAnimatingOut, bDoneAnimatingIn;
	bool		bAnimateOut, bAnimateIn;
	bool		bActive;
	int			nTouches;
	string		name;	// for debug printing
	float		animatedOutTimer;
	
	BaseInteraction()  {
		animatedOutTimer  = 0;
		bullet			  = NULL;
		mode			  = -1;
		nTouches		  = 0;
		bDoneAnimatingOut = false;
		bDoneAnimatingIn  = false;
		bAnimateOut		  = false;
		bAnimateIn		  = false;
		bActive			  = false;
	}
	
	~BaseInteraction() {}
	
	virtual void setup()  {}
	virtual void draw()   {}
	virtual void doubleTouched(ofVec2f touchpos) {}
	
	virtual void putToRest() {} 
	
	virtual void killallBubbles() {
		
		for (int i=0; i<bubbles.size(); i++) {
			bubbles[i]->rigidBody->destroy();
			delete bubbles[i];
			bubbles[i] = NULL;
		}
		bubbles.clear();
		
	}
	
	// required!
	virtual void update()								    = 0;
	virtual void drawContent()								= 0;
	virtual void drawSphere(BubbleShader * shader)			= 0;
	virtual void newBubbleRecieved(Donk::BubbleData * data) = 0;
	virtual void animatedOut()								= 0;
	virtual void animatedIn()								= 0;
	void champagne(ofVec3f pos);
	
};