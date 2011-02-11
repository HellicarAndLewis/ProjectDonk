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
		cerr << msg << endl;
		_exit(1);
	}
	
	polling_delay = json_settings["polling_delay_time_in_seconds"].asDouble();
	source_names = json_settings["sources"].getMemberNames();
	rendermachine_osc_port = json_settings["rendermachine"]["osc_port"].asInt();
	rendermachine_ip = json_settings["rendermachine"]["ip"].asString();
	http_get_timeout = json_settings["http_get_timeout"].asInt();
	http_auth = json_settings["sources_auth"].asString();

	controlbar.children.push_back(new ControlBar::Label("Mode:                  "));
	for(int i=0;i<source_names.size();i++){
		loaders.push_back(new AsyncHttpLoader());
		loaders.back()->timeout = http_get_timeout;
		controlbar.children.push_back(new ControlBar::Button(source_names[i]));
	}
	controlbar.doLayout();
	
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
					
					for(int j=0;j<json["totalItems"].asInt();j++){
						Json::Value bubble = json["items"][j];
						
						oscOut.setup(rendermachine_ip,rendermachine_osc_port);
						ofxOscMessage m;
						
						m.setAddress("/control/bubble/new");
						m.addStringArg("queueID");
						m.addStringArg(bubble["queueID"].asString());
						m.addStringArg("profileImageURL");
						m.addStringArg(bubble["profileImageURL"].asString());
						m.addStringArg("userName");
						m.addStringArg(bubble["userName"].asString());
						m.addStringArg("text");
						m.addStringArg(bubble["text"].asString());
						
						if(bubble["hasMedia"].asString()=="1"){
							for(int k=0;k<bubble["media"].size();k++){
								m.addStringArg("mediaID");
								m.addStringArg(bubble["media"][k]["mediaID"].asString());
								m.addStringArg("mediaThumbURL");
								m.addStringArg(bubble["media"][k]["mediaThumbURL"].asString());
								m.addStringArg("mediaURL");
								m.addStringArg(bubble["media"][k]["mediaURL"].asString());
							}
						}
						
						oscOut.sendMessage(m);				
						log(source_names[i] + string(" > ") +
							bubble["queueID"].asString() + string(" > ") +
							bubble["userName"].asString(),0);
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
				loaders[i]->get(json_settings["sources"][source_names[i]].asString(),http_auth);
			}
		}catch(logic_error err){
			log("Error calling remote server -- network down?",1);
		}
		
		lastConnectTime = ofGetElapsedTimef();
	}

	
	//resizing the console to fit the window
	int lineCount = (ofGetHeight()-6-controlbar.rect.height) / 8;
	while(console.size() > lineCount){
		console.pop_front();
		console_colors.pop_front();
	}

	controlbar.update(mouseX,mouseY);
	while(ControlBar::Control::eventQueue.size()>0){
		ControlBar::Event e = ControlBar::Control::eventQueue.front();
		ControlBar::Control::eventQueue.pop_front();
		if(e.what=="mouseDown" && e.obj->className=="Button"){
				ControlBar::Button *b = (ControlBar::Button*)e.obj;
				cout << b->text << endl;
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	controlbar.draw();
	glPushMatrix();
	glTranslatef(0,controlbar.rect.height,0);
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
	controlbar.mouseDown();
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
	
}
