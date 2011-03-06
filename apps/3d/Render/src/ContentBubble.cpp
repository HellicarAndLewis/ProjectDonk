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
	
	for (int i=0; i<16; i++) {
		billboadMatrix[i] = 0;
		m[i]			  = 0;
	}
	
	/*ofDisableArbTex();
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
	 "shader/skybox/berkeley_negative_z.png"); */
	
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
void ContentBubble::pushBubble() {
	
	if(rigidBody->isBody()) {
		
		pos = rigidBody->getPosition();
		rigidBody->body->getWorldTransform().getOpenGLMatrix(m);		
		btSphereShape * sphereShape = (btSphereShape*)rigidBody->shape;
		radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
		
		glPushMatrix();
		glTranslatef(0, 0, 0);
		glMultMatrixf(m);
		
		// create the billboar matrix
		for (int i=0; i<16; i++) {
			billboadMatrix[i] = m[i];
		}
		
		// ripped from here
		// http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat
		for(int i=0; i<3; i++ ) {
			for(int j=0; j<3; j++ ) {
				if ( i==j ) billboadMatrix[i*4+j] = 1.0;
				else		billboadMatrix[i*4+j] = 0.0;
			}
		}
		
		
	}
	
}

//--------------------------------------------------------------
void ContentBubble::popBubble() {
	if(rigidBody->isBody()) {
		glPopMatrix();
	}
	
}

//--------------------------------------------------------------
void ContentBubble::drawTwitterData() {
	glPushMatrix();
	glTranslatef(0, 0, 0);
	glMultMatrixf(billboadMatrix);
	
	// the sphere and twitter icon
	if(data) {
		data->radius = radius/1.6;
		data->draw();
	}
	glPopMatrix();
}

//--------------------------------------------------------------
void ContentBubble::drawHighLight() {
	
	// little touch outline...
	ofEnableAlphaBlending();
	ofFill();
	ofSetColor(255, 255, 255, touchAlpha);
	ofCircle(pos, radius*1.6);
	ofCircle(pos, 100);
	
}

//--------------------------------------------------------------
void ContentBubble::draw() {
	
	// for debuggin...
	ofSetColor(255, 255, 0);
	drawSphere(target, 3, 10);
	
	// the main bubble sphere
	ofSetColor(color);
	ofFill();
	drawSphere(ofVec3f(0, 0, 0), radius, 20);
}





