// KL_OTHER_RETURN_EDIT_MEMO.cpp : implementation file
//

#include "stdafx.h"

#include "KL_OTHER_RETURN_EDIT_MEMO.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_EDIT_MEMO dialog


CKL_OTHER_RETURN_EDIT_MEMO::CKL_OTHER_RETURN_EDIT_MEMO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_OTHER_RETURN_EDIT_MEMO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_RETURN_EDIT_MEMO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCur = 0;
	m_nTotalCnt = 0;

	m_BModify = FALSE;

	m_pGrid = NULL;
	m_pDM = NULL;
}


void CKL_OTHER_RETURN_EDIT_MEMO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_RETURN_EDIT_MEMO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_RETURN_EDIT_MEMO, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_RETURN_EDIT_MEMO)
	ON_BN_CLICKED(IDC_BUTTON_PRE, OnButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CKL_OTHER_RETURN_EDIT_MEMO::GetData()
{
	if(m_nCur < 0)
	{
		GetDlgItem(IDC_edt_KL_RETURN_MEMO_USERNO)->SetWindowText(_T(""));
		GetDlgItem(IDC_edt_KL_RETURN_MEMO_REGNO)->SetWindowText(_T(""));
		GetDlgItem(IDC_edt_KL_RETURN_MEMO_TITLE)->SetWindowText(_T(""));
		GetDlgItem(IDC_edt_KL_RETURN_MEMO_CONTENT)->SetWindowText(_T(""));
		return;
	}
		
	m_pGrid->SelectGetHeadPosition();
		
	for(int i=0; i<m_nCur; i++)
	{
		m_pGrid->SelectGetNext();						
	}
	
	CString strData;

	m_pDM->GetCellData(_T("이용자명"), m_pGrid->SelectGetIdx(), strData);
	GetDlgItem(IDC_edt_KL_RETURN_MEMO_USERNO)->SetWindowText(strData);

	m_pDM->GetCellData(_T("등록번호"), m_pGrid->SelectGetIdx(), strData);
	GetDlgItem(IDC_edt_KL_RETURN_MEMO_REGNO)->SetWindowText(strData);

	m_pDM->GetCellData(_T("서명"), m_pGrid->SelectGetIdx(), strData);
	GetDlgItem(IDC_edt_KL_RETURN_MEMO_TITLE)->SetWindowText(strData);

	m_pDM->GetCellData(_T("메모"), m_pGrid->SelectGetIdx(), strData);
	GetDlgItem(IDC_edt_KL_RETURN_MEMO_CONTENT)->SetWindowText(strData);
}

void CKL_OTHER_RETURN_EDIT_MEMO::RefreshData()
{
	m_nCur = 0;	
	m_pGrid->SelectMakeList();
	m_nTotalCnt = m_pGrid->SelectGetCount();
	m_pGrid->SelectGetHeadPosition();

	GetData();
}

void CKL_OTHER_RETURN_EDIT_MEMO::RefreshControl()
{	
	if(m_nCur <= 0)
	{
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	}
	else
	{ 
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);
	}

	if(m_nTotalCnt - 1 <= m_nCur)
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_RETURN_EDIT_MEMO message handlers

void CKL_OTHER_RETURN_EDIT_MEMO::OnButtonPre() 
{
	--m_nCur;

	if(m_nCur < 0)
	{
		m_nCur = 0;	
	}
	else
	{
		GetData();
		RefreshControl();
	}	
}

void CKL_OTHER_RETURN_EDIT_MEMO::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	++m_nCur;
	
	if(m_nTotalCnt <= m_nCur)
	{
		m_nCur = m_nTotalCnt;
	}
	else
	{
		GetData();
		RefreshControl();
	}	
}

void CKL_OTHER_RETURN_EDIT_MEMO::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	CString strContent;
	GetDlgItem(IDC_edt_KL_RETURN_MEMO_CONTENT)->GetWindowText(strContent);
	
	if(strContent.GetLength() > 1000)
	{
		AfxMessageBox(_T("메모는 1000자 이하로 작성바랍니다."));
		return;
	}

	if(!m_pDM)
	{
		AfxMessageBox(_T("다시 시도바랍니다."));
		return ;
	}

	int nSelect = m_pGrid->SelectGetIdx();
	CString strHoldLib, strLoanLib, strLocalKey, strOtherLoanKey;
	
	m_pDM->GetCellData(_T("제공도서관부호"), nSelect, strHoldLib);
	m_pDM->GetCellData(_T("대출도서관부호"), nSelect, strLoanLib);
	m_pDM->GetCellData(_T("로컬KEY"), nSelect, strLocalKey);
	m_pDM->GetCellData(_T("타관대출KEY"), nSelect, strOtherLoanKey);

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("IL_K18_LOC_B02_SERVICE"));
	Writer.addRecord();
	
	Writer.addElement(_T("제공도서관부호"), strHoldLib);
	Writer.addElement(_T("대출도서관부호"), strLoanLib);
	Writer.addElement(_T("로컬KEY"), strLocalKey);
	Writer.addElement(_T("타관대출KEY"), strOtherLoanKey);

	Writer.addElement(_T("메모"), strContent);

	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader; 
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if (0 > ids) 
	{
		AfxMessageBox(_T("저장에 실패하였습니다."));

	}	
	else
	{
		Reader.parseFrame(Reader.getFrame());
		if(_T("OK") != Reader.getElement(0,0))
		{			
			AfxMessageBox(_T("저장에 실패하였습니다."));			
		}
		else
		{
			AfxMessageBox(_T("저장하였습니다."));
			m_BModify = TRUE;
			m_pDM->SetCellData(_T("메모"), strContent, m_pGrid->SelectGetIdx());		
		}		
	}

}

void CKL_OTHER_RETURN_EDIT_MEMO::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	
	CDialog::OnCancel();
}

BOOL CKL_OTHER_RETURN_EDIT_MEMO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd());

	RefreshData();
	RefreshControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CKL_OTHER_RETURN_EDIT_MEMO::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreTranslateMessage(pMsg);
}
