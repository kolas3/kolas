// MarcManagerSubfield.cpp : implementation file
//

#include "stdafx.h"
#include "MarcManager.h"
#include "MarcManagerSubfield.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerSubfield dialog


CMarcManagerSubfield::CMarcManagerSubfield(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcManagerSubfield::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcManagerSubfield)
	m_sSubfieldCode = _T("");
	m_sSubfieldDescrip = _T("");
	m_sSubfiledPunc = _T("");
	m_nRadioEssential = -1;
	m_nRadioRepeat = -1;
	//}}AFX_DATA_INIT

}


VOID CMarcManagerSubfield::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcManagerSubfield)
	DDX_Text(pDX, IDC_eSubfieldCode, m_sSubfieldCode);
	DDX_Text(pDX, IDC_eSubfieldDescrip, m_sSubfieldDescrip);
	DDX_Text(pDX, IDC_eSubfieldPunc, m_sSubfiledPunc);
	DDX_Radio(pDX, IDC_rSubfieldEssential1, m_nRadioEssential);
	DDX_Radio(pDX, IDC_rSubfieldRepeat1, m_nRadioRepeat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarcManagerSubfield, CDialog)
	//{{AFX_MSG_MAP(CMarcManagerSubfield)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcManagerSubfield message handlers

VOID CMarcManagerSubfield::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	
	if(m_sSubfieldCode.IsEmpty())
	{
		AfxMessageBox(_T("코드가 입력되지 않았습니다!"));
	    return;
	}
	
	INT ids = 0;
	ids = CheckillegalSign(m_sSubfieldCode);
	if(ids<0) return;
	
	ids = CheckillegalSign(m_sSubfieldDescrip);
	if(ids<0) return;

	ids = CheckillegalSign(m_sSubfiledPunc);
	if(ids<0) return;

	CDialog::OnOK();
}


INT CMarcManagerSubfield::CheckillegalSign(CString strData)
{
	INT pos = strData.Find(_T("*"));
	if(pos<0) return 0;
	
	AfxMessageBox(_T("* 기호는 사용할 수 없습니다. 확인 해 주십시요!"));

	return -1;
}
