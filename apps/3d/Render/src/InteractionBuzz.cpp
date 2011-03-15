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
    COL_NOTHING		= 0,	
    COL_CONTAINER	= BIT(0),
    COL_BUBBLE_IN	= BIT(1),
	COL_BUBBLE_OUT	= BIT(2),
	COL_CONT_FRAME	= BIT(3),
};

int collidesWithNone		= COL_NOTHING;
int contCollidesWith		= COL_CONTAINER | COL_BUBBLE_OUT;
int bubbleOutCollidesWith	= COL_CONTAINER | COL_BUBBLE_OUT;
int bubbleInCollidesWith	= COL_BUBBLE_IN | COL_CONT_FRAME;
int contFrameCollidesWith	= COL_BUBBLE_IN;

bool bSetCallback = false;

struct BuzzFilterCallback : public btOverlapFilterCallback {
	
	// return true when pairs need collision 
	virtual bool	needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const {
		
		bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0; 
		collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
		
		//add some additional logic here that modified 'collides' 
		if(proxy1->m_collisionFilterGroup == COL_CONT_FRAME && proxy0->m_collisionFilterGroup == COL_CONT_FRAME)
		{
			collides = (proxy0->m_collisionFilterMask == proxy1->m_collisionFilterMask);
		}
		
		return collides;
	}
};


//--------------------------------------------------------
void InteractionBuzz::newBubbleRecieved(Donk::BubbleData * data) { 
	
	string currentPollGroup = data->polledGroup;
	cout << "polled group " << currentPollGroup << endl;
	
	// initialize the current pollGroup
	if(polledData.size()<=0) momPollGroup = currentPollGroup;
	
	// check if the poll group is new and if so create
	if( currentPollGroup != momPollGroup )
	{
		createMomAndChildBubbles();
		
		// set new poll group
		momPollGroup = currentPollGroup;
	}
	
	// record current data
	polledData.push_back(data);
	
};

//--------------------------------------------------------
void InteractionBuzz::update(){
	
	if(polledData.size() > 0) createMomAndChildBubbles();
	
	bool bAllOffScreen		= true;
	map <int, int> :: const_iterator it;
	
	for(int i=0; i<bubbles.size(); i++){
				
		ContentBubble * bubble = bubbles[i];
		
		//---- update touch
		if(nTouches == 0) {
			
			bubble->bTouched		= false;	
			bubble->bDoubleTouched	= false;		
		}
		
		if(bubble->buzzID == BUZZ_TYPE_BUBBLE_OLD){
			bubble->bTouched = false;
			bubble->bDoubleTouched	= false;
		}
		
		//---- update sizes
		if(bubble->buzzID == BUZZ_TYPE_BUBBLE_OUT){
			if(bubble->bDoubleTouched)	bubble->lerpRadius(150,0.1);
			else						bubble->lerpRadius(90,0.1);
		}
		
		
		
		//----- when not touched
		if(!bubble->bTouched) {
			
			if(bAnimateIn){
				
				if(bubble->alpha <= 255) bubble->alpha += 10;
				
				// inner bubbles do not seek targets
				if(bubble->buzzID != BUZZ_TYPE_BUBBLE_IN) 
					bubble->gotoTarget();
				
				// inner bubbles fall to bottom
				if(bubble->buzzID == BUZZ_TYPE_BUBBLE_IN)
					bubble->rigidBody->body->applyCentralForce(btVector3(0,60,0));
				
			}
		
		}
			
		
		//--- animate out
		if(bAnimateOut && !bDoneAnimatingOut) {
				
			// move off screen
			bubble->goOffScreen();
				
			// dead containers should count as offscreen
			if(bubble->getPosition().y > 0 && bubble->bAlive) {
				bAllOffScreen = false;
			}
				
		}
			
		
		
		
		// always constrain container structure to rigid body bubbles
		if(bubble->buzzID == BUZZ_TYPE_CONTAINER)
		{
			((BuzzContainerBubble*)bubble)->updateConstraint();
		}
		
		
		bubble->update();
		
		champagne(bubble->pos);
		
		//---- check kill removed bubbles
		if(bubble->buzzID == BUZZ_TYPE_BUBBLE_OLD){
			
			if(bubble->bAlive && bubbles[i]->getPosition().y < -90){
				bubble->bAlive = false;
				ofVec3f startPos = bubble->rigidBody->getPosition();
				setCollisionFilter(bubble->rigidBody,COL_NOTHING,collidesWithNone,startPos);
			}
		}


	}	
	
	
	//--- animate out check
	if(bAllOffScreen && bAnimateOut){
		bDoneAnimatingOut = true;
		for(int i=0; i<bubbles.size(); i++) {
			bubbles[i]->rigidBody->body->setActivationState(DISABLE_SIMULATION);
		}
		
	}
	
	
}

//--------------------------------------------------------
void InteractionBuzz::drawContent(){
	
	
	 for(int i=0; i<bubbles.size(); i++){
		if(bubbles[i]->bAlive){
		 if(bubbles[i]->buzzID == BUZZ_TYPE_CONTAINER)
		 {
			//((BuzzContainerBubble*)bubbles[i])->globe->drawChildren();
		 }
		}
	 }
	 
	
	for(int i=0; i<bubbles.size(); i++) {
		if(bubbles[i]->bAlive){
			bubbles[i]->drawHighLight();
			if(bubbles[i]->buzzID != BUZZ_TYPE_CONTAINER) bubbles[i]->drawTwitterData();
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
	
	for(int i=0; i<bubbles.size(); i++) 
	{
		bubbles[i]->offScreenTaget = bubbles[i]->rigidBody->getPosition();
		bubbles[i]->offScreenTaget.y = -300;
	}
	
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
	
	if(!bSetCallback)
	{
	btOverlapFilterCallback * filterCallback = new BuzzFilterCallback(); 
	bullet->world->getPairCache()->setOverlapFilterCallback(filterCallback);
	bSetCallback = true;
	}
}

//--------------------------------------------------------
void InteractionBuzz::killallBubbles() {
	
	for (int i=0; i<bubbles.size(); i++) {
		if(bubbles[i]->buzzID == BUZZ_TYPE_CONTAINER)
		{
			((BuzzContainerBubble*)bubbles[i])->globe->destroy();
			bullet->world->removeConstraint(((BuzzContainerBubble*)bubbles[i])->p2p);
		}
		bubbles[i]->rigidBody->destroy();
		delete bubbles[i];
		bubbles[i] = NULL;
	}
	bubbles.clear();
	
	// also clear types and map
	bubbleToContIndex.clear();
	
	polledData.clear();
}

//--------------------------------------------------------
void InteractionBuzz::doubleTouched(ofVec2f touchpos)
{
	
	// find out who has been popped
	int poppedID = -1;
	
	for(int i=0; i<bubbles.size(); i++)
	{
		if(bubbles[i]->buzzID == BUZZ_TYPE_CONTAINER && bubbles[i]->bAlive)
		{
			ContentBubble * bubble = bubbles[i];
			ofVec2f p1  = touchpos;
			ofVec2f p2  = bubble->rigidBody->getPosition();
			float	dis = p1.distance(p2);
			
			if(dis < bubble->radius + 10.0) {
				poppedID = i;
				break;
			}
			
		}
	}
	
	
	if(poppedID > -1)
	{
		clearContainer(poppedID);
		clearOutBubbles();
		releaseInBubbles(poppedID);
		
	}else{
		
		
		for(int i=0; i<bubbles.size(); i++) {
			
			if(bubbles[i]->buzzID != BUZZ_TYPE_BUBBLE_OUT) continue;
			
			ContentBubble * bubble = bubbles[i];
			ofVec2f p1  = touchpos;
			ofVec2f p2  = bubble->rigidBody->getPosition();
			float	dis = p1.distance(p2);
			
			if(dis < bubble->radius + 10.0) {
				bubble->doubleTouched();
				printf("hit this bubble: %p\n", bubble);
				break;
			}
		}
		
	}
	
}

//--------------------------------------------------------
void InteractionBuzz::createMomAndChildBubbles()
{
	if(polledData.size() <= 0) return;
	
	if(bubbles.size()<=0) momNowID = 0;
	
	// create container and children
	createMomBubble(momPollGroup);
	
	// start adding her children
	BuzzContainerBubble* mom = (BuzzContainerBubble*)bubbles[momNowID];
	
	int numChildren  = polledData.size();
	
	// base size on how many we have
	float cRad		= CONTAINER_RADIUS;
	float volCont	= (( 4.0/3.0)*PI )*(cRad*cRad*cRad);
	float volBubb	= volCont / (numChildren * 5.f);	
	float radius	= volBubb / (( 4.0/3.0)*PI );
	radius			= pow(radius, 1.0f/3.0f);
	
	// create inner content bubbles
	for( int i = 0; i < numChildren; i++)
	{
		createChildBubble( momNowID, polledData[i],  radius);
	}
	
	// new id
	momNowID = bubbles.size(); 
	
	// clear saved data
	polledData.clear();
	
}

//--------------------------------------------------------
void InteractionBuzz::createMomBubble(string group){
	
	cout << "Buzz: adding new container bubble" << endl;
	
	momPollGroup = group;
	
	ofVec3f center(interactiveRect.width/2,interactiveRect.height/2, 0);
	ofVec3f startPos(center.x + ofRandom(-500, 500), interactiveRect.height+300, ofRandom(-100, 100) );
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(startPos.x/SCALE, startPos.y/SCALE, startPos.z/SCALE));
	
	float	radius = CONTAINER_RADIUS+20;
	
	BuzzContainerBubble * container = new BuzzContainerBubble();
	
	container->buzzID			= BUZZ_TYPE_CONTAINER;
	container->pollingGroup		= group;
	container->radius			= radius;
	
	container->rigidBody		= new ofxBulletRigidBody();
	container->rigidBody->world = bullet->world;
	container->rigidBody->shape = new btSphereShape(radius/SCALE);
	container->rigidBody->createRigidBody(1, startTransform);
	
	container->target.set(center.x + ofRandom(-200, 200), center.y+ ofRandom(-100,100), 0);
	container->targetForce = 5;
	container->offScreenTaget.x = container->target.x;
	container->offScreenTaget.y = -300;
	
	container->createContainerBubble(bullet,startPos);
	
	bullet->world->addRigidBody(container->rigidBody->body, COL_CONTAINER, contCollidesWith);
	bullet->rigidBodies.push_back(container->rigidBody);
	
	bullet->world->addRigidBody(container->globe->body, COL_CONT_FRAME,momNowID);
	bullet->rigidBodies.push_back(container->globe);
	
	bubbles.push_back(container);
}

//--------------------------------------------------------
void InteractionBuzz::createChildBubble(int momID, Donk::BubbleData * data, float radius)
{
	BuzzContainerBubble* mom = (BuzzContainerBubble*)bubbles[momID];
	
	float hRad = radius *.5;
	ofVec3f momPos = mom->rigidBody->getPosition();
	ofVec3f startPos = ofVec3f(momPos.x+ofRandom(-hRad,hRad),momPos.y+ofRandom(-hRad,hRad),momPos.z+ofRandom(-hRad,hRad));
	
	ContentBubble * bubble = new ContentBubble();
	
	bubble->data	  = data;
	bubble->radius    = radius;
	bubble->originalRadius = radius;
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(startPos.x/SCALE, startPos.y/SCALE, startPos.z/SCALE));
	
	bubble->rigidBody = new ofxBulletRigidBody();
	bubble->rigidBody->world = bullet->world;
	bubble->rigidBody->shape = new btSphereShape(radius/SCALE);
	bubble->rigidBody->createRigidBody(1, startTransform);
	
	//bullet->world->addRigidBody(bubble->rigidBody->body, COL_BUBBLE_IN, bubbleInCollidesWith);
	bullet->world->addRigidBody(bubble->rigidBody->body, COL_CONT_FRAME, momID);//contFrameCollidesWith);
	bullet->rigidBodies.push_back(bubble->rigidBody);
	
	bubble->createContentBubble();
	bubble->target = momPos;
	bubble->targetForce = 20.f;
	
	bubble->offScreenTaget.x = bubble->target.x;
	bubble->offScreenTaget.y = -300;
	
	bubbles.push_back(bubble);
	
	bubble->buzzID = BUZZ_TYPE_BUBBLE_IN;
	
	// record types and mom index
	bubbleToContIndex.insert( pair<int,int>(bubbles.size()-1,momID) );
	
	mom->nChildren++;
}

//--------------------------------------------------------
void InteractionBuzz::clearContainer( int index ){

	//if(bubbles[index]->buzzID != BUZZ_TYPE_CONTAINER) return;
	
	BuzzContainerBubble* bubble = (BuzzContainerBubble*)bubbles[index];
	bubble->pop();
	
	// make sure container has no influence or collisions
	ofVec3f startPos = bubble->rigidBody->getPosition();
	startPos.y = -300;
	setCollisionFilter(bubble->rigidBody,COL_NOTHING,collidesWithNone,startPos);
	//bubble->rigidBody->body->setCollisionFlags(bubble->rigidBody->body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	
	bullet->world->removeRigidBody(bubble->rigidBody->body);
	//bubble->target.x = bubble->rigidBody->getPosition().x;
	//bubble->target.y = -300;
}

//--------------------------------------------------------
void InteractionBuzz::clearOutBubbles()
{
	for(int i=0; i<bubbles.size(); i++)
	{
		if(bubbles[i]->buzzID == BUZZ_TYPE_BUBBLE_OUT)
		{
			bubbles[i]->target.x = bubbles[i]->rigidBody->getPosition().x;
			bubbles[i]->target.y = -300;
			bubbles[i]->buzzID = BUZZ_TYPE_BUBBLE_OLD;
		}
	}
}
//--------------------------------------------------------
void InteractionBuzz::releaseInBubbles(int poppedID)
{
	map <int, int> :: const_iterator it;
	
	for(int i=0; i<bubbles.size(); i++)
	{
		ContentBubble * bubble = bubbles[i];
		if(bubble->buzzID == BUZZ_TYPE_BUBBLE_IN)
		{
						
			it =  bubbleToContIndex.find(i);
			
			if( it != bubbleToContIndex.end() && it->second == poppedID )
			{
				bubble->buzzID  = BUZZ_TYPE_BUBBLE_OUT;
				bubble->target = bubble->rigidBody->getPosition();
				bubble->target.x += ofRandom(-200, 200);
				bubble->target.y += ofRandom(-200, 200);
				bubble->targetForce = 10.f;
				
				bubble->rigidBody->body->clearForces();
				ofVec3f startPos = bubble->rigidBody->getPosition();
				setCollisionFilter(bubble->rigidBody,COL_BUBBLE_OUT,bubbleOutCollidesWith,startPos);				
			}
		}
	}
}

//--------------------------------------------------------
void InteractionBuzz::setCollisionFilter(ofxBulletRigidBody * rigidBody, int filter, int mask, ofVec3f startPos)
{
	//ofVec3f startPos = rigidBody->getPosition();
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(startPos.x, startPos.y, startPos.z));
	
	bullet->world->removeRigidBody(rigidBody->body);
	rigidBody->createRigidBody(1, startTransform);
	
	bullet->world->addRigidBody(rigidBody->body, filter, mask);
	
}

