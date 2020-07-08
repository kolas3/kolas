// MNG_CAT_MANAGER_04.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_CAT_MANAGER_04.h"

#include "MNG_CAT_MANAGER_01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_CAT_MANAGER_04 dialog


CMNG_CAT_MANAGER_04::CMNG_CAT_MANAGER_04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_CAT_MANAGER_04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_CAT_MANAGER_04::~CMNG_CAT_MANAGER_04()
{
}

BOOL CMNG_CAT_MANAGER_04::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}	


VOID CMNG_CAT_MANAGER_04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_CAT_MANAGER_04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_CAT_MANAGER_04, CDialog)
	//{{AFX_MSG_MAP(CMNG_CAT_MANAGER_04)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bINPUT, OnbINPUT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_bUP, OnbUP)
	ON_BN_CLICKED(IDC_bDOWN, OnbDOWN)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_CAT_MANAGER_04 message handlers

BOOL CMNG_CAT_MANAGER_04::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_CAT_MANAGER_04") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_CAT_MANAGER_04") );
		return false;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// CM_MNG_CAT_MANAGER_04

VOID CMNG_CAT_MANAGER_04::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	
	CEdit *pEdt;	
	switch( m_mode )
	{
		case 1 :
			
			SetWindowText( _T("저자기호표 설정 - 입력") );
			pEdt = ( CEdit * )GetDlgItem( IDC_bBEFORE );
			pEdt->ShowWindow( false );
			pEdt = ( CEdit * )GetDlgItem( IDC_bNEXT );
			pEdt->ShowWindow( false );
			
			break;

		case 2 :
			
			SetWindowText( _T("저자기호표 설정 - 수정") );
			pEdt = ( CEdit * )GetDlgItem( IDC_bBEFORE );
			pEdt->ShowWindow( true );
			pEdt = ( CEdit * )GetDlgItem( IDC_bNEXT );
			pEdt->ShowWindow( true );
			
			m_index = -1;
			DisplayModifyData( 0 );
			
			break;

		default :
			break;
	}
}

// Tag 입력
VOID CMNG_CAT_MANAGER_04::OnbINPUT() 
{
	// TODO: Add your control notification handler code here

	CMNG_CAT_MANAGER_05 dlg( this );
	dlg.m_mode = 1;
	
	if( IDOK == dlg.DoModal() )
	{
		AddTagToDM( dlg.m_tag, dlg.m_subFieldCode );
		ShowTagGrid();
	}
}

// Tag 수정
VOID CMNG_CAT_MANAGER_04::OnbMODIFY() 
{
	// TODO: Add your control notification handler code here
	
	CString tag, subFieldCode;
	INT checkedDMRow = GetCheckedTagInfo( tag, subFieldCode );
	if( -1 == checkedDMRow )
	{
		AfxMessageBox( _T("선택된 항목이 없습니다.") );
		return;
	}
	
	CMNG_CAT_MANAGER_05 dlg( this );
	dlg.m_mode = 2;
	dlg.m_tag = tag;
	dlg.m_subFieldCode = subFieldCode;
	
	if( IDOK == dlg.DoModal() )
	{
		AddTagToDM( dlg.m_tag, dlg.m_subFieldCode, checkedDMRow );
		ShowTagGrid();	
	}
}

// 저장
VOID CMNG_CAT_MANAGER_04::OnOK() 
{
	// TODO: Add extra validation here
	
	const INT fieldCnt = 11 + 5;
	CString alias[fieldCnt], data[fieldCnt];
	GetFieldData(alias, data);


	//수입순번호일때 tag 체크 하지 않음
	if ( data[2] != _T("3") )
	{
		//테그 체크
		for( INT i = 0; i < 8; i++ )
			if( 3 == i )
				continue;
			else if( !data[ i ].GetLength() )
			{
				AfxMessageBox( _T("필수 항목을 입력하여 주십시오") );
				return;
			}
	}
	
	INT cnt = CheckOverLap( data );
	switch( m_mode )
	{
		case 1 :
			if( cnt )
			{
				AfxMessageBox( _T("중복되는 데이타가 존재합니다. 다시 입력하여 주십시오") );
				return;
			}
			break;
		case 2 :
			if( cnt > 1 )
			{
				AfxMessageBox( _T("중복되는 데이타가 존재합니다. 다시 입력하여 주십시오") );
				return;
			}
			break;
		default :
			break;
	}
		
	if( SaveData( data ) )
	{
		AfxMessageBox( _T("Error : SaveData( CString data[] )") );
		return;
	}

	CMNG_CAT_MANAGER_01 *pParent = ( CMNG_CAT_MANAGER_01 * )pParentMgr;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_01") );
//  	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_CAT_MANAGER_01"), _T("그리드"));
	pDM->SetCellData(alias, 8, data, m_index);
	pParent->ShowGrid();

	switch( m_mode )
	{
		case 1 :
			AfxMessageBox( _T("저장되었습니다.") );
			CDialog::OnOK();	
			break;
		case 2 :
			AfxMessageBox( _T("수정되었습니다.") );
			break;
	}	
}

VOID CMNG_CAT_MANAGER_04::GetFieldData(CString alias[], CString data[] )
{
	const INT ctrlCnt = 5;
	CString ctrlAlias[ ctrlCnt ] = 
	{
		_T("발행형태구분"), _T("형식구분"), _T("생성방법"), _T("분류기호"), _T("보조등록구분")
	};
	CString strAlias[ctrlCnt] = 
	{
		_T("PUBLISH_FORM_CODE"), _T("FORM_CODE"), _T("RULE_TYPE"), _T("CLASS_CODE"), _T("SUB_REG_CODE")
	};

	for( INT i = 0; i < ctrlCnt; i++ )
	{
		alias[i] = strAlias[i];
		data[i] = TakeControlData(ctrlAlias[i]);
	}

	GetTagData(alias, data);
}

VOID CMNG_CAT_MANAGER_04::AddTagToDM( CString tag, CString subFieldCode, INT option )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_04") );

	CString alias[ 3 ] = { _T("태그"), _T("식별기호"), _T("우선순위") };
	CString data[ 3 ];
	data[ 0 ] = tag;
	data[ 1 ] = subFieldCode;
	
	if( -1 == option )
	{
		data[ 2 ].Format( _T("%d"), GetCurrentTagRank() );
		pDM->SetCellData( alias, 3, data, option );
	}
	else
		pDM->SetCellData( alias, 2, data, option );
}

VOID CMNG_CAT_MANAGER_04::ShowTagGrid()
{
	ControlDisplay( _T("CM_MNG_CAT_MANAGER_04"), _T("그리드") );
}

INT CMNG_CAT_MANAGER_04::GetCurrentTagRank()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_04") );
	
	CString rank;
	INT currentRank = -1;

	INT rowCnt = pDM->GetRowCount();
	if( !rowCnt )
		return 1;
	
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("우선순위"), i, rank );

		if( currentRank < _ttoi( rank ) )
			currentRank = _ttoi( rank );
	}

	return currentRank + 1;
}

BEGIN_EVENTSINK_MAP(CMNG_CAT_MANAGER_04, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_CAT_MANAGER_04)
	ON_EVENT(CMNG_CAT_MANAGER_04, IDC_mngGRID_2, -600 /* Click */, OnClickmngGRID2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_CAT_MANAGER_04::OnClickmngGRID2() 
{
	// TODO: Add your control notification handler code here

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )
		return;

	if( _T("V") == pGrid->GetText() )
	{
		pGrid->SetText( _T("") );
		return;
	}
	else
		pGrid->SetText( _T("V") );

	for( INT i = 1; i < pGrid->GetRows(); i++ )
		if( row == i )
			continue;
		else
		{
			pGrid->SetRow( i );
			pGrid->SetText( _T("") );
		}

	pGrid->SetRow( row );
}

INT CMNG_CAT_MANAGER_04::GetCheckedTagInfo( CString &tag, CString &subFieldCode )
{
	INT checkedDMRow = GetCheckedDMRow();
	if( -1 == checkedDMRow )
		return -1;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_04") );
	
	pDM->GetCellData( _T("태그"), checkedDMRow, tag );
	pDM->GetCellData( _T("식별기호"), checkedDMRow, subFieldCode );

	return checkedDMRow;
}

INT CMNG_CAT_MANAGER_04::GetCheckedDMRow()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	pGrid->SetCol( 1 );
	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );
		
		if( _T("V") == pGrid->GetText() )
		{
			pGrid->SetRow( row );
			pGrid->SetCol( col );

			return i - 1;
		}
	}

	pGrid->SetRow( row );
	pGrid->SetCol( col );

	return -1;
}

// tag 우선순위 up
VOID CMNG_CAT_MANAGER_04::OnbUP() 
{
	// TODO: Add your control notification handler code here
	
	ModifyTagRank( 1 );
}

// tag 우선순위 down
VOID CMNG_CAT_MANAGER_04::OnbDOWN() 
{
	// TODO: Add your control notification handler code here
	
	ModifyTagRank( -1 );
}

VOID CMNG_CAT_MANAGER_04::ModifyTagRank( INT flag )
{
	INT checkedDMRow = GetCheckedDMRow();
	if( -1 == checkedDMRow )
	{
		AfxMessageBox( _T("선택된 태그가 없습니다.") );
		return;
	}

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_04") );
	INT rowCnt = pDM->GetRowCount();

	INT newLocate = checkedDMRow - flag;
	if( newLocate < 0 || newLocate > rowCnt - 1 )
		return;

	ChangeTagLocate( checkedDMRow, newLocate );
	ShowTagGrid();

	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	pGrid->SetRow( newLocate + 1 );
	pGrid->SetCol( 1 );
	pGrid->SetText( _T("V") );
}

VOID CMNG_CAT_MANAGER_04::ChangeTagLocate( INT from, INT to )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_04") );

	const INT aliasCnt = 2;
	CString alias[ aliasCnt ] = { _T("태그"), _T("식별기호") };

	CString fromData[ aliasCnt ];
	CString toData[ aliasCnt ];

	pDM->GetCellData( alias, aliasCnt, fromData, from );
	pDM->GetCellData( alias, aliasCnt, toData, to );

	pDM->SetCellData( alias, aliasCnt, fromData, to );
	pDM->SetCellData( alias, aliasCnt, toData, from );
}

CString CMNG_CAT_MANAGER_04::TakeControlData( CString alias, INT option )
{
	// option == 0 : 멀티콤보의 코드값을 가져온다. 
	// option == 1 : 멀티콤보의 코드설명값을 가져온다. 
	
	CString result;

	INT ids = 0;
	ids = GetControlData( _T("CM_MNG_CAT_MANAGER_04"), alias, result, -1, option );
	if( ids < 0 )
	{
		MessageBox( _T("Error : GetControlData() of CM_MNG_CAT_MANAGER_04"), _T("K2Up") );
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
			MessageBox( _T("Error , Wrong Option : TakeControlData of CM_MNG_CAT_MANAGER_04"), _T("K2Up") );
			return _T("");
			break;
	}

	return result;
}

VOID CMNG_CAT_MANAGER_04::GetTagData(CString alias[], CString data[] )
{
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_04") );
	
	INT rowCnt = pDM->GetRowCount();
	data[ 5 ].Format( _T("%d"), rowCnt );

	CString buf;
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("태그"), i, buf );
		data[ 6 ] = data[ 6 ] + buf + _T("/");

		pDM->GetCellData( _T("식별기호"), i, buf );
		data[ 7 ] = data[ 7 ] + buf + _T("/");
	}

	if( _T("/") == data[ 6 ].Right( 1 ) )
	{
		buf = data[ 6 ].Left( data[ 6 ].GetLength() - 1 );
		data[ 6 ] = buf;
	}

	if( _T("/") == data[ 7 ].Right( 1 ) )
	{
		buf = data[ 7 ].Left( data[ 7 ].GetLength() - 1 );
		data[ 7 ] = buf;
	}

	alias[5] = _T("TAG_CNT");
	alias[6] = _T("TAG");
	alias[7] = _T("SUBFIELD_CODE");
}

INT CMNG_CAT_MANAGER_04::SaveData( CString data[] )
{
	const INT fieldCnt = 11;
	CString fieldName[ fieldCnt + 5 ] = {
		_T("PUBLISH_FORM_CODE"),	//	0
		_T("FORM_CODE"),			//	1
		_T("RULE_TYPE"),			//	2
		_T("CLASS_CODE"),			//	3
		_T("SUB_REG_CODE"),			//	4
		_T("TAG_CNT"),				//	5
		_T("TAG"),					//	6
		_T("SUBFIELD_CODE"),		//	7
		_T("REC_KEY"),				//	8
		_T("FIRST_WORK"),			//	9
		_T("LAST_WORK"),			// 10
		// 설명 필드 추가
		_T("FORM_CODE_DESC"),				// 11
		_T("PUBLISH_FORM_CODE_DESC"),		// 12
		_T("RULE_TYPE_DESC"),				// 13
		_T("SUBFIELD_CODE_DESC"),			// 14
		_T("SUB_REG_CODE_DESC") };			// 15

	CString fieldType[ fieldCnt ] = {
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC"), 
		_T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING") };

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_01") );
	
	switch( m_mode )
	{
		case 1 :
			pDM->MakeRecKey( data[ 8 ] );
			data[ 9 ] = GetWorkLogMsg( _T("정리관리"), __WFILE__, __LINE__ );
			break;
		case 2 :
			pDM->GetCellData( _T("REC_KEY"), m_index, data[ 8 ] );
			break;
	}
	data[ 10 ] = GetWorkLogMsg( _T("정리관리"), __WFILE__, __LINE__ );

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
	{
		if( data[ 5 ].IsEmpty() )
			data[ 5 ] = _T("0");
		pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i ] );
	}

	data[ 11 ] = data[ 1 ];
	data[ 12 ] = data[ 0 ];
	data[ 13 ] = data[ 2 ];
	data[ 14 ] = data[ 7 ];
	data[ 15 ] = data[ 4 ];		

	switch( m_mode )
	{
		case 1 :
			pDM->MakeInsertFrame( _T("MN_AUTHOR_RULE_TBL") );
			
			pDM->SetCellData( fieldName, fieldCnt + 5, data, -1 );

			break;

		case 2 :
			pDM->MakeUpdateFrame( _T("MN_AUTHOR_RULE_TBL"), _T("REC_KEY"), _T("NUMERIC"), data[ 8 ], m_index );
			
			pDM->SetCellData( fieldName[ 11 ], data[ 11 ], m_index );
			pDM->SetCellData( fieldName[ 12 ], data[ 12 ], m_index );
			pDM->SetCellData( fieldName[ 13 ], data[ 13 ], m_index );
			pDM->SetCellData( fieldName[ 14 ], data[ 14 ], m_index );
			pDM->SetCellData( fieldName[ 15 ], data[ 15 ], m_index );

			break;;
	}		

	pDM->SendFrame();
	pDM->EndFrame();

	return 0;
}

VOID CMNG_CAT_MANAGER_04::DisplayModifyData( INT flag )
{
	switch( flag )	
	{
		case 0 :

			SelectMakeList( _T("CM_MNG_CAT_MANAGER_01"), _T("그리드") );
			m_index = SelectGetHeadPosition( _T("CM_MNG_CAT_MANAGER_01"), _T("그리드") );

			if( m_index < 0 )
			{
				AfxMessageBox( _T("선택된 코드가 없습니다.") );
				PostMessage( WM_CLOSE );
				return;
			}

			break;

		case -1 :

			m_index = SelectGetPrev( _T("CM_MNG_CAT_MANAGER_01"), _T("그리드") );

			if( -1 == m_index )
			{
				m_index = SelectGetHeadPosition( _T("CM_MNG_CAT_MANAGER_01"), _T("그리드") );
				AfxMessageBox( _T(" 맨 처음 코드입니다.") );
				return;
			}

			break;

		case 1 :

			m_index = SelectGetNext( _T("CM_MNG_CAT_MANAGER_01"), _T("그리드") );
			if( -1 == m_index )
			{
				m_index = SelectGetTailPosition( _T("CM_MNG_CAT_MANAGER_01"), _T("그리드") );
				AfxMessageBox( _T("마지막 코드입니다.") );
				return;
			}

			break;

		default :

			break;
	}

	SetTagDataToDM();
	AllControlDisplay( _T("CM_MNG_CAT_MANAGER_04"), m_index );
}

VOID CMNG_CAT_MANAGER_04::OnbBEFORE() 
{
	// TODO: Add your control notification handler code here

	DisplayModifyData( -1 );	
}

VOID CMNG_CAT_MANAGER_04::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	
	DisplayModifyData( 1 );	
}

VOID CMNG_CAT_MANAGER_04::SetTagDataToDM()
{
	CESL_DataMgr *pDM;

	CString tagList, subFieldCodeList, cnt;
	pDM = FindDM( _T("DM_MNG_CAT_MANAGER_01") );
	pDM->GetCellData( _T("TAG"), m_index, tagList );
	pDM->GetCellData( _T("SUBFIELD_CODE"), m_index, subFieldCodeList );
	pDM->GetCellData( _T("TAG_CNT"), m_index, cnt );

	INT tagCnt = _ttoi( cnt );
	CString *tag = new CString[ tagCnt ];
	CString *subFieldCode = new CString[ tagCnt ];
	
	GetTagFromString( tagList, tag, tagCnt );
	GetTagFromString( subFieldCodeList, subFieldCode, tagCnt );

	const INT dmCols = 3;
	CString dmAlias[ dmCols ] = {
		_T("태그"), _T("식별기호"), _T("우선순위") };
	CString data[ dmCols ];
	
	pDM = FindDM( _T("DM_MNG_CAT_MANAGER_04") );
	pDM->FreeData();
	for( INT i = 0; i < tagCnt; i++ )
	{
		data[ 0 ] = tag[ i ];
		data[ 1 ] = subFieldCode[ i ];
		data[ 2 ].Format( _T("%d"), i + 1 );

		pDM->SetCellData( dmAlias, dmCols, data, -1 );
	}

	delete [] tag;
	delete [] subFieldCode;
}

VOID CMNG_CAT_MANAGER_04::GetTagFromString( CString tagList, CString *tag, INT tagCnt )
{
	CString buf;
	INT locate = 0;

	for( INT i = 0; i < tagCnt; i++ )
		if( i == tagCnt - 1 )
			tag[ i ] = tagList;
		else
		{
			locate = tagList.Find( _T("/") );
			tag[ i ] = tagList.Left( locate );
			buf = tagList.Right( tagList.GetLength() - locate - 1 );
			tagList = buf;	
		}
}

INT CMNG_CAT_MANAGER_04::CheckOverLap( CString data[] )
{
	const INT fieldCnt = 5;
	CString fieldName[ fieldCnt ] = {
		_T("PUBLISH_FORM_CODE"), _T("FORM_CODE"), _T(""), _T("CLASS_CODE"), _T("SUB_REG_CODE") };

	CString query, where, tmp;
	for( INT i = 0; i < fieldCnt; i++ )
		if( 2 == i )
			continue;
		else if( data[ i ].GetLength() )
		{
			tmp.Format( _T(" %s = '%s' AND"), fieldName[ i ], data[ i ] );
			where += tmp;
		}

	if( _T("AND") == where.Right( 3 ) )
	{
		tmp = where.Left( where.GetLength() - 3 );
		where = tmp;
	}

	query.Format( _T("SELECT COUNT( * ) FROM MN_AUTHOR_RULE_TBL WHERE %s AND MANAGE_CODE=UDF_MANAGE_CODE"), where );
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_CAT_MANAGER_01") );
	CString cnt;
	pDM->GetOneValueQuery( query, cnt );
	
	return _ttoi( cnt );
}

VOID CMNG_CAT_MANAGER_04::OnbDELETE() 
{
	DeleteTagInfo();
}


INT CMNG_CAT_MANAGER_04::DeleteTagInfo()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_CAT_MANAGER_04"), _T("그리드"));
	if( pGrid == NULL ) return -1;

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("삭제할 자료를 선택해주십시오") );
		return 1;
	}

	pGrid->SelectGetTailPosition();

	CESL_DataMgr* pDM = FindDM( _T("DM_MNG_CAT_MANAGER_04") );
	if( pDM == NULL ) return -2;

	INT nIndex;
	for( INT i = nSelectCnt - 1 ; i > -1 ; i-- )
	{
		nIndex = pGrid->SelectGetIdx();

		pDM->DeleteRow(nIndex);

		pGrid->SelectGetPrev();
	}

	pGrid->Display();

	return 0;
}

HBRUSH CMNG_CAT_MANAGER_04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
