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
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "ofxBulletBaseConstraint.h"
#include "ofxBulletRBtoRBAttractor.h"
#include "ofxBulletHollowSphere.h"

struct callback : public btOverlapFilterCallback
{
	// return true when pairs need collision
	bool	needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const;
	bool	(*collFunction)(void* obj1ToCompare, void* obj2ToCompare);
};


class ofxBullet {
	

public:
	
	ofCamera							* camera;
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
	
	void createCollisionFlag(bool	(*collFunction)(void* obj1ToCompare, void* obj2ToCompare));
	
	void createGround(float size);
	void addBody(ofxBulletRigidBody *b);
	
	//btGeneric6DofSpringConstraint * addSpring(btRigidBody* pBodyA,btRigidBody* pBodyB, float springLen);

	ofxBulletRigidBody * createSphere(ofVec3f pos, float radius, float mass);
	ofxBulletRigidBody * createBox(ofVec3f pos, ofVec3f size, float mass);
	ofxBulletRigidBody * createCone(ofVec3f pos, float radius, float height, float mass);
	
	vector <ofxBulletRigidBody*> rigidBodies;

	
	// -------------------------------------------------
	// -------------------------------------------------
	// -------------------------------------------------
	
	
	btVector3 getRayTo(int x, int y, ofCamera * cam);
	void mousePressed(int x, int y);
	void mouseReleased(int x, int y);
	void mouseDragged(int x, int y);
	
	
	//-----------
	/*
	 Not sure about this model yet for attraction. Trying out if its comforta
	 attraction, repulsion, constraints are managed by ofxBullet rather than 
	 */
	ofxBulletBaseConstraint * addAttractor(ofxBulletRigidBody * a, ofxBulletRigidBody * b, float force);
	
	// checks all attractions etc, destroys dead ones and updates others
	void solveConstraints();
	
	// clears all constraints of any type
	void clearConstraints();
	
	// clear all attractions for body a
	void clearAttractions(ofxBulletRigidBody * a);
	
	// vector of all constraints
	vector<ofxBulletBaseConstraint *> constraints;
	//------------------
	
	
	
};