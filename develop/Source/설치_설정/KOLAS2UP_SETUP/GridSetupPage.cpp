// GridSetupPage.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS2UP_SETUP.h"
#include "GridSetupPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridSetupPage property page

IMPLEMENT_DYNCREATE(CGridSetupPage, CPropertyPage)

CGridSetupPage::CGridSetupPage() : CPropertyPage(CGridSetupPage::IDD)
{
	//{{AFX_DATA_INIT(CGridSetupPage)
	m_nHeight = 0;
	//}}AFX_DATA_INIT

	m_strFileName = _T("..\\CFG\\Grid\\_Grid.cfg");
	m_strISBOLD = _T("보통");
	m_nHeight = 9;
	m_nLINECOLOR_R=192;		m_nLINECOLOR_G=192;		m_nLINECOLOR_B=192;
	m_cLINECOLOR = RGB(m_nLINECOLOR_R, m_nLINECOLOR_G, m_nLINECOLOR_B);
	m_nBACKCOLOR_R=255;		m_nBACKCOLOR_G=255;		m_nBACKCOLOR_B=255;
	m_cBACKCOLOR = RGB(m_nBACKCOLOR_R, m_nBACKCOLOR_G, m_nBACKCOLOR_B);
	m_nFontSize = 9;
	m_nFONTCOLOR_R = 0;		m_nFONTCOLOR_G=0;		m_nFONTCOLOR_B=0;
}

CGridSetupPage::~CGridSetupPage()
{
}

VOID CGridSetupPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridSetupPage)
	DDX_Control(pDX, IDC_bCH_BACKCOLOR, m_bCH_BACKCOLOR);
	DDX_Control(pDX, IDC_bCH_LINECOLOR, m_bCH_LINECOLOR);
	DDX_Control(pDX, IDC_cISBOLD, m_cISBOLD);
	DDX_Text(pDX, IDC_eHEIGHT, m_nHeight);
	DDV_MinMaxUInt(pDX, m_nHeight, 5, 1000);
	DDX_Text(pDX, IDC_eFONT, m_strFont);
	//}}AFX_DATA_MAP
	DDX_ColorPicker(pDX, IDC_bCH_BACKCOLOR, m_cBACKCOLOR);
	DDX_ColorPicker(pDX, IDC_bCH_LINECOLOR, m_cLINECOLOR);
}


BEGIN_MESSAGE_MAP(CGridSetupPage, CPropertyPage)
	//{{AFX_MSG_MAP(CGridSetupPage)
	ON_BN_CLICKED(IDC_bCH_LINEFONT, OnbCHLINEFONT)
	ON_BN_CLICKED(IDC_bCH_LINECOLOR, OnbCHLINECOLOR)
	ON_BN_CLICKED(IDC_bCH_BACKCOLOR, OnbCHBACKCOLOR)
	ON_NOTIFY(UDN_DELTAPOS, IDC_sHEIGHT, OnDeltapossHEIGHT)
	ON_CBN_SELCHANGE(IDC_cISBOLD, OnSelchangecISBOLD)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridSetupPage Operations
INT CGridSetupPage::ReadConfigFile(VOID)
{
	CStdioFile fd;
	if (!fd.Open(m_strFileName, CFile::modeRead | CFile::typeBinary)) {
		AfxMessageBox(_T("설정파일을 열수없습니다.[..\\CFG\\Grid\\_Grid.cfg]"));
		return -1;
	}

	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString str;
	CString strHeader, strData;
	INT pos;
	while(TRUE) {
		str = _T("");
		if (fd.ReadString(str) == FALSE) break;
		if (str.GetLength() == 0) break;
		strHeader = _T("");			strData = _T("");
		pos = str.Find('=', 0);
		if (pos == -1) continue;
		strHeader = str.Left(pos);
		strData = str.Right(str.GetLength() - pos-1);

		strHeader.TrimLeft();		strHeader.TrimRight();			//strHeader.MakeUpper();
		strData.TrimLeft();		strData.TrimRight();			//strData.MakeUpper();

		SetData(strHeader, strData);
	}
	fd.Close();
	return 0;
}

INT CGridSetupPage::SetData(CString strHeader, CString strData)
{
	if (strHeader == _T("TITLEBOLD")) m_strISBOLD = strData;
	if (strHeader == _T("LINEHEIGHT")) m_nHeight = _ttoi(strData);

	if (strHeader == _T("FONT_SIZE")) m_nFontSize = _ttoi(strData);
	if (strHeader == _T("FONTCOLOR_R")) m_nFONTCOLOR_R = _ttoi(strData);
	if (strHeader == _T("FONTCOLOR_G")) m_nFONTCOLOR_G = _ttoi(strData);
	if (strHeader == _T("FONTCOLOR_B")) m_nFONTCOLOR_B = _ttoi(strData);
	if (strHeader == _T("FONT_lfHeight")) m_fFont.lfHeight = _ttoi(strData);
	if (strHeader == _T("FONT_lfWidth")) m_fFont.lfWidth = _ttoi(strData);
	if (strHeader == _T("FONT_lfEscapement")) m_fFont.lfEscapement = _ttoi(strData);
	if (strHeader == _T("FONT_lfOrientation")) m_fFont.lfOrientation = _ttoi(strData);
	if (strHeader == _T("FONT_lfWeight")) m_fFont.lfWeight = _ttoi(strData);
	if (strHeader == _T("FONT_lfItalic")) m_fFont.lfItalic = _ttoi(strData);
	if (strHeader == _T("FONT_lfUnderline")) m_fFont.lfUnderline = _ttoi(strData);
	if (strHeader == _T("FONT_lfStrikeOut")) m_fFont.lfStrikeOut = _ttoi(strData);
	if (strHeader == _T("FONT_lfCharSet")) m_fFont.lfCharSet = _ttoi(strData);
	if (strHeader == _T("FONT_lfOutPrecision")) m_fFont.lfOutPrecision = _ttoi(strData);
	if (strHeader == _T("FONT_lfClipPrecision")) m_fFont.lfClipPrecision = _ttoi(strData);
	if (strHeader == _T("FONT_lfQuality")) m_fFont.lfQuality = _ttoi(strData);
	if (strHeader == _T("FONT_lfPitchAndFamily")) m_fFont.lfPitchAndFamily = _ttoi(strData);
	if (strHeader == _T("FONT_lfFaceName")) _stprintf(m_fFont.lfFaceName, _T("%s"), strData);

	if (strHeader == _T("LINECOLOR_R")) m_nLINECOLOR_R = _ttoi(strData);
	if (strHeader == _T("LINECOLOR_G")) m_nLINECOLOR_G = _ttoi(strData);
	if (strHeader == _T("LINECOLOR_B")) m_nLINECOLOR_B = _ttoi(strData);

	if (strHeader == _T("BACKCOLOR_R")) m_nBACKCOLOR_R = _ttoi(strData);
	if (strHeader == _T("BACKCOLOR_G")) m_nBACKCOLOR_G = _ttoi(strData);
	if (strHeader == _T("BACKCOLOR_B")) m_nBACKCOLOR_B = _ttoi(strData);

	return 0;
}

VOID CGridSetupPage::ShowFont(VOID)
{
	m_strFont.Format(_T("%s, 크기 %d"), m_fFont.lfFaceName, m_nFontSize);
	UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CGridSetupPage message handlers
BOOL CGridSetupPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	if (ReadConfigFile() < 0) {
		return FALSE;
	}

	m_cISBOLD.ResetContent();
	m_cISBOLD.AddString(_T("보통"));
	m_cISBOLD.AddString(_T("진하게"));
	if (m_strISBOLD == _T("진하게")) m_cISBOLD.SetCurSel(1);
		else m_cISBOLD.SetCurSel(0); 
	ShowFont();
	UpdateData(FALSE);

	m_cLINECOLOR = RGB(m_nLINECOLOR_R, m_nLINECOLOR_G, m_nLINECOLOR_B);
	m_cBACKCOLOR = RGB(m_nBACKCOLOR_R, m_nBACKCOLOR_G, m_nBACKCOLOR_B);
	m_bCH_BACKCOLOR.SetBkColor(m_cBACKCOLOR);
	m_bCH_LINECOLOR.SetBkColor(m_cLINECOLOR);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGridSetupPage::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);

	CStdioFile fd;
	if( !fd.Open(m_strFileName, CFile::modeWrite|CFile::modeCreate | CFile::typeBinary) ) {
		AfxMessageBox(_T("설정파일을 열수없습니다.[..\\CFG\\Grid\\_Grid.cfg]"));
		return FALSE;
	}

	if( 0 == fd.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fd.Write( &cUni, sizeof(TCHAR));
	}	
		
	CString str;
	CString strHeader, strData;

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("TITLEBOLD");
	m_cISBOLD.GetWindowText(strData);
	strData.TrimLeft();			strData.TrimRight();
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("LINEHEIGHT");
	strData.Format(_T("%d"), m_nHeight);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_SIZE");
	strData.Format(_T("%d"), m_nFontSize);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONTCOLOR_R");
	strData.Format(_T("%d"), m_nFONTCOLOR_R);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONTCOLOR_G");
	strData.Format(_T("%d"), m_nFONTCOLOR_G);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONTCOLOR_B");
	strData.Format(_T("%d"), m_nFONTCOLOR_B);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

// (START) - Font
	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfHeight");
	strData.Format(_T("%d"), m_fFont.lfHeight);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfWidth");
	strData.Format(_T("%d"), m_fFont.lfWidth);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfEscapement");
	strData.Format(_T("%d"), m_fFont.lfEscapement);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfOrientation");
	strData.Format(_T("%d"), m_fFont.lfOrientation);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfWeight");
	strData.Format(_T("%d"), m_fFont.lfWeight);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfItalic");
	strData.Format(_T("%d"), m_fFont.lfItalic);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfUnderline");
	strData.Format(_T("%d"), m_fFont.lfUnderline);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfStrikeOut");
	strData.Format(_T("%d"), m_fFont.lfStrikeOut);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfCharSet");
	strData.Format(_T("%d"), m_fFont.lfCharSet);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfOutPrecision");
	strData.Format(_T("%d"), m_fFont.lfOutPrecision);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfClipPrecision");
	strData.Format(_T("%d"), m_fFont.lfClipPrecision);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfQuality");
	strData.Format(_T("%d"), m_fFont.lfQuality);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfPitchAndFamily");
	strData.Format(_T("%d"), m_fFont.lfPitchAndFamily);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("FONT_lfFaceName");
	strData.Format(_T("%s"), m_fFont.lfFaceName);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);		fd.WriteString(str);

// (FINISH) - Font

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("LINECOLOR_R");
	strData.Format(_T("%d"), m_nLINECOLOR_R);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("LINECOLOR_G");
	strData.Format(_T("%d"), m_nLINECOLOR_G);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("LINECOLOR_B");
	strData.Format(_T("%d"), m_nLINECOLOR_B);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("BACKCOLOR_R");
	strData.Format(_T("%d"), m_nBACKCOLOR_R);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("BACKCOLOR_G");
	strData.Format(_T("%d"), m_nBACKCOLOR_G);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	str = _T("");		strHeader = _T("");		strData = _T("");
	strHeader = _T("BACKCOLOR_B");
	strData.Format(_T("%d"), m_nBACKCOLOR_B);
	str.Format(_T("%s = %s\r\n"), strHeader, strData);
	fd.WriteString(str);

	fd.Close();		

	return CPropertyPage::OnApply();
}

LRESULT CGridSetupPage::OnSelChange(WPARAM wParam, LPARAM lParam) 
{
	UpdateData();

	SetModified();

	if (lParam == IDC_bCH_BACKCOLOR) {
		m_cBACKCOLOR = (LONG)wParam;
		m_nBACKCOLOR_R = GetRValue(m_cBACKCOLOR);
		m_nBACKCOLOR_G = GetGValue(m_cBACKCOLOR);
		m_nBACKCOLOR_B = GetBValue(m_cBACKCOLOR);	
	}
	if (lParam == IDC_bCH_LINECOLOR) {
		m_cLINECOLOR = (LONG)wParam;
		m_nLINECOLOR_R = GetRValue(m_cLINECOLOR);
		m_nLINECOLOR_G = GetGValue(m_cLINECOLOR);
		m_nLINECOLOR_B = GetBValue(m_cLINECOLOR);
	}

	return 0;
}

VOID CGridSetupPage::OnbCHLINEFONT() 
{
	CFontDialog FontDialog(&m_fFont);
	//FontDialog.m_cf.rgbColors = m_crFont;
	m_cFONTCOLOR = RGB(m_nFONTCOLOR_R, m_nFONTCOLOR_G, m_nFONTCOLOR_B);
	FontDialog.m_cf.rgbColors = m_cFONTCOLOR;
	if(FontDialog.DoModal() != IDOK)
		return;
	SetModified();
	
	// Set the Font data 
	FontDialog.GetCurrentFont(&m_fFont);
	m_nFontSize = FontDialog.GetSize() / 10;
	m_cFONTCOLOR = FontDialog.GetColor();
	m_nFONTCOLOR_R = GetRValue(m_cFONTCOLOR);
	m_nFONTCOLOR_G = GetGValue(m_cFONTCOLOR);
	m_nFONTCOLOR_B = GetBValue(m_cFONTCOLOR);

	ShowFont();	
}

VOID CGridSetupPage::OnbCHLINECOLOR() 
{
	CColorDialog dlg(m_cLINECOLOR, CC_FULLOPEN | CC_ANYCOLOR, this);

    if (dlg.DoModal() != IDOK) return;
	SetModified();

    m_cLINECOLOR = dlg.GetColor();
	m_nLINECOLOR_R = GetRValue(m_cLINECOLOR);
	m_nLINECOLOR_G = GetGValue(m_cLINECOLOR);
	m_nLINECOLOR_B = GetBValue(m_cLINECOLOR);	
}

VOID CGridSetupPage::OnbCHBACKCOLOR() 
{
	CColorDialog dlg(m_cBACKCOLOR, CC_FULLOPEN | CC_ANYCOLOR, this);

    if (dlg.DoModal() != IDOK) return;
	SetModified();

    m_cBACKCOLOR = dlg.GetColor();
	m_nBACKCOLOR_R = GetRValue(m_cBACKCOLOR);
	m_nBACKCOLOR_G = GetGValue(m_cBACKCOLOR);
	m_nBACKCOLOR_B = GetBValue(m_cBACKCOLOR);	
}

VOID CGridSetupPage::OnDeltapossHEIGHT(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nHeight -= pNMUpDown->iDelta;
	UpdateData(FALSE);

	SetModified();
	
	*pResult = 0;
}

VOID CGridSetupPage::OnSelchangecISBOLD() 
{
	SetModified();	
}
