// CatCharSetManagementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CatCharSetManagementDlg.h"
#include "CharSetInputModifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCatCharSetManagementDlg dialog


CCatCharSetManagementDlg::CCatCharSetManagementDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCatCharSetManagementDlg)
	m_strKeywordFrom = _T("");
	m_strKeywordTo = _T("");
	//}}AFX_DATA_INIT

	m_pDM_Manage = NULL;
	m_pDM_Data = NULL;
	m_pGrid = NULL;

	m_bChanged = FALSE;
}

CCatCharSetManagementDlg::~CCatCharSetManagementDlg()
{
	
}

VOID CCatCharSetManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatCharSetManagementDlg)
	DDX_Text(pDX, IDC_eKEYWORD_FROM_CM, m_strKeywordFrom);
	DDX_Text(pDX, IDC_eKEYWORD_TO_CM, m_strKeywordTo);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CCatCharSetManagementDlg, CDialog)
	//{{AFX_MSG_MAP(CCatCharSetManagementDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bSEARCH_CM, OnbSEARCHCM)
	ON_BN_CLICKED(IDC_bMODIFY_CM, OnbMODIFYCM)
	ON_BN_CLICKED(IDC_bDELETE_CM, OnbDELETECM)
	ON_BN_CLICKED(IDC_bINSERT_CM, OnbINSERTCM)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()   
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatCharSetManagementDlg Operations
INT CCatCharSetManagementDlg::Init()
{
	EFS_BEGIN

	if (m_strCatagory.IsEmpty()) 
	{
		m_strLastErrMsg = _T("사전 분류를 선정하지 않았습니다.");
		return -1;
	}

	INT ids = -1;

	SetWindowText(m_strCatagory);
	GetDlgItem(IDC_STATIC_TITLE)->SetWindowText(m_strCatagory);

	// 관리 정보를 로드해 온다.
	CString strQuery;
	strQuery.Format(_T("CHARSET_NAME = '%s' AND ROWNUM = 1"), m_strCatagory);
	ids = m_pDM_Manage->RefreshDataManager(strQuery);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("관리 정보를 찾을 수 없습니다.");
		return -1;
	}

	if (m_pDM_Manage->GetRowCount() != 1)
	{
		m_strLastErrMsg = _T("관리 정보를 찾을 수 없습니다.");
		return -1;
	}

	// 관리 정보에 대당하는 데이터를 가져온다.

	CString strTblName;
	m_pDM_Manage->GetCellData(_T("TABLE_NAME"), 0, strTblName);
	if (strTblName.IsEmpty())
	{
		m_strLastErrMsg = _T("관리 정보를 찾을 수 없습니다.");
		return -1;
	}

	m_pDM_Data->TBL_NAME = strTblName;

	return 0;

	EFS_END
	return -1;
}

INT CCatCharSetManagementDlg::ApplyCharSetData()
{
	EFS_BEGIN

	BeginWaitCursor();

	INT ids = -1;

	// 040228_HSS 추가 전체 재검색
	ids = m_pDM_Data->RefreshDataManager(_T(""));
	if (ids<0) return -2;

	// 관리 DM Row
	INT nManagementDMRow = 0;
	
	// 관리 DM REC_KEY
	CString strRecKey = m_pDM_Manage->GetCellData(_T("REC_KEY"), nManagementDMRow);
	if (strRecKey.IsEmpty()) return -1;
	
	// 파일에 버전 및 데이터를 쓴다.
	CString strTempFileName = _T("~charsetmgr.tmp");
	CString strVersion = m_pDM_Manage->GetCellData(_T("VERSION"), nManagementDMRow);
	INT nVersion = _ttoi(strVersion) + 1;
	strVersion.Format(_T("%d"), nVersion);

	ids = MakeDataFile(strTempFileName, nVersion);
	if (ids < 0) return -1;

	// 관리 테이블에 파일, 파일길이, 버전정보를 업데이트 한다.

	// 파일을 통채로 BLOB으로 DB에 올린다.
	CStdioFile file;
	INT nFileSize = 0;
	if (!file.Open(strTempFileName, CFile::modeRead | CFile::typeBinary))
	{
		MessageBox(_T("파일을 여는데 실패했습니다."));
		return -1;
	}
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}
	
	nFileSize = file.GetLength();
	BYTE *pBuffer = (BYTE*)malloc(sizeof(BYTE) * (nFileSize) + 1024);
	ZeroMemory(pBuffer, nFileSize);
	nFileSize = file.ReadHuge(pBuffer, nFileSize);
	file.Close();

	// 파일
	ids = m_pDM_Manage->LOBUpdate(m_pDM_Manage->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
	if (ids < 0)
	{
		// 메모리 해제
		free(pBuffer);
		MessageBox(_T("변경적용 실패"));
		return -1;
	}
	
	// 파일길이, 버전정보
	CString strLen;
	strLen.Format(_T("%d"), nFileSize);

	ids = m_pDM_Manage->StartFrame();
	
	m_pDM_Manage->InitDBFieldData();
	m_pDM_Manage->AddDBFieldData(_T("VERSION"), _T("NUMERIC"), strVersion);
	m_pDM_Manage->AddDBFieldData(_T("FILE_LEN"), _T("NUMERIC"), strLen);
	m_pDM_Manage->MakeUpdateFrame(m_pDM_Manage->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey, nManagementDMRow);
	
	ids = m_pDM_Manage->SendFrame();
	if (ids < 0) 
	{
		// 메모리 해제
		free(pBuffer);
		MessageBox(_T("변경적용 실패했습니다."));
		return -1;
	}
	ids = m_pDM_Manage->EndFrame();
	if (ids < 0) 
	{
		// 메모리 해제
		free(pBuffer);
		MessageBox(_T("변경적용 실패했습니다."));
		return -1;
	}

	
	// 메모리 해제
	free(pBuffer);

	// 파일을 복사한다.
	CString strFileName = _T("..\\cfg\\") + m_pDM_Manage->GetCellData(_T("FILE_NAME"), nManagementDMRow);

	CopyFile(strTempFileName, strFileName, FALSE);
	
	
	// 파일 삭제
	DeleteFile(strTempFileName);

	EndWaitCursor();

	return 0;

	return -1;


	EFS_END
	return -1;
}

INT CCatCharSetManagementDlg::MakeDataFile(CString strFilePath, INT nVersion)
{
	EFS_BEGIN

	if (strFilePath.IsEmpty() || nVersion < 0) return -1;

	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		MessageBox(_T("파일을 생성하는데 실패했습니다."));
		return -1;
	}
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	// 버전
	CString strVersion;
	strVersion.Format(_T(".FILE_VERSION = %d\r\n"), nVersion);
	file.WriteString(strVersion);

	// 데이터
	//m_pDM_Data->RefreshDataManager(_T(""));
	INT nRowCount = m_pDM_Data->GetRowCount();

	INT nColFromData = m_pDM_Data->FindColumn(_T("FROM_DATA"));
	INT nColToData = m_pDM_Data->FindColumn(_T("TO_DATA"));

	CString strLine;
	CString strFromData;
	CString strToData;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strFromData = m_pDM_Data->GetCellData(idx, nColFromData);
		strToData = _T("Y");
		if (strFromData.IsEmpty() || strToData.IsEmpty()) continue;

		strLine.Format(_T("%-100s|%-100s\r\n"), strFromData, strToData);

		// 파일로 쓴다.
		file.WriteString(strLine);
	}

	file.Close();

	return 0;

	return -1;


	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CCatCharSetManagementDlg message handlers

BOOL CCatCharSetManagementDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_정리_사전관리")) < 0)
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error");
		goto ERR;
	}

	m_pDM_Manage = FindDM(_T("DM_관리_문자변환_관리정보"));
	if (m_pDM_Manage == NULL) goto ERR;

	m_pDM_Data = FindDM(_T("DM_관리_문자변환"));
	if (m_pDM_Data == NULL) goto ERR;

	m_pGrid = (CESL_Grid*)FindControl(_T("CM_정리_사전관리"), _T("GRID_DATA"));
	if (m_pGrid == NULL) goto ERR;

	if (Init() < 0) goto ERR;

	PostMessage(WM_SIZE);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);

	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CCatCharSetManagementDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (!m_pGrid || !m_pGrid->GetSafeHwnd()) return;

	CRect rcWnd;
	GetClientRect(rcWnd);

	CRect rcGrid;
	m_pGrid->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	rcWnd.DeflateRect(5, 5);
	rcGrid.bottom = rcWnd.bottom;
	rcGrid.left = rcWnd.left;
	rcGrid.right = rcWnd.right;

	m_pGrid->MoveWindow(rcGrid);

	EFS_END
}

BOOL CCatCharSetManagementDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbSEARCHCM();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

VOID CCatCharSetManagementDlg::OnbSEARCHCM() 
{
	EFS_BEGIN

	UpdateData();

	CString strQuery;
	
	if (m_strKeywordFrom.IsEmpty() && m_strKeywordTo.IsEmpty())
		;
	else if (!m_strKeywordFrom.IsEmpty() && m_strKeywordTo.IsEmpty())
		strQuery.Format(_T("FROM_DATA LIKE('%s%%')"), m_strKeywordFrom);
	else if (!m_strKeywordFrom.IsEmpty() && !m_strKeywordTo.IsEmpty())
		strQuery.Format(_T("FROM_DATA >= '%s' AND FROM_DATA <= '%s'"), m_strKeywordFrom, m_strKeywordTo);	
	
	m_pDM_Data->RefreshDataManager(strQuery);
	m_pGrid->Display();

	EFS_END
}

VOID CCatCharSetManagementDlg::OnbINSERTCM() 
{
	EFS_BEGIN

	CCharSetInputModifyDlg dlg(this);
	dlg.SetMode(MODE_INSERT_EXT);
	dlg.m_strCatagory = m_strCatagory;
	if (dlg.DoModal() != IDOK) return;

	m_pDM_Data->StartFrame();

	CString strRecKey;
	m_pDM_Data->MakeRecKey(strRecKey);
	if (strRecKey.IsEmpty()) return;

	m_pDM_Data->InitDBFieldData();
	m_pDM_Data->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
	m_pDM_Data->AddDBFieldData(_T("FROM_DATA"), _T("STRING"), dlg.m_strFromData);
	m_pDM_Data->AddDBFieldData(_T("TO_DATA"), _T("STRING"), _T("Y"));
	m_pDM_Data->MakeInsertFrame(m_pDM_Data->TBL_NAME);

	if (m_pDM_Data->SendFrame() < 0)
	{
		MessageBox(_T("입력에 실패했습니다."));
		return;
	}
	m_pDM_Data->EndFrame();

	// 데이터를 넣는다.
	m_pDM_Data->InsertRow(-1);
	INT nRow = m_pDM_Data->GetRowCount() -1;
	m_pDM_Data->SetCellData(_T("REC_KEY"), strRecKey, nRow);
	m_pDM_Data->SetCellData(_T("FROM_DATA"), dlg.m_strFromData, nRow);
	m_pGrid->DisplayLine(nRow);
	
	MessageBox(_T("입력했습니다."));

	m_bChanged = TRUE;

	EFS_END
}

VOID CCatCharSetManagementDlg::OnbMODIFYCM() 
{
	EFS_BEGIN

	INT ids = -1;
	ids = m_pGrid->SelectMakeList();
	if (ids < 0) 
	{
		MessageBox(_T("선택목록 구성을 실패했습니다."));
		return;
	}
	ids = m_pGrid->SelectGetCount();
	if (ids <= 0)
	{
		MessageBox(_T("선정한 항목이 없습니다."));
		return;
	}

	// 040915_HSS - Frame은 입력수정화면에서 직접 날린다!
	//m_pDM_Data->StartFrame();

	CCharSetInputModifyDlg dlg(this);
	dlg.SetMode(MODE_MODIFY_EXT);
	dlg.SetParentGrid(m_pGrid);
	dlg.m_strCatagory = m_strCatagory;
	dlg.DoModal();

	/*
	if (m_pDM_Data->SendFrame() < 0)
	{
		MessageBox(_T("수정에 실패했습니다."));
		return;
	}
	m_pDM_Data->EndFrame();
	*/

	m_bChanged = TRUE;

	EFS_END
}

VOID CCatCharSetManagementDlg::OnbDELETECM() 
{
	EFS_BEGIN

	INT ids = -1;
	ids = m_pGrid->SelectMakeList();
	if (ids < 0) 
	{
		MessageBox(_T("선택목록 구성을 실패했습니다."));
		return;
	}

	INT nCount = m_pGrid->SelectGetCount();

	CString strMsg;
	strMsg.Format(_T("선택한 [%d]건의 자료를 삭제하시겠습니까"), nCount);
	if (MessageBox(strMsg, _T("삭제확인"), MB_YESNO | MB_ICONQUESTION) == IDNO) return;

	m_pGrid->SelectGetPrev();

	INT idx = -1;
	
	ids = m_pDM_Data->StartFrame();

	CString strRecKey;
	idx = m_pGrid->SelectGetTailPosition();
	while (idx >= 0)
	{
		strRecKey = m_pDM_Data->GetCellData(_T("REC_KEY"), idx);
		if (!strRecKey.IsEmpty())
		{
			m_pDM_Data->MakeDeleteFrame(m_pDM_Data->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		}
		
		idx = m_pGrid->SelectGetPrev();
	}

	ids = m_pDM_Data->SendFrame();
	if (ids < 0) 
	{
		MessageBox(_T("삭제를 실패했습니다."));
	}
	ids = m_pDM_Data->EndFrame();
	if (ids < 0) 
	{
		MessageBox(_T("삭제를 실패했습니다."));
	}
	

	MessageBox(_T("삭제했습니다."));

	OnbSEARCHCM();

	m_bChanged = TRUE;

	EFS_END
}

VOID CCatCharSetManagementDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();
	
	// 변경 적용
	if (m_bChanged)
		ApplyCharSetData();

	EFS_END
}

HBRUSH CCatCharSetManagementDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
