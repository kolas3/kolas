// WorkSheet.cpp: implementation of the CWorkSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkSheet.h"
#include "rgbcolor.h"
#include "winspool.h"
#include "except.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//========================================================================
// Construction/Destruction
//========================================================================
CWorkSheet::CWorkSheet()
{
ECO_EXCEPTION_BEGIN
	// 1. 레포트 정보를 초기화한다.
	InitReportInfo();
	// 2. 마우스정보를 초기화한다.
	InitMouseInfo();
	// 3. Sheet크기를 초기화한다.
	InitSheetSize();
	// 4. 캐럿정보를 초기화한다.
	InitCaretInfo();
	// 5. 레포트편집를 시작한다.
	NewSheet();
	// 6. 페이지 정보 초기화
	m_CurPage = FIRST_PAGE;
	m_nPage   = 0;
ECO_EXCEPTION_END
}

CWorkSheet::~CWorkSheet()
{
ECO_EXCEPTION_BEGIN
	// 1. 레포트정보를 Free한다.
	FreeReportInfo();
	// 2. 비트맵정보를 Free한다.
	FreeBitmap();
ECO_EXCEPTION_END
}

//========================================================================
// New
//
// PURPOSE : 레포트정보를 새로한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::New( CDC * pDC )
{
	// 1. 이전에 사용된 레포트정보를 초기화한다.
	FreeReportInfo();
	// 2. 이전에 사용된 비트맵을 초기한다.
	FreeBitmap();
	// 3. 레포트 정보를 초기화한다.
	InitReportInfo();
	// 4. 비트맵를 초기화한다.
	InitBitmap( pDC );
	// 5. 마우스정보를 초기화한다.
	InitMouseInfo();
	// 6. Sheet크기를 초기화한다.
	InitSheetSize();
	// 7. 캐럿정보를 초기화한다.
	InitCaretInfo();
	// 8. 레포트편집를 시작한다.
	NewSheet();

	return 0;
}

//========================================================================
// InitReportInfo
//
// PURPOSE : 레포트정보를 초기화한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::InitReportInfo( VOID )
{
ECO_EXCEPTION_BEGIN
	// 1. 레포트 구조체를 초기화한다.
	_tcscpy( m_ReportInfo.version, REPORT_FILE_VERSION );
	m_ReportInfo.sheet_width   = SHEET_WIDTH;
	m_ReportInfo.sheet_height  = SHEET_HEIGHT;
	m_ReportInfo.sheet_size    = 0;

	m_ReportInfo.paper         = DMPAPER_A4;
	m_ReportInfo.orientation   = DMORIENT_LANDSCAPE;
	m_ReportInfo.print_dialog  = TRUE;
	m_ReportInfo.sheet_count   = 0;
	m_ReportInfo.sheet         = NULL;

	// 2. 복사용 스풀정보를 초기화한다.
	_tcscpy( m_ClipSheet.title, _T("ClipSheet") );
	m_ClipSheet.object_count = 0;
	m_ClipSheet.object_id    = 0; 
	m_ClipSheet.object       = NULL;

	// 3. 객체복사시 위치정보를 초기화한다.
	m_CopyPoint.x = 0;
	m_CopyPoint.y = 0;

	// 4. 현재 Sheet을 초기화한다.( 최소한 Sheet는 하나이다. )
	m_CurSheet = 0;

	// 5. 객체영역Rect을 초기화한다.
	m_ObjectRect.SetRect( 0, 0, 1, 1 );

	// 6. Sheet을 중앙에 위치시키는 변수를 초기화한다.
	m_CenterWidth = 0;

	// 7. 객체선책 개수를 초기화한다.
	m_SelectCnt = 0;

	// 8. 비트맵을 초기화한다.
    m_pMasterMemDC   = NULL;
    m_pMasterBitmap  = NULL;
    m_pRestoreMemDC  = NULL;
    m_pRestoreBitmap = NULL;

	// 9. 객체 Tab Stop 인덱스를 초기화한다.
	m_TabStopObjectIdx = -1;

	// 10.현재 페이지를 초기화한다.
	m_CurPage = FIRST_PAGE;

	// 11. 디폴트 파일명을 설정한다.
	_tcscpy( m_FileName, _T("제목없음") );

	// 12. 미리보기 플래그
	m_bPreview = FALSE;

	// 13. IMAGE 개수 초기화
	m_ImageCnt = 0;

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// InitMouseInfo
//
// PURPOSE : 마우스 정보를 초기화한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::InitMouseInfo( VOID )
{
ECO_EXCEPTION_BEGIN
	m_MouseInfo.clicked        = FALSE;   // Click 상태
	m_MouseInfo.moving         = FALSE;   // 이동  상태
	m_MouseInfo.object_idx     = -1;      // 선택된 객체의 인덱스
	m_MouseInfo.pick_point     = -1;      // 사이징포인트의 종류
	m_MouseInfo.anchor_point.x = -1;      // 사이징기준 x
	m_MouseInfo.anchor_point.y = -1;      // 사이징기준 y
	m_MouseInfo.move_offset.cx =  0;      // 사이징변위 widht
	m_MouseInfo.move_offset.cy =  0;      // 사이징변위 height

	return 0;
ECO_EXCEPTION_END
}

//========================================================================
// InitSheetSize
//
// PURPOSE : Sheet크기를 초기화한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::InitSheetSize( VOID )
{
ECO_EXCEPTION_BEGIN
	// 1. 레포트정보중 Sheet의 크기정보를 초기화한다.
	m_ReportInfo.sheet_width  = SHEET_WIDTH;
	m_ReportInfo.sheet_height = SHEET_HEIGHT;

	// 2. Sheet을 중앙에 위치시키기 위한 값을 계산한다. ( 수평방향만 가운데로 위치시킨다. )
	m_CenterWidth = INT( double( SHEET_WIDTH - m_ReportInfo.sheet_width ) / 2.0 );
	if ( 0 > m_CenterWidth ) {
		m_CenterWidth = 0;
	}

	return 0;
ECO_EXCEPTION_END
}

//========================================================================
// InitCaretInfo
//
// PURPOSE : 캐럿정보를 초기화한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::InitCaretInfo( VOID )
{
	m_CaretInfo.focus        = FALSE;  // 포커스 상태
	m_CaretInfo.object_index = -1;     // 포커스가 먹은 객체 인덱스

	return 0;
}

//========================================================================
// NewSheet
//
// PURPOSE : Sheet을 하나 만든다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewSheet( VOID )
{
ECO_EXCEPTION_BEGIN
	// 1. Sheet을 추가한다.
	InsertSheet( _T("레포트_양식") );

	// 2. 첫번째 Sheet을 작업 Sheet포인터에 할당한다.
	m_pSheet = &m_ReportInfo.sheet[0];

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// FreeObjectInfo
//
// PURPOSE : 객체정보의 메모리를 Free한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::FreeObjectInfo( REPORT_SHEET * pSheet    ,  // [IN] 객체정보를 삭제할 Sheet
							    INT            ObjectIdx )  // [IN] 객체의 인덱스
{
ECO_EXCEPTION_BEGIN
	INT i, j;

	switch ( pSheet->object[ObjectIdx]->kind ) {
		// 1. 테이블객체 메모리를 Free한다.
		case OBJECT_TABLE :
			// 1.1 문자열 길이 메모리를 Free한다.
			if ( NULL != pSheet->object[ObjectIdx]->table.char_len ) {
				free( pSheet->object[ObjectIdx]->table.char_len );
			}
			// 1.2 최대 문자열 메모리를 Free한다.
			if ( NULL != pSheet->object[ObjectIdx]->table.char_max ) {
				free( pSheet->object[ObjectIdx]->table.char_max );
			}
			// 1.3 들여쓰기 메모리를 Free한다.
			if ( NULL != pSheet->object[ObjectIdx]->table.indent_len ) {
				free( pSheet->object[ObjectIdx]->table.indent_len );
			}
			// 1.4 수평정렬 메모리를 Free한다.
			if ( NULL != pSheet->object[ObjectIdx]->table.horz_align ) {
				free( pSheet->object[ObjectIdx]->table.horz_align );
			}
			// 1.5 수직정렬 메모리를 Free한다.
			if ( NULL != pSheet->object[ObjectIdx]->table.vert_align ) {
				free( pSheet->object[ObjectIdx]->table.vert_align );
			}
			// 1.6 데이터필드 메모리를 Free한다.
			if ( NULL != pSheet->object[ObjectIdx]->table.data_field ) {
				for ( i = 0; i < pSheet->object[ObjectIdx]->table.col_count; i++ ) {
					if ( NULL != pSheet->object[ObjectIdx]->table.data_field[i] ) {
						free( pSheet->object[ObjectIdx]->table.data_field[i] );
					}
				}
				free( pSheet->object[ObjectIdx]->table.data_field );
			}
			// 1.7 문자열을 메모리를 Free한다.
			for ( i = 0; i < pSheet->object[ObjectIdx]->table.row_count; i++ ) {
				for ( j = 0; j < pSheet->object[ObjectIdx]->table.col_count; j++ ) {
					if ( NULL != pSheet->object[ObjectIdx]->table.cell[i][j].text ) {
						free( pSheet->object[ObjectIdx]->table.cell[i][j].text );
					}
				}
			}
			break;
		case OBJECT_GRID  :
			for ( i = 0; i < pSheet->object[ObjectIdx]->grid.row_count; i++ ) {
				for ( j = 0; j < pSheet->object[ObjectIdx]->grid.col_count; j++ ) {
					if ( NULL != pSheet->object[ObjectIdx]->grid.block[i][j].text ) {
						free( pSheet->object[ObjectIdx]->grid.block[i][j].text );
					}
					ClearParentInfoOfChild( pSheet, ObjectIdx, i, j );
				}
			}
			break;
		case OBJECT_BITMAP :
			// 1.9 비트맵 객체일 경우 비트맵을 제거한다.
			if ( NULL != pSheet->object[ObjectIdx]->bitmap.dib ) {
				delete pSheet->object[ObjectIdx]->bitmap.dib;
			}
			break;
		case OBJECT_BARCODE :
			if ( NULL != pSheet->object[ObjectIdx]->barcode.bc ) {
				delete pSheet->object[ObjectIdx]->barcode.bc;
			}
			break;
		case OBJECT_HLINE  :
		case OBJECT_VLINE  :
		case OBJECT_BOX    :
			break;
	}
	// 부모의 자식정보를 Clear한다.
	ClearChildInfoOfParent( pSheet, ObjectIdx );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// FreeObject
//
// PURPOSE : Sheet내 객체정보의 메모리를 Free한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::FreeObject( REPORT_SHEET * pSheet )
{
ECO_EXCEPTION_BEGIN
	INT i;

	// 1. Sheet가 NULL인지 검사한다.
	if ( NULL == pSheet->object ) {
		return 0;
	}

	// 2. Sheet내 객체의 정보를 삭제한다
	for ( i = 0; i < pSheet->object_count; i++ ) {
		if ( NULL == pSheet->object[i] ) {
			continue;
		}
		FreeObjectInfo( pSheet, i );
	}

	// 3. Sheet내 객체을 Free한다.
	free( pSheet->object );
	pSheet->object = NULL;

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// FreeReportInfo
//
// PURPOSE : 레포트정보를 Free한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::FreeReportInfo( VOID )
{
ECO_EXCEPTION_BEGIN
	INT i;

	// 1. Sheet내 객체정보 메모리를 Free한다.
	if ( NULL != m_ReportInfo.sheet ) {
		for ( i = 0; i < m_ReportInfo.sheet_count; i++ ) {
			FreeObject( &m_ReportInfo.sheet[i] );
		}
		free( m_ReportInfo.sheet );
	}

	// 2. 객체복사용 스풀정보의 메모리를 Free한다.
	if ( NULL != m_ClipSheet.object ) {    // Check
		FreeObject( &m_ClipSheet );
	}

	// 3. Image File을 삭제한다.
	//DeleteImage();

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// Create
//
// PURPOSE : Owner Window의 포인터를 보관한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::Create( CWnd  * pOwnderWnd                                                                     ,  // [IN] Owner Window의 포인터
					    CRect & ClientRect                                                                     ,  // [IN] ClientRect
					    VOID ( CWnd::*fpTrackObjectPopupMenu )( CPoint & point, PROPERTY_INFO & PropertyInfo ) ,  // [IN] 객체   팝업 메뉴 함수
						VOID ( CWnd::*fpTrackEditorPopupMenu )( CPoint & point                               ) ,  // [IN] 편집기 팝업 메뉴 함수
						VOID ( CWnd::*fpPropertyDialog       )( PROPERTY_INFO & PropertyInfo                 ), 
						CWinApp * pWinApp,
						CDC     * pDC )
{
ECO_EXCEPTION_BEGIN
	// 1. Owner 윈도우로부터 정보를 전달받는다.
	m_pOwnderWnd             = pOwnderWnd;
	m_ClientRect             = ClientRect;
	m_fpTrackObjectPopupMenu = fpTrackObjectPopupMenu;
	m_fpTrackEditorPopupMenu = fpTrackEditorPopupMenu;
	m_fpPropertyDialog       = fpPropertyDialog;
	m_pWinApp                = pWinApp;

	// 2. 비트맵을 초기화한다.( 레포트 에디터일 경우 )
	if ( NULL != pDC ) {
		InitBitmap( pDC );
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// InitBitmap
//
// PURPOSE : 그래픽작업에 필요한 Bitmap을 초기화한다.
//========================================================================
INT CWorkSheet::InitBitmap( CDC * pDC ) // [IN] Bitmap을 생성시 호환용 DC
{
ECO_EXCEPTION_BEGIN
	CBitmap * pOldBitmap;
	CBrush  * pOldBrush;
	CBrush    BkBrush( WHITE );

    m_pMasterMemDC   = new CDC;
    m_pMasterBitmap  = new CBitmap;
    m_pRestoreMemDC  = new CDC;
    m_pRestoreBitmap = new CBitmap;

    m_pMasterMemDC ->CreateCompatibleDC( pDC );
    m_pMasterBitmap->CreateCompatibleBitmap( pDC, m_ReportInfo.sheet_width, m_ReportInfo.sheet_height );

    m_pRestoreMemDC ->CreateCompatibleDC( pDC );
    m_pRestoreBitmap->CreateCompatibleBitmap( pDC, m_ReportInfo.sheet_width, m_ReportInfo.sheet_height );

    pOldBitmap = ( CBitmap * )m_pMasterMemDC->SelectObject( m_pMasterBitmap );
    pOldBrush = m_pMasterMemDC->SelectObject( &BkBrush );

    m_pMasterMemDC->PatBlt( 0, 0, m_ReportInfo.sheet_width, m_ReportInfo.sheet_height, PATCOPY );

    m_pMasterMemDC->SelectObject( pOldBitmap );
    m_pMasterMemDC->SelectObject( pOldBrush  );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// DrawObjectToMemDC
// 
// PURPOSE : 모든 객체를 메모리 DC에 그린다.
//========================================================================
INT CWorkSheet::DrawObjectToMemDC( BOOL bPrinting, INT nWidth, INT nHeight, INT ObjectIndex )
{
ECO_EXCEPTION_BEGIN
	CBitmap * pOldBitmap;
    CBrush  * pOldBrush;
    CBrush    BkBrush( WHITE );

    pOldBitmap = ( CBitmap * )m_pMasterMemDC->SelectObject( m_pMasterBitmap );
    pOldBrush  = m_pMasterMemDC->SelectObject( &BkBrush );

	m_pMasterMemDC->m_bPrinting = bPrinting;
    m_pMasterMemDC->PatBlt( 0, 0, m_ReportInfo.sheet_width, m_ReportInfo.sheet_height, PATCOPY );

	if ( TRUE == bPrinting ) {
		m_pMasterMemDC->SetMapMode    ( MM_ISOTROPIC );
		m_pMasterMemDC->SetWindowExt  ( m_ReportInfo.sheet_width, m_ReportInfo.sheet_height );
		m_pMasterMemDC->SetViewportExt( nWidth, nHeight );
	}
	DrawObject( m_pMasterMemDC );

	if ( -1 != ObjectIndex ) {
		DrawTableSpriteLine( m_pMasterMemDC, ObjectIndex );
	}

    m_pMasterMemDC->SelectObject( pOldBitmap );
    m_pMasterMemDC->SelectObject( pOldBrush  );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// CopyMemDCToScreen
//
// PURPOSE : 메모리 DC의 내용을 화면 DC에 복사한다.
//========================================================================
INT CWorkSheet::CopyMemDCToScreen( CDC * pDC ) // [IN] 화면 DC의 포인터
{
ECO_EXCEPTION_BEGIN
	CBitmap * pOldBitmap;

    pOldBitmap = ( CBitmap * )m_pMasterMemDC->SelectObject( m_pMasterBitmap );

    pDC->BitBlt ( 0, 0, m_ReportInfo.sheet_width, m_ReportInfo.sheet_height, m_pMasterMemDC, 0, 0, SRCCOPY );

    m_pMasterMemDC->SelectObject( pOldBitmap );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// FreeBitmap
//
// PURPOSE : Bitmap을 제거한다.
//========================================================================
INT CWorkSheet::FreeBitmap( VOID )
{
ECO_EXCEPTION_BEGIN
    if ( m_pMasterMemDC   != NULL ) delete m_pMasterMemDC;
    if ( m_pMasterBitmap  != NULL ) delete m_pMasterBitmap;
    if ( m_pRestoreMemDC  != NULL ) delete m_pRestoreMemDC;
    if ( m_pRestoreBitmap != NULL ) delete m_pRestoreBitmap;

ECO_EXCEPTION_END
	return 0;
}


//========================================================================
// PutObjectToStore
//
// PURPOSE : 객체의 이동을 위해서 보관용 Bitmap에 객체를 보관한다.
//           이때, 설정한 객체는 Master에 그려지고, 객체가 그려질 배경이미지
//           를 Store에 그린다. 
//========================================================================
INT CWorkSheet::PutObjectToStore( INT   ObjectIdx, // [IN] 보관할 객체의 인덱스
								  INT   Existed  ) // [IN] 존재하는 객체 제외 플레그
{
ECO_EXCEPTION_BEGIN
	CBitmap * pOldBitmap1;
	CBitmap * pOldBitmap2;
	CBrush  * pOldBrush, BkBrush( WHITE );

    pOldBitmap1 = ( CBitmap * )m_pRestoreMemDC->SelectObject( m_pRestoreBitmap );
    pOldBitmap2 = ( CBitmap * )m_pMasterMemDC ->SelectObject( m_pMasterBitmap  );

	pOldBrush = m_pMasterMemDC->SelectObject( &BkBrush );
	m_pMasterMemDC->PatBlt( 0, 0, m_ReportInfo.sheet_width, m_ReportInfo.sheet_height, PATCOPY );

	if ( TRUE == Existed ) {
		DrawObject( m_pMasterMemDC, OBJECT_ALL_DRAW, OBJECT_EXCEPT_SELECTED );
	}

	m_pRestoreMemDC->BitBlt( 0, 0, m_ObjectRect.Width(), m_ObjectRect.Height(), m_pMasterMemDC, m_ObjectRect.left, m_ObjectRect.top, SRCCOPY );

    DrawObject( m_pMasterMemDC, ObjectIdx );

    m_pRestoreMemDC->SelectObject( pOldBitmap1 );
    m_pMasterMemDC->SelectObject ( pOldBitmap2 );
	m_pMasterMemDC->SelectObject ( pOldBrush   );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// AdjustSheetDC
//
// PURPOSE : 객체의 이동을 위해서 보관용 Bitmap에 객체를 보관한다.
//========================================================================
INT CWorkSheet::AdjustScrollDC( CDC * pDC, CSize * Size )
{
ECO_EXCEPTION_BEGIN
	if ( NULL == Size ) {
		pDC->SetWindowOrg( CPoint( -SHEET_BLANK_WIDTH - m_CenterWidth, -SHEET_BLANK_HEIGHT ) );
	} else {
		pDC->SetWindowOrg( CPoint( -SHEET_BLANK_WIDTH - m_CenterWidth + Size->cx, -SHEET_BLANK_HEIGHT + Size->cy ) );
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// Draw
//
// PURPOSE : Owner Window의 Drawing함수가 호출하여 양식을 그린다.
//========================================================================
INT CWorkSheet::Draw( CDC * pDC ) // [IN] 화면 DC의 포인터
{
ECO_EXCEPTION_BEGIN
	// 1. 인쇄시에는 사용되지 않게 한다.
	if ( TRUE == pDC->m_bPrinting ) {
		return 0;
	}
	// 2. Sheet 플레임을 그린다.	
	DrawSheetFrame( pDC );
	// 3. 스크롤 처리를 한다.
	AdjustScrollDC( pDC, NULL );
	// 4. 객체를 그린다.
	// WWW DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// GetObjectRect
//
// PURPOSE : 객체의 외곽 영역을 구한다.
//========================================================================
INT CWorkSheet::GetObjectRect( INT     ObjectIdx,  // [IN ] 객체의 인덱스
							   CRect & Rect     )  // [OUT] 객체의 외곽영역
{
ECO_EXCEPTION_BEGIN
	INT left, top, width, height;

	switch ( m_pSheet->object[ObjectIdx]->kind ) {
		case OBJECT_TABLE   :
		case OBJECT_GRID    :
		case OBJECT_HLINE   :
		case OBJECT_VLINE   :
		case OBJECT_BOX     :
		case OBJECT_BITMAP  :
		case OBJECT_BARCODE :
			left   = m_pSheet->object[ObjectIdx]->comm.left;
			top    = m_pSheet->object[ObjectIdx]->comm.top;
			width  = m_pSheet->object[ObjectIdx]->comm.width;
			height = m_pSheet->object[ObjectIdx]->comm.height;

			Rect.SetRect( left, top, left + width, top + height );
			break;
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// GetSelectedObjectRect
//========================================================================
INT CWorkSheet::GetSelectedObjectRect( CRect & Rect )
{
ECO_EXCEPTION_BEGIN
	INT i, MinX = m_ReportInfo.sheet_width, MaxX = 0, MinY = m_ReportInfo.sheet_height, MaxY = 0, left, top, right, bottom;

	//
	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( -1 == m_pSheet->object[i]->select ) {
			continue;
		}
		left   = m_pSheet->object[i]->comm.left;
		top    = m_pSheet->object[i]->comm.top;
		right  = left + m_pSheet->object[i]->comm.width;
		bottom = top  + m_pSheet->object[i]->comm.height;

		if ( MinX > left   ) MinX = left;
		if ( MinY > top    ) MinY = top;
		if ( MaxX < right  ) MaxX = right;
		if ( MaxY < bottom ) MaxY = bottom;
	}

	//
	Rect.SetRect( MinX, MinY, MaxX, MaxY );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// DrawGridObject
//
// PURPOSE : 객체를 그린다.
//
//========================================================================
INT CWorkSheet::DrawGridBlock( CDC * pDC, INT ObjectIdx, INT RowIdx, INT ColIdx, CPoint & Point )
{
ECO_EXCEPTION_BEGIN
	INT     Left, Top, Right, Bottom, OldBkMode;
	CPen    HiddenPen,   VisiblePen,            * pOldPen   = NULL;
	CBrush  NormalBrush, FocusBrush, FillBrush, * pOldBrush = NULL;
	UINT    Align, DT_Align;
	COLORREF OldColor;

	// 블럭영역을 구한다.
	Left   = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].left   + Point.x;
	Top    = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].top    + Point.y;
	Right  = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].width  + Left;
	Bottom = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].height + Top;

	// 펜을 생성한다.
	VisiblePen. CreatePen       ( 0, 0, BLACK );
	HiddenPen.  CreatePen       ( 2, 0, RED   );
	NormalBrush.CreateSolidBrush( WHITE );
	FocusBrush. CreateSolidBrush( BLACK );
	FillBrush.  CreateSolidBrush( YELLOW );

	// 바인드된 슬레이브 블럭은 그리지 않는다.
	if ( BIND_STATUS_SLAVE == m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].bind_status ) {
		return 0;
	}

	// 브러시 결정
	if ( TRUE == m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].fill ) {
		pOldBrush = pDC->SelectObject( &FillBrush ); 
	} else {
		//pOldBrush = pDC->SelectObject( &NormalBrush );
		//++ 투명처리
		if ( -1 == m_pSheet->object[ObjectIdx]->select ) {
			/*KINITI*/ pDC->SelectStockObject( NULL_BRUSH );
			//pDC->SelectStockObject( WHITE_BRUSH );
		} else {
			pOldBrush = pDC->SelectObject( &NormalBrush );
		}
		//--
	}

	// 블럭경계선
	if ( TRUE == m_pSheet->object[ObjectIdx]->grid.horz_line && TRUE == m_pSheet->object[ObjectIdx]->grid.vert_line ) {
		pOldPen = pDC->SelectObject( &VisiblePen );
		EcoRectangle( pDC,  Left, Top, Right, Bottom );
		pDC->SelectObject( pOldPen   );
	} else {
		if ( TRUE == m_pSheet->object[ObjectIdx]->grid.horz_line ) { // 수평선만선보기
			pDC->SelectStockObject( NULL_PEN );
			EcoRectangle( pDC,  Left + 1, Top + 1, Right, Bottom ); // 블럭내부 채우기

			pOldPen = pDC->SelectObject( &VisiblePen );
			pDC->MoveTo( Left, Top        ); pDC->LineTo( Right, Top        );
			pDC->MoveTo( Left, Bottom - 1 ); pDC->LineTo( Right, Bottom - 1 );
			pDC->SelectObject( pOldPen );

			if ( FALSE == pDC->m_bPrinting ) {
				pOldPen = pDC->SelectObject( &HiddenPen );
				pDC->MoveTo( Left,      Top ); pDC->LineTo( Left,      Bottom );
				pDC->MoveTo( Right - 1, Top ); pDC->LineTo( Right - 1, Bottom );
				pDC->SelectObject( pOldPen );
			}
		} else 
			if ( TRUE == m_pSheet->object[ObjectIdx]->grid.vert_line ) {              // 수직선만선보기
				pDC->SelectStockObject( NULL_PEN );
				EcoRectangle( pDC,  Left + 1, Top + 1, Right, Bottom ); // 블럭내부 채우기

				pOldPen = pDC->SelectObject( &VisiblePen );
				pDC->MoveTo( Left,      Top ); pDC->LineTo( Left,      Bottom );
				pDC->MoveTo( Right - 1, Top ); pDC->LineTo( Right - 1, Bottom );
				pDC->SelectObject( pOldPen );

				if ( FALSE == pDC->m_bPrinting ) {
					pOldPen = pDC->SelectObject( &HiddenPen );
					pDC->MoveTo( Left, Top        ); pDC->LineTo( Right, Top        );
					pDC->MoveTo( Left, Bottom - 1 ); pDC->LineTo( Right, Bottom - 1 );
					pDC->SelectObject( pOldPen );
				}
			} else {                               // 사각은 은선
				pDC->SelectStockObject( NULL_PEN );
				EcoRectangle( pDC,  Left + 1, Top + 1, Right, Bottom ); // 블럭내부 채우기

				if ( FALSE == pDC->m_bPrinting ) {
					pOldPen = pDC->SelectObject( &HiddenPen );
					pDC->MoveTo( Left,      Top ); pDC->LineTo( Left,      Bottom );
					pDC->MoveTo( Right - 1, Top ); pDC->LineTo( Right - 1, Bottom );
					pDC->MoveTo( Left, Top        ); pDC->LineTo( Right, Top        );
					pDC->MoveTo( Left, Bottom - 1 ); pDC->LineTo( Right, Bottom - 1 );
					pDC->SelectObject( pOldPen );
				}
		}
	}
	//
	if ( NULL != pOldBrush ) {
		pDC->SelectObject( pOldBrush );
	}
	// 커서 그리기
	if ( TRUE == m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].focus && FALSE == pDC->m_bPrinting ) { //++
		pOldPen   = pDC->SelectObject( &VisiblePen );
		pOldBrush = pDC->SelectObject( &FocusBrush ); 
		if ( TRUE == m_pSheet->object[ObjectIdx]->grid.bind && RowIdx == m_pSheet->object[ObjectIdx]->grid.cursor_row && ColIdx == m_pSheet->object[ObjectIdx]->grid.cursor_col ) {
			EcoRectangle( pDC,  Left + 4, Top + 4, Right - 4, Bottom - 4 );
		} else {
			EcoRectangle( pDC,  Left + 2, Top + 2, Right - 2, Bottom - 2);
		}
		pDC->SelectObject( pOldPen   ); 
		pDC->SelectObject( pOldBrush ); 
	}
	// 사선을 그린다.
	if ( SLASH_NONE != m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].slash ) {
		pOldPen = pDC->SelectObject( &VisiblePen );
		switch ( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].slash ) {
			case SLASH_1 :
				pDC->MoveTo( Right, Top    );
				pDC->LineTo( Left,  Bottom );
				break;
			case SLASH_2 :
				pDC->MoveTo( Left,  Top    );
				pDC->LineTo( Right, Bottom );
				break;
		}
		pDC->SelectObject( pOldPen );
	}
	// 블럭 문자열
	if ( NULL != m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text ) {
		// Font
		CFont Font, * pOldFont = NULL;
		Font.CreateFontIndirect( &m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].font );
		pOldFont = pDC->SelectObject( &Font );

		if ( TRUE == m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].fill ) {
			OldColor = pDC->SetBkColor( YELLOW );
		} else {
			OldColor = pDC->SetBkColor( WHITE );
		}

		// Align
		switch ( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].horz_align ) {
			case ALGIN_LEFT    : Align = DT_LEFT;   break;
			case ALGIN_HCENTER : Align = DT_CENTER; break;
			case ALGIN_RIGHT   : Align = DT_RIGHT;  break;
		}
		switch ( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].orientation ) {
			case TEXT_ORIENTATION_HORZ :
				DT_Align = Align | DT_VCENTER | DT_SINGLELINE;
				break;
			case TEXT_ORIENTATION_VERT :
				DT_Align = Align/* & ~DT_CENTER*/;
				break;
		}
		// Draw Text
		if ( -1 == m_pSheet->object[ObjectIdx]->select ) {
			OldBkMode = pDC->SetBkMode( TRANSPARENT );
		} else {
			OldBkMode = pDC->SetBkMode( OPAQUE );
		}
        switch ( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].horz_align ) {
            case ALGIN_LEFT    : pDC->DrawText( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text, StringLength( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text ), CRect( Left + 3, Top + 2, Right - 1, Bottom ), DT_Align ); break;
            case ALGIN_HCENTER : pDC->DrawText( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text, StringLength( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text ), CRect( Left + 1, Top + 2, Right - 1, Bottom ), DT_Align ); break;
            case ALGIN_RIGHT   : pDC->DrawText( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text, StringLength( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text ), CRect( Left + 1, Top + 2, Right - 3, Bottom ), DT_Align ); break;
        }

		//pDC->DrawText( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text, StringLength( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].text ), CRect( Left + 1, Top + 2, Right - 1, Bottom ), DT_Align );
		pDC->SetBkColor( OldColor );
		pDC->SetBkMode( OldBkMode );

		if ( NULL != pOldFont ) {
			pDC->SelectObject( pOldFont );
		}
	}
	// 2000.1.16
	if ( FALSE == pDC->m_bPrinting ) {
		// Font
		CFont Font, * pOldFont = NULL;
		Font.CreateFontIndirect( &m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].font );
		pOldFont = pDC->SelectObject( &Font );

		if ( TRUE == m_pSheet->object[ObjectIdx]->grid.block[RowIdx][ColIdx].fill ) {
			OldColor = pDC->SetBkColor( YELLOW );
		} else {
			OldColor = pDC->SetBkColor( WHITE );
		}

		// Draw Text
		if ( -1 == m_pSheet->object[ObjectIdx]->select ) {
			OldBkMode = pDC->SetBkMode( TRANSPARENT );
		} else {
			OldBkMode = pDC->SetBkMode( OPAQUE );
		}

		if ( RowIdx == m_pSheet->object[ObjectIdx]->grid.page_row ) {
			pDC->DrawText( _T("page"), StringLength( _T("page") ), CRect( Left + 1, Top + 2, Right - 1, Bottom ), DT_VCENTER | DT_SINGLELINE | DT_CENTER );
		}
		if ( RowIdx == m_pSheet->object[ObjectIdx]->grid.last_row ) {
			pDC->DrawText( _T("last"), StringLength( _T("last") ), CRect( Left + 1, Top + 2, Right - 1, Bottom ), DT_VCENTER | DT_SINGLELINE | DT_CENTER );	
		}

		pDC->SetBkColor( OldColor );
		pDC->SetBkMode( OldBkMode );

		if ( NULL != pOldFont ) {
			pDC->SelectObject( pOldFont );
		}
	}
ECO_EXCEPTION_END	
	return 0;
}

//========================================================================
// DrawTableObject
//========================================================================
INT CWorkSheet::DrawTableObject( CDC * pDC, INT ObjectIdx, CPoint & Point )
{
ECO_EXCEPTION_BEGIN
    INT   i, j, ParentIdx, BlockRow, BlockCol;
    CFont Font, * pOldFont = NULL;

    if ( TRUE == pDC->m_bPrinting ) {
        if ( TRUE == GetParentInfo( m_pSheet, ObjectIdx, &ParentIdx, &BlockRow, &BlockCol ) ) {
            // 페이지 보기 처리
            if ( m_pSheet->object[ParentIdx]->grid.page_row != m_pSheet->object[ObjectIdx]->table.block_row ) {
                // 마지막 보기 처리
                if ( m_pSheet->object[ParentIdx]->grid.last_row == m_pSheet->object[ObjectIdx]->table.block_row ) {
                    if ( m_CurPage != END_PAGE ) {
                        return 0;
                    }
                }
            }
        } else {
            if ( m_nPage != 1 && m_CurPage != FIRST_PAGE && m_pSheet->object[ObjectIdx]->table.visible != VISIBLE_ALL ) {
                return 0;
            }
        }
    }

    // Font Setting
    Font.CreateFontIndirect( &m_pSheet->object[ObjectIdx]->table.font );
    pOldFont = pDC->SelectObject( &Font );

    // Draw Cell
    for ( i = 0; i < m_pSheet->object[ObjectIdx]->table.row_count; i++ ) {
        for ( j = 0; j < m_pSheet->object[ObjectIdx]->table.col_count; j++ ) {
            DrawTableCell( pDC, ObjectIdx, i, j, Point );
        }
    }

    //
    pDC->SelectObject( pOldFont );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// DrawGridObject
//========================================================================
INT CWorkSheet::DrawGridObject( CDC * pDC, INT ObjectIdx, CPoint & Point )
{
ECO_EXCEPTION_BEGIN

    INT i, j, ParentIdx, BlockRow, BlockCol;;

    if ( TRUE == GetParentInfo( m_pSheet, ObjectIdx, &ParentIdx, &BlockRow, &BlockCol ) ) {
        if ( TRUE == pDC->m_bPrinting ) {
            // 페이지 보기 처리
                if ( 0 == _tcsicmp( m_pSheet->object[ParentIdx]->grid.name, m_pSheet->object[ObjectIdx]->grid.parent_name ) ) {
                    if ( m_pSheet->object[ParentIdx]->grid.page_row != m_pSheet->object[ObjectIdx]->grid.block_row ) {
                        // 마지막 보기 처리
                            if ( m_pSheet->object[ParentIdx]->grid.last_row == m_pSheet->object[ObjectIdx]->grid.block_row ) {
                                if ( m_CurPage != END_PAGE ) {
                                    return 0;
                                }
                            }
                        }
                }
        }
        for ( i = 0; i < m_pSheet->object[ObjectIdx]->grid.row_count; i++ ) {
            for ( j = 0; j < m_pSheet->object[ObjectIdx]->grid.col_count; j++ ) {
                DrawGridBlock( pDC, ObjectIdx, i, j, Point );
            }
        }
    } else {
        for ( i = 0; i < m_pSheet->object[ObjectIdx]->grid.row_count; i++ ) {
            if ( m_nPage != 1 && m_CurPage != FIRST_PAGE && VISIBLE_ALL != m_pSheet->object[ObjectIdx]->grid.visible ) {
                return 0;
            }

            // 마지막 ROW
            if ( TRUE == pDC->m_bPrinting ) {
                if ( i == m_pSheet->object[ObjectIdx]->grid.last_row && 0 < m_pSheet->object[ObjectIdx]->grid.last_row ) {
                    if ( m_CurPage != END_PAGE ) {
                        continue;
                    }
                }
            }

            for ( j = 0; j < m_pSheet->object[ObjectIdx]->grid.col_count; j++ ) {
                DrawGridBlock( pDC, ObjectIdx, i, j, Point );
            }
        }
    }
ECO_EXCEPTION_END

return 0;
}

//========================================================================
// DrawTableObject
//
// PURPOSE : 객체를 그린다.
//========================================================================
#ifdef _UNICODE
	#define CHAR_SIZE( x )		1
	#define IS_HAN( a, b )		( ( 0xff00 & a ) && ( 0xff00 & a ) ? 2 : 1 )
#else
	#define CHAR_SIZE( x )		(x)
	#define IS_HAN( a, b )		( ( 0x80   & a ) && ( 0x80   & b ) ? 2 : 1 )
#endif

INT CWorkSheet::LineSplite( CString        strSrc,     // [IN ] 원본 스트링
							INT            nCharLen ,  // [IN ] 라인당 문자수 
							CStringArray & strArray )  // [OUT] 라인별 스트링
{
	INT nStrIdx = 0, nCharIdx = 0, nCharSize, nBuddy;
	CString strLine;

	if ( nCharLen < 2 ) // 최소 두자 이상
		return -1;

	if ( strSrc.IsEmpty() )
		return -2;

	while ( true )
	{
		// 문자당 화면크기 설정
		nBuddy = ( nStrIdx >= strSrc.GetLength()-1 ) ? 0 : 1;
		nCharSize = IS_HAN( strSrc[nStrIdx], strSrc[nStrIdx + nBuddy] );
		// 한라인이 Full
		if ( nCharIdx + nCharSize > nCharLen )
		{
			// 한라인 생성
			strArray.Add( strLine );
			strLine.Empty();
			nCharIdx = 0;
		} else {
			// 한문자 설정
			if ( 0 != nCharIdx || ' ' != strSrc[nStrIdx] ) // 첫짜가 공백이면
			{
				strLine  += strSrc.Mid( nStrIdx, CHAR_SIZE( nCharSize ) );
				nCharIdx += nCharSize;
			}
			nStrIdx += CHAR_SIZE( nCharSize );
		}
		// 문자열 순회종료
		if ( nStrIdx >= strSrc.GetLength() ) {
			strArray.Add( strLine );
			break;
		}
	}

	return 0;
}

VOID str_rtrim( _TCHAR * strln, _TCHAR * strline1, _TCHAR var	)
{
ECO_EXCEPTION_BEGIN
	INT i = 0;

    _tcscpy( strline1, strln );
#ifdef _UNICODE
    for ( i = _tcsclen(strln)-1; i >= 0; i-- ){
#elif _MBCS
    for ( i = _tcsclen(strln)-1; i >= 0; i-- ){
#endif
        if(strln[i] != var) {
            strline1[i+1] = '\0';
            return;
        }
    }
    strline1[0] = '\0';
ECO_EXCEPTION_END
}

static _TCHAR     LineText[50001];

INT CWorkSheet::DrawTableCell( CDC * pDC, INT ObjectIdx, INT RowIdx, INT ColIdx, CPoint & Point, INT * RowHeight )
{
ECO_EXCEPTION_BEGIN
	INT      Left, Top, Right, Bottom, TextHeight, TextLeft, TextTop, LineCnt, LineRest, MaxCharLineCnt;
	INT      text_len, offset, white, OldBkMode;
	CPen     HiddenPen,   VisiblePen,            * pOldPen   = NULL;
	CBrush   NormalBrush, FocusBrush, FillBrush, * pOldBrush = NULL;
	COLORREF OldColor;
	CRect    TextRect;
	CString strTemp;
	CStringArray strArray;

	wmemset( LineText, 0, 50001 );

	// 블럭영역을 구한다.
	Left   = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].left   + Point.x;
	Top    = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].top    + Point.y;
	Right  = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].width  + Left;
	Bottom = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].height + Top;

	// 펜을 생성한다.
	VisiblePen. CreatePen       ( 0, 0, BLACK );
	HiddenPen.  CreatePen       ( 2, 0, RED   );
	NormalBrush.CreateSolidBrush( WHITE  );
	FocusBrush. CreateSolidBrush( BLACK  );
	FillBrush.  CreateSolidBrush( YELLOW );

 	// 브러시 결정
	if ( TRUE == m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].fill ) {
		pOldBrush = pDC->SelectObject( &FillBrush ); 
	} else {
		//pOldBrush = pDC->SelectObject( &NormalBrush );
		//++ 투명처리
		if ( -1 == m_pSheet->object[ObjectIdx]->select ) {
			/*KINITI*/ pDC->SelectStockObject( NULL_BRUSH );
			//pDC->SelectStockObject( WHITE_BRUSH );
		} else {
			pOldBrush = pDC->SelectObject( &NormalBrush );
		}
		//--
	}

	// 블럭경계선
	if ( TRUE == m_pSheet->object[ObjectIdx]->table.horz_line && TRUE == m_pSheet->object[ObjectIdx]->table.vert_line ) {
		pOldPen = pDC->SelectObject( &VisiblePen );
		if ( NULL == RowHeight ) {
			EcoRectangle( pDC,  Left, Top, Right, Bottom );
		}	
		pDC->SelectObject( pOldPen );
	} else {
		if ( TRUE == m_pSheet->object[ObjectIdx]->table.horz_line ) { // 수평선만선보기
			if ( NULL == RowHeight ) {
				pDC->SelectStockObject( NULL_PEN );
				if ( NULL == RowHeight ) {
					EcoRectangle( pDC,  Left + 1, Top + 1, Right, Bottom ); // 블럭내부 채우기
				}

				pOldPen = pDC->SelectObject( &VisiblePen );
				if ( NULL == RowHeight ) {
					pDC->MoveTo( Left, Top        ); pDC->LineTo( Right, Top        );
					pDC->MoveTo( Left, Bottom - 1 ); pDC->LineTo( Right, Bottom - 1 );
				}
				pDC->SelectObject( pOldPen );

				if ( FALSE == pDC->m_bPrinting ) {
					pOldPen = pDC->SelectObject( &HiddenPen );
					if ( NULL == RowHeight ) {
						pDC->MoveTo( Left,      Top ); pDC->LineTo( Left,      Bottom );
						pDC->MoveTo( Right - 1, Top ); pDC->LineTo( Right - 1, Bottom );
					}
					pDC->SelectObject( pOldPen );
				}
			}
		} else 
			if ( TRUE == m_pSheet->object[ObjectIdx]->table.vert_line ) {              // 수직선만선보기
				pDC->SelectStockObject( NULL_PEN );
				if ( NULL == RowHeight ) {
					EcoRectangle( pDC,  Left + 1, Top + 1, Right, Bottom ); // 블럭내부 채우기
				}

				pOldPen = pDC->SelectObject( &VisiblePen );
				if ( NULL == RowHeight ) {
					pDC->MoveTo( Left,      Top ); pDC->LineTo( Left,      Bottom );
					pDC->MoveTo( Right - 1, Top ); pDC->LineTo( Right - 1, Bottom );
				}
				pDC->SelectObject( pOldPen );

				if ( FALSE == pDC->m_bPrinting ) {
					pOldPen = pDC->SelectObject( &HiddenPen );
					if ( NULL == RowHeight ) {
						pDC->MoveTo( Left, Top        ); pDC->LineTo( Right, Top        );
						pDC->MoveTo( Left, Bottom - 1 ); pDC->LineTo( Right, Bottom - 1 );
					}
					pDC->SelectObject( pOldPen );
				}
			} else {                               // 사각은 은선
				pDC->SelectStockObject( NULL_PEN );
				if ( NULL == RowHeight ) {
					EcoRectangle( pDC,  Left + 1, Top + 1, Right, Bottom ); // 블럭내부 채우기
				}

				if ( FALSE == pDC->m_bPrinting ) {
					pOldPen = pDC->SelectObject( &HiddenPen );
					if ( NULL == RowHeight ) {
						pDC->MoveTo( Left,      Top ); pDC->LineTo( Left,      Bottom );
						pDC->MoveTo( Right - 1, Top ); pDC->LineTo( Right - 1, Bottom );
						pDC->MoveTo( Left, Top        ); pDC->LineTo( Right, Top        );
						pDC->MoveTo( Left, Bottom - 1 ); pDC->LineTo( Right, Bottom - 1 );
					}
					pDC->SelectObject( pOldPen );
				}
		}
	}
	//
	if ( NULL != pOldBrush ) {
		pDC->SelectObject( pOldBrush );
	}

	// 커서 그리기
	if ( TRUE == m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].focus && FALSE == pDC->m_bPrinting ) { // ++
		pOldPen   = pDC->SelectObject( &VisiblePen );
		pOldBrush = pDC->SelectObject( &FocusBrush ); 
		if ( TRUE == m_pSheet->object[ObjectIdx]->table.bind && RowIdx == m_pSheet->object[ObjectIdx]->table.cursor_row && ColIdx == m_pSheet->object[ObjectIdx]->table.cursor_col ) {
			if ( NULL == RowHeight ) {
				EcoRectangle( pDC,  Left + 4, Top + 4, Right - 4, Bottom - 4 );
			}
		} else {
			if ( NULL == RowHeight ) {
				EcoRectangle( pDC,  Left + 2, Top + 2, Right - 2, Bottom - 2);
			}
		}
		pDC->SelectObject( pOldPen   ); 
		pDC->SelectObject( pOldBrush ); 
	}

	// 라인수
	LineCnt = m_pSheet->object[ObjectIdx]->table.line_max;
	if ( NULL != m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].text ) {
		INT i, real_len;
		//++ \n 처리
		text_len = StringLength( m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].text );
		offset   = 0;
		real_len = 0;
		for ( i = 0; i < text_len; i++ ) {
			if ( 13 != m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].text[i] || 10 != m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].text[i+1] ) {
				LineText[offset] = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].text[i];
				offset++;
				real_len++;
				if ( real_len >= m_pSheet->object[ObjectIdx]->table.char_len[ColIdx] ) {
					real_len = 0;
				}
			} else {
				//if ( real_len > 0 ) { // [ 시작이 new line인것을 제거 ]
				white = m_pSheet->object[ObjectIdx]->table.char_len[ColIdx] - real_len;
				for ( INT j = 0; j < white; j++ ) {
					LineText[offset+j] = ' ';
				}
				offset += white;
				//}
				i++;
				real_len = 0;
			}
		}
		//-- \n 처리
		// 한글

		strTemp = LineText;
		LineSplite( strTemp, m_pSheet->object[ObjectIdx]->table.char_len[ColIdx], strArray );

		LineCnt  = strArray.GetSize();
		if ( LineCnt > 0 )
			LineRest = strArray.GetAt(strArray.GetSize()-1).GetLength();
		else
			LineRest = 0;
	}
	
	TextHeight = m_pSheet->object[ObjectIdx]->table.char_cy * m_pSheet->object[ObjectIdx]->table.line_max + ( m_pSheet->object[ObjectIdx]->table.line_max - 1 ) * m_pSheet->object[ObjectIdx]->table.char_cy / 2;
	if ( TRUE == m_pSheet->object[ObjectIdx]->table.line_expand ) {
		if ( ( LineCnt - 1 ) * m_pSheet->object[ObjectIdx]->table.char_len[ColIdx] + LineRest > m_pSheet->object[ObjectIdx]->table.char_max[ColIdx] ) {
			MaxCharLineCnt = m_pSheet->object[ObjectIdx]->table.char_max[ColIdx] / m_pSheet->object[ObjectIdx]->table.char_len[ColIdx];
			if ( m_pSheet->object[ObjectIdx]->table.char_max[ColIdx] % m_pSheet->object[ObjectIdx]->table.char_len[ColIdx] ) {
				MaxCharLineCnt++;
			}
			if ( m_pSheet->object[ObjectIdx]->table.line_max > MaxCharLineCnt ) {
				MaxCharLineCnt = m_pSheet->object[ObjectIdx]->table.line_max;
			}
			TextHeight = m_pSheet->object[ObjectIdx]->table.char_cy * MaxCharLineCnt + ( MaxCharLineCnt - 1 ) * m_pSheet->object[ObjectIdx]->table.char_cy / 2;
		} else {
			TextHeight = m_pSheet->object[ObjectIdx]->table.char_cy * LineCnt + ( LineCnt - 1 ) * m_pSheet->object[ObjectIdx]->table.char_cy / 2;			
		}
	}

	// 문자열 상자
	pOldPen = pDC->SelectObject( &HiddenPen );
	pDC->SelectStockObject( NULL_BRUSH );

	TextRect.SetRect( 0, 
					  0, 
					  m_pSheet->object[ObjectIdx]->table.char_cx * m_pSheet->object[ObjectIdx]->table.char_len[ColIdx] + m_pSheet->object[ObjectIdx]->table.char_cx * m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx],
					  TextHeight );

	//++
	if ( NULL != RowHeight ) {
		*RowHeight = TextRect.Height() + m_pSheet->object[ObjectIdx]->table.char_cy;
	}
	//--
	
	// Horizontal Align
	switch ( m_pSheet->object[ObjectIdx]->table.horz_align[ColIdx] ) {
		case ALGIN_LEFT    : TextLeft = Left + INT( m_pSheet->object[ObjectIdx]->table.char_cx / 2 ); break;
		case ALGIN_HCENTER : TextLeft = Left + INT( ( ( Right - Left ) - TextRect.Width() ) / 2.0 ); break;
		case ALGIN_RIGHT   : TextLeft = Left + ( Right - Left - 2) - TextRect.Width();               break;
	}
	// Vertical Align
	switch ( m_pSheet->object[ObjectIdx]->table.vert_align[ColIdx] ) {
		case ALGIN_TOP     : TextTop = Top + INT( m_pSheet->object[ObjectIdx]->table.char_cy / 2 );   break;
		case ALGIN_VCENTER : TextTop = Top + INT( ( ( Bottom  - Top ) - TextRect.Height() ) / 2.0 ); break;
		case ALGIN_BOTTOM  : TextTop = Top + ( Bottom  - Top - 2 ) - TextRect.Height();              break;
	}
	// Text Out
	if ( TRUE == m_pSheet->object[ObjectIdx]->table.cell[RowIdx][ColIdx].fill ) {
		OldColor = pDC->SetBkColor( YELLOW );
	} else {
		OldColor = pDC->SetBkColor( WHITE );
	}

	for ( INT i = 0; i < strArray.GetSize(); i++ ) {
		if ( FALSE == m_pSheet->object[ObjectIdx]->table.line_expand && i >= m_pSheet->object[ObjectIdx]->table.line_max ) {
			break;
		}
		if ( i * m_pSheet->object[ObjectIdx]->table.char_len[ColIdx] + LineRest > m_pSheet->object[ObjectIdx]->table.char_max[ColIdx] ) {
			break;
		}

		if ( NULL == RowHeight ) {
			INT len, disp_width, align_left;

			len = strArray.GetAt(i).GetLength();

			switch ( m_pSheet->object[ObjectIdx]->table.horz_align[ColIdx] ) {
				case ALGIN_LEFT    : 
					align_left =  0;
					break;
				case ALGIN_HCENTER : 
					//str_rtrim( tempText, tempText, ' ' );
					strArray.GetAt(i).TrimRight( ' ' );
					len        = strArray.GetAt(i).GetLength();
					disp_width = len * m_pSheet->object[ObjectIdx]->table.char_cx + ( m_pSheet->object[ObjectIdx]->table.char_cx * m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx] );
					align_left = INT( (TextRect.Width() - disp_width ) / 2.0 ); 
					break;
				case ALGIN_RIGHT   : 
					disp_width = len * m_pSheet->object[ObjectIdx]->table.char_cx + 2 * ( m_pSheet->object[ObjectIdx]->table.char_cx * m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx] );
					align_left = TextRect.Width() - disp_width;
					break;
			}

			// 2000.1.16 // if ( /*FALSE == pDC->m_bPrinting &&*/ 0 != _tcscmp( tempText, _T("last") ) && 0 != _tcscmp( tempText, _T("page") ) ) {
			// ++
			if ( -1 == m_pSheet->object[ObjectIdx]->select ) {
				OldBkMode = pDC->SetBkMode( TRANSPARENT );
			} else {
				OldBkMode = pDC->SetBkMode( OPAQUE );
			}
			//--
			pDC->TextOut( TextLeft + align_left + m_pSheet->object[ObjectIdx]->table.char_cx * m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx], 
						  TextTop  + i * INT( m_pSheet->object[ObjectIdx]->table.char_cy + m_pSheet->object[ObjectIdx]->table.char_cy / 2.0 ),
						  strArray.GetAt(i) );

			//
			pDC->SetBkMode( OldBkMode );
			// 2000.1.16 // }
		}
	}

	//
	pDC->SetBkColor( OldColor );

	if ( 0 == RowIdx && FALSE == pDC->m_bPrinting ) { //++
		// Text Rectangle
		if ( NULL == RowHeight ) {
			EcoRectangle( pDC,  TextLeft, TextTop, TextLeft + TextRect.Width(),	TextTop + TextRect.Height() );
		}
	}

	pDC->SelectObject( pOldPen );

ECO_EXCEPTION_END
	return 0;
}

/*
	INT  i, j, Left, Top, Right, Bottom;
	CPen   Pen, * pOldPen;
	CBrush WhiteBrush, BlackBrush, * pOldBrush;

	//
	Pen.CreatePen( 0, 1, BLACK );
	WhiteBrush.CreateSolidBrush( WHITE );
	BlackBrush.CreateSolidBrush( BLACK );

	pOldPen = pDC->SelectObject( &Pen );

	//
	for ( i = 0; i < m_pSheet->object[ObjectIdx]->table.row_count; i++ ) {
		for ( j = 0; j < m_pSheet->object[ObjectIdx]->table.col_count; j++ ) {
			Left   = m_pSheet->object[ObjectIdx]->table.cell[i][j].left   + Point.x;
			Top    = m_pSheet->object[ObjectIdx]->table.cell[i][j].top    + Point.y;
			Right  = m_pSheet->object[ObjectIdx]->table.cell[i][j].width  + Left;
			Bottom = m_pSheet->object[ObjectIdx]->table.cell[i][j].height + Top;

			switch ( m_pSheet->object[ObjectIdx]->table.cell[i][j].focus ) {
				case TRUE  : 
					pOldBrush = pDC->SelectObject( &BlackBrush ); 
					EcoRectangle( pDC,  Left + 2 , Top + 2, Right - 2, Bottom - 2);
					break;
				case FALSE : 
					pOldBrush = pDC->SelectObject( &WhiteBrush ); 
					EcoRectangle( pDC,  Left, Top, Right, Bottom );
					break;
			}
			//
			pDC->DrawText ( _T("T\n\nT"), StringLength( _T("T\n\nT") ), CRect( Left + 1, Top + 1, Right - 1, Bottom - 1 ), DT_CENTER | DT_VCENTER | DT_SINGLELINE );
			//
			pDC->SelectObject( pOldBrush );
		}
	}

	//
	pDC->SelectObject( pOldPen );
*/

//========================================================================
// DrawTableSpriteLine
//========================================================================
/*
INT CWorkSheet::DrawTableSpriteLine( CDC * pDC, INT ObjectIndex )
{
ECO_EXCEPTION_BEGIN
	INT     i, Top, Bottom,  Height, TableTop, TableLeft, Left, Right, Row, Col, ParentIdx, BlockRow, BlockCol;
	CPen    Pen,  * pOldPen;
	CFont   Font, * pOldFont;
	CSize   FontSize;

	//========================================================================
	// 1. 폰트를 높이를 구한다.
	//========================================================================
	GetCharSize( pDC, m_pSheet->object[ObjectIndex]->table.font, FontSize );
	Font.CreateFontIndirect( &m_pSheet->object[ObjectIndex]->table.font );
	pOldFont = pDC->SelectObject( &Font ); 

	//========================================================================
	// 2. 선을 생성한다.
	//========================================================================
	Pen.CreatePen( 0, 1, BLACK );
	pOldPen = pDC->SelectObject( &Pen );

	//========================================================================
	// 3. 여백의 좌표를 구한다.
	//========================================================================
	TableTop  = m_pSheet->object[ObjectIndex]->table.top;
	TableLeft = m_pSheet->object[ObjectIndex]->table.left;
	Row       = m_pSheet->object[ObjectIndex]->table.row_count - 1;
	Col       = m_pSheet->object[ObjectIndex]->table.col_count - 1;

	ParentIdx = m_pSheet->object[ObjectIndex]->table.parent_index;
    if ( -1 == ParentIdx ) {
        return -1;
    }

	BlockRow  = m_pSheet->object[ObjectIndex]->table.block_row;
	BlockCol  = m_pSheet->object[ObjectIndex]->table.block_col;
	Bottom    = m_pSheet->object[ParentIdx]->grid.top + m_pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].top + m_pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].height;
	Top       = TableTop  + m_pSheet->object[ObjectIndex]->table.cell[Row][0  ].top  + m_pSheet->object[ObjectIndex]->table.cell[Row][0  ].height;
	Left      = TableLeft + m_pSheet->object[ObjectIndex]->table.cell[Row][0  ].left;
	Right     = TableLeft + m_pSheet->object[ObjectIndex]->table.cell[Row][Col].left + m_pSheet->object[ObjectIndex]->table.cell[Row][Col].width;

	//========================================================================
	// 4. 여백의 높이를 구한다.
	//========================================================================
	Height = Bottom - Top;

	//========================================================================
	// 5. 여백을 그린다.
	//========================================================================
	if ( 5 * FontSize.cy < Height ) {
		// 6.1 '이하여백'을 표시한다.
		pDC->MoveTo( Left,      Top );
		pDC->LineTo( Right - 1, Top );

		pDC->DrawText( _T("이  하  여  백"), CRect( Left, Top, Right, Bottom ), DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	} else {
		//pDC->MoveTo( Left  + 1, Top );
		//pDC->LineTo( Right - 1, Top );
		// 6.2 선으로 연결한다.
		for ( i = 1; i < m_pSheet->object[ObjectIndex]->table.col_count; i++ ) {
			pDC->MoveTo( TableLeft + m_pSheet->object[ObjectIndex]->table.cell[Row][i].left, Top       );
			pDC->LineTo( TableLeft + m_pSheet->object[ObjectIndex]->table.cell[Row][i].left, Bottom - 1);
		}
	}

	pDC->SelectObject( pOldPen  );
	pDC->SelectObject( pOldFont );

	return 0;
ECO_EXCEPTION_END
}
*/

INT CWorkSheet::DrawTableSpriteLine( CDC * pDC, INT ObjectIndex )
{
ECO_EXCEPTION_BEGIN
	INT     i, Top, Bottom,  Height, TableTop, TableLeft, Left, Right, Row, Col, ParentIdx, BlockRow, BlockCol;
	CFont   Font, * pOldFont;
	CSize   FontSize;

	//========================================================================
	// 1. 폰트를 높이를 구한다.
	//========================================================================
	GetCharSize( pDC, m_pSheet->object[ObjectIndex]->table.font, FontSize );
	Font.CreateFontIndirect( &m_pSheet->object[ObjectIndex]->table.font );
	pOldFont = pDC->SelectObject( &Font ); 

	//========================================================================
	// 2. 선을 생성한다.
	//========================================================================

	//========================================================================
	// 3. 여백의 좌표를 구한다.
	//========================================================================
	TableTop  = m_pSheet->object[ObjectIndex]->table.top;
	TableLeft = m_pSheet->object[ObjectIndex]->table.left;
	Row       = m_pSheet->object[ObjectIndex]->table.row_count - 1;
	Col       = m_pSheet->object[ObjectIndex]->table.col_count - 1;

	if ( 0 > Row || 0 > Col ) {
		return 0;
	}

	if ( FALSE == GetParentInfo( m_pSheet, ObjectIndex, &ParentIdx, &BlockRow, &BlockCol ) ) {
		return -1;
	}

	Bottom    = m_pSheet->object[ParentIdx]->grid.top + m_pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].top + m_pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].height;
	Top       = TableTop  + m_pSheet->object[ObjectIndex]->table.cell[Row][0  ].top  + m_pSheet->object[ObjectIndex]->table.cell[Row][0  ].height -1;
	Left      = TableLeft + m_pSheet->object[ObjectIndex]->table.cell[Row][0  ].left;
	Right     = TableLeft + m_pSheet->object[ObjectIndex]->table.cell[Row][Col].left + m_pSheet->object[ObjectIndex]->table.cell[Row][Col].width;

	//========================================================================
	// 4. 여백의 높이를 구한다.
	//========================================================================
	Height = Bottom - Top;

	//========================================================================
	// 5. 여백을 그린다.
	//========================================================================
	if ( 5 * FontSize.cy <= Height ) {
		CPen    Pen,  * pOldPen;
		Pen.CreatePen( 0, 0, BLACK );
		pOldPen = pDC->SelectObject( &Pen );

		// 6.1 '이하여백'을 표시한다.
		pDC->MoveTo( Left,      Top );
		pDC->LineTo( Right - 1, Top );

		pDC->DrawText( _T("이  하  여  백"), CRect( Left, Top, Right, Bottom ), DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		pDC->SelectObject( pOldPen  );
	} else {
		// Line 지우기
		CPen Pen1, Pen2, * pOldPen;
		        
		if ( 1 < Bottom - Top ) {
			Pen1.CreatePen( 0, 0, WHITE );
			pOldPen = pDC->SelectObject( &Pen1 );

			pDC->MoveTo( Left  + 1, Top );
			pDC->LineTo( Right - 1, Top );

			pDC->SelectObject( pOldPen );
		} else {
			if ( Bottom == Top ) {
				Pen1.CreatePen( 0, 0, BLACK );
				pOldPen = pDC->SelectObject( &Pen1 );

				pDC->MoveTo( Left  + 1, Top );
				pDC->LineTo( Right - 1, Top );

				pDC->SelectObject( pOldPen );
			}
		}
		// 6.2 선으로 연결한다.
		Pen2.CreatePen( 0, 0, BLACK );
		pOldPen = pDC->SelectObject( &Pen2 );
		for ( i = 1; i < m_pSheet->object[ObjectIndex]->table.col_count; i++ ) {
			pDC->MoveTo( TableLeft + m_pSheet->object[ObjectIndex]->table.cell[Row][i].left, Top       );
			pDC->LineTo( TableLeft + m_pSheet->object[ObjectIndex]->table.cell[Row][i].left, Bottom - 1);
		}
		pDC->SelectObject( pOldPen );
	}

	pDC->SelectObject( pOldFont );
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::SetCurPage( UINT CurPage, INT nPage )
{
	m_CurPage = CurPage;
	m_nPage   = nPage;

	return 0;
}

//========================================================================
// DrawObject
//
// PURPOSE : 객체를 그린다.
//========================================================================
INT CWorkSheet::DrawObject( CDC * pDC            , // [IN] 화면 DC
						    INT   ObjectIdx      , // [IN] 객체의 인겍스 
							INT   ExpcetSelected ) // [IN] 선택된 객체 제외 플레그
{
ECO_EXCEPTION_BEGIN
	INT      i;
	CRect    Rect;

	//=================================================================================
	// Object Drawing
	//=================================================================================
	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( -1 != ObjectIdx && i != ObjectIdx ) {
			continue;
		}
		if ( TRUE == ExpcetSelected && -1 != m_pSheet->object[i]->select ) {
			continue;
		}
		GetObjectRect( i, Rect );
		switch ( m_pSheet->object[i]->kind ) {
			case OBJECT_TABLE :
				DrawTableObject( pDC, i, CPoint( Rect.left, Rect.top ) );
				break;
			case OBJECT_GRID :
				DrawGridObject( pDC, i, CPoint( Rect.left, Rect.top ) );
				break;
			case OBJECT_HLINE : {
				CPen Pen, * pOldPen;
				Pen.CreatePen( m_pSheet->object[i]->hline.style, m_pSheet->object[i]->hline.thick, BLACK );
				pOldPen = pDC->SelectObject( &Pen );
				pDC->MoveTo( Rect.left,  Rect.top    );
				pDC->LineTo( Rect.right, Rect.bottom );
				pDC->SelectObject( pOldPen );
			}
				break;
			case OBJECT_VLINE : {
				CPen Pen, * pOldPen;
				Pen.CreatePen( m_pSheet->object[i]->vline.style, m_pSheet->object[i]->vline.thick, BLACK );
				pOldPen = pDC->SelectObject( &Pen );
				pDC->MoveTo( Rect.left,  Rect.top    );
				pDC->LineTo( Rect.right, Rect.bottom );
				pDC->SelectObject( pOldPen );
			}
				break;
			case OBJECT_BOX : {
				CPen Pen, * pOldPen;
				Pen.CreatePen( m_pSheet->object[i]->box.style, m_pSheet->object[i]->box.thick, BLACK );
				pOldPen = pDC->SelectObject( &Pen );
				pDC->SelectStockObject( NULL_BRUSH );
				EcoRectangle( pDC,  Rect.left, Rect.top, Rect.right, Rect.bottom );
				pDC->SelectObject( pOldPen );
			}
				break;
			case OBJECT_BITMAP : {
				pDC->DrawText( _T("b"), StringLength( _T("b") ), CRect( Rect.left, Rect.top, Rect.right, Rect.bottom ), DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				m_pSheet->object[i]->bitmap.dib->Show( pDC, CRect( Rect.left, Rect.top, Rect.right, Rect.bottom ) );
				break;
			}
			case OBJECT_BARCODE : {
				m_pSheet->object[i]->barcode.bc->Draw( pDC, CRect( Rect.left, Rect.top, Rect.right, Rect.bottom ) );
				break;
			}
		}
	}

	//=================================================================================
	// 인쇄시는 그리지 않는다.
	//=================================================================================
	if ( TRUE == pDC->m_bPrinting ) {
		return 0;
	}
	//=================================================================================
	// Pick Point Drawing
	//=================================================================================
	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( -1 != ObjectIdx && i != ObjectIdx ) {
			continue;
		}
		if ( TRUE == ExpcetSelected && -1 != m_pSheet->object[i]->select ) {
			continue;
		}
		if ( -1 != m_pSheet->object[i]->select && TRUE != m_MouseInfo.moving ) {
			GetObjectRect( i, Rect );
			if ( m_SelectCnt == m_pSheet->object[i]->select ) {
				DrawPickPoint( pDC, CYAN, m_pSheet->object[i]->kind, Rect.left, Rect.top, Rect.right, Rect.bottom );
			} else {
				DrawPickPoint( pDC, LIGHT_GRAY, m_pSheet->object[i]->kind, Rect.left, Rect.top, Rect.right, Rect.bottom );
			}
		}
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// DrawPoint
//
// PURPOSE : 포인트를 그린다.
//========================================================================
INT CWorkSheet::DrawPoint( CDC      * pDC         ,  // [IN] 화면 DC
						   COLORREF   PickColor   ,  // [IN] PICK COLOR
						   INT        Left        ,  // [IN] 좌표
						   INT        Top         ,  // [IN] 좌표
						   INT        Right       ,  // [IN] 좌표
						   INT        Bottom      )  // [IN] 좌표
{
ECO_EXCEPTION_BEGIN
	CPen   Pen,   * pOldPen;
	CBrush Brush, * pOldBrush;

	Pen.CreatePen( 0, 0, BLACK );
    Brush.CreateSolidBrush( PickColor );

	pOldPen   = pDC->SelectObject( &Pen   );
	pOldBrush = pDC->SelectObject( &Brush );

	EcoRectangle( pDC,  Left, Top, Right, Bottom );

	pDC->SelectObject( pOldPen   );
	pDC->SelectObject( pOldBrush );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// DrawPickPoint
//
// PURPOSE : 픽크 포인트를 그린다. ( 객체 크기 조절 )
//========================================================================
INT CWorkSheet::DrawPickPoint( CDC      * pDC      , // [IN] 화면 DC
							   COLORREF   PickColor, // [IN] 선택순서
							   INT        Kind     , // [IN] 객체의 종류
							   INT        Left     , // 좌표
							   INT        Top      , // 좌표 
							   INT        Right    , // 좌표
							   INT        Bottom   ) // 좌표
{
ECO_EXCEPTION_BEGIN
	INT HorzMiddle, VertMiddle;

	switch ( Kind ) {
		case OBJECT_HLINE :
		case OBJECT_VLINE :
			DrawPoint( pDC, PickColor, Left  - PICK_POINT_SIZE, Top    - PICK_POINT_SIZE, Left  + PICK_POINT_SIZE, Top    + PICK_POINT_SIZE );
			DrawPoint( pDC, PickColor, Right - PICK_POINT_SIZE, Bottom - PICK_POINT_SIZE, Right + PICK_POINT_SIZE, Bottom + PICK_POINT_SIZE );
			break;
		case OBJECT_TABLE   :
		case OBJECT_GRID    :
		case OBJECT_BOX     :
		case OBJECT_BITMAP  :
		case OBJECT_BARCODE :
			// BOUNDARY
			DrawPoint( pDC, PickColor, Left  - PICK_POINT_SIZE, Top    - PICK_POINT_SIZE, Left  + PICK_POINT_SIZE, Top    + PICK_POINT_SIZE );
			DrawPoint( pDC, PickColor, Right - PICK_POINT_SIZE, Top    - PICK_POINT_SIZE, Right + PICK_POINT_SIZE, Top    + PICK_POINT_SIZE );
			DrawPoint( pDC, PickColor, Left  - PICK_POINT_SIZE, Bottom - PICK_POINT_SIZE, Left  + PICK_POINT_SIZE, Bottom + PICK_POINT_SIZE );
			DrawPoint( pDC, PickColor, Right - PICK_POINT_SIZE, Bottom - PICK_POINT_SIZE, Right + PICK_POINT_SIZE, Bottom + PICK_POINT_SIZE );
			// INNER
			HorzMiddle = Left + ( Right  - Left ) / 2 - PICK_POINT_SIZE;
			VertMiddle = Top  + ( Bottom - Top  ) / 2 - PICK_POINT_SIZE;

			DrawPoint( pDC, PickColor, HorzMiddle, Top    - PICK_POINT_SIZE, HorzMiddle + 2 * PICK_POINT_SIZE, Top    + PICK_POINT_SIZE );  //-
			DrawPoint( pDC, PickColor, HorzMiddle, Bottom - PICK_POINT_SIZE, HorzMiddle + 2 * PICK_POINT_SIZE, Bottom + PICK_POINT_SIZE );  //_
			DrawPoint( pDC, PickColor, Left  - PICK_POINT_SIZE, VertMiddle, Left  + PICK_POINT_SIZE, VertMiddle + 2 * PICK_POINT_SIZE );
			DrawPoint( pDC, PickColor, Right - PICK_POINT_SIZE, VertMiddle, Right + PICK_POINT_SIZE, VertMiddle + 2 * PICK_POINT_SIZE );
			break;
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// DrawSheetFrame
//
// PURPOSE : Sheet 플레임을 그린다.
//========================================================================
INT CWorkSheet::DrawSheetFrame( CDC * pDC ) // [IN] 화면 DC
{
ECO_EXCEPTION_BEGIN
	RECT rect;

	//
	rect.left   = SHEET_BLANK_WIDTH  + 5 + m_CenterWidth;
	rect.top    = SHEET_BLANK_HEIGHT + 5;
	rect.right  = SHEET_BLANK_WIDTH  + m_ReportInfo.sheet_width + 5 + m_CenterWidth;
	rect.bottom = SHEET_BLANK_HEIGHT + m_ReportInfo.sheet_height + 5;

	DrawFrame( pDC, &rect, BLACK, BLACK );
	//
	rect.left   = SHEET_BLANK_WIDTH + m_CenterWidth;
	rect.top    = SHEET_BLANK_HEIGHT;
	rect.right  = SHEET_BLANK_WIDTH  + m_ReportInfo.sheet_width + m_CenterWidth;
	rect.bottom = SHEET_BLANK_HEIGHT + m_ReportInfo.sheet_height;

	DrawFrame( pDC, &rect, BLACK, WHITE );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// DrawSheetFrame
//
// PURPOSE : 플레임을 그린다.
//========================================================================
INT CWorkSheet::DrawFrame( CDC      * pDC         , // [IN] 화면 DC
						   RECT     * rect        , // [IN] 그릴 영역 
						   COLORREF   pen_color   , // [IN] 외곽색깔
						   COLORREF   brush_color ) // [IN] 채움색깔
{
ECO_EXCEPTION_BEGIN
	CPen   BackPen,   * pOldBackPen;
	CBrush BackBrush, * pOldBackBrush;
	
	// $ 1.1 C R E A T E   ' P E N  ,   B R U S H '
	BackPen.CreatePen( PS_SOLID, 0, pen_color );
	BackBrush.CreateSolidBrush( brush_color );
	// $ 1.2 S E L E C T   O B J E T
	pOldBackPen   = pDC->SelectObject( &BackPen   );
	pOldBackBrush = pDC->SelectObject( &BackBrush );
	// $ 1.3 D R A W   B A C K G R O U N D
	EcoRectangle( pDC, rect->left, rect->top, rect->right, rect->bottom );
	// $ 1.4 S E L E C T   O L D   O B J E T
	pDC->SelectObject( pOldBackPen   );
	pDC->SelectObject( pOldBackBrush );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// GetSheetSize
//
// PURPOSE : Sheet의 크기를 구한다.
//========================================================================
INT CWorkSheet::GetSheetSize( SIZE * SheetSize ) // [OUT] Sheet의 크기
{
ECO_EXCEPTION_BEGIN
	//*SheetSize = m_SheetSize;
	SheetSize->cx = m_ReportInfo.sheet_width;
	SheetSize->cy = m_ReportInfo.sheet_height;

	return 0;
ECO_EXCEPTION_END
}

//========================================================================
// SetSheetSize
//========================================================================
INT CWorkSheet::SetSheetPaper( INT PaperNo )
{
ECO_EXCEPTION_BEGIN
	switch ( PaperNo ) {
		case 0 : m_ReportInfo.paper = DMPAPER_A4; break;
		case 1 : m_ReportInfo.paper = DMPAPER_A3; break;
		case 2 : m_ReportInfo.paper = DMPAPER_A5; break;
		case 3 : m_ReportInfo.paper = DMPAPER_B4; break;
		case 4 : m_ReportInfo.paper = DMPAPER_B5; break;
	}

	//
	SetPaperKind( m_ReportInfo.paper, m_pWinApp );

ECO_EXCEPTION_END
	return 0;
}

INT CWorkSheet::SetSheetSize( INT SheetSizeNo, CDC * pDC )
{
ECO_EXCEPTION_BEGIN
	CSize SheetSize;

	if ( m_ReportInfo.sheet_width > m_ReportInfo.sheet_height ) {
		SheetSize.cx = SHEET_WIDTH;
		SheetSize.cy = SHEET_HEIGHT;
	} else {
		SheetSize.cx = SHEET_HEIGHT;
		SheetSize.cy = SHEET_WIDTH;
	}

	m_ReportInfo.sheet_width  = INT( double( SheetSize.cx ) * ( 1.0 + double( SheetSizeNo ) * 0.2 ) );
	m_ReportInfo.sheet_height = INT( double( SheetSize.cy ) * ( 1.0 + double( SheetSizeNo ) * 0.2 ) );

	m_ReportInfo.sheet_width  = m_ReportInfo.sheet_width;
	m_ReportInfo.sheet_height = m_ReportInfo.sheet_height;
	m_ReportInfo.sheet_size   = SheetSizeNo;

	//
	//m_CenterWidth = INT( double( SHEET_WIDTH - m_ReportInfo.sheet_width ) / 2.0 ); // WSO
	m_CenterWidth = INT( double( m_ClientRect.Width() - 19 - m_ReportInfo.sheet_width ) / 2.0 ); // WSO
	
	if ( 0 > m_CenterWidth ) {
		m_CenterWidth = 0;
	}

	// 
	FreeBitmap();
	InitBitmap( pDC );
	//
	DrawObjectToMemDC(); // WWW

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// SetSheetOrientation
//========================================================================
INT CWorkSheet::SetSheetOrientation( INT Orientation, CDC * pDC )
{
ECO_EXCEPTION_BEGIN
	INT SmallSize, BigSize;

	//
	SetPaperDirection( Orientation, m_pWinApp );

	//
	SmallSize = ( m_ReportInfo.sheet_width < m_ReportInfo.sheet_height ) ? m_ReportInfo.sheet_width : m_ReportInfo.sheet_height;
	BigSize   = ( m_ReportInfo.sheet_width > m_ReportInfo.sheet_height ) ? m_ReportInfo.sheet_width : m_ReportInfo.sheet_height;

	//
	switch ( Orientation ) {
		case DMORIENT_LANDSCAPE :
			m_ReportInfo.sheet_width  = BigSize;
			m_ReportInfo.sheet_height = SmallSize;
			m_ReportInfo.orientation  = DMORIENT_LANDSCAPE;
			break;
		case DMORIENT_PORTRAIT :
			m_ReportInfo.sheet_width  = SmallSize;
			m_ReportInfo.sheet_height = BigSize;
			m_ReportInfo.orientation  = DMORIENT_PORTRAIT;
			break;
	}

	//
	//m_CenterWidth = INT( double( SHEET_WIDTH - m_ReportInfo.sheet_width ) / 2.0 ); // WSO
	m_CenterWidth = INT( double( m_ClientRect.Width() - 19 - m_ReportInfo.sheet_width ) / 2.0 ); // WSO
	if ( 0 > m_CenterWidth ) {
		m_CenterWidth = 0;
	}

	//
	FreeBitmap();
	InitBitmap( pDC );
	//
	DrawObjectToMemDC(); // WWW

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// LButtonDown
//
// 목적 : Owner Window의 OnLButtonDown함수가 호출한다.
// 작성 : 우상옥
//========================================================================
INT CWorkSheet::LButtonDown( CDC    * pDC   , // [IN] 화면 DC
							 UINT     nFlags, // [IN] 마우스 플래그
							 CPoint & point , // [IN] 마우스 좌표
							 CSize  & Size  ) // [IN] 스크롤 크기 
{
ECO_EXCEPTION_BEGIN
	INT   PickPoint; //, i, j;
	BOOL  FindObjectIdx;
	//RECT  CopyObjectRect;
	CRect Rect;

	// 1. 스크롤 처리를 한다.
	AdjustScrollPoint( point, &Size );
	AdjustScrollDC   ( pDC,   &Size );

	// 2. 마우스정보를 초기화한다.
	m_MouseInfo.clicked      = TRUE;
	m_MouseInfo.moving       = FALSE;
	m_MouseInfo.anchor_point = point;
	m_MouseInfo.move_offset  = CSize( 0, 0 );

	// 3. 객체 선택처리를 한다.
	if ( -1 != ( FindObjectIdx = SearchObject( point, &PickPoint ) ) ) {
		// 3.1 마우스 정보를 설정한다.
		m_MouseInfo.object_idx = FindObjectIdx;
		m_MouseInfo.pick_point = PickPoint;
		// 3.2 멀티선택과 싱글선택을 처리한다.
		if ( ::GetKeyState( VK_SHIFT ) & 0x8000 ) { // SHIFT KEY
			// 3.2.1 멀티선택일 경우에 포커스를 없앤다.
			KillFocus(); 
			// 3.2.2 객체를 멀티로 선택한다.
			SelectObject( FindObjectIdx, OBJET_MULTI );
		} else {
			// 3.2.1 싱글선택일 경우에 포커스를 설정한다.
			KillFocus(); 
			// 3.2.2 선택된 객체에 포커스를 설정한다.
			SetFocus( FindObjectIdx );
			// 3.2.3 객체를 선택처리 한다.
			if ( FALSE == IsSelected( FindObjectIdx ) ) {
				SelectObject( FindObjectIdx );
			}
		}

		// 3.3 객체 복사 처리를 한다.
/*
		if ( ::GetKeyState( VK_CONTROL ) & 0x8000 && PickPoint == OBJECT_MOVE) { // CONTROL KEY
			// 4.2 자식객체가 있는 객체는 복사되지 않는다.
			if ( OBJECT_GRID == m_pSheet->object[FindObjectIdx]->kind ) {
				for ( i = 0; i < m_pSheet->object[FindObjectIdx]->grid.row_count; i++ ) {
					for ( j = 0; j < m_pSheet->object[FindObjectIdx]->grid.col_count; j++ ) {
						if ( 0 < m_pSheet->object[FindObjectIdx]->grid.block[i][j].child_count ) {
							AfxMessageBox( _T("자식객체가 있는 객체는 복사되지 않습니다.") );
							InitMouseInfo();
							return 0;
						}
					}
				}
			}
			// 3.3.1 객체의 영역을 구한다.
			GetObjectRect( FindObjectIdx, Rect );
			CopyObjectRect = Rect;
			// 3.3.2 객체를 복사한다.
			CopyObjectInfo( m_pSheet, m_pSheet, FindObjectIdx );
			// 3.3.3 마우스 정보를 설정한다.
			m_MouseInfo.object_idx = m_pSheet->object_count - 1;
			// 3.3.4 객체를 선택처리한다.
			SelectObject( m_MouseInfo.object_idx );
			// 3.3.5 선택된 객체에 포커스를 설정한다.
			SetFocus( m_MouseInfo.object_idx );
		} 
*/
		// 3.4 다시 그린다.
		DrawObjectToMemDC();
		CopyMemDCToScreen( pDC );

		// 3.5 선택된 객체들의 영역을 구한다.
		GetSelectedObjectRect( Rect );
		m_ObjectRect.SetRect( Rect.left  - PICK_POINT_SIZE, Rect.top    - PICK_POINT_SIZE, 
                              Rect.right + PICK_POINT_SIZE, Rect.bottom + PICK_POINT_SIZE );

		// 3.6 객체를 보관용 DC에 보관한다.
		PutObjectToStore( m_MouseInfo.object_idx, TRUE );

		// 3.7 Tab Stop을 설정한다.
		m_TabStopObjectIdx = m_MouseInfo.object_idx + 1;
	} else {
		// 선택되지 않았을 경우 마우스 정보를 초기한다.
		m_MouseInfo.object_idx = -1;
		m_MouseInfo.pick_point = -1;
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// SearchObjectRegion
//
// 목적 : 영역안에 있는 객체를 찾아 선택한다.
// 작성 : 우상옥
//========================================================================
INT CWorkSheet::SearchObjectRegion( CRect & Rect )
{
ECO_EXCEPTION_BEGIN
	INT   i;
	CRect ObjectRect;

	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		GetObjectRect( i, ObjectRect );
		if ( Rect.left < ObjectRect.left && Rect.top < ObjectRect.top && Rect.right > ObjectRect.right && Rect.bottom > ObjectRect.bottom ) {
			SelectObject( i, OBJET_MULTI );
		}
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// LButtonUp
//========================================================================
INT CWorkSheet::LButtonUp( CDC    * pDC   ,  // [IN] 화면 DC
						   UINT     nFlags,  // [IN] 마우스 플래그 
						   CPoint & point ,  // [IN] 마우스 좌표
						   CSize  & Size  ) 
{
ECO_EXCEPTION_BEGIN
	CRect Rect;

	//
	AdjustScrollPoint( point, &Size );
	AdjustScrollDC   ( pDC,   &Size );

	//
	if ( -1 == m_MouseInfo.object_idx ) {
		// Search Object
		SelectObject( -1 );

		//
		KillFocus();

		//
		Rect.SetRect( m_MouseInfo.anchor_point.x,                              m_MouseInfo.anchor_point.y, 
				      m_MouseInfo.anchor_point.x - m_MouseInfo.move_offset.cx, m_MouseInfo.anchor_point.y - m_MouseInfo.move_offset.cy );

		//
		Rect.NormalizeRect();
		SearchObjectRegion( Rect );
	} else { 
		// 객체삽입
		EmbedObject( m_MouseInfo.object_idx );
	}

	//
	InitMouseInfo();

	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// MouseMove
//
// PURPOSE : Owner Window의 OnMouseMove함수가 호출한다.
//========================================================================
INT CWorkSheet::MouseMove( CDC    * pDC   ,  // [IN] 화면 DC
						   UINT     nFlags,  // [IN] 마우스 플래그
						   CPoint & point ,  // [IN] 마우스 좌표
						   CSize  & Size  )
{
ECO_EXCEPTION_BEGIN
	CRect Rect;
	CPen  Pen, * pOldPen;

	AdjustScrollPoint( point, &Size  );
	AdjustScrollDC   ( pDC,   &Size );

	if ( -1 != m_MouseInfo.object_idx ) {
		m_MouseInfo.move_offset.cx = m_MouseInfo.anchor_point.x - point.x;
		m_MouseInfo.move_offset.cy = m_MouseInfo.anchor_point.y - point.y;
		m_MouseInfo.anchor_point   = point;
		m_MouseInfo.moving         = TRUE;

		switch ( m_MouseInfo.pick_point ) {
			case OBJECT_MOVE :
				MoveObject( pDC, m_MouseInfo.move_offset );
				break;
			case OBJECT_RESIZE + 0 :
			case OBJECT_RESIZE + 1 :
			case OBJECT_RESIZE + 2 :
			case OBJECT_RESIZE + 3 :
			case OBJECT_RESIZE + 4 :
			case OBJECT_RESIZE + 5 :
			case OBJECT_RESIZE + 6 :
			case OBJECT_RESIZE + 7 :
				ResizeObject( pDC, m_MouseInfo.move_offset, m_MouseInfo.pick_point );
				break;
		}
		GetSelectedObjectRect( Rect );
		PadBitmap( pDC, Rect );
	} else { // Region Select
		if ( TRUE == m_MouseInfo.clicked ) {
			Pen.CreatePen( 2, 0, BLACK );

			pOldPen = pDC->SelectObject( &Pen );

			pDC->SelectStockObject( NULL_BRUSH );

			//++
			Rect.SetRect( m_MouseInfo.anchor_point.x,                              m_MouseInfo.anchor_point.y, 
				          m_MouseInfo.anchor_point.x - m_MouseInfo.move_offset.cx, m_MouseInfo.anchor_point.y - m_MouseInfo.move_offset.cy );
			Rect.NormalizeRect();
			pDC->DrawFocusRect( Rect );
			//--

			m_MouseInfo.move_offset.cx = m_MouseInfo.anchor_point.x - point.x;
			m_MouseInfo.move_offset.cy = m_MouseInfo.anchor_point.y - point.y;

			Rect.SetRect( m_MouseInfo.anchor_point.x,                              m_MouseInfo.anchor_point.y, 
				          m_MouseInfo.anchor_point.x - m_MouseInfo.move_offset.cx, m_MouseInfo.anchor_point.y - m_MouseInfo.move_offset.cy );


			//++
			Rect.SetRect( m_MouseInfo.anchor_point.x,                              m_MouseInfo.anchor_point.y, 
				          m_MouseInfo.anchor_point.x - m_MouseInfo.move_offset.cx, m_MouseInfo.anchor_point.y - m_MouseInfo.move_offset.cy );
			Rect.NormalizeRect();
			pDC->DrawFocusRect( Rect );			
			//--

			pDC->SelectObject( pOldPen );
		}
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// MouseMove
//
// PURPOSE : Owner Window의 OnMouseMove함수가 호출한다.
//========================================================================
INT CWorkSheet::RButtonDown( CDC    * pDC   ,  // [IN] 화면 DC
							 UINT     nFlags,  // [IN] 마우스 플래그
							 CPoint & point ,  // [IN] 마우스 포인터
							 CSize  & Size  )
{
ECO_EXCEPTION_BEGIN
/*
	INT PickPoint, FindObjectIdx;
	CPoint PopupPoint;

	PopupPoint = point;
	AdjustScrollPoint( point, &Size );
	AdjustScrollDC   ( pDC,   &Size );

	if ( -1 != ( FindObjectIdx = SearchObject( point, &PickPoint ) ) ) {
		if ( FALSE == IsSelected( FindObjectIdx ) ) {
			// New Select
			SelectObject( FindObjectIdx );
		}
		// Redrawing
		DrawObjectToMemDC();
		CopyMemDCToScreen( pDC );

		// Object Popup Menu
		m_PropertyInfo.object_index = FindObjectIdx;

		( m_pOwnderWnd->*m_fpTrackObjectPopupMenu )( PopupPoint, m_PropertyInfo );

		// UnSelect
		SelectObject( -1 );
		// Redrawing
		DrawObjectToMemDC();
		CopyMemDCToScreen( pDC );
	} else {
		// UnSelect
		SelectObject( -1 );
		// Redrawing
		DrawObjectToMemDC();
		CopyMemDCToScreen( pDC );

		// Editor Popup Menu
		( m_pOwnderWnd->*m_fpTrackEditorPopupMenu )( PopupPoint );
	}
*/

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// MouseMove
//
// PURPOSE : Owner Window의 OnMouseMove함수가 호출한다.
//========================================================================
INT CWorkSheet::RButtonUp( CDC   *  pDC   ,  // [IN] 화면 DC
						   UINT     nFlags,  // [IN] 마우스 플래그
						   CPoint & point ,  // [IN] 마우스 포인터
						   CSize  & Size  )
{
ECO_EXCEPTION_BEGIN
	return 0;
ECO_EXCEPTION_END
}

//========================================================================
// ResizeObject
//
// PURPOSE : 객체의 크기를 변경한다. ( 좌표수정 )
//
//           1     2     3
//           +-----------+
//           |           |
//         4 |           | 5
//           |           |
//           +-----------+
//           6     7     8
//========================================================================
INT CWorkSheet::ResizeObject( CDC   * pDC       ,  // [IN] 화면 DC
							  CSize & Size      ,  // [IN] 이동 변위
							  INT     PickPoint )  // [IN] 크기변경 포인트
{
ECO_EXCEPTION_BEGIN
	CRect Rect;
	INT i, j, Width, Height, ChildCol, ChildRow, ChildWidth, ChildHeight;

	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( -1 == m_pSheet->object[i]->select ) {
			continue;
		}

		switch ( m_pSheet->object[i]->kind ) {
			case OBJECT_TABLE   :
			case OBJECT_GRID    :
			case OBJECT_BOX     :
			case OBJECT_BITMAP  :
			case OBJECT_BARCODE :
				switch ( PickPoint ) {
					case 1 :
						Width  = m_pSheet->object[i]->comm.width  + Size.cx;
						Height = m_pSheet->object[i]->comm.height + Size.cy;

						if ( LIMIT_SIZE > Width ) {
							Size.cx = 0;
						}
						if ( LIMIT_SIZE > Height ) {
							Size.cy = 0;
						}

						m_pSheet->object[i]->comm.left   -= Size.cx;
						m_pSheet->object[i]->comm.top    -= Size.cy;
						m_pSheet->object[i]->comm.width  += Size.cx;
						m_pSheet->object[i]->comm.height += Size.cy;
						break;
					case 2 : 
						Height = m_pSheet->object[i]->comm.height + Size.cy;

						if ( LIMIT_SIZE > Height ) {
							Size.cy = 0;
						}

						m_pSheet->object[i]->comm.top    -= Size.cy;
						m_pSheet->object[i]->comm.height += Size.cy;
						break;
					case 3 : 
						Width  = m_pSheet->object[i]->comm.width  - Size.cx;
						Height = m_pSheet->object[i]->comm.height + Size.cy;

						if ( LIMIT_SIZE > Width ) {
							Size.cx = 0;
						}
						if ( LIMIT_SIZE > Height ) {
							Size.cy = 0;
						}

						m_pSheet->object[i]->comm.top    -= Size.cy;
						m_pSheet->object[i]->comm.width  -= Size.cx;
						m_pSheet->object[i]->comm.height += Size.cy;
						break;
					case 4 : 
						Width  = m_pSheet->object[i]->comm.width + Size.cx;

						if ( LIMIT_SIZE > Width ) {
							Size.cx = 0;
						}

						m_pSheet->object[i]->comm.left  -= Size.cx;
						m_pSheet->object[i]->comm.width += Size.cx;
						break;
					case 5 : 
						Width  = m_pSheet->object[i]->comm.width - Size.cx;

						if ( LIMIT_SIZE > Width ) {
							Size.cx = 0;
						}

						m_pSheet->object[i]->comm.width -= Size.cx;
						break;
					case 6 : 
						Width  = m_pSheet->object[i]->comm.width  + Size.cx;
						Height = m_pSheet->object[i]->comm.height - Size.cy;

						if ( LIMIT_SIZE > Width ) {
							Size.cx = 0;
						}
						if ( LIMIT_SIZE > Height ) {
							Size.cy = 0;
						}

						m_pSheet->object[i]->comm.left   -= Size.cx;
						m_pSheet->object[i]->comm.width  += Size.cx;
						m_pSheet->object[i]->comm.height -= Size.cy;
						break;
					case 7 : 
						Height = m_pSheet->object[i]->comm.height - Size.cy;

						if ( LIMIT_SIZE > Height ) {
							Size.cy = 0;
						}

						m_pSheet->object[i]->comm.height -= Size.cy;
						break;
					case 8 :
						Width  = m_pSheet->object[i]->comm.width  - Size.cx;
						Height = m_pSheet->object[i]->comm.height - Size.cy;

						if ( LIMIT_SIZE > Width ) {
							Size.cx = 0;
						}
						if ( LIMIT_SIZE > Height ) {
							Size.cy = 0;
						}
						
						m_pSheet->object[i]->comm.width  -= Size.cx;
						m_pSheet->object[i]->comm.height -= Size.cy;
						break;
				}
				break;
			case OBJECT_HLINE :
				switch ( PickPoint ) {
					case 1 : 
						m_pSheet->object[i]->hline.left  -= Size.cx;
						m_pSheet->object[i]->hline.width += Size.cx;
						break;
					case 2 : 
						m_pSheet->object[i]->hline.width -= Size.cx;					
						break;
				}
				break;
			case OBJECT_VLINE :
				switch ( PickPoint ) {
					case 1 : 
						m_pSheet->object[i]->vline.top     -= Size.cy;
						m_pSheet->object[i]->vline.height  += Size.cy;
						break;
					case 2 : 
						m_pSheet->object[i]->vline.height -= Size.cy;
						break;
				}
				break;
		}
		//++ CHILD OBJECT
		switch ( m_pSheet->object[i]->kind ) {
			case OBJECT_TABLE :
				ChildCol = m_pSheet->object[i]->table.col_count - 1;
				ChildRow = m_pSheet->object[i]->table.row_count - 1;
				for ( j = 0; j < m_pSheet->object[i]->table.row_count; j++ ) {
					ChildWidth = m_pSheet->object[i]->table.width - m_pSheet->object[i]->table.cell[j][ChildCol].left;
					m_pSheet->object[i]->table.cell[j][ChildCol].width = ChildWidth;
				}
				for ( j = 0; j < m_pSheet->object[i]->table.col_count; j++ ) {
					ChildHeight = m_pSheet->object[i]->table.height - m_pSheet->object[i]->table.cell[ChildRow][j].top;
					m_pSheet->object[i]->table.cell[ChildRow][j].height = ChildHeight;
				}
				break;
			case OBJECT_GRID :
				ChildCol = m_pSheet->object[i]->grid.col_count - 1;
				ChildRow = m_pSheet->object[i]->grid.row_count - 1;
				for ( j = 0; j < m_pSheet->object[i]->grid.row_count; j++ ) {
					ChildWidth = m_pSheet->object[i]->grid.width - m_pSheet->object[i]->grid.block[j][ChildCol].left;
					m_pSheet->object[i]->grid.block[j][ChildCol].width        = ChildWidth;
					m_pSheet->object[i]->grid.block[j][ChildCol].origin_width = ChildWidth;
				}
				for ( j = 0; j < m_pSheet->object[i]->grid.col_count; j++ ) {
					ChildHeight = m_pSheet->object[i]->grid.height - m_pSheet->object[i]->grid.block[ChildRow][j].top;
					m_pSheet->object[i]->grid.block[ChildRow][j].height        = ChildHeight;
					m_pSheet->object[i]->grid.block[ChildRow][j].origin_height = ChildHeight;
				}
				break;
		}
		//--
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// MoveObject
//
// PURPOSE : 객체를 이동시킨다. ( 좌표수정 )
//========================================================================
INT CWorkSheet::MoveObject( CDC *   pDC      ,  // [IN] 화면 DC
                            CSize & Size     )  // [IN] 이동 변위
{
ECO_EXCEPTION_BEGIN
	INT i, ParentIdx, BlockRow, BlockCol;

	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( -1 == m_pSheet->object[i]->select ) {
			continue;
		}
		// 부모객체가 있으면 그 내부에서만 이동한다.
		if ( TRUE == GetParentInfo( m_pSheet, i, &ParentIdx, &BlockRow, &BlockCol ) ) {
			MoveChildObject( i, ParentIdx, Size );
		} else {
		// 부모객체가 없으면 자유롭게 이동한다.
			m_pSheet->object[i]->comm.left -= Size.cx;
			m_pSheet->object[i]->comm.top  -= Size.cy;
		}
		// 자식객체가 있는 부모객체를 이동한다.
		if ( OBJECT_GRID == m_pSheet->object[i]->kind ) {
			MoveParentObject( i, Size );
		}
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// MoveParentObject
//
// PURPOSE : 객체를 이동시킨다. ( 좌표수정 )
//========================================================================
INT CWorkSheet::MoveParentObject( INT ObjectIdx, CSize & Size )
{
ECO_EXCEPTION_BEGIN
	INT j, k, l, m;

	for ( j = 0; j < m_pSheet->object[ObjectIdx]->grid.row_count; j++ ) {
		for ( k = 0; k < m_pSheet->object[ObjectIdx]->grid.col_count; k++ ) {
			for ( l = 0; l < m_pSheet->object[ObjectIdx]->grid.block[j][k].child_count; l++ ) {
				for ( m = 0; m < m_pSheet->object_count; m++ ) {
					if ( 0 == _tcsicmp( m_pSheet->object[m]->comm.name, m_pSheet->object[ObjectIdx]->grid.block[j][k].child_name[l] ) ) {
						m_pSheet->object[m]->comm.left -= Size.cx;
						m_pSheet->object[m]->comm.top  -= Size.cy;
						break;
					}
				}
			}
		}
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// MoveChildObject
//========================================================================
INT CWorkSheet::MoveChildObject( INT ObjectIdx, INT ParentObjectIdx, CSize & Size )
{
ECO_EXCEPTION_BEGIN
	INT BlockRow, BlockCol, ChildLeft, ChildTop, ChildRight, ChildBottom;
	INT ParentLeft, ParentTop, BlockLeft, BlockTop, BlockRight, BlockBottom;

	BlockRow = m_pSheet->object[ObjectIdx]->comm.block_row;
	BlockCol = m_pSheet->object[ObjectIdx]->comm.block_col;

	ParentLeft = m_pSheet->object[ParentObjectIdx]->grid.left;
	ParentTop  = m_pSheet->object[ParentObjectIdx]->grid.top;

	BlockLeft   = ParentLeft + m_pSheet->object[ParentObjectIdx]->grid.block[BlockRow][BlockCol].left;
	BlockTop    = ParentTop  + m_pSheet->object[ParentObjectIdx]->grid.block[BlockRow][BlockCol].top;
	BlockRight  = BlockLeft  + m_pSheet->object[ParentObjectIdx]->grid.block[BlockRow][BlockCol].width;
	BlockBottom = BlockTop   + m_pSheet->object[ParentObjectIdx]->grid.block[BlockRow][BlockCol].height;

	ChildLeft   = m_pSheet->object[ObjectIdx]->comm.left - Size.cx;
	ChildTop    = m_pSheet->object[ObjectIdx]->comm.top  - Size.cy;
	ChildRight  = ChildLeft + m_pSheet->object[ObjectIdx]->comm.width  - Size.cx;
	ChildBottom = ChildTop  + m_pSheet->object[ObjectIdx]->comm.height - Size.cy;

	if ( BlockLeft > ChildLeft && 0 < Size.cx ) {
		m_pSheet->object[ObjectIdx]->comm.left = BlockLeft;
		return -1;
	}
	if ( BlockTop > ChildTop && 0 < Size.cy ) {
		m_pSheet->object[ObjectIdx]->comm.top = BlockTop;
		return -1;
	}
	if ( BlockRight < ChildRight && 0 > Size.cx ) {
		m_pSheet->object[ObjectIdx]->comm.left = BlockRight - m_pSheet->object[ObjectIdx]->comm.width;
		return -1;
	}
	if ( BlockBottom < ChildBottom && 0 > Size.cy ) {
		m_pSheet->object[ObjectIdx]->comm.top = BlockBottom - m_pSheet->object[ObjectIdx]->comm.height;
		return -1;
	}
	// Moving
	m_pSheet->object[ObjectIdx]->comm.left -= Size.cx;
	m_pSheet->object[ObjectIdx]->comm.top  -= Size.cy;

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// PadBitmap
//
// PURPOSE : 객체비트맵을 이동시킨다.
//========================================================================
INT CWorkSheet::PadBitmap( CDC   * pDC  ,  // [IN] 화면 DC
						   CRect & Rect )  // [IN] 이동할 영역
{
ECO_EXCEPTION_BEGIN
	// 1. BITMAP RESTORE.
	CBitmap * pOldRestoreBitmap, * pOldMasterBitmap;
	CRect     UpdateRect, TempRect;
	
	UpdateRect = Rect;
    UpdateRect.left  -= PICK_POINT_SIZE; UpdateRect.top    -= PICK_POINT_SIZE;
    UpdateRect.right += PICK_POINT_SIZE; UpdateRect.bottom += PICK_POINT_SIZE;

	TempRect.UnionRect( m_ObjectRect, UpdateRect );

    pOldRestoreBitmap = ( CBitmap * )m_pRestoreMemDC->SelectObject( m_pRestoreBitmap );
    pOldMasterBitmap  = ( CBitmap * )m_pMasterMemDC ->SelectObject( m_pMasterBitmap  );

    m_pMasterMemDC->BitBlt( m_ObjectRect.left, m_ObjectRect.top, m_ObjectRect.Width(), m_ObjectRect.Height(), m_pRestoreMemDC, 0, 0, SRCCOPY);

	// 2. 크기 비교 & 재생성 bitmap.
    m_pRestoreMemDC->BitBlt( 0, 0, UpdateRect.Width(), UpdateRect.Height(), m_pMasterMemDC, UpdateRect.left, UpdateRect.top, SRCCOPY );

	//Master Bitmap에 그림을 그린다.
    DrawObject( m_pMasterMemDC );
	
    pDC->BitBlt( TempRect.left, TempRect.top, TempRect.Width(), TempRect.Height(), m_pMasterMemDC, TempRect.left, TempRect.top, SRCCOPY );

	//원래로 돌려준다.
    m_pRestoreMemDC->SelectObject( pOldRestoreBitmap );
    m_pMasterMemDC ->SelectObject( pOldMasterBitmap  );

    m_ObjectRect = UpdateRect;

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// AdjustPoint
//
// PURPOSE : 플레임과 윈도우 사이의 간격을 보상해준다.
//========================================================================
INT CWorkSheet::AdjustScrollPoint( CPoint & point, CSize * Size )
{
ECO_EXCEPTION_BEGIN
	if ( NULL != Size ) {
		point.x = point.x - SHEET_BLANK_WIDTH  + Size->cx - m_CenterWidth;
		point.y = point.y - SHEET_BLANK_HEIGHT + Size->cy;
	} else {
		point.x = point.x - SHEET_BLANK_WIDTH - m_CenterWidth;
		point.y = point.y - SHEET_BLANK_HEIGHT;
	}

	return 0;
ECO_EXCEPTION_END
}

//========================================================================
// NewObject
//
// PURPOSE : 객체를 생성한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewObject( CDC * pDC, INT Kind, PROPERTY_INFO * Property_Info, CSize * Size )
{
ECO_EXCEPTION_BEGIN
	CRect ObjectRect;

	// 1. 스크롤 크기를 계산한다.
	AdjustScrollDC( pDC, Size );

	// 2. 객체개수를 증가하고 메모리를 할당한다.
	m_pSheet->object_count++;
	m_pSheet->object = ( REPORT_OBJECT ** )realloc( m_pSheet->object, sizeof( REPORT_OBJECT * ) * m_pSheet->object_count );
	m_pSheet->object[m_pSheet->object_count-1] = ( REPORT_OBJECT * )malloc( sizeof( REPORT_OBJECT ) );
	if ( NULL == m_pSheet->object ) {
		return -1;
	}
	m_pSheet->object[m_pSheet->object_count-1]->select = -1;
	m_pSheet->object[m_pSheet->object_count-1]->kind   = Kind;

	// 3. 개체정보를 구성한다.
	switch ( Kind ) {
		case OBJECT_TABLE   : NewTableObject  ( pDC, m_pSheet->object_count-1, CPoint( 10+Size->cx, 10+Size->cy ), Property_Info ); break;
		case OBJECT_GRID    : NewGridObject   ( pDC, m_pSheet->object_count-1, CPoint( 10+Size->cx, 10+Size->cy ), Property_Info ); break;
		case OBJECT_HLINE   : NewHLineObject  (      m_pSheet->object_count-1, CPoint( 10+Size->cx, 10+Size->cy ), Property_Info ); break;
		case OBJECT_VLINE   : NewVLineObject  (      m_pSheet->object_count-1, CPoint( 10+Size->cx, 10+Size->cy ), Property_Info ); break;
		case OBJECT_BOX     : NewBoxObject    (      m_pSheet->object_count-1, CPoint( 10+Size->cx, 10+Size->cy ), Property_Info ); break;
		case OBJECT_BITMAP  : NewBitmapObject (      m_pSheet->object_count-1, CPoint( 10+Size->cx, 10+Size->cy ), Property_Info ); break;
		case OBJECT_BARCODE : NewBarCodeObject(      m_pSheet->object_count-1, CPoint( 10+Size->cx, 10+Size->cy ), Property_Info ); break;
	}

	// 4. 객체의 크기를 설정한다.
	GetObjectRect( m_pSheet->object_count-1, ObjectRect );
	m_ObjectRect.SetRect( ObjectRect.left  - PICK_POINT_SIZE, ObjectRect.top    - PICK_POINT_SIZE, 
                          ObjectRect.right + PICK_POINT_SIZE, ObjectRect.bottom + PICK_POINT_SIZE );

	// 5. 객체를 보관용 비트맵에 PUT한다.
	PutObjectToStore( m_pSheet->object_count-1, OBJECT_ALL_DRAW );

	// 6. 객체를 그린다.
	DrawObject( pDC, m_pSheet->object_count-1 );
	DrawObjectToMemDC(); // WWW
ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// NewGridObject
//
// PURPOSE : 그리드 객체를 생성한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewGridObject( CDC * pDC, INT ObjectIdx, CPoint & Point, PROPERTY_INFO * Property_Info )
{
ECO_EXCEPTION_BEGIN
	INT     i, j, k, ExtraRow = 0, TempWidht, TempHeight;
	CSize   CharSize, ObjectRect;
	CRect   Rect;

	// 1. 그리드의 사이즈를 설정한다.
	GetCharSize( pDC, Property_Info->grid.font, CharSize );

	ObjectRect.cx = CharSize.cx * DEF_CHAR_LEN + 2 * INTEVAL_CX;
	ObjectRect.cy = CharSize.cy                + 2 * INTEVAL_CY;

	// 2. 그리드 전체 속성을 설정한다.
	if ( 0 < Property_Info->grid.page_row ) {
		ExtraRow++;
		Property_Info->grid.row_count++;
		m_pSheet->object[ObjectIdx]->grid.page_row = Property_Info->grid.row_count - 1;
	} else {
		Property_Info->grid.page_row = -1;
	}
	if ( 0 < Property_Info->grid.last_row ) {
		ExtraRow++;
		Property_Info->grid.row_count++;
		m_pSheet->object[ObjectIdx]->grid.last_row = Property_Info->grid.row_count - 1;
	} else {
		m_pSheet->object[ObjectIdx]->grid.last_row = -1;
	}

	_tcscpy( m_pSheet->object[ObjectIdx]->grid.name, Property_Info->grid.name );                        // 객체 이름
	m_pSheet->object[ObjectIdx]->grid.left       = Point.x;                                            // 객체 x 좌표
	m_pSheet->object[ObjectIdx]->grid.top        = Point.y;                                            // 객체 y 좌표 
	m_pSheet->object[ObjectIdx]->grid.width      = ObjectRect.cx * Property_Info->grid.col_count + 1;  // 객체 너비
	m_pSheet->object[ObjectIdx]->grid.height     = ObjectRect.cy * Property_Info->grid.row_count + 1;  // 객체 높이
	_tcscpy( m_pSheet->object[ObjectIdx]->grid.parent_name, _T("null") );
	m_pSheet->object[ObjectIdx]->grid.block_row  = -1;
	m_pSheet->object[ObjectIdx]->grid.block_col  = -1;
	m_pSheet->object[ObjectIdx]->grid.row_count  = Property_Info->grid.row_count;                      // 행개수
	m_pSheet->object[ObjectIdx]->grid.col_count  = Property_Info->grid.col_count;                      // 열개수
	m_pSheet->object[ObjectIdx]->grid.block      = NULL;                                               // 블록객체
    m_pSheet->object[ObjectIdx]->grid.cursor_row = 0;                                                  // 커서 행 
	m_pSheet->object[ObjectIdx]->grid.cursor_col = 0;                                                  // 커서 열
	m_pSheet->object[ObjectIdx]->grid.bind       = FALSE;                                              // 멀티 커서
    m_pSheet->object[ObjectIdx]->grid.anchor_row = 0;                                                  // 멀티 커서 시작 행
	m_pSheet->object[ObjectIdx]->grid.anchor_col = 0;                                                  // 멀티 커서 시작 열
	m_pSheet->object[ObjectIdx]->grid.horz_line  = Property_Info->grid.horz_line;                      // 수평선
	m_pSheet->object[ObjectIdx]->grid.vert_line  = Property_Info->grid.vert_line;                      // 수직선 
	m_pSheet->object[ObjectIdx]->grid.visible    = VISIBLE_ALL;

	// 3. 블럭을 생성을 할당한다.
	m_pSheet->object[ObjectIdx]->grid.block = ( REPORT_BLOCK ** )malloc( sizeof( REPORT_BLOCK * ) * Property_Info->grid.row_count );
	if ( NULL == m_pSheet->object[ObjectIdx]->grid.block ) {
		return -1;
	}

	m_pSheet->object[ObjectIdx]->grid.visible = VISIBLE_ALL;

	// 4. 블럭 속성을 설정한다.
	for ( i = 0; i < Property_Info->grid.row_count; i++ ) {
		// Column Alloc
		m_pSheet->object[ObjectIdx]->grid.block[i] = ( REPORT_BLOCK * )malloc( sizeof( REPORT_BLOCK ) * Property_Info->grid.col_count );
		if ( NULL == m_pSheet->object[ObjectIdx]->grid.block[i] ) {
			return -1;
		}
		// Column Setting
		for ( j = 0; j < Property_Info->grid.col_count; j++ ) {
			m_pSheet->object[ObjectIdx]->grid.block[i][j].left        = ObjectRect.cx * j;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].top         = ObjectRect.cy * i;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].width       = ObjectRect.cx + 1;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].height      = ObjectRect.cy + 1;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].text        = NULL;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].focus       = FALSE;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].horz_align  = ALGIN_HCENTER;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].fill        = FALSE;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].orientation = TEXT_ORIENTATION_HORZ;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].slash       = SLASH_NONE;
			wmemcpy( &m_pSheet->object[ObjectIdx]->grid.block[i][j].font, &Property_Info->grid.font, sizeof( LOGFONT ) );  
			m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_status = BIND_STATUS_NONE;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_row    = -1;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_col    = -1;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_width  = -1;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_height = -1;
			m_pSheet->object[ObjectIdx]->grid.block[i][j].child_count = 0;
			for ( k = 0; k < CHILD_MAX; k++ ) {
				_tcscpy( m_pSheet->object[ObjectIdx]->grid.block[i][j].child_name[k], _T("null") );
			}
		}
	}

	// Page Row, Last Row
	for ( k = 0; k < ExtraRow; k++ ) {
		Rect.SetRect( 0,
					  m_pSheet->object[ObjectIdx]->grid.row_count + k - ExtraRow,
					  m_pSheet->object[ObjectIdx]->grid.col_count - 1,
					  m_pSheet->object[ObjectIdx]->grid.row_count + k - ExtraRow );

		//++
		m_pSheet->object[ObjectIdx]->grid.block[Rect.top][Rect.left].origin_width  = m_pSheet->object[ObjectIdx]->grid.block[Rect.top][Rect.left].width;
		m_pSheet->object[ObjectIdx]->grid.block[Rect.top][Rect.left].origin_height = m_pSheet->object[ObjectIdx]->grid.block[Rect.top][Rect.left].height;
		//--
		
		// Binding
		for ( i = Rect.top; i <= Rect.bottom; i++ ) {
			for ( j = Rect.left; j <= Rect.right; j++ ) {
				if ( i == Rect.top && j == Rect.left ) {
					TempWidht  = m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].left - m_pSheet->object[ObjectIdx]->grid.block[i][j].left + m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].width;
					TempHeight = m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].top  - m_pSheet->object[ObjectIdx]->grid.block[i][j].top  + m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].height;
					m_pSheet->object[ObjectIdx]->grid.block[i][j].width         = TempWidht;
					m_pSheet->object[ObjectIdx]->grid.block[i][j].height        = TempHeight; 
					m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_status   = BIND_STATUS_MASTER;
					m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_row      = Rect.bottom;
					m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_col      = Rect.right;
				} else {
					m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_status = BIND_STATUS_SLAVE;
					m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_row    = Rect.top;
					m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_col    = Rect.left;
				}
			}
			if ( 0 == k ) {
				if ( 0 < m_pSheet->object[ObjectIdx]->grid.page_row ) {
					// 2000.1.16 m_pSheet->object[ObjectIdx]->grid.block[i][0].text = ( _TCHAR *)malloc( StringLength( _T("page") ) + 1 );
					// 2000.1.16 _tcscpy( m_pSheet->object[ObjectIdx]->grid.block[i][0].text, _T("page") );
					m_pSheet->object[ObjectIdx]->grid.block[i][0].text = NULL;
				} else {
					if ( 0 < m_pSheet->object[ObjectIdx]->grid.last_row ) {
						// 2000.1.16 m_pSheet->object[ObjectIdx]->grid.block[i][0].text = ( _TCHAR *)malloc( StringLength( _T("last") ) + 1 );
						// 2000.1.16 _tcscpy( m_pSheet->object[ObjectIdx]->grid.block[i][0].text, _T("last") );
						m_pSheet->object[ObjectIdx]->grid.block[i][0].text = NULL;
					}
				}
			}
			if ( 1 == k && 0 < m_pSheet->object[ObjectIdx]->grid.last_row ) {
				//m_pSheet->object[ObjectIdx]->grid.block[i][0].text = ( _TCHAR *)malloc( StringLength( _T("last") ) + 1 );
				//_tcscpy( m_pSheet->object[ObjectIdx]->grid.block[i][0].text, _T("last") );
				m_pSheet->object[ObjectIdx]->grid.block[i][0].text = NULL;
			}
		}
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// NewTableObject
//
// PURPOSE : 테이블 객체를 생성한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewTableObject( CDC * pDC, INT ObjectIdx, CPoint & Point, PROPERTY_INFO * Property_Info )
{
ECO_EXCEPTION_BEGIN
	INT     i, j;
	CSize   CharSize, ObjectRect;

	// 1. 테이블의 사이즈를 설정한다.
	GetCharSize( pDC, Property_Info->table.font, CharSize );

	ObjectRect.cx = CharSize.cx * DEF_CHAR_LEN + DEF_IDENT_CHAR_LEN * CharSize.cx + INT( CharSize.cx / 2.0 );
	ObjectRect.cy = CharSize.cy                + CharSize.cy;

	// 2. 테이블 전체 속성을 설정한다.
	m_pSheet->object[ObjectIdx]->table.left         = Point.x;
	m_pSheet->object[ObjectIdx]->table.top          = Point.y;
	m_pSheet->object[ObjectIdx]->table.width        = ObjectRect.cx * Property_Info->table.col_count + 1;  // 객체 너비
	m_pSheet->object[ObjectIdx]->table.height       = ObjectRect.cy * Property_Info->table.row_count + 1;  // 객체 높이
	_tcscpy( m_pSheet->object[ObjectIdx]->table.parent_name, _T("null") );
	m_pSheet->object[ObjectIdx]->table.block_row    = -1;
	m_pSheet->object[ObjectIdx]->table.block_col    = -1;
	_tcscpy( m_pSheet->object[ObjectIdx]->table.name, Property_Info->table.name );
	m_pSheet->object[ObjectIdx]->table.data_name[0] = '\0';
	m_pSheet->object[ObjectIdx]->table.data_field   = NULL;
	m_pSheet->object[ObjectIdx]->table.area         = TABLE_AREA_BLOCK;
	m_pSheet->object[ObjectIdx]->table.row_count    = Property_Info->table.row_count;
	m_pSheet->object[ObjectIdx]->table.col_count    = Property_Info->table.col_count;
    m_pSheet->object[ObjectIdx]->table.cell         = NULL;
    m_pSheet->object[ObjectIdx]->table.cursor_row   = 0;
	m_pSheet->object[ObjectIdx]->table.cursor_col   = 0;
	m_pSheet->object[ObjectIdx]->table.bind         = FALSE;
    m_pSheet->object[ObjectIdx]->table.anchor_row   = 0;
	m_pSheet->object[ObjectIdx]->table.anchor_col   = 0;
	m_pSheet->object[ObjectIdx]->table.horz_line    = Property_Info->table.horz_line;
	m_pSheet->object[ObjectIdx]->table.vert_line    = Property_Info->table.vert_line;
	m_pSheet->object[ObjectIdx]->table.line_expand  = TRUE;
	wmemcpy( &m_pSheet->object[ObjectIdx]->table.font, &Property_Info->table.font, sizeof( LOGFONT ) );
	m_pSheet->object[ObjectIdx]->table.char_cx      = CharSize.cx;
	m_pSheet->object[ObjectIdx]->table.char_cy      = CharSize.cy;
	m_pSheet->object[ObjectIdx]->table.line_max     = 1;
	m_pSheet->object[ObjectIdx]->table.visible      = VISIBLE_ALL;
	// 열단위
	m_pSheet->object[ObjectIdx]->table.data_field   = ( _TCHAR ** )malloc( sizeof( _TCHAR * ) * m_pSheet->object[ObjectIdx]->table.col_count );
	m_pSheet->object[ObjectIdx]->table.char_len     = ( INT  *  )malloc( sizeof( INT    ) * m_pSheet->object[ObjectIdx]->table.col_count );
	m_pSheet->object[ObjectIdx]->table.char_max     = ( INT  *  )malloc( sizeof( INT    ) * m_pSheet->object[ObjectIdx]->table.col_count ); 
	m_pSheet->object[ObjectIdx]->table.indent_len   = ( INT  *  )malloc( sizeof( INT    ) * m_pSheet->object[ObjectIdx]->table.col_count );
	m_pSheet->object[ObjectIdx]->table.horz_align   = ( INT  *  )malloc( sizeof( INT    ) * m_pSheet->object[ObjectIdx]->table.col_count );
	m_pSheet->object[ObjectIdx]->table.vert_align   = ( INT  *  )malloc( sizeof( INT    ) * m_pSheet->object[ObjectIdx]->table.col_count );

	for ( i = 0; i < m_pSheet->object[ObjectIdx]->table.col_count; i++ ) {
		m_pSheet->object[ObjectIdx]->table.data_field[i] = NULL;
		m_pSheet->object[ObjectIdx]->table.char_len  [i] = DEF_CHAR_LEN;
		m_pSheet->object[ObjectIdx]->table.char_max  [i] = DEF_CHAR_LEN;
		m_pSheet->object[ObjectIdx]->table.indent_len[i] = DEF_IDENT_CHAR_LEN;
		m_pSheet->object[ObjectIdx]->table.horz_align[i] = ALGIN_LEFT;
		m_pSheet->object[ObjectIdx]->table.vert_align[i] = ALGIN_VCENTER;
	}

	// 3. 셀을 생성을 할당한다.
	m_pSheet->object[ObjectIdx]->table.cell = ( REPORT_CELL ** )malloc( sizeof( REPORT_CELL * ) * Property_Info->table.row_count );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.cell ) {
		return -1;
	}

	// 4. 셀 속성을 설정한다.
	for ( i = 0; i < m_pSheet->object[ObjectIdx]->table.row_count; i++ ) {
		m_pSheet->object[ObjectIdx]->table.cell[i] = ( REPORT_CELL * )malloc( sizeof( REPORT_CELL ) * m_pSheet->object[ObjectIdx]->table.col_count );
		if ( NULL == m_pSheet->object[ObjectIdx]->table.cell[i] ) {
			return -1;
		}
		for ( j = 0; j < m_pSheet->object[ObjectIdx]->table.col_count; j++ ) {
			m_pSheet->object[ObjectIdx]->table.cell[i][j].left     = ObjectRect.cx * j;
			m_pSheet->object[ObjectIdx]->table.cell[i][j].top      = ObjectRect.cy * i;
			m_pSheet->object[ObjectIdx]->table.cell[i][j].width    = ObjectRect.cx + 1;
			m_pSheet->object[ObjectIdx]->table.cell[i][j].height   = ObjectRect.cy + 1;
			m_pSheet->object[ObjectIdx]->table.cell[i][j].text     = NULL;
			m_pSheet->object[ObjectIdx]->table.cell[i][j].focus    = FALSE;
			m_pSheet->object[ObjectIdx]->table.cell[i][j].fill     = FALSE;
		}
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// NewHLineObject
//
// PURPOSE : 수평선 객체를 생성한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewHLineObject( INT ObjectIdx, CPoint & Point, PROPERTY_INFO * Property_Info )
{
ECO_EXCEPTION_BEGIN

	_tcscpy( m_pSheet->object[ObjectIdx]->hline.name, Property_Info->hline.name );
	m_pSheet->object[ObjectIdx]->hline.left         = Point.x;
	m_pSheet->object[ObjectIdx]->hline.top          = Point.y;
	m_pSheet->object[ObjectIdx]->hline.width        = HLINE_DEFAULT_WITDH;
	m_pSheet->object[ObjectIdx]->hline.height       = HLINE_DEFAULT_HEIGHT;
	_tcscpy( m_pSheet->object[ObjectIdx]->hline.parent_name, _T("null") );
	m_pSheet->object[ObjectIdx]->hline.block_row    = -1;
	m_pSheet->object[ObjectIdx]->hline.block_col    = -1;
	m_pSheet->object[ObjectIdx]->hline.visible      = VISIBLE_ALL;
	m_pSheet->object[ObjectIdx]->hline.thick        = 1;
	m_pSheet->object[ObjectIdx]->hline.style        = Property_Info->hline.style;
ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// NewVLineObject
//
// PURPOSE : 수직선 객체를 생성한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewVLineObject( INT ObjectIdx, CPoint & Point, PROPERTY_INFO * Property_Info )
{
ECO_EXCEPTION_BEGIN

	_tcscpy( m_pSheet->object[ObjectIdx]->vline.name, Property_Info->vline.name );
	m_pSheet->object[ObjectIdx]->vline.left         = Point.x;
	m_pSheet->object[ObjectIdx]->vline.top          = Point.y;
	m_pSheet->object[ObjectIdx]->vline.width        = VLINE_DEFAULT_WITDH;
	m_pSheet->object[ObjectIdx]->vline.height       = VLINE_DEFAULT_HEIGHT;
	_tcscpy( m_pSheet->object[ObjectIdx]->vline.parent_name, _T("null") );
	m_pSheet->object[ObjectIdx]->vline.block_row    = -1;
	m_pSheet->object[ObjectIdx]->vline.block_col    = -1;
	m_pSheet->object[ObjectIdx]->vline.visible      = VISIBLE_ALL;
	m_pSheet->object[ObjectIdx]->vline.thick        = 1;
	m_pSheet->object[ObjectIdx]->vline.style        = Property_Info->vline.style;
ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// NewBoxObject
//
// PURPOSE : 상자 객체를 생성한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewBoxObject( INT ObjectIdx, CPoint & Point, PROPERTY_INFO * Property_Info )
{
ECO_EXCEPTION_BEGIN

	_tcscpy( m_pSheet->object[ObjectIdx]->box.name, Property_Info->box.name );
	m_pSheet->object[ObjectIdx]->box.left         = Point.x;
	m_pSheet->object[ObjectIdx]->box.top          = Point.y;
	m_pSheet->object[ObjectIdx]->box.width        = BOX_DEFAULT_WITDH;
	m_pSheet->object[ObjectIdx]->box.height       = BOX_DEFAULT_HEIGHT;
	_tcscpy( m_pSheet->object[ObjectIdx]->box.parent_name, _T("null") );
	m_pSheet->object[ObjectIdx]->box.block_row    = -1;
	m_pSheet->object[ObjectIdx]->box.block_col    = -1;
	m_pSheet->object[ObjectIdx]->box.visible      = VISIBLE_ALL;
	m_pSheet->object[ObjectIdx]->box.thick        = 1;
	m_pSheet->object[ObjectIdx]->box.style        = Property_Info->box.style;
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// NewBitmapObject
//
// PURPOSE : 비트맵 객체를 생성한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewBitmapObject( INT ObjectIdx, CPoint & Point, PROPERTY_INFO * Property_Info )
{
ECO_EXCEPTION_BEGIN
	_tcscpy( m_pSheet->object[ObjectIdx]->bitmap.name     , Property_Info->bitmap.name      );
	_tcscpy( m_pSheet->object[ObjectIdx]->bitmap.file_name, Property_Info->bitmap.file_name );

	m_pSheet->object[ObjectIdx]->bitmap.left         = Point.x;
	m_pSheet->object[ObjectIdx]->bitmap.top          = Point.y;
	m_pSheet->object[ObjectIdx]->bitmap.width        = BITMAP_DEFAULT_WITDH;
	m_pSheet->object[ObjectIdx]->bitmap.height       = BITMAP_DEFAULT_HEIGHT;
	_tcscpy( m_pSheet->object[ObjectIdx]->bitmap.parent_name, _T("null") );
	m_pSheet->object[ObjectIdx]->bitmap.block_row    = -1;
	m_pSheet->object[ObjectIdx]->bitmap.block_col    = -1;
	m_pSheet->object[ObjectIdx]->bitmap.visible      = VISIBLE_ALL;

	m_pSheet->object[ObjectIdx]->bitmap.dib = new CPicture;
	m_pSheet->object[ObjectIdx]->bitmap.dib->Load( Property_Info->bitmap.file_name );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// NewBarCodeObject
//
// PURPOSE : 바코드 객체를 생성한다.
// AUTHOR  : 우상옥
//========================================================================
INT CWorkSheet::NewBarCodeObject( INT ObjectIdx, CPoint & Point, PROPERTY_INFO * Property_Info )
{
ECO_EXCEPTION_BEGIN
	_tcscpy( m_pSheet->object[ObjectIdx]->barcode.name , Property_Info->barcode.name  );
	_tcscpy( m_pSheet->object[ObjectIdx]->barcode.value, Property_Info->barcode.value );
	m_pSheet->object[ObjectIdx]->barcode.code = Property_Info->barcode.code;

	m_pSheet->object[ObjectIdx]->barcode.left         = Point.x;
	m_pSheet->object[ObjectIdx]->barcode.top          = Point.y;
	m_pSheet->object[ObjectIdx]->barcode.width        = BARCODE_DEFAULT_WITDH;
	m_pSheet->object[ObjectIdx]->barcode.height       = BARCODE_DEFAULT_HEIGHT;
	_tcscpy( m_pSheet->object[ObjectIdx]->barcode.parent_name, _T("null") );
	m_pSheet->object[ObjectIdx]->barcode.block_row    = -1;
	m_pSheet->object[ObjectIdx]->barcode.block_col    = -1;
	m_pSheet->object[ObjectIdx]->barcode.visible      = VISIBLE_ALL;

	m_pSheet->object[ObjectIdx]->barcode.bc = new CBarcodCtrl;
	m_pSheet->object[ObjectIdx]->barcode.bc->SetStyle( m_pSheet->object[ObjectIdx]->barcode.code );
	m_pSheet->object[ObjectIdx]->barcode.bc->SetCaption( CString( m_pSheet->object[ObjectIdx]->barcode.value ) );

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// GetPickRect
//====================================================================================
INT CWorkSheet::GetPickRect( INT Kind, INT * PickPointCnt, CRect * Rect, INT Left, INT Top, INT Right, INT Bottom )
{
ECO_EXCEPTION_BEGIN
	INT HorzMiddle, VertMiddle;

	switch ( Kind ) {
		case OBJECT_TABLE   :
		case OBJECT_GRID    :
		case OBJECT_BOX     :
		case OBJECT_BITMAP  :
		case OBJECT_BARCODE :
			HorzMiddle = Left + ( Right  - Left ) / 2 - PICK_POINT_SIZE;
			VertMiddle = Top  + ( Bottom - Top  ) / 2 - PICK_POINT_SIZE;

			Rect[0].SetRect( Left-PICK_POINT_SIZE, Top-PICK_POINT_SIZE, Left+PICK_POINT_SIZE, Top+PICK_POINT_SIZE );         // 1
			Rect[1].SetRect( HorzMiddle, Top-PICK_POINT_SIZE, HorzMiddle+2*PICK_POINT_SIZE, Top+PICK_POINT_SIZE );           // 2
			Rect[2].SetRect( Right-PICK_POINT_SIZE, Top-PICK_POINT_SIZE, Right+PICK_POINT_SIZE, Top+PICK_POINT_SIZE );       // 3

			Rect[3].SetRect( Left-PICK_POINT_SIZE, VertMiddle, Left+PICK_POINT_SIZE, VertMiddle+2*PICK_POINT_SIZE );         // 4 
			Rect[4].SetRect( Right-PICK_POINT_SIZE, VertMiddle, Right+PICK_POINT_SIZE, VertMiddle+2* PICK_POINT_SIZE );      // 5

			Rect[5].SetRect( Left-PICK_POINT_SIZE, Bottom-PICK_POINT_SIZE, Left+PICK_POINT_SIZE, Bottom+PICK_POINT_SIZE );   // 6
			Rect[6].SetRect( HorzMiddle, Bottom-PICK_POINT_SIZE, HorzMiddle+2*PICK_POINT_SIZE, Bottom+PICK_POINT_SIZE );     // 7
			Rect[7].SetRect( Right-PICK_POINT_SIZE, Bottom-PICK_POINT_SIZE, Right+PICK_POINT_SIZE, Bottom+PICK_POINT_SIZE ); // 8

			*PickPointCnt = 8;
			break;
		case OBJECT_HLINE :
		case OBJECT_VLINE :
			Rect[0].SetRect( Left-PICK_POINT_SIZE, Top-PICK_POINT_SIZE, Left+PICK_POINT_SIZE, Top+PICK_POINT_SIZE );         // 1
			Rect[1].SetRect( Right-PICK_POINT_SIZE, Bottom-PICK_POINT_SIZE, Right+PICK_POINT_SIZE, Bottom+PICK_POINT_SIZE ); // 8

			*PickPointCnt = 2;
			break;
	}

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// SearchObject
//====================================================================================
INT CWorkSheet::SearchObject( CPoint & point, INT * PickPoint )
{
ECO_EXCEPTION_BEGIN
	CRect Rect, PickRect[8];
	INT   i, j, left, top, width, height, PickPointCnt;
	
	for ( i = m_pSheet->object_count-1; i >= 0; i-- ) {
		switch ( m_pSheet->object[i]->kind ) {
			case OBJECT_TABLE   :
			case OBJECT_GRID    :
			case OBJECT_BITMAP  :
			case OBJECT_BARCODE :
				left   = m_pSheet->object[i]->comm.left;
				top    = m_pSheet->object[i]->comm.top;
				width  = m_pSheet->object[i]->comm.width;
				height = m_pSheet->object[i]->comm.height;
				//++
				GetPickRect( OBJECT_BOX, &PickPointCnt, PickRect, left, top, left + width, top + height );
				for ( j = 0; j < PickPointCnt; j++ ) {
					if ( TRUE == PickRect[j].PtInRect( point ) ) {
						*PickPoint = j + 1;
						return i;
					}
				}
				//--
				Rect.SetRect( left, top, left + width, top + height );
				if ( TRUE == Rect.PtInRect( point ) ) {
					*PickPoint = OBJECT_MOVE;
					return i;
				}
				break;
			case OBJECT_BOX  :
				left   = m_pSheet->object[i]->comm.left;
				top    = m_pSheet->object[i]->comm.top;
				width  = m_pSheet->object[i]->comm.width;
				height = m_pSheet->object[i]->comm.height;
				//++
				GetPickRect( OBJECT_BOX, &PickPointCnt, PickRect, left, top, left + width, top + height );
				for ( j = 0; j < PickPointCnt; j++ ) {
					if ( TRUE == PickRect[j].PtInRect( point ) ) {
						*PickPoint = j + 1;
						return i;
					}
				}
				//--
				Rect.SetRect( left - PICK_POINT_SIZE, top - PICK_POINT_SIZE, left + width + PICK_POINT_SIZE, top + height + PICK_POINT_SIZE );
				if ( TRUE == Rect.PtInRect( point ) ) {
					Rect.SetRect( left + PICK_POINT_SIZE, top + PICK_POINT_SIZE, left + width - PICK_POINT_SIZE, top + height - PICK_POINT_SIZE );
					if ( FALSE == Rect.PtInRect( point ) ) {
						*PickPoint = OBJECT_MOVE;
						return i;
					}
				}
				break;
			case OBJECT_HLINE :
				left   = m_pSheet->object[i]->hline.left;
				top    = m_pSheet->object[i]->hline.top;
				width  = m_pSheet->object[i]->hline.width;
				height = m_pSheet->object[i]->hline.height;

				//++
				GetPickRect( OBJECT_HLINE, &PickPointCnt, PickRect, left, top, left + width, top + height );
				for ( j = 0; j < PickPointCnt; j++ ) {
					if ( TRUE == PickRect[j].PtInRect( point ) ) {
						*PickPoint = j + 1;
						return i;
					}
				}
				//--
				Rect.SetRect( left, top - PICK_POINT_SIZE, left + width, top + height + PICK_POINT_SIZE );
				if ( TRUE == Rect.PtInRect( point ) ) {
					*PickPoint = OBJECT_MOVE;
					return i;
				}
				break;
			case OBJECT_VLINE :
				left   = m_pSheet->object[i]->vline.left;
				top    = m_pSheet->object[i]->vline.top;
				width  = m_pSheet->object[i]->vline.width;
				height = m_pSheet->object[i]->vline.height;

				//++
				GetPickRect( OBJECT_VLINE, &PickPointCnt, PickRect, left, top, left + width, top + height );
				for ( j = 0; j < PickPointCnt; j++ ) {
					if ( TRUE == PickRect[j].PtInRect( point ) ) {
						*PickPoint = j + 1;
						return i;
					}
				}
				//--
				Rect.SetRect( left - PICK_POINT_SIZE, top, left + width + PICK_POINT_SIZE, top + height );
				if ( TRUE == Rect.PtInRect( point ) ) {
					*PickPoint = OBJECT_MOVE;
					return i;
				}
				break;
		}
	}

ECO_EXCEPTION_END
	return -1;
}

//====================================================================================
// IsSelected
//====================================================================================
BOOL CWorkSheet::IsSelected( INT ObjectIdx )
{
ECO_EXCEPTION_BEGIN
	if ( -1 != m_pSheet->object[ObjectIdx]->select ) {
		return TRUE;
	}

ECO_EXCEPTION_END
	return FALSE;
}

//====================================================================================
// SelectObject
//====================================================================================
INT CWorkSheet::SelectObject( INT ObjectIdx, BOOL Multi )
{
ECO_EXCEPTION_BEGIN
	INT i;

	if ( OBJET_MULTI == Multi ) {
		m_SelectCnt++;
	} else {
		m_SelectCnt = 0;
	}
	if ( -1 == ObjectIdx ) { // Clear Select
		m_SelectCnt = 0;
	}

	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( i == ObjectIdx ) {
			m_pSheet->object[i]->select = m_SelectCnt;
		} else {
			if ( TRUE != Multi ) {
				m_pSheet->object[i]->select = -1;
			}
		}
	}

	return 0;
ECO_EXCEPTION_END
}

//====================================================================================
// Delete
//====================================================================================
INT CWorkSheet::Delete( CDC * pDC, CSize & Size )
{
ECO_EXCEPTION_BEGIN
	INT  i;
	BOOL Freed;

	AdjustScrollDC( pDC, &Size );

	// Delete Object In Memory
	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		Freed = FALSE;
		if ( -1 == i ) {
			continue;
		}
		if ( -1 == m_pSheet->object[i]->select ) {
			continue;
		}
		if ( i < m_pSheet->object_count - 1 ) {
			FreeObjectInfo( m_pSheet, i );
			free( m_pSheet->object[i] );
			Freed = TRUE;
			wmemcpy( &m_pSheet->object[i], &m_pSheet->object[i+1], sizeof( REPORT_OBJECT * ) * ( m_pSheet->object_count - i - 1 ) );
			if ( m_CaretInfo.object_index == i ) {
				KillFocus();
			}
			i--;
		}
		if ( FALSE == Freed ) {
			if ( m_CaretInfo.object_index == i ) {
				KillFocus();
			}
			FreeObjectInfo( m_pSheet, m_pSheet->object_count-1 );
			free( m_pSheet->object[m_pSheet->object_count-1] );
		}
		m_pSheet->object_count--;
		m_pSheet->object = ( REPORT_OBJECT ** )realloc( m_pSheet->object, sizeof( REPORT_OBJECT * ) * m_pSheet->object_count );
	}

	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// SetCurSheet
//====================================================================================
INT CWorkSheet::SetSheet( INT SheetIdx )
{
ECO_EXCEPTION_BEGIN
	// 1. Sheet을 바꾼다.
	m_CurSheet = SheetIdx;
	m_pSheet   = &m_ReportInfo.sheet[m_CurSheet];

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// SetCurSheet
//====================================================================================
INT CWorkSheet::SetCurSheet( CDC * pDC, CSize & Size, _TCHAR * SheetName )
{
ECO_EXCEPTION_BEGIN
	INT i;

	// 1. Sheet를 바꾼다.
	for ( i = 0; i < m_ReportInfo.sheet_count; i++ ) {
		if ( 0 == _tcsicmp( m_ReportInfo.sheet[i].title, SheetName ) ) {
			m_CurSheet = i;
			m_pSheet   = &m_ReportInfo.sheet[m_CurSheet];
			break;
		}
	}

	// 2. 복사정보를 삭제한다.
	if ( NULL == m_ClipSheet.object ) {
		FreeObject( &m_ClipSheet );
		free( m_ClipSheet.object );
	}

	// 3. ClipSheet을 초기화한다.
	m_ClipSheet.object_count = 0;
	m_ClipSheet.object       = NULL;

	// 4. 복사시 원본과 비껴 붙이는 좌표를 초기화한다.
	m_CopyPoint.x = 0;
	m_CopyPoint.y = 0;

	// 5. 다시 그린다. 
	AdjustScrollDC( pDC, &Size );
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return -1;
}

//====================================================================================
// GetCurSheet
//====================================================================================
INT CWorkSheet::GetCurSheet( _TCHAR * SheetName )
{
ECO_EXCEPTION_BEGIN
	if ( NULL == SheetName ) {
		return -1;
	}

	_tcscpy( SheetName, m_pSheet->title );

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// InsertSheet
//====================================================================================
INT CWorkSheet::InsertSheet( _TCHAR * SheetName )
{
ECO_EXCEPTION_BEGIN
	if ( NULL == SheetName ) {
		return -1;
	}

	m_ReportInfo.sheet_count++;
	m_ReportInfo.sheet = ( REPORT_SHEET * )realloc( m_ReportInfo.sheet, sizeof( REPORT_SHEET ) * m_ReportInfo.sheet_count );
	if ( NULL == m_ReportInfo.sheet ) {
		return -1;
	}

	_tcscpy( m_ReportInfo.sheet[m_ReportInfo.sheet_count-1].title, SheetName );
	m_ReportInfo.sheet[m_ReportInfo.sheet_count-1].object_count = 0;
	m_ReportInfo.sheet[m_ReportInfo.sheet_count-1].object_id    = 0;
	m_ReportInfo.sheet[m_ReportInfo.sheet_count-1].object       = NULL;

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// UpdateSheet
//====================================================================================
INT CWorkSheet::UpdateSheet( _TCHAR * OldSheetName, _TCHAR * NewSheetName )
{
ECO_EXCEPTION_BEGIN
	INT i;

	if ( NULL == OldSheetName || NULL == NewSheetName ) {
		return -1;
	}

	for ( i = 0; i < m_ReportInfo.sheet_count; i++ ) {
		if ( 0 == _tcsicmp( m_ReportInfo.sheet[i].title, OldSheetName ) ) {
			_tcscpy( m_ReportInfo.sheet[i].title, NewSheetName );
			return 0;
		}
	}

ECO_EXCEPTION_END
	return -1;
}

//====================================================================================
// DeleteSheet
//====================================================================================
INT CWorkSheet::DeleteSheet( _TCHAR * SheetName )
{
ECO_EXCEPTION_BEGIN
	INT i;

	if ( NULL == SheetName ) {
		return -1;
	}

	for ( i = 0; i < m_ReportInfo.sheet_count; i++ ) {
		if ( 0 == _tcsicmp( m_ReportInfo.sheet[i].title, SheetName ) ) {
			FreeObject( &m_ReportInfo.sheet[i] );
			if ( i != m_ReportInfo.sheet_count - 1 ) {
				wmemcpy( &m_ReportInfo.sheet[i], &m_ReportInfo.sheet[i+1], ( m_ReportInfo.sheet_count - i - 1 ) * sizeof( REPORT_SHEET ) );
			}
			m_ReportInfo.sheet_count--;
			m_ReportInfo.sheet = ( REPORT_SHEET * )realloc( m_ReportInfo.sheet, sizeof( REPORT_SHEET ) * m_ReportInfo.sheet_count );
			if ( NULL == m_ReportInfo.sheet ) {
				return -1;
			}
			return 0;
		}
	}

ECO_EXCEPTION_END
	return -1;
}

//====================================================================================
// Order
//
// PURPOSE : 객체의 순서를 조정한다.
// AUTHOR  : 우상옥
//====================================================================================
INT CWorkSheet::Order( CDC * pDC, CSize & Size, INT Mode )
{
ECO_EXCEPTION_BEGIN
	INT             i, SelectCnt, ObjectIdx;
	REPORT_OBJECT * pTmpObject;

	// 1. 스크롤좌표를 조정한다.
	AdjustScrollDC( pDC, &Size );

	// 2. 객체를 하나만 선택했는지 조사한다.
	SelectCnt = 0;
	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( -1 != m_pSheet->object[i]->select ) {
			ObjectIdx = i;
			SelectCnt++;
		}
	}

	if ( 0 >= SelectCnt ) {
		return -1;
	}

	if ( 1 < SelectCnt ) {
		return -2;
	}

	// 3. 객체의 Oder를 조정한다.
	switch ( Mode ) {
		case DESIGN_BRING_TO_FORWARD :
			if ( ObjectIdx < m_pSheet->object_count - 1 ) {
				pTmpObject = m_pSheet->object[ObjectIdx+1];
				m_pSheet->object[ObjectIdx+1] = m_pSheet->object[ObjectIdx];
				m_pSheet->object[ObjectIdx  ] = pTmpObject;
			}
			break;
		case DESIGN_SEND_TO_BACKWARD :
			if ( ObjectIdx > 0 ) {
				pTmpObject = m_pSheet->object[ObjectIdx-1];
				m_pSheet->object[ObjectIdx-1] = m_pSheet->object[ObjectIdx];
				m_pSheet->object[ObjectIdx  ] = pTmpObject;
			}
			break;
		case DESIGN_BRING_TO_FRONT :
			if ( ObjectIdx < m_pSheet->object_count - 1 ) {
				pTmpObject = m_pSheet->object[m_pSheet->object_count-1];
				m_pSheet->object[m_pSheet->object_count-1] = m_pSheet->object[ObjectIdx];
				m_pSheet->object[ObjectIdx] = pTmpObject;
			}
			break;
		case DESIGN_SEND_TO_BACK :
			if ( ObjectIdx > 0 ) {
				pTmpObject = m_pSheet->object[0];
				m_pSheet->object[0] = m_pSheet->object[ObjectIdx];
				m_pSheet->object[ObjectIdx] = pTmpObject;
			}
			break;
	}

	// 2. 디시그리기
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

INT CWorkSheet::Align( INT Mode, CDC * pDC, CSize & Size  )
{
ECO_EXCEPTION_BEGIN
	INT i, Right, Bottom, PoleRight, PoleBottom, LastOrderObjectIdx = -1;
	INT MinX = m_ReportInfo.sheet_width, MaxX = 0, MinY = m_ReportInfo.sheet_height, MaxY = 0, OffsetX, OffsetY;

	// 1. '수평중앙/수직중앙' 정렬을 제외한 객체정을 처리한다.
	if ( DESIGN_HORZ_CENTER != Mode && DESIGN_VERT_CENTER != Mode ) {
		// Search Last Order
		for ( i = 0; i < m_pSheet->object_count; i++ ) {
			if ( m_SelectCnt == m_pSheet->object[i]->select ) {
				LastOrderObjectIdx = i;
				break;
			}
		}

		// Design
		for ( i = 0; i < m_pSheet->object_count; i++ ) {
			if ( -1 == m_pSheet->object[i]->select ) {
				continue;
			}
			if ( OBJECT_GRID == m_pSheet->object[i]->kind && i != LastOrderObjectIdx ) {
				if ( 1 < m_pSheet->object[i]->grid.row_count || 1 < m_pSheet->object[i]->grid.col_count ) {
					AfxMessageBox( _T("1행 1열 그리드만 사용 가능합니다."), MB_OK );
					goto _EXIT_;
				}
			}
			if ( OBJECT_TABLE == m_pSheet->object[i]->kind && i != LastOrderObjectIdx ) {
				if ( 1 < m_pSheet->object[i]->table.row_count || 1 < m_pSheet->object[i]->table.col_count ) {
					AfxMessageBox( _T("1행 1열 테이블만 사용 가능합니다."), MB_OK );
					goto _EXIT_;
				}
			}

			switch ( Mode ) {
				case DESIGN_ALIGN_TOP :
					m_pSheet->object[i]->comm.top = m_pSheet->object[LastOrderObjectIdx]->comm.top;
					break;
				case DESIGN_ALIGN_LEFT :
					m_pSheet->object[i]->comm.left = m_pSheet->object[LastOrderObjectIdx]->comm.left;
					break;
				case DESIGN_ALIGN_RIGHT :
					PoleRight = m_pSheet->object[LastOrderObjectIdx]->comm.left + m_pSheet->object[LastOrderObjectIdx]->comm.width;
					Right     = m_pSheet->object[i]->comm.left                  + m_pSheet->object[i]->comm.width;
					m_pSheet->object[i]->comm.left += ( PoleRight - Right );
					break;
				case DESIGN_ALIGN_BOTTOM :
					PoleBottom = m_pSheet->object[LastOrderObjectIdx]->comm.top + m_pSheet->object[LastOrderObjectIdx]->comm.height;
					Bottom     = m_pSheet->object[i]->comm.top                  + m_pSheet->object[i]->comm.height;
					m_pSheet->object[i]->comm.top += ( PoleBottom - Bottom );
					break;
				case DESIGN_SAME_WIDTH :
					m_pSheet->object[i]->comm.width = m_pSheet->object[LastOrderObjectIdx]->comm.width;
					// 그리드/테이블안의 원소의 크기를 조정한다.
					if ( OBJECT_GRID == m_pSheet->object[i]->kind ) {
						m_pSheet->object[i]->grid.block[0][0].width = m_pSheet->object[LastOrderObjectIdx]->comm.width;
					}
					if ( OBJECT_TABLE == m_pSheet->object[i]->kind ) {
						m_pSheet->object[i]->table.cell[0][0].width = m_pSheet->object[LastOrderObjectIdx]->comm.width;
					}
					break;
				case DESIGN_SAME_HEIGHT :
					m_pSheet->object[i]->comm.height = m_pSheet->object[LastOrderObjectIdx]->comm.height;
					// 그리드/테이블안의 원소의 크기를 조정한다.
					if ( OBJECT_GRID == m_pSheet->object[i]->kind ) {
						m_pSheet->object[i]->grid.block[0][0].height = m_pSheet->object[LastOrderObjectIdx]->comm.height;
					}
					if ( OBJECT_TABLE == m_pSheet->object[i]->kind ) {
						m_pSheet->object[i]->table.cell[0][0].height = m_pSheet->object[LastOrderObjectIdx]->comm.height;
					}
					break;
				case DESIGN_SAME_SIZE :
					m_pSheet->object[i]->comm.width  = m_pSheet->object[LastOrderObjectIdx]->comm.width;
					m_pSheet->object[i]->comm.height = m_pSheet->object[LastOrderObjectIdx]->comm.height;
					// 그리드/테이블안의 원소의 크기를 조정한다.
					if ( OBJECT_GRID == m_pSheet->object[i]->kind ) {
						m_pSheet->object[i]->grid.block[0][0].width  = m_pSheet->object[LastOrderObjectIdx]->comm.width;
						m_pSheet->object[i]->grid.block[0][0].height = m_pSheet->object[LastOrderObjectIdx]->comm.height;
					}
					if ( OBJECT_TABLE == m_pSheet->object[i]->kind ) {
						m_pSheet->object[i]->table.cell[0][0].width  = m_pSheet->object[LastOrderObjectIdx]->comm.width;
						m_pSheet->object[i]->table.cell[0][0].height = m_pSheet->object[LastOrderObjectIdx]->comm.height;
					}
					break;
			}
		}
	} else { // '수평중앙/수직중앙' 정렬을 처리한다.
		// SEARCH MIN, MAX
		for ( i = 0; i < m_pSheet->object_count; i++ ) {
			if ( -1 == m_pSheet->object[i]->select ) {
				continue;
			}
			if ( MinX > m_pSheet->object[i]->comm.left ) {
				MinX = m_pSheet->object[i]->comm.left;
			}
			if ( MinY > m_pSheet->object[i]->comm.top ) {
				MinY = m_pSheet->object[i]->comm.top;
			}
			if ( MaxX < m_pSheet->object[i]->comm.left + m_pSheet->object[i]->comm.width ) {
				MaxX = m_pSheet->object[i]->comm.left + m_pSheet->object[i]->comm.width;
			}
			if ( MaxY < m_pSheet->object[i]->comm.top + m_pSheet->object[i]->comm.height ) {
				MaxY = m_pSheet->object[i]->comm.top + m_pSheet->object[i]->comm.height;
			}
		}		

		OffsetX = INT( double( m_ReportInfo.sheet_width - ( MaxX - MinX ) ) / 2.0 );
		OffsetY = INT( double( m_ReportInfo.sheet_height - ( MaxY - MinY ) ) / 2.0 );

		// ALIGN
		switch ( Mode ) {
			case DESIGN_HORZ_CENTER :
				for ( i = 0; i < m_pSheet->object_count; i++ ) {
					if ( -1 == m_pSheet->object[i]->select ) {
						continue;
					}
					m_pSheet->object[i]->comm.left += ( OffsetX - MinX );
				}
				break;
			case DESIGN_VERT_CENTER :
				for ( i = 0; i < m_pSheet->object_count; i++ ) {
					if ( -1 == m_pSheet->object[i]->select ) {
						continue;
					}
					m_pSheet->object[i]->comm.top += ( OffsetY -MinY );
				}
				break;
		}
	}

_EXIT_:
	// Redrawing
	AdjustScrollDC( pDC, &Size );
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// P R I N T
//====================================================================================
INT CWorkSheet::Print( VOID )
{
ECO_EXCEPTION_BEGIN
    CDC     dc;
	BOOL    bPrintingOK;
    DOCINFO di;

	CPrintDialog printDlg(FALSE);
    if (printDlg.DoModal() == IDCANCEL)
        return -1;
    dc.Attach(printDlg.GetPrinterDC());
    dc.m_bPrinting = TRUE;
    CString strTitle = _T("레포트편집기");
    //strTitle.LoadString(AFX_IDS_APP_TITLE);

    ::ZeroMemory (&di, sizeof (DOCINFO));    
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;
    bPrintingOK = dc.StartDoc( &di );
    // Get the printing extents and store in the m_rectDraw field of a 
    // CPrintInfo object    
    CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

    for ( INT i = 0; i < 1; i++ ) {
        dc.StartPage();
        //Info.m_nCurPage = page;
        Info.m_nCurPage = i;

		// Mapping
		dc.SetMapMode( MM_ISOTROPIC );
		dc.SetWindowExt( m_ReportInfo.sheet_width, m_ReportInfo.sheet_height );
		dc.SetViewportExt( Info.m_rectDraw.Width(), Info.m_rectDraw.Height() );

		// Drawing
		// KINITI SetCurPage( END_PAGE );
		DrawObject( &dc );
		// KINITI SetCurPage( FIRST_PAGE );

		// 
        bPrintingOK = (dc.EndPage() > 0);
    }

    if ( bPrintingOK ) {
        dc.EndDoc();
	} else {
        dc.AbortDoc();
	}

    dc.Detach(); 

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// SetPaperDirection
//========================================================================
INT CWorkSheet::SetPaperDirection( INT Orientation, CWinApp * pWinApp )
{
ECO_EXCEPTION_BEGIN
	PRINTDLG pd;
	wmemset(&pd, 0, sizeof(pd));

	if ( pWinApp->GetPrinterDeviceDefaults( &pd ) )	{
		//Lock memory handle
		DEVMODE FAR* pDevMode = ( DEVMODE FAR* )::GlobalLock( pd.hDevMode );
		LPDEVNAMES lpDevNames;
		LPTSTR lpszDriverName, lpszDeviceName, lpszPortName;
		HANDLE hPrinter;
		if (pDevMode) {
			if ( DMORIENT_PORTRAIT != Orientation && DMORIENT_LANDSCAPE != Orientation ) {
				return -1;
			}
			pDevMode->dmOrientation = Orientation;
			
			// Unlock memory handle
			lpDevNames     = ( LPDEVNAMES )::GlobalLock( pd.hDevNames );
			lpszDriverName = ( LPTSTR )lpDevNames + lpDevNames->wDriverOffset;
			lpszDeviceName = ( LPTSTR )lpDevNames + lpDevNames->wDeviceOffset;
			lpszPortName   = ( LPTSTR )lpDevNames + lpDevNames->wOutputOffset;

			::OpenPrinter( lpszDeviceName, &hPrinter, NULL );

			::DocumentProperties( NULL, hPrinter, lpszDeviceName, pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER );

			// Sync the pDevMode
			::ClosePrinter( hPrinter     );
			::GlobalUnlock( pd.hDevNames );
			::GlobalUnlock( pd.hDevMode  );
		}
	}

ECO_EXCEPTION_END
	return 0;
}

INT CWorkSheet::SetPaperKind( INT Paper, CWinApp * pWinApp )
{
ECO_EXCEPTION_BEGIN
	PRINTDLG pd;
	wmemset(&pd, 0, sizeof(pd));

	if ( pWinApp->GetPrinterDeviceDefaults( &pd ) )	{
		//Lock memory handle
		DEVMODE FAR* pDevMode = ( DEVMODE FAR* )::GlobalLock( pd.hDevMode );
		LPDEVNAMES lpDevNames;
		LPTSTR lpszDriverName, lpszDeviceName, lpszPortName;
		HANDLE hPrinter;
		if (pDevMode) {
			pDevMode->dmPaperSize = Paper;
			
			// Unlock memory handle
			lpDevNames     = ( LPDEVNAMES )::GlobalLock( pd.hDevNames );
			lpszDriverName = ( LPTSTR )lpDevNames + lpDevNames->wDriverOffset;
			lpszDeviceName = ( LPTSTR )lpDevNames + lpDevNames->wDeviceOffset;
			lpszPortName   = ( LPTSTR )lpDevNames + lpDevNames->wOutputOffset;

			::OpenPrinter( lpszDeviceName, &hPrinter, NULL );

			::DocumentProperties( NULL, hPrinter, lpszDeviceName, pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER );

			// Sync the pDevMode
			::ClosePrinter( hPrinter     );
			::GlobalUnlock( pd.hDevNames );
			::GlobalUnlock( pd.hDevMode  );
		}
	}

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// E D I T
//====================================================================================
INT CWorkSheet::Cut( CDC * pDC, CSize & Size )
{
ECO_EXCEPTION_BEGIN
	//
	AdjustScrollDC( pDC, &Size );

	// Copy
	Copy( pDC, Size );

	// Delete Selected Object
	Delete( pDC, Size );

	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

	// Copy Position ( origin )
	m_CopyPoint.x = -10;
	m_CopyPoint.y = -10;

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// CopyObjectInfo
//
// 목  적: 객체를 복사한다.
// 작성자: 우상옥
//========================================================================
INT CWorkSheet::CopyObjectInfo( REPORT_SHEET * pDesSheet ,  // [IN/OUT] 객체가 복사될 Sheet
							    REPORT_SHEET * pSrcSheet ,  // [IN]     원본 객체가 있는 Sheet
								INT            SrcObjIdx )  // [IN]     원본 객체의 인데스
{
ECO_EXCEPTION_BEGIN
	INT             i, j, DesObjIdx, ParentIdx, BlockRow, BlockCol;
	//_TCHAR            FullFileName[256];
	REPORT_OBJECT * pDesObject;
	REPORT_OBJECT * pSrcObject;

	// 1. 복사될 객체를 위해서 메모리를 할당한다.
	pDesSheet->object = ( REPORT_OBJECT ** )realloc( pDesSheet->object, sizeof( REPORT_OBJECT * ) * ( pDesSheet->object_count + 1 ) );
	pDesSheet->object[pDesSheet->object_count] = ( REPORT_OBJECT * )malloc( sizeof( REPORT_OBJECT ) );

	// 2. 객체의 기본정보를 복사한다.
	wmemcpy( pDesSheet->object[pDesSheet->object_count], pSrcSheet->object[SrcObjIdx], sizeof( REPORT_OBJECT ) );
	_tcscpy( pDesSheet->object[pDesSheet->object_count]->comm.name, NamingObject( pSrcSheet->object[SrcObjIdx]->kind ) );
	pDesSheet->object_count++;
	pDesSheet->object_id++;

	// 3. 객체에 할당되어 있는 메모리를 복사한다.
	DesObjIdx  = pDesSheet->object_count-1;
	pDesObject = pDesSheet->object[DesObjIdx];
	pSrcObject = pSrcSheet->object[SrcObjIdx];

	switch ( pSrcObject->kind ) {
		case OBJECT_TABLE :
			pDesObject->table.char_len   = ( INT  *  )malloc( sizeof( INT    ) * pSrcObject->table.col_count );
			pDesObject->table.char_max   = ( INT  *  )malloc( sizeof( INT    ) * pSrcObject->table.col_count );
			pDesObject->table.indent_len = ( INT  *  )malloc( sizeof( INT    ) * pSrcObject->table.col_count );
			pDesObject->table.horz_align = ( INT  *  )malloc( sizeof( INT    ) * pSrcObject->table.col_count );
			pDesObject->table.vert_align = ( INT  *  )malloc( sizeof( INT    ) * pSrcObject->table.col_count );
			pDesObject->table.data_field = ( _TCHAR ** )malloc( sizeof( _TCHAR * ) * pSrcObject->table.col_count );

			wmemcpy( pDesObject->table.char_len,   pSrcObject->table.char_len,   sizeof( INT ) * pSrcObject->table.col_count );
			wmemcpy( pDesObject->table.char_max,   pSrcObject->table.char_max,   sizeof( INT ) * pSrcObject->table.col_count );
			wmemcpy( pDesObject->table.indent_len, pSrcObject->table.indent_len, sizeof( INT ) * pSrcObject->table.col_count );
			wmemcpy( pDesObject->table.horz_align, pSrcObject->table.horz_align, sizeof( INT ) * pSrcObject->table.col_count );
			wmemcpy( pDesObject->table.vert_align, pSrcObject->table.vert_align, sizeof( INT ) * pSrcObject->table.col_count );

			for ( i = 0; i < pSrcObject->table.col_count; i++ ) {
				if ( NULL != pSrcObject->table.data_field[i] ) {
					pDesObject->table.data_field[i] = ( _TCHAR * )malloc( sizeof( _TCHAR ) * StringLength( pSrcObject->table.data_field[i] ) + sizeof( _TCHAR ) ); // Multi Language
					_tcscpy( pDesObject->table.data_field[i], pSrcObject->table.data_field[i] );
				} else {
					pDesObject->table.data_field[i] = NULL;
				}
			}
			// Cell
			pDesObject->table.cell = ( REPORT_CELL ** )malloc( sizeof( REPORT_CELL * ) * pSrcObject->table.row_count );
			for ( i = 0; i < pSrcObject->table.row_count; i++ ) {
				pDesObject->table.cell[i] = ( REPORT_CELL * )malloc( sizeof( REPORT_CELL ) * pSrcObject->table.col_count );
				for ( j = 0; j < pSrcObject->table.col_count; j++ ) {
					wmemcpy( &pDesObject->table.cell[i][j], &pSrcObject->table.cell[i][j], sizeof( REPORT_CELL ) );
					if ( NULL != pSrcObject->table.cell[i][j].text ) {
						pDesObject->table.cell[i][j].text = ( _TCHAR * )malloc( sizeof( _TCHAR ) * StringLength( pSrcObject->table.cell[i][j].text ) + sizeof( _TCHAR ) ); // Multi Language
						_tcscpy( pDesObject->table.cell[i][j].text, pSrcObject->table.cell[i][j].text );
					} else {
						pDesObject->table.cell[i][j].text = NULL;
					}
				}
			}
			break;
		case OBJECT_GRID   :
			pDesObject->grid.visible = pSrcObject->grid.visible;
			// Block
			pDesObject->grid.block = ( REPORT_BLOCK ** )malloc( sizeof( REPORT_BLOCK * ) * pSrcObject->grid.row_count );
			for ( i = 0; i < pSrcObject->grid.row_count; i++ ) {
				pDesObject->grid.block[i] = ( REPORT_BLOCK * )malloc( sizeof( REPORT_BLOCK ) * pSrcObject->grid.col_count );
				for ( j = 0; j < pSrcObject->grid.col_count; j++ ) {
					wmemcpy( &pDesObject->grid.block[i][j], &pSrcObject->grid.block[i][j], sizeof( REPORT_BLOCK ) );
					if ( NULL != pSrcObject->grid.block[i][j].text ) {
						pDesObject->grid.block[i][j].text = ( _TCHAR * )malloc( sizeof( _TCHAR ) * StringLength( pSrcObject->grid.block[i][j].text ) + sizeof( _TCHAR ) ); // Multi Language
						_tcscpy( pDesObject->grid.block[i][j].text, pSrcObject->grid.block[i][j].text );
					} else {
						pDesObject->grid.block[i][j].text = NULL;
					}
				}
			}
			break;
		case OBJECT_BITMAP :
			/*
			MakeReferencePath( pDesObject->bitmap.file_name, FullFileName );
			pDesObject->bitmap.dib = new CMyDib( FullFileName );
			*/
			pDesObject->bitmap.dib = new CPicture;
			pDesObject->bitmap.dib->Load( pDesObject->bitmap.file_name );
			break;
		case OBJECT_BARCODE :
			pDesObject->barcode.bc = new CBarcodCtrl;
			pDesObject->barcode.bc->SetCaption( CString( pDesObject->barcode.value ) );
			break;
		case OBJECT_HLINE :
		case OBJECT_VLINE :
		case OBJECT_BOX   :
			break;
	}

	//자식객체 인덱스 정보
	if ( TRUE == GetParentInfo( m_pSheet, SrcObjIdx, &ParentIdx, &BlockRow, &BlockCol ) ) {
		AddChildInfo ( ParentIdx, DesObjIdx, BlockRow, BlockCol );
		SetParentInfo( DesObjIdx, ParentIdx, BlockRow, BlockCol );
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// Paste
//
// 목  적: 객체를 복사한다.
// 작성자: 우상옥
//========================================================================
INT CWorkSheet::Copy( CDC   * pDC  ,  // [IN] DC
					  CSize & Size )  // [IN] 스크롤 사이즈
{
ECO_EXCEPTION_BEGIN
	INT i, j, k;

	// 1. 이전에 복사된 메모리를 Free한다.
	if ( NULL == m_ClipSheet.object ) {
		FreeObject( &m_ClipSheet );
		free( m_ClipSheet.object );
	}

	// 2. ClipSheet을 초기화한다.
	m_ClipSheet.object_count = 0;
	m_ClipSheet.object       = NULL;

	// 3. 복사시 원본과 비껴 붙이는 좌표를 초기화한다.
	m_CopyPoint.x = 0;
	m_CopyPoint.y = 0;

	// 4. 선택된 객체를 복사한다.
	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		// 4.1 선택되지 않은 객체를 생략한다.
		if ( -1 == m_pSheet->object[i]->select ) {
			continue;
		}
		// 4.2 자식객체가 있는 객체는 복사되지 않는다.
		if ( OBJECT_GRID == m_pSheet->object[i]->kind ) {
			for ( j = 0; j < m_pSheet->object[i]->grid.row_count; j++ ) {
				for ( k = 0; k < m_pSheet->object[i]->grid.col_count; k++ ) {
					if ( 0 < m_pSheet->object[i]->grid.block[j][k].child_count ) {
						AfxMessageBox( _T("자식객체가 있는 객체는 복사되지 않습니다.") );
						m_pSheet->object[i]->select = -1;
						goto _SKIP_;
					}
				}
			}
		}
		// 4.3 객체를 복사한다.
		CopyObjectInfo( &m_ClipSheet, m_pSheet, i );
_SKIP_:;
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// Paste
//
// 목  적: 복사한 객체를 붙인다.
// 작성자: 우상옥
//========================================================================
INT CWorkSheet::Paste( CDC   * pDC  ,  // [IN] DC
					   CSize & Size )  // [IN] 스크롤 사이즈
{
ECO_EXCEPTION_BEGIN
	INT   i;
	CRect Rect;

	// 1. 스크롤 크기를 조정한다.
	AdjustScrollDC( pDC, &Size );

	// 2. 객체선택을 Clear한다.
	SelectObject( -1 );

	// 3. 원본에서 비켜 붙인다.
	m_CopyPoint.x += 10;
	m_CopyPoint.y += 10;

	// 4. 객체를 붙인다.
	for ( i = 0; i < m_ClipSheet.object_count; i++ ) {
		// 4.1 객체를 복사한다.
		CopyObjectInfo( m_pSheet, &m_ClipSheet, i );
		// 4.2 객체의 위치를 비껴서 위치시킨다.
		m_pSheet->object[m_pSheet->object_count-1]->comm.left += m_CopyPoint.x;
		m_pSheet->object[m_pSheet->object_count-1]->comm.top  += m_CopyPoint.y;
	}

	// 5. 다시 그린다.
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// TabStop
//
// 목  적: 객체의 선택을 이동시킨다.
// 작성자: 우상옥
//========================================================================
INT CWorkSheet::TabStop( CDC   * pDC  ,  // [IN] DC
						 CSize & Size )  // [IN] 스크롤 사이즈
{
ECO_EXCEPTION_BEGIN
	// 1. Tab Stop 인덱스를 초기한다.
	if ( m_TabStopObjectIdx > m_pSheet->object_count - 1 ) {
		m_TabStopObjectIdx = 0;
	}
	if ( 0 == m_pSheet->object_count ) {
		m_TabStopObjectIdx = -1;
		return 0;
	}

	// 2. 스크롤 크기를 조정한다.
	AdjustScrollDC( pDC, &Size );

	// 3. TabStop이 설정된 객체를 선택한다.
	SelectObject( m_TabStopObjectIdx );

	// 4. 포커스를 없앤다.
	KillFocus();

	// 5. 선택된 객체에 포커스를 설정한다.
	if ( -1 == m_TabStopObjectIdx ) {
		SetFocus( 0 );
	} else {
		SetFocus( m_TabStopObjectIdx );
	}

	// 6. Tab Stop 인덱스를 증가시킨다.
	m_TabStopObjectIdx++;

	// 7. 다시 그린다.
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// KeyMoveObject
//
// 목  적: 객체의 선택을 이동시킨다.
// 작성자: 우상옥
//========================================================================
INT CWorkSheet::KeyMoveObject( CDC   * pDC     ,  // [IN] DC
							   CSize & Size    ,  // [IN] 스크롤 크기
							   UINT    KeyValue,  // [IN] 키보드값
							   INT     Mode    )  // [IN] 속도 보통/빠르게
{
ECO_EXCEPTION_BEGIN
	INT   FastOffset = 0;
	CRect Rect;

	// 1. 빠르기 Mode일 경우 이동 Pixel을 조정한다.
	if ( KEY_MOVE_CONTROL == Mode ) {
		FastOffset = 10;
	}

	// 2. 선택된 객체의 영역을 구한다.
	GetSelectedObjectRect( Rect );
	m_ObjectRect.SetRect( Rect.left  - PICK_POINT_SIZE, Rect.top    - PICK_POINT_SIZE, 
						  Rect.right + PICK_POINT_SIZE, Rect.bottom + PICK_POINT_SIZE );

	// 3. 현재 Focus가 있는 객체를 보관용 DC에 보관한다.
	PutObjectToStore( m_CaretInfo.object_index, TRUE );

	// 4. 객체를 이동시킨다.
	switch ( KeyValue ) {
		case VK_LEFT   : MoveObject( pDC, CSize(  1 + FastOffset,  0              ) ); break;
		case VK_UP     : MoveObject( pDC, CSize(  0,               1 + FastOffset ) ); break;
		case VK_RIGHT  : MoveObject( pDC, CSize( -1 - FastOffset,  0              ) ); break;
		case VK_DOWN   : MoveObject( pDC, CSize(  0,              -1 - FastOffset ) ); break;
		case VK_F2     : 
			if ( -1 != m_CaretInfo.object_index ) {
				m_PropertyInfo.mode         = PROPERTY_MODE_DISPLAY;
				m_PropertyInfo.object_index = m_CaretInfo.object_index;
				m_PropertyInfo.object_kind  = m_pSheet->object[m_CaretInfo.object_index]->kind;
				_tcscpy( m_PropertyInfo.hline.name, m_pSheet->object[m_CaretInfo.object_index]->comm.name  );
				switch ( m_pSheet->object[m_CaretInfo.object_index]->kind ) {
					case OBJECT_HLINE   : m_PropertyInfo.hline.style = m_pSheet->object[m_CaretInfo.object_index]->hline.style;                    break;
					case OBJECT_VLINE   : m_PropertyInfo.vline.style = m_pSheet->object[m_CaretInfo.object_index]->vline.style;                    break;
					case OBJECT_BOX     : m_PropertyInfo.box.style = m_pSheet->object[m_CaretInfo.object_index]->box.style;                        break;
					case OBJECT_BARCODE : _tcscpy( m_PropertyInfo.barcode.value, m_pSheet->object[m_CaretInfo.object_index]->barcode.value );
										  m_PropertyInfo.barcode.code = m_pSheet->object[m_CaretInfo.object_index]->barcode.code;                  break;
					case OBJECT_BITMAP  : _tcscpy( m_PropertyInfo.bitmap.file_name, m_pSheet->object[m_CaretInfo.object_index]->bitmap.file_name ); break;
				}
				( m_pOwnderWnd->*m_fpPropertyDialog )( m_PropertyInfo );
			}
			break;
	}

	// 5. 이동된 객체의 영역을 구한다.
	GetSelectedObjectRect( Rect );

	// 6. 객체이동의 비트맵처리를 한다.
	PadBitmap( pDC, Rect );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// SetFocus
//
// 목  적: 객체에 포커스를 설정한다.
// 작성자: 우상옥
//========================================================================
INT CWorkSheet::SetFocus( INT ObjectIdx )
{
ECO_EXCEPTION_BEGIN
	INT RowCursor, ColCursor;

	// 1. 포커스정보를 설정한다.
	m_CaretInfo.focus        = TRUE;
	m_CaretInfo.object_index = ObjectIdx;

	// 2. 객체에 포커스를 설정한다.
	switch ( m_pSheet->object[m_CaretInfo.object_index]->kind ) {
		case OBJECT_TABLE :
			// 테이블의 0, 0 위치에 포커스 설정
			RowCursor = m_pSheet->object[ObjectIdx]->table.cursor_row = 0;
			ColCursor = m_pSheet->object[ObjectIdx]->table.cursor_col = 0;
			// 포커스 플래그 설정
			m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][ColCursor].focus = TRUE;
			break;
		case OBJECT_GRID :
			// 테이블의 0, 0 위치에 포커스 설정
			RowCursor = m_pSheet->object[ObjectIdx]->grid.cursor_row = 0;
			ColCursor = m_pSheet->object[ObjectIdx]->grid.cursor_col = 0;
			// 포커스 플래그 설정
			m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].focus = TRUE;
			break;
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// KillFocus
//
// 목  적: 객체에 포커스를 해제한다.
// 작성자: 우상옥
//========================================================================
INT CWorkSheet::KillFocus( VOID )
{
ECO_EXCEPTION_BEGIN
	INT i, j, k;

	// 1. 객체의 포커스를 해제한다.
	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		switch ( m_pSheet->object[i]->kind ) {
			case OBJECT_TABLE :
				for ( j = 0; j < m_pSheet->object[i]->table.row_count; j++ ) {
					for ( k = 0; k < m_pSheet->object[i]->table.col_count; k++ ) {
						m_pSheet->object[i]->table.cell[j][k].focus = FALSE;
					}
				}
				m_pSheet->object[i]->table.bind = FALSE;
				break;
			case OBJECT_GRID :
				for ( j = 0; j < m_pSheet->object[i]->grid.row_count; j++ ) {
					for ( k = 0; k < m_pSheet->object[i]->grid.col_count; k++ ) {
						m_pSheet->object[i]->grid.block[j][k].focus = FALSE;
					}
				}
				m_pSheet->object[i]->grid.bind = FALSE;
				break;
		}
	}

	// 2. 포커스 정보를 초기화한다.
	m_CaretInfo.focus        = FALSE;
	m_CaretInfo.object_index = -1; 

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// KeyMoveFocusCursorTable
//========================================================================
INT CWorkSheet::KeyMoveFocusCursorTable( CDC * pDC, UINT KeyValue )
{
ECO_EXCEPTION_BEGIN
	INT   i, j, RowCursor, ColCursor, TempRowCursor, TempColCursor, OffsetRow, OffsetCol;
	CRect Rect, BindRect;

	//
	RowCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_row;
	ColCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_col;

	// Kill Focus
	if ( FALSE == m_pSheet->object[m_CaretInfo.object_index]->table.bind ) {
		m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][ColCursor].focus = FALSE;
	} else {
		for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; i++ ) {
			for ( j = 0 ; j <= m_pSheet->object[m_CaretInfo.object_index]->table.col_count; j++ ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].focus = FALSE;
			}
		}
	}

	// Move Focus
	switch ( KeyValue ) {
		case VK_LEFT : 
			if ( ColCursor > 0 ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.cursor_col--;
			}
			break;
		case VK_RIGHT : 
			TempColCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_col;
			if ( TempColCursor < m_pSheet->object[m_CaretInfo.object_index]->table.col_count - 1 ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.cursor_col++;
			}
			break;
		case VK_UP :
			if ( RowCursor > 0 ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.cursor_row--;
			}
			break;
		case VK_DOWN : 
			TempRowCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_row;
			if ( TempRowCursor < m_pSheet->object[m_CaretInfo.object_index]->table.row_count - 1 ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.cursor_row++;
			}
			break;
		case VK_ESCAPE :
			if ( TRUE == m_pSheet->object[m_CaretInfo.object_index]->table.bind ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.bind = FALSE;
				m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][ColCursor].focus = TRUE;
			} else {
				KillFocus();
			}
			goto _EXIT_;
			break;
		case VK_F5 :
			m_pSheet->object[m_CaretInfo.object_index]->table.bind       = TRUE;
			m_pSheet->object[m_CaretInfo.object_index]->table.anchor_row = RowCursor;
			m_pSheet->object[m_CaretInfo.object_index]->table.anchor_col = ColCursor;
			m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][ColCursor].focus = TRUE;
			goto _EXIT_;
		case VK_F3 :
			if ( -1 != m_CaretInfo.object_index ) {
				if ( TRUE == m_pSheet->object[m_CaretInfo.object_index]->table.bind ) {
					::MessageBox( NULL, _T("멀티선택을 할 수 없습니다."), _T("알림"), MB_OK );
					m_pSheet->object[m_CaretInfo.object_index]->table.bind = FALSE;
					goto _EXIT_;
				}
				m_PropertyInfo.mode         = PROPERTY_MODE_SHAPE;
				m_PropertyInfo.object_index = m_CaretInfo.object_index;
				m_PropertyInfo.object_kind  = m_pSheet->object[m_CaretInfo.object_index]->kind;
				( m_pOwnderWnd->*m_fpPropertyDialog )( m_PropertyInfo );
			}
			break;
		case VK_F2 :
			if ( -1 != m_CaretInfo.object_index ) {
				m_PropertyInfo.mode         = PROPERTY_MODE_DISPLAY;
				m_PropertyInfo.object_index = m_CaretInfo.object_index;
				m_PropertyInfo.object_kind  = m_pSheet->object[m_CaretInfo.object_index]->kind;
				m_PropertyInfo.multi        = m_pSheet->object[m_CaretInfo.object_index]->table.bind;
				//
				_tcscpy( m_PropertyInfo.table.name,       m_pSheet->object[m_CaretInfo.object_index]->table.name      );
				_tcscpy( m_PropertyInfo.table.data_name,  m_pSheet->object[m_CaretInfo.object_index]->table.data_name );
				m_PropertyInfo.table.horz_line   = m_pSheet->object[m_CaretInfo.object_index]->table.horz_line;
				m_PropertyInfo.table.vert_line   = m_pSheet->object[m_CaretInfo.object_index]->table.vert_line;
				m_PropertyInfo.table.visible     = m_pSheet->object[m_CaretInfo.object_index]->table.visible;
				m_PropertyInfo.table.area        = m_pSheet->object[m_CaretInfo.object_index]->table.area;
				m_PropertyInfo.table.line_expand = m_pSheet->object[m_CaretInfo.object_index]->table.line_expand;
				m_PropertyInfo.table.line_max    = m_pSheet->object[m_CaretInfo.object_index]->table.line_max;
				if ( FALSE == m_pSheet->object[m_CaretInfo.object_index]->table.bind ) {
					if ( NULL != m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][ColCursor].text ) {
						_tcscpy( m_PropertyInfo.table.text, m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][ColCursor].text );
					} else {
						m_PropertyInfo.table.text[0] = '\0';
					}
					if ( NULL != m_pSheet->object[m_CaretInfo.object_index]->table.data_field[ColCursor] ) {
						_tcscpy( m_PropertyInfo.table.data_field, m_pSheet->object[m_CaretInfo.object_index]->table.data_field[ColCursor] );
					} else {
						m_PropertyInfo.table.data_field[0] = '\0';
					}
				} else {
					m_PropertyInfo.table.text      [0] = '\0';
					m_PropertyInfo.table.data_field[0] = '\0';
				}
				m_PropertyInfo.table.char_len    = m_pSheet->object[m_CaretInfo.object_index]->table.char_len  [ColCursor];
				m_PropertyInfo.table.char_max    = m_pSheet->object[m_CaretInfo.object_index]->table.char_max  [ColCursor];
				m_PropertyInfo.table.indent_len  = m_pSheet->object[m_CaretInfo.object_index]->table.indent_len[ColCursor];
				m_PropertyInfo.table.horz_align  = m_pSheet->object[m_CaretInfo.object_index]->table.horz_align[ColCursor];
				m_PropertyInfo.table.vert_align  = m_pSheet->object[m_CaretInfo.object_index]->table.vert_align[ColCursor];
				wmemcpy( &m_PropertyInfo.table.font, &m_pSheet->object[m_CaretInfo.object_index]->table.font, sizeof( LOGFONT ) );
				m_PropertyInfo.table.fill        = m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][ColCursor].fill;
				//
				( m_pOwnderWnd->*m_fpPropertyDialog )( m_PropertyInfo );
				m_pSheet->object[m_PropertyInfo.object_index]->table.bind                             = FALSE;
				m_pSheet->object[m_PropertyInfo.object_index]->table.cell[RowCursor][ColCursor].focus = TRUE;
				goto _EXIT_;
			}
			break;
	}

	//
	RowCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_row;
	ColCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_col;

	// Set Focus
	if ( FALSE == m_pSheet->object[m_CaretInfo.object_index]->table.bind ) {
		m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][ColCursor].focus = TRUE;
	} else {
		// MAKE BINDING FOCUS RECTANGLE
		OffsetRow = RowCursor - m_pSheet->object[m_CaretInfo.object_index]->table.anchor_row;
		OffsetCol = ColCursor - m_pSheet->object[m_CaretInfo.object_index]->table.anchor_col;

		Rect.SetRect( m_pSheet->object[m_CaretInfo.object_index]->table.anchor_col              ,
			          m_pSheet->object[m_CaretInfo.object_index]->table.anchor_row              ,
					  m_pSheet->object[m_CaretInfo.object_index]->table.anchor_col + OffsetCol  ,
					  m_pSheet->object[m_CaretInfo.object_index]->table.anchor_row + OffsetRow );

		Rect.NormalizeRect();

		// SET BINDING FOCUS
		for ( i = Rect.top; i <= Rect.bottom; i++ ) {
			for ( j = Rect.left; j <= Rect.right; j++ ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].focus = TRUE;
			}
		}
	}

_EXIT_:
	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// KeyMoveFocusResizeTable
//========================================================================
INT CWorkSheet::KeyMoveFocusResizeTable( CDC * pDC, UINT KeyValue )
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCursor, ColCursor;

	RowCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_row;
	ColCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_col;

	switch ( KeyValue ) {
		case VK_LEFT : 
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.col_count; i++ ) {
				if ( i == ColCursor ) {
					m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][i].width--;
				}
				if ( i == ColCursor + 1 ) {
					m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][i].left--;
					m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][i].width++;
				}
			}
			if ( ColCursor == m_pSheet->object[m_CaretInfo.object_index]->table.col_count - 1 ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.width--;
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; j++ ) {
					if ( j == RowCursor ) {
						continue;
					}
					m_pSheet->object[m_CaretInfo.object_index]->table.cell[j][ColCursor].width--;
				}
			}
			break;
		case VK_RIGHT : 
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.col_count; i++ ) {
				if ( i == ColCursor ) {
					m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][i].width++;
				}
				if ( i == ColCursor + 1 ) {
					m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][i].left++;
					m_pSheet->object[m_CaretInfo.object_index]->table.cell[RowCursor][i].width--;
				}
			}
			if ( ColCursor == m_pSheet->object[m_CaretInfo.object_index]->table.col_count - 1 ) {
				m_pSheet->object[m_CaretInfo.object_index]->table.width++;
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; j++ ) {
					if ( j == RowCursor ) {
						continue;
					}
					m_pSheet->object[m_CaretInfo.object_index]->table.cell[j][ColCursor].width++;
				}
			}
			break;
	}

	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0 ;
}

//========================================================================
// KeyMoveFocusExpandTable
//========================================================================
INT CWorkSheet::KeyMoveFocusExpandTable( CDC * pDC, UINT KeyValue, BOOL Fast )
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCursor, ColCursor, MoveUnit;

	RowCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_row;
	ColCursor = m_pSheet->object[m_CaretInfo.object_index]->table.cursor_col;

	switch ( Fast ) {
		case TRUE  : MoveUnit = 5; break;
		case FALSE : MoveUnit = 1; break;
	}

	//
	switch ( KeyValue ) {
		case VK_LEFT : 
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->table.col_count; j++ ) {
					if ( j == ColCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].width -= MoveUnit;
					}
					if ( j > ColCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].left -= MoveUnit;
					}
				}
			}
			m_pSheet->object[m_CaretInfo.object_index]->table.width -= MoveUnit;
			break;
		case VK_RIGHT : 
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->table.col_count; j++ ) {
					if ( j == ColCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].width += MoveUnit;
					}
					if ( j > ColCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].left += MoveUnit;
					}
				}
			}
			m_pSheet->object[m_CaretInfo.object_index]->table.width += MoveUnit;
			break;
/* SAME SIZE
		case VK_UP :
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->table.col_count; j++ ) {
					if ( i == RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].height -= MoveUnit;
					}
					if ( i > RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].top    -= MoveUnit * i;
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].height -= MoveUnit; //
					}
				}
			}
			m_pSheet->object[m_CaretInfo.object_index]->table.height -= ( i * MoveUnit );
			break;
		case VK_DOWN : 
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->table.col_count; j++ ) {
					if ( i == RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].height += MoveUnit;
					}
					if ( i > RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].top    += MoveUnit * i;
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].height += MoveUnit; //
					}
				}
			}
			m_pSheet->object[m_CaretInfo.object_index]->table.height += ( i * MoveUnit );
			break;
*/
		case VK_UP :
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->table.col_count; j++ ) {
					if ( i == RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].height -= MoveUnit;
					}
					if ( i > RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].top -= MoveUnit;
					}
				}
			}
			m_pSheet->object[m_CaretInfo.object_index]->table.height -= MoveUnit;
			break;
		case VK_DOWN : 
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->table.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->table.col_count; j++ ) {
					if ( i == RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].height += MoveUnit;
					}
					if ( i > RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->table.cell[i][j].top += MoveUnit;
					}
				}
			}
			m_pSheet->object[m_CaretInfo.object_index]->table.height += MoveUnit;
			break;
	}


	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// ExistedBinding
//========================================================================
BOOL CWorkSheet::ExistedBinding( INT ObjectIdx )
{
	INT i, j;

	for ( i = 0; i < m_pSheet->object[ObjectIdx]->grid.row_count; i++ ) {
		for ( j = 0; j < m_pSheet->object[ObjectIdx]->grid.col_count; j++ ) {
			if ( BIND_STATUS_NONE != m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_status ) {
				//if ( i == m_pSheet->object[ObjectIdx]->grid.page_row || i == m_pSheet->object[ObjectIdx]->grid.last_row ) {
				//	continue;
				//}
				return TRUE;
			}
		}
	}

	return FALSE;
}

//========================================================================
// KeyMoveFocusCursorGrid
//========================================================================
INT CWorkSheet::KeyMoveFocusCursorGrid( CDC * pDC, UINT KeyValue )
{
ECO_EXCEPTION_BEGIN
	INT   i, j, RowCursor, ColCursor, OffsetCol, OffsetRow, TempWidht, TempHeight, TempRowCursor, TempColCursor, ExtraRow = 0;
	CRect Rect, BindRect;

	//
	RowCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row;
	ColCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col;

	// Kill Focus
	if ( FALSE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind ) {
		m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].focus = FALSE;
	} else {
		for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
			for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].focus = FALSE;
			}
		}
	}

	// Move Focus
	switch ( KeyValue ) {
		case VK_LEFT : 
			if ( ColCursor > 0 ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col--;
			}
			// BIND ADJUST ++
			TempColCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col;
			if ( BIND_STATUS_SLAVE == m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][TempColCursor].bind_status ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][TempColCursor].bind_col;
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][TempColCursor].bind_row;
			}
			// BIND ADJUST --
			break;
		case VK_RIGHT : 
			// BIND ADJUST ++
			TempColCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col;
			if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][TempColCursor].bind_status ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][TempColCursor].bind_col;
			}
			// BIND ADJUST --
			TempColCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col;
			if ( TempColCursor < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count - 1 ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col++;
			}
			// BIND ADJUST ++
			TempColCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col;
			if ( BIND_STATUS_SLAVE == m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][TempColCursor].bind_status ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][TempColCursor].bind_col;
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][TempColCursor].bind_row;
			}
			// BIND ADJUST --
			break;
		case VK_UP :
			if ( TRUE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind && ( RowCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.last_row || RowCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.page_row ) ) {
				break;
			}
			if ( RowCursor > 0 ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row--;
			}
			// BIND ADJUST ++
			TempRowCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row;
			if ( BIND_STATUS_SLAVE == m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].bind_status ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].bind_col;
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].bind_row;
			}
			// BIND ADJUST --
			break;
		case VK_DOWN : 
			if ( TRUE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind && ( RowCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.last_row || RowCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.page_row ) ) {
				break;
			}
			// BIND ADJUST ++
			TempRowCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row;
			if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].bind_status ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].bind_row;
			}
			// BIND ADJUST --
			TempRowCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row;
			if ( TempRowCursor < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count - 1 ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row++;
			}
			// BIND ADJUST ++
			TempRowCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row;
			if ( BIND_STATUS_SLAVE == m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].bind_status ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].bind_col;
				m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].bind_row;
			}
			// BIND ADJUST --
			//++ CHILD OBJECT : 추후처리
			//if ( -1 != m_pSheet->object[m_CaretInfo.object_index]->grid.block[TempRowCursor][ColCursor].child_index ) {
			//	INT CaretObjectIdx = m_CaretInfo.object_index;
			//	KillFocus();
			//	SetFocus( m_pSheet->object[CaretObjectIdx].grid.block[TempRowCursor][ColCursor].child_index );
			//	goto _EXIT_;
			//}
			//-- CHILD OBJECT
			break;
		case VK_ESCAPE :
			if ( TRUE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.bind = FALSE;
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].focus = TRUE;
			} else {
				KillFocus();
			}
			goto _EXIT_;
			break;
		case VK_F5 :
			m_pSheet->object[m_CaretInfo.object_index]->grid.bind       = TRUE;
			m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_row = RowCursor;
			m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_col = ColCursor;
			m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].focus = TRUE;
			goto _EXIT_;
		case VK_F3 :
			if ( -1 != m_CaretInfo.object_index ) {
				if ( TRUE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind ) {
					::MessageBox( NULL, _T("멀티선택을 할 수 없습니다."), _T("알림"), MB_OK );
					m_pSheet->object[m_CaretInfo.object_index]->grid.bind = FALSE;
					goto _EXIT_;
				}
				if ( TRUE == ExistedBinding( m_CaretInfo.object_index ) ) {
					::MessageBox( NULL, _T("먼저 그리드의 묶음을 푼후에 사용하십시요."), _T("알림"), MB_OK );
					goto _EXIT_;
				}
				m_PropertyInfo.mode         = PROPERTY_MODE_SHAPE;
				m_PropertyInfo.object_index = m_CaretInfo.object_index;
				m_PropertyInfo.object_kind  = m_pSheet->object[m_CaretInfo.object_index]->kind;
				( m_pOwnderWnd->*m_fpPropertyDialog )( m_PropertyInfo );
			}
			break;
		case VK_F2 :
			if ( -1 != m_CaretInfo.object_index ) {
				m_PropertyInfo.mode         = PROPERTY_MODE_DISPLAY;
				m_PropertyInfo.object_index = m_CaretInfo.object_index;
				m_PropertyInfo.object_kind  = m_pSheet->object[m_CaretInfo.object_index]->kind;
				m_PropertyInfo.multi        = m_pSheet->object[m_CaretInfo.object_index]->grid.bind;
				//
				_tcscpy( m_PropertyInfo.grid.name, m_pSheet->object[m_CaretInfo.object_index]->grid.name );
				m_PropertyInfo.grid.page_row    = m_pSheet->object[m_CaretInfo.object_index]->grid.page_row;
				m_PropertyInfo.grid.last_row    = m_pSheet->object[m_CaretInfo.object_index]->grid.last_row;
				m_PropertyInfo.grid.horz_line   = m_pSheet->object[m_CaretInfo.object_index]->grid.horz_line;
				m_PropertyInfo.grid.vert_line   = m_pSheet->object[m_CaretInfo.object_index]->grid.vert_line;
				if ( FALSE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind ) {
					if ( NULL != m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].text ) {
						_tcscpy( m_PropertyInfo.grid.text, m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].text );
					} else {
						m_PropertyInfo.grid.text[0] = '\0';
					}
				} else {
					m_PropertyInfo.grid.text[0] = '\0';
				}
				m_PropertyInfo.grid.horz_align  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].horz_align;
				m_PropertyInfo.grid.fill        = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].fill;
				m_PropertyInfo.grid.orientation = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].orientation;
				m_PropertyInfo.grid.slash       = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].slash;
				wmemcpy( &m_PropertyInfo.grid.font, &m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].font, sizeof( LOGFONT ) );
				m_PropertyInfo.grid.visible     = m_pSheet->object[m_CaretInfo.object_index]->grid.visible;
				//
				( m_pOwnderWnd->*m_fpPropertyDialog )( m_PropertyInfo );
				m_pSheet->object[m_PropertyInfo.object_index]->grid.bind                              = FALSE;
				m_pSheet->object[m_PropertyInfo.object_index]->grid.block[RowCursor][ColCursor].focus = TRUE;
				goto _EXIT_;
			}
			break;
		case VK_F8 :
			if ( TRUE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind ) {
				goto _EXIT_;
			}
			// Page/Last Row
			/* 2000.1.16
			if ( RowCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.page_row && 0 != m_pSheet->object[m_CaretInfo.object_index]->grid.page_row ) {
				return 0;
			}
			if ( RowCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.last_row && 0 != m_pSheet->object[m_CaretInfo.object_index]->grid.last_row ) {
				return 0;
			}
			*/
			//////
			if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_status ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].width       = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].width;
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].height      = m_pSheet->object[m_CaretInfo.object_index]->grid.block[0][ColCursor].width;
				for ( i = RowCursor; i <= m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_row; i++ ) {
					for ( j = ColCursor; j <= m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_col; j++ ) {
						if ( BIND_STATUS_SLAVE == m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status = BIND_STATUS_NONE;
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row = -1;
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col = -1;
						}
					}
				}
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_status = BIND_STATUS_NONE;
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_row    = -1;
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_col    = -1;

				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].width  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].origin_width;
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].height = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].origin_height;
			}
			break;
		case VK_F7 :
			if ( TRUE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind ) {
				// MAKE BIND RECTANGLE
				OffsetRow = RowCursor - m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_row;
				OffsetCol = ColCursor - m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_col;

				Rect.SetRect( m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_col              ,
							  m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_row              ,
							  m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_col + OffsetCol  ,
							  m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_row + OffsetRow );
				Rect.NormalizeRect();

				if ( RowCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.page_row || RowCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.last_row ) {
                    if ( 0 != Rect.left || Rect.right != m_pSheet->object[m_CaretInfo.object_index]->grid.col_count-1 ) {
						::MessageBox( NULL, _T("페이지행과 마지막행은 전체를 묶어야 합니다."), _T("알림"), MB_OK );
						return 0;
					}
				}

				// BINDING VALIDATE
				for ( i = Rect.top; i <= Rect.bottom; i++ ) {
					for ( j = Rect.left; j <= Rect.right; j++ ) {
						if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
							BindRect.SetRect( j,
								              i,
											  m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col,
											  m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row );
							if ( Rect.left > BindRect.left || Rect.top > BindRect.top || Rect.right < BindRect.right || Rect.bottom < BindRect.bottom ) {
								::MessageBox( NULL, _T("선택된 셀 블록이 사각형이어야 하며 묶음을 포함해서는 안됩니다."), _T("알림"), MB_OK );
								return 0;
							}
						}
						if ( BIND_STATUS_SLAVE == m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
							BindRect.SetRect( m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col,
											  m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row,
											  j, i );
							if ( Rect.left > BindRect.left || Rect.top > BindRect.top || Rect.right < BindRect.right || Rect.bottom < BindRect.bottom ) {
								::MessageBox( NULL, _T("선택된 셀 블록이 사각형이어야 하며 묶음을 포함해서는 안됩니다."), _T("알림"), MB_OK );
								return 0;
							}
						}
					}
				}

				//++
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.top][Rect.left].origin_width  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.top][Rect.left].width;
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.top][Rect.left].origin_height = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.top][Rect.left].height;
				//--

				// BINDING
				for ( i = Rect.top; i <= Rect.bottom; i++ ) {
					for ( j = Rect.left; j <= Rect.right; j++ ) {
						if ( i == Rect.top && j == Rect.left ) {
							TempWidht  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.bottom][Rect.right].left - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].left + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.bottom][Rect.right].width;
							TempHeight = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.bottom][Rect.right].top  - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].top  + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.bottom][Rect.right].height;
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].width       = TempWidht;
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].height      = TempHeight; 
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status = BIND_STATUS_MASTER;
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row    = Rect.bottom;
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col    = Rect.right;
						} else {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status = BIND_STATUS_SLAVE;
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row    = Rect.top;
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col    = Rect.left;
						}
					}
				}
				// INIT BIND MODE
				m_pSheet->object[m_CaretInfo.object_index]->grid.bind = FALSE;
				//if ( BIND_STATUS_SLAVE == m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_status ) {
				//m_pSheet->object[m_CaretInfo.object_index]->grid.block[Rect.top][Rect.left].focus = TRUE;
				//} else {
				m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].focus = TRUE;
				//}
				goto _EXIT_;
			}
			break;
	}

	//
	RowCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row;
	ColCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col;

	// Set Focus
	if ( FALSE == m_pSheet->object[m_CaretInfo.object_index]->grid.bind ) {
		m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].focus = TRUE;
	} else {
		// MAKE BINDING FOCUS RECTANGLE
		OffsetRow = RowCursor - m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_row;
		OffsetCol = ColCursor - m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_col;

		Rect.SetRect( m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_col              ,
			          m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_row              ,
					  m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_col + OffsetCol  ,
					  m_pSheet->object[m_CaretInfo.object_index]->grid.anchor_row + OffsetRow );

		Rect.NormalizeRect();

		// ADJUST BIND
		//++
		if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_status ) {
			if ( Rect.right < m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_col ) {
				Rect.right  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_col;
			}
			if ( Rect.bottom < m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_row ) {
				Rect.bottom = m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][ColCursor].bind_row;
			}
		}
		//--
		// SET BINDING FOCUS
		for ( i = Rect.top; i <= Rect.bottom; i++ ) {
			for ( j = Rect.left; j <= Rect.right; j++ ) {
				//++
				if ( BIND_STATUS_SLAVE == m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
					INT Col, Row;
					Col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col;
					Row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row;
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].focus = TRUE;
				} else {
				//--
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].focus = TRUE;
				}
			}
		}
	}

_EXIT_:
	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// KeyMoveFocusResizeGrid
//========================================================================
INT CWorkSheet::KeyMoveFocusResizeGrid( CDC * pDC, UINT KeyValue )
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCursor, ColCursor;

	RowCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row;
	ColCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col;

	switch ( KeyValue ) {
		case VK_LEFT : 
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; i++ ) {
				if ( i == ColCursor ) {
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][i].width--;
				}
				if ( i == ColCursor + 1 ) {
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][i].left--;
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][i].width++;
				}
			}
			if ( ColCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.col_count - 1 ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.width--;
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; j++ ) {
					if ( j == RowCursor ) {
						continue;
					}
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[j][ColCursor].width--;
				}
			}
			break;
		case VK_RIGHT : 
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; i++ ) {
				if ( i == ColCursor ) {
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][i].width++;
				}
				if ( i == ColCursor + 1 ) {
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][i].left++;
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[RowCursor][i].width--;
				}
			}
			if ( ColCursor == m_pSheet->object[m_CaretInfo.object_index]->grid.col_count - 1 ) {
				m_pSheet->object[m_CaretInfo.object_index]->grid.width++;
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; j++ ) {
					if ( j == RowCursor ) {
						continue;
					}
					m_pSheet->object[m_CaretInfo.object_index]->grid.block[j][ColCursor].width++;
				}
			}
			break;
	}

	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// KeyMoveFocusExpandGrid
//========================================================================
INT CWorkSheet::KeyMoveFocusExpandGrid( CDC * pDC, UINT KeyValue, BOOL Fast )
{
ECO_EXCEPTION_BEGIN
	INT   i, j, RowCursor, ColCursor, MoveUnit;
	CSize Size;

	RowCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_row;
	ColCursor = m_pSheet->object[m_CaretInfo.object_index]->grid.cursor_col;

	switch ( Fast ) {
		case TRUE  : MoveUnit = 5; break;
		case FALSE : MoveUnit = 1; break;
	}

	//
	switch ( KeyValue ) {
		case VK_LEFT : 
			// Grid Cell
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
					if ( j == ColCursor ) {
						if ( BIND_STATUS_MASTER != m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].width -= MoveUnit;
						} else {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].origin_width -= MoveUnit;
						}
					}
					if ( j > ColCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].left -= MoveUnit;
					}
				}
			}
			// Bind Cell
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
					if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
						INT Row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row;
						INT Col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col;

						INT TempWidht  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].left - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].left + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].width;
						INT TempHeight = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].top  - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].top  + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].height;

						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].width  = TempWidht;
						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].height = TempHeight;
					}
				}
			}
			// Grid 
			m_pSheet->object[m_CaretInfo.object_index]->grid.width -= MoveUnit;
			break;
		case VK_RIGHT : 
			//Grid Cell
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
					if ( j == ColCursor ) {
						if ( BIND_STATUS_MASTER != m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].width += MoveUnit;
						} else {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].origin_width += MoveUnit;
						}
					}
					if ( j > ColCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].left += MoveUnit;
					}
				}
			}
			// Bind Cell
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
					if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
						INT Row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row;
						INT Col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col;

						INT TempWidht  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].left - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].left + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].width;
						INT TempHeight = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].top  - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].top  + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].height;

						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].width  = TempWidht;
						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].height = TempHeight;
					}
				}
			}
			// Grid
			m_pSheet->object[m_CaretInfo.object_index]->grid.width += MoveUnit;
			break;
		case VK_UP : 
			// Grid Cell
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
					if ( i == RowCursor ) {
						if ( BIND_STATUS_MASTER != m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].height -= MoveUnit;
						} else {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].origin_height -= MoveUnit;
						}
					}
					if ( i > RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].top -= MoveUnit;
					}
				}
			}
			// Bind Cell
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
					if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
						INT Row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row;
						INT Col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col;

						INT TempWidht  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].left - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].left + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].width;
						INT TempHeight = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].top  - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].top  + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].height;

						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].width  = TempWidht;
						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].height = TempHeight;
					}
				}
			}
			// Grid
			m_pSheet->object[m_CaretInfo.object_index]->grid.height -= MoveUnit;
			break;
		case VK_DOWN : 
			// Grid Cell
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
					if ( i == RowCursor ) {
						if ( BIND_STATUS_MASTER != m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].height += MoveUnit;
						} else {
							m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].origin_height += MoveUnit;
						}
					}
					if ( i > RowCursor ) {
						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].top += MoveUnit;
					}
				}
			}
			// Bind Cell
			for ( i = 0; i < m_pSheet->object[m_CaretInfo.object_index]->grid.row_count; i++ ) {
				for ( j = 0; j < m_pSheet->object[m_CaretInfo.object_index]->grid.col_count; j++ ) {
					if ( BIND_STATUS_MASTER == m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_status ) {
						INT Row = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_row;
						INT Col = m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].bind_col;

						INT TempWidht  = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].left - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].left + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].width;
						INT TempHeight = m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].top  - m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].top  + m_pSheet->object[m_CaretInfo.object_index]->grid.block[Row][Col].height;

						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].width  = TempWidht;
						m_pSheet->object[m_CaretInfo.object_index]->grid.block[i][j].height = TempHeight;
					}
				}
			}
			// Grid
			m_pSheet->object[m_CaretInfo.object_index]->grid.height += MoveUnit;
			break;
	}

	// Redrawing
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// KeyMoveFocus
//========================================================================
INT CWorkSheet::KeyMoveFocus( CDC * pDC, CSize & Size, UINT KeyValue, INT Mode )
{
ECO_EXCEPTION_BEGIN
	switch ( m_pSheet->object[m_CaretInfo.object_index]->kind ) {
		case OBJECT_TABLE :
			switch ( Mode ) {
				case KEY_MOVE_NORMAL  : KeyMoveFocusCursorTable( pDC, KeyValue        ); break;
				case KEY_MOVE_SHIFT   : KeyMoveFocusExpandTable( pDC, KeyValue, FALSE ); break;
				case KEY_MOVE_CONTROL : KeyMoveFocusExpandTable( pDC, KeyValue, TRUE  ); break;
			}
			break;
		case OBJECT_GRID :
			switch ( Mode ) {
				case KEY_MOVE_NORMAL  : KeyMoveFocusCursorGrid( pDC, KeyValue        ); break;
				case KEY_MOVE_SHIFT   : KeyMoveFocusExpandGrid( pDC, KeyValue, FALSE ); break;
				case KEY_MOVE_CONTROL : KeyMoveFocusExpandGrid( pDC, KeyValue, TRUE  ); break;
			}
			break;
	}

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// KeyMove
//========================================================================
INT CWorkSheet::KeyMove( CDC * pDC, CSize & Size, UINT KeyValue, INT Mode )
{
ECO_EXCEPTION_BEGIN
	// 화면 보정
	AdjustScrollDC( pDC, &Size );

	// 객체 키조작
	if ( -1 == m_CaretInfo.object_index ) {
		KeyMoveObject( pDC, Size, KeyValue, Mode ); 
	} else {
		switch ( m_pSheet->object[m_CaretInfo.object_index]->kind ) {
			case OBJECT_TABLE   :
			case OBJECT_GRID    :
				switch ( m_CaretInfo.focus ) {
					case FALSE : KeyMoveObject( pDC, Size, KeyValue, Mode ); break;
					case TRUE  : KeyMoveFocus ( pDC, Size, KeyValue, Mode ); break;
				}
				break;
			case OBJECT_BOX     :
			case OBJECT_BITMAP  :
			case OBJECT_BARCODE :
			case OBJECT_HLINE   :
			case OBJECT_VLINE   :
				KeyMoveObject( pDC, Size, KeyValue, Mode ); 
				break;
		}
	}
ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// F O N T
//====================================================================================
INT CWorkSheet::GetLogFont( INT FontNo, LOGFONT & LogFont )
{
ECO_EXCEPTION_BEGIN
	INT     FontIdx   [FONT_MAX] = {   8,   9,  10,  11,  12,  14,  16,  18,  20,  22,  24,  26,  28,  36,  48,  72 };
	INT     FontHeight[FONT_MAX] = { -11, -12, -13, -15, -16, -19, -21, -24, -27, -29, -32, -35, -37, -48, -64, -96 };
	LOGFONT TempLogFont          = { -11,   0,   0,   0, 400,   0,   0,   0, 129,   3,   2,   1,   49, _T("굴림체")     };
	
	//
	TempLogFont.lfHeight = FontHeight[FontNo];

	//
	wmemcpy( &LogFont, &TempLogFont, sizeof( LOGFONT ) );

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// GetCharSize
//========================================================================
INT CWorkSheet::GetCharSize( CDC * pDC, LOGFONT & LogFont, CSize & Size )
{
ECO_EXCEPTION_BEGIN
	CFont Font, * pOldFont;

	Font.CreateFontIndirect( &LogFont );
	pOldFont = pDC->SelectObject( &Font );

	Size = pDC->GetTextExtent( _T("W"), StringLength( _T("W") ) );

	pDC->SelectObject( pOldFont );

ECO_EXCEPTION_END
	return 0;
}

//====================================================================================
// PROPERTY
//====================================================================================
INT CWorkSheet::SetPropertyBlock( INT ObjectIdx, INT Row, INT Col, PROPERTY_INFO & PropertyInfo )
{
ECO_EXCEPTION_BEGIN
	// Text는 멀티로 입력되지 않는다.
	if ( FALSE == PropertyInfo.multi ) {
		if ( NULL != m_pSheet->object[ObjectIdx]->grid.block[Row][Col].text ) {
 			free( m_pSheet->object[ObjectIdx]->grid.block[Row][Col].text );
		}
		m_pSheet->object[ObjectIdx]->grid.block[Row][Col].text = ( _TCHAR * )malloc( sizeof( _TCHAR ) * StringLength( PropertyInfo.grid.text ) + sizeof( _TCHAR ) ); // Multi Language
		_tcscpy( m_pSheet->object[ObjectIdx]->grid.block[Row][Col].text, PropertyInfo.grid.text );
		//m_pSheet->object[ObjectIdx]->table.cell[Row][Col].text_len = StringLength( PropertyInfo.grid.text );
	}
	//
	m_pSheet->object[ObjectIdx]->grid.block[Row][Col].horz_align   = PropertyInfo.grid.horz_align;
	m_pSheet->object[ObjectIdx]->grid.block[Row][Col].fill         = PropertyInfo.grid.fill;
	m_pSheet->object[ObjectIdx]->grid.block[Row][Col].orientation  = PropertyInfo.grid.orientation;
	m_pSheet->object[ObjectIdx]->grid.block[Row][Col].slash        = PropertyInfo.grid.slash;
	wmemcpy( &m_pSheet->object[ObjectIdx]->grid.block[Row][Col].font, &PropertyInfo.grid.font, sizeof( LOGFONT ) );

ECO_EXCEPTION_END
	return 0;
}	

INT CWorkSheet::SetPropertyCell ( INT ObjectIdx, INT Row, INT Col, PROPERTY_INFO & PropertyInfo )
{
ECO_EXCEPTION_BEGIN
	// Text는 멀티로 입력되지 않는다.
	if ( FALSE == PropertyInfo.multi ) {
		if ( NULL != m_pSheet->object[ObjectIdx]->table.cell[Row][Col].text ) {
			free( m_pSheet->object[ObjectIdx]->table.cell[Row][Col].text );
		}
		m_pSheet->object[ObjectIdx]->table.cell[Row][Col].text = ( _TCHAR * )malloc( sizeof( _TCHAR ) * StringLength( PropertyInfo.table.text ) + sizeof( _TCHAR ) ); // Multi Language
		_tcscpy( m_pSheet->object[ObjectIdx]->table.cell[Row][Col].text, PropertyInfo.table.text );
	}
	//
	m_pSheet->object[ObjectIdx]->table.cell[Row][Col].fill = PropertyInfo.table.fill;

ECO_EXCEPTION_END
	return 0;
}

//========================================================================
// ChangeLinkedName
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::ChangeLinkedName( INT ObjectIdx, _TCHAR * szNewName )
{
	INT i, j;

	if ( 0 == _tcsicmp( m_pSheet->object[ObjectIdx]->comm.name, szNewName ) ) {
		return 0;
	}

	// 1. Parent의 자식객체 이름 정보를 바꾼다.
	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( OBJECT_GRID != m_pSheet->object[i]->kind ) {
			continue;
		}
		if ( 0 == _tcsicmp( m_pSheet->object[i]->grid.name, m_pSheet->object[ObjectIdx]->comm.parent_name ) ) {
			for ( j = 0; j < m_pSheet->object[i]->grid.block[m_pSheet->object[ObjectIdx]->comm.block_row][m_pSheet->object[ObjectIdx]->comm.block_col].child_count; j++ ) {
				if ( 0 == _tcsicmp( m_pSheet->object[i]->grid.block[m_pSheet->object[ObjectIdx]->comm.block_row][m_pSheet->object[ObjectIdx]->comm.block_col].child_name[j], m_pSheet->object[ObjectIdx]->comm.name ) ) {
					_tcscpy( m_pSheet->object[i]->grid.block[m_pSheet->object[ObjectIdx]->comm.block_row][m_pSheet->object[ObjectIdx]->comm.block_col].child_name[j], szNewName );
					break;
				}
			}
		}
	}

	// 2. Child의 부모객체 이름 정보를 바꾼다.
	if ( OBJECT_GRID != m_pSheet->object[ObjectIdx]->kind ) {
		return 0;
	}

	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( 0 == _tcsicmp( m_pSheet->object[i]->comm.parent_name, m_pSheet->object[ObjectIdx]->comm.name ) ) {
			_tcscpy( m_pSheet->object[i]->comm.parent_name, szNewName );
		}
	}

	return 0;

}

//========================================================================
// SetPropertyDisplay
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::SetPropertyDisplay( CDC * pDC, CSize & Size, PROPERTY_INFO & PropertyInfo )
{
ECO_EXCEPTION_BEGIN
	INT   i, j, ObjectIdx, Col, Row;
	CRect Rect;
	CSize CharSize, ObjectRect;

	// 1. 속성을 설정한 객체의 인덱스를 설정한다.
	ObjectIdx = m_PropertyInfo.object_index;

	// 2. 속성을 설정한다.
	switch ( m_PropertyInfo.object_kind ) {
		// 2.1 그리드의 속성을 변경한다.
		case OBJECT_GRID :
			Row  = m_pSheet->object[ObjectIdx]->grid.cursor_row;
			Col  = m_pSheet->object[ObjectIdx]->grid.cursor_col;
			// GRID
			ChangeLinkedName( ObjectIdx, m_PropertyInfo.grid.name );
			_tcscpy( m_pSheet->object[ObjectIdx]->grid.name, m_PropertyInfo.grid.name );
			m_pSheet->object[ObjectIdx]->grid.horz_line = m_PropertyInfo.grid.horz_line;
			m_pSheet->object[ObjectIdx]->grid.vert_line = m_PropertyInfo.grid.vert_line;
			// BLOCK
			if ( TRUE == m_PropertyInfo.multi ) {
				Rect.SetRect( m_pSheet->object[ObjectIdx]->grid.anchor_col, m_pSheet->object[ObjectIdx]->grid.anchor_row, Col, Row );
				Rect.NormalizeRect();

				m_pSheet->object[ObjectIdx]->grid.visible = m_PropertyInfo.grid.visible;
				for ( i = Rect.top; i <= Rect.bottom; i++ ) {
					for ( j = Rect.left; j <= Rect.right; j++ ) {
						SetPropertyBlock( ObjectIdx, i, j, m_PropertyInfo );
					}
				}
			} else {
				m_pSheet->object[ObjectIdx]->grid.visible = m_PropertyInfo.grid.visible;
				// Block Property
				SetPropertyBlock( ObjectIdx, Row, Col, m_PropertyInfo );
			}
			break;
		// 2.2 테이블의 속성을 설정한다.
		case OBJECT_TABLE :
			Row  = m_pSheet->object[ObjectIdx]->table.cursor_row;
			Col  = m_pSheet->object[ObjectIdx]->table.cursor_col;
			// TABLE
			ChangeLinkedName( ObjectIdx, m_PropertyInfo.table.name );
			_tcscpy( m_pSheet->object[ObjectIdx]->table.name,       m_PropertyInfo.table.name       );
			_tcscpy( m_pSheet->object[ObjectIdx]->table.data_name,  m_PropertyInfo.table.data_name  );
			m_pSheet->object[ObjectIdx]->table.visible                    = m_PropertyInfo.table.visible;
			m_pSheet->object[ObjectIdx]->table.horz_line   = m_PropertyInfo.table.horz_line;
			m_pSheet->object[ObjectIdx]->table.vert_line   = m_PropertyInfo.table.vert_line;
			m_pSheet->object[ObjectIdx]->table.area        = m_PropertyInfo.table.area;
			m_pSheet->object[ObjectIdx]->table.line_expand = m_PropertyInfo.table.line_expand;
			m_pSheet->object[ObjectIdx]->table.line_max    = m_PropertyInfo.table.line_max;
			wmemcpy( &m_pSheet->object[ObjectIdx]->table.font, &PropertyInfo.table.font, sizeof( LOGFONT ) );
			// 그리드의 사이즈를 설정한다.
			GetCharSize( pDC, m_PropertyInfo.table.font, CharSize );
			m_pSheet->object[ObjectIdx]->table.char_cx = CharSize.cx;
			m_pSheet->object[ObjectIdx]->table.char_cy = CharSize.cy;

			if ( TRUE == m_PropertyInfo.multi ) {
				Rect.SetRect( m_pSheet->object[ObjectIdx]->table.anchor_col, m_pSheet->object[ObjectIdx]->table.anchor_row, Col, Row );
				Rect.NormalizeRect();

				for ( i = Rect.top; i <= Rect.bottom; i++ ) {
					// Block Property
					for ( j = Rect.left; j <= Rect.right; j++ ) {
						// Col
						m_pSheet->object[ObjectIdx]->table.horz_align[j] = m_PropertyInfo.table.horz_align;
						m_pSheet->object[ObjectIdx]->table.vert_align[j] = m_PropertyInfo.table.vert_align;
						// Cell Property
						SetPropertyCell( ObjectIdx, i, j, m_PropertyInfo );
					}
				}
			} else {
				// Col Property
				m_pSheet->object[ObjectIdx]->table.horz_align[Col] = m_PropertyInfo.table.horz_align;
				m_pSheet->object[ObjectIdx]->table.vert_align[Col] = m_PropertyInfo.table.vert_align;
				m_pSheet->object[ObjectIdx]->table.char_len  [Col] = m_PropertyInfo.table.char_len;
				m_pSheet->object[ObjectIdx]->table.char_max  [Col] = m_PropertyInfo.table.char_max;
				m_pSheet->object[ObjectIdx]->table.indent_len[Col] = m_PropertyInfo.table.indent_len;
				if ( NULL != m_pSheet->object[ObjectIdx]->table.data_field[Col] ) {
					free( m_pSheet->object[ObjectIdx]->table.data_field[Col] );
				}
				m_pSheet->object[ObjectIdx]->table.data_field[Col] = ( _TCHAR * )malloc( sizeof( _TCHAR ) * StringLength( m_PropertyInfo.table.data_field ) + sizeof( _TCHAR ) );  // Multi Language
				_tcscpy( m_pSheet->object[ObjectIdx]->table.data_field[Col], m_PropertyInfo.table.data_field );
				//
				SetPropertyCell( ObjectIdx, Row, Col, m_PropertyInfo );
			}
			break;
		case OBJECT_BARCODE :
			ChangeLinkedName( ObjectIdx, m_PropertyInfo.barcode.name );
			_tcscpy( m_pSheet->object[ObjectIdx]->barcode.name , m_PropertyInfo.barcode.name  );
			_tcscpy( m_pSheet->object[ObjectIdx]->barcode.value, m_PropertyInfo.barcode.value );
			m_pSheet->object[ObjectIdx]->barcode.code = m_PropertyInfo.barcode.code;
			m_pSheet->object[ObjectIdx]->barcode.bc->SetCaption( CString( m_pSheet->object[ObjectIdx]->barcode.value ) );
			m_pSheet->object[ObjectIdx]->barcode.bc->SetStyle( m_pSheet->object[ObjectIdx]->barcode.code );
			break;
		case OBJECT_BITMAP :
			ChangeLinkedName( ObjectIdx, m_PropertyInfo.barcode.name );
			_tcscpy( m_pSheet->object[ObjectIdx]->bitmap.name     , m_PropertyInfo.bitmap.name      );
			_tcscpy( m_pSheet->object[ObjectIdx]->bitmap.file_name, m_PropertyInfo.bitmap.file_name );
			if ( NULL != m_pSheet->object[ObjectIdx]->bitmap.dib ) {
				delete m_pSheet->object[ObjectIdx]->bitmap.dib;
			}
			m_pSheet->object[ObjectIdx]->bitmap.dib = new CPicture;
			m_pSheet->object[ObjectIdx]->bitmap.dib->Load( m_pSheet->object[ObjectIdx]->bitmap.file_name );
			break;
		case OBJECT_HLINE :
			ChangeLinkedName( ObjectIdx, m_PropertyInfo.hline.name );
			_tcscpy( m_pSheet->object[ObjectIdx]->hline.name, m_PropertyInfo.hline.name );
			m_pSheet->object[ObjectIdx]->hline.style = m_PropertyInfo.hline.style;
			break;
		case OBJECT_VLINE :
			ChangeLinkedName( ObjectIdx, m_PropertyInfo.vline.name );
			_tcscpy( m_pSheet->object[ObjectIdx]->vline.name, m_PropertyInfo.vline.name );
			m_pSheet->object[ObjectIdx]->vline.style = m_PropertyInfo.vline.style;
			break;
		case OBJECT_BOX :
			ChangeLinkedName( ObjectIdx, m_PropertyInfo.box.name );
			_tcscpy( m_pSheet->object[ObjectIdx]->box.name, m_PropertyInfo.box.name );
			m_pSheet->object[ObjectIdx]->box.style = m_PropertyInfo.box.style;
			break;
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// SetPropertyShape
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::SetPropertyShape( CDC * pDC, CSize & Size, PROPERTY_INFO & PropertyInfo )
{
ECO_EXCEPTION_BEGIN
	INT ObjectIdx, RowIdx, ColIdx;

	// 1. 속성을 설정한 객체의 인덱스를 설정한다.
	ObjectIdx = m_PropertyInfo.object_index;

	// 2. 속성을 설정한다.
	switch ( m_PropertyInfo.object_kind ) {
		// 2.1 그리드의 속성을 변경한다.
		case OBJECT_GRID :
			RowIdx = m_pSheet->object[ObjectIdx]->grid.cursor_row;
			ColIdx = m_pSheet->object[ObjectIdx]->grid.cursor_col;
			switch ( m_PropertyInfo.grid.shape_cmd ) {
				case PROPERTY_SHAPE_INSERT_ROW : InsertGridRow    ( ObjectIdx, RowIdx ); break;
				case PROPERTY_SHAPE_INSERT_COL : InsertGridColumn ( ObjectIdx, ColIdx ); break;
				case PROPERTY_SHAPE_REMOVE_ROW : RemoveGridRow    ( ObjectIdx, RowIdx ); break;
				case PROPERTY_SHAPE_REMOVE_COL : RemoveGridColumn ( ObjectIdx, ColIdx ); break;
				case PROPERTY_SHAPE_PAGE_ROW   : ToggleGridPageRow( ObjectIdx         ); break;
				case PROPERTY_SHAPE_LAST_ROW   : ToggleGridLastRow( ObjectIdx         ); break;
			}
			break;
		// 2.2 테이블의 속성을 설정한다.
		case OBJECT_TABLE :
			RowIdx = m_pSheet->object[ObjectIdx]->table.cursor_row;
			ColIdx = m_pSheet->object[ObjectIdx]->table.cursor_col;
			switch ( m_PropertyInfo.table.shape_cmd ) {
				case PROPERTY_SHAPE_INSERT_ROW : InsertTableRow   ( ObjectIdx, RowIdx ); break;
				case PROPERTY_SHAPE_INSERT_COL : InsertTableColumn( ObjectIdx, ColIdx ); break;
				case PROPERTY_SHAPE_REMOVE_ROW : RemoveTableRow   ( ObjectIdx, RowIdx ); break;
				case PROPERTY_SHAPE_REMOVE_COL : RemoveTableColumn( ObjectIdx, ColIdx ); break;
			}
			break;
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// SetProperty
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::SetProperty( CDC * pDC, CSize & Size, PROPERTY_INFO & PropertyInfo )
{
ECO_EXCEPTION_BEGIN
	// 1. 객체의 속성을 전달받는다.
	wmemcpy( &m_PropertyInfo, &PropertyInfo, sizeof( PROPERTY_INFO ) );

	// 2. 속성모드(Display/Shape)에 따라 속성을 설정한다.
	switch ( m_PropertyInfo.mode ) {
		case PROPERTY_MODE_DISPLAY :
			SetPropertyDisplay( pDC, Size, PropertyInfo );
			break;
		case PROPERTY_MODE_SHAPE :
			SetPropertyShape( pDC, Size, PropertyInfo );
			break;
	}

	// 3. 다시 그린다.
	AdjustScrollDC( pDC, &Size );
	DrawObjectToMemDC();
	CopyMemDCToScreen( pDC );
ECO_EXCEPTION_END

	return 0;
}

//====================================================================================
// EmbedObject
//====================================================================================
INT CWorkSheet::EmbedObject( INT ChildObjectIdx )
{
ECO_EXCEPTION_BEGIN
	INT    i, j, ParentObjectIdx = -1, ChildCnt;
	CRect  ParentRect, ChildRect, BlockRect;
	CPoint ChildPoint;


	// 1. 이미 부모객체가 있는지 조하여, 있으면 작업을 수행하지 않는다.
	if ( 0 != _tcsicmp( _T("null"), m_pSheet->object[ChildObjectIdx]->comm.parent_name ) ) {
		return 0;
	}
	if ( OBJECT_GRID == m_pSheet->object[ChildObjectIdx]->kind ) {
		for ( i = 0; i < m_pSheet->object[ChildObjectIdx]->grid.row_count; i++ ) {
			for ( j = 0; j < m_pSheet->object[ChildObjectIdx]->grid.col_count; j++ ) {
				if ( 0 < m_pSheet->object[ChildObjectIdx]->grid.block[i][j].child_count ) {
					return 0;
				}
			}
		}

	}

	// 2. 부모객체(그리드)를 찾는다.
	ChildPoint.x = m_pSheet->object[ChildObjectIdx]->comm.left;
	ChildPoint.y = m_pSheet->object[ChildObjectIdx]->comm.top;

	//for ( i = 0; i < m_pSheet->object_count; i++ ) {
	for ( i = m_pSheet->object_count - 1; i >= 0 ; i-- ) {
		if ( i == ChildObjectIdx ) {
			continue;
		}
		if ( OBJECT_GRID != m_pSheet->object[i]->kind ) {
			continue;
		}
		// 2.1 부모영역을 설정한다.
		ParentRect.SetRect( m_pSheet->object[i]->grid.left, 
			                m_pSheet->object[i]->grid.top,
					        m_pSheet->object[i]->grid.left + m_pSheet->object[i]->grid.width, 
                            m_pSheet->object[i]->grid.top  + m_pSheet->object[i]->grid.height );

		// 2.2 자식객체가 부모객체 위에 있는지 조사한다.
		if ( TRUE == ParentRect.PtInRect( ChildPoint ) ) {
			if ( i > ChildObjectIdx ) {
				ParentObjectIdx = -1;
			} else {
				ParentObjectIdx = i;
			}
			break;
		}
	}

	// 3. 부모객체가 없으면 작업을 종료한다.
	if ( -1 == ParentObjectIdx ) {
		return 0;
	}

	// 4. 부모객체에 자식객체를 삽입한다.
	for ( i = 0; i < m_pSheet->object[ParentObjectIdx]->grid.row_count; i++ ) {
		for ( j = 0; j < m_pSheet->object[ParentObjectIdx]->grid.col_count; j++ ) {
			// 4.1 슬레이브 블럭은 제외한다.
			if ( BIND_STATUS_SLAVE == m_pSheet->object[ParentObjectIdx]->grid.block[i][j].bind_status ) {
				continue;
			}

			// 4.2 블럭의 크기를 설정한다.
			BlockRect.SetRect( m_pSheet->object[ParentObjectIdx]->grid.left + m_pSheet->object[ParentObjectIdx]->grid.block[i][j].left,
		                       m_pSheet->object[ParentObjectIdx]->grid.top  + m_pSheet->object[ParentObjectIdx]->grid.block[i][j].top,
		                       m_pSheet->object[ParentObjectIdx]->grid.left + m_pSheet->object[ParentObjectIdx]->grid.block[i][j].left + m_pSheet->object[ParentObjectIdx]->grid.block[i][j].width,
							   m_pSheet->object[ParentObjectIdx]->grid.top  + m_pSheet->object[ParentObjectIdx]->grid.block[i][j].top  + m_pSheet->object[ParentObjectIdx]->grid.block[i][j].height );

			// 4.3 자식객체가 부모객체보다 크면 제외한다.
			if ( m_pSheet->object[ChildObjectIdx]->comm.width  > m_pSheet->object[ParentObjectIdx]->grid.block[i][j].width || 
				 m_pSheet->object[ChildObjectIdx]->comm.height > m_pSheet->object[ParentObjectIdx]->grid.block[i][j].height ) 
			{
				continue;
			}

			// 4.4 자식객체가 부모객체 위에 있으면 '붙이기' 처리를 한다.
			if ( TRUE == BlockRect.PtInRect( ChildPoint ) ) {
				if ( CHILD_MAX <= m_pSheet->object[ParentObjectIdx]->grid.block[i][j].child_count ) {
					AfxMessageBox( _T("자식객체 붙이기는 32개까지만 가능합니다.") );
					return 0;
				}
				if ( IDYES == AfxMessageBox( _T("객체를 붙이겠습니까?"), MB_YESNO | MB_ICONQUESTION ) ) {
					// 4.4.1 자식객체에 부모객체 정보를 설정한다.
					_tcscpy( m_pSheet->object[ChildObjectIdx]->comm.parent_name, m_pSheet->object[ParentObjectIdx]->grid.name );
					m_pSheet->object[ChildObjectIdx]->comm.block_row = i;
					m_pSheet->object[ChildObjectIdx]->comm.block_col = j;
					// 4.4.2 부모객체에 자식객체 정보를 설정한다.
					m_pSheet->object[ParentObjectIdx]->grid.block[i][j].child_count++;
					ChildCnt = m_pSheet->object[ParentObjectIdx]->grid.block[i][j].child_count;
					_tcscpy( m_pSheet->object[ParentObjectIdx]->grid.block[i][j].child_name[ChildCnt-1], m_pSheet->object[ChildObjectIdx]->comm.name );
					return 0;
				}
			}
		}
	}
ECO_EXCEPTION_END

	return 0;
}

/* 
                                     <<   F I L E   L A Y O U T  >>

|---------------- HEADER ---------------------|--------------- SHEET -----------------|-------- OBJECT --------|
+---------+-------+-------------+-------------+-------+-------+--------+--------------+------------------------+
| version | paper | orientation | sheet_count | title | width | height | object_count | select | kind | object |
+---------+-------+-------------+-------------+-------+-------+--------+--------------+------------------------+
*/
//====================================================================================
// Save
//
// PURPOSE : 레포트 파일을 저장한다.
// AUTHOR  : 우상옥
//====================================================================================
INT CWorkSheet::Save( _TCHAR * FileName )
{
ECO_EXCEPTION_BEGIN
	FILE * fp;
	_TCHAR   CurFileName[256];
	INT    i, j, k, l, Len;

	if ( NULL == FileName ) {
		_tcscpy( CurFileName, m_FileName );
	} else {
		_tcscpy( CurFileName, FileName );
		_tcscpy( m_FileName,  FileName );
	}

	if ( NULL == ( fp = _tfopen( CurFileName, _T("wb") ) ) ) {
		return -1;
	}
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);

	// Version
	_tcscpy ( m_ReportInfo.version, REPORT_FILE_VERSION );
	m_ReportInfo.version[24*sizeof(_TCHAR)] = 26; // _TEOF

	fwrite(  m_ReportInfo.version,      TEXT_LEN*sizeof(_TCHAR),       1,  fp ); // Version
	fwrite( &m_ReportInfo.sheet_width,  sizeof( INT  ), 1,  fp ); // Sheet Width
	fwrite( &m_ReportInfo.sheet_height, sizeof( INT  ), 1,  fp ); // Sheet Height
	fwrite( &m_ReportInfo.sheet_size,   sizeof( INT  ), 1,  fp ); // Sheet Size
	fwrite( &m_ReportInfo.paper,        sizeof( INT  ), 1,  fp ); // Paper
	fwrite( &m_ReportInfo.orientation,  sizeof( INT  ), 1,  fp ); // Orientation
	fwrite( &m_ReportInfo.print_dialog, sizeof( BOOL ), 1,  fp ); // Orientation
	fwrite( &m_ReportInfo.sheet_count,  sizeof( INT  ), 1,  fp ); // Sheet Count

	// Sheet
	for ( i = 0; i < m_ReportInfo.sheet_count; i++ ) {
		fwrite(  m_ReportInfo.sheet[i].title       , TEXT_LEN*sizeof(_TCHAR)     , 1, fp ); // Title
		fwrite( &m_ReportInfo.sheet[i].object_count, sizeof( INT ), 1, fp ); // Object Count
		fwrite( &m_ReportInfo.sheet[i].object_id   , sizeof( INT ), 1, fp ); // Object ID

		for ( j = 0; j < m_ReportInfo.sheet[i].object_count; j++ ) {
			fwrite( &m_ReportInfo.sheet[i].object[j]->select, sizeof( INT ), 1, fp ); // Object Count
			fwrite( &m_ReportInfo.sheet[i].object[j]->kind,   sizeof( INT ), 1, fp ); // Object Count
			switch ( m_ReportInfo.sheet[i].object[j]->kind ) {
				case OBJECT_TABLE :
					fwrite(  m_ReportInfo.sheet[i].object[j]->table.name,         TEXT_LEN*sizeof(_TCHAR)         , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.left ,        sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.top  ,        sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.width,        sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.height,       sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.parent_name,  TEXT_LEN*sizeof(_TCHAR)         , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.block_row,    sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.block_col,    sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.visible,      sizeof( UINT    ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->table.data_name,    TEXT_LEN*sizeof(_TCHAR)         , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.area,         sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.cursor_row,   sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.cursor_col,   sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.bind,         sizeof( BOOL    ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.anchor_row,   sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.anchor_col,   sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.horz_line,    sizeof( BOOL    ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.vert_line,    sizeof( BOOL    ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.line_expand,  sizeof( BOOL    ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.font,         sizeof( LOGFONT ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.char_cx,      sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.char_cy,      sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.line_max,     sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.row_count,    sizeof( INT     ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->table.col_count,    sizeof( INT     ), 1, fp );
					if ( 0 != m_ReportInfo.sheet[i].object[j]->table.col_count ) {
						fwrite(  m_ReportInfo.sheet[i].object[j]->table.char_len,   m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fwrite(  m_ReportInfo.sheet[i].object[j]->table.char_max,   m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fwrite(  m_ReportInfo.sheet[i].object[j]->table.indent_len, m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fwrite(  m_ReportInfo.sheet[i].object[j]->table.horz_align, m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fwrite(  m_ReportInfo.sheet[i].object[j]->table.vert_align, m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
					}
					for ( k = 0; k < m_ReportInfo.sheet[i].object[j]->table.col_count; k++ ) {
						if ( NULL != m_ReportInfo.sheet[i].object[j]->table.data_field[k] ) {
							Len = sizeof( _TCHAR ) * StringLength( m_ReportInfo.sheet[i].object[j]->table.data_field[k] ) + sizeof( _TCHAR ); // Multi Language
							fwrite( &Len, sizeof( INT ), 1, fp );
							if ( 0 != Len ) {
								fwrite( m_ReportInfo.sheet[i].object[j]->table.data_field[k], Len, 1, fp );
							}
						} else {
							Len = 0;
							fwrite( &Len, sizeof( INT ), 1, fp );
						}
					}
					for ( k = 0; k < m_ReportInfo.sheet[i].object[j]->table.row_count; k++ ) {
						for ( l = 0; l < m_ReportInfo.sheet[i].object[j]->table.col_count; l++ ) {
							fwrite( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].left,   sizeof( INT  ), 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].top,    sizeof( INT  ), 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].width,  sizeof( INT  ), 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].height, sizeof( INT  ), 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].focus,  sizeof( BOOL ), 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].fill,   sizeof( BOOL ), 1, fp );
							if ( NULL != m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text ) {
								Len = sizeof( _TCHAR ) * StringLength( m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text ) + sizeof( _TCHAR ); // Multi Language
								fwrite( &Len, sizeof( INT ), 1, fp );
								if ( 0 != Len ) {
									fwrite( m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text, Len, 1, fp );
								}
							} else {
								Len = 0;
								fwrite( &Len, sizeof( INT ), 1, fp );
							}
						}
					}
					break;
				case OBJECT_GRID :
					fwrite(  m_ReportInfo.sheet[i].object[j]->grid.name,         TEXT_LEN*sizeof(_TCHAR),       1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.left,         sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.top,          sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.width,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.height,       sizeof( INT  ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->grid.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block_row,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block_col,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.visible,      sizeof( UINT ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.page_row,     sizeof( BOOL ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.last_row,     sizeof( BOOL ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.cursor_row,   sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.cursor_col,   sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.bind,         sizeof( BOOL ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.anchor_row,   sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.anchor_col,   sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.horz_line,    sizeof( BOOL ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.vert_line,    sizeof( BOOL ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.row_count,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->grid.col_count,    sizeof( INT  ), 1, fp );

					for ( k = 0; k < m_ReportInfo.sheet[i].object[j]->grid.row_count; k++ ) {
						for ( l = 0; l < m_ReportInfo.sheet[i].object[j]->grid.col_count; l++ ) {
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].left,          sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].top,           sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].width,         sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].height,        sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].focus,         sizeof( BOOL    ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].fill,          sizeof( BOOL    ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].horz_align,    sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].horz_align,    sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].orientation,   sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].slash,         sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].font,          sizeof( LOGFONT ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_status,   sizeof( BOOL    ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_row,      sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_col,      sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_width,  sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_height, sizeof( INT     ) , 1, fp );
							fwrite( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].child_count,   sizeof( INT     ) , 1, fp );
							fwrite(  m_ReportInfo.sheet[i].object[j]->grid.block[k][l].child_name,    CHILD_MAX*TEXT_LEN*sizeof( _TCHAR ), 1, fp );

							if ( NULL != m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text ) {
								Len = sizeof( _TCHAR ) * StringLength( m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text ) + sizeof( _TCHAR ); // Multi Language
								fwrite( &Len, sizeof( INT ), 1, fp );
								if ( 0 != Len ) {
									fwrite( m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text, Len, 1, fp );
								}
							} else {
								Len = 0;
								fwrite( &Len, sizeof( INT ), 1, fp );
							}
						}
					}
					break;
				case OBJECT_HLINE :
					fwrite(  m_ReportInfo.sheet[i].object[j]->hline.name,         TEXT_LEN*sizeof(_TCHAR),       1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.left,         sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.top,          sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.width,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.height,       sizeof( INT  ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->hline.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.block_row,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.block_col,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.visible,      sizeof( UINT ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.thick,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->hline.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_VLINE :
					fwrite(  m_ReportInfo.sheet[i].object[j]->vline.name,         TEXT_LEN*sizeof(_TCHAR),       1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.left,         sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.top,          sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.width,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.height,       sizeof( INT  ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->vline.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.block_row,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.block_col,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.visible,      sizeof( UINT ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.thick,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->vline.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_BOX :
					fwrite(  m_ReportInfo.sheet[i].object[j]->box.name,         TEXT_LEN*sizeof(_TCHAR),       1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.left,         sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.top,          sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.width,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.height,       sizeof( INT  ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->box.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.block_row,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.block_col,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.visible,      sizeof( UINT ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.thick,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->box.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_BITMAP :
					fwrite(  m_ReportInfo.sheet[i].object[j]->bitmap.name,         TEXT_LEN*sizeof(_TCHAR),       1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->bitmap.left,         sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->bitmap.top,          sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->bitmap.width,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->bitmap.height,       sizeof( INT  ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->bitmap.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->bitmap.block_row,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->bitmap.block_col,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->bitmap.visible,      sizeof( UINT ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->bitmap.file_name,    PATH_LEN*sizeof(_TCHAR),       1, fp );
					break;
				case OBJECT_BARCODE :
					fwrite(  m_ReportInfo.sheet[i].object[j]->barcode.name,         TEXT_LEN*sizeof(_TCHAR),       1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->barcode.left,         sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->barcode.top,          sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->barcode.width,        sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->barcode.height,       sizeof( INT  ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->barcode.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->barcode.block_row,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->barcode.block_col,    sizeof( INT  ), 1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->barcode.visible,      sizeof( UINT ), 1, fp );
					fwrite(  m_ReportInfo.sheet[i].object[j]->barcode.value,        TEXT_LEN*sizeof(_TCHAR),       1, fp );
					fwrite( &m_ReportInfo.sheet[i].object[j]->barcode.code,         sizeof( INT  ), 1, fp );
					break;
			}
		}

	}

	// FILE CLOSE
	fclose( fp );
ECO_EXCEPTION_END

	return 0;
}

//====================================================================================
// Load
//
// PURPOSE : 레포트 파일을 읽는다.
// AUTHOR  : 우상옥
//====================================================================================
INT CWorkSheet::Load( _TCHAR * FileName )
{
ECO_EXCEPTION_BEGIN
	FILE * fp;
	INT    i, j, k, l, Len;

	// 1. 이전사용 정보를 Free하고 초기화한다.
	FreeReportInfo();
	FreeBitmap    ();
	InitReportInfo();
	InitMouseInfo ();
	InitSheetSize ();
	InitCaretInfo ();

	// 2. 파일명을 설정한다.
	_tcscpy( m_FileName, FileName );

	// 3. 파일을 연다.
	if ( NULL == ( fp = _tfopen( FileName, _T("rb") ) ) ) {
		NewSheet();
		return -1;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------

	// 4. 파일을 읽는다.
	// 4.1 헤더정보를 읽는다.
	fread( m_ReportInfo.version, TEXT_LEN*sizeof( _TCHAR ), 1, fp ); // Version
	if ( 0 != wmemcmp( m_ReportInfo.version, REPORT_FILE_VERSION, 24 ) ) { // Multi Language
		fclose( fp );
		NewSheet();
		return -1;
	}
	fread( &m_ReportInfo.sheet_width,  sizeof( INT  ), 1,  fp ); // Sheet Width
	fread( &m_ReportInfo.sheet_height, sizeof( INT  ), 1,  fp ); // Sheet Height
	fread( &m_ReportInfo.sheet_size,   sizeof( INT  ), 1,  fp ); // Sheet Size
	fread( &m_ReportInfo.paper,        sizeof( INT  ), 1,  fp ); // Paper
	fread( &m_ReportInfo.orientation,  sizeof( INT  ), 1,  fp ); // Orientation
	fread( &m_ReportInfo.print_dialog, sizeof( BOOL ), 1,  fp ); // Orientation
	fread( &m_ReportInfo.sheet_count,  sizeof( INT  ), 1,  fp ); // Sheet Count

	m_ReportInfo.sheet = ( REPORT_SHEET * )malloc( sizeof( REPORT_SHEET ) * m_ReportInfo.sheet_count );
	if ( NULL == m_ReportInfo.sheet ) {
		NewSheet();
		fclose( fp );
		return -1;
	}

	// 4.2 Sheet정보를 읽는다.
	for ( i = 0; i < m_ReportInfo.sheet_count; i++ ) {
		fread(  m_ReportInfo.sheet[i].title       , TEXT_LEN*sizeof(_TCHAR)     , 1, fp ); // Title
		fread( &m_ReportInfo.sheet[i].object_count, sizeof( INT ), 1, fp ); // Object Count
		fread( &m_ReportInfo.sheet[i].object_id   , sizeof( INT ), 1, fp ); // Object ID

		m_ReportInfo.sheet[i].object = ( REPORT_OBJECT ** )malloc( m_ReportInfo.sheet[i].object_count * sizeof( REPORT_OBJECT * ) );
		if ( NULL == m_ReportInfo.sheet[i].object ) {
			NewSheet();
			return -1;
		}
		for ( j = 0; j < m_ReportInfo.sheet[i].object_count; j++ ) {
			m_ReportInfo.sheet[i].object[j] = ( REPORT_OBJECT * )malloc( sizeof( REPORT_OBJECT ) );
			fread( &m_ReportInfo.sheet[i].object[j]->select, sizeof( INT ), 1, fp ); // Object Count
			fread( &m_ReportInfo.sheet[i].object[j]->kind,   sizeof( INT ), 1, fp ); // Object Count
			switch ( m_ReportInfo.sheet[i].object[j]->kind ) {
				case OBJECT_TABLE :
					fread(  m_ReportInfo.sheet[i].object[j]->table.name ,        TEXT_LEN*sizeof(_TCHAR), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.left ,        sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.top  ,        sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.width,        sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.height,       sizeof( INT     ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->table.parent_name,  TEXT_LEN*sizeof(_TCHAR), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.block_row,    sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.block_col,    sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.visible,      sizeof( UINT    ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->table.data_name,    TEXT_LEN*sizeof(_TCHAR), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.area,         sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.cursor_row,   sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.cursor_col,   sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.bind,         sizeof( BOOL    ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.anchor_row,   sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.anchor_col,   sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.horz_line,    sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.vert_line,    sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.line_expand,  sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.font,         sizeof( LOGFONT ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.char_cx,      sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.char_cy,      sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.line_max,     sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.row_count,    sizeof( INT     ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->table.col_count,    sizeof( INT     ), 1, fp );
					if ( 0 != m_ReportInfo.sheet[i].object[j]->table.col_count ) {
						m_ReportInfo.sheet[i].object[j]->table.char_len = ( INT * )malloc( m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == m_ReportInfo.sheet[i].object[j]->table.char_len ) {
							NewSheet();
							fclose( fp );
							return -1;
						}
						m_ReportInfo.sheet[i].object[j]->table.char_max   = ( INT * )malloc( m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == m_ReportInfo.sheet[i].object[j]->table.char_max ) {
							NewSheet();
							fclose( fp );
							return -1;
						}
						m_ReportInfo.sheet[i].object[j]->table.indent_len = ( INT * )malloc( m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == m_ReportInfo.sheet[i].object[j]->table.indent_len ) {
							NewSheet();
							fclose( fp );
							return -1;
						}
						m_ReportInfo.sheet[i].object[j]->table.horz_align = ( INT * )malloc( m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == m_ReportInfo.sheet[i].object[j]->table.horz_align ) {
							NewSheet();
							fclose( fp );
							return -1;
						}
						m_ReportInfo.sheet[i].object[j]->table.vert_align = ( INT * )malloc( m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == m_ReportInfo.sheet[i].object[j]->table.vert_align ) {
							NewSheet();
							fclose( fp );
							return -1;
						}
						fread( m_ReportInfo.sheet[i].object[j]->table.char_len,   m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fread( m_ReportInfo.sheet[i].object[j]->table.char_max,   m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fread( m_ReportInfo.sheet[i].object[j]->table.indent_len, m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fread( m_ReportInfo.sheet[i].object[j]->table.horz_align, m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fread( m_ReportInfo.sheet[i].object[j]->table.vert_align, m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
					} else {
						m_ReportInfo.sheet[i].object[j]->table.char_len   = NULL;
						m_ReportInfo.sheet[i].object[j]->table.char_max   = NULL;
						m_ReportInfo.sheet[i].object[j]->table.indent_len = NULL;
						m_ReportInfo.sheet[i].object[j]->table.horz_align = NULL; 
						m_ReportInfo.sheet[i].object[j]->table.vert_align = NULL; 
					}
					m_ReportInfo.sheet[i].object[j]->table.data_field = ( _TCHAR ** )malloc( m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( _TCHAR * ) );
					for ( k = 0; k < m_ReportInfo.sheet[i].object[j]->table.col_count; k++ ) {
						fread( &Len, sizeof( INT ), 1, fp );
						if ( 0 != Len ) {
							m_ReportInfo.sheet[i].object[j]->table.data_field[k] = ( _TCHAR * )malloc( Len ); // Multi Language
							if ( NULL == m_ReportInfo.sheet[i].object[j]->table.data_field[k] ) {
								NewSheet();
								fclose( fp );
								return -1;
							}
							fread( m_ReportInfo.sheet[i].object[j]->table.data_field[k], Len, 1, fp );
						} else {
							m_ReportInfo.sheet[i].object[j]->table.data_field[k] = NULL;	
						}
					}
					m_ReportInfo.sheet[i].object[j]->table.cell = ( REPORT_CELL ** )malloc( m_ReportInfo.sheet[i].object[j]->table.row_count * sizeof( REPORT_CELL * ) );
					if ( NULL == m_ReportInfo.sheet[i].object[j]->table.cell ) {
						return -1;
					}
					for ( k = 0; k < m_ReportInfo.sheet[i].object[j]->table.row_count; k++ ) {
						m_ReportInfo.sheet[i].object[j]->table.cell[k] = ( REPORT_CELL * )malloc( m_ReportInfo.sheet[i].object[j]->table.col_count * sizeof( REPORT_CELL ) );
						if ( NULL == m_ReportInfo.sheet[i].object[j]->table.cell[k] ) {
							NewSheet();
							fclose( fp );
							return -1;
						}
						for ( l = 0; l < m_ReportInfo.sheet[i].object[j]->table.col_count; l++ ) {
							fread( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].left,   sizeof( INT  ), 1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].top,    sizeof( INT  ), 1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].width,  sizeof( INT  ), 1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].height, sizeof( INT  ), 1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].focus,  sizeof( BOOL ), 1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->table.cell[k][l].fill,   sizeof( BOOL ), 1, fp );
							fread( &Len, sizeof( INT ), 1, fp );
							if ( 0 != Len ) {
								m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text = ( _TCHAR * )malloc( Len ); // Multi Language
								if ( NULL == m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text ) {
									NewSheet();
									fclose( fp );
									return -1;
								}
								fread( m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text, Len, 1, fp );
							} else {
								m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text = NULL;
							}
						}
					}
					break;
				case OBJECT_GRID  :
					fread(  m_ReportInfo.sheet[i].object[j]->grid.name,         TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.left,         sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.top,          sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.width,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.height,       sizeof( INT  ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->grid.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.visible,      sizeof( UINT ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.page_row,     sizeof( BOOL ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.last_row,     sizeof( BOOL ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.cursor_row,   sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.cursor_col,   sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.bind,         sizeof( BOOL ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.anchor_row,   sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.anchor_col,   sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.horz_line,    sizeof( BOOL ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.vert_line,    sizeof( BOOL ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.row_count,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->grid.col_count,    sizeof( INT  ), 1, fp );
					m_ReportInfo.sheet[i].object[j]->grid.block = ( REPORT_BLOCK ** )malloc( m_ReportInfo.sheet[i].object[j]->grid.row_count * sizeof( REPORT_BLOCK * ) );
					for ( k = 0; k < m_ReportInfo.sheet[i].object[j]->grid.row_count; k++ ) {
						m_ReportInfo.sheet[i].object[j]->grid.block[k] = ( REPORT_BLOCK * )malloc( m_ReportInfo.sheet[i].object[j]->grid.col_count * sizeof( REPORT_BLOCK ) );
						for ( l = 0; l < m_ReportInfo.sheet[i].object[j]->grid.col_count; l++ ) {
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].left,          sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].top,           sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].width,         sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].height,        sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].focus,         sizeof( BOOL    ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].fill,          sizeof( BOOL    ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].horz_align,    sizeof( BOOL    ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].horz_align,    sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].orientation,   sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].slash,         sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].font,          sizeof( LOGFONT ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_status,   sizeof( BOOL    ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_row,      sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_col,      sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_width,  sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_height, sizeof( INT     ),   1, fp );
							fread( &m_ReportInfo.sheet[i].object[j]->grid.block[k][l].child_count,   sizeof( INT     ),   1, fp );
							fread(  m_ReportInfo.sheet[i].object[j]->grid.block[k][l].child_name,    CHILD_MAX*TEXT_LEN*sizeof(_TCHAR),  1, fp );
							fread( &Len, sizeof( INT ), 1, fp );
							if ( 0 != Len ) {
								m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text = ( _TCHAR * )malloc( Len ); // Multi Language
								if ( NULL == m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text ) {
									NewSheet();
									fclose( fp );
									return -1;
								}
								fread( m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text, Len, 1, fp );
							} else {
								m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text = NULL;
							}
						}
					}
					break;
				case OBJECT_HLINE :
					fread(  m_ReportInfo.sheet[i].object[j]->hline.name,         TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.left,         sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.top,          sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.width,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.height,       sizeof( INT  ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->hline.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.visible,      sizeof( UINT ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.thick,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->hline.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_VLINE :
					fread(  m_ReportInfo.sheet[i].object[j]->vline.name,         TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.left,         sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.top,          sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.width,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.height,       sizeof( INT  ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->vline.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.visible,      sizeof( UINT ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.thick,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->vline.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_BOX :
					fread(  m_ReportInfo.sheet[i].object[j]->box.name,         TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.left,         sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.top,          sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.width,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.height,       sizeof( INT  ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->box.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.visible,      sizeof( UINT ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.thick,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->box.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_BITMAP :
					fread(  m_ReportInfo.sheet[i].object[j]->bitmap.name,         TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->bitmap.left,         sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->bitmap.top,          sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->bitmap.width,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->bitmap.height,       sizeof( INT  ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->bitmap.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->bitmap.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->bitmap.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->bitmap.visible,      sizeof( UINT ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->bitmap.file_name,    PATH_LEN*sizeof(_TCHAR),       1, fp );
					m_ReportInfo.sheet[i].object[j]->bitmap.dib = new CPicture;
					m_ReportInfo.sheet[i].object[j]->bitmap.dib->Load( m_ReportInfo.sheet[i].object[j]->bitmap.file_name );
					break;
				case OBJECT_BARCODE :
					fread(  m_ReportInfo.sheet[i].object[j]->barcode.name,          TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->barcode.left,         sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->barcode.top,          sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->barcode.width,        sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->barcode.height,       sizeof( INT  ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->barcode.parent_name,  TEXT_LEN*sizeof(_TCHAR)      , 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->barcode.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->barcode.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->barcode.visible,      sizeof( UINT ), 1, fp );
					fread(  m_ReportInfo.sheet[i].object[j]->barcode.value,        TEXT_LEN*sizeof(_TCHAR),       1, fp );
					fread( &m_ReportInfo.sheet[i].object[j]->barcode.code,         sizeof( INT  ), 1, fp );
					m_ReportInfo.sheet[i].object[j]->barcode.bc = new CBarcodCtrl;
					m_ReportInfo.sheet[i].object[j]->barcode.bc->SetCaption( CString( m_ReportInfo.sheet[i].object[j]->barcode.value ) ); 
					break;
			}
		}

	}

	// 5. 파일을 닫는다.
	fclose( fp );

	// 6. 첫 Sheet을 작업 Sheet로 설정한다.
	m_pSheet = &m_ReportInfo.sheet[0];
ECO_EXCEPTION_END

	return 0;
}

//====================================================================================
// Import
//
// PURPOSE : 레포트 파일을 읽어 양식을 추가한다.
// AUTHOR  : 우상옥
//====================================================================================
INT CWorkSheet::Import( _TCHAR * FileName )
{
ECO_EXCEPTION_BEGIN
	FILE      * fp;
	INT         i, j, k, l, Len, NewSheetIdx;
	REPORT_INFO ReportInfo;

	// 1. 파일을 연다.
	if ( NULL == ( fp = _tfopen( FileName, _T("rb") ) ) ) {
		return -1;
	}

	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------

	// 2. 파일을 읽는다.
	// 2.1 헤더정보를 읽는다.
	fread( ReportInfo.version,       TEXT_LEN,       1,  fp ); // Version
	if ( 0 != wmemcmp( ReportInfo.version, REPORT_FILE_VERSION, 24 ) ) { // Multi Language
		fclose( fp );
		return -1;
	}
	fread( &ReportInfo.sheet_width,  sizeof( INT  ), 1,  fp ); // Sheet Width
	fread( &ReportInfo.sheet_height, sizeof( INT  ), 1,  fp ); // Sheet Height
	fread( &ReportInfo.sheet_size,   sizeof( INT  ), 1,  fp ); // Sheet Size
	fread( &ReportInfo.paper,        sizeof( INT  ), 1,  fp ); // Paper
	fread( &ReportInfo.orientation,  sizeof( INT  ), 1,  fp ); // Orientation
	fread( &ReportInfo.print_dialog, sizeof( BOOL ), 1,  fp ); // Orientation
	fread( &ReportInfo.sheet_count,  sizeof( INT  ), 1,  fp ); // Sheet Count

	ReportInfo.sheet = ( REPORT_SHEET * )malloc( sizeof( REPORT_SHEET ) * ReportInfo.sheet_count );
	if ( NULL == ReportInfo.sheet ) {
		fclose( fp );
		return -1;
	}

	// 2.2 Sheet정보를 읽는다.
	for ( i = 0; i < ReportInfo.sheet_count; i++ ) {
		fread(  ReportInfo.sheet[i].title       , TEXT_LEN     , 1, fp ); // Title
		_stprintf( ReportInfo.sheet[i].title, _T("%s_%d"), ReportInfo.sheet[i].title, m_ReportInfo.sheet_count + i );
		fread( &ReportInfo.sheet[i].object_count, sizeof( INT ), 1, fp ); // Object Count
		fread( &ReportInfo.sheet[i].object_id   , sizeof( INT ), 1, fp ); // Object ID

		ReportInfo.sheet[i].object = ( REPORT_OBJECT ** )malloc( ReportInfo.sheet[i].object_count * sizeof( REPORT_OBJECT * ) );
		if ( NULL == ReportInfo.sheet[i].object ) {
			return -1;
		}
		for ( j = 0; j < ReportInfo.sheet[i].object_count; j++ ) {
			ReportInfo.sheet[i].object[j] = ( REPORT_OBJECT * )malloc( sizeof( REPORT_OBJECT ) );
			fread( &ReportInfo.sheet[i].object[j]->select, sizeof( INT ), 1, fp ); // Object Count
			fread( &ReportInfo.sheet[i].object[j]->kind,   sizeof( INT ), 1, fp ); // Object Count
			switch ( ReportInfo.sheet[i].object[j]->kind ) {
				case OBJECT_TABLE :
					fread(  ReportInfo.sheet[i].object[j]->table.name ,        TEXT_LEN         , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.left ,        sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.top  ,        sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.width,        sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.height,       sizeof( INT     ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->table.parent_name,  TEXT_LEN         , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.block_row,    sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.block_col,    sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.visible,      sizeof( UINT    ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->table.data_name,    TEXT_LEN         , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.area,         sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.cursor_row,   sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.cursor_col,   sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.bind,         sizeof( BOOL    ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.anchor_row,   sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.anchor_col,   sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.horz_line,    sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.vert_line,    sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.line_expand,  sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.font,         sizeof( LOGFONT ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.char_cx,      sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.char_cy,      sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.line_max,     sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.row_count,    sizeof( INT     ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->table.col_count,    sizeof( INT     ), 1, fp );
					if ( 0 != ReportInfo.sheet[i].object[j]->table.col_count ) {
						ReportInfo.sheet[i].object[j]->table.char_len   = ( INT * )malloc( ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == ReportInfo.sheet[i].object[j]->table.char_len ) {
							fclose( fp );
							return -1;
						}
						ReportInfo.sheet[i].object[j]->table.char_max   = ( INT * )malloc( ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == ReportInfo.sheet[i].object[j]->table.char_max ) {
							fclose( fp );
							return -1;
						}
						ReportInfo.sheet[i].object[j]->table.indent_len = ( INT * )malloc( ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == ReportInfo.sheet[i].object[j]->table.indent_len ) {
							fclose( fp );
							return -1;
						}
						ReportInfo.sheet[i].object[j]->table.horz_align = ( INT * )malloc( ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == ReportInfo.sheet[i].object[j]->table.horz_align ) {
							fclose( fp );
							return -1;
						}
						ReportInfo.sheet[i].object[j]->table.vert_align = ( INT * )malloc( ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ) );
						if ( NULL == ReportInfo.sheet[i].object[j]->table.vert_align ) {
							fclose( fp );
							return -1;
						}
						fread( ReportInfo.sheet[i].object[j]->table.char_len,   ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fread( ReportInfo.sheet[i].object[j]->table.char_max,   ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fread( ReportInfo.sheet[i].object[j]->table.indent_len, ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fread( ReportInfo.sheet[i].object[j]->table.horz_align, ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
						fread( ReportInfo.sheet[i].object[j]->table.vert_align, ReportInfo.sheet[i].object[j]->table.col_count * sizeof( INT ), 1, fp );
					} else {
						ReportInfo.sheet[i].object[j]->table.char_len   = NULL;
						ReportInfo.sheet[i].object[j]->table.char_max   = NULL;
						ReportInfo.sheet[i].object[j]->table.indent_len = NULL;
						ReportInfo.sheet[i].object[j]->table.horz_align = NULL; 
						ReportInfo.sheet[i].object[j]->table.vert_align = NULL; 
					}
					ReportInfo.sheet[i].object[j]->table.data_field = ( _TCHAR ** )malloc( ReportInfo.sheet[i].object[j]->table.col_count * sizeof( _TCHAR * ) );
					for ( k = 0; k < ReportInfo.sheet[i].object[j]->table.col_count; k++ ) {
						fread( &Len, sizeof( INT ), 1, fp );
						if ( 0 != Len ) {
							ReportInfo.sheet[i].object[j]->table.data_field[k] = ( _TCHAR * )malloc( sizeof( _TCHAR ) * Len ); // Multi Language
							if ( NULL == ReportInfo.sheet[i].object[j]->table.data_field[k] ) {
								fclose( fp );
								return -1;
							}
							fread( ReportInfo.sheet[i].object[j]->table.data_field[k], Len, 1, fp );
						} else {
							ReportInfo.sheet[i].object[j]->table.data_field[k] = NULL;	
						}
					}
					ReportInfo.sheet[i].object[j]->table.cell = ( REPORT_CELL ** )malloc( ReportInfo.sheet[i].object[j]->table.row_count * sizeof( REPORT_CELL * ) );
					if ( NULL == ReportInfo.sheet[i].object[j]->table.cell ) {
						return -1;
					}
					for ( k = 0; k < ReportInfo.sheet[i].object[j]->table.row_count; k++ ) {
						ReportInfo.sheet[i].object[j]->table.cell[k] = ( REPORT_CELL * )malloc( ReportInfo.sheet[i].object[j]->table.col_count * sizeof( REPORT_CELL ) );
						if ( NULL == ReportInfo.sheet[i].object[j]->table.cell[k] ) {
							fclose( fp );
							return -1;
						}
						for ( l = 0; l < ReportInfo.sheet[i].object[j]->table.col_count; l++ ) {
							fread( &ReportInfo.sheet[i].object[j]->table.cell[k][l].left,   sizeof( INT  ), 1, fp );
							fread( &ReportInfo.sheet[i].object[j]->table.cell[k][l].top,    sizeof( INT  ), 1, fp );
							fread( &ReportInfo.sheet[i].object[j]->table.cell[k][l].width,  sizeof( INT  ), 1, fp );
							fread( &ReportInfo.sheet[i].object[j]->table.cell[k][l].height, sizeof( INT  ), 1, fp );
							fread( &ReportInfo.sheet[i].object[j]->table.cell[k][l].focus,  sizeof( BOOL ), 1, fp );
							fread( &ReportInfo.sheet[i].object[j]->table.cell[k][l].fill,   sizeof( BOOL ), 1, fp );
							fread( &Len, sizeof( INT ), 1, fp );
							if ( 0 != Len ) {
								ReportInfo.sheet[i].object[j]->table.cell[k][l].text = ( _TCHAR * )malloc( sizeof( _TCHAR ) * Len ); // Multi Language
								if ( NULL == ReportInfo.sheet[i].object[j]->table.cell[k][l].text ) {
									fclose( fp );
									return -1;
								}
								fread( ReportInfo.sheet[i].object[j]->table.cell[k][l].text, Len, 1, fp );
							} else {
								ReportInfo.sheet[i].object[j]->table.cell[k][l].text = NULL;
							}
						}
					}
					break;
				case OBJECT_GRID  :
					fread(  ReportInfo.sheet[i].object[j]->grid.name,         TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.left,         sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.top,          sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.width,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.height,       sizeof( INT  ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->grid.parent_name,  TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.block_row,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.block_col,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.visible,      sizeof( UINT ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.page_row,     sizeof( BOOL ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.last_row,     sizeof( BOOL ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.cursor_row,   sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.cursor_col,   sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.bind,         sizeof( BOOL ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.anchor_row,   sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.anchor_col,   sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.horz_line,    sizeof( BOOL ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.vert_line,    sizeof( BOOL ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.row_count,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->grid.col_count,    sizeof( INT  ), 1, fp );
					ReportInfo.sheet[i].object[j]->grid.block = ( REPORT_BLOCK ** )malloc( ReportInfo.sheet[i].object[j]->grid.row_count * sizeof( REPORT_BLOCK * ) );
					for ( k = 0; k < ReportInfo.sheet[i].object[j]->grid.row_count; k++ ) {
						ReportInfo.sheet[i].object[j]->grid.block[k] = ( REPORT_BLOCK * )malloc( ReportInfo.sheet[i].object[j]->grid.col_count * sizeof( REPORT_BLOCK ) );
						for ( l = 0; l < ReportInfo.sheet[i].object[j]->grid.col_count; l++ ) {
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].left,          sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].top,           sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].width,         sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].height,        sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].focus,         sizeof( BOOL    ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].fill,          sizeof( BOOL    ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].horz_align,    sizeof( BOOL    ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].horz_align,    sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].orientation,   sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].slash,         sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].font,          sizeof( LOGFONT ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_status,   sizeof( BOOL    ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_row,      sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_col,      sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_width,  sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_height, sizeof( INT     ),   1, fp );
							fread( &ReportInfo.sheet[i].object[j]->grid.block[k][l].child_count,   sizeof( INT     ),   1, fp );
							fread(  ReportInfo.sheet[i].object[j]->grid.block[k][l].child_name,    CHILD_MAX*TEXT_LEN,  1, fp );
							fread( &Len, sizeof( INT ), 1, fp );
							if ( 0 != Len ) {
								ReportInfo.sheet[i].object[j]->grid.block[k][l].text = ( _TCHAR * )malloc( sizeof( _TCHAR ) * Len ); // Multi Language
								if ( NULL == ReportInfo.sheet[i].object[j]->grid.block[k][l].text ) {
									fclose( fp );
									return -1;
								}
								fread( ReportInfo.sheet[i].object[j]->grid.block[k][l].text, Len, 1, fp );
							} else {
								ReportInfo.sheet[i].object[j]->grid.block[k][l].text = NULL;
							}
						}
					}
					break;
				case OBJECT_HLINE :
					fread(  ReportInfo.sheet[i].object[j]->hline.name,         TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.left,         sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.top,          sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.width,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.height,       sizeof( INT  ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->hline.parent_name,  TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.block_row,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.block_col,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.visible,      sizeof( UINT ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.thick,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->hline.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_VLINE :
					fread(  ReportInfo.sheet[i].object[j]->vline.name,         TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.left,         sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.top,          sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.width,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.height,       sizeof( INT  ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->vline.parent_name,  TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.block_row,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.block_col,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.visible,      sizeof( UINT ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.thick,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->vline.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_BOX :
					fread(  ReportInfo.sheet[i].object[j]->box.name,         TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.left,         sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.top,          sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.width,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.height,       sizeof( INT  ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->box.parent_name,  TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.block_row,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.block_col,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.visible,      sizeof( UINT ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.thick,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->box.style,        sizeof( INT  ), 1, fp );
					break;
				case OBJECT_BITMAP :
					fread(  ReportInfo.sheet[i].object[j]->bitmap.name,         TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->bitmap.left,         sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->bitmap.top,          sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->bitmap.width,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->bitmap.height,       sizeof( INT  ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->bitmap.parent_name,  TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->bitmap.block_row,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->bitmap.block_col,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->bitmap.visible,      sizeof( UINT ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->bitmap.file_name,    PATH_LEN,       1, fp );
					ReportInfo.sheet[i].object[j]->bitmap.dib = new CPicture;
					ReportInfo.sheet[i].object[j]->bitmap.dib->Load( ReportInfo.sheet[i].object[j]->bitmap.file_name );
					break;
				case OBJECT_BARCODE :
					fread(  ReportInfo.sheet[i].object[j]->barcode.name,          TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->barcode.left,         sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->barcode.top,          sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->barcode.width,        sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->barcode.height,       sizeof( INT  ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->barcode.parent_name,  TEXT_LEN      , 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->barcode.block_row,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->barcode.block_col,    sizeof( INT  ), 1, fp );
					fread( &ReportInfo.sheet[i].object[j]->barcode.visible,      sizeof( UINT ), 1, fp );
					fread(  ReportInfo.sheet[i].object[j]->barcode.value,        TEXT_LEN,       1, fp );
					fread( &ReportInfo.sheet[i].object[j]->barcode.code,         sizeof( INT  ), 1, fp );
					ReportInfo.sheet[i].object[j]->barcode.bc = new CBarcodCtrl;
					ReportInfo.sheet[i].object[j]->barcode.bc->SetCaption( CString( ReportInfo.sheet[i].object[j]->barcode.value ) ); 
					break;
			}
		}

	}

	// 3. 파일을 닫는다.
	fclose( fp );

	// 4. 양식을 추가한다.
	NewSheetIdx = m_ReportInfo.sheet_count;
	m_ReportInfo.sheet_count += ReportInfo.sheet_count;
	m_ReportInfo.sheet = ( REPORT_SHEET * )realloc( m_ReportInfo.sheet, sizeof( REPORT_SHEET ) * m_ReportInfo.sheet_count );
	if ( NULL == m_ReportInfo.sheet ) {
		return -1;
	}
	wmemcpy( &m_ReportInfo.sheet[NewSheetIdx], &ReportInfo.sheet[0], sizeof( REPORT_SHEET ) * ReportInfo.sheet_count );
	
ECO_EXCEPTION_END

	return 0;
}

//====================================================================================
// Convertor
//====================================================================================
INT CWorkSheet::Load_Ver05( _TCHAR * FileName )
{
ECO_EXCEPTION_BEGIN
	FILE * fp;
	INT    i, j, k, l, Len;

	// FILE OPEN
	if ( NULL == ( fp = _tfopen( FileName, _T("rb") ) ) ) {
		return -1;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------


	// Version
	fread(  m_$ReportInfo.version,      TEXT_LEN,       1,  fp ); // Version
	fread( &m_$ReportInfo.sheet_width,  sizeof( INT  ), 1,  fp ); // Version
	fread( &m_$ReportInfo.sheet_height, sizeof( INT  ), 1,  fp ); // Version
	fread( &m_$ReportInfo.sheet_size,   sizeof( INT  ), 1,  fp ); // Version
	fread( &m_$ReportInfo.paper,        sizeof( INT  ), 1,  fp ); // Paper
	fread( &m_$ReportInfo.orientation,  sizeof( INT  ), 1,  fp ); // Orientation
	fread( &m_$ReportInfo.print_dialog, sizeof( BOOL ), 1,  fp ); // Orientation
	fread( &m_$ReportInfo.sheet_count,  sizeof( INT  ), 1,  fp ); // Sheet Count

	m_$ReportInfo.sheet = ( $REPORT_SHEET * )malloc( sizeof( $REPORT_SHEET ) * m_$ReportInfo.sheet_count );
	if ( NULL == m_$ReportInfo.sheet ) {
		return -1;
	}

	// Sheet
	for ( i = 0; i < m_$ReportInfo.sheet_count; i++ ) {
		fread(  m_$ReportInfo.sheet[i].title       , TEXT_LEN     , 1, fp ); // Title
		fread( &m_$ReportInfo.sheet[i].object_count, sizeof( INT ), 1, fp ); // Object Count

		m_$ReportInfo.sheet[i].object = ( $REPORT_OBJECT * )malloc( m_$ReportInfo.sheet[i].object_count * sizeof( $REPORT_OBJECT ) );
		if ( NULL == m_$ReportInfo.sheet[i].object ) {
			return -1;
		}
		for ( j = 0; j < m_$ReportInfo.sheet[i].object_count; j++ ) {
			fread( &m_$ReportInfo.sheet[i].object[j].select, sizeof( INT ), 1, fp ); // Object Count
			fread( &m_$ReportInfo.sheet[i].object[j].kind,   sizeof( INT ), 1, fp ); // Object Count
			switch ( m_$ReportInfo.sheet[i].object[j].kind ) {
				case OBJECT_TABLE :
					fread( &m_$ReportInfo.sheet[i].object[j].table.left ,        sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.top  ,        sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.width,        sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.height,       sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.parent_index, sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.block_row,    sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.block_col,    sizeof( INT     ), 1, fp );
					fread(  m_$ReportInfo.sheet[i].object[j].table.name,         TEXT_LEN         , 1, fp );
					fread(  m_$ReportInfo.sheet[i].object[j].table.data_name,    TEXT_LEN         , 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.area,         sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.cursor_row,   sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.cursor_col,   sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.bind,         sizeof( BOOL    ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.anchor_row,   sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.anchor_col,   sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.horz_line,    sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.vert_line,    sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.line_expand,  sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.font,         sizeof( LOGFONT ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.char_cx,      sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.char_cy,      sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.line_max,     sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.visible,      sizeof( UINT    ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.row_count,    sizeof( INT     ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].table.col_count,    sizeof( INT     ), 1, fp );
					if ( 0 != m_$ReportInfo.sheet[i].object[j].table.col_count ) {
						m_$ReportInfo.sheet[i].object[j].table.char_len   = ( INT * )malloc( m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ) );
						if ( NULL == m_$ReportInfo.sheet[i].object[j].table.char_len ) {
							return -1;
						}
						m_$ReportInfo.sheet[i].object[j].table.char_max   = ( INT * )malloc( m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ) );
						if ( NULL == m_$ReportInfo.sheet[i].object[j].table.char_max ) {
							return -1;
						}
						m_$ReportInfo.sheet[i].object[j].table.indent_len = ( INT * )malloc( m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ) );
						if ( NULL == m_$ReportInfo.sheet[i].object[j].table.indent_len ) {
							return -1;
						}
						m_$ReportInfo.sheet[i].object[j].table.horz_align = ( INT * )malloc( m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ) );
						if ( NULL == m_$ReportInfo.sheet[i].object[j].table.horz_align ) {
							return -1;
						}
						m_$ReportInfo.sheet[i].object[j].table.vert_align = ( INT * )malloc( m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ) );
						if ( NULL == m_$ReportInfo.sheet[i].object[j].table.vert_align ) {
							return -1;
						}
						fread( m_$ReportInfo.sheet[i].object[j].table.char_len,   m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ), 1, fp );
						fread( m_$ReportInfo.sheet[i].object[j].table.char_max,   m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ), 1, fp );
						fread( m_$ReportInfo.sheet[i].object[j].table.indent_len, m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ), 1, fp );
						fread( m_$ReportInfo.sheet[i].object[j].table.horz_align, m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ), 1, fp );
						fread( m_$ReportInfo.sheet[i].object[j].table.vert_align, m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( INT ), 1, fp );
					} else {
						m_$ReportInfo.sheet[i].object[j].table.char_len   = NULL;
						m_$ReportInfo.sheet[i].object[j].table.char_max   = NULL;
						m_$ReportInfo.sheet[i].object[j].table.indent_len = NULL;
						m_$ReportInfo.sheet[i].object[j].table.horz_align = NULL; 
						m_$ReportInfo.sheet[i].object[j].table.vert_align = NULL; 
					}
					m_$ReportInfo.sheet[i].object[j].table.data_field = ( _TCHAR ** )malloc( m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( _TCHAR * ) );
					for ( k = 0; k < m_$ReportInfo.sheet[i].object[j].table.col_count; k++ ) {
						fread( &Len, sizeof( INT ), 1, fp );
						if ( 0 != Len ) {
							m_$ReportInfo.sheet[i].object[j].table.data_field[k] = ( _TCHAR * )malloc( sizeof( _TCHAR ) * Len ); // Multi Language
							if ( NULL == m_$ReportInfo.sheet[i].object[j].table.data_field[k] ) {
								return -1;
							}
							fread( m_$ReportInfo.sheet[i].object[j].table.data_field[k], Len, 1, fp );
						} else {
							m_$ReportInfo.sheet[i].object[j].table.data_field[k] = NULL;	
						}
					}
					m_$ReportInfo.sheet[i].object[j].table.cell = ( $REPORT_CELL ** )malloc( m_$ReportInfo.sheet[i].object[j].table.row_count * sizeof( $REPORT_CELL * ) );
					if ( NULL == m_$ReportInfo.sheet[i].object[j].table.cell ) {
						return -1;
					}
					for ( k = 0; k < m_$ReportInfo.sheet[i].object[j].table.row_count; k++ ) {
						m_$ReportInfo.sheet[i].object[j].table.cell[k] = ( $REPORT_CELL * )malloc( m_$ReportInfo.sheet[i].object[j].table.col_count * sizeof( $REPORT_CELL ) );
						if ( NULL == m_$ReportInfo.sheet[i].object[j].table.cell[k] ) {
							return -1;
						}
						for ( l = 0; l < m_$ReportInfo.sheet[i].object[j].table.col_count; l++ ) {
							fread( &m_$ReportInfo.sheet[i].object[j].table.cell[k][l].left,   sizeof( INT  ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].table.cell[k][l].top,    sizeof( INT  ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].table.cell[k][l].width,  sizeof( INT  ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].table.cell[k][l].height, sizeof( INT  ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].table.cell[k][l].focus,  sizeof( BOOL ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].table.cell[k][l].fill,   sizeof( BOOL ), 1, fp );
							fread( &Len, sizeof( INT ), 1, fp );
							if ( 0 != Len ) {
								m_$ReportInfo.sheet[i].object[j].table.cell[k][l].text = ( _TCHAR * )malloc( sizeof( _TCHAR ) * Len ); // Multi Language
								if ( NULL == m_$ReportInfo.sheet[i].object[j].table.cell[k][l].text ) {
									return -1;
								}
								fread( m_$ReportInfo.sheet[i].object[j].table.cell[k][l].text, Len, 1, fp );
							} else {
								m_$ReportInfo.sheet[i].object[j].table.cell[k][l].text = NULL;
							}
						}
					}
					break;
				case OBJECT_GRID  :
					fread( &m_$ReportInfo.sheet[i].object[j].grid.left,         sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.top,          sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.width,        sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.height,       sizeof( INT  ), 1, fp );
					fread(  m_$ReportInfo.sheet[i].object[j].grid.name,         TEXT_LEN,       1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.page_row,     sizeof( BOOL ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.last_row,     sizeof( BOOL ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.cursor_row,   sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.cursor_col,   sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.bind,         sizeof( BOOL ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.anchor_row,   sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.anchor_col,   sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.horz_line,    sizeof( BOOL ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.vert_line,    sizeof( BOOL ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.row_count,    sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].grid.col_count,    sizeof( INT  ), 1, fp );
					if ( 0 != m_$ReportInfo.sheet[i].object[j].grid.col_count ) {
						m_$ReportInfo.sheet[i].object[j].grid.visible = ( UINT * )malloc( m_$ReportInfo.sheet[i].object[j].grid.row_count * sizeof( UINT ) );
						if ( NULL == m_$ReportInfo.sheet[i].object[j].grid.visible ) {
							return -1;
						}	
						fread( m_$ReportInfo.sheet[i].object[j].grid.visible, m_$ReportInfo.sheet[i].object[j].grid.row_count * sizeof( UINT ), 1, fp );
					} else {
						m_$ReportInfo.sheet[i].object[j].grid.visible = NULL;
					}
					m_$ReportInfo.sheet[i].object[j].grid.block = ( $REPORT_BLOCK ** )malloc( m_$ReportInfo.sheet[i].object[j].grid.row_count * sizeof( $REPORT_BLOCK * ) );
					for ( k = 0; k < m_$ReportInfo.sheet[i].object[j].grid.row_count; k++ ) {
						m_$ReportInfo.sheet[i].object[j].grid.block[k] = ( $REPORT_BLOCK * )malloc( m_$ReportInfo.sheet[i].object[j].grid.col_count * sizeof( $REPORT_BLOCK ) );
						for ( l = 0; l < m_$ReportInfo.sheet[i].object[j].grid.col_count; l++ ) {
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].left,          sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].top,           sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].width,         sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].height,        sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].focus,         sizeof( BOOL    ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].fill,          sizeof( BOOL    ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].horz_align,    sizeof( BOOL    ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].horz_align,    sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].orientation,   sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].slash,         sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].font,          sizeof( LOGFONT ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].bind_status,   sizeof( BOOL    ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].bind_row,      sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].bind_col,      sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].origin_width,  sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].origin_height, sizeof( INT     ), 1, fp );
							fread( &m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_count,   sizeof( INT     ), 1, fp );
							if ( 0 != m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_count ) {
								m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index = ( INT * )malloc( m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_count * sizeof( INT ) );
								if ( NULL == m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index ) {
									return -1;
								}
								fread( m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index, m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_count * sizeof( INT ), 1, fp );
							} else {
								m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index = NULL;
							}
							fread( &Len, sizeof( INT ), 1, fp );
							if ( 0 != Len ) {
								m_$ReportInfo.sheet[i].object[j].grid.block[k][l].text = ( _TCHAR * )malloc( sizeof( _TCHAR ) * Len ); // Multi Language
								if ( NULL == m_$ReportInfo.sheet[i].object[j].grid.block[k][l].text ) {
									return -1;
								}
								fread( m_$ReportInfo.sheet[i].object[j].grid.block[k][l].text, Len, 1, fp );
							} else {
								m_$ReportInfo.sheet[i].object[j].grid.block[k][l].text = NULL;
							}
						}
					}
					break;
				case OBJECT_HLINE :
					fread( &m_$ReportInfo.sheet[i].object[j].hline.left,         sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].hline.top,          sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].hline.width,        sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].hline.height,       sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].hline.parent_index, sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].hline.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].hline.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].hline.visible,      sizeof( UINT ), 1, fp );
					break;
				case OBJECT_VLINE :
					fread( &m_$ReportInfo.sheet[i].object[j].vline.left,         sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].vline.top,          sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].vline.width,        sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].vline.height,       sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].vline.parent_index, sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].vline.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].vline.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].vline.visible,      sizeof( UINT ), 1, fp );
					break;
				case OBJECT_BOX   :
					fread( &m_$ReportInfo.sheet[i].object[j].box.left,         sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].box.top,          sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].box.width,        sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].box.height,       sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].box.parent_index, sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].box.block_row,    sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].box.block_col,    sizeof( INT  ), 1, fp );
					fread( &m_$ReportInfo.sheet[i].object[j].box.visible,      sizeof( UINT ), 1, fp );
					break;
			}
		}

	}

	// FILE CLOSE
	fclose( fp );
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::GetChildKind( $REPORT_SHEET * pReportSheet, INT child_index, _TCHAR * child_name )
{
ECO_EXCEPTION_BEGIN
	switch ( pReportSheet->object[child_index].kind ) {
		case OBJECT_TABLE : _stprintf( child_name, _T("테이블_%d"), child_index ); break;
		case OBJECT_GRID  : _stprintf( child_name, _T("그리드_%d"), child_index ); break;
		case OBJECT_HLINE : _stprintf( child_name, _T("수평선_%d"), child_index ); break;
		case OBJECT_VLINE : _stprintf( child_name, _T("수직선_%d"), child_index ); break;
		case OBJECT_BOX   : _stprintf( child_name, _T("상자_%d")  , child_index ); break;
	}

ECO_EXCEPTION_END
	return 0;
}

INT CWorkSheet::Convertor_Ver05_To_06( VOID )
{
ECO_EXCEPTION_BEGIN
	INT  i, j, k, l, m;
	_TCHAR child_name[32];

	// 1. Report Info
	wmemcpy( &m_ReportInfo, &m_$ReportInfo, sizeof( $REPORT_INFO ) );
	m_ReportInfo.print_dialog = TRUE;

	// 2. Report Sheet
	m_ReportInfo.sheet = ( REPORT_SHEET * )malloc( sizeof( REPORT_SHEET ) * m_ReportInfo.sheet_count );

	for ( i = 0; i < m_$ReportInfo.sheet_count; i++ ) {
		_tcscpy( m_ReportInfo.sheet[i].title, m_$ReportInfo.sheet[i].title );
		m_ReportInfo.sheet[i].object_count = m_$ReportInfo.sheet[i].object_count;
		m_ReportInfo.sheet[i].object_id    = 100;
		m_ReportInfo.sheet[i].object = ( REPORT_OBJECT ** )malloc( sizeof( REPORT_OBJECT * ) * m_$ReportInfo.sheet[i].object_count );
		for ( j = 0; j < m_$ReportInfo.sheet[i].object_count; j++ ) {
			m_ReportInfo.sheet[i].object[j] = ( REPORT_OBJECT * )malloc( sizeof( REPORT_OBJECT ) );
			m_ReportInfo.sheet[i].object[j]->select = m_$ReportInfo.sheet[i].object[j].select;
			m_ReportInfo.sheet[i].object[j]->kind   = m_$ReportInfo.sheet[i].object[j].kind;

			switch ( m_$ReportInfo.sheet[i].object[j].kind ) {
				case OBJECT_TABLE :
					if ( 0 == _tcscmp( m_$ReportInfo.sheet[i].object[j].table.name, _T("") ) ) {
						_stprintf( m_ReportInfo.sheet[i].object[j]->table.name, _T("테이블_%d"), j );
					} else {
						_stprintf( m_ReportInfo.sheet[i].object[j]->table.name, m_$ReportInfo.sheet[i].object[j].table.name );
					}
					m_ReportInfo.sheet[i].object[j]->table.left        = m_$ReportInfo.sheet[i].object[j].table.left;
					m_ReportInfo.sheet[i].object[j]->table.top         = m_$ReportInfo.sheet[i].object[j].table.top;
					m_ReportInfo.sheet[i].object[j]->table.width       = m_$ReportInfo.sheet[i].object[j].table.width;
					m_ReportInfo.sheet[i].object[j]->table.height      = m_$ReportInfo.sheet[i].object[j].table.height;
					if ( -1 != m_$ReportInfo.sheet[i].object[j].table.parent_index ) {
						if ( 0 == _tcscmp( m_$ReportInfo.sheet[i].object[m_$ReportInfo.sheet[i].object[j].table.parent_index].grid.name, _T("") ) ) {
							_stprintf( m_ReportInfo.sheet[i].object[j]->table.parent_name, _T("그리드_%d"), m_$ReportInfo.sheet[i].object[j].table.parent_index );
						} else {
							_tcscpy( m_ReportInfo.sheet[i].object[j]->table.parent_name, m_$ReportInfo.sheet[i].object[m_$ReportInfo.sheet[i].object[j].table.parent_index].grid.name );
						}
					} else {
						_tcscpy( m_ReportInfo.sheet[i].object[j]->table.parent_name, _T("null") );
					}
					m_ReportInfo.sheet[i].object[j]->table.block_row   = m_$ReportInfo.sheet[i].object[j].table.block_row;
					m_ReportInfo.sheet[i].object[j]->table.block_col   = m_$ReportInfo.sheet[i].object[j].table.block_col;
                    m_ReportInfo.sheet[i].object[j]->table.visible     = m_$ReportInfo.sheet[i].object[j].table.visible;
					wmemcpy( m_ReportInfo.sheet[i].object[j]->table.data_name, m_$ReportInfo.sheet[i].object[j].table.data_name, TEXT_LEN );
					m_ReportInfo.sheet[i].object[j]->table.area        = m_$ReportInfo.sheet[i].object[j].table.area;
					m_ReportInfo.sheet[i].object[j]->table.cursor_row  = m_$ReportInfo.sheet[i].object[j].table.cursor_row;
					m_ReportInfo.sheet[i].object[j]->table.cursor_col  = m_$ReportInfo.sheet[i].object[j].table.cursor_col;
					m_ReportInfo.sheet[i].object[j]->table.bind        = m_$ReportInfo.sheet[i].object[j].table.bind;
					m_ReportInfo.sheet[i].object[j]->table.anchor_row  = m_$ReportInfo.sheet[i].object[j].table.anchor_row;
					m_ReportInfo.sheet[i].object[j]->table.anchor_col  = m_$ReportInfo.sheet[i].object[j].table.anchor_col;
					m_ReportInfo.sheet[i].object[j]->table.horz_line   = m_$ReportInfo.sheet[i].object[j].table.horz_line;
					m_ReportInfo.sheet[i].object[j]->table.vert_line   = m_$ReportInfo.sheet[i].object[j].table.vert_line;
					m_ReportInfo.sheet[i].object[j]->table.line_expand = m_$ReportInfo.sheet[i].object[j].table.line_expand;
					wmemcpy( &m_ReportInfo.sheet[i].object[j]->table.font, &m_$ReportInfo.sheet[i].object[j].table.font, sizeof( LOGFONT ) );
					m_ReportInfo.sheet[i].object[j]->table.char_cx     = m_$ReportInfo.sheet[i].object[j].table.char_cx;
					m_ReportInfo.sheet[i].object[j]->table.char_cy     = m_$ReportInfo.sheet[i].object[j].table.char_cy;
					m_ReportInfo.sheet[i].object[j]->table.line_max    = m_$ReportInfo.sheet[i].object[j].table.line_max;
					m_ReportInfo.sheet[i].object[j]->table.row_count   = m_$ReportInfo.sheet[i].object[j].table.row_count;
					m_ReportInfo.sheet[i].object[j]->table.col_count   = m_$ReportInfo.sheet[i].object[j].table.col_count;
					m_ReportInfo.sheet[i].object[j]->table.char_len    = ( INT * )malloc( sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					wmemcpy( m_ReportInfo.sheet[i].object[j]->table.char_len, m_$ReportInfo.sheet[i].object[j].table.char_len, sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					m_ReportInfo.sheet[i].object[j]->table.char_max    = ( INT * )malloc( sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					wmemcpy( m_ReportInfo.sheet[i].object[j]->table.char_max, m_$ReportInfo.sheet[i].object[j].table.char_max, sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					m_ReportInfo.sheet[i].object[j]->table.indent_len  = ( INT * )malloc( sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					wmemcpy( m_ReportInfo.sheet[i].object[j]->table.indent_len, m_$ReportInfo.sheet[i].object[j].table.indent_len, sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					m_ReportInfo.sheet[i].object[j]->table.horz_align  = ( INT * )malloc( sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					wmemcpy( m_ReportInfo.sheet[i].object[j]->table.horz_align, m_$ReportInfo.sheet[i].object[j].table.horz_align, sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					m_ReportInfo.sheet[i].object[j]->table.vert_align  = ( INT * )malloc( sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					wmemcpy( m_ReportInfo.sheet[i].object[j]->table.vert_align, m_$ReportInfo.sheet[i].object[j].table.vert_align, sizeof( INT ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					m_ReportInfo.sheet[i].object[j]->table.data_field = ( _TCHAR ** )malloc( sizeof( _TCHAR * ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
					for ( k = 0; k < m_$ReportInfo.sheet[i].object[j].table.col_count; k++ ) {
						if ( NULL == m_$ReportInfo.sheet[i].object[j].table.data_field[k] ) {
							m_ReportInfo.sheet[i].object[j]->table.data_field[k] = NULL;
						} else {
							if ( 0 == StringLength( m_$ReportInfo.sheet[i].object[j].table.data_field[k] ) ) {
								m_ReportInfo.sheet[i].object[j]->table.data_field[k] = NULL;
							} else {
								m_ReportInfo.sheet[i].object[j]->table.data_field[k] = ( _TCHAR * )malloc( StringLength( m_$ReportInfo.sheet[i].object[j].table.data_field[k] ) + sizeof( _TCHAR ) ); // Multi Language
								_tcscpy( m_ReportInfo.sheet[i].object[j]->table.data_field[k], m_$ReportInfo.sheet[i].object[j].table.data_field[k] );
							}
						}
					}
					// CELL ================
					m_ReportInfo.sheet[i].object[j]->table.cell = ( REPORT_CELL ** )malloc( sizeof( REPORT_CELL * ) * m_$ReportInfo.sheet[i].object[j].table.row_count );
					for ( k = 0; k < m_$ReportInfo.sheet[i].object[j].table.row_count; k++ ) {
						m_ReportInfo.sheet[i].object[j]->table.cell[k] = ( REPORT_CELL * )malloc( sizeof( REPORT_CELL ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
						wmemcpy( m_ReportInfo.sheet[i].object[j]->table.cell[k], m_$ReportInfo.sheet[i].object[j].table.cell[k], sizeof( REPORT_CELL ) * m_$ReportInfo.sheet[i].object[j].table.col_count );
						for ( l = 0; l < m_$ReportInfo.sheet[i].object[j].table.col_count; l++ ) {
							if ( NULL == m_$ReportInfo.sheet[i].object[j].table.cell[k][l].text ) {
								m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text = NULL;
							} else {
								if ( 0 == StringLength( m_$ReportInfo.sheet[i].object[j].table.cell[k][l].text ) ) {
									m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text = NULL;
								} else {
									m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text = ( _TCHAR * )malloc( StringLength( m_$ReportInfo.sheet[i].object[j].table.cell[k][l].text ) + sizeof( _TCHAR ) ); // Multi Language
									_tcscpy( m_ReportInfo.sheet[i].object[j]->table.cell[k][l].text, m_$ReportInfo.sheet[i].object[j].table.cell[k][l].text );
								}
							}
						}
					}
					break;
				case OBJECT_GRID :
					if ( 0 == _tcscmp( m_$ReportInfo.sheet[i].object[j].grid.name, _T("") ) ) {
						_stprintf( m_ReportInfo.sheet[i].object[j]->grid.name, _T("그리드_%d"), j );
					} else {
						_tcscpy( m_ReportInfo.sheet[i].object[j]->grid.name, m_$ReportInfo.sheet[i].object[j].grid.name );
					}
					m_ReportInfo.sheet[i].object[j]->grid.left         = m_$ReportInfo.sheet[i].object[j].grid.left;
					m_ReportInfo.sheet[i].object[j]->grid.top          = m_$ReportInfo.sheet[i].object[j].grid.top;
					m_ReportInfo.sheet[i].object[j]->grid.width        = m_$ReportInfo.sheet[i].object[j].grid.width;
					m_ReportInfo.sheet[i].object[j]->grid.height       = m_$ReportInfo.sheet[i].object[j].grid.height;
					_stprintf( m_ReportInfo.sheet[i].object[j]->grid.parent_name, _T("null") );
					m_ReportInfo.sheet[i].object[j]->grid.block_row    = -1;
					m_ReportInfo.sheet[i].object[j]->grid.block_col    = -1;
                    m_ReportInfo.sheet[i].object[j]->grid.visible      = m_$ReportInfo.sheet[i].object[j].grid.visible[0];
					if ( 0 == m_$ReportInfo.sheet[i].object[j].grid.page_row ) {
						m_ReportInfo.sheet[i].object[j]->grid.page_row = -1;
					} else {
						m_ReportInfo.sheet[i].object[j]->grid.page_row = m_$ReportInfo.sheet[i].object[j].grid.page_row;
					}	
					if ( 0 == m_$ReportInfo.sheet[i].object[j].grid.last_row ) {
						m_ReportInfo.sheet[i].object[j]->grid.last_row = -1;
					} else {
						m_ReportInfo.sheet[i].object[j]->grid.last_row = m_$ReportInfo.sheet[i].object[j].grid.last_row;
					}
					m_ReportInfo.sheet[i].object[j]->grid.cursor_row   = m_$ReportInfo.sheet[i].object[j].grid.cursor_row;
					m_ReportInfo.sheet[i].object[j]->grid.cursor_col   = m_$ReportInfo.sheet[i].object[j].grid.cursor_col;
					m_ReportInfo.sheet[i].object[j]->grid.bind         = m_$ReportInfo.sheet[i].object[j].grid.bind;
					m_ReportInfo.sheet[i].object[j]->grid.anchor_row   = m_$ReportInfo.sheet[i].object[j].grid.anchor_row;
					m_ReportInfo.sheet[i].object[j]->grid.anchor_col   = m_$ReportInfo.sheet[i].object[j].grid.anchor_col;
					m_ReportInfo.sheet[i].object[j]->grid.horz_line    = m_$ReportInfo.sheet[i].object[j].grid.horz_line;
					m_ReportInfo.sheet[i].object[j]->grid.vert_line    = m_$ReportInfo.sheet[i].object[j].grid.vert_line;
					m_ReportInfo.sheet[i].object[j]->grid.row_count    = m_$ReportInfo.sheet[i].object[j].grid.row_count;
					m_ReportInfo.sheet[i].object[j]->grid.col_count    = m_$ReportInfo.sheet[i].object[j].grid.col_count;
					// BLOCK ================
					m_ReportInfo.sheet[i].object[j]->grid.block = ( REPORT_BLOCK ** )malloc( sizeof( REPORT_BLOCK * ) * m_$ReportInfo.sheet[i].object[j].grid.row_count );
					for ( k = 0; k < m_$ReportInfo.sheet[i].object[j].grid.row_count; k++ ) {
						m_ReportInfo.sheet[i].object[j]->grid.block[k] = ( REPORT_BLOCK * )malloc( sizeof( REPORT_BLOCK ) * m_$ReportInfo.sheet[i].object[j].grid.col_count );
						for ( l = 0; l < m_$ReportInfo.sheet[i].object[j].grid.col_count; l++ ) {
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].left          = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].left;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].top           = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].top;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].width         = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].width;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].height        = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].height;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].focus         = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].focus;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].fill          = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].fill;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].horz_align    = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].horz_align;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].orientation   = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].orientation;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].slash         = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].slash;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].font          = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].font;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_status   = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].bind_status;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_row      = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].bind_row;
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].bind_col      = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].bind_col;
							if ( 0 > m_$ReportInfo.sheet[i].object[j].grid.block[k][l].origin_width || 5000 < m_$ReportInfo.sheet[i].object[j].grid.block[k][l].origin_width ) {
								if ( 0 > k-1 ) {
									m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_width = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].width;
								} else {
									m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_width = m_$ReportInfo.sheet[i].object[j].grid.block[0][l].width;
								}
							} else {
								m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_width = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].origin_width;
							}
							if ( 0 > m_$ReportInfo.sheet[i].object[j].grid.block[k][l].origin_height || 5000 < m_$ReportInfo.sheet[i].object[j].grid.block[k][l].origin_height ) {
								if ( m_ReportInfo.sheet[i].object[j]->grid.col_count <= l+1 ) {
									m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_height = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].height;
								} else {
									m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_height = m_$ReportInfo.sheet[i].object[j].grid.block[k][l+1].height;
								}
							} else {
								m_ReportInfo.sheet[i].object[j]->grid.block[k][l].origin_height = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].origin_height;
							}
							m_ReportInfo.sheet[i].object[j]->grid.block[k][l].child_count   = m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_count;
							for ( m = 0; m < CHILD_MAX; m++ ) {
								_stprintf( m_ReportInfo.sheet[i].object[j]->grid.block[k][l].child_name[m], _T("null") );
							}
							for ( m = 0; m < m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_count; m++ ) {
								if ( OBJECT_TABLE == m_$ReportInfo.sheet[i].object[j].kind ) {
									if ( 0 == _tcscmp( m_$ReportInfo.sheet[i].object[m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index[m]].table.name, _T("") ) ) {
										_stprintf( child_name, _T("테이블_%d"), m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index[m] );
									} else {
										//GetChildKind( &m_$ReportInfo.sheet[i], m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index[m], child_name );
										_tcscpy( child_name, m_$ReportInfo.sheet[i].object[m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index[m]].table.name );
									}
								} else {
									GetChildKind( &m_$ReportInfo.sheet[i], m_$ReportInfo.sheet[i].object[j].grid.block[k][l].child_index[m], child_name );
								}
								_stprintf( m_ReportInfo.sheet[i].object[j]->grid.block[k][l].child_name[m], child_name );
							}

							if ( NULL == m_$ReportInfo.sheet[i].object[j].grid.block[k][l].text ) {
								m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text = NULL;
							} else {
								if ( 0 == StringLength( m_$ReportInfo.sheet[i].object[j].grid.block[k][l].text ) ) {
									m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text = NULL;
								} else {
									m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text = ( _TCHAR * )malloc( StringLength( m_$ReportInfo.sheet[i].object[j].grid.block[k][l].text ) + sizeof( _TCHAR ) ); // Multi Language
									_tcscpy( m_ReportInfo.sheet[i].object[j]->grid.block[k][l].text, m_$ReportInfo.sheet[i].object[j].grid.block[k][l].text );
								}
							}
						}
					}
					break;
				case OBJECT_HLINE :
					_stprintf( m_ReportInfo.sheet[i].object[j]->hline.name, _T("수평선_%d"), j );
					m_ReportInfo.sheet[i].object[j]->hline.left       = m_$ReportInfo.sheet[i].object[j].hline.left;
					m_ReportInfo.sheet[i].object[j]->hline.top        = m_$ReportInfo.sheet[i].object[j].hline.top;
					m_ReportInfo.sheet[i].object[j]->hline.width      = m_$ReportInfo.sheet[i].object[j].hline.width;
					m_ReportInfo.sheet[i].object[j]->hline.height     = m_$ReportInfo.sheet[i].object[j].hline.height;
					m_ReportInfo.sheet[i].object[j]->hline.block_row  = m_$ReportInfo.sheet[i].object[j].hline.block_row;
					m_ReportInfo.sheet[i].object[j]->hline.block_col  = m_$ReportInfo.sheet[i].object[j].hline.block_col;
                    m_ReportInfo.sheet[i].object[j]->hline.visible    = m_$ReportInfo.sheet[i].object[j].hline.visible;
					m_ReportInfo.sheet[i].object[j]->hline.thick      = 1;
                    m_ReportInfo.sheet[i].object[j]->hline.style      = PS_SOLID;
					if ( -1 != m_$ReportInfo.sheet[i].object[j].hline.parent_index ) {
						_stprintf( m_ReportInfo.sheet[i].object[j]->hline.parent_name, _T("그리드_%d"), m_$ReportInfo.sheet[i].object[j].hline.parent_index );
					} else {
						_tcscpy( m_ReportInfo.sheet[i].object[j]->hline.parent_name, _T("null") );
					}
					break;
				case OBJECT_VLINE :
					_stprintf( m_ReportInfo.sheet[i].object[j]->vline.name, _T("수직선_%d"), j );
					m_ReportInfo.sheet[i].object[j]->vline.left       = m_$ReportInfo.sheet[i].object[j].vline.left;
					m_ReportInfo.sheet[i].object[j]->vline.top        = m_$ReportInfo.sheet[i].object[j].vline.top;
					m_ReportInfo.sheet[i].object[j]->vline.width      = m_$ReportInfo.sheet[i].object[j].vline.width;
					m_ReportInfo.sheet[i].object[j]->vline.height     = m_$ReportInfo.sheet[i].object[j].vline.height;
					m_ReportInfo.sheet[i].object[j]->vline.block_row  = m_$ReportInfo.sheet[i].object[j].vline.block_row;
					m_ReportInfo.sheet[i].object[j]->vline.block_col  = m_$ReportInfo.sheet[i].object[j].vline.block_col;
                    m_ReportInfo.sheet[i].object[j]->vline.visible    = m_$ReportInfo.sheet[i].object[j].vline.visible;
					m_ReportInfo.sheet[i].object[j]->vline.thick      = 1;
                    m_ReportInfo.sheet[i].object[j]->vline.style      = PS_SOLID;
					if ( -1 != m_$ReportInfo.sheet[i].object[j].vline.parent_index ) {
						_stprintf( m_ReportInfo.sheet[i].object[j]->vline.parent_name, _T("그리드_%d"), m_$ReportInfo.sheet[i].object[j].vline.parent_index );
					} else {
						_tcscpy( m_ReportInfo.sheet[i].object[j]->vline.parent_name, _T("null") );
					}
					break;
				case OBJECT_BOX :
					_stprintf( m_ReportInfo.sheet[i].object[j]->box.name, _T("수평선_%d"), j );
					m_ReportInfo.sheet[i].object[j]->box.left       = m_$ReportInfo.sheet[i].object[j].box.left;
					m_ReportInfo.sheet[i].object[j]->box.top        = m_$ReportInfo.sheet[i].object[j].box.top;
					m_ReportInfo.sheet[i].object[j]->box.width      = m_$ReportInfo.sheet[i].object[j].box.width;
					m_ReportInfo.sheet[i].object[j]->box.height     = m_$ReportInfo.sheet[i].object[j].box.height;
					m_ReportInfo.sheet[i].object[j]->box.block_row  = m_$ReportInfo.sheet[i].object[j].box.block_row;
					m_ReportInfo.sheet[i].object[j]->box.block_col  = m_$ReportInfo.sheet[i].object[j].box.block_col;
                    m_ReportInfo.sheet[i].object[j]->box.visible    = m_$ReportInfo.sheet[i].object[j].box.visible;
					m_ReportInfo.sheet[i].object[j]->box.thick      = 1;
                    m_ReportInfo.sheet[i].object[j]->box.style      = PS_SOLID;
					if ( -1 != m_$ReportInfo.sheet[i].object[j].box.parent_index ) {
						_stprintf( m_ReportInfo.sheet[i].object[j]->box.parent_name, _T("그리드_%d"), m_$ReportInfo.sheet[i].object[j].box.parent_index );
					} else {
						_tcscpy( m_ReportInfo.sheet[i].object[j]->box.parent_name, _T("null") );
					}
					break;
			}
		}
	}
ECO_EXCEPTION_END
	
	return 0;
}

INT CWorkSheet::FreeObjectInfo_Ver05( INT SheetIdx, INT ObjectIdx )
{
ECO_EXCEPTION_BEGIN
	INT i, j, k, ParentIdx, BlockRow, BlockCol, ChildIdx;

	switch ( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].kind ) {
		case OBJECT_TABLE :
			if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.char_len ) {
				free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.char_len );
			}
			if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.char_max ) {
				free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.char_max );
			}
			if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.indent_len ) {
				free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.indent_len );
			}
			if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.horz_align ) {
				free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.horz_align );
			}
			if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.vert_align ) {
				free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.vert_align );
			}
			if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.data_field ) {
				for ( j = 0; j < m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.col_count; j++ ) {
					if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.data_field[j] ) {
						free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.data_field[j] );
					}
				}
				free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.data_field );
			}
			for ( j = 0; j < m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.row_count; j++ ) {
				for ( k = 0; k < m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.col_count; k++ ) {
					if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.cell[j][k].text ) {
						free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].table.cell[j][k].text );
					}
				}
			}
			// Clear Parent Infor
			if ( -1 != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].comm.parent_index ) {
				ParentIdx = m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].comm.parent_index;
				BlockRow  = m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].comm.block_row;
				BlockCol  = m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].comm.block_col;

				for ( i = 0; i < m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count; i++ ) {
					if ( ObjectIdx == m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[i] ) {
						if ( i != m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count - 1 ) {
							for ( j = i + 1; j < m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count; j++ ) {
								if ( ObjectIdx < m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[j] ) {
									m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[j]--;
								}
							}
							wmemcpy( &m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[i], &m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[i+1], sizeof( INT ) * ( m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count - i - 1 ) );
						}
						m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count--;
						m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index = ( INT * )realloc( m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index, sizeof( INT ) * m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count );
						break;
					}
				}
			}
			break;
		case OBJECT_GRID  :
   			if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.visible ) {
				m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.visible = NULL;
			}
			for ( j = 0; j < m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.row_count; j++ ) {
				for ( k = 0; k < m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.col_count; k++ ) {
					if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.block[j][k].text ) {
						free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.block[j][k].text );
					}
					// Child Info Clear
					for ( i = 0; i < m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.block[j][k].child_count; i++ ) {
						ChildIdx = m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.block[j][k].child_index[i];
						if ( ChildIdx > m_$ReportInfo.sheet[SheetIdx].object_count - 1 ) {
							continue;
						}
						m_$ReportInfo.sheet[SheetIdx].object[ChildIdx].comm.parent_index = -1;
						m_$ReportInfo.sheet[SheetIdx].object[ChildIdx].comm.block_row    = -1;
						m_$ReportInfo.sheet[SheetIdx].object[ChildIdx].comm.block_col    = -1;
					}
					//
					if ( NULL != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.block[j][k].child_index ) {
						free( m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].grid.block[j][k].child_index );
					}
				}
			}
			break;
		case OBJECT_HLINE :
		case OBJECT_VLINE :
		case OBJECT_BOX   :
			// Clear Parent Infor
			if ( -1 != m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].comm.parent_index ) {
				ParentIdx = m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].comm.parent_index;
				BlockRow  = m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].comm.block_row;
				BlockCol  = m_$ReportInfo.sheet[SheetIdx].object[ObjectIdx].comm.block_col;

				for ( i = 0; i < m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count; i++ ) {
					if ( ObjectIdx == m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[i] ) {
						if ( i != m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count - 1 ) {
							for ( j = i + 1; j < m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count; j++ ) {
								if ( ObjectIdx < m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[j] ) {
									m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[j]--;
								}
							}
							wmemcpy( &m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[i], &m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index[i+1], sizeof( INT ) * ( m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count - i - 1 ) );
						}
						m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count--;
						m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index = ( INT * )realloc( m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_index, sizeof( INT ) * m_$ReportInfo.sheet[SheetIdx].object[ParentIdx].grid.block[BlockRow][BlockCol].child_count );
						break;
					}
				}
			}
			break;
	}
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::FreeObject_Ver05( INT SheetIdx )
{
ECO_EXCEPTION_BEGIN
	INT i;

	if ( NULL == m_$ReportInfo.sheet[SheetIdx].object ) {
		return 0;
	}

	for ( i = 0; i < m_$ReportInfo.sheet[SheetIdx].object_count; i++ ) {
		FreeObjectInfo_Ver05( SheetIdx, i );
	}
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::Free_Ver05( VOID )
{
ECO_EXCEPTION_BEGIN
	INT i;

	if ( NULL != m_$ReportInfo.sheet ) {
		for ( i = 0; i < m_$ReportInfo.sheet_count; i++ ) {
			if ( NULL != m_$ReportInfo.sheet[i].object ) {
				FreeObject_Ver05( i );
			}
		}
		free( m_$ReportInfo.sheet );
	}
ECO_EXCEPTION_END

	return 0;
}

//====================================================================================
// MakeReferencePath
//====================================================================================
INT CWorkSheet::MakeReferencePath( _TCHAR * FileName, _TCHAR * FullFileName )
{
ECO_EXCEPTION_BEGIN
	if ( NULL == FileName || NULL == FullFileName ) {
		return -1;
	}

	//_stprintf( FullFileName, _T("\\통합정보시스템\\IIS_RPT\\%s"), FileName );
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::GetReportInfo( REPORT_INFO ** ReportInfo )
{
ECO_EXCEPTION_BEGIN
	*ReportInfo = &m_ReportInfo;
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::Recovery( VOID )
{
ECO_EXCEPTION_BEGIN
	FILE * fp;

	fp = _tfopen( _T("C:\\report.err"), _T("rb") );
	if ( NULL == fp ) {
		return -1;
	}

	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fp;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
	INT  FileCnt;
	_TCHAR FileName[100], FullFileName[250];

	_ftscanf( fp, _T("%d"), &FileCnt );

	for ( INT i = 0; i < FileCnt; i++ ) {
		_ftscanf( fp, _T("%s"), FileName );

		MakeReferencePath( FileName, FullFileName );

		Load( FullFileName );
		Save( FullFileName );
	}

	fclose( fp );
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::SetFileName( _TCHAR * FileName )
{
ECO_EXCEPTION_BEGIN
	_tcscpy( m_FileName, FileName );
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::GetFileName( _TCHAR * FileName )
{
ECO_EXCEPTION_BEGIN
	_tcscpy( FileName, m_FileName );
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// Store Image
//========================================================================
INT CWorkSheet::StoreImage( VOID )
{
	SaveImage( m_ImageCnt );

	return 0;
}

//========================================================================
// Save Image
//========================================================================
INT CWorkSheet::SaveImage( INT ImageIdx )
{
	INT  ids;
	_TCHAR szImageName[255];

	// 1. IMAGE NAMIMG
	_stprintf( szImageName, _T("%s%d"), IMAGE_NAME, ImageIdx );

	// 2. IMAGE SAVE
	ids = Save( szImageName );
	if ( ids ) {
		return -1;
	}

	return 0;
}

//========================================================================
// Load Image
//========================================================================
INT CWorkSheet::LoadImage( INT ImageIdx )
{
	INT  ids;
	_TCHAR szImageName[255];

	// 1. IMAGE NAMIMG
	_stprintf( szImageName, _T("%s%d"), IMAGE_NAME, ImageIdx );

	// 2. IMAGE SAVE
	ids = Load( szImageName );
	if ( ids ) {
		return -1;
	}

	return 0;
}

//========================================================================
// Delete Image
//========================================================================
INT CWorkSheet::DeleteImage( VOID )
{
	INT  i;
	_TCHAR szImageName[255];

	for ( i = 0; i < m_ImageCnt; i++ ) {
		_stprintf( szImageName, _T("%s%s"), IMAGE_NAME, i );
		DeleteFile( szImageName );
	}

	return 0;
}

//========================================================================
// Undo
//========================================================================
INT CWorkSheet::Undo( CDC * pDC, CSize & Size )
{
	//LoadImage( m_ImageCnt - 1 );

	return 0;
}

//========================================================================
// Redo
//========================================================================
INT CWorkSheet::Redo( CDC * pDC, CSize & Size )
{
	return 0;
}

//========================================================================
// InsertGridRow
//========================================================================
INT CWorkSheet::InsertGridRow( INT  ObjectIdx  ,  // [IN]
                               INT  RowIdx     ,  // [IN]
							   BOOL SpecialRow )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCnt, ColCnt;

	if ( FALSE == SpecialRow && ( RowIdx == m_pSheet->object[ObjectIdx]->grid.page_row || RowIdx == m_pSheet->object[ObjectIdx]->grid.last_row ) ) {
		return 0;
	}

	// 1. Row 메모리를 할당한다.
	m_pSheet->object[ObjectIdx]->grid.row_count++;

	RowCnt = m_pSheet->object[ObjectIdx]->grid.row_count;
	ColCnt = m_pSheet->object[ObjectIdx]->grid.col_count;

	m_pSheet->object[ObjectIdx]->grid.block = ( REPORT_BLOCK ** )realloc( m_pSheet->object[ObjectIdx]->grid.block, sizeof( REPORT_BLOCK * ) * m_pSheet->object[ObjectIdx]->grid.row_count );
	if ( NULL == m_pSheet->object[ObjectIdx]->grid.block ) {
		return -1;
	}

	// 2. 현재Row의 밑부분을 뒤로 민다.
	if ( RowIdx+1 <= RowCnt ) {
		wmemcpy( &m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1], &m_pSheet->object[ObjectIdx]->grid.block[RowIdx], sizeof( REPORT_BLOCK * ) * ( RowCnt - RowIdx - 1 ) );
	}

	// 3. Row의 메모리를 할당한다.
	m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1] = ( REPORT_BLOCK * )malloc( sizeof( REPORT_BLOCK ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1] ) {
		return -1;
	}

	// 5. Row내의 Column정보를 할당한다.
	for ( i = 0; i < ColCnt; i++ ) {
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].left          = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][i].left;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].top           = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][i].top + m_pSheet->object[ObjectIdx]->grid.block[RowIdx][i].height - 1;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].width         = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][i].width;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].height        = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][i].height;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].focus         = FALSE;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].fill          = FALSE;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].horz_align    = ALGIN_HCENTER;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].orientation   = TEXT_ORIENTATION_HORZ;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].slash         = SLASH_NONE;
		wmemcpy( &m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].font, &m_pSheet->object[ObjectIdx]->grid.block[RowIdx][i].font, sizeof( LOGFONT ) );
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].child_count   = 0;
		for ( j = 0; j < CHILD_MAX; j++ ) {
			_tcscpy( m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].child_name[j], _T("null") );
		}
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].text          = NULL;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].bind_status   = BIND_STATUS_NONE;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].bind_row      = -1;
		m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][i].bind_col      = -1;
	}

	// 6. 추가된 Row 밑의 위치를 내린다.
	for ( i = RowIdx + 2; i < RowCnt; i++ ) {
		for ( j = 0 ; j < ColCnt; j++ ) {
			m_pSheet->object[ObjectIdx]->grid.block[i][j].top += ( m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][0].height - 1 );
		}
	}

	// 7. 그리드 전체 높이를 조정한다.
	m_pSheet->object[ObjectIdx]->grid.height += ( m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1][0].height - 1 );

	//8. 페이지행과 마지막행의 위치를 조정한다.
	if ( FALSE == SpecialRow ) {
		if ( -1 != m_pSheet->object[ObjectIdx]->grid.page_row ) {
			m_pSheet->object[ObjectIdx]->grid.page_row++;
		}
		if ( -1 != m_pSheet->object[ObjectIdx]->grid.last_row ) {
			m_pSheet->object[ObjectIdx]->grid.last_row++;
		}
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// InsertGridColumn
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::InsertGridColumn( INT ObjectIdx,  // [IN] 
                                  INT ColIdx   )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCnt, ColCnt;

	// 1. Row 메모리를 할당한다.
	m_pSheet->object[ObjectIdx]->grid.col_count++;

	RowCnt = m_pSheet->object[ObjectIdx]->grid.row_count;
	ColCnt = m_pSheet->object[ObjectIdx]->grid.col_count;

	for ( i = 0; i < RowCnt; i++ ) {
		m_pSheet->object[ObjectIdx]->grid.block[i] = ( REPORT_BLOCK * )realloc( m_pSheet->object[ObjectIdx]->grid.block[i], sizeof( REPORT_BLOCK ) * ColCnt );
		if ( NULL == m_pSheet->object[ObjectIdx]->grid.block[i] ) {
			return -1;
		}
		if ( ColIdx+1 < ColCnt - 1 ) {
			wmemcpy( &m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1], &m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx], sizeof( REPORT_BLOCK ) * ( ColCnt - ColIdx - 1 ) );
		}

		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].left          = m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx].left + m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx].width - 1;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].top           = m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx].top;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].width         = m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx].width;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].height        = m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx].height;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].focus         = FALSE;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].fill          = FALSE;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].horz_align    = ALGIN_HCENTER;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].orientation   = TEXT_ORIENTATION_HORZ;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].slash         = SLASH_NONE;
		wmemcpy( &m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].font, &m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx].font, sizeof( LOGFONT ) );
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].child_count   = 0;
		for ( j = 0 ; j < CHILD_MAX; j++ ) {
			_tcscpy( m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].child_name[j], _T("null") );
		}
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].text          = NULL;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].bind_status   = BIND_STATUS_NONE;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].bind_row      = -1;
		m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1].bind_col      = -1;
	}

	// 5. 추가된 Column 오른쪽의 위치를 조정한다.
	for ( i = 0; i < RowCnt; i++ ) {
		for ( j = ColIdx + 2 ; j < ColCnt; j++ ) {
			m_pSheet->object[ObjectIdx]->grid.block[i][j].left += ( m_pSheet->object[ObjectIdx]->grid.block[0][ColIdx+1].width - 1 );
		}
	}

	// 6. 그리드 전체 높이를 조정한다.
	m_pSheet->object[ObjectIdx]->grid.width += ( m_pSheet->object[ObjectIdx]->grid.block[0][ColIdx+1].width - 1 );
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// RemoveGridRow
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::RemoveGridRow( INT  ObjectIdx  ,  // [IN]
                               INT  RowIdx     ,  // [IN]
							   BOOL SpecialRow )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCnt, ColCnt, TmpHeight, RealRowCnt;

	// 1. Row가 삭제되는 것을 막는 처리
	if ( 1 >= m_pSheet->object[ObjectIdx]->grid.row_count ) {
		return 0;
	}
	if ( FALSE == SpecialRow && ( RowIdx == m_pSheet->object[ObjectIdx]->grid.page_row || RowIdx == m_pSheet->object[ObjectIdx]->grid.last_row ) ) {
		return 0;
	}

	// 2. 현재 Row의 컬럼정보를 Free한다.
	RealRowCnt = m_pSheet->object[ObjectIdx]->grid.row_count;
	if ( 1 >= RealRowCnt ) {
		return 0;
	}

	m_pSheet->object[ObjectIdx]->grid.row_count--;
	RowCnt = m_pSheet->object[ObjectIdx]->grid.row_count;
	ColCnt = m_pSheet->object[ObjectIdx]->grid.col_count;

	TmpHeight = m_pSheet->object[ObjectIdx]->grid.block[RowIdx][0].height;

	for ( i = 0; i < ColCnt; i++ ) {
		if ( NULL != m_pSheet->object[ObjectIdx]->grid.block[RowIdx][i].text ) {
			free( m_pSheet->object[ObjectIdx]->grid.block[RowIdx][i].text );
		}
	}

	// 2. Row정보 메모리를 조정한다.
	free( m_pSheet->object[ObjectIdx]->grid.block[RowIdx] );

	// 3. 삭제할 Row의 아래 Row들을 당긴다.
	if ( RowIdx < RowCnt ) {
		wmemcpy( &m_pSheet->object[ObjectIdx]->grid.block[RowIdx], &m_pSheet->object[ObjectIdx]->grid.block[RowIdx+1], sizeof( REPORT_BLOCK * ) * ( RowCnt - RowIdx ) );
	}

	m_pSheet->object[ObjectIdx]->grid.block = ( REPORT_BLOCK ** )realloc( m_pSheet->object[ObjectIdx]->grid.block, sizeof( REPORT_BLOCK * ) * RowCnt );

	// 4. Row의 위치를 조정한다.
	for ( i = RowIdx; i < RowCnt; i++ ) {
		for ( j = 0 ; j < ColCnt; j++ ) {
			m_pSheet->object[ObjectIdx]->grid.block[i][j].top -= ( TmpHeight - 1 );
		}
	}

	// 5. Grid의 높이를 조정한다.
	m_pSheet->object[ObjectIdx]->grid.height -= ( TmpHeight - 1 );

	// 6. 페이지행과 마지막행의 위치를 조정한다.
	if ( FALSE == SpecialRow ) {
		if ( -1 != m_pSheet->object[ObjectIdx]->grid.page_row ) {
			m_pSheet->object[ObjectIdx]->grid.page_row--;
		}
		if ( -1 != m_pSheet->object[ObjectIdx]->grid.last_row ) {
			m_pSheet->object[ObjectIdx]->grid.last_row--;
		}
	}
	// 7. 커서위치 초기화
	m_pSheet->object[ObjectIdx]->grid.cursor_row = 0;
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// RemoveGridColumn
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::RemoveGridColumn( INT ObjectIdx,  // [IN] 
                                  INT ColIdx   )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCnt, ColCnt, TmpWidth;

	if ( 1 >= m_pSheet->object[ObjectIdx]->grid.col_count ) {
		return 0;
	}

	m_pSheet->object[ObjectIdx]->grid.col_count--;
	RowCnt     = m_pSheet->object[ObjectIdx]->grid.row_count;
	ColCnt     = m_pSheet->object[ObjectIdx]->grid.col_count;
	TmpWidth   = m_pSheet->object[ObjectIdx]->grid.block[0][ColIdx].width;

	// 1. 현재 Row의 컬럼정보를 Free한다.
	for ( i = 0; i < RowCnt; i++ ) {
		if ( NULL != m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx].text ) {
			free( m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx].text );
		}
	}

	// 2. 삭제할 Column의 옆의 Column들을 당긴다.
	if ( ColIdx < ColCnt ) {
		for ( i = 0; i < RowCnt; i++ ) {
			wmemcpy( &m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx], &m_pSheet->object[ObjectIdx]->grid.block[i][ColIdx+1], sizeof( REPORT_BLOCK ) * ( ColCnt - ColIdx ) );
		}
	}

	// 3. Row내의 Column정보 메모리를 조정한다.
	for ( i = 0; i < RowCnt; i++ ) {
		m_pSheet->object[ObjectIdx]->grid.block[i] = ( REPORT_BLOCK * )realloc( m_pSheet->object[ObjectIdx]->grid.block[i], sizeof( REPORT_BLOCK ) * ColCnt );
	}
	
	// 4. Row의 위치를 조정한다.
	for ( i = 0; i < RowCnt; i++ ) {
		for ( j = ColIdx; j < ColCnt; j++ ) {
			m_pSheet->object[ObjectIdx]->grid.block[i][j].left -= ( TmpWidth - 1 );
		}
	}

	// 5. Grid의 너비를 조정한다.
	m_pSheet->object[ObjectIdx]->grid.width -= ( TmpWidth - 1 );

	// 6. 
	m_pSheet->object[ObjectIdx]->grid.cursor_col = 0;
ECO_EXCEPTION_END
	
	return 0;
}

//========================================================================
// ToggleGridPageRow
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::ToggleGridPageRow( INT ObjectIdx )
{
ECO_EXCEPTION_BEGIN
	if ( 0 < m_pSheet->object[ObjectIdx]->grid.page_row ) {
		RemoveGridPageRow( ObjectIdx );
	} else {
		InsertGridPageRow( ObjectIdx );
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// ToggleGridLastRow
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::ToggleGridLastRow( INT ObjectIdx )
{
ECO_EXCEPTION_BEGIN
	if ( 0 < m_pSheet->object[ObjectIdx]->grid.last_row ) {
		RemoveGridLastRow( ObjectIdx );
	} else {
		InsertGridLastRow( ObjectIdx );
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// InsertGridPageRow
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::InsertGridPageRow( INT ObjectIdx )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT   i, j, ColIdx, RowIdx;
	INT   TempWidht, TempHeight;
	CRect Rect;

	//====================================================================
	// 1. Row 를 추가한다.
	//====================================================================
	if ( 0 < m_pSheet->object[ObjectIdx]->grid.last_row ) {
		RowIdx = m_pSheet->object[ObjectIdx]->grid.last_row;
	} else {
		RowIdx = m_pSheet->object[ObjectIdx]->grid.row_count;
	}
	ColIdx = m_pSheet->object[ObjectIdx]->grid.col_count - 1;

	InsertGridRow( ObjectIdx, RowIdx - 1, TRUE );

	m_pSheet->object[ObjectIdx]->grid.page_row = RowIdx;

	if ( 0 < m_pSheet->object[ObjectIdx]->grid.last_row ) {
		m_pSheet->object[ObjectIdx]->grid.last_row++;
	}
	//====================================================================
	// 3. 바인딩 영역을 설정한다.
	//====================================================================
	Rect.SetRect( 0, RowIdx, ColIdx, RowIdx );
			  
	//====================================================================
	// 4. 바인딩 작업을 한다.
	//====================================================================
	for ( i = Rect.top; i <= Rect.bottom; i++ ) {
		for ( j = Rect.left; j <= Rect.right; j++ ) {
			if ( i == Rect.top && j == Rect.left ) {
				TempWidht  = m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].left - m_pSheet->object[ObjectIdx]->grid.block[i][j].left + m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].width;
				TempHeight = m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].top  - m_pSheet->object[ObjectIdx]->grid.block[i][j].top  + m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].height;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_width  = m_pSheet->object[ObjectIdx]->grid.block[i][j].width;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_height = m_pSheet->object[ObjectIdx]->grid.block[i][j].height;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].width         = TempWidht;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].height        = TempHeight; 
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_status   = BIND_STATUS_MASTER;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_row      = Rect.bottom;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_col      = Rect.right;
			} else {
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_status   = BIND_STATUS_SLAVE;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_row      = Rect.top;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_col      = Rect.left;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_width  = m_pSheet->object[ObjectIdx]->grid.block[i][j].width;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_height = m_pSheet->object[ObjectIdx]->grid.block[i][j].height;
			}
		}
		//m_pSheet->object[ObjectIdx]->grid.block[i][0].text = ( _TCHAR *)malloc( StringLength( _T("page") ) + sizeof( _TCHAR ) ); // Multi Language
		//_tcscpy( m_pSheet->object[ObjectIdx]->grid.block[i][0].text, _T("page") );
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// InsertGridLastRow
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::InsertGridLastRow( INT ObjectIdx )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT   i, j, ColIdx, RowIdx;
	INT   TempWidht, TempHeight;
	CRect Rect;

	//====================================================================
	// 1. Row 를 추가한다.
	//====================================================================
	RowIdx = m_pSheet->object[ObjectIdx]->grid.row_count;
	ColIdx = m_pSheet->object[ObjectIdx]->grid.col_count - 1;

	InsertGridRow( ObjectIdx, RowIdx - 1, TRUE );

	m_pSheet->object[ObjectIdx]->grid.last_row = RowIdx;
	//====================================================================
	// 3. 바인딩 영역을 설정한다.
	//====================================================================
	Rect.SetRect( 0, RowIdx, ColIdx, RowIdx );
			  
	//====================================================================
	// 4. 바인딩 작업을 한다.
	//====================================================================
	for ( i = Rect.top; i <= Rect.bottom; i++ ) {
		for ( j = Rect.left; j <= Rect.right; j++ ) {
			if ( i == Rect.top && j == Rect.left ) {
				TempWidht  = m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].left - m_pSheet->object[ObjectIdx]->grid.block[i][j].left + m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].width;
				TempHeight = m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].top  - m_pSheet->object[ObjectIdx]->grid.block[i][j].top  + m_pSheet->object[ObjectIdx]->grid.block[Rect.bottom][Rect.right].height;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_width  = m_pSheet->object[ObjectIdx]->grid.block[i][j].width;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_height = m_pSheet->object[ObjectIdx]->grid.block[i][j].height;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].width         = TempWidht;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].height        = TempHeight; 
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_status   = BIND_STATUS_MASTER;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_row      = Rect.bottom;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_col      = Rect.right;
			} else {
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_status   = BIND_STATUS_SLAVE;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_row      = Rect.top;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].bind_col      = Rect.left;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_width  = m_pSheet->object[ObjectIdx]->grid.block[i][j].width;
				m_pSheet->object[ObjectIdx]->grid.block[i][j].origin_height = m_pSheet->object[ObjectIdx]->grid.block[i][j].height;
			}
		}
		//m_pSheet->object[ObjectIdx]->grid.block[i][0].text = ( _TCHAR *)malloc( StringLength( _T("last") ) + sizeof( _TCHAR ) ); // Multi Language
		//_tcscpy( m_pSheet->object[ObjectIdx]->grid.block[i][0].text, _T("last") );
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// RemoveGridPageRow
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::RemoveGridPageRow( INT ObjectIdx )  // [IN]
{
ECO_EXCEPTION_BEGIN

	RemoveGridRow( ObjectIdx, m_pSheet->object[ObjectIdx]->grid.page_row, TRUE );
	m_pSheet->object[ObjectIdx]->grid.page_row = -1;
	if ( -1 != m_pSheet->object[ObjectIdx]->grid.last_row ) {
		m_pSheet->object[ObjectIdx]->grid.last_row--;
	}
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// RemoveGridLastRow
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::RemoveGridLastRow( INT ObjectIdx )  // [IN]
{
ECO_EXCEPTION_BEGIN
	RemoveGridRow( ObjectIdx, m_pSheet->object[ObjectIdx]->grid.last_row, TRUE );
	m_pSheet->object[ObjectIdx]->grid.last_row = -1;
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// InsertTableRow
//========================================================================
INT CWorkSheet::InsertTableRow( INT ObjectIdx,  // [IN]
							    INT RowIdx   )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCnt, ColCnt;

	// 1. Row 메모리를 할당한다.
	m_pSheet->object[ObjectIdx]->table.row_count++;

	RowCnt = m_pSheet->object[ObjectIdx]->table.row_count;
	ColCnt = m_pSheet->object[ObjectIdx]->table.col_count;

	m_pSheet->object[ObjectIdx]->table.cell = ( REPORT_CELL ** )realloc( m_pSheet->object[ObjectIdx]->table.cell, sizeof( REPORT_CELL * ) * m_pSheet->object[ObjectIdx]->table.row_count );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.cell ) {
		return -1;
	}

	// 2. 현재Row의 밑부분을 뒤로 민다.
	if ( RowIdx+1 <= RowCnt ) {
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1], &m_pSheet->object[ObjectIdx]->table.cell[RowIdx], sizeof( REPORT_CELL * ) * ( RowCnt - RowIdx - 1 ) );
	}

	// 3. Row의 메모리를 할당한다.
	m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1] = ( REPORT_CELL * )malloc( sizeof( REPORT_CELL ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1] ) {
		return -1;
	}

	// 5. Row내의 Column정보를 할당한다.
	for ( i = 0; i < ColCnt; i++ ) {
		m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][i].left   = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][i].left;
		m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][i].top    = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][i].top + m_pSheet->object[ObjectIdx]->table.cell[RowIdx][i].height - 1;
		m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][i].width  = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][i].width;
		m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][i].height = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][i].height;
		m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][i].focus  = FALSE;
		m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][i].fill   = FALSE;
		m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][i].text   = NULL;
	}

	// 6. 추가된 Row 밑의 위치를 내린다.
	for ( i = RowIdx + 2; i < RowCnt; i++ ) {
		for ( j = 0 ; j < ColCnt; j++ ) {
			m_pSheet->object[ObjectIdx]->table.cell[i][j].top += ( m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][0].height - 1 );
		}
	}

	// 7. 그리드 전체 높이를 조정한다.
	m_pSheet->object[ObjectIdx]->table.height += ( m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1][0].height - 1 );
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// RemoveTableRow
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::RemoveTableRow( INT ObjectIdx,  // [IN] 
							    INT RowIdx   )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCnt, ColCnt, TmpHeight, RealRowCnt;

	// 1. Row가 삭제되는 것을 막는 처리
	if ( 1 >= m_pSheet->object[ObjectIdx]->table.row_count ) {
		return 0;
	}

	// 2. 현재 Row의 컬럼정보를 Free한다.
	RealRowCnt = m_pSheet->object[ObjectIdx]->table.row_count;
	if ( 1 >= RealRowCnt ) {
		return 0;
	}

	m_pSheet->object[ObjectIdx]->table.row_count--;
	RowCnt = m_pSheet->object[ObjectIdx]->table.row_count;
	ColCnt = m_pSheet->object[ObjectIdx]->table.col_count;

	TmpHeight = m_pSheet->object[ObjectIdx]->table.cell[RowIdx][0].height;

	for ( i = 0; i < ColCnt; i++ ) {
		if ( NULL != m_pSheet->object[ObjectIdx]->table.cell[RowIdx][i].text ) {
			free( m_pSheet->object[ObjectIdx]->table.cell[RowIdx][i].text );
		}
	}

	// 2. Row정보 메모리를 조정한다.
	free( m_pSheet->object[ObjectIdx]->table.cell[RowIdx] );

	// 3. 삭제할 Row의 아래 Row들을 당긴다.
	if ( RowIdx < RowCnt ) {
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.cell[RowIdx], &m_pSheet->object[ObjectIdx]->table.cell[RowIdx+1], sizeof( REPORT_CELL * ) * ( RowCnt - RowIdx ) );
	}

	m_pSheet->object[ObjectIdx]->table.cell = ( REPORT_CELL ** )realloc( m_pSheet->object[ObjectIdx]->table.cell, sizeof( REPORT_CELL * ) * RowCnt );

	// 4. Row의 위치를 조정한다.
	for ( i = RowIdx; i < RowCnt; i++ ) {
		for ( j = 0 ; j < ColCnt; j++ ) {
			m_pSheet->object[ObjectIdx]->table.cell[i][j].top -= ( TmpHeight - 1 );
		}
	}

	// 5. Grid의 높이를 조정한다.
	m_pSheet->object[ObjectIdx]->table.height -= ( TmpHeight - 1 );

	// 7. 커서위치 초기화
	m_pSheet->object[ObjectIdx]->table.cursor_row = 0;
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// InsertTableColumn
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::InsertTableColumn( INT ObjectIdx,  // [IN] 
								   INT ColIdx   )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCnt, ColCnt;

	// 1. Row 메모리를 할당한다.
	m_pSheet->object[ObjectIdx]->table.col_count++;

	RowCnt = m_pSheet->object[ObjectIdx]->table.row_count;
	ColCnt = m_pSheet->object[ObjectIdx]->table.col_count;

	m_pSheet->object[ObjectIdx]->table.char_len = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.char_len, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.char_len ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.char_max = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.char_max, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.char_max ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.indent_len = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.indent_len, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.indent_len ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.horz_align = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.horz_align, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.horz_align ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.vert_align = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.vert_align, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.vert_align ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.data_field = ( _TCHAR ** )realloc( m_pSheet->object[ObjectIdx]->table.data_field, sizeof( _TCHAR * ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.data_field ) {
		return -1;
	}

	if ( ColIdx+1 < ColCnt - 1 ) {
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.char_len  [ColIdx+1], &m_pSheet->object[ObjectIdx]->table.char_len  [ColIdx], sizeof( INT    ) * ( ColCnt - ColIdx - 1 ) );
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.char_max  [ColIdx+1], &m_pSheet->object[ObjectIdx]->table.char_max  [ColIdx], sizeof( INT    ) * ( ColCnt - ColIdx - 1 ) );
        wmemcpy( &m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx+1], &m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx], sizeof( INT    ) * ( ColCnt - ColIdx - 1 ) );
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.horz_align[ColIdx+1], &m_pSheet->object[ObjectIdx]->table.horz_align[ColIdx], sizeof( INT    ) * ( ColCnt - ColIdx - 1 ) );
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.vert_align[ColIdx+1], &m_pSheet->object[ObjectIdx]->table.vert_align[ColIdx], sizeof( INT    ) * ( ColCnt - ColIdx - 1 ) );
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.data_field[ColIdx+1], &m_pSheet->object[ObjectIdx]->table.data_field[ColIdx], sizeof( _TCHAR * ) * ( ColCnt - ColIdx - 1 ) );
	}

	m_pSheet->object[ObjectIdx]->table.char_len  [ColIdx+1] = m_pSheet->object[ObjectIdx]->table.char_len  [ColIdx];
	m_pSheet->object[ObjectIdx]->table.char_max  [ColIdx+1] = m_pSheet->object[ObjectIdx]->table.char_max  [ColIdx];
	m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx+1] = m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx];
	m_pSheet->object[ObjectIdx]->table.horz_align[ColIdx+1] = m_pSheet->object[ObjectIdx]->table.horz_align[ColIdx];
	m_pSheet->object[ObjectIdx]->table.vert_align[ColIdx+1] = m_pSheet->object[ObjectIdx]->table.vert_align[ColIdx];
	m_pSheet->object[ObjectIdx]->table.data_field[ColIdx+1] = NULL;

	for ( i = 0; i < RowCnt; i++ ) {
		m_pSheet->object[ObjectIdx]->table.cell[i] = ( REPORT_CELL * )realloc( m_pSheet->object[ObjectIdx]->table.cell[i], sizeof( REPORT_CELL ) * ColCnt );
		if ( NULL == m_pSheet->object[ObjectIdx]->table.cell[i] ) {
			return -1;
		}

		if ( ColIdx+1 < ColCnt - 1 ) {
			wmemcpy( &m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1], &m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx], sizeof( REPORT_CELL ) * ( ColCnt - ColIdx - 1 ) );
		}

		m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1].left  = m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx].left + m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx].width - 1;
		m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1].top   = m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx].top;
		m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1].width = m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx].width;
		m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1].height= m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx].height;
		m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1].focus = FALSE;
		m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1].fill  = FALSE;
		m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1].text  = NULL;
	}

	// 5. 추가된 Column 오른쪽의 위치를 조정한다.
	for ( i = 0; i < RowCnt; i++ ) {
		for ( j = ColIdx + 2 ; j < ColCnt; j++ ) {
			m_pSheet->object[ObjectIdx]->table.cell[i][j].left += ( m_pSheet->object[ObjectIdx]->table.cell[0][ColIdx+1].width - 1 );
		}
	}

	// 6. 그리드 전체 높이를 조정한다.
	m_pSheet->object[ObjectIdx]->grid.width += ( m_pSheet->object[ObjectIdx]->table.cell[0][ColIdx+1].width - 1 );
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// RemoveTableColumn
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::RemoveTableColumn(INT ObjectIdx,  // [IN] 
								  INT ColIdx   )  // [IN]
{
ECO_EXCEPTION_BEGIN
	INT i, j, RowCnt, ColCnt, TmpWidth;

	if ( 1 >= m_pSheet->object[ObjectIdx]->table.col_count ) {
		return 0;
	}

	m_pSheet->object[ObjectIdx]->table.col_count--;
	RowCnt     = m_pSheet->object[ObjectIdx]->table.row_count;
	ColCnt     = m_pSheet->object[ObjectIdx]->table.col_count;
	TmpWidth   = m_pSheet->object[ObjectIdx]->table.cell[0][ColIdx].width;

	if ( NULL != m_pSheet->object[ObjectIdx]->table.data_field[ColIdx] ) {
		free( m_pSheet->object[ObjectIdx]->table.data_field[ColIdx] );
	}

	if ( ColIdx < ColCnt ) {
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.char_len  [ColIdx], &m_pSheet->object[ObjectIdx]->table.char_len  [ColIdx+1], sizeof( INT    ) * ( ColCnt - ColIdx ) );
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.char_max  [ColIdx], &m_pSheet->object[ObjectIdx]->table.char_max  [ColIdx+1], sizeof( INT    ) * ( ColCnt - ColIdx ) );
        wmemcpy( &m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx], &m_pSheet->object[ObjectIdx]->table.indent_len[ColIdx+1], sizeof( INT    ) * ( ColCnt - ColIdx ) );
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.horz_align[ColIdx], &m_pSheet->object[ObjectIdx]->table.horz_align[ColIdx+1], sizeof( INT    ) * ( ColCnt - ColIdx ) );
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.vert_align[ColIdx], &m_pSheet->object[ObjectIdx]->table.vert_align[ColIdx+1], sizeof( INT    ) * ( ColCnt - ColIdx ) );
		wmemcpy( &m_pSheet->object[ObjectIdx]->table.data_field[ColIdx], &m_pSheet->object[ObjectIdx]->table.data_field[ColIdx+1], sizeof( _TCHAR * ) * ( ColCnt - ColIdx ) );
	}

	m_pSheet->object[ObjectIdx]->table.char_len = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.char_len, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.char_len ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.char_max = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.char_max, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.char_max ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.indent_len = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.indent_len, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.indent_len ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.horz_align = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.horz_align, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.horz_align ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.vert_align = ( INT * )realloc( m_pSheet->object[ObjectIdx]->table.vert_align, sizeof( INT ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.vert_align ) {
		return -1;
	}
	m_pSheet->object[ObjectIdx]->table.data_field = ( _TCHAR ** )realloc( m_pSheet->object[ObjectIdx]->table.data_field, sizeof( _TCHAR * ) * ColCnt );
	if ( NULL == m_pSheet->object[ObjectIdx]->table.data_field ) {
		return -1;
	}

	// 1. 현재 Row의 컬럼정보를 Free한다.
	for ( i = 0; i < RowCnt; i++ ) {
		if ( NULL != m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx].text ) {
			free( m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx].text );
		}
	}

	// 2. 삭제할 Column의 옆의 Column들을 당긴다.
	if ( ColIdx < ColCnt ) {
		for ( i = 0; i < RowCnt; i++ ) {
			wmemcpy( &m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx], &m_pSheet->object[ObjectIdx]->table.cell[i][ColIdx+1], sizeof( REPORT_CELL ) * ( ColCnt - ColIdx ) );
		}
	}

	// 3. Row내의 Column정보 메모리를 조정한다.
	for ( i = 0; i < RowCnt; i++ ) {
		m_pSheet->object[ObjectIdx]->table.cell[i] = ( REPORT_CELL * )realloc( m_pSheet->object[ObjectIdx]->table.cell[i], sizeof( REPORT_CELL ) * ColCnt );
	}
	
	// 4. Row의 위치를 조정한다.
	for ( i = 0; i < RowCnt; i++ ) {
		for ( j = ColIdx; j < ColCnt; j++ ) {
			m_pSheet->object[ObjectIdx]->table.cell[i][j].left -= ( TmpWidth - 1 );
		}
	}

	// 5. Grid의 너비를 조정한다.
	m_pSheet->object[ObjectIdx]->table.width -= ( TmpWidth - 1 );

	// 6. 커서초기화
	m_pSheet->object[ObjectIdx]->table.cursor_col = 0;
ECO_EXCEPTION_END

	return 0;
}

//========================================================================
// GetParentInfo
//
// 목적 : 부모정보를 얻는다.
// 작성 :
//========================================================================
INT CWorkSheet::GetParentInfo( REPORT_SHEET * pSheet, INT ObjectIdx, INT * ParentIdx, INT * RowIdx, INT * ColIdx )
{
ECO_EXCEPTION_BEGIN
	INT i;

	for ( i = 0; i < pSheet->object_count; i++ ) {
		if ( 0 == _tcsicmp( pSheet->object[i]->comm.name, pSheet->object[ObjectIdx]->comm.parent_name ) ) {
			*ParentIdx = i;
			*RowIdx = pSheet->object[ObjectIdx]->comm.block_row;
			*ColIdx = pSheet->object[ObjectIdx]->comm.block_col;
			return TRUE;
		}
	}
ECO_EXCEPTION_END

	return FALSE;
}

//========================================================================
// ClearParentInfo
//
// 목적 : 부모정보를 제거한다.
// 작성 :
//========================================================================
INT CWorkSheet::ClearChildInfoOfParent( REPORT_SHEET * pSheet, INT ObjectIdx )
{
ECO_EXCEPTION_BEGIN
	INT i, j, ParentIdx, BlockRow, BlockCol;

	if ( TRUE == GetParentInfo( pSheet, ObjectIdx, &ParentIdx, &BlockRow, &BlockCol ) ) {
		for ( i = 0; i < pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_count; i++ ) {
			if ( 0 == _tcsicmp( pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_name[i], pSheet->object[ObjectIdx]->comm.name ) ) {
				// 자식객체 이름 메모리를 조정한다.
				wmemcpy( pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_name[i], pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_name[i+1], TEXT_LEN * ( pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_count - i - 1 ) );
				// 사용하지 않는 부분을 초기화한다.
				for ( j = pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_count; j < CHILD_MAX; j++ ) {
					_tcscpy( pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_name[j], _T("null") );
				}
				// 자식개수를 줄인다.
				pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_count--;
				break;
			}
		}
	}
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::ClearParentInfoOfChild( REPORT_SHEET * pSheet, INT ObjectIdx, INT BlockRow, INT BlockCol )
{
ECO_EXCEPTION_BEGIN
	INT i, j;

	// 자식의 부모정보를 Clear한다.
	for ( i = 0; i < pSheet->object[ObjectIdx]->grid.block[BlockRow][BlockCol].child_count; i++ ) {
		for (  j = 0; j < pSheet->object_count; j++ ) { 
			if ( 0 == _tcsicmp( pSheet->object[j]->comm.name, pSheet->object[ObjectIdx]->grid.block[BlockRow][BlockCol].child_name[i] ) ) {
				_tcscpy( pSheet->object[j]->comm.parent_name, _T("null") );
				pSheet->object[j]->comm.block_row = -1;
				pSheet->object[j]->comm.block_col = -1;
				return TRUE;
			}
		}
	}
ECO_EXCEPTION_END

	return FALSE;
}

INT CWorkSheet::AddChildInfo( INT ParentIdx, INT ChildIdx, INT BlockRow, INT BlockCol )
{
ECO_EXCEPTION_BEGIN
	INT TmpChildIdx;

	m_pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_count++;
	TmpChildIdx = m_pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_count - 1;
	_tcscpy( m_pSheet->object[ParentIdx]->grid.block[BlockRow][BlockCol].child_name[TmpChildIdx], m_pSheet->object[ChildIdx]->comm.name );
ECO_EXCEPTION_END

	return 0;
}

INT CWorkSheet::SetParentInfo( INT ChildIdx, INT ParentIdx, INT BlockRow, INT BlockCol )
{
ECO_EXCEPTION_BEGIN
	_tcscpy( m_pSheet->object[ChildIdx]->comm.parent_name, m_pSheet->object[ParentIdx]->comm.name );
	m_pSheet->object[ChildIdx]->comm.block_row = BlockRow;
	m_pSheet->object[ChildIdx]->comm.block_col = BlockCol;
ECO_EXCEPTION_END

	return 0;
}

//====================================================================================
// NamingObject
//
// PURPOSE : 객체이름을 생성한다.
// AUTHOR  : 우상옥
//====================================================================================
_TCHAR * CWorkSheet::NamingObject( INT ObjectKind )
{
	static _TCHAR szObjectName[TEXT_LEN];

ECO_EXCEPTION_BEGIN
	switch ( ObjectKind ) {
		case OBJECT_GRID    : _stprintf( szObjectName, _T("그리드_%d"), m_pSheet->object_id ); break;
		case OBJECT_TABLE   : _stprintf( szObjectName, _T("테이블_%d"), m_pSheet->object_id ); break;
		case OBJECT_HLINE   : _stprintf( szObjectName, _T("수평선_%d"), m_pSheet->object_id ); break;
		case OBJECT_VLINE   : _stprintf( szObjectName, _T("수직선_%d"), m_pSheet->object_id ); break;
		case OBJECT_BOX     : _stprintf( szObjectName, _T("상자_%d")  , m_pSheet->object_id ); break;
		case OBJECT_BITMAP  : _stprintf( szObjectName, _T("비트맵_%d"), m_pSheet->object_id ); break;
		case OBJECT_BARCODE : _stprintf( szObjectName, _T("바코드_%d"), m_pSheet->object_id ); break;
	}
ECO_EXCEPTION_END
	
	return szObjectName;
}

//========================================================================
// IncrNamimg
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::IncrNamimg( VOID )
{
	m_pSheet->object_id++;

	return 0;
}

//========================================================================
// IsExistObjectName
//
// 목적 :
// 작성 :
//========================================================================
INT CWorkSheet::IsExistObjectName( INT ObjectIdx, _TCHAR * ObjectName )
{
ECO_EXCEPTION_BEGIN
	INT i;

	for ( i = 0; i < m_pSheet->object_count; i++ ) {
		if ( ObjectIdx == i ) {
			continue;
		}
		if ( 0 == _tcsicmp( ObjectName, m_pSheet->object[i]->comm.name ) ) {
			return TRUE;
		}
	}
ECO_EXCEPTION_END

	return FALSE;
}

INT CWorkSheet::EcoRectangle( CDC * pDC, INT left, INT top, INT right, INT bottom )
{
ECO_EXCEPTION_BEGIN
	if ( TRUE == pDC->m_bPrinting && FALSE == m_bPreview ) {
		pDC->Rectangle( left, top, right - 1, bottom - 1 );
	} else {
		pDC->Rectangle( left, top, right, bottom );
	}
ECO_EXCEPTION_END

	return 0;	
}

INT CWorkSheet::SetPreviewMode( BOOL Flag )
{
	m_bPreview = Flag;

	return 0;
}

INT CWorkSheet::StringLength( TCHAR * str )
{
	if (str == NULL) return 0;
#ifdef _UNICODE
	return _tcsclen(str);			// Unicode 인경우 스트링의 길이를 구한다.
#elif _MBCS
	return _tcsclen(str);				// MBCS인 경우 스트링의 길이를 구한다.
#endif
}

