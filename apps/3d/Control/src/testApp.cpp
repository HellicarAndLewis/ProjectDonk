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

	
	controlbar.children.push_back(new ControlBar::Button("Poll"));
	controlbar.children.back()->disable();
	controlbar.children.push_back(new ControlBar::Label("Mode:             "));
	controlbar.children.push_back(new ControlBar::Button("Swap"));
	controlbar.children.push_back(new ControlBar::Button("Poll"));
	controlbar.children.back()->disable();
	controlbar.children.push_back(new ControlBar::Label("Next:             "));
	for(int i=0;i<source_names.size();i++){
		loaders.push_back(new AsyncHttpLoader());
		loaders.back()->timeout = http_get_timeout;
		controlbar.children.push_back(new ControlBar::Button(source_names[i]));
		controlbar.children.back()->userData = (void*)i;
	}
	controlbar.doLayout();
	modes[0] = -1;
	modes[1] = -1;
	
	oscOut.setup(rendermachine_ip,rendermachine_osc_port);

}

//--------------------------------------------------------------
void testApp::update(){

	for(int i=0;i<loaders.size();i++){
		if(i!=modes[0] && i!=modes[1])continue;
		
		if(loaders[i]->status==2){
			if(!loaders[i]->errorString.empty()){
				log(loaders[i]->errorString,1);
			}else{
				int status = loaders[i]->response.getStatus();
				if(status!=200){
					char statString[256];
					sprintf(statString,"Server returned error %i",status);
					log(statString,1);
				}else{
					
					cout << source_names[i] << "=" << "\"\"\"" << loaders[i]->data << "\"\"\"" << endl;
					
					json = ofxJSON();
					json.parse(loaders[i]->data);
					
					if(!json.isObject()){ // accounting for incomplete data error
						log(source_names[i] + " error: json parsing failed. bubbles may have been lost.",1);
					}else if(source_names[i]=="vote"){ //parse VOTE format
						
						Json::Value question = json;
						ofxOscMessage m;
						m.setAddress("/control/question/update");
						m.addStringArg("questionID");
						m.addStringArg(question["questionID"].asString());
						m.addStringArg("text");
						m.addStringArg(question["text"].asString());
						m.addStringArg("tag1");
						m.addStringArg(question["tag1"].asString());
						m.addStringArg("tag2");
						m.addStringArg(question["tag2"].asString());
						m.addStringArg("count_tag1");
						m.addStringArg(question["count_tag1"].asString());
						m.addStringArg("count_tag2");
						m.addStringArg(question["count_tag2"].asString());
						oscOut.sendMessage(m);
						
						log(string("question update \"") +
							question["text"].asString() +
							string("\" ") +
							question["tag1"].asString() + string("=") + question["count_tag1"].asString() + string(",") +
							question["tag2"].asString() + string("=") + question["count_tag2"].asString()
							,0);
						
						//parse through each bubble set and send the bubbles
						for(int k=0;k<2;k++){
							Json::Value items;
							Json::Value tag;
							
							
							if(k==0){
								items = question["tag1_items"];
								tag = question["tag1"];
							}else{
								items = question["tag2_items"];
								tag = question["tag2"];
							}
							
							for(int ii=0;ii<items.size();ii++){
								Json::Value bubble = items[ii];

								ofxOscMessage m;
								m.setAddress("/control/bubble/new");
								m.addStringArg("mode");
								m.addStringArg("vote");
								m.addStringArg("questionID");
								m.addStringArg(question["questionID"].asString());
								m.addStringArg("tag");
								m.addStringArg(tag.asString());
								populateBubble(m,bubble);
								oscOut.sendMessage(m);				
								log(source_names[i] + string(" > ") +
									bubble["queueID"].asString() + string(" > ") +
									bubble["userName"].asString(),0);
								
							}
						}
						
					}else{ //parse default bubble format
						
						int itemCount = json["totalItems"].asInt();
						if(itemCount==0){
							log(source_names[i] + ": no new bubbles",2);
						}
						for(int j=0;j<itemCount;j++){
							Json::Value bubble = json["items"][j];
							
							ofxOscMessage m;
							m.setAddress("/control/bubble/new");
							m.addStringArg("mode");
							m.addStringArg(source_names[i]);
							populateBubble(m,bubble);
							oscOut.sendMessage(m);				
							log(source_names[i] + string(" > ") +
								bubble["queueID"].asString() + string(" > ") +
								bubble["userName"].asString(),0);
						}
							
					}

				}
			}
			
			loaders[i]->reset();
		}
	}
	
	//is it time to call the server again?
	if(ofGetElapsedTimef() - lastConnectTime > polling_delay || lastConnectTime == 0){
		
		//do consecutive blocking calls
		for(int i=0;i<source_names.size();i++){
			if(i!=modes[0] && i!=modes[1])continue;
			
			if(!json_settings["sources"][source_names[i]]["manual_poll"].asBool()){
				poll(i);
			}
			
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
		if(e.what=="mouseClicked" && e.obj->className=="Button"){
				ControlBar::Button *b = (ControlBar::Button*)e.obj;
			if(b->text!="Swap"){
				if(e.obj==controlbar.children[0]){
					if(modes[0]!=-1)poll(modes[0]);
					log("polled current mode",3);
				}else if(e.obj==controlbar.children[3]){
					if(modes[1]!=-1)poll(modes[1]);
					log("polled next mode",3);
					
				}else{
				
					//mode buttons
					//set upcoming
					modes[1] = (int)b->userData;
					
					log(string("set upcoming mode to \"") + source_names[modes[1]] + "\"",3);
					((ControlBar::Button*)controlbar.children[4])->text = string("Next: ") + source_names[modes[1]];
					
					ofxOscMessage m;
					m.setAddress("/control/mode/next");
					m.addStringArg(source_names[modes[1]]);
					oscOut.sendMessage(m);
					updateEnabledButtons();
				}
			}else{
				//do swap
				if(modes[1] != -1){
					modes[0] = modes[1];
					
					((ControlBar::Button*)controlbar.children[1])->text = string("Mode: ") + source_names[modes[1]];
					((ControlBar::Button*)controlbar.children[4])->text = "Next:";

					modes[1] = -1;
					
					ofxOscMessage m;
					m.setAddress("/control/mode/swap");
					oscOut.sendMessage(m);
					
					log(string("swapped current mode to \"") + source_names[modes[0]] + "\"",3);
					updateEnabledButtons();
				}else{
					log("Swap error - you must first queue up an upcoming mode by pressing one of the buttons on the right",1);
				}
			}
		}
	}
}
//--------------------------------------------------------------

void testApp::updateEnabledButtons(){
	for(int i=0;i<2;i++){
		
		bool enabled = false;
		
		if(modes[i]!=-1){
			enabled = json_settings["sources"][source_names[modes[i]]]["manual_poll"].asBool();
		}
		
		ControlBar::Control *ctl = controlbar.children[i*3];
		if(enabled)ctl->enable();
		else ctl->disable();
	}
}

//--------------------------------------------------------------
void testApp::poll(int i){
	try{
		if(loaders[i]->status==0){
			loaders[i]->get(json_settings["sources"][source_names[i]]["url"].asString(),http_auth);
		}else{
			log(source_names[i] + " (previous network call not yet finished)",2);
		}
	}catch(logic_error err){
		log("Error calling remote server -- network down?",1);
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(50,50,50);
	controlbar.draw();
	glPushMatrix();
	glTranslatef(0,controlbar.rect.height,0);
	for(int i=0;i<console.size();i++){
		switch(console_colors[i]){
			case 0: ofSetColor(128,255,128);break;
			case 1: ofSetColor(255,128,128);break;
			case 2: ofSetColor(100,100,100);break;
			case 3: ofSetColor(255,200,0);break;
			default: ofSetColor(255,255,255);break;
		}
		font.drawString(console[i],0,6);
		glTranslatef(0,8,0);
	}
	glPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	//debug event
	if(key=='d'){
		
		//synthesize the offending bubble
		
		json = ofxJSON();
		string s = "{\"queueID\":\"37047\",\"profileImageURL\":\"http:\\/\\/a3.twimg.com\\/profile_images\\/1266569315\\/mallardmadness.jpg\",\
					\"userName\":\"shortLAU\",\"text\":\"test of 140 characters and here is a bit more text now\",\"status\":\"approved\",\
					\"hasMedia\":\"1\",\"promoted\":null,\"media\":{\"mediaID\":\"38987\",\"mediaThumbURL\":\"http:\\/\\/twitpic.com\\/show\\/thumb\\/4aeder\",\
					\"mediaURL\":\"http:\\/\\/twitpic.com\\/show\\/full\\/4aeder\"}}";
		json.parse(s);
		Json::Value bubble = json;
		
		ofxOscMessage m;
		m.setAddress("/control/bubble/new");
		m.addStringArg("mode");
		m.addStringArg("inspiration");
		populateBubble(m,bubble);
		oscOut.sendMessage(m);
	}

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
	controlbar.mouseUp();
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

//--------------------------------------------------------------
void testApp::populateBubble(ofxOscMessage &m,Json::Value &bubble){
	m.addStringArg("queueID");
	m.addStringArg(bubble["queueID"].asString());
	m.addStringArg("polledGroup");
	char numString[64];
	sprintf(numString,"%i",ofGetFrameNum());
	m.addStringArg(numString);
	
	m.addStringArg("profileImageURL");
	m.addStringArg(bubble["profileImageURL"].asString());
	m.addStringArg("userName");
	m.addStringArg(bubble["userName"].asString());
	m.addStringArg("text");
	m.addStringArg(bubble["text"].asString());
	
		//W+K guarantees only one twitpic per message now...
		//	if(bubble["hasMedia"].asString()=="1"){
		//		for(int k=0;k<bubble["media"].size();k++){
		//			m.addStringArg("mediaID");
		//			m.addStringArg(bubble["media"][k]["mediaID"].asString());
		//			m.addStringArg("mediaThumbURL");
		//			m.addStringArg(bubble["media"][k]["mediaThumbURL"].asString());
		//			m.addStringArg("mediaURL");
		//			m.addStringArg(bubble["media"][k]["mediaURL"].asString());
		//		}
		//	}
		//trying to iterate through something that doesn't have an array!
	
	if(bubble["hasMedia"].asString()=="1"){
		m.addStringArg("mediaID");
		m.addStringArg(bubble["media"]["mediaID"].asString());
		m.addStringArg("mediaThumbURL");
		m.addStringArg(bubble["media"]["mediaThumbURL"].asString());
		m.addStringArg("mediaURL");
		m.addStringArg(bubble["media"]["mediaURL"].asString());
	}
	
}

