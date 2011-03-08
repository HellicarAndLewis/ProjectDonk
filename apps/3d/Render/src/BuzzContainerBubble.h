/*
 *  BuzzContainerBubble.h
 *  Render
 *
 *  Created by Chris on 3/8/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#pragma once
#include "ofxBullet.h"
#include "ContentBubble.h"

class BuzzContainerBubble : public ContentBubble{
	
	public:
	
		BuzzContainerBubble();
		
		void createContainerBubble(ofxBullet * bullet);
		void pop();
		
		void drawTwitterData(){}
		
		
		//void create(btDiscreteDynamicsWorld* world, float radius, ofVec3f position, int resolution);

	private:
	
		//btDiscreteDynamicsWorld * m_world;
		//vector<ofxBulletRigidBody*> m_children;
};

