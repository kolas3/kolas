#include "stdafx.h"
#include "BO_LOC_3320.h"
#include "direct.h"
#include "PreviewImage.h"
#include "..\\..\\..\\공통\\paintlib\\common\\Filter\\plfilterresizebilinear.h"
#include "..\\CO_USER_GROUP_MNG\\mng_user_group_code.h"
#include "..\\BO_LOC_3100\\BO_LOC_LOAN_HIS.h"
#include "SECEDE_USER_DLG.h"
#include "DUP_USER_DLG.h"
#include "CharConvert.h"
#include "UserEditSearchDlg.h"
#include "FeeReCard.h"

#include "LoanStopDateSet.h"

#include "FrameManager.h"
#include "KL_UserCardHistoryDlg.h"

#include "KL_SearchCurrentLoanInfoDlg.h"
#include "KL_SecedeDlg.h"

#include "UserCardHistoryDlg.h"
#include "UsernoRenumberDlg.h"

#include "BO_LOC_3000Api.h"

#include "KlIdPassViewDlg.h"

#include "KL_StatementDlg.h"

#include "UserCertifyDlg.h"

#include "UserDuplecateDlg.h"

#include "KlEditPassDlg.h"

#include "CertifyMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_strIpinFlag;
CString g_strMypinCertifyFlag;
BOOL	g_bEnableCertify; 
CString g_strPhoneCertifyFlag;

CBO_LOC_3320::CBO_LOC_3320(CESL_Mgr* pParent, CLoanShareManager* pLoanShareManager, CLonaManageValue* pLoanMangeValue)
	: CESL_Mgr(IDD, pParent)
{
	m_pParent = (CBO_LOC_3300*)pParent;
	m_pLoanMangeValue = pLoanMangeValue;
	m_bCheckCenterUpload = FALSE;
	m_pSpfWork = NULL;
	m_pBmp = NULL;	
	m_pCenterUserMgr = NULL;
	m_nCurrentIndex = 0;
	m_bIsInsertBefore = FALSE;	
	m_bViewCivilNoCheckMsg = FALSE;
	m_pLoanShareManager = pLoanShareManager;
	m_bCivilNoAutoCheck = FALSE;	

	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("아이핀사용여부"), g_strIpinFlag);
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("MYPIN사용여부"), g_strMypinCertifyFlag);
	
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("개인정보보호"), _T("휴대폰인증사용여부"), g_strPhoneCertifyFlag);
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호사용유무"), m_bCivilNoDisplayCheck);
	if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0) 
	{
		
		if (g_strIpinFlag.CompareNoCase(_T("Y")) != 0 && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0)
		{
			if (m_pInfo->m_bCertifyUse == FALSE && m_pInfo->m_bKLUse == TRUE)
			{
				
				m_bCivilNoDisplayCheck = _T("Y");
			}
		}
	}

	g_bEnableCertify = FALSE;
	if (m_pInfo->m_bCertifyUse) g_bEnableCertify = TRUE; 
	if (g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0) g_bEnableCertify = TRUE; 
	
	if (g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0) g_bEnableCertify = TRUE; 
	if (m_pInfo->m_bKLUse == TRUE && m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 && !m_pParent->m_pInfo->m_bCertifyUse) g_bEnableCertify = TRUE; 
	LastGrantUserNoType = -1;
	
	m_Secede_Mode = 0;	
	m_pFamilyAddDlg = NULL;	
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("수수료관리"), _T("수수료관리시스템사용유무"), m_sFeeMngSys);
	m_bImport = FALSE;	
	m_strLastWorkUserNo = _T("");	
	m_strLastWorkUserName = _T("");	
	
	m_bSetCivil2ToPassword = FALSE;	
	m_bAutoCivilNoInsert = FALSE;	
	bFirstView = FALSE;
		
	m_nLTMemberSaveMode = 0;	
	m_bIsRealNameCert = FALSE;	
	m_bIsKLMemberYN = FALSE;	
	m_bIsSelfLibSecede = FALSE;	
	m_strKLUserKey = _T("");	
	m_strKLWORKNO = _T("");	
	m_strGPIN_ASCII = _T("");	
	m_strGPIN_HASH = _T("");	
	m_strGPIN_SEX = _T("");
	m_bIsTCTInput = FALSE;	
	m_bIsReEntry = FALSE;
	
	m_strCI = _T("");	
		
	m_strStatementAlias = _T("개인정보 수집/이용");	
	m_bIsProvideYN = FALSE;	
	m_bIsCollectYN = FALSE;
	m_bIsCollectYNBefore = FALSE;
	m_bIsProvideYNBefore = FALSE;	
	m_strAgentName = _T("");	
	m_strAgentRelation = _T("");	
	m_strAgentNote = _T("");	
	m_bUnder14YN = FALSE;	
	m_bIsAgentRealNameCertYN =FALSE;	
	m_strCertCivilNo = _T("");	
	
	m_strAgreeType = _T("0");	
	m_bKLMemeberSearch = FALSE;
	m_strKlUserNo = _T("");
	m_bIsAgreeYN = FALSE;
	
	m_strCryptoPassword = _T("");
	m_strCryptoCardPassword = _T("");	
	m_nAction = 0;
	
	m_bIsOfficeShow = FALSE;
	m_strTmpImgUserKey = _T("");
	m_strAgreeDate = _T("");
	m_strExpireDate = _T("");	
	
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("책두레시스템"), _T("책두레사용여부"), m_strUseILL);	

}

CBO_LOC_3320::~CBO_LOC_3320()
{
	if(NULL != m_pFamilyAddDlg)
	{
		delete m_pFamilyAddDlg;
		m_pFamilyAddDlg = NULL;
	}
	if(NULL != m_pSpfWork)
	{
		delete m_pSpfWork;
		m_pSpfWork = NULL;
	}
	if(NULL != m_pBmp)
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}
	if(NULL != m_pCenterUserMgr)
	{
		delete m_pCenterUserMgr;
		m_pCenterUserMgr = NULL;
	}
	CString strFileName;
	DWORD dwPID = ::GetCurrentProcessId();		
	strFileName.Format(_T("..\\cfg\\TouchPad\\BSCH(%u).dat"), dwPID);
}

VOID CBO_LOC_3320::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3320)
	DDX_Control(pDX, IDC_btn3320Certify_Fail, m_btnCertify_Fail);
	DDX_Control(pDX, IDC_btn3320Certify_N, m_btnCertify_N);
	DDX_Control(pDX, IDC_btn3320Certify_Y, m_btnCertify_Y);
	DDX_Control(pDX, IDC_STRONGSTATIC4, m_StrongStatic04);
	DDX_Control(pDX, IDC_STATIC_17, m_MarkStaticStatic17);
	DDX_Control(pDX, IDC_STATIC_11, m_MarkStaticStatic11);
	DDX_Control(pDX, IDC_lblWORK_PHONE, m_MarkStaticWorkPhone);
	DDX_Control(pDX, IDC_lblWORK_ADDRESS, m_MarkStaticWorkAddress);
	DDX_Control(pDX, IDC_lblWORK_CODE, m_MarkStaticWorkCode);
	DDX_Control(pDX, IDC_STRONGSTATIC3, m_StrongStatic03);
	DDX_Control(pDX, IDC_lblHOME_PHONE, m_MarkStaticHomePhone);
	DDX_Control(pDX, IDC_lblADDRESS, m_MarkAddress);
	DDX_Control(pDX, IDC_lblHOME_CODE, m_MakrStaticHomeCode);
	DDX_Control(pDX, IDC_STRONGSTATIC2, m_StrongStatic02);
	DDX_Control(pDX, IDC_lblUD_CODE3, m_MakeStaticUDCode3);
	DDX_Control(pDX, IDC_lblUD_CODE2, m_MakeStaticUDCode2);
	DDX_Control(pDX, IDC_lblUD_CODE1, m_MakeStaticUDCode1);
	DDX_Control(pDX, IDC_STATIC_9, m_MarkStaticStatic9);
	DDX_Control(pDX, IDC_STATIC_10, m_MarkStaticStatic10);
	DDX_Control(pDX, IDC_MAKESTATIC08, m_MarkStatic08);
	DDX_Control(pDX, IDC_MAKESTATIC07, m_MarkStatic07);
	DDX_Control(pDX, IDC_MAKESTATIC06, m_MarkStatic06);
	DDX_Control(pDX, IDC_MAKESTATIC05, m_MarkStatic05);
	DDX_Control(pDX, IDC_STC_SELF_LOAN_STATION, m_MarkStaticSelfLoanStation);
	DDX_Control(pDX, IDC_STATIC_5, m_MarkStaticStatic5);
	DDX_Control(pDX, IDC_STATIC_7, m_MarkStaticStatic7);
	DDX_Control(pDX, IDC_staFAMILYID, m_MakeStaticFamilyId);
	DDX_Control(pDX, IDC_sNUM_TYPE, m_MakeStaticNumType);
	DDX_Control(pDX, IDC_MAKESTATIC03, m_MakeStatic03);
	DDX_Control(pDX, IDC_MAKESTATIC02, m_MakeStatic02);
	DDX_Control(pDX, IDC_MAKESTATIC01, m_MakeStatic01);
	DDX_Control(pDX, IDC_STATIC_CIVILNO, m_staCivilNo);
	DDX_Control(pDX, IDC_staKLMemberYN, m_staKLMemberYN);
	DDX_Control(pDX, IDC_stcUSER_2Contacts, m_sta2Contacts);
	DDX_Control(pDX, IDC_STATIC_SEX, m_MarkStaticSex);	
	DDX_Control(pDX, IDC_MAKESTATIC04, m_MakeStatic04);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3320, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3320)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_btnLOC_USERCLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btnUSER_SAVE, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnDUPLICATE3, OnbtnDUPLICATE3)
	ON_BN_CLICKED(IDC_btnDUPLICATE1, OnbtnDUPLICATE1)
	ON_BN_CLICKED(IDC_btnDUPLICATE2, OnbtnDUPLICATE2)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnOLDDATA, OnbtnOLDDATA)
	ON_CBN_SELCHANGE(IDC_cmbUSER7, OnSelchangecmbUSER7)
	ON_BN_CLICKED(IDC_btnCLEAR, OnbtnCLEAR)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_CIVILNUM1, OnChangeedtCIVILNUM1)
	ON_EN_CHANGE(IDC_edtUSER1, OnChangeedtUSER1)	
	ON_BN_CLICKED(IDC_btnDUPLICATE4, OnbtnDUPLICATE4)
	ON_BN_CLICKED(IDC_btnFIND1, OnbtnFIND1)
	ON_BN_CLICKED(IDC_btnFIND2, OnbtnFIND2)
	ON_EN_SETFOCUS(IDC_edtUSER21, OnSetfocusedtUSER21)
	ON_BN_CLICKED(IDC_btnINPUTCENTER, OnbtnINPUTCENTER)
	ON_EN_SETFOCUS(IDC_edtUSER7, OnSetfocusedtUSER7)
	ON_EN_SETFOCUS(IDC_edtUSER10, OnSetfocusedtUSER10)
	ON_EN_SETFOCUS(IDC_edtUSER13, OnSetfocusedtUSER13)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_btnPICTURE_MODIFY, OnbtnPICTUREMODIFY)
	ON_CBN_SELCHANGE(IDC_cmbUSER5, OnSelchangecmbUSER5)
	ON_BN_CLICKED(IDC_bMONETA, OnbMONETA)
	ON_CBN_SETFOCUS(IDC_cmbUSER6, OnSetfocuscmbUSER6)
	ON_EN_KILLFOCUS(IDC_edtUSER8, OnKillfocusedtUSER8)
	ON_BN_CLICKED(IDC_btn3320_MNG_GROUP, Onbtn3320MNGGROUP)
	ON_BN_CLICKED(IDC_btnOLDDATA3, OnbtnOLDDATA3)
	ON_BN_CLICKED(IDC_btnLOANINFO, OnbtnLOANINFO)	
	ON_EN_SETFOCUS(IDC_edtUSER22, OnSetfocusedtUSER22)
	ON_EN_SETFOCUS(IDC_edtUSER23, OnSetfocusedtUSER23)
	ON_BN_CLICKED(IDC_btnSERIALNO, OnbtnSERIALNO)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_edtUSER1, OnKillfocusedtUSER1)
	ON_CBN_SELCHANGE(IDC_cmbUSER9, OnSelchangecmbUSER9)
	ON_BN_CLICKED(IDC_chkMAILUSEYN, OnchkMAILUSEYN)
	ON_CBN_SELCHANGE(IDC_cmbLOST_MEMBER, OnSelchangecmbLOSTMEMBER)
	ON_BN_CLICKED(IDC_BN_FAMILY_ADD, OnbtnFamilyAdd)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_STATIC_PICTURE_POS, OnStaticPicturePos)
	ON_BN_CLICKED(IDC_BTN_RECARD, OnBtnRecard)
	ON_EN_KILLFOCUS(IDC_edtUSER7,OnKillfocusEditName)
	ON_EN_KILLFOCUS(IDC_EDIT_CIVILNUM1,OnKillfocusEditCivilNum1)
	ON_EN_KILLFOCUS(IDC_EDIT_CIVILNUM2,OnKillfocusEditCivilNum2)
	ON_BN_CLICKED(IDC_tbnLoanStopDateSet, OnbtnLoanStopDateSet)
	ON_EN_CHANGE(IDC_edtUSER19, OnChangeedtUSER19)
	ON_EN_SETFOCUS(IDC_edtUSER16, OnSetfocusEdtUser16)
	ON_EN_SETFOCUS(IDC_edtUSER_34, OnSetfocusEdtUser34)
	ON_EN_SETFOCUS(IDC_edtUSER29, OnSetfocusEdtUser29)
	ON_BN_CLICKED(IDC_chkSMSUSEYN, OnchkSMSUSEYN)
	ON_CBN_SELCHANGE(IDC_cmbKLMemberYN, OnSelchangecmbKLMemberYN)
	ON_BN_CLICKED(IDC_btnKLRealNameCert, OnbtnKLRealNameCert)
	ON_BN_CLICKED(IDC_btnSearchKLLib, OnbtnSearchKLLib)
	ON_BN_CLICKED(IDC_btnSearchKLCurrentLoan, OnbtnSearchKLCurrentLoan)
	ON_BN_CLICKED(IDC_BTN_USER_CARD, OnbtnViewUserCardHistory)
	ON_BN_CLICKED(IDC_btnUSEREDITLOG, OnbtnUserEditLog)
	ON_BN_CLICKED(IDC_btn3320_IDPASS_VIEW, OnbtnIDPASSView)
	ON_BN_CLICKED(IDC_btn3320Certify_N, Onbtn3320CertifyN)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_btn3320Certify_Y, Onbtn3320CertifyY)
	ON_BN_CLICKED(IDC_btn3320Certify_Fail, Onbtn3320CertifyFail)
	ON_BN_CLICKED(IDC_btnAGREEMENTINFO, OnbtnAGREEMENTINFO)	
	ON_BN_CLICKED(IDC_btnCHANGEPASSWORD, OnbtnCHANGEPASSWORD)
	ON_WM_SIZE()	
	ON_BN_CLICKED(IDC_btnCHANGECARDPW, OnbtnCHANGECARDPW)	
	ON_BN_CLICKED(IDC_btnReName, OnbtnReName)
	ON_BN_CLICKED(IDC_BTN_OFFICE_SHOWHIDE, ToggleOfficeView)
	//}}AFX_MSG_MAP
	ON_MESSAGE( WM_USER_PICTURE_IMPORT , OnUserPicImport )
	ON_MESSAGE( WM_USER + 1027 , OnTouchPad )
END_MESSAGE_MAP()

BOOL CBO_LOC_3320::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3320::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();	

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	
	m_strDMAlias = _T("DM_BO_LOC_3300");
	m_strCMAlias = _T("CM_BO_LOC_3320");

	CEdit * pEdit = (CEdit*)GetDlgItem(IDC_edtUSER5);
	if( 0 > InitESL_Mgr(_T("BO_LOC_3320")) )
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	m_pSpfWork = new BL_LOC_USER_PROC(this,m_pLoanShareManager);	
	m_pSpfWork->m_strIsKLSecurityCivilNo = m_pLoanMangeValue->m_strIsKLSecurityCivilNo;
	m_pSpfWork->SetTempData(_T("주민등록번호사용여부"), CComVariant(m_bCivilNoDisplayCheck));
	m_pSpfWork->SetTempData(_T("아이핀사용여부"), CComVariant(g_strIpinFlag));
	m_pSpfWork->SetTempData(_T("MYPIN사용여부"), CComVariant(g_strMypinCertifyFlag));
	m_pSpfWork->SetTempData(_T("본인인증기능사용여부"), CComVariant( (g_bEnableCertify ? _T("Y") : _T("N")) ));	
	m_pSpfWork->SetTempData(_T("휴대폰인증사용여부"), CComVariant(g_strPhoneCertifyFlag));	
	m_pSpfWork->SetTempData(_T("KL_IMPORT_YN"), CComVariant(_T(" ")));
	
	( (CEdit*)GetDlgItem(IDC_EDIT_CIVILNUM1) )->SetLimitText(6);
	( (CEdit*)GetDlgItem(IDC_EDIT_CIVILNUM2) )->SetLimitText(7);

	TakeManageValue();		
	
	ApplyMobile();

	CLocCommonAPI::MakeHPCorpNumList( GetDlgItem( IDC_cmbUSER5 ) );
	
	CreateMyTooltip();

	InitPictureControl();

	if( 3 == m_nUserNoAutoMode && ( 1 == m_nMode || 3 == m_nMode ) )
	{
		((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(TRUE);
	}

	((CEdit*)GetDlgItem(IDC_edtUSER19))->SetLimitText(4);
	((CEdit*)GetDlgItem(IDC_edtUSER20))->SetLimitText(4);

	
	m_hBalloonIcon = (HICON)::LoadImage(AfxFindResourceHandle(MAKEINTRESOURCE(IDI_BALLOONICON), RT_GROUP_ICON), MAKEINTRESOURCE(IDI_BALLOONICON), IMAGE_ICON, 0, 0, 0);
	m_Font.DeleteObject();
	m_Font.CreatePointFont(90, _T("굴림체"));

	if(TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0 )
	{		
		m_BalloonTip.Create(this);
		
		m_btnCertify_Y.SetSkin(IDB_UC_Y1,IDB_UC_Y2,IDB_UC_Y3,IDB_UC_Y4,0,0,0,0,0);
		m_btnCertify_N.SetSkin(IDB_UC_N1,IDB_UC_N2,IDB_UC_N3,IDB_UC_N4,0,0,0,0,0);
		m_btnCertify_Fail.SetSkin(IDB_UC_FAIL1,IDB_UC_FAIL2,IDB_UC_FAIL3,IDB_UC_FAIL4,0,0,0,0,0);

		if (!g_bEnableCertify)
		{
			m_btnCertify_Y.EnableWindow(FALSE);
			m_btnCertify_N.EnableWindow(FALSE);
			m_btnCertify_Fail.EnableWindow(FALSE);
		}
		
		if (!g_bEnableCertify)
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
		
		CRect rPos;
		GetDlgItem(IDC_STATIC_PICTURE_POS)->GetWindowRect(&rPos);
		ScreenToClient(rPos);
		
		
		CRect rObject;
		GetDlgItem(IDC_btn3320Certify_Y)->GetWindowRect(&rObject);
		ScreenToClient(rObject);
		int nHeight = rObject.Height();
		int nWidth = rObject.Width();
		rObject.left = rPos.left;
		rObject.right = rPos.left + nWidth;
		rObject.top = rPos.bottom - nHeight;
		rObject.bottom = rPos.bottom;
		GetDlgItem(IDC_btn3320Certify_Y)->MoveWindow(rObject);
		
		GetDlgItem(IDC_btn3320Certify_N)->GetWindowRect(&rObject);
		ScreenToClient(rObject);
		nHeight = rObject.Height();
		rObject.left = rPos.left;
		rObject.right = rPos.left + nWidth;
		rObject.top = rPos.bottom - nHeight;
		rObject.bottom = rPos.bottom;
		GetDlgItem(IDC_btn3320Certify_N)->MoveWindow(rObject);
		
		GetDlgItem(IDC_btn3320Certify_Fail)->GetWindowRect(&rObject);
		ScreenToClient(rObject);
		nHeight = rObject.Height();
		rObject.left = rPos.left;
		rObject.right = rPos.left + nWidth;
		rObject.top = rPos.bottom - nHeight;
		rObject.bottom = rPos.bottom;
		GetDlgItem(IDC_btn3320Certify_Fail)->MoveWindow(rObject);
	}

	return TRUE;

EFS_END
return FALSE;
}

VOID CBO_LOC_3320::OnShowWindow(BOOL bShow, UINT nStatus) 
{
EFS_BEGIN

	CDialog::OnShowWindow(bShow, nStatus);

	SetScreen(bFirstView);
	bFirstView = TRUE;
	
	ShowMONETABtn();	
	if( 0 == m_nMode )
	{
		SetDefaultMail();
	}	
	ShowCTUsrBtn();	
	m_bSetCivil2ToPassword	=	TRUE;	
	GetControlData( m_strCMAlias , _T("주민등록번호") , m_strLastCivilNo );
	GetControlData( m_strCMAlias , _T("이름") , m_strLastName );	
	
	if( 0 == m_nMode && bShow == TRUE && bFirstView == TRUE && m_bIsOfficeShow == FALSE){
		ToggleOfficeView();
	}
	
	if(0 != m_nMode && bShow == TRUE )
	{		
		CString strAction = _T("");
		if(m_nAction == ACTION_READ_F_USERINFO)
		{
			strAction.Format(_T("이용자 상세조회"));
		}
		else if (m_nAction == ACTION_READ_F_LOANINFO)
		{
			strAction.Format(_T("대출자 상세조회"));
		}
		else if (m_nAction == ACTION_READ_F_LOANUSERINFO)
		{
			strAction.Format(_T("대출중 이용자 조회"));
		}		
		else if (m_nAction == ACTION_READ_F_LOANIFOMGR)
		{
			strAction.Format(_T("대출자료관리 이용자 상세조회"));
		}
		else if (m_nAction == ACTION_READ_F_DELIVERY)
		{
			strAction.Format(_T("배달자료관리 이용자 상세조회"));
		}
		else if (m_nAction == ACTION_MOD_F_USERINFO)
		{
			strAction.Format(_T("이용자 수정"));
		}

		CESL_DataMgr* pDM = FindDM(m_strDMAlias);
		if(NULL==pDM) return;
		CString strRecKey;
		pDM->GetCellData(_T("REC_KEY"), 0, strRecKey);
		if(strRecKey.IsEmpty()) return;
		CString strQuery;

		strQuery.Format(_T("INSERT INTO CO_LOAN_USER_READ_TBL")
			_T("(REC_KEY,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK,WORKER_DO,USER_TYPE) ")
			_T("VALUES(ESL_SEQ.NEXTVAL,'%s','%s','%s','KOLASIII',%s,SYSDATE,'%s','%s' , 'U' );")
			, m_pInfo->LOCAL_IP
			, m_pInfo->USER_ID
			, m_pInfo->USER_NAME
			, strRecKey
			, GetWorkLogMsg(_T("이용자상세조회"),__WFILE__,__LINE__)
			, strAction);

		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->AddFrame(strQuery);
		pDM->SendFrame(TRUE);	
		pDM->EndFrame();		
	}

	
	GetLocUserData(m_stOriData);

EFS_END
}


VOID CBO_LOC_3320::OnbtnCLOSE()
{
EFS_BEGIN
	
	INT ids;	
	CString sCMAlias = m_strCMAlias;
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if(NULL==pCM) ERROR_MSG_RETURN_VOID2( -1010 , _T("OnbtnCLOSE") );

	ids = pCM->AllControlClear();
	if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("OnbtnCLOSE") );
		
	m_sImageFilePath = _T("");
	InitPictureControl();
		
	m_Secede_Mode = 0;
	m_bViewCivilNoCheckMsg = FALSE;
		
	GetDlgItem(IDC_EDIT_CIVILNUM1)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_CIVILNUM2)->SetWindowText(_T(""));
	
	m_bImport = FALSE;

	m_strLastCivilNo = _T("");	
	m_strLastName = _T("");	
	m_nLTMemberSaveMode = 0;
	m_bIsRealNameCert = FALSE;
	m_bIsKLMemberYN = FALSE;	
	m_strKLUserKey = _T("");	
	m_strKLWORKNO = _T("");	
	m_strGPIN_ASCII = _T("");	
	m_strGPIN_HASH = _T("");
	m_strGPIN_SEX = _T("");
	m_bIsTCTInput = FALSE;	
	m_bIsReEntry = FALSE;
	
	m_strCI = _T("");

	KL_DeleteUserNoClass(_T("타도서관이용자번호구분"));

	if( TRUE == m_bIsDoModal ) OnCancel();
	else OnClose();

EFS_END
}

VOID CBO_LOC_3320::SetMode( INT nMode , bool IsDoModal/*=FALSE*/)
{
EFS_BEGIN

	m_nMode = nMode;
	m_bIsDoModal = IsDoModal;

EFS_END
}

INT CBO_LOC_3320::InitUserAccountControl()
{
EFS_BEGIN
	INT		ids;

	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) return -1;
	
	CString strPassword; 
	strPassword = pDM->GetCellData(_T("비밀번호"), 0);
	if(m_pInfo->m_bVisitorPwEncryption && 0 != m_nMode && _T("") != strPassword)
	{
		((CEdit*)GetDlgItem(IDC_edtUSER5))->ShowWindow(SW_HIDE);
		((CEdit*)GetDlgItem(IDC_edtUSER6))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staPASSWORD))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staPASSWORD2))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_btnCHANGEPASSWORD))->ShowWindow(SW_SHOW);
	}
	else if( strPassword.IsEmpty() )
	{
		((CEdit*)GetDlgItem(IDC_edtUSER5))->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_edtUSER6))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_staPASSWORD))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_staPASSWORD2))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_btnCHANGEPASSWORD))->ShowWindow(SW_HIDE);
	}

	CString strCardPassword; 
	strCardPassword = pDM->GetCellData(_T("회원증비밀번호"), 0);
	if(m_pInfo->m_bCardPwEncryption && 0 != m_nMode && _T("") != strCardPassword)
	{
		((CEdit*)GetDlgItem(IDC_edtUSER_35))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_staPASSWORD3))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_btnCHANGECARDPW))->ShowWindow(SW_SHOW);
	}
	else if( strCardPassword.IsEmpty() )
	{
		((CEdit*)GetDlgItem(IDC_edtUSER_35))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_staPASSWORD3))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_btnCHANGECARDPW))->ShowWindow(SW_HIDE);
	}

	if(m_pInfo->m_bVisitorPwEncryption && m_pInfo->m_bCardPwEncryption && 0 != m_nMode && _T("") != strPassword && _T("") != strCardPassword)
	{			
		CRect rPos;
		GetDlgItem(IDC_btnCHANGEPASSWORD)->GetWindowRect(&rPos);
		ScreenToClient(&rPos);
		rPos.left = rPos.left + 112;
		rPos.right = rPos.right + 135;
		((CButton*)GetDlgItem(IDC_btnCHANGECARDPW))->MoveWindow(rPos);
		rPos.left = rPos.left + 136;
		rPos.right = rPos.right + 140;
		((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->MoveWindow(rPos);			
	}
	return 0;
EFS_END	
return -1;
}

VOID CBO_LOC_3320::SetScreen(BOOL bFirstView)
{
EFS_BEGIN

	INT		ids;

	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) ERROR_MSG_RETURN_VOID2( -1010 , _T("SetScreen"));

	INT nAutoMode;
	
	ids = m_pSpfWork->IsAutoMode(nAutoMode);
	if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
	
	if( 1 == nAutoMode )
	{
		if( 0 == m_nMode )
		{
			
			ids = SetControlData(m_strCMAlias, _T("대출자번호"), _T("[주민번호로생성]"));
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1030 , _T("SetScreen"));

			
			( (CEdit*)GetDlgItem(IDC_edtUSER1) )->SetReadOnly(TRUE);
		}
	}
	
	else if( 2 == nAutoMode )
	{
		if( 0 == m_nMode )
		{			
			ids = SetControlData(m_strCMAlias, _T("대출자번호"), _T("[자동생성]"));
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1050 , _T("SetScreen"));

			( (CEdit*)GetDlgItem(IDC_edtUSER1) )->SetReadOnly(TRUE);
		}

		if( 0 == m_nMode ||
		    1 == m_nMode ||
		    3 == m_nMode )
		{
			GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_SHOW);
			
			if( 1 == m_nMode ||
				3 == m_nMode )
			{
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_SHOW);
			}
		}  
	}
	
	
	else if( 3 == nAutoMode )
	{
		if( 0 == m_nMode )
		{			
			ids = SetControlData(m_strCMAlias, _T("대출자번호"), _T("[사용자ID로생성]"));
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1040 , _T("SetScreen"));

			
			( (CEdit*)GetDlgItem(IDC_edtUSER1) )->SetReadOnly(TRUE);
		}
	}

	CString strQuery, strValue;
	
	CString sIsFamily;
	m_pLoanMangeValue->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("가족회원기능사용여부"), sIsFamily );
	sIsFamily.TrimLeft();	sIsFamily.TrimRight();
	if( _T("Y") != sIsFamily )
	{
		GetDlgItem(IDC_staFAMILYID )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtUSER28 )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDUPLICATE3 )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BN_FAMILY_ADD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BN_FAMILY_ADD2)->ShowWindow(SW_HIDE);
	}

	
	CButton* pCivilErrCheck = (CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO);
	CButton* pCivilDupCheck = (CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2);
	INT nCivilCheck = BST_CHECKED;
	if( _T("Y") != m_bCivilNoDisplayCheck )
	{
		nCivilCheck = BST_UNCHECKED;
		pCivilErrCheck->SetCheck(nCivilCheck);
		pCivilDupCheck->SetCheck(nCivilCheck);
		pCivilErrCheck->ShowWindow(SW_HIDE);
		pCivilDupCheck->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CIVILNO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDUPLICATE4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CIVILNUM1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_CIVILNUM2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_CIVILNUM)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_STATIC_SEX)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SEX_M)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO_SEX_F)->ShowWindow(SW_SHOW);
	}
	else
	{
		pCivilErrCheck->SetCheck(nCivilCheck);
		pCivilDupCheck->SetCheck(nCivilCheck);
		pCivilErrCheck->ShowWindow(SW_SHOW);
		pCivilDupCheck->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_CIVILNO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDUPLICATE4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CIVILNUM1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_CIVILNUM2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_CIVILNUM)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_STATIC_SEX)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SEX_M)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_SEX_F)->ShowWindow(SW_HIDE);
	}

	
	if( 10000 == m_pInfo->MODE || 30000 == m_pInfo->MODE )
	{
		GetDlgItem(IDC_btnPICTURE_MODIFY)->EnableWindow(FALSE);
	}
	
	SetSMSUseYNMode();
	
	SetMailingUseYNMode();
	
	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("대출자접근권한"), GetUserID(), strValue);
	
	if( TRUE == strValue.IsEmpty() ) 
	{
		strValue = _T("1");
	}

	if( _T("1") == strValue )
	{
		GetDlgItem(IDC_btnCLEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnOLDDATA)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnINPUTCENTER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnUSER_SAVE)->ShowWindow(SW_HIDE);

		((CECO_ControlDateEdit*)GetDlgItem(IDC_edtUSER33))->SetElemEncryption(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER_34))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER29))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER16))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER17))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER18))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER2))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER_2Contacts))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER4))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER28))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER7))->SetPasswordChar(0x2022);

		{
			GetDlgItem(IDC_btn3320_MNG_GROUP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_BN_FAMILY_ADD)->ShowWindow(SW_HIDE);

			((CEdit*)GetDlgItem(IDC_edtUSER9))->SetPasswordChar(0x2022);
			((CEdit*)GetDlgItem(IDC_edtUSER10))->SetPasswordChar(0x2022);
			((CEdit*)GetDlgItem(IDC_edtUSER11))->SetPasswordChar(0x2022);

			((CEdit*)GetDlgItem(IDC_edtUSER12))->SetPasswordChar(0x2022);
			((CEdit*)GetDlgItem(IDC_edtUSER13))->SetPasswordChar(0x2022);
			((CEdit*)GetDlgItem(IDC_edtUSER14))->SetPasswordChar(0x2022);

			((CEdit*)GetDlgItem(IDC_edtUSER19))->SetPasswordChar(0x2022);
			((CEdit*)GetDlgItem(IDC_edtUSER20))->SetPasswordChar(0x2022);

			((CEdit*)GetDlgItem(IDC_edtUSER21))->SetPasswordChar(0x2022);			
		}
	}
	
	if( 0 != m_nMode )
	{
		GetDlgItem(IDC_btnINPUTCENTER)->ShowWindow(SW_HIDE);
	}

	if( TRUE != m_bCheckCenterUpload )
	{
		CCO_CENTER_USER_USE UseCenterUser(this);
		UseCenterUser.IsUseCenterUser(m_sLibKey, m_sLibCode);
		m_bCheckCenterUpload = TRUE;
	}
	
	if ( TRUE != bFirstView )
	{
		ids = SetUserDefineCodeLine();
		if ( 0 < ids ) 
		{
			InitCtrlsPos();
		}
		else if(0>ids) ERROR_MSG_RETURN_VOID2( -1070 , _T("SetScreen"));
	}

	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("무인반납중자료상태사용여부"), _T(""), strValue);
	if( _T("Y") != strValue )
	{ 
		GetDlgItem(IDC_STC_SELF_LOAN_STATION)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CMB_SELF_LOAN_STATION)->ShowWindow(SW_HIDE);
	}
	
	if( 0 == m_nMode )
	{
		SetWindowText(_T("이용자 입력"));
		
		INT		i;
		
		if(bFirstView == FALSE){
			
			const INT nUnVisibleCnt = 24;
			INT nUnVisibleGroup[ nUnVisibleCnt ] = {
				IDC_STATIC_UNVISIBLE_1,		IDC_btnOLDDATA,         IDC_STATIC_1,			IDC_STATIC_UNVISIBLE_3,		
				IDC_btnPREV,				IDC_STATIC_7,       	IDC_STATIC_UNVISIBLE_4,	IDC_btnNEXT,
				IDC_cmbUSER7,				IDC_STATIC_UNVISIBLE_5,	IDC_btnPRINT,           IDC_cmbLOST_MEMBER,
				IDC_STATIC__UNVISIBLE_6,	IDC_STATIC_UNVISIBLE_7, IDC_btn3320_MNG_GROUP,	IDC_datUSER1,
				IDC_datUSER2,				IDC_datUSER3,			IDC_datUSER4,			IDC_tbnLoanStopDateSet,
				IDC_btn3320_IDPASS_VIEW,	IDC_picUnityUserIcon	
				,IDC_STATIC__UNVISIBLE_8,IDC_DAT_UNSUBSCRIBE_LILL
			};
			
			const INT nMoveCtrlCnt = 12;
			INT nMoveCtrlGroup[ nMoveCtrlCnt ] =
			{
				IDC_btnCLEAR,				IDC_btnOLDDATA,			IDC_btnOLDDATA3,	IDC_btnNEXT,
				IDC_btnPRINT,				IDC_btnUSER_SAVE,			IDC_btnLOC_USERCLOSE,		IDC_btnINPUTCENTER,
				IDC_btnCHECKCENTER_UPLOAD,	IDC_STATIC_BUTTON_SEP,	IDC_BN_FAMILY_ADD, IDC_btnUSEREDITLOG
			};

			INT nMovedCtrlPosGroup[ nMoveCtrlCnt ] = 
			{
				IDC_btnCLEAR2,				IDC_btnOLDDATA2,	IDC_btnPREV2,		IDC_btnNEXT2,
				IDC_btnPRINT2,				IDC_btnSAVE2,		IDC_btnCLOSE2,		IDC_btnINPUTCENTER2, 
				IDC_btnCHECKCENTER_UPLOAD2,	IDC_STATIC_MOVE_1,	IDC_BN_FAMILY_ADD2, IDC_btnUSEREDITLOG2
			};

			CRect TempRect;

			for( i=0 ; i<nUnVisibleCnt ; i++ )
			{
				GetDlgItem(nUnVisibleGroup[i])->ShowWindow(SW_HIDE);
			}

			for( i=0 ; i<nMoveCtrlCnt ; i++ )
			{
				GetDlgItem(nMovedCtrlPosGroup[i])->GetWindowRect(TempRect);
				ScreenToClient(&TempRect);
				GetDlgItem(nMoveCtrlGroup[i])->MoveWindow(TempRect);
			}

			
			GetDlgItem(IDC_INPUTMODE_SIZE)->GetWindowRect( &TempRect );
			CRect WndPos;		
			GetWindowRect( &WndPos );		
			ClientToScreen(&WndPos);
			ClientToScreen(&TempRect);
			WndPos.SetRect( WndPos.left , WndPos.top , WndPos.right , TempRect.bottom );
			ScreenToClient(&WndPos);
			WndPos.bottom += 45;
			MoveWindow(WndPos);
		}

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbLOST_MEMBER);
		for( i=0 ; i<pCombo->GetCount() ; i++ )
		{
			
			pCombo->GetLBText(i, strValue);
			if( 0 == strValue.Compare(_T("N")) )
			{
				pCombo->SetCurSel(i);
			}
		}

		
		pCombo = (CComboBox*)GetDlgItem(IDC_CMB_SELF_LOAN_STATION);
		for( i=0 ; i<pCombo->GetCount() ; i++ )
		{
			pCombo->GetLBText(i, strValue);
			if( 0 == strValue.Compare(_T("N")) )
			{
				pCombo->SetCurSel(i);
			}
		}
		
		
		
		m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("주민등록번호자동중복검사여부"), strValue);
		if ( 0 == strValue.Compare(_T("Y")) ) 
		{
			m_bCivilNoAutoCheck = TRUE;	
			((CButton*)GetDlgItem(IDC_btnDUPLICATE4))->ShowWindow(SW_HIDE);
		}
		else
		{
			m_bCivilNoAutoCheck = FALSE;
			
			if(_T("Y") == m_bCivilNoDisplayCheck)
			{
				((CButton*)GetDlgItem(IDC_btnDUPLICATE4))->ShowWindow(SW_SHOW);
			}
		}
		
		GetDlgItem(IDC_btnUSEREDITLOG)->ShowWindow( SW_HIDE );

		
		if( _T("Y") == m_sFeeMngSys )
		{
			GetDlgItem( IDC_BTN_RECARD )->ShowWindow( SW_HIDE );
		}
		
		SetDlgItemText(IDC_BN_FAMILY_ADD, _T("가족 입력"));

		
		CESL_Mgr* pMgr = pParentMgr->FindSM(_T("BO_LOC_3100"));
		if(NULL != pMgr)
		{
			GetDlgItem(IDC_btnOLDDATA3)->ShowWindow(SW_HIDE);
		}
	}
	
	else if( 1 == m_nMode || 3 == m_nMode )
	{
		SetWindowText( _T("이용자 수정") );

		const INT nUnVisibleCnt = 3;
		INT nUnVisibleGroup[nUnVisibleCnt] =
		{
			IDC_btnPRINT, IDC_btnCLEAR, IDC_btnOLDDATA3
		};

		INT		i;
		for( i=0 ; i<nUnVisibleCnt ; i++ )
		{
			GetDlgItem(nUnVisibleGroup[i])->ShowWindow(SW_HIDE);
		}
		
		if( 3 == m_nMode )
		{
			GetDlgItem(IDC_btnPREV)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_btnNEXT)->ShowWindow(SW_HIDE);
		}
		
		if( _T("Y") == m_sFeeMngSys )
		{
			GetDlgItem( IDC_BTN_RECARD )->ShowWindow( SW_SHOW );
		}
		
		SetDlgItemText(IDC_BN_FAMILY_ADD, _T("가족 수정"));

		SetCurrentIndex();
		DisplayWindow();
		
		GetDlgItemText(IDC_edtUSER22, m_sRemark);
		
		m_pSpfWork->m_bRemarkEditYN = FALSE;
		
		GetDlgItemText(IDC_edtUSER1, m_pSpfWork->m_strBeforeUserNo);		
		
		if(TRUE == m_pParent->m_pInfo->m_bAgreeInfoRecordYN)
		{
			GetDlgItem(IDC_btnAGREEMENTINFO)->ShowWindow(SW_SHOW);	
		}
		else 
		{
			CString strAgreeStatement;
			pDM->GetCellData(_T("동의약관"), 0, strAgreeStatement);
						
			if(TRUE == m_pParent->m_pInfo->m_bKLUse)
			{
				CString strGetData;
				pDM->GetCellData(_T("통합회원전환여부"), 0, strGetData);
				if(_T("Y") == strGetData)
				{
					GetDlgItem(IDC_btnAGREEMENTINFO)->ShowWindow(SW_SHOW);	
				}				
				else if(_T("") != strAgreeStatement)
				{
					GetDlgItem(IDC_btnAGREEMENTINFO)->ShowWindow(SW_SHOW);	
				}
			}
		}
		InitUserAccountControl();
	}	
	else if( 2 == m_nMode || 4 == m_nMode || 5 == m_nMode || 6 == m_nMode || 7 == m_nMode )
	{
		SetWindowText(_T("이용자 상세보기"));		
		const INT nUnVisibleCnt = 23;
		INT nUnVisibleGroup[nUnVisibleCnt] =
		{
			IDC_btnDUPLICATE1,	IDC_btnCHECK_CIVIL_NO,	IDC_edtUSER5,			IDC_btnFIND1,
			IDC_btnDUPLICATE2,	IDC_btnCHECK_CIVIL_NO2,	IDC_edtUSER6,			IDC_btnFIND2,
			IDC_btnDUPLICATE3,	IDC_btnINPUTCENTER,		IDC_edtUSER_35,			IDC_btnOLDDATA,
			IDC_btnDUPLICATE4,	IDC_btn3320_MNG_GROUP,	IDC_btnCLEAR,			IDC_btnOLDDATA3,
			IDC_staPASSWORD,	IDC_staPASSWORD2,		IDC_staPASSWORD3,		IDC_btnUSER_SAVE,
			IDC_btnPRINT,		IDC_BN_FAMILY_ADD,		IDC_tbnLoanStopDateSet			
		};

		for( INT i=0; i<nUnVisibleCnt; i++ )
		{
			GetDlgItem(nUnVisibleGroup[i])->ShowWindow(SW_HIDE);
		}		
		if( m_sFeeMngSys == _T("Y") )
		{
			GetDlgItem( IDC_BTN_RECARD )->ShowWindow( SW_HIDE );
		}
		
		if( 6 == m_nMode )
		{
			GetDlgItem(IDC_btnPREV)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_btnNEXT)->ShowWindow(SW_HIDE);
		}		
		GetDlgItem(IDC_btnAGREEMENTINFO)->ShowWindow(SW_SHOW);	
		
		if(m_pInfo->m_bVisitorPwEncryption)
		{
			((CEdit*)GetDlgItem(IDC_edtUSER5))->ShowWindow(SW_HIDE);
			((CEdit*)GetDlgItem(IDC_edtUSER6))->ShowWindow(SW_HIDE);
			((CStatic*)GetDlgItem(IDC_staPASSWORD))->ShowWindow(SW_HIDE);
			((CStatic*)GetDlgItem(IDC_staPASSWORD2))->ShowWindow(SW_HIDE);
		}
		
		if(m_pInfo->m_bCardPwEncryption)
		{
			((CEdit*)GetDlgItem(IDC_edtUSER_35))->ShowWindow(SW_HIDE);
			((CStatic*)GetDlgItem(IDC_staPASSWORD3))->ShowWindow(SW_HIDE);
		}
		SetCurrentIndex();
		DisplayWindow();
	}
		
	if( _T("Y") == m_strUseILL && 0 != m_nMode)
	{		
		const int nShowCtrlCount = 5;
		UINT uShowCtrlID[nShowCtrlCount] = 
		{
			IDC_STATIC_USER3, IDC_staILL_APL_STOP_DATE, IDC_edtILL_APL_STOP_DATE, 
				IDC_staILL_EXPIRE_RETURN_CNT, IDC_edtILL_EXPIRE_RETURN_CNT 
		};
		
		INT nCnt = 0;
		for( nCnt=0 ; nCnt<nShowCtrlCount ; nCnt++ )
		{
			CRect rCtrl;
			CWnd* pWnd = GetDlgItem(uShowCtrlID[nCnt]);
			if( NULL != pWnd && NULL != pWnd->GetSafeHwnd() )
			{
				pWnd->ShowWindow(SW_SHOW);
			}
		}
				
		CRect rDlg, rClientDlg;
		GetWindowRect(rDlg);
		
		rClientDlg = rDlg;
		ScreenToClient(rClientDlg);
		
		CRect rCtrl, rSta, rClose;
		CWnd* pWnd = GetDlgItem(IDC_staILL_APL_STOP_DATE);
		if(NULL != pWnd)
		{
			pWnd->GetWindowRect(rCtrl);
		}
		
		GetDlgItem(IDC_STATIC_2)->GetWindowRect(rSta);		
		
		GetDlgItem(IDC_btnUSER_SAVE)->GetWindowRect(rClose);
		if(rCtrl.top < rSta.bottom || (rCtrl.bottom < rClose.bottom && rCtrl.bottom > rClose.top))
		{
			rDlg.bottom += 34;
			MoveWindow(rDlg); 
						
			const INT MAX_CTRL_CNT = 19;
			UINT uMoveCtrlID[MAX_CTRL_CNT] = 
			{	
				IDC_btnCLEAR            , IDC_btnPREV                 , IDC_btnNEXT                 , IDC_btn3320_MNG_GROUP, IDC_btnUSEREDITLOG, 
				IDC_btnINPUTCENTER      , IDC_btnLOANINFO             , IDC_btnCHECKCENTER_UPLOAD   , IDC_btnUSER_SAVE     , IDC_btnLOC_USERCLOSE, 
				IDC_STATIC_BUTTON_SEP   , IDC_btnOLDDATA              , IDC_BN_FAMILY_ADD           , IDC_btnAGREEMENTINFO , IDC_STATIC_USER3     , 
				IDC_staILL_APL_STOP_DATE, IDC_edtILL_APL_STOP_DATE    , IDC_staILL_EXPIRE_RETURN_CNT, IDC_edtILL_EXPIRE_RETURN_CNT 
			};
			
			INT nMoveCtrlCount = (rCtrl.top < rSta.bottom) ? 19 : 14;
			
			for( nCnt=0 ; nCnt<nMoveCtrlCount ; nCnt++ )
			{
				CRect rCtrl;
				CWnd* pWnd = GetDlgItem(uMoveCtrlID[nCnt]);
				if( NULL != pWnd && NULL != pWnd->GetSafeHwnd() )
				{
					pWnd->GetWindowRect(rCtrl);
					ScreenToClient(rCtrl);
					rCtrl.OffsetRect(CPoint(0, 34));
					pWnd->MoveWindow(rCtrl);
				}
			}
		}
		
	}
	
	if(_T("Y") == m_pLoanMangeValue->m_strIsAutoSetUsernoToUserid)
	{
		if(3 != m_nUserNoAutoMode && 0 == m_nMode)
		{
			GetDlgItem(IDC_edtUSER4)->SetWindowText( _T("[대출자번호로생성]") );
			((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(TRUE);
		}
	}
	
	m_bIsSelfLibSecede = FALSE;
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		CString strKLMemberYN;
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
		if( 10000 == m_pInfo->MODE || 30000 == m_pInfo->MODE )
		{
			if(0==m_nMode)
			{
				strKLMemberYN = _T("N");
			}
			else if(1==m_nMode||3==m_nMode)
			{
				ids = pDM->GetCellData(_T("통합회원전환여부"), 0, strKLMemberYN);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();

				if(_T("Y")==strKLMemberYN)
				{				
					m_bIsKLMemberYN = TRUE;					
					m_bIsRealNameCert = TRUE;

					((CEdit*)GetDlgItem(IDC_EDIT_CIVILNUM1))->SetReadOnly(FALSE);
					((CEdit*)GetDlgItem(IDC_EDIT_CIVILNUM2))->SetReadOnly(FALSE);

					GetDlgItem(IDC_btnDUPLICATE4)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_SHOW);

					GetDlgItem(IDC_btnCHECK_CIVIL_NO)->EnableWindow(FALSE);
					((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO))->SetCheck(FALSE);
					GetDlgItem(IDC_btnCHECK_CIVIL_NO2)->EnableWindow(FALSE);
					((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2))->SetCheck(FALSE);

					CString strUserNo;
					ids = pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
					if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
					KL_InsertUserNoClass(strUserNo);

					((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
				
					m_nUserNoAutoMode = 2;
					m_pSpfWork->m_nAutoMode = m_nUserNoAutoMode;
				}

				ids = pDM->GetCellData(_T("GPIN_SEX"), 0, m_strGPIN_SEX);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_SEX.TrimLeft();	m_strGPIN_SEX.TrimRight();

				ids = pDM->GetCellData(_T("GPIN_ASCII"), 0, m_strGPIN_ASCII);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_ASCII.TrimLeft();	m_strGPIN_ASCII.TrimRight();

				
				ids = pDM->GetCellData(_T("GPIN_HASH"), 0, m_strGPIN_HASH);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_HASH.TrimLeft();	m_strGPIN_HASH.TrimRight();

				ids = pDM->GetCellData(_T("GPIN_ASCII"), 0, m_strGPIN_ASCII);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_ASCII.TrimLeft();	m_strGPIN_ASCII.TrimRight();

				ids = pDM->GetCellData(_T("GPIN_SEX"), 0, m_strGPIN_SEX);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_SEX.TrimLeft();	m_strGPIN_SEX.TrimRight();

				CString strCertFlag;
				ids = pDM->GetCellData(_T("실명인증코드"), 0, strCertFlag);
				if(!strCertFlag.IsEmpty()) m_bIsRealNameCert = TRUE;
			}
			else
			{
				ids = pDM->GetCellData(_T("통합회원전환여부"), 0, strKLMemberYN);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();
			}

			GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_HIDE);			
			GetDlgItem(IDC_btnSearchKLLib)->ShowWindow(SW_HIDE);			
			GetDlgItem(IDC_btnSearchKLCurrentLoan)->ShowWindow(SW_HIDE);	
			GetDlgItem(IDC_BTN_USER_CARD)->ShowWindow(SW_HIDE);				
			
			GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_HIDE);

		
			GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_HIDE);
			pCombo->EnableWindow(FALSE);
		}
		else
		{
			if(0==m_nMode)
			{
				strKLMemberYN = _T("N");
		
				if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 && !m_pParent->m_pInfo->m_bCertifyUse)
				{					
					GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_SHOW);
				}
				else
				{
					GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_HIDE);
				}
				GetDlgItem(IDC_btnSearchKLLib)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_btnSearchKLCurrentLoan)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_HIDE);
				pCombo->EnableWindow(TRUE);

				
				((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->ShowWindow(SW_HIDE);
			}
			else if(1==m_nMode||3==m_nMode)
			{
				ids = pDM->GetCellData(_T("통합회원전환여부"), 0, strKLMemberYN);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();

				if(_T("Y")==strKLMemberYN)
				{
					
					m_bIsKLMemberYN = TRUE;

					
					m_bIsRealNameCert = TRUE;
					
					GetDlgItem(IDC_btnDUPLICATE4)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_SHOW);
					
					GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_SHOW);


					((CEdit*)GetDlgItem(IDC_EDIT_CIVILNUM1))->SetReadOnly(FALSE);
					((CEdit*)GetDlgItem(IDC_EDIT_CIVILNUM2))->SetReadOnly(FALSE);

					GetDlgItem(IDC_btnCHECK_CIVIL_NO)->EnableWindow(FALSE);
					((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO))->SetCheck(FALSE);
					GetDlgItem(IDC_btnCHECK_CIVIL_NO2)->EnableWindow(FALSE);
					((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2))->SetCheck(FALSE);

					GetDlgItem(IDC_btnSearchKLLib)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_btnSearchKLCurrentLoan)->ShowWindow(SW_SHOW);

					CString strUserNo;
					ids = pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
					if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
					KL_InsertUserNoClass(strUserNo);

					GetDlgItem(IDC_BTN_USER_CARD)->ShowWindow(SW_SHOW);
					((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
					pCombo->EnableWindow(FALSE);
					((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("책이음회원증\r\n발급이력"));
					CString strPassword, strCardPassword;
					strPassword = pDM->GetCellData(_T("비밀번호"), 0);
					strCardPassword = pDM->GetCellData(_T("회원증비밀번호"), 0);
					if(m_pInfo->m_bVisitorPwEncryption && m_pInfo->m_bCardPwEncryption && _T("") != strPassword && _T("") != strCardPassword)
					{
						
						((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("책이음회원증 발급이력"));	
					}

					
					m_nUserNoAutoMode = 2;
					m_pSpfWork->m_nAutoMode = m_nUserNoAutoMode;
				}
				else
				{
					strKLMemberYN = _T("N");
					
					if(TRUE == m_pParent->m_pInfo->m_bKLUse && TRUE == m_pParent->m_pInfo->m_bCertifyUse)
					{
						((CButton*)GetDlgItem(IDC_btnKLRealNameCert))->ShowWindow(SW_HIDE);
					}
					else
					{
						((CButton*)GetDlgItem(IDC_btnKLRealNameCert))->ShowWindow(SW_SHOW);
					}
					
					
					if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 && !m_pParent->m_pInfo->m_bCertifyUse)
					{					
						GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_SHOW);
					}
					else
					{
						GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_HIDE);
					}
					GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_btnSearchKLLib)->ShowWindow(SW_HIDE);
					GetDlgItem(IDC_btnSearchKLCurrentLoan)->ShowWindow(SW_HIDE);
					
					GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_HIDE);

					GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_HIDE);

					
					((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증\r\n발급이력"));
					
					
					CString strPassword, strCardPassword;
					pDM->GetCellData(_T("비밀번호"), 0, strPassword);
					pDM->GetCellData(_T("회원증비밀번호"), 0, strCardPassword);
					if(m_pInfo->m_bVisitorPwEncryption && m_pInfo->m_bCardPwEncryption && _T("") != strPassword && _T("") != strCardPassword)
					{
						((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증 발급이력"));	
					}
				}
				
				((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->ShowWindow(SW_SHOW);
				
				ids = pDM->GetCellData(_T("GPIN_HASH"), 0, m_strGPIN_HASH);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_HASH.TrimLeft();	m_strGPIN_HASH.TrimRight();

				ids = pDM->GetCellData(_T("GPIN_ASCII"), 0, m_strGPIN_ASCII);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_ASCII.TrimLeft();	m_strGPIN_ASCII.TrimRight();

				ids = pDM->GetCellData(_T("GPIN_SEX"), 0, m_strGPIN_SEX);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_SEX.TrimLeft();	m_strGPIN_SEX.TrimRight();

				CString strCertFlag;
				ids = pDM->GetCellData(_T("실명인증코드"), 0, strCertFlag);
				if(!strCertFlag.IsEmpty()) m_bIsRealNameCert = TRUE;
			}
			else
			{
				ids = pDM->GetCellData(_T("통합회원전환여부"), 0, strKLMemberYN);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				strKLMemberYN.TrimLeft();	strKLMemberYN.TrimRight();

				if(_T("Y")==strKLMemberYN)
				{
					m_bIsKLMemberYN = TRUE;					
					m_bIsRealNameCert = TRUE;

					GetDlgItem(IDC_btnSearchKLLib)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_btnSearchKLCurrentLoan)->ShowWindow(SW_SHOW);
					GetDlgItem(IDC_BTN_USER_CARD)->ShowWindow(SW_SHOW);

					CString strUserNo;
					ids = pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
					if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
					KL_InsertUserNoClass(strUserNo);

					
					((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("책이음회원증\r\n발급이력"));
					
					GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_SHOW);

					GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_SHOW);
				}
				else
				{
					strKLMemberYN = _T("N");
					GetDlgItem(IDC_btnSearchKLCurrentLoan)->ShowWindow(SW_HIDE);

					
					((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증\r\n발급이력"));
					
					GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_HIDE);

					GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_HIDE);
				}
				
				((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->ShowWindow(SW_SHOW);

				GetDlgItem(IDC_btnSearchKLLib)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_HIDE);
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
				pCombo->EnableWindow(FALSE);

				
				ids = pDM->GetCellData(_T("GPIN_HASH"), 0, m_strGPIN_HASH);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_HASH.TrimLeft();	m_strGPIN_HASH.TrimRight();

				ids = pDM->GetCellData(_T("GPIN_ASCII"), 0, m_strGPIN_ASCII);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_ASCII.TrimLeft();	m_strGPIN_ASCII.TrimRight();

				ids = pDM->GetCellData(_T("GPIN_SEX"), 0, m_strGPIN_SEX);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1020 , _T("SetScreen"));
				m_strGPIN_SEX.TrimLeft();	m_strGPIN_SEX.TrimRight();
			}
		}

		if(_T("Y")==strKLMemberYN) pCombo->SetCurSel(0);
		else pCombo->SetCurSel(1);

		GetDlgItem(IDC_staKLMemberYN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbKLMemberYN)->ShowWindow(SW_SHOW);
		if (1 == m_nMode)
		{	
			{
				CString strGetData;
				pDM->GetCellData(_T("실명인증코드"), 0, strGetData);
				if (g_bEnableCertify && !strGetData.IsEmpty())
				{
					GetDlgItem(IDC_btnReName)->ShowWindow(SW_SHOW);					
				}
				else
				{
					GetDlgItem(IDC_btnReName)->ShowWindow(SW_HIDE);					
				}				
			}

		}
		
		if (0 == m_nMode)
		{	
			
			if (!g_bEnableCertify)
			{
				GetDlgItem(IDC_cmbKLMemberYN)->EnableWindow(FALSE);				
			}
			else
			{
				GetDlgItem(IDC_cmbKLMemberYN)->EnableWindow(TRUE);				
			}
		}
		else if (1 == m_nMode || 3 == m_nMode)
		{
			
			if (!m_bIsKLMemberYN)
			{
				CString strGetData;
				pDM->GetCellData(_T("실명인증코드"), 0, strGetData);
				if (!g_bEnableCertify && strGetData.IsEmpty())
				{
					GetDlgItem(IDC_cmbKLMemberYN)->EnableWindow(FALSE);
				}
				else
				{
					GetDlgItem(IDC_cmbKLMemberYN)->EnableWindow(TRUE);
				}				
			}
		}
	}
	else
	{		
		((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증\r\n발급이력"));
		
		
		CString strPassword, strCardPassword;
		strPassword = pDM->GetCellData(_T("비밀번호"), 0);
		strCardPassword = pDM->GetCellData(_T("회원증비밀번호"), 0);
		if(m_pInfo->m_bVisitorPwEncryption && m_pInfo->m_bCardPwEncryption && _T("") != strPassword && _T("") != strCardPassword)
		{			
			((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증 발급이력"));	
		}
		
		if(0==m_nMode) ((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->ShowWindow(SW_HIDE);
		else ((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSearchKLLib)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSearchKLCurrentLoan)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_staKLMemberYN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbKLMemberYN)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_HIDE);

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
		pCombo->SetCurSel(1);
	}

	if(TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
	{
		m_bKLMemeberSearch = FALSE;
		m_strKlUserNo = _T("");
		m_strCertCivilNo = _T("");

		m_pSpfWork->m_strCertifyCode = _T("");
		m_pSpfWork->m_strCertifyFailCode = _T("");
		m_pSpfWork->m_strCertifyDate = _T("");
		m_pSpfWork->m_strCertifySex = _T("");
		m_pSpfWork->m_strCertifyWorker = _T("");
		m_pSpfWork->m_strCertifyName = _T("");
		m_pSpfWork->m_strCertifyCivilNo = _T("");

		
		CString strUserNoCode;
		CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(this, strUserNoCode);
		CString strYear = strUserNoCode.Left(4);
		strUserNoCode = m_pInfo->LIB_CODE + strYear.Right(2);
		CESL_ControlMultiComboBox* pCombo = (CESL_ControlMultiComboBox*)FindControl(m_strCMAlias, _T("이용자번호구분"));
		int nComboCnt = pCombo->GetRowCount();
		BOOL bFind = FALSE;
		CString strCode;
		for(INT i = 0; i < nComboCnt; i++)
		{
			pCombo->GetData(strCode, i, 0);
			strCode.TrimLeft(); strCode.TrimRight();
			if(strCode == strUserNoCode)
			{
				pCombo->SetCurSel(i);
				bFind = TRUE;
				break;
			}
		}
		
		if(FALSE == bFind && m_pInfo->m_bCertifyUse)
		{
			MessageBox(_T("번호구분에 현재년도의 값이 존재하지 않습니다.\r\n일반부호관리의 이용자번호구분의 코드를 입력하여 주십시오.\r\n메뉴 : 공통 > 관리 > 업무환경설정 > 일반부호관리"), _T("대출자번호 자동생성"), MB_OK|MB_ICONINFORMATION);
		}
		
		((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
		
		GetDlgItem(IDC_btnDUPLICATE4)->ShowWindow(FALSE);
		
		GetDlgItem(IDC_btnDUPLICATE1)->ShowWindow(FALSE);
		if(0 == m_nMode)
		{ 
			
			m_btnCertify_N.ShowWindow((g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || m_pInfo->m_bCertifyUse));

			m_btnCertify_Y.ShowWindow(FALSE);
			m_btnCertify_Fail.ShowWindow(FALSE);
		
			m_nCertifyState = 2;

			GetDlgItem(IDC_edtUSER7)->EnableWindow(TRUE);
			
			if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0)
			{
				GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(TRUE);
			}
			if (m_bCivilNoDisplayCheck.CompareNoCase(_T("N")) == 0)
			{
				GetDlgItem(IDC_RADIO_SEX_M)->EnableWindow(TRUE);
				GetDlgItem(IDC_RADIO_SEX_F)->EnableWindow(TRUE);
			}
		}
		else
		{
			CString strGetData;
			pDM->GetCellData(_T("실명인증코드"), 0, strGetData);
			m_pSpfWork->m_strCertifyCode = strGetData;
			pDM->GetCellData(_T("실명인증일자"), 0, strGetData);
			m_pSpfWork->m_strCertifyDate = strGetData;
			pDM->GetCellData(_T("GPIN_SEX"), 0, strGetData);
			m_pSpfWork->m_strCertifySex = strGetData;
			pDM->GetCellData(_T("실명인증작업자"), 0, strGetData);
			m_pSpfWork->m_strCertifyWorker = strGetData;
			pDM->GetCellData(_T("이름"), 0, strGetData);
			m_pSpfWork->m_strCertifyName = strGetData;

			if(TRUE == m_pInfo->m_bKLUse)
			{
				pDM->GetCellData(_T("주민등록번호_CRYPTO"), 0, strGetData);
				this->ESL_DES_Decryption(strGetData, m_pSpfWork->m_strCertifyCivilNo);
			}
			else
			{
				pDM->GetCellData(_T("주민등록번호"), 0, strGetData);
				m_pSpfWork->m_strCertifyCivilNo = strGetData;
			}

			if(!m_pSpfWork->m_strCertifyCode.IsEmpty())
			{ 

				m_btnCertify_Y.ShowWindow(TRUE);
				m_btnCertify_N.ShowWindow(FALSE);
				m_btnCertify_Fail.ShowWindow(FALSE);
				m_nCertifyState = 1;
				
				if (m_bIsKLMemberYN || m_pParent->m_pInfo->m_bCertifyUse|| g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0 )
				
				{
					
					
					if (!m_pSpfWork->m_strCertifyName.IsEmpty())
					{
						GetDlgItem(IDC_edtUSER7)->EnableWindow(FALSE);
					}
					if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 )
					{
						GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(FALSE);
						GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(FALSE);
					}
					if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0 && !m_pSpfWork->m_strCertifySex.IsEmpty())
					{
						GetDlgItem(IDC_RADIO_SEX_M)->EnableWindow(FALSE);
						GetDlgItem(IDC_RADIO_SEX_F)->EnableWindow(FALSE);
					}
				}
				
				pDM->GetCellData(_T("실명인증일자"), 0, strGetData);
				m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_Y));
				
				CString strValue;
				if(!strGetData.IsEmpty()) strValue.Format(_T("본인인증한 이용자입니다.\r\n인증일자 : %s"), strGetData);
				else strValue = _T("본인인증한 이용자입니다.");
				m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Y), strValue);
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
					
					if(1 == m_nMode || 3 == m_nMode)
					{ 
						m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_N));
						if (!g_bEnableCertify)
						{
							m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증 대상 이용자입니다."));
						}
						else
						{
							m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증을 하기 위해서는\r\n이 버튼을 클릭해 주세요."));
						}
					}
					else
					{ 
						m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_N));
						m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증 대상 이용자입니다."));
					}
				}
				else
				{ 
					
					m_pSpfWork->m_strCertifyFailCode = strGetData;

					m_btnCertify_Fail.ShowWindow(TRUE);
					m_btnCertify_Y.ShowWindow(FALSE);
					m_btnCertify_N.ShowWindow(FALSE);

					
					m_nCertifyState = 3;

					
					m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_Fail));
					CString strValue;
					strValue.Format(_T("%d"), m_pInfo->m_nCertifyAgency);
					CString strDesc = _T(""), strAction = _T("");
					m_pInfo->GetCertifyFailCodeToDesc(strValue, strGetData, strDesc, strAction);
					
					if(1 == m_nMode || 3 == m_nMode)
					{
						if(strDesc.IsEmpty()) strValue = _T("본인인증을 실패한 이용자입니다.");
						else strValue = _T("실패사유 : ") + strDesc + _T("\r\n본인인증을 하기 위해서는 이 버튼을 클릭해 주세요.");
					}
					else
					{
						if(strDesc.IsEmpty()) strValue = _T("본인인증을 실패한 이용자입니다.");
						else strValue = _T("실패사유 : ") + strDesc;
					}

					
					if (!g_bEnableCertify)
					{
						strValue = _T("본인인증을 실패한 이용자입니다.");
					}
					
					m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Fail), strValue);
				}

				GetDlgItem(IDC_edtUSER7)->EnableWindow(TRUE);

				if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0)
				{
					GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(TRUE);
					GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(TRUE);
				}
				
				if (m_bCivilNoDisplayCheck.CompareNoCase(_T("N")) == 0)
				{
					GetDlgItem(IDC_RADIO_SEX_M)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_SEX_F)->EnableWindow(TRUE);
				}
			}			
		}
	}
	else
	{
		m_btnCertify_Fail.ShowWindow(FALSE);
		m_btnCertify_Y.ShowWindow(FALSE);
		m_btnCertify_N.ShowWindow(FALSE);
		
		m_nCertifyState = 0;

		if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0)
		{			
			if (0 != m_nMode)
			{				
				CString strGetData;
				pDM->GetCellData(_T("GPIN_SEX"), 0, strGetData);
				m_pSpfWork->m_strCertifySex = strGetData;
			}
		}
	}
	if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0)
	{
		CButton* pbtnM = (CButton*)GetDlgItem(IDC_RADIO_SEX_M);
		CButton* pbtnF = (CButton*)GetDlgItem(IDC_RADIO_SEX_F);
		CButton* pbtnDo = NULL;
		CButton* pbtnUn = NULL;
		if		(m_pSpfWork->m_strCertifySex.CompareNoCase(_T("0")) == 0 || 0 == m_nMode /*입력일 경우 기본값 남성*/)
		{
			pbtnDo = pbtnM;
			pbtnUn = pbtnF;
		}
		else if (m_pSpfWork->m_strCertifySex.CompareNoCase(_T("1")) == 0)
		{
			pbtnDo = pbtnF;
			pbtnUn = pbtnM;
		}

		if (pbtnDo)
		{
			pbtnDo->SetCheck(TRUE);
			pbtnUn->SetCheck(FALSE);
		}
		else
		{
			pbtnM->SetCheck(FALSE);
			pbtnF->SetCheck(FALSE);
		}
	}
	pDM->GetCellData(_T("동의약관"), 0, m_strBeforeAgreeStatement);

EFS_END
}

INT CBO_LOC_3320::SetUserDefineCodeLine()
{
EFS_BEGIN

	const INT MAX_CODE_CNT = 3;
	const INT SELECT_CNT = 2;
	INT ids = -1;
	INT nShowCnt = 0;
	INT nClassCode[MAX_CODE_CNT] = { 94, 95, 96 };
	UINT nLabel[MAX_CODE_CNT] = { IDC_lblUD_CODE1, IDC_lblUD_CODE2, IDC_lblUD_CODE3 };
	UINT nCombo[MAX_CODE_CNT] = { IDC_cmbUD_CODE1, IDC_cmbUD_CODE2, IDC_cmbUD_CODE3 };
	CString sGetData[SELECT_CNT] = { _T(""), _T("") };
	CString sUseUserCode = _T("Y");

	CString sQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USERDEFINECODE'");
	CESL_DataMgr *pDM = FindDM( m_strDMAlias );
	if( NULL == pDM ) 
	{
		return -10;
	}

	ids = pDM->GetOneValueQuery( sQuery, sUseUserCode );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SetUserDefineCodeLine") );
	}

	if( _T("N") == sUseUserCode )
	{
		m_pSpfWork->SetUserDefineCodeUseMode( FALSE );
		return 0;
	}
	m_pSpfWork->SetUserDefineCodeUseMode( TRUE );

	
	INT		i;
	for ( i=0 ; i<MAX_CODE_CNT ; i++ )
	{
		sQuery.Format( _T("SELECT USE_YN, CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = %d"), nClassCode[i] );
		ids = pDM->GetValuesQuery( sQuery, sGetData, SELECT_CNT );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020, _T("SetUserDefineCodeLine") );
		}

		if ( _T("Y") == sGetData[0] )
		{
			GetDlgItem(nLabel[i])->ShowWindow( TRUE );
			GetDlgItem(nLabel[i])->SetWindowText( sGetData[1] );
			GetDlgItem(nCombo[i])->ShowWindow( TRUE );
			nShowCnt++;
		}
	}

	if( 0 < nShowCnt )
	{
		GetDlgItem(IDC_grpUSER_DEFINE)->ShowWindow( TRUE );
	}
	
	return nShowCnt;

EFS_END
return -1;
}

INT CBO_LOC_3320::InitCtrlsPos()
{
EFS_BEGIN

	INT	MOVE_CONSTANT;
	INT	  nMoveCnt;	
	UINT *unMoveCtrlsAlias;	
	
	UINT unMoveCtrlsAliasNormal[96]= {
		IDC_grpHOME					, IDC_lblHOME_CODE			, IDC_edtUSER9				, IDC_lblADDRESS		, IDC_edtUSER10				,
		IDC_btnFIND1				, IDC_lblHOME_PHONE			, IDC_edtUSER11				, IDC_STATIC_6			, IDC_grpWORK				,
		IDC_lblWORK_CODE			, IDC_edtUSER12				, IDC_lblWORK_ADDRESS		, IDC_edtUSER13			, IDC_btnFIND2				,
		IDC_lblWORK_PHONE			, IDC_edtUSER14				, IDC_STATIC_11				, IDC_edtUSER16			, IDC_STATIC_12				,
		IDC_edtUSER17				, IDC_STATIC_13				, IDC_edtUSER18				, IDC_STATIC_14			, IDC_edtUSER2				,
		IDC_cMONETA					, IDC_bMONETA				, IDC_STATIC_UNVISIBLE_2	, IDC_edtUSER22			, IDC_STATIC_MOVE_1			,
		IDC_STATIC_UNVISIBLE_1		, IDC_STATIC_UNVISIBLE_3	, IDC_cmbUSER7				, IDC_STATIC_UNVISIBLE_4, IDC_datUSER1				,
		IDC_STATIC_UNVISIBLE_5		, IDC_datUSER2				, IDC_STATIC__UNVISIBLE_6	, IDC_datUSER3			, IDC_STATIC_UNVISIBLE_7	,
		IDC_datUSER4				, IDC_btnOLDDATA2			, IDC_btnPREV2				, IDC_btnNEXT2			, IDC_btnINPUTCENTER2		,
		IDC_btnCHECKCENTER_UPLOAD2	, IDC_btnSAVE2				, IDC_btnCLOSE2				, IDC_btnPRINT2			, IDC_STATIC_1				,
		IDC_edtUSER23				, IDC_STATIC_2				, IDC_lblLOAN_BOOK_CNT		, IDC_edtUSER24			, IDC_lblCUR_DELAY_BOOK_CNT	,
		IDC_edtUSER25				, IDC_lblTOTAL_ARREAR		, IDC_edtUSER26				, IDC_lblDELAY_CNT		, IDC_edtUSER27				,
		IDC_lblDELAYDAY_CNT			, IDC_edtUSER32				, IDC_STATIC_BUTTON_SEP		, IDC_btnCLEAR			, IDC_btnOLDDATA			,
		IDC_edtMODE					, IDC_btnPRINT				, IDC_btnPREV				, IDC_btnNEXT			, IDC_btnINPUTCENTER		,
		IDC_btn3320_MNG_GROUP		, IDC_btnCHECKCENTER_UPLOAD	, IDC_btnUSER_SAVE			, IDC_btnLOC_USERCLOSE	, IDC_INPUTMODE_SIZE		,
		IDC_btnCLEAR2				, IDC_btnOLDDATA3			,		
		IDC_edtUSER29				, IDC_edtUSER_34			, IDC_STATIC_15				, IDC_STATIC_17         , IDC_btnLOANINFO			,		
		IDC_STRONGSTATIC2			, IDC_STRONGSTATIC3			, IDC_STRONGSTATIC4			, IDC_STRONGSTATIC5		, IDC_tbnLoanStopDateSet	,
		IDC_BN_FAMILY_ADD			, IDC_BN_FAMILY_ADD2		, IDC_btnUSEREDITLOG		, IDC_btnUSEREDITLOG2,
		IDC_stcUSER_2Contacts		, IDC_edtUSER_2Contacts	
		, IDC_btnAGREEMENTINFO
		
		,IDC_STATIC__UNVISIBLE_8,IDC_DAT_UNSUBSCRIBE_LILL
	};
	unMoveCtrlsAlias = unMoveCtrlsAliasNormal;
	
	nMoveCnt = 94;
	MOVE_CONSTANT = 34;
	
	CRect ctrlRect;

	INT		i;
	for ( i=0 ; i<nMoveCnt ; i++ )
	{
		GetDlgItem(unMoveCtrlsAlias[i])->GetWindowRect( ctrlRect );
		ScreenToClient( &ctrlRect );
		GetDlgItem(unMoveCtrlsAlias[i])->MoveWindow( ctrlRect.left, (ctrlRect.top+MOVE_CONSTANT), ctrlRect.Width( ), ctrlRect.Height( ) );
	}

	
	if ( 0 != m_nMode )
	{
		GetWindowRect( ctrlRect );
		ClientToScreen( &ctrlRect );		
		ctrlRect.SetRect( ctrlRect.left , ctrlRect.top , ctrlRect.right , (ctrlRect.bottom+MOVE_CONSTANT) );
		ScreenToClient( &ctrlRect );		
		MoveWindow( ctrlRect );
	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3320::OnbtnSAVE() 
{
EFS_BEGIN
	
	
	CString strSelectKLMemberYN = _T("");
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
		pCombo->GetLBText(pCombo->GetCurSel(), strSelectKLMemberYN);
		if(_T("Y") == strSelectKLMemberYN)
		{

			
			BOOL bChk = FALSE;
			bChk = ((CButton*)GetDlgItem(IDC_RADIO_SEX_M))->GetCheck();
			if(!bChk)
			{
				bChk = ((CButton*)GetDlgItem(IDC_RADIO_SEX_F))->GetCheck();
			}
			if(!bChk)
			{
				AfxMessageBox(_T("성별을 선택하시기 바랍니다."), MB_OK);
				return;
			}			
			if(!m_bIsKLMemberYN)
			{
				if(!m_bIsRealNameCert)
				{															
					if(0 > MakeAndShowStatement(strSelectKLMemberYN))
					{
						return;
					}

					if(TRUE != m_pInfo->m_bCertifyUse && g_strIpinFlag.CompareNoCase(_T("Y")) != 0 && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0)
			
					{
						INT ids = KLRealNameCert();
						if(!m_bIsRealNameCert) return;
					}
					else
					{	
						if(m_bIsTCTInput)
 						{
 							
 						}
 						else
 						{
 							m_nLTMemberSaveMode = 2; 
 						}			
					}
				}
				else
				{
					if(0 > MakeAndShowStatement(strSelectKLMemberYN))
					{
						return;
					}
					if(TRUE != m_pInfo->m_bCertifyUse && g_strIpinFlag.CompareNoCase(_T("Y")) != 0 && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0)
					{
						INT ids = KLRealNameCert();
						if(!m_bIsRealNameCert) return;
					}

					if(m_bIsTCTInput)
					{
						if(0==m_nMode) m_nLTMemberSaveMode = 4;	
						else m_nLTMemberSaveMode = 5; 
					}
					else
					{						
						if(0==m_nMode) m_nLTMemberSaveMode = 1; 
						
						else m_nLTMemberSaveMode = 2; 
					}
				}

				
 				if(2==m_nLTMemberSaveMode)
				{
					BOOL bIsKLUserNo=TRUE;
					CString strData;
					GetControlData(m_strCMAlias,_T("대출자번호"),strData);
					INT nLength = strData.GetLength();
					if(14>nLength)
					{
						bIsKLUserNo = FALSE;
					}
					
					if(bIsKLUserNo)
					{
						for(INT i=0; i<nLength; i++)
						{
							TCHAR cValue = strData.Mid(i, 1).GetAt(0);
							if( !(cValue>='0' && cValue<='9') )
							{
								bIsKLUserNo = FALSE;
								break;
							}
						}
					}
					
					if(!bIsKLUserNo)
					{
						CUsernoRenumberDlg dlg(m_pSpfWork,this);
						dlg.SetMode(1);
						if(IDOK!=dlg.DoModal()) 
						{
							AfxMessageBox(_T("이용자번호구분이 선택되지 않았습니다."));
							return;
						}
						KL_InsertUserNoClass(dlg.GetUserNoClass());
					}
				}
			}
			else
			{				
				if(!m_bIsRealNameCert)
				{										
					if(0 > MakeAndShowStatement(strSelectKLMemberYN))
					{
						return;
					}				
					if(TRUE != m_pInfo->m_bCertifyUse && g_strIpinFlag.CompareNoCase(_T("Y")) != 0 && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0)
					{
						INT ids = KLRealNameCert();
						if(!m_bIsRealNameCert) return;
					}
				}

				m_nLTMemberSaveMode = 3;
			}
		}
		else
		{
			m_nLTMemberSaveMode = 0;
		}
	}
	else
	{
		m_nLTMemberSaveMode = 0;
	}
	
	m_strLastWorkUserNo = _T("");	
	m_strLastWorkUserName = _T("");	

	CString sGetData, sBeforeLoanStopDate, sAfterLoanStopDate, sQuery, sUserNO, sUserID, sUserName;
	BOOL bEdit = FALSE;
	
	CString strBeforeCertifyCode = _T("");
	BOOL bCertifyEdit = FALSE;

	INT		ids;
	CString strMessage;
	
	CString strAddress;
	CString strZipcode;
	
	GetDlgItemText(IDC_edtUSER10, strAddress);
	GetDlgItemText(IDC_edtUSER9,  strZipcode);
	
	if( TRUE != strAddress.IsEmpty() && TRUE == strZipcode.IsEmpty() )
	{		
		strMessage = _T("자택 우편번호가 입력되지 않았습니다.");
	}
	
	GetDlgItemText(IDC_edtUSER13, strAddress);
	GetDlgItemText(IDC_edtUSER12, strZipcode);

	if( TRUE != strAddress.IsEmpty() && TRUE == strZipcode.IsEmpty() )
	{
		if( TRUE != strMessage.IsEmpty() ) 
		{
			strMessage += _T("\r\n");
		}
		
		strMessage += _T("근무지 우편번호가 입력되지 않았습니다.") ;
	}
	
	
	if( TRUE != strMessage.IsEmpty() )
	{
		strMessage = _T("[ 경고 ] \r\n\r\n") + strMessage;
		AfxMessageBox(strMessage);
	}
	
	CString strUserID;

	GetDlgItemText(IDC_edtUSER4, strUserID);

	if( TRUE != strUserID.IsEmpty() )
	{
		CString strPassWord;
		CString strRePassWord;

		GetDlgItemText(IDC_edtUSER5, strPassWord);
		GetDlgItemText(IDC_edtUSER6, strRePassWord);

		
		if(0 == m_nMode)
		{
			if( TRUE == strPassWord.IsEmpty() )
			{
				AfxMessageBox(_T("비밀번호를 입력해주십시오."));
				return;
			}
			if( TRUE == strRePassWord.IsEmpty() )
			{
				AfxMessageBox(_T("비밀번호확인을 입력해주십시오."));
				return;
			}
			if( 0 != strPassWord.Compare(strRePassWord) )
			{
				AfxMessageBox(_T("비밀번호와 비밀번호확인의 입력이 같지 않습니다."));
				return;
			}
		}
		else
		{
			if(TRUE != m_pInfo->m_bVisitorPwEncryption)
			{
				if( TRUE == strPassWord.IsEmpty() )
				{
					AfxMessageBox(_T("비밀번호를 입력해주십시오."));
					return;
				}
				if( TRUE == strRePassWord.IsEmpty() )
				{
					AfxMessageBox(_T("비밀번호확인을 입력해주십시오."));
					return;
				}
				if( 0 != strPassWord.Compare(strRePassWord) )
				{
					AfxMessageBox(_T("비밀번호와 비밀번호확인의 입력이 같지 않습니다."));
					return;
				}
			}
		}		
	}	
		
	CString strCardPassWord;
	GetDlgItemText( IDC_edtUSER_35 , strCardPassWord );

	if( TRUE != strCardPassWord.IsEmpty() )
	{
		strCardPassWord.TrimLeft();
		strCardPassWord.TrimRight();

		INT		nLength		=	strCardPassWord.GetLength();
		
		if(0 == m_nMode)
		{
			if( 4 > nLength || 20 < nLength )
			{
				AfxMessageBox(_T("회원증 비밀번호는 4자리 이상, 20자리 이하로 입력하여 주십시요."), MB_ICONWARNING);			
				return;
			}			
		}
		else
		{
			if(TRUE != m_pInfo->m_bCardPwEncryption)
			{
				if( 4 > nLength || 20 < nLength )
				{
					AfxMessageBox(_T("회원증 비밀번호는 4자리 이상, 20자리 이하로 입력하여 주십시요."), MB_ICONWARNING);			
					return;
				}
			}
		}
	}
	
	CString strUserNoType;
	
	if( 0 == m_nMode && 2 == m_nUserNoAutoMode )
	{
		GetControlData( m_strCMAlias , _T("이용자번호구분") , strUserNoType );
		if( TRUE == strUserNoType.IsEmpty() )
		{
			AfxMessageBox(_T("이용자번호구분이 선택되지 않았습니다."));
			return;
		}
		
		if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
		{
			CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
			INT nIndex = pCombo->GetCurSel();
			CString strData;
			pCombo->GetLBText(nIndex,strData);
			if(_T("Y")==strData)
			{
				if(8!=strUserNoType.GetLength())
				{
					AfxMessageBox(_T("통합이용자 번호구분이 선택되지 않았습니다."));
					GetDlgItem(IDC_cmbUSER9)->SetFocus();
					return;
				}

				if(!(4==m_nLTMemberSaveMode || 5==m_nLTMemberSaveMode))
				{
					if(m_pInfo->LIB_CODE != strUserNoType.Left(6))
					{
						AfxMessageBox(_T("통합이용자 번호구분이 선택되지 않았습니다."));
						GetDlgItem(IDC_cmbUSER9)->SetFocus();
						return;
					}
				}
			}
		}
	}
		
	
	if( SetFamilyKey() )
	{
		AfxMessageBox(_T("존재하지 않는 가족ID입니다.\r\n")
					  _T("다시 입력하여 주십시요.\r\n")
					  _T("가족ID를 찾으시려면 \"가족ID찾기\" 버튼을 클릭하여 주십시요"));
		return;
	}
		
	if( CheckBirthday() )
	{
		AfxMessageBox(_T("정확한 생일을 입력하여 주십시요"));
		return;
	}	
	
	ids = IsInputControl();
	if(0>ids) ERROR_MSG_RETURN_VOID2( -1010 , _T("OnbtnSAVE"));

	if(0<ids) return;
		
	CString strUserClass, strMemberClass;
	GetControlData(m_strCMAlias, _T("회원상태"), strUserClass);
	GetControlData(m_strCMAlias, _T("회원구분"), strMemberClass);
	if(_T("탈퇴회원")==strUserClass && _T("비회원")==strMemberClass)
	{
		AfxMessageBox(_T("탈퇴회원을 비회원으로 저장 할 수 없습니다.\r\n탈퇴처리는 이용자관리 메뉴의 탈퇴처리 기능을 이용하여 주십시요."),MB_ICONINFORMATION);
		return;
	}
	
	if( 0 == m_bCivilNoDisplayCheck.Compare(_T("Y")) )
	{
		CString strCivilNum;
		GetDlgItemText(IDC_edtUSER8, strCivilNum);
		SetDlgItemText(IDC_edtUSER8, MakeCivilNumFormat(strCivilNum));
	}
	
	ids = CheckIsPhone();
	if(0>ids) return;
	
	ids = CheckIsEMail();
	if(0>ids) return;
	
	if( 1 == ((CComboBox*)GetDlgItem(IDC_cmbUSER5))->GetCurSel() &&
		1 == ((CButton*)GetDlgItem(IDC_cMONETA))->GetCheck() )
	{
		m_pSpfWork->SetTempData(_T("MONETA_SERVICE_YN"), _T("Y"));
	}
	else
	{
		m_pSpfWork->SetTempData(_T("MONETA_SERVICE_YN"), _T("N"));
	}
		
	if( 0 == ((CButton*)GetDlgItem(IDC_btnCHECKCENTER_UPLOAD))->GetCheck() )
	{
		m_pSpfWork->SetTempData(_T("도서관KEY") , _T(""));
		m_pSpfWork->SetTempData(_T("도서관부호"), _T(""));		
	}
	else
	{
		m_pSpfWork->SetTempData(_T("도서관KEY") , CComVariant(m_sLibKey));
		m_pSpfWork->SetTempData(_T("도서관부호"), CComVariant(m_sLibCode));		
	}
	
	m_pSpfWork->SetTempData(_T("사진경로"), CComVariant(m_sImageFilePath));

	ids = CheckDuplecate();
	if(0 > ids)
	{
		AfxMessageBox(_T("중복검사에 실패하였습니다."), MB_ICONSTOP);
		return;
	}
	
	CString strMode;
	strMode.Format( _T("%d") , m_nMode );
	ids = SetControlData( m_strCMAlias , _T("모드") , strMode );
	if(0>ids) ERROR_MSG_RETURN_VOID2( -1030 , _T("OnbtnSAVE") );	
	
	LastGrantUserNoType = ((CComboBox*)GetDlgItem(IDC_cmbUSER9))->GetCurSel();
	
	if( 0 == m_nMode && 0 == m_Secede_Mode)
	{
		ids = IsReEntry(m_bIsReEntry);
		if(0 != ids) return;
		if(m_bIsReEntry)
		{
			int nResult = AfxMessageBox(_T("탈퇴한 이용자 정보가 존재합니다.\r\n")
										_T("신규 이용자로 저장을 하시겠습니까?\r\n")
										_T(" - 예 : 신규 이용자로 저장합니다.\r\n")
										_T(" - 아니오 : 탈퇴 정보를 확인합니다.\r\n")
										_T(" - 취소 : 저장을 취소합니다."), MB_ICONSTOP | MB_YESNOCANCEL);
			if(IDYES == nResult)
			{
				m_Secede_Mode = 1;
			}
			else if(IDNO == nResult)
			{
				CString strCivilNo;
				GetDlgItemText(IDC_edtUSER8, strCivilNo);
				CSECEDE_USER_DLG Dlg(this->m_pSpfWork, this);
				Dlg.SetCivilNo(strCivilNo);
				Dlg.m_sIsUnityLoanService = m_pLoanMangeValue->m_sIsUnityLoanService;
				Dlg.DoModal();
				if(Dlg.m_bApply) m_Secede_Mode = 2;
				return;
			}
			else return;
		}
	}
	
	if( 1 == m_nMode || 3 == m_nMode )
	{
		if( m_sFeeMngSys == _T("Y") )
		{
			ids = GetDataMgrData( m_strDMAlias, _T("대출정지일"), sBeforeLoanStopDate, 0 );
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1040 , _T("OnbtnSAVE") );
			
			ids = GetControlData( m_strCMAlias, _T("대출정지일"), sAfterLoanStopDate );
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1050 , _T("OnbtnSAVE") );

			if( sBeforeLoanStopDate != sAfterLoanStopDate )
			{
				bEdit = TRUE;
			}
		}

		if(TRUE == m_pParent->m_pInfo->m_bKLUse && (TRUE == m_pParent->m_pInfo->m_bCertifyUse || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0 ))
		{
			GetDataMgrData(m_strDMAlias, _T("실명인증코드"), strBeforeCertifyCode, 0);
			if(strBeforeCertifyCode.IsEmpty() && !m_pSpfWork->m_strCertifyCode.IsEmpty()) bCertifyEdit = TRUE;
		}

		CString strInferiorClass;
		ids = GetControlData(m_strCMAlias, _T("회원상태"), strInferiorClass, -1, 0);
		if(0>ids) ERROR_MSG_RETURN_VOID2( -1060, _T("OnbtnSAVE") );

		
		if( 0 == strInferiorClass.Compare(_T("대출정지")) )
		{
			CString strLoanStopDate;
			ids = GetControlData(m_strCMAlias, _T("대출정지일"), strLoanStopDate, -1, -1);
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1070 , _T("OnbtnSAVE") );

			if( TRUE == strLoanStopDate.IsEmpty() )
			{
				if( IDNO == AfxMessageBox(
					_T("회원 상태를 '대출정지'로 하시고 대출 정지일이 입력되지 않은 경우\r\n")
					_T("대출/반납 화면에서 현재 이용자를 검색시 연체된 책이 존재하지 않는다면\r\n")
					_T("회원 상태가 자동으로 '정상'으로 변경될 수 있습니다.\r\n")
					_T("저장하시겠습니까?"), MB_YESNO|MB_ICONQUESTION)) 
				{
					return;
				}
			}
		}
		
		else if( ( 0 == strInferiorClass.Compare(_T("탈퇴회원")) ) || 
			     ( 0 == strInferiorClass.Compare(_T("탈퇴")) ) )
		{
			
			if(TRUE == m_pInfo->m_bKLUse)
			{
				CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
				INT nIndex = pCombo->GetCurSel();
				CString strData;
				pCombo->GetLBText(nIndex,strData);
				if(_T("Y") == strData)
				{
					
					AfxMessageBox(_T("책이음회원은 탈퇴회원으로 저장 할 수 없습니다.\r\n책이음회원의 탈퇴처리를 원하시면 탈퇴처리 버튼을 이용하십시오."), MB_ICONSTOP);
					return;
				}
			}

			
			if( IDNO == AfxMessageBox( 
				_T("회원 상태를 '탈퇴회원'로 변경할 경우 탈퇴처리가 가능해지며\r\n")
				_T("탈퇴처리 후에는 이용자의 개인정보 및 대출자번호가 삭제됩니다.\r\n")
				_T("저장하시겠습니까?"), MB_YESNO | MB_ICONQUESTION ) )
			{
				return;
			}
			
			
			BOOL bSecede;
			ids = IsSecede( bSecede );
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1080 , _T("OnbtnSAVE"));

			if( TRUE != bSecede )
			{
				CString strMsg;
				strMsg.Format(
					_T("대출/예약이력을 가지고 있는 이용자는 탈퇴신청을 할 수 없습니다."));
				AfxMessageBox(strMsg);
				return;
			}
		}
		
		else if( ( 0 == strInferiorClass.Compare(_T("제적회원")) ) || 
			     ( 0 == strInferiorClass.Compare(_T("제적")) ) )
		{		
			ids = SetDataMgrData(m_strDMAlias, _T("회원구분"), _T("1"), 0);
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1090 , _T("OnbtnSAVE"));

			((CComboBox*)GetDlgItem( IDC_cmbUSER4 ))->SetCurSel(1);
		}

		
		CString sRemark;
		GetDlgItemText(IDC_edtUSER22, sRemark);
		if( m_sRemark != sRemark )
		{
			m_pSpfWork->m_bRemarkEditYN = TRUE;
		}
		
		GetDlgItemText(IDC_edtUSER1, m_pSpfWork->m_strAfterUserNo);
	}
	
	m_pSpfWork->m_nLTMemberSaveMode = m_nLTMemberSaveMode;

	if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SEX_M);
		ASSERT(pButton);
		if (pButton->GetCheck())
		{
			m_pSpfWork->m_strCertifySex = _T("0");
		}
		else
		{
			pButton = (CButton*)GetDlgItem(IDC_RADIO_SEX_F);
			ASSERT(pButton);
			if (pButton->GetCheck())
			{
				m_pSpfWork->m_strCertifySex = _T("1");
			}
		}
	}
 
	if( 0 == m_nMode )
	{				
		if(_T("Y") != strSelectKLMemberYN && m_pInfo->m_bAgreeInfoRecordYN)
		{			
			if(0 > MakeAndShowStatement(strSelectKLMemberYN))
			{
				return;
			}	
		}
	
		if(TRUE == m_pInfo->m_bCertifyUse || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
		{
			if((TRUE == m_pInfo->m_bCertifyUse || g_strMypinCertifyFlag.CompareNoCase(_T("Y"))== 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y"))  == 0) && TRUE == m_pInfo->m_bKLUse) if(TRUE != IsPrivacyState()) return;
			
			if(FALSE == m_pInfo->m_bKLUse && m_pSpfWork->m_strCertifyCode.IsEmpty() && m_pSpfWork->m_strCertifyFailCode.IsEmpty())
			{				
				if(IDYES == MessageBox( _T("본인인증 대상 이용자 입니다.\r\n")
										_T("본인인증을 수행하고 저장하시겠습니까?\r\n")
										_T(" - 예 : 본인인증을 수행하고 저장합니다\r\n")
										_T(" - 아니오 : 본인인증을 수행하지 않고 저장합니다"), _T("본인인증확인"), MB_YESNO|MB_ICONQUESTION))
				{
					StartUserCertify();
				}
			}
		}

		ids = m_pSpfWork->SPFExcutePath(_T("이용자관리_입력"));
		if(0>ids) return;
		
		CESL_DataMgr *pDM = FindDM(_T("DM_BO_LOC_3300"));
		CString strIsKLMemberYN = _T("N");
		if(1==m_nLTMemberSaveMode || 4==m_nLTMemberSaveMode)
		{
			ids = KLMemberInsert();
			if(0!=ids) 
			{
				
				AfxMessageBox(_T("통합도서 장애로 일반회원으로 가입되었습니다.\r\n시스템 정상화시 책이음회원으로 전환하여 주십시오."), MB_ICONSTOP);
			}
			else
			{
				strIsKLMemberYN = _T("Y");
			}
		}

		pDM->FreeData();

		CESL_DataMgr *pListDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
		INT nRowCnt = pListDM->GetRowCount();
		
		pListDM->SetCellData(_T("통합회원전환여부"),strIsKLMemberYN,nRowCnt-1);
		
		if (m_pInfo->m_bKLUse && strIsKLMemberYN.CompareNoCase(_T("Y")) == 0)
		{
			CString strRecKey;
			pListDM->GetCellData(_T("REC_KEY"), nRowCnt-1, strRecKey);

			if (!strRecKey.IsEmpty())
			{
				CString strDate;
				strDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), m_strKLCreateDate);

				pListDM->StartFrame();
				pListDM->InitDBFieldData();
				pListDM->AddDBFieldData(_T("KL_MEMBER_YN"),	_T("STRING"), strIsKLMemberYN);
				if(!m_strGPIN_HASH.IsEmpty())	pListDM->AddDBFieldData(_T("GPIN_HASH"), _T("STRING"), m_strGPIN_HASH);
				if(!m_strGPIN_ASCII.IsEmpty())	pListDM->AddDBFieldData(_T("GPIN_ASCII"), _T("STRING"), m_strGPIN_ASCII);
				if(!m_strKLWORKNO.IsEmpty())	pListDM->AddDBFieldData(_T("WORKNO"), _T("STRING"), m_strKLWORKNO);				
				pListDM->AddDBFieldData(_T("REG_DATE"), _T("NUMERIC"), strDate);
				pListDM->AddDBFieldData(_T("KL_REG_DATE"), _T("NUMERIC"), strDate);
				pListDM->AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), m_strGPIN_SEX);

				if (!m_pInfo->m_bCertifyUse && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && !m_strCI.IsEmpty() && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0)
				{
					pListDM->AddDBFieldData(_T("IPIN_HASH"), _T("STRING"), m_strCI);
					pListDM->AddDBFieldData(_T("IPIN_DATE"), _T("NUMERIC"), _T("SYSDATE"));
					pListDM->AddDBFieldData(_T("IPIN_RESULT"), _T("NUMERIC"), _T("NULL"));
					pListDM->AddDBFieldData(_T("CERTIFY_WORKER"), _T("STRING"), m_pInfo->USER_ID);					
				}

				pListDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY") , _T("NUMERIC") , strRecKey);
				pListDM->SendFrame();
				pListDM->EndFrame();
			}
		}
		CString strQuery = _T(""), strValue = _T(""), strSetData = _T(""), strWhere = _T(""), strCryptoCivilNo = _T("");
		if(TRUE == m_pInfo->m_bKLUse) if(!m_pSpfWork->m_strInsertCivilNo.IsEmpty()) this->ESL_DES_Encryption(m_pSpfWork->m_strInsertCivilNo, strCryptoCivilNo);
		if(!strCryptoCivilNo.IsEmpty())
		{
			strValue.Format(_T("CRYPTO_CIVIL_NO='%s'"), strCryptoCivilNo);
			strWhere += strValue;
		}
		if(!m_strGPIN_HASH.IsEmpty())
		{
			if(!strWhere.IsEmpty()) strWhere += _T(" OR ");
			strValue.Format(_T("GPIN_HASH='%s'"), m_strGPIN_HASH);
			strWhere += strValue;
		}
		if(!m_pSpfWork->m_strCertifyCode.IsEmpty())
		{
			if(!strWhere.IsEmpty()) strWhere += _T(" OR ");
			strValue.Format(_T("IPIN_HASH='%s'"), m_pSpfWork->m_strCertifyCode);
			strWhere += strValue;
		}
		if(!strWhere.IsEmpty())
		{
			strSetData.Format(_T("CRYPTO_CIVIL_NO=NULL,GPIN_ASCII=NULL,GPIN_HASH=NULL,GPIN_SEX=NULL,IPIN_HASH=NULL,IPIN_DATE=NULL,IPIN_RESULT=NULL,CERTIFY_WORKER=NULL"));
			strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET %s WHERE USER_CLASS='3' AND USER_NO IS NULL AND (%s);"), strSetData, strWhere);
			pDM->StartFrame();
			pDM->AddFrame(strQuery);
			pDM->SendFrame();
			pDM->EndFrame();
		}				
		
		m_bIsInsertBefore = TRUE;			
		m_pSpfWork->GetTempData(_T("대출자번호"), m_strLastWorkUserNo);		
		m_pSpfWork->GetTempData(_T("SHARE_CO_LOAN_USER_TBL_PROC_REC_KEY"), m_strInsertRecKey);		
		m_pSpfWork->GetTempData(_T("이름"), m_strLastWorkUserName);
		CString sMsg;
		sMsg.Format(_T("저장이 완료되었습니다.\r\n\r\n이름 : %s\r\n대출자번호 : %s"), m_strLastWorkUserName, m_strLastWorkUserNo);
		AfxMessageBox(sMsg, MB_ICONINFORMATION);

		CString strUserKey = pListDM->GetCellData(_T("REC_KEY"), nRowCnt-1);	
		if(TRUE == m_pParent->m_pInfo->m_bKLUse)
		{
			
			if(_T("Y") == strSelectKLMemberYN && _T("책이음서비스") != m_strBeforeAgreeStatement && m_bIsAgreeYN)
			{
				INT ids = InsertAgreementInfo(strUserKey, _T("책이음서비스"));
				if(0 > ids)
				{
  					AfxMessageBox(_T("동의 기록 실패"), MB_ICONINFORMATION);
				}			
			}
		}
		if(TRUE == m_pInfo->m_bAgreeInfoRecordYN && _T("Y") != strSelectKLMemberYN && TRUE == m_bIsAgreeYN)
		{
			INT ids = InsertAgreementInfo(strUserKey, _T("개인정보 수집/이용"));
			if(0 > ids)
			{
  				AfxMessageBox(_T("동의 기록 실패"), MB_ICONINFORMATION);
			}					
		}
		
		else if(_T("") != m_strAgentName)
		{	
			CString strQuery, strGetDuplication;
			CString strWork = m_pParent->GetWorkLogMsg(_T("이용약관 동의"),__WFILE__,__LINE__);
			pDM->StartFrame();
			strQuery.Format(_T("SELECT COUNT(1) FROM CO_AGENT_INFO_TBL WHERE USER_KEY = %s"), strUserKey);
			pDM->GetOneValueQuery(strQuery, strGetDuplication);
			if(_T("0") != strGetDuplication)
			{	
				strQuery.Format(_T("DELETE CO_AGENT_INFO_TBL WHERE USER_KEY = %s;"), strUserKey);
				pDM->AddFrame(strQuery);
			}
			strQuery.Format(_T("INSERT INTO CO_AGENT_INFO_TBL(USER_KEY, NAME, RELATION, NOTE, CERT_DATE, LIB_CODE, LIB_NAME, WORKER, FIRST_WORK) VALUES(%s, '%s', '%s', '%s', SYSDATE, '%s', '%s', 'KOLASIII', '%s');"), strUserKey, m_strAgentName, m_strAgentRelation, m_strAgentNote, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork);	
			pDM->AddFrame(strQuery);
			pDM->SendFrame();
			pDM->EndFrame();
		}		
		UpdateUserScanKey(strUserKey);

		OnbtnCLEAR();
	}	
	
	else if( 1 == m_nMode || 3 == m_nMode )
	{		
		if( (TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0) && TRUE == m_pInfo->m_bKLUse)
		{
			if(TRUE != IsPrivacyState()) return;
		}		

		if( TRUE == bEdit )
		{
			CESL_DataMgr *pDM = FindDM( m_strDMAlias );

			ids = GetDataMgrData( m_strDMAlias, _T("사용자ID"), sUserID, 0 );
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1100 , _T("OnbtnSAVE"));

			ids = GetDataMgrData( m_strDMAlias, _T("대출자번호"), sUserNO, 0 );
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1110 , _T("OnbtnSAVE"));

			ids = GetDataMgrData( m_strDMAlias, _T("이름"), sUserName, 0 );
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1120 , _T("OnbtnSAVE"));

			sQuery.Format(	_T("INSERT INTO CO_LOANSTOPDATE_LOG_TBL")
							_T("(REC_KEY, EDIT_DATE, USER_NO, USER_ID, USER_NAME, BEFORE, AFTER, FIRST_WORK) ")
							_T("VALUES(ESL_SEQ.NEXTVAL, SYSDATE, '%s', '%s', '%s', '%s', '%s', '%s');"),
							sUserNO, sUserID, sUserName, sBeforeLoanStopDate, sAfterLoanStopDate, 
							GetWorkLogMsg( _T("대출정지일수정"), __FILE__, __LINE__ ) ) ;

			pDM->StartFrame();
			pDM->InitDBFieldData();
			pDM->AddFrame( sQuery );
			pDM->SendFrame(TRUE);	
			pDM->EndFrame();
		}
		const nCheckCnt = 7;
		CString strSaveBeforeData[nCheckCnt], strSaveAfterData[nCheckCnt];
		CString strDMAlias[nCheckCnt] = 
		{
			_T("대출자번호"), _T("불량회원구분"), _T("회원구분"), _T("대출정지일"), _T("대출정지부여일"), _T("휴대폰"), _T("E_MAIL")
		};

		if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
		{
			for(INT i=0; i<nCheckCnt; i++)
			{
				ids = GetDataMgrData(m_strDMAlias, strDMAlias[i], strSaveBeforeData[i], 0);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1120 , _T("OnbtnSAVE"));
			}
		}
	
		GetLocUserData(m_stDesData);
		CString strLogData = _T("");
		CompareLocUser(m_stOriData, m_stDesData, strLogData);


		CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300"));
		pDM->SetCellData(_T("WORKER_DO"), strLogData, 0);
		
		ids = m_pSpfWork->SPFExcutePath( _T("이용자관리_수정") );
		if(0>ids) return;

		pDM->SetCellData(_T("WORKER_DO"), _T(""), 0);
				
		if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
		{
			for(INT i=0; i<nCheckCnt; i++)
			{
				ids = GetDataMgrData(m_strDMAlias, strDMAlias[i], strSaveAfterData[i], 0);
				if(0>ids) ERROR_MSG_RETURN_VOID2( -1120 , _T("OnbtnSAVE"));
			}
		}
		
		CString strIsKLMemberYN = _T("N");
		if(2==m_nLTMemberSaveMode || 5==m_nLTMemberSaveMode)
		{
			ids = KLMemberChange();

			if(0 != ids) 
			{				
				AfxMessageBox(_T("통합도서 장애로 책이음회원전환에 실패하였습니다.\r\n이후에 책이음회원으로 전환하여 주십시오."), MB_ICONSTOP);
				return;
			}
			strIsKLMemberYN = _T("Y");
			
			if(1==m_nMode)
			{
				SetDataMgrData( _T("DM_BO_LOC_3300_LIST"), _T("통합회원전환여부"), strIsKLMemberYN, m_nCurrentIndex);
			}
			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
			pCombo->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_btnKLRealNameCert))->ShowWindow(SW_HIDE);
			((CButton*)GetDlgItem(IDC_btnSearchKLLib))->ShowWindow(SW_SHOW);
			((CButton*)GetDlgItem(IDC_btnSearchKLCurrentLoan))->ShowWindow(SW_SHOW);
			
			((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("책이음회원증\r\n발급이력"));
			GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_SHOW);
		}
		else if(3==m_nLTMemberSaveMode)
		{
			strIsKLMemberYN = _T("Y");			
			CString strBeforeUserNo = _T("");
 			if(strSaveBeforeData[0] != strSaveAfterData[0])
			{
				strBeforeUserNo = strSaveBeforeData[0];
			}
			
			if(_T("정상회원")==strSaveAfterData[1]) strSaveAfterData[1] = _T("0");
			else if(_T("대출정지")==strSaveAfterData[1]) strSaveAfterData[1] = _T("1");
			else if(_T("제적회원")==strSaveAfterData[1]) strSaveAfterData[1] = _T("2");
			else if(_T("탈퇴회원")==strSaveAfterData[1]) strSaveAfterData[1] = _T("3");
			else strSaveAfterData[1] = _T("0");

			if(_T("정회원")==strSaveAfterData[2]) strSaveAfterData[2] = _T("0");
			else if(_T("비회원")==strSaveAfterData[2]) strSaveAfterData[2] = _T("1");
			else if(_T("준회원")==strSaveAfterData[2]) strSaveAfterData[2] = _T("2");
			else strSaveAfterData[2] = _T("0");

			BOOL bChange = FALSE;
			for(INT i=1; i<nCheckCnt; i++)
			{
				if(strSaveBeforeData[i] != strSaveAfterData[i])
				{
					bChange = TRUE;
					break;
				}
			}

			ids = KLMemberUpdate(strBeforeUserNo,bChange);
		}


		if( (TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0) && TRUE == m_pInfo->m_bKLUse)
		
		{ 
			if(_T("Y") == strIsKLMemberYN)
			{
				CESL_DataMgr *pDM = FindDM( m_strDMAlias );
				CString strRecKey;
				pDM->GetCellData(_T("REC_KEY"), 0, strRecKey);
				pDM->SetCellData(_T("통합회원전환여부"), strIsKLMemberYN, 0);
				if(!strRecKey.IsEmpty())
				{
					CString strQuery = _T("");
					CString strValue = _T("");
					strQuery.Format(_T("SELECT KL_MEMBER_YN FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strRecKey);
					ids = pDM->GetOneValueQuery(strQuery, strValue);
					if(0 > ids) ERROR_MSG_RETURN_VOID2(-1100, _T("OnbtnSAVE"));
					if(_T("Y") != strValue)
					{
						pDM->StartFrame();
						pDM->InitDBFieldData();
						pDM->AddDBFieldData(_T("KL_MEMBER_YN"),	_T("STRING"), strIsKLMemberYN);
						if(!m_strGPIN_HASH.IsEmpty()) pDM->AddDBFieldData(_T("GPIN_HASH"), _T("STRING"), m_strGPIN_HASH);
						if(!m_strGPIN_ASCII.IsEmpty()) pDM->AddDBFieldData(_T("GPIN_ASCII"), _T("STRING"), m_strGPIN_ASCII);
						if(!m_strKLWORKNO.IsEmpty()) pDM->AddDBFieldData(_T("WORKNO"), _T("STRING"), m_strKLWORKNO);
						CString strDate;
						strDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), m_strKLCreateDate);
						

						pDM->AddDBFieldData(_T("KL_REG_DATE"), _T("NUMERIC"), strDate);
						pDM->AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), m_strGPIN_SEX);
						pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY") , _T("NUMERIC") , strRecKey);
						pDM->SendFrame();
						pDM->EndFrame();
					}
				}
			}
		}

		else if( (FALSE == m_pInfo->m_bCertifyUse && g_strIpinFlag.CompareNoCase(_T("Y")) != 0 && g_strMypinCertifyFlag.CompareNoCase(_T("Y")) != 0 && g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) != 0) && TRUE == m_pInfo->m_bKLUse)
		{ 
			if(_T("Y") == strIsKLMemberYN)
			{
				CESL_DataMgr *pDM = FindDM( m_strDMAlias );
				CString strRecKey;
				pDM->GetCellData(_T("REC_KEY"), 0, strRecKey);
				pDM->SetCellData(_T("통합회원전환여부"), strIsKLMemberYN, 0);
				if(!strRecKey.IsEmpty())
				{
					CString strQuery = _T("");
					const int nSelectCnt = 2;
					CString strGetValues[nSelectCnt] = { _T(""), _T("") };
					strQuery.Format(_T("SELECT KL_MEMBER_YN, IPIN_HASH FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strRecKey);
					ids = pDM->GetValuesQuery(strQuery, strGetValues, nSelectCnt);
					if(0 > ids) ERROR_MSG_RETURN_VOID2(-1100, _T("OnbtnSAVE"));
					if(_T("Y") != strGetValues[0] || (strGetValues[0].IsEmpty() && !m_strCI.IsEmpty()))
					{
						pDM->StartFrame();
						pDM->InitDBFieldData();
						if(_T("Y") != strGetValues[0])
						{
							pDM->AddDBFieldData(_T("KL_MEMBER_YN"),	_T("STRING"), strIsKLMemberYN);
							if(!m_strGPIN_HASH.IsEmpty()) pDM->AddDBFieldData(_T("GPIN_HASH"), _T("STRING"), m_strGPIN_HASH);
							if(!m_strGPIN_ASCII.IsEmpty()) pDM->AddDBFieldData(_T("GPIN_ASCII"), _T("STRING"), m_strGPIN_ASCII);
							if(!m_strKLWORKNO.IsEmpty()) pDM->AddDBFieldData(_T("WORKNO"), _T("STRING"), m_strKLWORKNO);
							CString strDate;
							strDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), m_strKLCreateDate);
							

							pDM->AddDBFieldData(_T("KL_REG_DATE"), _T("NUMERIC"), strDate);
							pDM->AddDBFieldData(_T("GPIN_SEX"), _T("STRING"), m_strGPIN_SEX);						
						}
						if(strGetValues[0].IsEmpty() && !m_strCI.IsEmpty())
						{
							pDM->AddDBFieldData(_T("IPIN_HASH"), _T("STRING"), m_strCI);
							pDM->AddDBFieldData(_T("IPIN_DATE"), _T("NUMERIC"), _T("SYSDATE"));
							pDM->AddDBFieldData(_T("IPIN_RESULT"), _T("NUMERIC"), _T("NULL"));
							pDM->AddDBFieldData(_T("CERTIFY_WORKER"), _T("STRING"), m_pInfo->USER_ID);
						}
						pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY") , _T("NUMERIC") , strRecKey);
						pDM->SendFrame();
						pDM->EndFrame();
					}
				}
			}
		}
		
		m_pSpfWork->GetTempData( _T("대출자번호") , m_strLastWorkUserNo );
		
		if(TRUE == m_pParent->m_pInfo->m_bKLUse)
		{
			CESL_DataMgr *pDM = FindDM( m_strDMAlias );
			CString strUserKey = pDM->GetCellData(_T("REC_KEY"), 0);		
			CString strAgreementAlias = pDM->GetCellData(_T("동의약관"), 0);
			if(_T("Y") == strSelectKLMemberYN && _T("책이음서비스") == strAgreementAlias)
			{
				
				if ( !m_strAgreeDate.IsEmpty() && m_bIsAgreeYN){
					INT ids = InsertAgreementInfo(strUserKey, _T("책이음서비스"));
					if(0 > ids)
					{
						AfxMessageBox(_T("동의 기록 실패"), MB_ICONINFORMATION);
					}	
				}
			}
		}

		
		if(_T("Y") == strIsKLMemberYN && TRUE == bCertifyEdit)
		{
			CFrameWriter Writer(m_pParent->m_pInfo);
			Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("통합대출자번호"), m_strLastWorkUserNo);
			CString strSysdate;
			CTime tCurTime = CLocCommonAPI::GetCurrentDateTime(m_pParent, strSysdate);
			Writer.addElement(_T("수정일자"), strSysdate);
			Writer.addElement(_T("수정도서관부호"), m_pParent->m_pInfo->LIB_CODE);
			Writer.addElement(_T("G-PINHASH인증코드"), m_strGPIN_HASH);			
			Writer.addElement(_T("연계정보"), m_pSpfWork->m_strCertifyCode+_T("▲")+m_pSpfWork->m_strCertifyDate+_T("▲▲")+m_pSpfWork->m_strCertifyWorker);			
			Writer.addElement(_T("대출자이름"), m_strLastWorkUserName);			
			Writer.addElement("검색용CI", m_pSpfWork->m_strCertifyCode);

			CStringArray astrFrames;
			astrFrames.Add(Writer.getFrame());
			CFrameSender Sender(m_pParent->m_pInfo);
			Sender.SendTempTbl(astrFrames, m_pParent->GetWorkLogMsg( _T("개인정보보호실명인증"), __WFILE__, __LINE__ ));
		}
	
		this->ControlDisplay(_T("CM_BO_LOC_3320"), _T("수정일자"), 0);
	
		this->ControlDisplay(_T("CM_BO_LOC_3320"), _T("대출자번호"), 0);

		if (strIsKLMemberYN.CompareNoCase(_T("Y")) == 0)		
		{
			m_bIsKLMemberYN = TRUE;
		}

		m_pSpfWork->GetTempData(_T("이름"), m_strLastWorkUserName);

		CString sMsg;
		sMsg.Format(_T("저장이 완료되었습니다.\r\n\r\n이름 : %s\r\n대출자번호 : %s"), m_strLastWorkUserName, m_strLastWorkUserNo);
		AfxMessageBox(sMsg, MB_ICONINFORMATION);
		
	}


EFS_END
}

VOID CBO_LOC_3320::OnbtnDUPLICATE3() 
{
EFS_BEGIN

	INT ids;

	ids = m_pSpfWork->SPFExcutePath( _T("가족ID찾기") );

EFS_END
}

VOID CBO_LOC_3320::OnbtnDUPLICATE1() 
{
EFS_BEGIN

	INT ids;
	bool IsDuplicate;
	ids = IsDuplicateUserNo( IsDuplicate );	
	if( 0 > ids )
	{
		return ;
	}

	if( 0 < ids ) 
	{
		return;
	}

	if( IsDuplicate )
	{
		AfxMessageBox( _T("중복된 대출자 번호입니다. 다른 번호를 입력하십시오"), MB_ICONSTOP );
	}
	else
	{
		AfxMessageBox( _T("중복된 대출자 번호가 존재하지 않습니다. ") );
	}

EFS_END
}

INT CBO_LOC_3320::IsDuplicateUserNo( bool &IsDuplicate , CStringArray* paUserKey)
{
EFS_BEGIN

	INT ids;
	CString sSql;
	IsDuplicate = FALSE;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	
	CString sResult, sBeforeUserNo, sAfterUserNo;
	CESL_DataMgr *pDM = FindDM( m_strDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010, _T("IsDuplicateUserNo") );
	}

	ids = pDM->GetCellData( _T("대출자번호") , 0 , sBeforeUserNo );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("IsDuplicateUserNo") );
	}

	ids = GetControlData( m_strCMAlias, _T("대출자번호"), sAfterUserNo );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("IsDuplicateUserNo") );
	}

	if( TRUE == sAfterUserNo.IsEmpty() )
	{
		AfxMessageBox( _T("대출자번호를 입력하시고 중복조회 기능을 사용하십시오") );
		IsDuplicate = FALSE;
		return 1;
	}
	
	if( 0 == m_nMode )
	{ 
		if( 1 == m_nUserNoAutoMode )
		{ 
			sAfterUserNo = MakeCivilNum();
			sAfterUserNo.Replace( _T(" "), _T("") );
			if( TRUE == sAfterUserNo.IsEmpty() && _T("Y") == m_bCivilNoDisplayCheck )
			{
				AfxMessageBox( _T("주민등록번호를 입력하여 주십시오") );
				IsDuplicate = FALSE;
				return 2;
			}
		}
		else if( 2 == m_nUserNoAutoMode )
		{ 
			CString sValue;
			ids = GetControlData( m_strCMAlias, _T("이용자번호구분") , sValue );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040, _T("IsDuplicateUserNo") );
			}

			if ( TRUE == sValue.IsEmpty() )
			{
				AfxMessageBox( _T("번호구분을 선택하여 주십시오") );
				IsDuplicate = FALSE;
				return 3;
			}

			ids = m_pSpfWork->GetAutoUserNo(sAfterUserNo, sValue, _T(""), FALSE);
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("IsDuplicateUserNo") );
			}
		}
		else if( 3 == m_nUserNoAutoMode )
		{ 
			CEdit *pEdt = (CEdit*)GetDlgItem(IDC_edtUSER4);
			pEdt->GetWindowText( sAfterUserNo );
			if( TRUE == sAfterUserNo.IsEmpty() && _T("Y") == m_bCivilNoDisplayCheck )
			{
				AfxMessageBox( _T("사용자ID를 입력하여 주십시오") );
				IsDuplicate = FALSE;
				return 4;
			}
		}
		else
		{ 
			
		}

 		sSql.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE USER_NO='%s'") , sAfterUserNo );
	}
	else
	{ 
		if( sBeforeUserNo == sAfterUserNo )
		{
			
			CString sRecKey;
			ids = pDM->GetCellData( _T("REC_KEY") , 0 , sRecKey );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("IsDuplicateUserNo") );
			}

			sSql.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE USER_NO='%s' AND REC_KEY NOT IN(%s)"), sAfterUserNo, sRecKey );
		}
		else
		{
			sSql.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE USER_NO='%s'"), sAfterUserNo );
		}
	}

	ids = TmpDM.RestructDataManager(sSql);
  	if( 0 > ids ) 
  	{
  		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("IsDuplicateUserNo") );
  	}

	if( 0 < TmpDM.GetRowCount() ) 
	{
		IsDuplicate = TRUE;

		if(NULL != paUserKey)
		{
			CString strUserKey;
			const int ROW_COUNT = TmpDM.GetRowCount();
			int nRowCnt = 0;
			for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
			{
				strUserKey = TmpDM.GetCellData(nRowCnt, 0);

				if(FALSE == strUserKey.IsEmpty())
				{
					paUserKey->Add(strUserKey);
				}
			}
		}

	}

	return 0;

EFS_END
IsDuplicate = FALSE;
return -1;
}
VOID CBO_LOC_3320::OnbtnDUPLICATE2() 
{
EFS_BEGIN

	INT ids;
	bool IsDuplicate = false;
	CStringArray aUserKey;
	ids = IsDuplicateUserID( IsDuplicate, &aUserKey );	

	if( 0 > ids )
	{
		return ;
	}

	if( 0 < ids )
	{
		return;
	}

	if( IsDuplicate )
	{
		if( 0 == m_nMode )
		{
			if( IDYES == AfxMessageBox( _T("중복된 대출자 ID입니다. \n중복된 이용자를 확인하시겠습니까?"), MB_ICONSTOP | MB_YESNO) )
			{				
				m_pSpfWork->SearchAddLoanUser(aUserKey);
				CBO_LOC_3320 Dlg( m_pParent, m_pLoanShareManager, m_pLoanMangeValue );
				Dlg.SetMode(1,TRUE);
				if( 1 > Dlg.CheckParentDMCount() )
				{
					return ;
				}
				
				Dlg.DoModal();
			}
		}
		else
		{
			AfxMessageBox( _T("중복된 대출자 ID입니다. 다른 ID를 사용하십시오."), MB_ICONSTOP );
		}
	}
	else
	{
		AfxMessageBox( _T("중복된 대출자 ID가 존재하지 않습니다. ") );
	}	

EFS_END
}


INT CBO_LOC_3320::IsDuplicateUserID( bool &IsDuplicate, CStringArray* paUserKey)
{
EFS_BEGIN

	INT ids;
	CString sSql;

	CString sGetData;
	
	if(_T("Y") == m_pLoanMangeValue->m_strIsAutoSetUsernoToUserid && 
		3 != m_nUserNoAutoMode && 0 == m_nMode)
	{
		OnbtnDUPLICATE1();
		return 0;

	}
	else
	{
		ids = GetControlData(m_strCMAlias, _T("사용자ID") , sGetData );
		if( 0 > ids )
		{
			return -1;	
		}
	}

 	CESL_DataMgr *pDM = FindDM( m_strDMAlias );
 	if( NULL == pDM ) 
 	{
 		return -1;
 	}
	if( TRUE == sGetData.IsEmpty() )
	{

		IsDuplicate = FALSE;
		return 2;
	}
	
	sSql.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE USER_ID = '%s'") , sGetData );	
	if( 0 != m_nMode )
	{
		CString sRecKey;

		ids = pDM->GetCellData( _T("REC_KEY") , 0 , sRecKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("IsDuplicateUserNo") );
		}


		
		if(!sRecKey.IsEmpty())
		{
			sSql += _T(" AND REC_KEY NOT IN (")+ sRecKey +_T(")");
		}

	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	ids = TmpDM.RestructDataManager(sSql);
  	if( 0 > ids ) 
  	{
  		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::IsDuplicateCivilNo()") );
  	}

	if( 0 < TmpDM.GetRowCount() ) 
	{
		IsDuplicate = TRUE;

		if(NULL != paUserKey)
		{
			CString strUserKey;
			const int ROW_COUNT = TmpDM.GetRowCount();
			int nRowCnt = 0;
			for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
			{
				strUserKey = TmpDM.GetCellData(nRowCnt, 0);

				if(FALSE == strUserKey.IsEmpty())
				{
					paUserKey->Add(strUserKey);
				}
			}
		} 

	} 
 	else  
 	{
 		IsDuplicate = FALSE;
 	}

	return 0;	

EFS_END
return -1;
}


INT CBO_LOC_3320::DisplayWindow()
{
EFS_BEGIN

	INT ids;

	m_sImageFilePath = _T("");

	
	ids = SearchAllUserInfo();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("DisplayWindow") );
	}

	
	ids = ViewScreenLinkDM();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("DisplayWindow") );
	}
	
	
	ids = SetSelect();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("DisplayWindow") );
	}

	
	if( _T("Y") == m_bCivilNoDisplayCheck )
	{
		
		SetCivilNum(FALSE);
	}

	
	OnSelchangecmbKLMemberYN();

	InitUserAccountControl();
	m_bKLMemeberSearch = FALSE;
	m_strKlUserNo = _T("");

	
	m_Secede_Mode = 0;

	
	
	m_pSpfWork->SetTempData(_T("KL_IMPORT_YN"), CComVariant(_T(" ")));
	

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::SetCurrentIndex()
{
EFS_BEGIN

	if( ( 3 == m_nMode ) || ( 6 == m_nMode ) )
	{
		m_nCurrentIndex = 0;
		m_nFirstIndex = 0;
		m_nLastIndex = 0;
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( NULL == pGrid ) 
		{
			return -1;
		}
		
		INT nTmpIndex;
		if( bFirstView ) 
		{
			nTmpIndex = pGrid->GetIdx();
		}
		else 
		{
			nTmpIndex = pGrid->GetIdx();
		}

		pGrid->SelectMakeList();
		INT nSelectCount = pGrid->SelectGetCount();
		if( 0 < nSelectCount )
		{
			m_nViewMode = 1;
		}
		else 
		{
			m_nViewMode = 0;
		}

		if( 0 == m_nViewMode )
		{
			m_nCurrentIndex = pGrid->GetIdx();
			m_nLastIndex = pGrid->GetCount() - 1;
			m_nFirstIndex = 0;
		}
		else
		{
			m_nLastIndex = pGrid->SelectGetTailPosition();
			m_nFirstIndex = pGrid->SelectGetHeadPosition();
			INT		i;
			for( i=0 ; i<nSelectCount-1 ; i++ )
			{
				if( nTmpIndex == pGrid->SelectGetIdx() )
				{
					break;
				}

				pGrid->SelectGetNext();
			}
			if( i == nSelectCount ) 
			{
				m_nCurrentIndex = pGrid->SelectGetHeadPosition();
			}
			else 
			{
				m_nCurrentIndex = pGrid->SelectGetIdx();
			}
		}
	}
	
	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3320::SetPreNextButton()
{
EFS_BEGIN

	GetDlgItem(IDC_btnPREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnNEXT)->EnableWindow(TRUE);

	if( m_nCurrentIndex == m_nFirstIndex )
	{
		GetDlgItem(IDC_btnPREV)->EnableWindow(FALSE);
	}
	if( m_nCurrentIndex == m_nLastIndex )
	{
		GetDlgItem(IDC_btnNEXT)->EnableWindow(FALSE);
	}

EFS_END
}

VOID CBO_LOC_3320::OnbtnPREV() 
{
EFS_BEGIN
	MoveIndex(0);	
	DisplayWindow();

	CString sParentDMAlias = _T("DM_BO_LOC_3300_LIST");
	if(4 == m_nMode) sParentDMAlias = _T("DM_BO_LOC_3200");
	if(5 == m_nMode) sParentDMAlias = _T("DM_BO_LOC_3240_BO");
	if(7 == m_nMode) sParentDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = FindDM(sParentDMAlias);

	if(TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
	{
		m_pSpfWork->m_strCertifyCode = _T("");
		m_pSpfWork->m_strCertifyFailCode = _T("");
		m_pSpfWork->m_strCertifyDate = _T("");
		m_pSpfWork->m_strCertifySex = _T("");
		m_pSpfWork->m_strCertifyWorker = _T("");
		m_pSpfWork->m_strCertifyName = _T("");
		m_pSpfWork->m_strCertifyCivilNo = _T("");
		
		if(NULL != pDM) 
		{
			CString strGetData;
			pDM->GetCellData(_T("실명인증코드"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifyCode = strGetData;
			pDM->GetCellData(_T("실명인증일자"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifyDate = strGetData;
			pDM->GetCellData(_T("GPIN_SEX"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifySex = strGetData;
			pDM->GetCellData(_T("실명인증작업자"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifyWorker = strGetData;
			pDM->GetCellData(_T("이름"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifyName = strGetData;
			
			if(TRUE == m_pInfo->m_bKLUse)
			{
				pDM->GetCellData(_T("주민등록번호_CRYPTO"), m_nCurrentIndex, strGetData);
				this->ESL_DES_Decryption(strGetData, m_pSpfWork->m_strCertifyCivilNo);
			}
			else
			{
				pDM->GetCellData(_T("주민등록번호"), m_nCurrentIndex, strGetData);
				m_pSpfWork->m_strCertifyCivilNo = strGetData;
			}

			if (!m_pSpfWork->m_strCertifyCode.IsEmpty())
			{ 
				m_btnCertify_Y.ShowWindow(TRUE);
				m_btnCertify_N.ShowWindow(FALSE);
				m_btnCertify_Fail.ShowWindow(FALSE);				
				m_nCertifyState = 1;

				if (m_bIsKLMemberYN || m_pParent->m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0 )
				{					
					if (!m_pSpfWork->m_strCertifyName.IsEmpty())
					{
						GetDlgItem(IDC_edtUSER7)->EnableWindow(FALSE);
					}
					if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 )
					{
						GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(FALSE);
						GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(FALSE);
					}
					if (m_bCivilNoDisplayCheck.CompareNoCase(_T("N")) == 0 && !m_pSpfWork->m_strCertifySex.IsEmpty())
					{
						GetDlgItem(IDC_RADIO_SEX_M)->EnableWindow(FALSE);
						GetDlgItem(IDC_RADIO_SEX_F)->EnableWindow(FALSE);
					}
				}
								
				pDM->GetCellData(_T("실명인증일자"), m_nCurrentIndex, strGetData);
				m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_Y));

				CString strValue;
				if(!strGetData.IsEmpty()) strValue.Format(_T("본인인증한 이용자입니다.\r\n인증일자 : %s"), strGetData);
				else strValue = _T("본인인증한 이용자입니다.");
				
				m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Y), strValue);
			}
			else
			{
				pDM->GetCellData(_T("실명인증실패코드"), m_nCurrentIndex, strGetData);
				if(strGetData.IsEmpty())
				{ 
					m_btnCertify_N.ShowWindow(TRUE);
					m_btnCertify_Y.ShowWindow(FALSE);
					m_btnCertify_Fail.ShowWindow(FALSE);
					
					m_nCertifyState = 2;

					if (g_bEnableCertify)
					{
						if(1 == m_nMode || 3 == m_nMode)
						{ 
							m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_N));
							m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증을 하기 위해서는\r\n이 버튼을 클릭해 주세요."));
						}
						else
						{ 
							m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_N));
							m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증 대상 이용자입니다."));
						}
					}				
				}
				else
				{ 
					
					m_pSpfWork->m_strCertifyFailCode = strGetData;

					m_btnCertify_Fail.ShowWindow(TRUE);
					m_btnCertify_Y.ShowWindow(FALSE);
					m_btnCertify_N.ShowWindow(FALSE);

					m_nCertifyState = 3;

					if (g_bEnableCertify)
					{
						
						m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_Fail));
						CString strValue;
						strValue.Format(_T("%d"), m_pInfo->m_nCertifyAgency);
						CString strDesc = _T(""), strAction = _T("");
						m_pInfo->GetCertifyFailCodeToDesc(strValue, strGetData, strDesc, strAction);
						
						if(1 == m_nMode || 3 == m_nMode)
						{ 
							if(strDesc.IsEmpty()) strValue = _T("본인인증을 실패한 이용자입니다.");
							else strValue = _T("실패사유 : ") + strDesc + _T("\r\n본인인증을 하기 위해서는 이 버튼을 클릭해 주세요.");
						}
						else
						{
							if(strDesc.IsEmpty()) strValue = _T("본인인증을 실패한 이용자입니다.");
							else strValue = _T("실패사유 : ") + strDesc;
						}
						
						m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Fail), strValue);
					}
				}
				GetDlgItem(IDC_edtUSER7)->EnableWindow(TRUE);
				if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 )
				{
					GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(TRUE);
					GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(TRUE);
				}
				if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0)
				{
					GetDlgItem(IDC_RADIO_SEX_M)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_SEX_F)->EnableWindow(TRUE);
				}
			}
			
		}

		RedrawWindow();
	}
	else
	{
		if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0)
		{
			CString strGetData;
			pDM->GetCellData(_T("GPIN_SEX"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifySex = strGetData;
		}		
	}

	if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0)
	{
		CButton* pbtnM = (CButton*)GetDlgItem(IDC_RADIO_SEX_M);
		CButton* pbtnF = (CButton*)GetDlgItem(IDC_RADIO_SEX_F);
		CButton* pbtnDo = NULL;
		CButton* pbtnUn = NULL;
		if		(m_pSpfWork->m_strCertifySex.CompareNoCase(_T("0")) == 0)
		{
			pbtnDo = pbtnM;
			pbtnUn = pbtnF;
		}
		else if (m_pSpfWork->m_strCertifySex.CompareNoCase(_T("1")) == 0)
		{
			pbtnDo = pbtnF;
			pbtnUn = pbtnM;
		}

		if (pbtnDo)
		{
			pbtnDo->SetCheck(TRUE);
			pbtnUn->SetCheck(FALSE);
		}
		else
		{
			pbtnM->SetCheck(FALSE);
			pbtnF->SetCheck(FALSE);
		}
	}
	{
	CString strAction = _T("");
	if(m_nAction == ACTION_READ_F_USERINFO)
	{
		strAction.Format(_T("이용자 상세조회"));
	}
	else if (m_nAction == ACTION_READ_F_LOANINFO)
	{
		strAction.Format(_T("대출자 상세조회"));
	}
	else if (m_nAction == ACTION_READ_F_LOANUSERINFO)
	{
		strAction.Format(_T("대출중 이용자 조회"));
	}
	else if (m_nAction == ACTION_READ_F_LOANIFOMGR)
	{
		strAction.Format(_T("대출자료관리 이용자 상세조회"));
	}
	else if (m_nAction == ACTION_READ_F_DELIVERY)
	{
		strAction.Format(_T("배달자료관리 이용자 상세조회"));
	}
	else if (m_nAction == ACTION_MOD_F_USERINFO)
	{
		strAction.Format(_T("이용자 수정"));
	}
	
	CESL_DataMgr* m_pDM = FindDM(m_strDMAlias);
	if(NULL==m_pDM) return;
	CString strRecKey;
	m_pDM->GetCellData(_T("REC_KEY"), 0, strRecKey);
	if(strRecKey.IsEmpty()) return;
	CString strQuery;
	
	
	strQuery.Format(_T("INSERT INTO CO_LOAN_USER_READ_TBL")
		_T("(REC_KEY,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK,WORKER_DO,USER_TYPE) ")
		_T("VALUES(ESL_SEQ.NEXTVAL,'%s','%s','%s','KOLASIII',%s,SYSDATE,'%s','%s' , 'U');")
		, m_pInfo->LOCAL_IP
		, m_pInfo->USER_ID
		, m_pInfo->USER_NAME
		, strRecKey
		, GetWorkLogMsg(_T("이용자상세조회"),__WFILE__,__LINE__)
		, strAction);
	
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame(strQuery);
	m_pDM->SendFrame(TRUE);	
	m_pDM->EndFrame();		
	}
EFS_END
}


VOID CBO_LOC_3320::OnbtnNEXT() 
{
EFS_BEGIN
	MoveIndex(1);	
	DisplayWindow();

	CString sParentDMAlias = _T("DM_BO_LOC_3300_LIST");
	if(4 == m_nMode) sParentDMAlias = _T("DM_BO_LOC_3200");
	if(5 == m_nMode) sParentDMAlias = _T("DM_BO_LOC_3240_BO");
	if(7 == m_nMode) sParentDMAlias = _T("DM_BO_LOC_3400");
	CESL_DataMgr *pDM = FindDM(sParentDMAlias);	
	
	if(TRUE == m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
	{
		m_pSpfWork->m_strCertifyCode = _T("");
		m_pSpfWork->m_strCertifyFailCode = _T("");
		m_pSpfWork->m_strCertifyDate = _T("");
		m_pSpfWork->m_strCertifySex = _T("");
		m_pSpfWork->m_strCertifyWorker = _T("");
		m_pSpfWork->m_strCertifyName = _T("");
		m_pSpfWork->m_strCertifyCivilNo = _T("");
		
		if(NULL != pDM) 
		{
			CString strGetData;
			pDM->GetCellData(_T("실명인증코드"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifyCode = strGetData;
			pDM->GetCellData(_T("실명인증일자"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifyDate = strGetData;
			pDM->GetCellData(_T("GPIN_SEX"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifySex = strGetData;
			pDM->GetCellData(_T("실명인증작업자"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifyWorker = strGetData;
			pDM->GetCellData(_T("이름"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifyName = strGetData;
			
			if(TRUE == m_pInfo->m_bKLUse)
			{
				pDM->GetCellData(_T("주민등록번호_CRYPTO"), m_nCurrentIndex, strGetData);
				this->ESL_DES_Decryption(strGetData, m_pSpfWork->m_strCertifyCivilNo);
			}
			else
			{
				pDM->GetCellData(_T("주민등록번호"), m_nCurrentIndex, strGetData);
				m_pSpfWork->m_strCertifyCivilNo = strGetData;
			}

			if (!m_pSpfWork->m_strCertifyCode.IsEmpty())
			{ 
				m_btnCertify_Y.ShowWindow(TRUE);
				m_btnCertify_N.ShowWindow(FALSE);
				m_btnCertify_Fail.ShowWindow(FALSE);				
				m_nCertifyState = 1;

				if (m_bIsKLMemberYN || m_pParent->m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
				{
					
					
					if (!m_pSpfWork->m_strCertifyName.IsEmpty())
					{
						GetDlgItem(IDC_edtUSER7)->EnableWindow(FALSE);
					}
					if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 )
					{
						GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(FALSE);
						GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(FALSE);
					}
					if (m_bCivilNoDisplayCheck.CompareNoCase(_T("N")) == 0 && !m_pSpfWork->m_strCertifySex.IsEmpty())
					{
						GetDlgItem(IDC_RADIO_SEX_M)->EnableWindow(FALSE);
						GetDlgItem(IDC_RADIO_SEX_F)->EnableWindow(FALSE);
					}
				}

				
				pDM->GetCellData(_T("실명인증일자"), m_nCurrentIndex, strGetData);
				m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_Y));

				CString strValue;
				if(!strGetData.IsEmpty()) strValue.Format(_T("본인인증한 이용자입니다.\r\n인증일자 : %s"), strGetData);
				else strValue = _T("본인인증한 이용자입니다.");
				
				m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Y), strValue);
			}
			else
			{
				pDM->GetCellData(_T("실명인증실패코드"), m_nCurrentIndex, strGetData);
				if(strGetData.IsEmpty())
				{ 
					m_btnCertify_N.ShowWindow(TRUE);
					m_btnCertify_Y.ShowWindow(FALSE);
					m_btnCertify_Fail.ShowWindow(FALSE);

					
					m_nCertifyState = 2;
					
					if (g_bEnableCertify)
					{
						if(1 == m_nMode || 3 == m_nMode)
						{ 
							m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_N));
							m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증을 하기 위해서는\r\n이 버튼을 클릭해 주세요."));
						}
						else
						{ 
							m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_N));
							m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_N), _T("본인인증 대상 이용자입니다."));
						}
					}
				}
				else
				{ 
					
					m_pSpfWork->m_strCertifyFailCode = strGetData;
					m_btnCertify_Fail.ShowWindow(TRUE);
					m_btnCertify_Y.ShowWindow(FALSE);
					m_btnCertify_N.ShowWindow(FALSE);					
					m_nCertifyState = 3;

					if (g_bEnableCertify)
					{
						
						m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_Fail));
						CString strValue;
						strValue.Format(_T("%d"), m_pInfo->m_nCertifyAgency);
						CString strDesc = _T(""), strAction = _T("");
						m_pInfo->GetCertifyFailCodeToDesc(strValue, strGetData, strDesc, strAction);

						if(1 == m_nMode || 3 == m_nMode)
						{ 
							if(strDesc.IsEmpty()) strValue = _T("본인인증을 실패한 이용자입니다.");
							else strValue = _T("실패사유 : ") + strDesc + _T("\r\n본인인증을 하기 위해서는 이 버튼을 클릭해 주세요.");
						}
						else
						{
							if(strDesc.IsEmpty()) strValue = _T("본인인증을 실패한 이용자입니다.");
							else strValue = _T("실패사유 : ") + strDesc;
						}
						
						m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Fail), strValue);
					}
				}
				
				GetDlgItem(IDC_edtUSER7)->EnableWindow(TRUE);
				
				if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 )
				{
					GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(TRUE);
					GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(TRUE);
				}
				
				if (m_bCivilNoDisplayCheck.CompareNoCase(_T("N")) == 0 )
				{
					GetDlgItem(IDC_RADIO_SEX_M)->EnableWindow(TRUE);
					GetDlgItem(IDC_RADIO_SEX_F)->EnableWindow(TRUE);
				}
			}			
		}

		RedrawWindow();
	}
	else
	{
		if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0 && pDM != NULL)
		{
			CString strGetData;
			pDM->GetCellData(_T("GPIN_SEX"), m_nCurrentIndex, strGetData);
			m_pSpfWork->m_strCertifySex = strGetData;
		}
	}

	if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) != 0)
	{
		CButton* pbtnM = (CButton*)GetDlgItem(IDC_RADIO_SEX_M);
		CButton* pbtnF = (CButton*)GetDlgItem(IDC_RADIO_SEX_F);
		CButton* pbtnDo = NULL;
		CButton* pbtnUn = NULL;
		if		(m_pSpfWork->m_strCertifySex.CompareNoCase(_T("0")) == 0)
		{
			pbtnDo = pbtnM;
			pbtnUn = pbtnF;
		}
		else if (m_pSpfWork->m_strCertifySex.CompareNoCase(_T("1")) == 0)
		{
			pbtnDo = pbtnF;
			pbtnUn = pbtnM;
		}

		if (pbtnDo)
		{
			pbtnDo->SetCheck(TRUE);
			pbtnUn->SetCheck(FALSE);
		}
		else
		{
			pbtnM->SetCheck(FALSE);
			pbtnF->SetCheck(FALSE);
		}
	}
	{

	CString strAction = _T("");
	if(m_nAction == ACTION_READ_F_USERINFO)
	{
		strAction.Format(_T("이용자 상세조회"));
	}
	else if (m_nAction == ACTION_READ_F_LOANINFO)
	{
		strAction.Format(_T("대출자 상세조회"));
	}
	else if (m_nAction == ACTION_READ_F_LOANUSERINFO)
	{
		strAction.Format(_T("대출중 이용자 조회"));
	}
	else if (m_nAction == ACTION_READ_F_LOANIFOMGR)
	{
		strAction.Format(_T("대출자료관리 이용자 상세조회"));
	}
	else if (m_nAction == ACTION_READ_F_DELIVERY)
	{
		strAction.Format(_T("배달자료관리 이용자 상세조회"));
	}
	else if (m_nAction == ACTION_MOD_F_USERINFO)
	{
		strAction.Format(_T("이용자 수정"));
	}
	
	CESL_DataMgr* m_pDM = FindDM(m_strDMAlias);
	if(NULL==m_pDM) return;
	CString strRecKey;
	m_pDM->GetCellData(_T("REC_KEY"), 0, strRecKey);
	if(strRecKey.IsEmpty()) return;
	CString strQuery;
	
	
	strQuery.Format(_T("INSERT INTO CO_LOAN_USER_READ_TBL")
		_T("(REC_KEY,WORKER_IP,WORKER_ID,WORKER_NAME,WORKER_SYS,USER_KEY,WORK_DATE,FIRST_WORK,WORKER_DO,USER_TYPE) ")
		_T("VALUES(ESL_SEQ.NEXTVAL,'%s','%s','%s','KOLASIII',%s,SYSDATE,'%s','%s' , 'U' );")
		, m_pInfo->LOCAL_IP
		, m_pInfo->USER_ID
		, m_pInfo->USER_NAME
		, strRecKey
		, GetWorkLogMsg(_T("이용자상세조회"),__WFILE__,__LINE__)
		, strAction);
	
	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();
	m_pDM->AddFrame(strQuery);
	m_pDM->SendFrame(TRUE);	
	m_pDM->EndFrame();		
	}
EFS_END
}

INT CBO_LOC_3320::SearchAllUserInfo(CString strRecKey/*=_T("")*/)
{
EFS_BEGIN

	INT ids;
	CString sDMAlias = m_strDMAlias;

	CString sRecKey;
	
	if(!strRecKey.IsEmpty())
	{
		sRecKey = strRecKey;
	}
	else
	{
		ids = GetRecKey( sRecKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SearchAllUserInfo") );
		}
		
		if( TRUE == sRecKey.IsEmpty() ) 
		{
			return 100;	
		}
	}

	CString sWhere;
	sWhere.Format( _T(" CLU.REC_KEY = %s ") , sRecKey );
	
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SearchAllUserInfo") );
	}

	pDM->FreeData();

	ids = CLocCommonAPI::RefreshDataManager( this , pDM , sWhere );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SearchAllUserInfo") );
	}

	
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7,asDes8,asDes9,asDes10,asDes11;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("자택주소"), i, strGetData);
			asDes1.Add(strGetData);
			pDM->GetCellData(_T("근무지주소"), i, strGetData);
			asDes2.Add(strGetData);
			pDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes3.Add(strGetData);
			pDM->GetCellData(_T("E_MAIL"), i, strGetData);
			asDes4.Add(strGetData);
			pDM->GetCellData(_T("휴대폰"), i, strGetData);
			asDes5.Add(strGetData);
			pDM->GetCellData(_T("비밀번호"), i, strGetData);
			asDes6.Add(strGetData);
			pDM->GetCellData(_T("자택전화"), i, strGetData);
			asDes7.Add(strGetData);
			pDM->GetCellData(_T("근무지전화"), i, strGetData);
			asDes8.Add(strGetData);
			pDM->GetCellData(_T("IDX_핸드폰"), i, strGetData);
			asDes9.Add(strGetData);
			pDM->GetCellData(_T("회원증비밀번호"), i, strGetData);
			asDes10.Add(strGetData);
			pDM->GetCellData(_T("제2연락처"), i, strGetData);
			asDes11.Add(strGetData);
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
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("IDX_HANDPHONE"), asDes9);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), asDes10);
					p3000Api->CloseSocket();	
				}
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("SECOND_PHONE"), asDes11);
					p3000Api->CloseSocket();	
				}
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("자택주소"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("근무지주소"), asDes2.GetAt(i), i);
					pDM->SetCellData(_T("주민등록번호"), asDes3.GetAt(i), i);
					pDM->SetCellData(_T("E_MAIL"), asDes4.GetAt(i), i);
					pDM->SetCellData(_T("휴대폰"), asDes5.GetAt(i), i);
					pDM->SetCellData(_T("비밀번호"), asDes6.GetAt(i), i);
					pDM->SetCellData(_T("자택전화"), asDes7.GetAt(i), i);
					pDM->SetCellData(_T("근무지전화"), asDes8.GetAt(i), i);
					pDM->SetCellData(_T("IDX_핸드폰"), asDes9.GetAt(i), i);
					pDM->SetCellData(_T("회원증비밀번호"), asDes10.GetAt(i), i);
					pDM->SetCellData(_T("제2연락처"), asDes11.GetAt(i), i);
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
		asDes9.RemoveAll(); asDes9.FreeExtra();
		asDes10.RemoveAll(); asDes10.FreeExtra();
		asDes11.RemoveAll(); asDes11.FreeExtra();
	}	
	if( 1 == m_nMode || 3 == m_nMode )
	{
		if ( 0 < m_pSpfWork->m_BackUpDM.GetRowCount() ) 
		{
			m_pSpfWork->m_BackUpDM.DeleteRow(0);
		}

		CLocCommonAPI::MakeCopyDM( this, pDM , &m_pSpfWork->m_BackUpDM );		
		CLocCommonAPI::AddDM( pDM, &m_pSpfWork->m_BackUpDM , this);	
	}		
	
	if( 10000 == m_pInfo->MODE || 30000 == m_pInfo->MODE )
	{
		return 0;
	}

	MakePictureFileFromDBLOBField();

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::ViewScreenLinkDM()
{
EFS_BEGIN

	INT ids;

	CString sCMAlias = m_strCMAlias;

	CESL_ControlMgr *pChildCM = FindCM( sCMAlias );
	if( NULL == pChildCM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewScreenLinkDM") );
	}	
	ids = SettingDisplayBefore();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ViewScreenLinkDM") );
	}

	if (0 == m_nMode || 1 == m_nMode || 3 == m_nMode)
	{
		if (!m_pInfo->m_bVisitorPwEncryption)
		{
 			((CEdit*)GetDlgItem(IDC_edtUSER5))->SetRedraw(FALSE);
 			((CEdit*)GetDlgItem(IDC_edtUSER6))->SetRedraw(FALSE);
		}

		if (!m_pInfo->m_bCardPwEncryption)
		{
	 		((CEdit*)GetDlgItem(IDC_edtUSER_35))->SetRedraw(FALSE);
		}
	}	
	ids = pChildCM->AllControlDisplay(0);
	if (0 > ids) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("ViewScreenLinkDM") );
	}

	if (0 == m_nMode || 1 == m_nMode || 3 == m_nMode)
	{
 		((CEdit*)GetDlgItem(IDC_edtUSER5))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER6))->SetPasswordChar(0x2022);
		((CEdit*)GetDlgItem(IDC_edtUSER_35))->SetPasswordChar(0x2022);

		if (!m_pInfo->m_bVisitorPwEncryption)
		{
 			((CEdit*)GetDlgItem(IDC_edtUSER5))->SetRedraw(TRUE);
 			((CEdit*)GetDlgItem(IDC_edtUSER6))->SetRedraw(TRUE);
		}

		if (!m_pInfo->m_bCardPwEncryption)
		{
 			((CEdit*)GetDlgItem(IDC_edtUSER_35))->SetRedraw(TRUE);
		}
	}
	
	ids = SettingDisplayAfter();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("ViewScreenLinkDM") );
	}
	
	InitPictureControl();	
	SetPreNextButton();	
	SetCivilNum(FALSE);	
	GetControlData( m_strCMAlias , _T("주민등록번호") , m_strLastCivilNo );	
	GetControlData( m_strCMAlias , _T("이름") , m_strLastName );

	return 0;

EFS_END
return -1;
}


VOID CBO_LOC_3320::OnbtnOLDDATA() 
{
EFS_BEGIN

	INT ids;
	ids = ViewScreenLinkDM();	
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnOLDDATA") );
	}

EFS_END
}


INT CBO_LOC_3320::GetRecKey( CString &sRecKey )
{
EFS_BEGIN

	INT ids;	

	if( ( 3 == m_nMode ) || ( 6 == m_nMode ) )
	{
		sRecKey = m_sRecKey;
		return 0;
	}

	CString sParentDMAlias = _T("DM_BO_LOC_3300_LIST");
	CString sFieldAlias = _T("REC_KEY");
	if( 4 == m_nMode )
	{
		sParentDMAlias = _T("DM_BO_LOC_3200");
		sFieldAlias = _T("대출자KEY");
	}

	if( 5 == m_nMode )
	{
		sParentDMAlias = _T("DM_BO_LOC_3240_BO");
		sFieldAlias = _T("대출자KEY");
	}

	if( 7 == m_nMode )
	{
		sParentDMAlias = _T("DM_BO_LOC_3400");
		sFieldAlias = _T("이용자KEY");
	}

	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );
	if( NULL == pParentDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetRecKey") );
	}
	
	ids = pParentDM->GetCellData( sFieldAlias , m_nCurrentIndex , sRecKey );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("GetRecKey") );
	}
	
	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3320::SetRecKey( CString sRecKey )
{
EFS_BEGIN

	m_sRecKey = sRecKey;

EFS_END
}

INT CBO_LOC_3320::ViewCodeToDesc()
{
EFS_BEGIN

	CString strFuncName = _T("ViewCodeToDesc");

	INT ids;
	CString sCMAlias = m_strCMAlias;
	CString sDMAlias = m_strDMAlias;

	const INT nFieldCnt = 2;
	TCHAR *sDMFieldAlias[nFieldCnt] =
	{
		_T("우편발송지역") , _T("회원구분") 
	};
	
	CString sGetData[nFieldCnt];

	INT		i;
	for( i=0 ; i<nFieldCnt ; i++ )
	{
		ids = GetDataMgrData( sDMAlias , sDMFieldAlias[i] , sGetData[i] , 0 );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , strFuncName );
	}
	
	if( 0 == sGetData[0].Compare(_T("H")) )
	{
		sGetData[0] = _T("0");	
	}
	else if( 0 == sGetData[0].Compare(_T("W")) )
	{
		sGetData[0] = _T("1");	
	}
	
	if( 0 == sGetData[1].Compare(_T("0")) )
	{
		sGetData[1] = _T("0");	
	}
	else if( 0 == sGetData[1].Compare(_T("1")) )
	{
		sGetData[1] = _T("1");	
	}	
	else if( 0 == sGetData[1].Compare(_T("2")) )
	{
		sGetData[1] = _T("2");	
	}

	INT nCMIDC[nFieldCnt] =
	{
		IDC_cmbUSER3 , IDC_cmbUSER4
	};

	for( i=0 ; i<nFieldCnt ; i++ )
	{
		ids = SetDataMgrData( sDMAlias , sDMFieldAlias[i] , sGetData[i] , 0 );
		
		((CComboBox*)GetDlgItem( nCMIDC[i] ))->SetCurSel( _ttoi(sGetData[i]) );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , strFuncName );
		}
	}
	
	CString strShelfLocCode;
	ids = GetDataMgrData( sDMAlias , _T("자료실") , strShelfLocCode , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , strFuncName );

	INT nCnt;
	BOOL bFind = FALSE;
	CString strComboData;

	CString sInsertCode[2];
	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3320"));
	if(NULL == pCM)ERROR_MSG_RETURN_ERROR_CODE2(-1040, strFuncName);
	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("자료실")));
	if(NULL == pCMCB)ERROR_MSG_RETURN_ERROR_CODE2(-1050, strFuncName);

	nCnt = pCMCB->GetRowCount();
	for(i=0; i<nCnt; i++)
	{
		pCMCB->GetData(strComboData,i,0);
		if(strComboData==strShelfLocCode)
		{
			bFind = TRUE;
			break;
		}
	}

	if(!bFind)
	{
		CString strData[2];
		strData[0] = strShelfLocCode;
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료실구분"), strShelfLocCode, strData[1]);
		pCMCB->SetData(strData, 2);
		pCMCB->SetCurSel(nCnt);
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::ChangeSUMFieldToControl( TCHAR **sControlField , CString sSeperator , CString sData , INT nFieldCnt )
{
EFS_BEGIN

	INT ids;
	CString sCMAlias = m_strCMAlias;
	CString sDMAlias = m_strDMAlias;
	
	if( TRUE != sData.IsEmpty() )
	{
		CString sTmp;
		sTmp = sData;
		CString sLine;
		INT nSIndex=0;
		INT i = 0;

		while(true)
		{
			nSIndex = sTmp.Find( sSeperator , 0 );
			if( 0 > nSIndex && TRUE == sTmp.IsEmpty() ) 
			{
				break;
			}

			if( -1 < nSIndex ) 
			{
				sLine = sTmp.Mid( 0 , nSIndex );
			}
			else
			{
				sLine = sTmp;
				sTmp.Empty();
			}
			
			if( 0 == i )
			{				
				CLocCommonAPI::SetPhoneNumCel( GetDlgItem( IDC_cmbUSER5 ), sLine );
			}
			else 
			{
				ids = SetControlData( sCMAlias , sControlField[i] , sLine );
				if( 0 > ids )
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ChangeSUMFieldToControl") );
				}
			}

			sLine.Empty();
			sTmp = sTmp.Mid( nSIndex+1 );
			i++;
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::SettingEmail()
{
EFS_BEGIN

	INT ids;
	CString sCMAlias = m_strCMAlias;
	CString sDMAlias = m_strDMAlias;

	TCHAR *sDMFieldAlias =
	{
		_T("E_MAIL")
	};
	
	CString sGetData;

	ids = GetDataMgrData( sDMAlias , sDMFieldAlias , sGetData , 0 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SettingEmail") );
	}

	TCHAR *sCMEmailFieldAlias[2] = 
	{
		_T("EMAIL1") , _T("EMAIL2") 
	};

	if( TRUE == sGetData.IsEmpty() ) 
	{
		
		CComboBox *pCombo = (CComboBox*)(GetDlgItem( IDC_cmbUSER6 ));
		INT nCount = pCombo->GetCount();
		CString sGetComboData;
		BOOL bEqual = FALSE;
		INT		j;
		for( j=0 ; j<nCount ; j++ )
		{
			pCombo->GetLBText( j , sGetComboData );
			if( 0 == sGetComboData.Compare(_T("적용안함")) ) 
			{
				bEqual = TRUE;
			}
		}

		if( TRUE != bEqual ) 
		{
			pCombo->AddString( _T("적용안함") );
		}

		pCombo->SetWindowText( _T("적용안함") );

		ids = SetControlData( sCMAlias , _T("EMAIL1") , _T("") ); 
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SettingEmail") );
		}
	
		return 0;
	}
	
	sGetData.Replace(_T("@@"), _T("@"));
	CString sTmp = sGetData;
	CString sLine;
	INT nSIndex=0;
	INT i = 0;

	while(true)
	{
		nSIndex = sTmp.Find( _T("@") , 0 );
		if( 0 > nSIndex && TRUE == sTmp.IsEmpty() ) 
		{
			break;
		}

		if( -1 < nSIndex ) 
		{
			sLine = sTmp.Mid( 0 , nSIndex );
		}
		else
		{
			sLine = sTmp;
			sTmp.Empty();
		}

		if( 1 == i ) 
		{
			CComboBox *pCombo = (CComboBox*)(GetDlgItem( IDC_cmbUSER6 ));
			INT nCount = pCombo->GetCount();
			CString sGetComboData;
			bool bEqual = false;
			INT		j;
			for( j=0 ; j<nCount ; j++ )
			{
				pCombo->GetLBText( j , sGetComboData );
				if( 0 == sGetComboData.Compare(sLine) )
				{
					
					bEqual = true;
				}
			}

			if( TRUE != bEqual )
			{	
				pCombo->AddString( sLine );
				
			}

			pCombo->SetWindowText( sLine );
		}
		else
		{
			ids = SetControlData( sCMAlias , sCMEmailFieldAlias[i] , sLine ); 
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SettingEmail") );
			}
		}
		
		sLine.Empty();
		sTmp = sTmp.Mid( nSIndex+1 );
		i++;
	}
	return 0;	

EFS_END
return -1;
}

INT CBO_LOC_3320::SettingDisplayBefore()
{
EFS_BEGIN

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::SettingDisplayAfter()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SettingDisplayAfter");	

	CString sGetData;
	ids = GetDataMgrData( m_strDMAlias , _T("휴대폰") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1010 , strFuncName );
	
	ViewHandPhoneNum( sGetData );	
	ids = SettingEmail();
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1020 , strFuncName );

	
	ids = GetDataMgrData( m_strDMAlias , _T("불량회원구분") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , strFuncName );

	INT nUserState = 0;
	nUserState = _ttoi(sGetData);
	((CComboBox*)GetDlgItem(IDC_cmbUSER7))->SetCurSel( nUserState );

	
	ids = ViewCodeToDesc();
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1040 , strFuncName );

	
	ids = GetDataMgrData( m_strDMAlias , _T("생일TYPE") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1050 , strFuncName );

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbUSER8);
	CString sStr;
	INT		i;
	for( i=0 ; i<pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sStr);
		if( 0 == sStr.Compare(sGetData) )
		{
			pCombo->SetCurSel(i);
		}
	}

	ids = GetDataMgrData( m_strDMAlias , _T("생일") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1060 , strFuncName );
	
	ids = SetControlData( m_strCMAlias , _T("생일") , sGetData );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1070 , strFuncName );

	
	ids = GetDataMgrData( m_strDMAlias , _T("회원증분실") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1080 , strFuncName );

	if( TRUE == sGetData.IsEmpty() ) 
	{
		sGetData = _T("N");
	}
	
	pCombo = (CComboBox*)GetDlgItem(IDC_cmbLOST_MEMBER);
	
	for( i=0 ; i<pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sStr);
		if( 0 == sStr.Compare(sGetData) )
		{
			pCombo->SetCurSel(i);
		}
	}

	ids = GetDataMgrData( m_strDMAlias , _T("발급횟수") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1090 , strFuncName );

	if(sGetData == _T("")) 
	{
		ids = SetControlData( m_strCMAlias , _T("발급횟수") , _T("1") );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1100 , strFuncName );
	}
		
	ids = GetDataMgrData( m_strDMAlias , _T("무인대출제한") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1110 , strFuncName );

	if( TRUE == sGetData.IsEmpty() ) 
	{
		sGetData = _T("N");
	}
	
	pCombo = (CComboBox*)GetDlgItem(IDC_CMB_SELF_LOAN_STATION);
	for( i=0 ; i<pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sStr);
		if( 0 == sStr.Compare(sGetData) )
		{
			pCombo->SetCurSel(i);
		}
	}
	
	ids = GetDataMgrData( m_strDMAlias , _T("SMS수신여부") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1120 , strFuncName );
	
	if( 0 != m_sSMSVersion.Compare(_T("N")) )
	{ 
		if( ( 0 == sGetData.Compare(_T("N")) || TRUE == sGetData.IsEmpty() ) && 0 == m_sSMSVersion.Compare(_T("Y")) )
		{
			((CButton*)GetDlgItem(IDC_chkSMSUSEYN))->SetCheck(FALSE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_chkSMSUSEYN))->SetCheck(TRUE);
		}
	}
	else
	{ 
		((CButton*)GetDlgItem(IDC_chkSMSUSEYN))->SetCheck(FALSE);
	}

	ids = GetDataMgrData( m_strDMAlias , _T("메일수신여부") , sGetData , 0 );
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1130 , strFuncName );
	
	if( 0 == sGetData.Compare(_T("N")) )
	{
		((CButton*)GetDlgItem(IDC_chkMAILUSEYN))->SetCheck(FALSE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_chkMAILUSEYN))->SetCheck(TRUE);									  
	}
	
	if( -1 != LastGrantUserNoType ) 
	{
		( (CComboBox*) GetDlgItem (IDC_cmbUSER9) )->SetCurSel(LastGrantUserNoType);	
	}
	
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		ids = GetDataMgrData( m_strDMAlias , _T("통합회원전환여부") , sGetData , 0 );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1140 , strFuncName );

		pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
		if( 0 == sGetData.Compare(_T("Y")) )
		{
			m_bIsKLMemberYN = TRUE;
			pCombo->SetCurSel(0);
			pCombo->EnableWindow(FALSE);				
			((CButton*)GetDlgItem(IDC_btnKLRealNameCert))->ShowWindow(SW_HIDE);			
			((CButton*)GetDlgItem(IDC_btnSearchKLLib))->ShowWindow(SW_SHOW);			
			((CButton*)GetDlgItem(IDC_btnSearchKLCurrentLoan))->ShowWindow(SW_SHOW);			
			((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("책이음회원증\r\n발급이력"));
			
			GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_SHOW);
			
			m_bIsRealNameCert = TRUE;
		}
		else
		{			
			if (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 && !m_pParent->m_pInfo->m_bCertifyUse)
			{					
				GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_SHOW);
			}
			else
			{
				GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_HIDE);
			}

			ids = GetDataMgrData( m_strDMAlias , _T("실명인증코드") , sGetData , 0 );
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1140 , strFuncName );

			m_bIsKLMemberYN = FALSE;
			pCombo->SetCurSel(1);
			if (g_bEnableCertify)
			{
				pCombo->EnableWindow(TRUE);
			}
			else
			{
				pCombo->EnableWindow(!sGetData.IsEmpty());
			}
			((CButton*)GetDlgItem(IDC_btnSearchKLLib))->ShowWindow(SW_HIDE);
			
			((CButton*)GetDlgItem(IDC_btnSearchKLCurrentLoan))->ShowWindow(SW_HIDE);
			
			((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증\r\n발급이력"));
			
			GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_HIDE);

			m_bIsRealNameCert = FALSE;
			CString strCertFlag;
			GetDataMgrData( m_strDMAlias , _T("실명인증코드") , strCertFlag , 0 );
			if(!strCertFlag.IsEmpty()) m_bIsRealNameCert = TRUE;
		}
		((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->ShowWindow(SW_SHOW);

		ids = GetDataMgrData( m_strDMAlias , _T("GPIN_HASH") , m_strGPIN_HASH , 0 );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1150 , strFuncName );
		m_strGPIN_HASH.TrimLeft();	m_strGPIN_HASH.TrimRight();

		ids = GetDataMgrData( m_strDMAlias , _T("GPIN_ASCII") , m_strGPIN_ASCII , 0 );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1160 , strFuncName );
		m_strGPIN_ASCII.TrimLeft();	m_strGPIN_ASCII.TrimRight();

		ids = GetDataMgrData( m_strDMAlias , _T("GPIN_SEX") , m_strGPIN_SEX , 0 );
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1170 , strFuncName );
		m_strGPIN_SEX.TrimLeft();	m_strGPIN_SEX.TrimRight();

		
		if(TRUE != m_pInfo->m_bCertifyUse)
		{
			ids = GetDataMgrData(m_strDMAlias, _T("실명인증코드"), m_strCI, 0);
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1180, strFuncName);
			m_strCI.TrimLeft();	m_strCI.TrimRight();
		}
	}
	else
	{
		((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증\r\n발급이력"));
		CESL_DataMgr* pDM = FindDM(m_strDMAlias);
		CString strPassword, strCardPassword;
		strPassword = pDM->GetCellData(_T("비밀번호"), 0);
		strCardPassword = pDM->GetCellData(_T("회원증비밀번호"), 0);
		if(m_pInfo->m_bVisitorPwEncryption && m_pInfo->m_bCardPwEncryption && _T("") != strPassword && _T("") != strCardPassword)
		{
			
			((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증 발급이력"));	
		}
		((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_btnKLRealNameCert)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSearchKLLib)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnSearchKLCurrentLoan)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_staKLMemberYN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_cmbKLMemberYN)->ShowWindow(SW_HIDE);

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
		pCombo->SetCurSel(1);
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::SetSelect()
{
EFS_BEGIN

	INT ids;

	if( ( 0 == m_nMode ) || ( 3 == m_nMode ) || ( 6 == m_nMode ) ) 
	{
		return 0;
	}

	CString sCMAlias = _T("CM_BO_LOC_3300");
	CString sGridAlias = _T("MainGrid");

	if( 4 == m_nMode )
	{
		sCMAlias = _T("CM_BO_LOC_3200");
		sGridAlias = _T("MainGrid");
		if( TRUE != m_sGridAlias.IsEmpty() ) 
		{
			sGridAlias = m_sGridAlias;
		}
	}
	if( 5 == m_nMode )
	{
		sCMAlias = _T("CM_BO_LOC_3240");
		sGridAlias = _T("ReservationProcessGrid");
	}
	if( 7 == m_nMode )
	{
		sCMAlias = _T("CM_BO_LOC_3400");
		sGridAlias = _T("grid1");
	}

	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( NULL == pCM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SetSelect") );
	}

	CESL_Grid *pGrid = ((CESL_Grid*)(pCM->FindControl( sGridAlias )));
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SetSelect") );
	}
	
	ids = pGrid->SetReverse( m_nCurrentIndex );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SetSelect") );
	}

	return 0;

EFS_END
return -1;
}


VOID CBO_LOC_3320::OnSelchangecmbUSER7() 
{
EFS_BEGIN

	INT ids;

	CString sState;
	CComboBox *pComboBox = (CComboBox*)(GetDlgItem(IDC_cmbUSER7));
	pComboBox->GetLBText( pComboBox->GetCurSel() , sState );

	CString sLostState;
	CComboBox *pLostCardBox = (CComboBox*)(GetDlgItem(IDC_cmbLOST_MEMBER));
	pLostCardBox->GetLBText( pLostCardBox->GetCurSel(), sLostState );
	
	if( 0 == sState.Compare( _T("정상회원") ) )
	{	
		ids = SetControlData( m_strCMAlias, _T("대출정지일") , _T("") );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_VOID2( -1010 , _T("OnSelchangecmbUSER7") );
		}
		
		ids = SetControlData( m_strCMAlias, _T("제적일자"), _T("") );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_VOID2( -1020 , _T("OnSelchangecmbUSER7") );
		}
	}

	if( 0 != sState.Compare( _T("제적") ) && 0 != sState.Compare( _T("제적회원") ) ) 
	{
		return;
	}		
	
	CString strGetData;
	CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	CString sCurrentTime = strGetData.Left(10);
	
	ids = SetControlData( m_strCMAlias , _T("제적일자") , sCurrentTime );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1030 , _T("OnSelchangecmbUSER7") );
	}
	
EFS_END
}

VOID CBO_LOC_3320::OnbtnCLEAR() 
{
EFS_BEGIN

	INT ids;

	CString sCMAlias = m_strCMAlias;
	CESL_ControlMgr *pCM = FindCM( sCMAlias );
	if( NULL == pCM ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010 , _T("OnbtnCLEAR") );
	}
	
	ids = pCM->AllControlClear();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1020 , _T("OnbtnCLEAR") );
	}	
	m_sImageFilePath = _T("");

	InitPictureControl();
	
	m_Secede_Mode = 0;
		
	SetScreen(TRUE);
	SetDefaultMail();
	GetDlgItem(IDC_EDIT_CIVILNUM1)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_CIVILNUM2)->SetWindowText(_T(""));
	
	m_nLTMemberSaveMode = 0;	
	m_bIsRealNameCert = FALSE;	
	m_bIsKLMemberYN = FALSE;	
	m_bIsSelfLibSecede = FALSE;	
	m_strKLUserKey = _T("");	
	m_strKLWORKNO = _T("");	
	m_strGPIN_ASCII = _T("");	
	m_strGPIN_HASH = _T("");	
	m_strGPIN_SEX = _T("");	
	m_bIsTCTInput = FALSE;	
	m_bIsReEntry = FALSE;	
	m_strLastCivilNo = _T("");
	m_strLastName = _T("");
	OnSelchangecmbKLMemberYN();

EFS_END
}

CESL_Grid* CBO_LOC_3320::GetMainGridPtr()
{
EFS_BEGIN

	CString sParentCMAlias = _T("CM_BO_LOC_3300");
	CString sControlAlias = _T("MainGrid");

	if( 4 == m_nMode )
	{
		sParentCMAlias = _T("CM_BO_LOC_3200");
		sControlAlias = _T("MainGrid");
		if( TRUE != m_sGridAlias.IsEmpty() ) 
		{
			sControlAlias = m_sGridAlias;
		}
	}

	if( 5 == m_nMode )
	{
		sParentCMAlias = _T("CM_BO_LOC_3240");
		sControlAlias = _T("ReservationProcessGrid");
	}

	if( 7 == m_nMode )
	{
		sParentCMAlias = _T("CM_BO_LOC_3400");
		sControlAlias = _T("grid1");
	}

	CESL_ControlMgr *pCM = FindCM( sParentCMAlias );
	if( NULL == pCM ) 
	{
		return NULL;
	}

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sControlAlias));
	if( NULL == pGrid ) 
	{
		return NULL;
	}

	return pGrid;

EFS_END
return NULL;
}

INT CBO_LOC_3320::MoveIndex( INT nMode )
{	
EFS_BEGIN

	INT ids;
	ids = SetCurrentIndex();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MoveIndex") );
	}

	if( 0 == m_nViewMode )
	{
		if( 0 == nMode && ( m_nCurrentIndex != m_nFirstIndex )) 
		{
			m_nCurrentIndex--;
		}
		else if( 1 == nMode && ( m_nCurrentIndex != m_nLastIndex ) ) 
		{
			m_nCurrentIndex++;
		}
	}
	else
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if( 0 == nMode )
		{
			m_nCurrentIndex = pGrid->SelectGetPrev();
		}
		else if( 1 == nMode && ( m_nCurrentIndex != m_nLastIndex ) )
		{
			m_nCurrentIndex = pGrid->SelectGetNext();
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::CheckParentDMCount()
{
EFS_BEGIN

	CString sParentDMAlias = _T("DM_BO_LOC_3300_LIST");
	CESL_DataMgr *pParentDM = FindDM( sParentDMAlias );

	return pParentDM->GetRowCount();

EFS_END
return -1;
}

VOID CBO_LOC_3320::OnClose() 
{
	EFS_BEGIN

	m_strLastCivilNo = _T("");
	
	m_strLastName = _T("");

	if( m_bIsDoModal ) 
	{
		CDialog::OnClose();
	}
	else
	{		
		ShowWindow(SW_HIDE);
	}

	EFS_END
}
VOID CBO_LOC_3320::OnChangeedtCIVILNUM1() 
{
	if(m_bAutoCivilNoInsert==TRUE)
	{
		return;
	}

	CString strText;
	GetDlgItemText(IDC_EDIT_CIVILNUM1, strText);
		
	if( 6 == strText.GetLength() && TRUE == m_bSetCivil2ToPassword )
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_CIVILNUM2);
		if(NULL != pWnd)
		{
			pWnd->SetFocus();
		}
	}
}

VOID CBO_LOC_3320::OnChangeedtUSER1() 
{
	EFS_BEGIN
	
	if( 0 == m_nMode )
	{
		INT ids;

		CString strUserNo;

		ids = GetControlData( m_strCMAlias , _T("대출자번호") , strUserNo );
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_VOID2( -1000 , _T("OnChangeedtUSER1") ); 
		}
		strUserNo.TrimLeft(); 
		strUserNo.TrimRight();

		
		CString strBuf;
		if( 13 <= strUserNo.GetLength() )
		{
			strBuf = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6);
		}
		ids = CLocCommonAPI::CivilNoCheck( strBuf );
		if(0 == ids)
		{
			strUserNo = strUserNo.Mid(0,6) + _T("-") + strUserNo.Mid(6,7);
			ids = SetControlData( m_strCMAlias, _T("주민등록번호"), strUserNo );
			if(0 > ids) 
			{
				ERROR_MSG_RETURN_VOID2( -1010, _T("OnChangeedtUSER1") ); 
			}
			
			m_bAutoCivilNoInsert = TRUE;
			SetCivilNum();
			
			OnKillfocusEditCivilNum2();
			m_bAutoCivilNoInsert = FALSE;
		}
	}

	EFS_END
}

VOID CBO_LOC_3320::OnChangeedtUSER19()
{
	CString strText;
	GetDlgItemText(IDC_edtUSER19, strText);
	
	
	if( 4 == strText.GetLength())
	{
		CWnd* pWnd = GetDlgItem(IDC_edtUSER20);
		if(NULL != pWnd)
		{
			pWnd->SetFocus();
		}
	}
}

VOID CBO_LOC_3320::SetGridAlias( CString sGridAlias )
{
	EFS_BEGIN

	m_sGridAlias = sGridAlias;

	EFS_END
}

VOID CBO_LOC_3320::OnbtnDUPLICATE4() 
{
EFS_BEGIN
	INT		ids;
	bool	IsDuplicate = FALSE;
	
	CString civilNum;
	GetDlgItemText( IDC_edtUSER8, civilNum );
	SetDlgItemText( IDC_edtUSER8, MakeCivilNumFormat( civilNum ) );

	CStringArray aUserKey;
	ids = IsDuplicateCivilNo( IsDuplicate , &aUserKey);	

	if( 0!=ids ) return;

	if( IsDuplicate )
	{
		if( 0 == m_nMode )
		{
			if( IDYES == AfxMessageBox( _T("중복된 주민등록번호입니다.\r\n"
			                               _T("중복된 이용자를 확인하시겠습니까?")), MB_ICONSTOP | MB_YESNO) )
			{
				m_pSpfWork->SearchAddLoanUser(aUserKey);

				CBO_LOC_3320 Dlg( m_pParent, m_pLoanShareManager, m_pLoanMangeValue );
				Dlg.SetMode(1,TRUE);
				if( Dlg.CheckParentDMCount() < 1 )
				{
					return ;
				}				
				Dlg.DoModal();

				GetDlgItem(IDC_EDIT_CIVILNUM1)->SetFocus();
			}
		}
		else
		{
			AfxMessageBox( _T("중복된 주민등록번호입니다. 다른 번호를 입력하십시오"), MB_ICONSTOP );
		}
	}
	else
	{
		
		if( 0 == m_nMode && 0 == m_Secede_Mode)
		{
			ids = IsReEntry(m_bIsReEntry);
			if(0 != ids) return;
			if(m_bIsReEntry)
			{
				int nResult = AfxMessageBox(_T("탈퇴한 이용자 정보가 존재합니다.\r\n")
											_T("탈퇴 정보를 확인 하시겠습니까?"), MB_ICONSTOP | MB_YESNO);
				if(IDYES == nResult)
				{
					CString strCivilNo;
					GetDlgItemText(IDC_edtUSER8, strCivilNo);
					CSECEDE_USER_DLG Dlg(this->m_pSpfWork, this);
					Dlg.SetCivilNo(strCivilNo);
					Dlg.m_sIsUnityLoanService = m_pLoanMangeValue->m_sIsUnityLoanService;
					Dlg.DoModal();
					if(Dlg.m_bApply) m_Secede_Mode = 2;
					return;
				}
				else return;
			}
		}

		AfxMessageBox( _T("중복된 주민등록번호가 존재하지 않습니다. "), MB_ICONINFORMATION );
	}	
	
EFS_END
}

INT CBO_LOC_3320::IsDuplicateCivilNo( bool &bIsDuplicate, CStringArray* paUserKey )
{
EFS_BEGIN
	INT		ids;
	CString strCivilNo;
	CString strQuery;
	CString strWhere;
	CString strResult;

	bIsDuplicate = FALSE;

	CESL_DataMgr* pDM = FindDM( m_strDMAlias );
	if( !pDM ) return -1;

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	
	ids = GetControlData( m_strCMAlias , _T("주민등록번호") , strCivilNo );
	if( 0>ids ) return -1;

	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				CStringArray DataArray;
				DataArray.Add(strCivilNo);
				if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray))
				{
					strCivilNo = DataArray.GetAt(0);
				}
				DataArray.RemoveAll(); DataArray.FreeExtra();
				p3000Api->CloseSocket();
			}
			delete p3000Api;
			p3000Api = NULL;
		}
	}

	if( strCivilNo.IsEmpty() )
	{
		AfxMessageBox( _T("주민등록 번호를 입력하시고 중복 조회 기능을 사용하십시오") );		
		return 2;
	}
	
	if( 0 != m_nMode )
	{
		CString strRecKey;
		ids = pDM->GetCellData( _T("REC_KEY"), 0, strRecKey );
		if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3320::IsDuplicateCivilNo()") );

		strRecKey.TrimLeft(); strRecKey.TrimRight();
		if( !strRecKey.IsEmpty() )
		{
			strWhere = _T(" AND REC_KEY NOT IN ( '")+ strRecKey +_T("' )");
		}
	}

	strCivilNo.Replace(_T("-"),_T(""));	
	CString strHalfLineCivilNo = strCivilNo;	
	if( 6 < strHalfLineCivilNo.GetLength() )
	{
		strHalfLineCivilNo = strHalfLineCivilNo.Mid(0,6) + _T("-") + strHalfLineCivilNo.Mid(6);
	}	
	
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		this->ESL_DES_Encryption(strCivilNo, strCivilNo);
		this->ESL_DES_Encryption(strHalfLineCivilNo, strHalfLineCivilNo);
		strQuery.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL ")
						 _T(" WHERE CRYPTO_CIVIL_NO IN ('%s','%s')")
						 _T(" %s ")
						 _T(" AND (USER_CLASS!='3' OR MEMBER_CLASS!='1')")
						 , strCivilNo, strHalfLineCivilNo, strWhere );
	}
	else
	{
		strQuery.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL ")
	
						 _T(" WHERE CIVIL_NO IN ('%s','%s')")
						 _T(" %s ")
						 
						 
						 _T(" AND (USER_CLASS!='3' OR MEMBER_CLASS!='1')")
						 , strCivilNo, strHalfLineCivilNo, strWhere );
	}

	TmpDM.RestructDataManager(strQuery);
  	if( 0 > ids ) 
  	{
  		ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3320::IsDuplicateCivilNo()") );
  	}

	if( 0 < TmpDM.GetRowCount() ) 
	{
		bIsDuplicate = TRUE;

		if(NULL != paUserKey)
		{
			CString strUserKey;
			const int ROW_COUNT = TmpDM.GetRowCount();
			int nRowCnt = 0;
			for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
			{
				strUserKey = TmpDM.GetCellData(nRowCnt, 0);

				if(FALSE == strUserKey.IsEmpty())
				{
					paUserKey->Add(strUserKey);
				}
			}
		} 
	} 
	
	if( !bIsDuplicate )
	{
		if( 0 == m_nMode && GetDlgItem(IDC_btnINPUTCENTER)->IsWindowVisible() )
		{			
			if(m_bImport == FALSE)
			{			
				if( m_pCenterUserMgr )
				{
					delete m_pCenterUserMgr;
					m_pCenterUserMgr = NULL;
				}
			
				m_pCenterUserMgr = new CCO_CENTER_USER_MANAGER_01( this );
				m_pCenterUserMgr->Create( this );
				m_pCenterUserMgr->m_mode = 2;
			
				GetDlgItemText(IDC_edtUSER8, m_pCenterUserMgr->m_initCivilNum);
				GetDlgItemText(IDC_edtUSER7, m_pCenterUserMgr->m_initUserName);
					
				ids = m_pCenterUserMgr->IsDuplicateCenterUser();
				if( -3005 == ids )
				{
					AfxMessageBox( _T("센터의 데몬을 확인해 주세요"), MB_ICONSTOP );
				}
				if( 0 > ids ) return ids;

				
				if( 0 < ids )
				{
					INT nResult = AfxMessageBox(_T("센터 대출자 정보에서 중복 대출자가 존재합니다\r\n")
												_T("센터 대출자 반입을 수행 하시겠습니까?\r\n\r\n")
												_T("예       : 센터 대출자 반입\r\n")
												_T("아니오 : 이용자 입력\r\n")
												_T("취소    : 이용자 입력 취소"), MB_ICONQUESTION|MB_YESNOCANCEL);
					if( IDYES == nResult )
					{
						m_pCenterUserMgr->ShowWindow( true );
						return 1;
					}
					else if( IDNO == nResult )
					{
					}
					else if( IDCANCEL == nResult )
					{
						AfxMessageBox(_T("이용자 입력이 취소되었습니다"), MB_ICONSTOP);
						return 1;
					}
				}
			}
		}
	}
	


	return 0;
EFS_END
	return -1;
}

VOID CBO_LOC_3320::OnbtnFIND1() 
{
	EFS_BEGIN

	
CSearchZipCodeDlgOpenAPI dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_edtUSER9)->SetWindowText(dlg.GetApplyZipCode());
		GetDlgItem(IDC_edtUSER10)->SetFocus();
		GetDlgItem(IDC_edtUSER10)->SetWindowText(dlg.GetApplyAddress());
		((CEdit*)GetDlgItem(IDC_edtUSER10))->SetSel(dlg.GetApplyAddress().GetLength(),dlg.GetApplyAddress().GetLength());
	}
	EFS_END
}

VOID CBO_LOC_3320::OnbtnFIND2() 
{
	EFS_BEGIN

CSearchZipCodeDlgOpenAPI dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		GetDlgItem(IDC_edtUSER12)->SetWindowText(dlg.GetApplyZipCode());
		GetDlgItem(IDC_edtUSER13)->SetFocus();
		GetDlgItem(IDC_edtUSER13)->SetWindowText(dlg.GetApplyAddress());
		((CEdit*)GetDlgItem(IDC_edtUSER13))->SetSel(dlg.GetApplyAddress().GetLength(),dlg.GetApplyAddress().GetLength());
	}	
	EFS_END
}
VOID CBO_LOC_3320::OnSetfocusEdtUser16()
{
	CESL_ControlMgr *pCM = FindCM(m_strCMAlias);
	if(pCM) 
	{
		CESL_ControlEdit *pEdit1 = (CESL_ControlEdit *)pCM->FindControl(_T("학교명"));

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

	}	
}
VOID CBO_LOC_3320::OnSetfocusEdtUser34()
{
	CESL_ControlMgr *pCM = FindCM(m_strCMAlias);
	if(pCM) 
	{
		CESL_ControlEdit *pEdit1 = (CESL_ControlEdit *)pCM->FindControl(_T("근무처"));

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

	}	
}
VOID CBO_LOC_3320::OnSetfocusEdtUser29()
{
	CESL_ControlMgr *pCM = FindCM(m_strCMAlias);
	if(pCM) 
	{
		CESL_ControlEdit *pEdit1 = (CESL_ControlEdit *)pCM->FindControl(_T("부서명"));

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

	}	
}
VOID CBO_LOC_3320::OnSetfocusedtUSER21() 
{
	EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSER21)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_edtUSER21)->m_hWnd,hIME); 
    }			
	EFS_END
}

INT CBO_LOC_3320::IsInputControl()
{
EFS_BEGIN

	INT ids;
	CString sCMAlias = m_strCMAlias;
	
	const INT nCnt=13;
	TCHAR *sControlAlias[nCnt] = 
	{
		_T("대출자번호"), _T("사용자ID"),     _T("비밀번호"), _T("비밀번호확인"), 
		_T("이름"), 	  _T("주민등록번호"), _T("소속"),     _T("직급"), 
		_T("휴대폰1"),    _T("휴대폰2"),      _T("휴대폰3"),  _T("자택전화"), 
		_T("근무지전화")
	};

	CString sResultData[nCnt];
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = GetControlData( sCMAlias , sControlAlias[i] , sResultData[i] , -1 , 0 );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsInputControl") );

		sResultData[i].TrimRight(); sResultData[i].TrimLeft();		
	}
	
	if( sResultData[0].IsEmpty() )
	{
		AfxMessageBox(_T("대출자 번호를 먼저 입력해주십시오"));
		GetDlgItem(IDC_edtUSER1)->SetFocus();
		return 1;
	}
	
	if( sResultData[1].IsEmpty() && (!sResultData[2].IsEmpty()) )
	{
		AfxMessageBox(_T("아이디를 먼저 입력해주십시오"));
		GetDlgItem(IDC_edtUSER4)->SetFocus();
		return 1;
	}	
	
	if( m_nUserNoAutoMode == 3 )
	{
		if( sResultData[1].IsEmpty() )
		{
			AfxMessageBox(_T("사용자ID를 입력해주십시오"));
			GetDlgItem(IDC_edtUSER4)->SetFocus();
			return 1;
		}
	}
	
	if( !sResultData[1].IsEmpty() && (sResultData[2].IsEmpty()) )
	{
		AfxMessageBox(_T("비밀번호를 먼저 입력해주십시오"));
		GetDlgItem(IDC_edtUSER5)->SetFocus();
		return 1;
	}

	if( !sResultData[1].IsEmpty() && (!sResultData[2].IsEmpty()) )
	{
		if( sResultData[2].Compare( sResultData[3] ) != 0 )
		{
			AfxMessageBox(_T("비밀번호와 비밀번호 확인이 다릅니다. 다시 입력해주십시오"));
			GetDlgItem(IDC_edtUSER5)->SetFocus();
			return 1;
		}
	}

	
	if( sResultData[4].IsEmpty() )
	{
		AfxMessageBox(_T("이름을 먼저 입력해주십시오"));
		GetDlgItem(IDC_edtUSER7)->SetFocus();
		return 1;
	}

	if (sResultData[5].IsEmpty() && m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0) 
	{
		BOOL bDisplayCheckMessage = FALSE;
		if (m_bIsKLMemberYN)
		{
			
			bDisplayCheckMessage = TRUE;
		}
		if (((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO))->GetCheck())
		{
			
			bDisplayCheckMessage = TRUE;
		}

		if (bDisplayCheckMessage)
		{
			AfxMessageBox(_T("주민등록번호를 먼저 입력해주십시오"));
 			GetDlgItem(IDC_EDIT_CIVILNUM1)->SetFocus();
 			return 1;
		}
	}

	
	if( sResultData[6].IsEmpty() )
	{
		AfxMessageBox(_T("소속을 먼저 입력해주십시오"));
		GetDlgItem(IDC_cmbUSER1)->SetFocus();
		return 1;
	}

	if( sResultData[7].IsEmpty() )
	{
		AfxMessageBox(_T("직급을 먼저 입력해주십시오"));
		GetDlgItem(IDC_cmbUSER2)->SetFocus();
		return 1;
	}

	if ( sResultData[8] != _T("적용안함") && sResultData[8].GetLength() > 0 ) {
		if( ( sResultData[8].GetLength() + sResultData[9].GetLength() + sResultData[10].GetLength() + 2 ) > 35 )
		{
			AfxMessageBox(_T("휴대폰 번호가 35자가 넘어가면 저장되지 않습니다."));
			GetDlgItem(IDC_cmbUSER5)->SetFocus();
			return 1;
		}
	
	}
	if ( sResultData[8] == _T("적용안함") ) {
		if ( sResultData[9].GetLength() > 0 || sResultData[10].GetLength() > 0 ) {
			AfxMessageBox(_T("올바른 휴대폰 번호를 입력하여 주십시오."));
			GetDlgItem(IDC_cmbUSER5)->SetFocus();
			return 1;
		}
	}
	
	if( sResultData[11].GetLength() > 35 )
	{
		AfxMessageBox(_T("집전화 번호가 35자가 넘어가면 저장되지 않습니다."));
		GetDlgItem(IDC_edtUSER11)->SetFocus();
		return 1;
	}
	
	if( sResultData[12].GetLength() > 35 )
	{
		AfxMessageBox(_T("근무지 전화 번호가 35자가 넘어가면 저장되지 않습니다."));
		GetDlgItem(IDC_edtUSER14)->SetFocus();
		return 1;
	}
	
	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3320::OnbtnINPUTCENTER() 
{
EFS_BEGIN

	if( m_pCenterUserMgr )
	{
		delete m_pCenterUserMgr;
		m_pCenterUserMgr = NULL;
	}
	
	m_pCenterUserMgr = new CCO_CENTER_USER_MANAGER_01( this );
	m_pCenterUserMgr->Create( this );
	m_pCenterUserMgr->m_mode = 2;
	
	GetDlgItemText( IDC_edtUSER8, m_pCenterUserMgr->m_initCivilNum );
	GetDlgItemText( IDC_edtUSER7, m_pCenterUserMgr->m_initUserName );
	m_pCenterUserMgr->ShowWindow( true );
	
	if(TRUE == m_bImport)
	{
		m_nLTMemberSaveMode = 0;
		m_bIsRealNameCert = FALSE;
		m_bIsKLMemberYN = FALSE;
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
		pCombo->SetCurSel(1);
		pCombo->EnableWindow(TRUE);
		
		if(TRUE == m_pParent->m_pInfo->m_bKLUse && TRUE == m_pParent->m_pInfo->m_bCertifyUse)
		{
			((CButton*)GetDlgItem(IDC_btnKLRealNameCert))->ShowWindow(SW_HIDE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_btnKLRealNameCert))->ShowWindow(SW_SHOW);
		}

		((CButton*)GetDlgItem(IDC_btnSearchKLLib))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_btnSearchKLCurrentLoan))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_BTN_USER_CARD))->SetWindowText(_T("회원증\r\n발급이력"));
		GetDlgItem(IDC_btn3320_IDPASS_VIEW)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_picUnityUserIcon)->ShowWindow(SW_HIDE);
	}

EFS_END
}

INT CBO_LOC_3320::ImportCenterUser()
{
EFS_BEGIN

	CCO_CENTER_USER_MANAGER_01 ImportUser(this,1);
	ImportUser.DoModal();

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::ApplyMobile()
{
EFS_BEGIN

	INT ids;
	
	CString sValue = m_sSMSVersion;
	CESL_DataMgr *pDM = FindDM( m_strDMAlias );
	
	
	if( 0 == sValue.Compare(_T("N")) || TRUE == sValue.IsEmpty() )
	{
		INT nColumnCnt = pDM->RefList.GetCount();
		
		CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
		for( INT i = 0 ; i < nColumnCnt ; i++ )
		{
			sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
			ids = pDM->GetColumnRefInfo( i , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ApplyMobile") );
			}
			
			if( 0 != sFieldAlias.Compare( _T("SMS수신여부") ) ) 
			{
				continue;
			}
			
			ids = pDM->SetDataMgrRef( i , sFieldAlias , sFieldName , sFieldType  , sInitValue  , _T("udf") );			
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ApplyMobile") );
			}
		}		
	}


	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3320::OnSetfocusedtUSER7() 
{
EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM(m_strCMAlias);
	if (pCM) {
		CESL_ControlEdit *pEdit1 = (CESL_ControlEdit *)pCM->FindControl(_T("이름"));


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
		
	}

EFS_END
}

VOID CBO_LOC_3320::OnSetfocusedtUSER10() 
{
EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM(m_strCMAlias);
	if (pCM) 
	{
		CESL_ControlEdit *pEdit1 = (CESL_ControlEdit *)pCM->FindControl(_T("자택주소"));

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

	}

EFS_END
}

VOID CBO_LOC_3320::OnSetfocusedtUSER13() 
{
EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM(m_strCMAlias);
	if(pCM) 
	{
		CESL_ControlEdit *pEdit1 = (CESL_ControlEdit *)pCM->FindControl(_T("근무지주소"));

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

	}	

EFS_END
}


INT CBO_LOC_3320::InitPictureControl()
{
EFS_BEGIN
		
	INT ids;
	
	CString m_sImageFileExt;
	
	if( TRUE == m_sImageFilePath.IsEmpty() )
	{
	
		m_sImageFilePath = _T("");							   
	}
	
	if( TRUE == m_sImageFilePath.IsEmpty() )
	{
		if( NULL != m_pBmp )
		{
			delete m_pBmp;
		}

		m_pBmp = NULL;
				
		OnPaint();
		return 0;
	}
	
	CFile fFile;
	ids = fFile.Open( m_sImageFilePath , CFile::modeRead | CFile::typeBinary );
	if( 0 == ids )
	{
		if( NULL != m_pBmp )
		{
			delete m_pBmp;
		}

		m_pBmp = NULL;
		OnPaint();
		return 1;
	}
	
	fFile.Close();
	
	if( NULL == m_pBmp )
	{
		m_pBmp = new PLWinBmp;
	}
	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;
	
	cv.UnicodeToMultiByte( m_sImageFilePath.GetBuffer(0), &szTmp, &len );
	if( 0 < len )	
	{
		m_Decoder.MakeBmpFromFile (szTmp, m_pBmp);	
		delete [] szTmp;
	}	
	
	
	
	
	
	PostMessage(WM_PAINT);		
	
	return 0;
	
EFS_END	
return -1;
}

VOID CBO_LOC_3320::OnPaint() 
{
EFS_BEGIN
	CPaintDC dc(this); 

	if (!GetSafeHwnd()) 
	{
		return;
	}

	if (!GetDlgItem(IDC_STATIC_PICTURE) && !GetDlgItem(IDC_STATIC_PICTURE)->GetSafeHwnd()) 
	{
		return;
	}

	if (!GetDlgItem(IDC_STATIC_PICTURE_POS) && !GetDlgItem(IDC_STATIC_PICTURE_POS)->GetSafeHwnd()) 
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

		
		BOOL bCheckCiviNo = (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0);
		if (bCheckCiviNo)
		{
			CString strCivilNo;
			GetDataMgrData(_T("DM_BO_LOC_3300"), _T("주민등록번호"), strCivilNo, 0);
			
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
			GetDataMgrData(_T("DM_BO_LOC_3300"), _T("GPIN_SEX"), strGpinSex, 0);

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
	GetDlgItem(IDC_STATIC_PICTURE_POS)->GetWindowRect(rc);
	ClientToScreen(rc);
		
	PLFilterResizeBilinear Filter(rc.Width()-2, rc.Height()-2);
	Filter.ApplyInPlace(m_pBmp); 

	CDC* pDC = GetDlgItem(IDC_STATIC_PICTURE_POS)->GetDC();
	m_pBmp->Draw(pDC->m_hDC, 1, 1);
	ReleaseDC(pDC);

	if (GetDlgItem(IDC_picUnityUserIcon))	GetDlgItem(IDC_picUnityUserIcon)->Invalidate(FALSE);

	
	if (m_btnCertify_Y.IsWindowVisible())		m_btnCertify_Y.Invalidate(FALSE);
	if (m_btnCertify_N.IsWindowVisible())		m_btnCertify_N.Invalidate(FALSE);
	if (m_btnCertify_Fail.IsWindowVisible())	m_btnCertify_Fail.Invalidate(FALSE);

EFS_END
}

INT CBO_LOC_3320::MakePictureFileFromDBLOBField()
{
EFS_BEGIN
		
	INT ids;
	
	CString sTableName = _T("CO_LOAN_USER_TBL");		
	CString sRecKey;
	CString sFileExt;
	CString sFileName;
	
	ids = GetDataMgrData( m_strDMAlias , _T("REC_KEY") , sRecKey , 0 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("MakeRegNoFileFromDBLOBField") );
	}

	if( TRUE == sRecKey.IsEmpty() ) 
	{
		return 1;
	}
	
	ids = GetDataMgrData( m_strDMAlias , _T("사진경로") , sFileName , 0 );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("MakeRegNoFileFromDBLOBField") );
	}

	ids = GetDataMgrData( m_strDMAlias , _T("사진종류") , sFileExt , 0 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("MakeRegNoFileFromDBLOBField") );
	}

	CESL_DataMgr* pDM = FindDM( m_strDMAlias );

	if( TRUE == sFileExt.IsEmpty() )
	{
		return 0;
	}

	_tmkdir(_T("../대출증"));
	_tmkdir(_T("../대출증/이용자사진"));
	
	sFileName += sRecKey + _T(".") + sFileExt;

	ids = SetDataMgrData( m_strDMAlias , _T("사진경로") , sFileName , 0 );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("MakeRegNoFileFromDBLOBField") );
	}
	
	CFileFind ff;
	if(ff.FindFile(sFileName) )
	{		
		CString sQuery, sValue;
		sQuery.Format(_T("SELECT LENGTH(USER_PICTURE) FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), sRecKey);
		pDM->GetOneValueQuery( sQuery, sValue );		
		if( TRUE == sValue.IsEmpty() )
		{

			CString		strCivilNo;
			INT			ids , nSex , nCivilNoLength;
			ids = GetDataMgrData( _T("DM_BO_LOC_3300") , _T("주민등록번호") , strCivilNo , 0 );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("MakeRegNoFileFromDBLOBField") );
			}

			strCivilNo.TrimLeft();		strCivilNo.TrimRight();
			strCivilNo.Replace( _T("-") , _T("") );
			nCivilNoLength = strCivilNo.GetLength();

			if( 13 == nCivilNoLength )
			{
				nSex = _ttoi(strCivilNo.Mid(6,1));
				
				if( 1 == nSex || 3 == nSex || 9 == nSex || 5 == nSex || 7 == nSex )
				{
					m_sImageFilePath = _T("..\\BMP\\profile_man.JPG");
				}
				else if( 2 == nSex || 4 == nSex || 0 == nSex || 6 == nSex || 8 == nSex )
				{
					m_sImageFilePath = _T("..\\BMP\\profile_woman.JPG");
				}
				else
				{
					m_sImageFilePath = _T("..\\BMP\\profile_empty.JPG");
				}
			}
			else
			{
				m_sImageFilePath = _T("..\\BMP\\profile_empty.JPG");
			}
			
			return 1;
		}

		ff.FindNextFile();
		INT nLength = ff.GetLength();
		if( _ttoi(sValue) == nLength )
		{
			m_sImageFilePath = sFileName;
			return 0;
		}
	}
		
	BYTE *pBuffer = (BYTE *)malloc(MAX_BLOB_BUFFER);
	
	INT nSize;
	ids = pDM->LOBGet( sTableName , _T("REC_KEY") , _T("NUMERIC") , sRecKey , _T("USER_PICTURE") , pBuffer , &nSize );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("MakeRegNoFileFromDBLOBField") );
	}
	
	if( 0 >= nSize )
	{
		m_sImageFilePath = sFileName;
		free( pBuffer );
		return 0;
	}

	CFile fd;
	if( NULL == fd.Open( sFileName , CFile::modeCreate|CFile::modeWrite | CFile::typeBinary) ) 
	{
		AfxMessageBox( _T("이용자 관리 Tmp File Open Error~") );
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

VOID CBO_LOC_3320::OnbtnPICTUREMODIFY() 
{
EFS_BEGIN

	UserPictureModify();	

EFS_END
}

INT CBO_LOC_3320::UserPictureModify()
{
EFS_BEGIN

	CPreviewImage Dlg(this);

	Dlg.SetFileName(m_sImageFilePath);
	Dlg.DoModal();

	CString sFileName;
	Dlg.GetFileName(sFileName);

	if( 0 == Dlg.GetCloseMode() ) 
	{
		return 0;
	}

	m_sImageFilePath = sFileName;

	InitPictureControl();

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3320::SetDefaultMail()
{
EFS_BEGIN

	CString defaultValue, query;
	query.Format( _T("SELECT C.DESCRIPTION FROM CD_CODE_TBL C, MN_USER_CODE_TBL U, MN_USER_TBL MU ")
				  _T("WHERE C.CLASS=50 AND U.CODE_KEY=C.REC_KEY AND U.USER_KEY=MU.REC_KEY AND MU.ID='%s' ")
				  _T("AND U.DEFAULT_VALUE='Y'"), GetUserID() );
	
	CESL_DataMgr *pDM = FindDM(m_strDMAlias);
	pDM->GetOneValueQuery( query, defaultValue );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSER6 );
	INT initSel = pCombo->GetCurSel();
	for( INT i = 0; i < pCombo->GetCount(); i++ )
	{
		pCombo->SetCurSel( i );
		pCombo->GetWindowText( query );

		if( query == defaultValue )
		{
			return;
		}
	}
	
	pCombo->SetCurSel( initSel );
	return;

EFS_END
}

VOID CBO_LOC_3320::ShowMONETABtn()
{
EFS_BEGIN
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSER5 );
	CButton *pBtn = NULL;

	const INT cnt = 2;
	UINT id[ cnt ] = { IDC_cMONETA, IDC_bMONETA };
	bool flag[ cnt ] = { false, false };

	if( m_bUseMonetaService && 1 == pCombo->GetCurSel() )
	{
		switch( m_nMode )
		{
			case 0 :	
				flag[ 0 ] = true;
				break;
			case 1 :	
			case 3 :	
				flag[ 1 ] = true;
				break;
			default :
				break;
		}
	}

	INT		i;
	for( i=0 ; i<cnt ; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		pBtn->ShowWindow( flag[ i ] );
	}

EFS_END
}

VOID CBO_LOC_3320::OnSelchangecmbUSER5() 
{
EFS_BEGIN
	
	ShowMONETABtn();

	
	
	if ( m_nMode != 0 ) return;
	int ids = -1;
	CString strPhone1 = _T("");	
	ids = GetControlData( m_strCMAlias, _T("휴대폰1"), strPhone1 );
	if ( strPhone1 == _T("적용안함")) {
		ids = SetControlData( m_strCMAlias , _T("휴대폰2") , _T("") );
		ids = SetControlData( m_strCMAlias , _T("휴대폰3") , _T("") );		
	}

EFS_END
}

VOID CBO_LOC_3320::OnbMONETA() 
{
EFS_BEGIN
	
	CString informDate;
	CESL_DataMgr *pDM = FindDM( m_strDMAlias );
	
	
	CString strGetData;
	CTime time = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	informDate = strGetData.Left(10);


	
	m_pSpfWork->UploadMONETAInfo( 
								informDate,
								pDM->GetCellData( _T("휴대폰"), 0 ),
								pDM->GetCellData( _T("주민등록번호"), 0 ),
								m_pSpfWork->GetLibCode() );				

	
	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData( _T("MONETA_INFORM_DATE"),      _T("STRING"), informDate );
	pDM->AddDBFieldData( _T("MONETA_INFORM_PHONE_NUM"), _T("STRING"), pDM->GetCellData( _T("휴대폰"), 0 ) );
	pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL"), _T("REC_KEY") , _T("NUMERIC") , pDM->GetCellData( _T("REC_KEY"), 0 ) );
	pDM->SendFrame();
	pDM->EndFrame();

	AfxMessageBox( _T("MONETA Service에 등록되었습니다.") );

EFS_END
}

VOID CBO_LOC_3320::ShowCTUsrBtn()
{
EFS_BEGIN

	CButton *pBtn = ( CButton * )GetDlgItem( IDC_btnINPUTCENTER );

	INT nManageValue = m_pInfo->GetLoanUserShareMode();

		if( ( 0 == m_nMode ) && (( 1 == nManageValue ) || ( 2 == nManageValue )) )
		{
			pBtn->ShowWindow(SW_SHOW);
		}
		else
		{
			pBtn->ShowWindow(SW_HIDE);
		}


EFS_END
}

VOID CBO_LOC_3320::SetCenterUserInfo( CString data[] )
{
EFS_BEGIN

	const INT cnt = 28;
	CString ctrlAlias[ cnt ] = {
		_T(""),
		_T("대출자번호"),
		_T("사용자ID"),
		_T(""),
		_T("이름"),
		_T("생일"),
		_T(""),
		_T("주민등록번호"),
		_T("자택주소"),
		_T("근무지주소"),
		_T("자택우편번호"),
		_T("근무지우편번호"),
		_T("자택전화"),
		_T("근무지전화"),
		_T("근무지내선번호"),
		_T("부서명"),
		_T("직급명"),
		_T(""),
		_T(""),
		_T(""),
		_T(""),
		_T("학교명"),
		_T("학년"),
		_T("반"),
		_T("교번/순번/군번"),
		_T("우편발송지역"),
		_T("비고"),
		_T("자료실") };

	INT		i;
	for( i=0 ; i<cnt ; i++ )
	{
		if( TRUE != ctrlAlias[ i ].IsEmpty() )
		{
			SetControlData( m_strCMAlias , ctrlAlias[ i ] , data[ i ] );
		}
	}

	SetControlData( m_strCMAlias, _T("비밀번호"), data[ 3 ] );
	SetControlData( m_strCMAlias, _T("비밀번호확인"), data[ 3 ] );

	CComboBox *pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSER8 );
	if( _T("+") == data[ 6 ] )
	{
		pCombo->SetCurSel( 0 );
	}
	else
	{
		pCombo->SetCurSel( 1 );
	}

	CString phoneN_1, phoneN_2, phoneN_3;
	data[ 17 ].Replace( _T("-"), _T("") );
	INT		phoneNumLength = data[ 17 ].GetLength();
	if( 10 == phoneNumLength || 11 == phoneNumLength )
	{
		phoneN_1 = data[ 17 ].Left( 3 );
		phoneN_3 = data[ 17 ].Right( 4 );

		if( 10 == phoneNumLength )
		{
			phoneN_2 = data[ 17 ].Mid( 4, 3 );
		}
		else
		{
			phoneN_2 = data[ 17 ].Mid( 4, 4 );
		}
	}
	
	
	CLocCommonAPI::SetPhoneNumCel( GetDlgItem( IDC_cmbUSER5 ), phoneN_1 );

	SetControlData( m_strCMAlias, _T("휴대폰2"), phoneN_2 );
	SetControlData( m_strCMAlias, _T("휴대폰3"), phoneN_3 );


EFS_END
}

LRESULT CBO_LOC_3320::OnUserPicImport( WPARAM wParam, LPARAM lParam)
{
EFS_BEGIN

	m_sImageFilePath = *((CString*)wParam);

	InitPictureControl();

	return 0;

EFS_END
return -1;
}



VOID CBO_LOC_3320::ViewHandPhoneNum( CString phoneNum )
{
EFS_BEGIN

	CStringArray asPhoneNum;
	CLocCommonAPI::MakeArrayList( phoneNum , _T("-") , asPhoneNum );

	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cmbUSER5 );
	
	CString sPhoneNum3;
	INT		i;
	for( i=0 ; i<asPhoneNum.GetSize() ; i++ )
	{
		if( 0 == i )
		{
			
			CLocCommonAPI::SetPhoneNumCel( pCombo, phoneNum );
			
		}
		else if( 1 == i )
		{
			SetControlData( m_strCMAlias, _T("휴대폰2"),  asPhoneNum.GetAt(i) );
		}

		if( 1 < i )
		{
			if( TRUE != sPhoneNum3.IsEmpty() ) 
			{
				sPhoneNum3 += _T("-");
			}

			sPhoneNum3 += asPhoneNum.GetAt(i);
		}

		if( 1 < i && i == asPhoneNum.GetSize()-1 )
		{
			SetControlData( m_strCMAlias, _T("휴대폰3"),  sPhoneNum3 );
		}
	}

EFS_END
}

VOID CBO_LOC_3320::OnSetfocuscmbUSER6() 
{
EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;
	
    hIME = ::ImmGetContext(GetDlgItem(IDC_edtUSER21)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_ALPHANUMERIC;
		
		ImmSetConversionStatus(hIME,dwConversion,dwSentence );
		
        ImmReleaseContext(GetDlgItem(IDC_edtUSER21)->m_hWnd,hIME); 
    } 

EFS_END
}

INT CBO_LOC_3320::SetFamilyKey()
{
EFS_BEGIN
	
	CString familyID, familyKey;
	GetDlgItemText( IDC_edtUSER28, familyID );

	familyID.TrimLeft();
	familyID.TrimRight();

	if( TRUE == familyID.IsEmpty() )
	{
		SetControlData( m_strCMAlias , _T("가족KEY") , familyKey );
		return 0;
	}

	CString query;
	query.Format( _T("SELECT REC_KEY FROM CO_LOAN_FAMILY_ID_TBL WHERE ID = '%s'"), familyID );
	FindDM( m_strDMAlias )->GetOneValueQuery( query, familyKey );
	
	if( TRUE == familyKey.IsEmpty() )
	{
		return -1;
	}
	else
	{
		SetControlData( m_strCMAlias , _T("가족KEY") , familyKey );
	}

	return 0;

EFS_END
return -1;
}

VOID CBO_LOC_3320::TakeManageValue()
{
EFS_BEGIN

	
	
	CString result;
	m_pLoanMangeValue->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출자번호자동부여여부"), result );
	result.TrimLeft( );		result.TrimRight( );
	if( TRUE == result.IsEmpty() ) 
	{
		result = _T("N");
	}

	if( _T("U") == result )
	{
		m_nUserNoAutoMode = 3;
	}
	else if( _T("A") == result )
	{
		m_nUserNoAutoMode = 1;
	}
	else if( _T("Y") == result )
	{
		m_nUserNoAutoMode = 2;
	}
	else if( _T("N") == result )
	{
		m_nUserNoAutoMode = 0;
	}
	else
	{
		m_nUserNoAutoMode = 0;
	}

	
	
	INT ids = m_pInfo->GetSMSUseMode();
	if(0 == ids)
	{
		m_sSMSVersion = _T("N");
	}
	else if(1 == ids)
	{
		m_sSMSVersion = _T("Y");
	}
	else
	{
		m_sSMSVersion = _T("N");
	}
	m_pSpfWork->m_nAutoMode = m_nUserNoAutoMode;
	m_pSpfWork->m_sSMSInfo = m_sSMSVersion;
	m_pSpfWork->m_bSetedManageValue[ 0 ] = TRUE;
	m_pSpfWork->m_bSetedManageValue[ 1 ] = TRUE;
	m_pSpfWork->m_sIsUnityLoanService = m_pLoanMangeValue->m_sIsUnityLoanService;
	
	
	m_pLoanMangeValue->GetManageValue_Name( _T("기타"), _T("공통"), _T("MONETA사용여부"), result );
	result.TrimLeft();	result.TrimRight();
	if( _T("Y") == result )
	{
		m_bUseMonetaService = TRUE;
	}
	else
	{
		m_bUseMonetaService = FALSE;
	}
	
	
	m_pSpfWork->m_strIsAutoSetUsernoToUserid = m_pLoanMangeValue->m_strIsAutoSetUsernoToUserid;
	
	m_pSpfWork->m_strIsAutoSetCivilnoToPassword = m_pLoanMangeValue->m_strIsAutoSetCivilnoToPassword;

EFS_END
}

INT CBO_LOC_3320::CheckUserDuplicate( INT flag[] )
{
EFS_BEGIN

	const INT nCnt = 3;
	
	CString sCurrentUserInfo[ nCnt ];
	
	GetDlgItemText( IDC_edtUSER8, sCurrentUserInfo[ 0 ] );
	
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		if(0==m_nMode)
		{
			this->ESL_DES_Encryption(sCurrentUserInfo[0],sCurrentUserInfo[0]);
		}
		else
		{
			CString strCivilNo;
			int ids = GetDataMgrData( m_strDMAlias , _T("주민등록번호") , strCivilNo , 0 );
			if(strCivilNo==sCurrentUserInfo[0])
			{
				CString strTemp;
				ids = GetDataMgrData( m_strDMAlias , _T("주민등록번호_CRYPTO") , strTemp , 0 );
				
				if(!strTemp.IsEmpty())
				{
					sCurrentUserInfo[0] = strTemp;
				}
			}
			else
			{
				this->ESL_DES_Encryption(sCurrentUserInfo[0],sCurrentUserInfo[0]);
			}
		}
	}
	
	GetDlgItemText( IDC_edtUSER1, sCurrentUserInfo[ 1 ] );
	
	GetDlgItemText( IDC_edtUSER4, sCurrentUserInfo[ 2 ] );
	
	if( 1 == m_nUserNoAutoMode && 0 == m_nMode && 0 == sCurrentUserInfo[1].Compare( _T("[주민번호로생성]") ) )
	{
		sCurrentUserInfo[1] = sCurrentUserInfo[0];
		sCurrentUserInfo[1].Replace(_T("-"),_T(""));
	}
	
	if( 3 == m_nUserNoAutoMode && 0 == m_nMode && 0 == sCurrentUserInfo[1].Compare( _T("[사용자ID로생성]") ) )
	{
		sCurrentUserInfo[1] = sCurrentUserInfo[2];		
	}
	
 	if(_T("Y") == m_pLoanMangeValue->m_strIsAutoSetUsernoToUserid)
 	{
		if(3 != m_nUserNoAutoMode && 0 == m_nMode)
		{
 			sCurrentUserInfo[2] = sCurrentUserInfo[1];
		}
 	}	
	
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		sCurrentUserInfo[ i ].TrimLeft();
		sCurrentUserInfo[ i ].TrimRight();
		flag[ i ] = 0;

		if( TRUE == sCurrentUserInfo[ i ].IsEmpty() && 0!=i )
		{
			flag[ i ] = 1;
		}
	}
	
	CString sPastUserInfo[ nCnt ];
	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO( FindDM( m_strDMAlias )->CONNECTION_INFO );

	CString alias[ nCnt ] = { _T("주민등록번호"), _T("대출자번호"), _T("사용자ID") };
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		alias[0]=_T("주민등록번호_CRYPTO");
	}

	if( m_nMode )
	{
		FindDM( m_strDMAlias )->GetCellData( alias, nCnt, sPastUserInfo, 0 );
	}

	CString sQuery;
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		sQuery.Format( 	_T("SELECT '주민등록번호', USER_NO, USER_ID, REC_KEY FROM CO_LOAN_USER_TBL WHERE ") );
	}
	else
	{
		sQuery.Format( 	_T("SELECT CIVIL_NO, USER_NO, USER_ID, REC_KEY FROM CO_LOAN_USER_TBL WHERE ") );
	}

	sQuery += " (USER_CLASS!='3' OR MEMBER_CLASS!='1') AND (";
	INT nWCnt=0;

	CString strCivil1,strCivil2,strUserID1;
	switch( m_nMode )
	{
		case 0 :
			if( 1 == ((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2))->GetCheck() )
			{
				
				if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
				{
					sQuery += _T("CRYPTO_CIVIL_NO = '") + sCurrentUserInfo[0] +_T("' ");
				}
				else
				{
					sQuery += _T("CIVIL_NO IN ('") + MakeCivilNumFormat(sCurrentUserInfo[0]) + _T("','")+MakeCivilNumIndex(sCurrentUserInfo[0])+_T("') ");
				}

				nWCnt++;
			}
			if( TRUE != sCurrentUserInfo[1].IsEmpty() )
			{
				if( nWCnt > 0 ) sQuery += _T(" OR ");
				sQuery += _T("USER_NO = '") + sCurrentUserInfo[1] + _T("'");
				nWCnt++;
			}
			if( TRUE != sCurrentUserInfo[2].IsEmpty() )
			{
				if( nWCnt > 0 ) sQuery += _T(" OR ");
				sQuery += _T("USER_ID = '") + sCurrentUserInfo[2] + _T("'");
			}

			break;
		case 1 :
		case 3 :
			{
				CString sUserRecKey;
				FindDM( m_strDMAlias )->GetCellData(_T("REC_KEY"), 0, sUserRecKey);


				if( 1 == ((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2))->GetCheck() )
				{
;
					CString sWhere2 , strPastUserCivil1 , strPastUserCivil2;
 					strCivil1 = MakeCivilNumFormat(sCurrentUserInfo[0]);
 					strCivil2 = MakeCivilNumIndex(sCurrentUserInfo[0]);
 					strPastUserCivil1 = MakeCivilNumFormat(sPastUserInfo[0]);
 					strPastUserCivil2 = MakeCivilNumIndex(sPastUserInfo[0]);

					if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
					{
						sWhere2.Format( _T("( CRYPTO_CIVIL_NO = '%s' AND REC_KEY NOT IN (%s) )") 
										, sCurrentUserInfo[0] , sUserRecKey);
					}
					else
					{
						sWhere2.Format( _T("( CIVIL_NO IN ( '%s', '%s' ) AND REC_KEY NOT IN (%s) )") 
										, strCivil1 , strCivil2 , sUserRecKey);
					}
					sQuery += sWhere2;
					nWCnt++;
				}

				if( TRUE != sCurrentUserInfo[1].IsEmpty() )
				{
					if( 0 < nWCnt ) 
					{
						sQuery += _T(" OR ");
					}

					CString sWhere2;
					sWhere2.Format( _T("( USER_NO = '%s' ) AND REC_KEY NOT IN(%s)") ,
									sCurrentUserInfo[ 1 ], sUserRecKey );
					sQuery += sWhere2;
					nWCnt++;
				}
				if( TRUE != sCurrentUserInfo[2].IsEmpty() )
				{
					if( 0 < nWCnt ) 
					{
						sQuery += _T(" OR ");
					}

					CString sWhere2;
					sWhere2.Format( _T("( USER_ID = '%s' AND REC_KEY NOT IN (%s) )"), sCurrentUserInfo[2], sUserRecKey);
					sQuery += sWhere2;
				}
			}

			break;
		default :
			return -1;
			break;
	}

	sQuery += _T(")");

	DM.RestructDataManager(sQuery);
	
	if(TRUE == m_pInfo->m_bLibSecuUse)
	{
		CString strGetData;
		CStringArray asDes1;
		for(INT i = 0; i < DM.GetRowCount(); i++)
		{
			DM.GetCellData(i, 0, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
				for(i = 0; i < DM.GetRowCount(); i++)
				{
					DM.SetCellData(i, 0, asDes1.GetAt(i));
				}			
				p3000Api->CloseSocket();	
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	CString sBuf;
	for( i = 0; i < DM.GetRowCount(); i++ )
		for( INT j = 0; j < nCnt; j++ )
		{
			sBuf = DM.GetCellData( i, j );
			switch( m_nMode )
			{
				case 0 :
					if( j )
					{
						if( TRUE != sBuf.IsEmpty() && sBuf == sCurrentUserInfo[j] )
						{
							flag[ j ] = 2;					
						}
					}
					else
					{
						
						if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
						{
							if(TRUE != sBuf.IsEmpty() && sBuf == sCurrentUserInfo[0])
							{
								flag[ j ] = 2;
							}
						}
						else
						{
							if(TRUE != sBuf.IsEmpty() && sBuf == MakeCivilNumFormat(sCurrentUserInfo[0]) || sBuf == MakeCivilNumIndex(sCurrentUserInfo[0]))
							{
								flag[ j ] = 2;
							}
						}
					}
					break;
				case 1 :
				case 3 :
					if( j )
					{
						if(		TRUE != sBuf.IsEmpty() 
							&&	sBuf != sPastUserInfo[ j ] 
							&&	sBuf == sCurrentUserInfo[ j ] )
						{
							flag[ j ] = 2;
						}
					}
					else
					{
						
						if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
						{
							if(		TRUE != sBuf.IsEmpty() 
								&&	( sBuf == sCurrentUserInfo[0] ) 
								&&	( sBuf != sPastUserInfo[0]) )
							{
								flag[ j ] = 2;
							}
						}
						else
						{
							if(		TRUE != sBuf.IsEmpty() 
								&&	( sBuf == MakeCivilNumFormat( sCurrentUserInfo[ 0 ] ) || sBuf == MakeCivilNumIndex( sCurrentUserInfo[ 0 ] ) ) 
								&&	( sBuf != MakeCivilNumFormat( sPastUserInfo[ 0 ] ) || sBuf != MakeCivilNumIndex( sPastUserInfo[ 0 ] ) ) )
							{
								flag[ j ] = 2;
							}
						}
					}
					break;
				default :
					break;
			}
		}    
	
    CDUP_USER_DLG dlg(this) ;
	if( 0 < DM.GetRowCount() )
	{
		dlg.m_sIsUnityLoanService = m_pLoanMangeValue->m_sIsUnityLoanService;
		if(NULL != FindSM(_T("BO_LOC_3100")))
		{
			dlg.m_bDupUserView = FALSE;
		}
		dlg.SetDataManager (&DM) ;
		if( IDOK == dlg.DoModal() )
		{
			
			CStringArray aUserKey;
			CString strUserKey;
			const int ROW_COUNT = DM.GetRowCount();
			int nRowCnt = 0;
			for(nRowCnt=0;nRowCnt<ROW_COUNT;nRowCnt++)
			{
				DM.GetCellData(nRowCnt, 3, strUserKey);
				if(_T("") != strUserKey)
				{
					aUserKey.Add(strUserKey);
				}
			}

			m_pSpfWork->SearchAddLoanUser(aUserKey);
			CESL_Grid *pGrid = GetMainGridPtr();
			if( NULL == pGrid ) 
			{
				return -1;
			} 

			nRowCnt = pGrid->GetRows();
			for( i=0 ; i<nRowCnt-1 ; i++ )
			{
				pGrid->SetAt( i , 0 , _T("") );
			}
			CBO_LOC_3320 Dlg( m_pParent, m_pLoanShareManager, m_pLoanMangeValue );
			Dlg.SetMode( 1 , TRUE );
			if( Dlg.CheckParentDMCount() < 1 )
			{
				return 0;
			}

			Dlg.DoModal();

			return 100;
		}
		
		else
		{
			return 100;
		}
	}
		
	CString passWd[ 2 ];
	GetDlgItemText( IDC_edtUSER5, passWd[ 0 ] );
	GetDlgItemText( IDC_edtUSER6, passWd[ 1 ] );
	
	if( TRUE == sCurrentUserInfo[ 2 ].IsEmpty() )
	{
		if( TRUE == passWd[ 0 ].IsEmpty() && TRUE == passWd[ 1 ].IsEmpty() )
		{
			flag[ 2 ] = 0;
		}
	}
		
	return 0;

EFS_END
return -1;
}

CString CBO_LOC_3320::MakeCivilNumFormat( CString civilNum )
{
EFS_BEGIN

	civilNum = MakeCivilNumIndex( civilNum );
	INT		nLength = civilNum.GetLength();
	if( 13 != nLength )
	{
		return civilNum;
	}

	return civilNum.Left( 6 ) + _T("-") + civilNum.Right( 7 );

EFS_END
return _T("");
}

CString CBO_LOC_3320::MakeCivilNumIndex( CString civilNum )
{
EFS_BEGIN
	
	INT nLength = civilNum.GetLength();

	CString sIndex;
	TCHAR cNum;
	INT		i;
	for( i=0 ; i<nLength ; i++ )
	{
		cNum = civilNum.GetAt( i );

		if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
		{
			if( (47 < cNum && 58 > cNum) || 42==cNum )
			{
				sIndex += cNum;
			}
		}
		else
		{
			if( 47 < cNum && 58 > cNum )
			{
				sIndex += cNum;
			}
		}
	}

	return sIndex;

EFS_END
return _T("");
}

INT CBO_LOC_3320::CheckBirthday()
{
EFS_BEGIN
	
	CString birthDay;
	GetControlData( m_strCMAlias, _T("생일"), birthDay );
	if( TRUE == birthDay.IsEmpty() )
	{
		return 0;
	}

	INT y, m, d;
	_stscanf( birthDay.GetBuffer( 0 ), _T("%d/%d/%d"), &y, &m, &d );
	
	return IsTrueDate( y, m, d );

EFS_END
return -1;
}

INT CBO_LOC_3320::IsTrueDate( INT y, INT m, INT d )
{
EFS_BEGIN
	
	if( 100 > y || 4712 < y )
	{
		return -1;
	}
	
	if( 1 > m || 12 < m )
	{
		return -2;
	}

	INT days[ 12 ] = { 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 };
	if( ( ( 0 == y % 4 ) && ( 0 != y % 100 ) ) || ( 0 == y % 400 ) )
	{
		days[ 1 ] = 29;
	}

	if( d > days[ m - 1 ] )
	{
		return -3;
	}
	
	return 0;

EFS_END
return -1;
}

BOOL CBO_LOC_3320::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	m_ToolTip.RelayEvent(pMsg);

	if( ( WM_KEYDOWN == pMsg->message ) && ( VK_ESCAPE  == pMsg->wParam ) )
	{
		OnbtnCLOSE();
		return TRUE;
	}

	return CESL_Mgr::PreTranslateMessage(pMsg);

EFS_END
return FALSE;
}

VOID CBO_LOC_3320::CreateMyTooltip ()
{
EFS_BEGIN
	CString sHelp = _T("도움말 : 회원증 비밀번호 항목은 무인대출기를 \n사용할 때 필요한 항목입니다.\n")
					_T("이용자가 무인 대출기 사용시 회원증의 바코드와 \n회원증 비밀번호로 인증을 합니다.\n")
					_T("(암호는 4자리 이상, 20자리 이하의 숫자로 입력하여 주십시요.)");

	m_ToolTip.Create(this);

	m_ToolTip.SetMaxTipWidth(400);
	m_ToolTip.AddTool( GetDlgItem(IDC_edtUSER_35) , sHelp );
	m_ToolTip.Activate(TRUE);

	m_ToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_INITIAL,MAKELONG(0,0));
	m_ToolTip.SendMessage(TTM_SETDELAYTIME,(WPARAM)TTDT_AUTOPOP,MAKELONG(20000,0));

EFS_END
} 

VOID CBO_LOC_3320::Onbtn3320MNGGROUP() 
{
EFS_BEGIN

	CString sUserKey;
	INT ids = GetDataMgrData( m_strDMAlias , _T("REC_KEY") , sUserKey , 0 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010 , _T("Onbtn3320MNGGROUP") );
	}

	CMNG_USER_GROUP_CODE Dlg(this,sUserKey);
	Dlg.DoModal();

EFS_END
}

VOID CBO_LOC_3320::OnbtnOLDDATA3() 
{
EFS_BEGIN

	INT ids = -1; 

	
	if(m_strInsertRecKey.IsEmpty())
	{
		return;
	}
	
	ids = SearchAllUserInfo(m_strInsertRecKey);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010 , _T("OnbtnOLDDATA3") );
	}
	
	ids = ViewScreenLinkDM();
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1020 , _T("OnbtnOLDDATA3") );
	}	
	
	ids = SetSelect();
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_VOID2( -1030 , _T("OnbtnOLDDATA3") );
	}
	
	CString sDMAlias = m_strDMAlias;
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	pDM->FreeData();

	INT nAutoMode;
	ids = m_pSpfWork->IsAutoMode( nAutoMode );
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_VOID2( -1040 , _T("SetScreen") ); 
	}

	switch( nAutoMode )
	{
		case 1 :
			ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[주민번호로생성]") );
			break;
		case 2 :
			ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[자동생성]") );
			break;
		case 3 :
			ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[사용자ID로생성]") );
			break;
		case 0 :
		default :
			break;
	}

	if( 0 > ids )
	{
		ERROR_MSG_RETURN_VOID2( -1050 , _T("SetScreen") );
	}

EFS_END	
}

VOID CBO_LOC_3320::OnbtnLOANINFO()
{
EFS_BEGIN

	CESL_DataMgr	*pDM;
	CString			strRecKey;
	INT				ids;

	pDM = FindDM( m_strDMAlias );

	ids = pDM->GetCellData( _T("REC_KEY") , 0 , strRecKey );
	CBO_LOC_LOAN_HIS HIS(this, m_pLoanShareManager, m_pLoanMangeValue);
	HIS.SetUserKey(strRecKey);
	HIS.DoModal();
	
EFS_END	
}

VOID CBO_LOC_3320::OnSetfocusedtUSER22() 
{
EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM(m_strCMAlias);
	if (pCM) 
	{
		CESL_ControlEdit *pEdit1 = (CESL_ControlEdit *)pCM->FindControl(_T("비고"));
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

	}	

EFS_END
}

VOID CBO_LOC_3320::OnSetfocusedtUSER23() 
{
EFS_BEGIN

	CESL_ControlMgr *pCM = FindCM(m_strCMAlias);
	if (pCM) 
	{
		CESL_ControlEdit *pEdit1 = (CESL_ControlEdit *)pCM->FindControl(_T("대출정지회원사유"));
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

	}	

EFS_END
}

VOID CBO_LOC_3320::OnbtnSERIALNO()
{
	INT ids;	
	
	CString strValue = _T("");
	ids = GetControlData( m_strCMAlias , _T("이용자번호구분") , strValue );
	if(0>ids) ERROR_MSG_RETURN_VOID2( -1010 , _T("OnbtnSERIALNO") );
	
	if( TRUE == strValue.IsEmpty() )
	{
		AfxMessageBox( _T("선택된 일련번호가 없습니다.") );
		return;
	}

	
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		CString strData;
		CESL_ControlMultiComboBox* pCombo = (CESL_ControlMultiComboBox*)FindControl( m_strCMAlias , _T("이용자번호구분"));
		INT nIdx = pCombo->GetCurSel();
		pCombo->GetData(strData, nIdx,1);
		if(_T("타도서관이용자번호구분")==strData)
		{
			INT nCount = 512;
			INT nDefaultRow = 0;
			CString strCode[512], strDesc[512];
			m_pInfo->pCodeMgr->GetCode(_T("이용자번호구분"), strCode, strDesc, nCount, nDefaultRow);
			strValue = strCode[nDefaultRow];
			
			if(TRUE == m_pInfo->m_bCertifyUse)
			{
				CString strGetData;
				CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
				CString strYear = strGetData.Left(4);
				strValue = m_pInfo->LIB_CODE + strYear.Right(2);
			}
		}
	}

	CUsernoRenumberDlg dlg(m_pSpfWork,this);
	dlg.SetKLMemberYN(m_bIsKLMemberYN);
	dlg.SetUserNoClass(strValue);
	dlg.SetMode(0);
	if(IDOK==dlg.DoModal())
	{
		CESL_Grid *pGrid = GetMainGridPtr();
		if(NULL==pGrid) return;

		CString strUserNo;
		GetDataMgrData(_T("DM_BO_LOC_3300"), _T("대출자번호"), strUserNo, 0);
		SetDataMgrData(_T("DM_BO_LOC_3300_LIST"), _T("대출자번호"), strUserNo, m_nCurrentIndex);
		SetDataMgrData(_T("DM_BO_LOC_3300_LIST"), _T("대출자번호_VIEW"), strUserNo, m_nCurrentIndex);
		pGrid->SetProtectSelectColumn(TRUE);
		pGrid->DisplayLine(m_nCurrentIndex);
		pGrid->SetProtectSelectColumn(FALSE);
	}
	return;
}

VOID CBO_LOC_3320::OnTimer(UINT nIDEvent) 
{
	if( 10 == nIDEvent )
	{
		
		KillTimer(10);
		CivilNoCheck();
		m_bViewCivilNoCheckMsg = TRUE;
		return;
	}
	else if(1 == nIDEvent)
	{
		KillTimer(1);
		EndDialog(IDCANCEL);
		return;
	}

	CDialog::OnTimer(nIDEvent);
}

VOID CBO_LOC_3320::CivilNoCheck()
{
EFS_BEGIN

	
	if ( m_bCivilNoAutoCheck )
	{
		INT ids;
		bool IsDuplicate = FALSE;

		
		CString civilNum;
		GetDlgItemText( IDC_edtUSER8, civilNum );
		SetDlgItemText( IDC_edtUSER8, MakeCivilNumFormat( civilNum ) );

		ids = IsDuplicateCivilNo( IsDuplicate );	
		if(0>ids) return;
		if(0<ids) return;

		m_bViewCivilNoCheckMsg = TRUE;
		if( IsDuplicate )
		{			
			if( IDYES == AfxMessageBox( _T("중복된 주민등록번호입니다.\r\n"
										_T("중복된 이용자를 확인하시겠습니까?")), MB_ICONSTOP | MB_YESNO) )
			{
				CString strCivilNo;

				( (CEdit*) GetDlgItem(IDC_edtUSER8) )->GetWindowText(strCivilNo);
				
				m_pSpfWork->SearchAddLoanUser(_T(""), strCivilNo);
				CBO_LOC_3320 Dlg( m_pParent, m_pLoanShareManager, m_pLoanMangeValue );
				Dlg.SetMode(1,TRUE);
				if(1>Dlg.CheckParentDMCount() ) return ;
				
				Dlg.DoModal();
				GetDlgItem(IDC_EDIT_CIVILNUM1)->SetFocus();
			}
		}
	}


EFS_END
}

VOID CBO_LOC_3320::OnKillfocusedtUSER1() 
{
	if ( 1 == m_nUserNoAutoMode && ( 1 == m_nMode || 3 == m_nMode ) )
	{

		GetDlgItem(IDC_EDIT_CIVILNUM1)->SetFocus();
	}	
}

VOID CBO_LOC_3320::OnSelchangecmbUSER9() 
{
	INT ids = 0;

	
	if ( 0 == m_nMode )	
	{
		switch( m_nUserNoAutoMode )
		{
			case 1 :
				ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[주민번호로생성]") );
				break;
			case 2 :
				ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[자동생성]") );
				break;
			case 3 :
				ids = SetControlData( m_strCMAlias , _T("대출자번호") , _T("[사용자ID로생성]") );
				break;
			case 0 :
			default :
				break;
		}
	}
	
	else if ( 1 == m_nMode || 3 == m_nMode )
	{
		
		CString sCMAlias = m_strCMAlias;
		CESL_ControlMgr *pChildCM = FindCM( sCMAlias );
		if( NULL == pChildCM ) 
		{
			ERROR_MSG_RETURN_VOID2( -1010 , _T("OnSelchangecmbUSER9") );
		}
	
		ids = pChildCM->ControlDisplay( _T("대출자번호") , 0);		
	}

	if( 0 > ids )
	{
		ERROR_MSG_RETURN_VOID2( -2, _T("OnSelchangecmbUSER9") );
	}
}

VOID CBO_LOC_3320::OnchkSMSUSEYN()
{
EFS_BEGIN

	
	if(0==m_nMode)
	{
		BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkSMSUSEYN))->GetCheck();
		FILE *fp = _tfopen( _T("..\\cfg\\SMS수신여부설정.cfg"), _T("w+b") );
		
		fputc(0xFF, fp);
		fputc(0xFE, fp);
		_ftprintf( fp, _T("%d"), bCheck );
		fclose( fp );
	}

EFS_END
}

VOID CBO_LOC_3320::OnchkMAILUSEYN() 
{	
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_chkMAILUSEYN))->GetCheck();
	
	if(0==m_nMode)
	{
		FILE *fp = _tfopen( _T("..\\cfg\\Mailing수신여부설정.cfg"), _T("w+b") );
		
		fputc(0xFF, fp);
		fputc(0xFE, fp);
		_ftprintf( fp, _T("%d"), bCheck );
		fclose( fp );
	}

	if(TRUE==bCheck)
	{
		CString strMail;
		((CEdit*)GetDlgItem(IDC_edtUSER21))->GetWindowText(strMail);
		strMail.TrimLeft();	strMail.TrimRight();
		strMail.Replace(_T(" "), _T(""));
		if(strMail.IsEmpty())
		{
			((CEdit*)GetDlgItem(IDC_edtUSER21))->SetFocus();
		}
	}
}

INT CBO_LOC_3320::IsSecede(BOOL &bSecede)
{
EFS_BEGIN
	INT		ids;
	CString strQuery, strResult;
	CString strRecKey;
	bSecede = TRUE;

	ids = GetRecKey( strRecKey );
	if( 0 > ids ) 
	{
		return -1;
	}
	
	CESL_DataMgr* pDM = FindDM( m_strDMAlias );
	if( !pDM ) 
	{
		return -2;
	}

	strQuery.Format(	_T("SELECT 'X' FROM DUAL WHERE ")
						_T(" EXISTS(SELECT 'X' FROM LS_WORK_T01 WHERE ")
						_T(" STATUS IN ('0','2','3') AND USER_KEY=%s)"), strRecKey );
	ids = pDM->GetOneValueQuery( strQuery, strResult );
	if( 0 > ids && -4007 != ids ) 
	{
		return -3;
	}
	
	if( -4007 != ids )
	{
		bSecede = FALSE;
		return 0;
	}

	if(_T("Y") == m_strUseILL)
	{
		BOOL bILLUser = TRUE;
		ids = IsILLUser(bILLUser);
		if( 0 > ids ) 
		{
			return ids;
		}
		
		if( TRUE == bILLUser ) 
		{
			bSecede = FALSE;
		}
	}
	

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::IsReEntry(BOOL &bReEntry)
{
EFS_BEGIN
	
	if( _T("N") == m_bCivilNoDisplayCheck )
	{
		return 0;
	}

	INT		ids;
	CString strCivilNo;
	CString strQuery;
	CString strResult;
	
	bReEntry = FALSE;
	
	ids = GetControlData( m_strCMAlias , _T("주민등록번호") , strCivilNo );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("IsReEntry") );
	}
	
	if( TRUE == strCivilNo.IsEmpty() )
	{				
		return 0;
	}

	CESL_DataMgr* pDM = FindDM( m_strDMAlias );
	if( !pDM ) 
	{
		return -2;
	}
	if(_T("Y")==m_pLoanMangeValue->m_sIsUnityLoanService)
	{
		this->ESL_DES_Encryption(strCivilNo,strCivilNo);
		strQuery.Format(	_T("SELECT 'X' FROM DUAL WHERE ")
							_T(" EXISTS(SELECT 'X' FROM CO_LOAN_USER_TBL WHERE ")
							_T(" CRYPTO_CIVIL_NO = '%s' AND USER_CLASS='3' AND USER_NO IS NULL)") , strCivilNo );
	}
	else
	{
		strQuery.Format(	_T("SELECT 'X' FROM DUAL WHERE ")
							_T(" EXISTS(SELECT 'X' FROM CO_LOAN_USER_TBL WHERE ")
							_T(" CIVIL_NO='%s' AND USER_CLASS='3' AND USER_NO IS NULL)") , strCivilNo );
	}

	ids = pDM->GetOneValueQuery( strQuery, strResult );
	if( 0 > ids && -4007 != ids ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("IsReEntry") );
	}
	
	if( -4007 != ids )
	{
		bReEntry = TRUE;
	}

	return 0;

EFS_END
return -1;
}

void CBO_LOC_3320::OnSelchangecmbLOSTMEMBER() 
{
EFS_BEGIN

	CString sGetData;
	CString sCombo;
	CString sState;
	INT		ids;

	ids = GetDataMgrData( m_strDMAlias , _T("회원증분실") , sGetData , 0 );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010, _T("OnSelchangecmbLOSTMEMBER") );
	}

	if( TRUE == sGetData.IsEmpty() ) 
	{
		sGetData = _T("N");
	}
	
	CComboBox* pCombo = ( CComboBox* )GetDlgItem( IDC_cmbLOST_MEMBER );
	pCombo->GetLBText( pCombo->GetCurSel(), sCombo );

	if( 0 == sCombo.Compare( _T("Y") ) ) 
	{
		pCombo = ( CComboBox* )GetDlgItem( IDC_cmbUSER7 );
		pCombo->GetLBText( pCombo->GetCurSel() , sState );
	}

EFS_END	
}

void CBO_LOC_3320::OnbtnFamilyAdd() 
{	
	CString strFamilyID;
	CString strFamilyKey;

	GetDlgItemText( IDC_edtUSER28, strFamilyID );
	SetControlData( m_strCMAlias , _T("가족KEY") , strFamilyKey );

	if( strFamilyID.IsEmpty() )
	{
		AfxMessageBox(_T("가족ID를 입력하신 후에 사용가능 합니다."), MB_ICONINFORMATION);
		return;
	}

	if( strFamilyKey.IsEmpty() )
	{
		if( SetFamilyKey() )
		{
			AfxMessageBox( _T("존재하지 않는 가족ID입니다.\n")
				           _T("다시 입력하여 주십시요.\n")
						   _T("가족ID를 찾으시려면 \"가족ID찾기\" 버튼을 클릭하여 주십시요"), MB_ICONSTOP );
			return;
		}
	}
	if( NULL == m_pFamilyAddDlg )
	{
		m_pFamilyAddDlg = new CFamilyAddDlg(m_pSpfWork, this);
		
		m_pFamilyAddDlg->SetIsUnityLoanService(m_pLoanMangeValue->m_sIsUnityLoanService);
		
		m_pFamilyAddDlg->SetIsAutoSetCivilnoToPassword( m_pLoanMangeValue->m_strIsAutoSetCivilnoToPassword );
		m_pFamilyAddDlg->SetIsAutoSetUsernoToUserid( m_pLoanMangeValue->m_strIsAutoSetUsernoToUserid );
		m_pFamilyAddDlg->Create(this);
		m_pFamilyAddDlg->CenterWindow();		
	}

	m_pFamilyAddDlg->SetCivilNoAutoCheck(this->m_bCivilNoAutoCheck);
	m_pFamilyAddDlg->SetCivilNoDisplayCheck(this->m_bCivilNoDisplayCheck);
	m_pFamilyAddDlg->SetSMSVersion(this->m_sSMSVersion);
	m_pFamilyAddDlg->ShowWindow(SW_SHOW);
}

HBRUSH CBO_LOC_3320::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

void CBO_LOC_3320::OnStaticPicturePos() 
{
EFS_BEGIN

	UserPictureModify();	

EFS_END	
}

void CBO_LOC_3320::OnBtnBscOff() 
{	

}

void CBO_LOC_3320::OnKillfocusEdtBsc() 
{
	
}

void CBO_LOC_3320::OnSetfocusEdtBsc() 
{
	
}

INT	CBO_LOC_3320::BSC_RUN()
{
EFS_BEGIN

	return 0;

EFS_END	
return -1;
}

void CBO_LOC_3320::OnBtnRecard() 
{
	CESL_DataMgr	*pDM;
	CString			strRecKey;
	CString			strUserNo;
	CString			strName;
	
	pDM = FindDM( m_strDMAlias );

	pDM->GetCellData( _T("REC_KEY")		, 0 , strRecKey );
	pDM->GetCellData( _T("대출자번호")	, 0 , strUserNo );
	pDM->GetCellData( _T("이름")		, 0 , strName );

	CFeeReCard dlg(this, m_pLoanMangeValue);

	dlg.SetUserKey( strRecKey, strUserNo, strName );
	dlg.DoModal();
}

void CBO_LOC_3320::SetBirthDay(CString sCivilNo)
{
EFS_BEGIN

	INT ids;

	if(sCivilNo.IsEmpty())
	{
		ids = GetControlData( m_strCMAlias , _T("주민등록번호") , sCivilNo );
		if(0 > ids) ERROR_MSG_RETURN_VOID2( -1 , _T("SetBirthDay") );
	}

	
	sCivilNo.Replace( _T("-") , _T("") );
	sCivilNo.TrimLeft();	sCivilNo.TrimRight();

	
	if( sCivilNo.GetLength() < 8 ) return;

	CString sBirthDay;
	INT nSex = _ttoi(sCivilNo.Mid(6,1));
	if(nSex==1||nSex==2||nSex==5||nSex==6) 
	{ 
		sBirthDay = _T("19");
	}
	else if(nSex==3||nSex==4||nSex==7||nSex==8) 
	{ 
		sBirthDay = _T("20");
	}
	else if(nSex==9||nSex==0) 
	{ 
		sBirthDay = _T("18");
	}
	else 
	{ 
		return;
	}

	INT nYear = _ttoi(sBirthDay + sCivilNo.Left(2));
	INT nMonth	= _ttoi(sCivilNo.Mid(2,2));
	INT nDay	= _ttoi(sCivilNo.Mid(4,2));

	BOOL bVaildDate = FALSE;
	if( 1 <= nMonth && 12 >= nMonth )
	{
		
		INT nMonthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		if( !(nYear % 400) || ((nYear % 100) && !(nYear % 4)) )
		{ 
			nMonthDays[1] = 29;
		}
		if( 1 <= nDay && nMonthDays[nMonth-1] >= nDay )
		{ 
			bVaildDate = TRUE;
		}
	}
	
	if(!bVaildDate) return;

	
	sBirthDay.Format( _T("%d/%02d/%02d"), nYear, nMonth, nDay );
	ids = SetControlData( m_strCMAlias , _T("생일") , sBirthDay );
	if(0 > ids) ERROR_MSG_RETURN_VOID2( -1 , _T("SetBirthDay") );
	

EFS_END
}


INT CBO_LOC_3320::CheckIsPhone()
{
EFS_BEGIN
	
	if(	0 == m_sSMSVersion.Compare(_T("Y")) )
	{
		
		if( 1 == ((CButton*)GetDlgItem(IDC_chkSMSUSEYN))->GetCheck() )
		{
			CString		strPhone1,strPhone2;
			( (CEdit*)GetDlgItem(IDC_edtUSER19) )->GetWindowText(strPhone1);
			( (CEdit*)GetDlgItem(IDC_edtUSER20) )->GetWindowText(strPhone2);

			if( strPhone1.IsEmpty()	||	strPhone2.IsEmpty() )
			{
				AfxMessageBox(_T("SMS 서비스를 신청 하셨습니다.\r\n해당 서비스를 받으시려면 핸드폰 번호를 입력하셔야 합니다."),MB_ICONINFORMATION);
				return	-1;
			}
			m_pSpfWork->SetTempData(_T("SMS수신여부"), CComVariant(_T("Y")) );
		}
		else
		{
			m_pSpfWork->SetTempData(_T("SMS수신여부"), CComVariant(_T("N")) );
		}
	}
	return	0;

EFS_END
return	-1;
}

INT CBO_LOC_3320::CheckIsEMail()
{
EFS_BEGIN
	
	
	if( 1 == ((CButton*)GetDlgItem(IDC_chkMAILUSEYN))->GetCheck() )
	{
		
		CString		strMail1,strMail2;
		( (CEdit*)GetDlgItem(IDC_edtUSER21) )->GetWindowText(strMail1);
		( (CComboBox*)GetDlgItem(IDC_cmbUSER6) )->GetWindowText(strMail2);

		if( TRUE == strMail1.IsEmpty() || TRUE == strMail2.IsEmpty() )
		{
			AfxMessageBox(_T("E-Mail 수신을 신청하셨습니다.\r\n해당 서비스를 받으시려면 주소를 입력하셔야 합니다."));
			return	-1;
		}
		
		if( 0	>=	 strMail2.Find( _T('.')) )
		{
			AfxMessageBox(_T("E-Mail 주소를 정확히 입력하여 주십시요"));
			return	-1;
		}
	}
	return	0;

EFS_END
return	-1;
}

VOID CBO_LOC_3320::OnKillfocusEditName()
{
EFS_BEGIN
	
	
	CString strName;
	GetControlData( m_strCMAlias , _T("이름") , strName );
	if(m_strLastName != strName) 
	{
		m_strLastName = strName;
		m_bIsRealNameCert = FALSE;
	}

EFS_END
}

VOID CBO_LOC_3320::OnKillfocusEditCivilNum1()
{
EFS_BEGIN

	CString sCivilNo	=	MakeCivilNum();
	if( m_strLastCivilNo != sCivilNo )
	{
		
		INT ids = SetControlData(m_strCMAlias, _T("주민등록번호"), sCivilNo );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_VOID2( -1010 , _T("OnKillfocusEditCivilNum1") );
		}

		
		m_bIsRealNameCert = FALSE;
		m_strLastCivilNo = sCivilNo;
	}

EFS_END
}


VOID CBO_LOC_3320::OnKillfocusEditCivilNum2()
{
EFS_BEGIN
		
	CString	sCivilNo	=	MakeCivilNum();
	if( m_strLastCivilNo != sCivilNo )
	{		
		if(		0 == m_nMode	
			||	1 == m_nMode	
			||	3 == m_nMode )	
		{
			
			CString strPassword;
			GetDlgItem(IDC_edtUSER5)->GetWindowText(strPassword);
			
			if( _T("Y") == m_pLoanMangeValue->m_strIsAutoSetCivilnoToPassword && strPassword.IsEmpty() )
			{
				CString		strCivilNo2;
				GetDlgItem(IDC_EDIT_CIVILNUM2)->GetWindowText(strCivilNo2);

				GetDlgItem(IDC_edtUSER5)->SetWindowText(strCivilNo2);
				GetDlgItem(IDC_edtUSER6)->SetWindowText(strCivilNo2);
			}
		}
		
 		INT ids = SetControlData(m_strCMAlias, _T("주민등록번호"), sCivilNo );
 		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_VOID2( -1010 , _T("OnKillfocusEditCivilNum2") );
		}

		m_bIsRealNameCert = FALSE;

		CWnd* pWnd = GetFocus();
	
 		CivilNoCheck();

		if( NULL != pWnd )
		{
			pWnd->SetFocus();
		}
 				
 		
 		SetBirthDay( sCivilNo );

		m_strLastCivilNo = sCivilNo;
	}

EFS_END
}

VOID CBO_LOC_3320::OnKillfocusedtUSER8() 
{
EFS_BEGIN

	CString sCivilNo;
	INT ids;

	ids = GetControlData( m_strCMAlias , _T("주민등록번호") , sCivilNo );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010 , _T("OnKillfocusedtUSER8") );
	}

	if( m_strLastCivilNo != sCivilNo )
	{

		SetBirthDay(sCivilNo);

		m_strLastCivilNo = sCivilNo;
	}
EFS_END
}


CString CBO_LOC_3320::MakeCivilNum()
{
	CString sCivilNo;
	CString strCivilNum1;
	CString strCivilNum2;
	
	GetDlgItem(IDC_EDIT_CIVILNUM1)->GetWindowText(strCivilNum1);
	GetDlgItem(IDC_EDIT_CIVILNUM2)->GetWindowText(strCivilNum2);

	sCivilNo.Format(_T("%s%s"),strCivilNum1,strCivilNum2);
	
	
	return MakeCivilNumFormat(sCivilNo);
}

VOID CBO_LOC_3320::SetCivilNum(BOOL bNumCheck/* = TRUE*/)
{
EFS_BEGIN

	CString	sCivilNo;
	CString	strTemp;
	CString	strMessage;

	INT		nLength	=	0;
	INT		ids		=	GetControlData( m_strCMAlias , _T("주민등록번호") , sCivilNo );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010 , _T("SetCivilNum") );
	}

	strTemp			=	sCivilNo;

	sCivilNo.Replace(_T("-"),_T(""));

	
	nLength		=	sCivilNo.GetLength();
	if( 13	!=	nLength )	
	{
		
		if( 6 >= nLength )
		{
			
			GetDlgItem(IDC_EDIT_CIVILNUM1)->SetWindowText(sCivilNo);
			GetDlgItem(IDC_EDIT_CIVILNUM2)->SetWindowText(_T(""));
			return;
		}
		
		else if( 7 <= nLength && 12 >= nLength )
		{
			
			GetDlgItem(IDC_EDIT_CIVILNUM1)->SetWindowText( sCivilNo.Left(6) );
			
			GetDlgItem(IDC_EDIT_CIVILNUM2)->SetWindowText( sCivilNo.Right(nLength-6) );
			return;
		}
		
		else if( 13 < nLength )
		{
			
			GetDlgItem(IDC_EDIT_CIVILNUM1)->SetWindowText( sCivilNo.Left(6) );
			
			GetDlgItem(IDC_EDIT_CIVILNUM2)->SetWindowText( sCivilNo.Mid(6,7) );
			return;
		}
	}

	
	if(bNumCheck)
	{
		for( INT i=0 ; i<nLength ; i++ )
		{
			if( !isdigit(sCivilNo[i]) )
			{
				strMessage.Format(_T("저장 되어 있는 주민등록번호는 \"%s\" 입니다.\r\n주민등록번호는 숫자입니다. 다시 입력해 주십시요."),strTemp);
				AfxMessageBox( strMessage , MB_ICONINFORMATION );
				return;
			}
		}
	}

	GetDlgItem(IDC_EDIT_CIVILNUM1)->SetWindowText( sCivilNo.Left(6) );
	GetDlgItem(IDC_EDIT_CIVILNUM2)->SetWindowText( sCivilNo.Right(7) );

EFS_END
}

VOID CBO_LOC_3320::OnbtnLoanStopDateSet() 
{
EFS_BEGIN

	CString strLoanStopDate;
 	CLoanStopDateSet dlg;
	GetControlData(m_strCMAlias, _T("대출정지일"), strLoanStopDate, -1, -1);
	dlg.SetLoanStopDate(strLoanStopDate);
 	if( IDOK == dlg.DoModal() )
	{
		dlg.GetLoanStopDate(strLoanStopDate);
		SetControlData(m_strCMAlias, _T("대출정지일"), strLoanStopDate, -1, -1);
	}
 	
EFS_END
}


int CBO_LOC_3320::IsILLUser(BOOL &bILLUser)
{
	int ids = -1;
	bILLUser = FALSE;
	
	if(_T("Y") == m_strUseILL)
	{
		CESL_DataMgr* pDM = FindDM(m_strDMAlias);
		if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE2(-1, _T("CBO_LOC_3320::IsILLUser"));
		
		CString strUserNo;
		ids = pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-2, _T("CBO_LOC_3320::IsILLUser"));
		
		CString strSQLQuery;
		CString strSQLResult;
		
		strSQLQuery.Format(
			_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE USER_NO = '%s' AND ")
			_T("END_DIV IS NULL"),
			strUserNo);
		ids = pDM->GetOneValueQuery(strSQLQuery, strSQLResult);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-3, _T("CBO_LOC_3320::IsILLUser"));
		
		if(0 < _ttoi(strSQLResult)) bILLUser = TRUE;
	}
	
	return 0;
}

LRESULT	CBO_LOC_3320::OnTouchPad(WPARAM wParam, LPARAM lParam)
{	

	
	return 0;
}

VOID CBO_LOC_3320::OnbtnSearchKLLib()
{
	CString strUserNo, strGpinHash;
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300"));
	pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	pDM->GetCellData(_T("GPIN_HASH"), 0, strGpinHash);
	CKL_SecedeDlg dlg(this);
	dlg.SetUserNo(strUserNo);
	dlg.SetGpinHash(strGpinHash);
	dlg.DoModal();
	m_bIsSelfLibSecede = dlg.GetIsSelfLibSecede();
	if(TRUE == m_bIsSelfLibSecede)
	{
		CString strReckey;
		pDM->GetCellData(_T("REC_KEY"), 0, strReckey);		
		
		if(3 != m_nMode)
		{
			if(!dlg.GetIsChange())
			{
				CESL_DataMgr* pParentDM = FindDM( _T("DM_BO_LOC_3300_LIST") );
				if(NULL==pParentDM) return;
				pParentDM->DeleteRow(m_nCurrentIndex);
				AllControlDisplay(_T("CM_BO_LOC_3300"));
				AfxMessageBox(_T("탈퇴 처리가 완료 되었습니다."));
			}
		}
		OnbtnCLOSE();
	}
}

VOID CBO_LOC_3320::OnbtnSearchKLCurrentLoan()
{
	
 	CString strUserNo;
 	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300"));
 	pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	if(strUserNo.IsEmpty()) 
	{
		AfxMessageBox(_T("대출자번호가 없습니다."));
		return;
	}
	CKL_SearchCurrentLoanInfoDlg dlg(this);
 	dlg.SetUserNo(strUserNo);
	dlg.DoModal();
}

VOID CBO_LOC_3320::OnbtnKLRealNameCert() 
{
	INT ids = KLRealNameCert();


	if(0==ids) AfxMessageBox(_T("본인인증에 성공하였습니다."));
}


INT CBO_LOC_3320::KLRealNameCert()
{
	
	if(TRUE == m_pInfo->m_bCertifyUse) return 0;

	if(m_bIsTCTInput)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
		pCombo->SetCurSel(1);
		pCombo->EnableWindow(TRUE);
		OnSelchangecmbKLMemberYN();
	}	
	CString strName, strCivilNo, strUserNo;
	GetControlData(m_strCMAlias, _T("이름"), strName);
	strName.TrimLeft();	strName.TrimRight();
	if(strName.IsEmpty())
	{
		AfxMessageBox(_T("이름을 입력해주세요."),MB_ICONINFORMATION);
		return 1020;
	}
	
	CString strCivilNo1,strCivilNo2;
	GetDlgItem(IDC_EDIT_CIVILNUM1)->GetWindowText(strCivilNo1);
	GetDlgItem(IDC_EDIT_CIVILNUM2)->GetWindowText(strCivilNo2);
	strCivilNo.Format(_T("%s%s"),strCivilNo1,strCivilNo2);
	strCivilNo = MakeCivilNumFormat(strCivilNo);
	if(strCivilNo.IsEmpty())
	{
		AfxMessageBox(_T("주민번호를 입력해주세요."),MB_ICONINFORMATION);
		return 1030;
	}

	INT ids;
	if(0 != m_nMode)
	{
		CString strTemp;
		GetDataMgrData( m_strDMAlias , _T("주민등록번호") , strTemp , 0 );
		if(strTemp==strCivilNo)
		{
			GetDataMgrData( m_strDMAlias , _T("주민등록번호_CRYPTO") , strTemp , 0 );
			if(!strTemp.IsEmpty()) this->ESL_DES_Decryption(strTemp, strCivilNo);
		}
	}

	ids = CLocCommonAPI::CivilNoCheck(strCivilNo);
	if(0<ids)
	{
		AfxMessageBox(_T("잘못된 주민등록번호입니다.\r\n")
					  _T("올바른 주민등록번호를 입력해주십시오"));
		GetDlgItem(IDC_EDIT_CIVILNUM1)->SetFocus();
		return 1050;
	}
	
	if(TRUE == m_pParent->m_pInfo->m_bKLUse && FALSE == m_pParent->m_pInfo->m_bCertifyUse)
	{
		CESL_DataMgr pDM;
		pDM.SetCONNECTION_INFO(_T(""));
		CString strQuery, strValue, strCryptoCivilNo, strRecKey;
		this->ESL_DES_Encryption(strCivilNo, strCryptoCivilNo);
		ids = GetDataMgrData(m_strDMAlias, _T("REC_KEY"), strRecKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("KLRealNameCert"));
		if(strRecKey.IsEmpty())
		{
			strQuery.Format(_T("SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO='%s'"), strCryptoCivilNo);
		}
		else
		{
			strQuery.Format(_T("SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE CRYPTO_CIVIL_NO='%s' AND REC_KEY<>%s"), strCryptoCivilNo, strRecKey);		
		}
		ids = pDM.GetOneValueQuery(strQuery, strValue);
		if(0 > ids && -4007 != ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("KLRealNameCert"));
		if(!strValue.IsEmpty())
		{
			strQuery.Format(_T("중복된 주민등록번호입니다.\r\n대출자번호 : %s"), strValue);
			AfxMessageBox(strQuery);
			GetDlgItem(IDC_EDIT_CIVILNUM2)->SetFocus();
			return 1070;
		}
	}
	
	strCivilNo.Replace(_T("-"),_T(""));

	
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		
		AfxMessageBox(_T("통합도서서비스 장애가 발생하였습니다.\r\n일반회원으로 가입하여 대출/반납을 할 수 있습니다.\r\n시스템 정상화시 책이음회원으로 전환하여 주십시오."), MB_ICONSTOP);
		return 1060; 
	}

 	CKL_UserManagerDlg dlg(this);
 	dlg.SetMode(0);
 	dlg.SetUserName(strName);
 	dlg.SetCivilNo(strCivilNo);
 	if(IDOK == dlg.DoModal())
 	{
 		m_bIsRealNameCert = dlg.GetIsRealNameCert();
 		m_strGPIN_ASCII = dlg.GetGPIN_ASCII();
 		m_strGPIN_HASH = dlg.GetGPIN_HASH();
 		m_strGPIN_SEX = dlg.GetGPIN_SEX();		
 		m_strCI = dlg.GetCI();		
 		if(1==m_nMode || 3==m_nMode)
 		{
 			CESL_DataMgr *pDM = FindDM( m_strDMAlias );
 			pDM->StartFrame();
 			pDM->InitDBFieldData();
 			pDM->AddDBFieldData(_T("GPIN_SEX"),		_T("STRING"), m_strGPIN_SEX);
 			pDM->AddDBFieldData(_T("GPIN_HASH"),	_T("STRING"), m_strGPIN_HASH);
 			pDM->AddDBFieldData(_T("GPIN_ASCII"),	_T("STRING"), m_strGPIN_ASCII);
 			pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY") , _T("NUMERIC") , pDM->GetCellData(_T("REC_KEY"), 0));
 			pDM->SendFrame();
 			pDM->EndFrame();
 		}
 
 		
 		if(TRUE != m_pInfo->m_bCertifyUse && 0 != m_nMode && !m_strCI.IsEmpty())
 		{
 			CString strRecKey = _T("");
 			GetDataMgrData(m_strDMAlias, _T("REC_KEY"), strRecKey, 0);
 			if(!strRecKey.IsEmpty())
 			{
 				CESL_DataMgr pDM;
 				pDM.SetCONNECTION_INFO(_T(""));
 				CString strQuery = _T("");
 				strQuery.Format(_T("SELECT IPIN_HASH FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strRecKey);
 				CString strValue = _T("");
 				ids = pDM.GetOneValueQuery(strQuery, strValue);
 				if(0 > ids && -4007 != ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("KLRealNameCert"));
 				if(strValue.IsEmpty())
 				{
 					pDM.StartFrame();
 					pDM.InitDBFieldData();
 					pDM.AddDBFieldData(_T("IPIN_HASH"), _T("STRING"), m_strCI);
 					pDM.AddDBFieldData(_T("IPIN_DATE"), _T("NUMERIC"), _T("SYSDATE"));
 					pDM.AddDBFieldData(_T("IPIN_RESULT"), _T("NUMERIC"), _T("NULL"));
 					pDM.AddDBFieldData(_T("CERTIFY_WORKER"), _T("STRING"), m_pInfo->USER_ID);
 					pDM.MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
 					pDM.SendFrame();
 					pDM.EndFrame();
 				}
 			}
 		}
 		
 		m_strCertCivilNo = strCivilNo;
 		m_bIsTCTInput = dlg.GetIsTCTInput();
 		if(m_bIsTCTInput)
 		{
 			if(0==m_nMode) m_nLTMemberSaveMode = 4;	
 			else m_nLTMemberSaveMode = 5; 
 		}
 		else
 		{
 			CString strData = dlg.GetUserNo();
 			if(!strData.IsEmpty())
 			{
 				SetControlData(_T("CM_BO_LOC_3320"), _T("대출자번호"), strData);		
 			}
 			
 			if(0==m_nMode) m_nLTMemberSaveMode = 1; 
 			
 			else m_nLTMemberSaveMode = 2; 
 		}
 
 		if(4==m_nLTMemberSaveMode || 5==m_nLTMemberSaveMode)
 		{
 			ids = KL_SetMemberInfo(dlg.GetMemberInfoDM());
 			if(0>ids) return ids;
 		}
 		
 		
 		m_strCivilNo = strCivilNo;
 		return 0;
 	}

	m_nLTMemberSaveMode = 0;
	return 1060;
}

INT CBO_LOC_3320::KLMemberInsert()
{
	const INT nColCnt = 5;
	CString strData;
	CString strAlias[nColCnt];

	
	CString strDMAlias;
	INT nIdx;
	CESL_DataMgr *pDM;
	if(0==m_nMode)
	{
		strDMAlias = _T("DM_BO_LOC_3300_LIST");
		pDM = FindDM( strDMAlias );
		if(NULL==pDM) return -1010;

		nIdx = pDM->GetRowCount() -1;
		strAlias[0] = _T("대출자번호");
		strAlias[1] = _T("이름");
		strAlias[2] = _T("핸드폰");
		strAlias[3] = _T("주민등록번호");
		strAlias[4] = _T("생년월일");
	}
	else if(1==m_nMode || 3==m_nMode)
	{
		strDMAlias = _T("DM_BO_LOC_3300");
		pDM = FindDM( strDMAlias );
		if(NULL==pDM) return -1010;

		nIdx = 0;
		strAlias[0] = _T("대출자번호");
		strAlias[1] = _T("이름");
		strAlias[2] = _T("휴대폰");
		strAlias[3] = _T("주민등록번호");
		strAlias[4] = _T("생일");
	}

	CString strUserNo, strUserName, strHandphone, strCivilNo;
	strUserNo = pDM->GetCellData(strAlias[0],nIdx);
	strUserName = pDM->GetCellData(strAlias[1],nIdx);
	strHandphone = pDM->GetCellData(strAlias[2],nIdx);
	strCivilNo = pDM->GetCellData(strAlias[3],nIdx);
	CString strBirthYear, strTemp;
	strTemp = pDM->GetCellData(strAlias[4],nIdx);
	if(!strTemp.IsEmpty() && 3 < strTemp.GetLength())
	{
		strBirthYear = strTemp.Left(4);
	}
	CKL_UserManagerDlg dlg(this);
	dlg.SetMode(1);
	dlg.SetKLUserKey(m_strKLUserKey);
	dlg.SetUserNo(strUserNo);
	dlg.SetUserName(strUserName);
	dlg.SetHandphone(strHandphone);
	dlg.SetCivilNo(strCivilNo);
	dlg.SetGPIN_ASCII(m_strGPIN_ASCII);
	dlg.SetGPIN_HASH(m_strGPIN_HASH);
	dlg.SetGPIN_SEX(m_strGPIN_SEX);
	dlg.SetBirthYear(strBirthYear);

	dlg.SetCI(m_strCI);
	if (!m_pSpfWork->m_strCertifyCode.IsEmpty() && (TRUE == m_pInfo->m_bCertifyUse || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0))
	{
		dlg.SetCI(m_pSpfWork->m_strCertifyCode);
	}

	
	if(IDOK == dlg.DoModal()) 


	{
		m_strKLCreateDate = dlg.m_strCreateDate;
		return 0;
	}

	return 1010;
}

INT CBO_LOC_3320::KLMemberChange()
{
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3300") );
	if(NULL==pDM) return -1010;

	CString strUserNo, strUserName, strHandphone, strCivilNo;
	strUserNo = pDM->GetCellData(_T("대출자번호"),0);
	strUserName = pDM->GetCellData(_T("이름"),0);
	strHandphone = pDM->GetCellData(_T("휴대폰"),0);
	strCivilNo = pDM->GetCellData(_T("주민등록번호"),0);	

	CString strBirthYear, strTemp;
	strTemp = pDM->GetCellData(_T("생일"),0);
	if(!strTemp.IsEmpty() && 3 < strTemp.GetLength())
	{
		strBirthYear = strTemp.Left(4);
	}

	CKL_UserManagerDlg dlg(this);
	dlg.SetMode(2);
	dlg.SetKLUserKey(m_strKLUserKey);
	dlg.SetUserNo(strUserNo);
	dlg.SetUserName(strUserName);
	dlg.SetHandphone(strHandphone);
	dlg.SetCivilNo(strCivilNo);
	dlg.SetGPIN_ASCII(m_strGPIN_ASCII);
	dlg.SetGPIN_HASH(m_strGPIN_HASH);
	dlg.SetGPIN_SEX(m_strGPIN_SEX);

	dlg.SetBirthYear(strBirthYear);

	dlg.SetCI(m_strCI);

	if (!m_pSpfWork->m_strCertifyCode.IsEmpty() && (TRUE == m_pInfo->m_bCertifyUse || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0))
	{
		dlg.SetCI(m_pSpfWork->m_strCertifyCode);
	}

	if(IDOK == dlg.DoModal()) 
	{
		m_strKLCreateDate = dlg.m_strCreateDate;
		return 0;
	}

	return 1010;
}

INT CBO_LOC_3320::KLMemberUpdate(CString strBeforeUserNo, BOOL bChange)
{
	CKL_UserManagerDlg dlg(this);
	dlg.SetMode(4);
	dlg.SetBeforeUserNo(strBeforeUserNo);
	dlg.SetIsUpdateKLMemberLibManageInfo(bChange);
	
	dlg.SetCI(m_pSpfWork->m_strCertifyCode);
	
 	if(IDOK == dlg.DoModal())
	{
		return 0;
	}

	return 1010;
}


INT CBO_LOC_3320::KL_SetMemberInfo(CESL_DataMgr* pKLMemberInfoDM)
{
	const INT nColCnt = 19;
 	CString strDMAlias[nColCnt] = 
	{
		_T("대출자번호"),		_T("생일"),			_T("생일TYPE"),		_T("근무지내선번호"),	_T("부서명"),
		_T("직급명"),			_T("학교"),			_T("학년"),			_T("반"),				_T("교번/순번/군번"),
		_T("우편발송지역"), 	_T("비고"),			_T("자택주소"),		_T("자택전화"),			_T("자택우편번호"),
		_T("근무지우편번호"), 	_T("근무지전화"),	_T("근무지주소"),	_T("근무처")
	};

 	CString strCMAlias[nColCnt] = 
	{
		_T("대출자번호"),		_T("생일"),			_T("생일TYPE"),		_T("근무지내선번호"),	_T("부서명"),
		_T("직급명"),			_T("학교명"),		_T("학년"),			_T("반"),				_T("교번/순번/군번"),
		_T("우편발송지역"), 	_T("비고"),			_T("자택주소"),		_T("자택전화"),			_T("자택우편번호"),
		_T("근무지우편번호"), 	_T("근무지전화"),	_T("근무지주소"),	_T("근무처")
	};

	m_strKLUserKey = pKLMemberInfoDM->GetCellData(_T("통합대출자키"), 0);
	m_strKLWORKNO = pKLMemberInfoDM->GetCellData(_T("회원증RFID"), 0);

	CString strData;
	
	for(INT i=0; i<nColCnt; i++)
	{
		strData = pKLMemberInfoDM->GetCellData(strDMAlias[i], 0);
		if(strData.IsEmpty()) continue;
		SetControlData(_T("CM_BO_LOC_3320"), strCMAlias[i], strData);
	}
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
	pCombo->SetCurSel(0);
	pCombo->EnableWindow(FALSE);
	
	OnSelchangecmbKLMemberYN();	
	((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);

	
	CString sGetData;
	sGetData = pKLMemberInfoDM->GetCellData(_T("대출자번호"),0);
	((CEdit*)GetDlgItem(IDC_edtUSER1))->SetWindowText(sGetData);
	KL_InsertUserNoClass(sGetData);
	
	
	sGetData = pKLMemberInfoDM->GetCellData(_T("휴대폰"),0);
	ViewHandPhoneNum( sGetData );

	
	sGetData = pKLMemberInfoDM->GetCellData(_T("E_MAIL"),0);
	TCHAR *sCMEmailFieldAlias[2] = {_T("EMAIL1"),_T("EMAIL2")};
	if( TRUE == sGetData.IsEmpty() ) 
	{		
		CComboBox *pCombo = (CComboBox*)(GetDlgItem( IDC_cmbUSER6 ));
		INT nCount = pCombo->GetCount();
		CString sGetComboData;
		BOOL bEqual = FALSE;
		INT		j;
		for( j=0 ; j<nCount ; j++ )
		{
			pCombo->GetLBText( j , sGetComboData );
			if( 0 == sGetComboData.Compare(_T("적용안함")) ) 
			{
				bEqual = TRUE;
			}
		}

		if( TRUE != bEqual ) 
		{
			pCombo->AddString( _T("적용안함") );
		}

		pCombo->SetWindowText( _T("적용안함") );

		INT ids = SetControlData( m_strCMAlias , _T("EMAIL1") , _T("") ); 
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SettingEmail") );
		}
	
		return 0;
	}
	
	sGetData.Replace(_T("@@"), _T("@"));
	CString sTmp = sGetData;
	CString sLine;
	INT nSIndex=0;
	i = 0;

	while(true)
	{
		nSIndex = sTmp.Find( _T("@") , 0 );
		if( 0 > nSIndex && TRUE == sTmp.IsEmpty() ) 
		{
			break;
		}

		if( -1 < nSIndex ) 
		{
			sLine = sTmp.Mid( 0 , nSIndex );
		}
		else
		{
			sLine = sTmp;
			sTmp.Empty();
		}

		if( 1 == i ) 
		{
			CComboBox *pCombo = (CComboBox*)(GetDlgItem( IDC_cmbUSER6 ));
			INT nCount = pCombo->GetCount();
			CString sGetComboData;
			bool bEqual = false;
			INT		j;
			for( j=0 ; j<nCount ; j++ )
			{
				pCombo->GetLBText( j , sGetComboData );
				if( 0 == sGetComboData.Compare(sLine) ) bEqual = true;
			}

			if( TRUE != bEqual ) pCombo->AddString( sLine );
			pCombo->SetWindowText( sLine );
		}
		else
		{
			INT ids = SetControlData( m_strCMAlias , sCMEmailFieldAlias[i] , sLine ); 
			if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SettingEmail") );
		}
		
		sLine.Empty();
		sTmp = sTmp.Mid( nSIndex+1 );
		i++;
	}

	
	
	m_pSpfWork->SetTempData(_T("KL_IMPORT_YN"), CComVariant(_T("Y")));
	

	return 0;
}

VOID CBO_LOC_3320::OnSelchangecmbKLMemberYN() 
{
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
	INT nIndex = pCombo->GetCurSel();
	CString strData;
	pCombo->GetLBText(nIndex,strData);
	if(_T("Y")==strData)
	{
		
		if(_T("Y") == m_bCivilNoDisplayCheck)
		{
			GetDlgItem(IDC_btnCHECK_CIVIL_NO)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO))->SetCheck(TRUE);
			GetDlgItem(IDC_btnCHECK_CIVIL_NO2)->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2))->SetCheck(TRUE);
		}
		((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
		if( 0 == m_nMode )
		{
			
			INT ids = SetControlData(m_strCMAlias, _T("대출자번호"), _T("[자동생성]"));
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1050 , _T("SetScreen"));

			
			((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);

			GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_SHOW);
		}
		
		if(m_bIsKLMemberYN)
		{
			GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_SHOW);
		}

		m_nUserNoAutoMode = 2;
		m_pSpfWork->m_nAutoMode = m_nUserNoAutoMode;
	}
	else
	{
		if(_T("Y") == m_bCivilNoDisplayCheck)
		{
			INT nCivilCheck = BST_CHECKED;
			CButton* pCivilErrCheck = (CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO);
			CButton* pCivilDupCheck = (CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2);
			pCivilErrCheck->EnableWindow(TRUE);
			pCivilErrCheck->SetCheck(nCivilCheck);
			pCivilDupCheck->EnableWindow(TRUE);
			pCivilDupCheck->SetCheck(nCivilCheck);
		}

		((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(FALSE);
		
		CString strResult;
		m_pLoanMangeValue->GetManageValue( _T("열람"), _T("공통"), _T("이용자관리"), _T("대출자번호자동부여여부"), strResult);
		strResult.TrimLeft( );		strResult.TrimRight( );
		if(strResult.IsEmpty()) strResult = _T("N");

		if( _T("U") == strResult ) m_nUserNoAutoMode = 3;
		else if( _T("A") == strResult ) m_nUserNoAutoMode = 1;
		else if( _T("Y") == strResult ) m_nUserNoAutoMode = 2;
		else if( _T("N") == strResult ) m_nUserNoAutoMode = 0;
		else m_nUserNoAutoMode = 0;

		if( 0 == m_nMode )
		{
			CString strSetUserNoText = _T("");
			if(0==m_nUserNoAutoMode)
			{
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(FALSE);
				((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(FALSE);
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_HIDE);
			}
			else if(1==m_nUserNoAutoMode)
			{
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
				((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(FALSE);
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_HIDE);

				strSetUserNoText = _T("[주민번호로생성]");
			}
			else if(2==m_nUserNoAutoMode)
			{
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
				((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(FALSE);
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_SHOW);

				strSetUserNoText = _T("[자동생성]");
			}
			else if(3==m_nUserNoAutoMode)
			{
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
				((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(FALSE);
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_HIDE);

				strSetUserNoText = _T("[사용자ID로생성]");
			}
			INT ids = SetControlData(m_strCMAlias, _T("대출자번호"), strSetUserNoText);
			if(0>ids) ERROR_MSG_RETURN_VOID2( -1030 , _T("OnSelchangecmbKLMemberYN"));
		}
		else if( 1 == m_nMode || 3 == m_nMode )
		{
			if(0==m_nUserNoAutoMode)
			{
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(FALSE);
				((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(FALSE);
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_HIDE);
			}
			else if(1==m_nUserNoAutoMode)
			{
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(FALSE);
				((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(FALSE);
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_HIDE);
			}
			else if(2==m_nUserNoAutoMode)
			{
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(FALSE);
				((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(FALSE);
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_SHOW);
			}
			else if(3==m_nUserNoAutoMode)
			{
				((CEdit*)GetDlgItem(IDC_edtUSER1))->SetReadOnly(TRUE);
				((CEdit*)GetDlgItem(IDC_edtUSER4))->SetReadOnly(TRUE);
				GetDlgItem(IDC_btnSERIALNO)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_cmbUSER9)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_sNUM_TYPE)->ShowWindow(SW_HIDE);
			}
		}
		m_pSpfWork->m_nAutoMode = m_nUserNoAutoMode;
	}
}

INT CBO_LOC_3320::SelfLibSecede(CString strReckey)
{

	return 0;
}

INT CBO_LOC_3320::KL_InsertUserNoClass(CString strUserNo)
{
EFS_BEGIN

	BOOL bFind = FALSE;
	INT ids, nComboCnt;
	CString strTemp, strFuncName;
	strTemp = strUserNo.Left(8);
	strFuncName = _T("KL_InsertUserNoClass");

	CESL_ControlMultiComboBox* pCombo = (CESL_ControlMultiComboBox*)FindControl( m_strCMAlias , _T("이용자번호구분"));
	nComboCnt = pCombo->GetRowCount();
	for(INT i=0; i<nComboCnt; i++)
	{
		CString strCode;
		ids = pCombo->GetData(strCode, i, 0);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);

		strCode.TrimLeft(); strCode.TrimRight();
		if(strCode==strTemp)
		{
			pCombo->SetCurSel(i);
			bFind = TRUE;
			break;
		}
	}

	if(!bFind)
	{
		CString strInsertCode[2];
		strInsertCode[0] = strTemp;
		strInsertCode[1] = _T("타도서관이용자번호구분");
		ids = pCombo->SetData(strInsertCode, 2, nComboCnt);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);

		pCombo->SetCurSel(nComboCnt);
	}

	return 0;

EFS_END
return -1;
}

INT CBO_LOC_3320::KL_DeleteUserNoClass(CString strDesc)
{
EFS_BEGIN

	INT ids, nComboCnt;
	CString strTemp, strFuncName;
	strFuncName = _T("KL_DeleteUserNoClass");

	CESL_ControlMultiComboBox* pCombo = (CESL_ControlMultiComboBox*)FindControl( m_strCMAlias , _T("이용자번호구분"));
	nComboCnt = pCombo->GetRowCount();
	for(INT i=0; i<nComboCnt; i++)
	{
		CString strData;
		ids = pCombo->GetData(strData, i, 1);
		if(0>ids) ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);
		strData.TrimLeft(); strData.TrimRight();

		if(strData==strDesc)
		{
			pCombo->DeleteString(i);
			break;
		}
	}

EFS_END
return -1;
}

void CBO_LOC_3320::OnbtnViewUserCardHistory()
{
	CString strRecKey,strUserName, strUserNo, strCaption, strGpinHash;
	GetDlgItem(IDC_BTN_USER_CARD)->GetWindowText(strCaption);
	
	if(-1<strCaption.Find(_T("책이음회원증")))
	{
		CKL_UserCardHistoryDlg Dlg(this);
		CString strRecKey;
		GetRecKey(strRecKey);
		GetDataMgrData( m_strDMAlias, _T("이름"), strUserName, 0 );
		GetDataMgrData( m_strDMAlias, _T("대출자번호"), strUserNo, 0 );
		GetDataMgrData( m_strDMAlias, _T("GPIN_HASH"), strGpinHash, 0 );
		Dlg.SetUserKey(strRecKey);
		Dlg.SetUserName(strUserName);
		Dlg.SetUserNo(strUserNo);
		
		Dlg.m_nMode = m_nMode;
		Dlg.m_strGpinHash = strGpinHash;
		Dlg.DoModal();
	}
	else
	{
		CUserCardHistoryDlg Dlg(this);
		GetRecKey(strRecKey);
		GetDataMgrData( m_strDMAlias, _T("이름"), strUserName, 0 );
		GetDataMgrData( m_strDMAlias, _T("대출자번호"), strUserNo, 0 );
		Dlg.SetUserKey(strRecKey);
		Dlg.SetUserNo(strUserNo);
		Dlg.SetUserName(strUserName);
		
		Dlg.m_nMode = m_nMode;
		CString sValue;
		m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), sValue);
		if(_T("Y") != sValue) sValue = _T("N");
		Dlg.m_strCivilNoHideYN = sValue;
		Dlg.m_sIsUnityLoanService = m_pLoanMangeValue->m_sIsUnityLoanService;
		Dlg.DoModal();
	}
}

VOID CBO_LOC_3320::SetSMSUseYNMode()
{
	
	CButton *pCheck = (CButton*)GetDlgItem(IDC_chkSMSUSEYN);
	if( 0 != m_sSMSVersion.Compare(_T("N")) )
	{		
		pCheck->ShowWindow(SW_SHOW);
		
		
		if(0 == m_nMode)
		{
			CFileFind ff;
			if( !ff.FindFile(_T("..\\cfg\\SMS수신여부설정.cfg")) )
			{ 
				FILE *fp = _tfopen( _T("..\\cfg\\SMS수신여부설정.cfg"), _T("w+b") );
				fputc(0xFF, fp); 
				fputc(0xFE, fp);
				
				_ftprintf( fp, _T("%d"), 1 );
				pCheck->SetCheck(BST_CHECKED);
				fclose( fp );
			}
			else
			{
				CString strSetSMSUseYNMode;
				CStdioFile fp;
				if( !fp.Open( _T("..\\cfg\\SMS수신여부설정.cfg"), CFile::modeRead | CFile::typeBinary ) )
					return;
				
				TCHAR cUni;
				fp.Read( &cUni , sizeof(TCHAR) );
				if( 0xFEFF != cUni )
				{
					fp.SeekToBegin();
				}
				
				fp.ReadString(strSetSMSUseYNMode);
				fp.Close();
				pCheck->SetCheck(_ttoi(strSetSMSUseYNMode));
			}
		}
	}
	else
	{
		if(0 == m_nMode)
		{
			pCheck->ShowWindow(SW_HIDE);
			pCheck->SetCheck(0);
		}
	}
}


VOID CBO_LOC_3320::SetMailingUseYNMode()
{
	
	CString strValue;
	INT ids = m_pLoanMangeValue->GetManageValue(_T("기타"), _T("공통"), _T("메일링시스템"), _T("사용여부"), strValue);
	if(0>ids) ERROR_MSG_RETURN_VOID2( -1060 , _T("SetScreen"));

	CButton *pCheck = (CButton*)GetDlgItem(IDC_chkMAILUSEYN);
	if( 0 != strValue.Compare(_T("Y")) )
	{ 
		if(0 == m_nMode)
		{
			pCheck->SetCheck(0);
		}
	}
	else
	{ 
		if(0 == m_nMode)
		{
			CFileFind ff;
			if( !ff.FindFile(_T("..\\cfg\\Mailing수신여부설정.cfg")) )
			{
				FILE *fp = _tfopen( _T("..\\cfg\\Mailing수신여부설정.cfg"), _T("w+b") );
				fputc(0xFF, fp); 
				fputc(0xFE, fp);
				
				_ftprintf( fp, _T("%d"), 1 );
				pCheck->SetCheck(BST_CHECKED);
				fclose( fp );
			}
			else
			{
				CString strSetMailingUseYNMode;
				CStdioFile fp;
				if( !fp.Open( _T("..\\cfg\\Mailing수신여부설정.cfg"), CFile::modeRead | CFile::typeBinary ) )
					return;
				
				TCHAR cUni;
				fp.Read( &cUni , sizeof(TCHAR) );
				if( 0xFEFF != cUni )
				{
					fp.SeekToBegin();
				}
				
				fp.ReadString(strSetMailingUseYNMode);
				fp.Close();
				pCheck->SetCheck(_ttoi(strSetMailingUseYNMode));
			}
		}
	}
}

void CBO_LOC_3320::OnbtnUserEditLog()
{
	CUserEditSearchDlg dlg(this);
	GetRecKey(dlg.m_strUserKey);
	if(dlg.m_strUserKey.IsEmpty()) return;
	dlg.DoModal();
}

BOOL CBO_LOC_3320::KLGetSecedeYN(CString strUserNo)
{
	CKL_UserManagerDlg dlg(this);
	dlg.SetMode(9);
	dlg.SetUserNo(strUserNo);
	if(IDOK == dlg.DoModal())
	{
		if(dlg.GetIsSecede()) return TRUE;
	}
	
	return FALSE;
}


void CBO_LOC_3320::OnbtnIDPASSView()
{
	if(!m_pInfo->IsConnectUnityLoanService())
	{
		AfxMessageBox(_T("센터와 접속시도 중 입니다."));
		return;
	}
	CKlIdPassViewDlg dlg(this);
	dlg.m_nMode = m_nMode;
	
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	dlg.m_strUserNo = pDM->GetCellData(_T("대출자번호"), 0);

	dlg.DoModal();
}

void CBO_LOC_3320::OnDestroy() 
{
	CDialog::OnDestroy();
	
	::DestroyIcon(m_hBalloonIcon);
	m_Font.DeleteObject();
}


void CBO_LOC_3320::Onbtn3320CertifyY() 
{
	if(0 != m_nMode && 1 != m_nMode && 3 != m_nMode) return;	
	
	if(IDYES == MessageBox(_T("이미 본인인증된 이용자입니다.\r\n본인인증을 다시 하시겠습니까?"), _T("본인인증"), MB_YESNO|MB_ICONQUESTION))
	{
		StartUserCertify();
	}
}


void CBO_LOC_3320::Onbtn3320CertifyFail() 
{
	if(0 != m_nMode && 1 != m_nMode && 3 != m_nMode) return;
	StartUserCertify();
}


void CBO_LOC_3320::Onbtn3320CertifyN() 
{
	if(0 != m_nMode && 1 != m_nMode && 3 != m_nMode) return;
	StartUserCertify();
}

void CBO_LOC_3320::StartUserCertify(int nMode)
{

	if (g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0)
	{
		StartTotalCertify(nMode);
		return;
	}

	CUserCertifyDlg dlg(this);

	
	if(0 == m_nMode)
	{
		dlg.m_nMode = 0;
	}
	else
	{
		dlg.m_nMode = 1;
	}

	GetDlgItemText(IDC_edtUSER7, dlg.m_strName);
	
	if(_T("Y") == m_bCivilNoDisplayCheck)
	{
		
		if(TRUE == m_pInfo->m_bKLUse)
		{
			CString strCivilNo, strCivilNo1, strCivilNo2;
			GetDlgItemText(IDC_EDIT_CIVILNUM1, strCivilNo1);
			GetDlgItemText(IDC_EDIT_CIVILNUM2, strCivilNo2);
			strCivilNo.Format(_T("%s-%s"), strCivilNo1, strCivilNo2);
			if(0 != m_nMode)
			{
				CString strTemp;
				GetDataMgrData(m_strDMAlias, _T("주민등록번호"), strTemp, 0);
				if(strTemp == strCivilNo)
				{
					GetDataMgrData(m_strDMAlias, _T("주민등록번호_CRYPTO"), strTemp, 0);
					if(!strTemp.IsEmpty())
					{
						this->ESL_DES_Decryption(strTemp, strCivilNo);
						if(14 == strCivilNo.GetLength())
						{
							strCivilNo1 = strCivilNo.Left(6);
							strCivilNo2 = strCivilNo.Right(7);
						}
					}
				}
			}
			if(6 == strCivilNo1.GetLength()) dlg.m_strCivilNo1 = strCivilNo1;
			if(7 == strCivilNo2.GetLength()) dlg.m_strCivilNo2 = strCivilNo2;
		}
		else
		{
 			CString strCivilNo1, strCivilNo2;
 			GetDlgItemText(IDC_EDIT_CIVILNUM1, strCivilNo1);
 			if(6 == strCivilNo1.GetLength()) dlg.m_strCivilNo1 = strCivilNo1;
 			GetDlgItemText(IDC_EDIT_CIVILNUM2, strCivilNo2);
 			if(7 == strCivilNo2.GetLength()) dlg.m_strCivilNo2 = strCivilNo2;
		}

	}
	if(0 != m_nMode)
	{
		GetDataMgrData(m_strDMAlias, _T("REC_KEY"), dlg.m_strUserKey, 0);
		if(dlg.m_strUserKey.IsEmpty())
		{
			MessageBox(_T("이용자키가 없습니다."), _T("본인인증"), MB_OK|MB_ICONERROR);		
			return;
		}
	}
	dlg.m_strCivilNoDisplayCheck = m_bCivilNoDisplayCheck;
	
	if(TRUE == m_pInfo->m_bKLUse)
	{
		dlg.m_strBeforeCode = m_pSpfWork->m_strCertifyCode;
		dlg.m_strBeforeFailCode = m_pSpfWork->m_strCertifyFailCode;
		dlg.m_strBeforeDate = m_pSpfWork->m_strCertifyDate;
		dlg.m_strBeforeSex = m_pSpfWork->m_strCertifySex;
		dlg.m_strBeforeWorker = m_pSpfWork->m_strCertifyWorker;
		dlg.m_strBeforeName = m_pSpfWork->m_strCertifyName;
		dlg.m_strBeforeCivilNo = m_pSpfWork->m_strCertifyCivilNo;
		dlg.m_bIsKLMemberYN = m_bIsKLMemberYN;
	}

	dlg.DoModal();
	if(!dlg.m_strCertifyCode.IsEmpty())
	{ 
		m_bKLMemeberSearch = dlg.m_bKLMemeberSearch;
		m_strKlUserNo = dlg.m_strKlUserNo;
		
		if(!m_strKlUserNo.IsEmpty())
		{
			
			CString strQuery, strValue;
			if(0 != m_nMode)
			{
				GetDataMgrData(m_strDMAlias, _T("대출자번호"), strValue, 0);	
			}
			if(strValue != m_strKlUserNo)
			{
				CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300"));
				
				strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s' AND KL_MEMBER_YN='Y'"), m_strKlUserNo);

				pDM->GetOneValueQuery(strQuery, strValue);
				if(strValue.IsEmpty()) strValue = _T("0");
				if(0 < _ttoi(strValue))
				{
					strValue.Format(_T("자관의 통합도서 회원입니다.\r\n통합대출자번호 : %s"), m_strKlUserNo);
					AfxMessageBox(strValue);
					m_pSpfWork->m_strCertifyCode = _T("");
					m_pSpfWork->m_strCertifyDate = _T("");
					m_pSpfWork->m_strCertifySex = _T("");
					m_pSpfWork->m_strCertifyWorker = _T("");
					m_pSpfWork->m_strCertifyName = _T("");
					m_pSpfWork->m_strCertifyCivilNo = _T("");
					return;
				}
				CKL_UserManagerDlg dlg2(this);
				dlg2.InitESL_Mgr(_T("SM_KL_USER_MANAGER"));

				INT ids = dlg2.KL_GetMemberInfo(m_strKlUserNo, FALSE);
				
				if(0 != ids)
				{
					m_pSpfWork->m_strCertifyCode = _T("");
					m_pSpfWork->m_strCertifyDate = _T("");
					m_pSpfWork->m_strCertifySex = _T("");
					m_pSpfWork->m_strCertifyWorker = _T("");
					m_pSpfWork->m_strCertifyName = _T("");
					m_pSpfWork->m_strCertifyCivilNo = _T("");
					return;
				}

				m_bIsTCTInput = dlg2.GetIsTCTInput();
				if(m_bIsTCTInput)
 				{
 					if(0==m_nMode) m_nLTMemberSaveMode = 4;	
 					else m_nLTMemberSaveMode = 5; 
 				}
 				else
 				{
 					
 					if(0==m_nMode) m_nLTMemberSaveMode = 1; 
 					
 					else m_nLTMemberSaveMode = 2; 
 				}
											
				ids = KL_SetMemberInfo(dlg2.GetMemberInfoDM());
				if(0 != ids)
				{
					m_pSpfWork->m_strCertifyCode = _T("");
					m_pSpfWork->m_strCertifyDate = _T("");
					m_pSpfWork->m_strCertifySex = _T("");
					m_pSpfWork->m_strCertifyWorker = _T("");
					m_pSpfWork->m_strCertifyName = _T("");
					m_pSpfWork->m_strCertifyCivilNo = _T("");
					return;
				}
			}
		}
		m_pSpfWork->m_strCertifyCode = dlg.m_strCertifyCode;
		m_pSpfWork->m_strCertifyFailCode = _T("");
		m_pSpfWork->m_strCertifyDate = dlg.m_strCertifyDate;
		m_pSpfWork->m_strCertifySex = dlg.m_strCertifySex;
		m_pSpfWork->m_strCertifyWorker = dlg.m_strCertifyWorker;
		m_pSpfWork->m_strCertifyName = dlg.m_strCertifyName;
		m_pSpfWork->m_strCertifyCivilNo = dlg.m_strCertifyCivilNo;
		SetCertifySuccess();
		if(TRUE == m_pInfo->m_bKLUse)
		{
			m_strGPIN_SEX = m_pSpfWork->m_strCertifySex;
		}
		
		CString strBirthDay, strCivilNo;
		GetControlData(m_strCMAlias, _T("생일"), strBirthDay);
		strCivilNo = m_pSpfWork->m_strCertifyCivilNo;
		if(strBirthDay.IsEmpty()) SetBirthDay(strCivilNo);
		
		
		m_strCertCivilNo = dlg.m_strCertifyCivilNo;
	}
	else
	{
		if(!dlg.m_strCertifyFailCode.IsEmpty())
		{ 
			m_pSpfWork->m_strCertifyCode = _T("");
			m_pSpfWork->m_strCertifyFailCode = dlg.m_strCertifyFailCode;
			m_pSpfWork->m_strCertifyDate = dlg.m_strCertifyDate;
			m_pSpfWork->m_strCertifySex = dlg.m_strCertifySex;
			m_pSpfWork->m_strCertifyWorker = dlg.m_strCertifyWorker;
			m_pSpfWork->m_strCertifyName = dlg.m_strCertifyName;
			m_pSpfWork->m_strCertifyCivilNo = dlg.m_strCertifyCivilNo;
			SetCertifyFail();
		}
	}
}


void CBO_LOC_3320::SetCertifySuccess()
{
	m_btnCertify_Y.ShowWindow(TRUE);
	m_btnCertify_N.ShowWindow(FALSE);
	m_btnCertify_Fail.ShowWindow(FALSE);

	
	m_nCertifyState = 1;

	GetDlgItem(IDC_edtUSER7)->SetWindowText(m_pSpfWork->m_strCertifyName);
	GetDlgItem(IDC_edtUSER7)->EnableWindow(FALSE);	
	if(_T("Y") == m_bCivilNoDisplayCheck)
	{
		GetDlgItem(IDC_EDIT_CIVILNUM1)->SetWindowText(m_pSpfWork->m_strCertifyCivilNo.Left(6));
		GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CIVILNUM2)->SetWindowText(m_pSpfWork->m_strCertifyCivilNo.Right(7));
		GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_edtUSER8)->SetWindowText(m_pSpfWork->m_strCertifyCivilNo.Left(6)+_T("-")+m_pSpfWork->m_strCertifyCivilNo.Right(7));
	}

	if (m_bCivilNoDisplayCheck.CompareNoCase(_T("N")) == 0)
	{
		CButton* pBtnM = (CButton*)GetDlgItem(IDC_RADIO_SEX_M);
		CButton* pBtnF = (CButton*)GetDlgItem(IDC_RADIO_SEX_F);
		
		
		pBtnM->EnableWindow(m_pSpfWork->m_strCertifySex.IsEmpty());
		pBtnF->EnableWindow(m_pSpfWork->m_strCertifySex.IsEmpty());

		pBtnM->SetCheck(FALSE);
		pBtnF->SetCheck(FALSE);

		if (m_pSpfWork->m_strCertifySex.CompareNoCase(_T("0")) == 0)
		{
			pBtnM->SetCheck(TRUE);
		}
		if (m_pSpfWork->m_strCertifySex.CompareNoCase(_T("1")) == 0)
		{
			pBtnF->SetCheck(TRUE);
		}
	}
}


void CBO_LOC_3320::SetCertifyFail()
{
	m_btnCertify_Fail.ShowWindow(TRUE);
	m_btnCertify_Y.ShowWindow(FALSE);
	m_btnCertify_N.ShowWindow(FALSE);

	
	m_nCertifyState = 3;

	GetDlgItem(IDC_edtUSER7)->SetWindowText(m_pSpfWork->m_strCertifyName);
	GetDlgItem(IDC_edtUSER7)->EnableWindow(TRUE);
	if(_T("Y") == m_bCivilNoDisplayCheck)
	{
		GetDlgItem(IDC_EDIT_CIVILNUM1)->SetWindowText(m_pSpfWork->m_strCertifyCivilNo.Left(6));
		GetDlgItem(IDC_EDIT_CIVILNUM1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CIVILNUM2)->SetWindowText(m_pSpfWork->m_strCertifyCivilNo.Right(7));
		GetDlgItem(IDC_EDIT_CIVILNUM2)->EnableWindow(TRUE);
		GetDlgItem(IDC_edtUSER8)->SetWindowText(m_pSpfWork->m_strCertifyCivilNo.Left(6)+_T("-")+m_pSpfWork->m_strCertifyCivilNo.Right(7));
	}

	if (m_bCivilNoDisplayCheck.CompareNoCase(_T("N")) == 0)
	{
		CButton* pBtnM = (CButton*)GetDlgItem(IDC_RADIO_SEX_M);
		CButton* pBtnF = (CButton*)GetDlgItem(IDC_RADIO_SEX_F);
		
		pBtnM->EnableWindow(TRUE);
		pBtnF->EnableWindow(TRUE);

		pBtnM->SetCheck(FALSE);
		pBtnF->SetCheck(FALSE);

		if (m_pSpfWork->m_strCertifySex.CompareNoCase(_T("0")) == 0)
		{
			pBtnM->SetCheck(TRUE);
		}
		if (m_pSpfWork->m_strCertifySex.CompareNoCase(_T("1")) == 0)
		{
			pBtnF->SetCheck(TRUE);
		}
	}

	m_ToolTip.DelTool(GetDlgItem(IDC_btn3320Certify_Fail));
	CString strValue;
	strValue.Format(_T("%d"), m_pInfo->m_nCertifyAgency);
	CString strDesc = _T(""), strAction = _T("");
	m_pInfo->GetCertifyFailCodeToDesc(strValue, m_pSpfWork->m_strCertifyFailCode, strDesc, strAction);

	strValue = _T("실패사유 : ") + strDesc + _T("\r\n본인인증을 하기 위해서는 이 버튼을 클릭해 주세요.");
	
	m_ToolTip.AddTool(GetDlgItem(IDC_btn3320Certify_Fail), strValue);
}

int CBO_LOC_3320::CheckDuplecate()
{
	CString strUserKey = _T("");
	if(0 != m_nMode) 
	{
		GetDataMgrData(m_strDMAlias, _T("REC_KEY"), strUserKey, 0);
		if(strUserKey.IsEmpty()) return -1;
	}	
	CString strQuery, strWhere, strValue
		,strTemp = _T("");		

	if(!m_pSpfWork->m_strCertifyCode.IsEmpty())
	{
		if(0 == m_nMode) strTemp.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE IPIN_HASH='%s' AND ROWNUM < 2"), m_pSpfWork->m_strCertifyCode);
		else strTemp.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE (IPIN_HASH='%s' AND user_class <> '3' AND REC_KEY<>%s) AND ROWNUM < 2"), m_pSpfWork->m_strCertifyCode, strUserKey);

		strQuery += strTemp;
	}

	CString strCivilNo = _T("");
	if(_T("Y") == m_bCivilNoDisplayCheck && TRUE == ((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2))->GetCheck())
	{
		CString strCivilNo1, strCivilNo2;
		GetDlgItem(IDC_EDIT_CIVILNUM1)->GetWindowText(strCivilNo1);
		GetDlgItem(IDC_EDIT_CIVILNUM2)->GetWindowText(strCivilNo2);
		strCivilNo.Format(_T("%s%s"), strCivilNo1, strCivilNo2);
		strCivilNo = MakeCivilNumFormat(strCivilNo);

		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
			if(NULL != p3000Api)
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					CStringArray DataArray;
					DataArray.Add(strCivilNo);
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray))
					{
						strCivilNo = DataArray.GetAt(0);
					}
					DataArray.RemoveAll(); DataArray.FreeExtra();
					p3000Api->CloseSocket();
				}
				delete p3000Api;
				p3000Api = NULL;
			}
		}
		if(!strCivilNo.IsEmpty())
		{
			BOOL bCivilNoCheck = TRUE;
			if(0 != m_nMode)
			{
				CString strSavePreviewCivilNo;
				CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300"));
				pDM->GetCellData(_T("주민등록번호"), 0, strSavePreviewCivilNo);
				strSavePreviewCivilNo.TrimLeft();	strSavePreviewCivilNo.TrimRight();
				if(strSavePreviewCivilNo == strCivilNo) bCivilNoCheck = FALSE;
			}
			if(TRUE == bCivilNoCheck)
			{
				if(!strWhere.IsEmpty()) strWhere += _T(" OR ");
				if(0 == m_nMode) strValue.Format(_T("CIVIL_NO='%s'"), strCivilNo);
				else strValue.Format(_T("(CIVIL_NO='%s' AND REC_KEY<>%s)"), strCivilNo, strUserKey);
				strWhere += strValue;
			}
		}
	}

	CString strUserID = _T("");
	GetDlgItem(IDC_edtUSER4)->GetWindowText(strUserID);
	if(_T("[대출자번호로생성]") == strUserID) strUserID = _T("");
	if(!strUserID.IsEmpty())
	{
		if(!strQuery.IsEmpty()) strQuery += _T(" UNION ");
		if(0 == m_nMode) strTemp.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE USER_ID='%s' AND ROWNUM < 2"), strUserID);
		else strTemp.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE (USER_ID='%s' AND REC_KEY<>%s) AND ROWNUM < 2"), strUserID, strUserKey);
		
		strQuery += strTemp;
	}

	CString strName = _T("");
	CString strBirthDay = _T("");

	{
		GetControlData(m_strCMAlias, _T("이름"), strName);
		GetControlData(m_strCMAlias, _T("생일"), strBirthDay);
		if(!strName.IsEmpty() && !strBirthDay.IsEmpty())
		{
			if(!strQuery.IsEmpty()) strQuery += _T(" UNION ");
			if(0 == m_nMode) strTemp.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE (NAME='%s' AND TO_CHAR(BIRTHDAY,'YYYY/MM/DD')='%s') AND ROWNUM < 2"), strName, strBirthDay);
			else strTemp.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE (NAME='%s' AND TO_CHAR(BIRTHDAY,'YYYY/MM/DD')='%s' AND REC_KEY<>%s) AND ROWNUM < 2"), strName, strBirthDay, strUserKey);
			
			strQuery += strTemp;
		}
	}

	CString strPhone0, strPhone1, strPhone2, strHandphone, strIDXHandphone;
	
	((CComboBox*)(GetDlgItem(IDC_cmbUSER5)))->GetWindowText(strPhone0);
	( (CEdit*)GetDlgItem(IDC_edtUSER19) )->GetWindowText(strPhone1);
	( (CEdit*)GetDlgItem(IDC_edtUSER20) )->GetWindowText(strPhone2);
	
	strHandphone.Format(_T("%s-%s-%s"),strPhone0,strPhone1,strPhone2);
	strIDXHandphone.Format(_T("%s%s%s"),strPhone0,strPhone1,strPhone2);
	
	if(!strHandphone.IsEmpty() || !strIDXHandphone.IsEmpty())
	{
		GetControlData(m_strCMAlias, _T("이름"), strName);
		
		if(!strQuery.IsEmpty()) strQuery += _T(" UNION ");		
		if(0 == m_nMode) strTemp.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE (NAME = '%s' AND IDX_HANDPHONE = '%s') AND ROWNUM < 2"),strName, strIDXHandphone);
		else strTemp.Format(_T("SELECT REC_KEY FROM CO_LOAN_USER_TBL WHERE ( (NAME = '%s' AND IDX_HANDPHONE = '%s') AND REC_KEY<>%s) AND ROWNUM < 2"),strName, strIDXHandphone, strUserKey);
		
		strQuery += strTemp;
	}

	if(strQuery.IsEmpty()) return 0;
	CESL_DataMgr pDM;
	pDM.SetCONNECTION_INFO(_T(""));
	int ids = pDM.RestructDataManager(strQuery);

	if(0 > ids) return ids;

	if( pDM.GetRowCount() > 0)
	{
		BOOL bCivilNoDuplecateSkip = FALSE;
		if(FALSE == ((CButton*)GetDlgItem(IDC_btnCHECK_CIVIL_NO2))->GetCheck()) bCivilNoDuplecateSkip = TRUE;
		CUserDuplecateDlg dlg(this);
		dlg.m_nMode = m_nMode;
		dlg.m_strUserKey = strUserKey;
		dlg.m_bCivilNoDuplecateSkip = bCivilNoDuplecateSkip;
		dlg.m_strCivilNo = strCivilNo;
		dlg.m_strUserID = strUserID;
		dlg.m_strName = strName;
		dlg.m_strBirthDay = strBirthDay;
		dlg.m_strCertifyCode = m_pSpfWork->m_strCertifyCode;
		dlg.m_strHandphone			= strIDXHandphone;
		if(IDOK != dlg.DoModal()) return 1;
	}
	return 0;
}


INT CBO_LOC_3320::ShowStatementInfo(CString strStatementAlias)
{
	
	CString strBirth;
	CString strGetData;
	CESL_ControlMgr *pCM = FindCM(_T(""));
	GetControlData(m_strCMAlias, _T("생일"), strBirth);
	
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	CString strUserKey;	
	CString strIpinHash;
	pDM->GetCellData(_T("REC_KEY"), 0, strUserKey);
	pDM->GetCellData(_T("실명인증코드"), 0, strIpinHash);
	
	m_bUnder14YN = FALSE;
	if(10 == strBirth.GetLength())
	{
		CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strGetData);
		CString sCurrentTime = strGetData.Left(10);
		
		INT nBirthYear = _ttoi(strBirth.Left(4));
		INT nBirthMonth = _ttoi(strBirth.Mid(5, 2));
		INT nBirthDay = _ttoi(strBirth.Right(2));
		INT nCurrentYear = _ttoi(sCurrentTime.Left(4));
		INT nCurrentMonth = _ttoi(sCurrentTime.Mid(5, 2));
		INT nCurrentDay = _ttoi(sCurrentTime.Right(2));
		if(14 > (nCurrentYear-nBirthYear))
		{
			m_bUnder14YN = TRUE;
		}
		else if(14 == (nCurrentYear-nBirthYear) && 0 > (nCurrentMonth-nBirthMonth))
		{
			m_bUnder14YN = TRUE;	
		}	
		else if(14 == (nCurrentYear-nBirthYear) && 0 == (nCurrentMonth-nBirthMonth) && 0 > (nCurrentDay-nBirthDay))
		{
			m_bUnder14YN = TRUE;	
		}	
		else
		{
			m_bUnder14YN = FALSE;
		}
	}

	KL_StatementDlg dlg(this);
	if((m_pInfo->m_bCertifyUse && _T("") != m_pSpfWork->m_strCertifyCode) || _T("") != m_strGPIN_HASH)
	{
		dlg.m_bRealNameCertYN = TRUE;
	}

	if(_T("개인정보 수집/이용") == strStatementAlias)
	{
		m_strStatementAlias = strStatementAlias;
		dlg.m_nMode = 0;
		dlg.m_strStatementAlias = strStatementAlias;
		dlg.m_bUnder14YN = m_bUnder14YN;
		dlg.m_bIsKLMode = FALSE;
		dlg.m_bIsCertifyMode = TRUE;
		dlg.m_strUserKey = strUserKey;
		
		dlg.m_bIsKLMode = g_bEnableCertify;
		if(IDOK != dlg.DoModal()) return -1000;
	}
	else if(_T("책이음서비스") == strStatementAlias) 
	{
		m_strStatementAlias = strStatementAlias;
		dlg.m_nMode = 0;
		dlg.m_strStatementAlias = strStatementAlias;
		dlg.m_bUnder14YN = m_bUnder14YN;
		dlg.m_bIsKLMode = TRUE;
		dlg.m_strUserKey = strUserKey;
		dlg.m_bIsKLMode = g_bEnableCertify;
		if(_T("") != m_strGPIN_HASH)
		{
	    	dlg.m_strGpinHash = m_strGPIN_HASH;
		}
		else if(_T("") != strIpinHash)
		{
	    	dlg.m_strGpinHash = strIpinHash;
		}
		if(IDOK != dlg.DoModal()) return -1001;
	}
	m_bIsAgreeYN = dlg.m_bIsAgreeYN;
	m_strAgentName = dlg.m_strAgentName;
	m_strAgentRelation = dlg.m_strAgentRelation;
	m_strAgentNote = dlg.m_strAgentNote;
	m_strAgreeType = dlg.m_strAgreeType;
	m_strAgreeDate = dlg.m_strAgreeDate;
	m_strExpireDate = dlg.m_strExpireDate;

	CString strTmp = _T("");
	dlg.GetTmpUserKey(strTmp);
	if ( strTmp.GetLength() > 0 ) m_strTmpImgUserKey = strTmp;

	SetDataMgrData(m_strDMAlias, _T("대리인명"), m_strAgentName, 0);
	SetDataMgrData(m_strDMAlias, _T("대리인관계"), m_strAgentRelation, 0);
	SetDataMgrData(m_strDMAlias, _T("대리인비고"), m_strAgentNote, 0);
	if(m_bIsAgreeYN)
	{
		SetDataMgrData(m_strDMAlias, _T("동의약관"), dlg.m_strStatementAlias, 0);
	}

	return 0;
}


INT CBO_LOC_3320::InsertAgreementInfo(CString strUserKey, CString strStatementAlias)
{
	if(!m_pInfo->m_bAgreeInfoRecordYN && !m_pInfo->m_bKLUse)
	{
		return 0;
	}
	CString strQuery =_T("");
	CString strAgentKey =_T("");
	CString strWork = m_pParent->GetWorkLogMsg(_T("이용약관 동의"),__WFILE__,__LINE__);
	CString strGetDuplication;

	CESL_DataMgr* pDM = FindDM(m_strDMAlias);

	if(_T("") != strStatementAlias) 
	{
		pDM->StartFrame();		
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_AGREEMENT_INFO_TBL WHERE USER_KEY = %s"), strUserKey);
		pDM->GetOneValueQuery(strQuery, strGetDuplication);
		if(_T("0") != strGetDuplication)
		{	
			strQuery.Format(_T("DELETE CO_AGREEMENT_INFO_TBL WHERE USER_KEY = %s;"), strUserKey);
			pDM->AddFrame(strQuery);
		}
		
		if(0 < m_pInfo->m_nUserInfoRenewalDay)
		{
			strQuery.Format(_T("INSERT INTO CO_AGREEMENT_INFO_TBL(USER_KEY, STATEMENT_ALIAS, AGREE_TYPE, AGREE_YN, AGREE_DATE, EXPIRE_DATE, LIB_CODE, LIB_NAME, WORKER, FIRST_WORK) VALUES(%s, '%s', '3','Y', '%s', '%s', '%s', '%s', 'KOLASIII', '%s');"), strUserKey, strStatementAlias, m_strAgreeDate, m_strExpireDate, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork);
		}
		else
		{
			strQuery.Format(_T("INSERT INTO CO_AGREEMENT_INFO_TBL(USER_KEY, STATEMENT_ALIAS, AGREE_TYPE, AGREE_YN, AGREE_DATE, LIB_CODE, LIB_NAME, WORKER, FIRST_WORK) VALUES(%s, '%s', '3','Y', '%s', '%s', '%s', 'KOLASIII', '%s');"), strUserKey, strStatementAlias, m_strAgreeDate, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork);
		}
		pDM->AddFrame(strQuery);
		pDM->SendFrame();
		pDM->EndFrame();

		pDM->SetCellData(_T("동의약관"), strStatementAlias, 0);
		pDM->SetCellData(_T("동의방법"), _T("3"), 0);
		pDM->SetCellData(_T("동의여부"), _T("Y"), 0);
	}
	
	if(_T("") != m_strAgentName) 
	{ 
		pDM->StartFrame();
		strQuery.Format(_T("SELECT COUNT(1) FROM CO_AGENT_INFO_TBL WHERE USER_KEY = %s"), strUserKey);
		pDM->GetOneValueQuery(strQuery, strGetDuplication);
		if(_T("0") != strGetDuplication)
		{	
			strQuery.Format(_T("DELETE CO_AGENT_INFO_TBL WHERE USER_KEY = %s;"), strUserKey);
			pDM->AddFrame(strQuery);
		}
		strQuery.Format(_T("INSERT INTO CO_AGENT_INFO_TBL(USER_KEY, NAME, RELATION, NOTE, CERT_DATE, LIB_CODE, LIB_NAME, WORKER, FIRST_WORK) VALUES(%s, '%s', '%s', '%s', SYSDATE, '%s', '%s', 'KOLASIII', '%s');"), strUserKey, m_strAgentName, m_strAgentRelation, m_strAgentNote, m_pInfo->LIB_CODE, m_pInfo->LIB_NAME, strWork);	
		pDM->AddFrame(strQuery);
		pDM->SendFrame();
		pDM->EndFrame();

		pDM->SetCellData(_T("대리인명"), m_strAgentName, 0);
		pDM->SetCellData(_T("대리인관계"), m_strAgentRelation, 0);
		pDM->SetCellData(_T("대리인비고"), m_strAgentNote, 0);
	}

	return 0;
}

BOOL CBO_LOC_3320::CheckUnder14Years(CString strCivilNo)
{
	strCivilNo.Replace(_T("-"),_T(""));
	if(13 != strCivilNo.GetLength())
	{
		return FALSE;
	}

	CString strCivilType = strCivilNo.Mid(6, 1);
	CString strBirthYear = strCivilNo.Left(2);
	INT nBirthYear = 1900;
	INT nBirthMonth = _ttoi(strCivilNo.Mid(2, 2));
	INT nBirthDay = _ttoi(strCivilNo.Mid(4, 2));

	if(_T("1") == strCivilType || _T("2") == strCivilType || _T("3") == strCivilType || _T("4") == strCivilType)
	{
		nBirthYear = 1900 + _ttoi(strBirthYear);
	}
	else if(_T("5") == strCivilType || _T("6") == strCivilType || _T("7") == strCivilType || _T("8") == strCivilType)
	{
		nBirthYear = 2000 + _ttoi(strBirthYear);
	}
	else if(_T("9") == strCivilType || _T("0") == strCivilType)
	{
		nBirthYear = 1800 + _ttoi(strBirthYear);
	}
	CString strGetData;
	CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(this, strGetData);
	CString sCurrentTime = strGetData.Left(10);

	INT nCurrentYear = _ttoi(sCurrentTime.Left(4));
	INT nCurrentMonth = _ttoi(sCurrentTime.Mid(5, 2));
	INT nCurrentDay = _ttoi(sCurrentTime.Right(2));

	if(14 > (nCurrentYear-nBirthYear))
	{
		return TRUE;
	}
	else if(14 == (nCurrentYear-nBirthYear) && 0 > (nCurrentMonth-nBirthMonth))
	{
		return TRUE;	
	}	
	else if(14 == (nCurrentYear-nBirthYear) && 0 == (nCurrentMonth-nBirthMonth) && 0 > (nCurrentDay-nBirthDay))
	{
		return TRUE;	
	}	
	
	return FALSE;
}



void CBO_LOC_3320::OnbtnAGREEMENTINFO() 
{
	if ( m_pLoanMangeValue->m_sLoanUserAccessMode.IsEmpty() || m_pLoanMangeValue->m_sLoanUserAccessMode == _T("1") )
	{
		AfxMessageBox(_T("접근권한이 없습니다."));
		return;
	}

	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL==pDM) return;
	CString strUserKey;
	CString strAgentName;
	CString strAgentRelation;
	CString strAgentNote;
	CString strIsKLmemberYN;
	CString strQuery;
	CString strGetDuplication;
	CString strWork = this->GetWorkLogMsg(_T("이용약관 소급동의"),__WFILE__,__LINE__);
	CString strAgreeType;
	CString strUserNo;
	CString strGpinHash;
	CString strAgreementAlias;

	pDM->GetCellData(_T("REC_KEY"), 0, strUserKey);
	pDM->GetCellData(_T("대리인명"), 0, strAgentName);
	pDM->GetCellData(_T("대리인관계"), 0, strAgentRelation);
	pDM->GetCellData(_T("대리인비고"), 0, strAgentNote);
	pDM->GetCellData(_T("동의방법"), 0, strAgreeType);
	pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	pDM->GetCellData(_T("GPIN_HASH"), 0, strGpinHash);
	pDM->GetCellData(_T("동의약관"), 0, strAgreementAlias);
	pDM->GetCellData(_T("통합회원전환여부"), 0, strIsKLmemberYN);
	
	CString strBirth;
	CString strGetData;
	CESL_ControlMgr *pCM = FindCM(_T(""));
	GetControlData(m_strCMAlias, _T("생일"), strBirth);	
	m_bUnder14YN = FALSE;
	if(10 == strBirth.GetLength())
	{
		CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime((CESL_Mgr*)this, strGetData);
		CString sCurrentTime = strGetData.Left(10);
		INT nBirthYear = _ttoi(strBirth.Left(4));
		INT nBirthMonth = _ttoi(strBirth.Mid(5, 2));
		INT nBirthDay = _ttoi(strBirth.Right(2));
		INT nCurrentYear = _ttoi(sCurrentTime.Left(4));
		INT nCurrentMonth = _ttoi(sCurrentTime.Mid(5, 2));
		INT nCurrentDay = _ttoi(sCurrentTime.Right(2));
		if(14 > (nCurrentYear-nBirthYear))
		{
			m_bUnder14YN = TRUE;
		}
		else if(14 == (nCurrentYear-nBirthYear) && 0 > (nCurrentMonth-nBirthMonth))
		{
			m_bUnder14YN = TRUE;	
		}	
		else if(14 == (nCurrentYear-nBirthYear) && 0 == (nCurrentMonth-nBirthMonth) && 0 > (nCurrentDay-nBirthDay))
		{
			m_bUnder14YN = TRUE;	
		}	
		else
		{
			m_bUnder14YN = FALSE;
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
	
	dlg.m_bUnder14YN = m_bUnder14YN;
	
	dlg.m_bIsKLMode = (m_pInfo->m_bCertifyUse || g_strIpinFlag.CompareNoCase(_T("Y")) == 0 || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0);
	
	if(TRUE == m_pParent->m_pInfo->m_bKLUse && _T("Y") == strIsKLmemberYN)
	{
		dlg.m_bIsKLMemberYN = TRUE;
		
		dlg.m_strStatementAlias = _T("책이음서비스");
		
		if(_T("책이음서비스") == strAgreementAlias)
		{
			dlg.m_bIsAgreeYN = TRUE;
			dlg.m_strAgreeType = strAgreeType;
		}
		else
		{
			
			dlg.m_bIsAgreeYN = FALSE;
			dlg.m_strAgreeType = _T("0");
		}
		
		if(m_pInfo->m_bCertifyUse || g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0 || g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)

		{
			if(_T("") != m_pSpfWork->m_strCertifyCode)
			{
				dlg.m_bRealNameCertYN = TRUE;
			}
			else
			{
				dlg.m_bRealNameCertYN = FALSE;
			}
		}
		else
		{
			dlg.m_bRealNameCertYN = TRUE;
		}

		
		if(m_bUnder14YN)
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
	
	else if(TRUE == m_pParent->m_pInfo->m_bAgreeInfoRecordYN && _T("Y") != strIsKLmemberYN )
	{
		dlg.m_bIsKLMemberYN = FALSE;
		dlg.m_strStatementAlias = _T("개인정보 수집/이용");
		if(_T("개인정보 수집/이용") == strAgreementAlias)
		{		
			dlg.m_bIsAgreeYN = TRUE;
			dlg.m_strAgreeType = strAgreeType;
		}
	
		else
		{			
			dlg.m_bIsAgreeYN = FALSE;
			dlg.m_strAgreeType = _T("0");
		}
		
		if(_T("") != m_pSpfWork->m_strCertifyCode)
		{
			dlg.m_bRealNameCertYN = TRUE;
		}
		else
		{
			dlg.m_bRealNameCertYN = FALSE;
		}
		
		if(m_bUnder14YN)
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
	
	if(m_pInfo->m_bCertifyUse && _T("") == m_pSpfWork->m_strCertifyCode)
	{
		StartUserCertify();
	}
	
	pDM->SetCellData(_T("대리인명"), dlg.m_strAgentName, 0);
	pDM->SetCellData(_T("대리인관계"), dlg.m_strAgentRelation, 0);
	pDM->SetCellData(_T("대리인비고"), dlg.m_strAgentNote, 0);
	pDM->SetCellData(_T("동의방법"), dlg.m_strAgreeType, 0);
	pDM->SetCellData(_T("동의약관"), dlg.m_strStatementAlias, 0);
	return;
}


INT CBO_LOC_3320::MakeAndShowStatement(CString strIsKLMemberYN) 
{
	
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	CString strAgreeStatementAlias;
	CString strAgentName;
	
	pDM->GetCellData(_T("동의약관"), 0, strAgreeStatementAlias);
	pDM->GetCellData(_T("대리인명"), 0, strAgentName);

	if(TRUE == m_pInfo->m_bKLUse && _T("Y") == strIsKLMemberYN)
	{
		
		if(_T("책이음서비스") == strAgreeStatementAlias)
		{
			return 100;
		}
		else
		{
			if(0 > ShowStatementInfo(_T("책이음서비스")))
			{
				return -1000;
			}
		}
	}
	else if(_T("Y") != strIsKLMemberYN && TRUE == m_pInfo->m_bAgreeInfoRecordYN)
	{	
		if(_T("개인정보 수집/이용") == strAgreeStatementAlias)
		{
			return 200;
		}
		else
		{
			if(0 > ShowStatementInfo(_T("개인정보 수집/이용")))
			{
				return -1001;
			}
		}
	}
	return 0;
}				


void CBO_LOC_3320::OnbtnCHANGEPASSWORD() 
{
	
	CString strUserID = _T(""), strPassword = _T("");	
	GetControlData( m_strCMAlias, _T("사용자ID"), strUserID );
	GetControlData( m_strCMAlias, _T("비밀번호"), strPassword );
	if( strUserID.IsEmpty() && !strPassword.IsEmpty())
	{
		if(IDYES == MessageBox(
		_T("아이디가 입력되지 않습니다.\r\n")
		_T("기존 비밀번호를 삭제하시겠습니까?\r\n")
		_T(" - 예 : 비밀번호를 삭제합니다\r\n")
		_T(" - 아니오 : 비밀번호를 수정합니다"), _T("비밀번호삭제확인"), MB_YESNO|MB_ICONQUESTION))
		{
			m_strCryptoPassword = _T("");
			GetDlgItem(IDC_edtUSER5)->SetWindowText(m_strCryptoPassword);
			GetDlgItem(IDC_edtUSER6)->SetWindowText(m_strCryptoPassword);
			SetDataMgrData(m_strDMAlias, _T("비밀번호"), m_strCryptoPassword, 0);
			return ;
		}
	}
	
 	CESL_DataMgr *pDM = FindDM( m_strDMAlias );
	if(NULL == pDM)
	{
		AfxMessageBox(_T("FindDM Error"), MB_ICONSTOP);
	}
	CString strUserKey = pDM->GetCellData(_T("REC_KEY"), 0);

	CKlEditPassDlg dlg(this);
	dlg.m_nMode = 1;
	dlg.m_strUserKey = strUserKey;
	
	if(IDOK == dlg.DoModal())
	{
		GetDlgItem(IDC_edtUSER5)->SetWindowText(m_strCryptoPassword);
		GetDlgItem(IDC_edtUSER6)->SetWindowText(m_strCryptoPassword);
		SetDataMgrData(m_strDMAlias, _T("비밀번호"), m_strCryptoPassword, 0);
	}
}


void CBO_LOC_3320::OnbtnCHANGECARDPW() 
{
 	CESL_DataMgr *pDM = FindDM( m_strDMAlias );
	if(NULL == pDM)
	{
		AfxMessageBox(_T("FindDM Error"), MB_ICONSTOP);
	}
	CString strUserKey = pDM->GetCellData(_T("REC_KEY"), 0);

	CKlEditPassDlg dlg(this);
	dlg.m_nMode = 2;
	dlg.m_strUserKey = strUserKey;
	
	if(IDOK == dlg.DoModal())
	{
		GetDlgItem(IDC_edtUSER_35)->SetWindowText(m_strCryptoCardPassword);
		SetDataMgrData(m_strDMAlias, _T("회원증비밀번호"), m_strCryptoCardPassword, 0);
	}
}



BOOL CBO_LOC_3320::IsPrivacyState()
{
	INT ids = 0;
	
	CESL_DataMgr* pDM = FindDM(m_strDMAlias);
	if(NULL == pDM) return FALSE;
	INT nKlMemberMode = 0;
	CString strSelectYN;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cmbKLMemberYN);
	pCombo->GetLBText(pCombo->GetCurSel(), strSelectYN);
	if(0 == m_nMode)
	{
		if(_T("Y") == strSelectYN) nKlMemberMode = 2;
		else nKlMemberMode = 1;
	}
	else
	{
		CString strRecKey;
		pDM->GetCellData(_T("REC_KEY"), 0, strRecKey);
		CString strQuery = _T("");
		strQuery.Format(_T("SELECT KL_MEMBER_YN FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strRecKey);
		CString strValue = _T("");
		ids = pDM->GetOneValueQuery(strQuery, strValue);
		if(0 > ids && -4007 != ids) return FALSE;
		if(_T("Y") != strValue)
		{
			if(_T("Y") == strSelectYN) nKlMemberMode = 2;
			else nKlMemberMode = 1;
		}
		else
		{
			nKlMemberMode = 0;
		}
	}
	if(0 == nKlMemberMode && !m_pSpfWork->m_strCertifyCode.IsEmpty())
	{ 
		return TRUE;
	}
	else if(0 == nKlMemberMode && m_pSpfWork->m_strCertifyCode.IsEmpty())
	{ 
	
		if(IDYES == MessageBox(
			_T("본인인증 대상 이용자 입니다.\r\n")
			_T("본인인증을 수행하고 저장하시겠습니까?\r\n")
			_T(" - 예 : 본인인증을 수행하고 저장합니다\r\n")
			_T(" - 아니오 : 본인인증을 수행하지 않고 저장합니다"), _T("본인인증확인"), MB_YESNO|MB_ICONQUESTION))	
		{
			
			StartUserCertify();
			return TRUE;
		}
		else
		{
			return TRUE;
		}
	}
	else if(1 == nKlMemberMode && !m_pSpfWork->m_strCertifyCode.IsEmpty())
	{ 
		return TRUE;
	}
	else if(1 == nKlMemberMode && m_pSpfWork->m_strCertifyCode.IsEmpty())
	{ 
	
		if (!g_bEnableCertify)
			return TRUE;

		if(IDYES == MessageBox(
			_T("본인인증 대상 이용자 입니다.\r\n")
			_T("본인인증을 수행하고 저장하시겠습니까?\r\n")
			_T(" - 예 : 본인인증을 수행하고 저장합니다\r\n")
			_T(" - 아니오 : 본인인증을 수행하지 않고 저장합니다"), _T("본인인증확인"), MB_YESNO|MB_ICONQUESTION))
		{
			
			StartUserCertify();
			
			
			if(0 == m_nMode) if(m_pSpfWork->m_strCertifyCode.IsEmpty()) return FALSE;

			return TRUE;
		}
		else
		{
			return TRUE;
		}
	}
	else if(2 == nKlMemberMode && !m_pSpfWork->m_strCertifyCode.IsEmpty())
	{ 
		
		if(FALSE == m_bKLMemeberSearch)
		{ 
			CString strCivilNo;
			
			if(0 == m_nMode)
			{ 
				
				strCivilNo = m_strCertCivilNo;
			}
			else
			{ 
				
				pDM->GetCellData(_T("주민등록번호_CRYPTO"), 0, strCivilNo);
				this->ESL_DES_Decryption(strCivilNo, strCivilNo);
				strCivilNo.Replace(_T("-"), _T(""));
			}
			
			if(strCivilNo.IsEmpty() && (m_bCivilNoDisplayCheck.CompareNoCase(_T("Y")) == 0 || m_pInfo->m_bCertifyUse))
			{
				if(IDYES == MessageBox(
					_T("개인정보보호법 개정에 따라 주민번호 관리 방식이 변경되어\r\n")
					_T("주민등록번호를 폐기하였으니 책이음회원 전환을 위하여\r\n")
					_T("본인인증을 해주시기 바랍니다. (신분증 제출 요구)\r\n\r\n")
					_T("본인인증을 수행하고 저장하시겠습니까?\r\n")
					_T(" - 예 : 본인인증을 수행하고 저장합니다\r\n")
					_T(" - 아니오 : 본인인증을 수행하지 않고 저장을 취소합니다."), _T("본인인증확인"), MB_YESNO|MB_ICONQUESTION))
				{
					
					StartUserCertify();
					if(!m_pSpfWork->m_strCertifyCode.IsEmpty())
					{ 
						if(0==m_nMode) m_nLTMemberSaveMode = 1; 
						else m_nLTMemberSaveMode = 2; 
						return TRUE;
					}
					else
					{ 
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}

			
			if(strCivilNo.IsEmpty())
				strCivilNo = _T("000000-0000000");

			CString strUserNo;
			CKL_UserManagerDlg dlg(this);
			dlg.InitESL_Mgr(_T("SM_KL_USER_MANAGER"));
			ids = dlg.GetKlUserSearch(m_pSpfWork->m_strCertifyCode, strCivilNo, strUserNo);
			if(0 == ids)
			{
				if(!strUserNo.IsEmpty())
				{ 
					
					CString strQuery;
					
					strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s' AND KL_MEMBER_YN='Y'"), strUserNo);

					CString strValue;
					pDM->GetOneValueQuery(strQuery, strValue);
					if(strValue.IsEmpty()) strValue = _T("0");
					if(0 < _ttoi(strValue))
					{
						strValue.Format(_T("자관의 통합도서 회원입니다.\r\n통합대출자번호 : %s"), strUserNo);
						AfxMessageBox(strValue);
						return FALSE;
					}

					
					ids = dlg.KL_GetMemberInfo(strUserNo, FALSE);					
					

					if(0 != ids) return FALSE;
					
					ids = KL_SetMemberInfo(dlg.GetMemberInfoDM());
					if(0 != ids) return FALSE;
					if(0==m_nMode) m_nLTMemberSaveMode = 4;	
 					else m_nLTMemberSaveMode = 5; 
					return TRUE;
				}
				else
				{ 
					if(0==m_nMode) m_nLTMemberSaveMode = 1; 
					else m_nLTMemberSaveMode = 2; 
					return TRUE;
				}
			}
			else
			{ 
				MessageBox(_T("통합도서 회원으로 가입여부 조회에 실패하였습니다."), _T("가입여부확인실패"), MB_OK|MB_ICONSTOP);
				return FALSE;
			}
		}
		else
		{
			if(0==m_nMode) m_nLTMemberSaveMode = 1; 
			else m_nLTMemberSaveMode = 2; 
			return TRUE;
		}
	}
	else if(2 == nKlMemberMode && m_pSpfWork->m_strCertifyCode.IsEmpty())
	{ 
	
		
		if(IDYES == MessageBox(
			_T("책이음회원으로 가입하기 위해서는 본인인증에 성공해야합니다.\r\n")
			_T("본인인증을 수행하고 저장하시겠습니까?\r\n")
			_T(" - 예 : 본인인증을 수행하고 저장합니다\r\n")
			_T(" - 아니오 : 본인인증을 수행하지 않고 저장을 취소합니다."), _T("본인인증확인"), MB_YESNO|MB_ICONQUESTION))

		{
			
			StartUserCertify();
			if(!m_pSpfWork->m_strCertifyCode.IsEmpty())
			{ 
				if(0==m_nMode) m_nLTMemberSaveMode = 1; 
				else m_nLTMemberSaveMode = 2; 
				return TRUE;
			}
			else
			{ 
				return FALSE;
			}
		}
		else
		{ 
			return FALSE;
		}
	}
	else
	{ 
		MessageBox(_T("정의되지 않은 예외사항"), _T("저장가능여부확인"), MB_OK|MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}
VOID CBO_LOC_3320::StartTotalCertify(int nMode)
{
	CCertifyMainDlg dlg(this);
	if(nMode == 1)
	{
		dlg.m_strTitle = _T("개명요청을 위한 개인인증");
	}
	
	dlg.SetUsedMypinService(g_strMypinCertifyFlag.CompareNoCase(_T("Y")) == 0);
	dlg.SetUsedPhoneService(g_strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0);
	dlg.SetUserMode((m_nMode == 0) ? 0 : 1);

	
	if (m_nMode != 0)
	{
		CString strUserKey;
		GetDataMgrData(m_strDMAlias, _T("REC_KEY"), strUserKey, 0);
		if (strUserKey.IsEmpty())
		{
			MessageBox(_T("이용자키가 없습니다."), _T("본인인증"), MB_OK|MB_ICONERROR);
			return;
		}

		dlg.SetUserKey(strUserKey);
	}

	if (m_pInfo->m_bKLUse)
	{
		dlg.SetCertifyCI(m_pSpfWork->m_strCertifyCode);
		dlg.SetCertifyFailCode(m_pSpfWork->m_strCertifyFailCode);
		dlg.SetCertifyDate(m_pSpfWork->m_strCertifyDate);
		dlg.SetCertifyWorker(m_pSpfWork->m_strCertifyWorker);
		dlg.SetKLMemberYN(m_bIsKLMemberYN);
	}
	dlg.SetCertifyGender(m_pSpfWork->m_strCertifySex);

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
		m_bKLMemeberSearch = dlg.GetKLMemberSearch();
		m_strKlUserNo = dlg.GetKLMemberUserNo();

		
		if (!m_strKlUserNo.IsEmpty())
		{
			
			CString strQuery, strValue;
			if (0 != m_nMode)
			{
				GetDataMgrData(m_strDMAlias, _T("대출자번호"), strValue, 0);	
			}

			if (strValue != m_strKlUserNo)
			{
				CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300"));
				
				strQuery.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL WHERE USER_NO='%s' AND KL_MEMBER_YN='Y'"), m_strKlUserNo);
				pDM->GetOneValueQuery(strQuery, strValue);

				if (!strValue.IsEmpty() && 0 < _ttoi(strValue))
				{
					strValue.Format(_T("자관의 통합도서 회원입니다.\r\n통합대출자번호 : %s"), m_strKlUserNo);
					AfxMessageBox(strValue);
					m_pSpfWork->m_strCertifyCode = _T("");
					m_pSpfWork->m_strCertifyDate = _T("");
					m_pSpfWork->m_strCertifySex = _T("");
					m_pSpfWork->m_strCertifyWorker = _T("");
					m_pSpfWork->m_strCertifyName = _T("");
					m_pSpfWork->m_strCertifyCivilNo = _T("");
					return;
				}

				CKL_UserManagerDlg dlg2(this);
				dlg2.InitESL_Mgr(_T("SM_KL_USER_MANAGER"));
				
				INT ids = dlg2.KL_GetMemberInfo(m_strKlUserNo, FALSE);	

				if (0 != ids)
				{
					m_pSpfWork->m_strCertifyCode = _T("");
					m_pSpfWork->m_strCertifyDate = _T("");
					m_pSpfWork->m_strCertifySex = _T("");
					m_pSpfWork->m_strCertifyWorker = _T("");
					m_pSpfWork->m_strCertifyName = _T("");
					m_pSpfWork->m_strCertifyCivilNo = _T("");
					return;
				}

				m_bIsTCTInput = dlg2.GetIsTCTInput();
				if(m_bIsTCTInput)
 				{
 					if(0==m_nMode) m_nLTMemberSaveMode = 4;	
 					else m_nLTMemberSaveMode = 5; 
 				}
 				else
 				{
 					
 					if(0==m_nMode) m_nLTMemberSaveMode = 1; 
 					
 					else m_nLTMemberSaveMode = 2; 
 				}

				
				ids = KL_SetMemberInfo(dlg2.GetMemberInfoDM());
				if (0 != ids)
				{
					m_pSpfWork->m_strCertifyCode = _T("");
					m_pSpfWork->m_strCertifyDate = _T("");
					m_pSpfWork->m_strCertifySex = _T("");
					m_pSpfWork->m_strCertifyWorker = _T("");
					m_pSpfWork->m_strCertifyName = _T("");
					m_pSpfWork->m_strCertifyCivilNo = _T("");
					return;
				}
			}
		}

		m_pSpfWork->m_strCertifyCode = dlg.GetCertifyCI();
		m_pSpfWork->m_strCertifyFailCode = _T("");
		m_pSpfWork->m_strCertifyDate = dlg.GetCertifyDate();
		m_pSpfWork->m_strCertifySex = dlg.GetCertifyGender();
		m_pSpfWork->m_strCertifyWorker = dlg.GetCertifyWorker();
		m_pSpfWork->m_strCertifyName = dlg.GetUserName();
		m_pSpfWork->m_strCertifyCivilNo = _T("");
		if(TRUE == m_pInfo->m_bKLUse)
		{
			m_strGPIN_SEX = m_pSpfWork->m_strCertifySex;
		}

		SetCertifySuccess();

		
		CString strBirthDay;
		GetControlData(m_strCMAlias, _T("생일"), strBirthDay);
		
		
		if (strBirthDay.IsEmpty() || strBirthDay.Compare(dlg.GetBirthDay()) != 0 )	
		{
			SetControlData(m_strCMAlias, _T("생일"), dlg.GetBirthDay());
		}
	
		ViewHandPhoneNum(dlg.GetHandPhone());
	}
	else
	{
		
		if (!dlg.GetCertifyFailCode().IsEmpty())
		{
			m_pSpfWork->m_strCertifyCode = _T("");
			m_pSpfWork->m_strCertifyFailCode = dlg.GetCertifyFailCode();
			m_pSpfWork->m_strCertifyDate = dlg.GetCertifyDate();
			m_pSpfWork->m_strCertifySex = dlg.GetCertifyGender();
			m_pSpfWork->m_strCertifyWorker = dlg.GetCertifyWorker();
			m_pSpfWork->m_strCertifyName = dlg.GetUserName();
			m_pSpfWork->m_strCertifyCivilNo = _T("");
			SetCertifyFail();
		}
	}
}

void CBO_LOC_3320::GetLocUserData(ST_LOC_USER_INFO &stLocUserInfo)
{
	CString strTmp = _T("");

	((CEdit*)GetDlgItem(IDC_edtUSER1))->GetWindowText(stLocUserInfo.strUserNo);
	((CEdit*)GetDlgItem(IDC_edtUSER4))->GetWindowText(stLocUserInfo.strUserID);
	
	((CEdit*)GetDlgItem(IDC_edtUSER6))->GetWindowText(stLocUserInfo.strPwd);
	((CEdit*)GetDlgItem(IDC_edtUSER_35))->GetWindowText(stLocUserInfo.strLCPwd);
	((CEdit*)GetDlgItem(IDC_edtUSER28))->GetWindowText(stLocUserInfo.strFaID);

	stLocUserInfo.strEmail = _T("");
	((CEdit*)GetDlgItem(IDC_edtUSER21))->GetWindowText(strTmp);
	stLocUserInfo.strEmail + strTmp;
	((CComboBox*)GetDlgItem(IDC_cmbUSER6))->GetWindowText(strTmp);
	stLocUserInfo.strEmail + strTmp;
	

	((CEdit*)GetDlgItem(IDC_edtUSER7))->GetWindowText(stLocUserInfo.strName);

	stLocUserInfo.strPhone = _T("");
	((CEdit*)GetDlgItem(IDC_cmbUSER5))->GetWindowText(strTmp);
	stLocUserInfo.strPhone += strTmp;	
	((CEdit*)GetDlgItem(IDC_edtUSER19))->GetWindowText(strTmp);
	stLocUserInfo.strPhone += strTmp;
	((CEdit*)GetDlgItem(IDC_edtUSER20))->GetWindowText(strTmp);
	stLocUserInfo.strPhone += strTmp;

	stLocUserInfo.bMan = ((CButton*)GetDlgItem(IDC_RADIO_SEX_M))->GetCheck();

	stLocUserInfo.strBirth = _T("");
	((CEdit*)GetDlgItem(IDC_edtUSER33))->GetWindowText(strTmp);
	stLocUserInfo.strBirth += strTmp;
	((CComboBox*)GetDlgItem(IDC_cmbUSER8))->GetWindowText(strTmp);
	stLocUserInfo.strBirth += strTmp;


	GetControlData( m_strCMAlias , _T("회원관리구분") , stLocUserInfo.strManageCode );
	GetControlData( m_strCMAlias , _T("소속") , stLocUserInfo.strPositionCode );
	GetControlData( m_strCMAlias , _T("직급") , stLocUserInfo.strClassCode );	
	GetControlData( m_strCMAlias , _T("자료실") , stLocUserInfo.strShelfLocCode );	

	((CComboBox*)GetDlgItem(IDC_cmbUSER4))->GetWindowText(stLocUserInfo.strMemberClass);

	((CComboBox*)GetDlgItem(IDC_cmbUSER3))->GetWindowText(stLocUserInfo.strSendArea);
	((CComboBox*)GetDlgItem(IDC_edtUSER9))->GetWindowText(stLocUserInfo.strHZipCode);

	((CComboBox*)GetDlgItem(IDC_edtUSER10))->GetWindowText(stLocUserInfo.strHAddr);
	((CComboBox*)GetDlgItem(IDC_edtUSER11))->GetWindowText(stLocUserInfo.strHTel);


	((CComboBox*)GetDlgItem(IDC_edtUSER12))->GetWindowText(stLocUserInfo.strWZipCode);
	((CComboBox*)GetDlgItem(IDC_edtUSER13))->GetWindowText(stLocUserInfo.strWAddr);
	((CComboBox*)GetDlgItem(IDC_edtUSER14))->GetWindowText(stLocUserInfo.strWTel);
	((CComboBox*)GetDlgItem(IDC_edtUSER_34))->GetWindowText(stLocUserInfo.strWName);
	((CComboBox*)GetDlgItem(IDC_edtUSER29))->GetWindowText(stLocUserInfo.strWDepartName);

	((CComboBox*)GetDlgItem(IDC_edtUSER16))->GetWindowText(stLocUserInfo.strSCName);

	stLocUserInfo.strSCClass = _T("");
	((CComboBox*)GetDlgItem(IDC_edtUSER16))->GetWindowText(strTmp);
	stLocUserInfo.strSCClass + strTmp;
	((CComboBox*)GetDlgItem(IDC_edtUSER18))->GetWindowText(strTmp);
	stLocUserInfo.strSCClass + strTmp;
	((CComboBox*)GetDlgItem(IDC_edtUSER2))->GetWindowText(strTmp);
	stLocUserInfo.strSCClass + strTmp;

	((CComboBox*)GetDlgItem(IDC_edtUSER_2Contacts))->GetWindowText(stLocUserInfo.strSCContact);
	
}

void CBO_LOC_3320::CompareLocUser(ST_LOC_USER_INFO &stOriLocUserInfo, ST_LOC_USER_INFO &stDesLocUserInfo, CString &strContent)
{
	BOOL bModify = FALSE, bFirst = TRUE;

	CString strTemp = _T("");

	if(stOriLocUserInfo.strUserNo.Compare(stDesLocUserInfo.strUserNo) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("대출자번호 변경"));

		stOriLocUserInfo.strUserNo = stDesLocUserInfo.strUserNo;

		strContent += strTemp;		
	}

	if(stOriLocUserInfo.strUserID.Compare(stDesLocUserInfo.strUserID) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("사용자ID 변경"));

		stOriLocUserInfo.strUserID = stDesLocUserInfo.strUserID;

		strContent += strTemp;
	}
	
	if(stOriLocUserInfo.strPwd.Compare(stDesLocUserInfo.strPwd) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("비밀번호 변경"));

		stOriLocUserInfo.strPwd = stDesLocUserInfo.strPwd;

		strContent += strTemp;
	}	

	if(stOriLocUserInfo.strLCPwd.Compare(stDesLocUserInfo.strLCPwd) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("회원증비밀번호 변경"));

		stOriLocUserInfo.strLCPwd = stDesLocUserInfo.strLCPwd;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strFaID.Compare(stDesLocUserInfo.strFaID) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("가족ID 변경"));

		stOriLocUserInfo.strFaID = stDesLocUserInfo.strFaID;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strEmail.Compare(stDesLocUserInfo.strEmail) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("E-Mail 변경"));

		stOriLocUserInfo.strEmail = stDesLocUserInfo.strEmail;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strName.Compare(stDesLocUserInfo.strName) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("이름 변경"));

		stOriLocUserInfo.strName = stDesLocUserInfo.strName;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strPhone.Compare(stDesLocUserInfo.strPhone) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("휴대폰번호 변경"));

		stOriLocUserInfo.strPhone = stDesLocUserInfo.strPhone;

		strContent += strTemp;
	}
	
	if(stOriLocUserInfo.bMan != stDesLocUserInfo.bMan)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("성별 변경"));

		stOriLocUserInfo.bMan = stDesLocUserInfo.bMan;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strBirth.Compare(stDesLocUserInfo.strBirth) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("생년월일 변경"));

		stOriLocUserInfo.strBirth = stDesLocUserInfo.strBirth;

		strContent += strTemp;
	}


	if(stOriLocUserInfo.strPositionCode.Compare(stDesLocUserInfo.strPositionCode) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("소속 변경"));

		stOriLocUserInfo.strPositionCode = stDesLocUserInfo.strPositionCode;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strClassCode.Compare(stDesLocUserInfo.strClassCode) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("직급 변경"));

		stOriLocUserInfo.strClassCode = stDesLocUserInfo.strClassCode;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strHZipCode.Compare(stDesLocUserInfo.strHZipCode) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("자택 우편번호 변경"));

		stOriLocUserInfo.strHZipCode = stDesLocUserInfo.strHZipCode;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strHAddr.Compare(stDesLocUserInfo.strHAddr) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("자택 주소번호 변경"));

		stOriLocUserInfo.strHAddr = stDesLocUserInfo.strHAddr;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strHTel.Compare(stDesLocUserInfo.strHTel) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("자택 전화번호 변경"));

		stOriLocUserInfo.strHTel = stDesLocUserInfo.strHTel;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strWZipCode.Compare(stDesLocUserInfo.strWZipCode) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("근무지 우편번호 변경"));

		stOriLocUserInfo.strWZipCode = stDesLocUserInfo.strWZipCode;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strWAddr.Compare(stDesLocUserInfo.strWAddr) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("근무지 주소번호 변경"));

		stOriLocUserInfo.strWAddr = stDesLocUserInfo.strWAddr;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strWTel.Compare(stDesLocUserInfo.strWTel) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("근무지 전화번호 변경"));

		stOriLocUserInfo.strWTel = stDesLocUserInfo.strWTel;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strWName.Compare(stDesLocUserInfo.strWName) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("근무처 변경"));

		stOriLocUserInfo.strWName =  stDesLocUserInfo.strWName;

		strContent += strTemp;
	}

	if(stOriLocUserInfo.strWDepartName.Compare(stDesLocUserInfo.strWDepartName) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("근무지 부서명 변경"));

		stOriLocUserInfo.strWDepartName = stDesLocUserInfo.strWDepartName;

		strContent += strTemp;
	}
			
	if(stOriLocUserInfo.strSCName.Compare(stDesLocUserInfo.strSCName) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("학교명칭 변경"));

		stOriLocUserInfo.strSCName = stDesLocUserInfo.strSCName;

		strContent += strTemp;
	}	

	if(stOriLocUserInfo.strSCClass.Compare(stDesLocUserInfo.strSCClass) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("학년반번호정보 변경"));

		stOriLocUserInfo.strSCClass = stDesLocUserInfo.strSCClass;

		strContent += strTemp;
	}	

	if(stOriLocUserInfo.strSCContact.Compare(stDesLocUserInfo.strSCContact) != 0)
	{
		bModify = TRUE;
		if(!bFirst)
		{
			strContent += _T(", ");
		}
		else
		{
			bFirst = FALSE;
		}
		strTemp.Format(_T("제2연락처 변경"));

		stOriLocUserInfo.strSCContact = stDesLocUserInfo.strSCContact;

		strContent += strTemp;
	}	
}



VOID CBO_LOC_3320::OnbtnReName() 
{
EFS_BEGIN

	if(0 != m_nMode && 1 != m_nMode && 3 != m_nMode) return;
	StartUserCertify(1);	
EFS_END
}


void CBO_LOC_3320::ToggleOfficeView()
{
	int i = 0;
	const int nCntMax = 24;
	BOOL bCheck = FALSE;
	UINT nCnt[nCntMax] =
	{
		IDC_STATIC_UNVISIBLE_2,		IDC_STATIC_11,		IDC_edtUSER16,		IDC_STATIC_12,		IDC_edtUSER17,
		IDC_STATIC_13,				IDC_edtUSER18,		IDC_STATIC_14,		IDC_edtUSER2,		IDC_stcUSER_2Contacts,
		IDC_edtUSER_2Contacts,		IDC_STRONGSTATIC4,	IDC_edtUSER22,		IDC_lblWORK_CODE,	IDC_edtUSER12,
		IDC_lblWORK_ADDRESS,		IDC_edtUSER13,		IDC_btnFIND2,		IDC_STATIC_17,		IDC_edtUSER_34,
		IDC_STATIC_15,				IDC_edtUSER29,		IDC_lblWORK_PHONE,	IDC_edtUSER14
		
	};

	CRect	rcWindow,	
			rcBottom,	
			rcGroup,	
			rcWorkFrame,
			rcChild;	
	
	LONG	nTop,	
			nHeight;
	CString strValue;

	GetWindowRect(&rcWindow);

	GetDlgItem(IDC_grpWORK)->GetWindowRect(rcWorkFrame);
	nTop = rcWorkFrame.top + 30;

	
	GetDlgItem(IDC_STATIC_UNVISIBLE_2)->GetWindowRect(rcGroup);
	rcGroup.top = nTop;

	rcBottom = rcWindow;

	if(m_bIsOfficeShow == TRUE){
		rcBottom.top = rcGroup.top;
	}else{
		rcBottom.top = rcGroup.bottom;
	}

	nHeight = rcGroup.bottom - nTop;
	rcGroup.top = rcGroup.top + 5;
	rcGroup.bottom = rcGroup.bottom - 5;

	m_pLoanMangeValue->GetManageValue(_T("열람"), _T("공통"), _T("대출자접근권한"), GetUserID(), strValue);
	
	if( TRUE == strValue.IsEmpty() ) 
	{
		strValue = _T("1");
	}

	CWnd *pWnd = GetWindow(GW_CHILD);
	while(pWnd)
	{
		if(	pWnd->GetDlgCtrlID() != IDC_grpWORK && 
			pWnd->GetDlgCtrlID() != IDC_INPUTMODE_SIZE &&
			pWnd->GetDlgCtrlID() != IDC_cMONETA &&
			pWnd->GetDlgCtrlID() != IDC_bMONETA &&
			pWnd->GetDlgCtrlID() != IDC_edtEXCHANGE &&
			pWnd->GetDlgCtrlID() != IDC_edtCLASS_NO &&
			pWnd->GetDlgCtrlID() != IDC_BTN_OFFICE_SHOWHIDE){

			pWnd->GetWindowRect(rcChild);

			if(rcChild.IntersectRect(rcGroup,rcChild))
				pWnd->ShowWindow(m_bIsOfficeShow);
			
			

			pWnd->GetWindowRect(&rcChild);
			
			if(rcChild.IntersectRect(rcBottom,rcChild) && 
				(pWnd->IsWindowVisible() == TRUE	||
				 pWnd->GetDlgCtrlID() == IDC_btnCLOSE ||
					( strValue != _T("1") && 
						(
						pWnd->GetDlgCtrlID() == IDC_btnSAVE ||
						pWnd->GetDlgCtrlID() == IDC_BN_FAMILY_ADD ||
						pWnd->GetDlgCtrlID() == IDC_btnCLEAR
						)
					)
				)
			){

				bCheck = FALSE;
				for(i=0;i<nCntMax;i++){
					if(pWnd->GetDlgCtrlID() == nCnt[i]){
						bCheck = TRUE;
						break;
					}
				}
				if(bCheck == FALSE){
				
					if(m_bIsOfficeShow == TRUE){
						rcChild.top = rcChild.top + nHeight;
						rcChild.bottom = rcChild.bottom + nHeight;
					}else{
						rcChild.top = rcChild.top - nHeight;
						rcChild.bottom = rcChild.bottom - nHeight;
					}
					ScreenToClient(rcChild);
					pWnd->MoveWindow(rcChild);
				}
			}
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	if(m_bIsOfficeShow == TRUE)	
	{
		rcWorkFrame.bottom = rcWorkFrame.bottom + 55;
		ScreenToClient(rcWorkFrame);
		GetDlgItem(IDC_grpWORK)->MoveWindow(rcWorkFrame);

		rcWindow.bottom = rcWindow.bottom + nHeight;
		MoveWindow(rcWindow);
		GetDlgItem(IDC_BTN_OFFICE_SHOWHIDE)->SetWindowText(_T("▲"));

	}else{						
		rcWorkFrame.bottom = rcWorkFrame.bottom - 55;
		ScreenToClient(rcWorkFrame);
		GetDlgItem(IDC_grpWORK)->MoveWindow(rcWorkFrame);

		rcWindow.bottom = rcWindow.bottom - nHeight;
		MoveWindow(rcWindow);
		GetDlgItem(IDC_BTN_OFFICE_SHOWHIDE)->SetWindowText(_T("▼"));
	}

	m_bIsOfficeShow = !m_bIsOfficeShow;
}


void CBO_LOC_3320::UpdateUserScanKey(CString strUserKey)
{
	INT ids = -1;	
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3300"));

	if ( m_strTmpImgUserKey.GetLength() > 0 ) {
		CString strQuery;
		strQuery.Format(_T("UPDATE CO_AGREEMENT_SCAN_TBL")
						_T(" SET USER_KEY=%s, LAST_WORK='%s' WHERE USER_KEY=%s;")
			, strUserKey		
			, GetWorkLogMsg(_T("신규입력업데이트"),__WFILE__,__LINE__)
			, m_strTmpImgUserKey);
		pDM->StartFrame();
		pDM->InitDBFieldData();
		pDM->AddFrame(strQuery);
		pDM->SendFrame(TRUE);	
		pDM->EndFrame();

		m_strTmpImgUserKey = _T("");
	}
}