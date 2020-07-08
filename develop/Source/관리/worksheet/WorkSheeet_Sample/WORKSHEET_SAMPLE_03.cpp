// WORKSHEET_SAMPLE_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "WORKSHEET_SAMPLE_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWORKSHEET_SAMPLE_03 dialog


CWORKSHEET_SAMPLE_03::CWORKSHEET_SAMPLE_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CWORKSHEET_SAMPLE_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWORKSHEET_SAMPLE_03::~CWORKSHEET_SAMPLE_03()
{
	
}

BOOL CWORKSHEET_SAMPLE_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	
	
VOID CWORKSHEET_SAMPLE_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWORKSHEET_SAMPLE_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWORKSHEET_SAMPLE_03, CDialog)
	//{{AFX_MSG_MAP(CWORKSHEET_SAMPLE_03)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWORKSHEET_SAMPLE_03 message handlers

BOOL CWORKSHEET_SAMPLE_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MAIN_SAMPLE_SEARCH") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MAIN_SAMPLE_SEARCH") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CWORKSHEET_SAMPLE_03::OnOK() 
{
	/*
	CESL_DataMgr *pDM = FindDM( _T("DM_MAIN_SAMPLE") );
	pDM->RefreshDataManager( _T("") );

	CString strTmp;
	INT YY, MM, DD;
	srand(time(NULL));
	INT nRow = pDM->GetRowCount();
	CString strID;
	CString strQuery;
	pDM->StartFrame();
	for (INT i = 0; i < nRow; i++) {
		YY = rand() % 20 + 1970;
		MM = rand() % 12 + 1;
		DD = rand() % 27 + 1;
		strTmp.Format(_T("%04d/%02d/%02d"), YY, MM, DD);
		pDM->GetCellData(_T("ID"), i, strID);
		strQuery.Format(_T("UPDATE ESL_TEST SET BIRTHDAY='%s' WHERE ID='%s';"), strTmp, strID);
		pDM->AddFrame(strQuery);
	}
	
	INT ids = pDM->SendFrame();
	strTmp.Format(_T("%d"), ids);
	AfxMessageBox(strTmp);
	pDM->EndFrame();
	*/
	// TODO: Add extra validation here
	CString id, rank;
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( id );
	rank = TakeControlData( _T("직급") );
	
	CString where, tmp;
	if( id.GetLength() )
	{
		tmp.Format( _T(" ID = '%s' AND"), id );
		where += tmp;
	}
	
	rank.TrimLeft(); rank.TrimRight();
	rank.Replace( _T(" "), _T("") );
	if( rank.GetLength() )
	{
		tmp.Format( _T(" GRADE = '%s' AND"), id );
		where += tmp;
	}

	if( _T("AND") == where.Right( 3 ) )
	{
		tmp = where.Left( where.GetLength() - 3 );
		where = tmp;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MAIN_SAMPLE") );
	pDM->RefreshDataManager( where );

	ControlDisplay( _T("CM_MAIN_SAMPLE"), _T("MainGrid") );
	CDialog::OnOK();
}

CString CWORKSHEET_SAMPLE_03::TakeControlData(CString alias, INT option)
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MAIN_SAMPLE_SEARCH"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MAIN_SAMPLE_SEARCH"), _T("K2Up") );
		return _T("");
	}
	
	// 코드값이 _T("0") 이거나 설명값이 _T("적용안함")일때 _T("")를 반환한다. 
	switch( option )
	{
		case 0 :
			if( _T("0") == result )
				return _T("");
			break;
		case 1 :
			if( _T("적용안함") == result )
				return _T("");
			break;
		default :
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_MAIN_SAMPLE_SEARCH"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

HBRUSH CWORKSHEET_SAMPLE_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
