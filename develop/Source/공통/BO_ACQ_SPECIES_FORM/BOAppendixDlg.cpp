// BOAppendixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOAppendixDlg.h"
#include "AccessionNoSearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg dialog


CBOAppendixDlg::CBOAppendixDlg(CESL_Mgr* pParent /*= NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOAppendixDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM_Appendix = NULL;
	m_pDM_Species = NULL;
	m_pDM_Index = NULL;
	m_pFormApi = NULL;

	m_pNoteCtrl = NULL;

	m_nSelectedRow = -1;

	m_bReadOnly = FALSE;
	m_bCreateFormApi = FALSE;
	m_nOpenMode = 0;
	m_bAutoRegYN = FALSE;
}

CBOAppendixDlg::~CBOAppendixDlg()
{
	ApplySpeciesToAppendixData();
	
	if(m_bCreateFormApi && m_pFormApi)
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}
	DeleteNoteCtrl();
}

BOOL CBOAppendixDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CBOAppendixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOAppendixDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOAppendixDlg, CDialog)
	//{{AFX_MSG_MAP(CBOAppendixDlg)
	ON_BN_CLICKED(IDC_bINSERT_APPENDIX, OnbINSERTAPPENDIX)
	ON_BN_CLICKED(IDC_bSAVE_APPENDIX, OnbSAVEAPPENDIX)
	ON_BN_CLICKED(IDC_bDELETE_APPENDIX, OnbDELETEAPPENDIX)
	ON_BN_CLICKED(IDC_bCLEARSCREEN, OnbCLEARSCREEN)
	ON_CBN_SELCHANGE(IDC_cREG_CLASS, OnSelchangecREGCLASS)
	ON_EN_KILLFOCUS(IDC_edtAPPENDIX_REG_NO, OnKillfocusedtAPPENDIXREGNO)
	ON_BN_CLICKED(IDC_btnSET_ACCESSION_REC_NO, OnbtnSETACCESSIONRECNO)
	ON_WM_CTLCOLOR()
	ON_CBN_SETFOCUS(IDC_cREG_CLASS, OnSetfocuscREGCLASS)
	ON_CBN_KILLFOCUS(IDC_cREG_CLASS, OnKillfocuscREGCLASS)
	ON_BN_CLICKED(IDC_bSPECIES_COPY, OnbSPECIESCOPY)
	ON_MESSAGE(NOTE_CLICKED, OnNoteClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg Operations
INT CBOAppendixDlg::Init()
{
	// 부록 정보 만든다.
	if(m_pDM_Appendix == NULL)
		m_pDM_Appendix = FindDM(_T("DMUP_단행_종_부록정보"));

	if (m_pDM_Appendix == NULL) return -1;

	m_pCM = FindCM(_T("CMUP_단행_부록_부록정보"));
	if (m_pCM == NULL) return -1;
	m_pGrid_Appendix = (CESL_Grid*)m_pCM->FindControl(_T("Grid_Appendix"));
	
	if(m_pFormApi == NULL)
	{
		m_pFormApi = new CBOFormApi(this);
		if(m_nOpenMode > 0) m_pFormApi->SetAppMode(MODE_INSERT);
		else m_pFormApi->SetAppMode(MODE_UPDATE);
		m_bCreateFormApi = TRUE;
	}
		
	CString strLinkDM;
	strLinkDM.Format(_T(":%s"), m_pDM_Appendix->DB_MGR_ALIAS);
	CString strLinkData = m_pGrid_Appendix->ELEMENT->LINK_DATA;
	strLinkData.Replace(_T(":DMUP_단행_종_부록정보"), strLinkDM);
	m_pGrid_Appendix->ELEMENT->LINK_DATA = strLinkData;
	m_pGrid_Appendix->InitColumnLinkDMAndAlias();
	m_pGrid_Appendix->InitColumnInfo();

	CESL_Control_Element * pCE = NULL;

	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		strLinkData = pCE->LINK_DATA;
		strLinkData.Replace(_T(":DMUP_단행_종_부록정보"), strLinkDM);
		pCE->LINK_DATA = strLinkData;
		pCE->InitControl();
	}

	CString strValue, strQuery;
	strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='부록등록번호자동부여여부'");
//DEL 	strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='부록등록번호자동부여여부'");
	m_pDM_Appendix->GetOneValueQuery(strQuery, strValue);
	if(strValue == _T("Y")) m_bAutoRegYN = TRUE;
	else m_bAutoRegYN = FALSE;

	//현재 선택 DM Index설정
	if(m_pDM_Appendix->GetRowCount() > 0) 
	{
		m_nSelectedRow = 0;
		m_pGrid_Appendix->Display();
	}
	
	SetAppendixRegNoCtrl();
	Display();

	return 0;
}

INT CBOAppendixDlg::Display()
{
	CESL_Control_Element * pCE = NULL;
	INT nCnt;

	nCnt = m_pDM_Appendix->GetRowCount();
	if ( nCnt <= 0 ) return 0;

	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;
		if(m_nSelectedRow >= 0) pCE->Display(m_nSelectedRow);
	}

	

	//등록번호 설정
	SetRegNoCtrl();
	// 부록이 없을 경우 저장버튼, 삭제버튼 Disable
	if (nCnt < 1)
	{
		GetDlgItem(IDC_bSAVE_APPENDIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDELETE_APPENDIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_btnSET_ACCESSION_REC_NO)->EnableWindow(FALSE);
		SetAcqCodeCtrl();
	}
	else
	{
		GetDlgItem(IDC_bSAVE_APPENDIX)->EnableWindow(TRUE);
		GetDlgItem(IDC_bDELETE_APPENDIX)->EnableWindow(TRUE);
		GetDlgItem(IDC_btnSET_ACCESSION_REC_NO)->EnableWindow(TRUE);
	}

	if (m_bReadOnly)
		DisplayControlReadOnly();
	
	///2008.03.20 add by pdw
	///
	//m_pGrid_Appendix->Display();
	INT nRows = m_pDM_Appendix->GetRowCount();
	for (INT idx = 0; idx < nRows; idx++)
	{
		if (m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), idx) == _T("D"))
		{
			m_pGrid_Appendix->SetRowHeight(idx + 1, 0);			
		}
	}

	if(m_nSelectedRow >= 0) m_pGrid_Appendix->SetReverse(m_nSelectedRow);
	return 0;
}

INT CBOAppendixDlg::DisplayControlReadOnly()
{
	m_pCM->AllControlReadOnly();

	INT nButtonCount = 0;
	INT nArrButtonCtrl[16];
	
	nArrButtonCtrl[nButtonCount++] = IDC_bINSERT_APPENDIX;
	nArrButtonCtrl[nButtonCount++] = IDC_bSAVE_APPENDIX;
	nArrButtonCtrl[nButtonCount++] = IDC_bCLEARSCREEN;
	nArrButtonCtrl[nButtonCount++] = IDC_bDELETE_APPENDIX;
	nArrButtonCtrl[nButtonCount++] = IDC_btnSET_ACCESSION_REC_NO;
	
	CWnd *pWnd = NULL;

	for (INT i = 0; i < nButtonCount; i++)
	{
		pWnd = GetDlgItem(nArrButtonCtrl[i]);
		if (pWnd == NULL) continue;

		pWnd->EnableWindow(FALSE);
	}

	return 0;
}

INT CBOAppendixDlg::InsertAppendix()
{
	//부록등록번호 검사
	if(!CheckRegNo(TRUE)) return -1;
	m_pDM_Appendix->InsertRow(-1);
	INT nRowCount = m_pDM_Appendix->GetRowCount();
	
	//권일련번호 설정
	CString strVolSortNo;
	m_pFormApi->GetLastVolSortNo(m_pDM_Appendix, strVolSortNo);
	m_pCM->SetControlMgrData(_T("BB_권일련번호"), strVolSortNo);
	
	// 화면정보 수정
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Appendix, nRowCount-1);
	//등록번호 설정
	CString strRegNo;

	if(m_bAutoRegYN) 
	{
		strRegNo = _T("");
		//===================================================
		//2009.10.22 ADD BY PJW : 부록입력이고 자동부여일경우는 컨트롤에 NULL을 셋팅한다
		m_pCM->SetControlMgrData(_T("BB_등록번호"), strRegNo);
		//===================================================
	}
	else 
	{
		strRegNo = SetRegNoDMField();
	}
	m_pDM_Appendix->SetCellData(_T("BB_등록번호"), strRegNo, nRowCount-1);
		
	// 부록 플래그 추가
	m_pDM_Appendix->SetCellData(_T("BB_책부록플래그"), _T("A"), nRowCount - 1);

	// 쿼리 플래그 세팅
	CString strQueryFlag = m_pFormApi->GetQueryFlag(MODE_INSERT);
	m_pDM_Appendix->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, nRowCount - 1);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBOAppendixDlg message handlers

BOOL CBOAppendixDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr(_T("K2UPGRADE_단행_부록"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	if(m_nOpenMode == 0)
	{
		ESLAfxMessageBox(_T("부록화면 모드를 설정해 주십시요."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	//필수 입력항목 컨트롤 색상 설정
	AddColorControl(IDC_eAPPENDIX_TITLE, _T("CMUP_단행_부록_부록정보"), _T(""), 'E');
	//AddColorControl(IDC_cMEDIA_CLASS, _T("CMUP_단행_부록_부록정보"), _T("BB_매체구분_코드"), 'M');	

	ids = Init();
	if (ids < 0)
	{
		AfxMessageBox(_T("Init Data Manger Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOAppendixDlg::OnbINSERTAPPENDIX() 
{
	INT ids = -1;
	//필수입력항목 검사
	if(!ValidateData()) return ;

	ids = InsertAppendix();
	if(ids < 0) return;
	ESLAfxMessageBox(_T("부록정보를 추가하였습니다."));
	// 마지막 정보화면에 뿌리기
	m_nSelectedRow = m_pDM_Appendix->GetRowCount() - 1;
	CESL_Grid *pGrid = (CESL_Grid*)GetDlgItem(IDC_gBO_APPENDIX);
	/// 2008.03.20 add by pdw
	/// 
	///m_pGrid_Appendix->Display();
	m_pGrid_Appendix->DisplayLine(m_nSelectedRow);
	m_pGrid_Appendix->SetReverse(m_nSelectedRow);
	
	Display();	
}

VOID CBOAppendixDlg::OnbSAVEAPPENDIX() 
{
	if (m_nSelectedRow < 0) return;

	m_pFormApi->InitDefaultIgnoreAlias();
	m_pFormApi->AddDefaultIgnoreAlias(_T("BB_등록번호"));

	INT ids = -1;

	//필수입력항목 검사
	if(!ValidateData()) return ;

	//===================================================
	//2009.05.28 ADD BY LKS : Control Trim
	TrimControl();
	//===================================================

	//부록등록번호 검사
	if(!CheckRegNo(FALSE)) return ;
	
	// 화면정보 수정
	m_pFormApi->MakeDMFromCM(m_pCM, m_pDM_Appendix, m_nSelectedRow);
	//등록번호 설정
	m_pDM_Appendix->SetCellData(_T("BB_등록번호"), SetRegNoDMField(), m_nSelectedRow);

	// 쿼리 플래그 세팅 ========= [2003/10/01/박현석]
	CString strFlagSetting = m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), m_nSelectedRow);
	INT nFlagSetting;
	if (strFlagSetting == _T("I"))
		nFlagSetting = MODE_INSERT;
	else if (strFlagSetting == _T("D"))
		nFlagSetting = MODE_DELETE;
	else
		nFlagSetting = MODE_UPDATE;
	CString strQueryFlag = m_pFormApi->GetQueryFlag(nFlagSetting);
	m_pDM_Appendix->SetCellData(_T("BB_DB_FLAG"), strQueryFlag, m_nSelectedRow);

	ESLAfxMessageBox(_T("부록정보를 수정하였습니다."));
	// 현재 선택된 정보 화면에 다시 뿌리기
	// 현재 선택된 정보 화면에 다시 뿌리기
	/// 2008.03.20 add by pdw
	//
	//m_pGrid_Appendix->Display();
	/******************************************
	 ******************************************/
	//20080319 ADD BY PJW : 그리드의 선정을 유지하기 위해서 재선정 해준다.
	//20080516 UPDADTE BY PJW : 삭제된데이터 보이지 않도록 수정 (m_pGrid_Appendix->Display(); -> Display();)
	//2008.07.16 UPDATE BY PJW : 수정버튼 눌렀을경우 삭제 부록이 보이지 않도록 DISPLAY()제일아래로 내림
// 	Display();
	INT nSelect;
	CString strTemp;
	INT nCnt;
	m_pGrid_Appendix->SelectMakeList();
	nCnt = m_pGrid_Appendix->SelectGetCount();
	nSelect = m_pGrid_Appendix->SelectGetHeadPosition();
	m_pGrid_Appendix->Display();
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		m_pGrid_Appendix->SetTextMatrix( nSelect+1, 1,_T("V"));
		nSelect = m_pGrid_Appendix->SelectGetNext();
	}
	Display();
	/******************************************
	 ******************************************/
	//m_pGrid_Appendix->DisplayLine(m_nSelectedRow);
	m_pGrid_Appendix->SetReverse(m_nSelectedRow);
}

VOID CBOAppendixDlg::OnbDELETEAPPENDIX() 
{
	/*if (m_nSelectedRow < 0) return;

	CString strVolTitle;
	m_pCM->GetControlMgrData(_T("BB_편제명부록명"), strVolTitle);
		
	CString strMsg;
	strMsg.Format(_T("부록명이 [%s]인 부록을 삭제하시겠습니까?"), strVolTitle);
	if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDNO) return;

	// 삭제
	m_pFormApi->DeleteBookDMRow(m_pDM_Appendix, m_nSelectedRow);

	// 현재 선택된 정보 화면에 다시 뿌리기
	m_pGrid_Appendix->Display();
	m_pGrid_Appendix->SetReverse(m_nSelectedRow);
	
	Display();*/
	if (m_nSelectedRow < 0) return;

	CString strVolTitle;
	CString strWorkingStatus;
	INT nSelect;
	CString strChk;
	INT nCnt;

	m_pGrid_Appendix->SelectMakeList();
	nCnt = m_pGrid_Appendix->SelectGetCount();	
	//===================================================
	//2009.10.05 ADD BY PJW : 
//	nSelect = m_pGrid_Appendix->SelectGetHeadPosition();
	nSelect = m_pGrid_Appendix->SelectGetTailPosition();
	//===================================================
	
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		//대출중인 부록은 삭제할 수 없다.
		strWorkingStatus = m_pDM_Appendix->GetCellData(_T("BB_자료상태"), nSelect);
		
		m_pDM_Appendix->GetCellData(_T("BB_편제명부록명"), nSelect, strVolTitle);

		if( strWorkingStatus == _T("BOL211O")
			|| strWorkingStatus == _T("BOL212O")
			|| strWorkingStatus == _T("BOL213O")
			|| strWorkingStatus == _T("BOL214O")
			|| strWorkingStatus == _T("BOL215O")
			|| strWorkingStatus == _T("BOL217O")
			// 18.09.27 JMJ KOL.RED.2018.008
			|| strWorkingStatus == _T("BOL411O")
			|| strWorkingStatus == _T("BOL511O")
			// 2013.12.01 ADD BY BJS : 2013도서관통합서비스확대구축및개선 (통합도서타관대출 상태 추가)
			|| strWorkingStatus == _T("BOL611O")
			|| strWorkingStatus == _T("BOL218O") 
			// 17/07/20 김혜영 : 책나래대출자료 상태 추가
//*/ ADD ---------------------------------------------------------------------------
			|| strWorkingStatus == _T("BOL219O")
//*/ END ---------------------------------------------------------------------------	 
			)
		{
			AfxMessageBox(_T("대출중인 부록은 삭제할 수 없습니다."));
			return;
		}
		else
		{
			CString strMsg;
			strMsg.Format(_T("부록명이 [%s]인 부록을 삭제하시겠습니까?"), strVolTitle);
			if (MessageBox(strMsg, NULL, MB_ICONQUESTION | MB_YESNO) == IDYES) 
			{
				// 삭제
				m_pFormApi->DeleteBookDMRow(m_pDM_Appendix, nSelect);
			}
			nSelect = m_pGrid_Appendix->SelectGetPrev();			
		}
	}

	//===================================================
	//2010.05.02 ADD BY PJW : 삭제 되었음으로 새롭 SELECT LIST를 만든다.
//	m_nSelectedRow = m_pGrid_Appendix->SelectGetHeadPosition();
//	m_pGrid_Appendix->Display();
	m_pGrid_Appendix->SetProtectSelectColumn(TRUE);
	m_pGrid_Appendix->Display();
	m_pGrid_Appendix->SetProtectSelectColumn(FALSE);	
	m_pGrid_Appendix->SelectMakeList();
	//===================================================	
	
	if( 0 > m_nSelectedRow )
	{
		m_pGrid_Appendix->SetReverse(0);
	}
	else if( 0 < m_nSelectedRow )
	{
		m_pGrid_Appendix->SetReverse(m_nSelectedRow);
	}
	Display();
}

BEGIN_EVENTSINK_MAP(CBOAppendixDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOAppendixDlg)
	ON_EVENT(CBOAppendixDlg, IDC_gBO_APPENDIX, -605 /* MouseDown */, OnMouseDowngBOAPPENDIX, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CBOAppendixDlg, IDC_gBO_APPENDIX, 69 /* SelChange */, OnSelChangegBOAPPENDIX, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBOAppendixDlg::OnMouseDowngBOAPPENDIX(short Button, short Shift, long x, long y) 
{
	INT nRow = 	m_pGrid_Appendix->GetRow();
	if (nRow == 0) return;

	INT nCol = m_pGrid_Appendix->GetCol();
	if ( nCol == 1) return;

	m_nSelectedRow = nRow - 1;
	Display();
}

VOID CBOAppendixDlg::OnbCLEARSCREEN() 
{
	CESL_Control_Element * pCE = NULL;

	POSITION pos = m_pCM->Control_List.GetHeadPosition();
	while (pos)
	{
		pCE = (CESL_Control_Element*)m_pCM->Control_List.GetNext(pos);
		if (pCE == NULL) continue;
		if (pCE->CTRL_TYPE == _T("G")) continue;

		pCE->Clear();
	}

	return;	
}

VOID CBOAppendixDlg::OnSelChangegBOAPPENDIX() 
{
	// TODO: Add your control notification handler code here
	INT nRow, nCnt;

	nRow = m_pGrid_Appendix->GetRow();
	nCnt = m_pDM_Appendix->GetRowCount();
	if (nRow == 0 || nCnt == 0 ) return;

	m_nSelectedRow = nRow - 1;
	Display();	
}

VOID CBOAppendixDlg::SetAppendixRegNoCtrl()
{	
	if(m_bAutoRegYN && m_nOpenMode > 0)
	{
		GetDlgItem(IDC_stcAPPENDIX_REG_NO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_edtAPPENDIX_REG_NO)->ShowWindow(SW_HIDE);
	}
	else if(m_bAutoRegYN && m_nOpenMode < 0)
	{
		GetDlgItem(IDC_stcAPPENDIX_REG_NO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAPPENDIX_REG_NO)->ShowWindow(SW_SHOW);
		((CEdit*)GetDlgItem(IDC_edtAPPENDIX_REG_NO))->SetReadOnly(TRUE);
	}
	else 
	{
		GetDlgItem(IDC_stcAPPENDIX_REG_NO)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_edtAPPENDIX_REG_NO)->ShowWindow(SW_SHOW);
	}
}

VOID CBOAppendixDlg::OnSelchangecREGCLASS() 
{
	// TODO: Add your control notification handler code here
	if(m_bAutoRegYN) return;

	if(!CheckValidAppendixRegCode())
	{
		ESLAfxMessageBox(_T("부록에 대한 등록구분이 아닙니다."));
		m_pCM->SetControlMgrData(_T("BB_등록구분_코드"), m_pDM_Appendix->GetCellData(_T("BB_등록구분_코드"), m_nSelectedRow));
		return;
	}
}

BOOL CBOAppendixDlg::CheckValidAppendixRegCode()
{
	CString strRegCode;
	m_pCM->GetControlMgrData(_T("BB_등록구분_코드"), strRegCode);
	if(strRegCode.IsEmpty())
	{
		((CEdit*)GetDlgItem(IDC_edtAPPENDIX_REG_NO))->SetReadOnly(TRUE);
		return TRUE;
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_edtAPPENDIX_REG_NO))->SetReadOnly(FALSE);
	}

	CString strQuery, strResult;
	strQuery.Format(_T("SELECT MAX(REC_KEY) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='REG_NO' AND DIVISION_VALUE='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strRegCode);
//DEL 	strQuery.Format(_T("SELECT MAX(REC_KEY) FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='REG_NO' AND DIVISION_VALUE='%s'"), strRegCode);
	m_pDM_Appendix->GetOneValueQuery(strQuery, strResult);
	
	if(m_bAutoRegYN)
	{
		if(strResult.IsEmpty()) 
		{
			return FALSE;
		}
	}
	else	
	{
		if(!strResult.IsEmpty()) 
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

VOID CBOAppendixDlg::OnKillfocusedtAPPENDIXREGNO() 
{
	// TODO: Add your control notification handler code here
	CheckRegNo(FALSE);
}

BOOL CBOAppendixDlg::CheckDuplicateAppendixRegNo(BOOL bInsert)
{
	CString strRegNo, strRegCode, strQuery, strResult;
	m_pCM->GetControlMgrData(_T("BB_등록구분_코드"), strRegCode);
	m_pCM->GetControlMgrData(_T("BB_등록번호"), strRegNo);
	if(strRegCode.IsEmpty() || strRegNo.IsEmpty()) return TRUE;
	
	if(!CheckIsDigit(strRegNo)) return FALSE;
	// 2004.10.14 등록번호 큰숫자 오류 수정.
	/*INT nRegNo = _ttoi(strRegNo.GetBuffer(0));
	if(nRegNo < 0) return FALSE;
	strRegNo.Format(_T("%s%010d"), strRegCode, nRegNo);*/
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
	__int64 nRegNo = _ttoi64(strRegNo.GetBuffer(0));
	if(nRegNo < 0) return FALSE;
// 	strRegNo.Format(_T("%s%010I64d"), strRegCode, nRegNo);
	strRegNo = m_pInfo->MakeRegNo(strRegCode, strRegNo);
	///===================================================================

	CString strTmpRegNo, strTmpRecKey, strRecKey, strDBFlag;
	BOOL bDuplicate = FALSE;
	INT nRowCnt = m_pDM_Appendix->GetRowCount();
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		strDBFlag = m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), i);
		if(strDBFlag == _T("D")) continue;
		strTmpRecKey = m_pDM_Appendix->GetCellData(_T("BB_책KEY"), i);
		if(strRecKey.IsEmpty() && !strTmpRecKey.IsEmpty()) strRecKey = strTmpRecKey;
		else if(!strRecKey.IsEmpty() && !strTmpRecKey.IsEmpty()) strRecKey += _T(", ") + strTmpRecKey;
		if(i == m_nSelectedRow && !bInsert)	continue;
		strTmpRegNo = m_pDM_Appendix->GetCellData(_T("BB_등록번호"), i);
		if(strTmpRegNo == strRegNo) bDuplicate = TRUE;
	}
	if(bDuplicate) return FALSE;
	if(strRecKey.IsEmpty())
		strQuery.Format(_T("SELECT MAX(REC_KEY) FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG='A' AND REG_NO='%s' AND MANAGE_CODE = UDF_MANAGE_CODE"), strRegNo);
//DEL 		strQuery.Format(_T("SELECT MAX(REC_KEY) FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG='A' AND REG_NO='%s'"), strRegNo);
	else
		strQuery.Format(_T("SELECT MAX(REC_KEY) FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG='A' AND REG_NO='%s' AND REC_KEY NOT IN (%s) AND MANAGE_CODE = UDF_MANAGE_CODE"), strRegNo, strRecKey);
//DEL 		strQuery.Format(_T("SELECT MAX(REC_KEY) FROM BO_BOOK_TBL WHERE BOOK_APPENDIX_FLAG='A' AND REG_NO='%s' AND REC_KEY NOT IN (%s)"), strRegNo, strRecKey);
	m_pDM_Appendix->GetOneValueQuery(strQuery, strResult);
	if(!strResult.IsEmpty()) return FALSE;
	return TRUE;
}

VOID CBOAppendixDlg::SetRegNoCtrl()
{
	CString strRegNo;
	m_pDM_Appendix->GetCellData(_T("BB_등록번호"), m_nSelectedRow, strRegNo);
	strRegNo.TrimLeft();		strRegNo.TrimRight();
	if(strRegNo.IsEmpty()) 
	{
		//===================================================
		//2009.10.22 ADD BY PJW : 부록입력이고 자동부여일경우는 컨트롤에 NULL을 셋팅한다
		m_pCM->SetControlMgrData(_T("BB_등록번호"), strRegNo);
		//===================================================
		return;
	}
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strRegNo = strRegNo.Mid(2);
	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	strRegNo = strRegNo.Mid(nRegCodeLength);
	///===================================================================
	m_pCM->SetControlMgrData(_T("BB_등록번호"), strRegNo);
}

BOOL CBOAppendixDlg::CheckRegNo(BOOL bInsert)
{
	if(!((CEdit*)GetDlgItem(IDC_edtAPPENDIX_REG_NO))->IsWindowVisible())
		return TRUE;
	if(m_bAutoRegYN) return TRUE;

	if(!CheckValidAppendixRegCode())
	{
		ESLAfxMessageBox(_T("부록에 대한 등록구분이 아닙니다."));
		m_pCM->SetControlMgrData(_T("BB_등록구분_코드"), m_pDM_Appendix->GetCellData(_T("BB_등록구분_코드"), m_nSelectedRow));
		return FALSE;
	}

	if(!((CEdit*)GetDlgItem(IDC_edtAPPENDIX_REG_NO))->GetStyle() && ES_READONLY)
	{
		m_pCM->SetControlMgrData(_T("BB_등록번호"), _T(""));
	}
	else
	{
		CString strTmpData;
		m_pCM->GetControlMgrData(_T("BB_등록번호"), strTmpData);
		// 2004.12.06 수동부여일 경우에도 등록구분이 적용안함이면 등록번호를 반드시 입력하지 않아도 되도록 수정.
		// if(strTmpData.IsEmpty())
		CString strRegCode;
		m_pCM->GetControlMgrData(_T("BB_등록구분_코드"), strRegCode);

		if(strTmpData.IsEmpty() && strRegCode != _T(""))
		{
			ESLAfxMessageBox(_T("부록등록번호 수동 부여일 경우 등록번호를 반드시 입력해 주셔야 합니다."));
			return FALSE;
		}
	}

	if(!CheckDuplicateAppendixRegNo(bInsert))
	{
		ESLAfxMessageBox(_T("중복되었거나 올바르지 않은 등록번호를 입력하였습니다."));
		SetRegNoCtrl();
		return FALSE;
	}
	return TRUE;
}

CString CBOAppendixDlg::SetRegNoDMField()
{
	CString strRegNo, strRegCode;
	m_pCM->GetControlMgrData(_T("BB_등록구분_코드"), strRegCode);
	m_pCM->GetControlMgrData(_T("BB_등록번호"), strRegNo);

	if(strRegCode.IsEmpty() || strRegNo.IsEmpty()) return _T("");

	// 2004.10.14 등록번호 큰숫자 오류 수정.
	/*INT nRegNo = _ttoi(strRegNo.GetBuffer(0));
	if(nRegNo < 0) return _T("");
	strRegNo.Format(_T("%s%010d"), strRegCode, nRegNo);*/
	__int64 nRegNo = _ttoi64(strRegNo.GetBuffer(0));
	if(nRegNo < 0) return _T("");
	//=====================================================
	//2008.11.04 UPDATE BY PJW : 등록구분과 등록번호의 숫자부분을 조합한다 -> 등록구분코드 변경가능하도록 수정
// 	strRegNo.Format(_T("%s%010I64d"), strRegCode, nRegNo);
	strRegNo = m_pInfo->MakeRegNo(strRegCode, strRegNo);
	//===================================================================
	
	return strRegNo;
}

BOOL CBOAppendixDlg::CheckIsDigit(CString strRegNo)
{
	if(strRegNo.IsEmpty()) return FALSE;

	TCHAR * tmp = strRegNo.GetBuffer(0);
	INT size = strRegNo.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}

INT CBOAppendixDlg::ApplySpeciesToAppendixData()
{
	if(m_pDM_Appendix == NULL) return 0;
	if(m_pDM_Appendix->GetRowCount() == 0) return 0;
	if(m_pDM_Species == NULL) return 0;
	if(m_pDM_Species->GetRowCount() == 0) return 0;
	if(m_pFormApi == NULL) return 0;
	INT nRowCnt = m_pDM_Appendix->GetRowCount();

	CStringArray IgnoreArray;
	IgnoreArray.Add(_T("BS_매체구분_코드"));
	IgnoreArray.Add(_T("BS_관리구분_코드"));
	IgnoreArray.Add(_T("BS_배가위치부호"));
	/********************************************************
	 ********************************************************/
	//20071217 ADD PJW : 부록의 이용제한구분 코드가 종정보를 따라가지 않도록 수정
	IgnoreArray.Add(_T("BS_이용제한구분_코드"));
	/********************************************************
	 ********************************************************/
	
	for(INT i=0 ; i<nRowCnt ; i++)
	{
		m_pFormApi->CopyDM(m_pDM_Species, 0, m_pDM_Appendix, i, _T("BB"), &IgnoreArray);
	}
	return 0;
}

VOID CBOAppendixDlg::SetAcqCodeCtrl()
{
	CString strAcqCode;
	INT ids;
	if(m_pDM_Book == NULL)
	{
		ids = m_pInfo->pCodeMgr->GetDefaultCode(_T("입수구분"), strAcqCode);
		if(ids < 0) return ;
	}
	else
	{
		m_pDM_Book->SORT(_T("BB_책KEY"));
		for(INT i=m_pDM_Book->GetRowCount()-1 ; i>=0 ; i--)
		{
			if(m_pDM_Book->GetCellData(_T("BB_DB_FLAG"), i) == _T("D")) continue;
			else 
			{
				strAcqCode = m_pDM_Book->GetCellData(_T("BB_수입구분_코드"), i);
				break;
			}
		}
	}
	m_pCM->SetControlMgrData(_T("BB_수입구분_코드"), strAcqCode);
}

VOID CBOAppendixDlg::OnbtnSETACCESSIONRECNO() 
{
	// TODO: Add your control notification handler code here
	m_pGrid_Appendix->SetProtectSelectColumn(TRUE);
			
	CAccessionNoSearchDlg dlg(this);
	dlg.SetBookDM(m_pDM_Book);
	if(IDOK == dlg.DoModal())
	{
		SetAccessionNoToAppendix(dlg.GetAccessionRecKey(), dlg.GetAccessionNo());
	}

	Display();
	m_pGrid_Appendix->Display();

	m_pGrid_Appendix->SetProtectSelectColumn(FALSE);
}

HBRUSH CBOAppendixDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

BOOL CBOAppendixDlg::ValidateData()
{
	CString strTmpData;
	m_pCM->GetControlMgrData(_T("BB_편제명부록명"), strTmpData);
	if(strTmpData.IsEmpty())
	{
		ESLAfxMessageBox(_T("부록명을 입력해 주십시요."));
		return FALSE;
	}
	return TRUE;
}

VOID CBOAppendixDlg::OnSetfocuscREGCLASS() 
{
	// TODO: Add your control notification handler code here
	ShowNoteCtrlToRegCode();
}

VOID CBOAppendixDlg::ShowNoteCtrlToRegCode()
{
	DeleteNoteCtrl();
	m_pNoteCtrl = new CNoteCtrl;
	CString strNote;
	strNote.Format(_T("등록구분이 [적용안함]일 경우 등록번호를 부여하지 않습니다."));
	m_pNoteCtrl->SetNote(strNote);
	
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	
	point.y += 5;
	 
	m_pNoteCtrl->Create(point, 200, 36, this);
	m_pNoteCtrl->ShowWindow(SW_SHOW);
}

UINT CBOAppendixDlg::OnNoteClicked(LPARAM lParam, WPARAM wParam)
{
	if(m_pNoteCtrl) m_pNoteCtrl->ShowWindow(SW_HIDE);
	return 0;
}

VOID CBOAppendixDlg::DeleteNoteCtrl()
{
	if(m_pNoteCtrl)
	{
		delete m_pNoteCtrl;
		m_pNoteCtrl = NULL;
	}
}

VOID CBOAppendixDlg::OnKillfocuscREGCLASS() 
{
	// TODO: Add your control notification handler code here
	if(m_pNoteCtrl->GetSafeHwnd()) m_pNoteCtrl->ShowWindow(SW_HIDE);
}

INT CBOAppendixDlg::SetAccessionNoToAppendix(CString strAccessionNoKey, CString strAccessionNo)
{
	/***********************************************************
	 /***********************************************************/
	/// 20080320 update by pdw
	/// 선정된 자료만 부여되도록 수정 하였다
	/// 2008.07.14 UPDATE BY PDJ : 선정된 자료만 수정안되서 다시 수정
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CMUP_단행_부록_부록정보"), _T("Grid_Appendix"));

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();
	
	INT nMode = 0;
	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt > 0 )
		nMode = 1;
	else
		nSelectCnt = m_pDM_Appendix->GetRowCount();

	CString strDBFlag;
	INT nIndex;
	pGrid->SelectGetHeadPosition();
	for(INT i=0 ; i<nSelectCnt ; i++)
	{
		if( nMode == 0 )
			nIndex = i;
		else
			nIndex = pGrid->SelectGetIdx();	
		// 2007.08.17 REM BY PDJ
		// 자동일때는 저장시 무조건 부여 받으므로 체크하면 안되고
		// 수동일 경우 없으면 수정이 안되므로 체크할 필요 없음.
		//if(!m_pDM_Appendix->GetCellData(_T("BB_등록번호"), i).IsEmpty())
		//{
		m_pDM_Appendix->SetCellData(_T("BB_원부KEY"), strAccessionNoKey, nIndex);
		m_pDM_Appendix->SetCellData(_T("UDF_원부번호"), strAccessionNo, nIndex);
		
		// 2007.08.02 ADD BY PDJ
		// 입력일떄는 'I' Flag가 세팅되어 있으므로 상관없으나
		// 수정일때 원부번호 설정은 아무 Flag세팅이 없으므로 
		m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), nIndex, strDBFlag );
		if ( strDBFlag == _T("") ) m_pDM_Appendix->SetCellData(_T("BB_DB_FLAG"), _T("U"), nIndex);
		//}

		if( nMode == 1)
			pGrid->SelectGetNext();
	}
	/***********************************************************
	 /***********************************************************/
	/*
	CString strDBFlag;
	INT nCnt = m_pDM_Appendix->GetRowCount();
	for(INT i=0 ; i<nCnt ; i++)
	{
		// 2007.08.17 REM BY PDJ
		// 자동일때는 저장시 무조건 부여 받으므로 체크하면 안되고
		// 수동일 경우 없으면 수정이 안되므로 체크할 필요 없음.
		//if(!m_pDM_Appendix->GetCellData(_T("BB_등록번호"), i).IsEmpty())
		//{
		m_pDM_Appendix->SetCellData(_T("BB_원부KEY"), strAccessionNoKey, i);
		m_pDM_Appendix->SetCellData(_T("UDF_원부번호"), strAccessionNo, i);
		
		// 2007.08.02 ADD BY PDJ
		// 입력일떄는 'I' Flag가 세팅되어 있으므로 상관없으나
		// 수정일때 원부번호 설정은 아무 Flag세팅이 없으므로 
		m_pDM_Appendix->GetCellData(_T("BB_DB_FLAG"), i, strDBFlag );
		if ( strDBFlag == _T("") ) m_pDM_Appendix->SetCellData(_T("BB_DB_FLAG"), _T("U"), i);
		//}
	}
	*/
	return 0;
}

// 2005.11.7 ADD BY PDJ
// 부록정보는 대부분 종정보와 비슷하게 입력될 수 있으므로
// 이를 이용할 수 있는 버튼 추가.
// 무조건은 안된다.
VOID CBOAppendixDlg::OnbSPECIESCOPY() 
{	
	if ( m_pDM_Index == NULL ) return;
	CString strTitle;
	CString strAuthor;
	CString strPubYear;
	//===================================================
	//2009.03.26 ADD BY PJW : 편권차정보도 가져온다.
	CString strVol;
	//===================================================
	
	INT		nOffset1;
	INT		nOffset2;
	INT		nCut;

	strTitle = m_pDM_Index->GetCellData(_T("IBS_본표제"), 0 );
	strAuthor = m_pDM_Index->GetCellData(_T("IBS_저작자"), 0 );
	strPubYear = m_pDM_Index->GetCellData(_T("IBS_발행년"), 0 );
	//===================================================
	//2009.03.26 ADD BY PJW : 편권차정보도 가져온다.
	strVol = m_pDM_Index->GetCellData(_T("IBS_편권차"), 0 );
	//===================================================
	
	// 표제 구두점 이하 절사 
	// 본표 항목에 구둣점이 붙는 경우는 : = 표제관련 구둣점.
	nOffset1 = strTitle.Find(_T("="), 0 );
	nOffset2 = strTitle.Find(_T(":"), 0 );
	if ( nOffset1 != -1 && nOffset1 > 0 ) nCut = nOffset1;
	else if ( nOffset2 != -1 && nOffset2 > 0 ) nCut = nOffset2;
	else nCut = strTitle.GetLength();

	if ( !strTitle.IsEmpty() )
	{
		( (CEdit*) GetDlgItem (IDC_eAPPENDIX_TITLE) )->SetWindowText(strTitle.Left(nCut));
	}

	if ( !strAuthor.IsEmpty() ) 
	{
		( (CEdit*) GetDlgItem (IDC_edtAUTHOR) )->SetWindowText(strAuthor);
	}

	if ( !strPubYear.IsEmpty() ) 
	{
		( (CEdit*) GetDlgItem (IDC_edtPUB_YEAR) )->SetWindowText(strPubYear);	
	}

	//===================================================
	//2009.03.26 ADD BY PJW : 편권차정보도 가져온다.
	if ( !strVol.IsEmpty() ) 
	{
		( (CEdit*) GetDlgItem (IDC_edtVOL) )->SetWindowText(strVol);	
	}
	//===================================================
}

//===================================================
//2009.05.28 ADD BY LKS : Control Trim

VOID CBOAppendixDlg::TrimControl()
{
	INT ControlID[8]={IDC_eAPPENDIX_TITLE,IDC_edtAUTHOR,IDC_edtPUB_YEAR,IDC_eVOL_CODE,
					   IDC_eCOPY_CODE,IDC_edtVOL,IDC_edtVOL_SORT_NO,IDC_edtBOOK_CODE};

	CString strData;
	CEdit* pEdit = NULL;
	for(INT i=0;i<8;i++)
	{
		pEdit = (CEdit*)GetDlgItem(ControlID[i]);
		if (pEdit == NULL) continue;
		pEdit->GetWindowText(strData);
		if( !strData.IsEmpty() )
		{
			strData.TrimLeft();		strData.TrimRight();
			pEdit->SetWindowText(strData);	
		}
	}
}
//===================================================
