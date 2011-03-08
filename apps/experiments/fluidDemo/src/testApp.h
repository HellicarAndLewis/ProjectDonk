#pragma once

#include "MSAFluid.h"
//#include "MSATimer.h"
#include "ParticleSystem.h"

#include "ofMain.h"

#define NUM_EMITTERS 6

using namespace MSA;

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void mousePressed(int x, int y, int button);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	
	void fadeToColor(float r, float g, float b, float speed);
	void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce);
	
	int					fluidCellsX;
	bool				resizeFluid;
	bool				drawFluid;
	bool				drawParticles;
	
	MSA::FluidSolver	fluidSolver;
	MSA::FluidDrawerGl	fluidDrawer;	
	
	ParticleSystem		particleSystem;
	ofVec2f				pMouse;
	bool				addForces;
	
	ofVec2f				forceEmitters[6];
	int					currentEmitter;
	
};

extern testApp *myApp;



