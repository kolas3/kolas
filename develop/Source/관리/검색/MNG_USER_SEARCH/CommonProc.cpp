// CommonProc.cpp: implementation of the CCommonProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommonProc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommonProc::CCommonProc(CESL_Mgr* pParent, CESL_DataMgr* pDM )
{
	m_pParent = pParent;

	m_pDM = pDM;

	m_pLocCommonAPI = NULL;
	m_pLocCommonAPI = new CLocCommonAPI(m_pParent);
	m_pPreRegNoMgr = NULL;
	m_pPreRegNoMgr = new CPreRegCodeMgr(m_pParent);

	if(m_pParent->InitESL_Mgr(_T("SM_USERMAIN_MANAGER")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return ;
	}

	m_bLogin = FALSE;

	// KOL.UDF.022 시큐어코딩 보완
	m_pRegCodeDM = NULL;
	m_pSeCodeDM = NULL;
	m_pUseLimitCodeDM = NULL;
	m_pUseShelfLocCodeDM = NULL;
	m_pUserPositionDM = NULL;
	m_pIndividualUserPositionDM = NULL;
	m_pRegClassDM = NULL;
	m_pHolydayDM = NULL;
	m_pLoanInfoDM = NULL;
	m_pMemberInfoDM = NULL;
	m_nBookInfoIdx = -1;
	m_nMode = -1;
	m_strMaxRowCnt = _T("");
}

CCommonProc::~CCommonProc()
{
	if(m_pLocCommonAPI != NULL) 
	{
		delete m_pLocCommonAPI;
		m_pLocCommonAPI = NULL;
	}
	if(m_pPreRegNoMgr != NULL)
	{
		delete m_pPreRegNoMgr;
		m_pPreRegNoMgr = NULL;
	}
}

/*
@Create
 - 2009.09.15 BY PWR
@Description
 - 관리값 설정
@Parameter
 - 
@Return Value
 - -1000 : 사용자정보 Restruct실패
 - -1001 : 관리값 '검색대사용자ID'에 대한 유저정보(REC_KEY, ID, NAME)가 없는것이 있다
 - -1002 : 관리값정보 Restruct실패
 - -1003 : 관리값 '로그인모드','필수입력항목','SMS수신여부기본체크여부','메일수신여부기본체크여부','자료검색모드'에 대한 VALUE_1의 값이 없는것이 있다
 - -1004 : 관리값 '필수입력항목'의 VALUE_1의 값은 6자리가 안됨
 - -1005 : 코드 DM 생성실패
 - 0 : 정상
@Mini Spec
 1. 
*/
INT CCommonProc::LoadManageValue()
{
	INT ids;
	CString strQuery;		

//  	strQuery.Format(_T("SELECT REC_KEY, MANAGE_CODE FROM MN_USER_TBL ")
//  					_T("WHERE REC_KEY = %s"), m_pParent->m_pInfo->USER_PK);
// 	ids = m_pDM->RestructDataManager(strQuery);
// 	if(0 > ids)
// 	{
// 		AfxMessageBox(_T("검색대 사용자정보를 가져오는데 실패하였습니다.\r\n검색대 환경설정을 통해 설정해주세요."));
// 		return -1000;
// 	}
// 	// REC_KEY
// 	m_pDM->GetCellData(0, 0, m_strUserPK);	
// 	// MANAGE_CODE
// 	m_pDM->GetCellData(0, 1, m_strManageCode);	
	m_strManageCode = m_pParent->m_pInfo->MANAGE_CODE;
	if(m_strManageCode.IsEmpty())
	{
		AfxMessageBox(_T("사용자에 대한 관리구분이 설정되어있지 않습니다."));
		return -1001;
	}
	
	strQuery.Format(
		_T("SELECT ")
		_T("(SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='검색대' AND VALUE_NAME='자료검색모드' AND MANAGE_CODE=UDF_MANAGE_CODE AND ROWNUM=1),")
		_T("(SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='등록구분코드자리수설정' AND MANAGE_CODE=UDF_MANAGE_CODE AND ROWNUM=1) ")
		_T("FROM DUAL") );
	ids = m_pDM->RestructDataManager(strQuery);
	if(0 > ids) 
	{
		AfxMessageBox(_T("검색대 관리값정보를 가져오는데 실패하였습니다.\r\n검색대 환경설정을 통해 설정해주세요."));
		return -1002;
	}	
	// 자료검색모드
	m_pDM->GetCellData(0, 0, m_strSearchMode);
	if(m_strSearchMode.IsEmpty())
	{
		m_strSearchMode = _T("0");
	}
	// 등록구분코드자리수설정
	m_pDM->GetCellData(0, 1, m_strRegCodeLength);
	if(m_strRegCodeLength.IsEmpty())
	{
		m_strRegCodeLength = _T("2");
	}
	
	// LOCAL IP
	//m_strIP = GetIP();
	// 청구기호출력
	// 2010.05.12 UPDATE BY PWR : 영수증출력 인증파일 EPAC용에서 KOLAS용으로 바꿈
	CStdioFile file;		
	CString strTemp;		
	CString strLine;
	strTemp = _T("..\\cfg\\ReceiptPrintCode.cfg") ;	
	if(!file.Open (strTemp,CFile::modeRead | CFile::typeBinary)) 
	{
		m_strReceiptUseYN = _T("N");
	}
	else
	{
		TCHAR cUni;
		file.Read(&cUni, sizeof(TCHAR));
		if(0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		ids = file.ReadString (strLine);
		if(ids) m_strReceiptUseYN = strLine;
		// 16/07/01 김혜영 : 영수증 출력기 기종에 관계없이 출력 가능하도록 기능 개선
//*/ ADD ---------------------------------------------------------------------------
		m_strReceiptUseYN.TrimLeft();		m_strReceiptUseYN.TrimRight();
//*/ END ---------------------------------------------------------------------------
		if(m_strReceiptUseYN == _T("N") || m_strReceiptUseYN.IsEmpty())
		{
			m_strReceiptUseYN = _T("N");
		}
		file.Close();
	}
// 	CString sFileName = _T("..\\cfg\\UserMainReceipt.cfg");
// 	CString sCertifyFileName = _T("..\\cfg\\UserMainReceiptCode.cfg");
// 	CFileFind file;
// 	if(!file.FindFile(sFileName))
// 	{ // 파일 없다면 사용안함설정
// 		FILE *fp = _tfopen(sFileName, _T("w+b"));
// 		fputc(0xFF, fp); 
// 		fputc(0xFE, fp);
// 		
// 		_ftprintf(fp, _T("%s"), _T("N"));
// 		fclose( fp );
// 		m_strReceiptUseYN = _T("N");
// 	}
// 	else
// 	{ // 파일있다면 파일에 설정된 값으로 설정
// 		CString sValue;
// 		CStdioFile fp;
// 		if(!fp.Open(sFileName, CFile::modeRead | CFile::typeBinary))
// 		{
// 			return -1;
// 		}
// 		TCHAR cUni;
// 		fp.Read(&cUni, sizeof(TCHAR));
// 		if(0xFEFF != cUni)
// 		{
// 			fp.SeekToBegin();
// 		}
// 		fp.ReadString(sValue);
// 		fp.Close();
// 		sValue.TrimLeft();	sValue.TrimRight();
// 		if(_T("Y") == sValue)
// 		{ // 설정값이 사용함이라면 인증파일이 있을때 사용할 수 있다.
// 			CFileFind file2;
// 			if(!file2.FindFile(sCertifyFileName))
// 			{ // 인증실패
// 				m_strReceiptUseYN = _T("N");
// 			}
// 			else
// 			{ // 인증성공
// 				m_strReceiptUseYN = sValue;
// 			}
// 		}
// 		else
// 		{
// 			m_strReceiptUseYN = _T("N");
// 		}
// 	}

	ids = SetCodeDM();
	if(0 > ids)
	{
		AfxMessageBox(_T("코드정보를 가져오는데 실패하였습니다."));
		return -1005;
	}

	return 0;
}

INT CCommonProc::SendSearchBookInfoSQL(CESL_DataMgr *pDM, CString sPublishForm, CString sBoWhere, CString sSeWhere, CString &sResult, BOOL bReSearch)
{
	DWORD nSTART , nFINISH;
	nSTART = GetTickCount();

	INT ids;
	CESL_DataMgr pSeRSDM;
		
	INT nTotalCnt, nBOCnt, nSECnt;
	nTotalCnt = nBOCnt = nSECnt = 0;

	// 실제 Restruct할때 사용되는 
	CString strRestructBoWhere;
	CString strRestructSeWhere;
	// 전역WHERE를 잠시 저장
	CString strTempBoWhere = m_strBoWhere;
	CString strTempSeWhere = m_strSeWhere;
	// 결과내재검색이 아니면 전역WHERE를 담은 임시WHERE초기화 
	if(TRUE != bReSearch)
	{
		strTempBoWhere = _T("");
		strTempSeWhere = _T("");
	}

	// Query생성
	CString strBoQuery = _T("");
	CString strSeQuery = _T("");
	CString strCntQuery, strCount;
	// 자료검색모드 (0:책단위검색, 1:종단위검색)
	if(_T("0") == m_strSearchMode)
	{
		pSeRSDM.InitDMPKFromFile(_T("DM_USERMAIN_BOOKSEARCH_B_SE"));
		pSeRSDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		pSeRSDM.FreeData();

		if(TRUE == bReSearch)
		{ // 재검색이면 전역WHERE와 현재WHERE 합친 조건으로 사용
			strRestructBoWhere = strTempBoWhere + _T(" AND IB.REC_KEY IN ") + sBoWhere + _T(" AND ROWNUM <= ") + m_strMaxRowCnt + _T(" ");
			strRestructSeWhere = strTempSeWhere + _T(" AND IB.REC_KEY IN ") + sSeWhere + _T(" AND ROWNUM <= ") + m_strMaxRowCnt + _T(" ");
		}
		else
		{ // 재검색이 아니면 현재WHERE로 사용 (기본조건을 합친)
			strRestructBoWhere = 
				_T("IB.REC_KEY=BB.SPECIES_KEY AND BS.REC_KEY=BB.SPECIES_KEY AND ")
				+ sBoWhere + _T(" AND ROWNUM <= ") + m_strMaxRowCnt + _T(" ");
			strRestructSeWhere = 
				_T("IB.REC_KEY=BB.SPECIES_KEY AND BS.REC_KEY=BB.SPECIES_KEY AND ")
				_T("V.REC_KEY=BB.VOL_KEY AND ")
				+ sSeWhere + _T(" AND ROWNUM <= ") + m_strMaxRowCnt + _T(" ");
		}

		if(_T("단행") == sPublishForm)
		{
			pSeRSDM.FreeData();
			ids = pDM->RefreshDataManager(strRestructBoWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = pDM->GetRowCount();
			nSECnt = 0;
			nTotalCnt = nBOCnt+nSECnt;
		}
		else if(_T("연속") == sPublishForm)
		{
			pDM->FreeData();
			ids = pSeRSDM.RefreshDataManager(strRestructSeWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = 0;
			nSECnt = pSeRSDM.GetRowCount();
			nTotalCnt = nBOCnt+nSECnt;
		}
		else
		{
 			ids = pDM->RefreshDataManager(strRestructBoWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
 			ids = pSeRSDM.RefreshDataManager(strRestructSeWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = pDM->GetRowCount();
			nSECnt = pSeRSDM.GetRowCount();
			nTotalCnt = nBOCnt+nSECnt;
		}

		// DM 복사
		CString sData;
		const INT nColCnt = 46;
		CString sCAlias[nColCnt] =
		{
			_T("예약중인이력건수")	, _T("반납예정일")				, _T("부록자료수")			, _T("자료형태")			,
			_T("매체구분")			, _T("단행/연속구분")			, _T("IBS_청구_도서기호")	, _T("IBS_청구_분류기호")	,
			_T("분류기호구분")		, _T("IBS_청구_복본기호")		, _T("발행년")				, _T("BOOK_KEY")			,
			_T("등록번호")			, _T("IBS_청구_별치기호_코드")	, _T("자료실코드")			, _T("IBS_청구_권책기호")	,
			_T("편제")				, _T("자료상태")				, _T("저작자사항")			, _T("발행자사항")			,
			_T("IDX_KEY")			, _T("표제사항")				, _T("함이름")				, _T("관리구분")			,
			_T("이용제한구분")		, _T("등록구분")				, _T("부록여부")			, _T("편/권차")				,
			_T("면장수")			, _T("물리적특성")				, _T("크기")				, _T("딸림자료")			,
			_T("가격")				, _T("종KEY")					, _T("권호정보KEY")			, _T("본표제")				,
			_T("목차유무")			, _T("초록유무")				, _T("목차키")				, _T("이용대상구분")		,
			_T("청구기호")			, _T("대출가능여부")			, _T("자료형태설명")		, _T("자료실코드설명")		,
			_T("자료상태설명")		, _T("관리구분설명")
		};
		for(INT i = 0; i < pSeRSDM.GetRowCount(); i++)
		{
			INT nRow = pDM->GetRowCount();
			pDM->InsertRow(-1);
			CString sData;
			for(INT j = 0; j < nColCnt; j++)
			{
				sData.Empty();
				ids = pSeRSDM.GetCellData(sCAlias[j], i, sData);
				if(0 > ids) 
				{
					return -3;
				}
				ids = pDM->SetCellData(sCAlias[j], sData, nRow);
				if(0 > ids) 
				{
					return -3;
				}
			}
		}

		// 건수 확인
		if(1 > nTotalCnt)
		{
			return 1000;
		}

		// 조건저장
		m_strBoWhere = strRestructBoWhere;
		m_strSeWhere = strRestructSeWhere;
	}
	else
	{ // 종단위검색
		pSeRSDM.InitDMPKFromFile(_T("DM_USERMAIN_BOOKSEARCH_S_SE"));
		pSeRSDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		pSeRSDM.FreeData();

		if(TRUE == bReSearch)
		{
			strRestructBoWhere = strTempBoWhere + _T(" AND IB.REC_KEY IN ") + sBoWhere + _T(" ");
			strRestructSeWhere = strTempSeWhere + _T(" AND IB.REC_KEY IN ") + sSeWhere + _T(" ");
		}
		else
		{
			strRestructBoWhere = 
				_T("IB.REC_KEY=BB.SPECIES_KEY AND BS.REC_KEY=BB.SPECIES_KEY AND ")
				+ sBoWhere + _T(" ");
			strRestructSeWhere = 
				_T("IB.REC_KEY=BB.SPECIES_KEY AND BS.REC_KEY=BB.SPECIES_KEY AND ")
				_T("V.REC_KEY=BB.VOL_KEY AND ")
				+ sSeWhere + _T(" ");
		}

		if(_T("단행") == sPublishForm)
		{
			pSeRSDM.FreeData();
			ids = pDM->RefreshDataManager(strRestructBoWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = pDM->GetRowCount();
			nSECnt = 0;
			nTotalCnt = nBOCnt+nSECnt;
		}
		else if(_T("연속") == sPublishForm)
		{
			pDM->FreeData();
			ids = pSeRSDM.RefreshDataManager(strRestructSeWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = 0;
			nSECnt = pSeRSDM.GetRowCount();
			nTotalCnt = nBOCnt+nSECnt;
		}
		else
		{
 			ids = pDM->RefreshDataManager(strRestructBoWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
 			ids = pSeRSDM.RefreshDataManager(strRestructSeWhere);
 			if(0 > ids) 
 			{
 				return -3;
 			}
			nBOCnt = pDM->GetRowCount();
			nSECnt = pSeRSDM.GetRowCount();
			nTotalCnt = nBOCnt+nSECnt;
		}

		// DM복사
//+++++ 2011.9.16 채정호 [추가]
// 제목 : 서울시교육청 기능개선사업
// 요구 : 자료찾기에서 예약자료의 경우 예약으로 표시되도록 보완
		CString sData;
		const INT nColCnt = 23;
		CString sCAlias[nColCnt] =
		{
			_T("예약중인이력건수"), _T("자료형태")		, _T("매체구분")	, _T("단행/연속구분")	, _T("도서기호")	, 
			_T("분류기호")		, _T("발행년")		, _T("별치기호")		, _T("저작자사항")	, 
			_T("발행자사항")	, _T("관리구분")	, _T("IDX_KEY")			, _T("표제사항")	, 
			_T("검색용본표제")	, _T("목차유무")	, _T("초록유무")		, _T("목차키")		,
			_T("자료상태")		, _T("자료실코드")	, _T("자료상태설명")	, _T("자료실코드설명")	,
			_T("관리구분설명")	, _T("자료형태설명")
		};
//-----
		for(INT i = 0; i < pSeRSDM.GetRowCount(); i++)
		{
			INT nRow = pDM->GetRowCount();
			pDM->InsertRow(-1);
			CString sData;
			for(INT j = 0; j < nColCnt; j++)
			{
				sData.Empty();
				ids = pSeRSDM.GetCellData(sCAlias[j], i, sData);
				if(0 > ids) 
				{
					return -3;
				}
				ids = pDM->SetCellData(sCAlias[j], sData, nRow);
				if(0 > ids) 
				{
					return -3;
				}
			}
		}

		// 건수 확인
		if(1 > nTotalCnt)
		{
			return 1000;
		}
		// 조건저장
		m_strBoWhere = strRestructBoWhere;
		m_strSeWhere = strRestructSeWhere;
	}

	nFINISH = GetTickCount();
	CString sTotalCnt, sBOCnt, sSECnt;
	sTotalCnt.Format(_T("%d"), nTotalCnt);
	sBOCnt.Format(_T("%d"), nBOCnt);
	sSECnt.Format(_T("%d"), nSECnt);
	SetThousandFormat(sTotalCnt);
	SetThousandFormat(sBOCnt);
	SetThousandFormat(sSECnt);
	sResult.Format(_T("전체 %s건 (단행:%s건, 연속:%s건)     %d.%d sec."), sTotalCnt, sBOCnt, sSECnt, (nFINISH-nSTART)/1000, (nFINISH-nSTART)%1000);

	return 0;
}


INT CCommonProc::MakeShelfCode(CESL_DataMgr *pDM)
{
	INT ids;

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

		ids = m_pLocCommonAPI->MakeShelfCode(m_pParent, pDM, i, sShelfCode);
		if(0 > ids ) 
		{
			return -2;
		}
	}

	return 0;
}

INT CCommonProc::IsLoanCodeYN(CString sCodeAlias, CString sCode, CString &sValue)
{
	INT ids;
	if(sCode.IsEmpty())
	{
		sValue = _T("Y");
		return 0;
	}

	CESL_DataMgr *pCodeMgr;

	if(_T("등록구분") == sCodeAlias)
	{
		pCodeMgr = m_pRegCodeDM;
	}
	else if(_T("별치기호") == sCodeAlias)
	{
		pCodeMgr = m_pSeCodeDM;
	}
	else if(_T("이용제한구분") == sCodeAlias)
	{
		pCodeMgr = m_pUseLimitCodeDM;
	}
	else if(_T("자료실구분") == sCodeAlias)
	{
		pCodeMgr = m_pUseShelfLocCodeDM;
	}
	else 
	{
		sValue = _T("Y");
		return 0;
	}

	CString sLoanYN;
	CString sCodes;
	for(INT i = 0; i < pCodeMgr->GetRowCount(); i++)
	{
		ids = pCodeMgr->GetCellData(_T("코드"), i, sCodes);
		if(0 > ids) 
		{
			return -1;
		}
		sCodes.TrimLeft(); sCodes.TrimRight();

		if(sCodes == sCode)
		{
			ids = pCodeMgr->GetCellData(_T("대출여부"), i, sLoanYN );
			if(0 > ids) 
			{
				return -2;
			}
			sLoanYN.TrimLeft(); sLoanYN.TrimRight();
			sValue = sLoanYN;

			return 0;
		}
	}

	return 0;
}

INT CCommonProc::GetWorkingStatusDescToCode(CString &sCodeValue, INT nMode)
{
	m_pLocCommonAPI->GetWorkingStatusDescToCode(sCodeValue, nMode);
	return 0;
}


VOID CCommonProc::SetThousandFormat(CString &sValue)
{
	sValue.Replace( _T(","), _T("") );
	CString strBuf = sValue;

	INT nSize = ( strBuf.GetLength() * 3 ) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat(NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize - 1);

	sValue = pszFormattedNumber;

	if (pszFormattedNumber)
	{
		delete [] pszFormattedNumber;
	}
}

INT CCommonProc::SetCodeDM()
{
	INT ids;

// 	CESL_DataMgr pCodeDM;
// 	pCodeDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	
	// 1. DM_등록구분
	m_pRegCodeDM = m_pParent->FindDM(_T("DM_USERMAIN_등록구분"));
	if(NULL == m_pRegCodeDM) 
	{
		return -1;
	}
	m_pRegCodeDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_등록구분"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -2;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pRegCodeDM);
	// 2. DM_별치기호
	m_pSeCodeDM = m_pParent->FindDM(_T("DM_USERMAIN_별치기호"));
	if(NULL == m_pSeCodeDM) 
	{
		return -3;
	}
	m_pSeCodeDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_별치기호"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -4;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pSeCodeDM);
	
	// 3. DM_이용제한구분
	m_pUseLimitCodeDM = m_pParent->FindDM(_T("DM_USERMAIN_이용제한구분"));
	if(NULL == m_pUseLimitCodeDM) 
	{
		return -5;
	}
	m_pUseLimitCodeDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_이용제한구분"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -6;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pUseLimitCodeDM);
	
	// 4. DM_자료실
	m_pUseShelfLocCodeDM = m_pParent->FindDM(_T("DM_USERMAIN_자료실"));
	if(NULL == m_pUseShelfLocCodeDM) 
	{
		return -7;
	}
	m_pUseShelfLocCodeDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_자료실"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -8;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pUseShelfLocCodeDM);

	// 5. DM_직급별관리구분값
	m_pUserPositionDM = m_pParent->FindDM(_T("DM_USERMAIN_직급별관리구분값"));
	if(NULL == m_pUserPositionDM) 
	{
		return -7;
	}
	m_pUserPositionDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_직급별관리구분값"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -8;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pUserPositionDM);

	// 6. DM_직급별관리구분값_도서관별
	m_pIndividualUserPositionDM = m_pParent->FindDM(_T("DM_USERMAIN_직급별관리구분값_도서관별"));
	if(NULL == m_pIndividualUserPositionDM) 
	{
		return -7;
	}
	CString strWhere;
// 	strWhere.Format(_T("MANAGE_CODE='%s'"), m_strManageCode);
	strWhere.Format(_T("MANAGE_CODE=UDF_MANAGE_CODE"));
	m_pIndividualUserPositionDM->RefreshDataManager(strWhere);
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_직급별관리구분값_도서관별"));
// 	ids = pCodeDM.RefreshDataManager(strWhere);
// 	if(0 > ids)  
// 	{
// 		return -8;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pIndividualUserPositionDM);

	// 7. DM_등록직급설정값
	m_pRegClassDM = m_pParent->FindDM(_T("DM_USERMAIN_등록직급설정값"));
	if(NULL == m_pRegClassDM) 
	{
		return -7;
	}
	m_pRegClassDM->RefreshDataManager(_T(""));
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_등록직급설정값"));
// 	ids = pCodeDM.RefreshDataManager(_T(""));
// 	if(0 > ids)  
// 	{
// 		return -8;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pRegClassDM);

	// 8. DM_휴관일
	m_pHolydayDM = m_pParent->FindDM(_T("DM_USERMAIN_HOLYDAY_INFO"));
	if( NULL == m_pHolydayDM ) 
	{
		return -9;
	}
	//strWhere.Format( _T("MANAGE_CODE='%s'") , m_strManageCode );
	strWhere.Format( _T("MANAGE_CODE=UDF_MANAGE_CODE"));
	m_pHolydayDM->RefreshDataManager(strWhere);
// 	pCodeDM.InitDMPKFromFile(_T("DM_USERMAIN_HOLYDAY_INFO"));
// 	ids = pCodeDM.RefreshDataManager(strWhere);
// 	if( 0 > ids )
// 	{
// 		return -10;
// 	}
// 	CopyDM2DM(&pCodeDM, m_pHolydayDM);

	return 0;
}

INT CCommonProc::CopyDM2DM(CESL_DataMgr *pSourceDM, CESL_DataMgr *pTargetDM)
{
	pTargetDM->FreeData();
	INT nRow = pSourceDM->GetRowCount();
	INT nCurRow;
	INT row, col;
	INT nCols;	
	CString strTmp;

	if (nRow == 0) return 0;		// 소스에 데이타가 없음
	for (row = 0; row < nRow; row++) 
	{
		nCols = pSourceDM->m_nCols;
		pTargetDM->InsertRow(-1);
		nCurRow = pTargetDM->GetRowCount() - 1;
		for (col = 0; col < pSourceDM->m_nCols; col++) 
		{
			strTmp = pSourceDM->GetCellData(row, col);
			pTargetDM->SetCellData(nCurRow, col, strTmp);
		}
	}
	return 0;
}
