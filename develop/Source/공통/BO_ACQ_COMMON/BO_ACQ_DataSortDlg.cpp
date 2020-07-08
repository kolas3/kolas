// BO_ACQ_DataSortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BO_ACQ_DataSortDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DataSortDlg dialog


CBO_ACQ_DataSortDlg::CBO_ACQ_DataSortDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CBO_ACQ_DataSortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_ACQ_DataSortDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pComboMgr = NULL;
	m_strSortElementFileName = _T("../cfg/SortElements.cfg");
	m_strSortConditionFileName = _T("../cfg/SortSetting.cfg");
	m_bNotHaveSortElement = FALSE;
}

CBO_ACQ_DataSortDlg::~CBO_ACQ_DataSortDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
}

VOID CBO_ACQ_DataSortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_ACQ_DataSortDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_ACQ_DataSortDlg, CDialog)
	//{{AFX_MSG_MAP(CBO_ACQ_DataSortDlg)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_1, OnSelchangecmbORDERSORT1)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_2, OnSelchangecmbORDERSORT2)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_3, OnSelchangecmbORDERSORT3)
	ON_CBN_SELCHANGE(IDC_cmbORDER_SORT_4, OnSelchangecmbORDERSORT4)
	ON_BN_CLICKED(IDC_btnSAVE_DEFAULT_VALUE, OnbtnSAVEDEFAULTVALUE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_ACQ_DataSortDlg message handlers

BOOL CBO_ACQ_DataSortDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (InitESL_Mgr(_T("K2UPGRADE_주문일련번호")) < 0)
	{
		ESLAfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}	
	m_pCM = FindCM(_T("CM_주문일련번호"));
	m_pComboMgr = new CMulitComboBoxManager(this);

	GetSortElements(m_strSortElementFileName);
	GetDefaultSortCondition(m_strSortConditionFileName);
	m_arrSortCond.Add(_T("오름차순"));	m_arrSortCond.Add(_T("내림차순"));
	if(!Init())
	{
		ESLAfxMessageBox(_T("정렬화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_ACQ_DataSortDlg::OnSelchangecmbORDERSORT1() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("제1조건"), strCurSel);
	m_arrSort2.RemoveAll();
	INT cnt = m_arrSort1.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort1.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("사용안함")) continue;
		else m_arrSort2.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_SortCondition.m_strSecondAlias, &m_arrSort2, _T("제2조건"));
	OnSelchangecmbORDERSORT2();	
}

VOID CBO_ACQ_DataSortDlg::OnSelchangecmbORDERSORT2() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("제2조건"), strCurSel);
	m_arrSort3.RemoveAll();
	INT cnt = m_arrSort2.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort2.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("사용안함")) continue;
		else m_arrSort3.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_SortCondition.m_strThirdAlias, &m_arrSort3, _T("제3조건"));
	OnSelchangecmbORDERSORT3();		
}

VOID CBO_ACQ_DataSortDlg::OnSelchangecmbORDERSORT3() 
{
	// TODO: Add your control notification handler code here
	CString strCurSel, strTmpData;
	m_pCM->GetControlMgrData(_T("제3조건"), strCurSel);
	m_arrSort4.RemoveAll();
	INT cnt = m_arrSort3.GetSize();
	for(INT i=0 ; i<cnt ; i++)
	{
		strTmpData = m_arrSort3.GetAt(i);
		if(strTmpData == strCurSel && strTmpData != _T("사용안함")) continue;
		else m_arrSort4.Add(strTmpData);
	}
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_SortCondition.m_strFourthAlias, &m_arrSort4, _T("제4조건"));	
}

VOID CBO_ACQ_DataSortDlg::OnSelchangecmbORDERSORT4() 
{
	// TODO: Add your control notification handler code here
	
}

VOID CBO_ACQ_DataSortDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strTmpData, strTmpAlias, strTmpCond;
	m_pCM->GetControlMgrData(_T("제1조건"), strTmpData);
	if(strTmpData != _T("사용안함")) 
	{
		m_pCM->GetControlMgrData(_T("제1정렬조건"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strTmpAlias);
		m_SortCondition.m_strFirstAlias = strTmpAlias;
		if(strTmpCond == _T("내림차순")) m_SortCondition.m_nFirstType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nFirstType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nFirstType = 0;

	m_pCM->GetControlMgrData(_T("제2조건"), strTmpData);
	if(strTmpData != _T("사용안함")) 
	{
		m_pCM->GetControlMgrData(_T("제2정렬조건"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strTmpAlias);
		m_SortCondition.m_strSecondAlias= strTmpAlias;
		if(strTmpCond == _T("내림차순")) m_SortCondition.m_nSecondType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nSecondType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nSecondType = 0;

	m_pCM->GetControlMgrData(_T("제3조건"), strTmpData);
	if(strTmpData != _T("사용안함")) 
	{
		m_pCM->GetControlMgrData(_T("제3정렬조건"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strTmpAlias);
		m_SortCondition.m_strThirdAlias = strTmpAlias;
		if(strTmpCond == _T("내림차순")) m_SortCondition.m_nThirdType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nThirdType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nThirdType = 0;
	
	m_pCM->GetControlMgrData(_T("제4조건"), strTmpData);
	if(strTmpData != _T("사용안함")) 
	{
		m_pCM->GetControlMgrData(_T("제4정렬조건"), strTmpCond);
		m_MapSortData.Lookup(strTmpData, strTmpAlias);
		m_SortCondition.m_strFourthAlias = strTmpAlias;
		if(strTmpCond == _T("내림차순")) m_SortCondition.m_nFourthType = ESL_DATAMGR_SORT_DESC;
		else m_SortCondition.m_nFourthType = ESL_DATAMGR_SORT_ASC;
	}
	else m_SortCondition.m_nFourthType = 0;

	SetDataSort();

	CDialog::OnOK();
}

BOOL CBO_ACQ_DataSortDlg::Init()
{
	INT nCnt = m_MapSortData.GetCount();
	INT i = 0;
	CString strKey, strValue;
	POSITION pos = m_MapSortData.GetStartPosition();
	while(pos)
	{
		m_MapSortData.GetNextAssoc(pos, strKey, strValue);
		m_arrSort1.Add(strKey);
		if(strKey != m_SortCondition.m_strFirstAlias)
			m_arrSort2.Add(strKey);
		if(strKey != m_SortCondition.m_strFirstAlias && strKey != m_SortCondition.m_strSecondAlias)
			m_arrSort3.Add(strKey);
		if(strKey != m_SortCondition.m_strFirstAlias && strKey != m_SortCondition.m_strSecondAlias && strKey != m_SortCondition.m_strThirdAlias)
			m_arrSort4.Add(strKey);
		 i++;
	}
	m_arrSort1.Add(_T("사용안함"));		 m_arrSort2.Add(_T("사용안함"));
	m_arrSort3.Add(_T("사용안함"));		 m_arrSort4.Add(_T("사용안함"));
	SetComboData();
	return TRUE;
}

VOID CBO_ACQ_DataSortDlg::SetComboData()
{
	CString strSortCond;
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_SortCondition.m_strFirstAlias, &m_arrSort1, _T("제1조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_SortCondition.m_strSecondAlias, &m_arrSort2, _T("제2조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_SortCondition.m_strThirdAlias, &m_arrSort3, _T("제3조건"));
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_SortCondition.m_strFourthAlias, &m_arrSort4, _T("제4조건"));
	if(m_SortCondition.m_nFirstType == ESL_DATAMGR_SORT_DESC) strSortCond = _T("내림차순");
	else strSortCond = _T("오름차순");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &m_arrSortCond, _T("제1정렬조건"));
	if(m_SortCondition.m_nSecondType == ESL_DATAMGR_SORT_DESC) strSortCond = _T("내림차순");
	else strSortCond = _T("오름차순");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &m_arrSortCond, _T("제2정렬조건"));
	if(m_SortCondition.m_nThirdType == ESL_DATAMGR_SORT_DESC) strSortCond = _T("내림차순");
	else strSortCond = _T("오름차순");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &m_arrSortCond, _T("제3정렬조건"));
	if(m_SortCondition.m_nFourthType == ESL_DATAMGR_SORT_DESC) strSortCond = _T("내림차순");
	else strSortCond = _T("오름차순");
	m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, strSortCond, &m_arrSortCond, _T("제4정렬조건"));
}

VOID CBO_ACQ_DataSortDlg::OnbtnSAVEDEFAULTVALUE() 
{
	// TODO: Add your control notification handler code here
	SetDefaultSortCondition(m_strSortConditionFileName);	
}

INT CBO_ACQ_DataSortDlg::GetDefaultSortCondition(CString filename)
{
	if(filename.GetLength() == 0)
		filename = _T("../cfg/SortSetting.cfg");

	INT ids, i ;
	ids = i = 0;
	CString tmpstr, tmpvalue, tmpalias;
	CStdioFile file;
	CSortCondition * psfield = NULL;

	if ( file.Open(filename, CFile::modeRead | CFile::typeBinary) == NULL ) // 파일을 못 읽었을 경우, 
	{													// Default로 설정
		MakeDefaultSortCondition();
		return 0;
	}
	else
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while(file.ReadString(tmpstr))
		{
			ids = CheckString(tmpstr);
			if(ids == -1) continue;
			// 키설정
			else if(ids == 0) 
			{
				tmpvalue = GetKeyName(tmpstr);
				if(tmpvalue.GetLength() == 0) continue;
			}
			// Alias와 Value
			else
			{
				CString strAlias, strValue;
				ids = GetAliasAndValue(strAlias, strValue, tmpstr);
				if(strAlias.GetLength() == 0 || strValue.GetLength() == 0) continue;
				if (strAlias == _T("SortType"))
				{
					INT Sortids = 0;            
					if (strValue == _T("오름차순")) Sortids = 1;
					else Sortids = -1;
			
					if (tmpvalue == _T("1조건"))		m_SortCondition.m_nFirstType = Sortids;
					else if (tmpvalue == _T("2조건"))	m_SortCondition.m_nSecondType = Sortids;
					else if (tmpvalue == _T("3조건"))	m_SortCondition.m_nThirdType = Sortids;
					else if (tmpvalue == _T("4조건"))	m_SortCondition.m_nFourthType = Sortids;
				}
				else
				{
					if (tmpvalue == _T("1조건"))		m_SortCondition.m_strFirstAlias = strValue;
					else if (tmpvalue == _T("2조건"))	m_SortCondition.m_strSecondAlias = strValue;
					else if (tmpvalue == _T("3조건"))	m_SortCondition.m_strThirdAlias = strValue;
					else if (tmpvalue == _T("4조건"))	m_SortCondition.m_strFourthAlias = strValue;
				}
			}
		}
	}
	file.Close();
//	DisplaySetting();
	return 0;
}

CString CBO_ACQ_DataSortDlg::GetKeyName(CString str)
{
	if(str.GetLength() == 0) return _T("");
	INT firstidx, lastidx;
	firstidx = lastidx = 0;
	firstidx = str.Find('[');
	lastidx = str.Find(']');
	CString value = str.Mid(firstidx+1, lastidx - firstidx - 1);
	return value;
}

INT CBO_ACQ_DataSortDlg::CheckString(CString str)
{
	if(str.GetLength() == 0) return -1;
	str.TrimLeft();		str.TrimRight();
	TCHAR firstchar = str.GetAt(0);
	if(firstchar == '[')	
		return 0;
	return 1; 
}

INT CBO_ACQ_DataSortDlg::GetAliasAndValue(CString &strAlias, CString &strValue, CString str)
{
	if(str.GetLength() == 0) return -1;
	INT sepidx = str.Find('=');
	INT maxidx = str.GetLength() - 1;
	strAlias = str.Mid(0, sepidx);
	strValue = str.Mid(sepidx+1, maxidx);
	return 0;
}

INT CBO_ACQ_DataSortDlg::SetDefaultSortCondition(CString filename)
{
	CString strTmpAlias1, strTmpCond1;
	CString strTmpAlias2, strTmpCond2;
	CString strTmpAlias3, strTmpCond3;
	CString strTmpAlias4, strTmpCond4;

	m_pCM->GetControlMgrData(_T("제1조건"), strTmpAlias1);
	m_pCM->GetControlMgrData(_T("제2조건"), strTmpAlias2);
	m_pCM->GetControlMgrData(_T("제3조건"), strTmpAlias3);
	m_pCM->GetControlMgrData(_T("제4조건"), strTmpAlias4);
	m_pCM->GetControlMgrData(_T("제1정렬조건"), strTmpCond1);
	m_pCM->GetControlMgrData(_T("제2정렬조건"), strTmpCond2);
	m_pCM->GetControlMgrData(_T("제3정렬조건"), strTmpCond3);
	m_pCM->GetControlMgrData(_T("제4정렬조건"), strTmpCond4);

	if(filename.GetLength() == 0)
		filename = _T("../cfg/SortSetting.cfg");
	
	INT ids, i ;
	ids = i = 0;
	CString strTmpData, strData;
	CStdioFile file;

	if ( file.Open(filename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == NULL ) 
	{
		filename += _T(" 파일을 생성할수 없습니다.");
		AfxMessageBox(filename, MB_ICONSTOP);
		return -1;
	}
	else
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		strTmpData = _T("[1조건]");
		strTmpData += _T("\r\n");
		file.WriteString(strTmpData);
		strData.Format(_T("Alias=%s"), strTmpAlias1);
		strData += _T("\r\n");
		file.WriteString(strData);
		strData.Format(_T("SortType=%s"), strTmpCond1); 
		strData += _T("\r\n");
		file.WriteString(strData);

		strTmpData = _T("[2조건]");
		strTmpData += _T("\r\n");
		file.WriteString(strTmpData);
		strData.Format(_T("Alias=%s"), strTmpAlias2);
		strData += _T("\r\n");
		file.WriteString(strData);
		strData.Format(_T("SortType=%s"), strTmpCond2); 
		strData += _T("\r\n");
		file.WriteString(strData);

		strTmpData = _T("[3조건]");
		strTmpData += _T("\r\n");
		file.WriteString(strTmpData);
		strData.Format(_T("Alias=%s"), strTmpAlias3);
		strData += _T("\r\n");
		file.WriteString(strData);
		strData.Format(_T("SortType=%s"), strTmpCond3); 
		strData += _T("\r\n");
		file.WriteString(strData);

		strTmpData = _T("[4조건]");
		strTmpData += _T("\r\n");
		file.WriteString(strTmpData);
		strData.Format(_T("Alias=%s"), strTmpAlias4);
		strData += _T("\r\n");
		file.WriteString(strData);
		strData.Format(_T("SortType=%s"), strTmpCond4); 
		strData += _T("\r\n");
		file.WriteString(strData);
	}
	file.Close();
	AfxMessageBox(_T("기본값 설정을 저장하였습니다.")); 
	
	return 0;	
}

INT CBO_ACQ_DataSortDlg::GetSortElements(CString strFileName)
{
	if(strFileName.GetLength() == 0)
		strFileName = _T("../cfg/SortElements.cfg");
	INT ids, i ;
	ids = i = 0;

	CString strAlias, strValue1, strValue2, tmpstr;

	CStdioFile file;

	if ( file.Open(strFileName, CFile::modeRead | CFile::typeBinary) == NULL ) // 파일을 못 읽었을 경우,    // 3) ?????  
	{													// Default로 설정
		MakeDefaultSortElement();
		return 0;
	}
	else
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		while(file.ReadString(tmpstr))
		{
			tmpstr.TrimLeft();		tmpstr.TrimRight();
			ids = CheckString(tmpstr);
			if(ids == -1) continue;
			else
			{
				ids = GetSortElementByFile(strAlias, strValue1, strValue2, tmpstr);
				if (strAlias.GetLength() == 0 || strValue1.GetLength() == 0) continue;
				if (m_nType == 1) m_MapSortData.SetAt(strAlias, strValue1);
				else m_MapSortData.SetAt(strAlias, strValue2);
			}
		}
	}
	file.Close();
	return 0;
}

INT CBO_ACQ_DataSortDlg::GetSortElementByFile(CString &strAlias, CString &strValue1, CString &strValue2, CString str)
{
	if(str.GetLength() == 0) return -1;
	INT sepidx1 = str.Find('=');
	INT sepidx2 = str.Find('-');
	INT maxidx = str.GetLength() - 1;
	strAlias = str.Mid(0, sepidx1);		
	strValue1 = str.Mid(sepidx1 + 1, sepidx2 - sepidx1 - 1); 
	strValue2 = str.Mid(sepidx2 + 1, maxidx);
	strAlias.TrimLeft();	strAlias.TrimRight();
	strValue1.TrimLeft();	strValue1.TrimRight();
	strValue2.TrimLeft();	strValue2.TrimRight();
	
	return 0;
}

INT CBO_ACQ_DataSortDlg::SetDataSort()
{

	CString strGroupNo, strAcqYear, strWorkCode;
	// SORT
	INT nCondColumns = 0;
	CString strAlias[32];
	INT nCond[32];
	if(m_nType == 2)
	{
		strAlias[nCondColumns] = _T("BP_수입구분");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	}
	strAlias[nCondColumns] = _T("BP_수입년도");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	strAlias[nCondColumns] = _T("BP_그룹번호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;

	if(m_SortCondition.m_nFirstType != 0)
	{
		strAlias[nCondColumns] = m_SortCondition.m_strFirstAlias;	nCond[nCondColumns] = m_SortCondition.m_nFirstType;		nCondColumns++;
	}
	if(m_SortCondition.m_nSecondType != 0)
	{
		strAlias[nCondColumns] = m_SortCondition.m_strSecondAlias;	nCond[nCondColumns] = m_SortCondition.m_nSecondType;	nCondColumns++;
	}
	if(m_SortCondition.m_nThirdType != 0)
	{
		strAlias[nCondColumns] = m_SortCondition.m_strThirdAlias;	nCond[nCondColumns] = m_SortCondition.m_nThirdType;		nCondColumns++;
	}
	if(m_SortCondition.m_nFourthType != 0)
	{
		strAlias[nCondColumns] = m_SortCondition.m_strFourthAlias;	nCond[nCondColumns] = m_SortCondition.m_nFourthType;	nCondColumns++;
	}
	if(m_nType == 2)
	{
		strAlias[nCondColumns] = _T("BB_일련번호");		nCond[nCondColumns] = ESL_DATAMGR_SORT_ASC;		nCondColumns++;
	}
	m_pDM->SORT(nCondColumns, strAlias, nCond, ESL_DATAMGR_SORT_ASC);

	CString strMsg;
	strMsg.Format(_T("자료 정렬을 완성하였습니다."));
	ESLAfxMessageBox(strMsg);
	
	return 0;
}

CSortCondition * CBO_ACQ_DataSortDlg::GetSortCondition(CString strFileName)
{
	GetSortElements(m_strSortElementFileName);
	GetDefaultSortCondition(m_strSortConditionFileName);
	CString strTmpData;
	m_MapSortData.Lookup(m_SortCondition.m_strFirstAlias, strTmpData);
	if(!strTmpData.IsEmpty()) m_SortCondition.m_strFirstAlias = strTmpData;
	else m_SortCondition.m_nFirstType = 0;
	m_MapSortData.Lookup(m_SortCondition.m_strSecondAlias, strTmpData);
	if(!strTmpData.IsEmpty()) m_SortCondition.m_strSecondAlias = strTmpData;
	else m_SortCondition.m_nSecondType = 0;
	m_MapSortData.Lookup(m_SortCondition.m_strThirdAlias, strTmpData);
	if(strTmpData.IsEmpty()) m_SortCondition.m_nThirdType = 0;
	else m_SortCondition.m_strThirdAlias = strTmpData;
	m_MapSortData.Lookup(m_SortCondition.m_strFourthAlias, strTmpData);
	if(strTmpData.IsEmpty()) m_SortCondition.m_nFourthType = 0;
	else m_SortCondition.m_strFourthAlias = strTmpData;

	return &m_SortCondition;
}

INT CBO_ACQ_DataSortDlg::MakeDefaultSortCondition()
{
	CString strKey, strValue;
	POSITION pos = m_MapSortData.GetStartPosition();
	INT i = 0;
	while(pos)
	{
		m_MapSortData.GetNextAssoc(pos, strKey, strValue);
		if(i==0) 
		{
			m_SortCondition.m_strFirstAlias = strKey;
			m_SortCondition.m_nFirstType = 1;
		}
		else if(i==1) 
		{
			m_SortCondition.m_strSecondAlias = strKey;
			m_SortCondition.m_nSecondType = 1;
		}
		else if(i==2) 
		{
			m_SortCondition.m_strThirdAlias = strKey;
			m_SortCondition.m_nThirdType = 1;
		}
		else if(i==3) 
		{
			m_SortCondition.m_strFourthAlias = strKey;
			m_SortCondition.m_nFourthType = 1;
		}
		i++;
	}
	return 0;
}

INT CBO_ACQ_DataSortDlg::MakeDefaultSortElement()
{
	m_MapSortData.SetAt(_T("서명"), _T("IBS_본표제"));
	m_MapSortData.SetAt(_T("주제명"), _T("BP_주제구분"));
	m_MapSortData.SetAt(_T("발행년"), _T("IBS_발행년"));
	m_MapSortData.SetAt(_T("발행자"), _T("IBS_발행자"));
	m_MapSortData.SetAt(_T("저자"), _T("IBS_저작자"));

	if(m_strSortElementFileName.IsEmpty()) return 0;
	
	INT ids, i ;
	ids = i = 0;
	CString strTmpData, strData, strKey, strValue;
	CStdioFile file;

	if ( file.Open(m_strSortElementFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == NULL ) 
	{
		strTmpData.Format(_T("[%s] 파일을 생성할수 없습니다."), m_strSortElementFileName);
		AfxMessageBox(strTmpData, MB_ICONSTOP);
		return -1;
	}
	else
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
		
		POSITION pos = m_MapSortData.GetStartPosition();
		while(pos)
		{
			m_MapSortData.GetNextAssoc(pos, strKey, strValue);
			strTmpData.Format(_T("%s=%s-%s\r\n"), strKey, strValue, strValue);
			file.WriteString(strTmpData);
		}
	}
	file.Close();
	return 0;
}
