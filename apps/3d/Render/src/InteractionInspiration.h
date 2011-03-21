/*
 *  InteractionInspiration.h
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "BaseInteraction.h"

class InteractionInspiration : public BaseInteraction {

	
public:
	
	InteractionInspiration() {
		mode = MODE_INSPIRATION;
		name = "inspiration";
	}
	
	int maxBubbles;
	
	//--------------------------------------------------------
	void setup();
	void update();
	void newBubbleRecieved(Donk::BubbleData * data);
	void drawContent();
	void drawSphere(BubbleShader * shader);
	void animatedOut();
	void animatedIn();	
	
	void doubleTouched(ofVec2f touchpos);
	
};