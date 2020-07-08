// MngMailManager04.cpp : implementation file
//

#include "stdafx.h"
#include "MngMailManager04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngMailManager04 dialog


CMngMailManager04::CMngMailManager04(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngMailManager04)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMngMailManager04::~CMngMailManager04()
{
	
}

VOID CMngMailManager04::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngMailManager04)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMngMailManager04, CDialog)
	//{{AFX_MSG_MAP(CMngMailManager04)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngMailManager04 message handlers
BOOL CMngMailManager04::Create(CWnd* pParentWnd)
{	
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd); 

	EFS_END
	return FALSE;

}	

BOOL CMngMailManager04::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("MNG_MAIL_MANAGER_04") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_MAIL_MANAGER_02") );
		return false;
	}

	InitFileToDM();

	AllControlDisplay( _T("CM_MNG_MAIL_MANAGER_04") );

	CString sTotalDesc = 
	_T("적용 범위 :  메일 제목과 메일 내용에 적용이 됩니다. \r\n\r\n")
	_T("예) \r\n")
	_T("현재 도서관 환경 설정에서 입력되어 있는 도서관명이 \r\n")
	_T("'국립중앙 도서관'이라면 \r\n")
	_T("메일 제목에 '%도서관명%'이라고 입력하시면 후에 메일을 보낼 경우 \r\n")
	_T("'%도서관명%'이 '국립중앙 도서관'으로 변경됩니다.\r\n");

	GetDlgItem(IDC_staDESC)->SetWindowText( sTotalDesc );
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}


INT CMngMailManager04::InitFileToDM()
{
	EFS_BEGIN

	INT ids;
	
	////////////////////////////////////////////////////////////////////////
	// A\2005.10.11 ADD BY PDW(Jr)
	// 평생교육원용 알리아스 추가
	////////////////////////////////////////////////////////////////////////
	CString sFilePath = _T("");

	if (m_sMailGroup == _T("평생"))
	{
		sFilePath = _T("..\\cfg\\MailAlias\\MailAlias_LifeLong.txt");
		CString sTemp;
		this->GetWindowText(sTemp);
		sTemp = sTemp.Mid( 0, sTemp.Find(_T(")")) - 1 );
		sTemp += _T("_평생교육원 )");						// 강제적으로 윈도우 타이틀을 바꾼다 
		this->SetWindowText(sTemp);
	}
	else
	{
		sFilePath = _T("..\\cfg\\MailAlias\\MailAlias.txt");
	}

	CStdioFile fFile;
	ids = fFile.Open( sFilePath , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( ids == 0 )
	{
		AfxMessageBox(_T("알리아스 파일을 여는데 실패했습니다."));
		return 1;
	}
	
	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fFile.SeekToBegin();
	}
	
	CString sDMAlias = _T("DM_MNG_MAIL_MANAGER_04");
	CESL_DataMgr *pDM = FindDM( sDMAlias );


	pDM->FreeData();

	CString sString;
	CString sAlias;
	CString sDesc;
	INT nIndex;

	while( fFile.ReadString( sString ) )
	{
		nIndex = -1;
		sAlias.Empty(); sDesc.Empty();
		sString.TrimLeft();  sString.TrimRight();
		if( sString.IsEmpty() ) continue;
		
		nIndex = sString.Find(_T("|"));

		if( nIndex < 0 ) 
		{
			sAlias = sString;
		}
		else
		{
			sAlias = sString.Mid( 0 , nIndex );
			sDesc = sString.Mid( nIndex+1 , sString.GetLength() - nIndex );
		}
		sAlias.TrimLeft(); sAlias.TrimRight();
		sDesc.TrimLeft(); sDesc.TrimRight();
		

		ids = pDM->SetCellData( _T("알리아스") , sAlias , -1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeChangeAliasDM") );

		ids = pDM->SetCellData( _T("설명") , sDesc , pDM->GetRowCount()-1  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeChangeAliasDM") );
	}

	fFile.Close();


	return 0;

	EFS_END
	return -1;

}

INT CMngMailManager04::SetMailGroup(CString sMailGroup)
{
	m_sMailGroup = sMailGroup;
	return 0;
}

HBRUSH CMngMailManager04::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
