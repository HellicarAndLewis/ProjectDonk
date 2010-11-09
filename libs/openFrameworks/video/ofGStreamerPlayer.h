#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseVideoPlayer.h"

#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include "ofGstUtils.h"
#endif

class ofGStreamerPlayer : public ofBaseVideoPlayer{

	public:

		ofGStreamerPlayer();
		~ofGStreamerPlayer();

		 bool			loadMovie(string name);
		 void			closeMovie();	
		 void			idleMovie();

		 void			play();
		 void			stop();
		 
		 unsigned char * getPixels();

		 float 			getPosition();
		 float 			getDuration();
		 bool			getIsMovieDone();

		 void 			setPosition(float pct);
		 void 			setVolume(int volume);
		 void 			setLoopState(int state);
		 void   		setSpeed(float speed);
		 void			setFrame(int frame);  // frame 0 = first frame...
		 void 			setPaused(bool bPause);

		 int			getCurrentFrame();
		 int			getTotalNumFrames();

		 void			firstFrame();
		 void			nextFrame();
		 void			previousFrame();
				
	protected:
		//--------------------------------------
		#ifdef OF_VIDEO_PLAYER_GSTREAMER
		//--------------------------------------
		ofGstUtils 			gstUtils;
		//--------------------------------------
		#endif
		//--------------------------------------


};





