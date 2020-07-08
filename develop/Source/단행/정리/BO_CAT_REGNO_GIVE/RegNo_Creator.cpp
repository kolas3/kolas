// RegNo_Creator.cpp: implementation of the CRegNo_Creator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegNo_Creator.h"
#include "../../../공통/K2UP_Common/MakeSearchData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegNo_Creator::CRegNo_Creator(CESL_Mgr * pMgr):CSPF_Work(pMgr)
{

}

CRegNo_Creator::~CRegNo_Creator()
{

}

INT CRegNo_Creator::GetLastRegNo()
{
EFS_BEGIN

	CString regCode, CMAlias, DMAlias;
	INT lastRegno;
	GetTempData(_T("RegCode"), regCode);
	GetTempData(_T("CMAlias"), CMAlias);
	GetTempData(_T("DMAlias"), DMAlias);
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	if(regCode.GetLength() != 0)
		GetLastRegNo(DMAlias, regCode, &lastRegno);	
	SetTempData(_T("LastRegNo"), lastRegno);
	return 0;

EFS_END
return -1;

}

INT CRegNo_Creator::GetLastRegNo(CString DMAlias, CString regCode, INT *regno)
{
EFS_BEGIN

	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	if(regCode.GetLength() == 0) return -1000;
	CString tmpquery, tmpvalue;
	tmpquery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE DIVISION_VALUE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), regCode);
	pDM->GetOneValueQuery(tmpquery, tmpvalue);
	*regno = _ttoi(tmpvalue.GetBuffer(0));
	return 0;

EFS_END
return -1;

}

INT CRegNo_Creator::EARegProcessing()
{
EFS_BEGIN

	CString DMAlias, CMAlias;
	INT idx, lastRegno, opcode;
	GetTempData(_T("DMAlias"), DMAlias);
	GetTempData(_T("CMAlias"), CMAlias);
	GetTempData(_T("DMIndex"), idx);
	GetTempData(_T("LastRegNo"), lastRegno);
	GetTempData(_T("OperationCode"), opcode);
	CESL_ControlMgr * pCM = m_pParentMgr->FindCM(CMAlias);
	CESL_DataMgr * pDM = m_pParentMgr->FindDM(DMAlias);
	CString regno;
	pCM->GetControlMgrData(_T("등록번호"), regno);

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	if(opcode == 1000 || opcode == 2000)
	{
		//사용자 입력값 검사
		INT ids = IsValidRegNo(regno);
		if(ids < 0) return ids;
		//중복검사
		if(IsDuplcateRegNo(pDM, regno)) return -2000;
	}
	
	//등록부여 타입
	//1000:누락등록번호, 2000:마지막등록번호, 3000:마지막등록번호인데 누락등록번호삽입
	CStringList regnoList;
	INT type = CheckRegNoType(lastRegno, regno, regnoList, opcode);
	if(type == 3000) return -10000;
	//책에 갱신쿼리생성 및 AddFrame
	INT ids = MakeQueryAndAddFrame(pDM, regno, idx, opcode);
	if(ids < 0) return ids;
	//마지막 등록번호 갱신
	if(type == 1000)
	{
		//누락등록번호에서 해당번호삭제
		ids = MakeMissingRegNoUpdate(pDM, regno, regnoList);
	}
	else if(type == 2000)
	{
		//마지막 등록번호 갱신
		ids = MakeLastRegNoUpdate(pDM, regno);
	}
	else if(type == 3000)
	{
		//누락등록번호 생성 및 마지막 등록번호 갱신
		CString first, last, regcode, tmpstr;
		first = regnoList.GetHead();
		last = regnoList.GetTail();
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		regcode = regno.Left(2);
		
		regcode = regno.Left(nRegCodeLength);
		//=====================================================
		
		tmpstr.Format(_T("[%s%s]에서 [%s%s]까지의\n누락등록번호를 생성하시겠습니까?"), regcode, first, regcode, last);
		if(IDCANCEL == AfxMessageBox(tmpstr.GetBuffer(0), MB_OKCANCEL))
			return 0;
		MakeMissingRegNoUpdate(pDM, regno, regnoList);
		MakeLastRegNoUpdate(pDM, regno);
		regnoList.RemoveAll();
	}
	else if(type == 4000)
	{
		regnoList.RemoveAll();
		regnoList.AddTail(regno);
		MakeMissingRegNoUpdate(pDM, regno, regnoList);
	}
	
	ids = pDM->SendFrame();
	if(ids < 0) return -4000;
	if(opcode == 1000 || opcode == 2000)
		pDM->SetCellData(_T("BB_등록번호"), regno, idx);
	else if(opcode == -1000 || opcode == -2000)
		pDM->SetCellData(_T("BB_등록번호"), _T(""), idx);
	
	return 0;

EFS_END
return -1;

}

INT CRegNo_Creator::MakeMissingRegNoUpdate(CESL_DataMgr *pDM, CString regno, CStringList &regnoList)
{
EFS_BEGIN

	INT ids;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	CString regcode = regno.Left(2);
// 	INT regNo = _ttoi((regno.Right(10)).GetBuffer(0));
	
	CString regcode = regno.Left(nRegCodeLength);
	INT regNo = _ttoi((regno.Right(12-nRegCodeLength)).GetBuffer(0));
	//=====================================================
	
	CString tmpquery, tmpkey, tmpstr;
	INT cnt = regnoList.GetCount();
	if(cnt == 0)
	{
		tmpquery.Format(
			_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE='%s' AND ACCESSION_REG_NO=%d;"),
			regcode,
			regNo
		);
		ids = pDM->AddFrame(tmpquery);
		if ( 0 > ids ) return -1;
		return 0;
	}
	POSITION pos = regnoList.GetHeadPosition();
	for(INT i=0 ; i<cnt && pos != NULL ; i++)
	{
		ids = pDM->MakeRecKey(tmpkey);
		if ( 0 > ids ) return -1;
		tmpstr = regnoList.GetNext(pos);
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		tmpstr = tmpstr.Right(10);
		
		tmpstr = tmpstr.Right(12-nRegCodeLength);
		//=====================================================
		
		regNo = _ttoi(tmpstr.GetBuffer(0));
		tmpquery.Format(			
			_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, ACCESSION_REG_NO, FIRST_WORK, LAST_WORK, MANAGE_CODE) VALUES(%s, 'MO', '%s', %d, '%s', '%s', UDF_MANAGE_CODE);"),
//DEL 			_T("INSERT INTO CO_DROP_ACCESSION_NO_TBL (REC_KEY, PUBLISH_FORM_CODE, REG_CODE, ACCESSION_REG_NO, FIRST_WORK, LAST_WORK) VALUES(%s, 'MO', '%s', %d, '%s', '%s');"),
			tmpkey,
			regcode,
			regNo,
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG)
		);
		ids = pDM->AddFrame(tmpquery);
		if ( 0 > ids ) return -1;
	}
	return 0;

EFS_END
return -1;

}

INT CRegNo_Creator::MakeLastRegNoUpdate(CESL_DataMgr *pDM, CString regno)
{
EFS_BEGIN

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	CString regcode = regno.Left(2);
// 	INT regNo = _ttoi((regno.Right(10)).GetBuffer(0));

	CString regcode = regno.Left(nRegCodeLength);
	INT regNo = _ttoi((regno.Right(12-nRegCodeLength)).GetBuffer(0));
	//=====================================================
	
	CString tmpquery, tmpstr;
	tmpquery.Format(
		_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE DIVISION_VALUE='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"),
		regcode
	);
	pDM->GetOneValueQuery(tmpquery, tmpstr);
	if(tmpstr.IsEmpty())
	{
		CString result;
		CTime t = CTime::GetCurrentTime();
		result.Format(_T("%04d"), t.GetYear());
		INT ids = pDM->MakeRecKey(tmpstr);
		if ( 0 > ids ) return -1;
		tmpquery.Format(
			_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER, MANAGE_CODE) VALUES(%s, 'MO', '등록번호', '%s', '%s', %d, UDF_MANAGE_CODE);"),
//DEL 			_T("INSERT INTO CO_LAST_NUMBER_TBL (REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, LAST_NUMBER) VALUES(%s, 'MO', '등록번호', '%s', '%s', %d);"),
			tmpstr,
			regcode,
			result,
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			_ttoi((regno.Right(10).GetBuffer(0)))
			
			_ttoi((regno.Right(12-nRegCodeLength).GetBuffer(0)))
			//=====================================================
			
		);
	}
	else
		tmpquery.Format(
			_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d, LAST_WORK='%s' WHERE DIVISION_VALUE='%s' AND MANAGE_CODE = UDF_MANAGE_CODE;"),
			regNo,
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			regcode
		);
	
	INT ids = pDM->AddFrame(tmpquery);
	if ( 0 > ids ) return -1;
	return 0;

EFS_END
return -1;

}

INT CRegNo_Creator::MakeQueryAndAddFrame(CESL_DataMgr *pDM, CString regno, INT idx, INT opcode)
{
EFS_BEGIN

	INT ids;
	CString result, tmpstr, key, pkey, skey, dprice, acq_code, drate, price;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	pDM->GetCellData(_T("BB_책KEY"), idx, key);
	if(key.IsEmpty()) return -3000;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return -1;
	}

	if(opcode == 1000)
		tmpstr.Format(
			_T("UPDATE BO_BOOK_TBL SET REG_CODE='%s', REG_NO='%s', REG_DATE='%s', WORKING_STATUS = '%s', DELIVERY_YN = '%s', LAST_WORK = '%s', CHECKIN_DATE='%s', LAST_WORK='%s' WHERE REC_KEY = %s;"), 
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			regno.Left(2),
			regno.Left(nRegCodeLength),
			//=====================================================
			
			regno, 
			result,
			_T("BOA141N"),
			_T("Y"),
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			result,
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			key
		);
	else if(opcode == -1000)
		tmpstr.Format(
			_T("UPDATE BO_BOOK_TBL SET REG_NO=NULL, REG_DATE=NULL, WORKING_STATUS='%s', DELIVERY_YN='N', LAST_WORK='%s', CHECKIN_DATE=NULL WHERE REC_KEY = %s;"), 
			_T("BOA113N"),
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			key
		);
	else if(opcode == 2000)
		tmpstr.Format(
			_T("UPDATE BO_BOOK_TBL SET REG_CODE='%s', REG_NO='%s', REG_DATE='%s', LAST_WORK = '%s' WHERE REC_KEY = %s;"), 
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			regno.Left(2),
			//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
			regno.Left(nRegCodeLength),
			//=====================================================
			
			regno, 
			result,
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			key
		);
	else if(opcode == -2000)
		tmpstr.Format(
			_T("UPDATE BO_BOOK_TBL SET REG_NO=NULL, REG_DATE=NULL, LAST_WORK='%s' WHERE REC_KEY = %s;"), 
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			key
		);

	ids = pDM->AddFrame(tmpstr);
	if ( 0 > ids ) return -1;
	pDM->GetCellData(_T("BB_수입구분"), idx, acq_code);
	pDM->GetCellData(_T("BB_수서정보KEY"), idx, pkey);
	pDM->GetCellData(_T("BB_할인가"), idx, dprice);
	pDM->GetCellData(_T("BP_할인율"), idx, drate);
	pDM->GetCellData(_T("BB_가격"), idx, price);
	if(dprice.IsEmpty())
	{
		//DOUBLE nprice = atof(price.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		DOUBLE nprice = (DOUBLE)_tcstod(price.GetBuffer(0), &pEndPtr);
		//DOUBLE nrate = atof(drate.GetBuffer(0));
		DOUBLE nrate = (DOUBLE)_tcstod(drate.GetBuffer(0), &pEndPtr);

		DOUBLE ndprice = nprice - (nprice*(nrate/100));
		dprice.Format(_T("%10.2f"), ndprice);
	}
	if(pkey.IsEmpty()) return -3001;
	//수입구분별로 작업을 해야한다.
	if(opcode == 1000)
		tmpstr.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET CHECKIN_DATE='%s', DELIVERY_BOOK_CNT=DELIVERY_BOOK_CNT+%s, DELIVERY_PRICE=DELIVERY_PRICE+%s, LAST_WORK='%s' WHERE REC_KEY = %s;"),
			result,
			_T("1"),
			dprice,
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			pkey
		);
	else if(opcode == -1000)
		tmpstr.Format(
			_T("UPDATE BO_PURCHASEINFO_TBL SET DELIVERY_BOOK_CNT=DELIVERY_BOOK_CNT-%s, DELIVERY_PRICE=DELIVERY_PRICE-%s, LAST_WORK='%s' WHERE REC_KEY = %s;"),
			_T("1"),
			dprice,
			m_pParentMgr->GetWorkLogMsg(_T("등록번호부여"),WORK_LOG),
			pkey
		);

	ids = pDM->AddFrame(tmpstr);
	if ( 0 > ids ) return -1;
	
	pDM->GetCellData(_T("IBS_종KEY"), idx, skey);
	if(skey.IsEmpty()) return -3002;
	CString strMarc;
	tmpstr.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), skey);
	pDM->GetOneValueQuery(tmpstr, strMarc);
	CMarc marc;
	m_pParentMgr->m_pInfo->pMarcMgr->Decoding(strMarc, &marc);	
	if(opcode == 1000 || opcode == 2000 )
		m_pParentMgr->m_pInfo->pMarcMgr->SetItem(&marc, _T("049$l"), regno);
	else if(opcode == -1000 || opcode == -2000 )
	{
		m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(&marc, _T("049$l"), regno);
		m_pParentMgr->m_pInfo->pMarcMgr->GetField(&marc, _T("049"), tmpstr);
		if(tmpstr.IsEmpty())
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
	}
	m_pParentMgr->m_pInfo->pMarcMgr->Encoding(&marc, strMarc);
	tmpstr.Format(_T("UPDATE BO_SPECIES_TBL SET MARC='%s' WHERE REC_KEY=%s;"),
		strMarc,
		skey
	);
	ids = pDM->AddFrame(tmpstr);
	if ( 0 > ids ) return -1;
	return 0;

EFS_END
return -1;

}

BOOL CRegNo_Creator::IsDuplcateRegNo(CESL_DataMgr * pDM, CString regno)
{
EFS_BEGIN

	CString tmpquery, tmpstr;
	tmpquery.Format(_T("SELECT REC_KEY FROM BO_BOOK_TBL WHERE REG_NO = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), regno);
	pDM->GetOneValueQuery(tmpquery, tmpstr);
	if(tmpstr.GetLength() == 0)
		return FALSE;
	return TRUE;

EFS_END
return FALSE;

}

INT CRegNo_Creator::IsValidRegNo(CString regno)
{
EFS_BEGIN

	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	CMakeSearchData checker(m_pParentMgr);
	if(regno.GetLength() != 12) return -1000;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	CString tmpstr = regno.Right(10);
	
	CString tmpstr = regno.Right(12-nRegCodeLength);
	//=====================================================
	
	if(!checker.CheckDigit(tmpstr)) return -1001;
	return 0;

EFS_END
return -1;

}

INT CRegNo_Creator::CheckRegNoType(INT lastRegno, CString strRegno, CStringList &regnoList, INT opcode)
{
EFS_BEGIN

	if(opcode == -1000 || opcode == -2000) return 4000;
	CString regcode, tmpstr;
	INT regno, div;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================

	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	regcode = strRegno.Left(2);
// 	tmpstr = strRegno.Right(10);
	
	regcode = strRegno.Left(nRegCodeLength);
	tmpstr = strRegno.Right(12-nRegCodeLength);
	//=====================================================
	
	regno = _ttoi(tmpstr.GetBuffer(0));
	div = regno - lastRegno;
	if(div <= 0) return 1000;
	else if(div == 1) return 2000;
	else return 3000;
	/*
	for(INT i=1 ; i<div ; i++)
	{
		tmpstr.Format(_T("%010d"), lastRegno + i);
		regnoList.AddTail(tmpstr);
	}
	return 3000;
	*/

EFS_END
return -1;

}
