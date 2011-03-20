/*
 *  ParticleSystem.h
 *  
 *  ported from ofxMSAFluid Demo
 *  Created by Mehmet Akten on 02/05/2009.
 *
 *  Updated 2011. Guess this is illegal?
 *
 */
#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "MSAFluid.h"

#include "BaseInteraction.h"

#define NUM_EMITTERS		6
#define MAX_PARTICLES		1000 //50000

enum DRAWING_TYPE { SIMPLE_POINTS, POINT_SPRITE, SHADED_POINT_SPRITE };

//using namespace MSA;

class ParticleSystem {
public:	
	
	ParticleSystem();
	
	void init(ofVec2f simulationPixelSize);
	void update();
    void draw( bool drawingFluid );
	void addParticles( const ofVec2f &pos, int count );
	void addParticle( const ofVec2f &pos );
	//void setWindowSize( ofVec2f winSize );
	void fadeToColor(float r, float g, float b, float speed);
	void addForceAndParticle(ofVec2f pos, bool addColor, bool addForce);
	void addForceAtPoint( ofVec2f position );
	
	// set interaction
	void setInteractionMode(int mode);
	
    float				posArray[MAX_PARTICLES * 2 * 2];
    float				colArray[MAX_PARTICLES * 3 * 2];
	float				heightArray[MAX_PARTICLES  * 2 * 2];
	ofVec2f				windowSize;
	ofVec2f				invWindowSize;
    int					curIndex;
    Particle			particles[MAX_PARTICLES];
	int					fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	bool				drawParticles;
	MSA::FluidSolver	fluidSolver;
	MSA::FluidDrawerGl	fluidDrawer;	
	ofVec2f				prevForcePosition;
	bool				addForces;
	ofVec2f				forceEmitters[6];
	int					currentEmitter;
	ofShader			shader;
	DRAWING_TYPE		drawingType;
	
	/// mode options
	bool				useGravity;
	ofColor				particleColor;
	float				maxSpriteSize;
	
	// point sprites
	ofImage				interviewPointSprite;
	ofImage				buzzPointSprite;
	ofImage				votePointSprite;
	ofImage				performancePointSprite;
	ofImage				inspirationPointSprite;
	
	ofImage*			currentImage;
	
	// mode
	int					interactionMode;
	
	
};

