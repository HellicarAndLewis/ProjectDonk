/*
 *  txtDisplay.cpp
 *  txtmachinegun
 *
 *  Created by Chris on 9/30/10.
 *  Copyright 2010 csugrue. All rights reserved.
 *
 */

#include "txtDisplay.h"

TxtDisplay::TxtDisplay()
{
    bSetMessage = false;
    bFontsLoaded = false;
    
    //defaults
	totalHeight = 0;
	lineSpace = .8;
	textBoxWidth = 640;
	textBoxHeight = 480;
	textAlignMode = TEXT_ALIGN_CENTER;
    
}

TxtDisplay::~TxtDisplay()
{
    clearFonts();
}

//void TxtDisplay::setup()
void TxtDisplay::setup(string fontPath, int minSize, int maxSize, int stepSize)
{
    if(stepSize <= 0){
        ofLog(OF_LOG_ERROR, "TxtDisplay: Size Step must be at least 1");
        stepSize = 1;
    }
    
    bSetMessage = false;
    bFontsLoaded = false;
    
//	bestFontSize = 24;
//	mFontSize = 24;
	
//	ofxXmlSettings xml;
	//xml.loadFile("appSettings.xml");
	//string fontName = "fonts/" + xml.getValue("font","verdana.ttf");
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
    
//       font.loadFont(fontName,24,true,true,false);
//        font36.loadFont(fontName,22,true,true,false);
//        font24.loadFont(fontName,20,true,true,false);
//        font18.loadFont(fontName,18,true,true,false);
//        font16.loadFont(fontName,16,true,true,false);
//        font14.loadFont(fontName,14,true,true,false);
    


}

void TxtDisplay::clearFonts()
{
    map<int,ofTrueTypeFont*>::iterator it;
    for(it = fonts.begin(); it != fonts.end(); it++){
        delete it->second;
        cout << "deleted " << it->first << " font" << endl;
    }
    fonts.clear();
}

void TxtDisplay::setMessage(string message)
{

	// clear words
	words.clear();
	lines.clear();
	
    //TODO: does not support new lines so well...
    
	// split message into words
	tokenize(message, words, " ");
	
	//ofTrueTypeFont* myFont = getFontFromSize(bestFontSize);
	//int lineHeight = myFont->stringHeight("O");
	
	findBestTextFit(words, lines, totalHeight, maxLineHeight, mFontSize);
	//maxLineHeight = lineHeight;
	
	bSetMessage = true;

    cout << "txtbox w/h " << textBoxWidth << " " << textBoxHeight << " lines " << lines.size() << " line height: " << maxLineHeight << " font size " << mFontSize <<  endl;

}

void TxtDisplay::draw(float centerX, float centerY)
{
	if(lines.size() <= 0 ) return;
	if(!bSetMessage) return;
	
	ofTrueTypeFont* myFont = getFontFromSize(mFontSize);
	
	int x = 0;
	int y = 0;
	
	for( int i = 0; i < lines.size(); i++)
	{
		if( lines[i].words.size() <= 0 ) continue;
		
		if(textAlignMode == TEXT_ALIGN_CENTER)
		{
			x = centerX-(lines[i].width*.5);
			
			//y = (centerY-totalHeight*.5) + ( (lines[i].height)*i );
			y = (centerY-totalHeight*.5) + ( maxLineHeight*i );
		}
        else if (textAlignMode == TEXT_ALIGN_LEFT) {
			x = centerX-textBoxWidth*.5;
			//y = (centerY-totalHeight*.5) + ( (lines[i].height)*i );
			y = (centerY-totalHeight*.5) + ( maxLineHeight*i );
		}
        
		myFont->drawString(lines[i].words, x, y);		
	}
	
}


float TxtDisplay::getLineHeight()
{
    return maxLineHeight;
}

ofTrueTypeFont* TxtDisplay::getBestFont()
{
    if(!bSetMessage) return getFontFromSize(mMinSize);
    
    return getFontFromSize(mFontSize);
}

ofPoint TxtDisplay::getTopLeft(float centerX, float centerY)
{
	if(lines.size() <= 0 ) return ofPoint(centerX, centerY);
	if(!bSetMessage) return ofPoint(centerX, centerY);
    
    int x,y;
    
	for( int i = 0; i < lines.size(); i++)
	{
		if( lines[i].words.size() <= 0 ) continue;
		
		if(textAlignMode == TEXT_ALIGN_CENTER)
		{
			x = centerX-(lines[i].width*.5);
			
			//y = (centerY-totalHeight*.5) + ( (lines[i].height)*i );
			y = (centerY-totalHeight*.5) + ( maxLineHeight*i );
		}
        else if (textAlignMode == TEXT_ALIGN_LEFT) {
			x = centerX-textBoxWidth*.5;
			//y = (centerY-totalHeight*.5) + ( (lines[i].height)*i );
			y = (centerY-totalHeight*.5) + ( maxLineHeight*i );
		}
        
		return ofPoint(x,y);
	}
}


void TxtDisplay::drawFormatting()
{
	//mFontSize = 24;
	
    //JG don't use this function i broke it!
    draw(0,0);
    
    return;
    
    //JG this is broken now....
	string dummyText = "Feel the poison in my veins. Tell me are you strong enough. To keep up with my life. Welcome to the dangerzone. What you see ain't what you get. Let us fly this aeroplane. Tonight!";
	//string dummyText = "ライブラリーでは、所蔵資料の中からテーマにそった小さな展示を行っています。6月から始まったミニ展示「明治・大正時代の日本ガイドブック」、2回目の7月は「古都の旅」と題し、外国人旅行者のために書かれた京都や奈良のガイドブックを展示します。\n\n 한국에서 발매할 PS3는3월에 유럽에서 발매한 PS3와 동일한 최신 모델을 사용하여 새롭게 의 대용량 HDD(하드디스크 드라이브)를 표준 탑재하고 있다.  대용량HDD를 탑재함으로써 PS3발매와 동시에 실시할 온라인 서비스 “PLAYSTATION®Network”로 선보이게 될 다양한 엔터테인먼트 콘텐츠 다운로드와 온라인 게임 및 온라인 서비스를 보다 쾌적하게 사용할 수 있도록 하였다.\n\nHovedgruppen må være på minimum 10 personer, men deltakere kan knytte seg til hovedgruppen fra forskjellige destinasjoner. Tilknytningsbillett er gyldig inntil 24 timer før avreise hovedgruppe.\n\nUnicef said 3m people had been affected and more than 1,400 had been killed. The government said some 27,000 people remained trapped and awaiting help.";
	
    vector<string> tempwords;
	tokenize(dummyText,tempwords," ");
	
	int height = 0;
	vector<messageLines> mylines;
	
	ofTrueTypeFont * myFont = getFontFromSize(mFontSize);
	int lineHeight = myFont->stringHeight("O");
	
	findBestTextFit(tempwords,mylines,height,lineHeight,mFontSize);

	ofTrueTypeFont * myFont2 = getFontFromSize(mFontSize);
		
	int centerX = 40+textBoxWidth*.5;
	int centerY = 80+.5*(textBoxHeight);
	
	int x = 0;
	int y = 0;
	
	ofFill();
	ofSetColor(0,0,0);
	ofRect( centerX-(textBoxWidth*.5),(centerY-height*.5)-lineHeight,textBoxWidth, textBoxHeight );
	
	ofNoFill();
	ofSetColor(255,255,255);
	ofRect( centerX-(textBoxWidth*.5),(centerY-height*.5)-lineHeight,textBoxWidth, textBoxHeight );
	
	for( int i = 0; i < mylines.size(); i++)
	{
		if( mylines[i].words.size() <= 0 ) continue;
		
		if(textAlignMode == TEXT_ALIGN_CENTER)
		{
			x = centerX-(mylines[i].width*.5);
			y = (centerY-height*.5) + ( lineHeight*i );
		}else if (textAlignMode == TEXT_ALIGN_LEFT)
		{
			x = centerX-textBoxWidth*.5;
			y = (centerY-height*.5) + ( lineHeight*i );
		}
		
		myFont2->drawString(mylines[i].words, x, y);		
	}
	
}

void TxtDisplay::tokenize(const string& str,vector<string>& tokens,string delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);
	
    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void TxtDisplay::findBestTextFit( vector<string> wrds, vector<messageLines> & linZ, int & height, int & lineHeight, int & fontSize)
{
	//fontSize = 22;
	bool bFitTest = false;
	//step down until we get to one that fits
    for(fontSize = mMaxSize; fontSize >= mMinSize; fontSize -= mStepSize){
        if(fitText(wrds, linZ, height, lineHeight, fontSize)){
            cout << "found match at font size " << fontSize << " with line height " << lineHeight << endl;
            return;
        }
    }
    
    ofLog(OF_LOG_WARNING, "TextDisplay: No Fit Found, defaulting to smallest font");
    fitText(wrds, linZ, height, lineHeight, mMinSize);
    fontSize = mMinSize;
    
    
    /*
	while(!bFitTest && fontSize > 14)
	{
		
		bFitTest = fitText(wrds, linZ, height, lineHeight,fontSize);
		if(!bFitTest ) fontSize -= 2;

	}
     */
	
}

bool  TxtDisplay::fitText( vector<string> wrds, vector<messageLines> & linZ, int & height, int & lineHeight, int & fontSize )
{
	
	ofTrueTypeFont * myFont = getFontFromSize(fontSize);
	lineHeight = myFont->stringHeight("O");
    
    //cout << " line height <-> font :: " << lineHeight << " <-> " << fontSize << endl;
    
	// min max box 
	float maxLineWidth = textBoxWidth;
	
	linZ.clear();
	messageLines tempLine;
	
	// start loop through the words, adding the width of each plus space and calculate if need new line
	int spaceWidth = myFont->stringWidth(" ");
	int lineLen = 0;
	int tLines = 0;
	
	
	for( int i = 0; i < wrds.size(); i++)
	{
		int wordLen = spaceWidth + myFont->stringWidth(wrds[i]);
		if( tempLine.words.length() > 0 ) lineLen = myFont->stringWidth(tempLine.words);
		
		if( (lineLen+wordLen) >= maxLineWidth )
		{
			
			if( tempLine.words.length() > 0 )
			{
                tempLine.width =  myFont->stringWidth(tempLine.words);
                tempLine.height = lineSpace *  myFont->stringHeight(tempLine.words);
                linZ.push_back(tempLine);
                
                // add lines
                tempLine.words.clear();
                tLines++;
			}
			
			// start with this word
			lineLen =  myFont->stringWidth(wrds[i]);
			tempLine.words += wrds[i];
			
		}
        else{
			lineLen+=wordLen;
			if(tempLine.words.length() > 0) tempLine.words += " ";
			tempLine.words += wrds[i];
		}
	} 
	
	if(tempLine.words.size() > 0 )
	{
		tempLine.width =  myFont->stringWidth(tempLine.words);
		tempLine.height = lineSpace *  myFont->stringHeight(tempLine.words);
		linZ.push_back(tempLine);
	}
	else if( tLines == 0 && wrds.size() > 0 )
	{
		tempLine.width =  myFont->stringWidth(tempLine.words);
		tempLine.height =  myFont->stringHeight(tempLine.words);
		linZ.push_back(tempLine);
		
	}
	
	//height = 0;
	
	for( int i = 0; i < linZ.size(); i++)
	{
		//height += linZ[i].height;
		//if( i == 0 || linZ[i].height > lineHeight ) lineHeight = linZ[i].height;
	}
	
	height = 0;
	
	for( int i = 0; i < linZ.size(); i++)
	{
		height += lineHeight;
	}	
    
	return height <= textBoxHeight;
}


ofTrueTypeFont * TxtDisplay::getFontFromSize(int size)
{

    return fonts[size];
    
    /*
	switch (size) {
		case 22: return &font36; break;
		case 20: return &font24; break;
		case 18: return &font18; break;
		case 16: return &font16; break;
		case 14: return &font14; break;
		default: return &font24; break;
	}
	*/
	//return &font24;
}


