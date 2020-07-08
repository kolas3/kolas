#include "stdafx.h"
#include "SECEDE_USER_DLG.h"

#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSECEDE_USER_DLG::CSECEDE_USER_DLG(BL_LOC_USER_PROC* pSpfWork,CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CSECEDE_USER_DLG::IDD, pParent)
{	
	m_pSpfWork = pSpfWork;
	m_bApply   = FALSE;
}


CSECEDE_USER_DLG::~CSECEDE_USER_DLG()
{
	
}

VOID CSECEDE_USER_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	//{{AFX_DATA_MAP(CSECEDE_USER_DLG)
	//}}AFX_DATA_MAP	
	
}


BEGIN_MESSAGE_MAP(CSECEDE_USER_DLG, CDialog)
	//{{AFX_MSG_MAP(CSECEDE_USER_DLG)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSECEDE_USER_DLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	if(InitESL_Mgr(_T("SM_SECEDE_USER_DLG"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Failed")) ;
		return FALSE ;
	}

	if(ShowSecedeUserData()<0)
	{
		AfxMessageBox(_T("ShoeSecedeUserData() Failed")) ;
		return FALSE ;
	}

	return TRUE;  
	              
}

VOID CSECEDE_USER_DLG::OnBtnApply() 
{
	ApplySecedeInfo();
}

VOID CSECEDE_USER_DLG::SetCivilNo(CString strCivilNo)
{
	m_strCivilNo = strCivilNo;
}

INT CSECEDE_USER_DLG::ShowSecedeUserData()
{	
	INT		ids;
	CString strWhere;

	if( m_strCivilNo.IsEmpty() ) return -2;

	CESL_DataMgr* pDM = FindDM( _T("DM_SECEDE_USER_DLG") );
	if( !pDM ) return -1;

	
	
	CString strCivilNo;
	if(_T("Y")==m_sIsUnityLoanService)
	{
		strCivilNo.Format(_T("%s-%s"), m_strCivilNo.Left(6), m_strCivilNo.Right(7) );
		this->ESL_DES_Encryption(strCivilNo,strCivilNo);
		strWhere.Format( _T(" CLU.CRYPTO_CIVIL_NO='%s' "), strCivilNo );
	}
	else
	{
		strCivilNo = m_strCivilNo;
		
		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CStringArray DataArray;
			DataArray.Add(strCivilNo);
			CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
			if(NULL != p3000Api)
			{
				if(TRUE == p3000Api->ConnectSocket()) 
				{
					if(TRUE == p3000Api->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), DataArray))
					{
						strCivilNo = DataArray.GetAt(0);
					}
					p3000Api->CloseSocket();	
				}
				delete p3000Api;
				p3000Api = NULL;
			}
			DataArray.RemoveAll(); DataArray.FreeExtra();
		}
		strWhere.Format( _T(" CLU.CIVIL_NO='%s' "), strCivilNo );
	}

	ids = pDM->RefreshDataManager( strWhere );
	if( 0 > ids ) return -3;

	
	if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
	{
		CString strGetData;
		CStringArray asDes1;
		for(INT i = 0; i < pDM->GetRowCount(); i++)
		{
			pDM->GetCellData(_T("주민번호"), i, strGetData);
			asDes1.Add(strGetData);
		}
		CBO_LOC_3000Api *p3000Api = new CBO_LOC_3000Api(this);
		if(NULL != p3000Api)
		{
			if(TRUE == p3000Api->ConnectSocket()) 
			{
				p3000Api->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("CIVIL_NO"), asDes1);
				for(i = 0; i < pDM->GetRowCount(); i++)
				{
					pDM->SetCellData(_T("주민번호"), asDes1.GetAt(i), i);
				}
				p3000Api->CloseSocket();	
			}
			delete p3000Api;
			p3000Api = NULL;
		}
		asDes1.RemoveAll(); asDes1.FreeExtra();
	}

	
	ids = m_pSpfWork->UserCodeToDesc( pDM );
	if( 0 > ids ) return -4;

	ids = CLocCommonAPI::ChangeSecuriyCivilNo( this , pDM , -1, _T("주민번호") , m_sIsUnityLoanService );
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CBO_LOC_SECEDE::SecederSearch()") );

	this->AllControlDisplay( _T("CM_SECEDE_USER_DLG") );

	return 0;
}

INT CSECEDE_USER_DLG::ApplySecedeInfo()
{
	INT		ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_SECEDE_USER_DLG") );
	if( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ApplySecedeInfo()") );

	
	CESL_Grid* pGrid = (CESL_Grid*)this->FindControl( _T("CM_SECEDE_USER_DLG"), _T("MainGrid") );
	if( !pGrid ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ApplySecedeInfo()") );

	pGrid->SelectMakeList();
	INT nSelectCount = pGrid->SelectGetCount();

	if( 0 == nSelectCount )
	{
		AfxMessageBox( _T("적용할 탈퇴 이용자를 선정해주십시오."), MB_ICONSTOP );
		return 1;
	}
	else if( 1 < nSelectCount )
	{
		AfxMessageBox( _T("탈퇴 이용자는 반드시 한개의 값만 선정되어야 합니다."), MB_ICONSTOP );
		return 2;
	}

	pGrid->SelectGetHeadPosition();
	INT		nIndex = pGrid->SelectGetIdx();

	
	
	CString strPositionCode, strClassCode, strLoanStopDate, strUserName;
	
	
	ids = pDM->GetCellData( _T("대출직급정보_코드"), nIndex, strClassCode );
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ApplySecedeInfo()") );
	ids = pDM->GetCellData( _T("대출소속정보_코드"), nIndex, strPositionCode );
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ApplySecedeInfo()") );
	ids = pDM->GetCellData( _T("대출정지일"), nIndex, strLoanStopDate );
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ApplySecedeInfo()") );
	ids = pDM->GetCellData( _T("대출자명"), nIndex, strUserName );
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ApplySecedeInfo()") );

	
	ids = this->pParentMgr->SetControlData(_T("CM_BO_LOC_3320"), _T("직급"), strClassCode);
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ApplySecedeInfo()") );
	ids = this->pParentMgr->SetControlData(_T("CM_BO_LOC_3320"), _T("소속"), strPositionCode);
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ApplySecedeInfo()") );
	ids = this->pParentMgr->SetControlData(_T("CM_BO_LOC_3320"), _T("대출정지일"), strLoanStopDate);
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ApplySecedeInfo()") );
	ids = this->pParentMgr->SetControlData(_T("CM_BO_LOC_3320"), _T("이름"), strUserName);
	if( 0>ids ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ApplySecedeInfo()") );

	
	strLoanStopDate.Replace(_T("/"),_T(""));
	if( 8 < strLoanStopDate.GetLength() )
	{
		strLoanStopDate = strLoanStopDate.Mid(0,8);
	}
	
	if( 8 == strLoanStopDate.GetLength() )
	{


		CTime tStopDate( _ttoi( strLoanStopDate.Mid(0,4) ), _ttoi( strLoanStopDate.Mid(4,2) ), _ttoi( strLoanStopDate.Mid(6,2) ), 0, 0, 0 );
		CTime tCurDate = CTime::GetCurrentTime();

		if( _ttoi(tStopDate.Format(_T("%Y%m%d"))) > _ttoi(tCurDate.Format(_T("%Y%m%d"))) )
		{
			CComboBox* pCB = (CComboBox*)FindControl( _T("CM_BO_LOC_3320"), _T("회원상태") );
			if( !pCB ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("ApplySecedeInfo()") );			
			pCB->SetCurSel( 1 );			
		}
	}

	AfxMessageBox( _T("적용되었습니다"), MB_ICONINFORMATION );
	m_bApply = TRUE;
	return 0;
}

HBRUSH CSECEDE_USER_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
