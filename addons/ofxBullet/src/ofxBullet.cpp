/*
 *  ofxBullet.cpp
 *  Bullet
 *
 *  Created by Todd Vanderlin on 2/5/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBullet.h"


bool callback::needBroadphaseCollision(btBroadphaseProxy* proxy0, btBroadphaseProxy* proxy1) const
{
	
	if( proxy0->m_collisionFilterGroup == 2 && proxy1->m_collisionFilterGroup == 2) {
		return false;
	}
	
	return true;
}

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
	world  = NULL;
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
	
	if(world != NULL) {
		printf("--- Bullet World Created ---\n");	
	}
}

//--------------------------------------------------------------
void ofxBullet::setGravity(ofVec3f &g) {
	setGravity(g.x, g.y, g.z);
}

//--------------------------------------------------------------
void ofxBullet::setGravity(float x, float y, float z) {
	if(world != NULL) {
		world->setGravity(btVector3(x, y, z));	
	}
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
	float dt = ofGetElapsedTimeMillis() * 0.00001f;
	world->stepSimulation( dt );
	
	
	rigidBodies.erase(rigidBodies.begin(), partition(rigidBodies.begin(), rigidBodies.end(), removeRigidBody));
	
}

//--------------------------------------------------------------
void ofxBullet::draw() {
	
	//glPushMatrix();
	//glTranslated(0, 0, 0);
	//glScalef(SCALE, SCALE, SCALE);
	
	for	(int i=0; i<rigidBodies.size(); i++) {
		ofSetColor(255, 0, 255);
		rigidBodies[i]->draw();
	}
	
	//glPopMatrix();
	
}

//--------------------------------------------------------------
void ofxBullet::drawFloor() {
	//glPushMatrix();
	//glTranslated(0, 0, 0);
	//glScalef(SCALE, SCALE, SCALE);
	if (ground) {
		ground->draw();
	}
	//glPopMatrix();
}

//--------------------------------------------------------------
void ofxBullet::addBody(ofxBulletRigidBody * b) {
	
	if(b == NULL) return;
	
}

//--------------------------------------------------------------
void ofxBullet::createGround(float size) {
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 0, 0));
	
	ground = new ofxBulletRigidBody();
	ground->world = world;
	
	ground->shape = new btBoxShape(btVector3(btScalar(size/SCALE), 
											 btScalar(2.0/SCALE), 
											 btScalar(size/SCALE)));
	
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


//--------------------------------------------------------------
ofxBulletRigidBody * ofxBullet::createCone(ofVec3f pos, float radius, float height, float mass) {
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(pos.x/SCALE, pos.y/SCALE, pos.z/SCALE));
	
	ofxBulletRigidBody * b = new ofxBulletRigidBody();
	b->world = world;
	b->shape = new btConeShape(radius/SCALE, height/SCALE);
	b->createRigidBody(mass, startTransform);
	
	world->addRigidBody(b->body);
	rigidBodies.push_back(b);
	
	return b;
}




//--------------------------------------------------------------
//---------------- NEED TO CLEAN UP ----------------------
//-------------------RIP FROM BULLET---------------------------------
//--------------------------------------------------------------


//--------------------------------------------------------------
btVector3 ofxBullet::getRayTo(int x, int y, ofCamera * cam) {
	
	
	y = ofMap(y, 0, ofGetHeight(), ofGetHeight(), 0);
	float adjustx = fabs((ofGetWidth()/2)-x);
	float adjusty = fabs((ofGetHeight()/2)-y);
	adjustx = ofMap(adjustx, 0, ofGetWidth()/2, 0, ofGetWidth()/5);
	adjusty = ofMap(adjusty, 0, ofGetHeight()/2, 0, ofGetHeight()/5);
	if (x > ofGetWidth()/2) adjustx = -adjustx;
	if (y > ofGetHeight()/2) adjusty = -adjusty;
	x += adjustx; y+= adjusty;
	
	
	float top = 1.f;
	float bottom = -1.f;
	float nearPlane = 1.f;
	float tanFov = (top-bottom)*0.5f / nearPlane;
	float fov = 2.0 * atanf (tanFov);
	
	ofVec3f campos = cam->getGlobalPosition();
	
	btVector3 rayFrom = btVector3(campos.x, campos.y, campos.z);
	ofVec3f dir = cam->getLookAtDir();
	btVector3 rayForward = btVector3(dir.x, dir.y, dir.z);
	rayForward.normalize();
	float farPlane = 10000.f;
	rayForward *= farPlane;
	
	btVector3 rightOffset;
	ofVec3f camup = cam->getUpDir();
	btVector3 vertical = btVector3(camup.x, camup.y, camup.z);
	
	btVector3 hor;
	hor = rayForward.cross(vertical);
	hor.normalize();
	vertical = hor.cross(rayForward);
	vertical.normalize();
	vertical = -vertical;
	
	float tanfov = tanf(0.5f*fov);
	
	hor *= 2.f * farPlane * tanfov;
	vertical *= 2.f * farPlane * tanfov;
	
	btScalar aspect;
	
	if (ofGetWidth() > ofGetHeight()) {
		aspect = ofGetWidth() / (btScalar)ofGetHeight();
		hor*=aspect;
	} else 	{
		aspect = ofGetHeight() / (btScalar)ofGetWidth();
		vertical*=aspect;
	}
	
	
	btVector3 rayToCenter = rayFrom + rayForward;
	btVector3 dHor = hor * 1.f/float(ofGetWidth());
	btVector3 dVert = vertical * 1.f/float(ofGetHeight());
	
	
	btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * vertical;
	rayTo += x * dHor;
	rayTo -= y * dVert;
	return rayTo;
}


//--------------------------------------------------------------
btScalar mousePickClamp = 30.f;
btVector3 hitPos(-1,-1,-1);
float oldPickingDist  = 0.f;
btVector3 oldPickingPos;
btTypedConstraint*						m_pickConstraint;		// constraint for mouse picking
btRigidBody*							pickedBody;	

void ofxBullet::mousePressed(int x, int y){
	
	if (world != NULL) {
		
		btVector3 rayTo   = getRayTo(x, y, camera);
		ofVec3f campos    = camera->getGlobalPosition();
		btVector3 rayFrom = btVector3(campos.x, campos.y, campos.z);
		
		btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom,rayTo);
		world->rayTest(rayFrom, rayTo, rayCallback);
		
		
		if (rayCallback.hasHit()) {
			
			btRigidBody * body = btRigidBody::upcast(rayCallback.m_collisionObject);
			
			if (body) {
				
				
				//other exclusions?
				if (!(body->isStaticObject() || body->isKinematicObject())) {
					
					
					printf("Hit\n");
					
					pickedBody = body;
					pickedBody->setActivationState(DISABLE_DEACTIVATION);
					
					
					btVector3 pickPos = rayCallback.m_hitPointWorld;
					printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
					
					
					btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
					
					btPoint2PointConstraint* p2p = new btPoint2PointConstraint(*body,localPivot);
					p2p->m_setting.m_impulseClamp = oldPickingDist;
					
					world->addConstraint(p2p);
					m_pickConstraint = p2p;
					
					//save mouse position for dragging
					oldPickingPos = rayTo;
					hitPos = pickPos;
					
					oldPickingDist  = (pickPos-rayFrom).length();
					
					//very weak constraint for picking
					p2p->m_setting.m_tau = 0.1f;
				}
			}
		}	
	}
}

//--------------------------------------------------------------
void ofxBullet::mouseReleased(int x, int y){
	
	if (m_pickConstraint && world) {
		printf("removed");
		world->removeConstraint(m_pickConstraint);
		//delete m_pickConstraint;
		//printf("removed constraint %i",gPickingConstraintId);
		m_pickConstraint = 0;
		pickedBody->forceActivationState(ACTIVE_TAG);
		pickedBody->setDeactivationTime( 0.f );
		pickedBody = 0;
	}
	
}
//--------------------------------------------------------------
void ofxBullet::mouseDragged(int x, int y){
	
	if (m_pickConstraint) {
		//move the constraint pivot
		btPoint2PointConstraint* p2p = static_cast<btPoint2PointConstraint*>(m_pickConstraint);
		
		if (p2p) {
			//keep it at the same picking distance
			
			btVector3 newRayTo = getRayTo(x, y, camera);
			btVector3 rayFrom;
			btVector3 oldPivotInB = p2p->getPivotInB();
			btVector3 newPivotB;
			
			ofVec3f campos = camera->getGlobalPosition();
			rayFrom = btVector3(campos.x, campos.y, campos.z);
			btVector3 dir = newRayTo-rayFrom;
			dir.normalize();
			dir *= oldPickingDist;
			
			newPivotB = rayFrom + dir;
			
			p2p->setPivotB(newPivotB);
		}
		
	} else {
		mousePressed(x, y);
	}
}





