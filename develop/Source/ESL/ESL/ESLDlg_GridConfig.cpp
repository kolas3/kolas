// ESLDlg_GridConfig.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_GridConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ESL_UnicodeFileIO.h"
#include "ESL_MBCSFileIO.h"
#include "ESLDlg_GridConfig_INSERT.h"
#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_GridConfig dialog


CESLDlg_GridConfig::CESLDlg_GridConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_GridConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESLDlg_GridConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022
	INT i=0;
	USE_USERDEFINE = 0;
	nALIAS = 0;

	CM_ALIAS = _T("");
	GRID_ALIAS = _T("");
	DEFAULT_INIT_VALUE = _T("");
	DEFAULT_LINK_DATA = _T("");
	DM = _T("");
	USER_INIT_VALUE = _T("");
	USER_LINK_DATA = _T("");

	for(i=0;i<512;i++)
		ALIAS[i] = _T("");
}

CESLDlg_GridConfig::~CESLDlg_GridConfig(VOID)
{
	FreeList();
}

VOID CESLDlg_GridConfig::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_GridConfig)
	DDX_Control(pDX, IDC_COM_MSHFLEXGRID1, m_grid);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CESLDlg_GridConfig, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_GridConfig)
	ON_BN_CLICKED(IDC_COM_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_COM_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_COM_bINSERT, OnbINSERT)
	ON_BN_CLICKED(IDC_COM_bMODIFY, OnbMODIFY)
	ON_BN_CLICKED(IDC_COM_bDELETE, OnbDELETE)
	ON_BN_CLICKED(IDC_COM_bRIGHT, OnbRIGHT)
	ON_BN_CLICKED(IDC_COM_bLEFT, OnbLEFT)
	ON_BN_CLICKED(IDC_COM_RADIO1, OnComRadio1)
	ON_BN_CLICKED(IDC_COM_RADIO2, OnComRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_GridConfig message handlers

INT CESLDlg_GridConfig::SetColumnInfo(INT col, CString str, INT width, INT alignment)
{
EFS_BEGIN

	//if ( col > m_grid.GetCols(0)-2 ) return -1;

	m_grid.SetColAlignmentFixed(col, 4);
	m_grid.SetTextMatrix(0, col, str);
	m_grid.SetColWidth(col, 0, width*15);
	m_grid.SetColAlignment(col, alignment);
	
	UpdateData(false);
	return 0;

EFS_END
return -1;

}

INT CESLDlg_GridConfig::SetColumnInfo(CString init_value)	
{
EFS_BEGIN

	if (init_value.GetLength() == 0) return 0;
	CString column_info, sztemp;
	CString name, align, size;
	INT ialign, isize;
	INT first;
	for (INT i = 0; true; i++) {
		column_info.Empty();
		column_info = init_value.SpanExcluding(_T(","));
		if (column_info.GetLength() > 0) {
			name.Empty();
			align.Empty();
			size.Empty();
			first = 0;
			first = column_info.Find(':', 0);
			name = column_info.Mid(1, first-1);
			align = column_info.Mid(first+1, column_info.Find(':', first+1)-(first+1));
			first = column_info.Find(':', first+1);
			size = column_info.Mid(first+1, column_info.Find(']', 0)-(first+1));
			ialign = 4;
			isize = 50;
			if (align.GetLength()) ialign = _ttoi(align.GetBuffer(0));
			if (size.GetLength()) isize = _ttoi(size.GetBuffer(0));
			SetColumnInfo(i, name, isize, ialign);
		}
		if (column_info == init_value) break;
		sztemp = init_value.Mid(column_info.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		init_value = sztemp;
	}
	return 0;

EFS_END
return -1;

}

INT CESLDlg_GridConfig::SetColumnLink(CString link_data)	
{
EFS_BEGIN

	if (link_data.GetLength() == 0) return 0;
	CString column_info, sztemp;
	INT pos;
	for (INT i = 0; true; i++) {
		column_info.Empty();
		column_info = link_data.SpanExcluding(_T(","));
		column_info.TrimLeft();			column_info.TrimRight();
		if (column_info.GetLength() > 0) {
			pos = column_info.Find(_T("."), 0);
			m_grid.SetTextMatrix(1, i+1, column_info.Right(column_info.GetLength()- pos-1));
		}
		if (column_info == link_data) break;
		sztemp = link_data.Mid(column_info.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		link_data = sztemp;
	}
	UpdateData(false);
	return 0;

EFS_END
return -1;

}

BOOL CESLDlg_GridConfig::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	USER_INIT_VALUE = _T("");
	USER_LINK_DATA = _T("");
	UseUserDefine();
	INT cols;
	if (USER_INIT_VALUE.GetLength() > 0 && USER_LINK_DATA.GetLength() > 0) {
		cols =  CountLinkColumns(USER_INIT_VALUE);
	} else {
		cols =  CountLinkColumns(DEFAULT_INIT_VALUE);
	}
	m_grid.SetCols(0, cols+1);
	m_grid.SetRows(2);
	m_grid.SetColWidth(0, 0, 50*15);
	m_grid.SetTextMatrix(0, 0, _T("제목"));
	m_grid.SetTextMatrix(1, 0, _T("데이타"));
	UpdateData(false);
	if (USER_INIT_VALUE.GetLength() > 0 && USER_LINK_DATA.GetLength() > 0) {
		SetGridColumn(USER_INIT_VALUE, USER_LINK_DATA);
	} else {
		SetGridColumn(DEFAULT_INIT_VALUE, DEFAULT_LINK_DATA);
	}
	ShowGridColumn();
	UpdateData(true);
	if (m_grid.GetCols(0) > 1) {
		m_grid.SetRow(1);
		m_grid.SetCol(1);
		SetReverse(1);
		UpdateData(false);
	}
	ShowMode();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT CESLDlg_GridConfig::ShowGridColumn(VOID)
{
EFS_BEGIN

	INT count = COLLIST.GetCount();
	if (count == 0) return 0;
	m_grid.SetRows(2);
	UpdateData(false);
	GridColumn * pCol;
	POSITION pos;
	for (INT i = 0; i < count; i++) {
		pos = COLLIST.FindIndex(i);
		pCol = (GridColumn *)COLLIST.GetAt(pos);
		if (pCol) {
			SetColumnInfo(i+1, pCol->title, pCol->width, pCol->align);
			m_grid.SetTextMatrix(1, i+1, pCol->alias);
		}
	}
	UpdateData(false);
	return(count);


EFS_END
return -1;

}

INT CESLDlg_GridConfig::FreeList(VOID)
{
EFS_BEGIN

	INT count = COLLIST.GetCount();
	if (count == 0) return 0;
	GridColumn * col;
	for (INT i = 0; i < count; i++) {
		col = (GridColumn*)COLLIST.RemoveHead();
		if (col) delete col;
	}
	COLLIST.RemoveAll();
	return(count);

EFS_END
return -1;

}

INT CESLDlg_GridConfig::SetGridColumn(CString init_value, CString link_data)
{
EFS_BEGIN

	if (init_value.GetLength() == 0) return 0;
	FreeList();
	CString column_info, sztemp;
	CString name, align, size;
	INT ialign, isize;
	INT first;
	GridColumn *pCol;
	for (INT i = 0; true; i++) {
		column_info.Empty();
		column_info = init_value.SpanExcluding(_T(","));
		if (column_info.GetLength() > 0) {
			name.Empty();
			align.Empty();
			size.Empty();
			first = 0;
			first = column_info.Find(':', 0);
			name = column_info.Mid(1, first-1);
			align = column_info.Mid(first+1, column_info.Find(':', first+1)-(first+1));
			first = column_info.Find(':', first+1);
			size = column_info.Mid(first+1, column_info.Find(']', 0)-(first+1));
			ialign = 4;
			isize = 50;
			if (align.GetLength()) ialign = _ttoi(align.GetBuffer(0));
			if (size.GetLength()) isize = _ttoi(size.GetBuffer(0));
			pCol = NULL;
			pCol = new GridColumn;
			if (!pCol) continue;
			//SetColumnInfo(i, name, isize, ialign);
			pCol->title = name;
			pCol->width = isize;
			pCol->align = ialign;
			COLLIST.AddTail((CObject*)pCol);
		}
		if (column_info == init_value) break;
		sztemp = init_value.Mid(column_info.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		init_value = sztemp;
	}
	if (link_data.GetLength() == 0) return 0;
	INT pos;
	POSITION lPos;
	for (i = 0; true; i++) {
		pCol = NULL;
		column_info.Empty();
		column_info = link_data.SpanExcluding(_T(","));
		column_info.TrimLeft();			column_info.TrimRight();
		if (column_info.GetLength() > 0) {
			pos = column_info.Find(_T("."), 0);
			lPos = COLLIST.FindIndex(i);
			pCol = (GridColumn *)COLLIST.GetAt(lPos);
			if (pCol) pCol->alias = column_info.Right(column_info.GetLength()- pos-1);
			//m_grid.SetTextMatrix(1, i+1, column_info.Right(column_info.GetLength()- pos-1));
		}
		if (column_info == link_data) break;
		sztemp = link_data.Mid(column_info.GetLength()+1);
		if (sztemp.GetLength() == 0) break;
		link_data = sztemp;
	}
	return 0;

EFS_END
return -1;

}

INT CESLDlg_GridConfig::CountLinkColumns(CString init_value)
{	
EFS_BEGIN

	INT count = 0;
	if (init_value.GetLength() == 0) return 0;
	for (INT i = 0; i < init_value.GetLength(); i++) {
		if (init_value.GetAt(i) == ',') count++;
	}
	return count+1;

EFS_END
return -1;

}

// 사용자의 설정을 쓸건지 말것인지 결정하는 함수 
// true : 유져 설정을 쓸것
// false : 기본 설정을 쓸것
bool CESLDlg_GridConfig::UseUserDefine(VOID)
{
EFS_BEGIN

	USE_USERDEFINE = 0;
	CString filename;
	filename.Format(_T("..\\cfg\\grid\\%s_%s.cfg"), CM_ALIAS, GRID_ALIAS);
#ifdef _UNICODE
	CESL_UnicodeFileIO fd;
#elif _MBCS 
	CESL_MBCSFileIO fd;
#endif
	TCHAR szbuf[2048];
	INT ids, pLen, BufSize = 2047;
	ids = fd.Open(filename);
	if (ids < 0) return false;
	pLen = BufSize;
	ids = fd.GetFileLine(szbuf, &pLen, BufSize);
	if (ids == _TEOF) {
		fd.Close();
		return false;
	}
	if (szbuf[0] == '1') {
		USE_USERDEFINE = 1;
	} else {
		USE_USERDEFINE = 0;
	}
	ids = fd.GetFileLine(szbuf, &pLen, BufSize);
	if (ids == _TEOF) {
		fd.Close();
		return false;
	}
	USER_INIT_VALUE.Format(_T("%s"), szbuf);
	ids = fd.GetFileLine(szbuf, &pLen, BufSize);
	if (ids == _TEOF) {
		fd.Close();
		return false;
	}
	USER_LINK_DATA.Format(_T("%s"), szbuf);
	fd.Close();
	return true;

EFS_END
return FALSE;

}

VOID CESLDlg_GridConfig::ShowMode(VOID)
{
EFS_BEGIN

	CButton *b1 = (CButton*)GetDlgItem(IDC_COM_RADIO1);
	CButton *b2 = (CButton*)GetDlgItem(IDC_COM_RADIO2);

	if (USE_USERDEFINE == 1) {
		b1->SetCheck(false);
		b2->SetCheck(true);
	} else {
		b1->SetCheck(true);
		b2->SetCheck(false);
	}

	// 기본설정 사용 -> Diable
	// 사용자설정 사용 -> Enable
	INT nCTRL_ID = 6;
	UINT CTRL_ID[] =	{
							IDC_COM_MSHFLEXGRID1,
							IDC_COM_bLEFT,
							IDC_COM_bRIGHT,
							IDC_COM_bINSERT,
							IDC_COM_bMODIFY,
							IDC_COM_bDELETE
						};
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_COM_RADIO1);
	if (pButton1 == NULL) return;
	BOOL IsEnable = TRUE;
	if (pButton1->GetCheck() == TRUE) {
		IsEnable = FALSE;
	} else {
		IsEnable = TRUE;
	}

	CWnd *pWnd;
	for (INT i = 0; i < nCTRL_ID; i++) {
		pWnd = GetDlgItem(CTRL_ID[i]);
		if (pWnd) pWnd->EnableWindow(IsEnable);
	}
EFS_END
}

VOID CESLDlg_GridConfig::OnbSAVE() 
{
EFS_BEGIN

	CButton *b1 = (CButton*)GetDlgItem(IDC_COM_RADIO1);
	INT tmpUSE_USERDEFINE;
	if (b1->GetCheck()) {
		tmpUSE_USERDEFINE = 0;
	} else {
		tmpUSE_USERDEFINE = 1;
	}
	UpdateData(true);
	CString tmpINIT_VALUE, tmpLINK_DATA;
	CString str, tmp;
	INT width, alignment;
	INT nCols = m_grid.GetCols(0);
	INT i;
	tmpINIT_VALUE = _T("");
	tmpLINK_DATA = _T("");
	for (i = 1; i < nCols; i++) {
		str = m_grid.GetTextMatrix(0, i);
		width = m_grid.GetColWidth(i, 0)/15;
		alignment = m_grid.GetColAlignment(i);
		
		if (tmpINIT_VALUE.GetLength() == 0) tmp.Format(_T("[%s:%d:%d]"), str, alignment, width);
			else tmp.Format(_T(",[%s:%d:%d]"), str, alignment, width);
		tmpINIT_VALUE += tmp;

		str = m_grid.GetTextMatrix(1, i);
		if (str.GetLength() > 0) {
			tmp.Format(_T(":%s.%s"), DM,str);
		} else {
			tmp = _T("");
		}
		if (tmpLINK_DATA.GetLength() == 0) {
			tmpLINK_DATA = tmp;
		} else {
			tmpLINK_DATA = tmpLINK_DATA + _T(",") + tmp;
		}
		if (tmpLINK_DATA.GetLength() == 0 && i == 1) {
			tmpLINK_DATA = _T(" ");
		}
	}
	tmpLINK_DATA.TrimLeft();

	CString filename;
	filename.Format(_T("..\\cfg\\grid\\%s_%s.cfg"), CM_ALIAS, GRID_ALIAS);

	if ( tmpUSE_USERDEFINE == 0 )
		{
		CFileFind ff;
		if ( ff.FindFile(filename) )
		{
			CFile::Remove(filename);
		}		
		MessageBox(_T("기본설정 상태로 저장하였습니다."), _T("Warnning!"), MB_ICONEXCLAMATION);
		CDialog::OnOK();	
		return;

	}


	FILE *fd;
	fd = _tfopen(filename.GetBuffer(0), _T("wb"));
	if (!fd) {
		MessageBox(_T("저장에 실패하였습니다."), _T("Warnning!"), MB_ICONEXCLAMATION);
		return;
	}
		/*UNCHANGE*/fputc(0xFF, fd);
		/*UNCHANGE*/fputc(0xFE, fd);
	
	_ftprintf(fd, _T("%d\r\n"), tmpUSE_USERDEFINE);
	_ftprintf(fd, _T("%s\r\n"), tmpINIT_VALUE);
	_ftprintf(fd, _T("%s\r\n"), tmpLINK_DATA);
	fclose(fd);
	MessageBox(_T("저장하였습니다."), _T("Warnning!"), MB_ICONEXCLAMATION);

EFS_END
}

VOID CESLDlg_GridConfig::OnbCLOSE() 
{
EFS_BEGIN

	CDialog::OnOK();	

EFS_END
}

VOID CESLDlg_GridConfig::OnbINSERT() 
{
EFS_BEGIN

	UpdateData(true);
	CESLDlg_GridConfig_INSERT dlg;
	INT i;
	dlg.nALIAS = nALIAS;
	for (i = 0; i < nALIAS; i++) dlg.ALIAS[i] = ALIAS[i];
	dlg.m_maxCol = m_grid.GetCols(0);
	if (dlg.DoModal() != IDOK) return;

	UpdateData(true);
	m_grid.SetCols(0, m_grid.GetCols(0) + 1);
	UpdateData(false);

	GridColumn * pCol = new GridColumn;
	if (!pCol) return;
	INT col = dlg.rCOL;
	pCol->title = dlg.m_eTITLE;
	pCol->align = dlg.rALIGN;	
	pCol->width = dlg.m_eWidth;
	pCol->alias = dlg.rALIAS;

	COLLIST.AddTail((CObject*)pCol);
	/*
	POSITION pos;
	if (col == 1) {
		pos = COLLIST.FindIndex(0);
		COLLIST.InsertBefore(pos, (CObject*)pCol);
	} else {
		pos = COLLIST.FindIndex(col-2);
		COLLIST.InsertAfter(pos, (CObject*)pCol);
	}
	*/
	INT nCol = COLLIST.GetCount();
	m_grid.SetCols(0, nCol+1);
	SetReverse(nCol);
	UpdateData(false);
	ShowGridColumn();

EFS_END
}

VOID CESLDlg_GridConfig::OnbMODIFY() 
{
EFS_BEGIN

	UpdateData(true);
	INT col = m_grid.GetCol();
	if (col == 0) return;
	POSITION pos = 	COLLIST.FindIndex(col-1);
	GridColumn * pCol = (GridColumn *)COLLIST.GetAt(pos);
	if (!pCol) return;

	INT i;
	CESLDlg_GridConfig_INSERT dlg;
	dlg.nALIAS = nALIAS;
	for (i = 0; i < nALIAS; i++) dlg.ALIAS[i] = ALIAS[i];
	dlg.m_maxCol = m_grid.GetCols(0);
	dlg.mode = 2;
	dlg.rALIAS = pCol->alias;
	dlg.rALIGN = pCol->align;
	dlg.rCOL = col;
	dlg.m_eWidth = pCol->width;
	dlg.m_eTITLE = pCol->title;
	
	if (dlg.DoModal() != IDOK) return;
	pCol->title = dlg.m_eTITLE;
	pCol->align = dlg.rALIGN;	
	pCol->width = dlg.m_eWidth;
	pCol->alias = dlg.rALIAS;
	
	SetReverse(col);
	ShowGridColumn();

EFS_END
}

VOID CESLDlg_GridConfig::OnbDELETE() 
{
EFS_BEGIN

	UpdateData(true);
	INT col = m_grid.GetCol();	
	if (col == 0) return;

	POSITION pos = COLLIST.FindIndex(col-1);
	GridColumn * pCol;
	pCol = (GridColumn *)COLLIST.GetAt(pos);
	if (pCol) {
		delete pCol;
		pCol = NULL;
	}
	COLLIST.RemoveAt(pos);
	INT nCol = COLLIST.GetCount();
	m_grid.SetCols(0, nCol+1);
	UpdateData(false);
	ShowGridColumn();

EFS_END
}

VOID CESLDlg_GridConfig::OnbRIGHT() 
{
EFS_BEGIN

	UpdateData(true);
	INT col = m_grid.GetCol();	
	if (col == 0) return;
	INT nCols = m_grid.GetCols(0);
	if (col+1 > nCols) return;

	GridColumn * pCol = NULL;
	POSITION pos;
	pos = COLLIST.FindIndex(col-1);
	pCol = (GridColumn *)COLLIST.GetAt(pos);
	COLLIST.RemoveAt(pos);
	if (!pCol) return;

	col = col + 1;
	if (col == 1) {
		pos = COLLIST.FindIndex(0);
		COLLIST.InsertBefore(pos, (CObject*)pCol);
	} else {
		pos = COLLIST.FindIndex(col-2);
		COLLIST.InsertAfter(pos, (CObject*)pCol);
	}
	m_grid.SetCol(col);
	SetReverse(col);
	UpdateData(false);
	ShowGridColumn();	

EFS_END
}

VOID CESLDlg_GridConfig::OnbLEFT() 
{
EFS_BEGIN

	UpdateData(true);
	INT col = m_grid.GetCol();	
	if (col == 0) return;
	INT nCols = m_grid.GetCols(0);
	if (col-1 == 0) return;

	GridColumn * pCol = NULL;
	POSITION pos;
	pos = COLLIST.FindIndex(col-1);
	pCol = (GridColumn *)COLLIST.GetAt(pos);
	COLLIST.RemoveAt(pos);
	if (!pCol) return;

	col = col - 1;
	if (col == 1) {
		pos = COLLIST.FindIndex(0);
		COLLIST.InsertBefore(pos, (CObject*)pCol);
	} else {
		pos = COLLIST.FindIndex(col-2);
		COLLIST.InsertAfter(pos, (CObject*)pCol);
	}
	m_grid.SetCol(col);
	SetReverse(col);
	UpdateData(false);
	ShowGridColumn();	

EFS_END
}

INT CESLDlg_GridConfig::SetReverse(INT col)
{
EFS_BEGIN

	INT nCols = m_grid.GetCols(0);

	COLORREF blue, white, black;
	//blue = RGB(43,79,119);			
	blue = RGB(228,243,251);			//E4F3FB
	white = RGB(255,255,255);
	black = RGB(0,0,0);

	INT i;
	m_grid.SetRow(1);
	for (i = 1; i < nCols; i++) {
			m_grid.SetCol(i);
			if (i == col) {
				m_grid.SetCellBackColor(blue);
				m_grid.SetCellForeColor(black);
			} else {
				m_grid.SetCellBackColor(white);
				m_grid.SetCellForeColor(black);
			}
	}
	
	m_grid.SetCol(col);
	UpdateData(false);
	return 0;

EFS_END
return -1;

}

BEGIN_EVENTSINK_MAP(CESLDlg_GridConfig, CDialog)
    //{{AFX_EVENTSINK_MAP(CESLDlg_GridConfig)
	ON_EVENT(CESLDlg_GridConfig, IDC_COM_MSHFLEXGRID1, 69 /* SelChange */, OnSelChangeMshflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CESLDlg_GridConfig::OnSelChangeMshflexgrid1() 
{
EFS_BEGIN

	CMSHFlexGrid *pGrid = (CMSHFlexGrid *)GetDlgItem(IDC_COM_MSHFLEXGRID1);
	SetReverse(pGrid->GetCol());

EFS_END
}

VOID CESLDlg_GridConfig::OnComRadio1() 
{
	// 기본설정 사용 -> Diable
	// 사용자설정 사용 -> Enable
	INT nCTRL_ID = 6;
	UINT CTRL_ID[] =	{
							IDC_COM_MSHFLEXGRID1,
							IDC_COM_bLEFT,
							IDC_COM_bRIGHT,
							IDC_COM_bINSERT,
							IDC_COM_bMODIFY,
							IDC_COM_bDELETE
						};
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_COM_RADIO1);
	if (pButton1 == NULL) return;
	BOOL IsEnable = TRUE;
	if (pButton1->GetCheck() == TRUE) {
		IsEnable = FALSE;
	} else {
		IsEnable = TRUE;
	}

	CWnd *pWnd;
	for (INT i = 0; i < nCTRL_ID; i++) {
		pWnd = GetDlgItem(CTRL_ID[i]);
		if (pWnd) pWnd->EnableWindow(IsEnable);
	}
}

VOID CESLDlg_GridConfig::OnComRadio2() 
{
	// 기본설정 사용 -> Diable
	// 사용자설정 사용 -> Enable
	INT nCTRL_ID = 6;
	UINT CTRL_ID[] =	{
							IDC_COM_MSHFLEXGRID1,
							IDC_COM_bLEFT,
							IDC_COM_bRIGHT,
							IDC_COM_bINSERT,
							IDC_COM_bMODIFY,
							IDC_COM_bDELETE
						};
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_COM_RADIO1);
	if (pButton1 == NULL) return;
	BOOL IsEnable = TRUE;
	if (pButton1->GetCheck() == TRUE) {
		IsEnable = FALSE;
	} else {
		IsEnable = TRUE;
	}

	CWnd *pWnd;
	for (INT i = 0; i < nCTRL_ID; i++) {
		pWnd = GetDlgItem(CTRL_ID[i]);
		if (pWnd) pWnd->EnableWindow(IsEnable);
	}
}
