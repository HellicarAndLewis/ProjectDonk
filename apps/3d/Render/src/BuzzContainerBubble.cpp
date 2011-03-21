/*
 *  BuzzContainerBubble.cpp
 *  Render
 *
 *  Created by Chris on 3/8/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#include "BuzzContainerBubble.h"
#include "Mode.h"

BuzzContainerBubble::BuzzContainerBubble(){
	
	ContentBubble::ContentBubble();
	nChildren = 0;
	pollingGroup = "";
}


void BuzzContainerBubble::createContainerBubble(ofxBullet * bullet, ofVec3f _pos){
	
	ContentBubble::createContentBubble();
	
	globe = new ofxBulletCompoundBody();
	globe->world = bullet->world;
	globe->compoundShape = new btCompoundShape();
	
	int size = 80;
	
	btTransform startTrans;
	startTrans.setIdentity();
	startTrans.setOrigin(btVector3(_pos.x,_pos.y,_pos.z) );// btVector3(0,0,0));
	
	btVector3 btSize;
	btSize.setX( 200.f );
	btSize.setY( 1.f );//btSize.setY( 30.f );
	btSize.setZ( 200.f );
	
	btVector3 up(0, 1, 0);
	ofVec3f position = ofVec3f(0,0,0);//_pos;
	
	// create the sphere:
	for (int i = 1; i < 12; i++){
		for (int j = 0; j < 12; j++){
			
			btScalar cx = position.x + (cos(i) * sin(j) * btSize.getX() );
			btScalar cy = position.y + (cos(j) * btSize.getX());
			btScalar cz = position.z + (sin(i) * sin(j) * btSize.getX());
			
			btVector3 p(cx, cy, cz);
			btVector3 dist = p - ofVec3fToBtVec(position);
			dist.normalize();
			
			float theta = acos(dist.dot(up));
			btVector3 cross = up.cross(dist);
			cross.normalize();
			btQuaternion quat(cross, theta);
			
			btTransform t;
			t.setIdentity();
			t.setOrigin(p);
			t.setRotation(quat);
			
			btCollisionShape * spherePart = new btBoxShape(btVector3(size, 1, size));
			globe->compoundShape->addChildShape(t, spherePart);	
			
		}
	}
	
	globe->createCompoundBody(20, startTrans);
	
	// constraint to rigidBody
	//btVector3 localPivot = rigidBody->body->getCenterOfMassPosition();
	//p2p = new btPoint2PointConstraint(*globe->body, localPivot);
	//bullet->world->addConstraint(p2p);

}

void BuzzContainerBubble::pop(){
	
	if(bAlive)
	{
		bAlive = false;
		
		int nChildren = globe->compoundShape->getNumChildShapes();
		
		for( int i = nChildren-1; i >= 0; i--)
		{
			globe->compoundShape->removeChildShapeByIndex(i);
		}
		
	}
}


void BuzzContainerBubble::updateConstraint() {
	
	
	if(bAlive && rigidBody->isBody())
	{
		
		btVector3 rigidOrigin	= rigidBody->body->getWorldTransform().getOrigin();
		ofVec3f rigidPos		= ofVec3f(rigidOrigin.x(),rigidOrigin.y(),rigidOrigin.z() );
		
		btTransform globeTrans	= globe->body->getWorldTransform();
		btVector3 globeOrigin	= globeTrans.getOrigin();
		ofVec3f globePos		= ofVec3f( globeOrigin.x(),globeOrigin.y(),globeOrigin.z() );
		
		globe->body->setDamping(0.999, 0.99);
		
		ofVec3f frc = rigidPos - globePos;
		distanceToTarget = frc.length();
		float d = ABS(distanceToTarget) * 1;
		d *= 20000; // um this is a crazy big number... but works...
		frc.normalize();
		frc *= d;
		
		globe->body->clearForces();
		globe->body->applyCentralForce(btVector3(frc.x, frc.y, frc.z));
		
		//btVector3 localPivot = rigidBody->body->getCenterOfMassPosition();
		//p2p->setPivotB(localPivot);
	}
	

}



