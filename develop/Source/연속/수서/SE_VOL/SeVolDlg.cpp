// SeVolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeVolDlg.h"
#include "SeBookDlg.h"
#include "SeAppendixDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeVolDlg dialog


CSeVolDlg::CSeVolDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSeVolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeVolDlg)
	m_bBINDING_YN = FALSE;
	m_bREAD_FIRST_YN = FALSE;
	//}}AFX_DATA_INIT
	m_pCM = NULL;
	m_pDM = NULL;
	m_pParentGrid = NULL;
	m_nCurrentParentIdx = -1;
	m_nOpenMode = 1; //default
	m_nACQ_TYPE = 1; //default
	m_strUDF = _T("");
	m_bIsNonPublished = FALSE;
	
	m_strTITLE_INFO = _T(""); 
	m_strAUTHOR_INFO = _T("");
	m_strPUB_INFO = _T("");

	m_nMoveOption = 1; //default

	m_bIsLightVersion = FALSE;

	m_bKillfocusLack = FALSE;

	m_bIsBookCntIncreased = FALSE;
}

CSeVolDlg::~CSeVolDlg()
{
}

VOID CSeVolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeVolDlg)
	DDX_Check(pDX, IDC_chkVOL_BINDING_YN, m_bBINDING_YN);
	DDX_Check(pDX, IDC_chkVOL_READ_FIRST_YN, m_bREAD_FIRST_YN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeVolDlg, CDialog)
	//{{AFX_MSG_MAP(CSeVolDlg)
	ON_BN_CLICKED(IDC_btnVOL_PREV, OnbtnVOLPREV)
	ON_BN_CLICKED(IDC_btnVOL_NEXT, OnbtnVOLNEXT)
	ON_EN_KILLFOCUS(IDC_edtVOL_ACQ_BOOK_CNT, OnKillfocusedtVOLACQBOOKCNT)
	ON_EN_KILLFOCUS(IDC_edtVOL_MISS_BOOK_CNT, OnKillfocusedtVOLMISSBOOKCNT)
	ON_BN_CLICKED(IDC_btnVOL_BOOK_INFO, OnbtnVOLBOOKINFO)
	ON_BN_CLICKED(IDC_btnVOL_APPENDIX_INFO, OnbtnVOLAPPENDIXINFO)
	ON_CBN_SELCHANGE(IDC_cmbVOL_VOLUME_CLASS, OnSelchangecmbVOLVOLUMECLASS)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeVolDlg message handlers

BOOL CSeVolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( InitESL_Mgr(_T("연속_수서_권")) < 0 ) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_권") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_연속_권"));
	m_pDM = FindDM(_T("DM_체크인_작업"));
	
	if(!m_pCM || !m_pDM || !m_pParentGrid) {
		AfxMessageBox( _T("CM,DM,Gird Error : 연속_권,연속_권_작업") );
		return FALSE;
	}
	
	InitControls();

	if(InitOpenMode()<0)
	{
		EndDialog(0);
		return FALSE;
	}
	
	//라이트 버젼 설정
	if(m_bIsLightVersion)
	{
		SetLightVersion();
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSeVolDlg::SetLightVersion()
{
	//라이트 버젼의 경우 열람우선여부와 제본여부 콘트롤을 없앤다 
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_chkVOL_READ_FIRST_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_chkVOL_BINDING_YN);
	if(pWnd) pWnd->ShowWindow(FALSE);

}

INT CSeVolDlg::InitControls()
{
	//가격 성격
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	
	if(pCombo)
	{
		pCombo->InsertString( 0, _T("정가") );
		pCombo->InsertString( 1, _T("비매품") );
		pCombo->InsertString( 2, _T("가격불명") );
		pCombo->SetCurSel( 0 );
	}

	//권호 구분
	//1: 일반 2: 합병호 3: 미발행
	pCombo = NULL;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );

	if(pCombo)
	{
		pCombo->InsertString( 0, _T("일반") );
		pCombo->InsertString( 1, _T("합병호") );
		pCombo->InsertString( 2, _T("미발행") );
		pCombo->SetCurSel( 0 );
	}
	
	return 0;
}

INT CSeVolDlg::InitOpenMode()
{

	if(m_nOpenMode>0) //입력 
	{
		m_nCurrentParentIdx = m_pDM->GetRowCount()-1;
		
		m_strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentParentIdx);
		HideOrShowControl();
		ShowAllData();
	}
	else if(-1 == m_nOpenMode)  //일반 수정 
	{
		
		if(m_nCurrentParentIdx<0) {
			AfxMessageBox(_T("선정된 정보가 없습니다!"));
			return -1;
		}
		
		m_strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentParentIdx);
		HideOrShowControl();
		ShowAllData();
	}
	else if(-2 == m_nOpenMode) //합병호
	{
		m_pParentGrid->SelectMakeList();
		m_nCurrentParentIdx = m_pParentGrid->SelectGetHeadPosition();
		m_strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentParentIdx);

		HideOrShowControl();
		
		//일단 마지막 자료를 기준으로 모든 정보를 보여준후 
		ShowAllData();
		
		//권호구분을 합병호로 보여준다
		CComboBox * pCombo;
		pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
		pCombo->SetCurSel(1);
		pCombo->EnableWindow(FALSE);

		//권호, 부차적 권호를 통합해서 보여준다.
		CString strVOL_TITLE = _T("");
		CString strOTHER_VOL_TITLE = _T("");
		CString tmp;
		INT idx = m_pParentGrid->SelectGetTailPosition();
		
		while(idx>=0)
		{
			tmp = _T("");
			tmp = m_pDM->GetCellData(_T("SV_권호명"),idx);
			
			if(!tmp.IsEmpty()) {
				
				if(strVOL_TITLE.IsEmpty()) 
				{
					strVOL_TITLE += tmp;
				}
				else
				{
					strVOL_TITLE += _T("-");
					strVOL_TITLE += tmp;
				}
            }
			
			tmp = _T("");
			tmp = m_pDM->GetCellData(_T("SV_부차적권호명"),idx);
			
			if(!tmp.IsEmpty()) {
				
				if(strOTHER_VOL_TITLE.IsEmpty()) 
				{
					strOTHER_VOL_TITLE += tmp;
				}
				else
				{
					strOTHER_VOL_TITLE += _T("-");
					strOTHER_VOL_TITLE += tmp;
				}
            }
	
			idx = m_pParentGrid->SelectGetPrev();
		}
	

		m_pCM->SetControlMgrData(_T("SV_권호명"),strVOL_TITLE,m_nCurrentParentIdx);
		m_pCM->SetControlMgrData(_T("SV_부차적권호명"),strOTHER_VOL_TITLE,m_nCurrentParentIdx);
	}
	


	return 0;
}

INT CSeVolDlg::HideOrShowControl()
{
	if(m_nOpenMode>0) //입력
	{
		GetDlgItem(IDC_btnVOL_PREV)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_NEXT)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
		
	}
	else if(-1 == m_nOpenMode) //일반 수정 
	{
		CEdit* pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_EXP_BOOK_CNT);
		if(pEdit) pEdit->SetReadOnly();

		pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT);
		if(pEdit) pEdit->SetReadOnly(FALSE);
			
		pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT);
		if(pEdit) pEdit->SetReadOnly(FALSE);

		
		CString strVOLUME_CLASS = _T("");
		strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_권호구분"),m_nCurrentParentIdx);	
		if(_T("3") ==strVOLUME_CLASS) //미발행
		{
			//미발행 경우에는 책, 부록  버튼을 감춘후 입수, 결호 책수란도 read-only화 한다
			GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
			GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
			
			pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT);
			if(pEdit) pEdit->SetReadOnly();
			
			pEdit = (CEdit*)GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT);
			if(pEdit) pEdit->SetReadOnly();
			
			return 0;
		}

		
		if(_T("E") == m_strUDF || _T("C") == m_strUDF || _T("M") == m_strUDF)
		{
			//예측 수정의 경우에는 책, 부록  버튼을 감춘다 
			GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
			GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
			
		}
		else 
		{
			CString strCHECKIN_STATUS;
			strCHECKIN_STATUS = m_pDM->GetCellData(_T("SA_입수상태"),m_nCurrentParentIdx);	
			if(_T("2") == strCHECKIN_STATUS) //결호자료 
			{
				GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
				GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
		
			}
			else //입수 또는 미달자료  
			{
				GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(TRUE);
				GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(TRUE);
			}
		}
	}
	else if(-2 == m_nOpenMode) //합병호 
	{
		GetDlgItem(IDC_btnVOL_PREV)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_NEXT)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_APPENDIX_INFO)->ShowWindow(FALSE);
		GetDlgItem(IDC_btnVOL_BOOK_INFO)->ShowWindow(FALSE);
	}


	return 0;
}

INT CSeVolDlg::ShowAllData()
{
	//권호구분 
	CString strVOLUME_CLASS;
	strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_권호구분"),m_nCurrentParentIdx);
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	BOOL bBookCntSetReadOnly = FALSE;
	
	if(_T("1") == strVOLUME_CLASS) 	
	{
		pCombo->SetCurSel( 0 );
		pCombo->EnableWindow(TRUE);
	}
	else if(_T("2") == strVOLUME_CLASS)
	{
		pCombo->SetCurSel( 1 );
		pCombo->EnableWindow(TRUE);
	}
	else if(_T("3") == strVOLUME_CLASS) //미발행 자료
	{
		pCombo->SetCurSel( 2 );
		pCombo->EnableWindow(FALSE);
		bBookCntSetReadOnly = TRUE;
	}
	
	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT);

	if(bBookCntSetReadOnly) //미발행자료는 입수책수와 결호책수를 read only한다
	{
		pEdit1->SetReadOnly(TRUE);
		pEdit2->SetReadOnly(TRUE);
	}
	else
	{
		pEdit1->SetReadOnly(FALSE);
		pEdit2->SetReadOnly(FALSE);
	}


	//가격 성격
	CString strPRICE_CHARACTER;
	strPRICE_CHARACTER = m_pDM->GetCellData(_T("SV_가격성격"),m_nCurrentParentIdx);
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	if(_T("비매품") == strPRICE_CHARACTER) 	pCombo->SetCurSel( 1 );
	else if(_T("가격불명") == strPRICE_CHARACTER) 	pCombo->SetCurSel( 2 );
	else pCombo->SetCurSel( 0 );
	
	
	//열람우선여부
	CString strREAD_FIRST_YN;
	strREAD_FIRST_YN = m_pDM->GetCellData(_T("UDF_열람우선여부"),m_nCurrentParentIdx);
	
	if(_T("Y") == strREAD_FIRST_YN)
	{
		m_bREAD_FIRST_YN = TRUE;
	}
	else if(_T("N") == strREAD_FIRST_YN)
	{
		m_bREAD_FIRST_YN = FALSE;
	}
	
	//제본여부
	CString strBINDING_YN;
	strBINDING_YN = m_pDM->GetCellData(_T("SS_제본여부"),m_nCurrentParentIdx);
	
	if(_T("Y") == strBINDING_YN)
	{
		m_bBINDING_YN = TRUE;
	}
	else if(_T("N") == strBINDING_YN)
	{
		m_bBINDING_YN = FALSE;
	}
	

	m_pCM->AllControlDisplay(m_nCurrentParentIdx);
	
	if(m_nOpenMode<0) {
		SetGridRowBoldFont(m_pParentGrid, m_nCurrentParentIdx, TRUE);
	}
		
	UpdateData(FALSE);

	return 0;
}

INT CSeVolDlg::SaveAllDataToDM()
{
	const INT cnt  = 19;
	CString alias[cnt] = {
			_T("SV_가격"),
			_T("SA_결호책수"),
			_T("SV_권호명"),
			_T("SV_권호별서명"),
			_T("SV_권호일련번호"),
			_T("SV_딸림자료"),
			_T("SV_매체구분_코드"),
			_T("SV_면장수"),
			_T("SV_물리적특성"),
			_T("SV_발행일"),
			_T("SV_권호표제"),
			_T("SV_부차적권호명"),
			_T("SV_비고"),
			_T("UDF_입수일"),
			_T("SA_입수책수"),
			_T("SV_책크기"),
			_T("SV_형식구분_코드"),
			_T("SV_화폐구분"),
			_T("SA_입수예정일")};

	CString tmp;
	for(INT i=0;i<cnt;i++)
	{
		tmp = _T("");	
		m_pCM->GetControlMgrData(alias[i],tmp);
		m_pDM->SetCellData(alias[i],tmp,m_nCurrentParentIdx);
	}
	
	//가격 성격 
	CString strPRICE_CHARACTER = _T("");
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_PRICE_CHARACTER );
	if(pCombo) pCombo->GetWindowText(strPRICE_CHARACTER);
	m_pDM->SetCellData(_T("SV_가격성격"),strPRICE_CHARACTER,m_nCurrentParentIdx);
	

	//권호구분
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	if(pCombo) {
		
		INT nCurSel = pCombo->GetCurSel();
		
		if(0 == nCurSel) //일반
		{
			m_pDM->SetCellData(_T("SV_권호구분"),_T("1"),m_nCurrentParentIdx);
			m_pDM->SetCellData(_T("UDF_권호구분"),_T("일반"),m_nCurrentParentIdx);
		}
		else if(1 == nCurSel) //합병호
		{
	
			m_pDM->SetCellData(_T("SV_권호구분"),_T("2"),m_nCurrentParentIdx);
			m_pDM->SetCellData(_T("UDF_권호구분"),_T("합병호"),m_nCurrentParentIdx);
		}
	}
	
	if(m_bIsNonPublished) //신규로미발행 처리 된 경우 
	{
		m_pDM->SetCellData(_T("UDF_입수상태"),_T("입수"),m_nCurrentParentIdx);
		m_pDM->SetCellData(_T("UDF_권호구분"),_T("미발행"),m_nCurrentParentIdx);
		m_pDM->SetCellData(_T("SV_권호구분"),_T("3"),m_nCurrentParentIdx); //미발행
		m_pDM->SetCellData(_T("SA_입수책수"),_T("0"),m_nCurrentParentIdx);
		m_pDM->SetCellData(_T("SA_결호책수"),_T("0"),m_nCurrentParentIdx);

		m_pCM->ControlDisplay(_T("SA_입수책수"),m_nCurrentParentIdx);
		m_pCM->ControlDisplay(_T("SA_결호책수"),m_nCurrentParentIdx);

		m_bIsNonPublished = FALSE;
	}


	//UDF_상태 처리
	CString strACQ_BOOK_CNT;
	CString strMISS_BOOK_CNT;
	m_pCM->GetControlMgrData(_T("SA_입수책수"),strACQ_BOOK_CNT);
	m_pCM->GetControlMgrData(_T("SA_결호책수"),strMISS_BOOK_CNT);
	INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
	INT nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));
	


	if(_T("D") == m_strUDF || _T("U") == m_strUDF) //기존자료
	{
		if(0 == nACQ_BOOK_CNT)
		{
			if(0 == nMISS_BOOK_CNT)
			{
				
				CString strVOLUME_CLASS = _T("");
				strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_권호구분"),m_nCurrentParentIdx);
				if(_T("3") != strVOLUME_CLASS) //미발행
				{
					AfxMessageBox(_T("예정 권호가 아닌 경우에는  입수 책수와 결호 책수가 \n 동시에 0 이 될 수 없습니다!"));
					GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT)->SetFocus();
					return -1;	
				}
			}
			else //결호자료 
			{
				m_pDM->SetCellData(_T("UDF_입수상태"),_T("결호"),m_nCurrentParentIdx);
			}

		}
		else 
		{
			if(0 == nMISS_BOOK_CNT) //입수자료 
				m_pDM->SetCellData(_T("UDF_입수상태"),_T("입수"),m_nCurrentParentIdx);
			else //미달자료 
				m_pDM->SetCellData(_T("UDF_입수상태"),_T("미달"),m_nCurrentParentIdx);

		}

		m_pDM->SetCellData(_T("UDF_상태"), _T("U"),m_nCurrentParentIdx);

	}
	else if(_T("E") == m_strUDF || _T("C") == m_strUDF || _T("M") == m_strUDF) //예정자료  
	{
	
		if(0 == nACQ_BOOK_CNT)
		{
			if(0 == nMISS_BOOK_CNT)
			{
				m_pDM->SetCellData(_T("UDF_입수상태"),_T("예정"),m_nCurrentParentIdx);
				m_pDM->SetCellData(_T("UDF_상태"), _T("E"),m_nCurrentParentIdx);
	
			}
			else //결호자료 
			{
				m_pDM->SetCellData(_T("UDF_입수상태"),_T("결호(예정)"),m_nCurrentParentIdx);
				m_pDM->SetCellData(_T("UDF_상태"), _T("M"),m_nCurrentParentIdx);
			}
		}
		else 
		{
			if(0 == nMISS_BOOK_CNT) //입수자료 
			{
				m_pDM->SetCellData(_T("UDF_입수상태"),_T("입수(예정)"),m_nCurrentParentIdx);
				m_pDM->SetCellData(_T("UDF_상태"), _T("C"),m_nCurrentParentIdx);
			}
			else //미달자료 
			{
				m_pDM->SetCellData(_T("UDF_입수상태"),_T("미달(예정)"),m_nCurrentParentIdx);
				m_pDM->SetCellData(_T("UDF_상태"), _T("C"),m_nCurrentParentIdx);
			}

		}	

	}
	
	m_pCM->ControlDisplay(_T("UDF_입수상태"),m_nCurrentParentIdx);
	m_pParentGrid->DisplayLine(m_nCurrentParentIdx);
	if(-2 != m_nOpenMode) AfxMessageBox(_T("적용하였습니다!")); //합병호 처리는 메시지를 보내지 않는다

	return 0;
}

VOID CSeVolDlg::OnbtnVOLPREV() 
{
	// TODO: Add your control notification handler code here
	INT m_nBeforeIdx = m_nCurrentParentIdx;

	if(1 == m_nMoveOption)
	{
		m_nCurrentParentIdx--;

		if(m_nCurrentParentIdx < 0)
		{
			AfxMessageBox(_T("처음 항목입니다!"));
			m_nCurrentParentIdx++;
			return;
		}
		
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetPrev();

		if(nIdx<0) 
		{
			INT nHeadIdx = m_pParentGrid->SelectGetHeadPosition();
			
			if(m_nCurrentParentIdx == nHeadIdx)
			{
				AfxMessageBox(_T("처음 항목입니다!"));
				m_pParentGrid->SelectGetHeadPosition();
				return;
			}
			else //이전 선정 정보가 없을 경우 맨 앞의 선정정보를 택한다 
			{
				m_nCurrentParentIdx = nHeadIdx;
			}
		
		}
		else //선정 정보를 기준으로 움직인다
		{
			m_nCurrentParentIdx = nIdx;
		}

	}

	if(m_nBeforeIdx != m_nCurrentParentIdx) //이전 인덱스의 글씨 두께를 원래대로 한다 
	{
		if(m_nBeforeIdx>=0) SetGridRowBoldFont(m_pParentGrid, m_nBeforeIdx, FALSE);
	}
	
	m_strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentParentIdx);
	HideOrShowControl();
	ShowAllData();

}

VOID CSeVolDlg::OnbtnVOLNEXT() 
{
	// TODO: Add your control notification handler code here
	INT m_nBeforeIdx = m_nCurrentParentIdx;

	if(1 == m_nMoveOption)
	{
		m_nCurrentParentIdx++;
		INT nMaxIdx = m_pDM->GetRowCount()-1;
		if(m_nCurrentParentIdx > nMaxIdx)
		{
			AfxMessageBox(_T("마지막 항목입니다!"));
			m_nCurrentParentIdx--;
			return;
		}
	}
	else if(2 == m_nMoveOption)
	{
		INT nIdx = m_pParentGrid->SelectGetNext();

		if(nIdx<0) 
		{
			INT nTailIdx = m_pParentGrid->SelectGetTailPosition();
			
			if(m_nCurrentParentIdx == nTailIdx)
			{
				AfxMessageBox(_T("마지막 항목입니다!"));
				m_pParentGrid->SelectGetTailPosition();
				return;
			}
			else //이전 선정 정보가 없을 경우 맨뒤의 선정정보를 택한다 
			{
				m_nCurrentParentIdx = nTailIdx;
			}
		}
		else //선정 정보를 기준으로 움직인다
		{
			m_nCurrentParentIdx = nIdx;
		}
	}

	if(m_nBeforeIdx != m_nCurrentParentIdx) //이전 인덱스의 글씨 두께를 원래대로 한다 
	{
		if(m_nBeforeIdx>=0) SetGridRowBoldFont(m_pParentGrid, m_nBeforeIdx, FALSE);
	}
	
	m_strUDF = m_pDM->GetCellData(_T("UDF_상태"),m_nCurrentParentIdx);
	HideOrShowControl();
	ShowAllData();

}

VOID CSeVolDlg::OnOK() 
{
	if( VerifyAllData() >= 0 )	SaveAllDataToDM();
	
	// 2005-10-11 황정영 : 아래코드 제거
	//	if(-1 == m_nOpenMode) return; //일반 수정 

	CDialog::OnOK();
}

INT CSeVolDlg::VerifyAllData()
{
	
	//입수 책수가 증가했는지를 확인해 접수번호를 필요 유무를 확인한다. 
	CString strNewAcqBookCnt;
	CString strOldAcqBookCnt;
	INT nNewAcqBookCnt = 0;
	INT nOldAcqBookCnt = 0;

	m_pCM->GetControlMgrData(_T("SA_입수책수"),strNewAcqBookCnt);
	strOldAcqBookCnt = m_pDM->GetCellData(_T("SA_입수책수"),m_nCurrentParentIdx);

	nNewAcqBookCnt = _ttoi(strNewAcqBookCnt.GetBuffer(0));
	nOldAcqBookCnt = _ttoi(strOldAcqBookCnt.GetBuffer(0));

	if(nNewAcqBookCnt>nOldAcqBookCnt) m_bIsBookCntIncreased = TRUE;
	
	return 0;
}

VOID CSeVolDlg::OnKillfocusedtVOLACQBOOKCNT() 
{
	// TODO: Add your control notification handler code here
	// 입수 책수가 0 이 되면 이 권은 결호 자료가 된다 
	// 결호 책수 정보를 가져와 둘다 0 이 아니면 이 자료는 책수 미달이 된다. 
	// 미발행일 경우는 이 조건에서 배제된다
	
	
	CString strVOLUME_CLASS;
	strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_권호구분"),m_nCurrentParentIdx);

	if(_T("3") != strVOLUME_CLASS) //미발행이 아닐 경우
	{

		CString strACQ_BOOK_CNT;
		CString strMISS_BOOK_CNT;
		
		m_pCM->GetControlMgrData(_T("SA_입수책수"),strACQ_BOOK_CNT);
		m_pCM->GetControlMgrData(_T("SA_결호책수"),strMISS_BOOK_CNT);
		
		if(strACQ_BOOK_CNT.IsEmpty())
		{
			AfxMessageBox(_T("입수책수란은 빈칸이 될수 없습니다!"));
			GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT)->SetFocus();
			return;
		}

		INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
		INT nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));
		

		if(_T("D") == m_strUDF || _T("U") == m_strUDF) //기존자료
		{
			if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT)
			{
				if(!m_bKillfocusLack)
				{
					AfxMessageBox(_T("예정 권호가 아닌 경우에는  입수 책수와 결호 책수가 \n 동시에 0 이 될 수 없습니다!"));
					GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT)->SetFocus();
					m_bKillfocusLack = TRUE;
					return;	
				}

				m_bKillfocusLack = FALSE;
			
			}
		}
	}

}

VOID CSeVolDlg::OnKillfocusedtVOLMISSBOOKCNT() 
{
	// TODO: Add your control notification handler code here
	// 결호 자료가 0 이 되면 이 권은 입수자료가 된다.
	// 결호 책수 정보를 가져와 둘다 0 이 아니면 이 자료는 책수 미달이 된다. 
	// 미발행일 경우는 이 조건에서 배제된다
	
	
	CString strVOLUME_CLASS;
	strVOLUME_CLASS = m_pDM->GetCellData(_T("SV_권호구분"),m_nCurrentParentIdx);

	if(_T("3") != strVOLUME_CLASS) //미발행이 아닐 경우
	{
		CString strACQ_BOOK_CNT;
		CString strMISS_BOOK_CNT;
		
		m_pCM->GetControlMgrData(_T("SA_입수책수"),strACQ_BOOK_CNT);
		m_pCM->GetControlMgrData(_T("SA_결호책수"),strMISS_BOOK_CNT);
		
		if(strMISS_BOOK_CNT.IsEmpty())
		{
			AfxMessageBox(_T("결호책수란은 빈칸이 될수 없습니다!"));
			GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT)->SetFocus();
			return;
		}

		INT nACQ_BOOK_CNT = _ttoi(strACQ_BOOK_CNT.GetBuffer(0));
		INT nMISS_BOOK_CNT = _ttoi(strMISS_BOOK_CNT.GetBuffer(0));
		

		if(_T("D") == m_strUDF || _T("U") == m_strUDF) //기존자료
		{
			if(0 == nACQ_BOOK_CNT && 0 == nMISS_BOOK_CNT)
			{
				if(!m_bKillfocusLack)
				{

					AfxMessageBox(_T("예정 권호가 아닌 경우에는  입수 책수와 결호 책수가 \n 동시에 0 이 될 수 없습니다!"));
					GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT)->SetFocus();
					m_bKillfocusLack = TRUE;
					return;		
				}

				m_bKillfocusLack = FALSE;
			}
		}
	}
}

VOID CSeVolDlg::OnbtnVOLBOOKINFO() 
{
	// TODO: Add your control notification handler code here
	
	CSeBookDlg dlg(this);
	dlg.m_strVOL_TITLE = m_pDM->GetCellData(_T("SV_권호명"),m_nCurrentParentIdx);
	dlg.m_strACQ_VOL_KEY = m_pDM->GetCellData(_T("SA_수입구분별권KEY"),m_nCurrentParentIdx);
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_strTITLE_INFO = m_strTITLE_INFO;
	dlg.m_strAUTHOR_INFO = m_strAUTHOR_INFO;
	dlg.m_strPUB_INFO = m_strPUB_INFO;
	dlg.m_bIsLightVersion = m_bIsLightVersion;
	
	INT ids = dlg.DoModal();

	//권 삭제 operation 수행시 창을 닫는다
	if(11 == ids)
	{
		EndDialog(11);
	}

}

VOID CSeVolDlg::OnbtnVOLAPPENDIXINFO() 
{
	// TODO: Add your control notification handler code here
	CSeAppendixDlg dlg(this);
	dlg.strVOL_TITLE = m_pDM->GetCellData(_T("SV_권호명"),m_nCurrentParentIdx);
	dlg.strACQ_VOL_KEY = m_pDM->GetCellData(_T("SA_수입구분별권KEY"),m_nCurrentParentIdx);
	dlg.m_nACQ_TYPE = m_nACQ_TYPE;
	dlg.m_nCurrentParentIdx = m_nCurrentParentIdx;
	dlg.DoModal();
	
	//부록 갯수 update(WORK DM, DB DM);
	CString strArticleCnt;
	strArticleCnt = dlg.m_strNewArticleCnt;
	m_pDM->SetCellData(_T("UDF_부록수"),strArticleCnt,m_nCurrentParentIdx);

	CString strACQ_VOL_KEY_WORK;
	CString strACQ_VOL_KEY_DB;
	strACQ_VOL_KEY_WORK = m_pDM->GetCellData(_T("SA_수입구분별권KEY"),m_nCurrentParentIdx);
	
	CESL_DataMgr* pDM = NULL;
	pDM = FindDM(_T("DM_체크인_DB"));
	if(!pDM) return;
	

	//일단은 검색 방식으로 
	for(INT i=0;i<pDM->GetRowCount();i++)
	{
		strACQ_VOL_KEY_DB = _T("");
		strACQ_VOL_KEY_DB = pDM->GetCellData(_T("SA_수입구분별권KEY"),i);
					
		if(strACQ_VOL_KEY_WORK == strACQ_VOL_KEY_DB) {
			pDM->SetCellData(_T("UDF_부록수"),strArticleCnt,i);
		}

	}
	
	//부모 그리드 refresh
	m_pParentGrid->DisplayLine(m_nCurrentParentIdx);

}

VOID CSeVolDlg::SetGridRowBoldFont(CESL_Grid* pGrid, INT nRowIdx, BOOL bNewValue)
{
	INT nCol = pGrid->GetCol();
	INT nCols = pGrid->GetCols(0);
	
	if (nRowIdx >= 0)
	{
		pGrid->SetRow(nRowIdx + 1);
		
		for (INT i = 1 ; i < nCols; i++)
		{
			pGrid->SetCol(i);
			pGrid->SetCellFontBold(bNewValue);
		}
	}
	pGrid->SetCol(nCol);
}

VOID CSeVolDlg::OnSelchangecmbVOLVOLUMECLASS() 
{
	// TODO: Add your control notification handler code here
	// 권호구분 sel change
	
	//일반 수정 모드에서만 적용
	if(-1 != m_nOpenMode) return;
	
	CComboBox * pCombo;
	pCombo = ( CComboBox * )GetDlgItem( IDC_cmbVOL_VOLUME_CLASS );
	if(!pCombo) return;
	
	INT nCurSel = pCombo->GetCurSel();
	CString strCurClass;
	CString strNewClass;
	strCurClass = m_pDM->GetCellData(_T("SV_권호구분"),m_nCurrentParentIdx);
	

	CEdit* pEdit1 = (CEdit*)GetDlgItem(IDC_edtVOL_ACQ_BOOK_CNT);
	CEdit* pEdit2 = (CEdit*)GetDlgItem(IDC_edtVOL_MISS_BOOK_CNT);


	if(0 == nCurSel || 1 == nCurSel) //일반 , 합병호
	{
		pEdit1->SetReadOnly(FALSE);
		pEdit2->SetReadOnly(FALSE);

	}
	if(2 == nCurSel) //미발행 (처리)
	{
	
		CString msg;
		msg.Format(_T("현재의 권호를 미발행 처리 하시겠습니까?")); 
		
		if(AfxMessageBox(msg,MB_YESNO) == IDYES)
		{
			pCombo->SetCurSel(2);
			m_bIsNonPublished = TRUE;
			
			pEdit1->SetReadOnly(TRUE);
			pEdit2->SetReadOnly(TRUE);

		}
		else
		{
			if(_T("1") == strCurClass) pCombo->SetCurSel(0);
			else if(_T("2") == strCurClass) pCombo->SetCurSel(1);
			
			pEdit1->SetReadOnly(FALSE);
			pEdit2->SetReadOnly(FALSE);
		}

	}


}

HBRUSH CSeVolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
