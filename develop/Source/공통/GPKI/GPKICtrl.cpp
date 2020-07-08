// GPKICtrl.cpp: implementation of the CGPKICtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GPKICtrl.h"

#include "..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\\CfgFileApi.h"
#include "..\\..\\ESL\\FWDES.h"
//#include "..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\\LocCommonAPI.h"

#include <locale.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define WORK_DIR _T(".")
//#define CLIENT_SIGN_CERT_PATH "C:/GPKI/Certificate/class2/533이금화001_sig.cer"
//#define CLIENT_SIGN_PRIKEY_PATH "C:/GPKI/Certificate/class2/533이금화001_sig.key"
//#define CLIENT_SIGN_PRIKEY_PASSWD "gold0721"


#define SERVER_SIGN_CERT_PATH _T("../CFG/GPKI/Certificate/class1/SVR1371460003_sig.cer")
#define SERVER_SIGN_PRIKEY_PATH _T("../CFG/GPKI/Certificate/class1/SVR1371460003_sig.key")
#define SERVER_PRIKEY_PASSWD _T("rnrwnd@2015")


//#define SERVER_SIGN_CERT_PATH	_T("D:/GPKI/certificate/class1/WMA1371460001_sig.cer")
//#define SERVER_SIGN_PRIKEY_PATH _T("D:/GPKI/certificate/class1/WMA1371460001_sig.key")
//#define SERVER_PRIKEY_PASSWD _T("2014@kolas")

# define API_CONF_FILE_PATH "../CFG/GPKI/gpkiapi.conf"
# define ROOT_CERT_PATH1 "../CFG/GPKI/Certificate/class1/GPKIRootCA1.der"

CGPKICtrl::CGPKICtrl()
{

	m_strServerCert = _T("");
	m_strServerKey = _T("");
	m_strServerPwd = _T("");
	m_strRootCert = _T("");

	CString strGPKIPath = _T("..\\cfg\\GPKI.cfg");
	CFileFind findFile;

	
	CCfgFileApi cfgFileApi;

	cfgFileApi.SetFile(strGPKIPath);

	if(!findFile.FindFile(strGPKIPath))
	{
		FILE *ff = _tfopen(strGPKIPath, _T("w+b"));
		fputc(0xFF, ff); 
		fputc(0xFE, ff);
		fclose(ff);
		cfgFileApi.SetData(_T("GPKI_Path"), _T("C:\\GPKI"));
		cfgFileApi.SetData(_T("SERVER_SIGN_CERT_PATH"), SERVER_SIGN_CERT_PATH);
		cfgFileApi.SetData(_T("SERVER_SIGN_PRIKEY_PATH"), SERVER_SIGN_PRIKEY_PATH);
		cfgFileApi.SetData(_T("ROOT_CERT_PATH1"), ROOT_CERT_PATH1);

		/*
		CFWDES des;		
		des.Simple_Cipher(
		SERVER_PRIKEY_PASSWD,
		_T("k^Ty"),	// 암호키
		m_strServerPwd	
		);
		*/
		m_strServerPwd = SERVER_PRIKEY_PASSWD;

		CFWDES des;
		des.Write_DES_File(_T("..\\CFG\\GPKIServerInfo.cfg"),_T("k^Ty"), m_strServerPwd);
	}
	findFile.Close();

	cfgFileApi.GetData(_T("SERVER_SIGN_CERT_PATH"),m_strServerCert);
	cfgFileApi.GetData(_T("SERVER_SIGN_PRIKEY_PATH"),m_strServerKey);	
	cfgFileApi.GetData(_T("ROOT_CERT_PATH1"),m_strRootCert);	
	
		
	CFWDES DES;		// 암복호화 클래스
	int nRtn = DES.Read_DES_File(
		_T("..\\CFG\\GPKIServerInfo.cfg"),
		_T("k^Ty"),	// 암호키
		m_strServerPwd	// 복호화된 문자
	);
	if(nRtn != 0)
	{
		m_strServerPwd = SERVER_PRIKEY_PASSWD;

		CFWDES des;
		des.Write_DES_File(_T("..\\CFG\\GPKIServerInfo.cfg"),_T("k^Ty"), m_strServerPwd);
		DES.Read_DES_File(_T("..\\CFG\\GPKIServerInfo.cfg"),_T("k^Ty"),	m_strServerPwd );
	}
	m_strServerPwd.TrimRight();
	

	m_pEditLog = NULL;
}

CGPKICtrl::~CGPKICtrl()
{

}

BOOL CGPKICtrl::VerifyGPKI(CString strCert, CString strPwd, CString &strUserUID, CString& strUserKey)
{
	int nRv = GPKI_OK;
	void *pServerCtx = NULL;
	void *pClientCtx = NULL;
	char szSignTime[ 20] ;
	char szClientDN[ 256] ;

	char szMBSBuf1[512];	
	memset(szMBSBuf1, 0x00, sizeof(szMBSBuf1));
	char szMBSBuf2[512];	
	memset(szMBSBuf2, 0x00, sizeof(szMBSBuf2));
	TCHAR szWCSBuf[512];
	memset(szWCSBuf, 0x00, sizeof(szWCSBuf));

	BOOL bRtn = TRUE;

	CString strKey = _T("");

	BINSTR bsRandom, bsData;
	BINSTR bsCert, bsPriKey;
	BINSTR bsSignedData;
	BINSTR bsClientCert, bsServerCert, bsServerPriKey;
	// 변수 초기화
	memset(szSignTime, 0x00, sizeof(szSignTime));
	memset(szClientDN, 0x00, sizeof(szClientDN));
	GPKI_BINSTR_Create(&bsRandom);
	GPKI_BINSTR_Create(&bsData);
	GPKI_BINSTR_Create(&bsCert);
	GPKI_BINSTR_Create(&bsPriKey);
	GPKI_BINSTR_Create(&bsSignedData);
	GPKI_BINSTR_Create(&bsClientCert);
	GPKI_BINSTR_Create(&bsServerCert);
	GPKI_BINSTR_Create(&bsServerPriKey);
	// # # # # # # # # # # # # # # # # # #
	// # # # 서버 # # #
	// # # # # # # # # # # # # # # # # # #
	// API 초기화
	nRv = GPKI_API_Init(&pServerCtx, (char* )WORK_DIR);
	if (nRv != GPKI_OK)
	{
		printf("[SERVER] ErrCode = %d\n", nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// 단계 : ①
	nRv = GPKI_CRYPT_GenRandom(pServerCtx, 20, &bsRandom);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// # # # # # # # # # # # # # # # # # #
	// # # # 클라이언트 # # #
	// # # # # # # # # # # # # # # # # # #
	// 클라이언트 측 API 초기화
	nRv = GPKI_API_Init(&pClientCtx, (char* )WORK_DIR);
	if (nRv != GPKI_OK)
	{
		printf("[ CLIENT] ErrCode = %d\n", nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// 단계 : ②

	wcstombs(szMBSBuf1, strCert.GetBuffer(512), sizeof(szMBSBuf1));
	nRv = GPKI_STORAGE_ReadCert(pClientCtx, MEDIA_TYPE_FILE_PATH,		
		(char* )szMBSBuf1, DATA_TYPE_GPKI_SIGN, &bsCert);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	strKey = strCert;

	strKey.Replace(_T(".cer"),_T(".key"));
    wcstombs(szMBSBuf1, strKey.GetBuffer(512), sizeof(szMBSBuf1));
	wcstombs(szMBSBuf2, strPwd.GetBuffer(512), sizeof(szMBSBuf2));

	nRv = GPKI_STORAGE_ReadPriKey(pClientCtx, MEDIA_TYPE_FILE_PATH,
		(char* )szMBSBuf1, (char* )szMBSBuf2,
		DATA_TYPE_GPKI_SIGN, &bsPriKey);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	} 

	// 단계 : ③
	nRv = GPKI_CMS_MakeSignedData(pClientCtx, &bsCert, &bsPriKey, &bsRandom, NULL,
		&bsSignedData);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// # # # # # # # # # # # # # # # # # #
	// # # # 서버 # # #
	// # # # # # # # # # # # # # # # # # #
	// 단계 : ④
	nRv = GPKI_CMS_ProcessSignedData(pServerCtx, &bsSignedData, &bsData, &bsClientCert,
		szSignTime);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	// 단계 : ⑤
	if (bsData.nLen != bsRandom.nLen ||
		memcmp(bsData.pData, bsRandom.pData, bsRandom.nLen) != 0)
	{
		printf("[SERVER] 서버가 전송했던 랜덤값과 클라이언트가 보낸값이 다릅니다.\n");
		bRtn = FALSE;
		goto memfree;
	}
	// 단계 : ⑥

	wcstombs( szMBSBuf1, m_strServerCert.GetBuffer(512), sizeof(szMBSBuf1));
	nRv = GPKI_STORAGE_ReadCert(pServerCtx, MEDIA_TYPE_FILE_PATH,		
		(char*)szMBSBuf1, DATA_TYPE_GPKI_SIGN, &bsServerCert);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	wcstombs( szMBSBuf1, m_strServerKey.GetBuffer(512), sizeof(szMBSBuf1));
	wcstombs( szMBSBuf2, m_strServerPwd.GetBuffer(512), sizeof(szMBSBuf2));

	nRv = GPKI_STORAGE_ReadPriKey(pServerCtx, MEDIA_TYPE_FILE_PATH,
		(char* )szMBSBuf1, (char* )szMBSBuf2,
		//DATA_TYPE_GPKI_SIGN, &bsPriKey);
		DATA_TYPE_GPKI_SIGN, &bsServerPriKey);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	// 단계 : ⑦ (3.2.5. 참조)	
	//if (VerifyCert(&bsClientCert, CERT_TYPE_SIGN, &bsServerCert, &bsPriKey) > 0)
	if (VerifyCert(&bsClientCert, CERT_TYPE_SIGN, &bsServerCert, &bsServerPriKey) > 0)
	//if (VerifyCert(&bsServerCert, CERT_TYPE_SIGN, &bsClientCert, &bsPriKey) > 0)
	{
		bRtn = FALSE;
		goto memfree;
	}
	// 단계 : ⑧
	nRv = GPKI_CERT_Load(pServerCtx, &bsClientCert);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	nRv = GPKI_CERT_GetSubjectName(pServerCtx, sizeof(szClientDN), (char* )szClientDN);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	nRv = GPKI_CERT_Unload(pServerCtx);
	if (nRv != GPKI_OK)
	{
		DisplayServerErr(pServerCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	nRv = GPKI_CERT_Load(pClientCtx, &bsClientCert);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	nRv = GPKI_CERT_GetUID(pClientCtx, 512, szMBSBuf1);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}	
	mbstowcs( szWCSBuf, szMBSBuf1, sizeof(szWCSBuf));	
	strUserUID.Format(_T("%s"),szWCSBuf);

	nRv = GPKI_CERT_GetSerialNum(pClientCtx, 512, szMBSBuf1);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf1, sizeof(szWCSBuf));	
	strUserKey.Format(_T("%s"),szWCSBuf);

	nRv = GPKI_CERT_Unload(pClientCtx);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	// 획득한 클라이언트의 인증서 이름 (szClientDN)을 이용하여 해당 사용자가
	// 서버에서 제공하는 서비스를 이용할 수 있는 사용자 인지의 여부를 확인하고
	// 클라이언트의 로그인을 수용한다.	

memfree:
	// 변수 메모리 해제
	GPKI_BINSTR_Delete(&bsRandom);
	GPKI_BINSTR_Delete(&bsData);
	GPKI_BINSTR_Delete(&bsCert);
	GPKI_BINSTR_Delete(&bsPriKey);
	GPKI_BINSTR_Delete(&bsSignedData);
	GPKI_BINSTR_Delete(&bsClientCert);
	GPKI_BINSTR_Delete(&bsServerCert);
	GPKI_BINSTR_Create(&bsServerPriKey);
	// API 사용 종료
	GPKI_API_Finish(&pServerCtx);
	GPKI_API_Finish(&pClientCtx);

	return bRtn;
}

BOOL CGPKICtrl::VerifyCertList(CString strFile, CString * pStrValues, int nCount)
{
	_tsetlocale(LC_ALL, _T("Kor"));

	void *pClientCtx = NULL;

	BINSTR bsClientCert;
	GPKI_BINSTR_Create(&bsClientCert);

	CString strTmp = _T("");

	char szMBSBuf[512];	
	memset(szMBSBuf, 0x00, sizeof(szMBSBuf));
	TCHAR szWCSBuf[512];
	memset(szWCSBuf, 0x00, sizeof(szWCSBuf));

	int nIdx = 0;

	BOOL bRtn = TRUE;

	int nRv = GPKI_API_Init(&pClientCtx, (char* )WORK_DIR);
	if (nRv != GPKI_OK)
	{
		printf("[ CLIENT] ErrCode = %d\n", nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	wcstombs( szMBSBuf, strFile.GetBuffer(512), sizeof(szMBSBuf));
	nRv = GPKI_STORAGE_ReadCert(pClientCtx, MEDIA_TYPE_FILE_PATH,		
		(char*)szMBSBuf, DATA_TYPE_GPKI_SIGN, &bsClientCert);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	nRv = GPKI_CERT_Load(pClientCtx, &bsClientCert);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}

	nRv = GPKI_CERT_GetKeyUsage(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	strTmp.Format(_T("%s"),szWCSBuf);
	if(strTmp.Compare(_T("digitalSignature")) != 0 && strTmp.Compare(_T("digitalSignature, nonRepudiation")) != 0 )
	{
		bRtn = FALSE;
		goto memfree;
	}

	//nIdx = m_pDM->GetRowCount();
	//m_pDM->InsertRow(-1);	
	
	nRv = GPKI_CERT_GetUID(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}	
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	strTmp.Format(_T("%s"),szWCSBuf);
	//m_pDM->SetCellData(_T("인증서아이디"),strTmp,nIdx);
	pStrValues[0] = strTmp;


	nRv = GPKI_CERT_GetValidity(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		bRtn = FALSE;DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	strTmp.Format(_T("%s"),szWCSBuf);
	strTmp = strTmp.Mid(22, 10);
	//m_pDM->SetCellData(_T("만료일"),strTmp,nIdx);
	pStrValues[1] = strTmp;
	pStrValues[2] = _T("");


	/*
	nRv = GPKI_CERT_GetKeyUsage(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		//DisplayClientErr(pClientCtx, nRv);
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	strTmp.Format(_T("%s"),szWCSBuf);
	m_pDM->SetCellData(_T("용도"),strTmp,nIdx);
	*/
	
	nRv = GPKI_CERT_GetIssuerName(pClientCtx, 512, szMBSBuf);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	mbstowcs( szWCSBuf, szMBSBuf, sizeof(szWCSBuf));	
	//strTmp.Format(_T("%s"),szWCSBuf);
		
	AfxExtractSubString(strTmp, szWCSBuf, 0, ',');

	strTmp = strTmp.Right(strTmp.GetLength()-3);

	pStrValues[3] = strTmp;

	//m_pDM->SetCellData(_T("발급기관"),strTmp,nIdx);

	//m_pDM->SetCellData(_T("파일경로"),strFile,nIdx);

	nRv = GPKI_CERT_Unload(pClientCtx);
	if (nRv != GPKI_OK)
	{
		DisplayClientErr(pClientCtx, nRv);
		bRtn = FALSE;
		goto memfree;
	}
	
	memfree:
	// 변수 메모리 해제	
	GPKI_BINSTR_Delete(&bsClientCert);	
	// API 사용 종료	
	GPKI_API_Finish(&pClientCtx);
	return bRtn;
}

int CGPKICtrl::VerifyCert(BINSTR *pCert, int nCertType, BINSTR *pMyCert, BINSTR *pMyPriKey)
{
	int nRv = GPKI_OK;
	void *pContext = 0x00;
	BINSTR bsRootCert;

	char szMBSBuf[512];	
	memset(szMBSBuf, 0x00, sizeof(szMBSBuf));

	// 변수 초기화
	GPKI_BINSTR_Create(&bsRootCert);
	// API 초기화
	nRv = GPKI_API_Init(&pContext, (char* )WORK_DIR);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}
	/*  ① “ 통합검증서버“ 를 이용하여 검증
	② 신뢰하는 최상위 인증기관 인증서 설정
	③ 인증서 검증 방법 설정
	④ 인증서 검증
	인증서 검증 성공
	성공
	실패
	- 51-
	행정전자서명 인증서비스 표준모듈개발 표준보안API 개발자 매뉴얼*/
	// 단계 : ①
	nRv = GPKI_CERT_VerifyByIVS(pContext, (char* )API_CONF_FILE_PATH, pCert, pMyCert);
	if (nRv == GPKI_OK) /* 검증 성공시 종료 */
	{
		goto memfree;
	}
	else if (nRv == ERR_REVOKED_CERT || /* 폐지된 인증서임을 확인 */
		nRv == ERR_EXPIRED_CERT || /* 만료된 인증서임을 확인 */
		nRv == ERR_PATH_VALIDATION ||
		nRv == ERR_PATH_VALIDATION_CERT_POLICIES) /* 신뢰할 수 없는 인증서임을 확인 */
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}
	// 단계 : ②
	// 행안부 루트 인증서 설정
	wcstombs( szMBSBuf, m_strRootCert.GetBuffer(512), sizeof(szMBSBuf));
	nRv = GPKI_STORAGE_ReadCert(pContext, MEDIA_TYPE_FILE_PATH, (char* )szMBSBuf,
		DATA_TYPE_OTHER, &bsRootCert);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}
	nRv = GPKI_CERT_AddTrustedCert(pContext, &bsRootCert);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}
	/*
	// 한국인터넷진흥원 (구)루트 인증서 설정
	nRv = GPKI_STORAGE_ReadCert(pContext, MEDIA_TYPE_FILE_PATH, (char* )ROOT_CERT_PATH2,
	DATA_TYPE_OTHER, &bsRootCert);
	if (nRv != GPKI_OK)
	{
	DisplayErrMsg(pContext, nRv);
	goto memfree;
	}
	nRv = GPKI_CERT_AddTrustedCert(pContext, &bsRootCert);
	if (nRv != GPKI_OK)
	{
	DisplayErrMsg(pContext, nRv);
	goto memfree;
	}
	// 한국인터넷진흥원 (신)루트 인증서 설정
	nRv = GPKI_STORAGE_ReadCert(pContext, MEDIA_TYPE_FILE_PATH, (char* )ROOT_CERT_PATH3,
	DATA_TYPE_OTHER, &bsRootCert);
	if (nRv != GPKI_OK)
	{
	DisplayErrMsg(pContext, nRv);
	goto memfree;
	}
	nRv = GPKI_CERT_AddTrustedCert(pContext, &bsRootCert);
	if (nRv != GPKI_OK)
	{
	DisplayErrMsg(pContext, nRv);
	goto memfree;
	}
	*/
	// 단계 : ③
	
	nRv = GPKI_CERT_SetVerifyEnv(pContext, CERT_VERIFY_FULL_PATH, /* 전체 경로 검증 */
		CERT_REV_CHECK_ALL, /* OCSP 를 이용하여 폐지여부 확인 */
		//CERT_REV_CHECK_ARL | CERT_REV_CHECK_OCSP, /* OCSP 를 이용하여 폐지여부 확인 */
		//- 52-
		//행정전자서명 인증서비스 표준모듈개발 표준보안API 개발자 매뉴얼
		true, /* 캐쉬 사용 */
		NULL, /* 시스템 현재 시간 사용 */
		NULL); /* OCSP 정보가 인증서에 포함되어있으므로 설정하지 않아도 됨 */
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}


	// 단계 : ④
	nRv = GPKI_CERT_Verify(pContext, pCert, nCertType, /* 검증하고자 하는 인증서와 종류 */
		NULL, /* 모든 용도의 인증서 수용 (특정 인증서만 수용하고자 한다면 해당 정책 설정) */
		API_CONF_FILE_PATH, /* 환경 파일 설정 */
		//false, NULL, NULL); /* OCSP 요청 메시지 서명 */
		true, pMyCert, pMyPriKey); /* OCSP 요청 메시지 서명 */
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		goto memfree;
	}

	/*
	BINSTR  bsReqMsg, bsResMsg;	

	GPKI_BINSTR_Create(&bsReqMsg);
	GPKI_BINSTR_Create(&bsResMsg);
	*/

	/*
	nRv = GPKI_OCSP_MakeOCSPReq(pContext, pCert, true, pMyCert, pMyPriKey, &bsReqMsg);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		//DisplayFail1(MODULE_TYPE_CERT, TEST_TYPE_EXPECTED_VALUE, (char*)SPACE_LEVEL_2, nRv);
		goto memfree;
	}

	
	nRv = GPKI_OCSP_SendAndRecv(pContext, NULL, pCert, &bsReqMsg, &bsResMsg);
	if (nRv != GPKI_OK)
	{
		DisplayErrMsg(pContext, nRv);
		//DisplayFail1(MODULE_TYPE_CERT, TEST_TYPE_EXPECTED_VALUE, (char*)SPACE_LEVEL_2, nRv);
		goto memfree;
	}
	*/

//	GPKI_STORAGE_WriteFile(g_pContext, "c:/ocsp.req", &bsResMsg);

	/*
	nRv = GPKI_OCSP_VerifyResMsg(pContext, &bsReqMsg, &bsResMsg, &nCertStatCnt, &bsOCSPSvrCert);
	if (nRv != GPKI_OK)
	{
		//DisplayFail1(MODULE_TYPE_CERT, TEST_TYPE_EXPECTED_VALUE, (char*)SPACE_LEVEL_2, nRv);
		goto memfree;
	}
	*/

memfree:
	GPKI_BINSTR_Delete(&bsRootCert);
	// API 사용 종료
	GPKI_API_Finish(&pContext);
	return nRv;	
}


void CGPKICtrl::DisplayErrMsg(void *pContext, int nErrCode)
{
	CString strMsg;
	char szErrMsg[512] = {0,};
	TCHAR tszErrMsg[512] = {0,};
	if (GPKI_API_GetErrInfo(pContext, 512, szErrMsg) == GPKI_OK)
	{
		mbstowcs( tszErrMsg, szErrMsg, sizeof(tszErrMsg));		
		strMsg.Format(_T("%s"), tszErrMsg);
		//strMsg.Format(_T("ErrCode = %d, ErrInfo = %s\n"), nErrCode, tszErrMsg);
	}
	else
	{
		strMsg.Format(_T("ErrCode = %d"), nErrCode);
	}

	if(m_pEditLog)
	{
		m_pEditLog->SetWindowText(strMsg);
	}
	else
	{
		AfxMessageBox(strMsg);
	}
}
/*서버측 에러 메시지*/
void CGPKICtrl::DisplayServerErr(void *pContext, int nErrCode)
{
	CString strMsg;
	char szErrMsg[ 512] = {0,};
	TCHAR tszErrMsg[512] = {0,};
	if (GPKI_API_GetErrInfo(pContext, 512, szErrMsg) == GPKI_OK)
	{
		mbstowcs( tszErrMsg, szErrMsg, sizeof(tszErrMsg));		
		//strMsg.Format(_T("[ SERVER] \nErrCode=%d,ErrMsg=%s\n"), nErrCode, tszErrMsg);
		strMsg.Format(_T("%s"), tszErrMsg);
	}
	else
	{
		strMsg.Format(_T("[ SERVER] \nErrCode=%d"), nErrCode);
	}
	
	if(m_pEditLog)
	{
		m_pEditLog->SetWindowText(strMsg);
	}
	else
	{
		AfxMessageBox(strMsg);
	}

}
/*클라이언트측 에러 메시지*/
void CGPKICtrl::DisplayClientErr(void *pContext, int nErrCode)
{
	CString strMsg;
	char szErrMsg[ 512] = {0,} ;
	TCHAR tszErrMsg[512] = {0,};
	if (GPKI_API_GetErrInfo(pContext, 512, szErrMsg) == GPKI_OK)
	{
		mbstowcs( tszErrMsg, szErrMsg, sizeof(tszErrMsg));
		//strMsg.Format(_T("[ CLIENT] \nErrCode=%d,ErrMsg=%s\n"), nErrCode, tszErrMsg);		
		strMsg.Format(_T("%s"), tszErrMsg);
	}
	else
	{
		strMsg.Format(_T("[ CLIENT] \nErrCode=%d"), nErrCode);
	}

	if(m_pEditLog)
	{
		m_pEditLog->SetWindowText(strMsg);
	}
	else
	{
		AfxMessageBox(strMsg);
	}
}