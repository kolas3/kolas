// PalViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "piclook.h"
#include "PalViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPalViewDlg dialog


CPalViewDlg::CPalViewDlg(CWnd* pParent, PLPixel32 * pPal)
	: CDialog(CPalViewDlg::IDD, pParent),
    m_pPal (pPal)
{
	//{{AFX_DATA_INIT(CPalViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPalViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPalViewDlg)
	DDX_Control(pDX, IDC_PALFRAME, m_PalFrameCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPalViewDlg, CDialog)
	//{{AFX_MSG_MAP(CPalViewDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalViewDlg message handlers

void CPalViewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
  int x,y;
  CRect Rect;
  CBrush Brush;
  for (x = 0; x < 16; x++)
    for (y = 0; y < 16; y++)
    {
      Rect = CRect (x*12, y*13, x*12+11, y*13+12);
      Rect.OffsetRect (14, 14);
      PLPixel32 Col = m_pPal[x*8+y];
      Brush.CreateSolidBrush (RGB (Col.GetR(), Col.GetG(), Col.GetB ()));
      dc.FillRect (Rect, &Brush);
      Brush.DeleteObject();
    }
}
