//
//  RibbonSegment.cpp
//  Render
//
//  Created by James George on 3/13/11.
//  Copyright 2011 . No rights reserved.
//

#include "RibbonSegment.h"

void RibbonSegment::update(ofNode inFront)
{
    //damply move towards the front;
    ofVec3f newPosition = node.getPosition() + (inFront.getPosition() - node.getPosition()) * .1;
    
    //almost there...
    node.setPosition( newPosition );
    
    //stay on target
    node.lookAt(inFront, node.getUpDir());
    
}