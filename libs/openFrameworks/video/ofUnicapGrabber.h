#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofTypes.h"
#include "ofBaseVideoGrabber.h"

#ifdef OF_VIDEO_CAPTURE_UNICAP
	#include "ofUCUtils.h"
#endif

class ofUnicapGrabber : public ofBaseVideoGrabber{

	public :

		ofUnicapGrabber();
		virtual ~ofUnicapGrabber();

		bool			initGrabber(int w, int h, bool bTexture = true);
		void			grabFrame();
		
		void 			listDevices();
		void			close();
		
		void			videoSettings();
		
		void			setVerbose(bool bTalkToMe);

	protected:		
		//--------------------------------- linux unicap
		#ifdef OF_VIDEO_CAPTURE_UNICAP
			ofUCUtils	ucGrabber;
		#endif
};

