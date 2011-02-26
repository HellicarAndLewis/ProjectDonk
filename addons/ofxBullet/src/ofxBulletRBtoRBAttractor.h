/*
 *  ofxBulletRBtoRBAttractor.h
 *  Bullet
 *
 *  Created by Chris on 2/13/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofxBulletBaseConstraint.h"
#include "ofxBulletRigidBody.h"

class ofxBulletRBtoRBAttractor : public ofxBulletBaseConstraint{

	public:
		ofxBulletRBtoRBAttractor();
		void setup(ofxBulletRigidBody * _a,ofxBulletRigidBody * _b, float _force);
		void solve();
		
		bool shouldDestroy();
		
		void setForce(float _force){ force = _force;}
		
		ofxBulletRigidBody * getA(){ return a;}
		ofxBulletRigidBody * getB(){ return b;}
	
	protected:
		
		ofxBulletRigidBody * a;
		ofxBulletRigidBody * b;
		float force;
		

};
