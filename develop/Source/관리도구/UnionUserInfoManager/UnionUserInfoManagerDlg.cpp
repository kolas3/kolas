// UnionUserInfoManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UnionUserInfoManager.h"
#include "UnionUserInfoManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnionUserInfoManagerDlg dialog

CUnionUserInfoManagerDlg::CUnionUserInfoManagerDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnionUserInfoManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pDM = NULL;
	m_pGrid = NULL;
}

CUnionUserInfoManagerDlg::~CUnionUserInfoManagerDlg()
{
	DWORD dwPID = ::GetCurrentProcessId();

	CString strFileName;
  	strFileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwPID);
	::DeleteFile(strFileName);


}
void CUnionUserInfoManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnionUserInfoManagerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUnionUserInfoManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CUnionUserInfoManagerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnSEARCH, OnbtnSEARCH)
	ON_EN_SETFOCUS(IDC_edtSEARCH_NAME, OnSetfocusedtSEARCHNAME)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CUnionUserInfoManagerDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CUnionUserInfoManagerDlg)
	ON_EVENT(CUnionUserInfoManagerDlg, IDC_gridMAIN, -600 /* DblClick */, OnClickgridMain, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnionUserInfoManagerDlg message handlers

BOOL CUnionUserInfoManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	EnableThemeDialogTexture(GetSafeHwnd()); 


	CFrameWnd* pWnd = new CFrameWnd;

	// TODO: Add extra initialization here
	CUUIM_Login Dlg(this);
	if(IDCANCEL == Dlg.DoModal())
	{
		PostMessage(WM_QUIT, NULL, NULL);
		return FALSE;
	}

	InitESL(Dlg.m_strRecKey);

	((CEdit*)GetDlgItem(IDC_edtSEARCH_CIVILNO1))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_edtSEARCH_CIVILNO2))->SetLimitText(7);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUnionUserInfoManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
// 		CAboutDlg dlgAbout;
// 		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUnionUserInfoManagerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUnionUserInfoManagerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

INT CUnionUserInfoManagerDlg::InitESL(CString strUserPK)
{
 	if (InitESL_Mgr(_T("SM_UUIM")) < 0)
 	{
 		AfxMessageBox(_T("InitESL_Mgr Error : SM_UUIM"));
 		return FALSE;
 	}

	m_pDM = FindDM(_T("DM_UUIM"));
	if(NULL == m_pDM)
	{
		MessageBox(_T("파일을 찾을 수 없습니다.\r\nDM_UUIM"), _T("검색"), MB_OK);
	}

	m_pGrid = (CESL_Grid*)FindControl(_T("CM_UUIM"), _T("그리드"));
	if(NULL == m_pGrid)
	{
		MessageBox(_T("파일을 찾을 수 없습니다.\r\nCM_UUIM"), _T("검색"), MB_OK);
	}
// 	
	m_pInfo = new CESL_Information();
// 	m_pInfo->USER_PK = strUserPK;
	m_pInfo->pCodeMgr->ReadCodeData(strUserPK);
	
// 	CIndexTool *pIndexTool = new CIndexTool;
// 	m_pInfo->pIndexTool = (CObject*)pIndexTool;
// 	m_pIndexTool = pIndexTool;

	//===================================================
	//2009.06.11 ADD BY PJW : 현재 PROCESSID의 이름을 가지는 파일에 MANAGE_CODE를 저장한다.
	DWORD dwPID = ::GetCurrentProcessId();

	CStdioFile fd;
	CString strFileName;
	CString strLine;

  	strFileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwPID);
	
	
	if (!fd.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
	{
		return -1;
	}

	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}
		
 	strLine.Format(_T("%s\r\n"), m_pInfo->MANAGE_CODE );
	fd.WriteString(strLine);

	fd.Close();
	//===================================================
	
// 	m_pInfo->InitMarcMgr();
// 	m_pInfo->pMarcMgr->SetMarcGroup("통합마크");
// 	m_pMarcMgr = m_pInfo->pMarcMgr;
// 	
// 	m_pDM  = FindDM(_T("DM_INC_MAN"));
// 	if(m_pDM == NULL) AfxMessageBox (_T(" FindDM() Error"));
// 
// 	m_pDM_Index  = FindDM(_T("DM_INC_MAN_INDEX"));
// 	if(m_pDM_Index == NULL) AfxMessageBox (_T(" FindDM() Error"));
	
	return TRUE;
	
}

void CUnionUserInfoManagerDlg::OnbtnSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strName, strCivil1;
	
	GetDlgItemText(IDC_edtSEARCH_NAME, strName);
	GetDlgItemText(IDC_edtSEARCH_CIVILNO1, strCivil1);

	BOOL bIsShowNoDuplicateUser = ((CButton*)GetDlgItem(IDC_chkSEARCH_DUPLICATE))->GetCheck();

	if(TRUE == strName.IsEmpty() && TRUE == strCivil1.IsEmpty())
	{
		MessageBox(_T("이름 또는 주민등록번호 앞자리를 입력하여 주십시요."), _T("검색"), MB_OK);
		GetDlgItem(IDC_edtSEARCH_NAME)->SetFocus();
		return;
	}
// 	if(TRUE == strName.IsEmpty())
// 	{
// 		MessageBox(_T("이름을 입력하여 주십시요."), _T("검색"), MB_OK);
// 		GetDlgItem(IDC_edtSEARCH_NAME)->SetFocus();
// 		return;
// 	}
// 	if(TRUE == strCivil1.IsEmpty())
// 	{
// 		MessageBox(_T("주민등록번호 앞자리를 입력하여 주십시요."), _T("검색"), MB_OK);
// 		GetDlgItem(IDC_edtSEARCH_CIVILNO1)->SetFocus();
// 		return;
// 	}
// 	if(0 < strCivil1.GetLength() && strCivil1.GetLength() < 6)
// 	{
// 		MessageBox(_T("주민등록번호 앞자리를 모두 입력하여 주십시요."), _T("검색"), MB_OK);
// 		GetDlgItem(IDC_edtSEARCH_CIVILNO1)->SetFocus();
// 		return;
// 	}

	CString strWhere, strTemp;
	if(FALSE == strName.IsEmpty())
	{
		strTemp.Format(_T("CLU.IDX_USER_NAME BETWEEN '%s' AND '%s힝'"), strName, strName);
		strWhere = strWhere + strTemp + _T(" AND ");
	}
	if(FALSE == strCivil1.IsEmpty())
	{
		strTemp.Format(_T("CLU.CIVIL_NO LIKE '%s%%'"), strCivil1);
		strWhere = strWhere + strTemp + _T(" AND ");
	}
	strWhere.TrimRight(_T(" AND "));

	m_pDM->RefreshDataManager(strWhere);

	// 1. 주민등록번호가 같은 이용자가 여러명인 경우 한 명만 화면에 보이고 나머지는 모두 지운다.
	// 2. 주민등록번호가 중복되지 않은 이용자를 보지 않는 경우 중복자수가 1인 열을 지운다.
	// 3. 화면상에 표시되는 주민등록번호를 *처리한다.

	CString sIndex;
	TCHAR cNum;	
	BOOL bDeleteRow = FALSE;
	INT	i = 0 , nLength = 0;
	CString strCivil_1, strCivil_2, strDuplicateCnt;
	const INT ROW_COUNT = m_pDM->GetRowCount();
	for(INT nCnt = ROW_COUNT-1; nCnt >= 0; nCnt--)
	{
		if(ROW_COUNT > 1 || nCnt >= 1)
		{
			bDeleteRow = FALSE;
			m_pDM->GetCellData(_T("주민등록번호"), nCnt, strCivil_1);
			m_pDM->GetCellData(_T("주민등록번호"), nCnt-1, strCivil_2);

			if(strCivil_1 == strCivil_2)
			{
				m_pDM->DeleteRow(nCnt);	
				bDeleteRow = TRUE;
			}
			else if(FALSE == bIsShowNoDuplicateUser)
			{
				m_pDM->GetCellData(_T("중복자수"), nCnt, strDuplicateCnt);			
				if(1 >= _ttoi(strDuplicateCnt))
				{
					m_pDM->DeleteRow(nCnt);
					bDeleteRow = TRUE;
				}
			}
		}

		if(FALSE == bDeleteRow)
		{
			nLength = strCivil_1.GetLength();
			for( i = 0 ; i<nLength ; i++ )
			{
				cNum = strCivil_1.GetAt( i );
				if( 47 < cNum && 58 > cNum )
				{
					sIndex += cNum;
				}
			}	
			if( 13 == sIndex.GetLength())
			{
				sIndex = sIndex.Left( 6 ) + _T("-*******");
			}
			if(FALSE == strCivil_1.IsEmpty())
			{
				m_pDM->SetCellData(_T("주민등록번호_화면"), sIndex, nCnt);
			}
			sIndex.Empty();
		}
	}

/*	
	CString civilNum, sIndex;
	TCHAR cNum;	
	INT	i = 0 , nLength = 0;

	const INT ROW_CNT = m_pDM->GetRowCount();
	for(INT nRow = 0; nRow < ROW_CNT; nRow++)
	{
		m_pDM->GetCellData(_T("주민등록번호"), nRow, civilNum);
		nLength = civilNum.GetLength();
		for( i = 0 ; i<nLength ; i++ )
		{
			cNum = civilNum.GetAt( i );
			if( 47 < cNum && 58 > cNum )
			{
				sIndex += cNum;
			}
		}	
		if( 13 == sIndex.GetLength())
		{
			sIndex = sIndex.Left( 6 ) + _T("-*******");
		}
		if(FALSE == civilNum.IsEmpty())
		{
			m_pDM->SetCellData(_T("주민등록번호_화면"), sIndex, nRow);
		}
		sIndex.Empty();
	}
*/
	m_pGrid->Display();
}

void CUnionUserInfoManagerDlg::OnClickgridMain()
{
	INT nRow = m_pGrid->GetRow()-1;
	m_pGrid->SetReverse(nRow);	
	CString strRecKey;
	m_pDM->GetCellData(_T("REC_KEY"), nRow, strRecKey);

	if(FALSE == strRecKey.IsEmpty())
	{
		CUUIM_Detail Dlg(this);
		Dlg.m_nIdx = nRow;

		Dlg.DoModal();
	}
}

void CUnionUserInfoManagerDlg::OnSetfocusedtSEARCHNAME() 
{
	// TODO: Add your control notification handler code here
	SetIMEStatus(this, _T("한글"), IDC_edtSEARCH_NAME);
}

BOOL CUnionUserInfoManagerDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		if(VK_RETURN == pMsg->wParam)
		{
			OnbtnSEARCH();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
