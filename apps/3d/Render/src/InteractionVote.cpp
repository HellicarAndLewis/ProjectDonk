/*
 *  InteractionVote.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionVote.h"
#include "testApp.h"

void InteractionVote::setup() {
	voteBubbles[0]   = NULL;
	voteBubbles[1]   = NULL;
	bMadeVoteBubbles = false;
	
	setChoiceBubble(0, "Option 1");
	setChoiceBubble(1, "Option 2");
	
	printf("Vote bubbles are made!\n");

}

//--------------------------------------------------------
void InteractionVote::setChoiceBubble(int i, string choice) {
	
	float   radius = 100;
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	ofVec3f target(i==0?center.x-300:center.x+500, interactiveRect.height/2, 0);
	voteBubbles[i] = new VoteBubble();
	
	voteBubbles[i]->data	  = NULL;
	voteBubbles[i]->radius    = radius;
	voteBubbles[i]->rigidBody = bullet->createSphere(startPos, radius, 1);
	voteBubbles[i]->createContentBubble();
	voteBubbles[i]->setTarget(target.x, target.y, target.z);
	
	voteBubbles[i]->optionStr = choice;
	
	if(voteBubbles[0] != NULL && voteBubbles[1] != NULL) {
		bMadeVoteBubbles = true;	
	}
	
}

//--------------------------------------------------------
void InteractionVote::newBubbleRecieved(Donk::BubbleData * data) { 
	
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
void InteractionVote::update() {
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->update();	
		champagne(bubbles[i]->pos);
	}	
	
	
	if(bMadeVoteBubbles) {
		for (int i=0; i<2; i++) {
			
			
			voteBubbles[i]->gotoTarget();
			voteBubbles[i]->bobMe();
			
			voteBubbles[i]->update();
		}
		
	}
	
	
}

//--------------------------------------------------------
void InteractionVote::drawContent() {
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->drawHighLight();
		bubbles[i]->drawTwitterData();
	}
	
	
}

//--------------------------------------------------------
void InteractionVote::drawSphere(BubbleShader * shader) {
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->pushBubble();
		shader->begin();
		bubbles[i]->draw();
		shader->end();
		bubbles[i]->popBubble();
	}
	
	
	for (int i=0; i<2; i++) {
		voteBubbles[i]->pushBubble();
		shader->begin();
		voteBubbles[i]->draw();
		shader->end();
		voteBubbles[i]->popBubble();
	}
}


//--------------------------------------------------------
void InteractionVote::animatedOut() {
}

//--------------------------------------------------------
void InteractionVote::animatedIn() {
	
}