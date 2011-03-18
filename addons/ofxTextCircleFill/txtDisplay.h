/*
 *  txtDisplay.h
 *  txtmachinegun
 *
 *  Created by Chris on 9/30/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
//#include "ofxXmlSettings.h"



typedef struct {
	string words;
	int width, height;
} messageLines;

#define TEXT_ALIGN_CENTER	0
#define TEXT_ALIGN_LEFT		1

class TxtDisplay{

	public:
		TxtDisplay();
		~TxtDisplay();
		
		void setup(string fontPath, int minSize, int maxSize, int stepSize);
		void setMessage(string message);
		
		void draw(float centerX, float centerY);
		void drawFormatting();
		
		int textAlignMode;
		int textBoxWidth;
		int textBoxHeight;
		float lineSpace;
            
        //helpers for rendering txt yourself
        float getLineHeight();
        ofTrueTypeFont* getBestFont();
        ofPoint getTopLeft(float centerX, float centerY);
    
		bool bSetMessage;
        bool bFontsLoaded;
    
	protected:
		
		void tokenize(const string& str,vector<string>& tokens,string delimiters = " ");
		bool fitText( vector<string> wrds, vector<messageLines> & linZ, int & height, int & lineHeight, int & fontSize );
		void findBestTextFit( vector<string> wrds, vector<messageLines> & linZ, int & height, int & lineHeight, int & fontSize );
        void clearFonts();
    
		ofTrueTypeFont * getFontFromSize(int size);
		
		
		vector<string> words;
		vector<messageLines> lines;
		
        int mMinSize;
        int mMaxSize;
        int mStepSize;
		
		int mFontSize;
		int totalHeight;
		int maxLineHeight;
		
		
        /*
		ofTrueTypeFont font;
		ofTrueTypeFont font36;
		ofTrueTypeFont font24;
		ofTrueTypeFont font18;
		ofTrueTypeFont font16;
		ofTrueTypeFont font14;
		*/
        //JG made a map to support flex editing
        map<int, ofTrueTypeFont*> fonts;
		//int bestFontSize;
		
		
		
};