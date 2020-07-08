
// 2012.11.22 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차	

#include "stdafx.h"
#include "SearchZipCodeDlgNew.h"
#include "CfgFileApi.h"

#include "..\\..\\단행\\열람\\장서관리\\LOC_CommonAPI\\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlgNew dialog


CSearchZipCodeDlgNew::CSearchZipCodeDlgNew(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//URL 요청 재시도 횟수(파일을 참조하지 않을경우)
	RETRY_CNT = 3;

	m_nMode = 0; //0: 지번 1:도로명주소 2:건물명 
	m_nResultMode = 0; //0: 지번입력 1: 도로명주소입력
	m_nMaxPageCnt = 0;
	m_nTotalCount = 0;
	m_nCurrentPageCnt = 1;
	m_strSiDoCode = _T("");
	m_strSiGunGuCode = _T("");
	m_strUpMyunDongCode = _T("");
	
	m_SiDo = _T("");
	m_SiGunGu = _T("");
	m_UpMyunDong = _T("");
	m_Ri = _T("");
	m_BuildingName = _T("");
	m_Jibun1 = _T("");
	m_Jibun2 = _T("");
	m_Mountain = _T("");
	m_ResultAddress = _T("");
	m_ResultZipcode = _T("");
	m_bSearchProcess = FALSE;
}

CSearchZipCodeDlgNew::~CSearchZipCodeDlgNew()
{
	/*
	if(NULL != m_pDMSGG)
	{
		delete m_pDMSGG;
		m_pDMSGG = NULL;
	}
	if(NULL != m_pDMUMD)
	{
		delete m_pDMUMD;
		m_pDMUMD = NULL;
	}
	if(NULL != m_pDMRI)
	{
		delete m_pDMRI;
		m_pDMRI = NULL;
	}
	if(NULL != m_pResultDM)
	{
		delete m_pResultDM;
		m_pResultDM = NULL;
	}
	if(NULL != m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
	*/
}

void CSearchZipCodeDlgNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchZipCodeDlgNew)
	DDX_Control(pDX, IDC_edt_PAGENUM, m_edtPAGENUM);
	DDX_Control(pDX, IDC_btn_BUILDINGNAME, m_edtNAME);
	DDX_Control(pDX, IDC_edt_NUM2, m_edtNUM2);
	DDX_Control(pDX, IDC_edt_NUM1, m_edtNUM1);
	DDX_Control(pDX, IDC_chk_MOUNTAIN, m_chkMountain);
	DDX_Control(pDX, IDC_cmb_RI, m_cmbRI);
	DDX_Control(pDX, IDC_cmb_UPMYUNDONG, m_cmbUPMYUNDONG);
	DDX_Control(pDX, IDC_cmb_SIGUNGU, m_cmbSIGUNGU);
	DDX_Control(pDX, IDC_cmb_SIDO, m_cmbSIDO);
	DDX_Control(pDX, IDC_tab_OPERATION, m_tabOperation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchZipCodeDlgNew, CDialog)
	//{{AFX_MSG_MAP(CSearchZipCodeDlgNew)
	ON_CBN_SELCHANGE(IDC_cmb_SIDO, OnSelchangecmbSIDO)
	ON_CBN_SELCHANGE(IDC_cmb_SIGUNGU, OnSelchangecmbSIGUNGU)
	ON_CBN_SELCHANGE(IDC_cmb_UPMYUNDONG, OnSelchangecmbUPMYUNDONG)
	ON_NOTIFY(TCN_SELCHANGE, IDC_tab_OPERATION, OnSelchangetabOPERATION)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnbtnSEARCH)
	ON_BN_CLICKED(IDC_btn_PREV, OnbtnPREV)
	ON_BN_CLICKED(IDC_btn_NEXT, OnbtnNEXT)
	ON_CBN_DROPDOWN(IDC_cmb_SIGUNGU, OnDropdowncmbSIGUNGU)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchZipCodeDlgNew message handlers

BOOL CSearchZipCodeDlgNew::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd());
	if(InitESL_Mgr(_T("SM_새주소API")) < 0)
	{
		MessageBox(_T("InitESL_Mgr Error!"), _T("ERROR"));
		return FALSE;
	}
	
	InitControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// 시군구 콤보 셋팅
INT CSearchZipCodeDlgNew::SetSGGCombo(CString strCode) 
{
	if(m_bSearchProcess)
	{
		return 100;
	}
	INT ids;
	if(m_pDMSGG == NULL) 
	{
		return -100;
	}
	m_cmbSIGUNGU.ResetContent();
	m_pDMSGG->FreeData();
	
	CString strRequestURL;
	strRequestURL.Format(_T("http://www.juso.go.kr/getAreaCode.do?from=city&to=county&valFrom=%s"), strCode);
	
	m_bSearchProcess = TRUE;
	m_Api.RequestURL(strRequestURL);
	CString strErrCode = m_Api.GetErrorMessage();
	if(_T("") != strErrCode)
	{
		AfxMessageBox(strErrCode);
		m_bSearchProcess = FALSE;
		return -200;
	}
	CString strResultFrame = m_Api.m_strSearchResult;
	
	ids = XMLParse(_T("counties"), m_pDMSGG, strResultFrame);
	if(0 > ids)
	{
		m_bSearchProcess = FALSE;
		return -300;
	}

	m_cmbSIGUNGU.AddString(_T("선택안함"));
	for(int i=0; i<m_pDMSGG->GetRowCount(); i++)
	{
		CString strTmp;
		strTmp = m_pDMSGG->GetCellData(_T("name"), i);
		//검색불가 데이터 SKIP
		if(_T("수원시") == strTmp)continue;
		else if(_T("성남시") == strTmp)continue;
		else if(_T("안양시") == strTmp)continue;
		else if(_T("안산시") == strTmp)continue;
		else if(_T("고양시") == strTmp)continue;
		else if(_T("용인시") == strTmp)continue;
		else if(_T("청주시") == strTmp)continue;
		else if(_T("천안시") == strTmp)continue;
		else if(_T("전주시") == strTmp)continue;
		else if(_T("포항시") == strTmp)continue;
		else if(_T("창원시") == strTmp)continue;
		m_cmbSIGUNGU.AddString(strTmp);
		m_cmbSIGUNGU.SetCurSel(0);
	}
	m_bSearchProcess = FALSE;
	return 0;
}


//읍면동 콤보박스 셋팅
INT CSearchZipCodeDlgNew::SetUMDCombo() 
{
	if(m_bSearchProcess)
	{
		return 100;
	}
	INT ids;
	CString strCode;
	if(_T("") == m_strSiDoCode || _T("") == m_strSiGunGuCode)
	{
		return -100;
	}
	strCode.Format(_T("%s%s"), m_strSiDoCode, m_strSiGunGuCode);
	if(m_pDMUMD == NULL) 
	{
		return -200;
	}
	m_cmbUPMYUNDONG.ResetContent();
	m_pDMUMD->FreeData();
	
	CString strRequestURL;
	CString strErrCode;
	strRequestURL.Format(_T("http://www.juso.go.kr/getAreaCode.do?from=county&to=town&valFrom=%s"), strCode);

	m_bSearchProcess = TRUE;
	m_Api.RequestURL(strRequestURL);
	strErrCode = m_Api.GetErrorMessage();
	if(_T("") != strErrCode)
	{
		m_bSearchProcess = FALSE;
		AfxMessageBox(strErrCode);
		return -300;
	}

	CString strResultFrame = m_Api.m_strSearchResult;
	
	ids = XMLParse(_T("towns"), m_pDMUMD, strResultFrame);
	if(0 > ids)
	{
		m_bSearchProcess = FALSE;
		return -400;
	}
	m_cmbUPMYUNDONG.AddString(_T("선택안함"));
	if(0 < m_pDMUMD->GetRowCount())
	{		
		for(int i=0; i<m_pDMUMD->GetRowCount(); i++)
		{
			CString strTmp;
			strTmp = m_pDMUMD->GetCellData(_T("name"), i);
			m_cmbUPMYUNDONG.EnableWindow(TRUE);
			m_cmbUPMYUNDONG.AddString(strTmp);
		}
	}
	else
	{
		//읍면동이 없는 케이스
		m_cmbUPMYUNDONG.EnableWindow(FALSE);
	}
	m_cmbUPMYUNDONG.SetCurSel(0);

	m_bSearchProcess = FALSE;
	return 0;
}

//리 콤보박스 셋팅
INT CSearchZipCodeDlgNew::SetRICombo() 
{
	if(m_bSearchProcess)
	{
		return 100;
	}
	INT ids;
	CString strCode;
	if(_T("") == m_strSiDoCode || _T("") == m_strSiGunGuCode || _T("") == m_strUpMyunDongCode)
	{
		return -100;
	}
	strCode.Format(_T("%s%s%s"), m_strSiDoCode, m_strSiGunGuCode, m_strUpMyunDongCode);
	if(m_pDMRI == NULL) 
	{
		return -200;
	}
	m_cmbRI.ResetContent();
	m_pDMRI->FreeData();

	CString strRequestURL;
	strRequestURL.Format(
		_T("http://www.juso.go.kr/getAreaCode.do?from=town&to=ri&valFrom=%s"), strCode);

	m_bSearchProcess = TRUE;
	CString strErrCode;
	m_Api.RequestURL(strRequestURL);
	CString strResultFrame = m_Api.m_strSearchResult;
	strErrCode = m_Api.GetErrorMessage();
	if(_T("") != strErrCode)
	{
		m_bSearchProcess = FALSE;
		AfxMessageBox(strErrCode);
		return -300;
	}

	ids = XMLParse(_T("ris"), m_pDMRI, strResultFrame);
	if(0 > ids)
	{
		//리없음
		m_bSearchProcess = FALSE;
		return -400;
	}
	m_cmbRI.AddString(_T("선택안함"));
	for(int i=0; i<m_pDMRI->GetRowCount(); i++)
	{
		CString strTmp;
		strTmp = m_pDMRI->GetCellData(_T("name"), i);
		m_cmbRI.EnableWindow(TRUE);
		m_cmbRI.AddString(strTmp);
		m_cmbRI.SetCurSel(0);
	}

	m_bSearchProcess = FALSE;
	return 0;
}


INT CSearchZipCodeDlgNew::XMLParse(CString strResultAlias, CESL_DataMgr *pDM, CString strFrame) 
{
	CString strTmp;
	INT ids;
	INT nStart, nEnd, nNext;
	INT nStart2, nEnd2, nNext2;

	CString strResultAliasTag, strResultAliasTagEnd, strResult, strName, strValue;
	if(_T("") == strFrame)
	{
		return -100;
	}
	strResultAliasTag.Format(_T("<%s>"), strResultAlias);
	strResultAliasTagEnd.Format(_T("</%s>"), strResultAlias);

	ids = strFrame.Find(strResultAliasTag);
	if(0 > ids)
	{
		return -101;
	}
	ids = strFrame.Find(strResultAliasTagEnd);
	if(0 > ids)
	{
		return -102;
	}
	

	nNext = 0;
	nNext2 = 0;
	INT i = 0;
	while(true)
	{
		nStart = strFrame.Find(_T("<name>"), nNext);
		if(0 > nStart)
		{
			break;
		}
		nEnd = strFrame.Find(_T("</name>"), nStart);
		if(0 > nEnd)
		{
			break;
		}
		nStart = nStart + 6;
		strName = strFrame.Mid(nStart, (nEnd-nStart));
		nNext = nEnd;


		nStart2 = strFrame.Find(_T("<value>"), nNext2);
		if(0 > nStart2)
		{
			break;
		}
		nEnd2 = strFrame.Find(_T("</value>"), nStart2);
		if(0 > nEnd2)
		{
			break;
		}
		nStart2 = nStart2 + 7;
		strValue = strFrame.Mid(nStart2, (nEnd2-nStart2));
		nNext2 = nEnd2;

		pDM->InsertRow(-1);
		ids = pDM->SetCellData(_T("name"), strName, i);
		ids = pDM->SetCellData(_T("value"), strValue, i);

		if(0 > ids)
		{
			return -200;
		}
		i++;
	}
	return 0;
}

void CSearchZipCodeDlgNew::OnSelchangecmbSIDO() 
{
	CString strCity, strMsg;
	INT ids = ((CComboBox*)GetDlgItem(IDC_cmb_SIDO))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_cmb_SIDO))->GetLBText(ids , strCity );

	if(_T("서울특별시") == strCity)m_strSiDoCode = _T("11");
	else if(_T("부산광역시") == strCity)m_strSiDoCode = _T("26");
	else if(_T("대구광역시") == strCity)m_strSiDoCode = _T("27");
	else if(_T("인천광역시") == strCity)m_strSiDoCode = _T("28");
	else if(_T("광주광역시") == strCity)m_strSiDoCode = _T("29");
	else if(_T("대전광역시") == strCity)m_strSiDoCode = _T("30");
	else if(_T("울산광역시") == strCity)m_strSiDoCode = _T("31");
	// 13/12/03 박대우 : 새주소검색기능 세종시 추가
/*//NEW------------------------------------------------------------------------*/
	else if(_T("세종특별자치시") == strCity)m_strSiDoCode = _T("36");
/*//END------------------------------------------------------------------------*/
	else if(_T("경기도") == strCity)m_strSiDoCode = _T("41");
	else if(_T("강원도") == strCity)m_strSiDoCode = _T("42");
	else if(_T("충청북도") == strCity)m_strSiDoCode = _T("43");
	else if(_T("충청남도") == strCity)m_strSiDoCode = _T("44");
	else if(_T("전라북도") == strCity)m_strSiDoCode = _T("45");
	else if(_T("전라남도") == strCity)m_strSiDoCode = _T("46");
	else if(_T("경상북도") == strCity)m_strSiDoCode = _T("47");
	else if(_T("경상남도") == strCity)m_strSiDoCode = _T("48");
	else if(_T("제주특별자치도") == strCity)m_strSiDoCode = _T("50");	
	else
	{
		m_strSiDoCode = _T("");
	}
	if(_T("") != m_strSiDoCode)
	{
		m_cmbSIGUNGU.EnableWindow(TRUE);
		m_cmbUPMYUNDONG.ResetContent();
		m_cmbUPMYUNDONG.EnableWindow(FALSE);
		m_cmbRI.ResetContent();
		m_cmbRI.EnableWindow(FALSE);

		for(INT i=0; i<RETRY_CNT; i++)
		{
			ids = SetSGGCombo(m_strSiDoCode);
			if(0 > ids)continue;
			else
			{
				break;
			}
		}
		if(0 > ids)
		{
			// 2013.02.08 ADD BY 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
			//                            출력메시지 변경
/*//BEFORE-----------------------------------------------------------------------			
			strMsg.Format(_T("요청이 %d회 연속 실패하였습니다. 잠시후에 다시 시도해주세요."), RETRY_CNT);
*///AFTER------------------------------------------------------------------------			
			strMsg.Format(_T("[도로명주소 안내시스템]서비스가 혼잡하여 지연되고 있습니다. 잠시후에 다시 시도해주시기 바랍니다."));
/*//END------------------------------------------------------------------------*/			
			AfxMessageBox(strMsg);
		}
		m_cmbSIGUNGU.SetCurSel(0);
		m_cmbUPMYUNDONG.AddString(_T("선택안함"));
		m_cmbUPMYUNDONG.SetCurSel(0);
		m_cmbRI.AddString(_T("선택안함"));
		m_cmbRI.SetCurSel(0);
	}
	else
	{
		m_cmbSIGUNGU.ResetContent();
		m_cmbSIGUNGU.EnableWindow(FALSE);
		m_cmbUPMYUNDONG.ResetContent();
		m_cmbUPMYUNDONG.EnableWindow(FALSE);
		m_cmbRI.ResetContent();
		m_cmbRI.EnableWindow(FALSE);
		m_cmbSIGUNGU.AddString(_T("선택안함"));
		m_cmbSIGUNGU.SetCurSel(0);
		m_cmbUPMYUNDONG.AddString(_T("선택안함"));
		m_cmbUPMYUNDONG.SetCurSel(0);
		m_cmbRI.AddString(_T("선택안함"));
		m_cmbRI.SetCurSel(0);
	}
}

void CSearchZipCodeDlgNew::OnSelchangecmbSIGUNGU() 
{
	CString strSiGunGu;
	CString strTmp;
	CString strMsg;
	INT ids = m_cmbSIGUNGU.GetCurSel();
	m_cmbSIGUNGU.GetLBText(ids , strSiGunGu);
	if(_T("선택안함") == strSiGunGu)
	{
		m_cmbUPMYUNDONG.ResetContent();
		m_cmbUPMYUNDONG.EnableWindow(FALSE);
		m_cmbRI.ResetContent();
		m_cmbRI.EnableWindow(FALSE);
		m_strSiGunGuCode == _T("");
		m_cmbUPMYUNDONG.AddString(_T("선택안함"));
		m_cmbUPMYUNDONG.SetCurSel(0);
		m_cmbRI.AddString(_T("선택안함"));
		m_cmbRI.SetCurSel(0);
		return;
	}
	else
	{
		m_cmbRI.ResetContent();
		m_cmbRI.EnableWindow(FALSE);
		m_cmbRI.AddString(_T("선택안함"));
		m_cmbRI.SetCurSel(0);
	}

	for(INT i=0; i<m_pDMSGG->GetRowCount(); i++)
	{
		strTmp = m_pDMSGG->GetCellData(_T("name"), i);
		if(strTmp == strSiGunGu)
		{
			m_SiGunGu = strTmp;
			m_strSiGunGuCode = m_pDMSGG->GetCellData(_T("value"), i);
			break;
		}
		else
		{
			continue;
		}
	}

	for(INT j=0; j<RETRY_CNT; j++)
	{
		ids = SetUMDCombo();
		if(0 > ids)continue;
		else
		{
			break;
		}
	}
	if(0 > ids)
	{
		// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
		//                   출력메시지 변경
/*//BEFORE-----------------------------------------------------------------------
		strMsg.Format(_T("요청이 %d회 연속 실패하였습니다. 잠시후에 다시 시도해주세요."), RETRY_CNT);
*///AFTER------------------------------------------------------------------------
		strMsg.Format(_T("[도로명주소 안내시스템]서비스가 혼잡하여 지연되고 있습니다. 잠시후에 다시 시도해주시기 바랍니다."));
/*//END------------------------------------------------------------------------*/
		AfxMessageBox(strMsg);
	}
}

void CSearchZipCodeDlgNew::OnSelchangecmbUPMYUNDONG() 
{
	CString strUpMyunDong;
	CString strMsg;
	CString strTmp;
	INT ids = m_cmbUPMYUNDONG.GetCurSel();
	m_cmbUPMYUNDONG.GetLBText(ids , strUpMyunDong);
	if(_T("선택안함") == strUpMyunDong)
	{
		m_cmbRI.ResetContent();
		m_cmbRI.EnableWindow(FALSE);
		m_strUpMyunDongCode = _T("");
		m_cmbRI.AddString(_T("선택안함"));
		m_cmbRI.SetCurSel(0);
		return;
	}

	for(INT i=0; i<m_pDMUMD->GetRowCount(); i++)
	{
		strTmp = m_pDMUMD->GetCellData(_T("name"), i);
		if(strTmp == strUpMyunDong)
		{
			m_UpMyunDong = strTmp;
			m_strUpMyunDongCode = m_pDMUMD->GetCellData(_T("value"), i);
			break;
		}
		else
		{
			continue;
		}
	}
	for(INT j=0; j<RETRY_CNT; j++)
	{
		ids = SetRICombo();
		if(0 > ids)continue;
		else
		{
			break;
		}
	}
	if(0 > ids)
	{
		// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
		//                   출력메시지 변경
/*//BEFORE-----------------------------------------------------------------------
		strMsg.Format(_T("요청이 %d회 연속 실패하였습니다. 잠시후에 다시 시도해주세요."), RETRY_CNT);
*///AFTER------------------------------------------------------------------------
		strMsg.Format(_T("[도로명주소 안내시스템]서비스가 혼잡하여 지연되고 있습니다. 잠시후에 다시 시도해주시기 바랍니다."));
/*//END------------------------------------------------------------------------*/
		AfxMessageBox(strMsg);
		return;
	}
}

void CSearchZipCodeDlgNew::OnSelchangetabOPERATION(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	ChangeView();
	*pResult = 0;
}

void CSearchZipCodeDlgNew::OnbtnSEARCH() 
{
	if(m_bSearchProcess)
	{
		return;
	}
	
	m_bSearchProcess = TRUE;
	m_pResultDM->FreeData();
	INT ids;
	//검색결과에 따라 그리드 변경
	ids = ((CButton*)GetDlgItem(IDC_rad_OLD))->GetCheck();
	if(1 == ids)
	{
		m_pGrid = (CESL_Grid*)FindControl(_T("CM_새주소API_검색결과"), _T("MAINGRID"));
		((CESL_Grid*)GetDlgItem(IDC_gridNEWADDRESS))->ShowWindow(SW_SHOW);
		((CESL_Grid*)GetDlgItem(IDC_gridNEWADDRESS2))->ShowWindow(SW_HIDE);
		m_nResultMode = 0;
	}
	ids = ((CButton*)GetDlgItem(IDC_rad_NEW))->GetCheck();
	if(1 == ids)
	{
		m_pGrid = (CESL_Grid*)FindControl(_T("CM_새주소API_검색결과"), _T("NEWGRID"));
		((CESL_Grid*)GetDlgItem(IDC_gridNEWADDRESS))->ShowWindow(SW_HIDE);
		((CESL_Grid*)GetDlgItem(IDC_gridNEWADDRESS2))->ShowWindow(SW_SHOW);
		m_nResultMode = 1;
	}

	CString strTmp;
	ids = m_tabOperation.GetCurSel();
	if(0 <= ids)
	{
		m_nMode = ids;
	}

	CString strResult, strMsg;
	if(m_cmbSIDO.IsWindowEnabled() && m_cmbSIDO.IsWindowVisible())
	{
		ids = m_cmbSIDO.GetCurSel();
		m_cmbSIDO.GetLBText(ids , m_SiDo);
		if(_T("선택안함") == m_SiDo)
		{
			m_SiDo = _T("");
		}
	}
	else
	{
		m_SiDo = _T("");
	}
	if(m_cmbSIGUNGU.IsWindowEnabled() && m_cmbSIGUNGU.IsWindowVisible())
	{
		ids = m_cmbSIGUNGU.GetCurSel();
		m_cmbSIGUNGU.GetLBText(ids , m_SiGunGu);
		if(_T("선택안함") == m_SiGunGu)
		{
			m_SiGunGu = _T("");
		}
	}
	else
	{
		m_SiGunGu = _T("");
	}
	if(m_cmbUPMYUNDONG.IsWindowEnabled() && m_cmbUPMYUNDONG.IsWindowVisible())
	{
		ids = m_cmbUPMYUNDONG.GetCurSel();
		m_cmbUPMYUNDONG.GetLBText(ids , m_UpMyunDong);
		if(_T("선택안함") == m_UpMyunDong)
		{
			m_UpMyunDong = _T("");
		}
	}
	else
	{
		m_UpMyunDong = _T("");
	}
	if(m_cmbRI.IsWindowEnabled() && m_cmbRI.IsWindowVisible())
	{
		// 13/10/07 박대우 : 2012년도 기능개선패치 수정작업(20130604005)
		//                   리 단위 입력 오류 수정
/*//BEFORE------------------------------------------------------------
		ids = m_cmbRI.GetCurSel();
		m_cmbRI.GetLBText(ids , m_Ri);
		m_cmbUPMYUNDONG.GetLBText(ids , m_Ri);
		if(_T("선택안함") == m_UpMyunDong)
		{
			m_Ri = _T("");
		}
*///AFTER------------------------------------------------------------------------
		ids = m_cmbRI.GetCurSel();
		m_cmbRI.GetLBText(ids , m_Ri);
		if(m_Ri.CompareNoCase(_T("선택안함")) == 0)
		{
			m_Ri = _T("");
		}
/*//END------------------------------------------------------------------------*/
	}
	else
	{
		m_Ri = _T("");
	}	
	//지번(건물번호) 파싱
	m_edtNUM1.GetWindowText(m_Jibun1);
	m_edtNUM2.GetWindowText(m_Jibun2);
	m_Jibun1.TrimLeft();
	m_Jibun1.TrimRight();
	m_Jibun2.TrimLeft();
	m_Jibun2.TrimRight();

	//건물명
	m_edtNAME.GetWindowText(m_BuildingName);
	m_BuildingName.TrimLeft();
	m_BuildingName.TrimRight();
	ids = m_chkMountain.GetCheck();
	if(1 == ids)
	{
		m_Mountain = _T("1");
	}
	else
	{
		m_Mountain = _T("");
	}


	//필수입력 체크
	if(_T("") == m_SiDo || _T("선택안함") == m_SiDo)
	{
		m_bSearchProcess = FALSE;
		AfxMessageBox(_T("시/도를 선택해주세요."));	
		return;
	}
	// 13/12/03 박대우 : 새주소검색기능 세종시 추가
/*//BEFORE-----------------------------------------------------------------------
	else if(_T("") == m_SiGunGu || _T("선택안함") == m_SiGunGu)
*///AFTER------------------------------------------------------------------------
	else if(_T("선택안함") == m_SiGunGu)
/*//END------------------------------------------------------------------------*/
	{
		m_bSearchProcess = FALSE;
		AfxMessageBox(_T("시/군/구를 선택해주세요."));	
		return;
	}

	// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
	//                   최소검색조건을 제거하여 시/도/군/구 만으로도 검색할 수 있도록 변경
/*//REM--------------------------------------------------------------------------
	if(_T("") == m_Jibun1 && _T("") == m_Jibun2 && _T("") == m_BuildingName)
	{
		m_bSearchProcess = FALSE;
		AfxMessageBox(_T("검색조건이 부족합니다."));
		return;
	}
--//END------------------------------------------------------------------------*/

	//검색 시작
	for(INT i=0; i<RETRY_CNT; i++)
	{
		ids = SearchApi(_T(""));
		if(0 > ids)continue;
		else
		{
			break;
		}
	}
	if(0 > ids)
	{
		// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
		//                   출력메시지 변경
/*//BEFORE-----------------------------------------------------------------------
		strMsg.Format(_T("요청이 %d회 연속 실패하였습니다. 잠시후에 다시 시도해주세요."), RETRY_CNT);
*///AFTER------------------------------------------------------------------------
		strMsg.Format(_T("[도로명주소 안내시스템]서비스가 혼잡하여 지연되고 있습니다. 잠시후에 다시 시도해주시기 바랍니다."));
/*//END------------------------------------------------------------------------*/
		AfxMessageBox(strMsg);
		m_bSearchProcess = FALSE;
		return;
	}
	
	//페이지 셋팅
	INT nCountPerPage;
	m_pResultDM->GetCellData(_T("totalCount"), 0, strTmp);
	m_nTotalCount = _ttoi(strTmp);
	m_pResultDM->GetCellData(_T("intCountPerPage"), 0, strTmp);
	nCountPerPage = _ttoi(strTmp);
	if(0 < m_nTotalCount && 0 < nCountPerPage)
	{
		m_nMaxPageCnt = (m_nTotalCount / nCountPerPage);
		m_nLastPageCnt = m_nTotalCount % nCountPerPage;
		if(0 < m_nLastPageCnt)
		{
			m_nMaxPageCnt++;
		}	
	}
	m_nCurrentPageCnt = 1;
	SetPageNum();
	m_bSearchProcess = FALSE;
}

void CSearchZipCodeDlgNew::ChangeView() 
{
	//검색을 눌러야만 모드가 바뀜(멤버변수)
	// KOL.UDF.022 시큐어코딩 보완
	INT nMode = -1;
	INT ids = m_tabOperation.GetCurSel();
	if(0 <= ids)
	{
		nMode = ids;
	}
	if(0 == nMode)
	{
		//STATIC
		((CStatic*)GetDlgItem(IDC_stc_SIDO))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_SIGUNGU))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_UPMYUNDONG))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_RI))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_BUNJI))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_static_DASH))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_BUILDINGNM))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_BUNJI))->SetWindowText(_T("     번  지"));
		((CStatic*)GetDlgItem(IDC_stc_BUILDINGNM))->SetWindowText(_T("   건  물  명"));
		
		//INPUT
		m_cmbSIDO.ShowWindow(SW_SHOW);
		m_cmbSIGUNGU.ShowWindow(SW_SHOW);
		m_cmbUPMYUNDONG.ShowWindow(SW_SHOW);
		m_cmbRI.ShowWindow(SW_SHOW);
		m_chkMountain.ShowWindow(SW_SHOW);
		m_edtNUM1.ShowWindow(SW_SHOW);
		m_edtNUM2.ShowWindow(SW_SHOW);
		m_edtNAME.ShowWindow(SW_SHOW);
	}
	else if(1 == nMode)
	{
		//STATIC
		((CStatic*)GetDlgItem(IDC_stc_SIDO))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_SIGUNGU))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_UPMYUNDONG))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_stc_RI))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_stc_BUNJI))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_static_DASH))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_BUILDINGNM))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_BUNJI))->SetWindowText(_T("  건 물 번 호"));
		((CStatic*)GetDlgItem(IDC_stc_BUILDINGNM))->SetWindowText(_T("   도  로  명"));
		
		//INPUT
		m_cmbSIDO.ShowWindow(SW_SHOW);
		m_cmbSIGUNGU.ShowWindow(SW_SHOW);
		m_cmbUPMYUNDONG.ShowWindow(SW_HIDE);
		m_cmbRI.ShowWindow(SW_HIDE);
		m_chkMountain.ShowWindow(SW_HIDE);
		m_edtNUM1.ShowWindow(SW_SHOW);
		m_edtNUM2.ShowWindow(SW_SHOW);
		m_edtNAME.ShowWindow(SW_SHOW);
	}
	else if(2 == nMode)
	{
		//STATIC
		((CStatic*)GetDlgItem(IDC_stc_SIDO))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_SIGUNGU))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_UPMYUNDONG))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_RI))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_stc_BUNJI))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_static_DASH))->ShowWindow(SW_HIDE);
		((CStatic*)GetDlgItem(IDC_stc_BUILDINGNM))->ShowWindow(SW_SHOW);
		((CStatic*)GetDlgItem(IDC_stc_BUILDINGNM))->SetWindowText(_T("   건  물  명"));
		
		//INPUT
		m_cmbSIDO.ShowWindow(SW_SHOW);
		m_cmbSIGUNGU.ShowWindow(SW_SHOW);
		m_cmbUPMYUNDONG.ShowWindow(SW_SHOW);
		m_cmbRI.ShowWindow(SW_HIDE);
		m_chkMountain.ShowWindow(SW_HIDE);
		m_edtNUM1.ShowWindow(SW_HIDE);
		m_edtNUM2.ShowWindow(SW_HIDE);
		m_edtNAME.ShowWindow(SW_SHOW);
	}
	else
	{
		return;
	}
}

void CSearchZipCodeDlgNew::OnbtnPREV() 
{
	INT ids;
	CString strPageCount, strMsg;
	m_bSearchProcess = TRUE;
	if(2 > m_nMaxPageCnt)
	{
		m_bSearchProcess = FALSE;
		return;
	}
	if(1 == m_nCurrentPageCnt)
	{
		m_bSearchProcess = FALSE;
		return;
	}
	m_nCurrentPageCnt--;
	strPageCount.Format(_T("%d"), m_nCurrentPageCnt);

	for(INT i=0; i<RETRY_CNT; i++)
	{
		ids = SearchApi(strPageCount);
		if(0 > ids)continue;
		else
		{
			break;
		}
	}	
	if(0 > ids)
	{
		m_nCurrentPageCnt++;
		m_bSearchProcess = FALSE;
		// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
		//                   출력메시지 변경
/*//BEFORE-----------------------------------------------------------------------
		strMsg.Format(_T("요청이 %d회 연속 실패하였습니다. 잠시후에 다시 시도해주세요."), RETRY_CNT);
*///AFTER------------------------------------------------------------------------
		strMsg.Format(_T("[도로명주소 안내시스템]서비스가 혼잡하여 지연되고 있습니다. 잠시후에 다시 시도해주시기 바랍니다."));
/*//END------------------------------------------------------------------------*/
		AfxMessageBox(strMsg);
		return;
	}

	SetPageNum();
	m_bSearchProcess = FALSE;
}

void CSearchZipCodeDlgNew::OnbtnNEXT() 
{
	INT ids;
	CString strPageCount, strMsg;
	m_bSearchProcess = TRUE;
	if(2 > m_nMaxPageCnt)
	{
		m_bSearchProcess = FALSE;
		return;
	}	
	if(m_nCurrentPageCnt == m_nMaxPageCnt)
	{
		m_bSearchProcess = FALSE;
		return;
	}
	m_nCurrentPageCnt++;
	strPageCount.Format(_T("%d"), m_nCurrentPageCnt);

	for(INT i=0; i<RETRY_CNT; i++)
	{
		ids = SearchApi(strPageCount);
		if(0 > ids)continue;
		else
		{
			break;
		}
	}	
	if(0 > ids)
	{
		// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
		//                   출력메시지 변경
/*//BEFORE-----------------------------------------------------------------------
		strMsg.Format(_T("요청이 %d회 연속 실패하였습니다. 잠시후에 다시 시도해주세요."), RETRY_CNT);
*///AFTER------------------------------------------------------------------------
		strMsg.Format(_T("[도로명주소 안내시스템]서비스가 혼잡하여 지연되고 있습니다. 잠시후에 다시 시도해주시기 바랍니다."));
/*//END------------------------------------------------------------------------*/
		AfxMessageBox(strMsg);
		m_nCurrentPageCnt--;
		m_bSearchProcess = FALSE;
		return;
	}

	SetPageNum();
	m_bSearchProcess = FALSE;
}



INT CSearchZipCodeDlgNew::XMLParseToDM(CString strResultAlias, CESL_DataMgr *pDM, CString strFrame) 
{
	pDM->FreeData();
	CString strTmp;
	INT ids;
	CString strResultAliasTag, strResultAliasTagEnd, strResultAliasTagNone, strResult, strData;
	CString strTotalCount;
	CString strCountPerPage;
	if(_T("") == strFrame)
	{
		return -100;
	}
	strResultAliasTag.Format(_T("<%s>"), strResultAlias);
	strResultAliasTagEnd.Format(_T("</%s>"), strResultAlias);
	strResultAliasTagNone.Format(_T("<%s />"), strResultAlias);

	ids = strFrame.Find(strResultAliasTagNone);
	if(0 <= ids)
	{
		//검색결과 없음
		return 3000;
	}
	ids = strFrame.Find(_T("<totalCount>"));
	if(0 > ids)
	{
		//검색결과 없음
		return 3000;
	}
	
	ids = strFrame.Find(strResultAliasTagEnd);
	if(0 > ids)
	{
		//검색결과 없음
		return -200;
	}
	
	INT x, y;
	x = strFrame.Find(_T("<totalCount>")) + 12;
	y = strFrame.Find(_T("</totalCount>"));
	strTotalCount = strFrame.Mid(x, (y-x));
	x = strFrame.Find(_T("<intCountPerPage>")) + 17;
	y = strFrame.Find(_T("</intCountPerPage>"));
	strCountPerPage = strFrame.Mid(x, (y-x));

	INT LoopCount, TotalCount, CountPerPage;
	TotalCount = _ttoi(strTotalCount);
	CountPerPage = _ttoi(strCountPerPage);
	if(TotalCount <= CountPerPage)
	{
		LoopCount = TotalCount;
	}
	else
	{
		if(m_nMaxPageCnt == m_nCurrentPageCnt)
		{
			LoopCount = m_nLastPageCnt;
		}
		else
		{
			LoopCount = CountPerPage;
		}
	}

	CString *strRowFromFrame = new CString[LoopCount];
	INT nStart, nEnd, nNext = 0;

	// 검색결과 프레임을 검색결과 DM에 SET
	CString strJibun1, strJibun2, strMountain, strBuildNum1, strBuildNum2, strUnder, strJibun, strBuildNum;
	for(INT i=0; i<LoopCount; i++)
	{	
		nStart = strFrame.Find(_T("<siNm>"), nNext);
		nEnd = strFrame.Find(_T("</intCountPerPage>"), nNext) + 18;
		strRowFromFrame[i] = strFrame.Mid(nStart, (nEnd- nStart));
		nNext = nEnd;

		pDM->InsertRow(-1);

		strTmp = FindTag(_T("siNm"), strRowFromFrame[i]);
		pDM->SetCellData(_T("siNm"), strTmp, i);
		strTmp = FindTag(_T("sggNm"), strRowFromFrame[i]);
		pDM->SetCellData(_T("sggNm"), strTmp, i);		
		strTmp = FindTag(_T("emdNm"), strRowFromFrame[i]);
		pDM->SetCellData(_T("emdNm"), strTmp, i);
		strTmp = FindTag(_T("liNm"), strRowFromFrame[i]);
		pDM->SetCellData(_T("liNm"), strTmp, i);		
		strTmp = FindTag(_T("rn"), strRowFromFrame[i]);
		pDM->SetCellData(_T("rn"), strTmp, i);		
		strTmp = FindTag(_T("rnCd"), strRowFromFrame[i]);
		pDM->SetCellData(_T("rnCd"), strTmp, i);		
		strBuildNum1 = FindTag(_T("buldMnnm"), strRowFromFrame[i]);
		pDM->SetCellData(_T("buldMnnm"), strBuildNum1, i);		
		strBuildNum2 = FindTag(_T("buldSlno"), strRowFromFrame[i]);
		pDM->SetCellData(_T("buldSlno"), strBuildNum2, i);		
		strJibun1 = FindTag(_T("lnbrMnnm"), strRowFromFrame[i]);
		pDM->SetCellData(_T("lnbrMnnm"), strJibun1, i);		
		strJibun2 = FindTag(_T("lnbrSlno"), strRowFromFrame[i]);
		pDM->SetCellData(_T("lnbrSlno"), strJibun2, i);		
		strUnder = FindTag(_T("udrtYn"), strRowFromFrame[i]);
		pDM->SetCellData(_T("udrtYn"), strUnder, i);		
		strMountain = FindTag(_T("mtYn"), strRowFromFrame[i]);
		pDM->SetCellData(_T("mtYn"), strMountain, i);		
		strTmp = FindTag(_T("bdNm"), strRowFromFrame[i]);
		pDM->SetCellData(_T("bdNm"), strTmp, i);		
		strTmp = FindTag(_T("bdkd"), strRowFromFrame[i]);
		pDM->SetCellData(_T("bdkd"), strTmp, i);
		strTmp = FindTag(_T("zipCl"), strRowFromFrame[i]);
		pDM->SetCellData(_T("zipCl"), strTmp, i);		
		strTmp = FindTag(_T("admCd"), strRowFromFrame[i]);
		pDM->SetCellData(_T("admCd"), strTmp, i);		
		strTmp = FindTag(_T("totalCount"), strRowFromFrame[i]);
		pDM->SetCellData(_T("totalCount"), strTmp, i);		
		strTmp = FindTag(_T("intCurrentPage"), strRowFromFrame[i]);
		pDM->SetCellData(_T("intCurrentPage"), strTmp, i);		
		strTmp = FindTag(_T("intCountPerPage"), strRowFromFrame[i]);
		pDM->SetCellData(_T("intCountPerPage"), strTmp, i);
		
		if(_T("0") == strJibun2 || _T("") == strJibun2)
		{
			strJibun.Format(_T("%s%s"), strMountain, strJibun1);
		}
		else 
		{
			strJibun.Format(_T("%s%s-%s"), strMountain, strJibun1, strJibun2);
		}
		pDM->SetCellData(_T("지번"), strJibun, i);

		if(_T("0") == strBuildNum2 || _T("") == strBuildNum2)
		{
			strBuildNum.Format(_T("%s"), strBuildNum1);
		}
		else 
		{
			strBuildNum.Format(_T("%s-%s"), strBuildNum1, strBuildNum2);
		}
		pDM->SetCellData(_T("건물번호"), strBuildNum, i);
	}

	// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
	//                   할당 메모리 해제
/*//NEW------------------------------------------------------------------------*/
	delete [] strRowFromFrame;
/*//END------------------------------------------------------------------------*/
	return 0;
}

CString CSearchZipCodeDlgNew::FindTag(CString strAlias, CString strFrame) 
{
	CString strResult;
	CString strResultAliasTag, strResultAliasTagEnd, strResultAliasTagNone;
	strResultAliasTag.Format(_T("<%s>"), strAlias);
	strResultAliasTagEnd.Format(_T("</%s>"), strAlias);
	strResultAliasTagNone.Format(_T("<%s />"), strAlias);

	INT ids, nStart, nEnd, nLength;
	nLength = strAlias.GetLength();
	if(0 >= nLength)
	{
		return _T("");
	}
	ids = strFrame.Find(strResultAliasTagNone);
	if(0 <= ids)
	{
		return _T("");
	}
	nStart = strFrame.Find(strResultAliasTag);
	if(0 > nStart)
	{
		return _T("");
	}
	nEnd = strFrame.Find(strResultAliasTagEnd);
	if(0 > nEnd)
	{
		return _T("");
	}
	nStart = nStart + nLength+2;

	strResult = strFrame.Mid(nStart, (nEnd - nStart));

	return strResult;
}

void CSearchZipCodeDlgNew::OnOK() 
{
	INT ids;
	ids = m_pGrid->GetIdx();
	if(0 > ids)
	{
		return;
	}
	if(1 > m_pResultDM->GetRowCount())
	{
		AfxMessageBox(_T("선택된 데이터가 없습니다."));
		return;
	}

	// 검색결과 DM의 선정된 ROW를 불러와 하나의 주소스트링으로 파싱 
	CString strSido, strSiGunGU, strUpMyunDong, strRi, strRoadName, strBuldingName, strJibun1, strJibun2, strMountain, strBuildNum1, strBuildNum2, strJibun, BuildingNum;
	m_pResultDM->GetCellData(_T("siNm"), ids, strSido);
	m_pResultDM->GetCellData(_T("sggNm"), ids, strSiGunGU);
	m_pResultDM->GetCellData(_T("emdNm"), ids, strUpMyunDong);
	m_pResultDM->GetCellData(_T("liNm"), ids, strRi);
	m_pResultDM->GetCellData(_T("rn"), ids, strRoadName);
	m_pResultDM->GetCellData(_T("bdNm"), ids, strBuldingName);
	m_pResultDM->GetCellData(_T("lnbrMnnm"), ids, strJibun1);
	m_pResultDM->GetCellData(_T("lnbrSlno"), ids, strJibun2);
	m_pResultDM->GetCellData(_T("mtYn"), ids, strMountain);
	m_pResultDM->GetCellData(_T("buldMnnm"), ids, strBuildNum1);
	m_pResultDM->GetCellData(_T("buldSlno"), ids, strBuildNum2);
	m_pResultDM->GetCellData(_T("zipCl"), ids, m_ResultZipcode);
	m_pResultDM->GetCellData(_T("지번"), ids, strJibun);
	m_pResultDM->GetCellData(_T("건물번호"), ids, BuildingNum);

	ids = ((CButton*)GetDlgItem(IDC_rad_OLD))->GetCheck();
	//지번주소 입력
	if(1 == ids)
	{		
		//리가 있는경우
		if(_T("") != strRi)
		{
			m_ResultAddress.Format(_T("%s %s %s %s %s %s"), strSido, strSiGunGU, strUpMyunDong, strRi, strJibun, strBuldingName);
		}
		//리 없는 경우
		else
		{
			m_ResultAddress.Format(_T("%s %s %s %s %s"), strSido, strSiGunGU, strUpMyunDong, strJibun, strBuldingName);
		}
	}
	ids = ((CButton*)GetDlgItem(IDC_rad_NEW))->GetCheck();
	//새주소 입력
	if(1 == ids)
	{
		m_ResultAddress.Format(_T("%s %s %s %s %s"), strSido, strSiGunGU, strRoadName, BuildingNum, strBuldingName);
	}
	
	//콤보박스 상태 저장
	SaveControl();

	CDialog::OnOK();
}

void CSearchZipCodeDlgNew::SetPageNum() 
{
	CString strResult;
	strResult.Format(_T("%d / %d"), m_nCurrentPageCnt, m_nMaxPageCnt);

	m_edtPAGENUM.SetWindowText(strResult);

	return;
}

INT CSearchZipCodeDlgNew::SearchApi(CString strPage) 
{
	INT ids;
	CString strRequestURL;

	//URL 인코딩
	CString strTmp;
	strTmp = CLocCommonAPI::UrlEncode((LPCTSTR)m_SiDo);
	m_SiDo = strTmp;
	strTmp = CLocCommonAPI::UrlEncode((LPCTSTR)m_SiGunGu);
	m_SiGunGu = strTmp;
	strTmp = CLocCommonAPI::UrlEncode((LPCTSTR)m_UpMyunDong);
	m_UpMyunDong = strTmp;
	strTmp = CLocCommonAPI::UrlEncode((LPCTSTR)m_Ri);
	m_Ri = strTmp;
	strTmp = CLocCommonAPI::UrlEncode((LPCTSTR)m_BuildingName);
	m_BuildingName = strTmp;

	//지번검색
	if(0 == m_nMode)
	{
		strRequestURL.Format(
		_T("http://www.juso.go.kr/link/search.do?extend=false&mode=jibun_search&searchType=location_jibun&engineCtpNm=%s&engineSigNm=%s&engineEmdNm=%s&engineBdNm=%s&engineBdMaSn=%s&engineBdSbSn=%s&engineMtYn=%s&currentPage=%s"), 
		m_SiDo, m_SiGunGu, m_UpMyunDong, m_BuildingName, m_Jibun1, m_Jibun2, m_Mountain, strPage);
	}
	//도로명검색
	else if(1 == m_nMode)
	{
		strRequestURL.Format(
		_T("http://www.juso.go.kr/link/search.do?extend=false&mode=road_search&searchType=location_newaddr&topTab=1&engineCtpNm=%s&engineSigNm=%s&engineRdNm=%s&engineBdMaSn=%s&engineBdSbSn=%s&currentPage=%s"), 
		m_SiDo, m_SiGunGu, m_BuildingName, m_Jibun1, m_Jibun2, strPage);
	}
	//건물명검색
	else if(2 == m_nMode)
	{
		strRequestURL.Format(
		_T("http://www.juso.go.kr/link/search.do?extend=false&mode=road_search&searchType=location_buld&engineCtpNm=%s&engineSigNm=%s&engineEmdNm=%s&engineBdNm=%s&currentPage=%s"), 
		m_SiDo, m_SiGunGu, m_UpMyunDong, m_BuildingName, strPage);
	}
	else
	{
		return -100;
	}

	TRACE(_T("검색요청------------------------------------------------------\n"));
	TRACE(_T("%s\n"), strRequestURL);
	TRACE(_T("검색요청------------------------------------------------------\n"));
	
	COpenAPI *pApi = new COpenAPI();

	CString strErrCode;
	pApi->RequestURL(strRequestURL);
	strErrCode = pApi->GetErrorMessage();
	if(_T("") != strErrCode)
	{
		// 13/03/27 박대우 : 2012년도 도서관 통합서비스 확대구축 사업 2차 수정작업
		//                   할당 메모리 해제
/*//NEW------------------------------------------------------------------------*/
		delete pApi;
/*//END------------------------------------------------------------------------*/
		AfxMessageBox(strErrCode);
		return -200;
	}

	CString strFrame = pApi->m_strSearchResult;
	ids = XMLParseToDM(_T("results"), m_pResultDM, strFrame);
	// 검색결과 없음
	if(3000 == ids)
	{
		AfxMessageBox(_T("검색결과가 없습니다."));
		delete pApi;
		pApi = NULL;
		m_pResultDM->FreeData();
		m_nMaxPageCnt = 0;
		SetPageNum();
	}
	else if(-200 == ids)
	{
		TRACE(_T("결과오류------------------------------------------------------\n"));
		TRACE(_T("%s\n"), strFrame);
		TRACE(_T("결과오류------------------------------------------------------\n"));
		delete pApi;
		pApi = NULL;
//		AfxMessageBox(_T("통신오류가 발생 하였습니다. 잠시후 다시 시도해주세요."));
		return -300;
	}
	else if(0 > ids)
	{
		delete pApi;
		pApi = NULL;
		AfxMessageBox(_T("검색 실패"));
		return -400;
	}
	
	if(0 == m_nResultMode)
	{
		ControlDisplay(_T("CM_새주소API_검색결과"), _T("MAINGRID"));
	}
	else if(1 == m_nResultMode)
	{
		ControlDisplay(_T("CM_새주소API_검색결과"), _T("NEWGRID"));
	}
	if(NULL != pApi)
	{
		delete pApi;
		pApi = NULL;
	}
	return 0;
}


VOID CSearchZipCodeDlgNew::InitControl() 
{
	// DM FIND
	m_pResultDM = FindDM(_T("DM_새주소API_검색결과"));
	m_pDMSGG = FindDM(_T("DM_새주소API_시군구"));
	m_pDMUMD = FindDM(_T("DM_새주소API_읍면동"));
	m_pDMRI = FindDM(_T("DM_새주소API_리"));

	// 탭 컨트롤
	m_tabOperation.InsertItem( 0 , _T("지번") );
	m_tabOperation.InsertItem( 1 , _T("도로명주소") );
	m_tabOperation.InsertItem( 2 , _T("건물명") );
	
	// 시 콤보박스 초기화
	m_cmbSIDO.AddString(_T("선택안함"));
	m_cmbSIDO.AddString(_T("서울특별시"));
	m_cmbSIDO.AddString(_T("부산광역시"));
	m_cmbSIDO.AddString(_T("대구광역시"));
	m_cmbSIDO.AddString(_T("인천광역시"));
	m_cmbSIDO.AddString(_T("광주광역시"));
	m_cmbSIDO.AddString(_T("대전광역시"));
	m_cmbSIDO.AddString(_T("울산광역시"));
	// 13/12/03 박대우 : 새주소검색기능 세종시 추가
/*//NEW------------------------------------------------------------------------*/
	m_cmbSIDO.AddString(_T("세종특별자치시"));
/*//END------------------------------------------------------------------------*/
	m_cmbSIDO.AddString(_T("경기도"));
	m_cmbSIDO.AddString(_T("강원도"));
	m_cmbSIDO.AddString(_T("충청북도"));
	m_cmbSIDO.AddString(_T("충청남도"));
	m_cmbSIDO.AddString(_T("전라북도"));
	m_cmbSIDO.AddString(_T("전라남도"));
	m_cmbSIDO.AddString(_T("경상북도"));
	m_cmbSIDO.AddString(_T("경상남도"));
	m_cmbSIDO.AddString(_T("제주특별자치도"));	
	m_cmbSIDO.SetCurSel(0);
	m_cmbSIGUNGU.AddString(_T("선택안함"));
	m_cmbSIGUNGU.SetCurSel(0);
	m_cmbUPMYUNDONG.AddString(_T("선택안함"));
	m_cmbUPMYUNDONG.SetCurSel(0);
	m_cmbRI.AddString(_T("선택안함"));
	m_cmbRI.SetCurSel(0);

	// 페이지번호 셋팅
	m_edtPAGENUM.SetWindowText(_T("0 / 0"));

	// 검색결과 라디오버튼 셋팅
	((CButton*)GetDlgItem(IDC_rad_OLD))->SetCheck(1);

	// 모드 및 콤보박스 저장 불러오기
	// KOL.UDF.022 시큐어코딩 보완
	INT ids = -1;
	CString strTmp;
	CCfgFileApi fileApi;
	fileApi.SetFile(_T("..\\cfg\\LoanReturn\\newAddress_mode.txt"));

	//재시도 횟수 설정
	fileApi.GetData(_T("재시도횟수"), strTmp);
	if(_T("") != strTmp)
	{
		RETRY_CNT = _ttoi(strTmp);
	}

	CString strSiDo;
	fileApi.GetData(_T("시도"), strSiDo);
	if(_T("") != strSiDo)
	{
		m_cmbSIDO.SetCurSel(_ttoi(strSiDo));
		m_cmbSIDO.GetLBText(_ttoi(strSiDo), m_SiDo);
		fileApi.GetData(_T("시도코드"), m_strSiDoCode);
		m_cmbSIGUNGU.EnableWindow(TRUE);
	}

	CString strSiGunGuList;
	CString strSiGunGuCodeList;
	CString strCurSel;
	fileApi.GetData(_T("시군구"), strSiGunGuList);
	fileApi.GetData(_T("시군구코드"), strSiGunGuCodeList);
	fileApi.GetData(_T("시군구선택"), strCurSel);
	if(_T("") != strSiGunGuList)
	{
		INT i=0;
		m_pDMSGG->FreeData();
		//콤보박스 SHOW
		m_cmbSIGUNGU.ResetContent();
		m_cmbSIGUNGU.EnableWindow(TRUE);

		//시군구 콤보박스 셋팅
		while(0 != ids)
		{
			ids = AfxExtractSubString(strTmp, strSiGunGuList, i, _T(':'));
			if(_T("") == strTmp)
			{
				break;
			}
			m_pDMSGG->InsertRow(-1);
			m_pDMSGG->SetCellData(_T("name"), strTmp, i);
			m_cmbSIGUNGU.AddString(strTmp);
			ids = AfxExtractSubString(strTmp, strSiGunGuCodeList, i, _T(':'));
			m_pDMSGG->SetCellData(_T("value"), strTmp, i);
			i++;
		}
		m_cmbSIGUNGU.SetCurSel(_ttoi(strCurSel));
		if(0 < m_pDMSGG->GetRowCount())
		{
			m_pDMSGG->GetCellData(_T("value"), _ttoi(strCurSel), m_strSiGunGuCode);
			ids = SetUMDCombo();
			if(0 > ids)
			{
				//2번까진 재시도
				if(0 > SetUMDCombo())
				{
					return;
//					AfxMessageBox(_T("SetUMDCombo() Error!!!"));
				}
			}
		}
	}
	else
	{
		if(0 > SetSGGCombo(m_strSiDoCode))
		{
			//2번까진 재시도
			if(0 > SetSGGCombo(m_strSiDoCode))
			{
				return;
//				AfxMessageBox(_T("SetSGGCombo() Error!!!"));
			}
		}
	}

	fileApi.GetData(_T("모드"), strTmp);
	m_nResultMode = _ttoi(strTmp);
	//지번주소
	if(0 == m_nResultMode)
	{
		m_pGrid = (CESL_Grid*)FindControl(_T("CM_새주소API_검색결과"), _T("MAINGRID"));
		((CESL_Grid*)GetDlgItem(IDC_gridNEWADDRESS))->ShowWindow(SW_SHOW);
		((CESL_Grid*)GetDlgItem(IDC_gridNEWADDRESS2))->ShowWindow(SW_HIDE);
		((CButton*)GetDlgItem(IDC_rad_OLD))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_rad_NEW))->SetCheck(0);
	}
	//새주소
	else if(1 == m_nResultMode)
	{
		m_pGrid = (CESL_Grid*)FindControl(_T("CM_새주소API_검색결과"), _T("NEWGRID"));
		((CESL_Grid*)GetDlgItem(IDC_gridNEWADDRESS))->ShowWindow(SW_HIDE);
		((CESL_Grid*)GetDlgItem(IDC_gridNEWADDRESS2))->ShowWindow(SW_SHOW);
		((CButton*)GetDlgItem(IDC_rad_OLD))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_rad_NEW))->SetCheck(1);
	}
}

INT CSearchZipCodeDlgNew::SaveControl() 
{
	CCfgFileApi fileApi;
	fileApi.SetFile(_T("..\\cfg\\LoanReturn\\newAddress_mode.txt"));
	
	INT ids;
	CString strSido;
	CString strTmp;
	ids = m_cmbSIDO.GetCurSel();
	if(ids > 0)
	{
		strSido.Format(_T("%d"), ids);
		fileApi.SetData(_T("시도"), strSido);
		fileApi.SetData(_T("시도코드"), m_strSiDoCode);
	}

	ids = m_cmbSIGUNGU.GetCurSel();
	if(0 <= ids && m_cmbSIGUNGU.IsWindowEnabled() && m_cmbSIGUNGU.IsWindowVisible())
	{
		CString strSiGunGu = _T("선택안함:");
		CString strSiGunGuCode = _T("0:");
		CString strTmp2;
		for(INT i=0; i<m_pDMSGG->GetRowCount(); i++)
		{
			strTmp = m_pDMSGG->GetCellData(_T("name"), i);
			if(_T("선택안함") == strTmp)
			{
				continue;
			}
			strTmp2.Format(_T("%s%s:"), strSiGunGu, strTmp);
			strSiGunGu = strTmp2;
			strTmp = m_pDMSGG->GetCellData(_T("value"), i);
			strTmp2.Format(_T("%s%s:"), strSiGunGuCode, strTmp);
			strSiGunGuCode = strTmp2;
		}
		fileApi.SetData(_T("시군구"), strSiGunGu);
		fileApi.SetData(_T("시군구코드"), strSiGunGuCode);
		strTmp.Format(_T("%d"), ids);
		fileApi.SetData(_T("시군구선택"), strTmp);
	}

	ids = ((CButton*)GetDlgItem(IDC_rad_OLD))->GetCheck();
	if(1 == ids)
	{
		fileApi.SetData(_T("모드"), _T("0"));
	}
	ids = ((CButton*)GetDlgItem(IDC_rad_NEW))->GetCheck();
	if(1 == ids)
	{
		fileApi.SetData(_T("모드"), _T("1"));
	}

	return 0;
}

BEGIN_EVENTSINK_MAP(CSearchZipCodeDlgNew, CDialog)
    //{{AFX_EVENTSINK_MAP(CSearchZipCodeDlgNew)
	ON_EVENT(CSearchZipCodeDlgNew, IDC_gridNEWADDRESS, -601 /* DblClick */, OnDblClickgridNEWADDRESS, VTS_NONE)
	ON_EVENT(CSearchZipCodeDlgNew, IDC_gridNEWADDRESS2, -601 /* DblClick */, OnDblClickgridNEWADDRESS2, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void CSearchZipCodeDlgNew::OnDropdowncmbSIGUNGU() 
{
	
}

void CSearchZipCodeDlgNew::OnDblClickgridNEWADDRESS() 
{	
	if(m_pGrid->GetMouseRow() == 0)
	{
		return;	
	}
	if(1 > m_pResultDM->GetRowCount())
	{
		return;
	}
	OnOK() ;
}

void CSearchZipCodeDlgNew::OnDblClickgridNEWADDRESS2() 
{
	if(m_pGrid->GetMouseRow() == 0)
	{
		return;	
	}
	if(1 > m_pResultDM->GetRowCount())
	{
		return;
	}
	OnOK() ;
}

BOOL CSearchZipCodeDlgNew::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		OnbtnSEARCH();
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
