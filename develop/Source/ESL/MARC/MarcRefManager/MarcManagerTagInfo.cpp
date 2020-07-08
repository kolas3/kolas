// MarcManagerTagInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerTagInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerTagInfo dialog


CMarcManagerTagInfo::CMarcManagerTagInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerTagInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerTagInfo)
	m_nRadioEssential = -1;
	m_sTagName = _T("");
	m_sTagGroupCode = _T("");
	m_sTagExample = _T("");
	m_sTagDescrip = _T("");
	m_sTagCode = _T("");
	m_nRadioRepeat = -1;
	//}}AFX_DATA_INIT
   m_bModifyFlag = FALSE;

   // KOL.UDF.022
   m_pDM = NULL;
 
}


VOID CMarcManagerTagInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerTagInfo)
	DDX_Radio(pDX, IDC_rTagEssential1, m_nRadioEssential);
	DDX_Text(pDX, IDC_eTagName, m_sTagName);
	DDX_Text(pDX, IDC_eTagGroupCode, m_sTagGroupCode);
	DDX_Text(pDX, IDC_eTagExample, m_sTagExample);
	DDX_Text(pDX, IDC_eTagDescrip, m_sTagDescrip);
	DDX_Text(pDX, IDC_eTagCode, m_sTagCode);
	DDX_Radio(pDX, IDC_rTagRepeat1, m_nRadioRepeat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerTagInfo, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerTagInfo)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerTagInfo message handlers

BOOL CMarcManagerTagInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_bModifyFlag == true)
	{
		GetDlgItem(IDC_eTagCode)->EnableWindow(false);
    }
    
	GetDlgItem(IDC_eTagGroupCode)->EnableWindow(false);

	m_pDM = NULL;	
	m_pDM = new CESL_DataMgr;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMarcManagerTagInfo::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	
	if(m_sTagCode.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("태그 코드가 입력되지 않았습니다!"));
		return;
	}
	
	if(m_sTagName.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("태그 이름이 입력되지 않았습니다!"));
		return;
	}

	if(m_bModifyFlag == false)
	{
		ReadCfgFile.ReadCfgFile(_T(""));
		m_pDM->SetCONNECTION_INFO(ReadCfgFile.m_sConnection);
				
		CString query;
		query.Format(_T("SELECT PK FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s' AND TAG_CODE = '%s'"),m_sTagGroupCode,m_sTagCode);
    
		m_pDM->RestructDataManager(query);

		if(m_pDM->GetRowCount() != 0)
		{
			AfxMessageBox(_T("해당 그룹내에 동일한 코드가 있어서 입력 할 수 없습니다!"));
			return;
		}
   	}

	INT ids = 0;
	ids = CheckillegalSign(m_sTagCode);
	if(ids<0) return;
	
	ids = CheckillegalSign(m_sTagName);
	if(ids<0) return;

	ids = CheckillegalSign(m_sTagDescrip);
	if(ids<0) return;
	
	ids = CheckillegalSign(m_sTagExample);
	if(ids<0) return;

	CDialog::OnOK();
}

VOID CMarcManagerTagInfo::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	if(m_pDM) 
	{
       delete m_pDM;
       m_pDM = NULL;
	}
}

INT CMarcManagerTagInfo::CheckillegalSign(CString strData)
{
	INT pos = strData.Find(_T("*"));
	if(pos<0) return 0;
	
	AfxMessageBox(_T("* 기호는 사용할 수 없습니다. 확인 해 주십시요!"));

	return -1;
}
