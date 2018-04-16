#pragma once

//class xmlDocument
//	Holds the creation of the document Object by loading the XML file.
//	Keeps a reference to it through smart pointer
//
//	It's destroyed as soon as all the Scene objects have been loaded.

#include <Windows.h>
#include <stdio.h>
#include <memory>
#include <string>
#import<msxml6.dll>

namespace oxyde {
	namespace XML {
		MSXML2::IXMLDOMDocumentPtr loadDocument(std::wstring filename);

		int getIntAttributeFromElement(const MSXML2::IXMLDOMElementPtr&, _bstr_t attribName);

		unsigned short getUShortAttributeFromElement(const MSXML2::IXMLDOMElementPtr&, _bstr_t attribName);

		float getFloatAttributeFromElement(const MSXML2::IXMLDOMElementPtr&, _bstr_t attribName);

		std::wstring getWStringAttributeFromElement(const MSXML2::IXMLDOMElementPtr&, _bstr_t attribName);
	}
}


