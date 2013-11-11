// IMEEngine.cpp: implementation of the IMEEngine class.
//
//////////////////////////////////////////////////////////////////////
#include "IMEEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMEEngine::IMEEngine() : m_bCoInit(FALSE), m_bCoCreat(FALSE), m_bLangOpen(FALSE), m_pmorrslt(NULL)
{
	OleInitialize( NULL );
	m_bCoInit = TRUE;

	SetIMEEngine( L"MSIME.Japan" );
}

IMEEngine::~IMEEngine()
{
	UnsetIMEEngine();

	if( m_bCoInit ) {
		CoUninitialize();
		m_bCoInit = FALSE;
	}
}

BOOL IMEEngine::IsReady()
{
	return m_bLangOpen;
}

HRESULT IMEEngine::GetJMorphResult( DWORD dwRequest, DWORD dwCMode, const WCHAR *pwchInput )
{
	if( !IsReady() )
		return S_FALSE;

	if( m_pmorrslt ) {
		CoTaskMemFree( m_pmorrslt );
		m_pmorrslt = NULL;
	}
	return m_pIFELanguage->GetJMorphResult( dwRequest, dwCMode,
		static_cast<INT>(wcslen(pwchInput)), const_cast<WCHAR*>(pwchInput),
		NULL, &m_pmorrslt );
}

HRESULT IMEEngine::SetIMEEngine( LPCWSTR msime )
{
	static const IID IID_IFELanguage = { 0x019f7152, 0xe6db, 0x11d0, { 0x83, 0xc3, 0x00, 0xc0, 0x4f, 0xdd, 0xb8, 0x2e } };
	UnsetIMEEngine();

	CLSID	clsid;
	if( CLSIDFromString( const_cast<LPWSTR>(msime), &clsid ) != S_OK )
		return S_FALSE;

	if( CoCreateInstance( clsid, NULL, CLSCTX_SERVER, IID_IFELanguage, (LPVOID*)&m_pIFELanguage ) != S_OK )
		return S_FALSE;
	
	m_bCoCreat = TRUE;

	if( m_pIFELanguage )
		if( m_pIFELanguage->Open() == S_OK )
			m_bLangOpen = TRUE;

	m_pIFELanguage->GetConversionModeCaps( &m_dwCaps );
	m_ImeName = msime;

	return S_OK;
}

void IMEEngine::UnsetIMEEngine( void )
{
	if( m_pmorrslt ) {
		CoTaskMemFree( m_pmorrslt );
		m_pmorrslt = NULL;
	}

	if( m_bLangOpen ) {
		m_pIFELanguage->Close();
		m_bLangOpen = FALSE;
	}

	if( m_bCoCreat ) {
		m_pIFELanguage->Release();
		m_bCoCreat = FALSE;
	}

	m_dwCaps = 0;
	m_ImeName = NULL;
}
