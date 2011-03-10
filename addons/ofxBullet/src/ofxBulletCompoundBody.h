/*
 *  ofxBulletCompoundBody.h
 *  Render
 *
 *  Created by Chris on 3/8/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

/*
 *  ofxBulletRigidBody.h
 *  Bullet
 *
 *  Created by Todd Vanderlin on 2/5/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofxBulletUtils.h"
#include "btBulletDynamicsCommon.h"
#include "ofxBulletRigidBody.h"

class ofxBulletCompoundBody : public ofxBulletRigidBody{
	
	
	
public:
	
	btCompoundShape	*	compoundShape;
	ofxBulletCompoundBody();
	~ofxBulletCompoundBody();
	
	void createCompoundBody(int mass, const btTransform startTrans);
	void drawChildren();
	
	void destroy() {
		
		bDestroy = true;
		
		if(isBody()) {
			
			
			world->removeRigidBody(body);
			delete body;
			delete shape;
			delete compoundShape;
			body = NULL;
			shape = NULL;
			
			printf("~ofxBulletCompoundBody()\n");
		}
		
	}
	
};