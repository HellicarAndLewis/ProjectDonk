#pragma once
#include "ofMain.h"
#include "Scene.h"
#include "SceneGui.h"
#include "ofxFourUpDisplay.h"

namespace Donk { 


/**
 * \brief Basic app that you inherit from for your app
 */
class App: public ofBaseApp, public GuiListener {
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
	
	void controlChanged(GuiControl *control);
	
	bool guiEnabled;

protected:

	/** 
	 * This is the scene including the 
	 * cameras and projectors that you can 
	 * query and draw to etc.
	 */
	Scene *scene;
	
	// this is an extra gui (on key 3) for 
	// any interaction stuff you want to add.
	ofxXmlGui *getCalibrationGui();
	
private:
	
	ofxFourUpDisplay *viewports;
	
	// this is a little tab at the top of the gui
	// to allow tabbing between the different guis
	ofxXmlGui guiChooser;
	
	// this is a configurable gui, that you can add to if you wish
	ofxXmlGui *appGui;
	
	// this is for manipulating the projector position.
	SceneGui *sceneGui;

	// This is for choosing the current mode and setting its parameters
	ofxXmlGui *modeGui;
	
	// everything to do with image and interaction calibration goes here.
	ofxXmlGui *calibrationGui;
	
	// this is the current projector
	// that is being modified by the keys.
	// use the left and right arrows to select.
	int currProjectorId;
	
	void drawAllProjectors();
	
	// whether we're editing the mask.
	bool maskMode;
	
	// everything gets drawn to this intermediately
	// and then it can be post-processed with a shader
	ofFbo screenFbo;
};
}
/**
 * \mainpage 
 * This is a framework for creating installations involving projectors and kinects.
 */ 