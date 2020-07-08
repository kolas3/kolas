// SE_CAT_CODE_CHANGE.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_CODE_CHANGE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_CODE_CHANGE dialog


CSE_CAT_CODE_CHANGE::CSE_CAT_CODE_CHANGE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_CODE_CHANGE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_CODE_CHANGE::~CSE_CAT_CODE_CHANGE()
{
}

VOID CSE_CAT_CODE_CHANGE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_CODE_CHANGE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_CODE_CHANGE, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_CODE_CHANGE)
	ON_BN_CLICKED(IDC_bCHANGE_CODE, OnbCHANGECODE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_CODE_CHANGE message handlers

BOOL CSE_CAT_CODE_CHANGE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_등록_코드")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
	
	INT idx;

	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		EndDialog(IDCANCEL);
		return FALSE;
	}


	// 040304_HSS 등록번호 있는 자료가 선택된 경우 메세지 보여주기 ++++++++++++++++++
	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인 Error") );
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CString strRegNo;
	CString sRegCode = _T("");
	CString sSelfCode = _T("");
	CString sManageCode = _T("");
	// 040402_HSS 등록번호 있는 자료는 등록구분만 변경 안함
	/*
	while ( idx != -1 ) {
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		// 040324_HSS 기본내용 세팅 - 제일 위에 있는 내용 ++++++++++++++++++
		if (sRegCode.GetLength()==0){
			sRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
			SetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_등록구분"), sRegCode);
		}
		if (sSelfCode.GetLength()==0){
			sSelfCode = pDM->GetCellData( _T("BB_별치구분_코드") , idx );
			SetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_별치구분"), sSelfCode);
		}
		if (sManageCode.GetLength()==0){
			sManageCode = pDM->GetCellData( _T("BB_관리구분_코드") , idx );
			SetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_관리구분"), sManageCode);
		}
		// +++++++++++++++++++++++++++++++++++++++++++

		if (strRegNo.GetLength()>0){
			AfxMessageBox(_T("등록번호가 부여된 자료가 선택되었습니다.\n")
						_T("등록번호가 부여된 자료는 등록구분을 변경할 수 없습니다.\n")
						_T("등록구분의 변경을 원하시면 번호취소 후 변경하여 주시기 바랍니다."));
			// 040402_HSS 등록번호 있는 자료는 등록구분 변경 안함
			//EndDialog(IDCANCEL);
			//return;
			break;
		}
		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}
	*/
	// 그냥 첫번째 자료의 코드를 세팅
	sRegCode = pDM->GetCellData( _T("BB_등록구분_코드") , idx );
	SetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_등록구분"), sRegCode);
	sSelfCode = pDM->GetCellData( _T("BB_별치구분_코드") , idx );
	SetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_별치구분"), sSelfCode);
	sManageCode = pDM->GetCellData( _T("BB_관리구분_코드") , idx );
	SetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_관리구분"), sManageCode);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// 040324_HSS 별치, 관리구분 추가
VOID CSE_CAT_CODE_CHANGE::OnbCHANGECODE() 
{
EFS_BEGIN

	INT idx;
	CString strBookKey,strRegNo,strSpeciesKey,strQuery,sTmp;
	CString sRegCode, sSelfCode, sManageCode;
	
	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_연속_정리_등록_메인"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_연속_정리_등록_메인 error") );
		return ;
	}
	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("MAIN_GRID"));

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return ;
	}

	// 변경할 코드 가져오기 
	GetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_등록구분"), sRegCode);
	GetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_별치구분"), sSelfCode);
	GetControlData(_T("CM_연속_정리_등록_코드"), _T("BB_관리구분"), sManageCode);

	// 대상 책키 리스트 만들기
	SelectMakeList(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	idx = SelectGetHeadPosition(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	// 변경 spl 실행
	pDM->StartFrame();

	BOOL bSendMsg = FALSE;
	while ( idx != -1 ) {
		strBookKey = pDM->GetCellData( _T("BB_책KEY") , idx );
		// 040402_HSS 등록번호 있는 자료는 등록구분 변경 안함
		strRegNo = pDM->GetCellData( _T("BB_등록번호") , idx );
		if (strRegNo.GetLength()>0 && !bSendMsg)
		{
			AfxMessageBox(_T("선택된 자료 중 등록번호가 부여된 자료있습니다.\n")
						_T("등록번호가 부여된 자료의 등록구분은 변경할 수 없습니다.\n")
						_T("등록구분의 변경을 원하시면 번호취소 후 변경하여 주시기 바랍니다."));
			bSendMsg = TRUE;
		}

		pDM->InitDBFieldData();
		if (strRegNo.GetLength()<=0)
			pDM->AddDBFieldData(_T("REG_CODE"), _T("STRING"), sRegCode );
		pDM->AddDBFieldData(_T("SEPARATE_SHELF_CODE"), _T("STRING"), sSelfCode );
		pDM->AddDBFieldData(_T("MANAGE_CODE"), _T("STRING"), sManageCode );
		pDM->AddDBFieldData(_T("LAST_WORK"), _T("STRING"), GetWorkLogMsg(WORK_LOG) );
		pDM->MakeUpdateFrame(_T("SE_BOOK_TBL"), _T("REC_KEY"), _T("NUMERIC"), strBookKey, idx);
		
		if (strRegNo.GetLength()<=0)
			pDM->SetCellData(_T("BB_등록구분_코드"), sRegCode, idx);
		pDM->SetCellData(_T("BB_별치구분_코드"), sSelfCode, idx);
		pDM->SetCellData(_T("BB_관리구분_코드"), sManageCode, idx);

		// 040305_HSS 색인변경추가 ++++++++++++++++++++++++++++++
		// 040402_HSS 색인은 새로 가져오는 것이며 위에서 업데이트 안했을 경우 같은 값임
		CString strRegCode, strSeparateCode, strManageCode;
		strSpeciesKey = pDM->GetCellData( _T("IBS_종KEY") , idx );
		strRegCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'REG_CODE') FROM DUAL)"),strSpeciesKey);
		strSeparateCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'SEPARATE_SHELF_CODE') FROM DUAL)"),strSpeciesKey);
		strManageCode.Format(_T("(SELECT GET_SE_BOOK_INFO (%s,'MANAGE_CODE') FROM DUAL)"),strSpeciesKey);
		//=========================================================
		//2008.10.17 UPDATE BY PJW : MANAGE_CODE 는 디폴트로 들어간다.
//		strQuery.Format(_T("UPDATE IDX_SE_TBL SET REG_CODE = %s, IDX_SE_SHELF_CODE = %s, MANAGE_CODE = %s WHERE REC_KEY = %s;"),
//					strRegCode, strSeparateCode, strManageCode, strSpeciesKey);
		strQuery.Format(_T("UPDATE IDX_SE_TBL SET REG_CODE = %s, IDX_SE_SHELF_CODE = %s, MANAGE_CODE = UDF_MANAGE_CODE WHERE REC_KEY = %s;"),
					strRegCode, strSeparateCode,  strSpeciesKey);
		pDM->AddFrame( strQuery );
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++

		pGrid->DisplayLine(idx);
		idx = SelectGetNext(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	}
	INT ids = pDM->SendFrame();
	if (ids<0){
		AfxMessageBox(_T("업데이트에 실패하였습니다."));
	}
	else 
	{
		AfxMessageBox(_T("변경 되었습니다."));
		ids = pDM->EndFrame();
	}

	pGrid->ReshowSelectedRows();

	return ;	
EFS_END
}

HBRUSH CSE_CAT_CODE_CHANGE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
