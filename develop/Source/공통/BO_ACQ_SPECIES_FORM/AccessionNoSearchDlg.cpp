// AccessionNoSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AccessionNoSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccessionNoSearchDlg dialog


CAccessionNoSearchDlg::CAccessionNoSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CAccessionNoSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccessionNoSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pDM_Book = NULL;
	m_pGrid = NULL;
	m_MapAccessionRecKey.RemoveAll();
}
CAccessionNoSearchDlg::~CAccessionNoSearchDlg()
{
}

VOID CAccessionNoSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccessionNoSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccessionNoSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CAccessionNoSearchDlg)
	ON_BN_CLICKED(IDC_btnACCNO_SEARCH, OnbtnACCNOSEARCH)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccessionNoSearchDlg message handlers

BOOL CAccessionNoSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_부록원부번호설정")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_부록원부번호설정"));	
	m_pDM = FindDM(_T("DM_부록원부번호설정"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("원부번호설정 화면을 초기화 할 수 없습니다."));
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CAccessionNoSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}
	
	m_strAccessionRecKey = m_pDM->GetCellData(_T("REC_KEY"), ids);
	m_strAccessionNo = m_pDM->GetCellData(_T("원부번호"), ids);

	CDialog::OnOK();
}

VOID CAccessionNoSearchDlg::OnbtnACCNOSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strQuery, strOption, strMaker, strMakeYear, strAcqCode, strRegNo1, strRegNo2, strRegCode, strRegNo, strTmpData;
	m_pCM->GetControlMgrData(_T("원부생성자"), strMaker);
	m_pCM->GetControlMgrData(_T("원부생성년도"), strMakeYear);
	m_pCM->GetControlMgrData(_T("수입구분"), strAcqCode);
	m_pCM->GetControlMgrData(_T("등록번호1"), strRegNo1);
	m_pCM->GetControlMgrData(_T("등록번호2"), strRegNo2);

	strMaker.TrimLeft();	strMaker.TrimRight();
	strMakeYear.TrimLeft();	strMakeYear.TrimRight();
	strAcqCode.TrimLeft();	strAcqCode.TrimRight();
	strRegNo1.TrimLeft();	strRegNo1.TrimRight();
	strRegNo2.TrimLeft();	strRegNo2.TrimRight();
	strQuery.TrimLeft();	strQuery.TrimRight();

	INT ids = -1;
	if(!strRegNo1.IsEmpty())
	{
		ids = RegNoFormatting(strRegNo1);
		if(ids < 0) return ;
	}
	if(!strRegNo2.IsEmpty())
	{
		ids = RegNoFormatting(strRegNo2);
		if(ids  < 0) return;
	}

	strQuery = _T("REC_KEY IS NOT NULL");
	if(!strMaker.IsEmpty())
	{
		strOption.Format(_T(" AND ACCESSION_REC_MAKER = '%s'"), strMaker);
		strQuery += strOption;
	}
	if(!strMakeYear.IsEmpty())
	{
		strOption.Format(_T(" AND ACCESSION_REC_MAKE_YEAR = '%s'"), strMakeYear);
		strQuery += strOption;
	}
	if(!strAcqCode.IsEmpty())
	{
		strOption.Format(_T(" AND ACQUISIT_CODE = '%s'"), strAcqCode);
		strQuery += strOption;
	}
	if(strRegNo1.IsEmpty() && !strRegNo2.IsEmpty())
	{
		ESLAfxMessageBox(_T("시작등록번호를 입력해 주세요."));
		return ;
	}
	else if(!strRegNo1.IsEmpty() && strRegNo2.IsEmpty())
	{
		strOption.Format(_T(" AND REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE)"), strRegNo1);
//DEL 		strOption.Format(_T(" AND REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO='%s')"), strRegNo1);
		strQuery += strOption;
	}
	else if(!strRegNo1.IsEmpty() && !strRegNo2.IsEmpty())
	{
		strOption.Format(_T(" AND REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO BETWEEN '%s' AND '%s' AND MANAGE_CODE = UDF_MANAGE_CODE)"), strRegNo1, strRegNo2);
//DEL 		strOption.Format(_T(" AND REC_KEY IN (SELECT ACCESSION_REC_KEY FROM BO_BOOK_TBL WHERE REG_NO BETWEEN '%s' AND '%s')"), strRegNo1, strRegNo2);
		strQuery += strOption;
	}

	strQuery += _T(" AND MANAGE_CODE = UDF_MANAGE_CODE");

	ids = MakeDM(strQuery);
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("원부정보를 검색할 수 없습니다."));
	}

	return;
}

BOOL CAccessionNoSearchDlg::Init()
{
	if(m_pDM_Book == NULL) return FALSE;
	
	CString strAccessionRecKey, strTmpKey, strTmpValue, strQuery;

	INT nCnt = m_pDM_Book->GetRowCount();
	for(INT i=0 ; i<nCnt ; i++)
	{
		strTmpKey = m_pDM_Book->GetCellData(_T("BB_원부KEY"), i);
		if(!strTmpKey.IsEmpty())
			m_MapAccessionRecKey.SetAt(strTmpKey, _T(""));
	}
	
	if(m_MapAccessionRecKey.GetCount() == 0) return TRUE;

	POSITION pos = m_MapAccessionRecKey.GetStartPosition();
	while( pos != NULL )
	{
		m_MapAccessionRecKey.GetNextAssoc(pos, strTmpKey, strTmpValue);
		if(strAccessionRecKey.IsEmpty()) strAccessionRecKey = strTmpKey;
		else strAccessionRecKey += _T(", ") + strTmpKey;
	}
	
	strQuery.Format(_T("REC_KEY IN (%s)"), strAccessionRecKey);
	INT ids = MakeDM(strQuery);
	if(ids < 0) return FALSE;

	return TRUE;
}

INT CAccessionNoSearchDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
/*	INT ids;

		if(strDMFieldAlias == _T("등록번호"))
		{
			CString strTmpKey = m_pDM->GetCellData(_T("REC_KEY"), nRow);
			if(strTmpKey.IsEmpty()) return 0;
			ids = SetRegNoInfo(strTmpKey, str);
			if(ids < 0) return 0;
			m_pGrid->SetRowHeight(nRow+1, 200 * ids );
			m_pDM->SetCellData(strDMFieldAlias, str, nRow);
		}
		else */
	if(strDMFieldAlias == _T("수입구분"))
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("입수구분"), str, str);
	}
	return 0;
}

INT CAccessionNoSearchDlg::MakeDM(CString strQuery)
{
	INT ids = m_pDM->RefreshDataManager(strQuery);
	if(ids < 0) return -1;
	ids = m_pGrid->Display();
	if(ids < 0) return -1;
	ids = SetRegNoInfo();
	return 0;
}

INT CAccessionNoSearchDlg::SetRegNoInfo()
{
	INT ids;
	CString strTmpKey, strRegNoInfo;
	m_pGrid->ShowWindow(SW_HIDE);
	for(INT i=0 ; i<m_pDM->GetRowCount() ; i++)
	{
		strTmpKey = m_pDM->GetCellData(_T("REC_KEY"), i);
		if(strTmpKey.IsEmpty()) continue;
		ids = SetRegNoInfo(strTmpKey, strRegNoInfo);
		if(ids < 0) continue;
		m_pGrid->SetRowHeight(i+1, 200 * ids );
		m_pGrid->SetTextMatrix(i+1, 6, strRegNoInfo);
	}
	m_pGrid->ShowWindow(SW_SHOW);

	return 0;
}

INT CAccessionNoSearchDlg::SetRegNoInfo(CString strAccessionKey, CString &strRegNoInfo)
{
	if(strAccessionKey.IsEmpty()) return 0;
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	CString strQuery;
	strQuery.Format(
		_T("SELECT b.accession_rec_key ,B.REG_CODE, MIN (reg_no) || ' ~ ' || MAX (reg_no) ")
		_T("FROM bo_book_tbl b, co_accession_rec_tbl a WHERE a.rec_key = b.accession_rec_key ")
		_T("AND b.MANAGE_CODE = UDF_MANAGE_CODE AND a.rec_key = %s GROUP BY b.accession_rec_key, b.reg_code"), strAccessionKey);
//DEL 		_T("AND a.rec_key = %s GROUP BY b.accession_rec_key, b.reg_code"), strAccessionKey);
	INT ids = tmpDM.RestructDataManager(strQuery);
	if(ids < 0) return -1;

	strRegNoInfo.Empty();
	for(INT i=0 ; i<tmpDM.GetRowCount() ; i++)
	{
		if(strRegNoInfo.IsEmpty()) strRegNoInfo = tmpDM.GetCellData(i, 2);
		else strRegNoInfo += _T("\n") + tmpDM.GetCellData(i, 2);
	}
	return tmpDM.GetRowCount();
}

INT CAccessionNoSearchDlg::RegNoFormatting(CString &strRegNo)
{
	CString strRegCode, strTmpData;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strRegCode = strRegNo.Left(2);
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	strRegCode = strRegNo.Left(nRegCodeLength);
	//===================================================================
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("등록구분"), strRegCode, strTmpData);
	if(strTmpData.IsEmpty())
	{
		strTmpData.Format(_T("입력하신 등록번호[%s]는 올바르지 않습니다."), strRegNo);
		ESLAfxMessageBox(strTmpData);
		return -1;
	}
	strRegNo.Replace(strRegCode, _T(""));
	for(INT i=0 ; i<strRegNo.GetLength() ; i++)
	{
		if(!isdigit(strRegNo.GetAt(i)))
		{
			strTmpData.Format(_T("입력하신 등록번호[%s]는 올바르지 않습니다."), strRegNo);
			ESLAfxMessageBox(strTmpData);
			return -1;
		}
	}
	// 2004.10.14 등록번호 큰숫자 오류 수정.
	/*INT nRegNo = _ttoi(strRegNo.GetBuffer(0));
	strRegNo.Format(_T("%s%010d"), strRegCode, nRegNo);*/
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	__int64 nRegNo = _ttoi64(strRegNo.GetBuffer(0));
// 	strRegNo.Format(_T("%s%010I64d"), strRegCode, nRegNo);
	strRegNo = m_pInfo->MakeRegNo(strRegCode, strRegNo);
	//===================================================================


	return 0;
}

HBRUSH CAccessionNoSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}
