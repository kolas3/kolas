// DUP_KEY_SETTING.cpp : implementation file
//

#include "stdafx.h"
#include "DUP_KEY_SETTING.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDUP_KEY_SETTING dialog


CDUP_KEY_SETTING::CDUP_KEY_SETTING(CESL_Mgr* pParent, CString filename /*=NULL*/)
	: CESL_Mgr(CDUP_KEY_SETTING::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDUP_KEY_SETTING)
	m_chkAuthor = FALSE;
	m_chkISBN = FALSE;
	m_chkMedClass = FALSE;
	m_chkPubYear = FALSE;
	m_chkPublisher = FALSE;
	m_chkRegCode = FALSE;
	m_chkShelfCode = FALSE;
	m_chkTitle = FALSE;
	m_chkTwice = FALSE;
	m_chkVol = FALSE;
	m_radAuthor = -1;
	m_radISBN = -1;
	m_radMedCode = -1;
	m_radPubYear = -1;
	m_radPublisher = -1;
	m_radRegCode = -1;
	m_radShelfCode = -1;
	m_radTitle = -1;
	m_radVol = -1;
	m_radVolTitle = -1;
	m_chkVolTitle = FALSE;
	m_bIncludeDiscardedBook = FALSE;
	m_chkSeriesTitle = FALSE;
	//}}AFX_DATA_INIT
	m_SearchKey.SetSearchKeys(filename);
	m_FileName = filename;
	m_bInit = FALSE;
}

CDUP_KEY_SETTING::~CDUP_KEY_SETTING()
{
	
}


VOID CDUP_KEY_SETTING::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDUP_KEY_SETTING)
	DDX_Check(pDX, IDC_chkAUTHOR, m_chkAuthor);
	DDX_Check(pDX, IDC_chkISBN, m_chkISBN);
	DDX_Check(pDX, IDC_chkMED_CLASS, m_chkMedClass);
	DDX_Check(pDX, IDC_chkPUBLICATE_YEAR, m_chkPubYear);
	DDX_Check(pDX, IDC_chkPUBLISHER, m_chkPublisher);
	DDX_Check(pDX, IDC_chkREG_CODE, m_chkRegCode);
	DDX_Check(pDX, IDC_chkSEPARATE_SHELF_CODE, m_chkShelfCode);
	DDX_Check(pDX, IDC_chkTITLE, m_chkTitle);
	DDX_Check(pDX, IDC_chkTITLE_TWICE_SEARCH, m_chkTwice);
	DDX_Check(pDX, IDC_chkVOL, m_chkVol);
	DDX_Radio(pDX, IDC_radAUTHOR_RIGHTTRUNCATION, m_radAuthor);
	DDX_Radio(pDX, IDC_radISBN_EQUIVALENCE, m_radISBN);
	DDX_Radio(pDX, IDC_radMEDIACODE_EQUIVALENCE, m_radMedCode);
	DDX_Radio(pDX, IDC_radPUBLICATE_YEAR_EQUIVALENCE, m_radPubYear);
	DDX_Radio(pDX, IDC_radPUBLISHER_RIGHTTRUNCATION, m_radPublisher);
	DDX_Radio(pDX, IDC_radREGCODE_EQUIVALENCE, m_radRegCode);
	DDX_Radio(pDX, IDC_radSEPARATECODE_EQUIVALENCE, m_radShelfCode);
	DDX_Radio(pDX, IDC_radTITLE_RIGHTTRUNCATION, m_radTitle);
	DDX_Radio(pDX, IDC_radVOL_EQUIVALENCE, m_radVol);
	DDX_Radio(pDX, IDC_radVOL_TITLE_RIGHTTRUNCATION, m_radVolTitle);
	DDX_Check(pDX, IDC_chkVOL_TITLE, m_chkVolTitle);
	DDX_Check(pDX, IDC_chkDiscardBook, m_bIncludeDiscardedBook);
	DDX_Check(pDX, IDC_chkSeriesTitle, m_chkSeriesTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDUP_KEY_SETTING, CDialog)
	//{{AFX_MSG_MAP(CDUP_KEY_SETTING)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_chkAUTHOR, OnchkAUTHOR)
	ON_BN_CLICKED(IDC_chkPUBLISHER, OnchkPUBLISHER)
	ON_BN_CLICKED(IDC_chkPUBLICATE_YEAR, OnchkPUBLICATEYEAR)
	ON_BN_CLICKED(IDC_chkTITLE, OnchkTITLE)
	ON_BN_CLICKED(IDC_chkISBN, OnchkISBN)
	ON_BN_CLICKED(IDC_chkMED_CLASS, OnchkMEDCLASS)
	ON_BN_CLICKED(IDC_chkVOL, OnchkVOL)
	ON_BN_CLICKED(IDC_chkSEPARATE_SHELF_CODE, OnchkSEPARATESHELFCODE)
	ON_BN_CLICKED(IDC_chkREG_CODE, OnchkREGCODE)
	ON_BN_CLICKED(IDC_radALLWORDSEARCH, OnradALLWORDSEARCH)
	ON_BN_CLICKED(IDC_radTITLE_CUTSEARCH, OnradTITLECUTSEARCH)
	ON_BN_CLICKED(IDC_radTITLE_RIGHTTRUNCATION, OnradTITLERIGHTTRUNCATION)
	ON_BN_CLICKED(IDC_radTITLE_EQUIVALENCE, OnradTITLEEQUIVALENCE)
	ON_BN_CLICKED(IDC_radAUTHOR_RIGHTTRUNCATION, OnradAUTHORRIGHTTRUNCATION)
	ON_BN_CLICKED(IDC_radAUTHOR_EQUIVALENCE, OnradAUTHOREQUIVALENCE)
	ON_BN_CLICKED(IDC_radPUBLISHER_RIGHTTRUNCATION, OnradPUBLISHERRIGHTTRUNCATION)
	ON_BN_CLICKED(IDC_radPUBLISHER_EQUIVALENCE, OnradPUBLISHEREQUIVALENCE)
	ON_BN_CLICKED(IDC_radPUBLICATE_YEAR_EQUIVALENCE, OnradPUBLICATEYEAREQUIVALENCE)
	ON_BN_CLICKED(IDC_radISBN_EQUIVALENCE, OnradISBNEQUIVALENCE)
	ON_BN_CLICKED(IDC_radMEDIACODE_EQUIVALENCE, OnradMEDIACODEEQUIVALENCE)
	ON_BN_CLICKED(IDC_radVOL_EQUIVALENCE, OnradVOLEQUIVALENCE)
	ON_BN_CLICKED(IDC_radSEPARATECODE_EQUIVALENCE, OnradSEPARATECODEEQUIVALENCE)
	ON_BN_CLICKED(IDC_radREGCODE_EQUIVALENCE, OnradREGCODEEQUIVALENCE)
	ON_BN_CLICKED(IDC_chkTITLE_TWICE_SEARCH, OnchkTITLETWICESEARCH)
	ON_BN_CLICKED(IDC_chkVOL_TITLE, OnchkVOLTITLE)
	ON_BN_CLICKED(IDC_radVOL_TITLE_RIGHTTRUNCATION, OnradVOLTITLERIGHTTRUNCATION)
	ON_BN_CLICKED(IDC_radVOL_TITLE_EQUIVALENCE, OnradVOLTITLEEQUIVALENCE)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkDiscardBook, OnchkDiscardBook)
	ON_BN_CLICKED(IDC_chkSeriesTitle, OnchkSeriesTitle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDUP_KEY_SETTING message handlers

VOID CDUP_KEY_SETTING::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CESL_Mgr::OnClose();
	//DestroyWindow();
}

BOOL CDUP_KEY_SETTING::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("DUP_KEY_SETTING")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	
//	Display();
	this->SetDisplayData();
	m_bInit = TRUE;
	OnchkAUTHOR();
	OnchkISBN();
	OnchkMEDCLASS();
	OnchkPUBLICATEYEAR();
	OnchkPUBLISHER();
	OnchkREGCODE();
	OnchkSEPARATESHELFCODE();
	OnchkTITLE();
	OnchkTITLETWICESEARCH();
	OnchkVOL();
	OnchkVOLTITLE();
	m_bInit = FALSE;

	// 2007.06.01 ADD BY PDJ
	// 복본조사 제적자료 포함여부 조건 추가함
	// 기존 방식과 형식이 달라 별도의 파일로 추가함.
	// 또한 패치시 기존 데이터 overwrite 문제로 별도로 나가야 함.
	LoadAppendCondition();

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDUP_KEY_SETTING::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);	
}

VOID CDUP_KEY_SETTING::OnOK() 
{
	// TODO: Add extra validation here
	if(!CheckDupSearchElements()) return;
	GetDisplayData();
	m_SearchKey.GetSearchKeys(m_FileName);
	SaveAppendCondition();
	ESLAfxMessageBox(_T("설정하신 중복/복본조사 키값을 저장하였습니다."));
	//CDialog::OnOK();
}

VOID CDUP_KEY_SETTING::SetDisplayData()
{
	INT titleStype;
	CButton * pRadTitleAll = (CButton *)GetDlgItem(IDC_radALLWORDSEARCH);
	CButton * pRadRight = (CButton *)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION);
	CButton * pRadEqual = (CButton *)GetDlgItem(IDC_radTITLE_EQUIVALENCE);
	CButton * pRadTitleEng = (CButton *)GetDlgItem(IDC_radTITLE_CUTSEARCH);
	CButton * pChkTwice = (CButton *)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH);
	INT ids = m_SearchKey.GetFieldKeySettingInfo(_T("본표제"), &m_chkTitle, &titleStype, &m_chkTwice);
	if(ids < 0) 
	{
		m_SearchKey.SetDefaultSearchKeys();
	}
	
	if(titleStype == 2)
	{
		pRadTitleEng->SetCheck(1);
		this->m_radTitle = -1;
		pChkTwice->EnableWindow(FALSE);
	}
	else
	{
		pRadTitleAll->SetCheck(1);
		if(titleStype < 0)
		{
			m_SearchKey.GetFieldKeySettingInfo(_T("본표제"), &m_chkTitle, &titleStype, &m_chkTwice);
		}
		this->m_radTitle = titleStype;		
	}

	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("총서명"), &m_chkSeriesTitle, &titleStype, &m_chkTwice);
	if(ids < 0) 
	{
		m_SearchKey.SetDefaultSearchKeys();
	}
	
	if(titleStype == 2)
	{
		pRadTitleEng->SetCheck(1);
		this->m_radTitle = -1;
		pChkTwice->EnableWindow(FALSE);
	}
	else
	{
		pRadTitleAll->SetCheck(1);
		if(titleStype < 0)
		{
			m_SearchKey.GetFieldKeySettingInfo(_T("총서명"), &m_chkSeriesTitle, &titleStype, &m_chkTwice);
		}
		this->m_radTitle = titleStype;		
	}
	//===================================================

//	this->m_chkTwice = 0;


	ids = m_SearchKey.GetFieldKeySettingInfo(_T("저작자"), &this->m_chkAuthor, &this->m_radAuthor, NULL);
	if(ids < 0) return;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("발행자"), &this->m_chkPublisher, &this->m_radPublisher, NULL);
	if(ids < 0) return ;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("발행년"), &this->m_chkPubYear, &this->m_radPubYear, NULL);
	if(ids < 0) return;
	this->m_radPubYear = this->m_radPubYear - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("낱권ISBN"), &this->m_chkISBN, &this->m_radISBN, NULL);
	if(ids < 0) return;
	this->m_radISBN = this->m_radISBN - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("매체구분"), &this->m_chkMedClass, &this->m_radMedCode, NULL);
	if(ids < 0) return;
	this->m_radMedCode = this->m_radMedCode - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("편권차"), &this->m_chkVol, &this->m_radVol, NULL);
	if(ids < 0) return;
	this->m_radVol = this->m_radVol - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("권서명"), &this->m_chkVolTitle, &this->m_radVolTitle, NULL);
	if(ids < 0) return;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("별치기호"), &this->m_chkShelfCode, &this->m_radShelfCode, NULL);
	if(ids < 0) return;
	this->m_radShelfCode = this->m_radShelfCode - 1;
	ids = m_SearchKey.GetFieldKeySettingInfo(_T("등록구분"), &this->m_chkRegCode, &this->m_radRegCode, NULL);
	if(ids < 0) return;
	this->m_radRegCode = this->m_radRegCode -1;	

	this->UpdateData(FALSE);

}

VOID CDUP_KEY_SETTING::GetDisplayData()
{
	this->UpdateData(TRUE);

	INT ids;
	CButton * pRadTitleAll = (CButton *)GetDlgItem(IDC_radALLWORDSEARCH);
	if(pRadTitleAll->GetCheck() == 1)
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("본표제"), this->m_chkTitle, this->m_radTitle, this->m_chkTwice);
	}
	else
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("본표제"), this->m_chkTitle, 2, this->m_chkTwice);
	}
	if(ids < 0) return;

	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
	if(pRadTitleAll->GetCheck() == 1)
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("총서명"), this->m_chkSeriesTitle, this->m_radTitle, this->m_chkTwice);
	}
	else
	{
		ids = m_SearchKey.SetFieldKeySettingInfo(_T("총서명"), this->m_chkSeriesTitle, 2, this->m_chkTwice);
	}
	if(ids < 0) return;
	//===================================================	
	
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("저작자"), this->m_chkAuthor, this->m_radAuthor, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("발행자"), this->m_chkPublisher, this->m_radPublisher, FALSE);
	if(ids < 0) return ;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("발행년"), this->m_chkPubYear, this->m_radPubYear+1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("낱권ISBN"), this->m_chkISBN, this->m_radISBN + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("매체구분"), this->m_chkMedClass, this->m_radMedCode + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("편권차"), this->m_chkVol, this->m_radVol + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("권서명"), this->m_chkVolTitle, this->m_radVolTitle, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("별치기호"), this->m_chkShelfCode, this->m_radShelfCode + 1, FALSE);
	if(ids < 0) return;
	ids = m_SearchKey.SetFieldKeySettingInfo(_T("등록구분"), this->m_chkRegCode, this->m_radRegCode + 1, FALSE);
	if(ids < 0) return;			
}

VOID CDUP_KEY_SETTING::OnchkAUTHOR() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkAuthor = !this->m_chkAuthor;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radAUTHOR_RIGHTTRUNCATION);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radAUTHOR_EQUIVALENCE);
	if(this->m_chkAuthor)
	{
		if(this->m_radAuthor == -1)
			this->m_radAuthor = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radAuthor = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnchkPUBLISHER() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkPublisher = !this->m_chkPublisher;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radPUBLISHER_RIGHTTRUNCATION);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radPUBLISHER_EQUIVALENCE);
	if(this->m_chkPublisher)
	{
		if(this->m_radPublisher == -1)
			this->m_radPublisher = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPublisher = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnchkPUBLICATEYEAR() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkPubYear = !this->m_chkPubYear;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radPUBLICATE_YEAR_EQUIVALENCE);
	if(this->m_chkPubYear)
	{
		this->m_radPubYear = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPubYear = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnchkTITLE() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkTitle = !this->m_chkTitle;
	CButton * prad1 = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH);
	CButton * prad2 = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION);
	CButton * prad3 = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE);
	CButton * prad4 = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH);
	CButton * pchk = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH);
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
//	if(this->m_chkTitle)
	if(this->m_chkTitle  || this->m_chkSeriesTitle)
	//===================================================	
	{
		prad1->EnableWindow(TRUE);
		prad2->EnableWindow(TRUE);
		prad3->EnableWindow(TRUE);
		prad4->EnableWindow(TRUE);
		pchk->EnableWindow(TRUE);
	}
	else
	{
		if(!m_bInit)
			ESLAfxMessageBox(_T("서명조건을 해제할 경우 복본/중복조사 결과가 많아질 수 있습니다.\n이 사항은 복본/중복조사 속도를 저하 시킬수 있습니다."));
		prad1->EnableWindow(FALSE);
		prad2->EnableWindow(FALSE);
		prad3->EnableWindow(FALSE);
		prad4->EnableWindow(FALSE);
		pchk->EnableWindow(FALSE);
	}
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnchkISBN() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkISBN = !this->m_chkISBN;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radISBN_EQUIVALENCE);
	if(this->m_chkISBN)
	{
		this->m_radISBN = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radISBN = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnchkMEDCLASS() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkMedClass = !this->m_chkMedClass;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radMEDIACODE_EQUIVALENCE);
	if(this->m_chkMedClass)
	{
		this->m_radMedCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radMedCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);		
}

VOID CDUP_KEY_SETTING::OnchkVOL() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkVol = !this->m_chkVol;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radVOL_EQUIVALENCE);
	if(this->m_chkVol)
	{
		this->m_radVol = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radVol = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnchkSEPARATESHELFCODE() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkShelfCode = !this->m_chkShelfCode;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radSEPARATECODE_EQUIVALENCE);
	if(this->m_chkShelfCode)
	{
		this->m_radShelfCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radShelfCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnchkREGCODE() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkRegCode = !this->m_chkRegCode;
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radREGCODE_EQUIVALENCE);
	if(this->m_chkRegCode)
	{
		this->m_radRegCode = 0;
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radRegCode = -1;
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradALLWORDSEARCH() 
{
	// TODO: Add your control notification handler code here
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH);
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE);
	if(pradAllWord->GetCheck() == 1)
	{
		if(this->m_radTitle == -1)
			this->m_radTitle = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
		pchkTwice->EnableWindow(TRUE);
	}
	
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnradTITLECUTSEARCH() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH);
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE);
	if(pradCut->GetCheck() == 1)
	{
		//this->m_radTitle = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
		pchkTwice->EnableWindow(FALSE);
	}
	else
	{
		//this->m_radTitle = -1;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
		pchkTwice->EnableWindow(TRUE);
	}
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradTITLERIGHTTRUNCATION() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH);
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH);
	if(pradCut->GetCheck() == 1)
		this->m_radTitle = -1;
	else
	{
		this->m_radTitle = 0;
		pradAllWord->SetCheck(1);
	}
	
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnradTITLEEQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	CButton * pradCut = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH);
	CButton * pchkTwice = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH);
	CButton * pradAllWord = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH);
	if(pradCut->GetCheck() == 1)
		this->m_radTitle = -1;
	else
	{
		this->m_radTitle = 1;
		pradAllWord->SetCheck(1);
	}
	
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradAUTHORRIGHTTRUNCATION() 
{
	// TODO: Add your control notification handler code here
	this->m_chkAuthor = TRUE;
	this->m_radAuthor = 0;
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnradAUTHOREQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkAuthor = TRUE;
	this->m_radAuthor = 1;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradPUBLISHERRIGHTTRUNCATION() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPublisher = TRUE;
	this->m_radPublisher = 0;
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnradPUBLISHEREQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPublisher = TRUE;
	this->m_radPublisher = 1;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradPUBLICATEYEAREQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkPubYear = TRUE;
	this->m_radPubYear = 0;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradISBNEQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkISBN = TRUE;
	this->m_radISBN = 0;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradMEDIACODEEQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkMedClass = TRUE;
	this->m_radMedCode = 0;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradVOLEQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVol = TRUE;
	this->m_radVol = 0;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradSEPARATECODEEQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkShelfCode = TRUE;
	this->m_radShelfCode = 0;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradREGCODEEQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkRegCode = TRUE;
	this->m_radRegCode = 0;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnchkTITLETWICESEARCH() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkTwice = !this->m_chkTwice;
	this->UpdateData(FALSE);
}

VOID CDUP_KEY_SETTING::OnchkVOLTITLE() 
{
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkVolTitle = !this->m_chkVolTitle;
	CButton * pchkRight = (CButton*)GetDlgItem(IDC_radVOL_TITLE_RIGHTTRUNCATION);
	CButton * pchkEqui = (CButton*)GetDlgItem(IDC_radVOL_TITLE_EQUIVALENCE);
	if(this->m_chkVolTitle)
	{
		if(this->m_radVolTitle == -1)
			this->m_radVolTitle = 0;
		pchkRight->EnableWindow(TRUE);
		pchkEqui->EnableWindow(TRUE);
	}
	else
	{
		//this->m_radPublisher = -1;
		pchkRight->EnableWindow(FALSE);
		pchkEqui->EnableWindow(FALSE);
	}	
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradVOLTITLERIGHTTRUNCATION() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVolTitle = TRUE;
	this->m_radVolTitle = 0;
	this->UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::OnradVOLTITLEEQUIVALENCE() 
{
	// TODO: Add your control notification handler code here
	this->m_chkVolTitle = TRUE;
	this->m_radVolTitle = 1;
	this->UpdateData(FALSE);	
}

BOOL CDUP_KEY_SETTING::CheckDupSearchElements()
{
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
//	if(!m_chkVolTitle && !m_chkTitle && !m_chkAuthor && !m_chkPublisher && !m_chkPubYear 
//		&& !m_chkISBN && !this->m_chkMedClass && !this->m_chkRegCode && !this->m_chkShelfCode
//		&& !this->m_chkVol)
	if(!m_chkVolTitle && !m_chkTitle && !m_chkAuthor && !m_chkPublisher && !m_chkPubYear 
		&& !m_chkISBN && !this->m_chkMedClass && !this->m_chkRegCode && !this->m_chkShelfCode
		&& !this->m_chkVol && !this->m_chkSeriesTitle)
	//===================================================
	
	{
		ESLAfxMessageBox(_T("적어도 한 개 이상의 조사항목을 선정하셔야 합니다."));
		return FALSE;
	}

	// 17/05/10 김혜영 : 복본검색 개선 ('서명검색', 'ISBN검색'으로 선택)
/*//BEFORE----------------------------------------------------------------------------------------
	//===================================================
	//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
	//if(!m_chkTitle && !m_chkAuthor && !m_chkPublisher )
	if(!m_chkTitle && !m_chkAuthor && !m_chkPublisher && !m_chkSeriesTitle )
	//===================================================
	
	{
		ESLAfxMessageBox(_T("[서명]을 조사항목으로 설정하지 않을 경우\n[저작자]나 [발행자] 항목이 조사항목으로 선정되어야 합니다."));
		PostMessage(WM_COMMAND, IDC_chkAUTHOR, BN_CLICKED);
		PostMessage(WM_COMMAND, IDC_chkPUBLISHER, BN_CLICKED);
		return FALSE;
	}
*///AFTER-----------------------------------------------------------------------------------------
	if(!m_chkTitle && !m_chkISBN && !m_chkAuthor && !m_chkPublisher && !m_chkSeriesTitle )
	{
		ESLAfxMessageBox(_T("[서명] 또는 [ISBN]을 조사항목으로 설정하지 않을 경우\n[저작자]나 [발행자] 항목이 조사항목으로 선정되어야 합니다."));
		PostMessage(WM_COMMAND, IDC_chkAUTHOR, BN_CLICKED);
		PostMessage(WM_COMMAND, IDC_chkPUBLISHER, BN_CLICKED);
		return FALSE;
	}
/*//END-------------------------------------------------------------------------------------------*/
	
	return TRUE;
}

VOID CDUP_KEY_SETTING::LoadAppendCondition()
{	
	CStdioFile file;		
	CString strLine;
	CString filename = _T("duplicate_search_key2.txt");	

	if ( !file.Open(filename, CFile::modeRead | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("복본조사 설정키가 완전하지 않습니다."));
		return;
	}	
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	while( file.ReadString(strLine) )
	{			
		
		//====================================================
		//20080530 UPDATE BY PJW : 스트링 비교를 FIND로 변경
		/*
		if ( strLine == _T("[제적자료포함여부]") )
		{
			file.ReadString(strLine);
			if ( strLine == _T("USE_YN=1") ) 
			{
				m_bIncludeDiscardedBook = TRUE; 
			}
			else 
			{
				m_bIncludeDiscardedBook = FALSE;				
			}
		}
		*/
		if ( strLine.Find(_T("[제적자료포함여부]") ) >= 0 )
		{
			file.ReadString(strLine);
			if ( strLine.Find(_T("USE_YN=1") )  >= 0 )
			{
				m_bIncludeDiscardedBook = TRUE; 
			}
			else 
			{
				m_bIncludeDiscardedBook = FALSE;				
			}
		}
		//====================================================
		
		
	}

	file.Close();

	UpdateData(FALSE);	
}

VOID CDUP_KEY_SETTING::SaveAppendCondition()
{
	UpdateData(TRUE);		
	
	CStdioFile file;		
	CString strLine;
	CString filename = _T("duplicate_search_key2.txt");	

	m_bIncludeDiscardedBook = ( (CButton*) GetDlgItem(IDC_chkDiscardBook) )->GetCheck();

	if ( !file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary )) 
	{
		AfxMessageBox(_T("복본조사 설정키가 완전하지 않습니다."));
		return;
	}
		
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	if ( m_bIncludeDiscardedBook == TRUE) 
	{
		strLine = _T("[제적자료포함여부]\nUSE_YN=1\n");
	}
	else
	{
		strLine = _T("[제적자료포함여부]\nUSE_YN=0\n");
	}
	file.WriteString(strLine);
	file.Close();	
}

HBRUSH CDUP_KEY_SETTING::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CDUP_KEY_SETTING::OnchkDiscardBook() 
{
	this->UpdateData(TRUE);
	
}
//===================================================
//2010.11.29 ADD BY PJW : 조건으로 총서명을 추가한다.
void CDUP_KEY_SETTING::OnchkSeriesTitle() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if(!m_bInit) this->m_chkSeriesTitle = !this->m_chkSeriesTitle;
	CButton * prad1 = (CButton*)GetDlgItem(IDC_radALLWORDSEARCH);
	CButton * prad2 = (CButton*)GetDlgItem(IDC_radTITLE_RIGHTTRUNCATION);
	CButton * prad3 = (CButton*)GetDlgItem(IDC_radTITLE_EQUIVALENCE);
	CButton * prad4 = (CButton*)GetDlgItem(IDC_radTITLE_CUTSEARCH);
	CButton * pchk = (CButton*)GetDlgItem(IDC_chkTITLE_TWICE_SEARCH);
	if(this->m_chkTitle || this->m_chkSeriesTitle)
	{
		prad1->EnableWindow(TRUE);
		prad2->EnableWindow(TRUE);
		prad3->EnableWindow(TRUE);
		prad4->EnableWindow(TRUE);
		pchk->EnableWindow(TRUE);
	}
	else
	{
		if(!m_bInit)
			ESLAfxMessageBox(_T("서명조건을 해제할 경우 복본/중복조사 결과가 많아질 수 있습니다.\n이 사항은 복본/중복조사 속도를 저하 시킬수 있습니다."));
		prad1->EnableWindow(FALSE);
		prad2->EnableWindow(FALSE);
		prad3->EnableWindow(FALSE);
		prad4->EnableWindow(FALSE);
		pchk->EnableWindow(FALSE);
	}
	this->UpdateData(FALSE);
}
//===================================================

