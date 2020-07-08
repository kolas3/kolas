#include "stdafx.h"
#include "KL_SecedeDlg.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CKL_SecedeDlg::CKL_SecedeDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{	
	m_bIsSelfLib = FALSE;
	m_bIsRunningThread = FALSE;
	m_strUserNo = _T("");
	m_strGpinHash = _T("");
	m_bIsChange = FALSE;
	
	m_pDM = NULL;
	m_pGrid = NULL;
	m_strLibCodeList = _T("");
}
CKL_SecedeDlg::~CKL_SecedeDlg()
{
}

void CKL_SecedeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CKL_SecedeDlg) 
	//}}AFX_DATA_MAP	
	
}

BEGIN_MESSAGE_MAP(CKL_SecedeDlg, CDialog)
	//{{AFX_MSG_MAP(CKL_SecedeDlg)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnRetry, OnbtnRetry)
	ON_BN_CLICKED(IDC_btnSecede, OnbtnSecede)
	ON_BN_CLICKED(IDC_btnLOC_USERCLOSE, OnbtnCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CKL_SecedeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd());

	if(0 > InitESL_Mgr(_T("SM_KL_Secede")))
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	m_pDM = FindDM(_T("DM_KL_Secede"));
	if(NULL == m_pDM) return FALSE;
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_KL_Secede"), _T("grid"));
	if(NULL == m_pGrid) return FALSE;
	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	CString strQuery = _T("SELECT DISTINCT LIB_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE IS NOT NULL");
	INT ids = TmpDM.RestructDataManager(strQuery);
	if(0 > ids) return FALSE;
	if(1 > TmpDM.GetRowCount()) return FALSE;
	CString strGetData;
	m_strLibCodeList = _T("");
	for(INT i = 0; i < TmpDM.GetRowCount(); i++)
	{
		if(0 < i)
		{
			m_strLibCodeList += _T(",");
		}
		TmpDM.GetCellData(i, 0, strGetData);
		m_strLibCodeList += _T("'") + strGetData + _T("'");
	}
	if(6 > m_strLibCodeList.GetLength()) return FALSE;
	
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		SetDlgItemText(IDC_staStatus, _T("센터와 접속시도 중 입니다."));
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnSecede)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
		return TRUE; 
	}
	
	OnbtnRetry();

	return TRUE;
}

BOOL CKL_SecedeDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			OnbtnCLOSE();
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			if(GetDlgItem(IDC_btnRetry)->m_hWnd == pMsg->hwnd)
			{
				OnbtnRetry();
			}
			else if(GetDlgItem(IDC_btnSecede)->m_hWnd == pMsg->hwnd)
			{
				OnbtnSecede();
			}
			else if(GetDlgItem(IDC_btnLOC_USERCLOSE)->m_hWnd == pMsg->hwnd)
			{
				OnbtnCLOSE();
			}
			return TRUE;
		}
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CKL_SecedeDlg::OnbtnRetry() 
{
	if(TRUE == m_bIsRunningThread) return;
	SetTimer(0, 100, NULL);
}

void CKL_SecedeDlg::OnbtnSecede() 
{
	CString strLibCode;
	m_pGrid->SelectMakeList();
	if(0 == m_pGrid->SelectGetCount())
	{
		SetMessage(TRUE, _T("탈퇴처리할 도서관을 선택하여 주십시요."));
		return;
	}
	else if(1 < m_pGrid->SelectGetCount())
	{
		SetMessage(TRUE,_T("한번에 하나의 도서관만 선택할 수 있습니다."));
		return;
	}
	INT nIdx = m_pGrid->SelectGetIdx();
	CString strSecedeYN;
	m_pDM->GetCellData(_T("탈퇴가능여부"), nIdx, strSecedeYN);
	strSecedeYN.TrimLeft();	strSecedeYN.TrimRight();
	if(_T("O") != strSecedeYN)
	{
		SetMessage(TRUE,_T("탈퇴 가능한 도서관이 아닙니다.\r\n탈퇴가능한 도서관은 회원상태가 정상회원이고 대출현황이 없어야합니다."));
		return;
	}
	
	if(IDYES == MessageBox(_T("책이음 탈퇴 후 자관 회원으로 전환 하시겠습니까?"), _T("전환처리"), MB_ICONQUESTION|MB_YESNO))
	{
		m_bIsChange = TRUE;
		SetTimer(100, 100, NULL);
		return;
	}
	

	CString strGroupCode;
	m_pDM->GetCellData(_T("도서관그룹코드"), nIdx, strGroupCode);
	for(INT i = 0; i < m_pDM->GetRowCount(); i++)
	{
		CString strGetData;
		m_pDM->GetCellData(_T("도서관그룹코드"), i, strGetData);
		if(strGroupCode == strGetData)
		{
			m_pDM->GetCellData(_T("탈퇴가능여부"), i, strGetData);
			if(_T("O") != strGetData)
			{
				SetMessage(TRUE,_T("통합DB 도서관중에 탈퇴 가능하지 않은 도서관이 있습니다.\r\n탈퇴가능한 도서관은 회원상태가 정상회원이고 대출현황이 없어야합니다."));
				return;
			}
		}
	}
	if(IDYES != MessageBox(_T("탈퇴처리를 선택하셨습니다.\r\n\r\n※ 선정된 도서관에서 탈퇴처리된 이용자가\r\n    도서관을 이용하기 위해서는 재가입을 해야만합니다.\r\n\r\n※ 선정한 도서관과 같은 도서관리DB를\r\n    사용하는 도서관들도 탈퇴처리 됩니다.\r\n\r\n선정한 도서관의 이용자를 탈퇴처리 하시겠습니까?"), _T("탈퇴처리"), MB_ICONQUESTION|MB_YESNO))
	{
		SetMessage(TRUE, _T("탈퇴처리가 중지되었습니다."));
		return;
	}
	
	
	m_bIsChange = FALSE;
	SetTimer(100, 100, NULL);	
}

void CKL_SecedeDlg::OnbtnCLOSE()
{
	if(TRUE == m_bIsRunningThread) return;
	OnCancel();
}

void CKL_SecedeDlg::OnClose() 
{
	if(TRUE == m_bIsRunningThread) return;
	CDialog::OnClose();
}

void CKL_SecedeDlg::OnTimer(UINT nIDEvent) 
{
	if(0 == nIDEvent)
	{
		KillTimer(0);
		SetMessage(FALSE, _T("가입 된 도서관을 조회합니다."));
		AfxBeginThread(SearchJoinLibThread, this);
	}
	else if(100 == nIDEvent)
	{
		KillTimer(100);
		
		if(m_bIsChange)
			SetMessage(FALSE, _T("선택한 책이음 탈퇴 및 자관회원 전환을 요청하는 중입니다."));
		else
			SetMessage(FALSE, _T("선택한 도서관의 책이음 탈퇴처리를 요청하는 중입니다."));
		
		AfxBeginThread(SecedeThread, this);
	} 	
	else if(200 == nIDEvent)
	{
		KillTimer(200);
		
		if(m_bIsChange)
			MessageBox(_T("전환처리가 완료되었습니다."), _T("전환처리"), MB_ICONINFORMATION|MB_OK);
		else
			MessageBox(_T("탈퇴처리가 완료되었습니다."), _T("탈퇴처리"), MB_ICONINFORMATION|MB_OK);
		OnCancel();
	}	
	else if(300 == nIDEvent)
	{
		KillTimer(300);
		
		if(m_bIsChange)
			SetMessage(TRUE, _T("알수없는 오류가 발생하여 전환처리에 실패하였습니다."));
		else
			SetMessage(TRUE, _T("알수없는 오류가 발생하여 탈퇴처리에 실패하였습니다."));
	}
	else if(301 == nIDEvent)
	{
		KillTimer(301);
		
		if(m_bIsChange)
			SetMessage(TRUE, _T("전환처리에 실패하였습니다.\r\n전환처리 버튼을 선택하여 재시도 하십시오."));
		else
			SetMessage(TRUE, _T("탈퇴처리에 실패하였습니다.\r\n탈퇴처리 버튼을 선택하여 재시도 하십시오."));
	}
	else if(302 == nIDEvent)
	{
		KillTimer(302);
		SetMessage(TRUE, _T("알수없는 오류가 발생하여 도서관 조회에 실패하였습니다."));
	}
	else if(303 == nIDEvent)
	{
		KillTimer(303);
		SetMessage(TRUE, _T("미들웨어와의 통신에 실패하였습니다."));
	}
	else if(304 == nIDEvent)
	{
		KillTimer(304);
		SetMessage(TRUE, _T("가입도서관 조회에 실패하였습니다.\r\n도서관조회 버튼을 선택하여 재시도 하십시오."));
	}
	else if(305 == nIDEvent)
	{
		KillTimer(305);
		SetMessage(TRUE, _T("도서관 정보 설정에 실패하였습니다."));
	}
	else if(306 == nIDEvent)
	{
		KillTimer(306);
		SetMessage(TRUE, _T("탈퇴가능여부 설정에 실패하였습니다."));
	}
	else if(307 == nIDEvent)
	{
		KillTimer(307);
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_HIDE);
		SetMessage(TRUE, _T("조회가 완료되었습니다.\r\n탈퇴처리 버튼을 선택하면 선정된 도서관에 탈퇴 요청을 합니다."));
		ResultView();
	}
	else if(308 == nIDEvent)
	{
		KillTimer(308);
		SetDlgItemText(IDC_staStatus, _T("센터와 접속시도 중 입니다."));
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnSecede)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
	}
	else if(309 == nIDEvent)
	{
		KillTimer(309);
		SetDlgItemText(IDC_staStatus, _T("센터와 접속시도 중 입니다."));
		GetDlgItem(IDC_btnRetry)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnSecede)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(TRUE);
	}
	else if(310 == nIDEvent)
	{
		KillTimer(310);
		SetMessage(TRUE, _T("선정한 도서관에서 센터로 적용되지 않은 현황이 존재합니다."));
	}
	else if(400 == nIDEvent)
	{
		KillTimer(400);
		
		if(m_bIsChange)
			SetMessage(TRUE, _T("탈퇴처리를 완료하였습니다."));
		else
			SetMessage(TRUE, _T("전환처리를 완료하였습니다."));
		ResultView();
	}
	
	CDialog::OnTimer(nIDEvent);
}

UINT CKL_SecedeDlg::SearchJoinLibThread(LPVOID pParam)
{
EFS_BEGIN

	CKL_SecedeDlg* pDlg = (CKL_SecedeDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;
	if(!pDlg->m_pInfo->IsConnectUnityLoanService())
	{
		pDlg->SetTimer(308, 100, NULL);
		return 0; 
	}
	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->SearchJoinLib();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->SetTimer(302, 100, NULL);
	}
	else if(1 == ids)
	{
		pDlg->SetTimer(303, 100, NULL);
	}
	else if(2 == ids)
	{
		pDlg->SetTimer(304, 100, NULL);
	}
	else if(3 == ids)
	{
		pDlg->SetTimer(305, 100, NULL);
	}
	else if(4 == ids)
	{
		pDlg->SetTimer(306, 100, NULL);
	}
	else
	{
		pDlg->SetTimer(307, 100, NULL);
	}

	return 0;

EFS_END
return -1;
}

UINT CKL_SecedeDlg::SecedeThread(LPVOID pParam)
{
EFS_BEGIN

	CKL_SecedeDlg* pDlg = (CKL_SecedeDlg*)pParam;
	if(TRUE == pDlg->m_bIsRunningThread) return 0;

	if(!pDlg->m_pInfo->IsConnectUnityLoanService())
	{
		pDlg->SetTimer(309, 100, NULL);
		return 0;
	}
	pDlg->m_bIsRunningThread = TRUE;
	INT ids = pDlg->Secede();
	pDlg->m_bIsRunningThread = FALSE;
	if(0 > ids)
	{
		pDlg->SetTimer(300, 100, NULL);
	}
	else if(1 == ids)
	{
		pDlg->SetTimer(301, 100, NULL);
	}
	else if(2 == ids)
	{
		pDlg->SetTimer(200, 100, NULL);
	}
	else if(3 == ids)
	{
		pDlg->SetTimer(310, 100, NULL);
	}
	else
	{
		pDlg->SetTimer(400, 100, NULL);
	}

	return 0;

EFS_END
return -1;
}

INT CKL_SecedeDlg::SearchJoinLib()
{
EFS_BEGIN

	INT ids;
	
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	CFrameWriter Writer(m_pInfo);
	CString strCommand = _T("IL_K48_LOC_B01_SERVICE");
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		return 1;
	}
	CESL_DataMgr TempDM;
	Reader.makeDM(&TempDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		return 2;
	}
	
	ids = CopyDM(&TempDM);
	if(0 > ids) 
	{
		return 3;
	}
	
	ids = CheckSecede();
	if(0 > ids)
	{
		return 4;
	}

	return 0;
	
EFS_END
return -1;
}

INT CKL_SecedeDlg::Secede()
{
EFS_BEGIN

	INT ids;
	m_bIsSelfLib = FALSE;
	
	CString strLibCode;
	INT nIdx = m_pGrid->SelectGetIdx();
	if(0 > nIdx) return -3;
	CString strCreateLibCode;
	m_pDM->GetCellData(_T("가입도서관부호"), nIdx, strCreateLibCode);
	if(strCreateLibCode.IsEmpty()) return -3;
	if(0 <= m_strLibCodeList.Find(strCreateLibCode))
	{
		m_bIsSelfLib = TRUE;
	}
	CString strSysdate;
	CTime tSysdate = CLocCommonAPI::GetCurrentDateTime(this, strSysdate);
	CString strLog = GetWorkLogMsg(_T("통합회원탈퇴처리"), __WFILE__, __LINE__);
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K06_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), m_strUserNo);
	Writer.addElement(_T("G-PINHASH인증코드"), m_strGpinHash);
	Writer.addElement(_T("도서관부호"), strCreateLibCode);
	Writer.addElement(_T("요청도서관부호"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("요청도서관부호리스트"), m_strLibCodeList);
	Writer.addElement(_T("탈퇴일자"), strSysdate);
	Writer.addElement(_T("마지막작업"), strLog);

	if(m_bIsChange)
	{
		Writer.addElement(_T("자관전환여부"), "Y");
	}
	
	CFrameReader Reader;
	CFrameSender Sender(m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		return 1;
	}
	CESL_DataMgr TempDM;
	Reader.makeDM(&TempDM);
	if(1 > TempDM.GetRowCount())
	{
		return 1;
	}
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strErrorCode = Reader.getElement(0, 0);
		if(0 < strErrorCode.Find(_T("06LOCB01DT01")) || 0 < strErrorCode.Find(_T("06LOCR01DT02")))
		{
			return 3;
		}
		else
		{
			return 1;
		}
	}
	TempDM.GetCellData(_T("처리결과"), 0, strTemp);
	if(_T("OK") != strTemp)
	{
		return 1;
	}
	
	m_pDM->StartFrame();
	CString strQuery;
	strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET KL_SECEDE_DATE = TO_DATE('%s','YYYY/MM/DD HH24:MI:SS'), LAST_WORK = TO_CHAR(SYSDATE,'YYYY/MM/DD HH24:MI:SS') || ' 책이음탈퇴' WHERE USER_NO='%s';"), strSysdate, m_strUserNo);
	m_pDM->AddFrame(strQuery);
	m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	if(TRUE == m_bIsSelfLib) 
	{ 
		return 2;
	}
	else
	{ 		
		if(!m_bIsChange)
		{
			CString strGroupCode;
			m_pDM->GetCellData(_T("도서관그룹코드"), nIdx, strGroupCode);
			for(INT i = m_pDM->GetRowCount(); i >= 0; i--)
			{
				CString strData;
				m_pDM->GetCellData(_T("도서관그룹코드"), i, strData);
				if(strGroupCode == strData)
				{
					m_pDM->DeleteRow(i);
				}
			}
		}
		else
		{
			CString strGroupCode;
			m_pDM->GetCellData(_T("도서관그룹코드"), nIdx, strGroupCode);
			for(INT i = m_pDM->GetRowCount(); i >= 0; i--)
			{
				CString strData;
				m_pDM->GetCellData(_T("도서관그룹코드"), i, strData);
				if(strGroupCode == strData)
				{
					m_pDM->SetCellData(_T("회원상태"), _T("탈퇴회원"), i);
					m_pDM->SetCellData(_T("탈퇴가능여부"), _T("X"), i);
				}
			}			
		}		
	}

	return 0;

EFS_END
return -1;
}

VOID CKL_SecedeDlg::SetMessage(BOOL bButtonShow, CString strMsg)
{
	SetDlgItemText(IDC_staStatus, strMsg);
	GetDlgItem(IDC_btnRetry)->EnableWindow(bButtonShow);
	GetDlgItem(IDC_btnSecede)->EnableWindow(bButtonShow);
	GetDlgItem(IDC_btnLOC_USERCLOSE)->EnableWindow(bButtonShow);
}

INT CKL_SecedeDlg::CopyDM(CESL_DataMgr* TmpDm)
{
EFS_BEGIN

	CString strData;
	const INT nColCnt = 6;
	CString strALias[nColCnt] = {_T("가입도서관부호"),_T("도서관명"),_T("대출중인책수"),_T("대출정지일"),_T("회원상태"),_T("도서관그룹코드")};
	INT nCopyCnt = TmpDm->GetRowCount();
	for(INT i = 0; i < nCopyCnt; i++)
	{
		m_pDM->InsertRow(-1);
		for(INT j = 0; j < nColCnt; j++)
		{
			TmpDm->GetCellData(strALias[j], i, strData);
			if(4 == j)
			{
				if(_T("0") == strData) 
				{
					strData = _T("정상회원");
				}
				else if(_T("1") == strData)
				{
					strData = _T("대출정지");
				}
				else if(_T("2") == strData)
				{
					strData = _T("제적회원");
				}
				else if(_T("3") == strData)
				{
					strData = _T("탈퇴회원");
				}
			}
			m_pDM->SetCellData(strALias[j], strData, i);
		}
	}
	return 0;

EFS_END
return -1;
}

INT CKL_SecedeDlg::CheckSecede()
{
EFS_BEGIN

	CString strData;
	const INT nColCnt = 4;
	CString strALias[nColCnt] = {_T("대출중인책수"),_T("예약중인책수"),_T("대출정지일"),_T("회원상태")};
	INT nCnt = m_pDM->GetRowCount();
	for(INT i = 0; i < nCnt; i++)
	{
		CString strCheck = _T("O");
		m_pDM->GetCellData(_T("대출중인책수"), i, strData);
		if(0 < _ttoi(strData))
		{
			strCheck = _T("X");
		}
		m_pDM->GetCellData(_T("예약중인책수"), i, strData);
		if(0 < _ttoi(strData))
		{
			strCheck = _T("X");
		}
		m_pDM->GetCellData(_T("대출정지일"), i, strData);
		if(FALSE == strData.IsEmpty())
		{
			strCheck = _T("X");
		}
		m_pDM->GetCellData(_T("회원상태"), i, strData);
		if(_T("정상회원") != strData)
		{
			strCheck = _T("X");
		}
		m_pDM->SetCellData(_T("탈퇴가능여부"), strCheck, i);
	}

	return 0;

EFS_END
return -1;
}

VOID CKL_SecedeDlg::ResultView()
{
	if(NULL != m_pGrid)
	{
		m_pGrid->Display();
	}
}

HBRUSH CKL_SecedeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(IDC_staStatus == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(0,70,213));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

BEGIN_EVENTSINK_MAP(CKL_SecedeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CKL_SecedeDlg)
	ON_EVENT(CKL_SecedeDlg, IDC_gridSecede, -604 /* KeyUp */, OnKeyUpgridSecede, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKL_SecedeDlg::OnKeyUpgridSecede(short FAR* KeyCode, short Shift) 
{
	if( *KeyCode == 27 )
	{
		::PostMessage(this->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 1);
	}
}