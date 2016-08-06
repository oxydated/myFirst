#ifndef __XMLUTILITYFUNCTIONS_H__
#define __XMLUTILITYFUNCTIONS_H__

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#include <ObjBase.h>
#include <msxml6.h>
#include <comutil.h>
#include <tchar.h>

IXMLDOMDocument3* loadModelDocument(const char *name);

HRESULT getNodeElementFromNode(IXMLDOMNode* theNode, BSTR theQuery, IXMLDOMNode** returnedElementNode);

HRESULT getIntAttributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, int &retIntValue);

HRESULT getUShortAttributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, unsigned short &retUShortValue);

HRESULT getFloatAttributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, float &retFloatValue);

HRESULT getBSTRattributeFromElementInNode(IXMLDOMNode* theNode, LPCSTR theAttribute, _bstr_t &retBSTRvalue);

#endif