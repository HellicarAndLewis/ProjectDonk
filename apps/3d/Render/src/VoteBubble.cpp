/*
 *  VoteBubble.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/15/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "VoteBubble.h"

VoteBubble::VoteBubble() {
	pct = 0;	
	pctDes = 0;
	pctf = 0;
}

void VoteBubble::setOptionString(string str) {
	optionStr = str;	
}

void VoteBubble::drawInsideContent() {
	
	loadFont();
	
	pctf += (pctDes-pctf) * 0.4;
	pct  = round(pctf);
	
	// side not there is something
	// really weird with drawing type
	// inside the shader not sure why...
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	glMultMatrixf(billboadMatrix);
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255);
    
	float strW = font.stringWidth(optionStr)+10.0;
	if(strW < 0) strW = 1;
	
	float scl = MIN(1.0, (radius*2) / strW) * 0.89f;
		
	glPushMatrix();
	glTranslated( -((strW*scl)/2.0), 0, 0 );
	glScalef(scl, scl, 1);
	
	ofSetColor(20, 20, 20, 230);
	font.drawString(optionStr, 3, 3);
	glPushMatrix();
	glTranslatef(0, 0, 0.01);
	ofSetColor(255, 255, 255, 255);
	font.drawString(optionStr, 0, 0);
	glPopMatrix();
	
	glPopMatrix();
	
	string pctStr   = ofToString(pct);
	float  pctWidth = font.stringWidth(pctStr);
	float  optH     = font.stringHeight(optionStr)+8.0;
	glPushMatrix();
	glTranslated( -((pctWidth+10.0)/2.0), optH, 0 );
	glScalef(1.2, 1.2, 1);
	
	ofSetColor(20, 20, 20, 230);
	
	font.drawString(pctStr, 3, 3);
	ofSetColor(255, 255, 255, 255);
	glPushMatrix();
	glTranslatef(0, 0, 0.01);
	font.drawString(pctStr, 0, 0);
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glTranslated( pctWidth-3.0, optH, 0 );
	
	glScalef(0.52, 0.52, 1);
	ofSetColor(20, 20, 20, 230);
	font.drawString("%", 3, 3);
	glPushMatrix();
	glTranslatef(0, 0, 0.01);
	ofSetColor(255, 255, 255, 255);
	font.drawString("%", 0, 0);
	glPopMatrix();
	glPopMatrix();
	
	glPopMatrix();
}