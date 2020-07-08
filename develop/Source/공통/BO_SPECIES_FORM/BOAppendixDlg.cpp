// BOAppendixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOAppendixDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg dialog


CBOAppendixDlg::CBOAppendixDlg(CESL_Mgr* pParent /*= NULL*/, CESL_DataMgr *pDM_Book /*= NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOAppendixDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM_Book = pDM_Book;
	m_pCM = NULL;
	m_pDM_Appendix = NULL;
	m_pFormApi = NULL;

	m_nSelectedRow = -1;

	m_bReadOnly = FALSE;
}

BOOL CBOAppendixDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOAppendixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOAppendixDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOAppendixDlg, CDialog)
	//{{AFX_MSG_MAP(CBOAppendixDlg)
	ON_BN_CLICKED(IDC_bINSERT_APPENDIX, OnbINSERTAPPENDIX)
	ON_BN_CLICKED(IDC_bSAVE_APPENDIX, OnbSAVEAPPENDIX)
	ON_BN_CLICKED(IDC_bDELETE_APPENDIX, OnbDELETEAPPENDIX)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg Operations
INT CBOAppendixDlg::Init()
{
	if (m_pDM_Book == NULL || m_pFormApi == NULL) return -1;
	
	// 부록 정보 만든다.
	m_pDM_Appendix = FindDM(_T("DM_단행_부록_부록정보"));
	if (m_pDM_Appendix == NULL) return -1;

	m_pCM = FindCM(_T("CM_단행_부록_부록정보"));
	if (m_pCM == NULL) return -1;

	CString strKey;
	
	// 책단위일때
	if (m_nMode == 1)
		strKey = m_pFormApi->GetBookKey(m_pDM_Book, m_nBookIdx);
	
	// 부록정보 만들기
	INT ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol, strKey);
	if (ids < 0) return -1;
	
	return 0;
}

VOID CBOAppendixDlg::SetAppendixMode(INT nMode, CString strVol, INT nBookIdx /*= -1*/) 
{
	m_nMode = nMode; 
	m_strVol = strVol;
	m_nBookIdx = nBookIdx;
}

INT CBOAppendixDlg::Display()
{
	if (m_nSelectedRow < 0) return -1;

	CESL_Control_Element * pCE = NULL;

	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Display(m_nSelectedRow);
	}

	// 편권차 보여주기
	m_pCM->SetControlMgrData(_T("BB_편권차"), m_strVol);
	
	// 부록이 없을 경우 저장버튼, 삭제버튼 Disable
	if (m_pDM_Appendix->GetRowCount() < 1)
	{
		GetDlgItem(IDC_bSAVE_APPENDIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDELETE_APPENDIX)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_bSAVE_APPENDIX)->EnableWindow(TRUE);
		GetDlgItem(IDC_bDELETE_APPENDIX)->EnableWindow(TRUE);
	}

	if (m_bReadOnly)
		DisplayControlReadOnly();
	
	return 0;
}

INT CBOAppendixDlg::DisplayControlReadOnly()
{
	m_pCM->AllControlReadOnly();

	INT nButtonCount = 0;
	INT nArrButtonCtrl[16];
	
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT_APPENDIX;
	nArrButtonCtrl[nButtonCount++] = IDC_bSAVE_APPENDIX;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_bDELETE_APPENDIX;
	
	CWnd *pWnd = NULL;

	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;

		pWnd->EnableWindow(FALSE);
	}

	return 0;
}

INT CBOAppendixDlg::InsertAppendix(CString strParentKey)
{
	if (strParentKey.IsEmpty()) return -1;

	// 화면정보 추가
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book);
	INT nRowCount = m_pDM_Book->GetRowCount();
	
	// 부모키 설정
	m_pDM_Book->SetCellData(_T("BB_부모책KEY"), strParentKey, nRowCount - 1);
	
	// 부록 플래그 추가
	m_pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("A"), nRowCount - 1);

	// 편권차 넣기
	m_pDM_Book->SetCellData(_T("BB_편권차"), m_strVol, nRowCount - 1);
	
	// 쿼리 플래그 세팅
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, nRowCount - 1);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg message handlers

BOOL CBOAppendixDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr(_T("SM_단행_부록"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	ids = Init();
	if (ids < 0)
	{
		AfxMessageBox(_T("Init Data Manger Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_nSelectedRow = 0;
	AllControlDisplay(_T("CM_단행_부록_부록정보"), 0);

	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid != NULL)
		pGrid->SetReverse(m_nSelectedRow);
	
	Display();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOAppendixDlg::OnbINSERTAPPENDIX() 
{
	INT ids = -1;

	// 거부리스트 만들기 (책정보 수정안하게..)
	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_대표책여부"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_복본기호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));

	// 권인지 책인지 확인  권이면 책 개수 만큼 부록 추가한다.
	// 권단위
	if (m_nMode == 0)
	{
		CArray<CString, CString&> arrReckey;
		INT nRowCount = m_pDM_Book->GetRowCount();
		for (INT idx = 0; idx < nRowCount; idx++)
		{
			// 책이 아니면 넘어간다.
			if (!m_pFormApi->IsBook(m_pDM_Book, idx, m_strVol)) continue;
			
			// 부모의 rec_key가 있는지 체크한후 키 생성할지 결정
			CString strKey = m_pDM_Book->GetCellData(_T("BB_책KEY"), idx);
			if (strKey.IsEmpty())
			{
				strKey = m_pDM_Book->GetCellData(_T("BB_UNIQUE_KEY"), idx);
				if (strKey.IsEmpty())
				{
					// 키 새성
					strKey.Format(_T("U%d"), GetTickCount());
					m_pDM_Book->SetCellData(_T("BB_UNIQUE_KEY"), strKey, idx);
				}
			}

			arrReckey.Add(strKey);

			Sleep(20);
		}

		// 부록을 추가한다.
		nRowCount = arrReckey.GetSize();
		for (idx = 0; idx < nRowCount; idx++)
		{
			InsertAppendix(arrReckey.GetAt(idx));
		}

		// 부록정보 DM다시 만들기
		m_pFormApi->InitDefaultIgnoreAlias();
		ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol);
		if (ids < 0) return;
	}
	// 책단위
	else if (m_nMode == 1)
	{
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_배가위치부호"));
		m_pFormApi->AddDefaultIgnoreAlias(_T("BB_별치기호_코드"));
		// 부모의 rec_key가 있는지 체크한후 키 생성할지 결정
		CString strKey = m_pFormApi->GetBookKey(m_pDM_Book, m_nBookIdx);
		if (strKey.IsEmpty())
		{
			// 키 새성
			strKey.Format(_T("U%d"), GetTickCount());
			m_pDM_Book->SetCellData(_T("BB_UNIQUE_KEY"), strKey, m_nBookIdx);
		}

		InsertAppendix(strKey);

		// 부록정보 DM다시 만들기
		m_pFormApi->InitDefaultIgnoreAlias();
		ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol, strKey);
		if (ids < 0) return;
	}
	
	// 마지막 정보화면에 뿌리기
	m_nSelectedRow = m_pDM_Appendix->GetRowCount() - 1;
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid != NULL)
	{
		pGrid->Display();
		pGrid->SetReverse(m_nSelectedRow);
	}
	
	Display();
}

VOID CBOAppendixDlg::OnbSAVEAPPENDIX() 
{
	if (m_nSelectedRow < 0) return;

	m_pFormApi->InitDefaultIgnoreAlias();

	// Appdedix row 가 가리키고 있는 Book Row 찾기
	INT idx = _ttoi(m_pDM_Appendix->GetCellData(_T("BB_BOOK_IDX"), m_nSelectedRow));
	if (idx < 0) return;

	INT ids = -1;

	// 화면정보 수정
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Book, idx);

	// 쿼리 플래그 세팅
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

	CString strKey;
	// 책단위
	if (m_nMode == 1)
	{
		strKey = m_pFormApi->GetBookKey(m_pDM_Book, m_nBookIdx);
	}

	// 부록정보 DM 다시 만들기
	ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol, strKey);
	if (ids < 0) return;

	// 현재 선택된 정보 화면에 다시 뿌리기
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid != NULL)
	{
		pGrid->Display();
		pGrid->SetReverse(m_nSelectedRow);
	}
}

VOID CBOAppendixDlg::OnbDELETEAPPENDIX() 
{
	if (m_nSelectedRow < 0) return;

	// Appdedix row 가 가리키고 있는 Book Row 찾기
	INT idx = _ttoi(m_pDM_Appendix->GetCellData(_T("BB_BOOK_IDX"), m_nSelectedRow));
	if (idx < 0) return;

	CString strVolTitle;
	m_pCM->GetControlMgrData(_T("BB_편제명부록명"), strVolTitle);
		
	CString strMsg;
	strMsg.Format(_T("부록명이 [%s]인 부록을 삭제하시겠습니까?"), strVolTitle);
	if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return;

	// 삭제
	m_pFormApi->DeleteBookDMRow(m_pDM_Book, idx);

	CString strKey;
	// 책단위
	if (m_nMode == 1)
	{
		strKey = m_pFormApi->GetBookKey(m_pDM_Book, m_nBookIdx);
	}

	// 부록정보 DM 다시 만들기
	INT ids = m_pFormApi->MakeAppendixDM(m_pDM_Book, m_pDM_Appendix, m_strVol, strKey);
	if (ids < 0) return;

	if (m_nSelectedRow > 0)
		m_nSelectedRow--;

	// 현재 선택된 정보 화면에 다시 뿌리기
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid != NULL)
	{
		pGrid->Display();
		pGrid->SetReverse(m_nSelectedRow);
	}

	Display();
}

BEGIN_EVENTSINK_MAP(CBOAppendixDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOAppendixDlg)
	ON_EVENT(CBOAppendixDlg, IDC_gBO_APPENDIX, -605 /* MouseDown */, OnMouseDowngBOAPPENDIX, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBOAppendixDlg::OnMouseDowngBOAPPENDIX(short Button, short Shift, long x, long y) 
{
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	if (pGrid == NULL) return;

	INT nRow = 	pGrid->GetRow();
	if (nRow == 0) return;

	pGrid->SetReverse(nRow - 1);

	m_nSelectedRow = nRow - 1;

	Display();
}

VOID CBOAppendixDlg::OnbCLEARSCREEN() 
{
	CESL_Control_Element * pCE = NULL;

	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Clear();
	}

	return;	
}