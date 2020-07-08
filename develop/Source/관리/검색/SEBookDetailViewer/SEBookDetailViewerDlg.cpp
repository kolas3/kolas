// SEBookDetailViewerDlg.cpp : implementation file

#include "stdafx.h"
#include "stdafx.h"
#include "SEBookDetailViewerDlg.h"
#include "SEBookMarcViewer.h"
#include "SEBookAppViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEBookDetailViewerDlg dialog

#define RECORD_TERMINATOR	29  //0x1d  양쪽방향 화살표
#define FIELD_TERMINATOR	30  //0x1e  삼각형
#define SUBFIELD_CODE		31  //0x1f  역삼각형

CSEBookDetailViewerDlg::CSEBookDetailViewerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSEBookDetailViewerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	AfxInitRichEdit();

	m_pkCnt = 0;
}
CSEBookDetailViewerDlg::~CSEBookDetailViewerDlg()
{
}

BOOL CSEBookDetailViewerDlg::Create(CWnd* pParentWnd)
{	
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

EFS_END
return FALSE;

}

VOID CSEBookDetailViewerDlg::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSEBookDetailViewerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CSEBookDetailViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CSEBookDetailViewerDlg)
	ON_WM_CTLCOLOR()    
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bMARC, OnbMARC)
	ON_BN_CLICKED(IDC_bAPPENDIX, OnbAPPENDIX)
	ON_BN_CLICKED(IDC_bBEFORE, OnbBEFORE)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEBookDetailViewerDlg message handlers

BOOL CSEBookDetailViewerDlg::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
		
	if( InitESL_Mgr( _T("MNG_BUSINESS_SEARCHER_03_SE") ) < 0 )
	{
		AfxMessageBox( _T("Error : InitESL_Mgr - MNG_BUSINESS_SEARCHER_03") );
		return false;
	}

	m_marcInfo = NULL;
	
	m_ViewerControl.InitViewer( IDC_mngGRID_2 , this );
	m_ViewerControl.SetViewerWidth( 20 , 77 );

	/// 이동도서관이라면 MARC보기 버튼을 보여주지 않는다.
	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
	{
		GetDlgItem(IDC_bMARC)->ShowWindow(SW_HIDE);
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CSEBookDetailViewerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	if( !m_pk.GetLength() )
	{
		PostMessage( WM_CLOSE );
		return;
	}
	else
		ShowBODetailView();

	CButton *pBtn;
	if( 1 == m_pkCnt )
	{
		pBtn = ( CButton * )GetDlgItem( IDC_bBEFORE );
		pBtn->ShowWindow( false );
		pBtn = ( CButton * )GetDlgItem( IDC_bNEXT );
		pBtn->ShowWindow( false );
	}
	

EFS_END
}

INT CSEBookDetailViewerDlg::ShowBODetailView()
{
EFS_BEGIN

	FreeAllInfo();

	CString where;
	where.Format( _T(" I.REC_KEY  = '%s' "), m_pk );

	CESL_DataMgr *pDM;
	pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_SE") );
	INT ids = pDM->RefreshDataManager( where );

	pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_SE_BOOK") );
	ids = pDM->RefreshDataManager( where );

	m_bookRow = -1;
	ShowControls();
	
	return 0;

EFS_END
return -1;

}

VOID CSEBookDetailViewerDlg::OnClose() 
{
EFS_BEGIN

	// TODO: Add your message handler code here and/or call default
	
	FreeAllInfo();
	CDialog::OnClose();

EFS_END
}

VOID CSEBookDetailViewerDlg::FreeAllInfo()
{
EFS_BEGIN

	INT cnt = AllInfo.GetCount();
	if( !cnt )
		return;

	POSITION pos = AllInfo.GetHeadPosition();
	MarcInfo *marcInfo;
	
	while( pos )
	{
		marcInfo = ( MarcInfo * )AllInfo.GetNext( pos );
		if( NULL == marcInfo )
			continue;
		
		if( marcInfo->tagInfo.cnt )
		{
			marcInfo->tagInfo.data.RemoveAll();
			marcInfo->tagInfo.subFieldCode.RemoveAll();
			marcInfo->tagInfo.cnt = 0;
		}

		delete marcInfo;
		marcInfo = NULL;
	}

	AllInfo.RemoveAll();

EFS_END
}

VOID CSEBookDetailViewerDlg::ShowControls()
{
EFS_BEGIN

	
	SetCodeToDesc();
	ControlDisplay( _T("CM_MNG_BUSINESS_SEARCHER_03_SE"), _T("그리드") );

	ViewMarcInfo();

EFS_END
}

VOID CSEBookDetailViewerDlg::ViewMarcInfo()
{
EFS_BEGIN
/*
	SetMarc();

	m_bookType = GetBookType();
	switch( m_bookType )
	{
		case 'a' :	// 단행
			ShowSEBookInfo();
			break;
		case 'w' :	// 고서
			ShowOldBookInfo();
			break;
		default :	// 비도서자료
			ShowNonBookInfo();
			break;
	}
*/
// 그냥 INDEX TABLE의 INFO 필드를 가져온다.
	
	DisplaySpecies();

EFS_END
}

VOID CSEBookDetailViewerDlg::SetMarc()
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_SE") );
	pDM->GetCellData( _T("S_MARC"), 0, m_marc );

EFS_END
}

TCHAR CSEBookDetailViewerDlg::GetBookType()
{
	CString leader;
	TCHAR type;

	if( m_marc.IsEmpty() )
		return ' ';

	if( m_pInfo->pMarcMgr->Decoding( m_marc, &m_marcS ) < 0 )
	{
		AfxMessageBox( _T("Error : Marc Decoding") );
		return NULL;
	}

	m_marcS.GetLeader( leader );
	wmemcpy( &type, &leader.Mid( 5, 1 ).GetBuffer( 0 )[ 0 ], 1);

	return type;
}

VOID CSEBookDetailViewerDlg::ShowSEBookInfo()
{
EFS_BEGIN

	if( m_marc.IsEmpty() ) return ;

	CString firseInd, secondInd, subFieldCod, ind, data;
	POSITION tagPos = m_marcS.m_listTag.GetHeadPosition();

	while( tagPos )
	{
		CTag *pTag = ( CTag * )m_marcS.m_listTag.GetNext( tagPos );
		if( NULL == pTag )
			continue;

		ind = pTag->GetFirstIndicator() + pTag->GetSecondIndicator();
		INT tag = _ttoi( pTag->GetTagCode() );
		switch( tag )
		{
			case 240 :
			case 245 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("표제/저자사항"), ind, tag );
				break;

			case 250 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("판사항"), ind, tag );
				break;

			case 255 :
			case 260 :
			case 300 :
			case 440 :
			case 490 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					if( 255 == tag )
						AddMarcInfo( _T("수치데이터사항"), ind, tag );
					else if( 260 == tag )
						AddMarcInfo( _T("발행사항"), ind, tag );
					else if( 300 == tag )
						AddMarcInfo( _T("형태사항"), ind, tag );
					else if( 440 == tag || 490 == tag )
						AddMarcInfo( _T("총서사항"), ind, tag );
				break;

			case 500 :
				if( !GetSubFieldInfo( pTag, _T("azbefghijkmrctlsdnpqt") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 780 :
				if( !GetSubFieldInfo( pTag, _T("agtxz") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 785 :
				if( !GetSubFieldInfo( pTag, _T("agtxz") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 580 :
			case 506 :
			case 521 :
			case 504 :
			case 510 :
			case 536 :
			case 586 :
			case 538 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 775 :
				if( !GetSubFieldInfo( pTag, _T("atx") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 767 :
			case 765 :
			case 507 :
				if( !GetSubFieldInfo( pTag, _T("tza") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;
			
			case 787 :
			case 770 :
			case 772 :
			case 773 :
			case 533 :
			case 534 :
			case 856 :
			case 776 :
			case 590 :
			case 505 :
			case 501 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 502 :
				if( !GetSubFieldInfo( pTag, _T("abcd") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;
			
			case 520 :
				if( !GetSubFieldInfo( pTag, _T("abc") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 22 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("ISSN"), ind, tag );
				break;

			case 20 :
				if( !GetSubFieldInfo( pTag, _T("ag") ) )
					AddMarcInfo( _T("ISBN"), ind, tag );
				break;

			case 56 :
			case 82 :
			case 85 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("분류기호"), ind, tag );

			default :
				continue;
				break;
		}
	}

	ShowMarcInfo();

EFS_END
}

VOID CSEBookDetailViewerDlg::ShowOldBookInfo()
{
EFS_BEGIN

	if( m_marc.IsEmpty() ) return ;

	CString firseInd, secondInd, subFieldCod, ind, data;
	POSITION tagPos = m_marcS.m_listTag.GetHeadPosition();

	while( tagPos )
	{
		CTag *pTag = ( CTag * )m_marcS.m_listTag.GetNext( tagPos );
		if( NULL == pTag )
			continue;

		ind = pTag->GetFirstIndicator() + pTag->GetSecondIndicator();
		INT tag = _ttoi( pTag->GetTagCode() );
		switch( tag )
		{
			case 240 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("표제/저자사항"), ind, tag );
				break;

			case 245 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("표제/저자사항"), ind, tag );
				break;

			case 250 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("판사항"), ind, tag );
				break;

			case 260 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("발행사항"), ind, tag );
				break;

			case 300 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("형태사항"), ind, tag );
				break;

			case 440 :
			case 490 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("총서사항"), ind, tag );
				break;

			case 780 :
			case 785 :
				if( !GetSubFieldInfo( pTag, _T("agt") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 507 :
			case 767 :
			case 765 :
			case 775 :
				if( !GetSubFieldInfo( pTag, _T("at") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 500 :
				if( !GetSubFieldInfo( pTag, _T("dnpqtazc") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;
	
			case 787 :
			case 770 :
			case 772 :
			case 773 :
			case 776 :
			case 590 :
			case 562 :
			case 505 :
			case 501 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 580 :
			case 506 :
			case 521 :
			case 504 :
			case 510 :
			case 586 :
			case 538 :
			case 536 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 520 :
				if( !GetSubFieldInfo( pTag, _T("abc") ) )
					AddMarcInfo( _T("주기사항"), ind, tag );
				break;

			case 56 :
			case 82 :
			case 85 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("분류기호"), ind, tag );
				break;

			default :
				continue;
				break;
		}
	}

	ShowMarcInfo();

EFS_END
}

VOID CSEBookDetailViewerDlg::ShowNonBookInfo()
{
EFS_BEGIN

	if( m_marc.IsEmpty() ) return ;

	CString firseInd, secondInd, subFieldCod, ind, data;
	POSITION tagPos = m_marcS.m_listTag.GetHeadPosition();

	while( tagPos )
	{
		CTag *pTag = ( CTag * )m_marcS.m_listTag.GetNext( tagPos );
		if( NULL == pTag )
			continue;

		ind = pTag->GetFirstIndicator() + pTag->GetSecondIndicator();
		switch( _ttoi( pTag->GetTagCode() ) )
		{
			case 240 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("표제/저자사항"), ind, 240 );
				break;

			case 245 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("표제/저자사항"), ind, 245 );
				break;

			case 250 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("판사항"), ind, 250 );
				break;

			case 255 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("수치데이터사항"), ind, 255 );
				break;

			case 256 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("수치데이터사항"), ind, 256 );
				break;

			case 260 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("발행사항"), ind, 260 );
				break;

			case 300 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("형태사항"), ind, 300 );
				break;

			case 440 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("총서사항"), ind, 440 );
				break;

			case 490 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("총서사항"), ind, 490 );
				break;

			case 780 :
				if( !GetSubFieldInfo( pTag, _T("agtxz") ) )
					AddMarcInfo( _T("주기사항"), ind, 780 );
				break;

			case 785 :
				if( !GetSubFieldInfo( pTag, _T("agtxz") ) )
					AddMarcInfo( _T("주기사항"), ind, 785 );
				break;

			case 507 :
				if( !GetSubFieldInfo( pTag, _T("atxz") ) )
					AddMarcInfo( _T("주기사항"), ind, 507 );
				break;

			case 765 :
				if( !GetSubFieldInfo( pTag, _T("atxz") ) )
					AddMarcInfo( _T("주기사항"), ind, 765 );
				break;

			case 767 :
				if( !GetSubFieldInfo( pTag, _T("txz") ) )
					AddMarcInfo( _T("주기사항"), ind, 767 );
				break;

			case 500 :
				if( !GetSubFieldInfo( pTag, _T("dnspqtac") ) )
					AddMarcInfo( _T("주기사항"), ind, 500 );
				break;

			case 508 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 508 );
				break;

			case 511 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 511 );
				break;

			case 775 :
				if( !GetSubFieldInfo( pTag, _T("atxz") ) )
					AddMarcInfo( _T("주기사항"), ind, 775 );
				break;

			case 509 :
				if( !GetSubFieldInfo( pTag, _T("ab") ) )
					AddMarcInfo( _T("주기사항"), ind, 509 );
				break;
			
			case 516 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 516 );
				break;

			case 565 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 565 );
				break;

			case 28 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 28 );
				break;

			case 340 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 340 );
				break;

			case 351 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 351 );
				break;

			case 760 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 760 );
				break;

			case 762 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 762 );
				break;

			case 580 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 580 );
				break;

			case 502 :
				if( !GetSubFieldInfo( pTag, _T("abcd") ) )
					AddMarcInfo( _T("주기사항"), ind, 502 );
				break;

			case 521 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 521 );
				break;

			case 506 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 506 );
				break;

			case 540 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 540 );
				break;

			case 355 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 355 );
				break;

			case 504 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 504 );
				break;

			case 524 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 524 );
				break;

			case 518 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 518 );
				break;

			case 770 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 770 );
				break;

			case 772 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 772 );
				break;

			case 773 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 773 );
				break;

			case 533 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 533 );
				break;

			case 534 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 534 );
				break;

			case 535 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 535 );
				break;

			case 530 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 530 );
				break;

			case 856 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 856 );
				break;

			case 776 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 776 );
				break;

			case 510 :
				if( !GetSubFieldInfo( pTag, _T("abx") ) )
					AddMarcInfo( _T("주기사항"), ind, 510 );
				break;

			case 536 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 536 );
				break;

			case 586 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 586 );
				break;

			case 538 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 538 );
				break;

			case 556 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 556 );
				break;

			case 561 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 561 );
				break;

			case 581 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 581 );
				break;

			case 585 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 585 );
				break;

			case 590 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 590 );
				break;

			case 505 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 505 );
				break;

			case 501 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 501 );
				break;

			case 777 :
				if( !GetSubFieldInfo( pTag, _T("") ) )
					AddMarcInfo( _T("주기사항"), ind, 777 );
				break;

			case 24 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 24 );
				break;

			case 74 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 74 );
				break;

			case 27 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 27 );
				break;

			case 88 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("주기사항"), ind, 88 );
				break;

			case 520 :
				if( !GetSubFieldInfo( pTag, _T("abc") ) )
					AddMarcInfo( _T("주기사항"), ind, 520 );
				break;

			case 22 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("ISSN"), ind, 22 );
				break;

			case 20 :
				if( !GetSubFieldInfo( pTag, _T("ag") ) )
					AddMarcInfo( _T("ISBN"), ind, 20 );
				break;

			case 56 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("분류기호"), ind, 56 );
				break;

			case 82 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("분류기호"), ind, 82 );
				break;

			case 85 :
				if( !GetSubFieldInfo( pTag, _T("a") ) )
					AddMarcInfo( _T("분류기호"), ind, 85 );
				break;

			default :
				continue;
				break;
		}		
	}

	ShowMarcInfo();

EFS_END
}

VOID CSEBookDetailViewerDlg::ShowMarcInfo()
{
EFS_BEGIN

	FreeMarcInfoDM();
	
	switch( m_bookType )
	{
		case 'a' :	// 단행
			ConvertSEBookInfo();
			break;
		case 'w' :	// 고서
			ConvertOldBookInfo();
			break;
		default :	// 비도서자료
			ConvertNonBookInfo();
			break;		
	}
		
	SetDisplayData();
	ControlDisplay( _T("CM_MNG_BUSINESS_SEARCHER_03_SE"), _T("그리드_2") );
	SetMarcGridProperty();

EFS_END
}

INT CSEBookDetailViewerDlg::GetSubFieldInfo(CTag *pTag, CString subFieldCode)
{
EFS_BEGIN

	FreeTagInfo();

	CString code, data;
	CSubfield *pSubField;
	POSITION posSubField = pTag->m_listSubfield.GetHeadPosition();
	CString tmp;
	while( posSubField )
	{
		pSubField = ( CSubfield * )pTag->m_listSubfield.GetNext( posSubField );
		if( NULL == pSubField )
			continue;
		
		code = pSubField->GetSubfieldCode();
		code.Replace( SUBFIELD_CODE, '$' );

		if( subFieldCode.GetLength() )
		{
			for( INT i = 0; i < subFieldCode.GetLength(); i++ )
				if( subFieldCode.Mid( i, 1 ) == code.Right( 1 ) )
				{
					m_tagInfo.subFieldCode.Add( code.Right( 1 ) );
					
					tmp = pSubField->GetPunctuationMark();
					if( _T(".") == tmp || _T(",") == tmp )
						data =  tmp + pSubField->GetItemData();
					else
						data =  tmp + _T(" ") + pSubField->GetItemData();
			
					data.TrimLeft(); data.TrimRight();
					m_tagInfo.data.Add( data );
					m_tagInfo.cnt++;
				}
		}
		else
		{
			m_tagInfo.subFieldCode.Add( code.Right( 1 ) );

			tmp = pSubField->GetPunctuationMark();
			if( _T(".") == tmp || _T(",") == tmp )
				data =  tmp + pSubField->GetItemData();
			else
				data =  tmp + _T(" ") + pSubField->GetItemData();

			data.TrimLeft(); data.TrimRight();
			m_tagInfo.data.Add( data );
			m_tagInfo.cnt++;
		}
	}
	
	return 0;

EFS_END
return -1;

}

VOID CSEBookDetailViewerDlg::AddMarcInfo(CString title, CString ind, INT tag)
{
EFS_BEGIN

	m_marcInfo = new MarcInfo;
	
	m_marcInfo->title = title;
	m_marcInfo->ind = ind;
	m_marcInfo->tag.Format( _T("%03d"), tag );
	m_marcInfo->tagInfo.cnt = m_tagInfo.cnt;
	for( INT i = 0; i < m_tagInfo.cnt; i++ )
	{
		m_marcInfo->tagInfo.data.Add( m_tagInfo.data.GetAt( i ) );
		m_marcInfo->tagInfo.subFieldCode.Add( m_tagInfo.subFieldCode.GetAt( i ) );
	}
	
	AllInfo.AddTail( ( CObject * )m_marcInfo );

EFS_END
}

VOID CSEBookDetailViewerDlg::FreeMarcInfoDM()
{
EFS_BEGIN
/*
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_MarcInfo") );
	pDM->FreeData();
*/
EFS_END
}

INT CSEBookDetailViewerDlg::ConvertSEBookInfo()
{
EFS_BEGIN

	INT cnt = AllInfo.GetCount();
	if( !cnt )
		return -1;

	POSITION pos = AllInfo.GetHeadPosition();
	MarcInfo *marcInfo;

	CString tmp, ind, buf;
	INT i, tag;
	
	while( pos )
	{
		marcInfo = ( MarcInfo * )AllInfo.GetNext( pos );
		if( NULL == marcInfo )
			continue;

		tag = _ttoi( marcInfo->tag );
		switch( tag )
		{
			case 240 :

				if( _T("0") != marcInfo->ind.Right( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" [ ") + buf + _T(" ] ");
					
				break;
			
			case 245 :
			case 440 :
			case 490 :

				ind = marcInfo->ind.Right( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("1") == ind )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 250 :
			case 255 :
			case 260 :
			case 300 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 500 :
				
				ind = marcInfo->ind.Right( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );
					if( CString( _T("befghijkmrctlsd") ).Find( tmp ) > -1 && _T("1") == ind )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

					if( _T(" ") == tmp )
						buf = _T(" 관제") + buf;
					else if( _T("e") == tmp )
						buf = _T(" 부서명") + buf;
					else if( _T("f") == tmp )
						buf = _T(" 판권기서명") + buf;
					else if( _T("g") == tmp )
						buf = _T(" 표지서명") + buf;
					else if( _T("h") == tmp )
						buf = _T(" 책등서명") + buf;
					else if( _T("i") == tmp )
						buf = _T(" 번역서명") + buf;
					else if( _T("j") == tmp )
						buf = _T(" 대등서명") + buf;
					else if( _T("k") == tmp )
						buf = _T(" 잡정보") + buf;
					else if( _T("m") == tmp )
						buf = _T(" 부논문") + buf;
					else if( _T("r") == tmp )
						buf = _T(" 총서명관제") + buf;
					else if( _T("l") == tmp )
						buf = _T(" 번역저자명") + buf;
					else if( _T("s") == tmp )
						buf = _T(" 총서명편자") + buf;
				}

			break;

			case 780 :
			case 785 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("x") == tmp )
						buf = buf + _T(" ISSN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("z") == tmp )
						buf = buf + _T(" ISBN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( CString( _T("agz") ).Find( tmp ) > 0 )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 507 :

				ind = marcInfo->ind.Right( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					if( _T("1") == ind )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );
					if( _T("t") == tmp )
						buf = _T(" 원서명") + buf;
					else if( _T("z") == tmp )
						buf = _T(" ISBN") + buf;
					else if( _T("a") == tmp )
						buf = _T(" 원저자명") + buf;
				}

				break;

			case 767 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("t") == tmp )
						buf = buf + _T(" 서명 ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("z") == tmp )
						buf = buf + _T(" ISBN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("a") == tmp )
						buf = buf + _T(" 번역자명 : ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 765 :

				if( _T("0") == marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("t") == tmp )
						buf = buf + _T(" 서명 ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("z") == tmp )
						buf = buf + _T(" ISBN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("a") == tmp )
						buf = buf + _T(" 원저자명 : ") + marcInfo->tagInfo.data.GetAt( i );
					
				}
			
			case 775 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("t") == tmp )
						buf = buf + _T(" 서명 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("x") == tmp )
						buf = buf + _T(" ISSN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("a") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 580 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 787 :
			case 506 :
			case 521 :
			case 504 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() && 521 == tag )
					buf = _T(" 이용대상자 ") + buf;

				break;

			case 502 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T(" ") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" - ") + marcInfo->tagInfo.data.GetAt( i );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 770 :
			case 772 :
			case 773 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					ind = marcInfo->tagInfo.data.GetAt( i );
					if( _T("g") == marcInfo->tagInfo.subFieldCode.GetAt( i ) && _T("-") == ind.Right( 1 ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i ) + _T("    ");
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				if( 770 == tag )
					buf = _T(" 부록/보유 : ") + buf;
				else if( 772 == tag )
					buf = _T(" 모체본 : ") + buf;
				else if( 773 == tag )
					buf = _T(" 수록처 : ") + buf;

				break;

			case 533 :
			case 534 :
				
				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 856 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T(" ") == tmp || _T("0") == tmp )
						buf = buf + _T(" 전자자료 ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("1") == tmp )
						buf = buf + _T(" 전자버전 ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("2") == tmp )
						buf = buf + _T(" 관련 전자자료 ") + marcInfo->tagInfo.data.GetAt( i );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 776 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 이용가능한 다른 형태 자료 : ") + buf;

				break;

			case 510 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 해제/초록수록지 : ") + buf;

				break;

			case 536 :
			case 538 :
			case 586 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() && 586 == tag )
					buf = _T(" 수상 : ") + buf;

				break;

			case 590 :

				if( _T("1") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 505 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp )
						buf = buf + _T(" 내용 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T(" ") == tmp )
						buf = buf + _T(" 불완전내용 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("c") == tmp )
						buf = buf + _T(" 부분내용 : ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 501 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 합철 : ") + buf;

				break;

			case 520 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i< marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp )
						buf = buf + _T(" 요약 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T(" ") == tmp )
						buf = buf + _T(" 초록 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("c") == tmp )
						buf = buf + _T(" 해제 : ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 22 :
			case 20 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( 22 == tag )
						buf = _T(" ISSN ") + buf;
					else if( 20 == tag )
						buf = _T(" ISBN ") + buf;

				break;

			case 56 :
			case 82 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( 56 == tag )
						buf = _T(" KDC-> ") + buf;
					else if( 82 == tag )
						buf = _T(" DDC-> ") + buf;

				break;

			case 85 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("o") == tmp )
						buf = buf + _T(" KDCP-> ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("1") == tmp )
						buf = buf + _T(" NDCP-> ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("2") == tmp )
						buf = buf + _T(" CEC-> ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("3") == tmp )
						buf = buf + _T(" CWC-> ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("4") == tmp )
						buf = buf + _T(" COC-> ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			default :

				continue;
				break;
		}

		AllTrim( buf );
		AddToMarcInfoDM( marcInfo->title, buf );
		tmp.Empty(); buf.Empty();
	}

	return 0;

EFS_END
return -1;

}

INT CSEBookDetailViewerDlg::ConvertOldBookInfo()
{
EFS_BEGIN

	INT cnt = AllInfo.GetCount();
	if( !cnt )
		return -1;

	POSITION pos = AllInfo.GetHeadPosition();
	MarcInfo *marcInfo;

	CString tmp, ind, buf;
	INT i, tag;
	
	while( pos )
	{
		marcInfo = ( MarcInfo * )AllInfo.GetNext( pos );
		if( NULL == marcInfo )
			continue;

		tag = _ttoi( marcInfo->tag );
		switch( tag )
		{
			case 240 :

				if( _T("0") != marcInfo->ind.Right( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" [ ") + buf + _T(" ] ");
					
				break;

			case 245 :

				ind = marcInfo->ind.Right( 1 );
				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("1") == ind )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 250 :
			case 260 :
			case 300 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 440 :
			case 490 :

				ind = marcInfo->ind.Right( 1 );
				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("1") == ind )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" ( ") + buf + _T(" ) ");

				break;

			case 500 :

				ind = marcInfo->ind.Right( 1 );
				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("1") == ind && ( _T("c") == tmp || _T("d") == tmp ) )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else if( CString( _T("azcdnpqt") ).Find( tmp ) > 0 )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );					
				}				

				break;
	
			case 780 :
			case 785 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( 780 == tag )
						buf = _T(" 선행저록 : ") + buf;
					else if( 785 == tag )
						buf = _T(" 후속저록 : ") + buf;

				break;
				
			case 507 :

				ind = marcInfo->ind.Right( 1 );
				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("1") == ind )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 765 :
			case 767 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp )
						if( 765 == tag )
							buf = buf + _T(" 원저자명 : ") + marcInfo->tagInfo.data.GetAt( i );
						else if( 767 == tag )
							buf = buf + _T(" 번역자명 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("t") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 775 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 이용가능한 다른판 : ") + buf;

				break;

			case 580 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 787 :
			case 506 :
			case 521 :
			case 504 :

				if( _T("  ") != marcInfo->ind )
					continue;
				
				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( 521 == tag && buf.GetLength() )
					buf = _T(" 이용대상자 : ") + buf;

				break;

			case 770 :
			case 772 :
			case 773 :
			case 776 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );
					ind = marcInfo->tagInfo.data.GetAt( i );

					if( _T("g") == tmp && _T("-") == ind.Right( 1 ) && 776 != tag )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i ) + _T("    ");
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				AllTrim( buf );
				if( buf.GetLength() )
					if( 770 == tag )
						buf = _T(" 부록/보유 : ") + buf;
					else if( 772 == tag )
						buf = _T(" 모체본 : ") + buf;
					else if( 773 == tag )
						buf = _T(" 수록처 : ") + buf;
					else if( 776 == tag )
						buf = _T(" 이용가능한 다른 형태 자료 : ") + buf;

				break;

			case 510 :
			case 536 :
			case 586 :
			case 538 :
			case 590 :
			case 562 :
			case 501 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( 510 == tag )
						buf = _T(" 해제/초록수록지 : ") + buf;
					else if( 586 == tag )
						buf = _T(" 수상 : ") + buf;

				break;

			case 505 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp )
						buf = buf + _T(" 내용 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T(" ") == tmp )
						buf = buf + _T(" 불완전 내용: ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("c") == tmp )
						buf = buf + _T(" 부분내용 : ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 520 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp )
						buf = buf + _T(" 요약 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T(" ") == tmp )
						buf = buf + _T(" 초록: ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("c") == tmp )
						buf = buf + _T(" 해제 : ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 56 :
			case 82 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( 56 == tag )
						buf = _T(" KDC-> ") + buf;
					else if( 82 == tag )
						buf = _T(" DDC-> ") + buf;

				break;

			case 85 :

				ind = marcInfo->ind.Left( 1 );				
				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( _T("0") == ind )
						buf = _T(" KDCP-> ") + buf;
					else if( _T("1") == ind )
						buf = _T(" NDCP-> ") + buf;
					else if( _T("2") == ind )
						buf = _T(" CEC-> ") + buf;
					else if( _T("3") == ind )
						buf = _T(" CWC-> ") + buf;
					else if( _T("4") == ind )
						buf = _T(" COC-> ") + buf;

				break;

			default :

				continue;
				break;
		}

		AllTrim( buf );
		AddToMarcInfoDM( marcInfo->title, buf );
		tmp.Empty(); buf.Empty();
	}

	return 0;

EFS_END
return -1;

}

INT CSEBookDetailViewerDlg::ConvertNonBookInfo()
{
EFS_BEGIN

	INT cnt = AllInfo.GetCount();
	if( !cnt )
		return -1;

	POSITION pos = AllInfo.GetHeadPosition();
	MarcInfo *marcInfo;

	CString tmp, ind, buf;
	INT i, tag;
	
	while( pos )
	{
		marcInfo = ( MarcInfo * )AllInfo.GetNext( pos );
		if( NULL == marcInfo )
			continue;

		tag = _ttoi( marcInfo->tag );
		switch( tag )
		{
			case 240 :

				tmp = marcInfo->ind.Right( 1 );
				if( _T(" ") == tmp )
					continue;
				else if( _T("0") == tmp )
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				else if( _T("1") == tmp )
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" [ ") + buf + _T(" ] ");
								
				break;

			case 245 :

				tmp = marcInfo->ind.Right( 1 );
				if( _T("1") == tmp )
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
				else
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 250 :
			case 254 :
			case 255 :
			case 256 :
			case 260 :
			case 300 :
			case 340 :
			case 351 :
			case 540 :
			case 355 :

				if( _T("  ") == marcInfo->ind )
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 400 :
			case 490 :

				tmp = marcInfo->ind.Right( 1 );
				if( _T("1") == tmp )
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
				else
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 500 :

				ind = marcInfo->ind.Right( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );
					
					if( _T("c") == tmp && _T("1") == ind )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else if( _T("c") == tmp || _T("a") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("d") == tmp && _T("1") == ind )
						buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
					else if( _T("d") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("n") == tmp || _T("s") == tmp || _T("p") == tmp || _T("q") == tmp || _T("t") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
					else
						continue;
				}

				break;

			case 780 :
			case 785 :

				if( _T("0") == marcInfo->ind.Left( 1 ) )
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					{
						tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

						if( _T("a") == tmp || _T("g") == tmp || _T("t") == tmp )
							buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
						else if( _T("x") == tmp )
							buf = buf + _T(" ISSN ") + marcInfo->tagInfo.data.GetAt( i );
						else if( _T("z") == tmp )
							buf = buf + _T(" ISBN ") + marcInfo->tagInfo.data.GetAt( i );
						else
							continue;
					}

				break;

			case 507 :

				tmp = marcInfo->ind.Right( 1 );

				if( _T("1") == tmp )
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					{
						tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

						if( _T("t") == tmp )
							buf = buf + _T(" ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
						else if( _T("x") == tmp )
							buf = buf + _T(" ISSN ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
						else if( _T("z") == tmp )
							buf = buf + _T(" ISBN ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
						else if( _T("a") == tmp )
							buf = buf + _T(" 원저자명 : ") + RemoveParenthesis( marcInfo->tagInfo.data.GetAt( i ) );
						else 
							continue;
					}
				else
					for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					{
						tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

						if( _T("t") == tmp )
							buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
						else if( _T("x") == tmp )
							buf = buf + _T(" ISSN ") + marcInfo->tagInfo.data.GetAt( i );
						else if( _T("z") == tmp )
							buf = buf + _T(" ISBN ") + marcInfo->tagInfo.data.GetAt( i );
						else if( _T("a") == tmp )
							buf = buf + _T(" 원저자명 : ") + marcInfo->tagInfo.data.GetAt( i );
						else 
							continue;
					}

				break;

			case 765 :
			case 767 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );
						if( _T("x") == tmp )
						buf = buf + _T(" ISSN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("z") == tmp )
						buf = buf + _T(" ISBN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("t") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
					else if( 765 == tag && _T("a") == tmp )
						buf = buf + _T(" 원저자명 : ") + marcInfo->tagInfo.data.GetAt( i );
					else 
						continue;
				}

				break;

			case 508 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );
					if( _T("a") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 511 :

				ind = marcInfo->ind.Left( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("1") == ind && _T("a") == tmp )
						buf = buf + _T(" 배역 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("a") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 775 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp || _T("t") == tmp )
						buf = buf + _T(" 이용가능한 다른판 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("x") == tmp )
						buf = buf + _T(" ISSN : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("z") == tmp )
						buf = buf + _T(" ISBN : ") + marcInfo->tagInfo.data.GetAt( i );
					else
						continue;
				}

				break;

			case 509 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp || _T(" ") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;

			case 516 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" 파일유형 : ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 565 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 파일특성 : ") + buf;

				break;

			case 28 :

				if( _T("1") != marcInfo->ind.Right( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T(" ") == tmp )
						buf = buf + _T(" : ") + marcInfo->tagInfo.data.GetAt( i );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 발행자번호 : ") + buf;

				break;
			
			case 760 :
			case 762 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" : ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( 760 == tag )
						buf = _T(" 상위총서 : ") + buf;
					else if( 762 == tag )
						buf = _T(" 하위총서 : ") + buf;

				break;
			
			case 580 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" : ") + marcInfo->tagInfo.data.GetAt( i );

				break;
			
			case 502 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a")== tmp || _T("c") == tmp || _T("d") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T(" ") == tmp )
						buf = buf + _T(" - ") + marcInfo->tagInfo.data.GetAt( i );
				}

				break;
			
			case 521 :
			case 506 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( tag == 521 && buf.GetLength() )
					buf = _T(" 이용대상자 : ") + buf;	

				break;
			
			case 504 :
			case 518 :
				
				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				
				break;

			case 524 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 인용 : ") + buf;

				break;
		
			case 770 :
			case 772 :
			case 773 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("g") == marcInfo->tagInfo.subFieldCode.GetAt( i ) && _T("-") == marcInfo->tagInfo.data.GetAt( i ).Right( 1 ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i ) + _T("    ");
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;
			
			case 533 :
			case 534 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( 533 == tag && buf.GetLength() )
					buf = _T(" 복제 : ") + buf;

				break;

			case 535 :

				ind = marcInfo->ind.Left( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( _T("1") == ind )
						buf = _T(" 원본 소장기관 : ") + buf;
					else if( _T("2") == ind )
						buf = _T(" 복제본 소장기관 : ") + buf;

				break;

			case 530 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 856 :

				ind = marcInfo->ind.Right( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( _T(" ") == ind || _T("0") == ind )
						buf = _T(" 전자자료 : ") + buf;
					else if( _T("1") == ind )
						buf = _T(" 전자버전 : ") + buf;
					else if( _T("2") == ind )
						buf = _T(" 관련 전자자료 ; ") + buf;

				break;

			case 776 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 이용가증한 다른 형태 자료 : ") + buf;

				break;

			case 510 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp || _T(" ") == tmp || _T("x") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
				}
		
				break;

			case 536 :
			case 586 :
			case 538 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( 586 == tag && buf.GetLength() )
					buf = _T(" 수상 : ") + buf;

				break;

			case 556 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("z") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ISBNb : ") + marcInfo->tagInfo.data.GetAt( i );
					else
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 561 :
			case 581 :
			case 585 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 590 :

				if( _T("1") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				break;

			case 505 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp )
						buf = buf + _T(" 내용 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T(" ") == tmp )
						buf = buf + _T(" 불완전내용 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("c") == tmp )
						buf = buf + _T(" 부분내용 : ") + marcInfo->tagInfo.data.GetAt( i );
					else
						continue;
				}

				break;

			case 501 :
			case 22 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( 501 == tag )
						buf = _T(" 합철 : ") + buf;
					else if( 22 == tag )
						buf = _T(" ISSN ") + buf;

				break;

			case 777 :

				if( _T("0") != marcInfo->ind.Left( 1 ) )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					buf = _T(" 합쇄 : ") + buf;

				break;

			case 24 :

				ind = marcInfo->ind.Left( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("0") == ind )
						buf = buf + _T(" ISRC ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("1") == ind )
						buf = buf + _T(" UPC ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("2") == ind )
						buf = buf + _T(" ISMN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("3") == ind )
						buf = buf + _T(" EAN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("4") == ind )
						buf = buf + _T(" SICI ") + marcInfo->tagInfo.data.GetAt( i );
					else 
						continue;

				break;

			case 74 :
			case 27 :
			case 88 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				
				AllTrim( buf );
				if( buf.GetLength() )
					if( 27 == tag )
						buf = _T(" STRN ") + buf;
					else if( 88 == tag )
						buf = _T(" 보고서번호 : ") + buf;

				break;

			case 520 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i< marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp )
						buf = buf + _T(" 요약 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T(" ") == tmp )
						buf = buf + _T(" 초록 : ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("c") == tmp )
						buf = buf + _T(" 해제 : ") + marcInfo->tagInfo.data.GetAt( i );
					else
						continue;
				}

				break;
		
			case 20 :

				if( _T("  ") != marcInfo->ind )
					continue;

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
				{
					tmp = marcInfo->tagInfo.subFieldCode.GetAt( i );

					if( _T("a") == tmp )
						buf = buf + _T(" ISBN ") + marcInfo->tagInfo.data.GetAt( i );
					else if( _T("g") == tmp )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );
					else
						continue;
				}

				break;

			case 56 :
			case 82 :

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( 56 == tag )
						buf = _T(" KDC-> ") + buf;
					else if( 82 == tag )
						buf = _T(" DDC=> ") + buf;

				break;

			case 85 :

				ind = marcInfo->ind.Left( 1 );

				for( i = 0; i < marcInfo->tagInfo.cnt; i++ )
					if( _T("a") == marcInfo->tagInfo.subFieldCode.GetAt( i ) )
						buf = buf + _T(" ") + marcInfo->tagInfo.data.GetAt( i );

				AllTrim( buf );
				if( buf.GetLength() )
					if( _T("0") == ind )
						buf = _T(" KDCP-> ") + buf;
					else if( _T("1") == ind )
						buf = _T(" NDCP-> ") + buf;
					else if( _T("2") == ind )
						buf = _T(" CEC-> ") + buf;
					else if( _T("3") == ind )
						buf = _T(" CWC-> ") + buf;
					else if( _T("4") == ind )
						buf = _T(" COC-> ") + buf;

				break;
			
			default :

				continue;
				break;
		}

		AllTrim( buf );
		AddToMarcInfoDM( marcInfo->title, buf );
		tmp.Empty(); buf.Empty();
	}
	
	return 0;

EFS_END
return -1;

}

VOID CSEBookDetailViewerDlg::SetDisplayData()
{
EFS_BEGIN

	CESL_DataMgr *pDM_m = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_MarcInfo") );	
	CESL_DataMgr *pDM_d = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_displayMarc") );
	pDM_d->FreeData();

	const INT cnt = 2;
	CString alias[ cnt ] = { _T("title"), _T("contents") };
	CString data[ cnt ];

	const INT titleCnt = 10;
	CString title[ titleCnt ] = {
		_T("표제/저자사항"), _T("판사항"), _T("수치데이터사항"), _T("발행사항"), _T("형태사항"), _T("총서사항"), _T("주기사항"), _T("ISSN"), _T("ISBN"), _T("분류기호") };

	INT rowCnt;
	for( INT i = 0; i < titleCnt; i++ )
	{
		rowCnt = pDM_m->GetRowCount();
		for( INT j = rowCnt - 1 ; j > -1; j-- )
		{
			pDM_m->GetCellData( alias, cnt, data, j );
			if( title[ i ] == data[ 0 ] )
			{
				pDM_d->SetCellData( alias, cnt, data, -1 );
				pDM_m->DeleteRow( j );
			}
		}
	}

	for( i = 0; i < pDM_m->GetRowCount(); i++ )
	{
		pDM_m->GetCellData( alias, cnt, data, i );
		pDM_d->SetCellData( alias, cnt, data, -1 );
	}

	CString tmp1, tmp2;
	rowCnt = pDM_d->GetRowCount();
	for( i = rowCnt - 1; i > 0; i-- )
	{
		pDM_d->GetCellData( _T("title"), i, tmp1 );
		pDM_d->GetCellData( _T("title"), i - 1, tmp2 );
		if( tmp1 == tmp2 )
			pDM_d->SetCellData( _T("title"), _T(""), i );
	}	

EFS_END
}

VOID CSEBookDetailViewerDlg::SetMarcGridProperty()
{
EFS_BEGIN
/*
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_mngGRID_2 );
	pGrid->SetColWidth( 0, 0, 0 );
	pGrid->SetRowHeight( 0, 0 );
	
	COLORREF blue, white, black;
	blue  = RGB( 228, 243, 251 );
	white = RGB( 255, 255, 255 );
	black = RGB( 0, 0, 0 );

	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_displayMarc") );
	CString contents;
	const INT contentsLength = 90;
	INT lines = 0;
	
	for( INT i = 0; i < pDM->GetRowCount(); i++ )
	{
		pGrid->SetCol( 1 );
		pGrid->SetRow( i + 1 );
		pGrid->SetCellBackColor( blue );
		pGrid->SetCellForeColor( black );

		pDM->GetCellData( _T("contents"), i, contents );
		lines = GetMultiLineText( contentsLength, contents );
		
		if( lines > 1 )
		{
			pGrid->SetCol( 2 );
			pGrid->SetRowHeight( i + 1, lines * pGrid->GetRowHeight( i + 1 ) );
			pGrid->SetText( contents );
		}
	}

*/

EFS_END
}

VOID CSEBookDetailViewerDlg::FreeTagInfo()
{
EFS_BEGIN

	m_tagInfo.cnt = 0;
	m_tagInfo.data.RemoveAll();
	m_tagInfo.subFieldCode.RemoveAll();

EFS_END
}

VOID CSEBookDetailViewerDlg::AllTrim(CString &data)
{
EFS_BEGIN

	data.TrimLeft();
	data.TrimRight();

EFS_END
}

CString CSEBookDetailViewerDlg::RemoveParenthesis(CString data)
{
EFS_BEGIN

	INT locate_1 = data.Find( _T("(") );
	if( -1 == locate_1 )
		return data;

	INT locate_2 = data.Find( _T(")") );
	if( -1 == locate_2 )
		return data;

	data.Delete( locate_1, locate_2 - locate_1 + 1 );
	
	return data;

EFS_END
return _T("");

}

VOID CSEBookDetailViewerDlg::AddToMarcInfoDM(CString title, CString contents)
{
EFS_BEGIN

	const INT listCnt = 2;
	CString alias[ listCnt ] = { _T("title"), _T("contents") };
	CString data[ listCnt ];
	data[ 0 ] = title;
	data[ 1 ] = contents;

	for( INT i = 0; i < listCnt; i++ )
	{
		AllTrim( data[ i ] );
		if( !data[ i ].GetLength() )
			return;
	}
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_MarcInfo") );
	pDM->SetCellData( alias, listCnt, data, -1 );	

EFS_END
}

INT CSEBookDetailViewerDlg::GetMultiLineText(INT length, CString &contents)
{
EFS_BEGIN

	INT lines = contents.GetLength() / length;
	if( lines < 1 )
		return 1;
	lines = 1;

	INT bCnt = 0;
	INT locate = -1;
	
	locate = contents.Find( _T(" "), 0 );
	while( locate > 0 )
	{
		bCnt++;
		locate = contents.Find( _T(" "), ++locate );
	}
	bCnt++;

	CString *pContents = new CString[ bCnt ];
	for( INT i = 0; i < bCnt; i++ )
		if( bCnt - 1 == i )
			pContents[ i ] = contents;
		else
		{
			locate = contents.Find( _T(" "), 0 ) + 1;
			pContents[ i ] = contents.Left( locate );
			contents.Delete( 0, locate );
		}
	
	contents.Empty();
	INT len = 0;
	for( i = 0; i < bCnt; i++ )
	{
		if( pContents[ i ].GetLength() >= length )
		{
			contents = contents + _T("\n") + pContents[ i ];
			lines++;
			len = 0;
		}
		else
		{
			contents += pContents[ i ];
			len += pContents[ i ].GetLength();
		}		
		
		if( length < len )
		{
			len = pContents[ i ].GetLength();
			contents.Delete( contents.GetLength() - len, len );
			contents += _T("\n");
			len = 0;
			lines++;
			i--;
		}		
	}

	delete [] pContents;

	return lines;

EFS_END
return -1;

}

INT CSEBookDetailViewerDlg::SetDataPK( CString pk, INT option )
{
EFS_BEGIN

	m_pk = pk;
	
	return 0;

EFS_END
return -1;

}

VOID CSEBookDetailViewerDlg::OnbMARC() 
{
EFS_BEGIN

	CString marc;
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_SE") );
	pDM->GetCellData( _T("S_MARC"), 0, marc );

	marc.TrimLeft(); marc.TrimRight();

	if( marc.IsEmpty() ) 
	{
		AfxMessageBox( _T("마크가 존재하지 않는 자료입니다. ") );
		return ;
	}

	
	CSEBookMarcViewer dlg( this );
	dlg.DoModal();

EFS_END
}

VOID CSEBookDetailViewerDlg::OnbAPPENDIX() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CSEBookAppViewer dlg( this );
	dlg.m_bookKey = m_pk;
	dlg.DoModal();

EFS_END
}

VOID CSEBookDetailViewerDlg::ShowWorkStatus( CString status )
{
EFS_BEGIN

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_EDIT2 );
	pEdt->SetWindowText( GetWorkStatusDesc( status ) );

EFS_END
}

CString CSEBookDetailViewerDlg::GetWorkStatusDesc( CString status )
{
EFS_BEGIN

	

	//=====================================================
	//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 	const INT cnt = 23;
// 	CString desc[ cnt ] = {
// 		_T("구입대상자료"),		//	0
// 		_T("주문선정자료"),		//	1
// 		_T("검수대상자료"),		//	2
// 		_T("미납품자료"),		//	3
// 		_T("기증대상자료"),		//	4
// 		_T("기증부당자료"),		//	5
// 		_T("구입검수완료자료"),	//	6
// 		_T("기증접수완료자료"),	//	7
// 		_T("정리인계자료"),		//	8
// 		_T("분류대상사료"),		//	9
// 		_T("정리대상자료"),		// 10
// 		_T("복본추기자료"),		// 11
// 		_T("정리완료자료"),		// 12
// 		_T("비치대상자료"),		// 13
// 		_T("비치자료"),			// 14
// 		_T("관외대출자료"),		// 15
// 		_T("관내대출자료"),		// 16
// 		_T("특별대출자료"),		// 17
// 		_T("긴급대출자료"),		// 18
// 		_T("배달자료"),			// 19
// 		_T("수리제본자료"),		// 20
// 		_T("분실자료"),			// 21
// 		_T("제적자료") };		// 22
// 
// 	CString stat[ cnt ] = {
// 		_T("BOA111N"),	//	0
// 		_T("BOA112N"),	//	1
// 		_T("BOA113N"),	//	2
// 		_T("BOA114O"),	//	3
// 		_T("BOA131N"),	//	4
// 		_T("BOA132N"),	//	5
// 		_T("BOA141N"),	//	6
// 		_T("BOA142N"),	//	7
// 		_T("BOC111N"),	//	8
// 		_T("BOC112O"),	//	9
// 		_T("BOC113N"),	// 10	
// 		_T("BOC114N"),	// 11	
// 		_T("BOC115N"),	// 12	
// 		_T("BOL111O"),	// 13	
// 		_T("BOL112N"),	// 14	
// 		_T("BOL221O"),	// 15	
// 		_T("BOL222O"),	// 16	
// 		_T("BOL223O"),	// 17	
// 		_T("BOL224O"),	// 18	
// 		_T("BOL225O"),	// 19	
// 		_T("BOL251N"),	// 20	
// 		_T("BOL252N"),	// 21	
// 		_T("BOL253N") };// 22
		// 2013.12.01 UPDATE BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
		// 18.09.27 JMJ KOL.RED.2018.008
		const INT cnt = 26;
		//const INT cnt = 24;
		CString desc[ cnt ] = {
		_T("구입대상자료"),		//	0
		_T("주문선정자료"),		//	1
		_T("검수대상자료"),		//	2
		_T("미납품자료"),		//	3
		_T("기증대상자료"),		//	4
		_T("기증부당자료"),		//	5
		_T("구입검수완료자료"),	//	6
		_T("기증접수완료자료"),	//	7
		_T("정리인계자료"),		//	8
		_T("분류대상사료"),		//	9
		_T("정리대상자료"),		// 10
		_T("복본추기자료"),		// 11
		_T("정리완료자료"),		// 12
		_T("비치대상자료"),		// 13
		_T("비치자료"),			// 14
		_T("관외대출자료"),		// 15
		_T("관내대출자료"),		// 16
		_T("특별대출자료"),		// 17
		_T("긴급대출자료"),		// 18
		_T("배달자료"),			// 19
		_T("수리제본자료"),		// 20
		_T("분실자료"),			// 21
		_T("제적자료"),			// 22
		_T("단행책두레업무진행자료"),// 23
		_T("통합도서타관반납"),	// 24
		_T("통합도서타관대출")};	// 25

	CString stat[ cnt ] = {
		_T("BOA111N"),	//	0
		_T("BOA112N"),	//	1
		_T("BOA113N"),	//	2
		_T("BOA114O"),	//	3
		_T("BOA131N"),	//	4
		_T("BOA132N"),	//	5
		_T("BOA141N"),	//	6
		_T("BOA142N"),	//	7
		_T("BOC111N"),	//	8
		_T("BOC112O"),	//	9
		_T("BOC113N"),	// 10	
		_T("BOC114N"),	// 11	
		_T("BOC115N"),	// 12	
		_T("BOL111O"),	// 13	
		_T("BOL112N"),	// 14	
		_T("BOL221O"),	// 15	
		_T("BOL222O"),	// 16	
		_T("BOL223O"),	// 17	
		_T("BOL224O"),	// 18	
		_T("BOL225O"),	// 19	
		_T("BOL251N"),	// 20	
		_T("BOL252N"),	// 21	
		_T("BOL253N"),	// 22
		_T("BOL411O"),	// 23
		_T("BOL511O"),	// 24
		_T("BOL611O")};	// 25
	//=====================================================
	

	for( INT i = 0; i < cnt; i++ )
		if( status == stat[ i ] )
			return desc[ i ];


	return _T("");

EFS_END
return _T("");

}

VOID CSEBookDetailViewerDlg::SetCodeToDesc()
{
EFS_BEGIN


	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_BUSINESS_SEARCHER_03_SE_BOOK") );

	const INT nCnt = 4;
	CString sCodeName[nCnt] =
	{
		_T("자료실구분") , _T("매체구분") , _T("관리구분") , _T("이용제한구분")
	};
	CString sDMFieldName[nCnt] =
	{
		_T("자료실구분") , _T("매체구분") , _T("관리구분") , _T("이용제한구분")
	};
	
	for( INT i = 0 ; i < 4 ; i++ )
		m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, sCodeName[i] , sDMFieldName[i] );

	// 자료상태 코드 -> 설명
	// 청구기호 생성
	CLocCommonAPI::MakeShelfCode( this , pDM->DB_MGR_ALIAS );
	
	// 청구기호 + 함번호
	CString sWorkingStatus;
	CString sCallNo,sBoxName;
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("B_WORKING_STATUS") , i , sWorkingStatus );
		CLocCommonAPI::GetWorkingStatusDescToCode( sWorkingStatus , 1);
		pDM->SetCellData( _T("B_WORKING_STATUS") , sWorkingStatus , i );
		
		pDM->GetCellData( _T("청구기호") , i , sCallNo );
		pDM->GetCellData( _T("함번호") , i , sBoxName );

		if( !sBoxName.IsEmpty() )
			sCallNo += _T("(") + sBoxName + _T(")");

		pDM->SetCellData( _T("청구기호") , sCallNo , i );

	}




EFS_END
}

CString CSEBookDetailViewerDlg::GetShelfCode( CString data[] )
{
EFS_BEGIN

	CString shelfCode;
	shelfCode.Format( _T("%s-%s-%s-%s-%s"), data[ 0 ], data[ 1 ], data[ 2 ], data[ 3 ], data[ 4 ] );

	shelfCode.Replace( _T("----"), _T("-") );
	shelfCode.Replace( _T("---"), _T("-") );
	shelfCode.Replace( _T("--"), _T("-") );

	if( _T("-") == shelfCode.Left( 1 ) )
		shelfCode = shelfCode.Right( shelfCode.GetLength() - 1 );

	if( _T("-") == shelfCode.Right( 1 ) )
		shelfCode = shelfCode.Left( shelfCode.GetLength() - 1 );
	
	return shelfCode;

EFS_END
return _T("");

}

VOID CSEBookDetailViewerDlg::OnbBEFORE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	if( !m_idx )
	{
		AfxMessageBox( _T("맨 처음 목록입니다.") );
		return;
	}
	
	m_idx--;
	POSITION pos = m_pkList.FindIndex( m_idx );
	m_pk = m_pkList.GetPrev( pos );
	
	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );
	ShowBODetailView();

EFS_END
}

VOID CSEBookDetailViewerDlg::OnbNEXT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	
	
	if( m_idx == m_pkCnt - 1 )
	{
		AfxMessageBox( _T("맨 마지막 목록입니다.") );
		return;
	}
	
	m_idx++;
	POSITION pos = m_pkList.FindIndex( m_idx );
	m_pk = m_pkList.GetNext( pos );

	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );
	ShowBODetailView();

EFS_END
}

VOID CSEBookDetailViewerDlg::SetPKList( CStringList &pkList, CESL_Grid *grid, CArray< INT, INT > &gridRow, INT idx )
{
EFS_BEGIN

	m_pkList.RemoveAll();
	
	POSITION pos = pkList.GetHeadPosition();
	while( pos )
		m_pkList.AddTail( pkList.GetNext( pos ) );		

	m_pkCnt = m_pkList.GetCount();

	
	m_idx = idx;

	pos = m_pkList.FindIndex(m_idx);
	m_pk = m_pkList.GetNext( pos );

	m_eslGrid = grid;

	for( INT i = 0; i < gridRow.GetSize(); i++ )
		m_selectedGridRow.Add( gridRow.GetAt( i ) );

	m_eslGrid->SetReverse( m_selectedGridRow.GetAt( m_idx ) );

EFS_END
}

INT CSEBookDetailViewerDlg::DisplaySpecies()
{
	INT ids;
	
	CString sDMAlias = _T("DM_MNG_BUSINESS_SEARCHER_03_SE");
	
	CESL_DataMgr* pDM = FindDM( sDMAlias );
	if( pDM == NULL ) return -1;
	
	const INT nFieldCnt = 11;
	TCHAR* sDMFieldName[nFieldCnt] =
	{
		_T("서명사항") ,
		_T("저작자사항") ,
		_T("발행자사항") ,
		//_T("형태사항") ,
		_T("S_MARC"),
		_T("ISSN") ,
		_T("분류기호") ,
		_T("형식구분") ,
		_T("매체구분") ,
		_T("관리구분") ,
		_T("이용제한구분") ,
		_T("이용대상구분") 	
	};
	
	TCHAR* sTitleName[nFieldCnt] =
	{
		_T("서명사항") ,
		_T("저작자사항") ,
		_T("발행자사항") ,
		_T("형태사항") ,
		_T("ISSN") ,
		_T("분류기호") ,
		_T("형식구분") ,
		_T("매체구분") ,
		_T("관리구분") ,
		_T("이용제한구분") ,
		_T("이용대상구분") 	
	};
	
	
	CMarcMgr* pMarcMgr = m_pInfo->pMarcMgr;
	CMarc Marc;

	CString sData;
	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		ids = pDM->GetCellData(sDMFieldName[i] , 0 , sData );
		if( ids < 0 ) 
		{
			CString sEMsg;
			sEMsg.Format( _T("pDM->GetCellData(sDMFieldName[i] , m_nCurrentIndex , sData ) \r\n sDMFieldName[i] : %s \r\n m_nCurrentIndex : %d ") , 
				sDMFieldName[i] , 0 );
			AfxMessageBox( sEMsg );
		}
		if( ids < 0 ) return -2;
		
		if( 3 == i ){
		pMarcMgr->Decoding( sData , &Marc );
			sData.Empty();
			Marc.GetField( _T("300"), sData );
			sData.Replace( CString( 31 ) + _T("a") , _T("") );
			sData.Replace( CString( 31 ) + _T("b") , _T("") );
			sData.Replace( CString( 31 ) + _T("c") , _T("") );
			sData.Replace( CString( 31 ) + _T("d") , _T("") );
			sData.Replace( CString( 31 ) + _T("e") , _T("") );
			sData.Replace( CString( 30 ) , _T("") );
			sData.TrimLeft();
			sData.TrimRight();
		}		
		
		m_ViewerControl.SetControlData( sTitleName[i] , sData );
		
	}
	
	m_ViewerControl.Display();
	
	return 0;
}

HBRUSH CSEBookDetailViewerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

LRESULT CSEBookDetailViewerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CSEBookDetailViewerDlg::OnNcActivate(BOOL bActive) 
{	
	return CDialog::OnNcActivate(bActive);
}
