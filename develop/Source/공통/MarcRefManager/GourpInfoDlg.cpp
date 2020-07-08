// GourpInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GourpInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGourpInfoDlg dialog


CGourpInfoDlg::CGourpInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CGourpInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;
	m_pDM = NULL;
	m_nOpenMode = 1; //default
	m_strPK = _T("");
}


VOID CGourpInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGourpInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGourpInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CGourpInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGourpInfoDlg message handlers

BOOL CGourpInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(InitESL_Mgr( _T("마크레프_그룹정보") )<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 마크레프_그룹정보") );
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_마크레프_그룹정보"));
	m_pDM = FindDM(_T("DM_마크레프_그룹정보"));

	if(!m_pCM)
	{
		AfxMessageBox( _T("reference Error : CM_마크레프_그룹정보") );
		return FALSE;		
	}

	if(InitOpenMode()<0) return FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CGourpInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	CString strGroupCode;
	m_pCM->GetControlMgrData(_T("그룹코드"),strGroupCode);
	
	if(strGroupCode.IsEmpty())
	{
		AfxMessageBox(_T("그룹코드가 입력되지 않았습니다!"));
		return;
	}

	CString strDescrip;
	m_pCM->GetControlMgrData(_T("설명"),strDescrip);

	if(m_nOpenMode>0)
	{
		CString strResult;

		CString strQuery;
		strQuery.Format(_T("SELECT COUNT(*) FROM MARC_GROUP_TBL WHERE MARC_GROUP_CODE = '%s'"),strGroupCode);
    
		m_pDM->GetOneValueQuery(strQuery,strResult);

		if(_ttoi(strResult.GetBuffer(0)) > 0)
		{
			AfxMessageBox(_T("해당 그룹코드가 이미 존재합니다!"));
			return;
		}
   	}
	

	INT ids = 0;
	ids = CheckillegalSign(strGroupCode);
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

	CDialog::OnOK();
}

INT CGourpInfoDlg::InitOpenMode()
{
	if(m_nOpenMode<0)
	{
		CEdit* pEdit = (CEdit*)m_pCM->FindControl(_T("그룹코드"));
		if(pEdit) pEdit->SetReadOnly();

		if(m_strPK.IsEmpty())
		{
			AfxMessageBox(_T("해당 PK를 찾을 수 없습니다!"));
			return -1;	
		}	

		CString strWhere;
		strWhere.Format(_T("PK = %s"),m_strPK);
		m_pDM->RefreshDataManager(strWhere);
		
		if(0 == m_pDM->GetRowCount())
		{
			AfxMessageBox(_T("해당 그룹정보를 찾을 수 없습니다!"));
			return -1;	
		}	
		
		m_pCM->AllControlDisplay(0);
	}

	return 0;
}

INT CGourpInfoDlg::CheckillegalSign(CString strData)
{
	INT pos = strData.Find(_T("*"));
	if(pos<0) return 0;
	
	AfxMessageBox(_T("* 기호는 사용할 수 없습니다. 확인 해 주십시요!"));

	return -1;
}


CString CGourpInfoDlg::GetTodayDate()
{
	CString result;
	CTime t = CTime::GetCurrentTime();
	result.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	return result;
}

INT CGourpInfoDlg::SaveAllData()
{
	
	const INT cnt = 2;
	
	CString strCMAlias[cnt] = {
		_T("그룹코드"),
		_T("설명")};


	CString strDMAlias[cnt] = {
		_T("MARC_GROUP_CODE"),
		_T("DESCRIP")};
	
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
	
	CArray<CString,CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	m_pDM->StartFrame();

	if(m_nOpenMode>0)
	{
		m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_GROUP_TBL"), 0 , m_pDM);
	}
	else
	{
		RemoveAliasList.Add(_T("PK"));
		m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, _T("MARC_GROUP_TBL"), 0, m_pDM, _T("PK"), _T("NUMERIC"), m_strPK);
	}
	
	INT ids = m_pDM->SendFrame();
	m_pDM->EndFrame();

	return ids;
}
