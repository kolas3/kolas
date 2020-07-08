// BOISBNAdditionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BOISBNAdditionDlg.h"
#include "..\IndexToolKit\IndexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBOISBNAdditionDlg dialog


CBOISBNAdditionDlg::CBOISBNAdditionDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBOISBNAdditionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pFormApi = NULL;
	m_pFormApi = new CBOFormApi(this);

	m_pGrid = NULL;
	m_pCM = NULL;
	m_pDM = NULL;
	m_nSelectRow = -1;

	m_strAdditionISBN = _T("");
	m_strAdditionAddCode = _T("");
	m_strAdditionMissISBN = _T("");
}
CBOISBNAdditionDlg::~CBOISBNAdditionDlg()
{
	if(m_pFormApi)
	{
		delete m_pFormApi;
		m_pFormApi = NULL;
	}
}

VOID CBOISBNAdditionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBOISBNAdditionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBOISBNAdditionDlg, CDialog)
	//{{AFX_MSG_MAP(CBOISBNAdditionDlg)
	ON_BN_CLICKED(IDC_bISBN_CHECK, OnbISBNCHECK)
	ON_BN_CLICKED(IDC_bINPUT_ISBN, OnbINPUTISBN)
	ON_BN_CLICKED(IDC_bMODIFY_ISBN, OnbMODIFYISBN)
	ON_BN_CLICKED(IDC_bDELETE_ISBN, OnbDELETEISBN)
	ON_BN_CLICKED(IDC_bCLOSE_ISBN, OnbCLOSEISBN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBOISBNAdditionDlg Operations

INT CBOISBNAdditionDlg::ValidateData()
{
	// ³¹±Ç ISBN Ã¼Å©
	CString strItem;
	m_pCM->GetControlMgrData(_T("³¹±ÇISBN"), strItem);
	if(strItem.IsEmpty())
	{
		ESLAfxMessageBox(_T("³¹±Ç ISBNÇ×¸ñÀ» ÀÔ·ÂÇÏ¼¼¿ä"));
		return -1;
	}
	
	return 0;
}

INT CBOISBNAdditionDlg::Display()
{
	if(m_nSelectRow < 0) m_nSelectRow = 0;
	m_pCM->AllControlDisplay(m_nSelectRow);
	m_pGrid->SetReverse(m_nSelectRow);
	
	return 0;
}

INT CBOISBNAdditionDlg::EncodeAdditionISBN()
{
	m_strAdditionISBN.Empty();
	m_strAdditionAddCode.Empty();
	m_strAdditionMissISBN.Empty();

	INT rowCnt = m_pDM->GetRowCount();
	CString strTmpData;
	for(INT i=0 ; i<rowCnt ; i++)
	{
		if(i==0)
		{
			strTmpData = m_pDM->GetCellData(_T("³¹±ÇISBN"), i);
			((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
			m_strAdditionISBN = strTmpData;
			m_strAdditionAddCode = m_pDM->GetCellData(_T("³¹±ÇISBNºÎ°¡±âÈ£"), i);
			m_strAdditionMissISBN = m_pDM->GetCellData(_T("³¹±Ç¿À·ùISBN"), i);
		}
		else
		{
			strTmpData = m_pDM->GetCellData(_T("³¹±ÇISBN"), i);
			((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
			m_strAdditionISBN += _T("|") + strTmpData;
			m_strAdditionAddCode += _T("|") + m_pDM->GetCellData(_T("³¹±ÇISBNºÎ°¡±âÈ£"), i);
			m_strAdditionMissISBN += _T("|") + m_pDM->GetCellData(_T("³¹±Ç¿À·ùISBN"), i);
		}
	}
	return 0;
}

INT CBOISBNAdditionDlg::DecodeAdditionISBN()
{
	if (m_strAdditionISBN.IsEmpty())
		return 0;
	
	CStringArray arrISBN;
	CStringArray arrAddCode;
	CStringArray arrMissISBN;
	ParsingString(_T("|"), m_strAdditionISBN, &arrISBN);
	ParsingString(_T("|"), m_strAdditionAddCode, &arrAddCode);
	ParsingString(_T("|"), m_strAdditionMissISBN, &arrMissISBN);
	
	INT nISBNcnt = arrISBN.GetSize();
	INT nADDcnt = arrAddCode.GetSize();
	INT nERRORcnt = arrMissISBN.GetSize();
	INT cnt = MaxCount(nISBNcnt, nADDcnt, nERRORcnt);
	INT ids = -1;
	INT iResult, nRow;
	CString strTmpData, strConvData;
	for(INT i=0 ; i<cnt ; i++)
	{
		m_pDM->InsertRow(-1);
		nRow = m_pDM->GetRowCount()-1;
		if(nISBNcnt-1 >= i)
		{
			strTmpData = arrISBN.GetAt(i);
			ids = m_isbnChecker.ConvertISBN(strTmpData.GetBuffer(0), strConvData.GetBuffer(MAX_PATH), &iResult, FALSE);
			strConvData.ReleaseBuffer();
			if(ids < 0)
				m_pDM->SetCellData(_T("³¹±ÇISBN"), strTmpData, nRow);
			else 
			{
				INT size = strConvData.GetLength();
				strConvData = strConvData.Left(size);
				m_pDM->SetCellData(_T("³¹±ÇISBN"), strConvData, nRow);
			}
		}
		if(nADDcnt-1 >= i)
			m_pDM->SetCellData(_T("³¹±ÇISBNºÎ°¡±âÈ£"), arrAddCode.GetAt(i), nRow);
		if(nERRORcnt-1 >= i)
			m_pDM->SetCellData(_T("³¹±Ç¿À·ùISBN"), arrMissISBN.GetAt(i), nRow);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CBOISBNAdditionDlg message handlers

BOOL CBOISBNAdditionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids = InitESL_Mgr(_T("K2UPGRADE_Ãß°¡³¹±ÇISBN°ü¸®"));
	if (ids < 0) 
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	m_pCM = FindCM(_T("CM_Ãß°¡³¹±ÇISBN°ü¸®"));
	m_pDM = FindDM(_T("DM_Ãß°¡³¹±ÇISBN°ü¸®"));
	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("MainGrid"));

	// Decode Addition ISBN String
	if (DecodeAdditionISBN() < 0)
	{		
		AfxMessageBox(_T("Decode Addition ISBN String Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}	

	// Display
	Display();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBOISBNAdditionDlg::OnbISBNCHECK() 
{
	// ³¹±Ç Ã¼Å©
	CString strEaISBNFrom;
	CString strEaISBNTo;
	m_pCM->GetControlMgrData(_T("³¹±ÇISBN"), strEaISBNFrom);
	if (strEaISBNFrom.IsEmpty()) return;

	INT ids = m_pFormApi->CheckISBN(strEaISBNFrom, strEaISBNTo);
	if (ids < 0) return;
	
	m_pCM->SetControlMgrData(_T("³¹±ÇISBN"), strEaISBNTo);
}

VOID CBOISBNAdditionDlg::OnbINPUTISBN() 
{
	if (ValidateData() < 0) return;
	
	m_pDM->InsertRow(-1);
	m_nSelectRow = m_pDM->GetRowCount() - 1;
	CString strTmpData;
	m_pCM->GetControlMgrData(_T("³¹±ÇISBN"), strTmpData);
	m_pDM->SetCellData(_T("³¹±ÇISBN"), strTmpData, m_nSelectRow);
	m_pCM->GetControlMgrData(_T("³¹±ÇISBNºÎ°¡±âÈ£"), strTmpData);
	m_pDM->SetCellData(_T("³¹±ÇISBNºÎ°¡±âÈ£"), strTmpData, m_nSelectRow);
	m_pCM->GetControlMgrData(_T("³¹±Ç¿À·ùISBN"), strTmpData);
	m_pDM->SetCellData(_T("³¹±Ç¿À·ùISBN"), strTmpData, m_nSelectRow);
	
	Display();
}

VOID CBOISBNAdditionDlg::OnbMODIFYISBN() 
{
	if (ValidateData() < 0) return;
	CString strTmpData;
	m_pCM->GetControlMgrData(_T("³¹±ÇISBN"), strTmpData);
	m_pDM->SetCellData(_T("³¹±ÇISBN"), strTmpData, m_nSelectRow);
	m_pCM->GetControlMgrData(_T("³¹±ÇISBNºÎ°¡±âÈ£"), strTmpData);
	m_pDM->SetCellData(_T("³¹±ÇISBNºÎ°¡±âÈ£"), strTmpData, m_nSelectRow);
	m_pCM->GetControlMgrData(_T("³¹±Ç¿À·ùISBN"), strTmpData);
	m_pDM->SetCellData(_T("³¹±Ç¿À·ùISBN"), strTmpData, m_nSelectRow);	

	Display();
}

VOID CBOISBNAdditionDlg::OnbDELETEISBN() 
{
	INT nPrevRow = m_nSelectRow;
	m_pDM->DeleteRow(m_nSelectRow);
	if(nPrevRow < 0) nPrevRow = 0;
	m_nSelectRow = nPrevRow;

	Display();
}

BEGIN_EVENTSINK_MAP(CBOISBNAdditionDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CBOISBNAdditionDlg)
	ON_EVENT(CBOISBNAdditionDlg, IDC_gISBN, -605 /* MouseDown */, OnMouseDowngISBN, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CBOISBNAdditionDlg::OnMouseDowngISBN(short Button, short Shift, long x, long y) 
{
	m_nSelectRow = m_pGrid->GetRow() - 1;
	Display();
}

VOID CBOISBNAdditionDlg::OnbCLOSEISBN() 
{
	EncodeAdditionISBN();
	CDialog::OnOK();
}

INT CBOISBNAdditionDlg::ParsingString(CString DelimeterStr, CString strSource, CStringArray *pTargetArray)
{
	if(strSource.GetLength() == 0) return 0;
	INT pos = strSource.Find(DelimeterStr, 0);
	CString strTmpData;
	while(pos >= 0)
	{
		strTmpData = strSource.Mid(0, pos);
		strSource = strSource.Mid(pos+1);
		((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strTmpData);
//		if(!CheckIsHave(pTargetArray, strTmpData))
		pTargetArray->Add(strTmpData);
		pos = strSource.Find(DelimeterStr, 0);
	}
	((CIndexTool*)m_pInfo->pIndexTool)->GetIndex()->MakeIndex(strSource);
	if(!CheckIsHave(pTargetArray, strSource))
		pTargetArray->Add(strSource);
	return 0;
}

BOOL CBOISBNAdditionDlg::CheckIsHave(CStringArray *codeValue, CString code)
{
	INT cnt = codeValue->GetSize();
	if(cnt == 0) return FALSE;
	for(INT i=0 ; i<cnt ; i++)
		if(codeValue->GetAt(i) == code) return TRUE;
	return FALSE;
}

INT CBOISBNAdditionDlg::MaxCount(INT nISBNcnt, INT nADDcnt, INT nERRORcnt)
{
	INT nTmpData = -1;
	if(nTmpData < nISBNcnt)
		nTmpData = nISBNcnt;
	if(nTmpData < nADDcnt)
		nTmpData = nADDcnt;
	if(nTmpData < nERRORcnt)
		nTmpData = nERRORcnt;
	return nTmpData;
}
