// DonatorDataPrint.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorDataPrint.h"
#include "../../../공통/COMMON_DONATOR_MANAGER/DonatorMgr.h"
#include "../../../공통/COMMON_DONATOR_MANAGER/DonateDataView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorDataPrint dialog


CDonatorDataPrint::CDonatorDataPrint(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorDataPrint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorDataPrint)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pAddressDM = NULL;
	m_pGrid  = NULL;
	m_pSearchDlg = NULL;
	m_pPrintMgr = NULL;
	m_pDataProcessingDlg = NULL;
	m_pSortItemSet = NULL;
}

CDonatorDataPrint::~CDonatorDataPrint()
{
	if(m_pSearchDlg)
	{
		delete m_pSearchDlg;
		m_pSearchDlg = NULL;
	}
	if(m_pPrintMgr)
	{
		delete m_pPrintMgr;
		m_pPrintMgr = NULL;
	}
	if(m_pDataProcessingDlg)
	{
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
	if(m_pSortItemSet)
	{
		delete m_pSortItemSet;
		m_pSortItemSet = NULL;
	}

}

VOID CDonatorDataPrint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorDataPrint)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorDataPrint, CDialog)
	//{{AFX_MSG_MAP(CDonatorDataPrint)
	ON_WM_SIZE()
	ON_MESSAGE(DONATOR_QUERY_CREATE, OnQueryCreate)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorDataPrint message handlers

BOOL CDonatorDataPrint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr( _T("K2UPGRADE_인수증감사장출력") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : K2UPGRADE_인수증감사장출력") );
		return false;
	}
	m_pCM = FindCM(_T("CM_감사장인수증출력"));
	m_pDM = FindDM(_T("DM_기증자관리"));
	m_pAddressDM = FindDM(_T("DM_주소레이블"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));	

	m_pSearchDlg = new CDonatorNameGetter_Search(this);
	m_pSearchDlg->SetOpenMode(1000);
	InitSortItem();
	m_pPrintMgr = new CBasicPrintMgr(this);
	InitPopupMenuItem();	
	
	m_MarcConvertMgr.SetMarcMgr(m_pInfo->pMarcMgr);

	//도서관정보 설정
	CLibInfoMgr mgr(this);
	mgr.GetLibInfo(GetUserID(), &m_cLibInfo);

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDonatorDataPrint::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridDONATOR_DATA_LIST);
	if(pGrid->GetSafeHwnd() == NULL) return;
	//pGrid->MoveWindow(5, 5, cx-10, cy-10);		
	pGrid->MoveWindow(0, 0, cx, cy);
}

VOID CDonatorDataPrint::InitPopupMenuItem()
{
	CArray <CString, CString> RemoveListArray;
	RemoveListArray.RemoveAll();
	m_pGrid->InitPopupMenuItem();
	m_pGrid->AddPopupMenuItem((CView*)pMain, VIEW_STYLE, &RemoveListArray);	
}

VOID CDonatorDataPrint::ShowSearchDlg()
{
	if(!m_pSearchDlg->GetCreate())
	{
		m_pSearchDlg->Create(this);
	}
	
	m_pSearchDlg->ShowWindow(SW_SHOW);
	m_pSearchDlg->UpdateWindow();
	m_pSearchDlg->CenterWindow();
}

VOID CDonatorDataPrint::ShowDonateDataDlg()
{
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	CDonateDataView dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetMainGrid(m_pGrid);
	dlg.DoModal();
}

BOOL CDonatorDataPrint::Create(CWnd *pWnd)
{
	return CDialog::Create(IDD, pWnd);
}

UINT CDonatorDataPrint::OnQueryCreate(LPARAM lParam, WPARAM wParam)
{
	ShowDataProcessingDlg();
	m_pDataProcessingDlg->SetMessage(_T("기증자자료를 구축중입니다."));
	CString strQuery = m_pSearchDlg->GetWhereStmt();
	m_pDM->RefreshDataManager(strQuery, m_pDataProcessingDlg->GetProgressCtrl());
	if(m_pDM->GetRowCount() == 0)
	{
		DeleteDataProcessingDlg();
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		m_pGrid->Display();
		return 0;
	}
	//m_pDataProcessingDlg->SetMessage(_T("기증자자료를 출력중입니다."));
	DeleteDataProcessingDlg();
	m_pGrid->Display();
	return 0;
}

BEGIN_EVENTSINK_MAP(CDonatorDataPrint, CDialog)
    //{{AFX_EVENTSINK_MAP(CDonatorDataPrint)
	ON_EVENT(CDonatorDataPrint, IDC_gridDONATOR_DATA_LIST, -601 /* DblClick */, OnDblClickgridDONATORDATALIST, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDonatorDataPrint::OnDblClickgridDONATORDATALIST() 
{
	// TODO: Add your control notification handler code here
	if(m_pGrid->GetMouseRow() == 0) return ;

	if(m_pDM->GetRowCount() == 0) return ;	
	ShowDonatorDataDlg(GUPDATE);	
}

VOID CDonatorDataPrint::ShowDonatorDataDlg(OPERATION_TYPE nOpType)
{
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("검색된 자료가 없습니다."));
		return ;
	}

	CDonatorMgr dlg(this);
	dlg.SetOpenMode(nOpType);
	dlg.SetMainCMAlias(m_pCM->CM_ALIAS);
	dlg.DoModal();
//	m_pGrid->Display();
}

INT CDonatorDataPrint::MakeDonateData(CESL_DataMgr * pBookDM, CESL_DataMgr * pPrintDM, INT nIdx)
{
	CString strQuery, strDonatorKey;
	strDonatorKey = m_pDM->GetCellData(_T("기증자정보KEY"), nIdx);
	if(strDonatorKey.IsEmpty()) return -1;

	strQuery.Format(_T("I.REC_KEY=P.SPECIES_KEY AND P.REC_KEY=B.ACQ_KEY AND P.DONATOR_KEY=%s"), strDonatorKey);
	pBookDM->FreeData();
	pBookDM->RefreshDataManager(strQuery);
	pPrintDM->FreeData();
	CBO_ACQ_Business_Api mgr(this);
	mgr.InitTempData();
	mgr.SetTempPtr(_T("책DM"), (DWORD*)pBookDM);
	mgr.SetTempPtr(_T("종DM"), (DWORD*)pPrintDM);
	INT ids = mgr.SPFExecutePath(_T("책To종"));

	CString strTmpData;
	INT cnt = pPrintDM->GetRowCount();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData.Format(_T("%d"), i+1);
		pPrintDM->SetCellData(_T("UDF_일련번호"), strTmpData, i);
	}
	return 0;
}

VOID CDonatorDataPrint::DonateConfirmDataPrint()
{
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	BOOL bMakePrint = FALSE;
	CDonateDataView view(this);
	view.ShowDonateDataSearchDlgByModal();
	
	CString strSpeciesCnt, strBookCnt, strMsg, strDonator, strDonateData;
	CESL_DataMgr * pBO_BookDM = FindDM(_T("DM_기증접수자료관리"));
	CESL_DataMgr * pBO_PrintDM = FindDM(_T("DM_기증접수자료관리_종"));
	CESL_DataMgr * pSE_DM = FindDM(_T("DM_연속_기증자료"));
	CESL_DataMgr * pPrintDM = FindDM(_T("DM_통합_기증자료"));
	
	if(pBO_BookDM == NULL || pBO_PrintDM == NULL || pSE_DM == NULL || pPrintDM == NULL) return ;

	m_pPrintMgr->InitRptFile();

	INT i = m_pGrid->SelectGetHeadPosition();
	INT ids;
	
	while(i >= 0)
	{
		view.MakeBO_DonateData(m_pDM, pBO_BookDM, pBO_PrintDM, i);
		ids = SetMarcInfo(pBO_PrintDM);
		if(ids < 0) return ;
		view.MakeSE_DonateData(m_pDM, pSE_DM, i);
		view.MakeMergeDonateData(pBO_PrintDM, pSE_DM, pPrintDM);

		strDonator = m_pDM->GetCellData(_T("기증자"), i);

		if(pPrintDM->GetRowCount() == 0) 
		{
			strMsg.Format(_T("%s씨는 기증자료가 없습니다."), strDonator);
			ESLAfxMessageBox(strMsg);
			i = m_pGrid->SelectGetNext();
			continue;
		}
		
		ids = view.SetPrintDonateConfirmData(m_pDM, pBO_BookDM, pBO_PrintDM, pSE_DM, pPrintDM, m_pPrintMgr, &m_cLibInfo, i);
		if(ids < 0) 
		{
			i = m_pGrid->SelectGetNext();
			continue;
		}
		m_pPrintMgr->MakeXMLPageFile();
		bMakePrint = TRUE;
		i = m_pGrid->SelectGetNext();
	}
	if (bMakePrint == TRUE)
		m_pPrintMgr->Print2();
	else 
		return;
}

VOID CDonatorDataPrint::DonatorDataPrint()
{
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}
	m_pPrintMgr->SetPrintDM(m_pDM);
	m_pPrintMgr->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	m_pPrintMgr->SetSIReportFilename(_T("K2UP_기증자감사장"));
	m_pPrintMgr->InitRptFile();

	INT i = m_pGrid->SelectGetHeadPosition();
	while(i >= 0)
	{
		m_pPrintMgr->InitPrintData();
		m_pPrintMgr->AddPrintData(_T("기증자"), m_pDM->GetCellData(_T("기증자"), i));
		m_pPrintMgr->AddPrintData(_T("도서관명"), m_cLibInfo.GetLibName());
		m_pPrintMgr->MakeXMLPageFile();
		i = m_pGrid->SelectGetNext();
	}
	m_pPrintMgr->Print2();
}

VOID CDonatorDataPrint::AddressLabelPrint()
{
	if(CheckSelectDataValid() < 0)
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return ;
	}

	CString sLabelType;
	INT		ids;
	/// 주소 레이블 용지 형식 GET
	ids = GetManageValue( _T("수서")  , _T("공통") , _T("주소레이블종류") , _T("") , sLabelType );
	if( ids < 0 ) return;

	ids = MakeAddressDM();
	if(ids < 0) return;
	
	m_pPrintMgr->InitPrintData();
	m_pPrintMgr->SetPrintDM(m_pAddressDM);
	m_pPrintMgr->SetPrintType(BO_ACQ_DEFAULT);
	if ( sLabelType == '0' )
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_기증자주소레이블"));
	}
	else
	{
		m_pPrintMgr->SetSIReportFilename(_T("K2UP_기증자주소레이블_2X8"));
	}
	
	m_pPrintMgr->Print();
}

INT CDonatorDataPrint::CheckSelectDataValid()
{
	if(m_pDM->GetRowCount() == 0) return -1;

	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	ids = m_pGrid->SelectGetHeadPosition();
	if(ids < 0) return -1;
	return 0;
}

INT CDonatorDataPrint::MakeAddressDM()
{
	INT ids = m_pGrid->SelectMakeList();
	if(ids < 0) return -1;
	INT i = m_pGrid->SelectGetHeadPosition();
	BOOL bInsert = TRUE;
	CString ArrAddressAlias1[4] = {_T("주소1"), _T("기증자1"), _T("우편번호1"), _T("주소정보1")};
	CString ArrAddressAlias2[4] = {_T("주소2"), _T("기증자2"), _T("우편번호2"), _T("주소정보2")};
	m_pAddressDM->FreeData();

	while(i >= 0)
	{
		AddAddressDM(i, bInsert, ArrAddressAlias1, ArrAddressAlias2);
		i = m_pGrid->SelectGetNext();
		bInsert = !bInsert;
	}
	return 0;
}

INT CDonatorDataPrint::AddAddressDM(INT nIdx, BOOL bInsert, CString * ArrAlias1, CString * ArrAlias2)
{
	INT nCurRow;
	CString ArrAddressInfo[4];

	if(bInsert) 
		m_pAddressDM->InsertRow(-1);

	nCurRow = m_pAddressDM->GetRowCount() - 1;

	ArrAddressInfo[0] = m_pDM->GetCellData(_T("주소"), nIdx);
	ArrAddressInfo[1] = m_pDM->GetCellData(_T("기증자"), nIdx);
	ArrAddressInfo[2] = m_pDM->GetCellData(_T("우편번호"), nIdx);
	ArrAddressInfo[3] = m_pDM->GetCellData(_T("주소"), nIdx) + _T("\r\n\r\n") + m_pDM->GetCellData(_T("기증자"), nIdx) + _T(" 귀하") + _T("\r\n\r\n") + m_pDM->GetCellData(_T("우편번호"), nIdx);
	if(bInsert) m_pAddressDM->SetCellData(ArrAlias1, 4, ArrAddressInfo, nCurRow);
	else m_pAddressDM->SetCellData(ArrAlias2, 4, ArrAddressInfo, nCurRow);
	return 0;
}

VOID CDonatorDataPrint::ShowDataProcessingDlg()
{
	if(m_pDataProcessingDlg == NULL) m_pDataProcessingDlg = new CDataProcessingDlg(this);

	if(!m_pDataProcessingDlg->GetCreate())
	{
		if(!m_pDataProcessingDlg->Create(this))
		{
			delete m_pDataProcessingDlg;
			m_pDataProcessingDlg = NULL;
			return;
		}
	}
	m_pDataProcessingDlg->CenterWindow();
	m_pDataProcessingDlg->ShowWindow(SW_SHOW);
	m_pDataProcessingDlg->UpdateWindow();
	//프로그래스 컨트롤 초기화
	m_pDataProcessingDlg->ShowHideCancelButton(FALSE);
//	m_pDataProcessingDlg->GetProgressCtrl()->SetRange(0, 0);
	m_pDataProcessingDlg->GetProgressCtrl()->SetPos(0);
}

VOID CDonatorDataPrint::DeleteDataProcessingDlg()
{
	if(m_pDataProcessingDlg)
	{
		m_pDataProcessingDlg->ShowWindow(SW_HIDE);
		delete m_pDataProcessingDlg;
		m_pDataProcessingDlg = NULL;
	}
}

VOID CDonatorDataPrint::ShowSortDlg()
{
	if(m_pSortItemSet == NULL || m_pDM == NULL) 
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		return;
	}
	if(m_pDM->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("정렬할 자료가 없습니다."));
		return ;
	}
	
	CDataSortDlg dlg(this);
	dlg.SetMainDM(m_pDM);
	dlg.SetSortItemSet(m_pSortItemSet);
	dlg.SetOpenMode(1);
	if(IDOK == dlg.DoModal())
		m_pGrid->Display();
}

BOOL CDonatorDataPrint::InitSortItem()
{
	CStringArray * pTmpData = NULL;	
	
	m_pSortItemSet = new CSortItemSet(this);
	m_pSortItemSet->SetGrid(m_pGrid);
	m_pSortItemSet->InitDataManager(m_pDM->CONNECTION_INFO, _T("DM_정렬항목설정"));
	m_pSortItemSet->SetSortCfgFileName(GetSMAlias(), _T("MainGrid"));
	m_pSortItemSet->ReadSortItem();
	m_pSearchDlg->SetSortItem(m_pSortItemSet);
	return TRUE;
}

INT CDonatorDataPrint::SetMarcInfo(CESL_DataMgr *pDM)
{
	if(pDM == NULL) return -1;
	if(pDM->GetRowCount() == 0) return 0;
	INT nRowCount = pDM->GetRowCount();

	CString strTmpData;
	CMarc marc;
	INT ids;
	for(INT i=0; i<nRowCount ; i++)
	{
		strTmpData = pDM->GetCellData(_T("BS_MARC"), i);
		ids = m_pInfo->pMarcMgr->Decoding(strTmpData, &marc);
		if(ids < 0) return -1;
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
		if(ids >= 0) pDM->SetCellData(_T("UDF_총서사항"), strTmpData, i);
		strTmpData.Empty();	ids = m_MarcConvertMgr.GetEditionStatement(&marc, strTmpData);
		if(ids >= 0) pDM->SetCellData(_T("IBS_판사항"), strTmpData, i);
		strTmpData.Empty();	 strTmpData = pDM->GetCellData(_T("IBS_본표제"), i);
		ids = m_MarcConvertMgr.GetTitleStatement(&marc, strTmpData);
		if(ids >= 0) pDM->SetCellData(_T("UDF_표제사항"), strTmpData, i);
	}

	return 0;
}

HBRUSH CDonatorDataPrint::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
