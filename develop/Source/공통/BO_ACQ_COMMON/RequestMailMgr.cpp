// RequestMailMgr.cpp : implementation file
//

#include "stdafx.h"
#include "RequestMailMgr.h"
#include "K2UP_MailMgr.h"
#include "../BO_ACQ_PRINT_MGR/BasicPrintMgr.h"
#include "../BO_ACQ_PRINT_MGR/MakePrintDMMgr.h"
// 2011.09.08 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequestMailMgr dialog


CRequestMailMgr::CRequestMailMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequestMailMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequestMailMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_pDM = NULL; 
	m_pCM = NULL; 
	m_pGrid = NULL;
	m_OpenCode = -1;
	m_pComplexDM = NULL;
}

CRequestMailMgr::~CRequestMailMgr()
{
}

VOID CRequestMailMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequestMailMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequestMailMgr, CDialog)
	//{{AFX_MSG_MAP(CRequestMailMgr)
	ON_BN_CLICKED(IDC_btnRMAIL_FIND, OnbtnRMAILFIND)
	ON_BN_CLICKED(IDC_btnRMAIL_PRINT, OnbtnRMAILPRINT)
	ON_BN_CLICKED(IDC_btnRMAIL_SEND, OnbtnRMAILSEND)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequestMailMgr message handlers

BOOL CRequestMailMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_비치희망메일")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	GetWindowRect(&m_rcInitial);

	m_pCM = FindCM(_T("CM_비치희망메일"));
	m_pDM = FindDM(_T("DM_비치희망메일"));

	m_pComplexDM = FindDM(_T("DM_비치희망메일_통합"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	CString furnish_status[5][2] = {
		_T(" "), _T("적용안함"),
		_T("1"), _T("신청중"),
		_T("2"), _T("처리중"),
		_T("3"), _T("소장중"),
		_T("4"), _T("취소됨")
	};
	
	CMulitComboBoxManager mgr(this);
	mgr.SetMulTiComboBoxStrData(m_pCM->CM_ALIAS, _T("적용안함"), furnish_status, _T("비치상태"), 5);
	
	ShowDlgCtrlByOpenCode();	

	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("비치희망메일");
	}
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequestMailMgr::OnbtnRMAILFIND() 
{
	// TODO: Add your control notification handler code here
	CK2UP_MailMgr impl(this);
	impl.InitTempData();
	impl.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	impl.SetTempPtr(_T("CM"), (DWORD*)m_pCM);
	impl.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);	
	INT ids = impl.SPFExecutePath(_T("비치희망메일검색"));
	if(ids < 0)
	{
		return;
	}
	else if( ids == 2000)
		ESLAfxMessageBox(_T("검색 결과가 없습니다."));
	SortData();
	m_pGrid->Display();
}

INT MakeCopyDM(CESL_DataMgr * pSrcDM, CESL_DataMgr * pDstDM)
{
	INT ids = -1;

	pDstDM->TBL_NAME = pSrcDM->TBL_NAME;
	pDstDM->EXTRA_CONDITION = pSrcDM->EXTRA_CONDITION;
	pDstDM->CONDITION = pSrcDM->CONDITION;
	pDstDM->DB_MGR_ALIAS = pSrcDM->DB_MGR_ALIAS;
	

	pDstDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
	pDstDM->m_nCols = pSrcDM->RefList.GetCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pSrcDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) return -1;

		ids = pDstDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) return -1;
	}
	return 0;
}

VOID CRequestMailMgr::OnbtnRMAILPRINT() 
{
	CMakePrintDMMgr mgr(this);

	mgr.SetTempPtr(_T("출력DM"), (DWORD*)m_pDM);

	INT ids = mgr.SPFExecutePath(_T("일련번호설정"));
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("출력데이터에 오류가 있습니다."));
		return ;
	}

	CBasicPrintMgr PrintMgr(this);
	PrintMgr.InitPrintData();
	PrintMgr.SetPrintDM(m_pDM);
	PrintMgr.SetPrintType(0);
	PrintMgr.SetSIReportFilename(_T("K2UP_비치희망메일관리자료"));
	PrintMgr.SetPrivacyPrintLogParm( TRUE, m_pInfo->USER_PK, m_pInfo->USER_ID, _T("비치희망메일"), m_pInfo->MANAGE_CODE, _T("비치희망메일"),	0 );
	PrintMgr.Print();
}

VOID CRequestMailMgr::OnbtnRMAILSEND() 
{
	// TODO: Add your control notification handler code here
	CK2UP_MailMgr impl(this);
	impl.InitTempData();
	impl.InitTempPtr();
	impl.SetTempPtr(_T("PARENT_DM"), (DWORD*)m_pParentDM);
	impl.SetTempPtr(_T("DM"), (DWORD*)m_pDM);
	impl.SetTempPtr(_T("COMPLEX_DM"), (DWORD*)m_pComplexDM);
	impl.SetTempPtr(_T("Grid"), (DWORD*)m_pGrid);
	INT ids = impl.SPFExecutePath(_T("비치희망메일발송"));
	if(ids < 0)
	{
		return;
	}
	else if(ids == 1000)
	{
		ESLAfxMessageBox(_T("메일 발송할 올바른 메일 레코드 정보가 없습니다."));
		return ;
	}
	m_pGrid->Display();
}

VOID CRequestMailMgr::ShowDlgCtrlByOpenCode()
{
	GetDlgItem(IDC_edtRMAIL_REQUESTDATE1)->EnableWindow(TRUE);
	GetDlgItem(IDC_edtRMAIL_REQUESTDATE2)->EnableWindow(TRUE);
	GetDlgItem(IDC_cmbRMAIL_FURNISH_STATUS)->EnableWindow(TRUE);
	GetDlgItem(IDC_edtRMAIL_REQUESTOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnRMAIL_FIND)->EnableWindow(TRUE);
	CString value;
	GetManageValue(_T("수서"), _T("단행"), _T("비치희망자료알림메일자동발송"), _T(""), value);

	if(m_OpenCode > 0 && value == _T("Y"))
	{
		GetDlgItem(IDC_edtRMAIL_REQUESTDATE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtRMAIL_REQUESTDATE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_cmbRMAIL_FURNISH_STATUS)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtRMAIL_REQUESTOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnRMAIL_FIND)->EnableWindow(FALSE);
		SetMailDM();
	}
	else if(m_OpenCode > 0 && value == _T("N"))
	{
		EndDialog(IDCANCEL);
	}
	else
	{
		INT ids = SetMailDM(m_pParentGrid);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("선정된 비치희망자료를 구축할 수 없습니다."));
			return ;
		}
	}
}

VOID CRequestMailMgr::SetMailDM()
{
	CString strTemp;
	CString strRecKey;
	CString strFunishKeyList;
	CString strTempWhere;
	CString strReplase;
	INT nCnt;
	INT nLocation;
	INT nStartLocation;
	strFunishKeyList = m_strFunishKeyList;
	nCnt = 0;
	nLocation = 0;
	while( 1 )
	{
		nLocation = strFunishKeyList.Find(_T(","), nLocation+1);
		++nCnt;
		if( 0 > nLocation )
		{
			break;			
		}
	}
	if ( (nCnt % 1000) == 0 )
	{
		nCnt = (nCnt/1000);
	}
	else
	{
		nCnt = (nCnt/1000)+1;
	}
	nStartLocation = strFunishKeyList.GetLength()/nCnt;

	for( INT i = 0 ; i < nCnt ; i++)
	{
		if( i == (nCnt -1) )
		{
			strRecKey = strFunishKeyList;
		}
		else
		{
			nLocation = strFunishKeyList.Find(_T(","), nStartLocation);
			strRecKey = strFunishKeyList.Mid(0, nLocation );
			strTemp.Format(_T("%s,"), strRecKey);
			strFunishKeyList.Replace( strTemp, _T("") );
		}
		if( m_strWhereStmt.IsEmpty() )
		{
			m_strWhereStmt.Format(_T("REC_KEY IN (%s)"), strRecKey);
		}
		else
		{
			strTemp.Format(_T("OR REC_KEY IN (%s)"), strRecKey);
			m_strWhereStmt += strTemp;
		}
	}
	m_pDM->RefreshDataManager(m_strWhereStmt);

	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < m_pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1;
		for(INT i = 0; i < m_pDM->GetRowCount(); i++)
		{
			m_pDM->GetCellData(_T("EMAIL"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("BO_FURNISH_TBL"), _T("EMAIL"), asDes1);
				for(i = 0; i < m_pDM->GetRowCount(); i++)
				{
					m_pDM->SetCellData(_T("EMAIL"), asDes1.GetAt(i), i);
				}
				p3000Api->CloseSocket();	
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	m_pGrid->Display();
}

VOID CRequestMailMgr::SetOpenCode(INT openCode)
{
	m_OpenCode = openCode;
}

VOID CRequestMailMgr::SetFurnishKeyList(CStringArray *pArray)
{
	INT cnt = pArray->GetSize();
	m_strFunishKeyList.Empty();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(m_strFunishKeyList.IsEmpty()) m_strFunishKeyList = pArray->GetAt(i);
		else m_strFunishKeyList += _T(", ") + pArray->GetAt(i);
	}
}

INT CRequestMailMgr::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData;
	if(strDMFieldAlias == _T("UDF_비치상태설명"))
	{
		strTmpData = m_pDM->GetCellData(_T("비치상태"), nRow);
		if(strTmpData.GetLength() == 0) str = _T("신청중");
		else if(strTmpData == _T("1")) str = _T("신청중");
		else if(strTmpData == _T("2")) str = _T("처리중");
		else if(strTmpData == _T("3")) str = _T("소장중");
		else if(strTmpData == _T("4")) str = _T("취소됨");
		m_pDM->SetCellData(strDMFieldAlias, str, nRow);
	}
	return 0;
}

INT CRequestMailMgr::SetMailDM(CESL_Grid *pGrid)
{
	if(pGrid == NULL) return -1;
	m_pDM->FreeData();
	INT ids = pGrid->SelectMakeList();
	if(ids < 0) return 0;
	INT i = pGrid->SelectGetHeadPosition();
	if(i < 0) return 0;

	CK2UP_MailMgr impl(this);
	INT nCurRow;
	while(i >=0)
	{
		m_pDM->InsertRow(-1);
		nCurRow = m_pDM->GetRowCount()-1;
		ids = impl.CopyDMRowToDMByAlias(m_pParentDM, i, m_pDM, nCurRow);
		if(ids < 0) return -1;
		i = pGrid->SelectGetNext();
	}

	m_pGrid->Display();
	return 0;
}

INT CRequestMailMgr::SortData()
{
	if(m_pDM == NULL) return 0;
	if(m_pDM->GetRowCount() == 0) return 0;
	
	INT nCondColumns = 0;
	CString strAlias[5];
	INT nCond[5];
	strAlias[nCondColumns] = _T("비치상태");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("신청일자");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("신청자");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("본표제");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	m_pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}

VOID CRequestMailMgr::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pGrid->GetSafeHwnd() == NULL) return;
	
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	
	CRect rect;
	GetClientRect(rect);
	rect.top += 83;
	rect.bottom -= 4;
	rect.left += 4;
	rect.right -= 4;
	
	m_pGrid->MoveWindow(rect);		
}

HBRUSH CRequestMailMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
