// BO_CAT_PROC_0320_FORM_06.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0320_FORM_06.h"
#include "..\..\..\관리\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "..\..\..\ESL\ESL\DlgClassCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_06 dialog


CBO_CAT_PROC_0320_FORM_06::CBO_CAT_PROC_0320_FORM_06(CESL_Mgr* pParent, CString s007 /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_s007 = s007;
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320_FORM_06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0320_FORM_06::~CBO_CAT_PROC_0320_FORM_06()
{
}

BOOL CBO_CAT_PROC_0320_FORM_06::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0320_FORM_06::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_FORM_06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_FORM_06, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_FORM_06)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_06 message handlers

BOOL CBO_CAT_PROC_0320_FORM_06::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_06")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("자료범주표시"), _T("m"));
	INT i007Len = m_s007.GetLength();
	if ( m_s007 != _T("") &&  i007Len == 23 ) {
		CString sItemData = m_s007.Mid(1,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("자료식별표시"), sItemData);
		sItemData = m_s007.Mid(2,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("원본상태"), sItemData);
		sItemData = m_s007.Mid(3,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("색채"), sItemData);
		sItemData = m_s007.Mid(4,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("스크린형태"), sItemData);
		sItemData = m_s007.Mid(5,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("매체의음향수록"), sItemData);
		sItemData = m_s007.Mid(6,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("음향수록매체"), sItemData);
		sItemData = m_s007.Mid(7,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("필름의폭"), sItemData);
		sItemData = m_s007.Mid(8,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("음향의재생상태"), sItemData);
		sItemData = m_s007.Mid(9,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("제작요소"), sItemData);
		sItemData = m_s007.Mid(10,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("감광상태"), sItemData);
		sItemData = m_s007.Mid(11,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("제작상태"), sItemData);
		sItemData = m_s007.Mid(12,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("필름의기본재료"), sItemData);
		sItemData = m_s007.Mid(13,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("색깔의범주"), sItemData);
		sItemData = m_s007.Mid(14,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("천연색인화유형"), sItemData);
		sItemData = m_s007.Mid(15,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("변질상태"), sItemData);
		sItemData = m_s007.Mid(16,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("완전성"), sItemData);
		sItemData = m_s007.Mid(17,6);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_06"), _T("필름의검사일자"), sItemData);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0320_FORM_06::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 ) {
		//007부호넣기
		UINT wndCodeID[ 17 ] = {
			IDC_cMAT_DISCRIM, IDC_cORIGINAL_LEVEL, IDC_cCOLOR,
			IDC_cSCREEN_FORM, IDC_cMUSIC_CONTENTS, IDC_cMUSIC_MAT,
			IDC_cFIRM_LENGTH, IDC_cPLAY_LEVEL, IDC_cMAKE_ELEMENT,
			IDC_cEXPOSURE_LEVEL, IDC_cMAKE_LEVEL, IDC_cFIRM_MAT,
			IDC_cCOLOR_CATEGORY, IDC_cCOLOR_TYPE, IDC_cCHANGE_LEVEL,
			IDC_cCOMPLETENESS, IDC_eFIRM_DATE
		};		

		CWnd *pWnd;
		CString sValue;		//유형, 삽도표시...
		CString sControlAlias;
		for ( INT i = 0 ; i < 17 ; i++ ) {
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
						sValue = _T("(색채)");
						sControlAlias = _T("색채");
						break;
					case 3 :
						sValue = _T("(스크린형태)");
						sControlAlias = _T("스크린형태");
						break;
					case 4 :						
						sValue = _T("(매체의음향수록)");
						sControlAlias = _T("매체의음향수록");
						break;
					case 5 :						
						sValue = _T("(음향수록매체)");
						sControlAlias = _T("음향수록매체");
						break;
					case 6 :						
						sValue = _T("(필름의폭)");
						sControlAlias = _T("필름의폭");
						break;
					case 7 :						
						sValue = _T("(음향의재생상태)");
						sControlAlias = _T("음향의재생상태");
						break;
					case 8 :						
						sValue = _T("(제작요소)");
						sControlAlias = _T("제작요소");
						break;
					case 9 :						
						sValue = _T("(감광상태)");
						sControlAlias = _T("감광상태");
						break;
					case 10 :						
						sValue = _T("(제작상태)");
						sControlAlias = _T("제작상태");
						break;
					case 11 :						
						sValue = _T("(필름의기본재료)");
						sControlAlias = _T("필름의기본재료");
						break;
					case 12 :						
						sValue = _T("(색채의범주)");
						sControlAlias = _T("색깔의범주");
						break;
					case 13 :						
						sValue = _T("(천연색인화유형)");
						sControlAlias = _T("천연색인화유형");
						break;
					case 14 :						
						sValue = _T("(변질상태)");
						sControlAlias = _T("변질상태");
						break;
					case 15 :						
						sValue = _T("(완전성)");
						sControlAlias = _T("완전성");
						break;
					case 16 :						
						sValue = _T("(필름의검사일자)");
						sControlAlias = _T("필름의검사일자");
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
			if ( sLine == _T("<영화(m)>") ) {
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
			SetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), sControlAlias, sCode );			
		}
		return true;
	}
	else if ( pMsg->message == WM_KEYUP ) {
		UINT wndCodeID[ 17 ] = {
			IDC_cMAT_DISCRIM, IDC_cORIGINAL_LEVEL, IDC_cCOLOR,
			IDC_cSCREEN_FORM, IDC_cMUSIC_CONTENTS, IDC_cMUSIC_MAT,
			IDC_cFIRM_LENGTH, IDC_cPLAY_LEVEL, IDC_cMAKE_ELEMENT,
			IDC_cEXPOSURE_LEVEL, IDC_cMAKE_LEVEL, IDC_cFIRM_MAT,
			IDC_cCOLOR_CATEGORY, IDC_cCOLOR_TYPE, IDC_cCHANGE_LEVEL,
			IDC_cCOMPLETENESS, IDC_eFIRM_DATE
		};		

		CWnd *pWnd;
		CString sValue;		//유형, 삽도표시...
		CString sControlAlias;
		for ( INT i = 0 ; i < 17 ; i++ ) {
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
						sValue = _T("(색채)");
						sControlAlias = _T("색채");
						break;
					case 3 :
						sValue = _T("(스크린형태)");
						sControlAlias = _T("스크린형태");
						break;
					case 4 :						
						sValue = _T("(매체의음향수록)");
						sControlAlias = _T("매체의음향수록");
						break;
					case 5 :						
						sValue = _T("(음향수록매체)");
						sControlAlias = _T("음향수록매체");
						break;
					case 6 :						
						sValue = _T("(필름의폭)");
						sControlAlias = _T("필름의폭");
						break;
					case 7 :						
						sValue = _T("(음향의재생상태)");
						sControlAlias = _T("음향의재생상태");
						break;
					case 8 :						
						sValue = _T("(제작요소)");
						sControlAlias = _T("제작요소");
						break;
					case 9 :						
						sValue = _T("(감광상태)");
						sControlAlias = _T("감광상태");
						break;
					case 10 :						
						sValue = _T("(제작상태)");
						sControlAlias = _T("제작상태");
						break;
					case 11 :						
						sValue = _T("(필름의기본재료)");
						sControlAlias = _T("필름의기본재료");
						break;
					case 12 :						
						sValue = _T("(색채의범주)");
						sControlAlias = _T("색깔의범주");
						break;
					case 13 :						
						sValue = _T("(천연색인화유형)");
						sControlAlias = _T("천연색인화유형");
						break;
					case 14 :						
						sValue = _T("(변질상태)");
						sControlAlias = _T("변질상태");
						break;
					case 15 :						
						sValue = _T("(완전성)");
						sControlAlias = _T("완전성");
						break;
					case 16 :						
						sValue = _T("(필름의검사일자)");
						sControlAlias = _T("필름의검사일자");
						break;					
				}
			}
		}

		CString sData;
		GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), sControlAlias, sData );

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
			if ( sLine == _T("<영화(m)>") ) {
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
			SetControlData( _T("CM_BO_CAT_PROC_0320_FORM_06"), sControlAlias, _T("") );
			//return true;
			return CDialog::PreTranslateMessage(pMsg);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}
HBRUSH CBO_CAT_PROC_0320_FORM_06::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}