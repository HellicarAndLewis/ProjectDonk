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
	
	ofxBullet * bullet;
	int			mode;
	
	BaseInteraction()  {
		bullet = NULL;
		mode   = -1;
	}
	
	~BaseInteraction() {}
	
	virtual void animatedOut() {}
	virtual void animatedIn()  {}
	
	virtual void setup()  {}
	virtual void update() {};
	virtual void draw()   {};

};