#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "tinyxml.h"
#include <sstream>
#include "OgreString.h"

class XmlParser
{
protected:
	TiXmlDocument* xmlFile;
	TiXmlElement*  rootNode;
private:
	std::string intToString(int intToConvert);
public:
	XmlParser();
	XmlParser(char* fileName);
	~XmlParser();

	bool openXmlFile(char* fileName);
	TiXmlElement* SearchForElement(TiXmlElement* searchEle, char* nodeName);
	TiXmlElement* SearchForElement(TiXmlElement* searchEle, char* nodeName, int id);

	//Getters
	TiXmlElement* getXmlRootNode(){ return rootNode; }
};

#endif
