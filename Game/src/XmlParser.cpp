#include "XmlParser.h"

XmlParser::XmlParser()
{

}

XmlParser::XmlParser(char* fileName) //Allow the construction of the object and intialize the xml file simultaneously
{
	openXmlFile(fileName);
}

XmlParser::~XmlParser()
{
}

bool XmlParser::openXmlFile(char* fileName)
{
	xmlFile = new TiXmlDocument(fileName);

	bool loadOkay = xmlFile->LoadFile();
	if (loadOkay)
	{
		std::cout<<"Successfully opened the file: "<<fileName<<std::endl;
		rootNode = xmlFile->FirstChildElement();
		return true;
	}
	else
	{
		std::cout<<"Failed to open the file: "<<fileName<<std::endl;
		return false;
	}
}

TiXmlElement* XmlParser::SearchForElement(TiXmlElement* searchEle, char* nodeName)
{
   if (!searchEle) return NULL;
 
   TiXmlElement* pElement = searchEle->FirstChildElement();
   
   while (pElement)
   {
		if(strcmp(pElement->Value(), nodeName) == 0)
			return pElement;
		pElement = pElement->NextSiblingElement();
   }
   return NULL;
}

TiXmlElement* XmlParser::SearchForElement(TiXmlElement* searchEle, char* nodeName, int id)
{
   if (!searchEle) return NULL;
 
   TiXmlElement* pElement = searchEle->FirstChildElement();
   
   while (pElement)
   {
	    std::cout<<"Checking element: "<<pElement->Value()<<std::endl;
		if( strcmp( pElement->Value(), nodeName ) == 0 )
		{
			if( strcmp( pElement->Attribute("ID"), intToString(id).c_str() ) == 0 )
				return pElement;
		}
		pElement = pElement->NextSiblingElement();
   }
   return NULL;
}

std::string XmlParser::intToString(int intToConvert)
{
	std::string outputString;
	std::stringstream stringConverter;
	stringConverter << intToConvert;
	outputString = stringConverter.str();
	return outputString;
}
