// LoanShareManager.cpp : implementation file
//

#include "stdafx.h"
#include "LoanShareManager.h"
#include "direct.h"
#include "LoanShareWaitMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoanShareManager dialog

#define UPLOAD_CHECK_TIME 5000
#define UPLOAD_CHECK_TIMER_ID 1

// 쓰레드 구동 함수
UINT RunUploadThread( LPVOID pParam )
{
	CLoanShareManager* pLoanShareManager = (CLoanShareManager*)pParam;
	pLoanShareManager->SetThreadStatus(TRUE);
	pLoanShareManager->UploadToCenter();
	pLoanShareManager->SetThreadStatus(FALSE);
	
	return 0;
}

CLoanShareManager::CLoanShareManager(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	if( pParentMgr != NULL && pParentMgr->m_pInfo != NULL )
	{
		m_pInfo = pParentMgr->m_pInfo;
	}

	// 2009.06.30 UPDATE BY PWR : 사용안하는 클래스의 포인트를 가져올필요없이 바로 (CShareInfoCenterUp)로 가져오도록 수정
	m_pShareInfoCenterUp = NULL;
// 	m_pLoanUserCenterUp = NULL;
// 	m_pLoanInfoCenterUp = NULL;
// 	m_pUClassDelayInfoCenterUp = NULL;
// 	m_pUCARDelayInfoCenterUp = NULL;
// 	m_pHolydayInfoCenterUp = NULL;
// 	m_pCommonDelayEvnCenterUp = NULL;
	
	DoLoad();
}

CLoanShareManager::~CLoanShareManager()
{
	UnLoad();
}

VOID CLoanShareManager::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoanShareManager)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}

BEGIN_MESSAGE_MAP(CLoanShareManager, CDialog)
	//{{AFX_MSG_MAP(CLoanShareManager)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_START_CHECK, OnStartCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoanShareManager message handlers

VOID CLoanShareManager::OnTimer(UINT nIDEvent) 
{
EFS_BEGIN

	if(nIDEvent == UPLOAD_CHECK_TIMER_ID)
	{
		KillTimer(UPLOAD_CHECK_TIMER_ID);
		// 0. 처리할 자료가 존재하는지 확인
		if(m_UploadList.GetSize() > 0)
		{
			// 1. 쓰레드가 돌고 있는지 확인.
			if(!m_bRunThread)
			{ // 2. 돌고 있지 않다면 쓰레드 돌리기
				m_pUploadThread = AfxBeginThread(RunUploadThread, this, THREAD_PRIORITY_IDLE);
			}
		}
		SetTimer( UPLOAD_CHECK_TIMER_ID  , UPLOAD_CHECK_TIME , NULL );
	}

	CDialog::OnTimer(nIDEvent);

EFS_END
}

INT CLoanShareManager::DoLoad()
{
EFS_BEGIN

	// 쓰레드 상태 적용
	m_bRunThread = FALSE;

	// DM 로드 .. 클라이언트 오라클에서 쿼리를 날릴수 있도록
	INT ids = InitESL_Mgr(_T("SM_LOAN_INFO_SHARE_MANAGER"));
	if( ids < 0 ) 
	{
		CString sEMsg;
		sEMsg.Format(_T("Init ESLMgr Error!! [%d]"), ids);
		AfxMessageBox(sEMsg);
		m_sShareSystemVersion = _T("99");
		return 0;
	}

	// 대출 공유 버젼 셋팅
	InitShareSystemVersioin();
	if( m_sShareSystemVersion.Compare(_T("99")) == 0 )
	{
		return 0;
	}
	// 도서관 부호 가져오기
	ids = InitLocalLibCode();
	if( ids < 0 )
	{
		m_sShareSystemVersion = _T("99");
		return 0;
	}
	// 전략 객체 생성
	InitUploadObject();

	return 0;

EFS_END
return -1;
}

INT CLoanShareManager::SetThreadStatus( BOOL bRun )
{
EFS_BEGIN

	m_bRunThread = bRun;

	return 0;

EFS_END
return -1;
}

// 20040224 조병걸 수정 : 쓰레드로 돌지 않고 바로바로 템프 테이블에 쌓이도록 수정
INT CLoanShareManager::SendLoanInfoUpload(UPLOAD_SHARE_INFO UploadShareInfo, INT nMode/*=0*/)
{
EFS_BEGIN

	if(m_sShareSystemVersion == _T("99") && 0 == nMode)
	{
		return 1;
	}

	// 대출/반납 공유를 사용하지 않는데..
	if(m_sShareSystemVersion.GetAt(1) == '9' && 0 == nMode) 
	{
		// 반납 연기가 사용되면 return
		if(UploadShareInfo.nType == 1)
		{
			return 1;
		}
		if(UploadShareInfo.nType == 0)
		{
			if(_ttoi(UploadShareInfo.UploadShareLoanInfo.sCommand) != 11) 
			{
				return 1;
			}
		}
	}
	// 복사
	m_UploadList.Add(UploadShareInfo);

	UploadToCenter();

	return 0;

EFS_END
return -1;
}

INT CLoanShareManager::UnLoad()
{
EFS_BEGIN

	if(m_UploadList.GetSize() > 0)
	{
		// 1. 쓰레드가 돌고 있는지 확인.
		// 끝날때까지 기둘린다.
		CLoanShareWaitMessage* pWait;
		pWait = new CLoanShareWaitMessage(this);
		pWait->Create(this);
		pWait->CenterWindow();
		pWait->ShowWindow(SW_SHOW);

		while( m_UploadList.GetSize() != 0 )
		{
			pWait->SetRemainCnt( m_UploadList.GetSize() );
			if( !m_bRunThread )
			{
				UploadToCenter();
			}
			Sleep(5000);
		}
		delete pWait;
		
// 아래꺼 잘 안됨
//		WaitForSingleObject( m_pUploadThread , INFINITE );
	}

	// 2009.06.30 UPDATE BY PWR : 사용안하는 클래스의 포인트를 가져올필요없이 바로 (CShareInfoCenterUp)로 가져오도록 수정
	delete m_pShareInfoCenterUp;
// 	delete m_pLoanUserCenterUp;
// 	delete m_pLoanInfoCenterUp;
// 	delete m_pUClassDelayInfoCenterUp;
// 	delete m_pUCARDelayInfoCenterUp;
// 	delete m_pHolydayInfoCenterUp;
// 	delete m_pCommonDelayEvnCenterUp;
	
	return 0;

EFS_END
return -1;
}

INT CLoanShareManager::UploadToCenter()
{
EFS_BEGIN

	//INT ids;
	//CShareInfoCenterUp* pCenterUpStrategy;

	// 센터에 UPLOAD한다.
	while( m_UploadList.GetSize() != 0 )
	{
		// 1. COMMAND에 따라 처리할 전략 객체를 선택한다.
		UPLOAD_SHARE_INFO UploadInfo = m_UploadList.GetAt(0);

		// 2009.06.30 UPDATE BY PWR : 사용안하는 클래스의 포인트를 가져올필요없이 바로 (CShareInfoCenterUp)로 가져오도록 수정
		if(NULL == m_pShareInfoCenterUp) 
		{
			m_UploadList.RemoveAt(0);
			continue;
		}

		// 2. 처리한다.
		m_pShareInfoCenterUp->SetData(UploadInfo,m_sLibCode);
		m_pShareInfoCenterUp->InsertTempTable(UploadInfo.UploadShareLoanInfo,_T("444"));
		m_UploadList.RemoveAt(0);
		continue;
// 		pCenterUpStrategy = FindCenterUpStrategy(UploadInfo);
// 		if( pCenterUpStrategy == NULL ) 
// 		{
// 			m_UploadList.RemoveAt(0);
// 			WriteLog(UploadInfo,10000);
// 			continue;
// 		}
// 
// 		// 2. 처리한다.
// 		pCenterUpStrategy->SetData(UploadInfo,m_sLibCode);
// 		pCenterUpStrategy->InsertTempTable(UploadInfo.UploadShareLoanInfo,_T("444"));
// 		WriteLog(UploadInfo, 10006);
// 		m_UploadList.RemoveAt(0);
// 		continue;

		// 2009.06.30 DEL BY PWR : 사용안하는 소스정리
// 		ids = pCenterUpStrategy->UploadData();		
// 		if(ids == 10001)
// 		{ // ERROR -> TYPE이 존재하지 않습니다.
// 			WriteLog(UploadInfo,ids);
// 		}
// 		else if(ids == 10002)
// 		{ // ERROR -> Local의 이용자가 검색되지 않습니다.
// 			WriteLog(UploadInfo,ids);
// 		}
// 		else if(ids == 10004)
// 		{ // ERROR -> 주민등록번호가 존재하지 않습니다.
// 			WriteLog(UploadInfo,ids);
// 		}
// 		else if(ids == 10005)
// 		{ // ERROR -> 센터의 이용자가 검색되지 않습니다.
// 			WriteLog(UploadInfo,ids);
// 		}
// 		else if(ids == 10006)
// 		{ // ERROR -> 센터 서버와 연결이 되지 않습니다.
// 			WriteLog(UploadInfo,ids);
// 		}
// 		else if(ids == 10010)
// 		{ // ERROR -> 로컬의 대출/반납 정보를 검색할수 없습니다.
// 			WriteLog(UploadInfo,ids);
// 		}
// 		else if(ids == 10012)
// 		{ // ERROR -> 로컬의 대출/반납 정보에 연결된 센터 이용자가 존재하지 않습니다.
// 			WriteLog(UploadInfo,ids);
// 		}
// 		else if(ids == 10021)
// 		{ // ERROR -> 로컬의 직급별 반납연기 정보가 존재하지 않습니다.
// 			WriteLog(UploadInfo,ids);
// 		}
// 		m_UploadList.RemoveAt(0);
// /*
// 		// 속도 개선
// 		FINISH = GetTickCount();
// 		CString strMsg;
// 		strMsg.Format(_T("(Finish) - [%s] 걸린 시간 :  [%d.%d sec]"), _T("센터업로드걸린 시간") , (FINISH-START)/1000, (FINISH-START)%1000);
// 		CTime t = CTime::GetCurrentTime();
// 		CString filename, msg;
// 		filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), t.GetYear(), t.GetMonth(), t.GetDay());
// 		msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%s][%05d] %s;"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), _T("") , ids, strMsg);
// 		
// 		FILE* fp;
// 		
// 		fp = _tfopen (filename, _T("a+"));
// 		if (fp == NULL)
// 		{
// 			return 0;
// 		}
// 		_ftprintf (fp, _T("%s\n"), msg);
// 		
// 		fclose(fp);
// */		
	}

	return 0;

EFS_END
return -1;
}

INT CLoanShareManager::InitLocalLibCode()
{
EFS_BEGIN

	// 2009.06.30 UPDATE BY PWR : 도서관부호 m_pInfo에서 가져오도록 수정
// 	CString sLIbCode[1];
// 	CString sCond[1] = { _T("LIB_CODE") };
// 
// 	INT ids = CLocCommonAPI::GetLibInfo(sCond, sLIbCode, 1, this);
// 	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitLocalLibCode") );
// 
// 	m_sLibCode = sLIbCode[0];
	m_sLibCode = m_pInfo->LIB_CODE;

	return 0;

EFS_END
return -1;
}

INT CLoanShareManager::InitUploadObject()
{
EFS_BEGIN

	// 2009.06.30 UPDATE BY PWR : 사용안하는 클래스의 포인트를 가져올필요없이 바로 (CShareInfoCenterUp)로 가져오도록 수정
	m_pShareInfoCenterUp = new CShareInfoCenterUp(this);
// 	// 1. 대출자 UP
// 	m_pLoanUserCenterUp = new CLoanUserCenterUp(this);
// 	// 2. 대출/반납 UP
// 	m_pLoanInfoCenterUp = new CLoanInfoCenterUp(this);
// 	// 3. 직급별 반납연기 환경 UP
// 	m_pUClassDelayInfoCenterUp = new CUClassDelayInfoCenterUp(this);
// 	// 4. 직급별 등록구분별 환경 UP
// 	m_pUCARDelayInfoCenterUp = new CUCaRDelayCenterUp(this);
// 	// 5. 휴관일 정보 Up
// 	m_pHolydayInfoCenterUp = new CHolydayInfo(this);
// 	// 6. 일반적인 반납연기 Up
// 	m_pCommonDelayEvnCenterUp = new CCommonDelayEnvCenterUp(this);

	return 0;

EFS_END
return -1;
}

INT CLoanShareManager::InitShareSystemVersioin()
{
EFS_BEGIN

	CESL_DataMgr* pDM = FindDM( _T("DM_LOAN_INFO_SHARE_MANAGER") );
	if(pDM == NULL)
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1000, _T("InitShareSystemVersioin") );
	}

	CString sValue, strUnityLoanService;
	// 대출자 공유
	GetManageValue(_T("기타"), _T("공통"), _T("통합도서서비스"), _T("통합도서서비스"), strUnityLoanService);
	if(strUnityLoanService.IsEmpty())
	{
		strUnityLoanService = _T("N");
	}
	
// 	if(_T("Y") != strUnityLoanService)
// 	{
// 		m_sShareSystemVersion = _T("99");
// 		return 0;
// 	}

	GetManageValue(_T("기타"), _T("공통"), _T("대출공유"), _T("대출자공유버젼"), sValue);

	if(sValue == _T("1"))
	{
		m_sShareSystemVersion = _T("0");
	}
	else if(sValue == _T("2"))
	{
		m_sShareSystemVersion = _T("1");
	}
	else
	{
		m_sShareSystemVersion = _T("99");
		return 0;
	}

	// 대출정보 공유
	GetManageValue(_T("기타"), _T("공통"), _T("대출공유"), _T("대출반납공유버젼"), sValue);
	
	if(sValue == _T("1"))
	{
		m_sShareSystemVersion += _T("0");
	}
	else if(sValue == _T("2"))
	{
		m_sShareSystemVersion += _T("1");
	}
	else
	{
		m_sShareSystemVersion += _T("9");
		return 0;
	}

	return 0;
	
EFS_END
return -1;
}

LRESULT CLoanShareManager::OnStartCheck(WPARAM wParam, LPARAM lParam)
{
	SetTimer( UPLOAD_CHECK_TIMER_ID  , UPLOAD_CHECK_TIME , NULL );	
	return 0;
}

BOOL CLoanShareManager::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	// 20040224 조병걸 수정 : 쓰레드 중지
// 	if( m_sShareSystemVersion.Compare(_T("99")) != 0 )
// 		PostMessage( WM_START_CHECK );
	
	return TRUE;

EFS_END
return FALSE;
}

BOOL CLoanShareManager::Create(CWnd* pParentWnd) 
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);
	
EFS_END
return FALSE;
}

HBRUSH CLoanShareManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

// 2009.06.30 DEL BY PWR : 사용안하는 소스정리
// CShareInfoCenterUp* CLoanShareManager::FindCenterUpStrategy(UPLOAD_SHARE_INFO UploadInfo)
// {
// EFS_BEGIN
// 
// 	if( UploadInfo.nType == 0 )
// 	{
// 		INT nCommand = _ttoi(UploadInfo.UploadShareLoanInfo.sCommand);
// /*
// 		CString filename,msg;
// 
// 		CTime t = CTime::GetCurrentTime();
// 		/// 로그 찍기
// 		filename.Format(_T("..\\tunning\\ESL_DM_%04d%02d%02d.txt"), t.GetYear(), t.GetMonth(), t.GetDay());
// 		msg.Format(_T("[%04d-%02d-%02d.%02d:%02d:%02d][%s][Command:%d];"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), _T("FindCenterUpStrategy") , nCommand );
// 		
// 		FILE* fp;
// 		
// 		fp = _tfopen (filename, _T("a+"));
// 		if (fp == NULL)
// 		{
// 			return 0;
// 		}
// 		_ftprintf (fp, _T("%s\n"), msg);
// */
// 		if( nCommand == 11 ) 
// 		{ // 일반 이용자 업로드
// 			return m_pLoanUserCenterUp;
// 		}
// 		if( nCommand == 13 ) 
// 		{
// 			return m_pLoanUserCenterUp;
// 		}
// 		else if( nCommand == 51 ) 
// 		{ // 대출/반납 정보 업로드
// 			return m_pLoanInfoCenterUp;
// 		}
// 		else if( nCommand == 61 ) 
// 		{ // 직급별반납연기환경 업로드
// 			return m_pUClassDelayInfoCenterUp;
// 		}
// 		else if( nCommand == 62 ) 
// 		{ // 직급 등록구분별 반납연기 환경 업로드
// 			return m_pUCARDelayInfoCenterUp;
// 		}
// 		else if( nCommand == 63 ) 
// 		{ // 휴관일 환경 업로드
// 			return m_pHolydayInfoCenterUp;
// 		}
// 		else if( nCommand == 64 ) 
// 		{ // 일반적인 반납연기 환경 업로드
// 			return m_pCommonDelayEvnCenterUp;
// 		}
// 		else
// 		{
// 			return NULL;
// 		}
// 	}
// 	else if( UploadInfo.nType == 1 )
// 	{
// 		// 반납연기
// 		;
// 	}
// 	else
// 	{
// 		return NULL;
// 	}
// 	
// 	return NULL;
// 
// EFS_END
// return NULL;
// }

// 2009.06.30 DEL BY PWR : 사용안하는 소스정리
// INT CLoanShareManager::WriteLog(UPLOAD_SHARE_INFO UploadInfo,INT nErrorCode)
// {
// EFS_BEGIN
// 
// /*
// 	/// 폴더 생성
// 	_tmkdir( __FAIL_LOG_FILE_PATH__ );
// 
// 	CString sFileName = __FAIL_LOG_FILE_PATH___T("\\")__FAIL_FILE_NAME__ ;
// 
// 	/// 파일 생성
// 	CStdioFile fFailLogFile;
// 	fFailLogFile.Open( sFileName , CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate );
// 
// 	fFailLogFile.SeekToEnd();
// 
// 	CString sText;
// 	sText.Format(
// 				_T("-------------------------\r\n")
// 				_T("-------------------------\r\n")
// 				_T("[%s](ERROR_CODE:%d)")
// 				_T("nType : %d  \r\n")
// 				_T("-------------------------\r\n")
// 				_T("> UPLOAD_SHARE_LOAN_INFO \r\n")
// 				_T("sType : %s , \r\n")
// 				_T("sCommand : %s , \r\n")
// 				_T("sMasterUserInfoChangeYN : %s , \r\n")
// 				_T("sUpdatedTableName : %s , \r\n")
// 				_T("sUpdatedTableRecKey : %s , \r\n")
// 				_T("sUpdatedFields : %s  \r\n")
// 				_T("> UPLOAD_SHARE_DELAY_INFO \r\n")
// 				_T("sUpdateLibCode : %s , \r\n")
// 				_T("sUpdatedTableRecKey : %s , \r\n")
// 				_T("sUpdatedSetQuery : %s \r\n")
// 				, 
// 				CTime::GetCurrentTime().Format(_T("%Y:%m:%d%H%M%S")) , 
// 				nErrorCode , 
// 				UploadInfo.nType ,
// 				UploadInfo.UploadShareLoanInfo.sType , 
// 				UploadInfo.UploadShareLoanInfo.sCommand , 
// 				UploadInfo.UploadShareLoanInfo.sMasterUserInfoChangeYN , 
// 				UploadInfo.UploadShareLoanInfo.sUpdatedTableName  , 
// 				UploadInfo.UploadShareLoanInfo.sUpdatedTableRecKey  , 
// 				UploadInfo.UploadShareLoanInfo.sUpdatedFields  , 
// 				UploadInfo.UploadShareDelayInfo.sUpdateLibCode , 
// 				UploadInfo.UploadShareDelayInfo.sUpdatedTableRecKey ,
// 				UploadInfo.UploadShareDelayInfo.sUpdatedSetQuery
// 				);
// 
// 	fFailLogFile.WriteString(sText);
// 
// 	fFailLogFile.Close();
// */
// 	return 0;
// 
// EFS_END
// return -1;
// }
