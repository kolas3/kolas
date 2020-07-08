// Request_DataMgr.cpp : implementation file
//

#include "stdafx.h"
#include "Request_DataMgr.h"

/// 대출 공유
#include "../../../대출관리/LOAN_INFO_SHARE_MANAGER/LoanShareManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataMgr dialog


CRequest_DataMgr::CRequest_DataMgr(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CRequest_DataMgr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRequest_DataMgr)
	m_bSMSUseYN = FALSE;
	//}}AFX_DATA_INIT
	m_idx = -1;
	m_firstidx = -1;
	m_lastidx = -1;
	m_pNoteCtrl = NULL;
	m_pDM = NULL;
	m_pCM = NULL;
	m_pMainCM = NULL;
	m_pHandPhoneNo = NULL;
	m_pMainGrid = NULL;
	m_pUSERDM = NULL;
	m_bGridSelectYN = NULL;
	m_nOpType = OPERATION_TYPE_INVALID;
	pMakeSearchData = new CMakeSearchData(pParent); 
	m_pLoanMangeValue = NULL;
}

CRequest_DataMgr::~CRequest_DataMgr()
{
	if(m_pHandPhoneNo)
	{
		delete[] m_pHandPhoneNo;
		m_pHandPhoneNo = NULL;
	}
	if(pMakeSearchData)
	{
		delete pMakeSearchData; 
		pMakeSearchData = NULL; 
	}
	if(m_pLoanMangeValue)
	{
		delete m_pLoanMangeValue;
		m_pLoanMangeValue = NULL;
	}
}

VOID CRequest_DataMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRequest_DataMgr)
	DDX_Check(pDX, IDC_chkSMS_RECEIPT_YN, m_bSMSUseYN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRequest_DataMgr, CDialog)
	//{{AFX_MSG_MAP(CRequest_DataMgr)
	ON_BN_CLICKED(IDC_btnFINDZIPCODE, OnbtnFINDZIPCODE)
	ON_EN_KILLFOCUS(IDC_edtWANTED_TITLE, OnKillfocusedtWANTEDTITLE)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_btnREQUEST_PREV, OnbtnREQUESTPREV)
	ON_BN_CLICKED(IDC_btnREQUEST_NEXT, OnbtnREQUESTNEXT)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnREQ_TITLE_DUP_SEARCH, OnbtnREQTITLEDUPSEARCH)
	ON_BN_CLICKED(IDC_btnREQUEST_FIND_APPLICANT, OnbtnREQUESTFINDAPPLICANT)
	ON_BN_CLICKED(IDC_btnREQ_NEW_SAVE, OnbtnREQNEWSAVE)
	ON_EN_KILLFOCUS(IDC_edtNID, OnKillfocusedtNID)
	ON_MESSAGE(NOTE_CLICKED, OnNoteClicked)
	ON_EN_KILLFOCUS(IDC_edtREQUEST_PRICE, OnKillfocusedtREQUESTPRICE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRequest_DataMgr message handlers

BOOL CRequest_DataMgr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("K2UPGRADE_비치희망자료")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	

	m_pCM = FindCM(_T("CM_비치희망자료"));	
	m_pDM = FindDM(_T("DM_비치희망자료관리"));	
	m_pMainCM = FindCM(_T("CM_비치희망자료관리"));	
	m_pMainGrid = (CESL_Grid*)m_pMainCM->FindControl(_T("MainGrid"));
	m_pUSERDM = FindDM(_T("DM_BO_LOC_3300"));
	
	//필수 입력항목 컨트롤 색상 설정
	AddColorControl(IDC_edtAPPLICANT, _T(""), _T(""), 'E');
	AddColorControl(IDC_edtWANTED_TITLE, _T(""), _T(""), 'E');

	m_pHandPhoneNo = new CString[6];
	m_pHandPhoneNo[0] = _T("010");
	m_pHandPhoneNo[1] = _T("011");
	m_pHandPhoneNo[2] = _T("016");
	m_pHandPhoneNo[3] = _T("017");
	m_pHandPhoneNo[4] = _T("018");
	m_pHandPhoneNo[5] = _T("019");
	
	CMulitComboBoxManager mgr(this);
	mgr.SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T("011"), m_pHandPhoneNo, _T("핸드폰1"), 6);

	if(m_pInfo->GetSMSUseMode() < 1)
	{
		GetDlgItem(IDC_chkSMS_RECEIPT_YN)->ShowWindow(SW_HIDE);
		CRequestManager mgr(this);
		mgr.SetUDFCheck(m_pDM, _T("SMS수신여부"));
	}

	GetWindowRect(m_rcWindow);
	ShowCodeResource();

	if(m_nOpType == SINSERT)
	{
		CString strDate;
		CTime t = CTime::GetCurrentTime();
		CString strHour,strMin,strSec;
		strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());		// 입력일

		CButton * pBtn = (CButton*)GetDlgItem(IDC_btnREQUEST_PREV);
		CButton * pBtn1 = (CButton*)GetDlgItem(IDC_btnREQUEST_NEXT);
		CButton * pBtn2 = (CButton*)GetDlgItem(IDC_btnREQ_NEW_SAVE);
		pBtn->ShowWindow(SW_HIDE);
		pBtn1->ShowWindow(SW_HIDE);
		pBtn2->ShowWindow(SW_HIDE);
	}
	else
	{
		Display(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRequest_DataMgr::InitFirstLastIndex()
{
	INT ids = m_pMainGrid->SelectMakeList();
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

VOID CRequest_DataMgr::ShowNextPrevButton(INT idx)
{
	GetDlgItem(IDC_btnREQUEST_PREV)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnREQUEST_NEXT)->EnableWindow(TRUE);

	if(m_firstidx >= idx || idx < 0)
		GetDlgItem(IDC_btnREQUEST_PREV)->EnableWindow(FALSE);
	if(m_lastidx <= idx || idx < 0)
		GetDlgItem(IDC_btnREQUEST_NEXT)->EnableWindow(FALSE);
}

VOID CRequest_DataMgr::Display(INT ids)
{
	switch(ids)
	{
	case 0:
		InitFirstLastIndex();
		m_idx = GetFirstSelectIndex();
		ShowNextPrevButton(m_idx);
		break;
	case -1:
		m_idx = GetPrevSelectIndex();
		ShowNextPrevButton(m_idx);
		break;
	case 1:
		m_idx = GetNextSelectIndex();
		ShowNextPrevButton(m_idx);
		break;
	}
	
	m_pCM->AllControlDisplay(m_idx);
	
	CString strEmail, strHp, strFurnishStatus, strSMSReceipYN;
	m_pDM->GetCellData(_T("핸드폰"), m_idx, strHp);
	m_pDM->GetCellData(_T("EMAIL"), m_idx, strEmail);
	m_pDM->GetCellData(_T("비치상태"), m_idx, strFurnishStatus);
	m_pDM->GetCellData(_T("SMS수신여부"), m_idx, strSMSReceipYN);
	//20070615 UPDATE PJW : 우선대출예약여부 추가
	SetHandPhoneInfo(strHp);
	SetEmailInfo(strEmail);
	SetFurnishStatus(strFurnishStatus);
	
	if(strSMSReceipYN == _T("Y")) m_bSMSUseYN = TRUE;
	else m_bSMSUseYN = FALSE;
	UpdateData(FALSE);

	SetReverse(m_pMainCM->CM_ALIAS, _T("MainGrid"), m_idx);
}

INT CRequest_DataMgr::FindHandPhoneNo(CString strHpNo)
{
	for(INT i=0 ; i<6 ; i++)
	{
		if(m_pHandPhoneNo[i] == strHpNo)
			return i;
	}
	return -1;
}


VOID CRequest_DataMgr::OnbtnFINDZIPCODE() 
{
	// TODO: Add your control notification handler code here
	CSearchZipCodeDlg SearchZipCodeDlg(this);
	SearchZipCodeDlg.DoModal();

	// 결과물
	if(SearchZipCodeDlg.m_nCloseMode != 0)
	{
		m_pCM->SetControlMgrData(_T("우편번호"), SearchZipCodeDlg.m_sZipCode);
		m_pCM->SetControlMgrData(_T("주소"), SearchZipCodeDlg.m_sAddress);
	}	
}

VOID CRequest_DataMgr::OnKillfocusedtWANTEDTITLE() 
{
	// TODO: Add your control notification handler code here
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_edtWANTED_TITLE);
	if (pEdit == NULL) return;
/*
	// 리드 온리이면 체크하지 않는다.
	if (pEdit->GetStyle() & ES_READONLY) return;

	SetTimer(0, 1000, NULL);	
*/
}

VOID CRequest_DataMgr::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 0)
	{
		KillTimer(0);

		CString strTitle;
		CString strSearchCount, strSpeciesKey;

		INT ids = m_pCM->GetControlMgrData(_T("본표제"), strTitle);
		if (ids < 0) return;

		if (strTitle.IsEmpty()) return;

		CDuplicateSearch_Interface impl(this);
		if(m_nOpType != SINSERT)
		{
			m_pDM->GetCellData(_T("종KEY"), m_idx, strSpeciesKey);
		}
		ids = impl.ShowOnlyTitleDuplicateSearch(_T("비치희망복본조사"), strTitle, strSearchCount, strSpeciesKey);
		if (ids < 0) return;

		if (strSearchCount == _T("0"))	return;
		

		if (m_pNoteCtrl != NULL)
		{
			delete m_pNoteCtrl;
		}
		
		m_pNoteCtrl = new CNoteCtrl;
		CString strNote;
		strNote.Format(_T("%s건의 복본이 발견되었습니다."), strSearchCount);
		m_pNoteCtrl->SetNote(strNote);

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		point.y += 5;
		
		m_pNoteCtrl->Create(point, 200, 50, this);
		m_pNoteCtrl->ShowWindow(SW_SHOW);
	}		
	CDialog::OnTimer(nIDEvent);
}

VOID CRequest_DataMgr::OnbtnREQUESTPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

VOID CRequest_DataMgr::OnbtnREQUESTNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CRequest_DataMgr::OnOK() 
{
	// TODO: Add extra validation here
	CRequestManager pMgr(this);
	pMgr.InitTempData();
	CString sRRN, pubYear, strPrice;
	m_pCM->GetControlMgrData(_T("주민등록번호"), sRRN);
	m_pCM->GetControlMgrData(_T("발행년"), pubYear);
	m_pCM->GetControlMgrData(_T("가격"), strPrice);
	if(strPrice.IsEmpty()) 
	{
		strPrice = _T("0");
		m_pCM->SetControlMgrData(_T("가격"), strPrice);
	}
	INT ids;
	CString errstr;
	if(!pMgr.IsRRN(sRRN))
	{
		ESLAfxMessageBox(_T("입력하신 주민등록 번호가 올바르지 않습니다."));
		return;
	}
	
	ids = pMakeSearchData->IsValidYearData(pubYear);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("입력하신 발행년 정보가 올바르지 않습니다."));
		return;
	}
	if(!pMakeSearchData->CheckDigit(strPrice))
	{
		ESLAfxMessageBox(_T("입력하신 가격정보가 올바르지 않습니다."));
		return ;
	}
	
	ids = CheckInputFieldData(m_pCM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("필수입력항목을 채워주십시요."));
		return;
	}
	
	CString strSMSUseYN;
	UpdateData(TRUE);

	if(m_nOpType == SINSERT)
	{
		pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("DMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("SMSUSEYN"), m_bSMSUseYN);
		ids = pMgr.SPFExecutePath(_T("비치희망입력"));
	}
	else
	{
		pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("DMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("MainCMAlias"), m_pMainCM->CM_ALIAS.GetBuffer(0));
		pMgr.SetTempData(_T("SMSUSEYN"), m_bSMSUseYN);
		pMgr.SetTempData(_T("IDX"), m_idx);
		ids = pMgr.SPFExecutePath(_T("비치희망수정"));
	}
	if(ids < 0)
	{
		switch(ids)
		{
		case -1000:
			errstr = _T("비치희망자료 입력/수정을 위한 CM/DM을 찾을 수 없습니다.");
			break;
		case -1001:
			errstr = _T("입력/수정사항을 DataBase에 적용하던중 에러가 발생하였습니다.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	errstr = _T("비치희망 자료를 저장하였습니다.");
	ESLAfxMessageBox(errstr.GetBuffer(0));
	
	if(m_nOpType == SINSERT)
	{
		m_pMainGrid->DisplayLine(m_pDM->GetRowCount()-1);
		m_pCM->AllControlClear();
		CString strDate;
		CString strHour,strMin,strSec;
		CTime t = CTime::GetCurrentTime();
		strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());		// 입력일
		//20070607 UPDATE PJW : 시간 추가
		SetControlData(m_pCM->CM_ALIAS, _T("신청일자"), strDate);
	}
	else
	{
		m_pMainGrid->DisplayLine(m_idx);
		Display(1);
	}
	return;
}

INT CRequest_DataMgr::CheckInputFieldData(CESL_ControlMgr *pCM)
{
	if(!pCM) return -1;
	CString applicant, socialid, title, author, publisher;
	pCM->GetControlMgrData(_T("신청자"), applicant);
	//pCM->GetControlMgrData(_T("주민등록번호"), socialid);
	pCM->GetControlMgrData(_T("본표제"), title);
	//pCM->GetControlMgrData(_T("저작자"), author);
	//pCM->GetControlMgrData(_T("발행자"), publisher);
	if(applicant.IsEmpty() || title.IsEmpty())// || author.IsEmpty() || publisher.IsEmpty() || socialid.IsEmpty())
		return -1;
	return 0;
}

UINT CRequest_DataMgr::OnNoteClicked(LPARAM lParam, WPARAM wParam)
{
	// 복본 조사 들어간당
	CString strTmpData;

	CDuplicateSearch_Interface impl(this);
	impl.ShowTitleDuplicateSearchDlg(_T("서명자동복본조사"), strTmpData);
	
	return 0;
}

HBRUSH CRequest_DataMgr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (IsColorControl(pWnd->GetDlgCtrlID())) {
		return SetColor(pDC);
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

VOID CRequest_DataMgr::OnbtnREQTITLEDUPSEARCH() 
{
	// TODO: Add your control notification handler code here
	CString strTitle, strAuthor, strPublisher, strPubYear, strSearchCount, strTmpData;

	m_pCM->GetControlMgrData(_T("본표제"), strTitle);
	m_pCM->GetControlMgrData(_T("발행자"), strPublisher);
	m_pCM->GetControlMgrData(_T("발행년"), strPubYear);
	m_pCM->GetControlMgrData(_T("저작자"), strAuthor);
	
	if (strTitle.IsEmpty())
	{
		if(strAuthor.IsEmpty() && strPublisher.IsEmpty()) return;
	}
	
	CString strMarc;	strMarc.Empty();
	CMarc marc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &marc);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("245$a"), strTitle);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("245$d"), strAuthor);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("260$b"), strPublisher);
	m_pInfo->pMarcMgr->SetItem(&marc, _T("260$c"), strPubYear);
	m_pInfo->pMarcMgr->Encoding(&marc, strMarc);

	if(strMarc.IsEmpty()) return;
	
	CDuplicateSearch_Interface impl(this);

	//INT ids = impl.ShowMarcDuplicateSearch(_T("비치희망입력화면복본조사"), strMarc, strSearchCount);
	INT ids = impl.SearchDuplicateDataByIndividually(_T("건별복본조사"), strMarc, _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), strSearchCount);
	if (ids < 0) return;
	
	if (strSearchCount == _T("0"))
	{
		ESLAfxMessageBox(_T("복본조사 결과가 없습니다."));
		return;	
	}

	//impl.ShowTitleDuplicateSearchDlg(_T("비치희망입력화면복본조사"), strTmpData);
	impl.ShowTitleDuplicateSearchDlg(_T("비치희망입력화면복본조사"), strTmpData);
}

VOID CRequest_DataMgr::OnbtnREQUESTFINDAPPLICANT() 
{
	// TODO: Add your control notification handler code here

	CLoanShareManager LoanShareManager(this);
	
	if(m_pLoanMangeValue)
	{
		delete m_pLoanMangeValue;
		m_pLoanMangeValue = NULL;
	}

	m_pLoanMangeValue = new CLonaManageValue(this);
	CLoanUserManage UserDlg(this,&LoanShareManager,m_pLoanMangeValue,TRUE);
	UserDlg.pMain = this->pMain;
	UserDlg.pParentMgr = this;
	UserDlg.m_pInfo = this->m_pInfo;
	UserDlg.DoModal();
	
	CString sUserKey = UserDlg.m_sSelectRecKey;

	if( sUserKey.IsEmpty() ) return ;
	
	CString strQuery;
	strQuery.Format(_T("REC_KEY = %s"), sUserKey);
	m_pUSERDM->RefreshDataManager(strQuery);

	SetApplicantInfo();
}

INT CRequest_DataMgr::SetApplicantInfo()
{
	if(m_pUSERDM == NULL) return -1;
	if(m_pUSERDM->GetRowCount() == 0) return -1;

	m_pCM->SetControlMgrData(_T("신청자"), m_pUSERDM->GetCellData(_T("이름"), 0));
	m_pCM->SetControlMgrData(_T("우편번호"), m_pUSERDM->GetCellData(_T("자택우편번호"), 0));
	m_pCM->SetControlMgrData(_T("전화번호"), m_pUSERDM->GetCellData(_T("자택전화"), 0));
	m_pCM->SetControlMgrData(_T("주민등록번호"), m_pUSERDM->GetCellData(_T("주민등록번호"), 0));
	m_pCM->SetControlMgrData(_T("주소"), m_pUSERDM->GetCellData(_T("자택주소"), 0));
	CString strHP = m_pUSERDM->GetCellData(_T("휴대폰"), 0);
	CString strEMail = m_pUSERDM->GetCellData(_T("E_MAIL"), 0);

	SetHandPhoneInfo(strHP);
	SetEmailInfo(strEMail);
	return 0;
}

VOID CRequest_DataMgr::SetHandPhoneInfo(CString strHP)
{
	CString strHpNo1, strHpNo2, strHpNo3;
	CStringArray arrStr;
	CRequestManager mgr(this);
	mgr.ParsingString(_T("-"), strHP, &arrStr);
	INT cnt = arrStr.GetSize();
	if(cnt > 0) strHpNo1 = arrStr.GetAt(0);
	if(cnt > 1) strHpNo2 = arrStr.GetAt(1);
	if(cnt > 2) strHpNo3 = arrStr.GetAt(2);

	m_pCM->SetControlMgrData(_T("핸드폰2"), strHpNo2);
	m_pCM->SetControlMgrData(_T("핸드폰3"), strHpNo3);
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("핸드폰1"));
	INT nPos = FindHandPhoneNo(strHpNo1);
	pCmb->SetCurSel(nPos);
}

VOID CRequest_DataMgr::SetEmailInfo(CString strEMail)
{
	CString strEmailID, strEmailSite;
	CStringArray arrStr;
	CRequestManager mgr(this);
	mgr.ParsingString(_T("@"), strEMail, &arrStr, FALSE);
	INT cnt = arrStr.GetSize();
	if(cnt > 0) strEmailID = arrStr.GetAt(0);
	if(cnt > 1) strEmailSite = arrStr.GetAt(1);
	strEmailID.MakeLower();		strEmailSite.MakeLower();
	m_pCM->SetControlMgrData(_T("EMAIL_ID"), strEmailID);
	
	CString strEmailSiteCode;
	m_pInfo->pCodeMgr->ConvertDescToCode(_T("메일목록"), strEmailSite, strEmailSiteCode);
	if(strEmailSiteCode.IsEmpty() && !strEmailSite.IsEmpty())
	{
		CMulitComboBoxManager mgr(this);
		CString strData[1][2] = {strEmailSite, _T("")};
		mgr.AddMultiComboBoxStrData(m_pCM->CM_ALIAS, strData, _T("EMAIL_SITE"));
	}
	m_pCM->SetControlMgrData(_T("EMAIL_SITE"), strEmailSite);
}

INT CRequest_DataMgr::GetFirstSelectIndex()
{
	INT nFirstSelIdx = -1;
	if(m_nOpType == GUPDATE)
	{
		nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	else if(m_nOpType == SUPDATE)
	{
		if(m_bGridSelectYN)
			nFirstSelIdx = m_firstidx;
		else
			nFirstSelIdx = m_pMainGrid->GetRow() - 1;
	}
	if(nFirstSelIdx < 0) nFirstSelIdx = 0;
	return nFirstSelIdx;
}

INT CRequest_DataMgr::GetPrevSelectIndex()
{
	INT nPrevSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nPrevSelIndex = m_pMainGrid->SelectGetPrev();
		if(nPrevSelIndex < 0)
		{
			nPrevSelIndex = m_pMainGrid->SelectGetHeadPosition();
		}
	}
	else
	{
		if(m_idx != m_firstidx)
			nPrevSelIndex = m_idx - 1;
	}
	return nPrevSelIndex;
}

INT CRequest_DataMgr::GetNextSelectIndex()
{
	INT nNextSelIndex = -1;
	if(m_bGridSelectYN)
	{
		nNextSelIndex = m_pMainGrid->SelectGetNext();
		if(nNextSelIndex < 0)
		{
			nNextSelIndex = m_pMainGrid->SelectGetTailPosition();
		}
	}
	else
	{
		if(m_idx != m_lastidx)
			nNextSelIndex = m_idx + 1;
		else nNextSelIndex = m_idx;
	}
	return nNextSelIndex;
}

VOID CRequest_DataMgr::ShowCodeResource()
{
	INT i = 0;
	INT nCodeResourcesHeight = 115;
	CWnd *pWnd = NULL;
	
	CRect rcWindow;
	GetWindowRect(rcWindow);
	if (m_nOpType != SINSERT && rcWindow == m_rcWindow) return;

	INT nCount = 0;
	INT nArrCodeResourceID[256];
	nArrCodeResourceID[nCount++] = IDC_stcREQ_DATA_STATUS;
	nArrCodeResourceID[nCount++] = IDC_stcREQ_FURNISH_STATUS;
	nArrCodeResourceID[nCount++] = IDC_edtREQ_FURNISH_STATUS;
	nArrCodeResourceID[nCount++] = IDC_stcREQ_CANCEL_REASON;
	nArrCodeResourceID[nCount++] = IDC_richREQ_CANCEL_REASON;
	
	// Show / Hide Code Resource
	for (i = 0; i < nCount; i++)
	{
		pWnd = GetDlgItem(nArrCodeResourceID[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			if (m_nOpType != SINSERT)
				pWnd->ShowWindow(SW_SHOW);
			else
				pWnd->ShowWindow(SW_HIDE);
		}
	}

	// Move Other Resources
	nCount = 0;
	INT nArrOtherResources[256];
	nArrOtherResources[nCount++] = IDC_stcREQ_BUTTON_GROUP;
	nArrOtherResources[nCount++] = IDC_btnREQUEST_PREV;
	nArrOtherResources[nCount++] = IDC_btnREQUEST_NEXT;
	nArrOtherResources[nCount++] = IDC_btnREQ_TITLE_DUP_SEARCH;
	nArrOtherResources[nCount++] = IDOK;
	nArrOtherResources[nCount++] = IDCANCEL;

	CRect rcResource;
	for (i = 0; i <nCount ; i++)
	{
		pWnd = GetDlgItem(nArrOtherResources[i]);
		if (pWnd->GetSafeHwnd() != NULL)
		{
			pWnd->GetWindowRect(rcResource);
			ScreenToClient(rcResource);
			if (m_nOpType != SINSERT)
			{
				rcResource.top += nCodeResourcesHeight;
				rcResource.bottom += nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
			else
			{
				rcResource.top -= nCodeResourcesHeight;
				rcResource.bottom -= nCodeResourcesHeight;
				pWnd->MoveWindow(rcResource);
			}
		}
	}

	// Resize Window Size
	GetWindowRect(rcResource);
	if (m_nOpType != SINSERT)
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height(),  SWP_NOMOVE | SWP_NOZORDER);
	else
		SetWindowPos(this, rcResource.left, rcResource.top, rcResource.Width(), m_rcWindow.Height() - nCodeResourcesHeight,  SWP_NOMOVE | SWP_NOZORDER);
	
	Invalidate();
	UpdateWindow();
}

VOID CRequest_DataMgr::SetFurnishStatus(CString strFurnishStatus)
{
	CString strDesc;
	if(strFurnishStatus.IsEmpty()) strDesc = _T("신청중");
	else if(strFurnishStatus == _T("1")) strDesc = _T("신청중");
	else if(strFurnishStatus == _T("2")) strDesc = _T("처리중");
	else if(strFurnishStatus == _T("3")) strDesc = _T("소장중");
	else if(strFurnishStatus == _T("4")) strDesc = _T("취소됨");
	m_pCM->SetControlMgrData(_T("비치상태"), strDesc);

/*
	if(strFurnishStatus != _T("4"))
	{
		((CEdit*)GetDlgItem(IDC_richREQ_CANCEL_REASON))->SetReadOnly(TRUE);
	}
*/
}

VOID CRequest_DataMgr::OnbtnREQNEWSAVE() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add extra validation here
	CRequestManager pMgr(this);
	pMgr.InitTempData();
	CString sRRN, pubYear, strPrice;
	m_pCM->GetControlMgrData(_T("주민등록번호"), sRRN);
	m_pCM->GetControlMgrData(_T("발행년"), pubYear);
	m_pCM->GetControlMgrData(_T("가격"), strPrice);
	if(strPrice.IsEmpty()) 
	{
		strPrice = _T("0");
		m_pCM->SetControlMgrData(_T("가격"), strPrice);
	}
	INT ids;
	CString errstr;
	if(!pMgr.IsRRN(sRRN))
	{
		ESLAfxMessageBox(_T("입력하신 주민등록 번호가 올바르지 않습니다."));
		return;
	}
	
	ids = pMakeSearchData->IsValidYearData(pubYear);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("입력하신 발행년 정보가 올바르지 않습니다."));
		return;
	}
	if(!pMakeSearchData->CheckDigit(strPrice))
	{
		ESLAfxMessageBox(_T("입력하신 가격정보가 올바르지 않습니다."));
		return ;
	}
	
	ids = CheckInputFieldData(m_pCM);
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("필수입력항목을 채워주십시요."));
		return;
	}
	pMgr.SetTempData(_T("CMAlias"), m_pCM->CM_ALIAS.GetBuffer(0));
	pMgr.SetTempData(_T("DMAlias"), m_pDM->DB_MGR_ALIAS.GetBuffer(0));
	ids = pMgr.SPFExecutePath(_T("비치희망입력"));
	if(ids < 0)
	{
		switch(ids)
		{
		case -1000:
			errstr = _T("비치희망자료 입력/수정을 위한 CM/DM을 찾을 수 없습니다.");
			break;
		case -1001:
			errstr = _T("입력/수정사항을 DataBase에 적용하던중 에러가 발생하였습니다.");
			break;
		}
		ESLAfxMessageBox(errstr.GetBuffer(0));
		return;
	}
	errstr = _T("비치희망 자료를 저장하였습니다.");
	ESLAfxMessageBox(errstr.GetBuffer(0));
	
	m_pMainGrid->DisplayLine(m_pDM->GetRowCount()-1);	
	Display(1);
	return;	
}

VOID CRequest_DataMgr::OnKillfocusedtNID() 
{
	// TODO: Add your control notification handler code here
	CRequestManager pMgr(this);
	CString strCivilNo;
	m_pCM->GetControlMgrData(_T("주민등록번호"), strCivilNo);
	if(strCivilNo.IsEmpty()) return ;
	((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strCivilNo);
	if(strCivilNo.GetLength() != 13) 
	{
		ESLAfxMessageBox(_T("올바른 주민등록번호가 아닙니다."));
		GetDlgItem(IDC_edtNID)->SetFocus();
		return;
	}
	CString strHighNum, strLowNum;
	strHighNum = strCivilNo.Left(6);
	strLowNum = strCivilNo.Right(7);
	strCivilNo = strHighNum + _T("-") + strLowNum;
	if(!pMgr.IsRRN(strCivilNo))
	{
		ESLAfxMessageBox(_T("올바른 주민등록번호가 아닙니다."));
		m_pCM->SetControlMgrData(_T("주민등록번호"), _T(""));
	}
	else
		m_pCM->SetControlMgrData(_T("주민등록번호"), strCivilNo);
	return;
}

BOOL CRequest_DataMgr::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_CHAR)
    {
		if(GetDlgItem(IDC_edtNID)->m_hWnd == pMsg->hwnd || GetDlgItem(IDC_edtTEL_NO)->m_hWnd == pMsg->hwnd)
		{
			INT nVirtKey = (INT) pMsg->wParam;
			if ((nVirtKey < '0' || nVirtKey > '9') && (nVirtKey != '-') && nVirtKey != 8)
			{
				ESLAfxMessageBox(_T("숫자만 입력하실 수 있습니다.")); 
				return TRUE;
			}
		}
    }	
	return CDialog::PreTranslateMessage(pMsg);
}

VOID CRequest_DataMgr::OnKillfocusedtREQUESTPRICE() 
{
	// TODO: Add your control notification handler code here
	CString strPrice;
	m_pCM->GetControlMgrData(_T("가격"), strPrice);
	CString strTmpData;
	INT nPos = strPrice.Find(_T("."));
	if(nPos > 0) strTmpData = strPrice.Left(nPos);
	else strTmpData = strPrice;
	if(strTmpData.GetLength() > 8)
	{
		ESLAfxMessageBox(_T("가격정보는 소수점위 자료가 8자리 이상일 수 없습니다."));
		GetDlgItem(IDC_edtREQUEST_PRICE)->SetFocus();
		return ;
	}
}
