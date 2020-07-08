// TagDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TagDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTagDlg dialog


CTagDlg::CTagDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTagDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_pParentDM = NULL;
	m_pParentGrid = NULL;

	m_nOpenMode = 1; //default
	m_strMarcGroup = _T("");
	m_strPK = _T("");

	m_nCurrentParentIdx = -1;
}


VOID CTagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTagDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTagDlg, CDialog)
	//{{AFX_MSG_MAP(CTagDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTagDlg message handlers

BOOL CTagDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("마크레프_태그") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 마크레프_태그") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_마크레프_태그"));
	m_pDM = FindDM(_T("DM_마크레프_태그"));

	if(!m_pCM || !m_pDM)
	{
		AfxMessageBox( _T("reference Error : 마크레프_태그") );
		return FALSE;		
	}

	if(InitOpenMode()<0)
	{
		AfxMessageBox( _T("태그 정보 초기화에 실패!") );
		return FALSE;		
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CTagDlg::InitOpenMode()
{

	if(m_nOpenMode>0) //입력
	{
		if(!m_pParentDM || !m_pParentGrid)
		{
			AfxMessageBox( _T("부모 정보를 찾을 수 없습니다!") );
			return -1;		
		}
		
		
		if(m_strMarcGroup.IsEmpty())
		{
			AfxMessageBox( _T("그룹코드를 찾을 수 없습니다!") );
			return -1;		
		}

		m_pCM->SetControlMgrData(_T("그룹코드"),m_strMarcGroup);
	}
	else //수정 
	{

		if(!m_pParentDM || !m_pParentGrid || m_nCurrentParentIdx<0)
		{
			AfxMessageBox( _T("부모 정보를 찾을 수 없습니다!") );
			return -1;		
		}
		
		if(m_strPK.IsEmpty())
		{
			AfxMessageBox( _T("PK를 찾을 수 없습니다!") );
			return -1;		
		}	
		
		CString strWhere;
		strWhere.Format(_T("PK = %s"),m_strPK);
		m_pDM->RefreshDataManager(strWhere);

		INT nRow = m_pDM->GetRowCount();

		if(0 == nRow) 
		{
			AfxMessageBox( _T("해당 PK의 데이터가 없습니다!") );
			return -1;					
		}

		m_pCM->AllControlDisplay(0);
		
		
		//태그 코드는 수정하지 못하게 
		CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("태그코드"));
		if(pEdit) pEdit->SetReadOnly();


		//반복여부, 필수여부
		CString strRepeat,strEssential;
		
		strRepeat = m_pDM->GetCellData(_T("REPEAT_TYPE"),0);
		strEssential = m_pDM->GetCellData(_T("ESSENTIAL_TYPE"),0);
		
		CComboBox* pCombo = (CComboBox*)m_pCM->FindControl(_T("반복여부"));

		if(pCombo)
		{
			if(_T("Y") == strRepeat)
			{
				pCombo->SetCurSel(0);
			}
			else 
			{
				pCombo->SetCurSel(1);
			}

		}

		pCombo = (CComboBox*)m_pCM->FindControl(_T("필수여부"));

		if(pCombo)
		{
			if(_T("M") == strEssential)
			{
				pCombo->SetCurSel(0);
			}
			else if(_T("A") == strEssential)
			{
				pCombo->SetCurSel(1);
			}
			else if(_T("O") == strEssential)
			{
				pCombo->SetCurSel(2);
			}
			else if(_T("B") == strEssential)
			{
				pCombo->SetCurSel(3);
			}

		}
	}

	return 0;
}

VOID CTagDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CString strTagCode;
	m_pCM->GetControlMgrData(_T("태그코드"),strTagCode);
	
	if(strTagCode.IsEmpty())
	{
		AfxMessageBox(_T("태그 코드가 입력되지 않았습니다!"));
		return;
	}
	
	CString strTagName;
	m_pCM->GetControlMgrData(_T("이름"),strTagName);

	if(strTagName.IsEmpty())
	{
		AfxMessageBox(_T("태그 이름이 입력되지 않았습니다!"));
		return;
	}

	if(m_nOpenMode>0)
	{
		
		CString strMarcGroup;
		CString strResult;
		strMarcGroup = m_pDM->GetCellData(_T("그룹코드"),0);


		CString strQuery;
		strQuery.Format(_T("SELECT COUNT(*) FROM MARC_TAG_TBL WHERE MARC_GROUP_CODE = '%s' ")
					 _T("AND TAG_CODE = '%s'"),strMarcGroup,strTagCode);
    
		m_pDM->GetOneValueQuery(strQuery,strResult);

		if(_ttoi(strResult.GetBuffer(0)) > 0)
		{
			AfxMessageBox(_T("해당 그룹내에 동일한 코드가 있어서 입력 할 수 없습니다!"));
			return;
		}
   	}
	
	CString strDescrip,strExample;
	m_pCM->GetControlMgrData(_T("설명"),strDescrip);
	m_pCM->GetControlMgrData(_T("예시"),strExample);


	INT ids = 0;
	ids = CheckillegalSign(strTagCode);
	if(ids<0) return;
	
	ids = CheckillegalSign(strTagName);
	if(ids<0) return;

	ids = CheckillegalSign(strDescrip);
	if(ids<0) return;
	
	ids = CheckillegalSign(strExample);
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

INT CTagDlg::CheckillegalSign(CString strData)
{
	INT pos = strData.Find(_T("*"));
	if(pos<0) return 0;
	
	AfxMessageBox(_T("* 기호는 사용할 수 없습니다. 확인 해 주십시요!"));

	return -1;
}

INT CTagDlg::SaveAllData()
{
	const INT cnt = 5;
	
	CString strCMAlias[cnt] = {
		_T("그룹코드"),
		_T("설명"),
		_T("예시"),
		_T("이름"),
		_T("태그코드")};

	CString strDMAlias[cnt] = {
		_T("MARC_GROUP_CODE"),
		_T("DESCRIP"),
		_T("EXAMPLE"),
		_T("TAG_NAME"),
		_T("TAG_CODE")};


	if(m_nOpenMode>0)
	{
		m_pDM->FreeData();
		m_pDM->InsertRow(-1);
	
		CString strPK;
		m_pDM->MakeRecKey(strPK);
		m_pDM->SetCellData(_T("PK"),strPK,0);
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
	
	
	//ALIAS
	CString strAlias,strGroupCode,strTagCode;
	strGroupCode = m_pDM->GetCellData(_T("MARC_GROUP_CODE"),0);
	strTagCode = m_pDM->GetCellData(_T("TAG_CODE"),0);
	strAlias.Format(_T("%s_%S"),strGroupCode,strTagCode);
	m_pDM->SetCellData(_T("ALIAS"),strAlias,0);


	//반복여부 
	CString strText;
	CComboBox* pCombo = (CComboBox*)m_pCM->FindControl(_T("반복여부"));
	pCombo->GetWindowText(strText);

	if(_T("반복") == strText)
	{
		strData = _T("Y");
	}
	else
	{
		strData = _T("N");
	}

	m_pDM->SetCellData(_T("REPEAT_TYPE"),strData,0);

	//필수여부
	pCombo = (CComboBox*)m_pCM->FindControl(_T("필수여부"));
	pCombo->GetWindowText(strText);

	if(_T("필수") == strText)
	{
		strData = _T("M");
	}
	else if(_T("해당시필수") == strText)
	{
		strData = _T("A");
	}
	else if(_T("재량") == strText)
	{
		strData = _T("O");
	}
	else if(_T("공백") == strText)
	{
		strData = _T("B");
	}

	m_pDM->SetCellData(_T("ESSENTIAL_TYPE"),strData,0);

	CArray<CString,CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	m_pDM->StartFrame();

	if(m_nOpenMode>0)
	{
		m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_TAG_TBL"), 0 , m_pDM);
	}
	else
	{
		RemoveAliasList.Add(_T("PK"));
		m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_TAG_TBL"), 0, m_pDM, _T("PK"), _T("NUMERIC"), m_strPK);
	}
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();
	
	return ids;
}

INT CTagDlg::RefreshParent()
{
	if(m_nOpenMode>0)
	{
		m_pParentDM->InsertRow(-1);
		m_nCurrentParentIdx = m_pParentDM->GetRowCount()-1;
	}

	CopyDMToDMByAlias(m_pDM,0,m_pParentDM,m_nCurrentParentIdx);

	if(m_pParentGrid)
	{
		m_pParentGrid->DisplayLine(m_nCurrentParentIdx);
	}
	
	return 0;
}

INT CTagDlg::CopyDMToDMByAlias(CESL_DataMgr *pDM_SOURCE, INT SourceRowIdx, CESL_DataMgr *pDM_TARGET, INT TargetRowIdx)
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


CString CTagDlg::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}