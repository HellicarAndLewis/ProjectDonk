/*
 *  Champagne.cpp
 *  motionFluid
 *
 *  Created by Josh Nimoy on 3/8/11.
 *
 */

#include "Champagne.h"
#include "testApp.h"

Champagne::Champagne(){
	drawSphere = false;
	
}

void Champagne::setup()
{
	ofRectangle a = testApp::instance->projection->getInteractiveArea();
	
	emitter.set(a.x + a.width/2,a.y + a.height * 1.3 ,0);
	
	//play out the simulation a bit
	//for(int i=0;i<2000;i++)update();
	for(int i=0;i<500;i++)update();
}	

void Champagne::update(){
	vector<ofVec3f>::iterator dObj = disturbingObjects.begin();
	
	while(dObj != disturbingObjects.end()){
		if( dObj->y > 0 ) {
			dObj->y -= 8.f;
			if( ofRandomuf() < .1f) {
				float scale = min(ofGetHeight() / dObj->y, 25.f);
				particles.push_back( new BrownianObject( *dObj, scale + 2));
			}
			++dObj;
		} else {
			dObj = disturbingObjects.erase(dObj);
		}
	}
	
	/*
	 //emit a bubble from the emitter
	if(ofRandomuf() < 0.01){
		particles.push_back( new BrownianObject( emitter, 32));
	}
	 */
	
	vector<BrownianObject*>::iterator it = particles.begin();
	
	while(it != particles.end()){
		if( (*it)->isDead()) {
			if(*it) {
				//cout << "deleting " << endl;
				delete *it;
			}
			it = particles.erase(it);
		} else {
			it++;
		}
	}

	for(it = particles.begin();it!=particles.end();it++) (*it)->update();
	
}

void Champagne::draw()
{
	ofSetColor(255, 255, 255);
	
	vector<ofVec3f>::iterator dObj = disturbingObjects.begin();
	while(dObj != disturbingObjects.end())
	{
		glPushMatrix();
		glTranslatef(dObj->x, dObj->y, dObj->z);
		ofCircle(0,0,15);
		glPopMatrix();
		++dObj;
	}
	
	
	vector<BrownianObject*>::iterator it;
	for(it=particles.begin();it != particles.end();it++){
		(*it)->draw();
	}
	
}

void Champagne::createObjectAtPoint(ofVec3f point){
	
	disturbingObjects.push_back(point);
	
}



