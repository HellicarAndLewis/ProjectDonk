/*
 *  ofxBullet.h
 *  Bullet
 *
 *  Created by Todd Vanderlin on 2/5/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxBulletUtils.h"
#include "ofxBulletRigidBody.h"


class ofxBullet {
	

public:
	
	ofxBulletRigidBody					* ground;
	//btCollisionShape					* groundShape;
	
	btDefaultCollisionConfiguration		* collisionConfiguration;
	btCollisionDispatcher				* dispatcher;
	btBroadphaseInterface				* overlappingPairCache;
	btSequentialImpulseConstraintSolver * solver;
	btDiscreteDynamicsWorld				* world;
	
	
	ofxBullet();
	~ofxBullet();
	
	void init();
	void destroy();
	void update();
	void draw();
	void drawFloor();
	
	void createGround(float size);
	void addBody(ofxBulletRigidBody *b);
	
	ofxBulletRigidBody * createSphere(ofVec3f pos, float radius, float mass);

	ofxBulletRigidBody * createBox(ofVec3f pos, ofVec3f size, float mass);
	ofxBulletRigidBody * createCone(ofVec3f pos, float radius, float height, float mass);
	
	vector <ofxBulletRigidBody*> rigidBodies;

};