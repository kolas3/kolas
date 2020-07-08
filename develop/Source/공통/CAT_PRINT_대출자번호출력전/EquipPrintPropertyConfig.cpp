// EquipPrintPropertyConfig.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "EquipPrintPropertyConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyConfig dialog


CEquipPrintPropertyConfig::CEquipPrintPropertyConfig(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CEquipPrintPropertyConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// KOL.UDF.022 시큐어코딩 보완
	m_pCM = NULL;
}


VOID CEquipPrintPropertyConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquipPrintPropertyConfig)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEquipPrintPropertyConfig, CDialog)
	//{{AFX_MSG_MAP(CEquipPrintPropertyConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyConfig message handlers

BOOL CEquipPrintPropertyConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (InitESL_Mgr(_T("SM_정리_출력_장비_속성_낱장")) < 0) 
	{
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_정리_출력_장비_속성_낱장"));
	
	if ( m_pCM == NULL)
		return FALSE;


	CString sUnit;
	CString m_sUnit;
	
	CComboBox * pRegCode;	
	CString sRegCode;
	pRegCode=(CComboBox *)GetDlgItem(IDC_CobUnit);

	pRegCode->AddString(_T("픽셀(pixel)"));
	pRegCode->AddString(_T("밀리미터(mm)"));
	if ( sUnit == _T("밀리미터(mm)") ) {
		pRegCode->SetCurSel(0);
		m_sUnit = _T("밀리미터(mm)");
	}
	else {
		pRegCode->SetCurSel(1);
		m_sUnit = _T("픽셀(pixel)");
	}

	


/*
	CPrintPropertyInfo *pInfo = NULL;

	// 찾아서 없으면 생성한다.
	if (!m_mapPrintPropertyInfo.Lookup(m_strPrintFormatKey, (VOID*&)pInfo))
	{
		pInfo = new CPrintPropertyInfo;
		m_mapPrintPropertyInfo.SetAt(m_strPrintFormatKey, pInfo);
	}

	if (!pInfo) return -1;

	// 프로퍼티를 가져와서 보여준다.
	CString strData;
	
	strData.Format(_T("%d"), pInfo->m_nMarginUp);
	m_pCM->SetControlMgrData(_T("위쪽여백"), strData);

	strData.Format(_T("%d"), pInfo->m_nMarginSide);
	m_pCM->SetControlMgrData(_T("양쪽여백"), strData);

	strData.Format(_T("%d"), pInfo->m_nSpaceVert);
	m_pCM->SetControlMgrData(_T("세로간격"), strData);

	strData.Format(_T("%d"), pInfo->m_nSpaceHorz);
	m_pCM->SetControlMgrData(_T("가로간격"), strData);

	strData.Format(_T("%d"), pInfo->m_nLabelHight);
	m_pCM->SetControlMgrData(_T("레이블높이"), strData);

	strData.Format(_T("%d"), pInfo->m_nLabelWidth);
	m_pCM->SetControlMgrData(_T("레이블너비"), strData);

	strData.Format(_T("%d"), pInfo->m_nRowCount);
	m_pCM->SetControlMgrData(_T("가로개수"), strData);

	strData.Format(_T("%d"), pInfo->m_nColCount);
	m_pCM->SetControlMgrData(_T("세로개수"), strData);

	strData.Format(_T("%d"), pInfo->m_nMarginBarCode);
	m_pCM->SetControlMgrData(_T("바코드여백"), strData);

	// 폰트
	CString strFont;
	strFont.Format(_T("%s, %d"), pInfo->m_strFontFace, pInfo->m_nFontSize);
	GetDlgItem(IDC_bFONT)->SetWindowText(strFont);	
*/	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
