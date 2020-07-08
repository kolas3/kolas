// ConvertToHanjaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConvertToHanjaDlg.h"
#include "DlgHanjaWordManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConvertToHanjaDlg dialog


CConvertToHanjaDlg::CConvertToHanjaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertToHanjaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConvertToHanjaDlg)
	m_strInput = _T("");
	//}}AFX_DATA_INIT
	m_nPrevRow = -1;
	m_nPrevCol = -1;
}


void CConvertToHanjaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConvertToHanjaDlg)
	DDX_Text(pDX, IDC_eINPUT_HANJA_WORD, m_strInput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConvertToHanjaDlg, CDialog)
	//{{AFX_MSG_MAP(CConvertToHanjaDlg)
	ON_BN_CLICKED(IDC_bSEARCH_HANJA_WORD, OnbSEARCHHANJAWORD)
	ON_BN_CLICKED(IDC_bCONVERT_HANJA_WORD, OnbCONVERTHANJAWORD)
	ON_BN_CLICKED(IDC_bMANAGER_HANJA_WORD, OnbMANAGERHANJAWORD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConvertToHanjaDlg message handlers

BOOL CConvertToHanjaDlg::OnInitDialog() 
{
	try
	{
		CDialog::OnInitDialog();
		
		if (m_strInput.IsEmpty()) 
		{
			EndDialog(IDCANCEL);
			return FALSE;
		}
		
		CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_COM_MSHFLEXGRID);
		if (pGrid == NULL) return -1;
		
		pGrid->SetRowHeight(-1, 350);
		pGrid->SetColWidth(-1, 0, 2200);
		
		INT ids;
		
		ids = m_CharSetMgr.InitData(_T("KOREAN_WORD"), _T("CHINESE_WORD"));
		if (ids < 0)
		{
			EndDialog(IDCANCEL);
			return FALSE;
		}
		
		ids = FindHanja();
		if (ids < 0)
		{
			if (MessageBox(_T("맞는 한자가 없습니다. 새로 입력하시겠습니까?"), _T("한자로 변환"), MB_YESNO) == IDYES)
			{
				OnbMANAGERHANJAWORD();
			}
			else
			{
				EndDialog(IDCANCEL);
				return FALSE;
			}
		}
		
		
		return TRUE;
		
	}	
	catch (CEFSDMException *e) 
	{
		HandleException(e,__WFILE__, __LINE__);
	}
	catch (CException *e) 
	{
		HandleException(e, __WFILE__, __LINE__);
	}
	catch (...)
	{
		HandleException(__WFILE__, __LINE__, ::GetLastError());
	}
	
	return FALSE;
}

void CConvertToHanjaDlg::OnbCONVERTHANJAWORD() 
{
	EFS_BEGIN

	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_COM_MSHFLEXGRID);
	if (pGrid == NULL) return;

	m_strSelChi = pGrid->GetTextMatrix(pGrid->GetRow(), pGrid->GetCol());
	if (m_strSelChi.IsEmpty())
		EndDialog(IDCANCEL);
	else
		EndDialog(IDOK);

	EFS_END
}

INT CConvertToHanjaDlg::FindHanja()
{
	EFS_BEGIN

	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_COM_MSHFLEXGRID);
	if (pGrid == NULL) return -1;

	pGrid->Clear();

	CString strTo[128];
	INT nStrTo = 128;

	INT ids = m_CharSetMgr.FindKoreanWordToChineseWord(m_strInput, strTo, nStrTo);
	if (ids < 0 || nStrTo == 0)
		return -1;

	INT nCols = 2;
	INT nRows = 9;

	pGrid->SetCols(0, nCols);
	pGrid->SetRows(nStrTo > nRows ? nStrTo : nRows);

	for (INT i = 0; i < nStrTo; i ++)
	{
		pGrid->SetTextMatrix((i / nCols), (i % nCols), strTo[i]);
	}

	pGrid->SetRowHeight(-1, 350);
	pGrid->SetColWidth(-1, 0, 2200);

	return 0;

	EFS_END
	return -1;
}

void CConvertToHanjaDlg::OnbMANAGERHANJAWORD() 
{
	EFS_BEGIN

	CDlgHanjaWordManager dlg;
	if (!m_strInput.IsEmpty())
		dlg.m_strInput = m_strInput;

	dlg.DoModal();

	INT ids = m_CharSetMgr.InitData(_T("KOREAN_WORD"), _T("CHINESE_WORD"));
	if (ids >= 0)
	{
		FindHanja();	
	}

	EFS_END
}

void CConvertToHanjaDlg::OnbSEARCHHANJAWORD() 
{
	EFS_BEGIN

	UpdateData();
	FindHanja();
	
	EFS_END
}

BOOL CConvertToHanjaDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnbSEARCHHANJAWORD();
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;
}

BEGIN_EVENTSINK_MAP(CConvertToHanjaDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CConvertToHanjaDlg)	
	ON_EVENT(CConvertToHanjaDlg, IDC_COM_MSHFLEXGRID, 69 /* SelChange */, OnSelChangeComMshflexgrid, VTS_NONE)
	ON_EVENT(CConvertToHanjaDlg, IDC_COM_MSHFLEXGRID, -600 /* Click */, OnClickComMshflexgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CConvertToHanjaDlg::OnSelChangeComMshflexgrid() 
{
EFS_BEGIN

	CESL_Grid* pGrid = (CESL_Grid*)GetDlgItem(IDC_COM_MSHFLEXGRID);
	if( pGrid == NULL )		return;
	
	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();
	COLORREF blue, white;
	blue = RGB(228,243,251);
	white = RGB(255,255,255);
	
	if( m_nPrevRow >= 0 )
	{
		pGrid->SetRow(m_nPrevRow);
		pGrid->SetCol(m_nPrevCol);
		pGrid->SetCellBackColor(white);
		pGrid->SetCellFontBold(FALSE);
	}
	
	pGrid->SetRow(nRow);
	pGrid->SetCol(nCol);
	pGrid->SetCellBackColor(blue);
	pGrid->SetCellFontBold(TRUE);

	m_nPrevRow = nRow;
	m_nPrevCol = nCol;


EFS_END	
}

void CConvertToHanjaDlg::OnClickComMshflexgrid() 
{
	OnSelChangeComMshflexgrid();
}
