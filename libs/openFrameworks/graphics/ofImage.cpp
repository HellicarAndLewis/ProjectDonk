#include "ofImage.h"


//----------------------------------------------------------
// static variable for freeImage initialization:
static bool		bFreeImageInited = false;
//----------------------------------------------------------

void ofLoadImage(ofPixels & pix, string path){
	ofImage::loadImageIntoPixels(path, pix);
}

void ofLoadImageFromMemory(ofPixels & pix, unsigned char * bytes, int numBytes){
	ofImage::loadImageFromMemory(bytes, numBytes, pix);
}


//----------------------------------------------------------
ofImage::ofImage(){

	width						= 0;
	height						= 0;
	bpp							= 0;
	type						= OF_IMAGE_UNDEFINED;
	bUseTexture					= true;		// the default is, yes, use a texture

	//----------------------- init free image if necessary
	if (!bFreeImageInited){
		FreeImage_Initialise();
		bFreeImageInited = true;
	}
}

//----------------------------------------------------------
ofImage& ofImage::operator=(const ofImage& mom) {
	clone(mom);
	update();
	return *this;
}

//----------------------------------------------------------
ofImage::ofImage(const ofImage& mom) {

	if (!bFreeImageInited){
		FreeImage_Initialise();
		bFreeImageInited = true;
	}

	clear();
	clone(mom);
	update();
};

//----------------------------------------------------------
ofImage::~ofImage(){
	clear();
}

//----------------------------------------------------------
bool ofImage::loadImage(string fileName){
	bool bLoadedOk = false;
	bLoadedOk = loadImageIntoPixels(fileName, myPixels);

	if (bLoadedOk == true){
	if (myPixels.isAllocated() && bUseTexture){
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}
	update();
}

	return bLoadedOk;
}

//----------------------------------------------------------
void ofImage::saveImage(string fileName){
	saveImageFromPixels(fileName, myPixels);
}

//we could cap these values - but it might be more useful
//to be able to set anchor points outside the image

//----------------------------------------------------------
void ofImage::setAnchorPercent(float xPct, float yPct){
    if (bUseTexture)tex.setAnchorPercent(xPct, yPct);
}

//----------------------------------------------------------
void ofImage::setAnchorPoint(float x, float y){
    if (bUseTexture)tex.setAnchorPoint(x, y);
}

//----------------------------------------------------------
void ofImage::resetAnchor(){
   	if (bUseTexture)tex.resetAnchor();
}

//------------------------------------
void ofImage::draw(const ofRectangle & _r){
	if (bUseTexture){
		tex.draw(_r);
	}
}

//------------------------------------
void ofImage::draw(const ofPoint & _p, float _w, float _h){
	if (bUseTexture){
		tex.draw(_p, _w, _h);
	}
}

//------------------------------------
void ofImage::draw(float _x, float _y, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _w, _h);
	}
}

//------------------------------------
void ofImage::draw(float _x, float _y, float _z, float _w, float _h){
	if (bUseTexture){
		tex.draw(_x, _y, _z, _w, _h);
	}
}

//------------------------------------
void ofImage::draw(const ofPoint & p){
	draw(p.x,p.y,p.z,myPixels.getWidth(),myPixels.getHeight());
}

//------------------------------------
void ofImage::draw(float x, float y){
	draw(x,y,0.0f,myPixels.getWidth(),myPixels.getHeight());
}

//------------------------------------
void ofImage::draw(float x, float y, float z){
	draw(x,y,z,myPixels.getWidth(),myPixels.getHeight());
}

//------------------------------------
void ofImage::allocate(int w, int h, ofImageType type){

	myPixels.allocate(w, h, type);

	// take care of texture allocation --
	if (myPixels.isAllocated() && bUseTexture){
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}

	update();
}


//------------------------------------
void ofImage::clear(){

	myPixels.clear();
	if(bUseTexture)	tex.clear();

	width					= 0;
	height					= 0;
	bpp						= 0;
	type 					= OF_IMAGE_UNDEFINED;
	bUseTexture 			= true;		// the default is, yes, use a texture
}

//------------------------------------
unsigned char * ofImage::getPixels(){
	return myPixels.getPixels();
}

ofPixels ofImage::getOFPixels(){
	return myPixels;
}

ofPixels ofImage::getOFPixels() const{
	return myPixels;
}

//------------------------------------
//for getting a reference to the texture
ofTexture & ofImage::getTextureReference(){
	if(!tex.bAllocated() ){
		ofLog(OF_LOG_WARNING, "ofImage - getTextureReference - texture is not allocated");
	}
	return tex;
}

//----------------------------------------------------------
void ofImage::bind(){
	if (bUseTexture && tex.bAllocated())
		tex.bind();
}

//----------------------------------------------------------
void ofImage::unbind(){
	if (bUseTexture && tex.bAllocated())
		tex.unbind();
}



//------------------------------------
void  ofImage::setFromPixels(unsigned char * newPixels, int w, int h, ofImageType newType, bool bOrderIsRGB){

	allocate(w, h, newType);
	myPixels.setFromPixels(newPixels,w,h,newType);

	if (!bOrderIsRGB){
		myPixels.swapRgb();
	}

	update();
}

//------------------------------------
void ofImage::update(){

	if (myPixels.isAllocated() && bUseTexture){
		tex.loadData(myPixels.getPixels(), myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}

	width	= myPixels.getWidth();
	height	= myPixels.getHeight();
	bpp		= myPixels.getBitsPerPixel();
	type	= myPixels.getImageType();
}

//------------------------------------
void ofImage::setUseTexture(bool bUse){
	bUseTexture = bUse;
}


//------------------------------------
void ofImage::grabScreen(int _x, int _y, int _w, int _h){

	allocate(_w, _h, OF_IMAGE_COLOR);

	int screenHeight = ofGetHeight();
	_y = screenHeight - _y;
	_y -= _h; // top, bottom issues

	if (!((width == _w) && (height == _h))){
		resize(_w, _h);
	}

	#ifndef TARGET_OF_IPHONE
		glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT );											// be nice to anyone else who might use pixelStore
	#endif
		glPixelStorei(GL_PACK_ALIGNMENT, 1);														// set read non block aligned...
		glReadPixels(_x, _y, _w, _h, myPixels.getGlDataType(),GL_UNSIGNED_BYTE, myPixels.getPixels());		// read the memory....
	#ifndef TARGET_OF_IPHONE
		glPopClientAttrib();
	#endif

	int sizeOfOneLineOfPixels = myPixels.getWidth() * myPixels.getBytesPerPixel();
	unsigned char * tempLineOfPix = new unsigned char[sizeOfOneLineOfPixels];
	unsigned char * linea;
	unsigned char * lineb;
	for (int i = 0; i < myPixels.getHeight()/2; i++){
		linea = myPixels.getPixels() + i * sizeOfOneLineOfPixels;
		lineb = myPixels.getPixels() + (myPixels.getHeight()-i-1) * sizeOfOneLineOfPixels;
		memcpy(tempLineOfPix, linea, sizeOfOneLineOfPixels);
		memcpy(linea, lineb, sizeOfOneLineOfPixels);
		memcpy(lineb, tempLineOfPix, sizeOfOneLineOfPixels);
	}
	delete [] tempLineOfPix;
	update();
}


//------------------------------------
void ofImage::clone(const ofImage &mom){

	myPixels = mom.getOFPixels();

	tex.clear();
	bUseTexture = mom.bUseTexture;
	if (bUseTexture == true){
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}

	update();
}

//------------------------------------
void ofImage::setImageType(ofImageType newType){
	changeTypeOfPixels(myPixels, newType);
	update();
}

//------------------------------------
void ofImage::resize(int newWidth, int newHeight){
	resizePixels(myPixels, newWidth, newHeight);

	if (bUseTexture == true){
		tex.clear();
		tex.allocate(myPixels.getWidth(), myPixels.getHeight(), myPixels.getGlDataType());
	}

	update();
}


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
// freeImage based code & utilities:


//----------------------------------------------------
FIBITMAP *  ofImage::getBmpFromPixels(ofPixels &pix){

	FIBITMAP * bmp = NULL;

	int w						= pix.getWidth();
	int h						= pix.getHeight();
	unsigned char * pixels		= pix.getPixels();
	int bpp						= pix.getBitsPerPixel();
	int bytesPerPixel			= pix.getBytesPerPixel();

	#ifdef TARGET_LITTLE_ENDIAN
		pix.swapRgb();
	#endif

	bmp							= FreeImage_ConvertFromRawBits(pixels, w,h, w*bytesPerPixel, bpp, 0,0,0, true);

	#ifdef TARGET_LITTLE_ENDIAN
		pix.swapRgb();
	#endif

	//this is for grayscale images they need to be paletted from: http://sourceforge.net/forum/message.php?msg_id=2856879
	if( pix.getImageType() == OF_IMAGE_GRAYSCALE ){
		RGBQUAD *pal = FreeImage_GetPalette(bmp);
		for(int i = 0; i < 256; i++) {
			pal[i].rgbRed = i;
			pal[i].rgbGreen = i;
			pal[i].rgbBlue = i;
		}
	}

	return bmp;
}

//----------------------------------------------------
void ofImage::putBmpIntoPixels(FIBITMAP * bmp, ofPixels &pix){
	int width			= FreeImage_GetWidth(bmp);
	int height			= FreeImage_GetHeight(bmp);
	int bpp				= FreeImage_GetBPP(bmp);
	int bytesPerPixel	= bpp / 8;
	//------------------------------------------
	// call the allocation routine (which checks if really need to allocate) here:
	pix.allocate(width, height, bpp);
	FreeImage_ConvertToRawBits(pix.getPixels(), bmp, width*bytesPerPixel, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, true);  // get bits

#ifdef TARGET_LITTLE_ENDIAN
	pix.swapRgb();
#endif
}

//----------------------------------------------------
void ofImage::resizePixels(ofPixels &pix, int newWidth, int newHeight){

	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	convertedBmp = FreeImage_Rescale(bmp, newWidth, newHeight, FILTER_BICUBIC);
	putBmpIntoPixels(convertedBmp, pix);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);

}

//----------------------------------------------------
void ofImage::changeTypeOfPixels(ofPixels &pix, ofImageType newType){

	
		
	if (pix.getImageType() == newType) return;

	FIBITMAP * bmp					= getBmpFromPixels(pix);
	FIBITMAP * convertedBmp			= NULL;

	// check if we need to reallocate the texture.
	bool bNeedNewTexture = false;
	int oldType = pix.getImageType();
	if (newType > oldType){ 
		bNeedNewTexture = true;
	}
	
	// new type !
	switch (newType){
			
		//------------------------------------
		case OF_IMAGE_GRAYSCALE:
			convertedBmp = FreeImage_ConvertToGreyscale(bmp);
			break;
			
		//------------------------------------
		case OF_IMAGE_COLOR:
			convertedBmp = FreeImage_ConvertTo24Bits(bmp);
			if (bNeedNewTexture){
				tex.clear();
				tex.allocate(myPixels.getWidth(), myPixels.getHeight(), GL_RGB);
			}
			break;
		
		//------------------------------------
		case OF_IMAGE_COLOR_ALPHA:
			convertedBmp = FreeImage_ConvertTo32Bits(bmp);
			if (bNeedNewTexture){
				tex.clear();
				tex.allocate(myPixels.getWidth(), myPixels.getHeight(), GL_RGBA);
			}
			break;
	}

	putBmpIntoPixels(convertedBmp, pix);

	if (bmp != NULL)				FreeImage_Unload(bmp);
	if (convertedBmp != NULL)		FreeImage_Unload(convertedBmp);

}

//----------------------------------------------------
// freeImage based stuff:
void ofCloseFreeImage(){
	if (bFreeImageInited){
		FreeImage_DeInitialise();
		bFreeImageInited = false;
	}
}

//----------------------------------------------------
bool ofImage::loadImageIntoPixels(string fileName, ofPixels &pix){

	int					width, height, bpp;
	ofImageType			type;
	fileName			= ofToDataPath(fileName);
	bool bLoaded		= false;
	FIBITMAP 			* bmp = NULL;


	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if(fif == FIF_UNKNOWN) {
		// or guess via filename
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		bmp					= FreeImage_Load(fif, fileName.c_str(), 0);
		bLoaded = true;
		if (bmp == NULL){
			bLoaded = false;
		}
	}
	//-----------------------------

	if (bLoaded ){
		putBmpIntoPixels(bmp,pix);
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}

	return bLoaded;
}

//----------------------------------------------------
bool ofImage::loadImageFromMemory(unsigned char * buffer, unsigned int numBytes, ofPixels &pix){

	int					width, height, bpp;
	bool bLoaded		= false;
	FIBITMAP * bmp		= NULL;
	FIMEMORY *hmem		= NULL;
	
	printf("loadImageFromMemory\n");

	hmem = FreeImage_OpenMemory(buffer, numBytes);
	if (hmem == NULL){
		printf("couldn't create memory handle! \n");
		return false;
	}

	//get the file type!
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
	if( fif == -1 ){
		printf("unable to guess format", fif);
		return false;
		FreeImage_CloseMemory(hmem);
	}


	//make the image!!
	bmp = FreeImage_LoadFromMemory(fif, hmem, 0);
	
	if( bmp != NULL ){
		bLoaded = true;
		printf("FreeImage_LoadFromMemory worked!\n");
	}
	
	//-----------------------------

	if (bLoaded){
		putBmpIntoPixels(bmp,pix);
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
	
	if( hmem != NULL ){
		FreeImage_CloseMemory(hmem);
	}

	return bLoaded;
}


//----------------------------------------------------------------
void  ofImage::saveImageFromPixels(string fileName, ofPixels &pix){

	if (pix.isAllocated() == false){
		ofLog(OF_LOG_ERROR,"error saving image - pixels aren't allocated");
		return;
	}


	FIBITMAP * bmp	= getBmpFromPixels(pix);

	fileName = ofToDataPath(fileName);
	if (pix.isAllocated()){
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		fif = FreeImage_GetFileType(fileName.c_str(), 0);
		if(fif == FIF_UNKNOWN) {
			// or guess via filename
			fif = FreeImage_GetFIFFromFilename(fileName.c_str());
		}
		if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
			if((FREE_IMAGE_FORMAT)fif != FIF_JPEG)
			   FreeImage_Save(fif, bmp, fileName.c_str());
			else
			   FreeImage_Save(fif, bmp, fileName.c_str(),JPEG_QUALITYSUPERB);
		}
	}

	if (bmp != NULL){
		FreeImage_Unload(bmp);
	}
}

//----------------------------------------------------------
float ofImage::getHeight(){
	return height;
}

//----------------------------------------------------------
float ofImage::getWidth(){
	return width;
}

//----------------------------------------------------------
// Sosolimited: texture compression
// call this function before you call loadImage()
void ofImage::setCompression(ofTexCompression compression)
{
	tex.setCompression( compression );
}



