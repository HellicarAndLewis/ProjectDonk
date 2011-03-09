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
	velocity.set(ofRandom(-0.001, 0.001), ofRandom(-0.003, 0));
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
	update();
	
	//ofStrokeColor(255, age);
	radius = (max<float>( age * radius, minSize));
	ofSetColor((255*age), (255*age), (255*age));
	ofNoFill();
	ofCircle(position.x, position.y, radius);
	//ofSphere(position.x, position.y, 100 - ( 1000 - (age * 1000)), radius);
	ofFill();
	ofCircle(position.x, position.y, radius);
}
