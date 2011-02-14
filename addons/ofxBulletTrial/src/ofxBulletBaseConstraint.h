/*
 *  ofxBulletBaseConstraint.h
 *  Bullet
 *
 *  Created by Chris on 2/13/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#pragma once
#include "ofxBulletUtils.h"

typedef enum ConstraintType {
	constraintTypeAttraction
} ConstraintType;

class ofxBulletBaseConstraint{
	
public:
	ofxBulletBaseConstraint(){};
	~ofxBulletBaseConstraint(){};
	
	virtual void solve(){}
	virtual void turnOn(){}
	virtual void turnOff(){}
	virtual void kill(){_isDead=true;}
	virtual bool shouldDestroy(){ return false;}
	
	virtual int getType(){ return _type; }
	
protected:
	
	ConstraintType	_type;
	bool			_isOn;
	bool			_isDead;
	
};
