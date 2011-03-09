/*
 *  Champagne.cpp
 *  motionFluid
 *
 *  Created by Josh Nimoy on 3/8/11.
 *
 */

#include "Champagne.h"

Champagne::Champagne(){
	drawSphere = false;	
	emitter.set(500, ofGetHeight(), 0);
}

//----------------------------------------------------------------------

void Champagne::update(){
	vector<ofVec3f>::iterator dObj = disturbingObjects.begin();
	
	while(dObj != disturbingObjects.end())
	{
		if( dObj->y > 0 ) {
			dObj->y -= 7.f;
			if( ofRandomuf() < .4f) {
				float scale = min(ofGetHeight() / dObj->y, 10.f);
				particles.push_back( new BrownianObject( *dObj, scale + 2));
			}
			++dObj;
		} else {
			dObj = disturbingObjects.erase(dObj);
		}
	}
	
	
	if(ofRandomuf() < 0.1)
	{
		
		particles.push_back( new BrownianObject( emitter, 4));
	}
	
	vector<BrownianObject*>::iterator it = particles.begin();
	while(it != particles.end())
	{
		if( (*it)->isDead()) {
			if(*it) {
				delete *it;
			}
			it = particles.erase(it);
		} else {
			it++;
		}
	}

}

//----------------------------------------------------------------------


void Champagne::draw(){
	ofSetColor(255, 255, 255);
	
	/*
	vector<ofVec3f>::iterator dObj = disturbingObjects.begin();
	while(dObj != disturbingObjects.end())
	{
		glPushMatrix();
		glTranslatef(dObj->x, dObj->y, dObj->z);
		ofCircle(0,0,15);
		glPopMatrix();
		++dObj;
	}
	*/
	
	vector<BrownianObject*>::reverse_iterator it = particles.rbegin();
	while(it != particles.rend())
	{
		(*it)->draw();
		it++;
	}
	
}

//----------------------------------------------------------------------


void Champagne::createObjectAtPoint(ofVec3f point){
	
	disturbingObjects.push_back(point);
	
}



