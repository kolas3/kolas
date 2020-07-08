


#include "stdafx.h"
#include "resource.h"
#include "ILL_RFID_SECURITY_CHANGE2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_RFID_SECURITY_CHANGE2::CILL_RFID_SECURITY_CHANGE2(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
		
	

	m_pGrid = NULL;
	
	m_bIsStop = FALSE;
	m_bIsRunning = FALSE;
	m_bIsRunningThread = FALSE;
	m_bIsCancelThread = FALSE;
	m_bIsSecurity = TRUE;
	m_bIsNotSelect = FALSE;
	m_strTransNoList = _T("''");


}

CILL_RFID_SECURITY_CHANGE2::~CILL_RFID_SECURITY_CHANGE2()
{
#ifdef _RFID
	RFID.CLEAN_UP();
#endif
	
	MSG msg;
	while(m_bIsRunningThread)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void CILL_RFID_SECURITY_CHANGE2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
		
	
}


BEGIN_MESSAGE_MAP(CILL_RFID_SECURITY_CHANGE2, CDialog)
	//{{AFX_MSG_MAP(CILL_RFID_SECURITY_CHANGE2)
	ON_BN_CLICKED(IDC_btnILL_SECURITY_START, OnbtnILLSECURITYSTART)
	ON_BN_CLICKED(IDC_btnILL_SECURITY_STOP, OnbtnILLSECURITYSTOP)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_radRFID_SECURITY_ON, IDC_radRFID_SECURITY_OFF, OnRad)
END_MESSAGE_MAP()




BOOL CILL_RFID_SECURITY_CHANGE2::PreTranslateMessage(MSG* pMsg) 
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

BOOL CILL_RFID_SECURITY_CHANGE2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	InitGrid();
#ifdef _RFIFD
	CRFID_Interface RFID;
	if(0 > RFID.START_UP())
	{
		PostMessage(WM_CLOSE);
	}
#endif

	m_DM.InitDMPKFromFile(_T("DM_ILL_RFID_SECURITY_CHANGE"));
	m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	m_PrevDM.InitDMPKFromFile(_T("DM_ILL_RFID_SECURITY_CHANGE"));
	
	SetRad((TRUE == m_bIsSecurity) ? IDC_radRFID_SECURITY_ON : IDC_radRFID_SECURITY_OFF, m_bIsNotSelect);


	RefreshListThread(this);


	return TRUE;  
	              
}

void CILL_RFID_SECURITY_CHANGE2::SetRad(UINT uID, BOOL bIsNotSelect)
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
void CILL_RFID_SECURITY_CHANGE2::OnRad(UINT uID)
{
	SetRad(uID, FALSE);
}

void CILL_RFID_SECURITY_CHANGE2::OnbtnILLSECURITYSTART() 
{
	
	INT ids;
	CString sFuncInfo;
#ifndef _RFID
	return;
#endif

#ifdef _RFID
	
	m_bIsCancelThread = TRUE;

	MSG msg;
	while(m_bIsRunningThread)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if(TRUE == m_bIsRunning)
	{
		return;
	}
	m_bIsRunning = TRUE;

	GetDlgItem(IDC_btnILL_SECURITY_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_btnILL_SECURITY_STOP)->EnableWindow(TRUE);









	
	ids = RFID.SetRFIDMapNSecurity(m_pInfo->m_strRFIDMap.GetBuffer(0),
								   m_pInfo->m_strRFIDSecType.GetBuffer(0),
								   m_pInfo->m_nRFIDSecVal_f,
								   m_pInfo->m_nRFIDSecVal_s						);
	
	sFuncInfo.Format(_T("SetRFIDMapNSecurity (Set TagMap:%s)"), m_pInfo->m_strRFIDMap.GetBuffer(0));
	CLocCommonAPI::MakeRFIDLog(sFuncInfo, _T("RFID 대출/보안상태 업데이트"), __WFILE__, __LINE__, ids);	

	CStringArray astrDSFID, astrSerial;
	CString strTagWorkNo, strBookRegNo, strShowMessage, strResult;
	INT nMaxRow = 0, nRowCnt = 0, nRowCnt2 = 0, nMaxTag = 0, nTagCnt = 0;
	{			
		ShowMessage(_T("리스트에 있는 책을 리더기에 올려 주십시요."));






		nMaxRow = m_DM.GetRowCount();
		for(nRowCnt = 0; nRowCnt < nMaxRow; nRowCnt++)
		{


			m_DM.GetCellData(nRowCnt, 0, strBookRegNo);
			m_DM.GetCellData(nRowCnt, 3, strTagWorkNo);

			


			{
				if(TRUE == m_bIsStop)
				{
					break;
				}



				{
					strShowMessage.Format(
						_T("등록번호 [%s]인 자료의 보안상태 변경중입니다."), strBookRegNo);
					ShowMessage(strShowMessage);

					ids = RFID.ChangeTagLoanProcDataByUID(strTagWorkNo, !m_bIsSecurity);
					strResult = (0 == ids) ? _T("성공") : _T("실패");					
					m_DM.SetCellData(nRowCnt, 4, strResult);

					
					sFuncInfo.Format(_T("ChangeTagLoanProcDataByUID 1-2 (%s, %s, %d)"), strTagWorkNo, strBookRegNo, !m_bIsSecurity);
					CLocCommonAPI::MakeRFIDLog(sFuncInfo, _T("RFID 대출/보안상태 업데이트"), __WFILE__, __LINE__, ids);	

					strShowMessage.Format(
						_T("등록번호 [%s]인 자료의 보안상태 변경에 %s하였습니다."), strBookRegNo, strResult);
					ShowMessage(strShowMessage);					
				}
			}
		}


		Wait(500);
	}

	DisplayGrid();
	
	m_bIsStop = FALSE;
	m_bIsRunning = FALSE;
	GetDlgItem(IDC_btnILL_SECURITY_STOP)->EnableWindow(FALSE);
 	GetDlgItem(IDC_staILL_SECURITY_START)->EnableWindow(TRUE);
	
	m_bIsCancelThread = FALSE;
	ShowMessage(_T("보안상태를 변경할 책을 리더기위에 올려놓고 시작버튼을 누르십시요."));
#endif
}

void CILL_RFID_SECURITY_CHANGE2::OnbtnILLSECURITYSTOP() 
{
	
	m_bIsStop = TRUE;
}
void CILL_RFID_SECURITY_CHANGE2::ShowMessage(const CString &strMessage)
{
	SetDlgItemText(IDC_staILL_SECURITY_MSG, strMessage);
}

void CILL_RFID_SECURITY_CHANGE2::Wait(INT nTime)
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


void CILL_RFID_SECURITY_CHANGE2::OnCancel() 
{
	
	if(m_bIsRunning)
	{
		OnbtnILLSECURITYSTOP();
		SetTimer(1, 200, NULL);
		return;
	}

	CDialog::OnCancel();
}

void CILL_RFID_SECURITY_CHANGE2::OnTimer(UINT nIDEvent) 
{
	
	if(1 == nIDEvent)
	{
		KillTimer(nIDEvent);
		OnCancel();
	}
	else if(2 == nIDEvent)
	{
		KillTimer(2);		
		RefreshListThread(this);
	}
	CDialog::OnTimer(nIDEvent);
}

INT CILL_RFID_SECURITY_CHANGE2::RefreshList()
{
#ifndef _RFID
	return 0;
#endif

#ifdef _RFID
	CString strSerial;
	CStringArray astrDSFID, astrSerial;

	astrDSFID.RemoveAll();
	astrSerial.RemoveAll();
	RFID.GetTagSerial(astrDSFID, astrSerial);


	m_PrevDM.CopyDM(&m_DM);
	
	{
	INT nRFIDCnt = astrSerial.GetSize();
	INT nPrevCnt = m_PrevDM.GetRowCount();	
	INT nCnt = 0, nRow = 0;	
	if(0 < nRFIDCnt && 0 < nPrevCnt)
	{
		for(nCnt = nRFIDCnt-1; nCnt >= 0; nCnt--)
		{			
			for(nRow = nPrevCnt-1; nRow >= 0 ; nRow--)
			{
				if(m_PrevDM.GetCellData(nRow, 3) == astrSerial.GetAt(nCnt))
				{
					m_PrevDM.SetCellData(nRow, 5, _T("FIND"));
					astrSerial.RemoveAt(nCnt);
					break;
				}
			}
		}
	}
	
	for(nRow = nPrevCnt-1; nRow >= 0 ; nRow--)
	{
		if(m_PrevDM.GetCellData(nRow, 5) == _T("FIND"))
		{
			m_PrevDM.SetCellData(nRow, 5, _T(""));
		}
		else
		{
			m_PrevDM.DeleteRow(nRow);
		}
	}
	}

	MakeTransNoList(astrSerial, strSerial);
	


	














	
	CString strSQL, strWorkNoList;
	strSQL.Format(
	_T("SELECT BB.REG_NO,")
	   _T("IB.TITLE_INFO,")
	   _T("IB.AUTHOR_INFO,")
	   _T("BB.WORKNO,")
	   _T("'대기중', NULL ")
	_T("FROM IDX_BO_TBL IB,")
	   _T("BO_BOOK_TBL BB ")
	_T("WHERE BB.SPECIES_KEY = IB.REC_KEY ")
	_T("AND BB.REG_NO IS NOT NULL ")
	_T("AND BB.WORKNO IN (%s) ")

	_T("UNION ALL ")

	_T("SELECT BB.REG_NO,")
	   _T("IB.TITLE_INFO,")
	   _T("IB.AUTHOR_INFO,")
	   _T("BB.WORKNO,")
	   _T("'대기중', NULL ")
	_T("FROM IDX_SE_TBL IB,")
	   _T("SE_BOOK_TBL BB ")
	_T("WHERE BB.SPECIES_KEY = IB.REC_KEY ")
	_T("AND BB.REG_NO IS NOT NULL ")
	_T("AND BB.WORKNO IN (%s)"), strSerial, strSerial);
	
	if(strSerial.IsEmpty())
	{
		m_DM.FreeData();		
	}
	else
	{
		m_DM.RestructDataManager(strSQL);
		
		
  }

	INT nPrevCnt = m_PrevDM.GetRowCount();	
	INT nCnt = 0, nRow = 0;	
	for(nRow = nPrevCnt-1; nRow >= 0 ; nRow--)
	{
		if(0 == m_DM.GetRowCount())
		{
			MoveRow(&m_PrevDM, nRow, &m_DM, -1);
		}
		else
		{
			MoveRow(&m_PrevDM, nRow, &m_DM, 0);
		}
	}

	DisplayGrid();

	return 0;
#endif
}

INT CILL_RFID_SECURITY_CHANGE2::MakeTransNoList(CStringArray& aTransNoList, CString& strTransNoList)
{
	INT nCnt = 0;
	const INT SIZE = aTransNoList.GetSize();
	CString strTransNo;
	strTransNoList.Empty();

	for(nCnt = 0; nCnt < SIZE; nCnt++)
	{		
		strTransNoList = strTransNoList + _T(",'") + aTransNoList.GetAt(nCnt) + _T("'");
	}
	strTransNoList.TrimLeft(_T(","));

	return 0;
}

UINT CILL_RFID_SECURITY_CHANGE2::RefreshListThread(LPVOID pParam)
{	
	CILL_RFID_SECURITY_CHANGE2* pDlg = (CILL_RFID_SECURITY_CHANGE2*)pParam;

	if(!pDlg->m_bIsCancelThread)
	{
		if(TRUE == pDlg->m_bIsRunningThread)
		{
			return 1000;
		}

		pDlg->m_bIsRunningThread = TRUE;
		pDlg->RefreshList();	
		pDlg->m_bIsRunningThread = FALSE;
	}
	pDlg->SetTimer(2, 100, NULL);

	return 0;
}


void CILL_RFID_SECURITY_CHANGE2::InitGrid()
{
	m_pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_grid_RFID_INFO);

	COleFont font = m_pGrid->GetFont();
	CY cy = font.GetSize();
	cy.Lo = 90000;
	font.SetName(_T("새굴림"));
	font.SetItalic(FALSE);	
	font.SetUnderline(FALSE);
	font.SetStrikethrough(FALSE);		
	font.SetSize(cy);
	font.SetBold(FALSE);
	m_pGrid->SetRefFont(font);
	
	m_pGrid->SetRowHeight(-1, 255);	
	m_pGrid->SetRowHeight(0, 391);

	const INT COL_CNT = 5;
	CString strCaption[COL_CNT] =
	{
		_T(""), _T("등록번호"), _T("서명"), _T("저자"), _T("상태")
	};
	INT nColWidth[COL_CNT] = { 20, 100, 220, 80, 100 };
	INT nColAlignment[COL_CNT] = {7,1,1,1,1};
	
	m_pGrid->SetCols(0, COL_CNT); 
	m_pGrid->SetRow(0);
	INT nColCnt = (INT)m_pGrid->GetCols(0);
	for(INT nCnt = 0; nCnt < nColCnt; nCnt++)
	{
		m_pGrid->SetCol(nCnt);
		m_pGrid->SetColWidth(nCnt, 0, nColWidth[nCnt] * 15);
		if(0 == nCnt)
		{
			m_pGrid->SetColAlignmentFixed(nCnt, 7);
		}
		else
		{
			m_pGrid->SetColAlignmentFixed(nCnt, 4);
		}
		m_pGrid->SetColAlignment(nCnt, nColAlignment[nCnt]);
		m_pGrid->SetTextMatrix(0, nCnt, strCaption[nCnt]);
	}
}


void CILL_RFID_SECURITY_CHANGE2::DisplayGrid()
{	
	if(NULL == m_pGrid || NULL == m_pGrid->GetSafeHwnd())
	{
		return;
	}

	INT nMax = m_DM.GetRowCount();
	INT nStart = 0;
	INT nEnd = (1 < nMax) ? nMax+1 : 2;
	
	m_pGrid->SetRedraw(FALSE);
	m_pGrid->SetRows(nEnd);
	
	for(INT nRow = 1; nRow < nEnd; nRow++)
	{
 		m_pGrid->SetTextMatrix(nRow, 1, m_DM.GetCellData(nRow-1, 0));
 		m_pGrid->SetTextMatrix(nRow, 2, m_DM.GetCellData(nRow-1, 1));
 		m_pGrid->SetTextMatrix(nRow, 3, m_DM.GetCellData(nRow-1, 2));
 		m_pGrid->SetTextMatrix(nRow, 4, m_DM.GetCellData(nRow-1, 4));	
	}	

	m_pGrid->Refresh();
	m_pGrid->SetRedraw(TRUE);
}

INT CILL_RFID_SECURITY_CHANGE2::MoveRow(CESL_DataMgr* pSrcDM, INT nSrcRow, CESL_DataMgr* pDstDM, INT nDstRow)
{
	if(NULL == pSrcDM || NULL == pDstDM)
	{
		return -1000;
	}
	if(0 > nSrcRow || pSrcDM->GetRowCount() <= nSrcRow)
	{
		return -1010;
	}
	if(pDstDM->GetRowCount() <= nDstRow)
	{
		return -1020;
	}

	DWORD* row = (DWORD*)pSrcDM->data.GetAt(pSrcDM->m_pDataPosition[nSrcRow]);
	if(0 > nDstRow)
	{
		pDstDM->data.AddTail((CObject*)row);
	}
	else
	{
		POSITION pos = pDstDM->data.FindIndex(nDstRow);
		pDstDM->data.InsertBefore(pos, (CObject*)row);
	}

	pSrcDM->data.RemoveAt(pSrcDM->m_pDataPosition[nSrcRow]);
	pSrcDM->RefreshPositionInfo();
	pDstDM->RefreshPositionInfo();


	return 0;
}