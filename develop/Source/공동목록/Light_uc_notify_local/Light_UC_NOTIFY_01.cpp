// Light_UC_NOTIFY_01.cpp : implementation file
//

#include "stdafx.h"

#include "Light_UC_NOTIFY_01.h"
#include "..\check_dup_api\RS_SQL.h"
#include "Light_UC_NOTIFY_DETAIL.h"

#include "KolisNetService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLight_UC_NOTIFY_01 dialog


CLight_UC_NOTIFY_01::CLight_UC_NOTIFY_01(CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// KOL.UDF.022 시큐어코딩 보완
	m_pTabCtrl = NULL;
	m_CurrentDM = _T("");
	m_CurrentCM = _T("");
	m_pGrid = NULL;
	m_SERVER_TYPE = _T("");
}

CLight_UC_NOTIFY_01::~CLight_UC_NOTIFY_01()
{
	
}

BOOL CLight_UC_NOTIFY_01::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CLight_UC_NOTIFY_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLight_UC_NOTIFY_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLight_UC_NOTIFY_01, CDialog)
	//{{AFX_MSG_MAP(CLight_UC_NOTIFY_01)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLight_UC_NOTIFY_01 message handlers

BOOL CLight_UC_NOTIFY_01::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    if(InitESL_Mgr(_T("SM_UC_NOTIFY_MAIN"))<0)
	{
		AfxMessageBox (_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

    m_CurrentDM = _T("DM_UC_NOTIFY_MAIN") ;
    m_CurrentCM = _T("CM_UC_NOTIFY_MAIN") ;


	m_pTabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB);
	m_pTabCtrl->InsertItem (0,_T("받은공지사항"));
	m_pTabCtrl->InsertItem (1,_T("보낸공지사항"));
	m_pTabCtrl->SetCurSel (0) ;
   
	INT ids ;

    ids = SelectDataMgr(_RECEIPT_NOTIFY) ;

	EnableThemeDialogTexture(GetSafeHwnd());
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CLight_UC_NOTIFY_01::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MainGrid_notify);
	if (pGrid->GetSafeHwnd() == NULL) return;
    pGrid->MoveWindow(0, 0, cx, cy);

}

INT CLight_UC_NOTIFY_01::SelectDataMgr(INT Flag)
{
	CESL_DataMgr     * pDataMgr;
	CString            SqlBuf;
	INT                ids, RecCnt;

    pDataMgr = FindDM(m_CurrentDM) ;
	if(pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}

	SqlBuf.Format( _T("srflag = '%d'"), Flag );
	ids = pDataMgr->RefreshDataManager (SqlBuf);

    RecCnt = pDataMgr->GetRowCount () ;

	ids = AllControlDisplay(m_CurrentCM,-1) ;
  
	ids = GridDataDisplay_To_FontBold();

	return 0;
}

INT CLight_UC_NOTIFY_01::GridDataDisplay_To_FontBold()
{
    CESL_Grid        * pGrid ;
	CTabCtrl         * pabCtrl;
	CFont              Font ;
	CString            Conform;
	INT                i, RecCnt, CurSelIdx;
	//////////////////////////////////////////////////////////////////////////////////
	// 3. 읽지않은 공지사항의 그리드데이타를 볼드체로 만든다.
	//////////////////////////////////////////////////////////////////////////////////
    pabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB);
	if ( pabCtrl == NULL ) return -1;
    CurSelIdx = pabCtrl->GetCurSel();
    
    CESL_ControlMgr *pCM = FindCM(m_CurrentCM );

	pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid"));

     

    CESL_DataMgr *pDM = FindDM(m_CurrentDM );
    RecCnt = pDM->GetRowCount () ;

	if ( CurSelIdx == 0 ) {
		for ( i = 0; i < RecCnt; i++ ) {
			pDM->GetCellData (_T("확인"),i,Conform );
			if ( Conform == _T("0") ) {
                pGrid->SetRow(i+1);
				for(INT j = 0 ; j < 5 ;j++)
				{
					pGrid->SetCol(j+1);
					pGrid->SetCellFontBold (TRUE) ;
					pGrid->SetCellForeColor (RGB( 0, 80, 255 ) );
				}
			}
		}
	}
	return 0;
}

INT CLight_UC_NOTIFY_01::DisplayBaseScreen_To_SelfLibraryInfo()
{
   
	CString        LibraryCode, LibraryName, Buffer;

    CRS_SQL m_rs_sql(this);
	m_rs_sql.GetLibraryInfo (LibraryCode ) ;
    
	m_rs_sql.GetLibraryName (LibraryName ) ;


	Buffer.Format ( _T("자관도서관명   : %s\n자관도서관부호 : %s"), LibraryName, LibraryCode );

	GetDlgItem(IDC_PROS)->SetWindowText( Buffer );

	return 0 ;
}

VOID CLight_UC_NOTIFY_01::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTabCtrl * pabCtrl;
	INT        CurSelIdx, ids;

	////////////////////////////////////////////////////////////////////////////////////
	// 1. 선택한 탭의 Index를 구한다.
	////////////////////////////////////////////////////////////////////////////////////
    pabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB);
	if ( pabCtrl == NULL ) {
		AfxMessageBox( _T("pabCtrl의 NULLPOINT ERROR발생"), MB_ICONEXCLAMATION);
		return;
	}
    CurSelIdx = pabCtrl->GetCurSel();
	////////////////////////////////////////////////////////////////////////////////////
	// 2. CurSelIdx == 0 이면 받은공지사항을 보여준다.
	////////////////////////////////////////////////////////////////////////////////////
	if ( CurSelIdx == 0 ) {
		ids = SelectDataMgr( _RECEIPT_NOTIFY );
		if ( ids<0 ) {
			AfxMessageBox ( _T("SelectDataMgr(_RECEIPT_NOTIFY) Error"), MB_ICONEXCLAMATION);
			return;
		}
	////////////////////////////////////////////////////////////////////////////////////
	// 3. CurSelIdx == 1 이면 보낸공지사항을 보여준다.
	////////////////////////////////////////////////////////////////////////////////////
	} else if ( CurSelIdx == 1 ) {
		ids = SelectDataMgr( _SEND_NOTIFY );
		if ( ids<0 ) {
			AfxMessageBox ( _T("SelectDataMgr(_SEND_NOTIFY) Error"), MB_ICONEXCLAMATION);
			return;
		}
	}
	*pResult = 0;
}

VOID CLight_UC_NOTIFY_01::SubRECEIPTNO()
{
	CESL_DataMgr     * m_pDataMgr;
	CTabCtrl         * pabCtrl;
	CString            LibraryCode, SqlBuf, LibraryName, MsgBuf;
	CString            RecKey;
	INT                ids, Cnt, i;

	LibraryCode = m_pInfo->LIB_CODE;

	CString strKolisNetUseFlag;
	CString strDirectUploadFlag;

	if (!CKolisNetService::GetLibraryInfo(this, LibraryCode, LibraryName, strKolisNetUseFlag, strDirectUploadFlag))
	{
		AfxMessageBox(_T("KOLIS-NET OpenAPI 결과 수집에 실패하였습니다."));
		return;
	}

	m_SERVER_TYPE = _T("RS") ;

	m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_DOWN"));
	if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}
	
	if (strKolisNetUseFlag == _T("N") || strKolisNetUseFlag == _T("")) 
	{
		AfxMessageBox(_T("공동목록 서버의 접근 권한이 없습니다.\n국립중앙도서관의 정보화담당관실에 문의하십시오."));
		return;
	}	

	m_pDataMgr->FreeData();
	CKolisNetDataMapper Mapper;
	Mapper.AddData(KOLISNET_PARAM_CONFIRM, _T("확인"));
	Mapper.AddData(KOLISNET_PARAM_UN_KEY, _T("REC_KEY"));
	Mapper.AddData(KOLISNET_PARAM_CONTENTS, _T("내용"));
	Mapper.AddData(KOLISNET_PARAM_SEND_LIB_CODE, _T("보낸도서관부호"));
	Mapper.AddData(KOLISNET_PARAM_SEND_LIB_NAME, _T("보낸도서관명"));
	Mapper.AddData(KOLISNET_PARAM_SRFLAG, _T("받은보낸공지사항"));
	Mapper.AddData(KOLISNET_PARAM_TITLE, _T("제목"));
	Mapper.AddData(KOLISNET_PARAM_TRANS_DATE, _T("전송날짜"));
	Mapper.AddData(KOLISNET_PARAM_RECV_LIB_CODE, _T("받은도서관부호"));
	Mapper.AddData(KOLISNET_PARAM_RECV_LIB_NAME, _T("받은도서관명"));

	if (!CKolisNetService::GetKolisNetNotify(this, m_pInfo->LIB_CODE, m_pDataMgr, &Mapper))
	{
		AfxMessageBox(_T("KOLIS-NET OpenAPI 결과 수집에 실패하였습니다."));
		return;
	}
	Cnt = m_pDataMgr->GetRowCount () ;


	if ( Cnt == 0 ) {
		SqlBuf.Format ( _T("[%s]에 해당하는 공지사항이 없습니다."), LibraryName );
		MessageBox( SqlBuf, _T("받기"), MB_OK|MB_ICONINFORMATION );
		m_SERVER_TYPE = _T("LS") ;
		return;
	} else {
		MsgBuf.Format ( _T("[%s]에 해당하는 공지사항이[%d]건 있습니다.\n ")
						 _T("DownLoading 하시겠습니까? "), LibraryName, Cnt );
		ids = MessageBox( MsgBuf, _T("받기"), MB_OKCANCEL|MB_ICONQUESTION );
		if ( ids == IDOK ) {
			pabCtrl = (CTabCtrl *)GetDlgItem(IDC_TAB);
			if ( pabCtrl == NULL ) {
				AfxMessageBox ( _T("pabCtrl의 NULLPOINT ERROR발생"), MB_ICONEXCLAMATION);
				return;
			}
			pabCtrl->SetCurSel( 0 );

			for ( i = 0; i < Cnt; i++ ) {
				m_SERVER_TYPE = _T("LS") ;
				
				ids = AllInput( i );
				if ( ids<0 ) {
					AfxMessageBox (_T("AllInput() Error"), MB_ICONEXCLAMATION);
					return;
				}
                
				ids = m_pDataMgr->GetCellData (_T("REC_KEY"),i,RecKey) ;
				

			}

			ids = SelectDataMgr( _RECEIPT_NOTIFY );
			if ( ids<0 ) {
				AfxMessageBox (_T("SelectDataMgr(_RECEIPT_NOTIFY) Error"), MB_ICONEXCLAMATION);
				return;
			}
		}
	}
}

INT CLight_UC_NOTIFY_01::AllInput(INT RowIndex)
{
	CESL_DataMgr  * m_pDataMgr;
	
	CString         RecKey, Subject, Contents, ParentRecKey;
	CString         CurrentDate, CenterLibraryCode, CenterLibraryName;
	CString         pLibraryCode, pLibraryName;
	INT             ids, RowCnt, FileInfoCnt;

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
	CString  FieldType[10] = { _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING"), _T("STRING") };
    //INT    FieldType[10] = {2,1,1,1,1,1,1,1,1,1} ;
	/////////////////////////////////////////////////////////////////////////////////////
	// 입력항목을 체크한다.
	/////////////////////////////////////////////////////////////////////////////////////
	
	ids = GetDataMgrData (_T("DM_UC_NOTIFY_DOWN"),_T("보낸도서관부호"),CenterLibraryCode ,RowIndex) ;
	ids = GetDataMgrData (_T("DM_UC_NOTIFY_DOWN"),_T("보낸도서관명"),CenterLibraryName ,RowIndex) ;
    ids = GetDataMgrData (_T("DM_UC_NOTIFY_DOWN"),_T("받은도서관부호"),pLibraryCode ,RowIndex) ;
    ids = GetDataMgrData (_T("DM_UC_NOTIFY_DOWN"),_T("받은도서관명"),pLibraryName ,RowIndex) ;
    ids = GetDataMgrData (_T("DM_UC_NOTIFY_DOWN"),_T("제목"),Subject ,RowIndex) ;
    ids = GetDataMgrData (_T("DM_UC_NOTIFY_DOWN"),_T("내용"),Contents ,RowIndex) ;
    ids = GetDataMgrData (_T("DM_UC_NOTIFY_DOWN"),_T("전송날짜"),CurrentDate ,RowIndex) ;

    m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_MAIN")) ;


	GetDataMgrData(_T("DM_UC_NOTIFY_DOWN"), _T("REC_KEY"), RecKey, RowIndex);

	AttrData[0] = RecKey;
	AttrData[1] = CenterLibraryCode;
	AttrData[2] = CenterLibraryName;
	AttrData[3] = pLibraryCode;
	AttrData[4] = pLibraryName;
	AttrData[5] = Subject;
	AttrData[6] = Contents;
	AttrData[7] = _T("0");
	AttrData[8] = _T("0");
	AttrData[9] = CurrentDate;

	/////////////////////////////////////////////////////////////////////////////////////
	// DB에 UPDATE한다.
	/////////////////////////////////////////////////////////////////////////////////////
	RowCnt = m_pDataMgr->GetRowCount () ;

    m_pDataMgr->StartFrame () ;

	m_pDataMgr->MakeDeleteFrame(_T("UC_NOTIFY_TBL"), _T("REC_KEY"), _T("NUMERIC"), RecKey);
	m_pDataMgr->MakeDeleteFrame(_T("UC_NOTIFY_RECV_LIBRARY_TBL"), _T("PARENT_KEY"), _T("NUMERIC"), RecKey);
	m_pDataMgr->MakeDeleteFrame(_T("UC_NOTIFY_FILE_TBL"), _T("PARENT_KEY"), _T("NUMERIC"), RecKey);

	m_pDataMgr->InitDBFieldData () ;
    for(INT i = 0; i<10 ;i++)
	{
		m_pDataMgr->AddDBFieldData (AttrName[i],FieldType [i],AttrData[i]);

	}
	m_pDataMgr->MakeInsertFrame (_T("UC_NOTIFY_TBL")) ;
    m_pDataMgr->SendFrame ();
	m_pDataMgr->EndFrame () ;
	

	ids = GetDataMgrData (_T("DM_UC_NOTIFY_DOWN"),_T("REC_KEY"),ParentRecKey,RowIndex) ;

	ids = InsertFileInfo( RecKey, ParentRecKey, &FileInfoCnt );
	if ( ids<0 ) return -14;



	return 0;
}

INT CLight_UC_NOTIFY_01::InsertFileInfo( CString LocalCreateReckey, CString pParentRecKey, INT * RowCnt ) 
{
	CESL_DataMgr * pDataMgr;
	CString        RecKey, SqlBuf, FileName, FileSize;
	INT            i, ids;
	

	CString strURL;
	CString AttrData [5];
    CString AttrName [5] = { 
		_T("REC_KEY")     ,
		_T("PARENT_KEY")  ,
		_T("FILENAME")    ,
		_T("FILE_SIZE")	  ,
		_T("URL") };
	CString FieldType[5] = { _T("NUMERIC"), _T("NUMERIC"), _T("STRING"), _T("STRING"), _T("STRING") };

	pDataMgr = FindDM(_T("DM_UC_NOTIFY_FILEINFO"));
	if(pDataMgr == NULL) {
			AfxMessageBox(_T("FindDM() Error~!")) ;
			return -1 ;
	}

    m_SERVER_TYPE = _T("RS") ;

	CKolisNetDataMapper Mapper;
	Mapper.AddData(KOLISNET_PARAM_FILE_NAME, _T("파일이름"));
	Mapper.AddData(KOLISNET_PARAM_FILE_SIZE, _T("파일사이즈"));
	Mapper.AddData(KOLISNET_PARAM_UN_KEY, _T("PARENT_KEY"));
	Mapper.AddData(KOLISNET_PARAM_UNF_KEY, _T("REC_KEY"));
	Mapper.AddData(KOLISNET_PARAM_FILE_URL, _T("URL"));

	pDataMgr->FreeData();
	if (!CKolisNetService::GetKolisNetNotifyFileInfo(this, pParentRecKey, pDataMgr, &Mapper))
	{
		AfxMessageBox(_T("KOLIS-NET OpenAPI 결과 수집에 실패하였습니다."));
		return -1;
	}
     
    m_SERVER_TYPE = _T("LS") ;

	*RowCnt = pDataMgr->GetRowCount();
	
	pDataMgr->StartFrame ();
    

	if ( *RowCnt == 0 ) return 0;
	for ( i = 0; i < *RowCnt; i++ ) {
	
		pDataMgr->InitDBFieldData ();
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"),_T("REC_KEY"),RecKey,i) ;
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"),_T("파일이름"),FileName,i) ;
        ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"),_T("파일사이즈"),FileSize,i) ;
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"),_T("URL"),strURL,i) ;

		AttrData[0] = _T("ESL_SEQ.NEXTVAL");
		AttrData[1] = LocalCreateReckey;
		AttrData[2] = FileName;
		AttrData[3] = FileSize;
		AttrData[4] = strURL;

		for(INT j =0 ; j< 5 ; j++)
		{
			pDataMgr->AddDBFieldData (AttrName[j],FieldType[j],AttrData[j]);
        }

		pDataMgr->MakeInsertFrame (_T("UC_NOTIFY_FILE_TBL"));
		
	}
    pDataMgr->SendFrame () ;
	pDataMgr->EndFrame ();

	return 0;
}
/*
INT CLight_UC_NOTIFY_01::InsertServertoFile(INT FileInfoRowCnt)
{
	CESL_DataMgr * pDataMgr;
	CString        RecKey, FileName, FullpathName;
	INT            i, ids;
    
    pDataMgr = FindDM(_T("DM_UC_NOTIFY_FILEINFO"));
	if(pDataMgr == NULL){
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1 ;
	}

    
    m_SERVER_TYPE =_T("LS");
	for ( i = 0; i < FileInfoRowCnt; i++ ) {
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"), _T("REC_KEY"),RecKey,i);	
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_FILEINFO"), _T("파일이름"),FileName,i);
	
        
		m_SERVER_TYPE =_T("RS");
	    FullpathName.Format (_T("c:\\%s"), FileName );
		
		ids = RS_FetchFile(
					_T("UC_NOTIFY_FILE_TBL")  ,
					RecKey.GetBuffer (0)  ,
					FileName.GetBuffer (0),
					FullpathName.GetBuffer (0),
					1
			   );



		m_SERVER_TYPE =_T("LS");
        ids = RS_InsertFile( 
							_T("UC_NOTIFY_FILE_TBL")  ,
							 RecKey.GetBuffer (0) ,
							 FileName.GetBuffer (0), 
							 FullpathName.GetBuffer (0),    
							 0
							 ) ;

	    
		DeleteFile( _T("UC_NOTIFY_FILE_TBL"),FullpathName,RecKey );
	}


	return 0;
}

INT CLight_UC_NOTIFY_01::DeleteFile(CString TableName,CString FileName , CString RecKey)//테이블명,파일명,PrimaryKey
{

	 INT ids ;
     if ( FileName == NULL ) 
	 {
		 ids = RS_DeleteFile(
									TableName.GetBuffer (0)  ,
									RecKey.GetBuffer (0)     ,
									_T("*")           ,
									0
							   );
		 if(ids<0) return -1;
     } 
	 else 
	 {
	    ids = RS_DeleteFile(
							    TableName.GetBuffer (0)  ,
							    RecKey.GetBuffer (0)     ,
							    FileName.GetBuffer (0)   ,
							    0
					       );
		if(ids<0) return -2;
	 }

    return 0 ;
}*/

INT  CLight_UC_NOTIFY_01::DeleteSendLibraryInfo( CESL_DataMgr * pDataMgr, CString pParentRecKey, CString pLibraryCode )
{
  
	if (!CKolisNetService::DeleteKolisNetNotifyRecvLib(this, pParentRecKey, pLibraryCode))
	{
		return -1;
	}
	return 0;
}


VOID CLight_UC_NOTIFY_01::SubDelete()
{
	CESL_DataMgr * m_pDataMgr;
	INT            ids, RowCnt, SelectCnt;

	////////////////////////////////////////////////////////////////////////////////////
	// 1. 삭제할데이타가 있는지 체크한다.
	////////////////////////////////////////////////////////////////////////////////////
	
	m_pDataMgr = FindDM(_T("DM_UC_NOTIFY_MAIN"));
	if(m_pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;
	}
	
	RowCnt = m_pDataMgr->GetRowCount();
	if ( RowCnt == 0 ) {
		MessageBox( _T("삭제할 데이타가 없습니다."), _T("삭제"), MB_OK );
		return;
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	// 2. Grid 선택정보를 만든다.
	///////////////////////////////////////////////////////////////////////////////////////////
	
	CESL_ControlMgr *pCM = FindCM(_T("CM_UC_NOTIFY_MAIN"));
	if(pCM == NULL) {
		AfxMessageBox (_T("FindCM() Error~!"));
		return ;
	}

	m_pGrid = (CESL_Grid*)pCM->FindControl (_T("MainGrid"));
	if(m_pGrid == NULL) {
		AfxMessageBox(_T("FindControl() Error~!"));
		return ;
	}
	
	m_pGrid->SelectMakeList ();
	SelectCnt = m_pGrid->SelectGetCount ();
	
	if ( SelectCnt < 0 ) return;
	if ( SelectCnt == 0 ) {
		MessageBox( _T("삭제할 데이타를 선정하십시오."), _T("삭제"), MB_OK );
		return;
	}

	ids = MessageBox( _T("선택한 레코드를 삭제 하시겠습니까?"), _T("삭제"), MB_OKCANCEL|MB_ICONQUESTION );
	if ( ids == IDOK ) {

		///////////////////////////////////////////////////////////////////////////////////////////
		// 3. 삭제작업을 한다.
		///////////////////////////////////////////////////////////////////////////////////////////
		ids = DeleteProc();
		if ( ids <0) {
			AfxMessageBox ( _T("삭제를 실패하였습니다."), MB_ICONEXCLAMATION);
			return;
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		// 4. 화면을 초기화한다.
		///////////////////////////////////////////////////////////////////////////////////////////
	//	ScreenClear();
		pCM->AllControlDisplay(-1);
		MessageBox( _T("레코드가 삭제 되었습니다."), _T("삭제"), MB_OK );
	}	

	///////////////////////////////////////////////////////////////////////////////////////////
	// 5. 선택정보를 Free한다.
	///////////////////////////////////////////////////////////////////////////////////////////
    //AllControlDisplay (m_CurrentCM ,-1);
}

INT CLight_UC_NOTIFY_01::DeleteProc()
{
	CESL_DataMgr * pDataMgr, * pDataFileMgr;
	CString        RecKey, SqlBuf, NotifyRecKey, FileName;
	INT            ids, RecordIdx, SelectCnt, i, j, RecCnt;

	///////////////////////////////////////////////////////////////////////////////////////////
	// 1. 선택갯수를 구한다.
	///////////////////////////////////////////////////////////////////////////////////////////
	SelectCnt = m_pGrid->SelectGetCount ();
	if ( SelectCnt < 0 ) return -1;

	///////////////////////////////////////////////////////////////////////////////////////////
	// 2. Local Server를 설정한다.
	///////////////////////////////////////////////////////////////////////////////////////////
	
	pDataMgr = FindDM(_T("DM_UC_NOTIFY_MAIN"));
	if(pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -2 ;
	}
	
	m_SERVER_TYPE = _T("LS");

	///////////////////////////////////////////////////////////////////////////////////////////
	// 3. 선택정보의 Index를 마지막으로 되돌린다.
	///////////////////////////////////////////////////////////////////////////////////////////
    pDataFileMgr = FindDM(_T("DM_UC_NOTIFY_FILEINFO"));
	if(pDataFileMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return - 4 ;
	}
      
	///////////////////////////////////////////////////////////////////////////////////////////
	// 4. 선택갯수만큼 돌면서 Record를 삭제한다.
	///////////////////////////////////////////////////////////////////////////////////////////
	
    pDataMgr->StartFrame();
    
	for ( i = 0; i < SelectCnt; i++ ) {
	    pDataMgr->InitDBFieldData () ;
		///////////////////////////////////////////////////////////////////////////////////////////
		// 4.1 RecordIndex를 구한다.
		///////////////////////////////////////////////////////////////////////////////////////////
		RecordIdx = m_pGrid->SelectGetIdx ();
		if ( RecordIdx < 0 ) return -6;

		///////////////////////////////////////////////////////////////////////////////////////////
		// 4.2 RecordIndex의 RecKey를 구한다.
		///////////////////////////////////////////////////////////////////////////////////////////
		ids = GetDataMgrData (_T("DM_UC_NOTIFY_MAIN"),_T("REC_KEY"),NotifyRecKey ,RecordIdx );
		
		///////////////////////////////////////////////////////////////////////////////////////////
		// 4.3 공지사항을 삭제한다.
		///////////////////////////////////////////////////////////////////////////////////////////
		ids = pDataMgr->MakeDeleteFrame (_T("UC_NOTIFY_TBL"),_T("REC_KEY"),_T("NUMERIC"),NotifyRecKey) ;
		if ( ids<0 ) return -8;

		///////////////////////////////////////////////////////////////////////////////////////////
		// 4.4 공지사항에 해당하는 파일정보를 구한다.
		///////////////////////////////////////////////////////////////////////////////////////////
		SqlBuf.Format ( _T("PARENT_KEY = %s"), NotifyRecKey );
		ids = pDataFileMgr->RefreshDataManager (SqlBuf);
		if ( ids<0 ) return -9;

		///////////////////////////////////////////////////////////////////////////////////////////
		// 4.5 DB파일정보를 삭제한다.
		///////////////////////////////////////////////////////////////////////////////////////////
		
		RecCnt = pDataFileMgr->GetRowCount ();
        pDataFileMgr->StartFrame () ;
		for ( j = 0; j < RecCnt; j++ ) {			
			pDataFileMgr->InitDBFieldData () ;
			ids = pDataFileMgr->GetCellData(_T("REC_KEY"),j,RecKey);

			ids = pDataFileMgr->GetCellData(_T("파일이름"),j,FileName);
		
		//	ECO_DeleteFile( _T("UC_NOTIFY_FILE_TBL"), FileName, RecKey );
			
			ids = pDataFileMgr->MakeDeleteFrame (_T("UC_NOTIFY_FILE_TBL"), _T("REC_KEY"),_T("NUMERIC"),RecKey);
		}
        pDataFileMgr->SendFrame ();
		pDataFileMgr->EndFrame () ;

		///////////////////////////////////////////////////////////////////////////////////////////
		// 4.6 인덱스를 다음으로 돌린다.
		///////////////////////////////////////////////////////////////////////////////////////////
	    m_pGrid->SelectGetNext () ;  
       
		///////////////////////////////////////////////////////////////////////////////////////////
		// 4.7 SendFrame()
		///////////////////////////////////////////////////////////////////////////////////////////
		ids = pDataMgr->SendFrame();
		ids = pDataMgr->EndFrame();
		
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// 5. Grid 데이타를 display한다.
	///////////////////////////////////////////////////////////////////////////////////////////
	ids = AllControlDisplay( m_CurrentCM,-1) ;
	

	return 0;
}

BEGIN_EVENTSINK_MAP(CLight_UC_NOTIFY_01, CDialog)
    //{{AFX_EVENTSINK_MAP(CLight_UC_NOTIFY_01)
	ON_EVENT(CLight_UC_NOTIFY_01, IDC_MainGrid_notify, -601 /* DblClick */, OnDblClickMainGridnotify, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CLight_UC_NOTIFY_01::OnDblClickMainGridnotify() 
{
	// TODO: Add your control notification handler code here
	  
	  CESL_DataMgr *pDM = FindDM(_T("DM_UC_NOTIFY_MAIN"));
	  if(pDM == NULL)
	  {
		  AfxMessageBox(_T("FindDM() Error~!"));
		  return ;
	  }

	  if(pDM->GetRowCount () == 0)
	  {
		  AfxMessageBox(_T("선택 항목이 없습니다."));
		  return ;
	  }
	

	  CLight_UC_NOTIFY_DETAIL dlg(this);
	  dlg.SetDbClickMode (true) ;
	  dlg.DoModal ();
}

HBRUSH CLight_UC_NOTIFY_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
