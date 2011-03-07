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
#include "ofxFourUpDisplay.h"

/**
 * \brief The gui for arranging cameras
 */
class SceneGui: public Enableable, public GuiListener {
public:
	SceneGui(Scene *scene);

	void controlChanged(GuiControl *control);
	void setEnabled(bool enabled);
	void draw();
	void save();
private:
	ofxXmlGui gui;
	Scene *scene;
	
};