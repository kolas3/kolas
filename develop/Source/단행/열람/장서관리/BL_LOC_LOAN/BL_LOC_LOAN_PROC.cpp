#include "stdafx.h"
#include "BL_LOC_LOAN_PROC.h"
#include "LoanBookListDlg.h"
#include "BO_LOC_3170.h"
#include "BO_LOC_3170_ReserveInfo.h"
#include "InputReservatoinExpireDay.h"
#include "BO_LOC_3130.h"
#include "ReservationInfoDlg.h"
#include "TestMsgDlg.h"
#include "SearchBookListDlg.h"
#include "BL_LOC_RESERVATOIN_PROC.h"
#include "LoanAppendixListDlg.h"
#include "LoanHistory.h"
#include "BO_LOC_LOAN_DATE.h"
#include "FeeLoanStop.h"
#include "SBL.h"
#include "ReserveLinkBookInfoDlg.h"
#include "ReserveComplete.h"
#include "ILL_RETURN_SEARCH_BOOK.h"
#include "ILL_LOAN.h"
#include "ILL_RETURN.h"

#include "InsertPasswordDlg.h"
#include "..\\BL_LOC_USER\\LoanUserListDlg.h"
#include "..\\BL_LOC_USER\\FAMILY_ID.h"
#include "..\\BL_LOC_USER\\BO_LOC_3320.h"
#include "KL_SearchLoanInfoDlg.h"
#include "KL_UserSearchDlg.h"
#include "..\\BO_LOC_ARREAR\\ArrearInfoDlg.h"
#include "..\\BO_LOC_ARREAR\\ArrearViewDlg.h"
#include "LibListDlg.h"
#include "KL_UserImportDlg.h"
#include "KlOtherLoanListDlg.h"
#include "BO_LOC_3000Api.h"
#include "ILL_APL_3100.h"
#include "ILL_RESERVE_CONVERT_LIB_DLG.h"
#include "..\\..\\..\\..\\공통\\K2UP_Common\\MakeSearchData.h"
#include "..\\..\\..\\..\\관리\\검색\\BOBookDetailViewer\\BOBookDetailViewerDlg.h"
#include "..\\..\\..\\..\\관리\\검색\\SEBookDetailViewer\\SEBookDetailViewerDlg.h"
#include "../../../공통/COMMON_CONVERSION_MGR/MarcInfoConversionMgr.h"
#include "..\\RFID_JOB\\RFID_Interface.h"
#include "..\\RFID_JOB\\RFID_Change.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CBO_LOC_3320 *m_EditLoanUser;

CBL_LOC_LOAN_PROC::CBL_LOC_LOAN_PROC(CESL_Mgr *pWnd, CLoanShareManager* pLoanShareManager, CLonaManageValue *pManageValue, CPreRegCodeMgr* pPreRegNoMgr)
	: CSPF_Work(pWnd)
{
	m_bStopState = 3;
	m_bShowMessage = TRUE;
	m_pManageValue = pManageValue;
	m_pCheckLoan = NULL;
	m_pCheckLoan = new CCheckLoanBook(m_pParentMgr, m_pManageValue);
	m_pLocCommonAPI = NULL;
	m_pLocCommonAPI = new CLocCommonAPI(m_pParentMgr);
	m_sResultCMAlias = _T("CM_BO_LOC_3191_AUTO_JOB");
	m_sResultProgressAlias = _T("진행상황");
	m_sResultDMAlias = _T("DM_BO_LOC_3191_AUTO_JOB");

	m_pMessageDlg[0] = NULL;
	m_pMessageDlg[1] = NULL;
	m_pMessageDlg[2] = NULL;

	m_pLoanShareManager = pLoanShareManager;

	m_pPreRegNoMgr = pPreRegNoMgr;

	m_lp3000ApiSocket = NULL;
	m_h3000ApiDLL = AfxLoadLibrary(_T("BO_LOC_3000Api.dll"));
	if(m_h3000ApiDLL != NULL)
	{
		m_lp3000ApiSocket = (LP3000APISOCKET)GetProcAddress(m_h3000ApiDLL, "BO_LOC_3000Api_Socket");
	}

	m_ReceiptDlg = NULL;

	m_strUserID=pWnd->GetUserID();
	m_strLibCode=_T("");

	m_nMessageLoop = 0;
	m_bReturnBookNonFree = FALSE;
	m_obj3170DlgList = new CObList();
	m_bLoanCallReserve = FALSE;
	m_bBeep = FALSE;
	m_bSimpleGrid = FALSE;
	m_bSimpleCmd  = FALSE;
	m_EditLoanUser = NULL;
	m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("핀패드사용여부"), _T(""), m_strPinpadYN);
	m_strPinpadYN.TrimLeft();		m_strPinpadYN.TrimRight();
	m_strPinpadYN.MakeUpper();
	if(m_strPinpadYN.IsEmpty())
	{
		m_strPinpadYN = _T("N");
	}
 
	m_strReturnBookLibCode = _T("");
	
	m_pOtherReturnMgr = NULL;
	
	m_strMaxLoanCount = _T("");
	
	m_strLocalFreeYN = _T("");
	m_strOtherFreeYN = _T("");
	m_strUserNo = _T("");

	
	m_strSerchCivilNo = _T("");

	m_bManageCodeColorUseYN = FALSE;

	m_bReferenceColorUseYN = FALSE;

	m_nILLReturnProcCnt = 0;
	m_IsReqPrior = FALSE;
}

CBL_LOC_LOAN_PROC::~CBL_LOC_LOAN_PROC()
{
	
	INT nObjCount;
	nObjCount = m_obj3170DlgList->GetCount();
	if(!m_pManageValue->m_bVolReserve)
	{ 
		CBO_LOC_3170 *p3170Dlg;
		for(INT i = 0; i < nObjCount; i++)
		{
			p3170Dlg = NULL;
			p3170Dlg = (CBO_LOC_3170*)m_obj3170DlgList->RemoveHead();
			if(p3170Dlg != NULL) 
			{
				delete p3170Dlg;
			}
			p3170Dlg = NULL;
		}
	}
	else
	{ 
		CBO_LOC_3170_ReserveInfo *p3170Dlg;
		for(INT i = 0; i < nObjCount; i++)
		{
			p3170Dlg = NULL;
			p3170Dlg = (CBO_LOC_3170_ReserveInfo*)m_obj3170DlgList->RemoveHead();
			if(p3170Dlg != NULL) 
			{
				delete p3170Dlg;
			}
			p3170Dlg = NULL;
		}
	}
	m_obj3170DlgList->RemoveAll();
	delete m_obj3170DlgList;
	m_obj3170DlgList = NULL;

	
	if(m_ReceiptDlg != NULL)
	{
		delete m_ReceiptDlg;
	}
	m_ReceiptDlg = NULL;

	if(m_pCheckLoan != NULL) 
	{
		delete m_pCheckLoan;
	}
	m_pCheckLoan = NULL;

	if(m_pLocCommonAPI != NULL) 
	{
		delete m_pLocCommonAPI;
	}
	m_pLocCommonAPI = NULL;

	if(m_EditLoanUser != NULL)
	{
		delete m_EditLoanUser;
	}
	m_EditLoanUser = NULL;

	for(INT i = 0; i < 3; i++) 
	{
		if(m_pMessageDlg[i] != NULL)
		{
			if(m_pMessageDlg[i]->GetSafeHwnd() != NULL)
			{
				m_pMessageDlg[i]->DestroyWindow();
			}
			if(m_pMessageDlg[i] != NULL)
			{
				delete m_pMessageDlg[i];
			}
		}
		m_pMessageDlg[i] = NULL;
	}

	if(m_h3000ApiDLL != NULL)
	{
		AfxFreeLibrary(m_h3000ApiDLL);
		m_h3000ApiDLL = NULL;
	}
}

INT CBL_LOC_LOAN_PROC::SPFExcutePath(CString sPathAlias)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SPFExcutePath");

	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();
	
	if(NULL == this)
	{
		return -1000;
	}

	
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CString sGridAlias2 = _T("MainGrid2");	

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid *pGrid2 = (CESL_Grid*)(pCM->FindControl(sGridAlias2));
	if(pGrid2 == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	if(sPathAlias.Compare(_T("회원_검색")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = SearchLoanInfo();
	}
	else if(sPathAlias.Compare(_T("회원_검색_NOASK")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = SearchLoanInfo(FALSE);
	}
	else if(sPathAlias.Compare(_T("회원리로드")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReLoadLoanInfo();
	}
	else if(sPathAlias.Compare(_T("회원리로드_NOASK")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReLoadLoanInfo(FALSE);
	}
	else if(sPathAlias.Compare(_T("회원정보만리로드")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReLoadMemberInfo();
	}
	else if(sPathAlias.Compare(_T("대출자관리_회원변경")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ChangeMember();
	}
	else if(sPathAlias.Compare(_T("대출")) == 0)
	{
		m_bSimpleCmd = FALSE;
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = Loan(0);
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();
		}
	}
	
	else if(sPathAlias.Compare(_T("타관대출")) == 0)
	{
		m_bSimpleCmd = FALSE;
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = OtherLoan(0);
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();
		}
	}
	else if(sPathAlias.Compare(_T("긴급대출")) == 0)
	{
		m_bSimpleCmd = FALSE;
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = NoDBLoan();
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();			
		}
	}
	else if(sPathAlias.Compare(_T("반납")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReturnBook();
	}

	else if(sPathAlias.Compare(_T("책두레대출")) == 0)
	{			
		ids = Loan(2);
	}
	else if(sPathAlias.Compare(_T("책두레예약대출")) == 0)
	{
		ids = ILL_ReservationLoan();
	}
	else if(sPathAlias.Compare(_T("책두레반납")) == 0)
	{		
		m_bSimpleCmd = FALSE;
		ids = ReturnBook(TRUE, FALSE, TRUE);
	}	

	else if(sPathAlias.Compare(_T("예약")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = Reservation();		
		m_IsReqPrior = FALSE;
	}
	else if(sPathAlias.Compare(_T("예약자료지정")) == 0)
	{		
		m_bSimpleCmd = FALSE;
		ids = ReserveLinkBookInfo();
	}
	else if(sPathAlias.Compare(_T("예약대출")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReservationLoan();
	}
	else if(sPathAlias.Compare(_T("예약취소")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReservationCancel();
	}
	else if(sPathAlias.Compare(_T("예약상태")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ReservationStatus();
	}
	else if(sPathAlias.Compare(_T("취소")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ExcuteUNDO();
	}
	else if(sPathAlias.Compare(_T("반납연기")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = DelayLoanDate();
	}
	else if(sPathAlias.Compare(_T("화면초기화")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = InitDM();
		Send3000ApiSocket(_T("초기화"));
	}
	else if(sPathAlias.Compare(_T("이용자자료찾기")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = SearchBookInfo();
	}
	else if(sPathAlias.Compare(_T("대출_가족회원")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ViewFamilyMember();
	}
	else if(sPathAlias.Compare(_T("이용자_상세정보수정")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = EditLoanUserInfo();
	}
	else if(sPathAlias.Compare(_T("이용자_비고수정")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = SaveLoanUserEtcInfo();
	}
	else if(sPathAlias.Compare(_T("반납된자료예약만기일부여")) == 0)
	{
		m_bSimpleCmd = FALSE;		
		 
		ids = RecordReserveExpireDate();
	}
	else if(sPathAlias.Compare(_T("바코드업무")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = InputBarCode();
	}
	else if(sPathAlias.Compare(_T("화면로드_자동기능수행")) == 0) 
	{
		m_bSimpleCmd = FALSE;
		ids = RunAutoProc();
	}
	else if(sPathAlias.Compare(_T("상세종정보")) == 0) 
	{
		m_bSimpleCmd = FALSE;
		ids =  ViewDetail();
	}
	else if(sPathAlias.Compare(_T("영수증출력")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = Receipt_Print(0);
	}
	else if(sPathAlias.Compare(_T("영수증재발급")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = Receipt_Print(1);
	}
	else if(sPathAlias.Compare(_T("영수증출력체크")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = Receipt_PrintCheck();		
	}	
	else if(sPathAlias.Compare(_T("이용자검색바구니초기화")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = ClearSearchUserBasket();		
	}	
	else if(sPathAlias.Compare(_T("검색이용자바구니추가")) == 0)
	{
		m_bSimpleCmd = FALSE;		
		ids = AddSearchUserBasket();
	}
	else if(sPathAlias.Compare(_T("대출정지일해제")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = LoanStopDateCancel();		
	}
	
	else if(sPathAlias.Compare(_T("책두레대출화면")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = 0;
		
		CILL_LOAN Dlg(m_pParentMgr);
		Dlg.m_pInfo = m_pParentMgr->m_pInfo;
		Dlg.m_pLoan = this;
		Dlg.m_pManageValue = m_pManageValue;
		Dlg.m_pPreRegNoMgr = m_pPreRegNoMgr;

		Dlg.DoModal(); 
	}
	else if(sPathAlias.Compare(_T("책두레반납화면")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = 0;
	
		CILL_RETURN Dlg(m_pParentMgr);
		Dlg.m_pInfo = m_pParentMgr->m_pInfo;
		Dlg.m_pLoan = this;
		Dlg.m_pPreRegNoMgr = m_pPreRegNoMgr;
		Dlg.DoModal();
	}
	
	
	else if(sPathAlias.Compare(_T("책두레_전환")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = ILL_ReservationConvertToAPL();
	}
	else if(sPathAlias.Compare(_T("부록대출")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = AppendixLoanProc();		
	}
	else if(sPathAlias.Compare(_T("통합센터대출이력조회")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = KL_SearchLoanHistoryInfo();
	}
	else if(sPathAlias.Compare(_T("타관반납관리")) == 0)
	{
		m_bSimpleCmd = FALSE;
		ids = KL_OtherReturnMgr();
	}
	
	else if(sPathAlias.Compare(_T("RFID_대출")) == 0) 
	{	
		m_bSimpleCmd = TRUE;		
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = RFID_Loan();	
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();
			InitGridColor();
			ViewGrid();
		}
	}
	else if(sPathAlias.Compare(_T("RFID_반납")) == 0) 
	{
		m_bSimpleCmd = TRUE;		
		if(m_bSimpleGrid == TRUE) 
		{	
			m_pParentMgr->LockWindowUpdate();		
			pGrid->LockWindowUpdate();
			pGrid2->LockWindowUpdate();
		}
		ids = RFID_Return();
		if(m_bSimpleGrid == TRUE) 
		{
			m_pParentMgr->UnlockWindowUpdate();			
			pGrid->UnlockWindowUpdate();
			pGrid2->UnlockWindowUpdate();
			InitGridColor();
			ViewGrid();
		}
	}
	else if(sPathAlias.Compare(_T("RFID_회원증등록")) == 0) 
	{
		m_bSimpleCmd = FALSE;		
		ids = RecordRFIDUserInfo();
	}

	if(0 > ids)
	{
		ViewMessage(_T("기능을 정상적으로 수행하지 못하였습니다."));
		return -1;
	}	

	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), sPathAlias, (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	TRACE(strMsg.GetBuffer(0));
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->QueryLog(strMsg.GetBuffer(0));	
	
	return ids;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchLoanInfo(BOOL bAskPasswd/*=TRUE*/, CString strUserKey/*NULL*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SearchLoanInfo");
	
	StartLog(_T("회원검색:쿼리생성"));

	m_strUserNo = _T("");
	
	CString sWhere;
	
	MakeSearchMemberSql(sWhere, strUserKey);
	
	if(sWhere.IsEmpty()) 
	{
		EndLog();
		return 1;
	}
	EndLog();
	
	StartLog(_T("회원검색:이용자정보얻기"));
	ids = SearchLoanInfo(_T(""), sWhere, bAskPasswd);
	if(0 > ids) return ids;
	EndLog();
	if(ids == 1)
	{
		SelfCloseMessageBox(_T("존재하지 않는 이용자입니다."));
		
		SPFExcutePath(_T("화면초기화"));
		return 1;
	}
	
	if(ids == 2)
	{
		
		SPFExcutePath(_T("화면초기화"));
		return 1;
	}

	CString sMemberState;
	CString sLostUserCard;
	CString sArrear;	
	
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM(sMemberDMAlias);
	ids = pTmpDM->GetCellData(_T("회원상태"), 0, sMemberState);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(NULL != m_lp3000ApiSocket)
	{
		CString strUserKey = _T("");
		pTmpDM->GetCellData(_T("대출자KEY"), 0, strUserKey);
		
		Send3000ApiSocket(_T("회원_검색"), strUserKey);
	}
	
	ids = pTmpDM->GetCellData(_T("회원증분실"), 0, sLostUserCard);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);	

	
	CString sLoanUserKey;
	pTmpDM->GetCellData(_T("대출자KEY"), 0, sLoanUserKey);
	if(_T("Y") == m_pManageValue->m_ArrearCheckYN)
	{
		pTmpDM->GetCellData(_T("총연체료"), 0, sArrear);
		INT nArrear = _ttoi(sArrear);

		if(0 < nArrear && !sLoanUserKey.IsEmpty())
		{
			CArrearInfoDlg Arreardlg(m_pParentMgr);
			Arreardlg.m_strIsIncludeLoanCnt = m_pManageValue->m_sIsIncludeLoanCnt;
			Arreardlg.m_strUserKey = sLoanUserKey;
			Arreardlg.m_strLoanStopDate = pTmpDM->GetCellData(_T("대출정지일"), 0);
			Arreardlg.DoModal();
		}
	}

	if(sMemberState == _T("1"))
	{
		if(sLostUserCard == _T("Y"))
		{
			SelfCloseMessageBox(_T("분실된 회원카드 입니다."), m_pParentMgr);
		}
		else
		{
			SelfCloseMessageBox(_T("검색된 이용자는 대출정지회원입니다."), m_pParentMgr);
		}
	}
	else if(sMemberState == _T("2"))
	{
		SelfCloseMessageBox(_T("검색된 이용자는 제적상태입니다."), m_pParentMgr);
	}
	
	else if(sMemberState == _T("3"))
	{
		SelfCloseMessageBox(_T("검색된 이용자는 탈퇴신청회원입니다."), m_pParentMgr);
	}
	else 
	{
		if(sLostUserCard == _T("Y"))
		{
			SelfCloseMessageBox(_T("분실된 회원카드 입니다."), m_pParentMgr);
		}
		else
		{
			SelfCloseMessageBox(_T("이용자가 검색되었습니다."), m_pParentMgr, 2000);	
		}
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::SearchLoanInfo(CString sLoanUserKey, CString sSearchWhere, BOOL bAskPasswd/*=TRUE*/)
{
EFS_BEGIN

	INT ids;	
	CString strFuncName = _T("SearchLoanInfo");

	this->SetBEGIN_SBL();

	CString sWhere, sLoanStopDate, strQuery, strCardPasswd = _T("");
	CESL_DataMgr* pMemberDM;
	
	sWhere.Format(_T("CLU.REC_KEY = %s "), sLoanUserKey);

	if(sSearchWhere.IsEmpty() == FALSE)
	{
		sWhere = sSearchWhere;
	}

	BOOL bSearchUser = TRUE;
	if(FALSE == sLoanUserKey.IsEmpty() && sSearchWhere.IsEmpty() && 2 == bAskPasswd)
	{
		CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(1 == pTmpDM->GetRowCount())
		{
			CString strUserKey;
			pTmpDM->GetCellData(_T("대출자KEY"), 0, strUserKey);
			if(sLoanUserKey == strUserKey)
			{
				bSearchUser = FALSE;
			}
		}
	}

	if(bSearchUser)
	{
		ids = SearchMemberInfo(sWhere);
		if(0 > ids)	return ids;
		
		if(ids > 1)		
		{
 			CLoanUserListDlg user(m_pParentMgr, sWhere);
			user.m_sIsUnityLoanService = m_pManageValue->m_sIsUnityLoanService;
			user.m_strUserAccessAuth = m_pManageValue->m_sLoanUserAccessMode;
 			user.DoModal();
						
			CString sUserkey;
			ids = user.GetSelectUserKey(sUserkey);
			
			
			if(ids == 1)	return 2;
		}	
		else if(ids < 1)	
		{
 			return 1;
		}
	}

	if(bAskPasswd == TRUE)
	{
		if(m_strPinpadYN == _T("Y"))
		{
			CString strMainMemberClass;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("회원구분"), strMainMemberClass, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			if(_T("2") != strMainMemberClass)
			{				
				CCardpasswdDlg dlg(m_pParentMgr);

				pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
				strCardPasswd = pMemberDM->GetCellData(_T("회원증비밀번호"), 0);

				if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && !strCardPasswd.IsEmpty())
				{
					CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
					if(NULL != pApi)
					{
						if(TRUE == pApi->ConnectSocket()) 
						{
							CStringArray DataArray;
							DataArray.Add(strCardPasswd);
							if(TRUE == pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), DataArray))
							{
								strCardPasswd = DataArray.GetAt(0);
							}
							DataArray.RemoveAll();
							DataArray.FreeExtra();
							pApi->CloseSocket();	
						}
						delete pApi;
						pApi = NULL;
					}
				}

				INT		nLength		=	strCardPasswd.GetLength();

				if(strCardPasswd.IsEmpty() == TRUE)
				{
					AfxMessageBox(_T("비밀번호가 입력되지 않았습니다.\n\n새로운 비밀번호를 등록 하세요"));
				}				
				
				else if((4 > nLength || 20 < nLength ) && FALSE == m_pParentMgr->m_pInfo->m_bCardPwEncryption)
				{
					AfxMessageBox(_T("잘못된 비밀번호가 입력되었습니다.\n\n새로운 비밀번호를 등록 하세요"));
				}
				else
				{
					dlg.m_strCardpasswd = strCardPasswd;
				}

				if(IDOK != dlg.DoModal())	
				{
					return 2;
				}
				else
				{
					switch(dlg.m_nStatus)
					{
						case 0:
							break;
						case 1:
							{
								
								if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
								{
									CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
									if(NULL != pApi)
									{
										if(TRUE == pApi->ConnectSocket()) 
										{
											CStringArray DataArray;
											DataArray.Add(dlg.m_strCardpasswd);
											if(TRUE == pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), DataArray))
											{
												dlg.m_strCardpasswd = DataArray.GetAt(0);
											}
											DataArray.RemoveAll();
											DataArray.FreeExtra();
											pApi->CloseSocket();
										}
										delete pApi;
										pApi = NULL;
									}
								}
														
								CString strLastWork = m_pParentMgr->GetWorkLogMsg(_T("회원증비밀번호수정"), __WFILE__, __LINE__);
								strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET CARD_PASSWORD='%s',LAST_WORK='%s' WHERE REC_KEY=%s;"), dlg.m_strCardpasswd, strLastWork, pMemberDM->GetCellData(_T("대출자KEY"), 0));
								pMemberDM->StartFrame();
								pMemberDM->AddFrame(strQuery);

								ids = pMemberDM->SendFrame();
								if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
								pMemberDM->EndFrame();
							}
							break;
					}
				}
			}	
		}
	}
	
	ids = SearchLoanBookInfo();
	if(0 > ids) return ids;

	
	ids = AddMemberInfo();
	if(0 > ids) return ids;

	
	ids = ViewLoanScreen();
	if(0 > ids) return ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	INT nRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nRowCount);

	
	ids = BasketRegNoProc();
	if(0 > ids) return ids;

	
	if(m_ReceiptDlg != NULL) m_ReceiptDlg->initData();

	
	ids = AddSearchUserBasket();
	if(0 > ids) return ids;

	ids = ViewSimpleFamilyMember();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchMemberInfo(CString &sWhere)
{
EFS_BEGIN
	
	INT i, ids;
	CString strFuncName = _T("SearchMemberInfo");

	DWORD nSTART,nFINISH;
	CString sLoanStopDate, strMsg, sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pMemberDM, *pDM;

	pMemberDM = m_pParentMgr->FindDM(sMemberDMAlias);
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	if(m_pManageValue->m_sIs64lib == _T("Y"))
	{
	
		CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE;
		for(i=0; i<pMemberDM->GetColCount(); i++)
		{
			pMemberDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
			if(FIELD_ALIAS == _T("학과"))
			{
				pMemberDM->SetDataMgrRef(i, _T("학과"), _T("COURSE"), _T("str"), _T(""), _T(""));
			}
			else if(FIELD_ALIAS == _T("학부"))
			{
				pMemberDM->SetDataMgrRef(i, _T("학부"), _T("CLASS_GROUP"), _T("str"), _T(""), _T(""));
			}
		}
	}

	nSTART = GetTickCount();

	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pMemberDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출정지일"), sLoanStopDate, 0);	

	nFINISH = GetTickCount();
	
	strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), _T("SearchMemberInfo"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->QueryLog(strMsg.GetBuffer(0));

	
	CStringArray asDes1,asDes2,asDes3,asDes4,asDes5,asDes6,asDes7;

	
	for(i = 0; i < pMemberDM->GetRowCount(); i++)
	{
		CString sCode, sDesc;
		pMemberDM->GetCellData(_T("대출자소속"), i, sCode);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출소속정보"), sCode, sDesc);
		pMemberDM->SetCellData(_T("대출자소속설명"), sDesc, i);

		pMemberDM->GetCellData(_T("대출자직급"), i, sCode);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("대출직급정보"), sCode, sDesc);
		pMemberDM->SetCellData(_T("대출자직급설명"), sDesc, i);

		pMemberDM->GetCellData(_T("관리구분"), i, sCode);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("관리구분"), sCode, sDesc);
		pMemberDM->SetCellData(_T("관리구분설명"), sDesc, i);

	
		sDesc = _T("");
		pMemberDM->GetCellData(_T("회원상태"), i, sCode);	
		if(sCode.Compare(_T("0")) == 0)
			sDesc = _T("정상회원");
		else if(sCode.Compare(_T("1")) == 0)
			sDesc = _T("대출정지");
		else if(sCode.Compare(_T("2")) == 0)
			sDesc = _T("제적회원");
		else if(sCode.Compare(_T("3")) == 0)
			sDesc = _T("탈퇴회원");
		pMemberDM->SetCellData(_T("회원상태설명"), sDesc, i);

		
		sDesc = _T("");
		pMemberDM->GetCellData( _T("회원구분"), i, sCode);
		if( sCode.Compare(_T("0"))==0 )
			sDesc = _T("정회원");
		else if( sCode.Compare(_T("1"))==0 )
			sDesc = _T("비회원");
		else if( sCode.Compare(_T("2"))==0 )
			sDesc = _T("준회원");
		pMemberDM->SetCellData( _T("회원구분설명"),  sDesc, i);

		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			
			CString strGetData;
			pMemberDM->GetCellData(_T("주민등록번호"), i, strGetData);
			asDes1.Add(strGetData);
			pMemberDM->GetCellData(_T("이메일"), i, strGetData);
			asDes2.Add(strGetData);
			pMemberDM->GetCellData(_T("휴대폰"), i, strGetData);
			asDes3.Add(strGetData);
			pMemberDM->GetCellData(_T("대출자주소"), i, strGetData);
			asDes4.Add(strGetData);
			pMemberDM->GetCellData(_T("자택전화"), i, strGetData);
			asDes5.Add(strGetData);
			pMemberDM->GetCellData(_T("근무지전화"), i, strGetData);
			asDes6.Add(strGetData);
			pMemberDM->GetCellData(_T("회원증비밀번호"), i, strGetData);
			asDes7.Add(strGetData);
		}
	}
	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < pMemberDM->GetRowCount())
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes2);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes3);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes4);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes5);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes6);
					pApi->CloseSocket();
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CARD_PASSWORD"), asDes7);
					pApi->CloseSocket();
				}
				for(i = 0; i < pMemberDM->GetRowCount(); i++)
				{
					pMemberDM->SetCellData(_T("주민등록번호"), asDes1.GetAt(i), i);
					pMemberDM->SetCellData(_T("이메일"), asDes2.GetAt(i), i);
					pMemberDM->SetCellData(_T("휴대폰"), asDes3.GetAt(i), i);
					pMemberDM->SetCellData(_T("대출자주소"), asDes4.GetAt(i), i);
					pMemberDM->SetCellData(_T("자택전화"), asDes5.GetAt(i), i);
					pMemberDM->SetCellData(_T("근무지전화"), asDes6.GetAt(i), i);
					pMemberDM->SetCellData(_T("회원증비밀번호"), asDes7.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
		asDes3.RemoveAll(); asDes3.FreeExtra();
		asDes4.RemoveAll(); asDes4.FreeExtra();
		asDes5.RemoveAll(); asDes5.FreeExtra();
		asDes6.RemoveAll(); asDes6.FreeExtra();
		asDes7.RemoveAll(); asDes7.FreeExtra();
	}
	
	return pMemberDM->GetRowCount();

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchLoanBookInfo()
{
EFS_BEGIN

	INT ids;  
	CString strFuncName = _T("SearchLoanBookInfo");

	CString sLoanUserRecKey;
	CString sWhere;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sLoanUserRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	sWhere.Format(_T("CL.USER_KEY=%s"), sLoanUserRecKey);   
	
	CString sUnionQuery; 
	CString sBOVol,sSEVol;
	CString sBookCode		= _T("");
	CString sClassNo		= _T("");
	CString sCopyCode		= _T("");
	CString sManageCode		= _T("");
	CString sPrevWorkStat	= _T("");
	CString sSeShelfCode	= _T("");
	CString sVolCode		= _T("");
	CString sWorkStat		= _T("");
	CString sBOTitle,sSETitle;
	CString sLoanTypeCode;
	CString sAppendixFlag;
	CString sBOReserveUserCnt,sSEReserveUserCnt,sILLReserveUserCnt;
	if(m_pManageValue->m_bVolReserve)
	{ 
		sBookCode		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.BOOK_CODE)");
		sClassNo		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.CLASS_NO)");
		sCopyCode		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.COPY_CODE)");
		sPrevWorkStat	= _T("DECODE(CL.REG_NO,NULL,NULL,BB.PREV_WORKING_STATUS)");
		sSeShelfCode	= _T("DECODE(CL.REG_NO,NULL,NULL,BB.SEPARATE_SHELF_CODE)");
		sVolCode		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.VOL_CODE)");
		sWorkStat		= _T("DECODE(CL.REG_NO,NULL,NULL,BB.WORKING_STATUS)");
		sBOReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 ")
									_T("WHERE CL2.SPECIES_KEY=BB.SPECIES_KEY AND ((CL2.VOL_INFO=BB.VOL_SORT_NO) OR (CL2.VOL_INFO IS NULL AND BB.VOL_SORT_NO IS NULL)) AND CL2.STATUS='3')"));
		sSEReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 ")
									_T("WHERE CL2.SPECIES_KEY=BB.SPECIES_KEY AND CL2.VOL_INFO=BB.VOL_KEY AND CL2.STATUS='3')"));
		sILLReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2 ")
									_T("WHERE CL2.SPECIES_KEY=CL.SPECIES_KEY AND ((CL2.VOL_INFO=CL.VOL_INFO) OR (CL2.VOL_INFO IS NULL AND CL.VOL_INFO IS NULL)) AND CL2.STATUS='3')"));
	}
	else
	{ 
		sBookCode		= _T("BB.BOOK_CODE");
		sClassNo		= _T("BB.CLASS_NO");
		sCopyCode		= _T("BB.COPY_CODE");
		sPrevWorkStat	= _T("BB.PREV_WORKING_STATUS");
		sSeShelfCode	= _T("BB.SEPARATE_SHELF_CODE");
		sVolCode		= _T("BB.VOL_CODE");
		sWorkStat		= _T("BB.WORKING_STATUS");
		sBOReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2")
									_T(" WHERE CL2.BOOK_KEY=BB.REC_KEY AND CL2.STATUS='3')"));
		sSEReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2")
									_T(" WHERE CL2.BOOK_KEY=BB.REC_KEY AND CL2.STATUS='3')"));
		sILLReserveUserCnt.Format(	_T("(SELECT COUNT(1) FROM LS_WORK_T01 CL2")
									_T(" WHERE CL2.BOOK_KEY=CL.BOOK_KEY AND CL2.STATUS='3')"));
	}
	sManageCode		= _T("BB.MANAGE_CODE");
	sBOVol			= _T("DECODE(BB.BOOK_APPENDIX_FLAG,'A',NULL,'B',CONCAT(CONCAT(BB.VOL, '/'),BB.VOL_TITLE))");
	sBOTitle		= _T("DECODE(BB.BOOK_APPENDIX_FLAG,'A',BB.VOL_TITLE,'B',IB.TITLE_INFO)");
	sLoanTypeCode	= _T("DECODE(CL.LOAN_TYPE_CODE,'0','일반','1','특별','2','관내','3','무인','4','장기')");
	sSEVol			= _T("DECODE(BB.BOOK_TYPE,'A',V.VOL_TITLE,'B',V.VOL_TITLE,'C',V.VOL_TITLE)");
	sSETitle		= _T("DECODE(BB.BOOK_TYPE,'A',IB.TITLE_INFO,'B',IB.TITLE_INFO,'C',IB.TITLE_INFO)");
	sAppendixFlag	= _T("DECODE(CL.LOAN_KEY,NULL,'B','A')");
	
	CString strOderBy, strValue;
	ids = m_pManageValue->GetManageValue(_T("기타"), _T("공통"), _T("정렬기준"), _T("현황정렬기준"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	strValue.TrimLeft(); strValue.TrimRight();
	if(strValue.IsEmpty())
	{
		strValue = _T("0");
	}
	if(strValue == _T("1"))
	{
		strOderBy = _T("ORDER BY SORT_MANAGE_CODE, STATUS, RETURN_PLAN_DATE, PK");
	}
	else
	{
		strOderBy = _T("ORDER BY STATUS, RETURN_PLAN_DATE, PK");
	}

	
	sUnionQuery.Format(	
	
	_T("SELECT BB.BOOK_APPENDIX_FLAG,%s,%s,%s,%s,%s,BB.PRICE,BB.REC_KEY,CL.REG_NO,%s,%s,%s,")
		_T("CL.PUBLISH_FORM_CODE,CL.REC_KEY AS PK,TO_CHAR(CL.RESERVATION_EXPIRE_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AS RETURN_PLAN_DATE,CL.SHELF_LOC_CODE,CL.STATUS,")
		_T("%s,%s,IB.REC_KEY,TO_CHAR(CL.LOAN_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RESERVATION_DATE,'YYYY/MM/DD HH24:MI'),CL.LOAN_MANAGE_CODE,%s,BB.WORKNO,")
		_T("CL.DELAY_CNT,%s,BB.MEDIA_CODE,BB.VOL_SORT_NO,BB.REG_CODE,CL.UNMANNED_RESERVATION_LOAN,") 
		_T("CL.L_DEVICE,CL.R_DEVICE,CL.L_WORKER,CL.R_WORKER,NULL,")
		_T("(SELECT DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE = CL.SHELF_LOC_CODE AND ROWNUM=1), TO_CHAR(CL.CREATE_DATE, 'YYYY/MM/DD HH24:MI:SS'),")
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")		
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = BB.MANAGE_CODE AND ROWNUM = 1), ")
		_T("(SELECT DECODE(COUNT(1),0,'N','Y') FROM ILL_TRANS_TBL WHERE LOAN_KEY = CL.REC_KEY AND ROWNUM = 1), ")
		_T("(SELECT MAX(L.LIB_NAME) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")
		_T("CL.LILL_LOAN_YN,CL.LCT_KEY,CL.APPENDIX_SEND_YN,CL.APPENDIX_REGNO_LIST, H_LIB_CODE, ")
		_T("REPLACE(BB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE ")			
	_T("FROM LS_WORK_T01 CL, BO_BOOK_TBL BB, IDX_BO_TBL IB ")
	_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
		_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
		_T("AND CL.STATUS IN('0','2','3','L','D') ")
		_T("AND CL.PUBLISH_FORM_CODE='MO' ")
		_T("AND %s")
	_T(" UNION ")	
	_T("SELECT BB.BOOK_TYPE,%s,%s,%s,%s,%s,V.PRICE,BB.REC_KEY,CL.REG_NO,%s,%s,%s,")
		_T("CL.PUBLISH_FORM_CODE,CL.REC_KEY AS PK,TO_CHAR(CL.RESERVATION_EXPIRE_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AS RETURN_PLAN_DATE,CL.SHELF_LOC_CODE,CL.STATUS,")
		_T("%s,%s,IB.REC_KEY,TO_CHAR(CL.LOAN_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RESERVATION_DATE,'YYYY/MM/DD HH24:MI'),CL.LOAN_MANAGE_CODE,%s,BB.WORKNO,")
		_T("CL.DELAY_CNT,%s,V.MEDIA_CODE,BB.VOL_KEY,BB.REG_CODE,CL.UNMANNED_RESERVATION_LOAN,")
		_T("CL.L_DEVICE,CL.R_DEVICE,CL.L_WORKER,CL.R_WORKER,NULL,")
		_T("(SELECT DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE = CL.SHELF_LOC_CODE AND ROWNUM=1),TO_CHAR(CL.CREATE_DATE, 'YYYY/MM/DD HH24:MI:SS'),")
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = BB.MANAGE_CODE AND ROWNUM = 1), ")
		_T("(SELECT DECODE(COUNT(1),0,'N','Y') FROM ILL_TRANS_TBL WHERE LOAN_KEY = CL.REC_KEY AND ROWNUM = 1), ")
		_T("(SELECT MAX(L.LIB_NAME) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")
		_T("CL.LILL_LOAN_YN,CL.LCT_KEY,CL.APPENDIX_SEND_YN,CL.APPENDIX_REGNO_LIST, H_LIB_CODE, ")
		_T("REPLACE(BB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE ")				
	_T("FROM LS_WORK_T01 CL, SE_BOOK_TBL BB, IDX_SE_TBL IB, SE_VOL_TBL V ")
	_T("WHERE CL.BOOK_KEY=BB.REC_KEY ")
		_T("AND BB.SPECIES_KEY=IB.REC_KEY ")
		_T("AND BB.VOL_KEY=V.REC_KEY ")
		_T("AND CL.STATUS IN('0','2','3','L','D') ")
		_T("AND CL.PUBLISH_FORM_CODE='SE' ")
		_T("AND %s")
	_T(" UNION ")
	_T("SELECT 'B',NULL,NULL,NULL,NB.MANAGE_CODE,NB.PREV_WORKING_STATUS,NB.PRICE,NB.REC_KEY,CL.REG_NO,")
		_T("NB.SEPARATE_SHELF_CODE,NULL,NB.WORKING_STATUS,CL.PUBLISH_FORM_CODE,CL.REC_KEY AS PK,")
		_T("TO_CHAR(CL.RESERVATION_EXPIRE_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RETURN_PLAN_DATE,'YYYY/MM/DD') AS RETURN_PLAN_DATE,CL.SHELF_LOC_CODE,CL.STATUS,")
		_T("NULL,NB.TITLE,1,TO_CHAR(CL.LOAN_DATE,'YYYY/MM/DD HH24:MI'),")
		_T("TO_CHAR(CL.RESERVATION_DATE,'YYYY/MM/DD HH24:MI'),CL.LOAN_MANAGE_CODE,0,'',CL.DELAY_CNT,")
		_T("%s,NULL,NULL,NULL,CL.UNMANNED_RESERVATION_LOAN,CL.L_DEVICE,CL.R_DEVICE,CL.L_WORKER,")
		_T("CL.R_WORKER,CL.CALL_NO,")
		_T("(SELECT DESCRIPTION FROM CD_CODE_TBL WHERE CLASS = 19 AND CODE = CL.SHELF_LOC_CODE AND ROWNUM=1),TO_CHAR(CL.CREATE_DATE, 'YYYY/MM/DD HH24:MI:SS'),")
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")		
		_T("(SELECT MAX(L.LIB_CODE) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = NB.MANAGE_CODE AND ROWNUM = 1), ")
		_T("(SELECT DECODE(COUNT(1),0,'N','Y') FROM ILL_TRANS_TBL WHERE LOAN_KEY = CL.REC_KEY AND ROWNUM = 1), ")
		_T("(SELECT MAX(L.LIB_NAME) FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE = CL.MANAGE_CODE AND ROWNUM = 1), ")
		_T("CL.LILL_LOAN_YN,CL.LCT_KEY,CL.APPENDIX_SEND_YN,CL.APPENDIX_REGNO_LIST, '', ")
		_T("REPLACE(NB.MANAGE_CODE,UDF_MANAGE_CODE,'1') AS SORT_MANAGE_CODE ")			
	_T("FROM LS_WORK_T01 CL, CO_NON_DB_BOOK_TBL NB ")
	_T("WHERE CL.BOOK_KEY=NB.REC_KEY AND CL.STATUS IN('0','2','3','L','D') ")
		_T("AND CL.PUBLISH_FORM_CODE='NB' ")
		_T("AND %s ")
	_T("%s"),	
	sBookCode, sClassNo, sCopyCode, sManageCode, sPrevWorkStat,	sSeShelfCode, sVolCode, 
	sWorkStat, sBOVol, sBOTitle, sBOReserveUserCnt, sLoanTypeCode, sWhere,		
	sBookCode, sClassNo, sCopyCode, sManageCode, sPrevWorkStat, sSeShelfCode, sVolCode, 
	sWorkStat, sSEVol, sSETitle, sSEReserveUserCnt, sLoanTypeCode, sWhere, 	
	sLoanTypeCode, sWhere, 
	strOderBy);	
	
	DWORD nSTART,nFINISH;
	nSTART = GetTickCount();

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pParentMgr->m_pInfo->MODE));
	ids = TmpDM.RestructDataManager(sUnionQuery);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	CString sLoanBookDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanBookDM = m_pParentMgr->FindDM(sLoanBookDMAlias);
	if(NULL == pLoanBookDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	if(!m_bReturnBookNonFree)
	{ 
		CESL_DataMgr* pReturnBookDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));
		if(pReturnBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		pReturnBookDM->FreeData();
	}
	m_bReturnBookNonFree = FALSE;

	CString sData;

	const INT nColCnt = 48;
	CString sCAlias[nColCnt] =
	{
		_T("부록여부")				, _T("IBS_청구_도서기호")	, _T("IBS_청구_분류기호")	, 
		_T("IBS_청구_복본기호")		, _T("관리구분")			, _T("이전자료상태")		, 
		_T("책값")					, _T("책KEY")				, _T("등록번호")			, 
		_T("IBS_청구_별치기호_코드"), _T("IBS_청구_권책기호")	, _T("상태")				, 
		_T("단행/연속구분")			, _T("대출/반납정보KEY")	, _T("예약만기일")			,
		_T("반납예정일")			, _T("자료실")				, _T("대출상태")			, 
		_T("권호명")				, _T("본표제")				, _T("종KEY")				, 
		_T("대출일")				, _T("예약일")				, _T("대출장소")			, 
		_T("예약이용자수")			, _T("RFID시리얼")			, _T("반납연기횟수")		, 
		_T("대출형태")				, _T("매체구분")			, _T("권호정보KEY")			,
		_T("등록구분")				, _T("무인대출예약상태")	, _T("L_DEVICE")			, 
		_T("R_DEVICE")				, _T("L_WORKER")			, _T("R_WORKER")			,
		_T("청구기호")				, _T("자료실_설명")						, _T("생성일")					,
		_T("대출도서관부호")		, _T("소장관도서관부호")				, _T("책두레데이터여부")		, 
		_T("대출도서관")			, _T("타관대출상태")					, _T("LCT_KEY")					,
		_T("부록발송유무")			, _T("부록등록번호리스트")				, _T("제공도서관부호")			

	};

	CString strManageCode, strLocalKey;
	pLoanBookDM->FreeData();

	for(INT i = 0; i < TmpDM.GetRowCount(); i++)
	{
		pLoanBookDM->InsertRow(-1);
		for(INT j = 0; j < nColCnt; j++)
		{
			sData.Empty();
			ids = TmpDM.GetCellData(i, j, sData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			ids = pLoanBookDM->SetCellData(sCAlias[j], sData, i);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			
			if ( _tcscmp(sCAlias[j], _T("LCT_KEY")) == 0 && sData.IsEmpty() == FALSE ) {
					ids = pLoanBookDM->SetCellData(_T("타관대출KEY"), sData, i);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
				}				
			if ( _tcscmp(sCAlias[j], _T("제공도서관부호")) == 0 && sData.IsEmpty() == FALSE) {
				ids = pLoanBookDM->SetCellData(_T("소장관도서관부호"), sData, i);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			}
		}
		pLoanBookDM->SetCellData(_T("현황구분"), _T("로컬"), i);
		strLocalKey = pLoanBookDM->GetCellData(_T("대출/반납정보KEY"), i);
		pLoanBookDM->SetCellData(_T("로컬KEY"), strLocalKey, i);

	}
	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), sLoanBookDMAlias, (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	pLoanBookDM->QueryLog(strMsg.GetBuffer(0));
	
	ids = MakeShelfCode(sLoanBookDMAlias);
	if(0 > ids) return ids;
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		CString strGetData;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, _T("SearchLoanBookInfo"));
		if(_T("Y") == strGetData)
		{
				ids = KL_UserLoanInfo(strGetData);
				if(0 > ids) return ids;
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::FreeLoanInfoDM()
{
EFS_BEGIN

	CString strFuncName = _T("FreeLoanInfoDM");

	CString sLoanBookDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanBookDM = m_pParentMgr->FindDM(sLoanBookDMAlias);
	if(pLoanBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pLoanBookDM->FreeData();

	if(!m_bReturnBookNonFree)
	{ 
		CESL_DataMgr* pReturnBookDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));
		if(pReturnBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		pReturnBookDM->FreeData();
	}
	m_bReturnBookNonFree = FALSE;
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddLoanInfo(TCHAR **sSrcLoanBookInfoFieldAlias, CESL_DataMgr *pSrcDM)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddLoanInfo");

	const INT nCopyColumnCnt = 42;
	TCHAR *sSrcLoanInfoFieldAlias[nCopyColumnCnt] = 
	{
		_T("대출/반납정보KEY"), _T("종KEY"), _T("책KEY"), _T("등록번호"), _T("MARC"),
		_T("대출일"),_T("반납예정일"),_T("예약일"),_T("예약만기일"),_T("연체일수"),
		_T("대출상태"),				_T("자료실"),				_T("단행/연속구분"),		_T("IBS_청구_별치기호_코드"),	_T("IBS_청구_분류기호"),
		_T("IBS_청구_도서기호"),	_T("IBS_청구_권책기호"),	_T("IBS_청구_복본기호"),	_T("책값"),						_T("본표제"),
		_T("부록자료수"),			_T("반납연기횟수"), 		_T("이전자료상태"),			_T("상태"),						_T("권호명"), 
		_T("관리구분"),				_T("부록여부"),				_T("이전자료상태"),			_T("대출장소"),					_T("RFID시리얼"),
		_T("매체구분"),				_T("대출형태"),				_T("권호정보KEY"),			_T("예약이용자수"),				_T("L_DEVICE"), 
		_T("R_DEVICE"),				_T("L_WORKER"),				_T("R_WORKER"),				_T("청구기호"),					_T("생성일"),
		_T("소장관도서관부호"),		_T("대출도서관")
	};

	CString sLoanBookDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanBookDM = m_pParentMgr->FindDM(sLoanBookDMAlias);
	if(pLoanBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);


	CString sTmpSrcData;
	CString sAppendixCnt;	
	CString sSql;			
	INT nInsertRowIndex;
	for(INT i = 0 ; i < pSrcDM->GetRowCount() ; i++)
	{
		for(INT j = 0 ; j < nCopyColumnCnt ; j++)
		{			
			if(!(m_pManageValue->m_bVolReserve) && (32==j || 33==j))
				continue;

			sTmpSrcData.Empty();
			ids = pSrcDM->GetCellData(sSrcLoanBookInfoFieldAlias[j], i, sTmpSrcData);
			if(0 > ids)
			{
				CString sEMsg;
				sEMsg.Format(_T("pSrcDM->GetCellData(%s,%d,sTmpSrcData)"), sSrcLoanBookInfoFieldAlias[j], i);
				AfxMessageBox(sEMsg);
			}
			if(0 > ids&& ids != -4012) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(j == 0)
			{
				ids = pLoanBookDM->InsertRow(-1);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
				nInsertRowIndex = pLoanBookDM->GetRowCount() -1;
			}

			ids = pLoanBookDM->SetCellData(sSrcLoanInfoFieldAlias[j], sTmpSrcData, nInsertRowIndex);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewLoanScreen(INT nMode/*=-1*/)
{
EFS_BEGIN	
	
	INT ids;
	DWORD nSTART,nFINISH;

	nSTART = GetTickCount();

	if(m_bSimpleGrid == FALSE || m_bSimpleCmd == FALSE) 
	{
		ids = AllControlDisplayNotGrid(nMode);
		if(0 > ids) return ids;
	}
	
	if(nMode == 0)
	{
		ids = AddLoanInfoGrid();
		if(0 > ids) return ids;
	}
	else
	{		
		ids = AllControlDisplayGrid();
		if(0 > ids) return ids;
	}

	if(m_bSimpleGrid == FALSE || m_bSimpleCmd == FALSE) 
	{
		InitGridColor();
		ViewGrid();
	}
	nFINISH = GetTickCount();
	
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), _T("ViewLoanScreen"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->QueryLog(strMsg.GetBuffer(0));	
	
	return 0;

EFS_END
return -1;
}

int CBL_LOC_LOAN_PROC::AllControlDisplayNotGrid(INT nMode/*=-1*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllControlDisplayNotGrid");

	const INT nCMCnt = 2;
	TCHAR *sCMAliasList[nCMCnt] = 
	{
		_T("CM_BO_LOC_3100_MEMBER_INFO"), _T("CM_BO_LOC_3100_SEARCH_INFO")
	};

	CESL_ControlMgr *pCM;
	INT nIndex = -1;
	for(INT i = 0 ; i < nCMCnt ;i++)
	{
		if(i == 0 || i == 1) nIndex = 0;
		else nIndex = -1;

		pCM = m_pParentMgr->FindCM(sCMAliasList[i]);
		if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 

		pCM->AllControlClear();

		ids = m_pParentMgr->AllControlDisplay(sCMAliasList[i], nIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	
	CString strValue1;
	ids = m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();

	if(_T("Y") != strValue1)
	{
		CString strUserNo;	
		pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_MEMBER_INFO"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		ids = pCM->GetControlMgrData(_T("대출자번호"), strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
		ids = pCM->SetControlMgrData(_T("대출자번호"), strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	
	ids = InitUserManageValue();
	if(0 > ids) return ids;
	
	BOOL bDelaySync = FALSE;
	if(-1 != nMode) bDelaySync = TRUE;
	
	CESL_Mgr* pMgr = m_pParentMgr->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		pMgr->SendMessage(WM_USER_LOAD, (WPARAM)bDelaySync, 0);
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::MakeSearchMemberSql(CString &sWhere, CString strUserKey/*_T("")*/)
{
EFS_BEGIN

	INT i, ids;
	CString strFuncName = _T("MakeSearchMemberSql");

	const INT nControlCnt = 3;	
	TCHAR *sFieldAlias[nControlCnt] = 
	{
		_T("대출자이름"), _T("반납연기일"), _T("대출자번호") 
	};
	
	CString tmp, sBuf, sGetData[nControlCnt];
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CMakeSearchData MakeSearchData(m_pParentMgr);
	
	if ( strUserKey.GetLength() > 0 ) {
		if(sWhere.IsEmpty() == FALSE)			sWhere += _T(" AND ");
		tmp.Format(_T(" (CLU.REC_KEY = %s ) "), strUserKey);		
		sWhere += tmp;
		return 0;
	}
	
	for(i=0; i<nControlCnt; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sFieldAlias[i], sGetData[i]);
		if(0 > ids)	ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		sGetData[i].TrimLeft(); sGetData[i].TrimRight();
	}

	if(sGetData[0].IsEmpty() == FALSE && sGetData[2].IsEmpty() == TRUE)
	{
		if(sWhere.IsEmpty() == FALSE)		sWhere += _T(" AND ") ;
		
		MakeSearchData.GetKeyWord(sGetData[0]);
		tmp.Format(_T("CLU.IDX_USER_NAME BETWEEN '%s' AND '%s힝'"), sGetData[0], sGetData[0]);
		sWhere += tmp;		
		return 0;
	}
	
	if(sGetData[2].IsEmpty() == FALSE)
	{	
		sBuf = sGetData[ 2 ];
		
		CString sValue;
		m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출자번호자동부여여부"), sValue);
		sValue.TrimLeft();	sValue.TrimRight();	sValue.MakeUpper();
		if(sValue!=_T("U"))
		{
			sBuf.MakeUpper();
		}
		
		CString strName = sGetData[2];
		MakeSearchData.GetKeyWord(strName);
		
		if(sWhere.IsEmpty() == FALSE)			sWhere += _T(" AND ");

		if( m_pManageValue->m_strScccardNoYN.CompareNoCase(_T("Y")) == 0 ){
			tmp.Format(_T("(CLU.USER_NO = '%s' OR CLU.USER_ID  = '%s' OR (CLU.IDX_USER_NAME = '%s') OR (CLU.SCCCARD_NO = '%s') )"), sGetData[2], sBuf, strName, strName, sGetData[2]);
		} else {			
			tmp.Format(_T("(CLU.USER_NO = '%s' OR CLU.USER_ID  = '%s' OR (CLU.IDX_USER_NAME = '%s'))"), sGetData[2], sBuf, strName, strName);			
		}
		
		sWhere += tmp;

		return 0;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::Loan(INT nMode)
{
EFS_BEGIN
	
	this->SetBEGIN_SBL();	
	
	INT ids;
	BOOL IsLoan = false;
	CString sRegNo;
	
	StartLog(_T("대출:등록번호변환"));
	
	if(2 == nMode)
	{		
		ids = MakeRegNoSpaceToZero(FALSE);
		if(0 > ids) return ids;
	}
	else
	{
	ids = MakeRegNoSpaceToZero(TRUE);
	if(0 > ids) return ids;
	}	
	EndLog();

	StartLog(_T("대출:이용자검색되었는지확인"));
	ids = IsSearchedUser();
	EndLog();
	if(0 > ids) return ids;
	if(ids > 0)
	{		
		if(2 != nMode)
		{
		
		MoveRegNoToNextProcBasket(EXP_LOAN);
		}
		return 0;
	}

	StartLog(_T("대출:대출자검사"));
	ids = IsLoanMember(IsLoan);
	EndLog();
	if(0 > ids) return ids;

	if(!IsLoan) return 0;
	
	ids = IsRegNo(sRegNo);
	if(0 > ids) return ids;
	
	if(sRegNo.IsEmpty())
	{
		SelfCloseMessageBox(_T("대출할 책을 선택해 주십시오"));
		return 0;
	}

	StartLog(_T("대출:대출작업"));
	ids = Loan(sRegNo, nMode);
	EndLog();

	if(1==ids) return ids;
	else if(ids > 0)
	{
		InitExpDM();
		return ids;
	}
	else if(0 > ids)
	{
		ids = InitExpDM();
		return ids;
	}

	if(2 != nMode)
	{
		StartLog(_T("대출:부록대출"));
		ids = AppendixLoan();
		EndLog();
		if(0 > ids) return ids;
	}
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	INT nRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nRowCount);

	ids = InitExpDM();
 	if(0 > ids) return ids;
	
 	StartLog(_T("대출:대출자정보구축"));
 	ids = AddMemberInfo();
 	EndLog();
 	if(0 > ids) return ids;	
 	
 	StartLog(_T("대출:화면구성"));
 	ids = ViewLoanScreen();
 	EndLog();
 	if(0 > ids) return ids;

	SelfCloseMessageBox(_T("대출되었습니다."));
	
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");	
	CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));
	pMainGrid->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);	
	
	if(m_ReceiptDlg != NULL)
	{
		
		m_pParentMgr->PostMessage(WM_RECEIPT_CHANGE_TAB, 0, 1);
	}
	
	return 0;

EFS_END
	return -1;
}


INT CBL_LOC_LOAN_PROC::OtherLoan(INT nMode)
{	
	CString strOtherLoanKey;
	CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));

	CString strLoanBookCnt;

	
	INT nSelectCnt;
	pMainGrid->SelectMakeList();
	nSelectCnt = pMainGrid->SelectGetCount();
	if(0 >= nSelectCnt)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}

	INT ids;
	BOOL IsLoan = FALSE;
	
	StartLog(_T("대출:대출자검사"));
	ids = IsLoanMember(IsLoan);
	EndLog();
	if(0 > ids) return ids;

	if(!IsLoan) return 0;

	INT nLoanCnt = 0;
	
	if(NULL != pMainGrid)
	{
	
		CString sMsg = _T("선정된 데이터를 타관대출 하시겠습니까?");
		ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("타관대출"), MB_ICONWARNING | MB_YESNO);
		if(ids == IDNO)
		{
			return 0;
		}
		pMainGrid->SelectMakeList();
		
		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		if(pMainGrid->SelectGetCount())
		{
			INT nRowCount = pMainGrid->SelectGetCount();
			
			INT nIdx2 = pMainGrid->SelectGetTailPosition();

			CString strMsg;
			for(int j=0;j<nRowCount;j++)
			{
				CString strOtherLoanKey2 = pDM->GetCellData(_T("타관대출KEY"),nIdx2);
				CString strTranCode = pDM->GetCellData(_T("트랜잭션상태"),nIdx2);
				CString strRegNo = pDM->GetCellData(_T("등록번호"),nIdx2);

				if(strOtherLoanKey2.IsEmpty() || (!strOtherLoanKey2.IsEmpty() && _T("3")!=strTranCode))
				{
					strMsg.Format(_T("등록번호가 '%s'인 자료는 입수 상태가 아닙니다."), strRegNo);
					SelfCloseMessageBox(strMsg);
					return 0;
				}
								
		
				CString strLoanLibCode;

				ids = pDM->GetCellData(_T("대출도서관부호"), nIdx2, strLoanLibCode);
				if (_T("3") == strTranCode && strLoanLibCode != m_pParentMgr->m_pInfo->LIB_CODE) {
					strMsg.Format(_T("등록번호가 '%s'인 자료는 자관에서 입수한 자료가 아닙니다."), strRegNo);
					SelfCloseMessageBox(strMsg);
					return 0;
				}

				nIdx2 = pMainGrid->SelectGetPrev();
			}

			CString strRegNo, strTitle, strAppendixSendYN, strAppendixRegNoList;					
			CString strReturnExpireDate = _T("");
			CTime tCurrent = CTime::GetCurrentTime();

			CString strLoanExpiryFile;
			m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("대출관리"), _T("대출만기자료대출여부"), strLoanExpiryFile);
			strLoanExpiryFile.TrimLeft();strLoanExpiryFile.TrimRight();
			if (strLoanExpiryFile.IsEmpty())
			{
				strLoanExpiryFile = _T("Y");
			}

			INT nIdx = pMainGrid->SelectGetHeadPosition();
			for(int i=0;i<nRowCount;i++)
			{
				CString sData;
				strOtherLoanKey = pDM->GetCellData(_T("타관대출KEY"),nIdx);
				if(0!=strOtherLoanKey.GetLength())
				{
				
					pDM->GetCellData(_T("등록번호"), nIdx, strRegNo);
					pDM->GetCellData(_T("본표제"), nIdx, strTitle);
					pDM->GetCellData(_T("부록발송유무"), nIdx, strAppendixSendYN);
					pDM->GetCellData(_T("부록등록번호리스트"), nIdx, strAppendixRegNoList);
					if (_T("Y") == strAppendixSendYN) {
						strMsg.Format(
							_T("부록자료와 함께 입수된 자료입니다.\r\n")
							_T("\r\n")
							_T("[ %s:%s ]\r\n")
							_T("부록자료를 확인 하셨습니까?\r\n")
							_T(" - 부록등록번호 : %s\r\n")
							_T("( 예 : 대출,  아니오 : 대출취소 )")
							, strRegNo, strTitle, strAppendixRegNoList);
						if (IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("타관대출>부록확인"), MB_ICONINFORMATION|MB_YESNO)) {
							continue;
						}
					}
				
					pDM->GetCellData(_T("대출만기일"), nIdx, strReturnExpireDate);
					if(_T("Y") != strLoanExpiryFile && !strReturnExpireDate.IsEmpty())
					{						
						COleDateTime tReturnExpireDate(_ttoi(strReturnExpireDate.Mid(0,4)), _ttoi(strReturnExpireDate.Mid(5,2)), _ttoi(strReturnExpireDate.Mid(8,2)), 0, 0, 0);
						if(COleDateTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), 0, 0, 0) > tReturnExpireDate)
						{
							if(_T("S") == strLoanExpiryFile)
							{
								strMsg.Format(
								_T("등록번호가 '%s'인 자료는 대출만기일이 [%s]까지 입니다.\r\n")
								_T("대출만기일이 지난 자료를 대출하시겠습니까?\r\n")
								_T("( 예 : 대출,  아니오 : 대출취소 )")
								, strRegNo, strReturnExpireDate);
								if (IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("타관대출>대출만기일만료확인"), MB_ICONINFORMATION|MB_YESNO)) {
									continue;	
								}
							}
							else if(_T("N") == strLoanExpiryFile)
							{
								CString strMsg = _T("");
								strMsg.Format( _T("[%s] 자료가 대출 만기일이 지나 대출 취소되었습니다."), strRegNo );
								SelfCloseMessageBox(strMsg);
								continue;
							}
						}
					}
					m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합총대출책수"), strLoanBookCnt, 0);
				
					CString sLoanType;
					ids = GetLoanType(sLoanType);	
					if(0 > ids) return ids;
					if( _T("1") != sLoanType && _ttoi(strLoanBookCnt) > _ttoi(m_strMaxLoanCount)) 		
					{
						SelfCloseMessageBox(_T("통합대출가능권수 한도를 초과하여 대출할 수 없습니다."));
						return 0;			
					}

					CString strHoldLibCode;
					strHoldLibCode = pDM->GetCellData(_T("제공도서관부호"), nIdx);
					CString strLoanLibCode;
					strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
					CString strUserNo;
					m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);

					CString strLoanDate;					
					CTime tLoanDate = CTime::GetCurrentTime();

					CString strStringLoanDate = _T("");
					ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("소급대출일"), strStringLoanDate);
					if(0 > ids || strStringLoanDate.IsEmpty()) return ids;
					strLoanDate.Format(_T("%s %02d:%02d:%02d"), strStringLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute(), tLoanDate.GetSecond());
					
					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("LILL_K01_LOC_B01_SERVICE"));
					Writer.addRecord();
					Writer.addElement(_T("HOLD_LIB_CODE"), strHoldLibCode);
					Writer.addElement(_T("LOAN_LIB_CODE"), strLoanLibCode);
					Writer.addElement(_T("USER_NO"),strUserNo);
					Writer.addElement(_T("LOAN_KEY"),strOtherLoanKey);
					Writer.addElement(_T("L_DEVICE"),_T("KOLAS III"));
					Writer.addElement(_T("L_WORKER"),m_pParentMgr->m_pInfo->USER_ID);
					Writer.addElement(_T("LOAN_DATE"),strLoanDate);
					
					CFrameSender Sender(m_pParentMgr->m_pInfo);	
					CFrameReader Reader; 
					ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
					if (0 > ids) 
					{
						while(true) 
						{
							
							if(IDYES == MessageBox(m_pParentMgr->m_hWnd,_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("조회실패"), MB_YESNO | MB_ICONQUESTION)) 
							{
								ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
								if(0 <= ids) break;
							}
							else
							{
								return 0;
							}
						}
					}
					if(_T("OK") == Reader.getElement(0,0))
					{
						Send3000ApiSocket(_T("타관대출"));
						nLoanCnt++;
					}			
				}
				nIdx=pMainGrid->SelectGetNext();	
			}	
			if( nLoanCnt > 0 ) {
				SelfCloseMessageBox(_T("타관대출이 완료되었습니다."));				
				m_bReturnBookNonFree=TRUE;
				SPFExcutePath(_T("회원리로드_NOASK"));
			}
		}
		
	}
	return 0;
}
INT CBL_LOC_LOAN_PROC::IsLoanMember(BOOL &IsLoan)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsLoanMember");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO") ;
	CString sLoanType;
	CString sInferiorClass;
	CString sLoanStopDate;
	CString sMemberClass;
	CString sUnionInferioClass;
	CString sLostUserCard;

	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;

	if(0 == sLoanType.Compare(_T("1")))
	{
	}	
	else
	{
		IsLoan = FALSE;
		
		if(_T("Y") == m_pManageValue->m_ArrearCheckYN)
		{
			CString strArrear;
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("총연체료"), strArrear, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(0 < _ttoi(strArrear))
			{
				SelfCloseMessageBox(_T("미납된 연체료가 있습니다."), m_pParentMgr, 2000);
				return 0;
			}
		}

		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("회원상태"), sInferiorClass, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(0 != sInferiorClass.Compare(_T("0")))
		{
			SelfCloseMessageBox(_T("정상 회원만이 대출이 가능합니다."));
			return 0;
		}
		
		if(m_pManageValue->m_sLoanIsLostUserCard == _T("Y"))
		{
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("회원증분실"), sLostUserCard, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(sLostUserCard == _T("Y"))
			{
				SelfCloseMessageBox(_T("회원증분실 회원은 대출이 불가능 합니다."));
				return 0;
			}
		}
		
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("대출정지일"), sLoanStopDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		if(0 == sLoanStopDate.Find(_T("0000")))
		{
			sLoanStopDate = _T("");
			ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("대출정지일"), sLoanStopDate, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		}

		if(!sLoanStopDate.IsEmpty())
		{
			COleDateTime tLoanStopDate(_ttoi(sLoanStopDate.Mid(0,4)), _ttoi(sLoanStopDate.Mid(5,2)), _ttoi(sLoanStopDate.Mid(8,2)), 0, 0, 0);
			
			
			CString strGetData;
			CTime tCurrentCTime = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);

			COleDateTime tCurrent(tCurrentCTime.GetYear(), tCurrentCTime.GetMonth(), tCurrentCTime.GetDay(), 0, 0, 0);
			
			if(tCurrent <= tLoanStopDate)
			{
				SelfCloseMessageBox(_T("대출 정지일이 지나지 않았으므로 대출이 불가능합니다."));
				return 0;
			}
		}
		
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("회원구분"), sMemberClass, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

		if(0 == sMemberClass.Compare(_T("1")))
		{
			SelfCloseMessageBox(_T("비회원은 대출이 제한됩니다."));			
			return 0;
		}
		
		else if(0 == sMemberClass.Compare(_T("2")))
		{
			if(IDYES == (AfxMessageBox(_T("회원 인증을 받지 않았습니다.\r\n")
				                         _T("회원 인증을 하시겠습니까?"), MB_ICONQUESTION | MB_YESNO)))
			{
				SPFExcutePath(_T("이용자_상세정보수정"));
			}
			return 0;
		}
		
		if(0 == m_pManageValue->m_sIsUnionUserStatus.Compare(_T("Y")))
		{
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("통합대출회원상태"), sUnionInferioClass, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
			if(0 == sUnionInferioClass.Compare(_T("대출정지")))
			{
				SelfCloseMessageBox(_T("타 도서관 대출정지 회원입니다."));
				return 0;
			}
			else if(sUnionInferioClass.IsEmpty())
			{
				SelfCloseMessageBox(_T("통합대출자정보 동기화 중입니다."));
				return 0;
			}
		}
		
		if(m_pManageValue->m_sIsUnionUserRemoveStatus == _T("Y"))
 		{
 			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("통합대출회원상태"), sUnionInferioClass, 0);
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
 			if(sUnionInferioClass == _T("제적회원"))
 			{
 				SelfCloseMessageBox(_T("타 도서관 제적회원입니다."));
 				return 0;
 			}
 			else if(sUnionInferioClass.IsEmpty())
 			{
 				SelfCloseMessageBox(_T("통합대출자정보 동기화 중입니다."));
 				return 0;
 			}
 		}
	}
	IsLoan = TRUE;
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsRegNo(CString &sRegNo)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsRegNo");

	sRegNo.Empty();
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");

	ids = m_pParentMgr->GetControlData(sCMAlias, _T("대출자번호"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::Loan(CString &sRegNo, INT nMode /*=0*/ )
{
EFS_BEGIN

	INT ids;

	INT nBookCnt = 0;

	ids = SearchBookInfo(sRegNo, nBookCnt, nMode);
	if(0 > ids) return ids;

	if(nBookCnt == 0) 
	{		
		nMode = 1;
		ids = SearchBookInfo(sRegNo, nBookCnt, nMode);
		if(0 > ids) return ids;
		
		if(nBookCnt == 0)
		{
			ids = MessageBox(m_pParentMgr->m_hWnd, _T("입력한 등록번호를 가진 자료가 존재하지 않습니다. 긴급대출하시겠습니까?"), _T("대출/반납"), MB_YESNO | MB_ICONQUESTION);
			if(ids == IDYES)
				return 1;
			return 101;
		}
	}
	if(nBookCnt > 1)
	{
		CLoanBookListDlg BookList(m_pParentMgr);
		BookList.DoModal();
		
		if(BookList.GetCloseStatus() == 0)
		{
			SelfCloseMessageBox(_T("취소되었습니다."));
			return 0;
		}

	}

	CString sStatus;
	CString sBookKey;
	ids = IsBookRelationCurrentUser(sStatus,sBookKey);
	if(ids == 1)
	{
		ids = MessageBox(m_pParentMgr->m_hWnd,_T("이미 이용자가 대출한 책입니다. 반납하시겠습니까?"), _T("대출/반납"), MB_YESNO | MB_ICONQUESTION);
		if(ids == IDYES)
		{
			ids = BarCodeReturnBookCheck(sBookKey);
			if(0 > ids) return ids;
			
			ReturnBook(TRUE);
		}
		return 2;

	}
	else if(ids == 2)
	{
		CString strControlFlag;
		GetTempData(_T("대출반납흐름제어"), strControlFlag);

		INT nYesNo = -1;
		if (strControlFlag.CompareNoCase(_T("Y")) != 0){
			nYesNo = AfxMessageBox(_T("이미 이용자가 예약한 책입니다.")_T("\r\n예약대출 하시겠습니까?"), MB_YESNO);
		}

		if ( strControlFlag.CompareNoCase(_T("Y")) == 0 || nYesNo == IDYES ) 
		{
			ids = BarCodeReturnBookCheck(sBookKey);
			if(0 > ids) return ids;

			ids = SPFExcutePath( _T("예약대출") );
			if(0 > ids) return ids;
		}

		return 3;
	}

	BOOL IsLoan = false;

	ids = IsLoanBook(IsLoan, nMode);
	if(0 > ids) return ids;
	if(!IsLoan) return 2;
	
	ids = LoanDBProc(nMode);
	if(ids == 100 || ids == 101) return ids;
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchBookInfo(CString &sRegNo, INT &nBookCnt, INT nMode/*=0*/, BOOL bRegNo/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SearchBookInfo");
	
	CESL_DataMgr* pExpLoanInfoDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_BOOK_INFO"));
	if(pExpLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(nMode == 0)
	{
		if(pExpLoanInfoDM->GetRowCount() > 0)
		{
			CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);

			nBookCnt = pExpLoanInfoDM->GetRowCount();
			return 0;
		}
		
		ids = MakeInDBSearchBookInfo(sRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE, nBookCnt);
		if(0 > ids) return ids;
	}
	else if(nMode == 1)
	{
		if(pExpLoanInfoDM->GetRowCount() > 0)
		{
			CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);

			nBookCnt = pExpLoanInfoDM->GetRowCount();
			return 0;
		}

		ids = MakeNotInDBSearchBookInfo(sRegNo, bRegNo, nBookCnt);
		if(0 > ids) return ids;
	}
	else if(2 == nMode)
	{
		if(pExpLoanInfoDM->GetRowCount() > 0)
		{
			CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);

			nBookCnt = pExpLoanInfoDM->GetRowCount();
			return 0;
		}

		CESL_DataMgr* pILLDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_ILL_BOOK_INFO"));
		if(NULL == pILLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		CString strFindRegNo;
		CString strBookManageCode;		

		int nRowCnt = 0;
		const int ROW_COUNT = pILLDM->GetRowCount();
		
		for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
		{
			ids = pILLDM->GetCellData(_T("등록번호"), 0, strFindRegNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(strFindRegNo == sRegNo)
			{
				ids = pILLDM->GetCellData(_T("관리구분_책"), nRowCnt, strBookManageCode);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

				ids = MakeInDBSearchBookInfo(sRegNo, strBookManageCode, nBookCnt);
				if(0 > ids) return ids;
				break;
			}

		}
	}

	if(nBookCnt > 0)
	{
		ids = AddExpLoanInfo();
		if(0 > ids) return ids;
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitExpDM()
{
EFS_BEGIN

	CString strFuncName = _T("InitExpDM");

	
	const INT nDMCount = 5;
	TCHAR *sDMAlias[nDMCount] = 
	{
		_T("DM_EXP_LOAN_BO_BOOK_INFO"), _T("DM_EXP_LOAN_SE_BOOK_INFO"), _T("DM_EXP_LOAN_NON_DB_BOOK_INFO"),
		_T("DM_EXP_LOAN_BOOK_INFO"), _T("DM_EXP_LOAN_ILL_BOOK_INFO")
	};

	CESL_DataMgr *pDM;
	for(INT i = 0 ; i < nDMCount ; i++)
	{
		pDM = m_pParentMgr->FindDM(sDMAlias[i]);
		if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		pDM->FreeData();
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::MakeInDBSearchBookInfo(CString sRegNo, CString strManageCode, INT &nBookCnt)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("MakeInDBSearchBookInfo");

	const int EXP_DM_COUNT = 2;
	
	CString DMAlias[EXP_DM_COUNT] =
	{
		_T("DM_EXP_LOAN_BO_BOOK_INFO"), _T("DM_EXP_LOAN_SE_BOOK_INFO")
	};
		
 	CString sWhere[EXP_DM_COUNT];
 	sWhere[0].Format(
 		_T("BB.MANAGE_CODE='%s' AND BB.REG_NO = '%s' AND BB.WORKING_STATUS IN (") _T(HOLD_SEARCH_CONDITION)	
		_T(",'BOL217O','BOL218O', 'BOL219O') AND BB.BOOK_APPENDIX_FLAG IN ('B','A') "),
 		strManageCode, sRegNo);
	sWhere[1].Format(
		_T("BB.MANAGE_CODE='%s' AND BB.REG_NO = '%s' AND BB.WORKING_STATUS IN ('SEL112N',") _T(HOLD_SE_SEARCH_CONDITION)
		_T(",'SEL317O','SEL318O') AND BB.BOOK_TYPE IN ('B','C','A') ")
		_T("AND BB.BINDED_BOOK_YN != 'Y' "),
 		strManageCode, sRegNo);
	
	CString strMessage[EXP_DM_COUNT] = 
	{
		_T("단행대출할 자료"), _T("연속대출할 자료")
	};

	int nCnt = 0;
	CString strMsg;
	DWORD nSTART, nFINISH;
	CESL_DataMgr *pExpDM = NULL;
	for(nCnt = 0; nCnt < EXP_DM_COUNT; nCnt++)
	{
		pExpDM = m_pParentMgr->FindDM(DMAlias[nCnt]);
		if(NULL == pExpDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		nSTART = GetTickCount();
		ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pExpDM, sWhere[nCnt]);
		if(0 > ids)
		{
			return ids;
		}

		nFINISH = GetTickCount();

		strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), strMessage[nCnt], (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		TRACE(strMsg.GetBuffer(0));
		pExpDM->QueryLog(strMsg.GetBuffer(0));

		nBookCnt += pExpDM->GetRowCount();

		if(nBookCnt > 0)
		{
			return 0;
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::MakeNotInDBSearchBookInfo(CString sRegNo, BOOL bRegNo, INT &nBookCnt)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("MakeNotInDBSearchBookInfo");

	CString sLoanNonDBBookDMAlias = _T("DM_EXP_LOAN_NON_DB_BOOK_INFO");

	CString sDBFieldAlias;

	sDBFieldAlias = _T("REG_NO");

	CString sWhere;

	sWhere.Format(_T("NB.%s = '%s' AND ")
						_T("NB.WORKING_STATUS IN ")
						_T("(")
						_T(HOLD_SEARCH_CONDITION)
						_T(",'BOL217O','BOL218O', 'BOL219O')")
						, sDBFieldAlias, sRegNo);
	

	CESL_DataMgr *pBookDM = m_pParentMgr->FindDM(sLoanNonDBBookDMAlias);
	if(pBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();
	
	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pBookDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	nFINISH = GetTickCount();
	CString strMsg;
	strMsg.Format(_T("(Finish) - [긴급 대출할 자료] 걸린 시간 :  [%d.%d sec]"),  (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	TRACE(strMsg.GetBuffer(0));
	pBookDM->QueryLog(strMsg.GetBuffer(0));

	nBookCnt += pBookDM->GetRowCount();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddExpLoanInfo()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddExpLoanInfo");

	CString sDstDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CESL_DataMgr *pDstDM = m_pParentMgr->FindDM(sDstDMAlias);
	if(pDstDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	const INT nFieldCnt = 39;
	TCHAR *sDstDMFieldAlias[nFieldCnt] = 
	{
		_T("책KEY"), _T("상태"), _T("자료실구분"), _T("단행/연속구분"), _T("분류기호구분"), 
		_T("분류기호"), _T("관리구분"), _T("등록구분"), _T("종정보키"), _T("등록번호"), 
		_T("MARC"), _T("별치기호"), _T("이용제한구분"), 
		_T("IBS_청구_별치기호_코드"),	
		_T("IBS_청구_분류기호"),		
		_T("IBS_청구_도서기호"),		
		_T("IBS_청구_권책기호"),		
		_T("IBS_청구_복본기호")	,	
		_T("책값"), _T("형식구분"), _T("이용대상구분"), _T("본표제"), _T("저작자"), _T("발행자"),
		_T("권호명"), 
		_T("관리구분"),
		_T("부록여부"), 
		_T("이전자료상태"), 
		_T("대출장소"),
		_T("대출자KEY"), 
		_T("무인반납중여부"),
		_T("예약건수"),
		_T("부록책수"),
		_T("RFID시리얼"),
		_T("대출형태"),
		_T("매체구분"),
		_T("권호정보KEY"),
		_T("예약이용자수"),
		_T("청구기호") 
	};

	const INT nLoanInfoDMCnt = 3;
	TCHAR *sLoanInfoDMAlias[nLoanInfoDMCnt] =
	{
		_T("DM_EXP_LOAN_BO_BOOK_INFO"), _T("DM_EXP_LOAN_NON_DB_BOOK_INFO"), _T("DM_EXP_LOAN_SE_BOOK_INFO")
	};
	
	INT nCnt = 3;

	CESL_DataMgr* pDM;

	for(INT i = 0 ; i < nCnt ; i++)
	{
		
		ids = AddLoanBookDM(sDstDMAlias, sDstDMFieldAlias, sLoanInfoDMAlias[i], sDstDMFieldAlias, nFieldCnt);
		if(0 > ids) return ids;

		pDM = m_pParentMgr->FindDM(sLoanInfoDMAlias[i]);
	}

	
 	MakeShelfCode(sDstDMAlias);

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::IsLoanBook(BOOL &IsLoanBook, INT nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsLoanBook");

	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CString sBookState;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("상태"), sBookState, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sPublishForm;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("단행/연속구분"), sPublishForm, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strBookManageCode;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("관리구분"), strBookManageCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(2 != nMode)
	{
	if(sPublishForm.Compare(_T("MO"))==0 || (sPublishForm.Compare(_T("NB"))==0))
	{
		if(!(sBookState.Compare(_T("BOL112N"))==0)) 
		{
			
			CString sReserveMode;
			ids = m_pCheckLoan->GetReserveMode(sReserveMode);
			if(0 > ids) return ids;			
#ifdef	__K2UP__
			
			if((sBookState.Compare(_T("BOL211O"))==0)
			  ||(sBookState.Compare(_T("BOL212O"))==0)
			  ||(sBookState.Compare(_T("BOL213O"))==0)
			  ||(sBookState.Compare(_T("BOL214O"))==0)
			  ||(sBookState.Compare(_T("BOL215O"))==0)
			  ||(sBookState.Compare(_T("BOL217O"))==0)
			  ||(sBookState.Compare(_T("BOL218O"))==0)
			  ||(sBookState.Compare(_T("BOL219O"))==0)
			  
			  ||(sBookState.Compare(_T("BOL411O"))==0)
			  ||(sBookState.Compare(_T("BOL511O"))==0)
			  
			  ||(sBookState.Compare(_T("BOL611O"))==0)
			 )
#else
			if((sBookState.Compare(_T("BOL221O"))==0)
	     	  ||(sBookState.Compare(_T("BOL222O"))==0)
		  	  ||(sBookState.Compare(_T("BOL223O"))==0)
			  ||(sBookState.Compare(_T("BOL224O"))==0)
			  ||(sBookState.Compare(_T("BOL225O"))==0)
			 )			
#endif
			{
				if(_ttoi(sReserveMode)==0)
				{
					IsLoanBook = FALSE;
					SelfCloseMessageBox(_T("이미 대출된 책이므로 대출 할 수 없습니다."));
					return 0;
				}
				CString sMsg;
				sMsg.Format(_T("대출하려는 책이 이미 대출이 된 상태의 책입니다. \r\n예약하시겠습니까?"));
				ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("예약하기"), MB_ICONQUESTION | MB_YESNO);
				if(IDYES == ids)
				{
					SPFExcutePath(_T("예약"));			
				}
				IsLoanBook = FALSE;				
				return 0;
			}
			
			CString sMsg;
			GetWorkingStatusDescToCode(sBookState, 1);
			sMsg.Format(_T("현재 책의 상태가 (%s)이기 때문에 대출 할 수 없습니다."), sBookState);
			SelfCloseMessageBox(sMsg);
			IsLoanBook = FALSE;

			return 0;
		}
	}
	else
	{
		if(!(sBookState.Compare(_T("SEL112N"))==0) && !(sBookState.Compare(_T("SEL212N"))==0)) 
		{
			
			CString sReserveMode;
			ids = m_pCheckLoan->GetReserveMode(sReserveMode);
			if(0 > ids) return ids;			
			
			if((sBookState.Compare(_T("SEL311O"))==0)
			  ||(sBookState.Compare(_T("SEL312O"))==0)
 			  ||(sBookState.Compare(_T("SEL411O"))==0)
			  ||(sBookState.Compare(_T("SEL511O"))==0)
			  ||(sBookState.Compare(_T("SEL313O"))==0)
			  ||(sBookState.Compare(_T("SEL314O"))==0)
			  ||(sBookState.Compare(_T("SEL315O"))==0)
			)
			{				
				if(_ttoi(sReserveMode)==0)
				{
					IsLoanBook = FALSE;
					SelfCloseMessageBox(_T("이미 대출된 책이므로 대출 할 수 없습니다."));
					return 0;
				}

				CString sMsg;
				sMsg.Format(_T("대출하려는 책이 이미 대출이 된 상태의 책입니다. \r\n 예약하시겠습니까?"));
				ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("예약하기"), MB_ICONQUESTION | MB_YESNO);
				if(ids == IDYES) 
					SPFExcutePath(_T("예약"));			
				IsLoanBook = FALSE;
				return 0;
			}
			
			CString sMsg;
			GetWorkingStatusDescToCode(sBookState, 1);
			sMsg.Format(_T("현재 책의 상태가 (%s)이기 때문에 대출 할 수 없습니다."), sBookState);
			SelfCloseMessageBox(sMsg);
			IsLoanBook = FALSE;
			return 0;
		}
		
		else if(_T("SEL112N") == sBookState)
		{
			CString strRegNo;
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("등록번호"), strRegNo, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			if(strRegNo.IsEmpty())
			{
				SelfCloseMessageBox(_T("등록번호가 부여되지 않은 자료는 대출 할 수 없습니다."));
				IsLoanBook = FALSE;
				return 0;
			}
		}
	}
	
	CString sBookRecKey;
	CString sReserveCount;
	CString sVolReserveCnt;
	CString sVolInfoKey;
	CString sSpeciesKey;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("책KEY"), sBookRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("종정보키"), sSpeciesKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("권호정보KEY"), sVolInfoKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	ids = IsReservatedBook(sBookRecKey, sSpeciesKey, sVolInfoKey, sReserveCount, sVolReserveCnt);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	if((_ttoi(sReserveCount)>0) || (_ttoi(sVolReserveCnt)>0)) 
	{
		CString sMsg;
		sMsg.Format(_T("대출하려는 책이 반납은 되었으나 다른 이용자에 의해 예약이 된 상태의 책입니다. \r\n 예약하시겠습니까?"));
		ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("예약하기"), MB_ICONQUESTION | MB_YESNO);
		if(IDYES==ids)
		{
			m_bLoanCallReserve = TRUE;
			SPFExcutePath(_T("예약"));			
		}
		IsLoanBook = FALSE;
		return 0;
	}
	}	
	
	else if(nMode == 2)
	{
		CString strCompareState;
		if(sPublishForm.Compare(_T("MO"))==0 || (sPublishForm.Compare(_T("NB"))==0))
		{
			strCompareState = _T("BOL411O");
		}
		else
		{
			strCompareState = _T("SEL411O");
		}
		if(!(sBookState.Compare(strCompareState)==0)) 
		{
			
			CString sMsg;
			GetWorkingStatusDescToCode(sBookState, 1);
			sMsg.Format(
				_T("책두레 대상자료가 아닙니다.\r\n")
				_T("현재 책의 상태가 (%s)이기 때문에 대출 할 수 없습니다."), sBookState);
			SelfCloseMessageBox(sMsg);
			IsLoanBook = FALSE;
			
			return 0;
		}
				
		CString strBookKey = _T("");
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("책KEY"), strBookKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		
		CString strQuery;
		CString strValue;
		strQuery.Format(
			_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS IN ('L', 'D')"),
			strBookKey
			);
		
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
		ids = TmpDM.GetOneValueQuery(strQuery, strValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		
		if(_T("0") != strValue)
		{
			CString sMsg;		
			sMsg.Format(			
				_T("현재 책이 이미 책두레대출중이기 때문에 대출 할 수 없습니다."));
			SelfCloseMessageBox(sMsg);
			IsLoanBook = FALSE;
		}	
	}

	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;

	if(_ttoi(sLoanType) != 1)
	{		
		if(2 != nMode )
		{
		ids = IsUserBookLoan();
		}
		else
		{			
			ids = IsUserBookLoan(EXP_LOAN, TRUE, strBookManageCode);
		}
		if(0 > ids) return ids;
		if(ids > 0)
		{
			IsLoanBook = FALSE;
			return 0;
		}
	}

	if(2 == nMode)
	{
		IsLoanBook = TRUE;
		return 0;
	}

	ids = IsReLoanBook();

	if(_ttoi(sLoanType) == 1 && ids != 0)
	{		
		if(11 == ids)
			ids = 1;
		else if(22 == ids)
			ids = 2;
	}
	if(0 > ids) return ids;
	if(ids > 0)
	{
		if(m_pCheckLoan->IsViewReLoanHistory() == 0)
		{
			ids = ViewLoanHistory(ids,0);
			if(ids == 1)
			{
				IsLoanBook = FALSE;
				return 0;
			}
		}
		else
		{
			if(ids == 11 || (ids==22))
			{
				CString sMsg;
				if(ids == 11) sMsg.Format(_T("예전에 대출 되었던 책입니다. 대출하실수 없습니다. "));
				if(ids == 22) sMsg.Format(_T("가족중 한명이 예전에 대출 했었던 책입니다. 대출하실수 없습니다. "));
				SelfCloseMessageBox(sMsg);
				IsLoanBook = FALSE;
				return 0;
			}			
		}
	}
	
	IsLoanBook = TRUE;
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::LoanDBProc(INT nMode/*=0*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("LoanDBProc");
	
	CString strBookManageCode, strBookLibCode;

	BOOL bIsILL = (2 == nMode) ? TRUE : FALSE;

	CString sMode;
	sMode.Format(_T("%d"), nMode);
	
	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);	
	
	CESL_DataMgr *pKLDM = m_pParentMgr->FindDM(_T("DM_KL_LOAN"));
	if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CString strKLUserYN;
	pMemberDM->GetCellData(_T("통합회원여부"), 0, strKLUserYN);
	if(_T("Y") != strKLUserYN)
	{
		strKLUserYN = _T("N");
	}
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		pKLDM->FreeData();
		pKLDM->InsertRow(-1);
	}
	
	ids = InsertLoanRecord(pMobileApi, bIsILL, strKLUserYN, nMode);

	if(ids == 100 || ids == 101) return ids;
	
	if(0 > ids) return ids;
	
	if(FALSE == bIsILL)
	{
		ids = UpdateBookInfo(pMobileApi,sMode);
		if(0 > ids) return ids;
	}
	else
	{
		CESL_DataMgr* pILLDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_ILL_BOOK_INFO"));
		if(NULL == pILLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		CString strTransNo;
		ids = pILLDM->GetCellData(_T("트랜잭션번호"), 0, strTransNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = ILL_UpdateTransInfo(TRUE, strTransNo);
		if(0 > ids) return ids;
	}

	ids = UpdateLoanUserUpdate(pMobileApi,  strKLUserYN);
	if(0 > ids) return ids;
	
	ids = pMobileApi->SendFrame();
	if(0 > ids) return ids;	
	pMobileApi->EndFrame();
	
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("관리구분"), strBookManageCode, 0);

	CESL_DataMgr tdm;
	tdm.SetCONNECTION_INFO(m_pManageValue->m_pInfo->CONNECTION_INFO);
	CString strQuery;
	
	CString strBookLibKLUse;
	strQuery.Format(_T("SELECT LIB_CODE, (SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME = '통합도서서비스' AND MANAGE_CODE = ML.MANAGE_CODE) FROM MN_LIBINFO2_TBL ML WHERE MANAGE_CODE = '%s' AND ROWNUM = 1 "), strBookManageCode);
	ids = tdm.RestructDataManager(strQuery);
	if ( ids < 0) return ids;

	strBookLibCode = tdm.GetCellData(0, 0);
	strBookLibKLUse = tdm.GetCellData(0, 1);
	
	if( (bIsILL == TRUE && strBookLibKLUse.Compare(_T("Y")) == 0) || bIsILL == FALSE){
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
		{
			
			CFrameWriter Writer(m_pParentMgr->m_pInfo);
			Writer.setCommand(_T("IL_K10_LOC_B01_SERVICE"));
			Writer.addRecord();
			CString strGetData;
			ids = pKLDM->GetCellData(_T("통합대출자번호"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("통합대출자번호"), strGetData);
			
			CString strGpinHash;
			pMemberDM->GetCellData(_T("G-PINHASH인증코드"), 0, strGpinHash);
			Writer.addElement(_T("G-PINHASH인증코드"), strGpinHash);
			Writer.addElement(_T("대출도서관"), strBookLibCode);
	
			ids = pKLDM->GetCellData(_T("대출일"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("대출일"), strGetData);
			ids = pKLDM->GetCellData(_T("반납예정일"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("반납예정일"), strGetData);
			ids = pKLDM->GetCellData(_T("상태"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			
			if ( bIsILL == TRUE ) {
				if ( strGetData.Compare(_T("L")) == 0 || strGetData.Compare(_T("0")) == 0 ) strGetData = _T("0");
				else if ( strGetData.Compare(_T("R")) == 0 || strGetData.Compare(_T("1")) == 0 ) strGetData = _T("1");
				else if ( strGetData.Compare(_T("D")) == 0 || strGetData.Compare(_T("2")) == 0 ) strGetData = _T("2");
			}
			Writer.addElement(_T("상태"), strGetData);
	
			ids = pKLDM->GetCellData(_T("등록번호"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("등록번호"), strGetData);
			ids = pKLDM->GetCellData(_T("서명"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("서명"), strGetData);
	 	 	ids = pKLDM->GetCellData(_T("저자"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	 	 	Writer.addElement(_T("저자"), strGetData);
	 	 	ids = pKLDM->GetCellData(_T("발행자"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	 	 	Writer.addElement(_T("발행자"), strGetData);
			ids = pKLDM->GetCellData(_T("청구기호"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("청구기호"), strGetData);
			ids = pKLDM->GetCellData(_T("최종대출일"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("최종대출일"), strGetData);
			ids = pKLDM->GetCellData(_T("권책기호"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("편권차"), strGetData);
			ids = pKLDM->GetCellData(_T("로컬KEY"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("로컬KEY"), strGetData);
			
			ids = pKLDM->GetCellData(_T("부록여부"), 0, strGetData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			Writer.addElement(_T("부록여부"), strGetData);
			
			if ( bIsILL == TRUE) {
				Writer.addElement(_T("지역내상호대차상태"), _T("Y"));
			}
				
	 	 	CFrameSender Sender(m_pParentMgr->m_pInfo);
	 	 	Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("통합회원대출"), __WFILE__, __LINE__));
		}
	}
	
	CLocCommonAPI::MobileApiEndLog(pMobileApi);

	INT nMode = 0;
	if(_T("Y") == m_pManageValue->m_sIsShareInsert)
	{
		nMode = 1;
	}
	ShareInfoCenterUpload(_T("I"), _T("U"), nMode);	
	ids = UpdateSuccessDM();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InsertLoanRecord(CKolas2up_MobileApi *pMobileApi, BOOL bIsILLMode, CString strKLUserYN, INT i_nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InsertLoanRecord");
	
	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();
	pDM->StartFrame();
	
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	const INT nSrcFieldCnt = 15;
	TCHAR *sSrcFieldAlias[nSrcFieldCnt] = 
	{
		_T("책KEY"), _T("자료실구분"), _T("단행/연속구분"), _T("분류기호구분"), _T("분류기호"), 
		_T("관리구분"), _T("등록구분"), _T("종정보키"), _T("등록번호"), _T("별치기호"), 
		_T("본표제"), _T("저작자"), _T("발행자"), _T("청구기호"), _T("권호정보KEY")		
	};
	TCHAR *sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("BOOK_KEY"), _T("SHELF_LOC_CODE"), _T("PUBLISH_FORM_CODE"), _T("CLASS_NO_TYPE"), _T("CLASS_NO"), 
		_T("MANAGE_CODE"), _T("REG_CODE"), _T("SPECIES_KEY"), _T("REG_NO"), _T("SEPARATE_SHELF_CODE"), 
		_T("TITLE"), _T("AUTHOR"), _T("PUBLISHER"), _T("CALL_NO"), _T("VOL_INFO")
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), 
		_T("STRING"), _T("STRING"), _T("NUMERIC"), _T("STRING"), _T("STRING"),
		_T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("NUMERIC")
	};

	CESL_DataMgr *pKLDM = m_pParentMgr->FindDM(_T("DM_KL_LOAN"));
	if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sGetData;
	for(INT i= 0 ; i < nSrcFieldCnt ;i++)
	{
		if(!(m_pManageValue->m_bVolReserve) && (i+1)==nSrcFieldCnt) break;

		ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, sSrcFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		if(CString(_T("NUMERIC")).Compare(sDstFieldTypeAlias[i])==0)
		{
			if(sGetData.IsEmpty()) sGetData = _T("NULL");
		}
 		
 		if(_T("MANAGE_CODE") == sDstFieldAlias[i])
 		{
			sGetData = m_pParentMgr->m_pInfo->MANAGE_CODE;
 		}
		ids = pDM->AddDBFieldData(sDstFieldAlias[i], sDstFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
		{
			if(_T("REG_NO") == sDstFieldAlias[i])
			{
				ids = pKLDM->SetCellData(_T("등록번호"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			}
			else if(_T("TITLE") == sDstFieldAlias[i])
			{
				ids = pKLDM->SetCellData(_T("서명"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			}
 			else if(_T("AUTHOR") == sDstFieldAlias[i])
 			{
 				ids = pKLDM->SetCellData(_T("저자"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 			}
 			else if(_T("PUBLISHER") == sDstFieldAlias[i])
 			{
 				ids = pKLDM->SetCellData(_T("발행자"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 			}
			else if(_T("CALL_NO") == sDstFieldAlias[i])
			{
				ids = pKLDM->SetCellData(_T("청구기호"), sGetData, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
			}
		}
	}	
	
	CString sSrcUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	const INT nSrcUserFieldCnt = 4;
	TCHAR *sSrcUserFieldAlias[nSrcUserFieldCnt] = 
	{_T("대출자KEY"), _T("대출자소속"), _T("대출자직급"), _T("회원구분")};
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{_T("USER_KEY"), _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MEMBER_CLASS")};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING")};
	for(i= 0 ; i < nSrcUserFieldCnt ;i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sSrcUserDMAlias, sSrcUserFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
		ids = pDM->AddDBFieldData(sDstUserFieldAlias[i], sDstUserFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
	}

	CString sLoanType;
	ids = GetLoanType(sLoanType);	
	if(0 > ids) return ids;
	ids = pDM->AddDBFieldData(_T("LOAN_TYPE_CODE"), _T("STRING"), sLoanType, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);

	CString sRecKey;
	ids = pMobileApi->MakeRecKey(sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);

	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		ids = pKLDM->SetCellData(_T("로컬KEY"), sRecKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);
	}
	
	SetTempData(SHARE_CO_LOAN_TBL_INSERT_REC_KEY, CComVariant(sRecKey));

	Send3000ApiSocket(_T("대출"), sRecKey);	
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출/반납정보KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);	
	
	CString sReturnExpDate;
	CString sRegCode;
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("등록구분"), sRegCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);
	ids = GetReturnExpDate(sReturnExpDate, sRegCode);	
	
	if(ids == 100 || ids == 101) return ids;
	if(0 > ids) return ids;
	
	CString sTempDate;
	if(!sReturnExpDate.IsEmpty())
	{
		sTempDate = _T("TO_DATE('")+sReturnExpDate+_T(" 23:59:59")+_T("', 'YYYY/MM/DD HH24:MI:SS')");
	}
	else
	{
		sTempDate = _T("NULL");
	}
	ids = pDM->AddDBFieldData(_T("RETURN_PLAN_DATE"), _T("NUMERIC"), sTempDate, TRUE);
	
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);

	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		CString strReturnPlanDate = _T("");
		if(!sReturnExpDate.IsEmpty())
		{
			strReturnPlanDate = sReturnExpDate + _T(" 23:59:59");
		}
		ids = pKLDM->SetCellData(_T("반납예정일"), strReturnPlanDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1090, strFuncName);
	}
	
	CString sLoanStatus = (FALSE == bIsILLMode) ? _T("0") : _T("L");
	
	ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sLoanStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1100, strFuncName);
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		ids = pKLDM->SetCellData(_T("상태"), sLoanStatus, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1100, strFuncName);
	}
	
	CTime tLoanDate = CTime::GetCurrentTime();
	CString sLoanDate;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("소급대출일"), sLoanDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1110, strFuncName);
	CString strStringLoanDate;
	strStringLoanDate.Format(_T("%s %02d:%02d:%02d"), sLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute(), tLoanDate.GetSecond());
	CString sInLoanDate;
	sInLoanDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strStringLoanDate);
	ids = pDM->AddDBFieldData(_T("LOAN_DATE"), _T("NUMERIC"), sInLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1110, strFuncName);	
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		ids = pKLDM->SetCellData(_T("대출일"), strStringLoanDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
		ids = pKLDM->SetCellData(_T("최종대출일"), strStringLoanDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
		CString strVolCode;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("IBS_청구_권책기호"), strVolCode, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
		ids = pKLDM->SetCellData(_T("권책기호"), strVolCode, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);
	}
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출상태"), sLoanStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1130, strFuncName);
	sInLoanDate.Format(_T("%s %02d:%02d"), sLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute());
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출일"), sInLoanDate, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1130, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("반납예정일"), sReturnExpDate, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1130, strFuncName);
	if(sLoanType == _T("0"))
	{
		sLoanType = _T("일반");
	}
	else if(sLoanType == _T("1"))	
	{
		sLoanType = _T("특별");
	}
	else if(sLoanType == _T("2"))
	{
		sLoanType = _T("관내");
	}
	else if(sLoanType == _T("3"))	
	{
		sLoanType = _T("무인");
	}
	else if(sLoanType == _T("4"))
	{ 
		sLoanType = _T("장기");
	}
	else
	{
		sLoanType = _T("일반");
	}
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출형태"), sLoanType, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1140, strFuncName);

	
	CString sIsAppendix;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("부록여부"), sIsAppendix, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1150, strFuncName);
	if(sIsAppendix.Compare(_T("A")) == 0)
	{
		if(m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0)
		{
			pDM->AddDBFieldData(_T("LOAN_KEY"), _T("NUMERIC"), _T("1"));
		}
	}

	if(_T("Y") == m_pManageValue->m_sUseReceipt)
	{
		
		CString strLoanFlag, strReturnFlag;
		if(m_pManageValue->m_sUseReceiptType.GetAt(0) == 'Y') 
		{
			strLoanFlag = _T("N");
		}
		else
		{
			strLoanFlag = _T("C");
		}
		ids = pDM->AddDBFieldData(_T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), strLoanFlag);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1160, strFuncName);
	}
 	
 	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII-이동도서관"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
		
		ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_DEVICE"), _T("KOLASIII-이동도서관"), 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
	}
	else
	{
		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
		
		ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_DEVICE"), _T("KOLASIII"), 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
	}

	
	ids = pDM->AddDBFieldData(_T("L_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_WORKER"), m_pParentMgr->m_pInfo->USER_ID, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("소장관도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1170, strFuncName);

	
	CString sLog;
	sLog.Format(_T("%s"), m_pParentMgr->GetWorkLogMsg(_T("장서관리"), __WFILE__, __LINE__));
	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), sLog, TRUE);

	
	ids = pMobileApi->MakeInsertFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1180, strFuncName);

	pDM->InitDBFieldData();

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::UpdateBookInfo(CKolas2up_MobileApi *pMobileApi, CString sMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateBookInfo");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();

	CString sTableName;
	CString sWorkingStatus;

	CString sLoanInfoDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("단행/연속구분"), sTableName, 0);
	
	INT nBookType;
	if(sTableName.Compare(_T("MO"))== 0)
	{
		sTableName = _T("BO_BOOK_TBL");
		ids = GetBOWorkingStatus(sWorkingStatus, _ttoi(sMode));
		if(0 > ids) return ids;

		nBookType = 1;

	}
	else if(sTableName.Compare(_T("SE"))== 0)
	{
		sTableName = _T("SE_BOOK_TBL");
		ids = GetSEWorkingStatus(sWorkingStatus, _ttoi(sMode));
		if(0 > ids) return ids;

		nBookType = 2;
	}
	else if(sTableName.Compare(_T("NB")) == 0)
	{
		sTableName = _T("CO_NON_DB_BOOK_TBL");
		ids = GetBOWorkingStatus(sWorkingStatus, _ttoi(sMode));
		if(0 > ids) return ids;
		nBookType = 0;
	}
	
	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("책KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

#ifdef __K2UP__
	CString sPreWorkingStatus;
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("상태"), sPreWorkingStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CLocCommonAPI::GetWorkingStatusDescToCode(sPreWorkingStatus, 0);

	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), sPreWorkingStatus, TRUE);
#endif
	
	ids = pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sWorkingStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("대출"), __WFILE__, __LINE__), TRUE);
		
	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	pDM->InitDBFieldData();
	
	CString sSpeciesKey;
	m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("종정보키"), sSpeciesKey, 0);
	AddIdxQueryFrame(pDM, nBookType, sSpeciesKey);
	
	ids = m_pParentMgr->SetDataMgrData(sLoanInfoDMAlias, _T("상태"), sWorkingStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, strFuncName);

#ifdef __K2UP__
	ids = m_pParentMgr->SetDataMgrData(sLoanInfoDMAlias, _T("이전자료상태"), sPreWorkingStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, strFuncName);
#endif

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateLoanUserUpdate(CKolas2up_MobileApi *pMobileApi,  CString strKLUserYN)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanUserUpdate");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pDM->InitDBFieldData();
	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("대출자KEY"), sRecKey, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("총대출책수"), sLoanBookCnt, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) + 1);

	CString sIsAppendix;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("부록여부"), sIsAppendix, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	if(sIsAppendix.Compare(_T("A")) == 0 )
	{
		if(m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0)
		{
			sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) - 1);
		}
	}
	
	if(1 > _ttoi(sLoanBookCnt))
	{
		sLoanBookCnt = _T("0");
	}
	ids = pDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanBookCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	CString sLastLoanDate = _T("SYSDATE");
	ids = pDM->AddDBFieldData(_T("LAST_LOAN_DATE "), _T("NUMERIC"), sLastLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("대출"), __WFILE__, __LINE__), TRUE);
	
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN)
	{
		CESL_DataMgr *pKLDM = m_pParentMgr->FindDM(_T("DM_KL_LOAN"));
		if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		CString strUserNo;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pKLDM->SetCellData(_T("통합대출자번호"), strUserNo, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		ids = pKLDM->SetCellData(_T("부록여부"), sIsAppendix, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	
	CString sUpdateFields = _T("LOAN_COUNT,LAST_LOAN_DATE");
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(sUpdateFields));

	pDM->InitDBFieldData();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddLoanBookDM(CString sDstDMAlias, TCHAR **sDstDMFieldAlias, CString sSrcDMAlias, TCHAR **sSrcDMFieldAlias, INT nFieldCnt)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddLoanBookDM");	

	CESL_DataMgr *pDstDM = m_pParentMgr->FindDM(sDstDMAlias);
	if(pDstDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_DataMgr *pSrcDM = m_pParentMgr->FindDM(sSrcDMAlias);
	if(pSrcDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sGetData;
	INT nSrcRowCount = pSrcDM->GetRowCount();
	for(INT i = 0 ; i < nSrcRowCount ; i++)
	{
		ids = pDstDM->InsertRow(-1);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		for(INT j = 0 ; j < nFieldCnt ; j++)
		{
			if(!(m_pManageValue->m_bVolReserve) && nFieldCnt-2<j)
				break;

			sGetData.Empty();

			ids = pSrcDM->GetCellData(sSrcDMFieldAlias[j], i, sGetData);
			if(0 > ids&& (ids !=-4012) && (ids!=-111)) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if((sGetData == -111) || (ids == -4012)) sGetData = _T("");

			ids = pDstDM->SetCellData(sDstDMFieldAlias[j], sGetData, pDstDM->GetRowCount()-1); 
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetLoanType(CString &sLoanType)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetLoanType");

	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("대출형태"), sLoanType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	if(sLoanType.Compare(_T("3")) == 0)
		sLoanType = _T("4");

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetBOWorkingStatus(CString &sWorkingStatus, INT nMode/*=0*/)
{
EFS_BEGIN

	INT ids;

	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;
	
	if(sLoanType.Compare(_T("0"))==0)
	{
		sWorkingStatus = _T("BOL211O");
	}
	
	else if(sLoanType.Compare(_T("1"))==0)
	{
		sWorkingStatus = _T("BOL213O");
	}
	
	else if(sLoanType.Compare(_T("2"))==0)
	{
		sWorkingStatus = _T("BOL212O");
	}
	
	else if(sLoanType.Compare(_T("4"))==0)
	{
		sWorkingStatus = _T("BOL217O");
	}

	if(nMode == 1)
		sWorkingStatus = _T("BOL214O");

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetSEWorkingStatus(CString &sWorkingStatus, INT nMode/*=0*/)
{
EFS_BEGIN

	INT ids;

	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;

	if(sLoanType.Compare(_T("0"))==0)
	{
		sWorkingStatus = _T("SEL311O");
	}
	
	else if(sLoanType.Compare(_T("1"))==0)
	{
		sWorkingStatus = _T("SEL313O");
	}
	
	else if(sLoanType.Compare(_T("2"))==0)
	{
		sWorkingStatus = _T("SEL312O");
	}
	
	else if(sLoanType.Compare(_T("4"))==0)
	{
		sWorkingStatus = _T("SEL317O");
	}

	if(nMode == 1)
		sWorkingStatus = _T("SEL314O");

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddMemberInfo(BOOL bReturn/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddMemberInfo");

	if(m_pManageValue->m_sUnityLoanCntYN == _T("Y"))
	{ 
		ids = SetBookStatusCnt_Unity();
		if(0 > ids) return ids;
	}
	else
	{ 
		ids = SetBookStatusCnt();
		if(0 > ids) return ids;
	}
	
	ids = SettingDelayDay(bReturn);
	if(0 > ids) return ids;
	
	CString sMemberState;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("회원상태"), sMemberState, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(sMemberState.Compare(_T("0")) == 0)
		sMemberState = _T("정상");
	else if(sMemberState.Compare(_T("1")) == 0)
		sMemberState = _T("대출정지");
	else if(sMemberState.Compare(_T("2")) == 0)
		sMemberState = _T("제적");
	else if(sMemberState.Compare(_T("3")) == 0)
		sMemberState = _T("탈퇴");

	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("회원상태설명"), sMemberState, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strQuery;
	CString strResult;
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strUserKey;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), strUserKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(!strUserKey.IsEmpty())
	{
		strQuery.Format(
			_T("SELECT ")
			_T("((SELECT NVL(SUM(ARREAR),0) FROM LS_WORK_T01 CL WHERE CL.STATUS IN ('1','R') AND CL.USER_KEY=%s)")
			_T("+(SELECT NVL(SUM(ARREAR),0) FROM CO_LOAN_TBL CL WHERE CL.STATUS IN ('1','R') AND CL.USER_KEY=%s)) ")
			_T("FROM DUAL"), strUserKey, strUserKey);
		ids = pDM->GetOneValueQuery(strQuery, strResult);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총연체료"), strResult, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	
	INT nReservationCancelLimitCnt;
	ids = m_pCheckLoan->GetReservationCancelLimitCnt(nReservationCancelLimitCnt);
	if(0 > ids) return ids;

	INT nReservationCancelLimitPeriod;
	ids = m_pCheckLoan->GetReservationCancelLimitPeriod(nReservationCancelLimitPeriod);
	if(0 > ids) return ids;
	
	
	if(0 < nReservationCancelLimitCnt && 0 < nReservationCancelLimitPeriod)
	{	
		if(!strUserKey.IsEmpty())
		{
			strQuery.Format(_T("SELECT (SELECT COUNT(1) FROM LS_WORK_T01 ")
							_T("WHERE STATUS='4' AND USER_KEY=%s AND RESERVATION_CANCEL_TYPE='0' ")
							_T("AND RESERVE_CANCEL_DATE ")
							_T("BETWEEN TO_DATE(SYSDATE-%d,'YYYY/MM/DD') ")
							_T("AND TO_DATE(SYSDATE,'YYYY/MM/DD')+0.99999)")
							_T("+")
							_T("(SELECT COUNT(1) FROM CO_LOAN_TBL ")
							_T("WHERE STATUS='4' AND USER_KEY=%s AND RESERVATION_CANCEL_TYPE='0' ")
							_T("AND RESERVE_CANCEL_DATE ")
							_T("BETWEEN TO_DATE(SYSDATE-%d,'YYYY/MM/DD') ")
							_T("AND TO_DATE(SYSDATE,'YYYY/MM/DD')+0.99999) FROM DUAL")
							, strUserKey, nReservationCancelLimitPeriod
							, strUserKey, nReservationCancelLimitPeriod);
			
			ids = pDM->GetOneValueQuery(strQuery, strResult);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
					
			ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("예약취소갯수"), strResult, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(_ttoi(strResult) >= nReservationCancelLimitCnt)
			{
				ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("예약가능유무"), _T("X"), 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}
		}
	}
		
	CESL_Mgr* pMgr = m_pParentMgr->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		pMgr->SendMessage(WM_SHOWWINDOW, SW_SHOW, 0);
		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			CString strGetData;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			if(_T("Y") == strGetData)
			{
				pMgr->SendMessage(WM_UNITY_USER_ICON_SHOW, (WPARAM)TRUE, 0);
			}
			else
			{
				pMgr->SendMessage(WM_UNITY_USER_ICON_SHOW, (WPARAM)FALSE, 0);
			}
		}
	
		CString strIpinFlag;
		GetTempData(_T("아이핀사용여부"), strIpinFlag);
		CString strMypinFlag;
		GetTempData(_T("MYPIN사용여부"), strMypinFlag);
		CString strPhoneCertifyFlag;
		GetTempData(_T("휴대폰인증사용여부"), strPhoneCertifyFlag);
		if(TRUE == m_pParentMgr->m_pInfo->m_bCertifyUse || strIpinFlag.CompareNoCase(_T("Y")) == 0 || strMypinFlag.CompareNoCase(_T("Y")) == 0 || strPhoneCertifyFlag.CompareNoCase(_T("Y")) == 0)
		{
			pMgr->SendMessage(WM_CERTIFY_ICON_SHOW, 0, 0);
		}	
		
		pMgr->SendMessage(WM_USER_INFO_RENEWAL, (WPARAM)TRUE, 0);
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse || TRUE == m_pParentMgr->m_pInfo->m_bAgreeInfoRecordYN)
		{
			CString strIsKLmemberYN;
			CString strAgentName;
			CString strBirth;
			CString strCertCode = _T("");
			CString strStatementAlias;
			CString strCertifyCode;
			INT nCode = 0;  
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("생일"), strBirth, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1021, strFuncName);
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strIsKLmemberYN, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1022, strFuncName);

			CString strGetData;
	
			CTime tCurrentTime = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strGetData);

			CString sCurrentTime = strGetData.Left(10);
			BOOL bIsUnder14YN = FALSE;
			if(10 == strBirth.GetLength())
			{
				INT nBirthYear = _ttoi(strBirth.Left(4));
				INT nBirthMonth = _ttoi(strBirth.Mid(5, 2));
				INT nBirthDay = _ttoi(strBirth.Right(2));
				INT nCurrentYear = _ttoi(sCurrentTime.Left(4));
				INT nCurrentMonth = _ttoi(sCurrentTime.Mid(5, 2));
				INT nCurrentDay = _ttoi(sCurrentTime.Right(2));
				if(14 > (nCurrentYear-nBirthYear))
				{
					bIsUnder14YN = TRUE;
				}
				else if(14 == (nCurrentYear-nBirthYear) && 0 > (nCurrentMonth-nBirthMonth))
				{
					bIsUnder14YN = TRUE;	
				}	
				else if(14 == (nCurrentYear-nBirthYear) && 0 == (nCurrentMonth-nBirthMonth) && 0 > (nCurrentDay-nBirthDay))
				{
					bIsUnder14YN = TRUE;	
				}	
				else
				{
					bIsUnder14YN = FALSE;
				}
			}

			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("동의약관"), strStatementAlias, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1025, strFuncName);
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대리인명"), strAgentName, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1026, strFuncName);
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("실명인증코드"), strCertifyCode, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1027, strFuncName);
		
			if(TRUE == m_pParentMgr->m_pInfo->m_bAgreeInfoRecordYN && _T("Y") != strIsKLmemberYN)
			{				
				if(bIsUnder14YN)
				{
					if(_T("개인정보 수집/이용") != strStatementAlias || _T("") == strAgentName)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}				
				else
				{
					if(_T("개인정보 수집/이용") != strStatementAlias)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				
				if(m_pManageValue->m_pInfo->m_bCertifyUse && _T("") != strCertifyCode)
				{
					pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
				}
			}
			else if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strIsKLmemberYN)
			{
				if(bIsUnder14YN)
				{
					if(_T("책이음서비스") != strStatementAlias || _T("") == strAgentName)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				else
				{
					if(_T("책이음서비스") != strStatementAlias)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				
				if(m_pManageValue->m_pInfo->m_bCertifyUse && _T("") != strCertifyCode)
				{
					pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
				}
			}
			else if(TRUE == m_pParentMgr->m_pInfo->m_bAgreeInfoRecordYN && _T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") != strIsKLmemberYN)
			{
				
				if(bIsUnder14YN)
				{
					if(_T("개인정보 수집/이용") != strStatementAlias || _T("") == strAgentName)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				if(bIsUnder14YN)
				{
					if(_T("개인정보 수집/이용") != strStatementAlias)
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
					}
					else
					{
						pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)2, 0);
					}
				}
				if(m_pManageValue->m_pInfo->m_bCertifyUse && _T("") != strCertifyCode)
				{
					pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)1, 0);
				}
			}
			else
			{
				pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)0, 0);
			}
		}
	}

	return 0;
 
EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::UpdateSuccessDM(INT nMode /* = 0*/, BOOL bIsILL)
{
EFS_BEGIN

	INT ids = 0;

	ids = UpdateLoanBookInfoDM(nMode);
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateUserInfoDM(INT nMode /*= 0*/, BOOL bIsILL)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateUserInfoDM");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	
	CString sBookCnt;

	if((m_pManageValue->m_bVolReserve))
	{
		TestCode();
		return 0;
	}

	if(nMode == 0)	
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("총대출책수"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
		CString sIsAppendix;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("부록여부"), sIsAppendix, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(sIsAppendix.Compare(_T("A")) == 0)
		{
			if(m_pCheckLoan->IsAppendixIncludeLoanCnt() != 0)
			{
				if(FALSE == bIsILL)
				{
					INT nBookCnt = _ttoi(sBookCnt);		
					nBookCnt = (0 < nBookCnt) ? nBookCnt-1 : nBookCnt;
					sBookCnt.Format(_T("%d"), nBookCnt);
				}
				
			}
		}

		sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)+1);
		
		if(bIsILL == TRUE)
			ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("타관총대출책수"), sBookCnt, 0);
		else
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("총대출책수"), sBookCnt, 0);		
		
		if(_T("Y") == m_pManageValue->m_sIsUnionLoanCnt)
		{
			CString strUnionCnt;

			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("통합대출책수"), strUnionCnt, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(!strUnionCnt.IsEmpty())
			{
				if(0 == sIsAppendix.Compare(_T("A")))
				{
					if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt())
					{
						strUnionCnt.Format(_T("%d"), _ttoi(strUnionCnt)+1);
					}
				}

				strUnionCnt.Format(_T("%d"), _ttoi(strUnionCnt)-1);


			}
		}	
		
	}
	else if(nMode == 1) 
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("총예약책수"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)+1);
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("총예약책수"), sBookCnt, 0);
	}
	else if(nMode == 2)	
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("총예약책수"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
		INT nBookCnt = _ttoi(sBookCnt);		
		nBookCnt = (0 < nBookCnt) ? nBookCnt-1 : nBookCnt;
		sBookCnt.Format(_T("%d"), nBookCnt);
		
		
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("총예약책수"), sBookCnt, 0);
	}
	else if(nMode == 3)  
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("총예약책수"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		INT nBookCnt = _ttoi(sBookCnt);		
		nBookCnt = (0 < nBookCnt) ? nBookCnt-1 : nBookCnt;
		sBookCnt.Format(_T("%d"), nBookCnt);
		
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("총예약책수"), sBookCnt, 0);

		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("총대출책수"), sBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		sBookCnt.Format(_T("%d"), _ttoi(sBookCnt)+1);
		ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("총대출책수"), sBookCnt, 0);

		
		
		if(_T("Y") == m_pManageValue->m_sIsUnionLoanCnt)
		{
			CString strUnionCnt;

			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("통합대출책수"), strUnionCnt, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			if(!strUnionCnt.IsEmpty())
			{
				strUnionCnt.Format(_T("%d"), _ttoi(strUnionCnt)-1);


			}
		}			
	}
	else
	{
		ids = 0;
	}

	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	return 0;	

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::UpdateLoanBookInfoDM(INT nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanBookInfoDM");

	CString sDstDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	
	CESL_DataMgr *pSrcDM = m_pParentMgr->FindDM(sSrcDMAlias);
	if(pSrcDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	const INT nCopyColumnCnt = 42;
	TCHAR *sSrcFieldAlias[nCopyColumnCnt] = 
	{
		_T("대출/반납정보KEY"), _T("종정보키"), _T("책KEY"),_T("등록번호"),_T("MARC"),
		_T("대출일"),_T("반납예정일"),_T("예약일"),_T("예약만기일"),_T("연체일수"),
		_T("대출상태"),_T("자료실구분"),_T("단행/연속구분"), 
		_T("IBS_청구_별치기호_코드"),	
		_T("IBS_청구_분류기호"),		
		_T("IBS_청구_도서기호"),		
		_T("IBS_청구_권책기호"),		
		_T("IBS_청구_복본기호"),		
		_T("책값"), 
		_T("본표제"),
		_T("부록자료수"), 
		_T("반납연기횟수"),
		_T("이전자료상태"),
		_T("상태"),
		_T("권호명"),
		_T("관리구분"), 
		_T("부록여부"), 
		_T("이전자료상태"),
		_T("대출장소"),
		_T("RFID시리얼"),
		_T("매체구분"),
		_T("대출형태"),
		_T("권호정보KEY"),
		_T("예약이용자수"),		
		_T("L_DEVICE"),
		_T("R_DEVICE"),		
		_T("L_WORKER"),
		_T("R_WORKER"),
		_T("청구기호"),		
		_T("생성일"),
		_T("소장관도서관부호"),
		_T("대출도서관")
	};

	ids = AddLoanInfo(sSrcFieldAlias, pSrcDM);
	if(0 > ids) return ids;
	
	if(0 != nMode)
	{
		ids = MakeShelfCode(sDstDMAlias);
		if(0 > ids)
		{
			return ids;
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::NoDBLoan(CString sBookRecKey/*=_T("")*/)
{
EFS_BEGIN

	INT ids;

	ids = IsNoDBLoan();
	if(0 > ids) return ids;
	if(ids > 0) return 1;
	
	if(sBookRecKey.IsEmpty())
	{
		return 0;
	}

	ids = NoDBLoanDBProc(sBookRecKey);
	if(0 > ids) return ids;
	if(ids > 0) return ids;

	m_bReturnBookNonFree = TRUE;
	ids = SearchLoanBookInfo();
	if(0 > ids) return ids;
	
	ids = AddMemberInfo();
	if(0 > ids) return ids;
	
	ids = ViewLoanScreen();
	if(0 > ids) return ids;
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::NoDBLoanDBProc(CString &sBookRecKey)
{
EFS_BEGIN

	INT ids;
	ids = Loan(sBookRecKey, 1);
	if(0 > ids) return ids;
	if(ids > 0) 
	{
		InitExpDM();
		
		return ids;
	}
	
	ids = InitExpDM();
	if(0 > ids) return ids;
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsNoDBLoan()
{	
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsNoDBLoan");
	
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sMemberDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(pDM->GetRowCount() < 1) 
	{
		SelfCloseMessageBox(_T("대출자를 먼저 선택해주십시오"));
		return 1;
	}

	BOOL IsLoan = false;
	
	ids = IsLoanMember(IsLoan);
	if(0 > ids) return ids;

	if(!IsLoan) return 1;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReturnBook(BOOL bShowWindow/*=TRUE*/,BOOL bAllReturn/*=FALSE*/, BOOL bIsILL)
{	
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReturnBook");
	
	StartLog(_T("반납:이용자검색여부검사"));
	ids = IsSearchedUser();
	EndLog();
	if(0 > ids) return ids;
	if(ids > 0) return 0;
	
	ids = IsSearchedLoanInfo();
	if(0 > ids) return -1;
	if(ids > 0) return 0;

	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));
		if(NULL != pMainGrid)
		{
			pMainGrid->SelectMakeList();
			INT nIdx = pMainGrid->SelectGetHeadPosition();
			if(pMainGrid->SelectGetCount())
			{
				do
				{
					CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));

					CString sPaymentKey, sPaymentKey2;
					sPaymentKey = pDM->GetCellData(_T("결제처리KEY"), nIdx);
					if(!sPaymentKey.IsEmpty())
					{
						for(INT j = nIdx+1; j < pDM->GetRowCount(); j++)
						{
							sPaymentKey2 = pDM->GetCellData(_T("결제처리KEY"), j);
							if(sPaymentKey == sPaymentKey2)
							{
								pMainGrid->SetAt(j, pMainGrid->m_nSelectIdx, _T(""));
							}
						}
					}
					nIdx = pMainGrid->SelectGetNext();
				}while(nIdx >= 0);
			}
		}
	}

	if(_T("Y") == m_pManageValue->m_sIsUnionLoanCnt && FALSE == bAllReturn)
	{
		CString sUnionCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합대출책수"), sUnionCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(sUnionCnt.IsEmpty())
		{
			SelfCloseMessageBox(_T("통합대출자정보 동기화 중입니다."));
			return 0;
		}
	}	
	
	INT nMode = InitReturnMode();
	if(nMode < 0) return ids;

	INT IDD_MODE;

	if(nMode == 0)
		IDD_MODE = IDD_BO_LOC_3130;
	else if(nMode == 1)
		IDD_MODE = IDD_BO_LOC_3131;
	else if(nMode == 2)
		IDD_MODE = IDD_BO_LOC_3132;
	
	if(m_pManageValue->m_sFeeMngSys == _T("Y") && FALSE == bIsILL)
 	{
 		IDD_MODE = IDD_BO_LOC_3130_2;
 	}
	
	if(_T("Y") == m_pManageValue->m_ArrearCheckYN)
	{
		IDD_MODE = IDD_BO_LOC_3130_3;
	}
	CopyGridCheckToDM();
	EndLog();

	BOOL bLastWork;
	BOOL bIsReturnCancel;	
	
	CString strQuery, strValue;

	strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL ")
				_T("WHERE CLASS_ALIAS='책두레시스템' AND VALUE_NAME='책두레반납확인' ")
				_T("AND MANAGE_CODE='%s'"), m_pParentMgr->m_pInfo->MANAGE_CODE);
				
	CESL_DataMgr pDM;
	pDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	pDM.GetOneValueQuery(strQuery, strValue);
	
	StartLog(_T("반납:반납진행"));
	CBO_LOC_3130 ReturnDlg(m_pParentMgr, IDD_MODE, this, bShowWindow, bIsILL);
	
	ReturnDlg.m_n3130ILLReturnProcCnt = 0;

	if(strValue == _T("Y"))
		ReturnDlg.ILL_ALERT = TRUE;
		ReturnDlg.m_AllReturn = bAllReturn;				

	ReturnDlg.DoModal();
	EndLog();

	bIsReturnCancel = ReturnDlg.m_bIsReturnCancel;		
	bLastWork = ReturnDlg.m_bLastWork;

	
	if(TRUE == bIsReturnCancel) return 1;
	
	if ( ReturnDlg.m_n3130ILLReturnProcCnt > 0  ) {
		m_nILLReturnProcCnt = m_nILLReturnProcCnt + ReturnDlg.m_n3130ILLReturnProcCnt;
	}

	if(m_bSimpleGrid == FALSE || m_bSimpleCmd == FALSE)
	{
		StartLog(_T("반납:그리드제외한컨트롤갱신"));
		ids = AllControlDisplayNotGrid(1);
		EndLog();
		if(0 > ids) return ids;
	}	
	
	if(m_ReceiptDlg != NULL) 
	{		
		m_pParentMgr->PostMessage(WM_RECEIPT_CHANGE_TAB, 0, 2);
	}
	
	if(m_bSimpleGrid == FALSE || m_bSimpleCmd == FALSE)
	{
		StartLog(_T("반납:그리드갱신"));
		InitGridColor();
		ViewGrid();	
		EndLog();
	}

	if(!bLastWork)
		return 1;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitReturnMode()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InitReturnMode");

	CString sReturnType;

	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sReturnType.Compare(_T("0"))==0)
	{
		return 0;
	}
	else if(sReturnType.Compare(_T("1"))==0)
	{
		return 1;
	}
	else if(sReturnType.Compare(_T("2"))==0)
	{
		return 2;
	}
	else 
	{
		return -1;
	}

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::Reservation()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("Reservation");
	
	CString sValue;
	ids = m_pCheckLoan->GetReserveMode(sValue);
	if(0 > ids) return ids;

	if(_ttoi(sValue) == 0)
	{
		ViewMessage(_T("예약 기능이 사용 불가능한 상태입니다. 관리에서 설정하십시오"));
		return 0;
	}
	
	BOOL IsReserve;
	ids = IsReserveMember(IsReserve);
	if(0 > ids) return ids;

	if(!IsReserve) return 0;

	CString strData;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("예약가능유무"), strData, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(_T("X") == strData)
	{
		INT nReservationCancelLimitPeriod;
		ids = m_pCheckLoan->GetReservationCancelLimitPeriod(nReservationCancelLimitPeriod);
		if(0 > ids) return ids;

		CString strReservationCancelCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("예약취소갯수"), strReservationCancelCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		CString strMsg;
		strMsg.Format(_T("예약취소 건수가 '%d'일 동안 '%s'회 하였기 때문에 예약을 할 수 없습니다"), nReservationCancelLimitPeriod, strReservationCancelCnt);
		SelfCloseMessageBox(strMsg);
		return 0;
	}
	
	CString sRegNo;
	ids = GetRegNo(sRegNo);
	if(0 > ids) return ids;
	if(ids > 0) return 0;

	ids = SearchBookListSelect(sRegNo);
	if(0 > ids) return ids;
	if(ids > 0) 
	{
		InitExpDM();
		return 0;
	}
	
	ids = IsReservation();
	INT nMode = ids;
	if(0 > ids) return ids;
	
	CString sStatus, sBookKey;
	ids = IsBookRelationCurrentUser(sStatus,sBookKey);
	if(0 > ids) return ids;
	if(ids > 0)
	{
		SelfCloseMessageBox(_T("이 책은 현재 대출자가 이용중인 책입니다. 예약하실 수 없습니다."));
		InitExpDM();
		return 0; 
	}
	
	ids = IsUserBookLoan(EXP_RESERVE);
	if(0 > ids) return ids;
	if(ids > 0)
	{
		InitExpDM();
		return 0;
	}

	ids = ViewReservtionInfo(nMode);
	if(0 > ids) return ids;
	if(ids > 0) 
	{
		if(ids != 2) SelfCloseMessageBox(_T("취소되었습니다."));
		InitExpDM();
		return 0;
	}

	ids = IsReLoanBook();
	if(0 > ids) return ids;
	if(ids > 0)
	{
		if(m_pCheckLoan->IsViewReLoanHistory() == 0)
		{
			ids = ViewLoanHistory(ids,1);
			if(ids == 1)
			{
				InitExpDM();

				return 0;
			}
		}
		else
		{
			if(ids == 11 || ids == 22 || ids == 33)
			{
				CString sMsg;
				if(ids == 11) sMsg.Format(_T("예전에 대출 되었던 책입니다. 예약하실 수 없습니다. "));
				if(ids == 22) sMsg.Format(_T("가족중 한명이 예전에 대출 했었던 책입니다. 예약하실 수 없습니다. "));
				if(ids == 33) sMsg.Format(_T("가족중 한명이 예약한 책입니다. 예약 하실 수 없습니다."));
				SelfCloseMessageBox(sMsg);
				InitExpDM();

				return 0;
			}			
		}
	}

	this->SetBEGIN_SBL();
	
	ids = ReservationBook();
	if(1001==ids) return ids;
	else if(0 > ids) return ids;
	
	ids = UpdateSuccessDM(1);
	if(0 > ids) return ids;
	
	ids = AddMemberInfo();
	if(0 > ids) return ids;

	ids = ViewLoanScreen(3);
	if(ids != 0) return ids;

	SelfCloseMessageBox(_T("예약되었습니다."));

	InitExpDM();
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	INT nRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nRowCount);
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchBookListSelect(CString &sRegNo)
{
EFS_BEGIN

	INT ids;

	INT nBookCnt = 0;

	InitExpDM();

	ids = SearchBookInfo(sRegNo, nBookCnt);
	if(0 > ids) return ids;

	if(nBookCnt == 0) 
	{
		SelfCloseMessageBox(_T("자료가 존재하지 않습니다. "));
		return 1;
	}
	if(nBookCnt > 1)
	{
		CLoanBookListDlg BookList(m_pParentMgr);
		BookList.DoModal();
		
		if(BookList.GetCloseStatus() == 0)
		{
			SelfCloseMessageBox(_T("취소되었습니다."));
			return 1;
		}

	}
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetRegNo(CString &sRegNo)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetRegNo");

	CString sParentCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CString sRegFieldAlias = _T("대출자번호");

	ids = MakeRegNoSpaceToZero(TRUE);
	if(0 > ids) return ids;

	ids = m_pParentMgr->GetControlData(sParentCMAlias, sRegFieldAlias, sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sRegNo.IsEmpty())
	{
		SelfCloseMessageBox(_T("예약할 책의 등록번호를 입력하십시오."));
		return 1;
	}
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReservation()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsReservation");

	CString sWorkingStatus;

	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CString sFieldAlias = _T("상태");

	CString sReserveMode;
	ids = m_pCheckLoan->GetReserveMode(sReserveMode);
	if(0 > ids) return ids;

	ids = m_pParentMgr->GetDataMgrData(sDMAlias, sFieldAlias, sWorkingStatus, 0);
	if(0 > ids) return ids;

#ifdef __K2UP__
	if(
		(sWorkingStatus.Compare(_T("BOL211O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL212O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL213O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL214O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL215O")) == 0)  	
 	||	(sWorkingStatus.Compare(_T("BOL411O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL511O")) == 0) 	
	||	(sWorkingStatus.Compare(_T("BOL611O")) == 0) 	
	||	(sWorkingStatus.Compare(_T("BOL217O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL317O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL218O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL219O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL318O")) == 0) 	
	||	(sWorkingStatus.Compare(_T("SEL311O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL312O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL313O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL314O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL315O")) == 0) 	
 	||	(sWorkingStatus.Compare(_T("SEL411O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL511O")) == 0) 	
	)
#else
	if(
	(sWorkingStatus.Compare(_T("BOL221O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL222O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL223O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL224O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL225O")) == 0) 	
 	||	(sWorkingStatus.Compare(_T("BOL411O")) == 0) 
	||	(sWorkingStatus.Compare(_T("BOL511O")) == 0) 	
	||	(sWorkingStatus.Compare(_T("BOL611O")) == 0)  	
	||	(sWorkingStatus.Compare(_T("SEL311O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL312O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL313O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL314O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL315O")) == 0) 		
 	||	(sWorkingStatus.Compare(_T("SEL411O")) == 0) 
	||	(sWorkingStatus.Compare(_T("SEL511O")) == 0)  	
	)		
#endif
	{
		return 2;
	}
	else
	{
		if(_ttoi(sReserveMode) == 2)
		{
			if(sWorkingStatus.Compare(_T("BOL112N"))==0 || 
				(sWorkingStatus.Compare(_T("SEL212N"))==0) ||
				(sWorkingStatus.Compare(_T("SEL112N"))==0)
				)
				return 2;
		}		
		else if(1 == _ttoi(sReserveMode))
		{
			if(_T("BOL112N") == sWorkingStatus || 
				_T("SEL212N") == sWorkingStatus ||
				_T("SEL112N") == sWorkingStatus)
			{
				CString strBookKey;
				ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("책KEY"), strBookKey, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
				CString strQuery, strReserveCnt;
				strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS='3'"), strBookKey);
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);
				ids = TmpDM.GetOneValueQuery(strQuery, strReserveCnt);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
				if(0 < _ttoi(strReserveCnt))
				{
					return 2;
				}
			}
		}

		return 1;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewReservtionInfo(INT nMode)
{
EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("ViewReservtionInfo");

	INT nReserveUserCnt = 0;
	INT nCanReserveBookCnt = 1;
	CString sBookKey = _T("");
	CString sSpeciesKey = _T("");
	CString sVolInfoKey = _T("");
	CString sPublishFormCode = _T("");
	CString sWhere = _T("");
	CString sReserveUserCnt = _T("");
	CString sBookCnt = _T("");
	CString sBookCntSql = _T("");
	CString sReservationDMAlias = _T("DM_RESERVATION_INFO");

	CESL_DataMgr *pReservationDM = m_pParentMgr->FindDM(sReservationDMAlias);
	if(pReservationDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("책KEY"), sBookKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(!(m_pManageValue->m_bVolReserve))
	{
		sWhere.Format(_T("CL.BOOK_KEY=%s AND CL.STATUS='3' "), sBookKey);	
	}
	else
	{
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("종정보키"), sSpeciesKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("권호정보KEY"), sVolInfoKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("단행/연속구분"), sPublishFormCode, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(sPublishFormCode == _T("NB"))
		{
			return 0;	
		}

		if(sVolInfoKey.IsEmpty())
		{
			sWhere.Format(	_T("CL.BOOK_KEY=%s AND CL.STATUS='3' AND CL.VOL_INFO IS NULL"), sBookKey);
		}
		else
		{
			sWhere.Format(	_T("CL.SPECIES_KEY=%s AND CL.VOL_INFO=%s AND CL.STATUS='3' AND CL.PUBLISH_FORM_CODE='%s' "), 
							sSpeciesKey, sVolInfoKey, sPublishFormCode);
		}
	}	

	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pReservationDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	nReserveUserCnt = pReservationDM->GetRowCount();

	ids = m_pCheckLoan->GetReserveUserCnt(sReserveUserCnt);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if((_ttoi(sReserveUserCnt)*nCanReserveBookCnt) < (nReserveUserCnt+1))
	{
		ViewMessage(_T("이자료에 대한 예약 허용 인원을 초과하여 예약이 불가능합니다."));
		return 2;
	}
	
	CString strValue1;
	ids = m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();
		
	CString strEnc;
	CStringArray asDes1,asDes2;

	INT i;
	CString strUserNo;
	for(i = 0; i < nReserveUserCnt; i++)
	{
		
		if(_T("Y") == strValue1 && FALSE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			break;
		}
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			pReservationDM->GetCellData(_T("핸드폰"), i, strEnc);
			asDes1.Add(strEnc);
			pReservationDM->GetCellData(_T("전화번호"), i, strEnc);
			asDes2.Add(strEnc);
		}

		if(_T("Y") != strValue1)
		{
			ids = pReservationDM->GetCellData(_T("대출자번호"), i, strUserNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
			ids = pReservationDM->SetCellData(_T("대출자번호"), strUserNo, i);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	}

	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < nReserveUserCnt)
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->ConnectSocket()) 
			{
				pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes1);
				pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes2);
				for(i = 0; i < nReserveUserCnt; i++)
				{
					pReservationDM->SetCellData(_T("핸드폰"), asDes1.GetAt(i), i);
					pReservationDM->SetCellData(_T("전화번호"), asDes2.GetAt(i), i);
				}
				pApi->CloseSocket();
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	SetTempData(_T("예약순위"), _T("0"));

	if(pReservationDM->GetRowCount() > 0)
	{
		CReservationInfoDlg List(m_pParentMgr);
		List.DoModal();
		if(List.GetCloseMode() == 0) 
		{
			return 1;
		}

		SetTempData(_T("예약순위"), _T("1"));
	}
	
	
	else if(nMode == 1 && m_IsReqPrior == FALSE )
	{
		CString sMode;
		m_pCheckLoan->GetReserveMode(sMode);

		if(_ttoi(sMode) == 1)
		{
			ViewMessage(_T("대출중인 자료만 예약이 가능합니다."));
		}
		else 
		{
			ViewMessage(_T("비치중이거나 대출중인 자료만 예약이 가능합니다."));
		}
		return 2;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationBook()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReservationBook");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

	ids = ReservationInfoDBUpdate(pMobileApi);
	if(1001 == ids) return ids;
	else if(0 > ids) return ids;
	
	ids = ReservationUserInfoDBUpdate(pMobileApi);
	if(0 > ids) return ids;
	
	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	pMobileApi->EndFrame();
	
	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	ShareInfoCenterUpload(_T("I"), _T("U"));
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationInfoDBUpdate(CKolas2up_MobileApi *pMobileApi, CString strKLUserYN/*=_T("N")*/)
{
EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("ReservationInfoDBUpdate");

	const INT nSrcFieldCnt = 15;
	const INT nSrcUserFieldCnt = 4;
	const INT nDeleteCnt = 10;
	
	INT nLinkYN = IDNO;
	INT nExpireDay = 0;
	CString sGetData = _T("");

	CString sPublishFormCode = _T("");
	CString sSpecieskey = _T("");
	CString sVolinfo = _T("");
	CString sMoSeDelim = _T("");
	CString sALineQuery = _T("");

	CString sHoldCnt = _T("");
	CString sRecKey = _T("");
	CString sLoanStatus = _T("3");
	CString sWorkingStatus = _T("");
	CString sReservationOrder = _T("");
	CString sExpireDate = _T("");
	CString sReservationDate = _T("");
	CString sLog = _T("");
	
	CString sDMAlias = _T("DM_BO_LOC_3100_DB");	
	CString sSrcDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CString sSrcUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	
	
	TCHAR* sSrcFieldAlias[nSrcFieldCnt] = 
	{
		_T("책KEY")			, _T("단행/연속구분")	, _T("종정보키")	, _T("본표제")	, _T("저작자")	,
		_T("권호정보KEY")	, _T("관리구분")		, _T("발행자")     , _T("자료실구분"), _T("분류기호"),
		_T("등록구분")		, _T("등록번호")		, _T("별치기호")	, _T("분류기호구분"), _T("청구기호")
	};
	TCHAR* sDstFieldAlias[nSrcFieldCnt] = 
	{
		_T("BOOK_KEY"), _T("PUBLISH_FORM_CODE")	, _T("SPECIES_KEY")			, _T("TITLE")		, _T("AUTHOR")			,
		_T("VOL_INFO"), _T("MANAGE_CODE")		, _T("PUBLISHER")		,     _T("SHELF_LOC_CODE")	, _T("CLASS_NO")	,
		_T("REG_CODE"), _T("REG_NO")			, _T("SEPARATE_SHELF_CODE")	, _T("CLASS_NO_TYPE")	, _T("CALL_NO")
	};
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{
		_T("NUMERIC")	, _T("STRING"), _T("NUMERIC")	, _T("STRING"), _T("STRING"),
		_T("NUMERIC")	, _T("STRING"), _T("STRING")	, _T("STRING"), _T("STRING"),
		_T("STRING")	, _T("STRING"), _T("STRING")	, _T("STRING"), _T("STRING") 
	};
	
	
	TCHAR *sSrcUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("대출자KEY"), _T("대출자소속"), _T("대출자직급"), _T("회원구분") 
	};
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("USER_KEY"), _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MEMBER_CLASS") 
	};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") 
	};
	
	CString sDMFieldAlias[nDeleteCnt] = 
	{
		_T("분류기호"), _T("분류기호구분"), _T("등록구분"),	_T("등록번호"), _T("별치기호"), 
		_T("청구기호"), _T("상태"), _T("책값"),	_T("이전자료상태"), _T("예약만기일")
	};
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();
	pDM->StartFrame();


	CESL_DataMgr *pKLDM = m_pParentMgr->FindDM(_T("DM_KL_LOAN"));
	if(NULL == pKLDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	for(INT i= 0 ; i < nSrcFieldCnt ;i++)
	{		
		ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, sSrcFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if((m_pManageValue->m_bVolReserve) && 1==i)
		{
			sPublishFormCode = sGetData;
		}
		else if((m_pManageValue->m_bVolReserve) && 2==i)
		{
			sSpecieskey = sGetData;
		}
		else if((m_pManageValue->m_bVolReserve) && 5==i)
		{
			sVolinfo = sGetData;
			
			if(_T("MO")==sPublishFormCode)
			{
				sMoSeDelim.Format(	_T(" FROM BO_BOOK_TBL BB,")
									_T(" IDX_BO_TBL IB")
									_T(" WHERE IB.REC_KEY=BB.SPECIES_KEY")
									_T(" AND BB.WORKING_STATUS = 'BOL112N'")
									_T(" AND BB.VOL_SORT_NO"));
			}
			else if(_T("SE")== sPublishFormCode)
			{
				sMoSeDelim.Format(	_T(" FROM SE_BOOK_TBL BB,")
									_T(" IDX_SE_TBL IB,")
									_T(" SE_VOL_TBL V")
									_T(" WHERE IB.REC_KEY=BB.SPECIES_KEY")
									_T(" AND BB.VOL_KEY=V.REC_KEY")
									_T(" AND BB.WORKING_STATUS IN ('SEL112N', 'SEL212N')")
									_T(" AND BB.VOL_KEY"));	
			}

			if(FALSE == sVolinfo.IsEmpty())
			{
				sALineQuery.Format(_T("SELECT COUNT(BB.REC_KEY)%s = %s AND BB.SPECIES_KEY = %s"), sMoSeDelim, sVolinfo, sSpecieskey); 
			}
			else
			{
				sALineQuery.Format(_T("SELECT COUNT(BB.REC_KEY)%s IS NULL AND BB.SPECIES_KEY = %s"), sMoSeDelim, sSpecieskey); 
			}

			sALineQuery	+=	_T(" AND BB.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ')");

			ids = pDM->GetOneValueQuery(sALineQuery, sHoldCnt);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
					
			if(FALSE == m_bLoanCallReserve)
			{
				INT		nHoldCnt	=	_ttoi(sHoldCnt);
				
				if(0 < nHoldCnt)
				{
					INT			nReserveMarkCnt, nPossibleLoanCnt;
					CString		strReserveMarkCnt, strReserveMarkCntQuery;
					
					CString		strBookTbl;
					if(_T("MO")==sPublishFormCode)
					{
						strBookTbl	=	_T("BO_BOOK_TBL");
					}
					else if(_T("SE")== sPublishFormCode)
					{
						strBookTbl	=	_T("SE_BOOK_TBL");
					}

					strReserveMarkCntQuery.Format(_T("SELECT COUNT(CL.REC_KEY) FROM LS_WORK_T01 CL, %s BB WHERE CL.BOOK_KEY=BB.REC_KEY AND CL.SPECIES_KEY=%s AND CL.STATUS='3' "), strBookTbl, sSpecieskey);

					if(FALSE == sVolinfo.IsEmpty())
					{
						CString		strTemp;	
						strTemp.Format(_T("AND CL.VOL_INFO=%s "), sVolinfo);
						strReserveMarkCntQuery	+=	strTemp;
					}
					else
					{
						strReserveMarkCntQuery	+=	_T("AND CL.VOL_INFO IS NULL ");
					}
					
					strReserveMarkCntQuery	+=	_T("AND CL.REG_NO IS NOT NULL AND BB.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ')");
					
					ids = pDM->GetOneValueQuery(strReserveMarkCntQuery, strReserveMarkCnt);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

					nReserveMarkCnt		=	_ttoi(strReserveMarkCnt);		
					
					nPossibleLoanCnt	=	(nHoldCnt - nReserveMarkCnt);	
					if(0 < nPossibleLoanCnt)
					{
						sLog.Format(_T("동일한 자료 중 비치중인 상태의 자료가 존재합니다.(%d건)\n")
									 _T("그래도 예약 하시겠습니까?\n\n"), nPossibleLoanCnt);
						nLinkYN = AfxMessageBox(sLog, MB_ICONQUESTION | MB_YESNO);
						if(IDNO == nLinkYN)
						{
							pDM->InitDBFieldData();
							pDM->EndFrame();
							return 1001;
						}
					}
				}
			}
			else
			{
				m_bLoanCallReserve = FALSE;
			}
			
		}
		

		else if((m_pManageValue->m_bVolReserve) && 9==i)
		{
			break;
		}
		
		if(sGetData.GetLength() > 500)
		{
			sGetData = sGetData.Mid(0,500);
			if(sGetData.GetAt(499) < 0)
			{
				if(IsDBCSLeadByteEx(0, (BYTE)sGetData.GetAt(499)) != 0)
					sGetData = sGetData.Mid(0,499);
			}
		}
 
		ids = pDM->AddDBFieldData(sDstFieldAlias[i], sDstFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	}

	for(i= 0 ; i < nSrcUserFieldCnt ;i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sSrcUserDMAlias, sSrcUserFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

		ids = pDM->AddDBFieldData(sDstUserFieldAlias[i], sDstUserFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	}

	
	ids = pMobileApi->MakeRecKey(sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출/반납정보KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sLoanStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	if(FALSE == m_pManageValue->m_bVolReserve)
	{		
		ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("상태"), sWorkingStatus, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

		GetTempData(_T("예약순위"), sReservationOrder);

		if(sReservationOrder.Compare(_T("0")) == 0)
		{
			if(	     sWorkingStatus.Compare(_T("BOL112N"))==0 
				|| 	(sWorkingStatus.Compare(_T("SEL212N"))==0) 
				||	(sWorkingStatus.Compare(_T("SEL112N"))==0)		)
			{
				ids = GetReservationExpireDay(nExpireDay);
				if(0 > ids) return ids;

				
				ids = MakeReservationExpireDate(nExpireDay, sExpireDate);
				if(0 > ids) return ids;
				
			}

			if(!sExpireDate.IsEmpty())
			{
				m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("예약만기일"), sExpireDate, 0);
				CString strStringDate;
				strStringDate.Format(_T("%s 23:59:59"), sExpireDate);
				
				sExpireDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strStringDate);

				pDM->AddDBFieldData(_T("RESERVATION_EXPIRE_DATE"), _T("NUMERIC"), sExpireDate, TRUE);

			}
		}
	}

	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출상태"), sLoanStatus, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);	

	CString strSysdate;
	CTime tCurTime = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strSysdate);
	sReservationDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strSysdate);

	ids = pDM->AddDBFieldData(_T("RESERVATION_DATE"), _T("NUMERIC"), sReservationDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("예약일"), strSysdate, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	sLog.Format(_T("%s"), m_pParentMgr->GetWorkLogMsg(_T("예약"), __WFILE__, __LINE__));
	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), sLog, TRUE);
	
	ids = pMobileApi->MakeInsertFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	pDM->InitDBFieldData();
	
	if(m_pManageValue->m_bVolReserve)
	{
		for(INT m = 0 ; m < nDeleteCnt ; m++)
		{
			ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, sDMFieldAlias[m], _T(""), 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		}
	}

	SetTempData(SHARE_CO_LOAN_TBL_INSERT_REC_KEY, CComVariant(sRecKey));
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationUserInfoDBUpdate(CKolas2up_MobileApi *pMobileApi, CString strKLUserYN/*=_T("N")*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReservationUserInfoDBUpdate");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pDM->InitDBFieldData();
	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("대출자KEY"), sRecKey, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("총예약책수"), sLoanBookCnt, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) + 1);

	
	ids = pDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sLoanBookCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("장서관리"), __WFILE__, __LINE__), TRUE);

	
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();	
	
	CString sUpdateFields = _T("RESERVATION_COUNT");
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(sUpdateFields));
	

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReserveMember(BOOL &IsReserve)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsReserveMember");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO") ;

	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(pDM->GetRowCount() != 1)
	{
		SelfCloseMessageBox(_T("대출자를 먼저 선택하십시오"));

		
		MoveRegNoToNextProcBasket(EXP_RESERVE);
		IsReserve = FALSE;
		return 0;
	}

	CString sIsLoanBookCnt;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("예약가능책수"), sIsLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(_ttoi(sIsLoanBookCnt) < 1 && m_IsReqPrior == FALSE )
	{
		SelfCloseMessageBox(_T("예약 한도를 초과하여 대출할 수 없습니다."));
		IsReserve = FALSE;
		return 0;
	}

	CString sInferiorClass;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("회원상태"), sInferiorClass, 0);
	if(0 > ids) 
	{
		IsReserve = FALSE;
		return -1;
	}

	if(sInferiorClass.Compare(_T("0")) != 0)
	{
		SelfCloseMessageBox(_T("정상 회원만이 예약이 가능합니다."));
		IsReserve = FALSE;
		return 0;
	}	
	
	if(m_pManageValue->m_sLoanIsLostUserCard == _T("Y"))
	{		
		CString sLostUserCard;
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("회원증분실"), sLostUserCard, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(sLostUserCard == _T("Y"))
		{
			SelfCloseMessageBox(_T("회원증분실 회원은 예약이 불가능 합니다."));
			IsReserve = FALSE;
			return 0;
		}
	}
	

	IsReserve = TRUE;
	return 0;

EFS_END	
return -1;
}


INT CBL_LOC_LOAN_PROC::IsBookRelationCurrentUser(CString &sStatus, CString &sBookKey)
{
EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("IsBookRelationCurrentUser");

	BOOL bEqual = FALSE;
	CString sUserKey = _T("");
	CString sSpeciesKey = _T("");
	CString sVolInfoKey = _T("");
	CString sTmpBookKey = _T("");
	CString sTmpSpeciesKey = _T("");
	CString sTmpVolInfoKey = _T("");

	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sUserKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("책KEY"), sBookKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if((m_pManageValue->m_bVolReserve))
	{
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("종정보키"), sSpeciesKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_EXP_LOAN_BOOK_INFO"), _T("권호정보KEY"), sVolInfoKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		ids = pDM->GetCellData(_T("책KEY"), i, sTmpBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = pDM->GetCellData(_T("대출상태"), i, sStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		if(m_pManageValue->m_bVolReserve)
		{
			ids = pDM->GetCellData(_T("권호정보KEY"), i, sTmpVolInfoKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			ids = pDM->GetCellData(_T("종KEY"), i, sTmpSpeciesKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		}

		if(!m_pManageValue->m_bVolReserve && sTmpBookKey == sBookKey)
		{
			
			if(_T("0") == sStatus || _T("2") == sStatus || _T("대출") == sStatus || _T("반납연기") == sStatus ||
				_T("L") == sStatus || _T("D") == sStatus || _T("O") == sStatus) 
			{
				bEqual = TRUE;
			}
			
			else if(_T("3") == sStatus || _T("예약") == sStatus || 0 != sStatus.Find(_T("예약(")))
			{
				return 2;
			}
		}

		if(m_pManageValue->m_bVolReserve &&	sSpeciesKey == sTmpSpeciesKey && sVolInfoKey == sTmpVolInfoKey)
		{
			
			if(_T("0") == sStatus || _T("2") == sStatus || _T("대출") == sStatus || _T("반납연기") == sStatus ||
				_T("L") == sStatus || _T("D") == sStatus || _T("O") == sStatus)	
			{
				if(sTmpBookKey == sBookKey)
				{
					return 1;	
				}
				else		
				{
					return 3;
				}
			}
			
			else if(_T("3") == sStatus || _T("예약") == sStatus || 0 != sStatus.Find(_T("예약(")))
			{
				return 2;
			}
			
			
		}
	}

	if(bEqual)	
	{
		return 1;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationCancel(BOOL bOption)
{
EFS_BEGIN

	CString strFuncName = _T("ReservationCancel");

	INT ids, nIndex, nCount, nUnmennedCnt_Y, nUnmennedCnt_O;
	CString sRecKey, sStatus, sReservationBookCnt, sUserRecKey, sMailSql;

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nStatusColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("대출상태"), nStatusColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	//JOB.2019.0073 : 충돌 License
// 	CLocMailMgrAPI* pMailMgr;
// 	pMailMgr = NULL;
// 	if((m_pParentMgr->m_pInfo->MODE==10000) || (m_pParentMgr->m_pInfo->MODE==30000))
// 	{
// 	}
// 	else
// 	{		
// 		if(m_pCheckLoan->IsSendCancelMail())
// 		{
// 			pMailMgr = new CLocMailMgrAPI(m_pParentMgr);
// 			pMailMgr->CreateMailInfo();
// 		}
// 	}
// 	
	nCount = 0;
	nUnmennedCnt_Y = 0;
	nUnmennedCnt_O = 0;
	CString sValue;
	for(INT i = 0 ; i < nRowCount ;i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, sRecKey);
		if(0 > ids)
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		ids = pDM->GetCellData(_T("대출상태"), nIndex, sStatus);
		if(0 > ids)
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		
		
		if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
		{
			ids = pDM->GetCellData(_T("무인대출예약상태"), nIndex, sValue);
			if(0 > ids) 
			{
				//JOB.2019.0073 : 충돌 License
// 				if(pMailMgr != NULL)
// 					delete pMailMgr;
				ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}

			
			if(sStatus.Compare(_T("3")) != 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
			
			else if(sStatus.Compare(_T("3")) == 0 && sValue.Compare(_T("O")) == 0)
			{
				pGrid->SelectGetPrev();
				nUnmennedCnt_O++;
				continue;
			}
			
			if(sStatus.Compare(_T("3")) == 0 && sValue.Compare(_T("Y")) == 0)
			{
				pGrid->SelectGetPrev();
				nUnmennedCnt_Y++;
				continue;
			}
			
			else if(sStatus.Compare(_T("3")) == 0 && sValue.IsEmpty())
			{
				pGrid->SelectGetPrev();
				nCount++;
				continue;
			}
		}
		
		else
		{
			
			if(sStatus.Compare(_T("3")) != 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
			else
			{
				nCount++;
				continue;
			}
		}
	}



	if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
	{
		if(nCount + nUnmennedCnt_Y > 0)
		{
			if(!bOption)
			{
				CString sMsg;
				sMsg.Format(_T("선정된 자료를 예약취소하시겠습니까?\r\n\r\n")
							 _T("           일반예약 : [ %d ]건\r\n")
							 _T("     무인대출신청 : [ %d ]건\r\n"), nCount, nUnmennedCnt_Y);
				if(IDYES!=MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("예약 취소"), MB_YESNO | MB_ICONQUESTION))
				{
					//JOB.2019.0073 : 충돌 License
// 					if(pMailMgr != NULL)
// 						delete pMailMgr;
					return 0;
				}
			}
		}
		else
		{
			if(nUnmennedCnt_O > 0)
			{
				MessageBox(m_pParentMgr->m_hWnd, _T("무인대출대기 및 무인대출지연 자료는 예약취소 할 수 없습니다.")
												, _T("예약취소실패"), MB_OK | MB_ICONINFORMATION);
				//JOB.2019.0073 : 충돌 License
// 				if(pMailMgr != NULL)
// 					delete pMailMgr;
				return 0;
			}
			else
			{
				MessageBox(m_pParentMgr->m_hWnd, _T("선택한 자료중 예약된 자료가 존재하지 않습니다.")
												, _T("예약취소실패"), MB_OK | MB_ICONINFORMATION);
				//JOB.2019.0073 : 충돌 License
// 				if(pMailMgr != NULL)
// 					delete pMailMgr;
				return 0;
			}
		}
	}
	else
	{
		if(nCount > 0)
		{
			
			if(!bOption)
			{
				if(IDYES != MessageBox(m_pParentMgr->m_hWnd,  _T("선정된 자료를 예약취소하시겠습니까?"),
																_T("예약 취소"), MB_YESNO | MB_ICONQUESTION))
				{
					//JOB.2019.0073 : 충돌 License
// 					if(pMailMgr != NULL)
// 						delete pMailMgr;
					return 0;
				}
			}
		}
		else
		{


			SelfCloseMessageBox(_T("선택한 자료중 예약된 자료가 존재하지 않습니다."));
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			return 0;
		}
	}
	
	pGrid->SelectMakeList();
	nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	nCount = 0;
	for(i = 0; i < nRowCount; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, sRecKey);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		ids = pDM->GetCellData(_T("대출상태"), nIndex, sStatus);
		if(0 > ids)
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		
		
		if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
		{
			ids = pDM->GetCellData(_T("무인대출예약상태"), nIndex, sValue);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			
			if(sStatus.Compare(_T("3")) != 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
			
			else if(sStatus.Compare(_T("3")) == 0 && sValue.Compare(_T("O")) == 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
		}
		
		else
		{
			
			if(sStatus.Compare(_T("3")) != 0)
			{
				pGrid->SelectGetPrev();
				continue;
			}
		}

		
		TCHAR *sFieldAlias[6] = 
		{
			_T("단행/연속구분"), _T("책KEY"), _T("예약이용자수"), _T("종KEY"), _T("권호정보KEY"), _T("등록번호")
		};
		CString sGetData[6];
		
		for(INT i = 0 ; i < 6 ; i++)
		{
			sGetData[i].Empty();
			ids = pDM->GetCellData(sFieldAlias[i], nIndex, sGetData[i]);
		}

		
		CKolas2up_MobileApi *pMobileApi;
		pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

		this->SetBEGIN_SBL();

		
		
		pDM->StartFrame();
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), _T("4"), TRUE);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

 		CString sReserveCancelDate = _T("SYSDATE");

		
		
		ids = pDM->AddDBFieldData(_T("RESERVATION_CANCEL_TYPE"), _T("STRING"), _T("1"), TRUE);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
	
		ids = pDM->AddDBFieldData(_T("RESERVE_CANCEL_DATE"), _T("NUMERIC"), sReserveCancelDate, TRUE);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}	
		
		if(bOption)
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("책두레 전환 신청"), __WFILE__, __LINE__), TRUE);
		}
		else
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약취소"), __WFILE__, __LINE__), TRUE);
		}
		
		pDM->AddDBFieldData(_T("R_DEVICE"), _T("STRING"), _T("KOLASIII"), TRUE);
			
		ids = pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}		
		
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총예약책수"), sReservationBookCnt, 0);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		sReservationBookCnt.Format(_T("%d"), _ttoi(sReservationBookCnt) - 1);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sUserRecKey, 0);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
	
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sReservationBookCnt, TRUE);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}		
		
		if(bOption)
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("책두레 전환 신청"), __WFILE__, __LINE__), TRUE);
		}
		else
		{
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약취소"), __WFILE__, __LINE__), TRUE);
		}
		
		ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserRecKey);
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pMobileApi->SendFrame();
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		pDM->EndFrame();

		SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
		SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,RESERVE_CANCEL_DATE,RESERVATION_CANCEL_TYPE,R_DEVICE")));
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserRecKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_COUNT")));

		ShareInfoCenterUpload(_T("U"), _T("U"));
				
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
		
		if(nStatusColIndex != -1)
		{
			ids = pGrid->SetAt(nIndex, nStatusColIndex, _T("예약취소"));
			if(0 > ids) 
			{
				//JOB.2019.0073 : 충돌 License
// 				if(pMailMgr != NULL)
// 					delete pMailMgr;
				ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			}
		}

		m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출상태"), _T("4"), nIndex);

		if(1 < _ttoi(sGetData[2]) && FALSE == bOption)

		{
			CString strReserveMsg;
			if(sGetData[5].IsEmpty())
			{
				strReserveMsg.Format(
					_T("예약한 이용자가 있습니다. 예약 처리를 하시겠습니까?"), sGetData[5]);
			}
			else
			{
				strReserveMsg.Format(
					_T("[%s]을 예약한 이용자가 있습니다. 예약 처리를 하시겠습니까?"), sGetData[5]);
			}
			if(IDYES == AfxMessageBox(strReserveMsg, MB_YESNO))
			{
				ReservationStatus(sGetData);
			}
		}
		
		ids = AddMemberInfo();
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			return ids;
		}

		ids = AllControlDisplayNotGrid();
		if(0 > ids) 
		{
			//JOB.2019.0073 : 충돌 License
// 			if(pMailMgr != NULL)
// 				delete pMailMgr;
			return ids;
		}
		sMailSql.Format(_T(" CL.REC_KEY= %s "), sRecKey);	
		
		if(m_pParentMgr->m_pInfo->MODE == 10000 || (m_pParentMgr->m_pInfo->MODE == 30000))
		{
		}
		else
		{
			if(m_pCheckLoan->IsSendCancelMail())
			{
				//JOB.2019.0073 : 충돌 License
				// pMailMgr->MakeRecord(6, sMailSql, FALSE); 
			}
		}
	
		pDM->DeleteRow(nIndex);		

		pGrid->SelectGetPrev();
		nCount++;

		AllControlDisplayGrid();
		InitGridColor();
		ViewGrid();	
	}
		
	if(bOption)
	{
		SelfCloseMessageBox(_T("책두레 전환 신청이 완료되었습니다."));
	}
	else
	{
		if(nCount > 0) 
		{
			SelfCloseMessageBox(_T("예약이 취소 되었습니다.\r\n체크된 부분중 예약인 자료만 예약 취소가 되었습니다."));
		}
		else 
			SelfCloseMessageBox(_T("선택한 자료중 예약된 자료가 존재하지 않습니다."));
		}
	//JOB.2019.0073 : 충돌 License
// 	if(pMailMgr != NULL)
// 		delete pMailMgr;
// 	pMailMgr = NULL;
		
	INT nLoanRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nLoanRowCount);	
	
	return 0;

EFS_END
return -1;
}

VOID CBL_LOC_LOAN_PROC::ViewMessage(CString sMsg)
{
EFS_BEGIN

	MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("대출/반납"), MB_ICONINFORMATION | MB_OK);

	InputMessageBox(sMsg);

EFS_END
}

INT CBL_LOC_LOAN_PROC::DelayLoanDate(INT nMode /*=0*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("DelayLoanDate");
	
	CString sDMAlias	= _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias	= _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias	= _T("MainGrid");
	CString sTableName	= _T("LS_WORK_T01");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	INT nRowCount = pGrid->SelectGetCount();
	if(1 > nRowCount)
	{
		SelfCloseMessageBox(_T("선정된 반납연기 자료가 없습니다."));
		return 0;
	}
	
	CString strOtherLoanKey = _T(""), strLCTKey = _T(""), strLoanLib = _T(""), strBookLib = _T(""), strReloanYN = _T("N"), strMessage = _T("");
	BOOL bOtherLoan = FALSE;
	INT nIdx = -1, j = 0, nSuccCnt = 0;
	nIdx = pGrid->SelectGetHeadPosition();
	for( j=0 ; j < pGrid->SelectGetCount() ; j++) {
		ids = pDM->GetCellData(_T("타관대출KEY"), nIdx , strOtherLoanKey);
		ids = pDM->GetCellData(_T("LCT_KEY"), nIdx , strLCTKey);		
		ids = pDM->GetCellData(_T("대출도서관부호"), nIdx , strLoanLib);
		ids = pDM->GetCellData(_T("제공도서관부호"), nIdx , strBookLib);	
		
		if ( !strOtherLoanKey.IsEmpty() ) {
			bOtherLoan = TRUE;
			BOOL bSuccess = FALSE;
			ids = KL_Reloan_Proc(nIdx, bSuccess);
			
			if ( ids < 0 ) {
				pGrid->SetAt(nIdx, pGrid->m_nSelectIdx, _T(""));
				return ids;
			}
			pGrid->SetAt(nIdx, pGrid->m_nSelectIdx, _T(""));
			if ( bSuccess == TRUE ) nSuccCnt++;
		}
		nIdx = pGrid->SelectGetNext();
	}

	if ( nSuccCnt > 0 ) {
		strMessage.Format(_T("%d건의 지역상호대차자료가 반납연기 되었습니다."), nSuccCnt);
		SelfCloseMessageBox(strMessage);
	}	
	
	ids = pGrid->SelectMakeList();
	
	nRowCount = pGrid->SelectGetCount();
	if(1 > nRowCount) {		
		return 0;
	}
		
	CString sDelayDays		= _T("");
	CString sMemberState	= _T("");
	
	for(INT i = 0; i < pDM->GetRowCount(); i++)
	{
		ids = pDM->GetCellData(_T("연체일수"), i, sDelayDays);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(0 < _ttoi(sDelayDays))
		{
			SelfCloseMessageBox(_T("연체 이용자는 반납연기가 불가능합니다."));
			return 0;
		}
	}
	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("회원상태"), sMemberState, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(_T("0") != sMemberState)
	{		
		SelfCloseMessageBox(_T("정상회원만 반납연기가 가능합니다."));
		return 0;
	}
	
	if(_T("Y") == m_pManageValue->m_sLoanIsLostUserCard)
	{
		CString sLostUserCard;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("회원증분실"), sLostUserCard, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(_T("Y") == sLostUserCard)
		{
			SelfCloseMessageBox(_T("회원증분실 회원은 반납연기가 불가능 합니다."));
			return 0;
		}
		
	}

	CString sDelayDay		= _T("");	
	CString sDelayCanCnt	= _T("");	
	INT nDelayMode			= -1;		

	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("반납연기일"), sDelayDay);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(sDelayDay.IsEmpty())
	{
		SelfCloseMessageBox(_T("반납 연기일을 입력하십시오"));
		return 0;
	}
	
	ids = m_pCheckLoan->GetDelayCnt(sDelayCanCnt);
	if(0 > ids) return ids;

	ids = GetDelayLoanMode(nDelayMode);
	if(0 > ids) return ids;
	
	TCHAR *sLinkDMFieldAlias[2] = {_T("반납예정일"), _T("대출상태")};
	INT nColIndex[2] = {-1, -1};
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, sLinkDMFieldAlias, 2, nColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sSelected			= _T("");	
	CString sGetStatus			= _T("");	
	CString sUserManageCode		= _T("");   
	CString strGetManageValue	= _T("");	
	CString sRecKey				= _T("");	
	CString sLoanDate			= _T("");	
	CString sDelayOptionDate	= _T("");	
	CString sLoanDelayDate		= _T("");	
	CString strRegNo			= _T("");	
	CString sDelayCnt			= _T("");	
	CString strMsg				= _T("");	
	CString sRegCode			= _T("");	
	CString sTmpData			= _T("");	
		
	BOOL bSkipCheck = FALSE;				
	INT nCount		= 0;
	INT nDelayDay	= 0;
	INT nIndex		= 0;
		
	CString strLoanDate;
	CString sType;

	CString strGetData;
	CTime tCurTime = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);
	CString strCurDate = strGetData.Left(10);

	CESL_DataMgr tDM;
	tDM.SetCONNECTION_INFO(m_pManageValue->m_pInfo->CONNECTION_INFO);
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		KL_StartTemp();
	}

	pGrid->SelectGetHeadPosition();	

	for(i = 0; i < nRowCount; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(0 > nIndex) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = pDM->GetCellData(_T("등록번호"), nIndex, strRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			
			ids = pDM->GetCellData(_T("현황구분"), nIndex, sType);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(_T("지역") == sType || _T("통합") == sType)
			{
				strMsg.Format(_T("%s 현황은 반납연기 할 수 없습니다."), sType);
				ViewMessage(strMsg);
				pGrid->SelectGetNext();
				continue;
			}
		}

		CString strFlag;
		m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("반납연기관리"), _T("당일반납연기허용유무"), strFlag);
		strFlag.TrimLeft();	strFlag.TrimRight();
		strFlag.MakeUpper();
		if(_T("N") == strFlag)
		{
			ids = pDM->GetCellData(_T("대출일"), nIndex, strLoanDate);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(strCurDate == strLoanDate.Mid(0,10))
			{
				strMsg.Format(_T("등록번호가 '%s'인 자료는 오늘 대출한 자료이므로 반납연기 할 수 없습니다."), strRegNo);
				ViewMessage(strMsg);
				pGrid->SelectGetNext();
				continue;
			}
		}

		ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, sRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		ids = pDM->GetCellData(_T("반납예정일"), nIndex, sLoanDate);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		INT nRegCodeLength;
		nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;

		if(strRegNo.GetLength() > nRegCodeLength)
		{
			sRegCode = strRegNo.Left(nRegCodeLength);
		}
		
		CString sDescRegCode;
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("등록구분"), sRegCode, sDescRegCode);
		if(sDescRegCode.IsEmpty())
		{
			ids = pDM->GetCellData(_T("등록구분"), nIndex, sRegCode);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		ids = pDM->GetCellData(_T("대출상태"), nIndex, sGetStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		if(_T("1") == sGetStatus || _T("3") == sGetStatus || _T("4") == sGetStatus || 
			_T("R") == sGetStatus || _T("O") == sGetStatus) 
		{
			pGrid->SelectGetNext();
			continue;
		}

		if(_T("Y") == m_pManageValue->m_sBookCooperativeSys)
		{
			ids = m_pCheckLoan->GetUserManageCode(sUserManageCode);

			CString strBookManageCode;
			ids = pDM->GetCellData(_T("관리구분"), nIndex, strBookManageCode);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			CString strBookManageCodeDesc;
			ids = pDM->GetCellData(_T("관리구분설명"), nIndex, strBookManageCodeDesc);
			
			if(strBookManageCode != sUserManageCode)
			{
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pManageValue->m_pInfo->CONNECTION_INFO);	
				CString strQuery;
				strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL ")
								_T("WHERE CLASS_ALIAS='책두레시스템' AND VALUE_NAME='타관반납연기허용유무' ")
								_T("AND MANAGE_CODE='%s'"), strBookManageCode);
				CString strValue;
				TmpDM.GetOneValueQuery(strQuery, strValue);
				if(_T("Y") != strValue)
				{
					strMsg.Format(_T("'%s'에서는 자관자료에 대해 다른 도서관에서 반납연기할 수 없도록 금지하고 있습니다.")
									, strBookManageCodeDesc);
					ViewMessage(strMsg);
					pGrid->SelectGetNext();
					continue;
				}
			}
		}
		else 
		{
			if(_T("N") == m_pManageValue->m_sIsReturnManageCode)
			{
				ids = m_pCheckLoan->GetUserManageCode(sUserManageCode);
				ids = pDM->GetCellData(_T("관리구분"), nIndex, strGetManageValue);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				if(strGetManageValue != sUserManageCode)
				{			
					strMsg.Format(_T("등록번호가 '%s'인 자료는 관리구분이 다르므로 반납연기 할 수 없습니다."), strRegNo);
					ViewMessage(strMsg);
					pGrid->SelectGetNext();
					continue;
				}
			}
		}

		if(1 == nDelayMode)
		{
			CString sBookRecKey;
			CString sSpeciesKey;
			CString sVolInfoKey;
			ids = pDM->GetCellData(_T("책KEY"), nIndex, sBookRecKey);
			ids = pDM->GetCellData(_T("종KEY"), nIndex, sSpeciesKey);
			ids = pDM->GetCellData(_T("권호정보KEY"), nIndex, sVolInfoKey);

			CString sReserveBookCnt;
			CString sVolReserveCnt = _T("");
			
			ids = IsReservatedBook(sBookRecKey, sSpeciesKey, sVolInfoKey, sReserveBookCnt, sVolReserveCnt, nIndex);
			if(0 > ids) return ids;

			if(0 < _ttoi(sReserveBookCnt) || 0 < _ttoi(sVolReserveCnt)) 
			{
				strMsg.Format(_T("등록번호가 '%s'인 자료는 다른 사용자가 예약중이므로 반납연기가 불가능합니다."), strRegNo);
				ViewMessage(strMsg);
				pGrid->SelectGetNext();
				continue;
			}		
		}
		
		ids = pDM->GetCellData(_T("반납연기횟수"), nIndex, sDelayCnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(sDelayCnt.IsEmpty())
		{
			sDelayCnt = _T("0");
		}
		if(_ttoi(sDelayCanCnt) <= _ttoi(sDelayCnt))
		{
			strMsg.Format(_T("등록번호가 '%s'인 자료는 반납연기 횟수('%s'회)를 초과하여 반납연기가 불가능합니다."), strRegNo, sDelayCnt);
			ViewMessage(strMsg);
			pGrid->SelectGetNext();
			continue;
		}

		sDelayCnt.Format(_T("%d"), _ttoi(sDelayCnt) + 1);
		
		CString sDelayReturnDate;
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			
			sDelayOptionDate = strCurDate;

 			CTime tCurrentTime = CTime::GetCurrentTime();
 			sDelayReturnDate = sDelayOptionDate + tCurrentTime.Format(_T(" %H:%M:%S"));
			if(0 == m_pCheckLoan->GetDelayOption())
			{
				sDelayOptionDate = sLoanDate;
			}
		}
		else
		{
			if(0 == m_pCheckLoan->GetDelayOption())
			{
				sDelayOptionDate = sLoanDate;
			}
			else
			{
				
				sDelayOptionDate = strCurDate;

			}
			sDelayReturnDate = _T("SYSDATE");
		}

		CTime tLoanDate(_ttoi(sDelayOptionDate.Mid(0,4)), _ttoi(sDelayOptionDate.Mid(5,2)), _ttoi(sDelayOptionDate.Mid(8,2)), 0, 0, 0);

		m_pCheckLoan->GetReturnDelayDay(sTmpData, sRegCode);
		if(!bSkipCheck)
		{
			nDelayDay = _ttoi(sTmpData);
			
			
			if(1 > nDelayDay || 30 < nDelayDay)
			{
				strMsg.Format(	_T("반납연기일이 1에서 30일의 범위에 해당하지 않습니다.\r\n")
								_T("해당직급의 반납연기일수를 확인하여 주십시오."));
				ViewMessage(strMsg);
				pGrid->SelectGetNext();
				continue;
			}
			
		}
		if(1 == m_pCheckLoan->GetAutoDelayFunc())
		{
			if(!bSkipCheck)
			{
				CInputReservatoinExpireDay InputDlg(m_pParentMgr, _T("반납 연기일 입력"));
				InputDlg.SetDay(nDelayDay);
				if(IDOK != InputDlg.DoModal())
				{
					return 0;
				}
				InputDlg.GetDay(nDelayDay);
				bSkipCheck = InputDlg.GetAllApply();
			}
		}
		
		m_pCheckLoan->GetAddHolyDay(nDelayDay, tLoanDate);

		CTimeSpan tDelayDay(nDelayDay, 0, 0, 0);
		tLoanDate += tDelayDay;
		sLoanDelayDate = tLoanDate.Format(_T("%Y/%m/%d"));
		
		
		CString sStatus = _T("2");  
		
		
		if(_T("L") == sGetStatus || _T("D") == sGetStatus)
		{
			sStatus = _T("D");
		}
		
		CKolas2up_MobileApi *pMobileApi;
		pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

		this->SetBEGIN_SBL();

		pDM->StartFrame();
		pDM->InitDBFieldData();
		ids = pDM->AddDBFieldData(_T("DELAY_CNT"), _T("NUMERIC"), sDelayCnt, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		CString strReturnPlanDate;
		if(!sLoanDelayDate.IsEmpty())
		{
			strReturnPlanDate = _T("TO_DATE('") + sLoanDelayDate + _T(" 23:59:59") + _T("', 'YYYY/MM/DD HH24:MI:SS')");
		}
		else
		{
			strReturnPlanDate = _T("NULL");
		}
		ids = pDM->AddDBFieldData(_T("RETURN_PLAN_DATE"), _T("NUMERIC"), strReturnPlanDate, TRUE);
		
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
		CString sDBDelayReturnDate;
		if ( sDelayReturnDate == _T("SYSDATE") )
		{
			sDBDelayReturnDate.Format(_T("TO_DATE(%s, 'YYYY/MM/DD HH24:MI:SS')"), sDelayReturnDate);
		}
		else
		{
			sDBDelayReturnDate.Format(_T("TO_DATE('%s', 'YYYY/MM/DD HH24:MI:SS')"), sDelayReturnDate);
		}
		

		ids = pDM->AddDBFieldData(_T("DELAY_RETURN_DATE"), _T("NUMERIC"), sDBDelayReturnDate, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->AddDBFieldData(_T("PRE_RETURN_PLAN_DATE"), _T("STRING"), sLoanDate, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sStatus, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		
		
		CString strBookLibCode, strQuery;
		strQuery.Format(_T("SELECT GET_BOOK_LIB_CODE(PUBLISH_FORM_CODE, BOOK_KEY ) FROM LS_WORK_T01 WHERE REC_KEY = %s "), sRecKey);
		ids = tDM.GetOneValueQuery(strQuery, strBookLibCode);
		
		if ( strBookLibCode.GetLength() == 0 ) {
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		
		CString strLastWork;
		if(1 == nMode)
		{
			strLastWork = m_pParentMgr->GetWorkLogMsg(_T("[ET]반납연기"), __WFILE__, __LINE__);
		}
		else
		{
			strLastWork = m_pParentMgr->GetWorkLogMsg(_T("반납연기"), __WFILE__, __LINE__);
		}
		pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), strLastWork, TRUE);
		ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		

		
		CString strTransNo;
		
		{
		if(_T("L") == sGetStatus || _T("D") == sGetStatus)
		{			
			BOOL bInsertTrans = FALSE;
			BOOL bIsEndTrans = FALSE; 
			CString strQuery = _T("");		
			CString strValue[2];
			
			strQuery.Format(_T("SELECT MAX(TRANS_NO), COUNT(1) FROM ILL_TRANS_TBL WHERE LOAN_KEY=%s"), sRecKey);
			ids = pMobileApi->GetValuesQuery(strQuery, strValue, 2);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
			strTransNo = strValue[0];
			if(_T("1") != strValue[1]) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

			strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET RETURN_PLAN_DATE=%s, LAST_WORK='%s' ")
							_T("WHERE TRANS_NO='%s';")
							, strReturnPlanDate
							, m_pParentMgr->GetWorkLogMsg(_T("책두레반납연기"), __WFILE__, __LINE__)
							, strTransNo);
			pMobileApi->AddFrame(strQuery);
		}
		}
	

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		pDM->EndFrame();

		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			
			CString strGetData;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(_T("Y") == strGetData)
			{
				CString strLoanLibCode;
 				ids = pDM->GetCellData(_T("대출도서관부호"), nIndex, strLoanLibCode);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

 				pDM->GetCellData(_T("관리구분"), nIndex, strGetData);
				if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)
				{
					
					strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
					CString strUserNo;
					ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					CString strTempDate = _T("");
					if(!sLoanDelayDate.IsEmpty())
					{
						strTempDate = sLoanDelayDate + _T(" 23:59:59");
					}

					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
					Writer.addRecord();
					
					
					
					Writer.addElement(_T("대출도서관"), strBookLibCode);
					
					Writer.addElement(_T("통합대출자번호"), strUserNo);
					Writer.addElement(_T("반납예정일"), strTempDate);
					Writer.addElement(_T("반납연기일"), sDelayReturnDate);
					
					
					
					if ( _T("D") == sStatus || _T("2") == sStatus ) {
						Writer.addElement(_T("상태"), _T("2"));
					}else{
						Writer.addElement(_T("상태"), sStatus);
					}
										
					Writer.addElement(_T("로컬KEY"), sRecKey);
					
					if ( _T("D") == sStatus ) {
						Writer.addElement(_T("지역내상호대차상태"), _T("Y"));
					}

 					CString strFrame = Writer.getFrame();
 					KL_AddTemp(strFrame);
				}
			}
		}
		
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
		
		SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
		SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("DELAY_CNT,RETURN_PLAN_DATE,DELAY_RETURN_DATE,PRE_RETURN_PLAN_DATE,STATUS")));
				
		ShareInfoCenterUpload(_T("U"), _T("N"));
		
		if(-1 != nColIndex[0])
		{
			ids = pGrid->SetAt(nIndex, nColIndex[0], sLoanDelayDate);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
	
		m_pParentMgr->SetDataMgrData(sDMAlias, _T("반납예정일"), sLoanDelayDate, nIndex);

		if(-1 != nColIndex[1])
		{
			ids = pGrid->SetAt(nIndex, nColIndex[1], sStatus);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		m_pParentMgr->SetDataMgrData(sDMAlias, _T("대출상태"), sStatus, nIndex);
		m_pParentMgr->SetDataMgrData(sDMAlias, _T("반납연기횟수"), sDelayCnt, nIndex);
		pGrid->SelectGetNext();
		ViewGrid();

		nCount++;		
	}

	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		KL_EndTemp(m_pParentMgr->GetWorkLogMsg(_T("통합회원반납연기"), __WFILE__, __LINE__));
	}

	if(0 < nCount) 
	{
		SelfCloseMessageBox(_T("반납예정일이 연기되었습니다."));
		
		ids = SearchLoanBookInfo();
		if(0 > ids) return ids;
		
		ids = AddMemberInfo();
		if(0 > ids) return ids;
		
		ids = ViewLoanScreen();
		if(0 > ids) return ids;
		
	}
	else
	{ 
		SelfCloseMessageBox(_T("반납 연기할 자료가 존재하지 않습니다."));
	}
	
	INT nLoanRowCount = pDM->GetRowCount();
	this->SetEND_SBL(nLoanRowCount);
	

	return 0;
	
EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::SettingDelayDay(BOOL bReturn/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SettingDelayDay");

	
	CString sReturnType;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sReturnDate;
	CString sReturnDateControlAlias = _T("반납일");
	if(sReturnType.Compare(_T("2"))== 0)
	{
		sReturnDateControlAlias = _T("소급반납일");
	}

	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CTime tCurrent = CTime(_ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0, 0, 0);
	
	INT nRowCount = pDM->GetRowCount();

	CString sLoanDate;
	CString sDelayDay;	
	CString sStatus;	

	INT nMaxDelayDay = 0; 
	INT nTotalDelayDay = 0; 
	CTime tTemp; 

	CString sLoanPlace,sUserManageCode,sAppendixYN;

	for(INT i = 0 ; i < nRowCount ; i++)
	{
		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
 			CString strGetData;
 			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("현황구분"), strGetData, i);
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(_T("통합") == strGetData)
			{
				continue;
			}
			else if(_T("지역") == strGetData)
			{
				if(_T("1") != m_pManageValue->m_strUserClassShare)
				{
					continue;
				}
			}
		}
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출상태"), sStatus, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);		
		
		if(0 == sStatus.Compare(_T("3")))
		{
			ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("연체일수"), _T(""), i);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}

		if((sStatus.Compare(_T("1"))==0) || (sStatus.Compare(_T("3"))==0) || (sStatus.Compare(_T("4"))==0))
		{
			continue;
		}

		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("반납예정일"), sLoanDate, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		sLoanDate.TrimLeft(); 
		sLoanDate.TrimRight();
			
		if(sLoanDate.IsEmpty()) 
		{
			continue;
		}

		CTime tLoanDate(_ttoi(sLoanDate.Mid(0,4)), _ttoi(sLoanDate.Mid(5,2)), _ttoi(sLoanDate.Mid(8,2)), 0, 0, 0);
		CTimeSpan tDelayDay(0, 0, 0, 0);

		if(tLoanDate < tCurrent)
		{
			tDelayDay = tCurrent - tLoanDate; 
		}
		INT nDelayDay = tDelayDay.GetDays();
		sDelayDay.Format(_T("%d"), nDelayDay);

	
		CString strValue = _T("");
		m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("연체관리"), _T("연체기간휴관일포함유무"), strValue);
		if( !strValue.IsEmpty() && _T("Y") != strValue)		
		{
			CString strQuery = _T("");
			CString strHolidayCount  = _T("");
			INT nHolidayCount = 0;
			CESL_DataMgr pTempDM;
			pTempDM.SetCONNECTION_INFO( m_pParentMgr->m_pInfo->CONNECTION_INFO );

			strQuery.Format(_T("SELECT GET_HOLIDAYCOUNT(TO_DATE('%s','YYYY/MM/DD'), TO_DATE('%s','YYYY/MM/DD'), '%s') FROM DUAL"), sLoanDate , sReturnDate, m_pParentMgr->m_pInfo->MANAGE_CODE);
			pTempDM.GetOneValueQuery(strQuery, strHolidayCount );
			nHolidayCount = _ttoi( strHolidayCount );

			if(nHolidayCount < nDelayDay)
			{
				sDelayDay.Format(_T("%d"), nDelayDay - nHolidayCount);
			}			
		}

		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출장소"), sLoanPlace, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("부록여부"), sAppendixYN, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		ids = m_pCheckLoan->GetUserManageCode(sUserManageCode);		
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		if(sAppendixYN.Compare(_T("A")) == 0 )
		{
			if(_T("Y") != m_pCheckLoan->m_pManageValue->m_sIsAppendixLoanStop)
			{
				continue;
			}
		}	

		ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("연체일수"), sDelayDay, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(nMaxDelayDay < nDelayDay) 
		{
			nMaxDelayDay = nDelayDay;
		}
		nTotalDelayDay += nDelayDay;
	}

	INT nDelayMode = 0;
	if(nDelayMode == 1)
	{
		nTotalDelayDay = nMaxDelayDay;
	}

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sInferiorClassAlias = _T("회원상태");
	CString sInferiorClassDBName = _T("USER_CLASS");
	CString sRecKey;

	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("대출자KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sInferiorClass = _T("0");
	CString sLoanStopDate;
	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("대출정지일"), sLoanStopDate, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	sLoanStopDate.TrimLeft();
	sLoanStopDate.TrimRight();

	if(sLoanStopDate.Find(_T("0000")) == 0)
	{
		sLoanStopDate = _T("");
		ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_LOAN_INFO"), _T("대출정지일"), sLoanStopDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	if(!sLoanStopDate.IsEmpty())
	{
		COleDateTime tLoanStopDate(_ttoi(sLoanStopDate.Mid(0,4)), _ttoi(sLoanStopDate.Mid(5,2)), _ttoi(sLoanStopDate.Mid(8,2)), 0, 0, 0);		

		if(COleDateTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), 0, 0, 0) > tLoanStopDate)
		{
			sInferiorClass = _T("0");
			sLoanStopDate = _T("");
			ids = m_pParentMgr->SetDataMgrData(sMemberDMAlias, _T("대출정지일"), _T(""), 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		}
		else
		{
			sInferiorClass = _T("1");	
		}
	}
	else
	{
		sInferiorClass = _T("0");
	}

	
	if(nTotalDelayDay > 0)
	{
		sInferiorClass = _T("1");
	}

	
	CString sPreInferiorClass;
	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, sInferiorClassAlias, sPreInferiorClass, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	BOOL bMessageView = true;
	
	if((sPreInferiorClass.Compare(sInferiorClass)==0) || (sPreInferiorClass.Compare(_T("2"))==0) || (sPreInferiorClass.Compare(_T("3"))==0) || (sPreInferiorClass.IsEmpty()))
	{
		bMessageView = false;
	}
	
	if((sPreInferiorClass.Compare(_T("2"))==0) || (sPreInferiorClass.Compare(_T("3"))==0))
	{
		sInferiorClass = sPreInferiorClass;
	}
	
	CESL_DataMgr *pMemberDM = m_pParentMgr->FindDM(sMemberDMAlias);
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1150, strFuncName);

	
	ids = pMemberDM->SetCellData(sInferiorClassAlias, sInferiorClass, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
 	if(bMessageView)
 	{
		if(_T("0") == sInferiorClass)
		{
 			
 			CKolas2up_MobileApi *pMobileApi;
 			pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pMemberDM);
 			pMemberDM->InitDBFieldData();
 			pMemberDM->StartFrame();
 			
 			CString strSysDate, strLoanStopSetDate, strType;
 			if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 			{
 				
 				CString strSysDate;
 				CTime t = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strSysDate);
 				strLoanStopSetDate = _T("TO_DATE('") + strSysDate + _T("','YYYY/MM/DD HH24:MI:SS')");
 			}
 			else
 			{
 				strLoanStopSetDate = _T("SYSDATE");
 			}
 			pMemberDM->AddDBFieldData(sInferiorClassDBName, _T("STRING"), sInferiorClass, TRUE);		
 			
 			pMemberDM->AddDBFieldData(_T("LOAN_STOP_DATE"), _T("STRING"), sLoanStopDate, TRUE);
 			
 			if(!sLoanStopDate.IsEmpty())
 			{
 				pMemberDM->AddDBFieldData(_T("LOAN_STOP_SET_DATE"), _T("NUMERIC"), strLoanStopSetDate, TRUE);
 			}
 			
 			CString sLog;
 			pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("연체일설정"), __WFILE__, __LINE__), TRUE);
 			ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey); 
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 			ids = pMobileApi->SendFrame();
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 			pMemberDM->EndFrame();
 
  			if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
  			{
  				CString strKLUserYN;
  				ids = pMemberDM->GetCellData(_T("통합회원여부"), 0, strKLUserYN);
  				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1190, strFuncName);
				CString strUserNo;
 				ids = pMemberDM->GetCellData(_T("대출자번호"), 0, strUserNo);
  				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1190, strFuncName);
  				if(_T("Y") == strKLUserYN && !strUserNo.IsEmpty())
  				{
 					
  					CFrameWriter Writer(m_pParentMgr->m_pInfo);
  					Writer.setCommand(_T("IL_K28_LOC_B01_SERVICE"));
  					Writer.addRecord();
 					Writer.addElement(_T("수정도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
  					Writer.addElement(_T("통합대출자번호"), strUserNo);
 					Writer.addElement(_T("회원상태"), sInferiorClass);
 					CString strGetData;
 					Writer.addElement(_T("최종정보수정일"), strSysDate);
 					ids = pMemberDM->GetCellData(_T("회원구분"), 0, strGetData);
  					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1190, strFuncName);
 					Writer.addElement(_T("회원구분"), strGetData);
 					if(!sLoanStopDate.IsEmpty())
 					{
 						if(10 == sLoanStopDate.GetLength())
 						{
 							CString strDate;
 							strDate = sLoanStopDate + _T(" 00:00:00");
 							Writer.addElement(_T("대출정지일"), strDate);
 							Writer.addElement(_T("대출정지부여일"), strSysDate);
 						}
 					}
  					
 					CFrameSender Sender(m_pParentMgr->m_pInfo);
 					Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("통합회원도서관관리정보수정"), __WFILE__, __LINE__));
  				}
  			}
 
 			
 			CLocCommonAPI::MobileApiEndLog(pMobileApi);
 			
 			CString sUpdateField;
 			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
 			
 			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS,  CComVariant(_T("USER_CLASS,LOAN_STOP_DATE")));
 			ShareInfoCenterUpload(_T("N"), _T("U"));
		}
 	}

	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		
		CString strGetData;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("Y") == strGetData)
		{
 			
			CString strGetUserNo;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strGetUserNo, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, _T("CBL_LOC_LOAN_PROC::SearchLoanInfo( CString &sLoanUserKey )"));
			

				if((_T("2") != sInferiorClass || _T("3") != sInferiorClass) && !sInferiorClass.IsEmpty())
				{
					CString strGetData;
					ids = pMemberDM->GetCellData(_T("대출정지유무"), 0, strGetData);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
					if(_T("Y") == strGetData)
					{
						
						if(_T("1") == m_pManageValue->m_strUserClassShare)
						{	
							bMessageView = TRUE;
							sInferiorClass = _T("1");
							ids = pMemberDM->SetCellData(_T("회원상태"), sInferiorClass, 0);
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
						}
						else if(_T("2") == m_pManageValue->m_strUserClassShare)
						{
							SelfCloseMessageBox(_T("지역내 대출정지 회원입니다."));
						}
					}
				}
				m_strUserNo = strGetUserNo;
			
		}
	}
	
	if(bMessageView)
	{
		if(sInferiorClass.Compare(_T("1"))==0) 
		{
			

		}
		else
		{
			SelfCloseMessageBox(_T("정상회원으로 변경되었습니다."));
		}
	}

	
	if(_T("Y") == m_pManageValue->m_sBookCooperativeSys)
	{
		CString strAplStopDate;
		ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("책두레_신청제한일"), strAplStopDate, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(_T("") != strAplStopDate)
		{
			COleDateTime oleAplStopDate;
			int nYear = 1, nMonth = 1, nDay = 1;
			_stscanf(strAplStopDate.GetBuffer(0), _T("%d/%d/%d"), &nYear, &nMonth, &nDay);
			
			oleAplStopDate.SetDate(nYear, nMonth, nDay);

			COleDateTime oleToday = COleDateTime::GetCurrentTime();
			oleToday.SetDate(oleToday.GetYear(), oleToday.GetMonth(), oleToday.GetDay());

			if(oleToday > oleAplStopDate)
			{
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);		
				
 				CString strQuery;
 				strQuery.Format(
 					_T("UPDATE CO_LOAN_USER_TBL CLU ")
 					_T("SET CLU.ILL_EXPIRE_RETURN_CNT = 0, ")
 						_T("CLU.ILL_APL_STOP_SET_DATE = NULL, ")
 						_T("CLU.ILL_APL_STOP_DATE = NULL ")
 					_T("WHERE REC_KEY = %s AND ")
 						_T("TO_DATE(CLU.ILL_APL_STOP_DATE, 'YYYY/MM/DD') < TO_DATE(SYSDATE, 'YYYY/MM/DD');"),
						sRecKey);
 
 					TmpDM.StartFrame();
 					TmpDM.AddFrame(strQuery);
 					TmpDM.SendFrame();
					TmpDM.EndFrame();
			}
		}
	}

	return 0;
	
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitDM()
{
EFS_BEGIN

	StartLog(_T("화면초기화:DM초기화"));

	INT ids;

	const INT nDMCnt = 5;

	CString sDMAlias[nDMCnt] =
	{
		_T("DM_BO_LOC_3100_MEMBER_INFO"),	
		_T("DM_BO_LOC_3100_LOAN_INFO"),	
		_T("DM_BO_LOC_3100_BASKET")	,		
		_T("DM_BO_LOC_3100_RETURN_INFO"),  
		_T("DM_FAMILY_ID_INFO")				
	};

	CESL_DataMgr *pDM;
	for(INT i = 0 ; i < nDMCnt ; i++)
	{
		pDM = m_pParentMgr->FindDM(sDMAlias[i]);
		if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("InitDM"));
		if (m_bReturnBookNonFree && sDMAlias[i].CompareNoCase(_T("DM_BO_LOC_3100_RETURN_INFO")) == 0)
			continue;
		pDM->FreeData();
	}

	EndLog();
	
	CESL_Mgr* pMgr = m_pParentMgr->FindSM(_T("BO_LOC_3100"));
	if(NULL != pMgr)
	{
		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			pMgr->SendMessage(WM_UNITY_USER_ICON_SHOW, (WPARAM)FALSE, 0);
		}
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bCertifyUse) pMgr->SendMessage(WM_CERTIFY_ICON_SHOW, 0, 0);
	}

	
	m_strUserNo = _T("");

	StartLog(_T("화면초기화:화면구성"));
	ids = ViewLoanScreen();
	if(0 > ids) return ids;
	EndLog();

	StartLog(_T("화면초기화:바구니초기화"));
	ClearBasket();
	EndLog();
	
	pMgr->SendMessage(WM_AGREEMENT_BTN_SHOW, (WPARAM)0, 0);
	
	pMgr->SendMessage(WM_USER_INFO_RENEWAL, (WPARAM)FALSE, 0);
	
	if(m_ReceiptDlg != NULL) m_ReceiptDlg->initData();
	
	StartLog(_T("화면초기화:가족정보초기화"));
	ClearSimpleFamilyMember();
	EndLog();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationLoan()
{
EFS_BEGIN

	INT ids;
	
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(!pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("ReservationLoan"));
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(!pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ReservationLoan"));
	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();
	if(!nRowCount)
	{
		SelfCloseMessageBox(_T("대출할 책을 선택해 주십시오"));
		return 0;
	}
	
	BOOL IsLoan = FALSE;	
	ids = IsLoanMember(IsLoan);
	if(0 > ids) return ids;
	if(!IsLoan) return 0;
	
	INT nIndex;
	CString sStatus, sIsAppendix, sRFIDSerial, sRegNo;
	INT nProcessingCnt = 0;
	pGrid->SelectGetHeadPosition();

	CString sValue, sTmp;
	
 	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 	{
 		KL_StartTemp();
 	}
	
	CString sType;
	for (INT i = 0 ; i < nRowCount ; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("대출상태"), sStatus, nIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("ReservationLoan"));

		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("부록여부"), sIsAppendix, nIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("ReservationLoan"));
			
		if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
		{
			CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
			
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("등록번호"), sRegNo, nIndex);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -7, _T("ReservationLoan"));
			
			ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("무인대출예약상태"), sValue, nIndex);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("ReservationLoan"));

			
			if(sStatus.Compare(_T("3")))
			{
				pGrid->SelectGetNext();
				continue;
			}
			
			else if(sStatus == _T("3") && sValue == _T("Y"))
			{
				sTmp.Format(_T("등록번호가 ['%s']인 자료는 무인대출신청 자료입니다.\r\n")
							 _T("대출처리 하시겠습니까?"), sRegNo);
				if(IDYES != MessageBox(m_pParentMgr->m_hWnd, sTmp, _T("경고"), MB_ICONWARNING | MB_YESNO))
				{
					pGrid->SelectGetNext();
					continue;
				}
			}
			
			else if(sStatus == _T("3") && sValue == _T("O"))
			{
				sTmp = _T("무인대출대기 및 무인대출지연 자료는 대출 할 수 없습니다.");
				MessageBox(m_pParentMgr->m_hWnd, sTmp, _T("경고"), MB_ICONWARNING);
				pGrid->SelectGetNext();
				continue;
			}
		}
		else
		{
			if(sStatus.Compare(_T("3")))
			{
				pGrid->SelectGetNext();
				continue;
			}
		}
		
		CString strBookManageCode;
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("관리구분"), strBookManageCode, nIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, _T("ReservationLoan"));
		
		if(strBookManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE && _T("") != strBookManageCode)
		{
			CString sEMsg;
			
			if(sRegNo.IsEmpty())
			
			{
				sEMsg.Format(_T("타관 예약자료입니다."));
			}
			else
			{
				sEMsg.Format(_T("등록번호['%s'] : 타관 예약자료입니다."), sRegNo);
			}
			
			SelfCloseMessageBox(sEMsg);
			pGrid->SelectGetNext();
			continue;
		}
		
		ids = IsReservationMember(nIndex);
		if(0 > ids) return ids;
		if(ids > 0)
		{
			pGrid->SelectGetNext();
			continue;
		}
		
		ids = IsReservationLoan(nIndex);
		if(0 > ids) return ids;
		if(ids > 0)
		{
			pGrid->SelectGetNext();
			continue;
		}				
		
		ids = CheckLoanBookCnt(sIsAppendix);
		if(ids > 0)
		{
			pGrid->SelectGetNext();
			continue;			
		}

		this->SetBEGIN_SBL();		
		
		ids = ReservationDBProc(nIndex);
		if(0 > ids) return ids;
		
		
		if(ids == 100 || ids == 101)
		{
			pGrid->SelectGetNext();
			continue;
		}			
			
		ids = ReservationScreen(nIndex);
		if(0 > ids) return ids;

		nProcessingCnt++;
		pGrid->SelectGetNext();
		
		ids = AppendixLoan(nIndex);
		if(0 > ids) return ids;
				
		
		ids = InitExpDM();
		if(0 > ids) return ids;
	}
 	
 	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 	{
 		KL_EndTemp(m_pParentMgr->GetWorkLogMsg(_T("통합회원예약대출"), __WFILE__, __LINE__));
 	}
	if(nProcessingCnt > 0) 
	{
		m_bReturnBookNonFree = TRUE;
		ids = SearchLoanBookInfo();
		if(0 > ids) return ids;
		
		ids = AddMemberInfo();
		if(0 > ids) return ids;
		
		ids = ViewLoanScreen();
		if(0 > ids) return ids;
		
		CString sMsg;
		sMsg.Format(_T("책이 대출 되었습니다."),nProcessingCnt);
		SelfCloseMessageBox(sMsg);

		
		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		INT nLoanRowCount = pDM->GetRowCount();
		this->SetEND_SBL(nLoanRowCount);
		
	}

	return 0;

EFS_END
	return -1;
}
INT CBL_LOC_LOAN_PROC::AllControlDisplayGrid()
{
EFS_BEGIN	

	INT ids;
	CString strFuncName = _T("AllControlDisplayGrid");

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("자료실구분"), _T("자료실"));

	INT nCnt = pDM->GetRowCount();
	CString sData, sDesc;
	
	CString strCurrentTime;
	CTime tCurrent = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strCurrentTime);

	for(INT i = 0; i < nCnt; i++)
	{
		
		sData.Empty();		sDesc.Empty();
		ids = pDM->GetCellData("관리구분", i, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("관리구분"), sData, sDesc);
		ids = pDM->SetCellData(_T("관리구분설명"), sDesc, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		sData.Empty();		sDesc.Empty();
		ids = pDM->GetCellData(_T("단행/연속구분"), i, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		CString strLILLLoanKey;
		pDM->GetCellData(_T("타관대출KEY"), i, strLILLLoanKey);
		CString strLILLTran;
		pDM->GetCellData(_T("트랜잭션상태"), i, strLILLTran);

		if(sData.Compare(_T("MO")) == 0)
		{
			sDesc = _T("단행");
		}
		else if(sData.Compare(_T("SE")) == 0)
		{
			sDesc = _T("연속");
		}
		else
		{
			if(!strLILLLoanKey.IsEmpty())
			{
				sDesc = _T("");
			}
			else
			{
				sDesc = _T("긴급");
			}

		}

		pDM->GetCellData(_T("현황구분"), i, sData);
		if(_T("지역") == sData || _T("통합") == sData)
		{
			sDesc = _T("");
		}

		ids = pDM->SetCellData(_T("자료구분설명"), sDesc, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		sData.Empty();		sDesc.Empty();
		ids = pDM->GetCellData(_T("부록여부"), i, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("A") == sData)
		{
			sDesc = _T("부록");
		}
		else
		{
			sDesc = _T("책");
		}
		ids = pDM->SetCellData(_T("부록여부설명"), sDesc, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		sData.Empty();		sDesc.Empty();
		CString sDelayDay;
		ids = pDM->GetCellData(_T("대출상태"), i, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("연체일수"), i, sDelayDay);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		if(!strLILLLoanKey.IsEmpty())
		{
			if(_T("0") == strLILLTran)
			{
				sDesc = _T("신청");
			}
			else if(_T("1") == strLILLTran)
			{
				sDesc = _T("발송");
			}
			else if(_T("3") == strLILLTran)
			{
				sDesc = _T("입수");
			}
			else if(_T("4") == strLILLTran)
			{
				sDesc = _T("타관대출");
			}
			else 
			{
				
				if ( _T("0") == sData || _T("2") == sData ) sDesc = _T("타관대출");
				else sDesc = _T("알수없음");
			}
		}
		else
		{
			if(_T("0") == sData)	
			{
				if(0 < _ttoi(sDelayDay))
				{
					sDesc = _T("연체");
				}
				else
				{
					sDesc = _T("대출");
				}
			}
			else if(_T("1") == sData)
			{
				sDesc = _T("반납");
			}
			else if(_T("2") == sData)
			{
				sDesc = _T("반납연기");
			}
			else if(_T("3") == sData)
			{
				CString sRecKey, sValue, sReservationExpireDate;
				if(_T("Y") == m_pManageValue->m_sUnmannedReserveSys)
				{
					ids = pDM->GetCellData(_T("무인대출예약상태"), i, sValue);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
					if(_T("Y") == sValue)
					{
						sDesc = _T("예약(무인대출신청)");
					}
					else if(_T("O") == sValue)
					{
						ids = pDM->GetCellData(_T("예약만기일"), i, sReservationExpireDate);
						if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
						if(sReservationExpireDate.IsEmpty())
						{
							sDesc = _T("예약");
						}
						else
						{
							CTime tExpireDate(_ttoi(sReservationExpireDate.Mid(0,4)), _ttoi(sReservationExpireDate.Mid(5,2)), 
												_ttoi(sReservationExpireDate.Mid(8,2)),_ttoi(sReservationExpireDate.Mid(11,2)), 0, 0); 
							
							if(tExpireDate < CTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), tCurrent.GetHour(), 0, 0))
							{
								sDesc = _T("예약(무인대출지연)"); 
							}
							else
							{
								sDesc = _T("예약(무인대출대기)"); 
							}
						}
					}
					else
					{
						sDesc = _T("예약");
					}
				}
				else
				{
					sDesc = _T("예약");
				}
			}
			else if(_T("4") == sData)	
			{
				sDesc = _T("예약취소");
			}
			
			else if(_T("O") == sData) 
			{
				sDesc = _T("책두레예약");
			}
			else if(_T("L") == sData) 
			{
				sDesc = _T("책두레대출");
			}
			else if(_T("D") == sData)	
			{
				sDesc = _T("책두레반납연기");
			}
			else if(_T("R") == sData)	
			{
				sDesc = _T("책두레반납");
			}
			else
			{
				sDesc = _T("");
			}
		}
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
		{
			ids = pDM->GetCellData(_T("지역내상호대차상태"), i, sData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			if(_T("Y") == sData)
			{
				sDesc = _T("지역내상호대차");
			}
		}
		
		ids = pDM->SetCellData(_T("대출상태설명"), sDesc, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	
	
	
	ids = m_pParentMgr->ControlDisplay(sCMAlias, _T("MainGrid"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = m_pParentMgr->ControlDisplay(sCMAlias, _T("MainGrid2"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewGrid()
{
EFS_BEGIN	

	DWORD nSTART,nFINISH;
	nSTART = GetTickCount();

	ViewGridThread();

	nFINISH = GetTickCount();
	
	CString strMsg;
	strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), _T("ViewGrid"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	pDM->QueryLog(strMsg.GetBuffer(0));
	
	return 0;

EFS_END
return -1;
}

UINT RunViewGridThread(LPVOID lParam)
{
	CBL_LOC_LOAN_PROC *pObj = (CBL_LOC_LOAN_PROC *)lParam;
	if(pObj == NULL)
		return -1;
	
	pObj->ViewGridThread();
	return 0;
}

UINT RunLoanBeep(LPVOID lParam)
{
	::Beep(2100,20);	
	return 0;
}
UINT RunReturnBeep(LPVOID lParam)
{
	::Beep(1900,20);	
	return 0;
}
UINT RunErrorBeep(LPVOID lParam)
{
	::Beep(2100,200);
	return 0;
}

INT CBL_LOC_LOAN_PROC::ViewGridThread()
{
EFS_BEGIN

	INT ids;
	CString sStatusDesc;
	CString sBgColor;
	CString sFontColor;
	COLORREF crBgColor, crFontColor;

	InitColorDM();

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CString sGridAlias2 = _T("MainGrid2");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, _T("ViewGridThread"));
	}
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(NULL == pGrid)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, _T("ViewGridThread"));
	}
	CESL_Grid *pGrid2 = (CESL_Grid*)(pCM->FindControl(sGridAlias2));
	if(NULL == pGrid2)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, _T("ViewGridThread"));
	}

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, _T("ViewGridThread"));
	}
	
	if(0 >= pDM->GetRowCount()) 
	{
		pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);

		return 0;
	}

	CString sColorDMAlias = _T("DM_BO_LOC_3100_COLOR_SETTING");
	CESL_DataMgr *pColorDM = m_pParentMgr->FindDM(sColorDMAlias);
	if(NULL == pColorDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, _T("ViewGridThread"));
	}

	pGrid->SetRedraw(FALSE);

	INT nPreRow = pGrid->m_nReverse+1;
	BOOL bFirst = false;

	pColorDM->GetCellData(_T("화면_바탕색"), 0, sBgColor); 
	pGrid->SetBackColorBkg(_ttoi(sBgColor));

	INT nStatusColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("대출상태"), nStatusColIndex);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, _T("ViewGridThread")); 
	}

	BOOL bGridColorView = TRUE;

	INT nPreCol = pGrid->GetCol();
	for(INT i = 1; i < pDM->GetRowCount() + 1; i++)
	{
		pGrid->SetRow(i);

		CString sType;
		CString sStatus;
		CString sDelayDay;
		ids = pDM->GetCellData(_T("부록여부"), i-1, sType);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, _T("ViewGridThread"));
		}
		ids = pDM->GetCellData(_T("대출상태"), i-1, sStatus);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, _T("ViewGridThread"));
		}
		ids = pDM->GetCellData(_T("연체일수"), i-1, sDelayDay);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, _T("ViewGridThread"));
		}

		if(_T("0") == sStatus || _T("대출") == sStatus)	
		{
			if(_T("A") == sType)
			{
				pColorDM->GetCellData(_T("부록대출_글자색"), 0, sFontColor); 
				pColorDM->GetCellData(_T("부록대출_바탕색"), 0, sBgColor); 
			}
			else
			{
				pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor); 
				pColorDM->GetCellData(_T("대출_바탕색"), 0, sBgColor); 
			}
		}
		else if(_T("1") == sStatus || _T("반납") == sStatus)	
		{
			pColorDM->GetCellData(_T("반납_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("반납_바탕색"), 0, sBgColor); 
		}
		else if(_T("2") == sStatus || _T("반납연기") == sStatus)	
		{
			if(_T("A") == sType)
			{
				pColorDM->GetCellData(_T("부록반납연기_글자색"), 0, sFontColor); 
				pColorDM->GetCellData(_T("부록반납연기_바탕색"), 0, sBgColor); 
			}
			else
			{
				pColorDM->GetCellData(_T("반납연기_글자색"), 0, sFontColor); 
				pColorDM->GetCellData(_T("반납연기_바탕색"), 0, sBgColor); 
			}
		}
		else if(_T("3") == sStatus || _T("예약") == sStatus)	
		{
			pColorDM->GetCellData(_T("예약_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("예약_바탕색"), 0, sBgColor); 
		}
		else if(_T("4") == sStatus || _T("예약취소") == sStatus)	
		{
			pColorDM->GetCellData(_T("예약취소_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("예약취소_바탕색"), 0, sBgColor); 
		}
		
		else if(_T("L") == sStatus || _T("책두레대출") == sStatus)
		{
			pColorDM->GetCellData(_T("책두레대출_글자색"), 0, sFontColor);
			pColorDM->GetCellData(_T("책두레대출_바탕색"), 0, sBgColor);

			if(_T("") == sFontColor)
			{
				pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(250, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else if(_T("D") == sStatus || _T("책두레반납연기") == sStatus)
		{
			pColorDM->GetCellData(_T("책두레반납연기_글자색"), 0, sFontColor);
			pColorDM->GetCellData(_T("책두레반납연기_바탕색"), 0, sBgColor);
			if(_T("") == sFontColor) 
			{
				pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(230, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else if(_T("O") == sStatus || _T("책두레예약") == sStatus)
		{
			pColorDM->GetCellData(_T("책두레예약_글자색"), 0, sFontColor);
			pColorDM->GetCellData(_T("책두레예약_바탕색"), 0, sBgColor);
			if(_T("") == sFontColor)
			{
				pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor);
			}
			if(_T("") == sBgColor)
			{
				COLORREF _BK = RGB(210, 200, 127);
				sBgColor.Format(_T("%d"), _BK);
			}
		}
		else
		{
			COLORREF cr = RGB(255, 255, 255);
			sBgColor.Format(_T("%d"), cr);

			cr = RGB(0,0,0);
			sFontColor.Format(_T("%d"), cr);
		}

		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			CString sType;
			pDM->GetCellData(_T("현황구분"), i-1, sType);
			if(_T("지역") == sType)
			{
				CString sFontColor2, sBgColor2;
				if(_T("0") == sStatus || _T("대출") == sStatus)	
				{
					pColorDM->GetCellData(_T("지역대출_글자색"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("지역대출_바탕색"), 0, sBgColor2); 
				}
				else if(_T("2") == sStatus || _T("반납연기") == sStatus)	
				{
					pColorDM->GetCellData(_T("지역반납연기_글자색"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("지역반납연기_바탕색"), 0, sBgColor2); 
				}
				else if(_T("3") == sStatus || _T("예약") == sStatus)	
				{
					pColorDM->GetCellData(_T("지역예약_글자색"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("지역예약_바탕색"), 0, sBgColor2); 
				}
				if(!sFontColor2.IsEmpty())
				{
					sFontColor = sFontColor2;
				}
				if(!sBgColor2.IsEmpty())
				{
					sBgColor = sBgColor2;
				}
			}
			else if(_T("통합") == sType)
			{
				CString sFontColor2, sBgColor2;
				if(_T("0") == sStatus || _T("대출") == sStatus)	
				{
					pColorDM->GetCellData(_T("센터대출_글자색"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("센터대출_바탕색"), 0, sBgColor2); 
				}
				else if(_T("2") == sStatus || _T("반납연기") == sStatus)	
				{
					pColorDM->GetCellData(_T("센터반납연기_글자색"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("센터반납연기_바탕색"), 0, sBgColor2); 
				}
				else if(_T("3") == sStatus || _T("예약") == sStatus)	
				{
					pColorDM->GetCellData(_T("센터예약_글자색"), 0, sFontColor2); 
					pColorDM->GetCellData(_T("센터예약_바탕색"), 0, sBgColor2); 
				}
				if(!sFontColor2.IsEmpty())
				{
					sFontColor = sFontColor2;
				}
				if(!sBgColor2.IsEmpty())
				{
					sBgColor = sBgColor2;
				}
			}
			
			
			CString sOtherLoanKey;
			CString sFontColor3, sBgColor3;
			pDM->GetCellData(_T("타관대출KEY"), i-1, sOtherLoanKey);
			CString strLILLTran;
			pDM->GetCellData(_T("트랜잭션상태"), i-1, strLILLTran);
			if(!sOtherLoanKey.IsEmpty())
			{
				if(_T("0") == strLILLTran)	
				{
					pColorDM->GetCellData(_T("신청_글자색"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("신청_바탕색"), 0, sBgColor3); 
				}
				else if(_T("1") == strLILLTran)	
				{
					pColorDM->GetCellData(_T("발송_글자색"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("발송_바탕색"), 0, sBgColor3); 
				}
				else if(_T("3") == strLILLTran)	
				{
					pColorDM->GetCellData(_T("입수_글자색"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("입수_바탕색"), 0, sBgColor3); 
				}
				else if(_T("4") == strLILLTran || _T("1") == sStatus )
				{
					pColorDM->GetCellData(_T("타관대출_글자색"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("타관대출_바탕색"), 0, sBgColor3); 
				}
				else if(_T("4") == strLILLTran || _T("2") == sStatus )
				{
					pColorDM->GetCellData(_T("타관대출_반납연기_글자색"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("타관대출_반납연기_바탕색"), 0, sBgColor3); 
				}
				else 
				{
					pColorDM->GetCellData(_T("대출_글자색"), 0, sFontColor3); 
					pColorDM->GetCellData(_T("대출_바탕색"), 0, sBgColor3); 
				}
				if(!sFontColor3.IsEmpty())
				{
					sFontColor = sFontColor3;
				}
				if(!sBgColor3.IsEmpty())
				{
					sBgColor = sBgColor3;
				}
			}

		}
		
		if(0 < _ttoi(sDelayDay)	&& 
		
			(_T("0") == sStatus || _T("대출") == sStatus || _T("2") == sStatus || _T("반납연기") == sStatus
			|| _T("L") == sStatus || _T("책두레대출") == sStatus || _T("D") == sStatus || _T("책두레반납연기") == sStatus))
		{
			pColorDM->GetCellData(_T("연체_글자색"), 0, sFontColor); 
			pColorDM->GetCellData(_T("연체_바탕색"), 0, sBgColor);
		}

		if(!bGridColorView)
		{
			continue;
		}
		
		crFontColor = _ttoi(sFontColor);
		crBgColor = _ttoi(sBgColor);

		INT cols;
		cols = pGrid->GetCols(0);

		if(pGrid->m_nReverse+1 != i)
		{
			pGrid->SetCol(1);
			pGrid->GetCellFontBold();
			if(crBgColor != pGrid->GetCellBackColor() || (pGrid->GetCellFontBold())) 
			{				
				for (INT j = 1 ; j < cols ; j++) 
				{
					pGrid->SetCol(j);
					pGrid->SetCellBackColor(crBgColor);
					pGrid->SetCellForeColor(crFontColor);
					pGrid->SetCellFontBold(FALSE);
					pGrid->SetCellFontItalic(FALSE);					
					ids = SetColumnColor(pDM, pGrid, i-1, j);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ViewGridThread"));
					}
				}
			}
			if(i == pDM->GetRowCount())
			{
				bFirst = true;
				i = nPreRow-1;
				if(i < 0)
				{	
					for (INT j = 1 ; j < cols ; j++) 
					{
						pGrid->SetCol(j);
						ids = SetColumnColor(pDM, pGrid, i+1, j);
						if(0 > ids)
						{
							ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ViewGridThread"));
						}
					}

					pGrid->SetRedraw(TRUE);
					pGrid->Refresh();
					pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
		
					return 0;
				}
			}
		}
		else
		{
			if(!bFirst && (i != pDM->GetRowCount())) 
			{
				continue;			
			}
			if(crBgColor != pGrid->GetCellBackColor() || (!pGrid->GetCellFontBold())) 
			{
				for (INT k = cols-1 ; k > 0 ; k--)
				{
					pGrid->SetCol(k);
					pGrid->SetCellForeColor(crFontColor);
					pGrid->SetCellBackColor(crBgColor);
					pGrid->SetCellFontBold(TRUE);
					
					ids = SetColumnColor(pDM, pGrid, i-1, k);
					if(0 > ids)
					{
						ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ViewGridThread"));
					}
				}
			}			
			pGrid->SetCol(nPreCol);
			pGrid->SetCellBackColor(crBgColor);
			pGrid->SetRedraw(TRUE);
			pGrid->Refresh();		
			
			pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);

			return 0;
		}
	}
	pGrid->Refresh();
	pGrid->SetRedraw(TRUE);
	
	pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
		
	if(m_ReceiptDlg != NULL)
	{		
		m_pParentMgr->PostMessage(WM_RECEIPT_CHANGE_TAB, 0, 1);
	}

	return 0;

EFS_END

return -1;
}


INT CBL_LOC_LOAN_PROC::IsReservationLoan(INT nIndex)
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sBookRecKey; 
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("책KEY"), sBookRecKey, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("IsReservationLoan"));
	CString sPublishForm;	
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("단행/연속구분"), sPublishForm, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("IsReservationLoan"));

	CString sBookDMAlias;	
	CString sWhere;			
	CString sRightStatus;	
	if(sPublishForm.Compare(_T("MO")) == 0)
	{
		sBookDMAlias = _T("DM_EXP_LOAN_BO_BOOK_INFO");
		sWhere.Format(_T("BB.REC_KEY = '%s' AND ")
						_T("BB.WORKING_STATUS IN ")
						_T("(")
						_T(HOLD_SEARCH_CONDITION)
						_T(",'BOL217O','BOL218O','BOL219O','BOL411O')"),
						sBookRecKey);
		sRightStatus = _T("BOL112N");
	}
	else
	{
		sBookDMAlias = _T("DM_EXP_LOAN_SE_BOOK_INFO");
		sWhere.Format(_T("IB.REC_KEY = '%s' AND ")
						_T("IB.WORKING_STATUS IN ")
						_T("(")
						_T(HOLD_SE_SEARCH_CONDITION)
						
						_T(",'SEL317O','SEL318O','SEL411O')"),
						sBookRecKey);
		sRightStatus = _T("SEL212N");
	}


	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sBookDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("IsReservationLoan"));
	
	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("IsReservationLoan"));

	INT nBookCnt = pDM->GetRowCount();

	CString sWorkingStatus;
	ids = pDM->GetCellData(_T("상태"), 0, sWorkingStatus);	
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("IsReservationLoan"));

	CString sRegNo;
	ids = pDM->GetCellData(_T("등록번호"), 0, sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -6, _T("IsReservationLoan"));

	if(sRightStatus.Find(sWorkingStatus) < 0 )
	{
		CString sMsg;
		sMsg.Format(_T("현재 선택하신 '%s'의 책상태가 대출 가능 상태가 아니어서 이 책은 대출 할 수 없습니다."), sRegNo);
		SelfCloseMessageBox(sMsg);
		return 1;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReservationMember(INT nIndex)
{
EFS_BEGIN

	INT ids = 0;
	CString sRegNo = _T("");
	CString sBookRecKey = _T("");
	CString sLoanRecKey = _T("");
	CString sWhere = _T("");
	CString sCurrentLoanRecKey = _T("");
	CString sMsg = _T("");

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("IsReservationMember"));
	
	ids = pDM->GetCellData(_T("등록번호"), nIndex, sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("IsReservationMember"));
	
	if((m_pManageValue->m_bVolReserve) && sRegNo.IsEmpty())
	{
		sMsg.Format(_T("%d번째 항목은 예약자료지정을 먼저 해주십시오."), nIndex+1);
		SelfCloseMessageBox(sMsg);
		return 1;
	}

	if(!(m_pManageValue->m_bVolReserve))
	{
		ids = pDM->GetCellData(_T("책KEY"), nIndex, sBookRecKey );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("IsReservationMember"));

		sWhere.Format(_T(" SELECT REC_KEY FROM LS_WORK_T01"
						_T(" WHERE")
						_T(" RESERVATION_DATE = (SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE STATUS='3' AND BOOK_KEY=%s)")
						_T(" AND STATUS='3' AND BOOK_KEY = %s"))
						, sBookRecKey, sBookRecKey);
		ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, sWhere, sLoanRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("IsReservationMember"));

		ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, sCurrentLoanRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("IsReservationMember"));

		if(sLoanRecKey.Compare(sCurrentLoanRecKey) != 0)
		{
			sMsg.Format(_T("등록번호가 '%s'인 책은 현재 다른 사용자가 먼저 예약을 해놓은 상태입니다."), sRegNo);
			SelfCloseMessageBox(sMsg);
			return 1;
		}
	}

	else
	{

	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::ReservationDBProc(INT nIndex, BOOL bIsILLMode)
{
EFS_BEGIN

	INT ids;

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("ReservationDBProc"));

	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

	ids = UpdateLoanRecord(nIndex, pMobileApi, bIsILLMode);
	
	if(ids == 100 || ids == 101) return ids;
	
	if(ids != 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("ReservationDBProc"));

	if(FALSE == bIsILLMode)
	{
		ids = UpdateBookInfo(nIndex, pMobileApi);
		if(ids != 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("ReservationDBProc"));
	}
	else
	{
		CString strLoanKey;
		CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, strLoanKey);
		ILL_UpdateTransInfo(FALSE, strLoanKey);
	}
	
	ids = UpdateLoanUserUpdate(nIndex, pMobileApi);
	if(ids != 0) return ids;
	
	ids = pMobileApi->SendFrame();
	if(0 > ids) 
	{
		pDM->ExecuteQuery(_T("rollback;"), FALSE, FALSE);	
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("ReservationDBProc"));
	}

	pMobileApi->EndFrame();

	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	ShareInfoCenterUpload(_T("U"), _T("U"));	
	
	return 0;

EFS_END
	return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateLoanRecord(INT nIndex, CKolas2up_MobileApi *pMobileApi, BOOL bIsILLMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanRecord");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");	
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pDM->InitDBFieldData();
	pDM->StartFrame();
	
	CString sRecKey;
	CString sSrcDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");	
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("대출/반납정보KEY"), sRecKey, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sRegCode;
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("등록번호"), sRegCode, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;

	sRegCode = sRegCode.Left(nRegCodeLength);
	
	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;

	ids = pDM->AddDBFieldData(_T("LOAN_TYPE_CODE"), _T("STRING"), sLoanType, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString strAppendYN, strLoanKey;
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("부록여부"), strAppendYN, nIndex);
	if(_T("A")==strAppendYN)
	{
		strLoanKey = _T("1");
		ids = pDM->AddDBFieldData(_T("LOAN_KEY"), _T("NUMERIC"), strLoanKey, 1);
	}

	CString sReturnExpDate;	

	ids = GetReturnExpDate(sReturnExpDate, sRegCode);
	
	
	if(ids == 100 || ids == 101) return ids;
	
	if(0 > ids) return ids;

	
	CString sTempDate;
	if(!sReturnExpDate.IsEmpty())
	{
		sTempDate = _T("TO_DATE('")+sReturnExpDate+_T(" 23:59:59")+_T("', 'YYYY/MM/DD HH24:MI:SS')");
	}
	else
	{
		sTempDate = _T("NULL");
	}
	ids = pDM->AddDBFieldData(_T("RETURN_PLAN_DATE"), _T("NUMERIC"), sTempDate, TRUE);
	
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("반납예정일"), sReturnExpDate, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sLoanStatus = (TRUE != bIsILLMode) ? _T("0") : _T("L");

	ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sLoanStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	CTime tLoanDate = CTime::GetCurrentTime();
	
	CString sLoanDate;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("소급대출일"), sLoanDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString strLoanDateString;
	strLoanDateString.Format(_T("%s %02d:%02d:%02d"), sLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute(), tLoanDate.GetSecond());
	CString sInLoanDate;
	sInLoanDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strLoanDateString);
	
	ids = pDM->AddDBFieldData(_T("LOAN_DATE"), _T("NUMERIC"), sInLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출상태"), sLoanStatus, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strDMLoanDate;
	strDMLoanDate.Format(_T("%s %02d:%02d"), sLoanDate, tLoanDate.GetHour(), tLoanDate.GetMinute());
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출일"), strDMLoanDate, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(_T("Y") == m_pManageValue->m_sUseReceipt)
	{
		ids = pDM->AddDBFieldData(_T("RECEIPT_PRINT_LOAN_YN"), _T("STRING"), _T("N"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		ids = pDM->AddDBFieldData(_T("RECEIPT_PRINT_RETURN_YN"), _T("STRING"), _T("N"));
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	

	if(sLoanType.Compare(_T("0")) == 0)		sLoanType = _T("일반");
	else if(sLoanType.Compare(_T("1")) == 0)	sLoanType = _T("특별");
	else if(sLoanType.Compare(_T("2")) == 0)	sLoanType = _T("관내");
	else if(sLoanType.Compare(_T("3")) == 0)	sLoanType = _T("무인");
	else if(sLoanType.Compare(_T("4")) == 0)	sLoanType = _T("장기");
	else										sLoanType = _T("일반");

	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("대출형태"), sLoanType, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	CString sGetData = _T("");
	CString sTotalReserveCnt = _T("");
	ids = m_pParentMgr->GetDataMgrData(sSrcDMAlias, _T("예약이용자수"), sGetData, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(_ttoi(sGetData)<1)	sTotalReserveCnt = _T("0");
	else					sTotalReserveCnt.Format(_T("%d"), (_ttoi(sGetData)-1));
	ids = m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("예약이용자수"), sTotalReserveCnt, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약대출"), __WFILE__, __LINE__), TRUE);

 	
 	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
		m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_DEVICE"), _T("KOLASIII-이동도서관"), nIndex);
	}
	else
	{
 		m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_DEVICE"), _T("KOLASIII"), nIndex);
	}
 	
 	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
 		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII-이동도서관"));
	}
	else
	{
		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII"));
	}
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	m_pParentMgr->SetDataMgrData(sSrcDMAlias, _T("L_WORKER"), m_pParentMgr->m_pInfo->USER_ID, nIndex);
	pMobileApi->AddDBFieldData(_T("L_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID);

	
	
	
	ids = pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	pDM->InitDBFieldData();

	Send3000ApiSocket(_T("대출"), sRecKey);
 	
 	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 	{
 		
 		CString strGetData;
		CString sType;
 		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 		if(_T("Y") == strGetData)
 		{
			CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
			if(NULL == pLoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 			CString strLoanLibCode;
			ids = pLoanDM->GetCellData(_T("현황구분"), nIndex, sType);
 			ids = pLoanDM->GetCellData(_T("대출도서관부호"), nIndex, strLoanLibCode);
 			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
 			if(strLoanLibCode.IsEmpty() || strLoanLibCode == m_pParentMgr->m_pInfo->LIB_CODE)
			{
				CFrameWriter Writer(m_pParentMgr->m_pInfo);
 				Writer.setCommand(_T("IL_K10_LOC_B01_SERVICE"));
 				Writer.addRecord();
				
				CString strGpinHash;
				m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("G-PINHASH인증코드"), strGpinHash, 0);
				Writer.addElement(_T("G-PINHASH인증코드"), strGpinHash);
				Writer.addElement(_T("대출도서관"), m_pParentMgr->m_pInfo->LIB_CODE);
 				CString strRegNo;
 				ids = pLoanDM->GetCellData(_T("등록번호"), nIndex, strRegNo);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("등록번호"), strRegNo);
 				CString strUserNo;
 				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("통합대출자번호"), strUserNo);
				Writer.addElement(_T("대출일"), strLoanDateString);
				Writer.addElement(_T("최종대출일"), strLoanDateString);
				CString strReturnPlanDate;
				if(!sReturnExpDate.IsEmpty())
				{
					strReturnPlanDate = sReturnExpDate+_T(" 23:59:59");
				}
 				Writer.addElement(_T("반납예정일"), strReturnPlanDate);
 				Writer.addElement(_T("상태"), sLoanStatus);
				ids = pLoanDM->GetCellData(_T("본표제"), nIndex, strGetData);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("서명"), strGetData);
				ids = pLoanDM->GetCellData(_T("청구기호"), nIndex, strGetData);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("청구기호"), strGetData);
				ids = pLoanDM->GetCellData(_T("IBS_청구_권책기호"), nIndex, strGetData);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("편권차"), strGetData);
				Writer.addElement(_T("로컬KEY"), sRecKey);
				
				ids = pLoanDM->GetCellData(_T("부록여부"), nIndex, strGetData);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
				Writer.addElement(_T("부록여부"), strGetData);

				CESL_DataMgr tempDM;
				tempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
				CString strQuery;
				strQuery.Format(_T("SELECT AUTHOR,PUBLISHER FROM LS_WORK_T01 WHERE REC_KEY=%s"), sRecKey);
				tempDM.RestructDataManager(strQuery);
				tempDM.GetCellData(0, 0, strGetData);
				Writer.addElement(_T("저자"), strGetData);
				tempDM.GetCellData(0, 1, strGetData);
				Writer.addElement(_T("발행자"), strGetData);				
  				CString strFrame = Writer.getFrame();
  				KL_AddTemp(strFrame);
 			}
 		}
 	}
	
	SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_TYPE_CODE,RETURN_PLAN_DATE,STATUS,LOAN_DATE,L_DEVICE,L_WORKER")));
	
	
	return 0;

EFS_END
	return -1;
}


INT CBL_LOC_LOAN_PROC::UpdateBookInfo(INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateBookInfo");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();

	CString sTableName;
	CString sWorkingStatus;

	CString sLoanInfoDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("단행/연속구분"), sTableName, nIndex);
	
	INT nBookType = 0;

	if(sTableName.Compare(_T("MO"))== 0)
	{
		sTableName = _T("BO_BOOK_TBL");
		ids = GetBOWorkingStatus(sWorkingStatus);
		if(0 > ids) return ids;

		nBookType = 1;
	}
	else
	{
		sTableName = _T("SE_BOOK_TBL");
		ids = GetSEWorkingStatus(sWorkingStatus);
		if(0 > ids) return ids;

		nBookType = 2;
	}

	
	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("책KEY"), sRecKey, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
#ifdef __K2UP__
	CString sPreWorkingStatus;
	ids = m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("상태"), sPreWorkingStatus, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	pDM->AddDBFieldData(_T("PREV_WORKING_STATUS"), _T("STRING"), sPreWorkingStatus, TRUE);
#endif

	ids = pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sWorkingStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약작업"), __WFILE__, __LINE__), TRUE);

	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	pDM->InitDBFieldData();
	
	CString sSpeciesKey;
	m_pParentMgr->GetDataMgrData(sLoanInfoDMAlias, _T("종KEY"), sSpeciesKey, 0);
	AddIdxQueryFrame(pDM, nBookType, sSpeciesKey);
	
	ids = m_pParentMgr->SetDataMgrData(sLoanInfoDMAlias, _T("상태"), sWorkingStatus, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateLoanUserUpdate(INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanUserUpdate");

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	pDM->InitDBFieldData();
	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sReservationCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("대출자KEY"), sRecKey, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("총대출책수"), sLoanBookCnt, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("총예약책수"), sReservationCnt, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

	sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) + 1);

	if(1 > _ttoi(sLoanBookCnt))
	{
		sLoanBookCnt = _T("0");
	}
	ids = pDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanBookCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
	
	CString sLastLoanDate = _T("SYSDATE");
	ids = pDM->AddDBFieldData(_T("LAST_LOAN_DATE"), _T("NUMERIC"), sLastLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);
	
	INT nReservationCnt = _ttoi(sReservationCnt);
	nReservationCnt = (0 < nReservationCnt) ? nReservationCnt-1 : nReservationCnt;
	sReservationCnt.Format(_T("%d"), nReservationCnt);
	
	ids = pDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sReservationCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약대출"), __WFILE__, __LINE__), TRUE);

	
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);

	pDM->InitDBFieldData();

	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT,LAST_LOAN_DATE,RESERVATION_COUNT")));
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationScreen(INT nIndex)
{
EFS_BEGIN

	INT ids;

	ids = UpdateLoanInfoDMToCM(nIndex);
	if(0 > ids) return ids;

	ids = AddMemberInfo();
	if(0 > ids) return ids;

	ids = AllControlDisplayNotGrid(0);
	if(0 > ids) return ids;
	
	AllControlDisplayGrid();
	ViewGrid();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateLoanInfoDMToCM(INT nIndex)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateLoanInfoDMToCM");

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));

	const INT nFieldCnt = 4;
	TCHAR *sFieldAlias[nFieldCnt] =
	{
		_T("대출일"), _T("반납예정일"), _T("대출상태"), _T("대출장소")
	};
	
	INT nCol[nFieldCnt] =
	{
		-1, -1, -1, -1
	};

	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, sFieldAlias, nFieldCnt, nCol);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName); 

	CString sData;
	for(INT i = 0 ; i < 3 ;i++)
	{
		ids = pDM->GetCellData(sFieldAlias[i], nIndex, sData);

		if(nCol[i] !=  -1)
		{
			ids = pGrid->SetAt(nIndex, nCol[i], sData);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
		}
		
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetReturnExpDate(CString &sReturnExpDate, CString sRegCode)
{
EFS_BEGIN

	INT ids;
	
	CString sLoanDate;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("소급대출일"), sLoanDate);
	
	CTime tReturnExpDate(_ttoi(sLoanDate.Mid(0,4)), _ttoi(sLoanDate.Mid(5,2)), _ttoi(sLoanDate.Mid(8,2)), 0, 0, 0);
	
	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;
	
	INT nExpDay;
	ids = m_pCheckLoan->GetReturnExpDay(nExpDay, sRegCode, tReturnExpDate);
	
	if(nExpDay == 0)
	{
		if(_T("1") != sLoanType)
		{
			SelfCloseMessageBox(_T("대출기한이 '0'일인 자료는 대출할 수 없습니다. \r\n대출하기 위해서는 관리에서 설정해주십시오"));
			return 101;
		}
		else
		{
			ids = m_pCheckLoan->GetReturnExpDay(nExpDay, sRegCode, tReturnExpDate, FALSE);
		}
	}
	
	if(sLoanType.Compare(_T("2")) == 0) 
		nExpDay = 0;
	
	else if(sLoanType.Compare(_T("4")) == 0) 
	{		
		CBO_LOC_LOAN_DATE dlg;
		dlg.SetLoanExpDate(nExpDay);
		if(dlg.DoModal() == IDOK)
			nExpDay = dlg.GetLoanExpDate();
		else 
		{
			SelfCloseMessageBox(_T("작업을 취소하였습니다."));
			return 100;
		}
	}
	
	CTimeSpan tExpDay(nExpDay, 0, 0, 0);
	tReturnExpDate += tExpDay;
	sReturnExpDate = tReturnExpDate.Format(_T("%Y/%m/%d"));
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReservatedBook(CString &sBookRecKey, CString &sSpeciesKey, CString &sVolInfoKey, CString &sReserveBookCnt, CString &sVolReserveCnt, INT nIndex)
{
EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("IsReservatedBook");

	CString sWhere = _T("");
	
	CString sDMAlias;
	sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(nIndex == -1)
	{
		nIndex = 0;

		ids = pDM->GetCellData(_T("예약건수"), nIndex, sReserveBookCnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	
	else
	{	
		CESL_DataMgr *pDM_Loan = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		CString strOtherLoanYN;
		ids = pDM_Loan->GetCellData(_T("타관대출KEY"),nIndex,strOtherLoanYN);
		if(!strOtherLoanYN.IsEmpty())
		{
		}
		else
		{
		if(!(m_pManageValue->m_bVolReserve))
			sWhere.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS='3'"), sBookRecKey);
		else if((m_pManageValue->m_bVolReserve))
		{
			sWhere.Format(	_T(" SELECT COUNT(1) FROM LS_WORK_T01"
							   _T(" WHERE SPECIES_KEY=%s AND STATUS='3' AND REG_NO IS NULL AND VOL_INFO")), sSpeciesKey);
			if(sVolInfoKey.IsEmpty())
				sWhere += _T(" IS NULL");
			else
				sWhere += _T(" = ") + sVolInfoKey;
		}
		ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, sWhere, sVolReserveCnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	}

	if((m_pManageValue->m_bVolReserve))
	{
		sWhere.Format(_T(" SELECT COUNT(1) FROM LS_WORK_T01 WHERE BOOK_KEY=%s AND STATUS='3' AND REG_NO IS NOT NULL"), sBookRecKey);
		ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, sWhere, sReserveBookCnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
		

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetDelayLoanMode(INT &nDelayMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetDelayLoanMode");

	ids = m_pCheckLoan->GetDelayLoanMode(nDelayMode);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::LoanReservationInSearchBookList(CString strRegNo, CString strPublisherFormCode, INT nMode, BOOL IsReqPrior)
{
EFS_BEGIN
	
	INT	ids;
	CString strFuncName = _T("LoanReservationInSearchBookList");

	ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(ids > 0)
	{
		if(nMode == 11)
			MoveRegNoToNextProcBasket(EXP_LOAN);
		else if(nMode == 21)
			MoveRegNoToNextProcBasket(EXP_RESERVE);

		return 0;
	}

	if(nMode == 11)
	{
		ids = SPFExcutePath(_T("대출"));
		if(0 > ids) return ids;
	}
	
	else if(nMode == 21)
	{		
		m_IsReqPrior = IsReqPrior;
		
		ids = SPFExcutePath(_T("예약"));
		if(0 > ids) return ids;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchBookInfo()
{
EFS_BEGIN

	INT ids;
	CString sRegNo,sPublishForm;
	ids = SearchUserBook(0, sRegNo, sPublishForm);
	if(0 > ids) return ids;

	
	if(ids == 3) return 0;
	
	if(ids == 1) return 0;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetSqlSearchBookInfo(CString &sWhere, INT nMode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetSqlSearchBookInfo");

	INT nSearchField = 0;

	CString sCMAlias = _T("CM_BO_LOC_3110");
	const INT nFieldCnt = 4;
	
	TCHAR *sFieldAlias[nFieldCnt] = 
	{
		_T("책제목"), _T("저자명"), _T("발행자명"), _T("전체")
	};

	CString sGetData[nFieldCnt];

	for(INT i = 0 ; i < nFieldCnt ;i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sFieldAlias[i], sGetData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		sGetData[i].TrimRight();
	}

	CMakeSearchData MakeSearchData(m_pParentMgr);

	FILE *fd = _tfopen(_T("..\\CFG\\_newecosearch"), _T("rb"));

	CString sEcoSearchWhere;	
	INT nEcoSearchCnt = 0;
		
	BOOL bIsEqualSearch;
	if(!sGetData[0].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		
		bIsEqualSearch = FALSE;
		
		if(sGetData[0].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[0]);
		if(!bIsEqualSearch) 
			sGetData[0] += _T("*");
		
		if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
		{
			if(fd != NULL)
			{
				
				if(nMode == 0)
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
				else
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
			}
			else
			{
				if(nMode == 0)
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_TITLE, '") + sGetData[0] + _T("') ");
				else
					sEcoSearchWhere += _T(" &ECOSEARCH(, SE_IDX_TITLE, '") + sGetData[0] + _T("') ");
				
				nEcoSearchCnt++;
				
			}
		}
		else
			sWhere += _T(" CATSEARCH(IB.IDX_TITLE, '") + sGetData[0] + _T("',NULL) > 0 ");

		nSearchField++;
	}

	if(!sGetData[1].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");

		bIsEqualSearch = FALSE;
		
		if(sGetData[1].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		MakeSearchData.GetKeyWord(sGetData[1]);
		
		if(!bIsEqualSearch) 
			sGetData[1] += _T("*");

		if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
		{
			if(fd != NULL)
			{
				
				if(nMode == 0)
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
				else
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
				
			}
			else
			{
				if(!sEcoSearchWhere.IsEmpty())
					sEcoSearchWhere += _T(" INTERSECT ");
				
				if(nMode == 0)
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
				else
					sEcoSearchWhere += _T(" &ECOSEARCH(, SE_IDX_AUTHOR, '") + sGetData[1] + _T("') ");
				
				nEcoSearchCnt++;
			}
			
		}
		else
			sWhere += _T(" CATSEARCH(IB.IDX_AUTHOR, '") + sGetData[1] + _T("',NULL) > 0 ");

		nSearchField++;
	}

	if(!sGetData[2].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");

		bIsEqualSearch = FALSE;
		
		if(sGetData[2].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[2]);
		if(!bIsEqualSearch) 
			sGetData[2] += _T("*");
		if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
		{
			
			if(fd != NULL)
			{
				if(nMode == 0)
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_PUBLISHER, '") + sGetData[2] + _T("') ");
				else
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, SE_IDX_PUBLISHER, '") + sGetData[2] + _T("') ");
			}
			else
			{
				if(!sEcoSearchWhere.IsEmpty())
					sEcoSearchWhere += _T(" INTERSECT ");
				
				if(nMode == 0)
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_PUBLISHER, '") + sGetData[2] + _T("') ");
				else
					sEcoSearchWhere += _T(" &ECOSEARCH(, SE_IDX_PUBLISHER, '") + sGetData[2] + _T("') ");
				
				nEcoSearchCnt++;
			}
		}
		else			
			sWhere += _T(" CATSEARCH(IB.IDX_PUBLISHER, '") + sGetData[2] + _T("',NULL) > 0 ");
		
		nSearchField++;
	}

	
	if(!sGetData[3].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");

		bIsEqualSearch = FALSE;
		
		if(sGetData[3].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sGetData[3]);
		if(!bIsEqualSearch) 
			sGetData[3] += _T("*");
		if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
		{
			
			if(fd != NULL)
			{
				if(nMode == 0)
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_ALLITEM, '") + sGetData[3] + _T("') ");
				else
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, SE_IDX_ALLITEM, '") + sGetData[3] + _T("') ");
			}
			else
			{
				if(!sEcoSearchWhere.IsEmpty())
					sEcoSearchWhere += _T(" INTERSECT ");
				
				if(nMode == 0)
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_ALLITEM, '") + sGetData[3] + _T("') ");
				else
					sEcoSearchWhere += _T(" &ECOSEARCH(, SE_IDX_ALLITEM, '") + sGetData[3] + _T("') ");
				
				nEcoSearchCnt++;
			}
		}
		else			
			sWhere += _T(" CATSEARCH(IB.IDX_ALL_ITEM, '") + sGetData[3] + _T("',NULL) > 0 ");
		
		nSearchField++;
	}

	
	const INT nComboFieldCnt = 3;
	TCHAR *sComboAlias[nComboFieldCnt] =
	{
		_T("자료형태"), _T("매체구분"), _T("자료실")
	};

	TCHAR *sDBFieldAlias[ nComboFieldCnt ] =
	{
		_T("IB.FORM_CODE"), _T("IB.MEDIA_CODE"), _T("BB.SHELF_LOC_CODE")
	};

	CString sComboGetData[nComboFieldCnt];

	for(i = 0 ; i < nComboFieldCnt ; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sComboAlias[i], sComboGetData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		sComboGetData[i].TrimRight();
		if(sComboGetData[i].IsEmpty() || (sComboGetData[i].Compare(_T("0"))==0) || (sComboGetData[i].Compare(_T("적용안함"))==0)) continue;
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		sWhere += sDBFieldAlias[i] + CString(_T(" = '")) + sComboGetData[i] + CString(_T("' "));
	}	
	
	if(nMode == 1)
	{ 
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		
		sWhere += _T(" BB.BINDED_BOOK_YN != 'Y' ");
	}	
	
	const INT nKeyWordFieldCnt = 5;
	TCHAR *sKeyWordAlias[nKeyWordFieldCnt] =
	{
		_T("키워드1"), _T("키워드2"), _T("키워드3"), _T("키워드4"), _T("키워드5")
	};
	CString sKeyWordGetData[nKeyWordFieldCnt];

	for(i = 0 ; i < nKeyWordFieldCnt ; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sKeyWordAlias[i], sKeyWordGetData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		sKeyWordGetData[i].TrimRight();
	}
	
	CString sKeyWordWhere;
	if(!sKeyWordGetData[0].IsEmpty())
	{
		if(fd != NULL)
			sWhere += _T("(");

		bIsEqualSearch = FALSE;
		
		if(sKeyWordGetData[0].GetAt(0) == '\"')
			bIsEqualSearch = TRUE;
		
		MakeSearchData.GetKeyWord(sKeyWordGetData[0]);
		if(!bIsEqualSearch) 
			sKeyWordGetData[0] += _T("*");
		sKeyWordWhere = sKeyWordGetData[0];

		if(!sKeyWordGetData[2].IsEmpty())
		{
			bIsEqualSearch = FALSE;
			
			if(sKeyWordGetData[2].GetAt(0) == '\"')
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sKeyWordGetData[2]);
			if(!bIsEqualSearch) 
				sKeyWordGetData[2] += _T("*");
							
			sKeyWordGetData[1].MakeUpper();
			if(sKeyWordGetData[1].Compare(_T("OR")) == 0)
				sKeyWordWhere += _T(" | ");
			else
				sKeyWordWhere += _T(" ");
			sKeyWordWhere += sKeyWordGetData[2];				
		}
		if(!sKeyWordGetData[4].IsEmpty())
		{
			bIsEqualSearch = FALSE;
			
			if(sKeyWordGetData[4].GetAt(0) == '\"')
				bIsEqualSearch = TRUE;
			
			MakeSearchData.GetKeyWord(sKeyWordGetData[4]);
			if(!bIsEqualSearch) 
				sKeyWordGetData[4] += _T("*");

			sKeyWordGetData[3].MakeUpper();
			if(sKeyWordGetData[3].Compare(_T("OR")) == 0)
				sKeyWordWhere += _T(" | ");
			else
				sKeyWordWhere += _T(" ");
			sKeyWordWhere += sKeyWordGetData[4];				
		}

		if(!sKeyWordWhere.IsEmpty())
		{

			if(1 == m_pParentMgr->m_pInfo->GetSearchIndexType())
			{
				if(fd != NULL)
				{
					sWhere += _T(" &ECOSEARCH(IB.REC_KEY, BO_IDX_KEYWORd, '") + sKeyWordWhere + _T("') ");
				}
				else
				{
					if(!sEcoSearchWhere.IsEmpty())
						sEcoSearchWhere += _T(" INTERSECT ");
					
					sEcoSearchWhere += _T(" &ECOSEARCH(, BO_IDX_KEYWORd, '") + sKeyWordWhere + _T("') ");
				}
				nEcoSearchCnt++;
			}
			else
			{
				if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
				sWhere += CString(_T(" CATSEARCH(IB.IDX_KEYWORD,'")) + sKeyWordWhere + CString(_T("',NULL)>0 "));
			}
			nSearchField++;
		}

		if(fd != NULL)
		sWhere += _T(")");
	}
	
	
	const INT nPublishYearFieldCnt = 2;
	TCHAR *sPublishYearAlias[nPublishYearFieldCnt] =
	{
		_T("발행년도1"), _T("발행년도2")
	};
	CString sPublishYearGetData[nPublishYearFieldCnt];

	for(i = 0 ; i < nPublishYearFieldCnt ; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sPublishYearAlias[i], sPublishYearGetData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		sPublishYearGetData[i].TrimRight();
	}

	if(!sPublishYearGetData[0].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		if(sPublishYearGetData[1].IsEmpty())
		{
			sWhere += CString(_T(" IB.IDX_IPUB_YEAR = '")) + sPublishYearGetData[0] + CString(_T("' "));
		}
		else
			sWhere += CString(_T(" (IB.IDX_IPUB_YEAR >= '")) + sPublishYearGetData[0] + CString(_T("' AND "))
						+ CString(_T(" IB.IDX_IPUB_YEAR <= '")) + sPublishYearGetData[1] + CString(_T("')"));
	}

	
	const INT nClassNoCnt = 2;
	TCHAR* sClassNoAlias[nClassNoCnt] =
	{
		_T("분류기호1"), _T("분류기호2")
	};
	CString sClassNoData[nClassNoCnt];

	for(i = 0 ; i < nClassNoCnt ; i++)
	{
		ids = m_pParentMgr->GetControlData(sCMAlias, sClassNoAlias[i], sClassNoData[i]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		sClassNoData[i].TrimRight();
	}

	if(!sClassNoData[0].IsEmpty())
	{
		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");

		if(sClassNoData[1].IsEmpty())
		{
			sWhere += _T(" BB.CLASS_NO = '") + sClassNoData[0] + _T("' ");
		}
		else
		{
			sWhere += _T(" BB.CLASS_NO BETWEEN '") + sClassNoData[0] + _T("' AND '") + sClassNoData[1] + _T("' ") ;
		}
	}
	
	
	CString sRegNo;
	ids = m_pParentMgr->GetControlData(sCMAlias, _T("등록번호"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	if(!sRegNo.IsEmpty())
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);

		if(sWhere.GetLength() > 1) sWhere += _T(" AND ");
		sWhere += _T(" BB.REG_NO = '") + sRegNo +_T("'");

		nSearchField++;
	}

	if(fd != NULL) fclose(fd);

	if(!sEcoSearchWhere.IsEmpty())
	{
		if(!sWhere.IsEmpty())
			sWhere += _T(" AND ");
		if(nEcoSearchCnt == 1)
			sWhere += _T(" IB.REC_KEY IN ") + sEcoSearchWhere;
		else
			sWhere += _T(" IB.REC_KEY IN (") + sEcoSearchWhere + _T(") ");
	}

	
	CString sUseLimit;
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	CESL_Control_Element *pCE = pCM->FindControlElement(_T("사서제한"));
	if(pCE == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	
	CButton* pUseLimit = (CButton*)(m_pParentMgr->GetDlgItem(pCE->CTRL_ID));
	if(pUseLimit == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	if(pUseLimit->GetCheck() == 1)
	{
		if(!sWhere.IsEmpty())
			sWhere += _T(" AND ");

		sWhere += _T(" BB.USE_LIMIT_CODE != 'CL' ");
	}

	if(sWhere.IsEmpty())
	{
		CString sMsg = _T("조건을 입력하시고 검색하십시오.");
		SelfCloseMessageBox(sMsg);
		return 1;
	}

	if(nSearchField == 0 && (nMode == 0))
	{
		CString sMsg = _T("검색 조건이 부족하여 시간이 오래 걸리거나 제대로 기능을 수행하지 못할수 있습니다. \r\n검색하시겠습니까?");
		ids = MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("경고"), MB_ICONWARNING | MB_YESNO);
		if(ids == IDNO)
			return 1;
	}

	pCE = pCM->FindControlElement(_T("이용자용검색"));
	if(pCE == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	CButton* pUserSearch = (CButton*)(m_pParentMgr->GetDlgItem(pCE->CTRL_ID));
	if(pUserSearch == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	if(pUserSearch->GetCheck() == 1)
	{				
		if(!sWhere.IsEmpty())
			sWhere += _T(" AND ");

		if(0 == nMode)
		{
			sWhere += _T(" BB.WORKING_STATUS IN ('BOL112N','BOL113O','BOL114O','BOL115O','BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL217O','BOL218O','BOL219O','BOL411O','BOL511O','BOL611O') ");
			sWhere += _T(" AND BB.REG_NO IS NOT NULL ") ;
		}
		else if(1 == nMode)
		{
			
			sWhere += _T(" BB.WORKING_STATUS IN ('SEL112N','SEL212N','SEL213O','SEL214O','SEL215O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL317O','SEL318O','SEL411O','SEL511O') ");
		}
	}	
	else
	{
		if(0 == nMode)
		{
			if(!sWhere.IsEmpty())
				sWhere += _T(" AND ");

			sWhere += _T(" BB.WORKING_STATUS NOT IN ('BOT211O','BOT212O') ");				
		}
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SendSearchBookInfoSQL(CString &sBoWhere, CString &sSeWhere)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SendSearchBookInfoSQL");

	CString sPublishForm;
	CString sCMAlias = _T("CM_BO_LOC_3110");
	ids = m_pParentMgr->GetControlData(sCMAlias, _T("자료구분"), sPublishForm);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sDMFromDBAlias[2] = 
	{
		_T("DM_BO_LOC_3110_BO"), _T("DM_BO_LOC_3110_SE")
	};

	INT nStartDMIndex = 0;
	INT nFinishDMIndex = 2;
	if(sPublishForm.Compare(_T("단행")) == 0)
	{
		nStartDMIndex = 0;
		nFinishDMIndex = 1;
	}
	else if(sPublishForm.Compare(_T("연속")) == 0)
	{
		nStartDMIndex = 1;
		nFinishDMIndex = 2;
		
	}
	else if(sPublishForm.Compare(_T("전체")) == 0)
	{
		nStartDMIndex = 0;
		nFinishDMIndex = 2;
	}
	else
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sBookDMAlias = _T("DM_BO_LOC_3110");

	CESL_DataMgr *pBODM = m_pParentMgr->FindDM(sDMFromDBAlias[0]);
	CESL_DataMgr *pSEDM = m_pParentMgr->FindDM(sDMFromDBAlias[0]);
	pBODM->FreeData();
	pSEDM->FreeData();
	
	CESL_DataMgr *pDM;

	CESL_DataMgr *pBookDM = m_pParentMgr->FindDM(sBookDMAlias);
	if(pBookDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	pBookDM->FreeData();

	INT nTotalCount = 0;
	CString sWhere2;
	for(INT k = nStartDMIndex ; k < nFinishDMIndex ; k++)
	{
		if(k == 0) 
			sWhere2 = sBoWhere;
		else
			sWhere2 = sSeWhere;
		
		pDM = m_pParentMgr->FindDM(sDMFromDBAlias[k]);
		if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		DWORD nSTART, nFINISH;
		nSTART = GetTickCount();


		ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pDM, sWhere2);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		nFINISH = GetTickCount();
		CString strMsg;
		strMsg.Format(_T("(Finish) - 이용자 자료 찾기 검색시 걸린 시간 :  [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
		TRACE(strMsg.GetBuffer(0));
		pDM->QueryLog(strMsg.GetBuffer(0));
		
		nTotalCount += pDM->GetRowCount();

		nSTART = GetTickCount();	
		
		ids = MakeShelfCode(sDMFromDBAlias[k]);

 		if(0 > ids)
		{
			return ids;
		}
		
		CString sWorkingStatus;
		CString sShelfLocCode, sBoxName;
		const INT nCheckIsLoanCnt = 7;
		CString sCheckIsLoan[nCheckIsLoanCnt][2] =
		{
			{ _T("자료상태"), _T("") },
			{ _T("관리구분"), _T("") },
			{ _T("예약건수"), _T("") },
			{ _T("이용제한구분"), _T("") },
			{ _T("IBS_청구_별치기호_코드"), _T("") },
			{ _T("등록구분"), _T("") },
			{ _T("자료실명"), _T("") } 
		};
		CString sIsLoan;

		CString strTmpData;
		CMarcInfoConversionMgr pMarcConvertMgr;
		pMarcConvertMgr.SetMarcMgr( m_pParentMgr->m_pInfo->pMarcMgr);

		for(INT i = 0 ; i < pDM->GetRowCount() ; i++)
		{
		
			sIsLoan = _T("O");
			for(INT j = 0 ; j < nCheckIsLoanCnt ; j++)
			{
				ids = pDM->GetCellData(sCheckIsLoan[j][0], i, sCheckIsLoan[j][1]);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}

			
			if(sCheckIsLoan[0][1].Compare(_T("BOL112N"))==0 || 
				sCheckIsLoan[0][1].Compare(_T("SEL112N"))==0 ||
				sCheckIsLoan[0][1].Compare(_T("SEL212N"))==0
				)
			{
				
				CString sRegNo;
				pDM->GetCellData(_T("등록번호"), i, sRegNo);
				if(sRegNo.IsEmpty())
				{
					sIsLoan = _T("X");
				}
			}
			else
			{
				sIsLoan = _T("X");
			}

			
			ids = m_pCheckLoan->IsUseManageCode();
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			
			if(ids == 1) 
			{
				ids = m_pCheckLoan->IsLoanManageCode(sCheckIsLoan[1][1]);
				if(0 > ids) return ids;
				if(ids == 0) 
				{
					sIsLoan = _T("X");
				}
			}
			
			
			if(_ttoi(sCheckIsLoan[2][1]) > 0)
				sIsLoan = _T("X");

			
			ids = m_pCheckLoan->IsLoan(_T("이용제한구분"), sCheckIsLoan[3][1]);
			if(ids > 0)			
				sIsLoan = _T("X");

			
			ids = m_pCheckLoan->IsLoan(_T("별치기호"), sCheckIsLoan[4][1]);
			if(ids > 0)			
				sIsLoan = _T("X");
			
			
			ids = m_pCheckLoan->IsLoan(_T("등록구분"), sCheckIsLoan[5][1]);
			if(ids > 0)			
				sIsLoan = _T("X");
			
			
			ids = m_pCheckLoan->IsLoan(_T("자료실구분"), sCheckIsLoan[6][1]);
			if(ids > 0)			
				sIsLoan = _T("X");

			pDM->SetCellData(_T("대출가능여부"), sIsLoan, i);
			
			
			pDM->GetCellData(_T("자료상태"), i, sWorkingStatus);
			GetWorkingStatusDescToCode(sWorkingStatus, 1);
			pDM->SetCellData(_T("자료상태"), sWorkingStatus, i);

			pDM->GetCellData(_T("청구기호"), i, sShelfLocCode);
			pDM->GetCellData(_T("함번호"), i, sBoxName);
			if(sWorkingStatus.Compare(_T("실시간배가자료"))==0 && !sBoxName.IsEmpty())
			{
				sBoxName = sShelfLocCode + _T("(") + sBoxName + _T(")");
				pDM->SetCellData(_T("청구기호"), sBoxName, i);
			}

			
			
			
			CString sTFormInfo;
			CString sFormInfo[ 4 ];
			sFormInfo[0] = pDM->GetCellData(_T("PAGE"), i);
			sFormInfo[1] = pDM->GetCellData(_T("PHYSICAL_PROPERTY"), i);
			sFormInfo[2] = pDM->GetCellData(_T("BOOK_SIZE"), i);
			sFormInfo[3] = pDM->GetCellData(_T("ACCOMP_MAT"), i);
			CString output;
			if(!sFormInfo[0].IsEmpty())
				sTFormInfo += sFormInfo[0];
			if(!sFormInfo[1].IsEmpty())
				sTFormInfo = sTFormInfo + _T(":") + sFormInfo[1];
			if(!sFormInfo[2].IsEmpty())
				sTFormInfo = sTFormInfo + _T(";") + sFormInfo[2];
			if(!sFormInfo[3].IsEmpty())
				sTFormInfo = output + _T("+") + sFormInfo[3];
			pDM->SetCellData(_T("형태사항"), sTFormInfo, i);

			
			CMarc marc;		
			strTmpData = pDM->GetCellData(_T("MARC"), i);
			if ( 0 <= m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strTmpData, &marc) )
			{
				strTmpData.Empty();	
				ids = pMarcConvertMgr.GetSeriesStatement(&marc, strTmpData);
				if( ids >= 0 )	pDM->SetCellData(_T("총서사항"), strTmpData, i);
			}		
		}

		
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("자료실구분"), _T("자료실명"));

		
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("간행빈도"), _T("간행빈도"));

		nFINISH = GetTickCount();
		TRACE(_T("(Finish) - 이용자 자료 찾기 자료상태,자료실,간행빈도,함번호,청구기호 변경에 걸린 시간 : [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);	
	}

	if(nTotalCount < 1)
	{
		SelfCloseMessageBox(_T("자료가 존재하지 않습니다. \r\n다시 검색해주십시오"));
		return 3;
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::ViewSerachBookList(INT nEntryPoint, CString &sRegNo, CString &sPublishForm)
{
EFS_BEGIN

	INT ids;

	CSearchBookListDlg BookListDlg(m_pParentMgr, nEntryPoint);

	CString sDMAlias = _T("DM_BO_LOC_3110");
	ids = MakeShelfCode(sDMAlias);
	if(0 > ids)
	{
		return ids;
	}


	BookListDlg.SetParent(this);
	BookListDlg.DoModal();

	BookListDlg.GetSelectRegNo(sRegNo);
	BookListDlg.GetSelectPublishForm(sPublishForm);
	
	if(sRegNo.IsEmpty())
	{
		return 3;
	}
	else
	{
		
		if(BookListDlg.m_nCloseMode == 0)
			return 11;
		
		else if(BookListDlg.m_nCloseMode == 2)
			return 21;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsSearchedUser()
{
EFS_BEGIN

	
	
	
	CString strFuncName = _T("IsSearchedUser");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(!pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	if(1 > pDM->GetRowCount())
	{
		SelfCloseMessageBox(_T("대출자를 먼저 선택해주십시오"));
		return 1;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetWorkingStatusDescToCode(CString &sCodeValue, INT nMode)
{
EFS_BEGIN

	m_pLocCommonAPI->GetWorkingStatusDescToCode(sCodeValue, nMode);
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewFamilyMember()
{
EFS_BEGIN

	INT ids;

	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(0 < ids) return 0;

	ids = SearchFamilyMember();
	if(0 > ids) return ids;

	
	if(0 == ids) return 0;	

	CFAMILY_ID dlg(m_pParentMgr);
	dlg.m_strUserAccessAuth = m_pManageValue->m_sLoanUserAccessMode;
	CPoint point;
	GetCursorPos(&point);
	dlg.m_point = point;
	dlg.DoModal();

	CString sRecKey;
	dlg.GetMemberRecKey(sRecKey);

	if(sRecKey.IsEmpty())
		return 0;

	
	ids = SearchLoanInfo(sRecKey, _T(""));
	if(0 > ids) return ids;

	return 0;

EFS_END	
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchFamilyMember()
{
EFS_BEGIN

	INT ids, i, nRowCnt;
	CString strFuncName = _T("SearchFamilyMember");
	
	CString sParentDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sDMAlias = _T("DM_FAMILY_ID_INFO");

	CString sFamilyKey;
	ids = m_pParentMgr->GetDataMgrData(sParentDMAlias, _T("가족KEY"), sFamilyKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	pDM->FreeData();

	if(sFamilyKey.IsEmpty()) return 0;

	CString sWhere;
	sWhere.Format(_T("CLU.FID_KEY = %s"), sFamilyKey);

	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pDM, sWhere);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	nRowCnt = pDM->GetRowCount();
	CString strValue, strUserNo;
	ids = m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	strValue.TrimLeft();	strValue.TrimRight();
	strValue.MakeUpper();

	
	CString strEnc;
	CStringArray asDes1,asDes2;

	for(i=0 ; i<nRowCnt ; i++)
	{
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse)
		{
			pDM->GetCellData(_T("집전화"), i, strEnc);
			asDes1.Add(strEnc);
			pDM->GetCellData(_T("핸드폰"), i, strEnc);
			asDes2.Add(strEnc);
		}

		ids = pDM->GetCellData(_T("대출자번호"), i, strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("Y") != strValue)
		{	
			CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
		}
		ids = pDM->SetCellData(_T("대출자번호표시"), strUserNo, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bLibSecuUse && 0 < nRowCnt)
	{
		CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(m_pParentMgr);
		if(NULL != pApi)
		{
			if(TRUE == pApi->CheckConnection())
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes1);
					pApi->CloseSocket();	
				}
				if(TRUE == pApi->ConnectSocket()) 
				{
					pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
					pApi->CloseSocket();	
				}
				for(i = 0; i < nRowCnt; i++)
				{
					pDM->SetCellData(_T("집전화"), asDes1.GetAt(i), i);
					pDM->SetCellData(_T("핸드폰"), asDes2.GetAt(i), i);
				}
			}
			delete pApi;
			pApi = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
		asDes2.RemoveAll(); asDes2.FreeExtra();
	}

	return pDM->GetRowCount();


EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::IsSearchedLoanInfo()
{
EFS_BEGIN

	CString strFuncName = _T("IsSearchedLoanInfo");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nRowCount = pDM->GetRowCount();

	if(nRowCount < 0) return 2;
	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::CopyDMToDM(CESL_DataMgr *pDM, CString sDMAlias)
{
EFS_BEGIN

	INT ids;
	
	INT nRowCount= pDM->GetRowCount();

	CString sGetData;
	CString *sColAlias;
	INT nColCnt = pDM->GetColCount();
	sColAlias = new CString[nColCnt];
	ids = pDM->GetAllAlias(sColAlias, nColCnt);
	if(0 > ids) 
	{
		delete []sColAlias;
		return -1;
	}

	CESL_DataMgr *pDstDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDstDM == NULL) 
	{
		delete []sColAlias;
		return -1;
	}

	for(INT i = 0 ; i < nRowCount ; i++)
	{
		pDstDM->InsertRow(-1);
		for(INT j = 0 ; j < nColCnt ; j++)
		{
			ids = pDM->GetCellData(i, j, sGetData);
			if(0 > ids&& (ids != -4012)) 
			{
				delete []sColAlias;
				return -1;
			}
			if(ids == -4012) sGetData.Empty();
			
			ids = pDstDM->SetCellData(sColAlias[j], sGetData, pDstDM->GetRowCount()-1);
			if(0 > ids) 
			{
				delete []sColAlias;
				return -1;
			}
		}
	}

	delete []sColAlias;

	return 0;

EFS_END	
return -1;
}


INT CBL_LOC_LOAN_PROC::ReLoadLoanInfo(BOOL bAskPasswd /* =TRUE */)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReLoadLoanInfo");

	CString sRecKey;
	
	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("대출자KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, strFuncName);

	if(sRecKey.IsEmpty())
		return 0;

	
	ids = SearchLoanInfo(sRecKey, _T(""), bAskPasswd);
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}



INT CBL_LOC_LOAN_PROC::EditLoanUserInfo()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("EditLoanUserInfo");

	
	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(ids > 0) return 1;
	
	CString sRecKey;
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	INT nMode;
	m_pCheckLoan->GetLoanUserAccessMode(nMode);

	
	if(0 == nMode || 2 == nMode) 
	{
		nMode = 3;
	}
	else if(nMode == 1)
	{
		nMode = 6;
	}
	
	
	if(m_EditLoanUser == NULL)
	{
		m_EditLoanUser = new CBO_LOC_3320(m_pParentMgr,m_pLoanShareManager, m_pManageValue);
		
	}
	
	m_EditLoanUser->m_nAction = ACTION_READ_F_LOANINFO;
	m_EditLoanUser->SetMode(nMode,TRUE);
	m_EditLoanUser->SetRecKey(sRecKey);
	m_EditLoanUser->DoModal();
	BOOL bIsSelfLibSecede = m_EditLoanUser->KL_IsSelfLibSecede();
	delete m_EditLoanUser;
	m_EditLoanUser = NULL;
	
	
	if(bIsSelfLibSecede)
	{
		::PostMessage(m_pParentMgr->GetSafeHwnd(),WM_KEYDOWN,VK_ESCAPE,1);
		SelfCloseMessageBox(_T("탈퇴 처리가 완료 되었습니다."));
	}
	else
	{
		
		
		if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
		{
			SPFExcutePath(_T("회원리로드_NOASK"));
		}
		else
		{
			SPFExcutePath(_T("회원정보만리로드"));
		}
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::SaveLoanUserEtcInfo()
{
EFS_BEGIN

	INT ids;

	
	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(ids > 0) return 1;
	
	
	ids = SaveLoanUserEtcInfoDBProc();
	if(0 > ids) return ids;

	ids = UpdateEtcDM();
	if(0 > ids) return ids;

	SelfCloseMessageBox(_T("수정되었습니다."));

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::SaveLoanUserEtcInfoDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SaveLoanUserEtcInfoDBProc");

	CString sDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sTableName = _T("CO_LOAN_USER_TBL");

	CString sRecKey;
	CString sEtcInfo;
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("대출자KEY"), sRecKey, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);



	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_MEMBER_INFO"), _T("비고"), sEtcInfo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL)  ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

	pDM->StartFrame();
	pDM->InitDBFieldData();
	ids= pDM->AddDBFieldData(_T("REMARK"), _T("STRING"), sEtcInfo, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	ids= pDM->AddDBFieldData(_T("REMARK_EDIT_DATE"), _T("NUMERIC"), _T("SYSDATE"), TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("이용자비고수정"), __WFILE__, __LINE__), TRUE);

	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	pDM->EndFrame();

	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sRecKey));
	SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("REMARK,REMARK_EDIT_DATE")));
	
	ShareInfoCenterUpload(_T("N"), _T("U"));
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateEtcDM()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("UpdateEtcDM");

	CString sEtcInfo;
	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_MEMBER_INFO"), _T("비고"), sEtcInfo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("비고"), sEtcInfo, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsUserBookLoan(CString sMode, BOOL bIsILL, CString strBookManageCode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("IsUserBookLoan");

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sBookDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	CString sMediaCode;
	ids = m_pParentMgr->GetDataMgrData(sBookDMAlias, _T("매체구분"), sMediaCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sMediaCode.Compare(_T("OF")) == 0)
	{
		SelfCloseMessageBox(_T("매체구분이 'OF'인 자료는 대출이 불가능합니다."), m_pParentMgr);
		return 5;
	}
	
	CString sIsAppendix;
	
	ids = m_pParentMgr->GetDataMgrData(sBookDMAlias, _T("부록여부"), sIsAppendix, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sIsAppendix.Compare(_T("A")) == 0)
	{
		if(m_pCheckLoan->IsAppendixLoan() !=0)
		{
			SelfCloseMessageBox(_T("부록자료는 대출이 불가능합니다."), m_pParentMgr);
			return 1;
		}
	}
	
	CString strUserClassCode;
	CString strLoanPeriod;

	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("대출자직급"), strUserClassCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = m_pManageValue->GetUserPosition(strUserClassCode, _T("대출기한"), strLoanPeriod);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sRegCode, sValue;
	ids = m_pParentMgr->GetDataMgrData(sBookDMAlias, _T("등록구분"), sRegCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = m_pManageValue->GetRegClass(strUserClassCode, sRegCode, _T("대출기한"), sValue);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(!sValue.IsEmpty()) strLoanPeriod = sValue;
	
	if(0 == _ttoi(strLoanPeriod))
	{
		if(EXP_LOAN == sMode)
		{
			SelfCloseMessageBox(_T("대출기한이 '0'일인 자료는 대출할 수 없습니다. \r\n대출하기 위해서는 관리에서 설정해주십시오"), m_pParentMgr);
			return 1;
		}
		else
		{
			SelfCloseMessageBox(_T("대출기한이 '0'일인 자료는 예약할 수 없습니다. \r\n예약하기 위해서는 관리에서 설정해주십시오"), m_pParentMgr);
			return 1;
		}
	}

	if(sMode == EXP_LOAN)
	{
		
		ids = CheckLoanBookCnt(sIsAppendix, bIsILL, strBookManageCode);
		if(ids > 0) return 2;
	}

	if(FALSE == bIsILL)
	{
		ids = m_pCheckLoan->IsUseManageCode();
		if(0 > ids) return ids;
		
		if(ids == 1) 
		{
			ids = m_pCheckLoan->IsLoanManageCode();
			if(0 > ids) return ids;
			if(ids == 0) 
			{
				SelfCloseMessageBox(_T("관리구분값이 다르므로 대출을 할 수 없습니다. "));
				return 1;
			}
		}
	}
	
	const INT nCnt = 4;
	TCHAR *sFieldAlias[nCnt] = {
		_T("등록구분"), _T("별치기호"), _T("이용제한구분"), _T("자료실구분")
	};
	CString sCode[nCnt];
	for(INT i = 0 ; i < nCnt ; i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sBookDMAlias, sFieldAlias[i], sCode[i], 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("IsUserBookLoan"));
	}
	for(i = 0 ; i < nCnt ; i++)
	{
		ids = m_pCheckLoan->IsLoan(sFieldAlias[i], sCode[i]);
		if(0 > ids) return ids;
		if(ids > 0) 
		{
			CString sCodeDesc;
			ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(sFieldAlias[i], sCode[i], sCodeDesc);
			if(0 > ids) return ids;

			CString sMsg;
			sMsg.Format(_T("대출하려는 책의 %s가 대출할 수 없는 %s(%s)입니다.")
							, sFieldAlias[i], sFieldAlias[i], sCodeDesc);
			SelfCloseMessageBox(sMsg);
			return 2;
		}
	}

	CString sUserClassCode;
	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("대출자직급"), sUserClassCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("IsUserBookLoan"));
	
	CString sUseObjectCode;
	ids = m_pCheckLoan->IsUseObject(sUseObjectCode);
	if(0 > ids) return ids;
	if(ids > 0) 
	{
		CString sMsg;
		sMsg.Format(_T("이용대상이 '%s'인 자료는 대출할 수 없습니다. \r\n대출하기 위해서는 관리에서 설정해주십시오"), sUseObjectCode);
		SelfCloseMessageBox(sMsg);
		return 4;
	}	

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::RecordReserveExpireDate()
{
EFS_BEGIN

	const INT CHECK_CNT = 6;
	INT ids;

	
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	TCHAR *sFieldAlias[CHECK_CNT] = 
	{
		_T("단행/연속구분"), _T("책KEY"), _T("예약이용자수"), _T("종KEY"), _T("권호정보KEY"), _T("등록번호")
	};
	CString sGetData[CHECK_CNT];

	INT nRowIndex;
	ids = GetCurrentGridPos(nRowIndex);
	if(0 > ids) return ids;

	for(INT i = 0 ; i < CHECK_CNT ; i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, sFieldAlias[i], sGetData[i], nRowIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("RecordReserveExpireDate"));
	}
	
	if(m_pManageValue->m_bVolReserve && !sGetData[4].IsEmpty())
	{
		
		CString sVolReserveTable;
		
		if(sGetData[0] == _T("SE"))
			sVolReserveTable = _T("SE_BOOK_TBL");
		else if(sGetData[0] == _T("NB"))
			sVolReserveTable = _T("CO_NON_DB_BOOK_TBL");
		else
			sVolReserveTable = _T("BO_BOOK_TBL");			

		CString sQuery;
		sQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 CL ") 
			           _T(" WHERE CL.BOOK_KEY IN (SELECT REC_KEY FROM %s WHERE SPECIES_KEY = %s) ")
					   _T("   AND CL.REG_NO IS NULL ")
					   _T("   AND CL.STATUS = '3' ")
					   _T("   AND CL.VOL_INFO = %s "),
					   sVolReserveTable, sGetData[3], sGetData[4]);

		
		CESL_DataMgr* pTmpDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
		if(!pTmpDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -11, _T("RecordReserveExpireDate"));

		ids = pTmpDM->GetOneValueQuery(sQuery, sGetData[2]);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -12, _T("RecordReserveExpireDate"));		
	}
	

	
	if(_ttoi(sGetData[2]) == 0)  return 0;

	if(!(m_pManageValue->m_bVolReserve))
	{
		CString sSQL;
		sSQL.Format(_T("SELECT REC_KEY FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3' AND RESERVATION_DATE = ")
			_T("(SELECT MIN(RESERVATION_DATE) FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS = '3')"), sGetData[1], sGetData[1]);

		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
		if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("RecordReserveExpireDate"));

		CString sReturnDate;
		CString sReturnType;
		ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("RecordReserveExpireDate"));

		CString sReturnDateControlAlias = _T("반납일");
		if(sReturnType.Compare(_T("2"))== 0)
			sReturnDateControlAlias = _T("소급반납일");

		ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("RecordReserveExpireDate"));

		CString sLoanKey;
		ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, sSQL, sLoanKey);
		if(0 > ids&& ids != -4007) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("RecordReserveExpireDate"));

		
		if(sLoanKey.IsEmpty())
			return 0;
	}

	if(_T("Y") == m_pManageValue->m_sUnmannedReserveSys)
	{
		
		CString strQuery;
		if(!m_pManageValue->m_bVolReserve)
		{
 			strQuery.Format(	
 				_T("SELECT COUNT(1) FROM LS_WORK_T01 LS1")
 				_T(" WHERE LS1.BOOK_KEY=%s AND LS1.STATUS='3'")
 				_T(" AND LS1.UNMANNED_RESERVATION_LOAN IS NOT NULL")
 				_T(" AND LS1.RESERVATION_DATE=")
 						_T("(SELECT MIN(LS2.RESERVATION_DATE) FROM LS_WORK_T01 LS2")
 						_T(" WHERE LS1.BOOK_KEY=LS2.BOOK_KEY AND LS2.STATUS='3'")
 						_T(" GROUP BY LS2.BOOK_KEY)"), sGetData[1]);
 			CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
 			if(NULL == pDM){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("RecordReserveExpireDate"));}
 			CString strResult;
 			ids = CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pDM, strQuery, strResult);
 			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("RecordReserveExpireDate"));}
 			if(0 < _ttoi(strResult))
 			{
 				CString strMsg;
 				strMsg.Format(_T("[%s]는 무인예약 자료입니다."), sGetData[5]);
 				MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("예약 처리"), MB_OK | MB_ICONINFORMATION);
 				return 0;
 			}
		}
	}

	ids = GetReservationAutoMode();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("RecordReserveExpireDate"));

	CString strReserveMsg;
	strReserveMsg.Format(_T("[%s]는 예약된 자료입니다. 예약 처리를 하시겠습니까?"), sGetData[5]);
	INT nYes = MessageBox(m_pParentMgr->m_hWnd, strReserveMsg, _T("예약 처리"), MB_YESNO | MB_ICONQUESTION);
	if(nYes == IDYES)
	{		
		if(!(m_pManageValue->m_bVolReserve))
		{ 
			CBO_LOC_3170 *Dlg = new CBO_LOC_3170(m_pParentMgr, m_pLoanShareManager, m_pManageValue, 0);
			Dlg->pMain = m_pParentMgr->pMain;
			Dlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
			Dlg->m_pInfo = m_pParentMgr->m_pInfo;
			Dlg->SetInfo(sGetData[3], sGetData[4], sGetData[0], sGetData[1]);
			
			POSITION pos;
			pos = m_obj3170DlgList->AddTail((CObject*)Dlg);
			
			if(Dlg->Create(m_pParentMgr))
 			{
 				Dlg->CenterWindow();
 				Dlg->ShowWindow(SW_SHOWNOACTIVATE);
 				Dlg->UpdateWindow();				
 			}
 			else
			{
				m_obj3170DlgList->RemoveAt(pos);
			}
		}
		else 
		{ 
			CBO_LOC_3170_ReserveInfo *Dlg = new CBO_LOC_3170_ReserveInfo(m_pParentMgr, m_pLoanShareManager, m_pManageValue, 0);
			Dlg->pMain = m_pParentMgr->pMain;
			Dlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
			Dlg->m_pInfo = m_pParentMgr->m_pInfo;
			Dlg->SetInfo(sGetData[3], sGetData[4], sGetData[0], sGetData[1]);						
			Dlg->m_strReturnBookKey = sGetData[1];
			Dlg->m_strReturnBookRegNo = sGetData[5];			
			Dlg->m_nReserveInfoMode = 1;			
						
			POSITION pos;
			pos = m_obj3170DlgList->AddTail((CObject*)Dlg);
			if(Dlg->Create(m_pParentMgr))
 			{
 				Dlg->CenterWindow();
 				Dlg->ShowWindow(SW_SHOWNOACTIVATE);
 				Dlg->UpdateWindow();				
 			}
 			else
			{
				m_obj3170DlgList->RemoveAt(pos);
			}
		}
	}
	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::GetCurrentGridPos(INT &nRowIndex)
{
EFS_BEGIN

	CString strFuncName = _T("GetCurrentGridPos");

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	nRowIndex = pGrid->GetIdx();
	if(nRowIndex < 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}



INT CBL_LOC_LOAN_PROC::RecordReserveExpireDateDBProc(CESL_DataMgr *pDM, CString &sLoanKey, CString &sReturnDate)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("RecordReserveExpireDateDBProc");

	
	CString sTableName = _T("LS_WORK_T01");
	CBL_LOC_RESERVATOIN_PROC  ReservationInfo(m_pParentMgr, m_pLoanShareManager,  m_pManageValue);

	CString sPositionCode;
	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pUserDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	ids = pUserDM->GetCellData(_T("대출자직급"), 0, sPositionCode);

	INT nExpireDay = 0;
	ids = ReservationInfo.GetReservationExpireDay(nExpireDay, sPositionCode);
	if(0 > ids) return ids;
	CTimeSpan tExpireDay(nExpireDay, 0, 0, 0);

	CTime tDate;
	if(!sReturnDate.IsEmpty())
	{
		CTime tReturnDate(_ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0, 0, 0);
		tDate = tReturnDate;
	}else
	{
		
		CString strCurrentTime;
		tDate = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strCurrentTime);

	}
	
	tDate = tDate + tExpireDay;
	CString sReservationExpireDate;
	sReservationExpireDate = tDate.Format(_T("%Y/%m/%d"));
	sReservationExpireDate = _T("TO_DATE('")+sReservationExpireDate+_T(" 23:59:59")+_T("', 'YYYY/MM/DD HH24:MI:SS')");

	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

	pDM->StartFrame();
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("RESERVATION_EXPIRE_DATE"), _T("NUMERIC"), sReservationExpireDate, TRUE);
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약만기일부여"), __WFILE__, __LINE__), TRUE);
	
	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sLoanKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 
	ids = pMobileApi->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 

	pDM->EndFrame();
	
	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sLoanKey));
	SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_EXPIRE_DATE")));
	
	ShareInfoCenterUpload(_T("N"), _T("U"));
		
	if((m_pParentMgr->m_pInfo->MODE==10000) || (m_pParentMgr->m_pInfo->MODE==30000))
		return 0;

	CString sWhere;
	sWhere.Format(_T(" CL.LOAN_KEY = %s "), sLoanKey);
	
	
	if((m_pParentMgr->m_pInfo->MODE==10000) || (m_pParentMgr->m_pInfo->MODE==30000))
		return 0;

	//JOB.2019.0073 : 충돌 License
// 	CLocMailMgrAPI MailMgr(m_pParentMgr);
// 	MailMgr.MakeRecord(0, sWhere, FALSE);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetReservationAutoMode()
{
EFS_BEGIN

	INT ids;

	CBL_LOC_RESERVATOIN_PROC  ReservationInfo(m_pParentMgr, m_pLoanShareManager, m_pManageValue);
	INT nExpireDay = 0;
	ids = ReservationInfo.GetReservationAutoMode();

	if(ids == 0) return 2;
	else return 1;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanProc()
{
EFS_BEGIN

	INT	ids;
	CString strFuncName = _T("AppendixLoanProc");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_ControlMgr	*pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
	pGrid->SelectMakeList();
	INT	nRowCount = pGrid->SelectGetCount();
	
	pGrid->SelectGetTailPosition();
	
	if(0 >= nRowCount)
	{
		SelfCloseMessageBox(_T("부록대출하려는 책을 선정해 주십시오."));
		return 1;
	}
	
	if(1 < nRowCount)
	{
		SelfCloseMessageBox(_T("한개의 자료만 선정하여 주십시오."));
		return 2;
	}

	
	INT nIndex = pGrid->SelectGetIdx();
	if(0 > nIndex) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	CString	sStatus;
	ids = pDM->GetCellData(_T("대출상태"), nIndex, sStatus);
	if(_T("0") != sStatus && _T("2") != sStatus)
	{
		
		if(_T("O") != sStatus && _T("L") != sStatus && _T("D") != sStatus)
		{
			SelfCloseMessageBox(_T("선정된 자료는 대출상태가 아닙니다. 대출중인 자료를 선정하여 주십시오"));
		}
		else
		{
			SelfCloseMessageBox(_T("책두레대출된 자료에 대해선 부록대출이 지원되지 않습니다."));
		}
		return 3;
	}
	
	CString sIsBook;
	pDM->GetCellData(_T("부록여부"), nIndex, sIsBook);
	if(_T("B") != sIsBook) 
	{
		SelfCloseMessageBox(_T("부록자료는 부록대출을 할 수 없습니다."));
		return 4;
	}

	ids = IsLoanAppendix();
	if(0 != ids) 
	{
		SelfCloseMessageBox(_T("부록대출을 사용하지 않는 도서관입니다."));
		return 5;
	}
	
	CESL_DataMgr *pAppendixDM;
	pAppendixDM = MakeAppendixDM(nIndex);
	if(NULL == pAppendixDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName); 
	}
	if(1 > pAppendixDM->GetRowCount()) 
	{
		SelfCloseMessageBox(_T("부록자료가 존재하지 않는 자료입니다."));
		return 6;
	}

	CLoanAppendixListDlg AppendixList(m_pParentMgr);
	AppendixList.m_pPreRegNoMgr = m_pPreRegNoMgr;
	AppendixList.DoModal();

	if(1 > pAppendixDM->GetRowCount())
	{
		return 0;
	}
	
	CString sRegNo;
	for(INT i = 0; i < pAppendixDM->GetRowCount(); i++)
	{
		pAppendixDM->GetCellData(_T("등록번호"), i, sRegNo);
		
		ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName); 

		MakeRegNoSpaceToZero(TRUE);
		InitExpDM();
		SPFExcutePath(_T("대출"));
	}

	return 0;
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoan(INT nIndex)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AppendixLoan");

	CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 
	
	INT nMode = 0;

	if(nIndex == -1)
	{
		nIndex = pLoanInfoDM->GetRowCount()-1;
		nMode = 1;
	}

	CString sIsBook;
	pLoanInfoDM->GetCellData(_T("부록여부"), nIndex, sIsBook);

	CString strPubFormCode = _T("");
	
	if ( !(sIsBook.Compare(_T("C")) == 0 || sIsBook.Compare(_T("B")) == 0  ) ) return 0;
	
	ids = IsLoanAppendix();
	if(0 > ids) return ids;
	
	if(ids != 0) return 0;
	
	if(nMode == 1)
	{
		CESL_DataMgr* pLoanExpInfoDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_BOOK_INFO"));
		if(pLoanExpInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 
		
		if(pLoanExpInfoDM->GetRowCount() == 0)
		{
			AfxMessageBox(_T("이미 지워졌음"));
			return 1;
		}

		CString sACnt;
		ids = pLoanExpInfoDM->GetCellData(_T("부록책수"), 0, sACnt);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 
		
		if(_ttoi(sACnt) == 0) return 0;
	}
	
	CESL_DataMgr *pDM;
	pDM = MakeAppendixDM(nIndex);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 
	if(pDM->GetRowCount() < 1) return 0;
	
	ids = AddMemberInfo();
	if(0 > ids) return ids;
	
	ids = ViewLoanScreen(0);
 	if(ids != 0) return ids;

	CString strIsLoan;
	int nRowCnt = 0;
	const int ROW_COUNT = pDM->GetRowCount();
	for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
	{
		ids = pDM->GetCellData(_T("대출가능여부"), nRowCnt, strIsLoan);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("O") == strIsLoan)
		{
			break;
		}
	}

	
	if(_T("O") != strIsLoan)
	{
		return 0;
	}

	ids = MessageBox(m_pParentMgr->m_hWnd, _T("대출가능한 부록자료가 존재합니다. 같이 대출을 하시겠습니까?"), _T("부록 자료 대출"), MB_ICONQUESTION | MB_YESNO);
	if(ids == IDNO) return 0;

	CLoanAppendixListDlg AppendixList(m_pParentMgr);
	AppendixList.m_pPreRegNoMgr = m_pPreRegNoMgr;
	AppendixList.DoModal();

	if(pDM->GetRowCount() < 1 ) return 0;

	
	CString sRegNo;
	for(INT i=0 ;i < pDM->GetRowCount() ; i++)
	{
		pDM->GetCellData(_T("등록번호"), i, sRegNo);
		
		ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName); 

		MakeRegNoSpaceToZero(TRUE);

		InitExpDM();

		ids = Loan(sRegNo);
		if(0 > ids) return ids;		
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsLoanAppendix()
{
EFS_BEGIN

	return m_pCheckLoan->IsAppendixLoan();

EFS_END
return -1;
}

CESL_DataMgr* CBL_LOC_LOAN_PROC::MakeAppendixDM(INT nIndex)
{
EFS_BEGIN

	INT ids;
	CString sBookLoanDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sAppedixLoanDMAlias = _T("DM_BO_LOC_3100_APPENDIX_INFO");
	CString sAppedixLoanSEDMAlias = _T("DM_BO_LOC_3100_APPENDIX_INFO_SE");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sAppedixLoanDMAlias);
	if(pDM == NULL) return NULL;

	CESL_DataMgr *pSEDM = m_pParentMgr->FindDM(sAppedixLoanSEDMAlias);
	if(pSEDM == NULL) return NULL;
	
	
	CESL_DataMgr *pLoanBookDM = m_pParentMgr->FindDM(sBookLoanDMAlias);
	if(pLoanBookDM == NULL) return NULL;

	CString sLoanBookSpeciesKey;
	
	ids = m_pParentMgr->GetDataMgrData(sBookLoanDMAlias, _T("종KEY"), sLoanBookSpeciesKey, nIndex);
	if(0 > ids) return NULL;

	
	CString sWhere;
	sWhere.Format(_T("B.SPECIES_KEY = %s"), sLoanBookSpeciesKey);
	pDM->FreeData();

	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pDM, sWhere);
	if(0 > ids) return NULL;

	CString sLoanBookVolKey;
	ids = m_pParentMgr->GetDataMgrData(sBookLoanDMAlias, _T("권호정보KEY"), sLoanBookVolKey, nIndex);
	
	sWhere.Format(_T("B.SPECIES_KEY = %s AND B.VOL_KEY = '%s' "), sLoanBookSpeciesKey, sLoanBookVolKey);

	pSEDM->FreeData();
	ids = CLocCommonAPI::RefreshDataManager(m_pParentMgr, pSEDM, sWhere);
	if(0 > ids) return NULL;

	if((pDM->GetRowCount()==0) && (pSEDM->GetRowCount()>0))
	{
		CLocCommonAPI::AddDM(pSEDM, pDM, m_pParentMgr);
	}	
		CString sShelfLocCode, sBoxName;
		const INT nCheckIsLoanCnt = 7;
		CString sCheckIsLoan[nCheckIsLoanCnt][2] =
		{
			{ _T("자료상태"), _T("") },
			{ _T("관리구분"), _T("") },
			{ _T("예약건수"), _T("") },
			{ _T("이용제한구분"), _T("") },
			{ _T("IBS_청구_별치기호_코드"), _T("") },
			{ _T("등록구분"), _T("") },
			{ _T("자료실"), _T("") } 
		};
		CString sIsLoan;
		
		CString sWorkingStatus;
		for(INT i = 0 ; i < pDM->GetRowCount(); i++)
		{		
			sIsLoan = _T("O");
			for(INT j = 0 ; j < nCheckIsLoanCnt ; j++)
			{
				ids = pDM->GetCellData(sCheckIsLoan[j][0], i, sCheckIsLoan[j][1]);				
			}

			
			if(sCheckIsLoan[0][1].Compare(_T("BOL112N"))==0 || 
				sCheckIsLoan[0][1].Compare(_T("SEL112N"))==0 ||
				sCheckIsLoan[0][1].Compare(_T("SEL212N"))==0
				)
			{		
				
				CString sRegNo;
				pDM->GetCellData(_T("등록번호"), i, sRegNo);
				if(sRegNo.IsEmpty())
				{
					sIsLoan = _T("X");
				}
			}
			else
			{
				sIsLoan = _T("X");
			}

			
			ids = m_pCheckLoan->IsUseManageCode();			
			
			if(ids == 1) 
			{
				ids = m_pCheckLoan->IsLoanManageCode(sCheckIsLoan[1][1]);				
				if(ids == 0) 
				{
					sIsLoan = _T("X");
				}
			}
			
			
			if(_ttoi(sCheckIsLoan[2][1]) > 0)
			{
				sIsLoan = _T("X");
			}

			
			ids = m_pCheckLoan->IsLoan(_T("이용제한구분"), sCheckIsLoan[3][1]);
			if(ids > 0)			
			{
				sIsLoan = _T("X");
			}

			
			ids = m_pCheckLoan->IsLoan(_T("별치기호"), sCheckIsLoan[4][1]);
			if(ids > 0)			
			{
				sIsLoan = _T("X");
			}
			
			
			ids = m_pCheckLoan->IsLoan(_T("등록구분"), sCheckIsLoan[5][1]);
			if(ids > 0)		
			{
				sIsLoan = _T("X");
			}
			
			
			ids = m_pCheckLoan->IsLoan(_T("자료실구분"), sCheckIsLoan[6][1]);
			if(ids > 0)			
			{
				sIsLoan = _T("X");
			}

			pDM->SetCellData(_T("대출가능여부"), sIsLoan, i);

			pDM->GetCellData(_T("자료상태"), i, sWorkingStatus);			
			CLocCommonAPI::GetWorkingStatusDescToCode(sWorkingStatus, 1);			
			pDM->SetCellData(_T("자료상태"), sWorkingStatus, i);
		}

	
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("자료실구분"), _T("자료실"));	
	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("매체구분"), _T("매체구분"));

	return pDM;

EFS_END
return NULL;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanDBProc(CESL_DataMgr *pDM)
{
EFS_BEGIN

	INT ids;

	for(INT i=0 ;i < pDM->GetRowCount() ; i++)
	{
		
		CKolas2up_MobileApi *pMobileApi;
		pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);

		pDM->StartFrame();
		
		ids = AppendixLoanBookDBProc(pDM, i, pMobileApi);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanDBProc")); 
	
		
		ids = AppendixLoanInfoDBProc(pDM,i, pMobileApi);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("AppendixLoanDBProc")); 
		
		
		ids = AppendixLoanUserDBProc(pDM,i, pMobileApi);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("AppendixLoanDBProc")); 

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("AppendixLoanDBProc")); 
	
		pDM->EndFrame();

		
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
	}
	return 0;

EFS_END	
return -1;
}

INT CBL_LOC_LOAN_PROC::UpdateAppendixLoanProc(CESL_DataMgr *pDM)
{
EFS_BEGIN

	INT ids;

	INT nAppendixCnt = pDM->GetRowCount();

	CString sAppendixCnt;
	sAppendixCnt.Format(_T("%d"), nAppendixCnt);
	
	
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(sDMAlias);
	if(pLoanDM == NULL)ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("UpdateAppendixLoanProc"));
	
	ids = m_pParentMgr->SetDataMgrData( sDMAlias, _T("부록자료수"), sAppendixCnt, pLoanDM->GetRowCount()-1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("UpdateAppendixLoanProc")); 

	
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("UpdateAppendixLoanProc"));

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("UpdateAppendixLoanProc"));

	INT nStatusColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("부록자료수"), nStatusColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("UpdateAppendixLoanProc")); 
	
	if(nStatusColIndex != -1)
		pGrid->SetAt(pLoanDM->GetRowCount()-1, nStatusColIndex, sAppendixCnt);
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanBookDBProc(CESL_DataMgr *pDM, INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;

	CString sTableName = _T("BO_BOOK_TBL");
	CString sWorkingStatus;
	CString sRecKey;

	CString sPublishForm;
	ids = pDM->GetCellData(_T("단행/연속구분"), nIndex, sPublishForm);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanBookDBProc")); 


	if(sPublishForm.Compare(_T("SE")) == 0)
		sTableName = _T("SE_BOOK_TBL");
	
	ids = GetBOWorkingStatus(sWorkingStatus);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanBookDBProc")); 
	
	ids = pDM->GetCellData(_T("부록KEY"), nIndex, sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("AppendixLoanBookDBProc")); 

	pDM->InitDBFieldData();

	
	ids = pDM->AddDBFieldData(_T("WORKING_STATUS"), _T("STRING"), sWorkingStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("AppendixLoanBookDBProc")); 

	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("부록자료"), __WFILE__, __LINE__), TRUE);

	ids = pMobileApi->MakeUpdateFrame(sTableName, _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("AppendixLoanBookDBProc")); 

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanInfoDBProc(CESL_DataMgr *pDM, INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;

	pDM->InitDBFieldData();

	
	CString sLoanKey;
	CString sLoanDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(sLoanDMAlias);
	if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanInfoDBProc"));

	ids = pLoanDM->GetCellData(_T("대출/반납정보KEY"), pLoanDM->GetRowCount()-1, sLoanKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("AppendixLoanInfoDBProc"));
	
	ids = pDM->AddDBFieldData(_T("LOAN_KEY"), _T("NUMERIC"), sLoanKey, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("AppendixLoanInfoDBProc")); 

	
	const INT nSrcFieldCnt = 3;
	TCHAR *sSrcFieldAlias[nSrcFieldCnt] = 
	{	_T("부록KEY"), _T("자료실"), _T("단행/연속구분") };
	TCHAR *sDstFieldAlias[nSrcFieldCnt] = 
	{	_T("BOOK_KEY"), _T("SHELF_LOC_CODE"), _T("PUBLISH_FORM_CODE") };
	TCHAR *sDstFieldTypeAlias[nSrcFieldCnt] = 
	{	_T("NUMERIC"), _T("STRING"), _T("STRING") };

	CString sGetData;
	for(INT i= 0 ; i < nSrcFieldCnt ;i++)
	{
		ids = pDM->GetCellData(sSrcFieldAlias[i], nIndex, sGetData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("AppendixLoanInfoDBProc"));

		ids = pDM->AddDBFieldData(sDstFieldAlias[i], sDstFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -6, _T("AppendixLoanInfoDBProc"));

	}
	
	CString sSrcUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	const INT nSrcUserFieldCnt = 4;
	TCHAR *sSrcUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("대출자KEY"), _T("대출자소속"), _T("대출자직급"), _T("회원구분") 
	};
	TCHAR *sDstUserFieldAlias[nSrcUserFieldCnt] = 
	{
		_T("USER_KEY"), _T("USER_POSITION_CODE"), _T("USER_CLASS_CODE"), _T("MEMBER_CLASS") 
	};
	TCHAR *sDstUserFieldTypeAlias[nSrcUserFieldCnt] = 
	{
		_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") 
	};

	for(i= 0 ; i < nSrcUserFieldCnt ;i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sSrcUserDMAlias, sSrcUserFieldAlias[i], sGetData, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -7, _T("AppendixLoanInfoDBProc"));

		if(sGetData.IsEmpty() && (CString(_T("NUMERIC")).Compare(sDstUserFieldTypeAlias[i])==0)) sGetData = _T("NULL");
		ids = pDM->AddDBFieldData(sDstUserFieldAlias[i], sDstUserFieldTypeAlias[i], sGetData, TRUE);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -8, _T("AppendixLoanInfoDBProc"));

	}

	
	
	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;


	ids = pDM->AddDBFieldData(_T("LOAN_TYPE_CODE"), _T("STRING"), sLoanType, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -10, _T("AppendixLoanInfoDBProc"));

	
	CString sRecKey;
	ids = pDM->MakeRecKey(sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -11, _T("AppendixLoanInfoDBProc"));

	Send3000ApiSocket(_T("대출"), sRecKey);

	CString sLoanStatus = _T("0");
	ids = pDM->AddDBFieldData(_T("STATUS"), _T("STRING"), sLoanStatus, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -13, _T("AppendixLoanInfoDBProc"));
	CString sLoanDate = _T("SYSDATE");

	ids = pDM->AddDBFieldData(_T("LOAN_DATE"), _T("NUMERIC"), sLoanDate, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -14, _T("AppendixLoanInfoDBProc"));
	
 	
 	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
 		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII-이동도서관"));
	}
	else
	{
		ids = pDM->AddDBFieldData(_T("L_DEVICE"), _T("STRING"), _T("KOLASIII"));
	}
 	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, _T("AppendixLoanInfoDBProc"));
	}
	
	
	ids = pDM->AddDBFieldData(_T("L_WORKER"), _T("STRING"), m_pParentMgr->m_pInfo->USER_ID);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -15, _T("AppendixLoanInfoDBProc"));
	}

	
	CString sLog;
	sLog.Format(_T("%s"), m_pParentMgr->GetWorkLogMsg(_T("부록처리"), __WFILE__, __LINE__));
	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), sLog, TRUE);	
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), m_pParentMgr->m_pInfo->MANAGE_CODE, TRUE);	

	ids = pMobileApi->MakeInsertFrame(_T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -16, _T("AppendixLoanInfoDBProc"));

	return 0;
	

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AppendixLoanUserDBProc(CESL_DataMgr *pDM, INT nIndex, CKolas2up_MobileApi *pMobileApi)
{
EFS_BEGIN

	INT ids;

	ids = IsAppendixPlusLoanBookCnt();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AppendixLoanUserDBProc"));
	if(ids > 0) return 0;

	pDM->InitDBFieldData();	
	CString sRecKey;
	CString sLoanBookCnt;
	CString sDMUserInfoAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	ids = m_pParentMgr->GetDataMgrData(sDMUserInfoAlias, _T("대출자KEY"), sRecKey, 0); 
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("AppendixLoanUserDBProc"));
	
	sLoanBookCnt.Format(_T("%d"), _ttoi(sLoanBookCnt) + 1);

	if(1 > _ttoi(sLoanBookCnt))
	{
		sLoanBookCnt = _T("0");
	}
	ids = pDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanBookCnt, TRUE);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("AppendixLoanUserDBProc"));
	
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("부록처리"), __WFILE__, __LINE__), TRUE);
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("AppendixLoanUserDBProc"));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsAppendixPlusLoanBookCnt()
{
EFS_BEGIN

	return 1;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InputBarCode()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InputBarCode");
	
	CString strLoanKey;
	BOOL bIllProcMat, bIllReturnWill;

	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
	CString strGetBarcodeData;
 	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strGetBarcodeData);
 	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
 	strGetBarcodeData.TrimLeft(); strGetBarcodeData.TrimRight();
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CButton *pOtherCheck = (CButton*)pCM->FindControl(_T("타관반납우선"));
	if(NULL == pOtherCheck) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse && 10000 != m_pParentMgr->m_pInfo->MODE)
	{	
 		if(14 <= strGetBarcodeData.GetLength() && TRUE == IsNumericText(strGetBarcodeData.Left(6)))
 		{ 
			
			CKL_UserSearchDlg dlg(m_pParentMgr, this);
			dlg.m_nMode = 0;
			dlg.m_strUserNo = strGetBarcodeData;
			if(IDOK == dlg.DoModal())
			{
				ids = KL_UserInfoView();
				if(0 > ids) return ids;				
			}			
			m_strSerchCivilNo = _T("");

			return 0;
		}
		
		strGetBarcodeData.MakeUpper();
		CString strRegNoData = strGetBarcodeData;
		
		BOOL bUnionDBLoan = FALSE;
		ISUnionDBLoan(strGetBarcodeData, m_strReturnBookLibCode, bUnionDBLoan);
		
		if(bUnionDBLoan == FALSE || (bUnionDBLoan == TRUE && m_pManageValue->m_sBookCooperativeSys != _T("Y")) ){
			if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
			{ 				
				CKL_UserSearchDlg dlg(m_pParentMgr, this);
				CString strPrevRegNoFromOther;
				ids = m_pManageValue->GetManageValue(_T("기타"), _T("공통"), _T("타관이전등록구분"), _T("타관이전등록구분사용여부"), strPrevRegNoFromOther);			
				if( 0 == strPrevRegNoFromOther.Compare(_T("Y")) )
				{
					StartLog(_T("바코드입력:등록번호변환"));
					ids = CLocCommonAPI::MakeRegNoSpaceToZeroFromLib(m_pParentMgr, strRegNoData, m_pPreRegNoMgr, m_strReturnBookLibCode);
					EndLog();
					if( 0 <= ids )
					{
						strGetBarcodeData = strRegNoData;				
					}
				}		
				dlg.m_strRegNo = strGetBarcodeData;
				dlg.m_strLibCode = m_strReturnBookLibCode;
				dlg.m_nMode = 3;
				
				if(TRUE == pOtherCheck->GetCheck())
				{
					dlg.m_bOtherReturnFirst = TRUE;
				}
				if(IDOK == dlg.DoModal())
				{
					ids = KL_UserInfoView();
					if(0 > ids) return ids;
					if(TRUE == dlg.m_bLoanStopMode)
					{ 
						CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
						if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
						pGrid->SetAt(dlg.m_nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
						pGrid->m_nReverse = dlg.m_nIndex;
						pGrid->SetReverse(dlg.m_nIndex);
					}
				}
				
				if(TRUE == dlg.m_bKLUserYN)
				{
					return 0;
				}
			}
		}
		
		
		BOOL bFind = FALSE;
		if(0 < pMemberDM->GetRowCount())
		{
			
			strGetBarcodeData.MakeUpper();
			CString strRegNoData = strGetBarcodeData;
			CLocCommonAPI::MakeRegNoSpaceToZeroAuto(strRegNoData);

			CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
 			if(NULL != pLoanDM)
			{
				CString strGetData;
 				for(INT i = 0; i < pLoanDM->GetRowCount(); i++)
 				{
 					pLoanDM->GetCellData(_T("등록번호"), i, strGetData);
 					if(strGetData == strRegNoData)
 					{
 						bFind = TRUE;
						break;
 					}
 				}
			}
		}
		if(FALSE == bFind)
		{
			if(TRUE == pOtherCheck->GetCheck())
			{ 
				if(!m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
				{
					
					SelfCloseMessageBox(_T("책이음서비스 접속 시도중입니다."));
					return 0;
				}
				
				strGetBarcodeData.MakeUpper();
				CString strRegNoData = strGetBarcodeData;
				CLocCommonAPI::MakeRegNoSpaceToZeroAuto(strRegNoData);
				
				CKlOtherLoanListDlg dlg(m_pParentMgr);
				dlg.m_strRegNo = strRegNoData;
				
				ids = dlg.RefreshLoanData();
				if (0 == ids) { 
					
					if (IDOK == dlg.DoModal()) {
						
						CESL_DataMgr *pKL_LoanDM = m_pParentMgr->FindDM(_T("DM_KL_OTHER_LOAN_VIEW"));
						if(NULL == pKL_LoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
						CString strLoanLibCode;
						pKL_LoanDM->GetCellData(_T("대출도서관부호"), dlg.m_nIndex, strLoanLibCode);
						
						
						pKL_LoanDM->GetCellData(_T("로컬KEY"), dlg.m_nIndex, strLoanKey);

						
						ISUnionDBLoan(strGetBarcodeData, strLoanLibCode, bUnionDBLoan);
						
						
						if(bUnionDBLoan == FALSE || (bUnionDBLoan == TRUE && m_pManageValue->m_sBookCooperativeSys != _T("Y")) ){					
							ids = ISIllProcMat(strLoanKey, bIllProcMat);
							
							if ( ids ) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
							
							if ( bIllProcMat == TRUE ) {
								AfxMessageBox(_T("선택한 자료는 책두레진행자료입니다. 책두레반납을 하여 주십시오."));
								return 0;
							}						
							
							CString strUserNo, strRegNo;
							pKL_LoanDM->GetCellData(_T("통합대출자번호"), dlg.m_nIndex, strUserNo);
							m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strUserNo);
							ids = SPFExcutePath(_T("바코드업무"));
							if (0 != ids) return 0;
						}
					}
				}
				else if (200 == ids) {
					
					SelfCloseMessageBox(_T("조회된 대출 현황이 없습니다."));
				}
				else {
					
					SelfCloseMessageBox(_T("대출현황 조회에 실패하였습니다."));
				}

				if(bUnionDBLoan == FALSE || (bUnionDBLoan == TRUE && m_pManageValue->m_sBookCooperativeSys != _T("Y")) )
				return 0;
			}
		}
	}
	
	if(m_pCheckLoan->IsSunchang())
	{		
		if(pMemberDM->GetRowCount() == 0)
		{
			CString sRegNo;
			ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);

			ids = SPFExcutePath(_T("회원_검색_NOASK"));
			
			if(pMemberDM->GetRowCount() > 0)
				return 0;

			m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
		}
	}

	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("InputBarCode"));

	if(pMemberDM->GetRowCount() > 0)
	{
		CString sRegNo;		
		ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -32, _T("InputBarCode"));

		CString	strBarcodeReceiptPrint, strBarcodeReceiptPrintAll;
		CString	strReceiptUseFlag;
		CString strPrintItem;
		CString strPrintMode;
		CString strPrintDevice;
		CString strPreview;				
		CString strNotify;
			
		m_pManageValue->GetReceiptManageValue(strBarcodeReceiptPrint, strBarcodeReceiptPrintAll, strPrintItem,strPrintMode, strPrintDevice, strPreview, strNotify);
		
		if(sRegNo == strBarcodeReceiptPrint || sRegNo == strBarcodeReceiptPrintAll)
		{
			StartLog(_T("바코드입력:영수증출력"));
			if(m_ReceiptDlg->GetSafeHwnd() != NULL)
			{
				if(sRegNo == strBarcodeReceiptPrint)
				{
					m_ReceiptDlg->BarcodePrintCommand();
				}
				else
				{
					m_ReceiptDlg->BarcodePrintCommand(2);
				}				
			}
			else
			{
				m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG(0, m_pParentMgr, m_pManageValue);
				m_ReceiptDlg->pMain = m_pParentMgr->pMain;
				m_ReceiptDlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
				m_ReceiptDlg->m_pInfo = m_pParentMgr->m_pInfo;				
				if(m_ReceiptDlg->Create(m_pParentMgr))
				{
					if(sRegNo == strBarcodeReceiptPrint)
					{
						m_ReceiptDlg->BarcodePrintCommand(1);
					}
					else
					{
						m_ReceiptDlg->BarcodePrintCommand(2);
					}	
				}
			}	
			EndLog();
			return 0;
		}

		if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
		{ 				
			CString strPrevRegNoFromOther;
			ids = m_pManageValue->GetManageValue(_T("기타"), _T("공통"), _T("타관이전등록구분"), _T("타관이전등록구분사용여부"), strPrevRegNoFromOther);			
			if(0 == strPrevRegNoFromOther.Compare(_T("Y")) )
			{
				StartLog(_T("바코드입력:등록번호변환"));
				ids = CLocCommonAPI::MakeRegNoSpaceToZeroFromLib(m_pParentMgr, sRegNo, m_pPreRegNoMgr, m_strReturnBookLibCode);
				EndLog();			
			}
		}
		else
		{
			StartLog(_T("바코드입력:등록번호변환"));
			CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);
			EndLog();
		}
		
		CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -33, _T("InputBarCode"));

		CString sTmpRegNo,sBookKey,sPublishForm,sWorkingStatus,sPrevWorkingStatus,sLoanStatus;
		
		CString sBookLibILLUseYN, sBookLibKLUseYN, sLocal;		

		if(!sRegNo.IsEmpty())
		{
			for(INT i = 0 ; i < pLoanInfoDM->GetRowCount(); i++)
			{
				ids = pLoanInfoDM->GetCellData(_T("등록번호"), i, sTmpRegNo);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -34, _T("InputBarCode"));

				ids = pLoanInfoDM->GetCellData(_T("단행/연속구분"), i, sPublishForm);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -35, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("상태"), i, sWorkingStatus);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -36, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("대출상태"), i, sLoanStatus);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("이전자료상태"), i, sPrevWorkingStatus);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -38, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("대출/반납정보KEY"), i, strLoanKey);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -39, _T("InputBarCode"));
				ids = pLoanInfoDM->GetCellData(_T("현황구분"), i, sLocal);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -42, _T("InputBarCode"));

				if(sLoanStatus.Compare(_T("1")) == 0) continue;
				if(sLoanStatus.Compare(_T("3")) == 0) continue;
				if(sLoanStatus.Compare(_T("4")) == 0) continue;
				
				if(sRegNo.Compare(sTmpRegNo) == 0)
				{
					CESL_ControlMgr *pLoanCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
					if(NULL == pLoanCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
					CESL_Grid *pGrid = (CESL_Grid*)(pLoanCM->FindControl(_T("MainGrid")));
					if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
					
					CString strGetData, strMessage;
					CString strOtherReturnKey, strLCTKEY, strLoanLibCode;
					pLoanInfoDM->GetCellData(_T("타관대출KEY"), i, strOtherReturnKey);
					pLoanInfoDM->GetCellData(_T("LCT_KEY"), i, strLCTKEY);
					pLoanInfoDM->GetCellData(_T("대출도서관부호"), i, strLoanLibCode);
					if(sLoanStatus.IsEmpty()) {
											
						pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
						if(pGrid->m_sSelectString != strGetData) { 
							pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
							pGrid->m_nReverse = i;
							pGrid->SetReverse(i);
						}
						
							CString strTransCode;
							pLoanInfoDM->GetCellData(_T("트랜잭션상태"), i, strTransCode);
							if (_T("0") == strTransCode) { 
								SelfCloseMessageBox(_T("지역상호대차 신청중인 자료입니다."));
							} else if (_T("1") == strTransCode) { 
								SelfCloseMessageBox(_T("지역상호대차 발송중인 자료입니다."));
							} else if (_T("3") == strTransCode) { 
								SPFExcutePath(_T("타관대출"));
							} else { 
								SelfCloseMessageBox(_T("알수없는 지역상호대차 자료입니다."));
							}
				
						return 0;
					}
					
					BOOL bReturn = FALSE;
					int nOtherType = 0;
					ids = BookReturnTypeCheck(i);
															
					switch(ids)
					{
						case _RETURN_KL_TO_KL:
							m_strReturnBookLibCode = strLoanLibCode;
							if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
							{ 
								CKL_UserSearchDlg dlg(m_pParentMgr, this);
								strGetBarcodeData.MakeUpper();
								CLocCommonAPI::MakeRegNoSpaceToZero(strGetBarcodeData);

								dlg.m_strRegNo = strGetBarcodeData;
								dlg.m_strLibCode = m_strReturnBookLibCode;
								dlg.m_nMode = 3;
								
								if(TRUE == pOtherCheck->GetCheck())
								{
									dlg.m_bOtherReturnFirst = TRUE;
								}
								if(IDOK == dlg.DoModal())
								{
									ids = KL_UserInfoView();
									if(0 > ids) return ids;
									if(TRUE == dlg.m_bLoanStopMode)
									{ 
										pGrid->SetAt(dlg.m_nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
										pGrid->m_nReverse = dlg.m_nIndex;
										pGrid->SetReverse(dlg.m_nIndex);
									}
								}
							
								bReturn = TRUE;
							}
							break;
						case _RETURN_LILL_TO_BOOKLIB:
						
						
						
							pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
							if(pGrid->m_sSelectString != strGetData) { 
								pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
								pGrid->m_nReverse = i;
								pGrid->SetReverse(i);
							}			
						
							break;
						case _RETURN_LILL_TO_LILL:
						
							if(strOtherReturnKey.IsEmpty() == FALSE){
								nOtherType = 0; 
							}else if(strLCTKEY.IsEmpty() == FALSE && strLoanLibCode.IsEmpty() == FALSE && strLoanLibCode != m_pParentMgr->m_pInfo->LIB_CODE){
								nOtherType = 1; 
							}
							
							pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
							if(pGrid->m_sSelectString != strGetData) { 
								pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
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
								if(!m_pManageValue->m_nSetloanStopMode) { 
									ids = OtherReturn(nOtherType);
								}
							}	else {
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
								ids = OtherReturn(nOtherType);
							}	

							bReturn = TRUE;
							break;
						case _MESSAGE_NO_USE_KL_LILL:
							strMessage.Format(	_T("책이음상호대차 자료입니다.\r\n")
												_T("대출 도서관에 반납하시기 바랍니다."));
							SelfCloseMessageBox(strMessage, NULL, 4000);
							bReturn = TRUE;
							break;
						case _MESSAGE_NO_USE_ILL:
							strMessage.Format(	_T("책두레 자료입니다.\r\n")
												_T("대출 도서관에 반납하시기 바랍니다."));
							SelfCloseMessageBox(strMessage, NULL, 4000);
							bReturn = TRUE;
							break;
						case _MESSAGE_NO_USE_KL:
							strMessage.Format(	_T("책이음 자료입니다.\r\n")
												_T("대출 도서관에 반납하시기 바랍니다."));
							SelfCloseMessageBox(strMessage, NULL, 4000);
							bReturn = TRUE;
							break;
						case _RETURN_FAIL:
							bReturn = TRUE;
							break;
						default:
							break;
					}
					
					if(bReturn == TRUE){
						return 0;
					}
				
					ids = pLoanInfoDM->GetCellData(_T("책KEY"), i, sBookKey);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -38, _T("InputBarCode"));

					pDM->InsertRow(-1);
					ids = pDM->SetCellData(_T("책KEY"),sBookKey,0);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -39, _T("InputBarCode"));

					ids = pDM->SetCellData(_T("등록번호"),sTmpRegNo,0);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -40, _T("InputBarCode"));

					BarCodeReturn(pDM);
					if(0 > ids) return ids;
					pDM->FreeData();
					return 0;
				}
			}
		}
	}
		
	CString sRegNo;	
	CString strUserNo;
	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -50, _T("InputBarCode"));
	
	strUserNo = sRegNo;

	StartLog(_T("바코드입력:등록번호변환"));
	CString strBookManageCode;

	if(sRegNo.GetLength() == 19){
		CString strBarcodeLibCode	= sRegNo.Mid(1,6);
		CString strBarcode			= sRegNo.Mid(7);
		if(pMemberDM->GetRowCount() > 0){	
			if(m_pManageValue->GetLibDMValue(strBarcodeLibCode, _T("")) == FALSE){
				CString strMsg;
				strMsg.Format(_T("다른도서관 자료는 처리 할 수 없습니다.\r\n\r\n등록번호 : [%s]"), strBarcode);
				m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), _T(""));
				AfxMessageBox(strMsg);
				return 0;
			}
		}else{

			CKL_UserSearchDlg dlg(m_pParentMgr, this);
			dlg.m_strRegNo = strBarcode;
			dlg.m_strLibCode = strBarcodeLibCode;
			dlg.m_nMode = 3;
			if(TRUE == pOtherCheck->GetCheck())
			{
				dlg.m_bOtherReturnFirst = TRUE;
			}		
			if(IDOK == dlg.DoModal())
			{
				ids = KL_UserInfoView();
				if(0 > ids) return ids;
				if(TRUE == dlg.m_bLoanStopMode)
				{ 
					CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
					if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
					pGrid->SetAt(dlg.m_nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
					pGrid->m_nReverse = dlg.m_nIndex;
					pGrid->SetReverse(dlg.m_nIndex);
				}
		
		
			}	
			return 0;
		}
	}
	
	GetBookManageCode(strBookManageCode);
	
	CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, strBookManageCode, m_pPreRegNoMgr);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -51, _T("InputBarCode"));
	EndLog();

	if (m_pCheckLoan->IsOtherManageReturn() > 0)
	{
		CString strRegNo = strUserNo;
		strRegNo.MakeUpper();
 		ids = CLocCommonAPI::MakeRegNoSpaceToZero(strRegNo);
 		if(0 > ids) return -2;
		
		if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
		{ 				
			CString strPrevRegNoFromOther;
			ids = m_pManageValue->GetManageValue(_T("기타"), _T("공통"), _T("타관이전등록구분"), _T("타관이전등록구분사용여부"), strPrevRegNoFromOther);			
			if(0 == strPrevRegNoFromOther.Compare(_T("Y")) )
			{
				CString strRegNoData = strUserNo;
				StartLog(_T("바코드입력:등록번호변환"));
				ids = CLocCommonAPI::MakeRegNoSpaceToZeroFromLib(m_pParentMgr, strRegNoData, m_pPreRegNoMgr, m_strReturnBookLibCode);
				EndLog();
				
				if( 0 <= ids )
				{
					strRegNo = strRegNoData;				
				}
			}
		}

		CString strQuery;
		CString strValue;
		strQuery.Format(_T("SELECT USER_KEY, REC_KEY, MANAGE_CODE, STATUS, BOOK_KEY FROM LS_WORK_T01 ")
						_T("WHERE STATUS IN ('0', '2') AND REG_NO = '%s'"),	strRegNo);

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);	
		TmpDM.RestructDataManager(strQuery);
	
		if (TmpDM.GetRowCount() == 1)
		{
			CString strUserKey;
			CString strBookManageCode;
			CString strStatus;
			CString strBookKey;

			TmpDM.GetCellData(0, 0, strUserKey);
			TmpDM.GetCellData(0, 2, strBookManageCode);
			TmpDM.GetCellData(0, 3, strStatus);
			TmpDM.GetCellData(0, 4, strBookKey);

			if (strBookManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE)
			{
				SearchUserBookFromKey(strUserKey);					
				m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strRegNo);

				
				if(!m_pManageValue->m_nSetloanStopMode)
				{ 
					
					ids = BarCodeReturnBookCheck( strBookKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -5 , _T("BarCodeReturn") ); 
					
					ReturnBook(FALSE);
				}
				else
				{ 
					
					ids = BarCodeReturnBookCheck( strBookKey );
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd, -5 , _T("BarCodeReturn") );
				}
				return 0;
			}
		}
	}
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	
	if(sRegNo.GetLength() > nRegCodeLength)
	{
		CString sDescRegCode;
		CString sRegCode = sRegNo.Left(nRegCodeLength);
	
		m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("등록구분"), sRegCode, sDescRegCode);
		
		if(sDescRegCode.IsEmpty() && sRegNo.GetLength() != 12)
		{
			StartLog(_T("바코드입력:이용자검색준비"));
			CString sSql;
			CString sWhere;
			MakeSearchMemberSql(sWhere);

			sSql.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL CLU WHERE (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1') AND %s"), sWhere);

			CESL_DataMgr DM;
			DM.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pParentMgr->m_pInfo->MODE));

			CString sCount;
			CLocCommonAPI::GetOneValueQuery(m_pParentMgr, &DM, sSql, sCount);
			EndLog();

			if(_ttoi(sCount) > 0)
			{
				ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strUserNo);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("InputBarCode"));
				
				ids = SPFExcutePath(_T("회원_검색"));				
				
				if(1 == ids)
				{					
					return 0;
				}				

				ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("InputBarCode"));

				if(pMemberDM->GetRowCount() > 0)
					return 0;
			}
		}
	}

	StartLog(_T("바코드입력:책정보검색"));
	ids = GetBarCodeBookInfo(pDM);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("InputBarCode"));
	EndLog();
	
	CESL_DataMgr ILLDM;
	ILLDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);	
	CString strILLRegNo;

	StartLog(_T("바코드입력:책두레반납"));

	INT nOtherManageReturn = m_pCheckLoan->IsOtherManageReturn();
	INT nILLCnt = -1;
	BOOL bIsILLWork = FALSE;
	if(_T("Y") == m_pManageValue->m_sBookCooperativeSys || nOtherManageReturn == 1)
	{ 
		int ids = 0;
		CString  strRegNo;
		ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strILLRegNo);
 		strILLRegNo.MakeUpper();

		ids = CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, strILLRegNo, strBookManageCode, m_pPreRegNoMgr);		
 		if(0 > ids) return -2;

		CString strQuery;
		CString strValue;
		if(nOtherManageReturn == 1)
		{
			strQuery.Format(
				_T("SELECT USER_KEY, REC_KEY, MANAGE_CODE, STATUS, BOOK_KEY FROM LS_WORK_T01 ")
				_T("WHERE REG_NO = '%s' AND MANAGE_CODE NOT IN ('%s') AND STATUS IN ('0', '2') "),
				strILLRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE);
		}
		else
		{
			if(_T("I") == m_pManageValue->m_sBookCooperativeMode)
			{
				strQuery.Format(
					_T("SELECT USER_KEY, REC_KEY, MANAGE_CODE, STATUS, BOOK_KEY FROM LS_WORK_T01 ")
					_T("WHERE REG_NO = '%s' AND ((MANAGE_CODE NOT IN ('%s') AND STATUS IN ('0', '2')) OR (STATUS IN ('L', 'D')))"),
					strILLRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE);
			}
			else if(_T("D") == m_pManageValue->m_sBookCooperativeMode)
			{
				strQuery.Format(
					_T("SELECT USER_KEY, REC_KEY, MANAGE_CODE, STATUS, BOOK_KEY FROM LS_WORK_T01 ")
					_T("WHERE REG_NO = '%s' AND ((MANAGE_CODE NOT IN ('%s') AND STATUS IN ('0', '2')) OR (STATUS IN ('L', 'D'))) UNION ")
					_T("SELECT LS.USER_KEY, LS.REC_KEY, LS.MANAGE_CODE, LS.STATUS, LS.BOOK_KEY ")
					_T("FROM LS_WORK_T01 LS, ILL_TRANS_TBL ITL ")
					_T("WHERE LS.REC_KEY = ITL.LOAN_KEY AND ITL.REG_NO = '%s' AND LS.STATUS = 'O' AND ITL.APL_LIBCODE = '%s' AND ITL.STATE = '0014'"),
					strILLRegNo, m_pParentMgr->m_pInfo->MANAGE_CODE, strILLRegNo, m_pParentMgr->m_pInfo->LIB_CODE);
			}
		}
		
		ILLDM.RestructDataManager(strQuery);
		nILLCnt = ILLDM.GetRowCount();

	} 
	
	EndLog();
	if(ids > 0 && 0 >= nILLCnt)
	{
		ids = BarCodeMemberSearch();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("InputBarCode"));

		if(ids > 0) 
		{
			ids = IsSearchedUser();
			if(0 > ids) return ids;
			if(ids > 0) 
			{
				MoveRegNoToNextProcBasket(EXP_LOAN);
			}

			if(ids == 0) 
			{
				ids = MessageBox(m_pParentMgr->m_hWnd, _T("입력한 등록번호를 가진 자료가 존재하지 않습니다. 긴급대출하시겠습니까?"), _T("대출/반납"), MB_YESNO | MB_ICONQUESTION);
				if(ids == IDYES)
				{
					return 1;
				}
				return 0;
			}
		}

		return 0;
	}
	else if(ids > 0 && nILLCnt > 0)
	{
		bIsILLWork = TRUE;
	}
	
	
	else if(ids == 0 && nILLCnt > 0)
	{
		CString sBookWorkingStatus;
		ids = pDM->GetCellData(_T("상태"), 0, sBookWorkingStatus);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("InputBarCode"));
		}
	
		
		if(pMemberDM->GetRowCount() == 0 && 
			((sBookWorkingStatus.Compare(_T("BOL112N"))==0) ||
			(sBookWorkingStatus.Compare(_T("SEL212N"))==0) ||
			(sBookWorkingStatus.Compare(_T("SEL112N"))==0) ||
			(sBookWorkingStatus.Compare(_T("BOL411O"))==0) || 
			(sBookWorkingStatus.Compare(_T("SEL411O"))==0)))
		{
				bIsILLWork = TRUE;
		}
		else if((sBookWorkingStatus.Compare(_T("BOL411O"))==0) || 
			(sBookWorkingStatus.Compare(_T("SEL411O"))==0))
		{
				bIsILLWork = TRUE;
		}
		else
		{
			ids = MessageBox(m_pParentMgr->m_hWnd, _T("입력한 등록번호를 가진 자료가 자관의 자료입니까?\r\n\r\nYES : 자관자료로 처리, NO : 타관자료로 처리, CANCEL : 취소"), _T("대출/반납"), MB_YESNOCANCEL | MB_ICONQUESTION);
			if(IDYES == ids)
			{
				bIsILLWork = FALSE;
			}
			else if(IDNO == ids)
			{
				bIsILLWork = TRUE;
			}
			else
			{
				return 1000;
			}
		}
	}

	if(FALSE == bIsILLWork)
	{
	CString sLoanUserKey, sIsSelfReturning;
	CString sBookWorkingStatus;
	ids = pDM->GetCellData(_T("상태"), 0, sBookWorkingStatus);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("InputBarCode"));
	
	ids = pDM->GetCellData(_T("대출자KEY"), 0, sLoanUserKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("InputBarCode"));
	ids = pDM->GetCellData(_T("무인반납중여부"), 0, sIsSelfReturning);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("InputBarCode"));
	
	
	if(_ttoi(sIsSelfReturning) > 0)
	{
		CString sMsg;
		sMsg.Format(_T("'무인반납중'인 자료이기 때문에 처리 할 수 없습니다. "));
		SelfCloseMessageBox(sMsg);		
		pDM->FreeData();

		return 3;
	}
	
	if(sLoanUserKey.IsEmpty())
	{
		if(sBookWorkingStatus.Compare(_T("BOL112N"))==0 ||
			(sBookWorkingStatus.Compare(_T("SEL212N"))==0) ||
			(sBookWorkingStatus.Compare(_T("SEL112N"))==0)
			)
		{
			ids = BarCodeLoan();
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("InputBarCode"));
			pDM->FreeData();
			return 0;			
		}
		else
		{
			CLocCommonAPI::GetWorkingStatusDescToCode(sBookWorkingStatus, 1);
			CString sMsg;
			sMsg.Format(_T("'%s'이기 때문에 대출 할 수 없습니다. "), sBookWorkingStatus);
			SelfCloseMessageBox(sMsg);
			
			pDM->FreeData();
			return 0;
		}
	}
	
	else
	{
		ids = BarCodeReturn(pDM);
		if(0 > ids) return ids;
		pDM->FreeData();
		return 0;		
	}
	}
	
	else
	{
		CString strUserKey;
		CString strBookManageCode;
		CString strStatus;
		CString strBookKey;
		BOOL bUseSearchUser = FALSE;
		
		if(1 == nILLCnt)
		{
			ILLDM.GetCellData(0, 0, strUserKey);
			
			ILLDM.GetCellData(0, 2, strBookManageCode);
			ILLDM.GetCellData(0, 3, strStatus);
			ILLDM.GetCellData(0, 4, strBookKey);
			bUseSearchUser = TRUE;
		}
		else if(1 < nILLCnt)
		{
			CString strRecKeyList;
			
			CString strRecKey;			
			const int ROW_COUNT = ILLDM.GetRowCount();
			for(int nRow = 0; nRow < ROW_COUNT; nRow++)
			{
				ILLDM.GetCellData(nRow, 1, strRecKey);
				strRecKeyList = strRecKeyList + _T(", ") + strRecKey;
			}
			strRecKeyList.TrimLeft(_T(", "));
			
			CILL_RETURN_SEARCH_BOOK Dlg(m_pParentMgr);
			Dlg.m_sBookCooperativeMode = m_pManageValue->m_sBookCooperativeMode;
			Dlg.m_strOption = _T("A.REC_KEY IN (") + strRecKeyList + _T(")");
			if(IDOK == Dlg.DoModal())
			{
				strUserKey = Dlg.m_strUserKey;
				strStatus = Dlg.m_strStatus;
				strBookKey = Dlg.m_strBookKey;
				strBookManageCode = Dlg.m_strBookManageCode;
				bUseSearchUser = TRUE;
			}
		}	

		if(TRUE == bUseSearchUser)
		{
		
		
			CString strTmpQuery, strBarcodeUseYN, strGetData, strMessage, strOtherReturnKey, strLCTKEY, strLoanLibCode, sTmpRegNo;
			BOOL bReturn = FALSE;
			int nOtherType = 0, nids = 0, i = 0;

			CESL_ControlMgr *pLoanCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
			if(NULL == pLoanCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
			CESL_Grid *pGrid = (CESL_Grid*)(pLoanCM->FindControl(_T("MainGrid")));
			if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
			CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
			if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -33, strFuncName);
		
			ids = BookReturnTypeCheck(strILLRegNo);
			if(ids == _RETURN_FAIL){
				
				SearchLoanInfo(FALSE, strUserKey);
				ids = BookReturnTypeCheck(strILLRegNo);
			}

			for(i = 0 ; i < pLoanInfoDM->GetRowCount(); i++)
			{
				nids = pLoanInfoDM->GetCellData(_T("등록번호"), i, sTmpRegNo);
				if(0 > nids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -34, _T("InputBarCode"));

				if(sTmpRegNo.Compare(strILLRegNo) == 0){
					pLoanInfoDM->GetCellData(_T("타관대출KEY"), i, strOtherReturnKey);
					pLoanInfoDM->GetCellData(_T("LCT_KEY"), i, strLCTKEY);
					pLoanInfoDM->GetCellData(_T("대출도서관부호"), i, strLoanLibCode);
					break;
				}
			}

			switch(ids)
			{
				case _RETURN_KL_TO_KL:
					m_strReturnBookLibCode = strLoanLibCode;
					if(!m_strReturnBookLibCode.IsEmpty() && m_strReturnBookLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
					{ 
						CKL_UserSearchDlg dlg(m_pParentMgr, this);
						strGetBarcodeData.MakeUpper();
						CLocCommonAPI::MakeRegNoSpaceToZero(strGetBarcodeData);
	
						dlg.m_strRegNo = strGetBarcodeData;
						dlg.m_strLibCode = m_strReturnBookLibCode;
						dlg.m_nMode = 3;
						
						if(TRUE == pOtherCheck->GetCheck())
						{
							dlg.m_bOtherReturnFirst = TRUE;
						}
						if(IDOK == dlg.DoModal())
						{
							ids = KL_UserInfoView();
							if(0 > ids) return ids;
							if(TRUE == dlg.m_bLoanStopMode)
							{ 
								pGrid->SetAt(dlg.m_nIndex, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
								pGrid->m_nReverse = dlg.m_nIndex;
								pGrid->SetReverse(dlg.m_nIndex);
							}
						}
						bReturn = TRUE;
					}
					break;
				case _RETURN_LILL_TO_BOOKLIB:
					pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
					if(pGrid->m_sSelectString != strGetData) { 
						pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
						pGrid->m_nReverse = i;
						pGrid->SetReverse(i);
					}				
					
					break;
				case _RETURN_LILL_TO_LILL:
					pGrid->GetAt(i, pGrid->m_nSelectIdx, strGetData);
					if(pGrid->m_sSelectString != strGetData) { 
						pGrid->SetAt(i, pGrid->m_nSelectIdx, pGrid->m_sSelectString);
						pGrid->m_nReverse = i;
						pGrid->SetReverse(i);
						
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
						if(!m_pManageValue->m_nSetloanStopMode) { 
							ids = OtherReturn();
						}
					}	else {
						
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
						ids = OtherReturn();
					}																				
					bReturn = TRUE;
					break;

				case _RETURN_OTHER_MANAGE:	
				case _RETURN_OTHER_MANAGE_HOLD:	
				
					SearchUserBookFromKey(strUserKey);		
					m_pParentMgr->SetControlData(	_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strILLRegNo);
					
					if(!m_pManageValue->m_nSetloanStopMode)
					{ 
						
						nids = BarCodeReturnBookCheck(strBookKey);
						if(0 >nids) {
							bReturn = TRUE;
							break;
						}
						
						ReturnBook(FALSE);					
					}
					else
					{ 
						
						nids = BarCodeReturnBookCheck(strBookKey);
						if(0 >nids) {
							bReturn = TRUE;
							break;
						}

					}
					bReturn = TRUE;
					break;
					
				case _RETURN_NORMAL_TO_ILL:	
				case _RETURN_ILL_TO_BOOKLIB:	
				case _RETURN_ILL_TO_ILL:	
					SearchUserBookFromKey(strUserKey);	
					m_pParentMgr->SetControlData(	_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), strILLRegNo);
										
					strTmpQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME='바코드반납사용여부' AND MANAGE_CODE = '%s'"),m_pParentMgr->m_pInfo->MANAGE_CODE);
					pDM->GetOneValueQuery(strTmpQuery,strBarcodeUseYN);
					
					if(strBarcodeUseYN == _T("N"))
					{
						SPFExcutePath(_T("책두레반납화면"));
					}
					else{
						if(!m_pManageValue->m_nSetloanStopMode)
						{
							ILL_BarCodeReturn(strBookKey);
						}
						else
						{
							BarCodeReturnBookCheck(strBookKey);
						}
					}
					
					bReturn = TRUE;
					break;

				case _MESSAGE_NO_USE_KL_LILL:
					strMessage.Format(	_T("책이음상호대차 자료입니다.\r\n")
										_T("대출 도서관에 반납하시기 바랍니다."));
					SelfCloseMessageBox(strMessage, NULL, 4000);
					bReturn = TRUE;
					break;
				case _MESSAGE_NO_USE_ILL:
					strMessage.Format(	_T("책두레 자료입니다.\r\n")
										_T("대출 도서관에 반납하시기 바랍니다."));
					SelfCloseMessageBox(strMessage, NULL, 4000);
					bReturn = TRUE;
					break;
				case _MESSAGE_NO_USE_KL:
					strMessage.Format(	_T("책이음 자료입니다.\r\n")
										_T("대출 도서관에 반납하시기 바랍니다."));
					SelfCloseMessageBox(strMessage, NULL, 4000);
					bReturn = TRUE;
					break;
				case _RETURN_FAIL:
					bReturn = TRUE;
					break;
				default:
					break;
			}
			
			if(bReturn == TRUE){
				return nids;
			}
			
		}
	}
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetBarCodeBookInfo(CESL_DataMgr *pDM)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetBarCodeBookInfo");

	pDM->FreeData();

	CString sRegNo;

	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	
	if(sRegNo.GetLength() <= nRegCodeLength) return 2;
	
	CString strBookManageCode;
	GetBookManageCode(strBookManageCode);
	CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, strBookManageCode, m_pPreRegNoMgr);
	
	ids = InitExpDM();
	if(0 > ids) return ids;

	INT nBookCnt = 0;
	BOOL bRegNo = TRUE;

	for(INT i = 0 ; i < 2 ; i++)
	{
		ids = SearchBookInfo(sRegNo, nBookCnt, i, bRegNo);
		if(0 > ids) return ids;

		if(nBookCnt > 0) i = 2;
	}

	if(nBookCnt < 1) return 1;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BarCodeMemberSearch()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("BarCodeMemberSearch");

	CString sRegNo;
	ids = m_pParentMgr->GetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sSql;
	
	CString sTmp = sRegNo;
	sTmp.MakeUpper();

	CString sWhere;
	MakeSearchMemberSql(sWhere);

	sSql.Format(_T("SELECT COUNT(1) FROM CO_LOAN_USER_TBL CLU WHERE (CLU.USER_CLASS!='3' OR CLU.MEMBER_CLASS!='1') AND %s"), sWhere);

	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(CLocCommonAPI::GetDBConnection(m_pParentMgr->m_pInfo->MODE));

	CString sCount;
	CLocCommonAPI::GetOneValueQuery(m_pParentMgr, &DM, sSql, sCount);

	if(_ttoi(sCount) < 1) return 1;	
	
	ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	ids = SPFExcutePath(_T("회원_검색"));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BarCodeLoan()
{
EFS_BEGIN

	INT ids;
	
	ids = IsSearchedUser();
	if(0 > ids) return ids;
	if(ids > 0)
	{
		MoveRegNoToNextProcBasket(EXP_LOAN);
		return 0;
	}
	
	SPFExcutePath(_T("대출"));
	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::BarCodeReturn(CESL_DataMgr *pDM)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("BarCodeReturn");

	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sUserKey;
	ids = pDM->GetCellData(_T("대출자KEY"), 0, sUserKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString sBookKey;
	ids = pDM->GetCellData(_T("책KEY"), 0, sBookKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	
	
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CButton *pOtherCheck = (CButton*)pCM->FindControl(_T("타관반납우선"));
	if(NULL == pOtherCheck) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	
	if(pMemberDM->GetRowCount() == 0)
	{
		
		INT ids = SearchUserBookFromKey(sUserKey);
		if(0 != ids)
		{
			
			return ids;
		}
		if(!m_pManageValue->m_nSetloanStopMode)
		{ 
			ids = BarCodeReturn(pDM);
			if(0 > ids) return ids;
		}
		else
		{			
			ids = BarCodeReturnBookCheck(sBookKey);
			if(0 > ids) return ids;
		}
		return 0;
	}
	
	CString sStatus;
	
	CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	
	CString sTmpBookKey;
	CString sDelayDay;
	CString sSelect;
	
	CString strTmpQuery;
	BOOL    bDelayFlag = FALSE;
	if(!sBookKey.IsEmpty())
	{
		for(INT i = 0 ; i < pLoanInfoDM->GetRowCount(); i++)
		{
			ids = pLoanInfoDM->GetCellData(_T("대출상태"), i, sStatus);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

			if(sStatus.Compare(_T("3")) == 0) continue;
			if(sStatus.Compare(_T("1")) == 0) continue;
			if(sStatus.Compare(_T("4")) == 0) continue;

			ids = pLoanInfoDM->GetCellData(_T("책KEY"), i, sTmpBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
			
			if(sBookKey.Compare(sTmpBookKey) == 0)
			{
				CString strTmpQuery, strMessage;
				CString strBarcodeUseYN = _T("N");
				BOOL bReturn = FALSE;
				int nOtherType = 0;
				ids = BookReturnTypeCheck(i);
				
				switch(ids)
				{
					case _RETURN_NORMAL_TO_ILL:	
					case _RETURN_ILL_TO_BOOKLIB:	
					case _RETURN_ILL_TO_ILL:	
					
						if(TRUE == pOtherCheck->GetCheck()) {							
							if(IDNO == AfxMessageBox(_T("타관반납우선이 체크되어 있으나, 책두레처리를 진행합니다. 계속하시겠습니까?"), MB_ICONQUESTION|MB_YESNO) ) {
								bReturn = TRUE;
								break;
							}
						}
						
						strTmpQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME='바코드반납사용여부' AND MANAGE_CODE = '%s'"),m_pParentMgr->m_pInfo->MANAGE_CODE);
						pDM->GetOneValueQuery(strTmpQuery,strBarcodeUseYN);
						if(strBarcodeUseYN == _T("N"))
						{
							SPFExcutePath(_T("책두레반납화면"));
						}
						else
						{
							CESL_Grid* pGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid"));
							if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
							CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
							
							CString strCompBookKey = _T("");
							CString strCheck = _T("");
							for(INT i = 0 ; i < pLoanDM->GetRowCount() ; i++)
							{
								pLoanDM->GetCellData(_T("책KEY"),i,strCompBookKey);
								if(strCompBookKey.Compare(sBookKey)==0)
								{
									
									if(!m_pManageValue->m_nSetloanStopMode){
										BarCodeReturnBookCheck(sBookKey);												
									}
									pGrid->SelectMakeList();
									pGrid->GetAt(i,0,strCheck);

									if(strCheck == _T("V"))
									{
										ILL_BarCodeReturn(sBookKey);
									}
									else
									{
										BarCodeReturnBookCheck(sBookKey);
									}
								}
							}
							
						}
						
						bReturn = TRUE;
						break;
					case _RETURN_LILL_TO_BOOKLIB:
						break;
					case _MESSAGE_NO_USE_KL_LILL:
						strMessage.Format(	_T("책이음상호대차 자료입니다.\r\n")
											_T("대출 도서관에 반납하시기 바랍니다."));
						SelfCloseMessageBox(strMessage, NULL, 4000);
						bReturn = TRUE;
						break;
					case _MESSAGE_NO_USE_ILL:
						strMessage.Format(	_T("책두레 자료입니다.\r\n")
											_T("대출 도서관에 반납하시기 바랍니다."));
						SelfCloseMessageBox(strMessage, NULL, 4000);
						bReturn = TRUE;
						break;
					case _MESSAGE_NO_USE_KL:
						strMessage.Format(	_T("책이음 자료입니다.\r\n")
											_T("대출 도서관에 반납하시기 바랍니다."));
						SelfCloseMessageBox(strMessage, NULL, 4000);
						bReturn = TRUE;
						break;
					case _RETURN_FAIL:

						bReturn = TRUE;
						break;
					default:
						break;
				}
				
				if(bReturn == TRUE){
					return 0;
				}
				
				ids = BarCodeReturnBookCheck(sBookKey);
				if(0 > ids) return ids;
				
				if(m_pManageValue->m_sFeeMngSys == _T("Y") || m_pManageValue->m_ArrearCheckYN == _T("Y"))
				{ 
					
					CString strTempBookKey;
					CString sDelayDay;
					
					
					
					for (INT y=0 ; y<pLoanInfoDM->GetRowCount() ; y++)
					{
						pLoanInfoDM->GetCellData(_T("책KEY"), y, strTempBookKey);					
						if(strTempBookKey == sBookKey)
						{
							ids = pLoanInfoDM->GetCellData(_T("연체일수"), y, sDelayDay);
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
							if(_ttoi(sDelayDay) > 0) bDelayFlag = TRUE;
							break;
						}					
					}
					ReturnBook(bDelayFlag);
				}
				else
				{ 
					ReturnBook(FALSE);
				}

				return 0;
			}
		}
	}

	if(pMemberDM->GetRowCount()>0) 
	{
		::MessageBeep(0xFFFFFFFF);
	}

	SearchUserBookFromKey(sUserKey);
	
 	if(!m_pManageValue->m_nSetloanStopMode)
 	{ 
 		ids = BarCodeReturn(pDM);
 		if(0 > ids) return ids;
 	}
 	else
 	{
 		
 		ids = BarCodeReturnBookCheck(sBookKey);
 		if(0 > ids) return ids;
 	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BarCodeReturnBookCheck(CString sBookKey, INT nFlag/*=0*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("BarCodeReturnBookCheck");

	CString sLoanDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(sLoanDMAlias);
	if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nCheckColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("선정"), nCheckColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName); 


	CString sLoanBookKey;
	BOOL bCheck = FALSE;
	for(INT i = 0 ; i < pLoanDM->GetRowCount() ; i++)
	{
		if(nCheckColIndex == -1) continue;

		pGrid->SetAt(i, nCheckColIndex, _T(""));
		
		if(nFlag == 0)
		{
			ids = pLoanDM->GetCellData(_T("책KEY"), i, sLoanBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
		else if(nFlag == 1)
		{
			ids = pLoanDM->GetCellData(_T("등록번호"), i, sLoanBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
		
		if(sLoanBookKey.Compare(sBookKey) != 0) continue;

		pGrid->SetAt(i, nCheckColIndex, _T("V"));
		pGrid->m_nReverse = i;
		pGrid->SetReverse(i);
		bCheck = TRUE;
	}

	
	if(!bCheck) return 2;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::MakeRegNoSpaceToZero(BOOL bIsUsePrevRegNo)
{
EFS_BEGIN

	INT ids;
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CString sRegNo;

	ids = m_pParentMgr->GetControlData(sCMAlias, _T("대출자번호"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("MakeRegNoSpaceToZero")); 
	
	if(bIsUsePrevRegNo)
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(m_pParentMgr, sRegNo, m_pPreRegNoMgr);
	}
	else
	{
		CLocCommonAPI::MakeRegNoSpaceToZero(sRegNo);
	}

	ids = m_pParentMgr->SetControlData(sCMAlias, _T("대출자번호"), sRegNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("MakeRegNoSpaceToZero")); 
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::MakeShelfCode(CString sDMAlias)
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, _T("CBL_LOC_LOAN_PROC::MakeShelfCode(CString sDMAlias)"));
	}

	INT nRowCount = pDM->GetRowCount();
	
	INT i = 0;
	CString strPublishFormCode;
	CString sShelfCode;
	for(i = 0; i < nRowCount; i++)
	{
		pDM->GetCellData(_T("단행/연속구분"), i, strPublishFormCode);
		if(_T("NB") == strPublishFormCode || _T("긴급") == strPublishFormCode)
		{
			continue;
		}

		ids = CLocCommonAPI::MakeShelfCode(m_pParentMgr, pDM, i, sShelfCode);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("CBL_LOC_LOAN_PROC::MakeShelfCode(CString sDMAlias)"));
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitRegNoControl()
{
EFS_BEGIN

	INT ids;

	ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), _T(""));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("InitRegNoControl")); 
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitUserManageValue()
{
EFS_BEGIN

	INT ids;

	CString sReturnDelayDay;
	ids = m_pCheckLoan->GetReturnDelayDay(sReturnDelayDay, _T("NO_REG_CODE"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("InitUserManageValue")); 
	if(ids > 0) return 0;
	
	ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("반납연기일"), sReturnDelayDay);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("InitUserManageValue")); 

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::IsReLoanBook()
{
EFS_BEGIN

	INT ids;
		
	CString sLoanDate;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("소급대출일"), sLoanDate);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, _T("IsReLoanBook")); 
	}

	ids = m_pCheckLoan->IsReLoanBook(sLoanDate);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, _T("IsReLoanBook")); 
	}
	if(ids > 0)
	{
		return ids;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::CopyGridCheckToDM()
{
EFS_BEGIN

	INT ids;
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("CopyGridCheckToDM")); 


	CESL_Grid *pGrid = GetMainGridPtr();
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("CopyGridCheckToDM")); 

	INT nCheckColIndex = -1;
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, _T("선정"), nCheckColIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -3, _T("CopyGridCheckToDM")); 

	CString sCheck;
	for(INT i = 0 ; i < pDM->GetRowCount() ; i++)
	{
		if(nCheckColIndex == -1) continue;

		ids = pGrid->GetAt(i, nCheckColIndex, sCheck);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, _T("CopyGridCheckToDM"));

		pDM->SetCellData(_T("선정"), _T(""), i);

		if(sCheck.Compare(_T("V")) != 0) continue;

		pDM->SetCellData(_T("선정"), _T("V"), i);
	}
	
	return 0;

EFS_END
return -1;
}

CESL_Grid* CBL_LOC_LOAN_PROC::GetMainGridPtr()
{
EFS_BEGIN

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) return NULL;
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(pGrid == NULL) return NULL;

	return pGrid;

EFS_END
return NULL;
}

INT CBL_LOC_LOAN_PROC::RunAutoProc()
{
EFS_BEGIN

	AfxBeginThread(RunAutoThreadProc, this);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GoAutoProc()
{
	INT ids;

	CString sCMAlias = _T("CM_BO_LOC_3191_AUTO_JOB");
	CString sGridAlias = _T("그리드로그");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -11, _T("GoAutoProc"));
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -12, _T("GoAutoProc"));

	while(pGrid->GetSafeHwnd==NULL) ;

	ids = InitResultScreen();
	if(0 > ids) return ids;
	
	if(m_bBreak) return 100;

	
	ids = AllChangeUserClass();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -2, _T("GoAutoProc"));

	ids = SetResultScreen(50, _T("대출 정지 회원 자동 일괄 해제 작업 완료"));
	if(0 > ids) return ids;

	
	ids = SetResultScreen(50, _T("50% 완료"));
	if(0 > ids) return ids;

	
	if(m_bBreak) return 100;

	
	ids = AllChangeReserveCancel();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, _T("GoAutoProc()"));

	ids = SetResultScreen(100, _T("예약 만기일 지난 예약 자료 자동 예약 취소 작업 완료"));
	if(0 > ids) return ids;

	ids = SetResultScreen(100, _T("작업 완료"));
	if(0 > ids) return ids;

	return 0;
}

UINT RunAutoThreadProc(LPVOID lParam)
{
	INT ids;
	CString sErrorMsg;

	CBL_LOC_LOAN_PROC *pObj = (CBL_LOC_LOAN_PROC *)lParam;
	if(pObj == NULL)
		return -1;

	pObj->m_bStop = FALSE;
	pObj->m_bBreak = FALSE;

	ids = pObj->GoAutoProc();
	if(ids == 100)
	{
		pObj->m_bStopState = 1;
		sErrorMsg = _T("중지되었습니다.");
	}
	else if(0 > ids)
	{
		pObj->m_bStopState = 1;
		sErrorMsg.Format(_T("에러 발생-%d"),ids);
	}
	else if(ids == 0)
	{
		sErrorMsg = _T("완료되었습니다.");
	}

	pObj->SetResultScreen(100, sErrorMsg);
	
	pObj->m_bStopState = 0;
	pObj->m_bStop = TRUE;

	return 0;
}

INT CBL_LOC_LOAN_PROC::InitResultScreen()
{
EFS_BEGIN

	INT ids;
	
	ids = InitResultProgressBar();
	if(0 > ids) return ids;

	ids = InitResultLog();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitResultProgressBar()
{
EFS_BEGIN

	CProgressCtrl *pProgressCtrl;

	pProgressCtrl = GetProgressPtr(m_sResultCMAlias, m_sResultProgressAlias);
	if(pProgressCtrl == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("InitResultProgressBar"));

	pProgressCtrl->SetRange(0,100);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InitResultLog()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("InitResultLog");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(m_sResultDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pDM->FreeData();

	ids = ResultLogAdd(_T("작업시작"));
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SetResultScreen(INT nPos, CString sLog)
{
	INT ids;

	ids = ResultPorgressSetPos(nPos);
	if(0 > ids) return ids;

	ids = ResultLogAdd(sLog);
	if(0 > ids) return ids;

	return 0;
}

INT CBL_LOC_LOAN_PROC::ResultPorgressSetPos(INT nPos)
{
EFS_BEGIN

	CProgressCtrl *pProgressCtrl;

	pProgressCtrl = GetProgressPtr(m_sResultCMAlias, m_sResultProgressAlias);
	if(pProgressCtrl == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("ResultPorgressSetPos"));

	pProgressCtrl->SetPos(nPos);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ResultLogAdd(CString sLog)
{
	INT ids;
	CString strFuncName = _T("ResultLogAdd");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(m_sResultDMAlias);
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InsertRow(-1);

	CString sCurrentTime;
	CTime tCurrentTime = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, sCurrentTime);

	ids = pDM->SetCellData(_T("시간"), sCurrentTime, pDM->GetRowCount()-1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pDM->SetCellData(_T("내용"), sLog, pDM->GetRowCount()-1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	INT nIndex = pDM->GetRowCount()-1;

	CString sGridAlias = _T("그리드로그");

	ids = CLocCommonAPI::InsertGridRow(m_pParentMgr, m_sResultCMAlias, sGridAlias, nIndex, nIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName); 

	return 0;
}

CProgressCtrl* CBL_LOC_LOAN_PROC::GetProgressPtr(CString sCMAlias, CString sProgressAlias)
{
EFS_BEGIN

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) return NULL;

	CESL_Control_Element *pProgressElemnet;
	CProgressCtrl *pProgressCtrl;
	
	pProgressElemnet = pCM->FindControlElement(sProgressAlias);
	if(pProgressElemnet == NULL) return NULL;

	pProgressCtrl = (CProgressCtrl*)(m_pParentMgr->GetDlgItem(pProgressElemnet->CTRL_ID));
	if(pProgressCtrl == NULL) return NULL;

	return pProgressCtrl;	

EFS_END
return NULL;
}

INT CBL_LOC_LOAN_PROC::AllChangeUserClass()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeUserClass");

	ids = SetResultScreen(10, _T("대출 정지 회원 일괄 해제 시작"));
	if(0 > ids) return ids;

	ids = m_pCheckLoan->GetAutoChangeUserClass();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(ids > 0) 
	{
		ids = SetResultScreen(30, _T("관리에 대출 정지 회원 자동 일괄 해제 안함으로 설정되있으므로 작업을 수행하지 않음"));
		if(0 > ids) return ids;

		return 1;
	}

	ids = SetResultScreen(40, _T("DB작업중"));
	if(0 > ids) return ids;

	ids = AllChangeUserClassDBProc();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeUserClassDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeUserClassDBProc");
	CESL_DataMgr *pUserDM;
	CString	sUpdateSql;
	CString	strUserUpdateKeyList[2048];
	CString	strManageValue;
	
	if(_T("Y") == m_pManageValue->m_sBookCooperativeSys)
	{
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
		CString strQuery;
		strQuery.Format(
			_T("UPDATE CO_LOAN_USER_TBL CLU ")
			_T("SET CLU.ILL_EXPIRE_RETURN_CNT=0,CLU.ILL_APL_STOP_SET_DATE=NULL,CLU.ILL_APL_STOP_DATE=NULL ")
			_T("WHERE CLU.ILL_APL_STOP_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD');")
		);
		TmpDM.StartFrame();
		TmpDM.AddFrame(strQuery);	
		TmpDM.SendFrame();
		TmpDM.EndFrame();
	}
	
	
	strManageValue = m_pManageValue->m_strUserShare;
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		pUserDM = m_pParentMgr->FindDM(_T("DM_SHARE_LOAN_USER_LIST"));
		if(pUserDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		sUpdateSql.Format(_T("CLU.USER_CLASS='1' AND (CLU.LOAN_STOP_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') OR CLU.LOAN_STOP_DATE IS NULL) AND CLU.REC_KEY NOT IN (SELECT DISTINCT CL.USER_KEY FROM LS_WORK_T01 CL WHERE CLU.USER_CLASS='1' AND CLU.REC_KEY=CL.USER_KEY AND CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD'))"));
		pUserDM->RefreshDataManager(sUpdateSql);
	}	
	
	sUpdateSql.Format(
		_T("UPDATE CO_LOAN_USER_TBL CLU ")
		_T("SET CLU.USER_CLASS='0',CLU.LOAN_STOP_DATE=NULL,CLU.LAST_WORK='%s' ")
 		_T("WHERE CLU.USER_CLASS='1' AND (CLU.LOAN_STOP_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') OR CLU.LOAN_STOP_DATE IS NULL) AND CLU.REC_KEY NOT IN (SELECT DISTINCT CL.USER_KEY FROM LS_WORK_T01 CL WHERE CLU.USER_CLASS='1' AND CLU.REC_KEY=CL.USER_KEY AND CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD'));")
		, m_pParentMgr->GetWorkLogMsg(_T("대출정지일괄해제"),__WFILE__,__LINE__));
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	pDM->InitDBFieldData();
	pDM->StartFrame();
	pDM->AddFrame(sUpdateSql);
	ids = pDM->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	ids = pDM->EndFrame();
	
	INT i;
	INT nRowCount;
	CString	strUserKey;
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		nRowCount = pUserDM->GetRowCount();	
		for(i = 0; i < nRowCount; i++)
		{
			pUserDM->GetCellData(i, 0, strUserKey);
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(strUserKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("USER_CLASS,LOAN_STOP_DATE")));
			ShareInfoCenterUpload(_T("N"),_T("U"));
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeUserStopClass()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeUserStopClass");

	ids = SetResultScreen(10, _T("대출 정지 회원 일괄 선정 시작"));
	if(0 > ids) return ids;

	ids = m_pCheckLoan->GetAutoChangeUserClass();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(ids > 0) 
	{
		ids = SetResultScreen(30, _T("관리에 대출 정지 회원 자동 일괄 부여 안함으로 설정되있으므로 작업을 수행하지 않음"));
		if(0 > ids) return ids;

		return 1;
	}

	ids = SetResultScreen(40, _T("DB작업중"));
	if(0 > ids) return ids;

	ids = AllChangeUserStopClassDBProc();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::AllChangeUserStopClassDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeUserStopClassDBProc");

	CESL_DataMgr *pUserDM;
	CString	sUpdateSql;
	CString	strUserUpdateKeyList[2048];
	CString	strManageValue;

	strManageValue = m_pManageValue->m_strLoanShare;
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		pUserDM = m_pParentMgr->FindDM(_T("DM_SHARE_LOAN_STOP_USER_LIST"));	
		if(pUserDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
		sUpdateSql.Format(	
			_T("CLU.USER_CLASS='0' AND CLU.REC_KEY IN ")
			_T("(SELECT DISTINCT CL.USER_KEY FROM LS_WORK_T01 CL ")
			_T("WHERE CLU.USER_CLASS='0' AND CLU.REC_KEY=CL.USER_KEY AND CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < TO_DATE(SYSDATE,'YYYY/MM/DD'))"));

		pUserDM->RefreshDataManager(sUpdateSql);
	}	
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("AllChangeUserStopClassDBProc"));
	
	sUpdateSql.Format( _T("UPDATE CO_LOAN_USER_TBL CLU SET USER_CLASS='1', LAST_WORK='%s' ")
						_T("WHERE REC_KEY IN ")
						_T("(SELECT DISTINCT USER_KEY FROM LS_WORK_T01 CL ")
						_T("WHERE CLU.USER_CLASS='0' AND CLU.REC_KEY IN ")
						_T("(SELECT DISTINCT CL.USER_KEY FROM LS_WORK_T01 CL ")
						_T("WHERE CLU.USER_CLASS='0' AND CLU.REC_KEY=CL.USER_KEY AND CL.STATUS IN ('0','2') AND CL.RETURN_PLAN_DATE < TO_DATE(SYSDATE, 'YYYY/MM/DD'));")
						, m_pParentMgr->GetWorkLogMsg(_T("대출정지일괄선정"), __WFILE__, __LINE__));
	
	pDM->InitDBFieldData();
	pDM->StartFrame();
	pDM->AddFrame(sUpdateSql);

	ids = pDM->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pDM->EndFrame();	
	
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		CString	strUserKey;	
		INT		nRowCount;
		INT		i;

		nRowCount = pUserDM->GetRowCount();
		for (i=0 ; i<nRowCount ; i++)
		{				
			pUserDM->GetCellData(_T("유저키"), i, strUserKey);
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(strUserKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("USER_CLASS")));		
			ShareInfoCenterUpload(_T("N"),_T("U"));
		}
	}	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeReserveCancel()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeReserveCancel");

	ids = SetResultScreen(60, _T("예약 만기일이 지난 예약 자료 예약 취소 작업 시작"));
	if(0 > ids) return ids;

	ids = m_pCheckLoan->GetAutoChangeReserveCancel();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(ids > 0) 
	{
		ids = SetResultScreen(80, _T("관리에 예약 자동 취소 안함으로 설정되있으므로 작업을 수행하지 않음"));
		if(0 > ids) return ids;

		return 1;
	}

	ids = SetResultScreen(90, _T("DB작업중"));
	if(0 > ids) return ids;

	ids = AllChangeReserveCancelDBProc();
	if(0 > ids) return ids;

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeReserveCancelDBProc()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeReserveCancelDBProc");
	CESL_DataMgr *pLoanDM;
	CString strManageValue;
	CString sUpdateSql;
		
	strManageValue = m_pManageValue->m_strLoanShare;
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{	
		pLoanDM = m_pParentMgr->FindDM(_T("DM_SHARE_LOAN_INFO_LIST"));	
		if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		if(m_pManageValue->m_sUnmannedReserveSys == _T("Y"))
		{
			sUpdateSql.Format(_T("RESERVATION_EXPIRE_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') AND STATUS='3' AND UNMANNED_RESERVATION_LOAN IS NULL AND MANAGE_CODE=UDF_MANAGE_CODE"));
		}
		else
		{
			sUpdateSql.Format(_T("RESERVATION_EXPIRE_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') AND STATUS='3' AND MANAGE_CODE=UDF_MANAGE_CODE"));
		}
		
		pLoanDM->RefreshDataManager(sUpdateSql);
	}	
	if(m_pManageValue->m_sUnmannedReserveSys == _T("Y")) 
	{
		sUpdateSql.Format( 
			_T("UPDATE LS_WORK_T01 ")
			_T("SET STATUS='4',RESERVE_CANCEL_DATE=SYSDATE,LAST_WORK='%s',RESERVATION_CANCEL_TYPE='0',R_DEVICE='KOLASIII' ")
			_T("WHERE RESERVATION_EXPIRE_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') AND STATUS='3' AND UNMANNED_RESERVATION_LOAN IS NULL AND MANAGE_CODE=UDF_MANAGE_CODE;")
			, m_pParentMgr->GetWorkLogMsg(_T("예약자동취소"),__FILE__,__LINE__));
	}
	else 
	{
		sUpdateSql.Format( 
			_T("UPDATE LS_WORK_T01 ")
			_T("SET STATUS='4',RESERVE_CANCEL_DATE=SYSDATE,LAST_WORK='%s',RESERVATION_CANCEL_TYPE='0',R_DEVICE='KOLASIII' ")
			_T("WHERE RESERVATION_EXPIRE_DATE < TO_DATE(TO_CHAR(SYSDATE,'YYYY/MM/DD'),'YYYY/MM/DD') AND STATUS='3' AND MANAGE_CODE=UDF_MANAGE_CODE;")
			, m_pParentMgr->GetWorkLogMsg(_T("예약자동취소"),__FILE__,__LINE__));
	}
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	pDM->StartFrame();
	pDM->AddFrame(sUpdateSql);	
	sUpdateSql.Empty();
	
	sUpdateSql.Format(	
		_T("UPDATE CO_LOAN_USER_TBL CLU1 ")
		_T("SET RESERVATION_COUNT=(SELECT COUNT(1) FROM LS_WORK_T01 LW WHERE CLU1.REC_KEY=LW.USER_KEY AND LW.STATUS='3') ")
		_T("WHERE REC_KEY IN (SELECT UU.REC_KEY FROM CO_LOAN_USER_TBL UU,(SELECT USER_KEY AS PK,COUNT(1) AS CNT FROM LS_WORK_T01 LW,CO_LOAN_USER_TBL CLU WHERE LW.USER_KEY=CLU.REC_KEY AND STATUS='3' GROUP BY USER_KEY) CNT_TBL WHERE UU.REC_KEY=CNT_TBL.PK AND CNT <> UU.RESERVATION_COUNT);"));
	pDM->AddFrame(sUpdateSql);
	ids = pDM->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = pDM->EndFrame();
	
	if(strManageValue == _T("1") || strManageValue == _T("2"))
	{
		INT i;
		INT nRowCount;
		CString	strLoanKey;
		nRowCount = pLoanDM->GetRowCount();
		for(i = 0; i < nRowCount; i++)
		{
			pLoanDM->GetCellData(_T("이력키"), i, strLoanKey);
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(strLoanKey));
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,RESERVE_CANCEL_DATE")));			
			ShareInfoCenterUpload(_T("U"),_T("N"));
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeReserveUpdate()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AllChangeReserveUpdate");

	ids = SetResultScreen(60, _T("예약 만기일이 지난 다음 예약 자료 예약 만기일 부여 작업 시작"));
	if(0 > ids) return ids;

	ids = m_pCheckLoan->GetAutoChangeReserveCancel();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(ids > 0) 
	{
		ids = SetResultScreen(80, _T("관리에 예약 자동 취소 안함으로 설정되있으므로 작업을 수행하지 않음"));
		if(0 > ids) return ids;

		return 1;
	}

	ids = SetResultScreen(90, _T("DB작업중"));
	if(0 > ids) return ids;

	ids = AllChangeReserveUpdateDBProc();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AllChangeReserveUpdateDBProc()
{
EFS_BEGIN	

	if(!(m_pManageValue->m_bVolReserve)) return 0;

	INT ids;	
	CString strFuncName = _T("AllChangeReserveUpdateDBProc");

	CString strMonoUpdateSql;
	CString strMonoUpdateSql2;
	CString strSerialUpdateSql;
	CString strSerialUpdateSql2;	
	
	strMonoUpdateSql.Format(
		_T("UPDATE LS_WORK_T01 A SET A.RESERVATION_EXPIRE_DATE = ")
		
		_T("TO_DATE(TO_CHAR(TO_DATE(ECO$F_G_ISHOLIDAY$I")
		
		_T("(")
		_T("  SYSDATE + (SELECT MIN(RESERVATION_EXPIRE_DAY) FROM CD_CODE_TBL ")
		_T("  WHERE CLASS=31 AND CODE = (SELECT B.USER_CLASS_CODE FROM CO_LOAN_USER_TBL B ")
		_T("                             WHERE B.REC_KEY=A.USER_KEY)), '%s' ")
		
		_T("), 'YYYY/MM/DD')||' 23:59:59'), 'YYYY/MM/DD HH24:MI:SS') ")
		
		_T("WHERE A.REC_KEY IN (SELECT MIN(CL.REC_KEY) FROM LS_WORK_T01 CL ")
		_T("                     WHERE CL.STATUS='3' AND CL.RESERVATION_EXPIRE_DATE is NULL AND ")
		_T("                           (SELECT BB.WORKING_STATUS FROM BO_BOOK_TBL BB ")
		_T("                             WHERE BB.REC_KEY=CL.BOOK_KEY AND BB.MANAGE_CODE = '%s')='BOL112N' ") 

		_T("                     GROUP BY CL.SPECIES_KEY, CL.VOL_INFO, CL.REG_NO ")
		_T("                  ) ")
		_T("AND 0 = (SELECT COUNT(1) FROM LS_WORK_T01 CL ")
		_T("          WHERE CL.STATUS='3' AND CL.RESERVATION_EXPIRE_DATE is NOT NULL ")
		_T("                AND A.SPECIES_KEY = CL.SPECIES_KEY AND A.VOL_INFO = CL.VOL_INFO ")
		_T("                AND (SELECT BB.WORKING_STATUS FROM BO_BOOK_TBL BB ")
		_T("                  WHERE BB.REC_KEY=CL.BOOK_KEY AND BB.MANAGE_CODE = '%s')='BOL112N' ")

		_T("                  GROUP BY CL.SPECIES_KEY, CL.VOL_INFO, CL.REG_NO ")
		_T("       ) ")
		_T("AND (SELECT B.USER_CLASS_CODE FROM CO_LOAN_USER_TBL B WHERE B.REC_KEY=A.USER_KEY) is NOT NULL; "),
		m_pParentMgr->m_pInfo->MANAGE_CODE, m_pParentMgr->m_pInfo->MANAGE_CODE, m_pParentMgr->m_pInfo->MANAGE_CODE);	
	
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;	

		if(m_pManageValue->m_bVolReserve) {
		strMonoUpdateSql2.Format(
			_T("UPDATE LS_WORK_T01 A SET A.REG_NO=")
			_T("(SELECT MIN(B.REG_NO) FROM BO_BOOK_TBL B ")
			_T("WHERE B.WORKING_STATUS='BOL112N' AND B.MANAGE_CODE='%s' ")
			_T("AND B.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ') AND B.SEPARATE_SHELF_CODE NOT IN ")
			_T("(SELECT CODE FROM CD_CODE_TBL WHERE CLASS=11 AND LOAN_YN='N') ")
			_T("AND SUBSTR(B.REG_NO,1,%d) IN ")
			_T("(SELECT CODE FROM CD_CODE_TBL WHERE CLASS=4 AND LOAN_YN='Y') ")
			_T("AND B.SHELF_LOC_CODE IN ")
			_T("(SELECT CODE FROM CD_CODE_TBL WHERE CLASS=19 AND LOAN_YN='Y') ")
			_T("AND B.SPECIES_KEY=A.SPECIES_KEY AND NVL(B.VOL_SORT_NO,-100)=NVL(A.VOL_INFO,-100)) ")
			_T("WHERE A.STATUS='3' AND A.RESERVATION_EXPIRE_DATE IS NOT NULL AND REG_NO IS NULL;") 
			, nRegCodeLength, m_pParentMgr->m_pInfo->MANAGE_CODE);
	}
	
	strSerialUpdateSql.Format(
		_T("UPDATE LS_WORK_T01 A ")
		_T("SET A.RESERVATION_EXPIRE_DATE = ")
		_T("TO_DATE(TO_CHAR(TO_DATE(ECO$F_G_ISHOLIDAY$I ")
		_T("(SYSDATE + ")
		_T("(SELECT MIN(RESERVATION_EXPIRE_DAY) FROM CD_CODE_TBL ")
		_T("WHERE CLASS=31 AND CODE = (SELECT B.USER_CLASS_CODE FROM CO_LOAN_USER_TBL B WHERE B.REC_KEY=A.USER_KEY) ")
		_T("), '%s'), 'YYYY/MM/DD')||' 23:59:59'), 'YYYY/MM/DD HH24:MI:SS') ")
		_T("WHERE A.REC_KEY IN (SELECT MIN(CL.REC_KEY) FROM LS_WORK_T01 CL ")
		_T("            WHERE CL.STATUS='3' ")
		_T("                  AND CL.RESERVATION_EXPIRE_DATE IS NULL ")
		_T("                  AND (SELECT BB.WORKING_STATUS FROM SE_BOOK_TBL BB WHERE BB.REC_KEY=CL.BOOK_KEY AND BB.MANAGE_CODE='%s') IN ('SEL212N','SEL112N') ")
		_T("            GROUP BY CL.SPECIES_KEY, CL.VOL_INFO, CL.REG_NO) ")
		_T("AND      0 = (SELECT COUNT(1) FROM LS_WORK_T01 CL ")
		_T("            WHERE CL.STATUS='3' ")
		_T("                  AND CL.RESERVATION_EXPIRE_DATE IS NOT NULL ")
		_T("                AND A.SPECIES_KEY = CL.SPECIES_KEY AND A.VOL_INFO = CL.VOL_INFO ")
		_T("                  AND (SELECT BB.WORKING_STATUS FROM SE_BOOK_TBL BB WHERE BB.REC_KEY=CL.BOOK_KEY AND BB.MANAGE_CODE='%s') IN ('SEL212N','SEL112N') ")
		_T("            GROUP BY CL.SPECIES_KEY, CL.VOL_INFO, CL.REG_NO) ")
		_T("AND (SELECT B.USER_CLASS_CODE FROM CO_LOAN_USER_TBL B WHERE B.REC_KEY=A.USER_KEY) is NOT NULL;"),
		m_pParentMgr->m_pInfo->MANAGE_CODE, m_pParentMgr->m_pInfo->MANAGE_CODE, m_pParentMgr->m_pInfo->MANAGE_CODE);

	if(m_pManageValue->m_bVolReserve)
	{
		strSerialUpdateSql2.Format(
			_T("UPDATE LS_WORK_T01 A SET ")
			_T("A.REG_NO = (SELECT MIN(B.REG_NO) ")
			_T("FROM SE_BOOK_TBL B, SE_VOL_TBL C ")
			_T("WHERE B.VOL_KEY=C.REC_KEY AND B.MANAGE_CODE = '%s' AND ")

			_T("B.WORKING_STATUS in ('SEL212N','SEL112N') AND ")
			_T("B.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ') AND ")
			_T("B.SEPARATE_SHELF_CODE NOT IN (SELECT CODE FROM CD_CODE_TBL WHERE CLASS = 11 AND LOAN_YN='N') AND ")
			_T("SUBSTR(B.REG_NO,1,%d) IN (SELECT CODE FROM CD_CODE_TBL WHERE CLASS = 4 AND LOAN_YN='Y') AND ")
			_T("B.SHELF_LOC_CODE IN (SELECT CODE FROM CD_CODE_TBL WHERE CLASS = 19 AND LOAN_YN='Y') AND ")
			_T("B.SPECIES_KEY=A.SPECIES_KEY AND ")
			_T("NVL(C.VOL_SORT_NO,-100)=NVL(A.VOL_INFO,-100)) ")
			_T("WHERE A.STATUS='3' AND A.RESERVATION_EXPIRE_DATE IS NOT NULL AND REG_NO IS NULL;")
			, nRegCodeLength, m_pParentMgr->m_pInfo->MANAGE_CODE);
	}
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM("DM_BO_LOC_3100_DB");
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pDM->InitDBFieldData();
	pDM->StartFrame();
	pDM->AddFrame(strMonoUpdateSql);
	if(m_pManageValue->m_bVolReserve) pDM->AddFrame(strMonoUpdateSql2);
	pDM->AddFrame(strSerialUpdateSql);
	if(m_pManageValue->m_bVolReserve) pDM->AddFrame(strSerialUpdateSql2);
	ids = pDM->SendFrame();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	ids = pDM->EndFrame();
	
	return 0;

EFS_END
return -1;
}


BOOL CBL_LOC_LOAN_PROC::GetThreadState()
{
EFS_BEGIN

	return m_bStop;

EFS_END
return FALSE;
}


INT CBL_LOC_LOAN_PROC::SetStopThread()
{
EFS_BEGIN

	m_bBreak = TRUE;
	return 0;	

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetThreadEndState()
{
EFS_BEGIN

	return m_bStopState;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ChangeMember()
{
EFS_BEGIN

	if(m_sMemberKey.IsEmpty()) return 0;

	INT ids;
	
	ids = SearchLoanInfo(m_sMemberKey, _T(""));
	if(0 > ids)	return ids;
	
	m_sMemberKey.Empty();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SetChangeMemberKey(CString sMemberKey)
{
EFS_BEGIN

	m_sMemberKey = sMemberKey;
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewDetail()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ViewDetail");

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_DataMgr *pParentDM = m_pParentMgr->FindDM(sDMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(pParentDM->GetRowCount() < 1) return 0;

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sPublishForm;

	ids = pParentDM->GetCellData(_T("단행/연속구분"), pGrid->GetIdx(), sPublishForm);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -5, strFuncName);

	if(sPublishForm.Compare(_T("MO")) != 0 && (sPublishForm.Compare(_T("SE")) != 0)) return 0;

	CString sSpeciesKey;
	CStringList sSpeciesKeyList;
	CArray < INT, INT > asGridRow;

	INT nIndex = -1;
	nIndex = pGrid->GetIdx();
	pParentDM->GetCellData(_T("종KEY"), nIndex, sSpeciesKey);
	sSpeciesKeyList.AddTail(sSpeciesKey);
	asGridRow.Add(nIndex);

	if(sPublishForm.Compare(_T("MO")) == 0)
	{
		CBOBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList(sSpeciesKeyList, pGrid, asGridRow);
		Dlg.DoModal();
	}
	else
	{
		CSEBookDetailViewerDlg Dlg(m_pParentMgr);
		
		Dlg.SetPKList(sSpeciesKeyList, pGrid, asGridRow);
		Dlg.DoModal();
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::ReLoadMemberInfo()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReLoadMemberInfo");

	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pUserDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sLoanUserKey;
	ids = pUserDM->GetCellData(_T("대출자KEY"), 0, sLoanUserKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CString sWhere;
	sWhere.Format(_T("CLU.REC_KEY = %s "), sLoanUserKey);
	ids = SearchMemberInfo(sWhere);
	if(0 > ids) return ids;
	
	ids = AddMemberInfo();
	if(0 > ids) return ids;
	
	ids = AllControlDisplayNotGrid();
	if(0 > ids) return ids;

	ids = ViewSimpleFamilyMember();
	if(0 > ids) return ids;
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SelfCloseMessageBox(CString sMsg, CESL_Mgr *pParentMgr/*=NULL*/, INT nDelayTime/*=1000*/)
{
EFS_BEGIN		

	if(FALSE == m_bShowMessage)
	{
		return 0;
	}
	if(NULL == pParentMgr)
	{
		if(NULL == m_pParentMgr)
		{
			return 0;
		}
		pParentMgr = m_pParentMgr;
	}
	INT nMode=0;
	if(sMsg.Find(_T("\n")) >= 0)
	{
		nMode = 1;
	}
	else if(sMsg.GetLength() > 30)
	{
		nMode = 1;
	}
	else
	{
		nMode = 0;
	}
	if(m_nMessageLoop == 3) 
	{
		m_nMessageLoop = 0;
	}
	if(m_pMessageDlg[m_nMessageLoop] == NULL)
	{
		m_pMessageDlg[m_nMessageLoop] = new CMessageDlg(pParentMgr);		
	}
	for(int i = 0; i < 3; i++)
	{
		if(m_pMessageDlg[i] != NULL)
		{
			if(m_pMessageDlg[i]->m_sMsg == sMsg)
			{
				m_nMessageLoop = i;
				break;
			}
		}
	}
	if(m_pMessageDlg[m_nMessageLoop] != NULL)
	{
		m_pMessageDlg[m_nMessageLoop]->DestroyWindow();
		delete m_pMessageDlg[m_nMessageLoop];
		m_pMessageDlg[m_nMessageLoop] = NULL;
		m_pMessageDlg[m_nMessageLoop] = new CMessageDlg(pParentMgr);
	}
	m_pMessageDlg[m_nMessageLoop]->m_nDelayTime = nDelayTime;
	m_pMessageDlg[m_nMessageLoop]->Create(AfxGetMainWnd());
	m_pMessageDlg[m_nMessageLoop]->SetMsg(sMsg, nMode);
	m_pMessageDlg[m_nMessageLoop]->CenterWindow(pParentMgr);	
	
	CRect WndPos;
	m_pMessageDlg[m_nMessageLoop]->GetWindowRect(WndPos);
	m_pMessageDlg[m_nMessageLoop]->MoveWindow(WndPos.left, WndPos.top + (WndPos.Height()*m_nMessageLoop), WndPos.Width(), WndPos.Height());
	InputMessageBox(sMsg);
	m_pMessageDlg[m_nMessageLoop]->ShowWindow(SW_SHOWNOACTIVATE);
	m_nMessageLoop++;

	if(m_bBeep == TRUE) 
	{
		if(sMsg == _T("대출되었습니다."))
		{
			AfxBeginThread(RunLoanBeep, this);			
		}
		else if(sMsg == _T("반납이 완료되었습니다."))
		{
			AfxBeginThread(RunReturnBeep, this);						
		}
		else if(sMsg == _T("대출자를 먼저 선택해주십시오") ||
			      sMsg == _T("검색된 이용자는 대출정지회원입니다.") ||
				  sMsg == _T("회원 정보를 조회하는데 실패했습니다.") ||
				  sMsg == _T("존재하지 않는 이용자입니다.") ||
				  sMsg == _T("분실된 회원카드 입니다.") ||
				  sMsg == _T("검색된 이용자는 탈퇴신청회원입니다.") ||
				  sMsg == _T("정상 회원만이 대출이 가능합니다.") ||
				  sMsg == _T("회원증분실 회원은 대출이 불가능 합니다.") ||
				  sMsg == _T("대출 정지일이 지나지 않았으므로 대출이 불가능합니다.") ||
				  sMsg == _T("비회원은 대출이 제한됩니다.") ||
				  sMsg == _T("타 도서관 대출정지 회원입니다.") ||
				  sMsg == _T("타 도서관 제적회원입니다.") ||
				  sMsg == _T("대출자를 먼저 선택해주십시오") ||
				  sMsg == _T("대출 한도를 초과하여 대출할 수 없습니다.") ||
				  sMsg == _T("통합대출가능권수 한도를 초과하여 대출할 수 없습니다.") ||
				  sMsg == _T("도서관 대출가능권수 한도를 초과하여 대출할 수 없습니다.") ||
				  sMsg == _T("대출할 책을 선택해 주십시오") )
		{
			AfxBeginThread(RunErrorBeep, this);
		}
		
	}
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetDelayDay(INT nIndex, INT &nDelayDay)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetDelayDay");

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sDelayDay;
	CString sReturnExpDate;

	ids = pDM->GetCellData(_T("반납예정일"), nIndex, sReturnExpDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(sReturnExpDate.GetLength() < 9) return 0;

	
	
	CString sReturnType;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	CString sReturnDate;
	CString sReturnDateControlAlias = _T("반납일");
	if(sReturnType.Compare(_T("2"))== 0)
		sReturnDateControlAlias = _T("소급반납일");

	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, sReturnDate);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CTime tCurrent = CTime(_ttoi(sReturnDate.Mid(0,4)), _ttoi(sReturnDate.Mid(5,2)), _ttoi(sReturnDate.Mid(8,2)), 0, 0, 0);
	
	CTime tReturnExpDate(_ttoi(sReturnExpDate.Mid(0,4)), _ttoi(sReturnExpDate.Mid(5,2)), _ttoi(sReturnExpDate.Mid(8,2)), 0, 0, 0);

	CTimeSpan tDelayDay = tCurrent - tReturnExpDate;

	nDelayDay =  tDelayDay.GetDays();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::GetReservationExpireDay(INT &nExpireDay)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetReservationExpireDay");
	
	CString sPositionCode;

	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자직급"), sPositionCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = m_pCheckLoan->GetReservationExpireDay(sPositionCode, nExpireDay);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	
	ids = m_pCheckLoan->GetAutoSetReserveExpireDay();
	if(ids ==  0) return 0;

	CInputReservatoinExpireDay InputDlg(m_pParentMgr);
	InputDlg.SetDay(nExpireDay);
	
	InputDlg.DoModal();

	InputDlg.GetDay(nExpireDay);

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::MakeReservationExpireDate(INT nExpireDay, CString &sExpireDate)
{
EFS_BEGIN

	
	CString strGetData;
	CTime tExpireDate = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);

	CTimeSpan tExpireDay(nExpireDay, 0, 0, 0);
	
	tExpireDate += tExpireDay;

	sExpireDate = tExpireDate.Format(_T("%Y/%m/%d"));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchUserBook(INT nEntryPoint, CString &sRegNo, CString &sPublishForm)
{
EFS_BEGIN

	INT ids;

	CString strFuncName = _T("SearchUserBook");
	DWORD nSTART, nFINISH;
	nSTART = GetTickCount();

	CString sBoWhere,sSeWhere;
	ids = GetSqlSearchBookInfo(sBoWhere, 0);	
	if(0 != ids) return ids;
	
	ids = GetSqlSearchBookInfo(sSeWhere, 1);	
	if(0 != ids) return ids;
	
	ids = SendSearchBookInfoSQL(sBoWhere, sSeWhere);
	if(0 != ids) return ids;
	
	nFINISH = GetTickCount();
	TRACE(_T("(Finish) - 이용자 자료찾기 총 메모리 구성하는 시간 : [%d.%d sec]\n"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
	
	ids = ViewSerachBookList(nEntryPoint,sRegNo,sPublishForm);
	if(0 != ids) return ids;
	
	return ids;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::MoveRegNoToNextProcBasket(CString sMode,BOOL bIsRFID)
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("MoveRegNoToNextProcBasket");

	CString sRegNo;
	CString sBasketDMAlias = _T("DM_BO_LOC_3100_BASKET");
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	INT nInsertIndex;

	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_INFO"));
	if(!pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Control_Element* pCEB = pCM->FindControlElement(_T("처리할자료"));
	if(!pCEB) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CButton* pExpDataButton = (CButton*)(m_pParentMgr->GetDlgItem(pCEB->CTRL_ID));
	if(!pExpDataButton) return 0;

	if((pExpDataButton->GetCheck() != 1) && !bIsRFID) return 0;
	
	if(_T("L") == sMode)
	{
		
		MakeRegNoSpaceToZero(FALSE);
	}
	else
	{		
	MakeRegNoSpaceToZero(TRUE);
	}
	
	ids = IsRegNo(sRegNo);
	if(0 > ids) return ids;
	
	if(sRegNo.IsEmpty()) return 0;
	
	CESL_DataMgr *pBasketDM = m_pParentMgr->FindDM(sBasketDMAlias);
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	pBasketDM->InsertRow(-1);

	nInsertIndex = pBasketDM->GetRowCount()-1;

	ids = pBasketDM->SetCellData(_T("등록번호"), sRegNo, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pBasketDM->SetCellData(_T("예상처리"), sMode, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	CESL_Control_Element* pCE = pCM->FindControlElement(_T("바구니"));
	if(NULL == pCE) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	CComboBox* pCombo = NULL;
	CESL_Mgr* pParentMgr;
	pParentMgr = m_pParentMgr;
	while(pParentMgr != NULL)
	{
		pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
		pParentMgr = pParentMgr->pParentMgr;
	}
	
	if(NULL == pCombo) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sTmpRegNo;
	BOOL bIsInsert = TRUE;
	for(INT i = 0 ; i < pCombo->GetCount() ; i++)
	{
		pCombo->GetLBText(i, sTmpRegNo);
		if(sTmpRegNo.Compare(sRegNo) == 0) bIsInsert = FALSE;
	}

	if(bIsInsert)
		pCombo->AddString(sRegNo);

	pCombo->SetCurSel(pCombo->GetCount()-1);

	m_pParentMgr->SetControlData(sCMAlias, _T("대출자번호"), _T(""));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BasketRegNoProc()
{
EFS_BEGIN

	INT ids, i = 0, j = 0;
	CString strFuncName = _T("BasketRegNoProc");

	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sBasketDMAlias = _T("DM_BO_LOC_3100_BASKET");

	
	CESL_DataMgr *pBasketDM = m_pParentMgr->FindDM(sBasketDMAlias);
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	
	CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanInfoDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
	CESL_ControlMgr *pLoanCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)(pLoanCM->FindControl(_T("MainGrid")));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -37, strFuncName);

	CString strBookKey;
	CString sRegNo, sMode;	
	CString sSerialNo;
	CString strBookLibCode;
	for(i = 0; i < pBasketDM->GetRowCount(); i++)
	{
		ids = pBasketDM->GetCellData(_T("책키"), i, strBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pBasketDM->GetCellData(_T("등록번호"), i, sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pBasketDM->GetCellData(_T("예상처리"), i, sMode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pCM->SetControlMgrData(_T("대출자번호"), sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pBasketDM->GetCellData(_T("소장관도서관부호"), i, strBookLibCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		
		if(EXP_RESERVELOAN == sMode)
		{
			
			ids = BarCodeReturnBookCheck(strBookKey);

			if(0 > ids) return ids;
			if(2 == ids)
			{
				sMode = EXP_LOAN;
			}
		}
		
		if(EXP_RESERVELOAN == sMode)
		{
			InitExpDM();
			ids = SPFExcutePath(_T("예약대출"));
		}
		
		else if(EXP_LOAN == sMode)
		{
			InitExpDM();
			ids = SPFExcutePath(_T("대출"));
		}
		
		else if(EXP_RESERVE == sMode)
		{
			InitExpDM();
			SPFExcutePath(_T("예약"));
			continue;
		}
	
		else if(_T("L") == sMode)
		{
			
			InitExpDM();

			CString strSQL;
			CString strTransNo, strBookManageCode, strRegNo, strSpeciesKey;
			BOOL bIsAppendixLoan = FALSE;

			ids = pBasketDM->GetCellData(_T("책두레_트랜잭션번호"), i, strTransNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

			CESL_DataMgr* pILLDM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_ILL_BOOK_INFO"));
			strSQL.Format(	_T("SELECT ILL.TRANS_NO, (SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = ILL.GIVE_LIBCODE) AS GIVE_MANAGE_CODE, ILL.REG_NO, ILL.BOOK_APPENDIX_FLAG, ILL.SPECIES_KEY ")
							_T("FROM ILL_TRANS_TBL ILL ")
							_T("WHERE ILL.STATE = '0014' AND ILL.LOAN_KEY IS NULL AND ILL.SPECIES_KEY = (SELECT SPECIES_KEY FROM ILL_TRANS_TBL WHERE TRANS_NO = '%s') ORDER BY LENGTH(TRANS_NO), TRANS_NO")
							, strTransNo);
			TmpDM.RestructDataManager(strSQL);

			for(j = 0; j < TmpDM.GetRowCount(); j++){
				if(strTransNo == TmpDM.GetCellData(j, 0)){
					strSpeciesKey = TmpDM.GetCellData(j, 4);
				}
				if(_T("Y") == m_pManageValue->m_sIsLoanAppendix){
					if(strSpeciesKey == TmpDM.GetCellData(j, 4) && TmpDM.GetCellData(j, 3) == _T("A")){
						bIsAppendixLoan = m_pParentMgr->MessageBox(_T("대출하려는 책과 같이 입수된 부록자료가 존재합니다.\r\n같이 대출을 하시겠습니까?"), _T("부록자료대출"), MB_ICONQUESTION | MB_YESNO);
						break;
					}
				}
			}
			for(j = 0; j < TmpDM.GetRowCount(); j++){
				if(strSpeciesKey == TmpDM.GetCellData(j, 4) && strSpeciesKey.IsEmpty() == FALSE){
					
					if(bIsAppendixLoan == FALSE && TmpDM.GetCellData(j, 3) == _T("A")){
						break;
					}
					pILLDM->FreeData();
					pILLDM->InsertRow(-1);
					pILLDM->SetCellData(_T("트랜잭션번호"), TmpDM.GetCellData(j, 0), 0);
					pILLDM->SetCellData(_T("관리구분_책"), TmpDM.GetCellData(j, 1), 0);
					pILLDM->SetCellData(_T("등록번호"), TmpDM.GetCellData(j, 2), 0);
					
					m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), TmpDM.GetCellData(j, 2));

					SPFExcutePath(_T("책두레대출"));
				}
			}

		}

		if(0 < ids || 0 > ids) 
		{
			continue;
		}
		
		ids = pBasketDM->GetCellData(_T("시리얼"), i, sSerialNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
		if(sSerialNo.IsEmpty())
		{
			continue;
		}
		
		ids = RFID_UpdateLoan(0, sSerialNo);			
		if(0 > ids) {
			CString sEMsg;
			sEMsg.Format(_T("등록번호['%s'] : RFID 정보 입력에 실패하였습니다."), sRegNo);
			SelfCloseMessageBox(sEMsg);
		}	
		
		CString sLoanType;
		ids = GetLoanType(sLoanType);
		if(0 > ids) return ids;
	}
	pBasketDM->FreeData();

	ids = ClearBasket();
	if(0 > ids) return ids;

	m_pParentMgr->SetControlData(sCMAlias, _T("대출자번호"), _T(""));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ClearBasket()
{
EFS_BEGIN

	CString strFuncName = _T("ClearBasket");

	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(sCMAlias);
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Control_Element* pCE = pCM->FindControlElement(_T("바구니"));
	if(pCE == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
	CComboBox* pCombo = NULL;
	CESL_Mgr* pParentMgr;
	pParentMgr = m_pParentMgr;
	while(pParentMgr != NULL)
	{
		pCombo = (CComboBox*)(pParentMgr->GetDlgItem(pCE->CTRL_ID));
		pParentMgr = pParentMgr->pParentMgr;
	}
	
	if(pCombo == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT nCnt = pCombo->GetCount();
	for(INT i = nCnt-1 ; i > -1 ; i--)
		pCombo->DeleteString(i);

	pCombo->Clear();

	pCombo->SetCurSel(0);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewLoanHistory(INT nMode, INT nEntryPoint)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ViewLoanHistory");

	CString sUserDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CString sExpLoanDMAlias = _T("DM_EXP_LOAN_BOOK_INFO");
	
	CESL_DataMgr *pExpLoanDM = m_pParentMgr->FindDM(sExpLoanDMAlias);
	if(NULL == pExpLoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd,  -1000, strFuncName);
	
	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM(sUserDMAlias);
	if(NULL == pUserDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd,  -1000, strFuncName);

	CString sBookRecKey;
	CString sUserRecKey;
	CString sFamilyKey;

	ids = pExpLoanDM->GetCellData(_T("책KEY"), 0, sBookRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 
	
	ids = pUserDM->GetCellData(_T("대출자KEY"), 0, sUserRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 

	ids = pUserDM->GetCellData(_T("가족KEY"), 0, sFamilyKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName); 

	CLoanHistory Dlg(m_pParentMgr);
	Dlg.SetInitData(nMode, nEntryPoint, sUserRecKey, sFamilyKey, sBookRecKey , m_pManageValue->m_sIsUnityLoanService );
	Dlg.DoModal();

	return Dlg.GetCloseMode();

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::TestCode()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("TestCode");

	CESL_DataMgr* pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanInfoDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(sMemberDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_MEMBER_INFO"));	
	
	if(pDM->GetRowCount() == 0) return 0;

	INT nLoanCnt = 0; 
	INT nUnityLoanCnt = 0; 
	INT nOtherLoanCnt = 0; 
	INT nReserveCnt = 0;

	INT nDMCnt = pLoanInfoDM->GetRowCount();

	CString strUserManageCode = _T("");
	ids = m_pParentMgr->GetDataMgrData(sMemberDMAlias, _T("관리구분"), strUserManageCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sStatus,sAppendixYN;
	CString sManageCode = _T("");
	for(INT i = 0 ; i < pLoanInfoDM->GetRowCount() ; i++)
	{
		ids = pLoanInfoDM->GetCellData(_T("대출상태"), i, sStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanInfoDM->GetCellData(_T("부록여부"), i, sAppendixYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanInfoDM->GetCellData(_T("관리구분"), i, sManageCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(sStatus.Compare(_T("0")) == 0 || sStatus.Compare(_T("2")) == 0)
		{
			if(sAppendixYN.Compare(_T("A")) == 0)
			{
				if(m_pCheckLoan->IsAppendixIncludeLoanCnt() == 0)
				{
					nUnityLoanCnt++;
				}
			}
			else
				nUnityLoanCnt++;	
		}
		
		if(sStatus.Compare(_T("3")) == 0)
		{
			CString sValue;
			ids = pLoanInfoDM->GetCellData(_T("무인대출예약상태"), i, sValue);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			if(sValue.IsEmpty())
			{
				nReserveCnt++;
			}
		}
		
		if((_T("0") == sStatus || _T("2") == sStatus) 
 			&& strUserManageCode == sManageCode)
		{
			if(sAppendixYN.Compare(_T("A")) == 0) {
				if(m_pCheckLoan->IsAppendixIncludeLoanCnt() == 0)
					nLoanCnt++;
			}
			else {
				nLoanCnt++;	
			}
		}
	}
	
	CString sUserKey, sLoanBookCnt;
	CString sDBLoanBookCnt, sTotalLoanBookCnt, sCanLoanBookCnt;
	
	sDBLoanBookCnt.Format(_T("%d"), nUnityLoanCnt);

	pDM->GetCellData(_T("대출자KEY"), 0, sUserKey);
	pDM->GetCellData(_T("총대출책수"), 0, sLoanBookCnt); 

	if(_ttoi(sLoanBookCnt) != _ttoi(sDBLoanBookCnt))
	{
		if(!(m_pManageValue->m_bVolReserve))
		{
			pDM->StartFrame();
			pDM->InitDBFieldData();
			
			if(1 > _ttoi(sDBLoanBookCnt))
			{
				sDBLoanBookCnt = _T("0");
			}
			pDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sDBLoanBookCnt, TRUE);
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("대출책수"), __WFILE__, __LINE__), TRUE);
			pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
			ids = pDM->SendFrame();
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			pDM->EndFrame();
		}
		sLoanBookCnt.Format(_T("%d"), nLoanCnt);
		pDM->SetCellData(_T("총대출책수"), sLoanBookCnt, 0);
		pDM->GetCellData(_T("대출책수상태"), 0, sTotalLoanBookCnt);
		sTotalLoanBookCnt = sTotalLoanBookCnt.Mid(sTotalLoanBookCnt.Find(_T("/"))+1);
		sCanLoanBookCnt.Format(_T("%d"), _ttoi(sTotalLoanBookCnt) - nLoanCnt);
		sLoanBookCnt += _T("/") + sTotalLoanBookCnt;
		pDM->SetCellData(_T("대출가능책수"), sCanLoanBookCnt, 0);
		pDM->SetCellData(_T("대출책수상태"), sLoanBookCnt, 0);
		pCM->SetControlMgrData(_T("대출가능책수"), sLoanBookCnt);
	}

	CString sReserveBookCnt, sDBReserveBookCnt, sTotalReservationBookCnt, sCanReservationBookCnt ;
	sDBReserveBookCnt.Format(_T("%d"), nReserveCnt);

	pDM->GetCellData(_T("총예약책수"), 0, sReserveBookCnt);

	if(_ttoi(sReserveBookCnt) != _ttoi(sDBReserveBookCnt))
	{
		if(!(m_pManageValue->m_bVolReserve))
		{
			pDM->StartFrame();
			pDM->InitDBFieldData();
			pDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sDBReserveBookCnt, TRUE);
			pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약책수"), __WFILE__, __LINE__), TRUE);
			pDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
			pDM->SendFrame();
			pDM->EndFrame();
		}
		pDM->SetCellData(_T("총예약책수"), sDBReserveBookCnt, 0);	
		
		pDM->GetCellData(_T("예약책수상태"), 0, sTotalReservationBookCnt);
		sTotalReservationBookCnt = sTotalReservationBookCnt.Mid(sTotalReservationBookCnt.Find(_T("/"))+1);
		sCanReservationBookCnt.Format(_T("%d"), _ttoi(sTotalReservationBookCnt) - _ttoi(sDBReserveBookCnt));
		sDBReserveBookCnt += _T("/") + sTotalReservationBookCnt;
		pDM->SetCellData(_T("예약가능책수"), sCanReservationBookCnt, 0);
		pDM->SetCellData(_T("예약책수상태"), sDBReserveBookCnt, 0);
		
		pCM->SetControlMgrData(_T("예약가능책수"), sDBReserveBookCnt);
	}

	return 0;	

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InputMessageBox(CString sMsg)
{
EFS_BEGIN

	return 0;

	CTime tTime = CTime::GetCurrentTime();
	
	CString sTime;
	sTime = tTime.Format(_T("%H시 %M분 %S초"));

	CESL_DataMgr* pMessageDM = m_pParentMgr->FindDM(_T("DM_메시지박스"));
	if(pMessageDM == NULL)	return 0;

	if(pMessageDM->GetRowCount() == 1000)
	{
		pMessageDM->FreeData();
	}

	pMessageDM->InsertRow(-1);

	INT nInsertRow = pMessageDM->GetRowCount() - 1;

	pMessageDM->SetCellData(_T("시간"), sTime, nInsertRow);
	pMessageDM->SetCellData(_T("메시지"), sMsg, nInsertRow);
	
	m_pParentMgr->SendMessage(WM_INSERT_MESSAGE_BOX);

	if(m_pParentMgr->pParentMgr != NULL)
	{
		m_pParentMgr->pParentMgr->SendMessage(WM_INSERT_MESSAGE_BOX);
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ShareInfoCenterUpload(CString sLoanInfoType, CString sUserInfoType, INT nMode/*=0*/)
{
EFS_BEGIN

	CString sDMAlias = _T("DM_BO_LOC_3100_DB");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);	
	
	UPLOAD_SHARE_INFO UploadShareInfo;
	UploadShareInfo.nType = 0;

	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("51");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("LH_STORAGE_V01");
	
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_pParentMgr->m_pInfo->LIB_CODE;

	
	if(sLoanInfoType.Compare(_T("I")) == 0)
	{
		CString sRecKey;
		GetTempData(SHARE_CO_LOAN_TBL_INSERT_REC_KEY, sRecKey);
		
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	else if(sLoanInfoType.Compare(_T("U")) == 0)
	{
		CString sRecKey;
		CString sUpdateFields;
		GetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY , sRecKey);
		GetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS , sUpdateFields);
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;				
	}
	else if(sLoanInfoType.Compare(_T("D")) == 0 )
	{
		CString sRecKey;
		GetTempData(SHARE_CO_LOAN_TBL_DELETE_REC_KEY , sRecKey);
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}

	if(sLoanInfoType.Compare(_T("N")) != 0)
		m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo, nMode);
	
	
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
	{
		return 0;
	}

	
	UploadShareInfo.UploadShareLoanInfo.sCommand = _T("11");
	UploadShareInfo.UploadShareLoanInfo.sUpdatedTableName = _T("CO_LOAN_USER_TBL");
	UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("Y");
	UploadShareInfo.UploadShareLoanInfo.sLibCode = m_strLibCode;	
	
	if(sUserInfoType.Compare(_T("I")) == 0)
	{
		CString sRecKey;
		GetTempData(SHARE_CO_LOAN_USER_TBL_INSERT_REC_KEY, sRecKey);

		UploadShareInfo.UploadShareLoanInfo.sType = _T("I");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}
	else if(sUserInfoType.Compare(_T("U")) == 0)
	{
		CString sRecKey;
		CString sUpdateFields;
		GetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY , sRecKey);
		GetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS , sUpdateFields);
				
		UploadShareInfo.UploadShareLoanInfo.sType = _T("U");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		UploadShareInfo.UploadShareLoanInfo.sUpdatedFields = sUpdateFields;		
	}
	else if(sUserInfoType.Compare(_T("D")) == 0)
	{
		CString sRecKey;
		GetTempData(SHARE_CO_LOAN_USER_TBL_DELETE_REC_KEY , sRecKey);
		
		UploadShareInfo.UploadShareLoanInfo.sType = _T("D");
		
		UploadShareInfo.UploadShareLoanInfo.sUpdatedTableRecKey = sRecKey;
		
	}

	
	
	if(UploadShareInfo.UploadShareLoanInfo.sUpdatedFields.Find(_T("USER_CLASS")) >= 0 ||
		 UploadShareInfo.UploadShareLoanInfo.sUpdatedFields.Find(_T("LOAN_STOP_DATE")) >= 0)
	{
		UploadShareInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN = _T("N");
	}

	if(sUserInfoType.Compare(_T("N")) != 0)
		m_pLoanShareManager->SendLoanInfoUpload(UploadShareInfo);
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SearchUserBookFromKey(CString sUserKey, BOOL bMsgShow/*=TRUE*/, BOOL bPasswd/*=FALSE*/)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SearchUserBookFromKey");

	ids = SearchLoanInfo(sUserKey, _T(""), bPasswd);
	if(0 > ids) return ids;
	if(ids == 1)
	{
		SelfCloseMessageBox(_T("존재하지 않는 이용자입니다."));
		
		SPFExcutePath(_T("화면초기화"));
		return 1;		
	}
	
	if(ids == 2)
	{
		
		SPFExcutePath(_T("화면초기화"));
		return 1;
	}

	Send3000ApiSocket(_T("회원_검색"), sUserKey);
	
	CString sMemberState;
	CString sLostUserCard;
	CString sMemberDMAlias = _T("DM_BO_LOC_3100_MEMBER_INFO");
	CESL_DataMgr *pTmpDM = m_pParentMgr->FindDM(sMemberDMAlias);
	ids = pTmpDM->GetCellData(_T("회원상태"), 0, sMemberState);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(TRUE == bMsgShow)
	{		
		ids = pTmpDM->GetCellData(_T("회원증분실"), 0, sLostUserCard);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		if(sMemberState == _T("1"))
		{
			if(sLostUserCard == _T("Y"))
			{
				SelfCloseMessageBox(_T("분실된 회원카드 입니다."), m_pParentMgr);
			}
			else
			{
				SelfCloseMessageBox(_T("검색된 이용자는 대출정지회원입니다."), m_pParentMgr);
			}
		}
		
		else if(sMemberState == _T("3"))
		{
			SelfCloseMessageBox(_T("검색된 이용자는 탈퇴신청회원입니다."), m_pParentMgr);
		}
		
		else
		{
			if(sLostUserCard == _T("Y"))
			{
				SelfCloseMessageBox(_T("분실된 회원카드 입니다."), m_pParentMgr);
			}
			else
			{
				SelfCloseMessageBox(_T("이용자가 검색되었습니다."), m_pParentMgr, 2000);
			}
		}
		
		
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddLoanInfoGrid()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("AddLoanInfoGrid");

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	ids = m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM, _T("자료실구분"), _T("자료실"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CESL_Grid* pGrid = (CESL_Grid*)(m_pParentMgr->FindControl(sCMAlias, _T("MainGrid")));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	INT nInsertIndex = pDM->GetRowCount() - 1;

	
	CString strGetData;
	CTime tCurrent = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);

	CString sData, sDesc;
	
	ids = pDM->GetCellData(_T("관리구분"), nInsertIndex, sData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	m_pParentMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("관리구분"), sData, sDesc);
	ids = pDM->SetCellData(_T("관리구분설명"), sDesc, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	sData.Empty();		sDesc.Empty();
	ids = pDM->GetCellData(_T("단행/연속구분"), nInsertIndex, sData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	if(_T("MO") == sData)
	{
		sDesc = _T("단행");
	}
	else if(_T("SE") == sData)
	{
		sDesc = _T("연속");
	}
	else
	{
		
		CString sData2;
		pDM->GetCellData(_T("타관대출KEY"),nInsertIndex, sData2);
		if(0 != sData2.GetLength())
		{
			sDesc = _T("");
		}
		else
			sDesc = _T("긴급");
	}

	ids = pDM->SetCellData(_T("자료구분설명"), sDesc, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	sData.Empty();		sDesc.Empty();
	ids = pDM->GetCellData(_T("부록여부"), nInsertIndex, sData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	if(_T("A") == sData)
	{
		sDesc = _T("부록");
	}
	else
	{
		sDesc = _T("책");
	}
	ids = pDM->SetCellData(_T("부록여부설명"), sDesc, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	sData.Empty();		sDesc.Empty();
	CString sDelayDay;
	ids = pDM->GetCellData(_T("대출상태"), nInsertIndex, sData);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = pDM->GetCellData(_T("연체일수"), nInsertIndex, sDelayDay);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	
	CString strLILLLoanKey;
	pDM->GetCellData(_T("타관대출KEY"), nInsertIndex, strLILLLoanKey);
	CString strLILLTran;
	pDM->GetCellData(_T("트랜잭션상태"), nInsertIndex, strLILLTran);
	if(!strLILLLoanKey.IsEmpty())
	{ 
		if(_T("0") == strLILLTran)
		{
			sDesc = _T("신청");
		}
		else if(_T("1") == strLILLTran)
		{
			sDesc = _T("발송");
		}
		else if(_T("3") == strLILLTran)
		{
			sDesc = _T("입수");
		}
		else if(_T("4") == strLILLTran)
		{
			sDesc = _T("타관대출");
		}
		else 
		{
			sDesc = _T("알수없음");
		}
	}
	else
	{ 
		if(_T("0") == sData)	
		{
			if(0 < _ttoi(sDelayDay))	
			{
				sDesc = _T("연체");
			}
			else 
			{
				sDesc = _T("대출");
			}
		}
		else if(_T("1") == sData)	
		{
			sDesc = _T("반납");
		}
		else if(_T("2") == sData)	
		{
			sDesc = _T("반납연기");
		}
		else if(_T("3") == sData)
		{
			CString sRecKey, sValue, sReservationExpireDate;
			if(_T("Y") == m_pManageValue->m_sUnmannedReserveSys)
			{
				ids = pDM->GetCellData(_T("무인대출예약상태"), nInsertIndex, sValue);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
				if(_T("Y") == sValue)
				{
					sDesc = _T("예약(무인대출신청)");
				}
				else if(_T("O") == sValue)
				{
					ids = pDM->GetCellData(_T("예약만기일"), nInsertIndex, sReservationExpireDate);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
					if(sReservationExpireDate.IsEmpty())
					{
						sDesc = _T("예약");
					}
					else
					{
						CTime tExpireDate(_ttoi(sReservationExpireDate.Mid(0,4)), _ttoi(sReservationExpireDate.Mid(5,2)), 
											_ttoi(sReservationExpireDate.Mid(8,2)),_ttoi(sReservationExpireDate.Mid(11,2)), 0, 0); 
						
						if(tExpireDate < CTime(tCurrent.GetYear(),tCurrent.GetMonth(),tCurrent.GetDay(), tCurrent.GetHour(), 0, 0))
						{
							sDesc = _T("예약(무인대출지연)"); 
						}
						else
						{
							sDesc = _T("예약(무인대출대기)"); 
						}
					}
				}
				else
				{
					sDesc = _T("예약");
				}
			}
			else
			{
				sDesc = _T("예약");
			}
		}
		else if(_T("4") == sData)	
		{
			sDesc = _T("예약취소");
		}
		
		else if(_T("O") == sData)	
		{
			sDesc = _T("책두레예약");
		}
		else if(_T("L") == sData) 
		{
			sDesc = _T("책두레대출");
		}
		else if(_T("D") == sData)	
		{
			sDesc = _T("책두레반납연기");
		}
		else	
		{
			sDesc = _T("");
		}
	}
	
	if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse)
	{
		ids = pDM->GetCellData(_T("지역내상호대차상태"), nInsertIndex, sData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("Y") == sData)
		{
			sDesc = _T("지역내상호대차");
		}
	}
	
	ids = pDM->SetCellData(_T("대출상태설명"), sDesc, nInsertIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	ids = pGrid->DisplayLine(nInsertIndex);	
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	if(5 < nInsertIndex)
	{
		pGrid->SetTopRow(nInsertIndex);
	}

	CString sIndex;
	sIndex.Format(_T("%d"), nInsertIndex + 1);
	pGrid->SetTextMatrix(pDM->GetRowCount(), 0, sIndex); 

	ids = m_pParentMgr->ControlDisplay(sCMAlias, _T("MainGrid"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->ControlDisplay(sCMAlias, _T("MainGrid2"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	return 0;
	
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::CheckLoanBookCnt(CString sIsAppendix, BOOL bIsILL, CString strBookManageCode)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("CheckLoanBookCnt");

	CString sLoanType;
	ids = GetLoanType(sLoanType);
	if(0 > ids) return ids;
	if(_T("1") == sLoanType)
	{
		return 0;
	}
	
	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{ 
		
		CString sIsLoanBookCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("이동대출가능책수"), sIsLoanBookCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(1 > _ttoi(sIsLoanBookCnt)) 
		{	
			if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
			{ 
			}
			else
			{ 
				SelfCloseMessageBox(_T("대출 한도를 초과하여 대출할 수 없습니다."));
				return 2;
			}			
		}
	} 
	else
	{
		
		if(_T("Y") == m_pManageValue->m_sUnityLoanCntYN)
		{ 
			
			CString sIsLoanBookCnt;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합대출가능책수"), sIsLoanBookCnt, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			
			CString sIsIndividualLoanBookCnt;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출가능책수"), sIsIndividualLoanBookCnt, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			
			
			
			BOOL bOnlyAppendix = FALSE;

			if(1 > _ttoi(sIsLoanBookCnt))
			{
				if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
				{ 
				}
				else
				{ 
					SelfCloseMessageBox(_T("통합대출가능권수 한도를 초과하여 대출할 수 없습니다."));
					return 2;
				}			
			}
			
			if(TRUE == bIsILL)
			{
				CESL_DataMgr TmpDM;
				TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);

				CString strUserClassCode;
				m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자직급"), strUserClassCode, 0);
				CString strQuery;
				CString strLibTotalLoanCanCnt;
				strQuery.Format(_T("SELECT LOAN_BOOK_CNT FROM CD_INDIVIDUAL_CLASS_TBL ")
								_T("WHERE CODE='%s' AND MANAGE_CODE='%s'")
								, strUserClassCode, strBookManageCode);
				TmpDM.GetOneValueQuery(strQuery, strLibTotalLoanCanCnt);

				CString strIsAppendixCnt;

				strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
								_T("WHERE CLASS_ALIAS='부록관리' AND VALUE_NAME='부록대출권수포함여부' ")
								_T("AND MANAGE_CODE='%s'"), strBookManageCode);
				TmpDM.GetOneValueQuery(strQuery, strIsAppendixCnt);

				CESL_DataMgr* pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
				INT nLibLoanCnt = 0;

				CString strLoanStatus, strLDeveice;
				const int ROW_COUNT = pLoanDM->GetRowCount();

				for(INT nRow = 0; nRow < ROW_COUNT; nRow++)
				{					
					pLoanDM->GetCellData(_T("대출상태"), nRow, strLoanStatus);					
					if(_T("0") == strLoanStatus || _T("2") == strLoanStatus || 
						_T("L") == strLoanStatus || _T("D") == strLoanStatus)
					{
						pLoanDM->GetCellData(_T("L_DEVICE"), nRow, strLDeveice);
						if(_T("KOLASIII-이동도서관") == strLDeveice)
						{
							continue;
						}
						CString strGet;
						pLoanDM->GetCellData(_T("관리구분"), nRow, strGet);
						CString strType;
						pLoanDM->GetCellData(_T("부록여부"), nRow, strType);

						if(strGet == strBookManageCode)
						{
							if(_T("A") == strType)
							{
								if(_T("Y") == strIsAppendixCnt)
								{
									nLibLoanCnt++;
								}

								
								else bOnlyAppendix = TRUE;
							}
							else
							{
								nLibLoanCnt++;
							}
						}
					}
					else 
					{
						continue;
					}
				}	

				if(nLibLoanCnt >= _ttoi(strLibTotalLoanCanCnt))
				{
					if(bOnlyAppendix == 0)
					{					
						SelfCloseMessageBox(_T("도서관 대출가능권수 한도를 초과하여 대출할 수 없습니다."));
						return 2;
					}
				}
			}
			else
			{
			
				if(1 > _ttoi(sIsIndividualLoanBookCnt))
				{
					if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
					{ 
					}
					else
					{ 
						
							if(bOnlyAppendix == 0)
							{	
								SelfCloseMessageBox(_T("도서관 대출가능권수 한도를 초과하여 대출할 수 없습니다."));
			
								return 2;
							}
					}
				}
			}
		} 
		else
		{ 
			
			CString sUserManageCode;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("관리구분"), sUserManageCode, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

			CString sIsLoanBookCnt;
			
			
			if(TRUE == bIsILL || sUserManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE)
			{ 
				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("타관대출가능책수"), sIsLoanBookCnt, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
			}
			else		
			{ 
				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출가능책수"), sIsLoanBookCnt, 0);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
			}

			
			
			if(1 > _ttoi(sIsLoanBookCnt)) 
			{	
				if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
				{ 
				}
				else
				{ 
					SelfCloseMessageBox(_T("대출 한도를 초과하여 대출할 수 없습니다."));
					return 2;
				}			
			}
		} 
	} 
	
	if(_T("Y") == m_pManageValue->m_sIsUnionLoanCnt)
	{
		CString sUnionCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합대출책수"), sUnionCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
		if(sUnionCnt.IsEmpty())
		{
			SelfCloseMessageBox(_T("통합대출자정보 동기화 중입니다."));
			return 2;
		}

		CString strTotalLoanCnt;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총대출책수"), strTotalLoanCnt, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);

		if(1 > (_ttoi(sUnionCnt) - _ttoi(strTotalLoanCnt)))
		{
			
			if(0 != m_pCheckLoan->IsAppendixIncludeLoanCnt() && _T("A") == sIsAppendix)
			{
			}
			else
			{
				SelfCloseMessageBox(_T("통합 대출 한도를 초과하여 대출할 수 없습니다."));
				return 2;
			}
		}	
	}

	return 0;

EFS_END
return -1;
}

CString CBL_LOC_LOAN_PROC::GetSpeciesKey(CESL_DataMgr *pDM, CString recKey, INT type)
{
	EFS_BEGIN

	CString tblName;
	switch(type)
	{
		case 1 :
			tblName = _T("BO_BOOK_TBL");
			break;
		case 2 :
			tblName = _T("SE_BOOK_TBL");
			break;
		default :
			return _T("");
			break;
	}

	CString query, speciesKey;
	query.Format(_T("SELECT SPECIES_KEY FROM %s WHERE REC_KEY = %s"), tblName, recKey);
	pDM->GetOneValueQuery(query, speciesKey);
	return speciesKey;

	EFS_END

	return _T("");
}

INT CBL_LOC_LOAN_PROC::ReserveLinkBookInfo()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReserveLinkBookInfo");

	INT nSIndex = 0;
	CString sStatus = _T("");
	CString sLoanKey = _T("");
	CString sMsg = _T("");
	CReserveInfo RI;
	CReserveComplete RC;

	
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_Grid *pGrid = (CESL_Grid*)m_pParentMgr->FindControl(sCMAlias, sGridAlias);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	nSIndex = pGrid->GetIdx();
	
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("대출상태"), sStatus, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	if(sStatus.Compare(_T("3")) != 0)
	{
		AfxMessageBox(_T("이 자료는 예약중인 자료가 아닙니다."));
		return 1;
	}
	
	
	
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("단행/연속구분"), RI.sPublishFormCode, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("종KEY"), RI.sSpeciesKey, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("권호정보KEY"), RI.sVolInfo, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	
	CReserveLinkBookInfoDlg BookDlg(m_pParentMgr);
	BookDlg.SetIsAutoSetReservationDay(m_pManageValue->m_sIsAutoSetReservationDay);
	BookDlg.SetInfo(RI);
	ids = BookDlg.DoModal();
	
	
	if(5 == ids)
	{
		AfxMessageBox(_T("예약지정 할 수 있는 자료가 없습니다."), MB_ICONINFORMATION);
		return 0;
	}
	
	
	ids = BookDlg.GetInfo(RI);

	
	if(ids > 0) return 0;


	
	ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("대출/반납정보KEY"), sLoanKey, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	
	CESL_DataMgr* pUpdateDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pUpdateDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = RC.DBReserveComplete(RI, m_pParentMgr, sLoanKey, pUpdateDM);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		
	
	
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("상태"), RI.sWorkingStatus, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("이전자료상태"), RI.sPrevWorkingStatus, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("등록번호"), RI.sRegNo, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("IBS_청구_별치기호_코드"), RI.sSeparateShelfCode, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("자료실"), RI.sShelfLocCode, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("IBS_청구_권책기호"), RI.sVolCode, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("청구기호"), RI.sCallNo, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("RFID시리얼"), RI.sRFIDSerial, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(sDMAlias, _T("책KEY"), RI.sBookKey, nSIndex);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	pGrid->DisplayLine(nSIndex);	

	sMsg.Format(_T("예약지정완료되었습니다."));
	AfxMessageBox(sMsg);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::Receipt_Print(INT nType)
{
	if(m_pManageValue->m_sUseReceipt != _T("Y")) return 0;
	
	if(m_ReceiptDlg->GetSafeHwnd() == NULL)
	{
		m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG(nType, m_pParentMgr, m_pManageValue);
		m_ReceiptDlg->pMain = m_pParentMgr->pMain;
		m_ReceiptDlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		m_ReceiptDlg->m_pInfo = m_pParentMgr->m_pInfo;				
		if(m_ReceiptDlg->Create(m_pParentMgr))
		{
			m_ReceiptDlg->CenterWindow();
			m_ReceiptDlg->ShowWindow(SW_SHOW);
			m_ReceiptDlg->UpdateWindow();

			m_ReceiptDlg->initData();
		}
		else
		{
			delete m_ReceiptDlg;
		}
	}
	else
	{
		m_ReceiptDlg->ShowWindow(SW_SHOW);
		m_ReceiptDlg->initData();
	}		
	

	return 0;

}

INT CBL_LOC_LOAN_PROC::Receipt_PrintCheck()
{	
	CString	strMsg;
	CString	strMode;
	INT nLoanReceiptCnt;
	INT nReturnReceiptCnt;		

	if(m_pManageValue->m_sUseReceipt != _T("Y")) return 0;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pDM->GetRowCount() <= 0) return 0;	
	
	if(m_ReceiptDlg->GetSafeHwnd() == NULL)
	{
		m_ReceiptDlg = new CBO_LOC_RECEIPT_PRINT_DLG(0, m_pParentMgr, m_pManageValue);
		if(m_ReceiptDlg == NULL) return 0;
		m_ReceiptDlg->pMain = m_pParentMgr->pMain;
		m_ReceiptDlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		m_ReceiptDlg->m_pInfo = m_pParentMgr->m_pInfo;				
		if(m_ReceiptDlg->Create(m_pParentMgr))
		{			
			m_ReceiptDlg->CenterWindow();			
			m_ReceiptDlg->UpdateWindow();			
		}
		else
		{
			delete m_ReceiptDlg;
		}
	}
	
	if(!m_ReceiptDlg->IsWindowVisible())	return 0;

	StartLog(_T("영수증출력체크"));
	nLoanReceiptCnt = m_ReceiptDlg->GetLoanReceiptPrintCnt();
	nReturnReceiptCnt = m_ReceiptDlg->GetReturnReceiptPrintCnt();

	if(nLoanReceiptCnt == 0 && nReturnReceiptCnt == 0) 
	{
		EndLog();
		return 0;
	}
	else
	{
		if(m_ReceiptDlg->m_strMode == _T("0"))
		{
			if(AfxMessageBox(_T("미출력 영수증이 존재합니다.\n출력하시겠습니까?"), MB_YESNO) == IDYES)
			{
				m_ReceiptDlg->ReceiptAutoPrint(TRUE);							
			}
			else
			{
				
			}
		} else if(m_ReceiptDlg->m_strMode == _T("1"))
		{
			m_ReceiptDlg->ReceiptAutoPrint(TRUE);
		} else 
		{
			m_ReceiptDlg->ReceiptAutoPrint(FALSE);
		}
	}	

	EndLog();
	return 0;
}
INT CBL_LOC_LOAN_PROC::InitGridColor()
{	
	CString			sGridAlias = _T("MainGrid");
	CString			sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString			sLoanInfoDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	INT				nCols, nRowCount, i;

	CESL_DataMgr	*pLoanInfoDM = m_pParentMgr->FindDM(sLoanInfoDMAlias);
	nRowCount = pLoanInfoDM->GetRowCount();	

	if(m_bSimpleGrid == TRUE)
	{
		if(nRowCount > 0) return 0; 
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));

	nCols = pGrid->GetCols(0);
	for (i = 1 ; i < nCols ; i++) 
	{
		pGrid->SetCol(i);
		pGrid->SetCellBackColor(RGB(255,255,255));
		pGrid->SetCellForeColor(RGB(0,0,0));
		pGrid->SetCellFontBold(FALSE);
		pGrid->SetCellFontItalic(FALSE);
	}
	return 0;
}

INT CBL_LOC_LOAN_PROC::SetBBankMember()
{
EFS_BEGIN

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InsertBBankRecord(CKolas2up_MobileApi *pMobileApi, INT nMode)
{
EFS_BEGIN

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::InsertBBankRecord(CKolas2up_MobileApi *pMobileApi, CString strLoan_Key, INT nMode)
{
EFS_BEGIN

	return 0;

EFS_END
return -1;
}

INT	CBL_LOC_LOAN_PROC::ClearSearchUserBasket()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("ClearSearchUserBasket");
	
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_SEARCH_USER_BASKET"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	pDM->FreeData();

	ids = m_pParentMgr->ControlDisplay(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"), _T("이용자검색바구니"));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	

	pCM->AllControlClear();

	return 0;
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::AddSearchUserBasket()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("AddSearchUserBasket");

	
	CESL_DataMgr *pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	if(pMemberDM->GetRowCount() <= 0) return 0;	
	
	CESL_DataMgr* pBasketDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_SEARCH_USER_BASKET"));
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	CString strRecKey, strName, strUserNo;

	ids = pMemberDM->GetCellData(_T("대출자KEY"), 0, strRecKey);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pMemberDM->GetCellData(_T("대출자이름"), 0, strName);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	ids = pMemberDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString strValue1;

	ids = m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), strValue1);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	strValue1.TrimLeft();strValue1.TrimRight();
	strValue1.MakeUpper();

	if(_T("Y") != strValue1)
	{
		CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
	}
	
	CString strTime, strView;

	if( m_pManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pManageValue->m_sLoanUserAccessMode == _T("1") )
		strName = _T("***");
	
	CString strGetData;
	CTime t = m_pLocCommonAPI->GetCurrentDateTime(m_pParentMgr, strGetData);
	
	strTime.Format(_T("%02d시 %02d분 %02d초"),t.GetHour(),t.GetMinute(),t.GetSecond());
	strView.Format(_T("%s(%s)"),strName,strUserNo);

	if(0 == pBasketDM->GetRowCount())
		pBasketDM->InsertRow(-1);
	else
		pBasketDM->InsertRow(0);	

	pBasketDM->SetCellData(_T("검색시간"),strTime,0);
	pBasketDM->SetCellData(_T("대출자정보"),strView,0);

	pBasketDM->SetCellData(_T("REC_KEY"),strRecKey,0);
	pBasketDM->SetCellData(_T("대출자명"),strName,0);
	pBasketDM->SetCellData(_T("대출자번호"),strUserNo,0);

	
	INT nRowCnt = pBasketDM->GetRowCount();
	
	if(m_pManageValue->m_nUserBasketCnt < nRowCnt)	
	{
		for(INT i = nRowCnt ; i > m_pManageValue->m_nUserBasketCnt ; i--)
		{
			pBasketDM->DeleteRow(i-1);
		}
	}

	m_pParentMgr->ControlDisplay(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"), _T("이용자검색바구니"));

	
	if(1 < pBasketDM->GetRowCount())
	{
		m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_USER_BASKET"), _T("이전이용자"), pBasketDM->GetCellData(_T("대출자정보"),1));		
	}

	return 0;
EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ViewSimpleFamilyMember()
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("ViewSimpleFamilyMember");

	ids = ClearSimpleFamilyMember();
	if(0 > ids) return ids;

	ids = SearchFamilyMember();
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	if(0 == ids)
	{
		return 0;
	}
	
	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CKolas2up_ViewerControl* pCtrl = (CKolas2up_ViewerControl*)m_pParentMgr->GetDlgItem(1975);
	if(NULL == pCtrl) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_FAMILY_ID_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT	nRowCount = pDM->GetRowCount();
	for(INT i = 0; i < nRowCount; i++)
	{		
		CString strKey;
		CString strData;
		CString strUserNo;
		CString strUserName;
		CString strLoanCnt;

		ids = pDM->GetCellData(_T("대출자KEY"), i, strKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pDM->GetCellData(_T("대출자번호"), i, strUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		strUserNo.TrimLeft();
		strUserNo.TrimRight();

		
		CString sValue;
		ids = m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("이용자관리"), _T("대출화면주민번호표시유무"), sValue);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		sValue.TrimLeft();
		sValue.TrimRight();
		sValue.MakeUpper();

		if(_T("Y") != sValue)
		{
			CLocCommonAPI::ChangeSecuriyUserNo( m_pParentMgr , strUserNo , m_pManageValue->m_sIsUnityLoanService );
		}

		ids = pDM->SetCellData(_T("대출자번호표시"), strUserNo, i);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pDM->GetCellData(_T("이름"), i, strUserName);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		strUserName.TrimLeft();
		strUserName.TrimRight();
		
		if(m_pParentMgr->m_pInfo->MODE == 10000)
		{
			ids = pDM->GetCellData(_T("이동대출가능책수"), i, strLoanCnt);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			strLoanCnt.TrimLeft();
			strLoanCnt.TrimRight();
		}
		else
		{
			
			if(m_pManageValue->m_sUnityLoanCntYN == _T("Y"))
			{
				ids = pDM->GetCellData(_T("대출가능책수_통합환경"), i, strLoanCnt);
			}
			else
			{
				ids = pDM->GetCellData(_T("대출가능책수"), i, strLoanCnt);
			}
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			strLoanCnt.TrimLeft();
			strLoanCnt.TrimRight();
		}
		
		if(!strUserNo.IsEmpty())
		{
			strData = strUserNo;
		}

		if(!strUserName.IsEmpty())
		{
			if(!strData.IsEmpty())
			{
				strData += _T("/");
			}
			if ( m_pManageValue->m_sLoanUserAccessMode.IsEmpty() || m_pManageValue->m_sLoanUserAccessMode == _T("1") ) strUserName = _T("***");

			strData += strUserName;
		}
		
		if(!strLoanCnt.IsEmpty())
		{
			if(0 > _ttoi(strLoanCnt))
			{
				strLoanCnt = _T("0");
			}

			if(!strData.IsEmpty())
			{
				strData += _T("(");
			}

			strData += strLoanCnt + _T(")");
		}

		pCtrl->SetControlData(strKey, strData);
	}

	while(3 > nRowCount)
	{
		pCtrl->SetControlData(_T(" "), _T(" "));
		nRowCount++;
	}

	pCtrl->Display();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ClearSimpleFamilyMember()
{
EFS_BEGIN
	
	CString strFuncName = _T("ClearSimpleFamilyMember");

	CESL_ControlMgr* pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	
	CKolas2up_ViewerControl* pCtrl = (CKolas2up_ViewerControl*)m_pParentMgr->GetDlgItem(1975);

	
	
	if(NULL == pCtrl && m_pParentMgr->pParentMgr != NULL ) 
		pCtrl = (CKolas2up_ViewerControl*)m_pParentMgr->pParentMgr->GetDlgItem(1975);		
	
	if(NULL == pCtrl) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
		
	pCtrl->FreeLIST();
	pCtrl->Clear();
	
	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::LoanStopDateCancel()
{
EFS_BEGIN
	
	CESL_DataMgr	*pDM;
	CString			strRecKey;
	CString			strUserNo;
	CString			strName;
	CString			strLoanStopDate;
	
	pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));

	pDM->GetCellData(_T("대출자KEY")	, 0, strRecKey);
	pDM->GetCellData(_T("대출자번호")	, 0, strUserNo);
	pDM->GetCellData(_T("대출자이름")	, 0, strName);
	pDM->GetCellData(_T("대출정지일")	, 0, strLoanStopDate);

	if(strLoanStopDate.IsEmpty())
	{
		SelfCloseMessageBox(_T("해제할 대출정지일이 없습니다."));
	}
	else
	{
		CTime tCurrentTime;	
		tCurrentTime = CTime::GetCurrentTime();
		tCurrentTime = CTime(tCurrentTime.GetYear(), tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0);		
		
		CTime tLoanStopTime(_ttoi(strLoanStopDate.Mid(0,4)), _ttoi(strLoanStopDate.Mid(5,2)), _ttoi(strLoanStopDate.Mid(8,2)), 0, 0, 0);
		
		CTime tLimitTime(tCurrentTime.GetYear()+1, tCurrentTime.GetMonth(), tCurrentTime.GetDay(), 0, 0, 0);
		
		if(tLimitTime < tLoanStopTime)
		{
			AfxMessageBox(	_T("대출정지일은 1년을 넘을 수 없습니다.\r\n")
							_T("대출정지일을 확인해 주십시오."));
			return 0;
		}
		
		if(tLoanStopTime < tCurrentTime)
		{
			AfxMessageBox(_T("해제할 대출정지일이 없습니다."));
		}
		else
		{	
			CFeeLoanStop dlg(m_pParentMgr, m_pLoanShareManager, m_pManageValue);
			dlg.SetUserKey(pDM, strRecKey, strUserNo, strName, strLoanStopDate);
			dlg.m_strLibCode = m_strLibCode;
			dlg.DoModal();
			if(dlg.m_bPrint == TRUE)
			{
				ReLoadMemberInfo();
			}
		}
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::KL_OtherReturnMgr()
{
	if(NULL == m_pOtherReturnMgr)
	{
		m_pOtherReturnMgr = new CKL_OTHER_RETURN_MAIN(m_pParentMgr);
		m_pOtherReturnMgr->pMain = m_pParentMgr->pMain;
		m_pOtherReturnMgr->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		m_pOtherReturnMgr->m_pInfo = m_pParentMgr->m_pInfo;				
		
		
		m_pOtherReturnMgr->m_pLoanManageValue = m_pManageValue;
		m_pOtherReturnMgr->m_pLoanShareManager = m_pLoanShareManager;
		
		if(m_pOtherReturnMgr->Create(m_pParentMgr))
		{			
			m_pOtherReturnMgr->CenterWindow();
			m_pOtherReturnMgr->ShowWindow(SW_SHOW);
			m_pOtherReturnMgr->UpdateWindow();
			m_pOtherReturnMgr->pMain->SendMessage(WM_USER+1028, (WPARAM)m_pOtherReturnMgr, 0);
		}
		else
		{
			delete m_pOtherReturnMgr;
			m_pOtherReturnMgr = NULL;
		}
	}
	else
	{
		if(TRUE == m_pOtherReturnMgr->IsWindowVisible() && TRUE == m_pOtherReturnMgr->m_bShow)
		{
			m_pOtherReturnMgr->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pOtherReturnMgr->ShowWindow(SW_RESTORE);
			m_pOtherReturnMgr->SetActiveWindow();
			m_pOtherReturnMgr->SetFocus();
		}
	}

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_UserLoanInfo(CString strUserNo)
{
	CString strFuncName = _T("KL_UserLoanInfo");

	if(10000 != m_pParentMgr->m_pInfo->MODE)
	{
		CString strSearchMode = _T("");
		GetTempData(_T("기본검색범위"), strSearchMode);

		if(_T("1") == strSearchMode || _T("2") == strSearchMode)
		{
			if(m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
			{
				CKL_UserSearchDlg dlg(m_pParentMgr, this);
				dlg.m_strUserNo = strUserNo;
				dlg.m_strWorkNo = _T("");
				dlg.m_nMode = 1;
				dlg.DoModal();
			}
	
	
		}
	}

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_UserInfoView()
{
	INT ids;
	CString strFuncName = _T("KL_UserInfoView");

	ids = ViewLoanScreen();
	if(0 > ids) return ids;

	return 0;
}


INT CBL_LOC_LOAN_PROC::KL_GetMaxLoanCount()
{
	CKL_UserSearchDlg dlg(m_pParentMgr, this);
	dlg.m_nMode = 2;

	if(m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
	{
		dlg.DoModal();
	}

	if(dlg.m_strMaxLoanCount.IsEmpty())
	{
		
		CString strGetData = _T("");
		GetTempData(_T("기본통합대출가능권수"), strGetData);

		if(strGetData.IsEmpty())
		{
			m_strMaxLoanCount = _T("20");
		}
		else
		{
			m_strMaxLoanCount = strGetData;
		}
	}
	else
	{
		m_strMaxLoanCount = dlg.m_strMaxLoanCount;
	}

	return 0;
}


INT CBL_LOC_LOAN_PROC::KL_GetManage()
{
	m_strMaxLoanCount = _T("");
	m_strLocalFreeYN = _T("");
	m_strOtherFreeYN = _T("");
	m_strAutoSendYN = _T("");
	m_strLibCodeList = _T("");
	if(m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
	{
		CKL_UserSearchDlg dlg(m_pParentMgr, this);
		dlg.m_nMode = 6;
		if(IDOK == dlg.DoModal())
		{
			m_strMaxLoanCount = dlg.m_strMaxLoanCount;
			m_strLocalFreeYN = dlg.m_strLocalFreeYN;
			m_strOtherFreeYN = dlg.m_strOtherFreeYN;
			m_strAutoSendYN = dlg.m_strAutoSendYN;
			m_strLibCodeList = dlg.m_strLibCodeList;
		}
	}

	CString strGetData = _T("");;
	if(m_strMaxLoanCount.IsEmpty())
	{
		strGetData.Empty();
		GetTempData(_T("기본통합대출가능권수"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strMaxLoanCount = _T("20");
		}
		else
		{
			m_strMaxLoanCount = strGetData;
		}
	}
	if(m_strLocalFreeYN.IsEmpty())
	{
		strGetData.Empty();
		GetTempData(_T("지역내택배비무료여부"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strLocalFreeYN = _T("N");
		}
		else
		{
			m_strLocalFreeYN = strGetData;
		}
	}
	if(m_strOtherFreeYN.IsEmpty())
	{
		strGetData.Empty();
		GetTempData(_T("지역외택배비무료여부"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strOtherFreeYN = _T("N");
		}
		else
		{
			m_strOtherFreeYN = strGetData;
		}
	}
	
	if(m_strAutoSendYN.IsEmpty())
	{
		
		GetTempData(_T("자동발송여부"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strAutoSendYN = _T("N");
		}
		else
		{
			m_strAutoSendYN = strGetData;
		}
	}
	
	if(m_strLibCodeList.IsEmpty())
	{
		strGetData.Empty();
		GetTempData(_T("지역내도서관부호리스트"), strGetData);
		if(strGetData.IsEmpty())
		{
			m_strLibCodeList = _T("");
		}
		else
		{
			m_strLibCodeList = strGetData;
		}
	}

	return 0;
}


INT CBL_LOC_LOAN_PROC::KL_GetManage_Condition(CString strLibCode, CString& strBookLibReloanYN)
{
	strBookLibReloanYN = _T("N");
	if(!m_pParentMgr->m_pInfo->IsConnectUnityLoanService()) return 0;
	if ( strLibCode.GetLength() <= 0 ) return 0;

	CKL_UserSearchDlg dlg(m_pParentMgr, this);
	dlg.m_nMode = _MODE_LIB_CON_MANAGE_;
	dlg.m_SearchLibCode = strLibCode;
	dlg.DoModal();

	strBookLibReloanYN = dlg.m_strBookLibReloanYN;
	
	dlg.m_SearchLibCode = _T("");
	dlg.m_strBookLibReloanYN = _T("N");
	
	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_Reloan_Proc(INT nIdx, BOOL& bSuccess)
{	
	INT ids = -1, nExp = 0, nSuccCnt = 0 ;
	CString strBookLibCode = _T(""), strBookLibReloanYN = _T("N"), strFuncName = _T("KL_Reloan_Proc"), strReturnPlanDate = _T("");
	CString strStatus = _T(""), strReloanPeriod = _T(""), strReloanPlanDate = _T(""), strLocalKey = _T(""), strFailReason = _T("");

	CString sDMAlias	= _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias	= _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias	= _T("MainGrid");

	bSuccess = FALSE;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	

	
	ids = pDM->GetCellData(_T("제공도서관부호"), nIdx , strBookLibCode);
	ids = KL_GetManage_Condition(strBookLibCode, strBookLibReloanYN);
	if ( ids < 0 ) return ids;

	if ( strBookLibReloanYN != _T("Y") ) {
		AfxMessageBox(_T("소장관에서 지역상호대차자료의 반납연기를 허용하지 않습니다."));				
		return 0;
	}
	
	
	ids = pDM->GetCellData(_T("대출상태"), nIdx, strStatus);
	if(  !(_T("0") == strStatus || _T("2") == strStatus )) {
		AfxMessageBox(_T("대출중이거나 반납연기 상태의 자료만 반납연기 가능합니다."));		
		return 0;
	}
	
	ids = pDM->GetCellData(_T("반납예정일"), nIdx, strReturnPlanDate);
	m_pCheckLoan->GetReturnDelayDay(strReloanPeriod, _T("NO_REG_CODE"));
	CTime tRenewalTime(_ttoi(strReturnPlanDate.Left(4)),_ttoi(strReturnPlanDate.Mid(5,2)),_ttoi(strReturnPlanDate.Mid(8,2)),0,0,0);
	nExp = _ttoi(strReloanPeriod);
	m_pCheckLoan->GetAddTotalHolyDay( nExp , tRenewalTime);		
	CTime tReLoanPlanDate = tRenewalTime + CTimeSpan(nExp, 0, 0, 0);
	strReloanPlanDate = tReLoanPlanDate.Format(_T("%Y/%m/%d"));

	
	pDM->GetCellData(_T("로컬KEY"), nIdx, strLocalKey);
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(_T("LILL_K18_LOC_B01_SERVICE"));
	Writer.addRecord();	
	Writer.addElement(_T("제공도서관부호"),strBookLibCode);
	Writer.addElement(_T("제공도서관부호"),strBookLibCode);
	Writer.addElement(_T("로컬KEY"),strLocalKey);	
	Writer.addElement(_T("반납예정일"),strReloanPlanDate);
	Writer.addElement(_T("마지막작업"), m_pParentMgr->GetWorkLogMsg(_T("지역상호대차반납연기"), __WFILE__, __LINE__));
	
	CFrameSender Sender(m_pParentMgr->m_pInfo);	
	CFrameReader Reader; 
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if (0 > ids) {
		AfxMessageBox(_T("책이음서비스 미들웨어와의 통신에 실패하였습니다."));
		pGrid->Display();
		return 0;		
	}
	
	Reader.parseFrame(Reader.getFrame());
	if(_T("OK") != Reader.getElement(0,0)) {
		if(_T("")==Reader.getElement(0,0)) {
			pDM->SetCellData(_T("RESULT"),_T("FAIL"),nIdx);
			AfxMessageBox(_T("실패 발생"));
		}
		else {
			strFailReason.Format(_T("%s"), Reader.getElement(0, _T("처리결과설명")));
			pDM->SetCellData(_T("RESULT"), strFailReason, nIdx);
			AfxMessageBox(strFailReason);		
		}		
		return 0;
	}

	
	TCHAR *sLinkDMFieldAlias[2] = {_T("반납예정일"), _T("대출상태")};
	INT nColIndex[2] = {-1, -1};
	ids = CLocCommonAPI::GetGridColumnIndexs(m_pParentMgr, pGrid, sLinkDMFieldAlias, 2, nColIndex);
	if(0 > ids) return ids;
	
	ids = pDM->SetCellData(_T("반납예정일"), strReloanPlanDate, nIdx );	
	ids = pGrid->SetAt(nIdx, nColIndex[0], strReloanPlanDate);

	ids = pDM->SetCellData(_T("대출상태"), _T("2"), nIdx );
	ids = pGrid->SetAt(nIdx, nColIndex[1], _T("2"));
	
	bSuccess = TRUE;

	return 0;
}


INT CBL_LOC_LOAN_PROC::IsKLReloanUser(CESL_DataMgr* pDM, BOOL& bUser)
{
	INT ids = -1, i = 0;
	CString strDelayDays = _T(""), strUserClass = _T(""), strLostUserCard = _T("");
	bUser = TRUE;

	
	for( i = 0; i < pDM->GetRowCount(); i++) {
		ids = pDM->GetCellData(_T("연체일수"), i, strDelayDays);
		if(0 > ids) return ids;
		if(0 < _ttoi(strDelayDays)) {
			bUser = FALSE;
			SelfCloseMessageBox(_T("연체 이용자는 반납연기가 불가능합니다."));
			return 0;
		}
	}
	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("회원상태"), strUserClass, 0);
	if(0 > ids) return ids;
	if(_T("0") != strUserClass) {	
		bUser = FALSE;
		SelfCloseMessageBox(_T("정상회원만 반납연기가 가능합니다."));
		return 0;
	}	
	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("회원증분실"), strLostUserCard, 0);
	if(0 > ids) return ids;
	if(_T("Y") == strLostUserCard) {
		bUser = FALSE;
		SelfCloseMessageBox(_T("회원증분실 회원은 반납연기가 불가능 합니다."));
		return 0;		
	}

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_GetPaymentFree()
{
	if(!m_pParentMgr->m_pInfo->IsConnectUnityLoanService())
	{
		m_strLocalFreeYN = _T("N");
		m_strOtherFreeYN = _T("N");
		return 0;
	}
	CKL_UserSearchDlg dlg(m_pParentMgr, this);
	dlg.m_nMode = 5;
	dlg.DoModal();
	m_strLocalFreeYN = dlg.m_strLocalFreeYN;
	m_strOtherFreeYN = dlg.m_strOtherFreeYN;

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_StartTemp()
{
	m_asFrames.RemoveAll();
	m_asFrames.FreeExtra();

	return 0;
}
INT CBL_LOC_LOAN_PROC::KL_AddTemp(CString strFrame)
{
	m_asFrames.Add(strFrame);

	return 0;
}
INT CBL_LOC_LOAN_PROC::KL_EndTemp(CString strFirstWork)
{
	if(0 < m_asFrames.GetSize())
	{
		CFrameSender Sender(m_pParentMgr->m_pInfo);
		Sender.SendTempTbl(m_asFrames, strFirstWork);
	}

	return 0;
}


BOOL CBL_LOC_LOAN_PROC::IsNumericText(CString strValue)
{
	INT nLength = strValue.GetLength();
	if(1 > nLength) return FALSE;
	BOOL bCheck = FALSE;
	for(INT i = 0; i < strValue.GetLength(); i++)
	{
		TCHAR cValue = strValue.Mid(i, 1).GetAt(0);
		if(cValue >= '0' && cValue <= '9')
		{
			bCheck = TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return bCheck;
}

INT CBL_LOC_LOAN_PROC::KL_SearchLoanHistoryInfo()
{
	if(0 == IsSearchedUser())
	{
		CString strKLUserYN;
		CESL_DataMgr* pUserDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
		if(pUserDM)
		{
			pUserDM->GetCellData(_T("통합회원여부"), 0, strKLUserYN);
			if(_T("Y") != strKLUserYN)
			{
				SelfCloseMessageBox(_T("통합회원으로 전환한 회원만 검색할 수 있습니다."));
				return 1000;
			}

			CKL_SearchLoanInfoDlg Dlg(m_pParentMgr);
			Dlg.DoModal();
		}
	}

	return 0;
}

VOID CBL_LOC_LOAN_PROC::Send3000ApiSocket(CString strType, CString strMsg/*=_T("NULL")*/)
{
	if(NULL != m_lp3000ApiSocket)
	{
		m_lp3000ApiSocket(strType, strMsg);
	}
}
INT CBL_LOC_LOAN_PROC::KL_SearchLoanInfo(INT nMode, CString &strErrorMsg)
{
EFS_BEGIN

	INT	ids;
	CString strFuncName = _T("KL_SearchLoanInfo");

	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(1 > pDM->GetRowCount()) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString strCommand, strType;
	if(0 == nMode)
	{
		ids = SearchLoanBookInfo();
		if(0 > ids) return ids;
		return 0;
	}
	else if(1 == nMode)
	{	
		strCommand = _T("IL_K09_LOC_B01_SERVICE");
		strType = _T("지역");
	}
	else if(2 == nMode)
	{	
		strCommand = _T("IL_K04_LOC_B01_SERVICE");
		strType = _T("통합");

		
		ids = KL_SearchLoanInfo(1, strErrorMsg);
		if(0 != ids) return ids;
	}
	else
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	CString strIsUnityUser;
	pDM->GetCellData(_T("통합회원여부"), 0, strIsUnityUser);
	if(_T("Y") != strIsUnityUser) return 100;

	
	
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	
	CString strData;
	INT nRowCnt = pLoanDM->GetRowCount();
	for(INT nRow = nRowCnt; nRow >= 0; nRow--)
	{
		ids = pLoanDM->GetCellData(_T("현황구분"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(
			(1 == nMode && (_T("지역") == strData || _T("통합") == strData)) ||
			(2 == nMode && _T("통합") == strData)
			)
		{
			pLoanDM->DeleteRow(nRow);
		}
	}
	
	CString strUserNo;
	ids = pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;

	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("통합대출자번호"), strUserNo);
	
	CString strGpinHash;
	ids = pDM->GetCellData(_T("G-PINHASH인증코드"), 0, strGpinHash);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	Writer.addElement(_T("G-PINHASH인증코드"), strGpinHash);
	
	
	if((1 == nMode) && _T("N") == m_strLocalFreeYN)
	{	
		
		Writer.addElement(_T("도서관부호"), _T("000000"));
	}
	else
	{
		Writer.addElement(_T("도서관부호"), strLibCode);
	}

	CFrameReader Reader;
	CFrameSender Sender(m_pParentMgr->m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) return ids;
	
	CESL_DataMgr TmpDM;
	ids = Reader.makeDM(&TmpDM);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg,strTemp;
		TmpDM.GetCellData(0, 0, strTemp);
		strErrorMsg.Format(_T("미들웨어에서 에러가 발생하였습니다."));
		if(1 == nMode)
		{
			strMsg.Format(_T("지역센터대출현황조회(9) ERROR CODE : %s"), strTemp);
		}
		else
		{
			strMsg.Format(_T("통합센터대출현황조회(4) ERROR CODE : %s"), strTemp);
		}
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return 200;
	}

	if((1 == nMode) && _T("N") == m_strLocalFreeYN)
	{
		CString strKey1, strKey2;
		CString strLibCode;
		CString strPaymentKey;
		
		if(0 < TmpDM.GetRowCount())
		{
			INT nRowCnt = pLoanDM->GetRowCount();
			for(INT nRow = 0; nRow < nRowCnt; nRow++)
			{
				ids = pLoanDM->GetCellData(_T("현황구분"), nRow, strData);
				if(_T("지역") == strData && _T("통합") == strData)
				{
					continue;
				}
				
				pLoanDM->GetCellData(_T("로컬KEY"), nRow, strKey1);				
				if(!strKey1.IsEmpty())
				{
					for(INT nRow2 = 0; nRow2 < TmpDM.GetRowCount(); nRow2++)
					{
						TmpDM.GetCellData(_T("로컬KEY"), nRow2, strKey2);
						if(strKey1 == strKey2)
						{
							TmpDM.GetCellData(_T("대출도서관부호"), nRow2, strLibCode);
							if(!strLibCode.IsEmpty())
							{
								if(-1 < m_pManageValue->m_strLibCodeList.Find(strLibCode))
								{
									TmpDM.GetCellData(_T("결제처리KEY"), nRow, strPaymentKey); 							
									pLoanDM->SetCellData(_T("결제처리KEY"), strPaymentKey, nRow);
									continue;
								}
							}
						}
					}
				}
			}
			
			nRowCnt = TmpDM.GetRowCount();
			for(nRow = nRowCnt; nRow >= 0; nRow--)
			{
				TmpDM.GetCellData(_T("대출도서관부호"), nRow, strData);
				if(!strData.IsEmpty())
				{
					if(-1 < m_pManageValue->m_strLibCodeList.Find(strData))
					{
						TmpDM.DeleteRow(nRow);
					}
				}
			}	
		}
	}

	if(1 > TmpDM.GetRowCount())
	{
		return 0;
	}

	INT nInsertRow = 0;
	nRowCnt = TmpDM.GetRowCount();
	for(nRow = 0; nRow < nRowCnt; nRow++)
	{
		pLoanDM->InsertRow(-1);
		nInsertRow = pLoanDM->GetRowCount() - 1;

		ids = TmpDM.GetCellData(_T("대출도서관부호"), nRow, strData);

		ids = pLoanDM->SetCellData(_T("대출도서관부호"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("대출일"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("대출일"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("반납예정일"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("반납예정일"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("예약일"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("예약일"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("예약만기일"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("예약만기일"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("상태"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("대출상태"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("등록번호"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("등록번호"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("서명"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("본표제"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("청구기호"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("청구기호"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = pLoanDM->SetCellData(_T("현황구분"), strType, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

 		ids = TmpDM.GetCellData(_T("결제처리KEY"), nRow, strData);
 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
 		ids = pLoanDM->SetCellData(_T("결제처리KEY"), strData, nInsertRow);
 		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("대출도서관"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("대출도서관"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		ids = TmpDM.GetCellData(_T("로컬KEY"), nRow, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pLoanDM->SetCellData(_T("로컬KEY"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		
		TmpDM.GetCellData(_T("반납예정도서관부호"), nRow, strData);
		ids = pLoanDM->SetCellData(_T("반납예정도서관부호"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		TmpDM.GetCellData(_T("반납예정도서관명"), nRow, strData);
		ids = pLoanDM->SetCellData(_T("반납예정도서관명"), strData, nInsertRow);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	}

	return 0;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::KL_SearchLoanInfo(CString &strErrorMsg)
{
EFS_BEGIN

	INT	ids;
	CString strFuncName = _T("KL_SearchLoanInfo");

	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(1 > pDM->GetRowCount()) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CString strIsUnityUser;
	pDM->GetCellData(_T("통합회원여부"), 0, strIsUnityUser);
	if(_T("Y") != strIsUnityUser) return 100;
	
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pLoanDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString strData;
	INT nRowCnt = pLoanDM->GetRowCount();
	for(INT i = nRowCnt; i >= 0; i--)
	{
		ids = pLoanDM->GetCellData(_T("현황구분"), i, strData);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		if(_T("지역") == strData || _T("통합") == strData)
		{
			pLoanDM->DeleteRow(i);
		}
	}	
	
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(_T("IL_K42_LOC_B01_SERVICE"));
	Writer.addRecord();
	
	CString strUserNo;
	pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
 	if(strUserNo.IsEmpty())
 
 	{
 		strErrorMsg.Format(_T("대출자번호가 없습니다."));
 		return 200;
 	}
	Writer.addElement(_T("통합대출자번호"), strUserNo);

	CString strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	Writer.addElement(_T("도서관부호"), strLibCode);
	Writer.addElement(_T("지역내택배비무료"), m_strLocalFreeYN);
	
	CFrameReader Reader;
	CFrameSender Sender(m_pParentMgr->m_pInfo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids)
	{
		strErrorMsg.Format(_T("미들웨어 접속이 실패하였습니다."));
		return 200;
	}
	
	CESL_DataMgr TmpDM;
	ids = Reader.makeDM(&TmpDM);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg, strTemp;
		strTemp = Reader.getElement(0, 0);
		strErrorMsg.Format(_T("서비스 오류(%s)"), strTemp);
		strMsg.Format(_T("대출현황조회(42) ERROR CODE : %s"), strTemp);
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return 200;
	}
	if(1 > TmpDM.GetRowCount())
	{
		return 0;
	}
	
	if(_T("N") == m_strLocalFreeYN && 0 < TmpDM.GetRowCount())
	{
		CString strKey1, strKey2, strLibCode, strPaymentKey;
		INT nRowCnt = pLoanDM->GetRowCount();
		for(INT i = 0; i < nRowCnt; i++)
		{
			ids = pLoanDM->GetCellData(_T("현황구분"), i, strData);
			if(_T("지역") == strData && _T("통합") == strData) continue;
			pLoanDM->GetCellData(_T("로컬KEY"), i, strKey1);
			if(strKey1.IsEmpty()) continue;
			for(INT j = 0; j < TmpDM.GetRowCount(); j++)
			{
				TmpDM.GetCellData(_T("로컬KEY"), j, strKey2);
				if(strKey1 == strKey2)
				{
					TmpDM.GetCellData(_T("결제처리KEY"), i, strPaymentKey);
					if(strPaymentKey.IsEmpty()) break;
					TmpDM.GetCellData(_T("대출도서관부호"), j, strLibCode);
					if(strLibCode.IsEmpty()) break;
					if(-1 < m_pManageValue->m_strLibCodeList.Find(strLibCode))
					{	
						pLoanDM->SetCellData(_T("결제처리KEY"), strPaymentKey, i);
					}
					break;
				}
			}
		}
		
		nRowCnt = TmpDM.GetRowCount();
		for(i = nRowCnt; i >= 0; i--)
		{
			TmpDM.GetCellData(_T("대출도서관부호"), i, strData);
			if(strData.IsEmpty()) continue;			
			if(-1 < m_pManageValue->m_strLibCodeList.Find(strData))
			{
				TmpDM.DeleteRow(i);
			}
		}
	}
	
	INT nInsertRow = 0;
	nRowCnt = TmpDM.GetRowCount();
	for(i = 0; i < nRowCnt; i++)
	{
		
		if(0 == i)
		{
			TmpDM.GetCellData(_T("지역내대출정지유무"), i, strData);
			if(_T("Y") == strData)
			{
				pDM->SetCellData(_T("대출정지유무"), strData, 0);
			}
			else
			{
				pDM->SetCellData(_T("대출정지유무"), _T("N"), 0);
			}
			TmpDM.GetCellData(_T("로컬KEY"), i, strData);
			
			
			CString strData2;
			TmpDM.GetCellData(_T("타관대출KEY"), i, strData2);
			if(0 == strData2.GetLength() && strData.IsEmpty()) break;
			
		}
		
		pLoanDM->InsertRow(-1);
		nInsertRow = pLoanDM->GetRowCount() - 1;
		TmpDM.GetCellData(_T("대출도서관부호"), i, strData);
		pLoanDM->SetCellData(_T("대출도서관부호"), strData, nInsertRow);
		TmpDM.GetCellData(_T("대출일"), i, strData);
		pLoanDM->SetCellData(_T("대출일"), strData, nInsertRow);
		TmpDM.GetCellData(_T("반납예정일"), i, strData);
		
		if ( strData.GetLength() > 10 ) strData = strData.Left(10);
		pLoanDM->SetCellData(_T("반납예정일"), strData, nInsertRow);
		TmpDM.GetCellData(_T("예약일"), i, strData);
		pLoanDM->SetCellData(_T("예약일"), strData, nInsertRow);
		TmpDM.GetCellData(_T("예약만기일"), i, strData);
		pLoanDM->SetCellData(_T("예약만기일"), strData, nInsertRow);
		TmpDM.GetCellData(_T("상태"), i, strData);
		pLoanDM->SetCellData(_T("대출상태"), strData, nInsertRow);
		TmpDM.GetCellData(_T("등록번호"), i, strData);
		pLoanDM->SetCellData(_T("등록번호"), strData, nInsertRow);
		TmpDM.GetCellData(_T("서명"), i, strData);
		pLoanDM->SetCellData(_T("본표제"), strData, nInsertRow);
		TmpDM.GetCellData(_T("청구기호"), i, strData);
		pLoanDM->SetCellData(_T("청구기호"), strData, nInsertRow);
		TmpDM.GetCellData(_T("현황구분"), i, strData);
		pLoanDM->SetCellData(_T("현황구분"), strData, nInsertRow);
 		TmpDM.GetCellData(_T("결제처리KEY"), i, strData);
 		pLoanDM->SetCellData(_T("결제처리KEY"), strData, nInsertRow);
		TmpDM.GetCellData(_T("대출도서관"), i, strData);
		pLoanDM->SetCellData(_T("대출도서관"), strData, nInsertRow);
		TmpDM.GetCellData(_T("로컬KEY"), i, strData);
		pLoanDM->SetCellData(_T("로컬KEY"), strData, nInsertRow);
		TmpDM.GetCellData(_T("반납예정도서관부호"), i, strData);
		pLoanDM->SetCellData(_T("반납예정도서관부호"), strData, nInsertRow);
		TmpDM.GetCellData(_T("반납예정도서관명"), i, strData);
		pLoanDM->SetCellData(_T("반납예정도서관명"), strData, nInsertRow);
		
		
		TmpDM.GetCellData(_T("부록여부"), i, strData);
		if(strData.IsEmpty()) strData = _T("B");
		pLoanDM->SetCellData(_T("부록여부"), strData, nInsertRow);
		if(_T("A") == strData)
		{
			strData = _T("부록");
		}
		else
		{
			strData = _T("책");
		}
		pLoanDM->SetCellData(_T("부록여부설명"), strData, nInsertRow);
		
		TmpDM.GetCellData(_T("지역내상호대차상태"), i, strData);
		if(strData.IsEmpty()) strData = _T("N");
		pLoanDM->SetCellData(_T("지역내상호대차상태"), strData, nInsertRow);

		
		TmpDM.GetCellData(_T("제공도서관"), i, strData);
		pLoanDM->SetCellData(_T("제공도서관"), strData, nInsertRow);
		TmpDM.GetCellData(_T("제공도서관부호"), i, strData);
		pLoanDM->SetCellData(_T("제공도서관부호"), strData, nInsertRow);
		
		pLoanDM->SetCellData(_T("소장관도서관부호"), strData, nInsertRow);
		TmpDM.GetCellData(_T("타관대출KEY"), i, strData);
		pLoanDM->SetCellData(_T("타관대출KEY"), strData, nInsertRow);
		if(!strData.IsEmpty())
		{
			pLoanDM->SetCellData(_T("LCT_KEY"), strData, nInsertRow);

			TmpDM.GetCellData(_T("상태"), i, strData);
			pLoanDM->SetCellData(_T("트랜잭션상태"), strData, nInsertRow);
			if(_T("4") == strData)
			{
				pLoanDM->SetCellData(_T("대출상태"), _T("0"), nInsertRow);
			}
			else
			{
				pLoanDM->SetCellData(_T("대출상태"), _T(""), nInsertRow);
			}
		}
		
		TmpDM.GetCellData(_T("부록발송유무"), i, strData);
		pLoanDM->SetCellData(_T("부록발송유무"), strData, nInsertRow);
		TmpDM.GetCellData(_T("부록등록번호리스트"), i, strData);
		pLoanDM->SetCellData(_T("부록등록번호리스트"), strData, nInsertRow);
		TmpDM.GetCellData(_T("대출만기일"), i, strData);
		pLoanDM->SetCellData(_T("대출만기일"), strData, nInsertRow);
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::ReservationStatus(CString sGetData[])
{	
	if(!(m_pManageValue->m_bVolReserve))
	{ 
		CBO_LOC_3170 *Dlg = new CBO_LOC_3170(m_pParentMgr, m_pLoanShareManager, m_pManageValue, 0);
		Dlg->pMain = m_pParentMgr->pMain;
		Dlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		Dlg->m_pInfo = m_pParentMgr->m_pInfo;
		Dlg->SetInfo(sGetData[3], sGetData[4], sGetData[0], sGetData[1]);
		
		POSITION pos;
		pos = m_obj3170DlgList->AddTail((CObject*)Dlg);
		if(Dlg->Create(m_pParentMgr))
		{
			Dlg->CenterWindow();
			Dlg->ShowWindow(SW_SHOWNOACTIVATE);
			Dlg->UpdateWindow();				
		}
		else
		{
			m_obj3170DlgList->RemoveAt(pos);
		}
	}
	else 
	{ 
		CBO_LOC_3170_ReserveInfo *Dlg = new CBO_LOC_3170_ReserveInfo(m_pParentMgr, m_pLoanShareManager, m_pManageValue, 0);
		Dlg->pMain = m_pParentMgr->pMain;
		Dlg->pParentMgr = (CESL_Mgr*)m_pParentMgr;
		Dlg->m_pInfo = m_pParentMgr->m_pInfo;
		Dlg->SetInfo(sGetData[3], sGetData[4], sGetData[0], _T(""));
		
		
		POSITION pos;
		pos = m_obj3170DlgList->AddTail((CObject*)Dlg);
		if(Dlg->Create(m_pParentMgr))
		{
			Dlg->CenterWindow();
			Dlg->ShowWindow(SW_SHOWNOACTIVATE);
			Dlg->UpdateWindow();				
		}
		else
		{
			m_obj3170DlgList->RemoveAt(pos);
		}
	}

	return 0;
}

INT CBL_LOC_LOAN_PROC::ReservationStatus()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("ReservationStatus");

	INT			nIndex		=	-1;				
	BOOL		bEdit		=	FALSE;			
	CString		strStatus	=	_T("");			
	CString		sDMAlias	=	_T("DM_BO_LOC_3100_LOAN_INFO");	

	
	CESL_DataMgr	*pDM	=	m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_ControlMgr	*pCM	=	m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CESL_Grid	*pGrid		=	(CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	pGrid->SelectMakeList();
	
	INT	nRowCount	=	pGrid->SelectGetCount();
	
	pGrid->SelectGetTailPosition();
	
	if(0	>=	nRowCount)
	{
		SelfCloseMessageBox(_T("선정된 자료가 없습니다."));
		return	1;
	}
	
	if(1	<	nRowCount)
	{
		SelfCloseMessageBox(_T("한 개의 자료만 선정하여 주십시오."));
		return	2;
	}

	
	nIndex = pGrid->SelectGetIdx();
	if(0	>	nIndex) 
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -4, strFuncName);
	}

	CString sType;
	m_pParentMgr->GetDataMgrData(sDMAlias, _T("현황구분"), sType, nIndex);
	if(_T("지역") == sType || _T("통합") == sType)
	{
		SelfCloseMessageBox(sType + _T("현황은 예약상태를 확인할 수 없습니다."));
		return 3;
	}

	
	TCHAR *sFieldAlias[5] = 
	{
		 _T("단행/연속구분"), _T("책KEY"), _T("예약이용자수"), _T("종KEY"), _T("권호정보KEY")
	};
	CString sGetData[5];

	for(INT i = 0 ; i < 5 ; i++)
	{
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, sFieldAlias[i], sGetData[i], nIndex);
		if(0 > ids) return 5;
	}

	ReservationStatus(sGetData);

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SetBookStatusCnt_Unity()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SetBookStatusCnt_Unity");

	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT		nLoanCount		= 0; 
	INT		nUnityLoanCount	= 0; 
	INT		nTLLoanCount	= 0; 
	INT		nReserveCnt		= 0; 
	CString sAppendixYN		= _T(""); 
	CString sStatus			= _T(""); 
	CString sManageCode		= _T(""); 
	CString sUnmennedYN		= _T(""); 
	CString sL_Device		= _T(""); 
	CString sType			= _T("");	
	CString strHoldLibCode	= _T("");	
	const int nRow = pLoanDM->GetRowCount();
	for(INT i = 0; i < nRow; i++)
	{
		ids = pLoanDM->GetCellData(_T("대출상태"), i, sStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("부록여부"), i, sAppendixYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("관리구분"), i, sManageCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("무인대출예약상태"), i, sUnmennedYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("L_DEVICE"), i, sL_Device);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			ids = pLoanDM->GetCellData(_T("현황구분"), i, sType);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
		else
		{
			sType = _T("로컬");
		}
		if((_T("0") == sStatus || _T("2") == sStatus || _T("L") == sStatus || _T("D") == sStatus) 
			&& m_pParentMgr->m_pInfo->MANAGE_CODE == sManageCode && _T("KOLASIII-이동도서관") != sL_Device && _T("로컬") == sType)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nLoanCount++;
				}
			}
			else 
			{
				nLoanCount++;	
			}
		}
		
		CString strLILLLoanKey;
		pLoanDM->GetCellData(_T("타관대출KEY"), i, strLILLLoanKey);

		if(!strLILLLoanKey.IsEmpty() && sType != _T("로컬"))
		{
			nUnityLoanCount++;
		
			pLoanDM->GetCellData(_T("제공도서관부호"), i, strHoldLibCode);			
			if(m_pParentMgr->m_pInfo->LIB_CODE == strHoldLibCode)
			{
				if(_T("A") == sAppendixYN) 
				{ 
					
					if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
					{
						nLoanCount++;
					}
				}
				else 
				{
					nLoanCount++;	
				}
			}					
		}
		else
		{
			if((_T("0") == sStatus || _T("2") == sStatus) && _T("KOLASIII-이동도서관") != sL_Device)
			{
				if(_T("A") == sAppendixYN) 
				{ 
					
					if(FALSE == m_pParentMgr->m_pInfo->m_bKLUse)
					{
						if(m_pParentMgr->m_pInfo->MANAGE_CODE == sManageCode)
						{
							if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
							{
								nUnityLoanCount++;
							}
						}
					}
				}
				else 
				{
					nUnityLoanCount++;	
				}
			}
		}
				
		if((_T("0") == sStatus || _T("2") == sStatus || _T("L") == sStatus || _T("D") == sStatus) 
			&& m_pParentMgr->m_pInfo->MANAGE_CODE == sManageCode && _T("KOLASIII-이동도서관") == sL_Device && _T("로컬") == sType)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nTLLoanCount++;
				}
			}
			else 
			{
				nTLLoanCount++;	
			}
		}
		if(_T("3") == sStatus && sUnmennedYN.IsEmpty())
		{ 
			nReserveCnt++;
		}
	}

	CString sLoanCount, sUserKey;
	pMemberDM->GetCellData(_T("대출자KEY"), 0, sUserKey);
	pMemberDM->GetCellData(_T("총대출책수"), 0, sLoanCount); 
	
	INT nTotalLoanCount = nUnityLoanCount;
	if(_ttoi(sLoanCount) != nTotalLoanCount)
	{
		sLoanCount.Format(_T("%d"), nTotalLoanCount); 
		pMemberDM->StartFrame();
		pMemberDM->InitDBFieldData();
		
		if(1 > _ttoi(sLoanCount))
		{
			sLoanCount = _T("0");
		}
		pMemberDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanCount, TRUE);
		pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("대출책수UPDATE"), __WFILE__, __LINE__), TRUE);
		pMemberDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
		pMemberDM->SendFrame();
		pMemberDM->EndFrame();
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT")));
		ShareInfoCenterUpload(_T("N"), _T("U"));
	}

	INT nMaxLoanBookCnt;
	ids = m_pCheckLoan->GetIndividualLoanBookCnt(nMaxLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewLoanBookCnt;
	sViewLoanBookCnt.Format(_T("%d/%d"), nLoanCount, nMaxLoanBookCnt);
	
	sLoanCount.Format(_T("%d"), nLoanCount);
	
	CString sIsLoanBookCnt;
	sIsLoanBookCnt.Format(_T("%d"), nMaxLoanBookCnt - nLoanCount);
	
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총대출책수"), sLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출책수상태"), sViewLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출가능책수"), sIsLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	
	nMaxLoanBookCnt = 0;
	
	CString strKLUserYN;
	pMemberDM->GetCellData(_T("통합회원여부"), 0, strKLUserYN);
	if(_T("Y") != strKLUserYN) strKLUserYN = _T("N");
	if(_T("Y") == m_pManageValue->m_sIsUnityLoanService && _T("Y") == strKLUserYN && 10000 != m_pParentMgr->m_pInfo->MODE)
	{
		if(m_strMaxLoanCount.IsEmpty())
		{
		
			CString strGetData = _T("");
			GetTempData(_T("기본통합대출가능권수"), strGetData);
			if(strGetData.IsEmpty())
			{
				m_strMaxLoanCount = _T("20");
			}
			else
			{
				m_strMaxLoanCount = strGetData;
			}
		}
		nMaxLoanBookCnt = _ttoi(m_strMaxLoanCount);
	}
	else
	{
		ids = m_pCheckLoan->GetUnityLoanBookCnt(nMaxLoanBookCnt);
		if(0 > ids) return ids;
	}
	
	sViewLoanBookCnt = _T("");
	sViewLoanBookCnt.Format(_T("%d/%d"), nUnityLoanCount, nMaxLoanBookCnt);
	
	sLoanCount.Format(_T("%d"), nUnityLoanCount);
	
	sIsLoanBookCnt = _T("");
	sIsLoanBookCnt.Format(_T("%d"), nMaxLoanBookCnt - nUnityLoanCount);
	
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합총대출책수"), sLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합대출책수상태"), sViewLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합대출가능책수"), sIsLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);	
	
	INT nMaxTLLoanBookCnt;
	ids = m_pCheckLoan->GetTLLoanBookCnt(nMaxTLLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewTLLoanBookCnt;
	
	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
		sViewTLLoanBookCnt.Format(_T("%d/%d"), nTLLoanCount, nMaxTLLoanBookCnt);
	}
	else
	{
		sViewTLLoanBookCnt.Format(_T("%d"), nTLLoanCount);
	}

	CString sIsTLLoanBookCnt;
	sIsTLLoanBookCnt.Format(_T("%d"), nMaxTLLoanBookCnt - nTLLoanCount);
	sLoanCount.Format(_T("%d"), nTLLoanCount);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("이동총대출책수"), sLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("이동대출책수상태"), sViewTLLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("이동대출가능책수"), sIsTLLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

	CString sReserveCnt;
	pMemberDM->GetCellData(_T("총예약책수"), 0, sReserveCnt); 
	
	if(_ttoi(sReserveCnt) != nReserveCnt)
	{
		sReserveCnt.Format(_T("%d"), nReserveCnt); 
		pMemberDM->StartFrame();
		pMemberDM->InitDBFieldData();
		pMemberDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sReserveCnt, TRUE);
		pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약책수UPDATE"), __WFILE__, __LINE__), TRUE);
		pMemberDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
		pMemberDM->SendFrame();
		pMemberDM->EndFrame();
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_COUNT")));
		ShareInfoCenterUpload(_T("N"), _T("U"));
	}
	
	INT nMaxReservationBookCnt;
	ids = m_pCheckLoan->GetReserveBookCnt(nMaxReservationBookCnt);
	if(0 > ids) return ids;
	
	CString sViewReserveBookCnt;
	sViewReserveBookCnt.Format(_T("%d/%d"), nReserveCnt, nMaxReservationBookCnt);
	
	CString sIsReserveBookCnt;
	sIsReserveBookCnt.Format(_T("%d"), nMaxReservationBookCnt - nReserveCnt);

	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총예약책수"), sReserveCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("예약책수상태"), sViewReserveBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("예약가능책수"), sIsReserveBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);


	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::SetBookStatusCnt()
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("SetBookStatusCnt");

	CESL_DataMgr *pLoanDM  = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pLoanDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr* pMemberDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	if(pMemberDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	INT		nLoanCount		= 0; 
	INT		nOtherLoanCount	= 0; 
	INT		nTLLoanCount	= 0; 
	INT		nReserveCnt		= 0; 
	CString sAppendixYN		= _T(""); 
	CString sStatus			= _T(""); 
 	CString sManageCode		= _T(""); 
	CString sUserManageCode	= _T(""); 
	CString sUnmennedYN		= _T(""); 
	CString sL_Device		= _T(""); 
	CString sType			= _T("");

	
	ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("관리구분"), sUserManageCode, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);


	const int nRow = pLoanDM->GetRowCount();
	for(INT i = 0; i < nRow; i++)
	{
		ids = pLoanDM->GetCellData(_T("대출상태"), i, sStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("부록여부"), i, sAppendixYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
 		ids = pLoanDM->GetCellData(_T("관리구분"), i, sManageCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("무인대출예약상태"), i, sUnmennedYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pLoanDM->GetCellData(_T("L_DEVICE"), i, sL_Device);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if((_T("0") == sStatus || _T("2") == sStatus) && sUserManageCode == sManageCode && _T("KOLASIII-이동도서관") != sL_Device)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nLoanCount++;
				}
			}
			else 
			{
				nLoanCount++;	
			}
		}
		
		if(((_T("0") == sStatus || _T("2") == sStatus) && sUserManageCode != sManageCode) ||
			(_T("L") == sStatus || _T("D") == sStatus) && _T("KOLASIII-이동도서관") != sL_Device)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nOtherLoanCount++;
				}
			}
			else 
			{
				nOtherLoanCount++;	
			}
		}

		if((_T("0") == sStatus || _T("2") == sStatus) && sUserManageCode == sManageCode && _T("KOLASIII-이동도서관") == sL_Device)
		{
			if(_T("A") == sAppendixYN) 
			{ 
				if(0 == m_pCheckLoan->IsAppendixIncludeLoanCnt())
				{
					nTLLoanCount++;
				}
			}
			else 
			{
				nTLLoanCount++;	
			}
		}
		
		if(_T("3") == sStatus && sUnmennedYN.IsEmpty())
		{ 
			nReserveCnt++;
		}
	}

	CString sLoanCount, sUserKey;
	pMemberDM->GetCellData(_T("대출자KEY"), 0, sUserKey);
	pMemberDM->GetCellData(_T("총대출책수"), 0, sLoanCount); 
	
	INT nTotalLoanCount = nLoanCount + nOtherLoanCount;
	if(_ttoi(sLoanCount) != nTotalLoanCount)
	{
		sLoanCount.Format(_T("%d"), nTotalLoanCount); 
		pMemberDM->StartFrame();
		pMemberDM->InitDBFieldData();
		
		if(1 > _ttoi(sLoanCount))
		{
			sLoanCount = _T("0");
		}
		pMemberDM->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), sLoanCount, TRUE);
		pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("대출책수UPDATE"), __WFILE__, __LINE__), TRUE);
		pMemberDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
		pMemberDM->SendFrame();
		pMemberDM->EndFrame();
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT")));
		ShareInfoCenterUpload(_T("N"), _T("U"));
	}

	INT nMaxLoanBookCnt;
	ids = m_pCheckLoan->GetLoanBookCnt(nMaxLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewLoanBookCnt;
	sViewLoanBookCnt.Format(_T("%d/%d"), nLoanCount, nMaxLoanBookCnt);
	
	CString sIsLoanBookCnt;
	sIsLoanBookCnt.Format(_T("%d"), nMaxLoanBookCnt - nLoanCount);
	sLoanCount.Format(_T("%d"), nLoanCount);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총대출책수"), sLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출책수상태"), sViewLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출가능책수"), sIsLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);	
	
	INT nMaxOtherLoanBookCnt;	
	ids = m_pCheckLoan->GetOtherLoanBookCnt(nMaxOtherLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewOtherLoanBookCnt;
	sViewOtherLoanBookCnt.Format(_T("%d/%d"), nOtherLoanCount, nMaxOtherLoanBookCnt);
	
	CString sIsOtherLoanBookCnt;
	sIsOtherLoanBookCnt.Format(_T("%d"), nMaxOtherLoanBookCnt - nOtherLoanCount);
	CString sOtherLoanCount;
	sOtherLoanCount.Format(_T("%d"), nOtherLoanCount);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("타관총대출책수"), sOtherLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("타관대출책수상태"), sViewOtherLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("타관대출가능책수"), sIsOtherLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);

	INT nMaxTLLoanBookCnt;
	ids = m_pCheckLoan->GetTLLoanBookCnt(nMaxTLLoanBookCnt);
	if(0 > ids) return ids;
	
	CString sViewTLLoanBookCnt;
	
	if(m_pParentMgr->m_pInfo->MODE == 10000)
	{
		sViewTLLoanBookCnt.Format(_T("%d/%d"), nTLLoanCount, nMaxTLLoanBookCnt);
	}
	else
	{
		sViewTLLoanBookCnt.Format(_T("%d"), nTLLoanCount);
	}	
	
	CString sIsTLLoanBookCnt;
	sIsTLLoanBookCnt.Format(_T("%d"), nMaxTLLoanBookCnt - nTLLoanCount);
	CString sTLLoanCount;
	sTLLoanCount.Format(_T("%d"), nTLLoanCount);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("이동총대출책수"), sTLLoanCount, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("이동대출책수상태"), sViewTLLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("이동대출가능책수"), sIsTLLoanBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

	CString sReserveCnt;
	pMemberDM->GetCellData(_T("총예약책수"), 0, sReserveCnt); 
	
	if(_ttoi(sReserveCnt) != nReserveCnt)
	{
		sReserveCnt.Format(_T("%d"), nReserveCnt); 
		pMemberDM->StartFrame();
		pMemberDM->InitDBFieldData();
		pMemberDM->AddDBFieldData(_T("RESERVATION_COUNT"), _T("NUMERIC"), sReserveCnt, TRUE);
		pMemberDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParentMgr->GetWorkLogMsg(_T("예약책수UPDATE"), __WFILE__, __LINE__), TRUE);
		pMemberDM->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), sUserKey);
		pMemberDM->SendFrame();
		pMemberDM->EndFrame();
		
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sUserKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_COUNT")));
		ShareInfoCenterUpload(_T("N"), _T("U"));
	}
	
	INT nMaxReservationBookCnt;
	ids = m_pCheckLoan->GetReserveBookCnt(nMaxReservationBookCnt);
	if(0 > ids) return ids;
	
	CString sViewReserveBookCnt;
	sViewReserveBookCnt.Format(_T("%d/%d"), nReserveCnt, nMaxReservationBookCnt);
	
	CString sIsReserveBookCnt;
	sIsReserveBookCnt.Format(_T("%d"), nMaxReservationBookCnt - nReserveCnt);

	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("총예약책수"), sReserveCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("예약책수상태"), sViewReserveBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
	ids = m_pParentMgr->SetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("예약가능책수"), sIsReserveBookCnt, 0);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);


	return 0;

EFS_END
return -1;
}

VOID CBL_LOC_LOAN_PROC::SetBEGIN_SBL()
{
	BEGIN_SBL();
}

VOID CBL_LOC_LOAN_PROC::SetEND_SBL(INT nRowCount)
{
	END_SBL(nRowCount);
}
INT CBL_LOC_LOAN_PROC::GetBookManageCode(CString &strBookManageCode)
{
	CString strValue;
	m_pManageValue->GetManageValue(_T("열람"), _T("공통"), _T("등록구분"), _T("도서관별 등록구분 중복여부"), strValue);
	if(_T("N") == strValue || TRUE == strValue.IsEmpty())
	{
		strBookManageCode = m_pParentMgr->m_pInfo->MANAGE_CODE;
		return 0;
	}
	else
	{
		INT ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("책관리구분"), strBookManageCode);	
		return ids;
	}
	return 0;
}
INT CBL_LOC_LOAN_PROC::AddIdxQueryFrame(CESL_DataMgr *pDM, INT type, CString sSpeciesKey)
{
	EFS_BEGIN	

 	return 0;
	
	if((m_pParentMgr->m_pInfo->MODE == 10000) || (m_pParentMgr->m_pInfo->MODE == 30000))
		return 0;

	CString sIDXTblName, sProcName;
	switch(type)
	{
		case 1 :
			sIDXTblName = _T("IDX_BO_TBL");			
			sProcName = _T("TESTWORKSTATUS");
			break;
		case 2 :
			sIDXTblName = _T("IDX_SE_TBL");			
			sProcName = _T("TESTSEWORKSTATUS");
			break;
		default :
			return -1;
			break;
	}
	

	CString sQuery;
	sQuery.Format(_T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;"),sIDXTblName, sProcName, sSpeciesKey, sSpeciesKey);	
	pDM->AddFrame(sQuery);

	return 0;

	EFS_END

	return -1;
}


CString CBL_LOC_LOAN_PROC::getLimitWord(TCHAR* szToken, INT nCutCnt, BOOL bIsAddSpace)
{
	
	
	CharConvert cv;
	CHAR*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte(szToken, &szTmp, &len);
	if(0 < len)
	{


	}
	if(szTmp == NULL)
	{
		CString sTemp = _T("");
		for(INT i = 0; i < nCutCnt; i++)
		{
			sTemp += _T(" ");
		}
		return sTemp;
	}

	BOOL bIsCut = FALSE;

	INT nTokenLen = 0 ;
	INT nCompIndexCnt = 0 ;

	CString sReturnWord = _T("");
	CString sPeriods = _T("…");
	CHAR szPeriods[5];
	sprintf(szPeriods, "%s", "…");

	CHAR chComp = '\0' ;
	CHAR chExtra = '\0' ;

	CHAR szTmpWord[3] ; 
	CHAR szReturnWord[100] = "" ; 	

	nTokenLen = strlen(szTmp);

	if(nTokenLen > nCutCnt)
	{
		bIsCut = TRUE;
		nCutCnt -= 3;
	}

	for (INT i = 0 ; i < nCutCnt ; i++)
	{
		if(i > (nTokenLen-1))
		{
			if(bIsAddSpace)
			{
				chComp = ' ';
				sprintf (szTmpWord, "%c", chComp);
				strcat (szReturnWord, szTmpWord);
				continue;
			}
			else
				break;
		}
		else
		{
			chComp = szTmp[i];
			chExtra = szTmp[i + 1];
		}

		
		if(chComp & 0x80)
		{
			if((i + 2) > nCutCnt)
			{				
				sprintf(szPeriods, "%s", "... ");
				break;
			}

			sprintf (szTmpWord, "%c%c", chComp, chExtra) ;			
			strcat (szReturnWord, szTmpWord) ;
			i++;
		}
		
		else
		{
			sprintf (szTmpWord, "%c", chComp) ;
			strcat (szReturnWord, szTmpWord) ;
		}
	}

	if(bIsCut)
	{
		sprintf(szTmpWord, "%s", szPeriods);
		strcat(szReturnWord, szTmpWord);
	}
	
	if(0 < len)
	{
		delete [] szTmp;
	}

	
	
	TCHAR* pszDest = NULL;
	cv.MultiByteToUnicode(szReturnWord, &pszDest, &len);	
	sReturnWord.Format(_T("%s"), pszDest);

	if(0 < len)
	{
		delete [] pszDest;
	}

	return sReturnWord;
}

INT CBL_LOC_LOAN_PROC::StartLog(CString strLogTitle)
{
	CFileFind filefind;
	if(!filefind.FindFile(_T("..\\cfg\\_SpeedCheck"))){return 0;}

	m_strLogTitle = strLogTitle;
	m_nSTART = GetTickCount();
	return 0;
}

INT CBL_LOC_LOAN_PROC::EndLog()
{
	CFileFind filefind;
	if(!filefind.FindFile(_T("..\\cfg\\_SpeedCheck"))){return 0;}

	m_nFINISH = GetTickCount();
	double timeSec;
	timeSec = (m_nFINISH - m_nSTART) / 1000.0;
	CString strLog;
	SYSTEMTIME t;
	GetLocalTime(&t);
	strLog.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d.%03d][%s] - %g sec"), t.wYear,t.wMonth,t.wDay,t.wHour,t.wMinute,t.wSecond,t.wMilliseconds, m_strLogTitle, timeSec);
	WriteLog(strLog);
	return 0;
}

INT CBL_LOC_LOAN_PROC::WriteLog(CString strLog)
{
	CFileFind filefind;
	if(!filefind.FindFile(_T("..\\cfg\\_SpeedCheck"))){return 0;}

	CString filename;
	filename = filename + _T("..\\tunning\\SpeedCheck.txt");
	FILE *fp;	
	fp = _tfopen(filename, _T("a+b"));
	if(NULL == fp){return 0;}
	fseek(fp, 0, SEEK_END);		
	if(0 == ftell(fp))
	{
		fputc(0xFF, fp);
		fputc(0xFE, fp);
	}	
	_ftprintf(fp, _T("%s\r\n"), strLog);
	fclose(fp);
	return 0;
}

LRESULT CBL_LOC_LOAN_PROC::ReceiptChangeTab(WPARAM wParam, LPARAM lParam)
{
	INT nStatus = lParam;
	m_ReceiptDlg->CheckTab(nStatus);

	return 0;
}



INT CBL_LOC_LOAN_PROC::ExcuteUNDO()
{
EFS_BEGIN

	CString strFuncName = _T("ExcuteUNDO");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pCM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(NULL == pGrid) 
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(NULL == pDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nIndex,ids=-1;
	CString sRecKey,sStatus,sTempStatus,sManageCode,sLDevice;
	if(0 == nRowCount)
	{
		SelfCloseMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	
	for(INT i = 0; i < nRowCount; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) 
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}

		ids = pDM->GetCellData(_T("대출상태"), nIndex, sStatus);
		ids = pDM->GetCellData(_T("관리구분"), nIndex, sManageCode);
		ids = pDM->GetCellData(_T("L_DEVICE"), nIndex, sLDevice);

		
		CString sOtherLoanKey;
		ids = pDM->GetCellData(_T("타관대출KEY"),nIndex, sOtherLoanKey);
		
		if(sManageCode != m_pParentMgr->m_pInfo->MANAGE_CODE)
		{
			
			if(sOtherLoanKey.IsEmpty())
			{
				SelfCloseMessageBox(_T("다른 도서관의 자료가 있습니다.\r\n취소할 수 없습니다."));
				return 0;
			}
		}
		if(0 != i)
		{
			if(sTempStatus != sStatus)
			{
				SelfCloseMessageBox(_T("선택된 자료의 상태(대출/반납연기/예약)가\r\n일치하는 경우에만 사용 가능합니다."));
				return 0;
			}
		}

		
		if(_T("0") == sStatus || _T("2") == sStatus)
		{
			if(sOtherLoanKey.IsEmpty())
			{
				if(	_T("KOLASIII") == sLDevice || _T("KOLASIII-이동도서관") == sLDevice ||
					_T("KOLASYS") == sLDevice  || _T("SMARTID") == sLDevice )					
				{
				}
				else
				{
					SelfCloseMessageBox(_T("KOLASIII에서 대출한 자료만 취소 가능합니다."));
					return 0;
				}
			}
		}

		sTempStatus=sStatus;
		pGrid->SelectGetPrev();
	}

	
	CString sOtherLoanKey;
	ids = pDM->GetCellData(_T("타관대출KEY"),nIndex, sOtherLoanKey);
	
	if(_T("0") == sStatus)
	{
		if(!sOtherLoanKey.IsEmpty())
		{
			ids = UNDO_OtherLoan();
			if(0 == ids)
			{
				SelfCloseMessageBox(_T("타관대출이 취소되었습니다."));
			}
		}
		else
		{
			ids = UNDO_Loan();
			if(0 == ids)
			{
				SelfCloseMessageBox(_T("대출이 취소 되었습니다."));
			}
		}
	}	
	else if(_T("2") == sStatus)
	{
		ids = UNDO_Postphonement_Return();
		if(0 == ids)
		{
			SelfCloseMessageBox(_T("반납 연기가 취소 되었습니다."));
		}
		else if(15 == ids && 1 != nRowCount)
		{
			SelfCloseMessageBox(_T("반납 연기가 취소 되었습니다.\r\n(이미 1번 이상 반납연기 취소가 수행된 자료가 있습니다.)"));
		}
		else if(15 == ids && 1 == nRowCount)
		{
			SelfCloseMessageBox(_T("이미 1번 이상 반납연기 취소가 수행된 자료입니다."));
		}
	}
	else if(_T("3") == sStatus)
	{
		ids = UNDO_Reservation();
		if(0 == ids)
		{
			SelfCloseMessageBox(_T("예약이 취소 되었습니다."));
		}
	}
	else if(_T("L") == sStatus || _T("D") == sStatus)
	{
		SelfCloseMessageBox(_T("책두레대출 자료는 지원되지 않습니다."));
	}
	else if(_T("R") == sStatus)
	{
		SelfCloseMessageBox(_T("책두레반납 자료는 지원되지 않습니다."));
	}
	else if(_T("O") == sStatus)
	{
		SelfCloseMessageBox(_T("책두레예약 자료는 지원되지 않습니다."));
	}
	return ids;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UNDO_Loan()
{
EFS_BEGIN
	
	INT ids = 0;
	CString strFuncName = _T("UNDO_Loan");

	CString sMsg;
	sMsg.Format(_T("\r\n선정된 대출 자료를 취소하시겠습니까?\r\n"));
	if(IDYES!=MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("대출 취소"), MB_YESNO | MB_ICONQUESTION))
	{
		return 5;
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nIndex;
	CString sCLRecKey, sCLURecKey, sBookKey, sReservationDate, strPublishFormCode, strRegNo, sQuery, strSpeciesKey;

	for(INT i = 0 ; i < nRowCount ;i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

 		
 		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 		{
 			KL_StartTemp();
 		}

		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);
		pMobileApi->StartFrame();
		pMobileApi->InitDBFieldData();
		
		ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, sCLRecKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("책KEY"), nIndex, sBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sCLURecKey, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("예약일"), nIndex, sReservationDate);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("단행/연속구분"), nIndex, strPublishFormCode);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("등록번호"), nIndex, strRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		ids = pDM->GetCellData(_T("종KEY"), nIndex, strSpeciesKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		CString strLastWork;
		if(sReservationDate.IsEmpty()) 
		{
			
			pMobileApi->MakeDeleteFrame(_T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"),sCLRecKey);
			
			pMobileApi->InitDBFieldData();
			
			CString strLoanCount;
			strLoanCount.Format(_T("(SELECT DECODE(SIGN(NVL(LOAN_COUNT,0)-1),1,LOAN_COUNT-1,0) ")
								_T("FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s)"), sCLURecKey);
			pMobileApi->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), strLoanCount);

			CString strTmp;
	
			strTmp.Format(_T("(SELECT MAX(LOAN_DATE) FROM LS_WORK_T01 WHERE USER_KEY=%s) "), sCLURecKey);
			
			
			pMobileApi->AddDBFieldData(_T("LAST_LOAN_DATE"),_T("NUMERIC"),strTmp);
			strLastWork = m_pParentMgr->GetWorkLogMsg(_T("대출(UNDO)"), __WFILE__, __LINE__);
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"), strLastWork);
			pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"),_T("REC_KEY"),_T("NUMERIC"),sCLURecKey);

 			
 			if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 			{
 				
 				CString strGetData;
 				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
 				if(_T("Y") == strGetData)
 				{
 					CString strLoanLibCode;
 					ids = pDM->GetCellData(_T("대출도서관부호"), nIndex, strLoanLibCode);
 					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
 					if(strLoanLibCode.IsEmpty() || strLoanLibCode == m_pParentMgr->m_pInfo->LIB_CODE)
 					{
						strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
 						CString strUserNo;
 						ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);
 						if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
 
 						CFrameWriter Writer(m_pParentMgr->m_pInfo);
 						Writer.setCommand(_T("IL_K21_LOC_B01_SERVICE"));
 						Writer.addRecord();
 						Writer.addElement(_T("대출도서관부호"), strLoanLibCode);
 						Writer.addElement(_T("통합대출자번호"), strUserNo);
						Writer.addElement(_T("로컬KEY"), sCLRecKey);
  						CString strFrame = Writer.getFrame();
  						KL_AddTemp(strFrame);
 					}
 				}
 			}

			
			SetTempData(SHARE_CO_LOAN_TBL_DELETE_REC_KEY, CComVariant(sCLRecKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sCLURecKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT,LAST_LOAN_DATE")));
			ShareInfoCenterUpload(_T("D"), _T("U"));
		}
		else if(!sReservationDate.IsEmpty())
		{
			
			pMobileApi->AddDBFieldData(_T("STATUS"),_T("STRING"),_T("3"));
			pMobileApi->AddDBFieldData(_T("LOAN_DATE"),_T("NUMERIC"),_T("NULL"));
			pMobileApi->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T("NULL"));
			pMobileApi->AddDBFieldData(_T("L_DEVICE"),_T("NUMERIC"),_T("NULL"));
			
			pMobileApi->AddDBFieldData(_T("L_WORKER"),_T("NUMERIC"),_T("NULL"));
			
			pMobileApi->AddDBFieldData(_T("LOAN_TYPE_CODE"),_T("NUMERIC"),_T("NULL"));
			strLastWork = m_pParentMgr->GetWorkLogMsg(_T("예약대출(UNDO)"), __WFILE__, __LINE__);
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"), strLastWork);
			pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"),sCLRecKey);

			
			pMobileApi->InitDBFieldData();
			
			CString strLoanCount;
			strLoanCount.Format(_T("(SELECT DECODE(SIGN(NVL(LOAN_COUNT,0)-1),1,LOAN_COUNT-1,0) ")
								_T("FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s)"), sCLURecKey);
			pMobileApi->AddDBFieldData(_T("LOAN_COUNT"), _T("NUMERIC"), strLoanCount);

			pMobileApi->AddDBFieldData(_T("RESERVATION_COUNT"),_T("NUMERIC"),_T("RESERVATION_COUNT+1"));
			CString strTmp;
			
			strTmp.Format(_T("(SELECT MAX(LOAN_DATE) FROM LS_WORK_T01 WHERE USER_KEY=%s AND REC_KEY NOT IN (%s))"), sCLURecKey, sCLRecKey);			

			pMobileApi->AddDBFieldData(_T("LAST_LOAN_DATE"),_T("NUMERIC"),strTmp);
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("대출(UNDO)"), __WFILE__, __LINE__));
			pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"),_T("REC_KEY"),_T("NUMERIC"),sCLURecKey);

 			
 			if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 			{
 				
 				CString strGetData;
 				ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
 				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 				if(_T("Y") == strGetData)
 				{
 					CString strLoanLibCode;
 					ids = pDM->GetCellData(_T("대출도서관부호"), nIndex, strLoanLibCode);
 					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 					if(strLoanLibCode.IsEmpty() || strLoanLibCode == m_pParentMgr->m_pInfo->LIB_CODE)
 					{
						strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
 						CString strUserNo;
 						ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);
 						if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
 
 						CFrameWriter Writer(m_pParentMgr->m_pInfo);
 						Writer.setCommand(_T("IL_K21_LOC_B01_SERVICE"));
 						Writer.addRecord();
 						Writer.addElement(_T("대출도서관부호"), strLoanLibCode);
 						Writer.addElement(_T("통합대출자번호"), strUserNo);
						Writer.addElement(_T("로컬KEY"), sCLRecKey);
  						CString strFrame = Writer.getFrame();
  						KL_AddTemp(strFrame);
 					}
 				}
 			}
			
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sCLRecKey));
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,LOAN_DATE,RETURN_PLAN_DATE,L_DEVICE,L_WORKER,LOAN_TYPE_CODE")));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sCLURecKey));
			SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("LOAN_COUNT,RESERVATION_COUNT,LAST_LOAN_DATE")));
			ShareInfoCenterUpload(_T("U"), _T("U"));
		}
		
		CString strTblName;
		CString strWorkingStatus;
		if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode)
		{
			strTblName = _T("BO_BOOK_TBL");
		}
		else if(_T("SE") == strPublishFormCode || _T("연속") == strPublishFormCode)
		{
			strTblName = _T("SE_BOOK_TBL");
		}
		
		else if(_T("NB") == strPublishFormCode || _T("긴급") == strPublishFormCode)
		{
			strTblName = _T("CO_NON_DB_BOOK_TBL");
		}
		
		sQuery.Format(
			_T("SELECT WORKING_STATUS FROM %s WHERE REC_KEY = %s"), 
			strTblName, sBookKey);		
		ids = pDM->GetOneValueQuery(sQuery,strWorkingStatus);

		if(FALSE == strTblName.IsEmpty())
		{
			pMobileApi->InitDBFieldData();
			pMobileApi->AddDBFieldData(_T("WORKING_STATUS"),_T("NUMERIC"),_T("PREV_WORKING_STATUS"));
			pMobileApi->AddDBFieldData(_T("PREV_WORKING_STATUS"),_T("STRING"),strWorkingStatus);
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("대출(UNDO)"), __WFILE__, __LINE__));
			pMobileApi->MakeUpdateFrame(strTblName,_T("REC_KEY"),_T("NUMERIC"),sBookKey);

			
			int nBookType = -1;
			if(_T("BO_BOOK_TBL") == strTblName)
			{
				nBookType = 1;
			}
			else if(_T("SE_BOOK_TBL") == strTblName)
			{
				nBookType = 2;
			}
			else if(_T("CO_NON_DB_BOOK_TBL") == strTblName)
			{
				nBookType = 0;
			}			
			AddIdxQueryFrame(pDM, nBookType, strSpeciesKey);
		}

		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);

		pMobileApi->EndFrame();
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
		
 		
 		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
 		{
 			KL_EndTemp(strLastWork);
 		}

		pGrid->SelectGetPrev();
	}

	m_bReturnBookNonFree=TRUE;
	SPFExcutePath(_T("회원리로드_NOASK"));
	
	return 0;

EFS_END
return -1;
}



INT CBL_LOC_LOAN_PROC::UNDO_Postphonement_Return()
{
EFS_BEGIN

	CString strFuncName = _T("UNDO_Postphonement_Return");

	CString sMsg;
	sMsg.Format(_T("\r\n선정된 반납연기 자료를 취소하시겠습니까?\r\n"));
	if(IDYES!=MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("반납연기 취소"), MB_YESNO | MB_ICONQUESTION))
	{
		return 5;
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nIndex,ids=-1;
	CString sCLRecKey,sCLURecKey,sBookKey,sQuery,sDelayCnt,sReturnDate,sPreReturnDate;
	
	BOOL bIncludePassData = FALSE;

	for(INT i = 0 ; i < nRowCount ;i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);
		pMobileApi->StartFrame();
		pMobileApi->InitDBFieldData();

		ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, sCLRecKey);
		ids = pDM->GetCellData(_T("책KEY"), nIndex, sBookKey);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sCLURecKey, 0);
		ids = pDM->GetCellData(_T("반납연기횟수"), nIndex, sDelayCnt);
		ids = pDM->GetCellData(_T("반납예정일"), nIndex, sReturnDate);

		CESL_DataMgr tempDM;
		tempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
		sQuery.Format(_T("SELECT PRE_RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE REC_KEY = %s"),sCLRecKey);
		tempDM.GetOneValueQuery(sQuery,sPreReturnDate);

		
		
		if(sPreReturnDate==sReturnDate) 
		{
			bIncludePassData=TRUE;
			pGrid->SelectGetPrev();
			pDM->EndFrame();
			continue;
		}

		CString strStatus;
		if(sDelayCnt==_T("1"))
		{
			
			strStatus = _T("0");
			pMobileApi->AddDBFieldData(_T("STATUS"),_T("STRING"),_T("0"));
			pMobileApi->AddDBFieldData(_T("DELAY_CNT"),_T("NUMERIC"),_T("DELAY_CNT-1"));
			pMobileApi->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T("PRE_RETURN_PLAN_DATE"));

			
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sCLRecKey));
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,DELAY_CNT,RETURN_PLAN_DATE")));
			ShareInfoCenterUpload(_T("U"), _T("N"));
		}
		else
		{
			
			strStatus = _T("2");
			pMobileApi->AddDBFieldData(_T("STATUS"),_T("STRING"),_T("2"));
			pMobileApi->AddDBFieldData(_T("DELAY_CNT"),_T("NUMERIC"),_T("DELAY_CNT-1"));
			pMobileApi->AddDBFieldData(_T("RETURN_PLAN_DATE"),_T("NUMERIC"),_T("PRE_RETURN_PLAN_DATE"));

			
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_REC_KEY, CComVariant(sCLRecKey));
			SetTempData(SHARE_CO_LOAN_TBL_UPDATE_FIELDS, CComVariant(_T("STATUS,DELAY_CNT,RETURN_PLAN_DATE")));
			ShareInfoCenterUpload(_T("U"), _T("N"));
		}

		
		CString strLastWork;
		strLastWork = m_pParentMgr->GetWorkLogMsg(_T("반납연기(UNDO)"), __WFILE__, __LINE__);
		pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"), strLastWork);
		pMobileApi->MakeUpdateFrame(_T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"),sCLRecKey);

		ids = pMobileApi->SendFrame();
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, _T("UNDO_Postphonement_Return"));
		}

		pMobileApi->EndFrame();
		CLocCommonAPI::MobileApiEndLog(pMobileApi);

		
		if(_T("Y") == m_pManageValue->m_sIsUnityLoanService)
		{
			
			CString strGetData;
			ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("통합회원여부"), strGetData, 0);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			if(_T("Y") == strGetData)
			{
				CString strLoanLibCode;
				ids = pDM->GetCellData(_T("대출도서관부호"), nIndex, strLoanLibCode);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
 				pDM->GetCellData(_T("관리구분"), nIndex, strGetData);
				if(strGetData == m_pParentMgr->m_pInfo->MANAGE_CODE)			
				{
					strLoanLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
					CString strUserNo;
					ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), strUserNo, 0);
					if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

					CFrameWriter Writer(m_pParentMgr->m_pInfo);
					Writer.setCommand(_T("IL_K23_LOC_B01_SERVICE"));
					Writer.addRecord();
					Writer.addElement(_T("대출도서관"), strLoanLibCode);
					Writer.addElement(_T("통합대출자번호"), strUserNo);
					Writer.addElement(_T("반납예정일"), sPreReturnDate);
					Writer.addElement(_T("상태"), strStatus);
					Writer.addElement(_T("로컬KEY"), sCLRecKey);
 					CString strFrame = Writer.getFrame();
 					KL_StartTemp();
 					KL_AddTemp(strFrame);
 					KL_EndTemp(m_pParentMgr->GetWorkLogMsg(_T("통합회원반납연기취소"), __WFILE__, __LINE__));
				}
			}
		}

		pGrid->SelectGetPrev();
	}


	m_bReturnBookNonFree=TRUE;
	SPFExcutePath(_T("회원리로드_NOASK"));

	if(bIncludePassData)
	{
		return 15;
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::UNDO_Reservation()
{
EFS_BEGIN

	CString strFuncName = _T("UNDO_Reservation");

	CString sMsg;
	sMsg.Format(_T("\r\n선정된 예약 자료를 취소하시겠습니까?\r\n"));
	if(IDYES!=MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("예약 취소"), MB_YESNO | MB_ICONQUESTION))
	{
		return 5;
	}

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_LOAN_INFO"));
	if(pCM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MainGrid"));
	if(pGrid == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	if(pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	pGrid->SelectMakeList();
	INT nRowCount = pGrid->SelectGetCount();
	pGrid->SelectGetTailPosition();

	INT nIndex,ids=-1;
	CString sCLRecKey,sCLURecKey,sBookKey,sQuery,strPublishFormCode;

	for(INT i = 0 ; i < nRowCount ;i++)
	{
		nIndex = pGrid->SelectGetIdx();
		if(nIndex < 0) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		if(_T("Y") == m_pManageValue->m_sUnmannedReserveSys)
		{
			CString strUnmanned;
			ids = pDM->GetCellData(_T("무인대출예약상태"), nIndex, strUnmanned);
			if(_T("Y") ==strUnmanned)
			{ 
				sMsg.Format(_T("무인예약신청 자료입니다.\r\n취소하시겠습니까?\r\n"));
				if(IDYES != MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("예약 취소"), MB_YESNO|MB_ICONQUESTION))
				{
					return 5;
				}
			} 
			else if(_T("O") == strUnmanned)
			{
				sMsg.Format(_T("무인예약대기 자료는 지원되지 않습니다."));
				MessageBox(m_pParentMgr->m_hWnd, sMsg, _T("예약 취소"), MB_ICONINFORMATION);
				return 5;
			}
		}

		CKolas2up_MobileApi *pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, pDM);
		pMobileApi->StartFrame();
		pMobileApi->InitDBFieldData();

		ids = pDM->GetCellData(_T("대출/반납정보KEY"), nIndex, sCLRecKey);
		ids = pDM->GetCellData(_T("책KEY"), nIndex, sBookKey);
		ids = pDM->GetCellData(_T("단행/연속구분"), nIndex, strPublishFormCode);
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자KEY"), sCLURecKey, 0);

		
		pMobileApi->MakeDeleteFrame(_T("LS_WORK_T01"),_T("REC_KEY"),_T("NUMERIC"),sCLRecKey);

		
		pMobileApi->AddDBFieldData(_T("RESERVATION_COUNT"),_T("NUMERIC"),_T("RESERVATION_COUNT-1"));
		pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("예약(UNDO)"), __WFILE__, __LINE__));
		pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"),_T("REC_KEY"),_T("NUMERIC"),sCLURecKey);

		
		pMobileApi->InitDBFieldData();
		if(_T("MO") == strPublishFormCode || _T("단행") == strPublishFormCode)
		{
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("예약(UNDO)"), __WFILE__, __LINE__));
			pMobileApi->MakeUpdateFrame(_T("BO_BOOK_TBL"),_T("REC_KEY"),_T("NUMERIC"),sBookKey);
		}
		else
		{
			pMobileApi->AddDBFieldData(_T("LAST_WORK"),_T("STRING"),m_pParentMgr->GetWorkLogMsg(_T("예약(UNDO)"), __WFILE__, __LINE__));
			pMobileApi->MakeUpdateFrame(_T("SE_BOOK_TBL"),_T("REC_KEY"),_T("NUMERIC"),sBookKey);
		}
		
		ids = pMobileApi->SendFrame();
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		pMobileApi->EndFrame();
		CLocCommonAPI::MobileApiEndLog(pMobileApi);
		
		
		SetTempData(SHARE_CO_LOAN_TBL_DELETE_REC_KEY, CComVariant(sCLRecKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_REC_KEY, CComVariant(sCLURecKey));
		SetTempData(SHARE_CO_LOAN_USER_TBL_UPDATE_FIELDS, CComVariant(_T("RESERVATION_COUNT")));
		ShareInfoCenterUpload(_T("D"), _T("U"));

		pGrid->SelectGetPrev();
	}
	
	m_bReturnBookNonFree=TRUE;
	SPFExcutePath(_T("회원리로드_NOASK"));

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::KL_OtherLibUserImport(CString strLibCode, CString strLibName, CString &strUserNo)
{
	INT ids = 0;
	CString strGetLibCode=_T(""), strGetLibName=_T("");
	INT nMode = 0;
	if(!strLibCode.IsEmpty() && !strLibName.IsEmpty()) nMode = 1;
	
	if(0 == nMode)
	{
		ids = KL_GetSelectLibInfo(strUserNo, strGetLibCode, strGetLibName);
		if(0 != ids) return ids;
	}
	else
	{
		strGetLibCode = strLibCode;
		strGetLibName = strLibName;
	}
	
	const int nCnt = 15;
	CString strGetData[nCnt] = {_T(""),};
	ids = KL_OtherLibUserSearch(nMode, strGetLibCode, strUserNo, strGetData);
	if(0 != ids) return ids;
	
	CKL_UserImportDlg dlg(m_pParentMgr);
	for(INT i = 0; i < nCnt; i++)
	{
		dlg.m_strGetData[nCnt] = strGetData[nCnt];
	}
	dlg.m_strLibName = strGetLibName;
	if(IDOK != dlg.DoModal()) return 1;
	strUserNo = dlg.m_strUserNo;

	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_GetSelectLibInfo(CString strUserNo, CString &strLibCode, CString &strLibName)
{
	INT ids;
	if(14 <= strUserNo.GetLength())
	{
		strLibCode = strUserNo.Left(6);
		if(TRUE == IsNumericText(strLibCode))
		{
			CESL_DataMgr mwDM;
			CFrameReader Reader;
			CFrameWriter Writer(m_pParentMgr->m_pInfo);
			CFrameSender Sender(m_pParentMgr->m_pInfo);
			Writer.setCommand(_T("IL_K16_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("도서관부호"), strLibCode);
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if(0 > ids) 
			{
				
				SelfCloseMessageBox(_T("책이음서비스 접속 시도중입니다."), m_pParentMgr);
				return 100;
			}
			Reader.makeDM(&mwDM);
			CString strTemp = Reader.getCommand();
			if(_T("ERROR") == strTemp)
			{
				CString strMsg, strTemp;
				strTemp = Reader.getElement(0, 0);
				strMsg.Format(_T("도서관정보조회(16) ERROR CODE : %s"), strTemp);
				mwDM.QueryLog(strMsg.GetBuffer(0));
				strMsg = _T("도서관정보 조회에 실패하였습니다.\r\n")
						 _T("재시도 하시겠습니까?");
				if(IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION|MB_YESNO))
				{
					return KL_GetSelectLibInfo(strUserNo, strLibCode, strLibName);
				}
				return 101;
			}
			if(0 < mwDM.GetRowCount())
			{
				mwDM.GetCellData(_T("도서관명"), 0, strLibName);
			}
		}
	}
	if(strLibName.IsEmpty())
	{ 
		CLibListDlg dlg(m_pParentMgr, this);
		dlg.SetMode(2);
		if(IDOK != dlg.DoModal()) return 102;
		strLibCode = dlg.m_strLibCode;
		strLibName = dlg.m_strLibName;
	}
	
	return 0;
}

INT CBL_LOC_LOAN_PROC::KL_OtherLibUserSearch(INT nMode, CString strLibCode, CString strUserNo, CString strGetData[])
{
	INT ids;
	CESL_DataMgr mwDM;
	CFrameReader Reader;
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	CFrameSender Sender(m_pParentMgr->m_pInfo);
	Writer.setCommand(_T("IL_K33_LOC_B01_SERVICE"));
	Writer.addRecord();
	Writer.addElement(_T("도서관부호"), strLibCode);
	Writer.addElement(_T("대출자번호"), strUserNo);
	ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0 > ids) 
	{
		
		if(0 == nMode) SelfCloseMessageBox(_T("책이음서비스 접속 시도중입니다."), m_pParentMgr);
		return 100;
	}
	Reader.makeDM(&mwDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR") == strTemp)
	{
		CString strMsg, strTemp;
		strTemp = Reader.getElement(0, 0);
		if(-1 < strTemp.Find(_T("DT01")))
		{
			if(0 == nMode) SelfCloseMessageBox(_T("선택한 도서관에서 조회된 이용자가 없습니다."), m_pParentMgr, 2000);
			return 102;
		}
		strMsg.Format(_T("타도서관일반회원조회(33) ERROR CODE : %s"), strTemp);
		mwDM.QueryLog(strMsg.GetBuffer(0));
		if(0 == nMode)
		{
			strMsg = _T("타도서관 일반회원 조회에 실패하였습니다.\r\n")
					 _T("재시도 하시겠습니까?");
			if(IDYES == AfxMessageBox(strMsg, MB_ICONQUESTION|MB_YESNO))
			{
				return KL_OtherLibUserSearch(nMode, strLibCode, strUserNo, strGetData);
			}
		}
		return 101;
	}
	if(1 > mwDM.GetRowCount())
	{
		if(0 == nMode) SelfCloseMessageBox(_T("선택한 도서관에서 조회된 이용자가 없습니다."), m_pParentMgr, 2000);
		return 102;
	}
	if(1 < mwDM.GetRowCount())
	{
		if(0 == nMode) SelfCloseMessageBox(_T("2명 이상의 이용자가 검색되었습니다."), m_pParentMgr);
		return 103;
	}
	mwDM.GetCellData(_T("대출자번호"), 0, strGetData[0]);
	mwDM.GetCellData(_T("이름"), 0, strGetData[1]);
	mwDM.GetCellData(_T("주민등록번호"), 0, strGetData[2]);
	mwDM.GetCellData(_T("핸드폰번호"), 0, strGetData[3]);
	mwDM.GetCellData(_T("제2연락처"), 0, strGetData[4]);
	mwDM.GetCellData(_T("이메일"), 0, strGetData[5]);
	mwDM.GetCellData(_T("비고"), 0, strGetData[6]);
	mwDM.GetCellData(_T("집우편번호"), 0, strGetData[7]);
	mwDM.GetCellData(_T("집주소"), 0, strGetData[8]);
	mwDM.GetCellData(_T("집전화번호"), 0, strGetData[9]);
	mwDM.GetCellData(_T("직장우편번호"), 0, strGetData[10]);
	mwDM.GetCellData(_T("직장주소"), 0, strGetData[11]);
	mwDM.GetCellData(_T("직장전화번호"), 0, strGetData[12]);
	mwDM.GetCellData(_T("최종대출일자"), 0, strGetData[13]);
	mwDM.GetCellData(_T("최종수정일자"), 0, strGetData[14]);

	return 0;
}


INT CBL_LOC_LOAN_PROC::SetColumnColor(CESL_DataMgr *pDM, CESL_Grid *pGrid, INT row, INT col)
{
	
	if(!m_bManageCodeColorUseYN && !m_bReferenceColorUseYN)
	{
		return 100;
	}
	CString strDMAlias, strFieldAlias, strAlias, strColor, strTmp;
	pGrid->FindLinkDMAndAlias(col, strDMAlias, strFieldAlias);
	if(m_bManageCodeColorUseYN)
	{
		if(_T("관리구분설명") == strFieldAlias)
		{
			
			pDM->GetCellData(_T("관리구분설명"), row, strTmp);
			if(_T("") == strTmp)
			{
				strTmp = pGrid->GetAt(row, col);
			}
		
			strAlias.Format(_T("%s_글자색"), strTmp);
			m_pManageCodeColorDM->GetCellData(strAlias, 0, strColor);
			pGrid->SetCol(col);
			
			if(_T("") != strColor)
			{
				pGrid->SetCellForeColor(_ttoi(strColor));
			}
			strAlias.Format(_T("%s_바탕색"), strTmp);
			m_pManageCodeColorDM->GetCellData(strAlias, 0, strColor);
			
			if(_T("") != strColor)
			{
				pGrid->SetCellBackColor(_ttoi(strColor));
			}
		}
	}
	if(m_bReferenceColorUseYN)
	{	
		if(_T("자료실") == strFieldAlias)
		{
			
			pDM->GetCellData(_T("자료실"), row, strTmp);
			if(_T("") == strTmp)
			{
				strTmp = pGrid->GetAt(row, col);
			}
			strAlias.Format(_T("%s_글자색"), strTmp);
			m_pReferenceColorDM->GetCellData(strAlias, 0, strColor);
			pGrid->SetCol(col);
			
			if(_T("") != strColor)
			{
				pGrid->SetCellForeColor(_ttoi(strColor));
			}
			strAlias.Format(_T("%s_바탕색"), strTmp);
			m_pReferenceColorDM->GetCellData(strAlias, 0, strColor);
			
			if(_T("") != strColor)
			{
				pGrid->SetCellBackColor(_ttoi(strColor));
			}
		}
	}
	return 0;
}


INT CBL_LOC_LOAN_PROC::SetColorDM(CString strAlias)
{
	CCfgFileApi api;
	INT nDefaultValue = 0;
	INT nCount = m_pParentMgr->m_pInfo->pCodeMgr->GetCodeCount(strAlias);
	INT nFiledCnt = nCount * 2;
	nCount++;
	if(_T("관리구분") == strAlias)
	{	
		m_pManageCodeColorDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_COLOR_SETTING_MANAGE"));	
		CString* strCode = new CString[nCount];
		CString* strDesc = new CString[nCount];
		INT ids = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(strAlias, strCode, strDesc, nCount, nDefaultValue);
		if(0 > ids)
		{
			return -100;
		}
		api.SetFile(_T("..\\cfg\\LoanReturn\\화면색설정_관리구분.txt"));
		INT col = 0;
		m_pManageCodeColorDM->InitDataMgrRef(nFiledCnt);
		m_pManageCodeColorDM->InsertRow(-1);
		for(INT i=0; i<nCount; i++)
		{
			CString strTmp, strColorCode;
			strTmp.Format(_T("%s_글자색"), strDesc[i]);
			
			m_pManageCodeColorDM->SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
			
			strTmp.Format(_T("%s_바탕색"), strDesc[i]);
			
			m_pManageCodeColorDM->SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
		}
		
		CString strTmp, strColor;
		for(INT h=0; h<nCount; h++)
		{
			strTmp.Format(_T("%s_글자색"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_pManageCodeColorDM->SetCellData(strTmp, strColor, 0);
			strTmp.Format(_T("%s_바탕색"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_pManageCodeColorDM->SetCellData(strTmp, strColor, 0);
		}
		delete []strCode;
		strCode = NULL;
		delete []strDesc;
		strDesc = NULL;
	}
	else if(_T("자료실구분") == strAlias)
	{		
		m_pReferenceColorDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_COLOR_SETTING_REF"));
		CString* strCode = new CString[nCount];
		CString* strDesc = new CString[nCount];
		INT ids = m_pParentMgr->m_pInfo->pCodeMgr->GetCode(strAlias, strCode, strDesc, nCount, nDefaultValue);
		if(0 > ids)
		{
			return -100;
		}
		api.SetFile(_T("..\\cfg\\LoanReturn\\화면색설정_자료실.txt"));
		INT col = 0;
		m_pReferenceColorDM->InitDataMgrRef(nFiledCnt);
		m_pReferenceColorDM->InsertRow(-1);
		for(INT i=0; i<nCount; i++)
		{
			CString strTmp, strColorCode;
			strTmp.Format(_T("%s_글자색"), strDesc[i]);
			
			m_pReferenceColorDM->SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
			
			strTmp.Format(_T("%s_바탕색"), strDesc[i]);
			
			m_pReferenceColorDM->SetDataMgrRef(col, strTmp, _T(""), _T("str"), _T(""), _T(""));
			col++;
		}
		
		CString strTmp, strColor;
		for(INT h=0; h<nCount; h++)
		{
			strTmp.Format(_T("%s_글자색"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_pReferenceColorDM->SetCellData(strTmp, strColor, 0);
			strTmp.Format(_T("%s_바탕색"), strDesc[h]);
			api.GetData(strTmp, strColor);
			m_pReferenceColorDM->SetCellData(strTmp, strColor, 0);
		}
		delete []strCode;
		strCode = NULL;
		delete []strDesc;
		strDesc = NULL;
	}
	return 0;
}


VOID CBL_LOC_LOAN_PROC::FreeColorDM(CString strAlias)
{
	if(_T("관리구분") == strAlias)
	{
		m_pManageCodeColorDM = NULL;
	}
	else if(_T("자료실구분") == strAlias)
	{
		m_pReferenceColorDM = NULL;
	}
}


VOID CBL_LOC_LOAN_PROC::InitColorDM()
{
	
	CCfgFileApi api;
	CString strTmp;
	api.SetFile(_T("..\\cfg\\LoanReturn\\화면색설정_사용여부.txt"));
	api.GetData(_T("관리구분"), strTmp);
	if(_T("Y") == strTmp)
	{
		m_bManageCodeColorUseYN = TRUE;
		if(0 > SetColorDM(_T("관리구분")))
		{
			AfxMessageBox(_T("색상 초기화에 실패하였습니다."));
		}
	}
	api.GetData(_T("자료실"), strTmp);
	if(_T("Y") == strTmp)
	{
		m_bReferenceColorUseYN = TRUE;
		if(0 > SetColorDM(_T("자료실구분")))
		{
			AfxMessageBox(_T("색상 초기화에 실패하였습니다."));
		}
	}
}

INT CBL_LOC_LOAN_PROC::UNDO_OtherLoan()
{
	INT ids;

	if(IDNO == AfxMessageBox(_T("선택하신 자료는 타관대출 자료입니다. 대출을 취소하시겠습니까?"), MB_ICONQUESTION|MB_YESNO))
	{
		return 0;
	}

	CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));
	
	if(NULL != pMainGrid)
	{
		
		pMainGrid->SelectMakeList();
		INT nIdx = pMainGrid->SelectGetTailPosition();
		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		for(int i=0;i<pMainGrid->SelectGetCount();i++)
		{
			CString strLoanKey;
			strLoanKey = pDM->GetCellData(_T("타관대출KEY"),nIdx);
			CFrameWriter Writer(m_pParentMgr->m_pInfo);
			Writer.setCommand(_T("LILL_K02_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("TRANSACTION_WORK"), _T("UPDATE"));
			Writer.addElement(_T("TRANSACTION_CODE"), _T("3"));
			Writer.addElement(_T("LOAN_KEY"),strLoanKey);
			Writer.addElement(_T("CANCEL"),_T("1"));
			
			CFrameSender Sender(m_pParentMgr->m_pInfo);	
			CFrameReader Reader; 
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if (0 > ids) 
			{
				while(true) 
				{
					
					if(IDYES == MessageBox(m_pParentMgr->m_hWnd,_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("조회실패"), MB_YESNO | MB_ICONQUESTION)) 
					{
						ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
						if(0 <= ids) break;
					}
					else
					{
						return 0;
					}
				}
			}
			nIdx=pMainGrid->SelectGetPrev();			
		}
		m_bReturnBookNonFree=TRUE;
		SPFExcutePath(_T("회원리로드_NOASK"));
	}
	return 0;
}

INT CBL_LOC_LOAN_PROC::OtherReturn(int nType)
{
	INT ids = 0;
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));

	if(IDNO == AfxMessageBox(_T("선택하신 자료는 타관대출 자료입니다. 반납하시겠습니까?"), MB_ICONQUESTION|MB_YESNO))
	{
		return 0;
	}

	CESL_Grid* pMainGrid = (CESL_Grid*)m_pParentMgr->FindControl(_T("CM_BO_LOC_3100_LOAN_INFO"),_T("MainGrid"));


	INT nIdx = pMainGrid->SelectGetIdx();
	
	
	
	if(nIdx < 0){
		pMainGrid->SelectMakeList();
		pMainGrid->SelectGetHeadPosition();
		nIdx = pMainGrid->SelectGetIdx();
	}

	if(NULL != pMainGrid)
	{
		CString strReturnDate;
		CString strOtherLoanKey;

		CString strMsg, strRegNo, strTitle, strAppendixSendYN, strAppendixRegNoList;
		
		CTime tLoanDate = CTime::GetCurrentTime();

		INT nReturnType = InitReturnMode();		
		CString strDate, sReturnDateControlAlias = _T("반납일");
		if(nReturnType == 2)	
		{
			sReturnDateControlAlias = _T("소급반납일");
		}
		ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, strDate);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("OtherReturn"));
		strReturnDate.Format(_T("%s %02d:%02d:%02d"), strDate, tLoanDate.GetHour(), tLoanDate.GetMinute(), tLoanDate.GetSecond());

			strOtherLoanKey = pDM->GetCellData(_T("타관대출KEY"),nIdx);
			
			if(nType == 1)
			{				
				strOtherLoanKey = pDM->GetCellData(_T("LCT_KEY"),nIdx);
			}

			pDM->GetCellData(_T("등록번호"), nIdx, strRegNo);
			pDM->GetCellData(_T("본표제"), nIdx, strTitle);
			pDM->GetCellData(_T("부록발송유무"), nIdx, strAppendixSendYN);
			pDM->GetCellData(_T("부록등록번호리스트"), nIdx, strAppendixRegNoList);
			if (_T("Y") == strAppendixSendYN) {
				strMsg.Format(
					_T("부록자료와 함께 대출된 자료입니다.\r\n")
					_T("\r\n")
					_T("[ %s:%s ]\r\n")
					_T("부록자료를 확인 하셨습니까?\r\n")
					_T(" - 부록등록번호 : %s\r\n")
					_T("( 예 : 반납,  아니오 : 반납취소 )")
					, strRegNo, strTitle, strAppendixRegNoList);
				if (IDYES != MessageBox(m_pParentMgr->m_hWnd, strMsg, _T("타관반납>부록확인"), MB_ICONINFORMATION|MB_YESNO)) {
					return 0;
				}
			}
			
			CFrameWriter Writer(m_pParentMgr->m_pInfo);
			Writer.setCommand(_T("LILL_K07_LOC_B01_SERVICE"));
			Writer.addRecord();
			Writer.addElement(_T("LOAN_KEY"),strOtherLoanKey);
			Writer.addElement(_T("RETURN_DATE"),strReturnDate);
			Writer.addElement(_T("RETURN_LIB_CODE"),m_pParentMgr->m_pInfo->LIB_CODE);
			Writer.addElement(_T("R_DEVICE"),_T("KOLAS III"));
			Writer.addElement(_T("R_WORKER"),m_pParentMgr->m_pInfo->USER_ID);
			CFrameSender Sender(m_pParentMgr->m_pInfo);	
			CFrameReader Reader; 
			ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
			if (0 > ids) 
			{
				while(true) 
				{
					if(IDYES == MessageBox(m_pParentMgr->m_hWnd,_T("책이음서비스 미들웨어와의 통신에 실패하였습니다.\r\n재시도 하시겠습니까?"), _T("조회실패"), MB_YESNO | MB_ICONQUESTION)) 
					{
						ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
						if(0 <= ids) continue;
					}
					else
					{
						return 0;
					}
				}
			}
			if(_T("OK") == Reader.getElement(0,0))
			{
				CESL_DataMgr* pReturnBasket = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));	
				if (NULL == pReturnBasket) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1, _T("OtherReturn"));
				INT nInsertIdx = pReturnBasket->GetRowCount();
				pReturnBasket->InsertRow(-1);
				INT nColCnt = pReturnBasket->GetRefColumnCount();
				CString strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type, strResult;
				for (INT i = 0; i < nColCnt; i++) {
					pReturnBasket->GetColumnRefInfo(i, strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type);
					if (_T("R_DEVICE") == strField_Alias) {
						strResult = _T("KOLASIII");
					} else if (_T("R_WORKER") == strField_Alias) {
						strResult = m_pParentMgr->m_pInfo->USER_ID;
					} else if (_T("반납일") == strField_Alias) {
						strResult = strReturnDate;
					} else {
						pDM->GetCellData(strField_Alias, nIdx, strResult);
					}
					pReturnBasket->SetCellData(strField_Alias, strResult, nInsertIdx);
				}

				CString strHoldLibCode,
					strQry,
					strHoldManageCode,
					strLoanStopDate,
					strLocalKey,
					strReturnPlanDate;
				pDM->GetCellData(_T("반납예정일"), nIdx, strReturnPlanDate);
				COleDateTime t1 (_ttoi(strDate.Mid(0,4)), _ttoi(strDate.Mid(5,2)), _ttoi(strDate.Mid(8,2)), 0, 0, 0);
				COleDateTime t2 (_ttoi(strReturnPlanDate.Mid(0,4)), _ttoi(strReturnPlanDate.Mid(5,2)), _ttoi(strReturnPlanDate.Mid(8,2)), 0, 0, 0);
				if ( t1 > t2 )
				{
					pDM->GetCellData(_T("소장관도서관부호"), nIdx, strHoldLibCode);
					strQry.Format(_T("SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE = '%s'"),strHoldLibCode);
					if ( !strHoldLibCode.IsEmpty() && pDM->GetOneValueQuery(strQry,strHoldManageCode) != -4007 ){
						pDM->GetCellData( _T("로컬KEY"), nIdx, strLocalKey );					
						strQry.Format(_T("SELECT ECO$F_GET_LOANSTOPDATE( %s, TO_DATE('%s'), '%s' ) FROM DUAL"), strLocalKey, strDate, strHoldManageCode);				
						pDM->GetOneValueQuery( strQry, strLoanStopDate );
						if( !strLoanStopDate.IsEmpty() ) {
							CESL_DataMgr* pMemDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
							if (pMemDM != NULL)
							{
								pMemDM->SetCellData(_T("대출정지일"), strLoanStopDate ,0 );
								pMemDM->SetCellData(_T("회원상태"), _T("1"),0 );
							}
						}
					}					
				}
				
				AddMemberInfo();

				pDM->DeleteRow(nIdx);

				
				Send3000ApiSocket(_T("타관반납"));
				if (0 != pDM->GetRowCount()) {
					((CMSHFlexGrid*)pMainGrid)->RemoveItem(nIdx+1);
					CString sIndex;
					for (INT i = 0; i < pDM->GetRowCount(); i++) {
						sIndex.Format(_T("%d"), i+1);
						pMainGrid->SetTextMatrix(i+1, 0, sIndex);
					}
				}
				SetBookStatusCnt_Unity();
			}
			else {
				AddMemberInfo();	
			}
		}

	ViewLoanScreen();
	
	return ids;
	
}

INT CBL_LOC_LOAN_PROC::RFID_Loan(){
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("RFID_Loan");

	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	ClearBasket();
	
	CESL_DataMgr* pRFID_DM = m_pParentMgr->FindDM(_T("DM_RFID목록"));
	if(NULL == pRFID_DM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);

	CString sBasketDMAlias = _T("DM_BO_LOC_3100_BASKET");
	
	CESL_DataMgr *pBasketDM = m_pParentMgr->FindDM(sBasketDMAlias);
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sPublishForm;
	CString sLoanYN;	
	CString sRegNo;	
	CString sSerialNo;	
	CString sSql;	
	CString sTableName;
	CString sWorkingStatus;

	INT nInsertIndex;
	INT nLoanCnt = 0;
	for(INT i = 0; i < pRFID_DM->GetRowCount(); i++)
	{
		ids = pRFID_DM->GetCellData(_T("발행형태"), i, sPublishForm);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if(_T("MO") == sPublishForm || _T("B") == sPublishForm)
		{
			sTableName = _T("BO_BOOK_TBL");
		}
		else if(_T("SE") == sPublishForm || _T("S") == sPublishForm)
		{
			sTableName = _T("SE_BOOK_TBL");
		}
		else if(_T("NB") == sPublishForm || _T("N") == sPublishForm)
		{
			sTableName = _T("CO_NON_DB_BOOK_TBL");
		}
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pRFID_DM->GetCellData(_T("시리얼"), i, sSerialNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pRFID_DM->GetCellData(_T("책상태"), i, sWorkingStatus);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if(sWorkingStatus != _T("BOL611O") && sWorkingStatus != _T("BOL319O")){
			CString strIsILL;
			pRFID_DM->GetCellData(_T("책두레여부"), i, strIsILL);

			if(_T("Y") != strIsILL)
			{
				sSql.Format(_T("SELECT MAX(REG_NO) FROM %s WHERE WORKNO='%s' AND MANAGE_CODE='%s' ")
							, sTableName, sSerialNo, m_pParentMgr->m_pInfo->MANAGE_CODE);
			}
			else
			{
				sSql.Format(_T("SELECT MAX(REG_NO) FROM ILL_TRANS_TBL WHERE WORKNO='%s' AND APL_LIBCODE='%s'")
							, sSerialNo, m_pParentMgr->m_pInfo->LIB_CODE);
			}
			CLocCommonAPI::GetOneValueQuery(m_pParentMgr, pRFID_DM, sSql, sRegNo);
			sRegNo.TrimRight(); sRegNo.TrimLeft();
			if(sRegNo.IsEmpty())
			{
				CString sEMsg;
				sEMsg.Format(_T("시리얼 번호가 '%s'인 자료의 등록번호가 존재하지 않습니다."), sSerialNo);
				ViewMessage(sEMsg);
				continue;
			}
		}

		ids = pRFID_DM->GetCellData(_T("이용자번호"), i, sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		

		ids = pRFID_DM->GetCellData(_T("대출여부"), i, sLoanYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if(_T("1") == sLoanYN)
		{
			CString sEMsg;
			sEMsg.Format(_T("등록 번호가 '%s'인 자료는 현재 대출중인 자료입니다."), sRegNo);
			ViewMessage(sEMsg);
			continue;			
		}
		
		
		m_pParentMgr->SetControlData(sCMAlias, _T("대출자번호"), sRegNo);

		if(_T("0") == sLoanYN)
		{
			MoveRegNoToNextProcBasket(EXP_LOAN, TRUE);
		}
		else if(_T("2") == sLoanYN)
		{
			MoveRegNoToNextProcBasket(EXP_RESERVELOAN, TRUE);
		}
		else if(_T("L") == sLoanYN)
		{
			MoveRegNoToNextProcBasket(_T("L"), TRUE);
		}	
		else if(_T("LILL") == sLoanYN)
		{
			MoveRegNoToNextProcBasket(_T("LILL"), TRUE);
		}			

		nInsertIndex = pBasketDM->GetRowCount() - 1;
		ids = pBasketDM->SetCellData(_T("시리얼"), sSerialNo, nInsertIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		CString strBookKey;
		ids = pRFID_DM->GetCellData(_T("책KEY"), i, strBookKey);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		ids = pBasketDM->SetCellData(_T("책키"), strBookKey, nInsertIndex);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		nLoanCnt++;
	}

	if(0 == nLoanCnt)
	{
		SelfCloseMessageBox(_T("대출할 자료가 존재하지 않습니다."));
		return 1;
	}
	BasketRegNoProc();

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::RFID_Return()
{
EFS_BEGIN

	INT ids;
	TCHAR szMsg[256];
	CString strFuncName = _T("RFID_Return");
	
	
	CString sCMAlias = _T("CM_BO_LOC_3100_SEARCH_INFO");
	ClearBasket();
	
	
	CESL_DataMgr* pRFID_DM = m_pParentMgr->FindDM(_T("DM_RFID목록"));
	if(NULL == pRFID_DM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	CString sBasketDMAlias = _T("DM_BO_LOC_3100_BASKET");
	
	
	CESL_DataMgr *pBasketDM = m_pParentMgr->FindDM(sBasketDMAlias);
	if(NULL == pBasketDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
			
	CString sPublishForm;	
	CString sLoanYN;	
	CString sRegNo;	
	CString sSerialNo;	
	CString sSql;
	
	CString sTableName;
	CString sUserNo;
	CString sBookKey;
	BOOL	bDelayFlag;
	
	for(INT i = 0 ; i < pRFID_DM->GetRowCount() ; i++)
	{
		bDelayFlag = FALSE;

		ids = pRFID_DM->GetCellData(_T("발행형태"), i, sPublishForm);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if(sPublishForm.Compare(_T("B")) == 0)
			sTableName = _T("BO_BOOK_TBL");
		else if(sPublishForm.Compare(_T("S")) == 0)
			sTableName = _T("SE_BOOK_TBL");
		else if(sPublishForm.Compare(_T("N")) == 0)
			sTableName = _T("CO_NON_DB_BOOK_TBL");
		else
			sTableName = _T("BO_BOOK_TBL");
		
		ids = pRFID_DM->GetCellData(_T("시리얼"), i, sSerialNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		ids = pRFID_DM->GetCellData(_T("이용자번호"), i, sRegNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		ids = pRFID_DM->GetCellData(_T("대출여부"), i, sLoanYN);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		if(sLoanYN.Compare(_T("0")) == 0)
			continue;			
	
		ids = pRFID_DM->GetCellData(_T("반납대상이용자번호"), i, sUserNo);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		
		CString sTmpUserNo;
		ids = m_pParentMgr->GetDataMgrData(_T("DM_BO_LOC_3100_MEMBER_INFO"), _T("대출자번호"), sTmpUserNo, 0);
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		
		
		if(sTmpUserNo.Compare(sUserNo) != 0)
		{
			SPFExcutePath(_T("화면초기화"));
			
			ids = m_pParentMgr->SetControlData(_T("CM_BO_LOC_3100_SEARCH_INFO"), _T("대출자번호"), sUserNo);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			
			SPFExcutePath(_T("회원_검색_NOASK"));	
			if(m_pManageValue->m_nSetloanStopMode)
			{
				return 0;
			}

			sTmpUserNo = sUserNo;
		}	

		
		CESL_DataMgr *pLoanInfoDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		if(NULL == pLoanInfoDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);

		
		if(sTmpUserNo.Compare(sUserNo) == 0)
		{
			ids = pRFID_DM->GetCellData(_T("책KEY"), i, sBookKey);
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
			
			ids = BarCodeReturnBookCheck(sBookKey);
			if(0 > ids) return ids;

			CString strIsILL = pRFID_DM->GetCellData(_T("책두레여부"), i);
			BOOL bIsILL = (_T("Y") == strIsILL) ? TRUE : FALSE;

			if(m_pManageValue->m_sFeeMngSys == _T("Y") || m_pManageValue->m_ArrearCheckYN == _T("Y"))
			{
				if(bDelayFlag == FALSE)
				{
					CString strTempBookKey;
					CString sDelayDay;					
										
					for(INT y = 0 ; y<pLoanInfoDM->GetRowCount() ; y++)
					{
						pLoanInfoDM->GetCellData(_T("책KEY"), y, strTempBookKey);					
						if(strTempBookKey == sBookKey)
						{
							ids = pLoanInfoDM->GetCellData(_T("연체일수"), y, sDelayDay);
							if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
							if(_ttoi(sDelayDay) > 0) bDelayFlag = TRUE;
							break;
						}					
					}		
				}
				ids = ReturnBook(bDelayFlag, FALSE);
				if(0 > ids) return ids;
			}
			else
			{
				ids = ReturnBook(FALSE, FALSE);
				if(0 > ids) return ids;
			}

			BOOL bIsUpdate = TRUE;
			CString strRfidWorkingStatus = pRFID_DM->GetCellData(_T("책상태"), i);

			if(strRfidWorkingStatus != _T("BOL611O")){
				CString strGiveLibCode;
				strGiveLibCode = GetLIB_CODE(pRFID_DM,sBookKey,sTableName);
				if(m_pCheckLoan->IsOtherManageReturn() == FALSE){
					if(strGiveLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
					{
						if ( m_pManageValue->m_strSetRFIDSecuReturnLibYN == _T("N")) {
							bIsUpdate = FALSE;
						}
					}
				}
			}

			if(ids == 0)
			{
				DWORD nSTART,nFINISH;

				nSTART = GetTickCount();
				
			
				RFID_UpdateLoan(bIsUpdate, sSerialNo);
				nFINISH = GetTickCount();

				CString strMsg;
				strMsg.Format(_T("■[성공내역] 대출자번호 : %s / 등록번호 : %s / 책두레여부 : %s / 당시 책상태 : %s"),sUserNo, sRegNo, strIsILL, strRfidWorkingStatus);
				if(bIsUpdate == TRUE)
				{
					strMsg += _T(" / RFID보안 : 설정");
				}else{
					strMsg += _T(" / RFID보안 : 해제");
				}
				pRFID_DM->QueryLog(strMsg.GetBuffer(0));

				strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), _T("RFIDTaggin시간"), (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);
				
				pRFID_DM->QueryLog(strMsg.GetBuffer(0));
			}
		}
	}
	return 0;
EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::RecordRFIDUserInfo()
{
EFS_BEGIN
	INT	ids;
	CString strFuncName = _T("RecordRFIDUserInfo");

	CESL_DataMgr* pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	INT nCount = pDM->GetRowCount();

	if(1 > nCount)
	{
		SelfCloseMessageBox(_T("이용자를 검색해 주십시오."));
		return 1010;
	}

	CString strLibCode = m_pParentMgr->m_pInfo->LIB_CODE;
	CString strUserNo = _T("");
	CString strPassWord = _T("");
	CString strEncryptionPassWord = _T("");
	CString strRecKey;
	CString strUserName;	
	CString strKlUserYN;

	ids = pDM->GetCellData(_T("대출자번호"), 0, strUserNo);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);}
	ids = pDM->GetCellData(_T("대출자KEY"), 0, strRecKey);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);}
	ids = pDM->GetCellData(_T("대출자이름"), 0, strUserName);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);}
	
	ids = pDM->GetCellData(_T("통합회원여부"), 0, strKlUserYN);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);}

	CRFID_Interface RFID(m_pParentMgr);
	INT nTagCnt;
	CStringArray DSFID_LIST;
	CStringArray SERIAL_LIST;
	CString strFuncInfo;
	CString strQuery, strValue;
	
	ids = RFID.GET_STATUS();
	
	CLocCommonAPI::MakeRFIDLog(_T("GET_STATUS()"), _T("RFID 동기화"), __WFILE__, __LINE__, ids);
	if(0 > ids)
	{
		SelfCloseMessageBox(_T("리더기를 찾을 수 없습니다."));
		return 1020;
	}

	ids = RFID.GetTagSerial(SERIAL_LIST);
	nTagCnt = SERIAL_LIST.GetSize();
	
	strFuncInfo.Format(_T("RFID_GetTagSerials(SerialList, %d)"), nTagCnt);
	CLocCommonAPI::MakeRFIDLog(strFuncInfo, _T("RFID Tag 동기화"), __WFILE__, __LINE__, ids);

	if(1 != nTagCnt)
	{
		if(0 == nTagCnt)
		{
			SelfCloseMessageBox(_T("회원증이 존재하지 않습니다."));
			return 1030;
		}
		else if(1 < nTagCnt)
		{
			SelfCloseMessageBox(_T("하나의 회원증만 올려주십시오."));
			return 1040;
		}	
		
		return 1050;
	}	
	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	TmpDM.CheckManageCode();
	strQuery.Format(
		_T("SELECT BB.MANAGE_CODE, '단행테이블 : '||IB.TITLE_INFO||'('||BB.REG_NO||')', NULL ")
		_T("FROM BO_BOOK_TBL BB, IDX_BO_TBL IB ")
		_T("WHERE BB.SPECIES_KEY=IB.REC_KEY AND BB.WORKNO='%s' AND BB.REC_KEY <> %s ")
		_T("UNION ")
		_T("SELECT SB.MANAGE_CODE, '연속테이블 : '||SS.TITLE_INFO||'('||SB.REG_NO||')', NULL ")
		_T("FROM SE_BOOK_TBL SB, IDX_SE_TBL SS ")
		_T("WHERE SB.SPECIES_KEY=SS.REC_KEY AND SB.WORKNO='%s' AND SB.REC_KEY <> %s ")
		_T("UNION ")
		_T("SELECT CLU.MANAGE_CODE, '이용자테이블 : '||CLU.NAME||'('||CLU.USER_NO||')', CLU.WORKNO ")
		_T("FROM CO_LOAN_USER_TBL CLU ")
		_T("WHERE CLU.WORKNO='%s' AND CLU.REC_KEY <> %s")
		, SERIAL_LIST.GetAt(0), strRecKey, SERIAL_LIST.GetAt(0), strRecKey, SERIAL_LIST.GetAt(0), strRecKey);

	ids = TmpDM.RestructDataManager(strQuery);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);}

	INT nRowCnt = 0;	
	const INT ROW_COUNT = TmpDM.GetRowCount();
	const INT COL_CNT = 3;
		
	CString strMsg, strSerial, strManageCode, strSerialList;
	TmpDM.StartFrame();
	if(0 < ROW_COUNT)
	{
		for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
		{
			ids = TmpDM.GetCellData(nRowCnt, 0, strManageCode);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1040, strFuncName);}
			ids = TmpDM.GetCellData(nRowCnt, 1, strSerial);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);}
			
			
			if(0 > strSerial.Find(_T("이용자테이블")))
			{ 
				if(strManageCode != TmpDM.MANAGE_CODE)
				{
					TmpDM.EndFrame();
					strMsg.Format(_T("이 태그는 다른 도서관에서 등록된 태그이기 때문에 기록할 수 없습니다."));
					SelfCloseMessageBox(strMsg);
					return 1060;
				}
			}
			strSerialList = strSerialList + _T("\r\n") + strSerial;
		}
		
		strMsg.Format(_T("현재 사용중인 태그입니다. 다시 입력하시겠습니까?\r\n%s"), strSerialList);
		if(IDYES == AfxMessageBox(strMsg, MB_YESNO)) 
		{
			
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), SERIAL_LIST.GetAt(0));
			TmpDM.AddFrame(strQuery);
			strQuery.Format(_T("UPDATE SE_BOOK_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), SERIAL_LIST.GetAt(0));
			TmpDM.AddFrame(strQuery);

			CString strWorkNo;
			ids = TmpDM.GetCellData(0, 2, strWorkNo);
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);}
			
			if(TRUE != strWorkNo.IsEmpty())
			{ 
				if(SERIAL_LIST.GetAt(0) == strWorkNo)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO=NULL WHERE WORKNO='%s';"), strWorkNo);
				}
			}
			else
			{
				if(SERIAL_LIST.GetAt(0) == strWorkNo)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET WORKNO=NULL, CARD_PASSWORD=NULL WHERE WORKNO='%s';"), strWorkNo);
				}				
			}
			TmpDM.AddFrame(strQuery);
			ids = TmpDM.SendFrame();
			if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);}
			TmpDM.EndFrame();
		}
		else
		{
			TmpDM.EndFrame();
			return 1070;
		}
	}

	TmpDM.EndFrame();

	strQuery.Format(_T("SELECT WORKNO FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strRecKey);
	ids = pDM->GetOneValueQuery(strQuery, strValue);
	if(0 > ids && -4007 != ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1120, strFuncName);}
	
	CInsertPasswordDlg InsertPasswordDlg(m_pParentMgr);
	if(!strValue.IsEmpty())
	{
		if(strValue != SERIAL_LIST.GetAt(0))
		{
			InsertPasswordDlg.m_nViewMode = 1;
		}
	}

	InsertPasswordDlg.DoModal();

	INT nCloseMode = InsertPasswordDlg.GetCloseMode();
	if(nCloseMode == 0)
	{
		return 0;
	}
	strPassWord = InsertPasswordDlg.GetPassword();

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bCardPwEncryption && 64 > strPassWord.GetLength())
	{
		strEncryptionPassWord = CLocCommonAPI::StringToSHA256(strPassWord);
	}

	CString strUserNoAndName;
	CString sTempUserName;
	if(strUserName.GetLength() > 8)
	{
		sTempUserName = strUserName.Mid(0,8);
	}
	else
	{
		sTempUserName = strUserName;
	}
	strUserNoAndName.Format(_T("%s:%s"), strUserNo, sTempUserName);

	
	CString strRFID_Serial;	
	ids = RecordRFIDFromUserInfo(strLibCode, strUserNoAndName, strPassWord, strRecKey, strRFID_Serial);
	if(0 > ids)
	{
		strMsg.Format(_T("기록에 실패하였습니다."), strUserNo, strUserName);
		SelfCloseMessageBox(strMsg);
		return 1;
	}
	else if(1 == ids)
	{
		return 1;
	}
	else if(2 == ids)
	{
		return 2;
	}
	else if(3 == ids)
	{
		SelfCloseMessageBox(_T("책이 검색되어 진행 할 수 없습니다."));
		return 3;
	}
	
	CESL_DataMgr UpdateDM;
	UpdateDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	CKolas2up_MobileApi* pMobileApi = CLocCommonAPI::InitMobileApi(m_pParentMgr, &UpdateDM);
	if(NULL == pMobileApi){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1140, strFuncName);}
	pMobileApi->StartFrame();
	pMobileApi->InitDBFieldData();

	
	if(TRUE == m_pParentMgr->m_pInfo->m_bCardPwEncryption && 64 > strPassWord.GetLength()) {
		pMobileApi->AddDBFieldData(_T("CARD_PASSWORD"), _T("STRING"), strEncryptionPassWord);
	}
	else {
		pMobileApi->AddDBFieldData(_T("CARD_PASSWORD"), _T("STRING"), strPassWord);
	}
	
	pMobileApi->AddDBFieldData(_T("WORKNO"), _T("STRING"), strRFID_Serial);
	ids = pMobileApi->MakeUpdateFrame(_T("CO_LOAN_USER_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1150, strFuncName);}
	ids = pMobileApi->SendFrame();
	if(0 > ids){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1160, strFuncName);}
	pMobileApi->EndFrame();
	CLocCommonAPI::MobileApiEndLog(pMobileApi);
	
	if(TRUE == m_pParentMgr->m_pInfo->m_bKLUse && _T("Y") == strKlUserYN){
		CFrameWriter Writer(m_pParentMgr->m_pInfo);
		CString strModifyDate;
		CTime t = CLocCommonAPI::GetCurrentDateTime(m_pParentMgr, strModifyDate);
		Writer.setCommand(_T("IL_K24_LOC_B01_SERVICE"));
		Writer.addRecord();
		Writer.addElement(_T("통합대출자번호"), strUserNo);
		Writer.addElement(_T("회원증RFID"), SERIAL_LIST.GetAt(0));
		Writer.addElement(_T("수정일자"), strModifyDate);
		Writer.addElement(_T("수정도서관부호"), m_pParentMgr->m_pInfo->LIB_CODE);
		
		CFrameSender Sender(m_pParentMgr->m_pInfo);
		CFrameReader Reader;
		Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg( _T("통합RFID등록"), __WFILE__, __LINE__ ));	
	}
	
	strMsg.Format(_T("이용자번호 : [ %s ],  이름 : [ %s ]\r\n성공적으로 기록하였습니다."), strUserNo, strUserName);
	SelfCloseMessageBox(strMsg, m_pParentMgr, 4000);

	return 0;

EFS_END	
return -1;
}
INT CBL_LOC_LOAN_PROC::RecordRFIDFromUserInfo(CString strLibCode, CString strUserNo, CString strPassWord, CString strRecKey, CString &strRFID_Serail)
{
EFS_BEGIN
	INT ids;
	CRFID_Change RFID_Job;
	RFID_Job.m_pParentMgr = m_pParentMgr;
	ids = RFID_Job.RecordRFIDFromUserInfo(strLibCode, strUserNo, strPassWord, strRFID_Serail);
	return ids;

EFS_END
return -1;
}

CString CBL_LOC_LOAN_PROC::GetLIB_CODE(CESL_DataMgr *pDM,CString sBookKey,CString sTableName)
{
	EFS_BEGIN
	
	CString strQuery, strResult = _T(""),strTemp;


	if(sTableName == _T("")){
 		strQuery.Format(_T("SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REC_KEY = %s ")
						_T(" UNION ")
						_T("SELECT MANAGE_CODE FROM SE_BOOK_TBL WHERE REC_KEY = %s ")
						,sBookKey,sBookKey);
		pDM->GetOneValueQuery(strQuery, strTemp);

		if(strTemp == _T(""))
			return _T("");

 		strQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE ")
						_T("MANAGE_CODE='%s'")
						, strTemp);

	}else{
 		strQuery.Format(_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE ")
						_T("MANAGE_CODE=(SELECT MANAGE_CODE FROM %s WHERE REC_KEY=%s)")
						, sTableName, sBookKey);
	}
	pDM->GetOneValueQuery(strQuery, strResult);
		return strResult;

	EFS_END
		return _T("");
}

INT CBL_LOC_LOAN_PROC::ILL_ReservationLoan()
{
	EFS_BEGIN

	CString strFuncName = _T("ILL_ReservationLoan");
	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(!pCM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl(sGridAlias));
	if(!pGrid)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
	}
	pGrid->SelectMakeList();

	INT nRowCount = pGrid->SelectGetCount();
	if(!nRowCount)
	{
		MakeRegNoSpaceToZero(FALSE);

		CString sRegNo;
		IsRegNo(sRegNo);

		if(FALSE == sRegNo.IsEmpty())
		{
			BarCodeReturnBookCheck(sRegNo, 1);
		}

		pGrid->SelectMakeList();
		nRowCount = pGrid->SelectGetCount();
		if(!nRowCount)
		{
			SelfCloseMessageBox(_T("대출할 책을 선택해 주십시오"));
			return 0;
		}		
	}
	
	BOOL IsLoan = FALSE;	
	INT ids = IsLoanMember(IsLoan);
	if(0 > ids) return ids;
	if(!IsLoan) return 0;
	
	INT nIndex;
	CString sStatus, sIsAppendix, sRegNo, strSQL, strLoanKey, strAplLibCode;
	INT nProcessingCnt = 0;
	pGrid->SelectGetHeadPosition();

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	for (INT i = 0 ; i < nRowCount ; i++)
	{
		nIndex = pGrid->SelectGetIdx();
		
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("대출상태"), sStatus, nIndex);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		}
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("대출/반납정보KEY"), strLoanKey, nIndex);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
		}
				
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("등록번호"), sRegNo, nIndex);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1060, strFuncName);
		}

		if(sStatus.Compare(_T("O")))
		{
			pGrid->SelectGetNext();
			continue;
		}

		
		CString strBookManageCode;
		ids = m_pParentMgr->GetDataMgrData(sDMAlias, _T("관리구분"), strBookManageCode, nIndex);
		if(0 > ids) 
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1070, strFuncName);
		}
		

		ids = IsUserBookLoan(EXP_LOAN, TRUE, strBookManageCode);
		if(ids > 0)
		{
			pGrid->SelectGetNext();
			continue;			
		}
		

		strSQL.Format(_T("SELECT APL_LIBCODE FROM ILL_TRANS_TBL WHERE LOAN_KEY = %s"), strLoanKey);
		TmpDM.GetOneValueQuery(strSQL, strAplLibCode);

		if(strAplLibCode != m_pParentMgr->m_pInfo->LIB_CODE)
		{
			CString sEMsg;			
			if(sRegNo.IsEmpty())			
			{
				sEMsg.Format(_T("타관 예약자료입니다."));
			}
			else
			{
				sEMsg.Format(_T("등록번호['%s'] : 타관 예약자료입니다."), sRegNo);
			}
			
			SelfCloseMessageBox(sEMsg);
			pGrid->SelectGetNext();
			continue;
		}

		this->SetBEGIN_SBL();
		
		ids = ReservationDBProc(nIndex, TRUE);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1080, strFuncName);
		}
			
		if(ids == 100 || ids == 101)
		{
			pGrid->SelectGetNext();
			continue;
		}			
		
		ids = ReservationScreen(nIndex);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1090, strFuncName);
		}

		nProcessingCnt++;
		pGrid->SelectGetNext();
		
		
		ids = InitExpDM();

	if(0 > ids) return ids;
	}
	if(nProcessingCnt > 0) 
	{
		m_bReturnBookNonFree = TRUE;
		ids = SearchLoanBookInfo();
		if(0 > ids) return ids;
		ids = AddMemberInfo();
		if(0 > ids) return ids;
		
		ids = ViewLoanScreen();
		if(0 > ids) return ids;
		CString sMsg;
		sMsg.Format(_T("책이 대출 되었습니다."),nProcessingCnt);
		SelfCloseMessageBox(sMsg);

		CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
		INT nLoanRowCount = pDM->GetRowCount();
		this->SetEND_SBL(nLoanRowCount);

	}

	return 0;	

	EFS_END
		return -1;
}

INT CBL_LOC_LOAN_PROC::RFID_UpdateLoan(INT nMode, CString sSerialNo)
{
EFS_BEGIN
		
	INT				ids=0, nCnt=0;
	CRFID_Interface RFID(this->m_pParentMgr);	
	CString			sFuncInfo;

	
	for( nCnt = 0; nCnt < 2; nCnt++)
	{
		
		if(2 == nMode)	{ ids = RFID.ChangeTagLoanProcDataByUID(sSerialNo, nMode);	}
		else			{ ids = RFID.ChangeTagLoanProcDataByUID(sSerialNo, !nMode);	}

		if(0 == ids) break;
	}
	sFuncInfo.Format(_T("RFID_UpdateLoan 1-1 (%s, %d)"), sSerialNo, nMode);
	CLocCommonAPI::MakeRFIDLog(sFuncInfo, _T("RFID 대출/보안상태 업데이트"), __WFILE__, __LINE__, ids);

	return ids;

EFS_END
return -1;
}


INT CBL_LOC_LOAN_PROC::MoveGrid(CString strBookKey)
{
	EFS_BEGIN

	INT ids = 0;
	CString strFuncName = _T("MoveGrid");

	CString sDMAlias = _T("DM_BO_LOC_3100_LOAN_INFO");
	CESL_DataMgr *pDM = m_pParentMgr->FindDM( sDMAlias );
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	INT nRowCount = pDM->GetRowCount();

	CString sCMAlias = _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias = _T("MainGrid");

	CESL_Grid* pGrid = (CESL_Grid*)(m_pParentMgr->FindControl( sCMAlias , sGridAlias ));
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

	CString sWorkingStatus;
	for( INT i = nRowCount-1 ; i > -1 ; i-- )
	{
		CString strValue;
		ids = pDM->GetCellData( _T("책KEY"), i , strValue );
		if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

		if( strValue.Compare(strBookKey)==0 )
		{
			CESL_DataMgr* pReturnBasket = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_RETURN_INFO"));	
			if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

			INT nInsertIdx = pReturnBasket->GetRowCount();
			pReturnBasket->InsertRow(-1);

			INT nColCnt = pReturnBasket->GetRefColumnCount();
			for( INT j=0; j<nColCnt; j++ )
			{
				CString strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type;
				CString strResult;

				ids = pReturnBasket->GetColumnRefInfo(j, strField_Alias, strField_Name, strField_Type, strInit_Value, strData_Type);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

				ids = pDM->GetCellData(strField_Alias, i, strResult);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);

				ids = pReturnBasket->SetCellData(strField_Alias, strResult, nInsertIdx);
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}			

			
			if( m_pParentMgr->m_pInfo->MODE == 10000 )
			{
				ids = pReturnBasket->SetCellData(_T("R_DEVICE"), _T("KOLASIII-이동도서관"), nInsertIdx);
			}
			else
			{
				ids = pReturnBasket->SetCellData(_T("R_DEVICE"), _T("KOLASIII"), nInsertIdx);
			}
			if(0 > ids)
			{
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}

			
			ids = pReturnBasket->SetCellData(_T("R_WORKER"), m_pParentMgr->m_pInfo->USER_ID, nInsertIdx);
			if(0 > ids) 
			{
				if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
			}
			pDM->DeleteRow(i);
			if( pDM->GetRowCount() != 0 )
			{
				((CMSHFlexGrid*)pGrid)->RemoveItem(i+1);
				
				
				CString sIndex;
				for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
				{
					sIndex.Format(_T("%d"),i+1);
					pGrid->SetTextMatrix( i+1 , 0 , sIndex);
				}
			}
		}
	}

	if( pDM->GetRowCount() == 0 )
	{
		m_pParentMgr->AllControlDisplay( _T("CM_BO_LOC_3100_LOAN_INFO") );
	}
	else
	{
		m_pParentMgr->ControlDisplay(_T("CM_BO_LOC_3100_LOAN_INFO"), _T("MainGrid2"));
	}

	CESL_Grid* pGrid2 = (CESL_Grid*)(m_pParentMgr->FindControl( sCMAlias , _T("MainGrid2") ));
	pGrid2->SendMessage(WM_VSCROLL, SB_BOTTOM, 0 );
	return 0;

	EFS_END
	return -1;

}

INT CBL_LOC_LOAN_PROC::ISIllLoanMat(CString strLoanKey, BOOL& bIllReturnWill)
{
EFS_BEGIN
	INT ids;
	CESL_DataMgr TmpDM;
	TCHAR szSql[128];
	CString strValue=_T("");
	CString strFuncName = _T("ISIllLoanMat");

	bIllReturnWill = FALSE;

	
	if ( strLoanKey.GetLength() == 0 ) return 0;

	TmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	_stprintf( szSql, _T("SELECT IS_ILL_RETURN(%s, '%s', '', '' , '', '', '0') FROM DUAL"), strLoanKey, m_pParentMgr->m_pInfo->MANAGE_CODE);

	ids = TmpDM.GetOneValueQuery(szSql, strValue);
	if(0 > ids && ids != -4007){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);}

	
	if ( strValue != _T("0") ) {
		bIllReturnWill = FALSE;	
		return 0;
	}

	bIllReturnWill = TRUE;	
	return 0;	

EFS_END
	return -1;
}


INT CBL_LOC_LOAN_PROC::ISIllProcMat(CString strLoanKey, BOOL& bIllProcMat)
{
EFS_BEGIN
	INT ids;
	CESL_DataMgr TmpDM;
	TCHAR szSql[512];
	CString strValue=_T("");
	CString strFuncName = _T("ISIllProcMat");

	bIllProcMat = FALSE;

	TmpDM.SetCONNECTION_INFO(m_pParentMgr->CONNECTION_INFO);

	_stprintf( szSql, _T("SELECT WORKING_STATUS FROM BO_BOOK_TBL ")
		              _T("WHERE REC_KEY=(SELECT L.BOOK_KEY FROM LS_WORK_T01 L, ILL_TRANS_TBL I WHERE L.REC_KEY=I.LOAN_KEY AND L.PUBLISH_FORM_CODE='MO' AND REC_KEY=%s AND ROWNUM < 2) ")
					  _T("UNION ")
					  _T("SELECT WORKING_STATUS FROM SE_BOOK_TBL ")
		              _T("WHERE REC_KEY=(SELECT L.BOOK_KEY FROM LS_WORK_T01 L, ILL_TRANS_TBL I WHERE L.REC_KEY=I.LOAN_KEY AND L.PUBLISH_FORM_CODE='SE' AND REC_KEY=%s AND ROWNUM < 2) ")
					  , strLoanKey, strLoanKey);

	ids = TmpDM.GetOneValueQuery(szSql, strValue);
	if(0 > ids && ids != -4007){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);}

	
	if ( strValue == _T("BOL411O") || strValue == _T("SEL411O") ) {
		bIllProcMat = TRUE;			
	}
	
	return 0;	

EFS_END
	return -1;
}


INT CBL_LOC_LOAN_PROC::ISUnionDBLoan(CString strBarcode, CString strLibCode, BOOL& bUnionDBLoan)
{
	EFS_BEGIN
	INT ids;
	CESL_DataMgr TmpDM;
	TCHAR szSql[512];
	CString strValue=_T("");
	CString strFuncName = _T("ISUnionDBLoan");
	
	bUnionDBLoan = FALSE;	
	
	
	if ( strBarcode.GetLength() < 12 || strLibCode.GetLength() < 6 ) return 0;

	return 0;	
	
	EFS_END
		return -1;
}


INT CBL_LOC_LOAN_PROC::KLService11(CString strUserNo,     CString strBookLibCode, 
								   CString strReturnDate, CString strRetrunStatus,
								   CString strLoanKey,    CString strUserStatus, 
								   CString strLoanStopDate,  CString strLoanStopSetDate )
{
	
	
	CFrameWriter Writer(m_pParentMgr->m_pInfo);
	Writer.setCommand(_T("IL_K11_LOC_B01_SERVICE"));
	Writer.addRecord();	
	Writer.addElement(_T("통합대출자번호"), strUserNo);
	
	
	Writer.addElement(_T("대출도서관부호"), strBookLibCode);
	Writer.addElement(_T("반납도서관부호"), strBookLibCode);
	
	
	Writer.addElement(_T("반납일"), strReturnDate);			
	Writer.addElement(_T("상태"), strRetrunStatus);
	Writer.addElement(_T("로컬KEY"), strLoanKey);

	if ( strUserStatus != _T("0"))	
	{
		Writer.addElement(_T("회원상태"), strUserStatus);
		if (strLoanStopDate == _T("NULL")) {
			Writer.addElement(_T("대출정지일"), _T(""));
		}
		else Writer.addElement(_T("대출정지일"), strLoanStopDate);
		Writer.addElement(_T("대출정지부여일"), strLoanStopSetDate);
	}	
	else {
		Writer.addElement(_T("회원상태"), strUserStatus);
	}
	Writer.addElement(_T("지역내상호대차상태"), _T("Y"));
	
	
	CFrameSender Sender(m_pParentMgr->m_pInfo);
	Sender.SendTempTbl(&Writer, m_pParentMgr->GetWorkLogMsg(_T("통합회원책두레반납"), __WFILE__, __LINE__));

	return 0;	
}

INT CBL_LOC_LOAN_PROC::ILL_ReservationConvertToAPL()
{
EFS_BEGIN
		
	INT ids;
	CString strFuncName = _T("DelayLoanDate");
	
	CString sDMAlias	= _T("DM_BO_LOC_3100_LOAN_INFO");
	CString sCMAlias	= _T("CM_BO_LOC_3100_LOAN_INFO");
	CString sGridAlias	= _T("MainGrid");
	CString sTableName	= _T("LS_WORK_T01");
	
	CESL_DataMgr *pDM = m_pParentMgr->FindDM(sDMAlias);
	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_ControlMgr *pCM = m_pParentMgr->FindCM(sCMAlias);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sGridAlias);
	if(NULL == pGrid) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);	
	
	ids = pGrid->SelectMakeList();
	INT nIdx;
	nIdx = pGrid->SelectGetHeadPosition();
	INT nRowCount = pGrid->SelectGetCount();
	if(1 > nRowCount)
	{
		SelfCloseMessageBox(_T("선정된 예약 자료가 없습니다."));
		return 0;
	}
	if(1 != nRowCount)
	{
		SelfCloseMessageBox(_T("1개의 이력만 선정해주세요."));
		return 0;
	}
		
	CString strStatus, strLoanManageCode;
	strStatus = pDM->GetCellData(_T("대출상태"), nIdx);
	strLoanManageCode = pDM->GetCellData(_T("관리구분"), nIdx);
	if(_T("3") != strStatus)
	{
		
		SelfCloseMessageBox(_T("예약중인 자료만 선정해주세요."));
		
		return 0;
	}
		
	CString strRequestLibCode = _T("");
	CString strRequestLibName = _T("");
	CString strRequestManageCode = _T("");
	BOOL bLocalRequest = FALSE;
	
	CString strBookLibCode = _T("");
	CString strBookKey = _T("");
	strBookLibCode = pDM->GetCellData(_T("소장관도서관부호"), nIdx);
	strBookKey = pDM->GetCellData(_T("책KEY"), nIdx);	

	CILL_RESERVE_CONVERT_LIB_DLG dlg(m_pParentMgr);
	dlg.SetBookParm(strBookLibCode, strBookKey);
	if(IDOK != dlg.DoModal())
	{
		SelfCloseMessageBox(_T("취소되었습니다."));
		return 0;
	}
	else
	{
		bLocalRequest = TRUE;
		strRequestLibCode = dlg.m_strRequestLibCode;
		strRequestManageCode = dlg.m_strRequestManageCode;
		strRequestLibName = dlg.m_strRequestLibName;
	}

	CESL_DataMgr *pUserDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	
	CILL_APL_3100 Dlg(m_pParentMgr);
	Dlg.m_pInfo = m_pParentMgr->m_pInfo;
	
	Dlg.m_nMode = 2;
	Dlg.m_strReservationKey = pDM->GetCellData(_T("대출/반납정보KEY"), nIdx);
	Dlg.m_strUserNo = pUserDM->GetCellData(_T("대출자번호"), 0);
	Dlg.m_strUserNo2 = pUserDM->GetCellData(_T("대출자번호"), 0);
	Dlg.m_strUserKey = pUserDM->GetCellData(_T("대출자KEY"), 0);
	Dlg.m_strUserEmail = pUserDM->GetCellData(_T("이메일"), 0);
	Dlg.m_strUserCode = pUserDM->GetCellData(_T("대출자직급"), 0);
	Dlg.m_strUserName = pUserDM->GetCellData(_T("대출자이름"), 0);
	
	Dlg.m_strBookKey = pDM->GetCellData(_T("책KEY"), nIdx);
	Dlg.m_strBookTitle = pDM->GetCellData(_T("본표제"), nIdx);
	Dlg.m_strBookRegNo = pDM->GetCellData(_T("등록번호"), nIdx);
	Dlg.m_strPublishFormCode = pDM->GetCellData(_T("단행/연속구분"), nIdx);
	Dlg.m_strBookManageCode = strLoanManageCode;
	
	Dlg.m_strRequestLibName = strRequestLibName;
	Dlg.m_strRequestLibCode = strRequestLibCode;

	if(IDCANCEL == Dlg.DoModal())
	{
		
		if(Dlg.m_bAplOK)
		{
			ReservationCancel(TRUE);
			SelfCloseMessageBox(_T("책두레 신청 전환처리가 완료되었습니다."));
		}
	}

	return 0;
EFS_END
return 0;
}

INT CBL_LOC_LOAN_PROC::GetTransNo(CString &sTransNo)
{
EFS_BEGIN

	INT ids;
	CString strFuncName = _T("GetTransNo");

	CESL_DataMgr TmpDM;
  	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	CString sQuery;
 	sQuery.Format(	_T("SELECT ECO$F_GET_TRANS_NO$S('%s','%s') FROM DUAL")
 					, m_pParentMgr->m_pInfo->USER_ID, m_pParentMgr->m_pInfo->MANAGE_CODE);
 	ids = TmpDM.GetOneValueQuery(sQuery, sTransNo);
 	if(0 > ids || sTransNo.IsEmpty()) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(_T("MAX LAST_NUMBER COUNT") == sTransNo)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	else if(_T("LIB_CODE IS NOT FOUND") == sTransNo) 
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}

	return 0;

EFS_END
return -1;
}

INT CBL_LOC_LOAN_PROC::BookReturnTypeCheck(INT nIdx)
{

	CString strFuncName = _T("BookReturnTypeCheck");
	CString strManageDesc,								
			strMessage,									
			strBookLibCode,								
			strReturnLibCode,							
			strKLBookLibCode,							
			strLocal,									
			strTemp,
			strTemp2,
			strReturnOption								
			;
	
	BOOL	bBookLibILLUse					= FALSE,	
			bBookLibKLUse					= FALSE,	
			bReturnLibILLUse				= FALSE,	
			bReturnLibKLUse					= FALSE,	
			bReturnLibILLOtherReturnUse		= FALSE,	
			bKLUser							= FALSE,	
			bLILLUse						= FALSE,	
			bLILLData						= FALSE,	
			bILLData						= FALSE,	
			bBookLibOtherReturnUse			= FALSE,	
			bBookLibOtherReturnHoldUse		= FALSE,	
			bReturnLibOtherReturnUse		= FALSE,	
			bReturnLibOtherReturnHoldUse	= FALSE,	
			bOtherReturnOptionUse			= FALSE
			;
	
	int i = 0,
		idx = 0,
		nLoanStatus = 0;

	CESL_DataMgr	*pDM		= m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	CESL_DataMgr	*pMemberDM	= m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_MEMBER_INFO"));
	CESL_ControlMgr *pCM		= m_pParentMgr->FindCM(_T("CM_BO_LOC_3100_SEARCH_INFO"));


	if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	if(NULL == pMemberDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1001, strFuncName);
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1002, strFuncName);

	if(pDM->GetRowCount() == 0){
		
		pDM			= m_pParentMgr->FindDM(_T("DM_LR2_BO_BOOK_LIST"));
		pMemberDM	= m_pParentMgr->FindDM(_T("DM_LR2_BO_BOOK_LIST"));
		idx = nIdx;
		if(NULL == pDM) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1003, strFuncName);
		if(pDM->GetRowCount() == 0){
			return _RETURN_FAIL;
		}
	}

	if(pMemberDM->GetRowCount() == 0){
		return _RETURN_FAIL;
	}
	
	strBookLibCode		 = pDM->GetCellData( _T("소장관도서관부호"), nIdx);						
	strReturnLibCode	 = m_pParentMgr->m_pInfo->LIB_CODE;

	
	bBookLibILLUse				 = m_pManageValue->GetLibDMValue(strBookLibCode, _T("ILL_USE_YN"));
	bBookLibOtherReturnUse		 = m_pManageValue->GetLibDMValue(strBookLibCode, _T("분관반납사용여부"));
	bBookLibOtherReturnHoldUse	 = m_pManageValue->GetLibDMValue(strBookLibCode, _T("분관반납대기사용여부"));
	bBookLibKLUse				 = m_pManageValue->GetLibDMValue(strBookLibCode, _T("책이음사용여부"));

	bReturnLibILLUse			 = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("ILL_USE_YN"));
	bReturnLibILLOtherReturnUse	 = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("ILL_OTHER_RETURN_YN"));
	bReturnLibOtherReturnUse	 = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("분관반납사용여부"));
	bReturnLibOtherReturnHoldUse = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("분관반납대기사용여부"));
	bReturnLibKLUse				 = m_pManageValue->GetLibDMValue(strReturnLibCode, _T("책이음사용여부"));

	m_pManageValue->GetLibDMValue(strReturnLibCode, _T("타관반납종류"), strReturnOption);
	if(strReturnOption.IsEmpty() == TRUE)
		strReturnOption = _T("KL");

	strTemp				 = pDM->GetCellData( _T("책두레데이터여부"), nIdx);
	if(strTemp.Compare(_T("Y")) == 0)	 
		bILLData = TRUE;

	
	strTemp				 = pMemberDM->GetCellData( _T("통합회원여부"), idx);			
	if(strTemp.Compare(_T("Y")) == 0)		 
		bKLUser = TRUE;

	strKLBookLibCode	 = pDM->GetCellData( _T("대출도서관부호"), nIdx);	

	
	bLILLUse			 = m_pParentMgr->m_pInfo->m_bLillUse;
	
	strTemp				 = pDM->GetCellData( _T("LCT_KEY"), nIdx);						
	if(strTemp.IsEmpty() == FALSE)		 
		bLILLData = TRUE;

	
	strManageDesc		 = pDM->GetCellData( _T("관리구분설명"), nIdx);				
	strLocal			 = pDM->GetCellData( _T("현황구분"), nIdx);
	
	CString strLillOtherReturnYN = _T("");
	BOOL bLillOtherReturnYN = FALSE;
	GetTempData(_T("지역상호대차타관반납여부"),strLillOtherReturnYN);
	bLillOtherReturnYN = strLillOtherReturnYN == _T("Y");

	if(bLILLData == TRUE){
		nLoanStatus = _STATUS_LILL;
	}else if(bILLData == TRUE){
		nLoanStatus = _STATUS_ILL;
	}else if(strLocal.Compare(_T("로컬")) != 0){
		nLoanStatus = _STATUS_KL;
	}else{
		nLoanStatus = _STATUS_NORMAL;
	}

	if(nLoanStatus == _STATUS_LILL){
		if(bReturnLibKLUse == TRUE && bKLUser == TRUE){
			if(strReturnLibCode.Compare(strBookLibCode) == 0){
				return _RETURN_LILL_TO_BOOKLIB;
			}else{
				
				if(bLillOtherReturnYN == TRUE){
					return _RETURN_LILL_TO_LILL;
				} else {
					return _MESSAGE_NO_USE_KL_LILL;
				}
			}
		}else{
			return _MESSAGE_NO_USE_KL_LILL;

		}
	}

	if(nLoanStatus == _STATUS_ILL){
		if(bReturnLibILLUse == TRUE){
			if(strReturnLibCode.Compare(strBookLibCode) == 0){
				return _RETURN_ILL_TO_BOOKLIB;
			}else{
				return _RETURN_ILL_TO_ILL;
			}
		}else{
			return _MESSAGE_NO_USE_ILL;
		}
	}

	if(nLoanStatus == _STATUS_KL){
		if(bReturnLibKLUse == TRUE && bKLUser == TRUE){
			if(strReturnLibCode.Compare(strKLBookLibCode) == 0){
				return _RETURN_KL_TO_BOOKLIB;
			}else{
				
				if(bLillOtherReturnYN == TRUE){
					return _RETURN_KL_TO_KL;
				} else {
					return _MESSAGE_NO_USE_KL;
				}
			}
		}else{
			return _MESSAGE_NO_USE_KL;
		}
	}
	
	CButton *pOtherCheck = (CButton*)pCM->FindControl(_T("타관반납우선"));
	if(&pOtherCheck != NULL){
		if(TRUE == pOtherCheck->GetCheck()){
			bOtherReturnOptionUse = TRUE;
		}
	}	
	if(nLoanStatus == _STATUS_NORMAL){	
		if(strReturnLibCode.Compare(strBookLibCode) == 0){ 
			return _RETURN_NORMAL; 

		}else{
			if(bReturnLibILLUse == TRUE && bBookLibILLUse == TRUE && bReturnLibILLOtherReturnUse == TRUE){ 
				if(bOtherReturnOptionUse == TRUE && strReturnOption.Compare(_T("KL")) == 0 &&
					bReturnLibKLUse == TRUE && bBookLibKLUse == TRUE && bKLUser == TRUE	
					
					&& bLillOtherReturnYN == TRUE
					){
					return _RETURN_NORMAL_TO_KL; 
				}else{
					return _RETURN_NORMAL_TO_ILL; 
				}

			}else if(bReturnLibKLUse == TRUE && bBookLibKLUse == TRUE && bKLUser == TRUE 
				
				&& bLillOtherReturnYN == TRUE
				){ 
				if(bOtherReturnOptionUse == TRUE && strReturnOption.Compare(_T("ILL")) == 0 &&
					bReturnLibILLUse == TRUE && bBookLibILLUse == TRUE && bReturnLibILLOtherReturnUse == TRUE){
					return _RETURN_NORMAL_TO_ILL; 
				}else{
					return _RETURN_NORMAL_TO_KL; 
				}
			}else if(bReturnLibOtherReturnUse == TRUE && bBookLibOtherReturnUse == TRUE){ 
				if(bReturnLibOtherReturnHoldUse == TRUE && bBookLibOtherReturnHoldUse == TRUE){ 
					return _RETURN_OTHER_MANAGE_HOLD;	

				}else{
					return _RETURN_NORMAL; 

				}
			}
		}
	}

	return _RETURN_FAIL;
}

INT CBL_LOC_LOAN_PROC::BookReturnTypeCheck(CString strInputRegNo)
{
	int i = 0, ids = _RETURN_FAIL;
	CString strDMRegNo, strRegNo, strLogMsg;
	CESL_DataMgr	*pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));

	if(pDM->GetRowCount() == 0){

		return _RETURN_FAIL;
	}

	if(strInputRegNo.GetLength() > 12){
		strRegNo = strInputRegNo.Right(12);
	}else if(strInputRegNo.GetLength() < 12){
		strRegNo = strInputRegNo;
		CLocCommonAPI::MakeRegNoSpaceToZero(strRegNo);
	}else{
		strRegNo = strInputRegNo;
	}

	strLogMsg.Format(_T("입력:%s / 변환:%s\r\n"), strInputRegNo, strRegNo);
	pDM->QueryLog(strLogMsg.GetBuffer(0));

	for(i=0;i<pDM->GetRowCount();i++){
		strDMRegNo = pDM->GetCellData( _T("등록번호"), i);	

		if(strDMRegNo.Compare(strRegNo) == 0){
			ids = BookReturnTypeCheck(i);
			break;
		}
	}

	return ids;
}


int CBL_LOC_LOAN_PROC::ILL_UpdateTransInfo(INT bIsModeI, CString strData)
{
	CString strFuncName = _T("ILL_UpdateTransInfo");
	CString strTransNo;
	CString strLoanKey;
	CString strUpdateKeyAlias;
	INT ids = 0;

	CESL_DataMgr *pDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_DB"));
	if(NULL == pDM)
	{
		ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	}
	
	if(TRUE == bIsModeI)
	{
		CESL_DataMgr* pLOAN_DM = m_pParentMgr->FindDM(_T("DM_EXP_LOAN_BOOK_INFO"));
		if(NULL == pLOAN_DM)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);
		}
	
		ids = pLOAN_DM->GetCellData(_T("대출/반납정보KEY"), 0, strLoanKey);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1020, strFuncName);
		}

		strTransNo = strData;
		strData = strLoanKey;
	}
	else
	{
		strLoanKey = strData;
		CString strSQL;
		strSQL.Format(_T("SELECT TRANS_NO FROM ILL_TRANS_TBL WHERE LOAN_KEY = %s"), strLoanKey);
		ids = pDM->GetOneValueQuery(strSQL, strTransNo);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1030, strFuncName);
		}
	}

	CString strSQL;
	strSQL.Format(
		_T("UPDATE ILL_TRANS_TBL ITL ")
		_T("SET ")
		_T("(ITL.LS_LOAN_DATE, ITL.RETURN_PLAN_DATE, ITL.LAST_WORK, ITL.LOAN_KEY) = ")
		_T("(SELECT LS.LOAN_DATE, LS.RETURN_PLAN_DATE, '%s', LS.REC_KEY FROM LS_WORK_T01 LS WHERE LS.REC_KEY = %s)")
		_T("WHERE ITL.TRANS_NO = '%s';"),
		m_pParentMgr->GetWorkLogMsg(_T("대출"), __WFILE__, __LINE__),
		strData, strTransNo);
	
	pDM->AddFrame(strSQL);

	SetTempData(_T("SHARE_ILL_TRANS_TBL_INSERT_TRANS_NO"), CComVariant(strTransNo));

	return 0;
}


INT CBL_LOC_LOAN_PROC::ILL_BarCodeReturn(CString sBookKey)
{
	int ids = 0;
	INT i, j;
	CString strFuncName = _T("ILL_BarCodeReturn");

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	CString strQuery = _T("");
	CString strAppendixFlag = _T("");
	CString strDevice = _T("KOLASIII");
	CString strWorker = m_pParentMgr->m_pInfo->USER_ID;
	CString strLoanLibManageCode = _T("");

	CString sReturnType;
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), _T("반납형태"), sReturnType);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1010, strFuncName);	
	
	CString sReturnDate, strReturnDateString;
	CString sReturnDateControlAlias = _T("반납일");
	if(sReturnType.Compare(_T("2"))== 0)
		sReturnDateControlAlias = _T("소급반납일");
	
	ids = m_pParentMgr->GetControlData(_T("CM_LOAN_AND_RETURN_TYPE"), sReturnDateControlAlias, strReturnDateString);
	if(0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1000, strFuncName);
	
	sReturnDate.Format(_T("TO_DATE('%s','YYYY/MM/DD HH24:MI:SS')"), strReturnDateString);

	CESL_DataMgr TempDM;
	TempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	
	strQuery.Format(_T("SELECT CL.USER_NO, (SELECT ML.LIB_CODE FROM MN_LIBINFO2_TBL ML, BO_BOOK_TBL BB WHERE BB.REC_KEY = %s AND ML.MANAGE_CODE = BB.MANAGE_CODE AND ROWNUM < 2), ")
		
		_T("LW.REC_KEY, LW.LOAN_DATE, LW.RETURN_PLAN_DATE, LW.USER_KEY, LW.BOOK_KEY, LW.PUBLISH_FORM_CODE, ")
		
		_T("LW.REG_NO, LW.CALL_NO, LW.TITLE, LW.AUTHOR, LW.PUBLISHER, BB.VOL_TITLE, BB.BOOK_APPENDIX_FLAG, TO_CHAR(SYSDATE, 'YYYY/MM/DD'), CL.KL_MEMBER_YN ")
		
		_T(", (SELECT APL_LIBCODE FROM ILL_TRANS_TBL IT WHERE LW.REC_KEY=IT.LOAN_KEY AND ROWNUM =1 )")
		_T("FROM CO_LOAN_USER_TBL CL, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
		_T("WHERE CL.REC_KEY = LW.USER_KEY AND LW.BOOK_KEY = BB.REC_KEY AND LW.STATUS IN ('0','2','L','D') AND LW.BOOK_KEY = %s"),sBookKey, sBookKey);
	TempDM.RestructDataManager(strQuery);
	
	const INT nColCnt = 18;
	
	CString strValue[nColCnt] = 
	{
		_T(""), _T(""), _T(""), _T(""), _T(""), 
		_T(""), _T(""), _T(""), _T(""), _T(""), 
		_T(""), _T(""), _T(""), _T(""), _T(""), 
		_T(""), _T(""), _T("")
	};
	for( i=0; i<nColCnt;i++)
	{
		strValue[i] = TempDM.GetCellData(0,i);
	}

	CString strUserNo, strLoanLibCode, strReturnDate, strRetrunStatus, strLoanKey, strUserStatus, strLoanStopDate, strLoanStopSetDate, strKLMemberYN;
	CString strUserKey;
	CString strBookLibCode;
	strBookLibCode = strValue[1];

	strUserNo = _T("");
	strLoanLibCode = _T("");
	strReturnDate = _T("");
	strRetrunStatus = _T("");
	strLoanKey = _T("");
	strUserStatus = _T("0");
	strLoanStopDate = _T("NULL");
	strLoanStopSetDate = _T("");
	strKLMemberYN      = strValue[16];
	strUserKey = strValue[5];

	
	strLoanLibCode = strValue[17];
	
	if ( strLoanLibCode.GetLength() == 0 ) {
		strLoanLibCode = strBookLibCode;
	}	
	if ( strBookLibCode.GetLength() == 0 ) {
		AfxMessageBox(_T("소장관정보가 없어 책두레반납 처리불가. ILL_BarCodeReturn() Error!"));
		return 0;
	}	

	strUserNo = strValue[0];	
	strReturnDate = strValue[15];
	strRetrunStatus = _T("R");
	strLoanKey = strValue[2];

	TmpDM.StartFrame();	
	
	if(strBookLibCode.Compare(m_pParentMgr->m_pInfo->LIB_CODE) == 0 )
	{
		strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = %s, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN ('L','D');"),
						sReturnDate, m_pParentMgr->GetWorkLogMsg(_T("책두레 자관반납"), __WFILE__, __LINE__),strDevice, strWorker, sBookKey );		
		TmpDM.AddFrame(strQuery);
		
		strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET LS_RETURN_DATE = %s, RETURN_LIBCODE = '%s', LAST_WORK = '%s',  END_DATE = SYSDATE, END_USERID = '%s', END_DIV = '0010', STATE = '0009' WHERE STATE = '0014' AND BOOK_KEY = %s;"),
						sReturnDate, m_pParentMgr->m_pInfo->LIB_CODE, m_pParentMgr->GetWorkLogMsg(_T("책두레 자관반납"), __WFILE__, __LINE__), strWorker, sBookKey);
		
		TmpDM.AddFrame(strQuery);

		
		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET PREV_WORKING_STATUS = WORKING_STATUS, WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;"),sBookKey);
		TmpDM.AddFrame(strQuery);
		
	}
	else
	{
		CESL_DataMgr Temp;
		CString strIllCnt = _T("");
		Temp.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
		strQuery.Format(_T("SELECT TRANS_NO FROM ILL_TRANS_TBL WHERE BOOK_KEY = %s AND STATE = '0014'"),sBookKey);
		Temp.RestructDataManager(strQuery);

		INT nRowCnt = 0;
		nRowCnt = Temp.GetRowCount();
		
		
		if(nRowCnt == 0)
		{
			CESL_DataMgr TempDM;
			TempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
			
			strQuery.Format(_T("SELECT CL.USER_NO, (SELECT ML.LIB_CODE FROM MN_LIBINFO2_TBL ML, BO_BOOK_TBL BB WHERE BB.REC_KEY = %s AND ML.MANAGE_CODE = BB.MANAGE_CODE), ")
			
				_T("LW.REC_KEY, LW.LOAN_DATE, LW.RETURN_PLAN_DATE, LW.USER_KEY, LW.BOOK_KEY, LW.PUBLISH_FORM_CODE, ")
			
				_T("LW.REG_NO, LW.CALL_NO, LW.TITLE, LW.AUTHOR, LW.PUBLISHER, BB.VOL_TITLE, BB.BOOK_APPENDIX_FLAG ")
				_T("FROM CO_LOAN_USER_TBL CL, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
				_T("WHERE CL.REC_KEY = LW.USER_KEY AND LW.BOOK_KEY = BB.REC_KEY AND LW.STATUS IN ('0','2') AND LW.BOOK_KEY = %s"),sBookKey, sBookKey);
			TempDM.RestructDataManager(strQuery);
			
			const INT nColCnt = 15;
			CString strValue[nColCnt] = 
			{
				_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), 
				_T(""), _T(""), _T(""), _T(""), _T("")
			};
			for( j=0; j<nColCnt;j++)
			{
				strValue[j] = TempDM.GetCellData(0,j);
			}					
			
			
			strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = %s, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN ('0','2');"),
							sReturnDate, m_pParentMgr->GetWorkLogMsg(_T("책두레 타관반납"), __WFILE__, __LINE__),strDevice, strWorker, sBookKey );

			TmpDM.AddFrame(strQuery);
			
			strQuery.Format(_T("INSERT INTO ILL_TRANS_TBL(TRANS_NO, STATE, USER_NO, GIVE_LIBCODE, RETURN_LIBCODE, ")
				_T("LOAN_KEY, LS_LOAN_DATE, RETURN_PLAN_DATE, LS_RETURN_DATE, USER_KEY, BOOK_KEY, PUBLISH_FORM_CODE, ")
				_T("REG_NO, CALL_NO, TITLE_INFO, AUTHOR_INFO, PUBLISHER_INFO, VOL_INFO, FIRST_WORK, LAST_WORK, BOOK_APPENDIX_FLAG) ")
				_T("SELECT TRANS_NO_SEQ.NEXTVAL, '0014', '%s', '%s', '%s', LS.REC_KEY, LS.LOAN_DATE, SYSDATE, LS.RETURN_DATE, LS.USER_KEY, LS.BOOK_KEY, LS.PUBLISH_FORM_CODE, LS.REG_NO, ")
				_T("LS.CALL_NO, LS.TITLE, LS.AUTHOR, LS.PUBLISHER, LS.VOL_INFO, '%s', '%s', '%s' FROM LS_WORK_T01 LS WHERE LS.REC_KEY = %s; "),
				strValue[0], strValue[1], m_pParentMgr->m_pInfo->LIB_CODE,
				m_pParentMgr->GetWorkLogMsg(_T("책두레 타관반납"), __WFILE__, __LINE__), m_pParentMgr->GetWorkLogMsg(_T("책두레 타관반납"), __WFILE__, __LINE__), strValue[14],strValue[2]);

			TmpDM.AddFrame(strQuery);
			
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET PREV_WORKING_STATUS = 'BOL112N' , WORKING_STATUS = 'BOL411O' WHERE REC_KEY = %s;"),sBookKey);
			TmpDM.AddFrame(strQuery);
		}		
		else
		{
			strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = %s, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN('L','D');"),
							sReturnDate, m_pParentMgr->GetWorkLogMsg(_T("책두레 바코드반납"), __WFILE__, __LINE__),strDevice, strWorker, sBookKey );

			TmpDM.AddFrame(strQuery);

			
			strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET LS_RETURN_DATE = %s, RETURN_PLAN_DATE = SYSDATE, RETURN_LIBCODE = '%s', LAST_WORK = '%s' WHERE STATE = '0014' AND BOOK_KEY = %s;"),
							sReturnDate, m_pParentMgr->m_pInfo->LIB_CODE, m_pParentMgr->GetWorkLogMsg(_T("책두레 바코드반납"), __WFILE__, __LINE__), sBookKey);

			TmpDM.AddFrame(strQuery);
		}
	}

	TmpDM.SendFrame();
	TmpDM.EndFrame();

	TmpDM.StartFrame();
	
	CESL_DataMgr *pLoanDM = m_pParentMgr->FindDM(_T("DM_BO_LOC_3100_LOAN_INFO"));
	
	CString strDelayDay;
	INT nDelayDay = 0;
	CString strTmpKey;
	

	for( i=0;i<pLoanDM->GetRowCount();i++)
	{
		pLoanDM->GetCellData(_T("책KEY"),i,strTmpKey);
		if(strTmpKey == sBookKey)
		{
			pLoanDM->GetCellData(_T("연체일수"),i,strDelayDay);
			pLoanDM->GetCellData(_T("대출/반납정보KEY"),i,strLoanKey);
			strQuery.Format(_T("SELECT USER_KEY FROM LS_WORK_T01 WHERE REC_KEY = %s"),strLoanKey);
			TmpDM.GetOneValueQuery(strQuery,strUserKey);
		}
	}
		
	if(_T("1") == m_pManageValue->m_sLoanStopMode)
	{ 
		nDelayDay += _ttoi(strDelayDay);
	}
	else
	{ 
		if(nDelayDay < _ttoi(strDelayDay))
		{
			nDelayDay = _ttoi(strDelayDay);
		}
	}
		
	CString strLoanCount;
	strQuery.Format(_T("(SELECT DECODE(SIGN(NVL(LOAN_COUNT,0)-1),1,LOAN_COUNT-1,0) ")
		_T("FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s)"),strUserKey);
	TmpDM.GetOneValueQuery(strQuery,strLoanCount);
		
	
	if(0 < nDelayDay)
	{
		CString strQuery, strValue;
		if(_T("1") == m_pManageValue->m_sLoanStopMode)
		{ 
			strQuery.Format(_T("SELECT TO_CHAR(LOAN_STOP_DATE,'YYYY/MM/DD') FROM CO_LOAN_USER_TBL WHERE REC_KEY=%s"), strUserKey);
			TmpDM.GetOneValueQuery(strQuery, strValue);
			if(strValue.IsEmpty())
			{
				strQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(0,TO_DATE(SYSDATE),'%s',%d),'YYYY/MM/DD') FROM DUAL"),m_pParentMgr->m_pInfo->MANAGE_CODE, nDelayDay);
			}
			else
			{
				CString strReturnDateQuery;
				strReturnDateQuery.Format(_T("SELECT RETURN_DATE FROM LS_WORK_T01 WHERE REC_KEY = %s"),strLoanKey);
				TmpDM.GetOneValueQuery(strReturnDateQuery,m_strReturnDate);
				CTime tPreLSD(_ttoi(strValue.Mid(0, 4)), _ttoi(strValue.Mid(5, 2)), _ttoi(strValue.Mid(8, 2)), 0, 0, 0);
				CTime tReturn(_ttoi(m_strReturnDate.Mid(0, 4)), _ttoi(m_strReturnDate.Mid(5, 2)), _ttoi(m_strReturnDate.Mid(8, 2)), 0, 0, 0);
				if(tReturn <= tPreLSD)
				{
					CTimeSpan tOne(1,0,0,0);
					CTime tSetDate = tPreLSD + tOne;
					CString strSetDate;
					strSetDate.Format(_T("%04d/%02d/%02d"), tSetDate.GetYear(), tSetDate.GetMonth(), tSetDate.GetDay());
					strQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(0,TO_DATE('%s','YYYY/MM/DD'),'%s',%d),'YYYY/MM/DD') FROM DUAL"), strSetDate,m_pParentMgr->m_pInfo->MANAGE_CODE,nDelayDay);
				}
				else
				{
					strQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(0,TO_DATE(SYSDATE),'%s',%d),'YYYY/MM/DD') FROM DUAL"), m_pParentMgr->m_pInfo->MANAGE_CODE,nDelayDay);
				}
			}
		}
		else
		{ 
			strQuery.Format(_T("SELECT TO_CHAR(ECO$F_GET_LOANSTOPDATE(0,TO_DATE(SYSDATE),'%s',%d),'YYYY/MM/DD') FROM DUAL"), m_pParentMgr->m_pInfo->MANAGE_CODE, nDelayDay);
		}
		CString strLoanStopDate;
		ids = TmpDM.GetOneValueQuery(strQuery, strValue);
		if(0 > ids && ids != -4007){ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);}
		if(strValue.IsEmpty())
		{
			strLoanStopDate = _T("NULL");
		}
		else
		{
			strLoanStopDate.Format(_T("TO_DATE('%s','YYYY/MM/DD')"), strValue);
		}
	
		if(_T("NULL") != strLoanStopDate)
		{
			
			CString strReturnExpireDateQuery, strPreLoanStopDate;
			strReturnExpireDateQuery.Format(_T("SELECT LOAN_STOP_DATE FROM CO_LOAN_USER_TBL WHERE REC_KEY = %s"),strUserKey);
			TmpDM.GetOneValueQuery(strReturnExpireDateQuery, strPreLoanStopDate);
			
			if(strPreLoanStopDate != _T(""))
			{
				CTime tPreLSD(_ttoi(strPreLoanStopDate.Mid(0, 4)), _ttoi(strPreLoanStopDate.Mid(5, 2)), _ttoi(strPreLoanStopDate.Mid(8, 2)), 0, 0, 0);
				CTime tNewLSD(_ttoi(strValue.Mid(0, 4)), _ttoi(strValue.Mid(5, 2)), _ttoi(strValue.Mid(8, 2)), 0, 0, 0);
				
				if(tNewLSD >= tPreLSD)
				{
					strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET USER_CLASS = DECODE(USER_CLASS,'0','1',USER_CLASS), ")
						_T("LOAN_STOP_DATE = %s, LOAN_STOP_SET_DATE = SYSDATE, DELAYDAY_COUNT = DELAYDAY_COUNT+%d, DELAY_COUNT = DELAY_COUNT+1 ")
						_T("WHERE REC_KEY = %s;"),strLoanStopDate, nDelayDay, strUserKey);
					TmpDM.AddFrame(strQuery);
				}
				
				strUserStatus = _T("1");				
				strLoanStopSetDate = strReturnDate;				
			}
			
			else
			{
				strQuery.Format(_T("UPDATE CO_LOAN_USER_TBL SET USER_CLASS = DECODE(USER_CLASS,'0','1',USER_CLASS), ")
					_T("LOAN_STOP_DATE = %s, LOAN_STOP_SET_DATE = SYSDATE, DELAYDAY_COUNT = DELAYDAY_COUNT+%d, DELAY_COUNT = DELAY_COUNT+1 ")
					_T("WHERE REC_KEY = %s;"),strLoanStopDate, nDelayDay, strUserKey);
					TmpDM.AddFrame(strQuery);
				
				
				strUserStatus = _T("1");				
				strLoanStopSetDate = strReturnDate;
			}
		}
	}

	TmpDM.SendFrame();
	TmpDM.EndFrame();
	

	if ( strKLMemberYN == _T("Y") && m_pManageValue->m_sIsUnityLoanService == _T("Y") ) {
		
		ids = KLService11(strUserNo, strBookLibCode, strReturnDate, strRetrunStatus, strLoanKey, strUserStatus, strLoanStopDate, strLoanStopSetDate);	
	}

	if(strBookLibCode.Compare(m_pParentMgr->m_pInfo->LIB_CODE) == 0 ) {
		SPFExcutePath(_T("반납된자료예약만기일부여"));
	}
	
	ILL_APPENDIX_RETURN(sBookKey, strUserStatus, strLoanStopDate, strLoanStopSetDate, strUserKey);
	
	
	ILL_APPENDIX_REMAIN(sBookKey);	

	if ( strBookLibCode.Compare(m_pParentMgr->m_pInfo->LIB_CODE) != 0) {
		SelfCloseMessageBox(_T("소장관이 다른 책의 책두레 반납이 완료되었습니다."), m_pParentMgr, 3000);	
	}	
	
	
	ids = MoveGrid(sBookKey);
	if( 0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);	

	
	return ids;
}

INT CBL_LOC_LOAN_PROC::ILL_APPENDIX_RETURN(CString sBookKey, CString strpUserStatus, CString strpLoanStopDate, CString strpLoanStopSetDate, CString strUserKey)
{
	INT ids, i, j, k;
	CString strQuery;
	CString strAppendixFlag;
	CString strDevice = _T("KOLASIII");
	CString strWorker = m_pParentMgr->m_pInfo->USER_ID;
	CString strFuncName = _T("ILL_APPENDIX_RETURN");

	CESL_DataMgr TmpDM;	
	TmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	
	
	strQuery.Format(_T("SELECT BOOK_APPENDIX_FLAG FROM BO_BOOK_TBL WHERE REC_KEY = %s"),sBookKey);
	TmpDM.GetOneValueQuery(strQuery,strAppendixFlag);

	if(strAppendixFlag.Compare(_T("B"))!=0) return 0;
	
	
	
	CString strSpeciesKey = _T("");
	strQuery.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s"),sBookKey);
	TmpDM.GetOneValueQuery(strQuery,strSpeciesKey);
	
	
	INT nAppendixCnt = 0;
	
	CESL_DataMgr AppendixTmpDM;
	AppendixTmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
	
	
	strQuery.Format(_T("SELECT LW.BOOK_KEY FROM LS_WORK_T01 LW, BO_BOOK_TBL BO WHERE BO.BOOK_APPENDIX_FLAG='A' AND LW.BOOK_KEY = BO.REC_KEY AND LW.STATUS IN ('0','2','L','D') AND BO.SPECIES_KEY = '%s' AND LW.RETURN_DATE IS NULL AND LW.USER_KEY = %s"),strSpeciesKey,strUserKey);
			
	AppendixTmpDM.RestructDataManager(strQuery);
	nAppendixCnt = AppendixTmpDM.GetRowCount();

	
	if ( nAppendixCnt <= 0 ) return 0;			
	
	CString strMessage = _T("");
	strMessage.Format(_T("반납 대상 부록자료가 %d건 있습니다.\r\n 일괄 반납하시겠습니까?"),nAppendixCnt);
	if(IDYES != (AfxMessageBox(strMessage, MB_ICONQUESTION | MB_YESNO))) return 0;
			
	CESL_DataMgr SecTmpDM;
	SecTmpDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);	
	
	for( i=0;i<nAppendixCnt;i++)
	{
		CString strAppendixBookKey = AppendixTmpDM.GetCellData(i,0);
		
		CString strLoanLibManageCode = _T("");				
		strQuery.Format(_T("SELECT MANAGE_CODE FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND STATUS IN ('0','2','L','D') "),strAppendixBookKey);
		TmpDM.GetOneValueQuery(strQuery,strLoanLibManageCode);

		if (strLoanLibManageCode.GetLength() <= 0 ) {
			AfxMessageBox(_T("대출관정보가 없어 책두레반납 처리불가. ILL_APPENDIX_RETURN() Error!"));
			return 0;
		}

		CESL_DataMgr TempDM;
		TempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
		
		strQuery.Format(_T("SELECT CL.USER_NO, (SELECT ML.LIB_CODE FROM MN_LIBINFO2_TBL ML, BO_BOOK_TBL BB WHERE BB.REC_KEY = %s AND ML.MANAGE_CODE = BB.MANAGE_CODE AND ROWNUM < 2), ")
			
			_T("LW.REC_KEY, LW.LOAN_DATE, LW.RETURN_PLAN_DATE, LW.USER_KEY, LW.BOOK_KEY, LW.PUBLISH_FORM_CODE, ")
			
			_T("LW.REG_NO, LW.CALL_NO, LW.TITLE, LW.AUTHOR, LW.PUBLISHER, BB.VOL_TITLE, BB.BOOK_APPENDIX_FLAG, TO_CHAR(SYSDATE, 'YYYY/MM/DD'), KL_MEMBER_YN ")
			
		    _T(", (SELECT APL_LIBCODE FROM ILL_TRANS_TBL IT WHERE LW.REC_KEY=IT.LOAN_KEY AND ROWNUM =1 )")
			_T("FROM CO_LOAN_USER_TBL CL, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
			_T("WHERE CL.REC_KEY = LW.USER_KEY AND LW.BOOK_KEY = BB.REC_KEY AND LW.STATUS IN ('0','2','L','D') AND LW.BOOK_KEY = %s"),strAppendixBookKey, strAppendixBookKey);
		TempDM.RestructDataManager(strQuery);
		
		const INT nColCnt = 18;
		
		CString strValue[nColCnt] = 
		{
			_T(""), _T(""), _T(""), _T(""), _T(""), 
			_T(""), _T(""), _T(""), _T(""), _T(""), 
			_T(""), _T(""), _T(""), _T(""), _T(""), 
			_T(""), _T(""), _T("")
		};
		for( j=0; j<nColCnt;j++)
		{
			strValue[j] = TempDM.GetCellData(0,j);
		}
		
		CString strUserNo, strLoanLibCode, strReturnDate, strRetrunStatus, strLoanKey, strUserStatus, strLoanStopDate, strLoanStopSetDate, strKLMemberYN;
		
		strUserNo = _T("");
		strLoanLibCode = _T("");
		strReturnDate = _T("");
		strRetrunStatus = _T("");
		strLoanKey = _T("");
		strUserStatus = strpUserStatus;
		strLoanStopDate = strpLoanStopDate;
		strLoanStopSetDate = strpLoanStopSetDate;
		strKLMemberYN = strValue[16];

		CString strBookLibCode;
		strBookLibCode = strValue[1];
		strLoanLibCode = strValue[17];
		
		if ( strLoanLibCode.GetLength() == 0 ) {
			strLoanLibCode = strBookLibCode;
		}		
		
		if ( strLoanLibCode.GetLength() == 0 ) {
			AfxMessageBox(_T("소장관정보가 없어 책두레반납 처리불가. ILL_BarCodeReturn() Error!"));
			return 0;
		}
		
		strUserNo = strValue[0];		
		strReturnDate = strValue[15];
		strRetrunStatus = _T("R");
		strLoanKey = strValue[2];

		SecTmpDM.StartFrame();
		
		if(strBookLibCode.Compare(m_pParentMgr->m_pInfo->LIB_CODE) == 0 )		
		{
			strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = SYSDATE, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN ('L','D');"),
			m_pParentMgr->GetWorkLogMsg(_T("책두레 자관반납"), __WFILE__, __LINE__),strDevice, strWorker, strAppendixBookKey );
			
			SecTmpDM.AddFrame(strQuery);
			
			
			strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET LS_RETURN_DATE = SYSDATE, RETURN_PLAN_DATE = SYSDATE, RETURN_LIBCODE = '%s', LAST_WORK = '%s',  END_DATE = SYSDATE, END_USERID = '%s', END_DIV = '0010', STATE = '0009' WHERE STATE = '0014' AND BOOK_KEY = %s;"),
				m_pParentMgr->m_pInfo->LIB_CODE, m_pParentMgr->GetWorkLogMsg(_T("책두레 자관반납"), __WFILE__, __LINE__), strWorker, strAppendixBookKey);
			SecTmpDM.AddFrame(strQuery);

			
			strQuery.Format(_T("UPDATE BO_BOOK_TBL SET PREV_WORKING_STATUS = WORKING_STATUS, WORKING_STATUS = 'BOL112N' WHERE REC_KEY = %s;"),strAppendixBookKey);
			SecTmpDM.AddFrame(strQuery);
			
		}
		else
		{
			CESL_DataMgr Temp;
			CString strIllCnt = _T("");
			Temp.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
			strQuery.Format(_T("SELECT TRANS_NO FROM ILL_TRANS_TBL WHERE BOOK_KEY = %s AND STATE = '0014'"),strAppendixBookKey);
			Temp.RestructDataManager(strQuery);

			INT nRowCnt = 0;
			nRowCnt = Temp.GetRowCount();
			
			
			if(nRowCnt == 0)
			{
				CESL_DataMgr TempDM;
				TempDM.SetCONNECTION_INFO(m_pParentMgr->m_pInfo->CONNECTION_INFO);
				
				strQuery.Format(_T("SELECT CL.USER_NO, (SELECT ML.LIB_CODE FROM MN_LIBINFO2_TBL ML, BO_BOOK_TBL BB WHERE BB.REC_KEY = %s AND ML.MANAGE_CODE = BB.MANAGE_CODE), ")
				
					_T("LW.REC_KEY, LW.LOAN_DATE, LW.RETURN_PLAN_DATE, LW.USER_KEY, LW.BOOK_KEY, LW.PUBLISH_FORM_CODE, ")
				
					_T("LW.REG_NO, LW.CALL_NO, LW.TITLE, LW.AUTHOR, LW.PUBLISHER, BB.VOL_TITLE, BB.BOOK_APPENDIX_FLAG ")
					_T("FROM CO_LOAN_USER_TBL CL, LS_WORK_T01 LW, BO_BOOK_TBL BB ")
					_T("WHERE CL.REC_KEY = LW.USER_KEY AND LW.BOOK_KEY = BB.REC_KEY AND LW.STATUS IN ('0','2') AND LW.BOOK_KEY = %s"),strAppendixBookKey, strAppendixBookKey);
				TempDM.RestructDataManager(strQuery);
				
				const INT nColCnt = 15;
				CString strValue[nColCnt] = 
				{
					_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), 
					_T(""), _T(""), _T(""), _T(""), _T("")
				};
				for( k=0; k<nColCnt;k++)
				{
					strValue[k] = TempDM.GetCellData(0,k);
				}	
				
				strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = SYSDATE, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN ('0','2');"),
					m_pParentMgr->GetWorkLogMsg(_T("책두레 타관반납"), __WFILE__, __LINE__),strDevice, strWorker, strAppendixBookKey );
				SecTmpDM.AddFrame(strQuery);

				strQuery.Format(_T("INSERT INTO ILL_TRANS_TBL(TRANS_NO, STATE, USER_NO, GIVE_LIBCODE, RETURN_LIBCODE, ")
					_T("LOAN_KEY, LS_LOAN_DATE, RETURN_PLAN_DATE, LS_RETURN_DATE, USER_KEY, BOOK_KEY, PUBLISH_FORM_CODE, ")
					_T("REG_NO, CALL_NO, TITLE_INFO, AUTHOR_INFO, PUBLISHER_INFO, VOL_INFO, FIRST_WORK, LAST_WORK, BOOK_APPENDIX_FLAG) ")
					_T("SELECT TRANS_NO_SEQ.NEXTVAL, '0014', '%s', '%s', '%s', LS.REC_KEY, LS.LOAN_DATE, LS.RETURN_DATE, LS.RETURN_DATE, LS.USER_KEY, LS.BOOK_KEY, LS.PUBLISH_FORM_CODE, LS.REG_NO, ")
					_T("LS.CALL_NO, LS.TITLE, LS.AUTHOR, LS.PUBLISHER, LS.VOL_INFO, '%s', '%s', '%s' FROM LS_WORK_T01 LS WHERE LS.REC_KEY = %s; "),strValue[0], strValue[1], m_pParentMgr->m_pInfo->LIB_CODE,
					m_pParentMgr->GetWorkLogMsg(_T("책두레 타관반납"), __WFILE__, __LINE__), m_pParentMgr->GetWorkLogMsg(_T("책두레 타관반납"), __WFILE__, __LINE__), strValue[14],strValue[2]);
				SecTmpDM.AddFrame(strQuery);

				
				strQuery.Format(_T("UPDATE BO_BOOK_TBL SET PREV_WORKING_STATUS = 'BOL112N' , WORKING_STATUS = 'BOL411O' WHERE REC_KEY = %s;"),strAppendixBookKey);
				SecTmpDM.AddFrame(strQuery);
			}
			else
			{				
				strQuery.Format(_T("UPDATE LS_WORK_T01 SET RETURN_DATE = SYSDATE, STATUS = 'R', RETURN_TYPE_CODE = '0', LAST_WORK = '%s', R_DEVICE = '%s', R_WORKER = '%s' WHERE BOOK_KEY = %s AND STATUS IN('L','D');"),
					m_pParentMgr->GetWorkLogMsg(_T("책두레 바코드반납"), __WFILE__, __LINE__),strDevice, strWorker, strAppendixBookKey );
				SecTmpDM.AddFrame(strQuery);

				strQuery.Format(_T("UPDATE ILL_TRANS_TBL SET LS_RETURN_DATE = SYSDATE, RETURN_PLAN_DATE = SYSDATE, RETURN_LIBCODE = '%s', LAST_WORK = '%s' WHERE STATE = '0014' AND BOOK_KEY = %s;"),
					m_pParentMgr->m_pInfo->LIB_CODE, m_pParentMgr->GetWorkLogMsg(_T("책두레 바코드반납"), __WFILE__, __LINE__), strAppendixBookKey);		
				SecTmpDM.AddFrame(strQuery);
			}			
		}

		SecTmpDM.SendFrame();
		SecTmpDM.EndFrame();

		if ( strKLMemberYN == _T("Y") && m_pManageValue->m_sIsUnityLoanService == _T("Y") ) {			
			ids = KLService11(strUserNo, strBookLibCode, strReturnDate, strRetrunStatus, strLoanKey, strUserStatus, strLoanStopDate, strLoanStopSetDate);
			
		}

		ids = MoveGrid(strAppendixBookKey);
		if( 0 > ids) ERROR_MSG_RETURN_ERROR_CODE(m_pParentMgr->m_hWnd, -1050, strFuncName);
	}				

	return ids;
}
INT CBL_LOC_LOAN_PROC::ILL_APPENDIX_REMAIN(CString sBookKey)
{
	INT ids;
	CString strQuery = _T(""), strMsg = _T(""), strResult = _T("");

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO( m_pParentMgr->m_pInfo->CONNECTION_INFO );
	strQuery.Format(_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE SPECIES_KEY = ( SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY=%s ) AND STATE IN ('0001', '0002','0004', '0014') AND LOAN_KEY IS NULL AND BOOK_APPENDIX_FLAG='A'"), sBookKey);
	ids = TmpDM.GetOneValueQuery( strQuery, strResult );
	
	if ( _ttoi(strResult) > 0 ) {
		CString strMsg;
		strMsg.Format(_T("반납처리된 모체 자료중에 %s건의 부록자료가 대출을 위해 책두레진행중입니다.\n부록만 별도로 대출하려는 경우가 아니라면 확인하십시오."), strResult);
		AfxMessageBox(strMsg);
	}

	return 0;
}