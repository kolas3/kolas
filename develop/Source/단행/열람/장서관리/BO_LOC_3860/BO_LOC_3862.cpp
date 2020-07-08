// BO_LOC_3862.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3862.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3862 dialog


CBO_LOC_3862::CBO_LOC_3862(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3862)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3862::~CBO_LOC_3862()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
	m_Dlg_brush.DeleteObject();
}

VOID CBO_LOC_3862::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3862)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3862, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3862)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btn3862_CHANGE, Onbtn3862CHANGE)
	ON_BN_CLICKED(IDC_btn3862_CLOSE, Onbtn3862CLOSE)
	ON_BN_CLICKED(IDC_btn3862_CHANGE2, Onbtn3862CHANGE2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3862 message handlers

BOOL CBO_LOC_3862::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	m_Dlg_brush.CreateSolidBrush(RGB(239, 239, 239)); 

	INT ids = InitESL_Mgr( _T("BO_LOC_3862") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}



INT CBO_LOC_3862::InitControl()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3862"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitControl") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("그룹")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitControl") );

	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitControl") );

	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("이름") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) continue;
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitControl") );
	}
	
	
	pCMCB->SetCurSel(0);



	return 0;

EFS_END
return -1;

}

VOID CBO_LOC_3862::Onbtn3862CHANGE() 
{
EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);
	
	ChangeGroup(TRUE);

	AfxGetApp()->DoWaitCursor(-1);
	

EFS_END
}

VOID CBO_LOC_3862::Onbtn3862CLOSE() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}

INT CBO_LOC_3862::ChangeGroup(BOOL bIsAdd)
{
EFS_BEGIN

	INT ids;

	/// 선정된 것이 있는 확인..
	CESL_DataMgr* pDM = FindDM(_T("DM_BO_LOC_3860"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeGroup") );

	if( pDM->GetRowCount() == 0 ) 
	{
		AfxMessageBox( _T("이용자를 검색해주십시오.") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3860") , _T("메인그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ChangeGroup") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("변경할 이용자를 선정해주십시오.") );
		return 2;
	}

	CString sGroupCode;
	ids = GetControlData( _T("CM_BO_LOC_3862") , _T("그룹") , sGroupCode , -1 , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ChangeGroup") ); 

	if( sGroupCode.IsEmpty() )
	{
		AfxMessageBox(_T("변경할 그룹을 지정해주십시오."));
		return 3;
	}
	pDM->StartFrame();

	pGrid->SelectGetHeadPosition();

	CArray < INT , INT > aUIndexs;


	INT nIndex;
	CString sRecKey;
	CString sDeleteQuery;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		aUIndexs.Add(nIndex);

		ids = pDM->GetCellData( _T("이용자KEY") , nIndex , sRecKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("ChangeGroup") );

		pDM->InitDBFieldData();
		sDeleteQuery.Format( _T("DELETE FROM MN_LOAN_USER_GROUP_LIST_TBL WHERE USER_KEY=%s AND GROUP_CODE='%s';") , sRecKey , sGroupCode );
		pDM->AddFrame( sDeleteQuery );

		if( bIsAdd )
		{
			pDM->InitDBFieldData();
			pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , _T("ESL_SEQ.NextVal") ); 
			pDM->AddDBFieldData( _T("GROUP_CODE") , _T("STRING") , sGroupCode );
			pDM->AddDBFieldData( _T("USER_KEY") , _T("NUMERIC") , sRecKey  );
			ids = pDM->MakeInsertFrame( _T("MN_LOAN_USER_GROUP_LIST_TBL") );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("ChangeGroup") );
		}
		pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("ChangeGroup") );

	pDM->EndFrame();

	// DM에 반영하기 , 그리드 반영하기
	CString sPreGroupCodes;
	CStringArray asGroupCodeList;
	for( i = 0 ; i < nSelectCnt ; i++ )
	{
		asGroupCodeList.RemoveAll();

		ids = pDM->GetCellData( _T("그룹코드") , aUIndexs.GetAt(i)  , sPreGroupCodes );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("ChangeGroup") );


		sPreGroupCodes.Replace( sGroupCode , _T("") );

		CLocCommonAPI::MakeArrayList( sPreGroupCodes , _T(",") , asGroupCodeList );

		sPreGroupCodes.Empty();

		for( INT k = 0 ; k < asGroupCodeList.GetSize() ; k++ )
		{
			if( asGroupCodeList.GetAt(k).IsEmpty() ) continue;

			if( !sPreGroupCodes.IsEmpty() ) sPreGroupCodes += _T(",");
			sPreGroupCodes += asGroupCodeList.GetAt(k);
		}

		if( bIsAdd )
		{
			if( !sPreGroupCodes.IsEmpty() )
				sPreGroupCodes += _T(",");
			sPreGroupCodes += sGroupCode;
		}

		ids = pDM->SetCellData( _T("그룹코드") , sPreGroupCodes ,aUIndexs.GetAt(i) );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("ChangeGroup") );

		pGrid->DisplayLine( aUIndexs.GetAt(i) );
	}

	AfxMessageBox( _T("변경되었습니다. ") );

	return 0;

EFS_END
return -1;

}

VOID CBO_LOC_3862::Onbtn3862CHANGE2() 
{
	EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);
	
	ChangeGroup(FALSE);

	AfxGetApp()->DoWaitCursor(-1);
	
	EFS_END
}
HBRUSH CBO_LOC_3862::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=NULL;
	if(pWnd->m_hWnd==this->m_hWnd) hbr= HBRUSH(m_Dlg_brush);	
	else hbr =CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetTextColor(RGB(0, 0, 0));		
		pDC->SetBkMode(OPAQUE);		
		pDC->SetBkColor(RGB(239, 239, 239));		
		
		return m_Dlg_brush;		
	}
	// 여기까지 추가
	return hbr;
}