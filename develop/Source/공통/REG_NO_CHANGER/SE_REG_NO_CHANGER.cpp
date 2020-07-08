// SE_REG_NO_CHANGER.cpp : implementation file
//

#include "stdafx.h"
// #include "	\ add additional includes here"
#include "SE_REG_NO_CHANGER.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_REG_NO_CHANGER dialog


CSE_REG_NO_CHANGER::CSE_REG_NO_CHANGER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSE_REG_NO_CHANGER::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_REG_NO_CHANGER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentDM = NULL;
	m_pParentGrid = NULL;

	m_pRegNoChangerDM = NULL;
	m_pRegNoChangerCM = NULL;
	m_pRegNoChangerGrid = NULL;
}

CSE_REG_NO_CHANGER::~CSE_REG_NO_CHANGER()
{
}


void CSE_REG_NO_CHANGER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_REG_NO_CHANGER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_REG_NO_CHANGER, CDialog)
	//{{AFX_MSG_MAP(CSE_REG_NO_CHANGER)
	ON_BN_CLICKED(IDC_tbnStart2, OntbnStart2)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_EN_KILLFOCUS(IDC_edtCulSeRegCodeLength, OnKillfocusedtCulSeRegCodeLength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_REG_NO_CHANGER message handlers

void CSE_REG_NO_CHANGER::OntbnStart2() 
{
	INT nSelectCnt;
	INT nSelectIdx;
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
		m_pRegNoChangerDM->GetCellData(_T("SB_종KEY"),nSelectIdx,strSpeciesKey);
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
				continue;
			}

			strQuery.Format(_T("SELECT COUNT(1) FROM CD_CODE_TBL WHERE CLASS = 4 AND CODE = '%s'"), strChangeRegCode );
			m_pRegNoChangerDM->GetOneValueQuery(strQuery,strQueryData);

			if( _ttoi(strQueryData) < 1 )
			{
				m_pRegNoChangerGrid->SetTextMatrix( nSelectIdx+1, 1, _T("") );
				m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nSelectIdx );
				nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();	
				continue;
			}

			strQuery.Format(_T("SELECT COUNT(1) FROM SE_BOOK_TBL WHERE REG_NO = '%s' AND REG_CODE = '%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strChangeRegNo, strChangeRegCode );
			m_pRegNoChangerDM->GetOneValueQuery(strQuery,strQueryData);
			
			if( _ttoi(strQueryData) > 0)
			{
				m_pRegNoChangerGrid->SetTextMatrix( nSelectIdx+1, 1, _T("") );
				m_pRegNoChangerDM->SetCellData( _T("UDF_변경등록번호"), _T(""), nSelectIdx );
				nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();	
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
			continue;
		}
		
		MakeMarc( nSelectIdx );

		nSelectIdx = m_pRegNoChangerGrid->SelectGetNext();	
	}
	m_pRegNoChangerGrid->SelectRemoveAll();
	m_pRegNoChangerGrid->SelectMakeList();
	nSelectCnt = m_pRegNoChangerGrid->SelectGetCount();

	if( 0 < nSelectCnt )
	{
 		UpdateRegNo();
		ESLAfxMessageBox(_T("등록번호를 변경했습니다."));
	}
	else
	{
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
	}

	m_pRegNoChangerGrid->SetProtectSelectColumn(TRUE);
	m_pRegNoChangerGrid->Display();
	m_pRegNoChangerGrid->SetProtectSelectColumn(FALSE);
	return;
	
}

INT CSE_REG_NO_CHANGER::UpdateRegNo()
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
  	CString strHoldKey;
 	CString strIssRegCode;


	strSpeciesKey = _T("");

	m_pRegNoChangerGrid->SelectMakeList();	
	nSelectCnt = m_pRegNoChangerGrid->SelectGetCount();
	nSelectIdx = m_pRegNoChangerGrid->SelectGetHeadPosition();


 	ids = m_pRegNoChangerDM->StartFrame();
	for( INT i = 0 ; i < nSelectCnt ; i++ ) 
	{
		m_pRegNoChangerDM->GetCellData(_T("UDF_변경등록번호"), nSelectIdx, strChangeRegNo);
		m_pRegNoChangerDM->GetCellData(_T("HOLD_MARC"), nSelectIdx, strMarc);
		m_pRegNoChangerDM->GetCellData(_T("ISS_색인등록구분"), nSelectIdx, strIssRegCode);
		m_pRegNoChangerDM->GetCellData(_T("SS_소장KEY"), nSelectIdx, strHoldKey);
		m_pRegNoChangerDM->GetCellData(_T("SB_책KEY"), nSelectIdx,strBookKey);
		m_pRegNoChangerDM->GetCellData(_T("SB_종KEY"), nSelectIdx,strSpeciesKey);
		
		strChangeRegCode = strChangeRegNo.Left(m_nRegCodeLength);
		strQuery.Format(_T("UPDATE SE_BOOK_TBL SET REG_NO = '%s', REG_CODE = '%s' WHERE REC_KEY = %s;"), strChangeRegNo, strChangeRegCode, strBookKey );
 		m_pRegNoChangerDM->AddFrame(strQuery);
		if( strCompareSpeciesKey != strSpeciesKey )
		{
			strCompareSpeciesKey = strSpeciesKey;
			strQuery.Format(_T("UPDATE IDX_SE_TBL SET REG_CODE = '%s' WHERE REC_KEY = %s;"), strIssRegCode, strSpeciesKey );
 			m_pRegNoChangerDM->AddFrame(strQuery);
			if( !strMarc.IsEmpty() )
			{
				m_pRegNoChangerDM->AddDBFieldData(_T("HOLDING_DATA"), _T("STRING"), strMarc );
 				ids = m_pRegNoChangerDM->MakeUpdateFrame(_T("SE_HOLDING_TBL"), _T("REC_KEY"), _T("NUMERIC"), strHoldKey);
			}
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

INT CSE_REG_NO_CHANGER::MakeMarc(INT nSelectIdx)
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
 	CString strHoldKey;
 	CString strCompareHoldKey;
	CString strIssRegCode;
 
  	INT nDmRowCnt;
 	INT nItemCount;
 	INT nPreRegCodeLength;
 	INT ids;
	
 	nDmRowCnt = m_pRegNoChangerDM->GetRowCount();
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCulSeRegCodeLength);
	pEdit->GetWindowText(strTemp);
	nPreRegCodeLength = _ttoi( strTemp );

	m_pRegNoChangerDM->GetCellData(_T("UDF_변경등록번호"), nSelectIdx, strChangeRegNo);
	m_pRegNoChangerDM->GetCellData(_T("SB_등록번호"), nSelectIdx, strPreRegNo);
	m_pRegNoChangerDM->GetCellData(_T("HOLD_MARC"), nSelectIdx, strMarc);
	m_pRegNoChangerDM->GetCellData(_T("SS_소장KEY"), nSelectIdx, strHoldKey);
	m_pRegNoChangerDM->GetCellData(_T("ISS_색인등록구분"), nSelectIdx, strIssRegCode);

	if( strMarc.IsEmpty() )
	{
		return 0;
	}

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
				m_pRegNoChangerDM->GetCellData(_T("SS_소장KEY"), i, strCompareHoldKey);
				if( strHoldKey == strCompareHoldKey )
				{
					m_pRegNoChangerDM->SetCellData(_T("HOLD_MARC"), strMarc, i);
					m_pRegNoChangerDM->SetCellData(_T("ISS_색인등록구분"), strIssRegCode, i);
				}
 			}
			break;
		}
	}
	return 0;
}

INT CSE_REG_NO_CHANGER::UpdateParent(INT nSelectIdx)
{
	INT nParentSelectCnt;
 	INT nParentSelectIdx;
	CString strSpeciesKey;
	CString strParentSpeciesKey;
	CString strChangeRegNo;
	CString strPreRegNo;
	CString strParentRegNo;

	m_pRegNoChangerDM->GetCellData(_T("UDF_변경등록번호"), nSelectIdx, strChangeRegNo);
	m_pRegNoChangerDM->GetCellData(_T("SB_등록번호"), nSelectIdx, strPreRegNo);
	m_pRegNoChangerDM->GetCellData(_T("SB_종KEY"), nSelectIdx, strSpeciesKey);

	m_pParentGrid->SelectMakeList();
	nParentSelectCnt = m_pParentGrid->SelectGetCount();
	nParentSelectIdx = m_pParentGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nParentSelectCnt ; i++ )
	{
		m_pParentDM->GetCellData(_T("ISS_종KEY"), nParentSelectIdx, strParentSpeciesKey);
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

HBRUSH CSE_REG_NO_CHANGER::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CSE_REG_NO_CHANGER::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_연속_등록번호재부여")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pRegNoChangerCM = FindCM(_T("CM_연속_등록번호재부여"));
	if(m_pRegNoChangerCM == NULL )
	{
		ESLAfxMessageBox(_T("FindCM Error"));
		return FALSE;
	}
	m_pRegNoChangerDM = FindDM(_T("DM_연속_등록번호재부여"));
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

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCulSeRegCodeLength);
	pEdit->SetLimitText(1);
	CString strTemp;
	strTemp.Format(_T("%d"), pParentMgr->m_pInfo->nREG_CODE_LENGTH);
	pEdit->SetWindowText(strTemp);

	m_pRegNoChangerGrid->m_bGridEdit = TRUE;
	m_pRegNoChangerGrid->SELEDITOR_RemoveAllColumns();
	m_pRegNoChangerGrid->SELEDITOR_AddColumn(3);

	if(Init() < 0)
	{
		ESLAfxMessageBox(_T("DataInit Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSE_REG_NO_CHANGER::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CButton *pBtn = (CButton*)GetDlgItem(IDC_tbnStart1);
	if(pBtn->GetSafeHwnd() == NULL) return;
	pBtn->MoveWindow(cx - 90, 11, 80, 25);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridSE_REG_NO_CHANGER);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(5, 45, cx - 10, cy- 50);
	
	// TODO: Add your message handler code here
	
}

INT CSE_REG_NO_CHANGER::Init()
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
		m_pParentDM->GetCellData(_T("ISS_종KEY"),nSelectIdx,strTempData);
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
		strWhere = _T("S.REC_KEY = I.REC_KEY AND B.SPECIES_KEY = I.REC_KEY AND S.HOLDING_KEY = H.REC_KEY AND B.REG_NO IS NOT NULL AND ") + strWhere;
	}
	else
	{
		strWhere = _T("S.REC_KEY = I.REC_KEY AND B.SPECIES_KEY = I.REC_KEY AND S.HOLDING_KEY = H.REC_KEY AND B.REG_NO IS NOT NULL AND ( ") + strWhere + _T(")");
	}

	m_pRegNoChangerDM->RefreshDataManager(strWhere);
	m_pRegNoChangerGrid->Display();	
	
	return 0 ;
}
void CSE_REG_NO_CHANGER::OnKillfocusedtCulSeRegCodeLength() 
{
	CString strRegCodeLength;
	INT nRegCodeLength;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtCulSeRegCodeLength);

	pEdit->GetWindowText(strRegCodeLength);
	nRegCodeLength = _ttoi(strRegCodeLength);

	if( 0 > nRegCodeLength || 6 < nRegCodeLength )
	{
		ESLAfxMessageBox(_T("등록구분 자릿수 설정이 잘못 되었습니다."));
		strRegCodeLength.Format(_T("%d"), pParentMgr->m_pInfo->nREG_CODE_LENGTH);
		pEdit->SetWindowText(strRegCodeLength);
	}
}

