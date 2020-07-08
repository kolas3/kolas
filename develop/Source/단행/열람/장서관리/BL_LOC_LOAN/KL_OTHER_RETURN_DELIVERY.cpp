// KL_OTHER_RETURN_DELIVERY.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_RETURN_DELIVERY.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_DELIVERY dialog


CKL_OTHER_RETURN_DELIVERY::CKL_OTHER_RETURN_DELIVERY(CESL_Mgr* pParent /*=NULL*/, INT nSwitch)
	: CESL_Mgr(CKL_OTHER_RETURN_DELIVERY::IDD, pParent)
{
	m_nSwitch = nSwitch;

	m_nIndex = 0;
	m_nFirstGridIdx = 0;
	m_nLastGridIdx = 0; 
}


void CKL_OTHER_RETURN_DELIVERY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_RETURN_DELIVERY)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_RETURN_DELIVERY, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_RETURN_DELIVERY)
	ON_BN_CLICKED(IDC_btn_Prev, OnbtnPrev)
	ON_BN_CLICKED(IDC_btn_Next, OnbtnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_DELIVERY message handlers

BEGIN_EVENTSINK_MAP(CKL_OTHER_RETURN_DELIVERY, CDialog)
    //{{AFX_EVENTSINK_MAP(CKL_OTHER_RETURN_DELIVERY)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CKL_OTHER_RETURN_DELIVERY::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	if(InitESL_Mgr(_T("SM_KL_OTHER_RETURN_DELIVERYINFO")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
	}

	if(0 != m_nSwitch)
	{
		if(0 > DisplayControl(m_nSwitch))
		{
			AfxMessageBox(_T("조회가 실패하였습니다.")); 
		}
	}
	else
	{
		INT nIndex = SelectGetHeadPosition(_T("CM_KL_OTHER_RETURN_MNG_REQUEST"), _T("GRID"));
		if(0 > nIndex)
		{
			AfxMessageBox(_T("조회가 실패하였습니다.")); 
		}
		DisplayControl(nIndex);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CKL_OTHER_RETURN_DELIVERY::SearchPostInfo(CString strPaymentKey) 
{
	if(_T("") != strPaymentKey)
	{
		CFrameWriter Writer(m_pInfo);

		Writer.setCommand(_T("IL_K37_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("결제처리KEY"), strPaymentKey);
		Writer.addElement(_T("모드"), _T("Result"));

		CFrameReader Reader;
		CFrameSender Sender(m_pInfo);
		INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
		if(0 > ids)
		{
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
   
		CString strTmpData = TmpDM.GetCellData(_T("신청일자"), 0);
		if(_T("") != strTmpData)
		{
			CString strRequestDate = strTmpData.Mid(0, 4) + _T("/") + strTmpData.Mid(4, 2) + _T("/") + strTmpData.Mid(6, 2);
			((CEdit*)GetDlgItem(IDC_edt_RequestDate))->SetWindowText(strRequestDate);
		}
	

		strTmpData = TmpDM.GetCellData(_T("등기번호"), 0);

		if(_T("") != strTmpData)
		{
			((CEdit*)GetDlgItem(IDC_edt_PostNo))->SetWindowText(strTmpData);
		}

		CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_RETURN_DELIVERYINFO"));
		if(NULL == pDM) 
		{ 
			AfxMessageBox(_T("FindDM() Error!!")); 
			return -300; 
		}

		CString strDate = _T("");
		CString strTime = _T("");
		for(INT i=0; i<TmpDM.GetRowCount(); i++)
		{
			pDM->InsertRow(-1);
			strTmpData = TmpDM.GetCellData(_T("처리일자"), i);
			if(_T("") != strTmpData)
			{
				strDate = strTmpData.Mid(0, 4) + _T("/") + strTmpData.Mid(4, 2) + _T("/") + strTmpData.Mid(6, 2);
				pDM->SetCellData(_T("처리일자"), strDate, i);
			}

			strTmpData = TmpDM.GetCellData(_T("처리시간"), i);
			if(_T("") != strTmpData)
			{
				strTime = strTmpData.Mid(0, 2) + _T(":") + strTmpData.Mid(2, 2) + _T(":") + strTmpData.Mid(4, 2);
				pDM->SetCellData(_T("처리시간"), strTime, i);
			}
		
			strTmpData = TmpDM.GetCellData(_T("처리우체국명"), i);
			pDM->SetCellData(_T("처리우체국명"), strTmpData, i);	

			strTmpData = TmpDM.GetCellData(_T("처리코드명"), i);
			pDM->SetCellData(_T("처리코드명"), strTmpData, i);

			strTmpData = TmpDM.GetCellData(_T("처리상세코드명"), i);
			pDM->SetCellData(_T("처리상세코드명"), strTmpData, i);
		}	
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_KL_OTHER_RETURN_DELIVERYINFO"));
	INT ids = pCM->ControlDisplay(_T("MAINGRID"));
    if(0 > ids)
	{
		AfxMessageBox(_T("ControlDisplay() Error!!")); 
		return ids; 		
	}

	return 0;
}

void CKL_OTHER_RETURN_DELIVERY::OnbtnPrev() 
{
	INT row = SelectGetPrev(_T("CM_KL_OTHER_RETURN_MNG_REQUEST"), _T("GRID"));
	if ( row < 0 ) 
	{
        AfxMessageBox(_T("제일 처음 데이터 입니다."), MB_ICONEXCLAMATION);
		return;
    }

	INT ids = DisplayControl(row);
	if(0 > ids)
	{
        AfxMessageBox(_T("DisplayControl Error!"));
	}
}

void CKL_OTHER_RETURN_DELIVERY::OnbtnNext() 
{
	INT row = SelectGetNext(_T("CM_KL_OTHER_RETURN_MNG_REQUEST"), _T("GRID"));
	if ( row < 0 ) 
	{
         AfxMessageBox(_T("제일 마지막 데이터 입니다."), MB_ICONEXCLAMATION);
		return;
    }

	INT ids = DisplayControl(row);
	if(0 > ids)
	{
        AfxMessageBox(_T("DisplayControl Error!"));
	}
}

INT CKL_OTHER_RETURN_DELIVERY::DisplayControl(INT nIndex) 
{
	
	CESL_DataMgr *pDM = FindDM(_T("DM_KL_OTHER_RETURN_MNG_REQUEST"));
	if(NULL == pDM) 
	{ 
		AfxMessageBox(_T("FindDM() Error!!")); 
		return -100;
	}

	CString sGetData;
	CString tmpData;
	INT ids;

	CString strPaymentKey;

	ids = pDM->GetCellData(_T("결제처리KEY"), nIndex, strPaymentKey);
	if(0 > ids&& (ids != -4012)) 
	{
		return ids;
	}

	ids = pDM->GetCellData(_T("반납도서관"), nIndex, sGetData);
	if(0 > ids&& (ids != -4012)) 
	{
		return ids;
	}
	SetDlgItemText(IDC_edt_SendLibname,sGetData);
	
	ids = pDM->GetCellData(_T("대출도서관"), nIndex, sGetData);
	if(0 > ids&& (ids != -4012)) 
	{
		return ids;
	}
	SetDlgItemText(IDC_edt_ReceiveLibname,sGetData);
	
	ids = SearchPostInfo(strPaymentKey);
	if(0 > ids)
	{
		return ids;
	}

	pDM->FreeData();
	return 0;
}
