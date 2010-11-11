#pragma once

#include "ofConstants.h"

class ofBaseVideoGrabber{

	public :

		ofBaseVideoGrabber(){
		
		}
		
		virtual ~ofBaseVideoGrabber(){
		
		}

		//needs implementing
		virtual void			listDevices() = 0;		
		virtual bool			initGrabber(int w, int h) = 0;		
		virtual void			grabFrame() = 0;
		virtual unsigned char 	* getPixels() = 0;

		virtual bool			isFrameNew() = 0;

		virtual void			close() = 0;	
		virtual void			clearMemory() = 0;
		
		virtual float getHeight() = 0;
		virtual float getWidth() = 0;
		
		//might need implementing
		virtual void setVerbose(bool bTalkToMe){
			ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setVerbose not implemented");
		}

		virtual void setDeviceID(int _deviceID){
			ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setDeviceID not implemented");
		}

		virtual void setDesiredFrameRate(int framerate){
			ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::setDesiredFrameRate not implemented");
		}

		virtual void videoSettings(){
			ofLog(OF_LOG_WARNING, "ofBaseVideoGrabber::videoSettings not implemented");
		}

	protected:
	
};

