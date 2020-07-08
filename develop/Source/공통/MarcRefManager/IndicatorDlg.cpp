// IndicatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IndicatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndicatorDlg dialog


CIndicatorDlg::CIndicatorDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndicatorDlg)
	m_chkDefaultYN = FALSE;
	m_radOrder = 0;
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_nIndOrder = -1;
	m_nCurrentParentIdx = -1;
	m_strMARC_TAG_ALIAS = _T("");
	m_strPK = _T("");

	m_pParentDM1 = NULL;
	m_pParentGrid1 = NULL;

	m_pParentDM2 = NULL;
	m_pParentGrid2 = NULL;

	// KOL.UDF.022 시큐어코딩 보완
	m_nOpenMode = -1;
}


VOID CIndicatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndicatorDlg)
	DDX_Check(pDX, IDC_chkIndicatorDefault, m_chkDefaultYN);
	DDX_Radio(pDX, IDC_raidoIndicator1, m_radOrder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndicatorDlg, CDialog)
	//{{AFX_MSG_MAP(CIndicatorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndicatorDlg message handlers

BOOL CIndicatorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("마크레프_지시기호") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 마크레프_지시기호") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_마크레프_지시기호"));
	m_pDM = FindDM(_T("DM_마크레프_지시기호"));

	if(!m_pCM || !m_pDM)
	{
		AfxMessageBox( _T("reference Error : 마크레프_지시기호") );
		return FALSE;		
	}

	if(InitOpenMode()<0)
	{
		AfxMessageBox( _T("지시기호 정보 초기화에 실패!") );
		return FALSE;		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CIndicatorDlg::InitOpenMode()
{
	
	if(m_nOpenMode>0) //입력
	{
		if(!m_pParentDM1 || !m_pParentGrid1 || !m_pParentDM2 || !m_pParentGrid2)
		{
			AfxMessageBox( _T("부모 정보를 찾을 수 없습니다!") );
			return -1;		
		}
		
		
		if(m_strMARC_TAG_ALIAS.IsEmpty())
		{
			AfxMessageBox( _T("태그 알리아스를 찾을 수 없습니다!") );
			return -1;		
		}
		
	}
	else //수정 
	{

		if(!m_pParentDM1 || !m_pParentGrid1 || !m_pParentDM2 || !m_pParentGrid2 || m_nCurrentParentIdx<0)
		{
			AfxMessageBox( _T("부모 정보를 찾을 수 없습니다!") );
			return -1;		
		}
		

		if(m_nIndOrder<0)
		{
			AfxMessageBox( _T("지시기호 순서가 입력되지 않았습니다") );
			return -1;			
		}
			
		if(1 == m_nIndOrder)
		{
		    m_strPK = m_pParentDM1->GetCellData(_T("PK"),m_nCurrentParentIdx);
		}
		else if(2 == m_nIndOrder)
		{
		    m_strPK = m_pParentDM2->GetCellData(_T("PK"),m_nCurrentParentIdx);
		}

		if(m_strPK.IsEmpty())
		{
			AfxMessageBox( _T("PK를 찾을 수 없습니다!") );
			return -1;		
		}	
		
		CString strWhere;
		strWhere.Format(_T("PK = %s"),m_strPK);

		if(1 == m_nIndOrder) m_pDM->TBL_NAME = _T("MARC_FIRST_INDICATOR_TBL");
		else if(2 == m_nIndOrder) m_pDM->TBL_NAME = _T("MARC_SECOND_INDICATOR_TBL");

		m_pDM->RefreshDataManager(strWhere);

		INT nRow = m_pDM->GetRowCount();

		if(0 == nRow) 
		{
			AfxMessageBox( _T("해당 PK의 데이터가 없습니다!") );
			return -1;					
		}

		m_pCM->AllControlDisplay(0);
		

		//지시기호 순서는 수정하지 못하게 
		if(1 == m_nIndOrder)
		{
			m_radOrder = 0;
			GetDlgItem(IDC_radioIndicator2)->ShowWindow(FALSE);
		}
		else if(2 == m_nIndOrder)
		{
			m_radOrder = 1;
			GetDlgItem(IDC_raidoIndicator1)->ShowWindow(FALSE);
		}


		//기본값
		CString strDefaultYN;
		strDefaultYN = m_pDM->GetCellData(_T("DEFAULT_YN"),0);

		if(_T("V") == strDefaultYN)
		{
			m_chkDefaultYN = TRUE;
		}
	
	}
	
	UpdateData(FALSE);


	return 0;
}

VOID CIndicatorDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	CString strIndCode;
	m_pCM->GetControlMgrData(_T("코드"),strIndCode);
	
	if(strIndCode.IsEmpty())
	{
		AfxMessageBox(_T("지시기호 코드가 입력되지 않았습니다!"));
		return;
	}
	
	CString strDescrip;
	m_pCM->GetControlMgrData(_T("설명"),strDescrip);


	//다른 기본값 지시기호에 기본값 설정이 되어 있는지 check한다
	if(m_chkDefaultYN)
	{
		if(CheckAnotherDefault()<0) return;
	}


	INT ids = 0;
	ids = CheckillegalSign(strIndCode);
	if(ids<0) return;
	
	ids = CheckillegalSign(strDescrip);
	if(ids<0) return;

	if(SaveAllData()<0)
	{
		AfxMessageBox(_T("저장에 실패하였습니다!"));
		return;
	}
	else
	{
		AfxMessageBox(_T("저장하였습니다!"));
	}

	RefreshParent();

	CDialog::OnOK();
}
	

INT CIndicatorDlg::CheckillegalSign(CString strData)
{
	INT pos = strData.Find(_T("*"));
	if(pos<0) return 0;
	
	AfxMessageBox(_T("* 기호는 사용할 수 없습니다. 확인 해 주십시요!"));

	return -1;
}

INT CIndicatorDlg::SaveAllData()
{

	const INT cnt = 2;
	
	CString strCMAlias[cnt] = {
		_T("코드"),
		_T("설명")};

	CString strDMAlias[cnt] = {
		_T("IND_CODE"),
		_T("DESCRIP")};


	if(m_nOpenMode>0)
	{
		m_pDM->FreeData();
		m_pDM->InsertRow(-1);
		
		CString strPK;
		m_pDM->MakeRecKey(strPK);
		m_pDM->SetCellData(_T("PK"),strPK,0);
		m_pDM->SetCellData(_T("MARC_TAG_ALIAS"),m_strMARC_TAG_ALIAS,0);
		m_pDM->SetCellData(_T("CREATE_DATE"),GetTodayDate(),0);
	}
	else
	{
		m_pDM->SetCellData(_T("MODIFY_DATE"),GetTodayDate(),0);
	}
		

	CString strData;

	for(INT i=0;i<cnt;i++)
	{
		m_pCM->GetControlMgrData(strCMAlias[i],strData);
		m_pDM->SetCellData(strDMAlias[i],strData,0);
	}
	
	//기본값 
	if(m_chkDefaultYN)
	{
		m_pDM->SetCellData(_T("DEFAULT_YN"),_T("V"),0);
	}
	else
	{
		m_pDM->SetCellData(_T("DEFAULT_YN"),_T(""),0);
	}


	CArray<CString,CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	m_pDM->StartFrame();

	if(m_nOpenMode>0)
	{
		if(0 == m_radOrder)
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_FIRST_INDICATOR_TBL"), 0 , m_pDM);
		else
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_SECOND_INDICATOR_TBL"), 0 , m_pDM);
	}
	else
	{
		RemoveAliasList.Add(_T("PK"));
		
		if(0 == m_radOrder)
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_FIRST_INDICATOR_TBL"), 0, m_pDM, _T("PK"), _T("NUMERIC"), m_strPK);
		else
			m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_SECOND_INDICATOR_TBL"), 0, m_pDM, _T("PK"), _T("NUMERIC"), m_strPK);
	
	}
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	
	return ids;
}


INT CIndicatorDlg::RefreshParent()
{
	
	if(m_nOpenMode>0)
	{
		if(0 == m_radOrder)
		{
			m_pParentDM1->InsertRow(-1);
			m_nCurrentParentIdx = m_pParentDM1->GetRowCount()-1;
		
		}
		else
		{
			m_pParentDM2->InsertRow(-1);
			m_nCurrentParentIdx = m_pParentDM2->GetRowCount()-1;
		}
	
	}
	
	if(0 == m_radOrder)
		CopyDMToDMByAlias(m_pDM,0,m_pParentDM1,m_nCurrentParentIdx);
	else 
		CopyDMToDMByAlias(m_pDM,0,m_pParentDM2,m_nCurrentParentIdx);

	if(0 == m_radOrder)
		m_pParentGrid1->DisplayLine(m_nCurrentParentIdx);
	else 
		m_pParentGrid2->DisplayLine(m_nCurrentParentIdx);

	return 0;
}

INT CIndicatorDlg::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
{
	if (!pDM_SOURCE || !pDM_TARGET) return -1;

	INT nSourceColumnCount = pDM_SOURCE->GetRefColumnCount();
	CString field_alias, field_name, field_type, data_type, strData;
	INT ids;
	for (INT col = 0; col < nSourceColumnCount; col++) {
		strData = _T("");
		ids = pDM_SOURCE->FindColumnFieldName(col, field_alias, field_name, field_type, data_type, SourceRowIdx, strData);
		ids = pDM_TARGET->SetCellData(field_alias, strData, TargetRowIdx);
	}
	
	return 0;
}

CString CIndicatorDlg::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CIndicatorDlg::CheckAnotherDefault()
{
	INT nRowCnt = -1;
	BOOL bUseDefault = FALSE;
	CString strDefault;

	if(0 == m_radOrder)
	{
		nRowCnt = m_pParentDM1->GetRowCount();
		for(INT i=0;i<nRowCnt;i++)
		{
			if(m_nOpenMode<0 && i == m_nCurrentParentIdx) continue;

			strDefault = m_pParentDM1->GetCellData(_T("DEFAULT_YN"),i);
			if(_T("V") == strDefault) bUseDefault = TRUE;

		}
		
	}
	else
	{
		nRowCnt = m_pParentDM2->GetRowCount();
		for(INT i=0;i<nRowCnt;i++)
		{
			if(m_nOpenMode<0 && i == m_nCurrentParentIdx) continue;

			strDefault = m_pParentDM2->GetCellData(_T("DEFAULT_YN"),i);
			if(_T("V") == strDefault) bUseDefault = TRUE;

		}
	}


	if(bUseDefault) {
		AfxMessageBox(_T("다른 지시기호가 이미 기본 값으로 설정되어 있습니다!\n")
					  _T("먼저 다른 지시기호의 기본값 설정을 해지해 주십시요!"));
		return -1;
	}

	return 0;
}