/*
 *  InteractionPerformance.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionPerformance.h"


//--------------------------------------------------------
void InteractionPerformance::setup() {
	audio = Donk::AudioData::getInstance();	
	nBands = audio->getNumChannels();
	freq = new float[nBands];
	for(int i=0; i<nBands; i++) freq[i] = 0.0;

	
	// load all the images
	ofxDirList			lister;
	lister.setVerbose(false);
	int nFiles = lister.listDir("images/performance");
	
	for (int i=0; i<nFiles; i++) {
		images.push_back(ofImage());
		if(images.back().loadImage(lister.getPath(i))) {
			printf("loaded image in performance\n");
		}
	}
	
	
	for(int i=0; i<nBands; i++) {
		
		float offset = 500;
		float radius = 110;
		float x = offset + ((interactiveRect.width-offset)/nBands) * i;
		ofVec3f center(interactiveRect.width/2, 0, 0);
		ofVec3f startPos(center.x + ofRandom(-300, 300), 
						 interactiveRect.height, 
						 ofRandom(-100, 100));

		ContentBubble * bubble = new ContentBubble();
		bubble->performceImageID   = i;
		bubble->performanceChannel = i;
		
		bubble->data	  = NULL;
		bubble->radius    = radius;
		bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
		bubble->createContentBubble();
		ofRandom(500, interactiveRect.height-300);
		bubble->target.set(x, (interactiveRect.height/2), 0);
		bubble->performanceStartTarget = bubble->target;
		
		bubbles.push_back(bubble);
	}
	
}


//--------------------------------------------------------
void InteractionPerformance::newBubbleRecieved(Donk::BubbleData * data) { 
	
	/*ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	float   radius = 80;
	
	ContentBubble * bubble = new ContentBubble();
	bubble->performceImageID = (int)ofRandom(0, images.size());
	bubble->performanceChannel = ofRandom(0, nBands);
	
	bubble->data	  = NULL;
	bubble->radius    = radius;
	bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
	bubble->createContentBubble();
	bubble->target.set(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	bubbles.push_back(bubble);
	
	printf("new performance bubble added\n");
	*/
};

//--------------------------------------------------------
void InteractionPerformance::update() {
	
	for(int i=0; i<bubbles.size(); i++) {
		
		ofVec3f target = bubbles[i]->target;
		target.y = (bubbles[i]->performanceStartTarget.y) - (audio->getVolume(bubbles[i]->performceImageID) * 320.0);
		
		bubbles[i]->rigidBody->body->setDamping(0.99, 0.99);
		bubbles[i]->addAtrractionForce(target.x, target.y, target.z, 30.0);
		bubbles[i]->update();
		
		float newRad = freq[ bubbles[i]->performanceChannel ] * 60.0;
		bubbles[i]->setRadius(30 + newRad);
		
		//		ofVec3f p = bubbles[i]->getPosition();
		//		
		//	//	printf("%f %f\n", p.y, interactiveRect.height);
		//		
		//		if(p.y < 0) {
		//			ofVec3f center(interactiveRect.width/2, 0, 0);
		//			ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
		//			bubbles[i]->rigidBody->body->clearForces();
		//			bubbles[i]->rigidBody->setPosition(startPos, 0, 0);
		//		}
	}	
	
}


//--------------------------------------------------------
void InteractionPerformance::drawContent() {
	
	// fft goods
	glPushMatrix();
	ofSetColor(255, 0, 0);
	ofNoFill();
	ofBeginShape();
	for (int i=0; i<nBands; i++) {
		
		freq[i] *= 0.986f;
		if(freq[i] < audio->getVolume(i)) {
			freq[i] = audio->getVolume(i);
		}
		
		float x   = ((float)i/(nBands-1)) * interactiveRect.width;
		float y   = 200 + (interactiveRect.height/2);
		float frq = freq[i] * 200.0;
		ofVertex(x, y-frq);
	}
	ofEndShape();
	glPopMatrix();
	
	
	
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);
	for(int i=0; i<bubbles.size(); i++) {
		//bubbles[i]->drawHighLight();
		//bubbles[i]->pushBubble();
		
		bubbles[i]->pushBillboard();
		int     w = bubbles[i]->radius * 2;
		ofSetRectMode(OF_RECTMODE_CENTER);
		images[ bubbles[i]->performceImageID ].draw(0, 0, w, w);
		ofSetRectMode(OF_RECTMODE_CORNER);
		bubbles[i]->popBillboard();
		
		// test...
		//images[ bubbles[i]->performceImageID ].bind();
		
		//images[ bubbles[i]->performceImageID ].unbind();
		
		//bubbles[i]->popBubble();
		//bubbles[i]->pushBillboard();
		//images[	bubbles[i]->performceImageID ].draw(p.x, p.y);
		//bubbles[i]->popBillboard();
		
	}
	
}

//--------------------------------------------------------
void InteractionPerformance::drawSphere(BubbleShader * shader) {
	// i think it looks funny...
	/*for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->pushBubble();
		shader->begin();
		bubbles[i]->draw();
		shader->end();
		bubbles[i]->popBubble();
	}*/
	
}


//--------------------------------------------------------
void InteractionPerformance::animatedOut() {
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->rigidBody->body->setActivationState(DISABLE_DEACTIVATION);
	}
}

//--------------------------------------------------------
void InteractionPerformance::animatedIn() {
	
	// if(bullet) bullet->setGravity(0, -500, 0);
	
	/*for(int i=0; i<bubbles.size(); i++) {
		
		float radius = 80;
		float x = 100 + (i * 100);
		ofVec3f center(interactiveRect.width/2, 0, 0);
		ofVec3f startPos(center.x + ofRandom(-300, 300), 
						 interactiveRect.height, 
						 ofRandom(-100, 100));
		
		ContentBubble * bubble = new ContentBubble();
		bubble->performceImageID   = (int)ofRandom(0, images.size());
		bubble->performanceChannel = i;
		
		bubble->data	  = NULL;
		bubble->radius    = radius;
		bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
		bubble->createContentBubble();
		ofRandom(500, interactiveRect.height-300);
		bubble->target.set(x, (interactiveRect.height/2)-300, 0);
		bubbles.push_back(bubble);
	}
	*/
	
}




