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
		bubble->offScreenTaget = bubble->performanceStartTarget;
		bubble->offScreenTaget.y = -100;
		
        
		bubbles.push_back(bubble);        
	}
    
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
void InteractionPerformance::newBubbleRecieved(Donk::BubbleData * data) { 
};

//--------------------------------------------------------
void InteractionPerformance::update() {
	
	bool bAllOffScreen = true;
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(bubbles[i]->bAnimateOut) {
			bubbles[i]->goOffScreen();
			
			if(bubbles[i]->getPosition().y > 0) {
				bAllOffScreen = false;
			}
			
		}
		else {		
			ofVec3f target = bubbles[i]->target;
			target.y = (bubbles[i]->performanceStartTarget.y) - (audio->getVolume(bubbles[i]->performceImageID) * 320.0);
			
			bubbles[i]->rigidBody->body->setDamping(0.99, 0.99);
			bubbles[i]->addAtrractionForce(target.x, target.y, target.z, 30.0);
		}	
		
		bubbles[i]->update();
		
        float newRad = freq[ bubbles[i]->performanceChannel ] * 60.0;
		bubbles[i]->setRadius(30 + newRad);
		
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
	
	if(bAllOffScreen && bAnimateOut) {
		bDoneAnimatingOut = true;
	}
	
	lineAlpha += (lineAlphaDes-lineAlpha) * 0.02;

 
    updateRibbons();
}


//--------------------------------------------------------
void InteractionPerformance::drawContent() {
	
	ofEnableAlphaBlending();

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
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->bAnimateIn  = false;
		bubbles[i]->bAnimateOut = true;
		
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
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->bAnimateIn  = true;
		bubbles[i]->bAnimateOut =false;
		bubbles[i]->rigidBody->body->setActivationState(DISABLE_DEACTIVATION);
	}
	
	bAnimateIn  = true;
	bAnimateOut = false;
	
	bDoneAnimatingOut = true;
	bDoneAnimatingIn  = false;	
}





