// DonatorMgr.cpp : implementation file
//

#include "stdafx.h"
#include "DonatorMgr.h"
#include "../SearchZipCodeApi/SearchZipCodeDlg.h"

// 14/11/27 박대우 : 도로명주소 OpenAPI 변경
/*//BEFORE-----------------------------------------------------------------------
//2012.11.23 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차(새주소 조회)
#include "..\\..\\..\\..\\공통\\SearchZipCodeAPI\\SearchZipCodeDlgNew.h"
*///AFTER------------------------------------------------------------------------
#include "..\\..\\..\\..\\공통\\SearchZipCodeAPI\\SearchZipCodeDlgOpenAPI.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WORK_LOG GetWorkLogMsg(_T("기증자입력수정"), _T(__FILE__), __LINE__)

/////////////////////////////////////////////////////////////////////////////
// CDonatorMgr dialog


CDonatorMgr::CDonatorMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDonatorMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDonatorMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainGrid = NULL;
	m_nOpenMode = OPERATION_TYPE_INVALID;
	m_nIdx = -1; 
	m_pComboMgr = new CMulitComboBoxManager(this);
	m_firstidx = -1;
	m_lastidx = -1;
	m_pLastNumber = new CLastNumberManager(this);
	m_strMainCMAlias = _T("CM_기증자관리");
	m_bGridSelectYN = FALSE;

	m_pCMakeIndex = new CMakeSearchData(pParent);
}

CDonatorMgr::~CDonatorMgr()
{

	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
	if(m_pLastNumber)
	{
		delete m_pLastNumber;
		m_pLastNumber = NULL;
	}
	if (m_pCMakeIndex)
	{
		delete m_pCMakeIndex;
		m_pCMakeIndex = NULL; 
	}
}

VOID CDonatorMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDonatorMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDonatorMgr, CDialog)
	//{{AFX_MSG_MAP(CDonatorMgr)
	ON_BN_CLICKED(IDC_btnDONATOR_FIND_ZIP_CODE, OnbtnDONATORFINDZIPCODE)
	ON_BN_CLICKED(IDC_btnDONATOR_PREV, OnbtnDONATORPREV)
	ON_BN_CLICKED(IDC_btnDONATOR_NEXT, OnbtnDONATORNEXT)
	ON_EN_KILLFOCUS(IDC_edtDONATOR_NAME, OnKillfocusedtDONATORNAME)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_cmbDONATOR_FLAG, OnSelchangecmbDONATORFLAG)
	ON_EN_KILLFOCUS(IDC_edtDONATOR_YEAR, OnKillfocusedtDONATORYEAR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDonatorMgr message handlers

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자 입력창 초기화
//
// [ 미니스펙 ]
/////////////////////////////////////////////////////////////////////////////
BOOL CDonatorMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//=======================================================================
	// 1. SM/CM/DM을 설정한다.
	//=======================================================================
	if( InitESL_Mgr( _T("K2UPGRADE_기증자") ) < 0 )
	{
		ESLAfxMessageBox( _T("InitESL_Mgr Error : MNG_USER_MANAGER_02") );
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_기증자"));
	m_pDM = FindDM(_T("DM_기증자관리"));
	if( m_pCM == NULL || m_pDM == NULL )
	{
		ESLAfxMessageBox( _T("[CM_기증자/DM_기증자관리]를 설정할 수 없습니다.") );
		return FALSE;
	}

	m_pMainGrid = (CESL_Grid*)FindControl(m_strMainCMAlias, _T("MainGrid"));
	if( m_pMainGrid == NULL )
	{
		ESLAfxMessageBox( _T("[CM_기증자관리]를 설정할 수 없습니다.") );
		return FALSE;
	}

	//=======================================================================
	// 2. 접속정보를 설정한다.
	//=======================================================================
	this->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);

	//CString strData[2] = {_T("PN"), _T("GR")};
	//m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("PN"), strData, _T("기증자구분"), 2);

	//=======================================================================
	// 3. 필수 입력항목에 대해 컨트롤 색상을 변경한다.
	//=======================================================================	
	AddColorControl(IDC_edtDONATOR_NAME, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtDONATOR_YEAR, _T(""), _T(""), 'E');


	//=======================================================================
	// 4. 오픈모드에 따라 리소스의 표시를 설정한다. (입력/수정)
	//=======================================================================
	ShowCtrlByOpenMode();

	//=======================================================================
	// 5. 오픈모드가 입력이 아니라면 
	//=======================================================================
	if(m_nOpenMode == SINSERT)	OnSelchangecmbDONATORFLAG();
	else						Display(0);

	//=====================================================
	//2009.01.13 ADD BY PJW : 기증자 구분코드는 선택을 하수없도록 한다.
	//2009.02.03 REM BY PJW : 기증자 구분코드는 MANAGE_CODE 와 상관없어서 주석처리함
// 	CEdit* pEdit = (CEdit*) GetDlgItem(IDC_cmbDONATOR_FLAG);
// 	pEdit->EnableWindow(FALSE);
	//=====================================================	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDonatorMgr::OnbtnDONATORFINDZIPCODE() 
{
	// 14/11/27 박대우 : 도로명주소 OpenAPI 변경
/*//BEFORE-----------------------------------------------------------------------
	//2012.11.23 ADD BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차(새주소 조회)
	CSearchZipCodeDlgNew SearchZipCodeDlgNew(this);
	if(IDOK == SearchZipCodeDlgNew.DoModal())
	{			
		m_pCM->SetControlMgrData(_T("우편번호"), SearchZipCodeDlgNew.m_ResultZipcode);
		m_pCM->SetControlMgrData(_T("주소"), SearchZipCodeDlgNew.m_ResultAddress);
	}

	//2012.11.23 DEL BY BJY : 2012년도 도서관 통합서비스 확대구축 사업 2차(새주소 조회)
//	CSearchZipCodeDlg SearchZipCodeDlg(this);
//	SearchZipCodeDlg.DoModal();
//
//	// 결과물
//	if(SearchZipCodeDlg.m_nCloseMode != 0)
//	{
//		m_pCM->SetControlMgrData(_T("우편번호"), SearchZipCodeDlg.m_sZipCode);
//		m_pCM->SetControlMgrData(_T("주소"), SearchZipCodeDlg.m_sAddress);
//	}
*///AFTER------------------------------------------------------------------------
	CSearchZipCodeDlgOpenAPI dlg(this);
	if (dlg.DoModal() == IDOK)
	{
		m_pCM->SetControlMgrData(_T("주소"), dlg.GetApplyAddress());
		m_pCM->SetControlMgrData(_T("우편번호"), dlg.GetApplyZipCode());		
	}
/*//END------------------------------------------------------------------------*/	
}

VOID CDonatorMgr::OnbtnDONATORPREV() 
{
	Display(-1);
}

VOID CDonatorMgr::OnbtnDONATORNEXT() 
{
	Display(1);
}

VOID CDonatorMgr::OnOK() 
{
	INT ids = -1;
	CString strMsg;

	if( m_nOpenMode == SINSERT )	ids = InsertDonator();
	else							ids = UpdateDonator();

	if( ids < 0 )
	{
		if( m_nOpenMode == SINSERT )	strMsg = _T("기증자 정보를 입력할 수 없습니다.");
		else							strMsg = _T("기증자 정보를 수정할 수 없습니다.");
	}
	else if( ids == 0 )
	{
		strMsg = _T("기증자 정보를 저장하였습니다.");
		if( m_nOpenMode == SINSERT )	m_pCM->AllControlClear();
		else							Display(1);
	}
	else		return;
	ESLAfxMessageBox(strMsg);
	OnSelchangecmbDONATORFLAG();
}

VOID CDonatorMgr::ShowCtrlByOpenMode()
{
	if(m_nOpenMode == SINSERT)
	{
		GetDlgItem(IDC_btnDONATOR_PREV)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_btnDONATOR_NEXT)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_edtDONATOR_ID)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_edtDONATOR_SERIAL_NO)->ShowWindow(SW_HIDE);
		SetWindowText(_T("기증자정보 입력"));
	}
	else
	{
		GetDlgItem(IDC_btnDONATOR_PREV)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_btnDONATOR_NEXT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_cmbDONATOR_FLAG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtDONATOR_YEAR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtDONATOR_ID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtDONATOR_SERIAL_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_stcDONATOR_SERIAL_NO)->ShowWindow(SW_HIDE);
		SetWindowText(_T("기증자정보 수정"));
	}
}

VOID CDonatorMgr::Display(INT ids)
{
	INT nIdx;
	CString strID, strYear, strFlag, strSerialNo;

	switch( ids )
	{
	case 0:
		{
		InitFirstLastIndex();
		nIdx = GetFirstSelectIndex();
		ShowNextPrevButton(nIdx);
		}
		break;
	case -1:
		{
		nIdx = GetPrevSelectIndex();
		ShowNextPrevButton(nIdx);
		}
		break;
	case 1:
		{
		nIdx = GetNextSelectIndex();
		ShowNextPrevButton(nIdx);
		}
		break;
	default:		return ;
		break;
	}

	if( nIdx >= 0 )		 m_nIdx = nIdx;

	m_pDM->GetCellData(_T("아이디"), m_nIdx, strID);
	strFlag = strID.Left(2);
	strYear = strID.Mid(2,4);
	strSerialNo = strID.Right(5);

	//CString strData[2] = {_T("PN"), _T("GR")};
	//m_pComboMgr->SetSingleComboBoxSelect(m_pCM->CM_ALIAS, _T("기증자구분"), strData, strFlag, 2);
	
	m_pCM->SetControlMgrData(_T("등록년도"), strYear);
	m_pCM->SetControlMgrData(_T("일련번호"), strSerialNo);
	m_pCM->AllControlDisplay(m_nIdx);
	
	m_pMainGrid->SetReverse(m_nIdx);
}


//////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 그리드 선정체크 여부에 따라 멤버변수를 설정한다.
//
//////////////////////////////////////////////////////////////////////////////
VOID CDonatorMgr::InitFirstLastIndex()
{
	//=======================================================================
	// 1. 그리드 선정체크를 확인 한다.
	//=======================================================================
	INT ids = m_pMainGrid->SelectMakeList();

	//=======================================================================
	// 2. 선정체크가 되어 있지 않다면 if문으로 설정하고, 되어 있다면 else 문으로 설정한다.
	//		- if : 그리드 선정체크=FALSE, 첫 인덱스=0, 마지막 인덱스=마지막로우
	//		- else : 그리드 선정체크=TRUE, 첫인덱스=HeadPosition, 마지막인덱스=TailPosition
	//=======================================================================
	ids = m_pMainGrid->SelectGetHeadPosition();
	if(ids < 0)
	{
		m_bGridSelectYN = FALSE;
		INT cnt = m_pDM->GetRowCount();
		m_firstidx = 0;
		m_lastidx = cnt-1;
	}
	else
	{
		m_bGridSelectYN = TRUE;
		m_lastidx = m_pMainGrid->SelectGetTailPosition();
		m_firstidx = m_pMainGrid->SelectGetHeadPosition();
	}
}

VOID CDonatorMgr::ShowNextPrevButton(INT idx)
{
	if(idx < 0) idx = m_nIdx;
	GetDlgItem(IDC_btnDONATOR_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnDONATOR_NEXT)->EnableWindow(TRUE);

	if(m_firstidx >= idx || idx < 0)
		GetDlgItem(IDC_btnDONATOR_PREV)->EnableWindow(FALSE);
	if(m_lastidx <= idx || idx < 0)
		GetDlgItem(IDC_btnDONATOR_NEXT)->EnableWindow(FALSE);
}

//////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 기증자입력
//
// [ 미니스펙 ]
// 1. 기증자란이 비어있거나 동일한 기증자가 존재하는지 확인한다.
// 2. CM_기증자를 이용하여 DM_기증자관리를 구성한다.
// 3. DM을 구성한다.
// 4. 구성된 DM을 쿼리로 저장하여 디비에 적용한다.
// 5. 입력된 기증자를 그리드에 추가한다.
// 6. 마지막번호를 업데이트한다.
//
//////////////////////////////////////////////////////////////////////////////
INT CDonatorMgr::InsertDonator()
{
	//========================================================================
	// 변수정의
	//========================================================================
	INT ids, nCurRow;
	CString strTmpData, strYear, strID, strFlag;
	CArray <CString, CString> RemoveAliasList;


	//========================================================================
	// 1. 기증자란이 비어있거나 동일한 기증자가 존재하는지 확인한다.
	//========================================================================
	ids = CheckValidateDonatorInfo();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("기증자명은 입력하셔야 합니다."));
		return -1;
	}
	else if(ids > 0) return ids;
	
	//========================================================================
	// 2. CM_기증자를 이용하여 DM_기증자관리를 구성한다.
	//========================================================================
	m_pDM->InsertRow(-1);
	nCurRow = m_pDM->GetRowCount() - 1;
	ids = SetDMRowbyCMData(m_pCM, m_pDM, nCurRow);
	if( ids < 0 )	return ids;

	//========================================================================
	// 3. DM을 구성한다.
	//========================================================================
	m_pDM->SetCellData(_T("입력일"), GetTodayDate(), nCurRow);
	m_pDM->SetCellData(_T("최초작업"), WORK_LOG, nCurRow);
	m_pDM->SetCellData(_T("마지막작업"), WORK_LOG, nCurRow);
	
	strTmpData = m_pDM->GetCellData(_T("기증자"), nCurRow);
	m_pCMakeIndex->GetKeyWord(strTmpData);	//색인어 생성클래스 - 전달된 인자의 스트링을 색인어로 변환한다.
	m_pDM->SetCellData(_T("기증자인덱스"), strTmpData, nCurRow);
	m_pDM->MakeRecKey(strTmpData);
	m_pDM->SetCellData(_T("기증자정보KEY"), strTmpData, nCurRow);
	//strType, _T("CO"), strAcqYear, strDesc, strLastNo);

	// 등록 아이디를 만든다. = 등록년도+기증자구분+일련번호(5자리)
	m_pCM->GetControlMgrData(_T("등록년도"), strYear);
	m_pCM->GetControlMgrData(_T("기증자구분"), strFlag);
	m_pCM->GetControlMgrData(_T("일련번호"), strTmpData);
	INT nTmpData = _ttoi(strTmpData);
	strTmpData.Format(_T("%05d"), nTmpData);
	strID = strFlag+strYear+strTmpData;
	m_pDM->SetCellData(_T("아이디"), strID, nCurRow);

	//=====================================================
	//2008.12.12 ADD BY PJW : 기증자입력할경우도 MANAGE_CODE를 추가한다.
	m_pDM->SetCellData(_T("관리구분"), m_pInfo->MANAGE_CODE, nCurRow);
	//=====================================================

	
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("단행책수"));
	RemoveAliasList.Add(_T("연속책수"));

	//========================================================================
	// 4. 구성된 DM을 쿼리로 저장하여 디비에 적용한다.
	//========================================================================
	m_pDM->StartFrame();
	m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, nCurRow, m_pDM);
	ids = m_pDM->SendFrame();
	if( ids < 0 ) return -1;
	ids = m_pDM->EndFrame();
	if( ids < 0 ) return -1;
	
	//========================================================================
	// 5. 입력된 기증자를 그리드에 추가한다.
	//========================================================================
	m_pMainGrid->DisplayLine(nCurRow);

	//========================================================================
	// 6. 마지막번호를 업데이트한다.
	//========================================================================
	strTmpData.Format(_T("%d"), nTmpData);
	ids = m_pLastNumber->SetLastNumber(_T("DON_ID"), _T("CO"), strYear, strFlag, strTmpData);
	if( ids < 0 ) return -1;

	return 0;
}

INT CDonatorMgr::UpdateDonator()
{
	INT ids;
	CString strTmpData, strYear, strID, strFlag, strKey, strSerialNo;
	CArray <CString, CString> RemoveAliasList;

	//========================================================================
	// 1. 기증자란이 비어있거나 동일한 기증자가 존재하는지 확인한다.
	//========================================================================
	ids = CheckValidateDonatorInfo();
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("기증자명은 입력하셔야 합니다."));
		return -1;
	}
	else if(ids > 0) return ids;

	//========================================================================
	// 2. CM_기증자를 이용하여 DM_기증자관리를 구성한다.
	//========================================================================
	ids = SetDMRowbyCMData(m_pCM, m_pDM, m_nIdx);


	//========================================================================
	// 3. DM을 구성한다.
	//========================================================================
	m_pDM->SetCellData(_T("마지막작업"), WORK_LOG, m_nIdx);
	
	strTmpData = m_pDM->GetCellData(_T("기증자"), m_nIdx);
	m_pCMakeIndex->GetKeyWord(strTmpData);
	m_pDM->SetCellData(_T("기증자인덱스"), strTmpData, m_nIdx);
	strKey = m_pDM->GetCellData(_T("기증자정보KEY"), m_nIdx);
	
	/*
	m_pCM->GetControlMgrData(_T("등록년도"), strYear);
	m_pCM->GetControlMgrData(_T("기증자구분"), strFlag);
	m_pCM->GetControlMgrData(_T("일련번호"), strSerialNo);
	strID = strFlag+strYear+strSerialNo;
	m_pDM->SetCellData(_T("아이디"), strID, m_nIdx);
	*/

	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add(_T("기증자정보KEY"));
	RemoveAliasList.Add(_T("아이디"));
	RemoveAliasList.Add(_T("기증자구분"));
	RemoveAliasList.Add(_T("일련번호"));
	RemoveAliasList.Add(_T("등록년도"));
	RemoveAliasList.Add(_T("단행책수"));
	RemoveAliasList.Add(_T("연속책수"));

	//========================================================================
	// 4. 구성된 DM을 쿼리로 저장하여 디비에 적용한다.
	//========================================================================
	m_pDM->StartFrame();
	m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, m_nIdx, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strKey);
	ids = m_pDM->SendFrame();
	if( ids < 0 )	return -1;
	ids = m_pDM->EndFrame();
	if( ids < 0 )	return -1;

	//========================================================================
	// 5. 그리드를 다시 그린다.
	//========================================================================
	m_pMainGrid->DisplayLine(m_nIdx);
	return 0;	
}

CString CDonatorMgr::GetTodayDate(VOID)
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CDonatorMgr::SetDMRowbyCMData(CESL_ControlMgr * pCM, CESL_DataMgr * pDM, INT nCurRow)
{
	INT ids, controlCnt, i;
	CString strTmp, alias;
	POSITION pos;
	CESL_Control_Element* pCE = NULL;

	if( pCM == NULL || pDM == NULL )		return -1000;

	controlCnt = pCM->Control_List.GetCount();	
	pos = pCM->Control_List.GetHeadPosition();

	for( i=0 ; i<controlCnt && pos != NULL; i++ )
	{
		pCE = NULL;
		pCE = (CESL_Control_Element*)pCM->Control_List.GetNext(pos);
		if( pCE == NULL )	continue;

		alias = pCE->GetAlias();
		ids = pCM->GetControlMgrData(alias, strTmp);
		if( ids < 0 )		continue;

		pDM->SetCellData(alias, strTmp, nCurRow);
	}
	
	return 0;
}

INT CDonatorMgr::CheckValidateDonatorInfo()
{
	INT ids ;
	CString strTmpData, strMsg;

	m_pCM->GetControlMgrData(_T("기증자"), strTmpData);
	if( strTmpData.IsEmpty() == TRUE )		return -1;

	ids = CheckDuplicateDonator(strTmpData);
	if(ids < 0)
	{
		strMsg.Format(_T("동일 이름의 등록된 기증자가 존재합니다. 저장하시겠습니까?"));
		if( IDNO == ESLAfxMessageBox(strMsg, MB_YESNO) )	 return 1000;
	}

	return 0;
}

INT CDonatorMgr::CheckDuplicateDonator(CString strDonator)
{
	INT nRowCnt;
	CString strQuery;
	CESL_DataMgr tmpDM;

	if( m_nOpenMode == SINSERT ) 
	{
		//=====================================================
		//2009.01.12 UPDATE BY PJW : 기증자 중복검색할경우 MANAGE_CODE를 조건에 넣는다.
// 		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE DONATOR='%s'"), m_pDM->TBL_NAME, strDonator);
		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE DONATOR='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), m_pDM->TBL_NAME, strDonator);		
		//=====================================================
	}
	else
	{
		//=====================================================
		//2009.01.12 UPDATE BY PJW : 기증자 중복검색할경우 MANAGE_CODE를 조건에 넣는다.
// 		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE DONATOR='%s' AND REC_KEY != %s"), 
// 						m_pDM->TBL_NAME, strDonator, m_pDM->GetCellData(_T("기증자정보KEY"), m_nIdx));

		strQuery.Format(_T("SELECT REC_KEY FROM %s WHERE DONATOR='%s' AND REC_KEY != %s AND MANAGE_CODE = UDF_MANAGE_CODE"), 
 						m_pDM->TBL_NAME, strDonator, m_pDM->GetCellData(_T("기증자정보KEY"), m_nIdx));
		//=====================================================
		
	}
		
	
	tmpDM.SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	tmpDM.RestructDataManager(strQuery);
	
	nRowCnt = tmpDM.GetRowCount();
	if( nRowCnt > 0 )	return -1;
	else return 0;
}

VOID CDonatorMgr::OnKillfocusedtDONATORNAME() 
{
	/*
	CString strDonator;
	m_pCM->GetControlMgrData(_T("기증자"), strDonator);
	INT ids = CheckDuplicateDonator(strDonator);
	if(ids < 0) 
	{
		ESLAfxMessageBox(_T("중복된 기증자가 존재합니다."));
	}
	*/
}

HBRUSH CDonatorMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (IsColorControl(pWnd->GetDlgCtrlID())) {
		return SetColor(pDC);
	}

	return hbr;

}

INT CDonatorMgr::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpenMode == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpenMode == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_firstidx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CDonatorMgr::GetPrevSelectIndex()
{
	//===============================
	//2009.04.08 UPDATE BY PJW : 비교를 위해서 초기값을 0으로 셋팅한다.
// 	INT nPrevSelIndex = -1;
 	INT nPrevSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : 여러개를 선정후 이전,다음 할경우 인덱스가 맞지 않음
		while( 1 )
		{
			if(nPrevSelIndex == m_nIdx || nPrevSelIndex < 0 )
			{
				break;
			}
			else if( nPrevSelIndex < m_nIdx )
			{
				nPrevSelIndex = m_pMainGrid->SelectGetNext();
				if( nPrevSelIndex < 0 )
				{
					nPrevSelIndex = m_pMainGrid->SelectGetTailPosition();
					break;
				}
			}
			else if( nPrevSelIndex > m_nIdx )
			{
				nPrevSelIndex = m_pMainGrid->SelectGetPrev();
			}
		}
		//================================================

		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_nIdx != m_firstidx)
			nPrevSelIndex = m_nIdx - 1;
	}
	return nPrevSelIndex;
}

INT CDonatorMgr::GetNextSelectIndex()
{
	//===============================
	//2009.04.08 UPDATE BY PJW : 비교를 위해서 초기값을 0으로 셋팅한다.
// 	INT nNextSelIndex = -1;
 	INT nNextSelIndex = 0;
	//===============================
	
	if(m_bGridSelectYN)
	{
		//================================================
		//2009.04.08 ADD BY PJW : 여러개를 선정후 이전,다음 할경우 인덱스가 맞지 않음
		while( 1 )
		{
			if(nNextSelIndex == m_nIdx || nNextSelIndex < 0 )
			{
				break;
			}
			else if( nNextSelIndex < m_nIdx )
			{
				nNextSelIndex = m_pMainGrid->SelectGetNext();
			}
			else if( nNextSelIndex > m_nIdx )
			{
				nNextSelIndex = m_pMainGrid->SelectGetPrev();
			}
		}
		//================================================	

		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_nIdx != m_lastidx)
			nNextSelIndex = m_nIdx + 1;
	}
	return nNextSelIndex;
}

VOID CDonatorMgr::OnKillfocusedtDONATORYEAR() 
{
	OnSelchangecmbDONATORFLAG();	
}

/////////////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 입력시 부여될 일련번호의 값을 구한다.
//
// [ 미니스펙 ]
// 1. 등록년도, 기증자구분을 가져와 일련번호의 마지막값을 구한다.
// 2. 구해온 일련번호의 값에 +1을 하여 새로운 일련번호를 부여한다.
//
// 2005.10.19 강봉수 추가
/////////////////////////////////////////////////////////////////////////////
VOID CDonatorMgr::OnSelchangecmbDONATORFLAG() 
{
	INT nSerialNo;
	CString strYear, strFlag, strSerialNo;

	//=======================================================================
	// 1. 등록년도, 기증자구분을 가져와 일련번호의 마지막값을 구한다.
	//=======================================================================
	m_pCM->GetControlMgrData(_T("등록년도"), strYear);
	m_pCM->GetControlMgrData(_T("기증자구분"), strFlag);	
	m_pLastNumber->GetLastNumber(_T("DON_ID"), _T("CO"), strYear, strFlag, strSerialNo);
	
	//=======================================================================
	// 2. 구해온 일련번호의 값에 +1을 하여 새로운 일련번호를 부여한다.
	//=======================================================================
	nSerialNo = _ttoi(strSerialNo);
	nSerialNo++;
	strSerialNo.Format(_T("%d"), nSerialNo);
	m_pCM->SetControlMgrData(_T("일련번호"), strSerialNo);
}



