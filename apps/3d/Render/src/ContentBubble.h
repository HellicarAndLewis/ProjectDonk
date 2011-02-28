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
	unsigned int permTexture;
	
	GLUquadricObj *quadratic;
	
	ofShader cubeshader;
	ofxCubeMap cubeMap;
	
	ofVec3f sphereCenter;
	ofEasyCam cam;
	
	ofImage permImg;
	ofImage glossImg;
	
	ofTexture permTex, glossTex;
	
	ofPixels permPixels;
	ofPixels glossPixels;
	
	float xLightPos,yLightPos,zLightPos;
	bool inc;
	
	bool hasDrawnTex;
	
	ofFbo first;
	ofFbo second;
	
	
	//---------------------------------
	ofxBulletRigidBody * rigidBody;
	btScalar	m[16];
	ofVec3f		target;
	ofColor		color;
	Donk::BubbleData *data;
	
	ContentBubble();
	
	
	void createContentBubble();
	void update();
	void draw();
	
	void drawGlassBubble();
};


