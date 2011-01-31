#pragma once
#include "json.h"
#include "ofMain.h"

using namespace std;

// when outputting json, you can choose whether
// you want it formatted with extra whitespace
// to make it readable, use NICE, otherwise
// use CONDENSED
#define OFXJSON_NICE 0
#define OFXJSON_CONDENSED 1

class ofxJSON: public Json::Value {
public:
	
	ofxJSON(): Json::Value() {}
	
	/** \brief Creates a json object by parsing a string of Json, or if the string starts with http, load the url (blocking) */
	ofxJSON(string jsonStr);
	ofxJSON(const char *jsonStr);
	
	/** \brief Copy constructor */
	ofxJSON(const Json::Value &other): Json::Value(other) {}
	
	/**
	 * Parses a json string
	 */
	void parse(string &jsonStr);	
	
	/** \brief Parses json from a file, returns true on success */
	bool loadFromFile(string path);	
	
	/** \brief saves json to a file, returns true on success */
	bool saveToFile(string path);
	
	/**
	 * \brief (blocking) Loads from a url, returns false if url couldn't be read/found 
	 */
	bool loadFromUrl(string url);
	
	
	// inherited from jsoncpp
	
	// test if the json node is empty
	// bool empty();
	
	// typed getters
	// string asString();
	// int asInt();
	// double asDouble();
	// bool asBool();
	
	/**
	 * Creates a json array (non-associative) from an array of strings.
	 * You can include json in each string and it will be parsed.
	 */
	ofxJSON(vector<string> &array);
		
	/** \brief Outputs a string representation of the JSON. */
	string str(int type = OFXJSON_CONDENSED);	
	
	/** \brief Prints json to stdout */
	void print();
	
	
	
	/** \brief creates a key-value pair of json values where both are strings */
	ofxJSON(string a, string b);

	// same as the above, but with char*s
	ofxJSON(const char *a, const char *b) {(*this)[a] = b;}
	/** 
	 * \brief Creates a key-value pair of json values where the key 
	 * is a string and the value is a json node.
	 */
	ofxJSON(string a, Json::Value b);
};