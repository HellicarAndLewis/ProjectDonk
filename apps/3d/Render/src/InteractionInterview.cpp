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
	
	if(bAnimateOut) return;

	float   radius = ofRandom(160,220);//180;
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height+radius, ofRandom(-100, 100));
	
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	bubble->startRadius = radius;
	bubble->maxRadius = 240;
	
	bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->setTarget(center.x + ofRandom(-300, 300), startPos.y-radius, 0 );//ofRandom(500, interactiveRect.height-300), 0);
	
	bubble->setContentSideUp();
	
	bubbles.push_back(bubble);
};

//--------------------------------------------------------
void InteractionInterview::update() {

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
			bubbles[i]->lerpRadius(bubbles[i]->maxRadius, 0.1);
		}else{
			bubbles[i]->lerpRadius(bubbles[i]->startRadius, 0.1);
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
			if(!bubbles[i]->bTouched && !bubbles[i]->bDoubleTouched) {
				bubbles[i]->loopMe(interactiveRect.width, interactiveRect.height);
			}
			
		}
		
		
		bubbles[i]->update();	
		champagne(bubbles[i]->pos);

	}
	
	// we are now animating out lets also make sure that
	// for some reason it takes to long we just kill it all!
	if(bAnimateOut) {
		float time = (ofGetElapsedTimeMillis()-animatedOutTimer) / 1000.0;
		if(time > MAX_ANIMATION_TIME || bAllOffScreen && !bDoneAnimatingOut) {
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

//--------------------------------------------------------
void InteractionInterview::doubleTouched(ofVec2f touchpos) {
	
	for(int i=0; i<bubbles.size(); i++) {
		if(bubbles[i]==NULL) continue;
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

