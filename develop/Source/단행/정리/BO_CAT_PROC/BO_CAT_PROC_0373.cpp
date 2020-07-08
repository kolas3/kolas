// BO_CAT_PROC_0373.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0373.h"
#include "..\..\..\공통\BO_VOLUME_FORM\BOVolumeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0373 dialog


CBO_CAT_PROC_0373::CBO_CAT_PROC_0373(CESL_Mgr* pParent, INT tmp, CString m_PK_arg /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_idx = tmp;
	m_PK = m_PK_arg;
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0373)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
}

CBO_CAT_PROC_0373::~CBO_CAT_PROC_0373()
{
}

VOID CBO_CAT_PROC_0373::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0373)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0373, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0373)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bCat0373NEXT, OnbCat0373NEXT)
	ON_BN_CLICKED(IDC_bCat0373PREV, OnbCat0373PREV)
	ON_BN_CLICKED(IDC_bAPPEND, OnbAPPEND)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_bBOOKINFO, OnbBOOKINFO)
	ON_BN_CLICKED(IDC_bBOOKINFO_SEL, OnbBOOKINFOSEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0373 message handlers

BOOL CBO_CAT_PROC_0373::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0373::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (InitESL_Mgr(_T("BO_CAT_PROC_0373")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_marcEditor1.SubclassDlgItem(IDC_RICHEDIT1_BCP, -1, this);	
	CESL_DataMgr *pDM1;
	pDM1 = FindDM(_T("DM_BO_CAT_PROC_0000"));

	CString sMarc = pDM1->GetCellData( _T("BS_MARC") , m_idx );
	sRecKeyOri = pDM1->GetCellData( _T("BS_종KEY") , m_idx );
	CString sDupFlag = pDM1->GetCellData( _T("BS_복본여부") , m_idx );

	CString sFormCode = pDM1->GetCellData( _T("IBS_형식구분_코드") , m_idx );
	CString sMediaCode = pDM1->GetCellData( _T("IBS_매체구분_코드") , m_idx );

	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_형식구분_코드"), sFormCode );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_매체구분_코드"), sMediaCode );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_복본여부"), sDupFlag );

	CString sWorkStatus = pDM1->GetCellData( _T("IBS_작업상태INFO"), m_idx );
	sWorkStatus = pDM1->GetCellData( _T("BS_자료상태_코드"), 0 );
			
	pDM1->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	CString strTagCode1, strErrMsg1, strHelpMsg1;
	
	if(m_pInfo->pMarcMgr->Decoding(sMarc, &m_marc1))
	{
		POSITION pos1;
		pos1 = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos1)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode1, strErrMsg1, strHelpMsg1, pos1);
			UpdateData(FALSE);
		}	
	}

	CString strStreamMarc1 = m_strStreamMarc1;
	m_pInfo->pMarcMgr->Encoding(&m_marc1, strStreamMarc1);

	m_marcEditor1.InitUserAlias();
	m_marcEditor1.AddUserAlias(_T("본표제"));
	m_marcEditor1.AddUserAlias(_T("발행지"));
	m_marcEditor1.AddUserAlias(_T("발행자"));
	m_marcEditor1.AddUserAlias(_T("발행년"));
	m_marcEditor1.AddUserAlias(_T("입력날자"));
	m_marcEditor1.AddUserAlias(_T("발행년1"));
	m_marcEditor1.AddUserAlias(_T("유형"));

	m_marcEditor1.Init(m_pInfo->pMarcMgr, &m_marc1);
	m_marcEditor1.Display();	

	pDM = FindDM(_T("DM_BO_CAT_PROC_0373"));
	if ( pDM == NULL ) {
		AfxMessageBox(_T("DM_BO_CAT_PROC_0373 error"));
	}
	
	CString strWhere;
	
	strWhere = _T("I.REC_KEY = S.REC_KEY AND I.REC_KEY IN(") + m_PK + _T(") AND bo_get_min_reg_no(S.rec_key) IS NOT NULL");

	if (pDM->RefreshDataManager(strWhere) < 0)	return FALSE;
	
	m_marcEditor.SubclassDlgItem(IDC_RICHEDIT2_BCP, -1, this);	

	pDM->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	INT iRowCount = pDM->GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox( _T("검색된 결과가 없습니다.") );
		EndDialog(IDCANCEL);
		return FALSE;
	}
	iIdx = 1;

	iAllCount = iRowCount;

	CString sRowCount;
	sRowCount.Format( _T("%d") , iRowCount );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("총건수"), sRowCount);

	CString sIdx;
	sIdx.Format( _T("%d"), iIdx );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("번째"), sIdx);

	CString sFormCodeOri = pDM->GetCellData( _T("BS_형식구분_코드") , 0 );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_형식구분_코드_UN"), sFormCodeOri);

	CString sMediaCodeOri = pDM->GetCellData( _T("BS_매체구분_코드") , 0 );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_매체구분_코드_UN"), sMediaCodeOri);

	INT nRowIdx = 0;

	pDM->GetCellData( _T("BS_MARC") , 0 , m_strStreamMarc);
	pDM->GetCellData( _T("BS_종KEY") , 0 , sRecKey );

	sWorkStatus = pDM->GetCellData( _T("IBS_작업상태INFO"), 0 );
	if ( sWorkStatus >= _T("BOA111N") && sWorkStatus <= _T("BOA142N") ) {
		sWorkStatus = _T("수서자료");
	}
	else if ( sWorkStatus >= _T("BOC111N") && sWorkStatus <= _T("BOC115N") ){
		sWorkStatus = _T("정리자료");
	}
	else {
		sWorkStatus = _T("소장자료");
	}
	SetControlData( _T("CM_BO_CAT_PROC_0373"), _T("자료상태"), sWorkStatus );

	iIdx = 1;

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

	CString strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

	m_marcEditor1.Init(m_pInfo->pMarcMgr, &m_marc1);
	m_marcEditor1.Display();

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE; 

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0373::OnbCat0373NEXT() 
{
EFS_BEGIN

	iIdx ++;
	if ( iIdx > iAllCount ) {
		AfxMessageBox ( _T("마지막 자료입니다.") );		
		iIdx--;
		return;
	}
	CString sIdx;
	sIdx.Format( _T("%d"), iIdx );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("번째"), sIdx);

	CString sFormCodeOri = pDM->GetCellData( _T("BS_형식구분_코드") , iIdx-1 );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_형식구분_코드_UN"), sFormCodeOri);

	CString sMediaCodeOri = pDM->GetCellData( _T("BS_매체구분_코드") , iIdx-1 );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_매체구분_코드_UN"), sMediaCodeOri);

	CString sWorkStatus = pDM->GetCellData( _T("IBS_작업상태INFO"), iIdx-1 );	
	if ( sWorkStatus >= _T("BOA111N") && sWorkStatus <= _T("BOA142N") ) {
		sWorkStatus = _T("수서자료");
	}
	else if ( sWorkStatus >= _T("BOC111N") && sWorkStatus <= _T("BOC115N") ){
		sWorkStatus = _T("정리자료");
	}
	else if ( sWorkStatus >= _T("BOL112N") ) {
		sWorkStatus = _T("소장자료");
	}
	SetControlData( _T("CM_BO_CAT_PROC_0373"), _T("자료상태"), sWorkStatus );

	pDM->GetCellData( _T("BS_MARC") , iIdx-1, m_strStreamMarc );
	pDM->GetCellData( _T("BS_종KEY") , iIdx-1 , sRecKey );

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

	CString strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();	

EFS_END
}

VOID CBO_CAT_PROC_0373::OnbCat0373PREV() 
{
EFS_BEGIN

	iIdx --;
	if ( iIdx < 1 ) {
		AfxMessageBox ( _T("처음 자료입니다.") );		
		iIdx ++;
		return;
	}
	CString sIdx;
	sIdx.Format( _T("%d"), iIdx );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("번째"), sIdx);

	CString sFormCodeOri = pDM->GetCellData( _T("BS_형식구분_코드") , iIdx-1 );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_형식구분_코드_UN"), sFormCodeOri);

	CString sMediaCodeOri = pDM->GetCellData( _T("BS_매체구분_코드") , iIdx-1 );
	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_매체구분_코드_UN"), sMediaCodeOri);

	CString sWorkStatus = pDM->GetCellData( _T("IBS_작업상태INFO"), iIdx-1 );	
	if ( sWorkStatus >= _T("BOA111N") && sWorkStatus <= _T("BOA142N") ) {
		sWorkStatus = _T("수서자료");
	}
	else if ( sWorkStatus >= _T("BOC111N") && sWorkStatus <= _T("BOC115N") ){
		sWorkStatus = _T("정리자료");
	}
	else {
		sWorkStatus = _T("소장자료");
	}
	SetControlData( _T("CM_BO_CAT_PROC_0373"), _T("자료상태"), sWorkStatus );

	pDM->GetCellData( _T("BS_MARC") , iIdx-1, m_strStreamMarc );
	pDM->GetCellData( _T("BS_종KEY") , iIdx-1 , sRecKey );

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

	CString strStreamMarc = m_strStreamMarc;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();
	
	

EFS_END
}

VOID CBO_CAT_PROC_0373::OnbAPPEND() 
{
EFS_BEGIN
	GetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_복본여부"), sChk);
	if ( sChk == _T("V") ) {
		AfxMessageBox(_T("이미 복본추기가 된 자료입니다."));
		return;
	}
	CString sWorkStatus;
	GetControlData( _T("CM_BO_CAT_PROC_0373"), _T("자료상태"), sWorkStatus );
	if ( sWorkStatus != _T("소장자료") ) {
		AfxMessageBox( _T("추기되는 자료는 소장자료만 가능합니다.") );
		return;
	}
	CESL_DataMgr *pDmBookMain = FindDM( _T("DM_BO_CAT_PROC_0373_책") );
	if ( pDmBookMain == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0373_책 error") );
		return;
	}

	CString strWhere = _T("species_key = '") + sRecKey + _T("' or species_key ='") + sRecKeyOri + _T("'");
	if ( pDmBookMain->RefreshDataManager(strWhere) < 0 ) return;
	INT iMainCount = pDmBookMain->GetRowCount();

	INT ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$l") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$v") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$c") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$f") );
	
	CString sVolNoPre;
	INT iCopyCode = 1;
	CString sStartRegNo, sEndRegNo;
	INT iSwRegNo = 0;
	CString sPreShelfCode;
	INT iSwShelfCode = 0;
	for ( INT i = 0 ; i < pDmBookMain->GetRowCount() ; i ++ ) {
		CString sFlag = pDmBookMain->GetCellData( _T("BB_DB_FLAG"), i );
		if ( sFlag == _T("D") ) {
		}
		CString sRegNo = _T("");
		ids = pDmBookMain->GetCellData( _T("BB_등록번호"), i, sRegNo );		
		if ( sRegNo != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem(&m_marc, _T("049$l"), sRegNo );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$l"), sRegNo );
		}
		CString sItemData = _T("");
		ids = pDmBookMain->GetCellData( _T("BB_편권차"), i, sItemData );
		if ( sItemData != _T("") && sRegNo != _T("") ) {	//등록번호가 있을때
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$v"), sItemData );
			if ( sVolNoPre == sItemData ) {
				iCopyCode++;
				CString sCopyCode;
				sCopyCode.Format( _T("%d"), iCopyCode );
				
				ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$c"), sCopyCode );
				ids = pDmBookMain->SetCellData( _T("BB_복본기호"), sCopyCode, i );
			}
			else{
				iCopyCode = 1;
			}
		}
		sVolNoPre = sItemData;

		if ( sRegNo != _T("") ) {
			iSwRegNo++;
			if ( 1 == iSwRegNo ) {
				sStartRegNo = sRegNo;
				sEndRegNo = sRegNo;
			}
			else {
				if ( sStartRegNo > sRegNo ) {
					sStartRegNo = sRegNo;
				}
				if ( sRegNo > sEndRegNo ) {
					sEndRegNo = sRegNo;
				}
			}
		}		

		if ( sRegNo != _T("") ) {
			sRegNo = _T("");
			ids = pDmBookMain->GetCellData( _T("BB_별치기호_코드"), i, sRegNo );
			if ( sRegNo != _T("") ) {
				ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$f"), sRegNo );
				ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("049$f"), sRegNo );
			}
			if ( (i != 0) && (sPreShelfCode != sRegNo) && sPreShelfCode != _T("") ) {
				CString s049field;
				m_pInfo->pMarcMgr->GetField( &m_marc, _T("049"), s049field );
				s049field = _T("1") + s049field.Mid(1);
				m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
				m_pInfo->pMarcMgr->InsertField( &m_marc, _T("049") + s049field );
				iSwShelfCode = 1;
			}
			sPreShelfCode = sRegNo;
		}
	}

	if ( iSwShelfCode == 0 ) {
		CString s049field;
		m_pInfo->pMarcMgr->GetField( &m_marc, _T("049"), s049field );
		if ( s049field != _T("") ) {
			s049field = _T("0") + s049field.Mid(1);
		}
		m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("049") );
		m_pInfo->pMarcMgr->InsertField( &m_marc, _T("049") + s049field );
	}

	m_pInfo->pMarcMgr->SortMarc( &m_marc );
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_복본여부"), _T("V"));

	GetDlgItem(IDC_bCat0373PREV)->EnableWindow(false);
	GetDlgItem(IDC_bCat0373NEXT)->EnableWindow(false);

EFS_END
}

VOID CBO_CAT_PROC_0373::OnOK() 
{
EFS_BEGIN

	GetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_복본여부"), sChk);

	INT ids = pDM->StartFrame();	
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}

	CString sQuery;

	sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET MARC='%s', DUP_FLAG = 'V', LAST_WORK='%s' WHERE rec_key = '%s';") , m_strStreamMarc,GetWorkLogMsg(_T("복본조사"),WORK_LOG), sRecKey );
	INT ch = pDM->AddFrame( sQuery );
	sQuery.Format( _T("UPDATE BO_SPECIES_TBL SET WORKING_STATUS = 'BOC114N', LAST_WORK='%s' WHERE rec_key = '%s';") , GetWorkLogMsg(_T("복본조사"),WORK_LOG),sRecKeyOri );
	ch = pDM->AddFrame( sQuery );
	sQuery.Format( _T("UPDATE BO_BOOK_TBL SET SPECIES_KEY = '%s', DUP_SPECIES_KEY = '%s', LAST_WORK='%s' WHERE SPECIES_KEY ='%s';"), sRecKey, sRecKeyOri, GetWorkLogMsg(_T("복본조사"),WORK_LOG),sRecKeyOri );	
	ch = pDM->AddFrame( sQuery );
	ch = pDM->SendFrame();
	if ( ch < 0 ) {
		AfxMessageBox( _T("자료를 확인하세요.") );
		return;
	}
	else {
		AfxMessageBox( _T("복본추기 완료") );
	}
	ch = pDM->EndFrame();

	CESL_DataMgr *pDM1;
	pDM1 = FindDM(_T("DM_BO_CAT_PROC_0000"));
	CString sDupFlag = pDM1->GetCellData( _T("BS_복본여부") , m_idx );
	pDM1->SetCellData(_T("BS_복본여부"), sChk, m_idx);

	CDialog::OnOK();

EFS_END
}

VOID CBO_CAT_PROC_0373::OnbCANCEL() 
{
EFS_BEGIN

	GetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_복본여부"), sChk);
	if ( sChk == _T("") ) {
		AfxMessageBox(_T("복본추기가 안 된 자료입니다."));
		return;
	}
	CString strItemData;
	CArray<CString, CString&> pArray;
	
	m_pInfo->pMarcMgr->GetItem(&m_marc1, _T("049$l"), strItemData, &pArray);
	INT iCnt = pArray.GetSize();
	for ( INT i = 1 ; i <= iCnt ; i ++ ){
		CString sReg = pArray.GetAt(i-1);
		m_pInfo->pMarcMgr->DeleteItem(&m_marc, _T("049$l"), sReg);
	}

	m_pInfo->pMarcMgr->GetItem(&m_marc1, _T("049$f"), strItemData, &pArray);
	iCnt = pArray.GetSize();
	for ( i = 1 ; i <= iCnt ; i ++ ){
		CString sReg = pArray.GetAt(i-1);
		m_pInfo->pMarcMgr->DeleteItem(&m_marc, _T("049$f"), sReg);
	}

	m_pInfo->pMarcMgr->SortMarc( &m_marc );	

	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_marc);
	m_marcEditor.Display();

	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

	SetControlData(_T("CM_BO_CAT_PROC_0373"), _T("BS_복본여부"), _T(""));

	GetDlgItem(IDC_bCat0373PREV)->EnableWindow(true);
	GetDlgItem(IDC_bCat0373NEXT)->EnableWindow(true);

EFS_END
}

VOID CBO_CAT_PROC_0373::OnbBOOKINFO() 
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0300_책") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_책 error") );
		return;
	}

	CBOVolumeDlg dlg(this, pDM );	
	dlg.SetOpenMode(8000);
	dlg.SetParentInfo(sRecKeyOri , _T(""));
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_0373::OnbBOOKINFOSEL() 
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0300_책_DUP") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_책_DUP error") );
		return;
	}

	CBOVolumeDlg dlg(this, pDM );	
	dlg.SetOpenMode(8000);
	dlg.SetParentInfo(sRecKey, _T(""));
	dlg.DoModal();

EFS_END
}

HBRUSH CBO_CAT_PROC_0373::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
