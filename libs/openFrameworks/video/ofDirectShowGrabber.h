#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofTypes.h"
#include "ofBaseVideoGrabber.h"

#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
	#include "videoInput.h"
#endif


class ofDirectShowGrabber : public ofBaseVideoGrabber{

	public :

		ofDirectShowGrabber();
		virtual ~ofDirectShowGrabber();

		bool			initGrabber(int w, int h, bool bTexture = true);
		void			grabFrame();
		
		void 			listDevices();
		void			close();
		void			videoSettings();

	protected:
		//--------------------------------- directshow
		#ifdef OF_VIDEO_CAPTURE_DIRECTSHOW
			int 					device;
			videoInput 				VI;
			bool 					bDoWeNeedToResize;
		#endif	
};

