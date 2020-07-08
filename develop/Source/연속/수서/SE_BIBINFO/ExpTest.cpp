// ExpTest.cpp : implementation file
//

#include "stdafx.h"
#include "ExpTest.h"

#include "..\SE_API\SeExpectMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpTest dialog


CExpTest::CExpTest(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CExpTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpTest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pParentGrid = NULL;
	m_pParentDM = NULL;
	m_nACQ_TYPE = 1;
	m_strSpeciesKey = _T("");
	m_strPUB_FREQ = _T("");
}


VOID CExpTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpTest)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpTest, CDialog)
	//{{AFX_MSG_MAP(CExpTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpTest message handlers

BOOL CExpTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("연속_수서_테스트") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_테스트") );
		return false;
	}
	
	m_pCM = FindCM(_T("CM_연속_수서_테스트"));
	m_pDM = FindDM(_T("DM_체크인_예측"));
	

	if(!m_pCM || !m_pDM) 	{
		AfxMessageBox(_T("CM,DM Error! : CM_연속_수서_테스트"));
		return FALSE;
	}		
	
	if(!m_pParentGrid) {
		AfxMessageBox(_T("부모 그리드를 찾을 수 없습니다!"));
		return -1;
	}

	if(!m_pParentDM) {
		AfxMessageBox(_T("부모 DM을 찾을 수 없습니다!"));
		return -2;
	}
		
	INT nHead;
	m_pParentGrid->SelectMakeList();
	nHead = m_pParentGrid->SelectGetHeadPosition();
	if(nHead<0) {
		AfxMessageBox(_T("선정된 정보가 없습니다!"));
		return -3;
	}	
		
	m_strSpeciesKey = m_pParentDM->GetCellData(_T("ISS_종KEY"),nHead);
	m_strPUB_FREQ = m_pParentDM->GetCellData(_T("ISS_간행빈도"),nHead);

	if(!m_strSpeciesKey.IsEmpty()) ShowExpectedData();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CExpTest::ShowExpectedData()
{


}
