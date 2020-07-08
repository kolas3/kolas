// BO_LOC_3390.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3390.h"
#include "direct.h"
#include "StringTokenizer.h"
#include "BO_LOC_3391.h"
#include "BO_LOC_3000Api.h"
#include "ExportToHCell.h"
#include "..\\..\\..\\ESL\\ESL\\ESLDlg_ExportType.h"
#include "..\\..\\..\\ESL\\ESL\\XLEzAutomation.h"
#include "ExportToCSV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3390 dialog


CBO_LOC_3390::CBO_LOC_3390(CESL_Mgr* pParent,CLoanShareManager* pLoanShareManager)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3390)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sParentDMAlias = _T("");
	m_sParentCMAlias = _T("");
	m_sParentGridAlias = _T("");

	SetParent( _T("DM_BO_LOC_3300_LIST") , _T("CM_BO_LOC_3300") , _T("MainGrid") );

	m_pLoanShareManager = pLoanShareManager;

	m_sIsUnityLoanService = _T("");
	m_sEcoMailUseYN = _T("");
	m_civilNumViewMode = FALSE;
	m_nMode = 0;	
	m_sWorkingDirectory = _T("");
	m_sFileName = _T("");
	m_strLibCode = _T("");
	
	CESLDlg_ExportType Dlg;
	m_nDialogReturn = Dlg.DoModal();
	if(m_nDialogReturn == IDOK)
	{
		m_bIsExcel	 = Dlg.m_bIsExcel;
		m_bIsHancell = Dlg.m_bIsHancell;
		m_bIsCSV	 = Dlg.m_bIsCSV;
	}
}

CBO_LOC_3390::~CBO_LOC_3390()
{
}

VOID CBO_LOC_3390::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3390)
	
	CString strOutput;
	if(m_bIsExcel == TRUE)
	{
		strOutput.Format(_T("엑셀 헤더 설정하기"));
	}
	else if(m_bIsHancell == TRUE)
	{
		strOutput.Format(_T("한셀 헤더 설정하기"));
	}
	else if(m_bIsCSV == TRUE)
	{
		strOutput.Format(_T("CSV 헤더 설정하기"));
	}
	DDX_Text(pDX, IDC_bSET_DM_ALIAS, strOutput);

	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3390, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3390)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnFileSelect, OnbtnFileSelect)
	ON_BN_CLICKED(IDC_btnFileInputOutput, OnbtnFileInputOutput)
	ON_BN_CLICKED(IDC_btnDetailInfo, OnbtnDetailInfo)
	ON_BN_CLICKED(IDC_btnDelete, OnbtnDelete)
	ON_BN_CLICKED(IDC_bSET_DM_ALIAS, OnbSETDMALIAS)
	ON_BN_CLICKED(IDC_chkOVERWRITE, OnchkOVERWRITE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3390 message handlers

BOOL CBO_LOC_3390::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	
	if(m_nDialogReturn != IDOK)
	{
		return FALSE;
	}
	if (InitESL_Mgr(_T("BO_LOC_3390")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	CheckMailingSystem();

	SetControlFromMode();

	INT ids = InitDMFromMode();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::OnInitDialog()") );
	if( ids > 0 )
	{
		AfxMessageBox(_T("반출하실 자료를 먼져 선정해주십시오"));
		OnCancel();
	}

	MakeColumnAliasDM();

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3390_GRID"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( _T("FileUseInfoGrid") ));
	if(!m_pInfo->m_bUserInfoExportYN)
	{	
		pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		pGrid->m_bPrivacyPrintLogRecord = TRUE;
		pGrid->m_strPrintInfo = _T("이용자정보 파일 반입반출");
	}
	
	if ( TRUE == m_strLibCode.IsEmpty() )
	{
		m_strLibCode = m_pInfo->LIB_CODE;
	}

	return TRUE;  

	EFS_END
	return FALSE;

}

VOID CBO_LOC_3390::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid * pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_FileInputOuputGrid);
	
	if(pGrid->GetSafeHwnd() == NULL) {
		return;
	}
	CRect TopRect, BottomRect;
	GetDlgItem(IDC_TOP_POS)->GetWindowRect(&TopRect);
	GetDlgItem(IDC_BOTTOM_POS)->GetWindowRect(&BottomRect);

	pGrid->MoveWindow( 0 , TopRect.Height() , cx, cy-BottomRect.Height()-TopRect.Height());
	

	EFS_END
}

VOID CBO_LOC_3390::SetControlFromMode()
{
	EFS_BEGIN

	if( m_nMode == 0 ) 
	{
		SetWindowText( _T("이용자 파일 반입") );

		GetDlgItem(IDC_btnFileInputOutput)->SetWindowText(_T("반 입"));
		GetDlgItem(IDC_btnDetailInfo)->SetWindowText(_T("수 정"));
		
		GetDlgItem(IDC_btnDelete)->ShowWindow(SW_SHOW);	

		CRect InputRect;
		GetDlgItem(IDC_btnFileInputOutputPOS )->GetWindowRect( &InputRect );
		ScreenToClient( InputRect );
		GetDlgItem(IDC_btnFileInputOutput)->MoveWindow(  InputRect.left , InputRect.top  , InputRect.Width() , InputRect.Height() );
		
		CRect CloseRect;
		GetDlgItem(IDC_btnCLOSEPOS )->GetWindowRect( &CloseRect );
		ScreenToClient( CloseRect );
		GetDlgItem(IDC_btnCLOSE)->MoveWindow( CloseRect.left , CloseRect.top , CloseRect.Width() , CloseRect.Height() );

		GetDlgItem(IDC_GROUPBOX_UP2)->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_GROUPBOX_BOTTOM2)->ShowWindow(SW_SHOW);	
		GetDlgItem(IDC_GROUPBOX_UP)->ShowWindow(SW_HIDE);	
		GetDlgItem(IDC_GROUPBOX_BOTTOM)->ShowWindow(SW_HIDE);	

	}
	else
	{
		GetDlgItem(IDC_chkOVERWRITE)->ShowWindow(SW_HIDE);	
		GetDlgItem(IDC_chkOVERWRITE_NULL)->ShowWindow(SW_HIDE);
		
		SetWindowText( _T("이용자 파일 반출") );
	}

	EFS_END
}

INT CBO_LOC_3390::InitDMFromMode()
{
	EFS_BEGIN

	INT ids;

	if( m_nMode == 0 ) return 0;

	CESL_DataMgr *pParentDM = FindDM( m_sParentDMAlias );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::InitDMFromMode()") );
	CESL_ControlMgr *pParentCM = FindCM( m_sParentCMAlias );
	if( pParentCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::InitDMFromMode()") );
	CESL_Grid *pParentGrid = (CESL_Grid*)(pParentCM->FindControl( m_sParentGridAlias ));
	if( pParentGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3390::InitDMFromMode()") );

	pParentGrid->SelectMakeList();

	INT nSelectMode = 0;
	INT nRowCount = pParentGrid->SelectGetCount();
	if( nRowCount < 1 ) 
		nSelectMode = 1;

	if( nSelectMode == 0 )
	{
		pParentGrid->SelectGetHeadPosition();
	}
	else
	{
		nRowCount = pParentDM->GetRowCount();
		if( nRowCount < 1 ) return 1;
	}

	INT nMaxUpdateRecordCnt = 100;

	INT nIndex;
	CString sRecKey;
	CString sRecKeyList;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		if( nSelectMode == 0 ) nIndex = pParentGrid->SelectGetIdx();
		else nIndex = i;

		CString sBookRecKey;
		ids = pParentDM->GetCellData( _T("REC_KEY") , nIndex ,sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3390::InitDMFromMode()") );

		if( (i!=0) && ((i!=1)&&(i%nMaxUpdateRecordCnt!=1))|| (i==1)) sRecKeyList += _T(",");
		sRecKeyList += sRecKey;

		if( ( i !=0 && (i%nMaxUpdateRecordCnt == 0)) || (i == (nRowCount-1)) ) 
		{
			ids = MakeClientDM( sRecKeyList );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2(  -5 , _T("CBO_LOC_3390::InitDMFromMode()") );
			sRecKeyList.Empty();
		}

		if( nSelectMode == 0 )
			if( i !=  nRowCount-1 ) pParentGrid->SelectGetNext();
	}

	ids = GridDisplay();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("") );

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3390::SetParent( CString sDMAlias , CString sCMAlias , CString sGridAlias  )
{
	EFS_BEGIN

	m_sParentDMAlias = sDMAlias;
	m_sParentCMAlias = sCMAlias;
	m_sParentGridAlias = sGridAlias;

	EFS_END
}

INT CBO_LOC_3390::MakeClientDM( CString sRecKeyList )
{
	EFS_BEGIN

	INT ids;

	CString sWhere;
	CString sTmpDMAlias = _T("DM_BO_LOC_3390_TMP");
	CESL_DataMgr *pTmpDM = FindDM( sTmpDMAlias );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::MakeClientDM( CString sRecKeyList )") );

	pTmpDM->FreeData();

	sWhere.Format( _T("REC_KEY IN (%s)") , sRecKeyList );

	ids = CLocCommonAPI::RefreshDataManager( this , pTmpDM , sWhere );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::MakeClientDM( CString sRecKeyList )") );
	
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pTmpDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8;
		for(INT i = 0; i < pTmpDM->GetRowCount(); i++)
		{
			pTmpDM->GetCellData(_T("집_주소"), i, strGetData);
			asDes1.Add(strGetData);
			pTmpDM->GetCellData(_T("근무지_주소"), i, strGetData);
			asDes2.Add(strGetData);
			pTmpDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes3.Add(strGetData);
			pTmpDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes4.Add(strGetData);
			pTmpDM->GetCellData(_T("휴대폰"), i, strGetData);
			asDes5.Add(strGetData);
			pTmpDM->GetCellData(_T("비밀번호"), i, strGetData);
			asDes6.Add(strGetData);
			pTmpDM->GetCellData(_T("집_전화번호"), i, strGetData);
			asDes7.Add(strGetData);
			pTmpDM->GetCellData(_T("근무지_전화번호"), i, strGetData);
			asDes8.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes1);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_ADDR1"), asDes2);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes3);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes4);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes5);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("PASSWORD"), asDes6);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes7);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes8);
					p3000Api->CloseSocket();	
				}
				for(i = 0; i < pTmpDM->GetRowCount(); i++)
				{
					pTmpDM->SetCellData(_T("집_주소"), asDes1.GetAt(i), i);
					pTmpDM->SetCellData(_T("근무지_주소"), asDes2.GetAt(i), i);
					pTmpDM->SetCellData(_T("주민등록번호"), asDes3.GetAt(i), i);
					pTmpDM->SetCellData(_T("E_MAIL"), asDes4.GetAt(i), i);
					pTmpDM->SetCellData(_T("휴대폰"), asDes5.GetAt(i), i);
					pTmpDM->SetCellData(_T("비밀번호"), asDes6.GetAt(i), i);
					pTmpDM->SetCellData(_T("집_전화번호"), asDes7.GetAt(i), i);
					pTmpDM->SetCellData(_T("근무지_전화번호"), asDes8.GetAt(i), i);
				}
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
		asDes8.RemoveAll(); asDes8.FreeExtra();
	}

	ids = AddTmpDMToDefaultDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3390::MakeClientDM( CString sRecKeyList )") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::AddTmpDMToDefaultDM()
{
	EFS_BEGIN

	INT ids;

	CString sWhere;
	CString sDMAlias = _T("DM_BO_LOC_3390");
	CString sTmpDMAlias = _T("DM_BO_LOC_3390_TMP");
	CESL_DataMgr *pTmpDM = FindDM( sTmpDMAlias );
	if( pTmpDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::MakeClientDM( CString sRecKeyList )") );

	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::MakeClientDM( CString sRecKeyList )") );

	INT nAliasCnt;
	nAliasCnt = pDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDM->GetAllAlias( sColAlias , nAliasCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3390::MakeClientDM( CString sRecKeyList )") );

	CString sResult;
	for( INT j = 0 ; j < pTmpDM->GetRowCount() ; j++ )
	{
		pDM->InsertRow(-1);

		for( INT i = 0 ; i < nAliasCnt ; i++ )
		{
			if( sColAlias[i].Compare(_T("선정"))==0 ) continue;

			ids = pTmpDM->GetCellData( sColAlias[i] , j , sResult );
			if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3390::MakeClientDM( CString sRecKeyList )") );
			if( ids == -4012 ) sResult.Empty();
			
			ids = pDM->SetCellData( sColAlias[i] , sResult , pDM->GetRowCount()-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3390::MakeClientDM( CString sRecKeyList )") );
		}
	}

	delete []sColAlias;
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::GridDisplay()
{
	EFS_BEGIN

	INT ids;

	ids = CodeChangeInDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::GridDisplay()") );
	
	ids = AllControlDisplay( _T("CM_BO_LOC_3390_GRID") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::GridDisplay()") );

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::CodeChangeInDM()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::CodeChangeInDM()") );

	const INT nCnt = 3;

	TCHAR *sCodeAlias[nCnt][2] = { 
		{ _T("대출소속정보") , _T("소속키") } , 
		{ _T("대출직급정보") , _T("직급키") } ,
		{ _T("자료실구분") , _T("자료실") }
	};

	for( INT i = 0 ;i < nCnt ; i++ )
	{
		ids = m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM , sCodeAlias[i][0] , sCodeAlias[i][1]  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::CodeChangeInDM()") );
	}

	CString sCodeValue = _T("");

	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( _T("회원구분") , i , sCodeValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3390::CodeChangeInDM()") ); 

		if( sCodeValue.Compare(_T("0"))==0 )
			sCodeValue = _T("정회원");
		else if( sCodeValue.Compare(_T("1"))==0 )
			sCodeValue = _T("비회원");

		ids = pDM->SetCellData( _T("회원구분") ,  sCodeValue , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3390::CodeChangeInDM()") ); 
	}

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3390::OnbtnCLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CBO_LOC_3390::OnbtnFileSelect() 
{
	EFS_BEGIN

	INT ids;

	ids = LoadFile();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_LOC_3390::OnbtnFileSelect()") );

	EFS_END
}

INT CBO_LOC_3390::LoadFile()
{
	EFS_BEGIN

    INT     ids;
	TCHAR    sCurrentDirectory[_MAX_PATH];

    if ( _tgetcwd( sCurrentDirectory, _MAX_PATH ) == NULL )
	{
		AfxMessageBox( _T("SYSTEM NOT SEARCH DIRECTORY") );
		return -1;
	}

	bool bMode;
	if( m_nMode == 0 )
		bMode = true;
	else 
		bMode = false;
 
	CString sFileName, strExecuteType1, strExecuteType2, strExecuteType3;
	if(m_bIsExcel == TRUE)
	{
		strExecuteType1.Format(_T("xls"));
		strExecuteType2.Format(_T("*.xls"));
		strExecuteType3.Format(_T("XLS(*.XLS)|*.xls|모든파일(*.*)|*.*||"));
	}
	else if(m_bIsHancell == TRUE)
	{
		strExecuteType1.Format(_T("cell"));
		strExecuteType2.Format(_T("*.cell"));
		strExecuteType3.Format(_T("한셀파일(*.cell)|*.cell|모든파일(*.*)|*.*||"));
	}
	else if(m_bIsCSV == TRUE)
	{
		strExecuteType1.Format(_T("csv"));
		strExecuteType2.Format(_T("*.csv"));
		strExecuteType3.Format(_T("CSV파일(탭단위)(*.csv)|*.csv|모든파일(*.*)|*.*||"));
	}
	
	CFileDialog dlg(bMode, strExecuteType1, strExecuteType2, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strExecuteType3);
	if ( IDOK != dlg.DoModal() ) return 0;
	if ( dlg.GetFileTitle().IsEmpty() == TRUE ) return 1;
	sFileName = dlg.GetPathName();

	if( m_nMode == 0 )
	{
		if (m_bIsExcel == TRUE)
		{	
			ids = ExcelFileToDM( sFileName );	
		}
		else if(m_bIsHancell == TRUE)
		{
			ids = HCellFileToDM( sFileName );
		}
		else if(m_bIsCSV == TRUE)
		{
			ids = CSVFileToDM( sFileName );	
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::LoadFile()") );

		ids = GridDisplay();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::LoadFile()") );
	}

	m_sFileName = sFileName;
	ids = SetControlData( _T("CM_BO_LOC_3390_INPUT") , _T("파일이름") ,  m_sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("CBO_LOC_3390::LoadFile()") ); 
	
	_tchdir( sCurrentDirectory ) ;

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::FileToDM( CString sFileName )
{
	EFS_BEGIN

	INT ids;

	CFileException ex;
	CStdioFile fFile;
	ids = fFile.Open( sFileName , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary , &ex);
	if( ids == 0 )
	{
         TCHAR szError[1024];
         ex.GetErrorMessage(szError, 1024);
         CString sMsg;
		 sMsg.Format( _T("파일을 열수 없습니다.: %s") , szError );
		 AfxMessageBox( sMsg );
         return 1;
	}
	
		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}

	CString *sColAlias;
	INT nColCnt;
	CString sSeperator;
	sSeperator.Format( _T("%c") , SEPERATOR );

	CString sLine;
	fFile.ReadString( sLine );
	sLine.Delete(0);

	CStringTokenizer sToken( sLine , sSeperator );

	nColCnt = sToken.GetTokenCnt();
	
	sColAlias = new CString[nColCnt];
	
	for( INT i=0 ; i < nColCnt ; i++ )
	{
		sColAlias[i] = sToken.GetAt(i);
	}

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::FileToDM( CString sFileName )") ); 
	pDM->FreeData();

	INT nLine = 1;
	while( fFile.ReadString( sLine ) )
	{	
		sToken.ReMake( sLine , sSeperator );

		if( sToken.GetTokenCnt() != nColCnt )
		{
			fFile.Close();
			delete []sColAlias;
			CString sMsg;
			sMsg.Format( _T("%d 번째 행이 잘못되었습니다. 파일을 읽을수가 없습니다.") , nLine );
			AfxMessageBox( sMsg );
			continue;
		}

		pDM->InsertRow(-1);
		for( i = 0 ; i < nColCnt ; i++ )
		{
			ids = pDM->SetCellData( sColAlias[i] , sToken.GetAt(i) , pDM->GetRowCount()-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::FileToDM( CString sFileName )") ); 
		}
		nLine++;
	}

	delete []sColAlias;

	fFile.Close();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::ExcelFileToDM( CString sFileName )
{
	EFS_BEGIN

	CRect rc;
	m_pParentWnd->GetWindowRect(rc);
	C_ExcelManControl ExcelCtrl;
	
	ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rc, m_pParentWnd, WM_USER + 10000);
	if (ExcelCtrl.m_hWnd == NULL) 
	{
		AfxMessageBox( _T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오.") );
		return 1;
	}

    CExcelManager ExcelMgr;
	ExcelMgr.SetManager(&ExcelCtrl);		
    if(ExcelMgr.OpenManager() == FALSE) 
	{
        AfxMessageBox( _T("NOT Installed Excelmanager.ocx or No Excel!!!") );
		return 2;
	}

	ExcelMgr.SetUseTemplate(TRUE, sFileName);
	if (ExcelMgr.OpenManager() == FALSE)
	{
		AfxMessageBox( _T("Open Failed") );
		return 3;
	}
		
	CArray <CString,CString> 	sColAlias;
	sColAlias.RemoveAll();

	INT nColCount = 0;
	CString strAlias = _T("");

	while (TRUE)
	{
		strAlias = ExcelMgr.GetTextMatrix(1, nColCount + 1);
		if (strAlias.IsEmpty()) break;

		sColAlias.Add(strAlias);
		nColCount++;
	}

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::FileToDM( CString sFileName )") ); 
	pDM->FreeData();

	CString sValue = _T("");
	BOOL bStop     = FALSE;
	BOOL bRowEmpty = TRUE;
	INT nRow = 0;

	while(TRUE)
	{	
		bRowEmpty = TRUE;
		pDM->InsertRow( -1 );
		nRow = pDM->GetRowCount();

		for( INT i = 0 ; i < nColCount ; i++ )
		{
			sValue = ExcelMgr.GetTextMatrix(nRow + 1, i + 1);

			if( _tcscmp(sValue, _T("#_TEOF")) == 0 )
			{
				bStop = TRUE;
				i = nColCount;
			}

			if( !bStop )
			{
				if( !sValue.IsEmpty() )
					bRowEmpty = FALSE;

				sValue.Replace( _T("\n") , _T("\r\n") );

				if(_T("Y")==m_sIsUnityLoanService)
				{
					if(_T("주민등록번호")==sColAlias.GetAt(i))
					{
						CString strCivilNo;
						this->ESL_DES_Encryption(sValue,strCivilNo);
						CLocCommonAPI::ChangeSecuriyCivilNo(this,sValue,m_sIsUnityLoanService);
						pDM->SetCellData(_T("주민등록번호_CRYPTO") , strCivilNo , nRow - 1);
					}
				}

				pDM->SetCellData(sColAlias.GetAt(i) , sValue , nRow - 1);
			}
		}

		if( bRowEmpty )	break;
		if( bStop )		break;
	}
	
	pDM->DeleteRow( nRow - 1 );
	ExcelMgr.CloseManager();
	
	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3390::OnbtnFileInputOutput() 
{
	EFS_BEGIN

	INT ids;
	
	if( m_nMode == 0 )	
	{
		ids = DMToDB();	
	}
	else
	{
		if(m_bIsExcel == TRUE)
		{
			ids = MakeDMToExcelFile();
		}
		else if(m_bIsHancell == TRUE)
		{
			ids = MakeDMToHCellFile();
		}
		else if(m_bIsCSV == TRUE)
		{
			ids = MakeDMToCSVFile();
		}
	}

	EFS_END
}

INT CBO_LOC_3390::MakeDMToFile()
{
	EFS_BEGIN

	INT ids;

	ids = GetControlData( _T("CM_BO_LOC_3390_INPUT") , _T("파일이름") , m_sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::MakeDMToFile()") ); 

	if( m_sFileName.IsEmpty() ) 
	{
		AfxMessageBox(_T("반출될 파일 이름을 먼저 입력하십시오"));
		return 1;
	}

	CESL_Grid *pGrid = GetClientGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::MakeDMToFile()") ); 

	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();
	if( nRowCount < 1 ) 
	{
		AfxMessageBox(_T("반출할 이용자를 먼저 선택해 주십시오"));
		return 1;
	}

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_3390::MakeDMToFile()") );

	INT nAliasCnt;
	nAliasCnt = pDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDM->GetAllAlias( sColAlias , nAliasCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3390::MakeDMToFile()") );

	CString sSeperator;
	CString sLineEnd;
	sSeperator.Format( _T("%c") , SEPERATOR );
	sLineEnd.Format( _T("%s\r\n") , sSeperator );

	CStdioFile fMakeFile;
	CFileException ex;
	ids = fMakeFile.Open( m_sFileName , CFile::modeCreate | CFile::modeWrite | CFile::typeBinary , &ex );
	if( ids == 0 )
	{
         TCHAR szError[1024];
         ex.GetErrorMessage(szError, 1024);
         CString sMsg;
		 sMsg.Format( _T("파일을 생성할수 없습니다.: %s") , szError );
		 AfxMessageBox( sMsg );
         return 1;
	}

	if( 0 == fMakeFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		fMakeFile.Write( &cUni, sizeof(TCHAR));
	}
	
	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		if( i != 0 ){
			fMakeFile.Write( sSeperator , sSeperator.GetLength() );
		}
		else
			fMakeFile.Write( _T(".")  , 1 );

		fMakeFile.Write( sColAlias[i] , _tcsclen( sColAlias[i]) );
	}
	fMakeFile.Write( sLineEnd , sLineEnd.GetLength() );

	INT nIndex;
	CString sResult;
	pGrid->SelectGetHeadPosition();
	for( INT j = 0 ; j < nRowCount ; j++ )
	{
		nIndex = pGrid->SelectGetIdx();

		for( INT i = 0 ; i < nAliasCnt ; i++ )
		{
			ids = pDM->GetCellData( sColAlias[i] , nIndex , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3390::MakeDMToFile()") );
			
			if( i != 0 ) fMakeFile.Write( sSeperator , sSeperator.GetLength() );
			fMakeFile.Write( sResult , sResult.GetLength() );
		}
		fMakeFile.Write( sLineEnd , sLineEnd.GetLength() );

		pGrid->SelectGetNext();
	}

	delete []sColAlias;

	fMakeFile.Close();

	CString sMsg;
	sMsg.Format( _T("'%s' 파일이 생성되었습니다.") , m_sFileName );
	AfxMessageBox( sMsg );
	// 4. 끝
	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::MakeDMToExcelFile()
{
EFS_BEGIN

	INT ids;

	ids = GetControlData( _T("CM_BO_LOC_3390_INPUT") , _T("파일이름") , m_sFileName );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MakeDMToExcelFile") ); 
	}

	if( m_sFileName.IsEmpty() ) 
	{
		OnbtnFileSelect();

		if( m_sFileName.IsEmpty() )
		{
			return 1;
		}
	}

	CESL_Grid *pGrid = GetClientGridPtr();
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeDMToExcelFile") ); 
	}

	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();
	if( 1 > nRowCount ) 
	{
		AfxMessageBox(_T("반출할 이용자를 먼저 선택해 주십시오"));
		return 1;
	}

	INT nAliasCnt = GetColunAliasCount();
	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];

	ids = GetColumnAlias( sColAlias , nAliasCnt );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("MakeDMToExcelFile") );
	}

	CRect rc;
	GetClientRect(&rc);
	
	C_ExcelManControl ExcelCtrl;
	ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rc, this, 8805);
	if( NULL == ExcelCtrl.m_hWnd ) 
	{
		MessageBox( _T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오.") , _T("Warnning!") , MB_ICONEXCLAMATION );
		delete []sColAlias;
		return 1;
	}

    CExcelManager ExcelManager(&ExcelCtrl);
    if( FALSE == ExcelManager.OpenManager() ) 
	{
        MessageBox( _T("NOT Installed Excelmanager.ocx or No Excel!!!") , _T("Warnning!") , MB_ICONEXCLAMATION );
		delete []sColAlias;
		return 2;
	}

	ExcelManager.Font(1, 1, 40, 1, TRUE, FALSE, RGB(0, 0, 0), 11);

    ExcelManager.Horizontal_Alignment(1, 1, 40, 1, 3);

	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		ExcelManager.SetTextMatrix(1, i+1, sColAlias[i]);
	}

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("MakeDMToExcelFile") );
	}
	
	if(PrivacyPrintLogRecord(pDM, _T("이용자 관리 파일반출"), _T("E") ,0 ) < 0 ) return 2;
	
	INT nIndex = 0;
	INT nRow   = 2;
	CString sGetValue = _T("");
	pGrid->SelectGetHeadPosition();

	for( i=0 ; i<nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		for( INT j=0 ; j<nAliasCnt ; j++ )
		{
			ids = pDM->GetCellData( sColAlias[j] , nIndex , sGetValue );
			if( 0 > ids )
			{
				CString sEMsg = _T("");
				sEMsg.Format( _T("GetCellData( %s , %d , sResult)") , sColAlias[j] , nIndex );
				AfxMessageBox( sEMsg );
			}
			
			if( _T("Y")==m_sIsUnityLoanService && _T("주민등록번호")==sColAlias[j] )
			{
				CString strCivilNo;
				ids = pDM->GetCellData( _T("주민등록번호_CRYPTO") , nIndex , strCivilNo );
				this->ESL_DES_Decryption(strCivilNo, sGetValue);
			}

			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("MakeDMToExcelFile") );
			}

			if( _T("관리구분") == sColAlias[j] )
			{
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분") , sGetValue , sGetValue  );
				if( 0 > ids && -2004 != ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("DMToDB") );
				}
			}

			sGetValue = _T("'") + sGetValue;

			sGetValue.Replace(_T("\r"), NULL);
			ExcelManager.Cell_Format_String( 1 , 1 , nRow , j+1 );
			ExcelManager.SetTextMatrix( nRow , j+1 , sGetValue );
		}

		pGrid->SelectGetNext();
		nRow++;
	}

	ExcelManager.SaveAs(m_sFileName);
    ExcelManager.CloseManager();

	CString sMsg;
	sMsg.Format( _T("'%s' 파일이 생성되었습니다.") , m_sFileName );
	AfxMessageBox( sMsg );

	delete []sColAlias;

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3390::MakeDMToCSVFile()
{
EFS_BEGIN

	INT ids;
	
	ids = GetControlData( _T("CM_BO_LOC_3390_INPUT") , _T("파일이름") , m_sFileName );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MakeDMToExcelFile") ); 
	}

	if( m_sFileName.IsEmpty() ) 
	{
		OnbtnFileSelect();

		if( m_sFileName.IsEmpty() )
		{
			return 1;
		}
	}

	CESL_Grid *pGrid = GetClientGridPtr();
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeDMToExcelFile") ); 
	}

	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();
	if( 1 > nRowCount ) 
	{
		AfxMessageBox(_T("반출할 이용자를 먼저 선택해 주십시오"));
		return 1;
	}

	INT nAliasCnt = GetColunAliasCount();
	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];

	ids = GetColumnAlias( sColAlias , nAliasCnt );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("MakeDMToExcelFile") );
	}

	CRect rc;
	GetClientRect(&rc);

	CString strResult, strTempData;

	strResult.Empty();
	strTempData.Empty();
	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		strTempData.Format(_T("%s\t"), sColAlias[i]);
		strResult += strTempData;
	}
	strResult += _T("\r\n");

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("MakeDMToExcelFile") );
	}

	if(PrivacyPrintLogRecord(pDM, _T("이용자 관리 파일반출"), _T("C") ,0 ) < 0 ) return 2;
		
	INT nIndex = 0;
	INT nRow   = 2;
	CString sGetValue = _T("");
	pGrid->SelectGetHeadPosition();

	for( i=0 ; i<nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		for( INT j=0 ; j<nAliasCnt ; j++ )
		{
			ids = pDM->GetCellData( sColAlias[j] , nIndex , sGetValue );
			if( 0 > ids )
			{
				CString sEMsg = _T("");
				sEMsg.Format( _T("GetCellData( %s , %d , sResult)") , sColAlias[j] , nIndex );
				AfxMessageBox( sEMsg );
			}
			
			if( _T("Y")==m_sIsUnityLoanService && _T("주민등록번호")==sColAlias[j] )
			{
				CString strCivilNo;
				ids = pDM->GetCellData( _T("주민등록번호_CRYPTO") , nIndex , strCivilNo );
				this->ESL_DES_Decryption(strCivilNo, sGetValue);
			}

			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("MakeDMToExcelFile") );
			}

			if( _T("관리구분") == sColAlias[j] )
			{
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분") , sGetValue , sGetValue  );
				if( 0 > ids && -2004 != ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("DMToDB") );
				}
			}

			sGetValue.Replace(_T("\r"), NULL);
			
			
			strTempData.Format(_T("%s\t"), sGetValue);
			strResult += strTempData;
		}
		strResult += _T("\r\n");
		pGrid->SelectGetNext();
		nRow++;
	}

	CXLEzAutomation XL;			

	XL.m_bIsExcel = FALSE;
	XL.m_bIsHancell = FALSE;
	XL.m_bIsCSV = TRUE;
	XL.ExportCString(strResult, m_sFileName);

	CString sMsg;
	sMsg.Format( _T("'%s' 파일이 생성되었습니다.") , m_sFileName );
	AfxMessageBox( sMsg );

	delete []sColAlias;

	return 0;

EFS_END
return -1;
}

CESL_Grid* CBO_LOC_3390::GetClientGridPtr()
{
	EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3390_GRID");
	CString sGridAlias = _T("FileUseInfoGrid");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return NULL;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return NULL;

	return pGrid;

	EFS_END
	return NULL;

}

INT CBO_LOC_3390::DMToDB()
{
EFS_BEGIN

	INT ids;

	CESL_Grid *pGrid = GetClientGridPtr();
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("DMToDB") );
	}
	
	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();

	if( 1 > nRowCount )
	{
		AfxMessageBox( _T("먼저 반입할 자료를 선택해주십시오") );
		return 1;
	}

	ids = InitLogDM();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("DMToDB") ); 
	}

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("DMToDB") );
	}

	INT nCnt = pDM->GetRowCount();
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();

	pGrid->SelectGetHeadPosition();
	CString sRecKey = _T("");

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		CString	strData, strCodeValue;
		INT nIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("관리구분") , nIndex , strData );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("DMToDB") );

		strData.TrimLeft();
		strData.TrimRight();

		if( _T("적용안함") == strData || TRUE == strData.IsEmpty() )
		{
			ids = MakeFailureSuccessDM( pDM , nIndex , _T("관리구분 값이 존재하지 않습니다."), FALSE );

			pGrid->SelectGetNext();
			continue;
		}

		ids = m_pInfo->pCodeMgr->ConvertDescToCode( _T("관리구분") , strData , strCodeValue  );
		if(0>ids && -2005!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("DMToDB") );

		if(0==ids)
		{
			ids = pDM->SetCellData( _T("관리구분") , strCodeValue , nIndex );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("DMToDB") );
		}
		else if( -2005 == ids )
		{
			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분") , strData , strCodeValue  );
			if(0>ids && -2004!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("DMToDB") );

			if( -2004 == ids )
			{
				ids = MakeFailureSuccessDM( pDM , nIndex , _T("관리구분 값이 존재하지 않습니다."), FALSE );

				pGrid->SelectGetNext();
				continue;
			}

			strCodeValue.TrimLeft();	strCodeValue.TrimRight();
			strCodeValue.Replace( _T(" ") , _T("") );

			if( _T("적용안함") == strCodeValue || TRUE == strCodeValue.IsEmpty() )
			{
				ids = MakeFailureSuccessDM( pDM , nIndex , _T("관리구분 값이 존재하지 않습니다."), FALSE );

				pGrid->SelectGetNext();
				continue;
			}
		}

		ids = pDM->GetCellData( _T("소속키") , nIndex , strData );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("DMToDB") );

		strData.TrimLeft();
		strData.TrimRight();

		if( _T("적용안함") == strData || TRUE == strData.IsEmpty() )
		{
			ids = MakeFailureSuccessDM( pDM , nIndex , _T("소속 값이 존재하지 않습니다."), FALSE );

			pGrid->SelectGetNext();
			continue;
		}

		ids = m_pInfo->pCodeMgr->ConvertDescToCode( _T("대출소속정보") , strData , strCodeValue  );
		if(0>ids && -2005!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("DMToDB") );

		if( -2005 == ids )
		{
			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출소속정보") , strData , strCodeValue  );
			if(0>ids && -2004!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1100 , _T("DMToDB") );

			if( -2004 == ids )
			{
				ids = MakeFailureSuccessDM( pDM , nIndex , _T("소속 값이 존재하지 않습니다."), FALSE );

				pGrid->SelectGetNext();
				continue;
			}

			strCodeValue.TrimLeft();	strCodeValue.TrimRight();
			strCodeValue.Replace( _T(" ") , _T("") );

			if( _T("적용안함") == strCodeValue || TRUE == strCodeValue.IsEmpty() )
			{
				ids = MakeFailureSuccessDM( pDM , nIndex , _T("소속 값이 존재하지 않습니다."), FALSE );

				pGrid->SelectGetNext();
				continue;
			}

			strCodeValue = strData;
		}

		ids = pDM->SetCellData( _T("소속키") , strCodeValue , nIndex );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1110 , _T("DMToDB") );

		ids = pDM->GetCellData( _T("직급키") , nIndex , strData );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("DMToDB") );

		strData.TrimLeft();
		strData.TrimRight();
		strData.Replace( _T(" ") , _T("") );

		if( _T("적용안함") == strData || TRUE == strData.IsEmpty() )
		{
			ids = MakeFailureSuccessDM( pDM , nIndex , _T("직급 값이 존재하지 않습니다."), FALSE );

			pGrid->SelectGetNext();
			continue;
		}

		ids = m_pInfo->pCodeMgr->ConvertDescToCode( _T("대출직급정보") , strData , strCodeValue  );
		if(0>ids && -2005!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("DMToDB") );

		if( -2005 == ids )
		{
			ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보") , strData , strCodeValue  );
			if(0>ids && -2004!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1140 , _T("DMToDB") );

			if( -2004 == ids )
			{
				ids = MakeFailureSuccessDM( pDM , nIndex , _T("직급 값이 존재하지 않습니다."), FALSE );

				pGrid->SelectGetNext();
				continue;
			}

			strCodeValue.TrimLeft();	strCodeValue.TrimRight();
			strCodeValue.Replace( _T(" ") , _T("") );

			if( _T("적용안함") == strCodeValue || TRUE == strCodeValue.IsEmpty() )
			{
				ids = MakeFailureSuccessDM( pDM , nIndex , _T("직급 값이 존재하지 않습니다."), FALSE );

				pGrid->SelectGetNext();
				continue;
			}

			strCodeValue = strData;
		}

		ids = pDM->SetCellData( _T("직급키") , strCodeValue , nIndex );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1150 , _T("DMToDB") );
		
		ids = pDM->GetCellData( _T("자료실") , nIndex , strData );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1120 , _T("DMToDB") );

		strData.TrimLeft();
		strData.TrimRight();
		strData.Replace( _T(" ") , _T("") );

		if( TRUE != strData.IsEmpty() )
		{
			ids = m_pInfo->pCodeMgr->ConvertDescToCode( _T("자료실구분") , strData , strCodeValue );
			if(0>ids && -2005!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1130 , _T("DMToDB") );

			if( -2005 == ids )
			{
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , strData , strCodeValue  );
				if(0>ids && -2004!=ids) ERROR_MSG_RETURN_ERROR_CODE2( -1140 , _T("DMToDB") );

				if( -2004 == ids )
				{
					ids = MakeFailureSuccessDM( pDM , nIndex , _T("자료실 값이 존재하지 않습니다."), FALSE );

					pGrid->SelectGetNext();
					continue;
				}
				strCodeValue = strData;
			}
			ids = pDM->SetCellData( _T("자료실") , strCodeValue , nIndex );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1150 , _T("DMToDB") );
		}

		ids = pDM->GetCellData( _T("회원구분") , nIndex , strData );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1160 , _T("DMToDB") ); 

		if( 0 == strData.Compare( _T("정회원") ) ) strCodeValue = _T("0");
		else if( 0 == strData.Compare( _T("비회원") ) ) strCodeValue = _T("1");
		else if( 0 == strData.Compare( _T("준회원") ) ) strCodeValue = _T("2");
		else strCodeValue = _T("0");

		ids = pDM->SetCellData( _T("회원구분") , strCodeValue , nIndex );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1170 , _T("DMToDB") );
		
		ids = CheckDuplicate( pDM , nIndex , sRecKey );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1180 , _T("DMToDB") );

		if(1==ids) 
		{
			pGrid->SelectGetNext();
			continue;
		}

		if( ids != 2 )
		{
			ids = InsertDMToDB( pDM , nIndex );
		}
		else
		{
			ids = UpdateDMToDB( pDM , nIndex , sRecKey );
		}

		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1190 , _T("DMToDB") );

		ids = MakeFailureSuccessDM( pDM , nIndex , _T("성공") , TRUE );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1200 , _T("DMToDB") );

		pGrid->SelectGetNext();
	}
	
	ids = ViewInputResultMsg();
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1210 , _T("DMToDB") ); 

	ids = CodeChangeInDM();
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1220 , _T("DMToDB") ); 

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3390::CheckDuplicate( CESL_DataMgr *pDM , INT nIndex , CString &sRecKey )
{
EFS_BEGIN

	INT ids;

	CString sSql;
	CString sCount;

	const INT nCnt = 2;
	TCHAR *sColumnAlias[nCnt][2] = { { _T("대출자번호") , _T("USER_NO") } , { _T("대출자ID") , _T("USER_ID") } };
	
	CString strIs5163;
	GetManageValue( _T("기타") , _T("공통") , _T("5163") , _T("5163") ,  strIs5163);

	INT nCntFor = 2;
	if ( _T("Y") == strIs5163 ) 
	{
		nCntFor = 1;
	}

	CString sData;
	for( INT i = 0 ; i < nCntFor ; i++ )
	{
		ids = pDM->GetCellData( sColumnAlias[i][0] , nIndex  , sData );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("CheckDuplicate") );
		}

		if( TRUE == sData.IsEmpty() ) 
		{
			continue;
		}
		
		sSql.Format( _T("SELECT COUNT(*) FROM CO_LOAN_USER_TBL WHERE %s = '%s'") , sColumnAlias[i][1] , sData );

		ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sCount );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("CheckDuplicate") );
		}

		if( _ttoi(sCount) > 0 ) 
		{
			sSql.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE %s = '%s'") , sColumnAlias[i][1] , sData );

			ids = CLocCommonAPI::GetOneValueQuery( this , pDM , sSql , sRecKey );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("CheckDuplicate") );
			}

			INT nCheck = ((CButton*)GetDlgItem(IDC_chkOVERWRITE))->GetCheck();
			if( 1 != nCheck )
			{
				CString sFailMsg;
				sFailMsg.Format( _T("동일한 '%s'가 존재합니다.") , sColumnAlias[i][0] );
				ids = MakeFailureSuccessDM( pDM , nIndex , sFailMsg , FALSE );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("CheckDuplicate") );
				}

				return 1;
			}
			else
			{
				return 2;
			}			
		}

		if(_T("Y")==m_sIsUnityLoanService && _T("대출자번호")==sColumnAlias[i][0])
		{
			CKL_UserManagerDlg dlg(this);
			dlg.SetMode(7);
			dlg.SetUserNo(sData);
			if(IDOK == dlg.DoModal())
			{
				if(dlg.GetIsUserNoDuplicate())
				{
					CString strMsg;
					strMsg.Format(_T("동일한 대출자번호가 통합센터에 존재합니다."));
					ids = MakeFailureSuccessDM( pDM , nIndex , strMsg , FALSE );
					if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("CheckDuplicate") );
					return 1;
				}
			}
			else
			{
				CString sFailMsg;
				sFailMsg.Format( _T("'%s' 이용자반입을 취소하였습니다.") , sColumnAlias[i][0] );
				ids = MakeFailureSuccessDM( pDM , nIndex , sFailMsg , FALSE );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("CheckDuplicate") );
				}

				return 1;
			}
		}
	}
	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3390::DescChangeInDM()
{
	EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("DescChangeInDM()") );

	const INT nCnt = 2;
	TCHAR *sCodeAlias[nCnt][2] = { 
		{ _T("대출소속정보") , _T("소속키") } , 
		{ _T("대출직급정보") , _T("직급키") }
	};

	CString sCodeValue;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		for( INT j = 0 ;j < nCnt ; j++ )
		{			
			ids = pDM->GetCellData( sCodeAlias[j][1] , i , sCodeValue );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("DescChangeInDM()") );

			ids = m_pInfo->pCodeMgr->ConvertDescToCode( sCodeAlias[j][0] , sCodeValue , sCodeValue  );
			if( ids < 0 && (ids != -2005)) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("DescChangeInDM()") );

			ids = pDM->SetCellData( sCodeAlias[j][1] , sCodeValue , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("DescChangeInDM()") );
		}

		ids = pDM->GetCellData( _T("회원구분") , i , sCodeValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("DescChangeInDM()") ); 

		if( sCodeValue.Compare(_T("정회원"))==0 )
			sCodeValue = _T("0");
		else
			sCodeValue = _T("1");

		ids = pDM->SetCellData( _T("회원구분") ,  sCodeValue , i);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("DescChangeInDM()") ); 
	}

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::MakeFailureSuccessDM( CESL_DataMgr *pDM , INT nIndex , CString sFailMsg , bool bSuccess )
{
	EFS_BEGIN

	INT ids;

	CString sTargetDMAlias;

	if( bSuccess )
		sTargetDMAlias = _T("DM_BO_LOC_3390_SUCCESS");
	else
		sTargetDMAlias = _T("DM_BO_LOC_3390_TMP");

	CESL_DataMgr *pTargetDM = FindDM( sTargetDMAlias );
	if( pTargetDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeFailureSuccessDM( CESL_DataMgr *pDM , INT nIndex , CString sFailMsg , bool bSuccess )") );

	INT nAliasCnt;
	nAliasCnt = pDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDM->GetAllAlias( sColAlias , nAliasCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeFailureSuccessDM( CESL_DataMgr *pDM , INT nIndex , CString sFailMsg , bool bSuccess )") );

	CString sResult;

	pTargetDM->InsertRow(-1);

	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		if( sColAlias[i].Compare(_T("선정"))==0 )continue;

		ids = pDM->GetCellData( sColAlias[i] , nIndex , sResult );
		if( ids < 0 && ( ids!=-4012) ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("MakeFailureSuccessDM( CESL_DataMgr *pDM , INT nIndex , CString sFailMsg , bool bSuccess )") );
		if( ids == -4012 ) sResult.Empty();

		ids = pTargetDM->SetCellData( sColAlias[i] , sResult , pTargetDM->GetRowCount()-1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("MakeFailureSuccessDM( CESL_DataMgr *pDM , INT nIndex , CString sFailMsg , bool bSuccess )") );
	}
	ids = pTargetDM->SetCellData( _T("사유") , sFailMsg , pTargetDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("MakeFailureSuccessDM( CESL_DataMgr *pDM , INT nIndex , CString sFailMsg , bool bSuccess )") );

	delete []sColAlias;


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("CO_LOAN_USER_TBL");

	pDM->StartFrame();

	pDM->InitDBFieldData();

	INT nAliasCnt = GetColunAliasCount();
	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	ids = GetColumnAlias( sColAlias , nAliasCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3390::MakeDMToFile()") );

	CString *sDBFieldName;
	sDBFieldName = new CString[nAliasCnt];
	
	CString *sDBFieldType;
	sDBFieldType = new CString[nAliasCnt];

	INT nDMAliasCnt;
	nDMAliasCnt = pDM->RefList.GetCount();

	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{

		CESL_DataMgr::reference * pRef;
		POSITION pos;
		pos = pDM->RefList.GetHeadPosition();
		CString query, fields, isudf;
		INT col = 0;
		INT nColCnt = 0;
		CString sTmpType;
		for(INT j = 0; j < nDMAliasCnt; j++) {
			pRef = NULL;
			pRef = (CESL_DataMgr::reference*)(pDM->RefList).GetNext(pos);
			if (pRef) {
				sTmpType = pRef->DATA_TYPE;
				sTmpType.MakeLower();
				if( sTmpType.Compare(_T("udf") ) == 0 ) continue;
				if( sTmpType.Compare(_T("str"))==0 ) sTmpType = _T("STRING");
				else if( sTmpType.Compare(_T("int"))==0 ) sTmpType = _T("NUMERIC");
				else sTmpType = _T("STRING");

				if( sColAlias[i].Compare( pRef->FIELD_ALIAS ) != 0 ) continue;
				if( sColAlias[i].Compare( _T("그룹") ) == 0 ) continue;

				sDBFieldType[i] = sTmpType;
				sColAlias[i] = pRef->FIELD_ALIAS;
				sDBFieldName[i] = pRef->FIELD_NAME;
			}
		}	
	}

	CString sResult;

	BOOL bSecuMode = FALSE;
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				for( i = 0 ; i < nAliasCnt ; i++ )
				{
					if( sColAlias[i].IsEmpty() ) continue;
					if( sColAlias[i].Compare(_T("그룹") ) == 0 ) continue;

					ids = pDM->GetCellData( sColAlias[i] , nIndex , sResult );
					if( ids < 0 && (ids != -4012 )) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
					
					if(!sResult.IsEmpty())
					{
						if(_T("CARD_PASSWORD") == sDBFieldName[i] || _T("CIVIL_NO") == sDBFieldName[i] || _T("E_MAIL") == sDBFieldName[i] || _T("HANDPHONE") == sDBFieldName[i] || _T("IDX_HANDPHONE") == sDBFieldName[i] || _T("H_ADDR1") == sDBFieldName[i] || _T("H_PHONE") == sDBFieldName[i] || _T("W_ADDR1") == sDBFieldName[i] || _T("W_PHONE") == sDBFieldName[i] || _T("PASSWORD") == sDBFieldName[i] || _T("SECOND_PHONE") == sDBFieldName[i])
						{
							CStringArray asEnc;
							asEnc.Add(sResult);
							if(TRUE == p3000Api->ConnectSocket())
							{
								if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), sDBFieldName[i], asEnc))
								{
									sResult = asEnc.GetAt(0);
								}
								asEnc.RemoveAll(); asEnc.FreeExtra();
								p3000Api->CloseSocket();
							}
						}
					}

					ids = pDM->AddDBFieldData( sDBFieldName[i] , sDBFieldType[i] , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
				}
				bSecuMode = TRUE;
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}
	if(FALSE == bSecuMode)
	{
		for( i = 0 ; i < nAliasCnt ; i++ )
		{
			if( sColAlias[i].IsEmpty() ) continue;
			if( sColAlias[i].Compare(_T("그룹") ) == 0 ) continue;

			ids = pDM->GetCellData( sColAlias[i] , nIndex , sResult );
			if( ids < 0 && (ids != -4012 )) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
			
			ids = pDM->AddDBFieldData( sDBFieldName[i] , sDBFieldType[i] , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
		}
	}

	ids = pDM->GetCellData( _T("주민등록번호_CRYPTO") , nIndex , sResult );
	if( ids < 0 && (ids != -4012 )) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
	ids = pDM->AddDBFieldData( _T("CRYPTO_CIVIL_NO") , _T("STRING") , sResult );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("InsertDMToDB") );

	const INT nIndexCnt = 2;
	CString strArrIndex[nIndexCnt][3] =
	{{_T("IDX_USER_NAME"),_T("STRING"),_T("")}, {_T("IDX_HANDPHONE"),_T("STRING"),_T("")}};
	CString strArrIndexDMAlias[nIndexCnt] = {_T("이름"), _T("휴대폰")};

	for ( i=0; i<nIndexCnt; i++)
	{	
		ids = pDM->GetCellData( strArrIndexDMAlias[i], nIndex, sResult);
		if (sResult != _T("") ) 
		{
			if(TRUE == m_pInfo->m_bLibSecuUse)
			{
				CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
				if(NULL != p3000Api)
				{
					if(_T("IDX_HANDPHONE") == strArrIndex[i][0])
					{
						CStringArray asEnc;
						asEnc.Add(sResult);
						if(TRUE == p3000Api->ConnectSocket())
						{
							if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), strArrIndex[i][0], asEnc))
							{
								sResult = asEnc.GetAt(0);
							}
							asEnc.RemoveAll(); asEnc.FreeExtra();
							p3000Api->CloseSocket();
						}
					}
					delete p3000Api;
					p3000Api = NULL;
				}
			}
			ids = pDM->AddDBFieldData( strArrIndex[i][0], strArrIndex[i][1], sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
		}
	}

	const INT nDefaultCnt = 4;
	TCHAR *sDefaultField[nDefaultCnt][3] =
	{ {_T("USER_CLASS"),_T("STRING"),_T("0")} , {_T("MEMBER_CLASS"),_T("STRING"),_T("0")} , {_T("MAIL_SEND_AREA"),_T("STRING"),_T("H")} , {_T("REG_DATE"),_T("NUMERIC"),_T("SYSDATE")} };

	for( i=0 ; i < nDefaultCnt; i++ )
	{
		if( i == 3)
		{
			pDM->AddDBFieldData( sDefaultField[i][0] , sDefaultField[i][1] , sDefaultField[i][2] );
		}
		else
			pDM->AddDBFieldData( sDefaultField[i][0] , sDefaultField[i][1] , sDefaultField[i][2] );
	}

	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , pDM );


	CString sRecKey;

	ids = pMobileApi->MakeRecKey( sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

	pMobileApi->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	ids = pMobileApi->MakeInsertFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

	if( m_sEcoMailUseYN.Compare(_T("Y")) == 0 )
	{
		CString sGroupCodes;
		ids = pDM->GetCellData( _T("그룹") , nIndex , sGroupCodes );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

		ids = GroupCodeInsert( sGroupCodes , sRecKey , pMobileApi , pDM );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
	}

	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

	pDM->EndFrame();

	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	delete []sColAlias;
	delete []sDBFieldName;
	delete []sDBFieldType;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
 	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;

	m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);
	


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::UpdateDMToDB( CESL_DataMgr *pDM , INT nIndex , CString sRecKey )
{
	EFS_BEGIN

	INT ids;

	CString sTableName = _T("CO_LOAN_USER_TBL");

	pDM->StartFrame();

	pDM->InitDBFieldData();

	INT nAliasCnt = GetColunAliasCount();
	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	ids = GetColumnAlias( sColAlias , nAliasCnt );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBO_LOC_3390::MakeDMToFile()") );

	CString *sDBFieldName;
	sDBFieldName = new CString[nAliasCnt];
	
	CString *sDBFieldType;
	sDBFieldType = new CString[nAliasCnt];

	INT nDMAliasCnt;
	nDMAliasCnt = pDM->RefList.GetCount();

	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{

		CESL_DataMgr::reference * pRef;
		POSITION pos;
		pos = pDM->RefList.GetHeadPosition();
		CString query, fields, isudf;
		INT col = 0;
		INT nColCnt = 0;
		CString sTmpType;
		for(INT j = 0; j < nDMAliasCnt; j++) {
			pRef = NULL;
			pRef = (CESL_DataMgr::reference*)(pDM->RefList).GetNext(pos);
			if (pRef) {
				sTmpType = pRef->DATA_TYPE;
				sTmpType.MakeLower();
				if( sTmpType.Compare(_T("udf") ) == 0 ) continue;
				if( sTmpType.Compare(_T("str"))==0 ) sTmpType = _T("STRING");
				else if( sTmpType.Compare(_T("int"))==0 ) sTmpType = _T("NUMERIC");
				else sTmpType = _T("STRING");

				if( sColAlias[i].Compare( pRef->FIELD_ALIAS ) != 0 ) continue;

				sDBFieldType[i] = sTmpType;
				sColAlias[i] = pRef->FIELD_ALIAS;
				sDBFieldName[i] = pRef->FIELD_NAME;
			}
		}	
	}

	CString sResult;

	BOOL bOverWriteNull = ((CButton*)GetDlgItem(IDC_chkOVERWRITE_NULL))->GetCheck( );

	BOOL bSecuMode = FALSE;
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->CheckConnection())
			{
				for( i = 0 ; i < nAliasCnt ; i++ )
				{
					if( sColAlias[i].IsEmpty() ) continue;
					if( sColAlias[i].Compare(_T("그룹") ) == 0 ) continue;

					ids = pDM->GetCellData( sColAlias[i] , nIndex , sResult );
					if( ids < 0 && (ids != -4012 )) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

					if ( sResult.IsEmpty( ) && !bOverWriteNull ) continue;

					if(_T("H_ADDR1") == sDBFieldName[i] || _T("W_ADDR1") == sDBFieldName[i] || _T("CIVIL_NO") == sDBFieldName[i] || _T("E_MAIL") == sDBFieldName[i] || _T("HANDPHONE") == sDBFieldName[i] || _T("PASSWORD") == sDBFieldName[i] || _T("H_PHONE") == sDBFieldName[i] || _T("W_PHONE") == sDBFieldName[i])
					{
						CStringArray asEnc;
						asEnc.Add(sResult);
						if(TRUE == p3000Api->ConnectSocket())
						{
							if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), sDBFieldName[i], asEnc))
							{
								sResult = asEnc.GetAt(0);
							}
							asEnc.RemoveAll(); asEnc.FreeExtra();
							p3000Api->CloseSocket();
						}
					}

					ids = pDM->AddDBFieldData( sDBFieldName[i] , sDBFieldType[i] , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
				}
				bSecuMode = TRUE;
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}

	if(FALSE == bSecuMode)
	{
		for( i = 0 ; i < nAliasCnt ; i++ )
		{
			if( sColAlias[i].IsEmpty() ) continue;
			if( sColAlias[i].Compare(_T("그룹") ) == 0 ) continue;

			ids = pDM->GetCellData( sColAlias[i] , nIndex , sResult );
			if( ids < 0 && (ids != -4012 )) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

			if ( sResult.IsEmpty( ) && !bOverWriteNull )
				continue;

			ids = pDM->AddDBFieldData( sDBFieldName[i] , sDBFieldType[i] , sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
		}
	}

	ids = pDM->GetCellData( _T("주민등록번호_CRYPTO") , nIndex , sResult );
	if( ids < 0 && (ids != -4012 )) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
	ids = pDM->AddDBFieldData( _T("CRYPTO_CIVIL_NO") , _T("STRING") , sResult );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("InsertDMToDB") );

	const INT nIndexCnt = 2;
	CString strArrIndex[nIndexCnt][3] =
	{{_T("IDX_USER_NAME"),_T("STRING"),_T("")}, {_T("IDX_HANDPHONE"),_T("STRING"),_T("")}};
	CString strArrIndexDMAlias[nIndexCnt] = {_T("이름"), _T("휴대폰")};

	for ( i=0; i<nIndexCnt; i++)
	{	
		ids = pDM->GetCellData( strArrIndexDMAlias[i], nIndex, sResult);
		if (sResult != _T("") ) 
		{
			if(TRUE == m_pInfo->m_bLibSecuUse)
			{
				CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
				if(NULL != p3000Api)
				{
					if(_T("IDX_HANDPHONE") == strArrIndex[i][0])
					{
						CStringArray asEnc;
						asEnc.Add(sResult);
						if(TRUE == p3000Api->ConnectSocket())
						{
							if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), strArrIndex[i][0], asEnc))
							{
								sResult = asEnc.GetAt(0);
							}
							asEnc.RemoveAll(); asEnc.FreeExtra();
							p3000Api->CloseSocket();
						}
					}
					delete p3000Api;
					p3000Api = NULL;
				}
			}
			ids = pDM->AddDBFieldData( strArrIndex[i][0], strArrIndex[i][1], sResult );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
		}
	}

	CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi( this , pDM );

	pMobileApi->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
	ids = pMobileApi->MakeUpdateFrame( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

	if( m_sEcoMailUseYN.Compare(_T("Y")) == 0 )
	{
		CString sGroupCodes;
		ids = pDM->GetCellData( _T("그룹") , nIndex , sGroupCodes );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

		ids = GroupCodeInsert( sGroupCodes , sRecKey, pMobileApi , pDM);
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );
	}

	ids = pMobileApi->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertDMToDB( CESL_DataMgr *pDM , INT nIndex )") );

	pDM->EndFrame();

	CLocCommonAPI::MobileApiEndLog( pMobileApi );

	delete []sColAlias;
	delete []sDBFieldName;
	delete []sDBFieldType;

	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;
	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	
	UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
 	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;
	UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = 		
		_T("USER_ID,USER_NO,NAME,CIVIL_NO,BIRTHDAY,BIRTHDAY_TYPE,")
		_T("USER_CLASS_CODE,USER_POSITION_CODE,MEMBER_CLASS,REISSUE_CNT,")
		_T("SCHOOL,CLASS,CLASS_NO,GRADE,")
		_T("HANDPHONE,E_MAIL,PASSWORD,")
		_T("H_PHONE,H_ADDR1,H_ZIPCODE,")
		_T("W_PHONE,W_ADDR1,W_ZIPCODE,")
		_T("REMARK,SHELF_LOC_CODE");

	m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);


	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::ViewInputResultMsg()
{
	EFS_BEGIN

	INT ids;
	const INT nDMCnt = 2;
	TCHAR *sDMAlias[nDMCnt] = 
	{
		_T("DM_BO_LOC_3390_SUCCESS") , _T("DM_BO_LOC_3390_TMP")
	};
	CString sFileName[nDMCnt] = 
	{
		_T("Success.txt") , _T("Fail.txt")
	};

	CString sDefaultFileDir = _T("e:\\kolas2up\\log\\member_log\\");
	_tmkdir( _T("..\\log") );
	_tmkdir( _T("..\\log\\member_log") );

	CString strGetData;
	CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);

	CString sCurrentTime = tCurrentTime.Format( _T("[%Y%d%m %H%M%S]") );

	INT nRecordCnt[nDMCnt] = { 0 , 0 };

	CRect rect;
	GetClientRect(&rect);

	INT nAliasCnt = GetColunAliasCount();
	CString *sColAlias;
	sColAlias = new CString[nAliasCnt+1];
	ids = GetColumnAlias( sColAlias , nAliasCnt );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewInputResultMsg") );

	sColAlias[nAliasCnt] = _T("사유");

	CESL_DataMgr *pDM;

	TCHAR sCurrentDirectory[_MAX_PATH];
    _tgetcwd( sCurrentDirectory, _MAX_PATH ) ;

	CString sPath = sCurrentDirectory;
	sPath = sPath.Mid(0,sPath.GetLength()-4) + _T("\\log\\member_log\\");

	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		pDM = FindDM( sDMAlias[i]);
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ViewInputResultMsg") );

		ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1 , _T("주민등록번호") , m_sIsUnityLoanService );
		if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ViewInputResultMsg") );

		nRecordCnt[i] = pDM->GetRowCount();

		if( nRecordCnt[i] < 1 ) continue;

		sFileName[i] = sPath+sCurrentTime+sFileName[i];		
		
		CStdioFile File;
		if ( !File.Open( sFileName[i], CFile::modeWrite | CFile::modeCreate | CFile::typeBinary) )
		{
			AfxMessageBox( _T("로그 파일생성에 실패했습니다.") );
			return -11;
		}

		if( 0 == File.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			File.Write( &cUni, sizeof(TCHAR));
		}
		
		INT nIndex;
		CString sResult;
		CString strLine;

		for( INT j = 0 ; j < nRecordCnt[i] ; j++ )
		{
			nIndex = j;
			sResult.Empty();
			strLine.Empty();
			for( INT k = 0 ; k < nAliasCnt+1 ; k++ )
			{
				sResult.Empty();

				ids = pDM->GetCellData( sColAlias[k] , nIndex , sResult );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ViewInputResultMsg") );
				
				strLine = strLine + sResult + _T("\t");
			}
			
			strLine  = strLine + _T("\r\n");

			File.WriteString( strLine );
		}		
		
		File.Close();

	}

	delete []sColAlias;

	CString sEndMsg;
	INT nTotalRecordCnt = nRecordCnt[0] + nRecordCnt[1];

	if( nTotalRecordCnt < 1 )
	{
		sEndMsg.Format( _T("INSERT된 자료가 존재하지 않습니다.") );
	}
	else
	{
		if( nRecordCnt[0] > 0 )
		{
			if( nRecordCnt[1] > 0 )
				sEndMsg.Format( _T("총 '%d'건이 처리되었습니다.\r\n그 중 '%d'건이 반입되었으며 '%d'건이 반입에 실패하였습니다.")
								_T("\r\n성공한 로그는 '%s'파일에 실패한 로그는 '%s'파일에 저장하였습니다.")
				, nTotalRecordCnt, nRecordCnt[0] , nRecordCnt[1] , sFileName[0] , sFileName[1]  );
			else
				sEndMsg.Format( _T("선택하신 '%d'건이 반입 처리가 되었습니다.") 
								_T("\r\n성공한 로그는 '%s'파일에 저장하였습니다.")
				, nRecordCnt[0] , sFileName[0] );
		}
		else
		{
			if( nRecordCnt[1] > 0 )
				sEndMsg.Format( _T("선택하신 '%d'건이 모두 반입 처리에 실패했습니다.") 
								_T("\r\n실패한 로그는 '%s'파일에 저장하였습니다.")
				, nRecordCnt[1] , sFileName[1] );
		}
	}

	MessageBox( sEndMsg , _T("이용자 반입 처리") , MB_OK|MB_ICONINFORMATION );

	return 0;

	EFS_END
	return -1;

}


INT CBO_LOC_3390::InitLogDM()
{
	EFS_BEGIN

	const INT nDMCnt = 2;
	TCHAR *sDMAlias[nDMCnt] = 
	{
		_T("DM_BO_LOC_3390_SUCCESS") , _T("DM_BO_LOC_3390_TMP")
	};

	for( INT i = 0 ; i < nDMCnt ; i++ )
	{
		CESL_DataMgr *pDM = FindDM( sDMAlias[i] );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewInputResultMsg()") );

		pDM->FreeData();
	}

	return 0;

	EFS_END
	return -1;

}

VOID CBO_LOC_3390::OnbtnDetailInfo() 
{
	EFS_BEGIN

	CBO_LOC_3391 DetailView(this,m_pLoanShareManager);
	DetailView.SetMode( m_nMode , TRUE );
	DetailView.m_sIsUnityLoanService = m_sIsUnityLoanService;
	DetailView.DoModal();

	EFS_END
}

VOID CBO_LOC_3390::OnbtnDelete() 
{
	EFS_BEGIN

	INT ids;

	ids = DeleteSelectRow();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_LOC_3390::OnbtnDelete()") );
	if( ids > 0 ) return ;

	AfxMessageBox(_T("삭제되었습니다."));

	EFS_END
}

INT CBO_LOC_3390::DeleteSelectRow()
{
	EFS_BEGIN

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-2 , _T("CBO_LOC_3390::DeleteSelectRow()") ); 

	CESL_Grid *pGrid = GetClientGridPtr();
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2(-1 , _T("CBO_LOC_3390::DeleteSelectRow()") );

	pGrid->SelectMakeList();
	pGrid->SelectGetTailPosition();

	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt < 1 )
	{
		AfxMessageBox(_T("삭제할 자료를 먼저 선택하십시오") );
		return 1;
	}

	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		pDM->DeleteRow(nIndex);
		pGrid->SelectGetPrev();
	}

	pGrid->Display();

	return 0;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::GetColumnAlias( CString *sColAlias , INT &nAliasCnt )
{
	EFS_BEGIN

	INT ids;

	CString sColumnDMAlias = _T("DM_COLUMN_NAME");
	CESL_DataMgr *pColumnDM = FindDM( sColumnDMAlias );
	if ( pColumnDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::GetColumnAlias()") );

	INT nCount  = 0;
	INT nRowCnt = pColumnDM->GetRowCount();
	CString sGetValue = _T("");

	for ( INT i = 0 ; i < nRowCnt ; i++ )
	{
		ids = pColumnDM->GetCellData( _T("사용YN") , i , sGetValue );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnAlias") );

		if ( sGetValue == _T("Y") )
		{
			ids = pColumnDM->GetCellData( _T("컬럼") , i , sColAlias[nCount] );
			if ( ids < 0 && (ids != -4012)) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("GetColumnAlias") );

			nCount++;
		}
	}

	nAliasCnt = nCount;

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3390::GetColunAliasCount()
{
	EFS_BEGIN

	INT ids;

	CString sColumnDMAlias = _T("DM_COLUMN_NAME");
	CESL_DataMgr *pColumnDM = FindDM( sColumnDMAlias );
	if( pColumnDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::GetColunAliasCount()") );

	INT nCount  = 0;
	INT nRowCnt = pColumnDM->GetRowCount();
	CString sGetValue = _T("");

	for (INT i = 0;i < nRowCnt;i++)
	{
		ids = pColumnDM->GetCellData( _T("사용YN"), i, sGetValue );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::GetColunAliasCount()") );

		if ( sGetValue == _T("Y") )
			nCount++;
	}

	return nCount;

	EFS_END
	return -1;

}

INT CBO_LOC_3390::MakeColumnAliasDM()
{
	EFS_BEGIN

	INT ids;
	
	CString sColumnDMAlias = _T("DM_COLUMN_NAME");
	CESL_DataMgr *pColumnDM = FindDM( sColumnDMAlias );
	if( pColumnDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::GetColumnAlias()") );
	pColumnDM->FreeData();
	
	CStdioFile fFile;
	CString sFilePath = FILE_PATH;
	if ( !fFile.Open( sFilePath , CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary) ) 
	{
        AfxMessageBox( _T("파일명이 잘못되었습니다.\n올바른 경로인지 확인해 주십시오.") );
		return -1;
    }
	
		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}
	
	CString sLine;
	CString sColumnName;
	CStringTokenizer Tokener;
	INT nIndex = 0;
	INT nTokenCount = 0;
    while ( fFile.ReadString( sLine ) ) 
	{		
        sLine.TrimRight();sLine.TrimLeft();
		if( sLine.IsEmpty() )		continue;
		if( sLine.Left(1) == _T("#") )  continue;

		Tokener.ReMake(sLine, _T(":"));

		if ( nTokenCount != 0 )
		{
			if ( nTokenCount != Tokener.GetTokenCnt() )
			{
				AfxMessageBox( _T("반입반출정보 파일 형식이 틀립니다.") );
				fFile.Close();
				return -2;
			}
		}
		else
			nTokenCount = Tokener.GetTokenCnt();
		
		pColumnDM->InsertRow(-1);
		ids = pColumnDM->SetCellData( _T("컬럼")	   , Tokener.GetAt(0) , nIndex );
		if ( ids < 0 ) 
		{
			fFile.Close();
			return -3;
		}	

		ids = pColumnDM->SetCellData( _T("수정YN")     , Tokener.GetAt(1) , nIndex );
		if ( ids < 0 ) 
		{
			fFile.Close();
			return -3;
		}

		ids = pColumnDM->SetCellData( _T("사용YN")     , Tokener.GetAt(2) , nIndex );
		if ( ids < 0 ) 
		{
			fFile.Close();
			return -3;
		}

		nIndex++;
	}

	fFile.Close();

	return 0;

	EFS_END
	return -1;
}

VOID CBO_LOC_3390::OnbSETDMALIAS() 
{
	EFS_BEGIN
	// TODO: Add your control notification handler code here

	CExcelHeaderManager mgr( this );
	mgr.DoModal();

	EFS_END
}

INT CBO_LOC_3390::GetDMAlias( CArray< CString, CString >&sColAlias, CArray< CString, CString >&sDMAlias )
{
	EFS_BEGIN

	CStdioFile fp; 
	fp.Open( FILE_PATH, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( !fp.m_pStream )
	{
		AfxMessageBox( _T("반입반출필드정보.txt 파일을 찾을 수 없습니다.") );
		return -1;
	}
	
		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}
	
	sDMAlias.RemoveAll();
	
	CString sLine, DMAlias, sExcelAlias;
	INT locate = 0;
	INT colAliasCnt = sColAlias.GetSize();
	sDMAlias.SetSize( colAliasCnt );
	CString colAlias;
	while( fp.ReadString( sLine ) )
	{
		if( _T(".") == sLine.Left( 0 ) )
			continue;
		
		locate = sLine.Find( _T(":") );
		DMAlias = sLine.Left( locate );
		sExcelAlias = sLine.Right( sLine.GetLength() - locate - 1 );
		
		for( INT i = 0; i < colAliasCnt; i++ )
		{
			colAlias = sColAlias.GetAt( i );
			
			if( colAlias == sExcelAlias )
			{
				sDMAlias.SetAt( i, DMAlias );
				break;
			}
		}
	}

	return 0;

	EFS_END

	return -1;
}

VOID CBO_LOC_3390::SetCivilNumViewMode()
{
	EFS_BEGIN

	m_civilNumViewMode = TRUE;

	EFS_END
}

INT CBO_LOC_3390::CheckMailingSystem()
{
EFS_BEGIN

	INT ids; 
	
	ids = GetManageValue( _T("기타")  , _T("공통") , _T("메일링시스템") , _T("사용여부") , m_sEcoMailUseYN );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CheckMailingSystem") );

	if( m_sEcoMailUseYN.IsEmpty() ) m_sEcoMailUseYN = _T("N");

	if( m_sEcoMailUseYN.Compare(_T("Y")) == 0 ) return 0;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3390") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CheckMailingSystem") );

	INT nColumnIndex;

	nColumnIndex = pDM->FindColumn(_T("그룹"));
	if( nColumnIndex < 0 ) return 1;
	pDM->SetDataMgrRef( nColumnIndex , _T("그룹") , _T("GET_LIST_USER_GROUP_INFO(REC_KEY)") , _T("str") , _T("") , _T("udf") );

	pDM = FindDM( _T("DM_BO_LOC_3390_SUCCESS") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CheckMailingSystem") );

	nColumnIndex = pDM->FindColumn(_T("그룹"));
	if( nColumnIndex < 0 ) return 1;
	pDM->SetDataMgrRef( nColumnIndex , _T("그룹") , _T("GET_LIST_USER_GROUP_INFO(REC_KEY)") , _T("str") , _T("") , _T("udf") );

	pDM = FindDM( _T("DM_BO_LOC_3390_TMP") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CheckMailingSystem") );

	nColumnIndex = pDM->FindColumn(_T("그룹"));
	if( nColumnIndex < 0 ) return 1;
	pDM->SetDataMgrRef( nColumnIndex , _T("그룹") , _T("GET_LIST_USER_GROUP_INFO(REC_KEY)") , _T("str") , _T("") , _T("udf") );

	return 0;

EFS_END

return -1;
}

INT CBO_LOC_3390::GroupCodeInsert( CString sGroupCodes , CString sUserKey , CKolas2up_MobileApi* pMoblieApi , CESL_DataMgr* pDM )
{
EFS_BEGIN

	INT ids;

	sGroupCodes.TrimLeft(); sGroupCodes.TrimRight();
	sGroupCodes.Replace(_T("'") , _T("") );
	if( sGroupCodes.IsEmpty() ) return 0;

	CStringArray asGroupCodes;
	CLocCommonAPI::MakeArrayList( sGroupCodes , _T(",") , asGroupCodes );

	CString sDeleteQuery;
	CString sGroupCode;

	CString sRecKey;
	for( INT i = 0 ; i < asGroupCodes.GetSize() ; i++ )
	{
		sGroupCode = asGroupCodes.GetAt(i);

		sGroupCode.TrimLeft(); sGroupCode.TrimRight();
		if( sGroupCode.IsEmpty() ) continue;

		if( sGroupCode.GetLength() == 2 ) sGroupCode = _T("0") + sGroupCode;
		if( sGroupCode.GetLength() == 1 ) sGroupCode = _T("00") + sGroupCode;

		pDM->InitDBFieldData();
		sDeleteQuery.Format( _T("DELETE FROM MN_LOAN_USER_GROUP_LIST_TBL WHERE USER_KEY=%s AND GROUP_CODE='%s';") , sUserKey , sGroupCode );
		pMoblieApi->AddFrame( sDeleteQuery );

		ids = pDM->MakeRecKey( sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GroupCodeInsert") );

		pDM->InitDBFieldData();
		pDM->AddDBFieldData( _T("GROUP_CODE") , _T("STRING") , sGroupCode );
		pDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sUserKey  );
		ids = pMoblieApi->MakeInsertFrame( _T("MN_LOAN_USER_GROUP_LIST_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("GroupCodeInsert") );
	}


	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3390::OnchkOVERWRITE() 
{
	if ( ((CButton*)GetDlgItem(IDC_chkOVERWRITE))->GetCheck( ) )
		GetDlgItem(IDC_chkOVERWRITE_NULL)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_chkOVERWRITE_NULL)->EnableWindow(FALSE);	
}

HBRUSH CBO_LOC_3390::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CBO_LOC_3390::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	return CESL_Mgr::PreTranslateMessage(pMsg);

EFS_END
return FALSE;
}

INT CBO_LOC_3390::MakeDMToHCellFile()
{
EFS_BEGIN

	INT ids;
	
	ids = GetControlData( _T("CM_BO_LOC_3390_INPUT") , _T("파일이름") , m_sFileName );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MakeDMToExcelFile") ); 
	}

	if( m_sFileName.IsEmpty() ) 
	{
		OnbtnFileSelect();
		if( m_sFileName.IsEmpty() )
		{
			return 1;
		}
	}

	CESL_Grid *pGrid = GetClientGridPtr();
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeDMToExcelFile") ); 
	}

	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();
	if( 1 > nRowCount ) 
	{
		AfxMessageBox(_T("반출할 이용자를 먼저 선택해 주십시오"));
		return 1;
	}

	INT nAliasCnt = GetColunAliasCount();
	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];

	ids = GetColumnAlias( sColAlias , nAliasCnt );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("MakeDMToExcelFile") );
	}

	CRect rc;
	GetClientRect(&rc);	
	
	CoInitialize(NULL);
	CExportToHCell *HCellMgr;
	HCellMgr = new CExportToHCell;				

	ids = HCellMgr->InitExcel();
	if( ids ) 
	{
		HCellMgr->Release();
		delete HCellMgr;
		HCellMgr = NULL;		
		return -1;
	}

	HCellMgr->Open();
	HCellMgr->InitArray( nRowCount, nAliasCnt );
	
	HCellMgr->SetFont(1, 1, 40, 1, TRUE, FALSE, RGB(0, 0, 0), 11);

	HCellMgr->SetHorizAlign(1, 1, 40, 1, -4108);

	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		HCellMgr->SetTextToHCell( 1, i+1, sColAlias[i] );
	}

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("MakeDMToExcelFile") );
	}

	if(PrivacyPrintLogRecord(pDM, _T("이용자 관리 파일반출"), _T("H") ,0 ) < 0 ) return 2;

	INT nIndex = 0;	
	CString sGetValue = _T("");
	pGrid->SelectGetHeadPosition();
	
	for( i=0 ; i<nRowCount ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		for( INT j=0 ; j<nAliasCnt ; j++ )
		{
			ids = pDM->GetCellData( sColAlias[j] , nIndex , sGetValue );
			if( 0 > ids )
			{
				CString sEMsg = _T("");
				sEMsg.Format( _T("GetCellData( %s , %d , sResult)") , sColAlias[j] , nIndex );
				AfxMessageBox( sEMsg );
			}
			
			if( _T("Y")==m_sIsUnityLoanService && _T("주민등록번호")==sColAlias[j] )
			{
				CString strCivilNo;
				ids = pDM->GetCellData( _T("주민등록번호_CRYPTO") , nIndex , strCivilNo );
				this->ESL_DES_Decryption(strCivilNo, sGetValue);
			}

			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("MakeDMToExcelFile") );
			}

			if( _T("관리구분") == sColAlias[j] )
			{
				ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분") , sGetValue , sGetValue  );
				if( 0 > ids && -2004 != ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("DMToDB") );
				}
			}
		
			sGetValue.Replace(_T("\r"), NULL);
			HCellMgr->FillSafeArray( i, j, sGetValue );
		}			
		pGrid->SelectGetNext();
	}

	HCellMgr->PutAllbyArray( 2, 1, nRowCount, nAliasCnt );
	HCellMgr->Save(m_sFileName);

	HCellMgr->Close();
	delete HCellMgr;
	HCellMgr = NULL;

	CString sMsg;
	sMsg.Format( _T("'%s' 파일이 생성되었습니다.") , m_sFileName );
	AfxMessageBox( sMsg );

	delete []sColAlias;

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3390::HCellFileToDM( CString sFileName )
{
	EFS_BEGIN

	CRect rc;
	m_pParentWnd->GetWindowRect(rc);
	
	CoInitialize(NULL);
	CExportToHCell *HCellMgr;
	HCellMgr = new CExportToHCell;
	
	INT ids = HCellMgr->InitExcel();
	if( ids ) 
	{
		HCellMgr->Release();
		delete HCellMgr;
		HCellMgr = NULL;		
		return -1;
	}

	if (HCellMgr->Open(sFileName) != 0)
	{
		AfxMessageBox( _T("Open Failed") );
		HCellMgr->Close();
		delete HCellMgr;
		HCellMgr = NULL;
		return 3;
	}
		
	CArray <CString,CString> 	sColAlias;
	sColAlias.RemoveAll();

	INT nColCount = 0;
	CString strAlias = _T("");

	while (TRUE)
	{
		strAlias = HCellMgr->GetValue(nColCount + 1, 1);
		if (strAlias.IsEmpty()) break;

		sColAlias.Add(strAlias);
		nColCount++;
	}

	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::FileToDM( CString sFileName )") ); 
	pDM->FreeData();

	CString sValue = _T("");
	BOOL bStop     = FALSE;
	BOOL bRowEmpty = TRUE;
	INT nRow = 0;

	while(TRUE)
	{	
		bRowEmpty = TRUE;
		pDM->InsertRow( -1 );
		nRow = pDM->GetRowCount();

		for( INT i = 0 ; i < nColCount ; i++ )
		{
			sValue = HCellMgr->GetValue(i + 1, nRow + 1);

			if( _tcscmp(sValue, _T("#_TEOF")) == 0 )
			{
				bStop = TRUE;
				i = nColCount;
			}

			if( !bStop )
			{
				if( !sValue.IsEmpty() )
					bRowEmpty = FALSE;

				sValue.Replace( _T("\n") , _T("\r\n") );

				if(_T("Y")==m_sIsUnityLoanService)
				{
					if(_T("주민등록번호")==sColAlias.GetAt(i))
					{
						CString strCivilNo;
						this->ESL_DES_Encryption(sValue,strCivilNo);
						CLocCommonAPI::ChangeSecuriyCivilNo(this,sValue,m_sIsUnityLoanService);
						pDM->SetCellData(_T("주민등록번호_CRYPTO") , strCivilNo , nRow - 1);
					}
				}

				pDM->SetCellData(sColAlias.GetAt(i) , sValue , nRow - 1);
			}
		}

		if( bRowEmpty )	break;
		if( bStop )		break;
	}
	
	pDM->DeleteRow( nRow - 1 );
	HCellMgr->Close();
	delete HCellMgr;
	HCellMgr = NULL;
	return 0;

	EFS_END
	return -1;
}
INT CBO_LOC_3390::CSVFileToDM( CString sFileName )
{
	EFS_BEGIN

		
	CExportToCSV *CSVMgr;
	CSVMgr = new CExportToCSV;	

	BOOL bResult = FALSE;

	bResult = CSVMgr->Open(sFileName);
	if(bResult == FALSE)
	{
		AfxMessageBox(_T("파일이 정상적으로 열리지 않았습니다."));
		return -1;
	}
	
	INT nColCount = 0, nRowCount = 0;
	CString strAlias = _T("");

	nColCount = CSVMgr->GetColCount();
	nRowCount = CSVMgr->GetRowCount();

	CStringArray sColAlias;
	CSVMgr->GetAlias(sColAlias);
 
	CString sDMAlias = _T("DM_BO_LOC_3390");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::FileToDM( CString sFileName )") ); 
	pDM->FreeData();

	CString sValue = _T("");
	BOOL bStop     = FALSE;		// 루프 탈출 
	BOOL bRowEmpty = TRUE;	    // 행전체가 비어 있는지 검사
	INT nRow = 0;

	INT i = 0, j = 0;

	for(i = 0; i < nRowCount; i++)
	{
		pDM->InsertRow(-1);
		for(j = 0;j < nColCount; j++)
		{
			CSVMgr->GetCellData(i, j, sValue);
			if( _tcscmp(sValue, _T("#_TEOF")) == 0 )
			{
				bStop = TRUE;
				i = nColCount;
			}

			if( !bStop )
			{
				if( !sValue.IsEmpty() )
					bRowEmpty = FALSE;

				sValue.Replace( _T("\n") , _T("\r\n") );

				if(_T("Y")==m_sIsUnityLoanService)
				{
					if(_T("주민등록번호")==sColAlias.GetAt(i))
					{
						CString strCivilNo;
						this->ESL_DES_Encryption(sValue,strCivilNo);
						CLocCommonAPI::ChangeSecuriyCivilNo(this,sValue,m_sIsUnityLoanService);
						pDM->SetCellData(_T("주민등록번호_CRYPTO") , strCivilNo , i);
					}
				}

				pDM->SetCellData(sColAlias.GetAt(j) , sValue , i);
			}
		}
	}

	return 0;

	EFS_END
	return -1;
}