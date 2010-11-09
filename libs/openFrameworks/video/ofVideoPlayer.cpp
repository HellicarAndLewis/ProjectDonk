#include "ofVideoPlayer.h"
#include "ofUtils.h"

//---------------------------------------------------------------------------
ofVideoPlayer::ofVideoPlayer (){
	player						= NULL;
}

//---------------------------------------------------------------------------
ofVideoPlayer::~ofVideoPlayer(){
	if( player != NULL ){
		delete player;
		player = NULL;
	}
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::setPlayer(ofBaseVideoPlayer * newPlayer){
	if( player == NULL ){
		player = newPlayer;
		return true;
	}else{
		//TODO: should we delete newPlayer? This is why we need smart pointers. 
		ofLog(OF_LOG_ERROR, "ofVideoPlayer::setPlayer - player already set!");
	}
	
	return false;
}

//--------------------------------------------------------------------
ofBaseVideoPlayer * ofVideoPlayer::getPlayer(){
	return player;
}

//---------------------------------------------------------------------------
bool ofVideoPlayer::loadMovie(string name){
	if( player == NULL ){
		setPlayer( new OF_VID_PLAYER_TYPE );
	}
	
	bool bOk = player->loadMovie(name);
	width	 = player->getWidth();
	height	 = player->getHeight();
	
	return bOk;
}

//---------------------------------------------------------------------------
unsigned char * ofVideoPlayer::getPixels(){
	if( player != NULL ){		
		return player->getPixels();
	}
}

//---------------------------------------------------------------------------
//for getting a reference to the texture
ofTexture & ofVideoPlayer::getTextureReference(){
	if( player != NULL ){	
		return player->getTextureReference();
	}
	
	//TODO: fix this! create the player automatically?
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::isFrameNew(){
	if( player != NULL ){
		return player->isFrameNew();
	}
	
	return false;
}

//--------------------------------------------------------------------
void ofVideoPlayer::update(){
	if( player != NULL ){	
		player->update();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::idleMovie(){
	if( player != NULL ){	
		player->idleMovie();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::closeMovie(){
	if( player != NULL ){
		player->closeMovie();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::close(){
	if( player != NULL ){
		player->close();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::play(){
	if( player != NULL ){
		player->play();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::stop(){
	if( player != NULL ){
		player->stop();
	}
}

//--------------------------------------------------------
void ofVideoPlayer::setVolume(int volume){
	if( player != NULL ){
		player->setVolume(volume);
	}
}


//--------------------------------------------------------
void ofVideoPlayer::setLoopState(int state){
	if( player != NULL ){
		player->setLoopState(state);
	}
}


//---------------------------------------------------------------------------
void ofVideoPlayer::setPosition(float pct){
	if( player != NULL ){
		player->setPosition(pct);
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setFrame(int frame){
	if( player != NULL ){
		player->setFrame(frame);
	}
}


//---------------------------------------------------------------------------
float ofVideoPlayer::getDuration(){
	if( player != NULL ){
		return player->getDuration();
	}
	
	return 0.0;
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getPosition(){
	if( player != NULL ){
		return player->getPosition();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
int ofVideoPlayer::getCurrentFrame(){
	if( player != NULL ){
		return player->getCurrentFrame();
	}
	return 0;
}


//---------------------------------------------------------------------------
bool ofVideoPlayer::getIsMovieDone(){
	if( player != NULL ){
		return player->getIsMovieDone();
	}
	return false;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::firstFrame(){
	if( player != NULL ){
		player->firstFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::nextFrame(){
	if( player != NULL ){
		player->nextFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::previousFrame(){
	if( player != NULL ){
		player->previousFrame();
	}
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setSpeed(float _speed){
	if( player != NULL ){
		player->setSpeed(_speed);
	}
}

//---------------------------------------------------------------------------
float ofVideoPlayer::getSpeed(){
	if( player != NULL ){
		return player->getSpeed();
	}
	return 0.0;
}

//---------------------------------------------------------------------------
void ofVideoPlayer::setPaused(bool _bPause){
	if( player != NULL ){
		player->setPaused(_bPause);
	}
}

//------------------------------------
void ofVideoPlayer::setUseTexture(bool bUse){
	if( player != NULL ){
		player->setUseTexture(bUse);
	}
}

//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPercent(float xPct, float yPct){
	if( player != NULL ){
		player->setAnchorPercent(xPct, yPct);
	}
}

//----------------------------------------------------------
void ofVideoPlayer::setAnchorPoint(int x, int y){
	if( player != NULL ){
		player->setAnchorPoint(x, y);
	}
}

//----------------------------------------------------------
void ofVideoPlayer::resetAnchor(){
	if( player != NULL ){
		player->resetAnchor();
	}
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y, float _w, float _h){
	if( player != NULL ){
		player->draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofVideoPlayer::draw(float _x, float _y){
	if( player != NULL ){
		player->draw(_x, _y);
	}
}

//------------------------------------
int ofVideoPlayer::getTotalNumFrames(){
	if( player != NULL ){
		return player->getTotalNumFrames();
	}
	return 0;
}

//----------------------------------------------------------
float ofVideoPlayer::getHeight(){
	if(	player != NULL ){
		height = player->getHeight();
	}
	return (float)height;
}

//----------------------------------------------------------
float ofVideoPlayer::getWidth(){
	if(	player != NULL ){
		width = player->getWidth();
	}
	return (float)width;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPaused(){
	if(	player != NULL ){
		return player->isPaused();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isLoaded(){
	if(	player != NULL ){
		return player->isLoaded();
	}
	return false;
}

//----------------------------------------------------------
bool ofVideoPlayer::isPlaying(){
	if(	player != NULL ){
		return player->isPlaying();
	}
	return false;
}
