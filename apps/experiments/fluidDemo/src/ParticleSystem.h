/*
 *  ParticleSystem.h
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "Particle.h"

#define MAX_PARTICLES		3000 //50000

using namespace MSA;

enum DRAWING_TYPE { SIMPLE_POINTS, POINT_SPRITE, SHADED_POINT_SPRITE };

class ParticleSystem {
public:	
	
    float posArray[MAX_PARTICLES * 2 * 2];
    float colArray[MAX_PARTICLES * 3 * 2];
	float heightArray[MAX_PARTICLES  * 2 * 2];
	ofVec2f				windowSize;
	ofVec2f				invWindowSize;
	const FluidSolver	*solver;
	
    int curIndex;
	
	ofImage starImage;
	
    Particle particles[MAX_PARTICLES];
	
	ParticleSystem();
	void setFluidSolver( const FluidSolver *aSolver ) { solver = aSolver; }
	
    void updateAndDraw( bool drawingFluid );
	void addParticles( const ofVec2f &pos, int count );
	void addParticle( const ofVec2f &pos );
	void setWindowSize( ofVec2f winSize );
	
	ofShader shader;
	ofTexture pointSpriteTexture;
	
	DRAWING_TYPE drawingType;
	
};

