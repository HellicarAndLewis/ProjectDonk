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

static const float MOMENTUM = 0.62f;
static const float FLUID_FORCE = 0.6f;

Particle::Particle() {
	incAlpha = true;
}

void Particle::init(float x, float y) {

	incAlpha = true;
	
	pos.set(x, y);
	vel.set(0, 0);//vel.set(ofRandom(0.5), ofRandom(0.5));
	radius = 5;
	alpha = 0.1; //Rand::randFloat( 0.3f, 1 );
	mass = ofRandom(0.3f)+0.1; //Rand::randFloat( 0.1f, 1 );

}

void Particle::update( const FluidSolver &solver, const ofVec2f &windowSize, const ofVec2f &invWindowSize ) {
	// only update if particle is visible
	if( alpha == 0 ) {
		return;
	}
	
	vel = solver.getVelocityAtPos( pos * invWindowSize ) * (mass * FLUID_FORCE ) * windowSize + vel * MOMENTUM;
	pos += vel;	
	
	// hackish way to make particles glitter when the slow down a lot
	/*if( vel.lengthSquared() < 1 ) {
		vel += ofVec2f(ofRandom(-0.5, 0.5), ofRandom(-0.5, 0.0));
	}*/
	
	// fade out a bit (and kill if alpha == 0);
	
	if( incAlpha ) {
		alpha /= 0.996f;
		if(alpha > 0.95) {
			incAlpha = false;
		}
	} else {
		alpha *= 0.996;
	}
	if( alpha < 0.01f )
		alpha = 0;
}



void Particle::updateVertexArrays( bool drawingFluid, const ofVec2f &invWindowSize, int i, float* posBuffer, float* colBuffer, float* heightBuffer) {
	int vi = i * 4;
	posBuffer[vi++] = pos.x - vel.x;
	posBuffer[vi++] = pos.y - vel.y;
	posBuffer[vi++] = pos.x;
	posBuffer[vi++] = pos.y;
	
	heightBuffer[vi++] = alpha * 16;
	heightBuffer[vi++] = alpha * 16;
	heightBuffer[vi++] = alpha * 16;
	heightBuffer[vi++] = alpha * 16;
	
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
		
		ofColor color;
		//color.setHsb( 220 + (alpha * 20.f), 255.f, 0.5); //lerp( 0.5f, 1.0f, mass ) * alpha);
		color.setHsb( 190 + (alpha * 20.f), 255.f, 0.5); //lerp( 0.5f, 1.0f, mass ) * alpha);
		
		colBuffer[ci++] = color.r;
		colBuffer[ci++] = color.g;
		colBuffer[ci++] = color.b;
		colBuffer[ci++] = color.r;
		colBuffer[ci++] = color.g;
		colBuffer[ci++] = color.b;
	}
}