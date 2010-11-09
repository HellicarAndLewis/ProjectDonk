#include "ofBaseVideoPlayer.h"
#include "ofUtils.h"

//---------------------------------------------------------------------------
ofBaseVideoPlayer::ofBaseVideoPlayer (){

	bLoaded 					= false;
	width 						= 0;
	height						= 0;
	speed 						= 1;
	bUseTexture					= true;
	bStarted					= false;
	pixels						= NULL;
	nFrames						= 0;
	bPaused						= true;
	currentLoopState			= OF_LOOP_NORMAL;

}

//---------------------------------------------------------------------------
ofBaseVideoPlayer::~ofBaseVideoPlayer(){
	//up to parent to close movie
	clearMemory();
}

//---------------------------------------------------------------------------
unsigned char * ofBaseVideoPlayer::getPixels(){
	return pixels;	
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofBaseVideoPlayer::getTextureReference(){
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofBaseVideoPlayer - getTextureReference - texture is not allocated");
	}
	return tex;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::update(){
	idleMovie();
}

//---------------------------------------------------------------------------
bool ofBaseVideoPlayer::isFrameNew(){
	return bIsFrameNew;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::close(){
	closeMovie();
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::clearMemory(){

	if( tex.bAllocated() ){
		tex.clear();
	}
	if(pixels){
		delete[] pixels;
		pixels = NULL;
	}

}

//--------------------------------------------------------
void ofBaseVideoPlayer::setVolume(int volume){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return;
	}
	
}

//--------------------------------------------------------
void ofBaseVideoPlayer::setLoopState(int state){

	//store the current loop state;
	currentLoopState = state;
}

//---------------------------------------------------------------------------
int ofBaseVideoPlayer::getLoopState(){
	return currentLoopState;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setPosition(float pct){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return;
	}

}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setFrame(int frame){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return;
	}

}


//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getDuration(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return 0.0;
	}
	
	return 0.0;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getPosition(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return 0.0;
	}
	
	return 0.0;
}

//---------------------------------------------------------------------------
int ofBaseVideoPlayer::getCurrentFrame(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return 0;
	}
	
	return 0;
}


//---------------------------------------------------------------------------
bool ofBaseVideoPlayer::getIsMovieDone(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return false;
	}
	
	return false;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::firstFrame(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return;
	}
	
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::nextFrame(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return;
	}
	
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::previousFrame(){
	if( !isLoaded() ){
		ofLog(OF_LOG_ERROR, "ofBaseVideoPlayer: movie not loaded!");
		return;
	}
	
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setSpeed(float _speed){
	speed 				= _speed;
}

//---------------------------------------------------------------------------
float ofBaseVideoPlayer::getSpeed(){
	return speed;
}

//---------------------------------------------------------------------------
void ofBaseVideoPlayer::setPaused(bool _bPause){
	bPaused = _bPause;
}

//------------------------------------
void ofBaseVideoPlayer::setUseTexture(bool bUse){
	bUseTexture = bUse;
}

//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
void ofBaseVideoPlayer::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofBaseVideoPlayer::setAnchorPoint(int x, int y){
    if (bUseTexture)tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofBaseVideoPlayer::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}

//------------------------------------
void ofBaseVideoPlayer::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofBaseVideoPlayer::draw(float _x, float _y){
	draw(_x, _y, (float)width, (float)height);
}

//------------------------------------
int ofBaseVideoPlayer::getTotalNumFrames(){
	return 0;
}

//----------------------------------------------------------
float ofBaseVideoPlayer::getHeight(){
	return (float)height;
}

//----------------------------------------------------------
float ofBaseVideoPlayer::getWidth(){
	return (float)width;
}

//----------------------------------------------------------
bool ofBaseVideoPlayer::isPaused(){
	return bPaused;
}

//----------------------------------------------------------
bool ofBaseVideoPlayer::isLoaded(){
	return bLoaded;
}

//----------------------------------------------------------
bool ofBaseVideoPlayer::isPlaying(){
	return bPlaying;
}
