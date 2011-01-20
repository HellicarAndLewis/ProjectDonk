/*
 *  ProjectedRect.cpp
 *  audioReactive
 *
 *  Created by Marek Bereza on 20/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#include "ProjectedRect.h"


void ProjectedRect::setup(float width, float height) {
	
	this->width = width;
	this->height = height;
	fbo.setup(width, height, GL_RGB);
#ifdef TEST_IMAGE
	testImage.loadImage("UV.jpeg");
	this->width = testImage.getWidth();
	this->height = testImage.getHeight();
#endif		
}


void ProjectedRect::bind() {
#ifdef TEST_IMAGE
	testImage.bind();
#else
	fbo.getTexture(0).bind();
#endif
}

void ProjectedRect::unbind() {
#ifdef TEST_IMAGE
	testImage.unbind();
#else
	fbo.getTexture(0).unbind();
#endif
}

float ProjectedRect::getWidth() {
	return width;
}

float ProjectedRect::getHeight() {
	return height;
}
void ProjectedRect::render() {
	
	fbo.begin();
	//ofSetupScreen();
	ofClear(1, 1, 0, 0.5);
	this->draw();
	fbo.end();
}

void ProjectedRect::drawOnModel(Model *model) {
	
	ofViewport();
	// scale the texture matrix so we can use normalized tex coords
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glScalef(width, height, 1);
	glMatrixMode(GL_MODELVIEW);
	
	
	// draw the model (assuming it also draws normalized texCoords
	bind();
	glColor3f(1, 1, 1);
	model->draw();
	unbind();
	
	// reset the texture matrix
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

