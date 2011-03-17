/*
 *  InteractionInspiration.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionInspiration.h"
#include "testApp.h"


//--------------------------------------------------------
void InteractionInspiration::newBubbleRecieved(Donk::BubbleData * data) { 

	ofVec3f center(interactiveRect.width/2, 0, 0);
	float   radius = 80;
	
	ofVec3f startPos;
	startPos.x = (int)ofRandom(0,2) ? -100 : interactiveRect.width+100;
	startPos.y = (int)ofRandom(0,2) ? -100 : interactiveRect.height+100;		
	startPos.z = 0;
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->setTarget(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	
	bubble->setContentSideUp();
	
	bubble->offScreenTaget.x = bubble->target.x;
	bubble->offScreenTaget.y = -300;
	
	bubbles.push_back(bubble);
	
	
	printf("--- new bubble in inpiration ---\n");
};

//--------------------------------------------------------
void InteractionInspiration::update() {
	
	bool bAllOffScreen = true;
	
	for(int i=0; i<bubbles.size(); i++) {

		
		if(bubbles[i]->bDoubleTouched) {
			bubbles[i]->lerpRadius(150,0.1);
		}else{
			bubbles[i]->lerpRadius(90,0.1);
		}
		
		if(nTouches == 0) {
			
			if(bubbles[i]->bDoubleTouched) {
				printf("Double Touched Off!\n");
				//bubbles[i]->setRadius(90);
			}
			
			bubbles[i]->bTouched	   = false;
			bubbles[i]->bDoubleTouched = false;
			
		}
		
		
		if(bAnimateIn) {
			bubbles[i]->gotoTarget();
			bubbles[i]->bobMe();
		}
			
		else if(bAnimateOut && !bDoneAnimatingOut) {
			bubbles[i]->goOffScreen();
			
			float disToOffScreenTarget = bubbles[i]->getPosition().distance(bubbles[i]->offScreenTaget);
			if(disToOffScreenTarget > 300) {
				bAllOffScreen = false;
			}
		}
		
		
		bubbles[i]->update();
		
		
		champagne(bubbles[i]->pos);
		
	}	
	
	float animateOutTime = (ofGetElapsedTimeMillis()-timeAnimateOut)/1000.0;
	if(bAnimateOut && animateOutTime > 4.0 && !bDoneAnimatingOut) {
		bDoneAnimatingOut = true;
		killallBubbles();
	}
	if(bAnimateOut && bAllOffScreen) {
		bDoneAnimatingOut = true;
		killallBubbles();
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
	
	for(int i=0; i<bubbles.size(); i++) {
	
		bubbles[i]->bAnimateOut = true;
		bubbles[i]->offScreenTaget.x = (int)ofRandom(0,2) ? -100 : interactiveRect.width+100;
		bubbles[i]->offScreenTaget.y = (int)ofRandom(0,2) ? -100 : interactiveRect.height+100;		
		bubbles[i]->offScreenTaget.z = 0;
	}
	
	timeAnimateOut = ofGetElapsedTimeMillis();
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
			//bubble->setRadius(150);
			bubble->doubleTouched();
			printf("hit this bubble: %p\n", bubble);
			break;
		}
	}
	
}


