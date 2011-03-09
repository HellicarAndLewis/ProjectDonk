/*
 *  BuzzContainerBubble.h
 *  Render
 *
 *  Created by Chris on 3/8/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#pragma once
#include "ofxBullet.h"
#include "ContentBubble.h"

class BuzzContainerBubble : public ContentBubble{
	
	public:
	
		BuzzContainerBubble();
		
		void createContainerBubble(ofxBullet * bullet);
		void pop();
		void drawTwitterData(){}
				
	private:
	
};

