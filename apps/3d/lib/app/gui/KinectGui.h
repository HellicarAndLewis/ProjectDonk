/*
 *  KinectGui.h
 *  KinectExample
 *
 *  Created by Marek Bereza on 24/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "Enableable.h"
#include "Kinect.h"
#include "Scene.h"
#include "ofxXmlGui.h"

class KinectGui: public Enableable {
public:
	
	KinectGui(Scene *scene);
	void mousePressed(float x, float y, int button);
	void mouseReleased(float x, float y, int button);
	void mouseDragged(float x, float y, int button);
	void mouseMoved(float x, float y);
	
	void draw();
	
private:
	void setKinects(vector<Kinect*> &kinects);
	Scene *scene;
	vector<Kinect*> kinects;
	ofxXmlGui kinectSelector;
	int currKinectIndex;
};