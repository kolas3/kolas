// BO_BookData.cpp: implementation of the CBO_BookData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BO_BookData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBO_BookData::CBO_BookData(CESL_Mgr * pParentMgr) : CBO_BusinessData(pParentMgr)
{

}

CBO_BookData::~CBO_BookData()
{

}

INT CBO_BookData::ApplyDM(CESL_DataMgr * pDM, INT nIdx)
{
	if(m_pDM == NULL || pDM == NULL) return -1;
	if(nIdx >= pDM->GetRowCount()) return -1;
	
	if(nIdx < 0)
	{
		pDM->InsertRow(-1);
		nIdx = pDM->GetRowCount() - 1;
	}
	
	INT ids ;
	
	INT nColCnt = pDM->GetRefColumnCount();
	INT nPos;
	CString FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE, strTmpData;
	for(INT i=0 ; i<nColCnt ; i++)
	{
		pDM->GetColumnRefInfo(i, FIELD_ALIAS, FIELD_NAME, FIELD_TYPE, INIT_VALUE, DATA_TYPE);
		strTmpData = FIELD_ALIAS.Left(2);
		nPos = FIELD_NAME.Find(_T("."), 0);
		if(nPos >= 0) FIELD_NAME = FIELD_NAME.Mid(nPos+1);
		if(strTmpData == _T("BB")) 
		{
			ids = ApplyDMOneCell(pDM, nIdx, i, FIELD_NAME);
			if(ids < 0) return -1;
		}
	}
	return 0;		
}

INT CBO_BookData::ApplyMarcToData(CMarc * pMarc)
{
	if(pMarc == NULL || m_pParentMgr == NULL || m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	if(m_ArrDataAlias.GetSize() == 0) return 0;

	CString strData1, strData2, strData3;
	INT ids;
	// 일반적인 데이타 처리
	for (INT i = 0; i < m_ArrDataAlias.GetSize(); i++) 
	{
		strData1 = _T("");
		ids = m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, m_ArrMarcAlias.GetAt(i), strData1);
		if(ids < 0) return -1;
		m_pDM->SetCellData(m_ArrDataAlias.GetAt(i), strData1, 0);
	}

	//260$c에 데이터가 없으나 008 7-10에 데이터가 있으면 해당 데이터를 색인 발행년에 적용시킨다.
	strData1 = m_pDM->GetCellData(_T("BB_발행년"), 0);
	if(strData1.IsEmpty())
	{
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("008@7-10"), strData1);
		m_pDM->SetCellData(_T("BB_발행년"), strData1, 0);
	}
	
	// 편권차(마크->권 적용시는 권이 1개인 경우만 대표책에 적용해준다.)
/*
	if (nVolumes == 1) 
	{
		strData1 = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$n"), strData1);
		m_pDM->SetCellData(_T("BB_편권차"), strData1, 0);
	}
	// 편제(권서명)
	if (nVolumes == 1) 
	{
		strData1 = _T("");
		m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("245$p"), strData1);
		m_pDM->SetCellData(_T("BB_편제명부록명"), strData1, 0);
	}


	// 가격 사항
	CString strPrice;
	strData1 = _T("");
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$b"), strData1);
	if (strData1.GetLength() > 0)
	{
		ids = GetDigitData(strData1, strPrice);
		if(ids < 0) return -1;

		if(nVolumes == 1)
			m_pDM->SetCellData(_T("BB_가격"), strData2, 0);
	}	
	strData1.Empty();
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$a"), strData1);
	if(strData1.GetLength() > 0)
	{
		if(nVolumes == 1)
			m_pDM->SetCellData(_T("BB_가격성격"), strData1, 0);
	}
	strData1.Empty();
	m_pParentMgr->m_pInfo->pMarcMgr->GetItem(pMarc, _T("950$c"), strData1);
	if(strData1.GetLength() > 0)
	{
		if(nVolumes == 1)
			m_pDM->SetCellData(_T("BB_가격잡정보"), strData1, 0);
	}

	// 저작자
	strData1 = _T("");
	ids = Get_AUTHOR(pMarc, strData1);
	if(ids < 0) return -1;
	m_pDM->SetCellData(_T("BB_저작자"), strData1, 0);

	// 특수한 데이타 처리 - 020(낱권)		// 1개일 경우는 IDX와 대표BOOK로 1개 이상일경우는 무시
	CString strMarcData1;
	INT nPos;
	if (nVolumes == 1) 
	{
		FindItemByIndicator(pMarc, _T("020$a"), _T("  "), strMarcData1);
		//isbn에 893726387382(편권차)이렇게 들어간다. 그래서 '()'안의 문자는 빠져야한다.
		nPos = strMarcData1.Find(_T("("), 0);
		if(nPos >= 0) strMarcData1 = strMarcData1.Mid(0, nPos);
		m_pDM->SetCellData(_T("BB_낱권ISBN"), strMarcData1, 0);

		FindItemByIndicator(pMarc, _T("020$g"), _T("  "), strMarcData1);
		//파일반입시 부가기호 에러수정 - 부가기호는 TCHAR(5)이다 따라서 이 이상일 경우 Error메시지를 출력해야한다.
		if(strMarcData1.GetLength() > 5) 
		{
			strData1.Format(_T("낱권ISBN 부가기호[%s]가 오류가 있습니다. 해당 정보는 저장되지 않습니다."), strMarcData1);
			m_pParentMgr->ESLAfxMessageBox(strData1);
			strMarcData1.Empty();
		}
		((CIndexTool*)m_pParentMgr->m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strMarcData1);
		m_pDM->SetCellData(_T("BB_낱권ISBN부가기호"), strMarcData1, 0);

		FindItemByIndicator(pMarc, _T("020$z"), _T("  "), strMarcData1);
		m_pDM->SetCellData(_T("BB_낱권오류ISBN"), strMarcData1, 0);
	}
*/
	return 0;
}

INT CBO_BookData::ApplyDataToMarc(CMarc * pMarc)
{
	if(pMarc == NULL || m_pParentMgr == NULL || m_pDM == NULL) return -1;
	if(m_pDM->GetRowCount() == 0) return 0;
	if(m_ArrDataAlias.GetSize() == 0) return 0;
	
	CString strData1, strData2, strData3;

	// 일반적인 데이타 처리
	for (INT i = 0; i < m_ArrDataAlias.GetSize(); i++) {
		strData1.Empty();
		m_pDM->GetCellData(m_ArrDataAlias.GetAt(i), 0, strData1);
		if (strData1.GetLength() > 0) 
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, m_ArrMarcAlias.GetAt(i), strData1, _T(""), MODIFY_ITEM);
		else 
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, m_ArrMarcAlias.GetAt(i));
	}
	
/*
	// 편제(권서명)
	if (pDM_VOLUME->GetRowCount() == 1) 
	{
		strData1 = _T("");
		m_pDM->GetCellData(_T("BB_편제명부록명"), 0, strData1);
		if (strData1.GetLength() > 0) 
			m_pParentMgr->m_pInfo->pMarcMgr->SetItem(pMarc, _T("245$p"), strData1);
		else 
			m_pParentMgr->m_pInfo->pMarcMgr->DeleteItem(pMarc, _T("245$p"));
	}
	
	SetISBN_to_Marc_ByVolume(pMarc, pDM_VOLUME, _T("020"), _T("  "));		// IBS_낱권_ISBN is 020 blank blank
	*/
	
	return 0;
}

INT CBO_BookData::InitBusinessData(CString strConnectionInfo, CString strDMAlias/*=_T("")*/)
{
	if(strConnectionInfo.IsEmpty()) strConnectionInfo = m_pParentMgr->CONNECTION_INFO;
	if(!strDMAlias.IsEmpty()) m_strDMAlias = strDMAlias;
	
	if(strConnectionInfo.IsEmpty() || m_strDMAlias.IsEmpty()) return -1;
	
	INT ids = InitDataManager(strConnectionInfo, m_strDMAlias);
	if(ids < 0) return -1;
	
	m_ArrDataAlias.RemoveAll();	m_ArrMarcAlias.RemoveAll();
	m_ArrDataAlias.Add(_T("BB_발행년"));		m_ArrDataAlias.Add(_T("IBS_딸림자료"));
	m_ArrDataAlias.Add(_T("BB_면장수"));		m_ArrDataAlias.Add(_T("BB_물리적특성"));	
	m_ArrDataAlias.Add(_T("BB_크기"));			
	m_ArrMarcAlias.Add(_T("260$c"));			m_ArrMarcAlias.Add(_T("300$e"));
	m_ArrMarcAlias.Add(_T("300$a"));			m_ArrMarcAlias.Add(_T("300$b"));
	m_ArrMarcAlias.Add(_T("300$c"));			
	
	return 0;
}
