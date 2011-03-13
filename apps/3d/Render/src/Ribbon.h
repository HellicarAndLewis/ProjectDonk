//
//  Ribbon.h
//  Render
//
//  Created by James George on 3/13/11.
//  Copyright 2011 No rights reserved.
//
#pragma once
#include "ofMain.h"
#include "RibbonSegment.h"

class Ribbon
{
  public:
    Ribbon();
    ~Ribbon();
        
    void update();
    void draw();
    ofNode head;
    float speed;
    int channel;
    
    vector<RibbonSegment*> segments;
    int segmentsPerFrame;
    float maxSegmentAge;
    float maxRibbonAge;
    float ribbonCreatedTime;

    float maxThickness;
    float minThickness;
    
    //TODO interface parametric
    float noiseDamp;
    float noiseAmp;
    ofColor color;
    float curlySpeed;
    
    bool isDead();
  protected:
    float lastUpdate;
    
};