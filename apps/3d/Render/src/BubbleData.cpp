/*
 *  BubbleData.cpp
 *  Render
 *
 *  Created by Josh Nimoy on 2/11/11.
 *
 */

#include "BubbleData.h"

namespace Donk{

	BubbleData::BubbleData(ofxOscMessage &m){
		int index=0;
		while(index<m.getNumArgs()){
			string key = m.getArgAsString(index);
			index++;
			if(key=="mode"){
				mode = m.getArgAsString(index);
			}else if(key=="queueID"){
				id = m.getArgAsString(index);
			}else if(key=="profileImageURL"){
				profileImageURL = m.getArgAsString(index);
			}else if(key=="userName"){
				userName = m.getArgAsString(index);
			}else if(key=="text"){
				text = m.getArgAsString(index);
			}else if(key=="mediaID"){
				media.push_back(MediaEntry());
				media.back().id = m.getArgAsString(index);
			}else if(key=="mediaThumbURL"){
				media.back().thumb_url = m.getArgAsString(index);
			}else if(key=="mediaURL"){
				media.back().url = m.getArgAsString(index);
			}
			index++;
		}
	}

	BubbleData::~BubbleData(){
		
	}

}