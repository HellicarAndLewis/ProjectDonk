/*
 *  BubbleData.cpp
 *  Render
 *
 *  Created by Josh Nimoy on 2/11/11.
 *
 */

#include "BubbleData.h"
#include <GLUT/GLUT.h>
#include "BubbleProjection.h"

namespace Donk{

	vector<BubbleData*> BubbleData::all;

	//--------------------------------------------------------
	void BubbleData::add(ofxOscMessage &m){
		all.push_back(new BubbleData(m));
	}
	
	
	//--------------------------------------------------------
	void BubbleData::update(){
		//call update on all the bubbles
		vector<BubbleData*>::iterator bdit;
		for(bdit=all.begin();bdit!=all.end();bdit++){
			if(!(*bdit)->doneLoading()) {
				(*bdit)->step();
			}
		}
		
	}

	//--------------------------------------------------------
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
		bool allMediaLoaded = true;
		for(mit=media.begin();mit!=media.end();mit++){
			mit->step();
			
			// check to see if we're totally loaded
			if(mit->thumbImage.width==0 && mit->mediaImage.width==0) {
				allMediaLoaded = false;
			}
		}
		
		bool oldLoadingDone = loadingDone;
		loadingDone = profileImage.width>0 && allMediaLoaded;
		
		if(!oldLoadingDone && loadingDone) {
			// put it in the physics system
			printf("Done loading %s %s\n", userName.c_str(), text.c_str());
			BubbleProjection::getInstance()->bubbleReceived(this);
		}
		
		
	}
	
	//--------------------------------------------------------
	BubbleData::BubbleData(ofxOscMessage &m){
		
		loadingDone = false;
		
		int index=0;
		profileImageLoader = NULL;
		
		while(index<m.getNumArgs()){
			string key = m.getArgAsString(index);
			index++;
			if(key=="mode"){
				mode = m.getArgAsString(index);
			}else if(key=="queueID"){
				id = m.getArgAsString(index);
				
			}else if(key=="polledGroup"){
				polledGroup = m.getArgAsString(index);
				
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
//			}else if(key=="text"){ //think this is extraneous JGL
//				tag = m.getArgAsString(index);
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

	//--------------------------------------------------------
	BubbleData::~BubbleData(){
		if(profileImageLoader!=NULL)delete profileImageLoader;
		
			//what about the media that might have been loaded in? 
	}
	
	//--------------------------------------------------------
	BubbleData::MediaEntry::MediaEntry(){
		thumbLoader = NULL;
		mediaLoader = NULL;
	}
	
	//--------------------------------------------------------
	BubbleData::MediaEntry::~MediaEntry(){
		if(thumbLoader!=NULL)delete thumbLoader;
		if(mediaLoader!=NULL)delete mediaLoader;
	}
	
	//--------------------------------------------------------
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
	
	//--------------------------------------------------------
	bool BubbleData::doneLoading() {
		return loadingDone;
	}
}
