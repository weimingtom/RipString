// IMEEngine.h: interface for the IMEEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMEENGINE_H__24BFA61F_765F_44FE_980F_F4167EB4FCC4__INCLUDED_)
#define AFX_IMEENGINE_H__24BFA61F_765F_44FE_980F_F4167EB4FCC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Jp2Hiragana.cpp : Defines the entry point for the console application.
//

//#include <afxdisp.h>
//#include <atlbase.h>
//#include <comdef.h>

#include "msime.h"

/*
L"MSIME.China";
L"MSIME.Japan";
L"MSIME.Taiwan";
L"MSIME.Taiwan.ImeBbo";
*/

class IMEEngine  
{
private:
	BOOL	m_bCoInit;
	BOOL	m_bCoCreat;
	BOOL	m_bLangOpen;
	IFELanguage *m_pIFELanguage;
public:
	MORRSLT	*m_pmorrslt;
	DWORD	m_dwCaps;
	LPCWSTR	m_ImeName;
public:
	IMEEngine();
	~IMEEngine();
	BOOL IsReady();
	HRESULT SetIMEEngine( LPCWSTR msime = L"MSIME.Japan" );
	void UnsetIMEEngine( void );
	HRESULT GetJMorphResult( DWORD dwRequest, DWORD dwCMode, const WCHAR *pwchInput );
};

#endif // !defined(AFX_IMEENGINE_H__24BFA61F_765F_44FE_980F_F4167EB4FCC4__INCLUDED_)
