


#include "stdafx.h"
#include "resource.h"
#include "ILL_RFID_SECURITY_CHANGE.h"
#ifdef _RFID
#include "..\\RFID_JOB\\RFID_Interface.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_RFID_SECURITY_CHANGE::CILL_RFID_SECURITY_CHANGE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
		
	

	m_pDM = NULL;
	m_pGrid = NULL;
	
	m_bIsStop = FALSE;
	m_bIsRunning = FALSE;
	m_bIsSecurity = TRUE;
	m_bIsNotSelect = FALSE;
	m_strTransNoList = _T("''");
}


void CILL_RFID_SECURITY_CHANGE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
		
	
}


BEGIN_MESSAGE_MAP(CILL_RFID_SECURITY_CHANGE, CDialog)
	//{{AFX_MSG_MAP(CILL_RFID_SECURITY_CHANGE)
	ON_BN_CLICKED(IDC_btnILL_SECURITY_START, OnbtnILLSECURITYSTART)
	ON_BN_CLICKED(IDC_btnILL_SECURITY_STOP, OnbtnILLSECURITYSTOP)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_radRFID_SECURITY_ON, IDC_radRFID_SECURITY_OFF, OnRad)
END_MESSAGE_MAP()




BOOL CILL_RFID_SECURITY_CHANGE::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam || VK_F12 == pMsg->wParam || VK_F11 == pMsg->wParam)
		{
			return TRUE;
		}
		else if(VK_ESCAPE == pMsg->wParam && TRUE == m_bIsRunning)
		{
			OnCancel();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CILL_RFID_SECURITY_CHANGE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

#ifdef _RFID
	CRFID_Interface RFID;
	if(0 > RFID.START_UP())
	{
		PostMessage(WM_CLOSE);
	}
	RFID.CLEAN_UP();
#endif

	if(0 > InitESL_Mgr(_T("SM_ILL_RFID_SECURITY_CHANGE")))
	{
		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"SM_ILL_RFID_SECURITY_CHANGE\")"));
	}

	m_pDM = FindDM(_T("DM_ILL_RFID_SECURITY_CHANGE"));
	if(NULL != m_pDM)
	{
		INT aColIndex[256];	
		CString strWhere;
		
		m_pDM->SetCONNECTION_INFO(_T(""));

		m_pDM->TBL_NAME = _T("ILL_TRANS_TBL ITL, BO_BOOK_TBL BB");				
		strWhere.Format(_T("ITL.TRANS_NO IN (%s) AND ITL.PUBLISH_FORM_CODE = 'MO'"), m_strTransNoList);
		
		CString strSQL1 = m_pDM->MakeSelectQuery(strWhere, aColIndex);

		m_pDM->TBL_NAME = _T("ILL_TRANS_TBL ITL, SE_BOOK_TBL BB");				
		strWhere.Format(_T("ITL.TRANS_NO IN (%s) AND ITL.PUBLISH_FORM_CODE = 'SE'"), m_strTransNoList);
		
		CString strSQL2 = m_pDM->MakeSelectQuery(strWhere, aColIndex);

		CString strSQL;
		strSQL.Format(_T("%s UNION ALL %s"), strSQL1, strSQL2);
		
		m_pDM->RestructDataManager(strSQL);
	}
	
	m_pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_RFID_SECURITY_CHANGE"), _T("메인그리드"));
	if(NULL != m_pGrid)
	{
		m_pGrid->Display();
	}

	SetRad((TRUE == m_bIsSecurity) ? IDC_radRFID_SECURITY_ON : IDC_radRFID_SECURITY_OFF, m_bIsNotSelect);

	return TRUE;  
	              
}

void CILL_RFID_SECURITY_CHANGE::SetRad(UINT uID, BOOL bIsNotSelect)
{
	CButton* pBtn = NULL;
	INT nCnt = 0;
	const INT BTN_CNT = 2;
	UINT uBtnID[BTN_CNT] = {IDC_radRFID_SECURITY_ON, IDC_radRFID_SECURITY_OFF};
	
	for(nCnt = 0; nCnt < BTN_CNT; nCnt++)
	{
		pBtn = (CButton*)GetDlgItem(uBtnID[nCnt]);
		if(NULL != pBtn)
		{
			if(uBtnID[nCnt] == uID)
			{
				pBtn->SetCheck(TRUE);
			}
			else
			{
				pBtn->SetCheck(FALSE);
				if(TRUE == bIsNotSelect)
				{
					pBtn->EnableWindow(FALSE);
				}
			}
		}
	}

	m_bIsSecurity = (IDC_radRFID_SECURITY_ON == uID) ? TRUE : FALSE;
}
void CILL_RFID_SECURITY_CHANGE::OnRad(UINT uID)
{
	SetRad(uID, FALSE);
}

void CILL_RFID_SECURITY_CHANGE::OnbtnILLSECURITYSTART() 
{
	
	INT ids;
	CString sFuncInfo;
#ifndef _RFID
	return;
#endif

#ifdef _RFID
	
	if(TRUE == m_bIsRunning)
	{
		return;
	}
	m_bIsRunning = TRUE;

	GetDlgItem(IDC_btnILL_SECURITY_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnILL_SECURITY_STOP)->EnableWindow(TRUE);

	CRFID_Interface RFID;

	
	ids = RFID.SetRFIDMapNSecurity(m_pInfo->m_strRFIDMap.GetBuffer(0),
								   m_pInfo->m_strRFIDSecType.GetBuffer(0),
								   m_pInfo->m_nRFIDSecVal_f,
								   m_pInfo->m_nRFIDSecVal_s						);	

	sFuncInfo.Format(_T("SetRFIDMapNSecurity (Set TagMap:%s)"), m_pInfo->m_strRFIDMap.GetBuffer(0));
	CLocCommonAPI::MakeRFIDLog(sFuncInfo, _T("RFID 대출/보안상태 업데이트"), __WFILE__, __LINE__, ids);	

	if(0 > RFID.START_UP())
	{
		ShowMessage(_T("자료리더기를 발견하지 못하였습니다."));
		Wait(1000);
 		GetDlgItem(IDC_staILL_SECURITY_START)->EnableWindow(TRUE);
	}

	CStringArray astrDSFID, astrSerial;
	CString strBookWorkNo, strTagWorkNo, strBookRegNo, strShowMessage, strResult;
	INT nMaxRow = 0, nRowCnt = 0, nRowCnt2 = 0, nMaxTag = 0, nTagCnt = 0;
	while(0 < m_pDM->GetRowCount())
	{			
		ShowMessage(_T("리스트에 있는 책을 리더기에 올려 주십시요."));

		if(TRUE == m_bIsStop)
		{
			break;
		}

		astrDSFID.RemoveAll();
		astrSerial.RemoveAll();
		RFID.GetTagSerial(astrDSFID, astrSerial);

		nMaxRow = m_pDM->GetRowCount();
		for(nRowCnt = 0; nRowCnt < nMaxRow; nRowCnt++)
		{
			m_pDM->GetCellData(_T("WORKNO"), nRowCnt, strBookWorkNo);
			m_pDM->GetCellData(_T("등록번호"), nRowCnt, strBookRegNo);
			
			nMaxTag = astrSerial.GetSize();
			for(nTagCnt = 0; nTagCnt < nMaxTag; nTagCnt++)
			{
				if(TRUE == m_bIsStop)
				{
					break;
				}

				strTagWorkNo = astrSerial.GetAt(nTagCnt);
				if(strTagWorkNo == strBookWorkNo)
				{
					strShowMessage.Format(
						_T("등록번호 [%s]인 자료의 보안상태 변경중입니다."), strBookRegNo);
					ShowMessage(strShowMessage);

					ids = RFID.ChangeTagLoanProcDataByUID(strTagWorkNo, !m_bIsSecurity);
					strResult = (0 == ids) ? _T("성공") : _T("실패");					
					m_pDM->SetCellData(_T("결과"), strResult, nRowCnt);	

					sFuncInfo.Format(_T("ChangeTagLoanProcDataByUID 1-2 (%s, %s, %d)"), strTagWorkNo, strBookRegNo, !m_bIsSecurity);
					CLocCommonAPI::MakeRFIDLog(sFuncInfo, _T("RFID 대출/보안상태 업데이트"), __WFILE__, __LINE__, ids);	

					strShowMessage.Format(
						_T("등록번호 [%s]인 자료의 보안상태 변경에 %s하였습니다."), strBookRegNo, strResult);
					ShowMessage(strShowMessage);					
				}
			}
		}

		
		for(nRowCnt2 = nMaxRow-1; nRowCnt2 >= 0; nRowCnt2--)
		{
			m_pDM->GetCellData(_T("결과"), nRowCnt2, strResult);
			if(_T("성공") == strResult)
			{
				m_pDM->DeleteRow(nRowCnt2);
			}
		}
		m_pGrid->Display();
		Wait(500);
	}
	
	RFID.CLEAN_UP();
	
	m_bIsStop = FALSE;
	m_bIsRunning = FALSE;
	GetDlgItem(IDC_btnILL_SECURITY_STOP)->EnableWindow(FALSE);
 	GetDlgItem(IDC_staILL_SECURITY_START)->EnableWindow(TRUE);
#endif
}

void CILL_RFID_SECURITY_CHANGE::OnbtnILLSECURITYSTOP() 
{
	
	m_bIsStop = TRUE;
}
void CILL_RFID_SECURITY_CHANGE::ShowMessage(const CString &strMessage)
{
	SetDlgItemText(IDC_staILL_SECURITY_MSG, strMessage);
}

void CILL_RFID_SECURITY_CHANGE::Wait(INT nTime)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();
	
	while (GetTickCount() - dwStart < (ULONG)nTime)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}


void CILL_RFID_SECURITY_CHANGE::OnCancel() 
{
	
	if(m_bIsRunning)
	{
		OnbtnILLSECURITYSTOP();
		SetTimer(1, 200, NULL);
		return;
	}

	CDialog::OnCancel();
}

void CILL_RFID_SECURITY_CHANGE::OnTimer(UINT nIDEvent) 
{
	
	if(1 == nIDEvent)
	{
		KillTimer(nIDEvent);
		OnCancel();
	}
	CDialog::OnTimer(nIDEvent);
}
