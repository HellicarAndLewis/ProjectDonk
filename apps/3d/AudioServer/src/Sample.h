/*
 *  Sample.h
 *  AudioServer
 *
 *  Created by Mac Mini 1 on 10/03/2011.
 *
 */
/** 
 * \brief Plays a wav file on loop and delivers samples through getSample() one at a time
 */
#include "WavFile.h"
class Sample {
public:
	void load(string file) {
		wavFile.load(ofToDataPath(file));
	
		data = wavFile.data;
		length = wavFile.getLength();
		playhead = 0;
	}
	
	float getSample() {
		playhead %= length;
		return data[playhead++];
	}
	
private:
	float *data;
	int playhead;
	int length;
	WavFile wavFile;
};