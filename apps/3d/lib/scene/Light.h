/*
 *  Light.h
 *  Render
 *
 *  Simple subclass of ofLight for nicer custom drawing.
 * 
 *  Created by Marek Bereza on 07/03/2011.
 *
 */

class Light: public ofLight {
public:
	void customDraw() {
		ofNoFill();
		ofSetHexColor(0xFFFF00);
		ofBox(0, 0, 0, 0.2);
		ofFill();
	}
};