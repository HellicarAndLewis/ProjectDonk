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
	pointSpriteTexture.allocate(128, 128, GL_RGBA, false);
	ofImage loader;
	loader.loadImage("pointSpriteImg.png");
	pointSpriteTexture.loadData(loader.getPixels(), 128, 128, GL_RGBA);
	
	drawingType = POINT_SPRITE;
	
	glPointSize(31.f);
	
}

void ParticleSystem::setWindowSize( ofVec2f winSize )
{
	windowSize = winSize;
	invWindowSize = ofVec2f( 1.0f / winSize.x, 1.0f / winSize.y );
}

void ParticleSystem::updateAndDraw( bool drawingFluid ){

	
	for(int i=0; i<MAX_PARTICLES; i++) {
		if(particles[i].alpha > 0) {
			particles[i].update( *solver, windowSize, invWindowSize );
			
			//cout << particles[i].radius << endl;
			//particles[i].updateVertexArrays( drawingFluid, invWindowSize, i, posArray, colArray);
			particles[i].updateVertexArrays( true, invWindowSize, i, posArray, colArray);
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
			glEnable(GL_BLEND);
			//glDisable( GL_TEXTURE_2D );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			GLfloat fSizes[2];
			glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, fSizes);
			
			//shader.begin(); // Turn on the Shader
			
			GLfloat pointSizes[1];
			pointSizes[0] = fSizes[1];
			// Get the attribute and bind it
			/*GLint pixel_loc = glGetAttribLocation(shader.getProgram(), "pointSize");
			glVertexAttribPointer(pixel_loc, 1, GL_FLOAT, false, 0, &pointSizes);
			glBindAttribLocation(shader.getShader(GL_VERTEX_SHADER), pixel_loc, "pointSize");
			glEnableVertexAttribArray(pixel_loc);*/
			
			glDisable(GL_DEPTH_TEST);
			pointSpriteTexture.bind();
			
			
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);      // Enable Vertex Points
														 // Enable the Vertex Array and PixelSize Attribute
			
			//glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, posArray);
			
			//glEnableClientState(GL_COLOR_ARRAY);
			//glColorPointer(3, GL_FLOAT, 0, colArray);
			
			//glDrawArrays(GL_TRIANGLE_STRIP, 0, MAX_PARTICLES * 2);
			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES * 2);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			//glDisableClientState(GL_COLOR_ARRAY);
			
			pointSpriteTexture.unbind();
			
			// Clean up
			glDisableClientState(GL_VERTEX_ARRAY); 
			glDisable(GL_POINT_SPRITE);
			//glDisableVertexAttribArrayARB(pixel_loc);
			//shader.end();
		break;
			
		case SHADED_POINT_SPRITE:
		{
			//glEnable(GL_BLEND);
			//glDisable( GL_TEXTURE_2D );
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			
			GLfloat fSizes[2];
			glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, fSizes);
			
			//shader.begin(); // Turn on the Shader
			
			GLfloat pointSizes[1];
			pointSizes[0] = fSizes[1];
			// Get the attribute and bind it
			/*GLint pixel_loc = glGetAttribLocation(shader.getProgram(), "pointSize");
			 glVertexAttribPointer(pixel_loc, 1, GL_FLOAT, false, 0, &pointSizes);
			 glBindAttribLocation(shader.getShader(GL_VERTEX_SHADER), pixel_loc, "pointSize");
			 glEnableVertexAttribArray(pixel_loc);*/
			
			glDisable(GL_DEPTH_TEST);
			pointSpriteTexture.bind();
			
			
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
			glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);      // Enable Vertex Points
														 // Enable the Vertex Array and PixelSize Attribute
			
			glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 0, posArray);
			
			//glEnableClientState(GL_COLOR_ARRAY);
			//glColorPointer(3, GL_FLOAT, 0, colArray);
			
			//glDrawArrays(GL_TRIANGLE_STRIP, 0, MAX_PARTICLES * 2);
			glDrawArrays(GL_POINTS, 0, MAX_PARTICLES * 2);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			//glDisableClientState(GL_COLOR_ARRAY);
			
			pointSpriteTexture.unbind();
			
			// Clean up
			glDisableClientState(GL_VERTEX_ARRAY); 
			glDisable(GL_POINT_SPRITE);
			//glDisableVertexAttribArrayARB(pixel_loc);
			//shader.end();
		}
		break;
	}
	
}


void ParticleSystem::addParticles( const ofVec2f &pos, int count ){
	for(int i=0; i<count; i++)
		//addParticle( pos + Rand::randofVec2f() * 15 );
		addParticle( pos + ofVec2f(ofRandom(15), ofRandom(15)) );
}


void ParticleSystem::addParticle( const ofVec2f &pos ) {
	particles[curIndex].init( pos.x, pos.y );
	curIndex++;
	if(curIndex >= MAX_PARTICLES) curIndex = 0;
}
