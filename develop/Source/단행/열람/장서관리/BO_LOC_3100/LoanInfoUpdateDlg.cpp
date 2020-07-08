// LoanInfoUpdateDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "bo_loc_3100.h"
#include "LoanInfoUpdateDlg.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanInfoUpdateDlg dialog


CLoanInfoUpdateDlg::CLoanInfoUpdateDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CLoanInfoUpdateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoanInfoUpdateDlg)
		m_pParent = pParent;
	//}}AFX_DATA_INIT
}


void CLoanInfoUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanInfoUpdateDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoanInfoUpdateDlg, CDialog)
	//{{AFX_MSG_MAP(CLoanInfoUpdateDlg)
	ON_BN_CLICKED(IDC_btnNext, OnbtnNext)
	ON_BN_CLICKED(IDC_btnPrev, OnbtnPrev)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanInfoUpdateDlg message handlers

BOOL CLoanInfoUpdateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd());

	m_hLoan = new CECO_ControlDateEdit(NULL);
	m_hLoan->SubclassWindow( GetDlgItem(IDC_eLoanDate)->GetSafeHwnd() );
	m_hLoan->InitEdit();
	m_hLoan->ECO_SetReadOnly(FALSE);

	m_hLoan2 = new CECO_ControlHourEdit;
	m_hLoan2->SubclassWindow( GetDlgItem(IDC_eLoanDate2)->GetSafeHwnd() );
	m_hLoan2->InitEdit();

	m_hReturnPlandDate = new CECO_ControlDateEdit(NULL);
	m_hReturnPlandDate->SubclassWindow( GetDlgItem(IDC_eReturnPlanDate)->GetSafeHwnd() );
	m_hReturnPlandDate->InitEdit();
	m_hReturnPlandDate->ECO_SetReadOnly(FALSE);

	m_hReservationDate = new CECO_ControlDateEdit(NULL);
	m_hReservationDate->SubclassWindow( GetDlgItem(IDC_eReservationDate)->GetSafeHwnd() );
	m_hReservationDate->InitEdit();
	m_hReservationDate->ECO_SetReadOnly(FALSE);

	m_hReservationDate2 = new CECO_ControlHourEdit;
	m_hReservationDate2->SubclassWindow( GetDlgItem(IDC_eReservationDate2)->GetSafeHwnd() );
	m_hReservationDate2->InitEdit();

	m_hReservationExpireDate = new CECO_ControlDateEdit(NULL);
	m_hReservationExpireDate->SubclassWindow( GetDlgItem(IDC_eReservationExpireDate)->GetSafeHwnd() );
	m_hReservationExpireDate->InitEdit();
	m_hReservationExpireDate->ECO_SetReadOnly(FALSE);

	m_pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid") );
	if ( m_pGrid == NULL ) return FALSE;

	m_pGrid->SelectMakeList();

	m_nCurGridPos = 0;
	setCurrentIndexData( m_pGrid->SelectGetHeadPosition() );
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CLoanInfoUpdateDlg::setCurrentIndexData(INT idx)
{
	EFS_BEGIN
	if ( idx < 0 ) return;

	ControlButtonStatus(idx);

	m_hLoan->Clear();
	m_hLoan2->Clear();
	m_hReturnPlandDate->Clear();
	m_hReservationDate->Clear();
	m_hReservationDate2->Clear();
	m_hReservationExpireDate->Clear();

	CESL_DataMgr* pLoanInfoDM = m_pParent->FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );

	//대출정보 세팅
	GetDlgItem(IDC_REG_NO)->SetWindowText( pLoanInfoDM->GetCellData(_T("등록번호"), idx ) );
	GetDlgItem(IDC_BOOKNAME)->SetWindowText( pLoanInfoDM->GetCellData(_T("본표제"), idx ) );
	GetDlgItem(IDC_MANAGE_CODE)->SetWindowText( pLoanInfoDM->GetCellData(_T("관리구분"), idx ) );	
	GetDlgItem(IDC_LOAN_TYPE)->SetWindowText( pLoanInfoDM->GetCellData( _T("대출상태설명"), idx ) );

	//일자 정보 세팅
	CString sReturnPlanDate = pLoanInfoDM->GetCellData(_T("반납예정일"), idx );
	if ( !sReturnPlanDate.IsEmpty() )	m_hReturnPlandDate->SetData( sReturnPlanDate );
	
	CString sReservationDate =  pLoanInfoDM->GetCellData(_T("예약일"), idx );
	if ( !sReservationDate.IsEmpty() )
	{
		m_hReservationDate->SetData( sReservationDate );

		if ( sReservationDate.GetLength() > 10 )
		{
			sReservationDate = sReservationDate.Mid(11,sReservationDate.GetLength()); sReservationDate.TrimLeft();	sReservationDate.TrimRight();	
			m_hReservationDate2->SetData( sReservationDate );
		}
	}

	CString sReservationExpireDate = pLoanInfoDM->GetCellData(_T("예약만기일"), idx );
	if ( !sReservationExpireDate.IsEmpty() )	m_hReservationExpireDate->SetData( sReservationExpireDate );	

	CString sLoanDate = pLoanInfoDM->GetCellData(_T("대출일"), idx );
	if ( !sLoanDate.IsEmpty() )
	{	
		//sLoanDate = sLoanDate.Mid(0, 10); sLoanDate.TrimLeft();	sLoanDate.TrimRight();	
		m_hLoan->SetData( sLoanDate );

		//sLoanDate = pLoanInfoDM->GetCellData(_T("대출일"), idx );
		if ( sLoanDate.GetLength() > 10 )
		{
			sLoanDate = sLoanDate.Mid(11,sLoanDate.GetLength()); sLoanDate.TrimLeft();	sLoanDate.TrimRight();	
			m_hLoan2->SetData( sLoanDate );
		}
	}

	EFS_END
}

void CLoanInfoUpdateDlg::OnOK() 
{	
	EFS_BEGIN

	INT idx = m_pGrid->SelectGetIdx();
	if( idx < 0  ) {
		AfxMessageBox(_T("선택된 정보가 없습니다."));
		return;
	}
	CString strLoanStatus,sManageCode;
	CESL_DataMgr* pLoanInfoDM = m_pParent->FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
	
	//자관자료만 정보를 바꿀 수 있다.
	pLoanInfoDM->GetCellData( _T("관리구분"), idx, sManageCode );
	if ( sManageCode != m_pInfo->MANAGE_CODE )
	{
		AfxMessageBox(_T("자관자료만 변경 가능합니다."));
		return;
	}	
	//대출 혹은 예약일 때 만 변경 가능하다.
	pLoanInfoDM->GetCellData( _T("대출상태"), idx, strLoanStatus );
	if ( !( strLoanStatus == _T("0") || strLoanStatus == _T("2") || strLoanStatus == _T("L") 
		|| strLoanStatus == _T("3") || strLoanStatus == _T("4") || strLoanStatus == _T("O") ) 
		)
	{
		AfxMessageBox(_T("변경 불가능한 상태입니다."));
		return;
	}

	CString sRetutnPlan,sRservation,sRservation2,sReservationExp,sLoan,sLoan2,sRecKey;

	m_hReturnPlandDate->GetData(sRetutnPlan);
	m_hReservationDate->GetData(sRservation);
	m_hReservationDate2->GetData(sRservation2);
	m_hReservationExpireDate->GetData(sReservationExp);	
	m_hLoan->GetData(sLoan);
	m_hLoan2->GetData(sLoan2);
	if ( !sLoan2.IsEmpty() )	sLoan += _T(" ") + sLoan2;
	if ( !sRservation2.IsEmpty() )	sRservation += _T(" ") + sRservation2;

	pLoanInfoDM->GetCellData(_T("대출/반납정보KEY"), idx, sRecKey);

	INT ids = 0;
	pLoanInfoDM->StartFrame();
	
	CString sDateFormat;
	pLoanInfoDM->InitDBFieldData();

	if (!sLoan.IsEmpty())
	{
		sDateFormat = _T("YYYY/MM/DD");
		if (sLoan.GetLength() > 10)	sDateFormat += _T(" HH24:MI:SS");
		pLoanInfoDM->AddDBFieldData(_T("LOAN_DATE"),_T("NUMERIC"),_T("TO_DATE('") + sLoan  + _T("'") + _T(",'") + sDateFormat + _T("')"));			
	}
	else
	{
		pLoanInfoDM->AddDBFieldData(_T("LOAN_DATE"),_T("NUMERIC"),_T(""));			
	}
		
	if (!sRetutnPlan.IsEmpty())
	{
		pLoanInfoDM->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T("TO_DATE('") + sRetutnPlan + _T("','YYYY/MM/DD HH24:MI:SS')"));
	}
	else
	{
		pLoanInfoDM->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T(""));
	}
	
	if (!sRservation.IsEmpty())
	{
		sDateFormat = _T("YYYY/MM/DD");
		if (sRservation.GetLength() > 10)	sDateFormat += _T(" HH24:MI:SS");
		pLoanInfoDM->AddDBFieldData(_T("RESERVATION_DATE"),_T("NUMERIC"),_T("TO_DATE('") + sRservation + _T("'") + _T(",'") + sDateFormat + _T("')"));
	}
	else
	{
		pLoanInfoDM->AddDBFieldData(_T("RESERVATION_DATE"),_T("NUMERIC"),_T(""));
	}

	if (!sReservationExp.IsEmpty())
	{
		pLoanInfoDM->AddDBFieldData(_T("RESERVATION_EXPIRE_DATE"),_T("NUMERIC"),_T("TO_DATE('") + sReservationExp + _T("','YYYY/MM/DD HH24:MI:SS')"));
	}
	else
	{
		pLoanInfoDM->AddDBFieldData(_T("RESERVATION_EXPIRE_DATE"),_T("NUMERIC"),_T(""));
	}	
	
	pLoanInfoDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), pParentMgr->GetWorkLogMsg(_T("대출정보변경"), __WFILE__, __LINE__) );
	pLoanInfoDM->MakeUpdateFrame( _T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"), sRecKey);

	//책두레는 같이 변경해준다.
	if ( strLoanStatus == _T("L") )
	{
		pLoanInfoDM->InitDBFieldData();
		if (!sLoan.IsEmpty())
		{
			sDateFormat = _T("YYYY/MM/DD");
			if (sLoan.GetLength() > 10)	sDateFormat += _T(" HH24:MI:SS");
			
			ids = pLoanInfoDM->AddDBFieldData(_T("LOAN_DATE"),_T("NUMERIC"),_T("TO_DATE('") + sLoan  + _T("'") + _T(",'") + sDateFormat + _T("')"));			
		}
		if (!sRetutnPlan.IsEmpty())	pLoanInfoDM->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T("TO_DATE('") + sRetutnPlan + _T("','YYYY/MM/DD HH24:MI:SS')"));
		pLoanInfoDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), pParentMgr->GetWorkLogMsg(_T("대출정보변경"), __WFILE__, __LINE__) );
		pLoanInfoDM->MakeUpdateFrame( _T("ILL_TRANS_TBL"),_T("LOAN_KEY"),_T("NUMERIC"), sRecKey);
	}

	pLoanInfoDM->SendFrame();
	pLoanInfoDM->EndFrame();

	//예약은 책이음 동기화 필요없음
	if ( strLoanStatus == _T("3") || strLoanStatus == _T("4") || strLoanStatus == _T("O") )
	{
		AfxMessageBox(_T("수정되었습니다."));
		return;
	}

	CESL_DataMgr* pUserDM = pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	CString strLillUserYN = _T("N");
	if ( pUserDM == NULL )
	{
		pUserDM = pParentMgr->pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	}
	if ( pUserDM != NULL )
	{
		pUserDM->GetCellData(_T("통합회원여부"),0,strLillUserYN);
	}
	CString strGetData;
	if ( strLillUserYN == _T("Y") && m_pParent->m_pInfo->IsConnectUnityLoanService() )
	{
		CFrameWriter Writer(pParentMgr->m_pInfo);
		Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("로컬KEY"), sRecKey);
		Writer.addElement(_T("대출도서관"), pParentMgr->m_pInfo->LIB_CODE);
		pUserDM->GetCellData(_T("대출자번호"), 0, strGetData);
		Writer.addElement(_T("통합대출자번호"), strGetData);
		
		if(!sLoan.IsEmpty())
			Writer.addElement(_T("대출일"), sLoan );				
		else
			Writer.addElement(_T("대출일"), _T("") );				
		
		if(!sRetutnPlan.IsEmpty())
			Writer.addElement(_T("반납예정일"), sRetutnPlan + _T(" 23:59:59") );		
		else
			Writer.addElement(_T("반납예정일"), _T("") );		

		if (!sRservation.IsEmpty())
			Writer.addElement(_T("예약일"), sRservation );
		else
			Writer.addElement(_T("예약일"), _T("") );

		if (!sReservationExp.IsEmpty())		
			Writer.addElement(_T("예약만기일"), sReservationExp + _T(" 23:59:59") );		
		else
			Writer.addElement(_T("예약만기일"), _T("") );
		
		
		CFrameSender Sender(pParentMgr->m_pInfo);
		Sender.SendTempTbl(&Writer, pParentMgr->GetWorkLogMsg(_T("대출반납_대출정보_수정"), __WFILE__, __LINE__ ));
	}
	
	UpdateParentGrid();

	AfxMessageBox(_T("수정되었습니다."));
	EFS_END
}

VOID CLoanInfoUpdateDlg::UpdateParentGrid()
{
	EFS_BEGIN

	EFS_END	
}

VOID CLoanInfoUpdateDlg::ControlButtonStatus(INT idx)
{
	EFS_BEGIN
	GetDlgItem(IDC_btnNext)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnPrev)->EnableWindow(TRUE);

	if ( GridHasNext() == FALSE )
	{
		GetDlgItem(IDC_btnNext)->EnableWindow(FALSE);
	}

	if ( GridHasPrev() == FALSE )
	{
		GetDlgItem(IDC_btnPrev)->EnableWindow(FALSE);
	}

	m_hLoan->ECO_EnableWindow(FALSE);
	m_hLoan->EnableWindow(FALSE);
	
	m_hLoan2->Control_EnableWindow(FALSE);
	m_hLoan2->EnableWindow(FALSE);
	
	m_hReturnPlandDate->ECO_EnableWindow(FALSE);
	m_hReturnPlandDate->EnableWindow(FALSE);
	
	m_hReservationDate->ECO_EnableWindow(FALSE);
	m_hReservationDate->EnableWindow(FALSE);
	
	m_hReservationDate2->Control_EnableWindow(FALSE);
	m_hReservationDate2->EnableWindow(FALSE);
	
	m_hReservationExpireDate->ECO_EnableWindow(FALSE);
	m_hReservationExpireDate->EnableWindow(FALSE);
	
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	
	CESL_DataMgr* pLoanInfoDM = m_pParent->FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );
	
	CString strLoanStatus;
	pLoanInfoDM->GetCellData( _T("대출상태"), idx, strLoanStatus );	
	if ( strLoanStatus == _T("3") || strLoanStatus == _T("4") || strLoanStatus == _T("O") )
	{
		m_hReservationDate->ECO_EnableWindow(TRUE);
		m_hReservationDate->EnableWindow(TRUE);
		
		m_hReservationExpireDate->ECO_EnableWindow(TRUE);
		m_hReservationExpireDate->EnableWindow(TRUE);
		
		m_hReservationDate2->Control_EnableWindow(TRUE);
		m_hReservationDate2->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	
	if ( strLoanStatus == _T("0") || strLoanStatus == _T("2") || strLoanStatus == _T("L") )
	{
		m_hLoan->ECO_EnableWindow(TRUE);
		m_hLoan->EnableWindow(TRUE);
		
		m_hReturnPlandDate->ECO_EnableWindow(TRUE);
		m_hReturnPlandDate->EnableWindow(TRUE);
		
		m_hLoan2->Control_EnableWindow(TRUE);
		m_hLoan2->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	EFS_END
}

void CLoanInfoUpdateDlg::OnbtnNext() 
{
	// TODO: Add your control notification handler code here
	
	m_nCurGridPos += 1;
	setCurrentIndexData( m_pGrid->SelectGetNext() );		
	
}

void CLoanInfoUpdateDlg::OnbtnPrev() 
{
	// TODO: Add your control notification handler code here
	
	m_nCurGridPos -= 1;
	setCurrentIndexData( m_pGrid->SelectGetPrev() );				

}

BOOL CLoanInfoUpdateDlg::GridHasNext()
{
	if ( m_pGrid->SelectGetCount() == 0 )
		return FALSE;
	
	if ( m_nCurGridPos == ( m_pGrid->SelectGetCount() - 1 ) )
		return FALSE;

	if ( m_pGrid->m_arraySelect.FindIndex( m_nCurGridPos+1 ) == NULL )
		return FALSE;

	return TRUE;
}

BOOL CLoanInfoUpdateDlg::GridHasPrev()
{
	if ( m_pGrid->SelectGetCount() == 0 )
		return FALSE;
	
	if ( m_nCurGridPos == 0 )
		return FALSE;

	if ( m_pGrid->m_arraySelect.FindIndex( m_nCurGridPos-1 ) == NULL )
		return FALSE;

	return TRUE;
}

BOOL CLoanInfoUpdateDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->wParam == VK_ESCAPE ) {
		//return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CLoanInfoUpdateDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


HBRUSH CLoanInfoUpdateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	if ( pWnd->GetDlgCtrlID() == IDC_CAUTION )
	{
		pDC->SetTextColor(RGB(255,60,60));
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
