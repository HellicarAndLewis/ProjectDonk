/*
 *  VoteBubble.cpp
 *  Render
 *
 *  Created by Todd Vanderlin on 3/15/11.
 *  Copyright 2011 Interactive Design. All rights reserved.
 *
 */

#include "VoteBubble.h"

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
	
	ofSetColor(255, 255, 255);
	if(font.bLoadedOk) {
		if(optionStr == "") optionStr = "OPTION STR";
		float strW = font.stringWidth(optionStr);
		font.drawString(optionStr, -(strW/2), 0);
	}
	glPopMatrix();
}