// SE_DataSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SE_DataSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_DataSearchDlg dialog


CSE_DataSearchDlg::CSE_DataSearchDlg(CESL_Mgr* pParent /*=NULL*/)
	: CBO_SearchDlg(CSE_DataSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_DataSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL; 
	m_bShowMode = FALSE;

	// openbase flag	
	CFileFind ff;
	if ( ff.FindFile(_T("..\\cfg\\_OPENBASE")) )
	{
		m_bOpenbase = TRUE;
	}
	else
	{
		m_bOpenbase = FALSE;
	}
}
CSE_DataSearchDlg::~CSE_DataSearchDlg()
{
}

VOID CSE_DataSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_DataSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_DataSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CSE_DataSearchDlg)
	ON_BN_CLICKED(IDC_btnSE_SHOW_HIDE, OnbtnSESHOWHIDE)
	ON_BN_CLICKED(IDC_btnSE_CLEAR, OnbtnSECLEAR)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_DataSearchDlg message handlers

BOOL CSE_DataSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_연속자료검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_연속자료검색"));	

	GetWindowRect(m_rcWindow);
	ShowControlByShowMode();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSE_DataSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		GetDlgItem(IDOK)->SetFocus();
		PostMessage(WM_COMMAND, IDOK, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CSE_DataSearchDlg::ShowControlByShowMode()
{
	INT i = 0;
	INT nCodeResourcesHeight = 113;
	CWnd *pWnd = NULL;

	if (m_bShowMode)
		GetDlgItem(IDC_btnSE_SHOW_HIDE)->SetWindowText(_T("간략검색"));
	else
		GetDlgItem(IDC_btnSE_SHOW_HIDE)->SetWindowText(_T("상세검색"));


	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_bShowMode && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_stcGROUP_HIDE;
//  	nArrCodeResourceID[nCount++] = IDC_stcSE_MANAGE_CODE;
// 	nArrCodeResourceID[nCount++] = IDC_cmbSE_MANAGE_CODE;
	nArrCodeResourceID[nCount++] = IDC_stcSE_CLASSNOTYPE;
	nArrCodeResourceID[nCount++] = IDC_cmbSE_CLASSNOTYPE;
	nArrCodeResourceID[nCount++] = IDC_stcSE_FORMCODE;
	nArrCodeResourceID[nCount++] = IDC_cmbSE_FORMCODE;
	nArrCodeResourceID[nCount++] = IDC_stcSE_MEDIA_CODE;
	nArrCodeResourceID[nCount++] = IDC_cmbSE_MEDIA_CODE;
	nArrCodeResourceID[nCount++] = IDC_stcSE_USEOBJCODE;
	nArrCodeResourceID[nCount++] = IDC_cmbSE_USEOBJCODE;
	nArrCodeResourceID[nCount++] = IDC_stcSE_USELIMITCODE;
	nArrCodeResourceID[nCount++] = IDC_cmbSE_USELIMITCODE;
	nArrCodeResourceID[nCount++] = IDC_stcSE_MATCODE;
	nArrCodeResourceID[nCount++] = IDC_cmbSE_MATCODE;
	nArrCodeResourceID[nCount++] = IDC_stcSE_SUBREGCODE;
	nArrCodeResourceID[nCount++] = IDC_cmbSE_SUBREGCODE;
	
	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowMode)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	if (m_bShowMode)
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
}

VOID CSE_DataSearchDlg::OnbtnSESHOWHIDE() 
{
	// TODO: Add your control notification handler code here
	if (m_bShowMode)
	{
		m_bShowMode = FALSE;
	}
	else
	{
		m_bShowMode = TRUE;
	}	
	ShowControlByShowMode();	
}

VOID CSE_DataSearchDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strRegNo1, strRegNo2, strControlNo1, strControlNo2, strTitle, strPublisher, strKeyword, strFreq, strISSN1, strISSN2, strISSN;
	CString  strMediaCode, strFormCode, strClassNoType, strMatCode, strSubRegCode, strUseObjCode, strUseLimitCode;
	CString strManageCode ;
	strManageCode = m_pInfo->MANAGE_CODE;

	m_pCM->GetControlMgrData(_T("서명"), strTitle);			TrimAll(strTitle);
	m_pCM->GetControlMgrData(_T("발행자"), strPublisher);		TrimAll(strPublisher);
	m_pCM->GetControlMgrData(_T("키워드"), strKeyword);		TrimAll(strKeyword);
	m_pCM->GetControlMgrData(_T("등록번호1"), strRegNo1);		TrimAll(strRegNo1);
	m_pCM->GetControlMgrData(_T("등록번호2"), strRegNo2);		TrimAll(strRegNo2);
	m_pCM->GetControlMgrData(_T("제어번호1"), strControlNo1);	TrimAll(strControlNo1);
	m_pCM->GetControlMgrData(_T("제어번호2"), strControlNo2);	TrimAll(strControlNo2);
	m_pCM->GetControlMgrData(_T("간행빈도"), strFreq);			TrimAll(strFreq);
	m_pCM->GetControlMgrData(_T("ISSN1"), strISSN1);			TrimAll(strISSN1);
	m_pCM->GetControlMgrData(_T("ISSN2"), strISSN2);			TrimAll(strISSN2);
	
	if(m_bShowMode)
	{
// 		m_pCM->GetControlMgrData(_T("관리구분"), strManageCode);	TrimAll(strManageCode);
		m_pCM->GetControlMgrData(_T("매체구분"), strMediaCode);		TrimAll(strMediaCode);
		m_pCM->GetControlMgrData(_T("형식구분"), strFormCode);		TrimAll(strFormCode);
		m_pCM->GetControlMgrData(_T("분류표구분"), strClassNoType);	TrimAll(strClassNoType);
		m_pCM->GetControlMgrData(_T("자료구분"), strMatCode);		TrimAll(strMatCode);
		m_pCM->GetControlMgrData(_T("보조등록구분"), strSubRegCode);	TrimAll(strSubRegCode);
		m_pCM->GetControlMgrData(_T("이용대상구분"), strUseObjCode);	TrimAll(strUseObjCode);
		m_pCM->GetControlMgrData(_T("이용제한구분"), strUseLimitCode);	TrimAll(strUseLimitCode);
	}
	
	//본표제, 저작자, 발행자 색인어 생성형식으로 변환
	pMakeSearchData->GetKeyWord(strTitle);	pMakeSearchData->ChangeHanjaToHangul(strTitle);
	pMakeSearchData->GetKeyWord(strPublisher);	pMakeSearchData->ChangeHanjaToHangul(strPublisher);
	pMakeSearchData->GetKeyWord(strKeyword);	pMakeSearchData->ChangeHanjaToHangul(strKeyword);
	strISSN = strISSN1 + _T("-") + strISSN2;
	if(strISSN == _T("-")) strISSN.Empty();
	
	CString strWhere = _T("I.REC_KEY = S.REC_KEY AND I.MANAGE_CODE = UDF_MANAGE_CODE");
	CString strBookWhere = _T("REC_KEY IS NOT NULL");
	CString strOption;
//#ifdef _ORA_CATSEARCH_
	if(m_pInfo->GetSearchIndexType() == ORA_CATSEARCH)
	{
		if(!strTitle.IsEmpty())
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_TITLE, '%s*', NULL)>0"), strTitle);
			strWhere += strOption;
		}
		if(!strPublisher.IsEmpty())
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_PUBLISHER, '%s*', NULL)>0"), strPublisher);
			strWhere += strOption;
		}
		if(!strKeyword.IsEmpty())
		{
			strOption.Format(_T(" AND CATSEARCH(I.IDX_KEYWORD, '%s*', NULL)>0"), strKeyword);
			strWhere += strOption;
		}
		if(!strISSN.IsEmpty())
		{
			strOption.Format(_T(" AND CATSEARCH(I.ST_ISSN, '%s*', NULL)>0"), strISSN);
			strWhere += strOption;
		}
		if(!strUseLimitCode.IsEmpty())
		{
			strOption.Format(_T(" AND CATSEARCH(I.USE_LIMIT_CODE, '%s*', NULL)>0"), strUseLimitCode);
			strWhere += strOption;
		}
//		if(!strManageCode.IsEmpty())
//		{
//			strOption.Format(_T(" AND CATSEARCH(I.MANAGE_CODE, '%s*', NULL)>0"), strManageCode);
//			strWhere += strOption;
//		}
		strOption.Format(_T(" AND CATSEARCH(I.MANAGE_CODE, '%s*', NULL)>0"), strManageCode);

	}
//#else
	else if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		m_arrECOSEARCH.RemoveAll();
		if(!strTitle.IsEmpty())
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, SE_IDX_TITLE, '%s*') "), strTitle);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!strPublisher.IsEmpty())
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, SE_IDX_PUBLISHER, '%s*') "), strPublisher);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!strKeyword.IsEmpty())
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, SE_IDX_KEYWORD, '%s*') "), strKeyword);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!strISSN.IsEmpty())
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, SE_ST_ISSN, '%s*') "), strISSN);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
		if(!strUseLimitCode.IsEmpty())
		{
			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, SE_USE_LIMIT_CODE, '%s*') "), strUseLimitCode);
			//strWhere += strOption;
			m_arrECOSEARCH.Add(strOption);
		}
//		if(!strManageCode.IsEmpty())
//		{
//			strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, SE_MANAGE_CODE, '%s*') "), strManageCode);
//			//strWhere += strOption;
//			m_arrECOSEARCH.Add(strOption);
//		}
		
		strOption.Format(_T(" AND &ECOSEARCH(I.REC_KEY, SE_MANAGE_CODE, '%s*') "), strManageCode);
		m_arrECOSEARCH.Add(strOption);
		
		if ( m_bOpenbase )
		{			
			for ( int i=0 ; i<m_arrECOSEARCH.GetSize() ; i++ )
			{
				strOption = m_arrECOSEARCH.GetAt(i);
				strOption.Replace(_T("AND"), _T("INTERSECT"));
				strWhere += strOption;
			}			
		}
		else
		{		
			INT ids = GetECOSEARCHQuery(strOption);
			if(ids < 0) 
			{
				ESLAfxMessageBox(_T("검색 쿼리를 생성할 수 없습니다."));
				return;
			}
			if(!strOption.IsEmpty())
			{
				strWhere += _T(" AND I.REC_KEY IN ( ") + strOption + _T(")");
			}
		}
	}
//#endif

	if(!strFreq.IsEmpty())
	{
		strOption.Format(_T(" AND I.PUB_FREQ = '%s'"), strFreq);
		strWhere += strOption;
	}
	strWhere += MakeRangeCondition_Stmt(_T("I.CONTROL_NO"), strControlNo1, strControlNo2);
	
	if(!strSubRegCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.SUB_REG_CODE = '%s'"), strSubRegCode);
		strWhere += strOption;
	}
	if(!strMediaCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.MEDIA_CODE = '%s'"), strMediaCode);
		strWhere += strOption;
	}
	if(!strFormCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.FORM_CODE = '%s'"), strFormCode);
		strWhere += strOption;
	}
	if(!strClassNoType.IsEmpty())
	{
		strOption.Format(_T(" AND I.CLASS_NO_TYPE = '%s'"), strClassNoType);
		strWhere += strOption;
	}
	if(!strMatCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.MAT_CODE = '%s'"), strMatCode);
		strWhere += strOption;
	}
	if(!strUseObjCode.IsEmpty())
	{
		strOption.Format(_T(" AND I.USE_OBJ_CODE = '%s'"), strUseObjCode);
		strWhere += strOption;
	}

	//책관련 Query생성
	strBookWhere += MakeRangeCondition_Stmt(_T("REG_NO"), strRegNo1, strRegNo2);
	if(!strBookWhere.IsEmpty())
	{
		strWhere += _T(" AND I.REC_KEY IN (SELECT SPECIES_KEY FROM SE_BOOK_TBL WHERE ") + strBookWhere + _T(")");
	}

	SetWhereStmt(strWhere);
	// 쿼리 생성후 부모에게 메시지를 보낸다.
	::PostMessage(pParentMgr->GetSafeHwnd(), QUERY_CREATE, 0, 0);
}

VOID CSE_DataSearchDlg::ShowGroupInfo()
{
	return;
}

VOID CSE_DataSearchDlg::OnbtnSECLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	ShowGroupInfo();
}

HBRUSH CSE_DataSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}