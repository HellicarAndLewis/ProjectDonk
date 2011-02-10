/*
 *  ofxBullet.cpp
 *  Bullet
 *
 *  Created by Todd Vanderlin on 2/5/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBullet.h"

static bool removeRigidBody(ofxBulletRigidBody * b) {
	bool remove = true;
	if(b!=NULL) {
		remove = b->bDestroy;
		if(remove) {
			b->destroy();
			delete b;
			b = NULL;
		}
	}
	return remove;
}

ofxBullet::ofxBullet() {
	world = NULL;
	ground = NULL;
}

//--------------------------------------------------------------
ofxBullet::~ofxBullet() {
	destroy();
}

//--------------------------------------------------------------
void ofxBullet::init() {
	
	// collision configuration contains default setup 
	// for memory, collision setup. Advanced users 
	// can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();
	
	// use the default collision dispatcher. For 
	// parallel processing you can use a diffent 
	// dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	
	// btDbvtBroadphase is a good general purpose 
	// broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();
	
	// the default constraint solver. For parallel 
	// processing you can use a different solver
	// (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;
	world  = new btDiscreteDynamicsWorld(dispatcher,
										 overlappingPairCache,
										 solver,
										 collisionConfiguration);
}

//--------------------------------------------------------------
void ofxBullet::destroy() {
	
	
	if(world == NULL) return;

	
	// cleanup in the reverse order of creation/initialization	
	// remove the bodies from the dynamics world and delete them
	for (int i = world->getNumCollisionObjects()-1; i >= 0 ; i--) {
		
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		
		if (body) {
			if(body->getMotionState()) {
				delete body->getMotionState();
			}
		}
		
		world->removeCollisionObject( obj );
		delete obj;
		
	}
/*
	}
	
	for (int i = 0; i < rigidBodies.size(); i++) {
		btCollisionShape* shape = rigidBodies[i]->shape;
		delete shape;
		
	}
*/
	
	/*delete world;
	delete solver;
	delete overlappingPairCache;
	delete dispatcher;
	delete collisionConfiguration;	
	*/
}


//--------------------------------------------------------------
void ofxBullet::update() {
	
	if(world == NULL) return;
	float dt = ofGetElapsedTimeMillis() * 0.0001f;
	world->stepSimulation( dt );
	
	
	rigidBodies.erase(rigidBodies.begin(), partition(rigidBodies.begin(), rigidBodies.end(), removeRigidBody));
	
}

//--------------------------------------------------------------
void ofxBullet::draw() {
	
	glPushMatrix();
	glTranslated(0, 0, 0);
	glScalef(SCALE, SCALE, SCALE);
	
	for	(int i=0; i<rigidBodies.size(); i++) {
		ofSetColor(255, 0, 255);
		rigidBodies[i]->draw();
	}
	
	if (ground) {
		ofSetColor(100, 100, 100);
		ground->draw();
	}
	
	glPopMatrix();
	
}

//--------------------------------------------------------------
void ofxBullet::addBody(ofxBulletRigidBody * b) {
	
	if(b == NULL) return;
	
}

//--------------------------------------------------------------
void ofxBullet::createGround() {
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 0, 0));
	
	ground = new ofxBulletRigidBody();
	ground->world = world;
	
	ground->shape = new btBoxShape(btVector3(btScalar(250.0/SCALE), 
											 btScalar(10.0/SCALE), 
											 btScalar(250.0/SCALE)));

	//b->shape = new btSphereShape(radius/SCALE);
	ground->createRigidBody(0, startTransform);
	
	world->addRigidBody(ground->body);
	//rigidBodies.push_back(ground);
}


//--------------------------------------------------------------
ofxBulletRigidBody * ofxBullet::createSphere(ofVec3f pos, float radius, float mass) {
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(pos.x/SCALE, pos.y/SCALE, pos.z/SCALE));
	
	ofxBulletRigidBody * b = new ofxBulletRigidBody();
	b->world = world;
	b->shape = new btSphereShape(radius/SCALE);
	b->createRigidBody(mass, startTransform);
	
	world->addRigidBody(b->body);
	rigidBodies.push_back(b);
	
	return b;
}


//--------------------------------------------------------------
ofxBulletRigidBody * ofxBullet::createBox(ofVec3f pos, ofVec3f size, float mass) {
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(pos.x/SCALE, pos.y/SCALE, pos.z/SCALE));
	
	ofxBulletRigidBody * b = new ofxBulletRigidBody();
	b->world = world;
	
	size /= SCALE;
	b->shape = new btBoxShape(btVector3(size.x, size.y, size.z));
	b->createRigidBody(mass, startTransform);
	
	world->addRigidBody(b->body);
	rigidBodies.push_back(b);
	
	return b;
}









