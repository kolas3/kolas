// WrongRegNoListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WrongRegNoListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWrongRegNoListDlg dialog


CWrongRegNoListDlg::CWrongRegNoListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWrongRegNoListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWrongRegNoListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit( );

	m_nWrongRegNoCnt = 0;
}

CWrongRegNoListDlg::~CWrongRegNoListDlg()
{

}

VOID CWrongRegNoListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWrongRegNoListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWrongRegNoListDlg, CDialog)
	//{{AFX_MSG_MAP(CWrongRegNoListDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWrongRegNoListDlg message handlers

VOID CWrongRegNoListDlg::AddRegNo( CString regNo, CString error )
{
	m_sRegNo.Add( regNo );
	m_sError.Add( error );
	m_nWrongRegNoCnt++;
}

INT CWrongRegNoListDlg::CheckRegNo( CString &regNo )
{
	regNo.TrimLeft();
	regNo.TrimRight();
	regNo.Replace( _T(" "), _T("") );
	regNo.Replace( _T("\t"), _T("") );
	regNo.MakeUpper();

	INT len = regNo.GetLength();
	if( 12 != len )
	{
		AddRegNo( regNo, _T("등록번호 길이가 맞지 않습니다.") );
		return -1;
	}

	TCHAR cBuf;

	CString strRegNum;
	INT nRegCodeLength = 0;
	INT nFind = 0;
	CStdioFile file;
	CString strLine;
	CString strRegCodeLength;
	if (file.Open(_T("..\\Cfg\\RegCodeLength.TXT"), CFile::modeRead|CFile::typeBinary))
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();

			nFind = strLine.Find(_T("등록구분코드자리수"));
			if (nFind < 0) continue;
			nFind = strLine.Find(_T("|"));

			strRegCodeLength = strLine.Mid(nFind+1);
			strRegCodeLength.TrimLeft(); strRegCodeLength.TrimRight();
			nRegCodeLength = _ttoi(strRegCodeLength);
		}
	}

 	for( INT i = nRegCodeLength ; i < len ; i++ )
 	{
 		cBuf = regNo.GetAt( i );
 		if( cBuf < 48 || cBuf > 57 )
 		{
 			AddRegNo( regNo, _T("등록번호가 숫자로 이루어져 있지 않습니다.") );
 			return -3;
 		}
 	}
	
	return 0;
}

INT CWrongRegNoListDlg::GetWrongRegNoCount()
{
	return m_nWrongRegNoCnt;
}

BOOL CWrongRegNoListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRichEditCtrl *pRichEdt = ( CRichEditCtrl * )GetDlgItem( IDC_richWRONG_REG_NO );
	pRichEdt->SetWindowText( MakeWrongRegNoList() );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CWrongRegNoListDlg::MakeWrongRegNoList()
{
	CString errorList;

	INT cnt = m_sRegNo.GetSize();
	for( INT i = 0; i < cnt; i++ )
		errorList = errorList + m_sRegNo.GetAt( i ) + _T("\n");
	return errorList;
}

VOID CWrongRegNoListDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRichEditCtrl *pRichEdt = ( CRichEditCtrl * )GetDlgItem( IDC_richWRONG_REG_NO );
	if( pRichEdt->GetSafeHwnd() )
		pRichEdt->MoveWindow( 0, 0, cx, cy );
		
}

HBRUSH CWrongRegNoListDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
