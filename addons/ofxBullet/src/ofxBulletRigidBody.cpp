/*
 *  ofxBulletRigidBody.cpp
 *  Bullet
 *
 *  Created by Todd Vanderlin on 2/5/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ofxBulletRigidBody.h"

//--------------------------------------------------------------
ofxBulletRigidBody::ofxBulletRigidBody() {
	body  = NULL;
	shape = NULL;
	world = NULL;
	bDestroy = false;
}

//--------------------------------------------------------------
ofxBulletRigidBody::~ofxBulletRigidBody() {
	
}


//--------------------------------------------------------------
void ofxBulletRigidBody::createRigidBody(int mass, const btTransform startTrans) {
	
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);
	
	btMotionState* myMotionState;
	myMotionState = new btDefaultMotionState(startTrans);			
	
	/*switch (bodyType) {
	 case 0:
	 myMotionState = new btDefaultMotionState(startTrans);
	 mass = 0;
	 break;
	 case 1:
	 myMotionState = new btDefaultMotionState(startTrans);
	 break;
	 case 2:
	 myMotionState = new MyKinematicMotionState(startTrans);
	 break;
	 
	 default:
	 myMotionState = new btDefaultMotionState(startTrans);			
	 break;
	 }	
	 */
	
	btRigidBody::btRigidBodyConstructionInfo rbci(mass, myMotionState, shape, inertia);
	rbci.m_startWorldTransform = startTrans;	
	body = new btRigidBody(rbci);
	
	//if (bodyType == 2) {
	//	psb->setCollisionFlags(psb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	//	psb->setActivationState(DISABLE_DEACTIVATION);
	//}
}

/*
 //--------------------------------------------------------------
 void ofxBulletRigidBody::createBox(btTransform startTrans, btVector3 boxShape, int mass) {
 boxSize = boxShape;
 shape = new btBoxShape(boxShape);
 createRigidBody(mass, startTrans);
 }
 
 //--------------------------------------------------------------
 void ofxBulletRigidBody::createSphere(btTransform startTrans, int radius, int mass) {
 this->radius = radius;
 shape = new btSphereShape(sphereRadius);
 createRigidBody(mass, startTrans);	
 }*/

//--------------------------------------------------------------
void ofxBulletRigidBody::create(btDynamicsWorld * world) {
	
	if(world == NULL) return;
	
	this->world = world;
	
	
	float x = btScalar(ofRandom(-1, 1));
	float y = btScalar(ofRandom(-1, -6));
	float z = btScalar(ofRandom(-3, 3));
	
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(x, y, z));
	
	float radius = 1;
	shape = new btSphereShape(radius);	
	btVector3 inertia(0, 0, 0);
	float mass = 3.0;
	shape->calculateLocalInertia(mass, inertia);
	
	btMotionState * myMotionState;
	myMotionState = new btDefaultMotionState(startTransform);
	
	
	/*
	 struct	btRigidBodyConstructionInfo
	 {
	 btScalar			m_mass;
	 
	 ///When a motionState is provided, the rigid body will initialize its world transform from the motion state
	 ///In this case, m_startWorldTransform is ignored.
	 btMotionState*		m_motionState;
	 btTransform	m_startWorldTransform;
	 
	 btCollisionShape*	m_collisionShape;
	 btVector3			m_localInertia;
	 btScalar			m_linearDamping;
	 btScalar			m_angularDamping;
	 
	 ///best simulation results when friction is non-zero
	 btScalar			m_friction;
	 ///best simulation results using zero restitution.
	 btScalar			m_restitution;
	 
	 btScalar			m_linearSleepingThreshold;
	 btScalar			m_angularSleepingThreshold;
	 
	 //Additional damping can help avoiding lowpass jitter motion, help stability for ragdolls etc.
	 //Such damping is undesirable, so once the overall simulation quality of the rigid body dynamics system has improved, this should become obsolete
	 bool				m_additionalDamping;
	 btScalar			m_additionalDampingFactor;
	 btScalar			m_additionalLinearDampingThresholdSqr;
	 btScalar			m_additionalAngularDampingThresholdSqr;
	 btScalar			m_additionalAngularDampingFactor;
	 
	 
	 btRigidBodyConstructionInfo(	btScalar mass, btMotionState* motionState, btCollisionShape* collisionShape, const btVector3& localInertia=btVector3(0,0,0)):
	 m_mass(mass),
	 m_motionState(motionState),
	 m_collisionShape(collisionShape),
	 m_localInertia(localInertia),
	 m_linearDamping(btScalar(0.)),
	 m_angularDamping(btScalar(0.)),
	 m_friction(btScalar(0.5)),
	 m_restitution(btScalar(0.)),
	 m_linearSleepingThreshold(btScalar(0.8)),
	 m_angularSleepingThreshold(btScalar(1.f)),
	 m_additionalDamping(false),
	 m_additionalDampingFactor(btScalar(0.005)),
	 m_additionalLinearDampingThresholdSqr(btScalar(0.01)),
	 m_additionalAngularDampingThresholdSqr(btScalar(0.01)),
	 m_additionalAngularDampingFactor(btScalar(0.01))
	 {
	 m_startWorldTransform.setIdentity();
	 }
	 };
	 */
	
	btRigidBody::btRigidBodyConstructionInfo rbci(mass, myMotionState, shape, inertia);
	
	rbci.m_friction = 0.7;
	rbci.m_restitution = 0.07;
	rbci.m_startWorldTransform = startTransform;	
	body = new btRigidBody(rbci);
	
	// now add it to the world
	world->addRigidBody(body);
	
}

//--------------------------------------------------------------
bool ofxBulletRigidBody::isBody() {
	return body != NULL && world != NULL && shape != NULL;
}

//--------------------------------------------------------------
ofVec3f ofxBulletRigidBody::getPosition() {
	if(!isBody()) return 0;
	btScalar m[16];
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
	btVector3 org(m[12], m[13], m[14]);
	org *= SCALE;
	return ofVec3f(org.x(), org.y(), org.z());
}

//--------------------------------------------------------------
ofVec3f ofxBulletRigidBody::getBulletPosition() {
	if(!isBody()) return 0;
	btScalar m[16];
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
	btVector3 org(m[12], m[13], m[14]);
	return ofVec3f(org.x(), org.y(), org.z());
}

//--------------------------------------------------------------
void ofxBulletRigidBody::draw() {
	
	/*
	 BOX_SHAPE_PROXYTYPE,
	 TRIANGLE_SHAPE_PROXYTYPE,
	 TETRAHEDRAL_SHAPE_PROXYTYPE,
	 CONVEX_TRIANGLEMESH_SHAPE_PROXYTYPE,
	 CONVEX_HULL_SHAPE_PROXYTYPE,
	 CONVEX_POINT_CLOUD_SHAPE_PROXYTYPE,
	 CUSTOM_POLYHEDRAL_SHAPE_TYPE,
	 //implicit convex shapes
	 IMPLICIT_CONVEX_SHAPES_START_HERE,
	 SPHERE_SHAPE_PROXYTYPE,
	 MULTI_SPHERE_SHAPE_PROXYTYPE,
	 CAPSULE_SHAPE_PROXYTYPE,
	 CONE_SHAPE_PROXYTYPE,
	 CONVEX_SHAPE_PROXYTYPE,
	 CYLINDER_SHAPE_PROXYTYPE,
	 UNIFORM_SCALING_SHAPE_PROXYTYPE,
	 MINKOWSKI_SUM_SHAPE_PROXYTYPE,
	 MINKOWSKI_DIFFERENCE_SHAPE_PROXYTYPE,
	 CUSTOM_CONVEX_SHAPE_TYPE,
	 
	 
	 //cout << colShape->getShapeType() << endl;
	 //drawer.drawOpenGL(m, shape, btVector3(1, 0, 0), 0, aabbMin, aabbMax, ofxBulletStaticUtil::ofxVec4ToBtVec4(bodyColor));
	 
	 
	 */
	if(isBody()) {
		
		btPolyhedralConvexShape* colShape = (btPolyhedralConvexShape*)body->getCollisionShape();
		btScalar	m[16];
		body->getWorldTransform().getOpenGLMatrix(m);
		btVector3 aabbMin, aabbMax;
		world->getBroadphase()->getBroadphaseAabb(aabbMin, aabbMax);
		int type = colShape->getShapeType();
		
		
		// ----------------------
		// mult to the body matrix
		// ----------------------
		glPushMatrix(); 
		glTranslatef(0, 0, 0);
		glMultMatrixf(m);
		
		// ----------------------
		// box
		// ----------------------
		if(type == BOX_SHAPE_PROXYTYPE) {
			
			const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
			btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
			static int indices[36] = {
				0,1,2,
				3,2,1,
				4,0,6,
				6,0,2,
				5,1,4,
				4,1,0,
				7,3,1,
				7,1,5,
				5,4,7,
				7,4,6,
				7,2,3,
				7,6,2};
			static btVector3 vertices[8]={	btVector3(1,1,1),btVector3(-1,1,1),	btVector3(1,-1,1),	btVector3(-1,-1,1),	btVector3(1,1,-1),	btVector3(-1,1,-1),	btVector3(1,-1,-1),	btVector3(-1,-1,-1)};
			glBegin (GL_TRIANGLES);
			int si=36;
			for (int i=0;i<si;i+=3) {
				btVector3 v1 = vertices[indices[i]]*halfExtent;
				btVector3 v2 = vertices[indices[i+1]]*halfExtent;
				btVector3 v3 = vertices[indices[i+2]]*halfExtent;
				
				//v1 *= SCALE;
				//v2 *= SCALE;
				//v3 *= SCALE;
				
				btVector3 normal = (v3-v1).cross(v2-v1);
				normal.normalize ();
				
				glNormal3f(normal.getX(),normal.getY(),normal.getZ());
				glVertex3f (v1.x(), v1.y(), v1.z());
				glVertex3f (v2.x(), v2.y(), v2.z());
				glVertex3f (v3.x(), v3.y(), v3.z());
			}
			glEnd();
			
			
		}
		
		// ----------------------
		// Shpere
		// ----------------------
		else if(type == SPHERE_SHAPE_PROXYTYPE) {
			
			const btSphereShape* sphereShape = static_cast<const btSphereShape*>(shape);
			float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
			ofSphere(0, 0, 0, radius);
			
		}
		
		
		// ----------------------
		// end move
		// ----------------------
		glPopMatrix();
		
	}
	
	
	
	
	/*
	 
	 
	 btPolyhedralConvexShape * shape = (btPolyhedralConvexShape*)psb->getCollisionShape();
	 btScalar	m[16];
	 psb->getWorldTransform().getOpenGLMatrix(m);
	 btVector3 aabbMin,aabbMax;
	 dynamicsWorld->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
	 
	 const btSphereShape* sphereShape = (btSphereShape*)shape;//static_cast<const btSphereShape*>(shape);
	 float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
	 //drawSphere(radius,10,10);
	 //useWireframeFallback = false;
	 
	 //btScalar m[16];
	 btDefaultMotionState* myMotionState = (btDefaultMotionState*)psb->getMotionState();
	 myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
	 btVector3 org(m[12], m[13], m[14]);
	 ofPoint p(org.x(), org.y(), org.z());
	 
	 
	 glPushMatrix(); 
	 glMultMatrixf(m);
	 ofSphere(0, 0, 0, radius);
	 glPopMatrix();
	 //drawer.drawOpenGL(m, shape, btVector3(1, 0, 0), 0, aabbMin, aabbMax, ofxBulletStaticUtil::ofxVec4ToBtVec4(bodyColor));
	 */
	
}