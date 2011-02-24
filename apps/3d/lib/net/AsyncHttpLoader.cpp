/*
 *  AsyncHttpLoader.cpp
 *
 *  Created by Josh Nimoy on 2/9/11.
 *
 */

#include "AsyncHttpLoader.h"
#include <iostream>
#include "base64.h"

using namespace std;

AsyncHttpLoader::AsyncHttpLoader(){
	status = 0;
	timeout = 20;
	stopping = false;
}

AsyncHttpLoader::~AsyncHttpLoader(){
	if(status==1){
		stopping = true;
		while(status==1){
			//ofSleepMillis(1);
		}
	}
}

void AsyncHttpLoader::get(string s,string userpass){
	uri = URI(s);
	authInfo = userpass;
	pthread_create(&thread,NULL,AsyncHttpLoader::threadfunc, (void*)this);
}

void AsyncHttpLoader::reset(){
	status = 0;
	data = "";
	errorString = "";
}

void* AsyncHttpLoader::threadfunc(void*ptr){
	AsyncHttpLoader *ths = (AsyncHttpLoader*)ptr;
	ths->data = "";
	ths->errorString = "";
	
	std::string path(ths->uri.getPathAndQuery());
	if (path.empty()) path = "/";
	
	ths->status = 1;
	
	try{
		HTTPClientSession session(ths->uri.getHost(), ths->uri.getPort());
		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		
		string userpass = ths->authInfo;
		req.setCredentials("Basic", base64_encode((const unsigned char*)userpass.c_str(),userpass.size()));
		session.setTimeout(Poco::Timespan(ths->timeout,0));
		session.sendRequest(req);
		istream& rs = session.receiveResponse(ths->response);
		
		if(ths->stopping){
			ths->status = 2;
			return 0;
		}
		
		while(rs.good()){
			char c;
			rs.read(&c,1);
			if(rs.good())ths->data += c;
			
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

