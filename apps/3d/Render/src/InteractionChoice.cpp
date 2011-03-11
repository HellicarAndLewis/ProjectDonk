/*
 *  InteractionChoice.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionChoice.h"
#include "testApp.h"

//--------------------------------------------------------
void InteractionChoice::newBubbleRecieved(Donk::BubbleData * data) { 
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
void InteractionChoice::update() {
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->update();	
		champagne(bubbles[i]->pos);

	}	
}

//--------------------------------------------------------
void InteractionChoice::drawContent() {
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->drawHighLight();
		bubbles[i]->drawTwitterData();
	}
}

//--------------------------------------------------------
void InteractionChoice::drawSphere(BubbleShader * shader) {
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->pushBubble();
		shader->begin();
		bubbles[i]->draw();
		shader->end();
		bubbles[i]->popBubble();
	}
}


//--------------------------------------------------------
void InteractionChoice::animatedOut() {
}

//--------------------------------------------------------
void InteractionChoice::animatedIn() {
	
}