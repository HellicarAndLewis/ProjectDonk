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
#include "Mode.h"

//--------------------------------------------------------
void InteractionInspiration::setup() {
	maxBubbles = 50;
}

//--------------------------------------------------------
void InteractionInspiration::newBubbleRecieved(Donk::BubbleData * data) { 

	if(bAnimateOut) return;

	
	ofVec3f center(interactiveRect.width/2, 0, 0);
	float   radius = ofRandom( 70, (float)Donk::Mode::getInstance()->getValue("Max Bubble Size") );
	
	ofVec3f startPos;
	startPos.x = (int)ofRandom(0,2) ? -100 : interactiveRect.width+100;
	startPos.y = (int)ofRandom(0,2) ? -100 : interactiveRect.height+100;		
	startPos.z = 0;
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	bubble->startRadius  = radius;
	bubble->maxRadius    = 150.f;
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

		if(nTouches == 0) {
			if(bubbles[i]->bDoubleTouched) {
				printf("Double Touched Off!\n");
			}
			bubbles[i]->bTouched	   = false;
			bubbles[i]->bDoubleTouched = false;
		}
		
		if(bubbles[i]->bDoubleTouched) {
			bubbles[i]->lerpRadius(bubbles[i]->maxRadius,0.1);
		}else{
			bubbles[i]->lerpRadius(bubbles[i]->startRadius,0.1);
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
	
	float animateOutTime = (ofGetElapsedTimeMillis()-animatedOutTimer)/1000.0;
	if(bAnimateOut && animateOutTime > MAX_ANIMATION_TIME && !bDoneAnimatingOut) {
		bDoneAnimatingOut = true;
		killallBubbles();
	}
	if(bAnimateOut && bAllOffScreen) {
		bDoneAnimatingOut = true;
		killallBubbles();
	}
	
	bool bNeedToCleanUp = false;
	for (int i=0; i<bubbles.size(); i++) {
		if(i > maxBubbles) {
			int ind = (bubbles.size()-1) - i;
			if(!bubbles[ind]->bRemoveOffScreen) {
				bubbles[ind]->bRemoveOffScreen = true;
			}
			
			if(bubbles[ind]->bRemoveOffScreen) {
				bubbles[ind]->alpha -= 10.0;
				if(bubbles[ind]->alpha <= 0.4) {
					bubbles[ind]->bRemove = true;	
					bNeedToCleanUp		  = true;
				}
			}
		}
	}
	
	if(bNeedToCleanUp) bubbles.erase(bubbles.begin(), partition(bubbles.begin(), bubbles.end(), shouldRemoveBubble));
	
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
	
	animatedOutTimer = ofGetElapsedTimeMillis();
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
		
		if(bubbles[i]==NULL) continue;
		ContentBubble * bubble = bubbles[i];
		ofVec2f p1  = touchpos;
		ofVec2f p2  = bubble->rigidBody->getPosition();
		float	dis = p1.distance(p2);
		/*
		if(dis < bubble->radius + 10.0) {
			//bubble->setRadius(150);
			//bubble->doubleTouched();
			
			if(bubble->zoomTouched){
				bubble->zoomTouched = false;
			}
			else{
				bubble->doubleTouched();
				bubble->zoomTouched = true;
			}

			
			printf("hit this bubble: %p\n", bubble);
			break;
		}*/
		if(dis < bubble->radius + 10.0) {
			//bubble->setRadius(150);
			bubble->doubleTouched();
			printf("hit this bubble: %p\n", bubble);
			break;
		}
	}
	
}


