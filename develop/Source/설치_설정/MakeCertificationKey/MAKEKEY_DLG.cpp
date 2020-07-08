// MAKEKEY_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "MakeCertificationKey.h"
#include "MAKEKEY_DLG.h"

#include "..\\..\\ESL\\CHECK_CERTIFICATION\\CERTIFICATION_Mgr.h"

#include "MakeCertificationKeyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMAKEKEY_DLG dialog


CMAKEKEY_DLG::CMAKEKEY_DLG(CWnd* pParent /*=NULL*/)
	: CDialog(CMAKEKEY_DLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMAKEKEY_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bINSERT = TRUE ;
	m_strRecKey = _T("") ;
}


VOID CMAKEKEY_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMAKEKEY_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMAKEKEY_DLG, CDialog)
	//{{AFX_MSG_MAP(CMAKEKEY_DLG)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMAKEKEY_DLG message handlers

VOID CMAKEKEY_DLG::OnOK() 
{
	// TODO: Add extra validation here
	

	//1.GetData
	GetControlData () ;

	//2.암호화 

	m_strCERTIFICATION_KEY = MakeCertificationKey(m_strCOMPUTER_ID) ;
	
	INT nLens = m_strCERTIFICATION_KEY.GetLength () ;
    
	nLens = m_strCERTIFICATION_KEY.GetLength () ;
	
	//3.복호화 
	CString strTemp = MakeDESKey(m_strCERTIFICATION_KEY) ;
  

	CWnd *pWnd = (CWnd*)GetDlgItem (IDC_EDIT_CERTIFICATION_KEY) ;
	pWnd->SetWindowText (m_strCERTIFICATION_KEY) ;

	nLens = strTemp.GetLength () ;
	nLens = m_strCERTIFICATION_KEY.GetLength () ;
	if(m_strCOMPUTER_ID != strTemp) 
	{
		AfxMessageBox (_T("인증키가 정상적이지 않습니다.")) ;
		return ;
	}
	else 
	{
		CString strMsg ;
		strMsg.Format (_T("생성된 인증키를 설치자에게 전달하여 주십시요.")) ;
	}

	strTemp.Empty () ;
	pWnd->GetWindowText (strTemp);


	//CDialog::OnOK();
}

VOID CMAKEKEY_DLG::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

bool CMAKEKEY_DLG::SetLibInfoData()
{


	return TRUE ;
}

VOID CMAKEKEY_DLG::GetControlData()
{

	CWnd *pLibCode = (CWnd*)GetDlgItem (IDC_EDIT_LIBCODE) ;
	if(pLibCode)
	{
		pLibCode->GetWindowText (m_strLIBCODE ) ;
	}

	CWnd *pComID = (CWnd*)GetDlgItem (IDC_EDIT_ID) ;
	if(pComID)
	{
		pComID->GetWindowText (m_strCOMPUTER_ID  ) ;
	}

	CWnd *pRegister = (CWnd*)GetDlgItem (IDC_EDIT_REGISTER) ;
	if(pRegister)
	{
		pRegister ->GetWindowText (m_strREGISTER ) ;
	}

	CWnd *pCertification = (CWnd*)GetDlgItem (IDC_EDIT_CERTIFICATION_KEY) ;
	if(pCertification)
	{
		pCertification ->GetWindowText (m_strCERTIFICATION_KEY  ) ;
	}


}

CString CMAKEKEY_DLG::MakeCertificationKey(CString strData)
{
	CString strResult = _T("");

	if(strData == _T(""))
	{
		return _T("") ;
	}

	TCHAR *szData ;
	INT nLen = strData.GetLength () ;
	szData = (TCHAR*)malloc(sizeof(TCHAR)*nLen) ;

	_tcscpy(szData,strData.GetBuffer (0)) ;

	nLen = _tcsclen(szData) ;


	CCERTIFICATION_Mgr *pCertiMgr = new CCERTIFICATION_Mgr ;
    strResult = pCertiMgr->KEY_EnCoding (szData) ;

    delete pCertiMgr ;
	pCertiMgr = NULL ;

	//free(szData) ;

    nLen = strResult.GetLength () ;

	return strResult;
}

CString CMAKEKEY_DLG::MakeDESKey(CString strKey)
{
	CString strResult ;

	CCERTIFICATION_Mgr *pCertiMgr = new CCERTIFICATION_Mgr ;
    strResult = pCertiMgr->KEY_DeCoding (strKey.GetBuffer (0)) ;

    delete pCertiMgr ;
	pCertiMgr = NULL ;

	return strResult ;
}


VOID CMAKEKEY_DLG::OnbSAVE() 
{
	// TODO: Add your control notification handler code here
	
	//3.DB 저장
    GetControlData () ;

   
	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (_T("SMARTU@SMARTU/SMARTU#[]")) ;
	dm.m_dbkind = 1 ;

	

	if(m_strCERTIFICATION_KEY == _T("")) 
	{
		AfxMessageBox (_T("인증키를 생성하여 주십시요.")) ;
		return ;
	}


	CString strRecKey;
	CString strDate ;


	if(m_bINSERT == TRUE)
	{ 
		strRecKey=_T("") ;
		INT ids = dm.MakeRecKey (strRecKey) ;
		if(ids < 0 ) 
		{
			AfxMessageBox (_T("REC_KEY 확인 필요")) ;
			return ;
		}
		
		CTime t = CTime::GetCurrentTime() ;
		strDate.Format (_T("%04d/%02d/%02d"),t.GetYear () ,t.GetMonth () ,t.GetDay ()) ;
		
		CString strQuery ;
		strQuery.Format(_T("INSERT INTO K2L_CERTIFICATION_TBL (REC_KEY,LIB_CODE,COMPUTER_ID,REGISTER,CERTIFICATION_KEY,FIRST_DATE,LAST_DATE )")
			_T("VALUES (%s,'%s','%s','%s','%s','%s','%s') ;")
			,strRecKey,m_strLIBCODE,m_strCOMPUTER_ID,m_strREGISTER,m_strCERTIFICATION_KEY,strDate,strDate ) ;
		
		dm.StartFrame () ;
		dm.AddFrame (strQuery) ;
		dm.SendFrame () ;
		ids = dm.EndFrame () ;
		if( ids < 0)
		{
			AfxMessageBox(_T("인증 정보 입력 실패")) ;
			return ;
		}
		
		AfxMessageBox(_T("인증키 등록 성공")) ;
		
	}
	else
	{
	
		INT ids ;
		CTime t = CTime::GetCurrentTime() ;
		strDate.Format (_T("%04d/%02d/%02d"),t.GetYear () ,t.GetMonth () ,t.GetDay ()) ;
		
		CString strQuery ;
		//strQuery.Format(_T("INSERT INTO K2L_CERTIFICATION_TBL (REC_KEY,LIB_CODE,COMPUTER_ID,REGISTER,CERTIFICATION_KEY,FIRST_DATE,LAST_DATE )")
		//	_T("VALUES (%s,'%s','%s','%s','%s','%s','%s') ;")
		//	,strRecKey,m_strLIBCODE,m_strCOMPUTER_ID,m_strREGISTER,m_strCERTIFICATION_KEY,strDate,strDate ) ;
		
		strQuery.Format (_T(" UPDATE K2L_CERTIFICATION_TBL SET ")
			             _T(" LIB_CODE = '%s' , COMPUTER_ID = '%s' , REGISTER = '%s' , CERTIFICATION_KEY = '%s' , LAST_DATE = '%s' where rec_key = %s ;")
						 ,m_strLIBCODE,m_strCOMPUTER_ID,m_strREGISTER,m_strCERTIFICATION_KEY,strDate,m_strRecKey ) ;
		
	
		dm.StartFrame () ;
		dm.AddFrame (strQuery) ;
		dm.SendFrame () ;
		ids = dm.EndFrame () ;
		if( ids < 0)
		{
			AfxMessageBox(_T("인증 정보 수정 실패")) ;
			return ;
		}
		
		AfxMessageBox(_T("인증키 등록 성공")) ;

	}

	CMakeCertificationKeyDlg* pDLG = (CMakeCertificationKeyDlg*)GetParent () ;
	pDLG->SearchFunc () ;

	



}

VOID CMAKEKEY_DLG::SetModifyScreen(bool bInsert)
{
	m_bINSERT = bInsert ;
}

BOOL CMAKEKEY_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	CWnd *pButton = (CButton*)GetDlgItem (IDC_bSAVE) ;


	if(m_bINSERT == TRUE )
	{
		pButton->SetWindowText (_T("입 력")) ;
		return TRUE ;
	}
	
	SetModifyControlData() ;

	pButton->SetWindowText (_T("수 정")) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMAKEKEY_DLG::SetModifyControlData()
{
	if(m_strRecKey == _T("")) 
	{
		AfxMessageBox(_T("정보를 구성하지 못하여 습니다.")) ;
		return ;
	}

	CESL_DataMgr dm ;
	dm.SetCONNECTION_INFO (_T("SMARTU@SMARTU/SMARTU#[]")) ;
	dm.m_dbkind = 1 ;

	CString strQuery ;      //  0      1          2           3        4 
	strQuery.Format (_T(" SELECT REC_KEY,LIB_CODE,COMPUTER_ID,REGISTER,CERTIFICATION_KEY FROM K2L_CERTIFICATION_TBL ") 
					 _T(" WHERE REC_KEY = %s "),m_strRecKey ) ;

	dm.RestructDataManager (strQuery) ;

	m_strLIBCODE ;
	m_strCOMPUTER_ID ;
	m_strREGISTER ;
	m_strCERTIFICATION_KEY ;

	CWnd *pLibCode = (CWnd*)GetDlgItem (IDC_EDIT_LIBCODE) ;
	if(pLibCode)
	{
		pLibCode->SetWindowText  (dm.GetCellData (0,1) ) ;
	}

	CWnd *pComID = (CWnd*)GetDlgItem (IDC_EDIT_ID) ;
	if(pComID)
	{
		pComID->SetWindowText  (dm.GetCellData (0,2)  ) ;
	}

	CWnd *pRegister = (CWnd*)GetDlgItem (IDC_EDIT_REGISTER) ;
	if(pRegister)
	{
		pRegister ->SetWindowText  (dm.GetCellData (0,3) ) ;
	}


	CWnd *pWnd = (CWnd*)GetDlgItem (IDC_EDIT_CERTIFICATION_KEY) ;
	pWnd->SetWindowText (dm.GetCellData (0,4)) ;

    

}

VOID CMAKEKEY_DLG::SetRecKey(CString strRecKey)
{
	m_strRecKey = strRecKey ;
}
