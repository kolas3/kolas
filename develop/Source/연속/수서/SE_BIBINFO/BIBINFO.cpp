// BIBINFO.cpp : implementation file
//

#include "stdafx.h"
#include "BIBINFO.h"
#include "..\SE_API\SeApi.h"
#include "..\SE_SPECIES\SeSpeciesForm.h"
#include "BibinfoList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBIBINFO dialog


CBIBINFO::CBIBINFO(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(CBIBINFO::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBIBINFO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nACQ_TYPE = 0;
	m_pGrid = NULL;
	m_pDM =NULL;
	m_pDlg = NULL;
	m_nCurrentIdx = -1;

	m_bIsLightVersion = FALSE;
	m_bIsDepartMngUse = FALSE;	

	AfxInitRichEdit();
}

CBIBINFO::~CBIBINFO()
{
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;
	}
}

VOID CBIBINFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBIBINFO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBIBINFO, CDialog)
	//{{AFX_MSG_MAP(CBIBINFO)
	ON_WM_SIZE()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBIBINFO message handlers

BOOL CBIBINFO::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBIBINFO::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridBIBINFO);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy);	

}

BOOL CBIBINFO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr( _T("연속_수서_서지정보관리") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_서지정보관리") );
		return false;
	}

	CESL_ControlMgr* pCM = FindCM(_T("CM_연속_서지정보관리"));
	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("그리드"));
	m_pDM = FindDM(_T("DM_연속_서지정보관리_색인"));
	
	if(!m_pDM) {
		AfxMessageBox( _T("DM Error : DM_연속_서지정보관리_색인") );
		return false;
	}
	
	//라이트 버젼여부를 가져온다
	CSeApi api(this);
	m_bIsLightVersion = api.IsLightVersion(m_pDM);
	
	
	//부서지정 사용 여부를 가져온다
	m_bIsDepartMngUse = api.IsDepartMngUse(m_pDM);
	
	
	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CBibinfoSearch(this);
		m_pDlg->m_nACQ_TYPE = m_nACQ_TYPE;
		m_pDlg->pMain = this->pMain;
		m_pDlg->pParentMgr = (CESL_Mgr*)this;
		m_pDlg->m_pInfo = this->m_pInfo;
		m_pDlg->Create(this);
		m_pDlg->ShowWindow(FALSE);
	}
	
	GetManageValue(_T("기타"), _T("공통"), _T("육사"), _T("육사"), m_strIs64);
	GetManageValue(_T("기타"), _T("공통"), _T("5163"), _T("5163"), m_strIs5163);

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CBIBINFO::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
		m_pDlg->SetFocusOnTitle();
	}
}


VOID CBIBINFO::OnSendQuery(WPARAM w,LPARAM l)
{	
	INT nSearchType;
	nSearchType = m_pDlg->m_nSearchAcqType;
	
	if(1 == nSearchType)
	{
		m_pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_PURCHASE_MANAGE_TBL P");
	}
	else if(2 == nSearchType)
	{
		m_pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S, SE_DONATE_MANAGE_TBL D");
	}
	else if(3 == nSearchType)
	{
		m_pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_SPECIES_TBL S");
	}

	m_pDM->RefreshDataManager(m_pDlg->m_strQuery);
	CString strPURCHASE_YN, strDONATE_YN;
	CString strSRec, strDept, strCource, strQuery, strTbl;	

	for(INT i=0;i<m_pDM->GetRowCount();i++)
	{
		strPURCHASE_YN = _T("");
		strDONATE_YN = _T("");
		strPURCHASE_YN = m_pDM->GetCellData(_T("SS_구입여부"),i);
		strDONATE_YN = m_pDM->GetCellData(_T("SS_기증여부"),i);
		strSRec = m_pDM->GetCellData(_T("ISS_종KEY"),i);
				
		if(_T("Y") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입/기증"),i);
			if(m_nACQ_TYPE == 1)	strTbl = _T("SE_PURCHASE_MANAGE_TBL");
			else if (m_nACQ_TYPE == 2)	strTbl = _T("SE_DONATE_MANAGE_TBL");
			
		}
		else if(_T("N") == strPURCHASE_YN &&  _T("Y") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("기증"),i);
			strTbl = _T("SE_DONATE_MANAGE_TBL");
		}
		else if(_T("Y") == strPURCHASE_YN &&  _T("N") == strDONATE_YN)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입"),i);
			strTbl = _T("SE_PURCHASE_MANAGE_TBL");
		}		
		
		if(m_strIs64 == _T("Y"))
		{
			// 학처 및 학과 코드값을 얻어오기 [육사]
			// 2005.09.14 강봉수
			//
			if(!strSRec.IsEmpty())
			{
				strQuery.Format(_T("SELECT DEPT FROM %s WHERE SPECIES_KEY = '%s'"), strTbl, strSRec);
				m_pDM->GetOneValueQuery(strQuery, strDept);
				m_pDM->SetCellData(_T("UDF_학처명"), strDept, i);
				strQuery.Format(_T("SELECT COURCE FROM %s WHERE SPECIES_KEY = '%s'"), strTbl, strSRec);
				m_pDM->GetOneValueQuery(strQuery, strCource);
				m_pDM->SetCellData(_T("UDF_학과명"), strCource, i);
			}
		}
		strCource.Empty();
		strDept.Empty();
		strSRec.Empty();
		strTbl.Empty();
	}
	

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행상태코드"),_T("UDF_간행상태"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("관리구분"),_T("UDF_관리구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료구분"),_T("UDF_자료구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("이용제한구분"),_T("UDF_이용제한구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("이용대상구분"),_T("UDF_이용대상구분"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("학부"),_T("UDF_학처명"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("학과"),_T("UDF_학과명"));


	m_pGrid->Display();
	m_nCurrentIdx = -1;
	
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		CString str;
		str.Format(_T("검색 결과가 없습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		str.Format(_T(" %d 건의 자료가 검색되었습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}

	if( nRowCnt > 0 )
		m_pDlg->ShowWindow(FALSE);
}

VOID CBIBINFO::InsertSpecies()
{
	
	CSeSpeciesForm dlg(this);
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_nOpenMode = 1;
	dlg.m_pParentDM = m_pDM;
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_bIsLightVersion = m_bIsLightVersion;
	dlg.m_bIsDepartMngUse = m_bIsDepartMngUse;
	dlg.DoModal();
}

VOID CBIBINFO::ModifySpecies()
{
	
	if(m_nCurrentIdx<0)
	{
		AfxMessageBox(_T("선정 된 정보가 없습니다!"));
		return;		
	}
	else
	{
		CSeSpeciesForm dlg(this);

		m_pGrid->SelectMakeList();
		INT cnt = m_pGrid->SelectGetCount();

		if(cnt>0) dlg.m_nMoveOption = 2;
		else dlg.m_nMoveOption = 1;

		dlg.m_nCurrentIdx = m_nCurrentIdx;
		dlg.m_nACQ_TYPE = m_nACQ_TYPE;
		dlg.m_nOpenMode = -1;
		dlg.m_pParentDM = m_pDM;
		dlg.m_pParentGrid = m_pGrid;
		dlg.m_bIsLightVersion = m_bIsLightVersion;
		dlg.m_bIsDepartMngUse = m_bIsDepartMngUse;
		dlg.DoModal();
	}

}

VOID CBIBINFO::DeleteSpecies()
{
	m_pGrid->SelectMakeList();

	INT idx = m_pGrid->SelectGetHeadPosition();
	if(idx<0)
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
	    return;
	}
	
	CString msg;
	INT nCount = m_pGrid->SelectGetCount();
	msg.Format(_T("[ %d ] 건의 자료를 삭제 하시겠습니까?"),nCount);


	if(AfxMessageBox(msg,MB_YESNO) != IDYES) return; 
	
	BeginWaitCursor();

	CSeApi api(this);
	CString strSPECIES_KEY;
	CESL_DataMgr* pDM_SPECIES = FindDM(_T("DM_연속_서지정보관리_종"));
	CESL_DataMgr* pDM_SPECIES_DELETE = FindDM(_T("DM_연속_서지정보관리_삭제종"));
	CESL_DataMgr* pDM_VOL = FindDM(_T("DM_연속_서지정보관리_권"));
	CESL_DataMgr* pDM_VOL_DELETE = FindDM(_T("DM_연속_서지정보관리_삭제권"));
	CESL_DataMgr* pDM_BOOK = FindDM(_T("DM_연속_서지정보관리_책"));
	CESL_DataMgr* pDM_DROP_NO = FindDM(_T("DM_연속_서지정보관리_누락등록번호"));

	CString strTITLE;
	INT ids = 0;
	CArray<INT,INT> arrayDeletedRow;

	while(idx>=0)
	{
		strSPECIES_KEY = m_pDM->GetCellData(_T("ISS_종KEY"),idx);
		
		ids = api.SPECIES_CAT_DELETE(strSPECIES_KEY,
							     pDM_SPECIES,
							     pDM_SPECIES_DELETE,
							     pDM_VOL);
							     
							    
		
		if(ids<0)
		{
			EndWaitCursor();
			strTITLE = m_pDM->GetCellData(_T("ISS_본표제"),idx);

			if(-1001 == ids) //cancel
			{
				msg.Format(_T("[ %s ] 자료의 삭제를 취소하셨습니다!"),strTITLE);
				AfxMessageBox(msg);
				idx = m_pGrid->SelectGetNext();
				continue;
			}
			else if(-1002 == ids) //verifying fail
			{
				idx = m_pGrid->SelectGetNext();
				continue;
			}
			else
			{
				msg.Format(_T("[ %s ] 자료를 삭제하던 중 문제가 발생해 작업을 중단합니다!"),strTITLE);
				AfxMessageBox(msg);
				break;
			}
			
					
			
			return; 	
		}
		
		arrayDeletedRow.Add(idx);

		idx = m_pGrid->SelectGetNext();
	}
	
	
	INT nSize = arrayDeletedRow.GetSize();
	INT nDeletedRow;

	for(INT i=0;i<nSize;i++)
	{
		nDeletedRow = arrayDeletedRow.GetAt(nSize-i-1);
		m_pDM->DeleteRow(nDeletedRow);
	}
	
	m_pGrid->Display();
	EndWaitCursor();

	msg.Format(_T("[ %d ] 건의 자료를 삭제하였습니다!"),nSize);
	AfxMessageBox(msg);
	return; 	


}

VOID CBIBINFO::PrintGridData()
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
	if(m_strIs64 == _T("Y"))
	{
		ids = pSIReportManager->GetSIReportFileFromDB(_T("연속소장목록_육사"));
	}
	else
	{
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속소장목록"));
	}
	
	if (ids < 0) {
		ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	
	//=====================================================
	//2009.02.24 UPDATE BY PJW : 엑셀파일이 있으면 용지를 셋팅하지 않는다.
// 	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	CString strChkFile = _T("..\\CFG\\RptStyle\\연속소장목록.xls");
	CFileFind fFind;
	if( !fFind.FindFile(strChkFile) )
	{		
		pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	}
	//=====================================================
	
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	/*
	pSIReportManager->SetTempVariable(_T("형식"),m_strCondition);
	*/

	ids = pSIReportManager->Print();

	if (pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}


}

VOID CBIBINFO::PrintListData()
{
	
	INT nRowCnt = m_pDM->GetRowCount();
	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return;
	}
	
	m_pGrid->SelectMakeList();
	INT nSelected = m_pGrid->SelectGetCount();
	if(nSelected<1) 
	{
		AfxMessageBox(_T("선택된 항목이 없습니다!"));
		return;
	}
	

	//교열지 출력함수 
	CBibinfoList dlg(this);
	dlg.m_pParentGrid = m_pGrid;
	dlg.m_pParentDM = m_pDM;
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.DoModal();

}


BEGIN_EVENTSINK_MAP(CBIBINFO, CDialog)
    //{{AFX_EVENTSINK_MAP(CBIBINFO)
	ON_EVENT(CBIBINFO, IDC_gridBIBINFO, -600 /* Click */, OnClickgridBIBINFO, VTS_NONE)
	ON_EVENT(CBIBINFO, IDC_gridBIBINFO, -601 /* DblClick */, OnDblClickgridBIBINFO, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBIBINFO::OnClickgridBIBINFO() 
{
	// TODO: Add your control notification handler code here
	if(0 == m_pDM->GetRowCount()) return;

	INT row = m_pGrid->GetRow();
	if (row == 0) return;
	if (row > 0) {
		m_nCurrentIdx = row-1;
	}
}


VOID CBIBINFO::OnDblClickgridBIBINFO() 
{
	// TODO: Add your control notification handler code here
	
	if(0 == m_pGrid->GetMouseRow()) return;
	ModifySpecies();
}

HBRUSH CBIBINFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
