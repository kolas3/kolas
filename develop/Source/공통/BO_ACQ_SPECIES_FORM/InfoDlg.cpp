// InfoDlg.cpp: implementation of the CInfoDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InfoDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInfoDlg::CInfoDlg(CESL_Mgr * pParentMgr, UINT IDD):CESL_Mgr(IDD, pParentMgr)
{
	m_nDialogID = IDD;
	m_bCreate = FALSE;
	m_nAcqType = ACQ_TYPE_INVALID;
	m_nOpType = OPERATION_TYPE_INVALID;
	m_nSpeciesType = SPECIES_TYPE_INVALID;
	m_strWorkingStatus = _T("");
	m_pDM = NULL;
	m_bDonatorInfo = FALSE;
	m_pFormApi = NULL;
	m_pSpeciesApi = NULL;
	m_pDM_Book = NULL;
	m_pGroupCollection = NULL;
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_bInit = FALSE;
	m_bApplyPriceInfo = TRUE;
	m_bApplyVolPriceFromSpeciesPrice = TRUE;

	// KOL.UDF.022 시큐어코딩 보완
	m_pDM_Appendix = NULL;
}

CInfoDlg::~CInfoDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
}

BOOL CInfoDlg::CreateInfoDlg(CWnd *pParentWnd)
{
	m_bCreate = TRUE;
	// 2005-01-13
	// 가격 자릿수 설정값을 가져온다.
	m_strPosNumber.Empty();
	GetManageValue(_T("수서"), _T("단행"), _T("가격절사단위설정"), _T(""), m_strPosNumber);
	return CDialog::Create(m_nDialogID, pParentWnd);
}

INT CInfoDlg::SetSingleComboBoxStrData(CString strCMAlias, CString strInitVal, CStringArray * parrGroupNo, CString strCtrlAlias)
{
	CESL_ControlMgr *pCM = FindCM(strCMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(strCtrlAlias);
	pCMB->ResetContent();
	
	INT cnt = parrGroupNo->GetSize();
	CString strTmpData;
	INT nInitVal = 0;
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = parrGroupNo->GetAt(i);
		if(strInitVal == strTmpData) nInitVal = i;
		pCMB->AddString(strTmpData);
	}
	pCMB->SetCurSel(nInitVal);
	return 0;
}

INT CInfoDlg::SetMultiComboBoxStrData(CString strCMAlias, CString strInitVal, CString strData[][2], CString strCtrlAlias, INT nCount)
{
	m_pComboMgr->SetMulTiComboBoxStrData(strCMAlias, strInitVal, strData, strCtrlAlias, nCount);
	return 0;
}

INT CInfoDlg::GetSingleComboBoxSelectData(CString strCMAlias, CString strCtrlAlias, CStringArray * parrGroupNo, CString &strSelData)
{
	CESL_ControlMgr *pCM = FindCM(strCMAlias);
	CESL_ControlComboBox *pCMB = (CESL_ControlComboBox *)pCM->FindControl(strCtrlAlias);
	INT nCurSel = pCMB->GetCurSel();
	if(nCurSel >= 0) strSelData = parrGroupNo->GetAt(nCurSel);
	return 0;
}

CString CInfoDlg::GetTodayYear(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d"), t.GetYear());
	return result;
}

INT CInfoDlg::GetVolPriceSum(CString &strVolPriceSum)
{
	INT nRowCnt = GetBookDM()->GetRowCount();
	CString strVolSortNo, strTmpData, strData, strPrice ;
	// 2004.09.14
	DOUBLE nVolPriceSum = 0;
	GetBookDM()->SORT(_T("BB_권일련번호"));
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if( GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i) == _T("D") )	continue;
		
		strTmpData = GetBookDM()->GetCellData(_T("BB_권일련번호"), i);
		strPrice	= GetBookDM()->GetCellData(_T("BB_가격"), i);
		if( strTmpData != strVolSortNo )
		{
			strVolSortNo = strTmpData;
			// 2004.09.14
			//nVolPriceSum += _ttoi(strTmpData.GetBuffer(0));
			//nVolPriceSum += (DOUBLE)atof(strPrice.GetBuffer(0));

			TCHAR* pEndPtr = NULL;
			nVolPriceSum += _tcstod(strPrice.GetBuffer(0), &pEndPtr);
		}
	}
	// 2004.12.03
	//strVolPriceSum.Format(_T("%.2f"), nVolPriceSum);
	strVolPriceSum = TruncPrice(GetBookDM(), nVolPriceSum);
	
	return 0;
}

INT CInfoDlg::GetBookPriceSum(CString &strBookPriceSum)
{
	INT nRowCnt = GetBookDM()->GetRowCount();
//	INT nBookPriceSum = 0;  // 2004.09.14

	DOUBLE nBookPriceSum, nBookPrice;
	nBookPriceSum = 0;
	
	CString strTmpData;
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		if(GetBookDM()->GetCellData(_T("BB_DB_FLAG"), i) == _T("D")) continue;
		strTmpData = GetBookDM()->GetCellData(_T("BB_가격"), i);
		// 2004.09.14
		//nBookPriceSum += _ttoi(strTmpData.GetBuffer(0));
		//nBookPrice = (DOUBLE)atof(strTmpData.GetBuffer(0));
		TCHAR* pEndPtr = NULL;
		nBookPrice = _tcstod(strTmpData.GetBuffer(0), &pEndPtr);

		nBookPriceSum += nBookPrice;

		// 2006.03.31 KBS - UDF_단가를 대표책의 가격으로 저장한다. (국방대)
		if( _T("Y") == GetBookDM()->GetCellData(_T("BB_대표책여부"), i) )
		{
			GetDM()->SetCellData(_T("UDF_단가"), GetBookDM()->GetCellData(_T("BB_가격"), i), 0);
		}
	}
	// strBookPriceSum.Format(_T("%.6f"), nBookPriceSum);  // 2005-01-13
	strBookPriceSum.Format(_T("%.2f"), nBookPriceSum); 
	return 0;
}

BOOL CInfoDlg::GetDonatorInfo()
{
	CString strValue;
	GetManageValue(_T("수서"), _T("공통"), _T("기증자정보필수입력사항여부"), _T(""), strValue);
	if (strValue == _T("Y"))
		m_bDonatorInfo = TRUE;
	else 
		m_bDonatorInfo = FALSE;

	return m_bDonatorInfo;
}

INT CInfoDlg::SetBookOrderAndSerial(CESL_DataMgr *pBookDM, CMapStringToString *pMapBookOrderAndSerial)
{
	if(pBookDM == NULL || pMapBookOrderAndSerial == NULL) return -1;

	CString strBookSerial, strTmpData;
	INT nTmpData, nArrCnt, nBookSerial;
	CArray<INT, INT> ArrBookSerial;
	INT nRowCnt = pBookDM->GetRowCount();
	for(INT i=0; i<nRowCnt ; i++)
	{
		strTmpData = pBookDM->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D") ||strTmpData == _T("I")) continue;
		strTmpData = pBookDM->GetCellData(_T("BB_책KEY"), i);
		if(strTmpData.IsEmpty()) continue;

		strBookSerial = pBookDM->GetCellData(_T("BB_일련번호"), i);
		nTmpData = _ttoi(strBookSerial.GetBuffer(0));
		if(ArrBookSerial.GetSize() == 0) ArrBookSerial.Add(nTmpData);
		else
		{
			nArrCnt = ArrBookSerial.GetSize();
			for(INT j=0 ; j<nArrCnt ; j++)
			{
				nBookSerial = ArrBookSerial.GetAt(j);
				if(nBookSerial > nTmpData) 
				{
					ArrBookSerial.SetAt(j, nTmpData);
					break;
				}
			}
			if(j == nArrCnt) ArrBookSerial.Add(nTmpData);
		}
	}

	nArrCnt = 0;
	for(i=0; i<nRowCnt ; i++)
	{
		strTmpData = pBookDM->GetCellData(_T("BB_DB_FLAG"), i);
		if(strTmpData == _T("D") ||strTmpData == _T("I")) continue;
		strTmpData = pBookDM->GetCellData(_T("BB_책KEY"), i);
		if(strTmpData.IsEmpty()) continue;

		strTmpData.Format(_T("%d"), ArrBookSerial.GetAt(nArrCnt++));
		pBookDM->SetCellData(_T("BB_일련번호"), strTmpData, i);
		pBookDM->SetCellData(_T("BB_DB_FLAG"), _T("U"), i);
	}
	return 0;
}

INT CInfoDlg::AllControlClear()
{
	return 0;
}

CGroupInfo * CInfoDlg::GetGroupInfo()
{
	return NULL;
}

CString CInfoDlg::TruncPrice(CESL_DataMgr *pDM, DOUBLE dPrice)
{
	CString strTmpData, strTmp, strValue;
	INT nPos = -1;

	// strTmp.Format(_T("%.6f"), dPrice);  2005-01-13
	strTmp.Format(_T("%.2f"), dPrice);
	nPos = strTmp.Find(_T("."));

	/*// 국내서의 경우
	if(m_strCurrencyCode == _T("\\") || m_strCurrencyCode.IsEmpty())
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '국내서가격절사단위설정'"), strValue);
		if(strValue == _T("0"))
			strTmpData = strTmp.Left(nPos);
		else //if(strValue == _T("1"))
			strTmpData = strTmp.Left(nPos-1) + _T("0");
	}
	// 국외서의 경우
	else 
	{
		pDM->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_2 = '수서'AND GROUP_3 = '단행' AND CLASS_ALIAS = '국외서가격절사단위설정'"), strValue);

		if(strValue == _T("0"))  strTmpData = strTmp.Left(nPos);
		else if(strValue == _T("1"))  strTmpData = strTmp.Left(nPos+2);
		else if(strValue == _T("2"))  strTmpData = strTmp.Left(nPos+3);
		else if(strValue == _T("3"))  strTmpData = strTmp.Left(nPos+4);
		else if(strValue == _T("4"))  strTmpData = strTmp.Left(nPos+5);
		else if(strValue == _T("5"))  strTmpData = strTmp.Left(nPos+6);
		else if(strValue == _T("6"))  strTmpData = strTmp.Left(nPos+7);
	}
	*/

	CString strCurrency = GetDM()->GetCellData(_T("BP_화폐구분"), 0);

	if		( m_strPosNumber == _T("1") )	{
		if( strCurrency == _T("\\") )
			strTmpData = strTmp.Left(nPos);
		else
			strTmpData = strTmp.Left(nPos+2);
	}
	else if	( m_strPosNumber == _T("2") )	{
		if( strCurrency == _T("\\") )
			strTmpData = strTmp.Left(nPos);
		else
			strTmpData = strTmp.Left(nPos+3);
	}
	else									strTmpData = strTmp.Left(nPos);
	

	return strTmpData;	
}
