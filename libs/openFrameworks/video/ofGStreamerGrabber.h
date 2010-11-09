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

		bool			initGrabber(int w, int h, bool bTexture = true);
		void			grabFrame();
		
		void 			listDevices();
		void			close();

		unsigned char 	* getPixels();

	protected:
		#ifdef OF_VIDEO_CAPTURE_GSTREAMER
			ofGstUtils				gstUtils;
		#endif
};

