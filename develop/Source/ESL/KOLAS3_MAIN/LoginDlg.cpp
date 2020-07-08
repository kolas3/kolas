// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "LoginDlg.h"
#include "efs.h"
#include "KOLAS3_MAINview.h"

//2004-05-08 이학중 추가 
#include "..\\MENU_TOOLBAR_MGR\\MenuToolbar_Mgr.h"

#include "Imm.h"
#include "..\\ESL\\FWDES.h"

// 2012.10.06 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회(사용자 비밀번호 암호화)
#include "..\\..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\\LocCommonAPI.h"


// 2013.10.08 ADD BY BJS : 2013도서관통합서비스확대구축및개선 (일배치 로그삭제)
#include "..\\..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\\CfgFileApi.h"

// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : GPKI 인증서 로그인
#include "..\\..\\..\\공통\\GPKI\\GPKILoginDlg.h"
#include "NetworkInfoMgr.h"
// >> 2015/11/11 --ADD

// KOL.RED.2018.228 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "ESL_SearchEngine.h"
// KOL.RED.2018.228 ---------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <Winsock2.h>

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_strID = _T("");
	m_strPassword = _T("");
	m_bUserLoginMode = FALSE;
	//}}AFX_DATA_INIT

	m_pParentWnd = pParent;
	m_nFailcount = 0;
	m_pInfo = NULL;

	//2015년 공공도서관 시스템 개선 2015/11/23 _LCM : GPKI 인증시 로그인 정보
	m_bGPKI = FALSE;
	
	#ifdef _ECOLAS_S
		m_bmpLogin.LoadBMP(_T("..\\bmp\\MainImage\\ECOLAS-S\\Main_LOGIN.bmp"));
	#endif
	#ifdef _ECOLAS
		m_bmpLogin.LoadBMP(_T("..\\bmp\\MainImage\\ECOLAS\\LOGIN.bmp"));
	#endif
	#ifdef _KOLASIII
		m_bmpLogin.LoadBMP(_T("..\\bmp\\Main_LOGIN.bmp"));
	#endif
	#ifdef _KOLASIII_TL
		m_bmpLogin.LoadBMP(_T("..\\bmp\\Main_LOGIN.bmp"));
	#endif
	#ifdef _LIGHT
		m_bmpLogin.LoadBMP(_T("..\\bmp\\Main_LOGIN.bmp"));
	#endif
	
	m_bIsThreadRun = FALSE;
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));
}

CLoginDlg::~CLoginDlg()
{		
	CloseHandle(m_hThread);
}

VOID CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX, IDC_PROGRESS_MAIN1, m_ProgressCtrl);
	DDX_Text(pDX, IDC_eID, m_strID);
	DDV_MaxChars(pDX, m_strID, 10);
	DDX_Text(pDX, IDC_ePASSWORD, m_strPassword);

	// 2012.10.05 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회
	// 비밀번호 글자 수 설정
	DDV_MaxChars(pDX, m_strPassword, 20);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_EN_SETFOCUS(IDC_eID, OnSetfocuseID)
	// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : GPKI 인증서 로그인
	ON_BN_CLICKED(IDC_BUTTON_GPKI_LOGIN, OnButtonGpkiLogin)
	// >> 2015/11/11 --ADD
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLOSE_DIALOG, OnCloseDialog)
	ON_MESSAGE(WM_USER + 1004  , BeginLogin )	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg Threads
UINT ThreadFunc_LoginProcess(LPVOID pParam)
{
	CLoginDlg *pDlg = (CLoginDlg*)pParam;
	if (pDlg == NULL) return -1;
	
	pDlg->m_bIsThreadRun = TRUE;

	INT nLower = 0;
	INT nUpper = 9;
	INT nLog = 0;

	CString strText;

	CTextProgressCtrl *pProgressCtrl = &pDlg->m_ProgressCtrl;

	// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : IP인증
	CESL_DataMgr ipDM;
	CString strIP = _T(""), strQueryIP = _T(""), strUserID = _T("");
	BOOL bVerify = FALSE;
	// >> 2015/11/11 --ADD

	// 17/06/15 김혜영 : 로그인 실패시 로그인 제약기능 추가
//*/ ADD ---------------------------------------------------------------------------	
	INT ids = 0;
	INT nCfgFailCnt = 5;
	INT nLimitTime = 0;	
	CESL_DataMgr TmpDM;
	CString strQuery = _T("");
	CString strTemp = _T("");
	CString strLoginIP = pDlg->GetIP();
	BOOL bLoginRestriction = FALSE;

	TmpDM.SetCONNECTION_INFO(_T(""));			
	strQuery = _T("SELECT USE_YN FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE='LOGINRESTRICTION'");
	TmpDM.GetOneValueQuery(strQuery, strTemp);
	if(!strTemp.IsEmpty() && strTemp.Compare(_T("Y")) == 0)
	{
		bLoginRestriction = TRUE;
		strQuery = _T("SELECT UTIL_VALUE1, UTIL_VALUE2 FROM CO_UTIL_LOCK_TBL WHERE UTIL_CODE='LOGINRESTRICTION'");
		ids = TmpDM.RestructDataManager(strQuery);
		if(ids >= 0)
		{
			nCfgFailCnt = _ttoi(TmpDM.GetCellData(0, 0));
			nLimitTime = _ttoi(TmpDM.GetCellData(0, 1));
		}
	}
//*/ END ---------------------------------------------------------------------------

	pProgressCtrl->SetRange(nLower, nUpper);
	
	// 1. 로그인한다.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("데이터베이스에 접속하는중...");
	pProgressCtrl->SetWindowText(strText);
	nLog = pDlg->Login();
	if ( nLog < 0)
	{
		strText = _T("아이디, 패스워드 확인중...");
		pProgressCtrl->SetWindowText(strText);
		pDlg->m_bIsThreadRun = FALSE;

		// << 2015년 공공도서관 시스템 개선 2015/11/26 _LCM : GPKI로그인적용
		if(nLog != -100)
		{
			// 17/06/15 김혜영 : 로그인 실패시 로그인 제약기능 추가
/*//BEFORE-----------------------------------------------------------------------------
			// 2012.10.26 UPDATE BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 4회
			// 5번실패하면 프로그램을 종료한다.
			if (++pDlg->m_nFailcount == 5) goto ERR;
	// 		// 3번실패하면 프로그램을 종료한다.
	// 		if (++pDlg->m_nFailcount == 3) goto ERR;
					
			AfxMessageBox(_T("아이디 또는 암호가 잘못되었습니다. 아이디와 암호를 확인하십시오."));

			// 2012.10.26 UPDATE BY PWR : 2012년도 도서관 통합서비스 확대구축 사업 4회
			strText.Format(_T("ID, PASSWORD를 입력하세요[재시도:%d/5]"), pDlg->m_nFailcount);
	// 		strText.Format(_T("ID, PASSWORD를 입력하세요[재시도:%d/3]"), pDlg->m_nFailcount);
*///AFTER------------------------------------------------------------------------------						
			INT nFailCnt = ++pDlg->m_nFailcount;					
			if(bLoginRestriction != FALSE)
			{	
				strQuery.Format(_T("SELECT LOGIN_FAIL_COUNT FROM CO_LOGIN_RESTRICTION_TBL WHERE LOGIN_IP='%s' AND ROWNUM <= 1"), strLoginIP);
				ids = TmpDM.GetOneValueQuery(strQuery, strTemp);
				if(!strTemp.IsEmpty())
				{
					nFailCnt = _ttoi(strTemp) + 1;
					
					strQuery.Format(_T("SELECT TO_CHAR(LOGIN_RESTRICTION_DATE,'YYYY/MM/DD HH24:MI:SS'), ")
			        _T("DECODE(LOGIN_RESTRICTION_DATE, NULL , '1', SYSDATE-LOGIN_RESTRICTION_DATE) ")
					_T("FROM CO_LOGIN_RESTRICTION_TBL WHERE LOGIN_IP='%s'")
					,strLoginIP);
					ids = TmpDM.RestructDataManager(strQuery);	
					if(ids >= 0)
					{
						CString strLockTime = TmpDM.GetCellData(0, 0);
						CString strLockValue = TmpDM.GetCellData(0, 1);
						if ( strLockValue.Mid(0,1) == '-' ) 
						{
							strTemp.Format(_T("로그인 실패제한 %d회 이상으로 [ %s ]까지 사용차단되었습니다.\n 이후에 로그인 가능합니다.")
										  , nCfgFailCnt, strLockTime);
							AfxMessageBox(strTemp);
							goto ERR;
						}						
						else if(_ttoi(strTemp) >= nCfgFailCnt)
						{
							nFailCnt = 1;
						}
					}

					strQuery.Format(_T("UPDATE CO_LOGIN_RESTRICTION_TBL SET LOGIN_FAIL_DATE=SYSDATE, LOGIN_ID='%s', "
							        _T("LOGIN_FAIL_COUNT=%d WHERE LOGIN_IP='%s';")), pDlg->m_strID, nFailCnt, strLoginIP);
				}
				else
				{
					strQuery.Format(_T("INSERT INTO CO_LOGIN_RESTRICTION_TBL "
							        _T("(REC_KEY, LOGIN_IP, LOGIN_ID, LOGIN_FAIL_DATE, LOGIN_FAIL_COUNT) ")
									_T("VALUES (ESL_SEQ.NEXTVAL, '%s', '%s', SYSDATE, %d);"))
									, strLoginIP, pDlg->m_strID, nFailCnt);
				}
				TmpDM.StartFrame();	
				TmpDM.AddFrame(strQuery);		
				ids = TmpDM.SendFrame();
				TmpDM.EndFrame();

				if (nFailCnt >= nCfgFailCnt)
				{
					TmpDM.StartFrame();	
					// 제한시간동안 사용불가능하게 한다.
					strQuery.Format(_T("UPDATE CO_LOGIN_RESTRICTION_TBL SET LOGIN_RESTRICTION_DATE=SYSDATE+%d/1440 WHERE LOGIN_IP='%s';")
							        , nLimitTime, strLoginIP);
					TmpDM.AddFrame(strQuery);		
					ids = TmpDM.SendFrame();
					TmpDM.EndFrame();
					if(ids >= 0)
					{
						strText.Format(_T("로그인 실패제한 %d회 이상으로 사용이 차단되었습니다.\n %d분 후에 로그인 가능합니다.")
					      , nCfgFailCnt, nLimitTime);
					}
					else
					{
						strText = _T("사용차단정보기록을 실패했습니다.");
					}
					AfxMessageBox(strText);
					goto ERR;
				}							
				
			}
			else
			{
				if (nFailCnt == nCfgFailCnt) goto ERR;				
			}
			AfxMessageBox(_T("아이디 또는 암호가 잘못되었습니다. 아이디와 암호를 확인하십시오."));			
			strText.Format(_T("ID, PASSWORD를 입력하세요[재시도:%d/%d]"), nFailCnt, nCfgFailCnt);
/*//END-------------------------------------------------------------------------------*/
		}
		else
		{
			strText = _T("재시도 바랍니다.");
		}
		// >> 2015/11/26 --MOD

		pProgressCtrl->SetWindowText(strText);

		((CEdit*)pDlg->GetDlgItem(IDC_eID))->SetReadOnly(FALSE);
		((CEdit*)pDlg->GetDlgItem(IDC_eID))->SetFocus();
		((CEdit*)pDlg->GetDlgItem(IDC_ePASSWORD))->SetReadOnly(FALSE);
		pDlg->m_bIsThreadRun = FALSE;
		return -1;
	}
	else if (nLog > 0)
	{
		// 2009.03.19 ADD BY PDJ : 로그인 불가상태 처리
		pDlg->m_bIsThreadRun = FALSE;
		goto ERR;
		return -1;
	}

	// << 2015년 공공도서관 시스템 개선 15/10/14  _LCM : IP 인증	
	ipDM.SetCONNECTION_INFO(pDlg->m_pInfo->CONNECTION_INFO);	
	//((CEdit*)pDlg->GetDlgItem(IDC_eID))->GetWindowText(strUserID);
	strUserID = pDlg->m_strID;
	strQueryIP.Format(_T("SELECT SECU_LOGIN_IP FROM MN_USER_TBL WHERE ID = '%s' AND PERMITION = 'U'"), strUserID);
	ipDM.GetOneValueQuery(strQueryIP, strIP);
		
	if(strIP.Compare(_T("")) == 0)
	{
		bVerify = TRUE;
	}
	else if(strIP.Compare(_T("")) != 0)
	{
		CString strTok[3];

		int i = 0;

		for(i=0; i<3; i++)
		{
			AfxExtractSubString(strTok[i], strIP, i, ';');		

			if(pDlg->IsMatchIP(strTok[i]))
			{
				bVerify = TRUE;
				break;
			}			
		}
	}

	if(bVerify == FALSE)
	{
		AfxMessageBox(_T("등록된 IP가 아닙니다.\n\nIP등록 후 사용바랍니다."),MB_OK);
		goto ERR;
	}
	// >> 2015/10/14 --ADD
	
	// 17/06/15 김혜영 : 로그인 실패시 로그인 제약기능 추가
//*/ ADD ---------------------------------------------------------------------------
	if(bLoginRestriction != FALSE)
	{
		CString strLockTime = _T("");
		CString strLockValue = _T("");
		strQuery.Format(_T("SELECT TO_CHAR(LOGIN_RESTRICTION_DATE,'YYYY/MM/DD HH24:MI:SS'), ")
			            _T("DECODE(LOGIN_RESTRICTION_DATE, NULL , '1', SYSDATE-LOGIN_RESTRICTION_DATE) ")
						_T("FROM CO_LOGIN_RESTRICTION_TBL WHERE LOGIN_IP='%s'")
						,strLoginIP);
		ids = TmpDM.RestructDataManager(strQuery);	
		if(ids >= 0)
		{
			strLockTime = TmpDM.GetCellData(0, 0);
			strLockValue = TmpDM.GetCellData(0, 1);
			if ( strLockValue.Mid(0,1) == '-' ) 
			{				
				strTemp.Format(_T("로그인 실패제한 %d회 이상으로 [ %s ]까지 사용차단되었습니다.\n 이후에 로그인 가능합니다.")
					          , nCfgFailCnt, strLockTime);
				AfxMessageBox(strTemp);
				goto ERR;
			}
			strQuery.Format(_T("DELETE CO_LOGIN_RESTRICTION_TBL WHERE LOGIN_IP='%s';"), strLoginIP);
			TmpDM.StartFrame();	
			TmpDM.AddFrame(strQuery);		
			ids = TmpDM.SendFrame();
			TmpDM.EndFrame();
		}
	}
//*/ END ---------------------------------------------------------------------------

	pProgressCtrl->StepIt();

	//2003-1106 CESL_Information 변경 작업-박길주========================================
	//2003-1106 CESL_Information 변경 작업-박길주++++++++++++++++++++++++++++++++++++++++	
	// 1.5 코드 정보 로드한다.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("시스템정보 다운로드중...");
	pProgressCtrl->SetWindowText(strText);
	if (pDlg->DownloadSystemInfoData() < 0)
		goto ERR;
	
	pProgressCtrl->StepIt();
	//2003-1106 CESL_Information 변경 작업-박길주----------------------------------------	

	// 2. 코드 정보 로드한다.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("부호 데이터 다운로드중...");
	pProgressCtrl->SetWindowText(strText);
	pDlg->LoadCodeInfo();
	pDlg->m_ProgressCtrl.StepIt();

	// 3. Marc Intellisens 용 Reference를 로드한다.
	// 
	//if (!pDlg->m_bIsThreadRun) goto CANCEL;
	//strText = _T("마크 데이터 다운로드중...");
	//pProgressCtrl->SetWindowText(strText);	
	//if (pDlg->DownloadMarcRelation() < 0) 
	//goto ERR;
	
	pDlg->m_pInfo->InitMarcMgr();
	pProgressCtrl->StepIt();	

	// 4. 문자셋을 받는다.
#ifndef _KOLASIII_TL
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("문자 데이터 다운로드중...");
	pProgressCtrl->SetWindowText(strText);
	if (pDlg->DownloadCharSetData() < 0)
		goto ERR;
#endif
	pProgressCtrl->StepIt();
#ifndef _KOLASIII_TL
	// 5. 관리 데이터를 받는다.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("관리 데이터 다운로드중...");
	pProgressCtrl->SetWindowText(strText);
	if (pDlg->DownloadManagementData() < 0)
		goto ERR;
#endif
	pProgressCtrl->StepIt();
#ifndef _KOLASIII_TL
	// 6. ESL REFERENCE를 받는다.
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("프로그램 데이터 다운로드중...");
	pProgressCtrl->SetWindowText(strText);
	//if (pDlg->DownloadESLReference() < 0)
	//	goto ERR;

	// 15/02/26 박대우 : 공동목록 소장정보 수정시 발생하는 오류 관련 처리 
	//                   공동목록 M/W 연계작업 중에 발생한 수정작업으로 기존 수정내역과 소스 병합작업이
	//                   필요하여 함수를 호출하도록 수정하였다
//// ADD -------------------------------------------------------------------------------------
	pDlg->DownloadESLReference();
//// END -------------------------------------------------------------------------------------
#endif
	pProgressCtrl->StepIt();

	// 7. DVM Loding
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	if (pDlg->DownloadDVM() < 0)
		goto ERR;
	pProgressCtrl->StepIt();
	
	// 8.메뉴, 툴바그룹, 툴바 정보를 다운받는다.
	//2004-05-06 이학주 수정 
	/*
	static_cast<CKOLAS3_MAINView*>(pDlg->m_pParentWnd)->LoadMenuRef();
	static_cast<CKOLAS3_MAINView*>(pDlg->m_pParentWnd)->LoadToolbarRef();
	static_cast<CKOLAS3_MAINView*>(pDlg->m_pParentWnd)->LoadToolGroupRef();
    */

	// 9. 로그 검사 및 삭제
	//2013-10-08 방재선 추가
	if (!pDlg->m_bIsThreadRun) goto CANCEL;
	strText = _T("로그 검사중...");
	pProgressCtrl->SetWindowText(strText);
	pDlg->ControlLog();
	pDlg->m_ProgressCtrl.StepIt();


	if (pDlg->DownLoadMenuToolbar() < 0)
	goto ERR;

	strText = _T("로그인 성공");
	pProgressCtrl->SetWindowText(strText);
	pProgressCtrl->SetPos(nUpper);

	pDlg->PostMessage(WM_CLOSE_DIALOG, IDOK, 0);

	pDlg->m_bIsThreadRun = FALSE;
	SetEvent(pDlg->m_hThread);
			
	return 0;

CANCEL:
	pDlg->m_bIsThreadRun = FALSE;
	SetEvent(pDlg->m_hThread);
	return -1;

ERR:
	if ( nLog < 0 ) AfxMessageBox(_T("로그인 실패"));
	
	pDlg->PostMessage(WM_CLOSE_DIALOG, IDCANCEL, 0);

	pDlg->m_bIsThreadRun = FALSE;
	SetEvent(pDlg->m_hThread);
	
	return -1;
}

// << 2015년 공공도서관 시스템 개선 2015/11/30 _LCM : IP 매칭
#define DESIRED_WINSOCK_VERSION        0x0202
#define MINIMUM_WINSOCK_VERSION        0x0001

BOOL CLoginDlg::IsMatchIP(CString strIP)
{
	TCHAR parm_buffer[16][24];
	int parm_max_count = 16;

	int count = 0;
    WSADATA wsadata; 	

    // WS2_32.DLL을 사용할수 있도록 초기화한다.
    if(!WSAStartup(DESIRED_WINSOCK_VERSION, &wsadata)){
        // 현재 컴퓨터에 설치된 소켓이 이용가능한 버전인지 체크한다.
        if(wsadata.wVersion >= MINIMUM_WINSOCK_VERSION){
            IN_ADDR in_address;
            TCHAR host_name[255] = {0, };

            // 현재 컴퓨터의 호스트 명칭을 얻는다.
            gethostname((LPSTR)(LPCTSTR)host_name, 255);

            // 호스트 데이터베이스에서 지정한 호스트 명칭에 대응하는 호스트 정보를 얻는다.
            HOSTENT *p_host_info = gethostbyname((LPSTR)(LPCTSTR)host_name);
            if(p_host_info != NULL){
                for(int i = 0; p_host_info->h_addr_list[i]; i++){
                    if(parm_max_count > i){
                        memcpy(&in_address, p_host_info->h_addr_list[i], 4);
						TCHAR arrBuff[24] = {0,};
						mbstowcs(arrBuff, inet_ntoa(in_address), sizeof(TCHAR)*24);
                        wcscpy(parm_buffer[i], arrBuff);

						if(wcscmp(parm_buffer[i], strIP.GetBuffer(0)) == 0)
						{
							WSACleanup();
							return TRUE;
						}

                    } else break;
                }
                count = i;
            }
        }

        // WS2_32.DLL의 사용을 중단한다.
        WSACleanup();
    }
	return FALSE;
}
// >> 2015/11/30 --ADD

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg Operations
CString CLoginDlg::GetIP()
{
	CString strResult;
	TCHAR szHostName[255];
	HOSTENT *pHostent;
	
	if (gethostname((LPSTR)(LPCTSTR)szHostName, 255) == 0)
	{
		pHostent = gethostbyname((LPSTR)(LPCTSTR)szHostName);
		if (pHostent)
		{
			strResult.Format(_T("%d.%d.%d.%d"), 
							(BYTE)pHostent->h_addr_list[0][0], 
							(BYTE)pHostent->h_addr_list[0][1], 
							(BYTE)pHostent->h_addr_list[0][2], 
							(BYTE)pHostent->h_addr_list[0][3]);
		}		
	}
	
	return strResult;
}

// 16.09.29 JMJ KOL.UDF.020
CString CLoginDlg::GetMacAddress()
{
	/*
	CString strGateWay = _T("");
	CString strMacAddress = _T("");

	IP_ADAPTER_INFO ipAdapterInfo[5];
	DWORD dwBuflen = sizeof(ipAdapterInfo);


	DWORD dwStatus = GetAdaptersInfo(ipAdapterInfo, &dwBuflen);

	PIP_ADAPTER_INFO pipAdapterInfo = ipAdapterInfo;

	do 
	{
		strGateWay = (CString)pipAdapterInfo->GatewayList.IpAddress.String;
		if(!strGateWay.IsEmpty() && strGateWay[0] == '0'){
			pipAdapterInfo = pipAdapterInfo->Next;
		}else{
			strMacAddress.Format(_T("%02X%02X%02X%02X%02X%02X"),
				pipAdapterInfo->Address[0],
				pipAdapterInfo->Address[1],
				pipAdapterInfo->Address[2],
				pipAdapterInfo->Address[3],
				pipAdapterInfo->Address[4],
				pipAdapterInfo->Address[5]
			);
			break;
		}
	} while (pipAdapterInfo);

	return strMacAddress;
	*/
	// 18.11.12 JMJ KOL.RED.2018.006
	/*
		MAC ADDRESS 가져오는 것 기준
		iphlpapi.h (ipexport.h, iptypes.h) 제거
    */
	NCB Ncb;
    UCHAR uRetCode;
    LANA_ENUM lenum;
    int i;
    CString strOutput =_T("");
	CString strIP = _T("*");
    CString string;
    ADAPTER_STATUS Adapter;
	
    memset( &Ncb, 0, sizeof(Ncb) );
    Ncb.ncb_command = NCBENUM;
    Ncb.ncb_buffer = (UCHAR *)&lenum;
    Ncb.ncb_length = sizeof(lenum);
    uRetCode = Netbios( &Ncb );
	
    for(i=0; i < lenum.length ;i++)
    {
        memset( &Ncb, 0, sizeof(Ncb) );
        Ncb.ncb_command = NCBRESET;
        Ncb.ncb_lana_num = lenum.lana[i];
		
        uRetCode = Netbios( &Ncb );
		
		
        memset( &Ncb, 0, sizeof (Ncb) );
        Ncb.ncb_command = NCBASTAT;
        Ncb.ncb_lana_num = lenum.lana[i];
		
        strcpy( (char*)Ncb.ncb_callname, (LPSTR)(LPCTSTR) strIP ); 
        Ncb.ncb_buffer = (unsigned char *) &Adapter;
        Ncb.ncb_length = sizeof(Adapter);
		
        uRetCode = Netbios( &Ncb );
        if ( uRetCode == 0 )
        {
            string.Format(_T("%02X%02X%02X%02X%02X%02X"),
				Adapter.adapter_address[0],
				Adapter.adapter_address[1],
				Adapter.adapter_address[2],
				Adapter.adapter_address[3],
				Adapter.adapter_address[4],
				Adapter.adapter_address[5] );
            strOutput += string ;
			break;
        }
    }
    return strOutput;
}
INT CLoginDlg::Login()
{
	INT ids = -1;

	CString strQuery = _T("");
	
	m_strID.TrimLeft();
	m_strID.TrimRight();

	m_strPassword.TrimLeft();
	m_strPassword.TrimRight();

	m_strID.Replace(_T(" "), _T(""));
	m_strPassword.Replace(_T(" "), _T(""));

	m_strID.Replace(_T("'"), _T(""));
	m_strPassword.Replace(_T("'"), _T(""));		

	// 14/07/02 박대우 : 아이핀추가로 인해 FLAG설정 방법 변경
/*// BEFORE ---------------------------------------------------------------------------------
	//                          0        1         2          3		
	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, m_strPassword);

	
	// 2012.10.06 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회(사용자 비밀번호 암호화)
	if(m_pInfo->m_bUserPwEncryption)
	{
		CString strCryptoPassword;
		strCryptoPassword = CLocCommonAPI::StringToSHA256(m_strPassword);

		strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, strCryptoPassword);
	}
*/// AFTER -----------------------------------------------------------------------------------
	CString strPW = m_strPassword;
	// 2015년 공공도서관 시스템 개선 2015/11/23 _LCM : GPKI 인증시 로그인 정보 수정
	if (m_pInfo->m_bUserPwEncryption && !m_bGPKI)
	{
		strPW = CLocCommonAPI::StringToSHA256(m_strPassword);
	}
	// KOL.RED.2018.218 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	/*
	//                          0        1         2          3						4									5
	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE, TO_CHAR(EXPIRY_DATE, 'YYYY/MM/DD'), TO_DATE(TO_CHAR(EXPIRY_DATE, 'YYYY/MM/DD'))-TO_DATE(TO_CHAR(SYSDATE, 'YYYY/MM/DD')) , GPKI_USE, GPKI_KEY_VALUE "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, strPW);
	*/
	//                          0        1         2          3						4									5																			6		7				8
	strQuery.Format(_T("SELECT NAME, PERMITION, REC_KEY, MANAGE_CODE, TO_CHAR(EXPIRY_DATE, 'YYYY/MM/DD'), TO_DATE(TO_CHAR(EXPIRY_DATE, 'YYYY/MM/DD'))-TO_DATE(TO_CHAR(SYSDATE, 'YYYY/MM/DD')) , GPKI_USE, GPKI_KEY_VALUE, NVL(USERGRID_EXPORT_YN,'Y') "
						 _T("FROM MN_USER_TBL ")
						_T("WHERE ID='%s' AND PASSWD='%s'")), m_strID, strPW);	
	// KOL.RED.2018.218 ---------------------------------------------------------------------------	
	
//*/ END -------------------------------------------------------------------------------------
	ids = m_DM.RestructDataManager(strQuery);
	if (ids < 0) return ids;
	if (m_DM.GetRowCount() < 1) return -1;	// 로그인 실패

// << 2015년 공공도서관 시스템 개선 2015/11/23 _LCM : GPKI 인증시 로그인 정보 수정

	if( m_DM.GetCellData(0, 6).CompareNoCase(_T("Y")) == 0 && !m_bGPKI)
	{
		
		AfxMessageBox(_T("GPKI 로그인 사용자입니다.\r\n")
						_T("아이디 패스워드로 로그인 불가합니다.\r\n")
						_T("재로그인 바랍니다."));			
	
		return -100;
	}
	else if ( m_DM.GetCellData(0, 6).CompareNoCase(_T("Y")) != 0 && m_bGPKI )
	{
		AfxMessageBox(_T("GPKI 로그인 설정바랍니다.\r\n")
							_T("GPKI로 로그인 불가합니다.\r\n")
							_T("재로그인 바랍니다."));			
		return -100;
	}
	else if(m_DM.GetCellData(0, 6).CompareNoCase(_T("Y")) == 0 && m_bGPKI && m_DM.GetCellData(0, 7).CompareNoCase(_T("")) == 0)
	{
		AfxMessageBox(_T("GPKI 로그인 설정바랍니다.\r\n")
						_T("GPKI가 등록되지 않았습니다.\r\n")
						_T("관리자에게 문의바랍니다."));			
		return 100;
	}
		

// >> 2015/11/23 --ADD
	

	// 14/07/02 박대우 : 사용자 유효기간 기능 추가
//*/ ADD -------------------------------------------------------------------------------------
	if (m_DM.GetCellData(0, 1).CompareNoCase(_T("U")) == 0) // 일반사용자인 경우 확인 필요
	{
		CString strExpDate = m_DM.GetCellData(0, 4);
		if (!strExpDate.IsEmpty())
		{
			int nExpTerm = _ttoi(m_DM.GetCellData(0, 5));
			if (nExpTerm < 0)
			{
				AfxMessageBox(_T("사용자의 유효기간이 만기되어 로그인을 할 수 없는 상태입니다.\r\n")
							  _T("관리자에게 문의하십시오."), MB_ICONSTOP);	
				return 100;
			}
			// 사용기간이 얼마남지 않았다면 메시지로 알려준다
			if (nExpTerm < 10)
			{
				CString strMsg;
				strMsg.Format(_T("로그인 사용자의 유효기간이 [%d]일 남았습니다.\r\n")
							  _T("유효기간이 만기될 경우에는 로그인이 불가능하오니\r\n")
					          _T("관리자에게 문의하시기 바랍니다."),nExpTerm+1);
				AfxMessageBox(strMsg, MB_ICONINFORMATION);	
			}
		}
	}
//*/ END -------------------------------------------------------------------------------------

	// m_pInfo 세팅한다.
	m_pInfo->USER_ID		= m_strID;
	m_pInfo->USER_NAME		= m_DM.GetCellData(0, 0); 
	m_pInfo->USER_PRIVILEGE = m_DM.GetCellData(0, 1); 
	m_pInfo->USER_PK		= m_DM.GetCellData(0, 2);
	m_pInfo->LOCAL_IP		= GetIP();
	// 16.09.29 JMJ KOL.UDF.020
	m_pInfo->MAC_ADDRESS	= GetMacAddress();
	m_pInfo->MANAGE_CODE	= m_DM.GetCellData(0, 3);
	m_pInfo->MANAGE_CODE.TrimLeft();		
	m_pInfo->MANAGE_CODE.TrimRight();
	m_DM.MANAGE_CODE = m_pInfo->MANAGE_CODE;

	// KOL.RED.2018.218 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	if (m_pInfo->USER_PRIVILEGE.CompareNoCase(_T("U")) == 0)	// 사용자 권한인 경우에만 확인
		m_pInfo->m_bUserInfoExportYN = (m_DM.GetCellData(0, 8).CompareNoCase(_T("Y")) == 0);
	else // 관리자 / 통합관리자는 설정 정보와 무관한게 출력권한이 존재함
		m_pInfo->m_bUserInfoExportYN = TRUE;
	// KOL.RED.2018.218 ---------------------------------------------------------------------------	


	CString strTemp;
	// 2011.10.04 ADD BY PWR : 통합도서서비스
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='통합도서서비스' AND VALUE_NAME = '통합도서서비스' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	strTemp.TrimLeft(); strTemp.TrimRight();
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bKLUse = TRUE;
	}

	// 2009.11.19 ADD BY PWR : 통합대출권수사용여부가 사용안함이면 통합환경으로 보지 않는다. (PERMITION이 'A'이면 'S'로 설정)
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME='통합대출권수사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	strTemp.TrimLeft(); strTemp.TrimRight();
	if(_T("Y") != strTemp)
	{
		if(_T("A") == m_pInfo->USER_PRIVILEGE)
		{
			m_pInfo->USER_PRIVILEGE = _T("S");
		}
	}

	//2008.11.11 ADD  BY PJW : 등록구분코드 자리수 설정
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '등록구분코드자리수설정' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	m_pInfo->nREG_CODE_LENGTH = _ttoi(strTemp);
	SaveRegCodeLengthToFile();

	// 시스템 시간을 맞춘다.
	SYSTEMTIME t;
	CTime curtime = m_DM.GetDBTime();
	curtime.GetAsSystemTime(t);
	SetLocalTime(&t);

	// 2010.08.27 ADD BY PWR : 현재일자를 기록한다.
	CString strCurrentDateFilePath = _T("..\\cfg\\CurrentDate.cfg");
	CFileFind ff;
	if(TRUE == ff.FindFile(strCurrentDateFilePath)) DeleteFile(strCurrentDateFilePath);
	ff.Close();
	CString strCurrentDate;
	strCurrentDate.Format(_T("%04d/%02d/%02d"), curtime.GetYear(), curtime.GetMonth(), curtime.GetDay());
	FILE *fp = _tfopen(strCurrentDateFilePath, _T("w+b"));
	fputc(0xFF, fp); fputc(0xFE, fp);
	_ftprintf(fp, strCurrentDate);
	fclose(fp);

	//++2008.10.15 DEL BY PWR {{++
	// 통합시스템 적용
	// 관리구분별 도서관 설정 Y/N 을 읽어온다.
//	CString strTmp;
//	m_DM.GetOneValueQuery(	_T("SELECT VALUE_NAME ")
//							_T("FROM ESL_MANAGE_TBL ")
//							_T("WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' ")
//							_T("AND CLASS_ALIAS='관리구분별도서관설정' ")
//							_T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp );
//	strTmp.TrimLeft();		
//	strTmp.TrimRight();
//	strTmp.MakeUpper();
	//--2008.10.15 DEL BY PWR --}}

	// EFS를 위한 USER_INFO를 파일에 저장
	CString strResult[2];
	CStdioFile file;

	// 2009.03.11 UPDATE BY CJY : 여러개의 관리구분으로 로그인 할 경우를 위해 PID로 구분한다.
	DWORD dwSID = ::GetCurrentProcessId();
	CString strCHMfileName;
	strCHMfileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwSID);
		
// 	if (file.Open(_T("..\\cfg\\efs\\user_info.log"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	if (file.Open(strCHMfileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		//LIB_CODE = CA
		//USER_ID = MONAMY
		//IP = 127.0.0.1
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		//++2008.10.15 UPDATE BY PWR {{++
		// 통합시스템 적용
		strQuery.Format(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL ")
						_T("WHERE MANAGE_CODE=UDF_MANAGE_CODE") );
				m_DM.GetValuesQuery(strQuery, strResult, 2);

		// 2009.04.01 ADD BY CJY : 도서관부호 설정
		m_pInfo->LIB_CODE = strResult[0];
		// 2010.07.26 ADD BY PWR : 도서관이름 설정
		m_pInfo->LIB_NAME = strResult[1];
//		if (strTmp == _T("Y")) {
//			if (m_pInfo->MANAGE_CODE.GetLength() == 0) {
//				m_DM.GetValuesQuery(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
//										_T("WHERE MANAGE_CODE IS NULL")), strResult, 2);
//			} else {
//				strQuery.Format(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
//									_T("WHERE MANAGE_CODE = '%s'")), m_pInfo->MANAGE_CODE);
//				m_DM.GetValuesQuery(strQuery, strResult, 2);
//			}
//		} else {
//			m_DM.GetValuesQuery(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL "
//									_T("WHERE MANAGE_CODE IS NULL")), strResult, 2);
//		}
		//--2008.10.15 UPDATE BY PWR --}}
		
		CString strLine;
		//strLine.Format(_T("LIB_CODE = %s\r\n"), strResult[0]);
		//file.WriteString(strLine);

		strLine.Format(_T("%s\r\n"), m_pInfo->MANAGE_CODE);
		file.WriteString(strLine);

		//strLine.Format(_T("USER_ID = %s\r\n"), m_pInfo->USER_ID);
		//file.WriteString(strLine);



		//strLine.Format(_T("IP = %s\r\n"), m_pInfo->LOCAL_IP);
		//file.WriteString(strLine);

		file.Close();

		// 2009.03.18 ADD BY PDJ : 관리구분이 없는 경우 진행할 수 없다.
		if ( m_pInfo->MANAGE_CODE == _T("") )
		{
			AfxMessageBox(_T("로그인 사용자의 관리구분이 지정되지 않았습니다.\r\n")
							_T("관리구분이 없는 상태로 진행할 수 없습니다.\r\n")
							_T("관리자에게 문의하십시오."));			
			return 100;
		}
	}

	// MENU 및 SPF_PATH, SPF_STEP, SM초기화
	SetProgramPath(_T("SPF_PATH"),	_T("NULL"));
	SetProgramPath(_T("SPF_STEP"),	_T("NULL"));
	SetProgramPath(_T("SM"),		_T("NULL"));
	SetProgramPath(_T("MENU"),		_T("HOME"));

	// 도서관명 얻어오기
	m_pInfo->TITLE = strResult[1];

	//strQuery.Format(_T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME='도서관명'"));
	//ids = m_DM.GetOneValueQuery(strQuery, m_pInfo->TITLE);

	//=====================================================
	//2009.01.28 ADD BY PJW : 부호관리에 사용할 관리구분을 로그인  USER의 관리구분으로 셋팅한다.
	CString strSubQuery;
	m_DM.StartFrame();
	strSubQuery.Format(_T("SELECT REC_KEY FROM CD_CODE_TBL WHERE CLASS = 1"));
	strQuery.Format(_T("UPDATE MN_USER_CODE_TBL SET DEFAULT_VALUE = 'N' WHERE CODE_KEY IN ( %s ) AND USER_KEY = %s AND DEFAULT_VALUE = 'Y';"),strSubQuery,m_pInfo->USER_PK);
	m_DM.AddFrame(strQuery);	
	strQuery.Format(_T("UPDATE MN_USER_CODE_TBL SET DEFAULT_VALUE = 'Y', ACTIVE = 'Y' WHERE CODE_KEY IN ( %s AND CODE = UDF_MANAGE_CODE) AND USER_KEY = %s;"),strSubQuery,m_pInfo->USER_PK);
	m_DM.AddFrame(strQuery);		
	ids = m_DM.SendFrame();
	if( 0 > ids)
	{
		AfxMessageBox(_T("관리구분을 셋팅하는데 실패했습니다."));
	}
	m_DM.EndFrame();
	//=====================================================
	
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='통합도서서비스' AND VALUE_NAME='로컬서비스' AND MANAGE_CODE = '%s'"),m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);

	strCHMfileName.Format(_T("..\\CFG\\EFS\\UnionLoanServiceInfo(%u).log"), dwSID);
	if(file.Open(strCHMfileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		CString strLine;
		strLine.Format(_T("%s\r\n"), strTemp);
		file.WriteString(strLine);
		file.Close();
	}

	// 2011.12.12 ADD BY PWR : 개인정보보호
	if(10000 != m_pInfo->MODE)
	{
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='실명인증사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("Y") == strTemp) m_pInfo->m_bCertifyUse = TRUE;
		// 2014/07/02 박대우 : 아이핀사용유무 FLAG 추가
		//                     아이핀과 실명인증기능은 같이 사용할 수 없다
		//					   아이핀사용여부 우선순위가 높음
/*//ADD------------------------------------------------------------------------*/
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='아이핀사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("Y") == strTemp) m_pInfo->m_bCertifyUse = FALSE;
/*//END------------------------------------------------------------------------*/
		// 2014/07/26 박대우 : 마이핀사용유무 FLAG 추가
		//                     마이핀사용유무 FLAG 추가
		//					   마이핀사용여부가 실명인증보다 우선순위가 높게 설정
/*//ADD------------------------------------------------------------------------*/
		CString strMypinUsed;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='MYPIN사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strMypinUsed);	
		if(_T("Y") == strMypinUsed) m_pInfo->m_bCertifyUse = FALSE;
/*//END------------------------------------------------------------------------*/

		// 15/08/18 박현석 : 휴대폰인증 추가
		CString strPhoneUsed;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='휴대폰인증사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strPhoneUsed);	
		if(_T("Y") == strPhoneUsed) m_pInfo->m_bCertifyUse = FALSE;

		// 2014/07/26 박대우 : 마이핀사용유무 FLAG 추가
/*//BEFORE--------------------------------------------------------------------------
		if(TRUE == m_pInfo->m_bCertifyUse)
*///AFTER---------------------------------------------------------------------------
		// 15/08/18 박현석 : 휴대폰인증 추가
		if(TRUE == m_pInfo->m_bCertifyUse || _T("Y") == strMypinUsed || _T("Y") == strPhoneUsed)
/*//END------------------------------------------------------------------------*/
		{
			// 2012.01.25 ADD BY PWR : 주민등록번호관련조회
			strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='주민등록번호관련조회' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
			m_DM.GetOneValueQuery(strQuery, strTemp);
			if(_T("Y") == strTemp) m_pInfo->m_bCivilNoSearch = TRUE;
			// 실명인증 미들웨어 접속정보
			strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='미들웨어접속정보' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
			m_DM.GetOneValueQuery(strQuery, strTemp);
			strCHMfileName.Format(_T("..\\CFG\\EFS\\CertifyServiceInfo(%u).log"), dwSID);
			if(file.Open(strCHMfileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
			{
				if( 0 == file.GetLength() )
				{			
					TCHAR cUni = 0xFEFF;
					file.Write( &cUni, sizeof(TCHAR));
				}
				CString strLine;
				strLine.Format(_T("%s\r\n"), strTemp);
				file.WriteString(strLine);
				file.Close();
			}
			// 실명인증 신용평가정보기관
			strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='신용평가정보기관' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
			m_DM.GetOneValueQuery(strQuery, strTemp);
			if(_T("1") == strTemp) m_pInfo->m_nCertifyAgency = 1;
			strQuery = _T("SELECT AGENCY,CODE,DESCRIPTION,ACTION FROM CERTIFY_FAIL_CAUSE_TBL");
			m_pInfo->m_CertifyFailCodeDM.SetCONNECTION_INFO(CONNECTION_INFO);
			m_pInfo->m_CertifyFailCodeDM.RestructDataManager(strQuery);
			
			// 2012.12.13 DEL BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 8회
			// 동의기록여부
//			strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='동의기록여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
//			m_DM.GetOneValueQuery(strQuery, strTemp);
//			if(_T("Y") == strTemp)
//			{
//				m_pInfo->m_bAgreeInfoRecordYN = TRUE;
//			}
//			return 0;
		}
	}

	// 2012.09.26 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회
	// 갱신기간만료알림 사용여부
	if(TRUE == m_pInfo->m_bKLUse || TRUE == m_pInfo->m_bCertifyUse)
	{
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='갱신기간만료알림' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("Y") == strTemp)
		{
			m_pInfo->m_bNotifyExpirationYN = TRUE;
		}
	}
	// 2012.09.26 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2회
	// 회원정보 갱신기간
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='이용자관리' AND VALUE_NAME='회원정보갱신기간' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("") != strTemp)
	{
		m_pInfo->m_nUserInfoRenewalDay = _ttoi(strTemp);
	}

	// 2012.11.06 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='희망자료신청제한사용여부' AND VALUE_NAME='희망자료신청제한사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bFurnishWishLimitYN = TRUE;
	}
	else
	{
		m_pInfo->m_bFurnishWishLimitYN = FALSE;
	}

	//job.2018.0071 주단위 신청권수 제한 
	//==========================================================================================================================================================================================
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='희망자료신청주단위제한사용여부' AND VALUE_NAME='희망자료신청주단위제한사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bFurnishLimitByTheWeek = TRUE;
	}
	else
	{
		m_pInfo->m_bFurnishLimitByTheWeek = FALSE;
	}
	//==========================================================================================================================================================================================
	if(m_pInfo->m_bFurnishWishLimitYN)
	{
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='희망자료신청제한권수' AND VALUE_NAME='희망자료신청제한권수' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("") != strTemp)
		{
			m_pInfo->m_nFurnishWishMaxCnt = _ttoi(strTemp);
		}
	}

	//job.2018.0071 주단위 신청권수 제한
	//==========================================================================================================================================================================================
	else if (m_pInfo->m_bFurnishLimitByTheWeek)
	{
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='희망자료신청주단위제한권수' AND VALUE_NAME='희망자료신청주단위제한권수' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);
		if(_T("") != strTemp)
		{
			m_pInfo->m_nFurnishWishMaxCnt = _ttoi(strTemp);
		}
	}
	//==========================================================================================================================================================================================
		
	//JOB.2020.0004 ADD BY KYJ : 회원구분 비치희망신청 제한기능 추가
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='희망자료회원구분제한사용여부' AND VALUE_NAME='희망자료회원구분제한사용여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bRestictionToApply = TRUE;
	}
	
	// 2012.12.13 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 8회
	// 동의기록여부
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='개인정보보호' AND VALUE_NAME='동의기록여부' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if(_T("Y") == strTemp)
	{
		m_pInfo->m_bAgreeInfoRecordYN = TRUE;
	}

	// 2017/08/25 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
	// 통합마크 개정 적용
	//----------------------------------------------------------------------------------------------------------
	// 개정된 통합마크를 사용하기 위해서 DB에 설정된 통합마크 개정에 사용되는 그룹명을 조회하여 파일에 설정한다.
	// 개정된 통합마크에서 사용되는 그룹명은 "cfg\\MarcRelation\\통합마크_개정_그룹명.cfg" 파일에 기록된다. 
	// KOLASIII가 마크 그룹명을 "통합마크"를 사용할때 파일에 설정된 그룹명으로 대체된다. 
	// 신텍스 체크시 사용되는 파일명은 다음과 같이 대체된다. 
	// SimpleRelation_통합마크.txt  ->  SimpleRelation_파일에저장된그룹명.txt
	// 파일에 저장된 그룹명은 "통합마크_개정1"이며 이후 개정된 관리값이 사용될때 변경해가면서 사용하면 된다. 
	// 그룹명 대체 작업
	//  - 소스 : \Source\ESL\ESL\MarcRelation.cpp
	//  - 함수 : INT CMarcRelation::MakeMarcRelation(CString strMarcGroup)
	//  - 주석 : 통합마크 개정 적용
/*//NEW------------------------------------------------------------------------*/
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='통합마크개정' AND VALUE_NAME='그룹명' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	CString strFileName = _T("..\\cfg\\MarcRelation\\통합마크_개정_그룹명.cfg");
	DeleteFile(strFileName);
	if (!strTemp.IsEmpty()) {
		FILE *fp = _tfopen(strFileName, _T("w+b"));
		fputc(0xFF, fp); fputc(0xFE, fp);
		_ftprintf(fp, _T("그룹명=%s"), strTemp);
		fclose(fp);
	}
/*//END------------------------------------------------------------------------*/

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	strQuery.Format(_T("SELECT MAX(VALUE_1) FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='등록구분코드자리수설정' AND VALUE_NAME='등록구분자리수' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if (!strTemp.IsEmpty()) {
		m_pInfo->m_nRegCodeLength = _ttoi(strTemp);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------
	
	// KOL.RED.2018.228 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KOLAS검색엔진관리' AND VALUE_NAME='검색엔진사용유무' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
	m_DM.GetOneValueQuery(strQuery, strTemp);
	if (strTemp.CompareNoCase(_T("Y")) == 0)
	{
		CString strURL;
		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KOLAS검색엔진관리' AND VALUE_NAME='검색엔진URL' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strURL);

		CESL_SearchEngine::SetURL(strURL);
		//CESL_SearchEngine::SetURL(_T("http://192.168.1.86:8080/solr"));

		strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='KOLAS검색엔진관리' AND VALUE_NAME='검색엔진검색모드' AND MANAGE_CODE='%s'"), m_pInfo->MANAGE_CODE);
		m_DM.GetOneValueQuery(strQuery, strTemp);

		CESL_SearchEngine::SetSearchMode(_ttoi(strTemp));

		// 기본값 적용
		CESL_SearchEngine::SetTimeOut(5);
		CESL_SearchEngine::SetStart(0);
		CESL_SearchEngine::SetRows(20000);

		//{{검색엔진 상태 확인
		const INT nCoreCount = 4;
		CString strCoreAlias[nCoreCount] = {
			_T("IDX_BO_TBL"), _T("IDX_BO_TOC_TBL"),
			_T("IDX_SE_TBL"), _T("IDX_SE_TOC_TBL")
		};

		// 사용안함으로 초기화
		for (INT i=0; i<nCoreCount; i++)
			CESL_SearchEngine::SetStatus(strCoreAlias[i], FALSE);
		
		for (i=0; i<nCoreCount; i++)
		{
			CString strStatus;
			ids = CESL_SearchEngine::RequestPingQuery(strCoreAlias[i], strStatus);
			if (ids < 0) break; // 문제가 발생하였다면 더 이상 확인하지 않는다 
			CESL_SearchEngine::SetStatus(strCoreAlias[i], (strStatus.CompareNoCase(_T("OK")) == 0));
		}
		//}}

		// 코어 상태에 따라 검색엔진 사용가능 여부를 세팅한다
		BOOL bUsedFlag = FALSE;
		for (i=0; i<nCoreCount; i++)
		{
			bUsedFlag = bUsedFlag | CESL_SearchEngine::GetStatus(strCoreAlias[i]); // 사용할 수 있는 코어가 있는지 확인
			// 단행자료는 특별처리
			if (i == 0 && !bUsedFlag) break;
		}
		
		CESL_SearchEngine::SetUsed(bUsedFlag);


	}
	// KOL.RED.2018.228 ---------------------------------------------------------------------------
	// >> 2015/10/13 --MOD
	

	return 0;
}



//2003-1106 CESL_Information 변경 작업-박길주========================================	
INT CLoginDlg::DownloadSystemInfoData()
{
	INT ids;
	CString strTmp;
	CString sTemp;
	INT nMngMode = -1;
	// 2018.11.13 LJY : RFID 사용여부=====================================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
					 			 _T("FROM ESL_MANAGE_TBL ")
						         _T("WHERE CLASS_ALIAS='RFID사용여부' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp );
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("시스템정보 다운로드중 오류[RFID][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	/*
	nMngMode = 0;	//default, 해당레코드가 없을 경우의 값
	m_bUseRFID = FALSE;
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();

		if (strTmp == _T("Y")) m_bUseRFID = TRUE;

		// ==================================================================================
		nMngMode = (m_bUseRFID) ? 1 : 0;
	}
	m_pInfo->SetRFIDUseMode(nMngMode);
	*/
	//----------------------------------------------------------------------------------------------------------------------

	// SMS 서비스 사용여부 및 버젼 정보==================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
						         _T("FROM ESL_MANAGE_TBL ")
						         _T("WHERE CLASS_ALIAS='SMS버젼정보' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (통합시스템 적용)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("시스템정보 다운로드중 오류[SMS][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	nMngMode = 0;	//default, 해당레코드가 없을 경우의 값
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		//++2008.11.20 UPDATE BY KSJ {{++
		//<< K2UP에선 SMS버젼정보 사용한다는 정의가 'P(Premium)','S(Standard)','N(No)' 였으나
		//	 KOLASIII에선 'Y(Yes)','N(No)'로  정의한다 >>
		if		(strTmp == _T("N")) nMngMode = 0;
		else if (strTmp == _T("Y")) nMngMode = 1;
		else	nMngMode = 0;
		//else if (strTmp == _T("S")) nMngMode = 1;
		//else if (strTmp == _T("P")) nMngMode = 2;
		//--2008.11.20 UPDATE BY KSJ --}}
	}
	m_pInfo->SetSMSUseMode(nMngMode);

	// MONETA 서비스 사용여부============================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
								 _T("FROM ESL_MANAGE_TBL ")
						         _T("WHERE CLASS_ALIAS='MONETA사용여부' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (통합시스템 적용)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("시스템정보 다운로드중 오류[MONETA][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	nMngMode = 0;	//default, 해당레코드가 없을 경우의 값
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		if		(strTmp == _T("N")) nMngMode = 0;
		else if (strTmp == _T("Y")) nMngMode = 1;
	}
	m_pInfo->SetMONETAUseMode(nMngMode);

	// 대출자공유모드=================================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_1 ")
								 _T("FROM ESL_MANAGE_TBL ")
								 _T("WHERE CLASS_ALIAS='대출공유' ")
								 _T("AND VALUE_NAME='대출자공유버젼' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (통합시스템 적용)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("시스템정보 다운로드중 오류[대출자공유][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	nMngMode = 0;	//default, 해당레코드가 없을 경우의 값
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		if		(strTmp == _T("0")) nMngMode = 0;
		else if (strTmp == _T("1")) nMngMode = 1;
		else if (strTmp == _T("2")) nMngMode = 2;
	}
	m_pInfo->SetLoanUserShareMode(nMngMode);

	// 대출반납공유모드=================================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_1 ")
								 _T("FROM ESL_MANAGE_TBL ")
								 _T("WHERE CLASS_ALIAS='대출공유' ")
								 _T("AND VALUE_NAME='대출반납공유버젼' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (통합시스템 적용)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("시스템정보 다운로드중 오류[대출반납공유][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}
	nMngMode = 0;	//default, 해당레코드가 없을 경우의 값
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		if		(strTmp == _T("0")) nMngMode = 0;
		else if (strTmp == _T("1")) nMngMode = 1;
		else if (strTmp == _T("2")) nMngMode = 2;
	}
	m_pInfo->SetLoanInfoShareMode(nMngMode);

	// SEARCH_INDEX_TYPE=================================================================
	//		: 'C' - Oracle catsearch 사용, 'E' - ECO search 사용=========================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
							     _T("FROM ESL_MANAGE_TBL ")
			  			         _T("WHERE CLASS_ALIAS='SEARCH_INDEX_TYPE' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (통합시스템 적용)
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("시스템정보 다운로드중 오류[SEARCH_TYPE][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}

	nMngMode = 0;	//default, 해당레코드가 없을 경우의 값
	if (-4007!=ids) 
	{
		strTmp.TrimLeft();		
		strTmp.TrimRight();
		strTmp.MakeUpper();
		if		(strTmp == _T("C")) nMngMode = 0;
		else if (strTmp == _T("E")) nMngMode = 1;
	}
	m_pInfo->SetSearchIndexType(nMngMode);

	// KOL.RED.2018.228 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (nMngMode != 1 && CESL_SearchEngine::GetUsed())
	{
		// 검색엔진을 사용하는 경우 강제로 ECOSEARCH모드로 동작시킨다
		m_pInfo->SetSearchIndexType(1);
	}
	// KOL.RED.2018.228 ---------------------------------------------------------------------------

	//KOLAS II Upgrade System Client Version=============================================
	//		: 'U' - Upgrade, 'L' - Light=================================================
	ids = m_DM.GetOneValueQuery( _T("SELECT VALUE_NAME ")
								 _T("FROM ESL_MANAGE_TBL ")
								 _T("WHERE CLASS_ALIAS='LIGHT여부' ")
								 _T("AND MANAGE_CODE=UDF_MANAGE_CODE"), strTmp ); // ADD BY PWR (통합시스템 적용)

	TCHAR cClientVersion;		

	if (-4007==ids) 
	{

		if (static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->IsDebugMode()) 
		{
			sTemp.Format(_T("시스템정보 다운로드중 오류[KOLAS II Up Version 정보][정보없음]...\n\n")
						  _T("***빨간사과(ServerInstall)*** 를 이용하여\n")
						  _T("해당 도서관의 KOLAS II Up Version 정보를 설정하여야 합니다.[MANAGE 탭]\n")
						  _T("현재 해당 도서관에서는 메인 프로그램(KOLAS II UP.EXE)의 버전에 따라 자동으로 로그인 됩니다.\n\n")
						  _T("[_DEBUG모드] Upgrade 시스템으로 로그인 하시겠습니까?\n\n")
						  _T("       ->\t'예'(Yes)\t\t: Ugrade 시스템으로 로그인\n")
						  _T("\t'아니오'(No)\t: Light  시스템으로 로그인\n")
						  _T("\t'취소'(Cancel)\t: 시스템 종료"));
			INT ret = AfxMessageBox(sTemp, MB_YESNOCANCEL);
			if		(IDYES==ret) { cClientVersion = _T('U'); strTmp = _T("N"); }
			else if (IDNO ==ret) { cClientVersion = _T('L'); strTmp = _T("Y"); }
			else return -1;

			ids = 0;
		}
		else
		{
			//2003-1112[박길주] ====================================================
			//sTemp.Format(_T("시스템정보 다운로드중 오류[KIIClient][정보없음]..."));
			//AfxMessageBox(sTemp);
			//return -1;
			//2003-1112[박길주] ++++++++++++++++++++++++++++++++++++++++++++++++++++
			//도서관 유저는 KOLAS II UP.EXE 프로그램의 컴파일 옵션에 따라 로그인 되도록 한다. <- #######
#ifdef _LIGHT
			strTmp == _T("Y");
#else
			strTmp == _T("N");
#endif
			//2003-1112[박길주] ----------------------------------------------------
		}

	}
	
	if (0>ids && -4007!=ids) 
	{
		sTemp.Format(_T("시스템정보 다운로드중 오류[KOLAS II Up Version 정보][%d]..."), ids);
		AfxMessageBox(sTemp);
		return -1;
	}

	strTmp.TrimLeft();		
	strTmp.TrimRight();
	strTmp.MakeUpper();

	if		(strTmp == _T("N")) cClientVersion = _T('U');
	else if (strTmp == _T("Y")) cClientVersion = _T('L');
	else 
	{

		if (static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->IsDebugMode()) 
		{
			sTemp.Format(_T("시스템정보 다운로드중 오류[KOLAS II Up Version 잘못된 정보=(%s)]...\n\n")
						  _T("***빨간사과(ServerInstall)*** 를 이용하여\n")
						  _T("해당 도서관의 KOLAS II Up Version 정보를 설정하여야 합니다.[MANAGE 탭]\n")
						  _T("현재 해당 도서관에서는 메인 프로그램(KOLAS II UP.EXE)의 버전에 따라 자동으로 로그인 됩니다.\n\n")
						  _T("[_DEBUG모드] Upgrade 시스템으로 로그인 하시겠습니까?\n\n")
						  _T("       ->\t'예'(Yes)\t\t: Ugrade 시스템으로 로그인\n")
						  _T("\t'아니오'(No)\t: Light  시스템으로 로그인\n")
						  _T("\t'취소'(Cancel)\t: 시스템 종료"), strTmp);
			INT ret = AfxMessageBox(sTemp, MB_YESNOCANCEL);
			if		(IDYES==ret) cClientVersion = _T('U');
			else if (IDNO ==ret) cClientVersion = _T('L');
			else return -1;
		}
		else
		{
			//2003-1112[박길주] ====================================================
			//sTemp.Format(_T("시스템정보 다운로드중 오류[KIIClient 잘못된 정보=(%s)]..."), strTmp);
			//AfxMessageBox(sTemp);
			//return -1;
			//2003-1112[박길주] ++++++++++++++++++++++++++++++++++++++++++++++++++++
			//도서관 유저는 KOLAS II UP.EXE 프로그램의 컴파일 옵션에 따라 로그인 되도록 한다. <- #######
#ifdef _LIGHT
			cClientVersion = _T('L');
#else
			cClientVersion = _T('U');
#endif
			//2003-1112[박길주] ----------------------------------------------------
		}

	}
	m_pInfo->SetKIIUPClientVersion(cClientVersion);

	return 0;
}
//2003-1106 CESL_Information 변경 작업-박길주----------------------------------------	


INT CLoginDlg::LoadCodeInfo()
{
	m_pInfo->pCodeMgr->CONNECTION_INFO = CONNECTION_INFO;

	return m_pInfo->pCodeMgr->ReadCodeData(m_pInfo->USER_PK, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg 레퍼런스 다운받기..
INT CLoginDlg::DownloadESLReference()
{

	// 15/02/26 박대우 : 공동목록 소장정보 수정시 발생하는 오류 관련 처리 
	//                   공동목록 M/W 연계작업 중에 발생한 수정작업으로 기존 수정내역과 소스 병합작업이
	//                   필요하여 함수가 호출하되도록 수정하였다
//// ADD -------------------------------------------------------------------------------------
	m_pInfo->pESLMgr->SetCONNECTION_INFO(CONNECTION_INFO);

	return 0;
//// END -------------------------------------------------------------------------------------

	/**
		ESL REFERENCE를 다운 받는다.
	*/

	// esl_mgr_tbl과 esl_mgr_sub_tbl을 Join 한 Table을 가져 온다.
	m_pInfo->pESLMgr->SetCONNECTION_INFO(CONNECTION_INFO);
	
	CString strName;
	INT nVersion, nPrevVersion;
	BOOL bIsLoadRef = IsUpdateESLReference(strName, nVersion, nPrevVersion);
	if (bIsLoadRef)
	{		
		FILE *fd = _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
		if (fd) {
			fclose(fd);
			if (AfxMessageBox(_T("Reference를 다시 다운 받으시겠습니까?\r\n\r\n현재는 DEBUG 모드 입니다.(Release모드로 전환하시려면 CFG/_DEBUG 파일을 삭제하여 주십시오.)"), MB_YESNO) != IDYES) return 0;
		}

		CESL_ReferenceMgr rm;
		rm.SetCONNECTION_INFO(m_DM.CONNECTION_INFO);
		if (nVersion == 0) {
			rm.FULLDownLoad(m_pInfo, &m_ProgressCtrl);
		} else {
			if (nPrevVersion == 0) {
				rm.FULLDownLoad(m_pInfo, &m_ProgressCtrl);
			} else {
				rm.OPTIMIZEDownLoad(m_pInfo, &m_ProgressCtrl);
			}
		}

		UpdateESLReferenceVersionInfo(strName, nVersion);
	}

	return 0;
}

BOOL CLoginDlg::IsUpdateESLReference(CString &strName, INT &nVersion, INT &nPrevVersion)
{
	LPCTSTR lpszFileName = _T("..\\CFG\\Reference\\Version.cfg");
	
	INT nFind = -1;
	CString strTemp;
	CString strLine;

	nPrevVersion = 0;
	CStdioFile file;
	if (file.Open(lpszFileName, CFile::modeRead|CFile::typeBinary))
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();

			if (strLine.IsEmpty() || strLine.Left(1) == _T('.') || strLine.Left(1) == _T('#')) continue;

			nFind = strLine.Find('=');
			if (nFind < 0) continue;

			strName = strLine.Mid(0, nFind);
			nVersion = _ttoi(strLine.Mid(nFind + 1));
			nPrevVersion = nVersion;
		}
	} else {
		nPrevVersion = 0;
	}

	strName.TrimLeft();
	strName.TrimRight();
	if (strName.IsEmpty())
		strName = _T("ESL_REFERENCE");
	
	CString strQuery;
	CString strResult;
	strQuery.Format(_T("SELECT VERSION FROM ESL_VERSION_TBL WHERE NAME = '%s'"), strName, nVersion);
	if (m_DM.GetOneValueQuery(strQuery, strResult) < 0) return FALSE;
		
	nVersion = _ttoi(strResult);
	if (nPrevVersion != nVersion) return TRUE;
	return FALSE;
}

VOID CLoginDlg::UpdateESLReferenceVersionInfo(LPCTSTR lpcszName, const INT nVersion)
{
	FILE *fp = _tfopen(_T("..\\CFG\\Reference\\Version.cfg"), _T("wb"));
	if (fp == NULL) return;

		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	
	_ftprintf(fp, _T("%s=%d"), lpcszName, nVersion);

	fclose(fp);
}

INT CLoginDlg::DownloadDVM()
{
	m_pInfo->pESLDataVerifyMgr->SetCONNECTION_INFO(CONNECTION_INFO);
    return m_pInfo->pESLDataVerifyMgr->RestructDataManager(_T("select dvm.pk, dvm.db_verify_mgr_alias, dvm.connection_info, dvm.tbl_name, dvm.condition, dvm.extra_condition, dvms.field_alias, dvms.field_name, dvms.field_type, dvms.init_value, dvms.data_type, dvms.filter1, dvms.filter2 from esl_db_verify_mgr_tbl dvm, esl_db_verify_mgr_sub_tbl dvms where dvm.pk=dvms.db_verify_mgr_pk"));
}

INT CLoginDlg::DownloadCharSetData()
{
	// 버전을 체크해서 버전이 맞지 않으면 다운로드 한다.
	CString strVersionFilePath = _T("..\\cfg\\CharSetVersion.cfg");
	
	// 패치한다.
	CStdioFile file;
	CString strLine;
	if (!file.Open(strVersionFilePath, CFile::modeRead|CFile::typeBinary))
	{
		if (!file.Open(strVersionFilePath, CFile::modeReadWrite | CFile::modeCreate|CFile::typeBinary))
			return -1;
			
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}	
		
		file.Close();
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	CMap<CString, LPCTSTR, INT, INT> map;
	
	INT nFind = -1;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		map.SetAt(strLine.Mid(0, nFind), _ttoi(strLine.Mid(nFind + 1)));
	}

	file.Close();
	// 버젼 파일을 지운다.
	DeleteFile(strVersionFilePath);

	// 관리 테이블과 버전파일과의 싱크를 맞춘다.
	//CString strQuery = _T("SELECT CHARSET_NAME, VERSION, FROM_SET, TO_SET, FILE_NAME FROM CO_CHARSET_MANAGEMENT_TBL WHERE DOWN_FLAG = 'T'");
	CString strQuery = _T("SELECT REC_KEY, CHARSET_NAME, VERSION, FROM_SET, TO_SET, FILE_NAME, FILE_LEN FROM CO_CHARSET_MANAGEMENT_TBL WHERE DOWN_FLAG = 'T'");
	if (m_DM.RestructDataManager(strQuery) < 0) return -1;

	CString strManageFilePath = _T("..\\cfg\\_CO_CHARSET_MANAGEMENT_TBL.txt");
	DeleteFile(strManageFilePath);


	CString strCharSetName;
	CString strVersion;

	FILE *fVersionFile = NULL;			// 버젼정보 파일 포인터
	FILE *fManageFile = NULL;
	INT nVersion;
	INT nRowCount = m_DM.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strCharSetName = m_DM.GetCellData(idx, 1);
		strVersion = m_DM.GetCellData(idx, 2);

		if (strCharSetName.IsEmpty() || strVersion.IsEmpty()) continue;

		// _CO_CHARSET_MANAGEMENT_TBL.TXT 에 FROM_SET TO_SET FILE_NAME을 넣는다.
		fManageFile = _tfopen(strManageFilePath, _T("a+b"));
		strLine.Format(_T("%-30s|%-30s|..\\cfg\\%s|\r\n"), m_DM.GetCellData(idx, 3), m_DM.GetCellData(idx, 4), m_DM.GetCellData(idx, 5));
		if (fManageFile) {
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fManageFile, 0, SEEK_END);		
		if( 0 == ftell(fManageFile) )
		{
			/*UNCHANGE*/fputc(0xFF, fManageFile);
			/*UNCHANGE*/fputc(0xFE, fManageFile);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf(fManageFile, _T("%s"), strLine);
			fclose(fManageFile);
		}

		if (!map.Lookup(strCharSetName, nVersion))
		{
			map.SetAt(strCharSetName, -1);		
			nVersion = -1;	
		}

		/// 버전 비교
		if (nVersion < _ttoi(strVersion))
		{
			if (DownloadCharSetData(m_DM.GetCellData(idx, 0), _T("..\\CFG\\") + m_DM.GetCellData(idx, 5), _ttoi(m_DM.GetCellData(idx, 6)))) return -1;
			// 맵의 버전 정보를 업데이트 한다.
			map.SetAt(strCharSetName, _ttoi(strVersion));
			nVersion = _ttoi(strVersion);
		}
		fVersionFile = _tfopen(strVersionFilePath, _T("a+b"));
		if (fVersionFile) {
			
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fVersionFile, 0, SEEK_END);		
		if( 0 == ftell(fVersionFile) )
		{
			/*UNCHANGE*/fputc(0xFF, fVersionFile);
			/*UNCHANGE*/fputc(0xFE, fVersionFile);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf(fVersionFile, _T("%s=%d\r\n"), strCharSetName, nVersion);
			fclose(fVersionFile);
		}
	}
	return 0;
}

INT CLoginDlg::DownloadCharSetData(CString strRecKey, CString strFileName, INT nFileSize)
{
	if (strRecKey.IsEmpty()) return -1;

	INT ids = -1;
	CFile file;
	
	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	if (pBuffer == NULL)
	{
		MessageBox(_T("Malloc Memory Fail!"));
		return -1;
	}

	ids = m_DM.LOBGet(_T("CO_CHARSET_MANAGEMENT_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;
		
	//if (nBufferSize <= 0) goto END;
	
	// 파일에 저장한다.
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;

END:
	free(pBuffer);
	return 0;

ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] 다운로드에 실패했습니다."), strFileName);
	MessageBox(strMsg);
	free(pBuffer);
	return -1;
}

INT CLoginDlg::DownloadManagementData()
{
	// 버전을 체크해서 버전이 맞지 않으면 다운로드 한다.
	CString strVersionFilePath = _T("..\\cfg\\ManagementVersion.cfg");
	
	// 패치한다.
	CStdioFile file;
	CString strLine;
	if (!file.Open(strVersionFilePath, CFile::modeRead|CFile::typeBinary))
	{
		if (!file.Open(strVersionFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary))
			return -1;
			
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}	
	}

	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CMap<CString, LPCTSTR, INT, INT> map;
	
	INT nFind = -1;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		map.SetAt(strLine.Mid(0, nFind), _ttoi(strLine.Mid(nFind + 1)));
	}

	file.Close();
	// 버젼 파일을 지운다.
	DeleteFile(strVersionFilePath);

	// 마크 그룹 테이블과 버전파일과의 싱크를 맞춘다.
	CString strQuery = _T("SELECT REC_KEY, ALIAS, VERSION, FILE_NAME, FILE_SIZE FROM MN_MANAGE_FILE_TBL");
	if (m_DM.RestructDataManager(strQuery) < 0) return -1;

	CString strFileName;
	CString strAlias;
	CString strVersion;

	FILE *fVersionFile = NULL;
	INT nVersion;
	INT nRowCount = m_DM.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strAlias = m_DM.GetCellData(idx, 1);
		strVersion = m_DM.GetCellData(idx, 2);

		if (strAlias.IsEmpty() || strVersion.IsEmpty()) continue;

		if (!map.Lookup(strAlias, nVersion))
		{
			map.SetAt(strAlias, -1);
			nVersion = -1;
		}

		/// 버전 비교
		if (nVersion < _ttoi(strVersion))
		{
			strFileName.Format(_T("..\\cfg\\%s"), m_DM.GetCellData(idx, 3));
			if (DownloadManagementData(m_DM.GetCellData(idx, 0), strFileName, _ttoi(m_DM.GetCellData(idx, 4)))) return -1;
			// 맵의 버전 정보를 업데이트 한다.
			map.SetAt(strAlias, _ttoi(strVersion));
			nVersion = _ttoi(strVersion);
		}
		fVersionFile = _tfopen(strVersionFilePath, _T("a+b"));
		if (fVersionFile) {
			
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fVersionFile, 0, SEEK_END);		
		if( 0 == ftell(fVersionFile) )
		{
			/*UNCHANGE*/fputc(0xFF, fVersionFile);
			/*UNCHANGE*/fputc(0xFE, fVersionFile);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf(fVersionFile, _T("%s=%d\r\n"), strAlias, nVersion);
			fclose(fVersionFile);
		}
	}
	
	return 0;
}

INT CLoginDlg::DownloadManagementData(CString strRecKey, CString strFileName, INT nFileSize)
{
	if (strRecKey.IsEmpty()) return -1;

	INT ids = -1;
	CFile file;
	
	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	if (pBuffer == NULL)
	{
		MessageBox(_T("Malloc Memory Fail!"));
		return -1;
	}

	ids = m_DM.LOBGet(_T("MN_MANAGE_FILE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey, _T("FILE_DATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;
		
	//if (nBufferSize <= 0) goto END;
	
	// 파일에 저장한다.
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;

	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;


END:
	free(pBuffer);
	return 0;

ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] 다운로드에 실패했습니다."), strFileName);
	MessageBox(strMsg);
	free(pBuffer);
	return -1;
}

INT CLoginDlg::DownloadMarcRelation()
{
	EFS_BEGIN
	
	// 버전을 체크해서 버전이 맞지 않으면 다운로드 한다.
	CString strVersionFilePath = _T("..\\cfg\\MarcRelation\\Version.cfg");
	
	// 패치한다.
	CStdioFile file;
	CString strLine;
	if (!file.Open(strVersionFilePath, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(strVersionFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary))
			return -1;
			
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
	}
	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	CMap<CString, LPCTSTR, INT, INT> map;
	
	INT nFind = -1;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty()) continue;
		if (strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		nFind = strLine.Find('=');
		if (nFind < 0) continue;

		map.SetAt(strLine.Mid(0, nFind), _ttoi(strLine.Mid(nFind + 1)));
	}

	file.Close();
	DeleteFile(strVersionFilePath);

	// 마크 그룹 테이블과 버전파일과의 싱크를 맞춘다.
	CString strQuery = _T("SELECT PK, MARC_GROUP_CODE, VERSION, SIMPLE_FILE_SIZE FROM MARC_GROUP_TBL");
	if (m_DM.RestructDataManager(strQuery) < 0) return -1;

	CString strFileName;
	CString strMarcGroup;
	CString strVersion;

	FILE *fVersionFile;
	INT nVersion;
	INT nRowCount = m_DM.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strMarcGroup = m_DM.GetCellData(idx, 1);
		strVersion = m_DM.GetCellData(idx, 2);

		if (strMarcGroup.IsEmpty() || strVersion.IsEmpty()) continue;

		if (!map.Lookup(strMarcGroup, nVersion))
		{
			map.SetAt(strMarcGroup, -1);
			nVersion = -1;
		}

		/// 버전 비교
		if (nVersion < _ttoi(strVersion))
		{
			strFileName.Format(_T("..\\cfg\\MarcRelation\\SimpleRelation_%s.txt"), strMarcGroup);
			if (DownloadMarcRelation(m_DM.GetCellData(idx, 0), strFileName, _ttoi(m_DM.GetCellData(idx, 3)))) return -1;
			// 맵의 버전 정보를 업데이트 한다.
			map.SetAt(strMarcGroup, _ttoi(strVersion));
			nVersion = _ttoi(strVersion);
		}
		fVersionFile = _tfopen(strVersionFilePath, _T("a+b"));
		if (fVersionFile) {
			
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		// KOLAS 2 UP UNICODE 수정작업
		// 파일 생성시에는 UNICODE 헤더를 입력해야한다.
		fseek(fVersionFile, 0, SEEK_END);		
		if( 0 == ftell(fVersionFile) )
		{
			/*UNCHANGE*/fputc(0xFF, fVersionFile);
			/*UNCHANGE*/fputc(0xFE, fVersionFile);
		}
		// ------------------------------------------------------------------------------
			
			_ftprintf(fVersionFile, _T("%s=%d\r\n"), strMarcGroup, nVersion);
			fclose(fVersionFile);
		}
	}

	EFS_END
	return 0;
}

INT CLoginDlg::DownloadMarcRelation(CString strRecKey, CString strFileName, INT nFileSize)
{
	if (strRecKey.IsEmpty()) return -1;

	INT ids = -1;
	CFile file;
	
	INT nBufferSize = sizeof(BYTE) * nFileSize + 1024;
	BYTE *pBuffer = (BYTE*)malloc(nBufferSize);
	ZeroMemory(pBuffer, sizeof(BYTE) * nBufferSize);
	if (pBuffer == NULL)
	{
		MessageBox(_T("Malloc Memory Fail!"));
		return -1;
	}

	ids = m_DM.LOBGet(_T("MARC_GROUP_TBL"), _T("PK"), _T("NUMERIC"), strRecKey, _T("SIMPLE_FILE_DATA"), pBuffer, &nBufferSize);
	if (ids < 0) goto ERR;
		
	//if (nBufferSize <= 0) goto END;
	
	// 파일에 저장한다.
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) goto ERR;
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}	
		
	file.WriteHuge(pBuffer, nBufferSize);
	file.Close();	
	goto END;


END:
	free(pBuffer);
	return 0;

ERR:
	CString strMsg;
	strMsg.Format(_T("[%s] 다운로드에 실패했습니다."), strFileName);
	MessageBox(strMsg);
	free(pBuffer);
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (m_pInfo == NULL) 
	{
		MessageBox(_T("m_pInfo IS NULL"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
		
	FILE *fp = NULL;

	// 자동로그인
	fp =  _tfopen(_T("..\\CFG\\_DEBUG"), _T("rb"));
	if (fp != NULL) 
	{
		fclose(fp);
		SetTimer(0, 100, NULL);		
	}

	// 사용자 저장파일 로그인
	else
	{
		fp =  _tfopen(_T("..\\CFG\\UserLoginInfo.cfg"), _T("rb"));
		if(fp != NULL)
		{
			fclose(fp);
			if(m_strUser.IsEmpty() || m_strUser.Left(1) == _T("!") || m_strUser.Left(1) == _T("@") ) //K-BAR로 로그인하지 않는 경우!!
				SetTimer(5, 100, NULL);
		}
	}

	if ( !m_strUser.IsEmpty() && ( m_strUser.Left(1) != _T("!") && m_strUser.Left(1) != _T("@")) )
	{		
		INT		nOffset;
		INT     nIndex;

		nOffset = m_strUser.Find(_T(" "));
		m_strID = m_strUser.Left(nOffset);
		nIndex = nOffset+1;
		nOffset = m_strUser.Find(_T(" "), nIndex);	
		if ( nOffset == -1 )
		{
			m_strPassword = m_strUser.Mid(nIndex);
			m_AutoLoadMenu = _T("");
		}
		else
		{		
			m_strPassword = m_strUser.Mid(nIndex, nOffset - nIndex);
			m_AutoLoadMenu = m_strUser.Mid(nOffset+1);
		}
		SetTimer(0, 100, NULL);		
	}

	// 2005.10.11 ADD BY PDW(Jr)
	// 메일링로그인
	fp =  _tfopen(_T("..\\CFG\\_메일링유저"), _T("rb"));
	if (fp != NULL) 
	{
		SetTimer(3, 100, NULL);
		fclose(fp);
	}

	m_DM.SetDB(DBKIND);
	m_DM.SetCONNECTION_INFO(CONNECTION_INFO);

	// 프로그래스바 세팅
	m_ProgressCtrl.SetFont(GetFont());
	m_ProgressCtrl.SetWindowText(_T("아이디와 패스워드를 입력하세요."));
	m_ProgressCtrl.SetForeColour(RGB(0,128,255));
	m_ProgressCtrl.SetBkColour(RGB(0,0,0));
	m_ProgressCtrl.SetTextForeColour(RGB(64,224,208));

	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt(_T("Style"), _T("Value"), 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CLoginDlg::OnTimer(UINT nIDEvent) 
{
	// 사용자 저장파일 로그인
	if (nIDEvent == 5) 
	{
		KillTimer(5);
		CStdioFile file;
		CString sLoginInfo;
		CFWDES DES;		// 암복호화 클래스
		DES.Read_DES_File(
			_T("..\\CFG\\UserLoginInfo.cfg"),
			_T("k^Ty"),	// 암호키
			sLoginInfo	// 복호화된 문자
		);	
		// 아이디와 암호를 읽어들인다.
		INT nPos = sLoginInfo.Find(_T("\n"));
		m_strID = sLoginInfo.Left(nPos);
		m_strID.TrimLeft(); m_strID.TrimRight();
		nPos+=1;
		m_strPassword = sLoginInfo.Mid(nPos);
		m_strPassword.TrimLeft(); m_strPassword.TrimRight();

		// 읽어들인 아이디 OR 암호가 NULL이 아닌경우 체크박스 CHK!!
		if(!m_strID.IsEmpty())
		{
			((CButton*)(GetDlgItem(IDC_CHK_ID)))->SetCheck(TRUE);
			//((CButton*)(GetDlgItem(IDC_CHK_ID)))->SetButtonStyle(BS_CHECKBOX);
			// 아이디는 있지만 암호가 NULL인 경우 암호 EDTBOX에 포커스!!
			if(m_strPassword.IsEmpty())
			{
				((CEdit*)GetDlgItem(IDC_ePASSWORD))->SetFocus();
			}
		}
		if(!m_strPassword.IsEmpty())
		{
			((CButton*)(GetDlgItem(IDC_CHK_PWD)))->SetCheck(TRUE);
		}
		
		// EDTBOX에 읽어들인 아이디 및 암호 SET!!
		((CEdit*)GetDlgItem(IDC_eID))->SetWindowText(m_strID);
		((CEdit*)GetDlgItem(IDC_ePASSWORD))->SetWindowText(m_strPassword);
		
		m_bUserLoginMode = TRUE;
	}
	
	// 자동로그인
	if (nIDEvent == 0) {
		KillTimer(0);
		
		// 2005.11.15 ADD BY PDJ
		// 원하는 아이디로 자동 접속
		CStdioFile fFile;
		CString sLine;
		if ( fFile.Open( _T("..\\CFG\\_DEBUG"), CFile::modeRead | CFile::typeBinary ) )
		{
			TCHAR cUni;
			fFile.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fFile.SeekToBegin();
			}

			sLine.TrimRight();
			if ( fFile.ReadString( sLine ) ) m_strID = sLine;
			if ( fFile.ReadString( sLine ) ) m_strPassword = sLine;
			fFile.Close();
			if ( m_strID.IsEmpty() ) 
			{
				m_strID = _T("TKATLDH");
				m_strPassword = _T("ECO8369");				
			}
		}	

		if ( m_strID.IsEmpty() ) 
		{
			m_strID = _T("");
			m_strPassword = _T("");
		}	

		UpdateData(false);
		SetTimer(1, 500, NULL);		
	}
	if (nIDEvent == 1)
	{
		KillTimer(1);
		PostMessage(WM_KEYDOWN, VK_RETURN, 0);
	}
	// 2005.10.11 ADD BY PDW(Jr)
	// 메일링로그인
	if (nIDEvent == 3)
	{
		KillTimer(3);
		m_strID = _T("메일링");
		m_strPassword = _T("apdlffld");
		UpdateData(false);
		SetTimer(1, 500, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN )
		{
			m_bGPKI = FALSE;
			BeginLogin();
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			EndLogin(IDCANCEL);
			return FALSE;
		}
		
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

INT CLoginDlg::BeginLogin()
{
	if (m_bIsThreadRun == TRUE) return -1;
		
	// << 2015년 공공도서관 시스템 개선 2015/11/23 _LCM : GPKI 인증시 로그인 정보 수정
	if(!m_bGPKI)
	{
		UpdateData();
		if (m_strID.IsEmpty() || m_strPassword.IsEmpty()) return -1;
	}
	// >> 2015/11/23 --MOD

	((CEdit*)GetDlgItem(IDC_eID))->SetReadOnly();
	((CEdit*)GetDlgItem(IDC_ePASSWORD))->SetReadOnly();

	//사용자 접속정보 로그인인 경우(K-BAR 로그인이 아닌경우)
	if( m_bUserLoginMode )
	{
		//사용자 접속정보를 생성한다.
		
		//DES Algorithm이 파일이 있는경우 이어쓰기 때문에 기존 파일을 지운다!!
		FILE *fp = NULL;
		CString sLogInfoFilePath;
		sLogInfoFilePath = _T("..\\CFG\\UserLoginInfo.cfg");
		fp =  _tfopen(sLogInfoFilePath, _T("rb"));
		if (fp != NULL) 
		{
			fclose(fp);
			CFile::Remove(sLogInfoFilePath);
		}

		//아이디 및 암호를 암호화한다. (체크되어 있지 않은 경우 NULL값을 넘긴다.)
		CFWDES des;
		CString sLoginInfo,sInputID,sInputPWD;
		INT bCheckID,bCheckPSW;
		bCheckID = ((CButton*)(GetDlgItem(IDC_CHK_ID)))->GetCheck();
		bCheckPSW = ((CButton*)(GetDlgItem(IDC_CHK_PWD)))->GetCheck();
		if(bCheckID == 1)
		{
			sInputID = m_strID;
		}
		if(bCheckPSW == 1)
		{
			sInputPWD = m_strPassword;
		}
		sLoginInfo.Format(_T("%s\n%s") , sInputID , sInputPWD);
		des.Write_DES_File(_T("..\\CFG\\UserLoginInfo.cfg"),_T("k^Ty"),sLoginInfo);
		////////////////////////////////////
	}
	// 쓰레드로 로그인 프로세스를 진행한다.
	AfxBeginThread(ThreadFunc_LoginProcess, this);
	
	return 0;
}


INT CLoginDlg::EndLogin(INT nResult) 
{
	if (m_bIsThreadRun)
	{
		m_bIsThreadRun = FALSE;
		MSG msg;
		while (TRUE)
		{
			if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
				AfxGetApp()->PumpMessage();

			if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
				break;
		}
	}

	if (GetSafeHwnd())
		EndDialog(nResult);
	return 0;
}

VOID CLoginDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetBkMode(TRANSPARENT);
	m_bmpLogin.DrawDIB(&dc, 0, 0);

	// << 2015년 공공도서관 시스템 개선 2015/11/10 _LCM : GPKI 로그인 이미지 변경
	HBITMAP hBmp = 0;
	BITMAP bitmap;
	hBmp = (HBITMAP)LoadImage(0, _T("..\\bmp\\btn_gpki1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBmp, sizeof(BITMAP), &bitmap);

	((CButton*)(GetDlgItem(IDC_BUTTON_GPKI_LOGIN)))->SetBitmap(hBmp);
	// >> 2015/11/10 _LCM --MOD
	// Do not call CDialog::OnPaint() for painting messages
}

UINT CLoginDlg::OnCloseDialog(WPARAM wParam, LPARAM lParam)
{
	if (m_bIsThreadRun)
	{
		m_bIsThreadRun = FALSE;
		MSG msg;
		while (TRUE)
		{
			if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
				AfxGetApp()->PumpMessage();

			if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
				break;
		}
	}

	EndLogin(wParam);
	
	return 0;
}


INT CLoginDlg::DownLoadMenuToolbar()
{

	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadMenuRef();
	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadToolbarRef();
	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadToolGroupRef();
/*
	CString strVersionFilePath ;
    strVersionFilePath = _T("..\\cfg\\menutoolbar\\version.cfg") ; 

	BOOL bCheck ;


    CFileFind ff;
    if( ff.FindFile(strVersionFilePath) )
    {
         bCheck = TRUE ;
    }
	else bCheck = FALSE ;
	


	CMenuToolbar_Mgr MenuToolbar_Mgr(NULL) ;
	MenuToolbar_Mgr.SetConnectionInfo (m_pInfo->CONNECTION_INFO );
	MenuToolbar_Mgr.SetDefalutValue (m_strID ,_T("U"),_T("y"));
	MenuToolbar_Mgr.SetUploadValue (TRUE,TRUE,TRUE) ;
    
	
	if(bCheck == FALSE)
	{
		//AfxMessageBox(_T("Menu&Toolbar File 생성으로 시간이 지연 됩니다."));
		MenuToolbar_Mgr.Upload_File () ;
		MenuToolbar_Mgr.Make_Version_File ();
		MenuToolbar_Mgr.Download_File () ;
	}
	else 
	{
		MenuToolbar_Mgr.Download_File () ;
	}

	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadMenuRef_2();
	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadToolbarRef_2();
	static_cast<CKOLAS3_MAINView*>(m_pParentWnd)->LoadToolGroupRef_2();
*/ 
	return 0 ;
}


//=====================================================
//2008.11.05 ADD BY PJW : 등록구분코드의 LENGTH를 파일에 저장한다.
INT CLoginDlg::SaveRegCodeLengthToFile()
{
	// EFS를 위한 USER_INFO를 파일에 저장
	CString strResult[2];
	CStdioFile file;
	if (file.Open(_T("..\\Cfg\\RegCodeLength.TXT"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		CString strLine;
		strLine.Format(_T("등록구분코드자리수|%d\r\n"), m_pInfo->nREG_CODE_LENGTH);
		file.WriteString(strLine);

		file.Close();
	}
	return 0;
}
//=====================================================

void CLoginDlg::OnSetfocuseID() 
{
	// TODO: Add your control notification handler code here
	HIMC hIME;
    DWORD dwConversion, dwSentence;

    hIME = ::ImmGetContext(GetDlgItem(IDC_eID)->m_hWnd);
    if(hIME) 
    {
        ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		
		dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
		dwConversion |= IME_CMODE_NATIVE;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );

        ImmReleaseContext(GetDlgItem(IDC_eID)->m_hWnd,hIME); 
    }
}


// 2013.10.08 ADD BY BJS : 2013도서관통합서비스확대구축및개선 (일배치 로그삭제)
// 6개월 지난 로그 삭제
void CLoginDlg::ControlLog()
{
	CCfgFileApi m_CfgFileApi;
	CString filename = _T("");
	CString filepath = _T("");
	CString strDir = _T("");
	CString strDeleteFile = _T("");
	CString strFileTime = _T("");
	CString strDeleteDelay = _T("");
	CString strCurrentDay = _T("");
	CString strGetCfgDay = _T("");
	INT nDeleteDelay;
	CFileFind finder;
	CFile file;

	//오늘 실시하였는지를 검사

	CString strRunCheck = _T("..\\cfg\\로그삭제여부.cfg");
	m_CfgFileApi.SetFile(strRunCheck);
	CFileFind findFile;
	CTime t = CTime::GetCurrentTime();
	strCurrentDay.Format(_T("%04d-%02d-%02d"),t.GetYear(),t.GetMonth(),t.GetDay());	
	if(!findFile.FindFile(strRunCheck))
	{
		FILE *ff = _tfopen(strRunCheck, _T("w+b"));
		fputc(0xFF, ff); 
		fputc(0xFE, ff);
		fclose(ff);
		m_CfgFileApi.SetData(_T("최종검사일"), strCurrentDay);
	}
	findFile.Close();

	//최종검사일의 날짜와 오늘 날짜를 비교, 최종검사일이 오늘이 아닐 경우에만 로그 삭제를 실시한다.
	m_CfgFileApi.GetData(_T("최종검사일"), strGetCfgDay);
	if(strGetCfgDay != strCurrentDay)
	{

		//로그 삭제주기 파일검사

		CString strCfgFilePath = _T("..\\cfg\\로그삭제주기.cfg");
		m_CfgFileApi.SetFile(strCfgFilePath);		
		CFileFind find;
	
		// 파일이 없다면 생성하고 기본값(180일)으로 기록
		if(!find.FindFile(strCfgFilePath))
		{ 
			FILE *fp = _tfopen(strCfgFilePath, _T("w+b"));
			fputc(0xFF, fp); 
			fputc(0xFE, fp);
			fclose(fp);
			m_CfgFileApi.SetData(_T("삭제주기"), _T("180"));
		}
		find.Close();
	
		//cfg 폴더 내의 _log 파일 삭제
		if(find.FindFile(_T("..\\cfg\\_log")))
		{
			DeleteFile(_T("..\\cfg\\_log"));	
		}
		find.Close();	
	
		//cfg 폴더 내의 _framelog 파일 삭제
		if(find.FindFile(_T("..\\cfg\\_framelog")))
		{
			DeleteFile(_T("..\\cfg\\_framelog"));
		}
		find.Close();
	
		strDir = _T("..\\tunning\\*.*");
		filepath = _T("..\\tunning\\");
		BOOL bWorking = finder.FindFile(strDir);
		
		//cfg 폴더 내의 로그삭제주기.txt 파일을 통해 삭제 주기를 가져온다.
		m_CfgFileApi.GetData(_T("삭제주기"), strDeleteDelay);
		nDeleteDelay = _ttoi(strDeleteDelay);
		
	
		while(bWorking)
		{
			bWorking = finder.FindNextFile();
			filename = finder.GetFileName();			
			
			//파일의 시간과 현재 시간을 통해 차이를 구한다.
			CString strFilenameStart;
			CString strFilenameEnd;
	
			strDeleteFile.Format(_T("%s%s"),filepath,filename);
			strFilenameStart = filename.Left(3);
			strFilenameEnd = filename.Right(3);
			if(strFilenameStart == _T("ESL"))
			{
				strFileTime = filename.Mid(7,8);
				
				// 15/05/21 박대우 : 8자리가 전부 숫자인지 확인
//// ADD -------------------------------------------------------------------------------------
				INT nTimeIdx = 0;
				INT nTimeLen = strFileTime.GetLength();
				for (; nTimeIdx < nTimeLen; nTimeIdx++)
				{
					TCHAR ch = strFileTime[nTimeIdx];
					if (ch < '0' || ch > '9')
						break;
				}
				if (nTimeIdx < nTimeLen)
					continue;
//// END -------------------------------------------------------------------------------------

				CTime tCurrentTime = CTime::GetCurrentTime();
				CTime tRenewalTime(_ttoi(strFileTime.Left(4)),_ttoi(strFileTime.Mid(4,2)),_ttoi(strFileTime.Right(2)),0,0,0);
				CTimeSpan tSpan = tCurrentTime - tRenewalTime;
				INT nElapseDay = (INT)tSpan.GetDays();
				//차이가 cfg 파일에 설정된 숫자보다 클 경우, 로그를 삭제한다.
				if(nDeleteDelay < nElapseDay)
				{	
					DeleteFile(strDeleteFile);	
				}
			}			
			//tunning 폴더 내의 txt 파일 모두 제거
			if(strFilenameEnd == _T("txt"))	DeleteFile(strDeleteFile);
			//tunning 폴더 내의 log 파일 모두 제거
			if(strFilenameEnd == _T("log")) DeleteFile(strDeleteFile);			
		}		
		finder.Close();		
		//로그 삭제가 끝난 뒤 최종 검사일을 오늘 날짜로 세팅한다.
		m_CfgFileApi.SetFile(strRunCheck);
		m_CfgFileApi.SetData(_T("최종검사일"), strCurrentDay);
	}
}

// << 2015년 공공도서관 시스템 개선 2015/11/11 _LCM : GPKI 인증서 로그인
void CLoginDlg::OnButtonGpkiLogin() 
{
	// TODO: Add your control notification handler code here

	//CString str1, str2;
	//ShowGPKILoginDlg(str1, str2);

	//2015년 공공도서관 시스템 개선 2015/11/23 _LCM : GPKI 인증시 로그인 정보
	m_bGPKI = FALSE;
	
	CGPKILoginDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		m_bUserLoginMode = FALSE;
		m_bGPKI = TRUE;
		
		m_strID = dlg.m_strLoginInfo[0];
		m_strPassword = dlg.m_strLoginInfo[1];
		//UpdateData(FALSE);		
		BeginLogin();
	}	
}
// >> 2015/11/11 --ADD
