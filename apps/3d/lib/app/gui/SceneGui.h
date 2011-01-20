/*
 *  SceneGui.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */
#include "ofxXmlGui.h"
#include "Scene.h"
#include "Enableable.h"


class SceneGui: public Enableable {
public:
	SceneGui(Scene *scene);

	void mousePressed(float x, float y, int button);
	void mouseReleased(float x, float y, int button);
	void mouseDragged(float x, float y, int button);
	void mouseMoved(float x, float y);
	
	void draw();
	
private:
	ofxXmlGui gui;
	Scene *scene;
};