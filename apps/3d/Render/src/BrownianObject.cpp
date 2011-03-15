/*
 *  BrownianObject.cpp
 *  emptyExample
 *
 */

#include "BrownianObject.h"

BrownianObject::BrownianObject(ofVec2f initialLocation, float initialRadius, bool limitXMovement)
{
	
	radius = initialRadius;
	position.set(initialLocation.x, initialLocation.y);
	velocity.set(ofRandom(-0.3, 0.3), ofRandom(-2, -1));
	age = 1.f;
	minSize = ofRandom(12);
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

	//velocity.y += ofRandom(-br*1.5, br);// - map(age, 255, 0, 0.01, 0.1);
	velocity.y += ofRandom(-br, br);
}


void BrownianObject::draw() 
{
	
	//ofStrokeColor(255, age);
	radius = (max<float>( age * radius, minSize));
	//glBindTexture(GL_TEXTURE_2D, NULL); // what's this all about
	ofSetColor(255,255,255, 255);
	ofNoFill();
	ofCircle(position.x, position.y, radius);
}
