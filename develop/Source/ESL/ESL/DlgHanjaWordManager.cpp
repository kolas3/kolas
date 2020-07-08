// DlgHanjaWordManager.cpp : implementation file
//

#include "stdafx.h"
#include "DlgHanjaWordManager.h"
#include "DlgHanjaWordInputModify.h"
#include "FWDES.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHanjaWordManager dialog


CDlgHanjaWordManager::CDlgHanjaWordManager(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHanjaWordManager::IDD, pParent), m_Grid(NULL)
{
	//{{AFX_DATA_INIT(CDlgHanjaWordManager)
	m_strInput = _T("");
	//}}AFX_DATA_INIT

	m_nBlockSize = 80;
	m_nHalfBlockSize = m_nBlockSize/2;

}


VOID CDlgHanjaWordManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHanjaWordManager)
	DDX_Text(pDX, IDC_eINPUT_HANJA_WORD, m_strInput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHanjaWordManager, CDialog)
	//{{AFX_MSG_MAP(CDlgHanjaWordManager)
	ON_BN_CLICKED(IDC_bREFRESH_USER, OnbREFRESHUSER)
	ON_BN_CLICKED(IDC_bREFRESH_ORIGINAL, OnbREFRESHORIGINAL)
	ON_BN_CLICKED(IDC_bSEARCH_HANJA_WORD, OnbSEARCHHANJAWORD)
	ON_BN_CLICKED(IDC_bINPUT_HANJA_WORD, OnbINPUTHANJAWORD)
	ON_BN_CLICKED(IDC_bMODIFY_HANJA_WORD, OnbMODIFYHANJAWORD)
	ON_BN_CLICKED(IDC_bDELETE_HANJA_WORD, OnbDELETEHANJAWORD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHanjaWordManager message handlers

BOOL CDlgHanjaWordManager::OnInitDialog() 
{
	CDialog::OnInitDialog();

	INT ids;
	ids = GetReferenceDBInfo();
	if (ids < 0)
	{
		MessageBox(_T("Get Reference DB Info Failed!"), _T("Warnning!"), MB_ICONEXCLAMATION);
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_DM_Origin.SetCONNECTION_INFO(CONNECTION_INFO);
	m_DM_User.SetCONNECTION_INFO(CONNECTION_INFO);

	m_strOriginTableName = _T("CO_CHARSET_KOR_WD_2_CHI_WD_TBL");
	m_strUserTableName = _T("CO_CHARSET_U_K_WD_2_U_C_WD_TBL");

	m_Grid.SubclassDlgItem(IDC_MSHFLEXGRID_HANJA_WORD, this);
	

	if (!m_strInput.IsEmpty())
	{
		OnbSEARCHHANJAWORD();
	}
			
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CDlgHanjaWordManager::GetReferenceDBInfo(VOID)
{
	CString strDMInfo;
	CFWDES DES;
	DES.Read_DES_File(
		_T("..\\CFG\\UseDMInfoChange.cfg"),
		_T("k^Ty"),
		strDMInfo
		);
	
	CString strMode = _T("");
	CString strDBInfoBefore = _T("");
	CString strDBInfoAfter = _T("");
	
	CString strAlias = _T("");
	CString strLine = _T("");
	
	for(int nCnt=0; nCnt < 3; nCnt++)
	{
		AfxExtractSubString(strLine, strDMInfo, nCnt, '\n');
		AfxExtractSubString(strAlias, strLine, 0, '=');
		
		strAlias.TrimLeft();
		strAlias.TrimRight();
		if(_T("MODE") == strAlias)
		{
			AfxExtractSubString(strMode, strLine, 1, '=');
			strMode.TrimLeft();
			strMode.TrimRight();
		}
		else if(_T("BEFORE") == strAlias)
		{
			AfxExtractSubString(strDBInfoBefore, strLine, 1, '=');
			strDBInfoBefore.TrimLeft();
			strDBInfoBefore.TrimRight();
		}
		else if(_T("AFTER") == strAlias)
		{
			AfxExtractSubString(strDBInfoAfter, strLine, 1, '=');
			strDBInfoAfter.TrimLeft();
			strDBInfoAfter.TrimRight();
		}			
	}
	if(_T("TRUE") == strMode)
	{
		CONNECTION_INFO = strDBInfoAfter;
	}
	else if(_T("FALSE") == strMode)
	{
		CONNECTION_INFO = strDBInfoBefore;
	}

	return 0;

}

VOID CDlgHanjaWordManager::OnbSEARCHHANJAWORD()
{
	UpdateData();
	CString strQuery;
	CString strOption;
	if (!m_strInput.IsEmpty())
		strOption.Format(_T(" AND FROM_DATA LIKE '%s%%'"), m_strInput);
	
	strQuery.Format(_T("SELECT REC_KEY, FROM_DATA, TO_DATA FROM %s WHERE ROWNUM < 1000 %s ORDER BY FROM_DATA"), m_strOriginTableName, strOption);
	if (m_DM_Origin.RestructDataManager(strQuery) < 0) return;

	strQuery.Format(_T("SELECT REC_KEY, FROM_DATA, TO_DATA FROM %s WHERE ROWNUM < 1000 %s ORDER BY FROM_DATA"), m_strUserTableName, strOption);
	if (m_DM_User.RestructDataManager(strQuery) < 0) return;

	INT nRowCount_Origin = m_DM_Origin.GetRowCount();
	INT nRowCount_User = m_DM_User.GetRowCount();
	
	m_Grid.Clear();
	m_Grid.SetRows(nRowCount_Origin + nRowCount_User + 1);
	m_Grid.SetCols(0, 4);
	m_Grid.SetColWidth(0, 0, 0);
	m_Grid.SetColumnInfo(0, _T("타입"), 60, 1);
	m_Grid.SetColumnInfo(1, _T("한글단어"), 120, 1);
	m_Grid.SetColumnInfo(2, _T("한자단어"), 120, 1);

	INT nRow = 1;

	for (INT idx = 0; idx < nRowCount_Origin; idx++)
	{
		m_Grid.SetTextMatrix(nRow, 0, m_DM_Origin.GetCellData(idx, 0));
		m_Grid.SetTextMatrix(nRow, 1, _T("원본"));
		m_Grid.SetTextMatrix(nRow, 2, m_DM_Origin.GetCellData(idx, 1));
		m_Grid.SetTextMatrix(nRow, 3, m_DM_Origin.GetCellData(idx, 2));

		nRow++;
	}

	for (idx = 0; idx < nRowCount_User; idx++)
	{
		m_Grid.SetTextMatrix(nRow, 0, m_DM_User.GetCellData(idx, 0));
		m_Grid.SetTextMatrix(nRow, 1, _T("사용자"));
		m_Grid.SetTextMatrix(nRow, 2, m_DM_User.GetCellData(idx, 1));
		m_Grid.SetTextMatrix(nRow, 3, m_DM_User.GetCellData(idx, 2));

		nRow++;
	}

}

BOOL CDlgHanjaWordManager::IsDupData(CString strFromData, CString strToData)
{
	if (strFromData.IsEmpty() || strToData.IsEmpty()) return TRUE;

	CString strQuery;
	CString strResult;
	strQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE FROM_DATA = '%s' AND TO_DATA = '%s'")
					,m_strOriginTableName, strFromData, strToData);

	m_DM_Origin.GetOneValueQuery(strQuery, strResult);
	if (_ttoi(strResult) > 0) return TRUE;

	strQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE FROM_DATA = '%s' AND TO_DATA = '%s'")
					,m_strUserTableName, strFromData, strToData);
	m_DM_User.GetOneValueQuery(strQuery, strResult);
	if (_ttoi(strResult) > 0) return TRUE;

	return FALSE;
}

VOID CDlgHanjaWordManager::OnbINPUTHANJAWORD() 
{
	CDlgHanjaWordInputModify dlg;
	dlg.m_nMode = MODE_INPUT;

	dlg.m_strInput1 = m_strInput;
	dlg.m_strInput2 = m_strInput;
	
	if (dlg.DoModal() != IDOK) return;

	if (IsDupData(dlg.m_strInput1, dlg.m_strInput2))
	{
		MessageBox(_T("중복되는 값이 있습니다. 없을 경우 파일 갱신을 해주세요"), _T("Warnning!"), MB_ICONEXCLAMATION);
		return;
	}
		
	INT ids = -1;
	CString strPKSeq;
	strPKSeq.Format(_T("%s.NEXTVAL"), m_DM_User.GetDefaultSEQName());

	ids = m_DM_User.StartFrame();
	if (ids < 0)
		return;

	m_DM_User.InitDBFieldData();
	m_DM_User.AddDBFieldData(_T("REC_KEY"),		_T("NUMERIC"),	strPKSeq);
	m_DM_User.AddDBFieldData(_T("FROM_DATA"),	_T("STRING"),	dlg.m_strInput1);
	m_DM_User.AddDBFieldData(_T("TO_DATA"),		_T("STRING"),	dlg.m_strInput2);
	m_DM_User.MakeInsertFrame(m_strUserTableName);
	ids = m_DM_User.SendFrame();
	if (ids < 0)
		return;

	m_DM_User.EndFrame();

	MessageBox(_T("입력하였습니다."), _T("Success!"), MB_ICONINFORMATION);
	OnbSEARCHHANJAWORD();
}

VOID CDlgHanjaWordManager::OnbMODIFYHANJAWORD() 
{
	CDlgHanjaWordInputModify dlg;
	dlg.m_nMode = MODE_MODIFY;
	

	dlg.m_strInput1 = m_Grid.GetTextMatrix(m_Grid.GetRow(), 2);
	dlg.m_strInput2 = m_Grid.GetTextMatrix(m_Grid.GetRow(), 3);

	if (dlg.DoModal() != IDOK) return;

	INT ids = -1;

	if (IsDupData(dlg.m_strInput1, dlg.m_strInput2))
	{
		MessageBox(_T("중복되는 값이 있습니다. 없을 경우 파일 갱신을 해주세요"), _T("Warnning!"), MB_ICONEXCLAMATION);
		return;
	}

	CESL_DataMgr *pDM = NULL;
	CString strTableName;
	if (m_Grid.GetTextMatrix(m_Grid.GetRow(), 1) == _T("원본"))
	{
		strTableName = m_strOriginTableName;
		pDM = &m_DM_Origin;
	}
	else
	{
		strTableName = m_strUserTableName;
		pDM = &m_DM_User;
	}

	CString strRecKey = m_Grid.GetTextMatrix(m_Grid.GetRow(), 0);
	if (strRecKey.IsEmpty()) return;

	
	ids = pDM->StartFrame();
	if (ids < 0)
			return;

	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("TO_DATA"),	_T("STRING"),	dlg.m_strInput2);
	pDM->MakeUpdateFrame(strTableName, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	ids = pDM->SendFrame();
	if (ids < 0)
		return;

	pDM->EndFrame();

	MessageBox(_T("수정하였습니다."), _T("Success!"), MB_ICONINFORMATION);
	OnbSEARCHHANJAWORD();
}

VOID CDlgHanjaWordManager::OnbDELETEHANJAWORD() 
{
	INT ids;

	CString strMsg;
	strMsg.Format(_T("%s : %s 를 삭제하시겠습니까?"), m_Grid.GetTextMatrix(m_Grid.GetRow(), 2), m_Grid.GetTextMatrix(m_Grid.GetRow(), 3));

	if (MessageBox(strMsg, _T("삭제확인"), MB_YESNO) != IDYES) return;

	CESL_DataMgr *pDM = NULL;
	CString strTableName;
	if (m_Grid.GetTextMatrix(m_Grid.GetRow(), 1) == _T("원본"))
	{
		strTableName = m_strOriginTableName;
		pDM = &m_DM_Origin;
	}
	else
	{
		strTableName = m_strUserTableName;
		pDM = &m_DM_User;
	}

	CString strRecKey = m_Grid.GetTextMatrix(m_Grid.GetRow(), 0);
	if (strRecKey.IsEmpty()) return;

	ids = pDM->StartFrame();
	if (ids < 0) return;
	pDM->MakeDeleteFrame(strTableName, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	ids = pDM->SendFrame();
	if (ids < 0) return;
	pDM->EndFrame();

	MessageBox(_T("삭제하였습니다."), _T("Success!"), MB_ICONINFORMATION);
	OnbSEARCHHANJAWORD();
}

INT CDlgHanjaWordManager::RefreshCharSetData(CESL_DataMgr *pDM, CString strManageRecKey, CString strFilePath, INT nVersion)
{
	if (pDM == NULL || strManageRecKey.IsEmpty() || strFilePath.IsEmpty()) return -1;

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

	CString strVersion;
	strVersion.Format(_T(".FILE_VERSION = %d\r\n"), nVersion);
	file.WriteString(strVersion);

	INT nRowCount = pDM->GetRowCount();

	CString strLine;
	CString strFromData;
	CString strToData;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strFromData = pDM->GetCellData(idx, 0);
		strToData = pDM->GetCellData(idx, 1);
		if (strFromData.IsEmpty() || strToData.IsEmpty()) continue;

		strLine.Format(_T("%-100s|%-100s\r\n"), strFromData, strToData);

		file.WriteString(strLine);
	}

	file.Close();

	INT nFileSize = 0;
	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
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

	INT ids = pDM->LOBUpdate(_T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strManageRecKey, _T("FILE_DATA"), pBuffer, nFileSize);
	if (ids < 0)
	{
		MessageBox(_T("변경적용 실패"));
		return -1;
	}
	
	ids = pDM->StartFrame();
	CString strQuery;
	strQuery.Format(_T("UPDATE CO_CHARSET_MANAGEMENT_TBL SET VERSION = %d, FILE_LEN = %d WHERE REC_KEY = %s;"), nVersion, nFileSize, strManageRecKey);
	pDM->AddFrame(strQuery);
	ids = pDM->SendFrame();
	if (ids < 0) 
	{
		MessageBox(_T("변경적용 실패했습니다."));
	}
	ids = pDM->EndFrame();
	if (ids < 0) 
	{
		MessageBox(_T("변경적용 실패했습니다."));
	}

	free(pBuffer);

	return 0;
}

VOID CDlgHanjaWordManager::OnbREFRESHUSER() 
{
	BeginWaitCursor();

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_DM_Origin.CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(_T("SELECT FROM_DATA, TO_DATA FROM %s"), m_strUserTableName);
	dm.RestructDataManager(strQuery);

	strQuery = _T("SELECT REC_KEY, VERSION, FILE_NAME FROM CO_CHARSET_MANAGEMENT_TBL WHERE CHARSET_NAME = '사용자 한자단어변환'");
	CString strResult[3];
	if (dm.GetValuesQuery(strQuery, strResult, 3) < 0) return;


	if (RefreshCharSetData(&dm, strResult[0], _T("..\\cfg\\") + strResult[2], _ttoi(strResult[1]) + 1) < 0) return;

	EndWaitCursor();

	MessageBox(_T("갱신 완료!"), _T("Success!"), MB_ICONINFORMATION);
}

VOID CDlgHanjaWordManager::OnbREFRESHORIGINAL() 
{
	if (MessageBox(_T("시간이 오래 걸릴수 있습니다. 계속하시겠습니까?"), _T("갱신 확인"), MB_YESNO) == IDNO) return;

	BeginWaitCursor();

	CString strFileName = _T("..\\CFG\\ESL_KOREAN_WORD_TO_CHINESE_WORD.CFG");

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_DM_Origin.CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(_T("SELECT FROM_DATA, TO_DATA FROM %s"), m_strOriginTableName);
	dm.RestructDataManager(strQuery);

	strQuery = _T("SELECT REC_KEY, VERSION, FILE_NAME FROM CO_CHARSET_MANAGEMENT_TBL WHERE CHARSET_NAME = '한자단어변환'");
	CString strResult[3];
	if (dm.GetValuesQuery(strQuery, strResult, 3) < 0) return;


	if (RefreshCharSetData(&dm, strResult[0], _T("..\\cfg\\") + strResult[2], _ttoi(strResult[1]) + 1) < 0) return;

	EndWaitCursor();

	MessageBox(_T("갱신 완료!"), _T("Success!"), MB_ICONINFORMATION);
}

BEGIN_EVENTSINK_MAP(CDlgHanjaWordManager, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgHanjaWordManager)
	ON_EVENT(CDlgHanjaWordManager, IDC_MSHFLEXGRID_HANJA_WORD, -605 /* MouseDown */, OnMouseDownMshflexgridHanjaWord, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDlgHanjaWordManager::OnMouseDownMshflexgridHanjaWord(short Button, short Shift, long x, long y) 
{
	m_Grid.SetReverse(m_Grid.GetRow() - 1);
}

BOOL CDlgHanjaWordManager::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbSEARCHHANJAWORD();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
