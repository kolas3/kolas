// BO_CAT_PROC_0320_FORM_07.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0320_FORM_07.h"
#include "..\..\..\관리\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "..\..\..\ESL\ESL\DlgClassCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_07 dialog


CBO_CAT_PROC_0320_FORM_07::CBO_CAT_PROC_0320_FORM_07(CESL_Mgr* pParent, CString s007 /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_s007 = s007;
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320_FORM_07)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0320_FORM_07::~CBO_CAT_PROC_0320_FORM_07()
{
}

BOOL CBO_CAT_PROC_0320_FORM_07::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0320_FORM_07::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_FORM_07)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_FORM_07, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_FORM_07)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_07 message handlers

BOOL CBO_CAT_PROC_0320_FORM_07::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_07")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("자료범주표시"), _T("s"));
	INT i007Len = m_s007.GetLength();
	if ( m_s007 != _T("") &&  i007Len == 14 ) {
		CString sItemData = m_s007.Mid(1,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("자료식별표시"), sItemData);
		sItemData = m_s007.Mid(2,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("원본상태"), sItemData);
		sItemData = m_s007.Mid(3,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("속도"), sItemData);
		sItemData = m_s007.Mid(4,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("녹음재생형태"), sItemData);
		sItemData = m_s007.Mid(5,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("음구의폭/높이"), sItemData);
		sItemData = m_s007.Mid(6,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("크기"), sItemData);
		sItemData = m_s007.Mid(7,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("테이프의폭"), sItemData);
		sItemData = m_s007.Mid(8,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("테이프의구성"), sItemData);
		sItemData = m_s007.Mid(9,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("녹음기법"), sItemData);
		sItemData = m_s007.Mid(10,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("재질"), sItemData);
		sItemData = m_s007.Mid(11,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("편집유형"), sItemData);
		sItemData = m_s007.Mid(12,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("특수녹음방법"), sItemData);
		sItemData = m_s007.Mid(13,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_07"), _T("테이프의종류"), sItemData);		
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0320_FORM_07::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 ) {
		//007부호넣기
		UINT wndCodeID[ 13 ] = {
			IDC_cMAT_DISCRIM, IDC_cORIGINAL_LEVEL,
			IDC_cSPEED, IDC_cPLAY_LEVEL, IDC_cSOUND_LENGTH,
			IDC_cSIZE, IDC_cTAPE_WIDTH, IDC_cTAPE_FORMULA,
			IDC_cRECORD_MATHOD, IDC_cMATERIAL, IDC_cEDIT_TYPE,
			IDC_cTRECORD_MATHOD, IDC_cTAPE_KIND,
		};		

		CWnd *pWnd;
		CString sValue;		//유형, 삽도표시...
		CString sControlAlias;
		for ( INT i = 0 ; i < 13 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndCodeID[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );				
				switch( i ) {
					case 0 : 						
						sValue = _T("(자료식별표시)");
						sControlAlias = _T("자료식별표시");
						break;
					case 1 :
						sValue = _T("(원본상태)");
						sControlAlias = _T("원본상태");
						break;
					case 2 :
						sValue = _T("(속도)");
						sControlAlias = _T("속도");
						break;
					case 3 :
						sValue = _T("(녹음재생형태)");
						sControlAlias = _T("녹음재생형태");
						break;
					case 4 :						
						sValue = _T("(음구의폭/높이)");
						sControlAlias = _T("음구의폭/높이");
						break;
					case 5 :						
						sValue = _T("(007크기)");
						sControlAlias = _T("크기");
						break;
					case 6 :						
						sValue = _T("(테이프폭)");
						sControlAlias = _T("테이프의폭");
						break;
					case 7 :						
						sValue = _T("(테이프트랙의수)");
						sControlAlias = _T("테이프의구성");
						break;
					case 8 :						
						sValue = _T("(녹음기법주)");
						sControlAlias = _T("녹음기법");
						break;
					case 9 :						
						sValue = _T("(재질)");
						sControlAlias = _T("재질");
						break;
					case 10 :						
						sValue = _T("(편집유형)");
						sControlAlias = _T("편집유형");
						break;
					case 11 :						
						sValue = _T("(특수녹음방법)");
						sControlAlias = _T("특수녹음방법");
						break;
					case 12 :						
						sValue = _T("(음반,실린더,테이프의종류)");
						sControlAlias = _T("테이프의종류");
						break;					
				}
			}
		}
		if ( sValue.IsEmpty() ) {
			return FALSE;
		}

		//화면 띄우기
		CDlgClassCode dlg(this);		
		dlg.InitData();

		CString sPath = _T("..\\CFG\\007TAG코드.txt");
		//파일읽어오기
		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( sPath , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("007TAG코드 파일이 없습니다.") );
			return TRUE;
		}
		
		//++++++++++++++++++++++++++++++++++++++++++
		//Unicode일땐 2byte건너띄기
		FILE* fpUni = fp;
	
		if( 0xFF == fgetc(fpUni) ) 
		{
			if( 0xFE != fgetc(fpUni) )
			{
				fseek(fpUni, 0, SEEK_SET);
			}
		}
		else
		{
			fseek(fpUni, 0, SEEK_SET);
		}
		//------------------------------------------

		//[TX]찾기
		BOOL bFormYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );
			//[TX]찾기						
			//if ( sLine == _T("[TX]") ) {
			if ( sLine == _T("<녹음자료(s)>") ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("[TX]가 없습니다.") );
			return true;
		}
		//(자료식별표시)... 찾기
		BOOL bControlYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("[") ) {
				return true;
			}
			sLine.Replace( sLine.Right(1), NULL );
			if ( sLine == sValue ) {
				bControlYn = TRUE;
				break;
			}
		}
		//값 가져오기
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("\n") || sLine.Left(1) == _T("(") ) {
				break;
			}
			sLine.Replace( sLine.Right(1), NULL );
			CString sCode, sDesc;
			sCode = sLine.Left(1);
			sDesc = sLine.Mid(2);			
			dlg.AddData( sCode, sDesc );
		}
		if (dlg.DoModal() == IDOK) {			
			CString sCode = dlg.m_strSelCode;
			SetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), sControlAlias, sCode );			
		}
		return true;
	}
	else if ( pMsg->message == WM_KEYUP ) {
		UINT wndCodeID[ 13 ] = {
			IDC_cMAT_DISCRIM, IDC_cORIGINAL_LEVEL,
			IDC_cSPEED, IDC_cPLAY_LEVEL, IDC_cSOUND_LENGTH,
			IDC_cSIZE, IDC_cTAPE_WIDTH, IDC_cTAPE_FORMULA,
			IDC_cRECORD_MATHOD, IDC_cMATERIAL, IDC_cEDIT_TYPE,
			IDC_cTRECORD_MATHOD, IDC_cTAPE_KIND,
		};		

		CWnd *pWnd;
		CString sValue;		//유형, 삽도표시...
		CString sControlAlias;
		for ( INT i = 0 ; i < 13 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndCodeID[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );				
				switch( i ) {
					case 0 : 						
						sValue = _T("(자료식별표시)");
						sControlAlias = _T("자료식별표시");
						break;
					case 1 :
						sValue = _T("(원본상태)");
						sControlAlias = _T("원본상태");
						break;
					case 2 :
						sValue = _T("(속도)");
						sControlAlias = _T("속도");
						break;
					case 3 :
						sValue = _T("(녹음재생형태)");
						sControlAlias = _T("녹음재생형태");
						break;
					case 4 :						
						sValue = _T("(음구의폭/높이)");
						sControlAlias = _T("음구의폭/높이");
						break;
					case 5 :						
						sValue = _T("(007크기)");
						sControlAlias = _T("크기");
						break;
					case 6 :						
						sValue = _T("(테이프폭)");
						sControlAlias = _T("테이프의폭");
						break;
					case 7 :						
						sValue = _T("(테이프트랙의수)");
						sControlAlias = _T("테이프의구성");
						break;
					case 8 :						
						sValue = _T("(녹음기법주)");
						sControlAlias = _T("녹음기법");
						break;
					case 9 :						
						sValue = _T("(재질)");
						sControlAlias = _T("재질");
						break;
					case 10 :						
						sValue = _T("(편집유형)");
						sControlAlias = _T("편집유형");
						break;
					case 11 :						
						sValue = _T("(특수녹음방법)");
						sControlAlias = _T("특수녹음방법");
						break;
					case 12 :						
						sValue = _T("(음반,실린더,테이프의종류)");
						sControlAlias = _T("테이프의종류");
						break;					
				}
			}
		}

		CString sData;
		GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), sControlAlias, sData );

		if ( sValue.IsEmpty() ) {
			return FALSE;
		}

		//화면 띄우기
		//CDlgClassCode dlg;		
		//dlg.InitData();

		CString sPath = _T("..\\CFG\\007TAG코드.txt");
		//파일읽어오기
		FILE *fp;
		TCHAR cLine[256];
		fp = _tfopen( sPath , _T("rb") );
		if ( fp == NULL ) {
			AfxMessageBox( _T("007TAG코드 파일이 없습니다.") );
			return TRUE;
		}
		
		//++++++++++++++++++++++++++++++++++++++++++
		//Unicode일땐 2byte건너띄기
		FILE* fpUni = fp;
	
		if( 0xFF == fgetc(fpUni) ) 
		{
			if( 0xFE != fgetc(fpUni) )
			{
				fseek(fpUni, 0, SEEK_SET);
			}
		}
		else
		{
			fseek(fpUni, 0, SEEK_SET);
		}
		//------------------------------------------

		//[TX]찾기
		BOOL bFormYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			sLine.Replace( sLine.Right(1), NULL );
			//[TX]찾기						
			//if ( sLine == _T("[TX]") ) {
			if ( sLine == _T("<녹음자료(s)>") ) {
				bFormYn = TRUE;
				break;
			}
		}
		if ( bFormYn == FALSE ) {
			AfxMessageBox( _T("[TX]가 없습니다.") );
			return true;
		}
		//(자료식별표시)... 찾기
		BOOL bControlYn = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("[") ) {
				return true;
			}
			sLine.Replace( sLine.Right(1), NULL );
			if ( sLine == sValue ) {
				bControlYn = TRUE;
				break;
			}
		}
		//값 가져오기
		bool bFind = FALSE;
		while( _fgetts( cLine, 256, fp) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLine );
			if ( sLine.Left(1) == _T("\n") || sLine.Left(1) == _T("(") ) {
				break;
			}
			sLine.Replace( sLine.Right(1), NULL );
			CString sCode, sDesc;
			sCode = sLine.Left(1);
			//sDesc = sLine.Mid(2);			
			//dlg.AddData( sCode, sDesc );
			if ( sCode == sData ) {
				bFind = true;
				break;			
			}
		}
		if ( bFind == FALSE && sData != _T("") ) {
			AfxMessageBox ( _T("잘못된 값입니다.") );
			SetControlData( _T("CM_BO_CAT_PROC_0320_FORM_07"), sControlAlias, _T("") );
			//return true;
			return CDialog::PreTranslateMessage(pMsg);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}
HBRUSH CBO_CAT_PROC_0320_FORM_07::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}