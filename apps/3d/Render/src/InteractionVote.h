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
#include "VoteBubble.h"


class InteractionVote : public BaseInteraction {
	
	
public:
	
	InteractionVote() {
		mode = MODE_VOTE;
		name = "vote";
	}
	
	void setup();
	
	//--------------------------------------------------------
	void update();
	void newBubbleRecieved(Donk::BubbleData * data);
	ContentBubble * addBubbleToVote(int voteID);
	
	void drawContent();
	void drawSphere(BubbleShader * shader);
	void animatedOut();
	void animatedIn();	
	
	//--------------------------------------------------------
	void			setVoteBubble(int i, string choice);
	void			setVoteCount(int totalA, int totalB);
	
	bool			bMadeVoteBubbles;
	VoteBubble *	voteBubbles[2];
	int				voteIds[100];
	
	// animation
	int				pctA, pctB;
};








