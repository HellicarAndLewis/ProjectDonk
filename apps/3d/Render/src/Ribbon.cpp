//
//  Ribbon.cpp
//  Render
//
//  Created by James George on 3/13/11.
//  Copyright 2011 No rights reserved.
//

#include "Ribbon.h"

Ribbon::Ribbon()
{
    //maybe put initializers here?
}

Ribbon::~Ribbon()
{
    //clean it up
    for(int i = 0; i < segments.size(); i++){
        delete segments[i];
    }
}

void Ribbon::update()
{
    float thisTime = ofGetElapsedTimef();
    //add new ribbon bits
    for(int i = 0; i < segmentsPerFrame; i++){
        
        //interpolate time for smooth fade out action
        float currentTime = ofMap(i, 0, segmentsPerFrame, lastUpdate, thisTime, false);
        
        RibbonSegment* ribbon = new RibbonSegment();
        ribbon->timeAdded = currentTime;
        ribbon->node = head;
                
        //veer perlinly by picking a point in space and looking at it
        ofVec3f noiseSample = head.getPosition() / noiseDamp;
        ofVec3f turbulence = ofSignedNoise(noiseSample.x, noiseSample.y, noiseSample.z) * noiseAmp;
        ofVec3f outThere = head.getPosition() + head.getLookAtDir()*5 + turbulence;
        //curl the up vector around as we go, wobbling a bit for variance
        ofVec3f newUp = head.getUpDir().getRotated(curlySpeed + ofSignedNoise(noiseSample.x)*.2, head.getLookAtDir());
        
        head.lookAt(outThere, newUp);
        
        //onward!
        head.dolly( speed );
        
        segments.push_back( ribbon );
    }
    
    //the newest ribbon segment is at the end
    for(int i = 1; i < segments.size(); i++){
        ofNode nextNode = (i == segments.size()-1) ? head : segments[i+1]->node;
        segments[i]->update( nextNode );
    }
    
    for(int i = segments.size()-1; i >= 0; i--){
        if( (thisTime - segments[i]->timeAdded) > maxSegmentAge){
            delete segments[i];
            segments.erase(segments.begin()+i);
        }
    }
    
    lastUpdate = thisTime;
}

bool Ribbon::isDead()
{
    return ofGetElapsedTimef() - ribbonCreatedTime > maxRibbonAge;
}

void Ribbon::draw()
{
    float thisTime = ofGetElapsedTimef();
    
    //TODONK take out immediate mode and use a vertex array
    glBegin(GL_TRIANGLE_STRIP);
    
    for(int i = 0; i < segments.size(); i++){
        ofNode thisNode = segments[i]->node;
        ofNode nextNode = (i == segments.size()-1) ? head : segments[i+1]->node;

        //color and size attenuation
        //the further away each node is the thinner the line
        float thickness = ofMap(thisNode.getPosition().distance( nextNode.getPosition() ),
                                10, 100,
                                maxThickness, minThickness, true);
        //attenuate thickness if we are near the tails
        int tailTaper = 30;
        thickness *= ofMap(i, segments.size() - MIN(tailTaper,segments.size()), segments.size(), 1.0, 0.0, true);
        thickness *= ofMap(i, 0, MAX(tailTaper,segments.size()), 0.0, 1.0, true);
        
        //make fadies-action
        float alpha = 1.0;
        //attenuate the begining just a tad
        alpha *= ofMap(i, segments.size() - 10, segments.size(), 1.0, 0.0, true);
        //attenuate as the segment dies
        alpha *= ofMap(thisTime-segments[i]->timeAdded, maxSegmentAge, maxSegmentAge*.1, 0.0, 1.0, true);
        //attenuate as the ribbon itself dies
        alpha *= ofMap(thisTime-ribbonCreatedTime, maxRibbonAge, maxRibbonAge*.1, 0.0, 1.0, true);
        
        //add geometry
        //find the left and right vectors
        ofVec3f forward = thisNode.getLookAtDir();
        ofVec3f left = thisNode.getSideDir();
        ofVec3f right = left.getRotated(180, forward);
        glColor4f(color.r, color.g, color.b, alpha);
        ofVec3f leftPoint  = thisNode.getPosition() + left * thickness / 2.0;
        ofVec3f rightPoint = thisNode.getPosition() + right * thickness / 2.0;
        glVertex3f(leftPoint.x, leftPoint.y, leftPoint.z);
        glVertex3f(rightPoint.x, rightPoint.y, rightPoint.z);

    }
    
    glEnd();
    
}
