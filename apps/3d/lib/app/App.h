#pragma once
#include "ofMain.h"
#include "Scene.h"
#include "SceneGui.h"
#include "ofxFourUpDisplay.h"
#include "ofxProjectorBlend.h"

namespace Donk { 


/**
 * \brief Basic app that you inherit from for your app
 */
class App: public ofBaseApp {
public:

	/**
	 * Loads the model, settings, sets up the GUI etc.
	 */
	App();
	
	
	/**
	 * You override this to draw your scene - it will be called
	 * once for each projector for each scene.
	 */
	virtual void drawView() {};
	
	/**
	 * Any rendering to FBO should be done here (called once per frame before (drawView())
	 */
	virtual void render() {};
	
	
	// callbacks for gui
	void _update(ofEventArgs &e);
	void _draw(ofEventArgs &e);
	void _keyPressed(ofKeyEventArgs &e);
	void _keyReleased(ofKeyEventArgs &e);
protected:

	/** 
	 * This is the scene including the 
	 * cameras and projectors that you can 
	 * query and draw to etc.
	 */
	Scene *scene;
	
	// this is an extra gui (on key 3) for 
	// any interaction stuff you want to add.
	ofxXmlGui *getInteractionGui();
	
private:
	
	bool guiEnabled;
	
	SceneGui *sceneGui;
	ofxXmlGui *modeGui;
	ofxXmlGui *interactionGui;
	
	void drawAllProjectors();
	
	ofxProjectorBlend projectorBlend;
	bool usingProjectorBlend;
};
}
/**
 * \mainpage 
 * This is a framework for creating installations involving projectors and kinects.
 */ 