/*
 *  ContentBubble.h
 *  Render
 *
 *  Created by Todd Vanderlin on 2/26/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxBullet.h"
#include "BubbleData.h"
#include "ofxCubeMap.h"

//--------------------------------------------------------------
class ContentBubble {
	
public:
	
	
	//---------------------------------
	ofxBulletRigidBody * rigidBody;
	btScalar			 m[16];
	float				 billboadMatrix[16];
	
	ofVec3f				 target, pos;
	ofVec3f				 offScreenTaget;
	float				 distanceToTarget;
	float				 alpha;
	ofColor				 color;
	float			     radius, originalRadius;
	float				 targetForce;
	Donk::BubbleData *   data;
	
	float				 touchAlpha, touchAlphaTarget;
	bool				 bTouched, bDoubleTouched;
	int					 touchID;
	
	bool				 bAlive;
	
	// ---------------------------------
	ContentBubble();
	
	// ---------------------------------
	void doubleTouched();
	void setRadius(float r);
	
	// ---------------------------------
	void createContentBubble();
	void update();
	void gotoTarget();
	void goOffScreen();
	
	void pushBubble();
	void popBubble();
	
	virtual void drawTwitterData();
	virtual void drawHighLight();
	virtual void draw();
	static ofTrueTypeFont font;
	
	/**
	  for 2d layer to flip around when selected
	 */
	float rotateYTarget;
	float rotateY;
	float rotateYDirection;
};


