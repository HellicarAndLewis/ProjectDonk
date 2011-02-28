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
}

//--------------------------------------------------------------
void ContentBubble::createContentBubble() {
	
	color.r = 255;// ofRandom(0, 255);
	color.g = 255;
	color.b = 255;
	color.a = 25;
	
	
	/*
	
	ofDisableArbTex();
	
	//cout << glGetString(GL_EXTENSIONS) << endl;
	ofEnableNormalizedTexCoords();
	
	permImg.loadImage("shader/bubble/texturing.jpg");
	permImg.setImageType(OF_IMAGE_COLOR);
	glossImg.loadImage("shader/bubble/permutationTexture.jpg");
	glossImg.setImageType(OF_IMAGE_COLOR);
	
	ofEnableArbTex();
	
	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);	
	
	sphereCenter = ofVec3f(0, 0, 0);
	cam.setTarget(sphereCenter);
	cam.setDistance(300);
	
	cubeshader.setup("shader/bubble/fresnel_refraction.vs", "shader/bubble/fresnel_refraction.fs");
	
	cubeMap.loadImages("shader/bubble/skybox/berkeley_positive_x.png",
					   "shader/bubble/skybox/berkeley_positive_y.png",
					   "shader/bubble/skybox/berkeley_positive_z.png",
					   "shader/bubble/skybox/berkeley_negative_x.png",
					   "shader/bubble/skybox/berkeley_negative_y.png",
					   "shader/bubble/skybox/berkeley_negative_z.png");
	
	//cubeMap.enableFixedMapping();
	
	xLightPos = -1;
	yLightPos = -1;
	zLightPos = 1;
	inc = true;
	//hasDrawnTex = false;*/
	
}

//--------------------------------------------------------------
void ContentBubble::update() {
	
	
	if(rigidBody->isBody()) {
		
		rigidBody->body->setDamping(0.96, 0.96);
		
		ofVec3f frc = target-rigidBody->getPosition();
		frc.normalize();
		frc *= 1000.0;
		
		rigidBody->body->clearForces();
		rigidBody->body->applyCentralForce(btVector3(frc.x, frc.y, frc.z));
		
	}
	
	//target.y += 0.5;
	//if(target.y > 500) target.y = -500;
}

//--------------------------------------------------------------
void ContentBubble::drawGlassBubble() {

	
	
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	
	cubeshader.begin();
	
	glActiveTexture(GL_TEXTURE0);
	unsigned int texId1 = permImg.getTextureReference().getTextureData().textureID;
	unsigned int texTarget1 = permImg.getTextureReference().getTextureData().textureTarget;  
	glBindTexture(texTarget1, texId1);
	
	glActiveTexture(GL_TEXTURE1);
	unsigned int texId2 = permImg.getTextureReference().getTextureData().textureID;
	unsigned int texTarget2 = glossImg.getTextureReference().getTextureData().textureTarget;  
	glBindTexture(texTarget2, texId2);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, cubeMap.textureObject);
	
	cubeshader.setUniform1i("glossMap", 0);
	cubeshader.setUniform1i("baseMap", 1);
	cubeshader.setUniform1i("environmentMap", 2);
	
	cubeshader.setUniform1f("EdgeFalloff", 0.2f);
	
	float* eyeVector = new float[3];
	eyeVector[0] = ofGetWidth()/2;
	eyeVector[1] = ofGetHeight()/2;
	eyeVector[2] = 1;
	cubeshader.setUniform3fv("eyeVector", eyeVector);
	
	float* lightPosition = new float[3];
	lightPosition[0] = xLightPos;
	lightPosition[1] = yLightPos;
	lightPosition[2] = zLightPos;
	cubeshader.setUniform3fv("lightVector", lightPosition);
	
	cubeshader.setUniform1f("reflectAmount", 0.6f);
	
	float* pos = new float[3];
	pos[0] = 0.5;
	pos[1] = 0.5;
	pos[2] = 0.5;
	cubeshader.setUniform3fv("fresnelValues", pos);
	
	float* cpos = new float[3];
	cpos[0] = modelview[3];//0.5;
	cpos[1] = modelview[7];//0.5;
	cpos[2] = modelview[11];//0.5;
	cubeshader.setUniform3fv("CameraPos", cpos);
	
	float* lPos = new float[3];
	lPos[0] = 0.5;
	lPos[1] = 0.5;
	lPos[2] = 0.5;
	cubeshader.setUniform3fv("IoR_Values", lPos);
	
	//cubeshader.setUniform4mat("ModelWorld4x4", mat);
	cubeshader.setUniform4mat("ModelWorld4x4", &modelview[0]);
	
	/*renderGluSphereWithTexCoords(10, 0, -100, 40);
	renderGluSphereWithTexCoords(10, 30, -200, 40);
	renderGluSphereWithTexCoords(20, 40, -10, 40);
	*/
	
	cubeshader.end();
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	
	delete pos;
	delete lPos;
	delete eyeVector;
	delete lightPosition;
	//delete mat;
	
}

//--------------------------------------------------------------
void ContentBubble::draw() {
	
	ofSetColor(255, 255, 0);
	drawSphere(target, 3, 10);
	
	if(rigidBody->isBody()) {
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		
		rigidBody->body->getWorldTransform().getOpenGLMatrix(m);
		
		btSphereShape * sphereShape = (btSphereShape*)rigidBody->shape;
		float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
		
		glPushMatrix();
		glTranslatef(0, 0, 0);
		glMultMatrixf(m);
		
		ofEnableAlphaBlending();
		
		//ofSetColor(255, 255, 255);
		//ofFill();
		//drawSphere(ofVec3f(0, 0, 0), radius/3.0);
		data->radius = radius;
		data->draw();
		
		ofSetColor(color);
		ofFill();
		drawSphere(ofVec3f(0, 0, 0), radius, 20);
		
		
		
		glPopMatrix();
		
	}
}




