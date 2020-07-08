// IndexTagInsert.cpp : implementation file
//

#include "stdafx.h"
#include "IndexTagInsert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexTagInsert dialog


CIndexTagInsert::CIndexTagInsert(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexTagInsert)
	m_bTagNoType = FALSE;
	m_nTagType = -1;
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pGrid = NULL;

	m_strIndexGroup = _T("");
	m_strTagType = _T("");
	m_strTag = _T("");
	m_nCurrentIdx = -1;

}


VOID CIndexTagInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexTagInsert)
	DDX_Check(pDX, IDC_chkTagInsertNoType, m_bTagNoType);
	DDX_Radio(pDX, IDC_radTagInsertA, m_nTagType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexTagInsert, CDialog)
	//{{AFX_MSG_MAP(CIndexTagInsert)
	ON_BN_CLICKED(IDC_btnTagSearch, OnbtnTagSearch)
	ON_BN_CLICKED(IDC_chkTagInsertNoType, OnchkTagInsertNoType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexTagInsert message handlers

BOOL CIndexTagInsert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("인덱스에디터_태그입력") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 인덱스에디터_태그입력") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_인덱스에디터_태그입력"));
	m_pDM = FindDM(_T("DM_인덱스에디터_태그입력"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	

	if(!m_pCM || !m_pDM)
	{
		AfxMessageBox( _T("reference Error : CM_인덱스에디터_태그입력,DM_인덱스에디터_태그입력") );
		return FALSE;		
	}
	
	m_bTagNoType = TRUE;
	EnableRadioCtrl(FALSE);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexTagInsert::OnbtnTagSearch() 
{
	// TODO: Add your control notification handler code here
	
	CString strWhere;
	CString strTmp;
	CString strTagCode,strTagName;
	m_pCM->GetControlMgrData(_T("코드"),strTagCode);
	m_pCM->GetControlMgrData(_T("이름"),strTagName);
	
       
	strTmp.Format(_T("MARC_GROUP_CODE = '%s'"),m_strIndexGroup);
	strWhere += strTmp;
	
	
	if(!strTagCode.IsEmpty())
	{
		strTmp.Format(_T(" AND TAG_CODE like '%%%s%%' "),strTagCode);
		strWhere += strTmp;
	}
		
	if(!strTagName.IsEmpty() )
	{
		strTmp.Format(_T("AND TAG_NAME like '%%%s%%' "),strTagName);
		strWhere += strTmp;
	}    
	
    
	m_pDM->RefreshDataManager(strWhere);
	m_pGrid->Display();

	INT nRow = m_pDM->GetRowCount();

	if(nRow>0) m_nCurrentIdx = 0;
	else m_nCurrentIdx = -1;	
	
}

VOID CIndexTagInsert::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);


	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선정된 태그가 없습니다!"));
		return;
	}

	m_strTag = m_pDM->GetCellData(_T("TAG_CODE"),m_nCurrentIdx);

	if(m_bTagNoType)
	{
		m_strTagType = _T("");
	}
	else
	{	
		if(m_nTagType<0) 
		{
			AfxMessageBox(_T("태그 유형이 설정되지 않았습니다!"));
			return;
		}
		
		switch(m_nTagType)
		{	
		case 0 :
			m_strTagType = _T("A");
			break;
		case 1 :
			m_strTagType = _T("B");
			break;
		case 2 :
			m_strTagType = _T("C");
			break;
		case 3 :
			m_strTagType = _T("D");
			break;
		case 4 :
			m_strTagType = _T("E");
			break;
		case 5 :
			m_strTagType = _T("F");
			break;
		case 6 :
			m_strTagType = _T("G");
			break;
		default :
			break;
		}
		
	}


	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CIndexTagInsert, CDialog)
    //{{AFX_EVENTSINK_MAP(CIndexTagInsert)
	ON_EVENT(CIndexTagInsert, IDC_gridTag, -600 /* Click */, OnClickgridTag, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CIndexTagInsert::OnClickgridTag() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}

VOID CIndexTagInsert::EnableRadioCtrl(BOOL bEnable)
{

	if(bEnable)
	{
		GetDlgItem(IDC_radTagInsertA)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagInsertB)->EnableWindow(TRUE);	
		GetDlgItem(IDC_radTagInsertC)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagInsertD)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagInsertE)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagInsertF)->EnableWindow(TRUE);
		GetDlgItem(IDC_radTagInsertG)->EnableWindow(TRUE);
	}
	else 
	{

		GetDlgItem(IDC_radTagInsertA)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagInsertB)->EnableWindow(FALSE);	
		GetDlgItem(IDC_radTagInsertC)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagInsertD)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagInsertE)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagInsertF)->EnableWindow(FALSE);
		GetDlgItem(IDC_radTagInsertG)->EnableWindow(FALSE);

	}


}

VOID CIndexTagInsert::OnchkTagInsertNoType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_bTagNoType)
	{
		EnableRadioCtrl(FALSE);
	}
	else
	{
		EnableRadioCtrl(TRUE);
	}
}
