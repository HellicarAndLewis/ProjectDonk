/*
 *  Particle.h
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#pragma once

//#include "MSACore.h"
#include "MSAFluidSolver.h"

using namespace MSA;

class Particle {
public:	
    ofVec2f	pos, vel, center, windowSize;
    float	spriteSize;
    float	alpha;
    float	mass;
	
	bool	incAlpha;
	
	
	
	Particle();
	void init(float x, float y);
    void init(float x, float y, ofVec2f gravitationalForce);
    void update( const FluidSolver &solver, const ofVec2f &windowSize, const ofVec2f &invWindowSize, float maxSpriteSize );
	void updateVertexArrays( bool drawingFluid, const ofVec2f &invWindowSize, int i, float* posBuffer, float* colBuffer, float* heightBuffer);
};

