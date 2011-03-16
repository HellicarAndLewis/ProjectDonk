/*
 *  InteractionChoice.h
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "BaseInteraction.h"

class VoteBubble : public ContentBubble {

public:
	
	string optionStr;
	
	
	void draw() {
	
		ofSetColor(255, 255, 255);
		font.drawString(optionStr, 0, 0);
		ContentBubble::draw();
	
	}
};

class InteractionVote : public BaseInteraction {
	
	
public:
	
	InteractionVote() {
		mode = MODE_VOTE;
	}
	
	void setup();
	
	//--------------------------------------------------------
	void update();
	void newBubbleRecieved(Donk::BubbleData * data);
	void drawContent();
	void drawSphere(BubbleShader * shader);
	void animatedOut();
	void animatedIn();	
	
	//--------------------------------------------------------
	void			setChoiceBubble(int i, string choice);
	bool			bMadeVoteBubbles;
	VoteBubble *	voteBubbles[2];
};








