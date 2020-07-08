// BO_ACQ_CheckInDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_CheckInDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_CheckInDlg dialog


CBO_ACQ_CheckInDlg::CBO_ACQ_CheckInDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_CheckInDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_CheckInDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGroupCollection = NULL;
}
CBO_ACQ_CheckInDlg::~CBO_ACQ_CheckInDlg()
{
}

VOID CBO_ACQ_CheckInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_CheckInDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_CheckInDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_CheckInDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_CheckInDlg message handlers

BOOL CBO_ACQ_CheckInDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_주문자료검수완료")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_주문자료검수완료"));
	
	if(!Init())
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_CheckInDlg::SetTotalInfo()
{
	CString strTotalPrice, strTotalBookCnt;
	CString strBookKeyList;
	CString strSelect;
	CString strTmp;	
	
	CPriceInfoConversionMgr Convert;
	CESL_DataMgr *pBookDM;
	CESL_DataMgr tmpDM;

	pBookDM = FindDM(_T("DM_주문자료관리"));
	if ( pBookDM == NULL ) return;

	strBookKeyList = _T("");
	for ( INT i=0 ; i<pBookDM->GetRowCount() ; i++ )
	{		
		strSelect = pBookDM->GetCellData(_T("선정"), i );
		if ( strSelect == _T("V") )
		{
			strTmp = pBookDM->GetCellData(_T("BB_책KEY"), i );
			if ( !strBookKeyList.IsEmpty() ) strBookKeyList += _T(",");
			strBookKeyList += strTmp;
		}		
	}

	tmpDM.SetCONNECTION_INFO(this->CONNECTION_INFO);
	m_pGroupCollection->m_strSelectBookKey = strBookKeyList;
	m_pGroupCollection->GetBookTotalInfo(&tmpDM, strTotalPrice, strTotalBookCnt);

	// 2006.03.09 KBS - 가격에 콤마찍어준다.
	Convert.ConversionPriceInfoToDesc(strTotalPrice, strTotalPrice);
	m_pCM->SetControlMgrData(_T("총책수"), strTotalBookCnt);
	m_pCM->SetControlMgrData(_T("총가격"), strTotalPrice);

	CString strDeliveryBookCnt, strNonDeliveryBookCnt;
	m_pGroupCollection->GetDeliveryRelativeBookCnt(&tmpDM, strDeliveryBookCnt, strNonDeliveryBookCnt);

	m_pCM->SetControlMgrData(_T("납품책수"), strDeliveryBookCnt);
	m_pCM->SetControlMgrData(_T("미납책수"), strNonDeliveryBookCnt);
}

BOOL CBO_ACQ_CheckInDlg::Init()
{
	if(m_pGroupCollection == NULL) 
	{
		ESLAfxMessageBox(_T("차수정보가 올바르지 않습니다."));
		return FALSE;
	}
	m_pCM->SetControlMgrData(_T("수입년도"), m_pGroupCollection->GetAcqYear());
	CString strGroupInfo;
	m_pGroupCollection->GetGroupInfo(strGroupInfo);
	m_pCM->SetControlMgrData(_T("차수번호"), strGroupInfo);
	m_pCM->SetControlMgrData(_T("업무구분"), m_pGroupCollection->GetWorkCode());
	SetTotalInfo();
	return TRUE;
}

VOID CBO_ACQ_CheckInDlg::OnOK() 
{
	CDialog::OnOK();
}

VOID CBO_ACQ_CheckInDlg::SetGroupCollection(CGroupCollection *pGroupCollection)
{
	m_pGroupCollection = pGroupCollection;
}

HBRUSH CBO_ACQ_CheckInDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}