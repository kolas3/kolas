// UC_NotifyPopupDlg.cpp : implementation file


#include "stdafx.h"
#include "UC_NotifyPopupDlg.h"

// 2017/09/22 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// KOLIS-NET OpenAPI
/*//NEW------------------------------------------------------------------------*/
#include "KolisNetService.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "UC_NotifyPopup_FileDlg.h"

///////////////////////////////////////////////////////////////////////////
 //CUC_NotifyPopupDlg dialog


CUC_NotifyPopupDlg::CUC_NotifyPopupDlg(CESL_Mgr* pParent )
	: CESL_Mgr(CUC_NotifyPopupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_NotifyPopupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid = NULL;
	m_pDM = NULL;
}

CUC_NotifyPopupDlg::~CUC_NotifyPopupDlg()
{
	m_pGrid = NULL;
	if (m_pDM)
	{
		m_pDM->FreeData();
		m_pDM =NULL;
	}
}


VOID CUC_NotifyPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUC_NotifyPopupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUC_NotifyPopupDlg, CDialog)
	//{{AFX_MSG_MAP(CUC_NotifyPopupDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_bSAVE_FILE, OnbSAVEFILE)
	ON_BN_CLICKED(IDC_bCHECK_NOTIFY, OnbCHECKNOTIFY)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
 

BOOL CUC_NotifyPopupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_UC_NOTIFY_POPUP")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	

	CString m_LIB_CODE = GetLib_Code();
	if ( m_LIB_CODE == _T("") ) 
	{
		AfxMessageBox(_T("도서관 부호를 가져올 수 없습니다."));
		return FALSE;
	}


	CString strKolisNetUseYN;
	if (!CKolisNetService::GetLibraryKolisNetUseFlag(this, m_LIB_CODE, strKolisNetUseYN))
	{
		AfxMessageBox(_T("국립중앙도서관 공지사항을 가져오지 못하였습니다.\n\n국가자료공동목록 서버 설정을 확인하십시오.\n\n공동목록 메뉴 및 반입기능을 사용 할 수 없습니다."));
		CDialog::OnCancel();
		return FALSE;
	}

	if (strKolisNetUseYN.CompareNoCase(_T("N")) == 0 || strKolisNetUseYN.IsEmpty()) 
	{
		CDialog::OnCancel();	
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_UC_NOTIFY"));
	if (NULL == m_pDM) {
		AfxMessageBox(_T("DM 초기화 에러(DM_UC_NOTIFY)"));
		return FALSE;
	}

	CKolisNetDataMapper Mapper;
	Mapper.AddData(KOLISNET_PARAM_UN_KEY, _T("REC_KEY"));
	Mapper.AddData(KOLISNET_PARAM_SRFLAG, _T("SEND_RECV"));
	Mapper.AddData(KOLISNET_PARAM_RECV_LIB_CODE, _T("받을도서관부호"));
	Mapper.AddData(KOLISNET_PARAM_RECV_LIB_NAME, _T("받을도서관명"));
	Mapper.AddData(KOLISNET_PARAM_SEND_LIB_CODE, _T("보낸도서관부호"));
	Mapper.AddData(KOLISNET_PARAM_SEND_LIB_NAME, _T("보낸도서관명"));
	Mapper.AddData(KOLISNET_PARAM_CONFIRM, _T("확인"));
	Mapper.AddData(KOLISNET_PARAM_TITLE, _T("제목"));
	Mapper.AddData(KOLISNET_PARAM_CONTENTS, _T("내용"));
	Mapper.AddData(KOLISNET_PARAM_FILE_YN, _T("파일이름")); // 첨부된 첫번째 파일에 대한 내용은 파일유무로 대체
	Mapper.AddData(KOLISNET_PARAM_FILE_YN, _T("파일유무"));
	Mapper.AddData(KOLISNET_PARAM_TRANS_DATE, _T("전송날짜"));

	if (!CKolisNetService::GetKolisNetNotify(this, m_LIB_CODE, m_pDM, &Mapper))
	{
		AfxMessageBox(_T("국립중앙도서관 공지사항을 가져오지 못하였습니다.\n\n국가자료공동목록 서버 설정을 확인하십시오.\n\n공동목록 메뉴 및 반입기능을 사용 할 수 없습니다."));
		CDialog::OnCancel();
		return FALSE;
	}

	INT nCount = m_pDM->GetRowCount();
	if (0 == nCount)
	{
		CDialog::OnCancel();
		return FALSE;
	}

	for (INT i=0; i<nCount; i++)
	{
		CString strYN;
		m_pDM->GetCellData(_T("파일이름"), i, strYN);
		if (strYN.IsEmpty())
		{
			m_pDM->SetCellData(_T("파일이름"), _T("N"), i);
		}
	}

	// CM 초기화
	CESL_ControlMgr *pCM = FindCM(_T("CM_UC_NOTIFY"));
	if ( pCM == NULL )
	{
		AfxMessageBox(_T("DM 초기화 에러(DM_UC_NOTIFY)"));
		return FALSE;
	}

	// Grid 초기화
	m_pGrid = (CESL_Grid*)pCM->FindControl( _T("GRID_NOTIFY"));
	if ( m_pGrid == NULL )
	{
		AfxMessageBox(_T("GRID 초기화 에러(DM_UC_NOTIFY)"));
		return FALSE;
	}

	// grid display
	if( m_pGrid->Display() < 0 ) return FALSE;

	Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CUC_NotifyPopupDlg::InitRS_DM_MGR()
{
}

BOOL CUC_NotifyPopupDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	CDialog::Create(IDD, pParentWnd);
	
	// 검색 결과 없으면 return FALSE
	if ( m_pDM->GetRowCount() == 0 ) return FALSE;
	return TRUE;

	EFS_END
	return FALSE;
}

BEGIN_EVENTSINK_MAP(CUC_NotifyPopupDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUC_NotifyPopupDlg)
	ON_EVENT(CUC_NotifyPopupDlg, IDC_GRID_NOTIFY, -600 /* Click */, OnClickGridNotify, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

// 그리드 클릭시
VOID CUC_NotifyPopupDlg::OnClickGridNotify() 
{
	INT nIdx = m_pGrid->GetRow() -1;

	// 클릭한 데이터의 confirm update
	// UpdateConfirm(nIdx);

	// 화면 display
	Display(nIdx);
	
}

VOID CUC_NotifyPopupDlg::OnOk() 
{
	CDialog::OnOK();
}

// 그리드의 선택된 내용을 보여준다.
INT CUC_NotifyPopupDlg::Display(INT idx)
{
EFS_BEGIN

	CESL_ControlMgr * pCM = FindCM(_T("CM_UC_NOTIFY"));
	if ( pCM == NULL )
	{
		AfxMessageBox(_T("DM 초기화 에러(DM_UC_NOTIFY)"));
		return -1;
	}

	CString sSend_LibID;
	CString sSend_LibName;
	CString sRecv_LibID;
	CString sRecv_LibName;
	CString sTrans_Date;
	CString sTitle;
	CString sContents;
	CString sFile_Name;

	m_pDM->GetCellData(_T("보낸도서관부호"), idx, sSend_LibID);
	m_pDM->GetCellData(_T("보낸도서관명"), idx, sSend_LibName);
	m_pDM->GetCellData(_T("받을도서관부호"), idx, sRecv_LibID);
	m_pDM->GetCellData(_T("받을도서관명"), idx, sRecv_LibName);
	m_pDM->GetCellData(_T("전송날짜"), idx, sTrans_Date);
	m_pDM->GetCellData(_T("제목"), idx, sTitle);
	m_pDM->GetCellData(_T("내용"), idx, sContents);
	m_pDM->GetCellData(_T("파일이름"), idx, sFile_Name);

	// 도서관명(도서관ID)
	if ( sSend_LibID != _T("") ) sSend_LibName = sSend_LibName + _T("(") + sSend_LibID + _T(")");
	if ( sRecv_LibID != _T("") ) sRecv_LibName = sRecv_LibName + _T("(") + sRecv_LibID + _T(")");

	pCM->SetControlMgrData(_T("받는도서관"), sRecv_LibName);
	pCM->SetControlMgrData(_T("보낸도서관"), sSend_LibName);
	pCM->SetControlMgrData(_T("전송날짜"), sTrans_Date);
	pCM->SetControlMgrData(_T("제목"), sTitle);
	pCM->SetControlMgrData(_T("내용"), sContents);
	pCM->SetControlMgrData(_T("파일이름"), sFile_Name);


	if ( sFile_Name == _T("Y") ) {
		GetDlgItem(IDC_bSAVE_FILE)->EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_bSAVE_FILE)->EnableWindow(FALSE);
	}

	INT nCurSel = m_pGrid->GetTopRow();
	pCM->AllControlDisplay();
	m_pGrid->SetReverse(idx);
	m_pGrid->SetTopRow(nCurSel);

	return 0;

EFS_END
	return -1;
	
}

// 미확인 -> 확인 update
INT CUC_NotifyPopupDlg::UpdateConfirm(INT idx)
{
EFS_BEGIN
	
	CString sRec_Key;
	CString sConfirm;
	
	m_pDM->GetCellData(_T("REC_KEY"), idx, sRec_Key);
	m_pDM->GetCellData(_T("확인"), idx, sConfirm);

	// 이미 확인한 자료이면 return
	if ( sConfirm == _T("1")) return 1;


	if (!CKolisNetService::SetKolisNetNotifyConfirm(this, sRec_Key, m_pInfo->LIB_CODE, _T("1")))
	{
		return -1;
	}

	m_pDM->SetCellData(_T("확인"), _T("1"), idx);

EFS_END
	return -1;
}

// 도서관 부호 가져오기
CString CUC_NotifyPopupDlg::GetLib_Code()
{
	
	CString sLibCode=_T("");
	if (!m_pInfo->LIB_CODE.IsEmpty()) {
		sLibCode = m_pInfo->LIB_CODE;
	} else {
		CESL_DataMgr pDM;
		pDM.SetCONNECTION_INFO(_T(""));
		
		CString sQuery = _T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE");
		INT ids  = pDM.GetOneValueQuery(sQuery, sLibCode );
		if ( ids < 0 ) 
		{
			CString sQuery = _T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL");
			ids  = pDM.GetOneValueQuery(sQuery, sLibCode );
		}
	}

	return sLibCode;
}

BOOL CUC_NotifyPopupDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::Create(IDD, pParentWnd);

	// 검색 결과 없으면 return FALSE
	if ( m_pDM->GetRowCount() == 0 ) return FALSE;
	return TRUE;

}


VOID CUC_NotifyPopupDlg::OnbSAVEFILE() 
{
	INT nIdx = m_pGrid->GetRow() -1;

	CUC_NotifyPopup_FileDlg fileDlg(this);
	fileDlg.m_RowIndex = nIdx;
	fileDlg.DoModal();
}

VOID CUC_NotifyPopupDlg::OnbCHECKNOTIFY() 
{
	INT nIdx = m_pGrid->GetRow() -1;

	// 선택한 데이터의 confirm update
	UpdateConfirm(nIdx);
	
	AfxMessageBox(_T("확인처리 되었습니다."));
}

HBRUSH CUC_NotifyPopupDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CUC_NotifyPopupDlg::PreTranslateMessage(MSG* pMsg) 
{

	if (WM_KEYDOWN == pMsg->message) {
		if (VK_ESCAPE == pMsg->wParam) {
			OnCancel();
			return FALSE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
