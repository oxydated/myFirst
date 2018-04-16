#include <locale>
#include <Windows.h>
#include "XMLDocument.h"

namespace {
	class executedBeforeMain {
	public:
		explicit executedBeforeMain() {
			std::locale::global(std::locale("en-US"));
			SetThreadLocale(LOCALE_INVARIANT);
		}
	};

	executedBeforeMain instance = executedBeforeMain();
}

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

		int getIntAttributeFromElement(const MSXML2::IXMLDOMElementPtr &theElement, _bstr_t attribName)
		{
			_variant_t&& intVariant =  theElement->getAttribute(attribName);
			intVariant.ChangeType(VT_I4);
			return intVariant.intVal;
		}

		unsigned short getUShortAttributeFromElement(const MSXML2::IXMLDOMElementPtr &theElement, _bstr_t attribName)
		{
			_variant_t&& ushortVariant = theElement->getAttribute(attribName);
			ushortVariant.ChangeType(VT_UI2);
			return ushortVariant.intVal;
		}

		float getFloatAttributeFromElement(const MSXML2::IXMLDOMElementPtr &theElement, _bstr_t attribName)
		{
			_variant_t&& intVariant = theElement->getAttribute(attribName);
			intVariant.ChangeType(VT_BSTR);
			return std::stof(std::wstring(intVariant.bstrVal));
		}

		std::wstring getWStringAttributeFromElement(const MSXML2::IXMLDOMElementPtr &theElement, _bstr_t attribName)
		{
			_variant_t&& intVariant = theElement->getAttribute(attribName);
			intVariant.ChangeType(VT_BSTR);
			return std::wstring(intVariant.bstrVal);
		}
	}
}