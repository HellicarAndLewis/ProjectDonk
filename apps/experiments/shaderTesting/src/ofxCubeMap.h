/*
 *  Created by David Wicks on 2/27/09.
 *  Copyright 2009 The Barbarian Group. All rights reserved.
 *
 *  Ported to oF by brenfer on 7/2/10.
 *  Copyright 2010 Rockwell Group. All rights reserved.
 *
 *  Updated for of 007 and extended by James George 2/13/11
 *  Copyright 2011 FlightPhase. All rights reserved.
 *
 */

#ifndef	CUBE_MAP_H
#define CUBE_MAP_H

#include "ofMain.h"

class ofxCubeMap
{
public:
	
	unsigned int textureObject;	
	
	
	
	//these should all be the same size
	
	void loadImages( string pos_x, string pos_y, string pos_z, string neg_x,string neg_y,string neg_z)
	{	
		ofImage images[6];	
		bool loaded1 = images[0].loadImage(pos_x);
		bool loaded2 = images[1].loadImage(neg_x);
		bool loaded3 = images[2].loadImage(pos_y);
		bool loaded4 = images[3].loadImage(neg_y);
		bool loaded5 = images[4].loadImage(pos_z);
		bool loaded6 = images[5].loadImage(neg_z);
		

		loadFromOfImages(images[0],
						 images[2],
						 images[4],
						 images[1],
						 images[3],
						 images[5]);
		

/*
 //JG removed redundancy with overloaded call

		//create a texture object
		glGenTextures(1, &textureObject);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, textureObject);
		//assign the images to positions
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, images[0].getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)images[0].getTextureReference().getTextureData().tex_w, (GLint)images[0].getTextureReference().getTextureData().tex_h, 0, 
					 images[0].getTextureReference().getTextureData().glType, images[0].getTextureReference().getTextureData().pixelType, 
					 images[0].getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, images[1].getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)images[1].getTextureReference().getTextureData().tex_w, (GLint)images[1].getTextureReference().getTextureData().tex_h, 0, 
					 images[1].getTextureReference().getTextureData().glType, images[1].getTextureReference().getTextureData().pixelType, 
					 images[1].getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, images[2].getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)images[2].getTextureReference().getTextureData().tex_w, (GLint)images[2].getTextureReference().getTextureData().tex_h, 0, 
					 images[2].getTextureReference().getTextureData().glType, images[2].getTextureReference().getTextureData().pixelType, 
					 images[2].getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, images[3].getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)images[3].getTextureReference().getTextureData().tex_w, (GLint)images[3].getTextureReference().getTextureData().tex_h, 0, 
					 images[3].getTextureReference().getTextureData().glType, images[3].getTextureReference().getTextureData().pixelType, 
					 images[3].getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, images[4].getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)images[4].getTextureReference().getTextureData().tex_w, (GLint)images[4].getTextureReference().getTextureData().tex_h, 0, 
					 images[4].getTextureReference().getTextureData().glType, images[4].getTextureReference().getTextureData().pixelType, 
					 images[4].getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, images[5].getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)images[5].getTextureReference().getTextureData().tex_w, (GLint)images[5].getTextureReference().getTextureData().tex_h, 0, 
					 images[5].getTextureReference().getTextureData().glType, images[5].getTextureReference().getTextureData().pixelType, 
					 images[5].getPixels());
		//set filtering modes for scaling up and down
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 */
		
	}
	
	/* glTexImage2D(
	 cubefaces[i],
	 0,                  // level
	 GL_RGBA8,          // internal format
	 CUBE_MAP_SIZE,     // width
	 CUBE_MAP_SIZE,     // height
	 0,                 // border
	 GL_RGBA,           // format
	 GL_UNSIGNED_BYTE,   // type
	 CubeMap[CUBE_POS_X + i]); // pixel data
	 }*/
	 
	 void loadFromOfImages( ofImage pos_x, ofImage pos_y, ofImage pos_z, ofImage neg_x,ofImage neg_y,ofImage neg_z)
	{	
		//create a texture object
		glGenTextures(1, &textureObject);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, textureObject);
		//assign the images to positions
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, pos_x.getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)pos_x.getTextureReference().getTextureData().tex_w, (GLint)pos_x.getTextureReference().getTextureData().tex_h, 0, 
					 pos_x.getTextureReference().getTextureData().glType, pos_x.getTextureReference().getTextureData().pixelType, 
					 pos_x.getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, neg_x.getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)neg_x.getTextureReference().getTextureData().tex_w, (GLint)neg_x.getTextureReference().getTextureData().tex_h, 0, 
					 neg_x.getTextureReference().getTextureData().glType, neg_x.getTextureReference().getTextureData().pixelType, 
					 neg_x.getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, pos_y.getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)pos_y.getTextureReference().getTextureData().tex_w, (GLint)pos_y.getTextureReference().getTextureData().tex_h, 0, 
					 pos_y.getTextureReference().getTextureData().glType, pos_y.getTextureReference().getTextureData().pixelType, 
					 pos_y.getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, neg_y.getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)neg_y.getTextureReference().getTextureData().tex_w, (GLint)neg_y.getTextureReference().getTextureData().tex_h, 0, 
					 neg_y.getTextureReference().getTextureData().glType, neg_y.getTextureReference().getTextureData().pixelType, 
					 neg_y.getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, pos_z.getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)pos_z.getTextureReference().getTextureData().tex_w, (GLint)pos_z.getTextureReference().getTextureData().tex_h, 0, 
					 pos_z.getTextureReference().getTextureData().glType, pos_z.getTextureReference().getTextureData().pixelType, 
					 pos_z.getPixels());
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, neg_z.getTextureReference().getTextureData().glTypeInternal, 
					 (GLint)neg_z.getTextureReference().getTextureData().tex_w, (GLint)neg_z.getTextureReference().getTextureData().tex_h, 0, 
					 neg_z.getTextureReference().getTextureData().glType, neg_z.getTextureReference().getTextureData().pixelType, 
					 neg_z.getPixels());
		//set filtering modes for scaling up and down
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
	}

	void bindMulti( int pos )
	{
		glActiveTexture(GL_TEXTURE0 + pos );
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, textureObject);
	}

	void bind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, textureObject);
	}

	void unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, 0 );
	}

	void enableFixedMapping()
	{
		glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
		glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
		glTexGeni( GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP );
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);
		glEnable( GL_TEXTURE_CUBE_MAP );
	}

	void disableFixedMapping()
	{
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable( GL_TEXTURE_CUBE_MAP );
	}
};

#endif
