// MNG_AGREEMENT_STAT_01.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_AGREEMENT_STAT_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_AGREEMENT_STAT_01 dialog

#define STR_TAB_1 _T("개인정보 수집/이용")
#define STR_TAB_2 _T("책이음서비스")

#define STR_TAB_1_ALIAS _T("자관회원약관")
#define STR_TAB_2_ALIAS _T("책이음회원약관")



CMNG_AGREEMENT_STAT_01::CMNG_AGREEMENT_STAT_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CMNG_AGREEMENT_STAT_01::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_AGREEMENT_STAT_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;	
}


void CMNG_AGREEMENT_STAT_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_AGREEMENT_STAT_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_AGREEMENT_STAT_01, CDialog)
	//{{AFX_MSG_MAP(CMNG_AGREEMENT_STAT_01)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_AGREEMENT, OnSelchangeTabAgreement)
	ON_EN_MAXTEXT(IDC_EDIT_AGREE, OnMaxtextEditAgree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_AGREEMENT_STAT_01 message handlers

BOOL CMNG_AGREEMENT_STAT_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (InitESL_Mgr(_T("SM_MNG_AGREEMENT")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!!")); 
	}

	m_pCM = FindCM(_T("CM_MNG_AGREEMENT"));
	m_pDM = FindDM(_T("DM_MNG_AGREEMENT"));

	CTabCtrl *pTabCtlr = (CTabCtrl*)GetDlgItem(IDC_TAB_AGREEMENT);
	pTabCtlr->InsertItem(0,STR_TAB_1_ALIAS);
	pTabCtlr->InsertItem(1,STR_TAB_2_ALIAS);

	pTabCtlr->SetCurSel(0) ;
	RefreshDM(STR_TAB_1);
	Display(0);

	((CEdit*)GetDlgItem(IDC_EDIT_AGREE))->SetLimitText(64000);


	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMNG_AGREEMENT_STAT_01::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

void CMNG_AGREEMENT_STAT_01::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CTabCtrl *pTabCtlr = (CTabCtrl*)GetDlgItem(IDC_TAB_AGREEMENT);
    if (pTabCtlr->GetSafeHwnd() == NULL) return;
    pTabCtlr->MoveWindow (5,5, cx-10, cy-60);

	CEdit* pEditCtlr = (CEdit*)GetDlgItem(IDC_EDIT_AGREE);
	if (pEditCtlr->GetSafeHwnd() == NULL) return;
    pEditCtlr->MoveWindow (10,110, cx-20, cy-170);
	
	// TODO: Add your message handler code here	
}

void CMNG_AGREEMENT_STAT_01::OnModify()
{
	//int inputMode = m_pDM->GetRowCount() ? 0 : 1;
	int inputMode = 1;

	CTabCtrl *pTabCtlr = (CTabCtrl*)GetDlgItem(IDC_TAB_AGREEMENT);
	
	if(pTabCtlr)
	{
		const int nCnt = 4;
		CString strArrAlias[nCnt] = {
			_T("TERMS_TITLE"), _T("IMPL_DATE"), _T("TERMS_MEMO"), _T("ESSENTIAL")
		};
		CString strArrDatas[nCnt];

		int nType = pTabCtlr->GetCurSel();

		if(nType ==0 )
		{
			strArrDatas[0] = STR_TAB_1;			
		}
		else if(nType == 1)
		{
			strArrDatas[0] = STR_TAB_2;
		}

		CString strDate; 
		CTime t = m_pDM->GetDBTime();
		strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		
		strArrDatas[1] = strDate;
		
		
				
		((CEdit*)GetDlgItem(IDC_EDIT_AGREE))->GetWindowText(strArrDatas[ 2 ] );

		if(((CButton*)GetDlgItem(IDC_CHECK_ESSENTIAL))->GetCheck())
		{
			strArrDatas[ 3 ] = _T("Y");
		}
		else
		{
			strArrDatas[ 3 ] = _T("N");
		}

		//m_pCM->GetControlMgrData( _T("TERMS_MEMO"),  strArrDatas[ 2 ] );
		//m_pCM->GetControlMgrData( _T("ESSENTIAL"),  strArrDatas[ 3 ] );

		m_pDM->StartFrame();
		m_pDM->InitDBFieldData();		

		for (int i=0; i<nCnt; i++)
		{
			if( i != 2)
			{
				m_pDM->AddDBFieldData( strArrAlias[ i ], _T("STRING"), strArrDatas[ i ], true );
				m_pDM->SetCellData( strArrAlias[i] , strArrDatas[i] , 0 );			
			}
		}

		int nCount = strArrDatas[2].GetLength();
		CString strCount; 
		strCount.Format(_T("%d"), (nCount+1)*2);

		m_pDM->AddDBFieldData( _T("TERMS_LENGTH"), _T("NUMERIC"), strCount, true );				

		CString strRecKey = _T("");
		if(inputMode)
		{			
			m_pDM->MakeRecKey(strRecKey);
			m_pDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), strRecKey );
			m_pDM->MakeInsertFrame( _T("MN_AGREEMENT_TBL") );
		}
		else
		{
			m_pDM->MakeUpdateFrame( _T("MN_AGREEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_pDM->GetCellData( _T("REC_KEY"), 0 ) );
		}

		m_pDM->SendFrame();
		m_pDM->EndFrame();
		
		BYTE* pBuffer = new BYTE[(nCount+1)*2];

		memset(pBuffer, 0x00, (nCount+1)*2);

		memcpy(pBuffer, strArrDatas[2].GetBuffer(0), (nCount+1)*2);
				
		m_pDM->LOBUpdate( _T("MN_AGREEMENT_TBL"),  _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("TERMS_MEMO"), pBuffer, (nCount+1)*2);

		if(pBuffer)
			delete [] pBuffer;

		if( inputMode )
			AfxMessageBox( _T("저장되었습니다.") );
		else
			AfxMessageBox( _T("수정되었습니다.") );			
	}
}

void CMNG_AGREEMENT_STAT_01::Display(int nSelCur)
{	
	CString strData = _T("");
	if(m_pDM)
	{
		strData = m_pDM->GetCellData(_T("ESSENTIAL"), 0);

		if(strData.Compare(_T("Y")) == 0)
		{
			((CButton*)GetDlgItem(IDC_CHECK_ESSENTIAL))->SetCheck(TRUE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK_ESSENTIAL))->SetCheck(FALSE);
		}

		strData = m_pDM->GetCellData(_T("REC_KEY"), 0);
	
		CString strLength = m_pDM->GetCellData(_T("TERMS_LENGTH"), 0);

		INT nLength = _ttoi(strLength);

		if(nLength > 0)
		{
			BYTE* pBuffer = new BYTE[nLength ];
			memset(pBuffer, 0x00, nLength);


			INT ids = m_pDM->LOBGet(_T("MN_AGREEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strData, _T("TERMS_MEMO"), pBuffer, &nLength);
			
			((CEdit*)GetDlgItem(IDC_EDIT_AGREE))->SetWindowText((LPCTSTR)pBuffer);

			delete [] pBuffer;
		}
		else
		{
			((CEdit*)GetDlgItem(IDC_EDIT_AGREE))->SetWindowText(_T(""));
		}

		//((CEdit*)GetDlgItem(IDC_EDIT_AGREE))->SetWindowText(strData);
	}
}

int CMNG_AGREEMENT_STAT_01::RefreshDM(CString strTermType)
{
	if(!m_pDM)
		return -1;

	CString strWhere = _T(""), strQuery = _T("");

	//strWhere.Format(_T("REC_KEY = (SELECT MAX(REC_KEY) KEEP(DENSE_RANK FIRST ORDER BY IMPL_DATE DESC) FROM MN_AGREEMENT_TBL WHERE TERMS_TITLE = '%s') "), strTermType);

	//INT ids = m_pDM->RefreshDataManager(strWhere);

	/*TO_CHAR(TERMS_MEMO) AS TERMS_MEMO,*/
	strQuery.Format(_T("SELECT TERMS_TITLE, AMEND_DATE, IMPL_DATE, ESSENTIAL, FIRST_WORK, LAST_WORK, REC_KEY, TERMS_LENGTH FROM MN_AGREEMENT_TBL WHERE REC_KEY = (SELECT MAX(REC_KEY) KEEP(DENSE_RANK FIRST ORDER BY IMPL_DATE DESC) FROM MN_AGREEMENT_TBL WHERE TERMS_TITLE = '%s') "), strTermType);

	INT ids = m_pDM->RestructDataManager(strQuery);

	if(ids < 0) return -1;

	return 0;
}

void CMNG_AGREEMENT_STAT_01::OnSelchangeTabAgreement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	CTabCtrl *pTabCtlr = (CTabCtrl*)GetDlgItem(IDC_TAB_AGREEMENT);

	if(pTabCtlr)
	{
		int nType = pTabCtlr->GetCurSel();

		if(nType ==0 )
		{
			RefreshDM(STR_TAB_1);
		}
		else if(nType == 1)
		{
			RefreshDM(STR_TAB_2);
		}		

		Display(nType);
	}
	
	*pResult = 0;
}

BOOL CMNG_AGREEMENT_STAT_01::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {      
        if (GetDlgItem(IDC_EDIT_AGREE) == GetFocus())
        {
			CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_AGREE);
            int nLen = edit->GetWindowTextLength();
            edit->SetSel( nLen, nLen );
            edit->ReplaceSel( _T("\r\n") );            
        }
    }

	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMNG_AGREEMENT_STAT_01::OnMaxtextEditAgree() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox( _T("문구 한계를 초과 하였습니다. \n64000자 이하로 작성바랍니다.\n") );
}
