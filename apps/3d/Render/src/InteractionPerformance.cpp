/*
 *  InteractionPerformance.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/7/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionPerformance.h"
#include "testApp.h"

//--------------------------------------------------------
void InteractionPerformance::setup() {
	
	lineAlpha = 0;
	lineAlphaDes = 255;
	
	flockTime  = 0;
	flockPos.x = interactiveRect.width/2;
	flockPos.y = interactiveRect.height/2;
	flockPos.z = 0;
    
	audio = Donk::AudioData::getInstance();	
	nBands = audio->getNumChannels();
    
	freq = new float[nBands];
    colors = new ofColor[nBands];
    ribbonCounts = new int[nBands];
    lastFreq = new float[nBands];
	for(int i=0; i<nBands; i++){
        freq[i] = 0.0;   
        lastFreq[i] = 0.0;
        ribbonCounts[i] = 0;
    }
	
	
	// load all the images
	ofxDirList			lister;
	lister.setVerbose(false);
	int nFiles = lister.listDir("images/performance");
	printf("found %i files for performance\n", nFiles);
	for (int i=0; i<nFiles; i++) {
		images.push_back(ofImage());
		if(!images.back().loadImage(lister.getPath(i))) {
			printf("error loaded image in performance\n");
		}
	}
	
	//now lets have 36 bubbles in total, in random positions
	// addBubbles();
	
	
    
    maxRibbonsPerChannel = 100;
    //add uggo-matic colors on purpose for debug
    colors[0] = ofColor(1.0, 0.0, 0.0);
    colors[1] = ofColor(1.0, 1.0, 0.0);
    colors[2] = ofColor(0.0, 1.0, 1.0);
    colors[3] = ofColor(1.0, 0.0, 1.0);
    colors[4] = ofColor(0.0, 1.0, 0.0);
    colors[5] = ofColor(1.0, 1.0, 1.0);
}

//--------------------------------------------------------
void InteractionPerformance::addBubbles() {
	for(int j=0; j<nBands; j++){
		for(int i=0; i<nBands; i++) {
			
			ofVec3f center(interactiveRect.width/2, 0, 0);
			float radius = ofRandom(70, 120);
			
			ofVec3f startPos;
			startPos.x = (int)ofRandom(0,2) ? -100 : interactiveRect.width+100;
			startPos.y = (int)ofRandom(0,2) ? -100 : interactiveRect.height+100;		
			startPos.z = ofRandom(-500, 500);
			
			ContentBubble * bubble = new ContentBubble();
			
			bubble->startRadius = radius;
			bubble->maxRadius = radius; // not sure, maye set to larger for audio manipulation?
			bubble->data	  = NULL;
			bubble->radius    = radius;
			bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
			bubble->createContentBubble();
			bubble->setTarget(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);			
			
			bubble->performceImageID   = (int)ofRandom(0, images.size());//(j*6)+i; //now we are making 36, so go deeper into the image array
			bubble->performanceChannel = i;
			
			bubble->performanceStartTarget = bubble->target;
			bubble->offScreenTaget = bubble->performanceStartTarget;
			bubble->offScreenTaget.y = -100;
			
			bubbles.push_back(bubble);        
		}		
	}
}

//--------------------------------------------------------
void InteractionPerformance::newBubbleRecieved(Donk::BubbleData * data) { 
};

//--------------------------------------------------------
void InteractionPerformance::update() {
	
	flockTime += 0.003;
	float div = 900.0;
	float amp = 450.0;
	
	ofVec3f center;
	center.x = interactiveRect.width/2;
	center.y = interactiveRect.height/2;
	center.z = 0;
	
	bool bAllOffScreen = true;
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(bubbles[i]->bAnimateOut) {
			bubbles[i]->goOffScreen();
			
			if(bubbles[i]->getPosition().y > 0) {
				bAllOffScreen = false;
			}
			
		}
		else {		
			ofVec3f pos    = bubbles[i]->getPosition();
			ofVec3f target = bubbles[i]->target;
			target.y = (bubbles[i]->performanceStartTarget.y) - (audio->getVolume(bubbles[i]->performceImageID) * 320.0);
			
			float t = ofGetElapsedTimef() * 0.03;
			float div = 400.0;
			ofVec3f noise(ofSignedNoise(t, pos.y/div, pos.z/div),
						  ofSignedNoise(pos.x/div, t, pos.z/div),
						  ofSignedNoise(pos.x/div, pos.y/div, t));
			noise *= 200.0;
			
			bubbles[i]->flockPos.y -= 5;
			bubbles[i]->flockPos.x = cos(t) * 20;
			
			target += bubbles[i]->flockPos;
			target += noise;
			
			bubbles[i]->rigidBody->body->setDamping(0.99, 0.99);
			bubbles[i]->addAtrractionForce(target.x, target.y, target.z, 30.0);
			//bubbles[i]->rigidBody->setPosition(center, 0, 0);
		//	bubbles[i]->rigidBody->body->a ( ofVec3fToBtVec(target) );
			
			if(pos.y < -(bubbles[i]->radius*2)) {
				bubbles[i]->flockPos = 0;
				ofVec3f resetPos(pos.x, interactiveRect.height, 0);
				bubbles[i]->rigidBody->setPosition(resetPos, 0, 0);	
			}
		}	
		
		bubbles[i]->update();
		
        float newRad = freq[ bubbles[i]->performanceChannel ] * 100.0;
		bubbles[i]->lerpRadius(bubbles[i]->startRadius + newRad, 0.94);
		
		champagne(bubbles[i]->pos);
		
        
        //check for ribbon emmitting 
        float dFreq = freq[ bubbles[i]->performanceChannel ] - lastFreq[ bubbles[i]->performanceChannel ];
        int numRibbonsToEmit = ofMap(dFreq, .05, .2, 0, 20, true);
        
        //cout << " channel " << bubbles[i]->performanceChannel << " freq " << dFreq << endl;
        
        //clamp to max
        numRibbonsToEmit = MIN(numRibbonsToEmit, maxRibbonsPerChannel - (ribbonCounts[bubbles[i]->performanceChannel] + numRibbonsToEmit) );
        for(int j = 0; j < numRibbonsToEmit; j++){
            addRibbonForIndex(i);
        }
        //cache freq
        lastFreq[ bubbles[i]->performanceChannel ] = freq[ bubbles[i]->performanceChannel ];
	}	
	
	if(bAnimateOut) {
		float time = (ofGetElapsedTimeMillis()-animatedOutTimer) / 1000.0;
		if(bAllOffScreen && !bDoneAnimatingOut || time > 3.0) {
			bDoneAnimatingOut = true;
			killallBubbles();
			printf("-- killed all performance bubbles --\n");
		}
	}
	
	lineAlpha += (lineAlphaDes-lineAlpha) * 0.02;

 
    updateRibbons();
}


//--------------------------------------------------------
void InteractionPerformance::drawContent() {
	
	ofEnableAlphaBlending();


	/*
	// fft goods
	glPushMatrix();
	ofSetColor(255, 0, 0, lineAlpha);
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
	*/
    
    drawRibbons();
	
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
    
	//ofSetColor(255, 0, 0);
	//ofSphere(interactiveRect.width/2 + flockPos.x, flockPos.y + interactiveRect.height/2, flockPos.z+100, 10);
	
}

void InteractionPerformance::addRibbonForIndex(int emitterBubble)
{
    //for now just add one ribbon per frame to test
    Ribbon* ribbon = new Ribbon();
    ribbon->ribbonCreatedTime = ofGetElapsedTimef();    
    //animation params
    ribbon->segmentsPerFrame = 3; //careful with this guy, dramatically effects other ribbon parameters
    
    ribbon->maxSegmentAge = 3.0; //changing this will tend to make longer ribbons
    ribbon->maxRibbonAge = 4; //changing this will tend to make ribbons go longer
    ribbon->noiseDamp = 500; //controls the scale of the perlin field. bigger number means slower meander
    ribbon->noiseAmp = .5; //controls the scale of the perlin force applied, bigger number means greater influence
    ribbon->curlySpeed = 3; //twisty action!
    ribbon->speed = ofRandom(3, 8); //ranged speed
    ribbon->maxThickness = 40;
    ribbon->minThickness = 5;
    ribbon->channel = emitterBubble;
    
    //Geometry and space, no real tweaking to do here, random direction picking.
    //pick some random direction and upvec
    ribbon->head = ofNode();
    ribbon->head.setPosition( bubbles[emitterBubble]->getPosition() );
    ribbon->color = colors[emitterBubble];
    ofVec3f direction = ribbon->head.getPosition() + ofVec3f(ofRandomf(),ofRandomf(), ofRandomf()).normalized();
    ofVec3f up = direction.getCrossed(direction + ofVec3f(ofRandomf(),ofRandomf(), ofRandomf()));
    ribbon->head.lookAt(direction, up);
        
    ribbons.push_back(ribbon);
}

void InteractionPerformance::updateRibbons()
{
    for(int i = 0; i < ribbons.size(); i++){
        ribbons[i]->update();
    }
    
    for(int i = ribbons.size()-1; i >= 0; i--){
        if(ribbons[i]->isDead()){
            delete ribbons[i];
            ribbons.erase(ribbons.begin()+i);
        }
    } 
    
    //recalculate totals
    for(int i = 0; i < nBands; i++){
        ribbonCounts[i] = 0;
    }
    
    for(int i = 0; i < ribbons.size(); i++){
        ribbonCounts[ ribbons[i]->channel ]++;
    }
}

void InteractionPerformance::drawRibbons()
{
    for(int i = 0; i < ribbons.size(); i++){
        ribbons[i]->draw();
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
void InteractionPerformance::putToRest() {
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->rigidBody->body->setActivationState(DISABLE_SIMULATION);
	}	
}

//--------------------------------------------------------
void InteractionPerformance::animatedOut() {
	
	animatedOutTimer = ofGetElapsedTimeMillis();
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->bAnimateIn  = false;
		bubbles[i]->bAnimateOut = true;
		
		bubbles[i]->offScreenTaget.x = bubbles[i]->getPosition().x;
		bubbles[i]->offScreenTaget.y = -1000;
		bubbles[i]->offScreenTaget.z = ofRandom(-200, -400);
	}	
	
	bAnimateIn  = false;
	bAnimateOut = true;
	
	bDoneAnimatingOut = false;
	bDoneAnimatingIn  = true;

	lineAlphaDes = 0;
	
}

//--------------------------------------------------------
void InteractionPerformance::animatedIn() {
	
	lineAlphaDes = 255;
	addBubbles();
	
	bAnimateIn  = true;
	bAnimateOut = false;
	
	bDoneAnimatingOut = true;
	bDoneAnimatingIn  = false;	
}





