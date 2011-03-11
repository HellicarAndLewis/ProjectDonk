/*
 *  InteractionBuzz.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionBuzz.h"
#include "testApp.h"

#define BIT(x) (1<<(x))
enum collisiontypes{
    COL_NOTHING = 0,	
    COL_CONTAINER = BIT(0),
    COL_BUBBLE_IN = BIT(1),
	COL_BUBBLE_OUT = BIT(2),
	COL_CONT_FRAME = BIT(3),
	COL_CONT_DEAD = BIT(4)
};

int collidesWithNone		= COL_NOTHING;
int contCollidesWith		= COL_CONTAINER | COL_BUBBLE_OUT;
int bubbleOutCollidesWith	= COL_CONTAINER | COL_BUBBLE_OUT;
int bubbleInCollidesWith	= COL_BUBBLE_IN | COL_CONT_FRAME;
int contFrameCollidesWith	= COL_BUBBLE_IN;

//--------------------------------------------------------
void InteractionBuzz::newBubbleRecieved(Donk::BubbleData * data) { 
	
	// create the container first
	createContainerBubble(data);
	
	int id = bubbles.size()-1;
	ofVec3f momPos = bubbles[id]->rigidBody->getPosition();
	
	
	int   total = 10;
	float cRad = CONTAINER_RADIUS;
	float volCont = (( 4.0/3.0)*PI )*(cRad*cRad*cRad);
	float volBubb = volCont / (total * 5.f);
	
	float radius = volBubb / (( 4.0/3.0)*PI );
	radius = pow(radius, 1.0f/3.0f);
	
	float hRad = cRad *.5;
	
	for( int i = 0; i < total; i++)
	{
		
		ofVec3f startPos = ofVec3f(momPos.x+ofRandom(-hRad,hRad),momPos.y+ofRandom(-hRad,hRad),momPos.z+ofRandom(-hRad,hRad));
		
		
		ContentBubble * bubble = new ContentBubble();
		
		bubble->data	  = data;
		bubble->radius    = radius;
		
		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(startPos.x/SCALE, startPos.y/SCALE, startPos.z/SCALE));
		
		bubble->rigidBody = new ofxBulletRigidBody();
		bubble->rigidBody->world = bullet->world;
		bubble->rigidBody->shape = new btSphereShape(radius/SCALE);
		bubble->rigidBody->createRigidBody(1, startTransform);
		
		bullet->world->addRigidBody(bubble->rigidBody->body, COL_BUBBLE_IN, bubbleInCollidesWith);
		bullet->rigidBodies.push_back(bubble->rigidBody);
		
		bubble->createContentBubble();
		bubble->target = momPos;
		bubble->targetForce = 20.f;
		
		bubble->offScreenTaget.x = bubble->target.x;
		bubble->offScreenTaget.y = -300;
		
		bubbles.push_back(bubble);
		
		bubbleTypes.push_back(BUZZ_TYPE_BUBBLE_IN);
		bubbleToContIndex.insert( pair<int,int>(bubbles.size()-1,id) );
		
	}
	
};

//--------------------------------------------------------
void InteractionBuzz::update() {
	
	bool bAllOffFadedOut = true;
	bool bAllOffScreen = true;

	map <int, int> :: const_iterator it;
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(nTouches == 0) {
			bubbles[i]->bTouched = false;			
		}
		
		if(!bubbles[i]->bTouched) {
			
			if(bAnimateIn) {
				if(bubbles[i]->alpha <= 255) bubbles[i]->alpha += 10;
				if(bubbleTypes[i] != BUZZ_TYPE_BUBBLE_IN) bubbles[i]->gotoTarget();
			}
			
			else if(bAnimateOut && !bDoneAnimatingOut) {
				
				//if(bubbles[i]->alpha > 0) {
					//bubbles[i]->alpha -= 10;
					//bAllOffFadedOut = false;
				//}
				bubbles[i]->goOffScreen();
				if(bubbles[i]->getPosition().y > 0) {
					bAllOffScreen = false;
				}
				
			}
			
		}
		
		// inner bubbles fall to bottom of container
		if(bubbleTypes[i] == BUZZ_TYPE_BUBBLE_IN)
		{
			bubbles[i]->rigidBody->body->applyCentralForce(btVector3(0,40,0));
		}
		else if(bubbleTypes[i] == BUZZ_TYPE_CONTAINER)
		{
			((BuzzContainerBubble*)bubbles[i])->updateConstraint();
		}
		else if(bubbleTypes[i] == BUZZ_TYPE_BUBBLE_OUT)
		{
			if(bubbles[i]->getPosition().y > 0) 
				if(bubbles[i]->alpha > 0) bubbles[i]->alpha -= 10;
				//bubbles[i]->alpha = 0;
		}
		
		
		bubbles[i]->update();
		
		testApp::instance->projection->champagne.particles.push_back( new BrownianObject( bubbles[i]->pos, 0));

	}	
	
	if(bAllOffScreen && bAnimateOut) {
		bDoneAnimatingOut = true;
		for(int i=0; i<bubbles.size(); i++) {
			bubbles[i]->rigidBody->body->setActivationState(DISABLE_SIMULATION);
		}
		
	}
	
}

//--------------------------------------------------------
void InteractionBuzz::drawContent(){
	
	/*
	 for(int i=0; i<bubbles.size(); i++){
	 if(bubbles[i]->bAlive){
	 if(bubbleTypes[i] == BUZZ_TYPE_CONTAINER)
	 {
	 ((BuzzContainerBubble*)bubbles[i])->globe->drawChildren();
	 }
	 }
	 }
	 */
	
	for(int i=0; i<bubbles.size(); i++) {
		if(bubbles[i]->bAlive){
			bubbles[i]->drawHighLight();
			if(bubbleTypes[i] != BUZZ_TYPE_CONTAINER) bubbles[i]->drawTwitterData();
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
	
	// hack for now.
	killallBubbles();
	bDoneAnimatingOut = true;
	
	bAnimateIn  = true;
	bAnimateOut = false;
	
	bDoneAnimatingOut = true;
	bDoneAnimatingIn  = false;
	
	for(int i=0; i<bubbles.size(); i++) {
		bubbles[i]->rigidBody->body->setActivationState(DISABLE_DEACTIVATION);
	}
	
}

void InteractionBuzz::createContainerBubble(Donk::BubbleData * data){
	
	cout << "Buzz: adding new container bubble" << endl;
	
	BuzzContainerBubble * container = new BuzzContainerBubble();
	
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-500, 500), interactiveRect.height+200, ofRandom(-100, 100));
	float radius = CONTAINER_RADIUS+20;
	
	container->data	  = data;
	container->radius = radius;
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(startPos.x/SCALE, startPos.y/SCALE, startPos.z/SCALE));
	
	container->rigidBody = new ofxBulletRigidBody();
	container->rigidBody->world = bullet->world;
	container->rigidBody->shape = new btSphereShape(radius/SCALE);
	container->rigidBody->createRigidBody(1, startTransform);
	
	container->target.set(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	container->targetForce = 5;
	
	container->offScreenTaget.x = container->target.x;
	container->offScreenTaget.y = -300;
	
	bullet->world->addRigidBody(container->rigidBody->body, COL_CONTAINER, contCollidesWith);
	bullet->rigidBodies.push_back(container->rigidBody);
	
	container->createContainerBubble(bullet,startPos);
	bullet->world->addRigidBody(container->globe->body, COL_CONT_FRAME,contFrameCollidesWith);
	bullet->rigidBodies.push_back(container->globe);
	
	bubbles.push_back(container);
	bubbleTypes.push_back(BUZZ_TYPE_CONTAINER);
}

void InteractionBuzz::doubleTouched(ofVec2f touchpos)
{
	
	// find out who has been popped
	int poppedID = -1;
	
	for(int i=0; i<bubbles.size(); i++)
	{
		if(bubbleTypes[i] == BUZZ_TYPE_CONTAINER && ((BuzzContainerBubble*)bubbles[i])->bTouched && bubbles[i]->bAlive)
		{
			poppedID = i;
			
			BuzzContainerBubble* bubble = (BuzzContainerBubble*)bubbles[i];
			bubble->pop();
			bullet->world->removeRigidBody(bubble->rigidBody->body);
			
			break;
		}
	}
	
	
	if(poppedID > -1)
	{
		clearOldBubbles();
		releaseContainedBubbles(poppedID);
		
	}
	
	
	
}


void InteractionBuzz::clearOldBubbles()
{
	for(int i=0; i<bubbles.size(); i++)
	{
		if(bubbleTypes[i] == BUZZ_TYPE_BUBBLE_OUT)
		{
			bubbles[i]->target.y = -300;//bubbles[i]->offScreenTaget;
			bubbleTypes[i] == BUZZ_TYPE_BUBBLE_OLD;
		}
	}
}

void InteractionBuzz::releaseContainedBubbles(int poppedID)
{
	map <int, int> :: const_iterator it;
	
	for(int i=0; i<bubbles.size(); i++)
	{
		if(bubbleTypes[i] == BUZZ_TYPE_BUBBLE_IN)
		{
			ContentBubble * bubble = bubbles[i];
			
			it =  bubbleToContIndex.find(i);
			
			if( it != bubbleToContIndex.end() && it->second == poppedID )
			{
				bubbleTypes[i] = BUZZ_TYPE_BUBBLE_OUT;
				bubble->target = bubble->rigidBody->getPosition();
				bubble->target.x += ofRandom(-300, 300);
				bubble->target.y += ofRandom(-300, 300);
				bubble->targetForce = 10.f;
				
				setCollisionFilter(bubble->rigidBody,COL_BUBBLE_OUT,bubbleOutCollidesWith);
				
			}
		}
	}
}

void InteractionBuzz::setCollisionFilter(ofxBulletRigidBody * rigidBody, int filter, int mask)
{
	ofVec3f startPos = rigidBody->getPosition();
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
	
	bullet->world->removeRigidBody(rigidBody->body);
	rigidBody->createRigidBody(1, startTransform);
	
	bullet->world->addRigidBody(rigidBody->body, filter, mask);
	
	
}

