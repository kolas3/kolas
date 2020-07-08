// DLG_BATCH_MODIFY.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_BATCH_MODIFY.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_BATCH_MODIFY dialog


CDLG_BATCH_MODIFY::CDLG_BATCH_MODIFY(CESL_Mgr* pParent )
	: CESL_Mgr(CDLG_BATCH_MODIFY::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLG_BATCH_MODIFY)
	m_bCheck_1 = FALSE;
	m_bCheck_2 = FALSE;
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pMainCM = NULL;
}


void CDLG_BATCH_MODIFY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_BATCH_MODIFY)
	DDX_Check(pDX, IDC_CHECK1, m_bCheck_1);
	DDX_Check(pDX, IDC_CHECK2, m_bCheck_2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLG_BATCH_MODIFY, CDialog)
	//{{AFX_MSG_MAP(CDLG_BATCH_MODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_BATCH_MODIFY message handlers

BOOL CDLG_BATCH_MODIFY::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	INT ids, count;
	CString strcnt;
	CESL_Grid* pGrid = NULL;

	if( InitESL_Mgr(_T("K18_희망도서_일괄수정")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_K18_희망도서_일괄수정"));
	m_pDM = FindDM(_T("DM_비치희망자료관리"));
	m_pMainCM = FindCM(_T("CM_비치희망자료관리"));
	pGrid		= (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	if( m_pCM == NULL )
	{
		ESLAfxMessageBox(_T("[CM_K18_희망도서_일괄수정]를 설정할 수 없습니다."));
		return FALSE;
	}


	if( m_pDM == NULL || m_pMainCM == NULL || pGrid == NULL )
	{
		ESLAfxMessageBox(_T("[CM/DM_비치희망자료관리]를 설정할 수 없습니다."));
		return FALSE;
	}


	ids = pGrid->SelectMakeList();
	count = pGrid->SelectGetCount();
	if( ids < 0 || count <= 0 )
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		this->EndDialog(IDCANCEL);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	strcnt.Format(_T("%d\n"), count);
	OutputDebugString( strcnt );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLG_BATCH_MODIFY::OnOK() 
{
	CString	str_Check_1 , str_Check_2;
	CString reckey, skey ;

	CESL_Grid* pGrid = NULL;
	pGrid		= (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));

	UpdateData(TRUE);

	if( m_bCheck_1 )	str_Check_1 = "Y";
	else				str_Check_1 = "N";

	if( m_bCheck_2 )	str_Check_2 = "Y";
	else				str_Check_2 = "N";

	INT ids, i;
	ids = pGrid->SelectMakeList();
	i =	pGrid->SelectGetHeadPosition();

	m_pDM->StartFrame();
	while( i >= 0 )
	{	
		m_pDM->InitDBFieldData();

		m_pDM->GetCellData(_T("비치희망KEY"), i, reckey);

		m_pDM->AddDBFieldData(_T("RESERVATION_YN"), _T("STRING"), str_Check_1);
		m_pDM->AddDBFieldData(_T("SMS_RECEIPT_YN"), _T("STRING"), str_Check_2);

		m_pDM->MakeUpdateFrame(m_pDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), reckey, i);	
		i = pGrid->SelectGetNext();	
	}

	ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	int total_update = 0;
	if( ids >= 0 )
	{
		ids = pGrid->SelectMakeList();
		i =	pGrid->SelectGetHeadPosition();
		while( i >= 0 )
		{
			m_pDM->SetCellData(_T("우선대출예약여부")	, str_Check_1 , i);
			m_pDM->SetCellData(_T("SMS수신여부")		, str_Check_2 , i);
			i = pGrid->SelectGetNext();	
			total_update++;
		}
		m_pCM->AllControlDisplay();
	}
	else if( ids < 0 )
	{
		ESLAfxMessageBox(_T("DataBase에 적용하던중 에러가 발생하였습니다."));
	}


	CString str;
	str.Format( _T("일괄수정 : ids : %d") , ids );
	OutputDebugString( str );

	if( total_update > 0 )
	{
		str.Format( _T("%d 건이 수정되었습니다.") , total_update);
		AfxMessageBox( str );
	}

	CDialog::OnOK();
}

