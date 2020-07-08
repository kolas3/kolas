// DLG_FIXED_007.cpp : implementation file
//

#include "stdafx.h"
#include "DLG_FIXED_007.h"
#include "FixedFieldInfo.h"

#include "DLG_FIXED_007_A.h"
#include "DLG_FIXED_007_C.h"
#include "DLG_FIXED_007_D.h"
#include "DLG_FIXED_007_F.h"
#include "DLG_FIXED_007_G.h"
#include "DLG_FIXED_007_H.h"
#include "DLG_FIXED_007_K.h"
#include "DLG_FIXED_007_M.h"
#include "DLG_FIXED_007_O.h"
#include "DLG_FIXED_007_Q.h"
#include "DLG_FIXED_007_R.h"
#include "DLG_FIXED_007_S.h"
#include "DLG_FIXED_007_T.h"
#include "DLG_FIXED_007_V.h"
#include "DLG_FIXED_007_Y.h"
#include "DLG_FIXED_007_Z.h"

// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// 통합마크 개정 적용
/*//NEW------------------------------------------------------------------------*/
#include "DLG_FIXED_007_F_1.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007 dialog


CDLG_FIXED_007::CDLG_FIXED_007(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CESL_Mgr(IDD, pParent), m_MultiComboBox(NULL)
{
	//{{AFX_DATA_INIT(CDLG_FIXED_007)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
	m_lpszInfoFileName = _T("..\\CFG\\UNION_007TAG코드.txt");

	m_p007Form = NULL;
}
CDLG_FIXED_007::~CDLG_FIXED_007()
{
}

void CDLG_FIXED_007::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLG_FIXED_007)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007 message handlers
VOID CDLG_FIXED_007::FreeData()
{
	CFixedFieldInfo *pInfo = NULL;

	// 007 지우기
	POSITION pos = m_listInfo.GetHeadPosition();
	while (pos)
	{
		pInfo = static_cast<CFixedFieldInfo*>(m_listInfo.GetNext(pos));
		delete pInfo;
	}
	m_listInfo.RemoveAll();
}


BEGIN_MESSAGE_MAP(CDLG_FIXED_007, CDialog)
	//{{AFX_MSG_MAP(CDLG_FIXED_007)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_cMAT_CATEGORY, OnSelchangecMATCATEGORY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007 message handlers

INT CDLG_FIXED_007::Init()
{
	INT ids = -1;

	FreeData();
	ids = LoadInfo();
	if (ids < 0) return -1;

	m_strGroupName = _T("[007]");
	m_mapMatCatCode_MatCatDesc.SetAt(_T("a"), _T("지도자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("c"), _T("전자자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("d"), _T("구체"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("f"), _T("촉감자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("g"), _T("평면영사자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("h"), _T("마이크로자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("k"), _T("평면비영사자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("m"), _T("영화"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("o"), _T("고서"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("r"), _T("원격탐사도"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("q"), _T("악보"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("s"), _T("녹음자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("t"), _T("문자자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("v"), _T("비디오녹화자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("y"), _T("키트"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("z"), _T("형태를 구분 할 수 없는 자료"));

	// ComboBox Init
	ids  = InitMultiComboBox();
	if (ids < 0) return ids;

	CreateScreen();

	return 0;
}

INT CDLG_FIXED_007::InitMultiComboBox()
{
	// Map 에 정의한 Code와 설명을 보여준다.
	m_MultiComboBox.FreeComboData();
	m_MultiComboBox.PrevRow = -1;
	m_MultiComboBox.PrevCol = -1;
	m_MultiComboBox.m_nMaxRowCnt = 0;

	m_MultiComboBox.ResetContent();
	m_MultiComboBox.InitMultiComboBox(2);
	
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
	
	// Set Sel한다.
	CString strCat;
	m_pInfo->pMarcMgr->GetItem(m_pMarc, _T("007@0"), strCat);
	strCat.TrimLeft();
	strCat.TrimRight();
	if (strCat.IsEmpty())
		AfxMessageBox(_T("일치하는 자료범주가 없습니다.") );

	m_MultiComboBox.SelectString(strCat);
	return 0;
}

INT CDLG_FIXED_007::LoadInfo()
{
	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
/*//NEW------------------------------------------------------------------------*/
	CString strGetGroupName = _T("");
	CString strFileName = _T("..\\cfg\\MarcRelation\\통합마크_개정_그룹명.cfg");
	CFileFind find;
	if (TRUE == find.FindFile(strFileName)) {
		CStdioFile file;
		if (TRUE == file.Open(strFileName, CFile::modeRead|CFile::typeBinary)) {
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
		strFileName = _T("..\\CFG\\UNION_007TAG코드_") + strGetGroupName +_T(".txt");
		m_lpszInfoFileName = strFileName.GetBuffer(0);
	}
/*//END------------------------------------------------------------------------*/

	CStdioFile file;
	if (!file.Open(m_lpszInfoFileName, CFile::modeRead | CFile::typeBinary)) return -1;
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT nFind = -1;
	CString strLine;
	CString strGroupName;
	CString strSubGroupName;
	DWORD dwPos = 0;
	CFixedFieldInfo *pInfo = NULL;
	while (file.ReadString(strLine))
	{
		//strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;

		// Group
		if (strLine.Left(1) == _T("["))
		{
			// Position 저장
			dwPos = file.GetPosition();

			strGroupName = strLine;
			
			// SubGroup
			while (file.ReadString(strLine))
			{
				//strLine.TrimLeft();
				strLine.TrimRight();
				if (strLine.IsEmpty() || strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
				if (strLine.Left(1) == _T("[")) 
				{
					// Position 돌려놓기
					file.Seek(dwPos, CFile::begin);
					break;
				}
				
				if (strLine.Left(1) == _T("<"))
				{
					// Position 저장
					dwPos = file.GetPosition();
					
					strSubGroupName = strLine;
					
					// Item
					while (file.ReadString(strLine))
					{
						//strLine.TrimLeft();
						strLine.TrimRight();
						if (strLine.IsEmpty() || strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
						if (strLine.Left(1) == _T("[") || strLine.Left(1) == _T("<")) 
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
							pInfo = new CFixedFieldInfo(strGroupName, strSubGroupName, strLine);
							m_listInfo.AddTail(pInfo);
							
							// Code, Desc 넣기
							while (file.ReadString(strLine))
							{
								//strLine.TrimLeft();
								strLine.TrimRight();
								if (strLine.IsEmpty() || strLine.Left(1) == _T(".") || strLine.Left(1) == _T("#")) continue;
								if (strLine.Left(1) == _T("(") || strLine.Left(1) == _T("[")) 
								{
									// Position 돌려놓기
									file.Seek(dwPos, CFile::begin);
									break;
								}
								
								nFind = strLine.Find(_T(":"));
								if (nFind < 0) continue;
								
								pInfo->AddItem(strLine.Mid(0, nFind), strLine.Mid(nFind + 1));
							}
						}
					}
				}
			}
		}		
	}

	file.Close();

	return 0;
}

INT CDLG_FIXED_007::CreateScreen()
{
	delete m_p007Form;
	m_p007Form = NULL;

	m_MultiComboBox.GetData(m_strSubGroupName);

	CRect rcWnd;
	GetClientRect(rcWnd);
	rcWnd.DeflateRect(5, 50);

	if (m_strSubGroupName == _T("a"))
		m_p007Form = new CDLG_FIXED_007_A(this, m_pMarc);
	else if (m_strSubGroupName == _T("c"))
		m_p007Form = new CDLG_FIXED_007_C(this, m_pMarc);
	else if (m_strSubGroupName == _T("d"))
		m_p007Form = new CDLG_FIXED_007_D(this, m_pMarc);
	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
/*//BEFORE-----------------------------------------------------------------------
	else if (m_strSubGroupName == _T("f"))
		m_p007Form = new CDLG_FIXED_007_F(this, m_pMarc);
*///AFTER------------------------------------------------------------------------
	else if (m_strSubGroupName == _T("f")) {
		CString strGetGroupName = _T("");
		CString strFileName = _T("..\\cfg\\MarcRelation\\통합마크_개정_그룹명.cfg");
		CFileFind find;
		if (TRUE == find.FindFile(strFileName)) {
			m_p007Form = new CDLG_FIXED_007_F_1(this, m_pMarc);
		} else {
			m_p007Form = new CDLG_FIXED_007_F(this, m_pMarc);
		}
		find.Close();
	}
/*//END------------------------------------------------------------------------*/
	else if (m_strSubGroupName == _T("g"))
		m_p007Form = new CDLG_FIXED_007_G(this, m_pMarc);
	else if (m_strSubGroupName == _T("h"))
		m_p007Form = new CDLG_FIXED_007_H(this, m_pMarc);
	else if (m_strSubGroupName == _T("k"))
		m_p007Form = new CDLG_FIXED_007_K(this, m_pMarc);
	else if (m_strSubGroupName == _T("m"))
		m_p007Form = new CDLG_FIXED_007_M(this, m_pMarc);
	else if (m_strSubGroupName == _T("o"))
		m_p007Form = new CDLG_FIXED_007_O(this, m_pMarc);
	else if (m_strSubGroupName == _T("r"))
		m_p007Form = new CDLG_FIXED_007_R(this, m_pMarc);
	else if (m_strSubGroupName == _T("q"))
		m_p007Form = new CDLG_FIXED_007_Q(this, m_pMarc);
	else if (m_strSubGroupName == _T("s"))
		m_p007Form = new CDLG_FIXED_007_S(this, m_pMarc);
	else if (m_strSubGroupName == _T("t"))
		m_p007Form = new CDLG_FIXED_007_T(this, m_pMarc);
	else if (m_strSubGroupName == _T("v"))
		m_p007Form = new CDLG_FIXED_007_V(this, m_pMarc);
	else if (m_strSubGroupName == _T("y"))
		m_p007Form = new CDLG_FIXED_007_Y(this, m_pMarc);
	else if (m_strSubGroupName == _T("z"))
		m_p007Form = new CDLG_FIXED_007_Z(this, m_pMarc);
	
	if (!m_p007Form) return -1;
	m_p007Form->m_pListInfo = &m_listInfo;
	m_p007Form->m_strGroupName = m_strGroupName;
	if (!m_p007Form->Create(this)) return -1;

	m_p007Form->MoveWindow(rcWnd);
	m_p007Form->ShowWindow(SW_SHOW);
	m_p007Form->SetFocus();
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CDLG_FIXED_007 message handlers

BOOL CDLG_FIXED_007::OnInitDialog() 
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

VOID CDLG_FIXED_007::OnSelchangecMATCATEGORY() 
{
	CreateScreen();
}

VOID CDLG_FIXED_007::OnOK() 
{
	INT ids = -1;
	
	if (m_p007Form)
	{
		ids = m_p007Form->Get007();
		if (ids < 0) return;
	}
	
	CDialog::OnOK();
}

VOID CDLG_FIXED_007::OnDestroy() 
{
	CDialog::OnDestroy();

	FreeData();
	
	delete m_p007Form;
	m_p007Form = NULL;
}

HBRUSH CDLG_FIXED_007::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
