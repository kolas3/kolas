// MarcManagerRelation.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerRelation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerRelation dialog


CMarcManagerRelation::CMarcManagerRelation(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerRelation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerRelation)
	m_sRelationBasicParam = _T("");
	m_sRelationDescrip = _T("");
	m_sRelationGroupCode = _T("");
	m_sRelationOperator = _T("");
	m_sRelationParam = _T("");
	//}}AFX_DATA_INIT

    m_bModifyFlag = FALSE;
}


VOID CMarcManagerRelation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerRelation)
	DDX_Text(pDX, IDC_eRelationBasicParam, m_sRelationBasicParam);
	DDX_Text(pDX, IDC_eRelationDescrip, m_sRelationDescrip);
	DDX_Text(pDX, IDC_eRelationGroupCode, m_sRelationGroupCode);
	DDX_Text(pDX, IDC_eRelationOperator, m_sRelationOperator);
	DDX_Text(pDX, IDC_eRelationParam, m_sRelationParam);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerRelation, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerRelation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerRelation message handlers

BOOL CMarcManagerRelation::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_eRelationGroupCode)->EnableWindow(FALSE);

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMarcManagerRelation::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	if(m_sRelationBasicParam.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("기준인자를 입력해 주십시오!"));
	    return;
	}

    if(m_sRelationOperator.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("연산기호를 입력해 주십시오!"));
	    return;
	}

	INT ids = 0;
	ids = CheckillegalSign(m_sRelationBasicParam);
	if(ids<0) return;
	
	ids = CheckillegalSign(m_sRelationDescrip);
	if(ids<0) return;

	ids = CheckillegalSign(m_sRelationOperator);
	if(ids<0) return;
	
	ids = CheckillegalSign(m_sRelationParam);
	if(ids<0) return;

	CDialog::OnOK();
}

INT CMarcManagerRelation::CheckillegalSign(CString strData)
{
	INT pos = strData.Find(_T("*"));
	if(pos<0) return 0;
	
	AfxMessageBox(_T("* 기호는 사용할 수 없습니다. 확인 해 주십시요!"));

	return -1;
}
