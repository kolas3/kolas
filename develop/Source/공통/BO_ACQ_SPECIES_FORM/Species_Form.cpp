// Species_Form.cpp : implementation file
//
#include "stdafx.h"
#include "GroupInfoDlg.h"
#include "BasicInfoDlg.h"
#include "DivisionNoInfoDlg.h"
#include "DonateInfoDlg.h"
#include "PurchaseInfoDlg.h"
#include "Species_Form.h"
#include "BOAppendixDlg.h"
#include "BOMarcEditDlg.h"
#include "Light_DivisionNoInfoDlg.h"
#include "..\BO_VOLUME_FORM\BOVolumeDlg.h"
#include "..\BO_VOLUME_FORM\BOISBNAdditionDlg.h"
#include "..\BO_MARC_IMPORT\MARC_IMPORT.h"
#include "..\..\공동목록\Light_GatherDataSearch\UC_MARC_IMPORT.h"
#include "..\..\공동목록\MOKCHA_CHOROK_API\BO_INERT_TABLE.h"
#include "..\Duplicate_Data_Search\DuplicateSearch_Interface.h"
#include "../K2UP_Common/MulitComboBoxManager.h"
#include "../COVERMANAGER/COVER_MAIN.h"
// 2004.12.29 대출자 조회
#include "../../단행/수서/BO_ACQ_LOAN_USER_SEARCH_MGR/BO_ACQ_LOAN_USER_SEARCH_DLG.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecies_Form dialog
// 생성자
CSpecies_Form::CSpecies_Form(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSpecies_Form::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecies_Form)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nParentSelectedRow = -1;	
	m_nOpType = OPERATION_TYPE_INVALID;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_nParentType	= SPECIES;
	
	m_pDM_Index = NULL;
	m_pDM_Species = NULL; 
	m_pDM_Book = NULL;
	m_pDM_Accu = NULL;
	m_pDM_Appendix = NULL;
	m_pDM_Parent		= NULL;
	m_pDM_Parent_Book	= NULL; 
	m_pCM_Index = NULL;
	m_pCM_Accu = NULL;
	m_pGrid_Parent		= NULL;

	m_pGroupCollection	= NULL;	
	
	m_pArrDonateRegYN	= NULL;

	m_pNoteCtrl		= NULL;

	m_pHeadDlg		= NULL;
	m_pTailDlg		= NULL;

	m_pFormApi		= NULL; 
	m_pBOSpeciesApi = NULL;

	m_bShowCode = FALSE;
	m_bDupAddMode = FALSE;
	m_bAutoDupSearch = FALSE;
	m_bControlDisplay = FALSE;
	m_bViewVolInfo = FALSE;
	m_bAcqTypeChange = FALSE;
	m_bInsertAC = FALSE;
	m_bInsertData = FALSE;
	m_bGridSelectYN = FALSE;
	m_bShowEBookInfo = FALSE;
	m_bDupAddYN = FALSE;
	m_bCreateGroupCollection = FALSE;

	m_strIs5163				= _T("");
	m_strIs64				= _T("");
	m_strIs90				= _T("");
	m_strWorkingStatus		= _T("");	
	m_strLastErrMsg			= _T("");	
	m_strSpeciesKeyAlias	= _T("");
	m_strAccuKeyAlias		= _T("");	
	m_strRegCodeField		= _T("");	
	m_strDupIndexKey		= _T("");
	m_strPriorityCatalogYN	= _T("N");	// 우선정리자료 사용안함이 default
	m_strUseCodeYN			= _T("N");  // 사용자 정의코드 사용안함이 default
	m_strUseCoverYN			= _T("N");	// 표지 추가 기능 사용안함이 default
	m_strAcqAppendixYN		= _T("N");  // 수서단계에서도 부록정보 버튼 보이는 기능 	
	m_strDefaultValueFilePath = _T("..\\cfg\\species_form.cfg");
	m_szSpeciesKey[0] = '\0';

	m_arrIgnoreCTRLAlias.RemoveAll();

	//2007.08.10 ADD BY PJW : 신청시간 추가
	m_hApplicantTime = NULL;

	//20070821 ADD PJW : 새로저장을 체크 하기위한 변수를 FALSE로 세팅
	m_bNewSaveFlag = FALSE;

	// 2008.06.08 ADD BY PDJ : 목차초록을 위한 RichEdit20W Init.
	AfxInitRichEdit();


}
// 소멸자
CSpecies_Form::~CSpecies_Form()
{
	//E-BOOK임시폴더 지워라
	m_pFormApi->DeleteEBookTempFolder();

	if( m_pHeadDlg != NULL )
	{
		delete m_pHeadDlg;
		m_pHeadDlg = NULL;
	}
	if( m_pTailDlg != NULL )
	{
		delete m_pTailDlg;
		m_pTailDlg = NULL;
	}
	if( m_pFormApi != NULL )
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}
	if( m_pBOSpeciesApi !=NULL )
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	if( m_bCreateGroupCollection == TRUE )
	{
		if( m_pGroupCollection != NULL )
		{
			delete m_pGroupCollection ;
			m_pGroupCollection = NULL;
		}
	}
	// 16/10/12 김혜영 : 단행>수서대상>반입자료관리 메뉴의 입력창 실행 후 종료시 메모리누수 현상 수정
//*/ ADD ---------------------------------------------------------------------------
	if(m_hApplicantTime)
	{
		delete m_hApplicantTime;
		m_hApplicantTime = NULL;
	}
//*/ END ---------------------------------------------------------------------------
}


VOID CSpecies_Form::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecies_Form)
	DDX_Control(pDX, IDC_cmbAPPLICANT, m_cmbApplicant);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpecies_Form, CDialog)
	//{{AFX_MSG_MAP(CSpecies_Form)
	ON_BN_CLICKED(IDC_btnBSFORM_SHOW_HIDE_CODE, OnbtnBSFORMSHOWHIDECODE)
	ON_EN_KILLFOCUS(IDC_eTITLE, OnKillfocuseTITLE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_bMARC_EDIT, OnbMARCEDIT)
	ON_BN_CLICKED(IDC_bMARC_IMPORT, OnbMARCIMPORT)
	ON_BN_CLICKED(IDC_bKOLIS_NET, OnbKOLISNET)
	ON_BN_CLICKED(IDC_bAPPENDIX, OnbAPPENDIX)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_EN_KILLFOCUS(IDC_eREG_CLASS_CODE, OnKillfocuseREGCLASSCODE)
	ON_BN_CLICKED(IDC_bPREV_SPECIES, OnbPREVSPECIES)
	ON_BN_CLICKED(IDC_bNEXT_SPECIES, OnbNEXTSPECIES)
	ON_BN_CLICKED(IDC_bSAVEAS, OnbSAVEAS)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bEXTRA_ISBN, OnbEXTRAISBN)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_BN_CLICKED(IDC_bDEFAULT_CONFIG, OnbDEFAULTCONFIG)
	ON_BN_CLICKED(IDC_bDUP_SEARCH, OnbDUPSEARCH)
	ON_BN_CLICKED(IDC_bAPPLY_SERIES, OnbAPPLYSERIES)
	ON_BN_CLICKED(IDC_bVOL_INFO, OnbVOLINFO)
	ON_CBN_SELCHANGE(IDC_cCLASIFY_CLASS, OnSelchangecCLASIFYCLASS)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_CBN_SELCHANGE(IDC_cFORM_CLASS, OnSelchangecFORMCLASS)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cUSE_OBJECT_CLASS, OnSelchangecUSEOBJECTCLASS)
	ON_BN_CLICKED(IDC_btnPREV_CONTENT, OnbtnPREVCONTENT)
	ON_CBN_SELCHANGE(IDC_cWORK_CODE, OnSelchangecWORKCODE)
	ON_CBN_SELCHANGE(IDC_cMEDIA_CLASS, OnSelchangecMEDIACLASS)
	ON_BN_CLICKED(IDC_btnFIND_FILE1, OnbtnFINDFILE1)
	ON_BN_CLICKED(IDC_btnFIND_FILE3, OnbtnFINDFILE3)
	ON_BN_CLICKED(IDC_btnFIND_FILE2, OnbtnFINDFILE2)
	ON_BN_CLICKED(IDC_btnEBOOK_VIEW, OnbtnEBOOKVIEW)
	ON_EN_KILLFOCUS(IDC_edtAPPENDIX, OnKillfocusedtAPPENDIX)
	ON_BN_CLICKED(IDC_btnCover, OnbtnCover)
	ON_BN_CLICKED(IDC_btnSearchUser, OnbtnSearchUser)
	ON_CBN_SELCHANGE(IDC_cmbAPPLICANT, OnSelchangecmbAPPLICANT)	
	ON_CBN_SELCHANGE(IDC_cAPPLICAND_KIND_CLASS, OnSelchangecAPPLICANDKINDCLASS)
	ON_CBN_SELCHANGE(IDC_cAPPLICAND_KIND_COURCE, OnSelchangecAPPLICANDKINDCOURCE)
	ON_CBN_SELCHANGE(IDC_cAPPLICAND_KIND_DEPT, OnSelchangecAPPLICANDKINDDEPT)
	ON_EN_SETFOCUS(IDC_edtSERIES_VOL, OnSetfocusedtSERIESVOL)
	ON_EN_SETFOCUS(IDC_edtPHYSICAL_CHARACTER, OnSetfocusedtPHYSICALCHARACTER)
	ON_EN_SETFOCUS(IDC_edtSIZE, OnSetfocusedtSIZE)
	ON_EN_SETFOCUS(IDC_eEDITION_STMT, OnSetfocuseEDITIONSTMT)
	ON_EN_SETFOCUS(IDC_eSERIES_TITLE, OnSetfocuseSERIESTITLE)
	ON_EN_SETFOCUS(IDC_eREG_CLASS_CODE, OnSetfocuseREGCLASSCODE)
	ON_EN_SETFOCUS(IDC_eTITLE, OnSetfocuseTITLE)
	ON_EN_SETFOCUS(IDC_eVOL, OnSetfocuseVOL)
	ON_EN_SETFOCUS(IDC_ePUBLISHER, OnSetfocusePUBLISHER)
	ON_EN_SETFOCUS(IDC_ePUBLISH_PLACE, OnSetfocusePUBLISHPLACE)
	ON_EN_SETFOCUS(IDC_edtAPPENDIX, OnSetfocusedtAPPENDIX)
	ON_EN_SETFOCUS(IDC_eDISCRIPTION_SMALL, OnSetfocuseDISCRIPTIONSMALL)
	ON_EN_SETFOCUS(IDC_eDISCRIPTION, OnSetfocuseDISCRIPTION)
	ON_EN_SETFOCUS(IDC_eAUTHOR1, OnSetfocuseAUTHOR1)
	ON_EN_SETFOCUS(IDC_edtBSFORM_REQUESTOR, OnSetfocusedtBSFORMREQUESTOR)
	ON_EN_KILLFOCUS(IDC_edtBSFORM_REQUESTOR, OnKillfocusedtBSFORMREQUESTOR)
	ON_MESSAGE(ACQ_TYPE_CHANGE, OnAcqTypeChange)
	ON_MESSAGE(WORKING_STATUS_CHANGE, OnWorkingStatusChange)
	ON_MESSAGE(PURCHASE_SEQ_NO_CHANGE, OnPurchaseSeqNoChange)
	ON_MESSAGE(NOTE_CLICKED, OnNoteClicked)
	ON_MESSAGE(SET_TAB_ORDER, OnSetTabOrder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecies_Form message handlers
// 초기화
//
// [ 미니스펙 ]
// 1. SM을 설정(K2UPGRADE_단행_종) 한다.
// 2. 필수 입력항목에 대한 컨트롤 색상을 설정한다.
// 3. 윈도우 rect를 멤버변수에 저장한다.
// 4. 5163 DB에서 사용자정의코드 사용여부, 표지관리 사용여부 값을 가져온다.
// 5. 콤보박스를 설정한다.
// 6. [API/CM/DM/HeadDlg/TailDlg/Ctrl/무시할 알리아스들]을 초기화 및 설정한다.
// 7. EBook설정여부 및 입력창에서의 리소스들을 설정 및 초기화 한다.[ShowCodeResource(), ShowCodeResourceByMediaCode() 호출]
// 8. 주제구분 표시
// 9. 육사플래그에 따라서 신청구분 콤보박스를 활성화시킨다.
//
/////////////////////////////////////////////////////////////////////////////
BOOL CSpecies_Form::OnInitDialog() 
{
	CDialog::OnInitDialog();
		
	//=======================================================================
	// 변수정의
	//=======================================================================
	CESL_DataMgr* TmpDM = NULL;

	//=======================================================================
	// 1. SM을 설정(K2UPGRADE_단행_종) 한다.
	//=======================================================================
	if( InitESL_Mgr(_T("K2UPGRADE_단행_종")) < 0 )
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}		
	
	//=======================================================================
	// 2. 필수 입력항목에 대한 컨트롤 색상을 설정한다.
	//=======================================================================	

	AddColorControl(IDC_eTITLE, _T(""), _T(""), 'E');
	AddColorControl(IDC_eREG_CLASS_CODE, _T(""), _T(""), 'E');
	AddColorControl(IDC_cFORM_CLASS, _T("CMUP_단행_종_색인정보"), _T("IBS_형식구분_코드"), 'M');
	
	//AddColorControl(IDC_eAUTHOR1, _T(""), _T(""), 'E');
	
	//=======================================================================
	// 3. 윈도우 rect를 멤버변수에 저장한다.
	//=======================================================================	
	GetWindowRect(m_rcWindow);
	
	//=======================================================================
	// 4. 5163 DB에서 사용자정의코드 사용여부, 표지관리 사용여부 값을 가져온다.
	// 2004.11.04
	//=======================================================================	
	// BEGIN:
	TmpDM = FindDM(_T("DMUP_단행_종_종정보"));
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USERDEFINECODE'"), m_strUseCodeYN);
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'USECOVERIMG'"), m_strUseCoverYN);
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'PRIORITYCATALOG'"), m_strPriorityCatalogYN);
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE = 'ACQAPPENDIXINSERT'"), m_strAcqAppendixYN);
	// 2005-01-28
	GetManageValue( _T("기타"), _T("공통"), _T("5163"), _T("5163"), m_strIs5163 );
	GetManageValue( _T("기타"), _T("공통"), _T("육사"), _T("육사"), m_strIs64 );	
	GetManageValue( _T("기타"), _T("공통"), _T("국방대"), _T("국방대"), m_strIs90 );	
	// END:
	

	//=======================================================================
	// 5. 콤보박스를 설정한다.
	//=======================================================================
	m_cmbApplicant.ResetContent();
	m_cmbApplicant.InsertString( 0, _T("회원") );
	m_cmbApplicant.InsertString( 1, _T("부서") );
	m_cmbApplicant.InsertString( 2, _T("학과") );
	m_cmbApplicant.InsertString( 3, _T("학처") );
	
	if		( _T("Y") == m_strIs90 && ACQ_TYPE_INVALID != m_nAcqType )	m_cmbApplicant.SetCurSel(1);
	else if	( _T("Y") == m_strIs64 && ACQ_TYPE_INVALID != m_nAcqType )	m_cmbApplicant.SetCurSel(3);
	else					m_cmbApplicant.SetCurSel(0);

	//20070810 ADD PJW : 시간 초기 에디트 설정
	m_hApplicantTime = new CHourEditControl;
	m_hApplicantTime->SubclassWindow( GetDlgItem(IDC_edtBSFORM_REQUEST_HOUR)->GetSafeHwnd() );
	m_hApplicantTime->InitEdit();




	//=======================================================================
	// 6. [API/CM/DM/HeadDlg/TailDlg/Ctrl/무시할 알리아스들]을 초기화 및 설정한다.
	//=======================================================================
	if( Init() < 0 )
	{
		ESLAfxMessageBox(_T("종-입력/수정화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	//=======================================
	//20080612 ADD BY PJW : 구입신청자 존재여부에 따라서 날짜,시간 커트롤 재설정한다.
	OnKillfocusedtBSFORMREQUESTOR(); 
	//=======================================

	//=======================================================================
	// 7. EBook설정여부 및 입력창에서의 리소스들을 설정 및 초기화 한다.[ShowCodeResource(), ShowCodeResourceByMediaCode() 호출]
	//=======================================================================
	InitEBookCtrl();	

	//=======================================================================
	// 8. 주제구분 표시
	//=======================================================================
	OnSelchangecCLASIFYCLASS();

	//=======================================================================
	// 9. 서명을 첫 포커스로 하고 _T("한글")모드로 변경
	//=======================================================================	
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_eTITLE)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion=IME_CMODE_HANGUL;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_eTITLE)->m_hWnd,hIME); 
    }	

	//20070522 ADD PJW : 서명에 포커스 
	SetTimer(3,10,NULL);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  
}

VOID CSpecies_Form::MoveTailDlg(INT nCodeResourcesHeight)
{
	//INT nCodeResourcesHeight = 121;
	nCodeResourcesHeight -= 5;
	CRect rcResource;
	m_pTailDlg->GetWindowRect(rcResource);
	ScreenToClient(rcResource);
	if(m_bShowCode)
	{
		rcResource.top += (nCodeResourcesHeight);
		rcResource.bottom += (nCodeResourcesHeight);
		m_pTailDlg->MoveWindow(rcResource);
	}
	else
	{
		rcResource.top -= (nCodeResourcesHeight);
		rcResource.bottom -= (nCodeResourcesHeight);
		m_pTailDlg->MoveWindow(rcResource);
	}
}

VOID CSpecies_Form::SetTailInfoDlg()
{
	if(m_pTailDlg)
	{
		delete m_pTailDlg;
		m_pTailDlg = NULL;
	}
	if(m_nAcqType == ACQ_TYPE_INVALID)
		m_pTailDlg = new CBasicInfoDlg(this);
	else if(m_nAcqType == PURCHASE)
		m_pTailDlg = new CPurchaseInfoDlg(this);
	else if(m_nAcqType == DONATE)
		m_pTailDlg = new CDonateInfoDlg(this);
	m_pTailDlg->CreateInfoDlg(this);
	
	MoveTailDlg(121);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - Head/Tail창을 설정하고 메인창에 위치를 맞춰서 붙인다.
//
// [ 미니스펙 ]
// 1. 메인창의 Head 창과 Tail 창이 있다면 소멸한다.
// 2. m_nAcqType(입수) 타입에 따라 Head 창과 Tail창을 설정한다.
// 3. Head 창에 정보를 설정한다.
// 4. Tail 창에 정보를 설정한다.
// 5. 설정된 Head/Tail 창을 생성하여 메인 창에 붙인다.
// 6. Head/Tail창 및 메인 창에 렉트를 구하여 Tail창의 위치를 맞춘다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::SetHeadTailInfoDlg()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT nOffSet = 0;
	BOOL bShowEbookInfo;
	CString strValue;
	CRect rectHead, rectTail, rectParent;

	//=======================================================================
	// 1. 메인창의 Head 창과 Tail 창이 있다면 소멸한다.
	//=======================================================================
	if( m_pHeadDlg != NULL )
	{
		m_pHeadDlg->ShowWindow(SW_HIDE);
		delete m_pHeadDlg;
		m_pHeadDlg = NULL;
	}
	
	if( m_pTailDlg != NULL )
	{
		m_pTailDlg->ShowWindow(SW_HIDE);
		delete m_pTailDlg;
		m_pTailDlg = NULL;
	}
	

	//=======================================================================
	// 2. m_nAcqType(입수) 타입에 따라 Head 창과 Tail창을 설정한다.
	//=======================================================================
	switch( m_nAcqType )
	{
	case ACQ_TYPE_INVALID :
	{
		m_pHeadDlg = new CGroupInfoDlg(this);
		m_pTailDlg = new CBasicInfoDlg(this);
	}
		break;

	case PURCHASE :
	{
		if( m_nSpeciesType == BO_PURCHASE && m_pInfo->GetKIIUPClientVersion() == 'L' )
		{
			m_pHeadDlg = new CLight_DivisionNoInfoDlg(this);
		}
		else
		{
			m_pHeadDlg = new CDivisionNoInfoDlg(this);
		}
			m_pTailDlg = new CPurchaseInfoDlg(this);
	}
		break;

	case DONATE :
	{
		m_pHeadDlg = new CDivisionNoInfoDlg(this);
		m_pTailDlg = new CDonateInfoDlg(this);

		m_pHeadDlg->SetWorkingStatus(m_strWorkingStatus);
	}
		break;

	case OTHER_ACQ_TYPE :
	{
		m_pHeadDlg = new CDivisionNoInfoDlg(this);
		m_pTailDlg = new CBasicInfoDlg(this);
	}
		break;

	default:
		break;
	}
	
	//=======================================================================
	// 3. Head 창에 정보를 설정한다.
	//=======================================================================
	m_pHeadDlg->SetGroupCollection(m_pGroupCollection);	// 그룹정보
	m_pHeadDlg->SetAcqType(m_nAcqType);					// 입수정보
	m_pHeadDlg->SetAppendexDM(m_pDM_Appendix);			// 부록DM정보
	
	m_pHeadDlg->SetFormApi(m_pFormApi);					// 폼API
	m_pHeadDlg->SetSpeciesApi(m_pBOSpeciesApi);			// 종API
	m_pHeadDlg->SetSpeciesType(m_nSpeciesType);			// 종타입
	m_pHeadDlg->SetOpType(m_nOpType);					// 오픈타입
	m_pHeadDlg->SetBookDM(m_pDM_Book);					// 책 DM		
	m_pHeadDlg->SetDM(m_pDM_Accu);						// 구입(기증)정보 DM

	//=======================================================================
	// 4. Tail 창에 정보를 설정한다.
	//=======================================================================
	m_pTailDlg->SetFormApi(m_pFormApi);					// 폼API
	m_pTailDlg->SetSpeciesApi(m_pBOSpeciesApi);			// 종API		
	m_pTailDlg->SetSpeciesType(m_nSpeciesType);			// 종타입
	m_pTailDlg->SetOpType(m_nOpType);					// 오픈타입
	m_pTailDlg->SetBookDM(m_pDM_Book);					// 책 DM
	m_pTailDlg->SetDM(m_pDM_Accu);						// 구입(기증)정보 DM


	//=======================================================================
	// 5. 설정된 Head/Tail 창을 생성하여 메인 창에 붙인다.
	//=======================================================================
	m_pHeadDlg->CreateInfoDlg(this);
	m_pHeadDlg->ShowWindow(SW_SHOW);
	m_pTailDlg->CreateInfoDlg(this);
	m_pTailDlg->ShowWindow(SW_SHOW);

	//=======================================================================
	// 6. Head/Tail창 및 메인 창에 렉트를 구하여 Tail창의 위치를 맞춘다.
	//=======================================================================
	m_pHeadDlg->GetWindowRect(&rectHead);
	m_pTailDlg->GetWindowRect(&rectTail);
	this->GetWindowRect(&rectParent);
	
	if( m_rcWindow == rectParent )	bShowEbookInfo = TRUE;
	else bShowEbookInfo = FALSE;

	ScreenToClient(rectTail);
	ScreenToClient(rectParent);
	
	//249
	if( bShowEbookInfo == TRUE )	nOffSet = rectParent.bottom - 290;
	else							nOffSet = rectParent.bottom - 221; //nOffSet = rectParent.bottom - 184;

	rectTail.top += nOffSet;
	rectTail.bottom += nOffSet;
	rectTail.left = 0;

	m_pTailDlg->MoveWindow(rectTail);
	m_pTailDlg->ShowWindow(SW_SHOW);
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 종타입에따라 현재 입력창 왼쪽 맨위에 작업상태를 설정한다.
//
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::SetWorkingStatus()
{
	CEdit* pCtrl = (CEdit*)GetDlgItem(IDC_edtWORKING_STATUS);
	switch( m_nSpeciesType )
	{
	case BO_BASIC:
		{
		m_strWorkingStatus = _T("BOT111O");
			pCtrl->SetWindowText(_T("기초자료"));
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("기초자료"));
		}
		break;

	case BO_IMPORT:
		{
		m_strWorkingStatus = _T("BOT112O");
			pCtrl->SetWindowText(_T("반입자료"));
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("반입자료"));
		}
		break;

	case BO_PURCHASE:
		{
		m_strWorkingStatus = _T("BOA111N");
			pCtrl->SetWindowText(_T("구입대상자료"));
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("구입대상자료"));
		}
		break;

	case BO_ORDER:
		{
		m_strWorkingStatus = _T("BOA112N");
			pCtrl->SetWindowText(_T("주문자료"));
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("주문자료"));
		}
		break;

	case BO_MISSING:
		{
		m_strWorkingStatus = _T("BOA113O");
			pCtrl->SetWindowText(_T("미납자료"));
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("미납자료"));
		}
		break;

	case BO_REGOBJ:
		{
		if(m_nAcqType == PURCHASE)
		{
			m_strWorkingStatus = _T("BOA114O");
				pCtrl->SetWindowText(_T("구입등록인계대상자료"));
			//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("구입등록인계대상자료"));
		}
		else if(m_nAcqType == DONATE)
		{
			m_strWorkingStatus = _T("BOA211O");
				pCtrl->SetWindowText(_T("기증등록인계대상자료"));
			//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("기증등록인계대상자료"));
			}
		}
		break;

	case BO_DONATE:
		{
		m_strWorkingStatus = _T("BOA211O");
			pCtrl->SetWindowText(_T("기증접수자료"));
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("기증접수자료"));
		}
		break;

	case BO_NONDONATE:
		{
		m_strWorkingStatus = _T("BOA212O");
			pCtrl->SetWindowText(_T("기증부당자료"));
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("기증부당자료"));
		}
		break;

	case BO_REG_PURCHASE:
		{
		// 2004.09.07 등록자료일 경우에도 등록인계자료로 표시됨. 주석처리
		//m_strWorkingStatus = _T("BOR111N");

		// 2004.09.07 얘는 원래 주석처리 되어 있었음. 
		// m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("등록인계자료"));
		
		m_strWorkingStatus = m_pDM_Parent->GetCellData(_T("BB_자료상태_코드"), m_nParentSelectedRow);
	
		// 2004.09.07 주석처리
		// pCtrl->SetWindowText(_T("등록인계자료"));
			if( m_strWorkingStatus.IsEmpty() == TRUE )
		{
			m_strWorkingStatus = _T("BOR111N");
			pCtrl->SetWindowText(_T("등록인계자료"));
		}
		else
			{
			pCtrl->SetWindowText(m_pDM_Parent->GetCellData(_T("BB_자료상태_설명"), m_nParentSelectedRow));
			}
		}
		break;

	case BO_REG_DONATE:
		{
		// 2004.09.07 등록자료일 경우에도 등록인계자료로 표시됨. 주석처리
		//m_strWorkingStatus = _T("BOR111N");

		// 2004.09.07 얘는 원래 주석처리 되어 있었음. 
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("등록인계자료"));

		m_strWorkingStatus = m_pDM_Parent->GetCellData(_T("BB_자료상태_코드"), m_nParentSelectedRow);

		// 2004.09.07 주석처리
		//pCtrl->SetWindowText(_T("등록인계자료"));
			if( m_strWorkingStatus.IsEmpty() == TRUE )
		{
			m_strWorkingStatus = _T("BOR111N");
			pCtrl->SetWindowText(_T("등록인계자료"));
		}
		else
			{
			pCtrl->SetWindowText(m_pDM_Parent->GetCellData(_T("BB_자료상태_설명"), m_nParentSelectedRow));
			}
		}
		break;

	case BO_REG_INVALID:
		{
		// 2004.09.07 등록자료일 경우에도 등록인계자료로 표시됨. 주석처리
		//m_strWorkingStatus = _T("BOR111N");

		// 2004.09.07 얘는 원래 주석처리 되어 있었음. 
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("등록인계자료"));

		m_strWorkingStatus = m_pDM_Parent->GetCellData(_T("BB_자료상태_코드"), m_nParentSelectedRow);
		
		// 2004.09.07 주석처리
		//pCtrl->SetWindowText(_T("등록인계자료"));
			if( m_strWorkingStatus.IsEmpty() == TRUE )
		{
			m_strWorkingStatus = _T("BOR111N");
			pCtrl->SetWindowText(_T("등록인계자료"));
		}
		else
			pCtrl->SetWindowText(m_pDM_Parent->GetCellData(_T("BB_자료상태_설명"), m_nParentSelectedRow));
		}
		break;

	default :
		{
		m_strWorkingStatus = _T("BOT111O");
		pCtrl->SetWindowText(_T("기초자료"));
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 멤버변수 DM을 설정하고 내용을 초기화한다.
//
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::SetDataManagers()
{
	m_pDM_Index		= FindDM(_T("DMUP_단행_종_색인정보"));
	m_pDM_Species	= FindDM(_T("DMUP_단행_종_종정보"));
	m_pDM_Book		= FindDM(_T("DMUP_단행_종_책정보"));
	m_pDM_Appendix  = FindDM(_T("DMUP_단행_종_부록정보"));
	
	switch( m_nAcqType )
	{
	case PURCHASE:	m_pDM_Accu = FindDM(_T("DMUP_단행_종_구입정보"));
		break;
	case DONATE:	m_pDM_Accu = FindDM(_T("DMUP_단행_종_기증정보"));
		break;
	default:		m_pDM_Accu = FindDM(_T("DMUP_단행_종_기본정보"));
		break;
	}

	m_pDM_Accu->FreeData();
	m_pDM_Index->FreeData();
	m_pDM_Species->FreeData();
	m_pDM_Book->FreeData();
	m_pDM_Appendix->FreeData();
}

VOID CSpecies_Form::SetAccuDataManager()
{
	m_pTailDlg->ApplyData();
	
	CESL_DataMgr * m_pTmpDM = NULL;

	if(m_nAcqType == PURCHASE)
	{
		m_pTmpDM = FindDM(_T("DMUP_단행_종_구입정보"));
		m_pTmpDM->InsertRow(-1);
		CString strTmpData;
		m_pCM_Index->GetControlMgrData(_T("IBS_업무구분_코드"), strTmpData);
		m_pTmpDM->SetCellData(_T("BP_차수업무구분"), strTmpData, 0);
	}
	else if(m_nAcqType == DONATE)
	{
		m_pTmpDM = FindDM(_T("DMUP_단행_종_기증정보"));
		m_pTmpDM->InsertRow(-1);
	}
	else 
	{
		m_pTmpDM = FindDM(_T("DMUP_단행_종_기본정보"));
		m_pTmpDM->InsertRow(-1);
	}
	
	if(m_pTmpDM == NULL) return;
	CStringArray ArrCopyList;
	ArrCopyList.Add(_T("BP_가격"));			ArrCopyList.Add(_T("BP_가격성격"));	
	ArrCopyList.Add(_T("BP_가격잡정보"));	ArrCopyList.Add(_T("BP_화폐구분"));
	
	INT ids = m_pFormApi->CopyDMInArray(m_pDM_Accu, 0, m_pTmpDM, 0, &ArrCopyList);
	if(ids < 0) return ;
	m_pDM_Accu = m_pTmpDM;
}

VOID CSpecies_Form::InitFormApi()
{
	if(m_pFormApi)
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}
	m_pFormApi = new CBOFormApi(this);
	// 기본값 설정가능한 필드 Alias 설정하기
	m_pFormApi->InitDefaultAlias();
	m_pFormApi->AddDefaultAlias(_T("IBS_본표제"));
	m_pFormApi->AddDefaultAlias(_T("IBS_저작자"));
	m_pFormApi->AddDefaultAlias(_T("IBS_판사항"));
	m_pFormApi->AddDefaultAlias(_T("IBS_총서명"));
	m_pFormApi->AddDefaultAlias(_T("IBS_총서편차"));
	m_pFormApi->AddDefaultAlias(_T("IBS_편권차"));
	m_pFormApi->AddDefaultAlias(_T("IBS_발행지"));
	m_pFormApi->AddDefaultAlias(_T("IBS_발행자"));
	m_pFormApi->AddDefaultAlias(_T("IBS_발행년"));
	m_pFormApi->AddDefaultAlias(_T("IBS_면장수"));
	m_pFormApi->AddDefaultAlias(_T("IBS_물리적특성"));
	m_pFormApi->AddDefaultAlias(_T("IBS_크기"));
	m_pFormApi->AddDefaultAlias(_T("IBS_딸림자료"));
	m_pFormApi->AddDefaultAlias(_T("IBS_낱권ISBN"));
	m_pFormApi->AddDefaultAlias(_T("IBS_낱권ISBN부가기호"));
	m_pFormApi->AddDefaultAlias(_T("IBS_낱권오류ISBN"));
	m_pFormApi->AddDefaultAlias(_T("IBS_세트_ISBN1"));
	m_pFormApi->AddDefaultAlias(_T("IBS_세트_ISBN2"));
	m_pFormApi->AddDefaultAlias(_T("IBS_세트_ISBN3"));
	m_pFormApi->AddDefaultAlias(_T("BP_가격"));
	m_pFormApi->AddDefaultAlias(_T("BP_가격성격"));
	m_pFormApi->AddDefaultAlias(_T("BP_가격잡정보"));
	m_pFormApi->AddDefaultAlias(_T("BP_화폐구분"));

	// Default IgnoreList 작성
	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_대표책여부"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_복본기호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_배가위치부호"));	
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));	
}

VOID CSpecies_Form::InitSpeciesApi()
{
	// 종 API 초기화
	if (m_pBOSpeciesApi) 
	{
		delete m_pBOSpeciesApi;
		m_pBOSpeciesApi = NULL;
	}
	m_pBOSpeciesApi = new CBO_SPECIES(this);
	m_pBOSpeciesApi->InitDataMgr();
	//===================================================
	//2010.05.24 ADD BY PJW : API에 종타입을 셋팅해 준다.
	m_pBOSpeciesApi->SetSpeciesType(m_nSpeciesType);
	//===================================================	
	if (m_pBOSpeciesApi == NULL)
	{
		m_strLastErrMsg = _T("종 API 초기화 실패");
		return ;
	}

}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - [API/CM/DM/HeadDlg/TailDlg/Ctrl/무시할 알리아스들]을 초기화 및 설정한다.
//
// [ 미니스펙 ]
// 1. 종/폼 API 초기화한다.
// 2. 멤버 CM을 설정한다.
// 3. 자동 복본조사 모드를 설정한다.
// 4. 오픈타입에 따라 다음 내용을 수행한다.	
//		[입력]
//		1-1. DM설정 및 초기화 한다.
//		1-2. 자료상태를 설정한다.
//		1-3. 폼 API에 모드를 설정한다.
//		1-4. 색인, 종, 책, 구입정보 DM에 Row를 하나 추가한다.
//		1-5. 입수구분이 구입이라면, 구입정보 DM에 BP_차수업무구분에 _T("1")을 셋팅한다.
//		1-6. Head/Tail 다이얼로그를 설정 및 생성한다.
//		1-7. _T("화면지우기")와 _T("이전내용") 버튼을 보이게 한다.
//		1-8. 초기값을 불러온다.
//		[수정]
//		2-1. 부모그리드의 선택된 로우를 구성한다.
//
//		2-4. 그리드를 반전 표시한다		2-2. 종타입에따라 DM 및 작업상태를 설정한다. (입력에서의 1,2번)
//		2-3. 폼 API에 모드를 설정한다..
//		2-5. 멤버변수로 설정된 선정로우(m_nParentSelectedRow)의 종키와 수서정보키를 가져와 
//			 멤버DM(색인,종,수서정보,부록정보)을 구성한다.
//		2-6. Head/Tail 다이얼로그를 설정 및 생성한다.
//		2-7. 마크로부터 색인DM에 값을 설정한다.
//		2-8. 이전,다음, 새로저장버튼을 보이게 한다.
// 5. 우선정리자료와 부록정보 사용여부에 따라 종타입을 체크하여 우선정리자료체크박스와 부록정보버튼을 나타낸다.
// 6. 수입구분에 따라 구입신청자/신청일자를 활성화/비활성화 한다.
// 7. 복본기호 부여기준 설정
// 8. FormApi에 추가
// 9. 입력시 기본값 및 이전내용이 적용되어 지지 말아야할 Ctrl알리아스를 설정한다.
// 10. 총책수 및 총권수를 비활성화 시킨다.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::Init()
{
	//=======================================================================
	// 변수 정의
	//=======================================================================
	INT ids = -1;
	CString strValue;

	
	//=======================================================================
	// 1. 종/폼 API 초기화한다.
	//=======================================================================	
	InitSpeciesApi();	//종API초기화	
	InitFormApi();		//폼API초기화
	

	//=======================================================================
	// 2. 멤버 CM을 설정한다.
	//=======================================================================	
	m_pCM_Index	= FindCM(_T("CMUP_단행_종_색인정보"));
	m_pCM_Accu	= FindCM(_T("CMUP_단행_종_구입정보"));
	if( m_pCM_Index == NULL || m_pCM_Accu == NULL )
	{
		m_strLastErrMsg = _T("[CMUP_단행_종_색인정보/단행_종_구입정보]를 설정할 수 없습니다.");
		return -1;
	}		
	
	//=======================================================================
	// 3. 자동 복본조사 모드를 설정한다.
	//=======================================================================	
	GetManageValue(_T("수서"), _T("단행"), _T("수서종화면복본조사검색조건"), _T(""), strValue);
	if( strValue == _T("Y") )
	{
		m_bDupSearchType = TRUE;
		m_bAutoDupSearch = FALSE;

		GetDlgItem(IDC_bDUP_SEARCH)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_bDupSearchType = FALSE;

		GetManageValue(_T("수서"), _T("단행"), _T("수서서명자동복본조사"), _T(""), strValue);
		if (strValue == _T("Y"))		m_bAutoDupSearch = TRUE;
		else 
		{
			m_bAutoDupSearch = FALSE;
			GetDlgItem(IDC_bDUP_SEARCH)->ShowWindow(SW_SHOW);
		}
	}


	//=======================================================================
	// 4. 오픈타입에 따라 다음 내용을 수행한다.	
	//		[입력]
	//		1-1. DM설정 및 초기화 한다.
	//		1-2. 자료상태를 설정한다.
	//		1-3. 폼 API에 모드를 설정한다.
	//		1-4. 색인, 종, 책, 구입정보 DM에 Row를 하나 추가한다.
	//		1-5. 입수구분이 구입이라면, 구입정보 DM에 BP_차수업무구분에 _T("1")을 셋팅한다.
	//		1-6. Head/Tail 다이얼로그를 설정 및 생성한다.
	//		1-7. _T("화면지우기")와 _T("이전내용") 버튼을 보이게 한다.
	//		1-8. 초기값을 불러온다.
	//
	//		[수정]
	//		2-1. 부모그리드의 선택된 로우를 구성한다.
	//		2-2. 종타입에따라 DM 및 작업상태를 설정한다. (입력에서의 1,2번)
	//		2-3. 폼 API에 모드를 설정한다.
	//		2-4. 그리드를 반전 표시한다.
	//		2-5. 멤버변수로 설정된 선정로우(m_nParentSelectedRow)의 종키와 수서정보키를 가져와 
	//			 멤버DM(색인,종,수서정보,부록정보)을 구성한다.
	//		2-6. Head/Tail 다이얼로그를 설정 및 생성한다.
	//		2-7. 마크로부터 색인DM에 값을 설정한다.
	//		2-8. 이전,다음, 새로저장버튼을 보이게 한다.
	//=======================================================================
	switch( m_nOpType )
	{
	// 종-입력
	case SINSERT:
	{
			// 1. DM설정 및 초기화 한다.
		SetDataManagers();

			// 2. 자료상태를 설정한다.
		SetWorkingStatus();
			
			// 3. 폼 API에 모드를 설정한다.
		m_pFormApi->SetAppMode(MODE_INSERT);
			
		// 4. 색인, 종, 책, 구입정보 DM에 Row를 하나 추가한다.
		m_pDM_Index->InsertRow(-1);											// 색인
		m_pDM_Species->InsertRow(-1);										// 종
		m_pDM_Book->InsertRow(-1);											// 책
		if( m_pDM_Accu->GetRowCount() == 0 )	m_pDM_Accu->InsertRow(-1);	// 구입정보

		// 5. 입수구분이 구입이라면, 구입정보 DM에 BP_차수업무구분에 _T("1")을 셋팅한다.
		if( m_nAcqType == PURCHASE )			m_pDM_Accu->SetCellData(_T("BP_차수업무구분"), _T("1"), 0);
		
		// 6. Head/Tail 다이얼로그를 설정 및 생성한다.
		SetHeadTailInfoDlg();

			// 7. _T("화면지우기")와 _T("이전내용") 버튼을 보이게 한다.
		GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnPREV_CONTENT)->ShowWindow(SW_SHOW);
		SetWindowText(_T("서지정보입력"));

		m_pTailDlg->SetInit(FALSE);

		// 8. 구입신청일 시간 세팅
		CString strDate = m_pBOSpeciesApi->GetTodayDate();
		CString sDate, strTime;
		CTime t = CTime::GetCurrentTime();
		//strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());		// 입력일
		sDate.Format(_T("%s %02d:%02d:%02d"), strDate, t.GetHour(), t.GetMinute(), t.GetSecond());		// 입력일
		strApplicantTime(sDate);
			
			// 8. 초기값을 불러온다.
		LoadDefaultValue();
	}
		break;
	// 수정
	case SUPDATE:
	case GUPDATE:
	{
			// 1. 부모그리드의 선택된 로우를 구성한다.
		m_nParentSelectedRow = GetFirstSelectIndex();

			// 2. 종타입에따라 DM 및 작업상태를 설정한다. (입력에서의 1,2번)
		UpdateInitProc();

			// 3. 폼 API에 모드를 설정한다.
			m_pFormApi->SetAppMode(MODE_UPDATE);

			// 4. 그리드를 반전 표시한다.
		m_pGrid_Parent->SetReverse(m_nParentSelectedRow);

			// 5. 멤버변수로 설정된 선정로우(m_nParentSelectedRow)의 종키와 수서정보키를 가져와 
			//	  멤버DM(색인,종,수서정보,부록정보)을 구성한다.
		ids = MakeUpdateDM();
			if( ids < 0 ) 
		{
				ESLAfxMessageBox(_T("UPDATE DM 생성 실패"));
			return -1;
		}
		
			// 6. Head/Tail 다이얼로그를 설정 및 생성한다.
		SetHeadTailInfoDlg();

			// 7. 마크로부터 색인DM에 값을 설정한다.		
		ApplyMarcToIndexCM();	

			// 8. 이전,다음, 새로저장버튼을 보이게 한다.
		GetDlgItem(IDC_bSAVEAS		)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bPREV_SPECIES)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_bNEXT_SPECIES)->ShowWindow(SW_SHOW);
		SetWindowText(_T("서지정보수정"));
		
		m_pTailDlg->SetInit(TRUE);		

		Display();		
	}
		break;

	default:	return -1;
		break;

	}

	//=======================================================================
	// 5. 우선정리자료와 부록정보 사용여부에 따라 종타입을 체크하여 우선정리자료체크박스와 부록정보버튼을 나타낸다.
	//=======================================================================
	ShowControlBySpeciesType();	

	//=======================================================================
	// 6. 수입구분에 따라 구입신청자/신청일자를 활성화/비활성화 한다.
	//=======================================================================
	ShowControlByAcqType();	

	//=======================================================================
	// 7. 복본기호 부여기준 설정
	//=======================================================================
	m_cCopyCodeMgr.Init(m_pDM_Index->CONNECTION_INFO);	
	
	//=======================================================================
	// 8. FormApi에 추가
	//=======================================================================
	m_pFormApi->SetCopyCodeMgr(&m_cCopyCodeMgr);	

	//=======================================================================
	// 9. 입력시 기본값 및 이전내용이 적용되어 지지 말아야할 Ctrl알리아스를 설정한다.
	//=======================================================================
	m_arrIgnoreCTRLAlias.Add(_T("IBS_관리구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_매체구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_별치기호_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_보조등록구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_분류표구분"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_업무구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_이용대상구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_이용제한구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_자료구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_제어자료구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_형식구분_코드"));
	m_arrIgnoreCTRLAlias.Add(_T("BP_주제구분_코드"));	
	m_arrIgnoreCTRLAlias.Add(_T("자료구분"));
	m_arrIgnoreCTRLAlias.Add(_T("BP_총권수"));
	m_arrIgnoreCTRLAlias.Add(_T("BP_총책수"));
	m_arrIgnoreCTRLAlias.Add(_T("구입신청자"));
	// 2006.03.31 KBS - 국방대 요구사항 추가
	m_arrIgnoreCTRLAlias.Add(_T("IBS_도서분류코드1"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_도서분류코드2"));
	m_arrIgnoreCTRLAlias.Add(_T("IBS_도서분류코드3"));	

	//=======================================================================
	// 10. 총책수 및 총권수를 비활성화 시킨다.
	//=======================================================================
	((CEdit*)m_pCM_Accu->FindControl(_T("BP_총책수")))->SetReadOnly(TRUE);
	((CEdit*)m_pCM_Accu->FindControl(_T("BP_총권수")))->SetReadOnly(TRUE);	

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 종타입에따라 DM 및 작업상태를 설정한다.
//
// [ 미니스펙 ]
// 1. 종타입이 등록이라면 아래 내용을 처리한다.
//		i.	수입구분값을 가져와 멤버변수랑 비교하여 변경되었는지 체크한다.
//		ii.	수입구분값에따라 종타입과 입수구분을 멤버변수에 설정한다.
//		iii.그룹정보가 있다면 모두 삭제하고 다시 설정한다.
// 2. 멤버변수 DM을 설정하고 내용을 초기화한다.
// 3. 종타입에따라 현재 입력창 왼쪽 맨위에 작업상태를 설정한다.
//
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::UpdateInitProc()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	CString strAcqCode;

	//=======================================================================
	// 1. 종타입이 등록이라면 아래 내용을 처리한다.
	//		i.	수입구분값을 가져와 멤버변수랑 비교하여 변경되었는지 체크한다.
	//		ii.	수입구분값에따라 종타입과 입수구분을 멤버변수에 설정한다.
	//		iii.그룹정보가 있다면 모두 삭제하고 다시 설정한다.
	//=======================================================================
	if( m_nSpeciesType == BO_REG_INVALID	|| 
		m_nSpeciesType == BO_REG_PURCHASE	|| 
		m_nSpeciesType == BO_REG_DONATE		)
	{
		strAcqCode = m_pDM_Parent->GetCellData(_T("BB_수입구분"), m_nParentSelectedRow);
		// i.
		if( m_nAcqType != (ACQ_TYPE)_ttoi(strAcqCode.GetBuffer(0)) )	
		{
				m_bAcqTypeChange = TRUE;
		}
		else m_bAcqTypeChange = FALSE;

		// ii.
		if( strAcqCode == _T("0") || strAcqCode.IsEmpty() == TRUE ) 
		{
			m_nSpeciesType = BO_REG_INVALID;
			m_nAcqType = ACQ_TYPE_INVALID;
		}
		else if( strAcqCode == _T("1") )
		{
			m_nSpeciesType = BO_REG_PURCHASE;
			m_nAcqType = PURCHASE;
		}
		else if( strAcqCode == _T("2") )
		{
			m_nSpeciesType = BO_REG_DONATE;
			m_nAcqType = DONATE;
		}
		else 
		{
			m_nSpeciesType = BO_REG_INVALID;
			m_nAcqType = OTHER_ACQ_TYPE;
		}

		// iii.
		if( m_pGroupCollection )
		{
			m_pGroupCollection->RemoveAll();
			m_pGroupCollection->SetAcqType(m_nAcqType);
			m_pGroupCollection->SetAcqCode(strAcqCode);
		}
	}

	//=======================================================================
	// 2. 멤버변수 DM을 설정하고 내용을 초기화한다.
	//=======================================================================
	SetDataManagers();

	//=======================================================================
	// 3. 종타입에따라 현재 입력창 왼쪽 맨위에 작업상태를 설정한다.
	//=======================================================================
	SetWorkingStatus();
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 우선정리자료와 부록정보 사용여부에 따라 종타입을 체크하여 우선정리자료체크박스와 부록정보버튼을 나타낸다.
//
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::ShowControlBySpeciesType()
{
	// 숨김으로 초기화
	GetDlgItem(IDC_bAPPENDIX			)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_chkPRIORITY_CATALOG	)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_tPRIORITY_CATALOG	)->ShowWindow(SW_HIDE);

	//종화면을 호출하는 업무에 따라서 부록정보 버튼의 SHOW/HIDE를 설정하고 화면지우기 버튼을 이동한다.
	// 2004.10.13 5163 우선정리자료, 부록정보버튼에 따른 컨트롤 보이기/숨기기
	// BEGIN:
	if( m_strPriorityCatalogYN == _T("Y") || m_strAcqAppendixYN == _T("Y") )
	{
		// 2005-01-24 기증, 기증등록대상도 추가.
		if( m_nSpeciesType == BO_PURCHASE							|| 
			m_nSpeciesType == BO_DONATE								|| 
			m_nSpeciesType == BO_ORDER								|| 
			m_nSpeciesType == BO_MISSING							|| 
			(m_nSpeciesType == BO_REGOBJ && m_nAcqType == PURCHASE) || 
			(m_nSpeciesType == BO_REGOBJ && m_nAcqType == DONATE)	||
			m_nSpeciesType == BO_REG_PURCHASE						|| 
			m_nSpeciesType == BO_REG_DONATE							|| 
			m_nSpeciesType == BO_REG_INVALID						|| 
			m_nSpeciesType == BO_CAT								)
		{
			// 2004.10.13 5163_1.구입,등록단계에서 부록정보 버튼 보여준다.
			if( m_strAcqAppendixYN == _T("Y") )
			{
				GetDlgItem(IDC_bAPPENDIX)->ShowWindow(SW_SHOW);
			}

			// 2004.10.13 5163_1.구입,등록단계에서 우선정리자료로 선택할 수 있도록 체크박스를 보여준다.
			if( m_strPriorityCatalogYN == _T("Y") )
			{
				GetDlgItem(IDC_chkPRIORITY_CATALOG	)->ShowWindow(SW_SHOW);
				GetDlgItem(IDC_tPRIORITY_CATALOG	)->ShowWindow(SW_SHOW);
			}
			}
		}
	// END:
		else
		{
		if( m_nSpeciesType == BO_REG_PURCHASE	|| 
			m_nSpeciesType == BO_REG_DONATE		|| 
			m_nSpeciesType == BO_REG_INVALID	|| 
			m_nSpeciesType == BO_CAT			)
	{
			GetDlgItem(IDC_bAPPENDIX)->ShowWindow(SW_SHOW);
		}
	}

}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::ShowCodeResource()
{
	INT nCount, nCodeResourcesHeight, i;	
	INT nArrCodeResourceID[256];
	INT nArrOtherResources[256];
	CWnd *pWnd = NULL;
	CRect rcWindow, rcResource;

	//=======================================================================
	// 1. 간략검색/상세검색 안에 있는 리소스들을 설정하고 보이기 및 숨기기를 설정한다.
	//20070829 UPDATE PJW : 간략검색/상세검색  -> 간략보기/상세보기
	//=======================================================================
	if( m_bShowCode == TRUE )	GetDlgItem(IDC_btnBSFORM_SHOW_HIDE_CODE)->SetWindowText(_T("간략보기"));
	else						GetDlgItem(IDC_btnBSFORM_SHOW_HIDE_CODE)->SetWindowText(_T("상세보기"));
	
	/**************************************************************
	 **************************************************************/
	//20071205 ADD PJW : 비고 글자수 제한 수정
	//20080522 UPDATE PJW : 비고 글자수 200으로 수정
	
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_eDISCRIPTION);
	pEdt->SetLimitText(200);
	CEdit* pEdt1 = (CEdit*)GetDlgItem(IDC_eDISCRIPTION_SMALL);
	pEdt1->SetLimitText(200);
	/**************************************************************
	 **************************************************************/

	GetWindowRect(rcWindow);
	if( m_bShowCode == TRUE && rcWindow == m_rcWindow )		return;

	nCount = 0;	
	nCodeResourcesHeight = 97;
	nArrCodeResourceID[nCount++] = IDC_STATIC_CODE;
	nArrCodeResourceID[nCount++] = IDC_tSUB_REG_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cSUB_REG_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMEDIA_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tUSE_OBJECT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cUSE_OBJECT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tUSE_LIMIT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cUSE_LIMIT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tSHELF_CODE;
	nArrCodeResourceID[nCount++] = IDC_cCLASIFY_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tCONTROL_MAT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cCONTROL_MAT_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tMANAGE_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cMANAGE_CLASS;
	nArrCodeResourceID[nCount++] = IDC_tCLASIFY_CLASS;
	nArrCodeResourceID[nCount++] = IDC_cSHELF_CODE;
	nArrCodeResourceID[nCount++] = IDC_tDISCRIPTION;
	nArrCodeResourceID[nCount++] = IDC_eDISCRIPTION;
	// 2004.11.04 5163_3. 사용자 정의코드, 표지추가기능 추가
	// BEGIN:
	if( _T("Y") == m_strUseCodeYN )
	{
		nArrCodeResourceID[nCount++] = IDC_eDISCRIPTION_SMALL;
		nArrCodeResourceID[nCount++] = IDC_stcUSERCODE3;
		nArrCodeResourceID[nCount++] = IDC_cmbUSERCODE3;
	}
	
	// Show / Hide Code Resource
	for( i=0; i<nCount; i++ )
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if( pWnd->GetSafeHwnd() != NULL )
		{
			if( m_bShowCode == TRUE )	pWnd->ShowWindow(SW_SHOW);
			else						pWnd->ShowWindow(SW_HIDE);
		}
	}
	
	//=======================================================================
	// 2004.11.04 
	// 2. 사용자 정의코드 설정한다.
	//=======================================================================
	// BEGIN:
	if( _T("Y") == m_strUseCodeYN )
	{
		ShowUserDefineCode(m_bShowCode);
	}
	else // m_strUseCodeYN == _T("N") 일경우 사용자 정의코드 모두 숨김.
	{
		pWnd = GetDlgItem(IDC_stcUSERCODE1);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE1);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_stcUSERCODE2);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE2);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_stcUSERCODE3);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE3);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_eDISCRIPTION_SMALL);	pWnd->ShowWindow(SW_HIDE);
	}
	// END:


	//=======================================================================
	// 3. 이외의 나머지 리소스들을 설정한다.
	//=======================================================================
	// Move Other Resources
	nCount = 0;
	nArrOtherResources[nCount++] = IDC_STATIC_MARC;
	nArrOtherResources[nCount++] = IDC_tTITLE;
	nArrOtherResources[nCount++] = IDC_eTITLE;
	nArrOtherResources[nCount++] = IDC_bDUP_SEARCH;
	nArrOtherResources[nCount++] = IDC_tAUTHOR1;
	nArrOtherResources[nCount++] = IDC_eAUTHOR1;
	nArrOtherResources[nCount++] = IDC_tEDITION_STMT;
	nArrOtherResources[nCount++] = IDC_eEDITION_STMT;
	nArrOtherResources[nCount++] = IDC_tVOL;
	nArrOtherResources[nCount++] = IDC_eVOL;
	nArrOtherResources[nCount++] = IDC_tSERIES_TITLE;
	nArrOtherResources[nCount++] = IDC_eSERIES_TITLE;
	nArrOtherResources[nCount++] = IDC_ePUBLISH_PLACE;
	nArrOtherResources[nCount++] = IDC_tPUBLISH_PLACE;
	nArrOtherResources[nCount++] = IDC_ePUBLISHER;
	nArrOtherResources[nCount++] = IDC_tPUBLISHER;
	nArrOtherResources[nCount++] = IDC_ePUBLISH_YEAR;
	nArrOtherResources[nCount++] = IDC_tPUBLISH_YEAR;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN_ADD;
	nArrOtherResources[nCount++] = IDC_eEA_ISBN_MISTAKE;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN_ADD;
	nArrOtherResources[nCount++] = IDC_eSET_ISBN_MISTAKE;
	nArrOtherResources[nCount++] = IDC_tEA_ISBN;
	nArrOtherResources[nCount++] = IDC_bEXTRA_ISBN;
	nArrOtherResources[nCount++] = IDC_tSET_ISBN;
	nArrOtherResources[nCount++] = IDC_bISBN_CHECK;
	nArrOtherResources[nCount++] = IDC_STATIC_FORM_ELEMENT;
	nArrOtherResources[nCount++] = IDC_tPAGE;
	nArrOtherResources[nCount++] = IDC_tPHYSICAL_CHARACTER;
	nArrOtherResources[nCount++] = IDC_stcSIZE;
	nArrOtherResources[nCount++] = IDC_stcAPPENDIX;
	nArrOtherResources[nCount++] = IDC_edtAPPENDIX;
	nArrOtherResources[nCount++] = IDC_edtSIZE;
	nArrOtherResources[nCount++] = IDC_edtPHYSICAL_CHARACTER;
	nArrOtherResources[nCount++] = IDC_edtPAGE;
	nArrOtherResources[nCount++] = IDC_STATIC_BOOK_REGCODE;
	nArrOtherResources[nCount++] = IDC_tREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_tTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_tTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_bAPPLY_SERIES;
	nArrOtherResources[nCount++] = IDC_bVOL_INFO;
	nArrOtherResources[nCount++] = IDC_eTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_eTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_eREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_bAPPENDIX;
	nArrOtherResources[nCount++] = IDC_bMARC_EDIT;
	nArrOtherResources[nCount++] = IDC_bMARC_IMPORT;
	nArrOtherResources[nCount++] = IDC_bKOLIS_NET;
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_bPREV_SPECIES;
	nArrOtherResources[nCount++] = IDC_bNEXT_SPECIES;
	nArrOtherResources[nCount++] = IDC_bSAVEAS;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	nArrOtherResources[nCount++] = IDC_STATIC_ISBN;
	nArrOtherResources[nCount++] = IDC_bDEFAULT_CONFIG;
	nArrOtherResources[nCount++] = IDC_tSERIES_VOL;
	nArrOtherResources[nCount++] = IDC_edtSERIES_VOL;
	nArrOtherResources[nCount++] = IDC_btnPREV_CONTENT;
	nArrOtherResources[nCount++] = IDC_stcBSFORM_REQUESTOR;
	nArrOtherResources[nCount++] = IDC_edtBSFORM_REQUESTOR;
	nArrOtherResources[nCount++] = IDC_stcBSFORM_REQUEST_DATE;
	nArrOtherResources[nCount++] = IDC_edtBSFORM_REQUEST_DATE;
	//20070810 ADD PJW : 신청 시간을 위해서 IDC_edtBSFORM_REQUEST_HOUR 추가
	nArrOtherResources[nCount++] = IDC_edtBSFORM_REQUEST_HOUR;
	nArrOtherResources[nCount++] = IDC_STATIC_EBOOK_GROUP;
	nArrOtherResources[nCount++] = IDC_stcEBOOK_COVER_FILE;
	nArrOtherResources[nCount++] = IDC_edtEBOOK_COVER_FILE;
	nArrOtherResources[nCount++] = IDC_btnFIND_FILE1;
	nArrOtherResources[nCount++] = IDC_btnFIND_FILE2;
	nArrOtherResources[nCount++] = IDC_btnFIND_FILE3;
	nArrOtherResources[nCount++] = IDC_stcEBOOK_FILE;
	nArrOtherResources[nCount++] = IDC_edtEBOOK_FILE;
	nArrOtherResources[nCount++] = IDC_stcEBOOK_EXECUTE_FILE;
	nArrOtherResources[nCount++] = IDC_edtEBOOK_EXECUTE_FILE;
	nArrOtherResources[nCount++] = IDC_btnEBOOK_VIEW;
	//nArrOtherResources[nCount++] = IDC_APPLICAND_KIND;
	nArrOtherResources[nCount++] = IDC_cAPPLICAND_KIND_DEPT;
	nArrOtherResources[nCount++] = IDC_cAPPLICAND_KIND_COURCE;
	nArrOtherResources[nCount++] = IDC_cAPPLICAND_KIND_CLASS;
	nArrOtherResources[nCount++] = IDC_cmbAPPLICANT;
	nArrOtherResources[nCount++] = IDC_tClass_Kind;
	nArrOtherResources[nCount++] = IDC_tClass_Code;


	GetDlgItem(IDC_tClass_Kind)->ShowWindow(SW_SHOW);
	OnSelchangecmbAPPLICANT();

	// 2004.11.09 5163 표지추가 기능 사용.
	// BEGIN:
	if( _T("Y") == m_strUseCoverYN )		nArrOtherResources[nCount++] = IDC_btnCover;
	// END:
	
	// 2004.12.29 5163일 경우 화면에 대출자조회 버튼을 사용하여 입력 가능하도록 한다.
	if( _T("Y") == m_strIs5163 )
	{
		//nArrOtherResources[nCount++] = IDC_edtUserName;
		nArrOtherResources[nCount++] = IDC_btnSearchUser;
	}

	for( i=0; i<nCount ; i++ )
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if( pWnd->GetSafeHwnd() != NULL )
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if( m_bShowCode == TRUE )
			{
				rcResource.top += nCodeResourcesHeight;
				rcResource.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
			else
			{
				rcResource.top -= nCodeResourcesHeight;
				rcResource.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

	// m_strUseCoverYN == _T("N") 일경우 표지 추가 버튼 숨김.
	// BEGIN:
	if( _T("N") == m_strUseCoverYN )
	{
		pWnd = GetDlgItem(IDC_btnCover);			
		pWnd->ShowWindow(SW_HIDE);
	}
	// END:

	// 2004.12.29 5163의 경우 대출자조회 버튼 보여준다.
	if( _T("Y") == m_strIs5163 )
	{
		pWnd = GetDlgItem(IDC_btnSearchUser);			
		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd = GetDlgItem(IDC_btnSearchUser);			
		pWnd->ShowWindow(SW_HIDE);
	}

	MoveTailDlg(nCodeResourcesHeight);
	
	// Resize Window Size
	GetWindowRect(rcResource);
	if( m_bShowCode == TRUE )
	{
		if( m_bShowEBookInfo == TRUE )
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height()-65,  SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		if( m_bShowEBookInfo == TRUE )
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - (nCodeResourcesHeight),  SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - (nCodeResourcesHeight+65),  SWP_NOMOVE | SWP_NOZORDER);
	}

	//=========================================================
	//2008.10.20 ADD BY PJW : MANAGE_CODE 컨트롤은 숨긴다.
	pWnd = GetDlgItem(IDC_cMANAGE_CLASS);		pWnd->ShowWindow(SW_HIDE);
	pWnd = GetDlgItem(IDC_tMANAGE_CLASS);		pWnd->ShowWindow(SW_HIDE);
	//=========================================================
	
	Invalidate();

	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 멤버변수로 설정된 선정로우의 종키와 수서정보키를 가져와 멤버DM(색인,종,수서정보,부록정보)을 구성한다.
//
// [ 미니스펙 ]
// 1. 선택한 Row의 SpeciesKey, PurchaseKey를 가져온다.
// 2. 가져온 종키를 가지고 색인DM을 구성한다.(바인드처리)
// 3. 종KEY를 멤버변수로 복사한다.
// 4. 색인 DM에 정보가 없을 경우 새로 생성한다.
// 5. 색인DM의 작업상태가 등록 및 정리라면 복본추기를 설정한다.
// 6. 종DM을 구성한다. (바인드처리)
// 7. 종DM에 정보가 없을경우 새로생성한다.
// 8. 수서정보DM을 구성한다.
// 9. 책정보DM을 구성한다.
// 10. 책 복사할때 기본적으로 무시할 필드를 설정한다.
// 11. 수서정보키가 없거나 -1이라면 수서정보DM을 설정한다.
// 12. 부록정보DM을 구성한다.
// 13. 비치희망정보를 설정한다. (구입신청자/구입신청일) [BO_FURNISH_TBL]
// 14. 등록에서의 구입이라면 차수업무구분을 설정한다.
// 15. 종DM의 제어자료구분 및 비고를 색인DM에 설정한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::MakeUpdateDM()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids, nRepresentBookIdx = -1;
	CString strSpeciesKey, strAccuKey, strOption;
	EDBM_BIND_DATA_TYPE* pBind = NULL;

	
	if( m_nParentSelectedRow < 0 )		return -1;	
	

	//=======================================================================
	// 1. 선택한 Row의 SpeciesKey, PurchaseKey를 가져온다.
	//=======================================================================
	m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);
	m_pDM_Parent->GetCellData(m_strAccuKeyAlias, m_nParentSelectedRow, strAccuKey);
	if( strSpeciesKey.IsEmpty() == TRUE )	return -1;
	if( strAccuKey.IsEmpty() == TRUE && 
		(m_nSpeciesType != BO_BASIC && m_nSpeciesType != BO_IMPORT && m_nSpeciesType != BO_REG_INVALID) ) return -1;


	//=======================================================================
	// 2. 가져온 종키를 가지고 색인DM을 구성한다.(바인드처리)
	//=======================================================================
	InitEDBMBindDataType(&pBind, 0, 1);
	strOption.Format(_T("REC_KEY = :SPECIES_KEY") );
	AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
	ids = m_pDM_Index->RefreshDataManager(strOption, NULL, pBind);
	FreeEDBMBindData(&pBind, 1);
	if( ids < 0 )	return -1;	

	//=======================================================================
	// 3. 종KEY를 멤버변수로 복사한다.
	//=======================================================================
	_tcscpy ( m_szSpeciesKey , strSpeciesKey.GetBuffer(0) );
	
	
	//=======================================================================
	// 4. 색인 DM에 정보가 없을 경우 새로 생성한다.
	//=======================================================================
	if( m_pDM_Index->GetRowCount() == 0 )
	{
		if( IDNO == ESLAfxMessageBox(_T("색인정보가 존재하지 않습니다. 새로생성 하시겠습니까?."), MB_YESNO) )
			return -1;

		m_pDM_Index->InsertRow(-1);
		m_pDM_Index->SetCellData(_T("IBS_REC_KEY"), strSpeciesKey, 0);
		m_pDM_Index->StartFrame();
		CArray<CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
		m_pDM_Index->MakeQueryByDM(RemoveAliasList, m_pDM_Index, m_pDM_Index->TBL_NAME, 0, m_pDM_Index);
		ids = m_pDM_Index->SendFrame();
		if( ids < 0 ) return ids;
		ids = m_pDM_Index->EndFrame();
		if( ids < 0 ) return ids;
	}	

	//=======================================================================
	// 5. 색인DM의 작업상태가 등록 및 정리라면 복본추기를 설정한다.
	//=======================================================================
	if( m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0).Left(3) == _T("BOL")	||
		m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0).Left(3) == _T("BOC")	)
	{
		m_bDupAddYN = TRUE;
	}
	else
	{
		m_bDupAddYN = FALSE;
	}

	//=======================================================================
	// 6. 종DM을 구성한다. (바인드처리)
	//=======================================================================
	pBind = NULL;
	InitEDBMBindDataType( &pBind, 0, 1);
	strOption.Format(_T("REC_KEY = :SPECIES_KEY"));
	AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
	ids = m_pDM_Species->RefreshDataManager(strOption, NULL, pBind);
	FreeEDBMBindData(&pBind, 1);
	if( ids < 0 ) return -1;
	
	//=======================================================================
	// 7. 종DM에 정보가 없을경우 새로생성한다.
	//=======================================================================
	if(m_pDM_Species->GetRowCount() == 0)
	{
		if(IDNO == ESLAfxMessageBox(_T("종정보가 존재하지 않습니다. 새로생성 하시겠습니까?."), MB_YESNO))
			return -1;
		m_pDM_Species->InsertRow(-1);
		m_pDM_Species->SetCellData(_T("BS_종KEY"), strSpeciesKey, 0);
		m_pDM_Species->StartFrame();
		CArray<CString, CString> RemoveAliasList;	RemoveAliasList.RemoveAll();
		m_pDM_Species->MakeQueryByDM(RemoveAliasList, m_pDM_Species, m_pDM_Species->TBL_NAME, 0, m_pDM_Species);
		ids = m_pDM_Species->SendFrame();
		if(ids < 0) return ids;
		ids = m_pDM_Species->EndFrame();
		if(ids < 0) return ids;	
	}
	
	//=======================================================================
	// 8. 수서정보DM을 구성한다.
	//=======================================================================
	if( strAccuKey.IsEmpty() == FALSE  && strAccuKey != _T("-1") )
	{
		pBind = NULL;
		InitEDBMBindDataType( &pBind, 0, 1);

		strOption.Format(_T("REC_KEY = :REC_KEY "));
		AddEDBMBindDataByString(pBind, STRING, _T(":REC_KEY"), strAccuKey.GetBuffer(0) );

		ids = m_pDM_Accu->RefreshDataManager(strOption, NULL, pBind);
		FreeEDBMBindData(&pBind, 1);
		if( ids < 0 ) return ids;
	}
	
	//=======================================================================
	// 9. 책정보DM을 구성한다.
	//=======================================================================
	pBind = NULL;
	InitEDBMBindDataType( &pBind, 0, 1);
	
	if( strAccuKey.IsEmpty() == FALSE  && strAccuKey != _T("-1") )
	{
		strOption.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = :SPECIES_KEY AND B.ACQ_KEY = :ACQ_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC, VOL_SORT_NO"));
		AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
		AddEDBMBindDataByString(pBind, STRING, _T(":ACQ_KEY"), strAccuKey.GetBuffer(0) );
	}
	else
	{
		if( m_nSpeciesType == BO_BASIC )
		{
			strOption.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = :SPECIES_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND B.WORKING_STATUS = 'BOT111O' AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC, B.VOL_SORT_NO"));
			AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
		}
		else if(m_nSpeciesType == BO_IMPORT)
		{
			strOption.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = :SPECIES_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND B.WORKING_STATUS = 'BOT112O' AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC, B.VOL_SORT_NO"));
			AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
		}
		else if(m_nSpeciesType == BO_REG_INVALID || m_nSpeciesType == BO_REG_PURCHASE || m_nSpeciesType == BO_REG_DONATE)
		{
			strOption.Format(_T("B.REC_KEY=E.BOOK_KEY(+) AND B.SPECIES_KEY = :SPECIES_KEY AND B.BOOK_APPENDIX_FLAG = 'B' AND B.WORKING_STATUS BETWEEN 'BOR111N' AND 'BOR113O' AND B.REPRESENT_BOOK_YN IN ('Y', 'N') ORDER BY B.REPRESENT_BOOK_YN DESC, B.VOL_SORT_NO"));
			AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );
		}
		
	}
	
	ids = m_pFormApi->MakeBookDM(strOption, m_pDM_Book, pBind);
	FreeEDBMBindData(&pBind, 1);
	if( ids < 0 ) return ids;
	
	//=======================================================================
	// 10. 책 복사할때 기본적으로 무시할 필드를 설정한다.
	//=======================================================================
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_대표책여부"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_복본기호"));
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_배가위치부호"));	
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_DB_FLAG"));	

	
	//=======================================================================
	// 11. 수서정보키가 없거나 -1이라면 수서정보DM을 설정한다.
	//=======================================================================
	nRepresentBookIdx = -1;
	if( strAccuKey.IsEmpty() == TRUE || strAccuKey == _T("-1") )
	{
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nRepresentBookIdx);
		MakeAccuInfo(m_pDM_Species, m_pDM_Book, nRepresentBookIdx);
	}

	//=======================================================================
	// 12. 부록정보DM을 구성한다.
	//=======================================================================
	pBind = NULL;
	InitEDBMBindDataType( &pBind, 0, 1);
	strOption.Format(_T("SPECIES_KEY=:SPECIES_KEY AND BOOK_APPENDIX_FLAG='A'") );
	AddEDBMBindDataByString(pBind, STRING, _T(":SPECIES_KEY"), strSpeciesKey.GetBuffer(0) );

	// 2007.08.10 ADD BY PDJ
	// 원부번호를 보여주기 위해 UDF->STR로 변경
	SetUDFCheck(m_pDM_Appendix, _T("UDF_원부번호"), _T("str"));	
	ids = m_pDM_Appendix->RefreshDataManager(strOption, NULL, pBind);
	FreeEDBMBindData(&pBind, 1);
	SetUDFCheck(m_pDM_Appendix, _T("UDF_원부번호"), _T("udf"));

	// 2007.08.10 ADD BY PDJ
	// UPDATE시 원부번호 제외를 위해 UDF로 변환
	if(ids < 0) return ids;
	
	//=======================================================================
	// 13. 비치희망정보를 설정한다. (구입신청자/구입신청일) [BO_FURNISH_TBL]
	//=======================================================================
	SetFurnishInfo();

	//=======================================================================
	// 14. 등록에서의 구입이라면 차수업무구분을 설정한다.
	//=======================================================================
	if( m_nSpeciesType == BO_REG_PURCHASE && m_nAcqType == PURCHASE )
	{
		m_pDM_Accu->SetCellData(_T("BP_차수업무구분"), m_pDM_Index->GetCellData(_T("IBS_업무구분_코드"), 0), 0);
	}

	//=======================================================================
	// 15. 종DM의 제어자료구분 및 비고를 색인DM에 설정한다.
	//=======================================================================
	m_pDM_Index->SetCellData(_T("IBS_제어자료구분_코드"), m_pDM_Species->GetCellData(_T("BS_제어자료구분"), 0), 0);
	m_pDM_Index->SetCellData(_T("IBS_비고"), m_pDM_Species->GetCellData(_T("BS_비고"), 0), 0);

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 마크로 부터 다음정보를 가져와 색인DM에 설정한다.
//		1.	250$a - IBS_판사항
//		2.	260$a - IBS_발행지
//		3.	300$a - IBS_면장수
//		4.	300$b - IBS_물리적특성
//		5.	300$c - IBS_크기
//		6.	300$e - IBS_딸림자료
//		7.	020$a - IBS_낱권ISBN
//		8.	020$g - IBS_낱권ISBS부가기호
//		9.	020$z - IBS_낱권오류ISBN
//		10.	020$a - IBS_세트_ISBN1
//		11.	020$g - IBS_세트_ISBN2
//		12.	020$z - IBS_세트_ISBN3
//		13.	490$a - IBS_총서명
//		14.	440$a - IBS_총서명
//		15.	440$v - IBS_총서편차
//		16.	950$a - BP_가격성격
//		17.	245$n - IBS_편권차
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::ApplyMarcToIndexCM()
{
	CString strMarc, strMarcData1;
	CMarc marc;

	m_pDM_Species->GetCellData(_T("BS_MARC"), 0, strMarc);
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("250$a"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("250$a"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_판사항"), strMarcData1, 0);
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("260$a"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("260$a"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_발행지"), strMarcData1, 0);
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("300$a"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("300$a"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_면장수"), strMarcData1, 0);	
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("300$b"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("300$b"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_물리적특성"), strMarcData1, 0);	
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("300$c"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("300$c"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_크기"), strMarcData1, 0);	
	//m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("300$e"), _T("  "), strMarcData1);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("300$e"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_딸림자료"), strMarcData1, 0);	
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$a"), _T("  "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_낱권ISBN"), strMarcData1, 0);	
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$g"), _T("  "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_낱권ISBN부가기호"), strMarcData1, 0);
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$z"), _T("  "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_낱권오류ISBN"), strMarcData1, 0);
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$a"), _T("1 "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_세트_ISBN1"), strMarcData1, 0);
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$g"), _T("1 "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_세트_ISBN2"), strMarcData1, 0);
	m_pBOSpeciesApi->FindItemByIndicator(&marc, _T("020$z"), _T("1 "), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_세트_ISBN3"), strMarcData1, 0);

	m_pInfo->pMarcMgr->GetItem(&marc, _T("490$a"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_총서명"), strMarcData1, 0);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("440$a"), strMarcData1);
	if( strMarcData1.IsEmpty() == FALSE ) 
	{
		m_pDM_Index->SetCellData(_T("IBS_총서명"), strMarcData1, 0);
	}
	m_pInfo->pMarcMgr->GetItem(&marc, _T("440$v"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_총서편차"), strMarcData1, 0);
	m_pInfo->pMarcMgr->GetItem(&marc, _T("950$a"), strMarcData1);
	m_pDM_Accu->SetCellData(_T("BP_가격성격"), strMarcData1, 0);

	//편권차 추가다.
	m_pInfo->pMarcMgr->GetItem(&marc, _T("245$n"), strMarcData1);
	m_pDM_Index->SetCellData(_T("IBS_편권차"), strMarcData1, 0);

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 종DM과 책DM을 이용하여 수서정보DM을 설정한다.
//
// [ 미니스펙 ]
// 1. 대표책의 인덱스가 설정되어 있지 않다면 책DM에서 대표책을 설정하고 대표책인덱스를 가져온다.
// 2. 종DM에서 마크정보를 가져와 구조체마크를 구성한다.
// 3. 수서정보DM에 로우를 추가한다.
// 4. 마크에서 가격정보를 가져와 수서정보DM에 가격과 화폐구분에 값을 설정한다.
// 5. 마크에서 가격잡정보를 가져와 수서정보DM에 가격잡정보를 설정한다.
// 6. 책DM에서 대표책의 그룹번호를 가져와 수서정보DM에 설정한다.
// 7. 책DM에서 대표책의 수입년도를 가져와 수서정보DM에 설정한다.
// 8. 책DM에서 대표책의 입력일자를 가져와 수서정보DM에 설정한다.
// 9. 수서정보DM에 총책수를 설정한다.
// 10. 수서정보DM에 총권수를 설정한다.
// 11. 수서정보M에 주제구분_코드를 설정한다.
// 12. 복본추기할 때 Species_key가 필요하므로 Species_Key를 Set해준다.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::MakeAccuInfo(CESL_DataMgr * pSpeciesDM, CESL_DataMgr * pBookDM, INT nBookIdx)
{
	//=======================================================================
	// 변수 정의
	//=======================================================================
	INT nBookCnt, nVolCnt;
	CString strMarc, strData, strCurrencyCode, strPrice, strPriceOthreInfo;
	CMarc marc;

	//=======================================================================
	// 1. 대표책의 인덱스가 설정되어 있지 않다면 책DM에서 대표책을 설정하고 대표책인덱스를 가져온다.
	//=======================================================================
	if( nBookIdx < 0 )
	{
		m_pFormApi->SetRepresentationBook(pBookDM);
		m_pFormApi->GetRepresentationBook(pBookDM, nBookIdx);
	}
	
	//=======================================================================
	// 2. 종DM에서 마크정보를 가져와 구조체마크를 구성한다.
	//=======================================================================
	strMarc = pSpeciesDM->GetCellData(_T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	

	//=======================================================================
	// 3. 수서정보DM에 로우를 추가한다.
	//=======================================================================
	m_pDM_Accu->InsertRow(-1);


	//=======================================================================
	// 4. 마크에서 가격정보를 가져와 수서정보DM에 가격과 화폐구분에 값을 설정한다.
	//=======================================================================
	strData.Empty();	
	m_pInfo->pMarcMgr->GetItem(&marc, _T("950$b"), strData);
	if( strData.IsEmpty() == FALSE )	
	{
		GetPriceInfo(strData, strCurrencyCode, strPrice);
	}
	m_pDM_Accu->SetCellData(_T("BP_가격"), strPrice, 0);
	m_pDM_Accu->SetCellData(_T("BP_화폐구분"), strCurrencyCode, 0);
	

	//=======================================================================
	// 5. 마크에서 가격잡정보를 가져와 수서정보DM에 가격잡정보를 설정한다.
	//=======================================================================
	strData.Empty();	
	m_pInfo->pMarcMgr->GetItem(&marc, _T("950$c"), strData);
	if( strData.IsEmpty() == FALSE ) 
	{
		m_pDM_Accu->SetCellData(_T("BP_가격잡정보"), strData, 0);
	}
	
	//=======================================================================
	// 6. 책DM에서 대표책의 그룹번호를 가져와 수서정보DM에 설정한다.
	//=======================================================================
	strData.Empty();	
	pBookDM->GetCellData(_T("BB_그룹번호"), nBookIdx, strData);
	if( strData.IsEmpty() == FALSE )
	{
		m_pDM_Accu->SetCellData(_T("BP_그룹번호"), strData, 0);
	}

	//=======================================================================
	// 7. 책DM에서 대표책의 수입년도를 가져와 수서정보DM에 설정한다.
	//=======================================================================
	strData.Empty();	
	pBookDM->GetCellData(_T("BB_수입년도"), nBookIdx, strData);
	if( strData.IsEmpty() == FALSE ) 
	{
		m_pDM_Accu->SetCellData(_T("BP_수입년도"), strData, 0);
	}

	//=======================================================================
	// 8. 책DM에서 대표책의 입력일자를 가져와 수서정보DM에 설정한다.
	//=======================================================================
	strData.Empty();	
	pBookDM->GetCellData(_T("BB_입력일자"), nBookIdx, strData);
	if( strData.IsEmpty() == FALSE ) 
	{
		m_pDM_Accu->SetCellData(_T("BP_입력일자"), strData, 0);
	}


	//=======================================================================
	// 9. 수서정보DM에 총책수를 설정한다.
	//=======================================================================
	strData.Empty();
	nBookCnt = pBookDM->GetRowCount();
	strData.Format(_T("%d"), nBookCnt);
	m_pDM_Accu->SetCellData(_T("BP_총책수"), strData, 0);


	//=======================================================================
	// 10. 수서정보DM에 총권수를 설정한다.
	//=======================================================================
	strData.Empty();
	nVolCnt = m_pFormApi->GetVolumeCount(pBookDM);
	strData.Format(_T("%d"), nVolCnt);
	m_pDM_Accu->SetCellData(_T("BP_총권수"), strData, 0);
	
	//=======================================================================
	// 11. 수서정보M에 주제구분_코드를 설정한다.
	//=======================================================================
	m_pDM_Accu->SetCellData(_T("BP_주제구분_코드"), pSpeciesDM->GetCellData(_T("BS_주제구분_코드"), 0), 0);

	//=======================================================================
	// 2005-02-19 
	// 12. 복본추기할 때 Species_key가 필요하므로 Species_Key를 Set해준다.
	//=======================================================================
	m_pDM_Accu->SetCellData(_T("BP_종KEY"), m_szSpeciesKey, 0);

	return 0;
}

INT CSpecies_Form::GetPriceInfo(CString strSrcData, CString &strCurrencyCode, CString &strPrice)
{
	TCHAR * strData = strSrcData.GetBuffer(0);
	INT cnt = strSrcData.GetLength();
	for(INT i=0 ; i<cnt ; i++)
	{
		if(strData[i] >= '0' && strData[i] <= '9')
			break;
	}
	
	strCurrencyCode = strSrcData.Mid(0, i);
	strPrice = strSrcData.Mid(i, cnt);
	return 0;
}

INT CSpecies_Form::MakeDupAddMode()
{
	if(m_strDupIndexKey.IsEmpty()) return -1;

	if(m_pDM_Species->GetCellData(_T("BS_복본여부"), 0) == _T("Y") || m_bDupAddYN || m_bDupAddMode)
	{
		ESLAfxMessageBox(_T("해당자료는 복본추기 할 수 없는 상태입니다.\n종에 포함된 책의 자료상태/복본추기 여부를 확인하십시요."));
		return 0;
	}

	m_bDupAddMode = TRUE;

	ApplyData();

	CString strQueryFlag;
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx));

		m_pDM_Book->SetCellData(_T("BB_종KEY"), m_strDupIndexKey, idx);
		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("N"), idx);
	}		

	// 종 DM, Index DM을 키로 가져와 구성한다. 기존의 업무구분 정보는 유지 되어야 한다.
	CString strOption, strWorkCode, strValue;
	strWorkCode = m_pDM_Index->GetCellData(_T("IBS_업무구분_코드"), 0);

	strOption.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE SPECIES_KEY=%s"), m_strDupIndexKey);
	m_pDM_Species->GetOneValueQuery(strOption, strValue);
	if(strValue == _T("0"))
	{
		ESLAfxMessageBox(_T("적용하신 자료는 복본추기된 자료입니다. 이 정보를 적용할 수 없습니다."));
		m_bDupAddMode = FALSE;
		return 0;
	}
	
	strOption.Format(_T("REC_KEY = %s"), m_strDupIndexKey);
	m_pDM_Species->RefreshDataManager(strOption);
	m_pDM_Index->RefreshDataManager(strOption);		

	if(m_pDM_Species->GetRowCount() == 0 || m_pDM_Index->GetRowCount() == 0)
	{
		ESLAfxMessageBox(_T("적용하신 자료가 올바르지 않습니다."));
		m_bDupAddMode = FALSE;
		EndDialog(IDCANCEL);
		return 0;
	}
	
	if(strWorkCode != m_pDM_Index->GetCellData(_T("IBS_업무구분_코드"), 0))
	{
		strWorkCode = m_pDM_Index->GetCellData(_T("IBS_업무구분_코드"), 0);
		m_pCM_Index->GetControlMgrData(_T("IBS_업무구분_코드"), strWorkCode);
		INT nWorkCode = _ttoi(strWorkCode.GetBuffer(0));
		::PostMessage(m_pHeadDlg->GetSafeHwnd(), CHANGE_WORK_CODE, nWorkCode, 0);		
	}
	

	//마크를 추출하여 색인과 구입정보의 마크관련 정보를 설정한다.
	CMarc marc;
	CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu);
	
	// 화면 보여주기
	Display();
	
	return 0;
}

INT CSpecies_Form::Display()
{
	// 종-복본추기
	CStringArray ArrIgnoreAlias;
	
	if ( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
	{
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			SetEBookCtrl(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			SetEBookCtrl(FALSE);
		}
		
		// 2008/04/22 REM BY PDJ
		// 수정할 수 있도록 풀어달라
		//m_pCM_Index->AllControlReadOnly();
		//20070816 ADD PJW : 신청일자 ReadOnly<FALSE,TRUE 세팅>
		//m_hApplicantTime->Control_SetReadOnly(TRUE);

		//등록구분 컨트롤  ReadOnly안되게
		((CEdit*)GetDlgItem(IDC_eREG_CLASS_CODE))->SetReadOnly(FALSE);

		
		//화면지우기 안보이게
		GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_HIDE);
		
		m_pCM_Accu->ControlDisplay(_T("BP_주제구분_코드"), 0);
	}
	else 
	{
		// 입력일 경우 화면지우기 보이게
		if( this->m_nOpType == SINSERT )	GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_SHOW);
		else								GetDlgItem(IDC_bCLEARSCREEN)->ShowWindow(SW_HIDE);

		m_pFormApi->AllControlClear(m_pCM_Index, &m_arrIgnoreCTRLAlias);
		m_pFormApi->AllControlClear(m_pCM_Accu, &m_arrIgnoreCTRLAlias);
		m_pTailDlg->AllControlClear();
	}
	
	// 종-입력
	if( m_nOpType == SINSERT )
	{
		ShowRegCodeField();
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Accu->SetControlMgrData(_T("BP_총권수"), strVolumeCount);
		
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			SetEBookCtrl(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			SetEBookCtrl(FALSE);
		}
		
		ApplyRepresentBookDataToIndex(m_pDM_Book, m_pDM_Index, m_pCM_Index);
	}
	// 수서-종-구입-수정
	else if( m_nOpType != SINSERT )
	{
		CString strSubjectCode;
		m_pDM_Accu->GetCellData(_T("BP_주제구분_코드"), 0, strSubjectCode);
		//====================================================
		//2008.07.08 ADD BY PJW : 주제구분이 적용안함일경우 블랭크를 넣어준다.
		if( strSubjectCode.IsEmpty() )
		{
			m_pCM_Accu->SetControlMgrData(_T("BP_주제구분_코드"), _T("  "));
		}
		else
		{
			m_pCM_Accu->SetControlMgrData(_T("BP_주제구분_코드"), strSubjectCode);
		}
		//====================================================
		
		ShowRegCodeField();
		// BEGIN:
		// 2004.10.15 5163_1. 우선정리자료인지 DM에서 가져와서 화면에 보여준다.
		if(m_strPriorityCatalogYN == _T("Y"))
		{
			// 2005-01-24 기증, 기증등록대상도 추가.
			if( m_nSpeciesType == BO_PURCHASE || m_nSpeciesType == BO_DONATE || m_nSpeciesType == BO_ORDER || m_nSpeciesType == BO_MISSING || (m_nAcqType == PURCHASE && m_nSpeciesType == BO_REGOBJ) || (m_nAcqType == DONATE && m_nSpeciesType == BO_REGOBJ) ||
				m_nSpeciesType == BO_REG_PURCHASE || m_nSpeciesType == BO_REG_DONATE || m_nSpeciesType == BO_REG_INVALID || m_nSpeciesType == BO_CAT)
			{
				CString strCheck = _T("");
				CButton *b1 = (CButton*)GetDlgItem(IDC_chkPRIORITY_CATALOG);
				m_pDM_Species->GetCellData(_T("BS_우선정리자료"), 0, strCheck);
				if(strCheck == 'Y') b1->SetCheck(1);
				else b1->SetCheck(0);
			}
		}
		// 2004.10.15 5163_3. 사용자 정의 코드 사용 여부
		if(m_strUseCodeYN == _T("Y"))
		{
			CString strCode;
			m_pDM_Species->GetCellData(_T("BS_도서분류코드1"), 0, strCode);
			m_pCM_Index->SetControlMgrData(_T("IBS_도서분류코드1"), strCode);
			strCode.Empty();
			m_pDM_Species->GetCellData(_T("BS_도서분류코드2"), 0, strCode);
			m_pCM_Index->SetControlMgrData(_T("IBS_도서분류코드2"), strCode);
			strCode.Empty();
			m_pDM_Species->GetCellData(_T("BS_도서분류코드3"), 0, strCode);
			m_pCM_Index->SetControlMgrData(_T("IBS_도서분류코드3"), strCode);
		}
		// END:
		// 2005-02-04 5163의 경우 다른 비고 EditBox를 보여줘야 한다.
		if(_T("Y") == m_strIs5163)
		{
			CString strRemark;
			m_pDM_Species->GetCellData(_T("BS_비고"), 0, strRemark);
			GetDlgItem(IDC_eDISCRIPTION_SMALL)->SetWindowText(strRemark);
		}

		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount, strPriceType;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Accu->SetControlMgrData(_T("BP_총권수"), strVolumeCount);
		
		if (nVolumeCount == 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
			SetEBookCtrl(TRUE);
		}
		else if (nVolumeCount > 1)
		{
			GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(FALSE);
			SetEBookCtrl(FALSE);
		}

		CString strTmpAuthor = m_pDM_Index->GetCellData(_T("IBS_저작자"), 0);
		
		ApplyRepresentBookDataToIndex(m_pDM_Book, m_pDM_Index, m_pCM_Index);
		strTmpAuthor = m_pDM_Index->GetCellData(_T("IBS_저작자"), 0);
		// 구입정보 설정 - 신청구분/신청코드
		// 2005-09-28 강봉수
		//
		SetPurchaseInfo();
		strTmpAuthor = m_pDM_Index->GetCellData(_T("IBS_저작자"), 0);
	}
	else		return -1;

	m_pHeadDlg->Display(0);
	m_pTailDlg->Display(0);
	
	ShowControlByAcqType();
	//==============================================================
	//20080613 REM BY PJW : 구입신청자가 없을경우 날짜 시간 모두 NULL로 셋팅함으로 필요 없어진 코드
	//20070813 ADD PJW : 구입신청자가 없을때 NULL 값으로 시분초 세팅
	//CString strApplicantName, sTime, strApplicantData;
	//m_pCM_Index->GetControlMgrData(_T("구입신청자"), strApplicantName, 0);
	//if ( strApplicantName == _T(""))
	//{
	//	sTime.Format(_T("  :  :  "));
	//	strApplicantTime(sTime);
	//	
	//}
	//==============================================================

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - DM을 기본값으로 설정한다.
// [ 미니스펙 ]
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::LoadDefaultValue()
{
	//=======================================================================
	// 변수 정의
	//=======================================================================
	INT i, ids;
	CString strFormCode, strData, strPath, strMarc, strWorkCode;
	CMarc marc;


	//=======================================================================
	// 1. 등록구분 에디트 박스에 기본설정값으로 세팅한다.
	//=======================================================================	
	InitRegCodeField();		// 등록구분에서 이전 내용을 가지고 있어야 함.
	
	// 2005-02-21 
	// 등록구분값을 먼저 준 다음 ApplyData() 수행하여야 함.
	ApplyData();

	// 등록구분, 책부록플래그를 책에도 반영해야 합니다.
	for( i=0 ; i<m_pDM_Book->GetRowCount() ; i++ )
	{
		m_pDM_Book->SetCellData(_T("BB_등록구분_코드"), m_strRegCodeField, i);
		m_pDM_Book->SetCellData(_T("BB_책부록플래그"), _T("B"), i);
	}
	
	m_pCM_Index->GetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
	if( strFormCode.IsEmpty() == TRUE )		m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);

	strData = strFormCode + _T(".cfg");
	strPath = m_strDefaultValueFilePath;
	strPath.Replace(_T(".cfg"), strData);

	
	ids = m_pFormApi->LoadDefaultValue(strMarc, strPath);
	if( ids < 0 )
	{
		CString strMsg;
		strMsg.Format(_T("형식구분 : %s의 기본값이 설정되어 있지 않습니다."), strFormCode);
		ESLAfxMessageBox(strMsg);
//		OnbCLEARSCREEN();
		return 0;
	}
	
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);

	m_pFormApi->AllControlDisplay(m_pCM_Index, &m_arrIgnoreCTRLAlias, 0);
	m_pFormApi->AllControlDisplay(m_pCM_Accu, &m_arrIgnoreCTRLAlias, 0);

	m_pTailDlg->Display(0);
	ShowRegCodeField();
	
	
	m_pCM_Index->GetControlMgrData(_T("IBS_업무구분_코드"), strWorkCode);
	m_pDM_Accu->SetCellData(_T("BP_차수업무구분"), strWorkCode, 0);
	//차수업무구분 설정
	m_pHeadDlg->Display(0);
	//포커스를 서명에
	GetDlgItem(IDC_eTITLE)->SetFocus();
	return 0;
}

INT CSpecies_Form::SaveDefaultValue()
{
	CString strFormCode, strData, strPath;
	m_pCM_Index->GetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
	if(strFormCode.IsEmpty())
		m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);
	strData = strFormCode + _T(".cfg");
	strPath = m_strDefaultValueFilePath;
	strPath.Replace(_T(".cfg"), strData);
	m_pFormApi->SaveDefaultValue(m_pCM_Index, strPath);
	m_pFormApi->SaveDefaultValue(m_pDM_Accu, strPath);
	return 0;
}

INT CSpecies_Form::ShowRegCodeField()
{
	CArray<CRegCode, CRegCode> arrRegCode;
	m_pFormApi->GetRegCodeInfo(m_pDM_Book, &arrRegCode);
	CRegCode regCode;
	CString strTemp;
	CString strRegCodeField;
	INT nBookCount = 0;
	INT nArrCount = arrRegCode.GetSize();
	if (nArrCount != 0)
	{
		for (INT i = 0; i < nArrCount - 1; i++)
		{
			regCode = arrRegCode.GetAt(i);
			nBookCount += regCode.m_nCount;
			strTemp.Format(_T("%s%d/"), regCode.m_strRegCode, regCode.m_nCount);
			strRegCodeField += strTemp;
		}
		
		regCode = arrRegCode.GetAt(nArrCount - 1);	
		nBookCount += regCode.m_nCount;
		strTemp.Format(_T("%s%d"), regCode.m_strRegCode, regCode.m_nCount);
		strRegCodeField += strTemp;

		// 등록구분 필드
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(strRegCodeField);
		m_strRegCodeField = strRegCodeField;

		// 총 책수
		CString strBookCount;
		strBookCount.Format(_T("%d"), nBookCount);
		m_pCM_Accu->SetControlMgrData(_T("BP_총책수"), strBookCount);
		INT nVolumeCount = m_pFormApi->GetVolumeCount(m_pDM_Book);
		CString strVolumeCount;
		strVolumeCount.Format(_T("%d"), nVolumeCount);
		m_pCM_Accu->SetControlMgrData(_T("BP_총권수"), strVolumeCount);
	}

	return 0;
}

UINT CSpecies_Form::OnPurchaseSeqNoChange(LPARAM lParam, WPARAM wParam)
{
	CString strTmpData;

	if( lParam == 0 )	strTmpData = _T("");
	else strTmpData.Format(_T("%d"), lParam);

	m_pCM_Index->SetControlMgrData(_T("IBS_업무구분_코드"), strTmpData);
	
	return 0;
}

UINT CSpecies_Form::OnWorkingStatusChange(LPARAM lParam, WPARAM wParam)
{
	SPECIES_TYPE nSpeciesType = (SPECIES_TYPE)lParam;
	CEdit* pCtrl = NULL;

	pCtrl = (CEdit*)GetDlgItem(IDC_edtWORKING_STATUS);
	if(pCtrl == NULL) return -1;
	if( nSpeciesType == BO_DONATE )
	{
		m_strWorkingStatus = _T("BOA211O");
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("기증접수자료"));
		pCtrl->SetWindowText(_T("기증접수자료"));
	}
	else if( nSpeciesType == BO_NONDONATE )
	{
		m_strWorkingStatus = _T("BOA212O");
		//m_pCM_Index->SetControlMgrData(_T("자료구분"), _T("기증미등록자료"));
		pCtrl->SetWindowText(_T("기증미등록자료"));
	}

	return 0;
}

UINT CSpecies_Form::OnAcqTypeChange(LPARAM lParam, WPARAM wParam)
{
	CString strAcqCode, strWorkCode;
	strAcqCode.Format(_T("%d"), lParam);

	ACQ_TYPE nAcqType = m_pHeadDlg->GetAcqType();
	if(m_nAcqType == OTHER_ACQ_TYPE && nAcqType == OTHER_ACQ_TYPE) m_bAcqTypeChange = TRUE;
	else if(m_nAcqType == nAcqType ) return 0;
	else if(m_nAcqType == ACQ_TYPE_INVALID && nAcqType != ACQ_TYPE_INVALID) m_bAcqTypeChange = TRUE;
	else if(m_nAcqType != ACQ_TYPE_INVALID && nAcqType == ACQ_TYPE_INVALID) m_bAcqTypeChange = TRUE;
	else m_bAcqTypeChange = FALSE;

	m_nAcqType = nAcqType;
	if(m_nAcqType == PURCHASE) m_nSpeciesType = BO_REG_PURCHASE;
	else if(m_nAcqType == DONATE) m_nSpeciesType = BO_REG_DONATE;
	else if(m_nAcqType == ACQ_TYPE_INVALID || m_nAcqType == OTHER_ACQ_TYPE) m_nSpeciesType = BO_REG_INVALID;
	SetAccuDataManager();
	SetWorkingStatus();
	if(m_bCreateGroupCollection)
		SetParentGroupInfo((INT)m_nAcqType, m_pBOSpeciesApi->GetTodayYear(), _T(""), m_pDM_Index->CONNECTION_INFO);
	else 
	{
		m_pGroupCollection->RemoveAll();
		m_pGroupCollection->SetAcqType(m_nAcqType);
		m_pGroupCollection->SetWorkingStatus(_T("검수완료"));
	}
	m_pGroupCollection->SetAcqCode(strAcqCode);
	m_pGroupCollection->GetAcqType();
	SetHeadTailInfoDlg();
	ShowControlBySpeciesType();
	ShowControlByAcqType();
	OnSelchangecCLASIFYCLASS();
	m_pTailDlg->Display(0);
	
	return 0;
}

VOID CSpecies_Form::OnbtnBSFORMSHOWHIDECODE() 
{
	if( m_bShowCode == TRUE )	m_bShowCode = FALSE;
	else						m_bShowCode = TRUE;

	ShowCodeResource();	
}


VOID CSpecies_Form::OnKillfocuseTITLE() 
{
	// TODO: Add your control notification handler code here
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_eTITLE);
	if (pEdit == NULL) return;

	// 리드 온리이면 체크하지 않는다.
	if (pEdit->GetStyle() & ES_READONLY) return;

	SetTimer(0, 0, NULL);		
}

VOID CSpecies_Form::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	// 서명 자동복본조사
	if (nIDEvent == 0)
	{
		KillTimer(0);

		if (m_bAutoDupSearch == FALSE) return;
		if(m_pNoteCtrl)
		{
			if(m_pNoteCtrl->GetSafeHwnd())
				return;
		}

		/*************************************************
		 *************************************************/
		//20080508 ADD BY PJW : K2UP에서 다른 프로그램으로 포커스가 옮겨갈경우 return한다.
		CWnd * pWnd;
		pWnd = this->GetFocus();
		if( pWnd == NULL )
		{
			return;
		}
		/*************************************************
		 *************************************************/

		CString strSearchCount;

		SearchDuplicateDataByTitle(_T("서명자동복본조사"), strSearchCount);

		if (strSearchCount.IsEmpty() || strSearchCount == _T("0"))	return;
				
		m_pNoteCtrl = new CNoteCtrl;
		CString strNote;
		strNote.Format(_T("%s건의 복본이 발견되었습니다."), strSearchCount);
		m_pNoteCtrl->SetNote(strNote);

		CRect rcButton;
		GetDlgItem(IDC_bDUP_SEARCH)->GetWindowRect(rcButton);
		ScreenToClient(rcButton);
				
		rcButton.top += 50;
		rcButton.bottom += 60;

		rcButton.right += 60;

		CRect rcWindow;
		GetWindowRect(rcWindow);
		
		CPoint point;
		GetCursorPos(&point);
		
		if(point.y > rcWindow.bottom-25)
			point.y = rcWindow.bottom-25;
		else if(point.y < rcWindow.top+25)
			point.y = rcWindow.top+25;
		if(point.x > rcWindow.right-200)
			point.x = rcWindow.right-200;
		else if(point.x < rcWindow.left+200)
			point.x = rcWindow.left+200;

		ScreenToClient(&point);
		point.y += 5;
		
		m_pNoteCtrl->Create(point, 200, 25, this);
		m_pNoteCtrl->ShowWindow(SW_SHOW);
	}//20070522 ADD PJW : 서명에 포커스 
	else if (nIDEvent == 3)
	{
		KillTimer(3);
		GetDlgItem(IDC_eTITLE)->SetFocus();
	}
		
	CDialog::OnTimer(nIDEvent);
}

VOID CSpecies_Form::SearchDuplicateDataByTitle(CString strSearchType, CString &strSearchCount)
{
	CString strTitle, strTmpData;

	m_pCM_Index->GetControlMgrData(_T("IBS_본표제"), strTitle);
	
	if (strTitle.IsEmpty()) return ;
	INT nFind = strTitle.Find(_T("="));
	if (nFind >= 0)
	{
		strTitle = strTitle.Mid(0, nFind);
		nFind = strTitle.Find(_T(":"));
		if(nFind >= 0)
			strTitle = strTitle.Mid(0, nFind);
	}

	if (strTitle.IsEmpty()) return ;

	strTmpData = strTitle;
	((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
	if(strTmpData.IsEmpty()) return;
	
	// 수정모드일때 자신의 종키를 가져온다.
	CString strSpeciesKey;
	if (m_nOpType != SINSERT && m_nParentSelectedRow >= 0)
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);

	CDuplicateSearch_Interface impl(this);
	INT ids = impl.ShowOnlyTitleDuplicateSearch(strSearchType, strTitle, strSearchCount, strSpeciesKey);
	if (ids < 0) return;
}

VOID CSpecies_Form::OnbMARCEDIT() 
{
	INT ids = -1;
	CString strMarc, strLeader;
	CMarc marc;

	ids = m_pDM_Species->GetCellData(_T("BS_MARC"), 0, strMarc);
	if( strMarc.IsEmpty() == FALSE )
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if( ids < 0 )	return;
	}

	// 리더 정보 세팅 (긴급추가임)
	marc.GetLeader(strLeader);
	// 레코드 상태 없으면 넣는다. 5번째
	if( strLeader.GetAt(5) == ' ' )		strLeader.SetAt(5, 'n');
	// 레코드 형태 없으면 넣는다. 6번째
	if( strLeader.GetAt(6) == ' ' )		strLeader.SetAt(6, 'a');
	// 서지수준 없으면 넣는다. 7반째
	if( strLeader.GetAt(7) == ' ' )		strLeader.SetAt(7, 'm');
	// 설정된 리더를 저장한다.
	marc.SetLeader(strLeader.GetBuffer(0));


	// 복본추기 모드일경우는 마크보기만 된다.
	if( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
	{
		CBOMarcEditDlg dlg(this, &marc);
		dlg.SetReadOnly();
		dlg.DoModal();
	}
	else
	{
		ids = ApplyData();
		if( ids < 0 )	return;

		//종에서 마크를 생성시에 소장종이 아닌경우 100Tag를 재생성해야 한다.
		if( m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0).Left(3) != _T("BOC") && 
			m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0).Left(3) != _T("BOL") )
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("100"));
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("700"));
		}
		//2006.12.08 확장개선 : 008반입 그대로 로 세팅
		CString s008;
		m_pInfo->pMarcMgr->GetItem(&marc,_T("008*"),s008);
		// 종 -> 마크
		ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
		m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
		m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
		m_pInfo->pMarcMgr->DeleteEmptySubfield(&marc);
		m_pInfo->pMarcMgr->DeleteEmptyField(&marc);
		
		m_pInfo->pMarcMgr->SetItem(&marc,_T("008*"),s008);

		
		// 마트 편집 다이얼로그 띄우기
		CBOMarcEditDlg dlg(this, &marc);
		dlg.SetReadOnly(IsHoldDataMarc(m_pDM_Book));
		if(IDOK != dlg.DoModal()) return;
		
		// 마크 -> 종
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
		
		// 종 Marc로
		m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
		m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
		
		Display();
	}	
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//
// [ 미니스펙 ]
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::ApplyData()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids, nIdx, nRowCount, i;
	CString strCode, strVol, strQueryFlag, strApplicantCode, strRemark, strMsg;
	CButton* b1 = (CButton*)GetDlgItem(IDC_chkPRIORITY_CATALOG);


	//=======================================================================
	// 1. 오픈타입이 SINSERT, SUPDATE, GUPDATE라면 다음항목들을 체크하여 해당 DM에 셋팅한다.
	//		- i.	우선정리자료	:	체크박스 -> DMUP_단행_종_종정보
	//		- ii.	사용자정의코드	:	DMUP_단행_종_색인정보 -> DMUP_단행_종_종정보 (종DM)
	//		- iii.	색인CM -> 색인DM	CMUP_단행_종_색인정보 -> DMUP_단행_종_색인정보 (색인DM)
	//		- iv.	색인DM(IBS_제어자료구분_코드) -> 종DM(BS_제어자료구분)
	//		- v.	5163사용여부	:	종DM - BS_비고
	//		- vi.	수서정보CM -> 수서정보DM
	//=======================================================================
	if( m_nOpType == SINSERT || m_nOpType == SUPDATE || m_nOpType == GUPDATE )
	{
		// i.
		// 2004.10.14 5163_1. 우선정리자료 체크값받아서 DM에 저장하기.
		// BEGIN:
		if( m_strPriorityCatalogYN == _T("Y") )
		{
			nIdx = 0;
			nIdx = b1->GetCheck();
			if( nIdx == 1 )		m_pDM_Species->SetCellData(_T("BS_우선정리자료"), 'Y', 0);
			else m_pDM_Species->SetCellData(_T("BS_우선정리자료"), 'N', 0);
		}

		// ii.
		// 사용자정의코드 사용여부를 체크하여 색인DM->종DM으로 설정한다.
		if( m_strUseCodeYN == _T("Y") )
		{
			m_pCM_Index->GetControlMgrData(_T("IBS_도서분류코드1"), strCode);
			m_pDM_Species->SetCellData(_T("BS_도서분류코드1"), strCode, 0);
			
			strCode.Empty();
			m_pCM_Index->GetControlMgrData(_T("IBS_도서분류코드2"), strCode);
			m_pDM_Species->SetCellData(_T("BS_도서분류코드2"), strCode, 0);
			
			strCode.Empty();
			m_pCM_Index->GetControlMgrData(_T("IBS_도서분류코드3"), strCode);
			m_pDM_Species->SetCellData(_T("BS_도서분류코드3"), strCode, 0);
		}
		// END:

		// iii.
		// 색인 CM -> 색인 DM				
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Index, 0);
		if( ids < 0 ) 
		{
			strMsg.Empty();
			strMsg.Format( _T("색인 정보를 구축할 수 없습니다.\n[%s -> %s]"), m_pCM_Index->GetAlias(), m_pDM_Index->GETDMAlias() );
			ESLAfxMessageBox( strMsg ) ;
			//return -1;
		}		
		//20070810 ADD PJW : 구입신청일자 항목의 시간을 갱신
		//20070827 ADD PJW : 구입신청자만 입력했을 경우 예외처리 추가 구입신청일자 

		CString sDate, sTime, strApplicantDate;
		m_pCM_Index->GetControlMgrData(_T("구입신청일"), sDate, 0);
		m_hApplicantTime->GetData(sTime);
		if( sDate == _T("") )
		{
			m_pDM_Index->SetCellData(_T("구입신청일"), _T(""), 0);
		}
		else
		{
			strApplicantDate.Format(_T("%s %s"),sDate, sTime );
			m_pDM_Index->SetCellData(_T("구입신청일"), strApplicantDate, 0);
		}
		//strApplicantDate.Format(_T("%s %s"),sDate, sTime );
		//m_pDM_Index->SetCellData(_T("구입신청일"), strApplicantDate, 0);
		//GetDlgItem(IDC_edtBSFORM_REQUESTOR)->SetWindowText(_T(""));
		// iv.
		// 제어자료구분(비고)은 -> 색인 -> 종으로 반영되어야 한다.
		m_pDM_Species->SetCellData(_T("BS_제어자료구분"), m_pDM_Index->GetCellData(_T("IBS_제어자료구분_코드"), 0), 0);
		//m_pDM_Species->SetCellData(_T("BS_비고"), m_pDM_Index->GetCellData(_T("IBS_비고"), 0), 0);

		// v.
		// 2005-02-04 5163의 경우에는 작은 비고란에서 값을 가져와야 한다.
		if( _T("Y") == m_strIs5163 )
		{
			GetDlgItem(IDC_eDISCRIPTION_SMALL)->GetWindowText(strRemark);
			m_pDM_Index->SetCellData(_T("IBS_비고"), strRemark, 0);
			m_pDM_Species->SetCellData(_T("BS_비고"), strRemark, 0);
		}
		else	m_pDM_Species->SetCellData(_T("BS_비고"), m_pDM_Index->GetCellData(_T("IBS_비고"), 0), 0);

		// vi.
		if( m_pDM_Accu != NULL )
		{
		// 수서정보 CM -> 수서정보 DM
			ids = m_pFormApi->MakeDMFromCM(m_pCM_Accu, m_pDM_Accu, 0);
			if( ids < 0 )
		{
				strMsg.Empty();
				strMsg.Format(_T("수서 정보를 구축할 수 없습니다.\n[%s -> %s]"), m_pCM_Accu->GetAlias(), m_pDM_Accu->GETDMAlias());
				ESLAfxMessageBox( strMsg );
				return -1;
			}
			
			//비고는 구입/기증정보에도 들어가야 한다.
			m_pDM_Accu->SetCellData(_T("BP_비고"), m_pDM_Index->GetCellData(_T("IBS_비고"), 0), 0);

			//=====================================================
			//2008.11.14 ADD BY PJW : 수입정보테이블에 관리구분 추가한다.
			CString strManageCode;
			strManageCode = m_pInfo->MANAGE_CODE;
			m_pDM_Accu->SetCellData(_T("BP_관리구분"), strManageCode, 0);
			//=====================================================


			//수서정보.. 육사 추가
			//육사 관련 작업
			// 현재의 신청구분을 DM에 적용한다.
			// 2005-09-29 강봉수
			nIdx = m_cmbApplicant.GetCurSel();
			switch( nIdx )
			{
			case 1:	// 1 - 부서
				{
					m_pCM_Index->GetControlMgrData(_T("구입신청구분_부서"), strApplicantCode);
					// 단행-색인정보 저장
					m_pDM_Index->SetCellData(_T("구입신청구분_부서"), strApplicantCode, 0);	
					m_pDM_Index->SetCellData(_T("구입신청구분_학부"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("구입신청구분_학과"), _T(""), 0);	
					// 단행-구입정보 저장
					m_pDM_Accu->SetCellData(_T("BP_신청구분"),_T("CLA"),0);
					m_pDM_Accu->SetCellData(_T("BP_부서코드"), strApplicantCode, 0);
					m_pDM_Accu->SetCellData(_T("BP_학부코드"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_학과코드"),_T(""),0);
				}
				break;
			case 2:	// 2 - 학과
				{
					m_pCM_Index->GetControlMgrData(_T("구입신청구분_학과"), strApplicantCode);
					// 단행-색인정보 저장					
					m_pDM_Index->SetCellData(_T("구입신청구분_부서"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("구입신청구분_학부"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("구입신청구분_학과"), strApplicantCode, 0);	
					// 단행-구입정보 저장
					m_pDM_Accu->SetCellData(_T("BP_신청구분"),_T("COU"),0);
					m_pDM_Accu->SetCellData(_T("BP_부서코드"), _T(""), 0);
					m_pDM_Accu->SetCellData(_T("BP_학부코드"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_학과코드"),strApplicantCode,0);
				}
				break;
			case 3:	// 3 - 학처
				{
					m_pCM_Index->GetControlMgrData(_T("구입신청구분_학부"), strApplicantCode);
					// 단행-색인정보 저장					
					m_pDM_Index->SetCellData(_T("구입신청구분_부서"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("구입신청구분_학부"), strApplicantCode, 0);	
					m_pDM_Index->SetCellData(_T("구입신청구분_학과"), _T(""), 0);	
					// 단행-구입정보 저장
					m_pDM_Accu->SetCellData(_T("BP_신청구분"),_T("DEP"),0);
					m_pDM_Accu->SetCellData(_T("BP_부서코드"), _T(""), 0);
					m_pDM_Accu->SetCellData(_T("BP_학부코드"),strApplicantCode,0);
					m_pDM_Accu->SetCellData(_T("BP_학과코드"),_T(""),0);
				}
				break;

			case 0:	// 0 - 회원
			default:
				{
					// 단행-색인정보 저장
					m_pDM_Index->SetCellData(_T("구입신청구분_부서"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("구입신청구분_학부"), _T(""), 0);	
					m_pDM_Index->SetCellData(_T("구입신청구분_학과"), _T(""), 0);	
					// 단행-구입정보 저장
					m_pDM_Accu->SetCellData(_T("BP_신청구분"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_부서코드"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_학부코드"),_T(""),0);
					m_pDM_Accu->SetCellData(_T("BP_학과코드"),_T(""),0);
				}
				break;
			}
			//--이상.--
		}

		// 편권차/권일련번호
		m_pCM_Index->GetControlMgrData(_T("IBS_편권차"), strVol);
	}


	//=======================================================================
	// 2. 오픈타입에 따라 다음 작업을 수행한다.
	// [ 입력(SINSERT) ]
	//		- i.	쿼리플래그와 편권차를 이용하여 표준책정보를 만든다.
	//		- ii.	복본추기모드가 아니면 대표책정보를 생성한다.
	//		- iii.	단권일때 종화면 값을 적용시켜준다.
	//=======================================================================
	switch( m_nOpType )
	{
	// 종-입력
	case SINSERT:
	{
		// 단권일때 입력된 편권차가 전의 편권차와 다르면 모든 책을 삭제한다.
		// 2005-03-18 주석처리.
		/*
		if (m_pFormApi->GetVolumeCount(m_pDM_Book) == 1)
		{
			if (m_pDM_Book->GetCellData(_T("BB_편권차"), 0) != strVol)
			{
				m_pDM_Book->FreeData();
				m_pDM_Book->InsertRow(-1);
			}
		}
		*/

			// i.
			// 쿼리 플래그 설정
			strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);

		// 표준책 정보 만들기
			ids = MakeStandardBookFromIdx(strVol, strQueryFlag);
			if( ids < 0 )
			{
				ESLAfxMessageBox( _T("표준책정보를 생성할 수 없습니다.") );
				return -1;
			}

			// ii.
			// 복본추기 모드가 아니면 대표책 하나를 만든다.
			if( m_bDupAddMode == FALSE )	
			{
				ids = m_pFormApi->SetRepresentationBook(m_pDM_Book);
				if( ids < 0 )
				{
					strMsg.Empty();
					strMsg.Format( _T("대표책정보를 생성할 수 없습니다.\n[%s]"), m_pDM_Book->GETDMAlias());
					ESLAfxMessageBox( strMsg );
					return -1;
				}
			}

			// iii.
			/// 단권일때 종화면 값을 적용시켜 주고 다권일 경우 대표책을 이용하여 책DM을 구성한다.
			if( m_pFormApi->GetVolumeCount(m_pDM_Book) == 1 )	ApplyOneVolumeData(strVol, strQueryFlag);
			else												ApplyRepresentVolumeData(_T(""), strQueryFlag);

		// Working Status 적용
		ApplyWorkingStatus();
		ApplyAppendixWorkingStatus();
		
		//대표책의 코드값을 종의 코드값으로 적용시켜준다.
		ApplyRepresentBookDataToSpecies(m_pDM_Book, m_pDM_Species);

		m_pHeadDlg->ApplyData();
		m_pTailDlg->ApplyData();

		ApplyAppendixDM();
	}
		break;

	// 종-수정
	case SUPDATE:
	case GUPDATE:
	{
		// 책 DM RowCount
			nRowCount = m_pDM_Book->GetRowCount();
		// 단권일때 편권차 일괄 적용
			if( m_pFormApi->GetVolumeCount(m_pDM_Book) == 1 )
		{						
				for( i=0; i < nRowCount; i++ )	ids = m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, i);
		}

		/// 쿼리 플래그
			strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE);

		// 복본추기 모드가 아니면 대표책 정보 만들기
			if( m_bDupAddMode == TRUE )//FALSE여야하지않을까?
		{
			// 대표책을 찾는다.
				nIdx = -1;
			m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
			
			// 대표책 정보 만들기
			MakeStandardBookFromIdx(strVol, strQueryFlag, nIdx);
		}
				
			// 단권일때 종화면 값을 적용시켜준다.
			if( m_pFormApi->GetVolumeCount(m_pDM_Book) == 1 )	ApplyOneVolumeData(strVol, strQueryFlag);
			else												ApplyRepresentVolumeData(_T(""), strQueryFlag);

		// Working Status 적용
		ApplyWorkingStatus();
		ApplyAppendixWorkingStatus();
		
		//대표책의 코드값을 종의 코드값으로 적용시켜준다.
		ApplyRepresentBookDataToSpecies(m_pDM_Book, m_pDM_Species);
		
		m_pHeadDlg->ApplyData();
		m_pTailDlg->ApplyData();
		
		ApplyAppendixDM();
	}
	default:		//return -1;
		break;
	}
	
	// 복본추기 모드이면 종키를 세팅해준다.
	if( m_bDupAddMode == TRUE )
	{
		nRowCount = m_pDM_Book->GetRowCount();
		for( i = 0; i < nRowCount; i++ )
		{
			//strQueryFlag = m_pFormApi->GetQueryFlag(MODE_UPDATE, m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), i));
			m_pDM_Book->SetCellData(_T("BB_종KEY"), m_strDupIndexKey, i);
			// m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, i);
			m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("N"), i);
		}		
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인자로 온 idx번째 책DM에 편권차, 수입구분코드, DB_FLAG(쿼리플래그)를 입력한다.
//
// [ 미니스펙 ]
// 1. idx가 -1이면 첫번째 책을 찾아서 표준정보를 만들어준다.
// 2. 인자로 온 strVol값을 idx번째 편권차에 입력한다.
// 3. idx번째 수입구분코드에 입수타입값을 입력한다.
// 4. idx번째 DB_FLAG에 인자로온 strQueryFlag의 값을 입력한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::MakeStandardBookFromIdx(CString strVol, CString strQueryFlag, INT idx /*= -1*/)
{
	INT ids = -1;
	
	// idx 가 -1이면 첫번째 책을 찾아서 표준정보를 만들어준다.
	if (idx == -1)
	{
		INT nRowCount = m_pDM_Book->GetRowCount();
		for (INT i = 0; i < nRowCount; i++)
			if (m_pFormApi->IsBook(m_pDM_Book, i)) break;
		
		idx = i;
	}

	/// 색인정보로 부터 표준 정보 만들기
	if(!m_bViewVolInfo && !m_bDupAddMode)
		ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"));
	/// 편권차 적용
	if(m_pBOSpeciesApi->GetVolumeCount(m_pDM_Book) == 1)
		ids = m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, idx);
	//수입구분적용
	CString strTmpData;
	if(m_nAcqType == ACQ_TYPE_INVALID) strTmpData.Empty();
	else strTmpData.Format(_T("%d"), m_nAcqType);
	m_pDM_Book->SetCellData(_T("BB_수입구분_코드"), strTmpData, idx);

	/// 쿼리 플래그 적용
	m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);

	return 0;
}

// 2004.09.15 
/*
INT CSpecies_Form::ApplyRepresentVolumeData(CString strVolSortNo, CString strQueryFlag)
{
	// 색인정보로 부터 책 정보 적용시키기
	INT nBookCount = m_pDM_Book->GetRowCount();
	INT nReprentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nReprentBookIdx);
	if(nReprentBookIdx < 0)
	{
		nReprentBookIdx = 0;
		m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("Y"), nReprentBookIdx);
	}
	strVolSortNo = m_pDM_Book->GetCellData(_T("BB_권일련번호"), nReprentBookIdx);
	if(strVolSortNo.IsEmpty()) strVolSortNo = _T("10");
	CString strTmpData;
	INT ids ;
	CStringArray ArrAccept;
	ArrAccept.Add(_T("IBS_관리구분_코드"));		ArrAccept.Add(_T("IBS_매체구분_코드"));
	ArrAccept.Add(_T("IBS_별치기호_코드"));		ArrAccept.Add(_T("IBS_이용제한구분_코드"));
	ArrAccept.Add(_T("IBS_이용대상구분_코드"));	ArrAccept.Add(_T("IBS_면장수"));
	ArrAccept.Add(_T("IBS_물리적특성"));		ArrAccept.Add(_T("IBS_딸림자료"));
	ArrAccept.Add(_T("IBS_크기"));				ArrAccept.Add(_T("IBS_발행년"));

	CString strRepBookYN, strDBFlag;

	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		//같은 권인지 확인한다.
		strTmpData = m_pDM_Book->GetCellData(_T("BB_권일련번호"), idx);
		if(strTmpData != strVolSortNo) continue;

		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRepBookYN = m_pDM_Book->GetCellData(_T("BB_대표책여부"), idx);
		// 색인 CM -> 책 DM - 코드 정보는 반영되어야 한다.
		if(strDBFlag == _T("N") || strRepBookYN == _T("Y"))
			m_pFormApi->MakeDMFromCMInArray(m_pCM_Index, m_pDM_Book, &ArrAccept, idx, _T("BB"));

		// 쿼리 플래그 적용
		if(strDBFlag.IsEmpty())
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		if(strDBFlag == _T("N"))
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
	}

	return 0;
}
*/

INT CSpecies_Form::ApplyRepresentVolumeData(CString strVolSortNo, CString strQueryFlag)
{
	// 색인정보로 부터 책 정보 적용시키기
	INT nBookCount = m_pDM_Book->GetRowCount();
	INT nReprentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nReprentBookIdx);
	if(nReprentBookIdx < 0)
	{
		nReprentBookIdx = 0;
		m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("Y"), nReprentBookIdx);
	}
	strVolSortNo = m_pDM_Book->GetCellData(_T("BB_권일련번호"), nReprentBookIdx);
	if(strVolSortNo.IsEmpty()) strVolSortNo = _T("10");
	CString strTmpData;
	INT ids ;
	CStringArray ArrAccept;
	ArrAccept.Add(_T("IBS_관리구분_코드"));		ArrAccept.Add(_T("IBS_매체구분_코드"));
	ArrAccept.Add(_T("IBS_별치기호_코드"));		ArrAccept.Add(_T("IBS_이용제한구분_코드"));
	ArrAccept.Add(_T("IBS_이용대상구분_코드"));	ArrAccept.Add(_T("IBS_면장수"));
	ArrAccept.Add(_T("IBS_물리적특성"));		ArrAccept.Add(_T("IBS_딸림자료"));
	ArrAccept.Add(_T("IBS_크기"));				ArrAccept.Add(_T("IBS_발행년"));

	CString strRepBookYN, strDBFlag;

	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRepBookYN = m_pDM_Book->GetCellData(_T("BB_대표책여부"), idx);

		//같은 권인지 확인한다.
		strTmpData = m_pDM_Book->GetCellData(_T("BB_권일련번호"), idx);
		if(strTmpData != strVolSortNo) continue;

		// 색인 CM -> 책 DM - 코드 정보는 반영되어야 한다.
		if(strDBFlag == _T("N") || strRepBookYN == _T("Y"))
			m_pFormApi->MakeDMFromCMInArray(m_pCM_Index, m_pDM_Book, &ArrAccept, idx, _T("BB"));
	}

	for (idx = 0; idx < nBookCount; idx ++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		// 쿼리 플래그 적용
		if(strDBFlag.IsEmpty())
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		if(strDBFlag == _T("N"))
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
	}

	return 0;
}

INT CSpecies_Form::ApplyOneVolumeData(CString strVol, CString strQueryFlag)
{
	// 단권일때 종화면 값을 적용시켜준다.

	// 등록구분 적용시키기
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);

	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0 || arrRegCode.GetSize() <= 0)
	{
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		m_strLastErrMsg = _T("등록구분 필드가 잘못되었습니다.");
		return -1;
	}

	m_strRegCodeField = strRegCodeField;
	m_pFormApi->MakeBookFromRegCode(&arrRegCode, m_pDM_Book, strVol);

	// 색인정보로 부터 책 정보 적용시키기
	INT nBookCount = m_pDM_Book->GetRowCount();
	INT nReprentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nReprentBookIdx);
	if(nReprentBookIdx < 0)
	{
		nReprentBookIdx = 0;
		m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("Y"), nReprentBookIdx);
	}
/*
	//권일련번호를 적용한다. 복본기호부여기준에 따라서 권일련번호를 부여한다.
	m_pFormApi->SetVolSortNo(&m_cCopyCodeMgr, m_pDM_Book);
*/	
	CStringArray arrIgnore; arrIgnore.RemoveAll();
	arrIgnore.Add(_T("IBS_관리구분_코드"));		arrIgnore.Add(_T("IBS_매체구분_코드"));
	arrIgnore.Add(_T("IBS_별치기호_코드"));		arrIgnore.Add(_T("IBS_이용제한구분_코드"));	
	arrIgnore.Add(_T("IBS_면장수"));			arrIgnore.Add(_T("IBS_물리적특성"));
	arrIgnore.Add(_T("IBS_딸림자료"));			arrIgnore.Add(_T("IBS_크기"));
	arrIgnore.Add(_T("IBS_발행년"));

	CString strRepBookYN, strDBFlag;
	CString strVolSortNo = m_pDM_Book->GetCellData(_T("BB_권일련번호"), nReprentBookIdx);
	if(strVolSortNo.IsEmpty()) strVolSortNo = _T("10");
	for (INT idx = 0; idx < nBookCount; idx ++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		// 색인 CM -> 책 DM
		strDBFlag = m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx);
		strRepBookYN = m_pDM_Book->GetCellData(_T("BB_대표책여부"), idx);
		if(strDBFlag == _T("N") || strRepBookYN == _T("Y"))
			ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"));
		else
			ids = m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Book, idx, _T("BB"), &arrIgnore);
		// 편권차 적용
		ids = m_pDM_Book->SetCellData(_T("BB_편권차"), strVol, idx);
		// 쿼리 플래그 적용
		if(strDBFlag.IsEmpty())
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, idx);
		if(strDBFlag == _T("N"))
			ids = m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
		//권일련번호 적용
		ids = m_pDM_Book->SetCellData(_T("BB_권일련번호"), strVolSortNo, idx);
	}

	return 0;
}

INT CSpecies_Form::ApplyWorkingStatus()
{
	INT idx = 0;
	INT nRowCount = 0;

	CString strBookWorkingStatus;

	//색인에 자료상태를 저장한다.
	m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0, strBookWorkingStatus);
	if(strBookWorkingStatus.IsEmpty())
		m_pDM_Index->SetCellData(_T("IBS_작업상태INFO"), m_strWorkingStatus, 0);
	
	// 책에 Working Status를 넣는다.
	nRowCount = m_pDM_Book->GetRowCount();

	strBookWorkingStatus = _T("");
	for (idx = 0; idx < nRowCount; idx++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;
		// 책에 이미 작업상태가 설정된 상태라면 continue
		m_pDM_Book->GetCellData(_T("BB_자료상태"), idx, strBookWorkingStatus);
		if(strBookWorkingStatus.IsEmpty())
			m_pDM_Book->SetCellData(_T("BB_자료상태"), m_strWorkingStatus, idx);
		//20070821 ADD PJW : 새로저장일경우 등록자료는 등록인계자료로 설정한다.
		if(strBookWorkingStatus.Left(3) == _T("BOR") && m_bNewSaveFlag == TRUE)
		{
			m_pDM_Book->SetCellData(_T("BB_자료상태"), _T("BOR111N"), idx);
			m_pDM_Book->SetCellData(_T("BB_등록번호"), _T(""), idx);
			m_pDM_Book->SetCellData(_T("BB_원부KEY"), _T(""), idx);
			//DELm_bNewSaveFlag = FALSE;
		}
		else
		{
				if(strBookWorkingStatus.Left(3) == _T("BOR")  && m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
				m_pDM_Book->SetCellData(_T("BB_자료상태"), _T("BOR111N"), idx);
		}
		/*
		if(strBookWorkingStatus.Left(3) == _T("BOR")  && m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
				m_pDM_Book->SetCellData(_T("BB_자료상태"), _T("BOR111N"), idx);
		*/
	}

	return 0;
}

// 2007.08.10 ADD BY PDJ
// 부록은 자료상태 부여 안해도 되나? 해야됨.
INT CSpecies_Form::ApplyAppendixWorkingStatus()
{
	INT idx = 0;
	INT nRowCount = 0;

	CString strBookWorkingStatus;

	//색인에 자료상태를 저장한다.
	m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0, strBookWorkingStatus);
	if(strBookWorkingStatus.IsEmpty())
		m_pDM_Index->SetCellData(_T("IBS_작업상태INFO"), m_strWorkingStatus, 0);
	
	// 책에 Working Status를 넣는다.
	nRowCount = m_pDM_Appendix->GetRowCount();

	strBookWorkingStatus = _T("");
	for (idx = 0; idx < nRowCount; idx++)
	{		
		// 책에 이미 작업상태가 설정된 상태라면 continue
		m_pDM_Appendix->GetCellData(_T("BB_자료상태"), idx, strBookWorkingStatus);
		if(strBookWorkingStatus.IsEmpty())
			m_pDM_Appendix->SetCellData(_T("BB_자료상태"), m_strWorkingStatus, idx);
		//20070821 ADD PJW : 새로저장일경우 등록자료는 등록인계자료로 설정한다.
		if(strBookWorkingStatus.Left(3) == _T("BOR") && m_bNewSaveFlag == TRUE)
		{
			m_pDM_Book->SetCellData(_T("BB_자료상태"), _T("BOR111N"), idx);
			m_pDM_Book->SetCellData(_T("BB_등록번호"), _T(""), idx);
			m_pDM_Book->SetCellData(_T("원부번호"), _T(""), idx);
			m_bNewSaveFlag = FALSE;
		}
		else
		{
				if(strBookWorkingStatus.Left(3) == _T("BOR")  && m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
				m_pDM_Book->SetCellData(_T("BB_자료상태"), _T("BOR111N"), idx);
		}
		/*
		if(strBookWorkingStatus.Left(3) == _T("BOR") && m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), idx) == _T("I"))
			m_pDM_Appendix->SetCellData(_T("BB_자료상태"), _T("BOR111N"), idx);
		*/
	}

	return 0;
}

VOID CSpecies_Form::OnbMARCIMPORT() 
{
	// TODO: Add your control notification handler code here
	//m_pHeadDlg->ApplyData();
	ApplyData();
	CMARC_IMPORT dlg(this);
	
	if (dlg.DoModal() == IDOK)
	{
		INT ids = -1;

		if (dlg.m_strSpecies_key.IsEmpty()) goto ERR;

		CString strMarc;
		CString strQuery;
		strQuery.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), dlg.m_strSpecies_key);
		ids = m_pDM_Species->GetOneValueQuery(strQuery, strMarc);
		if (ids < 0) goto ERR;

		CMarc marc;
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0) goto ERR;

		m_pInfo->pMarcMgr->DeleteField(&marc, _T("001"));
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));

/*
		//책정보를 대표책을 제외하고 모두 지운다.
		for(INT i=1 ; i<m_pDM_Book->GetRowCount() ; i++)
		{
			m_pDM_Book->DeleteRow(i);
		}
*/

		//2: 다권정보삭제한 전체정보반입 -> 1 + 책도 가져온다.
		if (dlg.m_nRadBasicInfo == 2)
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("505"));
		}
		
		// 공통 : Marc to Index
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
		
		if (ids < 0) goto ERR;
		
		//1: 전체 정보 반입 -> 마크로 IDX구성 MARC도 가져옴
		if (dlg.m_nRadBasicInfo == 1 || dlg.m_nRadBasicInfo == 2)
		{
			m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
			ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
		}
		else
		{
			ids = m_pDM_Species->SetCellData(_T("BS_MARC"), _T(""), 0);
		}
		m_pFormApi->SetRepresentationBook(m_pDM_Book);
		
		Display();
		AfxMessageBox(_T("Marc를 반입하였습니다."), MB_ICONINFORMATION);
		return;
/*

		CStringArray DeleteFieldList;	DeleteFieldList.RemoveAll();
		DeleteFieldList.Add(_T("001"));	DeleteFieldList.Add(_T("090"));	DeleteFieldList.Add(_T("049"));
		
		//0: 기본정보반입 1: 전체정보반입 2: 다권정보삭제한 전체정보반입
		//0: 기본서지정보만 반입한다.
		//1: 서지정보와 다권정보(505) 책정보(049), 마크정보까지 반입한다.
		//2: 서지정보와 책정보(049), 마크정보를 반입한다.

		//책정보를 대표책을 제외하고 모두 지운다.
		for(INT i=1 ; i<m_pDM_Book->GetRowCount() ; i++)
		{
			m_pDM_Book->DeleteRow(i);
		}

		// 공통 : Marc to Index
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
		if(ids < 0) goto ERR;
		if(dlg.m_nRadBasicInfo == 1 || dlg.m_nRadBasicInfo == 2)
		{
			ids = m_pBOSpeciesApi->RestructBookBy049MarcTag(m_pDM_Book, &marc, TRUE);
			if (ids < 0) goto ERR;
		}
		if(dlg.m_nRadBasicInfo == 1)
		{
			ids = m_pBOSpeciesApi->RestructBookBy505MarcTag(m_pDM_Book, &marc);
			if (ids < 0) goto ERR;
		}
		else
			DeleteFieldList.Add(_T("505"));
		
		ids = m_pBOSpeciesApi->ClearRegNoFieldOnBookDM(m_pDM_Book);
		if (ids < 0) goto ERR;
		ids = m_pBOSpeciesApi->SetVolSortNo(m_pDM_Book);
		if (ids < 0) goto ERR;
		
		//DeleteField
		for(i=0 ; i<DeleteFieldList.GetSize() ; i++)
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, DeleteFieldList.GetAt(i));
		}

		//1: 전체 정보 반입 -> 마크로 IDX구성 MARC도 가져옴
		if (dlg.m_nRadBasicInfo == 1 || dlg.m_nRadBasicInfo == 2)
		{
			m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
			ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
		}
		m_pFormApi->SetRepresentationBook(m_pDM_Book);
		
		Display();
		AfxMessageBox(_T("Marc를 반입하였습니다."), MB_ICONINFORMATION);
		return;
*/
	}

	return;

ERR:
	AfxMessageBox(_T("Marc 반입 실패"));
	return;

}	


VOID CSpecies_Form::OnbKOLISNET() 
{
	// TODO: 공동목록 반입	
	ApplyData();
	CUC_MARC_IMPORT dlg(this);
	CString sTitle, sAuthor, sPublisher, sPubYear;
	
	GetDlgItem(IDC_eTITLE)->GetWindowText(sTitle);
	GetDlgItem(IDC_eAUTHOR1)->GetWindowText(sAuthor);
	GetDlgItem(IDC_ePUBLISHER)->GetWindowText(sPublisher);
	GetDlgItem(IDC_ePUBLISH_YEAR)->GetWindowText(sPubYear);

	dlg.SetSearchCondition( sTitle, sAuthor, sPublisher, sPubYear );
	if (dlg.DoModal() != IDOK) return;
	
	if( dlg.m_TocCheck_CON == TRUE )   // 목차 반입 여부 확인 
    {
		m_bInsertAC = TRUE;
        m_strContents = dlg.m_Contents;                // 목차 가져오기 
		m_pDM_Index->SetCellData(_T("IBS_목차유무"), _T("Y"), 0);
	}
    if( dlg.m_TocChek_ABS == TRUE )   // 초록 반입 여부 확인 
    {
		m_bInsertAC = TRUE;
        m_strAbstracts = dlg.m_Abstracts;              // 초록 가져오기 
		m_pDM_Index->SetCellData(_T("IBS_초록유무"), _T("Y"), 0);
    }
	CMarc marc;
	
	//===================================================
	//2009.05.06 UPDATE BY LKS : 
	CString str056Field;
	//===================================================

	if(dlg.sMarc.IsEmpty()) return;
	INT ids = m_pInfo->pMarcMgr->Decoding(dlg.sMarc, &marc);
	if (ids < 0) goto ERR;

/*
	//책정보를 대표책을 제외하고 모두 지운다.
	for(i=1 ; i<m_pDM_Book->GetRowCount() ; i++)
	{
		m_pDM_Book->DeleteRow(i);
	}
*/
	
	// 마크로 IDX 구성 (마크는 버림)
	ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
	if (ids < 0) goto ERR;
	
	// MARC 가져와 세팅
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), dlg.sMarc, 0);
	if (ids < 0) goto ERR;
	//색인 정보 -> 대표책정보 -> 책에 저장될때 대표저자만 들어가야 한다.
	m_pFormApi->SetRepresentationBook(m_pDM_Book);

	Display();

	//===================================================
	//2009.05.22 UPDATE BY LKS : 반입받은 마크의 056태그 a다음 숫자를 읽어 주제구분을 Set

	m_pInfo->pMarcMgr->GetField(&marc, _T("056"), str056Field);
	
	if(!str056Field.IsEmpty())
	{
		INT idx = str056Field.FindOneOf(_T("a"));
		
		CString strSubjectCode,strTemp;
		strSubjectCode = str056Field.GetAt(idx+1);

		CESL_ControlMultiComboBox* pCM_Subject = (CESL_ControlMultiComboBox *)m_pCM_Accu->FindControl(_T("BP_주제구분_코드"));
		if( pCM_Subject != NULL )
		{
			INT nComboCnt = pCM_Subject->GetRowCount();
			for( INT i=0; i < nComboCnt; i++ )
			{
				strTemp = pCM_Subject->GetItemDataString(i,0);
				
				if( strTemp == strSubjectCode ) break;
			}
				
			if( i <= nComboCnt)
				pCM_Subject->SetCurSel(i);
		}
	}
	//===================================================

	MessageBox(_T("공동목록을 반입하였습니다."));
	return;

ERR :
	MessageBox(_T("공동목록 반입실패"));
	
}

VOID CSpecies_Form::OnbAPPENDIX() 
{
	// TODO: Add your control notification handler code here
	INT ids = ApplyData();
	if(ids < 0) return ;

	CBOAppendixDlg dlg(this);
	dlg.SetFormApi(m_pFormApi);
	if(m_nOpType == SINSERT) dlg.SetOpenMode(1);
	else dlg.SetOpenMode(-1);
	dlg.SetAppendixDM(m_pDM_Appendix);
	dlg.SetSpeciesDM(m_pDM_Species);
	dlg.SetIndexDM(m_pDM_Index);
	dlg.SetBookDM(m_pDM_Book);
	dlg.DoModal();		
}

VOID CSpecies_Form::OnbCLEARSCREEN() 
{
	// TODO: Add your control notification handler code here
	ClearScreen();
	m_pTailDlg->ClearScreen();
	InitRegCodeField();
	OnKillfocuseREGCLASSCODE();
	this->OnSelchangecCLASIFYCLASS();
	GetDlgItem(IDC_eTITLE)->SetFocus();
	//2006-03-03 다권본일때 권/책종보를 기본값만 남기고 지운다.
	INT nRow = m_pDM_Book->GetRowCount();
	if( nRow > 1)
	{
		for( INT i = nRow; i > 1; i--)
		m_pDM_Book->DeleteRow(i-1);
	}
}

VOID CSpecies_Form::OnKillfocuseREGCLASSCODE() 
{
	// TODO: Add your control notification handler code here
	CString strRegCodeField;
	GetDlgItem(IDC_eREG_CLASS_CODE)->GetWindowText(strRegCodeField);
	
	CArray<CRegCode, CRegCode> arrRegCode;
	INT ids = m_pFormApi->FetchRegCode(strRegCodeField, &arrRegCode);
	if (ids < 0)
	{
		MessageBox(_T("없는 등록구분입니다."));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		return;
	}

	INT nRegCodeCount = arrRegCode.GetSize();
	if (nRegCodeCount <= 0) return;
	
	// 등록구분 바뀌는지 체크
	CString strVol;
	m_pCM_Index->GetControlMgrData(_T("IBS_편권차"), strVol);
	if (m_nOpType != SINSERT && m_pFormApi->IsRegCodeModify(&arrRegCode, m_pDM_Book, strVol))
	{
		AfxMessageBox(_T("등록구분이 수정이 될 수 없습니다. 책정보에서 수정해주세요"));
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
		return;			
	}

	m_strRegCodeField = strRegCodeField;

	INT nBookCount = 0;
	for (INT i = 0; i < nRegCodeCount; i++)
	{
		nBookCount += arrRegCode.GetAt(i).m_nCount;
	}

	CString strBookCount, strPrevBookCount;
	strBookCount.Format(_T("%d"), nBookCount);
	m_pCM_Accu->GetControlMgrData(_T("BP_총책수"), strPrevBookCount);
	if(strBookCount != strPrevBookCount)
	{
		m_pTailDlg->SetInit(FALSE);
		ApplyData();
	}
	m_pCM_Accu->SetControlMgrData(_T("BP_총책수"), strBookCount);
	m_bViewVolInfo = FALSE;	

}

VOID CSpecies_Form::OnbPREVSPECIES() 
{
	//===================================================
	//2009.03.28 UPDATE BY LKS : 시간컨트롤 초기화
	m_hApplicantTime->SetData(_T(""));
	//===================================================
	m_nParentSelectedRow = GetPrevSelectIndex();
	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	UpdateInitProc();
	if( MakeUpdateDM() < 0 )
	{
		AfxMessageBox(_T("UPDATE DM 생성 실패"));
		return;
	}

	if( m_bAcqTypeChange )	SetHeadTailInfoDlg();
	else m_pHeadDlg->Init();

	ApplyMarcToIndexCM();
	m_pTailDlg->Init();
	Display();	
	SetEBookCtrl();

	//===================================================
	//2009.10.20 ADD BY LKS
	OnSelchangecCLASIFYCLASS();
	//===================================================
}

VOID CSpecies_Form::OnbNEXTSPECIES() 
{
	//===================================================
	//2009.03.28 UPDATE BY LKS : 시간컨트롤 초기화
	//===================================================
	m_hApplicantTime->SetData(_T(""));

	m_nParentSelectedRow = GetNextSelectIndex();
	if( m_nParentSelectedRow > m_pDM_Parent->GetRowCount()-1 )
	{
		m_nParentSelectedRow = m_pDM_Parent->GetRowCount()-1;
	}

	//===================================================
	//2009.05.20 ADD BY LKS : 마지막 자료 저장시 m_nParentSelectedRow가 -1로 떨어져
	//						  MakeUpdateDM()시 ERROR
	if(m_nParentSelectedRow == -1)
	{
		if(m_pGrid_Parent->SelectGetCount() == 1)
			m_nParentSelectedRow = m_pGrid_Parent->SelectGetHeadPosition();
		else
			m_nParentSelectedRow = m_pDM_Parent->GetRowCount()-1;
		CWnd* hWnd;
		hWnd = GetDlgItem(IDC_bNEXT_SPECIES);
		if(hWnd != NULL)		
			hWnd->EnableWindow(FALSE);
	}
	//===================================================

	m_pGrid_Parent->SetReverse(m_nParentSelectedRow);
	UpdateInitProc();
	if( MakeUpdateDM() < 0 )
	{
		AfxMessageBox(_T("UPDATE DM 생성 실패"));
		return;
	}

	if( m_bAcqTypeChange )		SetHeadTailInfoDlg();
	else m_pHeadDlg->Init();
	ApplyMarcToIndexCM();
	m_pTailDlg->Init();
	Display();	
	SetEBookCtrl();

	//===================================================
	//2009.10.20 ADD BY LKS
	OnSelchangecCLASIFYCLASS();
	//===================================================
}

VOID CSpecies_Form::OnbSAVEAS() 
{
	// 20070821 ADD PJW : 새로저장 여부 플래그 TRUE 세팅
	m_bNewSaveFlag = TRUE;
	
	/************************************************************************
	 ************************************************************************/
	//20080515 ADD BY PJW : 서명만 입력하고 바로 저장시 자동복본조사 하지않도록 수정
	KillTimer(0);
	/************************************************************************ 
	 ************************************************************************/
	// TODO: Add your control notification handler code here
	if (m_nOpType == SINSERT) return;

	// 경고메시지
	if (MessageBox(_T("새로저장 작업을 하시면 더이상 수정 작업을 할 수 없습니다.\r\n"
				_T("계속하시겠습니까?")), NULL, MB_YESNO | MB_ICONQUESTION) != IDYES) return;

	// 책의 U -> I
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 플래그가 U가 아니면 넘어간다.
		if (m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), idx) != _T("U")) continue;

		m_pDM_Book->SetCellData(_T("BB_DB_FLAG"), _T("I"), idx);
	}

	/**
		저장 플로우
	*/
	
	// 데이터 확인
	INT ids = ValidateData();
	if (ids < 0) return;

	//===================================================
	//2009.05.28 ADD BY LKS : Trim Control
	TrimControl();
	//===================================================

	// 입력한 데이터 적용
	//SetOpType(SINSERT);
	m_pBOSpeciesApi->SetOpenMode(1100); //1100:입력, -1100:수정
	m_pHeadDlg->SetOpType(SINSERT);
	
	ids = ApplyData();
	if (ids < 0)
	{
		AfxMessageBox(_T("데이터 적용실패"));
		m_bNewSaveFlag = FALSE;
		return;
	}
	// 마크 오퍼레이션
	ids = ApplyIndexToMarc();
	if (ids < 0)
	{
		AfxMessageBox(_T("저장 실패"));
		EndDialog(IDOK);
		m_bNewSaveFlag = FALSE;
		return;
	}

	m_bNewSaveFlag = FALSE;

	m_pDM_Species->StartFrame();
	ids = CheckWorkNoInfo(m_pDM_Species);
	if( ids < 0)
	{
		ESLAfxMessageBox(_T("작업번호를 생성할 수 없습니다."));
		return ;
	}
	// 적용
	// 2004.11.09. 5163 표지기능 추가.
	// 새로저장일 경우 REC_KEY를 새로 만들도록 하기 위해 Species_key의 값을 비운다.
	if( m_strUseCoverYN == _T("Y"))	
	{
		m_pBOSpeciesApi->m_strSPECIES_KEY.Empty();
	}
	//===============================================================
	//20080613 ADD BY PJW : 주문번호를 NULL로 셋텡한다.
	//m_pDM_Accu->SetCellData((_T("BP_발주일련번호"), _T(""),0);
	//===============================================================
	ids = m_pBOSpeciesApi->BO_SPECIES_INSERT(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)m_nSpeciesType);
	if (ids < 0)		AfxMessageBox(_T("저장 실패"));
	else
	{
		if( m_nParentType == SPECIES || m_nParentType == VOLUME )
		{
			m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
		}
		//    m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
		if( m_nParentType == SPECIES )			
		{
			m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
			// 발주가격을 부모DM에 적용한다.(그리드출력용)
			m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_pDM_Parent->GetRowCount()-1 );
			//===================================================
			//2009.05.22 ADD BY PJW : 구입가격을 책DM에도 추가한다.
			INT nParentBookDmRowCnt;
			CString strOrderPrice;
			CString strSpeciesKey;
			strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_종KEY"), m_nParentSelectedRow );
			nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 			for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
			{
				if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_종KEY"), i) ) 
				{
					m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
				}
			}
			//===================================================
			
			// 그리드를 다시 그린다.
			m_pGrid_Parent->DisplayLine(m_pDM_Parent->GetRowCount()-1);
			m_pGrid_Parent->DisplayNumber();
			m_pGrid_Parent->SetTopRow(m_pDM_Parent->GetRowCount()-1+1);
		}
		else	m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

		if(m_bInsertAC == TRUE)
		{
			m_bInsertAC = FALSE;
			ids = InsertAbstractContentsInfo();			
			if(ids < 0) ESLAfxMessageBox(_T("목차초록 정보를 저장하지 못했습니다."));
		}

		// 2005-04-08 표지기능을 사용하는 도서관은 메시지를 다르게 보여준다.
		if( m_strUseCoverYN == _T("Y") )
		{
			MessageBox(_T("저장하였습니다.\n새로저장의 경우 표지정보는 저장되지 않습니다."), NULL, MB_OK | MB_ICONINFORMATION);	
		}
		else
		{
			ESLAfxMessageBox(_T("저장하였습니다."));
		}

		m_bControlDisplay = TRUE;
		m_pBOSpeciesApi->SetOpenMode(-1100); //1100:입력, -1100:수정
		m_pHeadDlg->SetOpType(SUPDATE);
		// 2004.12.21 저장 후 Species_key 값 초기화한다.
		m_szSpeciesKey[0] = '\0';
		OnbNEXTSPECIES();		
	}
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 필수항목을 체크한다. (서명, 등록구분) + Head / Tail_Dlg
//
// [ 미니스펙 ]
// 1. 꼭 들어가야될 항목을 설정한다.(서명, 등록구분)
// 2. 설정된 항목들에 값이 있는지 판단하여 없다면 메시지로 알려준다.
// 3. Head / TailDlg의 값도 유효한지 체크한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::ValidateData()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT i, ids;
	CString strText, strMsg;
	CStringArray ArrAlias;	
	CArray<INT, INT> ArrResource;
	CWnd* pWnd = NULL;
	
	
	//=======================================================================
	// 1. 꼭 들어가야될 항목을 설정한다.(서명, 등록구분)
	//=======================================================================
	ArrAlias.RemoveAll();	
	ArrAlias.Add(_T("서명"));	
	ArrAlias.Add(_T("등록구분"));
	
	ArrResource.RemoveAll();
	ArrResource.Add(IDC_eTITLE);	
	ArrResource.Add(IDC_eREG_CLASS_CODE);


	//=======================================================================
	// 2. 설정된 항목들에 값이 있는지 판단하여 없다면 메시지로 알려준다.
	//=======================================================================
	for( i=0; i<ArrAlias.GetSize(); i++ )
	{
		pWnd = GetDlgItem(ArrResource.GetAt(i));
		if( pWnd == NULL )		continue;

		pWnd->GetWindowText(strText);
		if( strText.IsEmpty() == TRUE )
		{
			strMsg.Format(_T("%s 을(를) 입력해 주세요"), ArrAlias.GetAt(i));
			MessageBox(strMsg);
			//==========================================================================
			//20080515 ADD BY PJW : 등록구분을 입력하지 않았을 경우 이전등록번호가 ""이면 디폴트값으로 넣어준다.
			if( m_strRegCodeField.IsEmpty() == TRUE )
			{
				// DB에서 기본값이 설정되어 있는지 판단하여 없다면 EM을 기본으로 한다.
				m_pInfo->pCodeMgr->GetDefaultCode(_T("등록구분"), m_strRegCodeField);
				if( m_strRegCodeField.IsEmpty() == TRUE )	m_strRegCodeField = _T("EM");
				GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
			}
			//==========================================================================
			pWnd->SetFocus();
			return -1;
		}
	}

	
	//=======================================================================
	// 3. Head / TailDlg의 값도 유효한지 체크한다.
	//=======================================================================
	ids = m_pHeadDlg->ValidateData();
	if( ids < 0 )	return -1;
	ids = m_pTailDlg->ValidateData();
	if( ids < 0 )	return -1;
	
	return 0;
}

INT CSpecies_Form::ApplyIndexToMarc(INT idx /*= 0*/)
{
	INT ids = -1;
	CMarc marc;
	CString strMarc;

	// 종 -> 마크
	strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	if (!strMarc.IsEmpty())
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
		if (ids < 0)
		{
			ESLAfxMessageBox(_T("저장된 마크정보가 올바르지 않습니다. 마크를 재생성 하겠습니다."));
			//return -1;
		}
	}

	//===================================================
	//2009.05.06 ADD BY LKS : 새로저장시에 049태크를 지워줌
	
	if(m_bNewSaveFlag)
	{
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	}
	//===================================================

	//종에서 마크를 생성시에 소장종이 아닌경우 100Tag를 재생성해야 한다.
	if(m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0).Left(3) != _T("BOC") 
		&& m_pDM_Index->GetCellData(_T("IBS_작업상태INFO"), 0).Left(3) != _T("BOL"))
	{
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("100"));
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("700"));
		//==========================================================================
		//20080502 ADD BY PJW : MARC생성하기전에 740tag는 삭제를 해주어 두개가 생기는 것을 방지한다.
		m_pInfo->pMarcMgr->DeleteField(&marc, _T("740"));//,_T(""));
		//==========================================================================
	}
	//==========================================================================
	//20080623 ADD BY PJW : 셋트isbn이 변경되거나 지워지지 않아서 저장하기 전에 
	m_pInfo->pMarcMgr->DeleteField(&marc, _T("020"));
	//==========================================================================
	ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
	m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
	m_pInfo->pMarcMgr->DeleteEmptySubfield(&marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&marc);
	ids = m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if (ids < 0) return ids;
	
	m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
	
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 저장 버튼
// [미니스펙 ]
//
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnbSAVE() 
{
	INT ids, openMode;
	CString strMarc;
	CMarc marc;	

	//==========================================================================
	//20080515 ADD BY PJW : 서명만 입력하고 바로 저장시 자동복본조사 하지않도록 수정
	KillTimer(0);
	//==========================================================================

	//==========================================================================
	//20080321 ADD BY PJW : 형식구분이 적용안함일경우 리턴한다.
	CString strFormCode;
	m_pCM_Index->GetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
	if(strFormCode.IsEmpty())
	{
		ESLAfxMessageBox(_T("형식구분은 적용안함을 선택할 수 없습니다."));
		CESL_ControlMultiComboBox * pCmb = (CESL_ControlMultiComboBox*)m_pCM_Index->FindControl(_T("IBS_형식구분_코드"));
		m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);
		m_pCM_Index->SetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
		if(pCmb) pCmb->SetFocus();
		return ;
	}
	//==========================================================================
	
	//=======================================================================
	// 1. 필수항목을 체크한다. (서명, 등록구분) + Head / Tail_Dlg
	//=======================================================================
	ids = ValidateData();
	if( ids < 0 )	return;

	//===================================================
	//2009.05.28 ADD BY LKS : Control Trim
	TrimControl();
	//===================================================

	// 입력한 데이터 적용
	ids = ApplyData();
	if( ids < 0 )
	{
		ESLAfxMessageBox(_T("데이터 적용실패"));
		return;
	}

	//=======================================================================
	//20080320 ADD BY PJW : 부록의 수입구분을 종과 맞춰준다.
	INT nAppendixCnt;
	nAppendixCnt = m_pDM_Appendix->GetRowCount();
	if ( m_nAcqType == PURCHASE )
	{
		for (INT i = 0 ; i < nAppendixCnt ; i++ )
		{
			m_pDM_Appendix->SetCellData(_T("BB_수입구분_코드"), _T("1"), i);
		}		
	}
	else if ( m_nAcqType == DONATE )
	{
		for (INT i = 0 ; i < nAppendixCnt ; i++ )
		{
			m_pDM_Appendix->SetCellData(_T("BB_수입구분_코드"), _T("2"), i);
		}
	}
	//=======================================================================
	//=======================================================================
	//2008.07.17 ADD BY PJW : 저장할경우 부록이 존재하면 자동 등록번호를 부여를 수행한다.
	// 부록 정보가 있으면 
	INT nBookCnt;
	CString strWorkStatus;
	if ( 0 < nAppendixCnt )
	{
		nBookCnt = m_pDM_Book->GetRowCount();
		for( INT i = 0 ; i < nBookCnt ; i++ )
		{
			ids = m_pDM_Book->GetCellData( _T("BB_자료상태"), i, strWorkStatus);
			if ( strWorkStatus == _T("BOR112N") )
			{
				// 부록 등록번호 적용한다.
				ids = ApplyAppendixRegNo(m_pDM_Appendix);
				if (ids < 0)
				{
					m_strLastErrMsg = _T("부록정보 쿼리만들기 실패");
					return;
				}
				
				break;
			}
		}
	}
	
	//=======================================================================

	// 종 API에 오픈모드 설정
	openMode = -1;
	if( m_nOpType == SINSERT )			openMode = 1100;
	else /*if(m_nOpType != SINSERT)*/	openMode = -1100;
	m_pBOSpeciesApi->m_nOpenMode = openMode;

	// 복본추기
	if( m_bDupAddMode == TRUE )
	{
		SPECIES_TYPE nSType = m_nSpeciesType;
		if( nSType == BO_REGOBJ )
		{
			if		( m_nAcqType == PURCHASE )	nSType = BO_REG_PURCHASE;
			else if	( m_nAcqType == DONATE	 )	nSType = BO_REG_DONATE;
		}		

		m_pDM_Book->StartFrame();
		ids = CheckWorkNoInfo(m_pDM_Book);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("작업번호를 생성할 수 없습니다."));
			return ;
		}
		//수정종정보에 따른 권일련번호/복본기호등을 수정한다.
		CBOVolumeDlg dlg(this, m_pDM_Book);
		dlg.SetDupIdxKey(m_strDupIndexKey);
		ids = dlg.AdjustCurrentBookDMByHoldBook(m_pDM_Book);
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("작업하신 책정보가 올바르지 않습니다."));
			return ;
		}
		// OpenMode에 따라서 복본추기 함수를 불러온다. 
		if( m_nOpType == SINSERT )
		{
			ids = m_pBOSpeciesApi->BO_SPECIES_ADDBOOK(_T("INSERT"), m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)nSType, (INT)m_nAcqType);
			if(ids >= 0)
			{
				if(m_nParentType == SPECIES || m_nParentType == VOLUME)
				{
					m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
				}				
				if( m_nParentType == SPECIES )
				{
					m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
					// 발주가격을 부모DM에 적용한다.(그리드출력용)
					m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_pDM_Parent->GetRowCount()-1 );

					//===================================================
					//2009.05.22 ADD BY PJW : 구입가격을 책DM에도 추가한다.
					INT nParentBookDmRowCnt;
					CString strOrderPrice;
					CString strSpeciesKey;

					strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_종KEY"), m_pDM_Parent->GetRowCount()-1 );
					nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 					for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
					{
						if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_종KEY"), i) ) 
						{
							m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
						}
					}
					//===================================================
					
					// 그리드를 다시 그린다.
					m_pGrid_Parent->DisplayLine(m_pDM_Parent->GetRowCount()-1);
					m_pGrid_Parent->DisplayNumber();
					m_pGrid_Parent->SetTopRow(m_pDM_Parent->GetRowCount()-1+1);
				}
				else	m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

				m_bDupAddMode = FALSE;
				
				SetDataManagers();
				m_pDM_Index->InsertRow(-1);
				m_pDM_Species->InsertRow(-1);
				m_pDM_Accu->InsertRow(-1);
				m_pDM_Book->InsertRow(-1);
				m_pFormApi->AllControlClear(m_pCM_Index, &m_arrIgnoreCTRLAlias);
				m_pFormApi->AllControlClear(m_pCM_Accu, &m_arrIgnoreCTRLAlias);
				m_pTailDlg->AllControlClear();
				LoadDefaultValue();		
				m_bInsertData = TRUE;
				SetEBookCtrl();
			}
		}
		else if (m_nOpType != SINSERT)
		{
			ids = m_pBOSpeciesApi->BO_SPECIES_ADDBOOK(_T("UPDATE"), m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)nSType, (INT)m_nAcqType);
			if(ids >= 0)
			{
				if(m_nSpeciesType == SPECIES || m_nSpeciesType == VOLUME)
				{
					m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
				}		
				
				if( m_nParentType == SPECIES )
				{
					m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, m_nParentSelectedRow, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
					// 발주가격을 부모DM에 적용한다.(그리드출력용)
					m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_nParentSelectedRow );
					//===================================================
					//2009.05.22 ADD BY PJW : 구입가격을 책DM에도 추가한다.
					INT nParentBookDmRowCnt;
					CString strOrderPrice;
					CString strSpeciesKey;
					
					strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_종KEY"), m_pDM_Parent->GetRowCount()-1 );
					nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 					for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
					{
						if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_종KEY"), i) ) 
						{
							m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
						}
					}
					//===================================================

					// 그리드를 다시 그린다.
					m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);
					m_pGrid_Parent->DisplayNumber();
					m_pGrid_Parent->SetTopRow(m_nParentSelectedRow+1);
				}
				else	m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, m_nParentSelectedRow, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

				m_bControlDisplay = TRUE;
				m_bDupAddMode = FALSE;
				m_pFormApi->AllControlClear(m_pCM_Index, NULL);
				m_pFormApi->AllControlClear(m_pCM_Accu, NULL);
				m_pTailDlg->AllControlClear();
				OnbNEXTSPECIES();
				GetDlgItem(IDC_edtWORKING_STATUS)->SetFocus();
			}
		}
		
		if (ids < 0)	
		{
			AfxMessageBox(_T("저장 실패"));
			return;
		}
		else
		{
			MessageBox(_T("저장 하였습니다."), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			m_bInsertData = TRUE;
			// 2004.12.21 저장 후 Species_key 값 초기화한다.
			m_szSpeciesKey[0] = '\0';
		}
	}
	// 종-입력
	else if (m_nOpType == SINSERT)
	{
		// 마크 오퍼레이션
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("저장 실패"));
			return;
		}
		m_pDM_Species->StartFrame();
		ids = CheckWorkNoInfo(m_pDM_Species);
		if( ids < 0)
		{
			ESLAfxMessageBox(_T("작업번호를 생성할 수 없습니다."));
			return ;
		}
		
		// 2004.11.09. 5163 표지기능 추가.
		if( m_strUseCoverYN == _T("Y"))
		{
			m_pBOSpeciesApi->m_strSPECIES_KEY = m_szSpeciesKey;
		}					

		ids = m_pBOSpeciesApi->BO_SPECIES_INSERT(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)m_nSpeciesType);
		if (ids < 0)	ESLAfxMessageBox(_T("저장 실패"));
		else
		{
			if( m_nParentType == SPECIES || m_nParentType == VOLUME )
			{
				m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
//				::PostMessage(pParentMgr->GetSafeHwnd(), CHANGE_DM , 0, 0);
			}			
			if( m_nParentType == SPECIES )
			{
				m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
				// 발주가격을 부모DM에 적용한다.(그리드출력용)
				m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_pDM_Parent->GetRowCount()-1 );
				//===================================================
				//2009.05.22 ADD BY PJW : 구입가격을 책DM에도 추가한다.
				INT nParentBookDmRowCnt;
				CString strOrderPrice;
				CString strSpeciesKey;
				strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_종KEY"), m_nParentSelectedRow );
				nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 				for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
				{
					if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_종KEY"), i) ) 
					{
						m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
					}
				}
				//===================================================
								
				//===================================================
				//2009.04.13 UPDATE BY LKS : 그리드에 기증자 소속,전화번호,주소 추가
				CString strData,strQuery;
				CString strTEL,strPOSITION,strADDRESS;
				m_pDM_Accu->GetCellData(_T("BP_기증자정보KEY"),0,strData);
				if(!strData.IsEmpty())
				{
					strQuery.Format(_T("SELECT ADDRESS FROM CO_DONATOR_TBL WHERE REC_KEY=%s"),strData);
					m_pDM_Accu->GetOneValueQuery(strQuery,strADDRESS);
					strQuery.Format(_T("SELECT POSITION FROM CO_DONATOR_TBL WHERE REC_KEY=%s"),strData);
					m_pDM_Accu->GetOneValueQuery(strQuery,strPOSITION);
					strQuery.Format(_T("SELECT TEL FROM CO_DONATOR_TBL WHERE REC_KEY=%s"),strData);
					m_pDM_Accu->GetOneValueQuery(strQuery,strTEL);

					m_pDM_Parent->SetCellData(_T("DO_소속"),strPOSITION,m_pDM_Parent->GetRowCount()-1);
					m_pDM_Parent->SetCellData(_T("DO_전화번호"),strTEL,m_pDM_Parent->GetRowCount()-1);
					m_pDM_Parent->SetCellData(_T("DO_주소"),strADDRESS,m_pDM_Parent->GetRowCount()-1);
				}
				//===================================================
				
				// 그리드를 다시 그린다.
				m_pGrid_Parent->DisplayLine(m_pDM_Parent->GetRowCount()-1);
				m_pGrid_Parent->DisplayNumber();
				m_pGrid_Parent->SetTopRow(m_pDM_Parent->GetRowCount()-1+1);
			}
			else m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, -1, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

			if(m_bInsertAC == TRUE)
			{
				m_bInsertAC = FALSE;
				ids = InsertAbstractContentsInfo();			
				if(ids < 0) ESLAfxMessageBox(_T("목차초록 정보를 저장하지 못했습니다."));
			}

			MessageBox(_T("저장 하였습니다"), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			
			SetDataManagers();
			m_pDM_Index->InsertRow(-1);
			m_pDM_Species->InsertRow(-1);
			m_pDM_Accu->InsertRow(-1);
			m_pDM_Book->InsertRow(-1);
			m_pFormApi->AllControlClear(m_pCM_Index, &m_arrIgnoreCTRLAlias);
			m_pFormApi->AllControlClear(m_pCM_Accu, &m_arrIgnoreCTRLAlias);
			m_pTailDlg->AllControlClear();
			// 2004.11.09  SpeciesKey 초기화
			m_szSpeciesKey[0] = '\0';
			LoadDefaultValue();	
			m_bInsertData = TRUE;
			SetEBookCtrl();
			//2005-11-17 : 구입신청일 기본값 세팅
			CString strDate;
			CString sDate = m_pBOSpeciesApi->GetTodayDate();
			CTime t = CTime::GetCurrentTime();
			strDate.Format(_T("%s %02d:%02d:%02d"), sDate, t.GetHour(), t.GetMinute(), t.GetSecond());		// 입력일
			strApplicantTime(strDate);
			
			//===================================================
			//2009.03.28 UPDATE BY LKS : 시간컨트롤 초기화 및 구입신청일 자동으로 추가되지 않도록 수정
			
			/*m_pCM_Index->SetControlMgrData(_T("구입신청일"), sDate, 0);*/
			m_hApplicantTime->SetData(_T(""));
			//===================================================

			GetDlgItem(IDC_edtBSFORM_REQUESTOR)->SetWindowText(_T(""));
			/*//20070810 ADD PJW : 신청일에 시분초 추가			
			CString strDate = m_pBOSpeciesApi->GetTodayDate();
			CString sDate, strTime;
			CTime t = CTime::GetCurrentTime();
			//strDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());		// 입력일
			sDate.Format(_T("%s %02d:%02d:%02d"), strDate, t.GetHour(), t.GetMinute(), t.GetSecond());		// 입력일
			strApplicantTime(sDate);
			m_pCM_Index->SetControlMgrData(_T("구입신청일"), sDate, 0);
			*/			
			//m_pCM_Index->SetControlMgrData(_T("구입신청일"), sDate, 0);
		}
	}
	// 수서-종-구입-수정
	else if (m_nOpType != SINSERT)
	{		
		// 마크 오퍼레이션
		ids = ApplyIndexToMarc();
		if (ids < 0)
		{
			AfxMessageBox(_T("저장 실패"));
			return;
		}
		
		m_pDM_Species->StartFrame();
		ids = CheckWorkNoInfo(m_pDM_Species);
		if( ids < 0)
		{
			ESLAfxMessageBox(_T("작업번호를 생성할 수 없습니다."));
			return ;
		}			
	
		ids = m_pBOSpeciesApi->BO_SPECIES_UPDATE(m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, m_pDM_Appendix, 0, (INT)m_nSpeciesType);
		if (ids < 0)	AfxMessageBox(_T("수정 실패"));		
		else
		{
			if(m_nParentType == SPECIES || m_nParentType == VOLUME)
			{
				m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent_Book, m_pGrid_Parent, -1, 0, BOOK, FALSE, m_pHeadDlg->GetGroupInfo());
			}
			if( m_nParentType == SPECIES )
			{
				m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, m_nParentSelectedRow, 0, m_nParentType, FALSE, m_pHeadDlg->GetGroupInfo());
				// 발주가격을 부모DM에 적용한다.(그리드출력용)
				m_pTailDlg->SetOrderPrice_InDM(m_pDM_Parent, m_nParentSelectedRow );
				//===================================================
				//2009.05.22 ADD BY PJW : 구입가격을 책DM에도 추가한다.
				INT nParentBookDmRowCnt;
				CString strOrderPrice;
				CString strSpeciesKey;
				strSpeciesKey = m_pDM_Parent->GetCellData(_T( "IBS_종KEY"), m_nParentSelectedRow );
				nParentBookDmRowCnt = m_pDM_Parent_Book->GetRowCount();
 				for( INT i = 0 ; i < nParentBookDmRowCnt ; i++ )
				{
					if ( strSpeciesKey == m_pDM_Parent_Book->GetCellData(_T("IBS_종KEY"), i) ) 
					{
						m_pTailDlg->SetOrderPrice_InDM( m_pDM_Parent_Book, i );
					}
				}
				//===================================================
				
				// 그리드를 다시 그린다.
				m_pGrid_Parent->DisplayLine(m_nParentSelectedRow);
				m_pGrid_Parent->DisplayNumber();
				m_pGrid_Parent->SetTopRow(m_nParentSelectedRow+1);
			}
			else m_pBOSpeciesApi->APPLY_PARENT_DM(m_pDM_Index, m_pDM_Species, m_pDM_Accu, m_pDM_Book, m_pDM_Parent, m_pGrid_Parent, m_nParentSelectedRow, 0, m_nParentType, TRUE, m_pHeadDlg->GetGroupInfo());

			/**************************************************************************
			 **************************************************************************/
			//20080221 UPDATE BY PJW : 수서단계에서 TOC_COMMON_TBL 생기지 않도록 수정
			//if(m_bInsertAC == TRUE)
			//m_bInsertAC = FALSE; {   -> { m_bInsertAC = FALSE; 
			/**************************************************************************
			 **************************************************************************/
			if(m_bInsertAC == TRUE)
			{
				m_bInsertAC = FALSE;
				ids = InsertAbstractContentsInfo();			
				if(ids < 0) ESLAfxMessageBox(_T("목차초록 정보를 저장하지 못했습니다."));		
			}
			//기증/기증미등록자료관리의 수정의 경우 SPECIES_TYPE과 자료상태가 일치하지 않으면 해당 자료를 DM/GRID에서 삭제하여야 한다.
			// BO_DONATE = BOA211O, BO_NONDONATE = BOA212O가 아니면 위의 경우이다.
			//
			if(m_nSpeciesType == BO_DONATE || m_nSpeciesType == BO_NONDONATE)
			{
				if(!CheckDonateRegYN())		AddDonateRegYNDeleteRow(m_pArrDonateRegYN, m_nParentSelectedRow);
				else						DeleteDonateRegYNDeleteRow(m_pArrDonateRegYN, m_nParentSelectedRow);
			}
		
			MessageBox(_T("수정 하였습니다"), NULL, MB_OK | MB_ICONINFORMATION);	
			m_bControlDisplay = TRUE;
			// 2004.12.21  SpeciesKey 초기화
			m_szSpeciesKey[0] = '\0';
			OnbNEXTSPECIES();
			
			GetDlgItem(IDC_edtWORKING_STATUS)->SetFocus();

//			::PostMessage(pParentMgr->GetSafeHwnd(), CHANGE_DM , 0, 0);
		}
	}	
	//20070522 ADD PJW : 서명에 포커스
	GetDlgItem(IDC_eTITLE)->SetFocus();
}

VOID CSpecies_Form::AddDonateRegYNDeleteRow(CArray<INT, INT> * pArrDonateRegYN, INT nRow)
{
	INT cnt = pArrDonateRegYN->GetSize();
	INT nTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		nTmpData = pArrDonateRegYN->GetAt(i);
		if(nTmpData < nRow) continue;
		else if(nTmpData > nRow)
		{
			pArrDonateRegYN->InsertAt(i, nRow);
			return ;
		}
	}
	pArrDonateRegYN->Add(nRow);
}

BOOL CSpecies_Form::CheckDonateRegYN()
{
	if(m_nSpeciesType == BO_DONATE && m_strWorkingStatus == _T("BOA212O")) 
		return FALSE;
	else if(m_nSpeciesType == BO_NONDONATE && m_strWorkingStatus == _T("BOA211O"))
		return FALSE;
	return TRUE;
}

VOID CSpecies_Form::DeleteDonateRegYNDeleteRow(CArray<INT, INT> * pArrDonateRegYN, INT nRow)
{
	if(pArrDonateRegYN == NULL) return ;
	INT cnt = pArrDonateRegYN->GetSize();
	if(cnt == 0) return ;
	INT nTmpData;
	for(INT i=0 ; i<cnt ; i++)
	{
		nTmpData = pArrDonateRegYN->GetAt(i);
		if(nTmpData < 0) continue;
		if(nTmpData == nRow)
		{
			pArrDonateRegYN->RemoveAt(i);
		}
	}
}

INT CSpecies_Form::ApplyParentDM(CESL_DataMgr *pDM_IDX_BO, 
							   CESL_DataMgr * pDM_ACCU, 
							   CESL_DataMgr * pDM_BOOK, 
							   CESL_DataMgr * pDM_TARGET_BOOK, 
							   CESL_DataMgr * pDM_TARGET_VOL, 
							   CESL_DataMgr * pDM_TARGET_SPECIES,
							   INT nIdx)
{
	INT nTargetMatchingInfo = 0;
	INT nSourceMatchingInfo = 0;
	CString strTargetMatchingInfo[MAX_TEMP_BIG_STRING_ARRAY_SIZE];
	CString strSourceMatchingInfo[MAX_TEMP_BIG_STRING_ARRAY_SIZE];
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_저작자");			strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_저작자");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_형식구분_코드");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_형식구분_코드");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_발행자");			strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_발행자");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_발행년");			strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_발행년");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_본표제");			strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_본표제");			
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_이용대상구분");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_이용대상구분");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("IBS_종KEY");				strSourceMatchingInfo[nSourceMatchingInfo++] = _T("IBS_REC_KEY");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_입력일");				strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_입력일");				
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_가격");				strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_가격");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_자료상태_코드");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_자료상태_코드");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_등록구분_코드");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_자료상태_코드");
	strTargetMatchingInfo[nTargetMatchingInfo++] = _T("BB_매체구분_코드");		strSourceMatchingInfo[nSourceMatchingInfo++] = _T("BB_매체구분_코드");
	

	INT ids, nTargetRowIdx, nRepresentBookIdx;
	CString strHeader, strData;
	m_pFormApi->GetRepresentationBook(pDM_BOOK, nRepresentBookIdx);
	//종정보 입력/수정
	if(nIdx == -1)
	{
		pDM_TARGET_SPECIES->InsertRow(-1);
		nTargetRowIdx = pDM_TARGET_SPECIES->GetRowCount() - 1;
	}
	else 
		nTargetRowIdx = nIdx;
		
	for (INT i = 0; i < nTargetMatchingInfo; i++) {
		strData = _T("");
		strHeader = strTargetMatchingInfo[i].Left(2);
		if (strHeader == _T("IB")) ids = pDM_IDX_BO->GetCellData(strSourceMatchingInfo[i], 0, strData);
		if (strHeader == _T("BP")) ids = pDM_ACCU->GetCellData(strSourceMatchingInfo[i], 0, strData);
		if (strHeader == _T("BB")) ids = pDM_BOOK->GetCellData(strSourceMatchingInfo[i], nRepresentBookIdx, strData);
		if (ids >= 0) pDM_TARGET_SPECIES->SetCellData(strTargetMatchingInfo[i], strData, nTargetRowIdx);
	}

	// 권입력
	INT nVolCnt = m_pFormApi->GetVolumeCount(pDM_BOOK);

	if(nIdx == -1 && pDM_TARGET_VOL != NULL)
	{
		for(INT j = 0 ; j<nVolCnt ; j++)
		{
			pDM_TARGET_VOL->InsertRow(-1);
			nTargetRowIdx = pDM_TARGET_VOL->GetRowCount() - 1;
			for (i = 0; i < nTargetMatchingInfo; i++) {
				strData = _T("");
				strHeader = strTargetMatchingInfo[i].Left(2);
				if (strHeader == _T("IB")) ids = pDM_IDX_BO->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (strHeader == _T("BP")) ids = pDM_ACCU->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (strHeader == _T("BB")) ids = pDM_BOOK->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (ids >= 0) pDM_TARGET_VOL->SetCellData(strTargetMatchingInfo[i], strData, nTargetRowIdx);
			}
		}
	}

	//책입력
	INT nBookCnt = m_pFormApi->GetVolumeCount(pDM_BOOK);
	if(nIdx == -1 && pDM_TARGET_BOOK != NULL)
	{
		for(INT j = 0 ; j<nBookCnt ; j++)
		{
			pDM_TARGET_BOOK->InsertRow(-1);
			nTargetRowIdx = pDM_TARGET_BOOK->GetRowCount() - 1;
			for (INT i = 0; i < nTargetMatchingInfo; i++) {
				strData = _T("");
				strHeader = strTargetMatchingInfo[i].Left(2);
				if (strHeader == _T("IB")) ids = pDM_IDX_BO->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (strHeader == _T("BP")) ids = pDM_ACCU->GetCellData(strSourceMatchingInfo[i], 0, strData);
				if (strHeader == _T("BB")) ids = pDM_BOOK->GetCellData(strSourceMatchingInfo[i], i, strData);
				if (ids >= 0) pDM_TARGET_BOOK->SetCellData(strTargetMatchingInfo[i], strData, nTargetRowIdx);
			}
		}
	}	
	
	return 0;
}

VOID CSpecies_Form::OnbEXTRAISBN() 
{
	// TODO: Add your control notification handler code here
	ApplyData();
	INT nRepresentBookIdx = -1;
	m_pFormApi->GetRepresentationBook(m_pDM_Book, nRepresentBookIdx);
	if(nRepresentBookIdx < 0) return ;
	CString strAdditionISBN = m_pDM_Book->GetCellData(_T("BB_추가낱권ISBN"), nRepresentBookIdx);
	CString strAdditionAddCode = m_pDM_Book->GetCellData(_T("BB_추가낱권ISBN부가기호"), nRepresentBookIdx);
	CString strAdditionMissISBN = m_pDM_Book->GetCellData(_T("BB_추가낱권오류ISBN"), nRepresentBookIdx);

	CBOISBNAdditionDlg dlg(this);
	dlg.SetAdditionISBN(strAdditionISBN);
	dlg.SetAdditionAddCode(strAdditionAddCode);
	dlg.SetAdditionMissISBN(strAdditionMissISBN);

	if (dlg.DoModal() == IDOK)
	{
		INT rowCnt = m_pDM_Book->GetRowCount();
		CString strVolSortNo = m_pDM_Book->GetCellData(_T("BB_권일련번호"), nRepresentBookIdx);
		for(INT i=0 ; i<rowCnt ; i++)
		{
			if(strVolSortNo == m_pDM_Book->GetCellData(_T("BB_권일련번호"), i))
			{
				m_pDM_Book->SetCellData(_T("BB_추가낱권ISBN"), dlg.GetAdditionISBN(), i);
				m_pDM_Book->SetCellData(_T("BB_추가낱권ISBN부가기호"), dlg.GetAdditionAddCode(), i);
				m_pDM_Book->SetCellData(_T("BB_추가낱권오류ISBN"), dlg.GetAdditionMissISBN(), i);
			}
		}
	}	
}

VOID CSpecies_Form::OnbISBNCHECK() 
{
	// TODO: Add your control notification handler code here
	// 낱권 체크
	CString strEaISBNFrom;
	CString strEaISBNTo;
	INT ids = m_pCM_Index->GetControlMgrData(_T("IBS_낱권ISBN"), strEaISBNFrom);
	if (ids >= 0)
	{
		ids = m_pFormApi->CheckISBN(strEaISBNFrom, strEaISBNTo);
		if (ids >= 0)
			m_pCM_Index->SetControlMgrData(_T("IBS_낱권ISBN"), strEaISBNTo);
	}

	// 세트 체그
	CString strSetISBNFrom;
	CString strSetISBNTo;
	ids = m_pCM_Index->GetControlMgrData(_T("IBS_세트_ISBN1"), strSetISBNFrom);
	if (ids >= 0)
	{
		ids = m_pFormApi->CheckISBN(strSetISBNFrom, strSetISBNTo);
		if (ids >= 0)
			m_pCM_Index->SetControlMgrData(_T("IBS_세트_ISBN1"), strSetISBNTo);
	}	
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
//
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnbDEFAULTCONFIG() 
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT ids;
	CString strFormCode, strData, strPath, strMarc;
	CMarc marc;

	ids = ApplyData();
	if( ids < 0 )	return;

	// 종 -> 마크
	m_pCM_Index->GetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
	if( strFormCode.IsEmpty() == TRUE )
	{
		m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);
	}

	strData = strFormCode + _T(".cfg");
	strPath = m_strDefaultValueFilePath;
	strPath.Replace(_T(".cfg"), strData);

	
	ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu, 0);
	m_pInfo->pMarcMgr->CheckDeepSyntax(&marc);
	m_pInfo->pMarcMgr->CheckSimpleSyntax(&marc);
	m_pInfo->pMarcMgr->DeleteEmptySubfield(&marc);
	m_pInfo->pMarcMgr->DeleteEmptyField(&marc);
	
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	m_pFormApi->SaveDefaultValue(strMarc, strPath);
	/*
	if (SaveDefaultValue() < 0)
		AfxMessageBox(_T("기본값 저장 실패"));
	else
	*/
	
	AfxMessageBox(_T("기본값을 저장하였습니다."), MB_ICONINFORMATION);	

}

VOID CSpecies_Form::OnbDUPSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strSearchCount;

	if(m_bDupSearchType)
		SearchDuplicateData(_T("건별복본조사"), strSearchCount);
	else
		SearchDuplicateDataByTitle(_T("서명자동복본조사"), strSearchCount);

	if(strSearchCount.IsEmpty() || strSearchCount == _T("0"))
	{
		ESLAfxMessageBox(_T("서명복본조사 결과가 없습니다."));
		return;
	}
	CDuplicateSearch_Interface impl(this);
	
	impl.ShowTitleDuplicateSearchDlg(_T("서명자동복본조사"), m_strDupIndexKey);
	if (!m_strDupIndexKey.IsEmpty())
		MakeDupAddMode();	
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 전집처리 버튼
// [ 미니스펙 ]
//
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnbAPPLYSERIES() 
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	CString strTmpData;

	//=======================================================================
	// 1. 전집처리여부를 확인한다.
	//=======================================================================
	if( IDNO == MessageBox(_T("책을 전집으로 만듭니다. 계속하시겠습니까?"), NULL, MB_YESNO | MB_ICONQUESTION) ) return;

	// ApplyData를 수행하면 이전에 설정한 가격타입정보가 무조건 [종가격]이 된다.
	// 이러한 이유로 설정 정보가 올바르지 않다. 따라서 ApplyData하기전에 가격타입을 저장하고 
	// 수행한 이후에 이정보를 다시 설정한다.
	m_pTailDlg->SetApplyPriceInfo(FALSE);

	// ApplyData
	if( ApplyData() < 0 )
	{
		MessageBox(_T("전집여부적용 실패"));
		return;
	}

	// 모든 책을 1권 1책으로 만든다.
	m_pFormApi->ApplyCompleteWorks(m_pDM_Book, strTmpData);
	// 가격정보를 각 권에 적용
	m_pTailDlg->SetApplyPriceInfo(TRUE);
	m_pTailDlg->SetApplyVolPriceFromSpeciesPrice(FALSE);
	m_pTailDlg->ApplyData();
	// 색인과 화면에 적용
	m_pCM_Index->SetControlMgrData(_T("IBS_편권차"), _T(""));

	MessageBox(_T("책을 전집으로 만들었습니다."));

	Display();
	m_bViewVolInfo = TRUE;
}

VOID CSpecies_Form::OnbVOLINFO() 
{
	// 입력한 데이터 적용
	ShowVolumeInfo(VOLUME_FIRST);
}

INT CSpecies_Form::ApplyRegNoToMarc(INT idx /*= 0*/)
{
	INT ids = -1;
	CMarc marc;
	CString strMarc;
	
	ids = m_pDM_Species->GetCellData(_T("BS_MARC"), idx, strMarc);
	if (ids < 0) return ids;

	ids = m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	if (ids < 0) return ids;

	// 기존의 049Tag을 지운다.
	ids = m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	if (ids < 0) return ids;
	
	// 각각의 책의 등록번호가 있으면 등록번호를 049$l에 넣는다.
	CString strRegNo;
	INT nRowCount = m_pDM_Book->GetRowCount();
	for (INT i = 0; i < nRowCount; i++)
	{
		// 책이 아니면 넘어간다.
		if (!m_pFormApi->IsBook(m_pDM_Book, idx)) continue;

		// 등록번호가 없으면 넘어간다.
		strRegNo = m_pDM_Book->GetCellData(_T("BB_등록번호"), i);
		if (strRegNo.IsEmpty()) continue;

		// 등록번호를 049$l에 넣는다.
		ids = m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), strRegNo);
	}

	ids = m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	if (ids < 0) return ids;

	// 종 DM에 넣기
	ids = m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, idx);
	return ids;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 분류표구분 콤보 변경
//
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnSelchangecCLASIFYCLASS() 
{
	//=======================================================================
	// 변수정의
	//=======================================================================	
	INT nDefaultValue, i, nCode = 256;
	CString class_name, strWhichCode, strDefaultValue;
	CString strCode[256], strDesc[256];
	//바뀔때마다 분류 구분을 바꾸어 준다. 
	//if(m_nSpeciesType == BO_BASIC || m_nSpeciesType == BO_IMPORT || m_nSpeciesType == BO_REG_INVALID) return ;
	CESL_ControlMultiComboBox* pCM_WhichCode = (CESL_ControlMultiComboBox *)m_pCM_Index->FindControl(_T("IBS_분류표구분"));	
    CESL_ControlMultiComboBox* pCM_Subject = (CESL_ControlMultiComboBox *)m_pCM_Accu->FindControl(_T("BP_주제구분_코드"));
	if( pCM_WhichCode == NULL || pCM_Subject == NULL )	return;
	
  
	//=======================================================================	
	// 1. 분류표구분에서 선택되어진 텍스트를 읽어 클래스명을 정의한다.
	//=======================================================================	
	pCM_WhichCode->GetData(strWhichCode, pCM_WhichCode->GetCurSel(), 1);
	strWhichCode.TrimLeft();		strWhichCode.TrimRight();
	strWhichCode.MakeUpper();
	
	if		( strWhichCode == _T("KDC") )	class_name = _T("KDC대주제부호관리");
	else if	( strWhichCode == _T("DDC") )	class_name = _T("DDC대주제부호관리");
	else if	( strWhichCode == _T("LC")	)	class_name = _T("LC주제부호");
	else									class_name = _T("");

	//=======================================================================	
	// 2. 주제구분코드를 초기화 시킨다.
	//=======================================================================	
	pCM_Subject->FreeComboData();
	pCM_Subject->ResetContent();
	pCM_Subject->InitMultiComboBox(2);
	if( class_name.GetLength() == 0 )	return ;

	//=======================================================================	
	// 3. 정의된 클래스명으로 DB에서 코드값과 설명을 가져온다.(MAX : 256)
	//=======================================================================	
	nDefaultValue = 0;
	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if( nCode == 0 )	return ;

		
	//=======================================================================	
	// 4. 수서정보DM에서 주제구분코드를 가져온다.
	//=======================================================================	
	if( m_pDM_Accu != NULL )	
	{
		m_pDM_Accu->GetCellData(_T("BP_주제구분_코드"), 0, strDefaultValue);

		//=============================================================================================
		//2008.12.04 ADD BY LKS : 적용안함 코드값이 "  "로 나오지 않아 콤보박스에 설정안되는 현상 수정.
		strDefaultValue.Replace(_T(" "), _T(""));
		if(strDefaultValue.IsEmpty())
		{
			//===================================================
			//2009.10.19 UPDATE BY LKS : 반입받은 마크의 056태그 a다음 숫자를 읽어 주제구분을 Set
			CMarc marc;
			CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
			m_pInfo->pMarcMgr->Decoding(strMarc, &marc);		
			CString str056Field;
			m_pInfo->pMarcMgr->GetField(&marc, _T("056"), str056Field);
			
			if(!str056Field.IsEmpty())
			{
				INT idx = str056Field.FindOneOf(_T("a"));
				
				CString strSubjectCode,strTemp;
				strSubjectCode = str056Field.GetAt(idx+1);

				strDefaultValue = strSubjectCode;
			}
			else
			{
				strDefaultValue = _T("  ");
			}
			//===================================================

			//strDefaultValue = _T("  ");
		}
		//=============================================================================================
	}
		
	//=======================================================================	
	// 5. 주제구분콤보에 코드값들을 설정하고 코드값들과 DM의 코드값을 비교하여 콤보를 설정한다.
	//=======================================================================	
	for( i=0; i<nCode; i++ )
	{
		if( strCode[i].GetLength() == 0 )	strCode[i] = _T("  ");
		
		if( strDefaultValue == strCode[i] ) nDefaultValue = i;
	
		pCM_Subject->AddItem(strCode[i], 0, i);
		pCM_Subject->AddItem(strDesc[i], 1, i);
	}

	pCM_Subject->SetCurSel(nDefaultValue);

}

VOID CSpecies_Form::OnbCLOSE() 
{
	// TODO: Add your control notification handler code here
	// 2004.11.09. 5163 표지기능 추가.
	INT ids;
	if( m_strUseCoverYN == _T("Y"))
	{
		if((m_nOpType == SINSERT) && (m_bInsertData == FALSE) && (m_szSpeciesKey[0] != '\0'))
		{
			CCOVER_MAIN dlg(this);
			ids = dlg.DeleteCoverInfo( m_szSpeciesKey );
			if( ids < 0 ) 
			{
				ESLAfxMessageBox(_T("표지 삭제 실패"));
				return ;
			}
		}
	}
	CDialog::OnOK();
}

INT CSpecies_Form::CheckWorkNoInfo(CESL_DataMgr * pDM_OUT)
{
	INT ids = m_pHeadDlg->InsertWorkNoInfo(pDM_OUT);
	if(ids < 0) return -1;
	
	return 0;
}

VOID CSpecies_Form::OnSelchangecFORMCLASS() 
{
	// TODO: Add your control notification handler code here
	//형식구분을 적용안함을 선택하였으면 에러메시지 출력
	CString strFormCode;
	m_pCM_Index->GetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
	if(strFormCode.IsEmpty())
	{
		ESLAfxMessageBox(_T("형식구분은 적용안함을 선택할 수 없습니다."));
		CESL_ControlMultiComboBox * pCmb = (CESL_ControlMultiComboBox*)m_pCM_Index->FindControl(_T("IBS_형식구분_코드"));
		m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);
		m_pCM_Index->SetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
		if(m_nOpType == SINSERT) LoadDefaultValue();
		else if(m_nOpType == SUPDATE) DeleteFormCodeMarcInfo();
		if(pCmb) pCmb->SetFocus();
		return ;
	}
	if(m_nOpType == SINSERT) 
	{
		CString strMsg, strTmpData;
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("형식구분"), strFormCode, strTmpData);
		strMsg.Format(_T("형식구분[%s:%s]에 대한 기본값을 적용하시겠습니까?"), strFormCode, strTmpData);
		if(IDYES == ESLAfxMessageBox(strMsg, MB_YESNO))
			LoadDefaultValue();
	}
}

INT CSpecies_Form::ClearScreen()
{
	CString strWorkingStatus, strFormCode;
	//m_pCM_Index->GetControlMgrData(_T("자료구분"), strWorkingStatus);
	m_pCM_Index->GetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
	if (m_pCM_Index)
		m_pCM_Index->AllControlClear();
	if (m_pCM_Accu)
		m_pCM_Accu->AllControlClear();
	m_pDM_Species->SetCellData(_T("BS_MARC"), _T(""), 0);
	//m_pCM_Index->SetControlMgrData(_T("자료구분"), strWorkingStatus);
	m_pCM_Index->SetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
	GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(_T("EM"));
	GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_eVOL)->EnableWindow(TRUE);
	return 0;
}

VOID CSpecies_Form::SetParentGroupInfo(INT nAcqCode, CString strAcqYear, CString strGroupInfo, CString strCONNECTIONINFO)
{
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
	this->SetCONNECTION_INFO(strCONNECTIONINFO);
	m_pGroupCollection = new CGroupCollection(this);
	
	m_pGroupCollection->SetAcqType((ACQ_TYPE)nAcqCode);

	if((ACQ_TYPE)nAcqCode == PURCHASE)
		m_pGroupCollection->SetWorkingStatus(_T("검수완료"));
	m_bCreateGroupCollection = TRUE;
}

UINT CSpecies_Form::OnNoteClicked(LPARAM lParam, WPARAM wParam)
{
	// 복본 조사 들어간당
	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("서명자동복본조사"), m_strDupIndexKey);
	if( m_strDupIndexKey.IsEmpty() == FALSE )		MakeDupAddMode();

	if( m_pNoteCtrl != NULL )
	{
		delete m_pNoteCtrl;
		m_pNoteCtrl = NULL;
	}
	
	return 0;
}

VOID CSpecies_Form::MoveCtrlByAcqCode()
{	
	INT nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_STATIC_BOOK_REGCODE;
	nArrOtherResources[nCount++] = IDC_tREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_tTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_tTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_bAPPLY_SERIES;
	nArrOtherResources[nCount++] = IDC_bVOL_INFO;
	nArrOtherResources[nCount++] = IDC_eTOTAL_BOOKP;
	nArrOtherResources[nCount++] = IDC_eTOTAL_VOLUMEP;
	nArrOtherResources[nCount++] = IDC_eREG_CLASS_CODE;
	nArrOtherResources[nCount++] = IDC_bAPPENDIX;
	nArrOtherResources[nCount++] = IDC_bMARC_EDIT;
	nArrOtherResources[nCount++] = IDC_bMARC_IMPORT;
	nArrOtherResources[nCount++] = IDC_bKOLIS_NET;
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_bPREV_SPECIES;
	nArrOtherResources[nCount++] = IDC_bNEXT_SPECIES;
	nArrOtherResources[nCount++] = IDC_bSAVEAS;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	
	CRect rcResource;
	CWnd * pWnd = NULL;
	for (INT i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if(m_nAcqType == ACQ_TYPE_INVALID && m_bAcqTypeChange)
			{
				pWnd->GetWindowRect(rcResource);
				ScreenToClient(rcResource);
				rcResource.top -= 22;
				rcResource.bottom -= 22;
				pWnd->MoveWindow(rcResource);
			}
			else if(m_nAcqType != ACQ_TYPE_INVALID && m_bAcqTypeChange)
			{
				pWnd->GetWindowRect(rcResource);
				ScreenToClient(rcResource);
				rcResource.top += 22;
				rcResource.bottom += 22;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

/*
	// Resize Window Size
	GetWindowRect(rcResource);
	if(m_nAcqType == ACQ_TYPE_INVALID)
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - 22,  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() + 22,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
*/
}

INT CSpecies_Form::DeleteFormCodeMarcInfo()
{
	CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pInfo->pMarcMgr->DeleteField(&marc, _T("007"));
	m_pInfo->pMarcMgr->DeleteField(&marc, _T("008"));
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	m_pDM_Species->SetCellData(_T("BS_MARC"), strMarc, 0);
	return 0;
}

HBRUSH CSpecies_Form::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//======================================================
	//20080612 UPDATE BY PJW : 서명 등록구분 색지정부분 주석처리하면 안됨
	if( IsColorControl(pWnd->GetDlgCtrlID()) ) 
	{
		return SetColor(pDC);
	}
	//======================================================
	


	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 수입구분에 따라 구입신청자/신청일자를 활성화/비활성화 한다.
//
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::ShowControlByAcqType()
{
	
	// 보이기로 초기화
	GetDlgItem(IDC_stcBSFORM_REQUESTOR   )->ShowWindow(SW_SHOW);	// 구입신청자
	GetDlgItem(IDC_edtBSFORM_REQUESTOR	 )->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_stcBSFORM_REQUEST_DATE)->ShowWindow(SW_SHOW);	// 신청일자
	GetDlgItem(IDC_edtBSFORM_REQUEST_DATE)->ShowWindow(SW_SHOW);
	//20070809 ADD PJW : 신청일자에 시분초(IDC_edtBSFORM_REQUEST_HOUR) 추가
	GetDlgItem(IDC_edtBSFORM_REQUEST_HOUR)->ShowWindow(SW_SHOW);

	//=======================================================================
	// 5163/육사/국방대 플래그에 따라서 신청구분 콤보박스를 활성화시킨다.
	//=======================================================================	
	if( m_strIs64 == _T("Y") || m_strIs90 == _T("Y") )	
	{	
		GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->EnableWindow(TRUE);	// 신청코드(부서)
		GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->EnableWindow(TRUE);	// 신청코드(학과)
		GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->EnableWindow(TRUE);	// 신청코드(학처)
		GetDlgItem(IDC_cmbAPPLICANT				)->EnableWindow(TRUE);	// 신청구분
	}
	else
	{
		GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->EnableWindow(FALSE);	// 신청코드(부서)
		GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->EnableWindow(FALSE);	// 신청코드(학과)
		GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->EnableWindow(FALSE);	// 신청코드(학처)
		GetDlgItem(IDC_cmbAPPLICANT				)->EnableWindow(FALSE);	// 신청구분
	}
		
	//=======================================================================
	// 수입구분에 따라서 신청구분 콤보박스를 활성화시킨다.
	//=======================================================================
	if( m_nAcqType == ACQ_TYPE_INVALID )
	{
		((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUESTOR)	)->SetReadOnly(TRUE);
		((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUEST_DATE)	)->SetReadOnly(TRUE);
		//20070816 ADD PJW : 신청일자 ReadOnly<FALSE,TRUE 세팅>
		if ( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
		{
			m_hApplicantTime->Control_SetReadOnly(TRUE);
		}
		else
		{
			m_hApplicantTime->Control_SetReadOnly(FALSE);
		}


		((CComboBox*)GetDlgItem(IDC_cmbAPPLICANT)		)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_btnSearchUser))->EnableWindow(FALSE);	// 2005-01-11 대출자조회 버튼도 비활성화 시킨다.
		GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->EnableWindow(FALSE);	// 신청코드(부서)
		GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->EnableWindow(FALSE);	// 신청코드(학과)
		GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->EnableWindow(FALSE);	// 신청코드(학처)
		GetDlgItem(IDC_cmbAPPLICANT				)->EnableWindow(FALSE);	// 신청구분
	}
	else if( m_nAcqType == DONATE && m_strIs90 == _T("Y") )
	{
		((CComboBox*)GetDlgItem(IDC_cmbAPPLICANT)		)->EnableWindow(FALSE);
		GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->EnableWindow(FALSE);	// 신청코드(부서)
		GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->EnableWindow(FALSE);	// 신청코드(학과)
		GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->EnableWindow(FALSE);	// 신청코드(학처)
		GetDlgItem(IDC_cmbAPPLICANT				)->EnableWindow(FALSE);	// 신청구분
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUESTOR)	)->SetReadOnly(FALSE);
		((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUEST_DATE)	)->SetReadOnly(FALSE);
		//20070816 ADD PJW : 신청일자 ReadOnly<FALSE,TRUE 세팅>
		if ( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
		{
			m_hApplicantTime->Control_SetReadOnly(TRUE);
		}
		else
		{
			m_hApplicantTime->Control_SetReadOnly(FALSE);
		}

		((CButton*)GetDlgItem(IDC_btnSearchUser)		)->EnableWindow(TRUE);
	}

}

VOID CSpecies_Form::SetFurnishInfo()
{
	CString strInputType = m_pDM_Accu->GetCellData(_T("BP_입력구분"), 0);
	CString strAcqKey = m_pDM_Accu->GetCellData(_T("BP_수서정보KEY"), 0);
	if(strAcqKey.IsEmpty()) return ;
	
	//ShowControlByAcqType();

	// 2005-02-16
	//
	CESL_DataMgr tmpDM;
	tmpDM.SetCONNECTION_INFO(m_pDM_Accu->CONNECTION_INFO);
	
	CString strQuery = _T("");	

	// 2006-01-16 강봉수
	// 신청일은 FURNISH_DATE -> APPLICANT_DATE	
	//strQuery.Format(_T("SELECT APPLICANT_NAME, APPLICANT_DATE FROM BO_FURNISH_TBL WHERE ACQ_KEY=%s"), strAcqKey);
	strQuery.Format(_T("SELECT APPLICANT_NAME, TO_CHAR(APPLICANT_DATE, 'YYYY/MM/DD HH24:MI:SS') FROM BO_FURNISH_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACQ_KEY=%s"), strAcqKey);
//DEL 	strQuery.Format(_T("SELECT APPLICANT_NAME, TO_CHAR(APPLICANT_DATE, 'YYYY/MM/DD HH24:MI:SS') FROM BO_FURNISH_TBL WHERE ACQ_KEY=%s"), strAcqKey);
	tmpDM.RestructDataManager(strQuery);

	if (tmpDM.GetRowCount() == 0) return ;
	CString strApplicantName = tmpDM.GetCellData(0, 0);
	CString strApplicantDate = tmpDM.GetCellData(0, 1);

	m_pCM_Index->SetControlMgrData(_T("구입신청자"), strApplicantName);
	//m_pCM_Index->SetControlMgrData(_T("구입신청일"), strApplicantDate);
	strApplicantTime(strApplicantDate);
	m_pDM_Index->SetCellData(_T("구입신청자"), strApplicantName, 0);
	m_pDM_Index->SetCellData(_T("구입신청일"), strApplicantDate, 0);
}


VOID CSpecies_Form::SetPurchaseInfo()
{
	// 육사 요구사항 - 신청구분에 회원, 부서, 학과, 학부가 추가되어야 함.
	// APPLICANT_CLASS에 따라 값 적용 - NULL(적용안함)에 대한 처리
	// 2005-09-29 강봉수
	//
	CString strApply = _T("");
	CString strAppKey = _T("");
	strAppKey = m_pDM_Accu->GetCellData(_T("BP_수서정보KEY"), 0);
	strApply = m_pDM_Accu->GetCellData(_T("BP_신청구분"), 0);
	
	//구입 신청 정보를 가지고 combo보여주기..
	//
	if ( strApply == _T("DEP") )
	{
		strApply = m_pDM_Accu->GetCellData(_T("BP_학부코드"), 0 );
		m_cmbApplicant.SetCurSel(3);
		m_pCM_Index->SetControlMgrData(_T("구입신청구분_학부"), m_pDM_Accu->GetCellData(_T("BP_학부코드"), 0 ) );
		//m_pDM_Index->SetCellData(_T("구입신청구분_학부"), m_pDM_Accu->GetCellData(_T("BP_학부코드"), 0 ) , 0);
	}
	else if ( strApply == _T("COU") )
	{
		m_cmbApplicant.SetCurSel(2);
		m_pCM_Index->SetControlMgrData(_T("구입신청구분_학과"), m_pDM_Accu->GetCellData(_T("BP_학과코드"), 0 ) );
		//m_pDM_Index->SetCellData(_T("구입신청구분_학과"), m_pDM_Accu->GetCellData(_T("BP_학과코드"), 0 ) , 0);
	}
	else if ( strApply == _T("CLA") )
	{
		m_cmbApplicant.SetCurSel(1);
		m_pCM_Index->SetControlMgrData(_T("구입신청구분_부서"), m_pDM_Accu->GetCellData(_T("BP_부서코드"), 0 ) );
		//m_pDM_Index->SetCellData(_T("구입신청구분_부서"), m_pDM_Accu->GetCellData(_T("BP_부서코드"), 0 ) , 0);
	}
	else
	{
		if( m_strIs64 == _T("Y") && strAppKey.IsEmpty() )
			m_cmbApplicant.SetCurSel(3);
		else
			m_cmbApplicant.SetCurSel(0);
	}

	OnSelchangecmbAPPLICANT();
}

VOID CSpecies_Form::OnSelchangecUSEOBJECTCLASS() 
{
	// TODO: Add your control notification handler code here
	CString strUseObjCode, strMarc;
	CMarc marc;
	m_pCM_Index->GetControlMgrData(_T("IBS_이용대상구분_코드"), strUseObjCode);
	if(strUseObjCode == _T("JU"))
	{
		strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	}
}

INT CSpecies_Form::InsertAbstractContentsInfo()
{
    CBO_INERT_TABLE api(this);
    
    CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);//MARC
    CString strSpeciesKey = m_pDM_Species->GetCellData(_T("BS_종KEY"), 0);

    api.GetMarcSteam (strMarc) ;
    return api.SetInsertValue(strSpeciesKey, _T(""), m_strContents, m_strAbstracts);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 이전내용 버튼
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::OnbtnPREVCONTENT() 
{
	if(!m_bInsertData) return ;

	// 2006.04.20 KBS - (공공요구)미리 기입된 내용이 있다면 지워지지 않도록 주석처리
	//OnbCLEARSCREEN();

	//이전 사용자 입력 서지정보를 가져온다.
	BOOL bUserUserInputMarc = FALSE;
	INT ids = ApplyData();
	if(ids < 0)	bUserUserInputMarc = FALSE;
	else bUserUserInputMarc = TRUE;

	if(bUserUserInputMarc)
	{
		ids = ApplyIndexToMarc();
		if(ids < 0) bUserUserInputMarc = FALSE;
		else bUserUserInputMarc = TRUE;
	}
	
	CMarc UserInputMarc, PrevMarc;
	CString strMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), 0);
	if(!strMarc.IsEmpty() && bUserUserInputMarc)
	{
		ids = m_pInfo->pMarcMgr->Decoding(strMarc, &UserInputMarc);
		if(ids < 0) bUserUserInputMarc = FALSE;
		else bUserUserInputMarc = TRUE;
	}

	m_nParentSelectedRow = m_pDM_Parent->GetRowCount() - 1;
	UpdateInitProc();
	ids = MakeUpdateDM();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("이전 작업내용을 불러올수 없습니다."));
		return ;
	}
	if(m_bDupAddYN) m_bDupAddYN = FALSE;
	
	if(m_bAcqTypeChange) SetHeadTailInfoDlg();
	else m_pHeadDlg->Init();

	ApplyMarcToIndexCM();
	m_pTailDlg->Init();
	CString strSubjectCode, strWorkCode;
	m_pDM_Accu->GetCellData(_T("BP_주제구분_코드"), 0, strSubjectCode);

	//====================================================
	//2008.07.08 ADD BY PJW : 주제구분이 적용안함일경우 블랭크를 넣어준다.
	//m_pCM_Accu->SetControlMgrData(_T("BP_주제구분_코드"), strSubjectCode);
	if( strSubjectCode.IsEmpty() )
	{
		m_pCM_Accu->SetControlMgrData(_T("BP_주제구분_코드"), _T("  "));
	}
	else
	{
		m_pCM_Accu->SetControlMgrData(_T("BP_주제구분_코드"), strSubjectCode);
	}
	//====================================================

	//차수업무구분 설정
	m_pCM_Index->GetControlMgrData(_T("IBS_업무구분_코드"), strWorkCode);
	m_pDM_Accu->SetCellData(_T("BP_차수업무구분"), strWorkCode, 0);
	//검색된 책 중에서 대표책을 제외하고 다 제거한다.
	INT nRowCnt = m_pDM_Book->GetRowCount();
	if(nRowCnt > 1)
	{
		for(INT i=nRowCnt-1 ; i>=0 ; i--)
		{
			if(m_pDM_Book->GetCellData(_T("BB_대표책여부"), i) != _T("Y"))
				m_pDM_Book->DeleteRow(i);
			if(m_pDM_Book->GetRowCount() == 1) break;
		}
		m_pDM_Book->SetCellData(_T("BB_대표책여부"), _T("Y"), 0);
	}

	((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUESTOR))->SetReadOnly(FALSE);
	((CEdit*)GetDlgItem(IDC_edtBSFORM_REQUEST_DATE))->SetReadOnly(FALSE);
	//20070816 ADD PJW : 신청일자 ReadOnly<FALSE,TRUE 세팅>
	if ( m_bDupAddMode == TRUE || m_bDupAddYN == TRUE )
	{
		m_hApplicantTime->Control_SetReadOnly(TRUE);
	}
	else
	{
		m_hApplicantTime->Control_SetReadOnly(FALSE);
	}

	

	if( bUserUserInputMarc == TRUE )
	{
		ids = m_pBOSpeciesApi->APPLY_CMarc_to_Species_NotNullData(&UserInputMarc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu);		
		if( ids < 0 )
		{
			ESLAfxMessageBox(_T("사용자 입력정보를 적용할 수 없습니다."));
		}
	}
	
	// 2005-01-06 m_szSpeciesKey 초기화
	m_szSpeciesKey[0] = '\0';

	Display();
	SetEBookCtrl();
}

VOID CSpecies_Form::OnSelchangecWORKCODE() 
{
	// TODO: Add your control notification handler code here
	if(m_nAcqType != PURCHASE ) return;

	CString strWorkCode;
	m_pCM_Index->GetControlMgrData(_T("IBS_업무구분_코드"), strWorkCode);
	INT nWorkCode = _ttoi(strWorkCode.GetBuffer(0));
	::PostMessage(m_pHeadDlg->GetSafeHwnd(), CHANGE_WORK_CODE, nWorkCode, 0);
}

INT CSpecies_Form::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	INT ids = m_pGrid_Parent->SelectMakeList();
	ids = m_pGrid_Parent->SelectGetHeadPosition();
	if(ids < 0) m_bGridSelectYN = FALSE;
	else m_bGridSelectYN = TRUE;

	if(m_bGridSelectYN)
	{
		m_nLastIdx = m_pGrid_Parent->SelectGetTailPosition();
		m_nFirstIdx = m_pGrid_Parent->SelectGetHeadPosition();
	}
	else
	{	
		m_nFirstIdx = 0;
		m_nLastIdx = m_pDM_Parent->GetRowCount() - 1;
	}
	
	//===================================================
	//2009.05.21 UPDATE BY LKS : 수정버튼을 눌렀을 때와 그리드 더블클릭했을 때의 동작 통일
	
	if( m_nOpType == GUPDATE || m_nOpType == SUPDATE )
	{
		nFirstSelIdx = m_pGrid_Parent->GetRow() - 1;
	}
//DEL	if( m_nOpType == GUPDATE )
//DEL	{
//DEL		nFirstSelIdx = m_pGrid_Parent->GetRow() - 1;
//DEL	}
//DEL	else if(m_nOpType == SUPDATE)
//DEL	{
//DEL		if(m_bGridSelectYN)
//DEL			nFirstSelIdx = m_pGrid_Parent->SelectGetHeadPosition();
//DEL		else
//DEL			nFirstSelIdx = m_pGrid_Parent->GetRow() - 1;
//DEL	}
	//===================================================
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;

	if(nFirstSelIdx ==  m_nFirstIdx)
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(TRUE);
	if(nFirstSelIdx == m_nLastIdx)
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(TRUE);
	

	return nFirstSelIdx;
}

INT CSpecies_Form::GetPrevSelectIndex()
{
	//===============================
	//2009.04.08 UPDATE BY PJW : 비교를 위해서 초기값을 0으로 셋팅한다.
// 	INT nPrevSelIndex = -1;
 	INT nPrevSelIndex = 0;
	//===============================
	

	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : 여러개를 선정후 이전,다음 할경우 인덱스가 맞지 않음
		while( 1 )
		{
			if(nPrevSelIndex == m_nParentSelectedRow || nPrevSelIndex < 0 )
			{
				break;
			}
			else if( nPrevSelIndex < m_nParentSelectedRow )
			{
				nPrevSelIndex = m_pGrid_Parent->SelectGetNext();
				if( nPrevSelIndex < 0 )
				{
					nPrevSelIndex = m_pGrid_Parent->SelectGetTailPosition();
					break;
				}
			}
			else if( nPrevSelIndex > m_nParentSelectedRow )
			{
				nPrevSelIndex = m_pGrid_Parent->SelectGetPrev();
			}
		}
		//================================================

		if(m_nFirstIdx == m_pGrid_Parent->SelectGetIdx())
			return m_pGrid_Parent->SelectGetIdx();
		
		nPrevSelIndex = m_pGrid_Parent->SelectGetPrev();
	}
	else
	{
		if(m_nFirstIdx == m_nParentSelectedRow)
			return m_nParentSelectedRow;

		nPrevSelIndex = m_nParentSelectedRow - 1;
	}
	
	if(nPrevSelIndex ==  m_nFirstIdx)
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(TRUE);
	if(nPrevSelIndex == m_nLastIdx)
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(TRUE);
	
	return nPrevSelIndex;
}

INT CSpecies_Form::GetNextSelectIndex()
{
	//===============================
	//2009.04.08 ADD BY PJW : 비교를 위해서 초기값을 0으로 셋팅한다.
// 	INT nNextSelIndex = -1;
 	INT nNextSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : 여러개를 선정후 이전,다음 할경우 인덱스가 맞지 않음
		while( 1 )
		{
			if(nNextSelIndex == m_nParentSelectedRow || nNextSelIndex < 0 )
			{
				break;
			}
			else if( nNextSelIndex < m_nParentSelectedRow )
			{
				nNextSelIndex = m_pGrid_Parent->SelectGetNext();
			}
			else if( nNextSelIndex > m_nParentSelectedRow )
			{
				nNextSelIndex = m_pGrid_Parent->SelectGetPrev();
			}
		}
		//================================================

		if(m_nLastIdx == m_pGrid_Parent->SelectGetIdx())
		{
			return m_pGrid_Parent->SelectGetIdx();
		}

		nNextSelIndex = m_pGrid_Parent->SelectGetNext();
	}
	else
	{
		if(m_nLastIdx == m_nParentSelectedRow) 
			return m_nParentSelectedRow;
		
		nNextSelIndex = m_nParentSelectedRow + 1;
	}
	if(nNextSelIndex ==  m_nFirstIdx)
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bPREV_SPECIES)->EnableWindow(TRUE);
	if(nNextSelIndex == m_nLastIdx)
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bNEXT_SPECIES)->EnableWindow(TRUE);
	
	return nNextSelIndex;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 등록구분 에디트 박스에 기본설정값으로 세팅한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::InitRegCodeField()
{
	//=======================================================================
	// 1. 등록구분을 기본설정값으로 설정한다.
	//=======================================================================
	if( m_strRegCodeField.IsEmpty() == TRUE )
	{
		// DB에서 기본값이 설정되어 있는지 판단하여 없다면 EM을 기본으로 한다.
		m_pInfo->pCodeMgr->GetDefaultCode(_T("등록구분"), m_strRegCodeField);
		if( m_strRegCodeField.IsEmpty() == TRUE )	m_strRegCodeField = _T("EM");
	}
	else
	{
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		m_strRegCodeField = m_strRegCodeField.Left(2);
		INT nRegCodeLength;
		nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
		m_strRegCodeField = m_strRegCodeField.Left(nRegCodeLength);
		//===================================================================
	}
	
	GetDlgItem(IDC_eREG_CLASS_CODE)->SetWindowText(m_strRegCodeField);
	GetDlgItem(IDC_eREG_CLASS_CODE)->EnableWindow(TRUE);
	GetDlgItem(IDC_eVOL)->EnableWindow(TRUE);

	SetEBookCtrl(TRUE);
}

VOID CSpecies_Form::ShowCodeResourceByMediaCode(CString strMediaCode, BOOL bInit/*=FALSE*/)
{
EFS_BEGIN
	INT i = 0;
	INT nCodeResourcesHeight = 65;
	CWnd *pWnd = NULL;

	if(strMediaCode == _T("EB")) m_bShowEBookInfo = TRUE;
	else m_bShowEBookInfo = FALSE;

	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_bShowEBookInfo && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_STATIC_EBOOK_GROUP;
	nArrCodeResourceID[nCount++] = IDC_stcEBOOK_COVER_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtEBOOK_COVER_FILE;
	nArrCodeResourceID[nCount++] = IDC_btnFIND_FILE1;
	nArrCodeResourceID[nCount++] = IDC_btnFIND_FILE2;
	nArrCodeResourceID[nCount++] = IDC_btnFIND_FILE3;
	nArrCodeResourceID[nCount++] = IDC_stcEBOOK_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtEBOOK_FILE;
	nArrCodeResourceID[nCount++] = IDC_stcEBOOK_EXECUTE_FILE;
	nArrCodeResourceID[nCount++] = IDC_edtEBOOK_EXECUTE_FILE;
	nArrCodeResourceID[nCount++] = IDC_btnEBOOK_VIEW;

	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_bShowEBookInfo)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Move Other Resources
	nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_bMARC_EDIT;
	nArrOtherResources[nCount++] = IDC_bMARC_IMPORT;
	nArrOtherResources[nCount++] = IDC_bKOLIS_NET;
	nArrOtherResources[nCount++] = IDC_bAPPENDIX;
	nArrOtherResources[nCount++] = IDC_bCLEARSCREEN;
	nArrOtherResources[nCount++] = IDC_bPREV_SPECIES;
	nArrOtherResources[nCount++] = IDC_bNEXT_SPECIES;
	nArrOtherResources[nCount++] = IDC_btnPREV_CONTENT;
	nArrOtherResources[nCount++] = IDC_bSAVEAS;
	nArrOtherResources[nCount++] = IDC_bSAVE;
	nArrOtherResources[nCount++] = IDC_bCLOSE;
	
	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if (m_bShowEBookInfo)
			{
				if(!bInit)
				{
					rcResource.top += nCodeResourcesHeight;
					rcResource.bottom += nCodeResourcesHeight;
					pWnd->MoveWindow(rcResource);
				}
			}
			else
			{
				rcResource.top -= nCodeResourcesHeight;
				rcResource.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

	// Resize Window Size
	GetWindowRect(rcResource);
	if (m_bShowEBookInfo)
	{		
		if(m_bShowCode)
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height()-97,  SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		if(m_bShowCode)
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - (nCodeResourcesHeight),  SWP_NOMOVE | SWP_NOZORDER);
		else
			SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - (nCodeResourcesHeight+97),  SWP_NOMOVE | SWP_NOZORDER);
	}
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CSpecies_Form::OnSelchangecMEDIACLASS() 
{
	// TODO: Add your control notification handler code here
	SetEBookCtrl();
}

VOID CSpecies_Form::OnbtnFINDFILE1() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_COVER, m_pCM_Index);
}

VOID CSpecies_Form::OnbtnFINDFILE3() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_EXECUTER, m_pCM_Index);
}

VOID CSpecies_Form::OnbtnFINDFILE2() 
{
	// TODO: Add your control notification handler code here
	m_pFormApi->EBookFileOpen(this, EBOOK_FILE, m_pCM_Index);
}

VOID CSpecies_Form::OnbtnEBOOKVIEW() 
{
	// TODO: Add your control notification handler code here
	CString strExeFile, strEBookFile, strBookKey;
	INT nVolCnt = m_pFormApi->GetVolumeCount(m_pDM_Book);
	if(nVolCnt == 1)
	{
		m_pCM_Index->GetControlMgrData(_T("IBS_원본파일"), strEBookFile);
		m_pCM_Index->GetControlMgrData(_T("IBS_실행파일"), strExeFile);
		INT nIdx;
		m_pFormApi->GetRepresentationBook(m_pDM_Book, nIdx);
		if(nIdx < 0) nIdx = 0;
		strBookKey = m_pDM_Book->GetCellData(_T("BB_책KEY"), nIdx);
		m_pFormApi->EBookView(strBookKey, strExeFile, strEBookFile);
	}
	else
	{
		ShowVolumeInfo(BOOK_FIRST);
	}
}

VOID CSpecies_Form::SetEBookCtrl(BOOL bEnable)
{
	GetDlgItem(IDC_btnFIND_FILE1)->EnableWindow(bEnable);
	GetDlgItem(IDC_btnFIND_FILE2)->EnableWindow(bEnable);
	GetDlgItem(IDC_btnFIND_FILE3)->EnableWindow(bEnable);

	((CEdit*)GetDlgItem(IDC_edtEBOOK_EXECUTE_FILE))->SetReadOnly(!bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 권별정보에 데이터를 적용한다.
// [ 미니스펙 ] - 생략
/////////////////////////////////////////////////////////////////////////////
VOID CSpecies_Form::ShowVolumeInfo(INT nTabOrder)
{
	INT openMode;
	CBOVolumeDlg dlg(this, m_pDM_Book);

	ApplyData();

	// 권 화면 띄우기
	if( m_nOpType == SINSERT )		openMode = 1100;
	else							openMode = -1100;
	dlg.SetOpenMode(openMode);

	if( m_bDupAddMode == TRUE )
	{
		dlg.SetDupIdxKey(m_strDupIndexKey);
	}
	dlg.SetTabOrder(nTabOrder);
	dlg.DoModal();

	Display();
	m_bViewVolInfo = TRUE;	
	SetEBookCtrl();
}

INT CSpecies_Form::InitEBookCtrl()
{
	// E-Book정보 Ctrl제어
	CString strMediaCode;

	m_pCM_Index->GetControlMgrData(_T("IBS_매체구분_코드"), m_strPrevMediaCode);
	if( m_strPrevMediaCode == _T("EB") )	m_bShowEBookInfo = TRUE;
	else m_bShowEBookInfo = FALSE;
	
	// 코드 구분값 콘트롤 보여주기
	ShowCodeResource();
	ShowCodeResourceByMediaCode(m_strPrevMediaCode, TRUE);

	return 0;
}

INT CSpecies_Form::SetEBookCtrl()
{
	CString strMediaCode;
	m_pCM_Index->GetControlMgrData(_T("IBS_매체구분_코드"), strMediaCode);
	if(m_strPrevMediaCode != strMediaCode && (strMediaCode == _T("EB") || m_strPrevMediaCode == _T("EB")))
		ShowCodeResourceByMediaCode(strMediaCode);
	m_strPrevMediaCode = strMediaCode;
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 부록DM을 구성한다.
// [ 미니스펙 ]
// 1. 부록DM이 없다면 에러리턴한다.
// 2. CM의값을 부록DM에 저장한다.
//
/////////////////////////////////////////////////////////////////////////////
INT CSpecies_Form::ApplyAppendixDM()
{
	//=======================================================================
	// 변수정의
	//=======================================================================
	INT nRowCnt, i;
	CStringArray ArrIgnoreList;

	//=======================================================================
	// 1. 부록DM이 없다면 에러리턴한다.
	//=======================================================================
	if( m_pDM_Appendix == NULL ) return -1;

	//=======================================================================
	// 2. CM의값을 부록DM에 저장한다.
	//=======================================================================
	nRowCnt = m_pDM_Appendix->GetRowCount();
	if( nRowCnt == 0 )		return 0;	
	
	ArrIgnoreList.Add(_T("IBS_관리구분_코드"));
	ArrIgnoreList.Add(_T("IBS_매체구분_코드"));
	ArrIgnoreList.Add(_T("IBS_별치기호_코드"));


	ArrIgnoreList.Add(_T("IBS_편권차"));
	ArrIgnoreList.Add(_T("IBS_저작자"));
	ArrIgnoreList.Add(_T("IBS_발행년"));	
	
	for( i=0 ; i<nRowCnt ; i++ )
	{
		m_pFormApi->MakeDMFromCM(m_pCM_Index, m_pDM_Appendix, i, _T("BB"), &ArrIgnoreList);
	}	
	
	return 0;
}

INT CSpecies_Form::ApplyRepresentBookDataToIndex(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_INDEX, CESL_ControlMgr * pCM_INDEX)
{
	//다권인지 파악한다.
	INT nVolCnt = m_pFormApi->GetVolumeCount(pDM_BOOK);
	INT nIdx = -1;
	if(nVolCnt == 1)
	{
		// 대표책을 찾는다.
		m_pFormApi->GetRepresentationBook(pDM_BOOK, nIdx);
		if(nIdx < 0) nIdx = 0;
		// 대표책을 색인에 넣는다.
		m_pFormApi->CopyDM(pDM_BOOK, nIdx, pDM_INDEX, 0, _T("IBS"));
	}
	else
	{
		// 2007.08.13 ADD BY PDJ
		// 다권본이라도 대표책의 정보를 종과 일치시킨다.
		// 대표책을 찾는다.
		m_pFormApi->GetRepresentationBook(pDM_BOOK, nIdx);		

		// 책의 코드 정보를 색인의 코드 정보에 반영 하여야 한다.		
		CStringArray ArrAccept;
		ArrAccept.Add(_T("BB_관리구분_코드"));
		ArrAccept.Add(_T("BB_매체구분_코드"));
		ArrAccept.Add(_T("BB_별치기호_코드"));
		ArrAccept.Add(_T("BB_이용제한구분_코드"));
		ArrAccept.Add(_T("BB_이용대상구분_코드"));		
		m_pFormApi->CopyDMInArray(pDM_BOOK, nIdx, pDM_INDEX, 0, &ArrAccept, _T("IBS"));
	}
	pCM_INDEX->AllControlDisplay(0);

	m_pFormApi->DisplayISBN(pCM_INDEX, _T("IBS_낱권ISBN"));
	m_pFormApi->DisplayISBN(pCM_INDEX, _T("IBS_세트_ISBN1"));
	
	return 0;
}

INT CSpecies_Form::ApplyRepresentBookDataToSpecies(CESL_DataMgr *pDM_BOOK, CESL_DataMgr *pDM_SPECIES)
{
	//다권인지 파악한다.
	INT nVolCnt = m_pFormApi->GetVolumeCount(pDM_BOOK);
	if(nVolCnt > 1) return 0;
	
	// 대표책을 찾는다.
	INT nIdx = -1;
	m_pFormApi->GetRepresentationBook(pDM_BOOK, nIdx);
	if(nIdx < 0) nIdx = 0;
	// 대표책을 종에 넣는다.
	m_pFormApi->CopyDM(pDM_BOOK, nIdx, pDM_SPECIES, 0, _T("BS"));
	
	return 0;
}

INT CSpecies_Form::ApplyUserInputMarc(CMarc *pUserInputMarc, CMarc *pPrevMarc)
{
	//이전내용이 적용될 마크Tag
	//008, 020, 100, 245, 250, 260, 300, 440, 490, 700, 950
	if(pUserInputMarc == NULL || pPrevMarc == NULL) return -1;
	CStringArray ArrTag;
	ArrTag.Add(_T("008"));	ArrTag.Add(_T("020"));	ArrTag.Add(_T("100"));	ArrTag.Add(_T("245"));
	ArrTag.Add(_T("250"));	ArrTag.Add(_T("260"));	ArrTag.Add(_T("300"));	ArrTag.Add(_T("440"));
	ArrTag.Add(_T("490"));	ArrTag.Add(_T("700"));	ArrTag.Add(_T("950"));
	CArray<CString, CString&> ArrData;	ArrData.RemoveAll();
	CString strTmpData, strTmpValue;
	INT ids;
	TCHAR cFirstInd, cSecondInd;
	for(INT i=0 ; i<ArrTag.GetSize() ; i++)
	{
		ArrData.RemoveAll();
		ids = m_pInfo->pMarcMgr->GetField(pUserInputMarc, ArrTag.GetAt(i), strTmpData, &ArrData);
		if(ids < 0) continue;
		if(ArrData.GetSize() == 0) continue;
		for(INT j=0 ; j<ArrData.GetSize() ; j++)
		{
			strTmpData.Empty();
			strTmpData = ArrData.GetAt(j);
			if(strTmpData.IsEmpty()) continue;
			cFirstInd = strTmpData.GetAt(0);
			cSecondInd = strTmpData.GetAt(1);
			ids = m_pInfo->pMarcMgr->DeleteField(pPrevMarc, ArrTag.GetAt(i));//, cFirstInd, cSecondInd);
			if(ids < 0) continue;
			m_pInfo->pMarcMgr->GetField(pPrevMarc, ArrTag.GetAt(i), strTmpValue);
			strTmpData = ArrTag.GetAt(i) + strTmpData;
			ids = m_pInfo->pMarcMgr->InsertField(pPrevMarc, strTmpData);
			if(ids < 0) continue;
			m_pInfo->pMarcMgr->GetField(pPrevMarc, ArrTag.GetAt(i), strTmpData);
		}
	}
	
	return 0;
}

BOOL CSpecies_Form::IsHoldDataMarc(CESL_DataMgr *pDM_BOOK)
{
	INT nCnt = pDM_BOOK->GetRowCount();
	for(INT i=0 ; i<nCnt ; i++)
	{
		if(pDM_BOOK->GetCellData(_T("BB_자료상태"), i).Left(3) == _T("BOL"))
			return FALSE;
	}
	return FALSE;
}

VOID CSpecies_Form::SearchDuplicateData(CString strSearchType, CString &strSearchCount)
{
	INT ids = ApplyData();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("입력하신 내용을 반영할 수 없습니다."));
		return ;
	}
	CMarc marc;
	ids = m_pBOSpeciesApi->APPLY_Species_to_CMarc(&marc, m_pDM_Index, m_pDM_Species, m_pDM_Book, m_pDM_Accu);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("입력하신 내용으로 마크정보를 생성할 수 없습니다."));
		return ;
	}
	CString strVolInfo, strVolTitleInfo, strShelfCodeInfo, strRegCodeInfo, strMediaCodeInfo, strMarc;
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	m_pDM_Index->GetCellData(_T("IBS_매체구분_코드"), 0, strMediaCodeInfo);
	if(m_pDM_Book->GetRowCount() > 0)
	{
		m_pBOSpeciesApi->GetBoBookInfo(m_pDM_Book, _T("BB_편권차"), strVolInfo);
		m_pBOSpeciesApi->GetBoBookInfo(m_pDM_Book, _T("BB_편제명부록명"), strVolTitleInfo);
		m_pBOSpeciesApi->GetBoBookInfo(m_pDM_Book, _T("BB_별치기호_코드"), strShelfCodeInfo);
		m_pBOSpeciesApi->GetBoBookInfo(m_pDM_Book, _T("BB_등록구분_코드"), strRegCodeInfo);
	}
	// 수정모드일때 자신의 종키를 가져온다.
	CString strSpeciesKey;
	if (m_nOpType != SINSERT && m_nParentSelectedRow >= 0)
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, m_nParentSelectedRow, strSpeciesKey);

	CDuplicateSearch_Interface impl(this);
	ids = impl.SearchDuplicateDataByIndividually(strSearchType, strMarc, strMediaCodeInfo, strVolInfo, strVolTitleInfo, strShelfCodeInfo, strRegCodeInfo, strSpeciesKey, strSearchCount);
	if (ids < 0) return;	
}

VOID CSpecies_Form::OnKillfocusedtAPPENDIX() 
{
	// TODO: Add your control notification handler code here
	m_pTailDlg->PostMessage(SET_TAB_ORDER, 0, 0);
}

UINT CSpecies_Form::OnSetTabOrder(LPARAM lParam, WPARAM wParam)
{
	if(wParam == 1)
		GetDlgItem(IDC_eREG_CLASS_CODE)->SetFocus();
	else
		GetDlgItem(IDC_edtWORKING_STATUS)->SetFocus();
	return 0;
}

BOOL CSpecies_Form::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_TAB && pMsg->hwnd == GetDlgItem(IDC_bCLOSE)->GetSafeHwnd() )
		{
			m_pHeadDlg->PostMessage(SET_TAB_ORDER, 0, 0);
		}

		//===================================================
		//2009.04.13 UPDATE BY LKS : 문자가 들어가면 프로그램 다운되는 현상 수정
//DEL		if(GetDlgItem(IDC_ePUBLISH_YEAR)->m_hWnd == pMsg->hwnd )
//DEL		{
//DEL			INT nVirtKey = (INT) pMsg->wParam;
//DEL			if ((nVirtKey < '0' || nVirtKey > '9') && (nVirtKey != '-') && nVirtKey != 8 && (nVirtKey != 189) && (nVirtKey != 109))
//DEL			{
//DEL				if(!(nVirtKey >= VK_NUMPAD0 && nVirtKey <= VK_NUMPAD9) && nVirtKey!=VK_DELETE && nVirtKey!=VK_LEFT && nVirtKey!=VK_RIGHT)
//DEL				{
//DEL					ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
//DEL					return TRUE;
//DEL				}
//DEL			}
//DEL		}
		//===================================================
		
		if( pMsg->wParam == VK_ESCAPE )		return TRUE;
		else if( pMsg->wParam == VK_RETURN )
		{
			if( GetDlgItem(IDC_eTITLE)->m_hWnd == pMsg->hwnd )	OnbDUPSEARCH();
			return TRUE;
		}
		
		// 2005-01-19
		// 블럭이 잡혀있는 경우에는 그 블럭설정을 SET한다.
		return CESL_Mgr::PreTranslateMessage(pMsg);
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

// 5163_3.사용자 정의 코드를 사용할 경우 사용하는 코드만 화면에 보여준다.
// BEGIN:
VOID CSpecies_Form::ShowUserDefineCode(BOOL bShowCode)
{
	CString strYN, strAlias;
	CWnd *pWnd = NULL;
	CESL_DataMgr * TmpDM;
	TmpDM = FindDM(_T("DMUP_단행_종_종정보"));

	// 사용자 정의 코드1 사용 여부.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strYN);
	if(strYN == _T("Y"))
	{
		// 2004.12.06 코드명도 바꿔준다.
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 91"), strAlias);
		pWnd = GetDlgItem(IDC_stcUSERCODE1);        pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_cmbUSERCODE1);		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd = GetDlgItem(IDC_stcUSERCODE1);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE1);		pWnd->ShowWindow(SW_HIDE);
	}

	// 사용자 정의 코드2 사용 여부.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strYN);
	if(strYN == _T("Y"))
	{
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 92"), strAlias);
		pWnd = GetDlgItem(IDC_stcUSERCODE2);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_cmbUSERCODE2);		pWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pWnd = GetDlgItem(IDC_stcUSERCODE2);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE2);		pWnd->ShowWindow(SW_HIDE);
	}

	// 사용자 정의 코드3 사용 여부.
	strYN.Empty();
	TmpDM->GetOneValueQuery(_T("SELECT USE_YN FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strYN);
	if(strYN == _T("Y") && bShowCode)
	{
		TmpDM->GetOneValueQuery(_T("SELECT CLASS_ALIAS FROM MN_USER_DEFINE_TBL WHERE CLASS_CODE = 93"), strAlias);
		pWnd = GetDlgItem(IDC_stcUSERCODE3);		pWnd->SetWindowText(strAlias);		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_cmbUSERCODE3);		pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_eDISCRIPTION_SMALL);	pWnd->ShowWindow(SW_SHOW);
		pWnd = GetDlgItem(IDC_eDISCRIPTION);		pWnd->ShowWindow(SW_HIDE);
	}
	else
	{
		pWnd = GetDlgItem(IDC_stcUSERCODE3);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_cmbUSERCODE3);		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_eDISCRIPTION_SMALL);	pWnd->ShowWindow(SW_HIDE);
	}
}
// END:

VOID CSpecies_Form::OnbtnCover() 
{
	// 2004.11.09 5163 표지 추가 기능.
	if( m_strUseCoverYN == _T("Y"))
	{
		CString strSpeciesKey;
		strSpeciesKey.Empty();

		if ( m_nOpType == SINSERT ) {
			CCOVER_MAIN dlg(this);
			dlg.SetInsertMode(TRUE);
			dlg.SetPubType(_MONO_);
			dlg.SetBasicName( m_szSpeciesKey );
			dlg.SetSpeciesKey(m_szSpeciesKey);
			dlg.SetLocalPath( _T("..\\bmp\\cover_info\\") );
			dlg.DoModal();
			dlg.GetSpeciesKey(strSpeciesKey);
			_tcscpy ( m_szSpeciesKey , strSpeciesKey.GetBuffer(0) );

		} else if( m_nOpType == SUPDATE || m_nOpType == GUPDATE){
			CCOVER_MAIN dlg(this);
			dlg.SetInsertMode(FALSE);
			dlg.SetPubType(_MONO_);
			dlg.SetLocalPath( _T("..\\bmp\\cover_info\\") );
			dlg.SetBasicName( m_szSpeciesKey );
			dlg.SetSpeciesKey( m_szSpeciesKey );
			dlg.DoModal();
		}
	}
}

// 2004.12.29
VOID CSpecies_Form::OnbtnSearchUser() 
{
	// TODO: Add your control notification handler code here
	CBO_ACQ_LOAN_USER_SEARCH_DLG UserSearchDlg(this);
	// 2005.01.05
	CString strApplicant;
	strApplicant.Empty();
	m_pCM_Index->GetControlMgrData(_T("구입신청자"), strApplicant);
	UserSearchDlg.SetUserID(strApplicant);

	UserSearchDlg.DoModal();

	CString sUserKey = UserSearchDlg.m_sSelectRecKey;
	if( sUserKey.IsEmpty() ) return ;

	CString strQuery, strName;
	strQuery.Format(_T("SELECT USER_NO FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s"), sUserKey);

	INT ids;

	CESL_DataMgr * TmpDM;
	TmpDM = FindDM(_T("DMUP_단행_종_종정보"));
	ids = TmpDM->GetOneValueQuery(strQuery, strName);
	if( ids < 0 )
	{
		AfxMessageBox(_T("대출자번호를 가져오지 못했습니다."));
		return ;
	}
	
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(IDC_edtBSFORM_REQUESTOR);
	pWnd->SetWindowText(strName);
	//pWnd->SetDlgItemText(IDC_edtBSFORM_REQUESTOR, strName);
	
}


VOID CSpecies_Form::OnSelchangecmbAPPLICANT() 
{
	// 해당리소스를 모두 숨긴다.
	GetDlgItem(IDC_tClass_Code				)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_cAPPLICAND_KIND_DEPT		)->ShowWindow(SW_HIDE);		//학부(처)
	GetDlgItem(IDC_cAPPLICAND_KIND_COURCE	)->ShowWindow(SW_HIDE);		//학과
	GetDlgItem(IDC_cAPPLICAND_KIND_CLASS	)->ShowWindow(SW_HIDE);		//부서
	

	INT nIdx = m_cmbApplicant.GetCurSel();
	// 0 - 회원
	// 1 - 부서
	// 2 - 학과
	// 3 - 학부(처)
	switch(nIdx)
	{	
	case 1:
		{
			GetDlgItem(IDC_cAPPLICAND_KIND_CLASS)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_tClass_Code)->ShowWindow(SW_SHOW);	
		}
		break;
	case 2:
		{
			GetDlgItem(IDC_cAPPLICAND_KIND_COURCE)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_tClass_Code)->ShowWindow(SW_SHOW);
		}
		break;
	case 3:
		{
			GetDlgItem(IDC_cAPPLICAND_KIND_DEPT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_tClass_Code)->ShowWindow(SW_SHOW);
		}
		break;
	case 0:
	default:
		break;
	}
}

VOID CSpecies_Form::OnSelchangecAPPLICANDKINDCLASS() 
{
	
}

VOID CSpecies_Form::OnSelchangecAPPLICANDKINDCOURCE() 
{
	
}

VOID CSpecies_Form::OnSelchangecAPPLICANDKINDDEPT() 
{
	
}

VOID CSpecies_Form::SetIME(INT nIDC, INT nMode)
{
	EFS_BEGIN

    HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(nIDC)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		if ( nMode == 0 ) dwConversion=IME_CMODE_ALPHANUMERIC;
		else dwConversion=IME_CMODE_HANGUL;		
		
		//20080218 DELETE BY PJW : 한/영 변경후 포커스 이동시 변경되지 않도록 수정			
		//ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(nIDC)->m_hWnd,hIME); 
    }

	EFS_END
}

VOID CSpecies_Form::OnSetfocusedtSERIESVOL() 
{
	SetIME(IDC_edtSERIES_VOL, 1);	
}

VOID CSpecies_Form::OnSetfocusedtPHYSICALCHARACTER() 
{
	SetIME(IDC_edtPHYSICAL_CHARACTER, 1);	
}

VOID CSpecies_Form::OnSetfocusedtSIZE() 
{
	SetIME(IDC_edtSIZE, 1);	
}

VOID CSpecies_Form::OnSetfocuseEDITIONSTMT() 
{
	SetIME(IDC_eEDITION_STMT, 1);
}

VOID CSpecies_Form::OnSetfocuseSERIESTITLE() 
{
	SetIME(IDC_eSERIES_TITLE, 1);	
}

VOID CSpecies_Form::OnSetfocuseREGCLASSCODE() 
{
	SetIME(IDC_eREG_CLASS_CODE, 0);	
}

VOID CSpecies_Form::OnSetfocuseTITLE() 
{
	SetIME(IDC_eTITLE, 1);
}

VOID CSpecies_Form::OnSetfocuseVOL() 
{
	SetIME(IDC_eVOL, 1);
}

VOID CSpecies_Form::OnSetfocusePUBLISHER() 
{
	SetIME(IDC_ePUBLISHER, 1);
}

VOID CSpecies_Form::OnSetfocusePUBLISHPLACE() 
{
	SetIME(IDC_ePUBLISH_PLACE, 1);
}

VOID CSpecies_Form::OnSetfocusedtAPPENDIX() 
{
	SetIME(IDC_edtAPPENDIX, 1);
}

VOID CSpecies_Form::OnSetfocuseDISCRIPTIONSMALL() 
{
	SetIME(IDC_eDISCRIPTION_SMALL, 1);
}

VOID CSpecies_Form::OnSetfocuseDISCRIPTION() 
{
	SetIME(IDC_eDISCRIPTION, 1);
}

VOID CSpecies_Form::OnSetfocuseAUTHOR1() 
{
	SetIME(IDC_eAUTHOR1, 1);
}

VOID CSpecies_Form::OnSetfocusedtBSFORMREQUESTOR() 
{
	SetIME(IDC_edtBSFORM_REQUESTOR, 1);	
}


//20070810 ADD PJW : 신청일자에서 시, 분, 초
VOID CSpecies_Form::strApplicantTime(CString strDateTime)
{

	m_hApplicantTime->SetData(strDateTime.Mid(strDateTime.Find(_T(" "))));
}

INT CSpecies_Form::SetUDFCheck(CESL_DataMgr *pDM, CString strAlias, CString strType)
{
	if(pDM == NULL || strAlias.IsEmpty()) return -1;
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = pDM->m_nCols;
	if(nCols < 0) nCols = pDM->GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_ALIAS == strAlias)
		{
			pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, strType);
		}
	}
	return 0;
}

/*

INT CSpecies_Form::SetUDFCheck(CESL_DataMgr *pDM, CString strAlias, CString strType)
{
	if(pDM == NULL || strAlias.IsEmpty()) return -1;
	
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
	
	INT col, nCols;
	nCols = pDM->m_nCols;
	if(nCols < 0) nCols = pDM->GetRefColumnCount();
	for (col = 0; col < nCols; col++) {
		if (pDM->GetColumnRefInfo(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE)!= 0) continue;
		if(FIELD_ALIAS == strAlias)
		{
			pDM->SetDataMgrRef(col, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, strType);
		}
	}
	return 0;
}	
*/
//==========================================================
//20080612 ADD PJW : 구입신청자가 없을때 NULL 값으로 시분초 세팅
void CSpecies_Form::OnKillfocusedtBSFORMREQUESTOR() 
{
	
	CString	strApplicantName;
	CString	strTime;
	CString	strApplicantDate;

	m_pCM_Index->GetControlMgrData(_T("구입신청자"), strApplicantName, 0);
	
	if ( strApplicantName == _T(""))
	{
		strTime.Format(_T("  :  :  "));
		strApplicantTime(strTime);
		m_pCM_Index->SetControlMgrData(_T("구입신청일"), _T(""), 0);		
		return;
	}

	CTime t = CTime::GetCurrentTime();
	m_pCM_Index->GetControlMgrData(_T("구입신청일"), strApplicantDate, 0);
	if(  strApplicantDate.IsEmpty() )
	{
		strApplicantDate.Format( _T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
		strTime.Format(_T("%02d:%02d:%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());
		m_pCM_Index->SetControlMgrData(_T("구입신청일"), strApplicantDate, 0);		
		strApplicantTime(strTime);
	}	
}
//==========================================================
//=======================================================================
//2008.07.17 ADD BY PJW : 부록 자동 등록번호 부여
INT CSpecies_Form::ApplyAppendixRegNo(CESL_DataMgr *pDM_Appendix)
{
	EFS_BEGIN

	if (pDM_Appendix == NULL) return -1;

	// 등록 번호 자동부여여부가 Y 이면 등록번호를 부여한다.
	CString strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2 = '기타' AND GROUP_3 = '공통' AND CLASS_ALIAS = '부록등록번호자동부여여부'");
//DEL 	CString strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2 = '기타' AND GROUP_3 = '공통' AND CLASS_ALIAS = '부록등록번호자동부여여부'");
	CString strValue;
	pDM_Appendix->GetOneValueQuery(strQuery, strValue);
	if (strValue != _T("Y")) return 0;

	// 등록구분이 있고 등록번호가 없으면 등록번호를 부여한다.

	INT nRowCount = pDM_Appendix->GetRowCount();
	if (nRowCount <= 0) return 0;

	INT ids = -1;
	CString strRecKey;
	CString strResult;
	CString strRegCode;
	CString strRegNo;
	CString strAccessionKey;
	CString strQueryFlag;
	CString strDbFalg;
	INT nNumber = 0;

	for (INT idx = 0; idx < nRowCount; idx++)
	{
		// 등록번호가 있으면 넘어간다.
		ids = pDM_Appendix->GetCellData( _T("BB_등록번호"), idx, strRegNo);
		if (ids >= 0 && !strRegNo.IsEmpty()) continue;

		// 등록구분 코드를 가져온다.
		ids = pDM_Appendix->GetCellData( _T("BB_등록구분_코드"), idx, strRegCode);
		if (ids < 0 || strRegCode.IsEmpty()) continue;

		ids = pDM_Appendix->GetCellData( _T("BB_DB_FLAG"), idx, strDbFalg);
		if( strDbFalg != _T("U") && strDbFalg != _T("I") )
		{
			pDM_Appendix->SetCellData( _T("BB_DB_FLAG"), _T("U"), idx);
		}

		// 마지막 등록번호를 가져온다.
		strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
						_T("WHERE KIND_CODE = 'REG_NO' AND ")
						_T("DIVISION_VALUE='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strRegCode);
//DEL 						_T("DIVISION_VALUE='%s'"), strRegCode);
		ids = pDM_Appendix->GetOneValueQuery(strQuery, strResult);

		if (ids < 0 || strResult.IsEmpty())
		{
			strResult = _T("0");

			INT ids = pDM_Appendix->StartFrame();
			if ( ids < 0 ) 
			{
				AfxMessageBox(_T("StartFrame 를 확인하세요"));
				return -1;
			}

			ids = pDM_Appendix->MakeRecKey(strRecKey);
			if (ids < 0) return ids;
			// 마지막 등록번호를 세팅한다.
			strQuery.Format(_T("INSERT INTO CO_LAST_NUMBER_TBL ")
							_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, MANAGE_CODE) ")
							_T("VALUES (%s, '%s', '%s', '%s', '%s', %s, UDF_MANAGE_CODE);")
//DEL 							_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER) ")
//DEL 							_T("VALUES (%s, '%s', '%s', '%s', '%s', %s);")
							, strRecKey, _T("CO"), _T("REG_NO"), strRegCode, CTime::GetCurrentTime().Format(_T("%Y")), strResult);
			ids = pDM_Appendix->AddFrame(strQuery);
			if (ids < 0) return ids;
			ids = pDM_Appendix->SendFrame();
			if (ids < 0) return ids;
			ids = pDM_Appendix->EndFrame();
			if (ids < 0) return ids;
		}
		// 마지막 등록번호 UPDATE 한다.
		nNumber = _ttoi(strResult) + 1;
		
		strRegNo.Format(_T("%s%010d"), strRegCode, nNumber);

		ids = pDM_Appendix->SetCellData( _T("BB_등록번호"), strRegNo, idx);
		//등록일자를 기록 한다.
		CString strRegNoInputDate;
		CTime t = CTime::GetCurrentTime();
		ids = pDM_Appendix->GetCellData( _T("BB_등록일"), idx, strRegNoInputDate);
		if(  strRegNoInputDate.IsEmpty() )
		{
			strRegNoInputDate.Format( _T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
			ids = pDM_Appendix->SetCellData( _T("BB_등록일"), strRegNoInputDate, idx);
		}	

		INT ids = pDM_Appendix->StartFrame();

		if ( ids < 0 ) 
		{
			AfxMessageBox(_T("StartFrame 를 확인하세요"));
			return -1;
		}
		strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET ")
						_T("LAST_NUMBER = %d WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '%s';"),
//DEL 						_T("LAST_NUMBER = %d WHERE KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '%s';"),
						nNumber, strRegCode);
		ids = pDM_Appendix->AddFrame(strQuery);
		if (ids < 0) return ids;
		ids = pDM_Appendix->SendFrame();
		if (ids < 0) return ids;
		ids = pDM_Appendix->EndFrame();
		if (ids < 0) return ids;
	}

	return 0;

	EFS_END
	return -1;
}
//=======================================================================
//===================================================
//2009.05.28 ADD BY LKS : Control Trim

VOID CSpecies_Form::TrimControl()
{
	INT ControlID[18]={IDC_eTITLE,IDC_edtAPPENDIX,IDC_eDISCRIPTION_SMALL,
		IDC_eAUTHOR1,IDC_eVOL,IDC_eSERIES_TITLE,IDC_edtSERIES_VOL,
		IDC_eEDITION_STMT,IDC_ePUBLISH_PLACE,IDC_ePUBLISHER,
		IDC_ePUBLISH_YEAR,IDC_eEA_ISBN,IDC_eEA_ISBN_ADD,IDC_eEA_ISBN_MISTAKE,
		IDC_eSET_ISBN,IDC_eSET_ISBN_ADD,IDC_eSET_ISBN_MISTAKE,
		IDC_edtPHYSICAL_CHARACTER};

	CString strData;
	CEdit* pEdit = NULL;
	for(INT i=0;i<18;i++)
	{
		pEdit = (CEdit*)GetDlgItem(ControlID[i]);
		if (pEdit == NULL) continue;
		pEdit->GetWindowText(strData);
		if( !strData.IsEmpty() )
		{
			strData.TrimLeft();		strData.TrimRight();
			pEdit->SetWindowText(strData);	
		}
	}
}
//===================================================