/*
 *  InteractionInspiration.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionInspiration.h"


//--------------------------------------------------------
void InteractionInspiration::newBubbleRecieved(Donk::BubbleData * data) { 
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	float   radius = 80;
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->target.set(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	
	bubble->offScreenTaget.x = bubble->target.x;
	bubble->offScreenTaget.y = -300;
	
	bubbles.push_back(bubble);
};

//--------------------------------------------------------
void InteractionInspiration::update() {
	
	bool bAllOffFadedOut = true;
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(nTouches == 0) {
			
			if(bubbles[i]->bDoubleTouched) {
				printf("Double Touched Off!\n");
				bubbles[i]->setRadius(90);
			}
			
			bubbles[i]->bTouched	   = false;
			bubbles[i]->bDoubleTouched = false;
			
		}
		
		if(!bubbles[i]->bTouched) {
			
			if(bAnimateIn) {
				if(bubbles[i]->alpha <= 255) bubbles[i]->alpha += 10;
				bubbles[i]->gotoTarget();
			}
			
			else if(bAnimateOut && !bDoneAnimatingOut) {
				if(bubbles[i]->alpha > 0) {
					bubbles[i]->alpha -= 10;
					bAllOffFadedOut = false;
				}
				// bubbles[i]->goOffScreen();
			}
			
		}
		
		
		bubbles[i]->update();
		
	}	
	
	if(bAllOffFadedOut && bAnimateOut) {
		bDoneAnimatingOut = true;
		for(int i=0; i<bubbles.size(); i++) {
			bubbles[i]->rigidBody->body->setActivationState(DISABLE_SIMULATION);
		}
		
	}
	
	
	
}

//--------------------------------------------------------
void InteractionInspiration::drawContent() {
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->drawHighLight();
		bubbles[i]->drawTwitterData();
	}
}

//--------------------------------------------------------
void InteractionInspiration::drawSphere(BubbleShader * shader) {
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->pushBubble();
		shader->begin();
		bubbles[i]->draw();
		shader->end();
		bubbles[i]->popBubble();
	}
}


//--------------------------------------------------------
void InteractionInspiration::animatedOut() {
	bAnimateIn  = false;
	bAnimateOut = true;
	
	bDoneAnimatingOut = false;
	bDoneAnimatingIn  = true;
}

//--------------------------------------------------------
void InteractionInspiration::animatedIn() {
	bAnimateIn  = true;
	bAnimateOut = false;
	
	bDoneAnimatingOut = true;
	bDoneAnimatingIn  = false;
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->rigidBody->body->setActivationState(DISABLE_DEACTIVATION);
	}
}

//--------------------------------------------------------
void InteractionInspiration::doubleTouched(ofVec2f touchpos) {
	
	for(int i=0; i<bubbles.size(); i++) {
		
		ContentBubble * bubble = bubbles[i];
		ofVec2f p1  = touchpos;
		ofVec2f p2  = bubble->rigidBody->getPosition();
		float	dis = p1.distance(p2);
		
		if(dis < bubble->radius + 10.0) {
			bubble->setRadius(120);
			bubble->doubleTouched();
			printf("hit this bubble: %p\n", bubble);
			break;
		}
	}
	
}


