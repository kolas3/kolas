// BO_ACQ_FileImportMgr.cpp: implementation of the CBO_ACQ_FileImportMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_ACQ_FileImportMgr.h"
#include <direct.h>
#include "../../../공통/INCLUDE/BO_ACQ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define WORK_LOG _T("파일반입반출"), _T(__FILE__), __LINE__

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_ACQ_FileImportMgr::CBO_ACQ_FileImportMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_FileImportMgr::IDD, pParent)
{
	m_pParent = pParent;
}

CBO_ACQ_FileImportMgr::~CBO_ACQ_FileImportMgr()
{

}

INT CBO_ACQ_FileImportMgr::GetFilePath()
{
	//사용자가 선택한 폴더의 경로를 얻어온다.
	ITEMIDLIST        *pidlBrowse;
    TCHAR            pszPathname[MAX_PATH];
 	TCHAR    szCurrentDir[512];
	_tgetcwd(szCurrentDir, 512);

	BROWSEINFO BrInfo;
    BrInfo.pidlRoot = NULL;

    memset( &BrInfo, 0, sizeof(BrInfo) );
    BrInfo.pszDisplayName = pszPathname;
    BrInfo.lpszTitle = _T("파일이 저장된 폴더의 위치를 선택하세요");
    BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

    // 다이얼로그를 띄우기
    pidlBrowse = ::SHBrowseForFolder(&BrInfo);    

    if( pidlBrowse != NULL)
    {
        // 패스를 얻어옴
        ::SHGetPathFromIDList(pidlBrowse, pszPathname);    
		_tchdir(szCurrentDir);   
    }
	else
	{
		_tchdir(szCurrentDir);   
		return -1;
	}

	m_strImportFilePath = pszPathname;

	return 0;

}

INT CBO_ACQ_FileImportMgr::ReadMasterFileInfo()
{
	// 경로에서 마스터 파일을 연다.
	BOOL bError;
	INT nStart = -1;
	CString strFilePath, strLine, strParam;
	CStdioFile MasterFile;

	strFilePath.Format(_T("%s\\ExportFileInfo.txt"), m_strImportFilePath);
	bError = MasterFile.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
	if(!bError){
		AfxMessageBox(_T("파일을 열수가 없습니다."));
		return -1;
	}

		TCHAR cUni;
		MasterFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			MasterFile.SeekToBegin();
		}

	m_nTableCount = 0;
	// 종타입에 따라 마스터 파일의 정보를 읽어온다.
	switch((SPECIES_TYPE)m_nSpeciesType)
	{
		case BO_REQUEST:
			// 반출정보가져옴.
			while(MasterFile.ReadString(strLine))
			{
				strLine.TrimLeft(); strLine.TrimRight();
				strParam = strLine.Left(1);
				if(strParam == _T("#"))
				{
					strLine.TrimLeft(_T("#"));
					nStart = strLine.Find(_T("="));
					if(nStart < 0) {
						MasterFile.Close();
						return -1;
					}
					strParam = strLine.Left(nStart);
					if(strParam == _T("반입종타입")) {
						strParam = strLine.Mid(nStart+1);
						if(strParam != _T("BO_REQUEST")) {
							MasterFile.Close();
							return -1;
						}
					}
					else if(strParam == _T("반출항목")) {
						strParam = strLine.Mid(nStart+1);
						m_CheckColumnArray.Add(strParam);
					}
					else return -1;
				}
				else
				{
					m_strTableNameArray[m_nTableCount] = strLine;	
					m_nTableCount++;
				}
			}
			break;

		case BO_ORDER:
			// 반출정보가져옴.
			while(MasterFile.ReadString(strLine))
			{
				strLine.TrimLeft(); strLine.TrimRight();
				strParam = strLine.Left(1);
				if(strParam == _T("#"))
				{
					strLine.TrimLeft(_T("#"));
					nStart = strLine.Find(_T("="));
					if(nStart < 0) {
						MasterFile.Close();
						return -1;
					}
					strParam = strLine.Left(nStart);
					if(strParam == _T("반입종타입")) {
						strParam = strLine.Mid(nStart+1);
						if(strParam != _T("BO_ORDER")) {
							MasterFile.Close();
							return -1;
						}
					}
					else if(strParam == _T("수입년도")) m_strAcqYear = strLine.Mid(nStart+1);
					else if(strParam == _T("차수번호")) m_strGroupInfo = strLine.Mid(nStart+1);
					else if(strParam == _T("업무구분")) m_strWorkCode = strLine.Mid(nStart+1);
					else return -1;
				}
				else
				{
					m_strTableNameArray[m_nTableCount] = strLine;	
					m_nTableCount++;
				}
			}
			break;

		default:
			MasterFile.Close();
			return -1;
			break;
	}

	MasterFile.Close();
	return 0;
}

INT CBO_ACQ_FileImportMgr::FileImportProc()
{

	INT ids;
	// 파일이 저장된 경로를 얻어온다.
	ids = GetFilePath();
	if(ids < 0) return -1;

	// 마스터 파일의 정보를 읽어온다. 
	ids = ReadMasterFileInfo();
	if(ids < 0) return -1;

	// 필요한 모든 파일이 존재하는지 확인한다.
	ids = IsExistFile();
	if(ids < 0) return -1;

	// SM 불러오기.
	if (InitESL_Mgr(_T("K2UPGRADE_단행_자료반출")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return -1;
	}			

	// 검수완료 자료인 경우에는 반입할 자료의 수입년도, 차수, 업무번호와 동일한 차수가 이미 존재하는지 확인한다.
	if((SPECIES_TYPE)m_nSpeciesType == BO_ORDER)
	{
		CString strTmp;
		strTmp.Format(_T("수입년도 : %s\r\n")
			          _T("업무구분 : %s\r\n")
					  _T("차수번호 : %s\r\n")
					  _T("인 자료를 반입하시겠습니까?"),
					  m_strAcqYear, m_strWorkCode, m_strGroupInfo);
		ids = AfxMessageBox(strTmp, MB_ICONQUESTION|MB_YESNO);
		if( ids == IDNO) return -1;

		ids = CheckImportInfo();
		if(ids <0 ) return -1;
	}

	BOOL bError;
	CString strBatFile, strLine;
	CStdioFile BatFile;

	// 배치파일을 만든다.
	strBatFile.Format(_T("%s\\ImportFile.bat"), m_strImportFilePath);
	bError = BatFile.Open(strBatFile, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary);
	if(!bError) {
		AfxMessageBox(_T("파일 반입 실패!"));
		return -1;
	}

	if( 0 == BatFile.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		BatFile.Write( &cUni, sizeof(TCHAR));
	}
		
	// 비치희망테이블에 같은 rec_key를 갖는 레코드가 있으면 삭제한 후 반입받는다.
	ids = DeleteSameRecKeyInFurnishTbl();
	if(ids < 0) return -1;

	// 배치파일에 sqlldr 실행 명령어를 저장한다.
	for(INT i = 0; i < m_nTableCount; i++)
	{
		strLine.Empty();
		strLine.Format(_T("sqlldr kolas2/kolas2#@k2up control='%s\\%s.ctl' data='%s\\%s.dat'\r\n"), 
			            m_strImportFilePath, m_strTableNameArray[i], m_strImportFilePath, m_strTableNameArray[i]);
		BatFile.WriteString(strLine);
	}

	BatFile.Close();

	// 배치파일을 실행시킨다.
	HINSTANCE error;
	error = ShellExecute(NULL,_T("OPEN"), strBatFile,NULL,NULL,SW_SHOW);
	ids = (INT)error;
	if(ids < 32)  {
		AfxMessageBox(_T("파일 반입 실패!"));
		return -1;
	}

	// 2005-01-07 sqlLoader 실행 후 약간의 Sleep 시간을 주어야 commit 된다.
	Sleep(1000);

	// 비치희망자료일 경우 마스터파일에서 반입항목을 읽어와서 반입항목이 아닌 컬럼들의 값들을 전부 NULL로 Update한다.
	if((SPECIES_TYPE)m_nSpeciesType == BO_REQUEST)
	{
		ids = UpdateNotImportColumn();
		if(ids < 0)  {
			AfxMessageBox(_T("파일 반입 실패!"));
			return -1;
		}
	}
	// 2005.01.04 
	// 차수번호관리 테이블에서 마지막 차수번호를 반입받은 차수번호로 Insert한다.
	else if((SPECIES_TYPE)m_nSpeciesType == BO_ORDER)
	{
		CESL_DataMgr * TmpDM;    
		TmpDM = FindDM(_T("DM_단행_자료반출_종정보"));
		CString strQuery, result;
		strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
			            _T("WHERE KIND_CODE = 'PUR_NO' AND PUBLISH_FORM_CODE = 'MO' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = '1' AND MANAGE_CODE = UDF_MANAGE_CODE"),
						m_strAcqYear);
		TmpDM->GetOneValueQuery(strQuery, result);
		// 2005-01-19 m_strGroupInfo로 변경. 실수다...ㅡ.,ㅜ 
		if( _ttoi(result) < _ttoi(m_strGroupInfo) )
		{

			TmpDM->InitDBFieldData();
			TmpDM->StartFrame();

			strQuery.Format(_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = %s, LAST_WORK = '%s' ")
							_T("WHERE KIND_CODE = 'PUR_NO' AND PUBLISH_FORM_CODE = 'MO' AND MAKE_YEAR = '%s' AND DIVISION_VALUE = '1' AND MANAGE_CODE = UDF_MANAGE_CODE;"),
							m_strGroupInfo, m_pParent->GetWorkLogMsg(WORK_LOG), m_strAcqYear);	

			TmpDM->AddFrame(strQuery); 
			TmpDM->SendFrame();  // 위에서 sqlLoder로 이미 입력해서 SendFrame이 성공안되도 어쩔수 없다.. 난몰라..
			TmpDM->EndFrame();
		}

		// 2005-01-17
		// IDX_BO_TMP_TBL에 Insert할 쿼리 생성해서 DB에 쿼리 날린다.
		ids = SendQueryToIDX_TMP();
		if(ids <0) return -1;

	}

	return 0;

}

INT CBO_ACQ_FileImportMgr::IsExistFile()
{

	BOOL bError;
	CString strFilePath;
	CStdioFile  File;

	// 필요한 파일들이 모두 존재하는지 체크한다.
	for( INT i=0; i< m_nTableCount; i++)
	{
		strFilePath.Format(_T("%s\\%s.ctl"), m_strImportFilePath, m_strTableNameArray[i]);
		bError = File.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
		if(!bError){
			AfxMessageBox(_T("파일이 존재하지 않거나 파일을 열수가 없습니다."));
			return -1;
		}
		else File.Close();

		strFilePath.Format(_T("%s\\%s.dat"), m_strImportFilePath, m_strTableNameArray[i]);
		bError = File.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
		if(!bError){
			AfxMessageBox(_T("파일이 존재하지 않거나 파일을 열수가 없습니다."));
			return -1;
		}
		else File.Close();	
	}

	return 0;
}

/************************************************************************************************
2004.11.04 비치희망자료관리 반입에서 rec_key가 중복되는 레코드는 반입 이전에 테이블에서 삭제한다.
************************************************************************************************/
INT CBO_ACQ_FileImportMgr::DeleteSameRecKeyInFurnishTbl()
{
	BOOL bError;
	CString strFilePath, strQuery;
	CStdioFile DataFile;

	strFilePath.Format(_T("%s\\BO_FURNISH_TBL.dat"), m_strImportFilePath);
	bError = DataFile.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
	if(!bError){
		AfxMessageBox(_T("파일을 열수가 없습니다."));
		return -1;
	}

		TCHAR cUni;
		DataFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			DataFile.SeekToBegin();
		}
	
	CString strLine, strRecKey;
	INT nStart, nEnd, ids;

	CESL_DataMgr * TmpDM;    
	TmpDM =FindDM(_T("DM_단행_자료반출_비치희망정보"));
	TmpDM->StartFrame();

	while(DataFile.ReadString(strLine))
	{
		strLine.TrimLeft(); strLine.TrimRight();
		nStart = strLine.Find(_T("<start>"));
		if(nStart < 0) {
			DataFile.Close();
			return -1;
		}
		nEnd = strLine.Find(_T("<end>"));
		if(nEnd < 0) {
			DataFile.Close();
			return -1;
		}
		strRecKey = strLine.Mid((nStart+7), (nEnd-(nStart+7)));
		m_DataReckeyArray.Add(strRecKey);

		strQuery.Format(_T("DELETE FROM BO_FURNISH_TBL WHERE REC_KEY = %s;"), strRecKey);
		ids = TmpDM->AddFrame(strQuery);
		//ids = TmpDM->MakeDeleteFrame(TmpDM->TBL_NAME, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		if(ids < 0) 
			return -1;
	}

	ids = TmpDM->SendFrame();
	if(ids < 0) {
		TmpDM->EndFrame();
		return -1;
	}
	TmpDM->EndFrame();

	return 0;
}

/***************************************************************************************************************
2004.11.05 비치희망자료관리 반입에서 파일의 내용을 반입한 후 반입항목이 아닌 컬럼들의 내용은 모두 NULL로 바꾼다.
           소스 엉망이다. 나중에 다시 짜자..(ㅜ.,ㅠ)
***************************************************************************************************************/
INT CBO_ACQ_FileImportMgr::UpdateNotImportColumn()
{
	CString	strExportList[100]; // 반출항목 전체 리스트
	CString strColumnName[100]; // DB 컬럼명 
	CString strNullYN[100];		// NULL로 만들 항목인지 여부 플래그

	// 반출항목과 연관된 DB의 컬럼명
	INT nColumnCount = 0;
	strExportList[nColumnCount] = _T("서명");			strColumnName[nColumnCount] = _T("표제");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("저자");			strColumnName[nColumnCount] = _T("저자명");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("발행자");			strColumnName[nColumnCount] = _T("발행자");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("발행년");			strColumnName[nColumnCount] = _T("발행년");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("가격");			strColumnName[nColumnCount] = _T("가격");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("추천의견");		strColumnName[nColumnCount] = _T("추천의견");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("비고");			strColumnName[nColumnCount] = _T("취소사유");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
//	strExportList[nColumnCount] = _T("SMS수신여부");	strColumnName[nColumnCount] = _T("SMS수신여부");strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("비치상태");		strColumnName[nColumnCount] = _T("비치상태");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("신청일");			strColumnName[nColumnCount] = _T("신청일자");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("신청자");			strColumnName[nColumnCount] = _T("신청자");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("E-MAIL");			strColumnName[nColumnCount] = _T("전자우편");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("주민등록번호");	strColumnName[nColumnCount] = _T("주민등록번호");strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("우편번호");		strColumnName[nColumnCount] = _T("우편번호");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("주소");			strColumnName[nColumnCount] = _T("주소");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("전화번호");		strColumnName[nColumnCount] = _T("전화번호");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("핸드폰");			strColumnName[nColumnCount] = _T("핸드폰");		strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
//	strExportList[nColumnCount] = _T("메일발송일");		strColumnName[nColumnCount] = _T("메일발송일");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;
	strExportList[nColumnCount] = _T("ISBN");			strColumnName[nColumnCount] = _T("메일발송일");	strNullYN[nColumnCount] = _T("Y");	nColumnCount++;

	CESL_DataMgr * TmpDM;    
	TmpDM = FindDM(_T("DM_단행_자료반출_비치희망정보"));
	TmpDM->StartFrame();

	TmpDM->InitDBFieldData();

	CString strCheck;
	INT nCount = m_CheckColumnArray.GetSize();
	// 체크된 항목 리스트와 DB의 컬럼명이 일치하면 NULL로 Update하지 않는다.
	for( INT i=0; i < nCount; i++)
	{
		strCheck = m_CheckColumnArray.GetAt(i);
		for(INT j=0; j < nColumnCount; j++)
		{
			if(strExportList[j] == strCheck)
				strNullYN[j] = _T("N");
		}
	}

	// 파일에서 모든 레코드에 대하여 반출 항목이 아닌 컬럼들은 NULL로 Update하도록 한다.
	INT ids;
	CString strFieldAlias, strFieldName, strType, strDataType, strData;
	INT nRowCount = m_DataReckeyArray.GetSize();
	if(nRowCount < 0 ) return -1;
	INT nDBColCnt;
	BOOL bDelete;
	CString strQuery;
	// TmpDM의 컬럼을 생성하기 위해 Refresh한다.
	strQuery.Format(_T("rec_key = %s"), m_DataReckeyArray.GetAt(0));
	TmpDM->RefreshDataManager(strQuery);
	nDBColCnt = TmpDM->GetColCount();
	for(i=0; i < nRowCount; i++)
	{
/*		for(INT j=0; j < nColumnCount; j++)
		{
			if(strNullYN[j] == _T("Y"))
			{
				nColIndex = TmpDM->FindColumn(strColumnName[j]);
				if( nColIndex == -1)
					return -1;
				ids = TmpDM->FindColumnFieldName(nColIndex, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
				if(ids < 0) continue;
				
				strType.MakeUpper();	strDataType.MakeUpper();
				if(strDataType == _T("udf")) continue;
				if(strType == _T("str")) TmpDM->AddDBFieldData(strFieldName, _T("STRING"), _T(""));
				else if(strType == _T("int")) 
					TmpDM->AddDBFieldData(strFieldName, _T("NUMERIC"), _T(""));			
			}
		}
*/
		for(INT j=0; j < nDBColCnt; j++)
		{
			bDelete = TRUE;
			ids = TmpDM->FindColumnFieldName(j, strFieldAlias, strFieldName, strType, strDataType, 0, strData);
			if(ids < 0) continue;
			for(INT k=0; k < nColumnCount; k++)
			{
				if( (strFieldAlias == strColumnName[k]) && (strNullYN[k] == _T("N")) )
				{
					bDelete = FALSE;
				}
			}
			if(bDelete && (strFieldName != _T("REC_KEY")) && (strFieldName != _T("FIRST_WORK")) && (strFieldName != _T("LAST_WORK")))
			{
				//============================================
				//20080611 UPDATE BY PJW : 소문자대문자들어갈수 있으므로 소문자로 바꾸도록 통일한다.
				//strType.MakeUpper();	strDataType.MakeUpper();
				strType.MakeLower();	strDataType.MakeLower();
				//============================================
				
				if(strDataType == _T("udf")) continue;
				if(strType == _T("str")) TmpDM->AddDBFieldData(strFieldName, _T("STRING"), _T(""));
				else if(strType == _T("int")) 
					TmpDM->AddDBFieldData(strFieldName, _T("NUMERIC"), _T(""));			
			}

		}
		TmpDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), m_pParent->GetWorkLogMsg(WORK_LOG));
		TmpDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), m_pParent->GetWorkLogMsg(WORK_LOG));

		ids = TmpDM->MakeUpdateFrame(_T("BO_FURNISH_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_DataReckeyArray.GetAt(i));
		if(ids < 0) return ids;
		TmpDM->InitDBFieldData();
	}

	ids = TmpDM->SendFrame();
	if(ids < 0) 
	{
		TmpDM->EndFrame();
		return ids;
	}
	ids = TmpDM->EndFrame();
	if(ids < 0) return ids;




	return 0;
}

INT CBO_ACQ_FileImportMgr::CheckImportInfo()
{
	CESL_DataMgr * TmpDM;    
	TmpDM =FindDM(_T("DM_단행_자료반출_종정보"));

	INT ids;
	CString strExist, strQuery, strTmp;
	strExist.Empty();
	strQuery.Format(_T("SELECT COUNT(*) FROM BO_PURCHASE_SEQ_NO_TBL WHERE ACQ_YEAR = %s AND SEQ_NO = %s AND WORK_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), 
		                    m_strAcqYear, m_strGroupInfo, m_strWorkCode);
	ids = TmpDM->GetOneValueQuery(strQuery, strExist);
	if( ids <0)
		return -1;

	if(strExist != _T("0"))
	{
		strTmp.Format(_T("이미 같은 차수번호(%s)가 존재하여 파일을 반입할 수 없습니다.\r\n확인해주시기 바랍니다."), m_strGroupInfo);
		AfxMessageBox(strTmp);
		return -1;
	}

	return 0;
}

// 2005-01-17
INT CBO_ACQ_FileImportMgr::SendQueryToIDX_TMP()
{
	BOOL bError;
	CString strFilePath, strQuery;
	CStdioFile DataFile;

	strFilePath.Format(_T("%s\\IDX_BO_TBL.dat"), m_strImportFilePath);
	bError = DataFile.Open(strFilePath, CFile::modeRead | CFile::typeBinary);
	if(!bError){
		AfxMessageBox(_T("파일을 열수가 없습니다."));
		return -1;
	}
		TCHAR cUni;
		DataFile.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			DataFile.SeekToBegin();
		}
	
	CString strLine, strRecKey, strDate;
	INT nStart, nEnd, ids;

	CTime t = CTime::GetCurrentTime();
	strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	CESL_DataMgr * TmpDM;    
	TmpDM =FindDM(_T("DM_단행_자료반출_색인정보"));
	if(TmpDM == NULL)
		return -1;
	TmpDM->StartFrame();

	while(DataFile.ReadString(strLine))
	{
		strLine.TrimLeft(); strLine.TrimRight();
		nStart = strLine.Find(_T("<start>"));
		if(nStart < 0) {
			DataFile.Close();
			return -1;
		}
		nEnd = strLine.Find(_T("<end>"));
		if(nEnd < 0) {
			DataFile.Close();
			return -1;
		}
		strRecKey = strLine.Mid((nStart+7), (nEnd-(nStart+7)));
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		strQuery.Format(_T("INSERT INTO IDX_TMP_BO_TBL (REC_KEY,SPECIES_KEY,IDX_WORK_FLAG,INPUT_DATE) ")
//  						_T("VALUES (%s.NEXTVAL,%s,'I','%s');"), DEFAULT_SEQ_NAME, strRecKey, strDate);
// 		ids = TmpDM->AddFrame(strQuery);
		//=====================================================
 		
		if(ids < 0)
		{
			ESLAfxMessageBox(_T("검색엔진과의 동기화를 위한 쿼리 실행 실패"));
			return -1;
		}
	}

	ids = TmpDM->SendFrame();
	if(ids < 0) {
		TmpDM->EndFrame();
		return -1;
	}

	TmpDM->EndFrame();

	return 0;
}

			
	