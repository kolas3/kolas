// ECO_NOTICE_MANAGER1.cpp : implementation file
//

#include "stdafx.h"
#include "ECO_NOTICE_MANAGER1.h"
#include "ECO_NOTICE_LIB_CODE.h"
#include "..\..\..\공동목록\check_dup_api\RS_SQL.h"
#include <direct.h>
#include "..\\..\\include\\공동목록\\RS_Acc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CECO_NOTICE_MANAGER dialog


CECO_NOTICE_MANAGER::CECO_NOTICE_MANAGER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CECO_NOTICE_MANAGER)
	//}}AFX_DATA_INIT
	m_FILE_INFO.FileCnt = 0;
}


VOID CECO_NOTICE_MANAGER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CECO_NOTICE_MANAGER)
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CECO_NOTICE_MANAGER, CDialog)
	//{{AFX_MSG_MAP(CECO_NOTICE_MANAGER)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_bIDOK, OnbIDOK)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bINCLUDE_FILE, OnbINCLUDEFILE)
	ON_BN_CLICKED(IDC_bAPPEND, OnbAPPEND)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDCANCEL2, OnCancel2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CECO_NOTICE_MANAGER message handlers
BOOL CECO_NOTICE_MANAGER::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add _T("About...") menu item to _tsystem menu.

	m_pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB1);
	m_pTabCtrl->InsertItem(0,_T("공지사항발송"));
	m_pTabCtrl->InsertItem(1,_T("보낸공지사항"));
	m_pTabCtrl->SetCurSel(0);

	if(InitESL_Mgr(_T("SM_ECO_NOTICE_INPUT"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
    m_SERVER_TYPE = _T("LS") ;

	// TODO: Add extra initialization here
	m_button1.AutoLoad( IDC_bINCLUDE_FILE, this );
    m_SelectLibCnt = 0 ;

	m_pDM = (CRS_DM_Mgr*)FindDM(_T("DM_ECO_NOTICE_INPUT"));

	m_pDM->m_dbkind =  4; 
	if ( m_pDM == NULL )
	{
		AfxMessageBox(_T("DM 초기화 에러(DM_ECO_NOTICE)"));
		return FALSE;
	}

	INT ids;

	ids = m_pDM->RefreshDataManager(_T(""));

	// CM 초기화
	CESL_ControlMgr *pCM = FindCM(_T("CM_ECO_NOTICE_INPUT"));
	if ( pCM == NULL )
	{
		AfxMessageBox(_T("CM 초기화 에러(CM_ECO_NOTICE_INPUT)"));
		return FALSE;
	}
	// Grid 초기화
	m_pGrid = (CESL_Grid*)pCM->FindControl( _T("MainGrid"));
	if ( m_pGrid == NULL )
	{
		AfxMessageBox(_T("GRID 초기화 에러(CM_ECO_NOTICE_INPUT)"));
		return FALSE;
	}
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;
	// grid display

	if( m_pGrid->Display() < 0 ) return FALSE;

	Display(0);

	AllControlDisplay (_T("CM_ECO_NOTICE_INPUT") ,-1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

INT CECO_NOTICE_MANAGER::Display(INT nMode)
{
	if( nMode == 0 )
	{
		(CWnd*)GetDlgItem(IDC_eRECEIPT_LIB)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_STATIC5)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_eTHEME)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_LIST1)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_bINCLUDE_FILE)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_bSEARCH)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_PROS)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_MainGrid)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bAPPEND)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_bIDOK)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDCANCEL2)->ShowWindow(SW_SHOW);
		(CWnd*)GetDlgItem(IDC_eVIEW)->ShowWindow(SW_SHOW);
		
	}
	else
	{
		(CWnd*)GetDlgItem(IDC_eRECEIPT_LIB)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC3)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_STATIC5)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_eTHEME)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_LIST1)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bINCLUDE_FILE)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bSEARCH)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_PROS)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_PROGRESS)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bAPPEND)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_bIDOK)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDCANCEL2)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_eVIEW)->ShowWindow(SW_HIDE);
		(CWnd*)GetDlgItem(IDC_MainGrid)->ShowWindow(SW_SHOW);

		m_pDM = (CRS_DM_Mgr*)FindDM(_T("DM_ECO_NOTICE_INPUT"));

		m_pDM->m_dbkind =  4; 
		if ( m_pDM == NULL )
		{
			AfxMessageBox(_T("DM 초기화 에러(DM_ECO_NOTICE)"));
			return FALSE;
		}

		INT ids;

		ids = m_pDM->RefreshDataManager(_T(""));

		CESL_ControlMgr *pCM = FindCM(_T("CM_ECO_NOTICE_INPUT"));
		if ( pCM == NULL )
		{
			AfxMessageBox(_T("CM 초기화 에러(CM_ECO_NOTICE_INPUT)"));
			return FALSE;
		}

		m_pGrid->DisplayLine(-1);
			
	}

	INT nCurSel = m_pGrid->GetTopRow();
	m_pGrid->SetReverse(0);
	m_pGrid->SetTopRow(nCurSel);
	return 0;
}

VOID CECO_NOTICE_MANAGER::OnbIDOK() 
{
	CESL_DataMgr * m_pDataMgr;
	CString        ParentRecKey, LibraryCode, LibraryName, LocalFileRecKey, tmpData;
	INT            ids, i;

	//////////////////////////////////////////////////////////////////////////////////
	// 1. 도서관 선택여부를 체크한다.
	//////////////////////////////////////////////////////////////////////////////////
	if ( m_SelectLibCnt == 0 ) {
		MessageBox( _T("공지사항을 보낼 도서관을 선택하십시오"), _T("확인"), MB_OK );
		return;
	}

    m_pDataMgr = FindDM(_T("DM_ECO_NOTICE_INPUT")) ;
    if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error"));
		return ;
	}

    CRS_SQL m_rs_sql(this) ;
	//m_rs_sql.GetLibraryInfo (LibraryCode);
	CString sWhere ;
	sWhere.Format (_T("select LIB_NAME from LIB_INFO_tbl where lib_CODE = '%s'"), _T("999999") ) ;
    m_rs_sql.ES_GetOneFieldValue (sWhere,LibraryName ) ;


	//////////////////////////////////////////////////////////////////////////////////
	// 1. center에 저장
	//////////////////////////////////////////////////////////////////////////////////

	for ( i = 0; i < m_SelectLibCnt; i++ ) {
		
		//////////////////////////////////////////////////////////////////////////////////
		// 2. 공지사항을 저장한다.
		//////////////////////////////////////////////////////////////////////////////////
				
		m_SERVER_TYPE =_T("ES") ;
        if(m_SERVER_TYPE == _T("ES"))
		{
			ids = Input( m_ParentRecKey, i, LibraryCode, LibraryName );
			if ( ids<0 ) {
				AfxMessageBox( _T("입력을 실패했습니다."), MB_OK);
				return;
			}
			if (ids == 100 )
			{
				return ;
			}
			
			//////////////////////////////////////////////////////////////////////////////////
			// 3. 보낸도서관 정보를 center에 저장한다.
			//////////////////////////////////////////////////////////////////////////////////
			ids = AllInputLibraryInfo(  m_select_Lib_Code.GetAt (i) , 
										m_select_Lib_Name.GetAt (i) , 
										m_ParentRecKey );
		

			ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, m_ParentRecKey ,m_SERVER_TYPE );
			if ( ids<0 ) {
				AfxMessageBox( _T("LocalInsertFile_Proc() ERROR"), MB_OK);
				return;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////
		// 4. local에 저장
		//////////////////////////////////////////////////////////////////////////////////
		
	    m_SERVER_TYPE =_T("LS") ;
		/*
		if(m_SERVER_TYPE == _T("LS"))
		{
			ids = m_pDataMgr->StartFrame();
			m_pDataMgr->InitDBFieldData () ;
        
			ids = Input( ParentRecKey, i, LibraryCode, LibraryName );
			if ( ids<0 ) {
				m_pDataMgr->EndFrame();
				AfxMessageBox ( _T("입력을 실패했습니다."), MB_OK);
				return;
			}

			ids = m_pDataMgr->SendFrame();
			ids = m_pDataMgr->EndFrame();
		
	
			ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, ParentRecKey ,m_SERVER_TYPE );
			if ( ids<0 ) {
				AfxMessageBox( _T("LocalInsertFile_Proc() ERROR"), MB_OK);
				return;
			}

		}*/
/*
		tmpData.Format ( _T("%d/%d 진행중"), i+1,  );
		m_pProgressBar.SetWindowText( tmpData );
		m_pProgressBar.StepIt();
*/
	}



	//ids = AllControlDisplay(_T("CM_ECO_NOTICE_MAIN"),-1);

	//m_pProgressBar.SetWindowText( _T("완료") );

    MessageBox( _T("공지사항을 보냈습니다."), _T("입력"), MB_OK );
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_ECO_NOTICE_INPUT"));
	pCM->AllControlClear () ;
}
////////////////////////////////////////////////////////////////////////////////////
// Input
//
// [PURPOSE]
// [MINISPEC]
// 1. 입력항목을 체크한다.
// 2. Local 저장일때는 보낸공지사항
// 3. center 저장일때는 받은공지사항
// 4. Local DB 저장일때는 Momory도 Insert 함
// 5. center DB 저장일때는 Momory도 Insert 안함
////////////////////////////////////////////////////////////////////////////////////
INT CECO_NOTICE_MANAGER::Input( CString RecKey, INT index, CString LibraryCode, CString LibraryName )
{
	CESL_DataMgr  * m_pDataMgr;
	CString         Subject, Contents;
	CString         CurrentDate;
	// KOL.UDF.022
	INT             ids = 0, RowCnt = 0;

    CString AttrData [10];
    CString AttrName [10] = { _T("REC_KEY")        , // 레코드KEY
		                    _T("send_libid")      , // 보낸 도서관 코드
							_T("send_libname")    , // 보낸 도서관 명
							_T("recv_libid")      , // 받는 도서관 코드
							_T("recv_libname")    , // 받는 도서관 명
							_T("title")           , // 제목
							_T("contents")        , // 내용
							_T("confirm")         , // 확인
							_T("srflag")          , // 보낸공지사항
							_T("trans_date")      };
    CString    FieldType[10] = {_T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

    m_pDataMgr = FindDM(_T("DM_ECO_NOTICE_INPUT")) ;
	if(m_pDataMgr == NULL){
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. 입력항목을 체크한다.
	/////////////////////////////////////////////////////////////////////////////////////
	ids = GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("제목"),Subject,-1) ;
	ids = GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("내용"),Contents,-1) ;
	
	
	if ( m_select_Lib_Code.GetAt (0).IsEmpty () ) {
		MessageBox( _T("받을도서관을 선택하십시오."), _T("도서관찾기체크"), MB_OK );
		return 100;
	}
	if ( m_arSelectLib.GetAt (index).IsEmpty () ||
		 Subject.IsEmpty () || 
		 Contents.IsEmpty () ) {
		MessageBox( _T("데이타를 입력하십시오."), _T("입력항목체크"), MB_OK );
		return 100;
	}
	/*if(m_SERVER_TYPE == _T("LS"))
	{
		m_pDataMgr->MakeRecKey (RecKey) ;
		m_ParentRecKey = RecKey ;
	}
    else*/
	if( m_SERVER_TYPE == _T("ES"))
	{
		CRS_SQL m_rs_sql(this) ;
        CString sqlBuffer;
		CString temp ;
		sqlBuffer.Format (_T("select max(rec_key) from ECO_NOTICE_tbl")) ;
		m_rs_sql.ES_GetOneFieldValue (sqlBuffer,temp);
		
        INT itmp ;
		itmp = _ttoi(temp.GetBuffer (0)) ;
		RecKey.Format (_T("%d"),itmp+1) ;

		m_ParentRecKey = RecKey ;
	}


	AttrData[0] = RecKey;
	AttrData[1] = LibraryCode;
	AttrData[2] = LibraryName;
	AttrData[3] = m_select_Lib_Code.GetAt (index);
	AttrData[4] = m_select_Lib_Name.GetAt (index);
	AttrData[5] = Subject;
	AttrData[6] = Contents;
	AttrData[7] = _T("0");
	/////////////////////////////////////////////////////////////////////////////////////
	// 2. Local 저장일때는 보낸공지사항
	/////////////////////////////////////////////////////////////////////////////////////
	if ( _T("LS") == m_SERVER_TYPE ) {
		AttrData[8] = _T("1");
	/////////////////////////////////////////////////////////////////////////////////////
	// 3. center 저장일때는 받은공지사항
	/////////////////////////////////////////////////////////////////////////////////////
	} else {
		AttrData[8] = _T("1");
	}

    CTime t = CTime::GetCurrentTime () ;
	CurrentDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
	AttrData[9] = CurrentDate;
     
    RowCnt ;
/*
	/////////////////////////////////////////////////////////////////////////////////////
	// 4. Local DB 저장일때는 Momory도 Insert 함
	/////////////////////////////////////////////////////////////////////////////////////
	if ( _T("LS") == m_SERVER_TYPE  ) {
		for(INT i =0 ; i <10;i++) {
        m_pDataMgr->AddDBFieldData (AttrName[i],FieldType[i],AttrData[i]) ;
        }

		ids = m_pDataMgr->MakeInsertFrame (_T("ECO_NOTICE_TBL")) ;
	/////////////////////////////////////////////////////////////////////////////////////
	// 5. center DB 저장일때는 Momory도 Insert 
	/////////////////////////////////////////////////////////////////////////////////////
	} else */
	if( m_SERVER_TYPE == _T("ES")){
		 CRS_SQL m_rs_sql(this);
		 INT FieldType[10] = { 2,1,1,1,1,1,1,1,1,1};
		 m_rs_sql.ES_InsertRow (_T("ECO_NOTICE_TBL"),10,AttrName,AttrData,FieldType,TRUE) ;

	}

	return 0;
}

VOID CECO_NOTICE_MANAGER::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	CComboBox * pCombo = (CComboBox*)GetDlgItem(IDC_eRECEIPT_LIB) ;
	CECO_NOTICE_LIB_CODE dlg(this) ;
	if(IDOK == dlg.DoModal ()) 
	{
		//dlg.m_arLibrary ;
        INT Cnt ;
        Cnt = dlg.m_arLibrary.GetSize () ;
		m_SelectLibCnt = Cnt ;
        for(INT i=0; i<Cnt; i++)
		{   
			pCombo->AddString (dlg.m_arLibrary.GetAt (i));
			m_arSelectLib.Add (dlg.m_arLibrary.GetAt (i));
			m_select_Lib_Code.Add (dlg.m_Lib_Code.GetAt (i));
			m_select_Lib_Name.Add (dlg.m_Lib_Name.GetAt (i));
		}
		pCombo->SetCurSel (0) ;

		CEdit* sEdit = (CEdit*)GetDlgItem(IDC_PROS) ;
		
		CString msg ;
		msg.Format (_T("외 %d 군데"),Cnt-1) ; 
		sEdit->SetWindowText (msg) ;
	}

}

INT CECO_NOTICE_MANAGER::AllInputLibraryInfo(CString pLibraryCode, CString pLibraryName, CString pParentRecKey)
{
//	CESL_DataMgr  * m_pDataMgr;
	CString         RecKey;
	//INT             ids;

    CString AttrData [4];
    CString AttrName [4] = {  _T("REC_KEY")    ,
		                     _T("PARENT_KEY") ,
							 _T("LIBID")      ,
	                         _T("LIBNAME")    };
	INT    FieldType[4] = { 2, 2, 1, 1 };
  
	CRS_SQL m_rs_sql(this);
    CString sqlBuffer ;
	CString temp ;
	sqlBuffer = _T("select max(rec_key) from ECO_NOTICE_RECV_LIBRARY_TBL");
	m_rs_sql.ES_GetOneFieldValue (sqlBuffer,temp) ;

	INT MaxKey ;
	MaxKey = _ttoi(temp.GetBuffer (0));

	RecKey.Format (_T("%d"),MaxKey+1);
/*  
	ids = MakePrimaryKey( RecKey );
	if ( ids ) return -1;
*/
	AttrData[0] = RecKey;
	AttrData[1] = pParentRecKey;
	AttrData[2] = pLibraryCode;
	AttrData[3] = pLibraryName;

	m_rs_sql.ES_InsertRow (_T("ECO_NOTICE_RECV_LIBRARY_TBL"),4,AttrName,AttrData,FieldType ,TRUE);


	return 0;
}

VOID CECO_NOTICE_MANAGER::OnbINCLUDEFILE() 
{
	FILE    * fp;
	CString   strPathName;
	TCHAR      FileName[256];
	TCHAR      currentpath[256], filesize[256];
	long      position;
	INT       numread, filelen;	

	_tgetcwd( currentpath, MAX_PATH );

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. 파일 Dialog를 띄운다.
	/////////////////////////////////////////////////////////////////////////////////////
	CFileDialog dlg( TRUE, _T(""), _T("*.*"),
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("모든파일(*.*)|*.*|") );
	dlg.m_ofn.lpstrTitle = _T("첨부 파일 삽입");
	dlg.m_ofn.lStructSize;

	if(IDOK == dlg.DoModal () )
	{
		
		m_FILE_INFO.FileCnt = m_FILE_INFO.FileCnt+1;
		strPathName = dlg.GetPathName();

		/////////////////////////////////////////////////////////////////////////////////////
		// 2. 파일을 open한다.
		/////////////////////////////////////////////////////////////////////////////////////
		fp = _tfopen( strPathName, _T("rb") );
		_tchdir( currentpath );

		/////////////////////////////////////////////////////////////////////////////////////
		// 3. Open한 파일의 크기를 구한다.
		/////////////////////////////////////////////////////////////////////////////////////
		fseek( fp, 0, SEEK_END);
		position = ftell( fp );
		fseek( fp, 0, SEEK_SET);

		incfile = (TCHAR*)malloc( position );
		numread = fread( incfile, sizeof( TCHAR ), position, fp );
		_stprintf( filesize, _T("%d"), numread );

		/////////////////////////////////////////////////////////////////////////////////////
		// 4. 구조체에 파일이름과 사이즈를 저장한다.
		/////////////////////////////////////////////////////////////////////////////////////
		_tcscpy( m_FILE_INFO.FilePathBuf[m_FILE_INFO.FileCnt-1], strPathName.GetBuffer(0) );
		_tcscpy( m_FILE_INFO.FileNameBuf[m_FILE_INFO.FileCnt-1], dlg.GetFileName() );
		_tcscpy( m_FILE_INFO.filesize   [m_FILE_INFO.FileCnt-1], filesize );

		/////////////////////////////////////////////////////////////////////////////////////
		// 5. 파일 사이즈의 KB단위로 계산한다.
		/////////////////////////////////////////////////////////////////////////////////////
		filelen = _tcsclen( filesize );
		if ( filelen < 4 ) {
			_stprintf( FileName, _T("%s(1KB)"), dlg.GetFileName() );
		} else if ( filelen == 4 ) {
			_stprintf( FileName, _T("%s(%cKB)"), dlg.GetFileName(), filesize[0] );
		} else if ( filelen == 5 ) {
			_stprintf( FileName, _T("%s(%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1] );
		} else if ( filelen == 6 ) {
			_stprintf( FileName, _T("%s(%c%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1], filesize[2] );
		} else if ( filelen == 7 ) {
			_stprintf( FileName, _T("%s(%c.%c%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1], filesize[2], filesize[3] );
		} else if ( filelen == 8 ) {
			_stprintf( FileName, _T("%s(%c%c.%c%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1], filesize[2], filesize[3], filesize[4] );
		} else if ( filelen == 9 ) {
			_stprintf( FileName, _T("%s(%c%c%c.%c%c%cKB)"), dlg.GetFileName(), filesize[0], filesize[1], filesize[2], filesize[3], filesize[4], filesize[5] );
		}

		/////////////////////////////////////////////////////////////////////////////////////
		// 6. 리스트 컨트롤에 추가한다.
		/////////////////////////////////////////////////////////////////////////////////////
		m_List.InsertColumn (0,_T(""));
		m_List.InsertItem (0,FileName) ;

		fclose( fp );
	}

}


INT CECO_NOTICE_MANAGER::InsertFile_Proc( CESL_DataMgr * pDataMgr, CString pLocalFileRecKey, CString pParentRecKey ,CString ServerType ) 
{
	INT  i, ids;
    

    if(ServerType == _T("ES"))
    {
        CRS_SQL m_rs_sql (this) ;
		CString sqlBuf ;
		CString tmp_Value ;
        INT tmpReckey ;
		
		sqlBuf = _T("select max(rec_key) from ECO_NOTICE_FILE_TBL ") ;
	     
		CString Field[5] = {_T("REC_KEY"),_T("FILENAME"),_T("FILE_SIZE"),_T("PARENT_KEY") } ;
		CString FieldData[4] ;
		INT     FieldType[4] = { 2,1,2,2} ;

		for ( i = 0; i < m_FILE_INFO.FileCnt; i++ ) {

			m_rs_sql.ES_GetOneFieldValue (sqlBuf,tmp_Value);
			tmpReckey = _ttoi(tmp_Value) ;
            pLocalFileRecKey.Format (_T("%d"),tmpReckey+1) ;
          
            FieldData[0] = pLocalFileRecKey;
            FieldData[1] = m_FILE_INFO.FileNameBuf[i] ;
			FieldData[2] = m_FILE_INFO.filesize[i] ;
			FieldData[3] = pParentRecKey ;


			m_rs_sql.ES_InsertRow (_T("ECO_NOTICE_FILE_TBL"),4,Field,FieldData,FieldType,TRUE) ;
            

			ids = RS_InsertFile( 
							_T("ECO_NOTICE_FILE_TBL")  ,
							 pLocalFileRecKey.GetBuffer (0) ,
							 m_FILE_INFO.FileNameBuf[i], 
							 m_FILE_INFO.FilePathBuf[i],    
							 4
							 ) ;
			if ( ids<0 ) {				
				return ids;
			}
	
		}

	}
/*	else if(ServerType == _T("LS"))
	{
        CString tmpName ;
		CString tmpSize ;

		for ( i = 0; i < m_FILE_INFO.FileCnt; i++ ) {
		    tmpName.Format (_T("%s"),m_FILE_INFO.FileNameBuf[i]);
			tmpSize.Format (_T("%d"),m_FILE_INFO.filesize[i]) ;


			pDataMgr->MakeRecKey(pLocalFileRecKey) ;
		
		    pDataMgr->StartFrame () ;
			pDataMgr->InitDBFieldData () ;
			pDataMgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),pLocalFileRecKey);
			pDataMgr->AddDBFieldData (_T("FILENAME"),_T("STRING"),tmpName) ;
            pDataMgr->AddDBFieldData (_T("FILE_SIZE"),_T("NUMERIC"),tmpSize) ;
            pDataMgr->AddDBFieldData (_T("PARENT_KEY"),_T("NUMERIC"),pParentRecKey) ;

		    pDataMgr->MakeInsertFrame (_T("ECO_NOTICE_FILE_TBL"));
			pDataMgr->SendFrame ();
			pDataMgr->EndFrame () ;

			
			ids = RS_InsertFile( 
							_T("ECO_NOTICE_FILE_TBL")  ,
							 pLocalFileRecKey.GetBuffer (0) ,
							 m_FILE_INFO.FileNameBuf[i], 
							 m_FILE_INFO.FilePathBuf[i],    
							 0
							 ) ;
			if ( ids<0 ) {				
				return ids;
			}
		}
	}
*/
	return 0;
}
VOID CECO_NOTICE_MANAGER::OnbAPPEND() 
{
	CESL_DataMgr * m_pDataMgr;
	CESL_ControlMgr * m_pControlMgr;
	CString           LibraryCode, LibraryName, MsqBuf, ParentRecKey;
	CString           LocalLibraryCode, LocalLibraryName, LocalFileRecKey;
	CString           tmpData, CenterParentRecKey, Subject, Contents;
	INT            ids, RecCnt, i;

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. 입력항목을 체크한다.
	/////////////////////////////////////////////////////////////////////////////////////
	
	GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("제목"),Subject,-1) ;
	GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("내용"),Contents,-1) ;


	if ( Subject.IsEmpty () || Contents.IsEmpty () ) {
		MessageBox( _T("데이타를 입력하십시오."), _T("입력항목체크"), MB_OK );
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// 1. 전체도서관 정보를 구한다.
	////////////////////////////////////////////////////////////////////////////////////
	
	m_pDataMgr = FindDM(_T("DM_ECO_NOTICE_LIB_CODE")) ;
	if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}
	
	m_SERVER_TYPE = _T("ES") ;

    CRS_SQL m_rs_sql(this) ;
	
	CString sWhere ;
	sWhere = _T("LIB_CODE IS NOT NULL");
	m_rs_sql.ES_Select (m_pDataMgr,sWhere,_T("DM_ECO_NOTICE_LIB_CODE"),100,3) ;

    RecCnt = m_pDataMgr->GetRowCount () ;

	if ( RecCnt == 0 ) {
		MessageBox( _T("도서관정보가 없습니다."), _T("도서관전체보내기"), MB_OK );
		return;
	}
   
    m_SERVER_TYPE = _T("LS") ;

	m_rs_sql.GetLibraryInfo (LocalLibraryCode ) ;
	CString tmp_Value;
    tmp_Value.Format (_T("select lib_name from lib_info_tbl where LIB_CODE = '%s'"),LocalLibraryCode ) ;
	m_rs_sql.ES_GetOneFieldValue (tmp_Value,LocalLibraryName ) ;



	MsqBuf.Format ( _T("전체 %d개 도서관에 위 공지사항을 보내시겠습니까?"), RecCnt );
	
	ids = MessageBox( MsqBuf, _T("도서관전체보내기"), MB_OKCANCEL|MB_ICONQUESTION );
	if ( IDCANCEL == ids ) return;

    m_pControlMgr = FindCM(_T("CM_ECO_NOTICE_INPUT")) ;
	if(m_pControlMgr == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return ;
	}
    
    
    MsqBuf.Format(_T("전체 %d개 도서관"), RecCnt );

    CComboBox * pCombo = (CComboBox*)GetDlgItem(IDC_eRECEIPT_LIB) ;
    pCombo->AddString (MsqBuf) ;

	GetDlgItem(IDC_PROS)->SetWindowText( _T("") );

	/////////////////////////////////////////////////////////////////////////////////////////
	// 2. Center공지사항에 저장할 키값을 구한다.
	/////////////////////////////////////////////////////////////////////////////////////////
    m_SERVER_TYPE = _T("ES") ;
     

	/*if(m_SERVER_TYPE == _T("LS"))
	{
		CString RecKey ;
		m_pDataMgr->MakeRecKey (RecKey) ;
		ParentRecKey = RecKey ;
	}
    else*/
	if (m_SERVER_TYPE == _T("ES"))
	{
		CRS_SQL m_rs_sql(this) ;
        CString sqlBuffer;
		CString temp ;
		sqlBuffer.Format (_T("select max(rec_key) from ECO_NOTICE_tbl")) ;
		m_rs_sql.ES_GetOneFieldValue (sqlBuffer,temp);
		
        INT itmp ;
		CString RecKey ;
		itmp = _ttoi(temp.GetBuffer (0)) ;
		RecKey.Format (_T("%d"),itmp+1) ;

		ParentRecKey = RecKey ;
	}
      

	InitProgress (RecCnt) ;

	////////////////////////////////////////////////////////////////////////////////////
	// 3. Center에 파일정보와 Server에 파일을 저장한다.
	////////////////////////////////////////////////////////////////////////////////////
	ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, ParentRecKey ,m_SERVER_TYPE );
	if ( ids<0 )
	{
		m_pDataMgr->EndFrame();
		AfxMessageBox ( _T("InsertFile_Proc"), MB_OK);
		return;
	}

    CenterParentRecKey =  ParentRecKey ;

	////////////////////////////////////////////////////////////////////////////////////
	// 4. Local에 공지사항을 저장한다.
	////////////////////////////////////////////////////////////////////////////////////
	/*m_SERVER_TYPE = _T("LS") ;


    m_pDataMgr->StartFrame();
	m_pDataMgr->InitDBFieldData () ;
    m_pDataMgr->MakeRecKey (ParentRecKey) ;

	ids = AllInput( ParentRecKey, LocalLibraryCode, LocalLibraryName );
	if ( ids<0 ) {
		m_pDataMgr->EndFrame();
		AfxMessageBox ( _T("입력을 실패했습니다."), MB_OK);
		return;
	}

	ids = m_pDataMgr->SendFrame(  );
	ids = m_pDataMgr->EndFrame();
	


	////////////////////////////////////////////////////////////////////////////////////
	// 5. local에 파일을저장한다.
	////////////////////////////////////////////////////////////////////////////////////
	m_SERVER_TYPE = _T("LS");
	ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, ParentRecKey ,m_SERVER_TYPE);
	if ( ids<0 ) {
		AfxMessageBox( _T("LocalInsertFile_Proc() ERROR"), MB_OK);
		return;
	}
*/
	/////////////////////////////////////////////////////////////////////////////////////////
	// 6. 보낸도서관 정보를 Center에 저장
	/////////////////////////////////////////////////////////////////////////////////////////
	m_SERVER_TYPE = _T("ES") ;


	for ( i = 0; i < RecCnt; i++ ) {
	     
		 ChangeProgressCtrl(i) ;

		 GetDataMgrData (_T("DM_ECO_NOTICE_LIB_CODE"),_T("LIB_CODE"),LibraryCode ,i) ;
         GetDataMgrData (_T("DM_ECO_NOTICE_LIB_CODE"),_T("LIB_NAME"),LibraryName  ,i) ;

	   	ids = AllInputLibraryInfo( LibraryCode, LibraryName, CenterParentRecKey );
		if ( ids<0 ) {
			if ( ids == 100 ) return;
			AfxMessageBox ( _T("입력을 실패했습니다."), MB_OK);
			return;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// 마지막으로 Center에 공지사항을 저장한다.
	/////////////////////////////////////////////////////////////////////////////////////////

	ids = AllInput( CenterParentRecKey, LocalLibraryCode, LocalLibraryName );
	

	

	MsqBuf.Format ( _T("전체 %d개 도서관에 공지사항을 보냈습니다."), RecCnt );
	MessageBox( MsqBuf, _T("도서관전체보내기"), MB_OK );

    CESL_ControlMgr *pCM = FindCM(_T("CM_ECO_NOTICE_INPUT"));
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	pCM->AllControlClear();
//	EndDialog(1);
}


INT CECO_NOTICE_MANAGER::AllInput( CString pParentRecKey, CString pLocalLibraryCode, CString pLocalLibraryName )
{
	CESL_DataMgr  * m_pDataMgr;
	
	CString         Subject, Contents;
	CString         CurrentDate;
	INT             RowCnt;

    CString AttrData [10];
    CString AttrName [10] = { _T("REC_KEY")         , // 레코드KEY
		                    _T("send_libid")      , // 보낸 도서관 코드
							_T("send_libname")    , // 보낸 도서관 명
							_T("recv_libid")      , // 받는 도서관 코드
							_T("recv_libname")    , // 받는 도서관 명
							_T("title")           , // 제목
							_T("contents")        , // 내용
							_T("confirm")         , // 확인
							_T("srflag")          , // 보낸공지사항
							_T("trans_date")      };
	CString    FieldType[10] = { _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. 입력항목을 체크한다.
	/////////////////////////////////////////////////////////////////////////////////////
	
	GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("제목"),Subject,-1) ;
	
	GetControlData (_T("CM_ECO_NOTICE_INPUT"),_T("내용"),Contents ,-1) ;


	if ( Subject.IsEmpty () || Contents.IsEmpty () ) {
		MessageBox( _T("데이타를 입력하십시오."), _T("입력항목체크"), MB_OK );
		return 100;
	}

	AttrData[0] = pParentRecKey;
	AttrData[1] = pLocalLibraryCode;
	AttrData[2] = pLocalLibraryName;
	AttrData[3] = _T("");
	AttrData[4] = _T("전체도서관");
	AttrData[5] = Subject;
	AttrData[6] = Contents;
	AttrData[7] = _T("0");

	/////////////////////////////////////////////////////////////////////////////////////
	// 3. Local 저장일때는 보낸 공지사항으로 설정
	/////////////////////////////////////////////////////////////////////////////////////
    if ( m_SERVER_TYPE == _T("LS") ) {
		AttrData[8] = _T("1");
	/////////////////////////////////////////////////////////////////////////////////////
	// 4. Center 저장일때는 받은 공지사항으로 설정
	/////////////////////////////////////////////////////////////////////////////////////
	} else {
		AttrData[8] = _T("1");
	}
   
	CTime t = CTime::GetCurrentTime ();
	CurrentDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;
	AttrData[9] = CurrentDate;

    m_pDataMgr = FindDM(_T("DM_ECO_NOTICE_INPUT")) ;
	if(m_pDataMgr == NULL) {
		AfxMessageBox (_T("FindDM() Error~!"));
		return - 5 ;
	}

    RowCnt = m_pDataMgr->GetRowCount ();

/*
	/////////////////////////////////////////////////////////////////////////////////////
	// 5. Local DB 저장일때는 Memory에도 저장
	/////////////////////////////////////////////////////////////////////////////////////
	if ( m_SERVER_TYPE == _T("LS") ) {
	
		for(INT i =0 ; i <10;i++) {
        m_pDataMgr->AddDBFieldData (AttrName[i],FieldType[i],AttrData[i]) ;
        }
		m_pDataMgr->MakeInsertFrame (_T("ECO_NOTICE_TBL"));
	   
	/////////////////////////////////////////////////////////////////////////////////////
	// 6. Center DB 저장일때는 DB만 저장
	/////////////////////////////////////////////////////////////////////////////////////
	} 
	else */
	if( m_SERVER_TYPE == _T("ES"))
	{
	   	 CRS_SQL m_rs_sql(this);
		 INT FieldType[10] = { 2,1,1,1,1,1,1,1,1,1};
		 m_rs_sql.ES_InsertRow (_T("ECO_NOTICE_TBL"),10,AttrName,AttrData,FieldType,TRUE) ;
	}

	return 0;
}

INT CECO_NOTICE_MANAGER::InitProgress (INT nRange)
{
	m_ProgressCtrl.SetRange (0,nRange) ;
	m_ProgressCtrl.SetPos (0) ;
    
	pCnt = (CStatic *)GetDlgItem(IDC_sCount) ;
	pCnt->SetWindowText (_T("진행상태: 0%"));

	return 0 ;
}

INT CECO_NOTICE_MANAGER::ChangeProgressCtrl(INT nRange)
{
	INT nLower, nUpper ;

	m_ProgressCtrl.GetRange (nLower,nUpper) ;
	if(nRange <= 0 ) nRange = 1 ;

	INT nPos = ((nRange*100)/nUpper) ;
	
	m_ProgressCtrl.SetPos (nRange) ;
    
    CString sPos ;
	sPos.Format (_T("진행상태 : %d(%d/%d)") , nPos,nRange,nUpper  );
    
    pCnt = (CStatic *)GetDlgItem(IDC_sCount) ;
	pCnt->SetWindowText (sPos);

	return 0 ;
}

VOID CECO_NOTICE_MANAGER::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	INT nIndex = m_pTabCtrl->GetCurSel();
	Display(nIndex);	
	*pResult = 0;
}

INT CECO_NOTICE_MANAGER::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	CString strTmpData, strPriceInfo, strPrice, strCurrencyCode;

	if(strDMFieldAlias == _T("확인"))
	{
		strTmpData = m_pDM->GetCellData(_T("확인"), nRow);
		
		if		( strTmpData == _T("0") )
		{
			str = _T("미확인");
		}
		else 
		{
			str = _T("확인");
		}
		m_pDM->SetCellData(_T("확인여부"), str, nRow);
	}

	return 0;
}

VOID CECO_NOTICE_MANAGER::OnCancel2() 
{
	EndDialog(1);	
}

INT CECO_NOTICE_MANAGER::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}
