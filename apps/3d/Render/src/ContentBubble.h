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
//#include "txtDisplay.h"
#include "ofxTextCircleFill.h"

//--------------------------------------------------------------
class ContentBubble {
	
public:
	
	
	//---------------------------------
	ofxBulletRigidBody * rigidBody;
	btScalar			 m[16];
	float				 billboadMatrix[16];
	
	ofVec3f				 bobTheta;
	ofVec3f				 target, pos, resetTarget;
	ofVec3f				 offScreenTaget;
	float				 distanceToTarget;
	float				 alpha;
	ofColor				 color;
	float			     radius;
	float				 targetForce;
	Donk::BubbleData *   data;
	
	float				 touchAlpha, touchAlphaTarget;
	bool				 bTouched, bDoubleTouched;
	bool				 bFlipToggle;
	float				 timeSinceLastFlip;
	int					 touchID;
	float				 birthDate, age;
	bool				 bAlive, bRemove;
	bool				 bAnimateOut, bAnimateIn;
	
	// --------------------------------- things for inspiration
	bool				 bRemoveOffScreen;
	
	// --------------------------------- things useing in performance
	int					 performceImageID;
	int					 performanceChannel;
	ofVec3f				 performanceStartTarget, noise;
	float				 startRadius, maxRadius;
	
	// --------------------------------- things for vote
	int					 voteBubbleID;
	int					 voteImageID;
	bool				 bVoteEnabled, bVoteNeedsUpdate;
	float				 voteDelay, voteTimer;
	
	// ---------------------------------
	int					 buzzID;
	float				 buzzWait;
	float				 buzzTime;
	ofVec3f				 buzzDest,buzzOrig;
	bool				 zoomTouched;	
	
	float				 loopCounter,loopTime;
    
    //------------------------ text formatting
    
	static ofTrueTypeFont   font; //JG now this is just used on VOTE
	//TxtDisplay              textDisplay;
    ofxTextCircleFill  textDisplay;
    int                     minFontSize;
    int                     maxFontSize;
	// ---------------------------------
	ContentBubble();
	void destroy();
	
	// ---------------------------------
	void doubleTouched();
	void releaseTouch();
	
	void setRadius(float r);
	void lerpRadius(float r,float speed);
	ofVec3f getPosition();
	void setTarget(float x, float y, float z=0);
	void setTarget(ofVec3f v);
	void addForce(float x, float y, float z=0, float scale=1.0);
	void addAtrractionForce(float x, float y, float z=0, float scale=1.0);
	void addAtrractionForce(ofVec3f &p, float scale=1.0);
	
	// ---------------------------------
	void createContentBubble();
	void update();
	void gotoTarget(float scale=1.0);
	void goOffScreen();
	
	// idle behaviours
	void bobMe();
	void buzzMe();
	void loopMe(float interactiveWidth,float interactiveHeight);
	void setLoopStart(float interactiveHeight);
	
	void pushBubble();
	void popBubble();
	void pushBillboard();
	void popBillboard();
	
	virtual void drawTwitterData();
	virtual void drawHighLight();
	virtual void draw();
    
    void loadFont();
	/**
	  for 2d layer to flip around when selected
	 */
	float rotateYTarget;
	float rotateY;
	float rotateYDirectionOff,rotateYDirectionOn;
	
	void setContentSideUp();
	void setUserSizeUp();
};


