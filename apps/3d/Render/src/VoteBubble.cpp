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
}

void VoteBubble::setOptionString(string str) {
	optionStr = str;	
}

void VoteBubble::drawInsideContent() {
	
	// side not there is something
	// really weird with drawing type
	// inside the shader not sure why...
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	glMultMatrixf(billboadMatrix);
	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255);
    
	float strW = font.stringWidth(optionStr)+10.0;
	if(strW < 0) strW = 1;
	
	float scl = (radius*2) / strW;
		
	glPushMatrix();
	glTranslated( -((strW*scl)/2.0), 0, 0 );
	glScalef(scl, scl, 1);
    //load font the first time around
    if(!font.bLoadedOk){
        font.loadFont("global/font/Gotham-Bold.otf",50);        
        printf("--- font is loaded for vote ---\n");	
    }
        
	if(font.bLoadedOk) {
		if(optionStr == "") optionStr = "OPTION STR";
		font.drawString(optionStr+"\n"+ofToString(pct)+"%", 0, 0);
	}
	glPopMatrix();
	
	glPopMatrix();
}