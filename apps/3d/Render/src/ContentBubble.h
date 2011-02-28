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


//--------------------------------------------------------------
class ContentBubble {
	
public:
	
	
	ofxBulletRigidBody * rigidBody;
	btScalar	m[16];
	ofVec3f		target;
	ofColor		color;
	Donk::BubbleData *data;
	
	ContentBubble() {
		rigidBody = NULL;	
	}
	
	void createContentBubble() {
		
		color.r = ofRandom(0, 255);
		color.g = 100;
		color.b = 40;
		color.a = 128;
		
	}
	
	void update() {

		if(rigidBody->isBody()) {
			
			rigidBody->body->setDamping(0.76, 0.76);
			
			ofVec3f frc = target-rigidBody->getPosition();
			frc.normalize();
			frc *= 900.0;
			
			rigidBody->body->clearForces();
			rigidBody->body->applyCentralForce(btVector3(frc.x, frc.y, frc.z));
			
		}
		
		//target.y += 0.5;
		//if(target.y > 500) target.y = -500;
	}
	
	void draw() {
		
		ofSetColor(255, 255, 0);
		drawSphere(target, 3, 10);
		
		if(rigidBody->isBody()) {
			
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_ALPHA_TEST);
			
			rigidBody->body->getWorldTransform().getOpenGLMatrix(m);
			
			btSphereShape * sphereShape = (btSphereShape*)rigidBody->shape;
			float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
			
			glPushMatrix();
			glTranslatef(0, 0, 0);
			glMultMatrixf(m);
			
			ofEnableAlphaBlending();
			
			//ofSetColor(255, 255, 255);
			//ofFill();
			//drawSphere(ofVec3f(0, 0, 0), radius/3.0);
			data->radius = radius;
			data->draw();
			
			ofSetColor(color);
			ofFill();
			drawSphere(ofVec3f(0, 0, 0), radius, 20);
			
			
			
			glPopMatrix();
			
		}
	}
	
};


