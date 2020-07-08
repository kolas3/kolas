// BO_LOC_3874.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_3874.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3874 dialog


CBO_LOC_3874::CBO_LOC_3874(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3874)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3874::~CBO_LOC_3874()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3874::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3874)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3874, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3874)
	ON_BN_CLICKED(IDC_bCLOSE, OnbCLOSE)
	ON_BN_CLICKED(IDC_bFileSelect, OnbFileSelect)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3874 message handlers

BOOL CBO_LOC_3874::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3874::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 	
	INT ids = InitESL_Mgr( _T("SM_BO_LOC_3874") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	InitGroupCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;
}

INT CBO_LOC_3874::InitGroupCombo()
{
	EFS_BEGIN

	INT i=0, ids=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3870_GROUP") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitGroupCombo()") );

	CESL_ControlMgr* pCM = FindCM(_T("CM_BO_LOC_3874"));
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitGroupCombo()") );

	CESL_ControlMultiComboBox* pCMCB = (CESL_ControlMultiComboBox*)(pCM->FindControl(_T("그룹코드")));
	if( pCMCB == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("InitGroupCombo()") );

	ids = pDM->RefreshDataManager(_T(""));
	if(ids<0) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InitGroupCombo()") );


	CString sInsertCode[2];
	
	pCMCB->InitMultiComboBox(2);

	sInsertCode[0] = _T("   ");
	sInsertCode[1] = _T("적용안함");
	ids = pCMCB->SetData( sInsertCode , 2 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("InitGroupCombo()") );

	for( i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		pDM->GetCellData( _T("코드") , i , sInsertCode[0] );
		pDM->GetCellData( _T("이름") , i , sInsertCode[1] );

		sInsertCode[0].TrimLeft(); sInsertCode[0].TrimRight();
		if( sInsertCode[0].IsEmpty() ) continue;
			
		ids = pCMCB->SetData( sInsertCode , 2 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InitGroupCombo()") );
	}

	pCMCB->SetCurSel(0);

	return 0;

	EFS_END
	return -1;
}

VOID CBO_LOC_3874::OnbCLOSE() 
{
EFS_BEGIN
	OnCancel();
EFS_END
}

VOID CBO_LOC_3874::OnbFileSelect() 
{
	EFS_BEGIN

	INT ids;

	ids = LoadFile();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("CBO_LOC_3874::OnbFileSelect() ") );

	EFS_END
}

VOID CBO_LOC_3874::OnbSAVE() 
{
EFS_BEGIN
	SaveProc();
EFS_END
}

INT CBO_LOC_3874::LoadFile()
{
	EFS_BEGIN

    CString sFileName;
    INT     ids;

	TCHAR    sCurrentDirectory[_MAX_PATH];

	//  현 디렉토리를 저장한다.
    _tgetcwd( sCurrentDirectory, _MAX_PATH ) ;

	if ( _tcscmp( m_sWorkingDirectory, _T("")) )
		_tchdir( m_sWorkingDirectory );

    CFileDialog dlg(true, _T("xls"), _T("*.xls"),
                    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					// 2009.02.19 UPDATE BY KSJ : 엑셀 파일만 불러 올 수 있도록 수정
                    //DEL _T("파일형식(*.XLS)|All Files(*.*)|*.*||") );
					_T("파일형식(*.XLS)") );
    if ( IDOK != dlg.DoModal() ) return 0;

	// 원래 디렉토리로 복귀
    _tgetcwd( m_sWorkingDirectory.GetBuffer(0), _MAX_PATH ) ;
	_tchdir( sCurrentDirectory ) ;

    // 파일명이 선택되지않고 '확인'을 click했을 경우.
    sFileName = dlg.GetFileTitle();
    if ( sFileName.IsEmpty() ) return 1;
    sFileName = dlg.GetPathName();

	ids = ExcelFileToDM( sFileName );	// 엑셀 파일 반입
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3390::LoadFile()") );

	ids = AllControlDisplay( _T("CM_BO_LOC_3874") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::LoadFile()") );

	m_sFileName = sFileName;
	ids = SetControlData( _T("CM_BO_LOC_3874") , _T("반입파일명") ,  m_sFileName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3, _T("CBO_LOC_3390::LoadFile()") ); 

	AfxMessageBox(_T("파일읽어오기에 성공하였습니다."));

	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3874::ExcelFileToDM(CString sFileName)
{
	EFS_BEGIN

	INT ids;

	// 1. 파일을 오픈한다.
	C_ExcelManControl ExcelCtrl;
	CRect rect;
	m_pParentWnd->GetWindowRect(rect);


	CString strLastError;
	ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, m_pParentWnd, WM_USER + 10000);
	if (ExcelCtrl.m_hWnd == NULL) {
		strLastError = _T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오.");
		AfxMessageBox( strLastError);
		return 1;
	}
    CExcelManager ExcelMgr;
	ExcelMgr.SetManager(&ExcelCtrl);
		
    if(ExcelMgr.OpenManager() == FALSE) 
	{
        strLastError = _T("NOT Installed Excelmanager.ocx or No Excel!!!");
		AfxMessageBox( strLastError);
		return 2;
	}

	ExcelMgr.SetUseTemplate(TRUE, sFileName);
	if (ExcelMgr.OpenManager() == FALSE)
	{
		strLastError = _T("Open Failed");
		AfxMessageBox( strLastError);
		return 3;
	}
	
	// Get Header
	
	
	CArray <CString,CString> 	sColAlias;
	sColAlias.RemoveAll();

	INT nColCount = 0;
	CString strAlias;
	while (TRUE)
	{
		strAlias = ExcelMgr.GetTextMatrix(1, nColCount + 1);

		if (strAlias.IsEmpty()) break;

		sColAlias.Add(strAlias);
		nColCount++;
	}

	if(nColCount < 1){
		AfxMessageBox(_T("엑셀파일의 형식이 잘못되었습니다. 확인후에 다시 작업해주십시요."));
		ExcelMgr.CloseManager();
		return 0;
	}

	CArray< CString, CString >sDMAliasList;
	GetDMAlias( sColAlias, sDMAliasList );
	INT DMAliasCnt = sDMAliasList.GetSize();
	
	// 3. 한라인씩 읽어가며 DM에 INSERT한다.
	CString sDMAlias = _T("DM_BO_LOC_3874");
	CESL_DataMgr *pDM = FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CBO_LOC_3874::ExcelFileToDM( CString sFileName )") ); 
	pDM->FreeData();

	CString strItem;
	BOOL bStop = FALSE;
	BOOL bRowEmpty;	// 행전체가 비어 있는지 검사
	// 엑셀 파일의 끝은 행 전체가 비어 있거나 #EOF가 존재하는 행이다.
	CString DMAlias;
	while(TRUE)
	{
		pDM->InsertRow(-1);
		bRowEmpty = TRUE;
		for( INT i = 0 ; i < nColCount ; i++ )
		{
			strItem = ExcelMgr.GetTextMatrix(pDM->GetRowCount() + 1, i + 1);
			
			if( strItem.Compare(_T("#_TEOF"))==0)
			{
				bStop = TRUE;
				
				i = nColCount;
			}

			if( !bStop )
			{
				if( !strItem.IsEmpty() )
					bRowEmpty = FALSE;

				strItem.Replace( _T("\n") , _T("\r\n") );

				//ids = pDM->SetCellData( sColAlias.GetAt(i) , strItem , pDM->GetRowCount()-1 );
				
				DMAlias = sDMAliasList.GetAt( i );
				if( !DMAlias.IsEmpty() )
					ids = pDM->SetCellData( sDMAliasList.GetAt(i) , strItem , pDM->GetRowCount()-1 );
				else
					pDM->SetCellData( sColAlias.GetAt(i) , strItem , pDM->GetRowCount()-1 );

				//if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CBO_LOC_3390::FileToDM( CString sFileName )") ); 
			}
		}
		if( bRowEmpty )
			bStop = TRUE;
		if( bStop ) break;
	}
	pDM->DeleteRow( pDM->GetRowCount()-1 );

	ExcelMgr.CloseManager();

	// 4. 끝
	return 0;

	EFS_END
	return -1;
}

INT CBO_LOC_3874::GetDMAlias( CArray< CString, CString >&sColAlias, CArray< CString, CString >&sDMAlias )
{
	EFS_BEGIN

	sDMAlias.RemoveAll();
	
	CString sLine, DMAlias, sExcelAlias;
	INT locate = 0;
	INT colAliasCnt = sColAlias.GetSize();
	sDMAlias.SetSize( colAliasCnt );
	CString colAlias;

	return 0;

	CStdioFile fp; 
	//fp.Open( FILE_PATH_, CFile::modeRead | CFile::shareDenyWrite );
	if( !fp.m_pStream )
	{
		AfxMessageBox( _T("반입반출필드정보.txt 파일을 찾을 수 없습니다.") );
		return -1;
	}
	
	while( fp.ReadString( sLine ) )
	{
		if( _T(".") == sLine.Left( 0 ) )
			continue;
		
		locate = sLine.Find( _T(":") );
		DMAlias = sLine.Left( locate );
		sExcelAlias = sLine.Right( sLine.GetLength() - locate - 1 );
		
		for( INT i = 0; i < colAliasCnt; i++ )
		{
			colAlias = sColAlias.GetAt( i );
			
			if( colAlias == sExcelAlias )
			{
				//sDMAlias.Add( DMAlias );
				sDMAlias.SetAt( i, DMAlias );
				break;
			}
		}
	}
	
	return 0;

	EFS_END

	return -1;
}

INT CBO_LOC_3874::SaveProc()
{
EFS_BEGIN
	INT ids = 0 , i=0;

	CESL_DataMgr *pDM = FindDM( _T("DM_BO_LOC_3874") );
	if(pDM==NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("SaveProc()") );
	
	if( pDM->GetRowCount() == 0 ) 
	{
		AfxMessageBox( _T("반입 대상이 존재하지 않습니다.") );
		return 1;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3874") , _T("파일반입그리드") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("SaveProc()") );

	pGrid->SelectMakeList();

	INT nSelectCnt = pGrid->SelectGetCount();

	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("반입 대상자를 선정해 주십시요.") );
		return 2;
	}

	//그룹코드 가져오기
	CString sGroup;
	ids = GetControlData( _T("CM_BO_LOC_3874") , _T("그룹코드") , sGroup , -1 , 0);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("SaveProc()") ); 

	if( sGroup.IsEmpty() )
	{
		AfxMessageBox(_T("지정할 그룹을 선택해주십시요."));
		return 3;
	}

	// DB에 insert.
	pDM->StartFrame();

	pGrid->SelectGetHeadPosition();

	INT nIndex;
	CString sRecKey;
	CString sTemp;
	for( i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();
		pDM->MakeRecKey(sRecKey);

		pDM->InitDBFieldData();

		// 이름
		ids = pDM->GetCellData( _T("이름") , nIndex , sTemp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -11 , _T("SaveProc()") );
		pDM->AddDBFieldData( _T("NAME") , _T("STRING") , sTemp );

		// 주민등록번호
		ids = pDM->GetCellData( _T("주민등록번호") , nIndex , sTemp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -13 , _T("SaveProc()") );
		pDM->AddDBFieldData( _T("CIVIL_NO") , _T("STRING") , sTemp );

		// 핸드폰
		ids = pDM->GetCellData( _T("핸드폰") , nIndex , sTemp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -15 , _T("SaveProc()") );
		pDM->AddDBFieldData( _T("HANDPHONE") , _T("STRING") , sTemp );

		// 이메일
		ids = pDM->GetCellData( _T("이메일") , nIndex , sTemp );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -17 , _T("SaveProc()") );
		pDM->AddDBFieldData( _T("E_MAIL") , _T("STRING") , sTemp );

		// 그룹코드
		pDM->AddDBFieldData( _T("GROUP_CODE") , _T("STRING") , sGroup );

		// REC_KEY
		pDM->AddDBFieldData( _T("REC_KEY") , _T("NUMERIC") , sRecKey );

		ids = pDM->MakeInsertFrame( _T("CO_NONMEMBER_TBL") );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -19 , _T("SaveProc()") );

		pGrid->SelectGetNext();
	}

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -21 , _T("SaveProc()") );

	pDM->EndFrame();

	sTemp.Format(_T("%d명의 비회원을 %s 그룹으로 반입하였습니다."), nSelectCnt , sGroup.GetBuffer(0) );
	AfxMessageBox(sTemp);

	return 0;
EFS_END
	return -1;
}

HBRUSH CBO_LOC_3874::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//MACRODEL	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
