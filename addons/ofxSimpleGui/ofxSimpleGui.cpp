
#include "ofxSimpleGui.h"

//------------------------------------------------------------------------------ constrcutor
ofxSimpleGui::ofxSimpleGui() {
	
	guiFocus  = 0;
	guiCount  = 0;
	
	mx		  = 0;
	my		  = 0;
	key		  = 0;
	mousedown = false;
	mouseup	  = true;
	xmlFile	  = "NULL";
	verbose   = true;
	doRender  = true;
	
	setListeners();
	
	doSave	   = false;
	saveButton = new ofxToggle("Save XML", &doSave); 
	
}

//------------------------------------------------------------------------------ set listeners
void ofxSimpleGui::setListeners() {
	ofMouseEvents.addListener(this);
	ofKeyEvents.addListener(this);
	ofAppEvents.addListener(this);
}

//------------------------------------------------------------------------------ toggle render
void ofxSimpleGui::toggleRender() {
	doRender = !doRender;
}
//------------------------------------------------------------------------------ set from xml
void ofxSimpleGui::setFromXML(string file) {
	
	xmlFile = file;
	
	if(XML.loadFile(file)) {
		
		// --- Toggles ---
		int numTogglesTags = XML.getNumTags("toggles");
		if(numTogglesTags > 0) {
			XML.pushTag("toggles", numTogglesTags-1);
			int numElements  = XML.getNumTags("element");
			for(int i=0; i<numElements; i++) {
				
				int node_id		 =  XML.getValue("element:id", 0, i);
				string node_name =  XML.getValue("element:name", "Name", i);
				
				for(int j=0; j<toggles.size(); j++) {
					if(toggles[j]->xmlID == node_id) {
						toggles[j]->set(XML.getValue("element:value", 0, i));
					}
				}
			}
		}
		XML.popTag();
		
		// --- Float Sliders
		int numSliderFTags = XML.getNumTags("slidersF");
		if(numSliderFTags > 0) {
			XML.pushTag("slidersF", numSliderFTags-1);
			int numElements  = XML.getNumTags("element");
			for(int i=0; i<numElements; i++) {
				
				int node_id		 =  XML.getValue("element:id", 0, i);
				string node_name =  XML.getValue("element:name", "Name", i);
				
				for(int j=0; j<slidersF.size(); j++) {
					if(slidersF[j]->xmlID == node_id) {
						slidersF[j]->set(XML.getValue("element:value", 0.0, i));
					}
				}
			}
		}
		XML.popTag();
		
		// --- Int Sliders
		int numSliderITags = XML.getNumTags("slidersI");
		if(numSliderITags > 0) {
			XML.pushTag("slidersI", numSliderITags-1);
			int numElements  = XML.getNumTags("element");
			for(int i=0; i<numElements; i++) {
				
				int node_id		 =  XML.getValue("element:id", 0, i);
				string node_name =  XML.getValue("element:name", "Name", i);
				
				for(int j=0; j<slidersI.size(); j++) {
					if(slidersI[j]->xmlID == node_id) {
						slidersI[j]->set(XML.getValue("element:value", 0.0, i));
					}
				}
			}
		}
		XML.popTag();
		if(verbose) cout << " --- GUI set From XML ---\n";
	}
	else cout << " --- ERROR IN XML ---\n";
}

//------------------------------------------------------------------------------ save to xml
void ofxSimpleGui::saveToXML(string file) {
	
	// --- Toggles ---
	int numTogglesTags = XML.getNumTags("toggles");
	if(numTogglesTags > 0) {
		XML.pushTag("toggles", numTogglesTags-1);
		int numElements  = XML.getNumTags("element");
		for(int i=0; i<numElements; i++) {
			
			int node_id		 =  XML.getValue("element:id", 0, i);
			string node_name =  XML.getValue("element:name", "Name", i);
			
			for(int j=0; j<toggles.size(); j++) {
				if(toggles[j]->xmlID == node_id) {
					XML.setValue("element:value", (*toggles[j]->value), i);
				}
			}
		}
	}
	XML.popTag();
	
	
	// --- Float Sliders
	int numSliderFTags = XML.getNumTags("slidersF");
	if(numSliderFTags > 0) {
		XML.pushTag("slidersF", numSliderFTags-1);
		int numElements  = XML.getNumTags("element");
		for(int i=0; i<numElements; i++) {
			
			int node_id		 =  XML.getValue("element:id", 0, i);
			string node_name =  XML.getValue("element:name", "Name", i);
			
			for(int j=0; j<slidersF.size(); j++) {
				if(slidersF[j]->xmlID == node_id) {
					XML.setValue("element:value", (*slidersF[j]->value), i);
				}
			}
		}
	}
	XML.popTag();
	
	
	// --- Int Sliders
	int numSliderITags = XML.getNumTags("slidersI");
	if(numSliderITags > 0) {
		XML.pushTag("slidersI", numSliderITags-1);
		int numElements  = XML.getNumTags("element");
		for(int i=0; i<numElements; i++) {
			
			int node_id		 =  XML.getValue("element:id", 0, i);
			string node_name =  XML.getValue("element:name", "Name", i);
			
			for(int j=0; j<slidersI.size(); j++) {
				if(slidersI[j]->xmlID == node_id) {
					XML.setValue("element:value", (*slidersI[j]->value), i);
				}
			}
		}
	}
	XML.popTag();
	
	XML.saveFile(file);
	cout << " --- Settings Saved ---\n";

}

//------------------------------------------------------------------------------ set gui title
void ofxSimpleGui::setTitle(string name) {
	guiTitle.set(name);
}

//------------------------------------------------------------------------------ set verbose
void ofxSimpleGui::setVerbose(bool v) {
	verbose = v;
}

//------------------------------------------------------------------------------ update
void ofxSimpleGui::update() {
	
	//save to xml	
	if(doSave) {
		if(xmlFile.compare("NULL") != 0) {
			saveToXML(xmlFile);
			doSave = false;
		}
	}
}

//------------------------------------------------------------------------------ get value 		
int ofxSimpleGui::getValueI(string nameID) {
	
	//SLIDER INTS
	for(int i=0; i<slidersI.size(); i++) {
		if(nameID.compare(slidersI[i]->name) == 0) return slidersI[i]->getValue();
		else return NULL;
	}
}

float ofxSimpleGui::getValueF(string nameID) {
	for(int i=0; i<slidersF.size(); i++) {
		if(nameID.compare(slidersF[i]->name) == 0) return slidersF[i]->getValue();
		else return NULL;
	}
}

bool ofxSimpleGui::getValueB(string nameID) {
	for(int i=0; i<toggles.size(); i++) {
		if(nameID.compare(toggles[i]->name) == 0) return toggles[i]->getValue();
		else return NULL;
	}
}

//------------------------------------------------------------------------------ render focus
void ofxSimpleGui::renderFocus(float x, float y) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	ofFill();
	ofSetColor(255, 90, 25, 200);
	ofRect(0, 0, 10, 10);
	glPopMatrix();
}
//------------------------------------------------------------------------------ draw		
void ofxSimpleGui::draw() {
	
	if(!doRender) return;
	
	glPushMatrix();
	glTranslatef(0, 0, 0);
	
	xspace = 10;
	yspace = 10;
	int sp = 50;
	
	//Gui Title
	guiTitle.render(xspace, yspace);
	yspace += sp;
	
	//INT
	for(int i=0; i<slidersI.size(); i++) {
		slidersI[i]->render(xspace, yspace);
		yspace += sp;
		if(guiFocus == slidersI[i]->guiID) slidersI[i]->focused = true;
		else							   slidersI[i]->focused = false;	
	}
	
	//FLOAT
	for(int i=0; i<slidersF.size(); i++) {
		slidersF[i]->render(xspace, yspace);
		yspace += sp;
		if(guiFocus == slidersF[i]->guiID) slidersF[i]->focused = true;
		else							   slidersF[i]->focused = false;	
	}
	
	//MOVIE
	for(int i=0; i<slidersMovie.size(); i++) {
		slidersMovie[i]->render(xspace, yspace);
		yspace += 50+sp;
	}
	
	//TOGGLE
	for(int i=0; i<toggles.size(); i++) {
		toggles[i]->render(xspace, yspace);
		yspace += 25;
		if(guiFocus == toggles[i]->guiID) toggles[i]->focused = true;
		else							  toggles[i]->focused = false;
	}
	
	//Save XML Button
	saveButton->render(xspace, yspace);
	
	//VIDEO WARPER
	for(int i=0; i<quadWarpers.size(); i++) {
		quadWarpers[i]->render();
	}
	
	glPopMatrix();
}

//------------------------------------------------------------------------ adding ui elements		
void ofxSimpleGui::addSlider(string _name, int *_value, int _min, int _max, int xmlID) {
	slidersI.push_back(new ofxSliderInt(_name, _value, _min, _max, xmlID, guiCount));
	guiCount++;
}

void ofxSimpleGui::addSlider(string _name, float *_value, float _min, float _max, int xmlID) {
	slidersF.push_back(new ofxSliderFloat(_name, _value, _min, _max, xmlID, guiCount));
	guiCount++;
}

void ofxSimpleGui::addMovieSlider(string _name, ofVideoPlayer *_input, float *_value, float _min, float _max, int xmlID) {
	slidersMovie.push_back(new ofxSliderMovie(_name, _input, _value, _min, _max));
	//guiCount++;
}	
void ofxSimpleGui::addQuadWarper(float x, float y, float sw, float sh, ofPoint **pts) {
	quadWarpers.push_back(new ofxQuadWarp(x, y, sw, sh, pts));
	//guiCount++;
}
void ofxSimpleGui::addToggle(string _name, bool *_value, int _xmlID) {
	toggles.push_back(new ofxToggle(_name, _value, _xmlID, guiCount));
	guiCount++;
}	
void ofxSimpleGui::addTitle(string _name) {
	titles.push_back(new ofxGuiTitle(_name));
}


void ofxSimpleGui::mouseMoved(int x, int y) {
	mx = x; my = y;
}
//------------------------------------------------------------------------ key press
void ofxSimpleGui::keyPressed(int key) {
	if(key == OF_KEY_DOWN) {
		if(guiFocus <= guiCount) guiFocus ++;
		else guiFocus = 0; 
	}
	if(key == OF_KEY_UP) {
		if(guiFocus <= guiCount) guiFocus --;
		else guiFocus = guiCount;
	}
	if(key == OF_KEY_RIGHT) {
		//INT
		for(int i=0; i<slidersI.size(); i++) {
			if(guiFocus == slidersI[i]->guiID) slidersI[i]->add();   			
		}
		//FLOAT
		for(int i=0; i<slidersF.size(); i++) {
			if(guiFocus == slidersF[i]->guiID) slidersF[i]->add();   			
		}
	}
	if(key == OF_KEY_LEFT) {
		//INT
		for(int i=0; i<slidersI.size(); i++) {
			if(guiFocus == slidersI[i]->guiID) slidersI[i]->sub();   			
		}
		//FLOAT
		for(int i=0; i<slidersF.size(); i++) {
			if(guiFocus == slidersF[i]->guiID) slidersF[i]->sub();   			
		}
	}
	if(key == OF_KEY_RETURN) {
		//TOGGLE
		for(int i=0; i<toggles.size(); i++) {
			if(guiFocus == toggles[i]->guiID) toggles[i]->toggle();   			
		}
	}
	
}
