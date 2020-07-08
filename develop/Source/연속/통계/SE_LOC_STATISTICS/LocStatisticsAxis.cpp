// LocStatisticsAxis.cpp : implementation file
//

#include "stdafx.h"
#include "LocStatisticsAxis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsAxis dialog


CLocStatisticsAxis::CLocStatisticsAxis(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocStatisticsAxis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strClassName = _T("");
	m_pGrid = NULL;
	m_bCreated = FALSE;
	m_nTYPE = 1; //default
}

CLocStatisticsAxis::~CLocStatisticsAxis()
{
}

VOID CLocStatisticsAxis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocStatisticsAxis)
	DDX_Control(pDX, IDC_treeCOLUMMN_NAME, m_ctrlTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocStatisticsAxis, CDialog)
	//{{AFX_MSG_MAP(CLocStatisticsAxis)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsAxis message handlers
BOOL CLocStatisticsAxis::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	m_bCreated = TRUE;

	return CDialog::Create(IDD, pParentWnd);
}

BOOL CLocStatisticsAxis::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("연속_열람_통계_행렬") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_열람_통계_행렬") );
		return FALSE;
	}
	
	CESL_ControlMgr* pCM = FindCM(_T("CM_연속_열람_통계_행렬"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("그리드"));
	
	if(!m_pGrid)
	{
		AfxMessageBox(_T("예상 모습 그리드를 찾을 수 없습니다!"));
		return FALSE;
	}

	if(m_strClassName.IsEmpty())
	{
		AfxMessageBox(_T("행 조건이 입력되지 않았습니다!"));
		return FALSE;
	}

	m_TreeImage.Create(IDB_TREEBITMAP, 16, 0, RGB(255, 255, 255));
	
	SetClassCondition();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLocStatisticsAxis::SetGrid(CString strClassName)
{
	//열은 KDC분류기호로 행은 조건에 따라 설정한다
	
	INT nKDCCode = 256;
	INT nDefaultValue = 0;
	CString strKDCCode[256], strKDCDesc[256];
	nDefaultValue = 0;
	
	m_pInfo->pCodeMgr->GetCode(_T("KDC대주제부호관리"), strKDCCode, strKDCDesc, nKDCCode, nDefaultValue);	
	INT nColCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("KDC대주제부호관리"));
	
	INT nRowCnt;
	INT nCode = 256;
	CString strCode[256], strDesc[256];

	if(_T("배가상태") == strClassName) //배가상태의 경우 
	{
		
		if(3 == m_nTYPE) //실시간 열람
		{
			GetWorkingStatusDescReal(strDesc);
			nRowCnt = 6;

		}
		else 
		{
			GetWorkingStatusDesc(strDesc);
			nRowCnt = 10;
		}
	
	}
	else //기타 조건의 경우 
	{
		m_pInfo->pCodeMgr->GetCode(strClassName, strCode, strDesc, nCode, nDefaultValue);	
		nRowCnt = m_pInfo->pCodeMgr->GetCodeCount(strClassName);
	}

	m_pGrid->SetCols(0,nColCnt+1);
	m_pGrid->SetRows(nRowCnt+1);

	//열 설정
	m_pGrid->SetRow(0);

	for(INT i=0;i<nColCnt;i++)
	{
		m_pGrid->SetCol(i+1);
		m_pGrid->SetText(strKDCDesc[i]);
	}
	
	//행 설정
	m_pGrid->SetCol(0);

	for(i=0;i<nRowCnt;i++)
	{
		m_pGrid->SetRow(i+1);
		m_pGrid->SetText(strDesc[i]);
	}

	return 0;
}

INT CLocStatisticsAxis::SetTreeCtrl(CString strClassName)
{
	m_ctrlTree.SetImageList( &m_TreeImage , TVSIL_NORMAL );

	m_ctrlTree.DeleteAllItems();

	TVINSERTSTRUCTW tvinsert;

	// 기본
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.mask = TVIF_TEXT | TVIF_IMAGE;
	tvinsert.item.state = 0;
	tvinsert.item.stateMask = 0;
	tvinsert.item.cchTextMax = 60;
	tvinsert.item.cChildren = 0;
	tvinsert.item.iSelectedImage = 0;
	
	const INT nCnt = 2;
	TCHAR *sTitleName[nCnt] = {_T("열"),_T("행")};

	HTREEITEM hItem;
	
	CString strDefaultColumn = _T("분류기호별");


	for( INT i = 0 ; i < 2 ; i++ )
	{
		tvinsert.item.iImage = i+1;
		tvinsert.hParent = NULL;
		tvinsert.item.pszText = (LPTSTR)(LPCTSTR)sTitleName[i];//(LPWSTR)sTitleName[i];
		hItem = m_ctrlTree.InsertItem(&tvinsert);
		tvinsert.hParent = hItem;
		

		tvinsert.item.iImage = 3;

		TCHAR sTemp[200];
		
		if(0 == i) 
			_tcscpy( sTemp , strDefaultColumn.GetBuffer(0) );
		else
			_tcscpy( sTemp , strClassName.GetBuffer(0) );

		tvinsert.item.pszText = (LPTSTR)(LPCTSTR)sTemp;
		m_ctrlTree.InsertItem( &tvinsert );
		m_ctrlTree.Expand(hItem,TVE_EXPAND);
		
	}

	
	return 0;
}

VOID CLocStatisticsAxis::ReSizeControl()
{
	if( GetSafeHwnd() == NULL ) return;

	INT cx , cy;
	CRect WndPos;
	GetWindowRect(WndPos);
	cx = WndPos.Width();
	cy = WndPos.Height();
	
	// 예상모습 문자
	if( GetDlgItem(IDC_staRIGHTTITLE)->GetSafeHwnd() == NULL ) return;
	CRect WndRect;
	GetDlgItem(IDC_staRIGHTTITLE)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_staRIGHTTITLE)->MoveWindow( WndRect.left , WndRect.top , cx - WndRect.left - 10 , WndRect.Height() );
	
	// 트리
	if( GetDlgItem(IDC_treeCOLUMMN_NAME)->GetSafeHwnd() == NULL ) return;
	GetDlgItem(IDC_treeCOLUMMN_NAME)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	GetDlgItem(IDC_treeCOLUMMN_NAME)->MoveWindow( WndRect.left , WndRect.top , WndRect.Width() , cy - WndRect.top - 50 );
	
	INT nTop;
	nTop = WndRect.top;
	
	// 그리드
	if( GetDlgItem(IDC_gridEXP_GRID)->GetSafeHwnd() == NULL ) return;
	
	GetDlgItem(IDC_gridEXP_GRID)->GetWindowRect(&WndRect);
	ScreenToClient(WndRect);
	
	GetDlgItem(IDC_gridEXP_GRID)->MoveWindow( WndRect.left , nTop , cx - WndRect.left - 10 , cy - nTop - 50 );
	GetDlgItem(IDC_gridEXP_GRID)->RedrawWindow();
	
}

VOID CLocStatisticsAxis::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	ReSizeControl();

}

VOID CLocStatisticsAxis::SetClassCondition()
{
	SetTreeCtrl(m_strClassName);
	SetGrid(m_strClassName);

}

VOID CLocStatisticsAxis::GetWorkingStatusDesc(CString strDesc[])
{
	strDesc[0] = _T("관외대출자료");
	strDesc[1] = _T("관내대출자료");
	strDesc[2] = _T("특별대출자료");
	strDesc[3] = _T("긴급대출자료");
	strDesc[4] = _T("배달자료");
	strDesc[5] = _T("비치자료");
	strDesc[6] = _T("수리제본자료");
	strDesc[7] = _T("분실자료");
	strDesc[8] = _T("제적자료");
	strDesc[9] = _T("파손자료");
	//=====================================================
	// 18.09.27 JMJ KOL.RED.2018.008
	strDesc[10] = _T("연속책두레업무진행자료");
	//=====================================================
	
}

VOID CLocStatisticsAxis::GetWorkingStatusDescReal(CString strDesc[])
{
	strDesc[0] = _T("실시간배가자료");
	strDesc[1] = _T("관외대출자료");
	strDesc[2] = _T("관내대출자료");
	strDesc[3] = _T("특별대출자료");
	strDesc[4] = _T("긴급대출자료");
	strDesc[5] = _T("배달자료");
	//=====================================================
	// 18.09.27 JMJ KOL.RED.2018.008
	strDesc[5] = _T("연속책두레업무진행자료");
	//=====================================================
	

}
HBRUSH CLocStatisticsAxis::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
