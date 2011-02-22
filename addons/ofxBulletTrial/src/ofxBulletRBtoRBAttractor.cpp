/*
 *  ofxBulletRBtoRBAttractor.cpp
 *  Bullet
 *
 *  Created by Chris on 2/13/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#include "ofxBulletRBtoRBAttractor.h"

ofxBulletRBtoRBAttractor::ofxBulletRBtoRBAttractor()
{
	_type = constraintTypeAttraction;
	_isOn = false;
	_isDead = false;
}

void ofxBulletRBtoRBAttractor::setup(ofxBulletRigidBody * _a,ofxBulletRigidBody * _b, float _force)
{
	a = _a;
	b = _b;
	force = _force;
	_isOn = true;
}

bool ofxBulletRBtoRBAttractor::shouldDestroy()
{
	if( a->bDestroy || b->bDestroy ) return true;
	if( _isDead ) return true;
	
	return false;
}

void ofxBulletRBtoRBAttractor::solve()
{
	if(_isDead || !_isOn)	return;
	
	ofVec3f ap = a->getPosition();
	ofVec3f bp = b->getPosition();
	ofVec3f c = ofVec3f(0, 10, 0);
	ofVec3f v = bp-ap;
	v.normalize();
	v *= force;
	
	a->body->applyCentralForce(btVector3(v.x, v.y, v.z));
	
}
