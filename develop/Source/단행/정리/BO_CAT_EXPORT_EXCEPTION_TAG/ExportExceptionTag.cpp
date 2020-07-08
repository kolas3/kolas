 ExportExceptionTag.cpp : implementation file


#include "stdafx.h"
#include "ExportExceptionTag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
//CExportExceptionTag dialog


CExportExceptionTag::CExportExceptionTag(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportExceptionTag)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pGrid = NULL;
	m_pEditCtrl = NULL;

	m_strFilePath = _T("..\\CFG\\ExceptionTag.CFG");
}

CExportExceptionTag::~CExportExceptionTag()
{
EFS_BEGIN

	if ( m_pCM != NULL) 
		m_pCM = NULL;

	if ( m_pGrid != NULL) 
		m_pGrid = NULL;

	if ( m_pEditCtrl != NULL) 
		m_pEditCtrl = NULL;


EFS_END
}


VOID CExportExceptionTag::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportExceptionTag)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportExceptionTag, CDialog)
	//{{AFX_MSG_MAP(CExportExceptionTag)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_EN_KILLFOCUS(IDC_EDIT_BOX, OnKillfocusEditCallno)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
//CExportExceptionTag message handlers

VOID CExportExceptionTag::OnbSAVE() 
{
EFS_BEGIN

	if ( !SaveToFile() ) 
	{
		AfxMessageBox(_T("파일로 저장중 에러가 발생하였습니다"));
		return;
	}

	AfxMessageBox(_T("정상적으로 저장되었습니다."));
	return;

EFS_END
}

VOID CExportExceptionTag::OnbCLOSE() 
{
	OnCancel();	
}

BOOL CExportExceptionTag::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	if (InitESL_Mgr(_T("SM_정리_마크반출_제외TAG")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_정리_마크반출_제외TAG"));
	if (m_pCM == NULL) 
	{
		AfxMessageBox(_T("CM Init Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pGrid = reinterpret_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID")));
	
	m_pEditCtrl = new CEdit();
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN, CRect(0, 0, 0, 0), this, IDC_EDIT_BOX);
	m_pEditCtrl->ShowWindow(SW_HIDE);
	
	//그리드 정보 초기화
	InitGrid();	

	SetGridIndexNo();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CExportExceptionTag, CDialog)
    //{{AFX_EVENTSINK_MAP(CExportExceptionTag)
	ON_EVENT(CExportExceptionTag, IDC_GRID, -601 /* DblClick */, OnDblClickGrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CExportExceptionTag::ShowControl()
{

EFS_BEGIN

	INT nCol = m_pGrid->GetCol();
	INT nRow = m_pGrid->GetRow();

	CString sOldGirdData = m_pGrid->GetTextMatrix(nRow, nCol);

	CString strText;
	strText.Format(_T("%s"), m_pGrid->GetTextMatrix(0, nCol));

	if ( strText != _T("TAG 이름") ) return ;


	// 콤보가 아니면	
	// 에디트 박스를 보여줄것
	ShowEditBox(m_pGrid);

	return ;

EFS_END
return ;

}
VOID CExportExceptionTag::ShowEditBox(CESL_Grid *pGrid)
{
EFS_BEGIN

	m_pEditCtrl->GetSafeHwnd();

	if (pGrid == NULL) return ;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();

	if (nRow < 1 || nCol < 1) return ;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// 텍스트 설정
	m_pEditCtrl->SetWindowText(pGrid->GetTextMatrix(nRow, nCol));

	m_pEditCtrl->MoveWindow(rcCell);
	m_pEditCtrl->ShowWindow(SW_SHOW);
	m_pEditCtrl->SetFocus();
	m_pEditCtrl->BringWindowToTop();

	m_nSelectedGridRow = nRow;
	m_nSelectedGridCol = nCol;

	return ;

	EFS_END
	return ;
}

VOID CExportExceptionTag::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
	EFS_BEGIN

	if (pGrid == NULL) return ;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return ;

	EFS_END
	return ;
}

VOID CExportExceptionTag::OnKillfocusEditCallno() 
{
EFS_BEGIN

	SetFocus();

	if (m_pEditCtrl == NULL) return ;
	
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return ;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			
	CString strData;
	m_pEditCtrl->GetWindowText(strData);

	ApplyDataChange(strData);

	return ;	
EFS_END
}

INT CExportExceptionTag::ApplyDataChange(LPCTSTR lpszData)
{
	EFS_BEGIN

	if (lpszData == NULL) return -1;
	if ( m_pGrid == NULL) return -1;

	// 그리드에 변경을 적용시킨다.
	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(m_nSelectedGridRow, m_nSelectedGridCol, lpszData);

	if ( m_pGrid->GetRows()-1 == m_nSelectedGridRow  && m_pGrid->GetTextMatrix( m_nSelectedGridRow , 1 ) != _T("") ) 
		m_pGrid->SetRows( m_nSelectedGridRow + 2 );


	SetGridIndexNo();

	return 0;

	EFS_END
	return -1;
}

VOID CExportExceptionTag::OnDblClickGrid() 
{
EFS_BEGIN
	if (m_pGrid->GetMouseRow() == 0) return;
	ShowControl();
EFS_END	
}

BOOL CExportExceptionTag::SaveToFile()
{
EFS_BEGIN

	// 파일로 저장
	FILE *file;
	file = _tfopen( m_strFilePath , _T("wb") ); 
	if ( file == NULL )
	{
		AfxMessageBox(_T("File Open 확인"));
		return FALSE;
	}

		/*UNCHANGE*/fputc(0xFF, file);
		/*UNCHANGE*/fputc(0xFE, file);
		
	CString strGridData = _T("");
	for ( INT nGridRowIdx = 0 ; nGridRowIdx < m_pGrid->GetRow(); nGridRowIdx++ )
	{
		strGridData += m_pGrid->GetTextMatrix( nGridRowIdx + 1 , 1);
		strGridData += _T(";");
	}

	_ftprintf( file, strGridData );
	fclose(file);

	return TRUE;

EFS_END
return FALSE;
}

VOID CExportExceptionTag::InitGrid()
{
EFS_BEGIN
	
	// 파일에 저장딘 TAG가 있을 경우 그리드에 보여주기
	FILE *file;
	file = _tfopen( m_strFilePath , _T("rb") ); 
	if ( file == NULL ) return;

	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = file;

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
	
	CString strArrItem[100];
	TCHAR cfileLine[256];
	CString sLine;
	INT nGridRowIdx = 1 ;
	_fgetts( cfileLine , 256, file);
	INT nArrIdx = 0;

	sLine.Format( _T("%s"), cfileLine );
	INT iFind = sLine.Find( _T(";") );
	
	while( iFind > 0 ) {
		
		iFind = sLine.Find( _T(";") );
		CString sItem = sLine.Left( iFind );

		strArrItem[nArrIdx] = sItem ; 
		sLine = sLine.Mid( iFind +1 );
		nArrIdx++;
	}

	m_pGrid->SetRows(nArrIdx + 1);		 

	for ( INT idx = 0 ; idx < nArrIdx ; idx++ ) 
	{
		m_pGrid->SetTextMatrix( idx + 1 , 1 , strArrItem[idx] );
	}

	return;
EFS_END
return;
}

BOOL CExportExceptionTag::PreTranslateMessage(MSG* pMsg) 
{
	//DELETE 키 입력시 해당 그리드 ROW 삭제
	INT nGridIdx = 0;
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE ) 
	{
		nGridIdx = m_pGrid->GetIdx();
		if ( nGridIdx + 2 != m_pGrid->GetRows() ) 	
			m_pGrid->RemoveAt( nGridIdx ) ;
	}

	SetGridIndexNo();

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CExportExceptionTag::SetGridIndexNo()
{
EFS_BEGIN

	// 그리드에 Index 번호 설정
	CString strGridIdx;
	for ( INT idx = 1; idx < m_pGrid->GetRows() ; idx++ )
	{
		strGridIdx.Format( _T("%d"), idx ); 
		m_pGrid->SetTextMatrix( idx, 0, strGridIdx );
	}


EFS_END
}

HBRUSH CExportExceptionTag::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
