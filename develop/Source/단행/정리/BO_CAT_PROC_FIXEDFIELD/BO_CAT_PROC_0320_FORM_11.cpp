// BO_CAT_PROC_0320_FORM_11.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0320_FORM_11.h"
#include "..\..\..\관리\KOMARC\KOMarcGetter\KOMarcGetterDlg.h"
#include "..\..\..\ESL\ESL\DlgClassCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_11 dialog


CBO_CAT_PROC_0320_FORM_11::CBO_CAT_PROC_0320_FORM_11(CESL_Mgr* pParent, CString s007 /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_s007 = s007;
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0320_FORM_11)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0320_FORM_11::~CBO_CAT_PROC_0320_FORM_11()
{
}

BOOL CBO_CAT_PROC_0320_FORM_11::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0320_FORM_11::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0320_FORM_11)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0320_FORM_11, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0320_FORM_11)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0320_FORM_11 message handlers

BOOL CBO_CAT_PROC_0320_FORM_11::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0320_FORM_11")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("자료범주표시"), _T("c"));
	INT i007Len = m_s007.GetLength();
	if ( m_s007 != _T("") &&  i007Len == 13 ) {
		CString sItemData = m_s007.Mid(1,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("자료식별표시"), sItemData);		
		sItemData = m_s007.Mid(2,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("색채"), sItemData);
		sItemData = m_s007.Mid(3,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("규격"), sItemData);
		sItemData = m_s007.Mid(4,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("음향"), sItemData);
		sItemData = m_s007.Mid(5,3);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("해상도"), sItemData);
		sItemData = m_s007.Mid(8,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("파일포맷"), sItemData);
		sItemData = m_s007.Mid(9,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("품질보증"), sItemData);
		sItemData = m_s007.Mid(10,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("이전의정보원"), sItemData);
		sItemData = m_s007.Mid(11,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("압축수준"), sItemData);
		sItemData = m_s007.Mid(12,1);
		SetControlData(_T("CM_BO_CAT_PROC_0320_FORM_11"), _T("재포맷의품질"), sItemData);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0320_FORM_11::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9 ) {
		//007부호넣기
		UINT wndCodeID[ 10 ] = {
			IDC_cMAT_DISCRIM, IDC_cORIGINAL_LEVEL, IDC_cEXPOSURE_LEVEL,
			IDC_cSTANDARD, IDC_cXXCONSUMR_RANGE, IDC_eXXCONSUME,
			IDC_cCOLOR, IDC_cEXPOSURE_MAT, IDC_cCREAT_PROCESS, IDC_cFIRM_MAT
		};		

		CWnd *pWnd;
		CString sValue;		//유형, 삽도표시...
		CString sControlAlias;
		for ( INT i = 0 ; i < 10 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndCodeID[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );				
				switch( i ) {
					case 0 :
						sValue = _T("(자료식별표시)");
						sControlAlias = _T("자료식별표시");
						break;
					case 1 :
						sValue = _T("(색채)");
						sControlAlias = _T("색채");
						break;
					case 2 :
						sValue = _T("(규격)");
						sControlAlias = _T("규격");
						break;
					case 3 :
						sValue = _T("(음향)");
						sControlAlias = _T("음향");
						break;
					case 4 :
						sValue = _T("(해상도)");
						sControlAlias = _T("해상도");
						break;
					case 5 :
						sValue = _T("(파일포맷)");
						sControlAlias = _T("파일포맷");
						break;
					case 6 :
						sValue = _T("(품질보증)");
						sControlAlias = _T("품질보증");
						break;
					case 7 :
						sValue = _T("(이전의정보원)");
						sControlAlias = _T("이전의정보원");
						break;
					case 8 :
						sValue = _T("(압축수준)");
						sControlAlias = _T("압축수준");
						break;
					case 9 :
						sValue = _T("(재포맷의품질)");
						sControlAlias = _T("재포맷의품질");
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
			if ( sLine == _T("<컴퓨터파일(c)>") ) {
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
			if ( sValue == _T("(해상도)") ) {
				CString sCode, sDesc;
				sCode = sLine.Left(3);
				sDesc = sLine.Mid(4);			
				dlg.AddData( sCode, sDesc );
			}
			else {
				CString sCode, sDesc;
				sCode = sLine.Left(1);
				sDesc = sLine.Mid(2);			
				dlg.AddData( sCode, sDesc );
			}
		}
		if (dlg.DoModal() == IDOK) {			
			CString sCode = dlg.m_strSelCode;
			SetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), sControlAlias, sCode );			
		}
		return true;
	}
	else if ( pMsg->message == WM_KEYUP ) {
		UINT wndCodeID[ 10 ] = {
			IDC_cMAT_DISCRIM, IDC_cORIGINAL_LEVEL, IDC_cEXPOSURE_LEVEL,
			IDC_cSTANDARD, IDC_cXXCONSUMR_RANGE, IDC_eXXCONSUME,
			IDC_cCOLOR, IDC_cEXPOSURE_MAT, IDC_cCREAT_PROCESS, IDC_cFIRM_MAT
		};		

		CWnd *pWnd;
		CString sValue;		//유형, 삽도표시...
		CString sControlAlias;
		for ( INT i = 0 ; i < 10 ; i++ ) {
			pWnd = ( CWnd * )GetDlgItem( wndCodeID[ i ] );
			if ( pWnd->m_hWnd == pMsg->hwnd ) {
				CKOMarcGetterDlg dlg( this );				
				switch( i ) {
					case 0 :
						sValue = _T("(자료식별표시)");
						sControlAlias = _T("자료식별표시");
						break;
					case 1 :
						sValue = _T("(색채)");
						sControlAlias = _T("색채");
						break;
					case 2 :
						sValue = _T("(규격)");
						sControlAlias = _T("규격");
						break;
					case 3 :
						sValue = _T("(음향)");
						sControlAlias = _T("음향");
						break;
					case 4 :
						sValue = _T("(해상도)");
						sControlAlias = _T("해상도");
						break;
					case 5 :
						sValue = _T("(파일포맷)");
						sControlAlias = _T("파일포맷");
						break;
					case 6 :
						sValue = _T("(품질보증)");
						sControlAlias = _T("품질보증");
						break;
					case 7 :
						sValue = _T("(이전의정보원)");
						sControlAlias = _T("이전의정보원");
						break;
					case 8 :
						sValue = _T("(압축수준)");
						sControlAlias = _T("압축수준");
						break;
					case 9 :
						sValue = _T("(재포맷의품질)");
						sControlAlias = _T("재포맷의품질");
						break;
				}
			}
		}

		CString sData;
		GetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), sControlAlias, sData );

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
			if ( sLine == _T("<컴퓨터파일(c)>") ) {
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
			if ( sValue == _T("(해상도)") ) {
				CString sCode, sDesc;
				sCode = sLine.Left(3);
				//sDesc = sLine.Mid(4);			
				//dlg.AddData( sCode, sDesc );
				if ( sCode == sData ) {
					bFind = true;
					break;			
				}
			}
			else {
				CString sCode, sDesc;
				sCode = sLine.Left(1);
				//sDesc = sLine.Mid(2);			
				//dlg.AddData( sCode, sDesc );
				if ( sCode == sData ) {
					bFind = true;
					break;			
				}
			}
		}
		if ( bFind == FALSE && sData != _T("") && sValue != _T("(해상도)") ) {
			AfxMessageBox ( _T("잘못된 값입니다.") );
			SetControlData( _T("CM_BO_CAT_PROC_0320_FORM_11"), sControlAlias, _T("") );
			//return true;
			return CDialog::PreTranslateMessage(pMsg);
		}
	}
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}
HBRUSH CBO_CAT_PROC_0320_FORM_11::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}