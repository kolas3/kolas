// IndexInfo.cpp : implementation file
//

#include "stdafx.h"
#include "IndexInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndexInfo dialog


CIndexInfo::CIndexInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndexInfo)
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM_TAG = NULL;

    m_strTagIndexType.Format(_T("A - 특정 서브필드의 내용을 붙여서 색인 작성($a || $b || $f)\r\n\r\n")
				 _T("B - 특정 서브필드의 내용을 붙여서 색인 작성($a || $b || $x || $y || $z)\r\n\r\n")
                 _T("C - 특정 서브필드의 내용을 붙여서 색인 작성($a || $p || $x || $y || $z)\r\n\r\n")
                 _T("D - 특정 서브필드의 내용을 붙여서 색인 작성($a || $x || $y || $z)\r\n\r\n")
                 _T("E - 특정 서브필드 ($p)가 있는 경우 색인 2개 작성 ($a || $p, $p)\r\n\r\n")
                 _T("F - 특정 서브필드 ($b)가 있는 경우 색인 2개 작성 ($a || $b || $k || $g, $b || $k || $g)\r\n\r\n")
                 _T("G - 식별기호가 없는 태그"));                  
		


	m_strIndicatorIndexType.Format(_T("A - 그대로 색인생성\r\n\r\n")
				 _T("B - 원괄호를 제외하고 색인생성\r\n\r\n") 
				 _T("C - 처음이 ")_T("(")_T(" 로 시작되면 원괄호(하나의 괄호)의 내용을 삭제하고 색인생성\r\n\r\n")
				 _T("D - 원괄호를 포함해서 색인 생성\r\n\r\n")
				 _T("E - 색인 2개 작성 (원괄호를 제외하고 색인작성 + 원괄호를 포함해서 색인작성)\r\n\r\n")
				 _T("F - 표출어 제외하고 색인작성, 원괄호를 제외하고 색인 생성\r\n\r\n")
				 _T("G - 표출어 제외하고 색인작성, 원괄호를 포함하고 색인 생성\r\n\r\n")
				 _T("H - 표출어 제외하고 색인작성, 색인 2개 작성(원괄호 제외,포함)\r\n\r\n")
				 _T("I - 역활어를 제외하고 색인작성\r\n\r\n")
				 _T("J - 색인화 하지 않음\r\n\r\n"));

}


VOID CIndexInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndexInfo)
	DDX_Control(pDX, IDC_tabIndexTypeInfo, m_tabIndexInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndexInfo, CDialog)
	//{{AFX_MSG_MAP(CIndexInfo)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabIndexTypeInfo, OnSelchangetabIndexTypeInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndexInfo message handlers

BOOL CIndexInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("인덱스에디터_색인정보") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 인덱스에디터_색인정보") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_인덱스에디터_색인정보"));
	m_pDM_TAG = FindDM(_T("DM_인덱스에디터_태그"));


	if(!m_pCM || !m_pDM_TAG)
	{
		AfxMessageBox( _T("reference Error : CM_인덱스에디터_색인정보,DM_인덱스에디터_태그") );
		return FALSE;		
	}


	m_tabIndexInfo.InsertItem(0,_T("태그 색인유형"));
	m_tabIndexInfo.InsertItem(1,_T("지시기호 색인유형"));
	m_tabIndexInfo.SetCurSel(0);
	m_pCM->SetControlMgrData(_T("색인유형설명"),m_strTagIndexType);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CIndexInfo::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}


BOOL CIndexInfo::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CIndexInfo::ResizeWindow(CRect rectParent)
{
	
	//자기 자신
	rectParent.left = rectParent.left + 455;
	//rectParent.left = rectParent.right - 450;
	rectParent.right -= 5;
	
	this->MoveWindow(rectParent);

	CWnd *pWnd = NULL;

	CRect rect,tmprect;
	this->GetClientRect(rect);
		
	tmprect = rect;

	//tab 색인 유형
	pWnd = (CWnd*)GetDlgItem(IDC_tabIndexTypeInfo);
	if(pWnd->GetSafeHwnd() == NULL) return;
	
	tmprect.top += 10;
	tmprect.bottom = tmprect.top + 230;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	
	

	//edit 색인 유형
	pWnd = (CWnd*)GetDlgItem(IDC_edtIndexTypeDesc);
	if(pWnd->GetSafeHwnd() == NULL) return;

	tmprect.top += 25;
	tmprect.bottom -= 10;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	

	tmprect = rect;

	//static 태그 설명
	pWnd = (CWnd*)GetDlgItem(IDC_stcTagInfo);
	if(pWnd->GetSafeHwnd() == NULL) return;
	
	tmprect.top += 250;
	tmprect.bottom -= 10;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	
	

	//edit 태그 설명
	pWnd = (CWnd*)GetDlgItem(IDC_edtTagDesc);
	if(pWnd->GetSafeHwnd() == NULL) return;

	tmprect.top += 25;
	tmprect.bottom -= 10;
	tmprect.left += 10;
	tmprect.right -= 10;	

	pWnd->MoveWindow(tmprect);	

}

VOID CIndexInfo::OnSelchangetabIndexTypeInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	INT nCursel = m_tabIndexInfo.GetCurSel();
	
	CString strMsg;

	if(0 == nCursel)
	{
		strMsg = m_strTagIndexType;
	}
	else if(1 == nCursel)
	{
		strMsg = m_strIndicatorIndexType;
	}

	m_pCM->SetControlMgrData(_T("색인유형설명"),strMsg);

	*pResult = 0;
}

VOID CIndexInfo::ShowTagDesc(CString strGroup, CString strTagCode)
{
	if(strGroup.IsEmpty() || strTagCode.IsEmpty()) return;

	CString strQuery,strResult;
	strQuery.Format(_T("SELECT DESCRIP FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s' AND TAG_CODE = '%s'"),
					strGroup,strTagCode);
	
	m_pDM_TAG->GetOneValueQuery(strQuery,strResult);
	m_pCM->SetControlMgrData(_T("태그설명"),strResult);

	
	CString strMsg;
	strMsg.Format(_T("[  %s  ]"),strTagCode);
	GetDlgItem(IDC_stcTagInfo)->SetWindowText(strMsg);
}

VOID CIndexInfo::ShowIndexTypeDesc(INT nType)
{
	m_tabIndexInfo.SetCurSel(nType);
	UpdateData(FALSE);

	
	CString strMsg;

	if(0 == nType)
	{
		strMsg = m_strTagIndexType;
	}
	else if(1 == nType)
	{
		strMsg = m_strIndicatorIndexType;
	}

	m_pCM->SetControlMgrData(_T("색인유형설명"),strMsg);
	
}