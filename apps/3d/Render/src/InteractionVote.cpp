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
#include "InteractionPerformance.h"
#include "QuestionData.h"

void InteractionVote::setup() {
	voteBubbles[0]   = NULL;
	voteBubbles[1]   = NULL;
	bMadeVoteBubbles = false;
	
	pctA			  = 0;
	pctB			  = 0;

	
	for (int i=0; i<100; i++) {
		voteIds[i] = 0;
	}
}

//--------------------------------------------------------
void InteractionVote::setVoteBubble(int i, string choice) {
	
	float   radius = 190;
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	ofVec3f target(i==0?center.x-300:center.x+500, interactiveRect.height/2, 0);
	
	if(voteBubbles[i] == NULL) {
		voteBubbles[i] = new VoteBubble();
		
		voteBubbles[i]->data	  = NULL;
		voteBubbles[i]->radius    = radius;
		voteBubbles[i]->startRadius = radius;
		voteBubbles[i]->maxRadius = radius;
		voteBubbles[i]->rigidBody = bullet->createSphere(startPos, radius, 1);
		voteBubbles[i]->createContentBubble();
		voteBubbles[i]->setTarget(target.x, target.y, target.z);
		voteBubbles[i]->setOptionString(choice);
	}
	
	voteBubbles[i]->setOptionString(choice);
	
	if(voteBubbles[0] != NULL && voteBubbles[1] != NULL) {
		bMadeVoteBubbles = true;	
	}
	
}

//--------------------------------------------------------
ContentBubble * InteractionVote::addBubbleToVote(int voteID) {
	
	float   radius = ofRandom(10, 40);
	ofVec3f center(interactiveRect.width/2, 0, 0);
	
	ofVec3f startPos;
	startPos.x = (int)ofRandom(0,2) ? -ofRandom(-300, -200) : interactiveRect.width+ofRandom(-300, -200);
	startPos.y = (int)ofRandom(0,2) ? -ofRandom(-300, -200) : interactiveRect.height+ofRandom(-300, -200);		
	startPos.z = ofRandom(-300, 300);
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->voteBubbleID = voteID;
	bubble->data		 = NULL;
	bubble->radius		 = radius;
	bubble->rigidBody	 = bullet->createSphere(startPos, radius, 1);
	bubble->bVoteEnabled = false;
	bubble->voteTimer	 = 0;
	bubble->voteDelay	 = 0;
	bubble->bVoteNeedsUpdate = false;
	
	bubble->createContentBubble();
	
	//bubble->setTarget( voteBubbles[voteID]->getPosition() );
	bubbles.push_back(bubble);
	return bubbles.back();
		// voteBubbles[voteID]->totalVotes ++;
}

//--------------------------------------------------------
void InteractionVote::newBubbleRecieved(Donk::BubbleData * data) { 
}

//--------------------------------------------------------
void InteractionVote::setVoteCount(int totalA, int totalB) {
	
	if(bAnimateOut) return;

	///int totalA = Donk::QuestionData::all[data->questionID].tag_counts[0];
	///int totalB = Donk::QuestionData::all[data->questionID].tag_counts[1];
	
	///string optA = Donk::QuestionData::all[data->questionID].tags[0];
	///string optB = Donk::QuestionData::all[data->questionID].tags[1];
	 
	int total      = totalA + totalB;
	pctA		   = round( ((float)totalA / (float)total) * 100.0);
	pctB           = round( ((float)totalB / (float)total) * 100.0);
	
	
	if(pctA == voteBubbles[0]->pct && pctB == voteBubbles[1]->pct) {
		printf("no need to update\n");
		//return;	
	}
	
	// Option A
	if(voteBubbles[0] != NULL) voteBubbles[0]->pctDes = pctA;	
	
	// Option B
	if(voteBubbles[1] != NULL) voteBubbles[1]->pctDes = pctB;	
	
	
	// the first time we are going to make the 
	// bubbles they will not be active...
	if(bubbles.size() < 100) {
		
		InteractionPerformance * interaction = (InteractionPerformance*)testApp::instance->projection->getInteraction(MODE_PERFORMANCE);

		for (int i=0; i<100; i++) {
			ContentBubble * bubble = addBubbleToVote(0); // 0 for initial setting
			if(interaction != NULL) bubble->voteImageID  = MIN(i, interaction->images.size()-1);
			else bubble->voteImageID = -1;
		}
		printf("just made %i bubbles!\n", (int)bubbles.size());
	}
	printf("A:%i\nB:%i\nT:%i\n", pctA, pctB, pctA+pctB);
	
	// now loop through all the bubbles and set them to 
	// the vote bubble that they should be on...
	for (int i=0; i<100; i++) {
		ContentBubble * bubble = bubbles[i];
		
		if(i < pctA) {
			bubble->setTarget( voteBubbles[0]->getPosition() );
			bubble->bVoteEnabled = true;
			//bubble->voteBubbleID = 0;
			voteIds[i] = 0;
		}
		else {
			bubble->setTarget( voteBubbles[1]->getPosition() );
			bubble->bVoteEnabled = true;
			//bubble->voteBubbleID = 1;
			voteIds[i] = 1;
			
		}
		
		bubble->voteDelay = ofRandom(0.5, 2.0);		
		bubble->voteTimer = ofGetElapsedTimeMillis();
		bubble->bVoteNeedsUpdate = true;
	}
	
	

	
};

//--------------------------------------------------------
void InteractionVote::update() {
	
	if(bAnimateOut) {
		bool bAllOffscreen = true;
		for(int i=0; i<bubbles.size(); i++) {
			if(bubbles[i]->getPosition().y > 0) {
				bAllOffscreen = false;
			}
		}
		
		// we are down and ready tp remove all the bubbles...
		float time = (ofGetElapsedTimeMillis() - animatedOutTimer) / 1000.0;
		if(bAllOffscreen && !bDoneAnimatingOut || time > MAX_ANIMATION_TIME) {
			killallBubbles();
			
			for (int i=0; i<2; i++) {
				if(voteBubbles[i]) {
					voteBubbles[i]->rigidBody->destroy();
					delete voteBubbles[i];
					voteBubbles[i] = NULL;
				}
			}
			
			bDoneAnimatingOut = true;
			printf("all done in vote remove it all\n");
		}
	}
	
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(bAnimateOut) {
			bubbles[i]->gotoTarget();
			bubbles[i]->update();	
		}
		else {
			
			float time = (ofGetElapsedTimeMillis() - bubbles[i]->voteTimer) / 1000.0;
			if(time > bubbles[i]->voteDelay && bubbles[i]->bVoteNeedsUpdate) {
				bubbles[i]->voteBubbleID = voteIds[i];
				bubbles[i]->bVoteNeedsUpdate = false;
			}
			
			if(bubbles[i]->bVoteEnabled) {
				
				int voteID   = bubbles[i]->voteBubbleID;
				if(voteID != -1) {
					ofVec3f  pos = voteBubbles[ voteID ]->getPosition();
					pos.z -= 130;
					bubbles[i]->setTarget( pos );
				}
				
				bubbles[i]->rigidBody->body->setDamping(0.999, 0.999); // <-- add some crazy damping
				bubbles[i]->gotoTarget(1.0);
				bubbles[i]->update();	
			}
		}
	}	
	
	
	for (int i=0; i<2; i++) {
		if(!voteBubbles[i]) continue;
		
		
		if(nTouches == 0) {
			voteBubbles[i]->bTouched = false;	
		}
		
		if(bAnimateOut) {
			voteBubbles[i]->gotoTarget();
			voteBubbles[i]->update();
		}
		else {
			voteBubbles[i]->rigidBody->body->setDamping(0.999, 0.999); // <-- add some crazy damping
			voteBubbles[i]->gotoTarget(50.0);
			voteBubbles[i]->bobMe();
			voteBubbles[i]->update();
		}
	}
	
	
	
}

//--------------------------------------------------------
void InteractionVote::drawContent() {

	
	ofEnableAlphaBlending();

	
	InteractionPerformance * interaction = (InteractionPerformance*)testApp::instance->projection->getInteraction(MODE_PERFORMANCE);

	ofSetColor(255, 255, 255);
	for(int i=0; i<bubbles.size(); i++) {
		if(bubbles[i]->bVoteEnabled) {
			
			ofEnableAlphaBlending();
			// bubbles[i]->drawHighLight();
			// bubbles[i]->drawTwitterData();
			
			if( bubbles[i]->voteImageID != -1) {
				bubbles[i]->pushBillboard();
				int     w = bubbles[i]->radius * 2;
				ofSetRectMode(OF_RECTMODE_CENTER);
				interaction->images[ bubbles[i]->voteImageID ].draw(0, 0, w, w);
				ofSetRectMode(OF_RECTMODE_CORNER);
				bubbles[i]->popBillboard();
			}
		}
	}
	
	
	
	for(int i=0; i<2; i++) {
		if(voteBubbles[i]) {
			voteBubbles[i]->drawInsideContent();
		}
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

	for(int i=0; i<bubbles.size(); i++) {
		ofVec3f offScreenPos;
		offScreenPos.x = bubbles[i]->getPosition().x;
		offScreenPos.y = ofRandom(-400, -500);	
		offScreenPos.z = ofRandom(-100, -400);
		bubbles[i]->setTarget(offScreenPos);
	}
	
	if(voteBubbles[0]) voteBubbles[0]->setTarget(-500, voteBubbles[0]->getPosition().y);
	if(voteBubbles[1]) voteBubbles[1]->setTarget(interactiveRect.width+500, voteBubbles[1]->getPosition().y);
	
	bAnimateOut		  = true;
	bDoneAnimatingOut = false;
	animatedOutTimer  = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------
void InteractionVote::animatedIn() {
	bAnimateOut		  = false;
	bDoneAnimatingOut = false;
	printf("--- Vote Animate In ---\n");
}

