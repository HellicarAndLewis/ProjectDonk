/*
 *  ofxBulletRigidBody.h
 *  Bullet
 *
 *  Created by Todd Vanderlin on 2/5/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#define SCALE 30
#include "ofMain.h"
#include "btBulletCollisionCommon.h"
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
		
		
		/*
		int i;
		for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject( obj );
			delete obj;
		}
		
		//delete collision shapes
		for (int j=0;j<m_collisionShapes.size();j++)
		{
			btCollisionShape* shape = m_collisionShapes[j];
			delete shape;
		}*/
		
	}
	
	ofVec3f getPosition();
	
	//void createSphere(btTransform startTrans, int radius, int mass);
	//void createBox();
	
	void createRigidBody(int mass, const btTransform startTrans);
	void create(btDynamicsWorld	* world);
	bool isBody();
	void draw();
	
	
	
	
	
};