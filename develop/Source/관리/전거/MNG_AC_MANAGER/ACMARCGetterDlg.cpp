// ACMARCGetterDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "ACMARCGetterDlg.h"
#include "..\..\..\관리\전거\MNG_AC_MANAGER\MNG_AC_MANAGER_02.h"
#include "..\..\..\관리\전거\MNG_AC_MANAGER\MNG_AC_MANAGER_03.h"
#include "..\..\..\관리\전거\MNG_AC_MANAGER\MNG_AC_MANAGER_04.h"


#include "MarcExportDlg.h"
#include "MarcImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CACMARCGetterDlg dialog


CACMARCGetterDlg::CACMARCGetterDlg( CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CACMARCGetterDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_marc = NULL;
	m_marcMgr = NULL;
	//}}AFX_DATA_INIT
}

CACMARCGetterDlg::~CACMARCGetterDlg()
{
/*	if( m_marcMgr )
	{
		delete m_marcMgr;
		m_marcMgr = NULL;
	}

	if( m_marc )
	{
		delete m_marc;
		m_marc = NULL;
	}
*/
}

BOOL CACMARCGetterDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CACMARCGetterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACMARCGetterDlg)
	DDX_Control(pDX, IDC_BOOK_AC_MARK, m_bookACMarc);
	DDX_Control(pDX, IDC_BOOL_FLAG, m_bookTag);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CACMARCGetterDlg, CDialog)
	//{{AFX_MSG_MAP(CACMARCGetterDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_bFIND, OnbFIND)
	ON_BN_CLICKED(IDC_bDETAIL_VIEW, OnbDETAILVIEW)
	ON_BN_CLICKED(IDC_bInput, OnbInput)
	ON_BN_CLICKED(IDC_bUpdate, OnbUpdate)
	ON_BN_CLICKED(IDC_bDelete, OnbDelete)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACMARCGetterDlg message handlers

BOOL CACMARCGetterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initia lization here
	
	if( InitESL_Mgr( _T("ACMARCGetterDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : ACMARCGetterDlg") );
		return false;
	}

	//수정, 삭제, 상세보기 버튼 비활성화
	GetDlgItem(IDC_bUpdate)->EnableWindow(false);
	GetDlgItem(IDC_bDelete)->EnableWindow(false);
	GetDlgItem(IDC_bDETAIL_VIEW)->EnableWindow(false);

	//윈도우 타이틀바 설정
	CString sSetWindowText = _T("");
	if ( m_ACType == 1) 
		sSetWindowText = _T("저자 전거통제");
	if ( m_ACType == 2) 
		sSetWindowText = _T("서명 전거통제");
	if ( m_ACType == 3) 
		sSetWindowText = _T("주제어 전거통제");

	SetWindowText(sSetWindowText);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CACMARCGetterDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_ACEdit );
	pEdt->SetWindowText( m_ACData );

	EnableCombo( false );

	// test
	//m_ACType = 1;
}

// 최초 서지정보를 셋팅한다. 
INT CACMARCGetterDlg::SetACInfo( CMarcMgr *marcMgr, CMarc *marc, INT ACType, CString acData )
{
	m_marcMgr = marcMgr;
	m_marc = marc;
	m_ACType = ACType;
	m_ACData = acData;
	return 0;
}

// long데이타를 DB에 update한다. 
INT CACMARCGetterDlg::UpdateLongData(CString tblNane, CString pkName, CString pkType, CString pk, CString fieldName, CString fieldType, CString data)
{
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	pDM->StartFrame();
	pDM->InitDBFieldData();

	pDM->AddDBFieldData( fieldName, fieldType, data );
	pDM->ORACLE_OCIMgr_MakeUpdateFrame( tblNane, pkName, pkType, pk );

	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;
}

// 확인
VOID CACMARCGetterDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	if( !m_ok )
	{
		AfxMessageBox( _T("채택표목을 선택하여 주십시오") );
		return;
	}
	
	CString tag, idx;
	GetSelectedBookInfo( tag, idx );
	if( !tag.GetLength() || !idx.GetLength() )
	{
		AfxMessageBox( _T("선택된 값에 오류가 있습니다.") );
		return;
	}

	if( AddACTagToBookMarc( tag, idx ) )
	{
		AfxMessageBox( _T("Error : AddACTagToBookMarc( CString tag, CString idx )") );
		return;
	}
	
	CDialog::OnOK();	
}

// 닫기
VOID CACMARCGetterDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel( );
}

// 전거조회 표목 검색
VOID CACMARCGetterDlg::OnbFIND()
{
	// TODO: Add your control notification handler code here

	CString keyWord = GetIndexText();
	keyWord.TrimLeft();
	keyWord.TrimRight();
	if( !keyWord.GetLength() )
	{
		AfxMessageBox( _T("전거조회표목을 입력하여 주십시오") );
		return;
	}

	SearchMasterMarcByKeyword( m_ACType, keyWord );
	EnableCombo( false );
}

// ACType으로 해당 키워드를 찾는다. 
INT CACMARCGetterDlg::SearchMasterMarcByKeyword( INT flag, CString keyWord )
{
	CString where;
	where.Format( _T("KEYWORD = '%s'"), keyWord );

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_ACMARCGetterDlg"));

	CESL_DataMgr *pDM;
	CString sTblAcType;	
	CString sTblName;
	switch( flag )
	{
		case 1 :
			pDM = FindDM( _T("DM_ACMARCGetterDlg_Author") );
			sTblAcType = _T("AC_TYPE IN ('0','1','2')");
			sTblName = _T("AC_INDEX_AUTHOR_TBL");
			break;
		case 2 :
			pDM = FindDM( _T("DM_ACMARCGetterDlg_Title") );
			sTblAcType = _T("AC_TYPE IN ('3')");
			sTblName = _T("AC_INDEX_TITLE_TBL");
			break;
		case 3 :
			pDM = FindDM( _T("DM_ACMARCGetterDlg_Subject") );
			sTblAcType = _T("AC_TYPE IN ('4')");
			sTblName = _T("AC_INDEX_SUBJECT_TBL");
			break;
		default :
			return -1;
			break;
	}

	CString strQuery;
	strQuery.Format ( _T(" SELECT REC_KEY, AC_MASTER_MAT_KEY, KEYWORD, FIRST_WORK, LAST_WORK, SELECT_YN FROM %s WHERE KEYWORD = '%s' "), sTblName, keyWord ) ;
	
	INT ids = pDM->RestructDataManager( strQuery );

	INT keyCnt = pDM->GetRowCount();

	if( !keyCnt )
	{
		AfxMessageBox( _T("검색 결과가 없습니다.") );
		return -1;
	}
	CString *ACKey = new CString[ keyCnt ];

	for( INT i = 0; i < keyCnt; i++ )
	{
		if ( flag != 3 ) 
			pDM->GetCellData( _T("AC_MASTER_MAT_KEY"), i, ACKey[ i ] );
		else
			pDM->GetCellData( i, 1, ACKey[ i ] );
	}
		

	SearchMasterACMarc( ACKey, keyCnt );

	delete [] ACKey;
	
	return 0;
}

// 주어진 keyword로 마스타마크를 찾는다. 
INT CACMARCGetterDlg::SearchMasterACMarc( CString *pk, INT cnt )
{
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	pDM->RefreshDataManager( MakeSelectQuery( pk, cnt ) );
	
	CString marc;
	CString sTmP;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData( _T("MARC"), i, marc );
		pDM->SetCellData( _T("채택표목"), GetIndexText(), i );
	}
	
	AllControlDisplay( _T("CM_ACMARCGetterDlg") );
	
	//ShowGrid();

	return 0;
}

CString CACMARCGetterDlg::MakeSelectQuery( CString *pk, INT cnt )
{
	CString where, tmp;
	for( INT i = 0; i < cnt; i++ )
		if( pk[ i ].GetLength() )
		{
			//tmp.Format( _T(" AC_CONTROL_NO = %s OR "), pk[ i ] );
			tmp.Format( _T(" REC_KEY = %s OR "), pk[ i ] );
			where += tmp;
		}

	if( _T("OR ") == where.Right( 3 ) )
	{
		tmp = where.Left( where.GetLength() - 3 );
		where = tmp;
	}

	where = where + _T("ORDER BY AC_CONTROL_NO");
	return where;
}

// 그리드에 채택표목에 대한 정보를 보여준다. 
VOID CACMARCGetterDlg::ShowGrid()
{
	ControlDisplay( _T("CM_ACMARCGetterDlg"), _T("그리드") );
	
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();
	
	CString marc, idx;

	pGrid->SetCol( 2 );
	INT idxxxxxx = 2;
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		
		pGrid->SetRow( i + 1 );

		pDM->GetCellData( _T("MARC"), i, marc );
		CString sACControlNo;
		pDM->GetCellData( _T("AC_CONTROL_NO"), i, sACControlNo );
		
		if( GetACIndex( marc, idx, i ) > 0 )
		{
			pGrid->SetText( idx );
			pGrid->SetTextMatrix( idxxxxxx , 2, idx );
			pGrid->SetTextMatrix( idxxxxxx , 3, sACControlNo );
			idxxxxxx++;

		}
	}	

	pGrid->SetRow( row );
	pGrid->SetCol( col );
}

CString CACMARCGetterDlg::GetIndexText()
{
	CString text;
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_ACEdit );
	pEdt->GetWindowText( text );

	return text;
}

BEGIN_EVENTSINK_MAP(CACMARCGetterDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CACMARCGetterDlg)
	ON_EVENT(CACMARCGetterDlg, IDC_mngGRID_1, -600 /* Click */, OnClickmngGRID1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CACMARCGetterDlg::OnClickmngGRID1() 
{
	// TODO: Add your control notification handler code here
	
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	if( !pDM->GetRowCount() )
		return;
	
	INT checkedRow = SetCheckGrid();

	if( checkedRow )
	{
		SetBookTagList( checkedRow - 1 );
		SetBookIdxList( checkedRow - 1 );
	}	
}

// marc에서 채택 표목을 추출한다.
INT CACMARCGetterDlg::GetACIndex( CString marc, CString &idx, INT dmRow )
{
	CMarc marcS;
	if( this->m_pInfo->pMarcMgr->Decoding( marc, &marcS ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return -1;
	}

	INT idxCnt = 0;
	switch( GetACTypeFromDM( dmRow ) )
	{
		case 0 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("100$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			else
				return -1;

			break;

		case 1 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("110$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			else
				return -1;

			break;
			
		case 2 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("111$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			return -1;
			
			break;

		case 3 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("130$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			else
				return -1;

			break;

		case 4 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("150$a"), idx );
			if( idxCnt > 0 )
				return idxCnt;
			else
				return -1;

			break;

		default :
			return -1;
			break;
	}

	return -1;
}

// 그리드의 선택row는 하나로 한다. 
INT CACMARCGetterDlg::SetCheckGrid()
{
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	
	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )
		return 0;

	if( _T("V") == pGrid->GetText() )
	{
		pGrid->SetText( _T("") );
		EnableCombo( false );
		m_dmRow = -1;
		return 0;
	}

	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		if( row == i )
		{
			pGrid->SetText( _T("V") );
			m_dmRow = i - 1;
		}
		else if( _T("V") == pGrid->GetText() )
			pGrid->SetText( _T("") );
	}

	pGrid->SetRow( row );
	EnableCombo( true );

	return row;
}

// 서지정보 콤보에 대한 UI
INT CACMARCGetterDlg::EnableCombo( bool flag )
{
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_BOOL_FLAG );
	pCombo->EnableWindow( flag );

	pCombo = ( CComboBox * )GetDlgItem( IDC_BOOK_AC_MARK );
	pCombo->EnableWindow( flag );
	
	CButton *pEdt = ( CButton * )GetDlgItem( IDOK );
	pEdt->EnableWindow( flag );
	
	GetDlgItem(IDC_bUpdate)->EnableWindow(flag);
	GetDlgItem(IDC_bDelete)->EnableWindow(flag);
	GetDlgItem(IDC_bDETAIL_VIEW)->EnableWindow(flag);

	m_ok = flag;
	
	return 0;
}

// 서지표시기호 콤보 리스트를 구성한다. 
INT CACMARCGetterDlg::SetBookTagList( INT dmRow )
{
	m_bookTag.ResetContent();
	
	switch( GetACTypeFromDM( dmRow ) )
	{
		case 0 :
			m_bookTag.InsertString( 0, _T("100") );
			m_bookTag.InsertString( 1, _T("700") );
			break;
		case 1 :
			m_bookTag.InsertString( 0, _T("110") );
			m_bookTag.InsertString( 1, _T("710") );
			break;
		case 2 :
			m_bookTag.InsertString( 0, _T("111") );
			m_bookTag.InsertString( 1, _T("711") );
			break;
		case 3 :
			m_bookTag.InsertString( 0, _T("130") );
			m_bookTag.InsertString( 1, _T("240") );
			m_bookTag.InsertString( 2, _T("730") );
			m_bookTag.InsertString( 3, _T("740") );
			m_bookTag.InsertString( 4, _T("830") );
			m_bookTag.InsertString( 5, _T("940") );
			break;
		case 4 :
			m_bookTag.InsertString( 0, _T("600") );
			m_bookTag.InsertString( 1, _T("610") );
			m_bookTag.InsertString( 2, _T("611") );
			m_bookTag.InsertString( 3, _T("630") );
			m_bookTag.InsertString( 4, _T("650") );
			m_bookTag.InsertString( 5, _T("651") );
			break;
	}
			
	return 0;
}

// 서지마크복사표목 콤보 리스트를 구성한다. 
INT CACMARCGetterDlg::SetBookIdxList( INT dmRow )
{
	m_bookACMarc.ResetContent();

	CString idx;
	INT num = 0;
	INT idxCnt = 0;

	CMarc marcS;
	if( m_pInfo->pMarcMgr->Decoding( GetMarcFromDM( dmRow ), &marcS ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return -1;
	}

	CArray < CString, CString& > idxArray;
	m_idxSignArray.RemoveAll();
	INT i;	

	switch( GetACTypeFromDM( dmRow ) )
	{
		case 0 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("100$a"), idx );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("400$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("500$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;

		case 1 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("110$a"), idx );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("410$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("510$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;
			
		case 2 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("111$a"), idx, &idxArray, &m_idxSignArray );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("411$a"), idx, &idxArray, &m_idxSignArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("511$a"), idx, &idxArray, &m_idxSignArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;

		case 3 :
			
			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("130$a"), idx );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("430$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("530$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;

		case 4 :

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("150$a"), idx );
			if( idxCnt > 0 )
				m_bookACMarc.InsertString( num++, idx );

			m_title450Cnt = idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("450$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			idxCnt = m_pInfo->pMarcMgr->GetItem( &marcS, _T("530$a"), idx, &idxArray );
			for( i = 0; i < idxCnt; i++ )
				m_bookACMarc.InsertString( num++, idxArray.GetAt( i ) );
			idxArray.RemoveAll();

			break;
	}
	
	return 0;
}

// 선택된 서지표시기호와 서지마크복사표목 값을 반환한다. 
VOID CACMARCGetterDlg::GetSelectedBookInfo( CString &tag, CString &idx )
{
	CComboBox *pCombo;

	pCombo = ( CComboBox * )GetDlgItem( IDC_BOOL_FLAG );
	pCombo->GetWindowText( tag );

	pCombo = ( CComboBox * )GetDlgItem( IDC_BOOK_AC_MARK );
	pCombo->GetWindowText( idx );
}

// 서지마크에 선택된 전거태그를 추가한다.  
INT CACMARCGetterDlg::AddACTagToBookMarc( CString tag, CString idx )
{
	// m_marcMgr, m_marc

	CString sign;
	switch( GetACTypeFromDM( m_dmRow ) )
	{
		case 0 :
			sign = _T("1 ");
			break;
		case 1 :
			sign = _T("  ");
			break;
		case 2 :
			sign = _T("  ");
			break;
		case 3 :
			sign = _T(" 0");
			break;
		case 4 :
			sign = GetTitleSign();
			break;
	}

	CString strSubfielda;
	strSubfielda.Format(_T("%ca"), SUBFIELD_CODE);
	m_marcMgr->InsertField( m_marc, tag + sign + strSubfielda + idx );

	return 0;
}

// 해당행의 marc를 가져온다.
CString CACMARCGetterDlg::GetMarcFromDM( INT dmRow )
{
	CString marc;
	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	pDM->GetCellData( _T("MARC"), dmRow, marc );

	return marc;
}


INT CACMARCGetterDlg::GetACTypeFromDM( INT dmRow )
{
	CString ACType;

	CESL_DataMgr *pDM = FindDM( m_sDmAlias );
	pDM->GetCellData( _T("AC_TYPE"), dmRow, ACType );
	
	return _ttoi( ACType );
}

VOID CACMARCGetterDlg::OnbDETAILVIEW() 
{
	// TODO: Add your control notification handler code here

	if( -1 == m_dmRow )
		return;

	CString strStreamMarc = GetMarcFromDM( m_dmRow );
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding( strStreamMarc, &marc );
	CShowACMarcDlg dlg( this );

	dlg.SetMarc( &marc );
	dlg.DoModal();
}

CString CACMARCGetterDlg::GetTitleSign()
{
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_BOOK_AC_MARK );

	return m_idxSignArray.GetAt( pCombo->GetCurSel() );
}

VOID CACMARCGetterDlg::OnbInput() 
{
EFS_BEGIN

	ACInput();
	return;
	
EFS_END
}

VOID CACMARCGetterDlg::OnbUpdate() 
{
EFS_BEGIN

	ACUpdate();
	return;

EFS_END
}

VOID CACMARCGetterDlg::OnbDelete() 
{
EFS_BEGIN

	//삭제
	ACDelete();
	
EFS_END
}

//==========================================================
// 전거 입력 : bMNGCALL 관리에서 콜하면 TRUE, 나머지는 FALSE
//==========================================================
VOID CACMARCGetterDlg::ACInput(BOOL bMNGCALL)
{
EFS_BEGIN

	// bMNGCALL 가 TRUE 이면 관리에서 호출
	// FALSE 이면 정리 Marc에서 호출

	//입력
	CMNG_AC_MANAGER_03 Dlg(this);

	//참조표목 입력에 참조표목 항목에 셋팅
	//관리 메뉴에서 호출할 경우 수행하지 않음
	if ( !bMNGCALL ) 
	{
		CString sTmpRefItem;
		GetDlgItem(IDC_ACEdit)->GetWindowText(sTmpRefItem);
		Dlg.SetRefItem(sTmpRefItem);
	}

	//전거통제 모드 설정
	if ( m_ACType == 1) 
		Dlg.SetACMode(AUTHOR_MODE);
	if ( m_ACType == 2) 
		Dlg.SetACMode(TITLE_MODE);
	if ( m_ACType == 3) 
		Dlg.SetACMode(SUBJECT_MODE);

	Dlg.DoModal();

	return;

EFS_END
}

//==========================================================
// 전거 수정
//==========================================================
VOID CACMARCGetterDlg::ACUpdate(BOOL bMNGCALL)
{
EFS_BEGIN

	SelectMakeList(m_sCmAlias, _T("그리드"));
	INT idx = SelectGetHeadPosition(m_sCmAlias, _T("그리드"));

	// bMNGCALL 가 TRUE 이면 관리에서 호출
	// FALSE 이면 정리 Marc에서 호출

	CString sTmpItem = _T("");
	if ( m_ACType != 1 && m_ACType != 2 && m_ACType != 3  )
	{
		m_ACType = 0;
		CESL_DataMgr *pDM = NULL;
		pDM = FindDM(m_sDmAlias);
		pDM->GetCellData(_T("AC_TYPE"), idx, sTmpItem);
	}
	
	if ( sTmpItem != _T("") )
	{
		if ( sTmpItem == _T("0") ||  sTmpItem == _T("1") || sTmpItem == _T("2") ) m_ACType = 1;
		else if ( sTmpItem == _T("3") ) m_ACType = 2;
		else if ( sTmpItem == _T("4")) m_ACType = 3;
	}

	if ( m_ACType == 0) return;

	//수정
	CMNG_AC_MANAGER_04 Dlg(this);
	//전거통제 모드 설정
	if ( m_ACType == 1) 
		Dlg.SetACMode(AUTHOR_MODE);
	if ( m_ACType == 2) 
		Dlg.SetACMode(TITLE_MODE);
	if ( m_ACType == 3) 
		Dlg.SetACMode(SUBJECT_MODE);

	//삭제해야 함
	//Dlg.SetACMode(AUTHOR_MODE);

	//선정된 항목의 키값 넘김
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_1 );
	INT checkedRow = pGrid->GetRow() -1 ;

	if( checkedRow )
	{
		if ( !bMNGCALL )
		{
			SetBookTagList( checkedRow - 1 );
			SetBookIdxList( checkedRow - 1 );
		}
	}


	
	CESL_DataMgr *pDM = NULL;
	pDM = FindDM( m_sDmAlias );
	if ( pDM == NULL ) return;

	CString sModifyRecKey;
	pDM->GetCellData(_T("REC_KEY"), idx, sModifyRecKey);

	Dlg.SetModifyRecKey(sModifyRecKey);	
	
	
	Dlg.DoModal();

EFS_END
}

VOID CACMARCGetterDlg::ACMarcExport()
{
EFS_BEGIN

	CMarcExportDlg Dlg( this );
	Dlg.DoModal();

	return;

EFS_END
}

VOID CACMARCGetterDlg::ACMarcImport()
{
EFS_BEGIN

	CMarcImportDlg Dlg( this );
	Dlg.DoModal();

	return;
	
EFS_END
}

VOID CACMARCGetterDlg::ACDelete()
{
EFS_BEGIN


	SelectMakeList(m_sCmAlias, _T("그리드"));
	INT idx = SelectGetHeadPosition(m_sCmAlias, _T("그리드"));

	CESL_DataMgr *pDM = NULL;
	pDM = FindDM( m_sDmAlias );
	if ( pDM == NULL ) return;

	CString sMasterRecKey;
	pDM->GetCellData(_T("REC_KEY"), idx, sMasterRecKey);

	CString sTblName;
	switch( m_ACType )
	{
	case 1 :
		sTblName = _T("AC_INDEX_AUTHOR_TBL");
		break;
	case 2 :
		sTblName = _T("AC_INDEX_TITLE_TBL");
		break;
	case 3 :
		sTblName = _T("AC_INDEX_SUBJECT_TBL");
		break;
	default :
		return;
		break;
	}

	CESL_DataMgr pDMTmp;

	pDMTmp.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	pDMTmp.StartFrame();
	
	CString strQuery;
	strQuery.Format(_T("DELETE %s WHERE AC_MASTER_MAT_KEY = %s; "), sTblName, sMasterRecKey );
	INT ids = pDMTmp.AddFrame(strQuery);
	if ( ids < 0 ) return;

	strQuery.Format(_T("DELETE AC_MASTER_MAT_TBL WHERE REC_KEY = %s; "), sMasterRecKey );
	ids = pDMTmp.AddFrame(strQuery);
	if ( ids < 0 ) return;

	ids = pDMTmp.SendFrame();
	if ( ids < 0 ) return;
	
	ids = pDMTmp.EndFrame();
	if ( ids < 0 ) return;


	
	AfxMessageBox(_T("삭제되었습니다."));
	
	AllControlDisplay(m_sCmAlias);
	return;

EFS_END
}

VOID CACMARCGetterDlg::ACSearch()
{
EFS_BEGIN
	
	CMNG_AC_MANAGER_02 Dlg(this);
	Dlg.DoModal();
	
EFS_END
}

HBRUSH CACMARCGetterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
