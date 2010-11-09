#include "ofGStreamerGrabber.h"
#include "ofUtils.h"

//--------------------------------------------------------------------
ofGStreamerGrabber::ofGStreamerGrabber(){

}


//--------------------------------------------------------------------
ofGStreamerGrabber::~ofGStreamerGrabber(){
	close();
}

//--------------------------------------------------------------------
bool ofGStreamerGrabber::initGrabber(int w, int h, bool setUseTexture){

	bUseTexture = setUseTexture;
	
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------
		gstUtils.setDeviceID(deviceID);

		if(gstUtils.initGrabber(w,h,attemptFramerate)){
			width 	= w;
			height 	= h;
			if (bUseTexture){
				// create the texture, set the pixels to black and
				// upload them to the texture (so at least we see nothing black the callback)
				tex.allocate(width,height,GL_RGB);
				tex.loadData(gstUtils.getPixels(), width, height, GL_RGB);
			}
			bGrabberInited = true;
			ofLog(OF_LOG_VERBOSE, "ofVideoGrabber: initied");
		}else{
			bGrabberInited = false;
			ofLog(OF_LOG_ERROR, "ofVideoGrabber: couldn't init");
		}
		return bGrabberInited;


	//---------------------------------
	#endif
	//---------------------------------
}

//--------------------------------------------------------------------
void ofGStreamerGrabber::listDevices(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------
		gstUtils.listDevices();
	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
unsigned char * ofGStreamerGrabber::getPixels(){
	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------
		return gstUtils.getPixels();
	//---------------------------------
	#endif
	//---------------------------------
}

//--------------------------------------------------------------------
void ofGStreamerGrabber::grabFrame(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------

		if (bGrabberInited){
			gstUtils.update();
			bIsFrameNew = gstUtils.isFrameNew();
			if(bIsFrameNew) {
				if (bUseTexture){
					tex.loadData(gstUtils.getPixels(), width, height, GL_RGB);
				}
			}
		}

	//---------------------------------
	#endif
	//---------------------------------

}

//--------------------------------------------------------------------
void ofGStreamerGrabber::close(){

	//---------------------------------
	#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	//--------------------------------

		gstUtils.close();

	//---------------------------------
	#endif
	//---------------------------------
	
	ofBaseVideoGrabber::clearMemory();
	
}
