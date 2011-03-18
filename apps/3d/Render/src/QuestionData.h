/*
 *  QuestionData.h
 *  Render
 *
 *  Created by Josh Nimoy on 2/13/11.
 *
 */

#pragma once

#include "ofxOsc.h"
#include <string>
#include <map>

using namespace std;

namespace Donk{
	
	class QuestionData{
	public:
		QuestionData();
		QuestionData(ofxOscMessage &m);
		~QuestionData();
		string id;
		string text;
		string tags[2];
		int tag_counts[2];
		
		static map<string,QuestionData> all;
	};
	
}

