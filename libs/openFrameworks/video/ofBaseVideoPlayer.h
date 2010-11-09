#pragma once
#include "ofConstants.h"
#include "ofTexture.h"


#ifdef OF_VIDEO_PLAYER_GSTREAMER
	#include "ofGstUtils.h"
#else
	#include "ofQtUtils.h"
#endif


//---------------------------------------------

class ofBaseVideoPlayer : public ofBaseVideo{

	public:

		ofBaseVideoPlayer();
		virtual ~ofBaseVideoPlayer();

		//needs implementing
		virtual bool				loadMovie(string name) = 0;
		virtual void				closeMovie() = 0; //same as close
		virtual void				idleMovie() = 0;

		//won't neeed implementing
		void						close();
		void						update();

		//might need implementing
		virtual void				play() = 0;
		virtual void				stop() = 0;
				
		virtual bool 				isFrameNew();
		virtual unsigned char * 	getPixels();
		virtual float 				getPosition();
		virtual float 				getSpeed();
		virtual float 				getDuration();
		virtual bool				getIsMovieDone();

		virtual void 				setPosition(float pct);
		virtual void 				setVolume(int volume);
		virtual void 				setLoopState(int state);
		virtual void   				setSpeed(float speed);
		virtual void				setFrame(int frame);  // frame 0 = first frame...

		virtual void 				setUseTexture(bool bUse);
		virtual ofTexture &			getTextureReference();
		virtual void 				draw(float x, float y, float w, float h);
		virtual void 				draw(float x, float y);

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        virtual void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        virtual void				setAnchorPoint(int x, int y);				//set the anchor point in pixels
       virtual  void				resetAnchor();								//resets the anchor to (0, 0)

		virtual void 				setPaused(bool bPause);

		virtual int					getCurrentFrame();
		virtual int					getTotalNumFrames();
		virtual int					getLoopState();

		virtual void				firstFrame();
		virtual void				nextFrame();
		virtual void				previousFrame();

		virtual float 				getHeight();
		virtual float 				getWidth();

		virtual bool				isPaused();
		virtual bool				isLoaded();
		virtual bool				isPlaying();
		
		int							width, height;
		bool						bLoaded;

		//these are public because the ofQuickTimePlayer implementation has some callback functions that need access
		//todo - fix this
		bool 				bUseTexture;			
		unsigned char * 	pixels;				
		bool 				bHavePixelsChanged;
		ofTexture 			tex;					// a ptr to the texture we are utilizing

	protected:
		virtual void		clearMemory();

		int					nFrames;				// number of frames
		bool				allocated;				// so we know to free pixels or not
		int					currentLoopState;
		bool 				bStarted;
		bool 				bPlaying;
		bool 				bPaused;
		bool 				bIsFrameNew;			// if we are new
		float				speed;

};






