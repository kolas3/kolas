#include "stdafx.h"
#include "BO_LOC_3193.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3193::CBO_LOC_3193(CESL_Mgr* pParent)
	: CESL_Mgr(IDD, pParent)
{	
	MailMgr = NULL;	
}

CBO_LOC_3193::~CBO_LOC_3193()
{	
	if( MailMgr != NULL ) delete MailMgr;
	MailMgr = NULL;
}

VOID CBO_LOC_3193::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3193)		
	DDX_Control(pDX, IDC_prgMAILSTATE, m_ctrlProgress);
	//}}AFX_DATA_MAP

	EFS_END
}

BEGIN_MESSAGE_MAP(CBO_LOC_3193, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3193)
	ON_BN_CLICKED(IDC_btnAUTO_JOB_CLOSE, OnbtnAUTOJOBCLOSE)
	ON_NOTIFY(NM_CLICK, IDC_tab3100_TITLE, OnClicktabTITLE)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBO_LOC_3193::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	INT ids;
	
	if(InitESL_Mgr(_T("BO_LOC_3193")) < 0){
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	MailMgr = new CLocMailMgrAPI(this);

	ids = MailMgr->CreateMailInfo();
	if( ids < 0 ) 
	{
		if( MailMgr )
			delete MailMgr;
		ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	}

	ids = MailMgr->IsUseBatchSendMail();
	if( ids < 0 ) 
	{
		if( MailMgr )
			delete MailMgr;
		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );
	}
	if( ids > 0 ) 
	{
		OnCancel();
	}

	InitControl();

	SetTimer( 0 , 100 , NULL );
	
	return TRUE;

EFS_END
return FALSE;

}

VOID CBO_LOC_3193::OnbtnAUTOJOBCLOSE() 
{
	EFS_BEGIN

		OnCancel();	

	EFS_END
}

INT CBO_LOC_3193::RunProc()
{
	EFS_BEGIN

	RunThread(this);
	
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3193::SendMail()
{
	EFS_BEGIN

	INT ids;

	INT nMailClassCnt = 10;

	INT nDMRCnt , nSuccessDMRCnt , nFailDMRCnt ;

	m_ctrlProgress.SetRange( 0 , 70 );

	for( INT i = 0 ; i < nMailClassCnt ; i++ )
	{		
		
		if ( i == 8 || i == 1) 
			continue;
		
		ids = MailMgr->SendBatchMail(i);

		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

		if( ids == 1 )
			continue;

		if( ids == 2 )
		{
			CString strTmp  = MailMgr->GetMailName(i) ;
			ids = RecordProcessingLog( MailMgr->GetMailName(i) , _T("메일을 보낼 자료가 존재하지 않습니다.") );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("SendMail") );
			continue;
		}

		ids = MailMgr->GetLogDMCnt( i , nDMRCnt , nSuccessDMRCnt , nFailDMRCnt);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SendMail") );

		ids = RecordLog( i , 10*i , MailMgr->GetMailName(i) , nDMRCnt , nSuccessDMRCnt , nFailDMRCnt );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("SendMail") );
	}

	m_ctrlProgress.SetPos( 70 );

	MessageBox( _T("발송이 완료되었습니다") , _T("일괄 메일 보내기") , MB_OK | MB_ICONINFORMATION );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3193::RecordLog( INT nMailClass , INT nPos , CString sMailName , INT nMailRecordCnt , INT nMailSuccessCnt , INT nMailFailCnt )
{
	EFS_BEGIN

	INT ids;

	
	CString sLog;
	sLog.Format( _T("메일을 보낼 총 자료 갯수 : %d , \n성공한 수 : %d , 실패한 수 : %d") , nMailRecordCnt , nMailSuccessCnt , nMailFailCnt );

	ids = RecordProcessingLog( sMailName , sLog );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RecordLog") );
	
	if( nMailFailCnt > 0 )
	{
		ids = RecordFailDM(nMailClass); 
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RecordLog") );
	}
	
	m_ctrlProgress.SetPos( nPos );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3193::RecordProcessingLog( CString sMailName , CString sLog )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pLogDM = FindDM( _T("DM_BO_LOC_3193"));
	if( pLogDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RecordProcessingLog") );

	pLogDM->InsertRow(-1);

	
	CString strGetData;
	ids = pLogDM->SetCellData( _T("시간") ,(CLocCommonAPI::GetCurrentDateTime(this, strGetData)).Format(_T("%m월%d일 %H시%M분%S초")) , pLogDM->GetRowCount()-1 );

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RecordProcessingLog") );

	ids = pLogDM->SetCellData( _T("메일종류") , sMailName , pLogDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("RecordProcessingLog") );

	ids = pLogDM->SetCellData( _T("내용") , sLog , pLogDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("RecordProcessingLog") );

	ids = CLocCommonAPI::InsertGridRow( this , _T("CM_BO_LOC_3193") , _T("진행상황그리드") , pLogDM->GetRowCount()-1 , pLogDM->GetRowCount()-1 , 500);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("RecordProcessingLog") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3193::InitControl()
{
	EFS_BEGIN

	InsertTablTitle( _T("진행 상황") );
	
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3193::InsertTablTitle( CString sTitle )
{
	EFS_BEGIN

	CTabCtrl *pTab = ((CTabCtrl*)GetDlgItem(IDC_tab3100_TITLE));

	
	pTab->InsertItem(pTab->GetItemCount(), sTitle);

	m_asTabTitle.Add( sTitle );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3193::RecordFailDM( INT nMailClass )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3193_FAIL_LOG") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("RecordFailDM") );

	ids = MailMgr->AddFailLog( nMailClass , pDM );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("RecordFailDM") );

	InsertTablTitle( MailMgr->GetMailName(nMailClass) );

	return 0;

	EFS_END
	return -1;

}

UINT RunThread(LPVOID lParam)
{
	
	CBO_LOC_3193 *pObj = (CBO_LOC_3193 *)lParam;
	if ( pObj == NULL )
		return -1;
	
	pObj->SendMail();
	
	return 0;
}

VOID CBO_LOC_3193::OnClicktabTITLE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	ViewCurrentGrid();
	
	*pResult = 0;

	EFS_END
}



INT CBO_LOC_3193::ViewCurrentGrid()
{
	EFS_BEGIN

	INT ids;

	CTabCtrl *pTab = ((CTabCtrl*)GetDlgItem(IDC_tab3100_TITLE));

	ids = ViewGrid(m_asTabTitle.GetAt(pTab->GetCurSel()));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewCurrentGrid") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3193::ViewGrid( CString sTitle )
{
	EFS_BEGIN

	INT ids;

	GetDlgItem(IDC_gridMAILLOG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_gridMAILLOG2)->ShowWindow(SW_HIDE);

	if( sTitle.Compare(_T("진행 상황")) == 0 )
	{
		GetDlgItem(IDC_gridMAILLOG)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_gridMAILLOG2)->ShowWindow(SW_SHOW);
		ids = ViewFailLogGrid(sTitle);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewGrid( CString sTitle )") );
	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3193::ViewFailLogGrid( CString sTitle )
{
	EFS_BEGIN

	INT ids;

	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3193_FAIL_LOG") );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewFailLogGrid") );

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl( _T("실패사유그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewFailLogGrid") );

	pGrid->RemoveAll2();


	CESL_DataMgr *pFailDM = FindDM( _T("DM_BO_LOC_3193_FAIL_LOG") );
	if( pFailDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ViewFailLogGrid") );

	CString sMailTitle;
	INT nGridIndex = 0;
	for( INT i = 0 ; i < pFailDM->GetRowCount() ; i++ )
	{
		ids = pFailDM->GetCellData( _T("메일종류") , i , sMailTitle );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewFailLogGrid") );
		
		if( sMailTitle.Compare( sTitle ) != 0 ) continue;

		ids = CLocCommonAPI::InsertGridRow( this , pGrid , i , nGridIndex , 500 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ViewFailLogGrid") );

		nGridIndex++;
	}

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3193::OnTimer(UINT nIDEvent) 
{
	EFS_BEGIN

	if( nIDEvent == 0 )
	{
		KillTimer(0);
		if( GetDlgItem(IDC_gridMAILLOG)->GetSafeHwnd() == NULL )
		{
			SetTimer(0,100,NULL);
			return ;
		}
		else
		{
			RunProc();
			return ;
		}
	}
	CDialog::OnTimer(nIDEvent);

	EFS_END
}

HBRUSH CBO_LOC_3193::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
