/*
 *  InteractionInterview.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionInterview.h"
#include "testApp.h"

//--------------------------------------------------------
void InteractionInterview::newBubbleRecieved(Donk::BubbleData * data) { 
	
	float   radius = 180;
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height+radius, ofRandom(-100, 100));
	
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->setTarget(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	
	bubble->setContentSideUp();
	
	bubbles.push_back(bubble);
};

//--------------------------------------------------------
void InteractionInterview::update() {

	bool bAllOffScreen = true;
	for(int i=0; i<bubbles.size(); i++) {
	
		if(bubbles[i]->bDoubleTouched) {
			bubbles[i]->lerpRadius(240,0.1);
		}else{
			bubbles[i]->lerpRadius(180,0.1);
		}
		
		if(bAnimateOut) {
			bubbles[i]->goOffScreen();
			float disToOffScreenTarget = bubbles[i]->getPosition().distance(bubbles[i]->offScreenTaget);
			if(disToOffScreenTarget > 300) {
				bAllOffScreen = false;
			}
		}
		else {
			bubbles[i]->gotoTarget();
			bubbles[i]->loopMe(interactiveRect.width,interactiveRect.height);
		}
		
		
		bubbles[i]->update();	
		
		champagne(bubbles[i]->pos);

	}
	
	
	if(bAnimateOut) {
		float time = (ofGetElapsedTimeMillis()-animatedOutTimer) / 1000.0;
		if(time > 3 || bAllOffScreen && !bDoneAnimatingOut) {
			bDoneAnimatingOut = true;
			killallBubbles();
		}
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
		bubbles[i]->offScreenTaget.x = (int)ofRandom(0,2) ? -500 : interactiveRect.width+500;
		bubbles[i]->offScreenTaget.y = (int)ofRandom(0,2) ? -500 : interactiveRect.height+500;		
		bubbles[i]->offScreenTaget.z = 0;
	}
	
	animatedOutTimer = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------
void InteractionInterview::animatedIn() {
	bAnimateIn  = true;
	bAnimateOut = false;
	
	bDoneAnimatingOut = true;
	bDoneAnimatingIn  = false;

}



