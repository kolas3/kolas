// SE_LOC_1560.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1560.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1560 dialog


CSE_LOC_1560::CSE_LOC_1560(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1560)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_LOC_1560::~CSE_LOC_1560()
{
}

VOID CSE_LOC_1560::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1560)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1560, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1560)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnCHANGE, OnbtnCHANGE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1560 message handlers
BOOL CSE_LOC_1560::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CSE_LOC_1560::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr(_T("SE_LOC_1560")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	m_pSpfWork = new CBL_SE_LOC_15(this);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



INT CSE_LOC_1560::ViewCurrentWindow()
{
	 

	INT ids;

	CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();

	ids = AllControlDisplay( _T("CM_SE_LOC_1560") , m_nCurrentIndex );
	if( ids < 0 ) return -1;

	GetDlgItem(IDC_btnPREV)->EnableWindow( TRUE );
	GetDlgItem(IDC_btnNEXT)->EnableWindow( TRUE );

	if( m_nCurrentIndex == m_nFirstIndex )
		GetDlgItem(IDC_btnPREV)->EnableWindow( FALSE );	
	if( m_nCurrentIndex == m_nLastIndex )
		GetDlgItem(IDC_btnNEXT)->EnableWindow( FALSE );
	
	INT nBookCnt=0;
	SetGridColoring(nBookCnt);

	CString sBookCnt;
	sBookCnt.Format( _T("%d") , nBookCnt );
	SetControlData( _T("CM_SE_LOC_1560") , _T("책수") , sBookCnt );

	return 0;
}


VOID CSE_LOC_1560::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	 

	CDialog::OnShowWindow(bShow, nStatus);

	bFirstView = TRUE;

	InitSetStartPos();
	INT ids = ViewCurrentWindow();
	if( ids < 0 ) return;

	bFirstView = FALSE;

	 
}

VOID CSE_LOC_1560::OnbtnPREV() 
{
	INT ids = MoveIndex( 0 );
	if( ids < 0 ) return;
}

VOID CSE_LOC_1560::OnbtnNEXT() 
{
	INT ids = MoveIndex( 1 );
	if( ids < 0 ) return;	 
}

INT CSE_LOC_1560::MoveIndex( INT nMode )
{
	INT ids;
	ids = InitSetStartPos();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSE_LOC_1560::MoveIndex( INT nMode )") );

	CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();
	if( nMode == 0 )
	{
		m_nCurrentIndex = pGrid->SelectGetPrev();
	}
	else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
	{
		m_nCurrentIndex = pGrid->SelectGetNext();
	}

	ViewCurrentWindow();

	return 0;

}


INT CSE_LOC_1560::InitSetStartPos()
{
	 

	CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();
	if( pGrid == NULL ) return -1;

	// 이전 다음이
	// 'V'체크가 된 것이 있다면 'V'체크 된것만 보여주며
	
	INT nTmpIndex;
	if( bFirstView ) nTmpIndex = pGrid->GetIdx();
	else nTmpIndex = m_nCurrentIndex;

	pGrid->SelectMakeList();
	
	INT nSelectCount = pGrid->SelectGetCount();
	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;

	m_nLastIndex = pGrid->SelectGetTailPosition();
	m_nFirstIndex = pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nSelectCount-1 ; i++ )
	{
		if( nTmpIndex == pGrid->SelectGetIdx() )
			break;
		pGrid->SelectGetNext();
	}
	if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
	else m_nCurrentIndex = pGrid->SelectGetIdx();

	return 0;

}


INT CSE_LOC_1560::SetGridColoring(INT &nBookCnt)
{
	INT ids;

	CString sParentDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = FindDM( sParentDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SetGridColoring") );

	CString sSelectSpeciesKey;
	CString sSpeciesKey;
	ids = pDM->GetCellData( _T("종정보KEY") , m_nCurrentIndex , sSelectSpeciesKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SetGridColoring") );


	CString sColor;
	nBookCnt = 0;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("종정보KEY") , i , sSpeciesKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SetGridColoring") );

		if( sSelectSpeciesKey.Compare(sSpeciesKey) == 0 )
		{
			sColor = _T("YELLOW");
			nBookCnt++;
		}
		else
			sColor = _T("WHITE");

		ids = GridColorChange( i , sColor);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SetGridColoring") );

	}

	sColor = _T("BLUE");
	ids = GridColorChange( m_nCurrentIndex , sColor);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SetGridColoring") );

	return 0;
}

INT CSE_LOC_1560::GridColorChange( INT nIndex , CString sColor )
{
	/// 20040512 조병걸 수정 -> 너무 느리다. 하지말자
/*
	CESL_Grid *pGrid = m_pSpfWork->GetMainGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GridColorChange") );

	COLORREF color;

	if( sColor.Compare(_T("WHITE")) == 0 )
		color = RGB(255,255,255);
	else if( sColor.Compare(_T("YELLOW")) == 0 )
		color = RGB(255,237,159);
	else if( sColor.Compare(_T("BLUE")) == 0 )
		color = RGB(228,243,251);

	INT nCols = pGrid->GetCols(0);

	pGrid->SetRow(nIndex+1);
	for( INT i = 1 ; i < nCols ; i++ )
	{
		pGrid->SetCol(i);
		pGrid->SetCellBackColor(color);
	}
*/
	return 0;
}

VOID CSE_LOC_1560::OnbtnCLOSE() 
{
	AllGridColorChange();
	OnCancel();
}


INT CSE_LOC_1560::AllGridColorChange()
{
	INT ids;

	CString sParentDMAlias = _T("DM_SE_LOC_1500");
	CESL_DataMgr *pDM = FindDM( sParentDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AllGridColorChange") );


	CString sColor =_T("WHITE");
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = GridColorChange( i , sColor);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("AllGridColorChange") );

	}

	return 0;
}

VOID CSE_LOC_1560::OnbtnCHANGE() 
{
	m_pSpfWork->SetTempData( _T("선택인덱스") , CComVariant(m_nCurrentIndex) );

	CString sBoxName;
	INT ids = GetControlData( _T("CM_SE_LOC_1560") , _T("함번호") , sBoxName );

	m_pSpfWork->SetTempData( _T("함번호") , CComVariant(sBoxName) );

	m_pSpfWork->SPFExcutePath( CHANGE_BOX_NAME );

}

HBRUSH CSE_LOC_1560::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
