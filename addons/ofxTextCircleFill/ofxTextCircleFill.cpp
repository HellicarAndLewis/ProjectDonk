/**
 *  ofxTextCircleFill.h
 *
 *  Created by James George on 3/18/2011
 *  Project Donk
 *
 */

#include "ofxTextCircleFill.h"

ofxTextCircleFill::ofxTextCircleFill()
{
    bSetMessage = false;
    bFontsLoaded = false;
    
    //defaults
    mMaxLineHeight = 0;
	mTotalLineHeight = 0;
	//lineSpace = .8;
}

ofxTextCircleFill::~ofxTextCircleFill()
{
    clearFonts();
}


void ofxTextCircleFill::setup(string fontPath, int minSize, int maxSize, int stepSize, float radius)
{
    if(stepSize <= 0){
        ofLog(OF_LOG_ERROR, "TxtDisplay: Size Step must be at least 1");
        stepSize = 1;
    }
    
    mRadius = radius;
    bSetMessage = false;
    bFontsLoaded = false;
    
    clearFonts();
    
    int numFontsLoaded = 0;

    for(int i = minSize; i <= maxSize; i += stepSize){
        ofTrueTypeFont* font = new ofTrueTypeFont();
        font->loadFont(fontPath, i, true, true, false);
        fonts[i] = font;
        
        numFontsLoaded++;
    }
    
    if(numFontsLoaded == 0){
        ofLog(OF_LOG_ERROR, "TxtDisplay: Did not load any fonts");
    }
    bFontsLoaded = true;
    
    mMinSize = minSize;
    mMaxSize = maxSize;
    mStepSize = stepSize;
    
}

void ofxTextCircleFill::clearFonts()
{
    //TODO make static
    map<int,ofTrueTypeFont*>::iterator it;
    for(it = fonts.begin(); it != fonts.end(); it++){
        delete it->second;
        //cout << "deleted " << it->first << " font" << endl;
    }
    fonts.clear();
}

void ofxTextCircleFill::setMessage(string message)
{

	// clear words
	words.clear();
	lines.clear();
    
	// split message into words
	words = ofSplitString(message, " \n");
	
	mFullMessage = message;
    
	findBestTextFit();
	
    mTotalLineHeight = 0;
    ofTrueTypeFont* font = getFontFromSize(mFontSize);
    for(int l = 0; l < lines.size(); l++){
        
        if( lines[l].words.size() == 0 ) continue;
        
        lines[l].width  = font->stringWidth(  lines[l].words );
        lines[l].height = font->stringHeight( lines[l].words );
        mMaxLineHeight = MAX(lines[l].height, mMaxLineHeight );
        
    }
    
    mTotalLineHeight = mMaxLineHeight*lines.size();
    
    cout << "circle with radius " << mRadius << " fit the line with " << lines.size() << " lines and font size " << mFontSize << " and total height " << mTotalLineHeight << endl;
	bSetMessage = true;
}

void ofxTextCircleFill::draw(float centerX, float centerY)
{
	if(lines.size() <= 0 ) return;
	if(!bSetMessage) return;
	
	ofTrueTypeFont* myFont = getFontFromSize(mFontSize);
	
	int x = 0;
	int y = 0;
	for( int i = 0; i < lines.size(); i++)
	{
		if( lines[i].words.size() <= 0 ) continue;
		
        x = centerX - (lines[i].width*.5);        
        y = centerY + (mMaxLineHeight*i - mTotalLineHeight*.5);
        
		myFont->drawString(lines[i].words, x, y);		
	}
}

void ofxTextCircleFill::findBestTextFit()
{
    
	//fontSize = 22;
	bool bFitTest = false;
	//step down until we get to one that fits
    for(int fontSize = mMaxSize; fontSize >= mMinSize; fontSize -= mStepSize){
        ofTrueTypeFont* thisFont = getFontFromSize(fontSize);
        float lineHeight = thisFont->stringHeight(mFullMessage);
        //int spaceWidth = thisFont->stringWidth(" ");
        int maxLinesInCircle = int(mRadius)/lineHeight;
        
        for(int numLines = 1; numLines <= maxLinesInCircle; numLines++){
            
            //attempt fit with this font and this many lines
            lines.clear();
            int currentWord = 0;

            //cout << "trying font size " << fontSize << " with line height " << lineHeight << " and " << numLines << " lines. "  << endl;

            for(int i = -(numLines-1)/2; i <= (numLines/2); i++){
                
                float lineY = i*lineHeight;
                //Nimoyian Theorm, tubular radiation!! 
                float lineWidth = sqrt(mRadius*mRadius - lineY*lineY);
                
                //cout << " Line Y is " << lineY << " line width " << lineWidth << endl;
                
                MessageLine line;
                line.words = "";
                bool firstWordOnLine = true;
                for(; currentWord < words.size(); currentWord++){
                    
                    string lineWithNextWord;
                    if(firstWordOnLine){
                        lineWithNextWord = words[currentWord];
                        firstWordOnLine = false;
                    }
                    else{
                        lineWithNextWord = line.words + " " + words[currentWord];
                    }
                    
                    float currentLineWidth = thisFont->stringWidth(lineWithNextWord);
                    if(currentLineWidth > lineWidth){
                        break;
                    }
                    line.words = lineWithNextWord;
                }
                lines.push_back( line ); 
                
                if(currentWord == words.size()){
                    //we're done, it fit! finalize calculations
                    mFontSize = fontSize;
                    return;
                }
            }
        }
    }
    
    //nothing ever fit!!
    mFontSize = mMinSize;
    ofLog(OF_LOG_WARNING, "TextDisplay: No Fit Found, defaulting to smallest font");
}

ofTrueTypeFont * ofxTextCircleFill::getFontFromSize(int size)
{
    return fonts[size];
}


