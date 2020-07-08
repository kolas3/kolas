// BOVolumeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOVolumeDlg.h"

#include "BOAppendixDlg.h"

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
	m_bShowHoldInfoButton = TRUE;


	m_pGrid_Volume = NULL;
	m_pGrid_VolumeHold = NULL;
	m_pGrid_Book = NULL;
	m_pGrid_BookHold = NULL;

	m_bReadOnly = FALSE;
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
	ON_BN_CLICKED(IDC_bAPPENDIX_INFO, OnbAPPENDIXINFO)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_VOLUME_BOOK, OnSelchangeTabVolumeBook)
	ON_BN_CLICKED(IDC_bVIEW_HOLD_INFO, OnbVIEWHOLDINFO)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_BN_CLICKED(IDC_b505TAG, Onb505TAG)
	ON_EN_KILLFOCUS(IDC_eREG_NO, OnKillfocuseREGNO)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_EN_KILLFOCUS(IDC_eREG_NO_SHOW, OnKillfocuseREGNOSHOW)
	ON_BN_CLICKED(IDC_bSELECT_REP_BOOK, OnbSELECTREPBOOK)
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
	INT ids;

	// DM 정보 
	if (m_pDM_Book == NULL)
	{
		m_pDM_Book = FindDM(_T("DM_단행_권_책정보"));
	}

	m_pDM_Volume = FindDM(_T("DM_단행_권_권정보"));
	
	m_pDM_BookHold = FindDM(_T("DM_단행_권_소장책정보"));
	m_pDM_VolumeHold = FindDM(_T("DM_단행_권_소장권정보"));

	if (m_pDM_Book == NULL ||
		m_pDM_Volume == NULL ||
		m_pDM_BookHold == NULL ||
		m_pDM_VolumeHold == NULL
		) 
		return -1;

	// CM 정보
	m_pCM = FindCM(_T("CM_단행_권_책정보"));
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


	// 권 DM 구축
	
	// 소장종이 있으면 소장종 구축한다.
	if (!GetDupIdxKey().IsEmpty()) 
	{
		// 소장 책 만들기
		CString strOption;
		strOption.Format(_T("SPECIES_KEY = %s"), GetDupIdxKey());
		ids = m_pDM_BookHold->RefreshDataManager(strOption);
		{
			AfxMessageBox(_T("RefreshDataManager 를 확인해주세요"));
			return -1;
		}

		// 소장 권 만들기
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_BookHold, m_pDM_VolumeHold);

		m_bShowHoldInfo = TRUE;
	}

	// Open Mode 에 따라서 ...
	if (GetOpenMode() == 1100)
	{
		m_pFormApi->SetAppMode(MODE_INSERT);

		// 등록번호는 입력 못한다.
		GetDlgItem(IDC_eREG_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_eREG_NO_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	else if (GetOpenMode() == -1100)
	{
		m_pFormApi->SetAppMode(MODE_UPDATE);

		// 등록번호는 입력 못한다.
		GetDlgItem(IDC_eREG_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_eREG_NO_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
	}
	
	// - 정리-종-입력 : 2100
	else if (GetOpenMode() == 2100)
	{
		m_bShowHoldInfoButton = FALSE;
		m_pFormApi->SetAppMode(MODE_INSERT);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_b505TAG)->ShowWindow(SW_SHOW);

		// 책 그리드의 DM 연결을 권으로 바꾼다.
		CString strLinkDM;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DM_단행_종_책정보"), strLinkDM);
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
		// 책 그리드의 DM 연결을 권으로 바꾼다.
		CString strLinkDM;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DM_단행_종_책정보"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();

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
		//등록번호 입력할 수 없고 대표책선정 버튼도 보이지 않게한다.
		GetDlgItem(IDC_eREG_NO)->EnableWindow(FALSE);
		GetDlgItem(IDC_eREG_NO_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSELECT_REP_BOOK)->ShowWindow(SW_HIDE);
		m_bShowHoldInfoButton = FALSE;
		if (m_strSpeciesKey.IsEmpty()) return -1;
		// DM 구성한다.
		CString strOption;
		if (m_strPurchaseKey.IsEmpty())
			strOption.Format(_T("SPECIES_KEY = %s AND REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY REPRESENT_BOOK_YN DESC"), m_strSpeciesKey);
		else
			strOption.Format(_T("SPECIES_KEY = %s AND ACCU_KEY = %s AND REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY REPRESENT_BOOK_YN DESC"), m_strSpeciesKey, m_strPurchaseKey);
		ids = m_pDM_Book->RefreshDataManager(strOption);
		if (ids < 0)
			return -1;

		// 책 그리드의 DM 연결을 권으로 바꾼다.
		CString strLinkDM = m_pDM_Book->DB_MGR_ALIAS;
		strLinkDM.Format(_T(":%s"), m_pDM_Book->DB_MGR_ALIAS);
		CString strLinkData = m_pGrid_Book->ELEMENT->LINK_DATA;
		strLinkData.Replace(_T(":DM_단행_종_책정보"), strLinkDM);
		m_pGrid_Book->ELEMENT->LINK_DATA = strLinkData;
		m_pGrid_Book->InitColumnLinkDMAndAlias();
		m_pGrid_Book->InitColumnInfo();

		m_bReadOnly = TRUE;
	}

	return 0;
}

INT CBOVolumeDlg::Display()
{
	if (m_nSelectedRow < 0) return -1;

	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_권_책정보"));
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

	ShowRegCodeField();
	ShowRegNo();
	OnKillfocuseREGCLASSCODE();

	if (m_bReadOnly)
		DisplayControlReadOnly();

    return 0;
}

INT CBOVolumeDlg::ClearScreen()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_권_책정보"));
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
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if (strRegNo.GetLength() < 3) 
// 		strRegNo.Empty();
// 	else
// 		strRegNo = strRegNo.Mid(2);
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;

	if (strRegNo.GetLength() <= nRegCodeLength) 
		strRegNo.Empty();
	else
		strRegNo = strRegNo.Mid(nRegCodeLength);
	//===================================================================

	GetDlgItem(IDC_eREG_NO_SHOW)->SetWindowText(strRegNo);

	return 0;
}

BOOL CBOVolumeDlg::IsDuplicateVolume(CString strVol)
{
	//if (strVol.IsEmpty()) return FALSE;
	
	CString strOriginVol;
	INT nRowCount = m_pDM_Volume->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		m_pDM_Volume->GetCellData(_T("BB_편권차"), idx, strOriginVol);
		if (strVol == strOriginVol) return TRUE;
	}

	return FALSE;
}

INT CBOVolumeDlg::MakeStandardBookFromCM(CStringArray *pArrAbortList /*= NULL*/)
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_권_책정보"));
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

	// 책상태일때 복본기호 추가
	if (m_nSelectedGrid == GRID_BOOK || m_nSelectedGrid == GRID_BOOK_HOLD)
	{
		CString strVol = m_pDM_Book->GetCellData(_T("BB_편권차"), nIdx);
		INT nDupNumber = m_pFormApi->GetLastDupCodeNumber(m_pDM_Book, strVol);

		CString strDupCode;
		strDupCode.Format(_T("c%d"), ++nDupNumber);
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
		GetControlData(_T("CM_단행_권_책정보"), _T("BB_편권차"), strVol);

		m_pFormApi->MakeBookFromRegCode(&arrRegCode, m_pDM_Book, strVol);
	}
	// 책상태
	else
	{
		CString strRegCode;
		GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCode);

		// 맞는 등록구분인지 체크
		if (!m_pFormApi->IsValidRegCode(strRegCode))
		{
			AfxMessageBox(_T("등록구분값이 올바르지 않습니다."));
			return -1;
		}
		m_pDM_Book->SetCellData(_T("BB_등록구분_코드"), strRegCode, m_nSelectedRow);
		
		// 책부록 플래그 추가
		m_pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), m_nSelectedRow);
	}

	return 0;
}

INT CBOVolumeDlg::ApplySaveData()
{
	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_권_책정보"));
	if (pCM == NULL) return -1;

	INT ids;
	// 수서-종-구입-입력 || // 수서-종-구입-수정 || // - 정리-종-수정 : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 || GetOpenMode() == -2100)
	{
		// 권 상태에서 수정을 했을때
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			// 등록구분 수정여부 체크
			if (IsRegCodeModified()) return -1;
			
			
			// 수정을 했을때 같은 편권차가 있는지 체크
			CString strVol = m_pDM_Volume->GetCellData(_T("BB_편권차"), m_nSelectedRow);
			CString strModifiedVol;
			GetControlData(_T("CM_단행_권_책정보"), _T("BB_편권차"), strModifiedVol);

			if (strVol != strModifiedVol && IsDuplicateVolume(strModifiedVol))
			{
				AfxMessageBox(_T("같은 이름의 편/권차가 존재합니다."));
				return -1;
			}

			INT nBookRowCount = m_pDM_Book->GetRowCount();

			// 거부리스트 만들기 (책정보 수정안하게..)
			m_pFormApi->InitDefaultIgnoreAlias();
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_대표책여부"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_복본기호"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_배가위치부호"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_별치기호_코드"));
					
			// 책에 적용
			for (INT idx = 0; idx < nBookRowCount; idx++)
			{
				// 책이 아니면 넘어간다.
				if (!m_pFormApi->IsBook(m_pDM_Book, idx, strVol)) continue;
				
				// CM 에 있는 내용을 적용
				m_pFormApi->MakeDMFromCM(pCM, m_pDM_Book, idx, _T(""));

				// 책부록 플래그 추가
				m_pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), idx);

				// DB Flag
				CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
				m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
			}

			// 등록구분코드 적용
			ApplyRegCode();

			// 권정보 다시 구성
			m_pDM_Volume->FreeData();
			m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
		}
		// 책 상태에서 수정을 했을때
		else
		{
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
		}
	}

	return 0;
}

INT CBOVolumeDlg::ApplyInsertData()
{
	// 수서-종-구입-입력 || // 수서-종-구입-수정 || // - 정리-종-수정 : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 || GetOpenMode() == -2100)
	{
		// 거부리스트 만들기 (책정보 수정안하게..)
		m_pFormApi->InitDefaultIgnoreAlias();
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_대표책여부"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_복본기호"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));

		// 권 상태
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			// 추가를 했을때 같은 편권차가 있는지 체크
			CString strVol;
			GetControlData(_T("CM_단행_권_책정보"), _T("BB_편권차"), strVol);

			if (IsDuplicateVolume(strVol))
			{
				AfxMessageBox(_T("같은 이름의 편/권차가 존재합니다."));
				return -1;
			}		
		}
		// 책상태
		else
		{
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_배가위치부호"));
			m_pFormApi->AddDefaultIgnoreAlias(_T("BB_별치기호_코드"));
		}

		INT ids = -1;

		// 표준 책 만들기
		ids = MakeStandardBookFromCM(NULL);
		if (ids < 0) return -1;

		// 책상태일때 SelectedRow 수정및 책 부록 플래그 추가
		if (m_nSelectedGrid == GRID_BOOK || m_nSelectedGrid == GRID_BOOK_HOLD)
		{
			m_nSelectedRow = m_pDM_Book->GetRowCount() - 1;
			// 책부록 플래그 추가
			m_pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), m_nSelectedRow);
		}

		// 등록구분 필드 적용
		ids = ApplyRegCode();
		if (ids < 0) return -1;

		// 권 상태에서 추가를 했을때
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			// 권정보 다시 구성
			m_pDM_Volume->FreeData();
			m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
		}
	}

	return 0;
}

INT CBOVolumeDlg::ApplyDeleteData()
{
	// 수서-종-구입-입력 || // 수서-종-구입-수정 || // - 정리-종-수정 : -2100
	if (GetOpenMode() == 1100 || GetOpenMode() == -1100 || GetOpenMode() == 2100 ||  GetOpenMode() == -2100)
	{
		// 권 상태
		if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD)
		{
			INT nVolCount = m_pDM_Volume->GetRowCount();
			if (nVolCount == 1) 
			{
				MessageBox(_T("마지막 권이므로 삭제할 수 없습니다."));
				return -1;
			}

			CString strVol;
			GetControlData(_T("CM_단행_권_책정보"), _T("BB_편권차"), strVol);
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
		}
		// 책상태
		else
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
			GetControlData(_T("CM_단행_권_책정보"), _T("BB_편제명부록명"), strVolTitle);
			CString strMsg;
			strMsg.Format(_T("권서명이 [%s]인 책을 삭제하시겠습니까?"), strVolTitle);
			if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return -1;
			
			// 책 DM 에서 선택한 로우를 지운다.
			m_pFormApi->DeleteBookDMRow(m_pDM_Book, m_nSelectedRow--);
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
	nArrButtonCtrl[nButtonCount++] = IDC_bAPPENDIX_INFO;

	CWnd *pWnd = NULL;

	CString strBookAppFlag;
	if (m_nSelectedGrid == GRID_BOOK)
		strBookAppFlag = m_pDM_Book->GetCellData(_T("BB_책부록플래그"), m_nSelectedRow);

	// 부록이면 추가, 등록구분 코드를 Disable
	if (strBookAppFlag == _T("A"))
	{
		((CEdit*)GetDlgItem(IDC_eREG_CLASS_CODE))->SetReadOnly(TRUE);
		GetDlgItem(IDC_bINSERT)->EnableWindow(FALSE);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_eREG_CLASS_CODE))->SetReadOnly(FALSE);
		GetDlgItem(IDC_bINSERT)->EnableWindow(TRUE);
	}

	

	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;

		if (m_bReadOnly && nArrButtonCtrl[i] != IDC_bAPPENDIX_INFO)
		{			
			pWnd->EnableWindow(FALSE);
		}
		else if (m_nSelectedGrid == GRID_VOLUME)
			pWnd->EnableWindow(TRUE);
		else if (m_nSelectedGrid == GRID_BOOK)
		{
			if (strBookAppFlag == _T("A"))
				pWnd->EnableWindow(FALSE);
			else
				pWnd->EnableWindow(TRUE);
		}			
		else if (m_nSelectedGrid == GRID_VOLUME_HOLD || m_nSelectedGrid == GRID_BOOK_HOLD)
			pWnd->EnableWindow(FALSE);
		else
			return -1;
	}

	return 0;
}

INT CBOVolumeDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	CESL_DataMgr * pDM = GetActiveDM();
	if(!pDM) return 0;
	if(strGridAlias == _T("GRID_VOLUME") || strGridAlias == _T("GRID_VOLUME_HOLD")) 
		return 0;

	if(strDMFieldAlias == _T("UDF_책자료상태"))
	{
		CString strWorkingStatus = pDM->GetCellData(_T("BB_자료상태"), nRow);
		if(strWorkingStatus == _T("BOA111N")) str = _T("구입대상자료");
		else if(strWorkingStatus == _T("BOA112N")) str = _T("주문선정자료");
		else if(strWorkingStatus == _T("BOA113N")) str = _T("검수대상자료");
		else if(strWorkingStatus == _T("BOA114O")) str = _T("미납품자료");
		else if(strWorkingStatus == _T("BOA131N")) str = _T("기증대상자료");
		else if(strWorkingStatus == _T("BOA132N")) str = _T("기증부당자료");
		else if(strWorkingStatus == _T("BOA141N")) str = _T("검수완료자료");
		else if(strWorkingStatus == _T("BOA142N")) str = _T("접수완료자료");
		else if(strWorkingStatus == _T("BOC111N")) str = _T("편목인계자료");
		else if(strWorkingStatus == _T("BOC112O")) str = _T("분류대상사료");
		else if(strWorkingStatus == _T("BOC113N")) str = _T("편목대상자료");
		else if(strWorkingStatus == _T("BOL111O")) str = _T("비치대상자료");
		else if(strWorkingStatus == _T("BOL112N")) str = _T("비치자료");
		else if(strWorkingStatus == _T("BOL221O")) str = _T("관외대출자료");
		else if(strWorkingStatus == _T("BOL222O")) str = _T("관내대출자료");
		else if(strWorkingStatus == _T("BOL223O")) str = _T("특별대출자료");
		else if(strWorkingStatus == _T("BOL224O")) str = _T("긴급대출자료");
		else if(strWorkingStatus == _T("BOL225O")) str = _T("배달자료");
		else if(strWorkingStatus == _T("BOL251N")) str = _T("수리제본자료");
		else if(strWorkingStatus == _T("BOL252N")) str = _T("분실자료");
		else if(strWorkingStatus == _T("BOL253N")) str = _T("제적자료");
		else if(strWorkingStatus == _T("BOL217O")) str = _T("장기대출자료");
		
		pDM->SetCellData(_T("UDF_책자료상태"), str, nRow);
	}
	return 0;
}

INT CBOVolumeDlg::DisplayActiveGrid(INT nRow, BOOL bSetTopRow /*=FALSE*/)
{
	CESL_Grid *pGrid  = GetActiveGrid();
	if (pGrid == NULL) return - 1;

	// 책부록플래그가 B 인경우 블랭크로 A인 경우 _T("V")로 표시
	CString strBookAppendixFlag;
	CESL_DataMgr *pDM = GetActiveDM();
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

	// 책 소팅
	SortBookDM(pDM);
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

		if (nRow != nIdx)
			pGrid->SetReverse(nRow);
	}
	else
		pGrid->SetReverse(nRow);

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
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_eREG_NO;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_eREG_NO_SHOW;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_eCOPY_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cSHELF_LOC_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cSHELF_CODE;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cUSE_LIMIT_CLASS;
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_cMANAGE_CLASS;
	//대표책 선정버튼
	nArrBookOnlyCtrl[nBookOnlyCtrlCount++] = IDC_bSELECT_REP_BOOK;

	// 권에서만 쓰이는 Ctrl
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_cMEDIA_CLASS;
	//nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_cUSE_LIMIT_CLASS;
	//nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_cMANAGE_CLASS;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eVOL;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eVOLTITLE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAUTHOR;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePUBLISH_YEAR;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eEA_ISBN;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eISBN2;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eISBN3;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePAGE_CNT;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePHYSICAL_PROPERTIES;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eSIZE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAPPENDIX;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eBOOK_PRICE;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_ePRICE_OTHRE_INFO;
	nArrVolumeOnlyCtrl[nVolumeOnlyCtrlCount++] = IDC_eAPPLICANT;
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
	//nArrButtonCtrl[nButtonCount++] = IDC_bAPPENDIX_INFO;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_b505TAG;
	nArrButtonCtrl[nButtonCount++] = IDC_bISBN_INSERT;
	nArrButtonCtrl[nButtonCount++] = IDC_bISBN_CHECK;
	nArrButtonCtrl[nButtonCount++] = IDC_eAPPLICANT;

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

	// 편권차로 정렬한다.
	pDM_Book->SORT(_T("BB_편권차"));

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
	INT ids = InitESL_Mgr(_T("SM_단행_권"));
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
		AfxMessageBox(_T("Init Data Manger Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_nSelectedRow = 0;
	//AllControlDisplay(_T("CM_단행_권_책정보"));
	
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab != NULL)
	{
		pTab->InsertItem(0, _T("권 정보"));
		pTab->InsertItem(1, _T("책 정보"));

		pTab->SetCurFocus(1);
		pTab->SetCurFocus(0);
	}

	// Status Bar For Grab HAHA;;
	//m_statusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);

	GetWindowRect(m_rcWindow);

	if (m_bShowHoldInfo)
	{
		m_bShowHoldInfo = FALSE;
		OnbVIEWHOLDINFO();
	}

	//Display();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOVolumeDlg::OnDestroy() 
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

VOID CBOVolumeDlg::OnKillfocuseREGNOSHOW() 
{
	// 권별정보면 리턴
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD) return;

	CEdit* pEditRegNoShow = (CEdit*)GetDlgItem(IDC_eREG_NO_SHOW);
	if (pEditRegNoShow == NULL) return;

	// 리드 온리이면 체크하지 않는다.
	if (pEditRegNoShow->GetStyle() & ES_READONLY) return;
	
	CString strQuery;
	CString strResult;

	INT idx = -1;
	INT nRowCount = -1;
	CString strRegCode;
	CString strRegNo;
	CString strRegNoField;
	CString strSelectRowRecKey;

	// 등록본호 가져온다.
	pEditRegNoShow->GetWindowText(strRegNo);
	if (strRegNo.IsEmpty()) return;

	INT nRegNo = _ttoi(strRegNo);

	// 등록구분가져온다.
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCode);
	if (strRegCode.IsEmpty()) return;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strRegNo.Format(_T("%010d"), nRegNo);
// 	pEditRegNoShow->SetWindowText(strRegNo);
// 	strRegNoField = strRegCode + strRegNo;

	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	strRegNoField = pParentMgr->m_pInfo->MakeRegNo(strRegCode,strRegNo);
	strRegNo = strRegNoField.Mid(nRegCodeLength);
	
	pEditRegNoShow->SetWindowText(strRegNo);
	//===================================================================
	// 마지막번호보다 클경우 메시지보여준다.
	CTime t(CTime::GetCurrentTime());
	
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE MANAGE_CODE = UDF_MANAGE_CODE AND DIVISION_VALUE = '%s' AND MAKE_YEAR = '%d' AND '%d' > LAST_NUMBER")
					, strRegCode, t.GetYear(), nRegNo);
	m_pDM_Book->GetOneValueQuery(strQuery, strResult);
	if  (_ttoi(strResult) > 0)
		goto OVER_NO;


	// 현재 BOOK_DM에서 중복을 체크한다.
	nRowCount = m_pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		// 현재 DM Row이면 넘어간다.
		if (idx == m_nSelectedRow) continue;

		// 책이 아니면 넘어간다.
		// if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;

		// 비교한다.
		if (m_pDM_Book->GetCellData(_T("BB_등록번호"), idx) == strRegNoField) goto FIND_DUP;
	}

	// DB에서 등록번호 중복을 체크한다.
	strQuery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '%s'"), strRegNoField);
	m_pDM_Book->GetOneValueQuery(strQuery, strResult);
	
	strSelectRowRecKey = m_pDM_Book->GetCellData(_T("BB_책KEY"), m_nSelectedRow);
	if(!strResult.IsEmpty() && strSelectRowRecKey != strResult) goto FIND_DUP;
	
	/*if (_ttoi(strResult) > 0)
		goto FIND_DUP;
	*/
	// 실제 등록번호 컨트롤에 등록번호를 넣는다.
	GetDlgItem(IDC_eREG_NO)->SetWindowText(strRegNoField);
	
	return;

FIND_DUP:
	MessageBox(_T("등록번호가 중복됩니다. 다시 입력해 주세요"));
	goto SET_REG_NO_SHOW;

OVER_NO:
	MessageBox(_T("등록번호가 마지막 번호보다 큽니다. 다시 입력해 주세요"));
	goto SET_REG_NO_SHOW;
	
SET_REG_NO_SHOW:
	strRegNo = m_pDM_Book->GetCellData(_T("BB_등록번호"), m_nSelectedRow);
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if(!strRegNo.IsEmpty()) strRegNo = strRegNo.Right(10);
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	
	if(!strRegNo.IsEmpty()) strRegNo = strRegNo.Right(12 - nRegCodeLength);
	//===================================================================
	GetDlgItem(IDC_eREG_NO_SHOW)->SetWindowText(strRegNo);
	pEditRegNoShow->SetFocus();			
	return;

}


BEGIN_EVENTSINK_MAP(CBOVolumeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOVolumeDlg)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME, -605 /* MouseDown */, OnMouseDownGridVolume, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK, -605 /* MouseDown */, OnMouseDownGridBook, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_VOLUME_HOLD, -605 /* MouseDown */, OnMouseDownGridVolumeHold, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOVolumeDlg, IDC_GRID_BOOK_HOLD, -605 /* MouseDown */, OnMouseDownGridBookHold, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
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
	
	Display();
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
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
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
}

VOID CBOVolumeDlg::OnSelchangeTabVolumeBook(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTabCtrl *pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_VOLUME_BOOK);
	if (pTab == NULL) return;

	m_nSelectedRow = 0;

	DisplayContorls();

	INT nCurSel = pTab->GetCurSel();

	if (nCurSel == 0)
	{
		// 권 정보 다시 구성
		m_pDM_Volume->FreeData();
		m_pBOSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_Book, m_pDM_Volume);
	}
	else if (nCurSel == 1)
	{
	
	}

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

VOID CBOVolumeDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;
	
	// StatusBar Ctrl
	/*if (m_statusBarCtrl.GetSafeHwnd() != NULL)
		m_statusBarCtrl.SendMessage(WM_SIZE, 0, 0);
		*/

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

VOID CBOVolumeDlg::OnbCLOSE() 
{
	// 소장종이 아니면 대표책 정보 자동 세팅
	if (GetDupIdxKey().IsEmpty())
		m_pFormApi->SetRepresentationBook(m_pDM_Book);

	// 복본기호 자동 일괄 적용
	m_pFormApi->AutoApplyDupCode(m_pDM_Book);

	// 부록등록번호 자동 일괄 적용
	m_pFormApi->AutoApplyAppendixRegNo(m_pDM_Book);
	
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

VOID CBOVolumeDlg::OnKillfocuseREGNO() 
{
	// 2003.03.06 더이상 사용하지 않음 : 등록번호를 숫자로 보여줌 OnKillfocusREGNOSHOW를 보기바람
	
	/*
	// 권별정보면 리턴
	if (m_nSelectedGrid == GRID_VOLUME || m_nSelectedGrid == GRID_VOLUME_HOLD) return;

	CEdit* pEditRegClassNo = (CEdit*)GetDlgItem(IDC_eREG_NO);
	if (pEditRegClassNo == NULL) return;
	
	// 리드 온리이면 체크하지 않는다.
	if (pEditRegClassNo->GetStyle() & ES_READONLY) return;

	CString strRegNoField;
	pEditRegClassNo->GetWindowText(strRegNoField);
	if (strRegNoField.IsEmpty()) return;


	// 등록구분을 체크한다.
	CString strRegCode = strRegNoField.Left(2);
	if (!m_pFormApi->IsValidRegCode(strRegCode))
	{
		MessageBox(_T("사용하지 않는 등록구분입니다."));
		pEditRegClassNo->SetFocus();			
	}

	if (strRegNoField.GetLength() < 3) return;
	
	//등록번호 체크
	
	INT idx = -1;
	INT nRowCount = -1;
	CString strQuery;
	CString strResult;

	CString strRegNo = strRegNoField.Mid(2);
	INT nRegNo = _ttoi(strRegNo);
	
	strRegNoField.Format(_T("%s%010d"), strRegCode,	nRegNo);
	pEditRegClassNo->SetWindowText(strRegNoField);

	// 마지막번호보다 클경우 메시지보여준다.
	CTime t(CTime::GetCurrentTime());
	
	strQuery.Format(_T("SELECT COUNT(*) FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE DIVISION_VALUE = '%s' AND MAKE_YEAR = '%d' AND '%d' > LAST_NUMBER")
					, strRegCode, t.GetYear(), nRegNo);
	m_pDM_Book->GetOneValueQuery(strQuery, strResult);
	if  (_ttoi(strResult) > 0)
		goto OVER_NO;


	// 현재 BOOK_DM에서 중복을 체크한다.
	nRowCount = m_pDM_Book->GetRowCount();
	for (idx = 0; idx < nRowCount; idx++)
	{
		// 현재 DM Row이면 넘어간다.
		if (idx == m_nSelectedRow) continue;

		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;

		// 비교한다.
		if (m_pDM_Book->GetCellData(_T("BB_등록번호"), idx) == strRegNoField) goto FIND_DUP;
	}

	// DB에서 등록번호 중복을 체크한다.
	strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE REG_NO = '%s'"), strRegNoField);
	m_pDM_Book->GetOneValueQuery(strQuery, strResult);

	if (_ttoi(strResult) > 0)
		goto FIND_DUP;	
	
	return;

FIND_DUP:
	MessageBox(_T("등록번호가 중복됩니다. 다시 입력해 주세요"));
	pEditRegClassNo->SetFocus();			
	return;

OVER_NO:
	MessageBox(_T("등록번호가 마지막 번호보다 큽니다. 다시 입력해 주세요"));
	pEditRegClassNo->SetFocus();			
	return;
	*/
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
