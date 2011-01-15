/*
 * ofPixels.cpp
 *
 *  Created on: 13/01/2011
 *      Author: arturo
 */

#include "ofPixels.h"

ofPixels::Data::Data(){
	pixels			= NULL;
	width			= 0;
	height			= 0;
	bytesPerPixel	= 0;
	bitsPerPixel	= 0;
	bAllocated		= false;
	glDataType		= GL_LUMINANCE;
	imageType		= OF_IMAGE_UNDEFINED;

}

ofPixels::Data::~Data(){
	if(pixels){
		delete[] pixels;
		pixels = NULL;
	}
}

ofPixels::ofPixels(): data(new Data){

}

void ofPixels::convertToCopy(){
	Data * newData = new Data(*data.get());
	Data * oldData = data.get();
	data.assign(newData);
	if(data->bAllocated){
		data->bAllocated=false;
		data->pixels=NULL;
		setFromPixels(oldData->pixels,data->width,data->height,data->imageType);
	}
}

/*ofPixels::ofPixels(ofPixels & mom,bool reference): data(reference?mom.data.get():new Data()){
	if(!reference && mom.isAllocated()){
		allocate(mom.getWidth(),mom.getHeight(),mom.getImageType());
		memcpy(data->pixels,mom.getPixels(),mom.getWidth()*mom.getHeight()*mom.getBytesPerPixel());
	}
}


void ofPixels::operator=(const ofPixels & mom){
	if(!mom.reference && mom.isAllocated()){
		allocate(mom.getWidth(),mom.getHeight(),mom.getImageType());
		memcpy(data->pixels,mom.getPixels(),mom.getWidth()*mom.getHeight()*mom.getBytesPerPixel());
	}else if(mom.reference){
		data.assign(mom.data.get());
	}
}*/

void ofPixels::allocate(int w, int h, int bitsPerPixel){
	ofImageType type;
	switch(bitsPerPixel){
	case 8:
		type=OF_IMAGE_GRAYSCALE;
		break;
	case 24:
		type=OF_IMAGE_COLOR;
		break;
	case 32:
		type=OF_IMAGE_COLOR_ALPHA;
		break;
	default:
		ofLog(OF_LOG_ERROR,"%i bits per pixel is not a supported image type", bitsPerPixel);
		return;
	}
	allocate(w,h,type);
}

void ofPixels::allocate(int w, int h, ofImageType type){

	//we check if we are already allocated at the right size
	if(data->bAllocated && w==data->width && h==data->height && type==data->imageType){
		return; //we don't need to allocate
	}

	//we do need to allocate, clear the data
	clear();

	data->imageType = type;
	data->width= w;
	data->height = h;
	switch(data->imageType){
	case OF_IMAGE_GRAYSCALE:
		data->bytesPerPixel = 1;
		data->glDataType = GL_LUMINANCE;
		break;
	case OF_IMAGE_COLOR:
		data->bytesPerPixel = 3;
		data->glDataType = GL_RGB;
		break;
	case OF_IMAGE_COLOR_ALPHA:
		data->bytesPerPixel = 4;
		data->glDataType = GL_RGBA;
		break;
	}

	data->bitsPerPixel = data->bytesPerPixel * 8;
	data->pixels = new unsigned char[w*h*data->bytesPerPixel];
	data->bAllocated = true;

}

void ofPixels::set(unsigned char val){
	memset(data->pixels,val,data->width*data->height*data->bytesPerPixel);
}

void ofPixels::setFromPixels(unsigned char * newPixels,int w, int h, ofImageType newType){
	allocate(w,h,newType);
	memcpy(data->pixels,newPixels,w*h*data->bytesPerPixel);
}


void ofPixels::swapRgb(){
	if (data->bitsPerPixel != 8){
		int sizePixels		= data->width*data->height;
		int cnt				= 0;
		unsigned char * pixels_ptr = data->pixels;

		while (cnt < sizePixels){
			std::swap(pixels_ptr[0],pixels_ptr[2]);
			cnt++;
			pixels_ptr+=data->bytesPerPixel;
		}
	}
}

void ofPixels::clear(){
	data.assign(new Data());
}

unsigned char * ofPixels::getPixels(){
	return data->pixels;
}

const unsigned char * ofPixels::getPixels() const{
	return data->pixels;
}

int ofPixels::getPixelIndex(int x, int y){
	if( !data->bAllocated ){
		return 0;
	}else{
		return ( x + y * data->width ) * data->bytesPerPixel;
	}
}

ofColor ofPixels::getPixel(int x, int y){
	ofColor c;
	int index = getPixelIndex(x, y);

	if( data->bytesPerPixel == 1 ){
		c.set( data->pixels[index] );
	}else if( data->bytesPerPixel == 3 ){
		c.set( data->pixels[index], data->pixels[index+1], data->pixels[index+2] );
	}else if( data->bytesPerPixel == 4 ){
		c.set( data->pixels[index], data->pixels[index+1], data->pixels[index+2], data->pixels[index+3] );
	}

	return c;
}

unsigned char & ofPixels::operator[](int pos){
	return data->pixels[pos];
}

unsigned char & ofPixels::at(int pos){
	return data->pixels[pos];
}

bool ofPixels::isAllocated() const{
	return data->bAllocated;
}

int ofPixels::getWidth() const{
	return data->width;
}

int ofPixels::getHeight() const{
	return data->height;
}

int ofPixels::getBytesPerPixel() const{
	return data->bytesPerPixel;
}

int ofPixels::getBitsPerPixel() const{
	return data->bitsPerPixel;
}

ofImageType ofPixels::getImageType() const{
	return data->imageType;
}

int ofPixels::getGlDataType() const{
	return data->glDataType;
}

