#include "XMLDocument.h"

namespace oxyde {
	namespace XML {

		MSXML2::IXMLDOMDocumentPtr loadDocument(std::wstring filename)
		{
			MSXML2::IXMLDOMDocumentPtr theDocument = nullptr;
			HRESULT res = theDocument.CreateInstance(__uuidof(MSXML2::DOMDocument60), NULL, CLSCTX_INPROC_SERVER);
			if (SUCCEEDED(res)) {
				theDocument->async = VARIANT_FALSE;
				theDocument->validateOnParse = VARIANT_FALSE;
				theDocument->resolveExternals = VARIANT_FALSE;
				if (theDocument->load(filename.data()) == VARIANT_TRUE) {
					int i = 0;
					i = 3;
				}
			}

			return theDocument;
		}

		int getIntAttributeFromElement(MSXML2::IXMLDOMElementPtr &theElement, _bstr_t attribName)
		{
			_variant_t&& intVariant =  theElement->getAttribute(attribName);
			intVariant.ChangeType(VT_I4);
			return intVariant.intVal;
		}

		/*
		HRESULT getIntAttributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, int &retIntValue) {
		HRESULT hr = S_OK;
		VARIANT attribValue;

		IXMLDOMElement* theElementInNode = NULL;
		hr = theNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theElementInNode);
		if (hr == S_OK) {
		hr = theElementInNode->getAttribute(_bstr_t(theAttribute), &attribValue);
		_variant_t theIntValue = _variant_t(attribValue);
		theIntValue.ChangeType(VT_I4);
		retIntValue = theIntValue.intVal;
		}
		return hr;
		}
		*/

	}
}