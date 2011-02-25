/*
 *  ProjectedRect.h
 *  audioReactive
 *
 *  Created by Marek Bereza on 19/01/2011.
 *
 */
#pragma once

#include "ofMain.h"
#include "Model.h"

// define this and it replaces the FBO with a test image.
//#define TEST_IMAGE

/**
 * \brief This is a texture that will wrap itself onto a 3d model.
 *
 * You subclass it, then implement draw() which fills the FBO
 * when render() is called by your app, and then feed drawOnModel() 
 * with the model you want to draw it onto.
 */
class ProjectedRect {
public:


	/**
	 * Called after texture is allocated (in setup)
	 */
	virtual void setup() {}
	
	virtual void update() {}
	
	/**
	 * implement this to draw your particle system or whatever
	 */ 
	virtual void draw() {}
	
	
	
	/**
	 * Decide how big you want it.
	 */
	void allocate(float width, float height);	
	
	/**
	 * Call this with a model to get it to draw
	 * the texture onto the model. glTexCoords
	 * should be normalized, then this class will scale
	 * them up to fit the texture size.
	 */
	void drawOnModel(Model *model);
	
	/**
	 * Call this to render the graphics into the fbo.
	 * This should be called once a frame (i.e. not 
	 * in drawView() which is called once per camera.)
	 */
	void render();	
	
	
	float getWidth();	
	float getHeight();
	
	
	// binds the texture
	void bind();
	void unbind();	

	
	
private:
#ifdef TEST_IMAGE
	ofImage testImage;
#endif
	
	ofFbo fbo;

protected:
	float width;
	float height;
};