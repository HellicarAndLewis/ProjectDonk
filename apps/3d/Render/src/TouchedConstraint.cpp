/*
 *  TouchedConstraint.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 2/28/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "TouchedConstraint.h"



//--------------------------------------------------------
TouchedConstraint::TouchedConstraint() {
	constraint = NULL;
	body	   = NULL;
	world	   = NULL;
	bRemove    = false;
}

TouchedConstraint::~TouchedConstraint() {
	
	if (constraint && world) {
		world->removeConstraint(constraint);
	}
	
	if(constraint) {
		delete constraint;
	}
}

//--------------------------------------------------------
void TouchedConstraint::destroy() {
	if (world != NULL) {
		if(constraint != NULL) {
			world->removeConstraint(constraint);
		
		//	if(constraint->getRigidBodyA()) constraint->getRigidBodyA().removeConstraintRef(constraint);
		//	if(constraint->getRigidBodyB()) constraint->getRigidBodyB().removeConstraintRef(constraint);
		}	
	}
	
	
	if(body) {
		body->forceActivationState(ACTIVE_TAG);
		body->setDeactivationTime( 0.f );
		constraint = NULL;
		body	   = NULL;
		world	   = NULL;
	}
}

//--------------------------------------------------------
void TouchedConstraint::setTouchBody(btDiscreteDynamicsWorld * world, btRigidBody * bd) {
	
	if(!bd) return;
	
	body = bd;
	body->setActivationState(DISABLE_DEACTIVATION);
	
	// get the body world position
	btScalar m[16];
	btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
	
	// set the pick pos and pivot pos
	btVector3 pickPos(m[12], m[13], m[14]);
	btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
	
	
	constraint = new btPoint2PointConstraint(*body, localPivot);
	
	this->world = world;
	world->addConstraint(constraint);	
	static_cast<btPoint2PointConstraint*>(constraint)->m_setting.m_tau = 0.1f;
	
	//delete p2p;
		
}

//--------------------------------------------------------	
void TouchedConstraint::updateTouchConstraint(ofVec2f pos) {

	if (constraint != NULL) {
		
		
		//move the constraint pivot
		btPoint2PointConstraint* p2p = static_cast<btPoint2PointConstraint*>(constraint);
		
		if (p2p && body) {
			
			// get the body world position
			btScalar m[16];
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			btVector3 bodyPos(m[12], m[13], m[14]);
			
			// touch pos
			btVector3 touchPos(pos.x, pos.y, 0);
			
			// direction of touch
			btVector3 dir = touchPos - bodyPos;
			dir.normalize();
			dir *= 3.0;
			
			// set the constraint pivot pos
			btVector3 newPivot = touchPos + dir;
			p2p->setPivotB(newPivot);
		}
		
	} 
	
}
