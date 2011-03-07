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
	
	vector <ContentBubble*> bubbles;

	InteractionInspiration() {
		mode = MODE_INSPIRATION;
	}
	
};