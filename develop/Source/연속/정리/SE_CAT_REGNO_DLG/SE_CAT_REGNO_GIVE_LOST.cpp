// SE_CAT_REGNO_GIVE_LOST.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_REGNO_GIVE_LOST.h"
#include "SE_CAT_GROUP_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REGNO_GIVE_LOST dialog


CSE_CAT_REGNO_GIVE_LOST::CSE_CAT_REGNO_GIVE_LOST(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_nCount = -1;
	m_nSelected = -1;

	m_pRegNoArray = NULL;
}

CSE_CAT_REGNO_GIVE_LOST::~CSE_CAT_REGNO_GIVE_LOST()
{
}


VOID CSE_CAT_REGNO_GIVE_LOST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_REGNO_GIVE_LOST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_REGNO_GIVE_LOST, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_REGNO_GIVE_LOST)
	ON_BN_CLICKED(IDC_BTN_Search, OnBTNSearch)
	ON_BN_CLICKED(IDC_CHANGE_GROUP, OnChangeGroup)
	ON_BN_CLICKED(IDC_bINSERT_DROP_REG_NO, OnbINSERTDROPREGNO)
	ON_CBN_SELCHANGE(IDC_cREG_CODE_DROP, OnSelchangecREGCODEDROP)
	ON_CBN_SELCHANGE(IDC_cREG_Change_CLASS, OnSelchangecREGChangeCLASS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REGNO_GIVE_LOST message handlers

BOOL CSE_CAT_REGNO_GIVE_LOST::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_누락등록번호_부여")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CESL_Grid * pGrid = (CESL_Grid*)FindControl(_T("CM_연속_정리_누락등록번호"), _T("MAIN_GRID"));
    m_nCount = pGrid->SelectGetCount();
		

	m_nSelected = 0;
	
	CString strbuf;
	strbuf.Format(_T(" %d/%d "),m_nSelected,m_nCount);
/*
	for( INT i = 0 ; i < m_pRegNoArray->GetSize() ; i++ ) {
		CString sTmp = m_pRegNoArray->GetAt(i);
	}
	*/


//
	
	CString strRegGroup;
	INT row = pGrid->GetRow();
	INT icount=0;
	INT iRowCount = pGrid->GetCount();

	CESL_DataMgr *pTmpDM = FindDM(_T("DM_연속_정리DM_책정보"));
	if ( NULL == pTmpDM ) 
	{
		AfxMessageBox ( _T("DM_연속_정리DM_책정보") );
		return FALSE;
	}
	CString strRegCode;
	//선정된 자료의 갯수 체크
	for (INT i = 1; i<=iRowCount; i++)
	{
		if (pGrid->GetAt(i-1,0) == _T("V"))	
			strRegCode = pTmpDM->GetCellData(_T("SB_등록구분_코드"), i-1);
	}

	SetControlData( _T("CM_연속_정리_누락등록번호_부여"), _T("등록구분"), strRegCode );
//


	GetDlgItem(IDC_EDT_COUNT)->SetWindowText(strbuf);

	// 원부번호 가져오기
	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_누락등록번호_부여"));
	if (!pDM) return FALSE;

	CString strQuery;
	m_strYear = CTime::GetCurrentTime().Format(_T("%Y"));
		
	strQuery.Format(_T("SELECT MAX(ACCESSION_REC_NO) FROM CO_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACCESSION_REC_MAKE_YEAR = %s"), m_strYear);
	
	CString strAccNo;
	pDM->GetOneValueQuery(strQuery, strAccNo);

	SetControlData(_T("CM_연속_정리_누락등록번호_부여"), _T("원부번호"), strAccNo);

	// 등록번호 읽어오기
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.09.16 이학중 수정 
	//처음 화면에 검색 내용 나타나지 않게 

	//Load_REGNO(_T(""));

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CAT_REGNO_GIVE_LOST::Load_REGNO(CString mQry)
{
	CString strQry;
	INT ids;

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_누락등록번호_부여"));
	
	strQry=_T(" PUBLISH_FORM_CODE = 'MO' ");
	
	if( mQry != _T(""))
	{
		strQry += mQry;
	}

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해 주세요"));
		return;
	}

	ids = pDM->GetRowCount();
	if ( ids == 0 )
	{
		AfxMessageBox(_T("검색된 누락등록번호가 없습니다."));
		return;
	}

	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_연속_정리_누락등록번호_부여"), _T("REGNO_GRID"));
	pGrid->Display();	
}

BEGIN_EVENTSINK_MAP(CSE_CAT_REGNO_GIVE_LOST, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_CAT_REGNO_GIVE_LOST)
	ON_EVENT(CSE_CAT_REGNO_GIVE_LOST, IDC_GRID_REGNO, -600 /* Click */, OnClickGridRegno, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_CAT_REGNO_GIVE_LOST::OnClickGridRegno() 
{
	// TODO: Add your control notification handler code here
	
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_연속_정리_누락등록번호_부여"), _T("REGNO_GRID"));
	if (pGrid == NULL) return;

	CString strbuf;
	
	INT nRow =  pGrid->GetRow();
	INT nCol =	pGrid->GetCol();

	// 선정될수 있는지 체크한다.
	if (nCol == 1)
	{
		pGrid->GetAt(nRow-1, 0 ,strbuf);
		
		if (strbuf == _T("V") )
		{
			pGrid->SetAt(nRow-1, 0 , _T(""));

			m_nSelected--;
		}
		else
		{			
			m_nSelected++;

			// 개수 체크
			if (m_nSelected > m_nCount)
			{
				AfxMessageBox(_T("선택 하신 책 수 보다 많은 누락 등록번호를 선택 할 수 없습니다."));
				m_nSelected--;
				return;
			}
			pGrid->SetAt(nRow-1, 0 , _T("V"));
		}

		strbuf.Format(_T(" %d/%d "),m_nSelected,m_nCount);
		GetDlgItem(IDC_EDT_COUNT)->SetWindowText(strbuf);
	}
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnBTNSearch() 
{
	CString sRegCode,sRegNO_s,sRegNO_e,strBuf,strQry;

	GetControlData(_T("CM_연속_정리_누락등록번호_부여"), _T("등록구분"), sRegCode);
	GetControlData(_T("CM_연속_정리_누락등록번호_부여"), _T("등록번호시작"), sRegNO_s);
	GetControlData(_T("CM_연속_정리_누락등록번호_부여"), _T("등록번호끝"), sRegNO_e);

	sRegCode.TrimLeft();
	sRegCode.TrimRight();

	sRegNO_s.TrimLeft();
	sRegNO_s.TrimRight();
	
	sRegNO_e.TrimLeft();
	sRegNO_e.TrimRight();

	if ( sRegCode != _T("") )
	{
		strBuf.Format(_T(" AND REG_CODE = '%s' "),sRegCode); 
		strQry += strBuf;  
	}

	if ( sRegNO_s != _T("") && sRegNO_e != _T("") )
	{
		strBuf.Format(_T(" AND REG_NO >= %s AND REG_NO <= %s "),sRegNO_s,sRegNO_e); 
		strQry += strBuf;  
	}

	if ( sRegNO_s != _T("") && sRegNO_e == _T("") )
	{
		strBuf.Format(_T(" AND REG_NO >= %s "),sRegNO_s); 
		strQry += strBuf;  
	}

	if ( sRegNO_s == _T("") && sRegNO_e != _T("") )
	{
		strBuf.Format(_T(" AND REG_NO <= %s "),sRegNO_e); 
		strQry += strBuf;  
	}

	Load_REGNO(strQry);
	
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnOK() 
{
	INT idx,Book_idx;
	SelectMakeList(_T("CM_연속_정리_누락등록번호_부여"), _T("REGNO_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_누락등록번호_부여"), _T("REGNO_GRID"));

	if (idx < 0)
	{
		//에러처리
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_누락등록번호_부여"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_누락등록번호_부여") );
		return ;
	}

	CESL_DataMgr *pBookDM = FindDM(_T("DM_연속_정리DM_책정보"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리DM_책정보") );
		return ;
	}
	
	Book_idx = SelectGetHeadPosition(_T("CM_연속_정리_누락등록번호"), _T("MAIN_GRID"));

	CString strRegNo,strRegCode,strReg,strRecKey,strQry,ACC_KEY;
	// 원부 번호를 구한다. 
	GetControlData( _T("CM_연속_정리_누락등록번호_부여"), _T("원부번호"), ACC_KEY );
	ACC_KEY.TrimLeft();
	ACC_KEY.TrimRight();
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //2003.10.09 이학중 수정 
	//원부번호가 없을경우 ACC_KEY = NULL 로 입력한다.
    /*
	if ( ACC_KEY==_T(""))
	{
		AfxMessageBox ( _T("원부 번호가 없습니다!!!") );
		return ;		
	}

	if( m_strYear == _T(""))
	{
		AfxMessageBox ( _T("다시 원부를 선택 해 주십시요!!!") );
		return ;		
	}

    */
	if(!ACC_KEY.IsEmpty () && !m_strYear.IsEmpty ())
	{
		strQry = _T("SELECT REC_KEY FROM CO_ACCESSION_REC_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND ACCESSION_REC_NO =")+ACC_KEY+_T("AND ACCESSION_REC_MAKE_YEAR =")+m_strYear;	
		pDM->GetOneValueQuery(strQry,ACC_KEY);
	}
	else
	{
		ACC_KEY = _T("") ;
	}


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



	//등록 번호를 넣어 준고 누락에서 삭제 한다.
	strRegNo = pDM->GetCellData( _T("등록번호") , idx );
	strRegCode = pDM->GetCellData( _T("등록구분") , idx );
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strReg.Format(_T("%s%010d"),strRegCode,_ttoi(strRegNo.GetBuffer(0)));
	
	strReg = pParentMgr->m_pInfo->MakeRegNo(strRegCode,strRegNo);
	//=====================================================
	

	pBookDM->SetCellData(_T("SB_등록번호"),strReg,Book_idx);
	pBookDM->SetCellData(_T("SB_원부KEY"),ACC_KEY,Book_idx);

	//누락에서 제거 
	m_pRegNoArray->Add(strReg);
	
	INT ids = pDM->StartFrame();
	if ( 0 > ids ) return;

	strRecKey = pDM->GetCellData( _T("REC_KEY") , idx );
	strQry.Format(_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE REC_KEY = %s ;"),strRecKey);
	ids = pDM->AddFrame( strQry );
	if ( 0 > ids ) return;
	
	idx = SelectGetNext(_T("CM_연속_정리_누락등록번호_부여"), _T("REGNO_GRID"));
	Book_idx = SelectGetNext(_T("CM_연속_정리_누락등록번호"), _T("MAIN_GRID"));

	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("등록번호") , idx );
		strRegCode = pDM->GetCellData( _T("등록구분") , idx );
		//=====================================================
		//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 		strReg.Format(_T("%s%010d"),strRegCode,_ttoi(strRegNo.GetBuffer(0)));
		//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
		strReg = pParentMgr->m_pInfo->MakeRegNo(strRegCode,strRegNo);
		//=====================================================
		
		pBookDM->SetCellData(_T("SB_등록번호"),strReg,Book_idx);
		pBookDM->SetCellData(_T("SB_원부KEY"),ACC_KEY,Book_idx);
		//누락에서 제거 
		m_pRegNoArray->Add(strReg);

		strRecKey = pDM->GetCellData( _T("REC_KEY") , idx );
		strQry.Format(_T("DELETE FROM CO_DROP_ACCESSION_NO_TBL WHERE REC_KEY = %s ;"),strRecKey);
		pDM->AddFrame( strQry );
		//
		idx = SelectGetNext(_T("CM_연속_정리_누락등록번호_부여"), _T("REGNO_GRID"));
		Book_idx = SelectGetNext(_T("CM_연속_정리_누락등록번호"), _T("MAIN_GRID"));
	}


	ids = pDM->SendFrame();
	if ( 0 > ids ) return;	
	ids = pDM->EndFrame();
	if ( 0 > ids ) return;

	CDialog::OnOK();
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnChangeGroup() 
{
	CSE_CAT_GROUP_INSERT Dlg(this);
	Dlg.DoModal();
	
	m_strYear = Dlg.m_strYear;

}

VOID CSE_CAT_REGNO_GIVE_LOST::OnbINSERTDROPREGNO() 
{
	INT ids = -1;
	CString strRegCode;
	CString strRegNo;
	CString strQuery;
	CString strResult;

	GetControlData(_T("CM_연속_정리_누락등록번호_부여"), _T("입력_등록구분"), strRegCode);
	GetControlData(_T("CM_연속_정리_누락등록번호_부여"), _T("입력_등록번호"), strRegNo);
	

	strRegCode.TrimLeft();
	strRegCode.TrimRight();

	strRegNo.TrimLeft();
	strRegNo.TrimRight();
	
	if (strRegCode.IsEmpty())
	{
		AfxMessageBox(_T("등록구분을 설정하세요"));
		return;
	}

	if (strRegNo.IsEmpty())
	{
		AfxMessageBox(_T("등록번호를 입력하세요"));
		return;
	}

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_누락등록번호_부여"));
	if (!pDM) return;
	// 마지막 번호보다 큰지 검사
	strQuery.Format(_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND KIND_CODE = 'REG_NO' AND DIVISION_VALUE = '%s'")
				    ,strRegCode);
	ids = pDM->GetOneValueQuery(strQuery, strResult);
	if (ids < 0 || _ttoi(strRegNo) > _ttoi(strResult))
	{
		AfxMessageBox(_T("입력하신 등록번호는 마지막 번호보다 큽니다.."));
		return;
	}

	// 중복 검사 1. 책 TABLE에서 2. 누락등록번호 TABLE에서
	CString strRegNoField;
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strRegNoField.Format(_T("%s%010s"), strRegCode, strRegNo);
	//nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	strRegNoField = pParentMgr->m_pInfo->MakeRegNo(strRegCode,strRegNo);
	//=====================================================
	
	strQuery.Format(_T("SELECT COUNT(*) FROM BO_BOOK_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_NO = '%s'"), strRegNoField);
	ids = pDM->GetOneValueQuery(strQuery, strResult);
	if (ids < 0 || strResult != _T("0"))
	{
		AfxMessageBox(_T("입력하신 등록번호는 중복된 번호 입니다."));
		return;
	}

	strQuery.Format(_T("SELECT COUNT(*) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND PUBLISH_FORM_CODE = 'MO' AND REG_CODE = '%s' AND REG_NO = %d")
					, strRegCode, _ttoi(strRegNo));
	ids = pDM->GetOneValueQuery(strQuery, strResult);
	if (ids < 0 || strResult != _T("0"))
	{
		AfxMessageBox(_T("입력하신 등록번호는 중복된 번호 입니다."));
		return;
	}

	// DB에 Insert한다.
	CString strRecKey;
	ids = pDM->StartFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("StartFrame 를 확인하세요"));
		return;
	}

	pDM->MakeRecKey(strRecKey);
	
	pDM->InitDBFieldData();
	pDM->AddDBFieldData(_T("REC_KEY"), _T("NUMERIC"), strRecKey);
	pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), strRegCode);
	pDM->AddDBFieldData(_T("REG_NO"), _T("NUMERIC"), strRegNo);
	pDM->AddDBFieldData(_T("PUBLISH_FORM_CODE"), _T("STRING"), _T("MO"));
	pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), _T("UDF_MANAGE_CODE"));

	pDM->AddDBFieldData(_T("FIRST_WORK"), _T("STRING"), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG));
	pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(_T("등록번호부여취소"),WORK_LOG));


	ids = pDM->MakeInsertFrame(_T("CO_DROP_ACCESSION_NO_TBL"));
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("MakeInsertFrame 를 확인하세요"));
		return;
	}
	
	ids = pDM->SendFrame();
	if (ids < 0)
	{
		AfxMessageBox(_T("DB와의 작업에 실패했습니다."));
		return;
	}

	ids = pDM->EndFrame();
	if ( ids < 0 ) 
	{
		AfxMessageBox(_T("EndFrame 를 확인하세요"));
		return;
	}

	// DM에 정보를 Insert한다.
	pDM->InsertRow(-1);
	INT nRow = pDM->GetRowCount() - 1;
	pDM->SetCellData(_T("REC_KEY"), strRecKey, nRow);
	pDM->SetCellData(_T("등록구분"), strRegCode, nRow);
	pDM->SetCellData(_T("등록번호"), strRegNo, nRow);


	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_연속_정리_누락등록번호_부여"), _T("REGNO_GRID"));
	pGrid->DisplayLine(nRow);
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnSelchangecREGCODEDROP() 
{
	// TODO: Add your control notification handler code here
	CString strSelRegCode;
	CString strRegCode;

	CESL_DataMgr *pTmpDM = FindDM(_T("DM_연속_정리DM_책정보"));
	if ( NULL == pTmpDM ) 
	{
		AfxMessageBox ( _T("DM_연속_정리DM_책정보") );
		return ;
	}

	//기존의 등록번호
	strRegCode = pTmpDM->GetCellData(_T("SB_등록구분_코드"), 0);

	//멀티드롭 선택된 항목
	GetControlData( _T("CM_연속_정리_누락등록번호_부여"), _T("입력_등록구분"), strSelRegCode );
	

	if ( strSelRegCode != strRegCode )
	{
		AfxMessageBox(_T("등록구분을 확인해 주세요"));
		SetControlData( _T("CM_연속_정리_누락등록번호_부여"), _T("입력_등록구분"), strRegCode );
		return;
	}
	
}

VOID CSE_CAT_REGNO_GIVE_LOST::OnSelchangecREGChangeCLASS() 
{
	// TODO: Add your control notification handler code here
	CString strSelRegCode;
	CString strRegCode;

	CESL_DataMgr *pTmpDM = FindDM(_T("DM_연속_정리DM_책정보"));
	if ( NULL == pTmpDM ) 
	{
		AfxMessageBox ( _T("DM_연속_정리DM_책정보") );
		return ;
	}

	//기존의 등록번호
	strRegCode = pTmpDM->GetCellData(_T("SB_등록구분_코드"), 0);

	//멀티드롭 선택된 항목
	GetControlData( _T("CM_연속_정리_누락등록번호_부여"), _T("등록구분"), strSelRegCode );
	

	if ( strSelRegCode != strRegCode )
	{
		AfxMessageBox(_T("등록구분을 확인해 주세요"));
		SetControlData( _T("CM_연속_정리_누락등록번호_부여"), _T("등록구분"), strRegCode );
		return;
	}
	
}

HBRUSH CSE_CAT_REGNO_GIVE_LOST::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
