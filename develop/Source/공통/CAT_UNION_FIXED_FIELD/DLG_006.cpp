// DLG_006.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_006.h"
#include "FixedFieldInfo.h"

#include "DLG_006_BK.h"
#include "DLG_006_CR.h"
#include "DLG_006_ER.h"
#include "DLG_006_MP.h"
#include "DLG_006_MU.h"
#include "DLG_006_MX.h"
#include "DLG_006_RB.h"
#include "DLG_006_VM.h"

// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// 통합마크 개정 적용
/*//NEW------------------------------------------------------------------------*/
#include "DLG_006_BK_1.h"
#include "DLG_006_MU_1.h"
#include "DLG_006_CR_1.h"
#include "DLG_006_VM_1.h"
#include "DLG_006_MX_1.h"
#include "DLG_006_ER_1.h"
#include "DLG_006_MP_1.h"
#include "DLG_006_RB_1.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_006 dialog


CDLG_006::CDLG_006(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CESL_Mgr(IDD, pParent), m_MultiComboBox(NULL)
{
	//{{AFX_DATA_INIT(CDLG_006)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
	m_lpszInfoFileName = _T("..\\CFG\\UNION_008TAG코드.txt");

	m_p006Form = NULL;
}

CDLG_006::~CDLG_006()
{
}

void CDLG_006::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_006)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDLG_006, CDialog)
	//{{AFX_MSG_MAP(CDLG_006)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_cMAT_CATEGORY, OnSelchangecMATCATEGORY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_006 message handlers
VOID CDLG_006::FreeData()
{
	CFixedFieldInfo *pInfo = NULL;

	// 006 지우기
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
		delete pInfo;
	}
	m_listInfo.RemoveAll();
}

INT CDLG_006::Init()
{
	INT ids = -1;

	FreeData();
	ids = LoadInfo(m_lpszInfoFileName);
	if (ids < 0) return -1;	
	
	m_mapMatCatCode_MatCatDesc.SetAt(_T("a"), _T("문자자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("c"), _T("필사악보 이외의 악보"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("d"), _T("필사악보"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("e"), _T("지도자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("f"), _T("필사지도"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("g"), _T("평면영사자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("i"), _T("녹음자료(음악 이외)"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("j"), _T("녹음자료(음악)"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("k"), _T("평면영사자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("m"), _T("전자자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("o"), _T("키트"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("p"), _T("복합자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("r"), _T("입체자료(실물)"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("s"), _T("연속간행물/갱신자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("t"), _T("필사문자자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("y"), _T("고서"));

	// ComboBox Init
	ids  = InitMultiComboBox();
	if (ids < 0) return ids;

	CreateScreen();

	return 0;
}

INT CDLG_006::InitMultiComboBox()
{
	// Map 에 정의한 Code와 설명을 보여준다.
	m_MultiComboBox.FreeComboData();
	m_MultiComboBox.PrevRow = -1;
	m_MultiComboBox.PrevCol = -1;
	m_MultiComboBox.m_nMaxRowCnt = 0;

	m_MultiComboBox.ResetContent();
	m_MultiComboBox.InitMultiComboBox(2);

	// 2008.06.26 UPDATE BY PDJ : 순서 불일치로 직접 넣는다.
	m_MultiComboBox.AddItem(_T("a"), 0, 0); m_MultiComboBox.AddItem(_T("문자자료")				, 1, 0);
	m_MultiComboBox.AddItem(_T("c"), 0, 1); m_MultiComboBox.AddItem(_T("필사악보 이외의 악보")	, 1, 1);
	m_MultiComboBox.AddItem(_T("d"), 0, 2); m_MultiComboBox.AddItem(_T("필사악보")				, 1, 2);
	m_MultiComboBox.AddItem(_T("e"), 0, 3); m_MultiComboBox.AddItem(_T("지도자료")				, 1, 3);
	m_MultiComboBox.AddItem(_T("f"), 0, 4); m_MultiComboBox.AddItem(_T("필사지도")				, 1, 4);
	m_MultiComboBox.AddItem(_T("g"), 0, 5); m_MultiComboBox.AddItem(_T("평면영사자료")			, 1, 5);
	m_MultiComboBox.AddItem(_T("i"), 0, 6); m_MultiComboBox.AddItem(_T("녹음자료(음악 이외)")	, 1, 6);
	m_MultiComboBox.AddItem(_T("j"), 0, 7); m_MultiComboBox.AddItem(_T("녹음자료(음악)")		, 1, 7);
	m_MultiComboBox.AddItem(_T("k"), 0, 8); m_MultiComboBox.AddItem(_T("평면비영사자료")		, 1, 8);
	m_MultiComboBox.AddItem(_T("m"), 0, 9); m_MultiComboBox.AddItem(_T("전자자료")				, 1, 9);
	m_MultiComboBox.AddItem(_T("o"), 0,10); m_MultiComboBox.AddItem(_T("키트")					, 1,10);
	m_MultiComboBox.AddItem(_T("p"), 0,11); m_MultiComboBox.AddItem(_T("복합자료")				, 1,11);
	m_MultiComboBox.AddItem(_T("r"), 0,12); m_MultiComboBox.AddItem(_T("입체자료(실물)")		, 1,12);
	m_MultiComboBox.AddItem(_T("s"), 0,13); m_MultiComboBox.AddItem(_T("연속간행물/갱신자료")	, 1,13);
	m_MultiComboBox.AddItem(_T("t"), 0,14); m_MultiComboBox.AddItem(_T("필사문자자료")			, 1,14);
	m_MultiComboBox.AddItem(_T("y"), 0,15); m_MultiComboBox.AddItem(_T("고서")					, 1,15);
	 
	/* 0, 0);
	INT idx = 0;
	CString strCode;
	CString strDesc;
	POSITION pos = m_mapMatCatCode_MatCatDesc.GetStartPosition();	
	while (pos)
	{
		m_mapMatCatCode_MatCatDesc.GetNextAssoc(pos, strCode, strDesc);
		m_MultiComboBox.AddItem(strCode, 0, idx);
		m_MultiComboBox.AddItem(strDesc, 1, idx);	

		idx++;
	}
	*/
	
	// Set Sel한다.
	CString strCat;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("006@0"), strCat);
	strCat.TrimLeft();
	strCat.TrimRight();
	if (strCat.IsEmpty())
		AfxMessageBox(_T("일치하는 자료범주가 없습니다.") );

	m_MultiComboBox.SelectString(strCat);
	return 0;
}

INT CDLG_006::LoadInfo(LPCTSTR lpszFileName)
{
	CStdioFile file;
	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
	if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary)) return -1;
*///AFTER------------------------------------------------------------------------
	if (_T("..\\CFG\\UNION_008TAG코드.txt") == lpszFileName) {
		CString strFileName = lpszFileName;
		CString strGetGroupName = _T("");
		CString strGroupFileName = _T("..\\cfg\\MarcRelation\\통합마크_개정_그룹명.cfg");
		CFileFind find;
		if (TRUE == find.FindFile(strGroupFileName)) {
			CStdioFile file;
			if (TRUE == file.Open(strGroupFileName, CFile::modeRead|CFile::typeBinary)) {
				TCHAR cUni;
				file.Read(&cUni, sizeof(TCHAR));
				if (0xFEFF != cUni) file.SeekToBegin();
				CString strTempData, strGetData;
				while (file.ReadString(strGetData)) {
					strGetData.TrimLeft();
					if (strGetData.IsEmpty()) continue;
					if (_T("#") == strGetData.Left(1)) continue;
					INT nPos = strGetData.Find(_T("="));
					if (0 <= nPos) {
						strTempData = strGetData.Left(nPos);
						strTempData.TrimLeft(); strTempData.TrimRight();
						if (_T("그룹명") == strTempData) {
							strGetGroupName = strGetData.Mid(nPos+1, strGetData.GetLength());
							strGetGroupName.TrimLeft(); strGetGroupName.TrimRight();
							break;
						}
					}
				}
				file.Close ();
			}
			find.Close();
		}
		if (!strGetGroupName.IsEmpty()) {
			strFileName = _T("..\\CFG\\UNION_008TAG코드_") + strGetGroupName +_T(".txt");
		}
		if (!file.Open(strFileName, CFile::modeRead | CFile::typeBinary)) return -1;
	} else {
		if (!file.Open(lpszFileName, CFile::modeRead | CFile::typeBinary)) return -1;
	}
/*//END------------------------------------------------------------------------*/
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT nFind = -1;
	CString strLine;
	CString strGroupName;
	CString strTmp;
	DWORD dwPos = 0;
	CFixedFieldInfo *pInfo = NULL;
	while (file.ReadString(strLine))
	{		
		strLine.TrimRight();

		if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;

		// Group
		if (strLine.Left(1) == _T("["))
		{
			// Position 저장
			dwPos = file.GetPosition();

			strGroupName = strLine;
			// Item
			while (file.ReadString(strLine))
			{				
				strLine.TrimRight();
				if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
				if (strLine.Left(1) == _T("[")) 
				{
					// Position 돌려놓기
					file.Seek(dwPos, CFile::begin);
					break;
				}
				if (strLine.Left(1) == _T("("))
				{
					// Position 저장
					dwPos = file.GetPosition();

					// Item 추가
					pInfo = new CFixedFieldInfo(strGroupName, strLine);
					m_listInfo.AddTail(pInfo);

					// Code, Desc 넣기
					while (file.ReadString(strLine))
					{						
						strLine.TrimRight();
						if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
						if (strLine.Left(1) == _T("(") || strLine.Left(1) == _T("[")) 
						{
							// Position 돌려놓기
							file.Seek(dwPos, CFile::begin);
							break;
						}

						nFind = strLine.Find(_T(":"));
						if (nFind < 0) continue;

						strTmp = strLine.Mid(0, nFind);
						strTmp.Replace(_T("~"), _T("|"));


						pInfo->AddItem(strTmp, strLine.Mid(nFind + 1));
					}
				}
			}
		}		
	}

	file.Close();
	return 0;
}

INT CDLG_006::CreateScreen()
{
	delete m_p006Form;
	m_p006Form = NULL;

	m_MultiComboBox.GetData(m_strSubGroupName);

	CRect rcWnd;
	GetClientRect(rcWnd);
	rcWnd.DeflateRect(5, 50);

	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
/*//NEW------------------------------------------------------------------------*/
	CString strFileName = _T("..\\cfg\\MarcRelation\\통합마크_개정_그룹명.cfg");
	CFileFind find;
	BOOL bRevision = find.FindFile(strFileName);
	find.Close();
/*//END------------------------------------------------------------------------*/
	
	if (m_strSubGroupName == _T("a"))
	{
		m_strGroupName = _T("[BK]");	
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_BK(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_BK_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_BK(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}	
	else if (m_strSubGroupName == _T("c"))
	{
		m_strGroupName = _T("[MU]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MU(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MU_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MU(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("d"))
	{
		m_strGroupName = _T("[MU]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MU(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MU_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MU(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("e"))
	{
		m_strGroupName = _T("[MP]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MP(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MP_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MP(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("f"))
	{
		m_strGroupName = _T("[MP]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MP(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MP_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MP(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("g"))
	{
		m_strGroupName = _T("[VM]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_VM(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_VM_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_VM(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("i"))
	{
		m_strGroupName = _T("[MU]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MU(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MU_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MU(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("j"))
	{
		m_strGroupName = _T("[MU]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MU(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MU_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MU(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("k"))
	{
		m_strGroupName = _T("[VM]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_VM(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_VM_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_VM(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("m"))
	{
		m_strGroupName = _T("[ER]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_ER(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_ER_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_ER(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("o"))
	{
		m_strGroupName = _T("[VM]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_VM(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_VM_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_VM(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("p"))
	{
		m_strGroupName = _T("[MX]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_MX(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_MX_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_MX(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("r"))
	{
		m_strGroupName = _T("[VM]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_VM(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_VM_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_VM(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("s"))
	{
		m_strGroupName = _T("[CR]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_CR(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_CR_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_CR(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("t"))
	{
		m_strGroupName = _T("[BK]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_BK(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_BK_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_BK(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}
	else if (m_strSubGroupName == _T("y"))
	{
		m_strGroupName = _T("[RB]");
		// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
		m_p006Form = new CDLG_006_RB(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
		if (TRUE == bRevision) {
			m_p006Form = new CDLG_006_RB_1(this, m_pMarc);
		} else {
			m_p006Form = new CDLG_006_RB(this, m_pMarc);
		}
/*//END------------------------------------------------------------------------*/
	}	
	
	if (!m_p006Form) return -1;		
	m_p006Form->m_pListInfo = &m_listInfo;
	m_p006Form->m_strGroupName = m_strGroupName;
	if (!m_p006Form->Create(this)) return -1;

	m_p006Form->MoveWindow(rcWnd);
	m_p006Form->ShowWindow(SW_SHOW);
	m_p006Form->SetFocus();	
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_006 message handlers

BOOL CDLG_006::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_pMarc)
	{
		m_strLastErrMsg = _T("MARC Pointer가 NULL입니다.");
		goto ERR;
	}

	// Multi ComboBox Subclassing
	if (!m_MultiComboBox.SubclassDlgItem(IDC_cMAT_CATEGORY, this))
		goto ERR;

	if (Init() < 0) goto ERR;
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;
}

VOID CDLG_006::OnSelchangecMATCATEGORY() 
{
	CreateScreen();
}

VOID CDLG_006::OnOK() 
{	
	INT ids = -1;
	
	if (m_p006Form)
	{
		ids = m_p006Form->Get006();
		if (ids < 0) return;
	}
		
	CDialog::OnOK();
}

VOID CDLG_006::OnDestroy() 
{
	CDialog::OnDestroy();

	FreeData();
	
	delete m_p006Form;
	m_p006Form = NULL;
}

HBRUSH CDLG_006::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
