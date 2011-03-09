/*
 *  InteractionBuzz.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/6/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "InteractionBuzz.h"

#define BIT(x) (1<<(x))
enum collisiontypes{
    COL_NOTHING = 0,	
    COL_CONTAINER = BIT(0),
    COL_BUBBLE_IN = BIT(1),
	COL_BUBBLE_OUT = BIT(2)
};

int contCollidesWith		= COL_CONTAINER | COL_BUBBLE_OUT;
int bubbleOutCollidesWith	= COL_CONTAINER | COL_BUBBLE_OUT;
int bubbleInCollidesWith	= COL_BUBBLE_IN;

//--------------------------------------------------------
void InteractionBuzz::newBubbleRecieved(Donk::BubbleData * data) { 
	
	// create the container
	createContainerBubble(data);
	
	int id = bubbles.size()-1;
	ofVec3f target = bubbles[id]->rigidBody->getPosition();
	
	ofVec3f center(interactiveRect.width/2, 0, 0);
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	float   radius = ofRandom(30,60);
	
	for( int i = 0; i < 4; i++)
	{
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	
	//bubble->rigidBody = bullet->createSphere(startPos, radius, 1);
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
	bubble->target = target;//.set(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	bubble->targetForce = 200.f;
	
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
	
	map <int, int> :: const_iterator it;
	
	for(int i=0; i<bubbles.size(); i++) {
		
		if(nTouches == 0) {
			bubbles[i]->bTouched = false;			
		}
		
		if(!bubbles[i]->bTouched) {
			
			if(bAnimateIn) {
				if(bubbles[i]->alpha <= 255) bubbles[i]->alpha += 10;
				bubbles[i]->gotoTarget();
			}
			
			else if(bAnimateOut && !bDoneAnimatingOut) {
				if(bubbles[i]->alpha > 0) {
					bubbles[i]->alpha -= 10;
					bAllOffFadedOut = false;
				}
				// bubbles[i]->goOffScreen();
			}
			
		}
		
		if(bubbleTypes[i] == BUZZ_TYPE_BUBBLE_IN)
		{
			it = bubbleToContIndex.find(i);
			ofVec3f targ = bubbles[ it->second ]->rigidBody->getPosition();
			bubbles[i]->target = targ;
		}
		
		bubbles[i]->update();

	}	
	
	if(bAllOffFadedOut && bAnimateOut) {
		bDoneAnimatingOut = true;
		for(int i=0; i<bubbles.size(); i++) {
			bubbles[i]->rigidBody->body->setActivationState(DISABLE_SIMULATION);
		}
		
	}
	
}

//--------------------------------------------------------
void InteractionBuzz::drawContent() {
	
	for(int i=0; i<bubbles.size(); i++) {
		if(bubbles[i]->bAlive){
			bubbles[i]->drawHighLight();
			bubbles[i]->drawTwitterData();
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
	ofVec3f startPos(center.x + ofRandom(-300, 300), interactiveRect.height, ofRandom(-100, 100));
	float radius = 200;
	
	container->data	  = data;
	container->radius = radius;
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(startPos.x/SCALE, startPos.y/SCALE, startPos.z/SCALE));
	
	container->rigidBody = new ofxBulletRigidBody();
	container->rigidBody->world = bullet->world;
	container->rigidBody->shape = new btSphereShape(radius/SCALE);
	container->rigidBody->createRigidBody(1, startTransform);
	
	bullet->world->addRigidBody(container->rigidBody->body, COL_CONTAINER, contCollidesWith);
	bullet->rigidBodies.push_back(container->rigidBody);
	
	container->createContainerBubble(bullet);
	container->target.set(center.x + ofRandom(-300, 300), ofRandom(500, interactiveRect.height-300), 0);
	container->offScreenTaget.x = container->target.x;
	container->offScreenTaget.y = -300;
	
	bubbles.push_back(container);
	bubbleTypes.push_back(BUZZ_TYPE_CONTAINER);
}

void InteractionBuzz::doubleTouched(ofVec2f touchpos)
{
	int poppedID = -1;
	for(int i=0; i<bubbles.size(); i++)
	{
		if(bubbleTypes[i] == BUZZ_TYPE_CONTAINER && ((BuzzContainerBubble*)bubbles[i])->bTouched)
		{
			poppedID = i;
			((BuzzContainerBubble*)bubbles[i])->pop();
			break;
		}
	}

	map <int, int> :: const_iterator it;
		
	for(int i=0; i<bubbles.size(); i++)
	{
		ContentBubble * bubble = bubbles[i];
			
		if(bubbleTypes[i] == BUZZ_TYPE_BUBBLE_IN && poppedID > -1)
		{
				it =  bubbleToContIndex.find(i);
				if( it != bubbleToContIndex.end() && it->second == poppedID )
				{
					bubbleTypes[i] = BUZZ_TYPE_BUBBLE_OUT;
					bubble->target.x += ofRandom(-300, 300);
					bubble->target.y += ofRandom(-300, 300);
					bubble->targetForce = 10.f;

					btTransform trans = bubble->rigidBody->body->getWorldTransform();
					
					btVector3	minAabb;
					btVector3	maxAabb;
					bubbles[i]->rigidBody->body->getCollisionShape()->getAabb(trans,minAabb,maxAabb);
					
					int type = bubble->rigidBody->body->getCollisionShape()->getShapeType();
					bubbles[i]->rigidBody->body->setBroadphaseHandle( 
																	 bullet->world->getBroadphase()->createProxy(	
																												 minAabb,
																												 maxAabb,
																												 type,
																												 bubble->rigidBody->body,
																												 COL_BUBBLE_OUT,
																												 bubbleOutCollidesWith,
																												 bullet->world->getDispatcher(),0 
																												 )
																	 );
				}
			}
			
		
		else if( bubbleTypes[i] == BUZZ_TYPE_BUBBLE_OUT )
		{
			ofVec2f p1  = touchpos;
			ofVec2f p2  = bubble->rigidBody->getPosition();
			float	dis = p1.distance(p2);
			
			if(dis < bubble->radius + 10.0) {
				bubble->setRadius(120);
				bubble->doubleTouched();
				printf("hit this bubble: %p\n", bubble);
				break;
			}
		}
	
	}
	
}

