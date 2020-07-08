// MarcEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcEditDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg dialog


CMarcEditDlg::CMarcEditDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	AfxInitRichEdit();
}

CMarcEditDlg::~CMarcEditDlg()
{
}

BOOL CMarcEditDlg::Create(CWnd *pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CMarcEditDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcEditDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcEditDlg)
	ON_BN_CLICKED(IDC_bPREV_MEC, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT_MEC, OnbNEXT)
	ON_BN_CLICKED(IDC_bAPPLY_MARC, OnbAPPLYMARC)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg Operations
INT CMarcEditDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strMarcFieldAlias, CString strMarcErrorFieldAlias /*= _T("")*/)
{
	EFS_BEGIN

	if (pDM_Parent == NULL || pGrid_Parent == NULL || strMarcFieldAlias.IsEmpty())
		return -1;
	
	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strMarcFieldAlias = strMarcFieldAlias;
	m_strMarcErrorFieldAlias = strMarcErrorFieldAlias;

	return 0;

	EFS_END
	return -1;

}

INT CMarcEditDlg::Init()
{
	EFS_BEGIN

	if (m_pDM_Parent == NULL || m_pGrid_Parent == NULL || m_strMarcFieldAlias.IsEmpty()) return -1;

	INT ids = -1;
	// Subclass RichEdit
	if (!m_marcEditor.SubclassDlgItem(IDC_RICHEDIT_MARC, -1, this))
	{
		AfxMessageBox(_T("Subclass Marc Editor Failed"));
		return -1;
	}

	// Marc Manager Set
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);

	// 부모그리드의 선택된 로우 구성
	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0)
	{
		AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
		return -1;
	}

	INT nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	if (nIdx < 0)
	{
		AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
		return -1;
	}

	m_pGrid_Parent->SetReverse(nIdx);

	return 0;

	EFS_END
	return -1;

}

INT CMarcEditDlg::Display()
{
	EFS_BEGIN

	INT ids = -1;

	CString strDMIdx = m_pGrid_Parent->GetTextMatrix(m_pGrid_Parent->SelectGetIdx() + 1, 0);
	m_nParentSelectedDMIdx = _ttoi(strDMIdx);

	if (m_nParentSelectedDMIdx < 0) return -1;

	CString strStreamMarc = m_pDM_Parent->GetCellData(m_strMarcFieldAlias, m_nParentSelectedDMIdx);
	if ( strStreamMarc.IsEmpty()) return -1;


	ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc);
	if (ids < 0)
	{
		return -1;
	}
	
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	
	m_marcEditor.Display();

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CMarcEditDlg message handlers

BOOL CMarcEditDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (Init() < 0)
	{
		//AfxMessageBox(_T("Marc Editor Init Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMarcEditDlg::OnbPREV() 
{
	EFS_BEGIN

	INT nIdx = m_pGrid_Parent->SelectGetPrev();
	if (nIdx < 0)
		nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	
	m_pGrid_Parent->SetReverse(nIdx);
	Display();

	EFS_END
}

VOID CMarcEditDlg::OnbNEXT() 
{
	EFS_BEGIN

	INT nIdx = m_pGrid_Parent->SelectGetNext();
	if (nIdx < 0)
		nIdx = m_pGrid_Parent->SelectGetTailPosition();

	m_pGrid_Parent->SetReverse(nIdx);
	Display();

	EFS_END
}

VOID CMarcEditDlg::OnbAPPLYMARC() 
{
	EFS_BEGIN

	INT ids = -1;
	if (m_bShowErrorDlg)
		ids = m_marcEditor.ApplyEdit(0);
	else 
		ids = m_marcEditor.ApplyEdit(1);		

	if (ids < 0)
	{
		// 오류 플래그 달기
		if (!m_strMarcErrorFieldAlias.IsEmpty())
		{
			m_pDM_Parent->SetCellData(m_strMarcErrorFieldAlias, _T("마크 문법 오류"), m_nParentSelectedDMIdx);
		}

		if (MessageBox(_T("문법 오류가 있습니다. 그래도 적용하시겠습니까?"), NULL, MB_YESNO | MB_ICONQUESTION) == IDNO)
			return;
	}
	else
	{
		// 정상 플래그 달기
		if (!m_strMarcErrorFieldAlias.IsEmpty())
		{
			m_pDM_Parent->SetCellData(m_strMarcErrorFieldAlias, _T(""), m_nParentSelectedDMIdx);
			m_pGrid_Parent->SetTextMatrix(m_pGrid_Parent->GetRow(), 9, _T(""));
		}	
	}
	
	CString strStremMarc;
	ids = m_pInfo->pMarcMgr->Encoding(&m_marc, strStremMarc);
	if (ids < 0)
	{
		AfxMessageBox(_T("적용실패"));
	}

	m_pDM_Parent->SetCellData(m_strMarcFieldAlias, strStremMarc, m_nParentSelectedDMIdx);
	AfxMessageBox(_T("적용하였습니다"));

	EFS_END
}

HBRUSH CMarcEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
