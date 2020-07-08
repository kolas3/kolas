// KL_OTHER_RETURN_SELECT_BOOK.cpp : implementation file
//

#include "stdafx.h"
#include "KL_OTHER_RETURN_SELECT_BOOK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_SELECT_BOOK dialog


CKL_OTHER_RETURN_SELECT_BOOK::CKL_OTHER_RETURN_SELECT_BOOK(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_RETURN_SELECT_BOOK)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKL_OTHER_RETURN_SELECT_BOOK::~CKL_OTHER_RETURN_SELECT_BOOK()
{

}
void CKL_OTHER_RETURN_SELECT_BOOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_RETURN_SELECT_BOOK)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_RETURN_SELECT_BOOK, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_RETURN_SELECT_BOOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_SELECT_BOOK message handlers

BOOL CKL_OTHER_RETURN_SELECT_BOOK::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd());
	InitESL_Mgr(_T("SM_KL_OTHER_RETURN_SELECT_BOOK"));	
	
	const INT COL_CNT = 25;
	CString strAlias[COL_CNT] = 
	{
		_T("트랜젝션상태"),		_T("대출도서관"),		_T("반납도서관"),		_T("대출일"),		_T("반납예정일"),
		_T("반납일"),		_T("반납연기일"),		_T("예약일"),		_T("예약취소일"),		_T("예약만기일"),
		_T("등록번호"),		_T("서명"),		_T("저자"),		_T("발행자"),		_T("청구기호"),
		_T("타관반납발송일"),		_T("복귀일"),		_T("발송자"),		_T("입수자"),		_T("트랜젝션상태설명"),
		_T("대출도서관부호"),		_T("반납도서관부호"),		_T("결제처리KEY"),		_T("로컬KEY"),		_T("부모ROW")
	};
	
	CString strGetData;
	CESL_DataMgr* pParentDM = FindDM(m_strParentDMAlias);
	CESL_DataMgr* pDM = FindDM(_T("DM_KL_OTHER_RETURN_SELECT_BOOK"));

	if(pDM && pParentDM)
	{

		for(INT nCnt = 0; nCnt < m_aRow.GetSize(); nCnt++)
		{
			INT nRow = m_aRow.GetAt(nCnt);			
			pDM->InsertRow(-1);
			INT nSetRow = pDM->GetRowCount() -1;
			
			for(INT nCol = 0; nCol < COL_CNT; nCol++)
			{
				strGetData.Empty();
				strGetData = pParentDM->GetCellData(strAlias[nCol], nRow);				
				pDM->SetCellData(strAlias[nCol], strGetData, nSetRow);
			}
			strGetData.Format(_T("%d"), nRow);
			pDM->SetCellData(_T("부모ROW"), strGetData, nSetRow);
		}
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_OTHER_RETURN_SELECT_BOOK"), _T("GRID"));
	if(pGrid)
	{
		pGrid->Display();
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_RETURN_SELECT_BOOK::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CKL_OTHER_RETURN_SELECT_BOOK::OnOK() 
{
	// TODO: Add extra validation here
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_KL_OTHER_RETURN_SELECT_BOOK"), _T("GRID"));
	pGrid->SelectMakeList();
	if(0 == pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("자료를 선정하여 주십시요."));
		return;
	}
	else if (1 < pGrid->SelectGetCount())
	{
		AfxMessageBox(_T("하나만 선정하여 주십시요."));
		return;
	}
	int nIdx = pGrid->SelectGetHeadPosition();
	CESL_DataMgr* pDM = FindDM(_T("DM_KL_OTHER_RETURN_SELECT_BOOK"));
	pDM->GetCellData(_T("부모ROW"), nIdx, m_strSelectRow);

	CDialog::OnOK();
}

void CKL_OTHER_RETURN_SELECT_BOOK::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
