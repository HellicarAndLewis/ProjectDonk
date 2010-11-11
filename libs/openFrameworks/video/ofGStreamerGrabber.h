#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofTypes.h"
#include "ofBaseVideoGrabber.h"

#ifdef OF_VIDEO_CAPTURE_GSTREAMER
	#include "ofGstUtils.h"
#endif


class ofGStreamerGrabber : public ofBaseVideoGrabber{

	public :

		ofGStreamerGrabber();
		virtual ~ofGStreamerGrabber();

		bool			initGrabber(int w, int h);
		void			grabFrame();
		
		void 			listDevices();
		void			close();

		unsigned char 	* getPixels();

		void			clearMemory();
		bool			isFrameNew();

		float			getWidth();
		float			getHeight();

		void			setVerbose(bool bTalkToMe);
		void			setDeviceID(int _deviceID);
		void			setDesiredFrameRate(int framerate);		

	protected:
	
		bool					bChooseDevice;
		int						deviceID;
		bool 					bVerbose;
		bool 					bGrabberInited;
		int						attemptFramerate;
		bool 					bIsFrameNew;	
		
		int						width, height;	
		#ifdef OF_VIDEO_CAPTURE_GSTREAMER
			ofGstUtils				gstUtils;
		#endif
};

