// MyContent.h: interface for the MyContent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCONTENT_H__E1B3AF99_0FA6_44CD_82E3_55719F9E3806__INCLUDED_)
#define AFX_MYCONTENT_H__E1B3AF99_0FA6_44CD_82E3_55719F9E3806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SAXContentHandlerImpl.h"
#include "mechdata.h"
#include "weapondata.h"
#include "technologydata.h"
#include "buildingdata.h"
#include "upgradedata.h"
#include <vector>

using std::vector;

class MyContent : public SAXContentHandlerImpl  
{
public:
    MyContent();
    virtual ~MyContent();
        
        virtual HRESULT STDMETHODCALLTYPE startElement( 
            /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName,
            /* [in] */ ISAXAttributes __RPC_FAR *pAttributes);
        
        virtual HRESULT STDMETHODCALLTYPE endElement( 
            /* [in] */ wchar_t __RPC_FAR *pwchNamespaceUri,
            /* [in] */ int cchNamespaceUri,
            /* [in] */ wchar_t __RPC_FAR *pwchLocalName,
            /* [in] */ int cchLocalName,
            /* [in] */ wchar_t __RPC_FAR *pwchRawName,
            /* [in] */ int cchRawName);

		virtual HRESULT STDMETHODCALLTYPE characters( 
            /* [in] */ wchar_t __RPC_FAR *pwchChars,
            /* [in] */ int cchChars);

        virtual HRESULT STDMETHODCALLTYPE startDocument();

		std::vector<CBuildingData *> &getBuildingData();
		std::vector<CMechData *> &getMechData();
		std::vector<CTechnologyData *> &getTechnologyData();
		std::vector<CWeaponData *> &getWeaponData();
		std::vector<CUpgradeData *> &getUpgradeData();
private:
		std::vector<CBuildingData *> mBuildingData;
		std::vector<CMechData *> mMechData;
		std::vector<CTechnologyData *> mTechnologyData;
		std::vector<CWeaponData *> mWeaponData;
		std::vector<CUpgradeData *> mUpgradeData;

		char mUseBuf;
		enum {
			TNONE,
			TBUILDING,
			TMECH,
			TTECHNOLOGY,
			TWEAPON,
			TUPGRADE
		} mDataType;
		std::vector<uint32> mBuffer;
		std::vector<uint32> mBuffer2;
		std::vector<CUpgradeData::CSetValue *> mBuffer3;
};

#endif // !defined(AFX_MYCONTENT_H__E1B3AF99_0FA6_44CD_82E3_55719F9E3806__INCLUDED_)
