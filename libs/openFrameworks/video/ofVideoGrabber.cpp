#include "ofVideoGrabber.h"
#include "ofUtils.h"

//--------------------------------------------------------------------
ofVideoGrabber::ofVideoGrabber(){
	grabber = NULL;
}

//--------------------------------------------------------------------
ofVideoGrabber::~ofVideoGrabber(){
	if(	grabber != NULL ){
		delete grabber;
		grabber = NULL;
	}	
}

//--------------------------------------------------------------------
bool ofVideoGrabber::setGrabber(ofBaseVideoGrabber * newGrabber){
	if( grabber == NULL ){
		grabber = newGrabber;
		return true;
	}else{
		//TODO: should we delete grabberPtr? This is why we need smart pointers. 
		ofLog(OF_LOG_ERROR, "ofVideoGrabber::setGrabber - grabber already set!");
	}
	return false;
}

//--------------------------------------------------------------------
ofBaseVideoGrabber * ofVideoGrabber::getGrabber(){
	return grabber;
}

//--------------------------------------------------------------------
bool ofVideoGrabber::initGrabber(int w, int h, bool setUseTexture){
	
	if( grabber == NULL ){
		setGrabber( new OF_VID_GRABBER_TYPE );
	}

	bool bOk = grabber->initGrabber(w, h, setUseTexture);
	width	= grabber->getWidth();
	height	= grabber->getWidth();	
	return bOk;	
}

//--------------------------------------------------------------------
void ofVideoGrabber::listDevices(){
	if(	grabber != NULL ){
		grabber->listDevices();
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::setVerbose(bool bTalkToMe){
	if(	grabber != NULL ){
		grabber->setVerbose(bTalkToMe);
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::setDeviceID(int _deviceID){
	if(	grabber != NULL ){
		grabber->setDeviceID(_deviceID);
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::setDesiredFrameRate(int framerate){
	if(	grabber != NULL ){
		grabber->setDesiredFrameRate(framerate);
	}
}

//---------------------------------------------------------------------------
unsigned char * ofVideoGrabber::getPixels(){
	if(	grabber != NULL ){
		return grabber->getPixels();
	}
	return NULL;
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofVideoGrabber::getTextureReference(){
	if(	grabber != NULL ){
		return grabber->getTextureReference();
	}
}

//---------------------------------------------------------------------------
bool  ofVideoGrabber::isFrameNew(){
	if(	grabber != NULL ){
		return grabber->isFrameNew();
	}
	return false;
}

//--------------------------------------------------------------------
void ofVideoGrabber::update(){
	if(	grabber != NULL ){
		grabber->update();
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::grabFrame(){
	if(	grabber != NULL ){
		grabber->grabFrame();
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::close(){
	if(	grabber != NULL ){
		grabber->close();
	}
}

//--------------------------------------------------------------------
void ofVideoGrabber::videoSettings(void){
	if(	grabber != NULL ){
		grabber->videoSettings();
	}
}

//------------------------------------
void ofVideoGrabber::setUseTexture(bool bUse){
	if(	grabber != NULL ){
		grabber->setUseTexture(bUse);
	}	
}


//----------------------------------------------------------
void ofVideoGrabber::setAnchorPercent(float xPct, float yPct){
	if(	grabber != NULL ){
		grabber->setAnchorPercent(xPct, yPct);
	}
}

//----------------------------------------------------------
void ofVideoGrabber::setAnchorPoint(int x, int y){
   	if(	grabber != NULL ){
		grabber->setAnchorPoint(x, y);
	}
}

//----------------------------------------------------------
void ofVideoGrabber::resetAnchor(){
   	if(	grabber != NULL ){
		grabber->resetAnchor();
	}
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y, float _w, float _h){
   	if(	grabber != NULL ){
		grabber->draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofVideoGrabber::draw(float _x, float _y){
   	if(	grabber != NULL ){
		grabber->draw(_x, _y);
	}
}

//----------------------------------------------------------
float ofVideoGrabber::getHeight(){
	if(	grabber != NULL ){
		height = grabber->getHeight();
	}
	return (float)height;
}

//----------------------------------------------------------
float ofVideoGrabber::getWidth(){
	if(	grabber != NULL ){
		width = grabber->getWidth();
	}
	return (float)width;
}
