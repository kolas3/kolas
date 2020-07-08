// LocStatisticsCond1.cpp : implementation file
//

#include "stdafx.h"
#include "LocStatisticsCond1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsCond1 dialog


CLocStatisticsCond1::CLocStatisticsCond1(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocStatisticsCond1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pCM = NULL;
	m_pApi = NULL;
}

CLocStatisticsCond1::~CLocStatisticsCond1()
{
}

VOID CLocStatisticsCond1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocStatisticsCond1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocStatisticsCond1, CDialog)
	//{{AFX_MSG_MAP(CLocStatisticsCond1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocStatisticsCond1 message handlers
BOOL CLocStatisticsCond1::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CLocStatisticsCond1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	if(InitESL_Mgr( _T("연속_열람_통계_조건1") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_열람_통계_조건1") );
		return FALSE;
	}
	
	m_pCM = FindCM(_T("CM_연속_열람_통계_조건1"));
	
	if(!m_pCM)
	{
		AfxMessageBox( _T("CM,DM error : 연속_열람_통계_조건1") );
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
		IDC_datREG_DATE3 , IDC_datREG_DATE2 
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
		GetDlgItem(IDC_MANAGECODE1)->ShowWindow(SW_HIDE);
 		GetDlgItem(IDC_cmbMANAGE_CODE)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CLocStatisticsCond1::SaveCondition()
{
   //해당 콘트롤들의 값을 읽어와 조건절로 만든후 api에 값을 설정한다

    CString strWhere;
    CString strtmp;
    BOOL bIsWonbu = FALSE;
	
	

	//원부번호 유무 파악
	CString strWonbuNum1,strWonbuNum2;
	m_pCM->GetControlMgrData(_T("원부번호1"),strWonbuNum1);
	m_pCM->GetControlMgrData(_T("원부번호2"),strWonbuNum2);

	if(strWonbuNum1.IsEmpty() && !strWonbuNum2.IsEmpty())
	{
		AfxMessageBox(_T("원부번호 범위의 뒷부분에만 조건이 입력되었습니다!"));
		return -1;
	}


	if(!strWonbuNum1.IsEmpty() || !strWonbuNum2.IsEmpty())
	{
		bIsWonbu = TRUE;
	}


	if(bIsWonbu) //원부번호 조건 있을 경우 
	{
		strWhere = _T("I.REC_KEY = S.REC_KEY")
				   _T(" AND S.REC_KEY = B.SPECIES_KEY")
				   _T(" AND B.ACQ_TYPE_VOL_KEY = V.REC_KEY")
			       _T(" AND W.REC_KEY = B.ACCESSION_REC_KEY")
				   _T(" AND W.PUBLISH_FORM_CODE = 'SE'");

		//원부 번호 범위
		strtmp = m_pApi->MakeIntRangeQuery(_T("W.ACCESSION_REC_NO"),strWonbuNum1,strWonbuNum2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}
		
	}
	else //원부번호 조건이 없을 경우
	{
		strWhere = _T("I.REC_KEY = S.REC_KEY")
			       _T(" AND S.REC_KEY = B.SPECIES_KEY")
				   _T(" AND B.ACQ_TYPE_VOL_KEY = V.REC_KEY");
	}
	
	

    //수입 구분 
    CString strACQ_CODE;
    m_pCM->GetControlMgrData(_T("수입구분"),strACQ_CODE);

    strACQ_CODE.TrimLeft();
	strACQ_CODE.TrimRight();
	
    if(!strACQ_CODE.IsEmpty())
    {
		strtmp.Format(_T(" AND B.ACQ_CODE = '%s'"),strACQ_CODE);
		strWhere += strtmp;
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
	
	//수입 년도
	CString strACQ_YEAR;
	m_pCM->GetControlMgrData(_T("수입년도"),strACQ_YEAR);
	
	strACQ_YEAR.TrimLeft();
	strACQ_YEAR.TrimRight();

    if(!strACQ_YEAR.IsEmpty())
    {
		strtmp.Format(_T(" AND B.ACQ_YEAR = '%s'"),strACQ_YEAR);
		strWhere += strtmp;
    }



	//등록 번호 
	CString strRegCode,strRegNo1,strRegNo2;

	m_pCM->GetControlMgrData(_T("등록번호구분"),strRegCode);
	m_pCM->GetControlMgrData(_T("등록번호1"),strRegNo1);
	m_pCM->GetControlMgrData(_T("등록번호2"),strRegNo2);

	strRegCode.TrimLeft();
	strRegCode.TrimRight();
	strRegNo1.TrimLeft();
	strRegNo1.TrimRight();
	strRegNo2.TrimLeft();
	strRegNo2.TrimRight();
	
	if(strRegNo1.IsEmpty() && !strRegNo2.IsEmpty())
	{
		AfxMessageBox(_T("등록번호 범위의 뒷부분에만 조건이 입력되었습니다!"));
		return -1;
	}

	if(!strRegNo1.IsEmpty() && strRegCode.IsEmpty())
	{
		AfxMessageBox(_T("등록구분이 입력되지 않았습니다!"));
		return -1;
	}
	
	if(!strRegNo1.IsEmpty() || !strRegNo2.IsEmpty())
	{
		//등록 번호 범위
		strRegNo1 = strRegCode + strRegNo1;
		strRegNo2 = strRegCode + strRegNo2;
		strtmp = m_pApi->MakeRangeQuery(_T("B.REG_NO"),strRegNo1,strRegNo2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}

	}

	//등록 일자
	CString strREG_DATE1,strREG_DATE2;

	CESL_Control_Element* pCE = NULL;
	pCE = (CESL_Control_Element*)m_pCM->FindControlElement(_T("등록일자1"));
	strREG_DATE1 = m_pApi->GetDate(this,pCE->CTRL_ID);
	if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() ) strREG_DATE1 = _T("");


	pCE = (CESL_Control_Element*)m_pCM->FindControlElement(_T("등록일자2"));
	strREG_DATE2 = m_pApi->GetDate(this,pCE->CTRL_ID);
	if( !GetDlgItem(pCE->CTRL_ID)->IsWindowEnabled() ) strREG_DATE2 = _T("");

	strREG_DATE1.TrimLeft();
	strREG_DATE1.TrimRight();
	strREG_DATE2.TrimLeft();
	strREG_DATE2.TrimRight();

	if(strREG_DATE1.IsEmpty() && !strREG_DATE2.IsEmpty())
	{
		AfxMessageBox(_T("등록 일자 범위의 뒷부분에만 조건이 입력되었습니다!"));
		return -1;
	}
	
	if(!strREG_DATE1.IsEmpty() || !strREG_DATE2.IsEmpty())
	{
		//등록 일자 범위
		strtmp = m_pApi->MakeRangeQuery(_T("B.REG_DATE"),strREG_DATE1,strREG_DATE2);

		if(!strtmp.IsEmpty())
		{
			strWhere += _T(" AND ");
			strWhere += strtmp;
		}

	}
	
	//작업 중(SEC111N,SEC112O,SEL211O) 조건 
	strWhere += _T(" AND B.WORKING_STATUS IN ('SEC111N','SEC112O','SEL211O')");


	m_pApi->m_strWhere = _T("");
	m_pApi->m_bWonbuCondition = bIsWonbu;
	m_pApi->m_strWhere = strWhere;

	return 0;
}
HBRUSH CLocStatisticsCond1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
