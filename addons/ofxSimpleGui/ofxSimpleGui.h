
#ifndef OFX_SIMPLE_GUI_H
#define OFX_SIMPLE_GUI_H

#include "ofxSimpleGuiMain.h"

#include "ofxToggle.h"
#include "ofxQuadWarp.h"
#include "ofxSliderMovie.h"
#include "ofxSliderFloat.h"
#include "ofxSliderInt.h"
#include "ofxGuiTitle.h"


//		--- Features ---
//		- set from xml 
//			allows you to define an xml id and set the variables
//			from the xml file.


//		- default save to xml button
//				if the xml file has been set of setFromXML() has been called
//				there is a default button created.

//		--- Todo ---
//		- figure out how to build the whol gui from xml and keep it simple.


//------------------------------------------------------------------------------ OFX SIMPLE GUI
class ofxSimpleGui : public ofMouseListener, public ofKeyListener, public ofAppListener {
	
	
private:
	
	bool							verbose;
	bool							doSave;
	bool							doRender;
	
protected:
	
	
public:
	
	vector <ofxGuiTitle*>			titles;
	vector <ofxToggle*>				toggles;		
	vector <ofxSliderInt*>			slidersI;
	vector <ofxSliderFloat*>		slidersF;
	
	vector <ofxSliderMovie*>		slidersMovie;
	vector <ofxQuadWarp*>			quadWarpers;
	
	bool							mousedown;
	bool							mouseup;
	float							mx, my;
	int								key;
	float							xspace, yspace;
	int								guiFocus;
	int								guiCount;

	
	ofxGuiTitle						guiTitle;
	ofxToggle*						saveButton;
	ofxXmlSettings					XML;
	string							xmlFile;
	
	//------------------------------------------------------------------------------
	
	ofxSimpleGui();
	
	void	setListeners();
	void	setFromXML(string file);
	void	saveToXML(string file);	
	void	setTitle(string name);
	void	setVerbose(bool v);
	
	int		getValueI(string nameID);
	float	getValueF(string nameID);
	bool	getValueB(string nameID);
	
	void	renderFocus(float x, float y);
	void    toggleRender();
	void	update();
	void	draw();
	
	void	addSlider(string _name, int *_value, int _min, int _max, int xmlID=NULL);
	void	addSlider(string _name, float *_value, float _min, float _max, int xmlID=NULL);
	void	addMovieSlider(string _name, ofVideoPlayer *_input, float *_value, float _min, float _max, int xmlID=NULL);
	void	addQuadWarper(float x, float y, float sw, float sh, ofPoint **pts);
	void	addToggle(string _name, bool *_value, int _xmlID=NULL);	
	void	addTitle(string _name);
	
	void	mouseMoved(int x, int y);
	void	keyPressed(int key);
	
};
#endif








