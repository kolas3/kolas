// Request_MailSendList.cpp : implementation file
//

#include "stdafx.h"
#include "Request_MailSendList.h"
#include "../../../공통/BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_MailSendList dialog


CRequest_MailSendList::CRequest_MailSendList(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequest_MailSendList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_MailSendList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pPrintDlg = NULL;
	m_pGrid = NULL;
	m_pSortItemSet = NULL;
}
CRequest_MailSendList::~CRequest_MailSendList()
{
}

VOID CRequest_MailSendList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_MailSendList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_MailSendList, CDialog)
	//{{AFX_MSG_MAP(CRequest_MailSendList)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnREQ_MAILSEND_FIND, OnbtnREQMAILSENDFIND)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnDATA_SORT, OnbtnDATASORT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_MailSendList message handlers

VOID CRequest_MailSendList::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CRequest_MailSendList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("K2UPGRADE_메일발송현황")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	GetWindowRect(&m_rcInitial);
	m_pCM = FindCM(_T("CM_메일발송현황"));
	m_pDM = FindDM(_T("DM_메일발송현황"));
	m_pGrid = (CESL_Grid *)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	if(m_pPrintDlg == NULL)
	{
		ESLAfxMessageBox(_T("출력 정보가 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("메일발송 리스트");
	}
	InitSortItem();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequest_MailSendList::SearchDM()
{
	CString mailSendDate1, mailSendDate2, strManageCode;
	m_pCM->GetControlMgrData(_T("메일발송일1"), mailSendDate1);
	m_pCM->GetControlMgrData(_T("메일발송일2"), mailSendDate2);
	m_pCM->GetControlMgrData(_T("관리구분"), strManageCode);
	CString strWhere, strOption;

	strWhere = _T(" REC_KEY IS NOT NULL");

	if(!strManageCode.IsEmpty())
	{
		strOption.Format(_T(" AND MANAGE_CODE = '%s'"), strManageCode);
		strWhere += strOption;
	}
	strOption.Empty();
	if(mailSendDate1.IsEmpty() && !mailSendDate2.IsEmpty())
	{
		ESLAfxMessageBox(_T("검색 시작일을 입력해 주세요."));
		return;
	}
	else if(!mailSendDate1.IsEmpty() && mailSendDate2.IsEmpty())
		strOption.Format(_T(" AND MAIL_SEND_DATE = '%s'"), mailSendDate1);
	else if(!mailSendDate1.IsEmpty() && !mailSendDate2.IsEmpty())
		strOption.Format(_T(" AND MAIL_SEND_DATE <= '%s' AND MAIL_SEND_DATE >= '%s'"), mailSendDate2, mailSendDate1);
	else
		strOption.Format(_T(" AND MAIL_SEND_DATE IS NOT NULL"));
	
	strWhere += strOption;
	m_pDM->RefreshDataManager(strWhere);

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1;
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData(_T("EMAIL"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *pApi = new CBO_LOC_3000Api(this);
		if(NULL != pApi)
		{
			if(TRUE == pApi->ConnectSocket()) 
			{
				pApi->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes1);
				for(i = 0; i < m_pDM->GetRowCount(); i++)
				{
					m_pDM->SetCellData(_T("EMAIL"), asDes1.GetAt(i), i);
				}
				pApi->CloseSocket();
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	DisplayGrid();
}


INT CRequest_MailSendList::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	if(strDMFieldAlias == _T("UDF_비치상태설명"))
	{
		if(str.GetLength() == 0) str = _T("적용안함");
		else if(str == _T("1")) str = _T("신청중");
		else if(str == _T("2")) str = _T("처리중");
		else if(str == _T("3")) str = _T("소장중");
		else if(str == _T("4")) str = _T("취소됨");
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

VOID CRequest_MailSendList::DisplayGrid()
{
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	m_pGrid->Display();
}

INT MakeCopyDM(CESL_DataMgr * pSrcDM, CESL_DataMgr * pDstDM)
{
	INT ids = -1;

	pDstDM->TBL_NAME = pSrcDM->TBL_NAME;
	pDstDM->EXTRA_CONDITION = pSrcDM->EXTRA_CONDITION;
	pDstDM->CONDITION = pSrcDM->CONDITION;
	pDstDM->DB_MGR_ALIAS = pSrcDM->DB_MGR_ALIAS;
	

	pDstDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
	pDstDM->m_nCols = pSrcDM->RefList.GetCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pSrcDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) return -1;

		ids = pDstDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) return -1;
	}
	return 0;
}

VOID CRequest_MailSendList::OnOK() 
{
	CMakePrintDMMgr PrintMgr(this);

	PrintMgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pDM);

	INT ids = PrintMgr.SPFExecutePath(_T("일련번호설정"));
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("출력데이터에 오류가 있습니다."));
		return ;
	}

	m_pPrintDlg->SetPrintDM(m_pDM);
	m_pPrintDlg->Print();
}

VOID CRequest_MailSendList::OnbtnREQMAILSENDFIND() 
{
	// TODO: Add your control notification handler code here
	SearchDM();	
}

VOID CRequest_MailSendList::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 51;
	rect.bottom -= 3;
	rect.left += 3;
	rect.right -= 3;
	
	m_pGrid->MoveWindow(rect);			
}

VOID CRequest_MailSendList::OnbtnDATASORT() 
{
	// TODO: Add your control notification handler code here
	if(m_pSortItemSet == NULL || m_pDM == NULL) 
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		return;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		DisplayGrid();	
}

BOOL CRequest_MailSendList::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	pTmpData = m_pSortItemSet->GetHeadSortList();
	CMapStringToString * pMap = m_pSortItemSet->GetConvertUDFToRealAliasMap();
	pMap->SetAt(_T("UDF_비치상태설명"), _T("비치상태"));
	m_pSortItemSet->ReadSortItem();
	return TRUE;
}

HBRUSH CRequest_MailSendList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
