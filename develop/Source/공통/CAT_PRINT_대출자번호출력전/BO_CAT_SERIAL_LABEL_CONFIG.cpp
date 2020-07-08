// BO_CAT_SERIAL_LABEL_CONFIG.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_SERIAL_LABEL_CONFIG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SERIAL_LABEL_CONFIG dialog


CBO_CAT_SERIAL_LABEL_CONFIG::CBO_CAT_SERIAL_LABEL_CONFIG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_SERIAL_LABEL_CONFIG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CBO_CAT_SERIAL_LABEL_CONFIG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_SERIAL_LABEL_CONFIG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_SERIAL_LABEL_CONFIG, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_SERIAL_LABEL_CONFIG)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_SERIAL_LABEL_CONFIG message handlers

BOOL CBO_CAT_SERIAL_LABEL_CONFIG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sPathUnit = _T("..\\CFG\\정리_연속용지_출력.cfg");
	//파일읽어오기
	FILE *fpSerialConfig;
	TCHAR cLineConfig[256];
	fpSerialConfig = _tfopen( sPathUnit , _T("rb") );
	if ( fpSerialConfig == NULL ) {
		//AfxMessageBox( _T("..\\CFG\\정리_장비_출력.cfg 파일이 없습니다.") );
		GetDlgItem(IDC_EdtFisrtLoc)->SetWindowText( _T("10") );
		GetDlgItem(IDC_EdtSecLoc)->SetWindowText( _T("30") );
		GetDlgItem(IDC_EdtRegnoLoc)->SetWindowText( _T("60") );
		GetDlgItem(IDC_EdtColnoLoc)->SetWindowText( _T("1") );
		GetDlgItem(IDC_EdtColCount)->SetWindowText( _T("2") );

		CComboBox *pCobRegnoPrintYN = NULL;
		pCobRegnoPrintYN = (CComboBox*)GetDlgItem(IDC_CobRegnoPrintYN);
		pCobRegnoPrintYN->InsertString( 0, _T("출력 함") );
		pCobRegnoPrintYN->InsertString( 1, _T("출력 안함") );		
		pCobRegnoPrintYN->SetCurSel( 0 );

		CComboBox *pCobFirstLineSpace = NULL;
		pCobFirstLineSpace = (CComboBox*)GetDlgItem(IDC_CobFirstLineSpace);
		pCobFirstLineSpace->InsertString( 0, _T("공백 있음") );
		pCobFirstLineSpace->InsertString( 1, _T("공백 없음") );		
		pCobFirstLineSpace->SetCurSel( 0 );

		GetDlgItem(IDC_EdtTopSpace)->SetWindowText( _T("0") );

		CComboBox *pCobSerialFont = NULL;
		pCobSerialFont = (CComboBox*)GetDlgItem(IDC_CobSerialFont);
		pCobSerialFont->InsertString( 0, _T("기본 폰트") );
		pCobSerialFont->InsertString( 1, _T("한글 명조체 10.5p") );
		pCobSerialFont->InsertString( 2, _T("한글 고딕체 10.5p") );
		pCobSerialFont->InsertString( 3, _T("한글 명조체 14p") );
		pCobSerialFont->InsertString( 4, _T("한글 고딕체 14p") );
		pCobSerialFont->InsertString( 5, _T("한글 샘물체 10.5p") );
		pCobSerialFont->InsertString( 6, _T("한글 샘물체 14p") );
		pCobSerialFont->InsertString( 7, _T("한글 궁서체 10.5p") );
		pCobSerialFont->InsertString( 8, _T("한글 필기체 10.5p") );
		pCobSerialFont->InsertString( 9, _T("한글 디나루체 10.5p") );
		pCobSerialFont->InsertString( 10, _T("한글 옛체 10.5p") );
		pCobSerialFont->SetCurSel( 0 );

		return FALSE;
	}	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fpSerialConfig;

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

	GetDlgItem(IDC_EdtFisrtLoc)->SetWindowText( _T("10") );
	GetDlgItem(IDC_EdtSecLoc)->SetWindowText( _T("30") );
	GetDlgItem(IDC_EdtRegnoLoc)->SetWindowText( _T("60") );
	GetDlgItem(IDC_EdtColnoLoc)->SetWindowText( _T("1") );
	GetDlgItem(IDC_EdtColCount)->SetWindowText( _T("2") );

	CComboBox *pCobRegnoPrintYN = NULL;
	pCobRegnoPrintYN = (CComboBox*)GetDlgItem(IDC_CobRegnoPrintYN);
	pCobRegnoPrintYN->InsertString( 0, _T("출력 함") );
	pCobRegnoPrintYN->InsertString( 1, _T("출력 안함") );		
	pCobRegnoPrintYN->SetCurSel( 0 );

	CComboBox *pCobFirstLineSpace = NULL;
	pCobFirstLineSpace = (CComboBox*)GetDlgItem(IDC_CobFirstLineSpace);
	pCobFirstLineSpace->InsertString( 0, _T("공백 있음") );
	pCobFirstLineSpace->InsertString( 1, _T("공백 없음") );		
	pCobFirstLineSpace->SetCurSel( 0 );

	GetDlgItem(IDC_EdtTopSpace)->SetWindowText( _T("0") );


	CComboBox *pCobSerialFont = NULL;
	pCobSerialFont = (CComboBox*)GetDlgItem(IDC_CobSerialFont);
	pCobSerialFont->InsertString( 0, _T("기본 폰트") );
	pCobSerialFont->InsertString( 1, _T("한글 명조체 10.5p") );
	pCobSerialFont->InsertString( 2, _T("한글 고딕체 10.5p") );
	pCobSerialFont->InsertString( 3, _T("한글 명조체 14p") );
	pCobSerialFont->InsertString( 4, _T("한글 고딕체 14p") );
	pCobSerialFont->InsertString( 5, _T("한글 샘물체 10.5p") );
	pCobSerialFont->InsertString( 6, _T("한글 샘물체 14p") );
	pCobSerialFont->InsertString( 7, _T("한글 궁서체 10.5p") );
	pCobSerialFont->InsertString( 8, _T("한글 필기체 10.5p") );
	pCobSerialFont->InsertString( 9, _T("한글 디나루체 10.5p") );
	pCobSerialFont->InsertString( 10, _T("한글 옛체 10.5p") );
	pCobSerialFont->SetCurSel( 0 );

	CString sLineConfig;
	CString sFirstLoc, sSecLoc, sColnoLoc, sColCount;
	while( _fgetts( cLineConfig, 256, fpSerialConfig) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLineConfig );
		//CString sItem = sLine.Right(10);
		INT iFind = sLine.Find( _T(":") );
		CString sItem = sLine.Mid( iFind+1 );		
		sItem.Replace( _T(" "), NULL );
		sItem.Replace( _T("\r\n"), NULL );

		if ( sLine.Left(1) == _T("1") ) {
			GetDlgItem(IDC_EdtFisrtLoc)->SetWindowText( sItem );		
		}
		else if ( sLine.Left(1) == _T("2") ) {
			GetDlgItem(IDC_EdtSecLoc)->SetWindowText( sItem );
		}
		else if ( sLine.Left(1) == _T("3") ) {
			GetDlgItem(IDC_EdtRegnoLoc)->SetWindowText( sItem );
		}
		else if ( sLine.Left(1) == _T("4") ) {
			GetDlgItem(IDC_EdtColnoLoc)->SetWindowText( sItem );
		}
		else if ( sLine.Left(1) == _T("5") ) {
			GetDlgItem(IDC_EdtColCount)->SetWindowText( sItem );
		}
		else if ( sLine.Left(1) == _T("6") ) {
			if ( sItem == _T("출력함") ) {
				pCobRegnoPrintYN->SetCurSel( 0 );
			}
			else {
				pCobRegnoPrintYN->SetCurSel( 1 );
			}
		}
		else if ( sLine.Left(1) == _T("7") ) {
			/*
			if ( sItem == _T("공백있음") ) {
				pCobFirstLineSpace->SetCurSel( 0 );
			}
			else {
				pCobFirstLineSpace->SetCurSel( 1 );
			}
			*/
			GetDlgItem(IDC_EdtTopSpace)->SetWindowText( sItem );
		}
		else if ( sLine.Left(1) == _T("8") ) {
			if ( sItem == _T("기본폰트") ) {
				pCobSerialFont->SetCurSel( 0 );
			}
			else if ( sItem == _T("한글명조체10.5p") ) {
				pCobSerialFont->SetCurSel( 1 );
			}
			else if ( sItem == _T("한글고딕체10.5p") ) {
				pCobSerialFont->SetCurSel( 2 );
			}
			else if ( sItem == _T("한글명조체14p") ) {
				pCobSerialFont->SetCurSel( 3 );
			}
			else if ( sItem == _T("한글고딕체14p") ) {
				pCobSerialFont->SetCurSel( 4 );
			}
			else if ( sItem == _T("한글샘물체10.5p") ) {
				pCobSerialFont->SetCurSel( 5 );
			}
			else if ( sItem == _T("한글샘물체14p") ) {
				pCobSerialFont->SetCurSel( 6 );
			}
			else if ( sItem == _T("한글궁서체10.5p") ) {
				pCobSerialFont->SetCurSel( 7 );
			}
			else if ( sItem == _T("한글필기체10.5p") ) {
				pCobSerialFont->SetCurSel( 8 );
			}
			else if ( sItem == _T("한글디나루체10.5p") ) {
				pCobSerialFont->SetCurSel( 9 );
			}
			else if ( sItem == _T("한글옛체10.5p") ) {
				pCobSerialFont->SetCurSel( 10 );
			}
		}
	}
/*
	GetDlgItem(IDC_EdtFisrtLoc)->SetWindowText( _T("10") );
	GetDlgItem(IDC_EdtSecLoc)->SetWindowText( _T("30") );
	GetDlgItem(IDC_EdtRegnoLoc)->SetWindowText( _T("60") );
	GetDlgItem(IDC_EdtColnoLoc)->SetWindowText( _T("1") );
	GetDlgItem(IDC_EdtColCount)->SetWindowText( _T("2") );

	CComboBox *pCobRegnoPrintYN = NULL;
	pCobRegnoPrintYN = (CComboBox*)GetDlgItem(IDC_CobRegnoPrintYN);
	pCobRegnoPrintYN->InsertString( 0, _T("출력 함") );
	pCobRegnoPrintYN->InsertString( 1, _T("출력 안함") );		
	pCobRegnoPrintYN->SetCurSel( 0 );

	CComboBox *pCobFirstLineSpace = NULL;
	pCobFirstLineSpace = (CComboBox*)GetDlgItem(IDC_CobFirstLineSpace);
	pCobFirstLineSpace->InsertString( 0, _T("공백 있음") );
	pCobFirstLineSpace->InsertString( 1, _T("공백 없음") );		
	pCobFirstLineSpace->SetCurSel( 0 );
*/	
	fclose(fpSerialConfig);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_SERIAL_LABEL_CONFIG::OnbSAVE() 
{
	// TODO: Add your control notification handler code here
	CString sPathUnit = _T("..\\CFG\\정리_연속용지_출력.cfg");
	//파일읽어오기
	FILE *fpSerial;
	//TCHAR cLineUnit[256];
	fpSerial = _tfopen( sPathUnit , _T("wb") );
	if ( fpSerial == NULL ) {
		AfxMessageBox( _T("..\\CFG\\정리_연속용지_출력.cfg 파일 확인.") );
		return; 
	}
	fseek(fpSerial, 0, SEEK_END);		
	if( 0 == ftell(fpSerial) )
	{
		/*UNCHANGE*/fputc(0xFF, fpSerial);
		/*UNCHANGE*/fputc(0xFE, fpSerial);
	}	

	CString sFirstLoc, sSecLoc, sRegnoLoc, sColnoLoc, sColCount, sRegnoPrintYN, sFirstLineSpace, sSerialFont;
	GetDlgItem(IDC_EdtFisrtLoc)->GetWindowText( sFirstLoc );
	GetDlgItem(IDC_EdtSecLoc)->GetWindowText( sSecLoc );
	GetDlgItem(IDC_EdtRegnoLoc)->GetWindowText( sRegnoLoc );
	GetDlgItem(IDC_EdtColnoLoc)->GetWindowText( sColnoLoc );
	GetDlgItem(IDC_EdtColCount)->GetWindowText( sColCount );
	GetDlgItem(IDC_CobRegnoPrintYN)->GetWindowText( sRegnoPrintYN );
	GetDlgItem(IDC_EdtTopSpace)->GetWindowText( sFirstLineSpace );
	GetDlgItem(IDC_CobSerialFont)->GetWindowText( sSerialFont );

	sFirstLoc = _T("1.첫번째 청구기호 위치 : ") + sFirstLoc + _T("\r\n");
	sSecLoc = _T("2.두번째 청구기호 위치 : ") + sSecLoc + _T("\r\n");
	sRegnoLoc = _T("3.등록번호 위치 : ") + sRegnoLoc + _T("\r\n");
	sColnoLoc = _T("4.청구기호 간격 : ") + sColnoLoc + _T("\r\n");
	sColCount = _T("5.레이블 출력 줄수 : ") + sColCount + _T("\r\n");
	sRegnoPrintYN = _T("6.등록번호 출력 여부 : ") + sRegnoPrintYN + _T("\r\n");
	//sFirstLineSpace = _T("7.첫라인 공백여부 : ") + sFirstLineSpace + _T("\n");
	sFirstLineSpace = _T("7.윗 공백 수 : ") + sFirstLineSpace + _T("\r\n");
	sSerialFont = _T("8.폰트 : ") + sSerialFont + _T("\r\n");

	_ftprintf( fpSerial, sFirstLoc );
	_ftprintf( fpSerial, sSecLoc );
	_ftprintf( fpSerial, sRegnoLoc );
	_ftprintf( fpSerial, sColnoLoc );
	_ftprintf( fpSerial, sColCount );
	_ftprintf( fpSerial, sRegnoPrintYN );
	_ftprintf( fpSerial, sFirstLineSpace );
	_ftprintf( fpSerial, sSerialFont );
/*
	CString sUnit;
	while( _fgetts( cLineUnit, 256, fpUnit) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLineUnit );
		if ( sLine.Left(4) == _T("UNIT") ) {
			sUnit = sLine.Mid(7);
		}
	}
	*/
	fclose(fpSerial);

	AfxMessageBox( _T("저장 하였습니다.") );
}
