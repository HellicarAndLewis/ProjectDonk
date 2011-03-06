/*
 *  ContentBubble.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 2/26/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ContentBubble.h"

//--------------------------------------------------------------
ContentBubble::ContentBubble() {
	rigidBody = NULL;
	bTouched  = false;
}

//--------------------------------------------------------------
void ContentBubble::createContentBubble() {
	
	color.r = 255;// ofRandom(0, 255);
	color.g = 255;
	color.b = 255;
	color.a = 25;
	
	touchAlphaTarget = 0;
	touchAlpha		 = 0;
	
	
	
	ofDisableArbTex();
	//cout << glGetString(GL_EXTENSIONS) << endl;
	ofEnableNormalizedTexCoords();
	
	permImg.loadImage("shader/texturing.jpg");
	permImg.setImageType(OF_IMAGE_COLOR);
	glossImg.loadImage("shader/permutationTexture.jpg");
	glossImg.setImageType(OF_IMAGE_COLOR);
	
	ofEnableArbTex();
	
	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);	
	
	shader.setup("shader/fresnel_refraction.vs", "shader/fresnel_refraction.fs");
	
	cubeMap.loadImages("shader/skybox/berkeley_positive_x.png",
					   "shader/skybox/berkeley_positive_y.png",
					   "shader/skybox/berkeley_positive_z.png",
					   "shader/skybox/berkeley_negative_x.png",
					   "shader/skybox/berkeley_negative_y.png",
					   "shader/skybox/berkeley_negative_z.png");
	
	//cubeMap.enableFixedMapping();
	//hasDrawnTex = false;
	
}

//--------------------------------------------------------------
void ContentBubble::update() {
	
	
	if(rigidBody->isBody()) {
		
		rigidBody->body->setDamping(0.99, 0.99); // <-- add some crazy damping
		
		ofVec3f frc = target - rigidBody->getBulletPosition();
		float d = ABS(frc.length());
		d *= 20.0;
		frc.normalize();
		frc *= d;
		
		rigidBody->body->clearForces();
		rigidBody->body->applyCentralForce(btVector3(frc.x, frc.y, frc.z));
		
	}
	
	
	touchAlpha += (touchAlphaTarget-touchAlpha) * 0.25;
	if(bTouched) touchAlphaTarget = 200;
	else	     touchAlphaTarget = 0;
	
}

//--------------------------------------------------------------
void ContentBubble::draw() {
	
	ofSetColor(255, 255, 0);
	drawSphere(target, 3, 10);
	
	
	
	
	if(rigidBody->isBody()) {
		
		
		ofVec3f pos = rigidBody->getPosition();
		rigidBody->body->getWorldTransform().getOpenGLMatrix(m);		
		btSphereShape * sphereShape = (btSphereShape*)rigidBody->shape;
		radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
		radius *= SCALE;
		
		// little touch outline...
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(255, 255, 255, touchAlpha);
		ofCircle(pos, radius*1.6);
		
		ofCircle(pos, 100);
		
		// the sphere and twitter icon
		glPushMatrix();
		glTranslatef(0, 0, 0);
		glMultMatrixf(m);
		data->radius = radius;
		data->draw();
		
		ofSetColor(color);
		ofFill();
		drawSphere(ofVec3f(0, 0, 0), radius, 20);
		renderSphere();
		
		glPopMatrix();
		
	}
}

/*void ContentBubble::renderSphere()
{
	
	glEnable(GL_TEXTURE_CUBE_MAP);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	
	shader.begin();
	
	glActiveTexture(GL_TEXTURE0);
	unsigned int texId1 = permImg.getTextureReference().getTextureData().textureID;
	unsigned int texTarget1 = permImg.getTextureReference().getTextureData().textureTarget;  
	glBindTexture(texTarget1, texId1);
	
	glActiveTexture(GL_TEXTURE1);
	unsigned int texId2 = glossImg.getTextureReference().getTextureData().textureID;
	unsigned int texTarget2 = glossImg.getTextureReference().getTextureData().textureTarget;  
	glBindTexture(texTarget2, texId2);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMap.textureObject);
	
	shader.setUniform1i("glossMap", 0);
	shader.setUniform1i("baseMap", 1);
	shader.setUniform1i("environmentMap", 2);
	
	shader.setUniform1f("EdgeFalloff", 0.2f);
	
	float* eyeVector = new float[3];
	eyeVector[0] = ofGetWidth()/2;
	eyeVector[1] = ofGetHeight()/2;
	eyeVector[2] = 1;
	shader.setUniform3fv("eyeVector", eyeVector);
	
	float* lpos = new float[3];
	lpos[0] = lightPosition.x;
	lpos[1] = lightPosition.y;
	lpos[2] = lightPosition.z;
	shader.setUniform3fv("lightVector", lpos);
	
	shader.setUniform1f("reflectAmount", 0.6f);
	
	float* pos = new float[3];
	pos[0] = 0.5;
	pos[1] = 0.5;
	pos[2] = 0.5;
	shader.setUniform3fv("fresnelValues", pos);
	
	float* cpos = new float[3];
	cpos[0] = modelview[3];//0.5;
	cpos[1] = modelview[7];//0.5;
	cpos[2] = modelview[11];//0.5;
	shader.setUniform3fv("CameraPos", cpos);
	
	float* lPos = new float[3];
	lPos[0] = 0.5;
	lPos[1] = 0.5;
	lPos[2] = 0.5;
	shader.setUniform3fv("IoR_Values", lPos);
	
	shader.setUniform4mat("ModelWorld4x4", &modelview[0]);
	//gluSphere(quadratic, rigidBody->boxSize.getX(), rigidBody->boxSize.getX(), rigidBody->boxSize.getX());
	gluSphere(quadratic, 100, 30, 30);
	shader.end();
	
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_CUBE_MAP);
	glDisable(GL_TEXTURE_2D);
	
	glActiveTexture(GL_TEXTURE0);
	
	delete pos;
	delete lPos;
	delete eyeVector;
	delete lpos;
	
	//gluSphere(quadratic, 100, 30, 30);
	
}*/




