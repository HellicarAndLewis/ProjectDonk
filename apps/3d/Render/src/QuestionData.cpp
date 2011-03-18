/*
 *  QuestionData.cpp
 *  Render
 *
 *  Created by Josh Nimoy on 2/13/11.
 *
 */

#include "QuestionData.h"

namespace Donk{
	
	map<string,QuestionData> QuestionData::all;
	
	QuestionData::QuestionData(){
	}
	
	QuestionData::QuestionData(ofxOscMessage &m){
		int index=0;
		while(index<m.getNumArgs()){
			string key = m.getArgAsString(index);
			index++;
			if(key=="questionID"){
				id = m.getArgAsString(index);
			}else if(key=="text"){
				text = m.getArgAsString(index);
			}else if(key=="tag1"){
				tags[0] = m.getArgAsString(index);
			}else if(key=="tag2"){
				tags[1] = m.getArgAsString(index);
			}else if(key=="count_tag1"){
				tag_counts[0] = atoi(m.getArgAsString(index).c_str());
			}else if(key=="count_tag2"){
				tag_counts[1] = atoi(m.getArgAsString(index).c_str());
			}
			index++;
		}
	}
	
	QuestionData::~QuestionData(){
		
	}
	
}