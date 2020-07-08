// MNG_MAIL_MANAGER_02.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MNG_MAIL_MANAGER_02.h"
#include "MNG_MAIL_MANAGER_01.h"
#include "MNGMAILMANAGER04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_MAIL_MANAGER_02 dialog


CMNG_MAIL_MANAGER_02::CMNG_MAIL_MANAGER_02(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_MAIL_MANAGER_02)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	AfxInitRichEdit();
}

CMNG_MAIL_MANAGER_02::~CMNG_MAIL_MANAGER_02()
{
	
}

BOOL CMNG_MAIL_MANAGER_02::Create(CWnd* pParentWnd)
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

	EFS_END
	return FALSE;

}	

VOID CMNG_MAIL_MANAGER_02::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_MAIL_MANAGER_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMNG_MAIL_MANAGER_02, CDialog)
	//{{AFX_MSG_MAP(CMNG_MAIL_MANAGER_02)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_CBN_KILLFOCUS(IDC_cmbBOX2, OnKillfocuscmbBOX2)
	ON_CBN_SELCHANGE(IDC_cmbBOX1, OnSelchangecmbBOX1)
	ON_BN_CLICKED(IDC_btnMAIL_PREV, OnbtnMAILPREV)
	ON_BN_CLICKED(IDC_btnMAIL_NEXT, OnbtnMAILNEXT)
	ON_BN_CLICKED(IDC_btnMAIL_SAVE, OnbtnMAILSAVE)
	ON_BN_CLICKED(IDC_btnMAIL_CLOSE, OnbtnMAILCLOSE)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnMAIL_HELP, OnbtnMAILHELP)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_MAIL_MANAGER_02 message handlers


BOOL CMNG_MAIL_MANAGER_02::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if( InitESL_Mgr( _T("MNG_MAIL_MANAGER_02") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_MAIL_MANAGER_02") );
		return false;
	}

	InitSetControls();
	m_checkedDMRow = NULL;

	CESL_Grid *pGrid = ( CESL_Grid * )FindControl( _T("CM_MNG_MAIL_MANAGER_01"), _T("그리드") );
	INT nTmpStartPos = pGrid->GetIdx();
	m_index = nTmpStartPos;
	pGrid->SelectMakeList();


	INT nIndex;
	if( pGrid->SelectGetCount() > 0 )
	{
		pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < pGrid->SelectGetCount() ; i++ )
		{
			nIndex = pGrid->SelectGetIdx();
			if( nTmpStartPos == nIndex )
				m_index = i;

			pGrid->SelectGetNext();
		}

	}


	CRect WndRect;
	GetWindowRect( &WndRect );
	MoveWindow( WndRect.left , WndRect.top , WndRect.Width() - 360 , WndRect.Height() );

	
	if( m_mode == 1 )
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbBOX2);
		pCombo->ShowDropDown(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMNG_MAIL_MANAGER_02::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	if( 1 == m_mode )
		return;

	CMNG_MAIL_MANAGER_01 *pParent = ( CMNG_MAIL_MANAGER_01 * )pParentMgr;
	m_checkedDMRow = pParent->GetCheckedDMRow( m_checkedRowCnt );
	
	if( m_checkedRowCnt < 1 )
	{
		AfxMessageBox( _T("선택된 목록이 없습니다.") );
		PostMessage( WM_CLOSE );
	}
	else
	{
		ShowModifyData();

		CWnd *pWnd;
		UINT wndID[ 3 ] = { IDC_cmbBOX1, IDC_cmbBOX2, IDC_edtEDT1 };
		for( INT i = 0; i < 3; i++ )
		{
			pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
			pWnd->EnableWindow( false );
		}
	}

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::InitSetControls()
{
	EFS_BEGIN

	CButton *pBtn;
	switch( m_mode )
	{
		case 1 :
			pBtn = ( CButton * )GetDlgItem( IDC_btnMAIL_PREV );
			pBtn->ShowWindow( false );
			pBtn = ( CButton * )GetDlgItem( IDC_btnMAIL_NEXT );
			pBtn->ShowWindow( false );
			SetWindowText( _T("입 력") );
			break;
		case 2 :
			SetWindowText( _T("수 정") );
			break;
		default :
			break;
	}

	CComboBox *pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX1 );
	pCombo->SetCurSel( 2 );

	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX3 );
	pCombo->InsertString( 0, _T("☞ 선택") );
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_USER") );
	CString id;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("ID"), i, id );
		pCombo->InsertString( i + 1, id );
	}
	
	/////////////////////////////////////////////////////////////////////
	// 2005.10.11 ADD BY PDW(Jr)
	// 기본 메일 타입이외의 타입을 설정한다.
	/////////////////////////////////////////////////////////////////////
	// 1. 메일 타입 DM을 초기화 한다.
	/////////////////////////////////////////////////////////////////////
	CESL_DataMgr *pMailDM = FindDM( _T("DM_BO_LOC_3800_MailGroup") );
	if (pMailDM == NULL)
	{
		CString error;
		error.Format(_T(" CMNG_MAIL_MANAGER_03::InitSetControls() \r\n'")
					 _T(" FindDM( DM_BO_LOC_3820_MailGroup ) :: error"));
		AfxMessageBox(error);
		return;
	}
	
	pMailDM->RefreshDataManager( _T("") );
	
	INT ids=0, cnt=0;
	
	cnt = pMailDM->GetRowCount();
	if (cnt < 1)
	{
		AfxMessageBox(_T("메일 그룹정보가 없습니다"));
		return;
	}

	/////////////////////////////////////////////////////////////////////
	// 2. 콤보박스에 없는 DM 정보 추가
	/////////////////////////////////////////////////////////////////////
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX1 );
	INT j = 0;
	for(i = 0;i < cnt;i++)
	{
		CString sTemp,sTemp2;
		pMailDM->GetCellData(_T("메일타입"), i, sTemp);
		
		INT row = pCombo->GetCount();
		for (j = 0;j < row;j++)
		{
			pCombo->GetLBText(j,sTemp2);
			if (sTemp == sTemp2)
				break;
		}

		if (j == row)
			pCombo->InsertString(row, sTemp);
	}

	pCombo->SetCurSel( 0 );

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::ViewDocFormatNum( CString mngName )
{
	EFS_BEGIN

	CString where;
	where.Format( _T(" G.M_TYPE = '%s' AND G.MNG_GROUP = '%s' "), GetWorkGroup(), mngName );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_02_docFormat") );
	pDM->RefreshDataManager( where );

	CString num, docFormat;
	INT rowCnt = pDM->GetRowCount();
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("M_FORMAT"), i, num );

		if( _ttoi( num ) != i + 1 )
		{
			docFormat.Format( _T("%d"), i + 1 );
			break;
		}
	}

	if( i == rowCnt )
		docFormat.Format( _T("%d"), i + 1 );

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_edtEDT1 );
	pEdt->SetWindowText( docFormat );

	EFS_END
}

CString CMNG_MAIL_MANAGER_02::GetMngName()
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX2 );
	CString mngName;
	pCombo->GetWindowText( mngName );
	mngName.TrimLeft(); mngName.TrimRight();

	return mngName;

	EFS_END
	return _T("");

}

VOID CMNG_MAIL_MANAGER_02::ViewMngList( CString group )
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX2 );
	pCombo->ResetContent();

	CString where;
	where.Format( _T(" M_TYPE = '%s' "), group );
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_02_group") );
	pDM->RefreshDataManager( where );

	INT rowCnt = pDM->GetRowCount();
	CString list;
	pCombo->InsertString( 0, _T("") );
	for( INT i = 0; i < rowCnt; i++ )
	{
		pDM->GetCellData( _T("MNG_GROUP"), i, list );
		pCombo->InsertString( i + 1, list );
	}
	pCombo->SetCurSel( 0 );

	EFS_END
}

CString CMNG_MAIL_MANAGER_02::GetWorkGroup()
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX1 );

	/////////////////////////////////////////////////////////////
	// 2005.10.11 UPD BY PDW(Jr)
	// 콤보박스의 내용을 리턴하도록 변경
	/////////////////////////////////////////////////////////////
	// 수정전
	/*
	switch( pCombo->GetCurSel() )
	{
		case 1 :
			return _T("수서");
			break;
		case 2 :
			return _T("열람");
			break;
		case 3 :
			return _T("정리");
			break;
		default :
			return _T("");
			break;
	}
	*/
	/////////////////////////////////////////////////////////////
	// 수정후
	CString sTemp;
	pCombo->GetWindowText(sTemp);
	
	if (sTemp == _T("☞ 선택"))
		return _T("");
	else
		return sTemp;

	EFS_END
	return _T("");

}

// DM_MNG_MAIL_MANAGER_02_docFormat
// DM_MNG_MAIL_MANAGER_02_group

INT CMNG_MAIL_MANAGER_02::GetMailInfo( CString data[] )
{
	EFS_BEGIN

	data[ 0 ] = GetWorkGroup();
	if( !data[ 0 ].GetLength() )
	{
		AfxMessageBox( _T("업무 구분을 선택하여 주십시오") );
		return -1;
	}
	
	data[ 1 ] = GetMngName();
	if( !data[ 1 ].GetLength() )
	{
		AfxMessageBox( _T("메일 종류를 입력해 주십시오") );
		return -1;
	}
/*
	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_edtEDT1 );
	pEdt->GetWindowText( data[ 2 ] );
	if( !data[ 2 ].GetLength() )
	{
		AfxMessageBox( _T("양식 번호를 입력하여 주십시오") );
		return -1;
	}
*/	
	CEdit* pEdt = ( CEdit * )GetDlgItem( IDC_edtEDT2 );
	pEdt->GetWindowText( data[ 3 ] );
	if( !data[ 3 ].GetLength() )
	{
		AfxMessageBox( _T("제목을 입력하여 주십시오") );
		return -1;
	}

	// 멀릿말
	GetDlgItem( IDC_edtEDT3 )->GetWindowText( data[ 5 ] );

	// 꼬릿말
	GetDlgItem( IDC_edtEDT5 )->GetWindowText( data[ 6 ] );

	// 코드
	GetDlgItem( IDC_edtEDT6 )->GetWindowText( data[ 7 ] );

	// 입력일 경우 메일 코드를 새로 생성한다.
	if( m_mode == 1 )
	{
		CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );
		pDM->GetOneValueQuery( _T("SELECT TO_NUMBER(MAIL_CLASS) FROM MN_MAIL_TBL WHERE TO_NUMBER(MAIL_CLASS) BETWEEN 30 AND 60") , data[7] );

		if( data[7].IsEmpty() ) data[7] = _T("30");
		data[7].Format( _T("%d") , _ttoi(data[7])+1 );
	}


	
	// 메일 통합 조건
	GetDlgItem( IDC_edtEDT7 )->GetWindowText( data[ 8 ] );

	// 기본 SQL문 WHERE절
	GetDlgItem( IDC_edtEDT13 )->GetWindowText( data[ 14 ] );


	// 일괄 처리 

	// 사용 여부
	GetDlgItem( IDC_edtEDT8 )->GetWindowText( data[ 9 ] );

	// 재발송 여부
	GetDlgItem( IDC_edtEDT9 )->GetWindowText( data[ 10 ] );

	// 조건앞설명
	GetDlgItem( IDC_edtEDT10 )->GetWindowText( data[ 11 ] );
	
	// 조건
	GetDlgItem( IDC_edtEDT11 )->GetWindowText( data[ 12 ] );

	// 조건뒤설명
	GetDlgItem( IDC_edtEDT12 )->GetWindowText( data[ 13 ] );

	return 0;

	EFS_END
	return -1;

}

INT CMNG_MAIL_MANAGER_02::ErrorCheck(CString data[])
{
	EFS_BEGIN

	CString query, result;
	// 2009.11.30 UPDATE BY KSJ : 검색 조건에 관리구분 추가
	//DEL query.Format( _T("	SELECT COUNT( * ) FROM MN_MAIL_GROUP_TBL G, MN_MAIL_TBL M WHERE G.MNG_GROUP = '%s' AND M.GROUP_KEY = G.REC_KEY"), data[ 1 ] );
	query.Format( _T("	SELECT COUNT( * ) FROM MN_MAIL_GROUP_TBL G, MN_MAIL_TBL M WHERE G.MNG_GROUP = '%s' AND M.GROUP_KEY = G.REC_KEY AND G.MANAGE_CODE=UDF_MANAGE_CODE"), data[ 1 ] );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_02_docFormat") );
	pDM->GetOneValueQuery( query, result );
	
	return _ttoi( result );

	EFS_END
	return -1;

}

INT CMNG_MAIL_MANAGER_02::SaveData( CString data[] )
{
	EFS_BEGIN

	CString groupKey;
	if( 1 == m_mode )
		groupKey = SaveMailGroup( data );
	else
		groupKey = GetCurrentGroupKey();
	
	const INT fieldCnt = 19;
	CString field[ fieldCnt ] = {
		_T("REC_KEY"), _T("GROUP_KEY"), _T("FORMAT"), _T("TITLE"), _T("CONTENTS"), _T("USER_KEY"), _T("REMARC"), _T("FIRST_WORK"), _T("LAST_WORK") , 
		_T("CONTENTS_HEADER") , _T("CONTENTS_TAIL") , _T("MAIL_CLASS") , _T("MAIL_SUM_CONDITION") , 
		_T("BATCH_PROCESSING_YN") , _T("BATCH_CONDITION_RE_SEND_YN") , _T("BATCH_CONDITION_HEAD_DESC"), _T("BATCH_CONDITION_VALUE") , _T("BATCH_CONDITION_TAIL_DESC") ,
		_T("DEFAULT_WHERE_SQL")
	};
	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") ,
		_T("STRING") , _T("STRING") , _T("NUMERIC") , _T("STRING") , 
		_T("STRING") , _T("STRING")  ,_T("STRING")  , _T("STRING") , _T("STRING") ,
		_T("STRING")

	};
	CString fieldData[ fieldCnt ];

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_02_docFormat") );

	if( 1 == m_mode )
		pDM->MakeRecKey( fieldData[ 0 ] );
	else
		fieldData[ 0 ] = GetCurrentMailKey();
	
	fieldData[ 1 ] = groupKey;
	fieldData[ 2 ] = _T("1");
	fieldData[ 3 ] = data[ 3 ];
	fieldData[ 4 ] = GetMailContents();
	fieldData[ 5 ] = data[ 4 ];
	fieldData[5] = GetUserKey();
	//fieldData[ 6 + m_mode ] = GetWorkLogMsg();
	fieldData[ 6 + m_mode ] = GetWorkLogMsg( _T("메일관리"), __WFILE__, __LINE__ );
	fieldData[9] = data[5];
	fieldData[10] = data[6];
	fieldData[11] = data[7];
	fieldData[12] = data[8];
	fieldData[13] = data[9];
	fieldData[14] = data[10];
	fieldData[15] = data[11];
	fieldData[16] = data[12];
	fieldData[17] = data[13];
	fieldData[18] = data[14];

	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
	{
		if( field[i].Compare( _T("USER_KEY") ) == 0 )
			continue;
//		fieldData[i].Replace( _T("'") , _T("''") );
		if( fieldData[ i ].GetLength() )
			pDM->AddDBFieldData( field[ i ], fieldType[ i ], fieldData[ i ] , TRUE );
	}

	if( 1 == m_mode )
		pDM->MakeInsertFrame( _T("MN_MAIL_TBL") );
	else if( 2 == m_mode )
		pDM->MakeUpdateFrame( _T("MN_MAIL_TBL"), _T("REC_KEY"), _T("NUMERIC"), fieldData[ 0 ] );
	
	
	AddDataToDM( data, fieldData, groupKey );
	INT ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveData") );
	pDM->EndFrame();
	
	return 0;

	EFS_END
	return -1;

}

CString CMNG_MAIL_MANAGER_02::SaveMailGroup( CString data[] )
{
	EFS_BEGIN

	CString pk;

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_02_docFormat") );
	pDM->MakeRecKey( pk );

	const INT fieldCnt = 3;
	CString fieldName[ fieldCnt ] = {
		_T("REC_KEY"), _T("M_TYPE"), _T("MNG_GROUP") };
	CString fieldType[ fieldCnt ] = {
		_T("NUMERIC"), _T("STRING"), _T("STRING") };
		
	pDM->StartFrame();
	pDM->InitDBFieldData();

	for( INT i = 0; i < fieldCnt; i++ )
		if( i )
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], data[ i - 1 ] );
		else
			pDM->AddDBFieldData( fieldName[ i ], fieldType[ i ], pk );
	pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("메일관리"), __WFILE__, __LINE__ ), TRUE );
	
	pDM->MakeInsertFrame( _T("MN_MAIL_GROUP_TBL") );

	pDM->SendFrame();
	pDM->EndFrame();

	return pk;

	EFS_END
	return _T("");

}

CString CMNG_MAIL_MANAGER_02::GetMailContents()
{
	EFS_BEGIN

	CString contents;
	CRichEditCtrl *pRichEdt = ( CRichEditCtrl * )GetDlgItem( IDC_edtEDT4 );

	pRichEdt->GetWindowText( contents );

	return contents;

	EFS_END
	return _T("");

}

VOID CMNG_MAIL_MANAGER_02::AddDataToDM( CString ctrlData[], CString mailData[], CString groupKey, INT option )
{
	EFS_BEGIN

	const INT fieldCnt = 20;
	CString alias[ fieldCnt ] = {
		_T("G_REC_KEY"), _T("G_M_TYPE"), _T("G_MNG_GROUP"), _T("M_REC_KEY"), _T("M_GROUP_KEY"), _T("M_FORMAT"), _T("M_TITLE"), _T("M_USER_KEY"), _T("U_ID") ,
		_T("M_CONTENTS") , _T("M_CONTENTS_HEADER") , _T("M_CONTENTS_TAIL") , _T("M_MAIL_CLASS") , _T("M_MAIL_SUM_CONDITION") ,
		_T("사용여부") , _T("재발송여부") , _T("조건앞설명") , _T("조건") , _T("조건뒤설명") , _T("기본SQL")

	};
	CString data[ fieldCnt ];
	data[ 0 ] = groupKey;
	data[ 1 ] = ctrlData[ 0 ];
	data[ 2 ] = ctrlData[ 1 ];
	data[ 3 ] = mailData[ 0 ];
	data[ 4 ] = groupKey;
	data[ 5 ] = ctrlData[ 2 ];
	data[ 6 ] = ctrlData[ 3 ];
	data[ 7 ] = ctrlData[ 4 ];
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX3 );
	pCombo->GetWindowText( data[ 8 ] );

	data[9] = mailData[4];
	data[10] = mailData[9];
	data[11] = mailData[10];
	data[12] = mailData[11];
	data[13] = mailData[12];
	data[14] = mailData[13];
	data[15] = mailData[14];
	data[16] = mailData[15];
	data[17] = mailData[16];
	data[18] = mailData[17];
	data[19] = mailData[18];

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );
	
	if( 1 == m_mode )
		pDM->SetCellData( alias, fieldCnt, data, -1 );
	else if( 2 == m_mode )
		pDM->SetCellData( alias, fieldCnt, data, m_checkedDMRow[ m_index ] );

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::OnClose() 
{
	EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	
	FreeCheckedInfo();
	
	CDialog::OnClose();

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::FreeCheckedInfo()
{
	EFS_BEGIN

	if( m_checkedRowCnt )
	{
		m_checkedRowCnt = 0;
		free( m_checkedDMRow );
		m_checkedDMRow = NULL;
	}

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::ShowModifyData()
{
	EFS_BEGIN

	const INT ctrlCnt = 16;
	CString data[ ctrlCnt ];
	GetCtrlDataFromDM( data );

	SetMailType( data[ 0 ] );
	SetMailGroup( data[ 0 ], data[ 1 ] );

	CEdit *pEdt;
	pEdt = ( CEdit * )GetDlgItem( IDC_edtEDT1 );
	pEdt->SetWindowText( data[ 2 ] );
	pEdt = ( CEdit * )GetDlgItem( IDC_edtEDT2 );
	pEdt->SetWindowText( data[ 3 ] );

	SetMailSender( data[ 4 ] );

	CRichEditCtrl *pRichEdt = ( CRichEditCtrl * )GetDlgItem( IDC_edtEDT4 );
	pRichEdt->SetWindowText( data[ 5 ] );

	pRichEdt = ( CRichEditCtrl * )GetDlgItem( IDC_edtEDT3 );	
	pRichEdt->SetWindowText( data[ 6 ] );

	pRichEdt = ( CRichEditCtrl * )GetDlgItem( IDC_edtEDT5 );	
	pRichEdt->SetWindowText( data[ 7 ] );

	GetDlgItem( IDC_edtEDT6 )->SetWindowText( data[ 8 ] );

	GetDlgItem( IDC_edtEDT7 )->SetWindowText( data[ 9 ] );

	GetDlgItem( IDC_edtEDT8 )->SetWindowText( data[ 10 ] );
	GetDlgItem( IDC_edtEDT9 )->SetWindowText( data[ 11 ] );
	GetDlgItem( IDC_edtEDT10 )->SetWindowText( data[ 12 ] );
	GetDlgItem( IDC_edtEDT11 )->SetWindowText( data[ 13 ] );
	GetDlgItem( IDC_edtEDT12 )->SetWindowText( data[ 14 ] );
	GetDlgItem( IDC_edtEDT13 )->SetWindowText( data[ 15 ] );

	// 부모 그리드 SetReverse
	CESL_ControlMgr *pParentCM = FindCM(_T("CM_MNG_MAIL_MANAGER_01"));
	CESL_Grid *pGrid = (CESL_Grid*)pParentCM->FindControl( _T("그리드") );

	pGrid->SetReverse(m_checkedDMRow[m_index]);


	EFS_END
}


VOID CMNG_MAIL_MANAGER_02::GetCtrlDataFromDM( CString data[] )
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );
	const INT fieldCnt = 16;
	CString alias[ fieldCnt ] = 
	{
		_T("G_M_TYPE"), _T("G_MNG_GROUP"), _T("M_FORMAT"), _T("M_TITLE"), _T("U_ID"), _T("M_CONTENTS"),_T("M_CONTENTS_HEADER") , _T("M_CONTENTS_TAIL") , _T("M_MAIL_CLASS") , _T("M_MAIL_SUM_CONDITION") ,
		_T("사용여부") , _T("재발송여부") , _T("조건앞설명") , _T("조건") , _T("조건뒤설명") , _T("기본SQL")
	};

	pDM->GetCellData( alias, fieldCnt, data, m_checkedDMRow[ m_index ] );

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::SetMailType( CString type )
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX1  );

	////////////////////////////////////////////////////////////////
	// 2005.10.11 UPD BY PDW(Jr)
	// type를 콤보박스 내용과 비교후 설정
	////////////////////////////////////////////////////////////////
	// 수정전
	/*
	if( _T("수서") == type )
		pCombo->SetCurSel( 1 );
	else if( _T("열람") == type )
		pCombo->SetCurSel( 2 );
	else if( _T("정리") == type )
		pCombo->SetCurSel( 3 );
	*/
	////////////////////////////////////////////////////////////////
	// 수정후 
	if ( _T("☞ 선택") != type)
	{
		INT cnt = pCombo->GetCount();
		CString sTemp;

		for(INT i = 0;i <cnt;i++)
		{
			pCombo->GetLBText(i, sTemp);
			if (sTemp == type)
			{
				pCombo->SetCurSel(i);
				break;
			}
		}
	}

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::SetMailGroup( CString type, CString group )
{
	EFS_BEGIN

	ViewMngList( type );
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX2 );
	INT listCnt = pCombo->GetCount();

	CString g;
	for( INT i = 0; i < listCnt; i++ )
	{
		pCombo->SetCurSel( i );
		pCombo->GetWindowText( g );

		if( g == group )
			break;
	}

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::SetMailSender( CString id )
{
	EFS_BEGIN

	/*
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX3 );	
	INT listCnt = pCombo->GetCount();

	CString ID;
	for( INT i = 0; i < listCnt; i++ )
	{
		pCombo->SetCurSel( i );
		pCombo->GetWindowText( ID );

		if( ID == id )
			break;
	}
	
	if( i == listCnt )
	{
		AfxMessageBox( _T("기존 발신자가 시스템에서 삭제되었습니다. 다시 선택하여 주십시오") );
		pCombo->SetCurSel( 0 );
	}
	*/

	EFS_END
}

CString CMNG_MAIL_MANAGER_02::GetCurrentMailKey()
{
	EFS_BEGIN

	CString pk;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );
	pDM->GetCellData( _T("M_REC_KEY"), m_checkedDMRow[ m_index ], pk );

	return pk;

	EFS_END
	return _T("");

}

CString CMNG_MAIL_MANAGER_02::GetCurrentGroupKey()
{
	EFS_BEGIN

	CString groupKey;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_MAIL_MANAGER_01") );
	pDM->GetCellData( _T("M_GROUP_KEY"), m_checkedDMRow[ m_index ], groupKey );

	return groupKey;

	EFS_END
	return _T("");

}


VOID CMNG_MAIL_MANAGER_02::OnKillfocuscmbBOX2() 
{
	EFS_BEGIN

	if( 2 == m_mode )
		return;
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_edtEDT1 );
	
	CString mngName = GetMngName();
	if( !mngName.GetLength() )
	{
		AfxMessageBox( _T("메일 종류를 입력하여 주십시오") );
		pEdt->SetWindowText( _T("") );
	}
	else
		ViewDocFormatNum( mngName );
	
//	pEdt->SetFocus();

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::OnSelchangecmbBOX1() 
{
	EFS_BEGIN

	CString group = GetWorkGroup();
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbBOX2 );
	if( !group.GetLength() )
	{
		AfxMessageBox( _T("업무 구분을 선택하여 주십시오") );
		pCombo->ResetContent();
	}
	else
		ViewMngList( group );

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::OnbtnMAILPREV() 
{
	EFS_BEGIN

	if( !m_index )
		AfxMessageBox( _T("맨 처음 목록입니다.") );
	else
	{
		m_index--;
		ShowModifyData();
	}

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::OnbtnMAILNEXT() 
{
	EFS_BEGIN

	if( m_checkedRowCnt -1 == m_index )
		AfxMessageBox( _T("맨 마지막 목록입니다.") );
	else
	{
		m_index++;
		ShowModifyData();
	}	

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::OnbtnMAILSAVE() 
{
	EFS_BEGIN

	const INT ctrlCnt = 15;
	CString data[ ctrlCnt ];
	if( GetMailInfo( data ) )
		return;
	
	if( 1 == m_mode )
		if( ErrorCheck( data ) )
		{
			AfxMessageBox( _T("메일종류가 중복됩니다.") );
			return;
		}
	
	if( SaveData( data ) )
	{
		AfxMessageBox( _T("Error CMNG_MAIL_MANAGER_02::SaveData( CString data[] )") );
		return;
	}

	CMNG_MAIL_MANAGER_01 *pParent = ( CMNG_MAIL_MANAGER_01 * )pParentMgr;

	if( m_mode == 1 )
		pParent->ShowGrid();
	else
		pParent->ShowGrid(m_checkedDMRow[ m_index ]);

	if( 1 == m_mode )
	{
		AfxMessageBox( _T("저장되었습니다.") );
	}
	else
		AfxMessageBox( _T("수정되었습니다.") );

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::OnbtnMAILCLOSE() 
{
	EFS_BEGIN

	OnCancel();

	EFS_END
}

CString CMNG_MAIL_MANAGER_02::GetUserKey()
{
	EFS_BEGIN

	CESL_DataMgr *pUserDM = FindDM(_T("DM_MNG_MAIL_MANAGER_USER"));
	
	CString sUserKey;
	CString sTmpUserID;
	CString sUserID;

//	((CComboBox*)GetDlgItem(IDC_cmbBOX3))->GetLBText(((CComboBox*)GetDlgItem(IDC_cmbBOX3))->GetCurSel() , sUserID );

	GetDlgItemText( IDC_cmbBOX3, sUserID );

	for( INT i = 0 ; i < pUserDM->GetRowCount() ; i++ )
	{
		pUserDM->GetCellData( _T("ID") , i , sTmpUserID );

		if( sTmpUserID.Compare(sUserID) == 0 )
		{
			pUserDM->GetCellData( _T("PK") , i , sUserKey );
			return sUserKey;
		}
	}

	return sUserKey;

	EFS_END
	return _T("");

}

VOID CMNG_MAIL_MANAGER_02::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);	

	EFS_END
}

VOID CMNG_MAIL_MANAGER_02::OnbtnMAILHELP() 
{
	EFS_BEGIN

	CMngMailManager04 HelpDlg(this);

	///////////////////////////////////////////////////
	// 2005.10.11 ADD BY PDW(Jr)
	// 콤보박스의 메일 그룹 값 설정
	///////////////////////////////////////////////////
	CString sTemp;
	GetDlgItemText(IDC_cmbBOX1, sTemp);
	HelpDlg.SetMailGroup(sTemp);
	HelpDlg.DoModal();

	EFS_END
}

HBRUSH CMNG_MAIL_MANAGER_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
