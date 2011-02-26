/*
 *  BubbleData.cpp
 *  Render
 *
 *  Created by Josh Nimoy on 2/11/11.
 *
 */

#include "BubbleData.h"
#include <GLUT/GLUT.h>

namespace Donk{

	vector<BubbleData*> Donk::BubbleData::all;

	void BubbleData::add(ofxOscMessage &m){
		all.push_back(new BubbleData(m));
	}
	
	void BubbleData::render(){
		//draw all the bubbles
		glPushMatrix();
		vector<BubbleData*>::iterator bdit;
		for(bdit=all.begin();bdit!=all.end();bdit++)(*bdit)->draw();
		glPopMatrix();
		
	}
	
	
	void BubbleData::draw(){
		if(profileImage.width != 0){
			ofSetColor(255,255,255);
			profileImage.bind();
			float w = profileImage.width;
			float h = profileImage.height;
			
			//draw the circle-masked thumbnail
			int steps = 60;
			float inc = TWO_PI/(float)steps;			
			glBegin(GL_TRIANGLE_FAN);
			for(int i=0;i<steps;i++){
				float x = cos(inc*i);
				float y = sin(inc*i);
				glTexCoord2f((x+1)*w*0.5,(y+1)*h*0.5);
				glVertex2f(x*radius,y*radius);
			}
			glEnd();
			profileImage.unbind();
			
			//glutWireSphere(radius,10,10);
			
			//move to the right, for now.
			glTranslatef(radius*2,0,0);
		}
		
	}
	
	
	void BubbleData::update(){
		//call update on all the bubbles
		vector<BubbleData*>::iterator bdit;
		for(bdit=all.begin();bdit!=all.end();bdit++){
			(*bdit)->step();
		}
		
	}

	
	
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
		
		radius = ofRandomuf()*100+50;
		
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
		thumbLoader = NULL;
		mediaLoader = NULL;
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
