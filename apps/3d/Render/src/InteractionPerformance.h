/*
 *  InteractionPerformance.h
 *  Render
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */


#pragma once
#include "BaseInteraction.h"

class InteractionPerformance : public BaseInteraction {
	
	
public:
	
	InteractionPerformance() {
		mode = MODE_PERFORMANCE;
	}
	
};