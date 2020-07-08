// KisaMarcImport.cpp : implementation file
//

#include "stdafx.h"
#include "KisaMarcImport.h"
#include <afx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKisaMarcImport dialog


CKisaMarcImport::CKisaMarcImport(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcImportDlg)
	m_nFileType = 1;
	m_strFilePath = _T("");
	//}}AFX_DATA_INIT

	m_CurrentCM = _T("");
	m_CurrentGrid = _T("");
	m_Se_Article_tbl = _T("");
	m_Se_Parent_Article_tbl = _T("");

}

CKisaMarcImport::~CKisaMarcImport()
{
	
}

VOID CKisaMarcImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcImportDlg)
	DDX_Radio(pDX, IDC_rEXCEL, m_nFileType);
	DDX_Text(pDX, IDC_eFILE_PATH, m_strFilePath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKisaMarcImport, CDialog)
	//{{AFX_MSG_MAP(CKisaMarcImport)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDD_bIMPORT, OnbIMPORT)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKisaMarcImport message handlers

BOOL CKisaMarcImport::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(InitESL_Mgr(_T("SM_KISAMARC_IMPORT"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!")) ;
		return FALSE ;
	}

	m_CurrentCM = _T("CM_KISAMARC_IMPORT") ;
    m_CurrentGrid = _T("GRID") ;
    m_Se_Article_tbl = _T("DM_KISAMARC_IMPORT") ;
	m_Se_Parent_Article_tbl = _T("DM_CONTENTMGR_SE_ARTICLE") ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CKisaMarcImport::OnbFILEOPEN() 
{

	UpdateData();

	CESL_DataMgr *pDM;
	pDM = FindDM( m_Se_Article_tbl );
	if ( pDM == NULL ) return;
	pDM->FreeData();


	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if (m_nFileType == 0)
	{
		OFN.lpstrFilter = _T("XLS파일(*.xls)\0*.xls\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("EXCEL 파일 열기");
	}
	else if(m_nFileType == 1)
	{
		OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("TEXT 파일 열기");
	}

	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);

		CStdioFile file;
		if (!file.Open(m_strFilePath, CFile::modeRead | CFile::typeBinary))
		{
			if (!file.Open(m_strFilePath, CFile::modeRead | CFile::modeCreate | CFile::typeBinary))
				return ;
			else
			{
				if( 0 == file.GetLength() )
				{			
					TCHAR cUni = 0xFEFF;
					file.Write( &cUni, sizeof(TCHAR));
				}
			}
		}
		
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}

		CString strStreamMarc;
		INT Idx = 0;
		if ( m_nFileType == 1) 
		{
			while (file.ReadString(strStreamMarc))
			{

				SetDM ( strStreamMarc , Idx );
				Idx++;
			}
		}
		else
		{
			CMarc *pMarc;
			pMarc = new CMarc;
			m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFileType, m_strFilePath);
			m_marcImpExpMgr.ImportFile();	
			ImportXlsFile( pMarc );

			CString strStreamMarc;
			m_pInfo->pMarcMgr->Encoding ( pMarc , strStreamMarc );
			AfxMessageBox(strStreamMarc);
		}
			
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM ( m_CurrentCM );
	pCM->AllControlDisplay();
	

}

VOID CKisaMarcImport::OnbIMPORT() 
{
	// 반입 
	CESL_DataMgr *pDM_Parent;
	pDM_Parent = FindDM ( m_Se_Parent_Article_tbl ) ;
	if ( pDM_Parent == NULL ) return;

	INT nCnt = pDM_Parent->GetRowCount();

	CESL_DataMgr *pDM;
	pDM = FindDM ( m_Se_Article_tbl ) ;
	if ( pDM == NULL ) return;

	CESL_ControlMgr *pCM = FindCM(m_CurrentCM) ;
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}

    CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl ( m_CurrentGrid ) ;
	if(pGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return ;
	}

	pGrid->SelectMakeList();
	INT nidx = pGrid->SelectGetHeadPosition();

	INT nParentDMRowCnt = pDM_Parent->GetRowCount();

	while ( nidx > -1 ) 
	{
		CString strItem;
		pDM_Parent->InsertRow(-1);
		pDM_Parent->SetCellData( _T("기사명"), pDM->GetCellData( _T("기사명"), nidx ), nParentDMRowCnt );
		pDM_Parent->SetCellData( _T("저자"), pDM->GetCellData( _T("저자"), nidx ), nParentDMRowCnt );
		pDM_Parent->SetCellData( _T("페이지"), pDM->GetCellData( _T("페이지"), nidx ), nParentDMRowCnt );
		pDM_Parent->SetCellData( _T("ARTICLE"), pDM->GetCellData( _T("ARTICLE"), nidx ), nParentDMRowCnt );
		pDM_Parent->SetCellData( _T("DB_FLAG"), _T("N") , nParentDMRowCnt );
		nParentDMRowCnt++;

		nidx = pGrid->SelectGetNext();
	}

//	CESL_ControlMgr *pCM_Parent = FindCM(_T("CM_CONTENTMGR_SE_EDIT")) ;
//	if(pCM_Parent == NULL) {
//		AfxMessageBox(_T("FindCM() Error~!")) ;
//		return ;
//	}
//	if ( pCM_Parent == NULL ) return;
//
//	pCM_Parent->AllControlDisplay();

	CDialog::OnOK();

}

VOID CKisaMarcImport::OnCancel() 
{
	CDialog::OnCancel();
}

VOID CKisaMarcImport::SetDM(CString strStreamMarc, INT Idx )
{

	//한 라인씩 읽어서 DM에 가지고 있기.
	CMarc *pMarc;
	pMarc = NULL;
	pMarc = new CMarc;

	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	
	//Marc Decoding
	m_pInfo->pMarcMgr->Decoding( strStreamMarc, pMarc ) ;

	CESL_DataMgr *pDM;
	pDM = FindDM( m_Se_Article_tbl );
	if ( pDM == NULL ) return;

	pDM->InsertRow(-1);

	CString strItem;
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$a"), strItem );
	pDM->SetCellData( _T("기사명"), strItem, Idx );
	
	m_pInfo->pMarcMgr->GetItem( pMarc, _T("245$d"), strItem );
	pDM->SetCellData( _T("저자"), strItem, Idx );

	pDM->SetCellData( _T("ARTICLE"), strStreamMarc, Idx );
		


}

INT CKisaMarcImport::ImportXlsFile( CMarc *pMarc )
{
	EFS_BEGIN

	C_ExcelManControl excelCtrl;
	CRect rect;
	m_pParentWnd->GetWindowRect(rect);

	excelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, m_pParentWnd, WM_USER + 10000);
	if (excelCtrl.m_hWnd == NULL) {
		AfxMessageBox(_T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오."));
		return -1;
	}
    CExcelManager excelMgr;
	excelMgr.SetManager(&excelCtrl);
		
    if(excelMgr.OpenManager() == FALSE) 
	{
        AfxMessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"));
		return -1;
	}

	excelMgr.SetUseTemplate(TRUE, m_strFilePath);
	if (excelMgr.OpenManager() == FALSE)
	{
		AfxMessageBox(_T("EXCEL Manager를 확인해 주세요."));
		return -1;
	}

	//마크구룹이 연속이면 간행빈도 설명 데이터를 먼저 가지고 있는다
	BOOL bIsSerial = FALSE;

	// Get Header
	m_arrayExcelHead.RemoveAll();
	
	CString strAlias;
	INT nColCount = 0;
	while (TRUE)
	{
		strAlias = excelMgr.GetTextMatrix(1, nColCount + 1);
		strAlias.TrimLeft();
		strAlias.TrimRight();
		strAlias.Replace(_T(" "), _T(""));
		if (strAlias.IsEmpty()) break;

		m_arrayExcelHead.Add(strAlias);
		nColCount++;
	}


	// Fetch Row
	CString strItem;
	CString strRow;
	CString strHeadAlias;
	INT nRowCount = 1;
	CString strPubFreqCode;

	while (TRUE)
	{
		
		strRow.Empty();
		pMarc->Init();
		for (INT i = 0; i < nColCount; i++)
		{
			strItem = excelMgr.GetTextMatrix(nRowCount + 1, i + 1);
			strHeadAlias = m_arrayExcelHead.GetAt(i);
			
			//modified by loveisverb 2004-03-12
			m_pInfo->pMarcMgr->SetItem(pMarc, m_arrayExcelHead.GetAt(i), strItem);
			strRow += strItem;
		}

		strRow.TrimLeft();
		strRow.TrimRight();
		if (strRow.IsEmpty()) break;
		nRowCount++;
		m_pInfo->pMarcMgr->CheckDeepSyntax(pMarc);
		m_pInfo->pMarcMgr->CheckSimpleSyntax(pMarc);
		m_pInfo->pMarcMgr->DeleteEmptySubfield(pMarc);
		m_pInfo->pMarcMgr->DeleteEmptyField(pMarc);
		
	}	
	
	excelMgr.CloseManager();
	
	return 0;

	EFS_END
	return -1;

}


HBRUSH CKisaMarcImport::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


