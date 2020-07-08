#include "stdafx.h"
#include "BO_LOC_31001.h"
#include "BO_LOC_3190.h"
#include "BO_LOC_3191.h"
#include "BO_LOC_3192.h"
#include "BO_LOC_3193.h"
#include "BO_LOC_3195.h"
#include "Imm.h"
#include "..\\..\\..\\..\\SMS\SMS_06\\SMS_061.h"
#include "..\\..\\..\\공통\\paintlib\\common\\Filter\\plfilterresizebilinear.h"
#include "CharConvert.h"
#include "..\\BL_LOC_LOAN\\SMSPreviewDlg.h"
#include "..\\BL_LOC_LOAN\\LibListDlg.h"
#include "..\\BL_LOC_LOAN\\GroupLibListDlg.h"
#include "..\\BL_LOC_LOAN\\KlUserSwitchDlg.h"
#include "..\\BL_LOC_LOAN\\KlUserSyncDlg.h"
#include "..\\BL_LOC_USER\\KL_SearchCurrentLoanInfoDlg.h"
#include "..\\BL_LOC_LOAN\\KL_UserImportSetDlg.h"
#include "..\\BL_LOC_LOAN\\KL_UserSearchDlg.h"
#include "..\\BO_LOC_ARREAR\\ArrearMainDlg.h"
#include "RegistryEx.h"
#include "..\\BL_LOC_USER\\UserCertifyDlg.h"
#include "..\\BL_LOC_USER\\KL_StatementDlg.h"
#include "..\\BL_LOC_LOAN\\KlCertifyDlg.h"
#include "StdioFileEx.h"
#include "..\\BL_LOC_USER\\CertifyMainDlg.h"
#include "..\\RFID_JOB\\RFID_Interface.h"
#include "..\\RFID_JOB\\RfidTagView.h"

#include "PersonInfo.h"

#include "LoanInfoUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_strIpinFlag;
CString g_strCivilNoFlag;
CString g_strMypinCertifyFlag;
CString g_strPhoneCertifyFlag;
CString g_strPrevRegNoFromOther;

#define TIMER_API_LRC_MONITOR	WM_USER+2048

#define __EXP_DATA_IS_CHECK_FILE _T("..\\cfg\\처리할자료체크.txt")

CBO_LOC_3100::CBO_LOC_3100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{	
	m_pLoan = NULL;		
	m_pFeeArrearReceipt		= NULL;
	m_pPreRegNoMgr			= NULL;
	m_pMessageBox			= NULL;
	m_pNote					= NULL;
	m_pUnionUserInfoViewer	= NULL;
	m_UseCenterUserInfo		= FALSE;
	m_UseCenterUserLoanLog	= FALSE;
	m_bVisibleBasket		= FALSE;
	m_bMenu					= FALSE;
	m_nBarcodeMode		=	0;
	m_nMouseDownRow		=	-1;
	m_nMouseUpRow		=	-1;	
	m_nLoanStopDateMode = 0;
	
	m_BarcodeFlowSettingDlg.pParentMgr = this;
	m_nCurSel = 0;
	
	m_userInfo_Simple	= NULL;
	m_userInfo_Addition = NULL;

	m_pBmp			= NULL;
	m_bShowPic		= TRUE;
	m_bInitSplitter	= FALSE;

	m_bIsClear = FALSE;
	
	m_nCertifyState = 0;
	
	m_b3100ControlLoadYN = FALSE;
}

CBO_LOC_3100::~CBO_LOC_3100()
{
EFS_BEGIN	
	if(_T("Y") == m_pLoanMangeValue->m_str3100ControlYN) {
		UnLoad3100ControlProcess();
	}	
	m_pLoan->m_pManageValue->m_sUseReceipt = _T("N");

	m_pLoan->SPFExcutePath( _T("영수증출력체크") );	

	
	CWinApp *pApp = AfxGetApp();	
	if ( m_pLoan->m_bBeep == TRUE ) pApp->WriteProfileString(_T("Settings"), _T("LoanBeep"),_T("TRUE"));	
	else pApp->WriteProfileString(_T("Settings"), _T("LoanBeep"),_T("FALSE"));
	if ( m_pLoan->m_bSimpleGrid == TRUE ) pApp->WriteProfileString(_T("Settings"), _T("LoanSimpleGrid"),_T("TRUE"));
	else pApp->WriteProfileString(_T("Settings"), _T("LoanSimpleGrid"),_T("FALSE"));		
	
	if ( m_pLoan != NULL ) delete m_pLoan;
	if ( m_pLoanShareManager != NULL ) delete m_pLoanShareManager;	
	if ( m_pLoanMangeValue != NULL ) delete m_pLoanMangeValue;
	if ( m_pMessageBox != NULL ) delete m_pMessageBox;
	if ( m_pUserSimpleDetailInfo != NULL ) delete m_pUserSimpleDetailInfo;
	if ( m_pNote != NULL ) delete m_pNote;
	if ( m_pPreRegNoMgr != NULL ) delete m_pPreRegNoMgr;
	if( m_pUnionUserInfoViewer )
	{
		delete m_pUnionUserInfoViewer;
		m_pUnionUserInfoViewer = NULL;
	}
		
	if( m_pFeeArrearReceipt )
	{
		delete m_pFeeArrearReceipt;
		m_pFeeArrearReceipt = NULL;
	}
			
	CString strLogMsg = _T("");
	if( pMain->GetSafeHwnd() )
	{
		if( pMain->GetParentFrame()->GetSafeHwnd() )
		{
			pMain->GetParentFrame()->SendMessage(WM_USER + 2004, NULL, (long)strLogMsg.GetBuffer(0));
		}
	}
	
	if( NULL !=	m_userInfo_Simple )
	{
		delete m_userInfo_Simple;
		m_userInfo_Simple = NULL;
	}
	
	if( NULL !=	m_userInfo_Addition )
	{
		delete m_userInfo_Addition;
		m_userInfo_Addition = NULL;
	}
	
	if( m_pBmp != NULL )
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}

EFS_END
}


VOID CBO_LOC_3100::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3100)
	DDX_Control(pDX, IDC_sta3100Control, m_stc3100Control);
	DDX_Control(pDX, IDC_btn3100Certify_Y, m_btnCertify_Y);
	DDX_Control(pDX, IDC_btn3100Certify_N, m_btnCertify_N);
	DDX_Control(pDX, IDC_btn3100Certify_Fail, m_btnCertify_Fail);
	DDX_Control(pDX, IDC_TAB_USER_INFO, m_tabUserInfo);
	DDX_Control(pDX, IDC_BTN_PREV_USER_SEARCH, m_btnPrevUserSearch);
	DDX_Control(pDX, IDC_btnBARCODE_NO_TYPE, m_btnBARCODE_NO_TYPE);	
	DDX_Control(pDX, IDC_btnFamily, m_btnFamily);
	DDX_Text(pDX, IDC_EDIT_USER_BASKET_MAX_COUNT, m_nUserBasketCnt);
	DDX_Control(pDX, IDC_btnUSE_RFID, m_btnRFIDUse);
	DDX_Control(pDX, IDC_btnRFID_RECORD, m_btnRFIDCardTagging);
	DDX_Control(pDX, IDC_btnUSE_ViewTag, m_btnRFIDView);
	DDX_Control(pDX, IDC_btnCheckRFID, m_btnRFIDConnectTest);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3100, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3100)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnFamily, OnbtnFamily)
	ON_BN_CLICKED(IDC_btnNUMBERFIND, OnbtnNUMBERFIND)
	ON_BN_CLICKED(IDC_radLOANTYPE1, OnradLOANTYPE1)
	ON_BN_CLICKED(IDC_radLOANTYPE2, OnradLOANTYPE2)
	ON_BN_CLICKED(IDC_radLOANTYPE3, OnradLOANTYPE3)
	ON_BN_CLICKED(IDC_radRETURNTYPE1, OnradRETURNTYPE1)
	ON_BN_CLICKED(IDC_radRETURNTYPE2, OnradRETURNTYPE2)
	ON_BN_CLICKED(IDC_radRETURNTYPE3, OnradRETURNTYPE3)
	ON_BN_CLICKED(IDC_btnDETAILINFO, OnbtnDETAILINFO)
	ON_BN_CLICKED(IDC_btnNOTEMODIFY, OnbtnNOTEMODIFY)
	ON_BN_CLICKED(IDC_btnNUMBERFIND2, OnbtnNUMBERFIND2)
	ON_BN_CLICKED(IDC_btnTEST, OnbtnTEST)
	ON_WM_TIMER()
	ON_EN_SETFOCUS(IDC_edtNUMBER, OnSetfocusedtNUMBER)
	ON_EN_SETFOCUS(IDC_edtNAME, OnSetfocusedtNAME)
	ON_CBN_SELCHANGE(IDC_cmbLOANTYPE, OnSelchangecmbLOANTYPE)
	ON_CBN_SELCHANGE(IDC_cmbRETURN_TYPE, OnSelchangecmbRETURNTYPE)
	ON_EN_KILLFOCUS(IDC_datEDIT_RETURN_DATE, OnKillfocusdatEDITRETURNDATE)
	ON_BN_CLICKED(IDC_btnSHOWMESSAGE, OnbtnSHOWMESSAGE)
	ON_BN_CLICKED(IDC_btnUSER_SIMPLE_DETAIL, OnbtnUSERSIMPLEDETAIL)
	ON_BN_CLICKED(IDC_btnMSGTEST, OnbtnMSGTEST)
	ON_BN_CLICKED(IDC_bESC, OnbESC)
	ON_COMMAND(ID_MENUITEM32771, OnMenuitem32771)
	ON_COMMAND(ID_MENUITEM32772, OnMenuitem32772)
	ON_BN_CLICKED(IDC_chkEXP_DATA, OnchkEXPDATA)
	ON_BN_CLICKED(IDC_cSetLoanStopDateMode, OncSetLoanStopDateMode)
	ON_BN_CLICKED(IDC_chkCHECK_RETURN, OncSetReturnCheckMode)
	ON_BN_CLICKED(IDC_btnSHELF_LOC_CODE_SET, OnbtnSHELFLOCCODESET)
	ON_EN_SETFOCUS(IDC_edtNOTE, OnSetfocusedtNOTE)
	ON_BN_CLICKED(IDC_bTEST_LOAN, OnbTESTLOAN)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnUserBasketCLEAR)
	ON_BN_CLICKED(IDC_BTN_SHOW_PREV_USER_LIST, OnBtnShowPrevUserList)
	ON_BN_CLICKED(IDC_BTN_PREV_USER_SEARCH, OnBtnPrevUserSearch)
	ON_EN_CHANGE(IDC_edtFAMILYID, OnChangeedtFAMILYID)
	ON_EN_CHANGE(IDC_edtNOTE, OnChangeedtNOTE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_31001, OnButton1)
	ON_EN_CHANGE(IDC_edtNAME, OnChangeedtNAME)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_btnLOAN_STOP_DATE_CANCEL, OnbtnLoanStopDateCancel)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_USER_INFO, OnSelchangeTabUserInfo)
	ON_BN_CLICKED(IDC_BUTTON_USER_BASKET_MAX_COUNT, OnButtonUserBasketMaxCount)
	ON_BN_CLICKED(IDC_chkBEEP, OnchkBEEP)
	ON_BN_CLICKED(IDC_chkSimpleGrid, OnchkSimpleGrid)
	ON_BN_CLICKED(IDC_btnTESTLoan, OnbtnTESTLoan)
	ON_BN_CLICKED(IDC_btnTESTSearchUser, OnbtnTESTSearchUser)
	ON_BN_CLICKED(IDC_BTN_LIBCODE, OnBtnLibcode)
	ON_BN_CLICKED(IDC_rad3100ACenter, OnRad3100ACenter)
	ON_BN_CLICKED(IDC_rad3100TCenter, OnRad3100TCenter)
	ON_BN_CLICKED(IDC_rad3100Local, OnRad3100Local)
	ON_BN_CLICKED(IDC_btnARREARINFO, OnbtnARREARINFO)
	ON_BN_CLICKED(IDC_btn3100Certify_Y, Onbtn3100CertifyY)
	ON_BN_CLICKED(IDC_btn3100Certify_N, Onbtn3100CertifyN)
	ON_BN_CLICKED(IDC_btn3100Certify_Fail, Onbtn3100CertifyFail)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_btnSTATEMENT, OnbtnSTATEMENT)
	ON_BN_CLICKED(IDC_btn3100_CERTIFY, Onbtn3100CERTIFY)
	ON_BN_CLICKED(IDC_rad3100SetControl, Onrad3100SetControl) 
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_rad3100ClearControl, Onrad3100ClearControl) 
	ON_BN_CLICKED(IDC_btnUSE_RFID, OnbtnUSERFID) 
	ON_BN_CLICKED(IDC_btnUSE_ViewTag, OnbtnUSEViewTag) 
	ON_BN_CLICKED(IDC_btnRFID_RECORD, OnbtnRFIDRecord) 
	ON_BN_CLICKED(IDC_btnCheckRFID, OnbtnRFIDCheck) 
	//}}AFX_MSG_MAP	
	ON_MESSAGE(WM_USER_REFRESH_GRID, OnRefreshGrid)	
	ON_MESSAGE(WM_REFRESH_GRID_FROM_LOAN_HIS,OnRefreshGridFromLoanHIS)
	ON_MESSAGE(WM_INSERT_MESSAGE_BOX, OnRefreshMessageBox )	
	ON_MESSAGE(WM_USER_LOAD, OnUserLoaded )	
	ON_MESSAGE(WM_USER_INIT, OnUserInit)
	ON_MESSAGE(WM_SEARCH_USER_FROM_KEY, OnSearchUserFromKey)
	ON_MESSAGE(WM_RECEIPT_CHANGE_TAB, ReceiptChangeTab)
	ON_MESSAGE(WM_UNITY_USER_ICON_SHOW, OnUnityUserIcon)
	ON_COMMAND_RANGE(5000, 5500, OnRegCodeSelected )	
	ON_MESSAGE( WM_USER + 1028 , OnSearchUser )		
	ON_MESSAGE(WM_CERTIFY_ICON_SHOW, OnCertifyIcon)	
	ON_MESSAGE(WM_AGREEMENT_BTN_SHOW , OnStatementButton )		
	ON_MESSAGE(WM_USER_INFO_RENEWAL , UserInfoRenewal )	
	ON_MESSAGE(WM_USER+3100, OnLoanReturnControlService)
END_MESSAGE_MAP()


BOOL CBO_LOC_3100::Create(CWnd* pParentWnd) 
{
EFS_BEGIN	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;
}


INT CBO_LOC_3100::RFIDBtnScreenControl()
{
EFS_BEGIN
	m_btnRFIDUse.LoadBitmaps(IDB_BITMAP_RFID_USE_ON,IDB_BITMAP_RFID_USE_ON,IDB_BITMAP_RFID_USE_OFF,IDB_BITMAP_RFID_USE_OFF);
	m_btnRFIDCardTagging.LoadBitmaps(IDB_BITMAP_RFID_TAGGING_ON,IDB_BITMAP_RFID_TAGGING_ON,IDB_BITMAP_RFID_TAGGING_OFF,IDB_BITMAP_RFID_TAGGING_OFF);
	m_btnRFIDView.LoadBitmaps(IDB_BITMAP_RFID_VIEW_ON,IDB_BITMAP_RFID_VIEW_ON,IDB_BITMAP_RFID_VIEW_OFF,IDB_BITMAP_RFID_VIEW_OFF);
	m_btnRFIDConnectTest.LoadBitmaps(IDB_BITMAP_RFID_CONNECT_TEST,IDB_BITMAP_RFID_CONNECT_TEST,IDB_BITMAP_RFID_CONNECT_TEST,IDB_BITMAP_RFID_CONNECT_TEST);

	m_btnRFIDUse.LoadBitmaps(IDB_BITMAP_RFID_USE_ON,IDB_BITMAP_RFID_USE_OFF,IDB_BITMAP_RFID_USE_ON,IDB_BITMAP_RFID_USE_OFF);
	m_btnRFIDCardTagging.LoadBitmaps(IDB_BITMAP_RFID_TAGGING_ON,IDB_BITMAP_RFID_TAGGING_OFF,IDB_BITMAP_RFID_TAGGING_ON,IDB_BITMAP_RFID_TAGGING_OFF);
	m_btnRFIDView.LoadBitmaps(IDB_BITMAP_RFID_VIEW_ON,IDB_BITMAP_RFID_VIEW_OFF,IDB_BITMAP_RFID_VIEW_ON,IDB_BITMAP_RFID_VIEW_OFF);
	m_btnRFIDConnectTest.LoadBitmaps(IDB_BITMAP_RFID_CONNECT_TEST,IDB_BITMAP_RFID_CONNECT_TEST,IDB_BITMAP_RFID_CONNECT_TEST,IDB_BITMAP_RFID_CONNECT_TEST);
	return TRUE;
EFS_END
return FALSE;
}

BOOL CBO_LOC_3100::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();	
	
	EnableThemeDialogTexture(GetSafeHwnd());
		
	if(InitESL_Mgr(_T("BO_LOC_3100")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pLoanMangeValue = new CLonaManageValue(this);
	m_pLoanMangeValue->pMain = pMain;
	m_pLoanMangeValue->pParentMgr = (CESL_Mgr*)this;
	m_pLoanMangeValue->SM_STYLE = 'T';
	m_pLoanMangeValue->m_pInfo = this->m_pInfo;
	m_pLoanMangeValue->DoModal();	
	m_pLoanShareManager = new CLoanShareManager(this);
	m_pLoanShareManager->Create( this );

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid") );
	if(pGrid==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("OnInitDialog") );
	
	pGrid->m_fpEventHandler = ( INT(CWnd::*)(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)) GridDisplayEventProc;	
	
	CFileException ex;
	CStdioFile fFile;
	if( fFile.Open( __EXP_DATA_IS_CHECK_FILE, CFile::modeRead|CFile::typeBinary, &ex ) )
	{
		TCHAR cUni;
		fFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fFile.SeekToBegin();
		}
		
		CString sIsCheck;
		fFile.ReadString(sIsCheck);
		fFile.Close();

		((CButton*)GetDlgItem(IDC_chkEXP_DATA))->SetCheck(_ttoi(sIsCheck));
	}

	SetCheckExpDataOption();
	
	short	shBtnColor = 30;
	
	m_btnBARCODE_NO_TYPE.SetColor(2, RGB(247,247,244), TRUE);
	m_btnBARCODE_NO_TYPE.SetColor(3, RGB(0,0,0), TRUE);
	m_btnBARCODE_NO_TYPE.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnBARCODE_NO_TYPE.SetMenu(IDR_BARCODE_MENU, m_hWnd);
	m_btnBARCODE_NO_TYPE.SetMenuCallback(m_hWnd, WM_USER + 100);
	m_btnBARCODE_NO_TYPE.SetWindowText( _T("번호") );
	m_btnBARCODE_NO_TYPE.SetAlignText( DT_RIGHT );
	
	m_btnPrevUserSearch.SetColor(2, RGB(247,247,244), TRUE);
	m_btnPrevUserSearch.SetColor(3, RGB(0,0,0), TRUE);
	m_btnPrevUserSearch.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnPrevUserSearch.SetWindowText( _T("이전대출자") );
	m_btnPrevUserSearch.SetAlignText( DT_RIGHT );
	
	m_FamilyInfoViewerControl.InitViewer( IDC_GRID_SIMPLE_FAMILY_VIEW , this );
	m_FamilyInfoViewerControl.SetViewerWidth( 0 , 100 );
	m_FamilyInfoViewerControl.Clear();	
	m_FamilyInfoViewerControl.Display();
	
	m_btnFamily.SetColor(2, RGB(247,247,244), TRUE);
	m_btnFamily.SetColor(3, RGB(0,0,0), TRUE);
	m_btnFamily.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_btnFamily.SetWindowText( _T("  가족 정보") );
	m_btnFamily.SetAlignText( DT_LEFT );

	((CButton*)GetDlgItem(IDC_chkSET_HOLD))->SetCheck(FALSE);
	
	if ( Check5163System() == TRUE ) 
	{
		( (CButton*) GetDlgItem (IDC_btnSHELF_LOC_CODE_SET) )->ShowWindow(SW_SHOW);
		( (CButton*) GetDlgItem (IDC_btnSHELF_LOC_CODE_SET) )->EnableWindow(TRUE);
	}
	
	RFIDBtnScreenControl();
	
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_btnNUMBERFIND);
	if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		CRect rRect = CRect(0, 0, 0, 0);
		pWnd->GetWindowRect(rRect);
		ScreenToClient(rRect);
		rRect.top = rRect.top + 1;
		pWnd->MoveWindow(&rRect);
		pWnd = (CWnd*)GetDlgItem(IDC_BTN_LIBCODE);
		pWnd->GetWindowRect(rRect);
		ScreenToClient(rRect);
		rRect.top = rRect.top + 1;
		pWnd->MoveWindow(&rRect);
		pWnd = (CWnd*)GetDlgItem(IDC_BTN_SHOW_PREV_USER_LIST);
		pWnd->GetWindowRect(rRect);
		ScreenToClient(rRect);
		rRect.top = rRect.top + 1;
		pWnd->MoveWindow(&rRect);
	}

	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호사용유무"), g_strCivilNoFlag);
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("아이핀사용여부"), g_strIpinFlag);
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("MYPIN사용여부"), g_strMypinCertifyFlag);
	
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("휴대폰인증사용여부"), g_strPhoneCertifyFlag);
	m_pLoanMangeValue->GetManageValue(_T("기타"), _T("공통"), _T("타관이전등록구분"), _T("타관이전등록구분사용여부"), g_strPrevRegNoFromOther);			

	
	if( ( _T("Y") == m_pLoanMangeValue->m_sIsUnityLoanService || 0 == g_strPrevRegNoFromOther.Compare(_T("Y")) )
		&& 10000 != m_pInfo->MODE && 30000 != m_pInfo->MODE)
	{
		
		pWnd = (CWnd*)GetDlgItem(IDC_EDT_PREV_USER_NAME);
		if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
		{
			INT nMove = 21;
			CRect rRect = CRect(0, 0, 0, 0);
			pWnd->GetWindowRect(rRect);
			ScreenToClient(rRect);
			rRect.top = rRect.top + nMove;
			rRect.bottom = rRect.bottom + nMove;
			pWnd->MoveWindow(&rRect);
			pWnd = (CWnd*)GetDlgItem(IDC_BTN_SHOW_PREV_USER_LIST);
			pWnd->GetWindowRect(rRect);
			ScreenToClient(rRect);
			rRect.top = rRect.top + nMove;
			rRect.bottom = rRect.bottom + nMove;
			pWnd->MoveWindow(&rRect);
			pWnd = (CWnd*)GetDlgItem(IDC_BTN_PREV_USER_SEARCH);
			pWnd->GetWindowRect(rRect);
			ScreenToClient(rRect);
			rRect.top = rRect.top + nMove;
			rRect.bottom = rRect.bottom + nMove;
			pWnd->MoveWindow(&rRect);
		}
		
		( (CButton*) GetDlgItem (IDC_STATIC_LIBCODE) )->ShowWindow(SW_SHOW);
		( (CButton*) GetDlgItem (IDC_EDT_LIBCODE) )->ShowWindow(SW_SHOW);
		( (CButton*) GetDlgItem (IDC_BTN_LIBCODE) )->ShowWindow(SW_SHOW);
		
		GetDlgItem(IDC_chk3100_OtherReturn)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_stc3100_OtherReturn)->ShowWindow(SW_SHOW);
				
		if (g_strCivilNoFlag.CompareNoCase(_T("Y")) != 0)
		{
			if (g_strIpinFlag.CompareNoCase(_T("Y")) != 0 && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0) 
			{
				if (m_pInfo->m_bCertifyUse == FALSE)
				{					
					g_strCivilNoFlag = _T("Y");
				}
			}
		}

		CString strReCertifyYN = _T("N");
		m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("본인인증조회사용여부"), strReCertifyYN);
		if (g_strCivilNoFlag.CompareNoCase(_T("Y")) == 0 || strReCertifyYN.CompareNoCase(_T("Y")) == 0 )
		{
			GetDlgItem(IDC_btn3100_CERTIFY)->ShowWindow(SW_SHOW);
			m_certifyToolTip.Create(this);
			m_certifyToolTip.SetMaxTipWidth(400);
			m_certifyToolTip.AddTool(GetDlgItem(IDC_btn3100_CERTIFY), _T("회원증 미지참자의 신분 확인을 위하여\r\n본인인증을 수행합니다."));
			m_certifyToolTip.Activate(TRUE);
			m_certifyToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_INITIAL,MAKELONG(800,0));
			m_certifyToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_AUTOPOP,MAKELONG(10000,0));
		}
	}
	else
	{
		GetDlgItem(IDC_rad3100ACenter)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_rad3100TCenter)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_btn3100_CERTIFY)->ShowWindow(SW_HIDE);
	}	
	
	CFile fDebug;
	if( fDebug.Open(_T("..\\cfg\\_debug"), CFile::modeRead | CFile::typeBinary ) == 0 )
	{
		GetDlgItem(IDC_btnMSGTEST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_bTEST_LOAN)->ShowWindow(SW_HIDE);
	}
		
 	if( SetCenterUserInfo() )
 	{
		
		m_pUnionUserInfoViewer = new CUnionUserInfoViewer( this );
		m_pUnionUserInfoViewer->m_UseCenterUserInfo = m_UseCenterUserInfo;
		m_pUnionUserInfoViewer->m_UseCenterUserLoanLog = m_UseCenterUserLoanLog;
		m_pUnionUserInfoViewer->Create( this );
		m_pUnionUserInfoViewer->ShowWindow( false );
 	}
	
	ChangeMobileDMFieldName();
	ChangeToolbarButton();
		
	m_pPreRegNoMgr = new CPreRegCodeMgr(this);
		
	pGrid->m_bFroceReverse = FALSE;

	CRect WndPos;
	GetWindowRect(WndPos);
	MoveWindow( WndPos.left , WndPos.top , WndPos.Width() , WndPos.Height() -1 );
	MoveWindow( WndPos.left , WndPos.top , WndPos.Width() , WndPos.Height() );

	SetTimer( 0 , 100 , NULL );

	m_pMessageBox = new CBO_LOC_3199(this);
	m_pMessageBox->Create(this);

	CRect MessageWnd;
	m_pMessageBox->GetWindowRect(MessageWnd);
	::AfxGetApp()->GetMainWnd()->GetWindowRect(WndPos);

	INT nX , nY;
	nX = WndPos.right - MessageWnd.Width();
	nY = WndPos.bottom - MessageWnd.Height();
	m_pMessageBox->MoveWindow(nX,nY,MessageWnd.Width(),MessageWnd.Height());

	m_pUserSimpleDetailInfo = new CBO_LOC_3197(this);
	m_pUserSimpleDetailInfo->Create(this);

	CRect UserInfo;
	m_pUserSimpleDetailInfo->GetWindowRect( UserInfo );
	CRect ButtonRect;
	GetDlgItem(IDC_btnUSER_SIMPLE_DETAIL)->GetWindowRect(ButtonRect);
	nX = ButtonRect.right - WndPos.left;
	nY = ButtonRect.top - WndPos.top - 100;
	m_pUserSimpleDetailInfo->MoveWindow(nX,nY,UserInfo.Width(),UserInfo.Height());
	
	CRect	rcNoteXPos , rcNoteYPos;
	GetDlgItem(IDC_STATIC_REMARK_POS)->GetWindowRect(rcNoteXPos);
	ScreenToClient(rcNoteXPos);
	GetDlgItem(IDC_STATIC_GRID_GROUP)->GetWindowRect(rcNoteYPos);
	ScreenToClient(rcNoteYPos);
	nX = rcNoteXPos.left - 20;
	nY = rcNoteYPos.top	+ 7;

	m_pNote = new CNoteCtrl;
	m_pNote->Create( CPoint(nX,nY), 380, 21, this );
	
	SetLoanStopDateMode();
	
	SetReturnCheckMode();
	
	if( m_pLoanMangeValue->m_sFeeMngSys == _T("Y") )
	{		
		m_pFeeArrearReceipt = new CFeeArrearReceiptDlg( this, m_pLoanMangeValue );
		m_pFeeArrearReceipt->Create( this );
		
		CRect rectParent;
		CRect rectFee;
		this->GetParentFrame()->GetWindowRect(rectParent);				
		m_pFeeArrearReceipt->GetClientRect(rectFee);
		m_pFeeArrearReceipt->MoveWindow(rectParent.right - rectFee.Width()-10, rectParent.bottom - rectFee.Height()-65,  rectFee.Width()+5, rectFee.Height()+30);
		
		((CButton*) GetDlgItem (IDC_cSetLoanStopDateMode))->EnableWindow(FALSE);
		m_nLoanStopDateMode = 0;
	}

	m_BarcodeFlowSettingDlg.InitDM();
	
	CMSHFlexGrid* gridLoanList;		
	CMSHFlexGrid* gridReturnList;	
	CStatic* staticReturnList;		
	CStatic* staticReturnGroup;		
	gridLoanList		= (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN1);
	gridReturnList		= (CMSHFlexGrid*)GetDlgItem(IDC_gridMAIN2);
	staticReturnList	= (CStatic*)GetDlgItem(IDC_STATIC_RETURN_LIST);
	staticReturnGroup	= (CStatic*)GetDlgItem(IDC_STATIC_GRID_GROUP2);

	if(!m_pInfo->m_bUserInfoExportYN)
	{	
		((CESL_Grid*)gridReturnList)->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
		((CESL_Grid*)gridLoanList)->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		((CESL_Grid*)gridReturnList)->m_bPrivacyPrintLogRecord = TRUE;
		((CESL_Grid*)gridReturnList)->m_strPrintInfo = _T("대출/반납 대출및예약목록");
		((CESL_Grid*)gridLoanList)->m_bPrivacyPrintLogRecord = TRUE;
		((CESL_Grid*)gridLoanList)->m_strPrintInfo = _T("대출/반납 처리목록");
	}

	CRect rectSplitterBar(0, 0, 0, 0);
	m_splitterBar.Create( WS_CHILD|WS_VISIBLE, rectSplitterBar, this, 9259, TRUE );
	m_splitterBar.ModifyStyleEx(0,WS_EX_TRANSPARENT);
	m_splitterBar.SetPanes(	 gridLoanList
							,gridReturnList
							,staticReturnList->GetSafeHwnd()
							,staticReturnGroup->GetSafeHwnd() );
	
	m_bInitSplitter	= TRUE;		
	
	CString strUserBasketCnt;
	GetUserBasketMaxCount();
	strUserBasketCnt.Format( _T("%d") , m_pLoanMangeValue->m_nUserBasketCnt );
	GetDlgItem( IDC_EDIT_USER_BASKET_MAX_COUNT )->SetWindowText( strUserBasketCnt );	
	
	CFileFind ff;
	if( ff.FindFile(_T("..\\cfg\\_test")) )
	{		
		( (CButton*) GetDlgItem (IDC_btnTESTLoan) )->ShowWindow(SW_SHOW);
		( (CButton*) GetDlgItem (IDC_btnTESTSearchUser) )->ShowWindow(SW_SHOW);
	}
	ff.Close();
	
	CString strValue;
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("등록구분"), _T("도서관별 등록구분 중복여부"), strValue);
	if(_T("N") == strValue || TRUE == strValue.IsEmpty())
	{
		GetDlgItem(IDC_staBOOK_MANAGE_CODE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbBookManageCode)->ShowWindow(SW_HIDE);		
	}

	SetDefaultFocus();

	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("아이핀사용여부"), g_strIpinFlag);
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("MYPIN사용여부"), g_strMypinCertifyFlag);
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("휴대폰인증사용여부"), g_strPhoneCertifyFlag);
	if(TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
	{
		m_hBalloonIcon = (HICON)::LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(IDI_BALLOONICON), RT_GROUP_ICON), MAKEINTRESOURCE(IDI_BALLOONICON), IMAGE_ICON, 0, 0, 0);
		m_BalloonTip.Create(this);
		m_Font.DeleteObject();
		m_Font.CreatePointFont(90, _T("굴림체"));
		m_BalloonTip.SetFont(&m_Font);
		
		m_btnCertify_Y.SetSkin(IDB_CERTIFY_Y1,IDB_CERTIFY_Y2,IDB_CERTIFY_Y3,IDB_CERTIFY_Y4,0,0,0,0,0);
		m_btnCertify_N.SetSkin(IDB_CERTIFY_N1,IDB_CERTIFY_N2,IDB_CERTIFY_N3,IDB_CERTIFY_N4,0,0,0,0,0);
		m_btnCertify_Fail.SetSkin(IDB_CERTIFY_FAIL1,IDB_CERTIFY_FAIL2,IDB_CERTIFY_FAIL3,IDB_CERTIFY_FAIL4,0,0,0,0,0);
		
		if (g_strIpinFlag.CompareNoCase(_T("Y")) == 0 && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0)
		{
			m_btnCertify_Y.EnableWindow(FALSE);
			m_btnCertify_N.EnableWindow(FALSE);
			m_btnCertify_Fail.EnableWindow(FALSE);
		}
		
		m_ToolTip.Create(this);
		m_ToolTip.SetMaxTipWidth(400);

		if (g_strIpinFlag.CompareNoCase(_T("Y")) == 0 && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0)
		{
			m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Y), _T("본인인증한 이용자입니다."));
			m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증을 하지 않은 이용자입니다."));
			m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Fail), _T("본인인증에 실패한 이용자입니다."));

		}
		else
		{
			m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Y), _T("본인인증한 이용자입니다."));
			m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증을 하기 위해서는\r\n이 버튼을 클릭해 주세요."));
			m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Fail), _T("본인인증에 실패한 이용자입니다.\r\n실패사유 및 조치사항을 확인하거나\r\n본인인증을 하기 위해서는\r\n이 버튼을 클릭해 주세요."));
		}
				
		m_ToolTip.Activate(TRUE);
		m_ToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_INITIAL,MAKELONG(0,0));
		m_ToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_AUTOPOP,MAKELONG(20000,0));		
		
		CRect rPos;
		GetDlgItem(IDC_STATIC_USER_PICTURE)->GetWindowRect(&rPos);
		ScreenToClient(rPos);
		
		CRect rObject;
		GetDlgItem(IDC_btn3100Certify_Y)->GetWindowRect(&rObject);
		ScreenToClient(rObject);
		int nHeight = rObject.Height();
		int nWidth = rObject.Width();
		rObject.left = rPos.left;
		rObject.right = rPos.left + nWidth;
		rObject.top = rPos.bottom - nHeight;
		rObject.bottom = rPos.bottom;
		GetDlgItem(IDC_btn3100Certify_Y)->MoveWindow(rObject);
		
		GetDlgItem(IDC_btn3100Certify_N)->GetWindowRect(&rObject);
		ScreenToClient(rObject);
		nHeight = rObject.Height();
		nWidth = rObject.Width();
		rObject.left = rPos.left;
		rObject.right = rPos.left + nWidth;
		rObject.top = rPos.bottom - nHeight;
		rObject.bottom = rPos.bottom;
		GetDlgItem(IDC_btn3100Certify_N)->MoveWindow(rObject);
		
		GetDlgItem(IDC_btn3100Certify_Fail)->GetWindowRect(&rObject);
		ScreenToClient(rObject);
		nHeight = rObject.Height();
		nWidth = rObject.Width();
		rObject.left = rPos.left;
		rObject.right = rPos.left + nWidth;
		rObject.top = rPos.bottom - nHeight;
		rObject.bottom = rPos.bottom;
		GetDlgItem(IDC_btn3100Certify_Fail)->MoveWindow(rObject);
		
		m_btnCertify_Y.ShowWindow(FALSE);
		m_btnCertify_N.ShowWindow(FALSE);
		m_btnCertify_Fail.ShowWindow(FALSE);		
		m_nCertifyState = 0;
	}
	
	if (_T("Y") == m_pLoanMangeValue->m_str3100ControlYN) {
		GetDlgItem(IDC_sta3100Control)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_rad3100SetControl)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_rad3100ClearControl)->ShowWindow(SW_SHOW);
		CString strGetData, strSetData;
		CString strSetFile = _T("..\\cfg\\3100SETLRC");
		CFileFind find;
		CStdioFileEx File;
		if (FALSE == find.FindFile(strSetFile)) {
			if (File.Open(strSetFile, CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode)) {	
				File.WriteString(_T("제어설정|0"));
				File.Close();
			}
			((CButton*)GetDlgItem(IDC_rad3100SetControl))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_rad3100ClearControl))->SetCheck(TRUE);
			Onrad3100ClearControl();
		} else {
			if(File.Open(strSetFile, CFile::modeRead|CFile::typeText)) {
				File.ReadString(strGetData);
				File.Close();
			}
			AfxExtractSubString(strSetData, strGetData, 1, '|');
			if (_T("1") == strSetData) {
				((CButton*)GetDlgItem(IDC_rad3100ClearControl))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_rad3100SetControl))->SetCheck(TRUE);
				Onrad3100SetControl();
			} else {
				((CButton*)GetDlgItem(IDC_rad3100SetControl))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_rad3100ClearControl))->SetCheck(TRUE);
				Onrad3100ClearControl();
			}
		}
	} else {
		GetDlgItem(IDC_sta3100Control)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_rad3100SetControl)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_rad3100ClearControl)->ShowWindow(SW_HIDE);
	}

	m_strRfidUseYN =_T("N");

	m_pLoanMangeValue->GetManageValue_Name(_T("기타"), _T("공통"), _T("RFID사용여부"), m_strRfidUseYN);
	if(_T("Y") == m_strRfidUseYN )
	{
		GetDlgItem(IDC_btnCheckRFID)->EnableWindow(TRUE);	
		RFIDConnCheck();
	}
	
	if( m_pLoanMangeValue->m_sBookCooperativeSys != _T("Y") || m_pInfo->MODE == 10000) {
		CString sAlias1 = _T("책두레_대출");
		CString sAlias2 = _T("책두레_반납");
		CString sAlias3 = _T("책두레_전환");
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, (WPARAM)&sAlias1, TRUE );
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, (WPARAM)&sAlias2, TRUE );
		pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, (WPARAM)&sAlias3, TRUE );
	}

	return TRUE;

EFS_END
return FALSE;
}

VOID CBO_LOC_3100::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	if( TRUE	==	m_bLBTNDown )
	{
		GetWindowRect(m_rc3100Now);
		ScreenToClient(m_rc3100Now);

		INT		nCY	=	m_rc3100Now.Height() - m_rc3100Before.Height();

		CWnd* pGuide11Wnd	=	GetDlgItem(IDC_GROUP_GUIDE_LINE11);
		if( NULL == pGuide11Wnd )					return;
		if( NULL == pGuide11Wnd->GetSafeHwnd() )	return;
		CRect		rcGuide11;
		pGuide11Wnd->GetWindowRect(rcGuide11);
		ScreenToClient(rcGuide11);
		rcGuide11.bottom	=	nCY;
		pGuide11Wnd->MoveWindow(rcGuide11);
	}

	SetCtrlPosition(cx, cy);	
	
	if( TRUE ==	m_bInitSplitter )
	{
		m_splitterBar.ReSetSplitterMoveMostValue();
	}

	GetWindowRect(m_rc3100Before);
	ScreenToClient(m_rc3100Before);

EFS_END
}

VOID CBO_LOC_3100::OnbtnFamily() 
{
EFS_BEGIN

	INT ids;

	ids = m_pLoan->SPFExcutePath( _T("대출_가족회원") );
	
	SetDefaultFocus();

EFS_END
}

VOID CBO_LOC_3100::OnbtnNUMBERFIND() 
{
EFS_BEGIN

	INT ids;

	CString sRegNo;
	GetDlgItem(IDC_edtNUMBER)->GetWindowText(sRegNo);
	sRegNo.TrimLeft(); sRegNo.TrimRight();
	if( sRegNo.IsEmpty() ) return ;

	
	ids = m_pLoan->SPFExcutePath( _T("대출") );
	if( ids == 1 ) 
	{		
		CBO_LOC_3192 Dlg(this,m_pLoanShareManager,m_pLoanMangeValue,1);
		Dlg.DoModal();
	}
	
	ClearNumberCtrl();
	
	ClearLibCodeControl();

EFS_END
}

BOOL CBO_LOC_3100::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN
	
	if (TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
	{
		m_ToolTip.RelayEvent(pMsg);
	}

	if (_T("Y") == m_pLoanMangeValue->m_sIsUnityLoanService && 10000 != m_pInfo->MODE && 30000 != m_pInfo->MODE)
	{
		if (g_strCivilNoFlag.CompareNoCase(_T("Y")) == 0)
		{
			m_certifyToolTip.RelayEvent(pMsg);
		}
	}

	INT ids;	
	
	if( WM_SYSKEYDOWN == pMsg->message || WM_SYSKEYUP   == pMsg->message )
	{
		CDialog::PreTranslateMessage(pMsg);
		return TRUE;
	}
	
	
	if( WM_RBUTTONUP == pMsg->message )
	{		
		if( GetDlgItem(IDC_edtNUMBER)->GetSafeHwnd() == pMsg->hwnd )
		{
			OpenRegCodeList(pMsg->pt);
			return TRUE;
		}		
	}
		
	if( WM_KEYDOWN == pMsg->message && VK_ESCAPE == pMsg->wParam )
	{
		if(m_bIsClear)
		{
			return TRUE;
		}

		m_bIsClear = TRUE;

		CString strPath;
		
		
		if( m_pLoanMangeValue->m_sFeeMngSys == _T("Y") )
		{
			if( m_pFeeArrearReceipt != NULL )
			{
				m_pFeeArrearReceipt->Clear();
			}
		}
						
		strPath = _T("영수증출력체크");
		ids = m_pLoan->SPFExcutePath( strPath );
				
		strPath = _T("화면초기화");
		ids = m_pLoan->SPFExcutePath( strPath );
		
		SetDefaultFocus();	
		
		if( !(((CButton*)GetDlgItem(IDC_chkSET_HOLD))->GetCheck()) )
		{			
			((CComboBox*)GetDlgItem(IDC_cmbLOANTYPE))->SetCurSel(0);
			SetLoanType(0);
			((CComboBox*)GetDlgItem(IDC_cmbRETURN_TYPE))->SetCurSel(0);
			SetReturnType(0);
			
			CString sLoanDate;
			CTime tTime = CTime::GetCurrentTime();
			sLoanDate.Format( _T("%04d/%02d/%02d"), tTime.GetYear(), tTime.GetMonth(), tTime.GetDay() );
			SetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("소급대출일"), sLoanDate );
			SetControlData( _T("CM_LOAN_AND_RETURN_TYPE"), _T("소급반납일"), sLoanDate );

			
			((CButton*) GetDlgItem (IDC_cSetLoanStopDateMode))->SetCheck(m_nLoanStopDateMode);
		}

		OnSelchangecmbLOANTYPE();
		OnSelchangecmbRETURNTYPE();

		
		OnUserInit(NULL, NULL);
		
		CESL_ControlMultiComboBox* pMCmb = (CESL_ControlMultiComboBox*)
			FindControl(_T("CM_LOAN_AND_RETURN_TYPE"), _T("책관리구분"));
		CString strManageCode;
		for(INT nCmbRowCnt = 0; nCmbRowCnt<pMCmb->GetRowCount();nCmbRowCnt++)
		{
			pMCmb->GetData(strManageCode, nCmbRowCnt, 0);
			if(strManageCode == m_pInfo->MANAGE_CODE)
			{
				pMCmb->SetCurSel(nCmbRowCnt);
				break;
			}
		}
				
		if ( m_pBmp == NULL ) m_pBmp = new PLWinBmp;
		
		CRect WndPos;
		m_Decoder.MakeBmpFromFile ("..\\BMP\\profile_empty.JPG", m_pBmp);
		CDC* pDC = GetDlgItem(IDC_STATIC_USER_PICTURE)->GetDC();
		GetDlgItem(IDC_STATIC_USER_PICTURE)->GetWindowRect(WndPos);
		ClientToScreen(WndPos);
		PLFilterResizeBilinear Filter ( WndPos.Width()-2, WndPos.Height()-2);
		Filter.ApplyInPlace (m_pBmp);
		m_pBmp->Draw( pDC->m_hDC, 1, 1 );
		ReleaseDC(pDC);
		GetDlgItem(IDC_picUnityUserIcon)->Invalidate(FALSE);

		if (TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
		{
			m_btnCertify_Y.ShowWindow(FALSE);
			m_btnCertify_N.ShowWindow(FALSE);
			m_btnCertify_Fail.ShowWindow(FALSE);
			m_btnCertify_Y.Invalidate(FALSE);
			m_btnCertify_N.Invalidate(FALSE);
			m_btnCertify_Fail.Invalidate(FALSE);

			m_nCertifyState = 0;
		}

		m_bIsClear = FALSE;

		ClearLibCodeControl();

		return TRUE;
	}
	
	CEdit *edit = (CEdit*)GetDlgItem(IDC_EDT_LIBCODE);
	if((pMsg->message == WM_LBUTTONDOWN) &&
		(pMsg->hwnd == edit->m_hWnd) && 
		(GetFocus()->m_hWnd != edit->m_hWnd) )
	{  
		edit->SetFocus();
		edit->SetSel(0, -1, TRUE);
		return TRUE;
	}

	if( WS_CHILD == (GetStyle() & WS_CHILD) )
	{
		if( VK_RETURN == pMsg->wParam )
		{
			if( WM_KEYDOWN == pMsg->message || WM_CHAR == pMsg->message )
			{
				if( !m_bEnter ) return TRUE;				
				
				if( GetDlgItem(IDC_EDT_LIBCODE)->GetSafeHwnd() == pMsg->hwnd )
				{
					CString strGetData;
					GetDlgItemText(IDC_EDT_LIBCODE, strGetData);
					strGetData.Replace(_T("#"), _T(""));
					strGetData.TrimLeft(); strGetData.TrimRight();
					if( strGetData.IsEmpty() ) return TRUE;
					if(6 == strGetData.GetLength())
					{
						if(TRUE == SetLibCodeControl(strGetData))
						{
							return TRUE;
						}
					}
					SetDefaultFocus();
					ClearLibCodeControl();
					return TRUE;
				}
				
				if( GetDlgItem(IDC_edtNUMBER)->GetSafeHwnd() == pMsg->hwnd )
				{
					CString strRegNo;
					GetDlgItemText(IDC_edtNUMBER, strRegNo);
					strRegNo.TrimLeft(); strRegNo.TrimRight();
					if( strRegNo.IsEmpty() ) return TRUE;

					
					if(_T("Y") == m_pLoanMangeValue->m_sIsUnityLoanService)
					{
						
						INT nLength = strRegNo.GetLength();
						if( _T("#") == strRegNo.Left(1))
						{ 
							if(7 == nLength)
							{
								CString strLibCode = strRegNo.Mid(1, 6);
								
								if(TRUE == SetLibCodeControl(strLibCode))
								{
									ClearNumberCtrl();
									return TRUE;
								}
							}
							
							SetDefaultFocus();
							ClearLibCodeControl();
						}
						else if(_T("$") == strRegNo.Left(1))
						{ 
							if(6 < nLength)
							{
								CString strLibCode = strRegNo.Mid(1, 6);
								
								if(TRUE == SetLibCodeControl(strLibCode))
								{
									ClearNumberCtrl();
									if(7 < nLength)
									{ 
										
										CString strData;
										strData = strRegNo.Mid(7, nLength);
										SetDlgItemText(IDC_edtNUMBER, strData);
										
										GetDlgItem(IDC_edtNUMBER)->PostMessage(WM_KEYDOWN, VK_RETURN);
										return TRUE;
									}
									else
									{
										return TRUE;
									}
								}
							}
						}
					}
					if( 0 == m_pLoanMangeValue->m_sIs5163lib.Compare(_T("Y")) )
					{
						INT	nLength = strRegNo.GetLength();
						
						if( 11 == nLength ) 
						{
							strRegNo = strRegNo.Left(5);
							GetDlgItem(IDC_edtNUMBER)->SetWindowText(strRegNo);
						}
																		
						else if( 8 == nLength )
						{ 
							
							if( _T("06") == strRegNo.Left(2) )
							{
								CString strQuery;
								CString strUserNo;

								CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_DB") );
								if ( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("PreTranslateMessage"));

								strQuery.Format(_T("SELECT MAX(EMP_NO) FROM COM_IDCARD ")
									             _T("WHERE IDCARD_NO='%s'") , strRegNo);

								ids = pDM->GetOneValueQuery( strQuery , strUserNo );
								if ( 0 > ids && -4007 != ids ) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("PreTranslateMessage"));
								if ( -4007 == ids ) strUserNo = strRegNo;

								GetDlgItem(IDC_edtNUMBER)->SetWindowText(strUserNo);
							}
						}
					} 
					
					CButton* pBtn = (CButton*)GetDlgItem(IDC_chkBARCODEUSE);
					if( NULL == pBtn ) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("CBO_LOC_3100::PreTranslateMessage()"));

					
					if( 1 == pBtn->GetCheck() )
					{
						m_bEnter = FALSE;						
						
						if(strRegNo == _T("52455455524E"))
						{
							ClearNumberCtrl();

							CUnionReturnDlg dlg(this);
							dlg.DoModal();					
							
							m_bEnter = TRUE;
							return TRUE;
						}

						OnbtnNUMBERFIND2();			
						ClearNumberCtrl();
						
						ClearLibCodeControl();
						
						m_bEnter = TRUE;
						
						ShowTabWindow();
						
						SetUserName();
						
						if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )
						{
							return TRUE;
						}						
						return TRUE;
					}
					
					else
					{
						CString strPath;
						m_bEnter = FALSE;						
						
						if( GetMode() == 0 )
						{
							SearchUser(FALSE);
						}
						else
						{
							strPath = _T("대출");
							ids = m_pLoan->SPFExcutePath(strPath);
						}
						
						ClearNumberCtrl();
						
						ClearLibCodeControl();

						m_bEnter = TRUE;
						ShowTabWindow(0);
						return TRUE;
					}
				} 
				else if( GetDlgItem(IDC_edtNAME)->GetSafeHwnd() == pMsg->hwnd )
				{
					m_bEnter = FALSE;
					
					SearchUser();
					SetDefaultFocus();

					ClearNumberCtrl();
					
					ClearLibCodeControl();
					m_bEnter = TRUE;
					
					ShowTabWindow();
					return TRUE;
				}				
				else if(GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT)->GetSafeHwnd() == pMsg->hwnd)
				{
					OnButtonUserBasketMaxCount();
					return TRUE;
				}
			} 
		} 
	}
	
	return CESL_Mgr::PreTranslateMessage(pMsg);

EFS_END
return FALSE;
}

INT CBO_LOC_3100::GetMode()
{
EFS_BEGIN

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pDM = FindDM( sDMAlias );

	if( pDM->GetRowCount() < 1 ) return 0;

	return 1;

EFS_END
return -1;
}

VOID CBO_LOC_3100::SetInitControl()
{
EFS_BEGIN
	
	((CButton*)(GetDlgItem( IDC_radLOANTYPE1 )))->SetCheck(1);	 
	((CButton*)(GetDlgItem( IDC_radRETURNTYPE1 )))->SetCheck(1); 

	SetDefaultFocus();
	
	INT ids;
	CString sValue;

	ids = m_pLoan->m_pCheckLoan->IsLoanReturnTypeUse( _T("대출"), _T("특별") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-1, _T("SetInitControl"));
	if( ids == 1 ) ;
	else
		((CComboBox*)GetDlgItem(IDC_cmbLOANTYPE))->AddString( _T("특별") );

	ids = m_pLoan->m_pCheckLoan->IsLoanReturnTypeUse( _T("대출"), _T("관내") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-2, _T("SetInitControl"));
	if( ids == 1 ) ;
	else
		((CComboBox*)GetDlgItem(IDC_cmbLOANTYPE))->AddString( _T("관내") );
	
	CString strValue;
	m_pLoanMangeValue->GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), strValue );
	if ( strValue == _T("Y") )
	{		
		ids = m_pLoan->m_pCheckLoan->IsLoanReturnTypeUse( _T("대출"), _T("장기") );
		if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-6, _T("SetInitControl"));
		if( ids == 1 ) ;
		else
			((CComboBox*)GetDlgItem(IDC_cmbLOANTYPE))->AddString( _T("장기") );
	}

	((CComboBox*)GetDlgItem(IDC_cmbLOANTYPE))->SetCurSel(0);
	SetLoanType(0);
	
	ids = m_pLoan->m_pCheckLoan->IsLoanReturnTypeUse( _T("반납"), _T("일괄") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-3, _T("SetInitControl"));
	if( ids == 1 ) ;
	else
		((CComboBox*)GetDlgItem(IDC_cmbRETURN_TYPE))->AddString( _T("일괄") );

	ids = m_pLoan->m_pCheckLoan->IsLoanReturnTypeUse( _T("반납"), _T("소급") );
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-4, _T("SetInitControl"));
	if( ids == 1 ) ;
	else
		((CComboBox*)GetDlgItem(IDC_cmbRETURN_TYPE))->AddString( _T("소급") );
	((CComboBox*)GetDlgItem(IDC_cmbRETURN_TYPE))->SetCurSel(0);
	SetReturnType(0);
	
	ids = m_pLoan->m_pCheckLoan->IsFamilyFunc();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2(-5, _T("SetInitControl()"));
	if( ids == 1 )
	{		
		MoveUserInfoCtrl();
	}
	
	if(_T("Y") != m_pLoanMangeValue->m_ArrearCheckYN)
	{
		GetDlgItem(IDC_btnARREARINFO)->ShowWindow(SW_HIDE);
	}

	
	((CButton*)GetDlgItem(IDC_chkBARCODEUSE))->SetCheck(1);

EFS_END
}

VOID CBO_LOC_3100::OnradLOANTYPE1() 
{
EFS_BEGIN

	SetLoanType(0);	

EFS_END
}

VOID CBO_LOC_3100::OnradLOANTYPE2() 
{
EFS_BEGIN

	SetLoanType(1);	

EFS_END
}

VOID CBO_LOC_3100::OnradLOANTYPE3() 
{
EFS_BEGIN

	SetLoanType(2);	

EFS_END
}

VOID CBO_LOC_3100::OnradRETURNTYPE1() 
{
EFS_BEGIN

	SetReturnType(0);	

EFS_END
}

VOID CBO_LOC_3100::OnradRETURNTYPE2() 
{
EFS_BEGIN

	SetReturnType(1);	

EFS_END
}

VOID CBO_LOC_3100::OnradRETURNTYPE3() 
{
EFS_BEGIN

	SetReturnType(2);		

EFS_END
}


INT CBO_LOC_3100::SetLoanType( INT nType )
{
EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_LOAN_AND_RETURN_TYPE");
	CString sFieldAlias = _T("대출형태");

	TCHAR *sType[4] =
	{
		_T("0"), _T("1"), _T("2"), _T("3")
	};
	ids = SetControlData( sCMAlias, sFieldAlias, sType[nType] );
	if( ids < 0 ) return -1;

	return 0;


EFS_END
return -1;
}


INT CBO_LOC_3100::SetReturnType( INT nType )
{
EFS_BEGIN

	GetDlgItem(IDC_datRETURN_DATE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_datEDIT_RETURN_DATE)->ShowWindow(SW_HIDE);
	if( nType != 2 )
	{
		GetDlgItem(IDC_datRETURN_DATE)->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_datRETURN_DATE))->SetReadOnly(TRUE);
	}
	else 
	{
		GetDlgItem(IDC_datEDIT_RETURN_DATE)->ShowWindow(SW_SHOW);
	}

	INT ids;
	CString sCMAlias = _T("CM_LOAN_AND_RETURN_TYPE");
	CString sFieldAlias = _T("반납형태");

	TCHAR *sType[3] =
	{
		_T("0"), _T("1"), _T("2")
	};
	
	ids = SetControlData( sCMAlias, sFieldAlias, sType[nType] );
	if( ids < 0 ) return -1;

	return 0;

EFS_END
return -1;
}
BEGIN_EVENTSINK_MAP(CBO_LOC_3100, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_LOC_3100)
	ON_EVENT(CBO_LOC_3100, IDC_gridMAIN1, -600 /* Click */, OnClickgridMAIN1, VTS_NONE)
	ON_EVENT(CBO_LOC_3100, IDC_gridMAIN1, -601 /* DblClick */, OnDblClickgridMAIN1, VTS_NONE)
	ON_EVENT(CBO_LOC_3100, IDC_gridMAIN1, -604 /* KeyUp */, OnKeyUpgridMAIN1, VTS_PI2 VTS_I2)
	ON_EVENT(CBO_LOC_3100, IDC_gridMAIN1, -606 /* MouseMove */, OnMouseMovegridMAIN1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBO_LOC_3100, IDC_gridMAIN1, -605 /* MouseDown */, OnMouseDowngridMAIN1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBO_LOC_3100, IDC_gridMAIN1, -607 /* MouseUp */, OnMouseUpgridMAIN1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBO_LOC_3100, IDC_gridUSER_BASKET, -601 /* DblClick */, OnDblClickgridUSERBASKET, VTS_NONE)
	ON_EVENT(CBO_LOC_3100, IDC_GRID_SIMPLE_FAMILY_VIEW, -600 /* Click */, OnClickGridSimpleFamilyView, VTS_NONE)
	ON_EVENT(CBO_LOC_3100, IDC_GRID_SIMPLE_FAMILY_VIEW, -601 /* DblClick */, OnDblClickGridSimpleFamilyView, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBO_LOC_3100::OnClickgridMAIN1() 
{

}

VOID CBO_LOC_3100::OnbtnDETAILINFO() 
{
EFS_BEGIN

	INT ids;
	ids = m_pLoan->SPFExcutePath( _T("이용자_상세정보수정") );
	
	SetDefaultFocus();

EFS_END
}

VOID CBO_LOC_3100::OnbtnNOTEMODIFY() 
{

}

INT CBO_LOC_3100::GetReturnDelayDay( CString &sReturnDelayDay )
{
EFS_BEGIN

	sReturnDelayDay = _T("0");
	return 0;

EFS_END
return -1;
}
VOID CBO_LOC_3100::OnbtnNUMBERFIND2() 
{
EFS_BEGIN

	if( m_nBarcodeMode == 2 )
	{ 
		SearchUser(FALSE);
	}
	else
	{ 
		INT ids;
		ids = m_pLoan->SPFExcutePath( _T("바코드업무") );				
		if( ids == 1 ) 
		{
			
			CBO_LOC_3192 Dlg(this,m_pLoanShareManager,m_pLoanMangeValue,1);
			Dlg.DoModal();
		}		
		if( m_pLoanMangeValue->m_sFeeMngSys == _T("Y") )
		{
			if( m_pFeeArrearReceipt != NULL )
			{
				ShowFeeReceipt();
			}
		}
	}

	SetDefaultFocus();
	

EFS_END
}

VOID CBO_LOC_3100::OnbtnTEST() 
{
EFS_BEGIN

	ColorSetting();

EFS_END
}

INT CBO_LOC_3100::ColorSetting()
{
EFS_BEGIN

	CBO_LOC_3190 Dlg(this, m_pLoanShareManager, m_pLoanMangeValue);

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3100::SetColorSetting()
{
EFS_BEGIN

	CBO_LOC_3190 Dlg(this, m_pLoanShareManager, m_pLoanMangeValue);
	Dlg.FileLoad();

	INT ids = m_pLoan->ViewGrid();				
	

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3100::RunAutoJob()
{
EFS_BEGIN

	if( (m_pLoan->m_pCheckLoan->GetAutoChangeReserveCancel() > 0) && 
		(m_pLoan->m_pCheckLoan->GetAutoChangeUserClass() > 0) );
	else
	{
		INT ids;
		CString strQuery, strCount;
		strQuery.Format(_T("SELECT COUNT(1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='자동작업' AND VALUE_NAME='자동작업일자' AND MANAGE_CODE=UDF_MANAGE_CODE"));
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
		TmpDM.GetOneValueQuery(strQuery, strCount);
		strCount.TrimLeft();	strCount.TrimRight();
		if(strCount.IsEmpty())
		{
			strCount = _T("0");
		}
		if(1 > _ttoi(strCount))
		{ 
			strQuery.Format(
				_T("INSERT INTO ESL_MANAGE_TBL(REC_KEY,GROUP_1,GROUP_2,GROUP_3,CLASS_ALIAS,CLASS_DESC,VALUE_NAME,VALUE_1,FIRST_WORK,SYS_SETUP_YN,MANAGE_CODE) ")
				_T("VALUES(ESL_SEQ.NEXTVAL,'L','기타','공통','자동작업','도서관별로 금일 대출반납 화면을 처음 실행할때 일자기록','자동작업일자',TO_CHAR(SYSDATE-1,'YYYY/MM/DD'),TO_CHAR(SYSDATE,'YYYY/MM/DD HH24:MI:SS'),'Y',UDF_MANAGE_CODE);"));
			TmpDM.StartFrame();
			TmpDM.AddFrame(strQuery);
			ids = TmpDM.SendFrame();
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("RunAutoJob"));
			}
			TmpDM.EndFrame();
		}
		
		CString strGetData;
		CTime tCurrent = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
		CString sCurrent = strGetData.Left(10);

		CString sDate;
		strQuery.Format(
			_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
			_T("WHERE CLASS_ALIAS='자동작업' AND VALUE_NAME='자동작업일자' AND MANAGE_CODE=UDF_MANAGE_CODE"));
		TmpDM.GetOneValueQuery(strQuery, sDate);
		sDate.TrimLeft();	sDate.TrimRight();
		if(!sDate.IsEmpty())
		{
			if(sCurrent == sDate)
			{ 
				return 0;
			}
		}
		
		CBO_LOC_3191 Dlg( this, m_pLoanShareManager, m_pLoanMangeValue );
 		Dlg.DoModal();
		
		strQuery.Format(
			_T("UPDATE ESL_MANAGE_TBL SET VALUE_1=TO_CHAR(SYSDATE,'YYYY/MM/DD'), LAST_WORK=TO_CHAR(SYSDATE,'YYYY/MM/DD HH24:MI:SS') ")
			_T("WHERE CLASS_ALIAS='자동작업' AND VALUE_NAME='자동작업일자' AND MANAGE_CODE=UDF_MANAGE_CODE;"));
		TmpDM.StartFrame();
		TmpDM.AddFrame(strQuery);
		ids = TmpDM.SendFrame();
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("RunAutoJob"));
		}
		TmpDM.EndFrame();
 	}
	
	if( m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000 )	return 0;

	//JOB.2019.0073 : 충돌 License
// 	CString strValue;
// 	m_pLoanMangeValue->GetManageValue( _T("기타"), _T("공통"), _T("메일링시스템"), _T("사용여부"), strValue );
// 	strValue.TrimLeft();	strValue.TrimRight();	strValue.MakeUpper();
// 	if ( strValue != _T("Y") ) 
// 	{	
// 		
// 		m_pLoanMangeValue->GetManageValue_Name( _T("기타"), _T("공통"), _T("메일일괄발송"), strValue );
// 		strValue.TrimLeft();	strValue.TrimRight();	strValue.MakeUpper();
// 		
// 		if( strValue == _T("Y") )
// 		{
// 			
// 			m_pLoanMangeValue->GetManageValue(_T("기타"), _T("공통"), _T("메일일괄발송시간설정"), _T(""), strValue);		
// 			strValue.TrimLeft();	strValue.TrimRight();	strValue.MakeUpper();
// 			
// 			CString strGetData;
// 			CTime tCurren = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
// 			CString sCurrent = strGetData.Left(10);
// 
// 			if( strValue < sCurrent )
// 			{
// 				this->SetManageValue(_T("기타"),_T("공통"),_T("메일일괄발송시간설정"),_T(""),sCurrent);
// 				CBO_LOC_3193 Dlg(this);
// 				Dlg.DoModal();				
// 			}
// 		}
// 	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3100::OnDblClickgridMAIN1() 
{
EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( pCM == NULL ) return;
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) return;

	if( pGrid->GetMouseRow() < 1 )
	{
		PostMessage( WM_USER_REFRESH_GRID , 0 , 0 );
		return ;
	}

	m_pLoan->SPFExcutePath(_T("상세종정보"));
	
	SetDefaultFocus();
	
	m_pLoan->ViewGrid();
	

EFS_END
}

VOID CBO_LOC_3100::OnKeyUpgridMAIN1(short FAR* KeyCode, short Shift) 
{
EFS_BEGIN

	if( *KeyCode == 27 ) 
	{		
		::PostMessage( this->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 1 );
	}

EFS_END
}

VOID CBO_LOC_3100::ClearNumberCtrl()
{
EFS_BEGIN

	GetDlgItem(IDC_edtNUMBER)->SetWindowText(_T(""));
	
	SetDefaultFocus();
	
EFS_END
}

VOID CBO_LOC_3100::OnTimer(UINT nIDEvent) 
{
EFS_BEGIN

	if( nIDEvent == 0 )
	{
		KillTimer(0);
		if( m_pLoanMangeValue->m_bLoad )
		{
			INT ids = RunFirstWork();
			SetDefaultFocus();
			if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnTimer") );
		}
		else
		{
			SetTimer( 0 , 100 , NULL );
		}
	}
	else if(WM_USER+1028 == nIDEvent)
	{
		KillTimer(WM_USER+1028);
		SearchUser(FALSE);
		SetUserName();
	}
	
	else if(1 == nIDEvent)
	{
		if(TRUE == (GetDlgItem(IDC_staticPRIVATEMANAGE)->IsWindowVisible()))
		{
			GetDlgItem(IDC_staticPRIVATEMANAGE)->ShowWindow(SW_HIDE);
		}
		else
		{
			GetDlgItem(IDC_staticPRIVATEMANAGE)->ShowWindow(SW_SHOW);
		}
	}
	else if(TIMER_API_LRC_MONITOR == nIDEvent)
	{
		if (!::FindWindow(NULL, _T("LoanReturnControl")))
		{
			KillTimer(TIMER_API_LRC_MONITOR);
			Load3100ControlProcess();
		}
	}
	CDialog::OnTimer(nIDEvent);

EFS_END
}

INT CBO_LOC_3100::RunFirstWork()
{
EFS_BEGIN

	INT ids;

	m_pLoan = new CBL_LOC_LOAN_PROC( this, m_pLoanShareManager, m_pLoanMangeValue, m_pPreRegNoMgr );
			
	ids = m_pInfo->GetSMSUseMode();
	if(ids==0)		m_sIsSMS = _T("N");
	else if(ids==1)	m_sIsSMS = _T("Y");
	else			m_sIsSMS = _T("N");

	if( m_sIsSMS == _T("Y") )
	{
		CESL_DataMgr* pUserDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
		INT nColumnCnt = pUserDM->RefList.GetCount();
		
		CString sFieldAlias, sFieldName, sFieldType, sInitValue, sDataType;
		for( INT i = 0; i < nColumnCnt; i++ )
		{
			sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
			ids = pUserDM->GetColumnRefInfo( i, sFieldAlias, sFieldName, sFieldType, sInitValue, sDataType );
			if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -2, _T("RunFirstWork") );
			
			if( sFieldAlias.Compare( _T("SMS수신여부") ) != 0 ) continue;
			
			ids = pUserDM->SetDataMgrRef( i, sFieldAlias, sFieldName, sFieldType, sInitValue, _T("") );			
			if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("RunFirstWork") );
		}
	}

	SetColorSetting();
	SetInitControl();
	RunAutoJob();
	
	InitTabUserInfo();
	
	if(_T("Y") == m_pLoanMangeValue->m_sIsUnityLoanService && 10000 != m_pInfo->MODE && 30000 != m_pInfo->MODE)
	{
		
		CString strGetData = _T("");
		CString strGetData2 = _T("");
		m_pLoan->GetTempData(_T("기본검색범위"), strGetData);
		m_pLoan->GetTempData(_T("기본통합대출가능권수"), strGetData2);
		if(strGetData2.IsEmpty())
		{
			m_pLoan->SetTempData(_T("기본통합대출가능권수"), CComVariant(_T("20")));
		}
		
		m_pLoan->SetTempData(_T("기본검색범위"), CComVariant(_T("2")));
		strGetData = _T("2");

		if(10000 == m_pInfo->MODE || 30000 == m_pInfo->MODE)
		{
			m_pLoan->SetTempData(_T("기본검색범위"), CComVariant(_T("0")));
			strGetData = _T("0");
		}
		else if(strGetData.IsEmpty())
		{
			m_pLoan->SetTempData(_T("기본검색범위"), CComVariant(_T("1")));
			strGetData = _T("1");
		}
		INT nCheck = _ttoi(strGetData);
		
		((CButton*)GetDlgItem(IDC_rad3100TCenter))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_rad3100Local))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_rad3100ACenter))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_rad3100TCenter))->ShowWindow(FALSE);

		if(10000 != m_pInfo->MODE && 30000 != m_pInfo->MODE)
		{
			m_pLoan->KL_GetManage();
		}
	}
	else
	{
		((CButton*)GetDlgItem(IDC_rad3100Local))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_rad3100ACenter))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_rad3100TCenter))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_rad3100Local))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_rad3100ACenter))->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_rad3100TCenter))->ShowWindow(FALSE);
	}
	
	CESL_Grid *pGrid_MainGrid_ALL = (CESL_Grid *)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO") , _T("MainGrid") );
	if( pGrid_MainGrid_ALL == NULL ) return -1;

	CArray <CString,CString> RemoveListArray;
	pGrid_MainGrid_ALL->InitPopupMenuItem();
	pGrid_MainGrid_ALL->AddPopupMenuItem( (CView*)pMain , this->VIEW_STYLE , &RemoveListArray );

	
	CWinApp *pApp = AfxGetApp();
	CString strbBeep;
	CString strbSimpleGrid;
	strbBeep = pApp->GetProfileString(_T("Settings"), _T("LoanBeep"));
	strbSimpleGrid = pApp->GetProfileString(_T("Settings"), _T("LoanSimpleGrid"));
	
	if ( strbBeep == _T("") || strbBeep == _T("TRUE")) 
	{
		pApp->WriteProfileString(_T("Settings"), _T("LoanBeep"),_T("TRUE"));
		( (CButton*) GetDlgItem(IDC_chkBEEP) )->SetCheck(TRUE);
		m_pLoan->m_bBeep = TRUE;
	}
	else
	{
		( (CButton*) GetDlgItem(IDC_chkBEEP) )->SetCheck(FALSE);
		m_pLoan->m_bBeep = FALSE;
	}

	if ( strbSimpleGrid == _T("TRUE")) 
	{
		pApp->WriteProfileString(_T("Settings"), _T("LoanSimpleGrid"),_T("TRUE"));
		( (CButton*) GetDlgItem(IDC_chkSimpleGrid) )->SetCheck(TRUE);
		m_pLoan->m_bSimpleGrid = TRUE;
	}
	else if ( strbSimpleGrid == _T("")) 
	{
		pApp->WriteProfileString(_T("Settings"), _T("LoanSimpleGrid"),_T("FALSE"));
		( (CButton*) GetDlgItem(IDC_chkSimpleGrid) )->SetCheck(FALSE);
		m_pLoan->m_bSimpleGrid = FALSE;
	}
	else
	{
		( (CButton*) GetDlgItem(IDC_chkSimpleGrid) )->SetCheck(FALSE);
		m_pLoan->m_bSimpleGrid = FALSE;
	}

	if(m_pLoan)
	{
		CString strFileName = _T("..\\cfg\\LoanFocus.cfg");
		m_pLoan->m_CfgFileApi.SetFile(strFileName);

		CFileFind FF;
		if(!FF.FindFile(strFileName))
		{ 
			FILE *fp = _tfopen(strFileName, _T("w+b"));
			fputc(0xFF, fp); 
			fputc(0xFE, fp);
			fclose(fp);		
				
		}
		FF.Close();		

		m_pLoan->m_CfgFileApi.GetData(_T("포커스"), m_strDefaultFocus);
	}
	SetDefaultFocus();
	
	m_pLoan->SetTempData(_T("대출반납흐름제어"), CComVariant(_T("N")));
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("아이핀사용여부"), g_strIpinFlag);
	m_pLoan->SetTempData(_T("아이핀사용여부"), CComVariant(g_strIpinFlag));
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("MYPIN사용여부"), g_strMypinCertifyFlag);
	m_pLoan->SetTempData(_T("MYPIN사용여부"), CComVariant(g_strMypinCertifyFlag));
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("휴대폰인증사용여부"), g_strPhoneCertifyFlag);
	m_pLoan->SetTempData(_T("휴대폰인증사용여부"), CComVariant(g_strPhoneCertifyFlag));
	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3100::OnSetfocusedtNUMBER() 
{
EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtNUMBER)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_ALPHANUMERIC;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_edtNUMBER)->m_hWnd,hIME); 
    } 

	if(m_pLoan)
	{
		CString strFileName = _T("..\\cfg\\LoanFocus.cfg");
		m_pLoan->m_CfgFileApi.SetFile(strFileName);

		CFileFind FF;
		if(!FF.FindFile(strFileName))
		{ 
			FILE *fp = _tfopen(strFileName, _T("w+b"));
			fputc(0xFF, fp); 
			fputc(0xFE, fp);
			fclose(fp);		
		}
		FF.Close();

		m_strDefaultFocus = _T("대출자번호");
		m_pLoan->m_CfgFileApi.SetData(_T("포커스"), _T("대출자번호"));
	}

EFS_END
}

VOID CBO_LOC_3100::OnSetfocusedtNAME() 
{
EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtNAME)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
        ImmReleaseContext(GetDlgItem(IDC_edtNAME)->m_hWnd,hIME); 
    } 

EFS_END
}

VOID CBO_LOC_3100::OnSelchangecmbLOANTYPE() 
{
EFS_BEGIN

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbLOANTYPE);	
	CString sLoanType;
	pCombo->GetLBText(pCombo->GetCurSel(), sLoanType);

	INT nMode;
	if( sLoanType == _T("일반") )		nMode = 0;
	else if( sLoanType == _T("특별") )	nMode = 1;
	else if( sLoanType == _T("관내") )	nMode = 2;
	else if( sLoanType == _T("장기") )	nMode = 3;
	else								nMode = 0;

	SetLoanType(nMode);

EFS_END
}

VOID CBO_LOC_3100::OnSelchangecmbRETURNTYPE() 
{
EFS_BEGIN

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbRETURN_TYPE);	
	CString sLoanType;
	pCombo->GetLBText(pCombo->GetCurSel(), sLoanType);

	INT nMode;
	if( sLoanType == _T("일반") )		nMode = 0;
	else if( sLoanType == _T("일괄") )	nMode = 1;
	else if( sLoanType == _T("소급") )	nMode = 2;
	else								nMode = 0;

	SetReturnType(nMode);
	
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(1 == pDM->GetRowCount())
	{
		CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
		pGrid->SelectMakeList();
		pGrid->SetProtectSelectColumn(TRUE);
		m_pLoan->SettingDelayDay();
		m_pLoan->AllControlDisplayGrid();
		m_pLoan->InitGridColor();
		m_pLoan->ViewGrid();
		pGrid->SetProtectSelectColumn(FALSE);
	}

EFS_END
}

VOID CBO_LOC_3100::OnKillfocusdatEDITRETURNDATE() 
{
EFS_BEGIN
	
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(1 == pDM->GetRowCount())
	{
		m_pLoan->SettingDelayDay();
		m_pLoan->AllControlDisplayGrid();
		m_pLoan->InitGridColor();
		m_pLoan->ViewGrid();
	}

EFS_END
}


INT CBO_LOC_3100::ChangeMobileDMFieldName()
{
	return 0;
}

LRESULT CBO_LOC_3100::OnRefreshGrid(WPARAM wParam, LPARAM lParam) 
{
EFS_BEGIN

	INT ids;
	
	ids = m_pLoan->ViewGrid();
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("OnRefreshGrid") )	
	
	return 0;

EFS_END
return -1;
}

LRESULT CBO_LOC_3100::OnRefreshGridFromLoanHIS(WPARAM wParam, LPARAM lParam) 
{
EFS_BEGIN

	m_pLoan->m_bReturnBookNonFree = TRUE;
	m_pLoan->SPFExcutePath( _T("회원리로드_NOASK") );

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3100::ChangeToolbarButton()
{
EFS_BEGIN
	
	CString sValue, strButtonAlias;
	sValue = m_pLoanMangeValue->m_sReserveMode;
	if( sValue == _T("0") )
	{
		strButtonAlias = _T("예약상태");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("예약");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("예약자료지정");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("예약대출");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
		strButtonAlias = _T("예약취소");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
	}
	else if ( !(m_pLoanMangeValue->m_bVolReserve) )
	{ 
		strButtonAlias = _T("예약자료지정");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
	}
		
	sValue = m_pLoanMangeValue->m_sIsDelayFunc;
	if( sValue != _T("Y") )
	{
		strButtonAlias = _T("반납연기");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);		
	}
	
	sValue = m_pLoanMangeValue->m_sUseReceipt;
	if( sValue != _T("Y") )
	{
		strButtonAlias = _T("영수증발급");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}
		
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("대출자접근권한"), m_pInfo->USER_ID, sValue);		
	if( _T("1") == sValue) 
	{
		strButtonAlias = _T("대출자");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}
	
	if(_T("Y") != m_pLoanMangeValue->m_sIsLoanAppendix)
	{
		strButtonAlias = _T("부록대출");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}
	
	m_pLoanMangeValue->GetManageValue(_T("기타"), _T("공통"), _T("타관반납설정"), _T(""), sValue);	
	if(_T("Y") != sValue)
	{
		strButtonAlias = _T("타관반납자료");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}

	if(m_pInfo->MODE == 10000 || m_pInfo->MODE == 30000)
	{
		strButtonAlias = _T("통함대출자정보보기");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("통합센터대출이력조회");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("타관반납관리");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("타관반납자료");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);	
	}

	if(_T("Y") != m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		strButtonAlias = _T("타관반납관리");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
		strButtonAlias = _T("통합센터대출이력조회");
		pMain->SendMessage(USER_TOOLBAR_HIDE_SHOW, (WPARAM)&strButtonAlias, TRUE);
	}

	return 0;

EFS_END
return -1;
}

LRESULT CBO_LOC_3100::OnRefreshMessageBox(WPARAM wParam, LPARAM lParam)
{
EFS_BEGIN

	m_pMessageBox->ViewMessageHistory();

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3100::OnbtnSHOWMESSAGE() 
{
	m_pMessageBox->ShowWindow(SW_SHOW);
}


VOID CBO_LOC_3100::OnbtnUSERSIMPLEDETAIL() 
{

}

LRESULT CBO_LOC_3100::OnUserLoaded(WPARAM wParam, LPARAM lParam) 
{
EFS_BEGIN
	
	SetUserName();
	
	if( m_sIsSMS == _T("Y") )
	{
		CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO"));
		if( pDM->GetRowCount() == 0 ) 
		{
			m_pNote->ShowWindow(SW_HIDE);
		}
		else
		{
			CString sHandPhone,sSMSUseYN;
			pDM->GetCellData( _T("휴대폰"), 0, sHandPhone );
			sHandPhone.TrimLeft(); sHandPhone.TrimRight();

			pDM->GetCellData( _T("SMS수신여부"), 0, sSMSUseYN );
			if( sHandPhone.IsEmpty() )
			{
				m_pNote->SetNote( _T("☞휴대폰 번호를 입력하시면 SMS서비스를 이용할 수 있습니다!!") );
				m_pNote->ShowWindow(SW_SHOW);
			}
			else
			{
				m_pNote->ShowWindow(SW_HIDE);
			}
		}
	}
	else
	{
		m_pNote->ShowWindow(SW_HIDE);
	}

	
	if( m_userInfo_Simple->m_pRemarkDlg->IsWindowVisible() )
	{
		m_userInfo_Simple->SetRemarkControl(FALSE);
	}

	CString userKey = GetCurrentUserPK();
	if( !userKey.IsEmpty() )
	{
		CodeChangeInDM();
		if( (m_pInfo->MODE != 10000))
		{
			this->ShowInfo();
		}
		ShowTabWindow();
	}
 	else
 	{
 		if( (m_pInfo->MODE != 10000))
 		{
 			this->ShowInfo();
 		}
 	}

	if( !userKey.IsEmpty() && m_UseCenterUserInfo )
	{
		
		if ( m_pLoanMangeValue->m_sIsUnionUserStatus		== _T("Y") ||
			 m_pLoanMangeValue->m_sIsUnionUserRemoveStatus	== _T("Y") || 
			 m_pLoanMangeValue->m_sIsUnionLoanCnt			== _T("Y") )
		{
			m_pUnionUserInfoViewer->ShowWindow( SW_SHOW );
		}

		if( m_pUnionUserInfoViewer->IsWindowVisible() )
		{
			m_pUnionUserInfoViewer->SearchCenterUserInfo( GetCurrentUserPK(), (BOOL)wParam );
		}
		else
		{
			m_pUnionUserInfoViewer->m_userKey = GetCurrentUserPK();
		}
	}

	return 0;
	
EFS_END
return -1;
}


BOOL CBO_LOC_3100::SetCenterUserInfo()
{
EFS_BEGIN


	if(_T("1") == m_pLoanMangeValue->m_strLoanShare || _T("2") == m_pLoanMangeValue->m_strLoanShare)
	{
		m_UseCenterUserInfo = TRUE;
	}

	if(_T("1") == m_pLoanMangeValue->m_strUserShare || _T("2") == m_pLoanMangeValue->m_strUserShare)
	{
		m_UseCenterUserLoanLog = TRUE;
	}

	CString alias = _T("통함대출자정보보기");
	pMain->SendMessage( USER_TOOLBAR_HIDE_SHOW, ( WPARAM )&alias, !m_UseCenterUserInfo );

	return m_UseCenterUserInfo;

EFS_END
return FALSE;
}

VOID CBO_LOC_3100::SearchUser( BOOL bAskPasswd /*=TRUE*/ )
{
EFS_BEGIN

	if( bAskPasswd == TRUE )
	{	
		m_pLoan->SPFExcutePath( _T("영수증출력체크") );
		m_pLoan->SPFExcutePath(_T("회원_검색"));
	}
	else
	{
		m_pLoan->SPFExcutePath(_T("회원_검색_NOASK"));
	}
	
	
	if( m_pLoanMangeValue->m_sFeeMngSys == _T("Y") )
	{
		if( m_pFeeArrearReceipt != NULL )
		{
			ShowFeeReceipt();
		}
	}
	
EFS_END
}

CString CBO_LOC_3100::GetCurrentUserPK()
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
	if( 1 == pDM->GetRowCount() )
	{
		CString sUserKey;
		pDM->GetCellData( _T("대출자KEY"), 0, sUserKey );
		return sUserKey;
	}
	else
	{
		return _T("");
	}

EFS_END
return _T("");
}

INT  CBO_LOC_3100::ConvertToDigit(TCHAR* szNumber, INT nType)
{
EFS_BEGIN
	
    TCHAR* szTemp = szNumber;	
    INT nData = 0;
    INT nPow = 1;
    INT ii, nLen;
	
    nLen = _tcsclen(szNumber);
	
    for ( ii = (nLen-1); ii >= 0; ii-- )
    {
        switch ( nType )		
        {
        case 2:
            if ( szTemp[ii] < '0' || szTemp[ii] > '1' )
			{	
                return 0; 
			}			
            nData += (szTemp[ii] - '0') * nPow;
            nPow *= 2;
            break;
			
        case 8:
            if ( szTemp[ii] < '0' || szTemp[ii] > '7' )
			{	
                return 0; 
			}			
            nData += (szTemp[ii] - '0') * nPow;
            nPow *= 8;
            break;
			
        case 16:
            if ( (szTemp[ii] < '0' && szTemp[ii] > '9') && (szTemp[ii] < 'A' && szTemp[ii] > 'F') )
            {
                return 0; 
            }
            if ( szTemp[ii] >= '0' && szTemp[ii] <= '9' )	
            {
                nData += (szTemp[ii] - '0') * nPow;	
            }
            if ( szTemp[ii] >= 'A' && szTemp[ii] <= 'F' )
            {	
                nData += (szTemp[ii] - 'A' + 10) * nPow;
            }
			
            nPow *= 16;
            break;	
        }
    }
	
    return nData;

EFS_END
return -1;
}

VOID CBO_LOC_3100::OnbtnMSGTEST() 
{
EFS_BEGIN

	INT ids;
	CString sFileName = _T("BarCodeLog.txt");

	CStdioFile fFile;	
	ids = fFile.Open( sFileName, CFile::modeRead | CFile::shareDenyWrite | CFile::typeBinary );
	if( ids==0 )
	{
		CString sEMsg;
		sEMsg.Format( _T("[%s]파일명이 잘못되었습니다.\r\n올바른 경로인지 확인해 주십시오.") , sFileName );
		AfxMessageBox( sEMsg );
		return ;
    }	

	CString sLine, sMessage, sLParam, sWParam;
	INT nIndex1, nIndex2, nIndex3, nIndex4, nIndex5;
	INT nCount = 0;

	CWnd* pWnd = GetDlgItem(IDC_edtNUMBER);
	UINT message , wParam , lParam;
	MSG Msg;
	Msg.hwnd = pWnd->m_hWnd;
	
	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fFile.SeekToBegin();
	}
	
	while( fFile.ReadString(sLine) )
	{
		nIndex1 = sLine.Find(_T(":"));
		nIndex2 = sLine.Find(_T("["),nIndex1+1);
		nIndex3 = sLine.Find(_T(":"),nIndex2+1);
		nIndex4 = sLine.Find(_T("pMsg"),nIndex3+1);
		nIndex5 = sLine.Find(_T(":"),nIndex4+1);
		
		sMessage = sLine.Mid( nIndex1+1,nIndex2-nIndex1-1);sMessage.TrimLeft(); sMessage.TrimRight();
		sLParam = sLine.Mid( nIndex3+1 , nIndex4-nIndex3 -1);sLParam.TrimLeft(); sLParam.TrimRight();
		sWParam = sLine.Mid( nIndex5+1); sWParam.TrimLeft(); sWParam.TrimRight();

		sMessage = sMessage.Mid(2);
		sLParam = sLParam.Mid(2);
		sWParam = sWParam.Mid(2);
		sMessage.MakeUpper();
		sLParam.MakeUpper();
		sWParam.MakeUpper();

		message = ConvertToDigit( sMessage.GetBuffer(0) , 16 );
		wParam = ConvertToDigit( sWParam.GetBuffer(0) , 16 );
		lParam = ConvertToDigit( sLParam.GetBuffer(0) , 16 );

		sMessage.Format( _T("%d") , message );
		sLParam.Format( _T("%d") , lParam );
		sWParam.Format( _T("%d") , wParam );

		Msg.message = message;
		Msg.lParam  = lParam;
		Msg.wParam  = wParam;

		PreTranslateMessage(&Msg);

		nCount++;
	}

	fFile.Close();	

EFS_END
}

VOID CBO_LOC_3100::OnbESC() 
{
EFS_BEGIN
	
	::PostMessage( this->m_hWnd, WM_KEYDOWN, VK_ESCAPE, 1 );

EFS_END
}

VOID CBO_LOC_3100::OnMenuitem32771() 
{
EFS_BEGIN

	m_btnBARCODE_NO_TYPE.SetWindowText(_T("번호"));	
	m_nBarcodeMode = 1;
	
	SetDefaultFocus();

EFS_END
}

VOID CBO_LOC_3100::OnMenuitem32772() 
{
EFS_BEGIN

	m_btnBARCODE_NO_TYPE.SetWindowText(_T("이용자번호"));
	m_nBarcodeMode = 2;
	
	SetDefaultFocus();

EFS_END
}


VOID CBO_LOC_3100::OnchkEXPDATA() 
{
EFS_BEGIN

	SetCheckExpDataOption();

EFS_END	
}

INT CBO_LOC_3100::SetCheckExpDataOption()
{
EFS_BEGIN

	CString sIsCheck;
	sIsCheck.Format( _T("%d"), ((CButton*)GetDlgItem(IDC_chkEXP_DATA))->GetCheck() );

	CStdioFile fFile;
	CFileException ex;
	if( fFile.Open( __EXP_DATA_IS_CHECK_FILE, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite|CFile::typeBinary, &ex ) )
	{
		if( 0 == fFile.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			fFile.Write( &cUni, sizeof(TCHAR));
		}
			
		fFile.WriteString(sIsCheck);
		fFile.Close();
	}

	if( _ttoi(sIsCheck) == 1 )
		((CComboBox*)GetDlgItem(IDC_cmbREG_NO_LIST))->EnableWindow(TRUE);
	else
		((CComboBox*)GetDlgItem(IDC_cmbREG_NO_LIST))->EnableWindow(FALSE);

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3100::SetLoanStopDateMode()
{
EFS_BEGIN
		
	CButton *pCheck = ( CButton * )GetDlgItem( IDC_cSetLoanStopDateMode );
	
	CString SetloanStopMode;
	m_pLoanMangeValue->GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("대출정지일부여여부"), SetloanStopMode );
	if( SetloanStopMode != _T("Y") )
	{
		pCheck->ShowWindow( FALSE );
		m_nLoanStopDateMode = 0;
		return;
	}
		
	CFileFind ff;
	if(!ff.FindFile(LOANSTOPDATE_SETMODE_FILEPATH))
	{ 
		FILE *fp = _tfopen(LOANSTOPDATE_SETMODE_FILEPATH, _T("w+b"));
		fputc(0xFF, fp); 
		fputc(0xFE, fp);
		_ftprintf( fp, _T("0"));
		fclose(fp);

		m_nLoanStopDateMode = 0;
		pCheck->SetCheck(FALSE);
		return;
	}
	ff.Close();

	CStdioFile fp;
	if( !fp.Open( LOANSTOPDATE_SETMODE_FILEPATH, CFile::modeRead | CFile::typeBinary ) )
	{
		m_nLoanStopDateMode = 0;
		pCheck->SetCheck(FALSE);
		return;
	}
	
	TCHAR cUni;
	fp.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fp.SeekToBegin();
	}
	
	fp.ReadString( SetloanStopMode );
	
	if(_T("1") == SetloanStopMode)
	{
		m_nLoanStopDateMode = 1;
		pCheck->SetCheck(TRUE);
	}
	else
	{
		m_nLoanStopDateMode = 0;
		pCheck->SetCheck(FALSE);
	}	
	fp.Close();
	return;

EFS_END
}

VOID CBO_LOC_3100::SetReturnCheckMode()
{
EFS_BEGIN
	
	CButton *pCheck = (CButton*)GetDlgItem( IDC_chkCHECK_RETURN );
	
	CFileFind ff;
	if( !ff.FindFile(_T("..\\cfg\\확인후반납설정.cfg")) )
	{ 
		FILE *fp = _tfopen( _T("..\\cfg\\확인후반납설정.cfg"), _T("w+b") );
		fputc(0xFF, fp); 
		fputc(0xFE, fp);
		
		m_pLoanMangeValue->m_nSetloanStopMode = 1;
		_ftprintf( fp, _T("%d"), m_pLoanMangeValue->m_nSetloanStopMode );
		pCheck->SetCheck( m_pLoanMangeValue->m_nSetloanStopMode );
		fclose( fp );
	}
	else
	{
		CString sSetReturnCheckMode;
		CStdioFile fp;
		if( !fp.Open( _T("..\\cfg\\확인후반납설정.cfg"), CFile::modeRead | CFile::typeBinary ) )
			return;
		
		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}
		
		fp.ReadString( sSetReturnCheckMode );
		m_pLoanMangeValue->m_nSetloanStopMode = _ttoi(sSetReturnCheckMode);
		pCheck->SetCheck( m_pLoanMangeValue->m_nSetloanStopMode );
		fp.Close();
	}
	ff.Close();

	return;

EFS_END
}

VOID CBO_LOC_3100::OncSetReturnCheckMode()
{
EFS_BEGIN

	FILE *fp = _tfopen( _T("..\\cfg\\확인후반납설정.cfg"), _T("w+b") );
	
	fputc(0xFF, fp);
	fputc(0xFE, fp);
		
	m_pLoanMangeValue->m_nSetloanStopMode = ((CButton*)GetDlgItem(IDC_chkCHECK_RETURN))->GetCheck();
	_ftprintf( fp, _T("%d"), m_pLoanMangeValue->m_nSetloanStopMode );
	fclose( fp );

EFS_END
}

VOID CBO_LOC_3100::OncSetLoanStopDateMode() 
{
EFS_BEGIN

	
	FILE *fp = _tfopen( LOANSTOPDATE_SETMODE_FILEPATH, _T("w+b") );	
	fputc(0xFF, fp);
	fputc(0xFE, fp);
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_cSetLoanStopDateMode))->GetCheck();
	if(TRUE == bCheck)
	{
		_ftprintf(fp, _T("1"));
		m_nLoanStopDateMode = 1;
	}
	else
	{
		_ftprintf(fp, _T("0"));
		m_nLoanStopDateMode = 0;
	}
	fclose( fp );

EFS_END
}


INT CBO_LOC_3100::OpenRegCodeList(POINT& pt)
{
EFS_BEGIN

	INT ids;
	INT nDefaultValue = 0;

	INT nCount = m_pInfo->pCodeMgr->GetCodeCount( _T("등록구분") );

	CString* strCode = new CString[nCount];
	CString* strDesc = new CString[nCount];

	nCount++;
	ids = m_pInfo->pCodeMgr->GetCode( _T("등록구분"), strCode, strDesc, nCount, nDefaultValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("OpenRegCodeList") );

	CMenu RegCodeMenu;
	RegCodeMenu.CreatePopupMenu();

	for( INT i = 0 ; i < nCount ; i++ )
	{
		RegCodeMenu.AppendMenu(MF_STRING, 5000+i, strCode[i]+_T("(")+strDesc[i]+_T(")"));
	}
		
	RegCodeMenu.TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);

	delete []strCode;
	delete []strDesc;

	return 0;

EFS_END
return -1;
}

LRESULT CBO_LOC_3100::OnRegCodeSelected(UINT  nIDC)
{
EFS_BEGIN

	INT ids;
	INT nDefaultValue = 0;
	
	INT nCount = m_pInfo->pCodeMgr->GetCodeCount( _T("등록구분") );
	
	CString* strCode = new CString[nCount];
	CString* strDesc = new CString[nCount];
	
	nCount++;
	ids = m_pInfo->pCodeMgr->GetCode( _T("등록구분") , strCode , strDesc , nCount , nDefaultValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OpenRegCodeList") );

	GetDlgItem(IDC_edtNUMBER)->SetWindowText( strCode[nIDC-5000] );
	
	SetDefaultFocus();
	((CEdit*)GetDlgItem(IDC_edtNUMBER))->SetSel(strCode[nIDC-5000].GetLength() , strCode[nIDC-5000].GetLength() );
	
	delete []strCode;
	delete []strDesc;
	
	return 0;

EFS_END
return -1;
}


VOID CBO_LOC_3100::OnMouseMovegridMAIN1(short Button, short Shift, long x, long y) 
{

}


VOID CBO_LOC_3100::OnMouseDowngridMAIN1(short Button, short Shift, long x, long y) 
{
EFS_BEGIN

	if( Button != 2 )
	{
		CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
		CString sGridAlias = _T("MainGrid");
		CESL_ControlMgr *pCM = FindCM( sCMAlias );
		if(pCM==NULL) return;
		
		CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
		if(pGrid==NULL) return;
		
		m_nMouseUpRow = pGrid->GetMouseRow();

		pGrid->m_nReverse = pGrid->GetIdx();
		m_pLoan->ViewGrid();
	}

EFS_END
}


VOID CBO_LOC_3100::OnMouseUpgridMAIN1(short Button, short Shift, long x, long y) 
{
EFS_BEGIN

	CESL_Grid* pGrid = ( CESL_Grid * )FindControl( _T("CM_BO_LOC_3100_LOAN_INFO") , _T("MainGrid") );	

	if( 1 != pGrid->GetCol() )	return;
	
	m_nMouseUpRow = pGrid->GetMouseRow();

EFS_END
}

VOID CBO_LOC_3100::SetSelectedGrid()
{
EFS_BEGIN

	if( -1 == m_nMouseDownRow || -1 == m_nMouseUpRow )
		return;
	
	CMSHFlexGrid *pGrid = ( CMSHFlexGrid * )GetDlgItem( IDC_gridMAIN1 );
	pGrid->SetRow( m_nMouseDownRow );
	pGrid->SetRowSel( m_nMouseUpRow );

	m_nMouseDownRow = -1;
	m_nMouseUpRow= -1;

EFS_END
}

INT CBO_LOC_3100::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN

	INT ids;

	
	if( strDMFieldAlias.Compare( _T("상태") ) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( str , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 ,  _T("GridDisplayEventProc")  );
	}
	else if( strDMFieldAlias.Compare( _T("부록여부") ) == 0 )
	{
		if( str == _T("A") )
			str = _T("부록");
		else
			str = _T("책");
	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3100::OnbtnSHELFLOCCODESET() 
{
EFS_BEGIN

	CBO_LOC_3195 dlg(this);
	dlg.DoModal();	

EFS_END
}

BOOL CBO_LOC_3100::Check5163System()
{
	CString strCheck;
	CString strTemp;
	strTemp = _T("..\\cfg\\MailSystem.cfg") ;
	CStdioFile file ;
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary) ) return FALSE ;
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
	
	CString strLine ;
	CString sTemp ;
		
	
	while( file.ReadString (strLine) )
	{	
		strLine.TrimLeft();
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;	
		
		INT idx;
		idx = strLine.Find(_T("["));
		if (0 <= idx)
		{
			INT nPos = strLine.Find (_T(":")) ;
			
			if(0 <= nPos) 
			{
				
				sTemp = strLine.Mid (idx+1,nPos-1) ;
				sTemp.TrimLeft();
				sTemp.TrimRight();
				if (sTemp == _T("5163")) 
				{
					strCheck = strLine.Mid(nPos+1);	
					INT nEnd = strCheck.Find (_T("]")) ;
					strCheck = strCheck.Mid (0,nEnd);
					strCheck.TrimLeft();
					strCheck.TrimRight();
				}			
			}
			break;
		}
	} 

	file.Close ();			
	if ( strCheck == _T("TRUE") ) 
	{
		return TRUE;	
	}

	return FALSE;
}

VOID CBO_LOC_3100::OnSetfocusedtNOTE() 
{
EFS_BEGIN

	CEdit *pEdit1;
	pEdit1 = (CEdit*)GetDlgItem(IDC_edtNOTE);
		
	HIMC hIME;
	DWORD dwConversion, dwSentence;
	
	hIME = ::ImmGetContext(pEdit1->m_hWnd);
	if(hIME) 
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		dwConversion=IME_CMODE_HANGUL;
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		ImmReleaseContext(pEdit1->m_hWnd,hIME); 
	} 

EFS_END		
}

INT CBO_LOC_3100::PrintScreen()
{
EFS_BEGIN

	INT ids;
	CString sMsg;	
	
	
	CString sUserDMAlias  = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr* pUserDM = FindDM( sUserDMAlias );
	if ( pUserDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") ); 

	INT nRowCnt;
	nRowCnt = pUserDM->GetRowCount();
	if ( nRowCnt < 1 ) 
	{
		sMsg.Format( _T("출력할 대출자가 없습니다.") );
		m_pLoan->SelfCloseMessageBox( sMsg );
		return 1;
	}	
	
	if(PrivacyPrintLogRecord(pUserDM, _T("대출/반납 화면출력"), _T("E") ,0 ) < 0 ) return 1;
	
	CString sUserName;                
	CString sUserNum;                 
	CString sUserClassNo;             
	CString sUserStatus;              
	CString sUserStopDay;        	  
	CString sUserLastModDay;     	  
	CString sUserLoanBookCnt;    	  
	CString sUserReservationCnt;      
	CString sUserRemark;              
	
	ids = pUserDM->GetCellData( _T("대출자이름"), 0, sUserName );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	ids = pUserDM->GetCellData( _T("대출자번호"), 0, sUserNum );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	ids = pUserDM->GetCellData( _T("번호"), 0, sUserClassNo );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	ids = pUserDM->GetCellData( _T("회원상태설명"), 0, sUserStatus );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	ids = pUserDM->GetCellData( _T("대출정지일"), 0, sUserStopDay );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	ids = pUserDM->GetCellData( _T("최종수정일"), 0, sUserLastModDay );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	ids = pUserDM->GetCellData( _T("총대출책수"), 0, sUserLoanBookCnt );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	ids = pUserDM->GetCellData( _T("총예약책수"), 0, sUserReservationCnt );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	ids = pUserDM->GetCellData( _T("비고"), 0, sUserRemark );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -10 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
		
	CString sUserAddress;
	ids = pUserDM->GetCellData( _T("대출자주소"), 0, sUserAddress );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
	
	CString sUserClass;
	ids = pUserDM->GetCellData( _T("대출자직급"), 0, sUserClass );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -18 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
		
	ids = m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("대출직급정보"), sUserClass, sUserClass );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
	
	CString sUserPhone;
	ids = pUserDM->GetCellData( _T("자택전화"), 0, sUserPhone );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -20 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
	
	CString sUserHP;
	ids = pUserDM->GetCellData( _T("휴대폰"), 0, sUserHP );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
	
	CString sSrcDMAlias  = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pSrcDM = FindDM( sSrcDMAlias );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );

	nRowCnt = pSrcDM->GetRowCount();
	if( nRowCnt < 1 )
	{
		sMsg.Format( _T("대출이력이 없습니다. 출력하시겠습니까?") );
		if ( IDNO == MessageBox( sMsg, NULL, MB_YESNO|MB_ICONQUESTION ) )
			return 1;
	}

	CString sPrintDMAlias  = _T("DM_BO_LOC_3100_LOAN_INFO_PRINT");
	CESL_DataMgr* pPrintDM = FindDM( sPrintDMAlias );
	if( pPrintDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -12 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
	pPrintDM->FreeData();

	const INT nCopyCnt=15;
	
	TCHAR *sField[nCopyCnt] =
	{
		_T("등록번호"),		_T("본표제"),	_T("대출일"),	_T("반납예정일"),
		_T("예약일"),		_T("연체일수"),	_T("대출상태"),	_T("자료실"),
		_T("관리구분"),		_T("매체구분"),	_T("대출형태"),	_T("청구기호"),
		_T("예약만기일"),	_T("번호"),		_T("비고")
	};

	CString sData;							
	CString sDelayCnt;						
	
	for ( INT i = 0 ; i < nRowCnt ; i++ )
	{
		pPrintDM->InsertRow(-1);
		
		sDelayCnt.Empty();

		for( INT j = 0 ; j < nCopyCnt - 2; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j] , i , sData );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -14 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
				
			if ( _tcscmp( sField[j], _T("연체일수") ) == 0 )
				sDelayCnt = sData;		
		    else if ( _tcscmp( sField[j], _T("대출상태") ) == 0 )
				GetStatusDesc( sData, sDelayCnt, &sData );
									
			ids = pPrintDM->SetCellData( sField[j] , sData , i );
			if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
		}
	
		
		sData.Format( _T("%d"), i + 1 );
		ids = pPrintDM->SetCellData( sField[j] , sData , i );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
		
		ids = pPrintDM->SetCellData( sField[j + 1] , sUserRemark , i );
		if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -22 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
	}

	
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pPrintDM , _T("관리구분") , _T("관리구분") );
	
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	CString strRPTAlias;
	if ( m_pLoanMangeValue->m_sIs90lib == _T("Y") )	strRPTAlias = _T("대출화면출력_국방대");
	else										strRPTAlias = _T("대출화면출력");

	pSIReportManager->SetCONNECTION_INFO( pPrintDM->CONNECTION_INFO );

	ids = pSIReportManager->GetSIReportFileFromDB( strRPTAlias);
	if ( ids < 0 ) 
	{
		sMsg.Format( _T("DB로 부터 SI Report 정보를 가져오지 못하였습니다.") );
		AfxMessageBox( sMsg );
		if( pSIReportManager )
			delete pSIReportManager;
		return 1;
	}

	pSIReportManager->SetDataMgr( 0, pPrintDM , 0) ;
	
	ids = pSIReportManager->SetTempVariable( _T("대출자이름")   , sUserName );
	ids = pSIReportManager->SetTempVariable( _T("대출자번호")   , sUserNum );
	ids = pSIReportManager->SetTempVariable( _T("교번")         , sUserClassNo );
	ids = pSIReportManager->SetTempVariable( _T("회원상태")     , sUserStatus );
	ids = pSIReportManager->SetTempVariable( _T("대출정지일")   , sUserStopDay );
	ids = pSIReportManager->SetTempVariable( _T("최종수정일")   , sUserLastModDay );
	ids = pSIReportManager->SetTempVariable( _T("대출책수")     , sUserLoanBookCnt );
	ids = pSIReportManager->SetTempVariable( _T("예약책수")     , sUserReservationCnt );
	ids = pSIReportManager->SetTempVariable( _T("비고")         , sUserRemark );
	ids = pSIReportManager->SetTempVariable( _T("자택주소")     , sUserAddress );
	ids = pSIReportManager->SetTempVariable( _T("대출직급정보") , sUserClass );
	ids = pSIReportManager->SetTempVariable( _T("자택전화번호") , sUserPhone );
	ids = pSIReportManager->SetTempVariable( _T("핸드폰")       , sUserHP );
	
	CString sLoanTotalCnt;
	sLoanTotalCnt.Format( _T("%d") , nRowCnt );
	ids = pSIReportManager->SetTempVariable( _T("총계") , sLoanTotalCnt );
	ids = pSIReportManager->Print();
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -16 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
	if (pSIReportManager) 
	{
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	CESL_DataMgr *pDM = FindDM( _T("DM_MNG_USER_MANAGER_PRINT_LOG") );
	std::vector<CString> list;

	CString strKey;
	ids = pUserDM->GetCellData(_T("대출자KEY"), 0, strKey );
	if ( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -25 , _T("CBL_LOC_LOAN_PROC::PrintScreen()") );
	list.push_back(strKey);
	
	CPersonInfo::SetPrintLog(this, pDM, PERINFO_EXCEL, m_pInfo, _T("대출반납화면출력"), pUserDM->GetRowCount(), &list);
	list.clear();

	return 0;
	
EFS_END
	return -1;
}


INT CBO_LOC_3100::GetStatusDesc(CString sStatusCode, CString sDelayCnt, CString *psGetDesc)
{
EFS_BEGIN

	
	if ( sStatusCode == _T("0") )
	{
		if ( _ttoi( sDelayCnt ) > 0 )	
			psGetDesc->Format( _T("연체") );
		else
			psGetDesc->Format( _T("대출") );
	}
	else if ( sStatusCode == _T("2") )      
		psGetDesc->Format( _T("반납연기") );	
	else if ( sStatusCode == _T("3") )
		psGetDesc->Format( _T("예약") );
	else
		psGetDesc->Format( _T("") );

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3100::OnbTESTLOAN() 
{	
	CStdioFile	fFile;	
	CString		strLine;
	CString		sTemp;
	INT			idx;

	FILE *fpTime;
	fpTime = _tfopen( _T("..\\tunning\\대출수행시간.txt"), _T("a+b") );	

	
	fseek(fpTime, 0, SEEK_END);		
	if( 0 == ftell(fpTime) )
	{
		fputc(0xFF, fpTime);
		fputc(0xFE, fpTime);
	}
			
	DWORD start_total, end_total;
	start_total = GetTickCount();
	
	DWORD start_app, end_app;
	CString strMsg;		
	INT		nProcCount = 0;

	if ( !fFile.Open(_T("..\\tunning\\대출TEST.TXT"), CFile::modeRead | CFile::typeBinary ) )
	{
		return;
	}
	
	CString strItem[2];
	MSG	pMsg;
	pMsg.message = WM_KEYDOWN;
	pMsg.wParam = 13;
	
	TCHAR cUni;
	fFile.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fFile.SeekToBegin();
	}
	while(fFile.ReadString (strLine))
	{
		start_app = GetTickCount();
		strLine.TrimLeft();
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;		
		if (strLine.Left(1) == _T("[") || strLine.IsEmpty()) continue;		

		idx = strLine.Find(_T(":"));
		if (0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if (sTemp == _T("이름")) 
			{					
				strItem[0] = strLine.Mid(idx+1);
				strItem[0].TrimLeft();
				strItem[0].TrimRight();
				(( CEdit*) GetDlgItem (IDC_edtNAME) )->SetWindowText(strItem[0]);
				(( CEdit*) GetDlgItem (IDC_edtNAME) )->SetFocus();
				pMsg.hwnd = GetDlgItem(IDC_edtNAME)->m_hWnd;
				PreTranslateMessage(&pMsg);
				end_app = GetTickCount();
				strMsg.Format(_T("%s:%s \t %d.%03d\n"), sTemp, strItem[0], (end_app-start_app)/1000, (end_app-start_app)%1000);	
			} 
			if (sTemp == _T("번호")) 
			{					
				strItem[1] = strLine.Mid(idx+1);
				strItem[1].TrimLeft();
				strItem[1].TrimRight();
				(( CEdit*) GetDlgItem (IDC_edtNUMBER) )->SetWindowText(strItem[1]);
				
				SetDefaultFocus();
				pMsg.hwnd = GetDlgItem(IDC_edtNUMBER)->m_hWnd;
				PreTranslateMessage(&pMsg);
				end_app = GetTickCount();
				strMsg.Format(_T("%s:%s \t %d.%03d\n"), sTemp, strItem[1], (end_app-start_app)/1000, (end_app-start_app)%1000);	
			} 
		}
		
		_ftprintf( fpTime, strMsg );		
	}
	fFile.Close ();	
	
	end_total = GetTickCount();
	strMsg.Format(_T("전체검색시간 \t %d.%03d\n=========================================\n"), (end_total-start_total)/1000, (end_total-start_total)%1000);	
	_ftprintf( fpTime, strMsg );		
	fclose(fpTime);
}


INT CBO_LOC_3100::SendOneSMS()
{	
	if ( m_pLoanMangeValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanMangeValue->m_sLoanUserAccessMode == _T("1") )
	{
		AfxMessageBox(_T("SMS 발송 불가능한 권한입니다.") );
		return -1;
	}

	INT		ids , nDMRowCnt;
	CString		strUserKey , strUserName , strPhoneNumber , strSMSUseYN ,  strResultMsg;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SendOneSMS") );
	}

	nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		m_pLoan->SelfCloseMessageBox( _T("이용자를 먼저 선택해 주십시요") );
		return 0;
	}
	
	ids		=	pDM->GetCellData( _T("SMS수신여부") , 0 , strSMSUseYN );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SendOneSMS") );
	}

	if( _T("Y") != strSMSUseYN )
	{
		m_pLoan->SelfCloseMessageBox( _T("이용자가 SMS Service 수신을 신청하지 않았습니다.") );
		return 0;
	}

	ids		=	pDM->GetCellData( _T("대출자KEY") , 0 , strUserKey );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SendOneSMS") );
	}

	ids		=	pDM->GetCellData( _T("대출자이름") , 0 , strUserName );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SendOneSMS") );
	}

	ids		=	pDM->GetCellData( _T("휴대폰") , 0 , strPhoneNumber );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("SendOneSMS") );
	}	
	
	CESL_DataMgr *pSMSEnvDM = FindDM( _T("DM_SMSSendDlg") );
	
	CString strQuery , strSMSSystemInfo , strDivideYN;
	strQuery=_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
			 _T("WHERE CLASS_ALIAS='SMS시스템정보' AND MANAGE_CODE=UDF_MANAGE_CODE");
	pSMSEnvDM->GetOneValueQuery(strQuery, strSMSSystemInfo);
		
	strQuery.Format( _T("SELECT MIN(SEND_DIVIDE_YN) ")
					 _T("FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE='%s'"), strSMSSystemInfo );
	pSMSEnvDM->GetOneValueQuery( strQuery, strDivideYN );

	CSMSPreviewDlg SMSDlg( this , TRUE );
	SMSDlg.SetInfo( strUserKey , _T("") , strUserName, _T("") , _T("") , strPhoneNumber, _T("SMS09") , strDivideYN );
	ids	=	SMSDlg.DoModal();
	if( 0 == ids )
	{
		
		strResultMsg.Format( _T("메시지를 전송하였습니다.") );
		m_pLoan->SelfCloseMessageBox( strResultMsg );
	}
	else if( 1010 == ids )
	{
		
		strResultMsg.Format( _T("80Byte를 초과하여 [%d건]의 문자로 나누어 전송하였습니다.") , SMSDlg.m_nDividCnt );
		m_pLoan->SelfCloseMessageBox( strResultMsg );
	}
	else if( 1020 == ids )
	{
		
		strResultMsg	=	_T("80Byte를 초과하여 80Byte의 메시지만 전송하였습니다.");
		m_pLoan->SelfCloseMessageBox( strResultMsg );
	}
	else if( 1030 == ids )
	{
		strResultMsg	=	_T("이용자의 핸드폰번호가 입력되지 않았습니다.");
		m_pLoan->SelfCloseMessageBox( strResultMsg );
	}
	else
	{
	}
	
 	return 0;
}

void CBO_LOC_3100::OnbtnUserBasketCLEAR() 
{
EFS_BEGIN

	ClearUserBasket();

EFS_END
return;
}

INT CBO_LOC_3100::ClearUserBasket()
{
EFS_BEGIN

	return m_pLoan->SPFExcutePath(_T("이용자검색바구니초기화"));

EFS_END
return -1;
}

INT	CBO_LOC_3100::SetCtrlPosition(INT cx, INT cy)
{
EFS_BEGIN
	
	const int nChangeableWidthCtrlCnt = 6;
	
	INT nChangeableWidthCtrlID[nChangeableWidthCtrlCnt] = {
		IDC_STATIC_CONFIG, IDC_STATIC_ALL,
		IDC_GROUP_GUIDE_LINE7, /*메인 그리드 캡션용*/
		IDC_GROUP_GUIDE_LINE8, /*메인 그리드용*/
		IDC_GROUP_GUIDE_LINE10, /*반납통 그리드용*/
		IDC_GROUP_GUIDE_LINE12
	};

	for( INT i=0; i<nChangeableWidthCtrlCnt; i++ )
	{
		CWnd* pWnd = GetDlgItem(nChangeableWidthCtrlID[i]);
		if( NULL == pWnd )					return -1;
		if( NULL == pWnd->GetSafeHwnd() )	return -1;

		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		
		pWnd->MoveWindow(1, rc.top, cx-2, rc.Height());
	}
	
	
	CWnd* pGuide7Wnd = GetDlgItem(IDC_GROUP_GUIDE_LINE7);
	if( NULL == pGuide7Wnd )				return -1;
	if( NULL == pGuide7Wnd->GetSafeHwnd() )	return -1;

	CWnd* pGuide8Wnd = GetDlgItem(IDC_GROUP_GUIDE_LINE8);
	if( NULL == pGuide8Wnd )				return -1;
	if( NULL == pGuide8Wnd->GetSafeHwnd() )	return -1;

	CWnd* pGuide10Wnd = GetDlgItem(IDC_GROUP_GUIDE_LINE10);
	if( NULL == pGuide10Wnd )				return -1;
	if( NULL == pGuide10Wnd->GetSafeHwnd() )	return -1;

	CWnd* pGuide11Wnd = GetDlgItem(IDC_GROUP_GUIDE_LINE11);
	if( NULL == pGuide11Wnd )				return -1;
	if( NULL == pGuide11Wnd->GetSafeHwnd() )	return -1;

	CWnd* pGuide12Wnd = GetDlgItem(IDC_GROUP_GUIDE_LINE12);
	if( NULL == pGuide12Wnd )				return -1;
	if( NULL == pGuide12Wnd->GetSafeHwnd() )	return -1;

	CRect rcGuide7;
	CRect rcGuide8;
	CRect rcGuide10;
	CRect rcGuide11;
	CRect rcGuide12;
	
	pGuide7Wnd->GetWindowRect(rcGuide7);
	pGuide8Wnd->GetWindowRect(rcGuide8);
	pGuide10Wnd->GetWindowRect(rcGuide10);
	pGuide11Wnd->GetWindowRect(rcGuide11);
	pGuide12Wnd->GetWindowRect(rcGuide12);

	ScreenToClient(rcGuide7);
	ScreenToClient(rcGuide8);
	ScreenToClient(rcGuide10);
	ScreenToClient(rcGuide11);
	ScreenToClient(rcGuide12);
	
	INT nGuideLine11StartPosY = cy - rcGuide11.Height();

	if( FALSE == this->m_bInitSplitter )
	{
		
	}
	pGuide11Wnd->MoveWindow(0, nGuideLine11StartPosY, rcGuide11.Width(), rcGuide11.Height());
	pGuide10Wnd->MoveWindow(0, nGuideLine11StartPosY, rcGuide10.Width(), rcGuide10.Height());
	pGuide12Wnd->MoveWindow(0, nGuideLine11StartPosY+(rcGuide8.top-rcGuide7.top), rcGuide12.Width(), rcGuide12.Height());

	CWnd* pMainGridWnd = GetDlgItem(IDC_gridMAIN1);
	if( NULL == pMainGridWnd )					return -1;
	if( NULL == pMainGridWnd->GetSafeHwnd() )	return -1;

	
	CWnd* pGridGroupWnd = GetDlgItem(IDC_STATIC_GRID_GROUP);
	if( NULL == pGridGroupWnd )					return -1;
	if( NULL == pGridGroupWnd->GetSafeHwnd() )	return -1;

	
	CWnd* pLoanListCapWnd = GetDlgItem(IDC_STATIC_LOAN_LIST);
	if( NULL == pLoanListCapWnd )					return -1;
	if( NULL == pLoanListCapWnd->GetSafeHwnd() )	return -1;

	
	CWnd* pSta3100Control = GetDlgItem(IDC_sta3100Control);
	if( NULL == pSta3100Control )					return -1;
	if( NULL == pSta3100Control->GetSafeHwnd() )	return -1;
	CWnd* pRad3100SetControl = GetDlgItem(IDC_rad3100SetControl);
	if( NULL == pRad3100SetControl )				return -1;
	if( NULL == pRad3100SetControl->GetSafeHwnd() )	return -1;
	CWnd* pRad3100ClearControl = GetDlgItem(IDC_rad3100ClearControl);
	if( NULL == pRad3100ClearControl )					return -1;
	if( NULL == pRad3100ClearControl->GetSafeHwnd() )	return -1;
	CRect rcSta3100Control;
	CRect rcRad3100SetControl;
	CRect rcRad3100ClearControl;
	pSta3100Control->GetWindowRect(rcSta3100Control);	
	pRad3100SetControl->GetWindowRect(rcRad3100SetControl);
	pRad3100ClearControl->GetWindowRect(rcRad3100ClearControl);

	CWnd* pMain2GridWnd = GetDlgItem(IDC_gridMAIN2);
	if( NULL == pMain2GridWnd )					return -1;
	if( NULL == pMain2GridWnd->GetSafeHwnd() )	return -1;
	
	CWnd* pGrid2GroupWnd = GetDlgItem(IDC_STATIC_GRID_GROUP2);
	if( NULL == pGrid2GroupWnd )				return -1;
	if( NULL == pGrid2GroupWnd->GetSafeHwnd() )	return -1;
	
	CWnd* pReturnListCapWnd = GetDlgItem(IDC_STATIC_RETURN_LIST);
	if( NULL == pReturnListCapWnd )					return -1;
	if( NULL == pReturnListCapWnd->GetSafeHwnd() )	return -1;

	CRect rcMainGrid;
	CRect rcLoanListCapWnd;
	CRect rcReturnListCapWnd;
	
	pMainGridWnd->GetWindowRect(rcMainGrid);	
	pLoanListCapWnd->GetWindowRect(rcLoanListCapWnd);
	pReturnListCapWnd->GetWindowRect(rcReturnListCapWnd);	
	
	ScreenToClient(rcLoanListCapWnd);
	ScreenToClient(rcReturnListCapWnd);

	INT	nMainGridStartPosX = rcGuide8.left;
	INT nMainGridWidth = rcGuide8.Width();

	if( m_bVisibleBasket )
	{
		CWnd* pWnd = GetDlgItem(IDC_GROUP_GUIDE_LINE2);
		if( NULL == pWnd )					return -1;
		if( NULL == pWnd->GetSafeHwnd() )	return -1;

		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		nMainGridStartPosX = rc.left;
		nMainGridWidth    -= rc.left;
	
		CWnd* pBasketGridWnd = GetDlgItem(IDC_gridUSER_BASKET);
		if( NULL == pBasketGridWnd )				return -1;
		if( NULL == pBasketGridWnd->GetSafeHwnd() )	return -1;		
		
		CWnd* pBasketStaticWnd = GetDlgItem(IDC_STATIC_BASKET);
		if( NULL == pBasketStaticWnd )					return -1;
		if( NULL == pBasketStaticWnd->GetSafeHwnd() )	return -1;
			
		CWnd* pClearBtnWnd = GetDlgItem(IDC_btnCLEAR);
		if( NULL == pClearBtnWnd )					return -1;
		if( NULL == pClearBtnWnd->GetSafeHwnd() )	return -1;		
		
		CWnd* pEditMaxCntWnd = GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT);
		if( NULL == pEditMaxCntWnd )					return -1;
		if( NULL == pEditMaxCntWnd->GetSafeHwnd() )		return -1;

		CWnd* pBtnMaxCntWnd = GetDlgItem(IDC_BUTTON_USER_BASKET_MAX_COUNT);
		if( NULL == pBtnMaxCntWnd )					return -1;
		if( NULL == pBtnMaxCntWnd->GetSafeHwnd() )	return -1;

		CWnd* pStaticMaxCntWnd1 = GetDlgItem(IDC_STATIC_USER_BASKET_MAX_COUNT1);
		if( NULL == pStaticMaxCntWnd1 )					return -1;
		if( NULL == pStaticMaxCntWnd1->GetSafeHwnd() )	return -1;

		CWnd* pStaticMaxCntWnd2 = GetDlgItem(IDC_STATIC_USER_BASKET_MAX_COUNT2);
		if( NULL == pStaticMaxCntWnd2 )					return -1;
		if( NULL == pStaticMaxCntWnd2->GetSafeHwnd() )	return -1;
		
		CRect rBasketGrid , rEditMaxCnt , rBtnMaxCnt , rStaticMaxCnt1 , rStaticMaxCnt2;
		pBtnMaxCntWnd->GetWindowRect( rBtnMaxCnt );
		ScreenToClient( rBtnMaxCnt );
		
		CWnd* pSearchWnd = GetDlgItem(IDC_STATIC_SEARCH);
		if( NULL == pSearchWnd )				return -1;
		if( NULL == pSearchWnd->GetSafeHwnd() )	return -1;

		pSearchWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		
		CRect rfid_rc;
		CWnd* pRfidBtn = GetDlgItem(IDC_btnUSE_RFID);
		pRfidBtn->GetWindowRect(&rfid_rc);
		ScreenToClient(&rfid_rc);
		pBasketGridWnd->MoveWindow(rc.left, rcGuide8.top, rc.Width()-(rfid_rc.Width()*2), cy-rcGuide8.top-( rBtnMaxCnt.Height()+10 ) );
		pBasketStaticWnd->MoveWindow(rc.left, rcGuide7.top, rc.Width()-(rfid_rc.Width()*2), rcGuide8.top-rcGuide7.top-3);
		
		CWnd* pReturnTypeWnd = GetDlgItem(IDC_cmbRETURN_TYPE);
		if( NULL == pReturnTypeWnd )				return -1;
		if( NULL == pReturnTypeWnd->GetSafeHwnd() )	return -1;
		
		CRect rcClearBtn;
		CRect rcpReturnTypeWnd;
			
		pClearBtnWnd->GetWindowRect(rcClearBtn);
		pReturnTypeWnd->GetWindowRect(rcpReturnTypeWnd);

		ScreenToClient(&rcpReturnTypeWnd);

		INT nClearBtnStartPosX = rcpReturnTypeWnd.right - rcClearBtn.Width();
		INT nClearBtnStartPosY = rcGuide7.top + 8;
		
		pClearBtnWnd->MoveWindow(nClearBtnStartPosX, nClearBtnStartPosY, rcClearBtn.Width(), rcClearBtn.Height());
		
		INT nPosX , nPosY;

		pBasketGridWnd->GetWindowRect( rBasketGrid );
		ScreenToClient( rBasketGrid );
		
		nPosX	=	rcpReturnTypeWnd.right - rBtnMaxCnt.Width();
		nPosY	=	rBasketGrid.bottom + 5;
		pBtnMaxCntWnd->MoveWindow( nPosX , nPosY , rBtnMaxCnt.Width() , rBtnMaxCnt.Height() );		
		
		pStaticMaxCntWnd2->GetWindowRect( rStaticMaxCnt2 );
		ScreenToClient( rStaticMaxCnt2 );
		nPosX	-=	( rStaticMaxCnt2.Width());
		pStaticMaxCntWnd2->MoveWindow( nPosX , nPosY+4 , rStaticMaxCnt2.Width() , rStaticMaxCnt2.Height() );
				
		pEditMaxCntWnd->GetWindowRect( rEditMaxCnt );
		ScreenToClient( rEditMaxCnt );
		nPosX	-=	( rEditMaxCnt.Width() + 3 );
		pEditMaxCntWnd->MoveWindow( nPosX , nPosY , rEditMaxCnt.Width() , rEditMaxCnt.Height() );

		
		pStaticMaxCntWnd1->GetWindowRect( rStaticMaxCnt1 );
		ScreenToClient( rStaticMaxCnt1 );
		nPosX	-=	( rStaticMaxCnt1.Width() + 3 );
		pStaticMaxCntWnd1->MoveWindow( nPosX , nPosY+4 , rStaticMaxCnt1.Width() , rStaticMaxCnt1.Height() );
		
		CRect	rcReturnGrid , rcReturnGroup , rcReturnCaption;

		pMain2GridWnd->GetWindowRect(rcReturnGrid);
		ScreenToClient(rcReturnGrid);
		rcReturnGrid.left		=	nMainGridStartPosX;

		pGrid2GroupWnd->GetWindowRect(rcReturnGroup);
		ScreenToClient(rcReturnGroup);
		rcReturnGroup.left		=	nMainGridStartPosX;

		pReturnListCapWnd->GetWindowRect(rcReturnCaption);
		ScreenToClient(rcReturnCaption);
		rcReturnCaption.left	=	nMainGridStartPosX;

		m_splitterBar.SetCtrlPosition(&rcReturnGroup,&rcReturnGrid,&rcReturnCaption);

		pSta3100Control->MoveWindow(nMainGridStartPosX+180, rcGuide7.top+13, rcSta3100Control.Width(), rcSta3100Control.Height());
		pRad3100SetControl->MoveWindow(nMainGridStartPosX+255, rcGuide7.top+11, rcRad3100SetControl.Width(), rcRad3100SetControl.Height());
		pRad3100ClearControl->MoveWindow(nMainGridStartPosX+305, rcGuide7.top+11, rcRad3100ClearControl.Width(), rcRad3100ClearControl.Height());
	}
	
	else {
		pSta3100Control->MoveWindow(nMainGridStartPosX+270, rcGuide7.top+13, rcSta3100Control.Width(), rcSta3100Control.Height());
		pRad3100SetControl->MoveWindow(nMainGridStartPosX+345, rcGuide7.top+11, rcRad3100SetControl.Width(), rcRad3100SetControl.Height());
		pRad3100ClearControl->MoveWindow(nMainGridStartPosX+395, rcGuide7.top+11, rcRad3100ClearControl.Width(), rcRad3100ClearControl.Height());
	}

	pMainGridWnd->MoveWindow(nMainGridStartPosX, rcGuide8.top, nMainGridWidth, cy-rcGuide8.top-rcGuide11.Height());
	pGridGroupWnd->MoveWindow(nMainGridStartPosX, rcGuide7.top, nMainGridWidth, rcGuide8.top-rcGuide7.top-3);
	pLoanListCapWnd->MoveWindow(nMainGridStartPosX+10, rcGuide7.top+13, rcLoanListCapWnd.Width(), rcLoanListCapWnd.Height());

	
	CRect rcGrid2Group , rcMain2Grid , rcReturnListCap;
	if( rcGuide8.top <= nGuideLine11StartPosY )
	{		
		
		rcGrid2Group.left		=	nMainGridStartPosX;		
		rcGrid2Group.top		=	nGuideLine11StartPosY;
		rcGrid2Group.right		=	rcGrid2Group.left + nMainGridWidth;	
		rcGrid2Group.bottom		=	rcGrid2Group.top  +	(rcGuide12.top-rcGuide10.top-3);
		
		rcMain2Grid.left		=	nMainGridStartPosX;
		rcMain2Grid.top			=	nGuideLine11StartPosY	+	(rcGuide8.top-rcGuide7.top);
		rcMain2Grid.right		=	rcMain2Grid.left		+	nMainGridWidth;
		rcMain2Grid.bottom		=	rcMain2Grid.top			+	(rcGuide11.Height()-(rcGuide8.top-rcGuide7.top));
		
		rcReturnListCap.left	=	nMainGridStartPosX+10;
		rcReturnListCap.top		=	nGuideLine11StartPosY+13;
		rcReturnListCap.right	=	rcReturnListCap.left	+	rcReturnListCapWnd.Width();
		rcReturnListCap.bottom	=	rcReturnListCap.top		+	rcReturnListCapWnd.Height();
				
		m_splitterBar.SetCtrlPosition(&rcGrid2Group,&rcMain2Grid,&rcReturnListCap);
				
	}
	else if( rcGuide8.top > nGuideLine11StartPosY )
	{
		
		rcGrid2Group.left		=	nMainGridStartPosX;		
		rcGrid2Group.top		=	rcGuide8.bottom		+	2;
		rcGrid2Group.right		=	rcGrid2Group.left	+	nMainGridWidth;	
		rcGrid2Group.bottom		=	rcGrid2Group.top	+	(rcGuide12.top-rcGuide10.top-3);

		rcMain2Grid.left		=	nMainGridStartPosX;
		rcMain2Grid.top			=	rcGuide8.bottom		+	rcGrid2Group.Height()	+	4;
		rcMain2Grid.right		=	rcMain2Grid.left	+	nMainGridWidth;
		rcMain2Grid.bottom		=	cy;
		
		rcReturnListCap.left	=	nMainGridStartPosX	+	10;
		rcReturnListCap.top		=	rcGuide8.bottom		+	15;
		rcReturnListCap.right	=	rcReturnListCap.left+	rcReturnListCapWnd.Width();
		rcReturnListCap.bottom	=	rcReturnListCap.top	+	rcReturnListCapWnd.Height();

		m_splitterBar.SetCtrlPosition(&rcGrid2Group,&rcMain2Grid,&rcReturnListCap);
	}
	else
	{
	}

	CRect rRadio;
	pLoanListCapWnd->GetWindowRect(rRadio);
	ScreenToClient(rRadio);
		
	rRadio.OffsetRect(130, 0);
	rRadio.right = rRadio.left + 60;

	GetDlgItem(IDC_rad3100ACenter)->MoveWindow(rRadio);

	rRadio.OffsetRect(60, 0);
	GetDlgItem(IDC_rad3100TCenter)->MoveWindow(rRadio);

	
	pSta3100Control->RedrawWindow();
	pLoanListCapWnd->RedrawWindow();

	return 0;

EFS_END
return -1;
}

void CBO_LOC_3100::OnBtnShowPrevUserList() 
{
	m_bVisibleBasket = !m_bVisibleBasket;
	
	CWnd* pBtnWnd = GetDlgItem(IDC_BTN_SHOW_PREV_USER_LIST);
	
	
	if( m_bVisibleBasket )
	{
		pBtnWnd->SetWindowText(_T("▲"));		
	}
	
	else
	{
		pBtnWnd->SetWindowText(_T("▼"));	
	}
	
	const int nCtrlCount = 8;
	UINT nID[nCtrlCount] = { IDC_STATIC_USER_BASKET,			IDC_gridUSER_BASKET,
							 IDC_btnCLEAR,						IDC_STATIC_BASKET,
							 IDC_EDIT_USER_BASKET_MAX_COUNT,	IDC_BUTTON_USER_BASKET_MAX_COUNT,
							 IDC_STATIC_USER_BASKET_MAX_COUNT1,	IDC_STATIC_USER_BASKET_MAX_COUNT2 };
	

	for( INT i=0;i<nCtrlCount;i++ )
	{
		CWnd* pWnd = GetDlgItem(nID[i]);
		pWnd->ShowWindow( m_bVisibleBasket ? SW_SHOW : SW_HIDE );
	}

	CRect rc;
	this->GetClientRect(rc);
	this->SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rc.Width(), rc.Height()));
	
	SetDefaultFocus();
}


void CBO_LOC_3100::OnBtnPrevUserSearch() 
{
	CString strData;
	GetDlgItemText(IDC_EDT_PREV_USER_NAME, strData);
	if( strData.IsEmpty() )
	{
		SetDefaultFocus();
		return;
	}

	ChangePrevUser(1);
	SetDefaultFocus();
}

INT	CBO_LOC_3100::ChangePrevUser(INT nIndex)
{
	INT		ids;
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_SEARCH_USER_BASKET"));
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CBL_LOC_LOAN_PROC::ClearUserBasket()") );	

	if( 1 > pDM->GetRowCount() )
		return 0;

	CString strUserNo;
	ids = pDM->GetCellData(_T("대출자번호"), nIndex, strUserNo);
	if( strUserNo.IsEmpty() )
		return 0;

	
	CString strValue1;
	ids = m_pLoanMangeValue->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBL_LOC_LOAN_PROC::ViewLoanScreen()") );
	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();

	if( _T("N") == strValue1 )
	{
		if( -1 < strUserNo.Find(_T("*")) )
		{
			
			CString strRecKey;
			ids = pDM->GetCellData(_T("REC_KEY"), nIndex, strRecKey);
			if( strRecKey.IsEmpty() )	return 0;

			CString strQuery;
			strQuery.Format(_T("SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strRecKey);
			ids = pDM->GetOneValueQuery(strQuery, strUserNo);
			if( strUserNo.IsEmpty() )	return 0;
		}
	}	

	SetDlgItemText(IDC_edtNUMBER, strUserNo);

	return m_pLoan->SPFExcutePath(_T("회원_검색"));
}

void CBO_LOC_3100::OnDblClickgridUSERBASKET() 
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"), _T("이용자검색바구니"));
	if( 1 > pGrid->GetMouseRow() )
		return;

	ChangePrevUser( pGrid->GetIdx() );

	
	SetDefaultFocus();
}

void CBO_LOC_3100::OnChangeedtFAMILYID() 
{
	CString strFamilyID;
	CString strOldFamilyID;
	
	GetDlgItemText(IDC_edtFAMILYID, strFamilyID);	

	if( !strFamilyID.IsEmpty() )
	{
		strFamilyID = _T("( ") + strFamilyID + _T(" )");
	}

	strFamilyID = _T("  가족 정보") + strFamilyID;

	
	
	GetDlgItemText(IDC_btnFamily, strOldFamilyID);
	if ( strOldFamilyID != strFamilyID )
	{
		
		SetDlgItemText(IDC_btnFamily, strFamilyID);	
	}	
}

void CBO_LOC_3100::OnClickGridSimpleFamilyView() 
{
	
}

void CBO_LOC_3100::OnDblClickGridSimpleFamilyView() 
{
EFS_BEGIN

	CESL_DataMgr* pDM = FindDM(_T("DM_FAMILY_ID_INFO"));
	if( NULL == pDM )	return;

	if( 1 > pDM->GetRowCount() )	return;

	CMSHFlexGrid* pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_GRID_SIMPLE_FAMILY_VIEW);
	if( NULL == pGrid ) return;

	
	CString strUserNo;
	pDM->GetCellData(_T("대출자번호"), pGrid->GetMouseRow(), strUserNo);
	if(strUserNo.IsEmpty())
	{
		CString strUserKey;
		pDM->GetCellData(_T("대출자KEY"), pGrid->GetMouseRow(), strUserKey);
		if(strUserKey.IsEmpty()) return;
		strUserKey.TrimLeft(); strUserKey.TrimRight();
		
		m_pLoan->SPFExcutePath(_T("영수증출력체크"));
		m_pLoan->SetChangeMemberKey(strUserKey);
		m_pLoan->SPFExcutePath(_T("대출자관리_회원변경"));	
	}
	else
	{
		m_pLoan->SPFExcutePath( _T("영수증출력체크") );
		SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strUserNo);
		CString strAskPwdYN;
		m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("가족회원비밀번호입력여부"), strAskPwdYN);
		strAskPwdYN.TrimLeft();		strAskPwdYN.TrimRight();
		strAskPwdYN.MakeUpper();
		
 		if(strAskPwdYN.IsEmpty() || strAskPwdYN == _T("Y"))
		{
			m_pLoan->SPFExcutePath(_T("회원_검색"));
		}
		else
		{
			m_pLoan->SPFExcutePath(_T("회원_검색_NOASK"));
		}
	}
	
	if( m_pLoanMangeValue->m_sFeeMngSys == _T("Y") )
	{
		if( m_pFeeArrearReceipt != NULL )
		{
			ShowFeeReceipt();
		}
	}
	SetDefaultFocus();


EFS_END
}

void CBO_LOC_3100::OnChangeedtNOTE() 
{

}

INT CBO_LOC_3100::SetRemarkControl(BOOL bShow)
{
	return 0;
}

HBRUSH CBO_LOC_3100::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	
	if(IDC_staticPRIVATEMANAGE == pWnd->GetDlgCtrlID())
    {
        pDC->SetTextColor(RGB(255,0,0));
        pDC->SetBkMode(TRANSPARENT);
    }

	return hbr;
}

void CBO_LOC_3100::OnButton1() 
{
	CBarcodeFlowSettingDlg dlg(this);
	dlg.DoModal();
}

INT CBO_LOC_3100::ShowBarcodeFlowSetting()
{
	CBarcodeFlowSettingDlg dlg(this);
	dlg.DoModal();

	return 0;
}


void CBO_LOC_3100::OnChangeedtNAME() 
{
	
}

VOID CBO_LOC_3100::SetUserName()
{
	
	CString strName, strMsg;
	GetDlgItemText(IDC_edtNAME, strName);
	strName.TrimLeft();	strName.TrimRight();
	
	if ( m_pLoanMangeValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanMangeValue->m_sLoanUserAccessMode == _T("1") ) 
		strName = _T("***");
	
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem( IDC_TAB_USER_INFO );

	if( strName.IsEmpty() )
	{
		TC_ITEM item;
		item.mask		=	TCIF_TEXT;
		item.pszText	=	_T("이용자 정보");
		pTab->SetItem( 0 , &item );
	}
	else
	{
		TC_ITEM item;
		item.mask		=	TCIF_TEXT;
		strMsg.Format(_T("[ %s ]님 간략 정보"),strName);
		item.pszText	=	strMsg.GetBuffer(0);
		pTab->SetItem( 0 , &item );
	}
}

BOOL CBO_LOC_3100::OnEraseBkgnd(CDC* pDC) 
{
 	const int nInvalidateCtrlCnt = 3;
 	INT nInvalidateCtrlID[nInvalidateCtrlCnt] = {
 		IDC_GRID_SIMPLE_FAMILY_VIEW, IDC_gridMAIN1,
 		IDC_gridUSER_BASKET
 	};
 
 	for( INT i=0; i<nInvalidateCtrlCnt; i++ )
 	{
 		CWnd* pWnd = GetDlgItem(nInvalidateCtrlID[i]);
 		if( NULL != pWnd )
 		{
 			if( NULL != pWnd->GetSafeHwnd() )
 				pWnd->Invalidate();
 		}
 	}	
 
 	return CDialog::OnEraseBkgnd(pDC);
}

VOID CBO_LOC_3100::ShowFeeReceipt() 
{	
	CString sUserDMAlias  = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr* pUserDM = FindDM( sUserDMAlias );
	if ( pUserDM == NULL ) return;

	
	CString sQuery, sCount;
	CString strUserNo;
	CString strRecKey;

	pUserDM->GetCellData( _T("대출자번호"), 0, strUserNo );
	pUserDM->GetCellData( _T("대출자KEY"), 0, strRecKey );
	if(!strUserNo.IsEmpty() && !strRecKey.IsEmpty())
	{
		sQuery.Format(	_T("SELECT (SELECT COUNT(1) ")
						_T("FROM CO_FEE_PAYMENT_INFO_TBL CFPI, LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU ")
						_T("WHERE CFPI.LOAN_INFO_KEY=CL.REC_KEY AND CLU.REC_KEY=CL.USER_KEY ")
						_T("AND CFPI.STATUS='N' AND CLU.REC_KEY=%s)")
						_T("+")
						_T("(SELECT COUNT(1) ")
						_T("FROM CO_FEE_PAYMENT_INFO_TBL CFPI, CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU ")
						_T("WHERE CFPI.LOAN_INFO_KEY=CL.REC_KEY AND CLU.REC_KEY=CL.USER_KEY ")
						_T("AND CFPI.STATUS='N' AND CLU.REC_KEY=%s) FROM DUAL")
						, strRecKey, strRecKey);
		pUserDM->GetOneValueQuery( sQuery , sCount );
		if( _ttoi(sCount) > 0 )
		{
			
			INT nRowCnt;
			nRowCnt = pUserDM->GetRowCount();
			if ( nRowCnt > 0 ) 
			{
				m_pFeeArrearReceipt->ShowWindow( SW_SHOWNOACTIVATE );					
				m_pFeeArrearReceipt->AddUser(strRecKey, strUserNo);
			}
		}
	}
}

VOID CBO_LOC_3100::OnbtnLoanStopDateCancel()
{
EFS_BEGIN

	INT ids;
	ids = m_pLoan->SPFExcutePath( _T("대출정지일해제") );

EFS_END
}

LRESULT CBO_LOC_3100::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	try
	{
		
		if( WM_3100_SPLITTER_MOVED == message)
		{
			
			SetControlGuideLine();
			
			OnShowWindow(SW_SHOW,NULL);
			Invalidate(TRUE);
		}
	}
	catch (CException *e)
	{ 

	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CBO_LOC_3100::OnPaint() 
{
EFS_BEGIN

	CPaintDC dc(this); 

	if (!GetSafeHwnd()) 
	{
		return;
	}

	if (!m_bShowPic)
	{
		return;
	}
	
	if (!m_pBmp)
	{
		enum
		{
			PROFILE_TYPE_MAN = 0,
			PROFILE_TYPE_WOMAN,
			PROFILE_TYPE_UNKNOWN,
		};

		int nProfileType = PROFILE_TYPE_UNKNOWN;
	
		
		BOOL bCheckCiviNo = (g_strCivilNoFlag.CompareNoCase(_T("Y")) == 0);
		if (bCheckCiviNo)
		{
			CString strCivilNo;
			GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("주민등록번호"), strCivilNo, 0);
			
			strCivilNo.TrimLeft();
			strCivilNo.TrimRight();
			strCivilNo.Replace(_T("-"), _T(""));

			
			if (strCivilNo.GetLength() == 13)
			{
				int nSex = _ttoi(strCivilNo.Mid(6,1));
				switch (nSex)
				{
				case 1: 
				case 3: 
				case 5: 
				case 7: 
				case 9: 
					nProfileType = PROFILE_TYPE_MAN;
					break;
				case 2: 
				case 4: 
				case 6: 
				case 8: 
				case 0: 
					nProfileType = PROFILE_TYPE_WOMAN;
					break;
				}
			}

			if (nProfileType == PROFILE_TYPE_UNKNOWN)
			{
				
				bCheckCiviNo = FALSE;
			}		
		}

		
		if (!bCheckCiviNo)
		{
			CString strGpinSex;
			GetDataMgrData(_T("DM_BO_LOC_3300"), _T("G-PIN성별"), strGpinSex, 0);

			if		(strGpinSex.CompareNoCase(_T("0")) == 0)	nProfileType = PROFILE_TYPE_MAN;
			else if (strGpinSex.CompareNoCase(_T("1")) == 0)	nProfileType = PROFILE_TYPE_WOMAN;
		}

		m_pBmp = new PLWinBmp;
		switch (nProfileType)
		{
		case PROFILE_TYPE_MAN:		m_Decoder.MakeBmpFromFile ("..\\BMP\\profile_man.jpg", m_pBmp);		break;
		case PROFILE_TYPE_WOMAN:	m_Decoder.MakeBmpFromFile ("..\\BMP\\profile_woman.jpg", m_pBmp);	break;
		case PROFILE_TYPE_UNKNOWN:	m_Decoder.MakeBmpFromFile ("..\\BMP\\profile_empty.JPG", m_pBmp);	break;
		}
	}

	CRect rc;
	GetDlgItem(IDC_STATIC_USER_PICTURE)->GetWindowRect(rc);
	ClientToScreen(rc);
		
	PLFilterResizeBilinear Filter(rc.Width()-2, rc.Height()-2);
	Filter.ApplyInPlace(m_pBmp); 

	CDC* pDC = GetDlgItem(IDC_STATIC_USER_PICTURE)->GetDC();
	m_pBmp->Draw(pDC->m_hDC, 1, 1);
	ReleaseDC(pDC);

	if (GetDlgItem(IDC_picUnityUserIcon))	GetDlgItem(IDC_picUnityUserIcon)->Invalidate(FALSE);

	if (m_btnCertify_Y.IsWindowVisible())		m_btnCertify_Y.Invalidate(FALSE);
	if (m_btnCertify_N.IsWindowVisible())		m_btnCertify_N.Invalidate(FALSE);
	if (m_btnCertify_Fail.IsWindowVisible())	m_btnCertify_Fail.Invalidate(FALSE);

EFS_END
}


VOID CBO_LOC_3100::InitTabUserInfo()
{
	CRect rect;
	rect.top=0;
	rect.left=0;
	rect.right=0;
	rect.bottom=0;

	m_userInfo_Simple = new CBO_LOC_3100_USER_INFO_SIMPLE(this, m_pLoan);	
	m_userInfo_Simple->Create(IDD_BO_LOC_3100_USER_INFO_SIMPLE, this);

	m_userInfo_Addition	=	new	CBO_LOC_3100_USER_INFO_ADDITION(this);
	m_userInfo_Addition->m_strUserAccessAuth = m_pLoanMangeValue->m_sLoanUserAccessMode;
	m_userInfo_Addition->Create( IDD_BO_LOC_3100_USER_INFO_ADDITION , this );

	m_tabUserInfo.InsertItem( 0 , _T("이용자 정보") );
	m_tabUserInfo.InsertItem( 1 , _T("부가 정보") );

	CRect rectStatic, rectNote;

	GetDlgItem(IDC_STATIC_REMARK_POS)->GetWindowRect(rectStatic);
	m_userInfo_Simple->GetDlgItem(IDC_EDIT_NOTE)->GetWindowRect(rectNote);

	
	
	m_userInfo_Simple->m_pRemarkDlg->SetWindowPos(NULL,rectStatic.left,rectStatic.top,rectNote.Width()+40,rectNote.Height()*3,NULL);
	
	m_tabUserInfo.SetCurSel(0);
	ShowTabWindow(0);
}


void CBO_LOC_3100::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	INT nPos = m_tabUserInfo.GetCurSel();
	ShowTabWindow(nPos);

	if(m_BalloonTip.GetSafeHwnd())
	{
		m_BalloonTip.Hide();
	}

	CDialog::OnShowWindow(bShow, nStatus);	
}


void CBO_LOC_3100::OnSelchangeTabUserInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ShowTabWindow( m_tabUserInfo.GetCurSel() );	
	*pResult = 0;
}


VOID CBO_LOC_3100::ShowTabWindow(INT nIndex)
{
	CRect	rect;
	CRect	rectDlg;

	if(-1 == nIndex) nIndex = m_tabUserInfo.GetCurSel();

	CWnd* pWnd = (CWnd*)GetDlgItem( IDC_TAB_USER_INFO );
	if(NULL != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		rect = CRect(0, 0, 0, 0);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		if( NULL !=	m_userInfo_Simple )
		{
			m_userInfo_Simple->GetClientRect(rectDlg);
			
			rect.left	= rect.left + 101;
			rect.top	= rect.top + 20;		
			rect.bottom	= rect.top + rectDlg.Height();
		}		
	}

	CESL_ControlMgr *pCM;
	CString	sCMAlias = _T("");

	if(nIndex == 0)
	{ 
		m_userInfo_Simple->SetWindowPos( NULL , rect.left , rect.top , rect.Width() , rect.Height() , SWP_SHOWWINDOW);
		m_userInfo_Addition->ShowWindow( FALSE );

		sCMAlias = _T("CM_BO_LOC_3100_USER_INFO_SIMPLE");
		pCM	= m_userInfo_Simple->FindCM( sCMAlias );
		if( pCM == NULL )	return;
		
		
		pCM->AllControlDisplay(0);

		INT ids;
		CString strValue1;
		ids = m_pLoanMangeValue->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1 );
		if( 0>ids ) ERROR_MSG_RETURN_VOID2( -5 , _T("ShowTabWindow") );
		strValue1.TrimLeft();strValue1.TrimRight();
		strValue1.MakeUpper();

		if( _T("Y") != strValue1 )
		{
			CString strUserNo;
								
			ids = pCM->GetControlMgrData( _T("대출자번호"), strUserNo );
			if( 0>ids ) ERROR_MSG_RETURN_VOID2( -5 , _T("ShowTabWindow") );

			CLocCommonAPI::ChangeSecuriyUserNo( this , strUserNo , m_pLoanMangeValue->m_sIsUnityLoanService );
			ids = pCM->SetControlMgrData( _T("대출자번호"), strUserNo );
			if( 0>ids ) ERROR_MSG_RETURN_VOID2( -5 , _T("ShowTabWindow") );
		}

		m_userInfo_Simple->UpdateWindow();
	} 
	else if(nIndex == 1)
	{ 
		m_userInfo_Addition->SetWindowPos( NULL , rect.left , rect.top , rect.Width() , rect.Height() , SWP_SHOWWINDOW);
		m_userInfo_Simple->ShowWindow( FALSE );

		sCMAlias = _T("CM_BO_LOC_3100_USER_INFO_ADDITION");
		pCM		=	m_userInfo_Addition->FindCM( sCMAlias );
		if( pCM == NULL )	return;
		
		pCM->AllControlDisplay(0);

		
		CString strRemark;
		CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(NULL==pDM)  return;
		pDM->GetCellData(_T("비고"), 0, strRemark);
		m_userInfo_Simple->m_pRemarkDlg->SetRemarkText(strRemark);

		m_userInfo_Addition->UpdateWindow();
	} 
	else
	{
		return;
	}

	
	{
		CString strRemark;		

	}
	
}


VOID CBO_LOC_3100::MoveUserInfoCtrl()
{
	GetDlgItem(IDC_btnFamily)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_edtFAMILYID)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_FAMILYID)->ShowWindow(SW_HIDE);
	
	
	
	GetDlgItem(IDC_STATIC_FAMILY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_GRID_SIMPLE_FAMILY_VIEW)->ShowWindow(SW_HIDE);

	int nResourceID[] = {
		IDC_picUnityUserIcon,		IDC_btnARREARINFO,
		IDC_btn3100Certify_Y,		IDC_btn3100Certify_N,		IDC_btn3100Certify_Fail,
		IDC_TAB_USER_INFO,			IDC_btnDETAILINFO,
		IDC_STATIC_REMARK_POS,		IDC_STATIC_USER_PICTURE,	IDC_STATIC_USER_INFO_LINE,
		IDC_staticPRIVATEMANAGE,	IDC_btnSTATEMENT,
		0
	};

	CRect rc;
	CRect rcFamily;
	GetDlgItem(IDC_STATIC_FAMILY)->GetClientRect(rcFamily);

	int idx = -1;
	while (nResourceID[++idx] > 0)
	{
		GetDlgItem(nResourceID[idx])->GetWindowRect(rc);
		ScreenToClient(rc);
		rc.OffsetRect(-rcFamily.Width()-4,0);

		GetDlgItem(nResourceID[idx])->MoveWindow(rc);
		GetDlgItem(nResourceID[idx])->RedrawWindow();
	}

	CRect rcNote, rcNoteXPos;

	m_pNote->GetWindowRect(rcNote);
	ScreenToClient(rcNote);

	GetDlgItem(IDC_STATIC_REMARK_POS)->GetWindowRect(rcNoteXPos);
	ScreenToClient(rcNoteXPos);

	rcNote.left	= rcNoteXPos.left - 20;
	rcNote.right = rcNote.left + 380;
	m_pNote->MoveWindow(rcNote);
}


INT CBO_LOC_3100::ShowInfo()
{
EFS_BEGIN

	if( !m_bShowPic ) return 0;
	
	ViewUserPic();
	return 0;

EFS_END
return -1;
}


INT CBO_LOC_3100::ViewUserPic()
{
EFS_BEGIN

	MakePictureFileFromDBLOBField();	
	InitPictureControl();				

	return 0;
	
EFS_END
return -1;
}


#define MAX_BLOB_BUFFER 20000000
INT CBO_LOC_3100::MakePictureFileFromDBLOBField()
{
EFS_BEGIN

	INT ids;
	
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakePictureFileFromDBLOBField") );
	
	if( pDM->GetRowCount() == 0 )
	{
		m_sImageFilePath = _T("");
		return 1;
	}
	
	CString sTableName = _T("CO_LOAN_USER_TBL");	
	CString sRecKey;
	CString sFileExt;
	CString sFileName;
	
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("대출자KEY") , sRecKey , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRegNoFileFromDBLOBField()") );
	if( sRecKey.IsEmpty() ) return 1;
	
	sFileName = _T("../대출증/이용자사진/");
	
	ids = GetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("사진종류") , sFileExt , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRegNoFileFromDBLOBField()") );

	CString strFileSize;
	if (!sFileExt.IsEmpty())
	{
		CString strQuery;
		strQuery.Format(_T("SELECT LENGTH(USER_PICTURE) FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sRecKey);
		pDM->GetOneValueQuery(strQuery, strFileSize);
	}	

	if (sFileExt.IsEmpty() || strFileSize.IsEmpty() || m_pInfo->MODE == 10000 )
	{
		enum
		{
			PROFILE_TYPE_MAN = 0,
			PROFILE_TYPE_WOMAN,
			PROFILE_TYPE_UNKNOWN,
		};

		int nProfileType = PROFILE_TYPE_UNKNOWN;

		
		BOOL bCheckCiviNo = (g_strCivilNoFlag.CompareNoCase(_T("Y")) == 0);
		if (bCheckCiviNo)
		{
			CString strCivilNo;
			GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("주민등록번호"), strCivilNo, 0);
			
			strCivilNo.TrimLeft();
			strCivilNo.TrimRight();
			strCivilNo.Replace(_T("-"), _T(""));

			
			if (strCivilNo.GetLength() == 13)
			{
				int nSex = _ttoi(strCivilNo.Mid(6,1));
				switch (nSex)
				{
				case 1: 
				case 3: 
				case 5: 
				case 7: 
				case 9: 
					nProfileType = PROFILE_TYPE_MAN;
					break;
				case 2: 
				case 4: 
				case 6: 
				case 8: 
				case 0: 
					nProfileType = PROFILE_TYPE_WOMAN;
					break;
				}
			}

			if (nProfileType == PROFILE_TYPE_UNKNOWN)
			{
				
				bCheckCiviNo = FALSE;
			}
		}

		
		if (!bCheckCiviNo)
		{
			CString strGpinSex;
			GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("G-PIN성별"), strGpinSex, 0);

			if		(strGpinSex.CompareNoCase(_T("0")) == 0)	nProfileType = PROFILE_TYPE_MAN;
			else if (strGpinSex.CompareNoCase(_T("1")) == 0)	nProfileType = PROFILE_TYPE_WOMAN;
		}

		switch (nProfileType)
		{
		case PROFILE_TYPE_MAN:		m_sImageFilePath = _T("..\\BMP\\profile_man.jpg");		break;
		case PROFILE_TYPE_WOMAN:	m_sImageFilePath = _T("..\\BMP\\profile_woman.jpg");	break;
		case PROFILE_TYPE_UNKNOWN:	m_sImageFilePath = _T("..\\BMP\\profile_empty.JPG");	break;
		}

		return 1;
	}
		
	_tmkdir(_T("../대출증"));
	_tmkdir(_T("../대출증/이용자사진"));
	
	sFileName += sRecKey + _T(".") + sFileExt;
	
	ids = SetDataMgrData( _T("DM_BO_LOC_3100_MEMBER_INFO") , _T("사진경로") , sFileName , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeRegNoFileFromDBLOBField()") );
	
	CFileFind ff;
	if (ff.FindFile(sFileName))
	{
		ff.FindNextFile();
		if (_ttoi(strFileSize) == ff.GetLength())
		{
			m_sImageFilePath = sFileName;
			ff.Close();
			return 0;
		}
	}
	ff.Close();

	
	BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
	
	INT nSize;
	ids = pDM->LOBGet( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey , _T("USER_PICTURE") , pBuffer , &nSize );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("MakeRegNoFileFromDBLOBField") );
	
	
	if(0 >= nSize)
	{
		m_sImageFilePath = sFileName;
		free( pBuffer );
		return 0;
	}

	CFile fd;
	if (fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) == NULL) {
		AfxMessageBox(_T("이용자 관리 Tmp File Open Error~"));
		free( pBuffer );
		return 1;
	}
	
	fd.WriteHuge(pBuffer, nSize);
	free( pBuffer );
	fd.Close();
	
	m_sImageFilePath = sFileName;
	
	return 0;
	
EFS_END
return -1;	
}


INT CBO_LOC_3100::InitPictureControl()
{
EFS_BEGIN
		
	INT ids;
	
	CString m_sImageFileExt;
	
	if( m_sImageFilePath.IsEmpty() )
	{
		if( m_pBmp != NULL )
			delete m_pBmp;
		m_pBmp = NULL;
		OnPaint();
		return 0;
	}
	
	CFile fFile;
	ids = fFile.Open( m_sImageFilePath , CFile::modeRead | CFile::typeBinary );
	if( ids == 0 )
	{
		if( m_pBmp != NULL )
			delete m_pBmp;
		m_pBmp = NULL;
		OnPaint();
		return 1;
	}
	fFile.Close();
	
	if( m_pBmp == NULL )
		m_pBmp = new PLWinBmp;

	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;
	
	cv.UnicodeToMultiByte( m_sImageFilePath.GetBuffer(0), &szTmp, &len );
	if( 0 < len)	
	{
		m_Decoder.MakeBmpFromFile (szTmp, m_pBmp);
		delete [] szTmp;
	}	
		
	OnPaint();
	
	return 0;
	
EFS_END
return -1;
}



INT CBO_LOC_3100::CodeChangeInDM()
{
EFS_BEGIN

	INT ids;
	
	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CodeChangeInDM()") );

	CString sData, sDesc;
	ids = pDM->GetCellData("관리구분", 0, sData);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CodeChangeInDM()") ); 
	this->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("관리구분"), sData, sDesc );

	ids = pDM->SetCellData( _T("관리구분설명"), sDesc, 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CodeChangeInDM()") ); 
	
	CString sCodeValue = _T("");
	ids = pDM->GetCellData( _T("회원구분") , 0 , sCodeValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CodeChangeInDM()") ); 

	if( sCodeValue.Compare(_T("0"))==0 )
		sCodeValue = _T("정회원");
	else if( sCodeValue.Compare(_T("1"))==0 )
		sCodeValue = _T("비회원");
	else if( sCodeValue.Compare(_T("2"))==0 )
		sCodeValue = _T("준회원");

	ids = pDM->SetCellData( _T("회원구분설명") ,  sCodeValue , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CodeChangeInDM()") ); 

	return 0;

EFS_END
return -1;
}


INT CBO_LOC_3100::SetControlGuideLine()
{
EFS_BEGIN
	
	CWnd* pGuide11Wnd	=	GetDlgItem(IDC_GROUP_GUIDE_LINE11);
	if( NULL == pGuide11Wnd )					return -1;
	if( NULL == pGuide11Wnd->GetSafeHwnd() )	return -1;

	CRect rc3100, rcSplitter, rcGuide11;
	
	this->GetWindowRect( &rc3100 );
	ScreenToClient( &rc3100 );	
	
	m_splitterBar.GetWindowRect( &rcSplitter );
	ScreenToClient( &rcSplitter );	
	
	pGuide11Wnd->GetWindowRect( &rcGuide11 );
	ScreenToClient( &rcGuide11 );
	
	rcGuide11.top		=	rcSplitter.bottom;
	rcGuide11.bottom	=	rc3100.bottom;
	pGuide11Wnd->MoveWindow( &rcGuide11 );
	
	return	0;

EFS_END
return	-1;
}

void CBO_LOC_3100::OnButtonUserBasketMaxCount() 
{
	CString sBtnName;
	CString strMaxCount;
	((CButton*)GetDlgItem(IDC_BUTTON_USER_BASKET_MAX_COUNT))->GetWindowText(sBtnName);
	if(sBtnName == _T("수정"))
	{ 
		((CButton*)GetDlgItem(IDC_BUTTON_USER_BASKET_MAX_COUNT))->SetWindowText(_T("저장"));
		GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT)->GetWindowText(strMaxCount);
		INT nLen = strMaxCount.GetLength();
		((CEdit*)GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT))->SetSel(0, nLen);
		GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT)->SetFocus();
	}
	else
	{ 
		GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT)->GetWindowText(strMaxCount);
		INT nMaxCnt = _ttoi(strMaxCount);
		if( nMaxCnt < 1 || nMaxCnt > 500 )
		{
			AfxMessageBox( _T("1에서 500사이의 정수를 입력하십시오.") );
			strMaxCount.Format(_T("%d"), m_pLoanMangeValue->m_nUserBasketCnt);
			GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT)->SetWindowText(strMaxCount);
			INT nLen = strMaxCount.GetLength();
			((CEdit*)GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT))->SetSel(0, nLen);
			GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT)->SetFocus();
			return;
		}

		((CButton*)GetDlgItem(IDC_BUTTON_USER_BASKET_MAX_COUNT))->SetWindowText(_T("수정"));
		GetDlgItem(IDC_EDIT_USER_BASKET_MAX_COUNT)->EnableWindow(FALSE);
		
		
		m_pLoanMangeValue->m_nUserBasketCnt = nMaxCnt;
		SaveUserBasketMaxCount( strMaxCount );	
	}
}

VOID CBO_LOC_3100::GetUserBasketMaxCount()
{
	CString		strMaxCount;
	CFileFind ff;
	if(ff.FindFile(_T("..\\cfg\\이전이용자목록.cfg")))
	{
		CStdioFile fp;
		if(fp.Open(_T("..\\cfg\\이전이용자목록.cfg"), CFile::modeRead | CFile::typeBinary ))
		{
			TCHAR cUni;
			fp.Read(&cUni, sizeof(TCHAR));
			if(0xFEFF != cUni )
			{
				fp.SeekToBegin();
			}
			fp.ReadString( strMaxCount );
			m_nUserBasketCnt = _ttoi( strMaxCount );
			fp.Close();

			m_pLoanMangeValue->m_nUserBasketCnt	=	m_nUserBasketCnt;
		}
	}
	ff.Close();
}

VOID CBO_LOC_3100::SaveUserBasketMaxCount(CString strMaxCount)
{
	CFile fFile;
	if( fFile.Open( _T("..\\cfg\\이전이용자목록.cfg") , CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ) )
	{		
		TCHAR cUni = 0xFEFF;			
		fFile.Write( &cUni, sizeof(TCHAR));
				
		TCHAR *tData = new TCHAR[strMaxCount.GetLength()+1];
		_tcscpy(tData, strMaxCount.GetBuffer(0));
		tData[strMaxCount.GetLength()] = NULL;
		fFile.Write(tData, strMaxCount.GetLength()*sizeof(TCHAR));
		TRACE(_T("%s\r\n"), tData);
		delete []tData;
		tData = NULL;
		fFile.Close();
	}
}

void CBO_LOC_3100::OnchkBEEP() 
{
	BOOL bCheck;

	bCheck = (( CButton*) GetDlgItem (IDC_chkBEEP))->GetCheck();

	if ( m_pLoan != NULL ) 
	{
		if ( bCheck == TRUE ) 
		{
			m_pLoan->m_bBeep = TRUE;
		}
		else
		{
			m_pLoan->m_bBeep = FALSE;
		}
		SetDefaultFocus();	
	}
		
}

void CBO_LOC_3100::OnchkSimpleGrid() 
{
	BOOL bCheck;

	bCheck = (( CButton*) GetDlgItem (IDC_chkSimpleGrid))->GetCheck();

	if ( m_pLoan != NULL ) 
	{
		if ( bCheck == TRUE ) 
		{
			m_pLoan->m_bSimpleGrid = TRUE;
		}
		else
		{
			m_pLoan->m_bSimpleGrid = FALSE;
		}

		SetDefaultFocus();
	}	
}

void CBO_LOC_3100::OnbtnTESTLoan() 
{
	DWORD nSTART , nFINISH;
	CString strTestlog;
	
	nSTART = GetTickCount();
	LoanReturnTest();
	nFINISH = GetTickCount();
	strTestlog.Format(_T("LOOP Tatal Time \t%d.%d------------------\r\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	TestLog(strTestlog);	
	AfxMessageBox(strTestlog);	
}

void CBO_LOC_3100::OnbtnTESTSearchUser() 
{
	DWORD nSTART , nFINISH;
	CString strTestlog;
	
	nSTART = GetTickCount();
	SearchUserTest();
	nFINISH = GetTickCount();
	strTestlog.Format(_T("LOOP Tatal Time \t%d.%d------------------\r\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	TestLog(strTestlog);	
	AfxMessageBox(strTestlog);	
}

VOID CBO_LOC_3100::LoanReturnTest()
{
	DWORD nSTART , nFINISH;
	CString log;
	int nRunCount = 0;
	
	CString strUserNo, strRegNo, strTime;
	CStdioFile sf;
	if (sf.Open(_T("..\\자동테스트\\대출반납\\대출자번호.txt"), CFile::modeRead | CFile::typeBinary)) {
		TCHAR cUni;
		sf.Read(&cUni, sizeof(TCHAR));
		if (0xFEFF != cUni) {
			sf.SeekToBegin();
		}
		sf.ReadString(strUserNo);
		strUserNo.TrimLeft();
		strUserNo.TrimRight();
		sf.Close();
	}

	if (_T("") == strUserNo) {
		AfxMessageBox(_T("대출자번호 설정 : ..\\자동테스트\\대출반납\\대출자번호.txt"));
		return;
	}
	
	if (sf.Open(_T("..\\자동테스트\\대출반납\\등록번호.txt"), CFile::modeRead | CFile::typeBinary)) {
		TCHAR cUni;
		sf.Read(&cUni, sizeof(TCHAR));
		if (0xFEFF != cUni) {
			sf.SeekToBegin();
		}
		sf.ReadString(strRegNo);
		strRegNo.TrimLeft();
		strRegNo.TrimRight();
		sf.Close();
	}

	if (_T("") == strRegNo) {
		AfxMessageBox(_T("등록번호 설정 : ..\\자동테스트\\대출반납\\등록번호.txt"));
		return;
	}

	if (sf.Open(_T("..\\자동테스트\\대출반납\\동작시간.txt"), CFile::modeRead | CFile::typeBinary)) {
		TCHAR cUni;
		sf.Read(&cUni, sizeof(TCHAR));
		if (0xFEFF != cUni) {
			sf.SeekToBegin();
		}
		sf.ReadString(strTime);
		strTime.TrimLeft();
		strTime.TrimRight();
		sf.Close();
	}
	
	if (_T("") == strTime || 1 > _ttoi(strTime)) {
		AfxMessageBox(_T("동작시간 설정 : ..\\자동테스트\\대출반납\\동작시간.txt"));
		return;
	}

	CTime tCurrentTime;
	CTime tEndTime = CTime::GetCurrentTime() + CTimeSpan(0, 0, _ttoi(strTime), 0); 
	
	while(TRUE) {
		nRunCount++;
		
		m_pLoan->SPFExcutePath(_T("화면초기화"));
		
		( (CEdit*) GetDlgItem (IDC_edtNUMBER) )->SetWindowText(strUserNo);
		Sleep(300);
		nSTART = GetTickCount();
		OnbtnNUMBERFIND2();
		nFINISH = GetTickCount();
		Sleep(300);
		log.Format(_T("이용자검색\t%d\t%s\t%d.%d\r\n"), nRunCount, strUserNo,(nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		TestLog(log);
		
		( (CEdit*) GetDlgItem (IDC_edtNUMBER) )->SetWindowText(strRegNo);
		Sleep(300);
		nSTART = GetTickCount();
		OnbtnNUMBERFIND2();
		nFINISH = GetTickCount();
		Sleep(300);
		log.Format(_T("대출\t%d\t%s\t%d.%d\r\n"), nRunCount, strUserNo,(nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		TestLog(log);
		Sleep(1000);
		
		m_pLoan->SPFExcutePath(_T("화면초기화"));
		
		( (CEdit*) GetDlgItem (IDC_edtNUMBER) )->SetWindowText(strUserNo);
		Sleep(300);
		nSTART = GetTickCount();
		OnbtnNUMBERFIND2();
		nFINISH = GetTickCount();
		Sleep(300);
		log.Format(_T("이용자검색\t%d\t%s\t%d.%d\r\n"), nRunCount, strUserNo,(nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		
		( (CEdit*) GetDlgItem (IDC_edtNUMBER) )->SetWindowText(strRegNo);
		Sleep(300);
		nSTART = GetTickCount();
		OnbtnNUMBERFIND2();
		nFINISH = GetTickCount();
		Sleep(300);
		log.Format(_T("반납\t%d\t%s\t%d.%d\r\n"), nRunCount, strUserNo,(nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		TestLog(log);

		Sleep(1000);

		tCurrentTime = CTime::GetCurrentTime();
		if (tCurrentTime > tEndTime) break;
	}

}

VOID CBO_LOC_3100::SearchUserTest()
{
	static int sProcessCnt = 0;
	CStringArray strarrayUserNo;
	CStringArray strarrayRegNo;
	CString strLine;

	strarrayUserNo.RemoveAll();
	strarrayUserNo.FreeExtra();
	CStdioFile sf;
	if ( sf.Open(_T("..\\자동테스트\\대출반납\\대출자번호.txt"), CFile::modeRead | CFile::typeBinary ) )
	{
		TCHAR cUni;
		sf.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni )
		{
			sf.SeekToBegin();
		}

		while ( sf.ReadString(strLine) )
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			strarrayUserNo.Add(strLine);
		}
		sf.Close();
	}

	strarrayRegNo.RemoveAll();	
	strarrayRegNo.FreeExtra();
	if ( sf.Open(_T("..\\자동테스트\\대출반납\\등록번호.txt"), CFile::modeRead | CFile::typeBinary ) )
	{
		TCHAR cUni;
		sf.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni )
		{
			sf.SeekToBegin();
		}

		while ( sf.ReadString(strLine) )
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			strarrayRegNo.Add(strLine);
		}
		sf.Close();
	}

	CString strLoanCount;
	CString strTotalCount;
	CStringArray strArrayLoanRegNo;
	CString strTestlog;
	INT nLoanConut;
	INT nLoanRemainCount;
	INT nTotalLoanCount;
	INT nTotalLoanRemainCount;
	INT offset;
	INT nPossibleLoanCount;
	INT nBookIndex;
	INT i,y;

	nBookIndex = 0;
	DWORD nSTART , nFINISH;
	for ( i=0 ; i<strarrayUserNo.GetSize() ; i++ )
	{		
		
		( (CEdit*) GetDlgItem (IDC_edtNUMBER) )->SetWindowText(strarrayUserNo.GetAt(i));

		nSTART = GetTickCount();
		OnbtnNUMBERFIND2();
		nFINISH = GetTickCount();

		strTestlog.Format(_T("이용자검색\t%d\t%s\t%d.%d\r\n"), sProcessCnt, strarrayUserNo.GetAt(i),(nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		TestLog(strTestlog);
	}

	sProcessCnt += 1;
}

VOID CBO_LOC_3100::TestLog(CString log)
{
EFS_BEGIN		
	CTime t = CTime::GetCurrentTime();
	CString filename;
	filename.Format(_T("..\\자동테스트\\대출반납_%04d%02d%02d.log"), t.GetYear(), t.GetMonth(), t.GetDay());
		
	FILE* fp;
	
	fp = _tfopen (filename, _T("a+b"));
	if (fp == NULL)
	{
		return ;
	}
	
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}

	_ftprintf (fp, _T("%s"), log);
	
	fclose(fp);
	
EFS_END
}

LRESULT CBO_LOC_3100::OnSearchUserFromKey(WPARAM wParam, LPARAM lParam)
{
	CString *strResult = (CString*)lParam;
	m_pLoan->SearchUserBookFromKey(*strResult, (BOOL)wParam, TRUE);

	return 0;
}

LRESULT CBO_LOC_3100::OnUnityUserIcon(WPARAM wParam, LPARAM lParam)
{
	BOOL bShow = (BOOL)wParam;
	
	if(_T("Y") == m_pLoan->m_pManageValue->m_sIsUnityLoanService)
	{
		GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(bShow);
	}
	else
	{
		GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(FALSE);
	}

	return 0;
}

LRESULT CBO_LOC_3100::OnUserInit(WPARAM wParam, LPARAM lParam)
{
	SetUserName();
	OnChangeedtFAMILYID();
	
	
	if( m_UseCenterUserInfo )
	{
		if( NULL != m_pUnionUserInfoViewer )
		{				
			if( m_pUnionUserInfoViewer->IsWindowVisible() )
			{
				m_pUnionUserInfoViewer->SearchCenterUserInfo(_T(""));
			}
			else
			{
				m_pUnionUserInfoViewer->m_userKey = GetCurrentUserPK();
			}
		}
	}
	
	CESL_ControlMgr* pCM;
	pCM		=	m_userInfo_Simple->FindCM(_T("CM_BO_LOC_3100_USER_INFO_SIMPLE"));
	if(NULL != pCM)	pCM->AllControlClear();
	pCM	=	m_userInfo_Addition->FindCM(_T("CM_BO_LOC_3100_USER_INFO_ADDITION"));
	if(NULL != pCM)	pCM->AllControlClear();

	
	
	m_userInfo_Simple->m_pRemarkDlg->SetRemarkText( _T("") );


	return 0;
}

LRESULT CBO_LOC_3100::OnSearchUser(WPARAM wParam, LPARAM lParam)
{
	CString strUserNo = *(CString*)lParam;
	if(FALSE == strUserNo.IsEmpty())
	{
		SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strUserNo);
		SetTimer(WM_USER+1028, 100, NULL);
	}
	return 0;
}

LRESULT CBO_LOC_3100::ReceiptChangeTab(WPARAM wParam, LPARAM lParam)
{
	m_pLoan->ReceiptChangeTab(wParam, lParam);

	return 0;
}

BOOL CBO_LOC_3100::SetLibCodeControl(CString strLibCode, CString strLibName)
{
	if(TRUE == m_pLoan->IsNumericText(strLibCode))
	{ 
		
		if(strLibCode == m_pInfo->LIB_CODE)
		{ 
			if(strLibName.IsEmpty())
			{
				strLibName = m_pInfo->LIB_NAME;
			}
		}
		else
		{ 
			if(strLibName.IsEmpty())
			{
				SetDlgItemText(IDC_EDT_LIBCODE, _T("조회중입니다."));
				CFrameWriter Writer(m_pInfo);
				Writer.setCommand(_T("IL_K16_LOC_B01_SERVICE"));
				Writer.addRecord();
				Writer.addElement(_T("도서관부호"), strLibCode);
				CFrameReader Reader;
				CFrameSender Sender(m_pInfo);
				Sender.CommMiddleWareDaemon(&Writer, &Reader);
				CESL_DataMgr tmpDM;
				tmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
				Reader.makeDM(&tmpDM);
				tmpDM.GetCellData(_T("도서관명"), 0, strLibName);
				tmpDM.FreeData();
			}
			else
			{
				strLibName = strLibName;
			}
		}
		CString strValue;
		if(strLibName.IsEmpty())
		{
			strValue.Format(_T("%s(알수없는 도서관)"), strLibCode);
		}
		else
		{
			strValue.Format(_T("%s(%s)"), strLibCode, strLibName);
		}
		SetDlgItemText(IDC_EDT_LIBCODE, strValue);
		m_pLoan->m_strReturnBookLibCode = strLibCode;

		return TRUE;
	}

	return FALSE;
}

VOID CBO_LOC_3100::ClearLibCodeControl()
{
	SetDlgItemText(IDC_EDT_LIBCODE, _T(""));
	m_pLoan->m_strReturnBookLibCode = _T("");
}

void CBO_LOC_3100::OnBtnLibcode() 
{
	if( _T("Y") == m_pLoanMangeValue->m_sIsUnityLoanService )
	{
		CLibListDlg dlg(this, m_pLoan);
		if(IDOK == dlg.DoModal())
		{
			SetLibCodeControl(dlg.m_strLibCode, dlg.m_strLibName);
		}
	}
	else if (0 == g_strPrevRegNoFromOther.Compare(_T("Y")) )
	{
		CGroupLibListDlg dlg(this, m_pLoan);
		if(IDOK == dlg.DoModal())
		{
			SetLibCodeControl(dlg.m_strLibCode, dlg.m_strLibName);
		}
	}

	ClearNumberCtrl();
}

void CBO_LOC_3100::OnRad3100Local() 
{
 	SearchUserLoanInfo(0);
}

void CBO_LOC_3100::OnRad3100ACenter() 
{
	SearchUserLoanInfo(1);
}

void CBO_LOC_3100::OnRad3100TCenter() 
{
	SearchUserLoanInfo(2);
}

INT CBO_LOC_3100::SearchUserLoanInfo(INT nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SearchUserLoanInfo");
	
	CString strGetData = _T("");
	m_pLoan->GetTempData(_T("기본검색범위"), strGetData);
	
	CString strMode;
	strMode.Format(_T("%d"), nMode);
	if(strMode != strGetData)
	{
		m_pLoan->SetTempData(_T("기본검색범위"), CComVariant(strMode));
		CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(pUserDM)
		{
			if(0 < pUserDM->GetRowCount())
			{
				CString strUserNo;
				ids = pUserDM->GetCellData(_T("대출자번호"), 0, strUserNo);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);				
				if(0 == nMode)
				{
					ids = m_pLoan->KL_UserLoanInfo(strUserNo);
					if(0 > ids) return ids;
				}
				else
				{
					CString sIsUnityUser;
					ids = pUserDM->GetCellData(_T("통합회원여부"), 0, sIsUnityUser);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);
					if(_T("Y") == sIsUnityUser)
					{
						ids = m_pLoan->KL_UserLoanInfo(strUserNo);
						if(0 > ids) return ids;
					}
				}
				m_pLoan->KL_UserInfoView();
			}
		}
	}

	return 0;

EFS_END
return	-1;
}

void CBO_LOC_3100::SetDefaultFocus()
{
	GetDlgItem(IDC_edtNUMBER)->SetFocus();
}

void CBO_LOC_3100::OnbtnARREARINFO() 
{
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL==pDM) return;
	if(1 > pDM->GetRowCount())
	{
		m_pLoan->SelfCloseMessageBox(_T("대출자를 먼저 선택해주십시오"));
		SetDefaultFocus();
		return;
	}
	CArrearMainDlg dlg(this);
	dlg.DoModal();
	SetDefaultFocus();
}

VOID CBO_LOC_3100::StartUserSwitch()
{
	
	return;

	if(_T("Y") != m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		
		m_pLoan->SelfCloseMessageBox(_T("책이음서비스 사용하는 도서관에서만 사용가능합니다."));
		SetDefaultFocus();
		return;
	}
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		m_pLoan->SelfCloseMessageBox(_T("센터와의 접속 시도중입니다."));
		SetDefaultFocus();
		return;
	}
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) return;
	if(1 > pDM->GetRowCount())
	{
		m_pLoan->SelfCloseMessageBox(_T("이용자를 먼저 조회해 주십시오"));
		SetDefaultFocus();
		return;
	}
	CString strUnity;
	pDM->GetCellData(_T("통합회원여부"), 0, strUnity);
	if(_T("Y") != strUnity)
	{
		m_pLoan->SelfCloseMessageBox(_T("통합회원에 대해서 사용 가능합니다."));
		SetDefaultFocus();
		return;
	}
	CString strName;
	pDM->GetCellData(_T("대출자이름"), 0, strName);
	if(strName.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("이용자의 이름이 없습니다."));
		SetDefaultFocus();
		return;
	}
	CString strUserNo;
	pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	if(strUserNo.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("대출자번호가 없습니다."));
		SetDefaultFocus();
		return;
	}
	CString strGPinHash;
	pDM->GetCellData(_T("G-PINHASH인증코드"), 0, strGPinHash);
	if(strGPinHash.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("G-PINHASH인증코드 값이 없습니다."));
		SetDefaultFocus();
		return;
	}
	CString strGPinSex;
	pDM->GetCellData(_T("G-PIN성별"), 0, strGPinSex);
	if(strGPinSex.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("G-PIN성별 값이 없습니다."));
		SetDefaultFocus();
		return;
	}
	CString strCryptoCivilNo;
	pDM->GetCellData(_T("암호화주민등록번호"), 0, strCryptoCivilNo);
	if(strCryptoCivilNo.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("암호화 주민등록번호가 없습니다."));
		SetDefaultFocus();
		return;
	}
	if(m_pInfo->LIB_CODE.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("도서관부호를 확인하십시오."));
		SetDefaultFocus();
		return;
	}
	CString strHandPhone;
	pDM->GetCellData(_T("휴대폰"), 0, strHandPhone);
	CString strTemp;
	CString strBirthYear;
	pDM->GetCellData(_T("생일"), 0, strTemp);
	if(!strTemp.IsEmpty() && 3 < strTemp.GetLength())
	{
		strBirthYear = strTemp.Left(4);
	}
	CString strUserClass;
	pDM->GetCellData(_T("회원상태"), 0, strUserClass);
	if(strUserClass.IsEmpty())
	{
		strUserClass = _T("0");
	}
	CString strMemberClass;
	pDM->GetCellData(_T("회원구분"), 0, strMemberClass);
	if(strMemberClass.IsEmpty())
	{
		strMemberClass = _T("0");
	}

	CKlUserSwitchDlg dlg(this);
	dlg.m_pLoan = m_pLoan;
	dlg.m_strUserNo = strUserNo;
	dlg.m_strUserName = strName;
	dlg.m_strGPinHash = strGPinHash;
	dlg.m_strGPinSex = strGPinSex;
	dlg.m_strCryptoCivilNo = strCryptoCivilNo;
	dlg.m_strRequestLibCode = m_pInfo->LIB_CODE;
	dlg.m_strHandPhone = strHandPhone;
	dlg.m_strUserClass = strUserClass;
	dlg.m_strMemberClass = strMemberClass;
	dlg.m_strBirthYear = strBirthYear;
	if(IDOK == dlg.DoModal())
	{
		
		m_pLoan->m_strUserNo = _T("");
		SetDlgItemText(IDC_edtNUMBER, strUserNo);
		m_pLoan->SPFExcutePath("회원_검색_NOASK");
	}
}

VOID CBO_LOC_3100::StartUserSync()
{
	if(_T("Y") != m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		
		m_pLoan->SelfCloseMessageBox(_T("책이음서비스 사용하는 도서관에서만 사용가능합니다."));
		SetDefaultFocus();
		return;
	}
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		m_pLoan->SelfCloseMessageBox(_T("센터와의 접속 시도중입니다."));
		SetDefaultFocus();
		return;
	}
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) return;
	if(1 > pDM->GetRowCount())
	{
		m_pLoan->SelfCloseMessageBox(_T("이용자를 먼저 조회해 주십시오"));
		SetDefaultFocus();
		return;
	}
	CString strUnity;
	pDM->GetCellData(_T("통합회원여부"), 0, strUnity);
	if(_T("Y") != strUnity)
	{
		m_pLoan->SelfCloseMessageBox(_T("통합회원에 대해서 사용 가능합니다."));
		SetDefaultFocus();
		return;
	}
	CString strName;
	pDM->GetCellData(_T("대출자이름"), 0, strName);
	if(strName.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("이용자의 이름이 없습니다."));
		SetDefaultFocus();
		return;
	}
	CString strUserNo;
	pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	if(strUserNo.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("대출자번호가 없습니다."));
		SetDefaultFocus();
		return;
	}
	CString strGPinHash;
	pDM->GetCellData(_T("G-PINHASH인증코드"), 0, strGPinHash);
	
	CString strGPinSex;
	pDM->GetCellData(_T("G-PIN성별"), 0, strGPinSex);
	if(strGPinSex.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("G-PIN성별 값이 없습니다."));
		SetDefaultFocus();
		return;
	}
	CString strCryptoCivilNo;
	pDM->GetCellData(_T("암호화주민등록번호"), 0, strCryptoCivilNo);


	if (g_strCivilNoFlag.CompareNoCase(_T("Y")) == 0 && strCryptoCivilNo.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("암호화 주민등록번호가 없습니다."));
 		SetDefaultFocus();
 		return;
	}
	if(m_pInfo->LIB_CODE.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("도서관부호를 확인하십시오."));
		SetDefaultFocus();
		return;
	}
	CString strHandPhone;
	pDM->GetCellData(_T("휴대폰"), 0, strHandPhone);
	CString strTemp;
	CString strBirthYear;
	pDM->GetCellData(_T("생일"), 0, strTemp);
	if(!strTemp.IsEmpty() && 3 < strTemp.GetLength())
	{
		strBirthYear = strTemp.Left(4);
	}
	CString strUserClass;
	pDM->GetCellData(_T("회원상태"), 0, strUserClass);
	if(strUserClass.IsEmpty())
	{
		strUserClass = _T("0");
	}
	CString strMemberClass;
	pDM->GetCellData(_T("회원구분"), 0, strMemberClass);
	if(strMemberClass.IsEmpty())
	{
		strMemberClass = _T("0");
	}
	
	CString strCI;
	pDM->GetCellData(_T("실명인증코드"), 0, strCI);
	
	CKlUserSyncDlg dlg(this);
	dlg.m_pLoan = m_pLoan;
	dlg.m_strUserNo = strUserNo;
	dlg.m_strUserName = strName;
	dlg.m_strGPinHash = strGPinHash;
	dlg.m_strGPinSex = strGPinSex;
	dlg.m_strCryptoCivilNo = strCryptoCivilNo;
	dlg.m_strRequestLibCode = m_pInfo->LIB_CODE;
	dlg.m_strHandPhone = strHandPhone;
	dlg.m_strUserClass = strUserClass;
	dlg.m_strMemberClass = strMemberClass;
	dlg.m_strBirthYear = strBirthYear;
	
	dlg.m_strCI = strCI;

	if(IDOK == dlg.DoModal())
	{		
		m_pLoan->m_strUserNo = _T("");
		SetDlgItemText(IDC_edtNUMBER, strUserNo);
		m_pLoan->SPFExcutePath("회원_검색_NOASK");
	}
}


VOID CBO_LOC_3100::StartUserImport()
{
	if(FALSE == m_pInfo->m_bKLUse)
	{
		
		m_pLoan->SelfCloseMessageBox(_T("책이음서비스 사용하는 도서관에서만 사용가능합니다."));
		SetDefaultFocus();
		return;
	}
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		m_pLoan->SelfCloseMessageBox(_T("센터와의 접속 시도중입니다."));
		SetDefaultFocus();
		return;
	}
	CKL_UserImportSetDlg dlg(this);
	dlg.m_pLoan = m_pLoan;
	if(IDOK == dlg.DoModal())
	{
		m_pLoan->m_strUserNo = _T("");
		SetDlgItemText(IDC_edtNUMBER, dlg.m_strUserNo);
		m_pLoan->SPFExcutePath("회원_검색_NOASK");
	}
}


VOID CBO_LOC_3100::SetLillPanalty()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) return;
	if(1 > pDM->GetRowCount())
	{
		m_pLoan->SelfCloseMessageBox(_T("이용자를 먼저 조회해 주십시오"));
		SetDefaultFocus();
		return;
	}
	CString strUnity;
	pDM->GetCellData(_T("통합회원여부"), 0, strUnity);
	if(_T("Y") != strUnity)
	{
		m_pLoan->SelfCloseMessageBox(_T("통합회원에 대해서 사용 가능합니다."));
		SetDefaultFocus();
		return;
	}
	if(FALSE == m_pInfo->m_bKLUse)
	{
		m_pLoan->SelfCloseMessageBox(_T("책이음서비스 사용하는 도서관에서만 사용가능합니다."));
		SetDefaultFocus();
		return;
	}
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		m_pLoan->SelfCloseMessageBox(_T("센터와의 접속 시도중입니다."));
		SetDefaultFocus();
		return;
	}
	CLill_Panalty dlg(this);
	dlg.DoModal();

}

VOID CBO_LOC_3100::KL_LoanView()
{
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) return;
	if(1 > pDM->GetRowCount())
	{
		m_pLoan->SelfCloseMessageBox(_T("이용자를 먼저 조회해 주십시오"));
		SetDefaultFocus();
		return;
	}
	CString strUnity;
	pDM->GetCellData(_T("통합회원여부"), 0, strUnity);
	if(_T("Y") != strUnity)
	{
		m_pLoan->SelfCloseMessageBox(_T("통합회원에 대해서 사용 가능합니다."));
		SetDefaultFocus();
		return;
	}
	
	CString strUserNo;
	pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	if(strUserNo.IsEmpty())
	{
		m_pLoan->SelfCloseMessageBox(_T("대출자번호 값이 없습니다."));
		SetDefaultFocus();
		return;
	}

	CKL_SearchCurrentLoanInfoDlg dlg(this);
	dlg.SetUserNo(strUserNo);

	dlg.DoModal();
}


LRESULT CBO_LOC_3100::OnCertifyIcon(WPARAM wParam, LPARAM lParam)
{
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL != pDM)
	{
		if(0 < pDM->GetRowCount())
		{
			CString strGetData;
			pDM->GetCellData(_T("실명인증코드"), 0, strGetData);
			if(!strGetData.IsEmpty())
			{ 
				m_btnCertify_Y.ShowWindow(TRUE);
				m_btnCertify_N.ShowWindow(FALSE);
				m_btnCertify_Fail.ShowWindow(FALSE);

				
				m_nCertifyState = 1;

				
				pDM->GetCellData(_T("실명인증일자"), 0, strGetData);
				m_ToolTip.DelTool(GetDlgItem(IDC_btn3100Certify_Y));
				CString strValue;

				if(!strGetData.IsEmpty()) strValue.Format(_T("본인인증한 이용자입니다.\r\n인증일자 : %s"), strGetData);
				else strValue = _T("본인인증한 이용자입니다.");
				
				m_ToolTip.AddTool(GetDlgItem(IDC_btn3100Certify_Y), strValue);
			}
			else
			{
				pDM->GetCellData(_T("실명인증실패코드"), 0, strGetData);
				if(strGetData.IsEmpty())
				{ 
					m_btnCertify_N.ShowWindow(TRUE);
					m_btnCertify_Y.ShowWindow(FALSE);
					m_btnCertify_Fail.ShowWindow(FALSE);

					
					m_nCertifyState = 2;

					if (g_strIpinFlag.CompareNoCase(_T("Y")) != 0 || (g_strIpinFlag.CompareNoCase(_T("Y")) == 0 && (g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)))
					{
						m_BalloonTip.Hide();
						CRect rect;
						GetDlgItem(IDC_btn3100Certify_N)->GetWindowRect(&rect);
						CPoint pt = rect.TopLeft();
						m_BalloonTip.SetIcon(m_hBalloonIcon);

						m_BalloonTip.Show(_T("본인인증을 하기 위해서는\n이 버튼을 클릭해 주세요."), &pt);
					}
				}
				else
				{ 
					m_btnCertify_Fail.ShowWindow(TRUE);
					m_btnCertify_Y.ShowWindow(FALSE);
					m_btnCertify_N.ShowWindow(FALSE);

					
					m_nCertifyState = 3;

					
					m_ToolTip.DelTool(GetDlgItem(IDC_btn3100Certify_Fail));
					CString strValue;
					strValue.Format(_T("%d"), m_pInfo->m_nCertifyAgency);
					CString strDesc = _T(""), strAction = _T("");
					m_pInfo->GetCertifyFailCodeToDesc(strValue, strGetData, strDesc, strAction);

					if(strDesc.IsEmpty()) strValue = _T("본인인증을 하기 위해서는\r\n이 버튼을 클릭해 주세요.");
					else strValue = _T("실패사유 : ") + strDesc + _T("\r\n본인인증을 하기 위해서는 이 버튼을 클릭해 주세요.");
					
					m_ToolTip.AddTool(GetDlgItem(IDC_btn3100Certify_Fail), strValue);
					
					if (g_strIpinFlag.CompareNoCase(_T("Y")) != 0 || (g_strIpinFlag.CompareNoCase(_T("Y")) == 0 && (g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)))
					{
						m_BalloonTip.Hide();
						CRect rect;
						GetDlgItem(IDC_btn3100Certify_Fail)->GetWindowRect(&rect);
						CPoint pt = rect.TopLeft();
						m_BalloonTip.SetIcon(m_hBalloonIcon);
						m_BalloonTip.Show(strValue, &pt);
					}
				}
			}
		}
		else
		{
			m_btnCertify_Y.ShowWindow(FALSE);
			m_btnCertify_N.ShowWindow(FALSE);
			m_btnCertify_Fail.ShowWindow(FALSE);

			
			m_nCertifyState = 0;
		}
	}

	return 0;
}

void CBO_LOC_3100::Onbtn3100CertifyY() 
{
	if(IDYES == MessageBox(_T("이미 본인인증된 이용자입니다.\r\n본인인증을 다시 하시겠습니까?"), _T("본인인증"), MB_YESNO|MB_ICONQUESTION))
	{
		StartUserCertify();
	}
}

void CBO_LOC_3100::Onbtn3100CertifyN() 
{
	StartUserCertify();
}

void CBO_LOC_3100::Onbtn3100CertifyFail() 
{
	StartUserCertify();
}

void CBO_LOC_3100::StartUserCertify()
{
	if (g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0)
	{
		StartTotalCertify();
		return;
	}
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) return;
	if(1 > pDM->GetRowCount()) return;
	CString strCivilNoUseYN = _T("");

	
	if(TRUE == m_pInfo->m_bKLUse && FALSE == m_pInfo->m_bCertifyUse) strCivilNoUseYN = _T("Y");
	else m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호사용유무"), strCivilNoUseYN);
	CUserCertifyDlg dlg(this);
	
	dlg.m_nMode = 1;

	pDM->GetCellData(_T("대출자이름"), 0, dlg.m_strName);
	if(_T("Y") == strCivilNoUseYN)
	{
		CString strCivilNo;
		
		if(TRUE == m_pInfo->m_bKLUse)
		{
			pDM->GetCellData(_T("암호화주민등록번호"), 0, strCivilNo);
			this->ESL_DES_Decryption(strCivilNo, strCivilNo);
		}
		else
		{
			pDM->GetCellData(_T("주민등록번호"), 0, strCivilNo);
		}
		strCivilNo.Replace(_T("-"), _T(""));
		if(13 == strCivilNo.GetLength())
		{
			if(TRUE == m_pLoan->IsNumericText(strCivilNo))
			{
				dlg.m_strCivilNo1 = strCivilNo.Left(6);
				dlg.m_strCivilNo2 = strCivilNo.Right(7);
			}
		}
	}
	pDM->GetCellData(_T("대출자KEY"), 0, dlg.m_strUserKey);
	if(dlg.m_strUserKey.IsEmpty()) return;
	dlg.m_strCivilNoDisplayCheck = strCivilNoUseYN;

	
	
	if(TRUE == m_pInfo->m_bKLUse)
	{
		pDM->GetCellData(_T("실명인증코드"), 0, dlg.m_strBeforeCode);
		pDM->GetCellData(_T("실명인증실패코드"), 0, dlg.m_strBeforeFailCode);
		pDM->GetCellData(_T("실명인증일자"), 0, dlg.m_strBeforeDate);
		pDM->GetCellData(_T("G-PIN성별"), 0, dlg.m_strBeforeSex);
		pDM->GetCellData(_T("실명인증작업자"), 0, dlg.m_strBeforeWorker);
		pDM->GetCellData(_T("대출자이름"), 0, dlg.m_strBeforeName);
		CString strGetData;
		pDM->GetCellData(_T("통합회원여부"), 0, strGetData);
		if(_T("Y") == strGetData) dlg.m_bIsKLMemberYN = TRUE;
		pDM->GetCellData(_T("암호화주민등록번호"), 0, strGetData);
		if(!strGetData.IsEmpty()) this->ESL_DES_Decryption(strGetData, dlg.m_strBeforeCivilNo);
	}

	dlg.DoModal();
	if(!dlg.m_strCertifyCode.IsEmpty())
	{ 
		pDM->SetCellData(_T("실명인증코드"), dlg.m_strCertifyCode, 0);
		pDM->SetCellData(_T("실명인증실패코드"), _T(""), 0);
		pDM->SetCellData(_T("실명인증일자"), dlg.m_strCertifyDate, 0);
		pDM->SetCellData(_T("G-PIN성별"), dlg.m_strCertifySex, 0);
		pDM->SetCellData(_T("실명인증작업자"), dlg.m_strCertifyWorker, 0);
		pDM->SetCellData(_T("대출자이름"), dlg.m_strCertifyName, 0);
		if(_T("Y") == strCivilNoUseYN)
		{
			pDM->SetCellData(_T("주민등록번호"), dlg.m_strCertifyCivilNo, 0);
		}
		m_btnCertify_Y.ShowWindow(TRUE);
		m_btnCertify_N.ShowWindow(FALSE);
		m_btnCertify_Fail.ShowWindow(FALSE);

		
		m_nCertifyState = 1;

		m_pLoan->SPFExcutePath(_T("회원정보만리로드"));
	}
	else
	{
		if(!dlg.m_strCertifyFailCode.IsEmpty())
		{ 
			pDM->SetCellData(_T("실명인증코드"), _T(""), 0);
			pDM->SetCellData(_T("실명인증실패코드"), dlg.m_strCertifyFailCode, 0);
			pDM->SetCellData(_T("실명인증일자"), dlg.m_strCertifyDate, 0);
			pDM->SetCellData(_T("G-PIN성별"), dlg.m_strCertifySex, 0);
			pDM->SetCellData(_T("실명인증작업자"), dlg.m_strCertifyWorker, 0);
			pDM->SetCellData(_T("대출자이름"), dlg.m_strCertifyName, 0);
			if(_T("Y") == strCivilNoUseYN)
			{
				pDM->SetCellData(_T("주민등록번호"), dlg.m_strCertifyCivilNo, 0);
			}
			m_btnCertify_Fail.ShowWindow(TRUE);
			m_btnCertify_Y.ShowWindow(FALSE);
			m_btnCertify_N.ShowWindow(FALSE);

			
			m_nCertifyState = 3;

			m_ToolTip.DelTool(GetDlgItem(IDC_btn3100Certify_Fail));
			CString strValue;
			strValue.Format(_T("%d"), m_pInfo->m_nCertifyAgency);
			CString strDesc = _T(""), strAction = _T("");
			m_pInfo->GetCertifyFailCodeToDesc(strValue, dlg.m_strCertifyFailCode, strDesc, strAction);

			strValue = _T("실패사유 : ") + strDesc + _T("\r\n본인인증을 하기 위해서는 이 버튼을 클릭해 주세요.");
			m_ToolTip.AddTool(GetDlgItem(IDC_btn3100Certify_Fail), strValue);
			m_pLoan->SPFExcutePath(_T("회원정보만리로드"));
		}
	}
}

void CBO_LOC_3100::OnDestroy() 
{
	CDialog::OnDestroy();
	
	
	if(TRUE == m_pInfo->m_bCertifyUse)
	{
		::DestroyIcon(m_hBalloonIcon);
		m_Font.DeleteObject();
	}

	if(_T("Y") == m_pLoanMangeValue->m_str3100ControlYN) 
	{
		UnLoad3100ControlProcess();
	}
}

LRESULT CBO_LOC_3100::OnStatementButton(WPARAM wParam, LPARAM lParam)
{
	m_nCode = (INT)wParam;
	CString strIsKLMember;
	CString strAgreeStatement;
	CString strAgentName;
	CString strRealNameCertYN;
	CString strExpireDate;
	CString strBirthday;
	CString strCertyfyCode;
	CString strAgreeType;

	GetDlgItem(IDC_staticPRIVATEMANAGE)->ShowWindow(SW_HIDE);
	KillTimer(1);
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->GetCellData(_T("통합회원여부"), 0, strIsKLMember);
	pDM->GetCellData(_T("동의약관"), 0, strAgreeStatement);
	pDM->GetCellData(_T("만료일자"), 0, strExpireDate);
	pDM->GetCellData(_T("대리인명"), 0, strAgentName);
	pDM->GetCellData(_T("실명인증코드"), 0, strCertyfyCode);
	pDM->GetCellData(_T("생일"), 0, strBirthday);
	pDM->GetCellData(_T("동의방법"), 0, strAgreeType);
	
	CString strGetData;
	BOOL bUnder14YN = FALSE;
	if(10 == strBirthday.GetLength())
	{
		COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();
		CString sCurrentTime = tCurrentTime.Format(_T("%Y/%m/%d"));
		INT nBirthYear = _ttoi(strBirthday.Left(4));
		INT nBirthMonth = _ttoi(strBirthday.Mid(5, 2));
		INT nBirthDay = _ttoi(strBirthday.Right(2));
		INT nCurrentYear = _ttoi(sCurrentTime.Left(4));
		INT nCurrentMonth = _ttoi(sCurrentTime.Mid(5, 2));
		INT nCurrentDay = _ttoi(sCurrentTime.Right(2));
		if(14 > (nCurrentYear-nBirthYear))
		{
			bUnder14YN = TRUE;
		}
		else if(14 == (nCurrentYear-nBirthYear) && 0 > (nCurrentMonth-nBirthMonth))
		{
			bUnder14YN = TRUE;	
		}	
		else if(14 == (nCurrentYear-nBirthYear) && 0 == (nCurrentMonth-nBirthMonth) && 0 > (nCurrentDay-nBirthDay))
		{
			bUnder14YN = TRUE;	
		}	
		else
		{
			bUnder14YN = FALSE;
		}
	}
	if(0 < m_nCode)
	{	
		if((TRUE == m_pInfo->m_bAgreeInfoRecordYN || (m_pInfo->m_bKLUse && _T("Y") == strIsKLMember)))
		{
			CString strMsg;
			GetDlgItem(IDC_btnSTATEMENT)->ShowWindow(SW_SHOW);
			
			
			if(_T("Y") == strIsKLMember)
			{
				
				if(_T("책이음서비스") != strAgreeStatement && _T("1") != strAgreeType)
				{
					SetTimer( 1 , 500 , NULL );
				}
				else if(m_pInfo->m_bCertifyUse && _T("") == strCertyfyCode)
				{
					SetTimer( 1 , 500 , NULL );
				}
				else if(bUnder14YN && _T("") == strAgentName && _T("1") != strAgreeType)
				{
					SetTimer( 1 , 500 , NULL );
				}
				
				if(0 < m_pInfo->m_nUserInfoRenewalDay)
				{
					
					if(10 <= strExpireDate.GetLength())
					{
						CString strGetData;
						strExpireDate = strExpireDate.Left(10);
						COleDateTime tRenewalTime(_ttoi(strExpireDate.Left(4)),_ttoi(strExpireDate.Mid(5, 2)),_ttoi(strExpireDate.Right(2)),0,0,0);
						COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();
						COleDateTimeSpan tSpan =  tRenewalTime - tCurrentTime;
						INT nElapseDay = (INT)tSpan.GetDays();
						if(0 > nElapseDay)
						{	
							SetTimer( 1 , 500 , NULL );
						}
					}
					else
					{
						SetTimer( 1 , 500 , NULL );
					}
				}
			}
			
			else if(TRUE == m_pInfo->m_bAgreeInfoRecordYN && _T("Y") != strIsKLMember)
			{
				if(_T("개인정보 수집/이용") != strAgreeStatement && _T("1") != strAgreeType)
				{
					SetTimer( 1 , 500 , NULL );
				}
				else if(m_pInfo->m_bCertifyUse && _T("") == strCertyfyCode)
				{
					SetTimer( 1 , 500 , NULL );
				}
				else if(bUnder14YN && _T("") == strAgentName && _T("1") != strAgreeType)
				{
					SetTimer( 1 , 500 , NULL );
				}
				
				if(0 < m_pInfo->m_nUserInfoRenewalDay)
				{
					
					if(10 <= strExpireDate.GetLength())
					{
						CString strGetData;
						strExpireDate = strExpireDate.Left(10);
						COleDateTime tRenewalTime(_ttoi(strExpireDate.Left(4)),_ttoi(strExpireDate.Mid(5, 2)),_ttoi(strExpireDate.Right(2)),0,0,0);
						COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();
						COleDateTimeSpan tSpan =  tRenewalTime - tCurrentTime;		
						INT nElapseDay = (INT)tSpan.GetDays();
						if(0 > nElapseDay)
						{	
							SetTimer( 1 , 500 , NULL );
						}
					}
					else
					{
						SetTimer( 1 , 500 , NULL );
					}
				}
			}
		}
	}
	else
	{
		GetDlgItem(IDC_btnSTATEMENT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_staticPRIVATEMANAGE)->ShowWindow(SW_HIDE);
		KillTimer(1);
	}
	return 0;
}

void CBO_LOC_3100::OnbtnSTATEMENT() 
{
	if ( m_pLoanMangeValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanMangeValue->m_sLoanUserAccessMode == _T("1") )
	{
		AfxMessageBox(_T("접근권한이 없습니다."));
		return;
	}
	
	CString strIsCollectYN;
	CString strIsProvideYN;
	CString strBirthday;
	CString strQuery;
	CString strUserKey;
	CString sIsKLMember;
	CString strAgentName;
	CString strAgentRelation;
	CString strAgentNote;
	CString strAgentKey;
	CString strWork = this->GetWorkLogMsg(_T("이용약관 소급동의"),__WFILE__,__LINE__);
	CString strGetDuplication;
	
	CString strAgreeType;
	CString strUserNo;
	CString strGpinHash;
	CString strIsKLmemberYN;
	
	CString strStatementAlias;
	CString strCertifyCode;

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->SetCONNECTION_INFO( m_pInfo->CONNECTION_INFO );
	pDM->GetCellData(_T("대출자KEY"), 0, strUserKey);
	pDM->GetCellData(_T("대리인명"), 0, strAgentName);
	pDM->GetCellData(_T("대리인관계"), 0, strAgentRelation);
	pDM->GetCellData(_T("대리인비고"), 0, strAgentNote);
	pDM->GetCellData(_T("생일"), 0, strBirthday);
	pDM->GetCellData(_T("통합회원여부"), 0, sIsKLMember);
	pDM->GetCellData(_T("동의방법"), 0, strAgreeType);
	pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	pDM->GetCellData(_T("G-PINHASH인증코드"), 0, strGpinHash);
	pDM->GetCellData(_T("통합회원여부"), 0, strIsKLmemberYN);
	pDM->GetCellData(_T("동의약관"), 0, strStatementAlias);
	pDM->GetCellData(_T("실명인증코드"), 0, strCertifyCode);
	
	CString strGetData;
	BOOL bUnder14YN = FALSE;
	if(10 == strBirthday.GetLength())
	{
		CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strGetData);
		CString sCurrentTime = strGetData.Left(10);
		INT nBirthYear = _ttoi(strBirthday.Left(4));
		INT nBirthMonth = _ttoi(strBirthday.Mid(5, 2));
		INT nBirthDay = _ttoi(strBirthday.Right(2));
		INT nCurrentYear = _ttoi(sCurrentTime.Left(4));
		INT nCurrentMonth = _ttoi(sCurrentTime.Mid(5, 2));
		INT nCurrentDay = _ttoi(sCurrentTime.Right(2));
		if(14 > (nCurrentYear-nBirthYear))
		{
			bUnder14YN = TRUE;
		}
		else if(14 == (nCurrentYear-nBirthYear) && 0 > (nCurrentMonth-nBirthMonth))
		{
			bUnder14YN = TRUE;	
		}	
		else if(14 == (nCurrentYear-nBirthYear) && 0 == (nCurrentMonth-nBirthMonth) && 0 > (nCurrentDay-nBirthDay))
		{
			bUnder14YN = TRUE;	
		}	
		else
		{
			bUnder14YN = FALSE;
		}
	}

	KL_StatementDlg dlg(this);
	dlg.m_nMode = 1;
	dlg.m_strAgentName = strAgentName;
	dlg.m_strAgentRelation = strAgentRelation;
	dlg.m_strAgentNote = strAgentNote;
	dlg.m_strUserKey = strUserKey;
	dlg.m_strAgreeType = strAgreeType;
	dlg.m_strUserNo = strUserNo;
	dlg.m_strGpinHash = strGpinHash;
	dlg.m_bUnder14YN = bUnder14YN;
	dlg.m_bIsKLMode = (m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0);

	
	if(_T("Y") == strIsKLmemberYN && TRUE == m_pInfo->m_bKLUse)
	{
		dlg.m_bIsKLMemberYN = TRUE;
		
		dlg.m_strStatementAlias = _T("책이음서비스");
		
		if(_T("책이음서비스") == strStatementAlias)
		{			
			dlg.m_bIsAgreeYN = TRUE;
			dlg.m_strAgreeType = strAgreeType;
		}
		else
		{
			dlg.m_bIsAgreeYN = FALSE;
			dlg.m_strAgreeType = _T("0");
		}
		if(m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)	
		{
			if(_T("") != strCertifyCode)
			{
				dlg.m_bRealNameCertYN = TRUE;
			}
			else
			{
				dlg.m_bRealNameCertYN = FALSE;
			}
		}
		
		if(bUnder14YN)
		{
			dlg.m_bUnder14YN = TRUE;
			if(_T("") != strAgentName && _T("") != strAgentRelation)
			{
				dlg.m_strAgentName = strAgentName;
				dlg.m_strAgentRelation = strAgentNote;
				dlg.m_strAgentNote = strAgentNote;
			}
		}
		if(IDOK!=dlg.DoModal())
		{
			return;
		}
	}
	else if(TRUE == m_pInfo->m_bAgreeInfoRecordYN && _T("Y") != strIsKLmemberYN )
	{
		dlg.m_bIsKLMemberYN = FALSE;
		dlg.m_strStatementAlias = _T("개인정보 수집/이용");
		
		if(_T("개인정보 수집/이용") == strStatementAlias)
		{
			
			dlg.m_bIsAgreeYN = TRUE;
			dlg.m_strAgreeType = strAgreeType;
		}

		else
		{
			
			dlg.m_bIsAgreeYN = FALSE;
			dlg.m_strAgreeType = _T("0");
		}
		
		if(_T("") != strCertifyCode)
		{
			dlg.m_bRealNameCertYN = TRUE;
		}
		else
		{
			dlg.m_bRealNameCertYN = FALSE;
		}
		
		if(bUnder14YN)
		{
			dlg.m_bUnder14YN = TRUE;
			if(_T("") != strAgentName && _T("") != strAgentRelation)
			{
				dlg.m_strAgentName = strAgentName;
				dlg.m_strAgentRelation = strAgentNote;
				dlg.m_strAgentNote = strAgentNote;
			}
		}
		if(IDOK!=dlg.DoModal())
		{
			return;
		}
	}

	if(m_pInfo->m_bCertifyUse && _T("") == strCertifyCode)
	{
		StartUserCertify();
	}

	m_pLoan->SPFExcutePath(_T("회원정보만리로드"));
	

	return;
}


void CBO_LOC_3100::Onbtn3100CERTIFY() 
{
	CString strReCertifyYN = _T("N");
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("본인인증조회사용여부"), strReCertifyYN);
	if ( strReCertifyYN.CompareNoCase(_T("Y")) == 0  && 
		(g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0) )
	{
		CCertifyMainDlg dlg(this);
		
		dlg.SetUsedMypinService(g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0);
		dlg.SetUsedPhoneService(g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0);
		dlg.SetUserMode(2);
		
		if(IDOK == dlg.DoModal())
		{
			CESL_DataMgr dm;
			dm.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			CString strQuery= _T("");	
			CString strUserNo = _T("");	

			
			if(TRUE == m_pInfo->m_bKLUse)
			{
				CKL_UserManagerDlg klUserDlg(this);
				klUserDlg.GetKlUserSearch(dlg.GetCertifyCI(), _T(""), strUserNo);
				if(_T("") == strUserNo)
				{
					strQuery.Format(_T("SELECT MAX(USER_NO) FROM CO_LOAN_USER_TBL WHERE IPIN_HASH = '%s'"), dlg.GetCertifyCI());
					dm.GetOneValueQuery(strQuery, strUserNo);
				}
			}
			else
			{
				strQuery.Format(_T("SELECT MAX(USER_NO) FROM CO_LOAN_USER_TBL WHERE IPIN_HASH = '%s'"), dlg.GetCertifyCI());
				dm.GetOneValueQuery(strQuery, strUserNo);
			}

			if(_T("") != strUserNo)
			{
				SetDlgItemText(IDC_edtNUMBER, strUserNo);
				GetDlgItem(IDC_edtNUMBER)->PostMessage(WM_KEYDOWN, VK_RETURN);	
			}
			else
			{
				m_pLoan->SelfCloseMessageBox(_T("조회된 이용자가 없습니다."));
				return;
			}		
		}
		return;
	}

	
	m_pLoan->m_strSerchCivilNo = _T("");

	CKlCertifyDlg dlg(this);
	if(IDOK == dlg.DoModal())
	{
		CString strUserNo = dlg.GetUserNo();
		if(strUserNo.IsEmpty())
		{
			AfxMessageBox(_T("조회된 이용자가 없습니다."));
			return;
		}

		
		m_pLoan->m_strSerchCivilNo = dlg.m_strSuccessCivilNo;

		SetDlgItemText(IDC_edtNUMBER, strUserNo);
		GetDlgItem(IDC_edtNUMBER)->PostMessage(WM_KEYDOWN, VK_RETURN);
	}
}

LRESULT CBO_LOC_3100::UserInfoRenewal(WPARAM wParam, LPARAM lParam)
{
	if(!(BOOL)wParam)
	{
		return 0;
	}
	
	GetDlgItem(IDC_staticPRIVATEMANAGE)->ShowWindow(SW_HIDE);
	KillTimer(1);
	
	if((m_pInfo->m_bAgreeInfoRecordYN || m_pInfo->m_bKLUse) && 0 < m_pInfo->m_nUserInfoRenewalDay)
	{
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		CString strMsg;
		CString strGetData;
		CString strRenewalDate;
		pDM->GetCellData(_T("만료일자"), 0, strRenewalDate);
		if(0 < m_pInfo->m_nUserInfoRenewalDay)
		{
			if(10 <= strRenewalDate.GetLength())
			{
				strRenewalDate = strRenewalDate.Left(10);
				COleDateTime tRenewalTime(_ttoi(strRenewalDate.Left(4)),_ttoi(strRenewalDate.Mid(5, 2)),_ttoi(strRenewalDate.Right(2)),0,0,0);
				COleDateTime tCurrentTime = COleDateTime::GetCurrentTime();
				COleDateTimeSpan tSpan = tRenewalTime -tCurrentTime;
				INT nElapseDay = (INT)tSpan.GetDays();

				if(0 > nElapseDay)
				{	
					SetTimer(1, 500, NULL);
				}
			}
			else if(_T("") == strRenewalDate)
			{	
				SetTimer(1, 500, NULL);
			}
		}
	}
	return 0;
}


INT CBO_LOC_3100::Load3100ControlProcess()
{
	if(_T("Y") != m_pLoanMangeValue->m_str3100ControlYN) {
		return -100;
	}
	
	HWND hWnd = ::FindWindow(NULL, _T("LoanReturnControl"));
	if(hWnd) {
		::PostMessage(hWnd, WM_QUIT, NULL, NULL);
	}

	CStdioFileEx File;
	if(!File.Open(_T("..\\cfg\\3100HWND"), CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode)) {
		return -200;
	}
	
	CString strData;
	strData.Format(_T("%d"), GetSafeHwnd());
	File.WriteString(strData);
	File.Close();
		
	CString strCmd = _T("MLoader.exe LoanReturnControl.exe");
	TCHAR lpCurrentDirectory[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, lpCurrentDirectory);
	
	PROCESS_INFORMATION pinfo;
	STARTUPINFO Startupinfo1;
	ZeroMemory(&Startupinfo1, sizeof(Startupinfo1));
	Startupinfo1.cb = sizeof(STARTUPINFO);	
   	Startupinfo1.dwFlags = STARTF_USESHOWWINDOW;
	Startupinfo1.wShowWindow = SW_SHOW;
	INT result = ::CreateProcess(NULL, strCmd.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, lpCurrentDirectory, &Startupinfo1, &pinfo);

	INT nRetry = 0;
	if( result <= 0 ) {
		while( ::FindWindow(NULL, _T("LoanReturnControl")) == NULL && nRetry < 3)	{
			if( ::CreateProcess(NULL, strCmd.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, lpCurrentDirectory, &Startupinfo1, &pinfo) > 0 ) {
				break;			
			}			
			nRetry++;
		}
	}

	strCmd.ReleaseBuffer();
	
	CloseHandle(pinfo.hProcess);
	CloseHandle(pinfo.hThread);
	
	if( nRetry >= 3 ) {
		AfxMessageBox(_T("대출반납제어 프로그램이 실행되지 않았습니다.\r\n대출반납화면을 종료 후 다시 실행해주시기 바랍니다."));
		return -300;
	}	

	KillTimer(TIMER_API_LRC_MONITOR);
	SetTimer(TIMER_API_LRC_MONITOR, 100, NULL);
	return 0;
}

INT CBO_LOC_3100::UnLoad3100ControlProcess()
{
	if (GetSafeHwnd()) 
	{
		KillTimer(TIMER_API_LRC_MONITOR);
	}
	if(_T("Y") != m_pLoanMangeValue->m_str3100ControlYN) {
		return -100;
	}
	
	CStdioFileEx File;
	if(File.Open(_T("..\\cfg\\3100HWND"), CFile::modeRead|CFile::typeText)) {
		CString strData1, strData2;	
		File.ReadString(strData1);
		File.Close();		
		
		strData2.Format(_T("%d"), GetSafeHwnd());
		if(strData1 != strData2) {
			return -200;
		}
	}
	
	HWND hWnd = ::FindWindow(NULL, _T("LoanReturnControl"));
	if(hWnd) {
		::PostMessage(hWnd, WM_QUIT, NULL, NULL);
	}
	
	return 0;
}


void CBO_LOC_3100::Onrad3100SetControl() 
{
	if (TRUE == m_b3100ControlLoadYN) return;

	if (0 != Load3100ControlProcess()) {
		((CButton*)GetDlgItem(IDC_rad3100SetControl))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_rad3100ClearControl))->SetCheck(TRUE);
	} else {
		m_b3100ControlLoadYN = TRUE;
		CStdioFileEx File;
		if (File.Open(_T("..\\cfg\\3100SETLRC"), CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode)) {	
			File.WriteString(_T("제어설정|1"));
			File.Close();
		}
	}
}

void CBO_LOC_3100::Onrad3100ClearControl() 
{
	if (FALSE == m_b3100ControlLoadYN) return;

	if (0 != UnLoad3100ControlProcess()) {
		((CButton*)GetDlgItem(IDC_rad3100SetControl))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_rad3100ClearControl))->SetCheck(FALSE);
	} else {
		m_b3100ControlLoadYN = FALSE;
		CStdioFileEx File;
		if (File.Open(_T("..\\cfg\\3100SETLRC"), CFile::modeCreate|CFile::modeWrite|CStdioFileEx::modeWriteUnicode)) {	
			File.WriteString(_T("제어설정|0"));
			File.Close();
		}
	}
}

LRESULT CBO_LOC_3100::OnLoanReturnControlService(WPARAM wParam, LPARAM lParam)
{
	INT ids = 0;
	CString strLine;
	if(TRUE == wParam) {
		strLine = *(CString*)lParam;
	} else {
		CStdioFileEx fLRCtoKOLAS;
		if(!fLRCtoKOLAS.Open(_T("..\\cfg\\3100LRCtoKOLAS"), CFile::modeRead|CFile::typeText)) {
			return -1011;
		}
		fLRCtoKOLAS.ReadString(strLine);
		fLRCtoKOLAS.Close();
	}
	CString strCommand, strUserNo, strRegNo;
	AfxExtractSubString(strCommand, strLine, 0, '|');
	AfxExtractSubString(strUserNo, strLine, 1, '|');
	AfxExtractSubString(strRegNo, strLine, 2, '|');

	if(FALSE == m_b3100ControlLoadYN) {		
		return -1012;
	}
	
	CString strTemp;
	
	if (_T("LRC_001_SERVICE") == strCommand) {
		
		SetDlgItemText(IDC_edtNUMBER, strUserNo);
		m_pLoan->SPFExcutePath(_T("회원_검색"));
		SetDefaultFocus();
	} 
	
	else if (_T("LRC_002_SERVICE") == strCommand) {
		
		CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));

		
		CString strSearchUserNo = _T("");
        if (pUserDM->GetRowCount())
		{
			pUserDM->GetCellData(_T("대출자번호"), 0, strSearchUserNo);
		}

		if (strUserNo.IsEmpty())
		{
			if (strSearchUserNo.IsEmpty())
			{
				
				return -1007;
			}
		}
		else
		{
			
			if (strUserNo.CompareNoCase(strSearchUserNo) != 0)
			{
				SetDlgItemText(IDC_edtNUMBER, strUserNo);
				m_pLoan->SPFExcutePath(_T("회원_검색"));
			}
		}

		if (pUserDM->GetRowCount())
		{
			CString strReserveMode;
			m_pLoan->m_pCheckLoan->GetReserveMode(strReserveMode);
			if (strReserveMode.CompareNoCase(_T("0")) != 0) 
			{
				m_pLoan->SetTempData(_T("대출반납흐름제어"), CComVariant(_T("Y")));
			}
			SetDlgItemText(IDC_edtNUMBER, strRegNo);
			ids = m_pLoan->SPFExcutePath(_T("대출"));
            if (1 == ids)
			{ 
				
				CBO_LOC_3192 Dlg(this,m_pLoanShareManager,m_pLoanMangeValue,1);
                Dlg.DoModal();
            }
            ClearNumberCtrl();
            ClearLibCodeControl();
			m_pLoan->SetTempData(_T("대출반납흐름제어"), CComVariant(_T("N")));
		}
	} 
	
	else if (_T("LRC_003_SERVICE") == strCommand) {
		m_pLoan->m_bReturnBookNonFree = TRUE;
		
		CESL_DataMgr* pUserDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		CString strSearchUserNo = _T("");
		if (0 < pUserDM->GetRowCount()) {
			pUserDM->GetCellData(_T("대출자번호"), 0, strSearchUserNo);
		}
		if (strSearchUserNo != strUserNo) { 

			SetDlgItemText(IDC_edtNUMBER, strUserNo);
			m_pLoan->SPFExcutePath(_T("회원_검색_NOASK"));
		}
		
		if (0 < pUserDM->GetRowCount()) {
			CESL_DataMgr *pLoanDM = FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
			CESL_ControlMgr *pCM = FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
			CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(_T("MainGrid")));
			CString strUserLoanRegNo = _T("");
			INT nCheckColIndex = 0;
			BOOL bCheck = FALSE;
			CLocCommonAPI::GetGridColumnIndexs(this, pGrid, _T("선정"), nCheckColIndex);
			CLocCommonAPI::MakeRegNoSpaceToZero(strRegNo);
			for(INT i = 0 ; i < pLoanDM->GetRowCount() ; i++) {
				pGrid->SetAt(i, nCheckColIndex, _T(""));
				pLoanDM->GetCellData(_T("등록번호"), i, strUserLoanRegNo);
				if(strUserLoanRegNo == strRegNo) {
					pGrid->SetAt(i, nCheckColIndex, _T("V"));
					pGrid->m_nReverse = i;
					pGrid->SetReverse(i);
					pGrid->SelectMakeList();
					INT nIdx = pGrid->SelectGetIdx();
					if(nIdx != i)
					{
						POSITION pos = pGrid->m_arraySelect.GetHeadPosition();
						while (pos)
						{
							pGrid->m_arrayPos = pos;
							if (i == pGrid->m_arraySelect.GetNext(pos))
								break;				
						}
					}
					bCheck = TRUE;
					break;
				}
			}			
			if(FALSE == bCheck) {
				m_pLoan->SelfCloseMessageBox(_T("이용자가 대출한 책이 아닙니다."));
			} else {
				SetDlgItemText(IDC_edtNUMBER, strRegNo);
				m_pLoan->SPFExcutePath(_T("바코드업무"));
				ClearNumberCtrl();
				ClearLibCodeControl();
			}
		}
		m_pLoan->m_bReturnBookNonFree = FALSE;
	}
	
	else if (_T("LRC_004_SERVICE") == strCommand) {
		if(_T("반납목록초기화") == strUserNo) {
			
			this->PostMessage(WM_KEYDOWN, VK_ESCAPE, NULL);
		}
	}

	CStdioFileEx fKOLAStoLRC;
	if(!fKOLAStoLRC.Open(_T("..\\cfg\\3100KOLAStoLRC"), CFile::modeWrite|CFile::modeCreate|CStdioFileEx::modeWriteUnicode)) {
		return -1013;
	}
	CString strResult;
	strResult.Format(_T("%s|성공"), strCommand);
	fKOLAStoLRC.WriteString(strResult);
	fKOLAStoLRC.Close();

	return 0;
}

VOID CBO_LOC_3100::StartTotalCertify()
{
	CCertifyMainDlg dlg(this);
	
	dlg.SetUsedMypinService(g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0);
	dlg.SetUsedPhoneService(g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0);
	dlg.SetUserMode(1);

	CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) return;
	if(1 > pDM->GetRowCount()) return;

	CString strUserKey;
	pDM->GetCellData(_T("대출자KEY"), 0, strUserKey);
	if (strUserKey.IsEmpty())
	{
		return;
	}

	dlg.SetUserKey(strUserKey);

	CString strCertifyCode;
	CString strCertifyFailCode;
	CString strCertifyDate;
	CString strCertifyWorker;
	CString strKLMemberYN;
	CString strGpinGender;
	
	pDM->GetCellData(_T("실명인증코드"), 0, strCertifyCode);
	pDM->GetCellData(_T("실명인증실패코드"), 0, strCertifyFailCode);
	pDM->GetCellData(_T("실명인증일자"), 0, strCertifyDate);
	pDM->GetCellData(_T("실명인증작업자"), 0, strCertifyWorker);
	pDM->GetCellData(_T("통합회원여부"), 0, strKLMemberYN);
	pDM->GetCellData(_T("G-PIN성별"), 0, strGpinGender);

	if (m_pInfo->m_bKLUse)
	{
		dlg.SetCertifyCI(strCertifyCode);
		dlg.SetCertifyFailCode(strCertifyFailCode);
		dlg.SetCertifyDate(strCertifyDate);
		dlg.SetCertifyWorker(strCertifyWorker);
		dlg.SetKLMemberYN(strKLMemberYN.CompareNoCase(_T("Y")) == 0);
	}
	dlg.SetCertifyGender(strGpinGender);

	CString strName;
	GetControlData(_T("CM_BO_LOC_3320") , _T("이름"), strName);
	dlg.SetUserName(strName);

	CString strBirthDay;
	GetControlData(_T("CM_BO_LOC_3320") , _T("생일"), strBirthDay);
	dlg.SetBirthDay(strBirthDay);

	CString strHandPhone;
	GetControlData(_T("CM_BO_LOC_3320") , _T("휴대폰"), strHandPhone);
	dlg.SetHandPhone(strHandPhone);
	
	dlg.DoModal();

	if (!dlg.GetCertifyCI().IsEmpty())
	{
		pDM->SetCellData(_T("실명인증코드"), dlg.GetCertifyCI(), 0);
		pDM->SetCellData(_T("실명인증실패코드"), _T(""), 0);
		pDM->SetCellData(_T("실명인증일자"), dlg.GetCertifyDate(), 0);		
		pDM->SetCellData(_T("실명인증작업자"), dlg.GetCertifyWorker(), 0);
		pDM->SetCellData(_T("대출자이름"), dlg.GetUserName(), 0);
		pDM->SetCellData(_T("G-PIN성별"), dlg.GetCertifyGender(), 0);

		m_btnCertify_Y.ShowWindow(TRUE);
		m_btnCertify_N.ShowWindow(FALSE);
		m_btnCertify_Fail.ShowWindow(FALSE);

		m_nCertifyState = 1;

		m_pLoan->SPFExcutePath(_T("회원정보만리로드"));
	}
	else
	{
		
		if (!dlg.GetCertifyFailCode().IsEmpty())
		{
			pDM->SetCellData(_T("실명인증코드"), _T(""), 0);
			pDM->SetCellData(_T("실명인증실패코드"), dlg.GetCertifyFailCode(), 0);
			pDM->SetCellData(_T("실명인증일자"), dlg.GetCertifyDate(), 0);		
			pDM->SetCellData(_T("실명인증작업자"), dlg.GetCertifyWorker(), 0);
			pDM->SetCellData(_T("대출자이름"), dlg.GetUserName(), 0);
			pDM->SetCellData(_T("G-PIN성별"), dlg.GetCertifyGender(), 0);

			m_btnCertify_Fail.ShowWindow(TRUE);
			m_btnCertify_Y.ShowWindow(FALSE);
			m_btnCertify_N.ShowWindow(FALSE);

			
			m_nCertifyState = 3;

			m_ToolTip.DelTool(GetDlgItem(IDC_btn3100Certify_Fail));			
			m_pLoan->SPFExcutePath(_T("회원정보만리로드"));
		}
	}
}
VOID CBO_LOC_3100::OnbtnUSERFID()
{
EFS_BEGIN

	Get_RFID_List();

EFS_END
}

VOID CBO_LOC_3100::OnbtnRFIDCheck()
{
EFS_BEGIN
	CString strMsg ;
	INT ids = -1;

	ids = RFIDConnCheck();
	if(ids != 0){
		strMsg.Format(_T("RFID연결에 실패하였습니다.[%d]"),ids);
	}else {
		strMsg.Format(_T("RFID연결 성공!!"));
	}
	AfxMessageBox( strMsg );

EFS_END
}

VOID CBO_LOC_3100::OnbtnUSEViewTag()
{
EFS_BEGIN
	ViewRFIDTagInfo();
EFS_END
}

VOID CBO_LOC_3100::OnbtnRFIDRecord()
{
EFS_BEGIN
	INT ids;
	ids = m_pLoan->SPFExcutePath(_T("RFID_회원증등록"));	
EFS_END
}

INT CBO_LOC_3100::Get_RFID_List()
{
EFS_BEGIN
	if(_T("Y") != m_strRfidUseYN ) return -1;

	INT				ids;
	CRFID_Interface RFID( this );
	INT				nTagCnt;
	CStringArray    DSFID_LIST;
	CStringArray    SERIAL_LIST;
		
	ids = RFID.GET_STATUS();
	if( ids < 0 ) 
	{
		m_pLoan->SelfCloseMessageBox( _T("자료 리더기를 발견하지 못했습니다.") );
		return 0;
	}
	AfxGetApp()->DoWaitCursor( 1 );	
	
	
	ids = RFID.GetTagSerial(SERIAL_LIST );
	nTagCnt = SERIAL_LIST.GetSize();

	if( 0 == nTagCnt )
	{
		m_pLoan->SelfCloseMessageBox( _T("자료가 리더기 위에 존재하지 않습니다.") );
		AfxGetApp()->DoWaitCursor( -1 );
		return 0;
	}
	AfxGetApp()->DoWaitCursor( 1 );	

	
	CESL_DataMgr* pRFID_DM = FindDM( _T("DM_RFID목록") );
	if( !pRFID_DM ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("Get_RFID_List") );
	pRFID_DM->FreeData();
		
	
	CStringArray saUserSerialList;			
	CStringArray saLoanUserNoList;			
	CStringArray saReturnUserNoList;		
	INT			 nWillLoanBookCnt = 0;		
	INT			 nWillReturnBookCnt = 0;	

	ids = MakeRFIDInfoList( nTagCnt , SERIAL_LIST , pRFID_DM , saLoanUserNoList, saReturnUserNoList, saUserSerialList ,  nWillLoanBookCnt , nWillReturnBookCnt );	
	if( ids > 0 ) 
	{
		return 0;
	}
	
	if( saReturnUserNoList.GetSize() > 1 /*&& !nAllReturn */) 
	{
		CString sMsg;
		sMsg.Format( _T("반납할 이용자가 '%d'명입니다.") , saReturnUserNoList.GetSize() );
		AfxMessageBox( sMsg );
		return 0;
	}	

	if( ( saUserSerialList.GetSize() == 1 ) && ( (nWillLoanBookCnt+nWillReturnBookCnt) == 0 ) )
	{
		CString sUserNo;
		sUserNo = saUserSerialList.GetAt(0);
		GetDlgItem(IDC_edtNUMBER)->SetWindowText(sUserNo);
		GetDlgItem(IDC_edtNAME)->SetWindowText(_T(""));
		SearchUser();
		return 0;
	}
	
	CString sBookStatus;
	if( ( saUserSerialList.GetSize() == 1 ) && ( (nWillLoanBookCnt+nWillReturnBookCnt) > 0 ) )
	{
		CString sUserNo;
		sUserNo = saUserSerialList.GetAt(0);
	
		CString sPreUserNo;
		CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
		if( pDM->GetRowCount() > 0 )
		{
			pDM->GetCellData( _T("대출자번호") , 0 , sPreUserNo );
		}

		if( sPreUserNo.Compare( sUserNo ) != 0 )
		{		
			GetDlgItem(IDC_edtNUMBER)->SetWindowText(sUserNo);
			GetDlgItem(IDC_edtNAME)->SetWindowText(_T(""));
			SearchUser();		
		}		
		
		if( nWillLoanBookCnt > 0 )
		{			
			ids = m_pLoan->SPFExcutePath(_T("RFID_대출"));				
		}
		
		else
		{
			if( (saUserSerialList.GetSize() == 1) &&  (saReturnUserNoList.GetSize() > 1) )
			{
				CString sEMsg;
				sEMsg.Format(_T("'%d'명의 회원이 반납할 자료입니다.회원카드를 올려놓지 마시고 일괄 반납을 이용해주십시오.") , saReturnUserNoList.GetSize() );
				m_pLoan->SelfCloseMessageBox( sEMsg );
				return 0;
			}

			if( (saUserSerialList.GetSize() == 1) &&  (saReturnUserNoList.GetSize() == 1) )
			{
				if( (saUserSerialList.GetAt(0)).Compare(saReturnUserNoList.GetAt(0)) != 0 )
				{
					CString sEMsg;
					sEMsg.Format( _T("반납할 이용자와 일치하지 않는 회원카드가 올려져 있습니다. 둘 중 하나만 올려놓고 이용해주십시오.") );
					m_pLoan->SelfCloseMessageBox( sEMsg );
					return 0;
				}
			}

			if( sPreUserNo.Compare( sUserNo ) == 0 )
			{				
				ids = m_pLoan->SPFExcutePath(_T("RFID_반납"));

				
				if( m_pLoanMangeValue->m_sFeeMngSys == _T("Y") )
				{
					
					CString sQuery, sCount;
					CString strUserNo;		
					CString strRecKey;
					pDM->GetCellData( _T("대출자번호"), 0, strUserNo );
					pDM->GetCellData( _T("대출자KEY"), 0, strRecKey );
					if(!strUserNo.IsEmpty() && !strRecKey.IsEmpty())
					{
						sQuery.Format(	_T("SELECT (SELECT COUNT(1) ")
										_T("FROM CO_FEE_PAYMENT_INFO_TBL CFPI, LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU ")
										_T("WHERE CFPI.LOAN_INFO_KEY=CL.REC_KEY AND CLU.REC_KEY=CL.USER_KEY ")
										_T("AND CFPI.STATUS='N' AND CLU.REC_KEY=%s)")
										_T("+")
										_T("(SELECT COUNT(1) ")
										_T("FROM CO_FEE_PAYMENT_INFO_TBL CFPI, CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU ")
										_T("WHERE CFPI.LOAN_INFO_KEY=CL.REC_KEY AND CLU.REC_KEY=CL.USER_KEY ")
										_T("AND CFPI.STATUS='N' AND CLU.REC_KEY=%s) FROM DUAL")
										, strRecKey, strRecKey);					
						CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_DB") );
						pDM->GetOneValueQuery( sQuery , sCount );
						if( _ttoi(sCount) > 0 )
						{
							if(m_pFeeArrearReceipt != NULL)
							{
								m_pFeeArrearReceipt->ShowWindow( SW_SHOWNOACTIVATE );		
								m_pFeeArrearReceipt->AddUser(strRecKey, strUserNo);
							}
						}
					}
				}
			}
			else
			{
				CESL_DataMgr* pLoanInfoDM = FindDM( _T("DM_BO_LOC_3100_LOAN_INFO") );

				CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO") , _T("MainGrid") );

				
				CString sTagRegNo;
				CString sLoanInfoRegNo;
				CString sSerialNo;
				for( INT i = 0 ; i < pRFID_DM->GetRowCount(); i++ )
				{
					sTagRegNo.Empty();
					pRFID_DM->GetCellData( _T("이용자번호") , i , sTagRegNo );
					
					for( INT j = 0 ; j < pLoanInfoDM->GetRowCount() ; j++ )
					{
						sLoanInfoRegNo.Empty();
						pLoanInfoDM->GetCellData( _T("등록번호") , j , sLoanInfoRegNo );

						if( sTagRegNo.Compare(sLoanInfoRegNo) == 0 )
						{
							pGrid->SetAt( j , 0 , _T("V") );
						}
					}
				}
			}
		}
		return 0;
	}
	
	CESL_DataMgr* pMemberDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
	if( pMemberDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("Get_RFID_List") );


		if( nWillLoanBookCnt > 0 )
		{
			if(1 == saLoanUserNoList.GetSize())
			{
				CString sUserNo;
				sUserNo = saLoanUserNoList.GetAt(0);
				
				CString sPreUserNo;
				CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_MEMBER_INFO") );
				if( pDM->GetRowCount() > 0 )
				{
					pDM->GetCellData( _T("대출자번호") , 0 , sPreUserNo );
				}
				
				if( sPreUserNo.Compare( sUserNo ) != 0 )
				{		
					GetDlgItem(IDC_edtNUMBER)->SetWindowText(sUserNo);
					GetDlgItem(IDC_edtNAME)->SetWindowText(_T(""));
					SearchUser();		
					return 0;
				}
			}
			
			if( pMemberDM->GetRowCount() > 0 )
			{				
				ids = m_pLoan->SPFExcutePath(_T("RFID_대출"));
				
			}
			else
				m_pLoan->SelfCloseMessageBox( _T("대출하려고 하였으나 이용자가 검색되지 않아 대출할수가 없습니다.\r\n")
				                              _T("이용자를 먼저 선택해주십시오") );
		}
		else
		{			
			ids = m_pLoan->SPFExcutePath(_T("RFID_반납"));	
			
			
			if( m_pLoanMangeValue->m_sFeeMngSys == _T("Y") )
			{
				
				CString sQuery, sCount;
				CString strUserNo;		
				CString strRecKey;
				
				pMemberDM->GetCellData( _T("대출자번호"), 0, strUserNo );
				pMemberDM->GetCellData( _T("대출자KEY"), 0, strRecKey );
				if(!strUserNo.IsEmpty() && !strRecKey.IsEmpty())
				{
					sQuery.Format(	_T("SELECT (SELECT COUNT(1) ")
									_T("FROM CO_FEE_PAYMENT_INFO_TBL CFPI, LS_WORK_T01 CL, CO_LOAN_USER_TBL CLU ")
									_T("WHERE CFPI.LOAN_INFO_KEY=CL.REC_KEY AND CLU.REC_KEY=CL.USER_KEY ")
									_T("AND CFPI.STATUS='N' AND CLU.REC_KEY=%s)")
									_T("+")
									_T("(SELECT COUNT(1) ")
									_T("FROM CO_FEE_PAYMENT_INFO_TBL CFPI, CO_LOAN_TBL CL, CO_LOAN_USER_TBL CLU ")
									_T("WHERE CFPI.LOAN_INFO_KEY=CL.REC_KEY AND CLU.REC_KEY=CL.USER_KEY ")
									_T("AND CFPI.STATUS='N' AND CLU.REC_KEY=%s) FROM DUAL")
									, strRecKey, strRecKey);					
					CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3100_DB") );
					pDM->GetOneValueQuery( sQuery , sCount );
					if( _ttoi(sCount) > 0 )
					{
						if(m_pFeeArrearReceipt != NULL)
						{
							m_pFeeArrearReceipt->ShowWindow( SW_SHOWNOACTIVATE );		
							m_pFeeArrearReceipt->AddUser(strRecKey, strUserNo);
						}
					}
				}
			} 
		} 
	return 0;
EFS_END
return -1;
}

INT CBO_LOC_3100::MakeRFIDInfoList( INT nTagCnt, CStringArray &SerialList, CESL_DataMgr* pDM, CStringArray &saLoanUserNoList, CStringArray &saReturnUserNoList, CStringArray &saUserSerialList, INT &nWillLoanBookCnt, INT &nWillReturnBookCnt )
{
EFS_BEGIN

	INT				ids, nTmpValue=0;
	CRFID_Interface RFID( this );	
	CString			sWhereSerialList;	
	CString			strFuncInfo;		
	INT				nBookCnt = 0;		
	TAGDATASTRU		pTagStru;
	USERDATASTRU	pUserStru;
	CString			sSerial;
	CESL_DataMgr	TmpDM;
	TmpDM.SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	CString strBOCnt, strSECnt, strCLUCnt;

	for( INT i= 0;i < nTagCnt;i++ )
	{
		CString sRealUserNo;

		CString sQuery;
		INT		nSerialCount = 0;

		sSerial = SerialList.GetAt(i);		

		CString strSQL;
		strSQL.Format(
			_T("SELECT COUNT(1), NULL FROM BO_BOOK_TBL WHERE WORKNO='%s' ")
			_T("UNION ALL ")
			_T("SELECT COUNT(1), NULL FROM SE_BOOK_TBL WHERE WORKNO='%s' ")
			_T("UNION ALL ")
			_T("SELECT COUNT(REC_KEY), MAX(USER_NO) FROM ( SELECT REC_KEY,USER_NO FROM CO_LOAN_USER_TBL WHERE WORKNO='%s')"), 			
			sSerial, sSerial, sSerial, sSerial);
		
		TmpDM.RestructDataManager(strSQL);

		TmpDM.GetCellData(0, 0, strBOCnt); 
		TmpDM.GetCellData(1, 0, strSECnt); 
		TmpDM.GetCellData(2, 0, strCLUCnt); 

		
		if(_T("Y") == m_pLoan->m_pManageValue->m_sIsUnityLoanService
			&& 10000 != m_pInfo->MODE && 30000 != m_pInfo->MODE)
		{
			INT nBoCnt = _ttoi(strBOCnt);
			INT nSeCnt = _ttoi(strSECnt);
			INT nCluCnt = _ttoi(strCLUCnt);
			if(0 == nBoCnt && 0 == nSeCnt && 0 == nCluCnt)
			{ 
				if(m_pInfo->IsConnectUnityLoanService())
				{
					CKL_UserSearchDlg dlg(this, m_pLoan);
					dlg.m_strWorkNo = sSerial;
					dlg.m_nMode = 0;
					if(IDOK == dlg.DoModal())
					{
						ids = m_pLoan->KL_UserInfoView();
						if(0 > ids) return ids;
						return 100;
					}
				}
			}
		}

		nSerialCount = _ttoi(strBOCnt) + _ttoi(strSECnt);
		if(0 == (nSerialCount) )
		{
			CString sRealUserNo;
			if(1 == _ttoi(strCLUCnt))
			{							
				TmpDM.GetCellData(2, 1, sRealUserNo);
				sRealUserNo.TrimLeft();sRealUserNo.TrimRight();
			}
			else
			{
				
				CString strIsUserReTagging;
				m_pLoan->m_pManageValue->GetManageValue(_T("기타"), _T("공통"), _T("RFID미등록자태깅"), _T("미등록태그자동태깅사용유무"), strIsUserReTagging);

				if(_T("Y") == strIsUserReTagging)
				{
					memset( &pUserStru, 0x00, sizeof(USERDATASTRU) );
					ids = RFID.GetTagAllDataByUID( sSerial, &pTagStru );
					
					strFuncInfo.Format(_T("RFID_GetTagDataBYSerial( SerialList[i], RFM_DATA_STRU *pTagData )"));
					CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID 대출/반납"), __FILE__, __LINE__, ids );
					if( ids < 0 )
					{
						CString sEMsg;
						sEMsg.Format(_T("RFID 정보를 읽어오는데 실패했습니다."));
						m_pLoan->SelfCloseMessageBox( sEMsg );
						return 9001;
					}
					if ( RFID.GetSerialCountDB(sSerial)==2)
					{
						TCHAR szBuff[100];
						memset(szBuff, 0x00, sizeof(szBuff));
						MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (CHAR*)pUserStru.strUserNo , -1, szBuff, sizeof(szBuff));
						sRealUserNo.Format(_T("%s"), szBuff);
						if( sRealUserNo.Find(_T(":")) > 0 ) 
							sRealUserNo = sRealUserNo.Mid(0, sRealUserNo.Find(_T(":")));
						
						
						CString strUserMapTblInfo;
						m_pLoan->m_pManageValue->GetManageValue(_T("기타"), _T("공통"), _T("RFID미등록자태깅"), _T("대출자번호일괄변환여부"), strUserMapTblInfo);

						CString sValue, sUserKey;

						
						if ( strUserMapTblInfo.IsEmpty() ) strUserMapTblInfo = _T("CO_LOAN_USER_TBL");

						
						sQuery.Format(_T("SELECT COUNT(*) FROM %s WHERE USER_NO = '%s'"), strUserMapTblInfo, sRealUserNo );
						TmpDM.GetOneValueQuery(sQuery, sValue);

						if ( sValue != _T("1") ) {
							CString sEMsg;
							sEMsg.Format(_T("[ 대출자번호: %s ]는 유일한 이용자정보와 매핑이 불가능하여 UID업데이트가 불가능합니다. 재태깅하십시오."), sRealUserNo);
							m_pLoan->SelfCloseMessageBox( sEMsg );
							return 9001;
						}						

						sQuery.Format(_T("SELECT REC_KEY FROM %s WHERE USER_NO = '%s'"), strUserMapTblInfo, sRealUserNo );
						TmpDM.GetOneValueQuery(sQuery, sUserKey);
						
						
						sQuery.Format(_T("SELECT WORKNO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s "), sUserKey);
						TmpDM.GetOneValueQuery(sQuery, sValue);
						if( sValue.IsEmpty() )
						{
							
							TmpDM.StartFrame();
							TmpDM.InitDBFieldData();
							sQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO='%s' WHERE REC_KEY='%s';"), sSerial, sUserKey);
							TmpDM.AddFrame( sQuery );
							TmpDM.SendFrame();
							TmpDM.EndFrame();

							
							sQuery.Format(_T("SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s "), sUserKey);
							TmpDM.GetOneValueQuery(sQuery, sRealUserNo);
						}
						else
						{
							if( sValue != sSerial )
							{
								CString sEMsg;
								sEMsg.Format(_T("[ 대출자번호: %s ]는 사용중지된 회원증입니다."), sRealUserNo);
								m_pLoan->SelfCloseMessageBox( sEMsg );
								return 9001;
							}
						}	
					}
				} 
				else
				{
				}
			} 
			if( !sRealUserNo.IsEmpty() )
				saUserSerialList.Add( sRealUserNo );
		}
		
		else if( 1 == (nSerialCount))
		{
			if( !sWhereSerialList.IsEmpty() )
				sWhereSerialList += _T(",");
			sWhereSerialList += CString(_T("'")) + sSerial + CString(_T("'"));
			nBookCnt++;
		}
		else
		{
			
			CString sEMsg;
			sEMsg.Format(_T("중복된 RFID SERIAL이 존재합니다"));
			m_pLoan->SelfCloseMessageBox( sEMsg );
			return 9001;
		}
	} 
	
	if ( nTagCnt != ( nBookCnt + saUserSerialList.GetSize()) )
	{
		CString sEMsg;
		sEMsg.Format( _T("알수없는 RFID Tag가 '%d'개 존재하여 작업을 진행할수 없습니다") , nTagCnt - ( nBookCnt + saUserSerialList.GetSize() ) );
		m_pLoan->SelfCloseMessageBox(sEMsg);
		return 1002;
	}

	if( saUserSerialList.GetSize() > 1 )
	{
		CString sEMsg;
		sEMsg.Format( _T("회원증이 '%d'개가 검색되었습니다. 하나만 올려놓으십시오") , saUserSerialList.GetSize() );
		m_pLoan->SelfCloseMessageBox(sEMsg);
		return 1001;
	}
	
	CString sRestructSql;
	CString strTempSQL;
	
	sRestructSql.Format(
		_T("SELECT B.REC_KEY, B.REG_NO, B.WORKNO, 'B', ")
			  _T("(SELECT CL.STATUS FROM LS_WORK_T01 CL WHERE CL.STATUS IN('0','2','5','6') AND CL.BOOK_KEY = B.REC_KEY ), ")
			  _T("(SELECT MAX(CLU.USER_NO) FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY=(SELECT MAX(CL.USER_KEY) FROM LS_WORK_T01 CL WHERE CL.BOOK_KEY=B.REC_KEY AND CL.STATUS IN('0','2'))), ")
			  _T("(SELECT COUNT(1) FROM LS_WORK_T01 CL WHERE CL.STATUS='3' AND CL.BOOK_KEY=B.REC_KEY) ") 
			  _T(", B.WORKING_STATUS, (SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = B.MANAGE_CODE) AS LIB_CODE ")
		_T("FROM (SELECT * FROM BO_BOOK_TBL BB  ")
		_T("WHERE WORKNO IN (%s) ORDER BY BOOK_APPENDIX_FLAG ) B ")
		_T("UNION ALL ")
		_T("SELECT B.REC_KEY, B.REG_NO, B.WORKNO, 'S', ")
			  _T("(SELECT CL.STATUS FROM LS_WORK_T01 CL WHERE CL.STATUS IN('0','2','5','6') AND CL.BOOK_KEY=B.REC_KEY ), ")
			  _T("(SELECT MAX(CLU.USER_NO) FROM CO_LOAN_USER_TBL CLU WHERE CLU.REC_KEY=(SELECT MAX(CL.USER_KEY) FROM LS_WORK_T01 CL WHERE CL.BOOK_KEY=B.REC_KEY AND CL.STATUS IN('0','2'))), ")
			  _T("(SELECT COUNT(1) FROM LS_WORK_T01 CL WHERE CL.STATUS='3' AND CL.BOOK_KEY=B.REC_KEY) ") 
			  _T(", B.WORKING_STATUS, (SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = B.MANAGE_CODE) AS LIB_CODE ")
		_T("FROM (SELECT * FROM SE_BOOK_TBL BB ")
		_T("WHERE WORKNO IN(%s) ORDER BY BOOK_TYPE ) B ")
		, sWhereSerialList, sWhereSerialList
	);

	
	if( nBookCnt > 0 )
	{
		
		ids = TmpDM.RestructDataManager( sRestructSql );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRFIDInfoList") );

		if( TmpDM.GetRowCount() != nBookCnt )
		{
			CString sEMsg;
			sEMsg.Format( _T("알수없는 RFID Tag가 '%d'개 존재하여 작업을 진행할수 없습니다.") , nBookCnt - TmpDM.GetRowCount() );
			m_pLoan->SelfCloseMessageBox(sEMsg);
			return 1002;
		}

		
		CString sData[9];
		INT nInsertIndex = 0;
		for( INT i = 0 ; i < TmpDM.GetRowCount() ; i++ )
		{
			for( INT j = 0 ; j < 9; j++ )
			{				
				ids = TmpDM.GetCellData( i , j , sData[j] );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("MakeRFIDInfoList") );
			}

			
			if( sData[4] == _T("5") )
			{
				CString sEMsg;
				sEMsg.Format(_T("자료중 등록번호가 '%s'인 자료가 무인반납중이므로 작업을 진행할수 없습니다.\r\n무인반납자료처리를 먼저 하시고 진행해주십시오.") , sData[1] );
				m_pLoan->SelfCloseMessageBox(sEMsg);
				return 1003;			
			}
			else if( sData[4] == _T("6") )
			{
				CString sEMsg;
				sEMsg.Format(_T("자료중 등록번호가 '%s'인 자료가 분관반납중이므로 작업을 진행할수 없습니다.\r\n분관반납자료처리를 먼저 하시고 진행해주십시오.") , sData[1] );
				m_pLoan->SelfCloseMessageBox(sEMsg);
				return 1003;			
			}

			
			pDM->InsertRow(-1);


			ids = pDM->SetCellData( _T("책KEY"), sData[0], nInsertIndex);
			if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -5, _T("MakeRFIDInfoList") );
			ids = pDM->SetCellData( _T("이용자번호"), sData[1], nInsertIndex);
			if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -5, _T("MakeRFIDInfoList") );
			ids = pDM->SetCellData( _T("시리얼"), sData[2], nInsertIndex );
			if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("MakeRFIDInfoList") );
			ids = pDM->SetCellData( _T("발행형태"), sData[3], nInsertIndex );
			if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -4, _T("MakeRFIDInfoList") );
			ids = pDM->SetCellData( _T("책상태"), sData[7], nInsertIndex);
			if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -7, _T("MakeRFIDInfoList") );
			ids = pDM->SetCellData( _T("소장관도서관부호"), sData[8], nInsertIndex);
			if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -7, _T("MakeRFIDInfoList") );

			if( sData[4] == _T("0") || sData[4] == _T("2") )
			{
				nWillReturnBookCnt++;
				
				BOOL bEqual = FALSE;
				for( INT k = 0 ; k < saReturnUserNoList.GetSize() ; k++ )
				{
					if( sData[5].Compare(saReturnUserNoList.GetAt(k)) == 0 ) 
						bEqual = TRUE;
				}

				if( !bEqual )
					saReturnUserNoList.Add(sData[5]);

				ids = pDM->SetCellData( _T("반납대상이용자번호"), sData[5], nInsertIndex );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -51, _T("MakeRFIDInfoList") );

				ids = pDM->SetCellData( _T("대출여부"), _T("1"), nInsertIndex );
				if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -6, _T("MakeRFIDInfoList") );
		
			}
			else
			{
				nWillLoanBookCnt++;
				if( _ttoi(sData[6]) )
				{
					ids = pDM->SetCellData( _T("대출여부"),  _T("2"), nInsertIndex);
					if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -6, _T("MakeRFIDInfoList") );
				}
				else
				{
					if(sData[7] == _T("BOL611O"))
						ids = pDM->SetCellData( _T("대출여부"),  _T("LILL"), nInsertIndex);
					else
						ids = pDM->SetCellData( _T("대출여부"),  _T("0"), nInsertIndex);
					if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -6, _T("MakeRFIDInfoList") );
				}
			}

			nInsertIndex++;
		} 
	} 
	if( nWillReturnBookCnt*nWillLoanBookCnt != 0 )
	{
		CString sEMsg;
		sEMsg.Format( _T("대출할 자료와 반납할 자료가 함께 존재하여 기능을 수행할 수 없습니다.") );
		m_pLoan->SelfCloseMessageBox(sEMsg);
		return 1004;					
	}
	if(saLoanUserNoList.GetSize() * saReturnUserNoList.GetSize() != 0)
	{
		CString sEMsg;
		sEMsg.Format( _T("대출할 자료와 반납할 자료가 함께 존재하여 기능을 수행할 수 없습니다.") );
		m_pLoan->SelfCloseMessageBox(sEMsg);
		return 1004;
	}

	return 0;
EFS_END
return -1;
}

VOID CBO_LOC_3100::ViewRFIDTagInfo()
{
	if(_T("Y") != m_strRfidUseYN ) return;
	
	CRfidTagView Dlg(this);
	Dlg.DoModal();
	return;
}

INT CBO_LOC_3100::RFIDConnCheck(){
	CRFID_Interface RFID( this );
	BOOL bRFIDConn = TRUE;
	INT ids = -1;

	ids = RFID.GET_STATUS();
	if(ids < 0){
		bRFIDConn = FALSE;
	}
	if( bRFIDConn )
	{
		GetDlgItem(IDC_btnRFID_RECORD)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnUSE_RFID)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnUSE_ViewTag)->EnableWindow(TRUE);
	}else{
		GetDlgItem(IDC_btnRFID_RECORD)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnUSE_RFID)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnUSE_ViewTag)->EnableWindow(FALSE);
	}
	return ids;
}

VOID CBO_LOC_3100::UpdateLoanInfo()
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid") );
	
	pGrid->SelectMakeList();
	if( pGrid->SelectGetCount() <= 0 ) 
	{
		AfxMessageBox(_T("선택된 대출정보가 없습니다."));
		return;
	}
	
	CLoanInfoUpdateDlg dlg(this);
	dlg.DoModal();
	
	return;
}

VOID CBO_LOC_3100::RFIDBtnControl()
{
	EFS_BEGIN
	if ( m_strRfidUseYN == _T("Y") && m_pLoanMangeValue->m_str3100ControlYN == _T("N") ) return;	
	
	GetDlgItem(IDC_btnCheckRFID)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_btnUSE_RFID)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_btnRFID_RECORD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_btnUSE_ViewTag)->ShowWindow(SW_HIDE);
	
	CRect rfid_rc;
	CWnd* pRfidBtn = GetDlgItem(IDC_btnCheckRFID);
	pRfidBtn->GetWindowRect(&rfid_rc);
	ScreenToClient(&rfid_rc);
	
	CRect pSeaech_rc;
	CWnd* pSearchWnd;
	const INT nControlCnt = 15;
	INT ControlList[] = {
		IDC_STATIC_SEARCH,		IDC_STATIC_FAMILY,		IDC_GRID_SIMPLE_FAMILY_VIEW,IDC_btnFamily,			IDC_STATIC_USER_INFO_LINE,	
			IDC_TAB_USER_INFO,		IDC_STATIC_SIMPLE_INFO,	IDC_STATIC_USER_PICTURE,	IDC_btnSTATEMENT,		IDC_btnDETAILINFO,	
			IDC_staticPRIVATEMANAGE,IDC_btn3100Certify_Y,	IDC_btn3100Certify_N,		IDC_btn3100Certify_Fail,IDC_picUnityUserIcon
	};
	
	for (int i = 0; i < nControlCnt; i++ )
	{
		pSearchWnd = GetDlgItem(ControlList[i]);		
		pSearchWnd->GetWindowRect(&pSeaech_rc);
		ScreenToClient(&pSeaech_rc);
		
		if ( ControlList[i] == IDC_STATIC_SEARCH )
		{
			pSearchWnd->MoveWindow(pSeaech_rc.left, pSeaech_rc.top, pSeaech_rc.Width()-(rfid_rc.Width()*2), pSeaech_rc.Height());
		}
		else
		{
			pSearchWnd->MoveWindow(pSeaech_rc.left-(rfid_rc.Width()*2), pSeaech_rc.top, pSeaech_rc.Width(), pSeaech_rc.Height());
		}		
	}
	
	EFS_END
}