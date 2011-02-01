


#include "Settings.h"
using namespace util;
Settings settings;

/**
 * Loads the file.
 */
void Settings::load(string path) {
	xmlFilePath = path;
	data.clear();
	ofxXmlSettings xml;
	if(!xml.loadFile(xmlFilePath)) {
		ofLog(OF_LOG_WARNING, "Can't load xml from %s\n", xmlFilePath.c_str());
	}
	
	xml.pushTag("properties");
	int numProps = xml.getNumTags("property");
	for(int i = 0; i < numProps; i++) {
		string key = xml.getAttribute("property", "key", "", i);
		string value = xml.getAttribute("property", "value", "", i);
		data[key] = value;
	}
	// even if the file isn't found, loaded should be true
	loaded = true;
}


/**
 * Saves the file. If you don't pass a parameter, it'll try to 
 * save it to the last place it was loaded from.
 */
bool Settings::save(string path) {
	if(path!="") xmlFilePath = path;
	if(xmlFilePath=="") {
		ofLog(OF_LOG_ERROR, "Error, no xml file specified\n");
		return false;
	}
	
	ofxXmlSettings xml;
	xml.addTag("properties");
	xml.pushTag("properties");
	
	
	map<string,string>::iterator it;
	int i = 0;
	for ( it=data.begin() ; it != data.end(); it++ ) {
		xml.addTag("property");
		xml.setAttribute("property", "key", (*it).first, i);
		xml.setAttribute("property", "value", (*it).second, i);
		i++;
	}
	xml.saveFile(xmlFilePath);
	ofLog(OF_LOG_NOTICE, "Saved to %s\n", xmlFilePath.c_str());
	return true;
}


void Settings::set(string key, string value) {
	data[key] = value;
	save();
}



string Settings::get(string key, string defaultValue) {
	if(!loaded) load(SETTINGS_FILE);
	if(data.find(key)==data.end()) {
		set(key, defaultValue);
		return defaultValue;
	}
	return data[key];
}


void Settings::set(string key, int value) {
	set(key, ofToString(value));
}

void Settings::set(string key, float value) {
	set(key, ofToString(value));
}

int Settings::getInt(string key, int defaultValue) {
	return ofToInt(get(key, ofToString(defaultValue)));
}

float Settings::getFloat(string key, float defaultValue) {
	return ofToFloat(get(key, ofToString(defaultValue)));
}

bool Settings::getBool(string key, bool defaultValue) {
	string val = get(key, defaultValue?"true":"false");
	return val=="true" || val=="yes" || val=="1" || val=="TRUE" || val=="YES";
}



