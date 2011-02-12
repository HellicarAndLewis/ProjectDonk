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

using namespace std;

namespace Donk{
	
	class BubbleData{
	public:
		BubbleData(ofxOscMessage &m);
		~BubbleData();
		string mode;
		string id;
		string profileImageURL;
		string userName;
		string text;
		
		class MediaEntry{
		public:
			string id;
			string thumb_url;
			string url;
		};
		
		vector<MediaEntry> media;
		
	};
	
}
