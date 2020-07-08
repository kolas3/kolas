// BO_CAT_PROC_0390.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0390.h"
#include "..\..\..\공통\K2UP_Common\MakeSearchData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0390 dialog


CBO_CAT_PROC_0390::CBO_CAT_PROC_0390(CESL_Mgr* pParent, CMarc &m_marc /*=NULL*/)
	: CESL_Mgr(CBO_CAT_PROC_0390::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0390)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

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
}

CBO_CAT_PROC_0390::~CBO_CAT_PROC_0390()
{
}

VOID CBO_CAT_PROC_0390::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0390)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0390, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0390)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bKEYWORD, OnbKEYWORD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0390 message handlers

BOOL CBO_CAT_PROC_0390::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0390::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	INT ids;
	ids = m_marcEditor.ApplyEdit();
	
	if (InitESL_Mgr(_T("BO_CAT_PROC_0390")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	pDM = FindDM( _T("DM_BO_CAT_PROC_0390_WORD") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0390_WORD") );
		return FALSE;
	}
	
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

	CArray<CString, CString&> pArray653;	
	CString strItemData;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("653$a"), strItemData, &pArray653);

	
	INT iYes = 0;
	for ( INT i = 0 ; i < pArray653.GetSize() ; i++ ) {
		sKeyword = sKeyword + pArray653.GetAt( i ) + _T(" ");
		pArray.Add( pArray653.GetAt( i ) );		
	}
	
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
		
	KeywordInsert( _T("245$a") );
	KeywordInsert( _T("245$b") );
	KeywordInsert( _T("245$c") );
	KeywordInsert( _T("245$p") );
	KeywordInsert( _T("245$x") );
	
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

	KeywordInsert( _T("057$t") );

	KeywordInsert( _T("500$b") );
	KeywordInsert( _T("500$e") );
	KeywordInsert( _T("500$f") );
	KeywordInsert( _T("500$g") );
	KeywordInsert( _T("500$h") );
	KeywordInsert( _T("500$i") );
	KeywordInsert( _T("500$j") );
	KeywordInsert( _T("500$k") );
	KeywordInsert( _T("500$r") );

	SetControlData(_T("CM_BO_CAT_PROC_0390"), _T("키워드"), sKeyword);
	
	m_marcEditor.SubclassDlgItem(IDC_reditKey, -1, this);

	CString strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc_key, strStreamMarc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc_key);
	m_marcEditor.Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_0390::KeywordMarcInsert( CString sTag )
{
EFS_BEGIN

	CString sValue;
	m_pInfo->pMarcMgr->GetField( &m_marc, sTag , sValue );
	if ( sValue != _T("") ) {
		m_pInfo->pMarcMgr->InsertField ( &m_marc_key, sTag + sValue );
	}	

	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_0390::KeywordInsert(CString sTag)
{
EFS_BEGIN

	CString sItemData = _T("");
	INT ids = m_pInfo->pMarcMgr->GetItem ( &m_marc , sTag, sItemData);
	if ( -1 != ids && 1 != ids) {
		AfxMessageBox ( _T("m_pInfo->pMarcMgr->GetItem Error") );
		return ;
	}

	if ( sTag == _T("245$a") ) {
		CString s245;
		m_pInfo->pMarcMgr->GetField( &m_marc, _T("245"), s245 );
		if ( s245.Left(1) == _T("1") ) {
			INT iStart = sItemData.Find( _T("(") );
			if ( iStart != -1 ) {
				INT iEnd = sItemData.Find( _T(")") );
				if ( iStart < iEnd ) {
					CString sTmp;
					sTmp = sItemData.Mid( iStart, iEnd-iStart+1 );
					sItemData.Replace( sTmp, _T("") );
				}
			}			
		}
		else if ( s245.Left(1) == _T("2") ) {
			sItemData.Replace( _T("(") , _T(" ") );
			sItemData.Replace( _T(")") , _T(" ") );

			INT iStart = sItemData.Find( _T(" ") );
			if ( iStart == 0 ) {
				sItemData = sItemData.Mid(1);
				iStart = sItemData.Find( _T(" ") );
			}
			while ( iStart != -1 ) {
				CString sToken;
				sToken = sItemData.Mid( 0, iStart );
				ids = KeywordMake( sToken );

				sItemData = sItemData.Mid( iStart + 1 );
				iStart = sItemData.Find( _T(" ") );
			}		
		}
	}

	ids = KeywordMake( sItemData );

EFS_END
}

INT CBO_CAT_PROC_0390::KeywordMake( CString sItemData )
{
EFS_BEGIN

	if ( !sItemData.IsEmpty() ) {
		CMakeSearchData *pMakeData = NULL;
		INT iCh = pMakeData->ChangeHanjaToHangul( sItemData );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sItemData ) error") );
			return -1;
		}
		iCh = pMakeData->GetKeyWord( sItemData );
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sItemData ) error") );
			return -1;
		}
		
		if ( sItemData != _T("") ) {
			INT iWord = SearchWord( sItemData );
			if ( iWord < 0 ) {
				AfxMessageBox( _T("SearchWord( sItemData ) error") );
			}

			CString sResult=_T("");
			INT iAuxiliaryWord = SearchAuxiliaryWord( sItemData, sResult );
			
			INT iUnlessWord = SearchUnlessWord( sItemData );

			INT ids;
			if ( iWord == 1 ) {
				ids = ArrayInsert( sItemData );
			}
			if ( iUnlessWord == 1 && iWord == 1 ) {
				CString sMessage;
				sMessage.Format( _T("'%s'는 단어와 불용어에 모두 포함되어 있습니다.") , sItemData );
				AfxMessageBox( sMessage );
			}
			if ( iWord != 1 && iAuxiliaryWord == 1 ) {
				ids = ArrayInsert ( sResult );
			}
			if ( iWord == 0 && iAuxiliaryWord == 0 && iUnlessWord == 0 ) {
				ids = ArrayInsert ( sItemData );
			}
		}
	}

	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0390::SearchWord(CString sWord)
{
EFS_BEGIN

	CString strWhere = _T("ROWNUM > 0");
	if (pDM->RefreshDataManager(strWhere) < 0)	return -1;

	INT iRowCount = pDM->GetRowCount();
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = pDM->GetCellData( _T("단어") , i );
		if ( sWordDb == sWord ) {
			return 1;
		}
	}
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0390::SearchAuxiliaryWord( CString sWord, CString &sResult )
{
EFS_BEGIN

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
	
	CString sQuery = _T("SELECT AUX_WORD FROM CO_AUXILIARY_WORD_TBL");
	dm.RestructDataManager(sQuery);

	INT iRowCount = dm.GetRowCount();
	CString sAuxWord = _T("");
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = dm.GetCellData( i, 0 );
		if ( sWordDb.IsEmpty() ) {
			continue;
		}
		INT iWordDb = sWordDb.GetLength();
		if ( sWord.GetLength() <= sWordDb.GetLength() ) {
			continue;
		}
		CString sTmp = sWord.Right( iWordDb );		

		if ( sWordDb == sTmp ) {
			if ( sAuxWord.IsEmpty() || sAuxWord.GetLength() > sWord.Left(iWordDb).GetLength() ) {
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

INT CBO_CAT_PROC_0390::SearchUnlessWord( CString sWord )
{
EFS_BEGIN

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);	
	
	CString sQuery = _T("SELECT AUX_WORD FROM CO_AUXILIARY_WORD_TBL");
	dm.RestructDataManager(sQuery);

	INT iRowCount = dm.GetRowCount();
	CString sAuxWord = _T("");
	for ( INT i = 0 ; i < iRowCount ; i++ ) {
		CString sWordDb = pDM->GetCellData( _T("단어") , i );
		if ( sWordDb == sWord ) {
			return 1;
		}
	}
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_PROC_0390::ArrayInsert( CString sItemData )
{
EFS_BEGIN

	INT iYes = 0;
	for ( INT i = 0 ; i < pArray.GetSize() ; i++ ) {
		if ( sItemData == pArray.GetAt( i ) ) {
			iYes = 1;
			break;
		}
	}
	if ( iYes == 0 ) {
		sKeyword = sKeyword + sItemData + _T(" ");
		pArray.Add( sItemData );
	}
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_PROC_0390::OnOK() 
{
EFS_BEGIN
	m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("653") );

	CString sItemData;
	GetControlData( _T("CM_BO_CAT_PROC_0390"), _T("키워드"), sItemData );
	sItemData.TrimLeft();
	sItemData.TrimRight();
	INT iStart = sItemData.Find( _T(" ") );
	if ( iStart == 0 ) {
		sItemData = sItemData.Mid(1);
		iStart = sItemData.Find( _T(" ") );
	}
	while ( iStart != -1 ) {
		CString sToken;
		sToken = sItemData.Mid( 0, iStart );
		
		m_pInfo->pMarcMgr->SetItem(&m_marc, _T("653$a"), sToken , _T(""), ADD_SUBFIELD);

		sItemData = sItemData.Mid( iStart + 1 );
		iStart = sItemData.Find( _T(" ") );
	}
	m_pInfo->pMarcMgr->SetItem(&m_marc, _T("653$a"), sItemData , _T(""), ADD_SUBFIELD);

	CDialog::OnOK();

EFS_END
}

VOID CBO_CAT_PROC_0390::OnbKEYWORD() 
{
	
}

HBRUSH CBO_CAT_PROC_0390::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
