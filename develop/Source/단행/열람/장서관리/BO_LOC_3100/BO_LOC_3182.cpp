#include "stdafx.h"
#include "BO_LOC_3180.h"
#include "BO_LOC_3182.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3182::~CBO_LOC_3182()
{
	
}
CBO_LOC_3182::CBO_LOC_3182(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{	
	m_nViewMode = 0;
	m_nLastIndex = 0;
	m_nFirstIndex = 0;
	m_nCurrentIndex = 0;
}


VOID CBO_LOC_3182::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3182)
	DDX_Control(pDX, IDC_cSTATUS, m_cComboBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3182, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3182)
	ON_BN_CLICKED(IDC_btn3182Prev, OnbPrev)
	ON_BN_CLICKED(DC_btn3182Next, OnbNext)
	ON_BN_CLICKED(IDC_bSave, OnbSave)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

INT CBO_LOC_3182::DoModal() 
{	
	return CDialog::DoModal();
}

BOOL CBO_LOC_3182::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	if(InitESL_Mgr(_T("SM_BO_LOC_3182")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}
	
	SetCurrentIndex();
	DisplayWindow();
	
	return TRUE;  	              
}

VOID CBO_LOC_3182::OnOK() 
{	
	CDialog::OnOK();
}

VOID CBO_LOC_3182::OnbPrev() 
{
	EFS_BEGIN

	MoveIndex(0);	
	DisplayWindow();

	EFS_END
}

VOID CBO_LOC_3182::OnbNext() 
{
	EFS_BEGIN

	MoveIndex(1);	
	DisplayWindow();

	EFS_END
}

VOID CBO_LOC_3182::OnbSave() 
{
	EFS_BEGIN

	INT ids=0;

	ids = SaveData();
	if(ids){
		AfxMessageBox(_T("ERROR CBO_LOC_3182::OnbSave() := SaveData"));
		return;
	}
	
	AfxMessageBox(_T("ÀúÀåµÇ¾ú½À´Ï´Ù."));

	EFS_END	
}

INT CBO_LOC_3182::SetCurrentIndex()
{
	EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3180") );
	if(pCM==NULL){
		AfxMessageBox(_T("CBO_LOC_3182::SetCurrentIndex() := ERROR FindCM"));
		return -1;
	}
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("MainGrid")));
	if(pGrid==NULL){
		AfxMessageBox(_T("CBO_LOC_3182::SetCurrentIndex() := FindControl"));
		return -1;
	}

	INT nTmpIndex;
	nTmpIndex = pGrid->GetIdx();

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();

	if( nSelectCount > 0 ) m_nViewMode = 1;
	else m_nViewMode = 0;

	if( m_nViewMode == 0 )
	{
		m_nCurrentIndex = pGrid->GetIdx();
		m_nLastIndex = pGrid->GetCount() - 1;
		m_nFirstIndex = 0;
	}
	else
	{
		m_nLastIndex = pGrid->SelectGetTailPosition();
		m_nFirstIndex = pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCount-1 ; i++ )
		{
			if( nTmpIndex == pGrid->SelectGetIdx() )
				break;
			pGrid->SelectGetNext();
		}
		if( i==nSelectCount ) m_nCurrentIndex = pGrid->SelectGetHeadPosition();
		else m_nCurrentIndex = pGrid->SelectGetIdx();
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3182::MoveIndex(INT nMode)
{
	EFS_BEGIN

	INT ids;
	ids = SetCurrentIndex();
	if( ids < 0 ) {
		AfxMessageBox(_T("ERROR CBO_LOC_3182::MoveIndex(INT nMode) := SetCurrentIndex()"));
		return -1;
	}

	if( m_nViewMode == 0 )
	{
		if( nMode == 0 && ( m_nCurrentIndex != m_nFirstIndex )) m_nCurrentIndex--;
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) ) m_nCurrentIndex++;
	}
	else
	{
		CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3180"));
		if( pCM == NULL ){
			AfxMessageBox(_T("ERROR FindCM"));
			return -2;
		}

		CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("MainGrid") ));
		if( pGrid == NULL ){
			AfxMessageBox(_T("ERROR FindControl"));
			return -3;
		}

		if( nMode == 0 )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( nMode == 1 && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3182::DisplayWindow()
{
	EFS_BEGIN

	INT ids=0;
	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3182"));
	if( pCM == NULL ){
		AfxMessageBox(_T("ERROR FindCM"));
		return -1;
	}

	ids = pCM->AllControlDisplay(m_nCurrentIndex);
	if( ids < 0 ){
		AfxMessageBox(_T("ERROR CBO_LOC_3182::DisplayWindow() := AllControlDisplay"));
		return -1;
	}

	CString sStatus;
	ids = this->GetDataMgrData(_T("DM_BO_LOC_3180"), _T("BOOK_STATUS_DESC"), sStatus, m_nCurrentIndex);
	INT nIndex=0;
	nIndex = GetComboIndex(sStatus);
	m_cComboBox.SetCurSel(nIndex);

	SetPreNextButton();

	ControlEnableDisable();

	ids = SetSelect();
	if(ids<0){
		AfxMessageBox(_T("ERROR CBO_LOC_3182::DisplayWindow() := SetSelect"));
		return -3;
	}

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3182::ControlEnableDisable()
{
	EFS_BEGIN

	GetDlgItem(IDC_eLOAN_DATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_eRETURN_PLAN_DATE)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_eRETURN_DATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_eLOAN_STOP_DATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_eARREAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_eREPARATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_cSTATUS)->EnableWindow(FALSE);

	CString sType;
	this->GetDataMgrData(_T("DM_BO_LOC_3180"), _T("WORKTYPE"), sType, m_nCurrentIndex);

	if(sType.Compare(_T("´ëÃâ"))==0 || sType.Compare(_T("¹«ÀÎ´ëÃâ"))==0){
		GetDlgItem(IDC_eLOAN_DATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_eRETURN_PLAN_DATE)->EnableWindow(TRUE);
	}else if(sType.Compare(_T("¹Ý³³"))==0 || sType.Compare(_T("¹«ÀÎ¹Ý³³"))==0){
		GetDlgItem(IDC_eRETURN_DATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_eLOAN_STOP_DATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_eARREAR)->EnableWindow(TRUE);
		GetDlgItem(IDC_eREPARATION)->EnableWindow(TRUE);
		GetDlgItem(IDC_cSTATUS)->EnableWindow(TRUE);		
	}

	return 0;
	EFS_END
	return -1;
}

VOID CBO_LOC_3182::SetPreNextButton()
{
	EFS_BEGIN

	GetDlgItem(IDC_btn3182Prev)->EnableWindow(TRUE);
	GetDlgItem(DC_btn3182Next)->EnableWindow(TRUE);

	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btn3182Prev)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(DC_btn3182Next)->EnableWindow(FALSE);
	}

	EFS_END
}

INT CBO_LOC_3182::SetSelect()
{
	EFS_BEGIN

	INT ids;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3180") );
	if( pCM == NULL ) {
		AfxMessageBox(_T("ERROR CBO_LOC_3182::SetSelect := FindCM()"));
		return -1;
	}

	CESL_Grid *pGrid = ((CESL_Grid*)(pCM->FindControl( _T("MainGrid") )));
	if( pGrid == NULL ) {
		AfxMessageBox(_T("ERROR CBO_LOC_3182::SetSelect := FindControl()"));
		return -2;
	}

	ids = pGrid->SetReverse( m_nCurrentIndex );
	if( ids < 0 ) {
		AfxMessageBox(_T("ERRORCBO_LOC_3182::SetSelect := SetReverse()"));
		return -3;
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3182::SaveData()
{
	EFS_BEGIN

	INT ids=0;

	CString sRECKEY;
	CString sUSERNO;
	CString sREGNO;
	
	CString sLOAN_DATE;
	CString sRETURN_PLAN_DATE;

	CString sRETRUN_DATE;
	CString sLOAN_STOP_DATE;
	CString sARREAR;
	CString sREPARATION;
	CString sSTATUS;
	CString sSTATUS_DESC;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3182") );
	if(pCM==NULL){
		AfxMessageBox(_T("CBO_LOC_3182::OnbSave() := ERROR FindCM"));
		return -1;
	}

	CESL_ControlMgr *pCM2 = FindCM(_T("CM_BO_LOC_3180"));
	if( pCM2 == NULL ){
		AfxMessageBox(_T("ERROR FindCM"));
		return -3;
	}

	CESL_Grid *pGrid = (CESL_Grid*)(pCM2->FindControl( _T("MainGrid") ));
	if( pGrid == NULL ){
		AfxMessageBox(_T("ERROR FindControl"));
		return -4;
	}

	CString sSelectCheck;
	sSelectCheck = pGrid->GetTextMatrix(m_nCurrentIndex+1, 1);
	
	pCM->GetControlMgrData(_T("USER_NO"), sUSERNO, m_nCurrentIndex, -1);
	pCM->GetControlMgrData(_T("REG_NO"), sREGNO, m_nCurrentIndex, -1);
	pCM->GetControlMgrData(_T("LOAN_DATE"), sLOAN_DATE, m_nCurrentIndex, -1);
	pCM->GetControlMgrData(_T("RETURN_PLAN_DATE"), sRETURN_PLAN_DATE, m_nCurrentIndex, -1);
	pCM->GetControlMgrData(_T("RETURN_DATE"), sRETRUN_DATE, m_nCurrentIndex, -1);
	pCM->GetControlMgrData(_T("LOAN_STOP_DATE"), sLOAN_STOP_DATE, m_nCurrentIndex, -1);
	pCM->GetControlMgrData(_T("ARREAR"), sARREAR, m_nCurrentIndex, -1);
	pCM->GetControlMgrData(_T("REPARATION"), sREPARATION, m_nCurrentIndex, -1);
	pCM->GetControlMgrData(_T("BOOK_STATUS"), sSTATUS, m_nCurrentIndex, -1);

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3180") );
	if( pDM == NULL ){
		AfxMessageBox(_T("ERROR FindDM"));
		return -2;
	}

	pDM->GetCellData(_T("REC_KEY"), m_nCurrentIndex, sRECKEY);

	pDM->SetCellData(_T("USER_NO"), sUSERNO, m_nCurrentIndex);
	pDM->SetCellData(_T("REG_NO"), sREGNO, m_nCurrentIndex);
	pDM->SetCellData(_T("LOAN_DATE"), sLOAN_DATE, m_nCurrentIndex);
	pDM->SetCellData(_T("RETURN_PLAN_DATE"), sRETURN_PLAN_DATE, m_nCurrentIndex);
	pDM->SetCellData(_T("RETURN_DATE"), sRETRUN_DATE, m_nCurrentIndex);
	pDM->SetCellData(_T("LOAN_STOP_DATE"), sLOAN_STOP_DATE, m_nCurrentIndex);
	pDM->SetCellData(_T("ARREAR"), sARREAR, m_nCurrentIndex);
	pDM->SetCellData(_T("REPARATION"), sREPARATION, m_nCurrentIndex);
	INT nIndex = m_cComboBox.GetCurSel();
	GetComboText(nIndex, sSTATUS_DESC);
	BookStatusDescToCode(sSTATUS_DESC, sSTATUS);
	pDM->SetCellData(_T("BOOK_STATUS"), sSTATUS, m_nCurrentIndex);
	pDM->SetCellData(_T("BOOK_STATUS_DESC"), sSTATUS_DESC, m_nCurrentIndex);
	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	CString tmpstr;
	pDM->AddDBFieldData(_T("USER_NO"), _T("STRING"), sUSERNO, TRUE);
	pDM->AddDBFieldData(_T("REG_NO"), _T("STRING"), sREGNO, TRUE);
	if(sLOAN_DATE.IsEmpty()){
		tmpstr.Empty();
		pDM->AddDBFieldData(_T("LOAN_DATE"), _T("NUMERIC"), tmpstr, TRUE);
	}else{
		tmpstr.Empty();
		tmpstr.Format(_T("to_date('%s', 'YYYY/MM/DD:HH24-MI-SS')"), sLOAN_DATE);
		pDM->AddDBFieldData(_T("LOAN_DATE"), _T("NUMERIC"), tmpstr, TRUE);
		pDM->AddDBFieldData(_T("PROC_DATE"), _T("NUMERIC"), tmpstr, TRUE);
	}
	pDM->AddDBFieldData(_T("RETURN_PLAN_DATE"), _T("STRING"), sRETURN_PLAN_DATE, TRUE);

	if(sRETRUN_DATE.IsEmpty()){
		tmpstr.Empty();
		pDM->AddDBFieldData(_T("RETURN_DATE"), _T("NUMERIC"), tmpstr, TRUE);
	}else{
		tmpstr.Empty();
		tmpstr.Format(_T("to_date('%s', 'YYYY/MM/DD:HH24-MI-SS')"), sRETRUN_DATE);
		pDM->AddDBFieldData(_T("RETURN_DATE"), _T("NUMERIC"), tmpstr, TRUE);
		pDM->AddDBFieldData(_T("PROC_DATE"), _T("NUMERIC"), tmpstr, TRUE);
	}

	pDM->AddDBFieldData(_T("LOAN_STOP_DATE"), _T("STRING"), sLOAN_STOP_DATE, TRUE);
	pDM->AddDBFieldData(_T("ARREAR"), _T("STRING"), sARREAR, TRUE);
	pDM->AddDBFieldData(_T("REPARATION"), _T("STRING"), sREPARATION, TRUE);
	pDM->AddDBFieldData(_T("BOOK_STATUS"), _T("STRING"), sSTATUS, TRUE);
	pDM->AddDBFieldData(_T("BOOK_STATUS_DESC"), _T("STRING"), sSTATUS_DESC, TRUE);

	ids = pDM->MakeUpdateFrame(_T("CO_OFFLINE_LOAN_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRECKEY);
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3182::SaveData() := MakeUpdateFrame()") );

	ids = pDM->SendFrame();
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3182::SaveData() := SendFrame()") );
	pDM->EndFrame();
	
	pGrid->DisplayLine(m_nCurrentIndex);
	if( ids < 0 ){
		AfxMessageBox(_T("ERROR DisplayLine(-1)"));
		return -5;
	}

	if(sSelectCheck.Compare(_T("V"))==0) pGrid->SetTextMatrix(m_nCurrentIndex+1, 1, _T("V"));

	return 0;
	EFS_END
	return -1;
}

INT CBO_LOC_3182::GetComboIndex(CString sStatus)
{
	EFS_BEGIN

	if(sStatus.Compare(_T("Á¤»ó¹Ý³³"))==0){
		return 1;
	}else if(sStatus.Compare(_T("¿¬Ã¼¹Ý³³"))==0){
		return 2;
	}else if(sStatus.Compare(_T("ÆÄ¼Õ¹Ý³³"))==0){
		return 3;
	}else if(sStatus.Compare(_T("ºÐ½Ç¹Ý³³"))==0){
		return 4;
	}else if(sStatus.Compare(_T("¿¬Ã¼ÆÄ¼Õ¹Ý³³"))==0){
		return 5;
	}else if(sStatus.Compare(_T("¿¬Ã¼ºÐ½Ç¹Ý³³"))==0){
		return 6;
	}else{
		return 0;
	}

	EFS_END
	return 0;
}

INT CBO_LOC_3182::GetComboText(INT nIndex, CString &sStatusDesc)
{
	EFS_BEGIN

	sStatusDesc.Empty();

	if(nIndex==1){
		sStatusDesc = _T("Á¤»ó¹Ý³³");
	}else if(nIndex==2){
		sStatusDesc = _T("¿¬Ã¼¹Ý³³");
	}else if(nIndex==3){
		sStatusDesc = _T("ÆÄ¼Õ¹Ý³³");
	}else if(nIndex==4){
		sStatusDesc = _T("ºÐ½Ç¹Ý³³");
	}else if(nIndex==5){
		sStatusDesc = _T("¿¬Ã¼ÆÄ¼Õ¹Ý³³");
	}else if(nIndex==6){
		sStatusDesc = _T("¿¬Ã¼ºÐ½Ç¹Ý³³");
	}

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3182::BookStatusDescToCode(CString sStatusDesc, CString &sStatus)
{
	EFS_BEGIN

	sStatus.Empty();

	if(sStatusDesc.Compare(_T("Á¤»ó¹Ý³³"))==0){
		sStatus = _T("1");
	}else if(sStatusDesc.Compare(_T("¿¬Ã¼¹Ý³³"))==0){
		sStatus = _T("4");
	}else if(sStatusDesc.Compare(_T("ÆÄ¼Õ¹Ý³³"))==0){
		sStatus = _T("5");
	}else if(sStatusDesc.Compare(_T("ºÐ½Ç¹Ý³³"))==0){
		sStatus = _T("6");
	}else if(sStatusDesc.Compare(_T("¿¬Ã¼ÆÄ¼Õ¹Ý³³"))==0){
		sStatus = _T("7");
	}else if(sStatusDesc.Compare(_T("¿¬Ã¼ºÐ½Ç¹Ý³³"))==0){
		sStatus = _T("8");
	}else{
		sStatus = _T("");
	}

	EFS_END
	return 0;
}

HBRUSH CBO_LOC_3182::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
