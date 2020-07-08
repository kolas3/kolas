// BO_LOC_3863.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3863.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3863 dialog


CBO_LOC_3863::CBO_LOC_3863(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3863)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_3863::~CBO_LOC_3863()
{
}

VOID CBO_LOC_3863::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3863)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_LOC_3863, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3863)
	ON_BN_CLICKED(IDC_btn3863_CLOSE, Onbtn3863CLOSE)
	ON_BN_CLICKED(IDC_btn3863_CHANGE, Onbtn3863CHANGE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3863 message handlers

BOOL CBO_LOC_3863::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	INT ids = InitESL_Mgr( _T("BO_LOC_3863") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}



INT CBO_LOC_3863::InitControl()
{
EFS_BEGIN

	INT ids;
/*
	CESL_DataMgr* pDM = FindDM(_T("DM_USER_GROUP_CODE_INFO"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitControl") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3863"));
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
*/

	// 필드 정보 구성
	CStdioFile fFieldInfo;
	ids = fFieldInfo.Open( _T("..\\cfg\\메일링시스템\\필드정보.txt") , CFile::modeRead | CFile::typeBinary );
	if( 0 == ids )
	{
		AfxMessageBox( _T("설정 정보 파일을 열수 없습니다. ") );
		return 1;
	}

	TCHAR cUni;
	fFieldInfo.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fFieldInfo.SeekToBegin();
	}

	CString strLine , strFieldInfoText;
	while( fFieldInfo.ReadString( strLine ) )
	{
		if( FALSE ==  strFieldInfoText.IsEmpty() )
		{
			strFieldInfoText +=  _T("\r\n");
		}
		strFieldInfoText +=  strLine;
	}

	fFieldInfo.Close();

	GetDlgItem( IDC_edt3863Text1 )->SetWindowText(strFieldInfoText);

	return 0;

EFS_END
return -1;

}

VOID CBO_LOC_3863::Onbtn3863CLOSE() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}

VOID CBO_LOC_3863::Onbtn3863CHANGE() 
{
EFS_BEGIN

	AfxGetApp()->DoWaitCursor(1);

	ChangeGroupSql();	

	AfxGetApp()->DoWaitCursor(-1);

EFS_END
}


INT CBO_LOC_3863::ChangeGroupSql()
{
EFS_BEGIN

	INT ids;

	CString sSql;
	CString sWhere;
	CString sGroup;

	GetDlgItem(IDC_edt3863_COND1)->GetWindowText( sWhere );
	sWhere.TrimLeft(); sWhere.TrimRight();

	if( sWhere.IsEmpty() )
	{
		AfxMessageBox( _T("쿼리문을 입력해주십시오.") );
		return 1;
	}
/*
	ids = GetControlData( _T("CM_BO_LOC_3863") , _T("그룹") , sGroup , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ChangeGroupSql") );
*/
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3860") );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ChangeGroupSql") );

	if( sWhere.GetAt( sWhere.GetLength()-1 ) != ';' )
	{
		sWhere += _T(";");
	}

	pDM->StartFrame();
	ids = pDM->AddFrame( sWhere );
	if( ids < 0 ) 
	{
		pDM->EndFrame();
		AfxMessageBox( _T("SQL문에 문제가 있어 변경하지 못하였습니다.") );
		return 2;
	}

	ids = pDM->SendFrame();
	if( ids < 0 )
	{
		pDM->EndFrame();
		AfxMessageBox( _T("변경하지 못하였습니다.") );
		return 1;
	}

	pDM->EndFrame();

	AfxMessageBox( _T("변경되었습니다.") );

	return 0;


EFS_END
return -1;

}

HBRUSH CBO_LOC_3863::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
