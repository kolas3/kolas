// BarcodeFlowSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BarcodeFlowSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarcodeFlowSettingDlg dialog


CBarcodeFlowSettingDlg::CBarcodeFlowSettingDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBarcodeFlowSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarcodeFlowSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
	m_pGridEdt = NULL;
}

CBarcodeFlowSettingDlg::~CBarcodeFlowSettingDlg()
{
	FreeGridEdt();
}

void CBarcodeFlowSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarcodeFlowSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBarcodeFlowSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CBarcodeFlowSettingDlg)
	ON_BN_CLICKED(IDC_BTN_SAFE, OnBtnSafe)
	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)
	ON_BN_CLICKED(IDC_BTN_3100_CLOSE, OnBtnClose)
	ON_WM_CTLCOLOR()
	ON_MESSAGE( UPDATE_GRIDTEXT, OnUpdateGridtext )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarcodeFlowSettingDlg message handlers

void CBarcodeFlowSettingDlg::OnBtnSafe() 
{
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_BARCODE_FLOW_SETTING"));
	if( !pDM ) {
		AfxMessageBox(_T("FindDM DM_BO_LOC_BARCODE_FLOW_SETTING Error!"));
		return;
	}

	INT nRow = m_pGrid->GetRows();
	for( INT i=1; i< nRow; i++ )
	{
		CString strWork = m_pGrid->GetTextMatrix(i,1);
		CString strBarcode = m_pGrid->GetTextMatrix(i,2);

		CString strData;
		for( INT j=0; j<pDM->GetRowCount(); j++ )
		{
			pDM->GetCellData(_T("업무"), j, strData);
			if( 0 == strData.Compare(strWork) )
			{
				pDM->SetCellData(_T("바코드"), strBarcode, j);	
				break;
			}
		}
	}

	AfxMessageBox(_T("저장이 완료 되었습니다."), MB_ICONINFORMATION);	
}

void CBarcodeFlowSettingDlg::OnBtnPrint() 
{
	// TODO: Add your control notification handler code here
	
}

void CBarcodeFlowSettingDlg::OnBtnClose() 
{
	CDialog::OnCancel();	
}

HBRUSH CBarcodeFlowSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CBarcodeFlowSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	EnableThemeDialogTexture(GetSafeHwnd()); 	

	if(InitESL_Mgr(_T("BO_LOC_BARCODE_FLOW_SETTING")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}	

	m_pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_FLOWER_LIST);
	
	INT ids = AllControlDisplay(_T("CM_BO_LOC_BARCODE_FLOW_SETTING"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBarcodeFlowSettingDlg::InitDM()
{
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_BARCODE_FLOW_SETTING"));
	if( !pDM ) {
		AfxMessageBox(_T("FindDM Error!"));
		return;
	}
	
	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("자료찾기"), 0);
	pDM->SetCellData(_T("바코드"), _T("WKFYCKWRL"), 0);
	pDM->SetCellData(_T("SM"), _T("자료찾기"), 0);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("예약대출"), 1);
	pDM->SetCellData(_T("바코드"), _T("DDIREOCNF"), 1);
	pDM->SetCellData(_T("SM"), _T("예약대출"), 1);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("예약취소"), 2);
	pDM->SetCellData(_T("바코드"), _T("DDIRCNLTH"), 2);
	pDM->SetCellData(_T("SM"), _T("예약취소"), 2);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("예약상태"), 3);
	pDM->SetCellData(_T("바코드"), _T("DDIRTKDXO"), 3);
	pDM->SetCellData(_T("SM"), _T("BO_LOC_3170"), 3);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("긴급대출"), 4);
	pDM->SetCellData(_T("바코드"), _T("RLSRMQEOCNF"), 4);
	pDM->SetCellData(_T("SM"), _T("긴급대출"), 4);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("이용자입력"), 5);
	pDM->SetCellData(_T("바코드"), _T("DLDYDWKDLQFUR"), 5);
	pDM->SetCellData(_T("SM"), _T("대출자"), 5);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("개인별대출이력보기"), 6);
	pDM->SetCellData(_T("바코드"), _T("RODLSEOCNFDLFUR"), 6);
	pDM->SetCellData(_T("SM"), _T("개인별대출이력보기"), 6);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("화면출력"), 7);
	pDM->SetCellData(_T("바코드"), _T("GHKAUSCNFFUR"), 7);
	pDM->SetCellData(_T("SM"), _T("화면출력"), 7);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("가족상세정보"), 8);
	pDM->SetCellData(_T("바코드"), _T("DDIRGKRL"), 8);
	pDM->SetCellData(_T("SM"), _T("가족상세정보"), 8);

	pDM->InsertRow(-1);
	pDM->SetCellData(_T("업무"), _T("초기화"), 9);
	pDM->SetCellData(_T("바코드"), _T("XHDGKQEOCNFWK"), 9);
	pDM->SetCellData(_T("SM"), _T("초기화"), 9);
}

VOID CBarcodeFlowSettingDlg::GetSM(CString strBarcode, CString& strSM)
{
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_BARCODE_FLOW_SETTING"));
	if( !pDM ) {
		AfxMessageBox(_T("FindDM Error!"));
		return;
	}

	CString strData;
	for( INT i=0; i<pDM->GetRowCount(); i++ )
	{
		pDM->GetCellData(_T("바코드"), i, strData);

		if( 0 == strBarcode.Compare(strData) )
		{
			pDM->GetCellData(_T("SM"), i, strSM);
			return;
		}
	}

	strSM.Empty();
}

BEGIN_EVENTSINK_MAP(CBarcodeFlowSettingDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBarcodeFlowSettingDlg)
	ON_EVENT(CBarcodeFlowSettingDlg, IDC_GRID_FLOWER_LIST, -600 /* Click */, OnClickGridFlowerList, VTS_NONE)
	ON_EVENT(CBarcodeFlowSettingDlg, IDC_GRID_FLOWER_LIST, -601 /* DblClick */, OnDblClickGridFlowerList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CBarcodeFlowSettingDlg::OnClickGridFlowerList() 
{
	
}

void CBarcodeFlowSettingDlg::OnDblClickGridFlowerList() 
{
	INT nCol = m_pGrid->GetCol();
	
	InputDataToGrid();
	
	m_pGrid->SetCol( nCol );	
}

INT CBarcodeFlowSettingDlg::InputDataToGrid()
{
	INT nRow = m_pGrid->GetRow();
	INT nCol = m_pGrid->GetCol();
	CString sValue = m_pGrid->GetText();

	if( !nRow || nCol != 2)
		return 0;
	
	FreeGridEdt();	

	m_pGridEdt = new CGridUpdateEdt;

	m_pGridEdt->SetParentWnd( (CWnd*)this );
	
	m_pGridEdt->m_nRow = nRow;
	m_pGridEdt->m_nCol = nCol;

	CWnd *pWnd = (CWnd*)GetDlgItem(IDC_GRID_FLOWER_LIST);
	
	m_pGridEdt->Create( ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL|WS_CHILD, CRect( 0 , 0, 10, 20 ), pWnd->GetParent(), 645 );

	CRect rect,gridRect;
	RECT parentRect;
	pWnd->GetWindowRect( &gridRect );
	pWnd->GetParent()->GetClientRect( &parentRect );

	::ClientToScreen( pWnd->GetParent()->m_hWnd, ( POINT * )&( parentRect.left ) );

	gridRect.top -= parentRect.top;
	gridRect.left -= parentRect.left;

	rect.left   = gridRect.left + ( long )( ( float )m_pGrid->GetCellLeft()   / 15.0f );
	rect.top    = gridRect.top  + ( long )( ( float )m_pGrid->GetCellTop()    / 15.0f );
	rect.right  = rect.left     + ( long )( ( float )m_pGrid->GetCellWidth()  / 15.0f );
	rect.bottom = rect.top      + ( long )( ( float )m_pGrid->GetCellHeight() / 15.0f );
	m_pGridEdt->MoveWindow( rect.left - 1, rect.top - 1, rect.Width(), rect.Height() );

	m_pGridEdt->ShowWindow( true );
	m_pGridEdt->SetFocus();

	m_pGridEdt->SetWindowText( sValue );

 	return 0;
}

INT CBarcodeFlowSettingDlg::FreeGridEdt()
{
	if ( m_pGridEdt )
	{
		delete m_pGridEdt;
		m_pGridEdt = NULL;
	}

	return 0;
}

VOID CBarcodeFlowSettingDlg::OnUpdateGridtext(WPARAM wParam, LPARAM lParam)
{
	if( wParam )
	{
		CString newValue;
		m_pGridEdt->GetWindowText( newValue );		

		INT curRow = m_pGrid->GetRow();
		INT curCol = m_pGrid->GetCol();

		m_pGrid->SetRow( m_pGridEdt->m_nRow );
		m_pGrid->SetCol( m_pGridEdt->m_nCol );
		m_pGrid->SetText( newValue );				
	}

	FreeGridEdt();
	m_pGrid->SetFocus();
}
