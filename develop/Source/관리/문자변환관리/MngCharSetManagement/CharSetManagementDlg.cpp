// CharSetManagementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CharSetManagementDlg.h"
#include "CharSetInputModifyDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharSetManagementDlg dialog


CCharSetManagementDlg::CCharSetManagementDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharSetManagementDlg)
	m_strKeywordFrom = _T("");
	m_strKeywordTo = _T("");
	//}}AFX_DATA_INIT

	m_pGrid_Data = NULL;

	m_pDM_Data = NULL;
	m_pDM_Manage = NULL;
}
CCharSetManagementDlg::~CCharSetManagementDlg()
{
	
}

BOOL CCharSetManagementDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CCharSetManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharSetManagementDlg)
	DDX_Text(pDX, IDC_eKEYWORD_FROM_CM, m_strKeywordFrom);
	DDX_Text(pDX, IDC_eKEYWORD_TO_CM, m_strKeywordTo);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CCharSetManagementDlg, CDialog)
	//{{AFX_MSG_MAP(CCharSetManagementDlg)
	ON_CBN_SELCHANGE(IDC_cMANAGE, OnSelchangecMANAGE)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bSEARCH_CM, OnbSEARCHCM)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharSetManagementDlg Operations
INT CCharSetManagementDlg::InitCharSetMangeData()
{
	EFS_BEGIN

	m_pDM_Manage->RefreshDataManager(_T(""));
	DisplayComboBox();
	return 0;

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::InitCharSetData(CString strTableName, CString strOption)
{
	EFS_BEGIN

	if (strTableName.IsEmpty()) return -1;

	INT ids = -1;

	m_pDM_Data->TBL_NAME = strTableName;

	return Display(strOption);
	

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::Display(CString strOption)
{
	EFS_BEGIN

	if (m_pDM_Data->TBL_NAME.IsEmpty()) return -1;

	INT ids = m_pDM_Data->RefreshDataManager(strOption);
	if (ids < 0) return ids;

	if (m_pDM_Data->GetRowCount() < 1)
		AfxMessageBox(_T("검색된 결과가 없습니다."));
	
	return m_pGrid_Data->Display();

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::DisplayComboBox()
{
	EFS_BEGIN

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMANAGE);
	if (pComboBox == NULL) return -1;

	pComboBox->ResetContent();
	pComboBox->AddString(_T("== 선택하세요 =="));
	
	INT nRowCount = m_pDM_Manage->GetRowCount();
	CString strCharSetName;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strCharSetName = m_pDM_Manage->GetCellData(_T("CHARSET_NAME"), idx);
		if (strCharSetName.IsEmpty()) continue;

		pComboBox->AddString(strCharSetName);
	}

	pComboBox->SetCurSel(0);

	return 0;

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::DisplayMangementInfo(INT nRow)
{
	EFS_BEGIN

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_MANAGE_CM);
	if (pGrid == NULL) return -1;

	pGrid->Clear();
	pGrid->SetCols(0, 2);
	pGrid->SetColWidth(0, 0, 80 * 15);
	pGrid->SetColWidth(1, 0, 140 * 15);
	
	// 보여줄것
	// CHARSET_NAME
	// VERSION
	// FROM_SET
	// TO_SET
	// FILE_NAME

	const INT nRows = 5;
	CString strArrShowAlias[nRows][2] = {
		{_T("그룹 이름"),		_T("CHARSET_NAME")},
		{_T("버전"),			_T("VERSION")},
		{_T("변경할데이터"),	_T("FROM_SET")},
		{_T("변경데이터"),		_T("TO_SET")},
		{_T("파일이름"),		_T("FILE_NAME")}
	};

	pGrid->SetRows(nRows + 1);
	for (INT i = 0; i < nRows; i++)
	{
		pGrid->SetTextMatrix(i + 1, 0, strArrShowAlias[i][0]);
		pGrid->SetTextMatrix(i + 1, 1, m_pDM_Manage->GetCellData(strArrShowAlias[i][1], nRow));
	}

	m_strCatagory = m_pDM_Manage->GetCellData(strArrShowAlias[0][1], nRow);
	
	return 0;

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::InsertCharSetData()
{
	EFS_BEGIN

	CCharSetInputModifyDlg dlg(this);
	if (m_strCatagory == _T("역할어") ||
		m_strCatagory == _T("조사") ||
		m_strCatagory == _T("단어") ||
		m_strCatagory == _T("불용어"))
	{
		dlg.SetMode(MODE_INSERT_EXT);
	}
	else
		dlg.SetMode(MODE_INSERT);
	dlg.m_strCatagory = m_strCatagory;
	if (dlg.DoModal() != IDOK) return -1;

	m_pDM_Data->StartFrame();


	CString strRecKey;
	m_pDM_Data->MakeRecKey(strRecKey);
	if (strRecKey.IsEmpty()) return -1;

	m_pDM_Data->InitDBFieldData();
	m_pDM_Data->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
	m_pDM_Data->AddDBFieldData(_T("FROM_DATA"), _T("STRING"), dlg.m_strFromData);
	//=====================================================
	//2009.01.20 ADD BY PJW : 입력할경우 "역할어","조사","단어","불용어"는 Y로 셋팅한다.
	if (m_strCatagory == _T("역할어") ||
		m_strCatagory == _T("조사") ||
		m_strCatagory == _T("단어") ||
		m_strCatagory == _T("불용어"))
	{
		dlg.m_strToData =  _T("Y");
	}
	//=====================================================
	m_pDM_Data->AddDBFieldData(_T("TO_DATA"), _T("STRING"), dlg.m_strToData);
	
	
	m_pDM_Data->MakeInsertFrame(m_pDM_Data->TBL_NAME);

	if (m_pDM_Data->SendFrame() < 0)
	{
		MessageBox(_T("입력에 실패했습니다."));
		return -1;
	}
	m_pDM_Data->EndFrame();

	// 데이터를 넣는다.
	m_pDM_Data->InsertRow(-1);
	INT nRow = m_pDM_Data->GetRowCount() -1;
	m_pDM_Data->SetCellData(_T("REC_KEY"), strRecKey, nRow);
	m_pDM_Data->SetCellData(_T("FROM_DATA"), dlg.m_strFromData, nRow);
	m_pDM_Data->SetCellData(_T("TO_DATA"), dlg.m_strToData, nRow);
	m_pGrid_Data->DisplayLine(nRow);

	MessageBox(_T("입력했습니다."));

	return 0;

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::ModifyCharSetData()
{
	EFS_BEGIN

	INT ids = -1;
	
	ids = MakeSelectListGrid();
	if (ids < 0) return -1;

	m_pDM_Data->StartFrame();

	CCharSetInputModifyDlg dlg(this);

	if (m_strCatagory == _T("역할어") ||
		m_strCatagory == _T("조사") ||
		m_strCatagory == _T("단어") ||
		m_strCatagory == _T("불용어"))
	{
		dlg.SetMode(MODE_MODIFY_EXT);
	}
	else
		dlg.SetMode(MODE_MODIFY);
	
	dlg.m_strCatagory = m_strCatagory;
	dlg.DoModal();

	
	return 0;

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::DeleteCharSetData()
{
	EFS_BEGIN

	INT ids = -1;
	ids = m_pGrid_Data->SelectMakeList();
	if (ids < 0) 
	{
		MessageBox(_T("선택목록 구성을 실패했습니다."));
		return ids;
	}

	INT nCount = m_pGrid_Data->SelectGetCount();

	CString strMsg;
	strMsg.Format(_T("선택한 [%d]건의 자료를 삭제하시겠습니까"), nCount);
	if (MessageBox(strMsg, _T("삭제확인"), MB_YESNO | MB_ICONQUESTION) == IDNO) return -1;

	m_pGrid_Data->SelectGetPrev();

	INT idx = -1;
	
	ids = m_pDM_Data->StartFrame();

	CString strRecKey;
	idx = m_pGrid_Data->SelectGetTailPosition();
	while (idx >= 0)
	{
		strRecKey = m_pDM_Data->GetCellData(_T("REC_KEY"), idx);
		if (!strRecKey.IsEmpty())
		{
			m_pDM_Data->MakeDeleteFrame(m_pDM_Data->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		}
		
		idx = m_pGrid_Data->SelectGetPrev();
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
	
	MessageBox(_T("삭제 성공."));

	OnbSEARCHCM();

	return 0;

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::ApplyCharSetData()
{
	EFS_BEGIN

	BeginWaitCursor();

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMANAGE);
	if (pComboBox == NULL) return -1;

	INT ids = -1;

	// 관리 DM Row
	INT nManagementDMRow = pComboBox->GetCurSel() - 1;
	if (nManagementDMRow < 0) return -1;

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
	//m_pDM_Manage->AddDBFieldData(_T("FILE_NAME"), _T("STRING"), m_pDM_Manage->GetCellData(_T("TABLE_NAME"), nManagementDMRow) + _T(".TXT"));
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

	// Display
	DisplayMangementInfo(nManagementDMRow);

	EndWaitCursor();
	MessageBox(_T("변경적용 완료."));

	return 0;

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::MakeDataFile(CString strFilePath, INT nVersion)
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
	m_pDM_Data->RefreshDataManager(_T(""));
	INT nRowCount = m_pDM_Data->GetRowCount();

	INT nColFromData = m_pDM_Data->FindColumn(_T("FROM_DATA"));
	INT nColToData = m_pDM_Data->FindColumn(_T("TO_DATA"));

	CString strLine;
	CString strFromData;
	CString strToData;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strFromData = m_pDM_Data->GetCellData(idx, nColFromData);
		strToData = m_pDM_Data->GetCellData(idx, nColToData);
		
		if ( m_strCatagory == _T("조사") || 
			 m_strCatagory == _T("단어") ||
			 m_strCatagory == _T("불용어") ||
			 m_strCatagory == _T("역할어") )
		{
			if (strFromData.IsEmpty()) continue;
		}
		else 
		{
			if (strFromData.IsEmpty() || strToData.IsEmpty()) continue;
		}

		strLine.Format(_T("%-100s|%-100s\r\n"), strFromData, strToData);

		// 파일로 쓴다.
		file.WriteString(strLine);
	}

	file.Close();

	return 0;

	EFS_END
	return -1;

}

INT CCharSetManagementDlg::MakeSelectListGrid()
{
	EFS_BEGIN

	INT ids = -1;
	// 부모 그리드의 선택된 로우 구성
	INT nRow = -1;
	ids = m_pGrid_Data->SelectMakeList();
	if (ids < 0)
	{
		AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
		return -1;
	}
	
	// 선택된 로우가 없으면 처음 선택된 로우를 선정한다.
	nRow = m_pGrid_Data->GetIdx();
	if (nRow < 0)
	{
		nRow = m_pGrid_Data->SelectGetHeadPosition();
		if (nRow < 0)
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}
	}
	else
	{
		POSITION pos = m_pGrid_Data->m_arraySelect.GetHeadPosition();
		while (pos)
		{
			m_pGrid_Data->m_arrayPos = pos;
			if (nRow == m_pGrid_Data->m_arraySelect.GetNext(pos))
				break;				
		}
		
		if (m_pGrid_Data->m_arrayPos == NULL)
		{
			m_pGrid_Data->SelectRemoveAll();
			INT nRowCount = m_pGrid_Data->GetCount();
			for (INT idx = 0; idx < nRowCount; idx++)
				m_pGrid_Data->m_arraySelect.AddTail(idx);
		}
		
		if (m_pGrid_Data->m_arraySelect.GetCount() > 0)
			m_pGrid_Data->m_arrayPos = m_pGrid_Data->m_arraySelect.GetHeadPosition();
		else
		{
			AfxMessageBox(_T("선택목록을 구성할 수 없습니다."));
			return -1;
		}
		
		pos = m_pGrid_Data->m_arraySelect.GetHeadPosition();
		while (pos)
		{
			m_pGrid_Data->m_arrayPos = pos;
			if (nRow == m_pGrid_Data->m_arraySelect.GetNext(pos))
				break;				
		}

		pos = m_pGrid_Data->m_arraySelect.GetHeadPosition();
		while (pos)
		{
			m_pGrid_Data->m_arrayPos = pos;
			if (nRow == m_pGrid_Data->m_arraySelect.GetNext(pos))
				break;				
		}
	}
	
	m_pGrid_Data->SetReverse(nRow);
	
	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CCharSetManagementDlg message handlers

BOOL CCharSetManagementDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("SM_관리_문자변환")) < 0)
		goto ERR;
	
	m_pDM_Manage = FindDM(_T("DM_관리_문자변환_관리정보"));
	if (m_pDM_Manage == NULL) goto ERR;

	m_pDM_Data = FindDM(_T("DM_관리_문자변환"));
	if (m_pDM_Data == NULL) goto ERR;

	m_pGrid_Data = (CESL_Grid*)FindControl(_T("CM_관리_문자변환"), _T("GRID_DATA"));
	if (m_pGrid_Data == NULL) goto ERR;

	InitCharSetMangeData();
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
ERR:
	MessageBox(_T("초기화 실패"));
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;

}

VOID CCharSetManagementDlg::OnSelchangecMANAGE() 
{
	EFS_BEGIN

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMANAGE);
	if (pComboBox == NULL) return ;

	INT nCurSel = pComboBox->GetCurSel();
	if (nCurSel < 1) return;

	CString strTableName = m_pDM_Manage->GetCellData(_T("TABLE_NAME"), nCurSel - 1);
	if (strTableName.IsEmpty()) return;
		
	DisplayMangementInfo(nCurSel - 1);
	
	m_pDM_Data->FreeData();
	m_pGrid_Data->Display();

	EFS_END
}

VOID CCharSetManagementDlg::OnbSEARCHCM() 
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
	
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMANAGE);
	if (pComboBox == NULL) return ;

	INT nCurSel = pComboBox->GetCurSel();
	if (nCurSel < 1)
	{
		AfxMessageBox(_T("검색 항목을 선택해 주세요"));
		return;
	}

	CString strTableName = m_pDM_Manage->GetCellData(_T("TABLE_NAME"), nCurSel - 1);
	if (strTableName.IsEmpty()) return;

	BeginWaitCursor();

	InitCharSetData(strTableName, strQuery);
	
	EndWaitCursor();

	EFS_END
}

BEGIN_EVENTSINK_MAP(CCharSetManagementDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCharSetManagementDlg)
	ON_EVENT(CCharSetManagementDlg, IDC_GRID_DATA_CM, -601 /* DblClick */, OnDblClickGridDataCm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCharSetManagementDlg::OnDblClickGridDataCm() 
{
	EFS_BEGIN
	
	ModifyCharSetData();

	EFS_END
}

VOID CCharSetManagementDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;

	CRect rcClient;
	GetClientRect(rcClient);

	// IDC_GRID_MANAGE_CM
	// IDC_GRID_DATA_CM

	CWnd *pWnd = NULL;

	CRect rcResource;

	pWnd = GetDlgItem(IDC_GRID_MANAGE_CM);
	if (pWnd == NULL) return;

	pWnd->GetWindowRect(rcResource);
	ScreenToClient(rcResource);

	rcResource.bottom = rcClient.bottom - 1;

	pWnd->MoveWindow(rcResource);

	pWnd = GetDlgItem(IDC_GRID_DATA_CM);
	if (pWnd == NULL) return;

	pWnd->GetWindowRect(rcResource);
	ScreenToClient(rcResource);

	rcResource.bottom = rcClient.bottom - 1;
	rcResource.right = rcClient.right - 1;

	pWnd->MoveWindow(rcResource);	

	EFS_END
}

BOOL CCharSetManagementDlg::PreTranslateMessage(MSG* pMsg) 
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

HBRUSH CCharSetManagementDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


