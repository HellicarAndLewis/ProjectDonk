//
//  RibbonSegment.h
//  Render
//
//  Created by James George on 3/13/11.
//  Copyright 2011 No rights reserved.
//
#pragma once
#include "ofMain.h"

class RibbonSegment
{
  public:
    ofNode node;
    void update(ofNode inFront);
    float timeAdded;
};