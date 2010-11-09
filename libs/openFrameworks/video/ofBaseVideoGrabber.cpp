#include "ofBaseVideoGrabber.h"
#include "ofUtils.h"

//--------------------------------------------------------------------
ofBaseVideoGrabber::ofBaseVideoGrabber(){

	// common
	bIsFrameNew				= false;
	bVerbose 				= false;
	bGrabberInited 			= false;
	bUseTexture				= true;
	bChooseDevice			= false;
	deviceID				= 0;
	width 					= 320;	// default setting
	height 					= 240;	// default setting
	pixels					= NULL;
	attemptFramerate		= -1;
}


//--------------------------------------------------------------------
ofBaseVideoGrabber::~ofBaseVideoGrabber(){
	//up to parent to close device
	clearMemory();
}


//--------------------------------------------------------------------
void ofBaseVideoGrabber::listDevices(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::listDevices needs implementing");
}

//--------------------------------------------------------------------
void ofBaseVideoGrabber::setVerbose(bool bTalkToMe){
	bVerbose = bTalkToMe;
}

//--------------------------------------------------------------------
void ofBaseVideoGrabber::setDeviceID(int _deviceID){
	deviceID		= _deviceID;
	bChooseDevice	= true;
}

//--------------------------------------------------------------------
void ofBaseVideoGrabber::setDesiredFrameRate(int framerate){
	attemptFramerate = framerate;
}

//---------------------------------------------------------------------------
unsigned char * ofBaseVideoGrabber::getPixels(){
	return pixels;
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofBaseVideoGrabber::getTextureReference(){
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber - getTextureReference - texture is not allocated");
	}
	return tex;
}

//---------------------------------------------------------------------------
bool  ofBaseVideoGrabber::isFrameNew(){
	return bIsFrameNew;
}

//--------------------------------------------------------------------
void ofBaseVideoGrabber::update(){
	grabFrame();
}

//--------------------------------------------------------------------
void ofBaseVideoGrabber::clearMemory(){
	if (pixels != NULL){
		delete[] pixels;
		pixels = NULL;
	}

	tex.clear();
}

//--------------------------------------------------------------------
void ofBaseVideoGrabber::videoSettings(){
	ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::videoSettings needs implementing");
}

//------------------------------------
void ofBaseVideoGrabber::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
void ofBaseVideoGrabber::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofBaseVideoGrabber::setAnchorPoint(int x, int y){
    if (bUseTexture)tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofBaseVideoGrabber::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}

//------------------------------------
void ofBaseVideoGrabber::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofBaseVideoGrabber::draw(float _x, float _y){
	draw(_x, _y, (float)width, (float)height);
}


//----------------------------------------------------------
float ofBaseVideoGrabber::getHeight(){
	return (float)height;
}

//----------------------------------------------------------
float ofBaseVideoGrabber::getWidth(){
	return (float)width;
}

