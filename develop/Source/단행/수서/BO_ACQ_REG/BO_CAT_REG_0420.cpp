// BO_CAT_REG_0420.cpp : implementation file
//

#include "stdafx.h"
// #include _T("BO_REG_MGR.h")
#include "BO_CAT_REG_0420.h"
#include "BO_CAT_REG_0410.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0420 dialog


CBO_CAT_REG_0420::CBO_CAT_REG_0420(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0420)
	m_nRegData = 0;
	m_nSelectData = 0;
	m_strRegNum1 = _T("");
	m_strRegNum2 = _T("");
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_REG_0420::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0420)
	DDX_Text(pDX, IDC_edtRegData, m_nRegData);
	DDX_Text(pDX, IDC_edtSelectData, m_nSelectData);
	DDX_Text(pDX, IDC_edtRegNum1, m_strRegNum1);
	DDX_Text(pDX, IDC_edtRegNum2, m_strRegNum2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_0420, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_0420)
	ON_BN_CLICKED(IDC_bOperationOK, OnbOperationOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_0420 message handlers

BOOL CBO_CAT_REG_0420::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_DM_ALIAS = _T("DM_단행_정리_등록_메인");
	m_CM_ALIAS = _T("CM_단행_정리_등록_메인");

	CESL_DataMgr *pDM = NULL;
	pDM = FindDM(m_DM_ALIAS);
	if (!pDM)
	{
		AfxMessageBox(_T("DM 에러!")); 
		return -1; 
	}
	CESL_ControlMgr *pCM = NULL;
	pCM = FindCM(m_CM_ALIAS);		
	if (!pCM)
	{
		AfxMessageBox(_T("CM 에러!")); 
		return -1; 
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	UpdateData(TRUE);
	Init();
	Display();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

  INT CBO_CAT_REG_0420::Display()
{
	CESL_DataMgr *pDM = FindDM(m_DM_ALIAS);
	CESL_ControlMgr *pCM = FindCM(m_CM_ALIAS);	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	pGrid->SelectMakeList();
	INT Index = pGrid->SelectGetCount();
	
	if (Index < 1)
	{
		AfxMessageBox(_T("선정한 자료가 없습니다.")); 
		EndDialog(IDCANCEL);
		return -1;
	}
// 선정자료의 수를 얻어옴
	m_nSelectData = Index; 

// 등록자료의 수와 누락등록 테이블에 들어갈 등록자료의 수를 얻어옴 
	INT idx;
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));

	CArray <CString, CString> RegNumberArray; 
	CString strRegNo, strRegCode;
	INT RegNoCount = 0;
//	INT DropRegNoCount = 0;
	
	while ( idx != -1 ) 
	{
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		
		if (strRegNo != _T("")) //등록자료 
		{
			RegNumberArray.Add(strRegNo);
/*
			strRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
			//누락 등록 테이블에 있는 지 조사 
			CString sQuery,sCount;
			sQuery.Format( _T("select count(reg_no) from co_drop_accession_no_tbl WHERE reg_code ='%s' and reg_no =%s") , strRegCode , strRegNo.Mid(2,10));
			pDM->GetOneValueQuery( sQuery , sCount );
			//없으면 누락 등록 테이블에 들어갈 등록번호 수를 증가  
			if ( sCount == _T("0") ) 
				DropRegNoCount++;	
*/			
			RegNoCount++;
		}
		
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	
	INT ArraySize = RegNumberArray.GetSize() - 1;
	if (ArraySize < 0)
	{
		if (OperationType == 0)
		{
			AfxMessageBox(_T("등록번호가 없습니다.")); 
			EndDialog(IDCANCEL); 
			return 0;
		}
		m_nRegData = 0; 
		m_strRegNum1 = _T("");
		m_strRegNum2 = _T("");
	}
//	m_nDropRegNum = ArraySize;
	else
	{
		m_nRegData = RegNoCount; 
		m_strRegNum1 = RegNumberArray.GetAt(0);
		m_strRegNum2 = RegNumberArray.GetAt(ArraySize);
	}
	
	UpdateData(FALSE);
	return 0;
}

VOID CBO_CAT_REG_0420::Init()
{
	if (OperationType == 0)
	{
		GetDlgItem(IDC_bOperationOK)->SetWindowText(_T("번호취소"));
		SetWindowText(_T("번호취소"));
	}
	else if (OperationType == 1)
	{
		GetDlgItem(IDC_bOperationOK)->SetWindowText(_T("등록인계취소"));
		SetWindowText(_T("등록인계취소"));
	}
	else if (OperationType == 2)
	{
		GetDlgItem(IDC_bOperationOK)->SetWindowText(_T("검수취소"));
		SetWindowText(_T("검수취소"));
	}
}

VOID CBO_CAT_REG_0420::OnbOperationOK() 
{
	// TODO: Add your control notification handler code here
	CBO_CAT_REG_0410 *pDlg = NULL;
	pDlg = new CBO_CAT_REG_0410(this);
	if (OperationType == 0)
	{
		pDlg->SetOperationType(OperationType);
		pDlg->CancelRegNo();
	}
	else if (OperationType == 1)
	{
		pDlg->SetOperationType(OperationType);
		pDlg->CancelRegNo();
	}
	else if (OperationType == 2)
	{
		pDlg->CancelType = 1; 
		pDlg->SetOperationType(1);
		pDlg->CancelRegNo();
	}
	

}
