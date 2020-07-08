// KL_OTHER_RETURN_REQUEST.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_RETURN_REQUEST.h"
#include "FrameManager.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"
#include "KL_OTHER_RETURN_DELIVERY.h"

#include "..\\..\\..\\..\\공통\\BO_ACQ_PRINT_MGR\\BasicPrintMgr.h"
#include <map>
#include "KL_OTHER_RETURN_EDIT_MEMO.h"
#include "KL_OTHER_RETURN_DETAILVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_REQUEST dialog


CKL_OTHER_RETURN_REQUEST::CKL_OTHER_RETURN_REQUEST(CESL_Mgr* pParent /*=NULL*/)
	: CKL_OTHER_RETURN_DEFAULT(pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_RETURN_REQUEST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strSMAlias = _T("SM_KL_OTHER_RETURN_MNG_REQUEST");
	m_strCMAlias = _T("CM_KL_OTHER_RETURN_MNG_REQUEST");
	m_strDMAlias = _T("DM_KL_OTHER_RETURN_MNG_REQUEST");
	m_pParent = pParent;
	m_API = new CGipyoPrintAPI();
}

CKL_OTHER_RETURN_REQUEST::~CKL_OTHER_RETURN_REQUEST()
{
	m_API->FreePrintLibrary();
	delete m_API;
	m_API = NULL;
}

BOOL CKL_OTHER_RETURN_REQUEST::Create(CWnd* pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

void CKL_OTHER_RETURN_REQUEST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_RETURN_REQUEST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_RETURN_REQUEST, CKL_OTHER_RETURN_DEFAULT)
	//{{AFX_MSG_MAP(CKL_OTHER_RETURN_REQUEST)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_cmb, OnSelchangecmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_REQUEST message handlers

BOOL CKL_OTHER_RETURN_REQUEST::OnInitDialog() 
{
	CKL_OTHER_RETURN_DEFAULT::OnInitDialog();
	
	ResetComboBox();
	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)FindControl(	m_strCMAlias, _T("멀티콤보_진행상태"));
	if(NULL == pCMB) return FALSE;	
	
	CString strData = _T("");
	int ROW = pCMB->GetCount();
	for(int nCnt = 0; nCnt < ROW; nCnt++)
	{
		strData = pCMB->GetItemDataString(nCnt, 1);
		if(_T("반납") == strData)
		{
			pCMB->SetCurSel(nCnt);
			break;
		}
	}

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("타관반납관리발송");
	}

	if (m_pLoanManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanManageValue->m_sLoanUserAccessMode == _T("1") )
	{
		m_pGrid->SetColumnEncryption(_T("이용자명"),TRUE);
		m_pGrid->SetColumnEncryption(_T("생년월일"),TRUE);
	}

	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_RETURN_REQUEST::ResizeControl()
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_grid);
	if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) return FALSE;
	
	CRect rDlg = CRect(0, 0, 0, 0);
	GetClientRect(rDlg);

	rDlg.top += 70;
	
	pWnd->MoveWindow(rDlg);

	return TRUE;
}

INT CKL_OTHER_RETURN_REQUEST::ResetComboBox()
{
	int ids = -1;

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox*)
		FindControl(m_strCMAlias, _T("멀티콤보_진행상태"));
	if(!pCMB)
	{
		return -1000;
	}

	const int COL_COUNT = 2;
	pCMB->ResetContent();
	pCMB->FreeComboData();
	pCMB->InitMultiComboBox(COL_COUNT);	

	const int STATE_COUNT = 3;
	CString strCmbData[STATE_COUNT][COL_COUNT] = 
	{		
		{ _T("0"), _T("반납")},
		{ _T("1"), _T("발송")},
		{ _T("2"), _T("복귀")}
	};

	m_strDescription[0] = _T("자관에서 타관 책을 반납하여 발송 대기중인 목록");
	m_strDescription[1] = _T("자관에서 타관 책을 반납하여 소장도서관으로 발송한 목록");
	m_strDescription[2] = _T("자관에서 타관 책을 반납하여 소장도서관으로 발송된 자료가 복귀된 목록");

	for(int nRow = 0; nRow < STATE_COUNT; nRow++)
	{
		for(int nCol = 0; nCol < 2; nCol++)
		{
			pCMB->AddItem(strCmbData[nRow][nCol], nCol, nRow);
		}
	}

	return 0;
}

void CKL_OTHER_RETURN_REQUEST::OnSize(UINT nType, int cx, int cy) 
{
	CKL_OTHER_RETURN_DEFAULT::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizeControl();
}

INT CKL_OTHER_RETURN_REQUEST::Return()
{
	int ids = 0;
	CString strFuncName = _T("CKL_OTHER_RETURN_REQUEST::Return()");

	{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
		
	CStringArray aCheckArray;
	aCheckArray.Add(_T("트랜젝션상태|!=|0"));
	aCheckArray.Add(_T("반납도서관부호|!=|") + m_pInfo->LIB_CODE);
	
	ids = UnCheckGrid(aCheckArray);
		
	}
	
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(
			_T("선정된 항목중 발송 가능한 항목이 없습니다.\r\n")
			_T("발송이 불가능한 대상은 아래와 같습니다.\r\n\r\n")			
			_T("1. 진행상태가 반납(0)이 아닌 자료\r\n")
			);
		return -1;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
	
	CFrameWriter Writer(m_pInfo);
	CFrameSender Sender(m_pInfo);
	CFrameReader Reader;
	CString strData;
	
	m_pGrid->SelectMakeList();
	if(0 < m_pGrid->SelectGetCount())
	{
		CString strReturnDate, strLoanLibCode, strLoanKey;
		CTime Today = CLocCommonAPI::GetCurrentDateTime(this, strReturnDate);

		INT nIdx = m_pGrid->SelectGetHeadPosition();
		do
		{
			Writer.clear();
			Writer.setCommand(_T("IL_K19_LOC_B01_SERVICE"));
			Writer.addRecord();
 			Writer.addElement(_T("트랜젝션상태"), _T("1"));
			strLoanLibCode = m_pDM->GetCellData(_T("대출도서관부호"), nIdx);
 			Writer.addElement(_T("대출도서관"), strLoanLibCode);
			strLoanKey = m_pDM->GetCellData(_T("로컬KEY"), nIdx);
			Writer.addElement(_T("로컬KEY"), strLoanKey);
 			Writer.addElement(_T("타관반납발송일"), strReturnDate);
 			Writer.addElement(_T("복귀일"), _T(""));
 			Writer.addElement(_T("발송자"), m_pInfo->USER_ID);
 			Writer.addElement(_T("입수자"), _T(""));
			Writer.addElement(_T("마지막작업"), GetWorkLogMsg(_T("발송"),__WFILE__ ,__LINE__));

			BOOL bSuccese = FALSE;
			Reader.clear();
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids)
			{
				 bSuccese = FALSE;
			}
			else
			{
 				CESL_DataMgr TempDM;
 				Reader.makeDM(&TempDM);
 				CString strTemp = Reader.getCommand();
 				if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
				{
					bSuccese = FALSE;
				}
				else
				{
					TempDM.GetCellData(_T("처리결과"), 0, strData);
					if(_T("OK") == strData)
					{
						bSuccese = TRUE;
					}
					else
					{
						bSuccese = FALSE;
					}
				}
			}
			if(TRUE == bSuccese)
			{
				m_pDM->SetCellData(_T("처리결과"), _T("성공"), nIdx);
			}
			else
			{
				m_pDM->SetCellData(_T("처리결과"), _T("실패"), nIdx);
			}
			nIdx = m_pGrid->SelectGetNext();
		}
		while(0 <= nIdx);
	}

	if(0 < m_pGrid->SelectGetCount())
	{
		INT nIdx = m_pGrid->SelectGetTailPosition();
		do
		{
			m_pDM->GetCellData(_T("처리결과"), nIdx, strData);
			if(_T("성공") == strData)
			{
				m_pDM->DeleteRow(nIdx);
			}
			nIdx = m_pGrid->SelectGetPrev();
		}
		while(0 <= nIdx);
	}
	m_pGrid->Display();

	return ids;
}

INT CKL_OTHER_RETURN_REQUEST::ReturnCancel()
{
	int ids = 0;
	CString strFuncName = _T("ReturnCancel");
	CString strState = GetComboBoxState();
	if(_T("0") != strState && _T("1") != strState)
	{
		AfxMessageBox(_T("발송자료관리에서는 반납과 발송인 상태만 취소 작업이 가능합니다."));
		return -1;
	}
	m_pGrid->SelectMakeList();
	if(1 > m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}
	if(1 < m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("작업취소는 한건씩 처리가능합니다.\r\n취소할 자료 하나만 선정해주십시오."));
		return -1;
	}
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	CString strReturnPlanDate = m_pDM->GetCellData(_T("반납예정일"), nIdx);

	CString strReturnLib = m_pDM->GetCellData(_T("반납도서관"), nIdx);
	CString strOtherLoanKey = m_pDM->GetCellData(_T("타관대출KEY"), nIdx);
	if(strReturnLib.IsEmpty())
	{
		ids = ReturnCancel_Send(strOtherLoanKey);
		return 0;
	}

	if(10 > strReturnPlanDate.GetLength())
	{
		AfxMessageBox(_T("반납예정일에 오류가 있습니다."));
		return -1;
	}
	CString strReturnDate = m_pDM->GetCellData(_T("반납일"), nIdx);
	if(10 > strReturnDate.GetLength())
	{
		AfxMessageBox(_T("반납일에 오류가 있습니다."));
		return -1;
	}

	CString strCurrentDate;
	CTime t = CLocCommonAPI::GetCurrentDateTime(this, strCurrentDate);
	if(_T("0") == strState)
	{
		if(strReturnDate.Mid(0,10) != strCurrentDate.Mid(0,10))
		{
			AfxMessageBox(_T("당일 처리한 자료만 취소 작업이 가능합니다."));
			return -1;
		}
	}
	else
	{
		CString strRequestDate = m_pDM->GetCellData(_T("타관반납발송일"), nIdx);
		if(10 > strRequestDate.GetLength())
		{
			AfxMessageBox(_T("발송일에 오류가 있습니다."));
			return -1;
		}
		if(strRequestDate.Mid(0,10) != strCurrentDate.Mid(0,10))
		{
			AfxMessageBox(_T("당일 처리한 자료만 취소 작업이 가능합니다."));
			return -1;
		}

	}

	if(_T("0") == strState)
	{
		CString strDate1 = strReturnPlanDate.Left(10);
		strDate1.Replace(_T("/"), _T(""));
		CString strDate2 = strReturnDate.Left(10);
		strDate2.Replace(_T("/"), _T(""));
		if(strDate1 < strDate2)
		{
			AfxMessageBox(_T("연체 자료는 타관반납 취소할 수 없습니다."));
			return -1;
		}
	}

	CString strOfferLibCode;
	strOfferLibCode = m_pDM->GetCellData(_T("제공도서관부호"),nIdx);
	if(!strOfferLibCode.IsEmpty())
	{
		CString strLoanKey;
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
		CFrameWriter Writer(m_pInfo);
		CFrameSender Sender(m_pInfo);
		CFrameReader Reader;
		CString strData;
		CString strReturnLibCode, strLoanLibCode;

		CString strTargetStatus = _T("");
		if ( strState == _T("0") ) {
			strTargetStatus = _T("4");
		}
		else if ( strState == _T("1") ) {
			strTargetStatus = _T("5");
		}

		Writer.clear();
		Writer.setCommand(_T("LILL_K02_LOC_B02_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("LOAN_KEY"), strOtherLoanKey);
		Writer.addElement(_T("TRANSACTION_CODE"), strTargetStatus);
		
		BOOL bSuccese = FALSE;
		Reader.clear();
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			bSuccese = FALSE;
		}
		else
		{
			CESL_DataMgr TempDM;
			Reader.makeDM(&TempDM);
			CString strTemp = Reader.getCommand();
			if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
			{
				bSuccese = FALSE;
			}
			else
			{
				TempDM.GetCellData(_T("RESULT"), 0, strData);
				if(_T("OK") == strData)
				{
					bSuccese = TRUE;
				}
				else
				{
					bSuccese = FALSE;
				}
			}
		}
		if(TRUE == bSuccese)
		{
			m_pDM->SetCellData(_T("처리결과"), _T("성공"), nIdx);
			Refresh();

			if ( strState == _T("0") ) {
				AfxMessageBox(_T("타관반납이 취소 되었습니다."));
			}
			else if ( strState == _T("1") ) {
				AfxMessageBox(_T("발송이 취소 되었습니다."));
			}			
		}
		else
		{
			m_pDM->SetCellData(_T("처리결과"), _T("실패"), nIdx);
			m_pGrid->Display();
		}
		
		
		
	}
	else
	{
		
		
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		TmpDM.SetManageCode(m_pInfo->MANAGE_CODE);
		CFrameWriter Writer(m_pInfo);
		CFrameSender Sender(m_pInfo);
		CFrameReader Reader;
		CString strData;
		CString strReturnLibCode, strLoanLibCode, strLoanKey;
		Writer.clear();
		Writer.setCommand(_T("IL_K41_LOC_B01_SERVICE"));
		Writer.addRecord();
		strLoanLibCode = m_pDM->GetCellData(_T("대출도서관부호"), nIdx);
		Writer.addElement(_T("대출도서관부호"), strLoanLibCode);
		strReturnLibCode = m_pDM->GetCellData(_T("반납도서관부호"), nIdx);
		Writer.addElement(_T("반납도서관부호"), strReturnLibCode);
		CString strPaymentKey = m_pDM->GetCellData(_T("결제처리KEY"), nIdx);
		strLoanKey = m_pDM->GetCellData(_T("로컬KEY"), nIdx);
		Writer.addElement(_T("로컬KEY"), strLoanKey);
		if(!strPaymentKey.IsEmpty()) Writer.addElement(_T("결제처리KEY"), strPaymentKey);
		Writer.addElement(_T("트렌젝션"), strState);
		Writer.addElement(_T("마지막작업"), GetWorkLogMsg(_T("타관반납관리취소"),__WFILE__ ,__LINE__));
		BOOL bSuccese = FALSE;
		Reader.clear();
		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
			bSuccese = FALSE;
		}
		else
		{
			CESL_DataMgr TempDM;
			Reader.makeDM(&TempDM);
			CString strTemp = Reader.getCommand();
			if(_T("ERROR") == strTemp || 1 > TempDM.GetRowCount())
			{
				bSuccese = FALSE;
			}
			else
			{
				TempDM.GetCellData(_T("처리결과"), 0, strData);
				if(_T("OK") == strData)
				{
					bSuccese = TRUE;
				}
				else
				{
					bSuccese = FALSE;
				}
			}
		}
		if(TRUE == bSuccese)
		{
			m_pDM->SetCellData(_T("처리결과"), _T("성공"), nIdx);
			Refresh();
			if ( strState == _T("0") ) {
				AfxMessageBox(_T("타관반납이 취소 되었습니다."));
			}
			else if ( strState == _T("1") ) {
				AfxMessageBox(_T("발송이 취소 되었습니다."));
			}			
		}
		else
		{
			m_pDM->SetCellData(_T("처리결과"), _T("실패"), nIdx);
			m_pGrid->Display();
		}
	}

	return ids;
}

void CKL_OTHER_RETURN_REQUEST::OnSelchangecmb() 
{
	// TODO: Add your control notification handler code here
	Refresh();
}


INT CKL_OTHER_RETURN_REQUEST::Refresh()
{
	CString strState = GetComboBoxState();

	int nState = _wtoi(strState);
	if(nState < 0)
	{
		nState = 0;		
	}
	SetDlgItemText(IDC_stext_state_description,m_strDescription[nState]);	

	BOOL bShow = FALSE;
	if(_T("0") == strState || _T("1") == strState) bShow = TRUE;
	m_pParent->SendMessage(WM_KL_RETURN_CANCEL_CTL, NULL, (LPARAM)bShow);
	m_pParent->SendMessage(WM_KL_ACTIVE_BUTTON, NULL, (LPARAM)nState);
	
	SetDateControlView();

	if(m_bIsSearched)
	{
		return 1000;
	}	

	m_bIsSearched = TRUE;
	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K18_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("트랜젝션상태"), strState);
 	Writer.addElement(_T("반납도서관"), m_pInfo->LIB_CODE);
	Writer.addElement(_T("지역상호대차대출도서관"), m_pInfo->LIB_CODE);
	if(_T("0") == strState)
	{
		Writer.addElement(_T("ORDER_BY"), _T("RETURN_DATE DESC"));
	}
	else if(_T("1") == strState)
	{
		Writer.addElement(_T("ORDER_BY"), _T("TR_SEND_DATE DESC"));
	}
	else if(_T("2") == strState)
	{
		Writer.addElement(_T("복귀일부터"), GetBeforeDate());
		Writer.addElement(_T("복귀일까지"), GetAfterDate());
		Writer.addElement(_T("ORDER_BY"), _T("TR_RETURN_DATE DESC"));
	}
	CString szData = Writer.getFrame();
	CFrameSender Sender(m_pInfo);	
	Sender.CommMiddleWareDaemon(szData, szData);
	m_pDM->FreeData();
	CFrameReader Reader;
	Reader.parseFrame(szData);
	Reader.makeDM(m_pDM);
	SetDescDM();
	for(int i = 0; i<10; i++)
	{
	CString strReturnDate= m_pDM->GetCellData(_T("반납일"), i);
	CString strLocalKey= m_pDM->GetCellData(_T("로컬KEY"), i);
	TRACE(strReturnDate);
	TRACE(strLocalKey);
}

	CString strUserNo = _T("");
	for(i=0; i<m_pDM->GetRowCount(); i++)
	{
		strUserNo = m_pDM->GetCellData(_T("이용자번호"), i);
		if(strUserNo.Compare(_T(""))!=0)
		{
			m_pDM->SetCellData(_T("이용자명"), GetUserName(strUserNo), i);
			m_pDM->SetCellData(_T("생년월일"), GetBirthDay(strUserNo), i);
			m_pDM->SetCellData(_T("대출자직급설명"), GetUserClassDesc(strUserNo), i);
		}		
	}

	SetTimer(1, 100, NULL);

	return 0;
}

INT CKL_OTHER_RETURN_REQUEST::DoRefresh()
{
	if(m_bIsSearched)
	{
		return 1000;
	}
	EnableWindow(FALSE);

	AfxBeginThread(OtherReturnRequestThread, this);

	return 0;
}

UINT CKL_OTHER_RETURN_REQUEST::OtherReturnRequestThread(LPVOID lParam)
{
	CKL_OTHER_RETURN_REQUEST* pDlg = (CKL_OTHER_RETURN_REQUEST*)lParam;
	pDlg->Refresh();

	return 0;
}


INT CKL_OTHER_RETURN_REQUEST::PrintGipyo()
{
	int ids = 0;
	CString strFuncName = _T("CKL_OTHER_RETURN_REQUEST::PrintGipyo()");

	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -1;
	}

	AfxMessageBox(_T("선정된 항목중 결제되지 않은 항목은 출력되지 않으며, 결제처리KEY가 같은 항목은 일괄 출력 됩니다."));
	
	m_pGrid->SelectMakeList();
	INT nIdx = m_pGrid->SelectGetHeadPosition();	
	   
	CString strarrData[57];

	while(0 <= nIdx)
	{
		
		CString strPaymentKey = m_pDM->GetCellData(_T("결제처리KEY"), nIdx);
		if(_T("") == strPaymentKey) 
		{
			continue;	
		}
		else 
		{

			CFrameWriter Writer(m_pInfo);

			Writer.setCommand(_T("IL_K37_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("결제처리KEY"), strPaymentKey);
			Writer.addElement(_T("모드"), _T("Request"));

			CFrameReader Reader;
			CFrameSender Sender(m_pInfo);
			INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids)
			{
				//에러코드 처리
				CString strMsg = Sender.getError(ids);
				if(strMsg.IsEmpty())
				{
					AfxMessageBox(_T("미들웨어 통신 실패"));
				}
				else if(_T("[MW95TCTB01DT01]") == strMsg.Right(16))
				{
					AfxMessageBox(_T("정확하지 않은 도서관부호입니다."));
				}
				else if(_T("[MK95LOCR01DB01]") == strMsg.Right(16))
				{
					AfxMessageBox(_T("검색결과가 없습니다."));
				}		
				else
				{
					AfxMessageBox(_T("Error!!!"));
				}
				return -100;
			}

			CESL_DataMgr TmpDM;
			TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			ids = Reader.makeDM(&TmpDM);
			
			CString strData;
			INT nInsertRow = 0;
			INT nRowCnt = TmpDM.GetRowCount();
			CString strTemp = Reader.getCommand();

			if(1 > TmpDM.GetRowCount())
			{
				return -200;
			}		
			

			CString strBook1 = TmpDM.GetCellData(_T("상품명1"), 0);
			CString strBook2 = TmpDM.GetCellData(_T("상품명2"), 0);
			CString strBook3 = TmpDM.GetCellData(_T("상품명3"), 0);
			CString strBook4 = TmpDM.GetCellData(_T("상품명4"), 0);
			CString strBook5 = TmpDM.GetCellData(_T("상품명5"), 0);
			CString strGood1, strGood2;

			if(_T("") != strBook1 && _T("") == strBook2 && _T("") == strBook3 && _T("") == strBook4 && _T("") == strBook5)
			{
				strGood1 = strBook1;
				strGood2 = _T("");
			}
			else if(_T("") != strBook1 && _T("") != strBook2 && _T("") == strBook3 && _T("") == strBook4 && _T("") == strBook5)
			{
				strGood1 = strBook1 + _T(",") + strBook2;
				strGood2 = _T("");
			}
			else if(_T("") != strBook1 && _T("") != strBook2 && _T("") != strBook3 && _T("") == strBook4 && _T("") == strBook5)
			{
				strGood1 = strBook1 + _T(",") + strBook2 + _T(",") + strBook3;
				strGood2 = _T("");
			}
			else if(_T("") != strBook1 && _T("") != strBook2 && _T("") != strBook3 && _T("") != strBook4 && _T("") == strBook5)
			{
				strGood1 = strBook1 + _T(",") + strBook2 + _T(",") + strBook3;
				strGood2 = strBook4;
			}
			else if(_T("") != strBook1 && _T("") != strBook2 && _T("") != strBook3 && _T("") != strBook4 && _T("") != strBook5)
			{
				strGood1 = strBook1 + _T(",") + strBook2 + _T(",") + strBook3;
				strGood2 = strBook4 + _T(",") + strBook5;
			}
			else
			{
				return -300;
			}

			strarrData[0] = TmpDM.GetCellData(_T("주문자성명"), 0);
			strarrData[1] = TmpDM.GetCellData(_T("주문자전화번호"), 0);
			strarrData[2] = TmpDM.GetCellData(_T("주문자기타전화번호"), 0);
			strarrData[3] = TmpDM.GetCellData(_T("주문자우편번호"), 0);
			strarrData[4] = TmpDM.GetCellData(_T("주문자주소"), 0);
			strarrData[5] = TmpDM.GetCellData(_T("주문자상세주소"), 0);
			strarrData[6] = TmpDM.GetCellData(_T("수취인성명"), 0);
			strarrData[7] = TmpDM.GetCellData(_T("수취인전화번호"), 0);
			strarrData[8] = TmpDM.GetCellData(_T("수취인기타전화번호"), 0);
			strarrData[9] = TmpDM.GetCellData(_T("수취인우편번호"), 0);
			strarrData[10] = TmpDM.GetCellData(_T("수취인주소"), 0);
			strarrData[11] = TmpDM.GetCellData(_T("수취인상세주소"), 0);
			strarrData[12] = TmpDM.GetCellData(_T("우편물중량"), 0);
			strarrData[13] = TmpDM.GetCellData(_T("우편물부피"), 0);
			strarrData[14] = TmpDM.GetCellData(_T("배송비"), 0);
			strarrData[15] = _T("16");									
			strarrData[16] = TmpDM.GetCellData(_T("등기번호"), 0);
			strarrData[17] = strGood1;									
			strarrData[18] = strGood2;									
			strarrData[19] = TmpDM.GetCellData(_T("배송메시지"), 0);
			strarrData[20] = TmpDM.GetCellData(_T("배송메시지"), 0);
			strarrData[21] = _T("");									
			strarrData[22] = _T("1");									
			strarrData[23] = _T("");									
			strarrData[24] = TmpDM.GetCellData(_T("주문자성명"), 0);
			strarrData[25] = _T("");								    
			strarrData[26] = _T("");							 			
			strarrData[27] = _T("");							 			
			strarrData[28] = _T("");										
			strarrData[29] = _T("");										
			strarrData[30] = _T("");										
			strarrData[31] = _T("");										
			strarrData[32] = _T("");										
			strarrData[33] = TmpDM.GetCellData(_T("주문자전화번호"), 0);	
			strarrData[34] = _T("");										
			strarrData[35] = TmpDM.GetCellData(_T("주문자주소"), 0);
			strarrData[36] = TmpDM.GetCellData(_T("주문자상세주소"), 0);
			strarrData[37] = _T("");										
			strarrData[38] = _T("");										
			strarrData[39] = _T("");										
			strarrData[40] = _T("");										
			strarrData[41] = _T("");										
			strarrData[42] = _T("");										
			strarrData[43] = _T("");										
			strarrData[44] = _T("");										
			strarrData[45] = _T("");										
			strarrData[46] = _T("");										
			strarrData[47] = _T("0");									    
			strarrData[48] = _T("");										
			strarrData[49] = _T("N");										
			strarrData[50] = _T("");										
			strarrData[51] = _T("");										
			strarrData[52] = _T("");										
			strarrData[53] = _T("");										
			strarrData[54] = _T("");										
			strarrData[55] = TmpDM.GetCellData(_T("주문자상세주소"), 0);	
			strarrData[56] = _T("N");										

			m_API->LoadPrintLibrary();
			
			CString strTmpData = m_API->MakeDataFormat(strarrData);

			if(_T("") != strTmpData)
			{
				m_API->ConvertUnicodeToMBCS(strTmpData);
				m_API->PutEmsData(m_API->m_postData);
			}
		}
		nIdx = m_pGrid->SelectGetNext();
	}
	m_API->ExecPrint();
	m_API->FreePrintLibrary();

	return 0;
}

INT CKL_OTHER_RETURN_REQUEST::ExeDeliveryInfo()
{
	m_pGrid->SelectMakeList();
	if(0 >= m_pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return -100;
	}

	INT nIdx = m_pGrid->SelectGetIdx();
	if(0 <= nIdx)
	{
		DeliveryInfo(nIdx, 0);
	}
	else
	{
		return -200;
	}
	return 0;
}

INT CKL_OTHER_RETURN_REQUEST::DeliveryInfo(INT nIdx, INT nSwitch)
{
	int ids = 0;
	CString strFuncName = _T("CKL_OTHER_RETURN_REQUEST::DeliveryInfo()");

	if(0 > nIdx)
	{
		return -100;
	}
	
	CString strPaymentKey = m_pDM->GetCellData(_T("결제처리KEY"), nIdx);
	if(_T("") == strPaymentKey)
	{
		AfxMessageBox(_T("조회대상 항목이 아닙니다."));
		return -200;
	}

	CString strLoanLibcode = m_pDM->GetCellData(_T("대출도서관부호"), nIdx);
	CString strReturnLibcode = m_pDM->GetCellData(_T("반납도서관부호"), nIdx);
	if(_T("") == strLoanLibcode || _T("") == strReturnLibcode)
	{
		AfxMessageBox(_T("항목이 없습니다."));
		return -300;
	}
	if(0 != nSwitch)
	{
		CKL_OTHER_RETURN_DELIVERY dlg(this, nIdx);
		dlg.DoModal();
	}
	else 
	{
		CKL_OTHER_RETURN_DELIVERY dlg(this);
		dlg.DoModal();
	}
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CKL_OTHER_RETURN_REQUEST, CKL_OTHER_RETURN_DEFAULT)
    //{{AFX_EVENTSINK_MAP(CKL_OTHER_RETURN_REQUEST)
	ON_EVENT(CKL_OTHER_RETURN_REQUEST, IDC_grid, -601 /* DblClick */, OnDblClickgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CKL_OTHER_RETURN_REQUEST::OnDblClickgrid() 
{
	INT nIdx = m_pGrid->GetIdx();

	if(0 <= nIdx)
	{
		DeliveryInfo(nIdx, 1);
	}
	else
	{
		return;
	}

	// TODO: Add your control notification handler code here
	
}

INT CKL_OTHER_RETURN_REQUEST::ReturnCancel_Send(CString strOtherLoanKey)
{
	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader;
	CFrameWriter Writer(m_pInfo);
	
	INT ids=0;

	INT nIdx = m_pGrid->SelectGetTailPosition();
	for(int i=0;i<m_pGrid->SelectGetCount();i++)
	{
		Writer.clear();
		Reader.clear();
		
		Writer.setCommand(_T("LILL_K02_LOC_B04_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("타관대출KEY"),strOtherLoanKey);

		ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if (0 > ids) 
		{
			while(true) 
			{
				if(IDYES == MessageBox(_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("조회실패"), MB_YESNO | MB_ICONQUESTION)) 
				{
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if(0 <= ids) break;
				}
				else
				{
					return 0;
				}
			}
		}
		m_pDM->DeleteRow(nIdx);
		nIdx=m_pGrid->SelectGetPrev();
	}
	AfxMessageBox(_T("작업 취소 완료"));
	m_pGrid->Display();
	return 0;		
}

CString CKL_OTHER_RETURN_REQUEST::GetUserName(CString strUserNo)
{
	int ids = 0;
	CString strUserName = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT name FROM co_loan_user_tbl WHERE user_no = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strUserName);
	if(ids < 0)
	{
		strUserName = _T("");
	}
	return strUserName;
}

CString CKL_OTHER_RETURN_REQUEST::GetBirthDay(CString strUserNo)
{
	int ids = 0;
	CString strBirthDay = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT BIRTHDAY FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strBirthDay);
	if(ids < 0)
	{
		strBirthDay = _T("");
	}
	return strBirthDay;
}

CString CKL_OTHER_RETURN_REQUEST::GetUserClassDesc(CString strUserNo)
{
	int ids = 0;
	CString strUserClassDesc = _T(""), strQuery = _T("");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	strQuery.Format(_T("SELECT ECO$F_GET_CODE2DESC('31', USER_CLASS_CODE) FROM CO_LOAN_USER_TBL WHERE USER_NO = '%s'"), strUserNo);
	ids = TmpDM.GetOneValueQuery(strQuery,strUserClassDesc);
	if(ids < 0)
	{
		strUserClassDesc = _T("");
	}
	return strUserClassDesc;
}

BOOL CKL_OTHER_RETURN_REQUEST::PrintWaybill()
{

EFS_BEGIN

	CESL_DataMgr* pSrcDM = FindDM(_T("DM_KL_OTHER_RETURN_MNG_REQUEST"));
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
	
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_KL_OTHER_RETURN_MNG_REQUEST") , _T("GRID") );
	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nSelectCnt = pGrid->SelectGetCount();

	if(nSelectCnt <= 0)
	{
		AfxMessageBox( _T("선택 사항이 없습니다.") );
		return 1;
	}

	CESL_DataMgr *pPrintDM = FindDM( _T("DM_KL_OTHER_RETURN_MNG_REQUEST_PRINT") );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
	pPrintDM->FreeData();

	const INT nCopyCnt = 8;
	TCHAR *sField[nCopyCnt][2] =
	{
		{_T("이용자번호"),_T("이용자번호")},
		{_T("이용자명"),_T("이용자명")},
		{_T("등록번호"),_T("등록번호")},
		{_T("서명"),_T("서명")},
		{_T("발행자"),_T("발행자")},
		{_T("청구기호"),_T("청구기호")},				
		{_T("부록발송유무"),_T("부록발송유무")},
		{_T("메모"),_T("메모")},		
	};

	CString sData, sData2;
	INT nIndex;
	INT ids;

	pGrid->SelectGetHeadPosition();


	std::map<CString, CString> mapLib;
	std::map<CString, CString>::iterator iter;
	
	CString strDate = pSrcDM->GetDate().Left(10);
	
	for( INT i = 0 ; i < nSelectCnt ; i++)
	{
		nIndex = pGrid->SelectGetIdx();

		ids = pSrcDM->GetCellData( _T("제공도서관부호") , nIndex , sData );

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );

		if(sData.Compare(_T("")) == 0)
		{
			ids = pSrcDM->GetCellData( _T("대출도서관부호") , nIndex , sData );

			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );

			ids = pSrcDM->GetCellData( _T("대출도서관") , nIndex , sData2 );

			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
		}
		else
		{
			ids = pSrcDM->GetCellData( _T("제공도서관") , nIndex , sData2 );

			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
		}		

		mapLib.insert(std::map<CString, CString>::value_type(sData, sData2));
				
		pGrid->SelectGetNext();
	}

	for(iter = mapLib.begin(); iter != mapLib.end(); iter++)
	{
		int nRowNum = 0;

		pPrintDM->FreeData();

		pGrid->SelectGetHeadPosition();

		for( i = 0 ; i < nSelectCnt ; i++ )
		{
			nIndex = pGrid->SelectGetIdx();

			ids = pSrcDM->GetCellData( _T("제공도서관부호") , nIndex , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
			
			if(sData.Compare(_T("")) == 0)
			{
				ids = pSrcDM->GetCellData( _T("대출도서관부호") , nIndex , sData );

				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
			}

			if(sData.Compare(iter->first) != 0)
			{
				pGrid->SelectGetNext();
				continue;
			}

			pPrintDM->InsertRow(-1);
			
			sData.Format( _T("%d") , nRowNum+1 );
			ids = pPrintDM->SetCellData( _T("번호") , sData , nRowNum);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );			
			
			for( INT j = 0 ; j < nCopyCnt ; j++ )
			{
				ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
				if( ids < 0 ) 
				{
					CString sEMsg;
					sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
					AfxMessageBox( sEMsg );
				}
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
				
				ids = pPrintDM->SetCellData( sField[j][1] , sData , nRowNum );
				if( ids < 0 ) 
				{
					CString sEMsg;
					sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nRowNum , sField[j][0] , sData );
					AfxMessageBox( sEMsg );
				}
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );
			}

			pGrid->SelectGetNext();
			nRowNum ++;
		}

		pPrintDM->SORT(_T("이용자번호"));
		pPrintDM->SORT(_T("이용자명"));

		for(i=0; i<pPrintDM->GetRowCount();i++)
		{
			sData.Format( _T("%d") , i+1 );
			ids = pPrintDM->SetCellData( _T("번호") , sData , i);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("PrintWaybill") );	
		}

		CString strAcqYear, strGroupInfo;
		CBasicPrintMgr PrintMgr(this);
		PrintMgr.InitPrintData();

		PrintMgr.AddPrintData(_T("HOLD_LIB"), iter->second);
		PrintMgr.AddPrintData(_T("LOAN_LIB"), m_pInfo->LIB_NAME);
		PrintMgr.AddPrintData(_T("SEND_DATE"), strDate);
		PrintMgr.SetPrintDM(pPrintDM);
		PrintMgr.SetPrintType(0);
		PrintMgr.SetSIReportFilename(_T("반납운송장"));
		PrintMgr.SetPrivacyPrintLogParm( TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, _T("타관반납관리 반납운송장"), m_pInfo->MANAGE_CODE, _T("타관반난관리 반납운송장"),	0 );
		PrintMgr.Print();
	}	

	mapLib.clear();
	
	return TRUE;
EFS_END
		
	return FALSE;
}

void CKL_OTHER_RETURN_REQUEST::ModifyMemo()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return;
	}

	CKL_OTHER_RETURN_EDIT_MEMO dlgEditMemo(m_pParent);
	dlgEditMemo.SetGrid(m_pGrid);
	dlgEditMemo.SetDM(m_pDM);

	dlgEditMemo.DoModal();

	if(dlgEditMemo.m_BModify)
		Refresh();
}

void CKL_OTHER_RETURN_REQUEST::ViewLoanUser()
{
	m_pGrid->SelectMakeList();
	if(m_pGrid->SelectGetCount() <= 0) 
	{
		AfxMessageBox(_T("먼저 선정을 하여 주십시요."));
		return;
	}

	CKL_OTHER_RETURN_DETAILVIEW dlgViewLoanUser(m_pParent);
	dlgViewLoanUser.SetGrid(m_pGrid);
	dlgViewLoanUser.SetDM(m_pDM);

	dlgViewLoanUser.DoModal();	
}
