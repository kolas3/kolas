// BO_CAT_PROC_0800.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0800.h"

//#include _T("..\..\..\esl\marc\Marcedit\MarcEditor.h")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0800 dialog


CBO_CAT_PROC_0800::CBO_CAT_PROC_0800(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0800)
	m_nUpdateMode = -1;
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}


VOID CBO_CAT_PROC_0800::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0800)
	DDX_Radio(pDX, IDC_rUPDATE_MODE, m_nUpdateMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0800, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0800)
	ON_BN_CLICKED(IDC_bCat0800MODIFY, OnbCat0800MODIFY)
	ON_BN_CLICKED(IDC_bNotApplyNext, OnbNotApplyNext)
	ON_BN_CLICKED(IDC_rUPDATE_MODE, OnrUPDATEMODE)
	ON_BN_CLICKED(IDC_rINSERT_MODE, OnrINSERTMODE)
	ON_BN_CLICKED(IDC_rDELETE_MODE, OnrDELETEMODE)
	ON_BN_CLICKED(IDC_bApplyNext, OnbApplyNext)
	ON_BN_CLICKED(IDC_bAllApply, OnbAllApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0800 message handlers

BOOL CBO_CAT_PROC_0800::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_CAT_PROC_0800::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0800")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	//CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	m_idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	iRowCount = 1;
	m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	
	while ( m_idx != -1 ) {
		m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		iRowCount ++;
	}

	CString sRowCount;
	sRowCount.Format ( _T("%d 건") , iRowCount );

	SetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("진행상황"), sRowCount);


	m_marcEditor.SubclassDlgItem(IDC_REDIT_0800, -1, this);

	//control init	
	CButton * pNORMAL_MARC;
	pNORMAL_MARC=(CButton *)GetDlgItem(IDC_rNORMAL_MARC);
	pNORMAL_MARC->SetCheck(1);	

	CButton * pUPDATE_MODE;
	pUPDATE_MODE=(CButton *)GetDlgItem(IDC_rUPDATE_MODE);
	pUPDATE_MODE->SetCheck(1);	

	CButton * pApplyNext;
	pApplyNext=(CButton *)GetDlgItem(IDC_bApplyNext);
	pApplyNext->EnableWindow(false);

	CButton * pNotApplyNext;
	pNotApplyNext=(CButton *)GetDlgItem(IDC_bNotApplyNext);
	pNotApplyNext->EnableWindow(false);

	CButton * pAllApply;
	pAllApply=(CButton *)GetDlgItem(IDC_bAllApply);
	pAllApply->SetWindowText(_T("모두수정"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0800::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
	//CDialog::OnCancel();
}

VOID CBO_CAT_PROC_0800::OnbCat0800MODIFY() 
{
	// TODO: Add your control notification handler code here	
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return ;
	}

	INT ids;
	CString sTagNo , sJisiCode , sSubFieldCode , sOldData , sNewData;		
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("TAG") , sTagNo );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	if ( sTagNo == _T("") ) {
		AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("지시기호") , sJisiCode );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("식별기호") , sSubFieldCode );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	if ( sSubFieldCode == _T("") ) {
		AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("변경전DATA") , sOldData );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	if ( sOldData == _T("") ) {
		AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
		return;
	}

	CButton * pUPDATE_MODE;
	pUPDATE_MODE=(CButton *)GetDlgItem(IDC_rUPDATE_MODE);

	if ( pUPDATE_MODE->GetCheck() == 1 ) {
		//m_uMode = 0;
		ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("변경후DATA") , sNewData );
		if ( 0 != ids ) {
			AfxMessageBox ( _T("GetControlData Error") );
			return;
		}
		if ( sNewData == _T("") ) {
			AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
			return;
		}
	}

	CButton * INSERT_MODE;
	INSERT_MODE=(CButton *)GetDlgItem(IDC_rINSERT_MODE);	
	if ( INSERT_MODE->GetCheck() == 1 ) {
		m_nUpdateMode = 1;
	}

	CButton * DELETE_MODE;
	DELETE_MODE=(CButton *)GetDlgItem(IDC_rDELETE_MODE);	
	if ( DELETE_MODE->GetCheck() == 1 ) {
		m_nUpdateMode = 2;
	}

	Display(0);

	/*
	if ( sTagNo == _T("") || sSubFieldCode == _T("") || sOldData == _T("") || sNewData == _T("") ) {
		AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
		return;
	}
	*/
/*
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return ;
	}
*/
	/*
	ids = SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	if ( 0 != ids ) {
		AfxMessageBox ( _T("SelectMakeList Error") );
	}
	m_idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	iCurCount = 1;
	CString sMarc = pDM->GetCellData( _T("BS_MARC") , m_idx );

	//m_idx = SelectGetNext ( _T("CM_BO_CAT_PROC_0000") , _T("ProcGrid") );	
	*/
  
	//decoding
	/*CMarc m_marc;
	CString strTagCode, strErrMsg, strHelpMsg;	
	if(m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	CString sItemData;
	//ids = m_pInfo->pMarcMgr->GetField( &m_marc, _T("245") , sItemData );
	//ids = m_pInfo->pMarcMgr->SetItem(&m_marc, _T("키워드"), sItemData , _T(""), ADD_SUBFIELD);
	ids = m_pInfo->pMarcMgr->GetItem ( &m_marc , _T("260$a") , sItemData );
	//ids = m_pInfo->pMarcMgr->SetItem ( &m_marc , _T("260$a") , sItemData );
	//if ( sItemData == sOldData ) {				
		
		pDM->SetMarcMgr(m_pInfo->pMarcMgr);
		m_pInfo->pMarcMgr->SetMarcGroup(_T("단행"));

		m_marcEditor.InitUserAlias();
		m_marcEditor.AddUserAlias(_T("본표제"));
		m_marcEditor.AddUserAlias(_T("저작자"));	
		m_marcEditor.AddUserAlias(_T("발행지"));
		m_marcEditor.AddUserAlias(_T("발행자"));
		m_marcEditor.AddUserAlias(_T("발행년"));
		m_marcEditor.AddUserAlias(_T("입력날짜"));
		m_marcEditor.AddUserAlias(_T("발행년1"));
		m_marcEditor.AddUserAlias(_T("유형"));

		m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);



		//////////////////////////////////////////////////
		// MODE CHANGE


		m_marcEditor.Display();
		TCHAR szLeader[25];
		m_marc.GetLeader(szLeader);

		CString sLeader;
		sLeader.Format( _T("%s") , szLeader );
		ids = SetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("리더") , sLeader);
		if ( 0 != ids ) {
			AfxMessageBox ( _T("SetControlData Error") );
		}
		CString sStatus;
		sStatus.Format ( _T("%d / %d") , iCurCount , iRowCount );
		ids = SetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("진행상황") , sStatus);
		if ( 0 != ids ) {
			AfxMessageBox ( _T("SetControlData Error") );
		}
	//}
	if ( sItemData == sOldData ) {
		return;
	}
	/*
	while ( m_idx != -1 ) {
		m_idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		iRowCount ++;
	}
	*/

	//CString strResult;
	//strResult.Format(_T("--------- 데이터의 변경이 완료되었습니다 ---------\n\n ■ 선택종수         : %d\n ■ 건별변경수      : %d\n ■ 사용자변경종수: %d"), 1, 2, 3);		
	//AfxMessageBox ( strResult );
	//ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("지시기호") , strResult );
}

VOID CBO_CAT_PROC_0800::OnbNotApplyNext() 
{

	Display(1);

	return;
	// TODO: Add your control notification handler code here
	m_idx = SelectGetNext ( _T("CM_BO_CAT_PROC_0000") , _T("ProcGrid") );	

	if ( m_idx == -1 ) {
		return;
	}

	CString sMarc = pDM->GetCellData( _T("BS_MARC") , m_idx );
	sRecKey = pDM->GetCellData( _T("BS_종KEY") , m_idx );

	//m_idx = SelectGetNext ( _T("CM_BO_CAT_PROC_0000") , _T("ProcGrid") );	

	//decoding
	CMarc m_marc;
	CString strTagCode, strErrMsg, strHelpMsg;	
	if(m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	CString sItemData;
	//ids = m_pInfo->pMarcMgr->GetField( &m_marc, _T("245") , sItemData );
	//ids = m_pInfo->pMarcMgr->SetItem(&m_marc, _T("키워드"), sItemData , _T(""), ADD_SUBFIELD);
	INT ids = m_pInfo->pMarcMgr->GetItem ( &m_marc , _T("260$a") , sItemData );
	//ids = m_pInfo->pMarcMgr->SetItem ( &m_marc , _T("260$a") , sItemData );
	//if ( sItemData == sOldData ) {				
		m_marcEditor.SubclassDlgItem(IDC_REDIT_0800, -1, this);
		pDM->SetMarcMgr(m_pInfo->pMarcMgr);
		m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
/*
		m_marcEditor.InitUserAlias();
		m_marcEditor.AddUserAlias(_T("본표제"));
		m_marcEditor.AddUserAlias(_T("저작자"));	
		m_marcEditor.AddUserAlias(_T("발행지"));
		m_marcEditor.AddUserAlias(_T("발행자"));
		m_marcEditor.AddUserAlias(_T("발행년"));
		m_marcEditor.AddUserAlias(_T("입력날짜"));
		m_marcEditor.AddUserAlias(_T("발행년1"));
		m_marcEditor.AddUserAlias(_T("유형"));
*/
		m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
		m_marcEditor.Display();
		TCHAR szLeader[25];
		m_marc.GetLeader(szLeader);

		CString sLeader;
		sLeader.Format( _T("%s") , szLeader );
		ids = SetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("리더") , sLeader);
		if ( 0 != ids ) {
			AfxMessageBox ( _T("SetControlData Error") );
		}
		CString sStatus;
		sStatus.Format ( _T("%d / %d") , iCurCount , iRowCount );
		ids = SetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("진행상황") , sStatus);
		if ( 0 != ids ) {
			AfxMessageBox ( _T("SetControlData Error") );
		}
	//}
}

VOID CBO_CAT_PROC_0800::OnrUPDATEMODE() 
{
	// TODO: Add your control notification handler code here
	CButton * pApplyNext;
	pApplyNext=(CButton *)GetDlgItem(IDC_bApplyNext);
	pApplyNext->SetWindowText(_T("수 정"));

	CButton * pJISI_CODE;
	pJISI_CODE=(CButton *)GetDlgItem(IDC_eJISI_CODE);
	pJISI_CODE->EnableWindow(false);

	CButton * pBeforeData;
	pBeforeData=(CButton *)GetDlgItem(IDC_sBeforeData);
	pBeforeData->SetWindowText(_T("변경전DATA"));

	CButton * pNEW_DATA;
	pNEW_DATA=(CButton *)GetDlgItem(IDC_eNEW_DATA);
	pNEW_DATA->EnableWindow(true);

	CButton * pAllApply;
	pAllApply=(CButton *)GetDlgItem(IDC_bAllApply);
	pAllApply->SetWindowText(_T("모두수정"));
	
}

VOID CBO_CAT_PROC_0800::OnrINSERTMODE() 
{
	// TODO: Add your control notification handler code here
	CButton * pApplyNext;
	pApplyNext=(CButton *)GetDlgItem(IDC_bApplyNext);
	pApplyNext->SetWindowText(_T("추 가"));

	CButton * pJISI_CODE;
	pJISI_CODE=(CButton *)GetDlgItem(IDC_eJISI_CODE);
	pJISI_CODE->EnableWindow(true);

	CButton * pBeforeData;
	pBeforeData=(CButton *)GetDlgItem(IDC_sBeforeData);
	pBeforeData->SetWindowText(_T("추가할DATA"));

	CButton * pNEW_DATA;
	pNEW_DATA=(CButton *)GetDlgItem(IDC_eNEW_DATA);
	pNEW_DATA->EnableWindow(false);	

	CButton * pAllApply;
	pAllApply=(CButton *)GetDlgItem(IDC_bAllApply);
	pAllApply->SetWindowText(_T("모두추가"));
}

VOID CBO_CAT_PROC_0800::OnrDELETEMODE() 
{
	// TODO: Add your control notification handler code here
	CButton * pApplyNext;
	pApplyNext=(CButton *)GetDlgItem(IDC_bApplyNext);
	pApplyNext->SetWindowText(_T("삭 제"));

	CButton * pJISI_CODE;
	pJISI_CODE=(CButton *)GetDlgItem(IDC_eJISI_CODE);
	pJISI_CODE->EnableWindow(false);

	CButton * pBeforeData;
	pBeforeData=(CButton *)GetDlgItem(IDC_sBeforeData);
	pBeforeData->SetWindowText(_T("삭제할DATA"));

	CButton * pNEW_DATA;
	pNEW_DATA=(CButton *)GetDlgItem(IDC_eNEW_DATA);
	pNEW_DATA->EnableWindow(false);	

	CButton * pAllApply;
	pAllApply=(CButton *)GetDlgItem(IDC_bAllApply);
	pAllApply->SetWindowText(_T("모두삭제"));
}


INT CBO_CAT_PROC_0800::Display(INT nDir)
{
	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return -1;
		}
		break;
	case -1:
		idx = SelectGetPrev(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		if (idx == -1) {
			idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
			AfxMessageBox(_T("처음 자료입니다."));
			return -1;
		}
		break;
	case 1:
		idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
		if (idx == -1) {
			idx = SelectGetTailPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
			AfxMessageBox(_T("마지막 자료입니다."));
			return -1;
		}
		break;
	}

	m_idx = idx;

	DisplayModifyData();

	//AllControlDisplay(_T("CM_BO_CAT_PROC_0300"), m_idx);
	SetReverse(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"), m_idx);
	return 0;
}

VOID CBO_CAT_PROC_0800::DisplayModifyData()
{
	UpdateData();
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return ;
	}

	INT ids;
	CString sTagNo , sJisiCode , sSubFieldCode , sOldData , sNewData;

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("TAG") , sTagNo );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("지시기호") , sJisiCode );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("식별기호") , sSubFieldCode );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	if ( sSubFieldCode == _T("") ) {
		AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("변경전DATA") , sOldData );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	if ( sOldData == _T("") ) {
		AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("변경후DATA") , sNewData );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}


	pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	CString sMarc = pDM->GetCellData( _T("BS_MARC") , m_idx );
	sRecKey = pDM->GetCellData( _T("BS_종KEY") , m_idx );

	//CMarc marc;
	CString strTagCode, strErrMsg, strHelpMsg;	
	if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	m_marcEditor.Init(m_pInfo->pMarcMgr, &marc);
	m_marcEditor.Display();

	// MODE CHAGE

	// MODIFY
/*	
	if (m_nUpdateMode == 0)
	{
		ids = m_marcEditor.OutFindInit(sTagNo, _T("$") + sSubFieldCode, sOldData);
		GetDlgItem(IDC_bApplyNext)->EnableWindow(TRUE);
		GetDlgItem(IDC_bAllApply)->EnableWindow(TRUE);
		GetDlgItem(IDC_bNotApplyNext)->EnableWindow(TRUE);
		if (ids > 0)
		{
			ids = m_marcEditor.OutFindNext(sTagNo, _T("$") + sSubFieldCode, sOldData);
			if (ids >= 0)
			{
				GetDlgItem(IDC_bApplyNext)->EnableWindow(TRUE);
				GetDlgItem(IDC_bAllApply)->EnableWindow(TRUE);
				GetDlgItem(IDC_bNotApplyNext)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_bApplyNext)->EnableWindow(FALSE);
				GetDlgItem(IDC_bAllApply)->EnableWindow(FALSE);
				GetDlgItem(IDC_bNotApplyNext)->EnableWindow(FALSE);
			}
		}
	}

	// ADD
	else if (m_nUpdateMode == 1)
	{
		GetDlgItem(IDC_bApplyNext)->EnableWindow(TRUE);
		GetDlgItem(IDC_bAllApply)->EnableWindow(TRUE);
		GetDlgItem(IDC_bNotApplyNext)->EnableWindow(TRUE);		
	}
	// DELETE
	else if (m_nUpdateMode == 2)
	{
		GetDlgItem(IDC_bApplyNext)->EnableWindow(TRUE);
		GetDlgItem(IDC_bAllApply)->EnableWindow(TRUE);
		GetDlgItem(IDC_bNotApplyNext)->EnableWindow(TRUE);

		ids = m_marcEditor.OutFindInit(sTagNo, _T("$") + sSubFieldCode, sOldData);
		if (ids > 0)
		{
			ids = m_marcEditor.OutFindNext(sTagNo, _T("$") + sSubFieldCode, sOldData);
			if (ids >= 0)
			{
				GetDlgItem(IDC_bApplyNext)->EnableWindow(TRUE);
				GetDlgItem(IDC_bAllApply)->EnableWindow(TRUE);
				GetDlgItem(IDC_bNotApplyNext)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_bApplyNext)->EnableWindow(FALSE);
				GetDlgItem(IDC_bAllApply)->EnableWindow(FALSE);
				GetDlgItem(IDC_bNotApplyNext)->EnableWindow(FALSE);
			}
		}
	}
	*/
}

VOID CBO_CAT_PROC_0800::OnbApplyNext() 
{
	UpdateData();

	INT ids;
	CString sTagNo, sSubFieldCode;
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("TAG") , sTagNo );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("식별기호") , sSubFieldCode );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}

	// Modify
	//INT ids;
	
	if (m_nUpdateMode == 0)
	{
	
		CString sNewData;
		ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("변경후DATA") , sNewData );
		if ( 0 != ids ) {
			AfxMessageBox ( _T("GetControlData Error") );
			return;
		}

		//m_marcEditor.OutFindReplace(sNewData);
		m_marcEditor.ApplyEdit();
		//m_marcEditor.Get
		//encoding
		
	}
	

	if (m_nUpdateMode == 2)	{
		if ( sSubFieldCode == _T("") ) {
			m_pInfo->pMarcMgr->DeleteField( &marc, sTagNo );
		}
		else {
			m_pInfo->pMarcMgr->DeleteItem( &marc, sTagNo + _T("$") + sSubFieldCode );
		}
		CString sItem;
		m_pInfo->pMarcMgr->GetItem(&marc, _T("260$a") , sItem);
	}

	CString strStreamMarc;// = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
	//UPDATE
	sRecKey = pDM->GetCellData( _T("BS_종KEY") , m_idx );
	INT iCh;	
	iCh = pDM->StartFrame();
	CString sQuery;
	sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC = '%s', LAST_WORK='%s' WHERE REC_KEY = '%s';") , strStreamMarc,GetWorkLogMsg(_T("데이터일괄변경"),WORK_LOG), sRecKey );
	iCh = pDM->AddFrame( sQuery );
	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) {
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return;
	}
	iCh = pDM->EndFrame();	
}

VOID CBO_CAT_PROC_0800::OnbAllApply() 
{
	UpdateData();


	pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	CString sTagNo , sJisiCode , sSubFieldCode , sOldData , sNewData;

	INT ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("TAG") , sTagNo );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("지시기호") , sJisiCode );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	
	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("식별기호") , sSubFieldCode );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	if ( sSubFieldCode == _T("") ) {
		AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("변경전DATA") , sOldData );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}
	if ( sOldData == _T("") ) {
		AfxMessageBox ( _T("모든 항목을 입력하십시오!!") );
		return;
	}

	ids = GetControlData ( _T("CM_BO_CAT_PROC_0800") , _T("변경후DATA") , sNewData );
	if ( 0 != ids ) {
		AfxMessageBox ( _T("GetControlData Error") );
		return;
	}

	SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	INT idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	
	while (idx >= 0)
	{
		CString sMarc = pDM->GetCellData( _T("BS_MARC") , idx );
		sRecKey = pDM->GetCellData( _T("BS_종KEY") , m_idx );

		CMarc marc;
		CString strTagCode, strErrMsg, strHelpMsg;	
		if(m_pInfo->pMarcMgr->Decoding(sMarc, &marc))
		{
			POSITION pos;
			pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
			while (pos)
			{
				m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
				UpdateData(FALSE);
			}	
		}

		m_marcEditor.Init(m_pInfo->pMarcMgr, &marc);
		m_marcEditor.Display();

		// Modify
		/*
		INT ids;
		if (m_nUpdateMode == 0)
		{
			ids = m_marcEditor.OutFindInit(sTagNo, _T("$") + sSubFieldCode, sOldData);
			if (ids > 0)
			{
				ids = m_marcEditor.OutFindNext(sTagNo, _T("$") + sSubFieldCode, sOldData);
				while (ids >= 0)
				{
					m_marcEditor.OutFindReplace(sNewData);
					ids = m_marcEditor.OutFindNext(sTagNo, _T("$") + sSubFieldCode, sOldData);
				}
				
			}
		}
		*/
		
		idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	}	
}
