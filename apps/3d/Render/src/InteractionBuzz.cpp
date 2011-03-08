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
	
	// only for testing!!
	//if(bubbles.size() > 3) return;
	
	createContainerBubble(data);
	createContainerBubble(data);
	
/*	ofVec3f center(interactiveRect.width/2, 0, 0);
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
	bubbleTypes.push_back(BUZZ_TYPE_BUBBLE);*/

};

//--------------------------------------------------------
void InteractionBuzz::update() {
	
	bool bAllOffFadedOut = true;
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(nTouches == 0) {
			bubbles[i]->bTouched = false;			
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
void InteractionBuzz::drawContent() {
	
	for(int i=0; i<bubbles.size(); i++) {
		if(bubbles[i]->bAlive){
			bubbles[i]->drawHighLight();
			bubbles[i]->drawTwitterData();
		}
	}
	
}

//--------------------------------------------------------
void InteractionBuzz::drawSphere(BubbleShader * shader) {
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(bubbles[i]->bAlive){
			bubbles[i]->pushBubble();
			shader->begin();
			bubbles[i]->draw();
			shader->end();
			bubbles[i]->popBubble();
		}
	}
	
	
}


//--------------------------------------------------------
void InteractionBuzz::animatedOut() {
	bAnimateIn  = false;
	bAnimateOut = true;
	
	bDoneAnimatingOut = false;
	bDoneAnimatingIn  = true;
}

//--------------------------------------------------------
void InteractionBuzz::animatedIn() {
	
	bAnimateIn  = true;
	bAnimateOut = false;
	
	bDoneAnimatingOut = true;
	bDoneAnimatingIn  = false;
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->rigidBody->body->setActivationState(DISABLE_DEACTIVATION);
	}
	
	cout << "ANIMATE IN" << endl;
}


void InteractionBuzz::createContainerBubble(Donk::BubbleData * data){
	
	cout << "Buzz: adding new container bubble" << endl;
	
	BuzzContainerBubble * container = new BuzzContainerBubble();
	
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	float radius = 200;
	
	container->data	  = data;
	container->radius = radius;
	container->rigidBody = bullet->createSphere(startPos, radius, 1);
	container->createContainerBubble(bullet);
	container->target.set(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	container->offScreenTaget.x = container->target.x;
	container->offScreenTaget.y = -300;
	
	bubbles.push_back(container);
	bubbleTypes.push_back(BUZZ_TYPE_CONTAINER);
}

void InteractionBuzz::doubleTouched(ofVec2f touchpos)
{
	for(int i=0; i<bubbles.size(); i++){
		
		if(bubbleTypes[i] == BUZZ_TYPE_CONTAINER && ((BuzzContainerBubble*)bubbles[i])->bTouched)
		{
			((BuzzContainerBubble*)bubbles[i])->pop();
		}
	}
}

