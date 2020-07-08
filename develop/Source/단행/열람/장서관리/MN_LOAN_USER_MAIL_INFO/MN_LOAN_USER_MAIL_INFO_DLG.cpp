// MN_LOAN_USER_MAIL_INFO_DLG.cpp : implementation file
//

#include "stdafx.h"
#include "MN_LOAN_USER_MAIL_INFO_DLG.h"
// 2011.09.07 ADD : 3000Api
#include "BO_LOC_3000Api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMN_LOAN_USER_MAIL_INFO_DLG dialog


CMN_LOAN_USER_MAIL_INFO_DLG::CMN_LOAN_USER_MAIL_INFO_DLG(CESL_Mgr* pParent , CString sAliasKey )
	: CPreNextDlg( pParent , IDD )
{
	//{{AFX_DATA_INIT(CMN_LOAN_USER_MAIL_INFO_DLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_sAliasKey = sAliasKey;
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CMN_LOAN_USER_MAIL_INFO_DLG::~CMN_LOAN_USER_MAIL_INFO_DLG()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CMN_LOAN_USER_MAIL_INFO_DLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMN_LOAN_USER_MAIL_INFO_DLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMN_LOAN_USER_MAIL_INFO_DLG, CDialog)
	//{{AFX_MSG_MAP(CMN_LOAN_USER_MAIL_INFO_DLG)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_edtMLUMI_CLOSE, OnedtMLUMICLOSE)
	ON_BN_CLICKED(IDC_btnMLUMI_PREV, OnbtnMLUMIPREV)
	ON_BN_CLICKED(IDC_btnMLUMI_NEXT, OnbtnMLUMINEXT)
	ON_BN_CLICKED(IDC_btnMLUMI_MODIFY, OnbtnMLUMIMODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMN_LOAN_USER_MAIL_INFO_DLG message handlers

BOOL CMN_LOAN_USER_MAIL_INFO_DLG::OnInitDialog() 
{
EFS_BEGIN

	CPreNextDlg::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr(_T("MN_LOAN_USER_MAIL_INFO_DLG"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );
	
	ViewScreen();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END

	return FALSE;
}



INT CMN_LOAN_USER_MAIL_INFO_DLG::ViewScreen()
{
EFS_BEGIN


	INT ids;

	if( m_nCurrentIndex < 0 ) return 1;

	CESL_DataMgr* pParentDM = FindDM( m_sParentDMAlias );
	if( pParentDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ViewScreen") );

	CString sEMail;
	ids = pParentDM->GetCellData( m_sAliasKey , m_nCurrentIndex , sEMail );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ViewScreen") );

	if( sEMail.GetAt(0) == '<' ) 
	{
		sEMail = sEMail.Mid(1);
	}

	if( sEMail.GetAt(sEMail.GetLength()-1) == '>' )
	{
		sEMail = sEMail.Mid(0,sEMail.GetLength()-1);
	}


	if( sEMail.IsEmpty() ) 
	{
		AfxMessageBox( _T("E_MAIL이 존재하지 않아 이용자를 검색할수 없습니다.") );
		return 1;
	}
	else
	{
		CESL_DataMgr* pDM = FindDM( _T("DM_MN_LOAN_USER_MAIL_INFO_DLG") );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ViewScreen") );

		// 2011.09.07 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse)
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					CStringArray DataArray;
					DataArray.Add(sEMail);
					if(TRUE == pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), DataArray))
					{
						sEMail = DataArray.GetAt(0);
					}
					DataArray.RemoveAll(); DataArray.FreeExtra();
					pApi->CloseSocket();
				}
				delete pApi;
				pApi = NULL;
			}
		}

		CString sWhere;
		sWhere.Format( _T("CLU.E_MAIL='%s'") , sEMail );
		ids = pDM->RefreshDataManager( sWhere );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ViewScreen") );

		// 2011.09.07 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse && 0 < pDM->GetRowCount())
		{
			CString strEnc;
			CStringArray asDes1,asDes2,asDes3,asDes4,asDes5;
			for(INT i = 0; i < pDM->GetRowCount(); i++)
			{
				pDM->GetCellData(_T("E_MAIL"), i, strEnc);
				asDes1.Add(strEnc);
				pDM->GetCellData(_T("핸드폰번호"), i, strEnc);
				asDes2.Add(strEnc);
				pDM->GetCellData(_T("집전화번호"), i, strEnc);
				asDes3.Add(strEnc);
				pDM->GetCellData(_T("직장전화번호"), i, strEnc);
				asDes4.Add(strEnc);
				pDM->GetCellData(_T("집주소"), i, strEnc);
				asDes5.Add(strEnc);
			}
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->CheckConnection())
				{
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), asDes1);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("HANDPHONE"), asDes2);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_PHONE"), asDes3);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("W_PHONE"), asDes4);
						pApi->CloseSocket();	
					}
					if(TRUE == pApi->ConnectSocket()) 
					{
						pApi->UserInfoDecrytion(_T("CO_LOAN_USER_TBL"), _T("H_ADDR1"), asDes5);
						pApi->CloseSocket();	
					}
					for(i = 0; i < pDM->GetRowCount(); i++)
					{
						pDM->SetCellData(_T("E_MAIL"), asDes1.GetAt(i), i);
						pDM->SetCellData(_T("핸드폰번호"), asDes2.GetAt(i), i);
						pDM->SetCellData(_T("집전화번호"), asDes3.GetAt(i), i);
						pDM->SetCellData(_T("직장전화번호"), asDes4.GetAt(i), i);
						pDM->SetCellData(_T("집주소"), asDes5.GetAt(i), i);
					}
				}
				delete pApi;
				pApi = NULL;
			}
			asDes1.RemoveAll(); asDes1.FreeExtra();
			asDes2.RemoveAll(); asDes2.FreeExtra();
			asDes3.RemoveAll(); asDes3.FreeExtra();
			asDes4.RemoveAll(); asDes4.FreeExtra();
			asDes5.RemoveAll(); asDes5.FreeExtra();
		}
	}


	ids = AllControlDisplay(_T("CM_MN_LOAN_USER_MAIL_INFO_DLG") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ViewScreen") );

	return 0;



EFS_END
return -1;

}





VOID CMN_LOAN_USER_MAIL_INFO_DLG::OnbtnMLUMIPREV() 
{
EFS_BEGIN

	CPreNextDlg::MovePrev();
	ViewScreen();
	

EFS_END
}

VOID CMN_LOAN_USER_MAIL_INFO_DLG::OnbtnMLUMINEXT() 
{
EFS_BEGIN

	CPreNextDlg::MoveNext();
	ViewScreen();
	

EFS_END
}

VOID CMN_LOAN_USER_MAIL_INFO_DLG::OnedtMLUMICLOSE() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}

VOID CMN_LOAN_USER_MAIL_INFO_DLG::OnbtnMLUMIMODIFY() 
{
EFS_BEGIN

	AfxGetApp()->DoWaitCursor(-1);

	ModifyEMail();

	AfxGetApp()->DoWaitCursor(1);
EFS_END	
}


INT CMN_LOAN_USER_MAIL_INFO_DLG::ModifyEMail()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_MN_LOAN_USER_MAIL_INFO_DLG") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ModifyEMail") ); 

	/// 그리드에 선정된 이용자가 존재하는지 알아본다.
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_MN_LOAN_USER_MAIL_INFO_DLG") , _T("이용자그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ModifyEMail") );

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("수정할 이용자가 존재하지 않습니다.") );
		return 1;
	}

	/// 그리드에 선정된 이용자를 UPDATE한다.
	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("수정할 이용자를 선정해주십시오.") );
		return 2;
	}

	pGrid->SelectGetHeadPosition();


	INT nIndex;
	CString sRecKey;
	CString sEMail;
	CArray <INT,INT> saSIndexs;

	GetDlgItem(IDC_edtMLUMI_EMAIL)->GetWindowText(sEMail);

	pDM->StartFrame();

	CString strData = sEMail;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		saSIndexs.Add(nIndex);

		ids = pDM->GetCellData( _T("이용자KEY") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ModifyEMail") );

		pDM->InitDBFieldData();

		// 2011.09.07 ADD : 3000Api
		if(TRUE == m_pInfo->m_bLibSecuUse && !strData.IsEmpty())
		{
			CBO_LOC_3000Api* pApi = new CBO_LOC_3000Api(this);
			if(NULL != pApi)
			{
				if(TRUE == pApi->ConnectSocket()) 
				{
					CStringArray DataArray;
					DataArray.Add(strData);
					if(TRUE == pApi->UserInfoEncrytion(_T("CO_LOAN_USER_TBL"), _T("E_MAIL"), DataArray))
					{
						strData = DataArray.GetAt(0);
					}
					DataArray.RemoveAll(); DataArray.FreeExtra();
					pApi->CloseSocket();
				}
				delete pApi;
				pApi = NULL;
			}
		}

		pDM->AddDBFieldData( _T("E_MAIL") , _T("STRING") , strData );
		ids = pDM->MakeUpdateFrame( _T("CO_LOAN_USER_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ModifyEMail") );
	}

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ModifyEMail") );

	pDM->EndFrame();

	/// DM UPdate한다.
	/// 그리드에 보여준다.
	for( i = 0 ; i < saSIndexs.GetSize() ; i++ )
	{
		ids = pDM->SetCellData( _T("E_MAIL") , sEMail, saSIndexs.GetAt(i)  );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ModifyEMail") );

		pGrid->DisplayLine( saSIndexs.GetAt(i) );
	}
	
	AfxMessageBox( _T("수정되었습니다.") );

	return 0;
EFS_END

return -1;
}
HBRUSH CMN_LOAN_USER_MAIL_INFO_DLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}