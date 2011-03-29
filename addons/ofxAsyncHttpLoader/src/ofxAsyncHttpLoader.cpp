/*
 *  ofxAsyncHttpLoader.cpp
 *
 *  Created by Josh Nimoy on 2/9/11.
 *
 */

#include "ofxAsyncHttpLoader.h"
#include <iostream>
#include <fstream>
#include "base64.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

ofxAsyncHttpLoader::ofxAsyncHttpLoader(){
	status = 0;
	timeout = 20;
	stopping = false;
}

ofxAsyncHttpLoader::~ofxAsyncHttpLoader(){
	if(status==1){
		stopping = true;
		while(status==1){
			//ofSleepMillis(1);
		}
	}
}

void ofxAsyncHttpLoader::get(string s,string userpass){
	uri = URI(s);
	authInfo = userpass;
	pthread_create(&thread,NULL,ofxAsyncHttpLoader::threadfunc, (void*)this);
}

void ofxAsyncHttpLoader::reset(){
	status = 0;
	data = "";
	errorString = "";
}

void* ofxAsyncHttpLoader::threadfunc(void*ptr){
	
begin_get:
	
	ofxAsyncHttpLoader *ths = (ofxAsyncHttpLoader*)ptr;
	ths->data = "";
	ths->errorString = "";
	
	std::string path(ths->uri.getPathAndQuery());
	
	if (path.empty()) path = "/";
	
	ths->status = 1;
	
	try{

		HTTPClientSession session(ths->uri.getHost(), ths->uri.getPort());
		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		
		if(!ths->authInfo.empty()){
			string userpass = ths->authInfo;
			req.setCredentials("Basic", base64_encode((const unsigned char*)userpass.c_str(),userpass.size()));
		}
		session.setTimeout(Poco::Timespan(ths->timeout,0));
		session.sendRequest(req);
		istream& rs = session.receiveResponse(ths->response);
		
		if(ths->response.getStatus()==302 || ths->response.getStatus()==303){
			string newUri = ths->response.get("Location");
			ths->uri = URI(urlDecode(newUri));
			goto begin_get;
		}
		
		
		if(ths->stopping){
			ths->status = 2;
			return 0;
		}
		
		
		while(rs.good()){
			char c;
			rs.read(&c,1);
			if(rs.good()){
				ths->data += c;
			}
			
			if(ths->stopping){
				ths->status = 2;
				return 0;
			}
		}
				
	}catch(Exception &err){
		ths->errorString = err.what();
	}
	ths->status = 2;

}

string ofxAsyncHttpLoader::urlDecode(string inStr){
	std::ostringstream outStr;
	for(int i=0;i<inStr.size();i++){
		switch(inStr[i]){
			case('+'):
			{
				outStr<<' ';
				break;
			}
			case('%'): // Convert all %xy hex codes into ASCII characters.
			{
				const std::string hexstr(inStr.substr(i+1,2)); // xy part of %xy.
				
				i+=2; // Skip over hex.
				//if(hexstr=="26" || hexstr=="3D"){
				//	// Do not alter URL delimeters.
				//	outStr<<'%'<<hexstr;
				//}else{
				std::istringstream hexstream(hexstr);
				int hexint;
				hexstream>>std::hex>>hexint;
				outStr<<static_cast<char>(hexint);
				//}
				break;
			}
			default: // Copy anything else.
			{
				outStr<<inStr[i];
				break;
			}
		}
	}
	return outStr.str();
	
}






