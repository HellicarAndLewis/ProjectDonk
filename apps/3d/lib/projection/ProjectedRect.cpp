/*
 *  ProjectedRect.cpp
 *  audioReactive
 *
 *  Created by Marek Bereza on 20/01/2011.
 *
 */

#include "ProjectedRect.h"


void ProjectedRect::allocate(float width, float height) {
	
	this->width = width;
	this->height = height;
	fbo.setUseMipmapping(true);
	fbo.setup(width, height, GL_RGBA);//,4);
#ifdef TEST_IMAGE
	testImage.loadImage("global/img/UV.jpeg");
	this->width = testImage.getWidth();
	this->height = testImage.getHeight();
#endif		
	setup();
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
	this->draw();
	fbo.end();
}

void ProjectedRect::drawOnModel(Model *model) {
	
	//glViewport(0, 0, ofGetWidth()*2, ofGetHeight()*2);
	// scale the texture matrix so we can use normalized tex coords
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glScalef(width, -height, 1);
	glTranslatef(0, -1, 0);
	glMatrixMode(GL_MODELVIEW);
	
	
	// draw the model (assuming it also draws normalized texCoords
	bind();
	glColor3f(1, 1, 1);
	model->drawModel();
	unbind();
	
	// reset the texture matrix
	glMatrixMode(GL_TEXTURE);
//	glLoadIdentity();
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

