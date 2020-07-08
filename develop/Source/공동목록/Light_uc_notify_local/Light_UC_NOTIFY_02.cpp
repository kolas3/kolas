// Light_UC_NOTIFY_02.cpp : implementation file
//

#include "stdafx.h"

#include "Light_UC_NOTIFY_02.h"
#include "UC_NOTIFY_LIB_CODE.h"
#include "..\check_dup_api\RS_SQL.h"
#include <direct.h>

// 2017/09/22 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
// KOLIS-NET OpenAPI
/*//NEW------------------------------------------------------------------------*/
#include "KolisNetService.h"
/*//END------------------------------------------------------------------------*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLight_UC_NOTIFY_02 dialog


CLight_UC_NOTIFY_02::CLight_UC_NOTIFY_02(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_FILE_INFO.FileCnt = 0;

	m_SelectLibCnt = 0;
	m_arSelectLib.FreeExtra() ;
	m_select_Lib_Code.FreeExtra();
	m_select_Lib_Name.FreeExtra() ;
	
	m_SERVER_TYPE = _T("");
	m_ParentRecKey = _T("");
	incfile = NULL;
	InsertCnt = 0;
	pCnt = NULL;
}

CLight_UC_NOTIFY_02::~CLight_UC_NOTIFY_02()
{
	
}

BOOL CLight_UC_NOTIFY_02::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CLight_UC_NOTIFY_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLight_UC_NOTIFY_02)
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLight_UC_NOTIFY_02, CDialog)
	//{{AFX_MSG_MAP(CLight_UC_NOTIFY_02)
	ON_BN_CLICKED(IDC_bIDOK, OnbIDOK)
	ON_BN_CLICKED(IDC_bSEARCH, OnbSEARCH)
	ON_BN_CLICKED(IDC_bINCLUDE_FILE, OnbINCLUDEFILE)
	ON_BN_CLICKED(IDC_bAPPEND, OnbAPPEND)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLight_UC_NOTIFY_02 message handlers

BOOL CLight_UC_NOTIFY_02::OnInitDialog() 
{
	CDialog::OnInitDialog();

    if(InitESL_Mgr(_T("SM_UC_NOTIFY_INPUT"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}
    m_SERVER_TYPE = _T("LS") ;

	// TODO: Add extra initialization here
	m_button1.AutoLoad( IDC_bINCLUDE_FILE, this );
    m_SelectLibCnt = 0 ;

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CLight_UC_NOTIFY_02::OnbIDOK() 
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

    m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_MAIN")) ;
    if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error"));
		return ;
	}

	LibraryCode = m_pInfo->LIB_CODE;
	LibraryName = m_pInfo->LIB_NAME;

	//////////////////////////////////////////////////////////////////////////////////
	// 1. center에 저장
	//////////////////////////////////////////////////////////////////////////////////

	for ( i = 0; i < m_SelectLibCnt; i++ ) {
		
		//////////////////////////////////////////////////////////////////////////////////
		// 2. 공지사항을 저장한다.
		//////////////////////////////////////////////////////////////////////////////////
				
		m_SERVER_TYPE =_T("RS") ;
        if(m_SERVER_TYPE == _T("RS"))
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
		
			// 2017/09/22 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
			// KOLIS-NET OpenAPI
			// 센터와 동일한 KEY로 사용
/*//BEFORE-----------------------------------------------------------------------
			ids = InsertFile_Proc( m_pDataMgr, LocalFileRecKey, ParentRecKey ,m_SERVER_TYPE );
*///AFTER------------------------------------------------------------------------
			ids = InsertFile_Proc(m_pDataMgr, LocalFileRecKey, m_ParentRecKey, m_SERVER_TYPE);
/*//END------------------------------------------------------------------------*/
			if ( ids<0 ) {
				AfxMessageBox( _T("LocalInsertFile_Proc() ERROR"), MB_OK);
				return;
			}

		}
/*
		tmpData.Format ( _T("%d/%d 진행중"), i+1,  );
		m_pProgressBar.SetWindowText( tmpData );
		m_pProgressBar.StepIt();
*/
	}



	ids = AllControlDisplay(_T("CM_UC_NOTIFY_MAIN"),-1);

	//m_pProgressBar.SetWindowText( _T("완료") );

    MessageBox( _T("공지사항을 보냈습니다."), _T("입력"), MB_OK );
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_UC_NOTIFY_INPUT"));
	pCM->AllControlClear () ;
	EndDialog(1);
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
INT CLight_UC_NOTIFY_02::Input( CString RecKey, INT index, CString LibraryCode, CString LibraryName )
{
	CESL_DataMgr  * m_pDataMgr;
	CString         Subject, Contents;
	CString         CurrentDate;
	// KOL.UDF.022 시큐어코딩 보완
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

    m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_MAIN")) ;
	if(m_pDataMgr == NULL){
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// 1. 입력항목을 체크한다.
	/////////////////////////////////////////////////////////////////////////////////////
	ids = GetControlData (_T("CM_UC_NOTIFY_INPUT"),_T("제목"),Subject,-1) ;
	ids = GetControlData (_T("CM_UC_NOTIFY_INPUT"),_T("내용"),Contents,-1) ;
	
	
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
	if(m_SERVER_TYPE == _T("LS"))
	{
		// 2017/09/22 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// KOLIS-NET OpenAPI
		// 센터와 동일한 값으로 생성
/*//BEFORE-----------------------------------------------------------------------
		m_pDataMgr->MakeRecKey (RecKey) ;
		m_ParentRecKey = RecKey ;
*///AFTER------------------------------------------------------------------------
		RecKey = m_ParentRecKey;
/*//END------------------------------------------------------------------------*/
	}
    else
	{
		// 2017/09/22 : 2017년 공공도서관 책이음서비스 확대구축 및 시스템개선
		// KOLIS-NET OpenAPI
/*//BEFORE-----------------------------------------------------------------------
		CRS_SQL m_rs_sql(this) ;
        CString sqlBuffer;
		CString temp ;
		sqlBuffer.Format (_T("select max(rec_key) from uc_notify_tbl")) ;
		m_rs_sql.GetOneFieldValue (sqlBuffer,temp);
		
        INT itmp ;
		itmp = _ttoi(temp.GetBuffer (0)) ;
		RecKey.Format (_T("%d"),itmp+1) ;

		m_ParentRecKey = RecKey ;
*///AFTER------------------------------------------------------------------------
//		CRS_SQL rsSql(this);
//		if (0 > rsSql.MakePrimaryKey(RecKey)) return -1;
//		m_ParentRecKey = RecKey ;
/*//END------------------------------------------------------------------------*/
	}


	AttrData[0] = RecKey;
	AttrData[1] = LibraryCode;
	AttrData[2] = LibraryName;
	AttrData[3] = m_select_Lib_Code.GetAt (index);
	AttrData[4] = m_select_Lib_Name.GetAt (index);
	AttrData[5] = Subject;
	AttrData[6] = Contents;
	AttrData[7] = _T("0");	

	AttrData[8] = _T("1");

    CTime t = CTime::GetCurrentTime () ;
	CurrentDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
	AttrData[9] = CurrentDate;
     
    RowCnt ;

	/////////////////////////////////////////////////////////////////////////////////////
	// 4. Local DB 저장일때는 Momory도 Insert 함
	/////////////////////////////////////////////////////////////////////////////////////
	if ( _T("LS") == m_SERVER_TYPE  ) {	

		m_pDataMgr->MakeDeleteFrame(_T("UC_NOTIFY_TBL"), _T("REC_KEY"), _T("NUMERIC"), RecKey);
		m_pDataMgr->MakeDeleteFrame(_T("UC_NOTIFY_RECV_LIBRARY_TBL"), _T("PARENT_KEY"), _T("NUMERIC"), RecKey);
		m_pDataMgr->MakeDeleteFrame(_T("UC_NOTIFY_FILE_TBL"), _T("PARENT_KEY"), _T("NUMERIC"), RecKey);

		for(INT i =0 ; i <10;i++) {	
        m_pDataMgr->AddDBFieldData (AttrName[i],FieldType[i],AttrData[i]) ;
        }

		ids = m_pDataMgr->MakeInsertFrame (_T("UC_NOTIFY_TBL")) ;
	/////////////////////////////////////////////////////////////////////////////////////
	// 5. center DB 저장일때는 Momory도 Insert 
	/////////////////////////////////////////////////////////////////////////////////////
	} else {
		
		if (!CKolisNetService::MakeKolisNetNotify(this, AttrData[0], AttrData[1], AttrData[2], AttrData[3], AttrData[4], AttrData[5], AttrData[6], AttrData[7], AttrData[8]))
		{
			AfxMessageBox(_T("KOLIS-NET OpenAPI 공지사항 입력에 실패하였습니다."));
			return -1;
		}
	}

	return 0;
}

VOID CLight_UC_NOTIFY_02::OnbSEARCH() 
{
	// TODO: Add your control notification handler code here
	
	
	CComboBox * pCombo = (CComboBox*)GetDlgItem(IDC_eRECEIPT_LIB) ;
	CUC_NOTIFY_LIB_CODE dlg(this) ;
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

INT CLight_UC_NOTIFY_02::AllInputLibraryInfo(CString pLibraryCode, CString pLibraryName, CString pParentRecKey)
{

	CString strRecKey;
	if (!CKolisNetService::GetSequenceNo(this, strRecKey))
	{
		return -1;
	}

	if (!CKolisNetService::MakeKolisNetNotifyRecvLib(this, pParentRecKey, strRecKey, pLibraryCode, pLibraryName))
	{
		AfxMessageBox(_T("KOLIS-NET OpenAPI 공지사항 입력에 실패하였습니다."));
		return -2;
	}
	return 0;
}

VOID CLight_UC_NOTIFY_02::OnbINCLUDEFILE() 
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


INT CLight_UC_NOTIFY_02::InsertFile_Proc( CESL_DataMgr * pDataMgr, CString pLocalFileRecKey, CString pParentRecKey ,CString ServerType ) 
{
	INT  i, ids;
    

    if(ServerType == _T("RS"))
    {
		CString strResult;
		for (i = 0; i < m_FILE_INFO.FileCnt; i++)
		{
			if (!CKolisNetService::UploadNotifyFile(this, _T("UC"), pParentRecKey, m_FILE_INFO.FileNameBuf[i], m_FILE_INFO.FilePathBuf[i], strResult))
			{
				return -1;
			}
		}
	}
	else if(ServerType == _T("LS"))
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

		    pDataMgr->MakeInsertFrame (_T("UC_NOTIFY_FILE_TBL"));
			pDataMgr->SendFrame ();
			pDataMgr->EndFrame () ;
		
		}

	}

	return 0;
}
VOID CLight_UC_NOTIFY_02::OnbAPPEND() 
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
	
	GetControlData (_T("CM_UC_NOTIFY_INPUT"),_T("제목"),Subject,-1) ;
	GetControlData (_T("CM_UC_NOTIFY_INPUT"),_T("내용"),Contents,-1) ;


	if ( Subject.IsEmpty () || Contents.IsEmpty () ) {
		MessageBox( _T("데이타를 입력하십시오."), _T("입력항목체크"), MB_OK );
		return;
	}

	////////////////////////////////////////////////////////////////////////////////////
	// 1. 전체도서관 정보를 구한다.
	////////////////////////////////////////////////////////////////////////////////////
	
	m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_LIB_CODE")) ;
	if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}
	
	m_SERVER_TYPE = _T("RS") ;


	CKolisNetDataMapper Mapper;
	Mapper.AddData(KOLISNET_PARAM_LIB_CODE, _T("LIB_ID"));
	Mapper.AddData(KOLISNET_PARAM_LIB_NAME, _T("NAME"));
	Mapper.AddData(KOLISNET_PARAM_LIB_KEY, _T("REC_KEY"));

	if (!CKolisNetService::GetLibrariesInfo(this, m_pDataMgr, &Mapper))
	{
		AfxMessageBox(_T("KOLIS-NET OpenAPI 도서관 목록 조회를 실패하였습니다."));
		return;
	}

	if (RecCnt == 0)
	{
		MessageBox(_T("도서관정보가 없습니다."), _T("도서관전체보내기"), MB_OK);
		return;
	}

	m_SERVER_TYPE = _T("LS");

	LocalLibraryCode = m_pInfo->LIB_CODE;
/*//END------------------------------------------------------------------------*/

	MsqBuf.Format ( _T("전체 %d개 도서관에 위 공지사항을 보내시겠습니까?"), RecCnt );
	
	ids = MessageBox( MsqBuf, _T("도서관전체보내기"), MB_OKCANCEL|MB_ICONQUESTION );
	if ( IDCANCEL == ids ) return;

    m_pControlMgr = FindCM(_T("CM_UC_NOTIFY_INPUT")) ;
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
    m_SERVER_TYPE = _T("RS") ;
     

	if(m_SERVER_TYPE == _T("LS"))
	{
		CString RecKey ;
		m_pDataMgr->MakeRecKey (RecKey) ;
		ParentRecKey = RecKey ;
	}
    else
	{		
		CKolisNetService::GetSequenceNo(this, ParentRecKey);
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
	m_SERVER_TYPE = _T("LS") ;


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

	/////////////////////////////////////////////////////////////////////////////////////////
	// 6. 보낸도서관 정보를 Center에 저장
	/////////////////////////////////////////////////////////////////////////////////////////
	m_SERVER_TYPE = _T("RS") ;


	for ( i = 0; i < RecCnt; i++ ) {
	     
		 ChangeProgressCtrl(i) ;

		 GetDataMgrData (_T("DM_UC_NOTIFY_LIB_CODE"),_T("도서관ID"),LibraryCode ,i) ;
         GetDataMgrData (_T("DM_UC_NOTIFY_LIB_CODE"),_T("도서관명"),LibraryName  ,i) ;

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

    CESL_ControlMgr *pCM = FindCM(_T("CM_UC_NOTIFY_INPUT"));
	if(pCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!"));
		return ;
	}

	pCM->AllControlClear();
	EndDialog(1);


}


INT CLight_UC_NOTIFY_02::AllInput( CString pParentRecKey, CString pLocalLibraryCode, CString pLocalLibraryName )
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
	
	GetControlData (_T("CM_UC_NOTIFY_INPUT"),_T("제목"),Subject,-1) ;
	
	GetControlData (_T("CM_UC_NOTIFY_INPUT"),_T("내용"),Contents ,-1) ;


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

	AttrData[8] = _T("1");

    m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_MAIN")) ;
	if(m_pDataMgr == NULL) {
		AfxMessageBox (_T("FindDM() Error~!"));
		return - 5 ;
	}

    RowCnt = m_pDataMgr->GetRowCount ();


	/////////////////////////////////////////////////////////////////////////////////////
	// 5. Local DB 저장일때는 Memory에도 저장
	/////////////////////////////////////////////////////////////////////////////////////
	if ( m_SERVER_TYPE == _T("LS") ) {
	
		for(INT i =0 ; i <10;i++) {
        m_pDataMgr->AddDBFieldData (AttrName[i],FieldType[i],AttrData[i]) ;
        }
		m_pDataMgr->MakeInsertFrame (_T("UC_NOTIFY_TBL"));
	   
	/////////////////////////////////////////////////////////////////////////////////////
	// 6. Center DB 저장일때는 DB만 저장
	/////////////////////////////////////////////////////////////////////////////////////
	} 
	else 
	{
		if (!CKolisNetService::MakeKolisNetNotify(this, AttrData[0], AttrData[1], AttrData[2], AttrData[3], AttrData[4], AttrData[5], AttrData[6], AttrData[7], AttrData[8]))
		{
			AfxMessageBox(_T("KOLIS-NET OpenAPI 공지사항 입력에 실패하였습니다."));
			return -1;
		}
	}

	return 0;
}


INT CLight_UC_NOTIFY_02::InitProgress (INT nRange)
{
	m_ProgressCtrl.SetRange (0,nRange) ;
	m_ProgressCtrl.SetPos (0) ;
    
	pCnt = (CStatic *)GetDlgItem(IDC_sCount) ;
	pCnt->SetWindowText (_T("진행상태: 0%"));

	return 0 ;
}

INT CLight_UC_NOTIFY_02::ChangeProgressCtrl(INT nRange)
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
HBRUSH CLight_UC_NOTIFY_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
