/*
 *  ofxJSON.cpp
 *  ofxJSONExample
 *
 *  Created by Marek Bereza on 31/01/2011.
 *  Copyright 2011 Marek Bereza. All rights reserved.
 *
 */

#include "ofxJSON.h"

ofxJSON::ofxJSON(string jsonStr) {
	if(jsonStr.find("http")==0) {
		loadFromUrl(jsonStr);
	} else {
		parse(jsonStr);
	}
}
ofxJSON::ofxJSON(const char *jsonStr) {
	string s = jsonStr;
	if(s.find("http")==0) {
		loadFromUrl(s);
	} else {
		parse(s);
	}

}

bool ofxJSON::loadFromUrl(string url) {

	// get url
	ofHttpResponse res = ofLoadURL(url);

	if(res.status<400) {
		if(res.status>=300) {
			ofLog(OF_LOG_WARNING, "Warning: got response code %d from server for url %s\n", res.status, url.c_str());
		}
		string json = res.data.getText();
		parse(json);
	} else {
		ofLog(OF_LOG_ERROR, "Got bad (%d) http response for '%s'\n-----%s\n-----\n", res.status, url.c_str(), res.data.getText().c_str());
		return false;
	}
}

ofxJSON::ofxJSON(string a, string b): Json::Value() {
	(*this)[a.c_str()] = b.c_str();
	
}

ofxJSON::ofxJSON(string a, Json::Value b): Json::Value() {
	(*this)[a.c_str()] = b;
}

void ofxJSON::parse(string &jsonStr) {
	clear();
	Json::Reader reader;
	reader.parse(jsonStr, *this);
}


ofxJSON::ofxJSON(vector<string> &array) {
	ostringstream toParse;
	toParse << "[";
	for(int i = 0; i < array.size(); i++) {
		toParse << "\""  <<array[i] << "\"";
		if(i<array.size()-1) toParse << ",";
	}
	toParse << "]";
	string output = toParse.str();
	parse(output);
	
}

string ofxJSON::str(int type) {
	if(type==OFXJSON_CONDENSED) {
		Json::FastWriter writer;
		return writer.write(*this);
	} else {
		Json::StyledWriter writer;
		return writer.write(*this);
	}
}

void ofxJSON::print() {
	printf("%s\n", str(OFXJSON_NICE).c_str());
}

bool ofxJSON::loadFromFile(string path) {
	path = ofToDataPath(path, false);
	ifstream ifs(path.c_str());
	if(!ifs || !ifs.is_open()) {
		cout << "ofxJSON::loadFromFile(): Can't open file: " << path << endl;
		return false;
	}
	
	std::string json((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	clear();
	parse(json);
	return true;
	
}

bool ofxJSON::saveToFile(string path) {
	
	
	path = ofToDataPath(path, false);
	
	
	ofstream ofs(path.c_str(), ios::out);
	if(!ofs) {
		cout << "ofxJSON::saveToFile(): Can't open file: " << path << endl;
		return false;
	}
	ofs << str(OFXJSON_NICE);
	ofs.close();
	
	return true;
}
