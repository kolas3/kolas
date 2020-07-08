// DonatorNameGetter_Search.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorNameGetter_Search.h"
#include "../K2UP_Common/MakeSearchData.h"
#include "../K2UP_Common/MulitComboBoxManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetter_Search dialog


CDonatorNameGetter_Search::CDonatorNameGetter_Search(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorNameGetter_Search::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorNameGetter_Search)
	m_nSearchType = -1;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
}


VOID CDonatorNameGetter_Search::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorNameGetter_Search)
	DDX_Radio(pDX, IDC_radID, m_nSearchType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorNameGetter_Search, CDialog)
	//{{AFX_MSG_MAP(CDonatorNameGetter_Search)
	ON_BN_CLICKED(IDC_radID, OnradID)
	ON_BN_CLICKED(IDC_radNAME, OnradNAME)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorNameGetter_Search message handlers

BOOL CDonatorNameGetter_Search::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr( _T("K2UPGRADE_기증자검색") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_02") );
		return false;
	}
	m_pCM = FindCM(_T("CM_기증자검색"));
	m_pDM = FindDM(_T("DM_기증자조회"));

	m_nSearchType = 0;
	UpdateData(FALSE);
	SetConditionContrl();

	CString arrDonatorType[2] = {
		_T("PN"), _T("GR")
	};
	
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("PN"), arrDonatorType, _T("기증자구분"), 2);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDonatorNameGetter_Search::SetConditionContrl()
{
	CWnd *pWnd;
	UINT wndID[ 6 ] = { IDC_cmbDONATOR_CLASS, IDC_edtDONATOR_REG_YEAR, IDC_edtDONATOR_SERIALNO1, IDC_edtDONATOR_SERIALNO2, IDC_edtDONATOR_NAME1, IDC_edtDONATOR_NAME2 };
	bool wndOption[ 6 ] = { false, false, false, false, false, false };
	INT i;
	
	UpdateData(TRUE);
	switch( m_nSearchType )
	{
		case 0 :
			for( i = 0; i < 4; i++ )
				wndOption[ i ] = true;
			break;
		case 1 :
			for( i = 4; i < 6; i++ )
				wndOption[ i ] = true;
			break;
		default :
			return;
			break;
	}

	for( i = 0; i < 6; i++ )
	{
		pWnd = ( CWnd * )GetDlgItem( wndID[ i ] );
		pWnd->EnableWindow( wndOption[ i ] );
	}		
}

VOID CDonatorNameGetter_Search::OnradID() 
{
	// TODO: Add your control notification handler code here
	SetConditionContrl();
}

VOID CDonatorNameGetter_Search::OnradNAME() 
{
	// TODO: Add your control notification handler code here

	SetConditionContrl();
}

VOID CDonatorNameGetter_Search::OnOK() 
{
	// TODO: Add extra validation here
	CMakeSearchData makeSearchData;
	CString strQuery, strOption, strTmpData, strTmpData1, strData;
	UpdateData(FALSE);
	
	switch( m_nSearchType )
	{
		case 0 :
			m_pCM->GetControlMgrData(_T("기증자구분"), strTmpData);
			if( !strTmpData.IsEmpty() )	strData = strTmpData;
			
			m_pCM->GetControlMgrData(_T("등록연도"), strTmpData);
			if( !strTmpData.IsEmpty() ) strData += strTmpData;
			else 
			{
				ESLAfxMessageBox(_T("등록년도를 설정해 주세요."));
				return;
			}

			m_pCM->GetControlMgrData(_T("일련번호1"), strTmpData);
			m_pCM->GetControlMgrData(_T("일련번호2"), strTmpData1);
			if(strTmpData.IsEmpty() && !strTmpData1.IsEmpty())
			{
				ESLAfxMessageBox(_T("시작 일련번호를 설정해 주십시요."));
				return;
			}
			else if(!strTmpData.IsEmpty() && strTmpData1.IsEmpty())
			{
				strData += strTmpData;
				strOption.Format(_T("DONATOR_ID = '%s' AND "), strData);
				strQuery += strOption;
			}
			else if(!strTmpData.IsEmpty() && !strTmpData1.IsEmpty() && strTmpData == strTmpData1)
			{
				strData += strTmpData;
				strOption.Format(_T("DONATOR_ID = '%s' AND "), strData);
				strQuery += strOption;
			}
			else if(!strTmpData.IsEmpty() && !strTmpData1.IsEmpty() && strTmpData != strTmpData1)
			{
				strOption.Format(_T("DONATOR_ID >= '%s' AND "), strData + strTmpData);
				strQuery += strOption;
				strOption.Format(_T("DONATOR_ID <= '%s' AND "), strData + strTmpData1);
				strQuery += strOption;
			}
			else if(strTmpData.IsEmpty() && strTmpData1.IsEmpty())
			{
				strOption.Format(_T("DONATOR_ID LIKE '%s%%' AND "), strData);
				strQuery += strOption;
			}
			
			break;

		case 1 :

			m_pCM->GetControlMgrData(_T("기증자명1"), strTmpData);
			if( !strTmpData.IsEmpty() )
			{
				makeSearchData.GetKeyWord(strTmpData);
				strOption.Format( _T("INDEX_DONATOR >= '%s' AND "), strTmpData );
				strQuery += strOption;
			}

			m_pCM->GetControlMgrData(_T("기증자명2"), strTmpData);
			if( !strTmpData.IsEmpty() )
			{
				makeSearchData.GetKeyWord(strTmpData);
				strOption.Format( _T("INDEX_DONATOR <= '%s' AND "), strTmpData );
				strQuery += strOption;
			}

			break;

		default :
			break;
	}

	m_pCM->GetControlMgrData(_T("등록일1"), strTmpData);
	if( !strTmpData.IsEmpty() )
	{
		strOption.Format( _T("INPUT_DATE >= '%s' AND "), strTmpData );
		strQuery += strOption;
	}

	m_pCM->GetControlMgrData(_T("등록일2"), strTmpData);
	if( !strTmpData.IsEmpty() )
	{
		strOption.Format( _T("INPUT_DATE <= '%s' AND "), strTmpData );
		strQuery += strOption;
	}

	if( _T("AND ") == strQuery.Right( 4 ) )
		strQuery = strQuery.Left( strQuery.GetLength() - 4 );

	m_pDM->RefreshDataManager(strQuery);
	
	CDialog::OnOK();
}
