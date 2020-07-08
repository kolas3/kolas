// BO_ACQ_ResultSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_ResultSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ResultSearchDlg dialog


CBO_ACQ_ResultSearchDlg::CBO_ACQ_ResultSearchDlg(CESL_Mgr* pParent /*=NULL*/, CESL_DataMgr * pDM, CESL_Grid * pGrid)
	: CBO_SearchDlg(CBO_ACQ_ResultSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_ResultSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = pDM;
	m_pGrid = pGrid;
	m_pCM = NULL;
	m_idx = 0;
	pMakeSearchData = new CMakeSearchData(pParent); 
}

CBO_ACQ_ResultSearchDlg::~CBO_ACQ_ResultSearchDlg()
{
	if (pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL; 
	}

}

VOID CBO_ACQ_ResultSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_ResultSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_ResultSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_ResultSearchDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_btnFINDALL, OnbtnFINDALL)
	ON_BN_CLICKED(IDC_btnFINDNEXT, OnbtnFINDNEXT)
	ON_EN_CHANGE(IDC_edtRESEARCH_TITLE, OnChangeedtRESEARCHTITLE)
	ON_EN_CHANGE(IDC_edtRESEARCH_AUTHOR, OnChangeedtRESEARCHAUTHOR)
	ON_EN_CHANGE(IDC_edtRESEARCH_ISBN, OnChangeedtRESEARCHISBN)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_EN_SETFOCUS(IDC_edtRESEARCH_AUTHOR, OnSetfocusedtRESEARCHAUTHOR)
	ON_EN_SETFOCUS(IDC_edtRESEARCH_TITLE, OnSetfocusedtRESEARCHTITLE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_ResultSearchDlg message handlers

VOID CBO_ACQ_ResultSearchDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CBO_ACQ_ResultSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_결과내재검색")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	//CM/DM설정
	m_pCM = FindCM(_T("CM_결과내재검색"));	

	if(!m_pDM || !m_pGrid)
	{
		ESLAfxMessageBox(_T("재검색할 DM과 그리드 컨트롤을 찾을 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_ResultSearchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CBO_ACQ_ResultSearchDlg::OnbtnFINDALL() 
{
	// TODO: Add your control notification handler code here
	FindAllResultData();
}

VOID CBO_ACQ_ResultSearchDlg::OnbtnFINDNEXT() 
{
	// TODO: Add your control notification handler code here
	FindNextResultData();
}

BOOL CBO_ACQ_ResultSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if (pMsg->message == WM_KEYDOWN && VK_RETURN == pMsg->wParam ) 
	{
		PostMessage(WM_COMMAND, IDC_btnFINDALL, BN_CLICKED);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBO_ACQ_ResultSearchDlg::OnChangeedtRESEARCHTITLE() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_idx = -1;
}

VOID CBO_ACQ_ResultSearchDlg::OnChangeedtRESEARCHAUTHOR() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_idx = -1;	
}

VOID CBO_ACQ_ResultSearchDlg::OnChangeedtRESEARCHISBN() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	m_idx = -1;
}

VOID CBO_ACQ_ResultSearchDlg::FindNextResultData()
{
	INT count = m_pDM->GetRowCount();
	if(count == 0)
	{
		ESLAfxMessageBox(_T("검색할 자료가 없습니다."));
		return;
	}
	CString title, author, isbn;
	m_pCM->GetControlMgrData(_T("본표제"), title);
	m_pCM->GetControlMgrData(_T("저작자"), author);
	m_pCM->GetControlMgrData(_T("낱권ISBN"), isbn);
	if(title.IsEmpty() && author.IsEmpty() && isbn.IsEmpty()) 
	{
		ESLAfxMessageBox(_T("검색조건을 입력해 주십시오"));
		return;
	}
	
	pMakeSearchData->GetKeyWord(title);		pMakeSearchData->ChangeHanjaToHangul(title);
	pMakeSearchData->GetKeyWord(author);		pMakeSearchData->ChangeHanjaToHangul(author);

	CString srcTitle, srcAuthor, srcISBN, tmpstr;
	INT ids;
	for(INT i=m_idx+1 ; i<count ; i++)
	{
		m_pDM->GetCellData(_T("IBS_색인본표제"), i, srcTitle);
		m_pDM->GetCellData(_T("IBS_색인저작자"), i, srcAuthor);
		m_pDM->GetCellData(_T("BB_낱권ISBN"), i, srcISBN);
		ids = srcTitle.Find(title.GetBuffer(0));
		if(ids >= 0)
		{
			if(!author.IsEmpty())
			{
				ids = srcAuthor.Find(author.GetBuffer(0));
				if(ids < 0)
					continue;
			}
			if(!isbn.IsEmpty())
			{
				ids = srcISBN.Find(isbn.GetBuffer(0));
				if(ids < 0)
					continue;
			}
			break;
		}
	}

	if(i >= count) 
	{ 
		ESLAfxMessageBox(_T("검색결과가 없습니다"));
		return;
	}

	this->ClearGridSelect();	

	m_idx = i;
	m_pGrid->SetAt(m_idx, 0, _T("V"));
	m_pGrid->SetReverse(m_idx );
	m_pGrid->SetTopRow(m_idx + 1);
}

VOID CBO_ACQ_ResultSearchDlg::FindAllResultData()
{
	INT count = m_pDM->GetRowCount();
	if(count == 0)
	{
		ESLAfxMessageBox(_T("검색할 자료가 없습니다."));
		return;
	}
	CString title, author, isbn;
	m_pCM->GetControlMgrData(_T("본표제"), title);
	m_pCM->GetControlMgrData(_T("저작자"), author);
	m_pCM->GetControlMgrData(_T("낱권ISBN"), isbn);
	if(title.IsEmpty() && author.IsEmpty() && isbn.IsEmpty()) 
	{
		ESLAfxMessageBox(_T("검색조건을 입력해 주십시오"));
		return;
	}
	
	pMakeSearchData->GetKeyWord(title);		pMakeSearchData->ChangeHanjaToHangul(title);
	pMakeSearchData->GetKeyWord(author);	pMakeSearchData->ChangeHanjaToHangul(author);

	
	CString srcTitle, srcAuthor, srcISBN, tmpstr;
	CStringList idxList;
	INT ids;
	for(INT i=0 ; i<count ; i++)
	{
		m_pDM->GetCellData(_T("IBS_색인본표제"), i, srcTitle);
		m_pDM->GetCellData(_T("IBS_색인저작자"), i, srcAuthor);
		m_pDM->GetCellData(_T("BB_낱권ISBN"), i, srcISBN);
		ids = srcTitle.Find(title.GetBuffer(0));
		if(ids >= 0)
		{
			if(!author.IsEmpty())
			{
				ids = srcAuthor.Find(author.GetBuffer(0));
				if(ids < 0)
					continue;
			}
			if(!isbn.IsEmpty())
			{
				ids = srcISBN.Find(isbn.GetBuffer(0));
				if(ids < 0)
					continue;
			}
			tmpstr.Format(_T("%d"), i);
			idxList.AddTail(tmpstr);
		}
	}

	if(idxList.IsEmpty())
	{ 
		ESLAfxMessageBox(_T("검색결과가 없습니다"));
		return;
	}
	this->ClearGridSelect();	

	count = idxList.GetCount();
	POSITION pos = idxList.GetHeadPosition();
	INT idx;
	while(pos != NULL)
	{
		idx = _ttoi((idxList.GetNext(pos)).GetBuffer(0));
		m_pGrid->SetAt(idx, 0, _T("V"));
		m_pGrid->SetReverse(idx);	
	}
	
	m_idx = idx;

	idx = _ttoi(idxList.GetHead().GetBuffer(0));
	m_pGrid->SetTopRow(idx+1);
}

VOID CBO_ACQ_ResultSearchDlg::ClearGridSelect()
{
	if(!m_pGrid) return;
	INT rowCnt = m_pGrid->GetCount();
	for(INT i=0 ; i<rowCnt ; i++)
	{
		m_pGrid->SetAt(i, 0, _T(""));
	}
}

VOID CBO_ACQ_ResultSearchDlg::ShowGroupInfo()
{
	return;
}

VOID CBO_ACQ_ResultSearchDlg::OnbtnCLEAR() 
{
	// TODO: Add your control notification handler code here
	ClearScreen(m_pCM);
	ShowGroupInfo();
}

VOID CBO_ACQ_ResultSearchDlg::OnSetfocusedtRESEARCHAUTHOR() 
{
	SetIME(IDC_edtRESEARCH_AUTHOR, 1);
}

VOID CBO_ACQ_ResultSearchDlg::OnSetfocusedtRESEARCHTITLE() 
{
	SetIME(IDC_edtRESEARCH_TITLE, 1);
}

HBRUSH CBO_ACQ_ResultSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
