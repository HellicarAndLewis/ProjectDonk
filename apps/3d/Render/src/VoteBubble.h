/*
 *  VoteBubble.h
 *  Render
 *
 *  Created by Todd Vanderlin on 3/15/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "BaseInteraction.h"

class VoteBubble : public ContentBubble {
	
public:
	
	string optionStr;
	
	void setOptionString(string str);
	void drawInsideContent();
};