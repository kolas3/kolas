// CSS_2_01.cpp : implementation file
//

#include "stdafx.h"
#include "CSS_2_01.h"
#include "CSS_2_02.h"
#include "CSS_2_03.h"
#include "CSS_2_04.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSS_2_01 dialog


CCSS_2_01::CCSS_2_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSS_2_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;

	m_pMSSQL_MGR = NULL;
}

CCSS_2_01::~CCSS_2_01()
{
	
}



VOID CCSS_2_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSS_2_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCSS_2_01, CDialog)
	//{{AFX_MSG_MAP(CCSS_2_01)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_bREQMGR, OnbREQMGR)
	ON_BN_CLICKED(IDC_BTN_REQUEST, OnBtnRequest)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSS_2_01 message handlers

VOID CCSS_2_01::SetMarcInfo( CMarcMgr *pMarcMgr, CMarc *pMarc )
{
	m_pMarcMgr = pMarcMgr ;
	m_pMarc = pMarc ;
}

VOID CCSS_2_01::OnBtnSearch() 
{
	//검색
	CString strKeyWord;
	GetDlgItem(IDC_EDT_KEYWORD)->GetWindowText(strKeyWord);
	strKeyWord.TrimLeft();
	strKeyWord.TrimRight();
	strKeyWord = _T("%") + strKeyWord + _T("%") ;


	CString strQuery;
	strQuery.Format(_T("SELECT id, name from term where sortname like '%%%s%%' order by sortname")
		,strKeyWord);

	FILE *fp;
	fp = _tfopen (_T("C:\\테스트1.txt"), _T("wb") );
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	_ftprintf( fp, strQuery );
	fclose(fp);

	m_pMSSQL_MGR->EXE_SELECT (strQuery,m_pDM, m_pCM) ;
	m_pCM->AllControlDisplay();
	return;
}

VOID CCSS_2_01::OnbREQMGR() 
{
	//내요청 
//	CCSS_REQ_INFO dlg( NULL, this, IDD_CSS_REQ_INFO_POP );
//	dlg.DoModal();
}

VOID CCSS_2_01::OnBtnRequest() 
{
	//요청하기 
	CCSS_2_03 dlg(this);
    GetDlgItemText(IDC_EDT_KEYWORD, dlg.m_sTerm);
	// 2003.10.15 wsw
//	dlg.SetSpeciesKey(m_szRecKey);
    dlg.DoModal();	
}

VOID CCSS_2_01::OnOK() 
{
	//Tag 생성
	CString sTerms;
	GetDlgItemText(IDC_EDT_SEL_TERM, sTerms);
	if(sTerms.GetLength() == 0) return;

	CCSS_2_02 dlg(this);
    dlg.SetTerms(sTerms);
	dlg.SetMarcInfo(m_pMarcMgr, m_pMarc);

	dlg.DoModal();

	CDialog::OnOK();
}

BOOL CCSS_2_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_주제어검색")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_주제어검색"));
	if (m_pCM == NULL) 
	{
		AfxMessageBox(_T("CM Init Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pDM = FindDM(_T("DM_주제어검색"));
	if ( m_pDM == NULL ) 
	{
		AfxMessageBox(_T("DM Init Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pDM_Relation = FindDM(_T("DM_주제어관계"));
	if ( m_pDM_Relation == NULL ) 
	{
		AfxMessageBox(_T("DM Init Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pMSSQL_MGR = new CMSSQL_MGR ;
	INT ids = m_pMSSQL_MGR->LoadDBinfo () ;
    if(ids<0)
	{
		AfxMessageBox(_T("DB_INFO 파일을 찾지 못하였습니다."));
		return FALSE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());
//	m_pGrid	= static_cast<CESL_Grid*>(m_pCM->FindControl(_T("SUBJECT_GRID")));
//	if ( m_pGrid == NULL )
//	{
//		AfxMessageBox ( _T(" Grid 초기화 에러") );
//		return FALSE;
//	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CCSS_2_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CCSS_2_01)
	ON_EVENT(CCSS_2_01, IDC_VCF_TERM, -600 /* Click */, OnClickVcfTerm, VTS_NONE)
	ON_EVENT(CCSS_2_01, IDC_VCF_THESARUS, -600 /* Click */, OnClickVcfThesarus, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CCSS_2_01::OnClickVcfTerm() 
{
	// 주제명 DB (TERM) 검색
	CMSHFlexGrid* pGrid;
	pGrid	= static_cast<CESL_Grid*>(m_pCM->FindControl(_T("SUBJECT_GRID")));
	INT nGridInx = SetCheckGrid( pGrid );

	ExeSearchThesaurus ( nGridInx );
}

INT CCSS_2_01::SetCheckGrid( CMSHFlexGrid* pGrid )
{

	INT row = pGrid->GetRow();
	INT col = pGrid->GetCol();

	if( 1 != col )
		return 0;

	if( _T("V") == pGrid->GetText() )
	{
		pGrid->SetText( _T("") );
	}

	for( INT i = 1; i < pGrid->GetRows(); i++ )
	{
		pGrid->SetRow( i );

		if( row == i )
		{
			pGrid->SetText( _T("V") );
		}
		else if( _T("V") == pGrid->GetText() )
			pGrid->SetText( _T("") );
	}

	pGrid->SetRow( row );
	return row;
}

VOID CCSS_2_01::ExeSearchThesaurus( INT nRowIdx )
{
	//검색된 용어로 검색한다.
	CString strTermID;
	m_pDM->GetCellData ( nRowIdx , 0 , strTermID );

	CString strQuery;
	strQuery.Format(_T("select th.relation, t.name, th.facet ")
                _T("from term t,thesaurus th,relation r ")
                _T("where t.id=th.reltermid and th.relation=r.relation and th.termid= %s ")
                _T("order by r.sortorder,t.sortname "),
                strTermID);


	FILE *fp;
	fp = _tfopen (_T("C:\\테스트1.txt"), _T("wb") );
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	_ftprintf( fp, strQuery );
	fclose(fp);

	m_pMSSQL_MGR->EXE_SELECT (strQuery,m_pDM_Relation, m_pCM) ;
	m_pCM->AllControlDisplay();
}

VOID CCSS_2_01::OnClickVcfThesarus() 
{

	CString strEditTerm;
	GetDlgItemText ( IDC_EDT_SEL_TERM , strEditTerm );

	CMSHFlexGrid* pGrid;
	pGrid	= static_cast<CESL_Grid*>(m_pCM->FindControl(_T("TERM_GRID")));
	
	INT nGridInx = SetCheckGrid ( pGrid ) ;

	CString sSelTerm;
	m_pDM_Relation->GetCellData ( nGridInx -1 , 1 , sSelTerm ) ;

	if ( !strEditTerm.IsEmpty() )
		strEditTerm = strEditTerm + _T("●") + sSelTerm ;
	else 
		strEditTerm = strEditTerm + sSelTerm ;

	SetDlgItemText ( IDC_EDT_SEL_TERM, strEditTerm );
}

HBRUSH CCSS_2_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
