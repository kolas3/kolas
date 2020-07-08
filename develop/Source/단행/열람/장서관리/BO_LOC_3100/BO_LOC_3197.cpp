#include "stdafx.h"
#include "BO_LOC_3197.h"
#include <direct.h>
#include "..\..\..\공통\paintlib\common\Filter\plfilterresizebilinear.h"
#include "CharConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3197::CBO_LOC_3197(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{

	m_pBmp     = NULL;	
	m_bShowPic = TRUE;
	m_strIs64  = _T("");
	m_strIsND  = _T("");
	m_pParent  = pParent;
	m_ctlSlider = NULL;
	
}

CBO_LOC_3197::~CBO_LOC_3197()
{

	if( m_pBmp != NULL ) delete m_pBmp;
	if( m_ctlSlider != NULL ) delete m_ctlSlider;
}

VOID CBO_LOC_3197::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CBO_LOC_3197)
	//}}AFX_DATA_MAP		
	
}


BEGIN_MESSAGE_MAP(CBO_LOC_3197, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3197)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnHIDE_PICTURE, OnbtnHIDEPICTURE)
	ON_BN_CLICKED(IDC_btnHIDE_PICTURE2, OnbtnHIDEPICTURE2)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 862, OnMyMethod)
END_MESSAGE_MAP()

BOOL CBO_LOC_3197::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	m_ctlSlider = new CSlider;
	m_ctlSlider->Create( this, 20, 1, _T("UserInfo") );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	INT ids = InitESL_Mgr(_T("BO_LOC_3197"));
	if( ids < 0 )
	{
		AfxMessageBox( _T("Init Refrence Error") );
	}
	
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , m_strIs64 );
	GetManageValue( _T("기타") , _T("공통") , _T("국방대") , _T("")   , m_strIsND );
	
	m_BitmapButtonList.Add(IDC_btnHIDE_PICTURE, this , _T("..\\BMP\\365_1.bmp"), _T("..\\BMP\\365_1.bmp"), _T("..\\BMP\\365_1.bmp"));
	m_BitmapButtonList.Add(IDC_btnHIDE_PICTURE2, this , _T("..\\BMP\\365_3.bmp"), _T("..\\BMP\\365_3.bmp"), _T("..\\BMP\\365_3.bmp"));
	GetDlgItem(IDC_btnHIDE_PICTURE2)->ShowWindow(SW_HIDE);

	InitGrid();

	return TRUE;  
	              
}


BOOL CBO_LOC_3197::Create(CWnd* pParentWnd) 
{
EFS_BEGIN
		
	return CDialog::Create(IDD, pParentWnd);
	
EFS_END
	return FALSE;	
}


INT CBO_LOC_3197::ShowInfo()
{
EFS_BEGIN
	ViewUserInfo();

	ComboBoxSetting();

	if( m_pInfo->MODE == 10000 ) m_bShowPic=FALSE;

	if( !m_bShowPic ) return 0;
	
	ViewUserPic();

	return 0;

EFS_END
	return -1;
}

#define MAX_BLOB_BUFFER 20000000

INT CBO_LOC_3197::MakePictureFileFromDBLOBField()
{
EFS_BEGIN
		
	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePictureFileFromDBLOBField") );

	if( pDM->GetRowCount() == 0 )
	{
		m_sImageFilePath = _T("");
		return 1;
	}
	
	CString sTableName = _T("CO_LOAN_USER_TBL");	
	
	CString sRecKey;
	CString sFileExt;
	CString sFileName;

	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("대출자KEY") , sRecKey , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRegNoFileFromDBLOBField()") );
	if( sRecKey.IsEmpty() ) return 1;
		
	sFileName = _T("../대출증/이용자사진/");
	
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("사진종류") , sFileExt , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRegNoFileFromDBLOBField()") );
	
	if( sFileExt.IsEmpty() )
	{
		m_sImageFilePath = _T("");
		return 1;
	}
	
	_tmkdir(_T("../대출증"));
	_tmkdir(_T("../대출증/이용자사진"));
	
	sFileName += sRecKey + _T(".") + sFileExt;
	
	ids = SetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("사진경로") , sFileName , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeRegNoFileFromDBLOBField()") );
	
	
	BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
	
	INT nSize;
	ids = pDM->LOBGet( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey , _T("USER_PICTURE") , pBuffer , &nSize );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeRegNoFileFromDBLOBField") );
	
	CFile fd;
	if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("이용자 관리 Tmp File Open Error~"));
		free( pBuffer );
		return 1;
	}
		
	fd.WriteHuge(pBuffer, nSize);
	free( pBuffer );
	
	fd.Close();
	
	m_sImageFilePath = sFileName;
	
	return 0;
	
EFS_END
	return -1;	
}

INT CBO_LOC_3197::InitPictureControl()
{
EFS_BEGIN

	INT ids;

	CString m_sImageFileExt;

	if( m_sImageFilePath.IsEmpty() )
	{
		if( m_pBmp != NULL )
			delete m_pBmp;
		m_pBmp = NULL;
		OnPaint();
		return 0;
	}

	CFile fFile;
	ids = fFile.Open( m_sImageFilePath , CFile::modeRead | CFile::typeBinary );
	if( ids == 0 )
	{
		if( m_pBmp != NULL )
			delete m_pBmp;
		m_pBmp = NULL;
		OnPaint();
		return 1;
	}
	fFile.Close();

	if( m_pBmp == NULL )
		m_pBmp = new PLWinBmp;

	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( m_sImageFilePath.GetBuffer(0), &szTmp, &len );
	if( 0 < len)	
	{
		m_Decoder.MakeBmpFromFile (szTmp, m_pBmp);	
		delete [] szTmp;
	}		

	OnPaint();

	return 0;

EFS_END
	return -1;
}

VOID CBO_LOC_3197::OnPaint() 
{
EFS_BEGIN

	CPaintDC dc(this); 

	if( GetSafeHwnd() == NULL ) return;
	if( GetDlgItem(IDC_STATIC_PICTURE)->GetSafeHwnd() == NULL ) return;

	if( !m_bShowPic ) return ;
	

	if( m_pBmp == NULL )
	{
		GetDlgItem(IDC_STATIC_PICTURE)->ShowWindow(SW_SHOW);
		return;
	}
	
	GetDlgItem(IDC_STATIC_PICTURE)->ShowWindow(SW_HIDE);
	
	CRect WndPos;
	
	CDC* pDC = GetDlgItem(IDC_STATIC_PICTURE_POS)->GetDC();
	GetDlgItem(IDC_STATIC_PICTURE_POS)->GetWindowRect(WndPos);
	ClientToScreen(WndPos);
	
	PLFilterResizeBilinear Filter ( WndPos.Width()-2, WndPos.Height()-2);
	Filter.ApplyInPlace (m_pBmp);   
	
	m_pBmp->Draw( pDC->m_hDC , 1 , 1  );
		
	ReleaseDC(pDC);
	
EFS_END	
}

VOID CBO_LOC_3197::OnShowWindow(BOOL bShow, UINT nStatus) 
{
EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);
	
	if( bShow == SW_HIDE ) return;

	if( GetSafeHwnd() == NULL ) return;

	if( GetDlgItem(IDC_STATIC_PICTURE)->GetSafeHwnd() == NULL ) return;

	
	ShowInfo();

EFS_END	
}

VOID CBO_LOC_3197::OnbtnHIDEPICTURE() 
{
EFS_BEGIN

	if( m_bShowPic ) 
		m_bShowPic = FALSE;
	else
		m_bShowPic = TRUE;
		
	ChangeDlg2();

	ViewUserPic();

	m_ctlSlider->RedrawWindow();

	OnPaint();

EFS_END	
}


INT CBO_LOC_3197::ChangeDlg()
{
EFS_BEGIN

	INT nUnVisible = SW_HIDE;
	INT nMoveLength = -95;
	CString sText = _T(">>");

	GetDlgItem(IDC_btnHIDE_PICTURE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_btnHIDE_PICTURE2)->ShowWindow(SW_SHOW);

	if( m_bShowPic )
	{
		nUnVisible = SW_SHOW;
		nMoveLength = 95;
		sText = _T("<<");
		GetDlgItem(IDC_btnHIDE_PICTURE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnHIDE_PICTURE2)->ShowWindow(SW_HIDE);
	}


	const INT nUnVisibleCnt = 2;

	INT nUnVisibleIDC[nUnVisibleCnt] =
	{
		IDC_STATIC_PICTURE , IDC_STATIC_PICTURE_POS
	};


	for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
	{
		GetDlgItem(nUnVisibleIDC[i])->ShowWindow(nUnVisible);
	}


	const INT nMoveCnt = 23;
	INT nMoveIDC[nMoveCnt] = 
	{
		IDC_staSU_HANDPHONE , IDC_staSU_PHONE_HOME , IDC_staPHONE_WORK , IDC_staSU_POS ,  
		IDC_staSU_CLASS , IDC_staSU_LOST , IDC_edtSU_HANDPHONE , IDC_edtSU_PHONE_HOME , 
		IDC_edt_PHONE_WORK , IDC_cmbSU_POS , IDC_cmbSU_CLASS , IDC_cmbSU_LOST , 
		IDC_staRECT , IDC_edtSU_LOST_UCARD ,
		IDC_staRECT2 , IDC_staSU_HANDPHONE2 , IDC_edtSU_SCHOOL , IDC_edtSU_GRADE , 
		IDC_staSU_PHONE_HOME2 , IDC_edt_CLASS , IDC_staPHONE_WORK2 , IDC_edt_CLASS_NO , 
		IDC_staPHONE_WORK3
	};


	CRect WndPos;
	for( i = 0 ; i < nMoveCnt ; i++ )
	{
		GetDlgItem(nMoveIDC[i])->GetWindowRect(WndPos);
		ScreenToClient(WndPos);
		GetDlgItem(nMoveIDC[i])->MoveWindow( WndPos.left + nMoveLength , WndPos.top , WndPos.Width() , WndPos.Height() );
	}
	
	GetWindowRect(WndPos);

	MoveWindow(1600 , 1200 , WndPos.Width()+ nMoveLength , WndPos.Height() );
	MoveWindow(WndPos.left , WndPos.top , WndPos.Width()+ nMoveLength , WndPos.Height() );

	return 0;

EFS_END
	return -1;
}


INT CBO_LOC_3197::ViewUserPic()
{
EFS_BEGIN

	MakePictureFileFromDBLOBField();
	InitPictureControl();

	return 0;
	
EFS_END
	return -1;
}

INT CBO_LOC_3197::ComboBoxSetting()
{
EFS_BEGIN

	CString sLostYN;

	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePictureFileFromDBLOBField") );

	if( pDM->GetRowCount() == 0 ) return 0;

	pDM->GetCellData( _T("회원증분실") ,  0 , sLostYN );

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbSU_LOST);

	CString sCombo;
	for( INT i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sCombo);
		if( sCombo.Compare(sLostYN) == 0 )
			pCombo->SetCurSel(i);
	}

	GetDlgItem(IDC_edtSU_LOST_UCARD)->SetWindowText(sLostYN);

	return 0;

EFS_END
	return -1;
}

VOID CBO_LOC_3197::OnbtnHIDEPICTURE2() 
{
	if( m_bShowPic ) 
		m_bShowPic = FALSE;
	else
		m_bShowPic = TRUE;
	
	
	
	ChangeDlg2();
	
	ViewUserPic();

	m_ctlSlider->RedrawWindow();

	this->Invalidate();
	this->SendMessage(WM_NCPAINT);
	OnPaint();	
}

VOID CBO_LOC_3197::InitGrid()
{
EFS_BEGIN	
	
	CMSHFlexGrid* grid = NULL;
	if ( grid == NULL )
	{
		grid = ( (CMSHFlexGrid*) GetDlgItem (IDC_gUSER_SIMPLE_INFO) );
		if ( grid == NULL ) return;
	}

	grid->SetColWidth(0,0,1050);
	grid->SetColWidth(1,0,1500);
	
	CString	strDate;
	INT	i;	

	if ( m_strIs64 == _T("Y") && m_strIsND != _T("Y") )			
	{
		CString	sFiledArray[] = { _T("휴대폰"),_T("자택전화"),_T("근무지전화"),_T("소속"), _T("직급"), _T("회원증분실"),_T("대학"), _T("학부"),_T("학과"),_T("학년"), _T("반"),_T("학번/군번") };	

		grid->SetRows(12);
		for ( i=0 ; i<12 ; i++ )
		{		
			grid->SetTextMatrix(i, 0, sFiledArray[i] );
		}		
	}	
	else if ( m_strIs64 != _T("Y") && m_strIsND == _T("Y") )	
	{
		CString	sFiledArray[] = { _T("휴대폰"),_T("자택전화"),_T("근무지전화"),_T("소속"), _T("직급"), _T("회원증분실"),_T("교번") };	

		grid->SetRows(7);
		for ( i=0 ; i<7 ; i++ )
		{		
			grid->SetTextMatrix(i, 0, sFiledArray[i] );
		}		

		INT nHeight = 71;
		CRect rc;
		GetWindowRect( &rc );		
		MoveWindow( rc.left , rc.top ,rc.Width(), rc.Height() - nHeight  );
		
		
		grid->GetWindowRect( rc );
		ScreenToClient( rc );
		grid->MoveWindow( rc.left, rc.top, rc.Width(), rc.Height() - nHeight );
		
	}
	else												
	{

		CString	sFiledArray[] = { _T("주민번호"),_T("휴대폰"),_T("자택전화"),_T("근무지전화"),_T("소속"), _T("직급"), _T("회원증분실"),_T("학교"), _T("학년") };	
		
		grid->SetRows(7);
		for ( i=0 ; i<7 ; i++ )
		{		
			grid->SetTextMatrix(i, 0, sFiledArray[i] );
		}

		CRect WndPos;
		GetDlgItem(IDC_gUSER_SIMPLE_INFO)->GetWindowRect(WndPos);
		ScreenToClient(WndPos);
		GetDlgItem(IDC_gUSER_SIMPLE_INFO)->MoveWindow( WndPos.left, WndPos.top , WndPos.Width() , WndPos.Height() - 52 );

		GetWindowRect(WndPos);		
		MoveWindow(WndPos.left , WndPos.top ,WndPos.Width(), WndPos.Height() - 52 );
	}

EFS_END
}

INT CBO_LOC_3197::ViewUserInfo()
{
	CMSHFlexGrid* grid = NULL;
	CESL_DataMgr	*pDM;
	CString *strDMAlias;
	CString strValue;
	INT		i;
	INT		nCount;
		
	if ( grid == NULL )
	{
		grid = ( (CMSHFlexGrid*) GetDlgItem (IDC_gUSER_SIMPLE_INFO) );
		if ( grid == NULL ) return -100;
	}
	
	CString strDMAliasNormal[] = { _T("주민등록번호"),_T("휴대폰"),_T("자택전화"),_T("근무지전화"),_T("대출자소속"), _T("대출자직급"), _T("회원증분실"),_T("학교"),_T("학년") };
	CString strDMAlias64[]     = { _T("휴대폰"),_T("자택전화"),_T("근무지전화"),_T("대출자소속"), _T("대출자직급"), _T("회원증분실"),_T("학교"), _T("학부"),_T("학과"),_T("학년"), _T("반"),_T("번호") };
    CString strDMAliasND[]     = { _T("휴대폰"),_T("자택전화"),_T("근무지전화"),_T("대출자소속"), _T("대출자직급"), _T("회원증분실"),_T("번호") };
	pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewUserInfo") );	
	
	if ( m_strIs64 == _T("Y") && m_strIsND != _T("Y") )			
	{		
		strDMAlias = strDMAlias64;
		nCount = 12;		
	}
	else if ( m_strIs64 != _T("Y") && m_strIsND == _T("Y") )	
	{
		strDMAlias = strDMAliasND;
		nCount = 7;		
	}
	else												
	{		
		strDMAlias = strDMAliasNormal;
		nCount = 7;
	}

	for ( i=0 ; i<nCount ; i++ )
	{
		pDM->GetCellData( strDMAlias[i] ,  0 , strValue );			

		if ( strDMAlias[i] == _T("대출자소속") )
		{
			m_pParent->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), strValue, strValue);
		}
		else if ( strDMAlias[i] == _T("대출자직급") )
		{
			m_pParent->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출직급정보"), strValue, strValue);
		}
		else if ( strDMAlias[i] == _T("학부") )
		{
			m_pParent->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학부"), strValue, strValue);
		}
		else if ( strDMAlias[i] == _T("학과") )
		{
			m_pParent->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학과"), strValue, strValue);
		}

		else if( strDMAlias[i] == _T("주민등록번호") )
		{
			INT		ids;
			CString opt;
			GetManageValue( _T("열람"), _T("공통"), _T("대출자주민등록번호보여주기"), _T("대출자주민번호"), opt );

			if( _T("N") != opt )
			{
				ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1 );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SearchUser()") );
				pDM->GetCellData( strDMAlias[i] , 0 , strValue );
			}

		}		
			
		grid->SetTextMatrix(i,1,_T(" ")+strValue);		
	}		

	return 0;
}

INT CBO_LOC_3197::ChangeDlg2()
{
	EFS_BEGIN

	INT nUnVisible = SW_HIDE;
	INT nMoveLength = -118;
	CString sText = _T(">>");

	GetDlgItem(IDC_btnHIDE_PICTURE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_btnHIDE_PICTURE2)->ShowWindow(SW_SHOW);

	if( m_bShowPic )
	{
		nUnVisible = SW_SHOW;
		nMoveLength = 118;
		sText = _T("<<");
		GetDlgItem(IDC_btnHIDE_PICTURE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnHIDE_PICTURE2)->ShowWindow(SW_HIDE);
	}


	const INT nUnVisibleCnt = 2;

	INT nUnVisibleIDC[nUnVisibleCnt] =
	{
		IDC_STATIC_PICTURE , IDC_STATIC_PICTURE_POS
	};


	for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
	{
		GetDlgItem(nUnVisibleIDC[i])->ShowWindow(nUnVisible);
	}

	CRect WndPos;
	GetDlgItem(IDC_gUSER_SIMPLE_INFO)->GetWindowRect(WndPos);
	ScreenToClient(WndPos);
	GetDlgItem(IDC_gUSER_SIMPLE_INFO)->MoveWindow( WndPos.left + nMoveLength , WndPos.top , WndPos.Width() , WndPos.Height() );
	
	
	GetWindowRect(WndPos);

	MoveWindow(1600 , 1200 , WndPos.Width()+ nMoveLength , WndPos.Height() );
	MoveWindow(WndPos.left , WndPos.top , WndPos.Width()+ nMoveLength , WndPos.Height() );

	this->RedrawWindow();
	this->SendMessage(WM_NCPAINT);

	return 0;

	EFS_END

	return -1;
}

VOID CBO_LOC_3197::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);	
}

HBRUSH CBO_LOC_3197::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}


VOID CBO_LOC_3197::SetTransparency(INT percent)
{
	SLWA pSetLayeredWindowAttributes = NULL;  
	HINSTANCE hmodUSER32 = LoadLibrary(_T("USER32.DLL")); 
	pSetLayeredWindowAttributes=(SLWA)GetProcAddress(hmodUSER32,"SetLayeredWindowAttributes");
    
	HWND hwnd = this->m_hWnd; 
	SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	pSetLayeredWindowAttributes(hwnd, 0, (255 * percent) / 100, LWA_ALPHA);
}

LONG CBO_LOC_3197::OnMyMethod(WPARAM wParam,LPARAM lParam)
{
	INT nPercent = (INT)lParam;
	SetTransparency(nPercent);
	
	return 0;
}
