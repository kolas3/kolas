// ESL_DVM_VIEWER.cpp : implementation file
//

#include "stdafx.h"
//#include _T("esl.h")
#include "ESL_DVM_VIEWER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESL_DVM_VIEWER dialog


CESL_DVM_VIEWER::CESL_DVM_VIEWER(CESL_DataVerifyMgr *pDVM /*=NULL*/)
	: CDialog(CESL_DVM_VIEWER::IDD, NULL)
{


	m_pDVM = pDVM;
	m_bInitDlg = false;
	//{{AFX_DATA_INIT(CESL_DVM_VIEWER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CESL_DVM_VIEWER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESL_DVM_VIEWER)
	DDX_Control(pDX, IDC_COM_MSHFLEXGRID1, m_grid_old);
	DDX_Control(pDX, IDC_COM_MSHFLEXGRID2, m_grid_new);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESL_DVM_VIEWER, CDialog)
	//{{AFX_MSG_MAP(CESL_DVM_VIEWER)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESL_DVM_VIEWER message handlers

BOOL CESL_DVM_VIEWER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// TODO: Add extra initialization here
	m_bInitDlg = true;

	GridSetColnum();
	ShowGridData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//data index setting
VOID CESL_DVM_VIEWER::GridSetColnum()
{
      
	reference *TmpRef;

	m_grid_old.SetCols(0, m_pDVM->RefList.GetCount()+1);
	m_grid_old.SetRows(2);

	m_grid_new.SetCols(0, m_pDVM->RefList.GetCount()+1);
	m_grid_new.SetRows(3);

	for(INT i=0 ; i < m_pDVM->RefList.GetCount() ; i++)
	{
		TmpRef = (reference *)m_pDVM->RefList.GetAt(m_pDVM->RefList.FindIndex(i));
		m_grid_old.SetTextMatrix(0, i+1, TmpRef->FIELD_ALIAS);
		m_grid_new.SetTextMatrix(0, i+1, TmpRef->FIELD_ALIAS);
	    m_grid_new.SetTextMatrix(1, i+1, TmpRef->FILTER1);
	
	}
  
	UpdateData(false);
}

VOID CESL_DVM_VIEWER::ShowGridData()
{

	m_grid_old.SetRows(m_pDVM->NewDM->GetRowCount()+1);
	m_grid_new.SetRows(m_pDVM->NewDM->GetRowCount()+2);

	for(INT i=0 ; i < m_pDVM->NewDM->GetRowCount() ; i++)
	{
		for(INT j=0 ; j < m_pDVM->NewDM->GetColCount() ; j++)
		{
			m_grid_old.SetTextMatrix(i+1, j+1, m_pDVM->OldDM->GetCellData(i,j) );

		}
	}
    
    for(INT p=1 ; p < m_pDVM->NewDM->GetRowCount()+1 ; p++)
	{
		for(INT q=0 ; q < m_pDVM->NewDM->GetColCount() ; q++)
		{
			m_grid_new.SetTextMatrix(p+1, q+1, m_pDVM->NewDM->GetCellData(p-1,q) );
	
		}
	}

    
	FilterDataColor();
	ShowModifyData();
	
	UpdateData(false);
}


VOID CESL_DVM_VIEWER::ShowModifyData()
{
	
	COLORREF blue,red;
	blue = RGB(228,243,251);			//E4F3FB
    red = RGB(255,200,200);
	

	for(INT i=0 ; i < m_pDVM->NewDM->GetRowCount() ; i++)
	{
		for(INT j=0 ; j < m_pDVM->NewDM->GetColCount() ; j++)
		{

			if( m_pDVM->OldDM->GetCellData(i,j) != m_pDVM->NewDM->GetCellData(i,j) )
			{
				
					m_grid_new.SetRow(i+2);
					m_grid_new.SetCol(j+1);
					m_grid_new.SetCellBackColor(blue);
			}
	
		}
	}

	UpdateData(false);

}


BOOL CESL_DVM_VIEWER::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CESL_DVM_VIEWER::FilterDataColor()
{
    COLORREF filter1color;
	filter1color = RGB(200,255,200);	
    
    
	for(INT i=0 ; i < m_pDVM->NewDM->GetColCount() ; i++)
	{

		m_grid_new.SetRow(1);
		m_grid_new.SetCol(i+1);
        m_grid_new.SetCellBackColor(filter1color);
	}
    UpdateData(false);
}


VOID CESL_DVM_VIEWER::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_pDVM->ViewFlag = true;
	CDialog::OnCancel();
}

VOID CESL_DVM_VIEWER::OnOK() 
{
	// TODO: Add extra validation here
	m_pDVM->ViewFlag = true;
	
    CString Msg;
    
    Msg = _T("데이터 변환 과정에서 문제가 발생하였습니다. 원상태로 복구하시겠습니까?");
	
	if(m_pDVM->DoRollBack == TRUE && MessageBox(Msg, _T("Warnning!"), MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		m_pDVM->DoRollBackData();
	    MessageBox(_T("데이터가 원상태로 복구되었습니다."), _T("Warnning!"), MB_ICONEXCLAMATION);
	
	}
   	
	CDialog::OnOK();
}


VOID CESL_DVM_VIEWER::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (!m_bInitDlg) return;
    

    RECT rect;
	
    m_grid_old.GetWindowRect(&rect);   
	ScreenToClient(&rect);
	rect.right = cx - 7;
	rect.bottom = cy/2 - 15;
	m_grid_old.MoveWindow(&rect);
    

	m_grid_new.GetWindowRect(&rect);   
	ScreenToClient(&rect);
	rect.top = cy/2 + 15;
	rect.right = cx - 7;
	rect.bottom = cy - 40;
	m_grid_new.MoveWindow(&rect);
    

    GetDlgItem(IDOK)->GetWindowRect(&rect); 
    ScreenToClient(&rect);
	rect.top = cy - 35;
	rect.left = cx - 273;
	rect.right = cx - 147;
	rect.bottom = cy - 3;
    GetDlgItem(IDOK)->MoveWindow(&rect);
    

    GetDlgItem(IDCANCEL)->GetWindowRect(&rect); 
    ScreenToClient(&rect);
	rect.top = cy - 35;
	rect.left = cx - 132;
	rect.right = cx -  7;
	rect.bottom = cy - 3;
    GetDlgItem(IDCANCEL)->MoveWindow(&rect);
    

    GetDlgItem(IDC_STATIC_VIEWER2)->GetWindowRect(&rect); 
    ScreenToClient(&rect);
	rect.top = cy/2 - 12;
    rect.bottom = cy/2 +2;
    GetDlgItem(IDC_STATIC_VIEWER2)->MoveWindow(&rect);
    

	// TODO: Add your message handler code here
	
}
