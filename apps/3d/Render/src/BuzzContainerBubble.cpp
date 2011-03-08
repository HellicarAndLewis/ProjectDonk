/*
 *  BuzzContainerBubble.cpp
 *  Render
 *
 *  Created by Chris on 3/8/11.
 *  Copyright 2011 csugrue. All rights reserved.
 *
 */

#include "BuzzContainerBubble.h"

BuzzContainerBubble::BuzzContainerBubble(){
	
	ContentBubble::ContentBubble();
}

void BuzzContainerBubble::createContainerBubble(ofxBullet * bullet){
	
	ContentBubble::createContentBubble();
	
	
}

void BuzzContainerBubble::pop(){
	
	if(bAlive)
	{
		cout << "POPPED" << endl;
		bAlive = false;
	}
}





