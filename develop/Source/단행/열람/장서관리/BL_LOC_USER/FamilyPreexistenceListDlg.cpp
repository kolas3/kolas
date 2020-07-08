#include "stdafx.h"
#include "FamilyPreexistenceListDlg.h"
#include "..\LOC_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CFamilyPreexistenceListDlg::CFamilyPreexistenceListDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFamilyPreexistenceListDlg::IDD, pParent)
{
	m_nSelectIdx = -1;
}


CFamilyPreexistenceListDlg::~CFamilyPreexistenceListDlg()
{
	
}

void CFamilyPreexistenceListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFamilyPreexistenceListDlg) 
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CFamilyPreexistenceListDlg, CDialog)
	//{{AFX_MSG_MAP(CFamilyPreexistenceListDlg)
	ON_BN_CLICKED(IDC_BTN_USER_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_USER_SELECT, OnBtnSelect)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()




void CFamilyPreexistenceListDlg::OnBtnClose() 
{
	CDialog::OnCancel();
}

void CFamilyPreexistenceListDlg::OnBtnSelect() 
{

	if( 0 > m_nSelectIdx )
	{
		AfxMessageBox(_T("대출자를 먼저 선택해주십시오"), MB_ICONINFORMATION);
		return ;
	}

	CDialog::OnOK();
}

INT CFamilyPreexistenceListDlg::SelectIndex()
{
	return 0;
}

HBRUSH CFamilyPreexistenceListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CFamilyPreexistenceListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	if( 0 > InitESL_Mgr(_T("BO_LOC_FAMILY_PREEXISTENCE_DLG")) )
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	ControlDisplay( _T("CM_FAMILY_PREEXISTENCE_LIST") , _T("FamilyPreexistenceList") );
	
	SetReverse(0);

	CESL_ControlMgr* pCM = FindCM(_T("CM_FAMILY_PREEXISTENCE_LIST"));
	pCM->AllControlDisplay(-1);

	return TRUE;
}

BEGIN_EVENTSINK_MAP(CFamilyPreexistenceListDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFamilyPreexistenceListDlg)CFamilySearchDlg
	ON_EVENT(CFamilyPreexistenceListDlg, IDC_GRID_FAMILY_PREEXISTENCE_LIST, -605 /* MouseDown */, OnMouseDownGridFamilyPreexistenceList, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFamilyPreexistenceListDlg, IDC_GRID_FAMILY_PREEXISTENCE_LIST, -601 /* DblClick */, OnDblClickGridFamilyPreexistenceList, VTS_NONE)
	ON_EVENT(CFamilyPreexistenceListDlg, IDC_GRID_FAMILY_PREEXISTENCE_LIST, 69 /* SelChange */, OnSelChangeGridFamilyPreexistenceList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CFamilyPreexistenceListDlg::OnMouseDownGridFamilyPreexistenceList(short Button, short Shift, long x, long y) 
{
	
}

void CFamilyPreexistenceListDlg::OnDblClickGridFamilyPreexistenceList() 
{
	CESL_Grid* pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_FAMILY_PREEXISTENCE_LIST);
	INT		nRow	=	pGrid->GetMouseRow();
	if( 0 > nRow ) 
	{
		return;
	}

	OnBtnSelect();
}

void CFamilyPreexistenceListDlg::OnSelChangeGridFamilyPreexistenceList() 
{
	CESL_Grid* pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_FAMILY_PREEXISTENCE_LIST);
	
	INT nSelectIdx = pGrid->GetIdx();
	if(	m_nSelectIdx == nSelectIdx || 0 > nSelectIdx )
	{
		return;
	}

	SetReverse(nSelectIdx);
}

INT CFamilyPreexistenceListDlg::SetReverse(INT nRowIndex)
{
	m_nSelectIdx = nRowIndex;

	CESL_Grid* pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_FAMILY_PREEXISTENCE_LIST);
	
	return pGrid->SetReverse(m_nSelectIdx);
}

INT CFamilyPreexistenceListDlg::ControlDisplay(CString strCMAlias, CString strCtrlAlias, INT nIdx)
{
EFS_BEGIN

	INT ids = 0;

	CESL_DataMgr* pDstDM = FindDM(_T("DM_FAMILY_PREEXISTENCE_LIST"));
	if( NULL == pDstDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ControlDisplay") );
	}

	CString strValue;
	GetManageValue( _T("열람") , _T("공통") , _T("이용자관리") , _T("대출화면주민번호표시유무") , strValue );
	strValue.TrimLeft();		strValue.TrimRight();	strValue.MakeUpper();

	

	const int ROW_COUNT = pDstDM->GetRowCount();
	INT		nCnt;
	for( nCnt=0 ; nCnt<ROW_COUNT ; nCnt++ )
	{
		CString strCivilNo;
		ids = pDstDM->GetCellData( _T("주민등록번호") , nCnt , strCivilNo );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ControlDisplay") );
		}
		CLocCommonAPI::ChangeSecuriyCivilNo( this , strCivilNo , m_sIsUnityLoanService );

		ids = pDstDM->SetCellData(_T("주민등록번호_VIEW"), strCivilNo, nCnt);
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ControlDisplay") );
		}

		if( _T("Y") != strValue )
		{
			CString strUserNo;
			ids = pDstDM->GetCellData( _T("대출자번호") , nCnt , strUserNo );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("ControlDisplay") );
			}

			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_sIsUnityLoanService );

			
			ids = pDstDM->SetCellData(_T("대출자번호_VIEW"), strUserNo, nCnt);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("ControlDisplay") );
			}
		}

	}
	
	int nResult = CESL_Mgr::ControlDisplay( strCMAlias , strCtrlAlias , nIdx );
	
	return ids;

EFS_END
return -1;
}
