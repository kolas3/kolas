// CheckinReceiptBookInfo.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinReceiptBookInfo.h"
#include "SeApi.h"
#include "../../../ESL/SIReportMgr/SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinReceiptBookInfo dialog


CCheckinReceiptBookInfo::CCheckinReceiptBookInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinReceiptBookInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strAcqCode = _T("");
	m_strAcqYear = _T("");
	m_strReceiptNo = _T("");
	m_nACQ_TYPE = 1; //defaut
}
CCheckinReceiptBookInfo::~CCheckinReceiptBookInfo()
{
}

VOID CCheckinReceiptBookInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinReceiptBookInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinReceiptBookInfo, CDialog)
	//{{AFX_MSG_MAP(CCheckinReceiptBookInfo)
	ON_BN_CLICKED(IDC_btnPRINT, OnbtnPRINT)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinReceiptBookInfo message handlers

BOOL CCheckinReceiptBookInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if(InitESL_Mgr(_T("연속_수서_체크인_접수번호_책정보"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_체크인_접수번호_책정보") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_체크인_접수번호_책정보"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_체크인_접수번호_책정보"));
	

	if(!m_pDM || !m_pCM || !m_pGrid) {
		AfxMessageBox( _T("DM,CM,grid Error : CM_체크인_접수번호, DM_체크인_접수번호") );
		return false;
	}



	CString strACQ_TYPE;
	if(1 == m_nACQ_TYPE) m_strAcqCode = _T("구입");
	else m_strAcqCode = _T("기증");

	if(m_strAcqYear.IsEmpty() || m_strReceiptNo.IsEmpty())
	{
		AfxMessageBox( _T("수입년도 혹은 접수번호가 입력되지 않았습니다!") );
		return false;
	}

	m_pCM->SetControlMgrData(_T("수입구분"),m_strAcqCode);   
	m_pCM->SetControlMgrData(_T("수입년도"),m_strAcqYear);  
	m_pCM->SetControlMgrData(_T("접수번호"),m_strReceiptNo); 

	
	CString strQuery,strTmpQuery;

	strQuery += _T("I.REC_KEY = B.SPECIES_KEY");
	strQuery += _T(" AND V.REC_KEY = B.VOL_KEY");
	strQuery += _T(" AND A.REC_KEY = B.ACQ_TYPE_VOL_KEY");
	strQuery += _T(" AND B.BOOK_TYPE = 'C'");

	strTmpQuery.Format(_T(" AND B.ACQ_CODE = '%d' AND B.ACQ_YEAR = '%s' AND B.RECEIPT_NO = %s")
					,m_nACQ_TYPE,m_strAcqYear,m_strReceiptNo);
	
	strQuery  += strTmpQuery;


	m_pDM->RefreshDataManager(strQuery);

	INT nRow = 0;
	nRow = m_pDM->GetRowCount();
	if(0 == nRow)
	{
		AfxMessageBox(_T("해당 접수번호내에 책정보가 없습니다!"));
		EndDialog(0);
		return FALSE;
	}
	
	CString strCnt;
	strCnt.Format(_T("%d"),nRow);
	m_pCM->SetControlMgrData(_T("책수"),strCnt); 


	//code to descript
	//자료실 
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
	
	//자료상태
	CSeApi api(this);
	CString strWorkingStatus,strDesc;

	for(INT i=0;i<nRow;i++)
	{
		strWorkingStatus = m_pDM->GetCellData(_T("SB_자료상태"),i);
		strDesc = api.GetWorkingStatusDesc(strWorkingStatus);
		m_pDM->SetCellData(_T("UDF_자료상태"),strDesc,i);
	}


	m_pGrid->Display();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//2005-12-23 황정영 : 목록출력 추가
VOID CCheckinReceiptBookInfo::OnbtnPRINT() 
{
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return;
	}

	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속_접수번호별_책목록"));
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return;
	}
	
	//=====================================================
	//2009.01.29 REM BY PJW : 엑셀을 사용함으로 필요 없어진 코드임
// 	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	//=====================================================
	
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	pSIReportManager->SetTempVariable(_T("수입구분"),m_strAcqCode);
	pSIReportManager->SetTempVariable(_T("수입년도"),m_strAcqYear);
	pSIReportManager->SetTempVariable(_T("접수번호"),m_strReceiptNo);
	
	
	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

	return;	
}

HBRUSH CCheckinReceiptBookInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
