// ESLDlg_GridConfig_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_GridConfig_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_GridConfig_INSERT dialog


CESLDlg_GridConfig_INSERT::CESLDlg_GridConfig_INSERT(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_GridConfig_INSERT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESLDlg_GridConfig_INSERT)
	m_eWidth = 0;
	m_eTITLE = _T("");
	//}}AFX_DATA_INIT
	mode = 1;
	nALIAS = 0;

	// KOL.UDF.022
	INT i=0;
	m_maxCol = 0;
	rCOL = 0;
	rALIGN = 0;
	rALIAS = _T("");
	for(i=0;i<512;i++)
		ALIAS[i] = _T("");
}


VOID CESLDlg_GridConfig_INSERT::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_GridConfig_INSERT)
	DDX_Control(pDX, IDC_COM_cALIAS, m_cAlias);
	DDX_Control(pDX, IDC_COM_cALIGN, m_cAlign);
	DDX_Text(pDX, IDC_COM_eWIDTH, m_eWidth);
	DDV_MinMaxUInt(pDX, m_eWidth, 0, 500);
	DDX_Text(pDX, IDC_COM_eTITLE, m_eTITLE);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CESLDlg_GridConfig_INSERT, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_GridConfig_INSERT)
	ON_BN_CLICKED(IDC_COM_bOK, OnbOK)
	ON_BN_CLICKED(IDC_COM_bCANCEL, OnbCANCEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_GridConfig_INSERT message handlers

VOID CESLDlg_GridConfig_INSERT::OnbOK() 
{
EFS_BEGIN

	UpdateData(true);
	m_cAlias.GetLBText(m_cAlias.GetCurSel(), rALIAS);
	if (m_cAlign.GetCurSel() == 0) rALIGN = 1;
	if (m_cAlign.GetCurSel() == 1) rALIGN = 4;
	if (m_cAlign.GetCurSel() == 2) rALIGN = 7;

	CDialog::OnOK();	

EFS_END
}

VOID CESLDlg_GridConfig_INSERT::OnbCANCEL() 
{
EFS_BEGIN

	CDialog::OnCancel();	

EFS_END
}

BOOL CESLDlg_GridConfig_INSERT::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 2009.10.19 ADD BY PDJ : Load Except Column name
	CString strExpColFile;
	CString strLine;
	CStringArray nExpColList;
	
	
	nExpColList.RemoveAll();
	strExpColFile = _T("..\\CFG\\Grid\\ExceptCol.CFG");
	CFileFind ff;
	if ( ff.FindFile(strExpColFile) )
	{
		CStdioFile fd;
		if (fd.Open(strExpColFile, CFile::modeRead|CFile::typeBinary) == TRUE) 		
		{
			TCHAR cUni;
			fd.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fd.SeekToBegin();
			}
			
			while(TRUE) 
			{
				if (fd.ReadString(strLine) == FALSE) break;							
				strLine.TrimLeft();			
				strLine.TrimRight();
				if (strLine.GetLength() > 0) nExpColList.Add(strLine);			
			}
		}	
	}

	INT	i,y;
	BOOL bAddCol;

	m_cAlign.AddString(_T("왼쪽"));
	m_cAlign.AddString(_T("가운데"));
	m_cAlign.AddString(_T("오른쪽"));
	if (mode == 1) m_cAlign.SelectString(0, _T("왼쪽"));
	if (mode == 2 && rALIGN == 1) m_cAlign.SelectString(0, _T("왼쪽"));
	if (mode == 2 && rALIGN == 4) m_cAlign.SelectString(0, _T("가운데"));
	if (mode == 2 && rALIGN == 7) m_cAlign.SelectString(0, _T("오른쪽"));
	m_cAlias.AddString(_T(""));
	for (i = 0; i < nALIAS; i++) {		

		bAddCol = TRUE;
		// Check Col Alias
		for ( y=0 ; y<nExpColList.GetSize() ; y++)
		{
			strLine = nExpColList.GetAt(y);
			if ( ALIAS[i] == strLine ) 
			{
				bAddCol = FALSE;
				break;		
			}
		}
		if ( bAddCol ) m_cAlias.AddString(ALIAS[i]);
	}
	if (mode == 1) m_cAlias.SetCurSel(0);
	//=====================================================================
	//사용자 환경설정시 MARC 관련 내용을 입력해준다.
	//2004-03-05 [이학중 수정]
	//if (mode == 2) 	m_cAlias.SelectString(0, rALIAS);
	if (mode == 2) 
	{
        INT nPos = rALIAS.Find (_T("MARC.")) ;
		if(nPos!=-1)
		{
			m_cAlias.AddString (rALIAS);
		}
		m_cAlias.SelectString(0, rALIAS);
	}
	//=====================================================================
	if (mode == 1) m_eWidth = 100;

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}
