/*
 *  BuzzContainerBubble.cpp
 *  Render
 *
 *  Created by Chris on 3/8/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#include "BuzzContainerBubble.h"

BuzzContainerBubble::BuzzContainerBubble(){
	
	ContentBubble::ContentBubble();
	nChildren = 0;
}


void BuzzContainerBubble::createContainerBubble(ofxBullet * bullet, ofVec3f _pos){
	
	ContentBubble::createContentBubble();
	
	globe = new ofxBulletCompoundBody();
	globe->world = bullet->world;
	globe->compoundShape = new btCompoundShape();
	
	int size = 80;
	
	btTransform startTrans;
	startTrans.setIdentity();
	startTrans.setOrigin( btVector3(pos.x,pos.y,pos.z) );
	
	btVector3 btSize;
	btSize.setX( 200.f );
	btSize.setY( 1.f );//btSize.setY( 30.f );
	btSize.setZ( 200.f );
	
	btVector3 up(0, 1, 0);
	ofVec3f position = _pos;
	
	// create the sphere:
	for (int i = 1; i < 12; i++){
		for (int j = 0; j < 12; j++){
			
			btScalar cx = position.x + (cos(i) * sin(j) * btSize.getX() );
			btScalar cy = position.y + (cos(j) * btSize.getX());
			btScalar cz = position.z + (sin(i) * sin(j) * btSize.getX());
			
			btVector3 pos(cx, cy, cz);
			btVector3 dist = pos - ofVec3fToBtVec(position);
			dist.normalize();
			
			float theta = acos(dist.dot(up));
			btVector3 cross = up.cross(dist);
			cross.normalize();
			btQuaternion quat(cross, theta);
			
			btTransform t;
			t.setIdentity();
			t.setOrigin(pos);
			t.setRotation(quat);
			
			btCollisionShape * spherePart = new btBoxShape(btVector3(size, 1, size));
			globe->compoundShape->addChildShape(t, spherePart);	
		}
	}
	
	globe->createCompoundBody(10, startTrans);
	
	// constraint to rigidBody
	btVector3 localPivot = rigidBody->body->getCenterOfMassPosition();
	p2p = new btPoint2PointConstraint(*globe->body, localPivot);
	bullet->world->addConstraint(p2p);
	//p2p->m_setting.m_damping = 1.001f;
	//p2p->m_setting.m_impulseClamp = .1;

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
		//btVector3 pickPos = rigidBody->body->getCenterOfMassPosition();
		//printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
		
		
		//btVector3 localPivot = globe->body->getCenterOfMassTransform().inverse() * pickPos;
		btVector3 localPivot = rigidBody->body->getCenterOfMassPosition();
		p2p->setPivotB(localPivot);
	}
	

}



