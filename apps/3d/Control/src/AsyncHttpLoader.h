/*
 *  AsyncHttpLoader.h
 *  Control
 *
 *  Created by Josh Nimoy on 2/9/11.
 *
 */

#pragma once

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;

using Poco::Path;
using Poco::URI;
using Poco::Exception;

#include <string>
using namespace std;

/**
 asyncronous http client
 because OF doesn't have one
 this class should always be
 dynamically allocated. Because it
 has its own thread, and the thread
 works with the instance, that instance 
 needs to always exist during the thread
 or else memory access error will occur.
 */
class AsyncHttpLoader{
public:
	AsyncHttpLoader();
	~AsyncHttpLoader();
	/**
	 trigger the server call
	 */
	void get(string s,string userpass);
	/**
	 the return data. don't touch this
	 unless done==true. the thread
	 will be vigorously writing to it
	 and i'm not mutexing.
	 */
	string data;
	/**
		the URL passed to get()
	 */
	URI uri;
	
	/**
		flags that the thread is (almost) finished
		and the data is safe to retrieve
	 */
	bool done;
	
	/**
		resets the object, among other things
		setting done to false again
	 */
	void reset();
	
	/**
		posix thread object
	 */
	pthread_t thread;
	
	/**
		static thread func. gets passed a pointer to this instance
	 */
	static void* threadfunc(void*ptr);
	
	/**
		the Poco response object. useful
		because you can call ::getStatus()
	 */
	HTTPResponse response;
	
	/**
		error message caught in the thread.
		empty if no errors.
	 */
	string errorString;
	
	/**
		timeout seconds
	 */
	int timeout;
	/**
		username:password
	 */
	string authInfo;
};


