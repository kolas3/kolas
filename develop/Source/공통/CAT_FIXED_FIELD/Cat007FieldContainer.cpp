// Cat007FieldContainer.cpp : implementation file
//

#include "stdafx.h"

#include "Cat007FieldContainer.h"

#include "FixedFieldInfo.h"

#include "Cat007FixedField_A.h"
#include "Cat007FixedField_C.h"
#include "Cat007FixedField_D.h"
#include "Cat007FixedField_G.h"
#include "Cat007FixedField_H.h"
#include "Cat007FixedField_K.h"
#include "Cat007FixedField_M.h"
#include "Cat007FixedField_O.h"
#include "Cat007FixedField_S.h"
#include "Cat007FixedField_T.h"
#include "Cat007FixedField_V.h"
#include "Cat007FixedField_Z.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCat007FieldContainer dialog


CCat007FieldContainer::CCat007FieldContainer(CESL_Mgr* pParent /*=NULL*/, CMarc *pMarc /*=NULL*/)
	: CESL_Mgr(IDD, pParent), m_MultiComboBox(NULL)
{
	//{{AFX_DATA_INIT(CCat007FieldContainer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMarc = pMarc;
	m_lpszInfoFileName = _T("..\\CFG\\007TAG코드.txt");

	m_p007Form = NULL;
}

VOID CCat007FieldContainer::FreeData()
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

CCat007FieldContainer::~CCat007FieldContainer()
{
}

VOID CCat007FieldContainer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCat007FieldContainer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCat007FieldContainer, CDialog)
	//{{AFX_MSG_MAP(CCat007FieldContainer)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_cMAT_CATEGORY, OnSelchangecMATCATEGORY)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCat007FieldContainer message handlers

INT CCat007FieldContainer::Init()
{
	INT ids = -1;

	FreeData();
	ids = LoadInfo();
	if (ids < 0) return -1;

	m_mapMatCatCode_MatCatDesc.RemoveAll();
	m_mapMatCatCode_MatCatDesc.SetAt(_T("h"), _T("마이크로형태자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("t"), _T("인쇄자료"));
	m_mapMatCatCode_MatCatDesc.SetAt(_T("z"), _T("형태를구분할수없는자료"));
	
	if (m_strGroupName != _T("[TX]"))
	{
		m_strGroupName = _T("[AV]");
		m_mapMatCatCode_MatCatDesc.SetAt(_T("a"), _T("지도자료"));
		m_mapMatCatCode_MatCatDesc.SetAt(_T("d"), _T("구체"));
		m_mapMatCatCode_MatCatDesc.SetAt(_T("g"), _T("평면영사자료"));
		m_mapMatCatCode_MatCatDesc.SetAt(_T("k"), _T("평면비영사자료"));
		m_mapMatCatCode_MatCatDesc.SetAt(_T("m"), _T("영화"));
		m_mapMatCatCode_MatCatDesc.SetAt(_T("s"), _T("녹음자료"));
		m_mapMatCatCode_MatCatDesc.SetAt(_T("v"), _T("비디오녹화자료"));
		m_mapMatCatCode_MatCatDesc.SetAt(_T("c"), _T("컴퓨터파일"));		
	}

	// ComboBox Init
	ids  = InitMultiComboBox();
	if (ids < 0) return ids;

	CreateScreen();

	return 0;
}

INT CCat007FieldContainer::InitMultiComboBox()
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

INT CCat007FieldContainer::LoadInfo()
{
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
		//strLine.TrimRight();

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
				//strLine.TrimRight();
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
						//strLine.TrimRight();
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
								//strLine.TrimRight();
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

INT CCat007FieldContainer::CreateScreen()
{
	delete m_p007Form;
	m_p007Form = NULL;

	m_MultiComboBox.GetData(m_strSubGroupName);

	CRect rcWnd;
	GetClientRect(rcWnd);
	rcWnd.DeflateRect(5, 50);

	if (m_strSubGroupName == _T("a"))
		m_p007Form = new CCat007FixedField_A(this, m_pMarc);
	else if (m_strSubGroupName == _T("c"))
		m_p007Form = new CCat007FixedField_C(this, m_pMarc);
	else if (m_strSubGroupName == _T("d"))
		m_p007Form = new CCat007FixedField_D(this, m_pMarc);
	else if (m_strSubGroupName == _T("g"))
		m_p007Form = new CCat007FixedField_G(this, m_pMarc);
	else if (m_strSubGroupName == _T("h"))
		m_p007Form = new CCat007FixedField_H(this, m_pMarc);
	else if (m_strSubGroupName == _T("k"))
		m_p007Form = new CCat007FixedField_K(this, m_pMarc);
	else if (m_strSubGroupName == _T("m"))
		m_p007Form = new CCat007FixedField_M(this, m_pMarc);
	else if (m_strSubGroupName == _T("o"))
		m_p007Form = new CCat007FixedField_O(this, m_pMarc);
	else if (m_strSubGroupName == _T("s"))
		m_p007Form = new CCat007FixedField_S(this, m_pMarc);
	else if (m_strSubGroupName == _T("t"))
		m_p007Form = new CCat007FixedField_T(this, m_pMarc);
	else if (m_strSubGroupName == _T("v"))
		m_p007Form = new CCat007FixedField_V(this, m_pMarc);
	else if (m_strSubGroupName == _T("z"))
		m_p007Form = new CCat007FixedField_Z(this, m_pMarc);
	
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
// CCat007FieldContainer message handlers

BOOL CCat007FieldContainer::OnInitDialog() 
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

VOID CCat007FieldContainer::OnSelchangecMATCATEGORY() 
{
	CreateScreen();
}

VOID CCat007FieldContainer::OnOK() 
{
	INT ids = -1;
	
	if (m_p007Form)
	{
		ids = m_p007Form->Get007();
		if (ids < 0) return;
	}
	
	CDialog::OnOK();
}

VOID CCat007FieldContainer::OnDestroy() 
{
	CDialog::OnDestroy();

	FreeData();
	
	delete m_p007Form;
	m_p007Form = NULL;
}

HBRUSH CCat007FieldContainer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
