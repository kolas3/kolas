// SeFileImportMarcEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SeFileImportMarcEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeFileImportMarcEdit dialog


CSeFileImportMarcEdit::CSeFileImportMarcEdit(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeFileImportMarcEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeFileImportMarcEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	AfxInitRichEdit();
	m_nCurrentParentIdx = -1;
	m_nType = 1; //default 
	m_Marc.Init();
	m_pDM_OK = NULL;
	m_pDM_ERR = NULL;
	m_pGrid_ERR = NULL;
	m_pGrid_OK = NULL;

}
CSeFileImportMarcEdit::~CSeFileImportMarcEdit()
{
}

VOID CSeFileImportMarcEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeFileImportMarcEdit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeFileImportMarcEdit, CDialog)
	//{{AFX_MSG_MAP(CSeFileImportMarcEdit)
	ON_BN_CLICKED(IDC_btnFILE_MARC_NEXT, OnbtnFILEMARCNEXT)
	ON_BN_CLICKED(IDC_btnFILE_MARC_PREV, OnbtnFILEMARCPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeFileImportMarcEdit message handlers

BOOL CSeFileImportMarcEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	if(1 == m_nType)
	{
		m_pGrid_OK->SelectMakeList();
		m_nCurrentParentIdx = m_pGrid_OK->SelectGetHeadPosition();
	}
	else if(2 == m_nType)
	{
		m_pGrid_ERR->SelectMakeList();
		m_nCurrentParentIdx = m_pGrid_ERR->SelectGetHeadPosition();
	}

	if(m_nCurrentParentIdx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		EndDialog(0);
		return FALSE;
	}
	

	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_marcEditor.SubclassDlgItem(IDC_richeditFILE_MARC, -1, this);
	m_marcEditor.SetMarcMgr( m_pInfo->pMarcMgr );

	RefreshAll();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CSeFileImportMarcEdit::RefreshAll()
{
	CString strStreamMarc;
	

	if(1 == m_nType)
		strStreamMarc = m_pDM_OK->GetCellData(_T("MARC"),m_nCurrentParentIdx);
	else if(2 == m_nType)
		strStreamMarc = m_pDM_ERR->GetCellData(_T("MARC"),m_nCurrentParentIdx);


	m_pInfo->pMarcMgr->Decoding(strStreamMarc,&m_Marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&m_Marc);

	m_marcEditor.Init( m_pInfo->pMarcMgr, &m_Marc );
	m_marcEditor.Display();

}

VOID CSeFileImportMarcEdit::OnOK() 
{
	// TODO: Add extra validation here
	
	INT ids;
	BOOL bIsError = FALSE;
	
	// 문법 체크
	CString strTagCode;
	CString strErrMsg;
	CString strHelpMsg;
	CString strAllErrorMsg = _T("");;
	CString strTemp;
	
	ids = m_pInfo->pMarcMgr->CheckSimpleSyntax(&m_Marc);
	if (ids < 0) 
	{
		POSITION pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		if(pos)
		{
			strTagCode = _T("");
			strErrMsg = _T("");
			strHelpMsg = _T("");

			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			strTemp.Format(_T("[%s] : %s - %s\n"), strTagCode, strErrMsg, strHelpMsg);
			strAllErrorMsg += strTemp;
		}

		bIsError = TRUE;
	}
	
	CString msg;
	m_marcEditor.ApplyEdit(0);	
	INT nRowIdx;
	CString strStreamMarc;

	if(bIsError)
	{
		if(1 == m_nType) //정상탭
		{
			msg.Format(_T("수정 사항에 오류가 존재 합니다! \n 적용할 경우 오류항목으로 이동합니다 \n 다시 오류를 수정하시겠습니까?"));
			if(AfxMessageBox(msg,MB_YESNO) == IDYES)
			{
				//RefreshAll();
				return;
			}
			else //오류 탭으로 이동
			{
				m_pDM_ERR->InsertRow(-1);
				nRowIdx = m_pDM_ERR->GetRowCount()-1;
				CopyDMToDMByAlias(m_pDM_OK,m_nCurrentParentIdx,m_pDM_ERR,nRowIdx);
				m_pDM_ERR->SetCellData(_T("UDF_파일반입오류"),strAllErrorMsg,nRowIdx);
				m_pInfo->pMarcMgr->Encoding(&m_Marc,strStreamMarc);
				m_pDM_ERR->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
				m_pDM_OK->DeleteRow(m_nCurrentParentIdx);

				m_pGrid_OK->Display();
				m_pGrid_ERR->Display(nRowIdx);
				return;
			}

		}
		else if(2 == m_nType) //오류탭
		{
			AfxMessageBox(_T("아직 오류가 존재 합니다. 확인 하십시요!"));
			
			m_pInfo->pMarcMgr->Encoding(&m_Marc,strStreamMarc);
			m_pDM_ERR->SetCellData(_T("MARC"),strStreamMarc,m_nCurrentParentIdx);
			m_pDM_ERR->SetCellData(_T("UDF_파일반입오류"),strAllErrorMsg,m_nCurrentParentIdx);
			m_pGrid_ERR->Display(m_nCurrentParentIdx);
			return;
		}

	}
	else
	{	
		if(1 == m_nType) //정상탭
		{
			AfxMessageBox(_T("적용되었습니다!"));

			m_pInfo->pMarcMgr->Encoding(&m_Marc,strStreamMarc);
			m_pDM_OK->SetCellData(_T("MARC"),strStreamMarc,m_nCurrentParentIdx);
			m_pGrid_OK->Display(m_nCurrentParentIdx);
			return;

		}
		else if(2 == m_nType) //정상탭으로 이동
		{
			AfxMessageBox(_T("오류가 없어 정상탭으로 이동하였습니다!"));
			
			m_pDM_OK->InsertRow(-1);
			nRowIdx = m_pDM_OK->GetRowCount()-1;
			CopyDMToDMByAlias(m_pDM_ERR,m_nCurrentParentIdx,m_pDM_OK,nRowIdx);
			m_pInfo->pMarcMgr->Encoding(&m_Marc,strStreamMarc);
			m_pDM_OK->SetCellData(_T("MARC"),strStreamMarc,nRowIdx);
			m_pDM_ERR->DeleteRow(m_nCurrentParentIdx);

			m_pGrid_ERR->Display();
			m_pGrid_OK->Display(nRowIdx);
			return;
		}

	}

	//CDialog::OnOK();
}

VOID CSeFileImportMarcEdit::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CSeFileImportMarcEdit::OnbtnFILEMARCNEXT() 
{
	// TODO: Add your control notification handler code here

	if(1 == m_nType)
	{
		INT nPos = m_pGrid_OK->SelectGetNext();
		if(nPos<0) {
			m_pGrid_OK->SelectGetTailPosition();
			AfxMessageBox(_T("맨 마지막 항목 입니다!"));
			return;
		}
		
		m_nCurrentParentIdx = nPos;
		m_pGrid_OK->SetReverse(m_nCurrentParentIdx);

	}
	else if(2 == m_nType)
	{
		INT nPos = m_pGrid_ERR->SelectGetNext();

		if(nPos<0) {
			m_pGrid_ERR->SelectGetTailPosition();
			AfxMessageBox(_T("맨 마지막 항목 입니다!"));
			return;
		}
		
		m_nCurrentParentIdx = nPos;
		m_pGrid_ERR->SetReverse(m_nCurrentParentIdx);
	}
	
	RefreshAll();
}

VOID CSeFileImportMarcEdit::OnbtnFILEMARCPREV() 
{
	// TODO: Add your control notification handler code here
	if(1 == m_nType)
	{
		INT nPos = m_pGrid_OK->SelectGetPrev();
		if(nPos<0) {
			m_pGrid_OK->SelectGetHeadPosition();
			AfxMessageBox(_T("맨 처음 항목 입니다!"));
			return;
		}
		
		m_nCurrentParentIdx = nPos;
		m_pGrid_OK->SetReverse(m_nCurrentParentIdx);

	}
	else if(2 == m_nType)
	{
		INT nPos = m_pGrid_ERR->SelectGetPrev();

		if(nPos<0) {
			m_pGrid_ERR->SelectGetHeadPosition();
			AfxMessageBox(_T("맨 처음 항목 입니다!"));
			return;
		}
		
		m_nCurrentParentIdx = nPos;
		m_pGrid_ERR->SetReverse(m_nCurrentParentIdx);
	}
	
	RefreshAll();
}

INT CSeFileImportMarcEdit::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}
HBRUSH CSeFileImportMarcEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
