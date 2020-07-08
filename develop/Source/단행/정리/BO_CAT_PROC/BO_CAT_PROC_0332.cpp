// BO_CAT_PROC_0332.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0332.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0332 dialog


CBO_CAT_PROC_0332::CBO_CAT_PROC_0332(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	AfxInitRichEdit();
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0332)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pGrid_Parent = NULL;
	m_pDM_Parent = NULL;

	m_nParentSelectedRow = 0;
}

CBO_CAT_PROC_0332::~CBO_CAT_PROC_0332()
{
}

VOID CBO_CAT_PROC_0332::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0332)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0332, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0332)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btn0332MarcCLOSE, Onbtn0332MarcCLOSE)
	ON_BN_CLICKED(IDC_bPREV_BCP, OnbPREVBCP)
	ON_BN_CLICKED(IDC_bNEXT_BCP, OnbNEXTBCP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0332 Operations
INT CBO_CAT_PROC_0332::MakeSelectListParentGrid()
{
	EFS_BEGIN

	INT ids = -1;

	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0)
	{
		AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
		return -1;
	}
	
	if (m_nParentSelectedRow == -1)
	{
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
		if (m_nParentSelectedRow < 0)
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}
	}
	else
	{
		POSITION pos = m_pGrid_Parent->m_arraySelect.GetHeadPosition();
		while (pos)
		{
			m_pGrid_Parent->m_arrayPos = pos;
			if (m_nParentSelectedRow == m_pGrid_Parent->m_arraySelect.GetNext(pos))
				break;				
		}
		
		if (m_pGrid_Parent->m_arrayPos == NULL)
		{
			m_pGrid_Parent->SelectRemoveAll();
			INT nRowCount = m_pGrid_Parent->GetCount();
			for (INT idx = 0; idx < nRowCount; idx++)
				m_pGrid_Parent->m_arraySelect.AddTail(idx);
		}
		
		if (m_pGrid_Parent->m_arraySelect.GetCount() > 0)
			m_pGrid_Parent->m_arrayPos = m_pGrid_Parent->m_arraySelect.GetHeadPosition();
		else
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}
		
		pos = m_pGrid_Parent->m_arraySelect.GetHeadPosition();
		while (pos)
		{
			m_pGrid_Parent->m_arrayPos = pos;
			if (m_nParentSelectedRow == m_pGrid_Parent->m_arraySelect.GetNext(pos))
				break;				
		}
	}
	
	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	
	return 0;

	EFS_END
	return -1;
}

INT CBO_CAT_PROC_0332::Display()
{
	CString strStreamMarc = m_pDM_Parent->GetCellData( _T("MARC") , m_nParentSelectedRow);

	INT ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc);
	if (ids < 0) return ids;
	
	m_marcEditor.InitUserAlias();
	m_marcEditor.InitLimitTag();
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	return 0;
}

BOOL CBO_CAT_PROC_0332::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	INT ids = -1;

	m_pGrid_Parent = (CESL_Grid*)FindControl(_T("CM_BO_CAT_PROC_0330"), _T("분류지원시스템_그리드"));
	if (!m_pGrid_Parent) goto ERR;

	m_nParentSelectedRow = m_pGrid_Parent->GetIdx();

	ids = MakeSelectListParentGrid();
	if (ids < 0) goto ERR;
		
	
	m_pDM_Parent = FindDM(_T("DM_BO_CAT_PROC_0330"));
	if ( NULL == m_pDM_Parent) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0330 error") );
		goto ERR;
	}

	m_pDM_Parent->SetMarcMgr(m_pInfo->pMarcMgr);
	
	m_marcEditor.SubclassDlgItem(IDC_REDT_0332_MARC_EDITOR, -1, this);		
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);

	Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	EndDialog(IDCANCEL);
	return FALSE;

EFS_END
return FALSE;
}

VOID CBO_CAT_PROC_0332::Onbtn0332MarcCLOSE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	EndDialog(IDOK);

EFS_END
}

VOID CBO_CAT_PROC_0332::OnbPREVBCP() 
{
	if (m_nParentSelectedRow < 0) return;
	INT nIdx = m_pGrid_Parent->SelectGetPrev();
	if (nIdx >= 0)
		m_nParentSelectedRow = nIdx;
	else
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

	Display();
}

VOID CBO_CAT_PROC_0332::OnbNEXTBCP() 
{
	if (m_nParentSelectedRow < 0) return;
	INT nIdx = m_pGrid_Parent->SelectGetNext();
	if (nIdx >= 0)
		m_nParentSelectedRow = nIdx;
	else
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetTailPosition();

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);	

	Display();
}

HBRUSH CBO_CAT_PROC_0332::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
