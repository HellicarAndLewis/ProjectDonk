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
	tcpClient.setVerbose(true);
	lastConnectTime = 0;
	font.loadFont("uni05_54.ttf", 6);
	font.setLineHeight(8);
	
	http_header_mode = true;
	http_expect_connected = false;
	
	if(!settings.loadFromFile("settings.json.txt")){
		string msg = "FATAL ERROR: failed to load JSON settings file: data/settings.json.txt";
		log(msg);
		cout << msg << endl;
		_exit(1);
	}
	
	polling_delay = settings["polling_delay_time_in_seconds"].asDouble();

}

//--------------------------------------------------------------
void testApp::update(){
		
	if(tcpClient.isConnected()){
		string rcv = tcpClient.receiveRaw();
		if(tcpClient.getNumReceivedBytes() > 0){
			parseReceivedBytes(rcv);
		}
	}else{
		//is the connection freshly closed?
		if(http_expect_connected){
			//web page is done loading now.
			http_expect_connected = false;
			parseJSON(http_data_buffer);
		}
		
		
		//is it time to call the server again?
		if(ofGetElapsedTimef() - lastConnectTime > polling_delay || lastConnectTime == 0){
	
			int server_port = settings["server_port"].asInt();

			http_header_mode = true;//go back to waiting for header chunk
			http_data_buffer.clear(); //reset data buffer
			
			// not connected, but waited long enough,
			// or it's the first time, so calling right away
			
			string ip = getIpFromDomain(settings["server"].asString());
			if(ip==""){
				log("Domain resolution failed. Network may be down.");
			}else{
				if(!tcpClient.setup(ip, server_port, false)){
					log("failed to setup TCP client");
				}else if(!tcpClient.sendRaw(string("GET ") + settings["path_buzz"].asString() + string(" HTTP 1.0\n\n"))){
					log("failed to send HTTP GET to web server");
					tcpClient.close();
				}else{
					log("calling server");
					http_expect_connected = true;
				}
			
			}
			
			lastConnectTime = ofGetElapsedTimef();
		}
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

string testApp::getIpFromDomain(string name){
	hostent *h = gethostbyname(name.c_str());
	if(h==NULL){
		return string("");
	}
	char ipstr[18];
	sprintf(ipstr,"%i.%i.%i.%i",
			(unsigned char)h->h_addr_list[0][0],
			(unsigned char)h->h_addr_list[0][1],
			(unsigned char)h->h_addr_list[0][2],
			(unsigned char)h->h_addr_list[0][3]);
	return string(ipstr);
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

//--------------------------------------------------------------

void testApp::parseReceivedBytes(string inBytes){
	if(http_header_mode){
		int nCount = 0;//consecutive line feed counter
		string::iterator it = inBytes.begin();
		for(;it!=inBytes.end();it++){
			if(http_header_mode){
				//just waiting for it to be over
				if(*it=='\n'||*it=='\r'){
					nCount++;
					if(nCount==4){
						http_header_mode = false;//start collecting the data from here	
					}
				}else nCount=0;
			}else{
				http_data_buffer += *it;
			}
		}
	}else http_data_buffer += inBytes;//otherwise it's quite simple.
}

//--------------------------------------------------------------

void testApp::parseJSON(string &data){
	//json.parse(data);
	//json.print();
	cout << data << endl;
}


