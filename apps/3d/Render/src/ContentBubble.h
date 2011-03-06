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
	btScalar	m[16];
	ofVec3f		target;
	ofColor		color;
	float		radius;
	Donk::BubbleData *data;
	
	
	float		touchAlpha, touchAlphaTarget;
	bool		bTouched;
	
	GLUquadricObj *quadratic;
	ofImage permImg, glossImg;
	ofVec3f	lightPosition;
	ofShader shader;
	ofxCubeMap cubeMap;
	
	void renderSphere() {}
	void setLightPosition(ofVec3f position) { lightPosition = position; }
	
	
	//---------------------------------
	ContentBubble();
	void createContentBubble();
	void update();
	void draw();
	
};


