// IndexSubfield.cpp : implementation file
//

#include "stdafx.h"
#include "IndexSubfield.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexSubfield dialog


CIndexSubfield::CIndexSubfield(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexSubfield)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;

	m_strIndexGroup = _T("");
	m_strTag = _T("");
	m_strSubfield = _T("");

	m_nOpenMode = 1; //default

}


VOID CIndexSubfield::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexSubfield)
	DDX_Control(pDX, IDC_listSubfield, m_listSubfield);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexSubfield, CDialog)
	//{{AFX_MSG_MAP(CIndexSubfield)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexSubfield message handlers

BOOL CIndexSubfield::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("인덱스에디터_식별기호") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 인덱스에디터_식별기호") );
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_인덱스에디터_식별기호"));
	if(!m_pDM)
	{
		AfxMessageBox( _T("reference Error : DM_인덱스에디터_식별기호") );
		return FALSE;		
	}

	if(m_strIndexGroup.IsEmpty() || m_strTag.IsEmpty())
	{
		AfxMessageBox( _T("식별기호 상위 정보를 찾을 수 없습니다!") );
		return FALSE;	
	}
	


	if(ShowAllData()<0) return FALSE;



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CIndexSubfield::ShowAllData()
{
	if(m_nOpenMode<0)
	{
		if(m_strSubfield.IsEmpty()) 
		{
			AfxMessageBox(_T("기존 식별기호 정보를 찾을 수 없습니다!"));
			return -1;
		}
	}

	CString strTagAlias;

	strTagAlias.Format(_T("%s_%s"),m_strIndexGroup,m_strTag);
	
	CString strWhere;
	strWhere.Format(_T("MARC_TAG_ALIAS = '%s'"),strTagAlias);
	m_pDM->RefreshDataManager(strWhere);

	
	m_listSubfield.ResetContent();
	INT nRow = m_pDM->GetRowCount();

	CString strSubfield;
	INT nCurSel = -1;
	for(INT i=0;i<nRow;i++)
	{
	    strSubfield =  m_pDM->GetCellData(_T("SUBFIELD_CODE"),i);
		if(strSubfield.IsEmpty()) continue;

		if(strSubfield == m_strSubfield) nCurSel = i;
		m_listSubfield.InsertString(i,strSubfield);
	}

	m_listSubfield.SetCurSel(nCurSel); 


	return 0;
}


VOID CIndexSubfield::OnOK() 
{
	// TODO: Add extra validation here
	
	UpdateData(TRUE);

	INT nCurSel = -1;
	nCurSel = m_listSubfield.GetCurSel();
	m_listSubfield.GetText(nCurSel,m_strSubfield);


	CDialog::OnOK();
}
