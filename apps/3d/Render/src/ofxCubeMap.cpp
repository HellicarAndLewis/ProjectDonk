/*
 *  ofxCubeMap.cpp
 *  bubbleShaderTests
 *
 *  Created by base on 14/02/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "ofxCubeMap.h"


void ofxCubeMap::loadFromOfImages( ofImage pos_x, ofImage pos_y, ofImage pos_z, ofImage neg_x,ofImage neg_y,ofImage neg_z)
{	
	
	
	GLint currentActiveTex;
	glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTex);
	float currentCoords[4];
	glGetFloatv(GL_CURRENT_TEXTURE_COORDS, &currentCoords[0]);
	
	cout << currentActiveTex << " " << currentCoords[0] << " " << currentCoords[1] << " " << currentCoords[2] << " " << currentCoords[3] << " " << endl;

	//create a texture object
	glGenTextures(1, &textureObject);
	//glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, textureObject);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureObject);
	
	unsigned char * data_px, * data_nx, * data_py, * data_ny, * data_pz, * data_nz;
	
	const int img_size = pos_x.getWidth();
	
	data_px = new unsigned char [img_size * img_size * 3];
	data_nx = new unsigned char [img_size * img_size * 3];
	data_py = new unsigned char [img_size * img_size * 3];
	data_ny = new unsigned char [img_size * img_size * 3];
	data_pz = new unsigned char [img_size * img_size * 3];
	data_nz = new unsigned char [img_size * img_size * 3];
	
	data_px = pos_x.getPixels();
	data_nx = neg_x.getPixels();
	data_py = pos_y.getPixels();
	data_ny = neg_y.getPixels();
	data_pz = pos_z.getPixels();
	data_nz = neg_z.getPixels();
	
/*	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_px); // postive x
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_nx); // negative x
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_py); // postive y
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_ny); // negative y
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_nz); // negative z
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pz); // positive z*/
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_px); // postive x
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_nx); // negative x
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_py); // postive y
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_ny); // negative y
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_nz); // negative z
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, img_size, img_size, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pz); // positive z
	
/*	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// Set far filtering mode
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     // Set near filtering mode */
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// Set far filtering mode
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);     // Set near filtering mode 

	glDisable(GL_TEXTURE_CUBE_MAP);
	
	/*delete data_px;
	delete data_nx;
	delete data_py;
	delete data_ny;
	delete data_pz;
	delete data_nz;*/

	glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTex);
	glGetFloatv(GL_CURRENT_TEXTURE_COORDS, &currentCoords[0]);
	
	cout << currentActiveTex << " " << currentCoords[0] << " " << currentCoords[1] << " " << currentCoords[2] << " " << currentCoords[3] << " " << endl;

}

// this should probably be a PBO
// though, that'll have to wait a little

void ofxCubeMap::updateImages( ofImage pos_x, ofImage pos_y, ofImage pos_z, ofImage neg_x,ofImage neg_y,ofImage neg_z )
{
	
	unsigned char * data_px, * data_nx, * data_py, * data_ny, * data_pz, * data_nz;
	
	const int img_size = pos_x.getWidth();
	
	data_px = new unsigned char [img_size * img_size * 3];
	data_nx = new unsigned char [img_size * img_size * 3];
	data_py = new unsigned char [img_size * img_size * 3];
	data_ny = new unsigned char [img_size * img_size * 3];
	data_pz = new unsigned char [img_size * img_size * 3];
	data_nz = new unsigned char [img_size * img_size * 3];
	
	data_px = pos_x.getPixels();
	data_nx = neg_y.getPixels();
	data_py = pos_y.getPixels();
	data_ny = neg_y.getPixels();
	data_pz = pos_z.getPixels();
	data_nz = neg_z.getPixels();
	
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, textureObject);
	
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, 0, 0, img_size, img_size, GL_RGB, GL_UNSIGNED_BYTE, data_px);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, 0, 0, img_size, img_size, GL_RGB, GL_UNSIGNED_BYTE, data_py);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, 0, 0, img_size, img_size, GL_RGB, GL_UNSIGNED_BYTE, data_pz);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, 0, 0, img_size, img_size, GL_RGB, GL_UNSIGNED_BYTE, data_nx);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, 0, 0, img_size, img_size, GL_RGB, GL_UNSIGNED_BYTE, data_ny);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, 0, 0, img_size, img_size, GL_RGB, GL_UNSIGNED_BYTE, data_nz);
	
	delete data_px;
	delete data_nx;
	delete data_py;
	delete data_ny;
	delete data_pz;
	delete data_nz;
	
}