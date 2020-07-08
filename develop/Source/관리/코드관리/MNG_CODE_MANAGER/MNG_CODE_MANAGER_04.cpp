// MNG_CODE_MANAGER_04.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CODE_MANAGER_04.h"
#include "MNG_CODE_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_04 dialog


CMNG_CODE_MANAGER_04::CMNG_CODE_MANAGER_04( CESL_Mgr* pParent /*=NULL*/ )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CODE_MANAGER_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
}

BOOL CMNG_CODE_MANAGER_04::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CODE_MANAGER_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CODE_MANAGER_04)
	DDX_Control(pDX, IDC_PROGRESS_PATTEN_1, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CODE_MANAGER_04, CDialog)
	//{{AFX_MSG_MAP(CMNG_CODE_MANAGER_04)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CODE_MANAGER_04 message handlers

VOID CMNG_CODE_MANAGER_04::OnOK() 
{
	// TODO: Add extra validation here
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT1 );
	pEdt->GetWindowText( m_patternName );

	if( !m_patternName.GetLength() )
	{
		AfxMessageBox( _T("패턴 이름을 입력하여 주십시오") );
		return;
	}
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_04_CODE") );
	pDM->RefreshDataManager( _T("") );
	
	CString tmp;
	if( MakeCodePattern( pDM) )
	{
		AfxMessageBox( _T("Error : MakeCodePattern( CESL_DataMgr *pDM )") );
		return;
	}
	else
	{
		tmp.Format( _T(" %s CODE Pattern 을 생성하였습니다."), m_patternName );
		AfxMessageBox( tmp );
	}

	CMNG_CODE_MANAGER_01 *pParent = ( CMNG_CODE_MANAGER_01 * )pParentMgr;
	pParent->m_patternName = m_patternName;
	pParent->ViewCodePattern();
	
	CDialog::OnOK();
}

VOID CMNG_CODE_MANAGER_04::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CMNG_CODE_MANAGER_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CODE_MANAGER_04") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CODE_MANAGER_04") );
		return false;
	}
	
	CMNG_CODE_MANAGER_01 *pParent = ( CMNG_CODE_MANAGER_01 * )pParentMgr;
	pParent->ViewToolBarBtns();

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CMNG_CODE_MANAGER_04::MakeCodePattern( CESL_DataMgr *pDM )
{
	const INT fieldCnt = 5;

	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"),			//	0
		_T("CODE_KEY"),			//	1
		_T("PATTERN_NAME"),		//	2
		_T("ACTIVE"),			//	3
		_T("DEFAULT_VALUE") };	//	4

	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };

	CString data[ fieldCnt ];

	data[ 2 ] = m_patternName;
	data[ 3 ] = _T("Y");

	pDM->StartFrame();
	pDM->InitDBFieldData();

	CString sClass, sBeforeClass;
	
	CString tmp;
	INT rowCnt = pDM->GetRowCount();
	bool defaultFlag = false;
	for( INT i = 0; i < rowCnt; i++ )
	{
		if( pDM->m_dbkind == 1 )
			data[ 0 ] = _T("ESL_SEQ.NEXTVAL");
		else
			pDM->MakeRecKey(data[0]);

		pDM->GetCellData( _T("CLASS"), i, sClass );
		if( i )
			pDM->GetCellData( _T("CLASS"), i - 1, sBeforeClass );
		if( sClass != sBeforeClass )
			defaultFlag = false;

		tmp.Format( _T(" %d / %d code 복사중"), i + 1, rowCnt );
		SetWindowText( tmp );

		pDM->GetCellData( _T("REC_KEY"), i, data[ 1 ] );
		
		pDM->GetCellData( _T("CODE"), i, data[ 4 ] );
		data[ 4 ].TrimLeft();
		data[ 4 ].TrimRight();
		if( ( !data[ 4 ].GetLength() || _T("0") == data[ 4 ] ) && !defaultFlag )
		{
			data[ 4 ] = _T("Y");
			defaultFlag = true;
		}
		else
			data[ 4 ] = _T("N");

		for( INT j = 0; j < fieldCnt; j++ )
			pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ] );

		pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("코드관리"), __WFILE__, __LINE__ ), TRUE );
		pDM->MakeInsertFrame( _T("MN_CODE_PATTERN_TBL") );
	}

	pDM->SendFrame();
	pDM->EndFrame();

	return 0;
}

VOID CMNG_CODE_MANAGER_04::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	CMNG_CODE_MANAGER_01 *pParent = ( CMNG_CODE_MANAGER_01 * )pParentMgr;

	pParent->ViewTabControl( 1 );
}

INT CMNG_CODE_MANAGER_04::GetPK()
{
	CString query = _T("SELECT MAX( REC_KEY ) FROM MN_CODE_PATTERN_TBL");
	CString pk;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CODE_MANAGER_04_CODE") );
	pDM->GetOneValueQuery( query, pk );
	
	return _ttoi( pk ) + 1;
}
