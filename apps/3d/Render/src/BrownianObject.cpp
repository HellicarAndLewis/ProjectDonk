/*
 *  BrownianObject.cpp
 *  emptyExample
 *
 *  Created by base on 24/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "BrownianObject.h"

BrownianObject::BrownianObject(ofVec2f initialLocation, float initialRadius, bool limitXMovement)
{
	
	radius = initialRadius;
	position.set(initialLocation.x, initialLocation.y);
	velocity.set(ofRandom(-0.1, 0.1), ofRandom(-0.3, 0));
	age = 1.f;
	minSize = ofRandom(3);
	xmoveLimited = limitXMovement;

}

void BrownianObject::update()
{
	age-=0.001f;
	position += velocity;
	float br = 0.01;
	if(xmoveLimited) {
	   velocity.x += ofRandom(-br/3, br/3);
	} else {
		velocity.x += ofRandom(-br, br);
	}

	velocity.y += ofRandom(-br*1.5, br);// - map(age, 255, 0, 0.01, 0.1);
}


void BrownianObject::draw() 
{
	
	//ofStrokeColor(255, age);
	radius = (max<float>( age * radius, minSize));
	glBindTexture(GL_TEXTURE_2D,NULL);
	ofSetColor(255,255,255, 255);
	ofNoFill();
	ofCircle(position.x, position.y, radius);
	//ofSphere(position.x, position.y, 100 - ( 1000 - (age * 1000)), radius);
	//ofFill();
	//ofCircle(position.x, position.y, radius);
}
