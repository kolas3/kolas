// BO_REG_EPC_SEARCH.cpp : implementation file
//

#include "stdafx.h"
#include "BO_REG_EPC_SEARCH.h"


#include "../../../공통/BO_ACQ_GROUP_MANAGER/DivisionNo_List_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/RequisitionNo_List_Dlg.h"
#include "../../../공통/BO_ACQ_GROUP_MANAGER/GroupCollection.h"

//==============================================
//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
#include "SBL.H"
//==============================================

#include "Imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_SEARCH dialog

CBO_REG_EPC_SEARCH::CBO_REG_EPC_SEARCH(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_REG_EPC_SEARCH)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_REG_EPC_SEARCH::~CBO_REG_EPC_SEARCH()
{
}

VOID CBO_REG_EPC_SEARCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_REG_EPC_SEARCH)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_REG_EPC_SEARCH, CDialog)
	//{{AFX_MSG_MAP(CBO_REG_EPC_SEARCH)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SEQ_INFO, OnBtnSeqInfo)
	ON_CBN_SELCHANGE(IDC_CB_EPCDATA_ACQ_CODE, OnSelchangeCbEpcdataAcqCode)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDT_EPCDATA_REG_NO1, OnSetfocusEdtEpcdataRegNo1)
	ON_EN_SETFOCUS(IDC_EDT_EPCDATA_REG_NO2, OnSetfocusEdtEpcdataRegNo2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_SEARCH message handlers

BOOL CBO_REG_EPC_SEARCH::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( 0 > InitESL_Mgr(_T("BO_BOOK_TRADE_REG_SEARCH")) )
	{
		AfxMessageBox(_T("InitESL_Mgr Error!"));
		return FALSE;
	}

	this->SetCONNECTION_INFO( this->m_pInfo->CONNECTION_INFO );

	ChangeScreenForAcqCode();

	// KOL.RED.2018.220 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//등록번호_까지|E|1124|||||||
	//등록번호_시작|E|1325|||||||
	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_BOOK_TRADE_SEARCH"), _T("등록번호_시작"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}

	{
		CESL_ControlEdit* pCtrl = (CESL_ControlEdit*)FindControl(_T("CM_BO_BOOK_TRADE_SEARCH"), _T("등록번호_까지"));
		ASSERT(pCtrl);

		pCtrl->SetKLRegnoMode(TRUE);

		pCtrl->SetRegCodeLength(m_pInfo->m_nRegCodeLength);
	}
	// KOL.RED.2018.220 ---------------------------------------------------------------------------	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_REG_EPC_SEARCH::OnBtnClose() 
{
	this->OnCancel();
}

VOID CBO_REG_EPC_SEARCH::OnBtnSearch() 
{
EFS_BEGIN

	SearchRegEpcList();

EFS_END
}

VOID CBO_REG_EPC_SEARCH::OnBtnSeqInfo() 
{
EFS_BEGIN

	ShowSeqInfo();

EFS_END
}

VOID CBO_REG_EPC_SEARCH::OnSelchangeCbEpcdataAcqCode() 
{
EFS_BEGIN

	ChangeScreenForAcqCode();

EFS_END
}

VOID CBO_REG_EPC_SEARCH::OnBtnClear() 
{
EFS_BEGIN

	ClearForAllControl();

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CBO_REG_EPC_SEARCH func

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  컨트롤에 입력된 값으로 Where절을 생성한다.
// [Mini Spec]
//  1. 컨트롤에 입력된 값을 저장
//  2. 쿼리 생성
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::MakeWhereFromControl( CString &strGetWhere )
{
EFS_BEGIN

	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	CString strCMAlias;
	const INT nControlCnt = 10;
	CString strControlAlias[nControlCnt] = {
		_T("자료상태"),
		_T("입수구분"),
		_T("수입년도"),
		_T("업무구분"),
		_T("차수번호"),
		_T("작업번호"),
		_T("원부년도"),
		_T("원부번호"),
		_T("등록번호_시작"),
		_T("등록번호_까지")
	};
	CString strControlData[nControlCnt];
	CString strBuf;
	CString strError;

	/////////////////////////////////////////////////////////////////////////
	// 1. 컨트롤에 입력된 값을 저장
	/////////////////////////////////////////////////////////////////////////
	strCMAlias = _T("CM_BO_BOOK_TRADE_SEARCH");
	
	for( INT i=0;i<nControlCnt;i++ )
	{
		ids = GetControlData( strCMAlias, strControlAlias[i], strControlData[i] );
		if( 0 > ids )
		{			
			strError.Format( _T("CBO_REG_EPC_SEARCH::MakeWhereFromControl() ERROR!![%s:%d]"), strControlAlias[i] );
			AfxMessageBox( strError );
			return ids;
		}

		strControlData[i].TrimLeft();
		strControlData[i].TrimRight();
	}
	
	/////////////////////////////////////////////////////////////////////////
	// 2. 쿼리 생성
	/////////////////////////////////////////////////////////////////////////
	strGetWhere.Empty();

	/// 자료상태
	if( 0 < strControlData[0].GetLength() )
	{
		if( _T("전체") == strControlData[0] )
		{
		}
		else if( _T("정리중자료") == strControlData[0] )
		{
			strBuf = _T(" BB.WORKING_STATUS < 'BOL112N' ");
		}
		else if( _T("소장자료") == strControlData[0] )
		{
			strBuf = _T(" BB.WORKING_STATUS > 'BOL111O' ");
		}

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// 입수구분
	if( 0 < strControlData[1].GetLength() )
	{
		strBuf = _T(" BB.ACQ_CODE='") + strControlData[1] + _T("' ");

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// 수입년도
	if( 0 < strControlData[2].GetLength() )
	{
		strBuf = _T(" BB.ACQ_YEAR='") + strControlData[2] + _T("' ");

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// 업무구분
	if( 0 < strControlData[3].GetLength() )
	{
		strBuf = _T(" IB.WORK_CODE='") + strControlData[3] + _T("' ");

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	if( 0 < strGetWhere.GetLength() )
	{
		strGetWhere += _T(" AND ");
	}
	strGetWhere += _T("IB.MANAGE_CODE = UDF_MANAGE_CODE");


	/// 차수번호 && 작업번호
	if( 0 < strControlData[4].GetLength() )
	{
		if( 0 < strControlData[5].GetLength() && _T("적용안함") != strControlData[5] )
		{
			strBuf = _T(" BB.ACQ_KEY IN ( SELECT REC_KEY FROM BO_PURCHASEINFO_TBL WHERE ORDER_SEQ_NO IN (") + strControlData[4] + _T(") AND ORDER_WORK_NO = ") + strControlData[5] + _T(") ");
		}
		else
		{
			strBuf = _T(" BB.ACQ_KEY IN ( SELECT REC_KEY FROM BO_PURCHASEINFO_TBL WHERE ORDER_SEQ_NO IN (") + strControlData[4] + _T(") ) ");
		}

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// 원부년도 && 원부번호	
	if( 0 < strControlData[6].GetLength() && 0 < strControlData[7].GetLength() )
	{
		strBuf = _T(" CA.ACCESSION_REC_MAKE_YEAR ='") + strControlData[6] + _T("' AND CA.ACCESSION_REC_NO =") + strControlData[7] + _T(" ");

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	/// 등록번호 시작 - 까지
	//=====================================================
	//2008.11.04 ADD BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	//=====================================================


	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	if( 2 < strControlData[8].GetLength() )
// 	{
// 		CString strRegCode,strRegNo,strBuf;
// 		if( 12 > strControlData[8].GetLength() )
// 		{
// 			strRegCode = strControlData[8].Mid( 0, 2 );
// 			strRegNo   = strControlData[8].Mid( 2 );
// 			
// 			for( INT i=0;i<(10-strRegNo.GetLength());i++ )
// 			{
// 				strBuf = _T("0") + strBuf;
// 			}
// 
// 			strControlData[8] = strRegCode + strBuf + strRegNo;
// 		}
// 
// 		if( 2 < strControlData[9].GetLength() )
// 		{
// 			if( 12 > strControlData[9].GetLength() )
// 			{
// 				strRegCode = strControlData[9].Mid( 0, 2 );
// 				strRegNo   = strControlData[9].Mid( 2 );
// 				strBuf     = _T("");
// 
// 				for( INT i=0;i<(10-strRegNo.GetLength());i++ )
// 				{
// 					strBuf = _T("0") + strBuf;
// 				}
// 
// 				strControlData[9] = strRegCode + strBuf + strRegNo;
// 			}
// 
// 			strBuf = _T(" BB.REG_NO BETWEEN '") + strControlData[8] + _T("' AND '") + strControlData[9] + _T("' ");
// 		}
// 		else
// 		{
// 			strBuf = _T(" BB.REG_NO = '") + strControlData[8] + _T("' ");
// 		}

	if( nRegCodeLength < strControlData[8].GetLength() )
	{
		CString strRegCode,strRegNo,strBuf;
		if( 12 > strControlData[8].GetLength() )
		{
			strRegCode = strControlData[8].Mid( 0, nRegCodeLength );
			strRegNo   = strControlData[8].Mid( nRegCodeLength );
			strControlData[8] = m_pInfo->MakeRegNo(strRegCode, strRegNo);			
		}

		if( nRegCodeLength < strControlData[9].GetLength() )
		{
			if( 12 > strControlData[9].GetLength() )
			{
				strRegCode = strControlData[9].Mid( 0, nRegCodeLength );
				strRegNo   = strControlData[9].Mid( nRegCodeLength );
				strBuf     = _T("");

				strControlData[9] = m_pInfo->MakeRegNo(strRegCode, strRegNo);
			}

			strBuf = _T(" BB.REG_NO BETWEEN '") + strControlData[8] + _T("' AND '") + strControlData[9] + _T("' ");
		}
		else
		{
			strBuf = _T(" BB.REG_NO = '") + strControlData[8] + _T("' ");
		}
	//=====================================================
	

		if( 0 < strGetWhere.GetLength() )
		{
			strGetWhere += _T(" AND ");
		}

		strGetWhere += strBuf;
	}

	return 0;

EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  수입 구분 코드값에 따른 화면 변경
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::ChangeScreenForAcqCode()
{
EFS_BEGIN
	
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	CString strAcqCode;

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("입수구분"), strAcqCode );
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::ChangeScreenForAcqCode() ERROR!!") );
		return ids;
	}
	
	if( _T("1") == strAcqCode )
	{		
		( GetDlgItem(IDC_STA_EPCDATA_SEQ_NO) )->SetWindowText( _T("차수번호") );
		( GetDlgItem(IDC_EDT_EPCDATA_SEQ_NO) )->EnableWindow( TRUE );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->SetWindowText( _T("작업번호") );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->ShowWindow( SW_SHOW );
		( GetDlgItem(IDC_CB_EPCDATA_WORK_NO) )->ShowWindow( SW_SHOW );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->ShowWindow( SW_SHOW );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->SetWindowText( _T("차수번호보기") );
	}
	else if( _T("2") == strAcqCode )
	{
		( GetDlgItem(IDC_STA_EPCDATA_SEQ_NO) )->SetWindowText( _T("작업번호") );
		( GetDlgItem(IDC_EDT_EPCDATA_SEQ_NO) )->EnableWindow( TRUE );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->ShowWindow( SW_HIDE );
		( GetDlgItem(IDC_CB_EPCDATA_WORK_NO) )->ShowWindow( SW_HIDE );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->ShowWindow( SW_SHOW );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->SetWindowText( _T("접수번호보기") );
	}
	else
	{
		( GetDlgItem(IDC_STA_EPCDATA_SEQ_NO) )->SetWindowText( _T("차수번호") );
		( GetDlgItem(IDC_EDT_EPCDATA_SEQ_NO) )->EnableWindow( FALSE );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->SetWindowText( _T("작업번호") );
		( GetDlgItem(IDC_STA_EPCDATA_WORK_NO) )->ShowWindow( SW_HIDE );
		( GetDlgItem(IDC_CB_EPCDATA_WORK_NO) )->ShowWindow( SW_HIDE );
		( GetDlgItem(IDC_BTN_SEQ_INFO) )->ShowWindow( SW_HIDE );		
	}

	return 0;

EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  차수번호/접수번호 정보 다이얼로그 팝업
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::ShowSeqInfo()
{
EFS_BEGIN
	
	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	CString strAcqCode;
	CString strAcqYear;
	CString strWorkCode;
	CGroupCollection Groups(this);
	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("입수구분"), strAcqCode );
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::ShowSeqInfo() ERROR!! -1") );
		return ids;
	}
	
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("수입년도"), strAcqYear);
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::ShowSeqInfo() ERROR!! -2") );
		return ids;
	}
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("업무구분"), strWorkCode);
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::ShowSeqInfo() ERROR!! -3") );
		return ids;
	}

	if( _T("1") == strAcqCode )
	{
		Groups.SetAcqType( (ACQ_TYPE)(_ttoi(strAcqCode)) );
		CDivisionNo_List_Dlg dlg(this);
		dlg.SetWorkStatus(_T("검수완료"));
		dlg.SetSpeciesType(BO_REG_PURCHASE);
		dlg.SetAcquisitYear(strAcqYear);
		dlg.SetWorkCode(strWorkCode);
		dlg.SetGroupCollection(&Groups);
		if(IDOK == dlg.DoModal())
		{	
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("차수번호"), dlg.GetSelDivNo());
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("수입년도"), Groups.GetAcqYear());
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("업무구분"), Groups.GetWorkCode());			
		}

		MakeWorkNoInfo();
	}
	else if( _T("2") == strAcqCode )
	{
		Groups.SetAcqType( (ACQ_TYPE)(_ttoi(strAcqCode)) );

		CRequisitionNo_List_Dlg dlg(this);
		dlg.SetSpeciesType(BO_REG_DONATE);
		dlg.SetAcqYear(strAcqYear);
		dlg.SetGroupCollection(&Groups);
		if(IDOK == dlg.DoModal())
		{
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("차수번호"), dlg.GetSelInfo());
			SetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("수입년도"), Groups.GetAcqYear());			
		}
	}

	return 0;

EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  작업번호 정보를 생성한다.
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::MakeWorkNoInfo()
{
EFS_BEGIN

	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		  ids;
	CString   strSeqNo;
	CString   strAcqYear;
	CString   strWorkCode;
	CString   strBuf;
	CGroupMgr GroupMgr(this);

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("차수번호"), strSeqNo );
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -1") );
		return ids;
	}
	
	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("수입년도"), strAcqYear);
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -2") );
		return ids;
	}

	ids = GetControlData( _T("CM_BO_BOOK_TRADE_SEARCH"), _T("업무구분"), strWorkCode);
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -3") );
		return ids;
	}

	GroupMgr.InitTempData();
	GroupMgr.SetTempData(_T("AcqYear"),		strAcqYear.GetBuffer(0)	);
	GroupMgr.SetTempData(_T("DivNo"),		strSeqNo.GetBuffer(0)	);
	GroupMgr.SetTempData(_T("WorkCode"),	strWorkCode.GetBuffer(0));

	ids = GroupMgr.SPFExecutePath(_T("마지막작업번호추출"));
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -4") );
		return ids;
	}
	
	/////////////////////////////////////////////////////////////////////////
	// 2. 컨트롤에 입력
	/////////////////////////////////////////////////////////////////////////
	CESL_ControlMgr* pCM = FindCM( _T("CM_BO_BOOK_TRADE_SEARCH") );
	if( !pCM )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -5") );
		return -1;
	}

	CESL_ControlMultiComboBox* pCMB = (CESL_ControlMultiComboBox *)pCM->FindControl( _T("작업번호") );
	if( !pCMB )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::MakeWorkNoInfo() ERROR!! -6") );
		return -1;
	}

	pCMB->InitMultiComboBox();

	pCMB->SetData( &CString(_T("적용안함")), 1 );
	
	for( INT i=1;i<=ids;i++ )
	{
		strBuf.Format( _T("%d"), i );
		pCMB->SetData( &strBuf, 1 );
	}
	
	pCMB->SetCurSel( 0 );

	return 0;

EFS_END
	return -1;	
}

INT	CBO_REG_EPC_SEARCH::ClearForAllControl()
{
EFS_BEGIN

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_BOOK_TRADE_SEARCH"));
	
	if (pCM)
	{
		pCM->AllControlClear();			
	}

	return 0;

EFS_END
	return -1;	
}

/////////////////////////////////////////////////////////////////////////////
// [Func Desc]
//  목록 검색
/////////////////////////////////////////////////////////////////////////////
INT CBO_REG_EPC_SEARCH::SearchRegEpcList()
{
EFS_BEGIN
	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	BEGIN_SBL();
	//==============================================

	/////////////////////////////////////////////////////////////////////////
	// Declare Variable
	/////////////////////////////////////////////////////////////////////////
	INT		ids;
	CString strWhere;
	
	/////////////////////////////////////////////////////////////////////////
	// 
	/////////////////////////////////////////////////////////////////////////
	ids = MakeWhereFromControl( strWhere ) ;
	if( 0 > ids )	return ids;

	/////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_BOOK_TRADE_MAIN") );
	if( !pDM )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::SearchRegEpcList() ERROR -1 !!!") );
		return -1;
	}

	ids = pDM->RefreshDataManager( strWhere );
	if( 0 > ids )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::SearchRegEpcList() ERROR -2 !!!") );
		return -2;
	}

	if( 0 == pDM->GetRowCount() )
	{
		AfxMessageBox( _T("검색된 자료가 없습니다.") );
		return 0;
	}
	
	CESL_ControlMgr* pCM = FindCM( _T("CM_BO_BOOK_TRADE_MAIN") );
	if( !pCM )
	{
		AfxMessageBox( _T("CBO_REG_EPC_SEARCH::SearchRegEpcList() ERROR -3 !!!") );
		return -1;
	}
		
	pCM->AllControlDisplay();

	//==============================================
	//2008.07.11 ADD BY PJW : 메인하단에 상태바LOG기록
	INT nRowCount = pDM->GetRowCount();
	END_SBL(nRowCount);
	//==============================================

	AfxMessageBox( _T("검색이 완료되었습니다.") );

	return 0;

EFS_END
	return -1;
}

HBRUSH CBO_REG_EPC_SEARCH::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

BOOL CBO_REG_EPC_SEARCH::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
 		if(VK_RETURN == pMsg->wParam)
		{
			OnBtnSearch();
			return TRUE;
		}
	};
	return CESL_Mgr::PreTranslateMessage(pMsg);
}

void CBO_REG_EPC_SEARCH::OnSetfocusEdtEpcdataRegNo1() 
{
	// TODO: Add your control notification handler code here
	SetIMEStatus(_T("영문"), IDC_EDT_EPCDATA_REG_NO1);
}

void CBO_REG_EPC_SEARCH::OnSetfocusEdtEpcdataRegNo2() 
{
	// TODO: Add your control notification handler code here
	SetIMEStatus(_T("영문"), IDC_EDT_EPCDATA_REG_NO2);
}

int CBO_REG_EPC_SEARCH::SetIMEStatus(CString strMode, UINT uControlID)
{
	HIMC hIME;
    DWORD dwConversion, dwSentence;
	hIME = ::ImmGetContext(GetDlgItem(uControlID)->m_hWnd);
	if(NULL != hIME)
	{
		ImmGetConversionStatus(hIME,&dwConversion,&dwSentence);
		if(_T("한글") == strMode)
		{
			dwConversion=IME_CMODE_HANGUL;
		}
		else if(_T("영문") == strMode)
		{
			dwConversion = dwConversion & ~IME_CMODE_LANGUAGE;
			dwConversion |= IME_CMODE_ALPHANUMERIC;
		}
		else return -1;

		ImmSetConversionStatus(hIME,dwConversion,dwSentence );		
		ImmReleaseContext(GetDlgItem(uControlID)->m_hWnd,hIME); 
	}
	else return -2;

	return 0;
}
