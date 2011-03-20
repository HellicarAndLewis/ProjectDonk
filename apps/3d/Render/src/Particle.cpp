/*
 *  Particle.cpp
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "Particle.h"

using namespace MSA;

static const float MOMENTUM = 0.1f;
static const float FLUID_FORCE = 0.6f;

Particle::Particle() {
	incAlpha = true;
	alpha = 0.0;
}

void Particle::init(float x, float y, ofVec2f gravitionalForce) {
	
	incAlpha = true;
	
	pos.set(x, y);
	center.set(gravitionalForce); 
	vel.set(0, 0);
	alpha = ofRandomuf();
	mass = ofRandom(0.3f)+0.1; //Rand::randFloat( 0.1f, 1 );
	
}

void Particle::init(float x, float y) {

	incAlpha = true;
	
	pos.set(x, y);
	vel.set(0, 0);//vel.set(ofRandom(0.5), ofRandom(0.5));
	alpha = ofRandomuf();
	mass = ofRandom(0.3f)+0.1; //Rand::randFloat( 0.1f, 1 );

}

void Particle::update( const FluidSolver &solver, const ofVec2f &windowSize, const ofVec2f &invWindowSize, float maxSpriteSize ) {
	// only update if particle is visible
	if( alpha == 0 ) {
		return;
	}
	
	vel = solver.getVelocityAtPos( pos * invWindowSize ) * (mass * FLUID_FORCE ) * windowSize + vel * MOMENTUM;
	pos += vel;	
	
	// wrap positions
	if( pos.x < 0 ) {
		vel.x *= -5;
		pos.x = 0;
	} else if( pos.x > windowSize.x ) {
		vel.x *= -5;
		pos.x = windowSize.x;
	}
	if( pos.y < 0 ) {
		vel.y *= -5;
		pos.y = 0;
	} else if( pos.y > windowSize.y ) {
		vel.y *= -5;
		pos.y = windowSize.y;
	}
	
	// hackish way to make particles glitter when the slow down a lot
	if( vel.lengthSquared() < 1 ) {
		vel += ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.0));
	}
	
	// fade out a bit (and kill if alpha == 0);
	
	if( incAlpha ) {
		alpha += 0.001;
		if(alpha > 0.99) {
			incAlpha = false;
		}
	} else {
		alpha -= 0.001;
		if( alpha < 0.01f ) {
			incAlpha = true;
		}
	}
	
	spriteSize = maxSpriteSize;

}



void Particle::updateVertexArrays( bool drawingFluid, const ofVec2f &invWindowSize, int i, float* posBuffer, float* colBuffer, float* heightBuffer) {
	int vi = i * 4;
	int hi = i * 4;

	ofVec2f centerDist = pos - center;
	
	if( centerDist.length() > 800 )
	{
		vel.y += (center.y - pos.y)/300;
		vel.x += (center.x - pos.x)/300;
	}

	posBuffer[vi++] = pos.x - vel.x;
	posBuffer[vi++] = pos.y - vel.y;
	posBuffer[vi++] = pos.x;
	posBuffer[vi++] = pos.y;

		
	heightBuffer[hi++] = alpha*spriteSize;
	heightBuffer[hi++] = alpha*spriteSize;
	heightBuffer[hi++] = alpha*spriteSize;
	heightBuffer[hi++] = alpha*spriteSize;
	
	int ci = i * 6;
	if( drawingFluid ) {
		// if drawing fluid, draw lines as black & white
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
		colBuffer[ci++] = alpha;
	} else {
		// otherwise, use color
		float vxNorm = vel.x * invWindowSize.x;
		float vyNorm = vel.y * invWindowSize.y;
		float v2 = vxNorm * vxNorm + vyNorm * vyNorm;
#define VMAX 0.013f
		if(v2>VMAX*VMAX) v2 = VMAX*VMAX;
		float satInc = mass > 0.5 ? mass * mass * mass : 0;
		satInc *= satInc * satInc * satInc;
//		Color color;
//		color.setHSV(0, mapRange(v2, 0.0f, VMAX*VMAX, 0.0f, 1.0f) + satInc, ofLerp(0.5, 1, mass) * alpha);
		
		// TODO - figure this out!
		
		//ofColor color( ci::CM_HSV, 0, v2 / ( VMAX * VMAX ), lerp( 0.5f, 1.0f, mass ) * alpha ); 
		ofColor color;
		//color.setHsb( 255, v2 / ( VMAX * VMAX ), lerp( 0.5f, 1.0f, mass ) * alpha);
		color.setHsb( 0, 0, 255);
		
		colBuffer[ci++] = color.r;
		colBuffer[ci++] = color.g;
		colBuffer[ci++] = color.b;
		colBuffer[ci++] = color.r;
		colBuffer[ci++] = color.g;
		colBuffer[ci++] = color.b;
	}
}