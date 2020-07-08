// EtcSetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS2UP_SETUP.h"
#include "EtcSetupPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "RegistryEx.h"

/////////////////////////////////////////////////////////////////////////////
// CEtcSetupPage property page

IMPLEMENT_DYNCREATE(CEtcSetupPage, CPropertyPage)

CEtcSetupPage::CEtcSetupPage() : CPropertyPage(CEtcSetupPage::IDD)
{
	//{{AFX_DATA_INIT(CEtcSetupPage)
	m_strLAST_PATCH_PK = _T("");
	m_strAUTOEXTRACT_LAST_PATCH_PK = _T("");
	m_strAUTOEXTRACT_VERSION = _T("");
	m_strAUTOEXTRACT_VERSION_PK = _T("");
	//}}AFX_DATA_INIT

	m_strRegDepth = _T("Software\\ECO\\KOLAS III");

	//++2009.01.19 ADD BY CJY {{++
//DEL	DB정보는 UseDMInfoChange로 통일. DB정보변환기능은 미사용
//DEL 	m_strDBInfoCfgFile = _T("..\\CFG\\UseDMInfoChange.cfg");
	//--2009.01.19 ADD BY CJY --}}
}

CEtcSetupPage::~CEtcSetupPage()
{
}

VOID CEtcSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtcSetupPage)
	DDX_Text(pDX, IDC_eLAST_PATCH_PK, m_strLAST_PATCH_PK);
	DDX_Text(pDX, IDC_eAUTOEXTRACT_LAST_PATCH_PK, m_strAUTOEXTRACT_LAST_PATCH_PK);
	DDX_Text(pDX, IDC_eAUTOEXTRACT_VERSION, m_strAUTOEXTRACT_VERSION);
	DDV_MaxChars(pDX, m_strAUTOEXTRACT_VERSION, 11);
	DDX_Text(pDX, IDC_eAUTOEXTRACT_PK, m_strAUTOEXTRACT_VERSION_PK);
	DDV_MaxChars(pDX, m_strAUTOEXTRACT_VERSION_PK, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEtcSetupPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEtcSetupPage)
	ON_EN_CHANGE(IDC_eLAST_PATCH_PK, OnChangeeLASTPATCHPK)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_eAUTOEXTRACT_LAST_PATCH_PK, OnChangeeAUTOEXTRACTLASTPATCHPK)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_EN_CHANGE(IDC_eAUTOEXTRACT_VERSION, OnChangeeAUTOEXTRACTVERSION)
	ON_EN_CHANGE(IDC_eAUTOEXTRACT_PK, OnChangeeAUTOEXTRACTPK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtcSetupPage message handlers

VOID CEtcSetupPage::OnChangeeLASTPATCHPK() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();
}

VOID CEtcSetupPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CPropertyPage::OnShowWindow(bShow, nStatus);
	
	if (bShow == TRUE) {
		RefreshData();
	}
}

INT CEtcSetupPage::RefreshData(VOID)
{
	// LAST_PATCH_PK 읽어옮
	m_strLAST_PATCH_PK = Get_LAST_PATCH_PK();
	m_strAUTOEXTRACT_LAST_PATCH_PK = Get_AUTOEXTRACT_LAST_PATCH_PK();
	

	//============================================================================
	//2004-07-24[이학중 수정 ]
	m_strAUTOEXTRACT_VERSION = Get_AUTOEXTRACT_VERSION();
	/*
	CString strTemp ;
	strTemp = Get_AUTOEXTRACT_VERSION();
    if(strTemp.GetLength ()>11)
	{
		m_strAUTOEXTRACT_VERSION = strTemp.Mid (0,10);
		m_strAUTOEXTRACT_VERSION_PK = strTemp.Mid (11) ;
	}
    if(strTemp.GetLength ()<= 11)
	{
		m_strAUTOEXTRACT_VERSION = strTemp.Mid (0,10);
		m_strAUTOEXTRACT_VERSION_PK = _T("00") ;	
	}
    */
	//============================================================================


	//++2009.01.19 ADD BY CJY {{++
//DEL	DB정보는 UseDMInfoChange로 통일. DB정보변환기능은 미사용
//DEL 	if (ConvertDBInfo_Show() == 0) {
//DEL 		CButton *pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
//DEL 		CButton *pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
//DEL 		
//DEL 		if (m_bDBInfoMode == TRUE && pButton1 && pButton2) {
//DEL 			pButton1->SetCheck(TRUE);
//DEL 			pButton2->SetCheck(FALSE);
//DEL 		}
//DEL 		if (m_bDBInfoMode == FALSE && pButton1 && pButton2) {
//DEL 			pButton1->SetCheck(FALSE);
//DEL 			pButton2->SetCheck(TRUE);
//DEL 		}
//DEL 	}
	//--2009.01.19 ADD BY CJY --}}

	UpdateData(FALSE);
	return 0;
}

BOOL CEtcSetupPage::OnApply() 
{
	SaveConfing();
	
	return CPropertyPage::OnApply();
}

INT CEtcSetupPage::SaveConfing()
{
	UpdateData(TRUE);

	Set_LAST_PATCH_PK(m_strLAST_PATCH_PK);
	Set_AUTOEXTRACT_LAST_PATCH_PK(m_strAUTOEXTRACT_LAST_PATCH_PK);
	
	//====================================================================
	//2004-07-24 [이학중 수정]
	Set_AUTOEXTRACT_VERSION(m_strAUTOEXTRACT_VERSION);
    /*
	CString strTemp ;
	strTemp.Format (_T("%s_%s"),m_strAUTOEXTRACT_VERSION,m_strAUTOEXTRACT_VERSION_PK);
    Set_AUTOEXTRACT_VERSION(strTemp); 
	*/
	 //====================================================================


	//++2009.01.19 ADD BY CJY {{++	
//DEL	DB정보는 UseDMInfoChange로 통일. DB정보변환기능은 미사용
//DEL 	ConvertDBInfo_Save();
	//--2009.01.19 ADD BY CJY --}}
	return 0;
}

CString CEtcSetupPage::Get_LAST_PATCH_PK(VOID)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString depth = m_strRegDepth;
	CString str = _T("");
	CRegistryEx reg(hKey);
	if ( reg.VerifyKey( depth ) ) {
		reg.Open( hKey, depth );
		reg.Read( _T("LAST_PATCH_PK"), str);
		reg.Close();
	}

	return str;
}

INT CEtcSetupPage::Set_LAST_PATCH_PK(CString str)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString depth = m_strRegDepth;

	CRegistryEx reg(hKey);
	if ( !reg.VerifyKey( depth ) ) reg.CreateKey( hKey, depth );
	else reg.Open( hKey, depth );

	reg.Write( _T("LAST_PATCH_PK"), str);
	reg.Close();
	return 0;
}

VOID CEtcSetupPage::OnChangeeAUTOEXTRACTLASTPATCHPK() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();
}

CString CEtcSetupPage::Get_AUTOEXTRACT_LAST_PATCH_PK(VOID)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString depth = m_strRegDepth;
	CString str = _T("");
	CRegistryEx reg(hKey);
	if ( reg.VerifyKey( depth ) ) {
		reg.Open( hKey, depth );
		reg.Read( _T("AUTOEXTRACT_LAST_PATCH_PK"), str);
		reg.Close();
	}

	return str;
}

CString CEtcSetupPage::Get_AUTOEXTRACT_VERSION(VOID)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString depth = m_strRegDepth;
	CString str = _T("");
	CRegistryEx reg(hKey);
	if ( reg.VerifyKey( depth ) ) {
		reg.Open( hKey, depth );
		reg.Read( _T("VERSION"), str);
		reg.Close();
	}

	return str;
}

INT CEtcSetupPage::Set_AUTOEXTRACT_LAST_PATCH_PK(CString str)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString depth = m_strRegDepth;

	CRegistryEx reg(hKey);
	if ( !reg.VerifyKey( depth ) ) reg.CreateKey( hKey, depth );
	else reg.Open( hKey, depth );

	reg.Write( _T("AUTOEXTRACT_LAST_PATCH_PK"), str);
	reg.Close();
	return 0;
}

INT CEtcSetupPage::Set_AUTOEXTRACT_VERSION(CString str)
{
	HKEY hKey = HKEY_LOCAL_MACHINE;
	CString depth = m_strRegDepth;

	CRegistryEx reg(hKey);
	if ( !reg.VerifyKey( depth ) ) reg.CreateKey( hKey, depth );
	else reg.Open( hKey, depth );

	reg.Write( _T("VERSION"), str);
	reg.Close();
	return 0;
}

//++2009.01.19 ADD BY CJY {{++
//DEL	DB정보는 UseDMInfoChange로 통일. DB정보변환기능은 미사용
//DEL 
//DEL INT CEtcSetupPage::ConvertDBInfo_Show()
//DEL {
//DEL 	CStdioFile fd;
//DEL 	if (fd.Open(m_strDBInfoCfgFile, CStdioFile::modeRead | CFile::typeBinary) == FALSE) return -1;
//DEL 	
//DEL 	TCHAR cUni;
//DEL 	fd.Read( &cUni, sizeof(TCHAR) );
//DEL 	if( 0xFEFF != cUni )
//DEL 	{
//DEL 		fd.SeekToBegin();
//DEL 	}
//DEL 
//DEL 	m_strDBInfoBefore = _T("");
//DEL 	m_strDBInfoAfter = _T("");
//DEL 	m_bDBInfoMode = FALSE;
//DEL 	CString strLine, strTag, strData;
//DEL 	while(TRUE) {
//DEL 		if (fd.ReadString(strLine) == FALSE) break;
//DEL 		strLine.TrimLeft();				strLine.TrimRight();
//DEL 		if (strLine.GetLength() == 0) continue;
//DEL 
//DEL 		INT nPos = strLine.Find('=', 0);
//DEL 		if (nPos == -1) continue;
//DEL 
//DEL 		strTag = strLine.Left(nPos);
//DEL 		strTag.MakeUpper();			strTag.TrimLeft();		strTag.TrimRight();
//DEL 		strData = strLine.Mid(nPos+1);
//DEL 		strData.TrimLeft();			strData.TrimRight();
//DEL 
//DEL 		if (strTag == _T("MODE")) {
//DEL 			strData.MakeUpper();
//DEL 			if (strData == _T("TRUE")) m_bDBInfoMode = TRUE;
//DEL 				else m_bDBInfoMode = FALSE;
//DEL 		}
//DEL 		if (strTag == _T("BEFORE")) m_strDBInfoBefore = strData;
//DEL 		if (strTag == _T("AFTER")) m_strDBInfoAfter = strData;
//DEL 	}
//DEL 	if (m_strDBInfoBefore.GetLength() == 0) return -2;
//DEL 	if (m_strDBInfoAfter.GetLength() == 0) return -3;
//DEL 
//DEL 	return 0;
//DEL }
//DEL 
//DEL INT CEtcSetupPage::ConvertDBInfo_Save()
//DEL {
//DEL 	CButton *pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
//DEL 	CButton *pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
//DEL 	if (pButton1 == NULL || pButton2 == NULL) return -1;
//DEL 	
//DEL 	if (pButton1->GetCheck() == FALSE && pButton2->GetCheck() == FALSE) {
//DEL 		DeleteFile(m_strDBInfoCfgFile);
//DEL 		return 0;
//DEL 	}
//DEL 
//DEL 	if (pButton1->GetCheck() == TRUE) m_bDBInfoMode = TRUE;
//DEL 	if (pButton2->GetCheck() == TRUE) m_bDBInfoMode = FALSE;
//DEL 	CString strDBInfoMode;
//DEL 	if (m_bDBInfoMode) strDBInfoMode = _T("TRUE");
//DEL 		else strDBInfoMode = _T("FALSE");
//DEL 
//DEL 	CStdioFile fd;
//DEL 	if (fd.Open(m_strDBInfoCfgFile, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) == FALSE) return -1;
//DEL 	
//DEL 	if( 0 == fd.GetLength() )
//DEL 	{			
//DEL 		TCHAR cUni = 0xFEFF;
//DEL 		fd.Write( &cUni, sizeof(TCHAR));
//DEL 	}	
//DEL 
//DEL 	CString strLine;
//DEL 
//DEL 	strLine.Format(_T("MODE = %s\r\n"), strDBInfoMode);
//DEL 	fd.WriteString(strLine);
//DEL 
//DEL 	strLine.Format(_T("BEFORE = %s\r\n"), m_strDBInfoBefore);
//DEL 	fd.WriteString(strLine);
//DEL 
//DEL 	strLine.Format(_T("AFTER = %s\r\n"), m_strDBInfoAfter);
//DEL 	fd.WriteString(strLine);
//DEL 	
//DEL 	fd.Close();
//DEL 
//DEL 	return 0;
//DEL }
//--2009.01.19 ADD BY CJY --}}

VOID CEtcSetupPage::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	SetModified();
}

VOID CEtcSetupPage::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	SetModified();
}

VOID CEtcSetupPage::OnChangeeAUTOEXTRACTVERSION() 
{
	SetModified();
}

VOID CEtcSetupPage::OnChangeeAUTOEXTRACTPK() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	SetModified();
}
