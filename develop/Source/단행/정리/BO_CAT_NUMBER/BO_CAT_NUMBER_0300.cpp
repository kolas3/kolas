// BO_CAT_NUMBER_0300.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_NUMBER_0000.h"
#include "BO_CAT_NUMBER_0300.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_NUMBER_0300 dialog


CBO_CAT_NUMBER_0300::CBO_CAT_NUMBER_0300(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_NUMBER_0300)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_NUMBER_0300::~CBO_CAT_NUMBER_0300()
{
}

VOID CBO_CAT_NUMBER_0300::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_NUMBER_0300)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_NUMBER_0300, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_NUMBER_0300)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_NUMBER_0300 message handlers

BOOL CBO_CAT_NUMBER_0300::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_NUMBER_0300::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_NUMBER_0300")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	Display(0);

	CString sWork;
	if ( m_sWork == _T("누락수입순번호관리") ) 
		sWork = m_sWork;
	else if ( m_sWork != _T("") )
		sWork = _T("마지막수입순번호");
	else
		sWork = ((CBO_CAT_NUMBER_0000*)pParentMgr)->m_sWork;
	
	if ( sWork == _T("마지막수입순번호") ) {
		pDM = FindDM( _T("DM_BO_CAT_ACQUISIT_NO") );
	}
	else {
		pDM = FindDM( _T("DM_BO_CAT_OMIT_NO") );
	}

	if ( pDM == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_ACQUISIT_NO ERROR") );
		return FALSE;
	}

	CString sClassCode = pDM->GetCellData( _T("분류기호구분") , m_idx );
	CString sClassNo = pDM->GetCellData( _T("분류기호"), m_idx );
	CString sShelfCode = pDM->GetCellData( _T("별치기호"), m_idx );
	CString sAcqNo = pDM->GetCellData( _T("수입순번호"), m_idx );
	SetControlData( _T("CM_BO_CAT_NUMBER_0300"), _T("분류기호구분"), sClassCode );
	SetControlData( _T("CM_BO_CAT_NUMBER_0300"), _T("분류기호"), sClassNo );
	SetControlData( _T("CM_BO_CAT_NUMBER_0300"), _T("별치기호"), sShelfCode );
	SetControlData( _T("CM_BO_CAT_NUMBER_0300"), _T("수입순번호"), sAcqNo );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_NUMBER_0300::Display(INT nDir)
{
EFS_BEGIN

	CString sCmAlias, sGridAlias;
	CString sWork;
	//((CBO_CAT_NUMBER_0000*)pParentMgr)->m_sWork = _T("마지막수입순번호");
	//GetControlData( sCmAlias , _T("작업구분"), sWork );
	if ( m_sWork == _T("누락수입순번호관리") ) 
		sWork = m_sWork;
	else if ( m_sWork != _T("") )
		sWork = _T("마지막수입순번호");
	else
		sWork = ((CBO_CAT_NUMBER_0000*)pParentMgr)->m_sWork;

	if ( sWork == _T("마지막수입순번호") ) {
		sCmAlias = _T("CM_BO_CAT_ACQUISIT_NO");
		sGridAlias = _T("GRD_ACQUISIT_NO");
	}
	else {
		sCmAlias = _T("CM_BO_CAT_ACQUISIT_NO");
		sGridAlias = _T("GRD_ACQUISIT_NO2");
	}
	
	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList( sCmAlias, sGridAlias );
		idx = SelectGetHeadPosition( sCmAlias, sGridAlias );
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return;
		}
		break;
	case -1:
		idx = SelectGetPrev( sCmAlias, sGridAlias );
		if (idx == -1) {
			idx = SelectGetHeadPosition( sCmAlias, sGridAlias );
			AfxMessageBox(_T("처음 자료입니다."));
			return;
		}
		break;
	case 1:
		idx = SelectGetNext( sCmAlias, sGridAlias );
		if (idx == -1) {
			idx = SelectGetTailPosition( sCmAlias, sGridAlias );
			AfxMessageBox(_T("마지막 자료입니다."));
			return;
		}
		break;
	}
	
	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_ACQUISIT_NO_0200"), m_idx);
	SetReverse( sCmAlias, sGridAlias, m_idx);

EFS_END
}

VOID CBO_CAT_NUMBER_0300::OnOK() 
{
EFS_BEGIN

	// TODO: Add extra validation here
	//홍수 임시로 수정 . 소정 추가수정 2004.09.15++++
	//CString sWork = ((CBO_CAT_NUMBER_0000*)pParentMgr)->m_sWork;

	CString sWork;
	if ( m_sWork == _T("누락수입순번호관리") ) 
		sWork = m_sWork;
	else if ( m_sWork != _T("") )
		sWork = _T("마지막수입순번호");
	else
		sWork = ((CBO_CAT_NUMBER_0000*)pParentMgr)->m_sWork;

	//홍수 임시로 수정  ----
	
	INT ids;
	CESL_DataMgr *pDM;
	if ( sWork == _T("마지막수입순번호") ) {
		pDM = FindDM(_T("DM_BO_CAT_ACQUISIT_NO"));
	}
	else {
		pDM = FindDM(_T("DM_BO_CAT_OMIT_NO"));
	}
    //CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_ACQUISIT_NO"));	
	if (!pDM) return;
		
	///수정 작업 시작 
	CString strID;
	//pCESLMgr->GetControlData(_T("CM_BO_CAT_REG_0520"), _T("등록작업자"), strID);
	strID = pDM->GetCellData ( _T("REC_KEY") ,m_idx );

	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}


	CString sClassCode, sClassNo, sShelfCode, sAcqNo; 
	
	GetControlData( _T("CM_BO_CAT_NUMBER_0300"), _T("분류기호구분"), sClassCode );
	GetControlData( _T("CM_BO_CAT_NUMBER_0300"), _T("분류기호"), sClassNo );
	GetControlData( _T("CM_BO_CAT_NUMBER_0300"), _T("별치기호"), sShelfCode );
	GetControlData( _T("CM_BO_CAT_NUMBER_0300"), _T("수입순번호"), sAcqNo );


	if ( m_sWork == _T("누락수입순번호관리") )
	{
		//=====================================================
		//2008.11.20 ADD BY PJW : 누락수입순번호를 부여하면 중복조사를 한다.
		CString strQuery;
		CString strCnt;
		if( sShelfCode.IsEmpty() )
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE IS NULL AND BOOK_CODE = '%s'")
						, sClassCode, sClassNo, sAcqNo );
		}
		else
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE = '%s' AND BOOK_CODE = '%s'")
						, sClassCode, sClassNo, sShelfCode, sAcqNo );
		}
		pDM->GetOneValueQuery(strQuery,strCnt);
		if( _ttoi( strCnt ) > 0 )
		{
			AfxMessageBox(_T("부여된 수입순번호가 존재합니다."));
			return;
		}

		if( sShelfCode.IsEmpty() )
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM CO_OMIT_BOOK_ACQUISITNO_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE IS NULL AND ACQUISIT_NO = %s")
						, sClassCode, sClassNo, sAcqNo );
		}
		else
		{
			strQuery.Format(_T("SELECT COUNT(1) FROM CO_OMIT_BOOK_ACQUISITNO_TBL WHERE  MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_NO_TYPE = '%s' AND CLASS_NO = '%s' AND SEPARATE_SHELF_CODE = '%s' AND ACQUISIT_NO = %s")
						, sClassCode, sClassNo, sShelfCode, sAcqNo );
		}
		pDM->GetOneValueQuery(strQuery,strCnt);
		if( _ttoi( strCnt ) > 0 )
		{
			AfxMessageBox(_T("누락수입순번호가 존재합니다."));
			return;
		}
		//=====================================================
	}
	
	pDM->SetCellData( _T("분류기호구분") , sClassCode, m_idx );
	pDM->SetCellData( _T("분류기호") , sClassNo, m_idx );
	pDM->SetCellData( _T("별치기호") , sShelfCode, m_idx );
	pDM->SetCellData( _T("수입순번호") , sAcqNo, m_idx );
	pDM->SetCellData( _T("마지막작업") , GetWorkLogMsg(_T("수입순번호수정"),WORK_LOG) , m_idx );

	
	///수정 작업 시작  
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_NUMBER_0300"));
	ids = pDM->MakeControlMgrFiledData((CObject*)pCM);
	ids = pDM->MakeControlMgrUpdateFrame(_T("REC_KEY"), _T("STRING"), strID, m_idx);

	

	//ids = pDM->MakeControlMgrUpdateFrame(_T("REGISTRANT"), _T("STRING"), strID, idx);
	//ids = pDM->MakeControlMgrUpdateFrame(_T("등록작업자"), _T("STRING"), strID);
   	///수정 작업 완료 


	ids = pDM->SendFrame();
	if (ids < 0) {
		AfxMessageBox(_T("수정사항을 DataBase에 적용하던중 에러가 발생하였습니다."));
		return;
	}
  
	/*
	CBO_CAT_REG_0520 * pREG_0520;
	INT tmp = pREG_0520->m
	*/
	ids = pDM->EndFrame();	
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return;
	}

	AfxMessageBox(_T("저장완료"));
	//pCESLMgr->AllControlDisplay(_T("DM_BO_CAT_REG_0500"));
	CESL_ControlMgr *pCM1 = FindCM(_T("CM_BO_CAT_ACQUISIT_NO"));
	if ( sWork == _T("마지막수입순번호") ) {
		AllControlDisplay(_T("DM_BO_CAT_ACQUISIT_NO"));
	}
	else {
		AllControlDisplay(_T("DM_BO_CAT_OMIT_NO"));
	}
	pCM1->AllControlDisplay(m_idx);

	CDialog::OnOK();
	//DestroyWindow();

EFS_END
}

VOID CBO_CAT_NUMBER_0300::OnCancel() 
{
EFS_BEGIN

	// TODO: Add extra cleanup here
	//DestroyWindow();
	CDialog::OnCancel();

EFS_END
}

HBRUSH CBO_CAT_NUMBER_0300::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
