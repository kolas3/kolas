// TabSecondForm.cpp : implementation file
//

#include "stdafx.h"
#include "TabSecondForm.h"
#include "SubfieldDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSecondForm dialog


CTabSecondForm::CTabSecondForm(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabSecondForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM_SUBFIELD = NULL;
	m_strTagAlias = _T("");
	m_pGridSubfield = NULL;
	m_nCurrentIdx = -1;

	CR_user = RGB(60,179,113);
	CR_BLACK = RGB(0,0,0);
}


VOID CTabSecondForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabSecondForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabSecondForm, CDialog)
	//{{AFX_MSG_MAP(CTabSecondForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSecondForm message handlers
BOOL CTabSecondForm::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CTabSecondForm::ResizeWindow(CRect rectParent)
{
	//자기 자신
	this->MoveWindow(rectParent);

	CWnd *pWnd = NULL;
	CRect rect;
	this->GetClientRect(rect);
	
	//그리드 static
	pWnd = (CWnd*)GetDlgItem(IDC_stcSubfield);
	if(pWnd->GetSafeHwnd() == NULL) return;

	rect.top += 10;
	rect.bottom -= 10;
	rect.left += 10;
	rect.right -= 10;	
	
	pWnd->MoveWindow(rect);	

	//그리드
	pWnd = (CWnd*)GetDlgItem(IDC_gridSubfield);
	if(pWnd->GetSafeHwnd() == NULL) return;
	
	rect.top += 15;
	rect.bottom -= 15;
	rect.left += 15;
	rect.right -= 15;	

	pWnd->MoveWindow(rect);	

}

BOOL CTabSecondForm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("마크레프_탭2") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 마크레프_탭2") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_마크레프_탭2"));
	m_pDM_SUBFIELD = FindDM(_T("DM_마크레프_식별기호"));
	m_pGridSubfield = (CESL_Grid*)m_pCM->FindControl(_T("식별기호그리드"));
	
	if(!m_pCM || !m_pDM_SUBFIELD)
	{
		AfxMessageBox( _T("reference Error : 마크레프_탭2") );
		return FALSE;		
	}

	m_pGridSubfield->m_bFroceReverse = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CTabSecondForm::SetAllData(CString strTagAlias)
{
	m_strTagAlias = strTagAlias;
}

VOID CTabSecondForm::ShowAllData()
{
	//식별기호
	CString strWhere;
	strWhere.Format(_T("MARC_TAG_ALIAS = '%s'"),m_strTagAlias);
	
	m_pDM_SUBFIELD->RefreshDataManager(strWhere);
	
	SetUdfField();
	
	DisplayGrid();

}

VOID CTabSecondForm::InsertSubfield()
{
	//태그 입력
	CSubfieldDlg dlg;
	dlg.m_nOpenMode = 1;
	dlg.m_strMARC_TAG_ALIAS = m_strTagAlias;
	dlg.m_pParentDM = m_pDM_SUBFIELD;
	dlg.m_pParentGrid = m_pGridSubfield;
	if(IDOK == dlg.DoModal())
	{
		DisplayGrid();
	}
}

VOID CTabSecondForm::ModifySubfield()
{
	INT nIdx;
	if(GetSelectedIdx(&nIdx)<0) return;
	

	CString strPK = m_pDM_SUBFIELD->GetCellData(_T("PK"),nIdx);

	//태그 수정
	CSubfieldDlg dlg;
	dlg.m_nOpenMode = -1;
	dlg.m_strPK = strPK;
	dlg.m_pParentDM = m_pDM_SUBFIELD;
	dlg.m_pParentGrid = m_pGridSubfield;
	dlg.m_nCurrentParentIdx = nIdx;
	if(IDOK == dlg.DoModal())
	{
		DisplayGrid();
	}
}

VOID CTabSecondForm::DeleteSubfield()
{
	INT nIdx;
	if(GetSelectedIdx(&nIdx)<0) return;

	CString strPK = m_pDM_SUBFIELD->GetCellData(_T("PK"),nIdx);
	CString strSubfieldCode = m_pDM_SUBFIELD->GetCellData(_T("SUBFIELD_CODE"),nIdx);
	CString strMsg;

	strMsg.Format(_T("[ %s ] 식별기호를 삭제 하시겠습니까?"),strSubfieldCode);
	if(AfxMessageBox(strMsg,MB_YESNO) != IDYES) return;

	CString strQuery;
    m_pDM_SUBFIELD->StartFrame();
    
	strQuery.Format(_T("DELETE FROM MARC_SUBFIELD_TBL WHERE PK = %s;"),strPK);
    m_pDM_SUBFIELD->AddFrame(strQuery);	
	
  	INT ids = m_pDM_SUBFIELD->SendFrame();
    m_pDM_SUBFIELD->EndFrame();

	if(ids<0)
	{
		AfxMessageBox(_T("삭제하던 중 에러가 발생하였습니다"));
	}
	else 
	{
		AfxMessageBox(_T("삭제하였습니다!"));
		m_pDM_SUBFIELD->DeleteRow(nIdx);
		DisplayGrid();
	}
}

INT CTabSecondForm::GetSelectedIdx(INT* nIdx)
{
	
	if(0 == m_pDM_SUBFIELD->GetRowCount())
	{
		AfxMessageBox(_T("현재 입력된 식별기호가 없습니다!"));
	    return -1;
	}

	m_pGridSubfield->SelectMakeList();

	INT nSelectedCnt = -1;
	
	nSelectedCnt  = m_pGridSubfield->SelectGetCount();

	if(0 == nSelectedCnt)
	{
		AfxMessageBox(_T("선정된 식별기호가 없습니다!"));
	    return -1;
	}

	if(nSelectedCnt>1)
	{
		AfxMessageBox(_T("한번에 하나의 식별기호만 선정할 수 있습니다!"));
	    return -1;
	}
	
	*nIdx = m_pGridSubfield->SelectGetHeadPosition();

	return 0;
}

VOID CTabSecondForm::SetUdfField()
{
	INT nRowCnt = m_pDM_SUBFIELD->GetRowCount();
	CString strRepeat,strEssential;

	for(INT i=0;i<nRowCnt;i++)
	{
		strRepeat = m_pDM_SUBFIELD->GetCellData(_T("REPEAT_TYPE"),i);
		strEssential = m_pDM_SUBFIELD->GetCellData(_T("ESSENTIAL_TYPE"),i);

		CString strTmp;

		if(_T("Y") == strRepeat)
		{
			strTmp  = _T("반복");
		}
		else if(_T("N") == strRepeat)
		{
			strTmp  = _T("반복불가");
		}

		m_pDM_SUBFIELD->SetCellData(_T("UDF_반복여부"),strTmp,i);


		if(_T("M") == strEssential)
		{
			strTmp = _T("필수");
		}
		else if(_T("A") == strEssential)
		{
			strTmp = _T("해당시필수");
		}
		else if(_T("O") == strEssential)
		{
			strTmp = _T("재량");
		}
		else if(_T("B") == strEssential)
		{
			strTmp = _T("공백");
		}

		m_pDM_SUBFIELD->SetCellData(_T("UDF_필수여부"),strTmp,i);
	}


}

VOID CTabSecondForm::DisplayGrid()
{
	m_pGridSubfield->Display();
	CString strUserDefine;

	for(INT i=0;i<m_pDM_SUBFIELD->GetRowCount();i++)
	{
		strUserDefine = m_pDM_SUBFIELD->GetCellData(_T("USER_DEFINE_YN"),i);
		if(_T("Y") == strUserDefine) SetGridRowUniqueColor(CR_user, i);

	}


}

INT CTabSecondForm::SetGridRowUniqueColor(COLORREF& colorref, INT nRowIdx)
{

	INT nCol = m_pGridSubfield->GetCol();
	INT nCols = m_pGridSubfield->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		m_pGridSubfield->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGridSubfield->SetCol(i);
			m_pGridSubfield->SetCellBackColor(colorref);
			m_pGridSubfield->SetCellForeColor(CR_BLACK);
		}
	}
	
	m_pGridSubfield->SetCol(nCol);
	
	return 0;
}

BEGIN_EVENTSINK_MAP(CTabSecondForm, CDialog)
    //{{AFX_EVENTSINK_MAP(CTabSecondForm)
	ON_EVENT(CTabSecondForm, IDC_gridSubfield, -600 /* Click */, OnClickgridSubfield, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CTabSecondForm::OnClickgridSubfield() 
{
	// TODO: Add your control notification handler code here
	INT nDMRow = m_pDM_SUBFIELD->GetRowCount();
	if(0 == nDMRow) return;

	INT row = m_pGridSubfield->GetRow();
	if (row == 0) return;
	if (row > 0) {

		if(m_nCurrentIdx>=0) {
			
			if(m_nCurrentIdx<nDMRow)
				SetGridRowBoldFont(m_nCurrentIdx,FALSE);
		}

		m_nCurrentIdx = row-1;
		SetGridRowBoldFont(m_nCurrentIdx,TRUE);
	
	}	
}

VOID CTabSecondForm::SetGridRowBoldFont(INT nRowIdx, BOOL bNewValue)
{
	INT nCol = m_pGridSubfield->GetCol();
	INT nCols = m_pGridSubfield->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		m_pGridSubfield->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			m_pGridSubfield->SetCol(i);
			m_pGridSubfield->SetCellFontBold(bNewValue);
		}

	}
	
	m_pGridSubfield->SetCol(nCol);
}