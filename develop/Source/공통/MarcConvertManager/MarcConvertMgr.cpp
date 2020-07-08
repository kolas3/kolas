// MarcConvertMgr.cpp : implementation file
//

#include "stdafx.h"
#include "MarcConvertMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcConvertMgr dialog

CMarcConvertMgr::CMarcConvertMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcConvertMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_nEncoding = UTF16_MARC;
	m_strPath = _T("");
}

CMarcConvertMgr::~CMarcConvertMgr()
{

}


void CMarcConvertMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcConvertMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarcConvertMgr, CDialog)
	//{{AFX_MSG_MAP(CMarcConvertMgr)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_rANSI, OnrANSI)
	ON_BN_CLICKED(IDC_rUTF8, OnrUTF8)
	ON_BN_CLICKED(IDC_rUTF16, OnrUTF16)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcConvertMgr message handlers

BOOL CMarcConvertMgr::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
		
	return CDialog::Create(IDD, pParentWnd);
}

// 1.Convert
// 1.1 UNICODE Marc -> ANSI Marc
INT CMarcConvertMgr::UnicodeToAnsi(CString strSrcMarc, CHAR** szpDestMarc)
{
	EFS_BEGIN	

	CString strDestMarc;
	INT		nLength;	
	
	DirectoryConvert(strSrcMarc, strDestMarc, ANSI_MARC );
	m_cvt.UnicodeToMultiByte(strDestMarc.GetBuffer(0), szpDestMarc, &nLength);

	return 0;
	EFS_END
	return -1;
}

INT CMarcConvertMgr::UnicodeToAnsi(TCHAR* szpSrcMarc, CHAR** szpDestMarc)
{
	EFS_BEGIN
	

	return 0;
	EFS_END
	return -1;
}

// 1.2 UNICODE Marc -> UTF8 Marc
INT CMarcConvertMgr::UnicodeToUTF8(CString strSrcMarc, CHAR** szpDestMarc)
{
	EFS_BEGIN	
		
	CString strDestMarc;
	INT		nLength;
	
	DirectoryConvert(strSrcMarc, strDestMarc, UTF8_MARC );
	m_cvt.UnicodeToUTF8(strDestMarc.GetBuffer(0), szpDestMarc, &nLength);

	return 0;
	EFS_END
	return -1;
}

INT CMarcConvertMgr::UnicodeToUTF8(TCHAR* szpSrcMarc, CHAR** szpDestMarc)
{
	EFS_BEGIN


	return 0;
	EFS_END
	return -1;
}

// 1.3 ANSI Marc	-> UNICODE Marc
INT CMarcConvertMgr::AnsiToUnicode(CHAR *szpSrcMarc, CString &strDestMarc)
{
	CString strSrcMarc;
	TCHAR	*szpDestMarc;
	INT		nLength;
	
	m_cvt.MultiByteToUnicode(szpSrcMarc, &szpDestMarc, &nLength );	

	strSrcMarc.Format(_T("%s"), szpSrcMarc);
	free(szpDestMarc);

	DirectoryConvert(strSrcMarc, strDestMarc, UTF16_MARC );

	return 0;
}

// 1.4 UTF8			-> UNICODE Marc
INT CMarcConvertMgr::UTF8ToUnicode(CHAR *szpSrcMarc, CString &strDestMarc)
{
	CString strSrcMarc;
	TCHAR	*szpDestMarc;
	INT		nLength;
	
	m_cvt.UTF8ToUnicode(szpSrcMarc, &szpDestMarc, &nLength );	

	strSrcMarc.Format(_T("%s"), szpSrcMarc);
	free(szpDestMarc);

	DirectoryConvert(strSrcMarc, strDestMarc, UTF16_MARC );

	return 0;
}

INT CMarcConvertMgr::DirectoryConvert(CString strSrcMarc, CString &strDestMarc, INT nConvertType)
{
	EFS_BEGIN 

	CMarc* pMarc;
	INT		ids;

	pMarc = new CMarc;
	ids = m_pInfo->pMarcMgr->Decoding ( strSrcMarc , pMarc );
	if ( ids < 0 ) return ids;

	// Directory
	INT n = 0;
	INT nDirCnt = pMarc->m_listTag.GetCount();
	const INT nLeaderLength = 24;
	const INT nDirTagLength = 3;
	const INT nDirFieldLength = 4;
	const INT nDirFieldInitAddrLength = 5;
	INT nFieldLength;
	
	CString strDirs;
	TCHAR szDir[nDirTagLength + nDirFieldLength + nDirFieldInitAddrLength + 1];
	ZeroMemory(szDir, nDirTagLength + nDirFieldLength + nDirFieldInitAddrLength + 1);

	// fields
	INT nFieldStartAddress = 0;
	CString strField;
	CString strTotalField;
	
	CString strFieldTerminatorCode;	
	strFieldTerminatorCode.Format(_T("%c"), FIELD_TERMINATOR);	
	
	POSITION pos = pMarc->m_listTag.GetHeadPosition();
	while (pos)
	{
		n = 0;
		CTag* pTag = (CTag*)pMarc->m_listTag.GetNext(pos);
		if (IsControlField(pTag->GetTagCode()))
		{
			strField = pTag->GetItemData();
			strField += pTag->GetFieldTerminator();
		}
		else
		{
			strField = (pTag->GetFirstIndicator() + pTag->GetSecondIndicator());	// Add Indicator
			POSITION posSub = pTag->m_listSubfield.GetHeadPosition();
			while (posSub)
			{
				CSubfield *pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSub);
				if (pSubfield == NULL) continue;

				// 처음 구두점은 넣지 않음
				// Add punctuation mark
				if (n++ != 0) 
					strField += pSubfield->GetPunctuationMark();
				// Add Subfield code
				strField += pSubfield->GetSubfieldCode();	
				// Add ItemData
				strField += pSubfield->GetItemData();		 
				
			}

			strField += strFieldTerminatorCode;			
		}
		// Add directory...
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		CString sRT;
		sRT.Format(_T("%c"),RECORD_TERMINATOR);
		if( 0 <= strField.Find(sRT) )
			strField.Replace(sRT,_T(""));
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++				
		if ( nConvertType == ANSI_MARC )
		{
			strField.TrimRight();
			nFieldLength = GetLengthUnicodeToAnsi(strField)-1;
		}
		else if ( nConvertType == UTF8_MARC )
		{
			strField.TrimRight();
			nFieldLength = GetLengthUnicodeToUTF8(strField)-1;
		}
		else
		{
			strField.TrimRight();
			nFieldLength = strField.GetLength();			
		}		

		_stprintf(szDir, _T("%03s%04d%05d"), pTag->GetTagCode(), nFieldLength, nFieldStartAddress);
		strDirs += szDir;

		nFieldStartAddress += nFieldLength;
		strTotalField += strField;		
	}

	// Add field terminate code to end of directory
	strDirs += strFieldTerminatorCode;

	// Add record terminate code to end of field
	strTotalField += pMarc->GetRecordTerminator();

	// Leader	
	TCHAR szLeader[25];
	ZeroMemory(szLeader, 25);
	
	pMarc->GetLeader(szLeader);

	// 레코드 길이
	INT nRecordLength;
	// 데이터기본번지
	INT nDataBaseAddr;
	if ( nConvertType == ANSI_MARC )
	{
		// 레코드 길이
		nRecordLength = nLeaderLength + GetLengthUnicodeToAnsi(strDirs) + GetLengthUnicodeToAnsi(strTotalField) - 2;
		// 데이터기본번지
		nDataBaseAddr = nLeaderLength + GetLengthUnicodeToAnsi(strDirs) - 1;
	}
	else if ( nConvertType == UTF8_MARC )
	{
		// 레코드 길이
		nRecordLength = nLeaderLength + GetLengthUnicodeToUTF8(strDirs) + GetLengthUnicodeToUTF8(strTotalField);
		// 데이터기본번지
		nDataBaseAddr = nLeaderLength + GetLengthUnicodeToUTF8(strDirs);
	}
	else
	{
		// 레코드 길이
		nRecordLength = nLeaderLength + strDirs.GetLength() + strTotalField.GetLength();
		// 데이터기본번지
		nDataBaseAddr = nLeaderLength + strDirs.GetLength();
	}	
	
		
	TCHAR szBuffer[6];
	ZeroMemory(szBuffer, 6);

	// input record length
	INT nPos;
	
	_stprintf(szBuffer, _T("%05d"), nRecordLength);
	nPos = 0;
	wmemcpy(szLeader + nPos, szBuffer, 5);

	// input data base address
	ZeroMemory(szBuffer, 6);
	
	_stprintf(szBuffer, _T("%05d"), nDataBaseAddr);
	nPos = 12;
	wmemcpy(szLeader + nPos, szBuffer, 5);
	
	pMarc->SetLeader(szLeader);
	strDestMarc = szLeader + strDirs + strTotalField;

	int nStreamLength;


	if ( nConvertType == ANSI_MARC )
	{
		nStreamLength = GetLengthUnicodeToAnsi(strDestMarc);
	}
	else if ( nConvertType == UTF8_MARC )
	{
		nStreamLength = GetLengthUnicodeToUTF8(strDestMarc);
	}
	else
	{
		nStreamLength = strDestMarc.GetLength();			
	}

	return nStreamLength;

	EFS_END
	return -1;
}

/***************************************************
3. UNICODE -> ANSI의 필요 바이트를 얻어온다.
***************************************************/
INT CMarcConvertMgr::GetLengthUnicodeToAnsi(CString strData )
{
	INT nNeedByte;

	nNeedByte = WideCharToMultiByte( CP_ACP	, 
								0				,
								strData.GetBuffer(0), 
								-1				,
								NULL			, 
								0				,
								NULL			, 
								NULL      );

	return nNeedByte;
}
/***************************************************
4. UNICODE -> UTF8의 필요 바이트를 얻어온다.
***************************************************/
INT CMarcConvertMgr::GetLengthUnicodeToUTF8(CString strData )
{
	INT nNeedByte;

	nNeedByte = WideCharToMultiByte( CP_UTF8	, 
								0				,
								strData.GetBuffer(0), 
								-1				,
								NULL			, 
								0				,
								NULL			, 
								NULL      );

	return nNeedByte;
}
/***************************************************
5. ANSI-> UNICODE의 필요 바이트를 얻어온다.
***************************************************/
INT CMarcConvertMgr::GetLengthAnsiToUniCode(CHAR* cpData )
{
	INT nNeedByte;
	nNeedByte = MultiByteToWideChar( CP_ACP	, 
								0				,
								cpData,
								-1				,
								NULL			, 
								NULL      );
								
	return nNeedByte;
}


/***************************************************
6. UTF8-> UNICODE의 필요 바이트를 얻어온다.
***************************************************/
INT CMarcConvertMgr::GetLengthUtf8ToUniCode(CHAR* cpData )
{
	INT nNeedByte=0;
	nNeedByte = MultiByteToWideChar( CP_UTF8	, 
								0				,
								cpData,
								-1				,
								NULL			, 
								NULL      );
	return nNeedByte;
}

BOOL CMarcConvertMgr::IsControlField(CString strTagCode)
{
	EFS_BEGIN

	INT nTag = _ttoi(strTagCode);
	if (nTag < 10)
		return TRUE;
	else 
		return FALSE;
	EFS_END
	return FALSE;
}

void CMarcConvertMgr::OnOK() 
{
	( (CEdit*) GetDlgItem (IDC_eFILE_NAME) )->GetWindowText(m_strPath);	

	m_strPath.TrimLeft();
	m_strPath.TrimRight();
	if ( m_strPath.IsEmpty() ) 
	{
		AfxMessageBox(_T("파일이 지정되지 않았습니다."));
		return;
	}
		
	CDialog::OnOK();
}

void CMarcConvertMgr::OnCancel() 
{
		
	CDialog::OnCancel();
}

void CMarcConvertMgr::OnbFILEOPEN() 
{
	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	CTime t_start = CTime::GetCurrentTime();
	CString strPath, filename;

	filename.Format(_T("(단행)반출마크_%04d%02d%02d%02d%02d%02d.TXT"), t_start.GetYear(), t_start.GetMonth(), t_start.GetDay(), t_start.GetHour(), t_start.GetMinute(), t_start.GetSecond() );

	GetFullPathName( filename , MAX_PATH, strPath.GetBuffer(MAX_PATH), NULL);	

	CFileDialog dlg(FALSE, _T("txt"), filename,
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					// 16/03/29 김혜영 : 한셀 적용하기 - 파일저장형식이 잘못되어 있어 수정
/*// BEFORE ----------------------------------------------------------------------------------
                    _T("파일형식(*.txt)|All Files(*.*)|*.*||") );
*/// AFTER -----------------------------------------------------------------------------------
					_T("TXT문서(*.txt)|*.txt|모든파일(*.*)|*.*||") );	
//*/ END -------------------------------------------------------------------------------------
	
	CString strDefaultPath = dlg.GetPathName();	
    if ( IDOK != dlg.DoModal() ) return ;

	_tchdir(szCurrentDir);	

	CString sFileName;
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return;
    sFileName = dlg.GetPathName();	

	( (CEdit*) GetDlgItem (IDC_eFILE_NAME) )->SetWindowText(sFileName);
	m_strPath = sFileName;

}

BOOL CMarcConvertMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	( (CButton*) GetDlgItem (IDC_rUTF16) )->SetCheck(TRUE);	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMarcConvertMgr::OnrANSI() 
{
	m_nEncoding = ANSI_MARC;	
}

void CMarcConvertMgr::OnrUTF8() 
{
	m_nEncoding = UTF8_MARC;	
}

void CMarcConvertMgr::OnrUTF16() 
{
	m_nEncoding = UTF16_MARC;	
}


// 다이얼로그 지원 옵션
INT CMarcConvertMgr::GetEncodingType()
{
	return m_nEncoding;
}

// 반출경로
CString CMarcConvertMgr::GetPath()
{
	return m_strPath;
}



HBRUSH CMarcConvertMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
