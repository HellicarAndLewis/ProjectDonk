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
#include "AudioData.h"
#include "ofxDirList.h"

class InteractionPerformance : public BaseInteraction {
	
	
public:
	
	Donk::AudioData *	audio;
	float *				freq; 
	int					nBands;
	vector	<ofImage>	images;
	float				lineAlpha, lineAlphaDes;
	
	//--------------------------------------------------------
	InteractionPerformance() {
		mode = MODE_PERFORMANCE;
		name = "performance";
	}
	
	//--------------------------------------------------------
	void setup();
	void update();
	
	void newBubbleRecieved(Donk::BubbleData * data);
	void drawContent();
	void drawSphere(BubbleShader * shader);
	
	void animatedOut();
	void animatedIn();
	void putToRest();
};