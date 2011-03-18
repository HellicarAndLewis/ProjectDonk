/**
 * Mode.cpp
 * Render
 *
 * Created by Marek Bereza on 21/02/2011.
 *
 */

#include "Mode.h"
#include "ofxXmlSettings.h"
#include "constants.h"
#include "testApp.h"

using namespace Donk;

Mode::Mode() {
	
	ofLogNotice() << "Mode::Mode()";
	selectedModeId = 0;
	currModeName = "";
	gui = NULL;
	// load the parameter schema from parameters.xml
	ofxXmlSettings xml;
	xml.loadFile("settings/parameters.xml");
	xml.pushTag("parameters");
	int numTags = xml.getNumTags("parameter");
	for(int i = 0; i < numTags; i++) {
		string parameterName = xml.getAttribute("parameter", "name", "", i);
		float parameterValue = xml.getAttribute("parameter", "defaultValue", 0.0, i);
		parameterNames.push_back(parameterName);
		parameterDefaults[parameterName] = parameterValue;
		
		parameterMinMax[parameterName] = make_pair(
			xml.getAttribute("parameter", "minValue", 0.0, i),
			xml.getAttribute("parameter", "maxValue", 1.0, i)
		);
	}
	
	// now load all the mode names in
	numTags = xml.getNumTags("mode");
	for(int i = 0; i < numTags; i++) {
		string modeName = xml.getAttribute("mode", "name", "", i);
		modeNames.push_back(modeName);
		// this is copy constructor, no?
		parameters[modeName] = parameterDefaults;
	}

	// now load mode specific values
	map<string,map<string,float> >::iterator it;
	map<string,float>::iterator it2;
	for(it = parameters.begin(); it != parameters.end(); it++) {
		string mode = (*it).first;
		xml.loadFile("settings/"+mode+".xml");
		xml.pushTag("values");
		numTags = xml.getNumTags("parameter");
		for(int i = 0; i < numTags; i++) {
			string key = xml.getAttribute("parameter", "key", "", i);
			float value = xml.getAttribute("parameter", "value", 0.0, i);
			parameters[mode][key] = value;
		}
		
		// call save here to generate an xml file if one didn't already exist
		//saveModeValues(mode);
		
	}
	
	// create all the tween objects
	for(int i = 0; i < parameterNames.size(); i++) {
		tweens[parameterNames[i]] = new Tween();
		tweens[parameterNames[i]]->setType(EASE_CUBIC, EASE_OUT);
		tweens[parameterNames[i]]->setValue(parameterDefaults[parameterNames[i]]);
	}
	
	
	
	
	print();
}

void Mode::saveModeValues(string modeName) {
	ofxXmlSettings xml;
	xml.addTag("values");
	xml.pushTag("values");
	for(int i = 0; i < parameterNames.size(); i++) {
		string key = parameterNames[i];
		float value = parameters[modeName][key];
		xml.addTag("parameter");
		xml.addAttribute("parameter", "key", key, i);
		xml.addAttribute("parameter", "value", value, i);
	}
	xml.saveFile("settings/"+modeName+".xml");

}

void Mode::print() {
	map<string,map<string,float> >::iterator it1;
	map<string,float>::iterator it2;
	
	for(it1 = parameters.begin(); it1 != parameters.end(); it1++) {
		string mode = (*it1).first;
		ofLogNotice() << "MODE " << mode << " {";
		
		for(it2 = parameters[mode].begin(); it2 != parameters[mode].end(); it2++) {
			ofLogNotice() << "\t" << (*it2).first << " = " << (*it2).second;
		}
		
		ofLogNotice() << "}";
	}
}

Mode *Mode::getInstance() {
	static Mode *instance = NULL;
	if(instance==NULL) {
		instance = new Mode();
	}
	return instance;
}

string Mode::getMode()
{
    return currModeName;
}

void Mode::setMode(string modeName) {
	if(currModeName==modeName) {
		//ofLogNotice() << "Mode: Not changing mode because we're already in '" << modeName<< "'";
		return;
	}
	
	currModeName = modeName;
	ofLogNotice() << "Mode: Changing mode to '" << currModeName << "'";

	// make gui reflect the same thing (if it wasn't changed by the gui)
	for(int i = 0; i < modeNames.size(); i++) {
		if(modeNames[i]==modeName) {
			selectedModeId = i;
			break;
		}
	}
	// repoint the sliders to the new mode
	for(int i = 0; i < parameterNames.size(); i++) {
		float *p =  &(parameters[modeName][parameterNames[i]]);
		gui->pointToValue(parameterNames[i], p);
	}

	// start the tweens
	for(int i = 0; i < parameterNames.size(); i++) {
		
		tweens[parameterNames[i]]->tween(parameters[modeName][parameterNames[i]], FADE_DURATION);
	}
	
	// not sure if this is the best place for this
	// I could not figure out where to put it. HAHA
	testApp::instance->projection->interactionModeChange(modeName);
	
}


float Mode::getValue(const string& key) {
	
	// for now this just switches, but we'll make it lerp soon
	//return parameters[currModeName][key];
	if(tweens.find(key)!=tweens.end()) {
		return tweens[key]->getValue();
	} else {
		return 0;
	}
}



ofxXmlGui *Mode::getGui() {
	if(gui==NULL) {
		
		gui = new ofxXmlGui();
		gui->x = 200;
		gui->setup(10, 60, 200);
		map<string,float>::iterator it;
		if(currModeName=="")  {
			currModeName = (*parameters.begin()).first;
			ofLogNotice() << "Choosing default mode " << currModeName;
		}
		GuiList *list = gui->addList("Mode", selectedModeId, modeNames);
		list->height = 110;
		for(it = parameters[currModeName].begin(); it != parameters[currModeName].end(); it++) {
			string key = (*it).first;
			gui->addSlider(key, (*it).second, parameterMinMax[key].first, parameterMinMax[key].second);
		}
		gui->addListener(this);
		currModeName = "34234df"; // force the mode to update
		setMode(currModeName);
	}
	return gui;
}

void Mode::controlChanged(GuiControl *control) {

	if(control->controlId=="Mode") {
		setMode(modeNames[selectedModeId]);
	} else if(tweens.find(control->controlId)!=tweens.end()) {
		tweens[control->controlId]->setValue(fval(control->value));
		saveModeValues(currModeName);
	}
}
