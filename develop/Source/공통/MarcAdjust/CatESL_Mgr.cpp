// CatESL_Mgr.cpp: implementation of the CCatESL_Mgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CatESL_Mgr.h"
#include "MarcAdjFrame.h"

// Program Mode - Univ Mode, Original Mode
#include "..\\MarcAdjustApi\\CatProgramMode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCatESL_Mgr::CCatESL_Mgr(CESL_Mgr *pParent /*= NULL*/) 
	: CESL_Mgr(IDD_CODE, pParent)
{
	m_pDM_Species = NULL;
	m_pDM_Index = NULL;
	m_pDM_Book = NULL;
	m_pDM_Volume = NULL;
	m_pDM_Purchase = NULL;
	m_pDM_Donation = NULL;

	m_pCatApi = NULL;

	m_pDM_HoldBook = NULL;

	m_pDM_Appendix = NULL;

	m_pDM_HoldAppendix = NULL;

	m_nModeMONO_SERIAL = MODE_MONO;

	m_pDM_HoldBook = NULL;
	m_nAppMode = -1;
}

BOOL CCatESL_Mgr::Create(CWnd* pParentWnd)
{
#ifdef _CAT_PROGRAM_MODE_UNIV_
	return CDialog::Create(IDD_CODE_UNIV, pParentWnd);
#else
	return CDialog::Create(IDD_CODE, pParentWnd);
#endif
}

BEGIN_MESSAGE_MAP(CCatESL_Mgr, CDialog)
	//{{AFX_MSG_MAP(CCatESL_Mgr)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 32015, OnCreate505Tag)
	ON_MESSAGE(WM_USER + 1001, OnSetFocusGridEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAdjCodeDlg Operations

INT CCatESL_Mgr::InitCatESL_Mgr(INT nAppMode, INT nModeMONO_SERIAL /*= MODE_MONO*/)
{
	CString strSMAlias;
	CString strDM_Specias;
	CString strDM_Index;
	CString strDM_Book;
	CString strDM_HoldBook;
	CString strDM_Volume;
	CString strDM_Purchase;
	CString strDM_Appendix;
	CString strDM_HoldAppendix;

	m_nModeMONO_SERIAL = nModeMONO_SERIAL;
	
	if (nModeMONO_SERIAL == MODE_MONO)
	{
		strSMAlias = _T("SM_단행_정리DM");

		strDM_Specias = _T("DM_단행_정리DM_종정보");
		strDM_Index = _T("DM_단행_정리DM_색인정보");
		strDM_Book = _T("DM_단행_정리DM_책정보");
		strDM_Volume = _T("DM_단행_정리DM_권정보");
		strDM_Purchase = _T("DM_단행_정리DM_구입정보");
		strDM_Appendix = _T("DM_단행_정리DM_부록정보");
	}
	else if (nModeMONO_SERIAL == MODE_SERIAL)
	{
		strSMAlias = _T("SM_연속_정리DM");

		strDM_Specias = _T("DM_연속_정리DM_종정보");
		strDM_Index = _T("DM_연속_정리DM_색인정보");
		strDM_Book = _T("DM_연속_정리DM_책정보");
		strDM_HoldBook = _T("DM_연속_정리DM_소장책정보");
		strDM_Volume = _T("DM_연속_정리DM_권정보");
		strDM_Purchase = _T("DM_연속_정리DM_구입정보");
		strDM_Appendix = _T("DM_연속_정리DM_부록정보");
		strDM_HoldAppendix = _T("DM_연속_정리DM_소장부록정보");
	}
	if (InitESL_Mgr(strSMAlias) < 0) return -1;

	m_pDM_Species = FindDM(strDM_Specias);
	m_pDM_Index = FindDM(strDM_Index);
	m_pDM_Book = FindDM(strDM_Book);
	m_pDM_Volume = FindDM(strDM_Volume);
	m_pDM_Purchase = FindDM(strDM_Purchase);
	
	if (m_pDM_Species == NULL || 
		m_pDM_Index == NULL || 
		m_pDM_Book == NULL || 
		m_pDM_Volume == NULL ||
		m_pDM_Purchase == NULL
		) return -1;


	if (m_pCatApi)
		delete m_pCatApi;
	if (nModeMONO_SERIAL == MODE_MONO)
		m_pCatApi = new CCatApi_BO(this);
	else if (nModeMONO_SERIAL == MODE_SERIAL)
		m_pCatApi = new CCatApi_SE(this);

	SetAppMode(nAppMode);

	if (m_nModeMONO_SERIAL == MODE_MONO)
	{
		m_pDM_Appendix = FindDM(strDM_Appendix);
		if (m_pDM_Appendix == NULL) return -1;
	}
	else if (m_nModeMONO_SERIAL == MODE_SERIAL)
	{
		m_pDM_HoldBook = FindDM(strDM_HoldBook);
		if (m_pDM_HoldBook == NULL) return -1;
		m_pDM_Appendix = FindDM(strDM_Appendix);
		if (m_pDM_Appendix == NULL) return -1;
		m_pDM_HoldAppendix = FindDM(strDM_HoldAppendix);
		if (m_pDM_HoldAppendix == NULL) return -1;
	}

	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CMarcAdjCodeDlg message handlers

VOID CCatESL_Mgr::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_pCatApi)
	{
		delete m_pCatApi;
		m_pCatApi = NULL;
	}
}


UINT CCatESL_Mgr::OnCreate505Tag(WPARAM wParam, LPARAM lParam)
{
	return static_cast<CMarcAdjFrame*>(GetParentFrame())->GetMarcAdjMgr()->CMD_Create505Tag();
}

VOID CCatESL_Mgr::OnSetFocusGridEdit()
{
	((CMarcAdjFrame*)GetParentFrame())->OnSetFocusGridEdit();
	return;
}
