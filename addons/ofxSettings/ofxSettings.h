#pragma once
#include "ofMain.h"
#include "ofxXmlSettings.h"
/**
 *  \brief Provides a persistent settings interface for stuff
 *
 *  It's basically a hashmap that saves itself every time you set a value.
 *  Also, if you ever try to get a value, you can optionally provide a default so
 *  it creates a default settings file if one didn't exist in the first place.
 * 
 *  It's also accessible everywhere, just include Settings.h and then 
 *  do something like
 * 
 *  int width = settings.getInt("width", 1024);
 * 
 *  Which will load the xml if not done already, then return the width value.
 *  If width hasn't been set, or there is no xml file, one will be created, 
 *  using the default value set in the second parameter.
 *
 *  The file format is xml, so easy on the eye when editing.
 */
class ofxSettings {
public:
	
	ofxSettings();
	
	// set a value
	void set(string key, string value);	
	void set(string key, int value);	
	void set(string key, float value);	
	void set(string key, bool value);
	
	
	/** get a string value (with optional default value) */
	string get(string key, string defaultValue = "");	
	
	/** get an int */
	int getInt(string key, int defaultValue = 0);	
	
	/** get a float */
	float getFloat(string key, float defaultValue = 0);
	
	/** get a bool */
	bool getBool(string key, bool defaultValue = false);
	

	/**
	 * Loads the file.
	 */
	void load(string path);	
	
private:
	string xmlFilePath;
	// map of data
	map<string,string> data;
	
	// have we tried to load yet?
	bool loaded;
	
	/**
	 * Saves the file. If you don't pass a parameter, it'll try to 
	 * save it to the last place it was loaded from.
	 */
	bool save(string path = "");	
	
};

	


extern ofxSettings settings;