/*
 *
 *  ParticleSystem.cpp
 *
 *  Ported from ofxMSAFluid
 *
 */

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() 
{
	curIndex = 0;
	//setWindowSize( ofVec2f( 1, 1 ) );
	
	shader.setup("shader/pointShader.vs", "shader/pointShader.fs" );
	
	ofDisableArbTex();
	dustParticle.loadImage("shader/pointSpriteImg.png");
	ofEnableArbTex();
	
	drawingType = SHADED_POINT_SPRITE;
}

void ParticleSystem::init()
{
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true);
	fluidSolver.setFadeSpeed(0.002f);
	fluidSolver.setDeltaT(1);
	fluidSolver.setColorDiffusion(0);
	fluidSolver.setVisc(0.00015);
	fluidDrawer.setup( &fluidSolver );
	
	setWindowSize(ofVec2f(ofGetWidth(), ofGetHeight()));
	
	fluidCellsX			= 150;
	drawFluid			= true;
	drawParticles		= true;
	resizeFluid			= true;
	
	//pMouse = ofVec2f(ofGetWidth()/2, ofGetHeight()/2);
	
	currentEmitter = 0;
	float inc = ofGetWidth()/3.f;
	for(int i = 0; i<NUM_EMITTERS/2; i++)
	{
		forceEmitters[i].set(i*inc, ofGetHeight());
	}
	
	for(int i = NUM_EMITTERS/2; i<NUM_EMITTERS; i++)
	{
		forceEmitters[i].set((i-NUM_EMITTERS/2)*inc, ofGetHeight()/2);
	}
}	

void ParticleSystem::setWindowSize( ofVec2f winSize )
{
	windowSize = winSize;
	invWindowSize = ofVec2f( 1.0f / winSize.x, 1.0f / winSize.y );
}

void ParticleSystem::draw( bool drawingFluid ){

	
	switch ( drawingType ) 
	{
		case SIMPLE_POINTS:
			
			glEnable(GL_BLEND);
			glDisable( GL_TEXTURE_2D );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_LINE_SMOOTH); 
			
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, posArray);
			
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, colArray);
			
			//glDrawArrays(GL_TRIANGLE_STRIP, 0, MAX_PARTICLES * 2);
			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES * 2);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // put blending back
			glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
		break;
	
		case POINT_SPRITE:
		{
			glEnable(GL_BLEND);
			//glDisable( GL_TEXTURE_2D );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			glDisable(GL_DEPTH_TEST);
			dustParticle.bind();
			
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
			
			// Enable Vertex Points
			// Enable the Vertex Array and PixelSize Attribute
			
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, posArray);

			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES * 2);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			
			dustParticle.unbind();
			
			// Clean up
			glDisableClientState(GL_VERTEX_ARRAY); 
			glDisable(GL_POINT_SPRITE);
			glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // put blending back
		}
		break;
			
		case SHADED_POINT_SPRITE:
		{
			
			shader.begin(); // Turn on the Shader
			
			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			// Get the attribute and bind it
			GLint pixel_loc = glGetAttribLocation(shader.getProgram(), "pointSize");
			glVertexAttribPointer(pixel_loc, 4, GL_FLOAT, false, 0, heightArray);
			glBindAttribLocation(shader.getProgram(), pixel_loc, "pointSize");
			glEnableVertexAttribArray(pixel_loc);
			
			glDisable(GL_DEPTH_TEST);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			
			dustParticle.bind();
			
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);      
			
			// try w/o vertex array
			/*glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, posArray);
			
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, colArray);
			
			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES * 2);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);*/
			
			glBegin(GL_POINTS);
			int i = 0;
			while( i < MAX_PARTICLES ) {
				glVertex3f(posArray[i], posArray[i+1], -1);
				i += 2;
			}
			glEnd();
				
			dustParticle.unbind();
			
			// Clean up
			glDisableClientState(GL_VERTEX_ARRAY); 
			glDisable(GL_POINT_SPRITE);
			glDisableVertexAttribArray(pixel_loc);
			shader.end();
			
			glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			//glDisable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // put blending back
		}
		break;
	}
	
}


void ParticleSystem::addParticles( const ofVec2f &pos, int count ){
	for(int i=0; i<count; i++)
		//addParticle( pos + Rand::randofVec2f() * 15 );
		addParticle( pos + ofVec2f(ofRandom(20), ofRandom(20)) );
}


void ParticleSystem::addParticle( const ofVec2f &pos ) {
	
	if(curIndex < MAX_PARTICLES) 
	{
		particles[curIndex].init( pos.x, pos.y );
		curIndex++;
	}
	
	if(curIndex == MAX_PARTICLES) {
		curIndex = 0;
	}
}

void ParticleSystem::update()
{
	/*if(resizeFluid)
	 {
	 
	 //float hwRatio = ofGetHeight()/ofGetWidth();
	 float hwRatio = 1200/800;
	 fluidSolver.setSize(fluidCellsX, fluidCellsX / hwRatio);
	 fluidDrawer.setup(&fluidSolver);
	 resizeFluid = false;
	 }*/
	
	// manage
	fluidSolver.update();
	
	for(int i = 0 ; i<NUM_EMITTERS; i++)
	{
		
		ofVec2f vel(0,ofRandom(1.f) * -1);
		
		ofVec2f constrainPos(
							 ofMap(forceEmitters[i].x, 0, 1024, 0.f, 1.f, true),
							 ofMap(forceEmitters[i].y, 0, 768, 0.f, 1.f, true) 
							 );
		
		const float colorMult = 100;
		const float velocityMult = 0.3;
		
		//addToFluid(forceEmitters[i], vel, true, true);
		int index = fluidSolver.getIndexForPos(constrainPos);
		fluidSolver.addForceAtIndex(index, vel * velocityMult);
	}
	
	if(ofGetFrameNum() % 2 == 0 ) {
		
		ofVec2f pos(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
		ofVec2f vel(sin(ofGetFrameNum()), cos(ofGetFrameNum()));
		addToFluid(pos, vel, true, true);
		
	}
	
	// update each particle
	for(int i=0; i<MAX_PARTICLES - 1; i++) {
		if(particles[i].alpha > 0) {
			particles[i].update( fluidSolver, windowSize, invWindowSize );
			//cout << particles[i].radius << endl;
			particles[i].updateVertexArrays( false, invWindowSize, i, posArray, colArray, heightArray);
		}
	}
}


// add force and dye to fluid, and create particles
void ParticleSystem::addToFluid( ofVec2f pos, ofVec2f vel, bool addColor, bool addForce ) {
    //float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    
	//balance the x and y components of speed with the screen aspect ratio
	float aspectRatio = ofGetWidth()/ofGetHeight();
	float speed = vel.x * vel.x  + vel.y * vel.y * aspectRatio * aspectRatio;
    if(speed > 0) {
		ofVec2f constrainPos(
							 ofMap(pos.x, 0, 1024, 0.f, 1.f, true),
							 ofMap(pos.y, 0, 768, 0.f, 1.f, true) 
							 );
		
        const float colorMult = 100;
        const float velocityMult = 30;
		
		int index = fluidSolver.getIndexForPos(constrainPos);
		
		if(addColor) {
			ofColor drawColor;
			drawColor.setHex(0x5E2612, 1.0);
			//drawColor.setHsb((ofGetFrameNum() % 360 ) / 360.0f, 1, 1);
			fluidSolver.addColorAtIndex(index, drawColor);// * colorMult);
		}
		
		//particleSystem.addParticles( pos * ofVec2f( ofGetWindowSize() ), 5 );
		addParticles( pos, 3 );
		//fluidSolver.addForceAtIndex(index, vel * velocityMult);
    }
}

