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
	currentImage = 0;
	curIndex = 0;
	useGravity = true;
	maxSpriteSize = 96.0;
	//setWindowSize( ofVec2f( 1, 1 ) );
	
	shader.setup("shader/pointShader.vs", "shader/pointShader.fs" );
	
	ofDisableArbTex();
	//dustParticle.loadImage("shader/pointSpriteImg.png");
	interviewPointSprite.loadImage("shader/InterviewSpriteImg_001.png");
	buzzPointSprite.loadImage("shader/BuzzSpriteImg_001.png");
	votePointSprite.loadImage("shader/VoteSpriteImg_002.png");
	performancePointSprite.loadImage("shader/PerformanceSpriteImg_001.png");
	inspirationPointSprite.loadImage("shader/InspirationSpriteImg_002.png");
	
	ofEnableArbTex();
	
	drawingType = SHADED_POINT_SPRITE;
	setInteractionMode(MODE_INTERVIEW);
}

void ParticleSystem::init(ofVec2f simulationPixelSize)
{
	// setup fluid stuff
	fluidSolver.setup(50, 50); //fluidSolver.setup(100, 100);
    fluidSolver.enableRGB(false);
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
	
	for( int i = 0; i < MAX_PARTICLES; i++) {
		ofVec2f pos(ofRandom(windowSize.x), ofRandom(windowSize.y));
		addForceAndParticle(pos, false, false);
	}
}

void ParticleSystem::setInteractionMode(int mode)
{
	interactionMode = mode;
	switch (mode) {
		case MODE_BUZZ:
			currentImage = &buzzPointSprite;
			break;
		case MODE_INTERVIEW:
			currentImage = &interviewPointSprite;
			break;
		case MODE_INSPIRATION:
			currentImage = &inspirationPointSprite;
			break;
		case MODE_VOTE:
			currentImage = &votePointSprite;
			break;
		case MODE_PERFORMANCE:
			currentImage = &performancePointSprite;
			break;
		default:
			break;
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
			currentImage->bind();
			
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
			
			currentImage->unbind();
			
			// Clean up
			glDisableClientState(GL_VERTEX_ARRAY); 
			glDisable(GL_POINT_SPRITE);
			glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // put blending back
		}
		break;
			
		case SHADED_POINT_SPRITE:
		{

			ofEnableAlphaBlending();
			ofSetColor(particleColor);
			
			shader.begin(); // Turn on the Shader
			shader.setUniform1f("maxSpriteSize", maxSpriteSize);

			//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			// Get the attribute and bind it
			GLint pixel_loc = glGetAttribLocation(shader.getProgram(), "pointSize");
			glVertexAttribPointer(pixel_loc, 4, GL_FLOAT, GL_TRUE, 0, heightArray);
			glBindAttribLocation(shader.getProgram(), pixel_loc, "pointSize");
			glEnableVertexAttribArray(pixel_loc);
			
			glDisable(GL_DEPTH_TEST);
			//glEnable(GL_ALPHA);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			
			currentImage->bind();
			
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
			
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, posArray);
			
			//glEnableClientState(GL_COLOR_ARRAY);
			//glColorPointer(3, GL_FLOAT, 0, colArray);
			
			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES * 2);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			//glDisableClientState(GL_COLOR_ARRAY);
				
			currentImage->unbind();
			
			// Clean up			
			glDisable(GL_POINT_SPRITE);
			glDisableVertexAttribArray(pixel_loc);
			glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
			
			shader.end();
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // put blending back

			
		}
		break;
	}
	
}

void ParticleSystem::update()
{	
	
	fluidSolver.update();
	
	/*
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
	}*/
	
	// can't add new ones b/c fluid solver breaks?
	/* 
	 if(ofGetFrameNum() % 5 == 0 ) {
		
		ofVec2f pos(ofRandom(windowSize.x), ofRandom(windowSize.y));
		ofVec2f vel(sin(ofGetFrameNum()), cos(ofGetFrameNum()));
		addForceAndParticle(pos, vel, true, true);
		
	}*/
	
	// update each particle
	for(int i = 0; i<MAX_PARTICLES-1; i++) {
		if(particles[i].alpha > 0) {
			
			particles[i].update( fluidSolver, windowSize, invWindowSize, maxSpriteSize );
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

void ParticleSystem::addForceAndParticle( ofVec2f pos, bool addColor, bool addForce ) 
{
	//balance the x and y components of speed with the screen aspect ratio
	float aspectRatio = windowSize.x/windowSize.y;
	ofVec2f vel = pos - prevForcePosition;
	float speed = vel.x * vel.x  + vel.y * vel.y * aspectRatio * aspectRatio;
	
	ofVec2f constrainPos(
						 ofMap(pos.x, 0, windowSize.x, 0.f, 1.f, true),
						 ofMap(pos.y, 0, windowSize.y, 0.f, 1.f, true) 
						 );
	
	const float velocityMult = 0.5; //30;
	
	int index = fluidSolver.getIndexForPos(constrainPos);

	fluidSolver.addForceAtIndex(index, vel * velocityMult);
	addParticle( pos  );
	
	// store for next force
	prevForcePosition = pos;
	
    /*if(speed > 0) {
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
		addParticles( pos, 1 );
    }*/
}


void ParticleSystem::addParticles( const ofVec2f &pos, int count ){
	for(int i=0; i<count; i++)
		addParticle( pos + ofVec2f(ofRandom(50), ofRandom(50)) );
}


void ParticleSystem::addParticle( const ofVec2f &pos ) {
	
	if(curIndex < MAX_PARTICLES) 
	{
		if(useGravity) {
			particles[curIndex].init( pos.x, pos.y, ofVec2f(ofRandom(400) + 800, ofRandom(400) + 800));
		} else {
			particles[curIndex].init( pos.x, pos.y );
		}
		curIndex++;
	}

	if(curIndex == MAX_PARTICLES) {
		curIndex = 0;
	}
}

