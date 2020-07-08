// SE_CAT_REG_0400.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_REG_0400.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_0400 dialog

CSE_CAT_REG_0400::CSE_CAT_REG_0400(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_0400)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_REG_0400::~CSE_CAT_REG_0400()
{
}

VOID CSE_CAT_REG_0400::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_0400)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}

BEGIN_MESSAGE_MAP(CSE_CAT_REG_0400, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_REG_0400)
	ON_BN_CLICKED(IDC_bACCESSIONNO_GIVE, OnbACCESSIONNOGIVE)
	ON_BN_CLICKED(IDC_rCanRegNo, OnrCanRegNo)
	ON_BN_CLICKED(IDC_rAcqBack, OnrAccCan)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_0400 message handlers

BOOL CSE_CAT_REG_0400::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("BO_CAT_REG_0400")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if(OpenMode == 0)
	{
		m_DM_ALIAS = _T("DM_연속_정리_등록_메인");
		m_CM_ALIAS = _T("CM_연속_정리_등록_메인");
	}
	else if(OpenMode == 2)
	{
		m_DM_ALIAS = _T("DM_연속_정리_원부_메인"); 
		m_CM_ALIAS = _T("CM_연속_정리_원부_메인");
	}
	else 
	{
		AfxMessageBox(_T("OpenMode Error"));
		return FALSE;
	}

	INT iReturn = Display(0);

	//선택이 안되어 있으면 화면을 띄우지 않는다.
	if ( iReturn == -1 ) {
		m_bInitDlg = FALSE;
		return FALSE;
	}

	CButton * pCanRegNo;
	pCanRegNo=(CButton *)GetDlgItem(IDC_rCanRegNo);
	pCanRegNo->SetCheck(1);	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
EFS_END
return FALSE;

}

BOOL CSE_CAT_REG_0400::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;
}

INT CSE_CAT_REG_0400::Display(INT nDir)
{
EFS_BEGIN

	INT idx;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));

	if(idx < 0)
	{
		//에러처리
		ESLAfxMessageBox(_T("선정된 자료가 없습니다."));
		return -1;
	}	

	CESL_DataMgr *pDM = FindDM(m_DM_ALIAS);
	if ( NULL == pDM ) 
	{
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return -1;
	}

	CString BookKey,strStatus;
	strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

	if (strStatus != _T("SER112N")) //등록자료 
	{
		//에러처리
		ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
		return -1;
	}
	idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));

	while ( idx != -1 ) 
	{
		strStatus = pDM->GetCellData( _T("BB_자료상태_코드") , idx );

		if (strStatus != _T("SER112N")) //등록자료 
		{
			//에러처리
			ESLAfxMessageBox(_T("등록 자료만 처리 할 수 있습니다."));
			return -1;
		}
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	return 0;

EFS_END
return -1;
}

VOID CSE_CAT_REG_0400::OnbACCESSIONNOGIVE() 
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
EFS_BEGIN

	if(OpenMode == 0)
		CancelRegNo();
	
	else if(OpenMode == 2)
		CancelRegNo();
	
EFS_END
}

VOID CSE_CAT_REG_0400::OnCancel() 
{
EFS_BEGIN

	DestroyWindow();		
	//CDialog::OnCancel();

EFS_END
}

VOID CSE_CAT_REG_0400::OnrCanRegNo() 
{
EFS_BEGIN

	CButton * chkRegCan;
	chkRegCan=(CButton *)GetDlgItem(IDC_rCanRegNo);
	chkRegCan->SetCheck(1);

EFS_END
}

VOID CSE_CAT_REG_0400::OnrAccCan() 
{
EFS_BEGIN

	CButton * chkRegCan;
	chkRegCan=(CButton *)GetDlgItem(IDC_rAcqBack);
	chkRegCan->SetCheck(1);

EFS_END
}

INT CSE_CAT_REG_0400::CancelRegNo()
{		
	m_DM_ALIAS = _T("DM_연속_정리_등록_메인");
	m_CM_ALIAS = _T("CM_연속_정리_등록_메인");
	
	INT iReturn = Display(0);
	if (iReturn == -1)
		return -1;

// DM, CM, GRID 선정자료 체크 
	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);
	if (!pDM)
	{
		AfxMessageBox(_T("DM 에러!")); 
		return -1; 
	}
	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);
	if (!pCM)
	{
		AfxMessageBox(_T("CM 에러!")); 
		return -1; 
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));
	pGrid->SelectMakeList();
	INT Index = pGrid->SelectGetCount();
	if (Index < 1)
	{
		AfxMessageBox(_T("선정한 자료가 없습니다.")); 
		return -1;
	}
	
	INT iCh;
	iCh = pDM->StartFrame();

	//누락등록번호 추가 변수
	INT iBookCount = -1; 
	CESL_DataMgr DM_BOOK;

	INT ids;// = 6;
	ids = ::MessageBox( GetSafeHwnd() , _T("선정하신 자료의 등록번호를 삭제하고 수서 복귀 하시겠습니까?") ,
			_T("삭 제") ,
			MB_YESNO | MB_ICONQUESTION );
	
	if ( ids == IDYES ) 
	{		
		DeleteRegNo();
		BackAcq();
	}
	else
		return -1;

	iCh = pDM->SendFrame();
	if ( iCh == -3 || iCh == -1) 
	{
		AfxMessageBox(_T("SendFrame Query Error!!"));
		return -5;
	}
	iCh = pDM->EndFrame();	
	DestroyWindow();	
	
	return 0;
}

INT CSE_CAT_REG_0400::DeleteRegNo()
{
	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	// 등록 번호를 선택하여 누락등록 번호에 넣는다. 
	INT idx;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
	
	CString strRegNo,strRegCode,sRecKey;
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = pParentMgr->m_pInfo->nREG_CODE_LENGTH;
	//=====================================================
	
	while ( idx != -1 ) 
	{
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		
		if (strRegNo != _T("")) //등록자료 
		{
			strRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
			sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx );

			// 등록 번호 제거
			sQuery.Format( _T("UPDATE SE_BOOK_TBL SET ACCESSION_REC_KEY = NULL, REG_NO = NULL , WORKING_STATUS = 'SER111N' WHERE REC_KEY = %s;") , sRecKey );
			iCh = pDM->AddFrame( sQuery );	

			//누락 등록 테이블에 있는 지 조사 
			CString sQuery,sCount;
			//=====================================================
			//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 			sQuery.Format( _T("SELECT COUNT(REG_NO) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO =%s") , strRegCode , strRegNo.Mid(2,10));
			sQuery.Format( _T("SELECT COUNT(REG_NO) FROM CO_DROP_ACCESSION_NO_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND REG_CODE ='%s' AND REG_NO =%s") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength ));
			//=====================================================
			
			pDM->GetOneValueQuery( sQuery , sCount );
			//없으면 입력 
			if ( sCount == _T("0") ) 
			{
				CString sRecKey;
				pDM->MakeRecKey(sRecKey);

				//=====================================================
				//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %s, UDF_MANAGE_CODE );") , sRecKey , _T("SE") , _T("") , strRegCode , strRegNo.Mid(2,10) );
				sQuery.Format( _T("INSERT INTO CO_DROP_ACCESSION_NO_TBL ( REC_KEY , PUBLISH_FORM_CODE , ACCESSION_REC_NO , REG_CODE , REG_NO, MANAGE_CODE ) VALUES ( %s, '%s', '%s', '%s', %s, UDF_MANAGE_CODE );") , sRecKey , _T("SE") , _T("") , strRegCode , strRegNo.Mid(nRegCodeLength,12 - nRegCodeLength) );
				//=====================================================				
				
				iCh = pDM->AddFrame( sQuery );		
			}		
			
			pDM->SetCellData( _T("원부번호") ,_T(""), idx );
			pDM->SetCellData( _T("BB_등록번호") ,_T(""), idx );
			pDM->SetCellData( _T("BB_자료상태_코드") ,_T("SER111N"), idx );

			// 해당 그리드 반영 
			pGrid->DisplayLine(idx);
		}
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	return 0;
}

INT CSE_CAT_REG_0400::BackAcq()
{
	INT iCh;	
	CString sQuery;

	CESL_DataMgr *pDM;	
	pDM = FindDM(m_DM_ALIAS);		

	CESL_ControlMgr *pCM;
	pCM = FindCM(m_CM_ALIAS);		
	
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	INT idx = 0;
	SelectMakeList(m_CM_ALIAS, _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(m_CM_ALIAS, _T("MAIN_GRID"));
	CString AcqCode, sRecKey;
    CArray <INT, INT> RemoveAllList;

	// 수서복귀 시간 (REG_TRANSFER_DATE)
	CTime t = CTime::GetCurrentTime();
	CString result;
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	while ( idx != -1 ) 
	{
		sRecKey = pDM->GetCellData( _T("BB_책KEY") , idx);

		sQuery.Format( _T("UPDATE SE_BOOK_TBL SET WORKING_STATUS = 'SER111N', PREV_WORKING_STATUS = 'SER112N', TRANSFER_YN = 'Y', REG_TRANSFER_DATE = '%s' WHERE REC_KEY = %s;"), result, sRecKey );
		iCh = pDM->AddFrame( sQuery );	

		RemoveAllList.Add(idx);
		idx = SelectGetNext(m_CM_ALIAS, _T("MAIN_GRID"));
	}
	
	// DM 반영  
	for ( INT i = RemoveAllList.GetSize()-1 ; i >= 0 ; i-- )
	{ 
		INT RowIdx = RemoveAllList.GetAt(i); 
		pDM->DeleteRow(RowIdx);
	}
	
	pGrid->Display();

	return 0;
}


HBRUSH CSE_CAT_REG_0400::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
