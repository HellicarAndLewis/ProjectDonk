/*
 *  ofxBulletRigidBody.h
 *  Bullet
 *
 *  Created by Todd Vanderlin on 2/5/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofxBulletUtils.h"
#include "btBulletDynamicsCommon.h"

class ofxBulletRigidBody {
	
	
	
public:
	
	bool						bDestroy;
	float						radius;
	btVector3					boxSize;
	int							bodyType;
	btDynamicsWorld			*	world;
	btRigidBody				*	body;
	btConvexInternalShape	*	shape;
	btPolyhedralConvexAabbCachingShape * polyShape;
	ofxBulletRigidBody();
	~ofxBulletRigidBody();
	
	void destroy() {
		
		bDestroy = true;
		
		if(isBody()) {
			
			
			world->removeRigidBody(body);
			delete body;
			delete shape;
			
			body = NULL;
			shape = NULL;
			
			printf("~ofxBulletRigidBody()\n");
		}

	}
	
	ofVec3f getBulletPosition();
	ofVec3f getPosition();
	void	setPosition(ofVec3f pos, ofVec3f rotDir, float degree) {
		
		/*
		//	cout << "only for kinematic body...?" << endl;
		//	assert(bodyType == KINEMATIC_BODY);
		btTransform currentPos;
		currentPos.setIdentity();	
		currentPos.setOrigin(btVec3Scaled(pos));
		
		//if (degree != 0) {
			//currentPos.setRotation(q);
		//}
		
		MyKinematicMotionState* myMotionState = (MyKinematicMotionState*)body->getMotionState();
		myMotionState->setKinematicPos(currentPos);
		
		*/
		
		
		if(!isBody()) return;
		
		/*btTransform transform = body->getCenterOfMassTransform();
		transform.setIdentity();
		transform.setOrigin(btVec3Scaled(pos));

		btQuaternion q;
		q.setRotation(tobtVec3(rotDir), ofDegToRad(degree));
		transform.setRotation(q);
		*/
		
		body->getWorldTransform().setOrigin(btVec3Scaled(pos));
		
		//MyKinematicMotionState* myMotionState = (MyKinematicMotionState*)body->getMotionState();
		//myMotionState->setKinematicPos(transform);
		//m_pRigidBody->getMotionState()->setWorldTransform( newTransform ); 
		
		//body->getMotionState()->setInterpolationWorldTransform(transform);
		//body->setCenterOfMassTransform(transform);
		
		
	}
	
	//void createSphere(btTransform startTrans, int radius, int mass);
	//void createBox();
	
	void createRigidBody(int mass, const btTransform startTrans);
	void create(btDynamicsWorld	* world);
	bool isBody();
	void draw();
	
	
	
};