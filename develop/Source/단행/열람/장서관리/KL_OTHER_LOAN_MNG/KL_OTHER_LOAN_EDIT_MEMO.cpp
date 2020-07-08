// KL_OTHER_LOAN_EDIT_MEMO.cpp : implementation file
//

#include "stdafx.h"
//#include "kl_other_loan_mng.h"
#include "KL_OTHER_LOAN_EDIT_MEMO.h"
#include "FrameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKL_OTHER_LOAN_EDIT_MEMO dialog


CKL_OTHER_LOAN_EDIT_MEMO::CKL_OTHER_LOAN_EDIT_MEMO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CKL_OTHER_LOAN_EDIT_MEMO::IDD, pParent)
//	: CESL_Mgr(CKL_OTHER_LOAN_EDIT_MEMO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKL_OTHER_LOAN_EDIT_MEMO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCur = 0;
	m_nTotalCnt = 0;
	m_BModify = FALSE;

	// KOL.UDF.022
	m_pGrid = NULL;
	m_pDM = NULL;
}


void CKL_OTHER_LOAN_EDIT_MEMO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKL_OTHER_LOAN_EDIT_MEMO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKL_OTHER_LOAN_EDIT_MEMO, CDialog)
	//{{AFX_MSG_MAP(CKL_OTHER_LOAN_EDIT_MEMO)
	ON_BN_CLICKED(IDC_BUTTON_PRE, OnButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CKL_OTHER_LOAN_EDIT_MEMO::GetData()
{
	if(m_nCur < 0)
	{
		GetDlgItem(IDC_edt_KL_LOAN_MEMO_USERNO)->SetWindowText(_T(""));
		GetDlgItem(IDC_edt_KL_LOAN_MEMO_REGNO)->SetWindowText(_T(""));
		GetDlgItem(IDC_edt_KL_LOAN_MEMO_TITLE)->SetWindowText(_T(""));
		GetDlgItem(IDC_edt_KL_LOAN_MEMO_CONTENT)->SetWindowText(_T(""));
		return;
	}
		
	m_pGrid->SelectGetHeadPosition();
		
	for(int i=0; i<m_nCur; i++)
	{
		m_pGrid->SelectGetNext();						
	}
	
	CString strData;

	m_pDM->GetCellData(_T("이용자번호"), m_pGrid->SelectGetIdx(), strData);
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_USERNO)->SetWindowText(strData);

	m_pDM->GetCellData(_T("등록번호"), m_pGrid->SelectGetIdx(), strData);
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_REGNO)->SetWindowText(strData);

	m_pDM->GetCellData(_T("서명"), m_pGrid->SelectGetIdx(), strData);
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_TITLE)->SetWindowText(strData);

	m_pDM->GetCellData(_T("메모"), m_pGrid->SelectGetIdx(), strData);
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_CONTENT)->SetWindowText(strData);
	
	/*
	m_pGrid->GetAt( m_pGrid->SelectGetIdx(), 2, strData);	
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_USERNO)->SetWindowText(strData);

	m_pGrid->GetAt( m_pGrid->SelectGetIdx(), 3, strData);	
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_REGNO)->SetWindowText(strData);

	m_pGrid->GetAt( m_pGrid->SelectGetIdx(), 4, strData);	
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_TITLE)->SetWindowText(strData);

	m_pGrid->GetAt( m_pGrid->SelectGetIdx(), 14, strData);	
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_CONTENT)->SetWindowText(strData);
	*/
}

void CKL_OTHER_LOAN_EDIT_MEMO::RefreshData()
{
	m_nCur = 0;	
	m_pGrid->SelectMakeList();
	m_nTotalCnt = m_pGrid->SelectGetCount();
	m_pGrid->SelectGetHeadPosition();

	GetData();
}

void CKL_OTHER_LOAN_EDIT_MEMO::RefreshControl()
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
// CKL_OTHER_LOAN_EDIT_MEMO message handlers

void CKL_OTHER_LOAN_EDIT_MEMO::OnButtonPre() 
{
	// TODO: Add your control notification handler code here
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

void CKL_OTHER_LOAN_EDIT_MEMO::OnButtonNext() 
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

BOOL CKL_OTHER_LOAN_EDIT_MEMO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd());

	RefreshData();
	RefreshControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKL_OTHER_LOAN_EDIT_MEMO::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();	
}

void CKL_OTHER_LOAN_EDIT_MEMO::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	CString strContent;
	GetDlgItem(IDC_edt_KL_LOAN_MEMO_CONTENT)->GetWindowText(strContent);
	
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
	CString strLoanKey;

	m_pDM->GetCellData(_T("로컬KEY"), nSelect, strLoanKey);	

	CFrameWriter Writer(m_pInfo);
	Writer.setCommand(_T("LILL_K02_LOC_B03_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("LOAN_KEY"), strLoanKey);
	Writer.addElement(_T("MEMO"), strContent);

	CFrameSender Sender(m_pInfo);	
	CFrameReader Reader; 
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if (0 > ids) 
	{
		AfxMessageBox(_T("저장에 실패하였습니다."));
		/*
		while(true) 
		{			
			if(IDYES == MessageBox(_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("저장실패"), MB_YESNO | MB_ICONQUESTION)) 
			{
				ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
				if(0 <= ids) break;
			}
			else
			{				
				return ;
			}
		}
		*/
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
			m_pDM->SetCellData(_T("MEMO"), strContent, m_pGrid->SelectGetIdx());		
		}
	}
	//CDialog::OnOK();	

	//수정했을 때만 저장되도럭..
}

BOOL CKL_OTHER_LOAN_EDIT_MEMO::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	/*
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {              
		if (GetDlgItem(IDC_edt_KL_LOAN_MEMO_CONTENT) == GetFocus())
        {
            CEdit* edit = (CEdit*)GetDlgItem(IDC_edt_KL_LOAN_MEMO_CONTENT);
            int nLen = edit->GetWindowTextLength();
            edit->SetSel( nLen, nLen );
            edit->ReplaceSel( _T("\r\n") );
        }
    }
	*/
	
	return CDialog::PreTranslateMessage(pMsg);
}
