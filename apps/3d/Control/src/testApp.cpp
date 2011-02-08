#include "testApp.h"

#include <netdb.h>


//--------------------------------------------------------------
void testApp::setup(){
	
	/**

		poll URLs on a set increment
		download their text
		parse as json
		interpret commands and act accordingly
	 
	 */
	
	ofSetWindowTitle("Donk Control");
	lastConnectTime = 0;
	font.loadFont("uni05_54.ttf", 6);
	font.setLineHeight(8);
	
	if(!json_settings.loadFromFile("settings.json.txt")){
		string msg = "FATAL ERROR: failed to load JSON settings file: data/settings.json.txt";
		log(msg);
		cout << msg << endl;
		_exit(1);
	}
	
	polling_delay = json_settings["polling_delay_time_in_seconds"].asDouble();
	source_names = json_settings["sources"].getMemberNames();
	osc_port = json_settings["osc_remote_port"].asInt();
	for(int i=0;i<json_settings["osc_destinations"].size();i++){
		osc_destinations.push_back(json_settings["osc_destinations"][i].asString());
	}
}

//--------------------------------------------------------------
void testApp::update(){

	//is it time to call the server again?
	if(ofGetElapsedTimef() - lastConnectTime > polling_delay || lastConnectTime == 0){
		
		//do consecutive blocking calls
		try{
			for(int i=0;i<source_names.size();i++){
				if(!json.loadFromUrl(json_settings["sources"][source_names[i]].asString())){
					log("Error retreiving json from server. json.loadFromUrl() returned false.");
				}else{
					for(int j=0;j<json.size();j++){
						Json::Value bubble = json[j];
						int rndDst = ofRandom(osc_destinations.size());
						
						oscOut.setup(osc_destinations[rndDst],osc_port);
						ofxOscMessage m;
						m.setAddress("/control/bubble/new");
						m.addStringArg("id");
						m.addStringArg(bubble["id"].asString());
						m.addStringArg("timestamp");
						m.addIntArg(bubble["timestamp"].asInt());
						m.addStringArg("intent");
						m.addStringArg(bubble["intent"].asString());
						m.addStringArg("profile_image_url");
						m.addStringArg(bubble["profile_image_url"].asString());
						m.addStringArg("media_text_url");
						m.addStringArg(bubble["media_text_url"].asString());
						oscOut.sendMessage(m);
						
						log(source_names[i] + string(" > ") + bubble["id"].asString() + string(" > ") + osc_destinations[rndDst]);
					}
				}
			}
		}catch(logic_error err){
			log("Error calling remote server -- network down?");
		}
		lastConnectTime = ofGetElapsedTimef();
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	ofColor(255,255,255);
	glPushMatrix();
	font.drawString(console,0,6);
	glPopMatrix();
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

//--------------------------------------------------------------
void testApp::log(int n){
	char s[256];
	sprintf(s,"%i",n);
	log(s);
}
//--------------------------------------------------------------
void testApp::log(string s){
	console += ofGetTimestampString();
	console += " ";
	console += s + '\n';
	
	//erase old stuff
	string::iterator it = console.begin();
	int count = 0;
	for(;it!=console.end();it++){
		if(*it=='\n')count++;
	}
	int lineCount = (ofGetHeight()-6) / 8;
	int eraseCount = 0;
	while(eraseCount < count-lineCount){
		if(console[0]=='\n')eraseCount++;
		console.erase(0,1);
	}
}
