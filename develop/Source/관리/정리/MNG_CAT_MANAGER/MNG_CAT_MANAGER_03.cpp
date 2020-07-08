// MNG_CAT_MANAGER_03.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CAT_MANAGER_03.h"

#include "MNG_CAT_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CAT_MANAGER_03 dialog


CMNG_CAT_MANAGER_03::CMNG_CAT_MANAGER_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CAT_MANAGER_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_CAT_MANAGER_03::~CMNG_CAT_MANAGER_03()
{
}

BOOL CMNG_CAT_MANAGER_03::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	

VOID CMNG_CAT_MANAGER_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CAT_MANAGER_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CAT_MANAGER_03, CDialog)
	//{{AFX_MSG_MAP(CMNG_CAT_MANAGER_03)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CAT_MANAGER_03 message handlers

BOOL CMNG_CAT_MANAGER_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CAT_MANAGER_03") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CAT_MANAGER_03") );
		return false;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMNG_CAT_MANAGER_03::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT ctrlCnt = 4;
	CString ctrlData[ ctrlCnt ];
	GetCtrlData( ctrlData );

	CMNG_CAT_MANAGER_01 *pParent = ( CMNG_CAT_MANAGER_01 * )pParentMgr;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_01") );	
	pDM->RefreshDataManager(MakeSelectQuery( ctrlData ));
	pParent->ShowGrid();

	if( !pDM->GetRowCount() )
		AfxMessageBox( _T("검색 결과가 없습니다.") );

	//CDialog::OnOK();
}

VOID CMNG_CAT_MANAGER_03::GetCtrlData( CString data[] )
{
	const INT ctrlCnt = 4;

	CString ctrlAlias[ ctrlCnt ] = {
		_T("형식구분"), _T("보조등록구분"), _T("분류구분"), _T("발행형태구분") };
	
	for( INT i = 0; i < ctrlCnt; i++ )
		data[ i ] = TakeControlData( ctrlAlias[ i ] );
}

CString CMNG_CAT_MANAGER_03::TakeControlData(CString alias, INT option)
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MNG_CAT_MANAGER_03"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MNG_CAT_MANAGER_03"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_MNG_CAT_MANAGER_03"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

CString CMNG_CAT_MANAGER_03::MakeSelectQuery( CString data[] )
{
	CString query, tmp;

	const INT fieldCnt = 4;

	CString fieldName[ fieldCnt ] = {
		_T("FORM_CODE"), _T("SUB_REG_CODE"), _T("CLASS_CODE"), _T("PUBLISH_FORM_CODE") };

	for( INT i = 0; i < fieldCnt; i++ )
	{
		data[ i ].TrimLeft();
		data[ i ].TrimRight();

		if( data[i].IsEmpty() ) continue;
		
		// 분류기호이면 우절단 검색
		if( i == 2 )
		{
			tmp.Format( _T(" %s LIKE ('%s%%') AND"), fieldName[ i ], data[ i ] );
			query += tmp;			
		}
		else if( data[ i ].GetLength() )
		{
			tmp.Format( _T(" %s = '%s' AND"), fieldName[ i ], data[ i ] );
			query += tmp;
		}
	}

	if( _T("AND") == query.Right( 3 ) )
	{
		tmp = query.Left( query.GetLength() - 3 );
		query = tmp;
	}

	return query;
}

HBRUSH CMNG_CAT_MANAGER_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
