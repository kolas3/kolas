// MarcSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcSample.h"
#include "MarcSampleDlg.h"
#include "..\..\..\공통\IndexToolKit\IndexTool.h"
#include "ESL_CharSetMgr.h"
#include "..\..\esl\DlgClassCode.h"

#include "..\MarcToolKit\CreateAutorCode.h"
#include "..\MarcToolKit\ApplyMarcRelationDlg.h"

#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

VOID CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcSampleDlg dialog

CMarcSampleDlg::CMarcSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcSampleDlg)
	m_strOutput = _T("");
	m_strExample = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	AfxInitRichEdit();

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// KOL.UDF.022
	m_strStreamMarc = _T("");
	m_ptr = NULL;
}

VOID CMarcSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcSampleDlg)
	DDX_Text(pDX, IDC_eOUTPUT, m_strOutput);
	DDX_Text(pDX, IDC_eSAMPLE, m_strExample);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarcSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcSampleDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bENCODING, OnbENCODING)
	ON_BN_CLICKED(IDC_bDECODING, OnbDECODING)
	ON_BN_CLICKED(IDC_bEDIT_MARC, OnbEDITMARC)
	ON_BN_CLICKED(IDC_bSORT_MARC, OnbSORTMARC)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bINSERT1, OnbINSERT1)
	ON_BN_CLICKED(IDC_bINSERT2, OnbINSERT2)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bALIASTOFILE, OnbALIASTOFILE)
	ON_BN_CLICKED(IDC_bMARC2TEXT, OnbMARC2TEXT)
	ON_BN_CLICKED(IDC_bSETSTREAMMARC, OnbSETSTREAMMARC)
	ON_BN_CLICKED(IDC_bINDEX, OnbINDEX)
	ON_BN_CLICKED(IDC_CHAR, OnChar)
	ON_BN_CLICKED(IDC_INSERT_FIELD, OnInsertField)
	ON_BN_CLICKED(IDC_bCLASS_CODE, OnbCLASSCODE)
	ON_BN_CLICKED(IDC_bDELTE_FIELD, OnbDELTEFIELD)
	ON_BN_CLICKED(IDC_bDELTE_EMPTY_FIELD, OnbDELTEEMPTYFIELD)
	ON_BN_CLICKED(IDC_bGET_FIELD, OnbGETFIELD)
	ON_BN_CLICKED(IDC_bCONVERSION, OnbCONVERSION)
	ON_BN_CLICKED(IDC_bTEST, OnbTEST)
	ON_BN_CLICKED(IDC_bSETIND, OnbSETIND)
	ON_BN_CLICKED(IDC_bUPDATE_FIELD, OnbUPDATEFIELD)
	ON_BN_CLICKED(IDC_bAUTORCODE, OnbAUTORCODE)
	ON_BN_CLICKED(IDC_NEW_MARC2TEXT, OnNewMarc2text)
	ON_NOTIFY(EN_KILLFOCUS, IDC_RICHEDIT, OnKillfocusRichedit)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcSampleDlg message handlers

BOOL CMarcSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	// IDM_ABOUTBOX must be in the _tsystem command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's _tmain window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//////////////////////////////////////////////////////////////////////////
	// Sample Start

	// Init Marc Manager
	m_marcMgr.InitMarcMgr(_T("단행"), TRUE);

	// Init Marc Editor
	m_marcEditor.SubclassDlgItem(IDC_RICHEDIT, IDC_MSHFLEXGRID, this);
	m_marcEditor.SetMarcMgr(&m_marcMgr);
	m_marcEditor.SetExampleEditBox(GetDlgItem(IDC_eEXAMPLE)->GetSafeHwnd());

	// Read Sample Marc
	CStdioFile file;
	if (!file.Open(_T("TESTMARC.TXT"), CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("Test Marc Load Filed!"));
		return FALSE;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	file.ReadString(m_strStreamMarc);
	m_strStreamMarc.TrimLeft();
	m_strStreamMarc.TrimRight();

	m_strExample = 
		_T("$ : 구분기호 \r\n")
		_T("* : Tag item data \r\n")
		_T("@ : 자리수 (제어필드에서 쓰임) \r\n")
		_T("EX)  001, *, \r\n")
		_T("     020, $, a	\r\n")
		_T("     008, @, 0-5 \r\n")
		_T("     008, @, 6 \r\n");

	m_marcEditor.Init(&m_marcMgr, &m_marc);
	UpdateData(FALSE);
		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CMarcSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

VOID CMarcSampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		INT cxIcon = GetSystemMetrics(SM_CXICON);
		INT cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		INT x = (rect.Width() - cxIcon + 1) / 2;
		INT y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The _tsystem calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMarcSampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CMarcSampleDlg::ShowErrMsg()
{
	CString strTagCode, strErrMsg, strHelpMsg;

	POSITION pos;
	pos = m_marcMgr.GetHeadPositionErrMsg();
	while (pos)
	{
		m_marcMgr.GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
		m_strOutput += ( strErrMsg + strHelpMsg + _T("\r\n"));
	}
	
	UpdateData(FALSE);
	
	CEdit *pEdit = ((CEdit*)GetDlgItem(IDC_eOUTPUT));
	pEdit->LineScroll(pEdit->GetLineCount());
}

///////////////////////////////////////////////////////////////////////////////
// Sample Code Start.~~~~~

VOID CMarcSampleDlg::OnbDECODING() 
{
	m_strOutput += _T("====Decode Start====\r\n");
	INT ids = m_marcMgr.Decoding(m_strStreamMarc, &m_marc);

	if (ids < 0)
		ShowErrMsg();
	else
	{
		m_strOutput += m_strStreamMarc + _T("\r\n");
		m_strOutput += _T("====Decode Finish====\r\n");

		UpdateData(FALSE);
		CEdit *pEdit = ((CEdit*)GetDlgItem(IDC_eOUTPUT));
		pEdit->LineScroll(pEdit->GetLineCount());
	}	
}

VOID CMarcSampleDlg::OnbENCODING() 
{
	m_strOutput += _T("====Encode Start====\r\n");
	CString strStreamMarc;
	INT ids = m_marcMgr.Encoding(&m_marc, strStreamMarc);
	if (ids < 0)
		ShowErrMsg();
	else
		m_strOutput += strStreamMarc + _T("\r\n");
	
	m_strOutput += _T("====Encode Finish====\r\n");
	UpdateData(FALSE);
	CEdit *pEdit = ((CEdit*)GetDlgItem(IDC_eOUTPUT));
	pEdit->LineScroll(pEdit->GetLineCount());
}

VOID CMarcSampleDlg::OnbEDITMARC() 
{
	m_marcEditor.InitUserAlias();
	m_marcEditor.AddUserAlias(_T("제어번호"));
	m_marcEditor.AddUserAlias(_T("낱권ISBN"));
	m_marcEditor.AddUserAlias(_T("낱권ISBN부가기호"));
	m_marcEditor.AddUserAlias(_T("세트ISBN"));
	m_marcEditor.AddUserAlias(_T("세트ISBN부가기호"));
	m_marcEditor.AddUserAlias(_T("본표제"));
	m_marcEditor.AddUserAlias(_T("부표제"));
	m_marcEditor.AddUserAlias(_T("대등표제"));
	m_marcEditor.AddUserAlias(_T("잡정보"));
	m_marcEditor.AddUserAlias(_T("저작자"));
	m_marcEditor.AddUserAlias(_T("판사항"));
	m_marcEditor.AddUserAlias(_T("편/권차"));
	m_marcEditor.AddUserAlias(_T("편제"));
	m_marcEditor.AddUserAlias(_T("발행지"));
	m_marcEditor.AddUserAlias(_T("발행자"));
	m_marcEditor.AddUserAlias(_T("발행년"));
	m_marcEditor.AddUserAlias(_T("면장수"));
	m_marcEditor.AddUserAlias(_T("물리적특성"));
	m_marcEditor.AddUserAlias(_T("크기"));
	m_marcEditor.AddUserAlias(_T("딸린자료"));
	m_marcEditor.AddUserAlias(_T("총서표제"));
	m_marcEditor.AddUserAlias(_T("총서편차"));
	m_marcEditor.AddUserAlias(_T("가격성격"));
	m_marcEditor.AddUserAlias(_T("가격"));
	m_marcEditor.AddUserAlias(_T("가격잡정보"));
	m_marcEditor.AddUserAlias(_T("등록번호"));
	m_marcEditor.AddUserAlias(_T("청구기호"));
	m_marcEditor.AddUserAlias(_T("도서기호"));
	m_marcEditor.AddUserAlias(_T("별치기호"));
	m_marcEditor.AddUserAlias(_T("키워드"));
	m_marcEditor.AddUserAlias(_T("입력날자"));
	m_marcEditor.AddUserAlias(_T("유형"));
	m_marcEditor.AddUserAlias(_T("발행년1"));
	m_marcEditor.AddUserAlias(_T("발행년2"));
	m_marcEditor.AddUserAlias(_T("발행국명"));
	m_marcEditor.AddUserAlias(_T("삽도표시"));
	m_marcEditor.AddUserAlias(_T("자료식별표시"));
	

	m_marcEditor.InitLimitTag();
	//m_marcEditor.AddLimitTag(_T("245"));
	m_marcEditor.Display();
}

VOID CMarcSampleDlg::OnbSORTMARC() 
{
	INT ids = m_marcMgr.SortMarc(&m_marc);
	if (ids < 0)
		ShowErrMsg();
	else
		OnbEDITMARC();
}

VOID CMarcSampleDlg::OnbSEARCH() 
{
	CString strAlias;
	CString strItemData;
	GetDlgItem(IDC_eALIAS)->GetWindowText(strAlias);
	
	INT ids = m_marcMgr.GetItem(&m_marc, strAlias, strItemData);
	if (ids < 0)
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(m_marc.GetLastError());
	else
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(strItemData);
}

VOID CMarcSampleDlg::OnbINSERT1() 
{
	CString strAlias;
	CString strItemData;
	GetDlgItem(IDC_eALIAS_KEYVALUE)->GetWindowText(strAlias);
	GetDlgItem(IDC_eITEMDATA)->GetWindowText(strItemData);

	INT ids = m_marcMgr.SetItem(&m_marc, strAlias, strItemData,_T(""));
	if (ids < 0)
		ShowErrMsg();
	else
		OnbEDITMARC();
}

VOID CMarcSampleDlg::OnbINSERT2() 
{
	CString strAliasType;
	CString strTagCode;
	CString strData;
	CString strItemData;

	GetDlgItem(IDC_eALIASTYPE)->GetWindowText(strAliasType);
	GetDlgItem(IDC_eTAGCODE)->GetWindowText(strTagCode);
	GetDlgItem(IDC_eDATA)->GetWindowText(strData);
	GetDlgItem(IDC_eITEMDATA2)->GetWindowText(strItemData);

	INT ids = m_marcMgr.SetItem(&m_marc, strAliasType, strTagCode, strData, strItemData, _T(""));
	if (ids < 0)
		ShowErrMsg();
	else
		OnbEDITMARC();
}

VOID CMarcSampleDlg::OnbDELETE() 
{
	CString strAlias;
	CString strItemData;
	GetDlgItem(IDC_eALIAS_KEYVALUE)->GetWindowText(strAlias);
	GetDlgItem(IDC_eITEMDATA)->GetWindowText(strItemData);

	INT ids = m_marcMgr.DeleteItem(&m_marc, strAlias, strItemData);
	if (ids < 0)
		ShowErrMsg();
	else
		OnbEDITMARC();
}

VOID CMarcSampleDlg::OnbALIASTOFILE() 
{
	CMarcAliasToFile dlg;
	dlg.DoModal();
}

VOID CMarcSampleDlg::OnbMARC2TEXT() 
{
	CMarcRelationToText marcRelationToText;
	marcRelationToText.DoModal();		
}

VOID CMarcSampleDlg::OnOK() 
{
	return;	
	CDialog::OnOK();
}

VOID CMarcSampleDlg::OnbSETSTREAMMARC() 
{
	GetDlgItem(IDC_eOUTPUT)->GetWindowText(m_strStreamMarc);	
}

VOID CMarcSampleDlg::OnbINDEX() 
{
	CString strIndexAlias;
	CString strIndexItem;
	GetDlgItem(IDC_eINDEX_ALIAS)->GetWindowText(strIndexAlias);
	
	CIndexTool indexTool;
	indexTool.Init(&m_marcMgr, &m_marc);
	INT ids = indexTool.GetIndex(strIndexAlias, strIndexItem);
	 if (ids < 0)
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(indexTool.GetLastError());
	else
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(strIndexItem);
}


VOID CMarcSampleDlg::OnChar() 
{
}

VOID CMarcSampleDlg::OnInsertField() 
{
	CString strField;
	GetDlgItem(IDC_eINDEX_ALIAS)->GetWindowText(strField);
	INT ids = m_marcMgr.InsertField(&m_marc, strField);
	if (ids >=0 )
	{
		OnbEDITMARC();
	}
}

VOID CMarcSampleDlg::OnbCLASSCODE() 
{
	CDlgClassCode dlg;
	dlg.m_strKeyword = _T("a");
	dlg.AddData(_T("a"), _T("A"));
	dlg.AddData(_T("b"), _T("AAA"));
	dlg.AddData(_T("c"), _T("ADD"));
	dlg.AddData(_T("d"), _T("AGG"));

	if (dlg.DoModal() == IDOK)
	{
		AfxMessageBox(dlg.m_strSelCode + dlg.m_strSelDesc);
	}

}

VOID CMarcSampleDlg::OnbDELTEFIELD() 
{
	CString strTagCode;
	GetDlgItem(IDC_eFIELD_TAG_CODE)->GetWindowText(strTagCode);
	INT ids = m_marcMgr.DeleteField(&m_marc, strTagCode);
	if (ids >=0 )
	{
		OnbEDITMARC();
	}	
}

VOID CMarcSampleDlg::OnbDELTEEMPTYFIELD() 
{
	INT ids = m_marcMgr.DeleteEmptyField(&m_marc);
	if (ids >= 0)
	{
		OnbEDITMARC();
	}
}

VOID CMarcSampleDlg::OnbGETFIELD() 
{
	CArray <CString, CString&> array;
	CString strField;
	INT ids = m_marcMgr.GetField(&m_marc, _T("245"), strField, &array);
	if (ids >= 0)
	{
		strField.Empty();
		for (INT i = 0; i < array.GetSize(); i++)
		{
			strField += array.GetAt(i);		
		}
		GetDlgItem(IDC_eOUTPUT)->SetWindowText(strField);
	}
}

VOID CMarcSampleDlg::OnbCONVERSION() 
{
}

VOID CMarcSampleDlg::OnbTEST() 
{
	/*AfxSocketInit(NULL);
	HOSTENT *pHostent;
	TCHAR szHostName[50];
	CString strMsg;

	INT i = gethostname(szHostName, 100);
	pHostent = gethostbyname(szHostName);

	/*
	DWORD *ptr = (DWORD*)this;

	SetPtr(ptr);

	ptr = NULL;

	GetPtr(&ptr);
	*/
}


INT CMarcSampleDlg::SetPtr(DWORD *ptr)
{
	m_ptr = ptr;
	return 0;
}

INT CMarcSampleDlg::GetPtr(DWORD **ptr)
{
	*ptr = m_ptr;
	return 0;
}

VOID CMarcSampleDlg::OnbSETIND() 
{
	CString strTagCode;
	CString strFirstInd;
	CString strSecondInd;
	
	GetDlgItem(IDC_eTAGCODE)->GetWindowText(strTagCode);
	GetDlgItem(IDC_eALIASTYPE)->GetWindowText(strFirstInd);
	GetDlgItem(IDC_eDATA)->GetWindowText(strSecondInd);
	
	INT ids;

	
	ids = m_marcMgr.SetTagIndicator(&m_marc, strTagCode, strFirstInd, strSecondInd);
}

VOID CMarcSampleDlg::OnbUPDATEFIELD() 
{
	CString strOldField;
	CString strNewField;

	GetDlgItem(IDC_eFIELD_FROM)->GetWindowText(strOldField);
	GetDlgItem(IDC_eFIELD_TO)->GetWindowText(strNewField);

	m_marcMgr.UpdateField(&m_marc, strOldField, strNewField);

	OnbEDITMARC();

}

VOID CMarcSampleDlg::OnbAUTORCODE() 
{
	CString str = _T("자자");
	CString str2;
	CCreateAutorCode a;
	a.GetLeeJaeChulAuthorData(_T('1'), str.GetBuffer(0), str2.GetBuffer(256));

}

VOID CMarcSampleDlg::OnNewMarc2text() 
{
	CApplyMarcRelationDlg dlg;
	dlg.DoModal();
}

VOID CMarcSampleDlg::OnKillfocusRichedit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

LRESULT CMarcSampleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_COMMAND)
	{
		UINT nID = HIWORD(wParam);
		UINT nID3 = LOWORD(wParam);
		
		UINT nID2 = IDC_RICHEDIT;

		UINT nID4 = EN_KILLFOCUS;

		INT i = 1;
	}
	if (message == EN_KILLFOCUS)
	{
		HWND a = (HWND)lParam;
		HWND b = GetDlgItem(IDC_RICHEDIT)->GetSafeHwnd();
		if ((HWND)lParam == GetDlgItem(IDC_RICHEDIT)->GetSafeHwnd())
			INT i = 0;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CMarcSampleDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KILLFOCUS)
	{

		HWND a = (HWND)pMsg->lParam;
		HWND b = GetDlgItem(IDC_RICHEDIT)->GetSafeHwnd();

		if ( a == b)
			INT i =  0;
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CMarcSampleDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	if (pNewWnd->GetSafeHwnd() == GetDlgItem(IDC_RICHEDIT)->GetSafeHwnd())
		AfxMessageBox(_T("a"));
	
}
