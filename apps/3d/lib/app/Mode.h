/**
 * Mode.h
 * Render
 *
 * This class acts as the current state of the app, that's why it's a singleton.
 * You can change modes by calling setMode(<mode name>)
 * Then you can request parameter values with getValue(). This class takes care
 * of interpolating between sets of parameters.
 * 
 * Created by Marek Bereza on 21/02/2011.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxXmlGui.h"
#include "Tween.h"

namespace Donk {
	
	
	class Mode: public GuiListener {
	public:

		/** Singleton */
		static Mode *getInstance();
		
		/** Give this method the name of the new mode and it will start tweening to it */
		void setMode(string modeName);
		string getMode();
        
		float getValue(const string &name);
		
		
		
		/** for debugging */
		void print(); 
		
		ofxXmlGui *getGui();
		void controlChanged(GuiControl *control);
		
	private:
		
		Mode();
		
		string currModeName;
		
		// this is a map that stores all the parameters
		// e.g. presets[modeName][parameterName] = value of the parameter for the mode.
		map<string,map<string,float> > parameters;
		
		vector<string> parameterNames;
		vector<string> modeNames;
		
		// this stores what's in parameters.xml - essentially a schema of the mode
		// (the name of each parameter and its default value)
		map<string,float> parameterDefaults;
		
		// these are the minimum and maximum values for 
		// parameters (for the gui)
		map<string,pair<float,float> > parameterMinMax;
		void saveModeValues(string modeName);
		ofxXmlGui *gui;
		
		map<string,Tween*> tweens;
		
		// this is for the gui, not an actual representation of which mode we're in.
		int selectedModeId;
	};
	
	

	
};