// LoanCardPrinter.cpp: implementation of the CLoanCardPrinter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoanCardPrinter.h"

#include "LoanCardFormatSetter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoanCardPrinter::CLoanCardPrinter( CESL_Mgr *pParent )
{
	m_pParent = pParent;

	m_startPoint[ 0 ].x = m_startPoint[ 1 ].x = m_startPoint[ 2 ].x = m_startPoint[ 3 ].x = 48;
	m_startPoint[ 0 ].y = 59;
	m_startPoint[ 1 ].y = 257;
	m_startPoint[ 2 ].y = 456;
	m_startPoint[ 3 ].y = 656;
	
/*
59
232
48
329

257
430
48
329

456
629
48
329

656
829
48
329

top
bottom
left
right
*/

}

CLoanCardPrinter::~CLoanCardPrinter()
{
	FreeObjectList();
}

INT CLoanCardPrinter::PrintLoanCard( CESL_DataMgr *pDM )
{
	m_pDM = pDM;
	if( MakeDefaultObjList() )
		return -1;

	CSIReportObjManager printMgr( this );
	
	// 대출증 출력 명수
	INT userCnt = pDM->GetRowCount();
	printMgr.m_userCnt = userCnt;
	// 한페이지에 올 대출증 개수
	printMgr.m_cardCntOnPage = 4;
	// 출력 페이지 개수
	printMgr.m_pageCnt = ( 
		userCnt % printMgr.m_cardCntOnPage 
		? userCnt / printMgr.m_cardCntOnPage + 1
		: userCnt / printMgr.m_cardCntOnPage );
	// 대출증 포맷에 있는 object 개수
	printMgr.m_objCntOnCard = GetDefaultObjCnt();

	CSIReportManager reportMgr( m_pParent );
	reportMgr.m_nDefaultPaperType = PAPER_A4_PORTRAIT;
	reportMgr.DeleteTempPageFiles();
	printMgr.Print();
	INT ids = reportMgr.Print2();

	return 0;
}

CSIReportObject *CLoanCardPrinter::MakeTextBoxObj( CESL_DataMgr *pDM, INT idx )
{
	CSIReportObject_Text_Box *pTextBox = new CSIReportObject_Text_Box;
	
	// TYPE -> TEXTBOX
	pTextBox->SetObjType( OBJECT_TEXT_BOX );
	
	// CSIReportObject
	pTextBox->m_strName = pDM->GetCellData( _T("ALIAS"), idx );

	// RECT
	pTextBox->SetRect( GetRectFromString( pDM->GetCellData( _T("BOX_SIZE"), idx ) ) );
	
	// 라인당 문자 개수
	pTextBox->char_len = _ttoi( pDM->GetCellData( _T("CHAR_LEN"), idx ) );
	
	// 전체 최대문자 개수
	pTextBox->char_max = _ttoi( pDM->GetCellData( _T("CHAR_MAX"), idx ) );
	
	// 수평 정렬
	CString sBuf = pDM->GetCellData( _T("HORZ_ALIGN"), idx );
	if( _T("왼쪽") == sBuf )
		pTextBox->horz_align = ALIGN_LEFT;
	else if( _T("중앙") == sBuf )
		pTextBox->horz_align = ALIGN_HCENTER;
	else if( _T("오른쪽") == sBuf )
		pTextBox->horz_align = ALIGN_RIGHT;

	// 수직정렬
	sBuf = pDM->GetCellData( _T("VERT_ALIGN"), idx );
	if( _T("위") == sBuf )
		pTextBox->vert_align = ALIGN_TOP;
	else if( _T("중앙") == sBuf )
		pTextBox->vert_align = ALIGN_VCENTER;
	else if( _T("아래") == sBuf )
		pTextBox->vert_align = ALIGN_BOTTOM;
	
	// 수평선 유무
	if( _T("Y") == pDM->GetCellData( _T("HORZ_LINE"), idx ) )
		pTextBox->horz_line = true;
	else
		pTextBox->horz_line = false;

	// 수직선 유무
	if( _T("Y") == pDM->GetCellData( _T("VERT_LINE"), idx ) )
		pTextBox->vert_line = true;
	else
		pTextBox->vert_line = false;

	/* font info
	LOGFONT font;							/// 폰트
	LONG size;
	CString text;							/// 내용
	COLORREF color;							/// 색깔
	*/	
	if( _T("Y") == pDM->GetCellData( _T("FONT_BOLD"), idx ) )
		pTextBox->text.font.lfWeight = FW_BOLD;
	pTextBox->text.size = _ttoi( pDM->GetCellData( _T("FONT_SIZE"), idx ) );
	_stprintf( pTextBox->text.font.lfFaceName, _T("%s"), pDM->GetCellData( _T("FONT"), idx ).GetBuffer( 0 ) );
	pTextBox->text.font.lfHeight = _ttoi( pDM->GetCellData( _T("FONT_SIZE"), idx ) );
	
	return ( CSIReportObject * )pTextBox;
}

CSIReportObject *CLoanCardPrinter::MakeBarcodeObj( CESL_DataMgr *pDM, INT idx )
{
	CSIReportObject_Barcode *pBarcode = new CSIReportObject_Barcode;

	// TYPE -> BARCODE
	pBarcode->SetObjType( OBJECT_BARCODE );

	// Alias
	pBarcode->m_strName = pDM->GetCellData( _T("ALIAS"), idx );

	// RECT
	pBarcode->SetRect( GetRectFromString( pDM->GetCellData( _T("BOX_SIZE"), idx ) ) );

	// STYLE
	pBarcode->SetStyle( _T("Code 3 of 9") );
	

	return ( CSIReportObject * )pBarcode;
}

VOID *CLoanCardPrinter::GetReportObject( INT locate, INT infoDMIndex )
{
	// 각 페이지 위치에 대한 정보와 값 추가하기.
	
	INT objCnt = m_objList.GetCount();
	if( infoDMIndex < objCnt && infoDMIndex > -1 )
		return ( VOID * )m_objList.GetAt( m_objList.FindIndex( infoDMIndex ) );
	else
		return NULL;
}

INT CLoanCardPrinter::GetDefaultObjCnt()
{
	CLoanCardFormatSetter *pParent = ( CLoanCardFormatSetter * )m_pParent;
	CESL_DataMgr *pInfoDM = pParent->FindDM( _T("DM_LoanCardFormatSetter") );

	return pInfoDM->GetRowCount();
}

INT CLoanCardPrinter::MakeDefaultObjList()
{
	CLoanCardFormatSetter *pParent = ( CLoanCardFormatSetter * )m_pParent;
	CESL_DataMgr *pDM = pParent->FindDM( _T("DM_LoanCardFormatSetter") );

	INT rowCnt = pDM->GetRowCount();
	if( !rowCnt )
	{
		AfxMessageBox( _T("대출증 구성 항목을 찾을 수 없습니다.") );
		return -1;
	}

	for( INT i = 0; i < rowCnt; i++ )
		switch( pDM->GetCellData( _T("TYPE"), i ).GetAt( 0 ) )
		{
			case 'T' :	// TEXT BOX
				m_objList.AddTail( ( CObject * )MakeTextBoxObj( pDM, i ) );
				break;
			case 'B' :	// BAR CODE
				m_objList.AddTail( ( CObject * )MakeBarcodeObj( pDM, i ) );
				break;
			default :
				break;
		}

	return 0;
}

CRect CLoanCardPrinter::GetRectFromString( CString size )
{
	INT x, y, width, height;
	_stscanf( size.GetBuffer( 0 ), _T("%d,%d,%d,%d"), &x, &y, &width, &height );
	
	CRect rect( x, y, x + width, y + height );
	return rect;
}

VOID CLoanCardPrinter::FreeObjectList()
{
	CSIReportObject *pObj = NULL;
	CSIReportObject_Text_Box *pTextBox = NULL;
	CSIReportObject_Barcode *pBarcode = NULL;
		
	POSITION pos = m_objList.GetHeadPosition();
	while( pos )
	{
		pObj = ( CSIReportObject * )m_objList.GetNext( pos );

		switch( pObj->GetObjType() )
		{
			case OBJECT_TEXT_BOX :
				pTextBox = ( CSIReportObject_Text_Box * )pObj;
				if( pTextBox )
				{
					delete pTextBox;
					pTextBox = NULL;
				}
				break;
			case OBJECT_BARCODE :
				pBarcode = ( CSIReportObject_Barcode * )pObj;
				if( pBarcode )
				{
					delete pBarcode;
					pBarcode = NULL;
				}
				break;
			default :
				if( pObj )
				{
					delete pObj;
					pObj = NULL;
				}
				break;
		}
	}

	m_objList.RemoveAll();
}

INT CLoanCardPrinter::SetObjectInfo( CSIReportObject *pObj, INT userIndex, INT locate, INT objIndex )
{
	CSIReportObject *pDefaultObj = ( CSIReportObject * )m_objList.GetAt( m_objList.FindIndex( objIndex ) );
	switch( pObj->GetObjType() )
	{
		case OBJECT_TEXT_BOX :
			CopyTextBox( ( CSIReportObject_Text_Box * )pObj, ( CSIReportObject_Text_Box * )pDefaultObj, userIndex, objIndex );
			break;
		case OBJECT_BARCODE :
			CopyBarCode( ( CSIReportObject_Barcode * )pObj, ( CSIReportObject_Barcode * )pDefaultObj, userIndex, objIndex );
			break;
		default :
			break;
	}
	
	CRect rect = pDefaultObj->GetRect();
	CRect newRect = CRect( 
							rect.left	+ m_startPoint[ locate ].x,
							rect.top	+ m_startPoint[ locate ].y,
							rect.right	+ m_startPoint[ locate ].x,
							rect.bottom	+ m_startPoint[ locate ].y );
	pObj->SetRect( newRect );
	
	return 0;
}

UINT CLoanCardPrinter::GetObjType( INT objIndex )
{
	CSIReportObject *pObj = ( CSIReportObject * )m_objList.GetAt( m_objList.FindIndex( objIndex ) );

	return pObj->GetObjType();
}

INT CLoanCardPrinter::CopyTextBox( CSIReportObject_Text_Box *pTextBox, CSIReportObject_Text_Box *pTB, INT userIndex, INT objIndex )
{
	pTextBox->char_len = pTB->char_len;
	pTextBox->char_max = pTB->char_max;
	pTextBox->horz_align = pTB->horz_align;
	pTextBox->vert_align = pTB->vert_align;
	pTextBox->horz_line = pTB->horz_line;
	pTextBox->vert_line = pTB->vert_line;
	pTextBox->text = pTB->text;
	
	CLoanCardFormatSetter *pParent = ( CLoanCardFormatSetter * )m_pParent;
	CESL_DataMgr *pDM = pParent->FindDM( _T("DM_LoanCardFormatSetter") );

	if( _T("Y") == pDM->GetCellData( _T("FIX"), objIndex ) )
		pTextBox->text.text = pDM->GetCellData( _T("CONTENT"), objIndex );
	else
		pTextBox->text.text = m_pDM->GetCellData( pDM->GetCellData( _T("CONTENT"), objIndex ), userIndex );
	
	return 0;
}

INT CLoanCardPrinter::CopyBarCode( CSIReportObject_Barcode *pBarCode, CSIReportObject_Barcode *pBC, INT userIndex, INT objIndex )
{
	pBarCode->SetStyle( _T("Code 3 of 9") );

	CLoanCardFormatSetter *pParent = ( CLoanCardFormatSetter * )m_pParent;
	CESL_DataMgr *pDM = pParent->FindDM( _T("DM_LoanCardFormatSetter") );
	pBarCode->SetCaption( m_pDM->GetCellData( pDM->GetCellData( _T("CONTENT"), objIndex ), userIndex ) );

	return 0;
}
