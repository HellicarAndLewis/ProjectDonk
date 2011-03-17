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
#include "ofxBulletCompoundBody.h"
#include "ContentBubble.h"

class BuzzContainerBubble : public ContentBubble{
	
public:
	
	BuzzContainerBubble();
	
	void createContainerBubble(ofxBullet * bullet, ofVec3f pos);
	void pop();
	void updateConstraint();
	
	ofxBulletCompoundBody   * globe;
	//btPoint2PointConstraint * p2p;
	int nChildren;
	string pollingGroup;
	
private:
	
	
};

