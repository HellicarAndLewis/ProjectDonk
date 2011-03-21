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
#include "Mode.h"

#define BUZZ_TYPE_CONTAINER		0
#define BUZZ_TYPE_BUBBLE_IN		1
#define BUZZ_TYPE_BUBBLE_OUT	2
#define BUZZ_TYPE_BUBBLE_OLD	3
#define BUZZ_TYPE_POPPED		4

#define CONTAINER_RADIUS		200


class InteractionBuzz : public BaseInteraction {
	
	
public:
	
	//--------------------------------------------------------
	InteractionBuzz() {
		mode = MODE_BUZZ;
		name = "buzz";
	}
	
	//--------------------------------------------------------
	void setup();
	void update();
	void newBubbleRecieved(Donk::BubbleData * data);
	void drawContent();
	void drawSphere(BubbleShader * shader);
	void animatedOut();
	void animatedIn();	
	
	void doubleTouched(ofVec2f touchpos);
	void killallBubbles();

	//--------------------------------------------------------
	void createMomAndChildBubbles();
	void createMomBubble(string group);
	void createChildBubble(int momID, Donk::BubbleData * data, float radius);
	
	void clearOutBubbles();
	void releaseInBubbles(int poppedID);
	void clearContainer( int index );
	
	//--------------------------------------------------------
	void setCollisionFilter(ofxBulletRigidBody * rigidBody, int filter, int mask);
	
	//--------------------------------------------------------
	map<int,int> bubbleToContIndex;
	int		momNowID;
	
	//--------------------------------------------------------
	vector<Donk::BubbleData *> polledData;
	string	momPollGroup;
	int startPoll;
	
	//--------------------------------------------------------
	int lastPoppedId;
	
	//--------------------------------------------------------
	// to debug buzz sticking issue
	void printBuzzBubblesStatus();
	
};