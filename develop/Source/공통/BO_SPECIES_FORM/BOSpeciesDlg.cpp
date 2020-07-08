// BOSpeciesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOSpeciesDlg.h"

#include "BOVolumeDlg.h"
#include "BOAppendixDlg.h"

#include "BOMarcEditDlg.h"
#include "BOISBNAdditionDlg.h"
#include "..\BO_MARC_IMPORT\MARC_IMPORT.h"
#include "..\Duplicate_Data_Search\DuplicateSearch_Interface.h"

#include "..\..\공동목록\Light_GatherDataSearch\UC_MARC_IMPORT.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOSpeciesDlg dialog


CBOSpeciesDlg::CBOSpeciesDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOSpeciesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nOpenMode = -1;
	m_bShowCode = TRUE;

	// DM 초기화
	m_pDM_Species = NULL;
	m_pDM_Book = NULL;
	m_pDM_Purchase = NULL;
	m_pDM_Donation = NULL;
	m_pDM_Index = NULL;

	// CM 초기화
	m_pCM_Index = NULL;
	m_pCM_Purchase = NULL;

	// Form Api
	m_pFormApi = new CBOFormApi(this);

	// Species API
	m_pBOSpeciesApi = NULL;	

	m_pDM_Parent = NULL;
	m_pGrid_Parent = NULL;
	m_nParentSelectedRow = -1;

	m_strRegCodeField.Empty();

	m_bDupAddMode = FALSE;

	m_strDefaultValueFilePath = _T("..\\cfg\\species_form.cfg");

	m_pNoteCtrl = NULL;

	m_bControlDisplay = FALSE;

	m_bAutoDupSearch = FALSE;

	m_bViewVolInfo = FALSE;
}

BOOL CBOSpeciesDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOSpeciesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOSpeciesDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOSpeciesDlg, CDialog)
	//{{AFX_MSG_MAP(CBOSpeciesDlg)
	ON_BN_CLICKED(IDC_bSHOW_HIDE_CODE, OnbSHOWHIDECODE)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_CBN_SELCHANGE(IDC_cCLASIFY_CLASS, OnSelchangecCLASIFYCLASS)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bDEFAULT_CONFIG, OnbDEFAULTCONFIG)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bSAVEAS, OnbSAVEAS)
	ON_BN_CLICKED(IDC_bMARC_EDIT, OnbMARCEDIT)
	ON_BN_CLICKED(IDC_bMARC_IMPORT, OnbMARCIMPORT)
	ON_BN_CLICKED(IDC_bKOLIS_NET, OnbKOLISNET)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_bDUP_SEARCH, OnbDUPSEARCH)
	ON_BN_CLICKED(IDC_bEXTRA_ISBN, OnbEXTRAISBN)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_EN_KILLFOCUS(IDC_eREG_CLASS_CODE, OnKillfocuseREGCLASSCODE)
	ON_BN_CLICKED(IDC_bVOL_INFO, OnbVOLINFO)
	ON_CBN_SELCHANGE(IDC_cPRICE_TYPE, OnSelchangecPRICETYPE)
	ON_EN_KILLFOCUS(IDC_ePRICE_DC, OnKillfocusePRICEDC)
	ON_EN_KILLFOCUS(IDC_ePRICE, OnKillfocusePRICE)
	ON_BN_CLICKED(IDC_bAPPLY_SERIES, OnbAPPLYSERIES)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_eTITLE, OnKillfocuseTITLE)
	ON_CBN_SELCHANGE(IDC_cFORM_CLASS, OnSelchangecFORMCLASS)
	ON_BN_CLICKED(IDC_bPREV_SPECIES, OnbPREVSPECIES)
	ON_BN_CLICKED(IDC_bNEXT_SPECIES, OnbNEXTSPECIES)
	ON_BN_CLICKED(IDC_btnSEARCH_DONATOR, OnbtnSEARCHDONATOR)
	ON_BN_CLICKED(IDC_btnGROUP_INSERT, OnbtnGROUPINSERT)
	ON_EN_KILLFOCUS(IDC_edtACQ_YEAR, OnKillfocusedtACQYEAR)
	//}}AFX_MSG_MAP
	ON_MESSAGE(NOTE_CLICKED, OnNoteClicked)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBOSpeciesDlg Operations
VOID CBOSpeciesDlg::ShowACQInfoCtrlByOpenMode()
{
EFS_BEGIN
	INT nArrCodeResourceID[256];
	INT nCount = 0;
	if(m_nOpenMode == 1200 || m_nOpenMode == -1200)//기증접수자료 입력/수정일 경우
	{
		nArrCodeResourceID[nCount++] = IDC_tPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_cPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_tPRICE;
		nArrCodeResourceID[nCount++] = IDC_ePRICE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_REAL;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_JOB;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_OTHER_INFO;
		nArrCodeResourceID[nCount++] = IDC_tPURCHASE;
		nArrCodeResourceID[nCount++] = IDC_cPROVIDER;
		nArrCodeResourceID[nCount++] = IDC_tCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_cCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_stcNONDONATE_REASON;
		nArrCodeResourceID[nCount++] = IDC_edtNONDONATE_REASON;
	}
	else if(m_nOpenMode == 1100 || m_nOpenMode == -1100)//구입대상자료 입력/수정일 경우
	{
		nArrCodeResourceID[nCount++] = IDC_stcDONATE_PRICE;
		nArrCodeResourceID[nCount++] = IDC_edtDONATE_PRICE;
		nArrCodeResourceID[nCount++] = IDC_stcDONATE_CURRENCY_CODE;
		nArrCodeResourceID[nCount++] = IDC_cmbDONATE_CURRENCY_CODE;
		nArrCodeResourceID[nCount++] = IDC_stcDONATOR;
		nArrCodeResourceID[nCount++] = IDC_edtDONATOR;
		nArrCodeResourceID[nCount++] = IDC_btnSEARCH_DONATOR;
		nArrCodeResourceID[nCount++] = IDC_stcNONDONATE_REASON;
		nArrCodeResourceID[nCount++] = IDC_edtNONDONATE_REASON;
	}
	else if(m_nOpenMode == 1000 || m_nOpenMode == -1000) //기초자료 입력/수정일 경우
	{
		nArrCodeResourceID[nCount++] = IDC_tPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_cPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_tPRICE;
		nArrCodeResourceID[nCount++] = IDC_ePRICE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_REAL;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_JOB;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_OTHER_INFO;
		nArrCodeResourceID[nCount++] = IDC_tPURCHASE;
		nArrCodeResourceID[nCount++] = IDC_cPROVIDER;
		nArrCodeResourceID[nCount++] = IDC_tCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_cCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_stcDONATOR;
		nArrCodeResourceID[nCount++] = IDC_edtDONATOR;
		nArrCodeResourceID[nCount++] = IDC_btnSEARCH_DONATOR;
		nArrCodeResourceID[nCount++] = IDC_stcNONDONATE_REASON;
		nArrCodeResourceID[nCount++] = IDC_edtNONDONATE_REASON;
	}
	else if(m_nOpenMode == 1300 || m_nOpenMode == -1300) //기증부당자료 입력/수정일경우
	{
		nArrCodeResourceID[nCount++] = IDC_tPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_cPRICE_TYPE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_DC;
		nArrCodeResourceID[nCount++] = IDC_tPRICE;
		nArrCodeResourceID[nCount++] = IDC_ePRICE;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_REAL;
		nArrCodeResourceID[nCount++] = IDC_tPRICE_JOB;
		nArrCodeResourceID[nCount++] = IDC_ePRICE_OTHER_INFO;
		nArrCodeResourceID[nCount++] = IDC_tPURCHASE;
		nArrCodeResourceID[nCount++] = IDC_cPROVIDER;
		nArrCodeResourceID[nCount++] = IDC_tCURRENCY_CLASS;
		nArrCodeResourceID[nCount++] = IDC_cCURRENCY_CLASS;
	}
	CWnd * pWnd = NULL;
	for(INT i=0 ; i<nCount ; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if(pWnd->GetSafeHwnd() != NULL)
			pWnd->ShowWindow(SW_HIDE);
	}

EFS_END
}

VOID CBOSpeciesDlg::ShowCodeResource()
{
	INT i = 0;
	INT nCodeResourcesHeight = 122;
	CWnd *pWnd = NULL;

	if( m_bShowCode )
	{
		GetDlgItem(IDC_bSHOW_HIDE_CODE)->SetWindowText(_T("간략검색"));
	}
	else
	{
		GetDlgItem(IDC_bSHOW_HIDE_CODE)->SetWindowText(_T("상세검색"));
	}


	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_bShowCode &&rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_STATIC_CODE;
	nArrCodeResourceID[nCount++] = IDC_tSUB_REG_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cSUB_REG_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tUSE_OBJECT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cUSE_OBJECT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tUSE_LIMIT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cUSE_LIMIT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tSHELF_CODE;
	nArrCodeResourceID[nCount++] = IDC_cCLASIFY_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tCONTROL_MAT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cCONTROL_MAT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMANAGE_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMANAGE_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tCLASIFY_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cSHELF_CODE;
	nArrCodeResourceID[nCount++] = IDC_tWORK_CODE;
	nArrCodeResourceID[nCount++] = IDC_cWORK_CODE;
	nArrCodeResourceID[nCount++] = IDC_tDISCRIPTION;
	nArrCodeResourceID[nCount++] = IDC_eDISCRIPTION;
	nArrCodeResourceID[nCount++] = IDC_tMAT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMAT_CLASS;

	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowCode)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Move Other Resources
	nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_STATIC_MARC;
	nArrOtherResources[nCount++] = IDC_tTITLE;
	nArrOtherResources[nCount++] = IDC_eTITLE;
	nArrOtherResources[nCount++] = IDC_bDUP_SEARCH;
	nArrOtherResources[nCount++] = IDC_tAUTHOR1;
	nArrOtherResources[nCount++] = IDC_eAUTHOR1;
	nArrOtherResources[nCount++] = IDC_tEDITION_STMT;
	nArrOtherResources[nCount++] = IDC_eEDITION_STMT;
	nArrOtherResources[nCount++] = IDC_tVOL;
	nArrOtherResources[nCount++] = IDC_eVOL;
	nArrOtherResources[nCount++] = IDC_tSERIES_TITLE;
	nArrOtherResources[nCount++] = IDC_eSERIES_TITLE;
	nArrOtherResources[nCount++] = IDC_ePUBLISH_PLACE;
	nArrOtherResources[nCount++] = IDC_tPUBLISH_PLACE;
	nArrOtherResources[nCount++] = IDC_ePUBLISHER;
	nArrOtherResources[nCount++] = IDC_tPUBLISHER;
	nArrOtherResources[nCount++] = IDC_ePUBLISH_YEAR;
	nArrOtherResources[nCount++] = IDC_tPUBLISH_YEAR;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN_ADD;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN_MISTAKE;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN_ADD;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN_MISTAKE;
	nArrOtherResources[nCount++] = IDC_tEA_ISBN;
	nArrOtherResources[nCount++] = IDC_bEXTRA_ISBN;
	nArrOtherResources[nCount++] = IDC_tSET_ISBN;
	nArrOtherResources[nCount++] = IDC_bISBN_CHECK;
	nArrOtherResources[nCount++] = IDC_STATIC_PURCHASE_GROUP;
	nArrOtherResources[nCount++] = IDC_tPRICE;
	nArrOtherResources[nCount++] = IDC_ePRICE;
	nArrOtherResources[nCount++] = IDC_tPRICE_DC;
	nArrOtherResources[nCount++] = IDC_ePRICE_DC;
	nArrOtherResources[nCount++] = IDC_tPURCHASE;
	nArrOtherResources[nCount++] = IDC_cPROVIDER;
	nArrOtherResources[nCount++] = IDC_bDEFAULT_CONFIG;
	nArrOtherResources[nCount++] = IDC_tPRICE_REAL;
	nArrOtherResources[nCount++] = IDC_eOrderPRICE;
	nArrOtherResources[nCount++] = IDC_cPRICE_TYPE;
	nArrOtherResources[nCount++] = IDC_tPRICE_JOB;
	nArrOtherResources[nCount++] = IDC_ePRICE_OTHER_INFO;
	nArrOtherResources[nCount++] = IDC_bMARC_EDIT;
	nArrOtherResources[nCount++] = IDC_bMARC_IMPORT;
	nArrOtherResources[nCount++] = IDC_bKOLIS_NET;
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_bPREV_SPECIES;
	nArrOtherResources[nCount++] = IDC_bNEXT_SPECIES;
	nArrOtherResources[nCount++] = IDC_bSAVEAS;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	nArrOtherResources[nCount++] = IDC_tTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_eTOTAL_VOLUMEP;	
	nArrOtherResources[nCount++] = IDC_tTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_eTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_bVOL_INFO;
	nArrOtherResources[nCount++] = IDC_tPRICE_TYPE;
	nArrOtherResources[nCount++] = IDC_tREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_eREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_STATIC_ISBN;
	nArrOtherResources[nCount++] = IDC_tCURRENCY_CLASS;
	nArrOtherResources[nCount++] = IDC_cCURRENCY_CLASS;
	nArrOtherResources[nCount++] = IDC_bAPPLY_SERIES;
	

	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if (m_bShowCode)
			{
				rcResource.top += nCodeResourcesHeight;
				rcResource.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
			else
			{
				rcResource.top -= nCodeResourcesHeight;
				rcResource.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

	// Resize Window Size
	GetWindowRect(rcResource);
	if (m_bShowCode)
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
}

INT CBOSpeciesDlg::MakeDupAddMode()
{
	if (m_strDupIndexKey.IsEmpty()) return -1;
	INT ids;

	m_bDupAddMode = TRUE;

	// 현재 있는 구입정보 DM의 키를 바꾸어준다.
	// m_pDM_Purchase->SetCellData(_T("BP_종KEY"), m_strDupIndexKey, 0);

	// 현재 있는 책 DM의 키를 바꾸어 준다.  // 현재 책을 UPDATE 모드로 바꾼다.
	// 현재 있는 책의 대표책 플래그를 모두 'N'으로 바꾸어준다.
	CString strQueryFlag;
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));

		m_pDM_Book->SetCellData(_T("BB_종KEY"), m_strDupIndexKey, idx);
		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("N"), idx);
	}		

	// 종 DM, Index DM을 키로 가져와 구성한다.
	CString strOption;
	strOption.Format(_T("REC_KEY = %s"), m_strDupIndexKey);
	ids = m_pDM_Species->RefreshDataManager(strOption);
	if ( ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해주세요"));
		return -1;
	}

	ids = m_pDM_Index->RefreshDataManager(strOption);		
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해주세요"));
		return -1;
	}


	// 화면 보여주기
	Display();
	
	return 0;
}

INT CBOSpeciesDlg::Init()
{
	INT ids = -1;
	m_bDupAddMode = FALSE;

	// 종 API 초기화
	if (m_pBOSpeciesApi) 
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	if (m_pBOSpeciesApi == NULL)
	{
		m_strLastErrMsg = _T("종 API 초기화 실패");
		return -1;
	}
	
	// 멤버 데이터 메니저 포인터 세팅 및 초기화	
	m_pDM_Index		= FindDM(_T("DM_단행_종_색인정보"));
	m_pDM_Species	= FindDM(_T("DM_단행_종_종정보"));
	m_pDM_Book		= FindDM(_T("DM_단행_종_책정보"));
	m_pDM_Purchase	= FindDM(_T("DM_단행_종_구입정보"));
	//m_pDM_Donation = FindDM(_T("DM_단행_종_기증정보"));

	if (m_pDM_Index == NULL || m_pDM_Species == NULL ||	
		m_pDM_Book == NULL ||m_pDM_Purchase == NULL	)
	{
		m_strLastErrMsg = _T("멤버 데이터 메니저 포인터 세팅 및 초기화 실패");
		return -1;
	}		

	m_pDM_Index->FreeData();
	m_pDM_Species->FreeData();
	m_pDM_Book->FreeData();
	m_pDM_Purchase->FreeData();

	// 콘트롤 메니저 포인터 세팅
	m_pCM_Index		= FindCM(_T("CM_단행_종_색인정보"));
	m_pCM_Purchase	= FindCM(_T("CM_단행_종_구입정보"));

	if (m_pCM_Index == NULL || m_pCM_Purchase == NULL) 
	{
		m_strLastErrMsg = _T("콘트롤 메니저 포인터 세팅 실패");
		return -1;
	}	

	// 자료상태 콤보에 값 넣어주기
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cWORKING_STATUS);
	if (pComboBox != NULL)
	{
		pComboBox->ResetContent();
		pComboBox->AddString(_T("구입대상자료"));
		pComboBox->AddString(_T("검수대상자료"));
		pComboBox->AddString(_T("검수완료자료"));
		pComboBox->SetCurSel(0);

		if (m_strWorkingStatus == _T("구입대상자료"))
		{
			pComboBox->SetCurSel(0);
		}
		else if (m_strWorkingStatus == _T("검수대상자료"))
		{
			pComboBox->SetCurSel(1);
		}
		else if (m_strWorkingStatus == _T("검수완료자료"))
		{
			pComboBox->SetCurSel(2);
		}
	}

	// 기본값 설정가능한 필드 Alias 설정하기
	m_pFormApi->InitDefaultAlias();
	m_pFormApi->AddDefaultAlias(_T("IBS_본표제"));
	m_pFormApi->AddDefaultAlias(_T("IBS_저작자"));
	m_pFormApi->AddDefaultAlias(_T("IBS_판사항"));
	m_pFormApi->AddDefaultAlias(_T("IBS_권서명"));
	m_pFormApi->AddDefaultAlias(_T("IBS_권차"));
	m_pFormApi->AddDefaultAlias(_T("IBS_발행지"));
	m_pFormApi->AddDefaultAlias(_T("IBS_발행자"));
	m_pFormApi->AddDefaultAlias(_T("IBS_발행년"));

	// Default IgnoreList 작성
	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_대표책여부"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_복본기호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_배가위치부호"));	
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));

	// 자동 복본조사 모드 설정하기
	CString strValue;
	GetManageValue(_T("수서"), _T("단행"), _T("수서서명자동복본조사"), _T(""), strValue);
	if (strValue == _T("Y"))
		m_bAutoDupSearch = TRUE;
	else 
	{
		GetDlgItem(IDC_bDUP_SEARCH)->ShowWindow(SW_SHOW);
		m_bAutoDupSearch = FALSE;
	}
	
	/**
		오픈 모드별로 DM 구성
	*/ 

	// 수서-종-구입-입력
	if (GetOpenMode() == 1100)
	{
		// Form API 모드 설정
		m_pFormApi->SetAppMode(MODE_INSERT);

		/**
			1책을 만든다.
		*/

		// 색인
		m_pDM_Index->InsertRow(-1);
		// 종
		m_pDM_Species->InsertRow(-1);
		// 책
		m_pDM_Book->InsertRow(-1);
		// 구입정보
		m_pDM_Purchase->InsertRow(-1);

		// 등록번호를 EM 으로 세팅한다.
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(_T("EM"));
		m_strRegCodeField = _T("EM");

		GetDlgItem(IDC_cWORKING_STATUS)->EnableWindow(TRUE);
		GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_SHOW);
		SetWindowText(_T("수서-종-구입-입력"));

		// 초기값을 불러온다.
		LoadDefaultValue();
	}
	// 수서-종-기증-입력
	else if (GetOpenMode() == 1200)
	{
		SetWindowText(_T("수서-종-기증-입력"));
	}
	// 수서-종-구입-수정
	else if (GetOpenMode() == -1100)
	{
		// 부모 정보 체크
		if (m_pDM_Parent == NULL || m_pGrid_Parent == NULL ||
			m_strSpeciesKeyAlias.IsEmpty() || m_strPurchaseKeyAlias.IsEmpty())
			return -1;

		// Form API 모드 설정
		m_pFormApi->SetAppMode(MODE_UPDATE);

		// 부모그리드의 선택된 로우 구성
		ids = m_pGrid_Parent->SelectMakeList();
		if (ids < 0)
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}

		if (m_nParentSelectedRow == -1)
		{
			m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
			if (m_nParentSelectedRow < 0)
			{
				AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
				return -1;
			}
		}

		m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

		ids = MakeUpdateDM();
		if (ids < 0)
		{
			AfxMessageBox(_T("UPDATE DM 생성 실패"));
			return -1;
		}

		// 종이 소장종인지 판단한다.
		if (m_pDM_Index->GetCellData(_T("IBS_업무구분_코드"), 0) == _T("BOL112N"))
			MakeDupAddMode();
	
		GetDlgItem(IDC_bSAVEAS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bPREV_SPECIES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bNEXT_SPECIES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
		SetWindowText(_T("수서-종-구입-수정"));

		Display();
	}
	// 수서-종-기증-수정
	else if (GetOpenMode() == -1200)
	{
	}
	// 열람-종-입력
	else if (GetOpenMode() == 3100)
	{
		
	}
	// 열람-종-수정
	else if (GetOpenMode() == -3100)
	{
		
	}
	// - 재활용 수정 : -7100
	else if (GetOpenMode() == -7100)
	{
		// 부모 정보 체크
		if (m_pDM_Parent == NULL || m_pGrid_Parent == NULL ||
			m_strSpeciesKeyAlias.IsEmpty() || m_strPurchaseKeyAlias.IsEmpty())
			return -1;

		// Form API 모드 설정
		m_pFormApi->SetAppMode(MODE_UPDATE);

		// 부모그리드의 선택된 로우 구성
		ids = m_pGrid_Parent->SelectMakeList();
		if (ids < 0)
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}

		m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
		if (m_nParentSelectedRow < 0)
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}

		m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

		// DM의 테이블 이름을 바꾸어준다.
		// BO_DELETE_SPECIES_TBL;
		// BO_DELETE_BOOK_TBL;
		m_pDM_Species->TBL_NAME = _T("BO_DELETE_SPECIES_TBL");
		m_pDM_Book->TBL_NAME = _T("BO_DELETE_BOOK_TBL");		

		ids = MakeUpdateDM();
		if (ids < 0)
		{
			AfxMessageBox(_T("UPDATE DM 생성 실패"));
			return -1;
		}

		SetWindowText(_T("재활용 수정"));
		GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
		Display();
	}
	else
	{
		return -1;
	}

	return 0;
}

INT CBOSpeciesDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, CString strPurchaseKeyAlias, INT nSelectedRow /*= -1*/)
{
	if (pDM_Parent == NULL || pGrid_Parent == NULL || strSpeciesKeyAlias.IsEmpty() || strPurchaseKeyAlias.IsEmpty())
		return -1;
	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;
	m_strPurchaseKeyAlias = strPurchaseKeyAlias;
	m_nParentSelectedRow = nSelectedRow;

	return 0;
}

INT CBOSpeciesDlg::ValidateData()
{
	// 꼭 들어가야될 항목
	
	// IBS_권서명 : IDC_eTITLE
	// IBS_저작자 : IDC_eAUTHOR1 없앰
	// 등록구분 : IDC_eREG_CLASS_CODE
	// BP_총책수 : 
	// BP_화폐구분 : IDC_cCURRENCY_CLASS 없앰
	// BP_가격 : IDC_ePRICE 없앰

	const INT nCount = 2;
	CString arrAlias[nCount] = {
		_T("권서명"), 
		_T("등록구분")
	};

	INT arrResource[2] = {
		IDC_eTITLE,
		IDC_eREG_CLASS_CODE
	};

	CString strText;

	CWnd *pWnd = NULL;
	for (INT i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(arrResource[i]);
		if (pWnd == NULL) continue;

		pWnd->GetWindowText(strText);
		if (strText.IsEmpty())
		{
			CString strMsg;
			strMsg.Format(_T("%s 을(를) 입력해 주세요"), arrAlias[i]);
			MessageBox(strMsg);
			pWnd->SetFocus();
			return -1;
		}
	}
	
	CComboBox * pCmb = (CComboBox*)GetDlgItem(IDC_cWORKING_STATUS);
	CString strWorkingStatus;
	if(pCmb)
		pCmb->GetWindowText(strWorkingStatus );

	if(strWorkingStatus == _T("검수대상자료") || strWorkingStatus == _T("검수완료자료"))
	{
		GetControlData(_T("CM_단행_종_구입정보"), _T("BP_화폐구분"), strText);
		strText.TrimLeft();			strText.TrimRight();
		if(strText.IsEmpty())
		{
			MessageBox(_T("화폐구분을 적용해 주세요"));
			GetDlgItem(IDC_cCURRENCY_CLASS)->SetFocus();
			return -1;
		}
		GetControlData(_T("CM_단행_종_구입정보"), _T("BP_가격"), strText);
		strText.TrimLeft();			strText.TrimRight();
		if(strText.IsEmpty() || strText == _T("0"))
		{
			MessageBox(_T("가격정보를 입력해 주세요"));
			GetDlgItem(IDC_ePRICE)->SetFocus();
			return -1;
		}
		GetControlData(_T("CM_단행_종_구입정보"), _T("BP_구입처부호"), strText);
		strText.TrimLeft();			strText.TrimRight();
		if(strText.IsEmpty())
		{
			MessageBox(_T("구입처 정보를 입력해 주세요"));
			GetDlgItem(IDC_cPROVIDER)->SetFocus();
			return -1;
		}
	}
	/*
	GetControlData(_T("CM_단행_종_구입정보"), _T("BP_화폐구분"), strText);
	strText.TrimLeft();
	strText.TrimRight();
	if (strText.IsEmpty())
	{
		MessageBox(_T("화폐구분을 적용해 주세요"));
		GetDlgItem(IDC_cCURRENCY_CLASS)->SetFocus();
		return -1;
	}
	*/
	
	return 0;
}

INT CBOSpeciesDlg::SetAuthorCMToDeletedUnlessWord(CESL_ControlMgr *pCM_Index, CString strCtrlAlias)
{
	CString strAuthor;
	GetControlData(_T("CM_단행_종_색인정보"), strCtrlAlias, strAuthor);
	if(strAuthor.IsEmpty()) return 0;
	GetAuthorToDeletedUnlessWord(strAuthor);
	SetControlData(_T("CM_단행_종_색인정보"), strCtrlAlias, strAuthor);
	return 0;
}

INT CBOSpeciesDlg::GetAuthorToDeletedUnlessWord(CString &strAuthor)
{
	CString strTmpData, strData;
	INT pos = strAuthor.Find(_T(";"), 0);
	if(pos < 0)
	{
		m_pBOSpeciesApi->DeleteUnlessWord(strAuthor);
		return 0;
	}
	strData.Empty();
	while(pos > 0)
	{
		strTmpData = strAuthor.Mid(0, pos);
		strTmpData.TrimLeft();		strTmpData.TrimRight();
		m_pBOSpeciesApi->DeleteUnlessWord(strTmpData);
		if(strData.IsEmpty()) strData = strTmpData;
		else strData += _T(";") + strTmpData;
		strAuthor = strAuthor.Mid(pos+1);
		strAuthor.TrimLeft();		strAuthor.TrimRight();
		pos = strAuthor.Find(_T(";"), 0);
		if(pos < 0)
		{
			m_pBOSpeciesApi->DeleteUnlessWord(strAuthor);
			if(strData.IsEmpty()) strData = strAuthor;
			else strData += _T(";") + strTmpData;
		}
	}

	strAuthor = strData;
	return 0;
}

INT CBOSpeciesDlg::ApplyData()
{
	INT ids = -1;
	CString strVol;

	// 수서-종-구입-입력
	if (GetOpenMode() == 1100)
	{
		// 색인 CM -> 색인 DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Index, 0);

		// 구입정보 CM -> 구입정보 DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Purchase, m_pDM_Purchase, 0);

		// 편권차
		m_pCM_Index->GetControlMgrData(_T("IBS_권차"), strVol);

		// 단권일때 입력된 편권차가 전의 편권차와 다르면 모든 책을 삭제한다.
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{
			if (m_pDM_Book->GetCellData(_T("BB_편권차"), 0) != strVol)
			{
				m_pDM_Book->FreeData();
				m_pDM_Book->InsertRow(-1);
			}
		}

		// 쿼리 플래그
		CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);

		// 표준책 정보 만들기
		MakeStandardBookFromIdx(strVol, strQueryFlag);

		/// 단권일때 종화면 값을 적용시켜준다.
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
			ApplyOneVolumeData(strVol, strQueryFlag);
		
		// Working Status 적용
		ids = ApplyWorkingStatus();

		// 복보추기 모드가 아니면 대표책 하나를 만든다.
		if (!m_bDupAddMode)
		{
			m_pFormApi->SetRepresentationBook(m_pDM_Book);
		}

	}
	// 수서-종-구입-수정
	else if (GetOpenMode() == -1100)
	{
		// 색인 CM -> 색인 DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Index, 0);

		// 구입정보 CM -> 구입정보 DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Purchase, m_pDM_Purchase, 0);

		// 책 DM RowCount
		INT nRowCount = m_pDM_Book->GetRowCount();

		// 편권차
		ids = m_pCM_Index->GetControlMgrData(_T("IBS_권차"), strVol);

		// 단권일때 편권차 일괄 적용
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{						
			for (INT idx = 0; idx < nRowCount; idx++)
				ids = m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, idx);
		}

		/// 쿼리 플래그
		CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE);

		// 복본추기 모드가 아니면 대표책 정보 만들기
		if (m_bDupAddMode)
		{
			// 대표책을 찾는다.
			INT nIdx = -1;
			m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
			
			// 대표책 정보 만들기
			MakeStandardBookFromIdx(strVol, strQueryFlag, nIdx);
		}
				
		/// 단권일때 종화면 값을 적용시켜준다.
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
			ApplyOneVolumeData(strVol, strQueryFlag);
	}
	// - 재활용 수정 : -7100
	else if (GetOpenMode() == -7100)
	{
		// 색인 CM -> 색인 DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Index, 0);

		// 구입정보 CM -> 구입정보 DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Purchase, m_pDM_Purchase, 0);

		// 책 DM RowCount
		INT nRowCount = m_pDM_Book->GetRowCount();

		// 편권차
		ids = m_pCM_Index->GetControlMgrData(_T("IBS_권차"), strVol);

		// 단권일때 편권차 일괄 적용
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{					
			for (INT idx = 0; idx < nRowCount; idx++)
				ids = m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, idx);
		}

		// 쿼리 플래그
		CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE);

		// 복본추기 모드가 아니면 대표책 정보 만들기
		if (m_bDupAddMode)
		{
			// 대표책을 찾는다.
			INT nIdx = -1;
			m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		
			// 대표책 정보 만들기
			MakeStandardBookFromIdx(strVol, strQueryFlag, nIdx);
		}
	}
	else
	{
		return -1;
	}

	// 복본추기 모드이면 종키를 세팅해준다.
	if (m_bDupAddMode)
	{
		INT nRowCount = m_pDM_Book->GetRowCount();
		for (INT idx = 0; idx < nRowCount; idx++)
		{
			//strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));

			m_pDM_Book->SetCellData(_T("BB_종KEY"), m_strDupIndexKey, idx);
			// m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
			m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("N"), idx);
		}		
	}

	
	// 가격정보 적용
	ids = ApplyCalculation();
	if (ids < 0)
	{
		AfxMessageBox(_T("가격정보 적용 실패"));
		return -1;
	}
	else if (ids > 0)
	{
		AfxMessageBox(_T("권별정보에 가격이 빠져있는 권이 있습니다."));
		return -1;
	}

	return 0;
}

INT CBOSpeciesDlg::ApplyIndexToMarc(INT idx /*= 0*/)
{
	INT ids = -1;
	CMarc marc;
	CString strMarc;

	ids = ApplyData();
	if (ids < 0) return ids;

	// 종 -> 마크
	strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	if (!strMarc.IsEmpty())
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0) return -1;
	}
	ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if (ids < 0) return ids;
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
	return ids;
}

INT CBOSpeciesDlg::ApplyRegNoToMarc(INT idx /*= 0*/)
{
	INT ids = -1;
	CMarc marc;
	CString strMarc;
	
	ids = m_pDM_Species->GetCellData(_T("BS_MARC"), idx, strMarc);
	if (ids < 0) return ids;

	ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	if (ids < 0) return ids;

	// 기존의 049Tag을 지운다.
	ids = m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	if (ids < 0) return ids;
	
	// 각각의 책의 등록번호가 있으면 등록번호를 049$l에 넣는다.
	CString strRegNo;
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT i = 0; i < nRowCount; i++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;

		// 등록번호가 없으면 넘어간다.
		strRegNo = m_pDM_Book->GetCellData(_T("BB_등록번호"), i);
		if (strRegNo.IsEmpty()) continue;

		// 등록번호를 049$l에 넣는다.
		ids = m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strRegNo);
	}

	ids = m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if (ids < 0) return ids;

	// 종 DM에 넣기
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, idx);
	return ids;
}

INT CBOSpeciesDlg::MakeStandardBookFromIdx(CString strVol, CString strQueryFlag, INT idx /*= -1*/)
{
	INT ids = -1;
	
	// idx 가 -1이면 첫번째 책을 찾아서 표준정보를 만들어준다.
	if (idx == -1)
	{
		INT nRowCount = m_pDM_Book->GetRowCount();
		for (INT i = 0; i < nRowCount; i++)
			if (m_pFormApi->IsBook(m_pDM_Book, i)) break;
		
		idx = i;
	}

	/// 색인정보로 부터 표준 정보 만들기
	if(!m_bViewVolInfo || !m_bDupAddMode)
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"));
	/// 편권차 적용
	ids = m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, idx);
	/// 쿼리 플래그 적용
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

	return 0;
}

INT CBOSpeciesDlg::ApplyOneVolumeData(CString strVol, CString strQueryFlag)
{
	// 단권일때 종화면 값을 적용시켜준다.

	// 등록구분 적용시키기
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0 || arrRegCode.GetSize() <= 0)
	{
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		m_strLastErrMsg = _T("등록구분 필드가 잘못되었습니다.");
		return -1;
	}

	m_strRegCodeField = strRegCodeField;
	m_pFormApi->MakeBookFromRegCode(&arrRegCode, m_pDM_Book, strVol);

	// 색인정보로 부터 책 정보 적용시키기
	INT nBookCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		
		// 색인 CM -> 책 DM
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"));

		// 편권차 적용
		ids = m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, idx);

		// 쿼리 플래그 적용
		ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

		//단권본일때 가격잡정보 책에 적용시키기
		ids = m_pDM_Book->SetCellData(_T("BB_가격잡정보"), m_pDM_Purchase->GetCellData(_T("BP_가격잡정보"), 0), idx);
	}

	return 0;
}

INT CBOSpeciesDlg::ApplyWorkingStatus()
{
	INT idx = 0;
	INT nRowCount = 0;

	CString strWorkingStatus;
	CString strBookWorkingStatus;		
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cWORKING_STATUS);
	if (pComboBox == NULL) goto ERR;
	
	pComboBox->GetLBText(pComboBox->GetCurSel(), strWorkingStatus);
	strWorkingStatus = m_pFormApi->ConvertWORKING_STATUSToCode(strWorkingStatus);
	if (strWorkingStatus.IsEmpty()) goto ERR;	


	if (GetOpenMode() > 0)
	{
		// 종에 Working Status를 넣는다.
		m_pDM_Species->SetCellData(_T("BS_자료상태_코드"), strWorkingStatus, 0);

		// 색인에 Working Status를 넣는다.
		m_pDM_Index->SetCellData(_T("IBS_작업상태INFO"), strWorkingStatus, 0);
	}
	
	// 책에 Working Status를 넣는다.
	nRowCount = m_pDM_Book->GetRowCount();

	strBookWorkingStatus = _T("");
	for (idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		// 책에 이미 작업상태가 설정된 상태라면 continue
		m_pDM_Book->GetCellData(_T("BB_자료상태"), idx, strBookWorkingStatus);
		if(strBookWorkingStatus.IsEmpty())
			m_pDM_Book->SetCellData(_T("BB_자료상태"), strWorkingStatus, idx);
	}

	return 0;

ERR:
	m_strLastErrMsg = _T("Working Status적용 실패");
	return -1;	
}

INT CBOSpeciesDlg::ApplyCalculation()
{
	INT ids = -1;
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_종_구입정보"));
	ids = m_pFormApi->MakeDMFromCM(pCM, m_pDM_Purchase, 0);

	CString strPriceType;
	ids = pCM->GetControlMgrData(_T("BP_주문정산타입"), strPriceType);
	if (ids < 0) return -1;

	CString strTemp;

	INT nResult = 0;
	INT nDiscountRate = 0;
	ids = pCM->GetControlMgrData(_T("BP_할인율"), strTemp);
	if (ids < 0) return -1;
	nDiscountRate = _ttoi(strTemp);

	INT nPrice = 0;
	// 가격 == 종가격의 값
	if (strPriceType == _T("종가격"))
	{
		ids = pCM->GetControlMgrData(_T("BP_가격"), strTemp);
		if (ids < 0) return -1;

		nPrice = _ttoi(strTemp);

		// 가격을 1/n으로 나어 각각의 책에 적용시킨다.
		ids = m_pFormApi->ApplySpeciesPriceToBookPrice(m_pDM_Book, nPrice);
		if (ids < 0) return -1;
	}
	// 가격 == 각각 책의 가격을 합한값
	else if (strPriceType == _T("권가격"))
	{
		ids = m_pFormApi->GetBookPriceSum(m_pDM_Book, nPrice);
		if (ids < 0) return -1;
		nResult = ids;
		strTemp.Format(_T("%d"), nPrice);
		pCM->SetControlMgrData(_T("BP_가격"), strTemp);	
	}
	else
	{
		return -1;
	}

	// 발주가격 계산
	// 발주가격 = 가격 * (1 - 할인율)
	nPrice = (nPrice * (100 - nDiscountRate)) / 100;

	strTemp.Format(_T("%d"), nPrice);
	ids = pCM->SetControlMgrData(_T("BP_발주가격"), strTemp);

	return nResult;
}

INT CBOSpeciesDlg::Display()
{
	// 수서-종-복본추기
	if (m_bDupAddMode)
	{
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Purchase->SetControlMgrData(_T("BP_총권수"), strVolumeCount);
		
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_eVOL)->EnableWindow(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_eVOL)->EnableWindow(FALSE);
		}

		// 색인 정보 Disable
		m_pCM_Index->AllControlReadOnly();
		
		// 종정보 Disable
		CESL_ControlMgr *pCM = FindCM(_T("CM_단행_종_종정보"));
		if (pCM)
		{
			pCM->AllControlReadOnly();
		}
		
		AllControlDisplay(_T("CM_단행_종_색인정보"), 0);
		AllControlDisplay(_T("CM_단행_종_종정보"), 0);
	}
	// 수서-종-구입-입력
	else if (GetOpenMode() == 1100)
	{
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Purchase->SetControlMgrData(_T("BP_총권수"), strVolumeCount);
		
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			GetDlgItem(IDC_eVOL)->EnableWindow(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_eVOL)->EnableWindow(FALSE);
		}

		
		// 대표책을 찾는다.
		INT nIdx = -1;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		// 대표책을 색인에 넣는다.
		m_pFormApi->CopyDM(m_pDM_Book, nIdx, m_pDM_Index, 0, _T("IBS"));
		// 구입정보중 가격잡정보적용 -- 단권본일경우
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{
			m_pDM_Purchase->SetCellData(_T("BP_가격잡정보"), m_pDM_Book->GetCellData(_T("BB_가격잡정보"), nIdx), 0);
			SetControlData(_T("CM_단행_종_색인정보"), _T("BP_가격잡정보"), m_pDM_Book->GetCellData(_T("BB_가격잡정보"), nIdx));
		}
		AllControlDisplay(_T("CM_단행_종_색인정보"), 0);

		// 대표책의 편권차 표시
		CString strVol;
		m_pDM_Book->GetCellData(_T("BB_편권차"), nIdx, strVol);
		m_pCM_Index->SetControlMgrData(_T("IBS_권차"), strVol);

	}
	// 수서-종-기증-입력
	else if (GetOpenMode() == 1200)
	{
	}
	// 수서-종-구입-수정
	else if (GetOpenMode() == -1100)
	{
		AllControlDisplay(_T("CM_단행_종_구입정보"), 0);
		
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount, strPriceType;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Purchase->SetControlMgrData(_T("BP_총권수"), strVolumeCount);
		//주문정산타입에 따라서 CM의 주문정산타입 콤보컨트롤을 설정한다.
		m_pDM_Purchase->GetCellData(_T("BP_주문정산타입"), 0, strPriceType);
		CESL_ControlComboBox * pCombo = (CESL_ControlComboBox*)FindControl(_T("CM_단행_종_구입정보"), _T("BP_주문정산타입"));
		if(strPriceType.IsEmpty() || strPriceType == _T("종가격"))
			pCombo->SetCurSel(0);
		else 
			pCombo->SetCurSel(1);
		
		// 대표책을 찾는다.
		INT nIdx = -1;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		
		m_pDM_Index->GetCellData(_T("IBS_저작자"), 0);
		// 대표책을 색인에 넣는다.
		m_pFormApi->CopyDM(m_pDM_Book, nIdx, m_pDM_Index, 0, _T("IBS"));
		m_pDM_Index->GetCellData(_T("IBS_저작자"), 0);
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{
			m_pDM_Purchase->SetCellData(_T("BP_가격잡정보"), m_pDM_Book->GetCellData(_T("BB_가격잡정보"), nIdx), 0);
			SetControlData(_T("CM_단행_종_구입정보"), _T("BP_가격잡정보"), m_pDM_Book->GetCellData(_T("BB_가격잡정보"), nIdx));
		}

		AllControlDisplay(_T("CM_단행_종_색인정보"), 0);

		// 대표책의 편권차 표시
		CString strVol;
		m_pDM_Book->GetCellData(_T("BB_편권차"), nIdx, strVol);
		m_pCM_Index->SetControlMgrData(_T("IBS_권차"), strVol);
	}
	// 수서-종-기증-수정
	else if (GetOpenMode() == -1200)
	{
	}
	// 열람-종-입력
	else if (GetOpenMode() == 3100)
	{
	}
	// 열람-종-수정
	else if (GetOpenMode() == -3100)
	{
	}
	else
	{
		return -1;
	}
	return 0;
}

INT CBOSpeciesDlg::ShowRegCodeField()
{
	CArray<CRegCode, CRegCode> arrRegCode;
	m_pFormApi->GetRegCodeInfo(m_pDM_Book, &arrRegCode);
	CRegCode regCode;
	CString strTemp;
	CString strRegCodeField;
	INT nBookCount = 0;
	INT nArrCount = arrRegCode.GetSize();
	if (nArrCount != 0)
	{
		for (INT i = 0; i < nArrCount - 1; i++)
		{
			regCode = arrRegCode.GetAt(i);
			nBookCount += regCode.m_nCount;
			strTemp.Format(_T("%s%d/"), regCode.m_strRegCode, regCode.m_nCount);
			strRegCodeField += strTemp;
		}
		
		regCode = arrRegCode.GetAt(nArrCount - 1);	
		nBookCount += regCode.m_nCount;
		strTemp.Format(_T("%s%d"), regCode.m_strRegCode, regCode.m_nCount);
		strRegCodeField += strTemp;

		// 등록구분 필드
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);

		// 총 책수
		CString strBookCount;
		strBookCount.Format(_T("%d"), nBookCount);

		m_pCM_Purchase->SetControlMgrData(_T("BP_총책수"), strBookCount);
	}

	return 0;
}

INT CBOSpeciesDlg::MakeUpdateDM()
{
	if (m_nParentSelectedRow < 0) return -1;

	INT ids = -1;
	
	// 선택한 Row의 SpeciesKey, PurchaseKey 가져오기
	CString strSpeciesKey;
	CString strPurchaseKey;

	m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);
	m_pDM_Parent->GetCellData(m_strPurchaseKeyAlias, m_nParentSelectedRow, strPurchaseKey);

	CString strOption;

	if (strSpeciesKey.IsEmpty()) return -1;
	if (strPurchaseKey.IsEmpty()) return -1;
	
	// 색인 DM 구성
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pDM_Index->RefreshDataManager(strOption);
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해주세요"));
		return -1;
	}
	
	// 종 DM 구성
	strOption.Format(_T("REC_KEY = %s"), strSpeciesKey);
	ids = m_pDM_Species->RefreshDataManager(strOption);
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해주세요"));
		return -1;
	}

	// 구입정보 DM 구성
	strOption.Format(_T("REC_KEY = %s"), strPurchaseKey);
	ids = m_pDM_Purchase->RefreshDataManager(strOption);
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해주세요"));
		return -1;
	}

	// 책 정보 DM 구성
	strOption.Format(_T("SPECIES_KEY = %s AND ACCU_KEY = %s AND REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY REPRESENT_BOOK_YN DESC, VOL"), strSpeciesKey, strPurchaseKey);
	ids = m_pDM_Book->RefreshDataManager(strOption);
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해주세요"));
		return -1;
	}

	return 0;
}

INT CBOSpeciesDlg::SaveDefaultValue()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_종_색인정보"));
	if (pCM == NULL) return -1;
	
	CString strFormCode;
	m_pFormApi->SaveDefaultValue(pCM, m_strDefaultValueFilePath);
	return 0;
}

INT CBOSpeciesDlg::LoadDefaultValue()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_종_색인정보"));
	if (pCM == NULL) return -1;
	
	m_pFormApi->LoadDefaultValue(pCM, m_strDefaultValueFilePath);
	
	return 0;
}

VOID CBOSpeciesDlg::MakeGroupNameList()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_종_색인정보"));
	if (pCM == NULL) return ;
	CString strAcqYear;
	pCM->GetControlMgrData(_T("Group년도"), strAcqYear);
	if(strAcqYear.IsEmpty()) strAcqYear = m_pBOSpeciesApi->GetTodayYear();
	//Group메니저의 메소드를 사용하여 CString[]의 Group이름 리스트를 추출한다.
}

/////////////////////////////////////////////////////////////////////////////
// CBOSpeciesDlg message handlers

BOOL CBOSpeciesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Check Open Mode
	if (GetOpenMode() == -1)
	{
		AfxMessageBox(_T("Open Mode를 설정해 주세요"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Initialize Esl Manger	
	INT ids = InitESL_Mgr(_T("SM_단행_종"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// Initialize Data Mangager
	ids = Init();
	if (ids < 0)
	{
		MessageBox(GetLastError());
		EndDialog(IDCANCEL);
		return FALSE;
	}

	// 윈도우 rect 저장
	GetWindowRect(m_rcWindow);

	// 코드 구분값 콘트롤 보여주기
	m_bShowCode = FALSE;
	ShowCodeResource();

	// 주제구분 표시
	OnSelchangecCLASIFYCLASS();

	//Group콤보 리스트 데이터 구축
	MakeGroupNameList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOSpeciesDlg::OnbSHOWHIDECODE() 
{
	if (m_bShowCode)
	{
		m_bShowCode = FALSE;
	}
	else
	{
		m_bShowCode = TRUE;
	}

	ShowCodeResource();
}

VOID CBOSpeciesDlg::OnbCLOSE() 
{
	CDialog::OnOK();	
}

VOID CBOSpeciesDlg::OnSelchangecCLASIFYCLASS() 
{
	//바뀔때마다 분류 구분을 바꾸어 준다. 

    CESL_ControlMgr *pCM_SPECIES = FindCM(_T("CM_단행_종_구입정보"));

	CESL_ControlMgr *pCM_Index = FindCM(_T("CM_단행_종_색인정보"));
	CESL_ControlMgr *pCM_Purchase = FindCM(_T("CM_단행_종_구입정보"));
	if (pCM_Index == NULL || pCM_Purchase == NULL) return;
	
	CESL_ControlMultiComboBox *pCM_WhichCode = (CESL_ControlMultiComboBox *)pCM_Index->FindControl(_T("IBS_분류표구분"));
	if (!pCM_WhichCode) return;
    CESL_ControlMultiComboBox *pCM_Subject = (CESL_ControlMultiComboBox *)pCM_Purchase->FindControl(_T("BP_주제구분_코드"));
	if (!pCM_Subject) return;
  
	CString class_name;
	CString strWhichCode;
	pCM_WhichCode->GetData(strWhichCode, pCM_WhichCode->GetCurSel(), 1);
	strWhichCode.TrimLeft();		strWhichCode.TrimRight();
	strWhichCode.MakeUpper();
	class_name = _T("");
	if (strWhichCode == _T("KDC")) class_name = _T("KDC대주제부호관리");
	if (strWhichCode == _T("DDC")) class_name = _T("DDC대주제부호관리");

	pCM_Subject->FreeComboData();
	pCM_Subject->ResetContent();
	pCM_Subject->InitMultiComboBox(2);
	if (class_name.GetLength() == 0) return ;

	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	nDefaultValue = 0;
	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if (nCode == 0) return ;



	CString strDefaultValue = m_pDM_Purchase->GetCellData(_T("BP_주제구분_코드"), 0);
		
	for (INT i = 0; i < nCode; i++) {
		if (strCode[i].GetLength() == 0) strCode[i] = _T("  ");
		
		if (strDefaultValue == strCode[i]) nDefaultValue = i;

		
		pCM_Subject->AddItem(strCode[i], 0, i);
		pCM_Subject->AddItem(strDesc[i], 1, i);
	}

	pCM_Subject->SetCurSel(nDefaultValue);	
}

VOID CBOSpeciesDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_pFormApi != NULL)
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}

	if (m_pBOSpeciesApi != NULL)
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}

	if (m_pNoteCtrl != NULL)
	{
		m_pNoteCtrl->DestroyWindow();
		delete m_pNoteCtrl;
	}
}

VOID CBOSpeciesDlg::OnbDEFAULTCONFIG() 
{
	if (SaveDefaultValue() < 0)
		AfxMessageBox(_T("기본값 저장 실패"));
	else
		AfxMessageBox(_T("기본값을 저장하였습니다."), MB_ICONINFORMATION);
}

VOID CBOSpeciesDlg::OnbPREVSPECIES() 
{
	if (m_nParentSelectedRow < 0) return;
	INT nIdx = m_pGrid_Parent->SelectGetPrev();
	if (nIdx >= 0)
		m_nParentSelectedRow = nIdx;		
	else
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
	if (m_nParentSelectedRow < 0) return;

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	if (MakeUpdateDM() < 0)
	{
		AfxMessageBox(_T("UPDATE DM 생성 실패"));
		return;
	}

	Display();
}

VOID CBOSpeciesDlg::OnbNEXTSPECIES() 
{
	if (m_nParentSelectedRow < 0) return;
	INT nIdx = m_pGrid_Parent->SelectGetNext();
	m_nParentSelectedRow = nIdx;
	if (nIdx >= 0)
		m_nParentSelectedRow = nIdx;
	else
		m_nParentSelectedRow = m_pGrid_Parent->SelectGetTailPosition();
	if (m_nParentSelectedRow < 0) return;

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	if (MakeUpdateDM() < 0)
	{
		AfxMessageBox(_T("UPDATE DM 생성 실패"));
		return;
	}
	Display();
}

VOID CBOSpeciesDlg::OnbSAVEAS() 
{
	if (GetOpenMode() != -1100) return;

	// 경고메시지
	if (MessageBox(_T("새로저장 작업을 하시면 더이상 수정 작업을 할 수 없습니다.\r\n"
				_T("계속하시겠습니까?")), NULL, MB_YESNO | MB_ICONQUESTION) != IDYES) return;

	// 책의 U -> I
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 플래그가 U가 아니면 넘어간다.
		if (m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) != _T("U")) continue;

		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
	}

	/**
		저장 플로우
	*/
	
	// 데이터 확인
	INT ids = ValidateData();
	if (ids < 0) return;

	// 입력한 데이터 적용
	SetOpenMode(1100);
	ids = ApplyData();
	if (ids < 0)
	{
		AfxMessageBox(_T("데이터 적용실패"));
		return;
	}

	CESL_DataMgr *pDM_TARGET = NULL;
	if (pParentMgr->SM_ALIAS == _T("K2UP_LIGHT_단행_수서")) 
		pDM_TARGET = pParentMgr->FindDM(_T("DM_LIGHT_단행_수서_종"));


	// 마크 오퍼레이션
	ids = ApplyIndexToMarc();
	if (ids < 0)
	{
		AfxMessageBox(_T("저장 실패"));
		EndDialog(IDOK);
		return;
	}

	// 적용
	ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_INSERT(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0, pDM_TARGET);
	if (ids < 0)
		AfxMessageBox(_T("저장 실패"));
	else
	{
		MessageBox(_T("저장 하였습니다"), NULL, MB_OK | MB_ICONINFORMATION);	
		m_bControlDisplay = TRUE;
		OnbCLEARSCREEN();
		LoadDefaultValue();			
	}

	// 닫기
	EndDialog(IDOK);
}

VOID CBOSpeciesDlg::OnbSAVE() 
{
	// 데이터 확인
	INT ids = ValidateData();
	if (ids < 0) return;

	// 입력한 데이터 적용
	ids = ApplyData();
	if (ids < 0)
	{
		AfxMessageBox(_T("데이터 적용실패"));
		return;
	}	

	CESL_DataMgr *pDM_TARGET = NULL;
	if (pParentMgr->SM_ALIAS == _T("K2UP_LIGHT_단행_수서")) 
		pDM_TARGET = pParentMgr->FindDM(_T("DM_LIGHT_단행_수서_종"));
	
	
	// 종 API에 오픈모드 설정
	m_pBOSpeciesApi->m_nOpenMode = GetOpenMode();

	CMarc marc;
	CString strMarc;

	// 복본추기
	if (m_bDupAddMode)
	{
		// OpenMode에 따라서 복본추기 함수를 불러온다. 
		if (GetOpenMode() == 1100)
		{
			ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_ADDBOOK(_T("INSERT"), m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		}
		else if (GetOpenMode() == -1100)
		{
			ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_ADDBOOK(_T("UPDATE"), m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		}

		if (ids < 0)
		{
			AfxMessageBox(_T("저장 실패"));
			return;
		}
		else
		{
			MessageBox(_T("저장 하였습니다."), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
		}
	}
	// 수서-종-구입-입력
	else if (GetOpenMode() == 1100)
	{
		// 마크 오퍼레이션
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("저장 실패"));
			return;
		}
		ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_INSERT(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0, pDM_TARGET);
		if (ids < 0)
			AfxMessageBox(_T("저장 실패"));
		else
		{
			if (m_pGrid_Parent) m_pGrid_Parent->DisplayLine(pDM_TARGET->GetRowCount()-1);
			MessageBox(_T("저장 하였습니다"), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			
			OnbCLEARSCREEN();
			LoadDefaultValue();			
		}
	}
	// 수서-종-구입-수정
	else if (GetOpenMode() == -1100)
	{
		// 마크 오퍼레이션
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("저장 실패"));
			return;
		}
		
		ids = m_pBOSpeciesApi->BO_SPECIES_PURCHASE_UPDATE(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0, pDM_TARGET, m_nParentSelectedRow);
		if (ids < 0)
			AfxMessageBox(_T("수정 실패"));
		else
		{
			if (m_pGrid_Parent) m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);
			MessageBox(_T("수정 하였습니다"), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			Init();
		}
	}
	// - 재활용 수정 : -7100
	else if (GetOpenMode() == -7100)
	{
		// 마크 오퍼레이션
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("저장 실패"));
			return;
		}

		ids = m_pBOSpeciesApi->BO_DELETE_SPECIES_PURCHASE_UPDATE(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		if (ids < 0)
			AfxMessageBox(_T("수정 실패"));
		else
		{
			MessageBox(_T("수정 하였습니다"), NULL, MB_OK | MB_ICONINFORMATION);
			m_bControlDisplay = TRUE;
		}
	}
}

VOID CBOSpeciesDlg::OnbMARCEDIT() 
{
	INT ids = -1;

	CMarc marc;
	CString strMarc;
	ids = m_pDM_Species->GetCellData(_T("BS_MARC"), 0, strMarc);
	if (!strMarc.IsEmpty())
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0) return;
	}

	// 리더 정보 세팅 (긴급추가임)
	CString strLeader;
	marc.GetLeader(strLeader);
	// 레코드 상태 없으면 넣는다. 5번째
	if (strLeader.GetAt(5) == ' ')
		strLeader.SetAt(5, 'n');

	// 레코드 형태 없으면 넣는다. 6번째
	if (strLeader.GetAt(6) == ' ')
		strLeader.SetAt(6, 'a');

	// 서지수준 없으면 넣는다. 7반째
	if (strLeader.GetAt(7) == ' ')
		strLeader.SetAt(7, 'm');

	marc.SetLeader(strLeader.GetBuffer(0));


	// 복본추기 모드일경우는 마크보기만 된다.
	if (m_bDupAddMode)
	{
		CBOMarcEditDlg dlg(this, &marc);
		dlg.SetReadOnly();
		dlg.DoModal();
	}
	else
	{
		ids = ApplyData();
		if (ids < 0) return;

		// 종 -> 마크
		ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		
		m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
		
		// 마트 편집 다이얼로그 띄우기
		CBOMarcEditDlg dlg(this, &marc);
		dlg.DoModal();

		// 마크 -> 종
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		
		// 종 Marc로
		m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
		m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);

		Display();
	}
}

VOID CBOSpeciesDlg::OnbMARCIMPORT() 
{
	CMARC_IMPORT dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		INT ids = -1;

		if (dlg.m_strSpecies_key.IsEmpty()) goto ERR;

		CString strMarc;
		CString strQuery;
		strQuery.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), dlg.m_strSpecies_key);
		ids = m_pDM_Species->GetOneValueQuery(strQuery, strMarc);
		if (ids < 0) goto ERR;

		CMarc marc;
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0) goto ERR;

		// 공통 : Marc to Index
		//ids = m_pBOSpeciesApi->CMARC_to_INDEX(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
		ids = m_pBOSpeciesApi->CMARC_to_INDEX(&marc, m_pDM_Index, 0);
		if (ids < 0) goto ERR;

		//0: 기본 정보 반입 -> 마크로 IDX만 구성 (마크는 버림)
		if (dlg.m_nRadBasicInfo == 0)
		{
			//색인 정보 -> 대표책정보 -> 책에 저장될때 대표저자만 들어가야 한다.
			m_pFormApi->CopyDM(m_pDM_Index, 0, m_pDM_Book, 0, _T("BB"));
		}
		//1: 전체 정보 반입 -> 마크로 IDX구성 MARC도 가져옴
		else if (dlg.m_nRadBasicInfo == 1 || dlg.m_nRadBasicInfo == 2)
		{
			ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
			if (ids < 0) goto ERR;

			//2: 다권정보삭제한 전체정보반입 -> 1 + 책도 가져온다.
			if (dlg.m_nRadBasicInfo == 2)
			{
				CString strOption;
				strOption.Format(_T("SPECIES_KEY = %s"), dlg.m_strSpecies_key);
				ids = m_pDM_Book->RefreshDataManager(strOption);
				if (ids < 0) goto ERR;
			}
		}
		m_pFormApi->SetRepresentationBook(m_pDM_Book);
		
		Display();
		AfxMessageBox(_T("Marc를 반입하였습니다."), MB_ICONINFORMATION);
		return;
	}

	return;

ERR:
	AfxMessageBox(_T("Marc 반입 실패"));
	return;

}

VOID CBOSpeciesDlg::OnbKOLISNET() 
{
	// TODO: 공동목록 반입
	CUC_MARC_IMPORT dlg(this);
	if (dlg.DoModal() != IDOK) return;

	CMarc marc;
	
	if(dlg.sMarc.IsEmpty()) return;
	INT ids = m_pInfo->pMarcMgr->Decoding(dlg.sMarc, &marc);
	if (ids < 0) goto ERR;

	// 마크로 IDX 구성 (마크는 버림)
	//ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Purchase, 0);
	ids = m_pBOSpeciesApi->CMARC_to_INDEX(&marc, m_pDM_Index, 0);
	if (ids < 0) goto ERR;
	
	// MARC 가져와 세팅
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), dlg.sMarc, 0);
	if (ids < 0) goto ERR;
	
	m_pFormApi->SetRepresentationBook(m_pDM_Book);

	Display();
	MessageBox(_T("공동목록을 반입하였습니다."));
	return;

ERR :
	MessageBox(_T("공동목록 반입실패"));
	
}

VOID CBOSpeciesDlg::OnbCLEARSCREEN() 
{
	//Init();
	CESL_ControlMgr *pCM = NULL;
	pCM = FindCM(_T("CM_단행_종_구입정보"));
	if (pCM)
	{
		pCM->AllControlClear();
	}

	
	pCM = FindCM(_T("CM_단행_종_색인정보"));
	if (pCM)
	{
		pCM->AllControlClear();
	}

	//Init();
	OnKillfocuseREGCLASSCODE();
	//Display();
}

VOID CBOSpeciesDlg::OnbVOLINFO() 
{
	// 입력한 데이터 적용
	ApplyData();

	// 권 화면 띄우기
	CBOVolumeDlg dlg(this, m_pDM_Book);
	dlg.SetOpenMode(GetOpenMode());
	if (m_bDupAddMode)
	{
		dlg.SetDupIdxKey(m_strDupIndexKey);
	}
	dlg.DoModal();

	ApplyCalculation();
	ApplyRegNoToMarc();
	
	//단권일 경우 대표책의 정보를 색인으로 복사하여야한다.
	Display();
	m_bViewVolInfo = TRUE;
}

VOID CBOSpeciesDlg::OnbDUPSEARCH() 
{
	CString strSearchCount;
	SearchDuplicateDataByTitle(_T("서명복본조사"), strSearchCount);
	if(strSearchCount.IsEmpty() || strSearchCount == _T("0"))
	{
		ESLAfxMessageBox(_T("서명복본조사 결과가 없습니다."));
		return;
	}
	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("서명자동복본조사"), m_strDupIndexKey);
	if (!m_strDupIndexKey.IsEmpty())
		MakeDupAddMode();
}

VOID CBOSpeciesDlg::OnbEXTRAISBN() 
{
	CString strAdditionISBN = m_pDM_Index->GetCellData(_T("IBS_ISBN"), 0);
	
	CBOISBNAdditionDlg dlg(this);
	dlg.SetAdditionISBN(strAdditionISBN);
	if (dlg.DoModal() == IDOK)
	{
		m_pDM_Index->SetCellData(_T("IBS_ISBN"), dlg.GetAdditionISBN(), 0);
	}
}

VOID CBOSpeciesDlg::OnbISBNCHECK() 
{
	// 낱권 체크
	CString strEaISBNFrom;
	CString strEaISBNTo;
	INT ids = GetControlData(_T("CM_단행_종_색인정보"), _T("IBS_낱권ISBN"), strEaISBNFrom);
	if (ids >= 0)
	{
		ids = m_pFormApi->CheckISBN(strEaISBNFrom, strEaISBNTo);
		if (ids >= 0)
			SetControlData(_T("CM_단행_종_색인정보"), _T("IBS_낱권ISBN"), strEaISBNTo);
	}

	// 세트 체그
	CString strSetISBNFrom;
	CString strSetISBNTo;
	ids = GetControlData(_T("CM_단행_종_색인정보"), _T("IBS_세트_ISBN1"), strSetISBNFrom);
	if (ids >= 0)
	{
		ids = m_pFormApi->CheckISBN(strSetISBNFrom, strSetISBNTo);
		if (ids >= 0)
			SetControlData(_T("CM_단행_종_색인정보"), _T("IBS_세트_ISBN1"), strSetISBNTo);
	}
}

VOID CBOSpeciesDlg::OnKillfocuseREGCLASSCODE() 
{
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		MessageBox(_T("없는 등록구분입니다."));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		return;
	}

	INT nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount <= 0) return;

	// 등록구분 바뀌는지 체크
	CString strVol;
	GetControlData(_T("CM_단행_종_색인정보"), _T("IBS_권차"), strVol);
	if (m_pFormApi->IsRegCodeModify(&arrRegCode, m_pDM_Book, strVol))
	{
		AfxMessageBox(_T("등록구분이 수정이 될수 있습니다. 책정보에서 수정해주세요"));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		return;			
	}

	m_strRegCodeField = strRegCodeField;

	INT nBookCount = 0;
	for (INT i = 0; i < nRegCodeCount; i++)
	{
		nBookCount += arrRegCode.GetAt(i).m_nCount;
	}

	CString strBookCount;
	strBookCount.Format(_T("%d"), nBookCount);

	SetControlData(_T("CM_단행_종_구입정보"), _T("BP_총책수"), strBookCount);
	m_bViewVolInfo = FALSE;
}

VOID CBOSpeciesDlg::OnSelchangecPRICETYPE() 
{
	ApplyCalculation();
}

VOID CBOSpeciesDlg::OnKillfocusePRICE() 
{
	ApplyCalculation();
}

VOID CBOSpeciesDlg::OnKillfocusePRICEDC() 
{
	ApplyCalculation();
}

VOID CBOSpeciesDlg::OnbAPPLYSERIES() 
{
	if (MessageBox(_T("책을 전집으로 만듭니다. 계속하시겠습니까?"), NULL, MB_YESNO | MB_ICONQUESTION) == IDNO) return;

	// ApplyData
	if (ApplyData() < 0)
	{
		MessageBox(_T("전집여부적용 실패"));
		return;
	}

	// 모든 책을 1권 1책으로 만든다.
	CString strVol;
	m_pFormApi->ApplyCompleteWorks(m_pDM_Book, strVol);

	// 색인과 화면에 적용
	m_pDM_Index->SetCellData(_T("BB_편권차"), strVol, 0);
	SetControlData(_T("CM_단행_종_색인정보"), _T("IBS_권차"), strVol);

	MessageBox(_T("책을 전집으로 만들었습니다."));

	Display();
	m_bViewVolInfo = TRUE;
}

UINT CBOSpeciesDlg::OnNoteClicked(LPARAM lParam, WPARAM wParam)
{
	// 복본 조사 들어간당
	//OnbDUPSEARCH();
	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("서명자동복본조사"), m_strDupIndexKey);
	if(!m_strDupIndexKey.IsEmpty())
		MakeDupAddMode();

	return 0;
}

VOID CBOSpeciesDlg::OnKillfocuseTITLE() 
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_eTITLE);
	if (pEdit == NULL) return;

	// 리드 온리이면 체크하지 않는다.
	if (pEdit->GetStyle() & ES_READONLY) return;

	SetTimer(0, 1000, NULL);	
}

VOID CBOSpeciesDlg::OnTimer(UINT nIDEvent) 
{
	// 서명 자동복본조사
	if (nIDEvent == 0)
	{
		KillTimer(0);

		if (m_bAutoDupSearch == FALSE) return;

		CString strSearchCount;

		SearchDuplicateDataByTitle(_T("서명자동복본조사"), strSearchCount);

		if (strSearchCount.IsEmpty() || strSearchCount == _T("0"))	return;
		
		if (m_pNoteCtrl != NULL)
		{
			delete m_pNoteCtrl;
		}
		
		m_pNoteCtrl = new CNoteCtrl;
		CString strNote;
		strNote.Format(_T("%s건의 복본이 발견되었습니다."), strSearchCount);
		m_pNoteCtrl->SetNote(strNote);

		CRect rcButton;
		GetDlgItem(IDC_bDUP_SEARCH)->GetWindowRect(rcButton);
		ScreenToClient(rcButton);
				
		rcButton.top += 50;
		rcButton.bottom += 60;

		rcButton.right += 60;

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		point.y += 5;
		
		m_pNoteCtrl->Create(point, 200, 50, this);
		m_pNoteCtrl->ShowWindow(SW_SHOW);
	}
	
	CDialog::OnTimer(nIDEvent);
}


VOID CBOSpeciesDlg::OnSelchangecFORMCLASS() 
{
	CString strOriginFormClass;
	CString strFormClass;

	m_pDM_Index->GetCellData(_T("IBS_형식구분_코드"), 0, strOriginFormClass);
	GetControlData(_T("CM_단행_종_색인정보"), _T("IBS_형식구분_코드"), strFormClass);

	if (strOriginFormClass == strFormClass) return;

	if (strFormClass == _T(""))
	{
		MessageBox(_T("형식구분을 선택하셔야 됩니다."));
		GetDlgItem(IDC_cFORM_CLASS)->SetFocus();
		return;
	}
	else if (strFormClass == _T("BK"))
	{
		m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
		MessageBox(_T("도서로 바뀌었습니다."));
	}
	else if (strFormClass == _T("VM") ||
			 strFormClass == _T("CF") ||
			 strFormClass == _T("MP") ||
			 strFormClass == _T("MU")
			 )
	{
		m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
		MessageBox(_T("비도서로 바뀌었습니다."));
	}
	
	m_pDM_Index->SetCellData(_T("IBS_형식구분_코드"), strFormClass, 0);

}

VOID CBOSpeciesDlg::SearchDuplicateDataByTitle(CString strSearchType, CString &strSearchCount)
{
	CString strTitle;

	INT ids = GetControlData(_T("CM_단행_종_색인정보"), _T("IBS_본표제"), strTitle);
	if (ids < 0) return ;

	if (strTitle.IsEmpty()) return ;
	INT nFind = strTitle.Find(_T("="));
	if (nFind >= 0)
	{
		strTitle = strTitle.Mid(0, nFind);
		nFind = strTitle.Find(_T(":"));
		if(nFind >= 0)
			strTitle = strTitle.Mid(0, nFind);
	}

	if (strTitle.IsEmpty()) return ;

	// 수정모드일때 자신의 종키를 가져온다.
	CString strSpeciesKey;
	if (GetOpenMode() == -1100 && m_nParentSelectedRow >= 0)
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);

	CDuplicateSearch_Interface impl(this);
	ids = impl.ShowOnlyTitleDuplicateSearch(strSearchType, strTitle, strSearchCount, strSpeciesKey);
	if (ids < 0) return;
}

VOID CBOSpeciesDlg::OnbtnSEARCHDONATOR() 
{
	// TODO: Add your control notification handler code here
	//기증자 검색 화면 출력
}

VOID CBOSpeciesDlg::OnbtnGROUPINSERT() 
{
	// TODO: Add your control notification handler code here
	//Group추가 화면을 출력한다.
}

VOID CBOSpeciesDlg::OnKillfocusedtACQYEAR() 
{
	// TODO: Add your control notification handler code here
	//수입년도 KillFoucs 이벤트 프로시져
	//수입년도가 바뀔때 마다 Group리스트를 재구성해야한다.
}
