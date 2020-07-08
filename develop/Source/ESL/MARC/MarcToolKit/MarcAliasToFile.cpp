// MarcAliasToFile.cpp : implementation file
//

#include "stdafx.h"
#include "MarcAliasToFile.h"
#include "ESL_Mgr.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcAliasToFile dialog


CMarcAliasToFile::CMarcAliasToFile(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcAliasToFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcAliasToFile)
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT
}


VOID CMarcAliasToFile::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcAliasToFile)
	DDX_Text(pDX, IDC_eFILEPATH, m_strFilePath);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcAliasToFile, CDialog)
	//{{AFX_MSG_MAP(CMarcAliasToFile)
	ON_BN_CLICKED(IDC_bSTART, OnbSTART)
	ON_BN_CLICKED(IDC_bFILE, OnbFILE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcAliasToFile message handlers

BOOL CMarcAliasToFile::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);

	m_readCfgFile.ReadCfgFile(_T(""));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMarcAliasToFile::OnbFILE() 
{
	EFS_BEGIN

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = _T("CFG颇老(*.cfg)\0*.cfg\0葛电颇老(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("ALIAS CONFIG 颇老 历厘...");
	OFN.lpstrFile = lpstrFile;
	OFN.lpstrDefExt = _T("cfg");
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&OFN) != 0)
	{
		m_strFilePath = OFN.lpstrFile;
		GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);
	}	

	UpdateData(FALSE);
		

	EFS_END
}

VOID CMarcAliasToFile::OnbSTART() 
{
	EFS_BEGIN

	GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);
	
	CESL_DataMgr dm;
	CString strQuery = _T("SELECT ALIAS, TAG_CODE, ALIAS_TYPE, DATA FROM MARC_ALIAS_TBL");

	dm.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);
	dm.RestructDataManager(strQuery);

	INT nCount = dm.GetRowCount();
	if (nCount == 0) return;

	CStdioFile file;
	if (!file.Open(m_strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		return;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	for (INT i = 0; i < nCount; i++)
	{
		CString strLine = dm.GetCellData(i, 0) + _T("\t\t\t=") + dm.GetCellData(i, 1) + dm.GetCellData(i, 2) + dm.GetCellData(i, 3) + _T("\r\n");
		file.WriteString(strLine);
	}

	file.Close();	


	GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);

	EFS_END
}



