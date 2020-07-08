#include "stdafx.h"
#include "KL_SearchCurrentLoanInfoDetailDlg.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKL_SearchCurrentLoanInfoDetailDlg::CKL_SearchCurrentLoanInfoDetailDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_bIsRunningThread = FALSE;	
	m_strUserNo = _T("");
	m_strLibCode = _T("");
}
CKL_SearchCurrentLoanInfoDetailDlg::~CKL_SearchCurrentLoanInfoDetailDlg()
{
}

void CKL_SearchCurrentLoanInfoDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
			
	//{{AFX_DATA_MAP(CKL_SearchCurrentLoanInfoDetailDlg) 
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKL_SearchCurrentLoanInfoDetailDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_SearchCurrentLoanInfoDetailDlg)
	ON_BN_CLICKED(IDC_btnLOC_USERCLOSE, OnbtnCLOSE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnRetry, OnbtnRetry)
	ON_MESSAGE(WM_RESULT_VIEW,ResultView)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CKL_SearchCurrentLoanInfoDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());
					
	if(0>InitESL_Mgr(_T("SM_KL_SearchCurrentLoanInfoDetail")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	CESL_Grid*	pGrid	=	(CESL_Grid*)GetDlgItem( IDC_gridCurrentLoanInfoDetail );
	INT nReserveDateIdx, nReserveExpireDateIdx;
	nReserveDateIdx = -1;
	nReserveExpireDateIdx = -1;
	CLocCommonAPI::GetGridColumnIndexs( this , pGrid , _T("예약일") , nReserveDateIdx );
	CLocCommonAPI::GetGridColumnIndexs( this , pGrid , _T("예약만기일") , nReserveExpireDateIdx );
	pGrid->SetColWidth( nReserveDateIdx+1 , 0 , 0 );
	pGrid->SetColWidth( nReserveExpireDateIdx+1 , 0 , 0 );

	
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

	return TRUE;  
	              
}

BOOL CKL_SearchCurrentLoanInfoDetailDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN )
		{
			return TRUE;
		}
		else if(pMsg->wParam == VK_ESCAPE )
		{
			if(TRUE == m_bIsRunningThread) return TRUE;
		}
	}	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CKL_SearchCurrentLoanInfoDetailDlg::OnbtnRetry() 
{
	SetTimer(0, 100, NULL);
	::SetFocus(this->GetSafeHwnd());
}

void CKL_SearchCurrentLoanInfoDetailDlg::OnbtnCLOSE() 
{
	if(TRUE == m_bIsRunningThread) return;

	CDialog::OnCancel();
}

void CKL_SearchCurrentLoanInfoDetailDlg::OnClose()
{
	if(TRUE == m_bIsRunningThread) return;
	
	CDialog::OnClose();
}


VOID CKL_SearchCurrentLoanInfoDetailDlg::SetInfo(CString strUserNo, CString strLibCode) 
{
	m_strUserNo = strUserNo;
	m_strLibCode = strLibCode;
}

void CKL_SearchCurrentLoanInfoDetailDlg::OnTimer(UINT nIDEvent) 
{
	if(0==nIDEvent)
	{
		KillTimer(nIDEvent);
		AfxBeginThread(SearchLoanInfoDetailThread, this);
	}
	
	CDialog::OnTimer(nIDEvent);
}

UINT CKL_SearchCurrentLoanInfoDetailDlg::SearchLoanInfoDetailThread(LPVOID pParam)
{
	CKL_SearchCurrentLoanInfoDetailDlg* pDlg = (CKL_SearchCurrentLoanInfoDetailDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	if(!pDlg->m_pInfo->IsConnectUnityLoanService())
	{
		pDlg->SetDlgItemText(IDC_staStatus, _T("센터와 접속시도 중 입니다."));
		pDlg->GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		pDlg->GetDlgItem(IDC_btnDetailView)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
		return TRUE; 
	}

	pDlg->m_bIsRunningThread = TRUE;

	INT ids = pDlg->StartSearch();
	if(0==ids) pDlg->GetDlgItem(IDC_btnRetry)->ShowWindow(SW_HIDE);
	else pDlg->GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);

	pDlg->m_bIsRunningThread = FALSE;

	return 0;
}

INT CKL_SearchCurrentLoanInfoDetailDlg::StartSearch()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("StartSearch");
	SetMessage(FALSE, _T("대출 현황을 조회합니다."));

	
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K04_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);
	Writer.addElement(_T("도서관부호"), m_strLibCode);
	Writer.addElement(_T("전체검색"), _T("Y"));

	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0>ids)
	{
		CString strMsg;
		strMsg.Format(_T("미들웨어와의 통신에 실패하였습니다.[Error:%d]"),ids);
		SetMessage(TRUE, strMsg);
		return -1030;
	}
	
	CESL_DataMgr TempDM;
	Reader.makeDM(&TempDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp)
	{
		CString strMsg,strTemp;
		TempDM.GetCellData(0, 0, strTemp);
		strMsg.Format(_T("Error가 발생하였습니다. [%s]"),strTemp);
		SetMessage(TRUE, strMsg);
		return -1040;
	}
	SetMessage(FALSE, _T("대출 현황을 불러오는 중입니다."));

	
	ids = CopyDM(&TempDM);
	if(0>ids) 
	{
		CString strMsg;
		strMsg.Format(_T("Error가 발생하였습니다."));
		SetMessage(FALSE, strMsg);
		return -1040;
	}

	
	this->SendMessage(WM_RESULT_VIEW, 0, 0);
	SetMessage(TRUE, _T(""));

	return 0;

EFS_END
return -1;
}

VOID CKL_SearchCurrentLoanInfoDetailDlg::SetMessage(BOOL bButtonShow, CString strMsg)
{
	SetDlgItemText(IDC_staStatus, strMsg);
	GetDlgItem(IDC_btnRetry)->EnableWindow(bButtonShow);
	GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(bButtonShow);
}

INT CKL_SearchCurrentLoanInfoDetailDlg::CopyDM(CESL_DataMgr* TmpDm)
{
EFS_BEGIN

	INT ids;
	CString strData;
	
	const INT nColCnt = 14;
	CString strALias[nColCnt] =
	{
		_T("대출도서관"),	_T("등록번호"),		_T("서명"),			_T("청구기호"),		_T("편권차"),
		_T("대출일"),		_T("반납예정일"),	_T("반납연기일"),	_T("예약일"),		_T("예약만기일"),
		_T("상태"),			_T("저자"),			_T("발행자"),		_T("트랜잭션코드")
	};

	CESL_DataMgr* pDM = FindDM(_T("DM_KL_SearchCurrentLoanInfoDetail"));
	if(NULL==pDM) return -1010;

	pDM->FreeData();

	INT nCopyCnt = TmpDm->GetRowCount();
	for(INT i=0; i<nCopyCnt; i++)
	{
		pDM->InsertRow(-1);
		
		CString strTranCode;
		TmpDm->GetCellData( strALias[13] , i , strTranCode );

		for( INT j=0; j<nColCnt; j++ )
		{
			ids = TmpDm->GetCellData( strALias[j] , i , strData );
			
			
			ids = TmpDm->GetCellData( strALias[j] , i , strData );			
			
			if(10==j)
			{
				if(strTranCode.IsEmpty())
				{
					if("0"==strData)
					{
						strData = "대출";
					}
					else if("2"==strData)
					{
						strData = "반납연기";
					}
					
					else if("L"==strData)
					{
						strData = "책두레대출";
					}
					else if("D"==strData)
					{
						strData = "책두레반납연기";
					}
					else
					{
						strData = "알수없음";
					}
				}
				else
				{
					if("0"==strTranCode)
					{
						strData = "신청";
					}
					else if("1"==strTranCode)
					{
						strData = "발송";
					}
					else if("3"==strTranCode)
					{
						strData = "입수";
					}
					else if("4"==strTranCode)
					{
						strData = "타관대출";
					}
					else
					{
						strData = "알수없음";
					}
				}
			}


			ids = pDM->SetCellData( strALias[j] , strData , i );
			if(0>ids) return -1030;
		}
	}

	return 0;

EFS_END
return -1;
}

VOID CKL_SearchCurrentLoanInfoDetailDlg::ResultView()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_SearchCurrentLoanInfoDetail"), _T("grid"));
	pGrid->Display();
}
