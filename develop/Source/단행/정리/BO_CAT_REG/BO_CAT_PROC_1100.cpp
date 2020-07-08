// BO_CAT_PROC_1100.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_1100.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1100 dialog


CBO_CAT_PROC_1100::CBO_CAT_PROC_1100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_1100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_PROC_1100::~CBO_CAT_PROC_1100()
{
	
}


VOID CBO_CAT_PROC_1100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_1100)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_1100, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_1100)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bMAKE_FILE, OnbMAKEFILE)
	ON_BN_CLICKED(IDC_chkDivision, OnchkDivision)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_1100 message handlers

BOOL CBO_CAT_PROC_1100::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}
BOOL CBO_CAT_PROC_1100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_1100")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pMarcInfo;
	pMarcInfo=(CButton *)GetDlgItem(IDC_chkMarcInfo);	
	pMarcInfo->SetCheck(1);

	CButton * pGeneralMarc;
	pGeneralMarc = (CButton *)GetDlgItem(IDC_rGeneralMarc);
	pGeneralMarc->SetCheck(1);

	//INT ch, drive, curdrive;	
	INT curdrive;
	/* Save current drive. */
	curdrive = _getdrive();
	CString sPath;
	if(curdrive==3){
		sPath=_T("C:\\KOLAS2UP\\BIN\\반출_AV.TXT");
	}
	if(curdrive==4){
		sPath=_T("D:\\KOLAS2UP\\BIN\\반출_AV.TXT");
	}
	if(curdrive==5){
		sPath=_T("E:\\KOLAS2UP\\BIN\\반출_AV.TXT");
	}

	sPath = _T("..\\BIN\\반출_AV.TXT");
	CString sFileName;
	//GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("파일명"), sFileName );
	SetControlData( _T("CM_BO_CAT_PROC_1100"), _T("파일명"), sPath );

	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_1100::OnbFILEOPEN() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, _T("*.*"), _T("*.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return;

    //_tchdir( sCurrentDirectory );

    // 파일명이 선택되지않고 '확인'을 click했을 경우.
	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return;
    sFileName = dlg.GetPathName();	

	SetControlData( _T("CM_BO_CAT_PROC_1100"), _T("파일명"), sFileName );

EFS_END
}

VOID CBO_CAT_PROC_1100::OnbMAKEFILE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	//if ( IDNO == AfxMessageBox( _T("파일을 작성하시겠습니까?") , MB_YESNO) ) return;

	//=================================================================
    // File Check / Open
    //=================================================================
    //TCHAR file_name[512];
    CStdioFile file, ref_file;
//	INT ids;
    //ids = ECO_GetDialogManagerData( '.', _T("CTRL_기본화면"), _T("파일명"), -1, file_name );
    //if (ids) CHECKERROR_RETURN(_T("ECO_GetDialogManagerData() Error!!!"), FALSE);
/*
    if ( _tcsclen( file_name ) <= 0 ) {
        _stprintf( msg, _T("파일명을 입력해야 합니다.") );
        CHECKERROR_RETURN(msg, FALSE);
    }

    if ( !file.Open( file_name, CFile::modeCreate | CFile::modeWrite ) ) {
        _stprintf( msg, _T("파일명이 잘못되었습니다.\n올바른 경로인지 확인해 주십시오.") );
        CHECKERROR_RETURN(msg, FALSE);
    }
    file.Close();
	
	if ( !ref_file.Open( REF_FILE_NAME, CFile::modeCreate | CFile::modeWrite ) )
        CHECKERROR_RETURN(_T("ref_file.Open() Error!!!"), FALSE);

    ref_file.WriteString( file_name );
    ref_file.WriteString( _T("\n") );
    ref_file.Close();

	// 파일을 작성한다.
    file.WriteString( sStreamMarc );
	*/

	/*
	INT ch, drive, curdrive;	
	
	curdrive = _getdrive();
	CString sPath;
	if(curdrive==3){
		sPath=_T("C:\\KOLAS2UP\\BIN\\반출_AV.TXT");
	}
	if(curdrive==4){
		sPath=_T("D:\\KOLAS2UP\\BIN\\반출_AV.TXT");
	}
	if(curdrive==5){
		sPath=_T("E:\\KOLAS2UP\\BIN\\반출_AV.TXT");
	}
	*/

	CString sFileName;
	GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("파일명"), sFileName );
	//SetControlData( _T("CM_BO_CAT_PROC_1100"), _T("파일명"), sPath );
	if ( sFileName.IsEmpty() ) {
		AfxMessageBox( _T("파일명을 입력해야 합니다.") );
		return;
	}

	CString sRegCode;
	GetControlData ( _T("CM_BO_CAT_PROC_1100"), _T("등록구분"), sRegCode );
	sRegCode.Replace( _T(" "), NULL );
	if ( sRegCode == _T("") ) {
		AfxMessageBox( _T("등록구분을 선택하세요.") );
		return;
	}

	CString sNoStart, sNoEnd;	
	GetControlData ( _T("CM_BO_CAT_PROC_1100"), _T("등록번호부터"), sNoStart );
	GetControlData ( _T("CM_BO_CAT_PROC_1100"), _T("등록번호까지"), sNoEnd );

	if ( sNoStart == _T("") || sNoEnd == _T("") ) {
		AfxMessageBox( _T("등록번호를 입력하세요.") );
		return;
	}

	if ( IDNO == AfxMessageBox( _T("파일을 작성하시겠습니까?") , MB_YESNO) ) return;

	CESL_DataMgr * pDM;
	pDM = FindDM( _T("DM_BO_CAT_PROC_1100") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1100 error") );
		return;
	}

	//등록번호 만들기	
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	CString sRegNoStart, sRegNoEnd;
// 	sRegNoStart.Format( _T("%s%010s"), sRegCode, sNoStart );
// 	sRegNoEnd.Format( _T("%s%010s"), sRegCode, sNoEnd );

	CString sRegNoStart, sRegNoEnd;
	sRegNoStart = m_pInfo->MakeRegNo( sRegCode, sNoStart );
	sRegNoEnd = m_pInfo->MakeRegNo( sRegCode, sNoEnd );
	//=====================================================
	

	CString strWhere;

	CButton * pStartReg;
	pStartReg=(CButton *)GetDlgItem(IDC_chCLASS1);	

	//자료실
	CString sShelfCode;
	GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("자료실구분"), sShelfCode );
	sShelfCode.Replace( _T(" "), NULL );
	
	if ( pStartReg->GetCheck() == 1 ) {		//첫등록번호 체크
		strWhere.Format( _T("bo_get_min_reg_no(rec_key) >= '%s' AND bo_get_min_reg_no(rec_key) <= '%s'"), sRegNoStart, sRegNoEnd );
		if ( sShelfCode != _T("") ) {
			strWhere = strWhere + _T(" AND REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SHELF_LOC_CODE ='") + sShelfCode + _T("')");
		}
	}
	else {
		strWhere.Format( _T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s')"), sRegNoStart, sRegNoEnd );
		if ( sShelfCode != _T("") ) {
			strWhere.Format( _T("REC_KEY IN (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REG_NO >= '%s' AND REG_NO <= '%s' AND SHELF_LOC_CODE ='%s')"), sRegNoStart, sRegNoEnd, sShelfCode );
		}
	}

	if (pDM->RefreshDataManager(strWhere) < 0)	return;
	INT iRowCount;
	iRowCount = pDM->GetRowCount();

	if ( pDM->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	CButton * pMarcInfo;
	pMarcInfo=(CButton *)GetDlgItem(IDC_chkMarcInfo);	
	
	//파일나누기
	CButton * pDivision;
	pDivision = (CButton *)GetDlgItem(IDC_chkDivision);	

	FILE *fp;
	//TCHAR tmp[256];
	if ( pDivision->GetCheck() != 1 ) {		//파일자동분할 아닐때
		fp = _tfopen( sFileName , _T("wb") );
		if ( fp == NULL ) {
			AfxMessageBox(_T("파일을 선택하세요."));
			return;
		}
				/*UNCHANGE*/fputc(0xFF, fp);
				/*UNCHANGE*/fputc(0xFE, fp);
		
		//_ftprintf( fp, _T("adf\r\n"));
	}
	
	
	if ( pDivision->GetCheck() == 1 ) {		//파일자동분할
		CString sDataCount;
		GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("건수"), sDataCount );
		if ( sDataCount == _T("") ) {
			AfxMessageBox( _T("건수를 선택하세요.") );
			return;
		}
		INT iDataCount;
		iDataCount = _ttoi( sDataCount.GetBuffer(0) );
		INT iFileCount, iLastCount;
		iFileCount = iRowCount / iDataCount;

		iLastCount = iRowCount % iDataCount;

		for ( INT i = 1 ; i < iFileCount ; i++ ) {
			FILE *fp;
			CString sFileCount;
			sFileCount.Format( _T("%d"), i );

			CString sDivFileName;
			sDivFileName.Format( _T("%s%s.TXT"), sFileName.Left( sFileName.GetLength()-4 ), sFileCount );

			//sFileName += sFileCount;
			fp = _tfopen( sDivFileName, _T("wb") );

			for ( INT j = 0 ; j < iDataCount ; j++ ) {
				CString sMarc;
				INT iRow = i*iDataCount + j;
				sMarc = pDM->GetCellData( _T("BS_MARC"), iRow );

				if ( pMarcInfo->GetCheck() == 0 ) {	
					//자관정보 빼기
					CMarc marc;
					//decoding
					CString strTagCode, strErrMsg, strHelpMsg;			
					if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc)) {
						POSITION pos;
						pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
						while (pos)	{
							m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
							UpdateData(FALSE);
						}	
					}
					m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
					m_pInfo->pMarcMgr->DeleteField( &marc, _T("090") );

					//Encoding
					m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
				}
				
				/*UNCHANGE*/fputc(0xFF, fp);
				/*UNCHANGE*/fputc(0xFE, fp);
		
				sMarc += _T("\r\n");
				_ftprintf( fp, sMarc );
			}
			fclose( fp );
		}

		if ( iLastCount != 0 ) {
			FILE *fp;

			CString sFileCount;
			sFileCount.Format( _T("%d"), iFileCount );

			CString sDivFileName;
			sDivFileName.Format( _T("%s%s.TXT"), sFileName.Left( sFileName.GetLength()-4 ), sFileCount );
			fp = _tfopen( sDivFileName, _T("wb") );

			for ( i = 0 ; i < iLastCount ; i++ ) {
				
				CString sMarc;
				INT iRow = iFileCount*iDataCount + i;
				sMarc = pDM->GetCellData( _T("BS_MARC"), iRow );

				if ( pMarcInfo->GetCheck() == 0 ) {	
					//자관정보 빼기
					CMarc marc;
					//decoding
					CString strTagCode, strErrMsg, strHelpMsg;			
					if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc)) {
						POSITION pos;
						pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
						while (pos)	{
							m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
							UpdateData(FALSE);
						}	
					}
					m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
					m_pInfo->pMarcMgr->DeleteField( &marc, _T("090") );

					//Encoding
					m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
				}
				
				/*UNCHANGE*/fputc(0xFF, fp);
				/*UNCHANGE*/fputc(0xFE, fp);

				sMarc += _T("\r\n");
				_ftprintf( fp, sMarc );
			}
			fclose( fp );
		}
	}
	else {
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			CString sMarc;
			sMarc = pDM->GetCellData( _T("BS_MARC"), i-1 );

			if ( pMarcInfo->GetCheck() == 0 ) {	
				//자관정보 빼기
				CMarc marc;
				//decoding
				CString strTagCode, strErrMsg, strHelpMsg;			
				if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc)) {
					POSITION pos;
					pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
					while (pos)	{
						m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
						UpdateData(FALSE);
					}	
				}
				m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
				m_pInfo->pMarcMgr->DeleteField( &marc, _T("090") );

				//Encoding
				m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
			}

			sMarc += _T("\n");
			_ftprintf( fp, sMarc );
		}
	}

	fclose( fp );

	CString sMessage;
	sMessage.Format( _T("검색된 %d건에 대한 파일작성이 완료되었습니다."), iRowCount );
	AfxMessageBox( sMessage );

EFS_END
}

INT CBO_CAT_PROC_1100::MakeFileSelected(CString sCMAlias, CString sGridAlias)
{
	EFS_BEGIN

	INT idx;
	SelectMakeList(sCMAlias, sGridAlias);
	//DELidx = SelectGetHeadPosition(sCMAlias, sGridAlias);
	idx = SelectGetTailPosition(sCMAlias, sGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return -1;		
	}

	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CFileDialog dlg(FALSE, _T("txt"), _T("반출.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return -1;

	_tchdir(szCurrentDir);

	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return -1;
    sFileName = dlg.GetPathName();	


	//if ( IDNO == AfxMessageBox( _T("파일을 작성하시겠습니까?") , MB_YESNO) ) return -1;

	CESL_DataMgr * pDmRegMain;
	pDmRegMain = FindDM( _T("DM_단행_정리_등록_메인") );
	if ( pDmRegMain == NULL ) {
		AfxMessageBox( _T("DM_단행_정리_등록_메인 확인") );
		return -1;
	}

	CESL_DataMgr * pDM;
	pDM = FindDM( _T("DM_BO_CAT_PROC_1100") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_1100 error") );
		return -1;
	}

	FILE *fp;
	fp = _tfopen( sFileName , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox(_T("파일을 선택하세요."));
		return -1;
	}
	
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);

	//idx = SelectGetNext(sCMAlias, sGridAlias);
	CString sBookKeyList;
	CStringArray aBookKeyList;
	while ( idx >= 0 ) {
		CString sBookKey = pDmRegMain->GetCellData( _T("IBS_종KEY") , idx );
		sBookKeyList = sBookKeyList + _T(", ") + sBookKey;

		if ( ((idx+1) % 999 ) == 0 ) {
		//if ( idx == 950 ) {
			aBookKeyList.Add( sBookKeyList );
			sBookKeyList = _T("");
			//break;
		}

		//DELidx = SelectGetNext(sCMAlias, sGridAlias);
		idx = SelectGetPrev(sCMAlias, sGridAlias);
	}
	//sBookKeyList = sBookKeyList.Mid(2);
	aBookKeyList.Add( sBookKeyList );
/*
	CString strWhere;
	strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
	if (pDM->RefreshDataManager(strWhere) < 0)	return -1;
*/
	INT iRowCount;
	INT iTotCount = 0;
	for ( INT j = 0 ; j < aBookKeyList.GetSize() ; j++ ) {
		//sBookKeyList = sBookKeyList.Mid(2);
		sBookKeyList = aBookKeyList.GetAt(j).Mid(2);

		CString strWhere;
		//strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
		//===================================================
		//2009.05.14 UPDATE BY LKS : 
		//strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" ) ORDER BY INPUT_DATE");
		strWhere = _T(" REC_KEY IN ( ") + sBookKeyList + _T(" )");
		//===================================================
		if (pDM->RefreshDataManager(strWhere) < 0)	return -1;

		iRowCount = pDM->GetRowCount();
		iTotCount = iTotCount + iRowCount;
	/*
		FILE *fp;
		fp = _tfopen( sFileName , _T("w") );
		if ( fp == NULL ) {
			AfxMessageBox(_T("파일을 선택하세요."));
			return -1;
		}
	*/
		for ( INT i = 1 ; i <= iRowCount ; i++ ) {
			CString sMarc;
			sMarc = pDM->GetCellData( _T("BS_MARC"), i-1 );
			/*
			if ( pMarcInfo->GetCheck() == 0 ) {	
				//자관정보 빼기
				CMarc marc;
				//decoding
				CString strTagCode, strErrMsg, strHelpMsg;			
				if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc)) {
					POSITION pos;
					pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
					while (pos)	{
						m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
						UpdateData(FALSE);
					}	
				}
				m_pInfo->pMarcMgr->DeleteField( &marc, _T("049") );
				m_pInfo->pMarcMgr->DeleteField( &marc, _T("090") );

				//Encoding
				m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
			}
			*/
			CMarc marc;
			m_pInfo->pMarcMgr->Decoding( sMarc, &marc );
			m_pInfo->pMarcMgr->Encoding( &marc, sMarc, TRUE );

			sMarc += _T("\r\n");
			_ftprintf( fp, sMarc );
		}
	}

	fclose( fp );

	CString sMessage;
	sMessage.Format( _T("선정된 %d건에 대한 파일작성이 완료되었습니다."), iTotCount );
	AfxMessageBox( sMessage );

	return 0;
	EFS_END
	return -1;
}

VOID CBO_CAT_PROC_1100::OnchkDivision() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CButton * pDivision;
	pDivision = (CButton *)GetDlgItem(IDC_chkDivision);	

	CButton * pDataCount;
	pDataCount = (CButton *)GetDlgItem(IDC_cDataCount);	
	if ( pDivision->GetCheck() == 1 ) {
		pDataCount->EnableWindow(TRUE);
	}
	else {
		pDataCount->EnableWindow(FALSE);
	}

EFS_END
}

VOID CBO_CAT_PROC_1100::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

HBRUSH CBO_CAT_PROC_1100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

