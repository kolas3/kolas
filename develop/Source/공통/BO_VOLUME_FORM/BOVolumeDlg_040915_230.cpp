// BOVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOVolumeDlg.h"
#include "BOISBNAdditionDlg.h"

//#include _T("BOAppendixDlg.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOVolumeDlg dialog
CBOVolumeDlg::CBOVolumeDlg(CESL_Mgr* pParent /*=NULL*/, CESL_DataMgr *pDM_Book /*= NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOVolumeDlg)
	//}}AFX_DATA_INIT

	m_pDM_Book = pDM_Book;
	m_nOpenMode = -1;

	m_pFormApi = NULL;
	m_pFormApi = new CBOFormApi(this);
	
	m_pBOSpeciesApi = NULL;

	m_pDM_Volume = NULL;
	m_pCM = NULL;

	m_nSelectedGrid = -1;
	m_nSelectedRow = -1;

	m_pDM_BookHold = NULL;
	m_pDM_VolumeHold = NULL;


	m_bShowHoldInfo = FALSE;
	m_bShowHoldInfoButton = FALSE;


	m_pGrid_Volume = NULL;
	m_pGrid_VolumeHold = NULL;
	m_pGrid_Book = NULL;
	m_pGrid_BookHold = NULL;

	m_bReadOnly = FALSE;

	m_nTabOrder = VOLUME_FIRST;
}

CBOVolumeDlg::~CBOVolumeDlg()
{
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
}

BOOL CBOVolumeDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOVolumeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOVolumeDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOVolumeDlg, CDialog)
	//{{AFX_MSG_MAP(CBOVolumeDlg)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_eREG_CLASS_CODE, OnKillfocuseREGCLASSCODE)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bINSERT, OnbINSERT)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VOLUME_BOOK, OnSelchangeTabVolumeBook)
	ON_BN_CLICKED(IDC_bVIEW_HOLD_INFO, OnbVIEWHOLDINFO)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_b505TAG, Onb505TAG)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_BN_CLICKED(IDC_bSELECT_REP_BOOK, OnbSELECTREPBOOK)
	ON_BN_CLICKED(IDC_bISBN_INSERT, OnbISBNINSERT)
	ON_EN_KILLFOCUS(IDC_eCOPY_CODE, OnKillfocuseCOPYCODE)
	ON_BN_CLICKED(IDC_btnAPPLY_SERIES, OnbtnAPPLYSERIES)
	ON_CBN_SELCHANGE(IDC_cMEDIA_CLASS, OnSelchangecMEDIACLASS)
	ON_BN_CLICKED(IDC_btnVOL_FIND_FILE1, OnbtnVOLFINDFILE1)
	ON_BN_CLICKED(IDC_btnVOL_FIND_FILE3, OnbtnVOLFINDFILE3)
	ON_BN_CLICKED(IDC_btnVOL_FIND_FILE2, OnbtnVOLFINDFILE2)
	ON_BN_CLICKED(IDC_btnVOL_EBOOK_VIEW, OnbtnVOLEBOOKVIEW)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_EN_KILLFOCUS(IDC_eBOOK_PRICE, OnKillfocuseBOOKPRICE)
	//}}AFX_MSG_MAP
	ON_MESSAGE(MSG_GRID_DISPLAY, OnGridDisplay)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBOVolumeDlg Operations
INT CBOVolumeDlg::SetParentInfo(CString strSpeciesKey, CString strPurchaseKey)
{
	if (strSpeciesKey.IsEmpty()) return -1;
	
	m_strSpeciesKey = strSpeciesKey;
	m_strPurchaseKey = strPurchaseKey;

	return 0;
}

INT CBOVolumeDlg::SetHoldInfoUI(BOOL bShowHoldInfo, BOOL bShowHoldInfoButton)
{
	m_bShowHoldInfo = bShowHoldInfo;
	m_bShowHoldInfoButton = bShowHoldInfoButton;

	return 0;
}

INT CBOVolumeDlg::Init()
{
	// 종 API
	if (m_pBOSpeciesApi) 
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	m_pBOSpeciesApi->m_nOpenMode = GetOpenMode();

	// DM 정보 
	if (m_pDM_Book == NULL)
	{
		m_pDM_Book = FindDM(_T("DMUP_단행_권_책정보"));
	}

	m_pDM_Volume = FindDM(_T("DMUP_단행_권_권정보"));
	
	m_pDM_BookHold = FindDM(_T("DMUP_단행_권_소장책정보"));
	m_pDM_VolumeHold = FindDM(_T("DMUP_단행_권_소장권정보"));

	if (m_pDM_Book == NULL ||
		m_pDM_Volume == NULL ||
		m_pDM_BookHold == NULL ||
		m_pDM_VolumeHold == NULL
		) 
		return -1;

	// CM 정보
	m_pCM = FindCM(_T("CMUP_단행_권_책정보"));
	if (m_pCM == NULL) return -1;

	// Grid 정보

	m_pGrid_Volume		= (CESL_Grid*)m_pCM->FindControl(_T("GRID_VOLUME"));
	m_pGrid_VolumeHold	= (CESL_Grid*)m_pCM->FindControl(_T("GRID_VOLUME_HOLD"));
	m_pGrid_Book		= (CESL_Grid*)m_pCM->FindControl(_T("GRID_BOOK"));
	m_pGrid_BookHold	= (CESL_Grid*)m_pCM->FindControl(_T("GRID_BOOK_HOLD"));

	if (m_pGrid_Volume == NULL ||
		m_pGrid_VolumeHold == NULL ||
		m_pGrid_Book == NULL||
		m_pGrid_BookHold == NULL
		)
	return -1;	

	// DB연결정보 설정
	this->SetCONNECTION_INFO(m_pDM_Book->CONNECTION_INFO);
	
	// 권 DM 구축
	
	// 소장종이 있으면 소장종 구축한다.
	if (!GetDupIdxKey().IsEmpty()) 
	{
		// 소장 책 만들기
		CString strOption;
		strOption.Format(_T("B.REC_KEY = E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B'"), GetDupIdxKey());
		m_pFormApi->MakeBookDM(strOption, m_pDM_BookHold);
			
		// 소장 권 만들기
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_BookHold, m_pDM_VolumeHold);
		m_bShowHoldInfoButton = TRUE;
		m_bShowHoldInfo = TRUE;
	}

	// Open Mode 에 따라서 ...
	if (GetOpenMode() == 1100)
	{
		m_pFormApi->SetAppMode(MODE_INSERT);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	else if (GetOpenMode() == -1100)
	{
		m_pFormApi->SetAppMode(MODE_UPDATE);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	
	// - 정리-종-입력 : 2100
	else if (GetOpenMode() == 2100)
	{
		m_bShowHoldInfoButton = FALSE;
		m_pFormApi->SetAppMode(MODE_INSERT);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);

		// 책 그리드의 DM 연결을 권으로 바꾼다.
		CString strLinkDM;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_단행_종_책정보"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();
	}
	// - 정리-종-수정 : -2100
	else if (GetOpenMode() == -2100)
	{
		m_bShowHoldInfoButton = FALSE;
		m_pFormApi->SetAppMode(MODE_UPDATE);

		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);
		// 책 그리드의 DM 연결을 권으로 바꾼다.
		CString strLinkDM;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_단행_종_책정보"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();
		//편권차에 맞게 권일련번호를 정리한다.
		//AdjustCurrentBookDM(m_pDM_Book);
	}
	// 열람-종-입력
	else if (GetOpenMode() == 3100)
	{
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	// 열람-종-수정
	else if (GetOpenMode() == -3100)
	{
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_SHOW);
	}
	// - 재활용 수정 : -7100
	else if (GetOpenMode() == -7100)
	{
		m_bReadOnly = TRUE;
	}
	// - 권정보 상세보기 : 8000
	else if (GetOpenMode() == 8000)
	{
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnAPPLY_SERIES)->ShowWindow(SW_HIDE);
		m_bShowHoldInfoButton = FALSE;
		if (m_strSpeciesKey.IsEmpty()) return -1;
		// DM 구성한다.
		CString strOption;
		INT ids;

		if (m_strPurchaseKey.IsEmpty() || m_strPurchaseKey == _T("ALL"))
			strOption.Format(_T("B.REC_KEY = E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC"), m_strSpeciesKey);
		else
			strOption.Format(_T("B.REC_KEY = E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.ACQ_KEY = %s AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC"), m_strSpeciesKey, m_strPurchaseKey);
		//ids = m_pDM_Book->RefreshDataManager(strOption);
		ids = m_pFormApi->MakeBookDM(strOption, m_pDM_Book);
		if (ids < 0) return -1;

		GetDlgItem(IDC_btnVOL_FIND_FILE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE3)->ShowWindow(SW_HIDE);
		
		// 책 그리드의 DM 연결을 권으로 바꾼다.
		CString strLinkDM = m_pDM_Book->DB_MGR_ALIAS;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_단행_종_책정보"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();

		m_bReadOnly = TRUE;
	}
	// - 복본추기 후 권정보 상세보기 : -8000
	else if (GetOpenMode() == -8000)
	{
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnAPPLY_SERIES)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnVOL_FIND_FILE3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bINSERT)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stcUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cUSE_LIMIT_CODE)->ShowWindow(SW_SHOW);

		m_bShowHoldInfoButton = FALSE;
		
		// 책 그리드의 DM 연결을 권으로 바꾼다.
		CString strLinkDM = m_pDM_Book->DB_MGR_ALIAS;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_단행_종_책정보"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();
	}
	//복본기호 부여방식 설정
	m_cCopyCodeMgr.Init(m_pDM_Book->CONNECTION_INFO);
	m_pFormApi->SetCopyCodeMgr(&m_cCopyCodeMgr);
	
	return 0;
}

INT CBOVolumeDlg::Display()
{
	if (m_nSelectedRow < 0) return -1;

	CESL_ControlMgr *pCM = FindCM(_T("CMUP_단행_권_책정보"));
	if (pCM == NULL) return -1;

	CESL_Control_Element * pCE = NULL;

	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Display(m_nSelectedRow);
	}

	m_pFormApi->DisplayISBN(pCM, _T("BB_낱권ISBN"));
	ShowRegCodeField();
//	ShowRegNo();
	OnKillfocuseREGCLASSCODE();

	if (m_bReadOnly)
		DisplayControlReadOnly();


	CString strMediaCode;
	m_pCM->GetControlMgrData(_T("BB_매체구분_코드"), strMediaCode);
	if(strMediaCode != m_strPrevMediaCode && (strMediaCode == _T("EB") || m_strPrevMediaCode == _T("EB")))
		ShowCodeResourceByMediaCode(strMediaCode);
	m_strPrevMediaCode = strMediaCode;
	
	ShowPrevNextButton();
	
    return 0;
}

INT CBOVolumeDlg::ClearScreen()
{
	CESL_ControlMgr *pCM = FindCM(_T("CMUP_단행_권_책정보"));
	if (pCM == NULL) return -1;

	CESL_Control_Element * pCE = NULL;

	POSITION pos = pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Clear();
	}

	ShowRegCodeField();
	OnKillfocuseREGCLASSCODE();

	return 0;
}

INT CBOVolumeDlg::ShowRegCodeField()
{
	// 등록구분 보여주기
	CString strRegCodeField;

	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM == NULL) return -1;

	// 권 상태
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
	{
		CString strVol = pDM->GetCellData(_T("BB_편권차"), m_nSelectedRow);
		CArray<CRegCode, CRegCode> arrRegCode;
		if (m_nSelectedGrid == GRID_VOLUME)
			m_pFormApi->GetRegCodeInfo(m_pDM_Book, &arrRegCode, strVol);
		else if (m_nSelectedGrid == GRID_VOLUME_HOLD)
			m_pFormApi->GetRegCodeInfo(m_pDM_BookHold, &arrRegCode, strVol);
		CRegCode regCode;
		CString strTemp;
		
		INT nArrCount = arrRegCode.GetSize();
		if (nArrCount == 0) 
		{
			if (strRegCodeField.IsEmpty()) strRegCodeField = _T("EM");
			GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
			return 0;
		}
		for (INT i = 0; i < nArrCount - 1; i++)
		{
			regCode = arrRegCode.GetAt(i);
			strTemp.Format(_T("%s%d/"), regCode.m_strRegCode, regCode.m_nCount);
			strRegCodeField += strTemp;
		}
		regCode = arrRegCode.GetAt(nArrCount - 1);
		strTemp.Format(_T("%s%d"), regCode.m_strRegCode, regCode.m_nCount);
		strRegCodeField += strTemp;
		
		if (strRegCodeField.IsEmpty()) strRegCodeField = _T("EM");
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
	}
	// 책상태
	else
	{
		strRegCodeField = pDM->GetCellData(_T("BB_등록구분_코드"), m_nSelectedRow);
		CString strBookAppFlag = pDM->GetCellData(_T("BB_책부록플래그"), m_nSelectedRow);
		if (strBookAppFlag != _T("A"))
			if (strRegCodeField.IsEmpty()) strRegCodeField = _T("EM");
		
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
	}

	return 0;
}

INT CBOVolumeDlg::ShowRegNo()
{
	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM == NULL) return -1;
	
	CString strRegNo = pDM->GetCellData(_T("BB_등록번호"), m_nSelectedRow);
	if (strRegNo.GetLength() < 3) 
		strRegNo.Empty();
	else
		strRegNo = strRegNo.Mid(2);

	return 0;
}

BOOL CBOVolumeDlg::IsDuplicateVolSortNo(CString strVolSortNo, INT nSelectedRow)
{
	CString strOriginVolSortNo;
	INT nRowCount = m_pDM_Volume->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if(idx == nSelectedRow) continue;
		m_pDM_Volume->GetCellData(_T("BB_권일련번호"), idx, strOriginVolSortNo);
		if(strVolSortNo == strOriginVolSortNo) return TRUE;
	}

	return FALSE;
}

BOOL CBOVolumeDlg::IsDuplicateVolume(CString strVol, INT nSelectedRow)
{
	//if (strVol.IsEmpty()) return FALSE;
	
	CString strOriginVol;
	INT nRowCount = m_pDM_Volume->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if(idx == nSelectedRow) continue;
		m_pDM_Volume->GetCellData(_T("BB_편권차"), idx, strOriginVol);
		if (strVol == strOriginVol) 
		{
			return TRUE;
		}
	}
	return FALSE;
}

INT CBOVolumeDlg::MakeStandardBookFromCM(CStringArray *pArrAbortList /*= NULL*/)
{
	CESL_ControlMgr *pCM = FindCM(_T("CMUP_단행_권_책정보"));
	if (pCM == NULL) return -1;

	// 마지막 로우가 비어잇는지 체크
	DWORD *row;
	INT nRowCount = m_pDM_Book->GetRowCount();
	INT nIdx = nRowCount -1;
	POSITION pos = m_pDM_Book->data.FindIndex(nRowCount - 1);
	if (pos)
	{
		row = (DWORD*)m_pDM_Book->data.GetAt(pos);
		if (row != NULL)
		{
			TCHAR *pCell = NULL;
			INT nColCount = m_pDM_Book->RefList.GetCount();
			for (INT idx = 0; idx < nColCount; idx++)
			{
				pCell = (TCHAR*)row[idx];
				if (pCell == NULL) continue;
				
				if (_tcscmp(pCell, _T("")) != 0)
				{
					nIdx = - 1;
					break;
				}
			}
		}
	}

	m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, nIdx, _T(""), pArrAbortList);

	nIdx = m_pDM_Book->GetRowCount() - 1;

	// DB Flag 적용
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, nIdx);

	//이용제한구분 적용, 책 자료상태 적용 
	CString strWorkingStatus;
	INT nRepresentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nRepresentBookIdx);
	if(nRepresentBookIdx < 0) nRepresentBookIdx = 0;
	m_pDM_Book->SetCellData(_T("BB_이용제한구분_코드"), m_pDM_Book->GetCellData(_T("BB_이용제한구분_코드"), nRepresentBookIdx), nIdx);
	//자료상태 정의 배가 이후의 자료에 대하여 소급 입력할 경우 배가대상자료가 아닌경우 무조건 배가자료 상태를 갖도록 한다.
	strWorkingStatus = m_pDM_Book->GetCellData(_T("BB_자료상태"), nRepresentBookIdx);
	if(strWorkingStatus.Left(3) == _T("BOL") && strWorkingStatus != _T("BOL111O")) strWorkingStatus = _T("BOL112N");
	m_pDM_Book->SetCellData(_T("BB_자료상태"), strWorkingStatus, nIdx);

	// 책상태일때 복본기호 추가
	if (m_nSelectedGrid == GRID_BOOK || m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		CString strVol = m_pDM_Book->GetCellData(_T("BB_편권차"), nIdx);
		INT nDupNumber = m_pFormApi->GetLastDupCodeNumber(m_pDM_Book, strVol);

		CString strDupCode;
		strDupCode.Format(_T("%d"), ++nDupNumber);
		m_pDM_Book->SetCellData(_T("BB_복본기호"), strDupCode, nIdx);
	}

	return 0;
}

INT CBOVolumeDlg::ApplyRegCode()
{
	// 등록구분에 맞게 책 수 조절
	// 권 상태
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
	{
		
		CString strRegCodeField;
		GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

		CArray<CRegCode, CRegCode> arrRegCode;
		INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
		if (ids < 0)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
			return -1;
		}

		m_strRegCodeField = strRegCodeField;
		INT nRegCodeCount = arrRegCode.GetSize();
		if (nRegCodeCount <= 0 ) return -1;

		CString strVol;
		GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_편권차"), strVol);

		m_pFormApi->MakeBookFromRegCode(&arrRegCode, m_pDM_Book, strVol);
		// 책에 Working Status를 넣는다.
		INT nRowCount = m_pDM_Book->GetRowCount();
		INT nRepresentBookIdx;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nRepresentBookIdx);
		if(nRepresentBookIdx < 0) nRepresentBookIdx = 0;
		
		CString strBookWorkingStatus = _T("");
		for (INT idx = 0; idx < nRowCount; idx++)
		{
			// 책이 아니면 넘어간다.
			if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
			// 책에 이미 작업상태가 설정된 상태라면 continue
			m_pDM_Book->GetCellData(_T("BB_자료상태"), idx, strBookWorkingStatus);
			if(strBookWorkingStatus.IsEmpty())
				m_pDM_Book->SetCellData(_T("BB_자료상태"), m_pDM_Book->GetCellData(_T("BB_자료상태"), nRepresentBookIdx), idx);
			if(strBookWorkingStatus.Left(3) == _T("BOR") && m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
				m_pDM_Book->SetCellData(_T("BB_자료상태"), _T("BOR111N"), idx);
		}		
	}
	// 책상태
	else
	{
		CString strRegCode, strVol, strSeShelfCode, strCopyCode;
		GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCode);

		// 맞는 등록구분인지 체크
		if (!m_pFormApi->IsValidRegCode(strRegCode))
		{
			AfxMessageBox(_T("등록구분값이 올바르지 않습니다."));
			return -1;
		}
		// 책부록 플래그 추가
		m_pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), m_nSelectedRow);
		//등록구분설정
		m_pDM_Book->SetCellData(_T("BB_등록구분_코드"), strRegCode, m_nSelectedRow);
/*		// 복본기호 설정
		INT ids = SetCopyCode(m_pDM_Book, m_nSelectedRow);
		if(ids < 0) return -1;
*/	}

	return 0;
}

INT CBOVolumeDlg::ApplySaveData()
{
 	CESL_ControlMgr *pCM = FindCM(_T("CMUP_단행_권_책정보"));
	if (pCM == NULL) return -1;

	INT ids;
	// 수서-종-구입-입력 || // 수서-종-구입-수정 || // - 정리-종-수정 : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 || GetOpenMode() == -2100 || GetOpenMode() == -8000)
	{
		// 권 상태에서 수정을 했을때
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			ids = SaveVolume(pCM);
			if(ids < 0) return ids;
		}
		// 책 상태에서 수정을 했을때
		else
		{
			ids = SaveBook(pCM);
			if(ids < 0) return ids;
		}
	}
	
	return 0;
}

INT CBOVolumeDlg::ApplyInsertData()
{
	INT ids;
	// 수서-종-구입-입력 || // 수서-종-구입-수정 || // - 정리-종-수정 : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 || GetOpenMode() == -2100)
	{
		// 거부리스트 만들기 (책정보 수정안하게..)
		m_pFormApi->InitDefaultIgnoreAlias();
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_대표책여부"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_복본기호"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_EBOOKKEY"));

		// 권 상태
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			ids = InsertVolume();
			if(ids < 0) return ids;
		}
		// 책상태
		else
		{
			ids = InsertBook();
			if(ids < 0) return ids;
		}
	}
	return 0;
}

INT CBOVolumeDlg::ApplyDeleteData()
{
	INT ids;
	// 수서-종-구입-입력 || // 수서-종-구입-수정 || // - 정리-종-수정 : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 ||  GetOpenMode() == -2100)
	{
		// 권 상태
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			ids = DeleteVolume();
			if(ids < 0) return ids;
		}
		// 책상태
		else
		{
			ids = DeleteBook();
			if(ids < 0) return ids;
		}
	}

	if (m_nSelectedRow < 0) m_nSelectedRow = 0;

	// 소장종이 아니면 대표책 정보 자동 세팅
	if (GetDupIdxKey().IsEmpty())
		m_pFormApi->SetRepresentationBook(m_pDM_Book);
	
	return 0;
}

CESL_DataMgr *CBOVolumeDlg::GetActiveDM()
{
	CESL_DataMgr *pDM = NULL;
	if (m_nSelectedGrid == GRID_VOLUME)
	{
		pDM = m_pDM_Volume;
	}
	else if (m_nSelectedGrid == GRID_VOLUME_HOLD)
	{
		pDM = m_pDM_VolumeHold;
	}
	else if (m_nSelectedGrid == GRID_BOOK)
	{
		pDM = m_pDM_Book;
	}
	else if (m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		pDM = m_pDM_BookHold;
	}

	return pDM;
}

INT CBOVolumeDlg::LinkCMWithActiveDM()
{
	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM == NULL) return -1;

	return LinkCMWithDM(m_pCM, pDM);

}

INT CBOVolumeDlg::LinkCMWithDM(CESL_ControlMgr *pCM, CESL_DataMgr *pDM)
{
	if (pCM == NULL || pDM == NULL) return -1;
	
	// DM의 Alias 구하기
	CString strDMAlias = pDM->DB_MGR_ALIAS;


	// CM의 콘트롤 돌면서 link 시킬꺼 찾아 DM의 Alias로 바꾼다.
	CESL_Control_Element *pCE = NULL;
	POSITION pos = pCM->Control_List.GetHeadPosition();

	INT ids = -1;
	CString strLinkData;
	CString strLinkDM;
	while (pos)
	{
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		
		// Grid인 경우 넘어간다.
		if (pCE->CTRL_TYPE == _T("G")) continue;

		 strLinkData = pCE->LINK_DATA;
		
		 ids = GetLinkDM(strLinkData, strLinkDM);
		 if (ids < 0) continue;

		 strLinkData.Replace(strLinkDM, strDMAlias);

		 pCE->LINK_DATA = strLinkData;
	}

	return 0;
}

INT CBOVolumeDlg::GetLinkDM(CString strLinkData, CString &strLinkDM)
{
	if (strLinkData.IsEmpty()) return -1;

	if (strLinkData.GetAt(0) == ':')
	{
		INT nFind = strLinkData.Find('.', 1);
		if (nFind < 0) return -1;

		strLinkDM = strLinkData.Mid(1, nFind - 1);
	}

	return 0;
}

CESL_Grid *CBOVolumeDlg::GetActiveGrid()
{
	CESL_Grid *pGrid = NULL;
	if (m_nSelectedGrid == GRID_VOLUME)
		pGrid = m_pGrid_Volume;
	else if (m_nSelectedGrid == GRID_VOLUME_HOLD)
		pGrid = m_pGrid_VolumeHold;
	else if (m_nSelectedGrid == GRID_BOOK)
		pGrid = m_pGrid_Book;
	else if (m_nSelectedGrid == GRID_BOOK_HOLD)
		pGrid = m_pGrid_BookHold;
	
	return pGrid;
}

INT CBOVolumeDlg::SetActiveGrid(INT nSelectedGrid)
{
	if (nSelectedGrid == -1) return -1;
	
	m_nSelectedGrid = nSelectedGrid;

	if (m_nSelectedRow < 0) return -1;
	
	GridAllClearReverse();

	DisplayButtons();
	
	// CM을 ActiveDM과 연결해준다.
	LinkCMWithActiveDM();
	
	return 0;
}

INT CBOVolumeDlg::GridAllClearReverse()
{
	CESL_Control_Element *pCE = NULL;
	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;

		if (pCE->CTRL_TYPE == _T("G")) 
		{
			GridClearReverse((CESL_Grid*)pCE->Control);
		}
	}

	return 0;
}

INT CBOVolumeDlg::GridClearReverse(CESL_Grid *pGrid)
{
	if (pGrid == NULL) return -1;

	COLORREF crWhite = RGB(255, 255, 255);
	COLORREF crBlack = RGB(0, 0, 0);

	INT nReverse = pGrid->m_nReverse;

	INT nCol = pGrid->GetCol();
	INT nRow = pGrid->GetRow();

	INT nCols = pGrid->GetCols(0);

	if (nReverse >= 0 && nReverse < pGrid->GetRows() - 1)
	{
		pGrid->SetRow(nReverse + 1);
		for (INT i = 1; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(crWhite);
			pGrid->SetCellForeColor(crBlack);
		}
	}

	pGrid->SetRow(nRow);
	pGrid->SetCol(nCol);
		
	return 0;
}

INT CBOVolumeDlg::DisplayButtons()
{
	INT nButtonCount = 0;
	INT nArrButtonCtrl[16];
	
	nArrButtonCtrl[nButtonCount++] = IDC_bSAVE;
	nArrButtonCtrl[nButtonCount++] = IDC_bDELETE;
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_b505TAG;
	nArrButtonCtrl[nButtonCount++] = IDC_btnAPPLY_SERIES;

	CWnd *pWnd = NULL;
	
	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;

		if (m_bReadOnly)// && nArrButtonCtrl[i] != IDC_bAPPENDIX_INFO)
		{			
			pWnd->EnableWindow(FALSE);
		}
		else if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_BOOK)
		{
			pWnd->EnableWindow(TRUE);
		}			
		else if (m_nSelectedGrid == GRID_VOLUME_HOLD || m_nSelectedGrid == GRID_BOOK_HOLD)
			pWnd->EnableWindow(FALSE);
		else
			return -1;
	}

	//OpenMode가 -8000이면 추가/삭제/전집처리/화면지우기 Disable
	if(GetOpenMode() == -8000)
	{
		GetDlgItem(IDC_bINSERT)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnAPPLY_SERIES)->EnableWindow(FALSE);
	}
	return 0;
}

INT CBOVolumeDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	CESL_DataMgr * pDM = GetActiveDM();
	if(!pDM) return 0;
	if(m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD) 
		return 0;

	if(strDMFieldAlias == _T("UDF_책자료상태"))
	{
		CString strWorkingStatus = pDM->GetCellData(_T("BB_자료상태"), nRow);
		if(strWorkingStatus == _T("BOT111O")) str = _T("기초자료");
		else if(strWorkingStatus == _T("BOT112O")) str = _T("반입자료");
		else if(strWorkingStatus == _T("BOA111N")) str = _T("구입대상자료");
		else if(strWorkingStatus == _T("BOA112N")) str = _T("주문자료");
		else if(strWorkingStatus == _T("BOA113O")) str = _T("미납품자료");
		else if(strWorkingStatus == _T("BOA114O")) str = _T("구입인계대상자료");
		else if(strWorkingStatus == _T("BOA211O")) str = _T("기증접수자료");
		else if(strWorkingStatus == _T("BOA212O")) str = _T("기증미등록자료");
		else if(strWorkingStatus == _T("BOR111N")) str = _T("등록인계자료");
		else if(strWorkingStatus == _T("BOR112N")) str = _T("등록자료");
		else if(strWorkingStatus == _T("BOR113O")) str = _T("가원부번호부여자료");
		else if(strWorkingStatus == _T("BOC111O")) str = _T("복본조사대상자료");
		else if(strWorkingStatus == _T("BOC112O")) str = _T("분류대상자료");
		else if(strWorkingStatus == _T("BOC113N")) str = _T("정리대상자료");
		else if(strWorkingStatus == _T("BOC114O")) str = _T("열람인계대상자료");
		else if(strWorkingStatus == _T("BOL111O")) str = _T("열람인계자료");
		else if(strWorkingStatus == _T("BOL112N")) str = _T("배가자료");
		else if(strWorkingStatus == _T("BOL113O")) str = _T("수리제본자료");
		else if(strWorkingStatus == _T("BOL114O")) str = _T("파손자료");
		else if(strWorkingStatus == _T("BOL115O")) str = _T("제적자료");
		else if(strWorkingStatus == _T("BOL211O")) str = _T("관외대출자료");
		else if(strWorkingStatus == _T("BOL212O")) str = _T("관내대출자료");
		//=====================================================
		// 18.09.27 JMJ KOL.RED.2018.008
		else if(strWorkingStatus == _T("BOL411O")) str = _T("단행책두레업무진행자료");
		//=====================================================
		else if(strWorkingStatus == _T("BOL213O")) str = _T("특별대출자료");
		else if(strWorkingStatus == _T("BOL214O")) str = _T("긴급대출자료");
		else if(strWorkingStatus == _T("BOL215O")) str = _T("배달자료");
		pDM->SetCellData(_T("UDF_책자료상태"), str, nRow);
	}
	return 0;
}

INT CBOVolumeDlg::DisplayActiveGrid(INT nRow, BOOL bSetTopRow /*=FALSE*/)
{
	CESL_Grid *pGrid  = GetActiveGrid();
	if (pGrid == NULL) return - 1;
	CESL_DataMgr *pDM = GetActiveDM();
	INT nRows = pDM->GetRowCount();
	INT idx;
/*
	// 책부록플래그가 B 인경우 블랭크로 A인 경우 _T("V")로 표시
	CString strBookAppendixFlag;
	
	if (pDM == NULL) return -1;

	// 책부록플래그가 B 인경우 블랭크로 A인 경우 _T("V")로 표시
	INT nRows = pDM->GetRowCount();
	for (INT idx = 0; idx < nRows; idx++)
	{
		strBookAppendixFlag = pDM->GetCellData(_T("BB_책부록플래그"), idx);
		if (strBookAppendixFlag == _T("B"))
			strBookAppendixFlag.Empty();
		else if (strBookAppendixFlag == _T("A"))
			strBookAppendixFlag = _T("V");

		pDM->SetCellData(_T("BB_부록여부"), strBookAppendixFlag, idx);

	}
*/
	// 책 소팅
	//SortBookDM(pDM);
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	pGrid->Display();
	
	// QUERY FLAG가 D인 자료는 Height를 0으로 하자
	for (idx = 0; idx < nRows; idx++)
	{
		if (pDM->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D"))
		{
			pGrid->SetRowHeight(idx + 1, 0);			
		}
	}

	pGrid->GetRows();

	if (m_nSelectedGrid == GRID_BOOK || m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		// 대표책 인덱스 가져오기
		// KOL.UDF.022 시큐어코딩 보완
		INT nIdx =-1;

		if (m_nSelectedGrid == GRID_BOOK)
			m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		else if (m_nSelectedGrid == GRID_BOOK_HOLD)
			m_pFormApi->GetRepresentationBook(m_pDM_BookHold, nIdx);

		if  (nIdx >= 0)
			SetGridRowBackColor(pGrid, nIdx);
		//그리드 출력 동기화 문제
		
		if (nRow != nIdx)
			pGrid->SetReverse(nRow);
	}
	else
		pGrid->SetReverse(nRow);
	//그리드 출력 동기화 문제
	if (bSetTopRow)
			pGrid->SetTopRow(nRow + 1);
	return 0;
}

INT CBOVolumeDlg::DisplayGrids()
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return -1;

	INT nCurSel = pTab->GetCurSel();
	if (nCurSel == -1) return -1;


	CRect rcStaticGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcStaticGrid);
	ScreenToClient(rcStaticGrid);

	m_pGrid_Volume->ShowWindow(SW_HIDE);
	m_pGrid_VolumeHold->ShowWindow(SW_HIDE);
	m_pGrid_Book->ShowWindow(SW_HIDE);
	m_pGrid_BookHold->ShowWindow(SW_HIDE);

	if (nCurSel == 0)
	{
		if (m_bShowHoldInfo)
		{
			m_pGrid_VolumeHold->ShowWindow(SW_SHOW);
			m_pGrid_VolumeHold->MoveWindow(rcStaticGrid.left, rcStaticGrid.top, rcStaticGrid.Width(),rcStaticGrid.Height() / 2);

			m_pGrid_Volume->ShowWindow(SW_SHOW);
			m_pGrid_Volume->MoveWindow(rcStaticGrid.left, rcStaticGrid.top + (rcStaticGrid.Height() / 2), rcStaticGrid.Width(),rcStaticGrid.Height() / 2);
		}
		else
		{
			m_pGrid_Volume->ShowWindow(SW_SHOW);
			m_pGrid_Volume->MoveWindow(rcStaticGrid.left, rcStaticGrid.top, rcStaticGrid.Width(), rcStaticGrid.Height());
		}

		SetActiveGrid(GRID_VOLUME);
	}
	else if (nCurSel == 1)
	{
		if (m_bShowHoldInfo)
		{
			m_pGrid_BookHold->ShowWindow(SW_SHOW);
			m_pGrid_BookHold->MoveWindow(rcStaticGrid.left, rcStaticGrid.top, rcStaticGrid.Width(),rcStaticGrid.Height() / 2);

			m_pGrid_Book->ShowWindow(SW_SHOW);
			m_pGrid_Book->MoveWindow(rcStaticGrid.left, rcStaticGrid.top + (rcStaticGrid.Height() / 2), rcStaticGrid.Width(),rcStaticGrid.Height() / 2);
		}
		else
		{
			m_pGrid_Book->ShowWindow(SW_SHOW);
			m_pGrid_Book->MoveWindow(rcStaticGrid.left, rcStaticGrid.top, rcStaticGrid.Width(), rcStaticGrid.Height());
		
		}

		SetActiveGrid(GRID_BOOK);
	}

	return 0;
}

INT CBOVolumeDlg::DisplayContorls()
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return -1;

	INT nCurSel = pTab->GetCurSel();
	if (nCurSel == -1) return -1;

	CWnd *pWnd = NULL;

	// SHOW, HIDE 소장정보 버튼
	if (m_bShowHoldInfoButton)
		GetDlgItem(IDC_bVIEW_HOLD_INFO)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_bVIEW_HOLD_INFO)->ShowWindow(SW_HIDE);

	
	// SHOW, HIDE Grid
	DisplayGrids();

	// Ctrl Endable / Disable
	INT nBookOnlyCtrlCount = 0;
	INT nVolumeOnlyCtrlCount = 0;

	INT nArrBookOnlyCtrl[64];
	INT nArrVolumeOnlyCtrl[64];

	// 책에서만 쓰이는 Ctrl
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_eCOPY_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cSHELF_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cMANAGE_CLASS;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_bSELECT_REP_BOOK;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_btnVOL_FIND_FILE1;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_btnVOL_FIND_FILE2;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_btnVOL_FIND_FILE3;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_edtVOL_EBOOK_EXECUTE_FILE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cUSE_LIMIT_CODE;

	// 권에서만 쓰이는 Ctrl
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_cMEDIA_CLASS;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eVOL;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eVOLTITLE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAUTHOR;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePUBLISH_YEAR;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_edtVOL_SORT_NO;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eEA_ISBN;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eISBN2;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eISBN3;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePAGE_CNT;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePHYSICAL_PROPERTIES;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eSIZE;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAPPENDIX;
//	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eBOOK_PRICE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePRICE_OTHRE_INFO;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_bISBN_INSERT;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_bISBN_CHECK;
		
	TCHAR tszClassName[16];
	tszClassName[0] = 0;

	for (INT i = 0; i < nBookOnlyCtrlCount; i++)
	{
		pWnd = GetDlgItem(nArrBookOnlyCtrl[i]);
		if (pWnd == NULL) continue;
		GetClassName(pWnd->GetSafeHwnd(), tszClassName, 100);

		if (nCurSel == 0)
		{
			if (_tcscmp(tszClassName, _T("Edit")) == 0)
				((CEdit*)pWnd)->SetReadOnly(TRUE);
			else
				pWnd->EnableWindow(FALSE);
		}
		else if (nCurSel == 1)
		{
			if (_tcscmp(tszClassName, _T("Edit")) == 0)
				((CEdit*)pWnd)->SetReadOnly(FALSE);
			else
				pWnd->EnableWindow(TRUE);
		}
	}

	for (i = 0; i < nVolumeOnlyCtrlCount; i++)
	{
		pWnd = GetDlgItem(nArrVolumeOnlyCtrl[i]);
		if (pWnd == NULL) continue;

		GetClassName(pWnd->GetSafeHwnd(), tszClassName, 100);
		if (nCurSel == 0)
		{
			if (_tcscmp(tszClassName, _T("Edit")) == 0)
				((CEdit*)pWnd)->SetReadOnly(FALSE);
			else
				pWnd->EnableWindow(TRUE);
		}
		else if (nCurSel == 1)
		{
			if (_tcscmp(tszClassName, _T("Edit")) == 0)
				((CEdit*)pWnd)->SetReadOnly(TRUE);
			else
				pWnd->EnableWindow(FALSE);
		}
	}

	// 화면지우기버튼
	pWnd = GetDlgItem(IDC_bCLEARSCREEN);
	if(nCurSel == 0)
	{
		if(pWnd) pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		if(pWnd) pWnd->ShowWindow(SW_HIDE);
	}
	// 전집처리버튼
	pWnd = GetDlgItem(IDC_btnAPPLY_SERIES);
	if(nCurSel == 0)
	{
		if(pWnd) pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		if(pWnd) pWnd->ShowWindow(SW_SHOW);
	}
		
	
	// 저장버튼
	if (m_nSelectedGrid == GRID_VOLUME_HOLD || m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		GetDlgItem(IDC_bSAVE)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_bSAVE)->ShowWindow(SW_SHOW);
	}

	return 0;
}

INT CBOVolumeDlg::DisplayControlReadOnly()
{
	//모든 컨트롤을 readonly시킨다. 단 그리드는 제외돼어야한다.
	m_pCM->AllControlReadOnly();
	//권/책 그리드는 readonly를 풀어준다
	m_pGrid_Book->EnableWindow(TRUE);
	m_pGrid_Volume->EnableWindow(TRUE);

	INT nButtonCount = 0;
	INT nArrButtonCtrl[16];
	
	nArrButtonCtrl[nButtonCount++] = IDC_bSAVE;
	nArrButtonCtrl[nButtonCount++] = IDC_bDELETE;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_b505TAG;
	nArrButtonCtrl[nButtonCount++] = IDC_bISBN_INSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_bISBN_CHECK;
	nArrButtonCtrl[nButtonCount++] = IDC_btnAPPLY_SERIES;	
//	nArrButtonCtrl[nButtonCount++] = IDC_eAPPLICANT;

	CWnd *pWnd = NULL;

	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;
		pWnd->EnableWindow(FALSE);
	}

	return 0;
}

INT CBOVolumeDlg::SetGridRowBackColor(CESL_Grid *pGrid, INT nRow)
{
	if (pGrid == NULL) return -1;

	COLORREF red, white, black;
	
	red = RGB(251,196,234);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRow >= 0)
	{
		pGrid->SetRow(nRow + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellBackColor(red);
			pGrid->SetCellForeColor(black);
		}
	}
	
	pGrid->SetCol(nCol);

	return 0;
}

BOOL CBOVolumeDlg::IsRegCodeModified()
{
	INT nRegCodeCount = -1;
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		MessageBox(_T("없는 등록구분 입니다."));
		return TRUE;
	}
	
	nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount < 1) 
	{
		return TRUE;
	}
	
	CString strVol;
	m_pCM->GetControlMgrData(_T("BB_편권차"), strVol);

	CESL_DataMgr *pDM = NULL;

	if (m_nSelectedGrid == GRID_VOLUME)
		pDM = m_pDM_Book;
	else if (m_nSelectedGrid == GRID_VOLUME_HOLD)
		pDM = m_pDM_BookHold;
	
	if (m_pFormApi->IsRegCodeModify(&arrRegCode, pDM, strVol))
	{
		AfxMessageBox(_T("등록구분이 수정이 될수 있습니다. 책정보에서 수정해주세요"));
		Display();
		return TRUE;
	}

	return FALSE;
}

INT CBOVolumeDlg::SortBookDM(CESL_DataMgr *pDM_Book)
{
	if (pDM_Book == NULL) return -1;

	// 권일련번호로 정렬한다.
	//pDM_Book->SORT(_T("BB_권일련번호"));
	Sort_BookDM(pDM_Book);

	CString strVol;

	// 정렬된 편권차 내부에서 부록관련해서 정렬한다.
	INT nRowCount = pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(pDM_Book, idx)) continue;

		// 편권차를 가져온다.
		strVol = pDM_Book->GetCellData(_T("BB_편권차"), idx);

		INT nStart = idx;

		// 편권차가 아닐때가지 ㅋㅋ
		while (idx < nRowCount)
		{
			if (!m_pFormApi->IsBook(pDM_Book, idx, strVol)) break;

			idx++;
		}

		INT nFinish = idx - 1;

		
		pDM_Book->SORT(_T("BB_책부록플래그"), nStart, nFinish, FALSE);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBOVolumeDlg message handlers

BOOL CBOVolumeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Initialize Esl Manger	
	INT ids = InitESL_Mgr(_T("K2UPGRADE_단행_권"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	GetWindowRect(m_rcWindow);

	// Initialize Data Mangager
	ids = Init();
	if (ids < 0)
	{
		AfxMessageBox(_T("Init Data Manger Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_nSelectedRow = 0;
	//AllControlDisplay(_T("CMUP_단행_권_책정보"));
	
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab != NULL)
	{
		pTab->InsertItem(0, _T("권 정보"));
		pTab->InsertItem(1, _T("책 정보"));
		if(m_nTabOrder == VOLUME_FIRST)
		{	
			pTab->SetCurFocus(1);
			pTab->SetCurFocus(0);
		}
		else if(m_nTabOrder == BOOK_FIRST)
		{
			pTab->SetCurFocus(0);
			pTab->SetCurFocus(1);
		}
	}

	// Status Bar For Grab HAHA;;
	//m_statusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);

	if (m_bShowHoldInfo)
	{
		m_bShowHoldInfo = FALSE;
		OnbVIEWHOLDINFO();
	}

	m_pCM->GetControlMgrData(_T("BB_매체구분_코드"), m_strPrevMediaCode);
	ShowCodeResourceByMediaCode(m_strPrevMediaCode);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOVolumeDlg::OnDestroy() 
{

	// 소장권이 있을 경우 현재 추가되어진 책정보의 VOL_SORT_NO를 정리하여야 한다.
	if(m_pDM_VolumeHold != NULL && m_pDM_BookHold != NULL)
	{
		if(m_pDM_VolumeHold->GetRowCount() > 0 && m_pDM_BookHold->GetRowCount() > 0)
		{
			INT ids = AdjustCurrentBookDMByHoldBook(m_pDM_VolumeHold, m_pDM_BookHold, m_pDM_Book);
			if(ids < 0)
			{
				ESLAfxMessageBox(_T("현재 추가되어진 책정보의 권일련번호를 정리할 수 없습니다."));
			}
		}
	}
	CDialog::OnDestroy();
}

VOID CBOVolumeDlg::OnKillfocuseREGCLASSCODE() 
{
	CEdit* pEditClassCode = (CEdit*)GetDlgItem(IDC_eREG_CLASS_CODE);
	if (pEditClassCode == NULL) return;

	// 리드 온리이면 체크하지 않는다.
	if (pEditClassCode->GetStyle() & ES_READONLY) return;

	INT nRegCodeCount = -1;
	CString strRegCodeField;
	pEditClassCode->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		MessageBox(_T("없는 등록구분 입니다."));
		goto ERR;
	}
	
	nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount < 1) 
	{
		goto ERR;
	}

	//  권상태
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
	{
		INT nBookCnt = 0;
		for (INT i = 0; i < nRegCodeCount; i++)
		{
			nBookCnt += arrRegCode.GetAt(i).m_nCount;
		}

		CString strBookCnt;
		strBookCnt.Format(_T("%d"), nBookCnt);

		GetDlgItem(IDC_eBOOK_CNT)->SetWindowText(strBookCnt);
	}
	// 책상태
	else
	{
		CRegCode regCode;
		regCode = arrRegCode.GetAt(0);

		if (regCode.m_nCount > 1) goto ERR;
		
		m_strRegCodeField = regCode.m_strRegCode;
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
		CString strBookCnt;
		strBookCnt.Format(_T("%d"), 1);
		GetDlgItem(IDC_eBOOK_CNT)->SetWindowText(strBookCnt);
	}

	m_strRegCodeField = strRegCodeField;

ERR:
	pEditClassCode->SetWindowText(m_strRegCodeField);
	return;
}

BEGIN_EVENTSINK_MAP(CBOVolumeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOVolumeDlg)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME, -605 /* MouseDown */, OnMouseDownGridVolume, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK, -605 /* MouseDown */, OnMouseDownGridBook, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME_HOLD, -605 /* MouseDown */, OnMouseDownGridVolumeHold, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK_HOLD, -605 /* MouseDown */, OnMouseDownGridBookHold, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME, 69 /* SelChange */, OnSelChangeGridVolume, VTS_NONE)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK, 69 /* SelChange */, OnSelChangeGridBook, VTS_NONE)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME_HOLD, 69 /* SelChange */, OnSelChangeGridVolumeHold, VTS_NONE)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK_HOLD, 69 /* SelChange */, OnSelChangeGridBookHold, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBOVolumeDlg::OnbINSERT() 
{
	if (ApplyInsertData() < 0)
		return;

	DisplayContorls();
	// 마지막  정보 화면에 뿌려주기
	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM != NULL)
	{
		//m_nSelectedRow = pDM->GetRowCount() - 1;

		DisplayActiveGrid(m_nSelectedRow, TRUE);
	}
	
	Display();
}	


VOID CBOVolumeDlg::OnbSAVE() 
{
	if (m_nSelectedRow < 0) return;
	
	if (ApplySaveData() < 0)
		return;
	// 선택된 권 정보 화면에 뿌려주기
	DisplayActiveGrid(m_nSelectedRow, TRUE);
	
	PostMessage(WM_COMMAND, IDC_btnNEXT, BN_CLICKED);
}

VOID CBOVolumeDlg::OnbDELETE() 
{
	if (m_nSelectedRow < 0) return;
	
	if (ApplyDeleteData() < 0)
		return;
	
	DisplayContorls();

	// 이전 권 정보 화면에 뿌려주기
	DisplayActiveGrid(m_nSelectedRow, TRUE);
	
	Display();
}

VOID CBOVolumeDlg::OnbAPPENDIXINFO() 
{
/*	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return;

	CBOAppendixDlg dlg(this, m_pDM_Book);
	dlg.SetFormApi(m_pFormApi);
	dlg.SetReadOnly(m_bReadOnly);

	CESL_DataMgr *pDM = GetActiveDM();

	CString strVol;
	if (pDM)
		 strVol = pDM->GetCellData(_T("BB_편권차"), m_nSelectedRow);

	INT nCurSel = pTab->GetCurSel();
	if (nCurSel == 0)
	{
		dlg.SetAppendixMode(0, strVol);	
	}
	else if (nCurSel == 1)
	{
		dlg.SetAppendixMode(1, strVol, m_nSelectedRow);
	}
	
	dlg.DoModal();

	Display();

	DisplayActiveGrid(m_nSelectedRow, TRUE);
*/
}

VOID CBOVolumeDlg::OnSelchangeTabVolumeBook(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return;

	INT nCurSel = pTab->GetCurSel();
	if (nCurSel == 0)
	{
		// 권 정보 다시 구성
		m_pDM_Volume->FreeData();
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
		m_pDM_Volume->SORT(_T("BB_권일련번호"));
		m_nSelectedRow = 0;
	}
	else if (nCurSel == 1)
	{
		//m_pDM_Book->SORT(_T("BB_권일련번호"));
		Sort_BookDM(m_pDM_Book);
		m_nSelectedRow = GetFirstRowBookDM(m_pDM_Book);
		if(m_nSelectedRow < 0)
			return ;
	}

	DisplayContorls();

	// 소장자료를 보여준다.
	INT nSelectedGrid = m_nSelectedGrid;

	if (nSelectedGrid == GRID_VOLUME)
	{
		SetActiveGrid(GRID_VOLUME_HOLD);
		DisplayActiveGrid(0);
	}
	else if (nSelectedGrid == GRID_BOOK)
	{
		SetActiveGrid(GRID_BOOK_HOLD);
		DisplayActiveGrid(0);
	}
	

	// 기본 복원
	SetActiveGrid(nSelectedGrid);

	Display();

	DisplayActiveGrid(m_nSelectedRow);
	
	*pResult = 0;
}

VOID CBOVolumeDlg::OnbVIEWHOLDINFO() 
{
	if (m_bShowHoldInfo)
	{
		m_bShowHoldInfo = FALSE;
		GetDlgItem(IDC_bVIEW_HOLD_INFO)->SetWindowText(_T("소장자료보기"));
	}
	else
	{
		m_bShowHoldInfo = TRUE;
		GetDlgItem(IDC_bVIEW_HOLD_INFO)->SetWindowText(_T("소장자료숨기기"));
	}

	m_nSelectedRow = 0;
	DisplayContorls();
	Display();

	DisplayActiveGrid(m_nSelectedRow);

}

VOID CBOVolumeDlg::OnMouseDownGridVolume(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_VOLUME);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;
	
	if (SetActiveGrid(GRID_VOLUME) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();
}

VOID CBOVolumeDlg::OnMouseDownGridBook(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_BOOK);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;

	if (SetActiveGrid(GRID_BOOK) < 0) return;
	
	// 대표책 인덱스 가져오기
	INT nIdx;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
	if (m_nSelectedRow == nIdx)
		SetGridRowBackColor(pGrid, m_nSelectedRow);
	else
		pGrid->SetReverse(m_nSelectedRow);
	
	Display();
}

VOID CBOVolumeDlg::OnMouseDownGridVolumeHold(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_VOLUME_HOLD);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;

	if (SetActiveGrid(GRID_VOLUME_HOLD) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();
}

VOID CBOVolumeDlg::OnMouseDownGridBookHold(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_BOOK_HOLD);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;
	
	if (SetActiveGrid(GRID_BOOK_HOLD) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();
}
/*
VOID CBOVolumeDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;
	
	// StatusBar Ctrl
	/*if (m_statusBarCtrl.GetSafeHwnd() != NULL)
		m_statusBarCtrl.SendMessage(WM_SIZE, 0, 0);
		*/
/*
	CRect rcWnd;
	GetWindowRect(rcWnd);

	if (rcWnd.Width() < m_rcWindow.Width())
	{
		rcWnd.right = rcWnd.left + m_rcWindow.Width();
		if (rcWnd.Height() < m_rcWindow.Height())
			rcWnd.bottom = rcWnd.top + m_rcWindow.Height();	

		MoveWindow(rcWnd);
		return;
	}

	//if (rcWnd.Height() < m_rcWindow.Height())
	if (m_rcWindow.top >= 0 && rcWnd.Height() != m_rcWindow.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcWindow.Height();	
		if (rcWnd.Width() < m_rcWindow.Width())
			rcWnd.right = rcWnd.left + m_rcWindow.Width();			

		MoveWindow(rcWnd);
		return;
	}

	// Grid만 리사이징 --> IDC_STATIC_GRID

	CRect rcClient;
	// Tab
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		pWnd->SetWindowPos(this, rcClient.left, rcClient.top, cx - 22, cy - 271, SWP_NOZORDER);
	}
	// Static Grid
	pWnd = GetDlgItem(IDC_STATIC_GRID);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		pWnd->SetWindowPos(this, rcClient.left, rcClient.top, cx - 32, cy - 301, SWP_NOZORDER);
	}

	DisplayGrids();
}
*/
VOID CBOVolumeDlg::OnbCLOSE() 
{
	// 소장종이 아니면 대표책 정보 자동 세팅
	if (GetDupIdxKey().IsEmpty())
		m_pFormApi->SetRepresentationBook(m_pDM_Book);

	CDialog::OnOK();
}

VOID CBOVolumeDlg::OnbCLEARSCREEN() 
{
	ClearScreen();
}

VOID CBOVolumeDlg::Onb505TAG() 
{
	// 저장한다.
	OnbSAVE();

	// TODO : 정제헌씨 꺼 붙이기
	pParentMgr->SendMessage(WM_USER + 32015);
}

VOID CBOVolumeDlg::OnbISBNCHECK() 
{
	// 낱권 체크
	CString strEaISBNFrom;
	CString strEaISBNTo;
	GetDlgItem(IDC_eEA_ISBN)->GetWindowText(strEaISBNFrom);
	if (strEaISBNFrom.IsEmpty()) return;

	INT ids = m_pFormApi->CheckISBN(strEaISBNFrom, strEaISBNTo);
	if (ids < 0) return;
	
	GetDlgItem(IDC_eEA_ISBN)->SetWindowText(strEaISBNTo);	
}

LRESULT CBOVolumeDlg::OnGridDisplay(WPARAM wParam, LPARAM lParam)
{
	CESL_Grid *pGrid  = GetActiveGrid();
	if (pGrid == NULL) return - 1;

	CString strBookAppendixFlag;
	CESL_DataMgr *pDM = GetActiveDM();
	if (pDM == NULL) return -1;

	// QUERY FLAG가 D인 자료는 Height를 0으로 하자
	INT nRows = pDM->GetRowCount();
	for (INT idx = 0; idx < nRows; idx++)
	{
		if (pDM->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D"))
		{
			pGrid->SetRowHeight(idx + 1, 0);			
		}
	}

	// 대표책 보여준다.
	INT nIdx;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
	
	if  (nIdx >= 0 && m_nSelectedGrid == GRID_BOOK)
		SetGridRowBackColor(pGrid, nIdx);

	return 0;
}

VOID CBOVolumeDlg::OnbSELECTREPBOOK() 
{
	// TODO: Add your control notification handler code here
	if(m_nSelectedRow < 0) 
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	if(IDYES == ESLAfxMessageBox(_T("선정된 자료를 대표책으로 설정하시겠습니까?"), MB_YESNO))
	{
		INT nIdx;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		if(m_nSelectedRow == nIdx)
		{
			ESLAfxMessageBox(_T("이미 대표책으로 설정된 자료입니다."));
			return;
		}
		INT ids = m_pFormApi->SetRepresentationBook(m_pDM_Book, m_nSelectedRow);
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("선정된 자료를 대표책으로 설정할 수 없습니다."));
			return;
		}
		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), nIdx);
		if(m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), m_nSelectedRow).GetLength() == 0)
			m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("U"), m_nSelectedRow);
		DisplayActiveGrid(nIdx);
		DisplayActiveGrid(m_nSelectedRow);
		
	}
}

VOID CBOVolumeDlg::OnbISBNINSERT() 
{
	// TODO: Add your control notification handler code here
	CString strAdditionISBN = m_pDM_Book->GetCellData(_T("BB_추가낱권ISBN"), m_nSelectedRow);
	CString strAdditionAddCode = m_pDM_Book->GetCellData(_T("BB_추가낱권ISBN부가기호"), m_nSelectedRow);
	CString strAdditionMissISBN = m_pDM_Book->GetCellData(_T("BB_추가낱권오류ISBN"), m_nSelectedRow);

	CBOISBNAdditionDlg dlg(this);
	dlg.SetAdditionISBN(strAdditionISBN);
	dlg.SetAdditionAddCode(strAdditionAddCode);
	dlg.SetAdditionMissISBN(strAdditionMissISBN);

	if (dlg.DoModal() == IDOK)
	{
		INT rowCnt = m_pDM_Book->GetRowCount();
		CString strVolSortNo = m_pDM_Book->GetCellData(_T("BB_권일련번호"), m_nSelectedRow);
		for(INT i=0 ; i<rowCnt ; i++)
		{
			if(strVolSortNo == m_pDM_Book->GetCellData(_T("BB_권일련번호"), i))
			{
				m_pDM_Book->SetCellData(_T("BB_추가낱권ISBN"), dlg.GetAdditionISBN(), i);
				m_pDM_Book->SetCellData(_T("BB_추가낱권ISBN부가기호"), dlg.GetAdditionAddCode(), i);
				m_pDM_Book->SetCellData(_T("BB_추가낱권오류ISBN"), dlg.GetAdditionMissISBN(), i);
			}
		}
	}	
}

VOID CBOVolumeDlg::OnKillfocuseCOPYCODE() 
{
	// TODO: Add your control notification handler code here
//	if(GetOpenMode() < 0)
//	{
	CString strCopyCode;
	m_pCM->GetControlMgrData(_T("BB_복본기호"), strCopyCode); 

	CString strVolSortNo = m_pDM_Book->GetCellData(_T("BB_권일련번호"), m_nSelectedRow);
	INT ids = SetCopyCode(m_pDM_Book, m_nSelectedRow, strCopyCode);
	if(ids < 0)
		m_pCM->SetControlMgrData(_T("BB_복본기호"), m_pDM_Book->GetCellData(_T("BB_복본기호"), m_nSelectedRow));
/*
	if(CheckDuplicateCopyCode(strVolSortNo, strCopyCode, m_nSelectedRow))
	{
		ESLAfxMessageBox(_T("동일한 복본기호가 있습니다."));
		m_pCM->SetControlMgrData(_T("BB_복본기호"), m_pDM_Book->GetCellData(_T("BB_복본기호"), m_nSelectedRow));
	}
	}
*/
}

BOOL CBOVolumeDlg::CheckDuplicateCopyCode(CESL_DataMgr * pDM_BOOK, CString strVolSortNo, CString strCopyCode, INT nSelectedRow)
{
	if(pDM_BOOK == NULL) return TRUE;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	CString strTmpData;
	if(strCopyCode.IsEmpty()) strCopyCode = _T("1");
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strTmpData = pDM_BOOK->GetCellData(_T("BB_권일련번호"), i);
		if(i == nSelectedRow) continue;
		if(strTmpData == strVolSortNo)
		{
			strTmpData = pDM_BOOK->GetCellData(_T("BB_복본기호"), i);
			if(strTmpData.IsEmpty()) strTmpData = _T("1");
			if(strTmpData == strCopyCode)
				return TRUE;
		}
	}
	return FALSE;
}

VOID CBOVolumeDlg::OnbtnAPPLYSERIES() 
{
	// TODO: Add your control notification handler code here
	if (MessageBox(_T("책을 전집으로 만듭니다. 계속하시겠습니까?"), NULL, MB_YESNO | MB_ICONQUESTION) == IDNO) return;

	// 모든 책을 1권 1책으로 만든다.
	CString strVol;
	m_pFormApi->ApplyCompleteWorks(m_pDM_Book, strVol);

	MessageBox(_T("책을 전집으로 만들었습니다."));

	Display();
	DisplayActiveGrid(m_nSelectedRow);
}

BOOL CBOVolumeDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYUP && 
		pMsg->hwnd != GetActiveGrid()->GetSafeHwnd() && 
		pMsg->wParam == VK_UP)
	{
		PostMessage(WM_COMMAND, IDC_btnPREV, BN_CLICKED);
		return TRUE;
	}
	if ( pMsg->message == WM_KEYUP && 
		pMsg->hwnd != GetActiveGrid()->GetSafeHwnd() && 
		pMsg->wParam == VK_DOWN)
	{
		PostMessage(WM_COMMAND, IDC_btnNEXT, BN_CLICKED);
		return TRUE;
	}
		
	if ( pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) 
			return TRUE;
		return CESL_Mgr::PreTranslateMessage(pMsg);
	}

	if (pMsg->message == WM_CHAR && pMsg->hwnd == GetDlgItem(IDC_eBOOK_PRICE)->GetSafeHwnd())
    {
		INT nVirtKey = (INT) pMsg->wParam;
		if ((nVirtKey < '0' || nVirtKey > '9') && nVirtKey != '.' && nVirtKey != 8)
		{
			ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
            return TRUE;
		}
    }

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CBOVolumeDlg::OnSelChangeGridVolume() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_VOLUME);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;
	
	if (SetActiveGrid(GRID_VOLUME) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();	
}

VOID CBOVolumeDlg::OnSelChangeGridBook() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_BOOK);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;

	if (SetActiveGrid(GRID_BOOK) < 0) return;
	
	// 대표책 인덱스 가져오기
	INT nIdx;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
	if (m_nSelectedRow == nIdx)
		SetGridRowBackColor(pGrid, m_nSelectedRow);
	else
		pGrid->SetReverse(m_nSelectedRow);
	
	Display();	
}

VOID CBOVolumeDlg::OnSelChangeGridVolumeHold() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_VOLUME_HOLD);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;

	if (SetActiveGrid(GRID_VOLUME_HOLD) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();	
}

VOID CBOVolumeDlg::OnSelChangeGridBookHold() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_GRID_BOOK_HOLD);
	if (pGrid == NULL) return;
	m_nSelectedRow = pGrid->GetRow() - 1;
	
	if (SetActiveGrid(GRID_BOOK_HOLD) < 0) return;
	pGrid->SetReverse(m_nSelectedRow);

	Display();	
}

INT CBOVolumeDlg::Sort_BookDM(CESL_DataMgr * pDM)
{
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	strAlias[nCondColumns] = _T("BB_권일련번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_등록구분_코드");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	return 0;
}

INT CBOVolumeDlg::MergeVolume(INT nVolSelectedRow, CString strVol, CString strVolSortNo)
{
	CString strDupValue, strDupAlias, strTmpData, strSrcValue;
	if(!strVol.IsEmpty())
	{
		strDupValue = strVol;
		strDupAlias = _T("BB_편권차");
	}
	if(!strVolSortNo.IsEmpty())
	{
		strDupValue = strVolSortNo;
		strDupAlias = _T("BB_권일련번호");
	}
	
	INT nVolumeCnt = m_pDM_Volume->GetRowCount();
	for(INT i=0 ; i<nVolumeCnt ; i++)
	{
		strTmpData = m_pDM_Volume->GetCellData(strDupAlias, i);
		if(strTmpData == strDupValue)
			break;
	}
	strSrcValue = m_pDM_Volume->GetCellData(strDupAlias, nVolSelectedRow);

	INT nBookCnt = m_pDM_Book->GetRowCount();
	for(INT j=0 ; j<nBookCnt ; j++)
	{
		strTmpData = m_pDM_Book->GetCellData(strDupAlias, j);
		if(strTmpData == strSrcValue)
		{
			m_pDM_Book->SetCellData(_T("BB_편권차"), m_pDM_Volume->GetCellData(_T("BB_편권차"), i), j);
			m_pDM_Book->SetCellData(_T("BB_권일련번호"), m_pDM_Volume->GetCellData(_T("BB_권일련번호"), i), j);
			m_cCopyCodeMgr.SetSrcInfo(
				m_pDM_Volume->GetCellData(_T("BB_편권차"), i),
				m_pDM_Book->GetCellData(_T("BB_등록구분_코드"), j),
				m_pDM_Book->GetCellData(_T("BB_별치기호_코드"), j));

			INT nCopyCode = m_pBOSpeciesApi->GetLastDupCodeNumber(m_pDM_Book, &m_cCopyCodeMgr, j, 0, nBookCnt-1, _T("-1"));
			if(nCopyCode != 0) strTmpData.Format(_T("%d"), nCopyCode+1);
			else strTmpData.Empty();
			m_pDM_Book->SetCellData(_T("BB_복본기호"), strTmpData, j);
		}
	}

	m_pDM_Volume->FreeData();
	m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
	m_pDM_Volume->SORT(_T("BB_권일련번호"));
	ESLAfxMessageBox(_T("저장하였습니다."));
	m_nSelectedRow = 0;
	return 0;
}

CString CBOVolumeDlg::GetVol(CESL_DataMgr *pDM_Book, CString strModifiedVolSortNo)
{
	INT nRowCnt = pDM_Book->GetRowCount();
	CString strVolSortNo;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVolSortNo = pDM_Book->GetCellData(_T("BB_권일련번호"), i);
		if(strModifiedVolSortNo == strVolSortNo)
		{
			return pDM_Book->GetCellData(_T("BB_편권차"), i);
		}
	}
	return _T("NOT_FOUND");
}

CString CBOVolumeDlg::GetVolSortNo(CESL_DataMgr *pDM_Book, CString strModifiedVol)
{
	INT nRowCnt = pDM_Book->GetRowCount();
	CString strVol;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVol = pDM_Book->GetCellData(_T("BB_편권차"), i);
		if(strModifiedVol == strVol)
		{
			return pDM_Book->GetCellData(_T("BB_권일련번호"), i);
		}
	}
	return _T("");
}

INT CBOVolumeDlg::GetFirstRowBookDM(CESL_DataMgr *pDM_Book)
{
	if(pDM_Book == NULL) return -1;
	INT nRowCnt = pDM_Book->GetRowCount();
	if(nRowCnt == 0) return -1;

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(pDM_Book->GetCellData(_T("BB_DB_FLAG"), i) == _T("D")) continue;
		if(pDM_Book->GetCellData(_T("BB_책부록플래그"), i) == _T("A"))continue;
		return i;
	}
	return -1;
}

INT CBOVolumeDlg::SetCopyCode(CESL_DataMgr *pDM)
{
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	strAlias[nCondColumns] = _T("BB_등록번호");					nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_권일련번호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(m_cCopyCodeMgr.GetRegCodeCheckYN())
		strAlias[nCondColumns] = _T("BB_등록구분_코드");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	if(m_cCopyCodeMgr.GetSeShelfCodeCheckYN())
		strAlias[nCondColumns] = _T("BB_별치기호_코드");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	
	pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);
	INT nRowCnt = pDM->GetRowCount();
	CString strRegNo, strRegCode, strSeShelfCode, strVol, strTmpData;
	INT nLastCopyCode;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strRegNo = pDM->GetCellData(_T("BB_등록번호"), i);
		strRegCode = pDM->GetCellData(_T("BB_등록구분_코드"), i);
		strSeShelfCode = pDM->GetCellData(_T("BB_별치기호_코드"), i);
		strVol = pDM->GetCellData(_T("BB_편권차"), i);
		m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
		if(GetOpenMode() == 2100 || GetOpenMode() == -2100)
			nLastCopyCode = m_pFormApi->GetLastDupCodeNumber(pDM, i, i, strRegNo);
		else
			nLastCopyCode = m_pFormApi->GetLastDupCodeNumber(pDM, i, i);
		if(nLastCopyCode == 0) strTmpData = _T("");
		else strTmpData.Format(_T("%d"), ++nLastCopyCode);
		pDM->SetCellData(_T("BB_복본기호"), strTmpData, i);
	}
	return 0;
}

INT CBOVolumeDlg::SetCopyCode(CESL_DataMgr *pDM, INT nCurSel, CString strCopyCode)
{
	INT nRowCnt = pDM->GetRowCount();
	CString strRegNo, strRegCode, strSeShelfCode, strVol, strTmpData;
	strRegCode = pDM->GetCellData(_T("BB_등록구분_코드"), nCurSel);
	strSeShelfCode = pDM->GetCellData(_T("BB_별치기호_코드"), nCurSel);
	strVol = pDM->GetCellData(_T("BB_편권차"), nCurSel);
	if(strCopyCode.IsEmpty() || strCopyCode == _T("0") || strCopyCode == _T("1")) strCopyCode = _T("");
	m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{		
		if(i == nCurSel) continue;
		// 복본인지 검사한다.
		if (!m_cCopyCodeMgr.IsCopy(pDM, i)) continue;
		strTmpData = pDM->GetCellData(_T("BB_복본기호"), i);
		if(strTmpData == strCopyCode) 
		{
			ESLAfxMessageBox(_T("중복된 복본기호가 있습니다. 복본기호를 확인해 주십시요."));
			return -1;
		}
	}
	if(strCopyCode == _T("1")) 
		pDM->SetCellData(_T("BB_복본기호"), _T(""), nCurSel);
	return 0;
}

VOID CBOVolumeDlg::ShowCodeResourceByMediaCode(CString strMediaCode)
{
EFS_BEGIN
	BOOL bShowEBookInfo;
	INT i = 0;
	INT nCodeResourcesHeight = 65;
	CWnd *pWnd = NULL;

	if(strMediaCode == _T("EB")) bShowEBookInfo = TRUE;
	else bShowEBookInfo = FALSE;

	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (bShowEBookInfo && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_stcVOL_EBOOK_GROUP;
	nArrCodeResourceID[nCount++] = IDC_stcVOL_EBOOK_COVER_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtVOL_EBOOK_COVER_FILE;
	nArrCodeResourceID[nCount++] = IDC_btnVOL_FIND_FILE1;
	nArrCodeResourceID[nCount++] = IDC_btnVOL_FIND_FILE2;
	nArrCodeResourceID[nCount++] = IDC_btnVOL_FIND_FILE3;
	nArrCodeResourceID[nCount++] = IDC_stcVOL_EBOOK_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtVOL_EBOOK_FILE;
	nArrCodeResourceID[nCount++] = IDC_stcVOL_EBOOK_EXECUTE_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtVOL_EBOOK_EXECUTE_FILE;
	nArrCodeResourceID[nCount++] = IDC_btnVOL_EBOOK_VIEW;

	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (bShowEBookInfo)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Move Other Resources
	nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_b505TAG;
	nArrOtherResources[nCount++] = IDC_bINSERT;
	nArrOtherResources[nCount++] = IDC_btnNEXT;
	nArrOtherResources[nCount++] = IDC_btnPREV;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bDELETE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	nArrOtherResources[nCount++] = IDC_btnAPPLY_SERIES;
	nArrOtherResources[nCount++] = IDC_STATIC_GROUP2;
	
	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if (bShowEBookInfo)
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
	if (bShowEBookInfo)
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CBOVolumeDlg::OnSelchangecMEDIACLASS() 
{
	// TODO: Add your control notification handler code here
	CString strMediaCode;
	m_pCM->GetControlMgrData(_T("BB_매체구분_코드"), strMediaCode);
	if(strMediaCode != m_strPrevMediaCode && (strMediaCode == _T("EB") || m_strPrevMediaCode == _T("EB")))
		ShowCodeResourceByMediaCode(strMediaCode);
	m_strPrevMediaCode = strMediaCode;	
}

VOID CBOVolumeDlg::OnbtnVOLFINDFILE1() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_COVER, m_pCM);
	// CM 에 있는 내용을 적용
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book, m_nSelectedRow);
}

VOID CBOVolumeDlg::OnbtnVOLFINDFILE3() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_EXECUTER, m_pCM);
	// CM 에 있는 내용을 적용
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book, m_nSelectedRow);
}

VOID CBOVolumeDlg::OnbtnVOLFINDFILE2() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_FILE, m_pCM);
	// CM 에 있는 내용을 적용
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book, m_nSelectedRow);
}

VOID CBOVolumeDlg::OnbtnVOLEBOOKVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strExeFile, strEBookFile, strBookKey;
	m_pCM->GetControlMgrData(_T("BB_원본파일"), strEBookFile);
	m_pCM->GetControlMgrData(_T("BB_실행파일"), strExeFile);
	strBookKey = m_pDM_Book->GetCellData(_T("BB_책KEY"), m_nSelectedRow);
	m_pFormApi->EBookView(strBookKey, strExeFile, strEBookFile);
}

INT CBOVolumeDlg::AdjustCurrentBookDMByHoldBook(CESL_DataMgr *pDM_VolumeHold, CESL_DataMgr * pDM_BookHold, CESL_DataMgr *pDM_Book)
{
	if(pDM_VolumeHold == NULL || pDM_BookHold == NULL ||  pDM_VolumeHold == NULL) return -1;


	m_pFormApi->InitDefaultIgnoreAlias();
	CString strVol, strVolSortNo, strCopyCode;
	INT nRowCnt = pDM_Book->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVol = pDM_Book->GetCellData(_T("BB_편권차"), i);
		strVolSortNo = m_pBOSpeciesApi->FindVolSortNo(pDM_VolumeHold, strVol, -1);
		pDM_Book->SetCellData(_T("BB_권일련번호"), strVolSortNo, i);
		strVolSortNo = pDM_Book->GetCellData(_T("BB_권일련번호"), i);
		strCopyCode = pDM_Book->GetCellData(_T("BB_복본기호"), i);
		if(CheckDuplicateCopyCode(pDM_BookHold, strVolSortNo, strCopyCode, -1))
		{
			strVol = pDM_Book->GetCellData(_T("BB_편권차"), i);
			INT nCopyCode = m_pFormApi->GetLastDupCodeNumber(pDM_BookHold, strVol);
			strCopyCode.Format(_T("%d"), nCopyCode+1);
			pDM_Book->SetCellData(_T("BB_복본기호"), strCopyCode, i);			
		}
		
		m_pFormApi->CopyDM(pDM_Book, i, pDM_BookHold, -1);
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(pDM_BookHold, pDM_VolumeHold);
	}
	return 0;
}

INT CBOVolumeDlg::AdjustCurrentBookDMByHoldBook(CESL_DataMgr *pDM_BOOK)
{
	if(!Init())
	{
		ESLAfxMessageBox(_T("초기화 실폐"));
		return -1;
	}
	CESL_DataMgr DM_VolumeHold;
	DM_VolumeHold.InitDMPKFromFile(_T("DMUP_단행_권_소장권정보"));
	DM_VolumeHold.SetCONNECTION_INFO(pDM_BOOK->CONNECTION_INFO);
	DM_VolumeHold.SetMarcMgr(pParentMgr->m_pInfo->pMarcMgr);	
	CESL_DataMgr DM_BookHold;
	DM_BookHold.InitDMPKFromFile(_T("DMUP_단행_권_소장책정보"));
	DM_BookHold.SetCONNECTION_INFO(pDM_BOOK->CONNECTION_INFO);
	DM_BookHold.SetMarcMgr(pParentMgr->m_pInfo->pMarcMgr);

	if (!GetDupIdxKey().IsEmpty()) 
	{
		// 소장 책 만들기
		CString strOption;
		strOption.Format(_T("B.REC_KEY = E.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.BOOK_APPENDIX_FLAG='B'"), GetDupIdxKey());
		m_pFormApi->MakeBookDM(strOption, &DM_BookHold);
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(&DM_BookHold, &DM_VolumeHold);
	}
	else return -1;

	INT ids = AdjustCurrentBookDMByHoldBook(&DM_VolumeHold, &DM_BookHold, pDM_BOOK);
	return ids;
}

INT CBOVolumeDlg::AdjustCurrentBookDM(CESL_DataMgr *pDM_BOOK)
{
	if(pDM_BOOK == NULL) return -1;
	
	m_pFormApi->InitDefaultIgnoreAlias();
	CString strVol, strVolSortNo, strCopyCode;
	INT nRowCnt = pDM_BOOK->GetRowCount();
	INT nCondColumns = 0;
	CString strAlias[2];
	INT nCond[2];
	strAlias[nCondColumns] = _T("BB_편권차");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	strAlias[nCondColumns] = _T("BB_복본기호");				nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;	nCondColumns++;
	pDM_BOOK->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strVol = pDM_BOOK->GetCellData(_T("BB_편권차"), i);
		strVolSortNo = m_pBOSpeciesApi->FindVolSortNo(pDM_BOOK, strVol, i);
		pDM_BOOK->SetCellData(_T("BB_권일련번호"), strVolSortNo, i);
		strVolSortNo = pDM_BOOK->GetCellData(_T("BB_권일련번호"), i);
		strCopyCode = pDM_BOOK->GetCellData(_T("BB_복본기호"), i);
		if(CheckDuplicateCopyCode(pDM_BOOK, strVolSortNo, strCopyCode, -1))
		{
			strVol = pDM_BOOK->GetCellData(_T("BB_편권차"), i);
			INT nCopyCode = m_pFormApi->GetLastDupCodeNumber(pDM_BOOK, strVol);
			strCopyCode.Format(_T("%d"), nCopyCode+1);
			pDM_BOOK->SetCellData(_T("BB_복본기호"), strCopyCode, i);			
		}
	}
	return 0;
}

VOID CBOVolumeDlg::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
	if(m_nSelectedRow > 0)
		m_nSelectedRow--;

	Display();
	GetActiveGrid()->SetReverse(m_nSelectedRow);
}

VOID CBOVolumeDlg::OnbtnNEXT() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr * pDM = GetActiveDM();
	if(m_nSelectedRow < pDM->GetRowCount()-1)
		m_nSelectedRow++;

	Display();
	GetActiveGrid()->SetReverse(m_nSelectedRow);
}

VOID CBOVolumeDlg::ShowPrevNextButton()
{
	GetDlgItem(IDC_btnNEXT)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnPREV)->EnableWindow(TRUE);
	if(m_nSelectedRow == 0) 
		GetDlgItem(IDC_btnPREV)->EnableWindow(FALSE);
	if(m_nSelectedRow == GetActiveDM()->GetRowCount()-1)
		GetDlgItem(IDC_btnNEXT)->EnableWindow(FALSE);
}

VOID CBOVolumeDlg::OnKillfocuseBOOKPRICE() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlEdit * pEdt = (CESL_ControlEdit*)m_pCM->FindControl(_T("BB_가격"));
	if(!pEdt) return;
	if(pEdt->GetStyle() == ES_READONLY) return;	

	CString strPrice;
	m_pCM->GetControlMgrData(_T("BB_가격"), strPrice);
	CString strTmpData;
	INT nPos = strPrice.Find(_T("."));
	if(nPos > 0) strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;
	if(strTmpData.GetLength() > 8)
	{
		ESLAfxMessageBox(_T("가격정보는 소수점 사위자료가 8자리 이상일 수 없습니다."));
		GetDlgItem(IDC_eBOOK_PRICE)->SetFocus();
		return ;
	}
}

INT CBOVolumeDlg::SaveVolume(CESL_ControlMgr * pCM)
{
	// 등록구분 수정여부 체크
	if (IsRegCodeModified()) return -1;			
	
	// 수정을 했을때 같은 편권차가 있는지 체크
	CString strVol = m_pDM_Volume->GetCellData(_T("BB_편권차"), m_nSelectedRow);
	CString strVolSortNo = m_pDM_Volume->GetCellData(_T("BB_권일련번호"), m_nSelectedRow);
	CString strModifiedVol, strModifiedVolSortNo;
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_편권차"), strModifiedVol);
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_권일련번호"), strModifiedVolSortNo);
	//권호일련번호가 없을 경우 마지막 일련버호를 추출하여 설정한다.
	if(strModifiedVolSortNo.IsEmpty())
	{
		m_pFormApi->GetLastVolSortNo(m_pDM_Book, strModifiedVolSortNo);
		SetControlData(_T("CMUP_단행_권_책정보"), _T("BB_권일련번호"), strModifiedVolSortNo);
	}
	
	if (IsDuplicateVolume(strModifiedVol, m_nSelectedRow))//, strModifiedVolSortNo))
	{
		if(IDYES == ESLAfxMessageBox(_T("같은 이름의 편/권차가 존재합니다. 권정보를 통합 하시겠습니까?"), MB_YESNO))
		{
			MergeVolume(m_nSelectedRow, strModifiedVol, _T(""));
			return 0;
		}
		return -1;
	}
	
	if(IsDuplicateVolSortNo(strModifiedVolSortNo, m_nSelectedRow))
	{
		if(IDYES == ESLAfxMessageBox(_T("같은 권호일련번호가 존재합니다. 권정보를 통합 하시겠습니까?"), MB_YESNO))
		{
			MergeVolume(m_nSelectedRow, _T(""), strModifiedVolSortNo);
			return 0;
		}
		return -1;
	}
	
	// 발행일 정보로 발행년 설정하기
	//ApplyPublishDate();
	
	INT nBookRowCount = m_pDM_Book->GetRowCount();
	
	// 거부리스트 만들기 (책정보 수정안하게..)
	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_대표책여부"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_EBOOKKEY"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_복본기호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_배가위치부호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));

	// 등록구분코드 적용
	ApplyRegCode();

	CStringArray ArrIgnore;	ArrIgnore.RemoveAll();
	ArrIgnore.Add(_T("BB_별치기호_코드"));
	ArrIgnore.Add(_T("BB_관리구분_코드"));
	ArrIgnore.Add(_T("BB_이용제한구분_코드"));
	
	// 책에 적용
	CString strDBFlag;
	for (INT idx = 0; idx < nBookRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx, strVol)) continue;
		
		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		// CM 에 있는 내용을 적용
		if(strDBFlag == _T("N"))
			m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, idx, _T(""));
		else
			m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, idx, _T(""), &ArrIgnore);
		
		// 책부록 플래그 추가
		m_pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), idx);
		
		// DB Flag
		if(strDBFlag == _T("N"))
			m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
		else
		{
			CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
			m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		}
	}
	
	// 권정보 다시 구성
	m_pDM_Volume->FreeData();
	m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
	m_pDM_Volume->SORT(_T("BB_권일련번호"));
	ESLAfxMessageBox(_T("권정보를 수정하였습니다."));

	return 0;
}

INT CBOVolumeDlg::SaveBook(CESL_ControlMgr * pCM)
{
	BOOL bReSetCopyCode = FALSE;
	INT ids;
	CString strRegNo, strRegCode;
	CString strVol, strVolSortNo, strShelfCode;
	CString strModifiedRegCode, strModifiedVol, strModifiedVolSortNo, strModifiedShelfCode, strTmpData;
	
	strRegCode = m_pDM_Book->GetCellData(_T("BB_등록구분_코드"), m_nSelectedRow);
	strRegNo = m_pDM_Book->GetCellData(_T("BB_등록번호"), m_nSelectedRow);
	strVol = m_pDM_Book->GetCellData(_T("BB_편권차"), m_nSelectedRow);
	strVolSortNo = m_pDM_Book->GetCellData(_T("BB_권일련번호"), m_nSelectedRow);
	strShelfCode = m_pDM_Book->GetCellData(_T("BB_별치기호_코드"), m_nSelectedRow);	
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_편권차"), strModifiedVol);
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_권일련번호"), strModifiedVolSortNo);
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_별치기호_코드"), strModifiedShelfCode);
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strModifiedRegCode);
	
	m_cCopyCodeMgr.SetSrcInfo(strModifiedVol, strModifiedRegCode, strModifiedShelfCode);
	
	// 등록번호가 부여된 자료는 등록구분을 수정하면 안됨
	if(!strRegNo.IsEmpty() && strRegNo.Left(2) != strModifiedRegCode) 
	{
		ESLAfxMessageBox(_T("등록번호가 부여된 자료는 등록구분을 수정할 수 없습니다."));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegNo.Left(2));
		return 0;
	}
	
	// 이전 권일련번호와 수정된 권일련번호가 다를경우 - 편권차를 추출한다.
	// 이전 편권차와 수정된 편권차가 다를경우 - 권일련번호를 추출한다.
	if(strVolSortNo != strModifiedVolSortNo)
	{
		strTmpData = GetVol(m_pDM_Book, strModifiedVolSortNo);
		if(strTmpData != _T("NOT_FOUND")) strModifiedVol = strTmpData;
		SetControlData(_T("CMUP_단행_권_책정보"), _T("BB_편권차"), strModifiedVol);
	}
	else if(strVol != strModifiedVol)
	{
		strModifiedVolSortNo = GetVolSortNo(m_pDM_Book, strModifiedVol);
		if(strModifiedVolSortNo.IsEmpty())
			m_pFormApi->GetLastVolSortNo(m_pDM_Book, strModifiedVolSortNo);
		SetControlData(_T("CMUP_단행_권_책정보"), _T("BB_권일련번호"), strModifiedVolSortNo);
		bReSetCopyCode = TRUE;
	}
	if(strShelfCode != strModifiedShelfCode && m_cCopyCodeMgr.GetSeShelfCodeCheckYN() && m_pFormApi->GetVolumeCount(m_pDM_Book) > 1)
	{
		m_pDM_Book->SetCellData(_T("BB_별치기호_코드"), strModifiedShelfCode, m_nSelectedRow);
		bReSetCopyCode = TRUE;
	}
	if(strRegCode != strModifiedRegCode && m_cCopyCodeMgr.GetRegCodeCheckYN() && m_pDM_Book->GetRowCount() > 1)
	{
		m_pDM_Book->SetCellData(_T("BB_등록구분_코드"), strModifiedRegCode, m_nSelectedRow);
		bReSetCopyCode = TRUE;
	}
	if(bReSetCopyCode)
	{
		if(IDYES == ESLAfxMessageBox(_T("복본기호 부여기준 정보가 변경되었습니다.\n복본기호를 재설정 하시겠습니까?"), MB_YESNO))
		{
			INT nCopyCode = m_pBOSpeciesApi->GetLastDupCodeNumber(m_pDM_Book, &m_cCopyCodeMgr, m_nSelectedRow, 0, m_nSelectedRow, _T("-1"));
			if(nCopyCode != 0) strTmpData.Format(_T("%d"), nCopyCode+1);
			else strTmpData.Empty();
			m_pCM->SetControlMgrData(_T("BB_복본기호"), strTmpData);
			m_pDM_Book->SetCellData(_T("BB_복본기호"), strTmpData, m_nSelectedRow);
		}
	}
	m_pFormApi->InitDefaultIgnoreAlias();
	
	// CM 에 있는 내용을 적용
	ids = m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, m_nSelectedRow);
	if (ids < 0) return -1;
	
	// 책부록 플래그 추가
	m_pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), m_nSelectedRow);
	
	// DB Flag 적용
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), m_nSelectedRow));
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, m_nSelectedRow);
	
	// 등록구분코드 적용
	ApplyRegCode();
	ESLAfxMessageBox(_T("책정보를 수정하였습니다."));
	return 0;
}

INT CBOVolumeDlg::InsertVolume()
{
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_원본파일"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_이미지파일"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_실행파일"));
	// 추가를 했을때 같은 편권차가 있는지 체크
	CString strVol, strVolSortNo;
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_편권차"), strVol);
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_권일련번호"), strVolSortNo);
	//권호일련번호가 없을 경우 마지막 일련버호를 추출하여 설정한다.
	if (IsDuplicateVolume(strVol, -1))//
	{
		AfxMessageBox(_T("같은 이름의 편/권차가 존재합니다."));
		return -1;
	}
	
	if(strVolSortNo.IsEmpty() || IsDuplicateVolSortNo(strVolSortNo, -1))
	{
		m_pFormApi->GetLastVolSortNo(m_pDM_Book, strVolSortNo);
		SetControlData(_T("CMUP_단행_권_책정보"), _T("BB_권일련번호"), strVolSortNo);
	}
	
	// 발행일 정보로 발행년 설정하기
	//ApplyPublishDate();
	INT ids = -1;
	// 표준 책 만들기
	ids = MakeStandardBookFromCM(NULL);
	if (ids < 0) return -1;
	// 등록구분 필드 적용
	ids = ApplyRegCode();
	if (ids < 0) return -1;
	m_pDM_Volume->FreeData();
	m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
	m_pDM_Volume->SORT(_T("BB_권일련번호"));
	
	ESLAfxMessageBox(_T("권정보를 추가하였습니다."));
	return 0;
}

INT CBOVolumeDlg::InsertBook()
{
	CString strRegCode, strSeShelfCode, strVol, strTmpData, strVolSortNo;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCode);
	if (!m_pFormApi->IsValidRegCode(strRegCode))
	{
		AfxMessageBox(_T("등록구분값이 올바르지 않습니다."));
		return -1;
	}
	CStringArray arrIgnorelist;
	CString strWorkingStatus;
	arrIgnorelist.Add(_T("BB_등록구분_코드"));
	arrIgnorelist.Add(_T("BB_관리구분_코드"));
	arrIgnorelist.Add(_T("BB_별치기호_코드"));
	arrIgnorelist.Add(_T("BB_복본기호"));
	arrIgnorelist.Add(_T("BB_발행일자"));
	arrIgnorelist.Add(_T("BB_DB_FLAG"));
	arrIgnorelist.Add(_T("BB_책KEY"));
	arrIgnorelist.Add(_T("BB_대표책여부"));
	m_pFormApi->CopyDM(m_pDM_Book, m_nSelectedRow, m_pDM_Book, -1, _T(""), &arrIgnorelist);
	m_nSelectedRow = m_pDM_Book->GetRowCount() - 1;
	//자료상태 정의 배가 이후의 자료에 대하여 소급 입력할 경우 배가대상자료가 아닌경우 무조건 배가자료 상태를 갖도록 한다.
	strWorkingStatus = m_pDM_Book->GetCellData(_T("BB_자료상태"), m_nSelectedRow);
	if(strWorkingStatus.Left(3) == _T("BOL") && strWorkingStatus != _T("BOL111O"))
	{
		strWorkingStatus = _T("BOL112N");
		m_pDM_Book->SetCellData(_T("BB_자료상태"), strWorkingStatus, m_nSelectedRow);			
	}
	else if(strWorkingStatus.Left(3) == _T("BOR"))
	{
		strWorkingStatus = _T("BOR111N");
		m_pDM_Book->SetCellData(_T("BB_자료상태"), strWorkingStatus, m_nSelectedRow);			
	}
	m_pDM_Book->SetCellData(_T("BB_등록구분_코드"), strRegCode, m_nSelectedRow);
	m_pCM->GetControlMgrData(_T("BB_관리구분_코드"), strTmpData);
	m_pDM_Book->SetCellData(_T("BB_관리구분_코드"), strTmpData, m_nSelectedRow);
	m_pCM->GetControlMgrData(_T("BB_별치기호_코드"), strSeShelfCode);
	m_pDM_Book->SetCellData(_T("BB_별치기호_코드"), strSeShelfCode, m_nSelectedRow);
	m_pCM->GetControlMgrData(_T("BB_복본기호"), strTmpData);	
	m_pCM->GetControlMgrData(_T("BB_편권차"), strVol);	
	BOOL bReSetCopyCode = FALSE;
	if(strVol != m_pDM_Book->GetCellData(_T("BB_편권차"), m_nSelectedRow))
	{
		m_pFormApi->GetLastVolSortNo(m_pDM_Book, strVolSortNo);
		m_pCM->SetControlMgrData(_T("BB_권일련번호"), strVolSortNo);
		m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, m_nSelectedRow);
		m_pDM_Book->SetCellData(_T("BB_권일련번호"), strVolSortNo, m_nSelectedRow);
		bReSetCopyCode = TRUE;
	}
	strVolSortNo = m_pDM_Book->GetCellData(_T("BB_권일련번호"), m_nSelectedRow);

	m_cCopyCodeMgr.SetSrcInfo(strVol, strRegCode, strSeShelfCode);
	INT nCopyCode = m_pBOSpeciesApi->GetLastDupCodeNumber(m_pDM_Book, &m_cCopyCodeMgr, m_nSelectedRow, 0, m_nSelectedRow, _T("-1"));
	if(nCopyCode != 0) strTmpData.Format(_T("%d"), nCopyCode+1);
	else strTmpData.Empty();
	m_pCM->SetControlMgrData(_T("BB_복본기호"), strTmpData);
	m_pDM_Book->SetCellData(_T("BB_복본기호"), strTmpData, m_nSelectedRow);
/*
	if(CheckDuplicateCopyCode(m_pDM_Book, strVolSortNo, strTmpData, m_nSelectedRow))
	{
		strTmpData = m_pDM_Book->GetCellData(_T("BB_편권차"), m_nSelectedRow);
		INT nCopyCode = m_pFormApi->GetLastDupCodeNumber(m_pDM_Book, strTmpData);
		strTmpData.Format(_T("%d"), nCopyCode+1);
		m_pCM->SetControlMgrData(_T("BB_복본기호"), strTmpData);
		m_pDM_Book->SetCellData(_T("BB_복본기호"), strTmpData, m_nSelectedRow);
	}
*/	
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), m_nSelectedRow);
	m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("N"), m_nSelectedRow);
	ESLAfxMessageBox(_T("책정보를 추가하였습니다."));
	return 0;
}

INT CBOVolumeDlg::DeleteVolume()
{
	INT nVolCount = m_pDM_Volume->GetRowCount();
	if (nVolCount == 1) 
	{
		MessageBox(_T("마지막 권이므로 삭제할 수 없습니다."));
		return -1;
	}
	
	CString strVol;
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_편권차"), strVol);
	if (strVol.IsEmpty()) return -1;
	
	// 삭제 가능한 권인지 체크한다.
	if (m_pFormApi->CheckDeleteVolume(m_pDM_Book, strVol) < 0)
	{
		MessageBox(_T("삭제할수 없는 상태의 책이 존재합니다. 삭제할 수 없습니다."));
		return -1;
	}
	
	CString strMsg;
	strMsg.Format(_T("편/권자가 [%s]인 권을 삭제하시겠습니까?"), strVol);
	if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return -1;
	
	// 책 DM 에서 권에 대항하는 책을 지운다.
	m_pFormApi->DeleteVolume(m_pDM_Book, strVol);
	
	// 권 DM 에서 현재 Row를 지운다.
	m_pDM_Volume->DeleteRow(m_nSelectedRow--);
	return 0;
}

INT CBOVolumeDlg::DeleteBook()
{
	//INT nBookCount = m_pDM_Book->GetRowCount();
	INT nBookCount = m_pFormApi->GetBookCount(m_pDM_Book);
	if (nBookCount == 1)
	{
		AfxMessageBox(_T("마지막 책이므로 삭제할 수 없습니다."));
		return -1;
	}
	
	// 삭제 가능한 책인지 체크한다.
	if (m_pFormApi->CheckDeleteBook(m_pDM_Book, m_nSelectedRow) < 0)
	{
		MessageBox(_T("삭제할수 없는 상태의 책이 존재합니다. 삭제할 수 없습니다."));
		return -1;
	}
	
	CString strVolTitle;
	GetControlData(_T("CMUP_단행_권_책정보"), _T("BB_편제명부록명"), strVolTitle);
	CString strMsg;
	strMsg.Format(_T("권서명이 [%s]인 책을 삭제하시겠습니까?"), strVolTitle);
	if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return -1;
	
	// 책 DM 에서 선택한 로우를 지운다.
	m_pFormApi->DeleteBookDMRow(m_pDM_Book, m_nSelectedRow--);
	return 0;
}
