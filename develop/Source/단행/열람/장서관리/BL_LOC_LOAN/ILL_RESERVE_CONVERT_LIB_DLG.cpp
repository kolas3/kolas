// ILL_RESERVE_CONVERT_LIB_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "ILL_RESERVE_CONVERT_LIB_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILL_RESERVE_CONVERT_LIB_DLG dialog


CILL_RESERVE_CONVERT_LIB_DLG::CILL_RESERVE_CONVERT_LIB_DLG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CILL_RESERVE_CONVERT_LIB_DLG::IDD, pParent)
{
	m_pParent = pParent;

	m_strBookLibCode = _T("");
	m_strBookKey     = _T("");
}


void CILL_RESERVE_CONVERT_LIB_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CILL_RESERVE_CONVERT_LIB_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CILL_RESERVE_CONVERT_LIB_DLG, CDialog)
	//{{AFX_MSG_MAP(CILL_RESERVE_CONVERT_LIB_DLG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILL_RESERVE_CONVERT_LIB_DLG message handlers

BOOL CILL_RESERVE_CONVERT_LIB_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(0 > InitESL_Mgr(_T("SM_ILL_RESERVE_CONVERT_LIB_DLG")))
	{
		AfxMessageBox(_T("InitEsl Error!!!"));
		return TRUE;
	}
	
	CESL_DataMgr *pDM = FindDM(_T("DM_ILL_RESERVE_CONVERT_LIB_DLG"));
	pDM->RefreshDataManager(_T(""));

	CESL_ControlMgr *pCM = FindCM(_T("CM_ILL_RESERVE_CONVERT_LIB_DLG"));
	pCM->AllControlDisplay();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CILL_RESERVE_CONVERT_LIB_DLG, CDialog)
    //{{AFX_EVENTSINK_MAP(CILL_RESERVE_CONVERT_LIB_DLG)
	ON_EVENT(CILL_RESERVE_CONVERT_LIB_DLG, IDC_grid_Main, -601 /* DblClick */, OnDblClickgridMain, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CILL_RESERVE_CONVERT_LIB_DLG::PreTranslateMessage(MSG* pMsg) 
{
	if(WM_KEYDOWN == pMsg->message)
	{
		if(pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}
		else if(pMsg->wParam == VK_RETURN )
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CILL_RESERVE_CONVERT_LIB_DLG::OnDblClickgridMain() 
{
	INT ids = 0;
	BOOL bAble = FALSE;
	CString strMsg = _T(""), strFuncName = _T("OnDblClickgridMain()");

	ids = IsILL_Convert_Able( bAble, strMsg);
	if ( ids < 0 ) {
		strMsg.Format(_T("%s Error!![%d]"), ids );
		AfxMessageBox( strMsg );
		return;
	}
	if ( ids == 100 ) {
		AfxMessageBox( strMsg );
		return;
	}
	
	CDialog::OnOK();
}

INT CILL_RESERVE_CONVERT_LIB_DLG::IsILL_Convert_Able(BOOL& bAble, CString& strMsg) 
{
	INT ids = -1, nReqLibIdx = -1, i = -1, nRowCnt = 0;
	CString strILLUseYn = _T(""), strILLOfferYN = _T(""), strManageCode = _T(""), strLibCode = _T(""), strReqLibCode = _T(""), strLibName = _T(""), strQuery = _T(""), strResult = _T("");

	bAble = TRUE;

	CESL_DataMgr *pDM = FindDM(_T("DM_ILL_RESERVE_CONVERT_LIB_DLG"));
	CESL_Grid *pGrid = (CESL_Grid*)FindControl(_T("CM_ILL_RESERVE_CONVERT_LIB_DLG"), _T("MainGrid"));
	nReqLibIdx = pGrid->GetIdx();

	if ( nReqLibIdx < 0 ) return -50;

	if ( m_strBookKey.GetLength() == 0 ) return -55;

	if ( m_strBookLibCode.GetLength() == 0 ) {
		strQuery.Format( _T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL L WHERE L.MANAGE_CODE=(SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REC_KEY=%s ) AND ROWNUM < 2 "), m_strBookKey);
		ids = pDM->GetOneValueQuery(strQuery, m_strBookLibCode);
		if ( ids < 0 ) return -60;
	}	

	ids = pDM->GetCellData(_T("ILL_USE_YN"), nReqLibIdx, strILLUseYn );
	if ( ids < 0 ) return -210;
	if ( strILLUseYn != _T("Y")) {
		bAble = FALSE;
		strMsg.Format(_T("요청관은 책두레를 사용하지 않습니다."));
		return 100;
	}
	
	ids = pDM->GetCellData(_T("LIB_CODE"), nReqLibIdx, strReqLibCode );
	if ( ids < 0 ) return -220;
	if ( strReqLibCode == m_strBookLibCode) {
		bAble = FALSE;
		strMsg.Format(_T("자관으로 책두레전환은 불가능합니다."));
		return 100;
	}

	nRowCnt = pDM->GetRowCount();
	for( i =0; i< nRowCnt; i++ ) {
		ids = pDM->GetCellData(_T("LIB_CODE"), i, strLibCode );
		if ( ids < 0 ) return -100;

		if ( strLibCode != m_strBookLibCode) continue;

		ids = pDM->GetCellData(_T("ILL_USE_YN"), i, strILLUseYn );
		if ( ids < 0 ) return -110;
		if ( strILLUseYn != _T("Y")) {
			bAble = FALSE;
			strMsg.Format(_T("소장관은 책두레를 사용하지 않습니다."));
			return 100;
		}

		ids = pDM->GetCellData(_T("ILL_OFFER_YN"), i, strILLOfferYN );
		if ( ids < 0 ) return -120;
		if ( strILLOfferYN != _T("Y")) {
			bAble = FALSE;
			strMsg.Format(_T("소장관은 책두레제공을 하지 않습니다."));
			return 100;
		}

		strQuery.Format(_T("SELECT '1'||IS_ILL_APPLICANT('SHELF_LOC_CODE',(SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REC_KEY=%s),(SELECT SHELF_LOC_CODE FROM BO_BOOK_TBL WHERE REC_KEY=%s) )|| ")
			                        _T("IS_ILL_APPLICANT('REG_CODE'      ,(SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REC_KEY=%s),(SELECT REG_CODE FROM BO_BOOK_TBL WHERE REC_KEY=%s) )|| ")
			                        _T("IS_ILL_APPLICANT('SE_SHELF_CODE' ,(SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REC_KEY=%s),(SELECT SEPARATE_SHELF_CODE FROM BO_BOOK_TBL WHERE REC_KEY=%s) ) AS ILL_APP_ABLE ")
			            _T("FROM DUAL")
			, m_strBookKey, m_strBookKey, m_strBookKey, m_strBookKey, m_strBookKey, m_strBookKey );

		ids = pDM->GetOneValueQuery(strQuery, strResult);
		if ( ids < 0 ) return -150;
		if ( strResult != _T("1YYY") ) {
			bAble = FALSE;
			strMsg.Format(_T("소장관에서 책두레허용 자료실/등록구분/별치기호로 설정되지 않은 자료 입니다."));
			return 100;
		}

		strQuery.Format( _T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REC_KEY=%s "), m_strBookKey);
		ids = pDM->GetOneValueQuery(strQuery, strResult);
		if ( ids < 0 ) return -150;
		if ( strResult != _T("BOL112N") ) {
			bAble = FALSE;
			strMsg.Format(_T("해당자료는 비치상태가 아니여서 책두레전환이 불가능 합니다."));
			return 100;
		}

		break;
	}	

	ids = pDM->GetCellData(_T("MANAGE_CODE"), nReqLibIdx, m_strRequestManageCode );
	if ( ids < 0 ) return -230;	
	m_strRequestLibCode = strReqLibCode;
	ids = pDM->GetCellData(_T("LIB_NAME"), nReqLibIdx, m_strRequestLibName );
	if ( ids < 0 ) return -240;		
	
	return 0;
}

void CILL_RESERVE_CONVERT_LIB_DLG::SetBookParm(CString strLibCode, CString strBookKey)
{
	m_strBookLibCode = strLibCode;
	m_strBookKey     = strBookKey;
}
