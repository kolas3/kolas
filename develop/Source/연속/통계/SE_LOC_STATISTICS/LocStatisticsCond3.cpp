// LocStatisticsCond3.cpp : implementation file
//

#include "stdafx.h"
#include "LocStatisticsCond3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsCond3 dialog


CLocStatisticsCond3::CLocStatisticsCond3(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocStatisticsCond3)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pApi = NULL;
	m_pCM = NULL;

}

CLocStatisticsCond3::~CLocStatisticsCond3()
{
}

VOID CLocStatisticsCond3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocStatisticsCond3)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocStatisticsCond3, CDialog)
	//{{AFX_MSG_MAP(CLocStatisticsCond3)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsCond3 message handlers
BOOL CLocStatisticsCond3::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CLocStatisticsCond3::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("연속_열람_통계_조건3") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_열람_통계_조건3") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_열람_통계_조건3"));
	
	if(!m_pCM)
	{
		AfxMessageBox( _T("CM,DM error : 연속_열람_통계_조건3") );
		return FALSE;
	}


	if(!m_pApi)
	{
		AfxMessageBox( _T("통계 API를 찾을 수 없습니다!") );
		return FALSE;
	}
	
	// Date 컨트롤 적용
	/// 날짜 컨트롤 UnCheck하기
	SYSTEMTIME systime;	
	CDateTimeCtrl* pTimeCtrl = NULL;
	
	const INT nCnt = 2;
	INT nIDC[nCnt] =
	{
		IDC_datCON3_SHELF_DATE1 , IDC_datCON3_SHELF_DATE2 
	};
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		pTimeCtrl = (CDateTimeCtrl*)GetDlgItem(nIDC[i]);
		pTimeCtrl->GetTime(&systime);
		DateTime_SetSystemtime(pTimeCtrl->m_hWnd , GDT_NONE , &systime );
	}
	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	// 2009.08.31 ADD BY CJY : 통합 관리자에 경우에만 관리구분을 선택가능
	if(NULL != m_pInfo && _T("S") != m_pInfo->USER_PRIVILEGE)
	{
		GetDlgItem(IDC_MANAGECODE3)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbCON3_MANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLocStatisticsCond3::SaveCondition()
{
	
	//해당 콘트롤들의 값을 읽어와 조건절로 만든후 api에 값을 설정한다
    CString strWhere;
    CString strtmp;
  
	strWhere = _T("I.REC_KEY = S.REC_KEY")
			   _T(" AND S.REC_KEY = B.SPECIES_KEY")
			   _T(" AND B.ACQ_TYPE_VOL_KEY = V.REC_KEY");

	// 2007.09.04 add by pdw
	// 부록자료 포함유무 선택	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_chkAPPENDIX);
	if( pBtn )
	{
		if( pBtn->GetCheck() )
		{
			strWhere += _T(" AND B.BOOK_TYPE IN ( 'B' , 'C' , 'A' ) ");
		}
		else
		{
			strWhere += _T(" AND B.BOOK_TYPE IN ( 'B' , 'C' ) ");
		}
	}

	//관리 구분
	CString strMANAGE_CODE;
	m_pCM->GetControlMgrData(_T("관리구분"),strMANAGE_CODE);
	
	strMANAGE_CODE.TrimLeft();
	strMANAGE_CODE.TrimRight();

    if(!strMANAGE_CODE.IsEmpty())
    {
		strtmp.Format(_T(" AND B.MANAGE_CODE = '%s'"),strMANAGE_CODE);
		strWhere += strtmp;
    }

	//배가 부서
	//20070820 ADD PJW : 배가부서별 조건
	CString strSHELF_LOC_CODE;
	m_pCM->GetControlMgrData(_T("배가부서"),strSHELF_LOC_CODE);
	
	strSHELF_LOC_CODE.TrimLeft();
	strSHELF_LOC_CODE.TrimRight();

    if(!strSHELF_LOC_CODE.IsEmpty())
    {
		strtmp.Format(_T(" AND B.SHELF_LOC_CODE = '%s'"),strSHELF_LOC_CODE);
		strWhere += strtmp;
    }



	//배가 일자
	CString strSHELF_DATE1,strSHELF_DATE2;

	CESL_Control_Element* pCE = NULL;
	pCE = (CESL_Control_Element*)m_pCM->FindControlElement(_T("배가일자1"));
	strSHELF_DATE1 = m_pApi->GetDate(this,pCE->CTRL_ID);
	if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() ) strSHELF_DATE1 = _T("");


	pCE = (CESL_Control_Element*)m_pCM->FindControlElement(_T("배가일자2"));
	strSHELF_DATE2 = m_pApi->GetDate(this,pCE->CTRL_ID);
	if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() ) strSHELF_DATE2 = _T("");

	strSHELF_DATE1.TrimLeft();
	strSHELF_DATE1.TrimRight();
	strSHELF_DATE2.TrimLeft();
	strSHELF_DATE2.TrimRight();

	if(strSHELF_DATE1.IsEmpty() && !strSHELF_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("배가일자 범위의 뒷부분에만 조건이 입력되었습니다!"));
		return -1;
	}
	
	if(!strSHELF_DATE1.IsEmpty() || !strSHELF_DATE2.IsEmpty())
	{
		//배가일자 범위
		strtmp = m_pApi->MakeRangeQuery(_T("B.SHELF_DATE"),strSHELF_DATE1,strSHELF_DATE2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}

	}
	
	// 자료 상태
	CString strStatus;
	m_pCM->GetControlMgrData(_T("자료상태"),strStatus);
	strtmp = _T("");
	
	m_pApi->m_arrayWorkingStatus.RemoveAll();

	if( !strStatus.IsEmpty() )
	{
		if( strStatus.Compare( _T("실시간열람자료(실시간배가자료+실시간대출중자료)") ) == 0 )
		{
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			strtmp.Format( _T(" AND (B.WORKING_STATUS = 'SEL112N'")
// 						   _T(" OR (B.PREV_WORKING_STATUS = 'SEL112N' AND B.WORKING_STATUS IN ('SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O')))"));
			strtmp.Format( _T(" AND (B.WORKING_STATUS = 'SEL112N'")
							// 18.09.27 JMJ KOL.RED.2018.008
						   _T(" OR (B.PREV_WORKING_STATUS = 'SEL112N' AND B.WORKING_STATUS IN ('SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O')))"));
			//=====================================================
		
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL112N"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL311O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL312O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL313O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL314O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL315O"));
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
			// 18.09.27 JMJ KOL.RED.2018.008
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL411O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL511O"));
			//=====================================================
	
		}
		else if( strStatus.Compare(_T("실시간배가자료")) == 0 )
		{
			strtmp.Format( _T(" AND B.WORKING_STATUS = 'SEL112N'"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL112N"));
		}
		else if( strStatus.Compare(_T("실시간대출중자료")) == 0 )
		{
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
// 			strtmp.Format( _T(" AND B.PREV_WORKING_STATUS = 'SEL112N' AND B.WORKING_STATUS IN ('SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O')") );
			// 18.09.27 JMJ KOL.RED.2018.008
			strtmp.Format( _T(" AND B.PREV_WORKING_STATUS = 'SEL112N' AND B.WORKING_STATUS IN ('SEL311O' , 'SEL312O' , 'SEL313O' , 'SEL314O' , 'SEL315O' , 'SEL315O', 'SEL411O', 'SEL511O', 'SEL611O')") );
			//=====================================================
			
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL311O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL312O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL313O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL314O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL315O"));
			//=====================================================
			//2008.12.01 UPDATE BY PJW : 단행 상호대차 자료상태 추가
			// 18.09.27 JMJ KOL.RED.2018.008
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL411O"));
			m_pApi->m_arrayWorkingStatus.Add(_T("SEL511O"));
			//=====================================================
	
		}

	}	
	
	if(!strtmp.IsEmpty())
	{
		strWhere += strtmp;
	}
	


	m_pApi->m_strWhere = _T("");
	m_pApi->m_strWhere = strWhere;


	return 0;
}


HBRUSH CLocStatisticsCond3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
