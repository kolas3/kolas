#include "stdafx.h"
#include "KL_SearchCurrentLoanInfoDlg.h"
#include "KL_SearchCurrentLoanInfoDetailDlg.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_SearchCurrentLoanInfoDlg dialog


CKL_SearchCurrentLoanInfoDlg::CKL_SearchCurrentLoanInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_SearchCurrentLoanInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsRunningThread = FALSE;
	m_strUserNo = _T("");
}
CKL_SearchCurrentLoanInfoDlg::~CKL_SearchCurrentLoanInfoDlg()
{
}


void CKL_SearchCurrentLoanInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_SearchCurrentLoanInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_SearchCurrentLoanInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_SearchCurrentLoanInfoDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnDetailView, OnbtnDetailView)
	ON_BN_CLICKED(IDC_btnLOC_USERCLOSE, OnbtnCLOSE)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_RESULT_VIEW,ResultView)
	ON_MESSAGE(WM_SET_STATUS,SetMessage)
	ON_BN_CLICKED(IDC_btnRetry, OnbtnRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKL_SearchCurrentLoanInfoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKL_SearchCurrentLoanInfoDlg)
	ON_EVENT(CKL_SearchCurrentLoanInfoDlg, IDC_gridCurrentLoanInfo, -601 /* DblClick */, OnDblClickgridCurrentLoanInfo, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_SearchCurrentLoanInfoDlg message handlers

BOOL CKL_SearchCurrentLoanInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	if(0>InitESL_Mgr(_T("SM_KL_SearchCurrentLoanInfo")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	CESL_Grid*	pGrid	=	(CESL_Grid*)GetDlgItem( IDC_gridCurrentLoanInfo );
	INT nReserveCntIdx = -1;
	CLocCommonAPI::GetGridColumnIndexs( this , pGrid , _T("예약중인책수") , nReserveCntIdx );
	pGrid->SetColWidth( nReserveCntIdx+1 , 0 , 0 );

	if(!m_pInfo->IsConnectUnityLoanService())
	{
		SetDlgItemText(IDC_staStatus, _T("센터와 접속시도 중 입니다."));
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDetailView)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
		return TRUE; 
	}

	SetTimer(0, 100, NULL);
	::SetFocus(this->GetSafeHwnd());
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_SearchCurrentLoanInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnDetailView)->m_hWnd == pMsg->hwnd ) OnbtnDetailView();
			else if(GetDlgItem(IDC_btnLOC_USERCLOSE)->m_hWnd == pMsg->hwnd ) OnbtnCLOSE();
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			if(TRUE == m_bIsRunningThread) return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CKL_SearchCurrentLoanInfoDlg::OnTimer(UINT nIDEvent) 
{
	if(0 == nIDEvent)
	{
		KillTimer(0);
		AfxBeginThread(SearchLoanInfoThread, this);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CKL_SearchCurrentLoanInfoDlg::OnbtnRetry() 
{
	SetTimer(0, 100, NULL);
	::SetFocus(this->GetSafeHwnd());		
}

void CKL_SearchCurrentLoanInfoDlg::OnbtnDetailView() 
{
	CESL_DataMgr* pDM = FindDM(_T("DM_KL_SearchCurrentLoanInfo"));
	if(pDM == NULL) return;

	INT nRowCnt = pDM->GetRowCount();
	if(0>=nRowCnt) return;

	CString strLibCode;
	CESL_ControlMgr* pCM = FindCM(_T("CM_KL_SearchCurrentLoanInfo"));
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_SearchCurrentLoanInfo"), _T("grid"));
	INT nIdx = pGrid->GetIdx();
	
	CString strLoanCnt, strReserveCnt;
	INT ids = pDM->GetCellData(_T("총대출중인책수"), nIdx, strLoanCnt);
	if(0>ids) return;

	if( 0>=_ttoi(strLoanCnt) )
	{
		SetMessage(TRUE,_T("대출 현황이 없습니다."));		
		return;
	}

	SetMessage(TRUE,_T(""));
	ids = pDM->GetCellData(_T("가입도서관부호"), nIdx, strLibCode);
	if(0>ids) return;

	CKL_SearchCurrentLoanInfoDetailDlg dlg(this);
	dlg.SetInfo(m_strUserNo, strLibCode);
	dlg.DoModal();
}
	
void CKL_SearchCurrentLoanInfoDlg::OnbtnCLOSE() 
{
	if(TRUE == m_bIsRunningThread) return;

	CDialog::OnCancel();
}

void CKL_SearchCurrentLoanInfoDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;

	CDialog::OnClose();
}

UINT CKL_SearchCurrentLoanInfoDlg::SearchLoanInfoThread(LPVOID pParam)
{
	CKL_SearchCurrentLoanInfoDlg* pDlg = (CKL_SearchCurrentLoanInfoDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	if(!pDlg->m_pInfo->IsConnectUnityLoanService())
	{
		pDlg->SetDlgItemText(IDC_staStatus, _T("센터와 접속시도 중 입니다."));
		pDlg->GetDlgItem(IDC_btnRetry)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_btnDetailView)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
		return 0; 
	}

	pDlg->m_bIsRunningThread = TRUE;

	INT ids = pDlg->StartSearch();
	if(0==ids) pDlg->GetDlgItem(IDC_btnRetry)->ShowWindow(SW_HIDE);
	else pDlg->GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);

	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

INT CKL_SearchCurrentLoanInfoDlg::StartSearch()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("StartSearch");
	m_bIsRunningThread = TRUE;
	SetMessage(FALSE, _T("가입 된 도서관을 조회합니다."));

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K48_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);


	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0>ids) 
	{
		CString strMsg = Sender.getError(ids);
		if(strMsg.IsEmpty())
		{
			SetMessage(TRUE, _T("미들웨어와의 통신에 실패하였습니다."));
		}
		else
		{
			SetMessage(TRUE, strMsg);
		}
		return -1010;
	}

	CESL_DataMgr TempDM;
	Reader.makeDM(&TempDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp)
	{
		CString strMsg,strTemp;
		Reader.getError(strTemp, strMsg);
		if(0 < strTemp.Find(_T("DT")))
		{
			SetMessage(TRUE, _T("조회된 결과가 없습니다."));
			return -1020;
		}
		else
		{
			if(strMsg.IsEmpty())
			{
				SetMessage(TRUE, _T("미들웨어에서 에러가 발생하였습니다."));
			}
			else
			{
				SetMessage(TRUE, strMsg);
			}
			strMsg.Format(_T("통합회원가입도서관정보(5) ERROR CODE : %s"), strTemp);
			TempDM.QueryLog(strMsg.GetBuffer(0));
			return -1020;
		}
	}
	SetMessage(FALSE, _T("도서관 정보를 불러오는 중입니다."));
	
	ids = CopyDM(&TempDM);
	if(0>ids) 
	{
		SetMessage(TRUE, _T("Error가 발생하였습니다."));
		return -1030;
	}

	this->SendMessage(WM_RESULT_VIEW, 0, 0);
	SetMessage(TRUE, _T("조회가 완료되었습니다."));
	
	return 0;

EFS_END
return -1;
}

INT CKL_SearchCurrentLoanInfoDlg::CopyDM(CESL_DataMgr* TmpDm)
{
EFS_BEGIN

	INT ids;
	CString strData;
	const INT nColCnt = 6;
	CString strALias[nColCnt] =
	{
		_T("가입도서관부호"),	_T("도서관명"),		_T("대출중인책수"), 
		_T("예약중인책수"),		_T("대출정지일"),	_T("회원상태")
	};

	CESL_DataMgr* pDM = FindDM(_T("DM_KL_SearchCurrentLoanInfo"));
	if(NULL==pDM) return -1010;

	INT nCopyCnt = TmpDm->GetRowCount();
	for(INT i=0; i<nCopyCnt; i++)
	{
		ids = pDM->InsertRow(-1);
		if(0>ids) return -1020;

		for( INT j=0; j<nColCnt; j++ )
		{
			ids = TmpDm->GetCellData( strALias[j] , i , strData );
			if(0>ids) return -1030;

			if(5==j)
			{
				if(_T("0")==strData) strData = _T("정상회원");
				else if(_T("1")==strData) strData = _T("대출정지");
				else if(_T("2")==strData) strData = _T("제적회원");
				else if(_T("3")==strData) strData = _T("탈퇴회원");
				else strData = _T("정상회원");
			}
			if(2 == j)
			{
				int nLoanCnt = 0;
				TmpDm->GetCellData(_T("대출중인책수(책)"), i, strData);
				if(strData.IsEmpty()) strData = _T("0");
				nLoanCnt += _ttoi(strData);
				CString strLoanCnt = strData + _T("(");
				TmpDm->GetCellData(_T("대출중인책수(부록)"), i, strData);
				if(strData.IsEmpty()) strData = _T("0");
				nLoanCnt += _ttoi(strData);

				strLoanCnt += strData + _T(")");
				TmpDm->GetCellData(_T("총대출가능책수"), i, strData);
				strData = strLoanCnt;
				CString strTotCnt;
				strTotCnt.Format(_T("%d"), nLoanCnt);
				pDM->SetCellData(_T("총대출중인책수"), strTotCnt, i);
			}

			ids = pDM->SetCellData( strALias[j] , strData , i );
			if(0>ids) return -1040;
		}
	}

	return 0;

EFS_END
return -1;
}

VOID CKL_SearchCurrentLoanInfoDlg::ResultView()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_SearchCurrentLoanInfo"), _T("grid"));
	pGrid->Display();
}

void CKL_SearchCurrentLoanInfoDlg::OnDblClickgridCurrentLoanInfo() 
{
	OnbtnDetailView();
}

VOID CKL_SearchCurrentLoanInfoDlg::SetMessage(BOOL bButtonShow, CString strMsg)
{
	SetDlgItemText(IDC_staStatus, strMsg);
	GetDlgItem(IDC_btnRetry)->EnableWindow(bButtonShow);
	GetDlgItem(IDC_btnDetailView)->EnableWindow(bButtonShow);
	GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(bButtonShow);
}
