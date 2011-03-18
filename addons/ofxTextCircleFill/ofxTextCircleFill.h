/**
 *  ofxTextCircleFill.h
 *
 *  Created by James George on 3/18/2011
 *  Project Donk
 *
 */

#pragma once

#include "ofMain.h"

typedef struct {
	string words;
	int width, height;
} MessageLine;


class ofxTextCircleFill
{

  public:
    ofxTextCircleFill();
    ~ofxTextCircleFill();
    
    void setup(string fontPath, int minSize, int maxSize, int stepSize, float radius);
    void setMessage(string message);
    
    void draw(float centerX, float centerY);
        
    bool bSetMessage;
    bool bFontsLoaded;
    
  protected:
    
    void findBestTextFit();
    void clearFonts();

    ofTrueTypeFont * getFontFromSize(int size);
    
    vector<string> words;
    vector<MessageLine> lines;
    
    int mMinSize;
    int mMaxSize;
    int mStepSize;
    float mRadius;
    
    int mFontSize;
    int mTotalLineHeight;
    int mMaxLineHeight;
    string mFullMessage;
    
    //JG made a map to support flex editing
    map<int, ofTrueTypeFont*> fonts;

};