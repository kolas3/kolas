// MNG_AC_MANAGER_DOBORA.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_AC_MANAGER_DOBORA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_DOBORA dialog


CMNG_AC_MANAGER_DOBORA::CMNG_AC_MANAGER_DOBORA(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_AC_MANAGER_DOBORA)
		// NOTE: the ClassWizard will add member initialization here
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMarc = NULL;
	m_pMasterDM = NULL;
	m_pGrid = NULL;
	m_sACType = _T("0");
	m_bSelected = FALSE;
	//}}AFX_DATA_INIT
}

CMNG_AC_MANAGER_DOBORA::~CMNG_AC_MANAGER_DOBORA()
{
	if(!m_pMarc)
	{
		delete m_pMarc;
		m_pMarc = NULL;
	}
}

VOID CMNG_AC_MANAGER_DOBORA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_AC_MANAGER_DOBORA)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_AC_MANAGER_DOBORA, CDialog)
	//{{AFX_MSG_MAP(CMNG_AC_MANAGER_DOBORA)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_AC_MANAGER_DOBORA message handlers

VOID CMNG_AC_MANAGER_DOBORA::OnbCLOSE() 
{
EFS_BEGIN

	CDialog::OnCancel();
EFS_END
}

BOOL CMNG_AC_MANAGER_DOBORA::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(_T("도보라 조회"));
	
	if( InitESL_Mgr( _T("MNG_AC_MANAGER_DOBORA") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : MNG_AC_MANAGER_DOBORA") );
		return FALSE;
	}
	
	m_pDM = FindDM(_T("DM_MNG_AC_MANAGER_DOBORA"));
	if(m_pDM == NULL)
	{
		AfxMessageBox(_T("DM_MNG_AC_MANAGER_DOBORA not Found!!"));
		return FALSE;
	}

	m_pMasterDM = FindDM(_T("DM_ACMARCGetterDlg_MASTER"));
	if(m_pMasterDM == NULL)
	{
		AfxMessageBox(_T("DM_ACMARCGetterDlg_MASTER not Found!!"));
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_MNG_AC_MANAGER_DOBORA"));
	if(m_pCM == NULL)
	{
		AfxMessageBox(_T("CM_MNG_AC_MANAGER_DOBORA not Found!!"));
		return FALSE;
	}
	
	//Marc Editor 셋팅
	if (!m_marcEditor.SubclassDlgItem(IDC_RICHEDIT1, -1, this))
	{
		AfxMessageBox(_T("Subclass Marc Editor Failed"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	//마크 값 초기화
	if (m_pMarc)
		m_pMarc = NULL;
	m_pMarc = new CMarc;

	m_pInfo->pMarcMgr->SetMarcGroup(_T("전거"));

	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);
	m_marcEditor.Init(m_pInfo->pMarcMgr, m_pMarc);

	//메인 그리드 설정
	m_pGrid = static_cast<CESL_Grid*>(FindControl(_T("CM_MNG_AC_MANAGER_DOBORA"), _T("GRID")));
	if (m_pGrid == NULL) return FALSE;

	if ( !DoSearchRefItem() ) return FALSE;

	m_pCM->AllControlDisplay();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMNG_AC_MANAGER_DOBORA::DoSearchRefItem()
{
EFS_BEGIN

	CESL_DataMgr *pDM = NULL;

	//전거 테이블 셋팅
	CString sTBLNAME;

	if ( m_ACMODE == AUTHOR_MODE )
	{
		pDM = FindDM( _T("DM_ACMARCGetterDlg_Author") );
	}
	else if ( m_ACMODE == TITLE_MODE )
	{
		pDM = FindDM( _T("DM_ACMARCGetterDlg_Title") );
	}
	else if ( m_ACMODE = SUBJECT_MODE )
	{
		pDM = FindDM( _T("DM_ACMARCGetterDlg_Subject") );
	}
	if ( pDM ==  NULL ) return FALSE;
			

	CString strWhere;
	strWhere.Format ( _T(" KEYWORD = '%s' "), m_sRefItem ) ;

	pDM->RefreshDataManager(strWhere);

	INT keyCnt = pDM->GetRowCount();

	if( keyCnt < 1)
	{
		AfxMessageBox( _T("검색 결과가 없습니다.") );
		CDialog::OnCancel();
		return FALSE;
	}
	

	CString * ACKey = new CString[ keyCnt ];

	for( INT i = 0; i < keyCnt; i++ )
		pDM->GetCellData( _T("AC_MASTER_MAT_KEY"), i, ACKey[ i ] );

	SearchMasterACMarc( ACKey, keyCnt);

	delete [] ACKey;

	return TRUE;

EFS_END
return FALSE;
}

CString CMNG_AC_MANAGER_DOBORA::GetMarc()
{
	CString sMarc;
	m_pInfo->pMarcMgr->Encoding(m_pMarc, sMarc);
	return sMarc;
}  

INT CMNG_AC_MANAGER_DOBORA::GetACType()
{
	return _ttoi(m_sACType);
}

INT CMNG_AC_MANAGER_DOBORA::SearchMasterACMarc( CString *pk, INT cnt )
{
	CString sTmp = MakeSelectQuery( pk, cnt );
	
	INT ids; 

	ids = m_pMasterDM->RefreshDataManager( sTmp );
	if(ids < 0)
	{
		AfxMessageBox(_T("DM_ACMARCGetterDlg_MASTER not Found!!"));
		return -1;
	}
	
	CString sMarc;
	CMarc marc;
	for( INT i = 0; i < m_pMasterDM->GetRowCount(); i++ )
	{
		sMarc	= _T("");
		sTmp	= _T("");

		m_pMasterDM->GetCellData( _T("MARC"), i, sMarc );
		m_pInfo->pMarcMgr->Decoding(sMarc, &marc); 
		if(m_ACMODE == AUTHOR_MODE)
		{

			if(_T("0") == m_pMasterDM->GetCellData(_T("AC_TYPE"),i) )
			{
				m_pInfo->pMarcMgr->GetItem( &marc, _T("100$a"), sTmp );
				m_pMasterDM->SetCellData( _T("채택표목"), sTmp, i);
				continue;
			}
			if(_T("1") == m_pMasterDM->GetCellData(_T("AC_TYPE"),i) )
			{
				m_pInfo->pMarcMgr->GetItem( &marc, _T("110$a"), sTmp );
				m_pMasterDM->SetCellData( _T("채택표목"), sTmp, i);
				continue;
			}
			if(_T("0") == m_pMasterDM->GetCellData(_T("AC_TYPE"),i) )
			{
				m_pInfo->pMarcMgr->GetItem( &marc, _T("111$a"), sTmp );
				m_pMasterDM->SetCellData( _T("채택표목"), sTmp, i);
				continue;
			}
		}
		else if( m_ACMODE == TITLE_MODE )
		{
			m_pInfo->pMarcMgr->GetItem( &marc, _T("130$a"), sTmp );
			m_pMasterDM->SetCellData( _T("채택표목"), sTmp, i);
			continue;

		}
		else if( m_ACMODE == SUBJECT_MODE )
		{
			m_pInfo->pMarcMgr->GetItem( &marc, _T("150$a"), sTmp );
			m_pMasterDM->SetCellData( _T("채택표목"), sTmp, i);
			continue;

		}
		else
		{
			m_pMasterDM->SetCellData( _T("채택표목"), _T(""), i);
		}
	}
	
	DisplayMarc(0);
	return 0;
}

VOID CMNG_AC_MANAGER_DOBORA::DisplayMarc(INT nIdx)
{
	CString sMarc, sTmp;
	sMarc = m_pMasterDM->GetCellData(_T("MARC"),nIdx);
	if(sMarc.IsEmpty() == TRUE)
	{
		AfxMessageBox(_T("Marc 정보가 올바르지 않습니다."));
		return;
	}

	m_pInfo->pMarcMgr->Decoding(sMarc,m_pMarc);

	m_marcEditor.Display();
}


CString CMNG_AC_MANAGER_DOBORA::MakeSelectQuery( CString *pk, INT cnt )
{
	CString sWhere, tmp;
	sWhere = _T(" REC_KEY IN ( ");
	for( INT i = 0; i < cnt; i++ )
	{
		if( i == cnt -1 )
			sWhere += pk[i];
		else 
		{
			sWhere += pk[i];
			sWhere += _T(" , ");
		}
	}
	sWhere += _T(" ) ");

	return sWhere;
}

VOID CMNG_AC_MANAGER_DOBORA::OnOK() 
{
	if(!m_bSelected)
		m_sACType = m_pMasterDM->GetCellData(_T("AC_TYPE"),0);

	CDialog::OnOK();
}

BEGIN_EVENTSINK_MAP(CMNG_AC_MANAGER_DOBORA, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_AC_MANAGER_DOBORA)
	ON_EVENT(CMNG_AC_MANAGER_DOBORA, IDC_DoboraGRID, -605 /* MouseDown */, OnMouseDownDoboraGRID, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_AC_MANAGER_DOBORA::OnMouseDownDoboraGRID(short Button, short Shift, long x, long y) 
{
	long nRowLocation;
	nRowLocation = m_pGrid->GetRow()-1;
	
	m_sACType = m_pMasterDM->GetCellData(_T("AC_TYPE"),nRowLocation);
	DisplayMarc(nRowLocation);
}


HBRUSH CMNG_AC_MANAGER_DOBORA::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
