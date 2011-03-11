/*
 *  InteractionBuzz.h
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "BaseInteraction.h"
#include "BuzzContainerBubble.h"

#define BUZZ_TYPE_CONTAINER		0
#define BUZZ_TYPE_BUBBLE_IN		1
#define BUZZ_TYPE_BUBBLE_OUT	2
#define BUZZ_TYPE_BUBBLE_OLD	3

#define CONTAINER_RADIUS		200

class InteractionBuzz : public BaseInteraction {
	
	
public:
	
	//--------------------------------------------------------
	InteractionBuzz() {
		mode = MODE_BUZZ;
	}
	
	//--------------------------------------------------------
	void update();
	void newBubbleRecieved(Donk::BubbleData * data);
	void drawContent();
	void drawSphere(BubbleShader * shader);
	void animatedOut();
	void animatedIn();	
	
	void doubleTouched(ofVec2f touchpos);
	void killallBubbles();

	//--------------------------------------------------------
	void createContainerBubble(Donk::BubbleData * data);
	void clearOldBubbles();
	void releaseContainedBubbles(int poppedID);
	void clearContainer( int index );
	
	//--------------------------------------------------------
	void setCollisionFilter(ofxBulletRigidBody * rigidBody, int filter, int mask);
	
	//--------------------------------------------------------
	//vector<int> bubbleTypes;
	map<int,int> bubbleToContIndex;
};