// BO_Species_Detail_Viewer.cpp : implementation file
//

#include "stdafx.h"
#include "BO_Species_Detail_Viewer.h"
#include "BO_Species_Detail_Marc_Viewer.h"
#include "BO_Species_Detail_Appendix_Viewer.h"
#include "../COMMON_EBOOK_API/EBookViewMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_Species_Detail_Viewer dialog


CBO_Species_Detail_Viewer::CBO_Species_Detail_Viewer(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_Species_Detail_Viewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_Species_Detail_Viewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOpenMode = 0;
	m_pCM = NULL;
	m_pDM_INDEX = NULL;
	m_pDM_SPECIES = NULL;
	m_pDM_BOOK = NULL;
	m_pDM_VOLUME = NULL;
	m_pSpeciesApi = NULL;
	m_pMarcDefaultValueConfigDlg = NULL;
	m_nIdx = -1;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;
	m_pGrid_Volume = NULL;
	m_nFirstIdx = -1;;
	m_nLastIdx = -1;
	m_bGridSelectYN = FALSE;
}

CBO_Species_Detail_Viewer::~CBO_Species_Detail_Viewer()
{
	CEBookViewMgr mgr;
	mgr.DeleteEBookTempFolder();
	if(m_pSpeciesApi)
	{
		delete m_pSpeciesApi;
		m_pSpeciesApi = NULL;
	}
	if(m_pMarcDefaultValueConfigDlg)
	{
		delete m_pMarcDefaultValueConfigDlg;
		m_pMarcDefaultValueConfigDlg = NULL;
	}
}

VOID CBO_Species_Detail_Viewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_Species_Detail_Viewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_Species_Detail_Viewer, CDialog)
	//{{AFX_MSG_MAP(CBO_Species_Detail_Viewer)
	ON_BN_CLICKED(IDC_btnDVIEWER_PREV, OnbtnDVIEWERPREV)
	ON_BN_CLICKED(IDC_btnDVIEWER_NEXT, OnbtnDVIEWERNEXT)
	ON_BN_CLICKED(IDC_btnDVIEWER_MARC_VIEW, OnbtnDVIEWERMARCVIEW)
	ON_BN_CLICKED(IDC_btnDVIEWER_APPENDIX_VIEW, OnbtnDVIEWERAPPENDIXVIEW)
	ON_BN_CLICKED(IDC_btnEBOOK_VIEW, OnbtnEBOOKVIEW)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_Species_Detail_Viewer message handlers

BOOL CBO_Species_Detail_Viewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_단행_종_상세보기")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_단행_종_상세보기"));
	m_pDM_INDEX = FindDM(_T("DMUP_단행_종_색인정보"));
	m_pDM_SPECIES = FindDM(_T("DMUP_단행_종_종정보"));
	m_pDM_BOOK = FindDM(_T("DMUP_단행_종_상세보기_책정보"));
	m_pDM_VOLUME = FindDM(_T("DMUP_단행_종_상세보기_권정보"));
	m_pGrid_Volume = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid_Volume"));
	
	if(!Init())
	{
		ESLAfxMessageBox(_T("종 상세보기 화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	Display(0);

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBO_Species_Detail_Viewer::Init()
{
	if(m_nOpenMode == 0) return FALSE;
	if(m_pParentDM == NULL || m_pParentDM->GetRowCount() == 0) return FALSE;
	if(m_pParentGrid == NULL) return FALSE;
	if(m_strSpeciesKeyAlias.IsEmpty()) m_strSpeciesKeyAlias = _T("IBS_종KEY");
	
	m_nIdx = m_pParentGrid->GetRow() - 1;
	if(m_nIdx < 0) m_nIdx = 0;

	//종/책의 테이블 이름 설정
	if(m_nOpenMode > 0) 
	{
		m_pDM_SPECIES->TBL_NAME = _T("BO_DELETE_SPECIES_TBL");
		m_pDM_BOOK->TBL_NAME = _T("BO_DELETE_BOOK_TBL");
	}
	else if(m_nOpenMode < 0)
	{
		m_pDM_SPECIES->TBL_NAME = _T("BO_SPECIES_TBL");
		m_pDM_BOOK->TBL_NAME = _T("BO_BOOK_TBL");
	}
	
	//종API초기화
	m_pSpeciesApi = new CBO_SPECIES(this);
	//마크 깆본값 설정 화면 초기화
	m_pMarcDefaultValueConfigDlg = new CMarcDefaultValueConfigDlg(this);
	
	return TRUE;
}

VOID CBO_Species_Detail_Viewer::Display(INT idx)
{
	switch(idx)
	{
	case 0:
		InitFirstLastIndex();
		m_nIdx = GetFirstSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		m_nIdx = GetPrevSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		m_nIdx = GetNextSelectIndex();
		ShowNextPrevButton(m_nIdx);
		break;
	default:
		return ;
	}
	SetDetailInfo();
	m_marcEditor.Display();
	m_pCM->AllControlDisplay(0);
	m_pGrid_Volume->Display();
	SetWorkingStatusDesc();
	m_pParentGrid->SetReverse(m_nIdx);
	m_pGrid_Volume->SetReverse(0);
}

INT CBO_Species_Detail_Viewer::SetMarcInfoAndCtrl()
{
	//마크와 그리드 컨트롤 연결
	m_strStreamMarc = m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc);
	m_marcEditor.SubclassDlgItem(-1, IDC_gridDETAIL_VIEWER_INFO, this);
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.InitUserAlias();
	m_marcEditor.GetGridCtrl()->SetGridColWidth(-1, -1, 420);

	m_marcEditor.AddUserAlias(_T("서명"));
	m_marcEditor.AddUserAlias(_T("저자"));
	m_marcEditor.AddUserAlias(_T("발행지"));
	m_marcEditor.AddUserAlias(_T("발행자"));
	m_marcEditor.AddUserAlias(_T("발행년"));	
	m_marcEditor.AddUserAlias(_T("면장수"));
	m_marcEditor.AddUserAlias(_T("물리적특성"));
	m_marcEditor.AddUserAlias(_T("딸림자료"));
	m_marcEditor.AddUserAlias(_T("크기"));
	m_marcEditor.AddUserAlias(_T("총서명"));
	m_marcEditor.AddUserAlias(_T("총서편차"));
	m_marcEditor.AddUserAlias(_T("가격"));
	return 0;
}

VOID CBO_Species_Detail_Viewer::ShowNextPrevButton(INT idx)
{
	CButton * pBtn1 = (CButton * )GetDlgItem(IDC_btnDVIEWER_PREV);
	CButton * pBtn2 = (CButton * )GetDlgItem(IDC_btnDVIEWER_NEXT);
	pBtn1->EnableWindow(TRUE);
	pBtn2->EnableWindow(TRUE);
	
	if((m_nFirstIdx >= idx || idx < 0) && pBtn1)
		pBtn1->EnableWindow(FALSE);
	if((m_nLastIdx <= idx || idx < 0) && pBtn2)
		pBtn2->EnableWindow(FALSE);
}

INT CBO_Species_Detail_Viewer::SetDetailInfo()
{
	//색인/종/책 정보 설정
	CString strQuery, strSpeciesKey;
	strSpeciesKey = m_pParentDM->GetCellData(m_strSpeciesKeyAlias, m_nIdx);
	strQuery.Format(_T("REC_KEY=%s"), strSpeciesKey);
	m_pDM_INDEX->RefreshDataManager(strQuery);
	m_pDM_SPECIES->RefreshDataManager(strQuery);
	strQuery.Format(_T("SPECIES_KEY=%s ORDER BY VOL_SORT_NO"), strSpeciesKey);
	m_pDM_BOOK->RefreshDataManager(strQuery);
	m_pDM_VOLUME->FreeData();
	m_pSpeciesApi->CONVERT_BOOK_to_VOLUME(m_pDM_BOOK, m_pDM_VOLUME);
	SetMarcInfoAndCtrl();
	return 0;
}

VOID CBO_Species_Detail_Viewer::OnbtnDVIEWERPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CBO_Species_Detail_Viewer::OnbtnDVIEWERNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CBO_Species_Detail_Viewer::OnbtnDVIEWERMARCVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strMarc = m_pDM_SPECIES->GetCellData(_T("BS_MARC"), 0);
	if(strMarc.IsEmpty())
	{
		ESLAfxMessageBox(_T("마크 스트림이 올바르지 않습니다."));
		return ;
	}
	CBO_Species_Detail_Marc_Viewer viewer(this);
	viewer.SetMarcStream(strMarc);
	viewer.DoModal();
}

VOID CBO_Species_Detail_Viewer::OnbtnDVIEWERAPPENDIXVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strSpeciesKey = m_pDM_SPECIES->GetCellData(_T("BS_종KEY"), 0);
	CBO_Species_Detail_Appendix_Viewer viewer(this);
	viewer.SetSpeciesKey(strSpeciesKey);
	viewer.DoModal();
}

VOID CBO_Species_Detail_Viewer::SetWorkingStatusDesc()
{
	CString strWorkStatus = m_pDM_INDEX->GetCellData(_T("IBS_작업상태INFO"), 0);
	strWorkStatus = strWorkStatus.Left(3);
	if(strWorkStatus == _T("BOA")) strWorkStatus = _T("수서자료");
	else if(strWorkStatus == _T("BOR")) strWorkStatus = _T("등록자료");
	else if(strWorkStatus == _T("BOC")) strWorkStatus = _T("정리자료");
	else if(strWorkStatus == _T("BOL")) strWorkStatus = _T("소장자료");
	else strWorkStatus = _T("수서자료");

	m_pCM->SetControlMgrData(_T("IBS_자료상태INFO"), strWorkStatus);
}

VOID CBO_Species_Detail_Viewer::InitFirstLastIndex()
{
	INT ids = m_pParentGrid->SelectMakeList();
	ids = m_pParentGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pParentDM->GetRowCount();
		m_nFirstIdx = 0;
		m_nLastIdx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_nLastIdx = m_pParentGrid->SelectGetTailPosition();
		m_nFirstIdx = m_pParentGrid->SelectGetHeadPosition();
	}
}

INT CBO_Species_Detail_Viewer::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == 2000 || m_nOpenMode == -2000)
	{
		nFirstSelIdx = m_pParentGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == 1000 || m_nOpenMode == -1000)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_nFirstIdx;
		else
			nFirstSelIdx = m_pParentGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CBO_Species_Detail_Viewer::GetPrevSelectIndex()
{
	//===============================
	//2008.07.09 UPDATE BY PJW : 비교를 위해서 초기값을 0으로 셋팅한다.
// 	INT nPrevSelIndex = -1;
 	INT nPrevSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : INDEX꼬임현상으로 nPrevSelIndex가 0보다작으면 마지막 선정 INDEX를 넣는다.
		while( 1 )
		{
			if(nPrevSelIndex == m_nIdx || nPrevSelIndex < 0 )
			{
				break;
			}
			else if( nPrevSelIndex < m_nIdx )
			{
				nPrevSelIndex = m_pParentGrid->SelectGetNext();
				if( nPrevSelIndex < 0 )
				{
					nPrevSelIndex = m_pParentGrid->SelectGetTailPosition();
					break;
				}
			}
			else if( nPrevSelIndex > m_nIdx )
			{
				nPrevSelIndex = m_pParentGrid->SelectGetPrev();
			}
		}
		//================================================
		nPrevSelIndex = m_pParentGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pParentGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nFirstIdx)
			nPrevSelIndex = m_nIdx - 1;
	}
	return nPrevSelIndex;
}

INT CBO_Species_Detail_Viewer::GetNextSelectIndex()
{
	//===============================
	//2008.07.09 UPDATE BY PJW : 비교를 위해서 초기값을 0으로 셋팅한다.
// 	INT nNextSelIndex = -1;
 	INT nNextSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2008.07.08 ADD BY PJW : 여러개를 선정후 이전,다음 할경우 인덱스가 맞지 않음
		while( 1 )
		{
			if(nNextSelIndex == m_nIdx || nNextSelIndex < 0 )
			{
				break;
			}
			else if( nNextSelIndex < m_nIdx )
			{
				nNextSelIndex = m_pParentGrid->SelectGetNext();
			}
			else if( nNextSelIndex > m_nIdx )
			{
				nNextSelIndex = m_pParentGrid->SelectGetPrev();
			}
		}
		//================================================

		nNextSelIndex = m_pParentGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pParentGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nIdx != m_nLastIdx)
			nNextSelIndex = m_nIdx + 1;
	}
	return nNextSelIndex;
}

VOID CBO_Species_Detail_Viewer::OnbtnEBOOKVIEW() 
{
	// TODO: Add your control notification handler code here
	CEBookViewMgr mgr;
	INT nRow = m_pGrid_Volume->GetRow()-1;
	CString strMediaCode = m_pDM_VOLUME->GetCellData(_T("BB_매체구분_코드"), nRow);
	if(strMediaCode != _T("EB"))
	{
		ESLAfxMessageBox(_T("선정하신 책은 전자책이 아닙니다."));
		return;
	}
	CString strBookKey = m_pDM_VOLUME->GetCellData(_T("BB_책KEY"), nRow);
	mgr.EBookViewByBookKey(strBookKey, m_pDM_BOOK->CONNECTION_INFO);
}

HBRUSH CBO_Species_Detail_Viewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
