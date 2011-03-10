/*
 *  ParticleSystem.h
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 *  Updated 2011. Guess this is illegal?
 *
 */
#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "MSAFluid.h"

#define NUM_EMITTERS 6
#define MAX_PARTICLES		2000 //50000

enum DRAWING_TYPE { SIMPLE_POINTS, POINT_SPRITE, SHADED_POINT_SPRITE };

//using namespace MSA;

class ParticleSystem {
public:	
	
	ParticleSystem();
	
	void init();
	
    void updateAndDraw( bool drawingFluid );
	void addParticles( const ofVec2f &pos, int count );
	void addParticle( const ofVec2f &pos );
	void setWindowSize( ofVec2f winSize );
	
    float posArray[MAX_PARTICLES * 2 * 2];
    float colArray[MAX_PARTICLES * 3 * 2];
	float heightArray[MAX_PARTICLES  * 2 * 2];
	ofVec2f				windowSize;
	ofVec2f				invWindowSize;
	const FluidSolver	*solver;
	
    int curIndex;
	
	ofImage starImage;
	
    Particle particles[MAX_PARTICLES];
	
	
	void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
	
	int					fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	bool				drawParticles;
	
	MSA::FluidSolver	fluidSolver;
	MSA::FluidDrawerGl	fluidDrawer;	

	ofVec2f				pMouse;
	bool				addForces;
	
	ofVec2f				forceEmitters[6];
	int					currentEmitter;
	
	
	ofShader shader;
	ofTexture pointSpriteTexture;
	
	DRAWING_TYPE drawingType;
	
};

