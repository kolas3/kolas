


#include "stdafx.h"
#include "resource.h"
#include "ILL_MODIFY.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





CILL_MODIFY::CILL_MODIFY(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	
	
	mCtrl_pedtAplHour = NULL;
	mCtrl_pedtAplReqHour = NULL;
	mCtrl_pedtGiveHour = NULL;
	mCtrl_pedtTakeHour = NULL;
	mCtrl_pedtReturnHour = NULL;
	mCtrl_pedtReturnPlanHour = NULL;

	m_nClickBtnIdx = -1;

	m_pParentDM = NULL;
	m_pParentGrid = NULL;

	mCtrl_pGrid = NULL;

	m_pLoanShareManager = NULL;
}


void CILL_MODIFY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_btnILL_MODIFY_TAKE_DATE, mCtrl_btnTakeDate);
	DDX_Control(pDX, IDC_btnILL_MODIFY_RETURN_PLAN_DATE, mCtrl_btnReturnPlanDate);
	DDX_Control(pDX, IDC_btnILL_MODIFY_RETURN_DATE, mCtrl_btnReturnDate);
	DDX_Control(pDX, IDC_btnILL_MODIFY_GIVE_DATE, mCtrl_btnGiveDate);
	DDX_Control(pDX, IDC_btnILL_MODIFY_APLREQ_DATE, mCtrl_btnAplReqDate);
	DDX_Control(pDX, IDC_btnILL_MODIFY_APL_DATE, mCtrl_btnAplDate);
	
}


BEGIN_MESSAGE_MAP(CILL_MODIFY, CDialog)
	//{{AFX_MSG_MAP(CILL_MODIFY)
	ON_COMMAND_RANGE(IDM_TODAY, IDM_RESET, OnClickMenu)		
	ON_MESSAGE(WM_CLICK_DATE_BTN, OnClickDateBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CILL_MODIFY::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitESL_Mgr(_T("ILL_MODIFY"));
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	Init_HourControl();

	mCtrl_pGrid = new CESL_Grid(NULL);
	mCtrl_pGrid->SubclassDlgItem(IDC_gridILL_MOIDFY, this);

	
	
	mCtrl_btnAplDate.SetMenu(IDR_MENU_ILL_MODIFY, m_hWnd);
	mCtrl_btnAplDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 0);

	mCtrl_btnAplReqDate.SetMenu(IDR_MENU_ILL_MODIFY, m_hWnd);
	mCtrl_btnAplReqDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 1);

	mCtrl_btnGiveDate.SetMenu(IDR_MENU_ILL_MODIFY, m_hWnd);
	mCtrl_btnGiveDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 2);

	mCtrl_btnTakeDate.SetMenu(IDR_MENU_ILL_MODIFY, m_hWnd);
	mCtrl_btnTakeDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 3);

	mCtrl_btnReturnPlanDate.SetMenu(IDR_MENU_ILL_MODIFY, m_hWnd);
	mCtrl_btnReturnPlanDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 4);

	mCtrl_btnReturnDate.SetMenu(IDR_MENU_ILL_MODIFY, m_hWnd);
	mCtrl_btnReturnDate.SetMenuCallback(m_hWnd, WM_CLICK_DATE_BTN, 5);

	
	
	const int DAY_COUNT = 6;
	int nUpdateCnt[DAY_COUNT];
	CString strAlias[DAY_COUNT] = 
	{ 
		_T("신청일"), _T("요청일"), _T("발송일"), _T("입수일"), _T("복귀예정일"),_T("복귀일")
	};
	CString strTransNoList[DAY_COUNT];
	
	for(int nCnt = 0; nCnt < DAY_COUNT; nCnt++)
	{
		Get_Update_TransNoList(strAlias[nCnt], strTransNoList[nCnt], nUpdateCnt[nCnt]);
		if(0 >= nUpdateCnt[nCnt]) EnableControl(strAlias[nCnt], FALSE);
		
		mCtrl_pGrid->SetTextMatrix(0, nCnt, strAlias[nCnt]);	
		CString strCnt;
		strCnt.Format(_T("%d"), nUpdateCnt[nCnt]);
		mCtrl_pGrid->SetTextMatrix(1, nCnt, strCnt);

		mCtrl_pGrid->SetCol(nCnt);

		mCtrl_pGrid->SetRow(0);		
		mCtrl_pGrid->SetCellAlignment(4);
		mCtrl_pGrid->SetRow(1);
		mCtrl_pGrid->SetCellAlignment(4);		
		mCtrl_pGrid->SetColWidth(nCnt, 0, 66*15);
	}

	return TRUE;  
	              
}

BOOL CILL_MODIFY::PreTranslateMessage(MSG* pMsg) 
{
	
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

int CILL_MODIFY::Init_HourControl()
{
	const int HOUR_CONTROL_COUNT = 6;
	CHourEditControl **pEditHour[HOUR_CONTROL_COUNT] = 
	{
		&mCtrl_pedtAplHour,
		&mCtrl_pedtAplReqHour,
		&mCtrl_pedtGiveHour,
		&mCtrl_pedtTakeHour,
		&mCtrl_pedtReturnPlanHour,
		&mCtrl_pedtReturnHour
	};
	UINT uID[HOUR_CONTROL_COUNT] = 
	{
		IDC_edtILL_MODIFY_APL_HOUR,
		IDC_edtILL_MODIFY_APLREQ_HOUR,
		IDC_edtILL_MODIFY_GIVE_HOUR,
		IDC_edtILL_MODIFY_TAKE_HOUR,
		IDC_edtILL_MODIFY_RETURN_PLAN_HOUR,
		IDC_edtILL_MODIFY_RETURN_HOUR
	};

 	CWnd* pWnd = NULL;
 	for(int nCnt = 0; nCnt < HOUR_CONTROL_COUNT; nCnt++)
 	{
 		pWnd = GetDlgItem(uID[nCnt]);
 		if(NULL == pWnd || NULL == pWnd->GetSafeHwnd()) continue;
 
 		(*pEditHour[nCnt]) = new CHourEditControl;		
 		(*pEditHour[nCnt])->SubclassWindow(pWnd->GetSafeHwnd());
 		(*pEditHour[nCnt])->InitEdit();
 	}

	return 0;
}

void CILL_MODIFY::OnClickMenu(WPARAM wParam, LPARAM lParam)
{
	
	SYSTEMTIME SYSTIME;

	GetLocalTime(&SYSTIME);

	CString strDate;
	strDate.Format(_T("%02d:%02d:%02d"), SYSTIME.wHour, SYSTIME.wMinute, SYSTIME.wSecond);

	CString strDay;
	strDay.Format(_T("%04d/%02d/%02d"), SYSTIME.wYear, SYSTIME.wMonth, SYSTIME.wDay);

	const int HOUR_CONTROL_COUNT = 6;
	CString strAlias[HOUR_CONTROL_COUNT] = 
	{
		_T("신청일"), _T("요청일"), _T("발송일"), _T("입수일"), _T("복귀예정일"), _T("복귀일")
	};
	
	CHourEditControl **pEditHour[HOUR_CONTROL_COUNT] = 
	{
		&mCtrl_pedtAplHour,
		&mCtrl_pedtAplReqHour,
		&mCtrl_pedtGiveHour,
		&mCtrl_pedtTakeHour,
		&mCtrl_pedtReturnPlanHour,
		&mCtrl_pedtReturnHour
	};
	
	CECO_ControlDateEdit* pDate = (CECO_ControlDateEdit*)
		FindControl(_T("CM_ILL_MODIFY"), strAlias[m_nClickBtnIdx]);
	if(NULL == pDate) return;

	CHourEditControl* pHourEdit = (*pEditHour[m_nClickBtnIdx]);
	if(NULL == pHourEdit) return;

	if(IDM_RESET == wParam)
	{
		pDate->Clear();
		pHourEdit->Clear();
	}
	else if(IDM_TODAY == wParam)
	{
		pDate->SetData(strDay);
		pHourEdit->SetData(strDate);
	}

}

int CILL_MODIFY::OnClickDateBtn(WPARAM wParam, LPARAM lParam)
{
	m_nClickBtnIdx = (int)lParam;

	return 0;
}

void CILL_MODIFY::OnOK() 
{
	
	const int DAY_COUNT = 6;
	int nUpdateCnt[DAY_COUNT];
	CString strAlias[DAY_COUNT] = 
	{ 
		_T("신청일"), _T("요청일"), _T("발송일"), _T("입수일"), _T("복귀예정일"), _T("복귀일") 
	};
	CString strDBAlias[DAY_COUNT] = 
	{
		_T("APL_DATE"), _T("APLREQ_DATE"), _T("GIVE_DATE"), _T("TAKE_DATE"), _T("RETURN_PLAN_DATE"), _T("RETURN_DATE")
	};
	CString strTransNoList[DAY_COUNT];
	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.StartFrame();

	BOOL bIsUpdate = FALSE;
	BOOL bEqual = FALSE;
	INT i = 0, k = 0;
	CString strDate, strTransNo;
	CString strQuery;
	CStringArray astrTransNo;
	for(int nCnt = 0; nCnt < DAY_COUNT; nCnt++)
	{		
		Get_Update_TransNoList(strAlias[nCnt], strTransNoList[nCnt], nUpdateCnt[nCnt]);		

		if(0 < nUpdateCnt)
		{
			int ids = Get_Date(strAlias[nCnt], strDate);
			if(0 > ids)
			{
				CString strMsg;
				strMsg.Format(_T("[%s]이 올바르지 않습니다."), strAlias[nCnt]);

				AfxMessageBox(strMsg);				
				break;
			}
			else if(0 < ids) continue;
			
			strQuery.Format(
				_T("UPDATE ILL_TRANS_TBL SET %s = TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS'), LAST_WORK = '%s' WHERE TRANS_NO IN (%s);"),
				strDBAlias[nCnt],
				strDate,
				GetWorkLogMsg(_T("일괄변경"), __WFILE__, __LINE__),
				strTransNoList[nCnt]);	
			
			TmpDM.AddFrame(strQuery);
			bIsUpdate = TRUE;

			for(i = 0; ; i++)
			{
				if(FALSE == AfxExtractSubString(strTransNo, strTransNoList[nCnt], i, ','))
				{
					break;
				}

				bEqual = FALSE;
				for(k = 0 ; k < astrTransNo.GetSize() ; k++ )
				{
					if( strTransNo.Compare(astrTransNo.GetAt(k)) == 0 ) 
						bEqual = TRUE;
				}
				if(FALSE == bEqual)
				{
					astrTransNo.Add(strTransNo);
				}				
			}			
		}
	}

	int ids = -1;
	if(FALSE == bIsUpdate)
	{
		TmpDM.EndFrame();
		return;
	}
	else 
	{
		ids = TmpDM.SendFrame();
		TmpDM.EndFrame();
	}

	if(0 > ids) return;

	if(_T("D") == m_sBookCooperativeMode)
	{
 		UPLOAD_SHARE_INFO UploadShareInfo;	
 		UploadShareInfo.nType = 0;
 
 		UploadShareInfo.UploadShareLoanInfo.sCommand = _T("71");
 		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");	
 		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("ILL_TRANS_TBL");
 		UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pInfo->LIB_CODE; 		
 		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = _T("APL_DATE,APLREQ_DATE,GIVE_DATE,TAKE_DATE,RETURN_PLAN_DATE,RETURN_DATE");

		for(k = 0 ; k < astrTransNo.GetSize() ; k++ )
		{
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = astrTransNo.GetAt(k);
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey.TrimLeft(_T("'"));
			UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey.TrimRight(_T("'"));
 			m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);
		}
	}

	else 
	{
		AfxMessageBox(_T("변경되었습니다."));
	}
	CDialog::OnOK();
}

int CILL_MODIFY::Get_Date(CString strAlias, CString &strDate)
{	
	
	CString strDay;
	CECO_ControlDateEdit* pDay = (CECO_ControlDateEdit*)FindControl(_T("CM_ILL_MODIFY"), strAlias);
	if(NULL == pDay) return -1;
	pDay->GetData(strDay);

	
	CString strTime;
	CHourEditControl* pedtHour = NULL;
	if(_T("신청일") == strAlias)
	{
		pedtHour = mCtrl_pedtAplHour;
	}
	else if(_T("요청일") == strAlias)
	{
		pedtHour = mCtrl_pedtAplReqHour;
	}
	else if(_T("발송일") == strAlias)
	{
		pedtHour = mCtrl_pedtGiveHour;
	}
	else if(_T("입수일") == strAlias)
	{
		pedtHour = mCtrl_pedtTakeHour;
	}
	else if(_T("복귀예정일") == strAlias)
	{
		pedtHour = mCtrl_pedtReturnPlanHour;
	}
	else if(_T("복귀일") == strAlias)
	{
		pedtHour = mCtrl_pedtReturnHour;
	}
	else return -2;
	pedtHour->GetData(strTime);
	
	
	if(_T("") == strDay &&_T("") == strTime) { return 1; }
	else if(_T("") == strDay &&_T("") != strTime) { return -4; }
	else if(_T("") != strDay &&_T("") == strTime) { strTime = _T("00:00:00"); }	

	strDate.Format(_T("%s %s"), strDay, strTime);

	return 0;
}

int CILL_MODIFY::Get_Update_TransNoList(CString strAlias, CString &strTransNoList, int &nUpdateCnt)
{
	if(NULL == m_pParentGrid) return -1;

	m_pParentGrid->SelectMakeList();
	if(0 > m_pParentGrid->SelectGetCount()) return -2;
		
	CString strDayAlias = strAlias;
	CString strLibAlias;
	if(_T("신청일") == strAlias)
	{
		strLibAlias = _T("요청도서관_코드");
	}
	else if(_T("요청일") == strAlias)
	{
		strLibAlias = _T("요청도서관_코드");
	}
	else if(_T("발송일") == strAlias)
	{
		strLibAlias = _T("제공도서관_코드");
	}
	else if(_T("입수일") == strAlias)
	{
		strLibAlias = _T("요청도서관_코드");
	}
	else if(_T("복귀예정일") == strAlias)
	{
		strLibAlias = _T("요청도서관_코드|반납도서관_코드");
	}
	else if(_T("복귀일") == strAlias)
	{
		strLibAlias = _T("반납도서관_코드");
	}
	else return -2;
	
	nUpdateCnt = 0;
	CString strNoList;
	int nIdx = m_pParentGrid->SelectGetHeadPosition();	
	do
	{	
		CString strGet;
		BOOL bIsUpdate = FALSE;

		m_pParentDM->GetCellData(strDayAlias, nIdx, strGet);
		if(_T("") == strGet) bIsUpdate = FALSE;
		else
		{
			CString strInCheckValue;
			for(int nCnt = 0; ; nCnt++)
			{
				if(FALSE == AfxExtractSubString(strInCheckValue, strLibAlias, nCnt, '|')) 
					break;

				m_pParentDM->GetCellData(strInCheckValue, nIdx, strGet);
				if(strGet == m_strLibCode) bIsUpdate = TRUE;
			}

			if(TRUE == bIsUpdate)
			{
				m_pParentDM->GetCellData(_T("트랜잭션번호"), nIdx, strGet);

				strNoList = strNoList + _T(",'") + strGet + _T("'");
				nUpdateCnt++;
			}
		}

		nIdx = m_pParentGrid->SelectGetNext();
	}while(0 <= nIdx);
	
	strNoList.TrimLeft(_T(","));
	
	strTransNoList = strNoList;

	return 0;
}







































































































































































































int CILL_MODIFY::EnableControl(CString strAlias, BOOL bEnable)
{
	
	CString strDay;
	CECO_ControlDateEdit* pDay = (CECO_ControlDateEdit*)FindControl(_T("CM_ILL_MODIFY"), strAlias);
	if(NULL == pDay) return -1;
	pDay->EnableWindow(bEnable);

	
	CString strTime;
	CHourEditControl* pedtHour = NULL;
	CWinXPButtonST* pBtn = NULL;
	if(_T("신청일") == strAlias)
	{
		pedtHour = mCtrl_pedtAplHour;
		pBtn = &mCtrl_btnAplDate;
	}
	else if(_T("요청일") == strAlias)
	{
		pedtHour = mCtrl_pedtAplReqHour;
		pBtn = &mCtrl_btnAplReqDate;
	}
	else if(_T("발송일") == strAlias)
	{
		pedtHour = mCtrl_pedtGiveHour;
		pBtn = &mCtrl_btnGiveDate;
	}
	else if(_T("입수일") == strAlias)
	{
		pedtHour = mCtrl_pedtTakeHour;
		pBtn = &mCtrl_btnTakeDate;
	}
	else if(_T("복귀예정일") == strAlias)
	{
		pedtHour = mCtrl_pedtReturnPlanHour;
		pBtn = &mCtrl_btnReturnPlanDate;
	}
	else if(_T("복귀일") == strAlias)
	{
		pedtHour = mCtrl_pedtReturnHour;
		pBtn = &mCtrl_btnReturnDate;
	}
	else return -2;
	pedtHour->EnableWindow(bEnable);
	pBtn->EnableWindow(bEnable);

	return 0;
}