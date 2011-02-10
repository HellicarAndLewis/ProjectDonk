/*
 *  AsyncHttpLoader.cpp
 *  Control
 *
 *  Created by Josh Nimoy on 2/9/11.
 *
 */

#include "AsyncHttpLoader.h"
#include <iostream>

using namespace std;

AsyncHttpLoader::AsyncHttpLoader(){
	done = false;
	timeout = 20;
}

AsyncHttpLoader::~AsyncHttpLoader(){
	
}

void AsyncHttpLoader::get(string s){
	uri = URI(s);
	pthread_create(&thread,NULL,AsyncHttpLoader::threadfunc, (void*)this);
}

void AsyncHttpLoader::reset(){
	done = false;
	data = "";
	errorString = "";
}

void* AsyncHttpLoader::threadfunc(void*ptr){
	AsyncHttpLoader *ths = (AsyncHttpLoader*)ptr;
	ths->data = "";
	ths->errorString = "";
	
	std::string path(ths->uri.getPathAndQuery());
	if (path.empty()) path = "/";
	
	try{
		HTTPClientSession session(ths->uri.getHost(), ths->uri.getPort());
		HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		session.setTimeout(Poco::Timespan(ths->timeout,0));
		session.sendRequest(req);
		istream& rs = session.receiveResponse(ths->response);
		while(rs.good()){
			char c;
			rs >> c;
			if(rs.good())ths->data += c;
		}
	}catch(Exception &err){
		ths->errorString = err.what();
	}
	ths->done = true;

}

