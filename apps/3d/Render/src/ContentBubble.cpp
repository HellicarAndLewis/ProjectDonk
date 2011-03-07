/*
 *  ContentBubble.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 2/26/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "ContentBubble.h"

ofTrueTypeFont ContentBubble::font;

//--------------------------------------------------------------
ContentBubble::ContentBubble() {
	rigidBody = NULL;
	bTouched  = false;
	rotateY = 0;
	rotateYTarget = 0;
	if(ofRandomuf()>0.5){
		rotateYDirection = 180;
	}else{
		rotateYDirection = -180;
	}
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
	
	touchAlpha += (touchAlphaTarget-touchAlpha) * 0.1;
	rotateY += (rotateYTarget-rotateY) * 0.05;
	if(bTouched){
		touchAlphaTarget = 90;
		rotateYTarget = rotateYDirection;
	}else{
		touchAlphaTarget = 0;
		rotateYTarget = 0;
	}
	
}


//--------------------------------------------------------------
void ContentBubble::pushBubble() {
	
	if(rigidBody->isBody()) {
		
		pos = rigidBody->getPosition();
		rigidBody->body->getWorldTransform().getOpenGLMatrix(m);		
		btSphereShape * sphereShape = (btSphereShape*)rigidBody->shape;
		//radius doesn't include the margin, so draw with margin
		radius = sphereShape->getMargin();
		
		glPushMatrix();
		glMultMatrixf(m);
		
		// create the billboard matrix
		for (int i=0; i<16; i++) {
			billboadMatrix[i] = m[i];
		}

		// ripped from here
		// http://lighthouse3d.com/opengl/billboarding/index.php?billCheat
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
	glMultMatrixf(billboadMatrix);
	
	glRotatef(rotateY,0,1,0);
	// the twitter icon and text
	if(data) {
		//if no twitter icon, don't draw.
		if(data->profileImage.width != 0){
			
			float data_radius = radius*0.9;
			
			//lazyload the font
			if(!font.bLoadedOk){
				font.loadFont("global/font/Gotham-Bold.otf",50);
				if(font.bLoadedOk) {
					printf("--- font is loaded ---\n");	
				}
			}
			
			//draw twitter icon as a disk
			ofSetColor(255,255,255);
			data->profileImage.bind();
			float w = data->profileImage.width;
			float h = data->profileImage.height;
			//draw the circle-masked thumbnail
			int steps = 60;
			float inc = TWO_PI/(float)steps;
			glBegin(GL_TRIANGLE_FAN);
			for(int i=0;i<steps;i++){
				float x = cos(inc*i);
				float y = sin(inc*i);
				glTexCoord2f((x+1)*w*0.5,(y+1)*h*0.5);
				glVertex2f(x*data_radius,y*data_radius);
			}
			glEnd();
			data->profileImage.unbind();
			
			//draw twitter text content
			ofRectangle textBB = font.getStringBoundingBox(data->userName, 0,0);
			glPushMatrix();
			float s = data_radius/textBB.width*1.75;
			glScalef(s,s,s);
			glTranslated(-textBB.width/2, 0,0.2);
			ofSetColor(0,0,0);
			font.drawString(data->userName,0,0);
			glTranslatef(2,2,0.2);
			ofSetColor(255,255,255);
			font.drawString(data->userName,0,0);
			data->profileImage.unbind();			
			glPopMatrix();
			
		}
		
	}
	glPopMatrix();
}

//--------------------------------------------------------------
void ContentBubble::drawHighLight() {
	glPushMatrix();
	glMultMatrixf(billboadMatrix);

	
	// little touch outline...
	ofEnableAlphaBlending();
	float rad_extend = radius + 100;
	
	//draw circular hailo
	glBegin(GL_TRIANGLE_STRIP);
	int steps = 30;
	float inc = TWO_PI/steps;
	for(int i=0;i<steps+1;i++){
		float x = cos(i*inc);
		float y = sin(i*inc);
		glColor4f(1,1,1,touchAlpha/255.0);
		glVertex2f(x*radius,y*radius);
		glColor4f(1,1,1, 0);
		glVertex2f(x*rad_extend,y*rad_extend);
		
	}
	glEnd();
	glPopMatrix();
}

//--------------------------------------------------------------
void ContentBubble::draw() {
	
	// for debuggin...
	//ofSetColor(255, 255, 0);
	//drawSphere(target, 3, 10);
	
	// the main bubble sphere
	ofSetColor(color);
	ofFill();
	drawSphere(ofVec3f(0, 0, 0), radius, 60);
}

