/*
 *  InteractionBuzz.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionBuzz.h"


//--------------------------------------------------------
void InteractionBuzz::newBubbleRecieved(Donk::BubbleData * data) { 
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	float   radius = 80;
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->target.set(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	bubbles.push_back(bubble);
};

//--------------------------------------------------------
void InteractionBuzz::update() {
	
	
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(nTouches == 0) {
			bubbles[i]->bTouched = false;			
		}
		
		if(!bubbles[i]->bTouched) {
			if(bAnimateIn) {
				bubbles[i]->gotoTarget();
			}
			else if(bAnimateOut) {
				bubbles[i]->goOffScreen();
			}
		}
		
		
		bubbles[i]->update();

	}	
}

//--------------------------------------------------------
void InteractionBuzz::drawContent() {
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->drawHighLight();
		bubbles[i]->drawTwitterData();
	}
}

//--------------------------------------------------------
void InteractionBuzz::drawSphere(BubbleShader * shader) {
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->pushBubble();
		shader->begin();
		bubbles[i]->draw();
		shader->end();
		bubbles[i]->popBubble();
	}
}


//--------------------------------------------------------
void InteractionBuzz::animatedOut() {
	bAnimateIn  = false;
	bAnimateOut = true;
	
	bAnimationDone = false;
}

//--------------------------------------------------------
void InteractionBuzz::animatedIn() {
	bAnimateIn  = true;
	bAnimateOut = false;
	
	bAnimationDone = false;
}


