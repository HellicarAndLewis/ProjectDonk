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
#include "Mode.h"

#include "Ribbon.h"

class InteractionPerformance : public BaseInteraction {
	
	
public:
	
	Donk::AudioData *	audio;
	float *				freq; 
	int					nBands;
	vector	<ofImage>	images;
	float				lineAlpha, lineAlphaDes;
	
	ofVec3f				flockPos;
	float				flockTime;
	
	//--------------------------------------------------------
	InteractionPerformance() {
		mode = MODE_PERFORMANCE;
		name = "performance";
	}
	
	//--------------------------------------------------------
	void setup();
	void update();
	
	void addBubbles();
	void newBubbleRecieved(Donk::BubbleData * data);
	void drawContent();
	void drawSphere(BubbleShader * shader);
	
	void animatedOut();
	void animatedIn();
	void putToRest();
    
    
    //JG ribbon balognie 
    void addRibbonForIndex(int emitterBubble);
    void updateRibbons();
    void drawRibbons();
    //JG added ribbons for flare and bg effect
    vector<Ribbon*> ribbons;
    //JG we probably want to augment the bubble datastore instead of keeping parallel arrays but whatevs
    int maxRibbonsPerChannel;
    ofColor* colors; //assign a color to each band
    int* ribbonCounts; //make sure we can max out to control framerates
    float* lastFreq; //pops ribbons on dramatic positive changes in amplitude
};