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
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bMAKE_FILE, OnbMAKEFILE)
	ON_BN_CLICKED(IDC_chkDivision, OnchkDivision)
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

	INT curdrive;

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

	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	
	CFileDialog dlg(FALSE, _T("*.*"), _T("*.TXT"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
    if ( IDOK != dlg.DoModal() ) return;

	_tchdir(szCurrentDir);

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

    CStdioFile file, ref_file;

	CString sFileName;
	GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("파일명"), sFileName );
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
	
	CString sRegNoStart, sRegNoEnd;
	sRegNoStart = m_pInfo->MakeRegNo( sRegCode, sNoStart );
	sRegNoEnd = m_pInfo->MakeRegNo( sRegCode, sNoEnd );

	CString strWhere;

	CButton * pStartReg;
	pStartReg=(CButton *)GetDlgItem(IDC_chCLASS1);	

	CString sShelfCode;
	GetControlData( _T("CM_BO_CAT_PROC_1100"), _T("자료실구분"), sShelfCode );
	sShelfCode.Replace( _T(" "), NULL );
	
	if ( pStartReg->GetCheck() == 1 ) {
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
	
	CButton * pDivision;
	pDivision = (CButton *)GetDlgItem(IDC_chkDivision);	

	FILE *fp;
	if ( pDivision->GetCheck() != 1 ) {
		fp = _tfopen( sFileName , _T("wb") );
		if ( fp == NULL ) {
			AfxMessageBox(_T("파일을 선택하세요."));
			return;
		}
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}	
	
	if ( pDivision->GetCheck() == 1 ) {
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

			fp = _tfopen( sDivFileName, _T("wb") );

			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
			
			for ( INT j = 0 ; j < iDataCount ; j++ ) {
				CString sMarc;
				INT iRow = i*iDataCount + j;
				sMarc = pDM->GetCellData( _T("BS_MARC"), iRow );

				if ( pMarcInfo->GetCheck() == 0 ) {	
					
					CMarc marc;

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

					m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
				}
				else
				{

				}

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

			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
			
			for ( i = 0 ; i < iLastCount ; i++ ) {
				
				CString sMarc;
				INT iRow = iFileCount*iDataCount + i;
				sMarc = pDM->GetCellData( _T("BS_MARC"), iRow );

				if ( pMarcInfo->GetCheck() == 0 ) {
					CMarc marc;
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

					m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
				}
				else
				{

				}

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
				CMarc marc;
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

				m_pInfo->pMarcMgr->Encoding( &marc, sMarc );
			}
			else
			{

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

VOID CBO_CAT_PROC_1100::OnchkDivision() 
{
EFS_BEGIN

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

HBRUSH CBO_CAT_PROC_1100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
