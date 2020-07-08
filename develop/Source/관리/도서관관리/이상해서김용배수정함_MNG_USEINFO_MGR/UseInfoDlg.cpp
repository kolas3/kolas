// UseInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UseInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUseInfoDlg dialog

CUseInfoDlg::CUseInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUseInfoDlg)
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_bInsert = TRUE;
	AfxInitRichEdit();
}

VOID CUseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUseInfoDlg)
	DDX_Control(pDX, IDC_TGroup2, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUseInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CUseInfoDlg)
	ON_CBN_SELCHANGE(IDC_cManage, OnSelchangecManage)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TGroup2, OnSelchangeTGroup2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUseInfoDlg message handlers

BOOL CUseInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_MNG_USEINFO_MGR")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!!")); 
	}
	m_pCM = FindCM(_T("CM_MNG_USEINFO_MGR"));
	m_pDM = FindDM(_T("DM_MNG_USEINFO_MGR"));
	m_ctrlTab.InsertItem(0, _T("공지사항"));
	m_ctrlTab.InsertItem(1, _T("이용안내"));
	InitManage();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUseInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CUseInfoDlg::OnModify()
{
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("관리구분"));
	if (pCMB == NULL) return;

	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	CString strManage;
//	CString strDate;
	CString strInfo;
	CString query;
	CString strRecKey;
	m_pCM->GetControlMgrData(_T("정보내용"), strInfo);
	if(strInfo.IsEmpty())
	{
		ESLAfxMessageBox(_T("내용을 입력해 주십시요."));
		return ;
	}
	

	if(m_pDM->GetRowCount() == 0)
		m_bInsert = TRUE;
	else 
		m_bInsert = FALSE;


	INT num = m_ctrlTab.GetCurSel(); 
	if (num == 0)
	{	
		m_pCM->GetControlMgrData(_T("관리구분"), strManage);
//		m_pCM->GetControlMgrData(_T("등록일"), strDate);
		m_pCM->GetControlMgrData(_T("정보내용"), strInfo);
	
		// 데이터가 없을 경우 입력하는 부분. 	m_bInsert는 pDM->GetRowCount() = 0 일경우 TRUE	
		if(m_bInsert)   
		{
			m_pDM->InsertRow(-1);
			m_pDM->MakeRecKey(strRecKey);
			m_pDM->SetCellData(_T("REC_KEY"), strRecKey, 0);
			m_pDM->SetCellData(_T("FIRST_WORK"), this->GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), 0);
		}
		else
			strRecKey = m_pDM->GetCellData(_T("REC_KEY"), 0);
		m_pDM->SetCellData(_T("MANAGE_CODE"), strManage, 0);
//		m_pDM->SetCellData(_T("ANN_UPDATE_DATE"), strDate, 0);
		m_pDM->SetCellData(_T("ANNOUNCEMENT"), strInfo, 0);
		m_pDM->SetCellData(_T("LAST_WORK"), this->GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), 0);

		m_pDM->StartFrame(); 
		if(m_bInsert)
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
		else
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	}
	else if (num == 1)
	{
		m_pCM->GetControlMgrData(_T("관리구분"), strManage);
//		m_pCM->GetControlMgrData(_T("등록일"), strDate);
		m_pCM->GetControlMgrData(_T("정보내용"), strInfo);
		
        // 데이터가 없을 경우 입력하는 부분 
		if(m_bInsert)
		{
			m_pDM->InsertRow(-1);
			m_pDM->MakeRecKey(strRecKey);
			m_pDM->SetCellData(_T("REC_KEY"), strRecKey, 0);
			m_pDM->SetCellData(_T("FIRST_WORK"), this->GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), 0);
		}
		else 
			strRecKey = m_pDM->GetCellData(_T("REC_KEY"), 0);
		m_pDM->SetCellData(_T("MANAGE_CODE"), strManage, 0);
//		m_pDM->SetCellData(_T("GUI_UPDATE_DATE"), strDate, 0);
		m_pDM->SetCellData(_T("GUIDE"), strInfo, 0);
		m_pDM->SetCellData(_T("LAST_WORK"), this->GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), 0);

		m_pDM->StartFrame(); 
		if(m_bInsert)
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
		else
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);	
	}

	INT ids = m_pDM->SendFrame();
	if(ids < 0) 
		ESLAfxMessageBox(_T("변경내용을 저장할 수 없습니다."));
	ESLAfxMessageBox(_T("변경내용을 저장하였습니다."));
	return;
	Display();
}

VOID CUseInfoDlg::OnSelchangecManage() 
{
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("관리구분"));
	if (pCMB == NULL) return;

	CString strManage;
	m_pCM->GetControlMgrData(_T("관리구분"), strManage);
	if (strManage.IsEmpty()) 
	{
		pCMB->SetCurSel(m_nMANAGE_CODE);
		AfxMessageBox(_T("[관리구분별도서관설정]이 'Y'일때는 적용안함은 설정할 수 없습니다."));

		m_pCM->SetControlMgrData(_T("관리구분"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
//		return;
	}
	CString strManageCode;
	m_pCM->GetControlMgrData(_T("관리구분"), strManageCode);
	RefreshDM(strManageCode);
	Display();
}

VOID CUseInfoDlg::InitManage()
{
	CString strTmp, strQuery;
	strTmp = _T("");
	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='관리구분별도서관설정' "));
	m_pDM->GetOneValueQuery(strQuery, strTmp);
	strTmp.TrimLeft();				strTmp.TrimRight();
	strTmp.MakeUpper();
	if (strTmp == _T("N")) 
		m_cUseMANAGE_CODE = 'N';
	else 
		m_cUseMANAGE_CODE = 'Y';   // m_cUseMANAGE_CODE : 관리구분별도서관설정 Y/N

	strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID='%s'"), GetUserID());
	m_pDM->GetOneValueQuery(strQuery, m_strUSER_MANAGE_CODE);    // m_strUSER_MANAGE_CODE : 관리구분별도서관 
	m_strUSER_MANAGE_CODE.TrimLeft();			m_strUSER_MANAGE_CODE.TrimRight();
	// 관리구분을 가져온다.
	CString strManageCode;
	m_pCM->GetControlMgrData(_T("관리구분"), strManageCode);   // strManageCode : 관리구분별도서관기호
	RefreshDM(strManageCode);
	Display();
}  

VOID CUseInfoDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CStatic *pStatic;

	pStatic = ( CStatic * )GetDlgItem( IDC_SGroup2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 9, 3, cx- 16, 45 );

	pStatic = ( CStatic * )GetDlgItem( IDC_TGroup2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 9, 53, cx - 16, cy - 60);

	pStatic = ( CStatic * )GetDlgItem( IDC_eInfo2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 17, 80, cx - 31, cy - 95 );	
}

INT CUseInfoDlg::RefreshDM(CString strManageCode)
{
	CString strWhere;
	if(m_cUseMANAGE_CODE == _T("Y"))
	{
		strWhere.Format(_T("MANAGE_CODE = '%s'"), strManageCode);
	}
	else
	{
		strWhere.Format(_T("MANAGE_CODE IS NULL"));
	}

	if(m_pDM == NULL) return -1;
	INT ids = m_pDM->RefreshDataManager(strWhere);
	if(ids < 0) return -1;
	
	CString strCheck;
//	m_pDM->GetCellData(_T("정보내용"), strCheck);
//	if (strCheck.IsEmpty())
	if(m_pDM->GetRowCount() == 0)
		m_bInsert = TRUE;
	else 
		m_bInsert = FALSE;
	return 0;
}

VOID CUseInfoDlg::Display()
{
	if(m_cUseMANAGE_CODE == _T("Y"))
		GetDlgItem(IDC_cManage)->EnableWindow(TRUE);
	else 
	{
		GetDlgItem(IDC_cManage)->EnableWindow(FALSE);
		m_pCM->SetControlMgrData(_T("관리구분"), _T(""));
	}
	CString strDate; 
	CTime t = m_pDM->GetDBTime();
	strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	INT nCurSel = m_ctrlTab.GetCurSel();
	if(m_bInsert) 
	{
		if(nCurSel == 0)
		{
			m_pCM->SetControlMgrData(_T("정보내용"), m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0));
			m_pCM->SetControlMgrData(_T("등록일"), strDate, 0);
		}
		else
		{
			m_pCM->SetControlMgrData(_T("정보내용"), m_pDM->GetCellData(_T("GUIDE"), 0));
			m_pCM->SetControlMgrData(_T("등록일"), strDate, 0);
		}
		return;
	}
	if(nCurSel == 0)
	{
		m_pCM->SetControlMgrData(_T("정보내용"), m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0));
		m_pCM->SetControlMgrData(_T("등록일"), strDate, 0);
		m_pCM->SetControlMgrData(_T("관리구분"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
	}
	else
	{
		m_pCM->SetControlMgrData(_T("정보내용"), m_pDM->GetCellData(_T("GUIDE"), 0));
		m_pCM->SetControlMgrData(_T("등록일"), strDate, 0);
		m_pCM->SetControlMgrData(_T("관리구분"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
	}
}	

VOID CUseInfoDlg::OnSelchangeTGroup2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	Display();
	*pResult = 0;
}
