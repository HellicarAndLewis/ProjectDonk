/*
 *  SceneGui.h
 *  emptyExample
 *
 *  Created by Marek Bereza on 06/01/2011.
 *
 */
#include "ofxXmlGui.h"
#include "Scene.h"
#include "Enableable.h"

/**
 * \brief The gui for arranging cameras
 */
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