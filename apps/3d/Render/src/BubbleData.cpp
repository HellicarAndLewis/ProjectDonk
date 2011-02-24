/*
 *  BubbleData.cpp
 *  Render
 *
 *  Created by Josh Nimoy on 2/11/11.
 *
 */

#include "BubbleData.h"

namespace Donk{

	void BubbleData::step(){
		if(profileImageLoader!=NULL){
			if(profileImageLoader->status==2){
				ofBuffer buff;
				buff.set(profileImageLoader->data.c_str(), profileImageLoader->data.size());
				profileImage.loadImage(buff);
				delete profileImageLoader;
				profileImageLoader = NULL;
			}
		}
		
		vector<MediaEntry>::iterator mit;
		for(mit=media.begin();mit!=media.end();mit++){
			mit->step();
		}
		
	}
	
	BubbleData::BubbleData(ofxOscMessage &m){
		int index=0;
		profileImageLoader = NULL;
		
		while(index<m.getNumArgs()){
			string key = m.getArgAsString(index);
			index++;
			if(key=="mode"){
				mode = m.getArgAsString(index);
			}else if(key=="queueID"){
				id = m.getArgAsString(index);
			}else if(key=="profileImageURL"){
				profileImageURL = m.getArgAsString(index);
				
				//start downloading it
				profileImageLoader = new AsyncHttpLoader();
				profileImageLoader->get( profileImageURL, "" );
				
			}else if(key=="userName"){
				userName = m.getArgAsString(index);
			}else if(key=="text"){
				text = m.getArgAsString(index);
			}else if(key=="questionID"){
				questionID = m.getArgAsString(index);
			}else if(key=="text"){
				tag = m.getArgAsString(index);
			}else if(key=="mediaID"){
				media.push_back(MediaEntry());
				media.back().id = m.getArgAsString(index);
			}else if(key=="mediaThumbURL"){
				media.back().thumb_url = m.getArgAsString(index);
				//start downloading it
				media.back().thumbLoader = new AsyncHttpLoader();
				media.back().thumbLoader->get( media.back().thumb_url, "" );
				
			}else if(key=="mediaURL"){
				media.back().url = m.getArgAsString(index);
				media.back().mediaLoader = new AsyncHttpLoader();
				media.back().mediaLoader->get( media.back().url, "" );
			}
			index++;
		}
	}

	BubbleData::~BubbleData(){
		if(profileImageLoader!=NULL)delete profileImageLoader;
	}
	
	BubbleData::MediaEntry::MediaEntry(){
		
	}
	
	BubbleData::MediaEntry::~MediaEntry(){
		if(thumbLoader!=NULL)delete thumbLoader;
		if(mediaLoader!=NULL)delete mediaLoader;
	}
	
	void BubbleData::MediaEntry::step(){
		
		if(thumbLoader!=NULL){
			if(thumbLoader->status==2){
				ofBuffer buff;
				buff.set(thumbLoader->data.c_str(), thumbLoader->data.size());
				thumbImage.loadImage(buff);
				delete thumbLoader;
				thumbLoader = NULL;
			}
		}

		if(mediaLoader!=NULL){
			if(mediaLoader->status==2){
				ofBuffer buff;
				buff.set(mediaLoader->data.c_str(), mediaLoader->data.size());
				mediaImage.loadImage(buff);
				delete mediaLoader;
				mediaLoader = NULL;
			}
		}
		
	}
	
	

}
