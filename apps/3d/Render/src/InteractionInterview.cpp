/*
 *  InteractionInterview.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionInterview.h"

//--------------------------------------------------------
void InteractionInterview::newBubbleRecieved(Donk::BubbleData * data) { 
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	float   radius = 180;
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->setTarget(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	bubbles.push_back(bubble);
};

//--------------------------------------------------------
void InteractionInterview::update() {

	bool bAllOffScreen = true;
	for(int i=0; i<bubbles.size(); i++) {
	
		if(bAnimateOut) {
			bubbles[i]->goOffScreen();
			float disToOffScreenTarget = bubbles[i]->getPosition().distance(bubbles[i]->offScreenTaget);
			if(disToOffScreenTarget > 300) {
				bAllOffScreen = false;
			}
		}
		else {
			bubbles[i]->gotoTarget();
		}
		
		
		bubbles[i]->update();	
	}
	
	
	if(bAnimateOut && bAllOffScreen) {
		bDoneAnimatingOut = true;
		killallBubbles();
	}
}

//--------------------------------------------------------
void InteractionInterview::drawContent() {
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->drawHighLight();
		bubbles[i]->drawTwitterData();
	}
}

//--------------------------------------------------------
void InteractionInterview::drawSphere(BubbleShader * shader) {
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->pushBubble();
		shader->begin();
		bubbles[i]->draw();
		shader->end();
		bubbles[i]->popBubble();
	}
}


//--------------------------------------------------------
void InteractionInterview::animatedOut() {
	bAnimateIn  = false;
	bAnimateOut = true;
	
	bDoneAnimatingOut = false;
	bDoneAnimatingIn  = true;
	
	for(int i=0; i<bubbles.size(); i++) {
		
		bubbles[i]->bAnimateOut = true;
		bubbles[i]->offScreenTaget.x = (int)ofRandom(0,2) ? -100 : interactiveRect.width+100;
		bubbles[i]->offScreenTaget.y = (int)ofRandom(0,2) ? -100 : interactiveRect.height+100;		
		bubbles[i]->offScreenTaget.z = 0;
	}
	
}

//--------------------------------------------------------
void InteractionInterview::animatedIn() {
	bAnimateIn  = true;
	bAnimateOut = false;
	
	bDoneAnimatingOut = true;
	bDoneAnimatingIn  = false;

}



