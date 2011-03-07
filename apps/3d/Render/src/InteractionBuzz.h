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
};