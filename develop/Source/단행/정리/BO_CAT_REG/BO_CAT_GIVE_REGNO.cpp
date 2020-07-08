// BO_CAT_GIVE_REGNO.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_GIVE_REGNO.h"
#include "..\BO_CAT_REGNO_GIVE\BO_CAT_REGNO_GIVE1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_GIVE_REGNO dialog


CBO_CAT_GIVE_REGNO::CBO_CAT_GIVE_REGNO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_GIVE_REGNO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CBO_CAT_GIVE_REGNO::~CBO_CAT_GIVE_REGNO()
{
	
}

BOOL CBO_CAT_GIVE_REGNO::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}
VOID CBO_CAT_GIVE_REGNO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_GIVE_REGNO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_GIVE_REGNO, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_GIVE_REGNO)
	ON_BN_CLICKED(IDC_GIVEREGNO, OnGiveregno)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_GIVE_REGNO message handlers

BOOL CBO_CAT_GIVE_REGNO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_단행_정리_일괄부여")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_단행_정리_가원부_메인"));

	CString m_Year;

	// 040226_HSS 수정 LocalTime으로 Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());
	
	SetControlData(_T("CM_단행_정리_일괄부여"), _T("입수년도"), m_Year);
	
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


VOID CBO_CAT_GIVE_REGNO::OnGiveregno() 
{
	// TODO: Add your control notification handler code here

	INT idx;
	idx = SearchData();

	if ( idx ) return;
	
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_단행_정리_가원부_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_REG error") );
		return ;
	}

	idx = pDM->GetRowCount();
	if (idx == 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		return ;			
	}


	CString sTmpBookKey;

	for (INT i = 0 ; i < idx ; i++)
	{
		CString sRecKeyTmp = pDM->GetCellData( _T("BB_등록번호") , i);
		if ( sRecKeyTmp != _T("") ) {
			sTmpBookKey = sTmpBookKey + _T(", ") + sRecKeyTmp;
		}

	}

	if ( sTmpBookKey == _T("") ) {
		BO_CAT_REGNO_GIVE dlg(this, _T(""));
		dlg.m_strParentCMAlias = _T("CM_단행_정리_가원부_메인");
		dlg.m_strParentDMAlias = _T("DM_단행_정리_가원부_메인");
		dlg.m_nOpenMode = 1;
		dlg.DoModal();
		INT iCount = dlg.m_aBookKey.GetSize();
	}
	else {
		AfxMessageBox( _T("등록번호 부여된 자료가 있습니다.") );
		return ;
	}
	
	DeletePreGroup();

	return ;


}

INT CBO_CAT_GIVE_REGNO::SearchData()
{
	CString strQry,strpreGroup1,strpreGroup2,strYear;
	INT ids;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_단행_정리_가원부_메인"));

	// 차수 번호 와 가원부 번호를 메인에 보여 준다.
	GetControlData(_T("CM_단행_정리_일괄부여"), _T("가원부번호시작"), strpreGroup1);
	GetControlData(_T("CM_단행_정리_일괄부여"), _T("가원부번호끝"), strpreGroup2);		 

	if( (strpreGroup1 != _T(""))&&(strpreGroup2 != _T("")) )
	{
		strQry = _T(" A.ACCESSION_REC_NO >=")+strpreGroup1+_T(" AND A.ACCESSION_REC_NO <= ") + strpreGroup2;
	}

	if( (strpreGroup1 != _T(""))&&(strpreGroup2 == _T("")) )
	{
		strQry = _T(" A.ACCESSION_REC_NO =")+strpreGroup1;
	}

	if( (strpreGroup1 == _T(""))&&(strpreGroup2 != _T("")) )
	{
		strpreGroup1 = strpreGroup2;
		strQry = _T(" A.ACCESSION_REC_NO =")+strpreGroup1;
	}

	if( strQry == _T(""))
	{
		AfxMessageBox(_T("가원부 범위를 지정해 주십시요 !"));
		return -1;
	}

	strpreGroup1 = strpreGroup1 + _T(" - ") +strpreGroup2;
	SetControlData(_T("CM_단행_정리_가원부_메인"), _T("가원부번호"), strpreGroup1);

	GetControlData(_T("CM_단행_정리_일괄부여"), _T("입수년도"), strYear);
	if( strYear != _T("") )
	{
		strQry = strQry + _T(" AND A.ACCESSION_REC_MAKE_YEAR ='")+strYear+_T("'");
	}


	strQry = strQry+_T(" AND B.WORKING_STATUS = 'BOR113O' ");


	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해 주세요"));
		return -1;
	}


	if ( pDM->GetRowCount() == 0 )
	{		
		AfxMessageBox(_T("자료가 없습니다. !"));
		return -2;
	}

	CESL_ControlMgr *pCM;
	CESL_Grid *pGrid_MainGrid_All;

	pCM = FindCM(_T("CM_단행_정리_가원부_메인"));
	pGrid_MainGrid_All = ( CESL_Grid * )FindControl(_T("CM_단행_정리_가원부_메인"), _T("MAIN_GRID"));
	pGrid_MainGrid_All->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;


	//그리드 콜백 함수 연결 

	pCM->AllControlDisplay();
	
	INT max = pGrid_MainGrid_All->GetRows() - 1;
	
	for ( INT i = 0 ; i < max ; i++ )
	{
		pGrid_MainGrid_All->SetAt(i, 0, _T("V"));
	}

	return 0;
}

INT CBO_CAT_GIVE_REGNO::GridDisplayEventProc(CString &strGridAlias, INT nRow, 
INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN
    if (strGridAlias == _T("MAIN_GRID")) {
        if (strDMFieldAlias == _T("BB_자료상태_코드")) {
            if ( str == _T("BOR111N")) {
				str = _T("등록인계자료");
			}
			else if ( str == _T("BOR112N") ) {
				str = _T("등록자료");
			}
			else if ( str == _T("BOR113O") ) {
				str = _T("가원부자료");
			}
        }
    }
	return 0;

EFS_END
return -1;

}

INT CBO_CAT_GIVE_REGNO::DeletePreGroup()
{
	return 0;
}





HBRUSH CBO_CAT_GIVE_REGNO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
