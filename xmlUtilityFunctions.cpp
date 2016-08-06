#include "xmlUtilityFunctions.h"

IXMLDOMDocument3* loadModelDocument(const char *name){
	IXMLDOMDocument3* theDocument = NULL;
	HRESULT hr = S_OK;

	_variant_t theFileName = _variant_t(name);
	VARIANT theLoadingResult = _variant_t(false).Detach();

	if (theDocument == NULL){
		CoInitialize(NULL);
		hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&theDocument));

		theDocument->put_async(VARIANT_FALSE);
		theDocument->put_validateOnParse(VARIANT_FALSE);
		theDocument->put_resolveExternals(VARIANT_FALSE);
		theDocument->put_preserveWhiteSpace(VARIANT_TRUE);

		hr = theDocument->load(theFileName, &theLoadingResult.boolVal);
		bool itWorked = (theLoadingResult.boolVal == VARIANT_TRUE);
		VariantClear(&theLoadingResult);

		//IXMLDOMProcessingInstruction* thePI = NULL;
		//hr = theDocument->createProcessingInstruction(L"xml", L"version='1.0'", &thePI);
		//hr = theDocument->appendChild(thePI, NULL);
	}
	return theDocument;
}

HRESULT getNodeElementFromNode(IXMLDOMNode* theNode, BSTR theQuery, IXMLDOMNode** returnedElementNode){
	HRESULT hr = S_OK;

	IXMLDOMElement* theElementInNode = NULL;
	hr = theNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theElementInNode);

	//IXMLDOMNode* theQueriedNode = NULL;
	hr = theElementInNode->selectSingleNode(theQuery, returnedElementNode);
	//hr = theQueriedNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)returnedElement);
	return hr;
}

HRESULT getIntAttributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, int &retIntValue){
	HRESULT hr = S_OK;
	VARIANT attribValue;

	IXMLDOMElement* theElementInNode = NULL;
	hr = theNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theElementInNode);
	if (hr == S_OK){
		hr = theElementInNode->getAttribute(_bstr_t(theAttribute), &attribValue);
		_variant_t theIntValue = _variant_t(attribValue);
		theIntValue.ChangeType(VT_I4);
		retIntValue = theIntValue.intVal;
	}
	return hr;
}

HRESULT getUShortAttributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, unsigned short &retUShortValue){
	HRESULT hr = S_OK;
	VARIANT attribValue;

	IXMLDOMElement* theElementInNode = NULL;
	hr = theNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theElementInNode);
	if (hr == S_OK){
		hr = theElementInNode->getAttribute(_bstr_t(theAttribute), &attribValue);
		_variant_t theUShortValue = _variant_t(attribValue);
		theUShortValue.ChangeType(VT_UI2);
		retUShortValue = theUShortValue.uiVal;
	}
	return hr;
}

HRESULT getFloatAttributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, float &retFloatValue){
	HRESULT hr = S_OK;
	VARIANT attribValue;

	IXMLDOMElement* theElementInNode = NULL;
	hr = theNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theElementInNode);
	if (hr == S_OK){
		hr = theElementInNode->getAttribute(_bstr_t(theAttribute), &attribValue);
		VARIANT floatVar;
		floatVar.vt = VT_R4;
		VariantChangeTypeEx(&floatVar, &attribValue, LOCALE_INVARIANT, 0, VT_R4);
		retFloatValue = floatVar.fltVal;
	}
	return hr;
}

HRESULT getBSTRattributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, _bstr_t &retBSTRvalue){
	HRESULT hr = S_OK;
	VARIANT attribValue;

	IXMLDOMElement* theElementInNode = NULL;
	hr = theNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&theElementInNode);
	if (hr == S_OK){
		hr = theElementInNode->getAttribute(_bstr_t(theAttribute), &attribValue);
		retBSTRvalue = _variant_t(attribValue);
	}
	return hr;
}
