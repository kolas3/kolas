// BO_REG_NO_CHANGER.cpp : implementation file
//

#include "stdafx.h"
// #include "	\ add additional includes here"
// #include "BO_REG_NO_CHANGER.h"
#include "BO_REG_NO_CHANGER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_NO_CHANGER dialog


CBO_REG_NO_CHANGER::CBO_REG_NO_CHANGER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_REG_NO_CHANGER::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_REG_NO_CHANGER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentDM = NULL;
	m_pParentGrid = NULL;

	m_pRegNoChangerDM = NULL;
	m_pRegNoChangerCM = NULL;
	m_pRegNoChangerGrid = NULL;
}
CBO_REG_NO_CHANGER::~CBO_REG_NO_CHANGER()
{
}

void CBO_REG_NO_CHANGER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_REG_NO_CHANGER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_REG_NO_CHANGER, CDialog)
	//{{AFX_MSG_MAP(CBO_REG_NO_CHANGER)
	ON_BN_CLICKED(IDC_tbnStart1, OntbnStart1)
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_edtCulBoRegCodeLength, OnKillfocusedtCulBoRegCodeLength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_NO_CHANGER message handlers

void CBO_REG_NO_CHANGER::OntbnStart1() 
{
	INT nSelectCnt;
	INT nSelectIdx;
	//===================================================
	//2009.10.12 ADD BY PJW : 등록번호 변경사항을 메세지로 보여준다.
	INT nNotRegCode = 0;
	INT nErrorRegNo = 0;
	INT nDupRegNo = 0;
	CString strMsg;
	//===================================================
	CString strChangeRegNo;
	CString strChangeRegCode;
	CString strChangeRegNum;
	CString strSpeciesKey;
	CString strQuery;
	CString strQueryData;
	nSelectCnt = 0;
	m_pRegNoChangerGrid->SelectMakeList();
	nSelectCnt = m_pRegNoChangerGrid->SelectGetCount();

	nSelectIdx = m_pRegNoChangerGrid->SelectGetHeadPosition();
	for ( INT i = 0 ; i < nSelectCnt ;i++ )
	{
 		strChangeRegNo	= m_pRegNoChangerGrid->GetTextMatrix( nSelectIdx+1, 4 );
		strChangeRegNo.MakeUpper();
 		m_pRegNoChangerDM->GetCellData(_T("BB_종KEY"),nSelectIdx,strSpeciesKey);
 		if( strChangeRegNo.GetLength() > m_nRegCodeLength && strChangeRegNo.GetLength() <= 12 )
 		{
 			strChangeRegCode	= strChangeRegNo.Left( m_nRegCodeLength );
 			strChangeRegNum	= strChangeRegNo.Mid( m_nRegCodeLength );
 			strChangeRegNo	= m_pInfo->MakeRegNo( strChangeRegCode, strChangeRegNum );
 			strChangeRegNo.MakeUpper();
 
 			if( strChangeRegNo == _T("ERROR"))
 			{
 				m_pRegNoChangerGrid->SetTextMatrix( nSelectIdx+1, 1, _T("") );
 				m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nSelectIdx );
 				nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();	
				//===================================================
				//2009.10.12 ADD BY PJW : 등록번호 변경사항을 메세지로 보여준다.
				++nErrorRegNo;
				//===================================================
 				continue;
 			}
 
 			strQuery.Format(_T("SELECT COUNT(1) FROM CD_CODE_TBL WHERE CLASS = 4 AND CODE = '%s'"), strChangeRegCode );
 			m_pRegNoChangerDM->GetOneValueQuery(strQuery,strQueryData);
 
 			if( _ttoi(strQueryData) < 1 )
 			{
 				m_pRegNoChangerGrid->SetTextMatrix( nSelectIdx+1, 1, _T("") );
 				m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nSelectIdx );
 				nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();	
				//===================================================
				//2009.10.12 ADD BY PJW : 등록번호 변경사항을 메세지로 보여준다.
				++nNotRegCode;
				//===================================================				
 				continue;
 			}
 
 			strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE REG_NO = '%s' AND REG_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strChangeRegNo, strChangeRegCode );
 			m_pRegNoChangerDM->GetOneValueQuery(strQuery,strQueryData);
 			
 			if( _ttoi(strQueryData) > 0)
 			{
 				m_pRegNoChangerGrid->SetTextMatrix( nSelectIdx+1, 1, _T("") );
 				m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nSelectIdx );
 				nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();	
				//===================================================
				//2009.10.12 ADD BY PJW : 등록번호 변경사항을 메세지로 보여준다.
				++nDupRegNo;
				//===================================================				
 				continue;
 			}
 			else
 			{
 				m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), strChangeRegNo, nSelectIdx );
 			}
 		}
 		else
 		{
 			m_pRegNoChangerGrid->SetTextMatrix( nSelectIdx+1, 1, _T("") );
 			m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nSelectIdx );
			nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();	
			//===================================================
			//2009.10.12 ADD BY PJW : 등록번호 변경사항을 메세지로 보여준다.
			++nErrorRegNo;
			//===================================================
			continue;
 		}
		
		MakeMarc( nSelectIdx );

		nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();	
	}
 	m_pRegNoChangerGrid->SelectRemoveAll();
	m_pRegNoChangerGrid->SelectMakeList();
	nSelectCnt = m_pRegNoChangerGrid->SelectGetCount();

	//===================================================
	//2009.10.05 UPDATE BY PJW : 등록번호 변경 메세지 변경
//	if( 0 < nSelectCnt )
//	{
// 		UpdateRegNo();
//		ESLAfxMessageBox(_T("등록번호 교체를 완료 하였습니다.."));
//	}
//	else
//	{
//		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
//	}
	if( 0 < nSelectCnt )
	{
 		UpdateRegNo();
	}
	strMsg.Format(_T("등록번호중복 : %d\t\r\n등록번호에러 : %d\t\r\n없는등록구분 : %d\t\r\n변경등록번호 : %d"),
					nDupRegNo, nErrorRegNo, nNotRegCode, nSelectCnt	);
	ESLAfxMessageBox(strMsg);
	//===================================================

 	m_pRegNoChangerGrid->SetProtectSelectColumn(TRUE);
 	m_pRegNoChangerGrid->Display();
 	m_pRegNoChangerGrid->SetProtectSelectColumn(FALSE);
	return;

}

INT CBO_REG_NO_CHANGER::UpdateRegNo()
{
	INT nSelectCnt;
  	INT nSelectIdx;
  	INT ids;
	CString strPreRegNo;
  	CString strChangeRegNo;
  	CString strChangeRegCode;
  	CString strQuery;
  	CString strSpeciesKey;
	CString strCompareSpeciesKey;
  	CString strBookKey;
	CString strMarc;
 	CString strIbsRegCode;


	strSpeciesKey = _T("");
	strCompareSpeciesKey = _T("");

	m_pRegNoChangerGrid->SelectMakeList();	
	nSelectCnt = m_pRegNoChangerGrid->SelectGetCount();
	nSelectIdx = m_pRegNoChangerGrid->SelectGetHeadPosition();


  	ids = m_pRegNoChangerDM->StartFrame();
	for( INT i = 0 ; i < nSelectCnt ; i++ ) 
	{
		m_pRegNoChangerDM->GetCellData(_T("UDF_변경등록번호"), nSelectIdx, strChangeRegNo);
		m_pRegNoChangerDM->GetCellData(_T("BS_MARC"), nSelectIdx, strMarc);
		m_pRegNoChangerDM->GetCellData(_T("IBS_색인등록구분"), nSelectIdx, strIbsRegCode);
		m_pRegNoChangerDM->GetCellData(_T("BB_책KEY"), nSelectIdx,strBookKey);
		m_pRegNoChangerDM->GetCellData(_T("BB_종KEY"), nSelectIdx,strSpeciesKey);
		
		strChangeRegCode = strChangeRegNo.Left(m_nRegCodeLength);
 		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_NO = '%s', REG_CODE = '%s' WHERE REC_KEY = %s;"), strChangeRegNo, strChangeRegCode, strBookKey );
  		m_pRegNoChangerDM->AddFrame(strQuery);
		if( strCompareSpeciesKey != strSpeciesKey )
		{
			strCompareSpeciesKey = strSpeciesKey;
			//===================================================
			//2010.02.04 UPDATE BY PJW : DB FUNCTION으로 등록구분코드 셋팅하도록 수정함
//			strQuery.Format(_T("UPDATE IDX_BO_TBL SET REG_CODE = '%s' WHERE REC_KEY = %s;"), strIbsRegCode, strSpeciesKey );
			strQuery.Format(_T("UPDATE IDX_BO_TBL SET REG_CODE = GET_BO_BOOK_INFO(%s, 'REG_CODE') WHERE REC_KEY = %s;"), strSpeciesKey, strSpeciesKey );
			//===================================================
  			m_pRegNoChangerDM->AddFrame(strQuery);			
 			m_pRegNoChangerDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc );
  			ids = m_pRegNoChangerDM->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
			UpdateParent( nSelectIdx );

		}
		nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();
	}
	
  	ids = m_pRegNoChangerDM->SendFrame();
  	if (ids < 0)
  	{
  		AfxMessageBox(_T("DB와의 작업이 실패했습니다."));
  		return -1;
  	}
 	
  	ids = m_pRegNoChangerDM->EndFrame();
  	if (ids < 0)
  	{
  		AfxMessageBox(_T("DB와의 작업이 실패했습니다."));
  		return -1;
  	}

	return 0;
}
// INT CBO_REG_NO_CHANGER::UpdateRegNo()
// {
// 	INT nSelectCnt;
//   	INT nSelectIdx;
//   	INT ids;
// 	CString strPreRegNo;
//   	CString strChangeRegNo;
//   	CString strChangeRegCode;
//   	CString strQuery;
//   	CString strSpeciesKey;
// 	CString strCompareSpeciesKey;
//   	CString strBookKey;
// 	CString strMarc;
//  	CString strIbsRegCode;
// 
// 
// 	strSpeciesKey = _T("");
// 	strCompareSpeciesKey = _T("");
// 
// 	m_pRegNoChangerGrid->SelectMakeList();	
// 	nSelectCnt = m_pRegNoChangerGrid->SelectGetCount();
// 	nSelectIdx = m_pRegNoChangerGrid->SelectGetHeadPosition();
// 
// 
//  	ids = m_pRegNoChangerDM->StartFrame();
// 	for( INT i = 0 ; i < nSelectCnt ; i++ ) 
// 	{
// 		m_pRegNoChangerDM->GetCellData(_T("UDF_변경등록번호"), nSelectIdx, strChangeRegNo);
// 		m_pRegNoChangerDM->GetCellData(_T("BS_MARC"), nSelectIdx, strMarc);
// 		m_pRegNoChangerDM->GetCellData(_T("IBS_색인등록구분"), nSelectIdx, strIbsRegCode);
// 		m_pRegNoChangerDM->GetCellData(_T("BB_책KEY"), nSelectIdx,strBookKey);
// 		m_pRegNoChangerDM->GetCellData(_T("BB_종KEY"), nSelectIdx,strSpeciesKey);
// 		
// 		strChangeRegCode = strChangeRegNo.Left(m_nRegCodeLength);
// 		strQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_NO = '%s', REG_CODE = '%s' WHERE REC_KEY = %s;"), strChangeRegNo, strChangeRegCode, strBookKey );
//  		m_pRegNoChangerDM->AddFrame(strQuery);
// 		if( strCompareSpeciesKey != strSpeciesKey )
// 		{
// 			strCompareSpeciesKey = strSpeciesKey;
// 			strQuery.Format(_T("UPDATE IDX_BO_TBL SET REG_CODE = '%s' WHERE REC_KEY = %s;"), strIbsRegCode, strSpeciesKey );
//  			m_pRegNoChangerDM->AddFrame(strQuery);			
// 			m_pRegNoChangerDM->AddDBFieldData(_T("MARC"), _T("STRING"), strMarc );
//  			ids = m_pRegNoChangerDM->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);
// 			UpdateParent( nSelectIdx );
// 
// 		}
// 		nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();
// 	}
// 	
//  	ids = m_pRegNoChangerDM->SendFrame();
//  	if (ids < 0)
//  	{
//  		AfxMessageBox(_T("DB와의 작업이 실패했습니다."));
//  		return -1;
//  	}
// 	
//  	ids = m_pRegNoChangerDM->EndFrame();
//  	if (ids < 0)
//  	{
//  		AfxMessageBox(_T("DB와의 작업이 실패했습니다."));
//  		return -1;
//  	}
// 
// 	return 0;
// }

INT CBO_REG_NO_CHANGER::UpdateParent(INT nSelectIdx)
{
	INT nParentSelectCnt;
 	INT nParentSelectIdx;
	CString strSpeciesKey;
	CString strParentSpeciesKey;
	CString strChangeRegNo;
	CString strPreRegNo;
	CString strParentRegNo;

	m_pRegNoChangerDM->GetCellData(_T("UDF_변경등록번호"), nSelectIdx, strChangeRegNo);
	m_pRegNoChangerDM->GetCellData(_T("BB_등록번호"), nSelectIdx, strPreRegNo);
	m_pRegNoChangerDM->GetCellData(_T("BB_종KEY"), nSelectIdx, strSpeciesKey);

	m_pParentGrid->SelectMakeList();
	nParentSelectCnt = m_pParentGrid->SelectGetCount();
	nParentSelectIdx = m_pParentGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nParentSelectCnt ; i++ )
	{
		m_pParentDM->GetCellData(_T("IBS_종KEY"), nParentSelectIdx, strParentSpeciesKey);
		if( strParentSpeciesKey == strSpeciesKey )
		{
			m_pParentDM->GetCellData(_T("시작등록번호"), nParentSelectIdx, strParentRegNo);			
			if( strParentRegNo == strPreRegNo )
			{
				m_pParentDM->SetCellData( _T("시작등록번호"), strChangeRegNo, nParentSelectIdx );
			}

			m_pParentDM->GetCellData(_T("끝등록번호"), nParentSelectIdx, strParentRegNo);
			if( strParentRegNo == strPreRegNo )
			{
				m_pParentDM->SetCellData( _T("끝등록번호"), strChangeRegNo, nParentSelectIdx );
			}
			
			break;
		}
		nParentSelectIdx = m_pParentGrid->SelectGetNext();
	}
	
	return 0;
}

INT CBO_REG_NO_CHANGER::MakeMarc(INT nSelectIdx)
{
 	CArray<CString, CString&> arrItem;
  	CMarc Marc;
  	CString strMarc;
  	CString	strMarcRegNo;
 	CString strFieldData;
 	CString strOldFieldData;
 	CString strTemp;
 	CString strChangeRegNo;
 	CString strPreRegNo;
 	CString strSpeciesKey;
 	CString strCompareSpeciesKey;
	CString strIssRegCode;
 
  	INT nDmRowCnt;
 	INT nItemCount;
 	INT nPreRegCodeLength;
 	INT ids;
	
 	nDmRowCnt = m_pRegNoChangerDM->GetRowCount();
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCulBoRegCodeLength);
	pEdit->GetWindowText(strTemp);
	nPreRegCodeLength = _ttoi( strTemp );

	m_pRegNoChangerDM->GetCellData(_T("UDF_변경등록번호"), nSelectIdx, strChangeRegNo);
	m_pRegNoChangerDM->GetCellData(_T("BB_등록번호"), nSelectIdx, strPreRegNo);
	m_pRegNoChangerDM->GetCellData(_T("BS_MARC"), nSelectIdx, strMarc);
	m_pRegNoChangerDM->GetCellData(_T("BB_종KEY"), nSelectIdx, strSpeciesKey);
	m_pRegNoChangerDM->GetCellData(_T("IBS_색인등록구분"), nSelectIdx, strIssRegCode);

	strTemp = strChangeRegNo.Left(m_nRegCodeLength);

	if( strIssRegCode.IsEmpty() )
	{
		strIssRegCode = strTemp;
	}
	else
	{
		if( strIssRegCode.Find(strTemp) < 0 )
		{
			strIssRegCode += _T(" ") + strTemp;
		}
	}

	m_pInfo->pMarcMgr->Decoding(strMarc, &Marc);
  	ids = m_pInfo->pMarcMgr->GetItem(&Marc, _T("049$l"),strMarcRegNo,&arrItem);
 	m_pInfo->pMarcMgr->GetField(&Marc, _T("049"),strOldFieldData);
 	nItemCount = arrItem.GetSize();
	for( INT j = 0 ; j < nItemCount ; j++ )
	{
		CString strTempRegNo;
		CString strTempRegCode;
		CString strTempRegNum;

		strMarcRegNo = arrItem.GetAt(j);
		strTempRegCode = strMarcRegNo.Left(nPreRegCodeLength);
		strTempRegNum = strMarcRegNo.Mid(nPreRegCodeLength);
		strTempRegNo = m_pInfo->MakeRegNo( strTempRegCode, strTempRegNum );
		if( strPreRegNo == strTempRegNo )
		{
			strTempRegCode = strChangeRegNo.Left(m_nRegCodeLength);
			strTempRegNum.Format(_T("%d"),_ttoi(strChangeRegNo.Mid(m_nRegCodeLength)));
			strChangeRegNo = strTempRegCode + strTempRegNum;
			strOldFieldData.Replace(strMarcRegNo,strChangeRegNo);
			strFieldData.Format(_T("049%s"),strOldFieldData);
			ids = m_pInfo->pMarcMgr->DeleteField(&Marc, _T("049"));
 			ids = m_pInfo->pMarcMgr->InsertField(&Marc, strFieldData);
			m_pInfo->pMarcMgr->Encoding( &Marc, strMarc );

			for( INT i = 0 ; i < nDmRowCnt ; i++)
 			{
				m_pRegNoChangerDM->GetCellData(_T("BB_종KEY"), i, strCompareSpeciesKey);
				if( strSpeciesKey == strCompareSpeciesKey )
				{
					m_pRegNoChangerDM->SetCellData(_T("BS_MARC"), strMarc, i);
					m_pRegNoChangerDM->SetCellData(_T("IBS_색인등록구분"), strIssRegCode, i);
				}
 			}
			break;
		}
	}
	return 0;
}

BOOL CBO_REG_NO_CHANGER::OnInitDialog() 
{
	CDialog::OnInitDialog();
 	
 	if (InitESL_Mgr(_T("SM_단행_등록번호재부여")) < 0)
 	{
 		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
 		return FALSE;
 	}	
 	
 	m_pRegNoChangerCM = FindCM(_T("CM_단행_등록번호재부여"));
 	if(m_pRegNoChangerCM == NULL )
 	{
 		ESLAfxMessageBox(_T("FindCM Error"));
 		return FALSE;
 	}
 	m_pRegNoChangerDM = FindDM(_T("DM_단행_등록번호재부여"));
 	if(m_pRegNoChangerCM == NULL )
 	{
 		ESLAfxMessageBox(_T("FindDM Error"));
 		return FALSE;
 	}
 
 	m_pRegNoChangerGrid = (CESL_Grid *)m_pRegNoChangerCM->FindControl(_T("REG_CHANGE_GRID"));
 	if(m_pRegNoChangerGrid == NULL )
 	{
 		ESLAfxMessageBox(_T("FindControl Error"));
 		return FALSE;
 	}
 	
 	if(Init() < 0)
 	{
 		ESLAfxMessageBox(_T("DataInit Error"));
 		return FALSE;
 	}
 
 	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCulBoRegCodeLength);
 	pEdit->SetLimitText(1);
 	CString strTemp;
 	strTemp.Format(_T("%d"), m_nRegCodeLength);
 	pEdit->SetWindowText(strTemp);

	
 	m_pRegNoChangerGrid->m_bGridEdit = TRUE;
 	m_pRegNoChangerGrid->SELEDITOR_RemoveAllColumns();
	m_nEditCol = GetGridColumnIdx(m_pRegNoChangerGrid, _T("변경등록번호"));
 	m_pRegNoChangerGrid->SELEDITOR_AddColumn(m_nEditCol);
 
 	EnableThemeDialogTexture(GetSafeHwnd());
 	
 	return TRUE;  // return TRUE unless you set the focus to a control
 	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CBO_REG_NO_CHANGER::Init()
{
	INT nSelectCnt;
	INT nSelectIdx;
	INT nKeyCnt;
	CString strSpeciesKey;
	CString strTempData;
	CString strWhereSpecieskey;
	CString strWhere;
	CString strQuery;

	m_nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;

	m_pParentGrid->SelectMakeList();
	nSelectCnt = m_pParentGrid->SelectGetCount();
	nSelectIdx = m_pParentGrid->SelectGetHeadPosition();
	nKeyCnt = 0; 
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		m_pParentDM->GetCellData(_T("IBS_종KEY"),nSelectIdx,strTempData);
		if( strSpeciesKey.IsEmpty() )
		{
			strSpeciesKey = strTempData;
		}
		else
		{
			strSpeciesKey += _T(", ") + strTempData; 
		}
		
		if( nKeyCnt == 1000 )
		{
			strWhereSpecieskey.Format(_T("B.SPECIES_KEY IN (%s) "),strSpeciesKey);
			if( strWhere.IsEmpty() )
			{
				strWhere = strWhereSpecieskey;
			}
			else
			{				
				strWhere += _T("OR ") + strWhereSpecieskey;
			}
			strSpeciesKey.Empty();
		}
		nSelectIdx = m_pParentGrid->SelectGetNext();
	}

	if( !strSpeciesKey.IsEmpty() )
	{
		strWhereSpecieskey.Format(_T("B.SPECIES_KEY IN (%s) "),strSpeciesKey);
		if( strWhere.IsEmpty() )
		{
			strWhere = strWhereSpecieskey;
		}
		else
		{				
			strWhere += _T("OR ") + strWhereSpecieskey;
		}
	}
	if( 0 > strWhere.Find( _T("OR ") ) )
	{
		strWhere = _T("S.REC_KEY = I.REC_KEY AND B.SPECIES_KEY = I.REC_KEY AND B.REG_NO IS NOT NULL AND ") + strWhere;
	}
	else
	{
		strWhere = _T("S.REC_KEY = I.REC_KEY AND B.SPECIES_KEY = I.REC_KEY AND B.REG_NO IS NOT NULL AND ( ") + strWhere + _T(" )");
	}

	m_pRegNoChangerDM->RefreshDataManager(strWhere);
	m_pRegNoChangerGrid->Display();
	
	return 0 ;
}

VOID CBO_REG_NO_CHANGER::OnOK() 
{
	CDialog::OnOK();
}

void CBO_REG_NO_CHANGER::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CButton *pBtn = (CButton*)GetDlgItem(IDC_tbnStart1);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow(cx - 90, 11, 80, 25);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridBO_REG_NO_CHANGER);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(5, 45, cx - 10, cy- 50);
}

void CBO_REG_NO_CHANGER::OnKillfocusedtCulBoRegCodeLength() 
{
	CString strRegCodeLength;
	INT nRegCodeLength;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCulBoRegCodeLength);

	pEdit->GetWindowText(strRegCodeLength);
	nRegCodeLength = _ttoi(strRegCodeLength);

	if( 0 > nRegCodeLength || 6 < nRegCodeLength )
	{
		ESLAfxMessageBox(_T("등록구분 자릿수 설정이 잘못 되었습니다."));
		strRegCodeLength.Format(_T("%d"), m_nRegCodeLength);
		pEdit->SetWindowText(strRegCodeLength);
	}
	
}

BEGIN_EVENTSINK_MAP(CBO_REG_NO_CHANGER, CDialog)
    //{{AFX_EVENTSINK_MAP(CBO_REG_NO_CHANGER)
	ON_EVENT(CBO_REG_NO_CHANGER, IDC_gridBO_REG_NO_CHANGER, -601 /* DblClick */, OnDblClickgridBOREGNOCHANGER, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
// 
// void CBO_REG_NO_CHANGER::OnClickgridBOREGNOCHANGER() 
// {
// 	// TODO: Add your control notification handler code here
// 	
// }

// BOOL CBO_REG_NO_CHANGER::PreTranslateMessage(MSG* pMsg) 
// {
// // 	if (ESL_GRID_MODIFY == pMsg->message)
// // 	{
// // 		INT nRowIdx = this->m_pRegNoChangerGrid->GetRow();
// // 		INT nColIdx = this->m_pRegNoChangerGrid->GetCol();
// // 		// 납풉책수 컬럼이 수정되었다면.
// // 		if (nRowIdx > 0 && nColIdx == m_nEditCol+1) 
// // 		{	
// //  			ReRegNoCheck();	
// // 		}
// // 		return TRUE;
// // 	}
// // 
// // 	if ( pMsg->message == WM_KEYDOWN ) 
// // 	{
// // 		if(pMsg->wParam == VK_RETURN) 
// // 		{
// // 			return TRUE;
// // 		}
// // 	}
// 
// 	return CDialog::PreTranslateMessage(pMsg);
// }


INT CBO_REG_NO_CHANGER::GetGridColumnIdx(CESL_Grid * pGrid, CString strColHeader)
{
	INT colCnt = pGrid->GetColCount();
	INT width, alignment, nPos;
	CString tmpstr;
	for(INT i=0 ; i<colCnt ; i++)
	{
		pGrid->GetColumnInfo(i, tmpstr, width, alignment);
		nPos = tmpstr.Find(strColHeader);
		if(nPos >= 0) return i;
	}
	return -1;
}

INT CBO_REG_NO_CHANGER::ReRegNoCheck()
{	
	INT nCurRow;
 	INT nCurCul;
	nCurRow = m_pRegNoChangerGrid->GetRow();
	nCurCul = m_pRegNoChangerGrid->GetCol();

	CString strChangeRegNo;
	CString strSpeciesKey;

 	CString strChangeRegCode;
 	CString strChangeRegNum;
 	CString strQuery;
	CString strQueryData;

	m_pRegNoChangerGrid->Display();
	strChangeRegNo	= m_pRegNoChangerGrid->GetTextMatrix( nCurRow, nCurCul );
	m_pRegNoChangerDM->GetCellData(_T("BB_종KEY"),nCurRow-1,strSpeciesKey);
	if( strChangeRegNo.GetLength() > m_nRegCodeLength && strChangeRegNo.GetLength() <= 12 )
	{
		strChangeRegCode	= strChangeRegNo.Left( m_nRegCodeLength );
		strChangeRegNum	= strChangeRegNo.Mid( m_nRegCodeLength );
		strChangeRegNo	= m_pInfo->MakeRegNo( strChangeRegCode, strChangeRegNum );
		strChangeRegNo.MakeUpper();

		if( strChangeRegNo == _T("ERROR"))
		{
			m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nCurRow-1 );
			ESLAfxMessageBox(_T("등록번호가 잘 못 되었습니다."));
			m_pRegNoChangerGrid->SetTextMatrix( nCurRow, nCurCul, _T("") );
			m_pRegNoChangerGrid->SetFocus();
			return 0;
		}

		strQuery.Format(_T("SELECT COUNT(1) FROM CD_CODE_TBL WHERE CLASS = 4 AND CODE = '%s'"), strChangeRegCode );
		m_pRegNoChangerDM->GetOneValueQuery(strQuery,strQueryData);

		if( _ttoi(strQueryData) < 1 )
		{
			m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nCurRow-1 );
			ESLAfxMessageBox(_T("없는 등록구분 입니다."));
			m_pRegNoChangerGrid->SetTextMatrix( nCurRow, nCurCul, _T("") );
			m_pRegNoChangerGrid->SetFocus();
			return 0;

		}

		strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE REG_NO = '%s' AND REG_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strChangeRegNo, strChangeRegCode );
		m_pRegNoChangerDM->GetOneValueQuery(strQuery,strQueryData);
		
		if( _ttoi(strQueryData) > 0)
		{
			m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nCurRow-1 );
			ESLAfxMessageBox(_T("등록번호가 중복됩니다."));
			m_pRegNoChangerGrid->SetTextMatrix( nCurRow, nCurCul, _T("") );
			m_pRegNoChangerGrid->SetFocus();
			return 0;
		}
		else
		{
			m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), strChangeRegNo, nCurRow-1 );
			m_pRegNoChangerGrid->SetTextMatrix( nCurRow, nCurCul, strChangeRegNo );
			m_pRegNoChangerGrid->SetFocus();
			return 0;
		}
	}
	else
	{
		m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nCurRow-1 );
		ESLAfxMessageBox(_T("등록번호가 잘 못 되었습니다."));
		m_pRegNoChangerGrid->SetTextMatrix( nCurRow, nCurCul, _T("") );
		m_pRegNoChangerGrid->SetFocus();
	}
	return 0;
}

void CBO_REG_NO_CHANGER::OnDblClickgridBOREGNOCHANGER() 
{
	CString strChangeRegNo;

	INT nCurRow;
 	INT nCurCul;
	nCurRow = m_pRegNoChangerGrid->GetRow();
	nCurCul = m_pRegNoChangerGrid->GetCol();

// 	if(nCurRow == 0 ) return ;
// 	if(nCurCul == m_nEditCol+1) return ;	

	strChangeRegNo	= m_pRegNoChangerGrid->GetTextMatrix( nCurRow, nCurCul );
	m_pRegNoChangerDM->SetCellData(_T("UDF_변경등록번호"), strChangeRegNo, nCurRow - 1);
}
