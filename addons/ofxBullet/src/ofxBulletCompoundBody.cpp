/*
 *  ofxBulletCompoundBody.cpp
 *  Render
 *
 *  Created by Chris on 3/8/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#include "ofxBulletCompoundBody.h"

//--------------------------------------------------------------
ofxBulletCompoundBody::ofxBulletCompoundBody() {
	
	ofxBulletRigidBody::ofxBulletRigidBody();
	compoundShape = NULL;
}

//--------------------------------------------------------------
ofxBulletCompoundBody::~ofxBulletCompoundBody() {
	
}

//--------------------------------------------------------------
void ofxBulletCompoundBody::createCompoundBody(int mass, const btTransform startTrans)
{
	
	btVector3 inertia(0, 0, 0);
	compoundShape->calculateLocalInertia(mass, inertia);
	//shape = new btSphereShape(200);
	
	btMotionState* myMotionState;
	myMotionState = new btDefaultMotionState(startTrans);			
	

	btRigidBody::btRigidBodyConstructionInfo rbci(mass, myMotionState, compoundShape, inertia);
	//rbci.m_startWorldTransform = startTrans;	
	body = new btRigidBody(rbci);
	
	
	
	
}

//--------------------------------------------------------------
void ofxBulletCompoundBody::drawChildren()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	int nChildren = compoundShape->getNumChildShapes();
	//cout << "compound kids: " << nChildren << endl;
	
	btScalar	m[16];
	body->getWorldTransform().getOpenGLMatrix(m);
	btVector3 aabbMin, aabbMax;
	world->getBroadphase()->getBroadphaseAabb(aabbMin, aabbMax);
	
	// ----------------------
	// mult to the body matrix
	// ----------------------
	glPushMatrix(); 
	glTranslatef(0, 0, 0);
	glMultMatrixf(m);
	
	for( int i = 0; i < nChildren; i++)
	{
		btCollisionShape* colShape = compoundShape->getChildShape(i);
		
		int type = colShape->getShapeType();
		
		btScalar	c[16];
		compoundShape->getChildTransform(i).getOpenGLMatrix(c);

		glPushMatrix(); 
		glTranslatef(0, 0, 0);
		glMultMatrixf(c);
		
		
		// ----------------------
		// box
		// ----------------------
		if(type == BOX_SHAPE_PROXYTYPE) {
			
			const btBoxShape* boxShape = static_cast<const btBoxShape*>(colShape);
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
		
		glPopMatrix();
	}
	
	glPopMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

