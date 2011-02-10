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
		log(msg,1);
		cout << msg << endl;
		_exit(1);
	}
	
	polling_delay = json_settings["polling_delay_time_in_seconds"].asDouble();
	source_names = json_settings["sources"].getMemberNames();
	rendermachine_osc_port = json_settings["rendermachine"]["osc_port"].asInt();
	rendermachine_ip = json_settings["rendermachine"]["ip"].asString();
	http_get_timeout = json_settings["http_get_timeout"].asInt();
	
	for(int i=0;i<source_names.size();i++){
		loaders.push_back(new AsyncHttpLoader());
		loaders.back()->timeout = http_get_timeout;
	}
}

//--------------------------------------------------------------
void testApp::update(){

	for(int i=0;i<loaders.size();i++){
		if(loaders[i]->done){
			if(!loaders[i]->errorString.empty()){
				log(loaders[i]->errorString,1);
			}else{
				int status = loaders[i]->response.getStatus();
				if(status!=200){
					char statString[256];
					sprintf(statString,"Server returned error %i",status);
					log(statString,1);
				}else{
					json.parse(loaders[i]->data);
					
					for(int j=0;j<json.size();j++){
						Json::Value bubble = json[j];
						oscOut.setup(rendermachine_ip,rendermachine_osc_port);
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
						log(source_names[i] + string(" > ") + bubble["id"].asString(),0);
					}
				}
			}
			
			loaders[i]->reset();
		}
	}
	
	//is it time to call the server again?
	if(ofGetElapsedTimef() - lastConnectTime > polling_delay || lastConnectTime == 0){
		
		//do consecutive blocking calls
		try{
			for(int i=0;i<source_names.size();i++){
				loaders[i]->get(json_settings["sources"][source_names[i]].asString());
			}
		}catch(logic_error err){
			log("Error calling remote server -- network down?",1);
		}
		
		lastConnectTime = ofGetElapsedTimef();
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	ofColor(255,255,255);
	glPushMatrix();
	for(int i=0;i<console.size();i++){
		if(console_colors[i]==0){
			ofSetColor(128,255,128);
		}else{
			ofSetColor(255,128,128);
		}
		font.drawString(console[i],0,6);
		glTranslatef(0,8,0);
	}
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
void testApp::log(int n,int color=0){
	char s[256];
	sprintf(s,"%i",n);
	log(s,color);
}
//--------------------------------------------------------------
void testApp::log(string s,int color=0){
	string displayLine = ofGetTimestampString();
	displayLine += " ";
	displayLine += s + '\n';
	
	console.push_back(displayLine);
	console_colors.push_back(color);
	
	//erase old stuff
	int lineCount = (ofGetHeight()-6) / 8;
	while(console.size() > lineCount){
		console.pop_front();
		console_colors.pop_front();
	}
}
