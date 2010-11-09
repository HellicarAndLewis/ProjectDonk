#pragma once

#include "ofConstants.h"
#include "ofTexture.h"
#include "ofGraphics.h"
#include "ofTypes.h"


class ofBaseVideoGrabber : public ofBaseVideo{

	public :

		ofBaseVideoGrabber();
		virtual ~ofBaseVideoGrabber();

		//needs implementing
		virtual bool	initGrabber(int w, int h, bool bTexture = true) = 0;		
		virtual void	grabFrame() = 0;
		virtual void	close() = 0;	
		
		//might need implementing
		virtual void		videoSettings();
		virtual unsigned char 	* getPixels();

		//shouldn't need implementing 
		virtual void			listDevices();
		virtual bool			isFrameNew();
		virtual ofTexture &		getTextureReference();
		virtual void 			setVerbose(bool bTalkToMe);
		virtual void			setDeviceID(int _deviceID);
		virtual void			setDesiredFrameRate(int framerate);
		virtual void 			setUseTexture(bool bUse);
		virtual void 			draw(float x, float y, float w, float h);
		virtual void 			draw(float x, float y);
		virtual void			update();

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
        virtual void			setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
        virtual void			setAnchorPoint(int x, int y);				//set the anchor point in pixels
        virtual void			resetAnchor();								//resets the anchor to (0, 0)

		virtual float 			getHeight();
		virtual float 			getWidth();

		int				height;
		int				width;

	protected:

		virtual void			clearMemory();

		bool					bChooseDevice;
		int						deviceID;
		bool					bUseTexture;
		ofTexture 				tex;
		bool 					bVerbose;
		bool 					bGrabberInited;
	    unsigned char * 		pixels;
		int						attemptFramerate;
		bool 					bIsFrameNew;
		
};

