// MarcTextToDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcTextToDB.h"
#include "MarcTextToDBDlg.h"

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
// CMarcTextToDBDlg dialog

CMarcTextToDBDlg::CMarcTextToDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcTextToDBDlg::IDD, pParent) , m_grid(NULL)
{
	//{{AFX_DATA_INIT(CMarcTextToDBDlg)
	m_strFilePath = _T("");
	m_strLog = _T("");
	m_strGroupName = _T("");
	m_nTotal = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

VOID CMarcTextToDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcTextToDBDlg)
	DDX_Text(pDX, IDC_eFILEPATH, m_strFilePath);
	DDX_Text(pDX, IDC_eLOG, m_strLog);
	DDX_CBString(pDX, IDC_cGROUP, m_strGroupName);
	DDX_Text(pDX, IDC_eTOTAL, m_nTotal);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarcTextToDBDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcTextToDBDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bSTART, OnbSTART)
	ON_BN_CLICKED(IDC_bDELTE, OnbDELTE)
	ON_CBN_SELCHANGE(IDC_cGROUP, OnSelchangecGROUP)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcTextToDBDlg message handlers

BOOL CMarcTextToDBDlg::OnInitDialog()
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
	
	// TODO: Add extra initialization here

	m_readCfgFile.ReadCfgFile(_T(""));
	m_DM.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);

	m_DM.RestructDataManager(_T("SELECT MARC_GROUP_CODE FROM MARC_GROUP_TBL"));
	INT nCount = m_DM.GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		((CComboBox*)GetDlgItem(IDC_cGROUP))->AddString(m_DM.GetCellData(i, 0));
	}

	m_grid.SubclassDlgItem(IDC_MSHFLEXGRID, this);
	SetColumnInfo();

		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

VOID CMarcTextToDBDlg::SetColumnInfo()
{
	m_grid.RemoveAll();
	m_grid.SetRows(2);

	m_grid.SetCols(0, 5);
	m_grid.SetColumnInfo(0, _T("테그 이름"), 200, 1);
	m_grid.SetColumnInfo(1, _T("테그 코드"), 200, 1);
	m_grid.SetColumnInfo(2, _T("SUBFIELD CODE"), 200, 1);
	m_grid.SetColumnInfo(3, _T("구두점"), 200, 1);

	
}

VOID CMarcTextToDBDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

VOID CMarcTextToDBDlg::OnPaint() 
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
HCURSOR CMarcTextToDBDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CMarcTextToDBDlg::OnbFILEOPEN() 
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");

	wmemset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = _T("CFG파일(*.cfg)\0*.cfg\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("MARC INFO CFG 파일 열기...");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrDefExt = _T("cfg");
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if (::GetOpenFileName(&OFN) == 0) return;

	m_strFilePath = OFN.lpstrFile;

	UpdateData(FALSE);	
}

VOID CMarcTextToDBDlg::OnbSTART() 
{
	m_nTotal = 0;
	m_strLog = _T("");
	UpdateData(FALSE);
	UpdateData();
	if (m_strFilePath.IsEmpty()) return;
	if (m_strGroupName.IsEmpty()) return;

	GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_DM.CONNECTION_INFO);
	dm.StartFrame();

	m_DM.StartFrame();

	CStdioFile file;
	file.Open(m_strFilePath, CFile::modeRead | CFile::typeBinary);

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	// 반복
	CString strRepeatType;

	// 적용수준
	CString strEssentialType;

	// 테그
	CString strTagCode;
	CString strTagName;

	// 제 1 지시기호
	CString strFirstIndSet;

	// 제 2 지시기호
	CString strSecondIndSet;

	CString strCount;
	
	CString strLine;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.Left(1) == _T(".") || strLine.IsEmpty()) continue;
		
		// 테그 이름
		if (!GetItem(strTagName, strLine)) continue;

		// 적용수준
		if (!GetItem(strEssentialType, strLine)) continue;
		if (strEssentialType == _T("N")) strEssentialType = _T("B");

		// 반복
		if (!GetItem(strRepeatType, strLine)) continue;
		if (strRepeatType == _T("T")) strRepeatType = _T("Y");
		else if (strRepeatType == _T("F")) strRepeatType = _T("N");

		// 테그
		if (!GetItem(strTagCode, strLine)) continue;

		// 제1 지시기호 세트
		if (!GetItem(strFirstIndSet, strLine)) continue;
				
		// 제2 지시기호 세트
		if (!GetItem(strSecondIndSet, strLine)) continue;
		
		// 식별기호 개수
		if (!GetItem(strCount, strLine)) continue;
		INT nCount = _ttoi(strCount);
		
		// 테그 처리
		CString strMarcTagAlias = m_strGroupName + _T("_") + strTagCode;	

		CString strPKSeq;
		strPKSeq.Format(_T("%s.NEXTVAL"), m_DM.GetDefaultSEQName());

		// Insert MARC_TAG_TBL
		m_DM.InitDBFieldData();
		m_DM.AddDBFieldData(_T("PK"), _T("NUMERIC"), strPKSeq);
		m_DM.AddDBFieldData(_T("ALIAS"), _T("STRING"), strMarcTagAlias);
		m_DM.AddDBFieldData(_T("MARC_GROUP_CODE"), _T("STRING"), m_strGroupName);
		m_DM.AddDBFieldData(_T("TAG_CODE"), _T("STRING"), strTagCode);
		m_DM.AddDBFieldData(_T("TAG_NAME"), _T("STRING"), strTagName);
		m_DM.AddDBFieldData(_T("REPEAT_TYPE"), _T("STRING"), strRepeatType);
		m_DM.AddDBFieldData(_T("ESSENTIAL_TYPE"), _T("STRING"), strEssentialType);
		m_DM.MakeInsertFrame(_T("MARC_TAG_TBL"));

		// Insert MARC_FIRST_INDICATOR_TBL
		for (INT i = 0; i < strFirstIndSet.GetLength(); i++)
		{
			CString strFirstInd = strFirstIndSet.GetAt(i);
			if (strFirstInd != _T(","))
			{
				if (strFirstInd == _T("b")) strFirstInd = _T("_");

				m_DM.InitDBFieldData();
				m_DM.AddDBFieldData(_T("PK"), _T("NUMERIC"), strPKSeq);
				m_DM.AddDBFieldData(_T("MARC_TAG_ALIAS"), _T("STRING"), strMarcTagAlias);
				m_DM.AddDBFieldData(_T("IND_CODE"), _T("STRING"), strFirstInd);
				m_DM.MakeInsertFrame(_T("MARC_FIRST_INDICATOR_TBL"));
			}
			
		}
		
		// Insert MARC_SECOND_INDICATOR_TBL
		//for (i = 0; i < strSecondIndSet.GetLength(); i++)
		for (i = 0; i < strFirstIndSet.GetLength(); i++)
		{
		//	CString strSecondInd = strSecondIndSet.GetAt(i);
			CString strSecondInd = strFirstIndSet.GetAt(i);
			if (strSecondInd != _T(","))
			{
				if (strSecondInd == _T("b")) strSecondInd = _T("_");
			
				// Insert MARC_SECOND_INDICATOR_TBL
				m_DM.InitDBFieldData();
				m_DM.AddDBFieldData(_T("PK"), _T("NUMERIC"), strPKSeq);
				m_DM.AddDBFieldData(_T("MARC_TAG_ALIAS"), _T("STRING"), strMarcTagAlias);
				m_DM.AddDBFieldData(_T("IND_CODE"), _T("STRING"), strSecondInd);
				m_DM.MakeInsertFrame(_T("MARC_SECOND_INDICATOR_TBL"));

				/*
				// TEMP...ㅠ.ㅠ
				// 있는 지 찾아서 없으면 넣는다.
				CString strResult;
				CString strQuery;
				strQuery.Format(_T("SELECT COUNT(*) FROM MARC_SECOND_INDICATOR_TBL WHERE MARC_TAG_ALIAS = '%s', IND_CODE = '%s'"), strMarcTagAlias, strSecondInd);
				INT ids = dm.GetOneValueQuery(strQuery, strResult);
				if (ids < 0 || _ttoi(strResult) == 0)
				{
					dm.InitDBFieldData();
					dm.AddDBFieldData(_T("PK"), _T("NUMERIC"), strPKSeq);
					dm.AddDBFieldData(_T("MARC_TAG_ALIAS"), _T("STRING"), strMarcTagAlias);
					dm.AddDBFieldData(_T("IND_CODE"), _T("STRING"), strSecondInd);
					dm.MakeInsertFrame(_T("MARC_SECOND_INDICATOR_TBL"));
				}
				*/
			}
			
		}

		m_nTotal++;
		m_strLog += strTagName + _T("\r\n");
		m_grid.SetTextMatrix(m_grid.GetRows() -1, 1, strTagName);
		m_grid.SetTextMatrix(m_grid.GetRows() -1, 2, strMarcTagAlias);
		m_grid.SetRows(m_grid.GetRows() + 1);

		// 서브필드 처리
		if (nCount > 0)
		{
			ProcSubfield(&file, strMarcTagAlias, nCount);
		}

	}

	file.Close();

	if (m_DM.SendFrame() < 0)
	{
		AfxMessageBox(_T("실패"));
	}
	m_DM.EndFrame();
	
	/*
	if (dm.SendFrame() < 0)
	{
		AfxMessageBox(_T("실패"));
	}
	dm.EndFrame();
	*/
	

	GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);

	UpdateData(FALSE);
}

INT CMarcTextToDBDlg::GetCount(CString strText, CString strDelemeter)
{
	INT nStart = 0;
	INT nEnd = 0;

	INT n = 1;
	while (TRUE)
	{
		nEnd = strText.Find(strDelemeter, nStart);
		if (nEnd < 0) break;

		nStart = nEnd + 1;

		n++;
	}

	return n;
}

VOID CMarcTextToDBDlg::ProcSubfield(CStdioFile *pFile, CString strMarcTagAlias, INT nCount)
{
	// 식별기호
	CString strSubfieldCode;

	// 반복
	CString strRepeatType;

	// 적용수준
	CString strEssentialType;

	// 구두점
	CString strPuncMark;

	CString strCount;
	CString strTemp;

	CString strLine;

	INT n = 0;
	while (n != nCount)
	{
		if (!pFile->ReadString(strLine)) return;

		strLine.TrimLeft();
		strLine.TrimRight();
		if (strLine.Left(1) == _T(".") || strLine.IsEmpty()) continue;
		n++;
		
		strPuncMark.Empty();

		// 식별기호
		if (!GetItem(strSubfieldCode, strLine)) continue;

		// 반복
		if (!GetItem(strRepeatType, strLine)) continue;
		if (strRepeatType == _T("T")) strRepeatType = _T("Y");
		else if (strRepeatType == _T("F")) strRepeatType = _T("N");

		// 쓰레기
		if (!GetItem(strTemp, strLine)) continue;

		// 구두점  개수
		if (!GetItem(strCount, strLine)) continue;
		INT nPuncCount = _ttoi(strCount);

		// 구두점
		if (nCount > 0)
			ProcPuncMark(pFile, nPuncCount, strPuncMark);

		CString strPKSeq;
		strPKSeq.Format(_T("%s.NEXTVAL"), m_DM.GetDefaultSEQName());
		
		m_DM.InitDBFieldData();
		m_DM.AddDBFieldData(_T("PK"), _T("NUMERIC"), strPKSeq);
		m_DM.AddDBFieldData(_T("MARC_TAG_ALIAS"), _T("STRING"), strMarcTagAlias);
		m_DM.AddDBFieldData(_T("SUBFIELD_CODE"), _T("STRING"), strSubfieldCode);
		m_DM.AddDBFieldData(_T("REPEAT_TYPE"), _T("STRING"), strRepeatType);
		m_DM.AddDBFieldData(_T("ESSENTIAL_TYPE"), _T("STRING"), strEssentialType);
		m_DM.AddDBFieldData(_T("PUNCTUATION_MARK_SET"), _T("STRING"), strPuncMark);
		m_DM.MakeInsertFrame(_T("MARC_SUBFIELD_TBL"));

		m_grid.SetTextMatrix(m_grid.GetRows() -1, 3, strSubfieldCode);
		m_grid.SetTextMatrix(m_grid.GetRows() -1, 4, strPuncMark);
		m_grid.SetRows(m_grid.GetRows() + 1);
	}
}

VOID CMarcTextToDBDlg::ProcPuncMark(CStdioFile *pFile, INT nCount, CString &strPuncMark)
{
	CString strTemp;
	CString strLine;

	INT n = 0;
	while ( n != nCount)
	{
		if (!pFile->ReadString(strLine)) return;
		
		strLine.TrimLeft();
		strLine.TrimRight();
		if (strLine.IsEmpty()) continue;
		n++;


		if (!GetItem(strTemp, strLine)) continue;

		if (strTemp.Left(1) == _T("$")) strTemp = _T("_");

		if (strPuncMark.IsEmpty())
			strPuncMark = strTemp.Left(1);
		else
			strPuncMark += _T("|") + strTemp.Left(1);
	}
}

BOOL CMarcTextToDBDlg::GetItem(CString &strItemData, CString &strLine, CString strFind /*= _T(" ")*/)
{
	INT nFind = strLine.Find(_T(" "));
	if (nFind < 0) 
	{
		nFind = strLine.GetLength() - 1;
		if (nFind < 0) return FALSE;
	}
	Mid(strLine, strItemData, nFind);
	Cut(strLine, nFind + 1);

	return TRUE;
}

VOID CMarcTextToDBDlg::Mid(CString strText, CString &strOutput, INT nEnd)
{
	strOutput = strText.Left(nEnd + 1);
	strOutput.TrimLeft();
	strOutput.TrimRight();	
}

VOID CMarcTextToDBDlg::Cut(CString &strLine, INT nStart)
{
	strLine = strLine.Mid(nStart);
	strLine.TrimLeft();
	strLine.TrimRight();
}

VOID CMarcTextToDBDlg::OnbDELTE() 
{

}

VOID CMarcTextToDBDlg::OnSelchangecGROUP() 
{
	UpdateData(TRUE);
}

VOID CMarcTextToDBDlg::OnClose() 
{
	
	CDialog::OnClose();
}

VOID CMarcTextToDBDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	CESL_DataMgr dm1;
	while(TRUE) {
		if (dm1.ORACLE_GetConnectedDB() == TRUE) {
			dm1.ORACLE_Commit();
			dm1.ORACLE_DisconnectDB();
		} else {
			break;
		}
	}		
}
