// MarcExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcExportDlg.h"
#include <direct.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\..\..\공통\MarcAdjust\MarcAdjustConstant.h"
#include "..\bo_cat_export_exception_tag\ExportExceptionTag.h"
#include "..\\..\\..\\공통\\MarcConvertManager\\MarcConvertMgr.h"
#include "ECO_FILE_API.h"

/////////////////////////////////////////////////////////////////////////////
// CMarcExportDlg dialog


CMarcExportDlg::CMarcExportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcExportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;

	m_nModeMONO_SERIAL = cMODE_MONO;
}

CMarcExportDlg::~CMarcExportDlg()
{
}

BOOL CMarcExportDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}


VOID CMarcExportDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcExportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP


	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcExportDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcExportDlg)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bMAKE_FILE, OnbMAKEFILE)
	ON_BN_CLICKED(IDC_chkDivision, OnchkDivision)
	ON_BN_CLICKED(IDC_bException_Tag, OnbExceptionTag)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcExportDlg message handlers

BOOL CMarcExportDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_정리_마크반출")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_정리_마크반출"));
	if (m_pCM == NULL) 
	{
		AfxMessageBox(_T("CM Init Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}


	m_pDM = FindDM(_T("DM_정리_마크반출_단행"));
	
	static_cast<CButton*>(GetDlgItem(IDC_chkMarcInfo))->SetCheck(1);
	static_cast<CButton*>(GetDlgItem(IDC_rGeneralMarc))->SetCheck(1);

	CTime t_start = CTime::GetCurrentTime();
	CString strPath, filename;

	filename.Format(_T("..\\BIN\\(단행)반출마크_%04d%02d%02d.TXT"), t_start.GetYear(), t_start.GetMonth(), t_start.GetDay());

	GetFullPathName( filename , MAX_PATH, strPath.GetBuffer(MAX_PATH), NULL);


	m_pCM->SetControlMgrData(_T("파일명"), strPath );

	( (CButton*) GetDlgItem(IDC_rUTF16) )->SetCheck(TRUE);

	CString strValue;
	GetManageValue( _T("기타") , _T("공통") , _T("5163") , _T("5163") , strValue );
	if ( strValue != _T("Y") )
		GetDlgItem(IDC_bException_Tag)->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  

	EFS_END
	return FALSE;
}

VOID CMarcExportDlg::OnbFILEOPEN() 
{
EFS_BEGIN


	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT 파일 열기");
	OFN.lpstrDefExt = _T("TXT");
	
	BOOL bSuccess = GetSaveFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
		m_pCM->SetControlMgrData(_T("파일명"), OFN.lpstrFile);


EFS_END
}

VOID CMarcExportDlg::OnbMAKEFILE() 
{
EFS_BEGIN

	CMarc marc;
	CString strFileName;
	CString strOption;
	CString strMarcStream;
	CString strExpFileName;
	CString strBeforeSKey;
	CString strSpeciesKey;

	CESL_DataMgr * pDM;

	INT idxData = 0;
	INT nDataCount = 0;

	FILE *file;

	m_pCM->GetControlMgrData( _T("파일명"), strFileName );
	if ( strFileName.IsEmpty() ) 
	{
		AfxMessageBox( _T("파일명을 입력해야 합니다.") );
		return;
	}

	if ( IDNO == AfxMessageBox( _T("파일을 작성하시겠습니까?") , MB_YESNO) ) return;
	
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	
	_tsplitpath(strFileName, drive, dir, fname, ext);

	pDM = new CESL_DataMgr();
	pDM->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	pDM->InitDMPKFromFile(_T("DM_정리_마크반출_단행"));
	pDM->MANAGE_CODE = m_pInfo->MANAGE_CODE;
	
	INT ids = MakeOptionQuery(strOption);
	if (ids < 0) return;

	if (m_pDM->RefreshDataManager(strOption) < 0) return;
	
	INT nRowCount = m_pDM->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		return;
	}

	INT nIdx = 0;
	CString strTempSpeciesKey;
	BOOL bDupFlag;
	for( INT i = 0 ; i<nRowCount ; i++ )
	{
		ids = m_pDM->GetCellData(_T("BS_종KEY"),i, strSpeciesKey);
		bDupFlag = FALSE;

		for ( INT j = 0 ; j < i ; j ++ )
		{
			ids = m_pDM->GetCellData(_T("BS_종KEY"),j, strTempSpeciesKey);
			if( strSpeciesKey == strTempSpeciesKey )
			{
				bDupFlag = TRUE;
				break;
			}
		}
		
		if ( bDupFlag == FALSE )
		{
			pDM->InsertRow(-1);		
			pDM->SetCellData(_T("BS_MARC"),m_pDM->GetCellData(_T("BS_MARC"),i),nIdx);
			pDM->SetCellData(_T("BS_종KEY"),m_pDM->GetCellData(_T("BS_종KEY"),i),nIdx);
			pDM->SetCellData(_T("MARC"),m_pDM->GetCellData(_T("MARC"),i),nIdx);
			nIdx++;
		}		
	}
	
	if( nIdx > 0 )
		nRowCount = nIdx;

	ids = MakeMarcInfo(pDM);
	if (ids < 0) return;

	BOOL bDivisionFile = FALSE;
	if (static_cast<CButton*>(GetDlgItem(IDC_chkDivision))->GetCheck() == 1 )  bDivisionFile = TRUE;

	INT nDataCountPerFile = 0;
	INT nFileCount = 0;
	if (bDivisionFile)
	{
		CString strDataCount;
		m_pCM->GetControlMgrData(_T("건수"), strDataCount);
		nDataCountPerFile = _ttoi(strDataCount);
		if (nDataCountPerFile == 0) 
		{
			AfxMessageBox(_T("건수를 선택해주세요"));
			return;
		}

		nFileCount = static_cast<INT>(ceil(static_cast<DOUBLE>(nRowCount) / static_cast<DOUBLE>(nDataCountPerFile)));
	}
	else
	{
		nFileCount = 1;
		nDataCountPerFile = nRowCount;
	}
	
	BOOL bCheck;
	INT	nEncodingType;

	bCheck = ( (CButton*) GetDlgItem(IDC_rANSI) )->GetCheck();
	if ( bCheck ) nEncodingType = ANSI_MARC;
	bCheck = ( (CButton*) GetDlgItem(IDC_rUTF8) )->GetCheck();
	if ( bCheck ) nEncodingType = UTF8_MARC;
	bCheck = ( (CButton*) GetDlgItem(IDC_rUTF16) )->GetCheck();
	if ( bCheck ) nEncodingType = UTF16_MARC;
	
	CMarcConvertMgr MarcConvertMgr(this);
	for (INT idxFile = 0; idxFile < nFileCount; idxFile++)
	{
		if (nFileCount > 1)
			strExpFileName.Format(_T("%s%s%s_%05d%s"), drive, dir, fname, idxFile, ext);
		else
			strExpFileName = strFileName;
		
		if ( nEncodingType == UTF16_MARC )
		{
			file = FdkFopen( strExpFileName, _T("w") );	

			if ( &file == NULL )
			{
				AfxMessageBox(_T("반출화일 생성에 실패하였습니다"));
				fclose(file);
				return;
			}
		}
		
		else if ( nEncodingType == UTF8_MARC )
		{
			file = _tfopen ( strExpFileName , _T("w"));

			if ( &file == NULL )
			{
				AfxMessageBox(_T("반출화일 생성에 실패하였습니다"));
				fclose(file);
				return;			
			}
			BYTE pb[3] = { 0xEF, 0xBB, 0xBF };
			fwrite(pb,1,3,file );
		}
		else 
		{
			file = _tfopen ( strExpFileName , _T("w"));

			if ( &file == NULL )
			{
				AfxMessageBox(_T("반출화일 생성에 실패하였습니다"));
				fclose(file);
				return;			
			}		
		}		
		
		if (nFileCount > 1)
			idxData = idxFile * nDataCountPerFile;

		nDataCount = idxData + nDataCountPerFile;
		CMarc pMarc;
		for (; idxData < nDataCount; idxData++)
		{
			if (idxData > nRowCount - 1) break;
			ids = pDM->GetCellData(_T("MARC"), idxData, strMarcStream);
			if (ids < 0 || strMarcStream.IsEmpty()) continue;

			DeleteExceptionTag( strMarcStream );
	
			if ( ANSI_MARC == nEncodingType )
			{
				m_pInfo->pMarcMgr->Decoding( strMarcStream, &pMarc );	
				m_pInfo->pMarcMgr->m_nEncodingCharset = CP_ACP;
				m_pInfo->pMarcMgr->Encoding( &pMarc, strMarcStream, TRUE );
				
				CHAR *szpMarc;
				szpMarc = NULL;
				MarcConvertMgr.UnicodeToAnsi(strMarcStream, &szpMarc);
				fprintf( file, "%s\n", szpMarc );
				free(szpMarc);
			}
			else if ( UTF8_MARC == nEncodingType )
			{
				m_pInfo->pMarcMgr->Decoding( strMarcStream, &pMarc );	
				m_pInfo->pMarcMgr->m_nEncodingCharset = CP_UTF8;
				m_pInfo->pMarcMgr->Encoding( &pMarc, strMarcStream, TRUE );

				CHAR *szpMarc;
				szpMarc = NULL;
				MarcConvertMgr.UnicodeToUTF8(strMarcStream, &szpMarc);
				fprintf( file, "%s\n", szpMarc );				
				free(szpMarc);
			}			
			else 
			{				
				FdkFprintf( file, _T("%s\r\n"), strMarcStream );
			}
		}		

		fclose(file);
		m_pInfo->pMarcMgr->m_nEncodingCharset = CP_UTF8;
	}

	CString sMessage;
	sMessage.Format( _T("검색된 %d건에 대한 파일작성이 완료되었습니다."), nRowCount );
	AfxMessageBox( sMessage );

EFS_END
}


VOID CMarcExportDlg::OnchkDivision() 
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

INT CMarcExportDlg::MakeOptionQuery(CString &strOption)
{
EFS_BEGIN

	strOption.Empty();

	CString sRegCode;
	CString sNoStart, sNoEnd;	
	CString sRegNoStart, sRegNoEnd;
	CButton * pStartReg;
	CString sShelfCode;

	m_pCM->GetControlMgrData( _T("등록구분"), sRegCode );
	sRegCode.Replace( _T(" "), NULL );
	if ( sRegCode == _T("") ) {
		AfxMessageBox( _T("등록구분을 선택하세요.") );
		return - 1;
	}

	m_pCM->GetControlMgrData( _T("등록번호부터"), sNoStart );
	m_pCM->GetControlMgrData( _T("등록번호까지"), sNoEnd );

	if ( sNoStart == _T("") || sNoEnd == _T("") ) 
	{
		AfxMessageBox( _T("등록번호를 입력하세요.") );
		return -1;
	}

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	
	sRegNoStart = m_pInfo->MakeRegNo( sRegCode, sNoStart );
	sRegNoEnd = m_pInfo->MakeRegNo( sRegCode, sNoEnd );	

	pStartReg=(CButton *)GetDlgItem(IDC_chCLASS1);	

	m_pCM->GetControlMgrData( _T("자료실구분"), sShelfCode );
	sShelfCode.Replace( _T(" "), NULL );

	if ( pStartReg->GetCheck() == 1 ) {
		strOption.Format( _T("( bo_get_min_reg_no(s.rec_key) between '%s' AND  '%s' ) AND (B.reg_no between '%s' AND '%s') "), sRegNoStart, sRegNoEnd, sRegNoStart, sRegNoEnd );
		if ( sShelfCode != _T("") ) 
			strOption = strOption + _T(" AND S.REC_KEY IN ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE SHELF_LOC_CODE ='") + sShelfCode + _T("' GROUP BY SPECIES_KEY)");
	}
	else {
		strOption.Format( _T("B.REG_NO >= '%s' AND B.REG_NO <= '%s'"), sRegNoStart, sRegNoEnd );
		if ( sShelfCode != _T("") ) {
			strOption.Format( _T("B.REG_NO >= '%s' AND B.REG_NO <= '%s' AND B.SHELF_LOC_CODE ='%s'"), sRegNoStart, sRegNoEnd, sShelfCode );
		}
	}

	strOption += _T(" AND B.MANAGE_CODE=UDF_MANAGE_CODE");
	

	return 0;

EFS_END
	return -1;
}

INT CMarcExportDlg::MakeMarcInfo(CESL_DataMgr *pDM)
{
EFS_BEGIN

	if (pDM == NULL) return -1;
	INT ids = -1;
	CMarc marc;
	POSITION pos = NULL;
	CTag *pTag = NULL;
	CString strHoldInfo;
	CString strStreamMarc;
	CString sRegCode;
	CString sNoStart, sNoEnd;

	CButton * pDelRegNo;

	BOOL bMarcInfo = FALSE;
	if (static_cast<CButton*>(GetDlgItem(IDC_chkMarcInfo))->GetCheck() == 1 )  
	{
		bMarcInfo = TRUE;
	}
	
	pDelRegNo = (CButton *)GetDlgItem(IDC_chCLASS2);

	m_pCM->GetControlMgrData( _T("등록구분"), sRegCode );
	sRegCode.Replace( _T(" "), NULL );

	m_pCM->GetControlMgrData( _T("등록번호부터"), sNoStart );
	m_pCM->GetControlMgrData( _T("등록번호까지"), sNoEnd );

	INT nRowCount = pDM->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		ids = pDM->GetCellData(_T("BS_MARC"), idx, strStreamMarc);
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) continue;
		
		if (!bMarcInfo)
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
		}

		if ( pDelRegNo->GetCheck() == 1 ) {
			CString strRegNo, strRegCode;
			INT nRegNum;
			CArray<CString, CString&> p049lArray;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("049$l"), strRegNo, &p049lArray );
			for ( INT j=0 ; j < p049lArray.GetSize() ; j++ ) {
				strRegNo = p049lArray.GetAt(j);
				strRegCode = strRegNo.Mid(0, strRegNo.GetLength()-10);
				nRegNum = _ttoi(strRegNo.Mid(strRegNo.GetLength()-10, 10));

				if (sRegCode.Compare(strRegCode) 
					|| nRegNum < _ttoi(sNoStart)
					|| nRegNum > _ttoi(sNoEnd) )
				{
					m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$l"), strRegNo );
				}
			}
		}
		
		ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc,TRUE);
		ids = pDM->SetCellData(_T("MARC"), strStreamMarc, idx);
	}

	return 0;

EFS_END
	return -1;
}


VOID CMarcExportDlg::OnbExceptionTag() 
{
EFS_BEGIN

	CExportExceptionTag dlg(this);
	dlg.DoModal();

EFS_END
}

VOID CMarcExportDlg::DeleteExceptionTag( CString &strStreamMarc )
{
EFS_BEGIN

	FILE *file;
	file = _tfopen( _T("..\\CFG\\ExceptionTag.CFG"), _T("rb") ); 
	if ( file == NULL ) return;

	FILE* fpUni = file;

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
	
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

	CString sLine;
	TCHAR cfileLine[256];

	_fgetts( cfileLine , 256, file);
	AfxMessageBox(strStreamMarc);

	sLine.Format( _T("%s"), cfileLine );
	INT iFind = sLine.Find( _T(";") );
	while( iFind > 0 ) {

		iFind = sLine.Find( _T(";") );
		CString sTag = sLine.Left( iFind );

		m_pInfo->pMarcMgr->DeleteField( &marc , sTag );

		sLine = sLine.Mid( iFind +1 );
	}
	
	m_pInfo->pMarcMgr->Encoding( &marc , strStreamMarc );
	AfxMessageBox(strStreamMarc);

EFS_END
}

HBRUSH CMarcExportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
