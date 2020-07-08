// DlgRptFileManage.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgRptFileManage.h"
#include "SIReportEditorView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgRptFileManageModify.h"
#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgRptFileManage dialog


CDlgRptFileManage::CDlgRptFileManage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRptFileManage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRptFileManage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_pDM = new CESL_DataMgr;

	m_pGrid = NULL;
	m_pGrid = new CESL_Grid(NULL);

	m_strOpenFileName = _T("");
}


VOID CDlgRptFileManage::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRptFileManage)
	DDX_Control(pDX, IDC_cSEARCH, m_cSearch);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgRptFileManage, CDialog)
	//{{AFX_MSG_MAP(CDlgRptFileManage)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bDESC, OnbDESC)
	ON_BN_CLICKED(IDC_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_bOPEN, OnbOPEN)
	ON_BN_CLICKED(IDC_bOPENFILE, OnbOPENFILE)
	ON_BN_CLICKED(IDC_bEXPORT, OnbEXPORT)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRptFileManage message handlers

BOOL CDlgRptFileManage::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_pDM) {
		AfxMessageBox(_T("Init DataManager Fail!"));
		return FALSE;
	}

	m_pDM->SetCONNECTION_INFO(m_sConnection);
	m_pGrid->SubclassDlgItem(IDC_MSHFLEXGRID, this);


	//GetDlgItem(IDC_eSEARCH)->SetWindowText(m_sUserID);
	m_cSearch.AddString(_T("제목"));
	m_cSearch.AddString(_T("사용자"));
	
	m_cSearch.SetCurSel(0);
	UpdateData(false);
		
	if (m_strOpenAlias.GetLength() > 0) SetWindowText(_T("리포트 파일 여는중..."));

	SetGridColumnInfo();
	
	SetTimer(0, 100, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// Operations

BOOL CDlgRptFileManage::SaveToFile(BYTE* pBuffer, INT nBufferSize, CString strFileName)
{
EFS_BEGIN

	if (pBuffer == NULL) return FALSE;

	CFile file;
	if( !file.Open(strFileName, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) ) 
	{
		free(pBuffer);
		AfxMessageBox(_T("Create Temp File Fail"));
		return FALSE;
	}
		
	file.WriteHuge(pBuffer, nBufferSize);
	free(pBuffer);
	file.Close();
	return TRUE;

EFS_END
return FALSE;

}

VOID CDlgRptFileManage::SetGridColumnInfo()
{
EFS_BEGIN

	m_pGrid->SetCols(0, 10);
	m_pGrid->SetColWidth(0, 0, 20*15);
	m_pGrid->SetColumnInfo(0, _T("IDX"), 60, 4);
	m_pGrid->SetColumnInfo(1, _T("업무단계"), 105, 1);
	m_pGrid->SetColumnInfo(2, _T("제목"), 155, 1);
	m_pGrid->SetColumnInfo(3, _T("작성자"), 80, 4);
	m_pGrid->SetColumnInfo(4, _T("설명"), 120, 1);
	m_pGrid->SetColumnInfo(5, _T("크기"), 60, 7);
	m_pGrid->SetColumnInfo(6, _T("처음 작업"), 100, 4);
	m_pGrid->SetColumnInfo(7, _T("마지막 작업"), 100, 4);
	m_pGrid->SetColumnInfo(8, _T("구분"), 50, 4);


EFS_END
}

VOID CDlgRptFileManage::WaitUser(BOOL flag)
{
EFS_BEGIN

	GetDlgItem(IDC_bSEARCH)->EnableWindow(flag);
	GetDlgItem(IDC_bDESC)->EnableWindow(flag);
	GetDlgItem(IDC_bDELETE)->EnableWindow(flag);
	GetDlgItem(IDC_bOPEN)->EnableWindow(flag);
	GetDlgItem(IDC_bOPENFILE)->EnableWindow(flag);
	GetDlgItem(IDC_bEXPORT)->EnableWindow(flag);
	GetDlgItem(IDC_bCLOSE)->EnableWindow(flag);
	GetDlgItem(IDC_bMODIFY)->EnableWindow(flag);

EFS_END
}

VOID CDlgRptFileManage::ErrorMessageBox(CString msg, INT ids)
{
EFS_BEGIN

	CString errormsg;
	errormsg.Format(_T("%s [%d]"), msg, ids);
	AfxMessageBox(errormsg);

EFS_END
}

VOID CDlgRptFileManage::OnDestroy() 
{
EFS_BEGIN

	CDialog::OnDestroy();
	
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}

	if (m_pGrid) {
		delete m_pGrid;
		m_pGrid = NULL;
	}

EFS_END
}

VOID CDlgRptFileManage::OnbSEARCH() 
{
EFS_BEGIN

	WaitUser(false);
	INT ids;
	CString sQuery;
	CString condition;
	UpdateData(true);
	GetDlgItem(IDC_eSEARCH)->GetWindowText(condition);
	CString strWhere;
	if (condition.GetLength() > 0) 
	{
		INT y = m_cSearch.GetCurSel();
		if (m_cSearch.GetCurSel() == 0) 
			strWhere.Format(_T("WHERE FLAG IN ('X', 'U', 'B') AND RPTALIAS LIKE '%%%s%%'"), condition);
		else
			strWhere.Format(_T("WHERE FLAG IN ('X', 'U', 'B') AND ENGINEER LIKE '%%%s%%'"), condition);
	} 
	else 
		strWhere.Empty();

	sQuery.Format(_T("SELECT PK, RPTALIAS, ENGINEER, DESCRIP, RPTSIZE, FIRST_DATE, LAST_DATE, FLAG, BITMAP_ALIAS, RPTGROUP FROM ESL_SIREPORT_TBL %s ORDER BY RPTALIAS, RPTGROUP"), strWhere);
	
	ids = m_pDM->RestructDataManager(sQuery);
	if (ids < 0) 
	{
		ErrorMessageBox(_T("DB에서 데이타를 가져오지 못하였습니다."), ids);
		WaitUser(true);
		return;
	}

	INT i, nRow;
	nRow = m_pDM->GetRowCount();
	if (nRow <= 0) {
		m_pGrid->RemoveAll();
		SetGridColumnInfo();
		AfxMessageBox(_T("검색된 데이타가 없습니다."));
		WaitUser(true);
		return;
	}
	m_pGrid->SetRows(nRow+1);
	CString strTmp;
	INT n = 0;
	for (i = 0; i < nRow; i++) 
	{
		if (m_pDM->GetCellData(i, 7) == _T("X") || m_pDM->GetCellData(i, 7) == _T("U"))		// Flag
		{
			n++;
			strTmp.Format(_T("%d"), i);
			m_pGrid->SetTextMatrix(n, 0, strTmp                   );		// DM row IDX
			m_pGrid->SetTextMatrix(n, 1, m_pDM->GetCellData(i, 0));		// Pk
			m_pGrid->SetTextMatrix(n, 2, m_pDM->GetCellData(i, 9));		// Rpt Group
			m_pGrid->SetTextMatrix(n, 3, m_pDM->GetCellData(i, 1));		// Title
			m_pGrid->SetTextMatrix(n, 4, m_pDM->GetCellData(i, 2));		// Engineer
			m_pGrid->SetTextMatrix(n, 5, m_pDM->GetCellData(i, 3));		// Description
			m_pGrid->SetTextMatrix(n, 6, m_pDM->GetCellData(i, 4));		// Report File Size
			m_pGrid->SetTextMatrix(n, 7, m_pDM->GetCellData(i, 5));		// First Work
			m_pGrid->SetTextMatrix(n, 8, m_pDM->GetCellData(i, 6));		// Last Work
			
			CString strFlag;

			if (m_pDM->GetCellData(i, 7) == _T("X"))
				strFlag = _T("원본");
			else if (m_pDM->GetCellData(i, 7) == _T("U"))
				strFlag = _T("사용자");

			m_pGrid->SetTextMatrix(n, 9, strFlag);		// FLAG
		}
	}

	m_pGrid->SetRows(n + 1);
	WaitUser(true);

EFS_END
}

VOID CDlgRptFileManage::OnbDESC() 
{
EFS_BEGIN

	CString desc;
	INT	ids = m_pDM->GetCellData(m_pGrid->GetRow()-1, 3, desc);
	if (ids < 0) {
		ErrorMessageBox(_T("설명을 가져오지 못했습니다."), ids);
		return;
	}
	AfxMessageBox(desc, MB_ICONINFORMATION);

EFS_END
}

VOID CDlgRptFileManage::OnbDELETE() 
{
EFS_BEGIN

	INT ids;

	//==================================================================================
    //2004-03-08 [이학중]
	CString strPK = m_pGrid->GetTextMatrix(m_pGrid->GetRow() , 1);
	CString strRptAlias = m_pGrid->GetTextMatrix(m_pGrid->GetRow() , 3);
	//==================================================================================
	
	CString msg;
	msg.Format(_T("[%s] 리포트 를 삭제 하시겠습니까?"), strRptAlias);
	if (AfxMessageBox(msg, MB_YESNO) != IDYES) return;

	CString strOption;
	if (((CSIReportEditorView*)GetParentFrame()->GetActiveView())->m_nEditorMode == USER_NORMAL)
		strOption = _T("AND FLAG = 'U'");

	CString sQuery;
	
	//==================================================================================
    //2004-06-14 [이학중]
	//sQuery.Format(_T("DELETE FROM ESL_SIREPORT_TBL WHERE RPTALIAS = '%s' %s;"), strRptAlias, strOption);
	//sQuery.Format(_T("DELETE FROM ESL_SIREPORT_TBL WHERE RPTALIAS = '%s' %s;"), strPK, strOption);
	sQuery.Format(_T("DELETE FROM ESL_SIREPORT_TBL WHERE PK = '%s' %s;"), strPK, strOption);
	//==================================================================================
	
	ids = m_pDM->ExecuteQuery(sQuery, 1);
	if (ids < 0) {
		ErrorMessageBox(_T("Delete Report Fail!"), ids);
		OnbSEARCH();
		return;
	}
	OnbSEARCH();
	return;	

EFS_END
}

VOID CDlgRptFileManage::OnbOPEN() 
{
EFS_BEGIN

	CString	strPK, strRptAlias, strBuffer;
	INT ids;
	INT row = _ttoi(m_pGrid->GetTextMatrix(m_pGrid->GetRow(), 0));
	ids = m_pDM->GetCellData(row, 0, strPK);
	ids = m_pDM->GetCellData(row, 1, strRptAlias);
	ids = m_pDM->GetCellData(row, 4, strBuffer);
	
	INT nRptSize = _ttoi(strBuffer);
	
	// Save XML File
	if (!SaveSIReportLOBData(strPK, nRptSize, m_strTempFileName))
		return;
	m_strOpenFileName = m_strTempFileName;

	// Save Bitmap File
	INT nCount = m_pDM->GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		if (m_pDM->GetCellData(i, 1) == strRptAlias && m_pDM->GetCellData(i, 7) == _T("B"))
		{
			CString strBmpFileName;
			strBmpFileName.Format(_T("%s\\%s.bmp"), EDITOR_TEMP_DIR, m_pDM->GetCellData(i, 8));

			SaveSIReportLOBData(m_pDM->GetCellData(i, 0), _ttoi(m_pDM->GetCellData(i, 4)), strBmpFileName);
		}
	}
	
	m_strTitle = strRptAlias;
	ids = m_pDM->GetCellData(row, 3, m_strDescription);
	ids = m_pDM->GetCellData(row, 9, m_strRptGroup);

	m_strReportPK = strPK;

	EndDialog(IDOK);

EFS_END
}

BOOL CDlgRptFileManage::SaveSIReportLOBData(CString strPK, INT nRptSize, CString strFileName)
{
EFS_BEGIN

	INT ids;
	INT nBufferSize = sizeof(BYTE) * nRptSize + 1024;
	BYTE *pBuffer = NULL;
	pBuffer = (BYTE*)malloc(nBufferSize);
	if (pBuffer == NULL)
	{
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return FALSE;
	}

	ids = m_pDM->LOBGet(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), pBuffer, &nBufferSize);
	if (ids < 0) {
		ErrorMessageBox(_T("Get Report Data Fail!"), ids);
		free(pBuffer);
		return FALSE;
	}
	if (nBufferSize <= 0) {
		AfxMessageBox(_T("Empty Report Data!"));
		return FALSE;
	}

	if (!SaveToFile(pBuffer, nBufferSize, strFileName))
		return FALSE;	

	return TRUE;

EFS_END
return FALSE;

}

VOID CDlgRptFileManage::OpenRPT(CString strRptAlias)
{
EFS_BEGIN

	if (strRptAlias.GetLength() == 0) return;
	WaitUser(false);
	
	INT ids;
	CString strBuffer;
	CString strPK;
	CString sQuery;

	CString strOption;

	sQuery.Format(_T("SELECT PK FROM ESL_SIREPORT_TBL WHERE RPTALIAS = '%s' AND FLAG = 'U'"), strRptAlias);
	ids = m_pDM->GetValuesQuery(sQuery, &strPK, 1);
	if (ids < 0 && ids != -4007)
	{
		EndDialog(IDCANCEL);
		return;
	}

	if (strPK.IsEmpty())
	{
		sQuery.Format(_T("SELECT PK FROM ESL_SIREPORT_TBL WHERE RPTALIAS = '%s' AND FLAG = 'X'"), strRptAlias);
		ids = m_pDM->GetOneValueQuery(sQuery, strPK);

		if (strPK.IsEmpty())
		{
			AfxMessageBox(_T("맞는 형식이 없습니다."));
			EndDialog(IDCANCEL);
			return;
		}
	}

	sQuery.Format(_T("SELECT PK, RPTSIZE, RPTALIAS, DESCRIP FROM ESL_SIREPORT_TBL WHERE PK=%s"), strPK);
	CString result[4];
	ids = m_pDM->GetValuesQuery(sQuery, result, 4);
	if (ids < 0) {
		AfxMessageBox(_T("Report 정보를 가져올 수 없습니다."));
		EndDialog(IDCANCEL);
		return;
	}

	// Save XML File
	if (!SaveSIReportLOBData(result[0], _ttoi(result[1]), m_strTempFileName))
	{
		EndDialog(IDCANCEL);
		return;
	}
	m_strOpenFileName = m_strTempFileName;

	// Save Bitmap File
	sQuery.Format(_T("SELECT PK, RPTALIAS, RPTSIZE, BITMAP_ALIAS FROM ESL_SIREPORT_TBL WHERE RPTALIAS = '%s' AND FLAG = 'B' "), strRptAlias);
	m_pDM->RestructDataManager(sQuery);

	INT nCount = m_pDM->GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		CString strBmpFileName;
		strBmpFileName.Format(_T("%s\\%s.bmp"), EDITOR_TEMP_DIR, m_pDM->GetCellData(i, 3));

		if (!SaveSIReportLOBData(m_pDM->GetCellData(i, 0), _ttoi(m_pDM->GetCellData(i, 2)), strBmpFileName))
		{
			EndDialog(IDCANCEL);
			return;
		}
	}

	m_strOpenFileName = m_strTempFileName;

	m_strTitle = result[2];
	m_strTitle.TrimLeft();		m_strTitle.TrimRight();
	
	m_strDescription = result[3];
	m_strDescription.TrimLeft();		m_strDescription.TrimRight();
	m_strReportPK = strPK;

	EndDialog(IDOK);

EFS_END
}

VOID CDlgRptFileManage::OnbOPENFILE() 
{
EFS_BEGIN

	TCHAR szbuf1[2048];
	GetCurrentDirectory(2047, szbuf1);

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");

	memset(&OFN, 0, sizeof(OPENFILENAME));

	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = _T("RPT파일(*.rpt)\0*.rpt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("RPT 파일 열기...");
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.lpstrDefExt = _T("rpt");
	OFN.Flags = OFN_OVERWRITEPROMPT;

	if (::GetOpenFileName(&OFN) != 0)
	{
		CString strFileName;
		strFileName = OFN.lpstrFile;
		SetCurrentDirectory(szbuf1);
			
		if (!strFileName.IsEmpty())
		{
			m_strOpenFileName = strFileName;
			EndDialog(IDOK);
		}
	}

EFS_END
}

VOID CDlgRptFileManage::OnbEXPORT() 
{
EFS_BEGIN

	CString szBuf, title, pk;
	INT ids, rptsize;
	
	INT row = _ttoi(m_pGrid->GetTextMatrix(m_pGrid->GetRow(), 0));

	ids = m_pDM->GetCellData(row, 4, szBuf);
	if (ids < 0) {
		ErrorMessageBox(_T("Get szBUFF Fail!"), ids);
		return;
	}
	ids = m_pDM->GetCellData(row, 1, title);
	if (ids < 0) {
		ErrorMessageBox(_T("Get TITLE Fail!"), ids);
		return;
	}
	ids = m_pDM->GetCellData(row, 0, pk);
	if (ids < 0) {
		ErrorMessageBox(_T("Get PK Fail!"), ids);
		return;
	}
	rptsize = _ttoi(szBuf);
	INT BufferSize = sizeof(BYTE)*rptsize + 1024;
	BYTE *pBuffer = NULL;
	pBuffer = (BYTE*)malloc(BufferSize);
	if (!pBuffer) {
		AfxMessageBox(_T("Malloc Memory Fail!"));
		return;
	}
	ids = m_pDM->LOBGet(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), pk, _T("RPTDATA"), pBuffer, &BufferSize);
	if (ids < 0) {
		ErrorMessageBox(_T("Get Report Data Fail!"), ids);
		free(pBuffer);
		return;
	}
	if (BufferSize <= 0) {
		AfxMessageBox(_T("Empty Report Data!"));
		return;
	}

	CString strExportFileName;
	title.TrimLeft();		title.TrimRight();
	strExportFileName.Format(_T("%s.rpt"), title);

	if (!SaveToFile(pBuffer, BufferSize, strExportFileName))
		return;

	CString msg;
	msg.Format(_T("Export successfully complete! [%s]"), strExportFileName);
	AfxMessageBox(msg);

EFS_END
}

BEGIN_EVENTSINK_MAP(CDlgRptFileManage, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgRptFileManage)
	ON_EVENT(CDlgRptFileManage, IDC_MSHFLEXGRID, -605 /* MouseDown */, OnMouseDownMshflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgRptFileManage, IDC_MSHFLEXGRID, -601 /* DblClick */, OnDblClickMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDlgRptFileManage::OnMouseDownMshflexgrid(short Button, short Shift, long x, long y) 
{
EFS_BEGIN

	m_pGrid->SetReverse(m_pGrid->GetRow()-1);

EFS_END
}

VOID CDlgRptFileManage::OnTimer(UINT nIDEvent) 
{
EFS_BEGIN

	if (nIDEvent == 0) {
		KillTimer(0);
		if (m_strOpenAlias.GetLength()) {
			OpenRPT(m_strOpenAlias);
		} else {
			OnbSEARCH();
		}
	}	
	CDialog::OnTimer(nIDEvent);

EFS_END
}

BOOL CDlgRptFileManage::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnbSEARCH();
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}		
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}

VOID CDlgRptFileManage::OnbCLOSE() 
{
EFS_BEGIN

	CDialog::OnCancel();

EFS_END
}

VOID CDlgRptFileManage::OnbMODIFY() 
{
EFS_BEGIN

	CDlgRptFileManageModify	dlg;

	INT ids, row;
	CString strPK;
	//row = m_pGrid->GetRow();
	row = _ttoi(m_pGrid->GetTextMatrix(m_pGrid->GetRow(), 0));
	ids = m_pDM->GetCellData(row, 0, strPK);
	ids = m_pDM->GetCellData(row, 1, dlg.m_sTITLE);
	ids = m_pDM->GetCellData(row, 3, dlg.m_sDESCRIPTION);
	ids = m_pDM->GetCellData(row, 9, dlg.m_strRptGroup);

	if (dlg.DoModal() != IDOK) return;

	CString sQuery;
	sQuery.Format(_T("UPDATE ESL_SIREPORT_TBL SET RPTALIAS='%s', DESCRIP='%s', RPTGROUP='%s' WHERE PK=%s;"), dlg.m_sTITLE, dlg.m_sDESCRIPTION, dlg.m_strRptGroup, strPK);
	ids = m_pDM->ExecuteQuery(sQuery, 1);
	if (ids < 0) {
		ErrorMessageBox(_T("수정에 실패하였습니다!"), ids);
	} else {
		AfxMessageBox(_T("수정되었습니다."));
	}
	OnbSEARCH();
	return;	

EFS_END
}

VOID CDlgRptFileManage::OnDblClickMshflexgrid() 
{
EFS_BEGIN

	OnbOPEN();	

EFS_END
}
