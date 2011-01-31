#include "testApp.h"
#include "ofxJSON.h"

ofxJSON json;

//--------------------------------------------------------------
void testApp::setup(){
	
	
	
	
	////////////////////////////////////////////////////////////////
	// READING JSON

	// jsoncpp only works with double quotes!!
	string jsonStr = "[\"This\", \"is\", \"a\", \"json\", \"test\"]";
	
	// parse the json
	json = ofxJSON(jsonStr);
	
	// print it to stdout
	json.print();
	
	// loop through it
	for(int i = 0; i < json.size(); i++) {
		cout << json[i].asString() << endl;
	}
	
	// try an associative array
	jsonStr = "{\"This\": \"is\", \"an\": \"associative\", \"array\": {\"Nested\":[\"Hello\", \"One\"]}}";
	
	/*
	 {
		"This": "is",
		"an": "associative",
		"array": {
			"Nested": ["Hello", "one"]
		}
	 
	}
	 
	 */
	// parse a new string
	json.parse(jsonStr);
	

	// associative array access
	cout << json["This"].asString() << endl;
	
	// nested array access
	for(int i = 0; i < json["array"]["Nested"].size(); i++) {
		cout << json["array"]["Nested"][i].asString() << endl;
	}
	
	////////////////////////////////////////////////////////////////
	// READING JSON

	cout << "----------------------------" << endl;
	
	// create an associative array
	json = ofxJSON();
	json["apples"] = 10;
	json["oranges"] = 14;
	
	json.print();
	
	// indexed array
	json = ofxJSON();
	for(int i = 0; i < 10; i++) {
		json[i] = ofRandom(0, 1);
	}
	// add a nested element
	json[10] = ofxJSON("Associative", "Array");
	json[11] = ofxJSON("Another", ofxJSON("associative", "array"));
	json[12] = ofxJSON("[\"embedded\", \"json\", \"code\", \"that\", \"gets\", \"parsed\"]");
	json.print();

	
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

