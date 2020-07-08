// ILL_APL.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ILL_APL_3100.h"
#include "ILL_SEARCH_BOOK.h"
#include "ILL_API_APPENDIX_DLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILL_APL_3100 dialog


CILL_APL_3100::CILL_APL_3100(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CILL_APL_3100)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsAutoRequest = FALSE;
	m_bIsVolReserve = FALSE;	
	m_pLoanShareManager = NULL;
	m_pLoanMangeValue = NULL;

	m_bConstAplCount = FALSE;	
	m_nMode = 0;
	m_strReservationKey = _T("");

	m_bAplOK = FALSE;
}


void CILL_APL_3100::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CILL_APL_3100, CDialog)
	//{{AFX_MSG_MAP(CILL_APL_3100)
	ON_BN_CLICKED(IDC_btnDDFINDUSER, OnbtnDDFINDUSER)
	ON_BN_CLICKED(IDC_btnDDFINDBOOK, OnbtnDDFINDBOOK)
	ON_BN_CLICKED(IDC_btnDDCLOSE, OnbtnDDCLOSE)
	ON_BN_CLICKED(IDC_btnAPL, OnbtnAPL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILL_APL_3100 message handlers

void CILL_APL_3100::OnbtnDDFINDUSER() 
{
}

void CILL_APL_3100::OnbtnDDFINDBOOK() 
{
	// TODO: Add your control notification handler code here
	CILL_SEARCH_BOOK Dlg(this);
	Dlg.m_pInfo = m_pInfo;
	Dlg.m_sBookCooperativeMode = m_sBookCooperativeMode;
	Dlg.DoModal();
}

BOOL CILL_APL_3100::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::OnInitDialog()");

	// TODO: Add extra initialization here
	ids = InitESL_Mgr(_T("ILL_APL"));
	if(0 > ids)
	{
		AfxMessageBox(_T("ERROR : InitESL_Mgr(\"ILL_APL\")"));
		CDialog::OnCancel();
	}
	
	// TODO: Add extra initialization here
	EnableThemeDialogTexture(GetSafeHwnd()); 

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(
		_T("SELECT LIB_NAME, LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"),
		m_pInfo->MANAGE_CODE);
	
	CString strValue;
	ids = TmpDM.RestructDataManager(strQuery);
	if(0 > ids) return FALSE;

	ids = TmpDM.GetCellData(0, 0, strValue);

	CESL_ControlEdit* pEdit = (CESL_ControlEdit*)FindControl(_T("CM_ILL_APL"), _T("신청_요청도서관이름"));
	if(NULL == pEdit) return FALSE;
	
	if(2 == m_nMode)
	{
		if(_T("") == m_strRequestLibName)
		{
			AfxMessageBox(_T("데이터가 잘못되었습니다."));
			return FALSE;
		}
		strValue = m_strRequestLibName;
	}
	ids = pEdit->SetData(strValue);
	if(0 > ids) return FALSE;

	ids = TmpDM.GetCellData(0, 1, strValue);

	pEdit = (CESL_ControlEdit*)FindControl(_T("CM_ILL_APL"), _T("신청_요청도서관코드"));
	if(NULL == pEdit) return FALSE;
	

	if(2 == m_nMode)
	{
		if(_T("") == m_strRequestLibCode)
		{
			AfxMessageBox(_T("데이터가 잘못되었습니다."));
			return FALSE;
		}
		strValue = m_strRequestLibCode;
	}

	ids = pEdit->SetData(strValue);
	if(0 > ids) return FALSE;
	
	if(2 == m_nMode)
	{
		pEdit = (CESL_ControlEdit*)FindControl(_T("CM_ILL_APL"), _T("신청_제공도서관이름"));
		if(NULL == pEdit) return FALSE;
		
		TmpDM.GetCellData(0, 0, strValue);
		ids = pEdit->SetData(strValue);
		if(0 > ids) return FALSE;
		
		pEdit = (CESL_ControlEdit*)FindControl(_T("CM_ILL_APL"), _T("신청_제공도서관코드"));
		if(NULL == pEdit) return FALSE;
		
		TmpDM.GetCellData(0, 1, strValue);
		ids = pEdit->SetData(strValue);
		if(0 > ids) return FALSE;
	}

	{
	int ids = 0;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery;
	strQuery.Format(
			_T("SELECT VALUE_1 ")
					_T("FROM ILL_MANAGE_TBL ")
			_T("WHERE VALUE_NAME = '신청시자동요청기능사용유무' AND MANAGE_CODE = '%s'"),
			m_pInfo->MANAGE_CODE);
	
	CString strValue;
	ids = TmpDM.GetOneValueQuery(strQuery, strValue);
	if(0 > ids) return FALSE;
	
	m_bIsAutoRequest = (_T("Y") == strValue) ? TRUE : FALSE;
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_ILL_IS_APL_CODE"));
	if(NULL == pDM)
	{
		CDialog::OnCancel();
		return FALSE;
	}
	pDM->RefreshDataManager(_T(""));

	{
	CESL_ControlEdit* pEdit = NULL;
	pEdit = (CESL_ControlEdit*)FindControl(_T("CM_ILL_APL"), _T("신청_이용자의견"));
	if(NULL != pEdit)
	{
		pEdit->SetLimitText(300);
	}
	pEdit = (CESL_ControlEdit*)FindControl(_T("CM_ILL_APL"), _T("신청_비고"));
	if(NULL != pEdit)
	{
		pEdit->SetLimitText(300);
	}
	}

	if(1 == m_nMode || 2 == m_nMode)
	{
		GetDlgItem(IDC_btnDDFINDUSER)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_edt_ILL_APL_USER_NO)->SetWindowText(m_strUserNo);
		GetDlgItem(IDC_edt_ILL_APL_USER_NO2)->SetWindowText(m_strUserNo2);
		GetDlgItem(IDC_edt_ILL_APL_APL_USERKEY)->SetWindowText(m_strUserKey);
		GetDlgItem(IDC_edt_ILL_APL_USER_EMAIL)->SetWindowText(m_strUserEmail);
		GetDlgItem(IDC_edt_ILL_APL_USER_CLASSCODE)->SetWindowText(m_strUserCode);
		GetDlgItem(IDC_edt_ILL_APL_USER_NAME)->SetWindowText(m_strUserName);
		GetDlgItem(IDC_edt_ILL_APL_APL_BOOKKEY)->SetWindowText(m_strBookKey);
		GetDlgItem(IDC_edt_ILL_APL_BOOK_TITLE)->SetWindowText(m_strBookTitle);
		GetDlgItem(IDC_edt_ILL_APL_BOOK_REG_NO)->SetWindowText(m_strBookRegNo);
		GetDlgItem(IDC_edt_ILL_APL_BOOK_MANAGECODE)->SetWindowText(m_strBookManageCode);
		GetDlgItem(IDC_edt_ILL_APL_PUBLISH_FORM)->SetWindowText(m_strPublishFormCode);
		
		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

		CString strQuery;
		CString strLibName, strLibCode;
		strQuery.Format(_T("SELECT LIB_CODE, LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), m_strBookManageCode);
		TmpDM.RestructDataManager(strQuery);
		
		strLibCode = TmpDM.GetCellData(0, 0);
		strLibName = TmpDM.GetCellData(0, 1);

		GetDlgItem(IDC_edt_ILL_APL_GIVE_LIBCODE)->SetWindowText(strLibCode);
		GetDlgItem(IDC_edt_ILL_APL_GIVE_LIBNAME)->SetWindowText(strLibName);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CILL_APL_3100::OnbtnDDCLOSE() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CILL_APL_3100::OnbtnAPL() 
{
	if ( IDNO == MessageBox( _T("예약을 취소하고 책두레 신청자료로 전환하시겠습니까?"), NULL, MB_YESNO|MB_ICONQUESTION ) )
	{
		return;
	}

	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::OnbtnAPL() ");

	CESL_ControlMgr* pCM = FindCM(_T("CM_ILL_APL"));
	if(NULL == pCM) ERROR_MSG_RETURN_VOID2(-1, strFuncName);

	CESL_ControlEdit* pEdit = NULL;

	ids = Check_Data();
	if(0 > ids) return;

	CString strBookManageCode;
	CString strUserClassCode;

	ids = GetControlData(_T("CM_ILL_APL"), _T("이용자_직급코드"), strUserClassCode);
	if(0 > ids ) ERROR_MSG_RETURN_VOID2(-2, strFuncName);

	
	ids = GetControlData(_T("CM_ILL_APL"), _T("책_관리구분"), strBookManageCode);
	if(0 > ids ) ERROR_MSG_RETURN_VOID2(-3, strFuncName);

	CString strUserNo;
	ids = GetControlData(_T("CM_ILL_APL"), _T("이용자_이용자번호"), strUserNo);
	if(0 > ids ) ERROR_MSG_RETURN_VOID2(-4, strFuncName);

	CString strUserKey;
	ids = GetControlData(_T("CM_ILL_APL"), _T("이용자_키"), strUserKey);
	if(0 > ids ) ERROR_MSG_RETURN_VOID2(-4, strFuncName);

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString strQuery;

	CString strMaxAplCnt;
	
	CString strTemp;
	
	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME = '책두레사용여부' AND MANAGE_CODE = '%s' AND ROWNUM < 2"),strBookManageCode);
	ids = TmpDM.GetOneValueQuery(strQuery, strTemp);
	if(0 > ids || strTemp != _T("Y")){
		AfxMessageBox(_T("해당 책의 소장관은 책두레를 이용하지 않습니다."));
		return;
	}

	strQuery.Format(_T("SELECT VALUE_1 FROM ILL_MANAGE_TBL WHERE VALUE_NAME = '제공기능사용유무' AND MANAGE_CODE = '%s' AND ROWNUM < 2"),strBookManageCode);
	ids = TmpDM.GetOneValueQuery(strQuery, strTemp);
	if(0 > ids || strTemp != _T("Y")){
		AfxMessageBox(_T("해당 책의 소장관은\r\n\r\n책두레 이용시 책을 제공하지 않습니다."));
		return;
	}

	strQuery.Format(
		_T("SELECT MAX(VALUE_1) FROM ")
		_T("ILL_MANAGE_TBL WHERE VALUE_NAME = '책두레최대신청가능책수'"));	
	ids = TmpDM.GetOneValueQuery(strQuery, strMaxAplCnt);
	if(0 > ids)
	{
		m_bConstAplCount = FALSE;
	}
	else
	{
		m_bConstAplCount = TRUE;

		CESL_ControlEdit* pEdit = NULL;
		
		CString strBookKey;
		pEdit = (CESL_ControlEdit*)pCM->FindControl(_T("책_키"));
		if(NULL == pEdit) ERROR_MSG_RETURN_VOID2(-6, strFuncName);	
		pEdit->GetData(strBookKey);	
		
		CString strWorkingStatus;
		strQuery.Format(_T("SELECT WORKING_STATUS FROM BO_BOOK_TBL WHERE REC_KEY = %s"),strBookKey);
		TmpDM.GetOneValueQuery(strQuery,strWorkingStatus);
		
		BOOL bIsBookStatus = TRUE;
		
		if(strWorkingStatus.Compare(_T("BOL112N"))!=0)	bIsBookStatus = FALSE;
		
		if(FALSE == bIsBookStatus)
		{
			AfxMessageBox(_T("해당 책은 비치중인 자료가 아니므로 신청할 수 없습니다."));
			return;
		}

		CString strExpireDate;
		strQuery.Format(_T("SELECT RESERVATION_EXPIRE_DATE FROM LS_WORK_T01 WHERE BOOK_KEY = %s AND USER_KEY = %s AND STATUS='3'"),strBookKey,strUserKey);
		TmpDM.GetOneValueQuery(strQuery,strExpireDate);
		
		BOOL bIsReservationExpireDate = TRUE;
		if(strExpireDate == _T("")) bIsReservationExpireDate = FALSE;
		
		if(FALSE == bIsReservationExpireDate)
		{
			AfxMessageBox(_T("해당 이용자는 예약만기일이 부여되어 있지 않아 전환할 수 없습니다."));
			return;
		}
	
		CString strLibTotalLoanCanCnt;
		strQuery.Format(								
			_T("SELECT LOAN_BOOK_CNT FROM CD_INDIVIDUAL_CLASS_TBL WHERE CODE = '%s' AND MANAGE_CODE = '%s'"),
			strUserClassCode, strBookManageCode);
		TmpDM.GetOneValueQuery(strQuery, strLibTotalLoanCanCnt);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-6, strFuncName);
		
		CString strLoanCount;
		strQuery.Format(_T("SELECT COUNT(1) FROM LS_WORK_T01 WHERE USER_KEY = %s AND MANAGE_CODE = '%s' AND STATUS IN ('0', '2', 'L', 'D')"), strUserKey, strBookManageCode);		
		ids = TmpDM.GetOneValueQuery(strQuery, strLoanCount);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-7, strFuncName);
		if(_ttoi(strLoanCount) >= strLibTotalLoanCanCnt)
		{
			AfxMessageBox(_T("이용자의 제공도서관 대출책수가 초과되어 신청할 수 없습니다."));
			return;
		}

		CString strILLCnt;

		CString strILLCntSpecies;
		strQuery.Format(_T("SELECT MAX(VALUE_1) FROM ILL_MANAGE_TBL WHERE VALUE_NAME = '신청권수범위설정'"));
		TmpDM.GetOneValueQuery(strQuery,strILLCntSpecies);

		if(strILLCntSpecies.Compare(_T("N"))==0)
		{		
			strQuery.Format(
				_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE ")
				_T("USER_NO = '%s' ")
				_T("AND END_DIV IS NULL AND RETURN_LIBCODE IS NULL AND LOAN_KEY IS NULL")
				_T(" AND BOOK_APPENDIX_FLAG != 'A'")			
				_T(" AND STATE != '0008'")		
				, strUserNo);
		}
		else
		{
			strQuery.Format(
				_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE ")
				_T("USER_NO = '%s' ")			
				_T("AND END_DIV IS NULL AND RETURN_LIBCODE IS NULL")
				_T(" AND BOOK_APPENDIX_FLAG != 'A'")			
				_T(" AND STATE != '0008'")		
				, strUserNo);		
		}		

		ids = TmpDM.GetOneValueQuery(strQuery, strILLCnt);
		if(0 > ids) ERROR_MSG_RETURN_VOID2(-7, strFuncName);

		strMaxAplCnt = (_ttoi(strLibTotalLoanCanCnt)>_ttoi(strMaxAplCnt))? strMaxAplCnt : strLibTotalLoanCanCnt;

		BOOL bIsApl = TRUE;

		int nIsAplCnt = _ttoi(strMaxAplCnt) - _ttoi(strILLCnt);
		if(0 >= nIsAplCnt) { bIsApl = FALSE; }
	
		if(FALSE == bIsApl)
		{
			AfxMessageBox(_T("이용자의 신청한도가 넘어 신청할 수 없습니다."));
			return;
		}
	}
	
	TmpDM.StartFrame();

	CString strTransQuery;
	ids = Get_Insert_TransTbl_Query(strTransQuery);
	if(0 > ids) 
	{
		return;
	}	
	TmpDM.AddFrame(strTransQuery);

	CString strBookQuery;
	ids = Get_Update_BookTbl_Query(strBookQuery);
	if(0 > ids) 
	{
		return;
	}
	strBookQuery.Replace(_T("@ANOTHER_LIB"), _T("@") + strBookManageCode);
	TmpDM.AddFrame(strBookQuery);

	CString strIdxQuery;
	ids = Get_UpdateIdxTblQuery(strIdxQuery);
	if(0 > ids)
	{
		return;
	}
	strIdxQuery.Replace(_T("@ANOTHER_LIB"), _T("@") + strBookManageCode);
	TmpDM.AddFrame(strIdxQuery);


	ids = TmpDM.SendFrame();
	TmpDM.EndFrame();
	if(0 > ids)
	{
		return;
	}

	if(_T("D") == m_sBookCooperativeMode && ids >= 0)
	{
	
	}

	m_bAplOK = TRUE;

	CDialog::OnCancel();
}

int CILL_APL_3100::Check_Data()
{
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::Check_Data() ");

	CESL_ControlMgr* pCM = FindCM(_T("CM_ILL_APL"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CESL_ControlEdit* pEdit = NULL;

	const int ALIAS_CNT = 5;
	CString strAlias[ALIAS_CNT] = 
	{
		_T("이용자_이용자번호"),
		_T("책_등록번호"),
		_T("책_발행형태구분"),
		_T("신청_요청도서관코드"),
		_T("신청_제공도서관코드")
	};
	CString strDBField[ALIAS_CNT] = 
	{
		_T("USER_NO"),
		_T("REG_NO"),
		_T("PUBLISH_FORM_CODE"),
		_T("APL_LIBCODE"),
		_T("GIVE_LIBCODE")
	};
	CString strMessage[ALIAS_CNT] = 
	{
		_T("이용자번호"),
		_T("등록번호"),
		_T("발행형태구분"),
		_T("요청도서관부호"),
		_T("제공도서관부호")
	};

	CString strGet;
	for(int nCnt = 0; nCnt < ALIAS_CNT; nCnt++)
	{
		pEdit = (CESL_ControlEdit*)pCM->FindControl(strAlias[nCnt]);
		if(NULL == pEdit) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);

		pEdit->GetData(strGet);

		if(_T("") == strGet)
		{
			CString strMsg;
			strMsg.Format(
				_T("신청에 필요한 필수정보[%s]가 없습니다."),
				strMessage[nCnt]);

			if(_T("요청도서관코드") == strMessage[nCnt] || _T("제공도서관코드") == strMessage[nCnt])
			{
				strMsg = strMsg + _T("\r\n\r\n[공통] - [관리] - [시스템환경설정] - [도서관정보설정]에서\r\n해당 도서관의 도서관부호가 설정되어 있는지 확인해보시기 바랍니다.");
			}
			AfxMessageBox(strMsg);

			return -1;
		}
	}
	return 0;
}

int CILL_APL_3100::Get_Insert_TransTbl_Query(CString &strQuery)
{
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::Get_Insert_TransTbl_Query() ");

	CESL_ControlMgr* pCM = FindCM(_T("CM_ILL_APL"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CESL_ControlEdit* pEdit = NULL;

	CString strColumnQuery;
	CString strValueQuery;
	{
	const int ALIAS_CNT = 17;
	CString strAlias[ALIAS_CNT] = 
	{
		_T("이용자_이용자번호"),
		_T("이용자_키"),
		_T("책_키"),
		_T("책_권"),
		_T("책_등록번호"),
		_T("책_발행년도"),
		_T("책_발행자"),
		_T("책_발행형태구분"),
		_T("책_서명"),
		_T("책_저자"),
		_T("책_청구기호"),
		_T("모체_키"),
		_T("부록여부"),	
		_T("신청_요청도서관코드"),		
		_T("신청_제공도서관코드"),
		_T("신청_비고"),
		_T("신청_이용자의견")
	};
	CString strDBField[ALIAS_CNT] = 
	{
		_T("USER_NO"),
		_T("USER_KEY"),
		_T("BOOK_KEY"),
		_T("VOL_INFO"),
		_T("REG_NO"),
		_T("PUB_YEAR_INFO"),
		_T("PUBLISHER_INFO"),
		_T("PUBLISH_FORM_CODE"),
		_T("TITLE_INFO"),
		_T("AUTHOR_INFO"),
		_T("CALL_NO"),
		_T("SPECIES_KEY"),
		_T("BOOK_APPENDIX_FLAG"),
		_T("APL_LIBCODE"),		
		_T("GIVE_LIBCODE"),
		_T("APL_CONTENT"),
		_T("APL_USER_CONTENT")
	};

	CString strGet;

	CString strQuery = _T("");
	CString strResult = _T("");
	CString strBookKey = _T("");
	CString strBookAppendixFlag = _T("");
	CString strSpeciesKey = _T("");

	for(int nCnt = 0; nCnt < ALIAS_CNT; nCnt++)
	{
 		CESL_DataMgr TmpDM;
 		TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
		pEdit = (CESL_ControlEdit*)pCM->FindControl(strAlias[nCnt]);
		if(NULL == pEdit) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
		}
		pEdit->GetData(strGet);
		if(2 == nCnt)
		{
			strBookKey = strGet;			
			strQuery.Format(_T("SELECT BOOK_APPENDIX_FLAG FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strBookKey);
			TmpDM.GetOneValueQuery(strQuery, strBookAppendixFlag);	
			{
				strQuery.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strBookKey);
				TmpDM.GetOneValueQuery(strQuery, strSpeciesKey);	
			}
		}
		if(11 == nCnt)
		{
			strGet = strSpeciesKey;
		}
		if(12 == nCnt)
		{
			strGet = strBookAppendixFlag;
		}

		if(_T("") == strGet) continue;

		
		strGet.Replace(_T("'"), _T("''"));
		strQuery.Format(_T("'%s'"), strGet);
		strValueQuery = strValueQuery + _T(", ") + strQuery;

		strColumnQuery = strColumnQuery + _T(", ") + strDBField[nCnt];
	}
	}
	strValueQuery.TrimLeft(_T(", "));
	strColumnQuery.TrimLeft(_T(", "));

	CString strData;
	CString strSEQData;

	CESL_DataMgr TmpDM;
 	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.GetOneValueQuery(_T("SELECT TRANS_NO_SEQ.NEXTVAL FROM DUAL"),	strSEQData);
	m_strTransNo = strSEQData;

	{
	const int ALIAS_CNT = 7;
	CString strDBField[ALIAS_CNT] = 
	{	
		_T("TRANS_NO"), 
		_T("STATE"), 
		_T("APL_DIV"), 
		_T("APL_DATE"), 		
		_T("FIRST_WORK"), 
		_T("APLREQ_USERID"), 
		_T("APLREQ_DATE")		
	};
	CString strValue[ALIAS_CNT] = 
	{			
		_T("'")	+ strSEQData + _T("'"),
		(TRUE == m_bIsAutoRequest) ? _T("'0002'") : _T("'0001'"), 
		_T("'0002'"), 
		_T("SYSDATE"),		
		_T("'") + GetWorkLogMsg( (TRUE == m_bIsAutoRequest) ? _T("자동요청") : _T("신청"), __WFILE__, __LINE__ ) + _T("'"),
		_T("'") + m_pInfo->USER_ID + _T("'"),
		_T("SYSDATE")
	};

	const int MAX_CNT = (TRUE == m_bIsAutoRequest) ? 7 : 5;
	for(int nCnt = 0; nCnt < MAX_CNT; nCnt++)
	{		
		strValueQuery = strValueQuery + _T(", ") + strValue[nCnt];
		
		strColumnQuery = strColumnQuery + _T(", ") + strDBField[nCnt];
	}
	}	
	strQuery.Format(_T("INSERT INTO ILL_TRANS_TBL (%s) VALUES (%s);"),
		strColumnQuery, strValueQuery);

	return 0;
}

int CILL_APL_3100::Get_Insert_TransTbl_Query(CString &strQuery, CESL_DataMgr *pDM, INT nRow)
{
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::Get_Insert_TransTbl_Query() ");

	CESL_ControlMgr* pCM = FindCM(_T("CM_ILL_APL"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CESL_ControlEdit* pEdit = NULL;

	CString strColumnQuery;
	CString strValueQuery;
	{
		
	const int ALIAS_CNT = 17;
	CString strAlias[ALIAS_CNT] = 
	{
		_T("이용자_이용자번호"),
		_T("이용자_키"),
		_T("부록_키"),
		_T("부록_권"),
		_T("부록_등록번호"),
		_T("부록_발행년도"),
		_T("부록_발행자"),
		_T("부록_발행형태구분"),
		_T("부록_서명"),
		_T("부록_저자"),
		_T("부록_청구기호"),
		_T("모체_키"),		
		_T("신청_요청도서관코드"),		
		_T("신청_제공도서관코드"),
		_T("신청_비고"),
		_T("신청_이용자의견"),
		_T("신청_부록여부")
	};
	CString strDBField[ALIAS_CNT] = 
	{
		_T("USER_NO"),
		_T("USER_KEY"),
		_T("BOOK_KEY"),
		_T("VOL_INFO"),
		_T("REG_NO"),
		_T("PUB_YEAR_INFO"),
		_T("PUBLISHER_INFO"),
		_T("PUBLISH_FORM_CODE"),
		_T("TITLE_INFO"),
		_T("AUTHOR_INFO"),
		_T("CALL_NO"),
		_T("SPECIES_KEY"),
		_T("APL_LIBCODE"),		
		_T("GIVE_LIBCODE"),
		_T("APL_CONTENT"),
		_T("APL_USER_CONTENT"),
		_T("BOOK_APPENDIX_FLAG")
	};

	CString strGet;
	CString strBookKey = _T("");
	CString strSpeciesKey = _T("");
	for(int nCnt = 0; nCnt < ALIAS_CNT; nCnt++)
	{
		if(2 <= nCnt && 10 >= nCnt)
		{
			strGet = pDM->GetCellData(strAlias[nCnt], nRow);
			if(7 == nCnt)
			{
				strGet = _T("MO");
			}
			else if(2 == nCnt)
			{
				strBookKey = strGet;
			}
		}
		else if(11 == nCnt && _T("") != strBookKey)
		{
			CString strQuery;
			strQuery.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s"), strBookKey);
 			CESL_DataMgr TmpDM;
 			TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
			TmpDM.GetOneValueQuery(strQuery, strSpeciesKey);
			strGet = strSpeciesKey;
		}
		else if(16 == nCnt)
		{
			strGet = _T("A");
		}
		else
		{
			pEdit = (CESL_ControlEdit*)pCM->FindControl(strAlias[nCnt]);
			if(NULL == pEdit) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);
			pEdit->GetData(strGet);
		}
		if(_T("") == strGet) continue;
	
		CString strQuery;		
		strGet.Replace(_T("'"), _T("''"));
		strQuery.Format(_T("'%s'"), strGet);
		strValueQuery = strValueQuery + _T(", ") + strQuery;

		strColumnQuery = strColumnQuery + _T(", ") + strDBField[nCnt];
	}
	}
	strValueQuery.TrimLeft(_T(", "));
	strColumnQuery.TrimLeft(_T(", "));

	CString strData;
	CString strSEQData;

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.GetOneValueQuery(_T("SELECT TRANS_NO_SEQ.NEXTVAL FROM DUAL"),	strSEQData);
	m_strTransNo = strSEQData;

	{
	const int ALIAS_CNT = 7;
	CString strDBField[ALIAS_CNT] = 
	{	
		_T("TRANS_NO"), 
		_T("STATE"), 
		_T("APL_DIV"), 
		_T("APL_DATE"), 		
		_T("FIRST_WORK"), 
		_T("APLREQ_USERID"), 
		_T("APLREQ_DATE")		
	};
	CString strValue[ALIAS_CNT] = 
	{			
		_T("'")	+ strSEQData + _T("'"),
		(TRUE == m_bIsAutoRequest) ? _T("'0002'") : _T("'0001'"), 
		_T("'0002'"), 
		_T("SYSDATE"),		
		_T("'") + GetWorkLogMsg( (TRUE == m_bIsAutoRequest) ? _T("자동요청") : _T("신청"), __WFILE__, __LINE__ ) + _T("'"),
		_T("'") + m_pInfo->USER_ID + _T("'"),
		_T("SYSDATE")
	};

	const int MAX_CNT = (TRUE == m_bIsAutoRequest) ? 7 : 5;
	for(int nCnt = 0; nCnt < MAX_CNT; nCnt++)
	{		
		strValueQuery = strValueQuery + _T(", ") + strValue[nCnt];
		
		strColumnQuery = strColumnQuery + _T(", ") + strDBField[nCnt];
	}
	}
	
	strQuery.Format(_T("INSERT INTO ILL_TRANS_TBL (%s) VALUES (%s);"),
		strColumnQuery, strValueQuery);

	return 0;
}



int CILL_APL_3100::Get_Update_BookTbl_Query(CString &strQuery)
{
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::Get_Update_BookTbl_Query(CString&) ");

	CESL_ControlMgr* pCM = FindCM(_T("CM_ILL_APL"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CESL_ControlEdit* pEdit = NULL;

	CString strBookKey;
	pEdit = (CESL_ControlEdit*)pCM->FindControl(_T("책_키"));
	if(NULL == pEdit) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);	
	pEdit->GetData(strBookKey);	
	if(_T("") == strBookKey) return -3;

	CString strPublishForm;
	pEdit = (CESL_ControlEdit*)pCM->FindControl(_T("책_발행형태구분"));
	if(NULL == pEdit) ERROR_MSG_RETURN_ERROR_CODE2(-4, strFuncName);	
	pEdit->GetData(strPublishForm);
	if(_T("") == strPublishForm) return -5;

	CString strTblName;
	CString strWorkingStatus;

	if(_T("MO") == strPublishForm)
	{
		strTblName = _T("BO_BOOK_TBL");
		strWorkingStatus = _T("BOL411O");
	}
	else if(_T("SE") == strPublishForm)
	{
		strTblName = _T("SE_BOOK_TBL");
		strWorkingStatus = _T("SEL411O");
	}
	else if(_T("NB") == strPublishForm)
	{
		strTblName = _T("CO_NON_DB_BOOK_TBL");
		strWorkingStatus = _T("BOL411O");
	}
	CFileFind FF;
	if(_T("D") == m_sBookCooperativeMode && TRUE == FF.FindFile(_T("..\\cfg\\_debug")))
	{
		strTblName = strTblName + _T("@ANOTHER_LIB");
	}	
	
	strQuery.Format(
		_T("UPDATE %s SET ")
		_T("PREV_WORKING_STATUS = WORKING_STATUS, ")
		_T("WORKING_STATUS = '%s', ")
		_T("LAST_WORK = '%s' ")
		_T("WHERE REC_KEY = %s;"),
		strTblName, 
		strWorkingStatus,
		GetWorkLogMsg( _T("신청"), __WFILE__, __LINE__ ),
		strBookKey
		);

	return 0;
}

int CILL_APL_3100::Get_Update_BookTbl_Query(CString &strQuery, CString strBookKey, CString strPublishForm)
{
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::Get_Update_BookTbl_Query(CString&, CString, CString) ");

	CString strTblName;
	CString strWorkingStatus;

	if(_T("MO") == strPublishForm)
	{
		strTblName = _T("BO_BOOK_TBL");
		strWorkingStatus = _T("BOL411O");
	}
	else if(_T("SE") == strPublishForm)
	{
		strTblName = _T("SE_BOOK_TBL");
		strWorkingStatus = _T("SEL411O");
	}
	else if(_T("NB") == strPublishForm)
	{
		strTblName = _T("CO_NON_DB_BOOK_TBL");
		strWorkingStatus = _T("BOL411O");
	}
	CFileFind FF;
	if(_T("D") == m_sBookCooperativeMode && TRUE == FF.FindFile(_T("..\\cfg\\_debug")))
	{
		strTblName = strTblName + _T("@ANOTHER_LIB");
	}	
	
	strQuery.Format(
		_T("UPDATE %s SET ")
		_T("PREV_WORKING_STATUS = WORKING_STATUS, ")
		_T("WORKING_STATUS = '%s', ")
		_T("LAST_WORK = '%s' ")
		_T("WHERE REC_KEY = %s;"),
		strTblName, 
		strWorkingStatus,
		GetWorkLogMsg( _T("신청"), __WFILE__, __LINE__ ),
		strBookKey
		);

	return 0;
}

int CILL_APL_3100::Get_UpdateIdxTblQuery(CString &strQuery)
{
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::Get_UpdateIdxTblQuery");

	if( (m_pInfo->MODE == 10000) || (m_pInfo->MODE == 30000) )
	{
		return 1000;
	}

	CESL_ControlEdit* pEdit = NULL;
	CESL_ControlMgr* pCM = FindCM(_T("CM_ILL_APL"));
	if(NULL == pCM)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, strFuncName);	
	}

	CString strBookKey;
	pEdit = (CESL_ControlEdit*)pCM->FindControl(_T("책_키"));
	if(NULL == pEdit) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, strFuncName);	
	}
	pEdit->GetData(strBookKey);	
	if(_T("") == strBookKey)	
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, strFuncName);
	}

	CString strPublishForm;
	pEdit = (CESL_ControlEdit*)pCM->FindControl(_T("책_발행형태구분"));
	if(NULL == pEdit)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, strFuncName);	
	}

	pEdit->GetData(strPublishForm);
	if(_T("") == strPublishForm)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1040, strFuncName);	
	}

	CString strTblName, strIDXTblName, strProcName;
	if(_T("MO") == strPublishForm || _T("단행") == strPublishForm)
	{
		strTblName = _T("BO_BOOK_TBL");
		strIDXTblName = _T("IDX_BO_TBL");			
		strProcName = _T("TESTWORKSTATUS");
	}
	else if(_T("SE") == strPublishForm || _T("연속") == strPublishForm)
	{
		strTblName = _T("SE_BOOK_TBL");
		strIDXTblName = _T("IDX_SE_TBL");			
		strProcName = _T("TESTSEWORKSTATUS");
	}
	else if(_T("NB") == strPublishForm || _T("긴급") == strPublishForm)
	{
		return 1010;
	}
	else
	{
		return -1050;
	}

	CFileFind FF;
	if(_T("D") == m_sBookCooperativeMode && TRUE == FF.FindFile(_T("..\\cfg\\_debug")))
	{
		CString strBookManageCode;
		ids = GetControlData(_T("CM_ILL_APL"), _T("책_관리구분"), strBookManageCode);
		if(0 > ids )
		{
			return -1070;
		}

		strTblName = strTblName + _T("@") + strBookManageCode;
		strIDXTblName = strIDXTblName + _T("@") + strBookManageCode;
		strProcName = strProcName + _T("@") + strBookManageCode;
	}	

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	
	CString strGetQuery, strSpeciesKey;
	strGetQuery.Format(
		_T("SELECT BB.SPECIES_KEY FROM %s BB WHERE BB.REC_KEY = %s AND ROWNUM = 1"),
		strTblName, strBookKey);
	ids = TmpDM.GetOneValueQuery(strGetQuery, strSpeciesKey);
	if(0 > ids)
	{
		return -1060;
	}
		
	strQuery.Format(
		_T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;"),
		strIDXTblName, strProcName , strSpeciesKey , strSpeciesKey);	

	return 0;
}

int CILL_APL_3100::Get_UpdateIdxTblQuery(CString &strQuery, CString strBookKey, CString strPublishForm)
{
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::Get_UpdateIdxTblQuery");

	if( (m_pInfo->MODE == 10000) || (m_pInfo->MODE == 30000) )
	{
		return 1000;
	}

	CString strTblName, strIDXTblName, strProcName;
	if(_T("MO") == strPublishForm || _T("단행") == strPublishForm)
	{
		strTblName = _T("BO_BOOK_TBL");
		strIDXTblName = _T("IDX_BO_TBL");			
		strProcName = _T("TESTWORKSTATUS");
	}
	else if(_T("SE") == strPublishForm || _T("연속") == strPublishForm)
	{
		strTblName = _T("SE_BOOK_TBL");
		strIDXTblName = _T("IDX_SE_TBL");			
		strProcName = _T("TESTSEWORKSTATUS");
	}
	else if(_T("NB") == strPublishForm || _T("긴급") == strPublishForm)
	{
		return 1010;
	}
	else
	{
		return -1050;
	}

	CFileFind FF;
	if(_T("D") == m_sBookCooperativeMode && TRUE == FF.FindFile(_T("..\\cfg\\_debug")))
	{
		CString strBookManageCode;
		ids = GetControlData(_T("CM_ILL_APL"), _T("책_관리구분"), strBookManageCode);
		if(0 > ids )
		{
			return -1070;
		}

		strTblName = strTblName + _T("@") + strBookManageCode;
		strIDXTblName = strIDXTblName + _T("@") + strBookManageCode;
		strProcName = strProcName + _T("@") + strBookManageCode;
	}	

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	
	CString strGetQuery, strSpeciesKey;
	strGetQuery.Format(
		_T("SELECT BB.SPECIES_KEY FROM %s BB WHERE BB.REC_KEY = %s AND ROWNUM = 1"),
		strTblName, strBookKey);
	ids = TmpDM.GetOneValueQuery(strGetQuery, strSpeciesKey);
	if(0 > ids)
	{
		return -1060;
	}
		
	strQuery.Format(
		_T("UPDATE %s SET WORKING_STATUS = %s(%s) WHERE REC_KEY = %s;"),
		strIDXTblName, strProcName , strSpeciesKey , strSpeciesKey);	

	return 0;
}


INT CILL_APL_3100::GetTransNo(CString &sTransNo)
{
EFS_BEGIN

	INT ids;
	CESL_DataMgr TmpDM;
  	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString sQuery, sYear, sYear2;
	sQuery.Format(_T("SELECT TO_CHAR(SYSDATE,'YYYY') FROM DUAL"));
	ids = TmpDM.GetOneValueQuery(sQuery, sYear);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1000, _T("GetTransNo"));
	}
	if(sYear.IsEmpty() || 4 != sYear.GetLength())
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1010, _T("GetTransNo"));
	}
	sYear2 = sYear.Right(2);
	if(m_pInfo->MANAGE_CODE.IsEmpty())
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1020, _T("GetTransNo"));
	}
	sQuery.Format(_T("SELECT COUNT(1) FROM CO_LAST_NUMBER_TBL ")
				  _T("WHERE DIVISION_VALUE='%s' AND PUBLISH_FORM_CODE='CO' ")
				  _T("AND KIND_CODE='ILL_NO' AND MANAGE_CODE='%s'")
				  , sYear2, m_pInfo->MANAGE_CODE);
	CString sCount;
	ids = TmpDM.GetOneValueQuery(sQuery, sCount);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1030, _T("GetTransNo"));
	}
	if( _ttoi(sCount) < 1 )
	{
		CString sFirstWork = GetWorkLogMsg(_T("TRANS_NO생성"), __WFILE__, __LINE__);
		sQuery.Format(	_T("INSERT INTO CO_LAST_NUMBER_TBL ")
						_T("(REC_KEY,KIND_CODE,PUBLISH_FORM_CODE,MAKE_YEAR,DIVISION_VALUE,")
						_T("LAST_NUMBER,CREATE_DATE,CREATE_WORKER,FIRST_WORK,MANAGE_CODE) ")
						_T("VALUES(ESL_SEQ.NEXTVAL,'ILL_NO','CO','%s','%s',0,SYSDATE,'%s','%s','%s');")
						, sYear, sYear2, m_pInfo->USER_ID
						, sFirstWork, m_pInfo->MANAGE_CODE);
		TmpDM.StartFrame();
		TmpDM.AddFrame(sQuery);
		ids = TmpDM.SendFrame();
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1040, _T("GetTransNo"));
		}
		TmpDM.EndFrame();
	}
	CString sLastNumber;
	sQuery.Format(	_T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
					_T("WHERE DIVISION_VALUE='%s' AND PUBLISH_FORM_CODE='CO' AND KIND_CODE='ILL_NO' ")
					_T("AND MANAGE_CODE='%s' FOR UPDATE")
					, sYear2, m_pInfo->MANAGE_CODE);
	ids = TmpDM.GetOneValueQuery(sQuery, sLastNumber);
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1050, _T("GetTransNo"));
	}
	CString sLibCode = m_pInfo->LIB_CODE;
	if(sLibCode.IsEmpty())
	{
		sQuery.Format(	_T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE='%s'")
						, m_pInfo->MANAGE_CODE);
		ids = TmpDM.GetOneValueQuery(sQuery, sLibCode);
		if(0 > ids || sLibCode.IsEmpty())
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1060, _T("GetTransNo"));
		}
	}
	if(6 != sLibCode.GetLength())
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1070, _T("GetTransNo"));
	}
	INT nLastNumber = _ttoi(sLastNumber);
	if(5 < sLastNumber.GetLength())
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1080, _T("GetTransNo"));
	}
	while(TRUE)
	{
		nLastNumber++;
		sLastNumber.Format(_T("%05d"), nLastNumber);
		if(5 != sLastNumber.GetLength())
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1090, _T("GetTransNo"));
		}
		sTransNo.Format(_T("%s-%s%s"), sLibCode, sYear2, sLastNumber);
		sQuery.Format(_T("SELECT COUNT(1) FROM ILL_TRANS_TBL WHERE TRANS_NO='%s'"), sTransNo);
		ids = TmpDM.GetOneValueQuery(sQuery, sCount);
		if(0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2(-1100, _T("GetTransNo"));
		}
		if(1 > _ttoi(sCount))
		{
			break;
		}
	}
	CString sLastWork = GetWorkLogMsg(_T("TRANS_NO생성"), __WFILE__, __LINE__);
	sQuery.Format(	_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER=%d, LAST_WORK='%s' ")
					_T("WHERE PUBLISH_FORM_CODE='CO' AND KIND_CODE='ILL_NO' AND DIVISION_VALUE='%s' ")
					_T("AND MANAGE_CODE='%s';")
					, nLastNumber, sLastWork, sYear2, m_pInfo->MANAGE_CODE);
	TmpDM.StartFrame();
	TmpDM.AddFrame(sQuery);
	ids = TmpDM.SendFrame();
	if(0 > ids)
	{
		ERROR_MSG_RETURN_ERROR_CODE2(-1110, _T("GetTransNo"));
	}
	TmpDM.EndFrame();

	return 0;

EFS_END
	return -1;
}

int CILL_APL_3100::GetAppendixInfo(CESL_DataMgr *pDM, CString &strWhere)
{
	int ids = 0;
	CString strFuncName = _T("CILL_APL_3100::GetAppendixInfo(CString&) ");

	CESL_ControlMgr* pCM = FindCM(_T("CM_ILL_APL"));
	if(NULL == pCM) ERROR_MSG_RETURN_ERROR_CODE2(-1, strFuncName);

	CESL_ControlEdit* pEdit = NULL;

	CString strBookKey, strBookManageCode;
	pEdit = (CESL_ControlEdit*)pCM->FindControl(_T("책_키"));
	if(NULL == pEdit) ERROR_MSG_RETURN_ERROR_CODE2(-2, strFuncName);	
	pEdit->GetData(strBookKey);	
	if(_T("") == strBookKey) 
	{
		return -100;
	}

	ids = GetControlData(_T("CM_ILL_APL"), _T("책_관리구분"), strBookManageCode);
	if(0 > ids ) ERROR_MSG_RETURN_ERROR_CODE2(-3, strFuncName);

	CString strQuery;
	CString strResult;
	strQuery.Format(_T("SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG = 'A' AND MANAGE_CODE = '%s' AND SPECIES_KEY = (SELECT SPECIES_KEY FROM BO_BOOK_TBL WHERE REC_KEY = %s AND BOOK_APPENDIX_FLAG != 'A') GROUP BY SPECIES_KEY"), strBookManageCode, strBookKey);
	pDM->GetOneValueQuery(strQuery, strResult);
	if(_T("") != strResult)
	{
		strWhere.Format(_T("BB.SPECIES_KEY = %s AND BB.WORKING_STATUS IN ('BOL112N')"), strResult);
		return 0;
	}
	else
	{
		pDM->FreeData();
		return 0;
	}

	return -200;
}

BOOL CILL_APL_3100::PreTranslateMessage(MSG* pMsg) 
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