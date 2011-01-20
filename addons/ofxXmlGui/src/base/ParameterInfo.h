/*
 *  ParameterInfo.h
 *  Gui
 *
 *  Created by Marek Bereza on 12/02/2010.
 *  Copyright 2010 Apple Inc. All rights reserved.
 *
 */

class ParameterInfo {
public:
	string name;
	string xmlName;
	void *value;
	string type;
	ParameterInfo(string _name, string _xmlName, string _type, void *_value) {
		name = _name;
		type = _type;
		value = _value;
		xmlName = _xmlName;
	}
};