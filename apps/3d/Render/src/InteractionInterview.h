/*
 *  InteractionInterview.h
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "BaseInteraction.h"

class InteractionInterview : public BaseInteraction {
	
	
public:
	
	InteractionInterview() {
		mode = MODE_INTERVIEW;
		name = "interview";
	}
	
	//--------------------------------------------------------
	void update();
	void newBubbleRecieved(Donk::BubbleData * data);
	void drawContent();
	void drawSphere(BubbleShader * shader);
	void animatedOut();
	void animatedIn();	
	void doubleTouched(ofVec2f touchpos);

};