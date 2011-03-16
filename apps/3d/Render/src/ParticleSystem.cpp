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

void ParticleSystem::init(ofVec2f simulationPixelSize)
{
	// setup fluid stuff
	fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(true);
	fluidSolver.setFadeSpeed(0.002f);
	fluidSolver.setDeltaT(1);
	fluidSolver.setColorDiffusion(0);
	fluidSolver.setVisc(0.00015);
	fluidDrawer.setup( &fluidSolver );

	windowSize = simulationPixelSize;
	invWindowSize = ofVec2f( 1.0f / simulationPixelSize.x, 1.0f / simulationPixelSize.y );
	
	fluidCellsX			= 150;
	drawFluid			= true;
	drawParticles		= true;
	resizeFluid			= true;
	
	prevForcePosition = ofVec2f(windowSize.x/2, windowSize.y/2);
	
	particleColor.set(1, 1, 1, 1);
	
	currentEmitter = 0;
	float inc = windowSize.y/3.f;
	for(int i = 0; i<NUM_EMITTERS/2; i++)
	{
		forceEmitters[i].set(i*inc, windowSize.y);
	}
	
	for(int i = NUM_EMITTERS/2; i<NUM_EMITTERS; i++)
	{
		forceEmitters[i].set((i-NUM_EMITTERS/2)*inc, windowSize.y/2);
	}
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
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			glDisable(GL_DEPTH_TEST);
			dustParticle.bind();
			
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
			
			// Enable Vertex Points
			// Enable the Vertex Array
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
			
			glPointSize(pointSize);
			
			//glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			glColor3f(particleColor.r, particleColor.g, particleColor.b);
			//ofSetColor(particleColor);
			
			// Get the attribute and bind it
			
			//shader.begin(); // Turn on the Shader
			//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
			/*GLint pixel_loc = glGetAttribLocation(shader.getProgram(), "pointSize");
			glVertexAttribPointer(pixel_loc, 4, GL_FLOAT, false, 0, heightArray);
			glBindAttribLocation(shader.getProgram(), pixel_loc, "pointSize");
			glEnableVertexAttribArray(pixel_loc); */

			glDisable(GL_DEPTH_TEST);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			
			dustParticle.bind();
			
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, posArray);
			
			//glEnableClientState(GL_COLOR_ARRAY);
			//glColorPointer(3, GL_FLOAT, 0, colArray);
			
			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES * 2);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			//glDisableClientState(GL_COLOR_ARRAY);
				
			dustParticle.unbind();
			
			// Clean up
			glDisableClientState(GL_VERTEX_ARRAY); 
			
			glDisable(GL_POINT_SPRITE);
			//glDisableVertexAttribArray(pixel_loc);
			//shader.end();
			//glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // put blending back

			
		}
		break;
	}
	
}

void ParticleSystem::update()
{	
	
	fluidSolver.update();
	
	for(int i = 0 ; i<NUM_EMITTERS; i++)
	{
		
		ofVec2f vel(0,ofRandom(1.f) * -1);
		
		ofVec2f constrainPos(
							 ofMap(forceEmitters[i].x, 0, windowSize.x, 0.f, 1.f, true),
							 ofMap(forceEmitters[i].y, 0, windowSize.y, 0.f, 1.f, true) 
							 );
		
		const float colorMult = 100;
		const float velocityMult = 0.3;
		
		//addForceAndParticle(forceEmitters[i], vel, true, true);
		int index = fluidSolver.getIndexForPos(constrainPos);
		fluidSolver.addForceAtIndex(index, vel * velocityMult);
	}
	
	if(ofGetFrameNum() % 2 == 0 ) {
		
		ofVec2f pos(ofRandom(windowSize.x), ofRandom(windowSize.y));
		ofVec2f vel(sin(ofGetFrameNum()), cos(ofGetFrameNum()));
		addForceAndParticle(pos, vel, true, true);
		
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

// just add force, no new particles
void ParticleSystem::addForceAtPoint( ofVec2f position )
{
	
	ofVec2f constrainPos(
						 ofMap(position.x, 0, windowSize.x, 0.f, 1.f, true),
						 ofMap(position.y, 0, windowSize.y, 0.f, 1.f, true) 
						 );
	
	const float colorMult = 100;
	const float velocityMult = 0.5; //30;
	
	int index = fluidSolver.getIndexForPos(constrainPos);
	ofVec2f vel = position - prevForcePosition;
	
	float aspectRatio = windowSize.x/windowSize.y;
	float speed = vel.x * vel.x  + vel.y * vel.y * aspectRatio * aspectRatio;
	fluidSolver.addForceAtIndex(index, vel * velocityMult);
	
	// store for next force
	prevForcePosition = position;
}

// add force and dye to fluid, and create particles
void ParticleSystem::addForceAndParticle( ofVec2f pos, ofVec2f vel, bool addColor, bool addForce ) {
    //float speed = vel.x * vel.x  + vel.y * vel.y * getWindowAspectRatio() * getWindowAspectRatio();    
	//balance the x and y components of speed with the screen aspect ratio
	float aspectRatio = windowSize.x/windowSize.y;
	float speed = vel.x * vel.x  + vel.y * vel.y * aspectRatio * aspectRatio;
    if(speed > 0) {
		ofVec2f constrainPos(
							 ofMap(pos.x, 0, windowSize.x, 0.f, 1.f, true),
							 ofMap(pos.y, 0, windowSize.y, 0.f, 1.f, true) 
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


void ParticleSystem::addParticles( const ofVec2f &pos, int count ){
	for(int i=0; i<count; i++)
		//addParticle( pos + Rand::randofVec2f() * 15 );
		addParticle( pos + ofVec2f(ofRandom(20), ofRandom(20)) );
}


void ParticleSystem::addParticle( const ofVec2f &pos ) {
	
	if(curIndex < MAX_PARTICLES) 
	{
		if(useGravity) {
			particles[curIndex].init( pos.x, pos.y );
		} else {
			particles[curIndex].init( pos.x, pos.y, ofVec2f(0, -1));
		}
		curIndex++;
	}

	if(curIndex == MAX_PARTICLES) {
		curIndex = 0;
	}
}
