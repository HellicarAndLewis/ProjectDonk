/*
 *  ParticleSystem.cpp
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "ParticleSystem.h"

using namespace MSA;

ParticleSystem::ParticleSystem() 
{
	curIndex = 0;
	setWindowSize( ofVec2f( 1, 1 ) );
	
	shader.setup("pointShader.vs", "pointShader.fs" );
	
	ofDisableArbTex();
	starImage.loadImage("pointSpriteImg.png");
	ofEnableArbTex();
	
	drawingType = SHADED_POINT_SPRITE;	
}

void ParticleSystem::setWindowSize( ofVec2f winSize )
{
	windowSize = winSize;
	invWindowSize = ofVec2f( 1.0f / winSize.x, 1.0f / winSize.y );
}

void ParticleSystem::updateAndDraw( bool drawingFluid ){

	
	for(int i=0; i<MAX_PARTICLES - 1; i++) {
		if(particles[i].alpha > 0) {
			particles[i].update( *solver, windowSize, invWindowSize );
			
			//cout << particles[i].radius << endl;
			//particles[i].updateVertexArrays( drawingFluid, invWindowSize, i, posArray, colArray);
			particles[i].updateVertexArrays( false, invWindowSize, i, posArray, colArray, heightArray);
		}
	}
	
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
			
			glDisable(GL_BLEND);
		break;
	
		case POINT_SPRITE:
		{
			glEnable(GL_BLEND);
			//glDisable( GL_TEXTURE_2D );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			glDisable(GL_DEPTH_TEST);
			starImage.bind();
			
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
			
			starImage.unbind();
			
			// Clean up
			glDisableClientState(GL_VERTEX_ARRAY); 
			glDisable(GL_POINT_SPRITE);
		}
		break;
			
		case SHADED_POINT_SPRITE:
		{
			
			shader.begin(); // Turn on the Shader
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			// Get the attribute and bind it
			GLint pixel_loc = glGetAttribLocation(shader.getProgram(), "pointSize");
			glVertexAttribPointer(pixel_loc, 4, GL_FLOAT, false, 0, heightArray);
			glBindAttribLocation(shader.getProgram(), pixel_loc, "pointSize");
			glEnableVertexAttribArray(pixel_loc);
			
			glDisable(GL_DEPTH_TEST);
			glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			
			starImage.bind();
			
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);      
			
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, posArray);
			
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, colArray);
			
			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES * 2);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			
			starImage.unbind();
			
			// Clean up
			glDisableClientState(GL_VERTEX_ARRAY); 
			glDisable(GL_POINT_SPRITE);
			glDisableVertexAttribArray(pixel_loc);
			shader.end();
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
		//return;
	}
}
