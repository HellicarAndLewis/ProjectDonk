/*
 *  ofxBulletHollowSphere.cpp
 *  Bullet
 *
 *  Created by base on 12/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxBulletHollowSphere.h"

ofxBulletHollowSphere::ofxBulletHollowSphere()
{}

void ofxBulletHollowSphere::create(btDiscreteDynamicsWorld* world, float radius, ofVec3f position, int resolution = 40)
{
	
	// Not sure what is going on here...
	// Need a update from josh 
	
	
	
	/*m_world = world;
	
	btVector3 btSize;
	btSize.setX( 50.f );
	btSize.setY( 1.f );//btSize.setY( 30.f );
	btSize.setZ( 50.f );
	
	btCollisionShape* shape = new btBoxShape( btSize );
	
	btVector3 localInertia( 0, 0, 0 );
	//shape->calculateLocalInertia(1.f,localInertia);
	
	btVector3 up(0, 1, 0);
	
	// create the sphere:
	for (int i = 1; i < 24; i++){
		for (int j = 0; j < 24; j++){
			
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
			
			btDefaultMotionState* motionstate = new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo cInfo(1.f, motionstate, shape, localInertia);
			btRigidBody* spherePart = new btRigidBody(cInfo);
			
			// containing bubbles have upwards gravity
			spherePart->setCollisionFlags(1);
			
			ofxBulletRigidBody* ofxbody = new ofxBulletRigidBody(spherePart);
			
			m_children.push_back(ofxbody);
			m_world->addRigidBody(spherePart);
			//spherePart->activate(true);
			
		}
	}*/
	
}

void ofxBulletHollowSphere::translate(ofVec3f position)
{}

void ofxBulletHollowSphere::rotate(ofVec3f position)
{}

void ofxBulletHollowSphere::setCollisionFlags()
{}

void ofxBulletHollowSphere::draw()
{
	/*vector<ofxBulletRigidBody*>::iterator it = m_children.begin();
	while( it != m_children.end())
	{
		const btVector3 pos = (*it)->getCenterOfMassPosition();
		ofCircle(pos.getX(), pos.getY(), (200 - pos.getZ())/50); // totally faking 3d
																 //btTransform t = it->getWorldTransform();
																 //btQuaternion q = t.getRotation();
		++it;
	}*/
}