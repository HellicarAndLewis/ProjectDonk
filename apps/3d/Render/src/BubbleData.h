/*
 *  BubbleData.h
 *  Render
 *
 *  Created by Josh Nimoy on 2/11/11.
 *
 */

#pragma once

#include "ofxOsc.h"
#include <vector>
#include <string>
#include "AsyncHttpLoader.h"
#include "ofxCubeMap.h"
#include <GLUT/glut.h>

using namespace std;

namespace Donk{
	
	class BubbleData{
	public:
		BubbleData(ofxOscMessage &m);
		~BubbleData();
		
		/**
			basic fields received
		 */
		string mode;
		string id;
		string polledGroup;
		string profileImageURL;
		string userName;
		string text;
		string questionID; ///only filled by question bubbles
		string tag; ///only filled by question bubbles
		
		class MediaEntry{
		public:
			MediaEntry();
			~MediaEntry();
			string id;
			string thumb_url;
			string url;
			/**
				network loaders
			 */
			AsyncHttpLoader *thumbLoader;
			AsyncHttpLoader *mediaLoader;
			/**
				drawable images
			 */
			ofImage thumbImage;
			ofImage mediaImage;
			
			/**
				receiving "update" event from testApp
			 */
			void step();
			
		};
		
		bool doneLoading();
		
		vector<MediaEntry> media;
	private:		
		/**
			network loader for profile image
		 */
		AsyncHttpLoader *profileImageLoader;
		bool loadingDone;
		
	public:
		
		ofImage profileImage;
		
		/**
			receiving "update" event from testApp
		 */
		void step();
		static vector<BubbleData*> all;
		static void update();
		static void add(ofxOscMessage &m);		
	};
	
}

