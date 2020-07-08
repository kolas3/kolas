// BO_CAT_PROC_0400_08.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "BO_CAT_PROC_0400_08.h"
#include "..\..\..\공통\BO_VOLUME_FORM\BOVolumeDlg.h"

#include "..\..\..\공통\MarcAdjustApi\CatApi_BO.h"

#include "..\..\..\공통\COVERMANAGER\COVER_MAIN.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0400_08 dialog


CBO_CAT_PROC_0400_08::CBO_CAT_PROC_0400_08(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0400_08)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInitDlg = TRUE;
	AfxInitRichEdit();
	iMainRow = -1;
	iObjectRow = -1;
}

CBO_CAT_PROC_0400_08::~CBO_CAT_PROC_0400_08()
{
}

VOID CBO_CAT_PROC_0400_08::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0400_08)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0400_08, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0400_08)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnMain, OnbtnMain)
	ON_BN_CLICKED(IDC_btnObject, OnbtnObject)
	ON_BN_CLICKED(IDC_bUNION, OnbUNION)
	ON_BN_CLICKED(IDC_bBOOKINFO, OnbBOOKINFO)
	ON_BN_CLICKED(IDC_bBOOKINFO_SEL, OnbBOOKINFOSEL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0400_08 message handlers

BOOL CBO_CAT_PROC_0400_08::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

INT CBO_CAT_PROC_0400_08::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	switch(nDir)
	{
	case 0:
		SelectMakeList(m_strParentCMAlias, m_strParentGridAlias);
		idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
		if (idx < 0)
		{
			AfxMessageBox(_T("선정된 자료가 없습니다."));
			EndDialog(IDCANCEL);
			return -1;
		}
		break;
	case -1:
		idx = SelectGetPrev(m_strParentCMAlias, m_strParentGridAlias);
		if (idx == -1) {
			idx = SelectGetHeadPosition(m_strParentCMAlias, m_strParentGridAlias);
			AfxMessageBox(_T("처음 자료입니다."));
			return -1;
		}
		break;
	case 1:
		idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
		if (idx == -1) {
			idx = SelectGetTailPosition(m_strParentCMAlias, m_strParentGridAlias);
			AfxMessageBox(_T("마지막 자료입니다."));
			return -1;
		}
		break;
	}
	
	m_idx = idx; 
	AllControlDisplay(_T("CM_BO_CAT_PROC_0400"), m_idx);
	SetReverse(m_strParentCMAlias, m_strParentGridAlias, m_idx);
	return 0;

EFS_END
return -1;

}

/////////////////////////////////////////////////////////////////////////////
BOOL CBO_CAT_PROC_0400_08::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	CESL_DataMgr *pDM;
	CString sRegNo;

	if (InitESL_Mgr(_T("BO_CAT_PROC_0400")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	if (m_strParentCMAlias.IsEmpty())
		m_strParentCMAlias = _T("CM_BO_CAT_PROC_0000");
	if (m_strParentDMAlias.IsEmpty())
		m_strParentDMAlias = _T("DM_BO_CAT_PROC_0000");
	if (m_strParentGridAlias.IsEmpty())
		m_strParentGridAlias = _T("ProcGrid");
	
	INT iReturn = Display(0);

	if ( iReturn == -1 ) {
		m_bInitDlg = FALSE;
		return FALSE;
	}
	
	pDM = FindDM(m_strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	sRecKey = pDM->GetCellData( _T("BS_종KEY") , m_idx );
	
	sRegNo = pDM->GetCellData( _T("시작등록번호") , m_idx );
	if ( sRegNo == _T("") ) {
		AfxMessageBox ( _T("등록번호를 부여해야 합니다.") );
		m_bInitDlg = FALSE;
		return FALSE;
	}

	sRegNo = pDM->GetCellData( _T("BS_복본여부") , m_idx );
	if ( sRegNo == _T("V") ) {
		AfxMessageBox ( _T("복본자료는 종통합을 할 수 없습니다.") );
		m_bInitDlg = FALSE;
		CDialog::OnCancel();

		return FALSE;
	}

	if ( sRecKey == _T("") ) {
		AfxMessageBox (_T("선정된 자료가 없습니다."));
		m_bInitDlg = FALSE;
		return FALSE;
	}	

	INT iRowCount = 1;

	m_idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);
	CString sRecKeySub;
	while ( m_idx != -1 ) {		
		CString sRegNo;
		sRegNo = pDM->GetCellData( _T("시작등록번호") , m_idx );
		if ( sRegNo == _T("") ) {
			AfxMessageBox ( _T("등록번호를 부여해야 합니다.") );
			m_bInitDlg = FALSE;
			return FALSE;
		}
		sRegNo = pDM->GetCellData( _T("BS_복본여부") , m_idx );
		if ( sRegNo == _T("V") ) {
			AfxMessageBox ( _T("복본자료는 종통합을 할 수 없습니다.") );

			CDialog::OnCancel();
			m_bInitDlg = FALSE;
			return FALSE;
		}

		CString sKey;
		pDM->GetCellData( _T("BS_종KEY") , m_idx , sKey );	
		sRecKey = sRecKey + _T(" , ") + sKey;
		m_idx = SelectGetNext(m_strParentCMAlias, m_strParentGridAlias);

		iRowCount ++ ;
	} 

	if ( iRowCount < 2 ) {
		AfxMessageBox ( _T("종통합은 두자료 이상이 선택되어야 합니다.") );
		m_bInitDlg = FALSE;
		return FALSE;
	}

	CESL_DataMgr *pDM_0400;
	pDM_0400 = FindDM(_T("DM_BO_CAT_PROC_0400"));

	CString sWhere;

	sWhere.Format ( _T("I.REC_KEY = S.REC_KEY AND S.REC_KEY IN ( %s )") , sRecKey );
	
	if (pDM_0400->RefreshDataManager(sWhere) < 0)	return FALSE;
	iGridCount = pDM_0400->GetRowCount();
	if ( pDM_0400->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}

	CString strWorkingStatus;

	INT n113NCnt = 0;
	INT n114OCnt = 0;
	INT n211OCnt = 0;
	INT n111OCnt = 0;
	INT nHoldCnt = 0;
	INT nDmRowCnt = pDM_0400->GetRowCount();
	for ( int i=0 ; i < nDmRowCnt ; i++ )
	{
		strWorkingStatus = pDM_0400->GetCellData(_T("IBS_작업상태INFO"), i);
		if ( strWorkingStatus != _T("BOL111O") && strWorkingStatus.Left(3) == _T("BOL") )
		{
			++nHoldCnt;
		}
		else if ( strWorkingStatus == _T("BOC113N"))
		{
			++n113NCnt;
		}
		else if ( strWorkingStatus == _T("BOC114O"))
		{
			++n114OCnt;
		}
		else if ( strWorkingStatus == _T("BOC211O"))
		{
			++n211OCnt;
		}
		else if ( strWorkingStatus == _T("BOL111O"))
		{
			++n111OCnt;			
		}
	}

	if( nDmRowCnt != nHoldCnt && 
		nDmRowCnt != n113NCnt && 
		nDmRowCnt != n114OCnt && 
		nDmRowCnt != n211OCnt && 
		nDmRowCnt != n111OCnt 	)
	{
		AfxMessageBox(_T("소장중인 자료 또는 같은 자료 상태끼리만 종통합이 가능합니다."));
		CDialog::OnCancel();
		return TRUE;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0400"));
	pCM->AllControlDisplay();

	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_PROC_0400"));
	pCM_SUB->FindControl(_T("종통합_그리드"));
	
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("종통합_그리드"));

	pGrid_sub->SetRows( iGridCount + 1 );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0400_08::OnbtnCLOSE() 
{
EFS_BEGIN
	CDialog::OnCancel();
EFS_END
}

BEGIN_EVENTSINK_MAP(CBO_CAT_PROC_0400_08, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_CAT_PROC_0400_08)
	ON_EVENT(CBO_CAT_PROC_0400_08, IDC_MSHFLEXGRID, -600 /* Click */, OnClickMshflexgrid, VTS_NONE)
	ON_EVENT(CBO_CAT_PROC_0400_08, IDC_MSHFLEXGRID, -605 /* MouseDown */, OnMouseDownMshflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_CAT_PROC_0400_08::OnClickMshflexgrid() 
{
EFS_BEGIN

	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_PROC_0400"));
	pCM_SUB->FindControl(_T("종통합_그리드"));
	
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("종통합_그리드"));

	iRow = pGrid_sub->GetRow();

	INT nCol = pGrid_sub->GetMouseCol();
	INT nRow = pGrid_sub->GetMouseRow();
	
	if( nCol == 1 )
		CreatePopUpButton( IDC_MSHFLEXGRID );	

EFS_END
}

VOID CBO_CAT_PROC_0400_08::CreatePopUpButton( UINT f_inButtonType )
{
EFS_BEGIN

    CMenu      m_ContextMenu;
    CRect      s_rect;

    CButton * pButton = ( CButton * )GetDlgItem( f_inButtonType );
    if ( pButton == NULL ) {
        return;
    }

    m_ContextMenu.CreatePopupMenu();
	switch( f_inButtonType ) {
		case IDC_MSHFLEXGRID:
			m_ContextMenu.AppendMenu( MF_STRING , 2010 , _T("통합주자료선정")	  );
			m_ContextMenu.AppendMenu( MF_STRING , 17001 , _T("통합대상자료선정") );	

			break;
		default:
			break;
	}

    pButton->GetWindowRect( &s_rect );

	m_ContextMenu.TrackPopupMenu( TPM_LEFTALIGN, s_rect.left + Selectedx, s_rect.top + Selectedy, this );

	return;
EFS_END
}

VOID CBO_CAT_PROC_0400_08::OnMouseDownMshflexgrid(short Button, short Shift, long x, long y) 
{
EFS_BEGIN

	Selectedx = x;
	Selectedy = y;

EFS_END
}

VOID CBO_CAT_PROC_0400_08::OnbtnMain() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM_0400;
	pDM_0400 = FindDM(_T("DM_BO_CAT_PROC_0400"));

	m_marcEditor.SubclassDlgItem(IDC_redtMain, -1, this);

	if ( NULL == pDM_0400 ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return ;
	}
	pDM_0400->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));

	CString sStatusMain = pDM_0400->GetCellData( _T("IBS_작업상태INFO"), iRow-1 );
	
	if ( sStatusMain >= _T("BOL112N") && sStatusMain <= _T("BOL511O") ) 
	{
		sStatusMain = _T("소장자료");
	}
	else 
	{
		sStatusMain = _T("정리자료");
	}

	SetControlData( _T("CM_BO_CAT_PROC_0400"), _T("주자료상태"), sStatusMain );

	CString m_strStreamMarc = pDM_0400->GetCellData( _T("BS_MARC") , iRow-1 );

	sMainRecKey = pDM_0400->GetCellData( _T("BS_종KEY") , iRow-1 );

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

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0400"));
	pCM->AllControlDisplay(0);	

	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_PROC_0400"));
	pCM_SUB->FindControl(_T("종통합_그리드"));
	
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("종통합_그리드"));

	pGrid_sub->SetRows( iGridCount + 1 );

	iMainRow = iRow;
	pGrid_sub->SetTextMatrix( iMainRow , 1 , _T("주자료") );
	if ( iObjectRow != -1 && iMainRow != iObjectRow ) {
		pGrid_sub->SetTextMatrix( iObjectRow , 1 , _T("대상자료") );
	}	

	if ( iObjectRow > 0 ) {
		GetDlgItem(IDC_bUNION)->EnableWindow(true);	
	}
	GetDlgItem(IDC_bBOOKINFO_SEL)->EnableWindow(true);

	if ( iMainRow == iObjectRow ) {
		m_marc_obj.Init();
		m_marcEditor_obj.Display();
		GetDlgItem(IDC_bUNION)->EnableWindow(false);
		GetDlgItem(IDC_bBOOKINFO)->EnableWindow(false);
		SetControlData( _T("CM_BO_CAT_PROC_0400"), _T("대상자료상태"), _T("") );
	}	

EFS_END
}

VOID CBO_CAT_PROC_0400_08::OnbtnObject() 
{
EFS_BEGIN
	
	CESL_ControlMgr *pCM_SUB;
	pCM_SUB = FindCM(_T("CM_BO_CAT_PROC_0400"));
	pCM_SUB->FindControl(_T("종통합_그리드"));
	
	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM_SUB->FindControl(_T("종통합_그리드"));

	INT top, bottom;
	top = pGrid_sub->GetRow();
	bottom = pGrid_sub->GetRowSel();
	INT tmp;
	if ( top < bottom ) {
		tmp = bottom;
		bottom = top;
		top = tmp;
	}

	CArray <INT, INT> aText;
	INT iRows = pGrid_sub->GetRows();	
	for ( INT i = 1 ; i < iRows ; i++ ) {
		CString sText = pGrid_sub->GetTextMatrix( i, 1 );
		if ( sText == _T("대상자료") ) {
			aText.Add( i );
		}
	}

	CESL_DataMgr *pDM_0400;
	pDM_0400 = FindDM(_T("DM_BO_CAT_PROC_0400"));

	m_marcEditor_obj.SubclassDlgItem(IDC_redtOjbect, -1, this);

	if ( NULL == pDM_0400 ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return ;
	}
	pDM_0400->SetMarcMgr(m_pInfo->pMarcMgr);
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	
	CString sStatusMain = pDM_0400->GetCellData( _T("IBS_작업상태INFO"), iRow-1 );
	
	if ( sStatusMain >= _T("BOL112N") && sStatusMain <= _T("BOL511O") ) 
	{
		sStatusMain = _T("소장자료");
	}
	else {
		sStatusMain = _T("정리자료");
	}

	SetControlData( _T("CM_BO_CAT_PROC_0400"), _T("대상자료상태"), sStatusMain );

	CString m_strStreamMarc_obj = pDM_0400->GetCellData( _T("BS_MARC") , iRow-1 );
	
	sObjectRecKey = pDM_0400->GetCellData( _T("BS_종KEY") , iRow-1 );

	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(m_strStreamMarc_obj, &m_marc_obj))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	CString strStreamMarc = m_strStreamMarc_obj;
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

	m_marcEditor_obj.InitUserAlias();
	m_marcEditor_obj.AddUserAlias(_T("본표제"));
	m_marcEditor_obj.AddUserAlias(_T("저작자"));	
	m_marcEditor_obj.AddUserAlias(_T("발행지"));
	m_marcEditor_obj.AddUserAlias(_T("발행자"));
	m_marcEditor_obj.AddUserAlias(_T("발행년"));
	m_marcEditor_obj.AddUserAlias(_T("입력날짜"));
	m_marcEditor_obj.AddUserAlias(_T("발행년1"));
	m_marcEditor_obj.AddUserAlias(_T("유형"));

	m_marcEditor_obj.Init(m_pInfo->pMarcMgr, &m_marc_obj);
	m_marcEditor_obj.Display();

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_BO_CAT_PROC_0400"));
	pCM->AllControlDisplay(0);	

	pGrid_sub->SetRows( iGridCount + 1 );

	iObjectRow = iRow;

	if ( iMainRow != -1 && iObjectRow != iMainRow) {
		pGrid_sub->SetTextMatrix( iMainRow , 1 , _T("주자료") );
	}
	for ( i = bottom ; i <= top ; i++ ) {
		pGrid_sub->SetTextMatrix( i , 1 , _T("대상자료") );
	}
	for ( i = 0 ; i < aText.GetSize() ; i++ ) {
		pGrid_sub->SetTextMatrix( aText.GetAt(i) , 1 , _T("대상자료") );
	}

	pGrid_sub->SetReverse( iRow-1 );

	aObjIdx.RemoveAll();
	for ( i = 1 ; i < iRows ; i++ ) {
		CString sText = pGrid_sub->GetTextMatrix( i , 1 );
		if ( sText == _T("대상자료") ) {
			aObjIdx.Add( i );
		}
	}

	GetDlgItem(IDC_bUNION)->EnableWindow(true);
	GetDlgItem(IDC_bBOOKINFO)->EnableWindow(true);

	if ( iMainRow == iObjectRow ) {
		m_marc.Init();
		m_marcEditor.Display();
		GetDlgItem(IDC_bUNION)->EnableWindow(false);
		GetDlgItem(IDC_bBOOKINFO_SEL)->EnableWindow(false);
		SetControlData( _T("CM_BO_CAT_PROC_0400"), _T("주자료상태"), _T("") );
	}	

EFS_END
}

VOID CBO_CAT_PROC_0400_08::OnbUNION() 
{
EFS_BEGIN
	
	CString sVolNo;		
	CString sPriceSum;	
	CString sVolPre;    
	CString sStartRegNo, sEndRegNo;
	CString sPreShelfCode;
	CString sVolNoPre;
	CString strItemData;
	CString sRegNo;
	CString sSetIsbn;
	CString strObjRecKey, strObjectVol, strObjectVolCode, strShelfCode, strObjectVolSortNo;
	CArray<CString, CString&> array;
	CArray<CString, CString&> pArray;

	INT iPriceSum = 0;
	INT ids = -1;
	INT iVolCount = 0;
 
	CESL_DataMgr *pDmSpecies = FindDM( _T("DM_BO_CAT_PROC_0400") );
	if ( pDmSpecies == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0400 error") );
		return;
	}

	sObjectRecKey = _T("");
	for ( INT i = 0 ; i < aObjIdx.GetSize() ; i++ ) {
		CString sSpeciesKey = pDmSpecies->GetCellData( _T("BS_종KEY"), aObjIdx.GetAt(i)-1 );
		sObjectRecKey = sObjectRecKey + _T(", ") +sSpeciesKey;
	}
	if ( sObjectRecKey == _T("") ) return;
		sObjectRecKey = sObjectRecKey.Mid(2);

	CESL_DataMgr *pDmBookObject = FindDM( _T("DM_BO_CAT_PROC_0300_책") );
	if ( pDmBookObject == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_책 error") );
		return;
	}

	CString strWhere = _T("species_key in (") + sObjectRecKey + _T(")");
	if ( pDmBookObject->RefreshDataManager(strWhere) < 0 ) return;		
	INT iObjectCount = pDmBookObject->GetRowCount();

	CESL_DataMgr *pDmBookMain = FindDM( _T("DM_BO_CAT_PROC_0300_책_MAIN") );
	if ( pDmBookMain == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_책 error") );
		return;
	}
	strWhere = _T("species_key = '") + sMainRecKey + _T("'");

	if ( pDmBookMain->RefreshDataManager(strWhere) < 0 ) return;
	INT iDmBookMainCount = pDmBookMain->GetRowCount();

	INT iMaxVolSortNo = 0 ;
	for ( i = 0 ; i < iDmBookMainCount ; i++ ) {
		CString sVolSortNo = pDmBookMain->GetCellData( _T("BB_권일련번호"), i );
		INT iVolSortNo = _ttoi( sVolSortNo );
		if ( iVolSortNo > iMaxVolSortNo ) {
			iMaxVolSortNo = iVolSortNo;
		}
	}

	for ( i = 0 ; i < iObjectCount ; i++ ) 
	{
		INT iFind = 0;
		CString sVolNo = pDmBookObject->GetCellData( _T("BB_편권차"), i );
		for ( INT j = 0 ; j < iDmBookMainCount ; j++ ) {
			CString sMainVolCode = pDmBookMain->GetCellData( _T("BB_편권차"), j );
			if ( sVolNo == sMainVolCode ) {
				CString sVolSortNo = pDmBookMain->GetCellData( _T("BB_권일련번호"), j );
				pDmBookObject->SetCellData( _T("BB_권일련번호"), sVolSortNo, i );			
				iFind = 1;
				break;
			}

		}
		if ( iFind == 0 ) 
		{
			CString sMaxVolSortNo;
			for( INT k = 0 ; i > k ; k++ )
			{
				if( i == 0 )
				{
					iMaxVolSortNo += 10;
					sMaxVolSortNo.Format( _T("%d"), iMaxVolSortNo );
					pDmBookObject->SetCellData( _T("BB_권일련번호"), sMaxVolSortNo, i );
					iFind = 1;
					break;
				}
				CString sVolNoPrev = pDmBookObject->GetCellData( _T("BB_편권차"), k );
				if( sVolNoPrev == sVolNo )
				{
					pDmBookObject->GetCellData( _T("BB_권일련번호"), k, sMaxVolSortNo );
					pDmBookObject->SetCellData( _T("BB_권일련번호"), sMaxVolSortNo, i );
					iFind = 1;
					break;
				}
			}
			if( iFind == 0)
			{
				iMaxVolSortNo += 10;
				sMaxVolSortNo.Format( _T("%d"), iMaxVolSortNo );
				pDmBookObject->SetCellData( _T("BB_권일련번호"), sMaxVolSortNo, i );
			}
		}
	}

	INT nColCnt;
	INT nCol;
	CString strFIELD_ALIAS;
	CString strFIELD_NAME;
	CString strFIELD_TYPE;
	CString strINIT_VALUE;
	CString strDATA_TYPE;

	nColCnt = pDmBookMain->GetRefColumnCount();
	for (nCol = 0; nCol < nColCnt; nCol++) {
		if (pDmBookMain->GetColumnRefInfo(nCol, strFIELD_ALIAS, strFIELD_NAME, strFIELD_TYPE, strINIT_VALUE, strDATA_TYPE)!= 0) continue;
		if(strFIELD_ALIAS.Find(_T("BB_원본등록번호")) >= 0)
		{
			pDmBookMain->SetDataMgrRef(nCol, strFIELD_ALIAS, strFIELD_NAME, strFIELD_TYPE, strINIT_VALUE, _T("UDF"));
			break;
		}
	}

	strWhere = _T("SPECIES_KEY = '") + sMainRecKey + _T("' OR SPECIES_KEY IN (") + sObjectRecKey + _T(")")
		       + _T(" ORDER BY DECODE(VOL_CODE, NULL, '0', VOL_CODE) ASC, DECODE(COPY_CODE, NULL, '1', COPY_CODE) ASC, REG_NO ASC");
	if ( pDmBookMain->RefreshDataManager(strWhere) < 0 ) 
	{
		AfxMessageBox( _T("주자료 책DM구성 실패") );
		return;
	}
	
	pDmBookMain->SetDataMgrRef(nCol, strFIELD_ALIAS, strFIELD_NAME, strFIELD_TYPE, strINIT_VALUE, _T("str"));
	CString strBookRegNo;
	for ( i = 0 ; i < pDmBookMain->GetRowCount() ; i++ )
 	{
 		pDmBookMain->GetCellData(_T("BB_등록번호"), i, strBookRegNo);
 		pDmBookMain->SetCellData(_T("BB_원본등록번호"), strBookRegNo, i );
 	}

	INT iMainCount = pDmBookMain->GetRowCount();

	CCatApi_BO *Convert = new CCatApi_BO(this);
	if( Convert == NULL )
	{
		AfxMessageBox(_T("CCatApi_BO is NULL!!"));
		return;
	}
	
	ids = Convert->Convert049ToBook(&m_marc_obj, pDmBookObject);
	ids = Convert->Convert049ToBook(&m_marc, pDmBookMain, FALSE);
	
	CString strTmpVol;
	for ( i = 0; i < pDmBookObject->GetRowCount(); i++)
	{
		pDmBookObject->GetCellData(_T("BB_책KEY"), i, strObjRecKey);
		pDmBookObject->GetCellData(_T("BB_편권차"), i, strObjectVol);
		pDmBookObject->GetCellData(_T("BB_복본기호"), i, strObjectVolCode);
		pDmBookObject->GetCellData(_T("BB_별치기호"), i, strShelfCode);
		pDmBookObject->GetCellData(_T("BB_권일련번호"), i, strObjectVolSortNo);

		for (INT j = 0 ; j < pDmBookMain->GetRowCount(); j++)
		{
			CString strMainRecKey;
			pDmBookMain->GetCellData(_T("BB_책KEY"), j, strMainRecKey);
			pDmBookMain->GetCellData(_T("BB_권일련번호"), j, strTmpVol);
			if ( strObjRecKey == strMainRecKey )
			{
				pDmBookMain->SetCellData(_T("BB_편권차"), strObjectVol, j );
				pDmBookMain->SetCellData(_T("BB_권책기호"), strObjectVol, j );
				pDmBookMain->SetCellData(_T("BB_복본기호"), strObjectVolCode , j );
				pDmBookMain->SetCellData(_T("BB_별치기호"), strShelfCode, j);
				pDmBookMain->SetCellData(_T("BB_권일련번호"), strObjectVolSortNo, j);
				break;
			}
		}
	}

	CESL_DataMgr *pDmVol;
	pDmVol = FindDM( _T("DM_BO_CAT_PROC_0300_권") );
	if ( pDmVol == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_권 error") );
		return;
	}
	ids = pDmVol->FreeData();

	CBO_SPECIES API(this);
	API.CONVERT_BOOK_to_VOLUME( pDmBookMain, pDmVol );

	if ( pDmVol != NULL )
	{
		iVolCount = pDmVol->GetRowCount();
	}

	else if( iVolCount > 1 )
	{
		ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("245$n") );
		ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("245$p") );
	}

	CString strTempSetISBN;
	strTempSetISBN = _T("");
	ids = m_pInfo->pMarcMgr->GetField( &m_marc, _T("020"), sSetIsbn, &array );
	for ( i = 0 ; i < array.GetSize() ; i++ ) {
		if( array.GetAt(i).Left(1) == _T("1") ) {
			strTempSetISBN = array.GetAt(i);
		}
	}

	if ( strTempSetISBN == _T("") )
	{
		ids = m_pInfo->pMarcMgr->GetField( &m_marc_obj, _T("020"), sSetIsbn, &array );
		for ( i = 0 ; i < array.GetSize() ; i++ ) {
			if( array.GetAt(i).Left(1) == _T("1") ) {
				strTempSetISBN = array.GetAt(i);
			}
		}
	}
	ids = m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("020") );
	ids = m_pInfo->pMarcMgr->InsertField( &m_marc, _T("020") + strTempSetISBN );

	m_pInfo->pMarcMgr->DeleteField( &m_marc, _T("505") );
	CStringArray strAddIsbnList;
	strAddIsbnList.RemoveAll();
	for ( i = 0 ; i < iVolCount ; i++ ) 
	{	
		CString sVolTitle, sVol, sAuthor;
		sVolTitle = pDmVol->GetCellData( _T("BB_편제명부록명"), i );
		sVol = pDmVol->GetCellData( _T("BB_편권차"), i );
		sAuthor = pDmVol->GetCellData( _T("BB_저작자"), i );
		if ( sVol != sVolPre ) {
			if ( sVol != _T("") ) {
				m_pInfo->pMarcMgr->SetItem( &m_marc, _T("505$n"), sVol );
			}
			if ( sVolTitle != _T("") ) {
				m_pInfo->pMarcMgr->SetItem( &m_marc, _T("505$t"), sVolTitle );
			}
			if ( sAuthor != _T("") ) {
				if ( i < iVolCount -1 ) {
					sAuthor = sAuthor + _T("--");
				}
				m_pInfo->pMarcMgr->SetItem( &m_marc, _T("505$d"), sAuthor );
			}
		}
		sVolPre = sVol;
		

		CString sItemData = _T("");
		CString strISBNVol = _T("");
		
		ids = pDmVol->GetCellData( _T("BB_편권차"), i, strISBNVol );
		if ( strISBNVol != _T("") ) {
			if ( i == 0 ) {
				sVolNo = strISBNVol;
			}
			else {
				sVolNo = sVolNo + _T(",") + strISBNVol;
			}
		}

		ids = pDmVol->GetCellData( _T("BB_가격"), i, sItemData );
		CString sPrice = sItemData;
		if ( sItemData != _T("") ) {
			INT iPrice = _ttoi ( sItemData.GetBuffer(0) );
			iPriceSum = iPriceSum + iPrice;
		}

		ids = pDmVol->GetCellData( _T("BB_낱권ISBN"), i, sItemData );		

		for ( int y=0 ; y<strAddIsbnList.GetSize() ; y++ )
		{
			if ( sItemData == strAddIsbnList.GetAt(y) )
			{
				sItemData = _T("");
				break;
			}
		}
		if ( sItemData != _T("") ) 
		{
			strAddIsbnList.Add(sItemData);
			
			if ( sPrice != _T("") ) {
				CString sMoneyCode;
				CString s950b;
				m_pInfo->pMarcMgr->GetItem( &m_marc, _T("950$b"), s950b );
				for ( int i = 0 ; i < s950b.GetLength() ; i++ ) {
					if ( s950b.GetAt(i) < '0' || s950b.GetAt(i) > '9' ) {
						sMoneyCode = sMoneyCode + s950b.GetAt(i);
					}
					else {
						break;
					}
				}
				sPrice = sMoneyCode + sPrice;					
			}
			CString s020g, s020z;
			pDmVol->GetCellData( _T("BB_낱권ISBN부가기호"), i, s020g );
			pDmVol->GetCellData( _T("BB_낱권오류ISBN"), i, s020z );

			CString strTmp=_T("");
			strTmp.Format(_T("020  %ca%s"), SUBFIELD_CODE, sItemData);

			if ( !strISBNVol.IsEmpty())
			{
				strTmp += _T("(")+strISBNVol+_T(")");											
			}
			sItemData = strTmp;
			if ( s020g != _T("") ) {
				sItemData.Format(_T("%s%cg%s"), strTmp, SUBFIELD_CODE, s020g);
			}
			if ( s020z != _T("") ) {
				sItemData.Format(_T("%s%cz%s"), strTmp, SUBFIELD_CODE, s020z);
			}
			if ( sPrice != _T("") ) {
				sItemData.Format(_T("%s%cc%s"), strTmp, SUBFIELD_CODE, sPrice);
			}
			ids = m_pInfo->pMarcMgr->InsertField( &m_marc, sItemData );			
		}		
	}

	sPriceSum.Format( _T("%d"), iPriceSum );
	if ( sPriceSum != _T("") ) {
		CString sMoneyCode;
		GetControlData( _T("CM_BO_CAT_PROC_0300"), _T("BS_화폐구분"), sMoneyCode );
		CString sPreMoney;
		ids = m_pInfo->pMarcMgr->GetItem( &m_marc, _T("950$b"), sPreMoney );
		
		if( !sPreMoney.IsEmpty() )
		{
			for ( int k = 0 ; k < sPreMoney.GetLength() ; k++ ) 
			{
				CString sOneWord = sPreMoney.Mid( k, 1 );
				if ( sOneWord < _T("0") || sOneWord > _T("9") ) 
				{
					sMoneyCode = sMoneyCode + sOneWord;
				}
			}
			sPriceSum = sMoneyCode + sPriceSum;			

			ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("950$b") );
			ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("950$b"), sPriceSum );
		}
	}
	ids = m_pInfo->pMarcMgr->SetItem( &m_marc, _T("245$n"), sVolNo );

	m_pInfo->pMarcMgr->GetItem(&m_marc_obj, _T("049$l"), sRegNo);
	m_pInfo->pMarcMgr->GetItem(&m_marc_obj, _T("049$l"), strItemData, &pArray);

	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$l") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$v") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$c") );
	ids = m_pInfo->pMarcMgr->DeleteItem( &m_marc, _T("049$f") );
		
	int iCopyCode = 1;
	int iSwRegNo = 0;
	int iSwShelfCode = 0;
	int iRespresentCount = 0;

	CString str049FieldData;

	for ( i = pDmBookMain->GetRowCount()-1 ; i >=0 ; i-- ) {
		CString strTmpFlag;
		pDmBookMain->GetCellData(_T("BB_책부록플래그"), i, strTmpFlag);
		if ( strTmpFlag == _T("A") ) {
			pDmBookMain->DeleteRow(i);
		}
	}

	str049FieldData = _T("");
	for ( i = 0 ; i < pDmBookMain->GetRowCount() ; i ++ ) 
	{
		CString sRepresentYn;
		ids = pDmBookMain->GetCellData( _T("BB_대표책여부"), i, sRepresentYn );
		if ( sRepresentYn == _T("Y") ) {
			iRespresentCount++;
			if ( iRespresentCount == 2 ) {
				ids = pDmBookMain->SetCellData( _T("BB_대표책여부"), _T("N"), i );
				m_sRepresentNoBookKey = pDmBookMain->GetCellData( _T("BB_책KEY"), i );
			}
		}

		sRegNo = _T("");
		ids = pDmBookMain->GetCellData( _T("BB_등록번호"), i, sRegNo );
		if ( sRegNo != _T("") ) {
			ids = m_pInfo->pMarcMgr->DeleteItem(&m_marc, _T("049$l"), sRegNo );
			str049FieldData += "$l" + sRegNo;
		}		

		CString sItemData = _T("");
		CString strCopyCode = _T("");		
		if ( sRegNo != _T("") ) 
		{
			ids = pDmBookMain->GetCellData( _T("BB_편권차"), i, sItemData );
			if ( sItemData != _T("") ) 
			{				
				str049FieldData += "$v" + sItemData;
			}						
			
			pDmBookMain->GetCellData( _T("BB_복본기호"), i, strCopyCode );
			if ( strCopyCode != _T("") )
			{
				str049FieldData += "$c" + strCopyCode;
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
				str049FieldData += "$f" + sRegNo;
			}

			if ( (i != 0) && (sPreShelfCode != sRegNo) ) 
			{							
				iSwShelfCode = 1;
			}			
			sPreShelfCode = sRegNo;
		}		
	}


	str049FieldData.Replace('$', SUBFIELD_CODE);
	if ( iSwShelfCode == 1 ) 
	{
		m_pInfo->pMarcMgr->InsertField( &m_marc, _T("0491 ") + str049FieldData );		
	}
	else 
	{
		m_pInfo->pMarcMgr->InsertField( &m_marc, _T("0490 ") + str049FieldData );
	}
	
	m_pInfo->pMarcMgr->DeleteEmptyField( &m_marc );
	m_pInfo->pMarcMgr->DeleteEmptySubfield( &m_marc );

	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&m_marc);

	m_marcEditor.Display();

	GetDlgItem(IDC_bBOOKINFO)->EnableWindow(false);
	GetDlgItem(IDC_bUNION)->EnableWindow(false);
	GetDlgItem(IDC_bBOOKINFO_SEL)->EnableWindow(false);

	m_marc_obj.Init();
	m_marcEditor_obj.Display();

	GetDlgItem(IDOK)->EnableWindow(true);

EFS_END
}

VOID CBO_CAT_PROC_0400_08::OnOK() 
{
EFS_BEGIN


	CBO_SPECIES API(this);
	CIndexTool indexTool;

	CESL_DataMgr *pDM_PROC_0000; 
	CESL_DataMgr *pWhere;
	CESL_DataMgr dm;
	CESL_ControlMgr *pCM;

	CArray<CString, CString&> pArray;

	CString TITLE, AUTHOR, PUBLISHER;
	CString sTitle;
	CString sTitleTagX;
	CString sTitleTagB;
	CString sAuthor;
	CString sAuthorTagE;
	CString sPublisher;
	CString strItemData;
	CString sQuery;
	
	CString strQuery, strReturnValue;
	CString sWhere;
	
	INT	chk	= -1;
	INT iCh = -1;
	INT	nSpeciesCount = 1;

	m_marcEditor.ApplyEdit();
	m_pInfo->pMarcMgr->Encoding(&m_marc, m_strStreamMarc);

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0400"));
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0400") );
		return;
	}

	iCh = pDM->StartFrame();
	if ( 0 > iCh ) return;

	CESL_DataMgr *pDM_BOOK = FindDM( _T("DM_BO_CAT_PROC_0300_책_MAIN") );
	if ( pDM_BOOK == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_책 error") );
		return;
	}
	INT iBookCount = pDM_BOOK->GetRowCount();

	for ( INT i = 0 ; i < iBookCount ; i++ ) {
		CString sBookFlag = pDM_BOOK->GetCellData( _T("BB_책부록플래그"), i );
		if ( sBookFlag == _T("A") ) {
			continue;
		}
		CString sItem = _T("");
		m_pInfo->pMarcMgr->GetItem( &m_marc , _T("056$a") , sItem );
		if ( sItem == _T("") ) {
			m_pInfo->pMarcMgr->GetItem( &m_marc , _T("082$a") , sItem );
		}
		if ( sItem != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_분류기호"), sItem, i );
			CString sDbFlag = pDM_BOOK->GetCellData( _T("BB_DB_FLAG"), i );
			if ( sDbFlag == _T("") ) {			
				INT ids = pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T("U") , i );
			}
		}
		sItem = _T("");
		m_pInfo->pMarcMgr->GetItem( &m_marc , _T("090$b") , sItem );
		if ( sItem != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_도서기호"), sItem, i );
			CString sDbFlag = pDM_BOOK->GetCellData( _T("BB_DB_FLAG"), i );
			if ( sDbFlag == _T("") ) {
				INT ids = pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T("U") , i );
			}
		}
		CString sWorkStatus = pDM_BOOK->GetCellData( _T("BB_자료상태"), i );
		if ( sWorkStatus < _T("BOC111N") && sWorkStatus != _T("") ) {
			pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T(""), i );
		}
		else {
			INT ids = pDM_BOOK->SetCellData( _T("BB_DB_FLAG"), _T("U") , i );
		}
	}

	indexTool.Init(m_pInfo->pMarcMgr, &m_marc);

	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("서명"), TITLE);
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("저자"), AUTHOR);
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("발행자"), PUBLISHER);

	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("245$a"), sTitle);	
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("245$x"), sTitleTagX);
	if ( sTitleTagX != _T("") ) {
		sTitle = sTitle + _T(" = ") + sTitleTagX;
	}
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("245$b"), sTitleTagB);
	if ( sTitleTagB != _T("") ) {
		sTitle = sTitle + _T(" : ") + sTitleTagB;
	}
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("245$d"), sAuthor );
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("245$e"), sAuthorTagE );
	if ( sAuthorTagE != _T("") ) {
		sAuthor = sAuthor + _T(" ; ") + sAuthorTagE;
	}
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("260$b"), strItemData, &pArray );
	for ( i = 0 ; i < pArray.GetSize() ; i++ ) {
		if ( i == 0  ) {
			sPublisher = pArray.GetAt(i);
		}
		else {
			sPublisher = sPublisher + _T(" : ") + pArray.GetAt(i);
		}
	}

	pDM->InitDBFieldData();

	pDM->AddDBFieldData(_T("IDX_ITITLE"), _T("STRING"), TITLE);

	CString sIdxAllItem;	// AllItem
	CString strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("본표제") , strIndexItem );	
	pDM->AddDBFieldData(_T("IDX_TITLE"), _T("STRING"), strIndexItem);
	pDM->AddDBFieldData(_T("TITLE"), _T("STRING"), sTitle);
	sIdxAllItem = strIndexItem;

	strIndexItem = _T("");

	chk = indexTool.GetIndex( _T("키워드파일") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_KEYWORD"), _T("STRING"), strIndexItem);	
	if ( strIndexItem != _T("") ) {
		sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;
	}

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("저작자") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_AUTHOR"), _T("STRING"), strIndexItem);
	pDM->AddDBFieldData(_T("AUTHOR"), _T("STRING"), sAuthor);
	if ( strIndexItem != _T("") ) {
		sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;
	}

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("발행자") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_PUBLISHER"), _T("STRING"), strIndexItem);
	pDM->AddDBFieldData(_T("PUBLISHER"), _T("STRING"), sPublisher);
	if ( strIndexItem != _T("") ) {
		sIdxAllItem = sIdxAllItem + _T(" ") + strIndexItem;
	}

	CArray<CString, CString&> pArrayIdxAllItem;
	INT iPos = sIdxAllItem.Find( _T(" ") );
	if( iPos >= 0 )
	{
		while ( iPos != -1 ) {
			CString sItem = sIdxAllItem.Left( iPos );
			INT iFind = 0;
			for ( INT i = 0 ; i < pArrayIdxAllItem.GetSize() ; i++ ) {
				if ( sItem == pArrayIdxAllItem.GetAt(i) ) {
					iFind = 1;
					break;
				}
			}
			if ( iFind == 0 ) {
				pArrayIdxAllItem.Add( sItem );
			}
			sIdxAllItem = sIdxAllItem.Mid( iPos+1 );
			iPos = sIdxAllItem.Find( _T(" ") );
		}
		sIdxAllItem = _T("");
		for ( i = 0 ; i < pArrayIdxAllItem.GetSize() ; i++ ) {
			sIdxAllItem = sIdxAllItem + _T(" ") + pArrayIdxAllItem.GetAt(i);
		}	
		sIdxAllItem = sIdxAllItem.Mid(1);
	}

	pDM->AddDBFieldData(_T("IDX_ALL_ITEM"), _T("STRING"), sIdxAllItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("범위검색용발행자") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_IPUBLISHER"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("표준부호_ISBN") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_CODE"), _T("STRING"), strIndexItem);
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("표준부호_ISSN") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_ISSN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("표준부호_STRN") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_STRN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("표준부호_RNSTRN") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_RNSTRN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("표준부호_국가서지번호") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_CBN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("표준부호_저작권등록번호") , strIndexItem );
	pDM->AddDBFieldData(_T("ST_CAN"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("기타부호_청구기호_별치기호") , strIndexItem );
	pDM->AddDBFieldData(_T("SE_SHELF_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("기타부호_청구기호_분류기호") , strIndexItem );
	pDM->AddDBFieldData(_T("CLASS_NO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("기타부호_청구기호_도서기호") , strIndexItem );
	pDM->AddDBFieldData(_T("BOOK_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("기타부호_청구기호_권책기호") , strIndexItem );
	pDM->AddDBFieldData(_T("VOL_CODE_DISP"), _T("STRING"), strIndexItem);	//_T("기타부호_청구기호_권책기호_DISP")
	pDM->AddDBFieldData(_T("VOL_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("기타부호_제어번호") , strIndexItem );
	pDM->AddDBFieldData(_T("CONTROL_NO"), _T("STRING"), strIndexItem);
	
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("KDCP") , strIndexItem );
	pDM->AddDBFieldData(_T("KDCP_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("KDC") , strIndexItem );
	pDM->AddDBFieldData(_T("KDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("DDC") , strIndexItem );
	pDM->AddDBFieldData(_T("DDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("CEC") , strIndexItem );
	pDM->AddDBFieldData(_T("CEC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("CWC") , strIndexItem );
	pDM->AddDBFieldData(_T("CWC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("COC") , strIndexItem );
	pDM->AddDBFieldData(_T("COC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("UDC") , strIndexItem );
	pDM->AddDBFieldData(_T("UDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("NDC") , strIndexItem );
	pDM->AddDBFieldData(_T("NDC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("LC") , strIndexItem );
	pDM->AddDBFieldData(_T("LC_CLASS"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("한국대학부호") , strIndexItem );
	pDM->AddDBFieldData(_T("UNI_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("정부기관부호") , strIndexItem );
	pDM->AddDBFieldData(_T("GOV_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("학위논문학과") , strIndexItem );
	pDM->AddDBFieldData(_T("DEG_CODE"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("판종") , strIndexItem );
	pDM->AddDBFieldData(_T("EDIT"), _T("STRING"), strIndexItem);
	
	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("범위검색용발행년도") , strIndexItem );
	pDM->AddDBFieldData(_T("IDX_IPUB_YEAR"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	m_pInfo->pMarcMgr->GetItem( &m_marc, _T("260$c"), strIndexItem);
	pDM->AddDBFieldData(_T("PUB_YEAR"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("본문언어") , strIndexItem );
	pDM->AddDBFieldData(_T("TEXT_LANG"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("요약문언어") , strIndexItem );
	pDM->AddDBFieldData(_T("SUM_LANG"), _T("STRING"), strIndexItem);


	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("표제사항INFO") , strIndexItem );
	pDM->AddDBFieldData(_T("TITLE_INFO"), _T("STRING"), sTitle);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("저작자사항") , strIndexItem );
	pDM->AddDBFieldData(_T("AUTHOR_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("발행자사항") , strIndexItem );
	pDM->AddDBFieldData(_T("PUB_INFO"), _T("STRING"), strIndexItem);

	strIndexItem = _T("");
	chk = indexTool.GetIndex( _T("발행년도사항") , strIndexItem );
	pDM->AddDBFieldData(_T("PUB_YEAR_INFO"), _T("STRING"), strIndexItem);	

	chk = pDM->MakeUpdateFrame( _T("IDX_BO_TBL"), _T("REC_KEY"), _T("STRING"), sMainRecKey );
	if ( 0 > chk ) return;

	CCatApi_BO *Convert = new CCatApi_BO(this);
	if( Convert == NULL )
	{
		AfxMessageBox(_T("CCatApi_BO is NULL!!"));
		return;
	}
	
	CESL_DataMgr *pDM_Index = FindDM( _T("DM_단행_정리DM_색인정보") );
	CStringArray Index_classfycodes;
	INT	ids;

	Index_classfycodes.Add( _T("IBS_이용제한구분_코드"));
	Index_classfycodes.Add( _T("IBS_관리구분_코드"));
	Index_classfycodes.Add( _T("IBS_자료있는곳INFO_코드"));
	Index_classfycodes.Add( _T("IBS_등록구분_코드"));
	CStringArray Book_classfycodes;
	Book_classfycodes.Add( _T("BB_이용제한구분_코드"));
	Book_classfycodes.Add( _T("BB_관리구분_코드"));
	Book_classfycodes.Add( _T("BB_배가위치부호"));
	Book_classfycodes.Add( _T("BB_등록구분_코드"));
	CString Index_Update_sql;
	Convert->MakeIndexClassifyCodeConcatenationSQL(pDM_Index, pDM_BOOK, &Index_classfycodes, &Book_classfycodes, 4, Index_Update_sql, sMainRecKey, _T("BB"));
	ids = pDM->AddFrame(Index_Update_sql);
	if ( 0 > ids ) return ;	

	API.MakeBookQueryFrame(pDM_BOOK, sMainRecKey, _T("0"), pDM );

	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("MARC"), _T("STRING"), m_strStreamMarc, 1 );
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("종통합"), WORK_LOG), 1 );
	INT idsTmp = pDM->MakeUpdateFrame( _T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("STRING"), sMainRecKey );
	if ( 0 > idsTmp ) return;

	sQuery.Format( _T("DELETE FROM BO_SPECIES_TBL WHERE REC_KEY in (%s);"), sObjectRecKey );
	iCh = pDM->AddFrame( sQuery );
	if ( 0 > iCh ) return;
	sQuery.Format( _T("DELETE FROM IDX_BO_TBL WHERE REC_KEY in (%s);"), sObjectRecKey );
	iCh = pDM->AddFrame( sQuery );
	if ( 0 > iCh ) return;
	
	sQuery.Format(_T("DELETE FROM BO_CONTENTS_TBL WHERE TOC_COMMON_KEY IN ( SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY IN ( %s ) );"),sObjectRecKey);
	iCh = pDM->AddFrame( sQuery );

	sQuery.Format(_T("DELETE FROM BO_ABSTRACTS_TBL WHERE TOC_COMMON_KEY IN ( SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY IN ( %s ) );"),sObjectRecKey);
	iCh = pDM->AddFrame( sQuery );

	sQuery.Format(_T("DELETE FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY IN ( %s );"),sObjectRecKey);
	iCh = pDM->AddFrame( sQuery );

	sQuery.Format(_T("DELETE FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY IN ( %s );"),sObjectRecKey);
	iCh = pDM->AddFrame( sQuery );

	CStringArray aObjKey;
	CString sObjKey = sObjectRecKey;
	INT nPos = sObjKey.Find(',');
	if(  nPos > 0 )
	{
		
		aObjKey.Add(sObjKey.Left(nPos));
		sObjKey = sObjKey.Mid(nPos + 2 );
		while( nPos > 0)
		{
			nPos = sObjKey.Find(',');
			if( nPos > 0 )
			{
				aObjKey.Add(sObjKey.Left(nPos));
				sObjKey = sObjKey.Mid(nPos + 2 );
			}
			else
			{
				aObjKey.Add(sObjKey);
			}
		}
	}
	else
	{
		aObjKey.Add(sObjectRecKey);
	}
	
	for( i = 0; i< aObjKey.GetSize() ; i++)
	{
		CString sKey = aObjKey.GetAt(i);
		if( !sKey.IsEmpty() )
		{
	
		}
	}

	sQuery.Format( _T("UPDATE BO_BOOK_TBL SET SPECIES_KEY = '%s', LAST_WORK='%s' WHERE SPECIES_KEY in(%s);") , sMainRecKey, GetWorkLogMsg(_T("종통합"),WORK_LOG), sObjectRecKey );
	iCh = pDM->AddFrame( sQuery );
	if ( 0 > iCh ) return;

	sQuery.Format(_T("UPDATE LS_WORK_T01 SET SPECIES_KEY = %s WHERE SPECIES_KEY in(%s);") , sMainRecKey, sObjectRecKey );
	iCh = pDM->AddFrame( sQuery );
	
	sQuery.Format( _T("UPDATE BO_PURCHASEINFO_TBL SET SPECIES_KEY = '%s'WHERE SPECIES_KEY in(%s);") , sMainRecKey, sObjectRecKey );
	iCh = pDM->AddFrame( sQuery );
	if ( 0 > iCh ) return;

	sQuery.Format( _T("UPDATE BO_DONINFO_TBL SET SPECIES_KEY = '%s'WHERE SPECIES_KEY in(%s);") , sMainRecKey, sObjectRecKey );
	iCh = pDM->AddFrame( sQuery );
	if ( 0 > iCh ) return;

	if ( m_sRepresentNoBookKey != _T("") )
		sQuery.Format( _T("UPDATE BO_BOOK_TBL SET REPRESENT_BOOK_YN = 'Y', LAST_WORK='%s' WHERE REC_KEY in(%s);") , GetWorkLogMsg(_T("종통합"),WORK_LOG), m_sRepresentNoBookKey );

	iCh = pDM->AddFrame( sQuery );
	if ( 0 > iCh ) return;

	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{
		CString strJJSQuery;
		strJJSQuery.Format(_T("ECO$P_MAKE_BO$I(%s);"), sMainRecKey);
		pDM->AddFrame(strJJSQuery);		
		if ( 0 > iCh ) return;
	}

	dm.SetCONNECTION_INFO( pDM->CONNECTION_INFO);

	strQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'");
	dm.GetOneValueQuery(strQuery, strReturnValue);
	
	if ( strReturnValue == _T("Y") )
	{
		TCHAR ** szKeyList = NULL;
		szKeyList = (TCHAR**) malloc (sizeof(TCHAR*) * nSpeciesCount );
		for ( INT i = 0 ; i < nSpeciesCount ; i++ ) {
			szKeyList[i] = (TCHAR*) malloc (sizeof(TCHAR) * 64 );
			szKeyList[i][0] = '\0';
		}
		_tcscpy ( szKeyList[0] , sObjectRecKey.GetBuffer(0) );

		CCOVER_MAIN dlg(this);
		dlg.SetLocalPath( _T("..\\bmp\\cover_info\\") );
		INT ids = dlg.UnitySpecies( pDM, sMainRecKey.GetBuffer(0), sMainRecKey.GetBuffer(0), szKeyList , nSpeciesCount );
		
		if ( ids < 0 )
		{
			dlg.CoverFileRollBack();
			return;
		}

		
		if ( NULL != szKeyList ) {
			for ( i = 0 ; i < nSpeciesCount ; i++ ) {
				if ( NULL != szKeyList[i] ) {
					free ( szKeyList[i] );
					szKeyList[i] = NULL;
				}
			}
			free ( szKeyList );
			szKeyList = NULL;
		}
	}

	Index_Update_sql.Format(_T("UPDATE IDX_BO_TBL SET IDX_HOLD_PLACE = GET_BO_MAIN_SHELF_CODE(%s), ")
		_T("MAIN_PLACE_INFO = GET_BO_MAIN_SHELF_CODE(%s), ")
		_T("REG_CODE = GET_BO_BOOK_INFO(%s, 'REG_CODE'), ")
		_T("PLACE_INFO = GET_BO_BOOK_INFO(%s, 'SHELF_LOC_CODE'), ")
		_T("IDX_VOL_TITLE = GET_BO_BOOK_INFO(%s, 'VOL_TITLE_INDEX'), ")
		_T("IDX_VOL = GET_BO_BOOK_INFO(%s, 'VOL_INDEX'), ")
		_T("IDX_SE_SHELF_CODE = GET_BO_BOOK_INFO(%s, 'SEPARATE_SHELF_CODE'), ")
		_T("MANAGE_CODE = GET_BO_BOOK_INFO(%s, 'MANAGE_CODE'), ")
		_T("USE_LIMIT_CODE = GET_BO_BOOK_INFO(%s, 'USE_LIMIT_CODE'),")
		_T("WORKING_STATUS = TESTWORKSTATUS(%s) WHERE REC_KEY = %s;"),
		sMainRecKey, sMainRecKey, sMainRecKey, sMainRecKey, sMainRecKey, sMainRecKey, sMainRecKey, sMainRecKey, sMainRecKey, sMainRecKey, sMainRecKey);
	ids = pDM->AddFrame(Index_Update_sql);
	if ( 0 > ids ) return ;

	CString strRecKey;
	ids = pDM->MakeRecKey(strRecKey);
	if (ids < 0) return;

	iCh = pDM->SendFrame();
	if ( iCh < 0 ) {
		AfxMessageBox( _T("통합 저장 SendFrame() 확인") );
		return;
	}
	iCh = pDM->EndFrame();
	if ( 0 > iCh ) return;

	AfxMessageBox(_T("저장하였습니다."));

	pDM_PROC_0000 = FindDM(m_strParentDMAlias);
	if ( pDM_PROC_0000 == NULL ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}
	
	pWhere = FindDM( _T("DM_BO_CAT_PROC_WHERE") );
	if ( pWhere == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_WHERE ERROR") );
		return;
	}
	
	pWhere->GetCellData( _T("조건절"), 0, sWhere );

	if (pDM_PROC_0000->RefreshDataManager(sWhere) < 0)	
	{
		AfxMessageBox(_T("DM_BO_CAT_PROC_WHERE not Found!!"));
		return;
	}
	
	pCM = FindCM(m_strParentCMAlias);
	if ( pCM == NULL ) {
		AfxMessageBox ( _T("CM_BO_CAT_PROC_0000 error") );
		return;
	}
	for ( i = 0 ; i < pDM_PROC_0000->GetRowCount() ; i++ ) {
		CString sCatalogStatus = pDM_PROC_0000->GetCellData( _T("BS_정리상태"), i );
		if ( sCatalogStatus == _T("0") ) {
			pDM_PROC_0000->SetCellData( _T("BS_정리상태"), _T("정리안됨"), i );
		}
		else if ( sCatalogStatus == _T("1") ) {
			pDM_PROC_0000->SetCellData( _T("BS_정리상태"), _T("정리됨"), i );
		}
	}
	pCM->AllControlDisplay();
	
	CDialog::OnCancel();

EFS_END
}

VOID CBO_CAT_PROC_0400_08::OnbBOOKINFO() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0300_책") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_책 error") );
		return;
	}
	INT iBookSw = pDM->GetRowCount();
	if ( iBookSw == 0 ) {
		CString strWhere = _T("species_key = '") + sObjectRecKey + _T("'");
		if ( pDM->RefreshDataManager(strWhere) < 0 ) return;
		iBookSw = 1;
	}

	CBOVolumeDlg dlg(this, pDM );	
	dlg.SetOpenMode(8000);
	dlg.SetParentInfo(sObjectRecKey , _T(""));
	dlg.DoModal();

EFS_END
}

VOID CBO_CAT_PROC_0400_08::OnbBOOKINFOSEL() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_CAT_PROC_0300_책_MAIN") );
	if ( pDM == NULL ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0300_책_MAIN error") );
		return;
	}
	INT iBookSw = pDM->GetRowCount();
	if ( iBookSw == 0 ) {
		CString strWhere = _T("species_key = '") + sMainRecKey + _T("'");
		if ( pDM->RefreshDataManager(strWhere) < 0 ) return;
		iBookSw = 1;
	}
	CBOVolumeDlg dlg(this, pDM );	
	dlg.SetOpenMode(8000);
	dlg.SetParentInfo(sMainRecKey , _T(""));
	dlg.DoModal();

EFS_END
}

HBRUSH CBO_CAT_PROC_0400_08::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
