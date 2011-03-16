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
		
}

//--------------------------------------------------------
void InteractionVote::setChoiceBubble(int i, string choice) {
	
	float   radius = 180;
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	ofVec3f target(i==0?center.x-300:center.x+500, interactiveRect.height/2, 0);
	voteBubbles[i] = new VoteBubble();
	
	voteBubbles[i]->data	  = NULL;
	voteBubbles[i]->radius    = radius;
	voteBubbles[i]->rigidBody = bullet->createSphere(startPos, radius, 1);
	voteBubbles[i]->createContentBubble();
	voteBubbles[i]->setTarget(target.x, target.y, target.z);
	voteBubbles[i]->setOptionString(choice);
	
	if(voteBubbles[0] != NULL && voteBubbles[1] != NULL) {
		bMadeVoteBubbles = true;	
	}
	
}

//--------------------------------------------------------
void InteractionVote::addVoteToBubble(int voteID, int amt, Donk::BubbleData * data) {
	
	for (int i=0; i<amt; i++) {
		
		ofVec3f center(interactiveRect.width/2, 0, 0);
		
		ofVec3f startPos;
		startPos.x = (int)ofRandom(0,2) ? -ofRandom(-300, -200) : interactiveRect.width+ofRandom(-300, -200);
		startPos.y = (int)ofRandom(0,2) ? -ofRandom(-300, -200) : interactiveRect.height+ofRandom(-300, -200);		
		startPos.z = ofRandom(-300, 300);
		
		float   radius = 40;
		
		ContentBubble * bubble = new ContentBubble();
		
		bubble->voteBubbleID = voteID;
		bubble->data		 = data;
		bubble->radius		 = radius;
		bubble->rigidBody	 = bullet->createSphere(startPos, radius, 1);
		bubble->createContentBubble();
		
		bubble->setTarget( voteBubbles[voteID]->getPosition() );
		bubbles.push_back(bubble);
	
		voteBubbles[voteID]->totalVotes ++;
	}
}

//--------------------------------------------------------
void InteractionVote::newBubbleRecieved(Donk::BubbleData * data) { 
	
	int questionID = (int)ofRandom(0, 2);
	int totalA     = ofRandom(2, 40);
	int totalB     = ofRandom(2, 40);
	
	if(questionID == 0) {
		if(voteBubbles[0] == NULL) setChoiceBubble(0, "Option A");
		if(voteBubbles[0]->totalVotes < totalA) {
			int amt = totalA - voteBubbles[0]->totalVotes;
			printf("need to make: %i\n", amt);
			addVoteToBubble(0, amt, data);
		}		
	}
	
	if(questionID == 1) {
		if(voteBubbles[1] == NULL) setChoiceBubble(1, "Option B");
		if(voteBubbles[1]->totalVotes < totalB) {
			int amt = totalB - voteBubbles[1]->totalVotes;
			printf("need to make: %i\n", amt);
			addVoteToBubble(1, amt, data);
		}
	}
	
	
};

//--------------------------------------------------------
void InteractionVote::update() {
	
	for(int i=0; i<bubbles.size(); i++) {
		
		int voteID   = bubbles[i]->voteBubbleID;
		ofVec3f  pos = voteBubbles[ voteID ]->getPosition();
		
		bubbles[i]->setTarget( pos );
		//bubbles[i]->addAtrractionForce(pos, 100.0);
		
		bubbles[i]->rigidBody->body->setDamping(0.999, 0.999); // <-- add some crazy damping
		
		bubbles[i]->gotoTarget(1.0);
		bubbles[i]->update();	
		champagne(bubbles[i]->pos);
		
		
	}	
	
	
	for (int i=0; i<2; i++) {
		if(!voteBubbles[i]) continue;
		
		voteBubbles[i]->rigidBody->body->setDamping(0.999, 0.999); // <-- add some crazy damping

		voteBubbles[i]->gotoTarget(50.0);
		voteBubbles[i]->bobMe();
		voteBubbles[i]->update();
		
	}
		
	
	
}

//--------------------------------------------------------
void InteractionVote::drawContent() {
	
	for(int i=0; i<2; i++) {
		if(voteBubbles[i]) voteBubbles[i]->drawInsideContent();
	}
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->drawHighLight();
		bubbles[i]->drawTwitterData();
	}
	
	
	
}

//--------------------------------------------------------
void InteractionVote::drawSphere(BubbleShader * shader) {
	
	
	
	for (int i=0; i<2; i++) {
		if(!voteBubbles[i]) continue;
		voteBubbles[i]->pushBubble();
		shader->begin();
		voteBubbles[i]->draw();
		shader->end();
		voteBubbles[i]->popBubble();
	}
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->pushBubble();
		shader->begin();
		bubbles[i]->draw();
		shader->end();
		bubbles[i]->popBubble();
	}
	
}


//--------------------------------------------------------
void InteractionVote::animatedOut() {
}

//--------------------------------------------------------
void InteractionVote::animatedIn() {
	
}