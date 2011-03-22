/*
 *  TouchedConstraint.h
 *  Render
 *
 *  Created by Todd Vanderlin on 2/28/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofxBullet.h"

class TouchedConstraint {
	
public:
	
	bool bRemove;
	//--------------------------------------------------------
	TouchedConstraint();
	~TouchedConstraint();
	
	
	void destroy();
	void setTouchBody(btDiscreteDynamicsWorld * world, btRigidBody * bd);	
	void updateTouchConstraint(ofVec2f pos);
	
	//--------------------------------------------------------	
	btRigidBody				* body;
	btTypedConstraint		* constraint;		
	btDiscreteDynamicsWorld * world;	
};