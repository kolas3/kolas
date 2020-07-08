// FeeHistoryReceiptDlg.cpp : implementation file
// MAKE BY PWR

#include "stdafx.h"
#include "FeeEditHistory.h"
#include "FeeEditArrear.h"
#include "FeeEditLoanStop.h"
#include "FeeEditReCard.h"
#include "FeeEditReCard2.h"
#include "FeeHistoryReceiptDlg.h"
#include "FeeInsert.h"
#include "FeeInsertLogDlg.h"

// SBL
#include "SBL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeHistoryReceiptDlg dialog

CFeeHistoryReceiptDlg::CFeeHistoryReceiptDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CFeeHistoryReceiptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeHistoryReceiptDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
}
CFeeHistoryReceiptDlg::~CFeeHistoryReceiptDlg()
{
}

VOID CFeeHistoryReceiptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeHistoryReceiptDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFeeHistoryReceiptDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeHistoryReceiptDlg)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFeeHistoryReceiptDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFeeHistoryReceiptDlg)
	ON_EVENT(CFeeHistoryReceiptDlg, IDC_gridMAIN, -601 /* DblClick */, OnDblClickgridMAIN, VTS_NONE)
	ON_EVENT(CFeeHistoryReceiptDlg, IDC_gridMAIN, -600 /* Click */, OnClickgridMAIN, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeHistoryReceiptDlg message handlers

BOOL CFeeHistoryReceiptDlg::Create(CWnd* pParentWnd) 
{
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CFeeHistoryReceiptDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("FEE_HISTORY") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : FEE_HISTORY") );
		return FALSE;
	}

	m_pGrid = (CESL_Grid*)FindControl( _T("CM_FEE_HISTORY") , _T("MainGrid") );
	if( m_pGrid == NULL ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::OnInitDialog()\r\n")
						_T("-Reference Error::CM_FEE_HISTORY"), MB_ICONSTOP );
		return FALSE;
	}

	//JOB.2019.0017 ADD BY KYJ
	//개인정보 엑셀 반출,로그 설정값 적용
	//-------------------------------------------------------------------------
	if(!m_pInfo->m_bUserInfoExportYN)
	{
		m_pGrid->m_uUseKeyFlag = ESL_GRID_NOT_USE_F12;
	}
	else
	{
		m_pGrid->m_bPrivacyPrintLogRecord = TRUE;
		m_pGrid->m_strPrintInfo = _T("수수료 납부이력");
	}
	//-------------------------------------------------------------------------

	m_pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !m_pDM ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::OnInitDialog()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return FALSE;
	}

	CString sValue;
	GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("대출정지휴관일포함유무"), m_sIsHolyDay );
	GetManageValue( _T("열람"), _T("공통"), _T("연체관리"), _T("연체료"), sValue );
	m_nBasicArrear = _ttoi( sValue );
	GetManageValue( _T("열람"), _T("공통"), _T("부록관리"), _T("부록연체적용여부"), m_sSupplement );

	// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), m_sIsUnityLoanService );
	m_sIsUnityLoanService.TrimLeft();	m_sIsUnityLoanService.TrimRight();
	m_sIsUnityLoanService.MakeUpper();
	if(_T("Y")!=m_sIsUnityLoanService) m_sIsUnityLoanService = _T("N");

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;
}

VOID CFeeHistoryReceiptDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	const INT term = 0;	
	CESL_Grid *pGrid = ( CESL_Grid * )GetDlgItem( IDC_gridMAIN );
	if( pGrid->GetSafeHwnd() )
		pGrid->MoveWindow( term, term, cx - ( term * 2 ), cy - ( term * 2 ) );	
}

// 2008.03.04 ADD BY PWR
// ** 이력을 검색한다. ( sQuery로 완성된 쿼리가 들어온다. )
VOID CFeeHistoryReceiptDlg::SearchData( CString sQuery )
{
	INT ids;
	// 1. 맴버 번수로 저장
	m_sQuery = sQuery;

	// 2. 받은 쿼리로 검색을 하고 DM으로 복사
	InsertDM( sQuery );

	// 3. 그리드에 표시
	ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
		if( ids == -1 )
			sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control을 찾을 수 없음");
		else if( ids == -1006 )
			sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM을 찾을 수 없음");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
		return;
	}
}

// 2008.03.05 ADD BY PWR
// ** DM에 데이타를 넣는다. ( 검색결과->TempDM->pDM )
VOID CFeeHistoryReceiptDlg::InsertDM( CString sQuery )
{
// 2008.04.22 UPDATE BY PWR
// ID추가 ( 로그인한ID를 보여주도록 DM에 추가 )
	INT ids;

	BEGIN_SBL();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 1. TEMP에 검색결과 넣어준다.
	CESL_DataMgr *pTmpDM = FindDM( _T("DM_FEE_HISTORY_TMP") );
	if( !pTmpDM ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::InsertDM()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY_TMP"), MB_ICONSTOP );
		return;
	}
	CESL_DataMgr *pDM = FindDM( _T("DM_FEE_HISTORY") );
	if( !pDM ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::InsertDM()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return;
	}
	pDM->FreeData();

	ids = pTmpDM->RestructDataManager( sQuery );
	if( ids < 0 )
	{
		CString sErrorMsg;
		sErrorMsg = _T("RestructDataManager Error::검색이력");
		if( ids == -4004 )
			sErrorMsg = _T("RestructDataManager Error::검색이력\r\n-접속되지않음");
		else if( ids == -4005 )
			sErrorMsg = _T("RestructDataManager Error::검색이력\r\n-쿼리실행시 에러발생");
		else if( ids == -4006 )
			sErrorMsg = _T("RestructDataManager Error::검색이력\r\n-쿼리가 너무많음");
		AfxMessageBox( sErrorMsg, MB_ICONSTOP );
		return;
	}

	END_SBL(pTmpDM->GetRowCount());

	if( pTmpDM->GetRowCount() <= 0 ) return;

////////////////////////////////////////////////////////////////////////////////////////////////////	
/// 2. TEMP에서 실제 DM으로 검색된 결과를 복사한다.
	CString sData[24];
	CString sAlias[] = { _T("대출자번호") , _T("대출자명")		, _T("회원구분")	, _T("소속")		,
						 _T("직급")		  , _T("영수증출력유무"), _T("수수료종류")	, _T("납부금액")	,
						 _T("납부일자")	  , _T("대출일자")		, _T("반납예정일")	, _T("반납일자")	, 
						 _T("등록번호")	  , _T("서명")			, _T("자료실")		, _T("RECEIPT_KEY")	, 
						 _T("회원증종류") , _T("로그인ID")		, _T("PAYMENT_KEY")	, _T("LOAN_INFO_KEY")	, 
						 _T("대출정지일") , _T("소급입력유무")	, _T("관리구분") };

	for(INT i = 0; i< pTmpDM->GetRowCount(); i++)
	{
		pDM->InsertRow(-1);
		for(INT j = 0; j < pTmpDM->GetColCount(); j++ )
		{
			pTmpDM->GetCellData(i, j, sData[j] );
			pDM->SetCellData( sAlias[j], sData[j], i );
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3. 코드 설명으로 변경
	//this->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("대출소속정보"), _T("소속") );
	//this->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("대출직급정보"), _T("직급") );
	//// 2008.04.04 ADD BY PWR
	//// 자료실 코드를 자료실 설명으로 변경
	//// 자료실을 연체료만 보여주도록 한다.
	//this->m_pInfo->pCodeMgr->ConvertCodeToDescInDM( pDM, _T("자료실구분"), _T("자료실") );
	
	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		// 2009.03.04 ADD BY PWR : 코드 -> 설명 변경 FOR문에서 다 처리하도록 
		CString sCode, sDesc;
		// 소속
		pDM->GetCellData( _T("소속"), i, sCode);
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), sCode, sDesc);
		pDM->SetCellData( _T("소속"), sDesc, i);
		// 직급
		sCode.Empty();	sDesc.Empty();
		pDM->GetCellData( _T("직급"), i, sCode);
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출직급정보"), sCode, sDesc);
		pDM->SetCellData( _T("직급"), sDesc, i);
		// 자료실
		sCode.Empty();	sDesc.Empty();
		pDM->GetCellData( _T("자료실"), i, sCode);
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료실구분"), sCode, sDesc);
		pDM->SetCellData( _T("자료실"), sDesc, i);
		// 관리구분
		sCode.Empty();	sDesc.Empty();
		pDM->GetCellData( _T("관리구분"), i, sCode);
		this->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("관리구분"), sCode, sDesc);
		pDM->SetCellData( _T("관리구분설명"), sDesc, i);
		// 대출자번호
		sCode.Empty(); sDesc.Empty();
		pDM->GetCellData( _T("대출자번호"), i, sCode );
		// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용 할 경우 표시방법 변경
//DEL 		CString strBuf = sCode;
//DEL 		if( 13 <= sCode.GetLength() )
//DEL 		{
//DEL 			strBuf = sCode.Mid(0,6) + _T("-") + sCode.Mid(6);
//DEL 		}
//DEL 		ids = CLocCommonAPI::CivilNoCheck( strBuf );
//DEL 		if( 0 == ids )
//DEL 		{
//DEL 			sDesc.Format(_T("%s**%s******"),strBuf.Left(4),strBuf.Mid(7,1));
//DEL 		}
//DEL 		else
//DEL 		{
//DEL 			sDesc = sCode;
//DEL 		}
		CLocCommonAPI::ChangeSecuriyUserNo( this , sCode , m_sIsUnityLoanService );
		pDM->SetCellData( _T("대출자번호설명"), sCode, i );

		CString sInferiorClass;
		CString sFeeClass;
		CString sPayment;
		CString sCardClass;
		CString sShelf;
		CString sRetroYN;

		pDM->GetCellData( _T("회원구분")	, i, sInferiorClass );
		pDM->GetCellData( _T("수수료종류")	, i, sFeeClass );
		pDM->GetCellData( _T("납부금액")	, i, sPayment );
		pDM->GetCellData( _T("회원증종류")	, i, sCardClass );
		pDM->GetCellData( _T("자료실")		, i, sShelf ); 
		pDM->GetCellData( _T("소급입력유무"), i, sRetroYN ); 

		// 3.1 회원구분 ( 0:정상, 1:불량, 2:제적, 3:대출증분실 )
		if( _ttoi(sInferiorClass) == 0 )
			sInferiorClass = _T("정상회원");
		else if( _ttoi(sInferiorClass) == 1 )
			sInferiorClass = _T("대출정지");
		else if( _ttoi(sInferiorClass) == 2 )
			sInferiorClass = _T("제적회원");
		else if( _ttoi(sInferiorClass) == 3 )
			sInferiorClass = _T("대출증분실");

		// 3.2 수수료종류 ( 0:연체료, 1:회원증재발급, 2:대출정지일해제 )
		if( _ttoi(sFeeClass) == 0 )
		{
			sFeeClass = _T("연체료");
		}
		else if( _ttoi(sFeeClass) == 1 )
		{
			if( !sCardClass.IsEmpty() )
			{
				sFeeClass.Format( _T("회원증재발급\n(%s)"), sCardClass );
			}
			else
			{
				sFeeClass = _T("회원증재발급");
			}
			pDM->SetCellData( _T("자료실")	, _T(""), i );
		}
		else if( _ttoi(sFeeClass) == 2 )
		{
			sFeeClass = _T("대출정지일해제");
			pDM->SetCellData( _T("자료실")	, _T(""), i );
		}

		// 3.3 납부금액 천원단위 표현
		SetMoneyFormat( sPayment );

		pDM->SetCellData( _T("회원구분")	, sInferiorClass, i );
		pDM->SetCellData( _T("수수료종류")	, sFeeClass		, i );
		pDM->SetCellData( _T("납부금액")	, sPayment		, i );

		// 2009.03.06 ADD BY PWR : RETRO_YN이 Y인자료는 소급입력자료이다.
		// 소급입력은 이용자 키만 물려서 자료실을 알수없으므로
		if(sRetroYN == _T("Y"))
		{
			pDM->SetCellData( _T("자료실")	, _T(""), i );
		}
	}
}

// 2008.03.05 ADD BY PWR
// ** 천원단위 표현
VOID CFeeHistoryReceiptDlg::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace(_T(","), _T(""));

	CString strBuf = sMoney;
	strBuf.Replace(_T(","), _T(""));

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize-1);

	sMoney = pszFormattedNumber;

	if (pszFormattedNumber)
		delete [] pszFormattedNumber;
}

// 2008.03.06 ADD BY PWR
// ** 목록출력
INT CFeeHistoryReceiptDlg::PrintList()
{
EFS_BEGIN

	INT ids;
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 1. 목록 DM 과 출력 DM 초기화 
	CESL_DataMgr *pSrcDM = FindDM( _T("DM_FEE_HISTORY") );
	if( pSrcDM == NULL ) 
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::PrintList()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY"), MB_ICONSTOP );
		return -1;
	}
		
	if( pSrcDM->GetRowCount() < 1 )
	{
		AfxMessageBox( _T("출력할 목록이 존재하지 않습니다.") );
		return -2;
	}
	
	CESL_DataMgr *pPrintDM = FindDM( _T("DM_FEE_HISTORY_PRINT") );
	if( pPrintDM == NULL )
	{
		AfxMessageBox(	_T("CFeeHistoryReceiptDlg::PrintList()\r\n")
						_T("-Reference Error::DM_FEE_HISTORY_PRINT"), MB_ICONSTOP );
		return -3;
	}
	pPrintDM->FreeData();

////////////////////////////////////////////////////////////////////////////////////////////////////
/// 2. 출력 필드 설정
	const INT nCopyCnt = 15;
	TCHAR *sField[nCopyCnt][2] =
	{
		{ _T("대출자번호설명")	, _T("대출자번호")		},
		{ _T("대출자명")		, _T("대출자명")		},
		{ _T("회원구분")		, _T("회원구분")		},
		{ _T("소속")			, _T("소속")			},
		{ _T("직급")			, _T("직급")			},
		{ _T("영수증출력유무")	, _T("영수증출력유무")	},
		{ _T("수수료종류")		, _T("수수료종류")		},
		{ _T("납부금액")		, _T("납부금액")		},
		{ _T("납부일자")		, _T("납부일자")		},
		{ _T("대출일자")		, _T("대출일자")		},
		{ _T("반납예정일")		, _T("반납예정일")		},
		{ _T("반납일자")		, _T("반납일자")		},
		{ _T("등록번호")		, _T("등록번호")		}, 
		{ _T("서명")			, _T("서명")			},
		{ _T("자료실")			, _T("자료실")			}
	};
	
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 3. 출력모드 설정
	// 선정된 로우가 없다면 전체 출력
	m_pGrid->SelectMakeList();
	m_pGrid->SelectGetHeadPosition();
	
	INT nMode = 0;
	INT nSelectCnt = m_pGrid->SelectGetCount();
	
	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = pSrcDM->GetRowCount();
	
////////////////////////////////////////////////////////////////////////////////////////////////////
// 4. pSrcDM -> pPrintDM으로 복사
	CString sData;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = m_pGrid->SelectGetIdx();		

		pPrintDM->InsertRow(-1);
		
		sData.Format( _T("%d") , i+1 );
		ids = pPrintDM->SetCellData( _T("번호") , sData , i);
		if( ids < 0 ) AfxMessageBox( _T("PrintList") );
		
		for( INT j = 0 ; j < nCopyCnt ; j++ )
		{
			ids = pSrcDM->GetCellData( sField[j][0] , nIndex , sData );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , nIndex , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) AfxMessageBox( _T("PrintList") );
			
			ids = pPrintDM->SetCellData( sField[j][1] , sData , i );
			if( ids < 0 ) 
			{
				CString sEMsg;
				sEMsg.Format( _T("sField[%d][0] : %s , j : %d , sData : %s ") , i , sField[j][0] , sData );
				AfxMessageBox( sEMsg );
			}
			if( ids < 0 ) AfxMessageBox( _T("PrintList") );
		}
		if( nMode == 1)
			m_pGrid->SelectGetNext();
	}
	
////////////////////////////////////////////////////////////////////////////////////////////////////
/// 5. SIreport 출력
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	if (!pSIReportManager) {
		AfxMessageBox( _T("Report Manager를 생성하지 못하였습니다.") );
		return -3;
	}
	pSIReportManager->SetCONNECTION_INFO(pPrintDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB( _T("수수료납부이력") );
	if (ids < 0) {
		AfxMessageBox( _T("DB로 부터 SI Report 정보를 가져오지 못하였습니다.") );
		return -4;
	}
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	pSIReportManager->m_bPrivacyPrintLogRecord = TRUE;
	pSIReportManager->SetDataMgr(0, pPrintDM , 0);
	
	ids = pSIReportManager->Print();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}	
	
	return 0;
	
EFS_END
return -1;
}

// 2008.06.02 ADD BY PWR
// ** 수정이력보기
INT CFeeHistoryReceiptDlg::PaymentEditHistory()
{
EFS_BEGIN

	CFeeEditHistory dlg(this);
	// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
	dlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
	dlg.DoModal();

	return 0;

EFS_END
return -1;
}

// 2008.05.29 ADD BY PWR
// ** 납부금액 수정
INT CFeeHistoryReceiptDlg::PaymentEdit()
{
EFS_BEGIN
	
	// 검색된 데이터가 있는지 검사한다.
	if( m_pDM->GetRowCount() < 1 ) return 0;

	// 납부취소할 자료 하나만 선정했는지 검사한다.
	m_pGrid->SelectMakeList();
	m_pGrid->SelectGetHeadPosition();
	INT nSelectCnt = m_pGrid->SelectGetCount();
	if( nSelectCnt < 1 )
	{
		AfxMessageBox( _T("수정할 자료를 선정해 주십시오."), MB_ICONINFORMATION );
		return 0;
	}
	else if( nSelectCnt > 1 )
	{
		AfxMessageBox( _T("수정할 자료 하나만 선정해 주십시오."), MB_ICONINFORMATION );
		return 0;
	}

	// 수수료 종류별 수정
	m_nIndex = m_pGrid->SelectGetIdx();
	CString sFeeClass, sCardClass;
	m_pDM->GetCellData( _T("수수료종류"), m_nIndex, sFeeClass );
	m_pDM->GetCellData( _T("회원증종류"), m_nIndex, sCardClass );

	// 1. 회원증재발급 수정
	if( !sCardClass.IsEmpty() )
	{
		EditReCard();
		return 0;
	}
	// 2. 연체료 수정
	else if( sFeeClass == _T("연체료") )
	{
		EditArrear();
		return 0;
	}
	// 3. 대출정지일해제 수정
	else if( sFeeClass == _T("대출정지일해제") )
	{
		EditLoanStopDateCancel();
		return 0;
	}
	// 2009.03.04 ADD BY PWR : 소급입력추가
	else
	{
		EditReCard2();
		return 0;
	}
	return 0;

EFS_END
return -1;
}

// 2008.05.29 ADD BY PWR
// ** 연체료 수정
INT CFeeHistoryReceiptDlg::EditArrear()
{
EFS_BEGIN

	INT ids;
	CString sErrorMsg;

	CFeeEditArrear dlg(this);
	dlg.m_nIndex = m_nIndex;
	dlg.DoModal();
	if( dlg.m_bSuccess == TRUE )
	{
		// 받은 쿼리로 검색을 하고 DM으로 복사
		InsertDM( m_sQuery );
		// 그리드에 표시
		ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
		if( ids < 0 )
		{
			sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
			if( ids == -1 )
				sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control을 찾을 수 없음");
			else if( ids == -1006 )
				sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM을 찾을 수 없음");
			AfxMessageBox( sErrorMsg, MB_ICONSTOP );
			return -1;
		}

		AfxMessageBox( _T("수정되었습니다.") );
	}

	return 0;

EFS_END
return -1;
}

// 2008.05.29 ADD BY PWR
// ** 대출정지일 해제 수수료 수정
INT CFeeHistoryReceiptDlg::EditLoanStopDateCancel()
{
EFS_BEGIN

	INT ids;
	CString sErrorMsg;

	CFeeEditLoanStop dlg(this);
	dlg.m_nIndex = m_nIndex;
	dlg.DoModal();
	if( dlg.m_bSuccess == TRUE )
	{
		// 받은 쿼리로 검색을 하고 DM으로 복사
		InsertDM( m_sQuery );
		// 그리드에 표시
		ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
		if( ids < 0 )
		{
			sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
			if( ids == -1 )
				sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control을 찾을 수 없음");
			else if( ids == -1006 )
				sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM을 찾을 수 없음");
			AfxMessageBox( sErrorMsg, MB_ICONSTOP );
			return -1;
		}

		AfxMessageBox( _T("수정되었습니다.") );
	}

	return 0;

EFS_END
return -1;
}

// 2008.05.29 ADD BY PWR
// ** 회원증재발급 수수료 수정
INT CFeeHistoryReceiptDlg::EditReCard()
{
EFS_BEGIN

	INT ids;
	CString sErrorMsg;

	CFeeEditReCard dlg(this);
	dlg.m_nIndex = m_nIndex;
	dlg.DoModal();
	if( dlg.m_bSuccess == TRUE )
	{
		// 받은 쿼리로 검색을 하고 DM으로 복사
		InsertDM( m_sQuery );
		// 그리드에 표시
		ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
		if( ids < 0 )
		{
			sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
			if( ids == -1 )
				sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control을 찾을 수 없음");
			else if( ids == -1006 )
				sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM을 찾을 수 없음");
			AfxMessageBox( sErrorMsg, MB_ICONSTOP );
			return -1;
		}

		AfxMessageBox( _T("수정되었습니다.") );
	}

	return 0;

EFS_END
return -1;
}

// 2009.03.04 ADD BY PWR
// ** 회원증재발급 수수료 수정 (소급입력된거)
INT CFeeHistoryReceiptDlg::EditReCard2()
{
EFS_BEGIN

	INT ids;
	CString sErrorMsg;

	CFeeEditReCard2 dlg(this);
	dlg.m_nIndex = m_nIndex;
	dlg.DoModal();
	if( dlg.m_bSuccess == TRUE )
	{
		// 받은 쿼리로 검색을 하고 DM으로 복사
		InsertDM( m_sQuery );
		// 그리드에 표시
		ids = ControlDisplay( _T("CM_FEE_HISTORY"), _T("MainGrid") );	
		if( ids < 0 )
		{
			sErrorMsg = _T("ControlDisplay Error::CM_FEE_HISTORY-MainGrid");
			if( ids == -1 )
				sErrorMsg = _T("ControlDisplay Error::'MainGrid' Grid Control을 찾을 수 없음");
			else if( ids == -1006 )
				sErrorMsg = _T("ControlDisplay Error::'CM_FEE_HISTORY' CM을 찾을 수 없음");
			AfxMessageBox( sErrorMsg, MB_ICONSTOP );
			return -1;
		}

		AfxMessageBox( _T("수정되었습니다.") );
	}

	return 0;

EFS_END
return -1;
}

/*
@Create
 - 2009.03.03 BY PWR
@Description
 - 수수료 소급입력
@Parameter
 - 
@Return Value
 - 
@Mini Spec
 1. 
*/
INT CFeeHistoryReceiptDlg::FeeInsertProc()
{
	CFeeInsert dlg(this);
	// 2010.09.08 ADD BY KSJ : 통합도서서비스 사용여부
	dlg.m_sIsUnityLoanService = m_sIsUnityLoanService;
	dlg.pMain = this->pMain;
	dlg.pParentMgr = (CESL_Mgr*)this;
	dlg.m_pInfo = this->m_pInfo;
	if( IDOK == dlg.DoModal() )
	{
		CFeeInsertLogDlg logDlg(this);
		logDlg.pMain = this->pMain;
		logDlg.pParentMgr = (CESL_Mgr*)this;
		logDlg.m_pInfo = this->m_pInfo;
		logDlg.m_strFeeClass = dlg.m_strFeeClass;
		logDlg.m_strPaymentDate = dlg.m_strPaymentDate;
		logDlg.m_strMoneyPaid = dlg.m_strMoneyPaid;
		logDlg.m_strUserKey = dlg.m_strUserKey;
		logDlg.m_strUserName = dlg.m_strUserName;
		logDlg.m_strUserNo = dlg.m_strUserNo;
		logDlg.m_strUserNoDesc = dlg.m_strUserNoDesc;
		if( IDOK == logDlg.DoModal() )
		{
			AfxMessageBox(_T("수수료를 입력하였습니다."));
		}
	}

	return 0;
}

HBRUSH CFeeHistoryReceiptDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

void CFeeHistoryReceiptDlg::OnDblClickgridMAIN() {}
void CFeeHistoryReceiptDlg::OnClickgridMAIN() {}
