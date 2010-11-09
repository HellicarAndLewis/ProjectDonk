#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofBaseVideoPlayer.h"

#ifdef OF_VIDEO_PLAYER_QUICKTIME
	#include "ofQtUtils.h"
#endif


class ofQuickTimePlayer : public ofBaseVideoPlayer{

	public:

		ofQuickTimePlayer();
		~ofQuickTimePlayer();

		 bool			loadMovie(string name);
		 void			closeMovie();	
		 void			idleMovie();

		 void			play();
		 void			stop();

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

		 void			firstFrame();
		 void			nextFrame();
		 void			previousFrame();
				
	protected:
		void createImgMemAndGWorld();
		void start();
		
		//--------------------------------------
		#ifdef OF_VIDEO_PLAYER_QUICKTIME
		//--------------------------------------
			MovieDrawingCompleteUPP myDrawCompleteProc;
			MovieController  	thePlayer;
			GWorldPtr 			offscreenGWorld;
			Movie 			 	moviePtr;
			unsigned char * 	offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)
			void				qtGetFrameCount(Movie & movForcount);
		//--------------------------------------
		#endif
		//--------------------------------------

};





