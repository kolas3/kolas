// BO_CAT_PROC_KEYWORD_0000.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_PROC_KEYWORD_0000.h"
//#include _T("BO_CAT_PROC_KEYWORD_UNLESS.h")
//#include _T("BO_CAT_AUXILIARY_WORD_0100.h")
//#include _T("BO_CAT_WORD_0100.h")
//#include _T("..\..\..\공통\IndexToolKit\IndexTool.h")

// 사전관리
#include "..\..\..\관리\문자변환관리\MngCharSetManagement\CatCharSetManagementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_KEYWORD_0000 dialog


CBO_CAT_PROC_KEYWORD_0000::CBO_CAT_PROC_KEYWORD_0000(CESL_Mgr* pParent, CMarc &m_marc /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_KEYWORD_0000)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();

	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

	pDM_KeyWord = NULL;
	m_MakeSearchData = new CMakeSearchData(this);
}
/*
CBO_CAT_PROC_KEYWORD_0000::~CBO_CAT_PROC_KEYWORD_0000()
{
	if(m_MakeSearchData)
	{
		delete m_MakeSearchData;
		m_MakeSearchData = NULL;
	}
}
*/


CBO_CAT_PROC_KEYWORD_0000::~CBO_CAT_PROC_KEYWORD_0000()
{
	if(m_MakeSearchData)
	{
		delete m_MakeSearchData;
		m_MakeSearchData = NULL;
	}
}

BOOL CBO_CAT_PROC_KEYWORD_0000::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_KEYWORD_0000::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_KEYWORD_0000)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_KEYWORD_0000, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_KEYWORD_0000)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bUNLESS_WORD_INFO, OnbUNLESSWORDINFO)
	ON_BN_CLICKED(IDC_bAUX_WORD, OnbAUXWORD)
	ON_BN_CLICKED(IDC_bWORD, OnbWORD)
	ON_BN_CLICKED(IDC_bKEYWORD, OnbKEYWORD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_KEYWORD_0000 message handlers

BOOL CBO_CAT_PROC_KEYWORD_0000::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0390")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	//단어
	pDM_KeyWord = FindDM( _T("DM_BO_CAT_PROC_0390_WORD") );
	if ( pDM_KeyWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_WORD") );
		return FALSE;
	}
	CString sPath = _T("..\\CFG\\CO_WORD_TBL.TXT");
	//단어 파일읽어오기
	FILE *fp;
	TCHAR cLine[256];
	fp = _tfopen( sPath , _T("rb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_WORD_TBL.txt 파일이 없습니다.") );
		return FALSE;
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
	
	INT iRowCount = 0;
	while( _fgetts( cLine, 256, fp) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		if ( sLine.Left(1) == _T(".") ) {
			continue;
		}
		INT iPos = sLine.Find(_T("|"));
		//CString sResult = sLine.Mid( iPos+1, 2 );		
		CString sResult = sLine.Left( iPos );
		sResult.TrimRight();
		pDM_KeyWord->InsertRow(-1);
		pDM_KeyWord->SetCellData( _T("단어"), sResult, iRowCount );
		iRowCount++;
	}	
	fclose( fp );

	//조사
	CESL_DataMgr *pDmAuxWord;
	pDmAuxWord = FindDM( _T("DM_BO_CAT_PROC_0390_AuxWord") );
	if ( pDmAuxWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_AuxWord") );
		return FALSE;
	}
	
	sPath = _T("..\\CFG\\CO_AUXILIARY_WORD_TBL.TXT");
	//조사 파일읽어오기
	fp = _tfopen( sPath , _T("rb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_AUXILIARY_WORD_TBL.txt 파일이 없습니다.") );
		return FALSE;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	fpUni = fp;

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
	
	iRowCount = 0;
	while( _fgetts( cLine, 256, fp) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		if ( sLine.Left(1) == _T(".") ) {
			continue;
		}
		INT iPos = sLine.Find(_T("|"));
		CString sResult = sLine.Left( iPos );
		sResult.TrimRight();
		pDmAuxWord->InsertRow(-1);
		pDmAuxWord->SetCellData( _T("조사"), sResult, iRowCount );
		iRowCount++;
	}	
	fclose( fp );


	//불용어
	CESL_DataMgr *pUnlessWord;
	pUnlessWord = FindDM( _T("DM_BO_CAT_PROC_0390_UnlessWord") );
	if ( pUnlessWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_UnlessWord") );
		return FALSE;
	}

	sPath = _T("..\\CFG\\CO_UNLESS_WORD_TBL.txt");
	//파일읽어오기
	fp = _tfopen( sPath , _T("rb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("CO_UNLESS_WORD_TBL.txt 파일이 없습니다.") );
		return FALSE;
	}
	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	fpUni = fp;

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
	
	iRowCount = 0;
	while( _fgetts( cLine, 256, fp) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLine );
		if ( sLine.Left(1) == _T(".") ) {
			continue;
		}
		INT iPos = sLine.Find(_T("|"));
		CString sResult = sLine.Left( iPos );
		sResult.TrimRight();
		pUnlessWord->InsertRow(-1);
		pUnlessWord->SetCellData( _T("불용어"), sResult, iRowCount );
		iRowCount++;
	}
	
	fclose( fp );

	Init(m_strStreamMarc);
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_KEYWORD_0000::Init(CString strStreamMarc)
{
	INT ids = -1;
	m_strStreamMarc = strStreamMarc;

	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	//653 tag 읽기
	CArray<CString, CString&> pArray653;	
	CString strItemData;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("653$a"), strItemData, &pArray653);

	//처음생성
	if ( strItemData == _T("") ) {
		m_sStart = _T("Y");
		GetDlgItem(IDC_bKEYWORD)->EnableWindow(false);
	}
	
	INT iYes = 0;
	for ( INT i = 0 ; i < pArray653.GetSize() ; i++ ) {
		m_strKeyword = m_strKeyword + pArray653.GetAt( i ) + _T(" ");
		if ( m_sStart == _T("Y") ) {
			pArray.Add( pArray653.GetAt( i ) );		
		}
	}

	//바탕화면 마크에서 값 읽고 m_marc_key 구조체에 넣기
	ids = KeywordMarcInsert( _T("245") );
	ids = KeywordMarcInsert( _T("440") );
	ids = KeywordMarcInsert( _T("490") );
	ids = KeywordMarcInsert( _T("130") );
	ids = KeywordMarcInsert( _T("240") );
	ids = KeywordMarcInsert( _T("730") );
	ids = KeywordMarcInsert( _T("740") );
	ids = KeywordMarcInsert( _T("830") );
	ids = KeywordMarcInsert( _T("930") );
	ids = KeywordMarcInsert( _T("940") );
	ids = KeywordMarcInsert( _T("949") );
	ids = KeywordMarcInsert( _T("507") );
	ids = KeywordMarcInsert( _T("500") );

	//키워드 넣기
	KeywordInsert( _T("245$a") );
	KeywordInsert( _T("245$b") );
	KeywordInsert( _T("245$c") );
	KeywordInsert( _T("245$p") );
	KeywordInsert( _T("245$x") );
	
	//440 insert
	KeywordInsert( _T("440$a") );
	KeywordInsert( _T("440$p") );
	KeywordInsert( _T("440$x") );


	KeywordInsert( _T("490$a") );
	KeywordInsert( _T("490$p") );
	KeywordInsert( _T("490$x") );

	KeywordInsert( _T("130$a") );
	KeywordInsert( _T("130$p") );

	KeywordInsert( _T("240$a") );
	KeywordInsert( _T("240$p") );

	KeywordInsert( _T("730$a") );
	KeywordInsert( _T("730$p") );

	KeywordInsert( _T("740$a") );
	KeywordInsert( _T("740$p") );

	KeywordInsert( _T("830$a") );
	KeywordInsert( _T("830$p") );

	KeywordInsert( _T("930$a") );
	KeywordInsert( _T("930$p") );

	KeywordInsert( _T("940$a") );
	KeywordInsert( _T("940$p") );

	KeywordInsert( _T("949$a") );
	KeywordInsert( _T("949$p") );

	KeywordInsert( _T("507$t") );

	KeywordInsert( _T("500$b") );
	KeywordInsert( _T("500$e") );
	KeywordInsert( _T("500$f") );
	KeywordInsert( _T("500$g") );
	KeywordInsert( _T("500$h") );
	KeywordInsert( _T("500$i") );
	KeywordInsert( _T("500$j") );
	KeywordInsert( _T("500$k") );
	KeywordInsert( _T("500$r") );

	SetControlData(_T("CM_BO_CAT_PROC_0390"), _T("키워드"), m_strKeyword);

	m_marcEditor.SubclassDlgItem(IDC_reditKey, -1, this);

	//encoding
	strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc_key, strStreamMarc);

	//insert marc
	m_marcEditor.InitUserAlias();
	m_marcEditor.AddUserAlias(_T("본표제"));
	m_marcEditor.AddUserAlias(_T("저작자"));
	m_marcEditor.AddUserAlias(_T("발행지"));
	m_marcEditor.AddUserAlias(_T("발행자"));
	m_marcEditor.AddUserAlias(_T("발행년"));
	m_marcEditor.AddUserAlias(_T("입력날짜"));
	m_marcEditor.AddUserAlias(_T("발행년1"));
	m_marcEditor.AddUserAlias(_T("유형"));

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc_key);
	m_marcEditor.Display();

	CButton *pBtn = ( CButton * )GetDlgItem( IDOK );
	pBtn->SetFocus();

	return 0;
}

INT CBO_CAT_PROC_KEYWORD_0000::KeywordMarcInsert( CString sTag )
{
EFS_BEGIN

	CString sValue;
	CArray<CString, CString&> arrItem;
	m_pInfo->pMarcMgr->GetField( &m_marc, sTag , sValue, &arrItem);

	INT nCount = arrItem.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		sValue = arrItem.GetAt(idx);
		if ( sValue != _T("") ) {
			m_pInfo->pMarcMgr->InsertField ( &m_marc_key, sTag + sValue );
		}		
	}
	

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_KEYWORD_0000::KeywordInsert(CString sTag)
{
EFS_BEGIN
	CString strItemData;
	CArray<CString, CString&> arrItem;
	INT ids = m_pInfo->pMarcMgr->GetItem ( &m_marc , sTag, strItemData, &arrItem);
	if (ids < 0) 
	{
		///AfxMessageBox ( _T("m_pInfo->pMarcMgr->GetItem Error") );
		return ;
	}
	
	LPTSTR lpszTok = NULL;
	
	INT nRowCount = arrItem.GetSize();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		//245tag일 경우 지시기호를 조사하여 키워드 생성
		//제1지시기호가 1일경우 관칭을 삭제하고 키워드 생성
		//제1지시기호가 2일경우 관칭을 포함해서 키워드 생성	

		strItemData = arrItem.GetAt(idx);
		if (strItemData.IsEmpty()) continue;

		if ( sTag == _T("245$a") ) 
		{
			CString s245;
			m_pInfo->pMarcMgr->GetField( &m_marc, _T("245"), s245 );
			if ( s245.Left(1) == _T("1") ) {
				//관칭제거
				INT iStart = strItemData.Find( _T("(") );
				if ( iStart != -1 ) 
				{
					INT iEnd = strItemData.Find( _T(")") );
					if ( iStart < iEnd ) {
						CString sTmp;
						sTmp = strItemData.Mid( iStart, iEnd-iStart+1 );
						strItemData.Replace( sTmp, _T("") );
					}
				}			
			}
			else if ( s245.Left(1) == _T("2") ) 
			{
				//관칭포함
				strItemData.Replace( _T("(") , _T(" ") );
				strItemData.Replace( _T(")") , _T(" ") );
				
				INT iStart = strItemData.Find( _T(" ") );
				if ( iStart == 0 ) {
					strItemData = strItemData.Mid(1);
					iStart = strItemData.Find( _T(" ") );
				}
				while ( iStart != -1 ) {
					CString sToken;
					sToken = strItemData.Mid( 0, iStart );
					//키워드 생성
					ids = KeywordMake( sToken );
					
					strItemData = strItemData.Mid( iStart + 1 );
					iStart = strItemData.Find( _T(" ") );
				}		
			}
		}
		
		strItemData.Replace( _T("(") , _T("") );
		strItemData.Replace( _T(")") , _T("") );
		
		strItemData.TrimLeft();
		strItemData.TrimRight();
		
		lpszTok = _tcstok(strItemData.GetBuffer(0), _T(" "));
		while (lpszTok)
		{
			ids = KeywordMake(lpszTok);

			lpszTok = _tcstok(NULL, _T(" "));
		}
		/*
		
		INT iStart = strItemData.Find( _T(" ") );
		if ( iStart == 0 ) {
			strItemData = strItemData.Mid(1);
			iStart = strItemData.Find( _T(" ") );
		}
		while ( iStart != -1 ) 
		{
			CString sToken;
			sToken = strItemData.Mid( 0, iStart );
			//키워드 생성
			ids = KeywordMake( sToken );
			
			strItemData = strItemData.Mid( iStart + 1 );
			iStart = strItemData.Find( _T(" ") );
		}		
		
		ids = KeywordMake( strItemData );
		*/
	}

EFS_END
}

INT CBO_CAT_PROC_KEYWORD_0000::KeywordMake( CString strItemData )
{
EFS_BEGIN
	if ( !strItemData.IsEmpty() ) {
		INT iCh = m_MakeSearchData->ChangeHanjaToHangul(strItemData);	//한자 한글 변환
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strItemData ) error") );
			return -1;
		}
		iCh = m_MakeSearchData->GetKeyWord(strItemData);	//공백, 특수문자 제거
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strItemData ) error") );
			return -1;
		}

		if ( strItemData != _T("") ) {
			//단어에 포함될 경우 키워드 생성
			INT iWord = SearchWord( strItemData );
			if ( iWord < 0 ) {
				AfxMessageBox( _T("SearchWord( strItemData ) error") );
			}
			
			//조사에 포함될 경우 조사를 삭제하고 키워드 생성
			CString sResult=_T("");
			INT iAuxiliaryWord = SearchAuxiliaryWord( strItemData, sResult );
			
			// 불용어에 포함될 경우 키워드를 생성하지 않는다.
			INT iUnlessWord = SearchUnlessWord( strItemData );

			INT ids;
			if ( iWord == 1 ) {
				ids = ArrayInsert( strItemData );
			}
			if ( iUnlessWord == 1 && iWord == 1 ) {
				CString sMessage;
				sMessage.Format( _T("'%s'는 단어와 불용어에 모두 포함되어 있습니다.") , strItemData );
				AfxMessageBox( sMessage );
			}
			if ( iWord != 1 && iAuxiliaryWord == 1 ) {
				ids = ArrayInsert ( sResult );
			}
			if ( iWord == 0 && iAuxiliaryWord == 0 && iUnlessWord == 0 ) {
				ids = ArrayInsert ( strItemData );
			}
		}
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_KEYWORD_0000::SearchWord(CString sWord)
{
EFS_BEGIN
	
	INT iRowCount = pDM_KeyWord->GetRowCount();
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = pDM_KeyWord->GetCellData( _T("단어") , i );
		if ( sWordDb == sWord ) {
			return 1;
		}
	}
	
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_KEYWORD_0000::SearchAuxiliaryWord( CString sWord, CString &sResult )
{
EFS_BEGIN

	CESL_DataMgr *pDmAuxWord;
	pDmAuxWord = FindDM( _T("DM_BO_CAT_PROC_0390_AuxWord") );
	if ( pDmAuxWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_AuxWord") );
		return FALSE;
	}

	INT iRowCount = pDmAuxWord->GetRowCount();
	CString sAuxWord = _T("");
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = pDmAuxWord->GetCellData( i, 0 );
		if ( sWordDb.IsEmpty() ) {
			continue;
		}
		INT iWordDb = sWordDb.GetLength();
		if ( sWord.GetLength() <= sWordDb.GetLength() ) {
			continue;
		}
		CString sTmp = sWord.Right( iWordDb );		

		if ( sWordDb == sTmp ) {
			//===================================================
			//2010.05.20 UPDATE BY PJW : 조사를 제한 단어 비교시 원래 데이터의 길이에서 조사의 길이를 제하고 비교한다.
//			if ( sAuxWord.IsEmpty() || sAuxWord.GetLength() > sWord.Left(iWordDb).GetLength() ) 
			if ( sAuxWord.IsEmpty() || sAuxWord.GetLength() > sWord.Left(sWord.GetLength() - iWordDb).GetLength() ) 
			//===================================================			
			{
				INT iPosition;
				iPosition = sWord.GetLength() - iWordDb;
				sAuxWord = sWord.Left( iPosition );
			}
		}
	}
	if ( !sAuxWord.IsEmpty() ) {
		sResult = sAuxWord;
		return 1;
	}
	return 0;

EFS_END
return -1;

}

//INT CESL_DataMgr::GetCellData(INT rowidx, INT idx, CString &result)
INT CBO_CAT_PROC_KEYWORD_0000::SearchUnlessWord( CString sWord )
{
EFS_BEGIN

	CESL_DataMgr *pUnlessWord;
	pUnlessWord = FindDM( _T("DM_BO_CAT_PROC_0390_UnlessWord") );
	if ( pUnlessWord == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_UnlessWord") );
		return FALSE;
	}

	INT iRowCount = pUnlessWord->GetRowCount();
	CString sAuxWord = _T("");
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = pUnlessWord->GetCellData( _T("불용어") , i );
		if ( sWordDb == sWord ) {
			return 1;
		}
	}
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_KEYWORD_0000::ArrayInsert( CString strItemData )
{
EFS_BEGIN

	INT iYes = 0;
	for ( INT i = 0 ; i < pArray.GetSize() ; i++ ) {
		if ( strItemData == pArray.GetAt( i ) ) {
			iYes = 1;
			break;
		}
	}
	if ( iYes == 0 ) {
		if ( m_sStart == _T("Y") ) {
			m_strKeyword = m_strKeyword + strItemData + _T(" ");
		}
		pArray.Add( strItemData );
	}
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_KEYWORD_0000::MakeKeyWord()
{
	m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("653") );
	
	//화면에서 공백으로 끊어서 단어 추출
	CString strItemData;
	GetControlData( _T("CM_BO_CAT_PROC_0390"), _T("키워드"), strItemData );
	strItemData.TrimLeft();
	strItemData.TrimRight();
	INT iStart = strItemData.Find( _T(" ") );
	if ( iStart == 0 ) {
		strItemData = strItemData.Mid(1);
		iStart = strItemData.Find( _T(" ") );
	}
	while ( iStart != -1 ) {
		CString sToken;
		sToken = strItemData.Mid( 0, iStart );
		//===================================================
		//2010.01.07 ADD BY PJW : 데이터가 null일 경우는 생성하지 않는다.
//		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("653$a"), sToken , _T(""), ADD_SUBFIELD);
		if( !sToken.IsEmpty() )
		{
			m_pInfo->pMarcMgr->SetItem(&m_marc, _T("653$a"), sToken , _T(""), ADD_SUBFIELD);
		}
		//===================================================

		strItemData = strItemData.Mid( iStart + 1 );
		iStart = strItemData.Find( _T(" ") );
	}		
	//화면에서 공백으로 끊어서 단어 추출
	//marc에 insert
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("653$a"), strItemData , _T(""), ADD_SUBFIELD);

	//키워드 생성땜시 추가..
	CString strStreamMarc;
	m_pInfo->pMarcMgr->Encoding( &m_marc, strStreamMarc ) ;
	SetStreamMarc (strStreamMarc);

}

VOID CBO_CAT_PROC_KEYWORD_0000::OnOK() 
{
EFS_BEGIN
	MakeKeyWord();

	CDialog::OnOK();

EFS_END
}

BOOL CBO_CAT_PROC_KEYWORD_0000::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_KEYWORD_0000::OnbUNLESSWORDINFO() 
{
EFS_BEGIN
	
	CCatCharSetManagementDlg dlg(this);
	dlg.SetCatagory(_T("불용어"));
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_0000::OnbAUXWORD() 
{
EFS_BEGIN

	CCatCharSetManagementDlg dlg(this);
	dlg.SetCatagory(_T("조사"));
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_0000::OnbWORD() 
{
EFS_BEGIN

	CCatCharSetManagementDlg dlg(this);
	dlg.SetCatagory(_T("단어"));
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_KEYWORD_0000::OnbKEYWORD() 
{
	m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("653") );

	m_strKeyword.Empty();
	
	for ( INT i = 0 ; i < pArray.GetSize() ; i++ ) {
		m_strKeyword += _T(" ") + pArray.GetAt( i );
	}

	m_strKeyword.TrimLeft();
	m_strKeyword.TrimRight();
	
	SetControlData( _T("CM_BO_CAT_PROC_0390"), _T("키워드"), m_strKeyword);
}

HBRUSH CBO_CAT_PROC_KEYWORD_0000::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}