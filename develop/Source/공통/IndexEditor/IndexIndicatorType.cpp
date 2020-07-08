// IndexIndicatorType.cpp : implementation file
//

#include "stdafx.h"
#include "IndexIndicatorType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexIndicatorType dialog


CIndexIndicatorType::CIndexIndicatorType(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexIndicatorType)
	m_radIndicatorType = -1;
	m_bIndicatorAll = FALSE;
	m_bRepeat = FALSE;
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	
	m_strIndexGroup = _T("");
	m_strTag = _T("");
	m_strIndicator = _T("");
	m_strIndicatorType = _T("");
	m_strSingleOption = _T("");

	m_nOpenMode = 1; //default 입력
}


VOID CIndexIndicatorType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexIndicatorType)
	DDX_Control(pDX, IDC_listSecondIndicator, m_listSecondIndicator);
	DDX_Control(pDX, IDC_listFirstIndicator, m_listFirstIndicator);
	DDX_Radio(pDX, IDC_radIndTypeA, m_radIndicatorType);
	DDX_Check(pDX, IDC_chkIndicatorAll, m_bIndicatorAll);
	DDX_Check(pDX, IDC_chkIndicatorRepeat, m_bRepeat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexIndicatorType, CDialog)
	//{{AFX_MSG_MAP(CIndexIndicatorType)
	ON_BN_CLICKED(IDC_chkIndicatorAll, OnchkIndicatorAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexIndicatorType message handlers

BOOL CIndexIndicatorType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	
	if(InitESL_Mgr( _T("인덱스에디터_지시기호") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 인덱스에디터_지시기호") );
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_인덱스에디터_지시기호"));
	if(!m_pDM)
	{
		AfxMessageBox( _T("reference Error : DM_인덱스에디터_지시기호") );
		return FALSE;		
	}

	if(m_strIndexGroup.IsEmpty() || m_strTag.IsEmpty())
	{
		AfxMessageBox( _T("지시기호 상위 정보를 찾을 수 없습니다!") );
		return FALSE;	
	}


	if(ShowAllData()<0) return FALSE;



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexIndicatorType::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	if(!m_bIndicatorAll)
	{
		CString strFirst,strSecond;

		INT nCurSel = -1;
		nCurSel = m_listFirstIndicator.GetCurSel();
	
		if(nCurSel<0) 
		{
			AfxMessageBox(_T("제 1 지시기호가 선택되지 않았습니다!"));
			return;
		}

		m_listFirstIndicator.GetText(nCurSel,strFirst);

		nCurSel = m_listSecondIndicator.GetCurSel();
		
		if(nCurSel<0) 
		{
			AfxMessageBox(_T("제 2 지시기호가 선택되지 않았습니다!"));
			return;
		}

		m_listSecondIndicator.GetText(nCurSel,strSecond);
	

		m_strIndicator.Format(_T("%s%s"),strFirst,strSecond);
	}
	else
	{
		m_strIndicator = _T("AL");
	}


	switch(m_radIndicatorType)
	{	
	case 0 :
		m_strIndicatorType = _T("A");
		break;
	case 1 :
		m_strIndicatorType = _T("B");
		break;
	case 2 :
		m_strIndicatorType = _T("C");
		break;
	case 3 :
		m_strIndicatorType = _T("D");
		break;
	case 4 :
		m_strIndicatorType = _T("E");
		break;
	case 5 :
		m_strIndicatorType = _T("F");
		break;
	case 6 :
		m_strIndicatorType = _T("G");
		break;
	case 7 :
		m_strIndicatorType = _T("H");
		break;
	case 8 :
		m_strIndicatorType = _T("I");
		break;
	case 9 :
		m_strIndicatorType = _T("J");
		break;
	default :
		break;
	}

	if(m_bRepeat)
	{
		m_strSingleOption = _T("P");
	}
	else
	{
		m_strSingleOption = _T("S");
	}


	CDialog::OnOK();
}


INT CIndexIndicatorType::ShowAllData()
{
	CString strFirst,strSecond;
	
	if(m_nOpenMode<0)
	{
		if(2 != m_strIndicator.GetLength()) return -1;
		
		strFirst = m_strIndicator.GetAt(0);
		strSecond = m_strIndicator.GetAt(1);

		
		if(m_strIndicatorType.IsEmpty()) 
		{
			AfxMessageBox(_T("지시기호 유형을 찾을 수 없습니다!"));
			return -1;
		}
	}

	CString strTagAlias;

	strTagAlias.Format(_T("%s_%s"),m_strIndexGroup,m_strTag);
	
	//첫번째 지시기호 - 수정시에는 선정까지 한다.
	CString strWhere;
	strWhere.Format(_T("MARC_TAG_ALIAS = '%s'"),strTagAlias);
	m_pDM->TBL_NAME = _T("MARC_FIRST_INDICATOR_TBL");
	m_pDM->RefreshDataManager(strWhere);

	
	m_listFirstIndicator.ResetContent();
	INT nRow = m_pDM->GetRowCount();

	CString strIndCode;
	INT nCurSel = -1;
	for(INT i=0;i<nRow;i++)
	{
	    strIndCode =  m_pDM->GetCellData(_T("IND_CODE"),i);
		if(strIndCode.IsEmpty()) continue;

		if(strIndCode == strFirst) nCurSel = i;
		m_listFirstIndicator.InsertString(i,strIndCode);
	}
	
	if(m_nOpenMode<0) m_listFirstIndicator.SetCurSel(nCurSel); 
	
	

	//두번째 지시기호
	m_pDM->TBL_NAME = _T("MARC_FIRST_INDICATOR_TBL");
	m_pDM->RefreshDataManager(strWhere);

	m_listSecondIndicator.ResetContent();
	nRow = m_pDM->GetRowCount();
	nCurSel = -1;

	for(i=0;i<nRow;i++)
	{
	    strIndCode =  m_pDM->GetCellData(_T("IND_CODE"),i);
		if(strIndCode.IsEmpty()) continue;

		if(strIndCode == strSecond) nCurSel = i;
		m_listSecondIndicator.InsertString(i,strIndCode);
	}
	
	if(m_nOpenMode<0) m_listSecondIndicator.SetCurSel(nCurSel); 
	

	if(_T("AL") == m_strIndicator) 
	{
		m_bIndicatorAll = TRUE;
		m_listFirstIndicator.EnableWindow(FALSE);	
		m_listSecondIndicator.EnableWindow(FALSE);	
	}

	if(m_nOpenMode>0) m_strIndicatorType = _T("A"); //입력시 기본값
	
	switch(m_strIndicatorType.GetAt(0))
	{	
	case 'A' :
		m_radIndicatorType = 0;
		break;
	case 'B' :
		m_radIndicatorType = 1;
		break;
	case 'C' :
		m_radIndicatorType = 2;
		break;
	case 'D' :
		m_radIndicatorType = 3;
		break;
	case 'E' :
		m_radIndicatorType = 4;
		break;
	case 'F' :
		m_radIndicatorType = 5;
		break;
	case 'G' :
		m_radIndicatorType = 6;
		break;
	case 'H' :
		m_radIndicatorType = 7;
		break;
	case 'I' :
		m_radIndicatorType = 8;
		break;
	case 'J' :
		m_radIndicatorType = 9;
		break;
	default :
		break;
	}

	if(_T("S") == m_strSingleOption)
	{
		m_bRepeat = FALSE;
	}
	else
	{
		m_bRepeat = TRUE;
	}

	UpdateData(FALSE);


	return 0;
}

VOID CIndexIndicatorType::OnchkIndicatorAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	if(m_bIndicatorAll)
	{
		m_listFirstIndicator.EnableWindow(FALSE);	
		m_listSecondIndicator.EnableWindow(FALSE);	
	}
	else
	{
		m_listFirstIndicator.EnableWindow(TRUE);	
		m_listSecondIndicator.EnableWindow(TRUE);	

	}

}
