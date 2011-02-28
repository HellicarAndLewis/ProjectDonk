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
		
		glPopMatrix();
		
	}
}




