// ******************************************************************
//
//	testSax: example of MSXML SAX2/COM use and base classes for handlers
//  (C) Microsoft Corp., 2000
//
// ******************************************************************
//
// SAXErrorHandler.h: interface for the SAXErrorHandler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAXERRORHANDLER_H__736F82C7_A4FF_4B7F_B862_A77B333A299D__INCLUDED_)
#define AFX_SAXERRORHANDLER_H__736F82C7_A4FF_4B7F_B862_A77B333A299D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import <msxml3.dll> raw_interfaces_only 
using namespace MSXML2;

class SAXErrorHandlerImpl : public ISAXErrorHandler  
{
public:
	SAXErrorHandlerImpl();
	virtual ~SAXErrorHandlerImpl();

		// This must be correctly implemented, if your handler must be a COM Object (in this example it does not)
		long __stdcall QueryInterface(const struct _GUID &,void ** );
		unsigned long __stdcall AddRef(void);
		unsigned long __stdcall Release(void);

        virtual HRESULT STDMETHODCALLTYPE error( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode);
        
        virtual HRESULT STDMETHODCALLTYPE fatalError( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode);
        
        virtual HRESULT STDMETHODCALLTYPE ignorableWarning( 
            /* [in] */ ISAXLocator __RPC_FAR *pLocator,
            /* [in] */ unsigned short * pwchErrorMessage,
			/* [in] */ HRESULT errCode);

};

#endif // !defined(AFX_SAXERRORHANDLER_H__736F82C7_A4FF_4B7F_B862_A77B333A299D__INCLUDED_)
