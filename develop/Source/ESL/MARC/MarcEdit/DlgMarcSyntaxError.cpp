// DlgMarcSyntaxError.cpp : implementation file
//

#include "stdafx.h"
#include "DlgMarcSyntaxError.h"
#include "MSHFlexGrid.h"
#include "efs.h"
#include "ESL_DataMgr.h"
#include "FWDES.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMarcSyntaxError dialog


CDlgMarcSyntaxError::CDlgMarcSyntaxError(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMarcSyntaxError::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMarcSyntaxError)
	m_bViewDescription = FALSE;
	m_strDesc = _T("");
	//}}AFX_DATA_INIT

	m_pMarcMgr = NULL;
	// KOL.UDF.022
	m_pWnd = NULL;
	m_nReversedRow = 0;
	m_strConnectionInfo = _T("");
}


VOID CDlgMarcSyntaxError::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMarcSyntaxError)
	DDX_Check(pDX, IDC_chkVIEW_DESCRIPTION, m_bViewDescription);
	DDX_Text(pDX, IDC_eDESCRIPTION, m_strDesc);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CDlgMarcSyntaxError, CDialog)
	//{{AFX_MSG_MAP(CDlgMarcSyntaxError)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_chkVIEW_DESCRIPTION, OnchkVIEWDESCRIPTION)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMarcSyntaxError message handlers

BOOL CDlgMarcSyntaxError::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	m_pWnd = pParentWnd;
	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

BOOL CDlgMarcSyntaxError::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	SetGridFormat();

	LoadConnectionInfo();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CDlgMarcSyntaxError::SetGridFormat()
{
	EFS_BEGIN

	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);
	
	pMSHFlexGrid->SetCols(0, 4);
	
	pMSHFlexGrid->SetColWidth(0, 0, 40 * 15);
	pMSHFlexGrid->SetColWidth(1, 0, 80 * 15);
	pMSHFlexGrid->SetColWidth(2, 0, 2000);
	pMSHFlexGrid->SetColWidth(3, 0, 4000);
	
	pMSHFlexGrid->SetColAlignmentFixed(0, 4);
	pMSHFlexGrid->SetColAlignmentFixed(1, 4);
	pMSHFlexGrid->SetColAlignmentFixed(2, 4);
	pMSHFlexGrid->SetColAlignmentFixed(3, 4);
	pMSHFlexGrid->SetColAlignment(0, 4);
	pMSHFlexGrid->SetColAlignment(1, 4);
	pMSHFlexGrid->SetColAlignment(2, 1);
	pMSHFlexGrid->SetColAlignment(3, 1);
	
	pMSHFlexGrid->SetTextMatrix(0, 0, _T("LINE"));
	pMSHFlexGrid->SetTextMatrix(0, 1, _T("TAG CODE"));
	pMSHFlexGrid->SetTextMatrix(0, 2, _T("ERROR"));
	pMSHFlexGrid->SetTextMatrix(0, 3, _T("HELP"));

	pMSHFlexGrid->SetRowHeight(-1, 350);

	EFS_END
}

INT CDlgMarcSyntaxError::LoadConnectionInfo()
{
	//++2009.01.07 UPDATE BY CJY {{++
	// 접속정보 암호화
	CString strDMInfo;
	CFWDES DES;		// 암복호화 클래스
	DES.Read_DES_File(	// 접속정보를 복호화하여 읽어온다.
		_T("..\\CFG\\UseDMInfoChange.cfg"),	// 접속정보 파일
		_T("k^Ty"),	// 암호키
		strDMInfo	// 복호화된 문자
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
		m_strConnectionInfo = strDBInfoAfter;
	}
	else if(_T("FALSE") == strMode)
	{
		m_strConnectionInfo = strDBInfoBefore;
	}

	return 0;
//DEL 	INT nFind = -1;
//DEL 	CStdioFile file;
//DEL 	if (!file.Open(_T("..\\cfg\\RefEditor\\refeditor_uni.cfg"), CFile::modeRead | CFile::typeBinary)) return -1;
//DEL 	CString strLine;
//DEL 	TCHAR cUni;
//DEL 	file.Read( &cUni, sizeof(TCHAR) );
//DEL 	if( 0xFEFF != cUni )
//DEL 	{
//DEL 		file.SeekToBegin();
//DEL 	}
//DEL 	
//DEL 	while (file.ReadString(strLine))
//DEL 	{
//DEL 		strLine.TrimLeft();
//DEL 		strLine.TrimRight();
//DEL 
//DEL 		nFind = strLine.Find(_T(" "));
//DEL 		if (nFind <= 0) continue;
//DEL 
//DEL 		if (strLine.Mid(0, nFind) != _T("HOST")) continue;
//DEL 
//DEL 		m_strConnectionInfo = strLine.Mid(nFind + 1);
//DEL 		break;
//DEL 	}
	//--2009.01.07 UPDATE BY CJY --}}
	return 0;
}

INT CDlgMarcSyntaxError::SetReverse(INT nRow)
{
	EFS_BEGIN

	INT i;
	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);

	COLORREF blue, white, black;
	blue = RGB(228,243,251);
	white = RGB(255,255,255);
	black = RGB(0,0,0);

    INT col;
    col = pMSHFlexGrid->GetCol();

	INT cols;
	cols = pMSHFlexGrid->GetCols(0);
	
	
	if ( m_nReversedRow >= 0 && m_nReversedRow < pMSHFlexGrid->GetRows()) {
		pMSHFlexGrid->SetRow(m_nReversedRow);
		for ( i = 1 ; i < cols ; i++ ) {
			pMSHFlexGrid->SetCol(i);
			pMSHFlexGrid->SetCellBackColor(white);
			pMSHFlexGrid->SetCellForeColor(black);
		}
	}
	if ( nRow >= 0 ) {
		pMSHFlexGrid->SetRow(nRow);
		for ( i = 1 ; i < cols ; i++ ) {
			pMSHFlexGrid->SetCol(i);
			pMSHFlexGrid->SetCellBackColor(blue);
			pMSHFlexGrid->SetCellForeColor(black);
		}
		m_nReversedRow = nRow;
	}

    pMSHFlexGrid->SetCol(col);

	return 0;

	EFS_END
	return -1;

}

VOID CDlgMarcSyntaxError::ShowDescription()
{
	if (!m_pMarcMgr) return;

	CString strTagCode;

	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);
	INT nRow = pMSHFlexGrid->GetRow();
	if (nRow < 1) return;

	CString strTagField = pMSHFlexGrid->GetTextMatrix(nRow, 1);
	strTagCode = strTagField.Left(3);

	m_strConnectionInfo.TrimLeft();
	m_strConnectionInfo.TrimRight();

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_strConnectionInfo);

	CString strQuery;
	strQuery.Format(_T("SELECT DESCRIP FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s' AND TAG_CODE = '%s'")
					, m_pMarcMgr->GetMarcGroup(), strTagCode);
	dm.GetOneValueQuery(strQuery, m_strDesc);

	//=====================================================
	//2008.12.09 ADD BY PJW : 멀티라인 에디트박스에 개행을 하기위해서 데이터 변경
	//						  1. \n\r -> \n : \n\r, \n 모두 존재할경우 대비
	//						  2. \n -> \n\r : 개행문자추가
	CString strData;
	strData.Format(_T("\n"));
	m_strDesc.Replace(_T("\r\n"),strData);	
	m_strDesc.Replace(_T("\r"),strData);	
	m_strDesc.Replace(strData,_T("\r\n"));
	//=====================================================

	UpdateData(FALSE);
}

VOID CDlgMarcSyntaxError::OnClose() 
{
	EFS_BEGIN

	ShowWindow(SW_HIDE);

	EFS_END
}

VOID CDlgMarcSyntaxError::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	// Description을 본다면 반반씩 넓이를 맞춘다.
	CRect rcWnd;
	GetClientRect(rcWnd);
	
	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);
	if (!pMSHFlexGrid || pMSHFlexGrid->GetSafeHwnd() == NULL) return;

	CEdit *pEdit= (CEdit*)GetDlgItem(IDC_eDESCRIPTION);
	if (!pEdit || pEdit->GetSafeHwnd() == NULL) return;

	INT nHeight;
	if (m_bViewDescription)
		nHeight = rcWnd.Height() / 2;
	else
		nHeight = rcWnd.Height();

	pMSHFlexGrid->MoveWindow(0, 20, rcWnd.Width(), nHeight - 20);

	if (m_bViewDescription)
	{
		pEdit->MoveWindow(0, nHeight, rcWnd.Width(), nHeight);
		pEdit->ShowWindow(SW_SHOW);
	}
	else
		pEdit->ShowWindow(SW_HIDE);

	EFS_END
}

VOID CDlgMarcSyntaxError::AddErrorMessage(INT nLine, CString strTag, CString strErrMsg, CString strHelpMsg)
{
	EFS_BEGIN

	nLine++;

	CString strLine;
	strLine.Format(_T("%d"), nLine);
	
	AddErrorMessage(strLine, strTag, strErrMsg, strHelpMsg);

	EFS_END
}

VOID CDlgMarcSyntaxError::AddErrorMessage(CString strLine, CString strTag, CString strErrMsg, CString strHelpMsg)
{
	EFS_BEGIN

	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);
	
	INT nRow;
	INT nRows = pMSHFlexGrid->GetRows();
	CString strBuffer;
	strBuffer = pMSHFlexGrid->GetTextMatrix(nRows - 1, 0);
	if (nRows == 2 && strBuffer.IsEmpty()) 
	{
		pMSHFlexGrid->SetRows(nRows);
		nRow = nRows - 1;
	}
	else
	{
		pMSHFlexGrid->SetRows(nRows + 1);
		nRow = nRows;
	}

	pMSHFlexGrid->SetTextMatrix(nRow , 0, strLine.GetBuffer(0));
	pMSHFlexGrid->SetTextMatrix(nRow , 1, strTag.GetBuffer(0));
	pMSHFlexGrid->SetTextMatrix(nRow , 2, strErrMsg.GetBuffer(0));
	pMSHFlexGrid->SetTextMatrix(nRow , 3, strHelpMsg.GetBuffer(0));

	pMSHFlexGrid->SetRowHeight(-1, 350);

	EFS_END
}

BEGIN_EVENTSINK_MAP(CDlgMarcSyntaxError, CDialog)
    //{{AFX_EVENTSINK_MAP(CDlgMarcSyntaxError)
	ON_EVENT(CDlgMarcSyntaxError, IDC_gridERROR, -605 /* MouseDown */, OnMouseDowngridERROR, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgMarcSyntaxError, IDC_gridERROR, -601 /* DblClick */, OnDblClickgridERROR, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CDlgMarcSyntaxError::OnMouseDowngridERROR(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	INT nRow;
	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);
	nRow = pMSHFlexGrid->GetRow();

	SetReverse(nRow);

	if (m_bViewDescription)
		ShowDescription();

	EFS_END
}

VOID CDlgMarcSyntaxError::OnDblClickgridERROR() 
{
	EFS_BEGIN

	INT nRow;
	CString strLine;
	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);
	nRow = pMSHFlexGrid->GetRow();
	strLine = pMSHFlexGrid->GetTextMatrix(nRow, 0);
	if (strLine.IsEmpty()) return;

	INT nLineNumber = _ttoi(strLine);

	nLineNumber--;
	m_pWnd->SendMessage(WM_LINE_LOCATION, (WPARAM)nLineNumber, 0);

	EFS_END
}

BOOL CDlgMarcSyntaxError::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	EFS_BEGIN

	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);
	INT nRowsScrolled = 1 * zDelta / 120;

    if (nRowsScrolled > 0)
        for (INT i = 0; i < nRowsScrolled; i++)
           pMSHFlexGrid->PostMessage(WM_VSCROLL, SB_LINEUP, 0);
    else
        for (INT i = 0; i > nRowsScrolled; i--)
            pMSHFlexGrid->PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);

	EFS_END
	return FALSE;

}

VOID CDlgMarcSyntaxError::RemoveAll()
{
	EFS_BEGIN

	CMSHFlexGrid *pMSHFlexGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridERROR);
	
	pMSHFlexGrid->Clear();
	
	SetGridFormat();
	pMSHFlexGrid->SetRows(2);

	EFS_END
}

VOID CDlgMarcSyntaxError::OnchkVIEWDESCRIPTION() 
{
	UpdateData();
	SendMessage(WM_SIZE);
}

// 다이얼로그 XP테마적용
VOID CDlgMarcSyntaxError::EnableThemeDialogTexture(HWND hwndDlg)
{
	HINSTANCE hDll=LoadLibrary(_T("UxTheme.dll"));	// 'UxTheme.dll'
	if(hDll==NULL)
		return;	// the DLL won't be available on anything except Windows XP
	ULONG (PASCAL *lpfnEnableTheme)(HWND, DWORD);
	(FARPROC&)lpfnEnableTheme=GetProcAddress(hDll,"EnableThemeDialogTexture");	// 'EnableThemeDialogTexture'
	if(lpfnEnableTheme)
		lpfnEnableTheme(hwndDlg, 6);	// ETDT_ENABLETAB = (ETDT_ENABLE | ETDT_USETABTEXTURE) = (2|4) = 6
	else ASSERT(FALSE);
	FreeLibrary(hDll);
}
