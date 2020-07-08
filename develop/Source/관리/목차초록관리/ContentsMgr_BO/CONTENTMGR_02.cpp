// CONTENTMGR_02.cpp : implementation file
//

#include "stdafx.h"
#include "CONTENTMGR_02.h"
#include "CONTENTMGR_01.h"
// 키워드 생성
#include "..\..\..\단행\정리\BO_CAT_PROC_KEYWORD\BO_CAT_PROC_KEYWORD_0000.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "CommonDefine.h"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCONTENTMGR_01 *g_pParent;

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_02 dialog


CCONTENTMGR_02::CCONTENTMGR_02(INT nDlgType,CESL_Mgr* pParent  /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUC_DOWN_TAG_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDlgType = nDlgType ;
	
	m_DbClckMode = FALSE ;
	m_idx = -1;

	m_KeyWord_update = false ;
	m_MakeSearchData = NULL;
	g_pParent = (CCONTENTMGR_01*)pParent;

	m_str90=_T("");
}
CCONTENTMGR_02::~CCONTENTMGR_02()
{

}

BOOL CCONTENTMGR_02::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}


VOID CCONTENTMGR_02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCONTENTMGR_02)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCONTENTMGR_02, CDialog)
	//{{AFX_MSG_MAP(CCONTENTMGR_02)
	ON_BN_CLICKED(IDC_bBRINGINC, OnbBRINGINC)
	ON_BN_CLICKED(IDC_bBRINGINA, OnbBRINGINA)
	ON_BN_CLICKED(IDC_bSAVEA, OnbSAVEA)
	ON_BN_CLICKED(IDC_bSAVEC, OnbSAVEC)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_MESSAGE(MSG_SHOW_VIEWER, OnShowViewer)
	ON_BN_CLICKED(IDC_bCDELETE, OnbCDELETE)
	ON_BN_CLICKED(IDC_bADELETE, OnbADELETE)
	ON_BN_CLICKED(IDC_bMOK_KEYWORD, OnbMOKKEYWORD)
	ON_BN_CLICKED(IDC_bCHO_KEYWORD, OnbCHOKEYWORD)
	ON_BN_CLICKED(IDC_bDCMInput, OnbDCMInput)
	ON_WM_CTLCOLOR() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCONTENTMGR_02 message handlers

BOOL CCONTENTMGR_02::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	TocRect;
	CRect	ChoRect;
	INT		ids;	
	
    m_ParentGrid = _T("MainGrid_CONTENT");
	m_ParentCM   = _T("CM_CONTENTMGR_MAIN");
	m_ParentDM   = _T("DM_BO_CONTENTMGR_SPECIES") ;

    if(InitESL_Mgr(_T("SM_CONTENTMGR_02"))<0)
	{
		AfxMessageBox(_T("InitESL_Mgr() Error~!"));
		return FALSE ;
	}

	CESL_ControlMgr *pCM = FindCM(_T("CM_CONTENTMGR_02")) ;
	if(pCM == NULL) 
	{
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -3 ;
	}

	m_pGrid = (CESL_Grid*)pCM->FindControl(_T("VolGrid")) ;
	if(m_pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return -4;
	}

	CESL_DataMgr* pDataMgr;	
	CString strWhere;

	pDataMgr = FindDM(m_ParentDM);
	if(pDataMgr == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -2 ;
	}	
	
	//++2008.10.22 UPDATE BY PWR {{++
	// 통합시스템 적용
	strWhere.Format( _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
					 _T("WHERE CLASS_ALIAS='국방대' AND VALUE_NAME='국방대' AND GROUP_1='L' ")
					 _T("AND GROUP_2='기타' AND GROUP_3='공통' AND MANAGE_CODE=UDF_MANAGE_CODE") );
// 	strWhere.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='국방대' AND VALUE_NAME='국방대' AND GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통'"));
	//--2008.10.22 UPDATE BY PWR --}}
	pDataMgr->GetOneValueQuery( strWhere, m_str90, FALSE );	

	if ( m_str90 != _T("Y") )
	{
		m_pGrid->ShowWindow(SW_HIDE);
		
		( (CStatic*) GetDlgItem (IDC_STATIC_01) )->GetWindowRect(TocRect);
		ScreenToClient(&TocRect);
		TocRect.top = TocRect.top - 90;
		TocRect.bottom = TocRect.bottom - 90;
		( (CStatic*) GetDlgItem (IDC_STATIC_01) )->MoveWindow(TocRect);		

		( (CStatic*) GetDlgItem (IDC_STATIC_02) )->GetWindowRect(TocRect);
		ScreenToClient(&TocRect);
		TocRect.top = TocRect.top - 90;
		TocRect.bottom = TocRect.bottom - 90;
		( (CStatic*) GetDlgItem (IDC_STATIC_02) )->MoveWindow(TocRect);		

		( (CStatic*) GetDlgItem (IDC_TocRect) )->GetWindowRect(TocRect);
		ScreenToClient(&TocRect);
		TocRect.top = TocRect.top - 90;		
		( (CStatic*) GetDlgItem (IDC_TocRect) )->MoveWindow(TocRect);		

		( (CStatic*) GetDlgItem (IDC_ChoRect) )->GetWindowRect(TocRect);
		ScreenToClient(&TocRect);
		TocRect.top = TocRect.top - 90;		
		( (CStatic*) GetDlgItem (IDC_ChoRect) )->MoveWindow(TocRect);	
	}

	(( CStatic* ) GetDlgItem (IDC_TocRect))->GetWindowRect(TocRect);
	(( CStatic* ) GetDlgItem (IDC_ChoRect))->GetWindowRect(ChoRect);

	ScreenToClient(&TocRect);
	ScreenToClient(&ChoRect);

	m_TocEditdlg.MyCreate(this,TocRect ,_DAN_SINGLE);
	m_ChorokDlg.MyCreate(this,&m_TocEditdlg,ChoRect);

    if(m_DbClckMode == TRUE)
	{
         DbClick_Move(0) ;
	}
    else
	{

		ids = Init(0);
		if ( ids ) return TRUE;
    }

	LoadVolInfo();

	// 목차,초록 처음것 보여주기
	PrevNextProc(0);
	m_pGrid->SetReverse(0);

	
	m_MakeSearchData = new CMakeSearchData(this);	 

    //IDC_bSPECIES
	(CButton*)GetDlgItem(IDC_bSPECIES)->ShowWindow(FALSE) ;
	
	strOldContents = m_TocEditdlg.GetFinalToc();
	strOldAbstract = m_ChorokDlg.GetChorokText();

	( (CEdit*) GetDlgItem (IDC_eMOK_KEYWORD) )->SetLimitText(4000);
	( (CEdit*) GetDlgItem (IDC_eCHO_KEYWORD) )->SetLimitText(4000);

	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



INT CCONTENTMGR_02::Init(INT nDir)
{
    INT ids ;

	CString strShow;

   	// Member Initialize
	m_strCommonRecKey=_T("");  //목차 초록 공통정보의 레코드키(레코드 유무여부도 이것으로 체크)
	m_strCSource=_T("");	   //목차 원본
	m_strASource=_T("");	   //초록 원본
	m_strCCreateDate=_T("");   //목차 생성일
	m_strCUpdateDate=_T("");   //목차 수정일 
	m_strACreateDate=_T("");   //초록 생성일
	m_strAUpdateDate=_T("");   //초록 수정일 
	m_strMarc=_T("");	       //마크
	m_strFormCode=_T("");	   //형식구분
	m_strMatCode=_T("");	   //자료구분
	m_strRegCode=_T("");	   //등록구분
	m_strUseLimit=_T("");	   //이용제한구분
	m_strSpeciesKey=_T("");    //종키
	m_strControlNo=_T("");     //제어번호
//	m_strUbControlNo=_T("");   //.NET제어번호(key)
	m_strClassCode=_T("");	   //종구분
	m_strConYN=_T("");		   //목차유무
	m_strAbsYN=_T("");		   //목차유무
//	m_strNetConYN=_T("");	   //NET목차유무
//	m_strNetAbsYN=_T("");	   //NET초록유무
	
	m_strConKey=_T("");	       //목차키
	m_strAbsKey=_T(""); 	   //초록키

    //<-  목차초록 최종 검색엔진 정리
    m_strWorkStatus;       //작업상태
    m_strPlaceInfo;        //자료있는곳
    //->  목차초록 최종 검색엔진 정리
	
	// Initialize
	CString strWhere;
	CString strTemp;
	
	CESL_DataMgr* pDataMgr;	

	pDataMgr = FindDM(m_ParentDM);
	if(pDataMgr == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -2 ;
	}

	CESL_ControlMgr *pParentCM = FindCM(m_ParentCM) ;
	if(pParentCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -3 ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pParentCM->FindControl(m_ParentGrid) ;
	if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return -4;
	}	

	CESL_DataMgr* pDataContents;

    pDataContents = FindDM(_T("DM_목차초록")) ;
	if(pDataContents == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}
		
	switch(nDir)
	{
	case 0 :
		
		SelectMakeList(m_ParentCM ,m_ParentGrid);
		ids = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
		if (ids < 0)
		{
			//2004-07-07 이학중 
			//AfxMessageBox(_T("선택 목록을 구성할 수 없습니다."));	// 에러
			AfxMessageBox(_T("선정 항목이 없습니다."));	// 에러
			EndDialog(IDCANCEL);
			return -222;
		}
		break;
		
	case -1 :
		
		ids = SelectGetPrev (m_ParentCM ,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
			AfxMessageBox(_T("처음 자료입니다."));
			return -223;
		}
		break;
		
	case 1 :
		
		ids = SelectGetNext (m_ParentCM ,m_ParentGrid);
		if (ids == -1) {
			ids = SelectGetTailPosition(m_ParentCM ,m_ParentGrid);
			AfxMessageBox(_T("마지막 자료입니다."));
			return -224;
		}
		break;
		
	}
	
	m_idx = ids; 
	SetReverse(m_ParentCM ,m_ParentGrid, m_idx);   
	
	if(pDataMgr->GetRowCount()>0){
		m_nDataRowIdx= pGrid->SelectGetIdx ();
	}else{
		m_nDataRowIdx=-1;
	}
	
	

	// 3.화면을 셋팅한다.
	if(m_nDlgType==0){
		//SetDlgItemText(IDC_eCONTROL_NO,m_strControlNo);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_제어번호"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eCONTROL_NO,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_본표제"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eTITLE,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_저작자"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eAUTHOR,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_발행자"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_ePUBLISHER,strTemp);
	}else{
        if(GetDataMgrData (m_ParentDM ,_T("IBS_제어번호"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eCONTROL_NO,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_본표제"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eTITLE,strTemp);
        if(GetDataMgrData (m_ParentDM ,_T("IBS_저작자"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eAUTHOR,strTemp);
        if(GetDataMgrData (m_ParentDM ,_T("IBS_발행자"),strTemp,m_nDataRowIdx ) == 0)
		{	SetDlgItemText(IDC_ePUBLISHER,strTemp);
			SetControlData (_T("CM_CONTENTMGR_02"),_T("발행자"),strTemp,-1) ;
		}
	}	
	
	GetDataMgrData (m_ParentDM ,_T("IBS_종KEY"),m_strSpeciesKey,m_nDataRowIdx );

	return 0 ;
}

VOID CCONTENTMGR_02::OnbBRINGINC() 
{
	// TODO: Add your control notification handler code here
	if(!BringIn(0))AfxMessageBox(_T("목차반입실패"));
}

VOID CCONTENTMGR_02::OnbBRINGINA() 
{
	// TODO: Add your control notification handler code here
	if(!BringIn(1))AfxMessageBox(_T("초록반입실패"));
}

VOID CCONTENTMGR_02::OnCancel() 
{
	CString strFinalContents, strFinalAbstract;
	CString strMsg;

	strFinalContents = m_TocEditdlg.GetFinalToc();
	strFinalAbstract = m_ChorokDlg.GetChorokText();

	strMsg = _T("");
	if ( strOldContents != strFinalContents && strOldAbstract != strFinalAbstract)
	{
		strMsg = _T("목차와 초록이 변경되었습니다.\n");		
	} 
	else if ( strOldContents != strFinalContents )
	{
		strMsg = _T("목차가 변경되었습니다.\n");		
	}
	else if ( strOldAbstract != strFinalAbstract )
	{
		strMsg = _T("초록이 변경되었습니다.\n");		
	}	

	if ( !strMsg.IsEmpty() )
	{
		strMsg += _T("닫으시겠습니까?");
		if ( AfxMessageBox(strMsg, MB_OKCANCEL)!=IDOK )
		{
			return;
		}
	}
	
	CDialog::OnCancel();
}

VOID CCONTENTMGR_02::OnbSAVEA() 
{	
	Save(1);
	m_ChorokDlg.SetChorok(m_strTemp);
	strOldAbstract = m_strTemp;
}

VOID CCONTENTMGR_02::OnbSAVEC() 
{	
	Save(0);
	m_TocEditdlg.SetTocFromString(m_strTemp);
	strOldContents = m_strTemp;	
}


BOOL CCONTENTMGR_02::BringIn(INT nType)
{
	// 현재 경로를 저장한다.
	TCHAR szOriginPath[1024] =_T("\0"); //현재경로
	GetCurrentDirectory(1024,szOriginPath);
	// 탐색 시작
	CFileDialog filedlg(true,_T(""),_T(""),OFN_HIDEREADONLY |OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_LONGNAMES|OFN_PATHMUSTEXIST,_T("목차/초록 (*.toc) |*.toc|텍스트 (*.txt) |*.txt|"),this);
POPUPDLG:
	if(_tcscmp(m_szCurrentPath,_T(""))!=0)
		SetCurrentDirectory(m_szCurrentPath);
	if(filedlg.DoModal()==IDOK){
		GetCurrentDirectory(1024,m_szCurrentPath);
		SetCurrentDirectory(szOriginPath);
		CFile file;
		if(!file.Open(filedlg.GetPathName(),CFile::modeRead | CFile::typeBinary))
		{
			AfxMessageBox(_T("화일을 열 수 없습니다."));
			goto ERR_RETURN;
		}
		else
		{
			TCHAR cUni;
			file.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				file.SeekToBegin();
			}
			
			// 1. 편집기의 내용백업
			CString strBackup;
			if(nType==0)
				strBackup=m_TocEditdlg.GetFinalToc();
			else
				strBackup=m_ChorokDlg.GetChorokText();

			// 2. 오픈된 스트림 에서 편집기에 넣어준다.
			CString strFileStream;
			if(file.ReadHuge( strFileStream.GetBuffer(file.GetLength()),file.GetLength())<=0){
				strFileStream.ReleaseBuffer();
				goto ERR_RETURN;
			}
			strFileStream.ReleaseBuffer();

			//=====================================================
			//2010.01.18 ADD BY PJW : 쓰레기값 제거 코드
 			TCHAR cData = 52685;

			for( INT i = strFileStream.GetLength() -1 ; i >= 0 ; i-- )
			{
				if ( cData != strFileStream.GetAt(i) )
				{
					strFileStream = strFileStream.Left(i+1);
					break;
				}
			}
			//=====================================================


			if(nType==0)
			{
				CString strTempStream ;
				strTempStream = ReMake_BringInStream(nType,strFileStream) ;
				m_TocEditdlg.SetTocFromString( strTempStream );
				//m_TocEditdlg.SetTocFromString( strFileStream );
            }
			else
				m_ChorokDlg.SetChorok( strFileStream );

			// 3. 그대로 진행할것인지 사용자에게 묻는다.
			if(AfxMessageBox(_T("계속하시겠습니까?"),MB_YESNO)==IDNO)
			{
				if(nType==0)m_TocEditdlg.SetTocFromString( strBackup );
				else m_ChorokDlg.SetChorok( strBackup  );
				goto POPUPDLG;
			}
		}
	}
	SetCurrentDirectory(szOriginPath);
	return true;
ERR_RETURN:
	SetCurrentDirectory(szOriginPath);
	return false;
}

VOID CCONTENTMGR_02::OnbNEXT() 
{	
     if(m_DbClckMode == TRUE)
	{
        DbClick_Move(1);
		LoadVolInfo();
	}
    else
	{
	     Init(1);
		 PrevNextProc(0);
		 LoadVolInfo();
		 m_pGrid->SetReverse(0);
	}
	 
}

VOID CCONTENTMGR_02::OnbPREV() 
{	
     if(m_DbClckMode == TRUE)
	{
        DbClick_Move(-1) ;
		LoadVolInfo();
	}
    else
	{
	    Init(-1);
		PrevNextProc(0);
		LoadVolInfo();
		m_pGrid->SetReverse(0);
	}
}


// m_nDlgType 0 : 종선택후 입력 1: 기본화면에서 수정     // DISP에서 마크 메니저 가져다 쓰기위해 구분을 둔다.
// nType      0 : 목차          1:초록   
VOID CCONTENTMGR_02::Save(INT nType)
{ 
	// Initialize
	CString strBtnText;
	CString strConAbsYN;		//유무
	CString strConAbs  ;		//목차나 초록 
	CString szRecKey;			//목차나 초록키 
	CString system_date;		// YYYY/MM/DD
	CString strVolSortNo;
	CString strQuery;
	CString strValue;
	BOOL	bFirstInsert;		// 목차 초록 공통테이블에 행을 삽입할지를 결정하는 컬럼
	INT ids=-1;	
	CESL_DataMgr* pDataMgr;

    pDataMgr = FindDM(m_ParentDM ) ;
	if(pDataMgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	CESL_DataMgr* pVolDataMgr;
	CString	      strWhere;
	
	pVolDataMgr = FindDM(_T("DM_CONTENTS_VOL")) ;
	if(pVolDataMgr == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return ;
	}

	// 2006.01.11 ADD BY PDJ
	// 다권에 대한 번호
	INT nRow = m_pGrid->GetRow();
	pVolDataMgr->GetCellData(_T("일련번호"), nRow-1, strVolSortNo );	

    if ( m_strCommonRecKey == _T("") ) 
	{
		bFirstInsert = TRUE;
	}
	else 
	{
		bFirstInsert = FALSE;
	}

	// Get Current Date
	CTime t = CTime::GetCurrentTime () ;
    system_date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ()) ;

	
	// Syntex Check and Get Values
    if(nType==0){
        strConAbs=m_TocEditdlg.GetFinalToc();
         m_strTemp=strConAbs;
        if(m_TocEditdlg.CheckSyntax())return;
    }
    else {
        strConAbs=m_ChorokDlg.GetChorokText();
        m_strTemp=strConAbs;

    }

	if(nType==0)GetDlgItemText(IDC_bSAVEC,strBtnText);
	else GetDlgItemText(IDC_bSAVEA,strBtnText);

	if( strBtnText == _T("저  장") ) 
	{

		///////////////////////////////////////////////////////////////////////////////////////////////
		// DB Operation
		///////////////////////////////////////////////////////////////////////////////////////////////
	    
		pDataMgr->StartFrame () ;
	
		
		// 1. 목차/초록 유무값 변경 (LS)
		if(m_strSpeciesKey!=_T(""))
		{		
			pDataMgr->InitDBFieldData () ;
			
			if(nType==0)pDataMgr->AddDBFieldData(_T("CONTENTS_YN"),_T("STRING"),_T("Y"));
			else pDataMgr->AddDBFieldData(_T("ABSTRACTS_YN"),_T("STRING"),_T("Y"));						
			
			ids = pDataMgr->MakeUpdateFrame (_T("IDX_BO_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey);
			if(ids <0)goto ERR_RETURN;

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 2003.11.3 이학중 추가 
			// IDX_BO_TBL UPDATE 시 IDX_TMP_BO_TBL 입력하기 
			//=====================================================
			//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 			pDataMgr->InitDBFieldData ();
// 			
// 			CString strTmp_Key ;
// 			CString system_Date ;
// 			CTime t = CTime::GetCurrentTime ();
// 			system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
// 			
// 			// 1. 목차초록 컬렉션 생성 TEMP TABLE : IDX_TMP_BO_TBL
// 			pDataMgr->MakeRecKey (strTmp_Key) ;
// 			pDataMgr->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
// 			pDataMgr->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
// 			pDataMgr->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
// 			pDataMgr->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
// 			ids = pDataMgr->MakeInsertFrame (_T("IDX_TMP_BO_TBL"));
// 			if(ids<0) {
// 				AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 				return ;
// 			}			
			//=====================================================
			
		}

		// 2. 목차 DB 작업
		// 2.1. 목차 초록 공통정보가 없음. -> 새로운 레코드를 생성한다.
		if(m_strCommonRecKey==_T("")){  // bFirstInsert와 같은 의미이지만 이것이 더 안정적이다.
			if(pDataMgr->MakeRecKey (m_strCommonRecKey))goto ERR_RETURN; 
			m_strCommonRecKey.ReleaseBuffer();
			pDataMgr->InitDBFieldData () ;
			pDataMgr->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey);
			pDataMgr->AddDBFieldData(_T("SPECIES_KEY"),_T("STRING"),m_strSpeciesKey);
			pDataMgr->AddDBFieldData(_T("SPECIES_CLASS"),_T("STRING"),_T("1"));//배가이후
			pDataMgr->AddDBFieldData(_T("CONTROL_NO"),_T("STRING"),m_strControlNo);
			pDataMgr->AddDBFieldData(_T("VOL_SORT_NO"),_T("NUMERIC"),strVolSortNo);			
			//pDataMgr->AddDBFieldData(_T("USE_LIMIT_CODE"),_T("STRING"),m_strUseLimit);
			// 2009.09.16 ADD BY PWR : 목차초록 저장시 MANAGE_CODE추가
			pDataMgr->AddDBFieldData(_T("MANAGE_CODE"),_T("STRING"),m_pInfo->MANAGE_CODE);
		    ids = pDataMgr->MakeInsertFrame (_T("BO_TOC_COMMON_TBL")) ; 
			if(ids < 0 )goto ERR_RETURN;

			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// 2006.1.2 ADD BY PDJ 목차/초록에는 TOC키를 관리하지 않는다. 1:N 구조
			//2003.11.3 이학중 추가 
			//BO_SPECIES_TBL 에 TOC_COMMON_KEY -> UPDATE 			
			/*
			pDataMgr->InitDBFieldData () ;
			pDataMgr->AddDBFieldData(_T("TOC_COMMON_KEY"),_T("NUMERIC"),m_strCommonRecKey);
			ids = pDataMgr->MakeUpdateFrame (_T("BO_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey);
			if(ids<0) {
				AfxMessageBox(_T("MakeUpdateFrame(BO_SPECIES_TBL) Error~!"));
				return ;
			}
			*/
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			
		}

		// 2.2. 삽입하거나 수정한다.
		if(nType==0){
			if(m_strConKey==_T("")){ 
				if(pDataMgr->MakeRecKey (szRecKey))goto ERR_RETURN; 
			}else{
				szRecKey = m_strConKey.GetBuffer(0);
			}
			// Set 'CONTENTS_YN' 
			if(m_strSpeciesKey!=_T("")){
			/*	
				pDataMgr->InitDBFieldData();
				pDataMgr->AddDBFieldData(_T("KOLIS_CON_YN"),_T("STRING"),_T("Y"));
				ids = pDataMgr->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey);
				if(ids <0 )goto ERR_RETURN;
			*/
			}

		}else{
			if(m_strAbsKey==_T("")){
				if(pDataMgr->MakeRecKey(szRecKey))goto ERR_RETURN; 
			}else{
				szRecKey = m_strAbsKey.GetBuffer(0);
			}
			// Set 'ABSTRACTS_YN' 
			if(m_strSpeciesKey!=_T("")){
			/*(
				pDataMgr->InitDBFieldData();
				pDataMgr->AddDBFieldData(_T("KOLIS_ABS_YN"),_T("STRING"),_T("Y"));
				ids = pDataMgr->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey);
				if(ids <0 )goto ERR_RETURN;
			*/
			}
		}

		// 2.3. 목차/초록 각각의 테이블에 정보를 넣는다.
		pDataMgr->InitDBFieldData();
		if(nType==0){
			if(m_strConKey==_T("")){
				pDataMgr->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),szRecKey);
				pDataMgr->AddDBFieldData(_T("CREATE_DATE"),_T("STRING"),system_date);
				m_strCCreateDate=system_date;
			}
			pDataMgr->AddDBFieldData (_T("TOC_COMMON_KEY"),_T("NUMERIC"),m_strCommonRecKey);
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//2003.10.24 이학중 수정 
			//' '인용부호 있을경우 PARAMETER :true 추가 
			pDataMgr->AddDBFieldData(_T("CONTENTS"),_T("STRING"),strConAbs,true);
			//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			pDataMgr->AddDBFieldData(_T("UPDATE_DATE"),_T("STRING"),system_date);
			m_strCUpdateDate=system_date;
			// 2009.09.16 ADD BY PWR : 목차초록 저장시 MANAGE_CODE추가
			pDataMgr->AddDBFieldData(_T("MANAGE_CODE"),_T("STRING"),m_pInfo->MANAGE_CODE);
		}else{
			if(m_strAbsKey==_T("")){
				pDataMgr->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),szRecKey);
				pDataMgr->AddDBFieldData(_T("CREATE_DATE"),_T("STRING"),system_date);
				m_strACreateDate=system_date;
			}
			pDataMgr->AddDBFieldData (_T("TOC_COMMON_KEY"),_T("NUMERIC"),m_strCommonRecKey);
		     //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			//2003.10.24 이학중 수정 
			//' '인용부호 있을경우 PARAMETER :true 추가 
			pDataMgr->AddDBFieldData(_T("ABSTRACTS"),_T("STRING"),strConAbs,true);
			//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			pDataMgr->AddDBFieldData(_T("UPDATE_DATE"),_T("STRING"),system_date);
			m_strAUpdateDate=system_date;
		}
		
		if(nType==0)
		{
			if(m_strConKey==_T("")){
				ids = pDataMgr->MakeInsertFrame(_T("BO_CONTENTS_TBL")) ;
				if(ids <0)goto ERR_RETURN;

				pDataMgr->InitDBFieldData();
				pDataMgr->AddDBFieldData(_T("CONTENTS_KEY"),_T("NUMERIC"),szRecKey);
				ids  = pDataMgr->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey);
				if(ids <0)goto ERR_RETURN;			
			}else{
				ids = pDataMgr->MakeUpdateFrame(_T("BO_CONTENTS_TBL"),_T("REC_KEY"),_T("NUMERIC"),szRecKey) ;
				if(ids <0 )goto ERR_RETURN;			
			}

			// 2006.01.14 ADD BY PDJ
			// 세미나 자료 처리	: 세미나를 추가로 잡는다.
			if ( m_strMatCode==_T("MI") && m_str90==_T("Y") )
			{
				m_strConKey=szRecKey;
				MakeSemiCollection(	0, pDataMgr, strConAbs );	
			}
		} 
		else
		{
			if(m_strAbsKey==_T("")){
				ids = pDataMgr->MakeInsertFrame (_T("BO_ABSTRACTS_TBL")) ;
				if(ids <0)goto ERR_RETURN;		

				pDataMgr->InitDBFieldData();
				pDataMgr->AddDBFieldData(_T("ABSTRACTS_KEY"),_T("NUMERIC"),szRecKey);
				ids  = pDataMgr->MakeUpdateFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey) ;
				if(ids <0 )goto ERR_RETURN;
			}else{
				ids = pDataMgr->MakeUpdateFrame(_T("BO_ABSTRACTS_TBL"),_T("REC_KEY"),_T("NUMERIC"),szRecKey) ;
				if(ids <0)goto ERR_RETURN;			
			}
		}
		// 2.4 검색 엔진 작업전에 멤버 변수들을 DB와 일치 시킨다.
		if(nType==0)
		{
			m_strConYN=_T("Y");
			m_strCSource=strConAbs;
			m_strConKey=szRecKey;
		}
		else
		{
			m_strAbsYN=_T("Y");
			m_strASource=strConAbs;
			m_strAbsKey=szRecKey;
		}

		////////////////////////////////////////////////////////////
		// 검색 엔진 작업
		////////////////////////////////////////////////////////////
		// 목차,초록이 처음입력이면 insert
		ids = InsertIndexTable(pDataMgr,nType) ;
		if(ids == 100)
		{
			pDataMgr->EndFrame () ;
			return ;
		}
	
		ids = pDataMgr->SendFrame ();
		if(ids <0)goto ERR_RETURN;
		
		pDataMgr ->EndFrame () ;
		
        
	}
	      
	else  //복원 ///////////////////////////////////////////////////////////////
	{
		AfxMessageBox(_T("수정화면 입니다.")) ;
		if(ids == 100)
		{
			pDataMgr->EndFrame () ;
			return ;
		}
	}

    ////////////////////////////////////////////////////////////
	// Saved 성공이다. !!!
	////////////////////////////////////////////////////////////
	
	// 화면 작업
	if(nType==0){


		if(SetDataMgrData(m_ParentDM,_T("BS_목차유무"),_T("Y"),m_nDataRowIdx))goto ERR_RETURN;
		if(m_nDlgType==1){
			if(SetDataMgrData(m_ParentDM,_T("BS_목차유무"),_T("Y"),m_nDataRowIdxForDlgType1))goto ERR_RETURN;
		}
		SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 있음>"));
		GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
		MessageBox(_T("목차를 저장했습니다."));
        /* 2003년 6월 25일 수정 
		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,6,_T("Y")) ;
        */
		//2006-03-08 황정영 수정
		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,6,_T("Y")) ;
		pGrid->m_bProtectSelectColumn = TRUE;
		AllControlDisplay (m_ParentCM ,-1) ;
		pGrid->m_bProtectSelectColumn = FALSE;


	}else{
		
		if(SetDataMgrData(m_ParentDM,_T("BS_초록유무"),_T("Y"),m_nDataRowIdx))goto ERR_RETURN;
		if(m_nDlgType==1){
			if(SetDataMgrData(m_ParentDM,_T("BS_초록유무"),_T("Y"),m_nDataRowIdxForDlgType1))goto ERR_RETURN;
		}
		SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 있음>"));
 		GetDlgItem(IDC_bADELETE)->EnableWindow(true);
		MessageBox(_T("초록을 저장했습니다."));  
		//2006-03-08 황정영 수정
		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,7,_T("Y")) ;
		pGrid->m_bProtectSelectColumn = TRUE;
		AllControlDisplay (m_ParentCM ,-1) ;
		pGrid->m_bProtectSelectColumn = FALSE;
	}

	return;
	

ERR_RETURN:	// 실패했을 경우
	
/*	

	ECO_SetServerType(CS);
	if(pDataMgr)pDataMgr->ExecuteRollback();
	ECO_EndFrame();

	AfxMessageBox(_T("자료저장에 실패하였습니다."));
*/
  Init(0);

  return;
} 

VOID CCONTENTMGR_02::OnShowViewer()
{
	m_TocEditdlg.ShowViewer (TRUE, FALSE) ;
//	m_TocEditorDlg.ShowViewer(TRUE, FALSE) ;
}


//nType 0 : 목차 , 1 : 초록 
INT CCONTENTMGR_02::InsertIndexTable( CESL_DataMgr *pTempMgr ,INT nType )
{
    INT ids ;
	CMakeSearchData *pMakeData = NULL;
	pMakeData = new CMakeSearchData(this);
    CString sBody;
		// Syntex Check and Get Values

    CESL_DataMgr *IndexDM_Mgr = FindDM(_T("DM_CONTENTMGR_INDEX")) ;
	if(IndexDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		delete pMakeData;
		return -1;
	}

    CString sWhere ,check_TOC_YN;

    if(nType==0)
	{
        sBody=m_TocEditdlg.GetFinalToc();
        if(m_TocEditdlg.CheckSyntax()) 
		{
			delete pMakeData;
			return -1;
		}
    }
    else 
	{
        sBody=m_ChorokDlg.GetChorokText();
	}
	/////////////////////////////////////////////////////////////////////
	//인덱스 키워드 생성 하기 
	/////////////////////////////////////////////////////////////////////
	MakeIndexKeyWord(sBody) ;

	INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//한자 한글 변환
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
		delete pMakeData;
		return -1;
	}
	LPCTSTR lpszTok = _tcstok(sBody.GetBuffer(0), _T(" "));
	CString tmp_Data;
	CString strKeyWord;
	
	while (lpszTok)
	{   
		tmp_Data.Format (_T("%s"),lpszTok);
		iCh = pMakeData->GetKeyWord( tmp_Data );				//공백, 특수문자 제거
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			delete pMakeData;
			return -2;
		}

		strKeyWord +=  _T(" ") + tmp_Data;

		lpszTok = _tcstok(NULL, _T(" "));
	}

	// 사용 종료
	delete pMakeData;

	strKeyWord.TrimLeft () ;
    sBody = strKeyWord ;
    

	if(m_KeyWord_update == TRUE)
    sBody = m_strUpdateKeyword ;
    
    
	if(m_KeyWord_update == TRUE)
    sBody = m_strUpdateKeyword ;


    if(nType==0)
	{
		 CString tmp_Body ;
         ((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->GetWindowText(tmp_Body) ;
		 if(sBody != tmp_Body)
			 sBody = tmp_Body ;
	}
	else
	{
         CString tmp_Body ;
         ((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->GetWindowText(tmp_Body) ;
		 if(sBody != tmp_Body)
			 sBody = tmp_Body ;
	}     


    //m_nDataRowIdx,m_ParentDM 
   
    //m_nDlgType==0 : //입력(종선택후)
	//m_nDlgType==1 : //수정(종의 유무와상관없다.);
   
    CString  v_SPECIES_KEY,v_SE_SHELF_CODE,v_CLASS_NO ,v_BOOK_CODE,v_VOL_CODE_DISP,v_TITLE_INFO, v_AUTHOR_INFO, v_PUB_INFO, v_PUB_YEAR_INFO ,v_PLACE_INFO;
    CString  v_RecKey , v_SPECIES_CLASS ,v_TOC_USE_LIMIT_CODE , v_ABS_USE_LIMIT_CODE;
	CString  v_TOC_CREATE_DATE ,v_TOC_UPDATE_DATE, v_ABS_CREATE_DATE ,v_ABS_UPDATE_DATE;
	CString  v_TOC_PRESENT, v_ABS_PRESENT;
    
    CTime t = CTime::GetCurrentTime () ;
	CString systemDate ;
	systemDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
    

	//입력화면 일 경우 //////////////////////////////////////////////////////////////////////////
	//if(m_nDlgType == 0)
	//{

		CESL_DataMgr *pDM = FindDM(m_ParentDM );
		if(pDM == NULL) {
			AfxMessageBox(_T("FindDM() Error~!")) ;
		}        
		pDM->GetCellData (_T("IBS_종KEY"),m_nDataRowIdx,v_SPECIES_KEY );
        pDM->GetCellData (_T("별치기호"),m_nDataRowIdx,v_SE_SHELF_CODE );
		pDM->GetCellData (_T("분류기호"),m_nDataRowIdx,v_CLASS_NO  );
		pDM->GetCellData (_T("도서기호"),m_nDataRowIdx,v_BOOK_CODE );
        
		pDM->GetCellData (_T("권책기호출력"),m_nDataRowIdx,v_VOL_CODE_DISP );
        pDM->GetCellData (_T("표제사항"),m_nDataRowIdx,v_TITLE_INFO );
		pDM->GetCellData (_T("저작자사항"),m_nDataRowIdx,v_AUTHOR_INFO  );
        pDM->GetCellData (_T("발행자사항"),m_nDataRowIdx,v_PUB_INFO  );
		pDM->GetCellData (_T("발행년도사항"),m_nDataRowIdx,v_PUB_YEAR_INFO );
        pDM->GetCellData (_T("자료있는곳"),m_nDataRowIdx,v_PLACE_INFO  );  
		pDM->GetCellData (_T("종구분값"),m_nDataRowIdx,v_SPECIES_CLASS  );

		// 2009.09.16 ADD BY PWR : 목차, 초록 유무추가 (CREATE_DATE를 넣을지 UPDATE_DATE를 넣을지 유무를 위해)
		pDM->GetCellData(_T("BS_목차유무"),m_nDataRowIdx,v_TOC_PRESENT);
		pDM->GetCellData(_T("BS_초록유무"),m_nDataRowIdx,v_ABS_PRESENT);
		
		sWhere.Format (_T(" REC_KEY = %s"),m_strCommonRecKey) ;
		IndexDM_Mgr->RefreshDataManager (sWhere);	

        if(nType == 0 )
		{
		    GetControlData (_T("CM_CONTENTMGR_02"),_T("목차이용제한구분"),v_TOC_USE_LIMIT_CODE) ;
            v_TOC_CREATE_DATE = systemDate ;
		}
		else
		{
        	GetControlData (_T("CM_CONTENTMGR_02"),_T("초록이용제한구분"),v_ABS_USE_LIMIT_CODE) ;
			v_ABS_CREATE_DATE = systemDate ;
		}
          
		// 2006.1.3 ADD BY PDJ
		// 입력일지라도 목차나 초록이 나중에 입력되는 경우 수정모드이다.
		BOOL bInsert;
		bInsert = FALSE;
		if ( IndexDM_Mgr->GetRowCount() <= 0 )
		{
			bInsert = TRUE;						
		}
	
        pTempMgr->InitDBFieldData () ;       
		if ( bInsert )
		{
			// 입력모드이면
			pTempMgr->AddDBFieldData (_T("REC_KEY") ,_T("NUMERIC"),m_strCommonRecKey) ;			
			if(nType == 0 ) 
			{
				pTempMgr->AddDBFieldData (_T("TOC_CREATE_DATE") ,_T("STRING"),v_TOC_CREATE_DATE) ;			
			}
			else
			{
				pTempMgr->AddDBFieldData (_T("ABS_CREATE_DATE") ,_T("STRING"),v_ABS_CREATE_DATE) ;   			
			}
		}		
		else
		{
			// 2009.09.16 UPDATE BY PWR : 수정시 목차, 초록 각각은 입력일 수 있다. 입력이면 CREATE_DATE가 입력되어야한다.
			if(nType == 0 )
			{
				if(_T("Y") == v_TOC_PRESENT)
				{
					pTempMgr->AddDBFieldData (_T("TOC_UPDATE_DATE"),_T("STRING"),v_TOC_CREATE_DATE);
				}
				else
				{
					pTempMgr->AddDBFieldData (_T("TOC_CREATE_DATE"),_T("STRING"),v_TOC_CREATE_DATE);
				}
			}
			else
			{
				if(_T("Y") == v_ABS_PRESENT)
				{
					pTempMgr->AddDBFieldData (_T("ABS_UPDATE_DATE"),_T("STRING"),v_ABS_CREATE_DATE);
				}
				else
				{
					pTempMgr->AddDBFieldData (_T("ABS_CREATE_DATE"),_T("STRING"),v_ABS_CREATE_DATE);
				}
			}
// 			if(nType == 0 )
// 			{
// 				pTempMgr->AddDBFieldData (_T("TOC_UPDATE_DATE") ,_T("STRING"),v_TOC_CREATE_DATE) ;			
// 			}
// 			else
// 			{
// 				pTempMgr->AddDBFieldData (_T("ABS_UPDATE_DATE") ,_T("STRING"),v_ABS_CREATE_DATE) ;   			
// 			}
		}

		if(nType == 0 )
		{   
			// 목차일때			
			pTempMgr->AddDBFieldData (_T("IDX_TOC_KEYWORD") ,_T("STRING"),sBody) ;			
			pTempMgr->AddDBFieldData (_T("TOC_YN"),_T("STRING"),_T("Y"));
			pTempMgr->AddDBFieldData (_T("TOC_USE_LIMIT_CODE") ,_T("STRING"),v_TOC_USE_LIMIT_CODE) ;            
			pTempMgr->AddDBFieldData (_T("CONTENTS_KEY") ,_T("NUMERIC"),m_strConKey ) ;
		}
		else
		{
			// 초록일때			
			pTempMgr->AddDBFieldData (_T("IDX_ABS_KEYWORD") ,_T("STRING"),sBody) ;			
			pTempMgr->AddDBFieldData (_T("ABS_YN"),_T("STRING"),_T("Y"));
			pTempMgr->AddDBFieldData (_T("ABS_USE_LIMIT_CODE") ,_T("STRING"),v_ABS_USE_LIMIT_CODE) ;
			pTempMgr->AddDBFieldData (_T("ABSTRACTS_KEY") ,_T("NUMERIC"),m_strAbsKey  );			
		}
        
		ids =   pTempMgr->AddDBFieldData (_T("SPECIES_KEY") ,_T("NUMERIC"),v_SPECIES_KEY  ) ;
		ids =   pTempMgr->AddDBFieldData (_T("SE_SHELF_CODE") ,_T("STRING"),v_SE_SHELF_CODE) ;
		ids =	pTempMgr->AddDBFieldData (_T("CLASS_NO") ,_T("STRING"),v_CLASS_NO) ;
		ids =	pTempMgr->AddDBFieldData (_T("BOOK_CODE") ,_T("STRING"),v_BOOK_CODE) ;
		ids =	pTempMgr->AddDBFieldData (_T("VOL_CODE_DISP") ,_T("STRING"),v_VOL_CODE_DISP) ;
		ids =	pTempMgr->AddDBFieldData (_T("TITLE_INFO") ,_T("STRING"),v_TITLE_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("PUB_INFO") ,_T("STRING"),v_PUB_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("PUB_YEAR_INFO") ,_T("STRING"),v_PUB_YEAR_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("PLACE_INFO") ,_T("STRING"),v_PLACE_INFO) ;
		ids =	pTempMgr->AddDBFieldData (_T("SPECIES_CLASS") ,_T("STRING"),v_SPECIES_CLASS) ;
		// 2009.09.16 ADD BY PWR : 저작자 사항 추가
		ids =	pTempMgr->AddDBFieldData (_T("AUTHOR_INFO") ,_T("STRING"),v_AUTHOR_INFO) ;
       
		if ( bInsert )
		{
			pTempMgr->MakeInsertFrame (_T("IDX_BO_TOC_TBL"));
		}
		else
		{
			pTempMgr->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strCommonRecKey) ;					
		}
        
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.11.12 이학중 추가 
		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{   
			CString strData ;
			strData.Format(_T("ECO$P_MAKE_BOTOC$I(%s);"), m_strCommonRecKey);
			pTempMgr->AddFrame(strData);
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//}

	return 0 ;
}



INT CCONTENTMGR_02::UpdateIndexTable(CESL_DataMgr *pTempMgr, INT nType)
{

  
	CMakeSearchData *pMakeData = NULL;
    
	pMakeData = new CMakeSearchData(this); 


    CString sBody;
		// Syntex Check and Get Values
   
    CESL_DataMgr *IndexDM_Mgr = FindDM(_T("DM_CONTENTMGR_INDEX")) ;
	if(IndexDM_Mgr == NULL) {
		AfxMessageBox(_T("FindDM() Error~!"));
		return -1;
	}

    CString sWhere ,check_TOC_YN;

    if(nType==0)
	{
        sBody=m_TocEditdlg.GetFinalToc();
        if(m_TocEditdlg.CheckSyntax())return -1;
    }
    else 
	{
        sBody=m_ChorokDlg.GetChorokText();
	}
	
	/////////////////////////////////////////////////////////////////////
	//인덱스 키워드 생성 하기 
	/////////////////////////////////////////////////////////////////////
	INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//한자 한글 변환
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
		return -1;
	}

	MakeIndexKeyWord(sBody) ;

	LPCTSTR lpszTok = _tcstok(sBody.GetBuffer(0), _T(" "));
	CString tmp_Data;
	CString strKeyWord;
	
	while (lpszTok)
	{   
		tmp_Data.Format (_T("%s"),lpszTok);
		iCh = pMakeData->GetKeyWord( tmp_Data );				//공백, 특수문자 제거
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			return -2;
		}

		strKeyWord +=  _T(" ") + tmp_Data;

		lpszTok = _tcstok(NULL, _T(" "));
	}

	strKeyWord.TrimLeft () ;
    sBody = strKeyWord ;

	if(m_KeyWord_update == TRUE)
    sBody = m_strUpdateKeyword ;


    if(nType==0)
	{
		 CString tmp_Body ;
         ((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->GetWindowText(tmp_Body) ;
		 if(sBody != tmp_Body)
			 sBody = tmp_Body ;
	}
	else
	{
         CString tmp_Body ;
         ((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->GetWindowText(tmp_Body) ;
		 if(sBody != tmp_Body)
			 sBody = tmp_Body ;
	}     

    //m_nDataRowIdx,m_ParentDM 
   
    //m_nDlgType==0 : //입력(종선택후)
	//m_nDlgType==1 : //수정(종의 유무와상관없다.);
   
    CString  v_SPECIES_KEY,v_SE_SHELF_CODE,v_CLASS_NO ,v_BOOK_CODE,v_VOL_CODE_DISP,v_TITLE_INFO, v_AUTHOR_INFO, v_PUB_INFO, v_PUB_YEAR_INFO ,v_PLACE_INFO ;
    CString  v_RecKey , v_SPECIES_CLASS ,v_TOC_USE_LIMIT_CODE , v_ABS_USE_LIMIT_CODE;
	CString  v_TOC_CREATE_DATE ,v_TOC_UPDATE_DATE, v_ABS_CREATE_DATE ,v_ABS_UPDATE_DATE    ;
    
    CTime t = CTime::GetCurrentTime () ;
	CString systemDate ;
	systemDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());

    
	CESL_DataMgr *pDM = FindDM(m_ParentDM );
	if(pDM == NULL) {
	AfxMessageBox(_T("FindDM() Error~!")) ;
	}        
	pDM->GetCellData (_T("IBS_종KEY"),m_nDataRowIdx,v_SPECIES_KEY );

    
    
	if(nType == 0 )
	{
		GetControlData (_T("CM_CONTENTMGR_02"),_T("목차이용제한구분"),v_TOC_USE_LIMIT_CODE) ;
		v_TOC_UPDATE_DATE = systemDate ;
	}
	else
	{
		sWhere.Format (_T(" SPECIES_KEY = %s"),v_SPECIES_KEY) ;
		IndexDM_Mgr->RefreshDataManager (sWhere);
	 /*
		IndexDM_Mgr->GetCellData (_T("TOC_YN"),0,check_TOC_YN);
		if(!(check_TOC_YN == _T("Y")))
		{
			AfxMessageBox(_T("목차를 먼저 생성하신후 작업을 진행 하시기 바랍니다.")) ;
			return 100 ;
		}
	 */
		GetControlData (_T("CM_CONTENTMGR_02"),_T("초록이용제한구분"),v_ABS_USE_LIMIT_CODE) ;
		v_ABS_UPDATE_DATE = systemDate ;
	}

    
	pTempMgr->InitDBFieldData () ;
	
	if(nType == 0 )
	{   
	//	pTempMgr->AddDBFieldData (_T("REC_KEY") ,_T("NUMERIC"),v_RecKey) ;
		pTempMgr->AddDBFieldData (_T("IDX_TOC_KEYWORD") ,_T("STRING"),sBody) ;
		pTempMgr->AddDBFieldData (_T("TOC_UPDATE_DATE") ,_T("STRING"),v_TOC_UPDATE_DATE) ;
		pTempMgr->AddDBFieldData (_T("TOC_USE_LIMIT_CODE") ,_T("STRING"),v_TOC_USE_LIMIT_CODE) ;
		
	}
	else
	{
		pTempMgr->AddDBFieldData (_T("IDX_ABS_KEYWORD") ,_T("STRING"),sBody) ;
		pTempMgr->AddDBFieldData (_T("ABS_UPDATE_DATE") ,_T("STRING"),v_ABS_UPDATE_DATE) ;  
		pTempMgr->AddDBFieldData (_T("ABS_USE_LIMIT_CODE") ,_T("STRING"),v_ABS_USE_LIMIT_CODE) ;
		
		pTempMgr->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),v_SPECIES_KEY) ;
		
		return 0 ;
	}

    pTempMgr->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),v_SPECIES_KEY) ;
    
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//2003.11.12 이학중 추가 
	CString strQuery ;
	//CString v_RecKey ;
	strQuery.Format (_T("SELECT REC_KEY FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s "),v_SPECIES_KEY );
	pTempMgr->GetOneValueQuery (strQuery,v_RecKey);
	if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
	{   
		CString strData ;
		strData.Format(_T("ECO$P_MAKE_BOTOC$I(%s);"), v_RecKey);
		pTempMgr->AddFrame(strData);
	}
	
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	delete pMakeData;

	return 0 ;
}

INT CCONTENTMGR_02::MakeIndexKeyWord(CString &tmpValue)
{	
    
	tmpValue.TrimLeft () ; tmpValue.TrimRight () ;

	INT first,end ;
	first = tmpValue.Find (_T("<body>")) ;
    end   = tmpValue.Find (_T("</body>"));
    
	if(first<0)
	{
		tmpValue.Replace(_T("\r\n"),_T(" "));
		return 0;
	}
    
    tmpValue = tmpValue.Mid (first+6,end-first-6) ;

	// 2005.12.09 ADD BY PDJ
	// 목차 번호를 키워드에서 빼기
	// '/' 이후 빼기
	CString sBodyTemp;
	CString strNewBody;
	CString strLine;
	CString strChar;
	
	INT		nOffSet;	
	INT		nPos;
	INT		nLength;
	INT		i;
	BOOL	bNumFlag;

	sBodyTemp = tmpValue;

	nOffSet = 0;
	nPos = 0;
	strNewBody = _T("");
	while ( nOffSet >= 0)
	{
		nOffSet = sBodyTemp.Find(_T("\r\n"), nPos );		
		if ( nOffSet == -1 )
		{
			strLine = sBodyTemp.Mid(nPos);
		}
		else
		{
			strLine = sBodyTemp.Mid(nPos, nOffSet-nPos);
		}		

		bNumFlag = FALSE;
		// 저자, 페이지 떼기
		nLength = strLine.Find(_T("/"));
		if ( nLength < 0 ) 
		{
			nLength = strLine.GetLength();			
		}
		else
		{
			strLine = strLine.Left(nLength);
		}

		// 저자가 없을 경우 페이지 떼기
		nLength = strLine.Find(_T("="));
		if ( nLength < 0 ) 
		{
			nLength = strLine.GetLength();			
		}
		else
		{
			strLine = strLine.Left(nLength);
		}

		for ( i=0 ; i<nLength ; i++ )
		{
			strChar = strLine.Mid(i, 1);
			if ( strChar >= '0' && strChar <= '9' )
			{
				bNumFlag = TRUE;
				continue;
			}
			else
			{
				if ( bNumFlag == TRUE )
				{
					if ( strChar == _T(".") || strChar == _T(" ") )
					{
						strLine = strLine.Mid(i+1);
						break;
					}
					else
					{
						bNumFlag = FALSE;
						break;

					}
				}
				else
				{
					bNumFlag = FALSE;
					break;
				}				
			}			
		}			
		
		strLine.TrimLeft(_T(" "));
		strLine.TrimRight(_T(" "));
		nPos = nOffSet+2;
		strNewBody += strLine + _T("\r\n");
	}

	strNewBody.TrimRight(_T("\r\n"));	
	strNewBody.Replace(_T("\r\n"),_T(" "));	

	strNewBody.MakeUpper();

	// 키워드 룰 적용하기
	nPos = 0;
	nOffSet = 0;
	sBodyTemp = _T("");
	while(nOffSet >= 0)
	{
		nOffSet = strNewBody.Find(_T(" "), nPos );		
		if ( nOffSet == -1 )
		{
			strLine = strNewBody.Mid(nPos);
		}
		else
		{
			strLine = strNewBody.Mid(nPos, nOffSet-nPos);
		}
		

		strLine.TrimLeft(_T(" "));
		strLine.TrimRight(_T(" "));
		
		strChar = KeywordMake(strLine);
		if ( !strChar.IsEmpty() )
		{
			sBodyTemp += strChar + _T(" ");
		}

		nPos = nOffSet+1;
	}

	tmpValue = sBodyTemp;

	return 0 ;
}


VOID CCONTENTMGR_02::Delete(INT nType)
{
    
	INT ids ;

	CESL_DataMgr *pDM = FindDM(_T("DM_목차초록"));
	if(pDM == NULL){
		AfxMessageBox(_T("FindDM() Error~!"));
		return ;

	}

	CString sQuery ,tmp_Common_Key ;
    
	sQuery.Format (_T("SELECT REC_KEY FROM BO_TOC_COMMON_TBL WHERE SPECIES_KEY = %s"),m_strSpeciesKey); 
	pDM->GetOneValueQuery (sQuery,tmp_Common_Key) ;
    

	pDM->StartFrame ();

	if(nType == 0) // 목차 삭제
	{    
		pDM->InitDBFieldData ();
		pDM->MakeDeleteFrame (_T("BO_CONTENTS_TBL"),_T("TOC_COMMON_KEY"),_T("NUMERIC"),tmp_Common_Key);

		pDM->InitDBFieldData ();
	    pDM->MakeDeleteFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.11.12 이학중 추가 
		CString strQuery;
		CString v_RecKey;
		strQuery.Format (_T("SELECT REC_KEY FROM IDX_BO_TOC_TBL WHERE SPECIES_KEY = %s "),m_strSpeciesKey );
		pDM->GetOneValueQuery (strQuery,v_RecKey);
		if(m_pInfo->GetSearchIndexType() == ECO_SEARCH)
		{   
			CString strData ;
			strData.Format(_T("ECO$P_DELETE_BOTOC$I(%s);"), v_RecKey);
			pDM->AddFrame(strData);
		}
		
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		/*
		pDM->AddDBFieldData (_T("TOC_YN"),_T("STRING"),_T("N"));
		pDM->AddDBFieldData (_T("CONTENTS_KEY"),_T("NUMERIC"),_T("")) ;
		pDM->AddDBFieldData (_T("TOC_USE_LIMIT_CODE"),_T("STRING"),_T(""));
		pDM->AddDBFieldData (_T("IDX_TOC_KEYWORD"),_T("STRING"),_T("")) ;
		pDM->AddDBFieldData (_T("TOC_CREATE_DATE"),_T("STRING"),_T(""));
		pDM->AddDBFieldData (_T("TOC_UPDATE_DATE"),_T("STRING"),_T(""));
		
		pDM->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		*/

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData (_T("CONTENTS_YN"),_T("STRING"),_T("N")) ;
		pDM->MakeUpdateFrame (_T("IDX_BO_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey) ;


		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.11.3 이학중 추가 
		// IDX_BO_TBL UPDATE 시 IDX_TMP_BO_TBL 입력하기 
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		pDM->InitDBFieldData ();
// 		
// 		CString strTmp_Key ;
// 		CString system_Date ;
// 		CTime t = CTime::GetCurrentTime ();
// 		system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
// 		
// 		pDM->MakeRecKey (strTmp_Key) ;
// 		pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
// 		pDM->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
// 		pDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
// 		pDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);		
// 		ids = pDM->MakeInsertFrame (_T("IDX_TMP_BO_TBL"));
// 		if(ids<0) {
// 			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 			return ;
// 		}
		//=====================================================
		
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2006.01.14 ADD BY PDJ
		// 세미나 자료 처리	: 세미나를 추가로 잡는다.
		if ( m_strMatCode==_T("MI") && m_str90==_T("Y") )
		{			
			// 기존 색인 삭제			
			// DELETE 
			MakeSemiCollection(1, pDM);
		}

		m_strConYN = _T("N") ;

		ids = SetDataMgrData(m_ParentDM ,_T("BS_목차유무"),_T("N"),m_idx) ;

		SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 없음>"));
		GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
		MessageBox(_T("목차를 삭제했습니다."));

		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,6,_T("N")) ;

        
	}
    else  //초록 삭제 
	{

        pDM->InitDBFieldData ();
		pDM->MakeDeleteFrame (_T("BO_ABSTRACTS_TBL"),_T("TOC_COMMON_KEY"),_T("NUMERIC"),tmp_Common_Key);
        /*
		pDM->InitDBFieldData ();
	    pDM->MakeDeleteFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		*/
		/*
		pDM->AddDBFieldData (_T("TOC_YN"),_T("STRING"),_T("N"));
		pDM->AddDBFieldData (_T("CONTENTS_KEY"),_T("NUMERIC"),_T("")) ;
		pDM->AddDBFieldData (_T("TOC_USE_LIMIT_CODE"),_T("STRING"),_T(""));
		pDM->AddDBFieldData (_T("IDX_TOC_KEYWORD"),_T("STRING"),_T("")) ;
		pDM->AddDBFieldData (_T("TOC_CREATE_DATE"),_T("STRING"),_T(""));
		pDM->AddDBFieldData (_T("TOC_UPDATE_DATE"),_T("STRING"),_T(""));
		
		pDM->MakeUpdateFrame (_T("IDX_BO_TOC_TBL"),_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		*/

		pDM->InitDBFieldData ();
		pDM->AddDBFieldData (_T("ABSTRACTS_YN"),_T("STRING"),_T("N")) ;
		pDM->MakeUpdateFrame (_T("IDX_BO_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey) ;

		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		// 2003.11.3 이학중 추가 
		// IDX_BO_TBL UPDATE 시 IDX_TMP_BO_TBL 입력하기 
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		pDM->InitDBFieldData ();
// 		
// 		CString strTmp_Key ;
// 		CString system_Date ;
// 		CTime t = CTime::GetCurrentTime ();
// 		system_Date.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());
// 		
// 		pDM->MakeRecKey (strTmp_Key) ;
// 		pDM->AddDBFieldData (_T("REC_KEY"),_T("NUMERIC"),strTmp_Key);
// 		pDM->AddDBFieldData (_T("SPECIES_KEY"),_T("NUMERIC"),m_strSpeciesKey);
// 		pDM->AddDBFieldData (_T("IDX_WORK_FLAG"),_T("STRING"),_T("I"));
// 		pDM->AddDBFieldData (_T("INPUT_DATE"),_T("STRING"),system_Date);
// 		
// 		ids = pDM->MakeInsertFrame (_T("IDX_TMP_BO_TBL"));
// 		if(ids<0) {
// 			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 			return ;
// 		}
		//=====================================================
		
		
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        
		m_strAbsYN = _T("N") ;
        
        ids = SetDataMgrData(m_ParentDM ,_T("BS_초록유무"),_T("N"),m_idx) ;

		SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 없음>"));
		GetDlgItem(IDC_bADELETE)->EnableWindow(false);
		MessageBox(_T("초록을 삭제했습니다."));

		CESL_Grid *pGrid = (CESL_Grid*)FindControl(m_ParentCM , m_ParentGrid );
		pGrid->SetAt (m_idx,7,_T("N")) ;
	}
    
	if((m_strConYN == _T("N")) && (m_strAbsYN == _T("N")))
	{
		pDM->InitDBFieldData ();
		pDM->MakeDeleteFrame (_T("BO_TOC_COMMON_TBL"),_T("REC_KEY"),_T("NUMERIC"),tmp_Common_Key) ;

		
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //2003.11.3 이학중 추가 
		//BO_SPECIES_TBL 에 TOC_COMMON_KEY -> UPDATE 
        pDM->InitDBFieldData () ;
		//2003.11.12 이학중 수정 
		pDM->AddDBFieldData(_T("TOC_COMMON_KEY"),_T("NUMERIC"),_T("''"));
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		ids = pDM->MakeUpdateFrame (_T("BO_SPECIES_TBL"),_T("REC_KEY"),_T("NUMERIC"),m_strSpeciesKey);
		if(ids<0) {
		AfxMessageBox(_T("MakeUpdateFrame(BO_SPECIES_TBL) Error~!"));
		return ;
		}
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}
    
   

    pDM->SendFrame ();
	pDM->EndFrame ();

}

VOID CCONTENTMGR_02::OnbADELETE() 
{
	// TODO: Add your control notification handler code here
	
    if(IDYES == AfxMessageBox(_T("초록을 삭제 하시겠습니까 ?"),MB_YESNO,0) ) 
	{
	   Delete(1) ;
	   strOldAbstract = _T("");
	   m_ChorokDlg.SetChorok(_T(""));
	}
}
VOID CCONTENTMGR_02::OnbCDELETE() 
{
	// TODO: Add your control notification handler code here
//	AfxMessageBox(_T("목차를 삭제 하시겠습니까 ?"),MB_YESNO,0) ;
	
	if(IDYES == AfxMessageBox(_T("목차를 삭제 하시겠습니까 ?"),MB_YESNO,0) ) 
	{
	   Delete(0) ;
	   strOldContents = _T("");
	   m_TocEditdlg.SetTocFromString(_T(""));
	}
}

CString CCONTENTMGR_02::ReMake_BringInStream(INT nType,CString strStream)
{
	CString str_BringStream ;

	if(nType == 0)
	{  
         m_arrBringDefault[7] = _T("<body>목차\r\n")+strStream+_T("\r\n</body>") ;
		 for(INT i=0 ;i<8;i++)
		 {
			str_BringStream += m_arrBringDefault[i] ;
		 }
         //테스트후 삭제 할것 .
		 //AfxMessageBox (str_BringStream) ;
		 
	}
	else
	{


	}

    
	return str_BringStream ;
	
}


INT CCONTENTMGR_02::DbClick_Move (INT ndir)
{
	CString strShow;   
	INT ids ;

	// Initialize
	CString strWhere;
	CString strTemp;
	
	CESL_DataMgr* pDataMgr;
	CESL_DataMgr* pDataContents;

    pDataContents = FindDM(_T("DM_목차초록")) ;
	if(pDataContents == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}

	pDataMgr = FindDM(_T("DM_BO_CONTENTMGR_SPECIES"));
	if(pDataMgr == NULL) 
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -2 ;
	}

	CESL_ControlMgr *pParentCM = FindCM(m_ParentCM) ;
	if(pParentCM == NULL) {
		AfxMessageBox(_T("FindCM() Error~!")) ;
		return -3 ;
	}

	CESL_Grid *pGrid = (CESL_Grid*)pParentCM->FindControl(m_ParentGrid) ;
	if(pGrid == NULL)
	{
		AfxMessageBox(_T("FindControl() Error~!")) ;
		return -4;
	}



	INT nIdx = -1;

    ids = pGrid->SelectMakeList () ;
	if (m_idx >= 0)
	{
		nIdx = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
		while (nIdx >= 0)
		{
			if (m_idx == nIdx) break;
			nIdx = pGrid->SelectGetNext();
		}
	}
    INT nCount ;
	
	nCount = pGrid->SelectGetCount () ;
	if(nCount >0)
	{
		switch (ndir)
		{
		case 0 :

			pGrid->SetAt (pGrid->GetIdx (),0,_T("V")) ;
			//pGrid->SelectMakeList();
			m_idx = pGrid->GetIdx () ;
      
			nIdx = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
			while (nIdx >= 0)
			{
				if (m_idx == nIdx) break;
				nIdx = pGrid->SelectGetNext();
			}
			if (m_idx < 0)
			{
				//2004-07-09 이학중 
				//AfxMessageBox(_T("선택 목록을 구성할 수 없습니다."));	// 에러
				AfxMessageBox(_T("선정 항목이 없습니다."));	// 에러
				EndDialog(IDCANCEL);
				return -222;
			}
			break;
			
		case -1 :
		
			m_idx = SelectGetPrev (m_ParentCM ,m_ParentGrid);
			if (m_idx == -1) {
				m_idx = SelectGetHeadPosition(m_ParentCM ,m_ParentGrid);
				AfxMessageBox(_T(" 처음 자료입니다."));
				return -223;
			}
			break;
			
		case 1 :
		
			m_idx = SelectGetNext (m_ParentCM ,m_ParentGrid);
			if (m_idx == -1) {
				m_idx = SelectGetTailPosition(m_ParentCM ,m_ParentGrid);
				AfxMessageBox(_T(" 마지막 자료입니다."));
				return -224;
			}
			break;
		}


	}
    else
	{

		switch (ndir)
		{
		case 0 :
				  m_idx = pGrid->GetIdx () ;
				 
			break;

		case -1 :
				  if(m_idx ==0 )
				  {
					  AfxMessageBox (_T("처음 목록 입니다.")) ;
					  return - 333 ;
				  }

				  m_idx-- ;
				  
			break;
    
		case 1 :
				  if(m_idx+1 == pGrid->GetCount ())
				  {
					  AfxMessageBox (_T("마지막 목록 입니다.")) ;
					  return - 333 ;
				  }

				  m_idx++ ;


			break;
		}
	}

	   	// Member Initialize
	m_strCommonRecKey=_T("");  //목차 초록 공통정보의 레코드키(레코드 유무여부도 이것으로 체크)
	m_strCSource=_T("");	   //목차 원본
	m_strASource=_T("");	   //초록 원본
	m_strCCreateDate=_T("");   //목차 생성일
	m_strCUpdateDate=_T("");   //목차 수정일 
	m_strACreateDate=_T("");   //초록 생성일
	m_strAUpdateDate=_T("");   //초록 수정일 
	m_strMarc=_T("");	       //마크
	m_strFormCode=_T("");	   //형식구분
	m_strMatCode=_T("");	   //자료구분
	m_strRegCode=_T("");	   //등록구분
	m_strUseLimit=_T("");	   //이용제한구분
	m_strSpeciesKey=_T("");    //종키
	m_strControlNo=_T("");     //제어번호
//	m_strUbControlNo=_T("");   //.NET제어번호(key)
	m_strClassCode=_T("");	   //종구분
	m_strConYN=_T("");		   //목차유무
	m_strAbsYN=_T("");		   //목차유무
//	m_strNetConYN=_T("");	   //NET목차유무
//	m_strNetAbsYN=_T("");	   //NET초록유무
	m_strLibCode=_T("");	   //도서관부호 

	
	m_strConKey=_T("");	       //목차키
	m_strAbsKey=_T(""); 	   //초록키

    //<-  목차초록 최종 검색엔진 정리
    m_strWorkStatus;       //작업상태
    m_strPlaceInfo;        //자료있는곳
    //->  목차초록 최종 검색엔진 정리
  
  //  m_idx = ids; 
    SetReverse (m_ParentCM ,m_ParentGrid ,m_idx) ;
      
    ////////////////////////////////////////////////////////////////////////////////////////////
	INT nTempIndex ;
	CString sMokcha_Check ;
	nTempIndex = m_idx ;

	GetDataMgrData(m_ParentDM,_T("목차유무"),sMokcha_Check,nTempIndex) ;

	if(sMokcha_Check == _T("Y"))
	{
         m_nDlgType = 1 ;
	}
	else
	{
         m_nDlgType = 0 ; 
	}


	//m_nDlgType==0 : //입력(종선택후)
	//m_nDlgType==1 : //수정(종의 유무와상관없다.);

    // 1.목차 초록을 가져 온다. 2. 종정보 가져온다.

	if(m_nDlgType==0){

		m_nDataRowIdx = pGrid->GetIdx () ;

        GetDataMgrData (m_ParentDM ,_T("IBS_종KEY"),m_strSpeciesKey,m_nDataRowIdx );

		strWhere=_T(" toc.SPECIES_KEY=")+m_strSpeciesKey;

		INT ids = pDataContents ->RefreshDataManager (strWhere) ;

	}
	else
	{
		//목차 종키 가저오기 
      	
		m_nDataRowIdxForDlgType1 = pGrid->GetIdx () ;

        GetDataMgrData (m_ParentDM ,_T("IBS_종KEY"),m_strSpeciesKey,m_nDataRowIdxForDlgType1 ) ;

		strWhere=_T(" toc.SPECIES_KEY=")+m_strSpeciesKey;

		INT ids = pDataContents->RefreshDataManager (strWhere);
		if( pDataContents->GetRowCount () <= 0)
		{
		    AfxMessageBox(_T("목차 초록이 DB에 없기 때문에 수정 하실 수 없습니다.\n 새로 생성후 수정 하시기 바랍니다."));
			//return -1 ;
			OnCancel () ;
		}

      
    	GetDataMgrData (_T("DM_목차초록"),_T("종키"),m_strSpeciesKey,0) ;//종키
		if(_ttoi(m_strSpeciesKey.GetBuffer (0))>0){
			/*
			strWhere=_T(" i.rec_key=")+m_strSpeciesKey;
			//ECO_SetServerType(LS);
			pDataMgr->RefreshDataManager (strWhere) ;
			*/
		}
		else
		{
		//	pDataMgr->FreeData () ;
		}
	
		if(pDataMgr->GetRowCount()>0){
			m_nDataRowIdx= pGrid->GetIdx ();
		}else{
			m_nDataRowIdx=-1;
		}
  
	}

     if(pDataContents->GetRowCount()>0){
		
		pDataContents->GetCellData (_T("레코드키"),0,m_strCommonRecKey ) ;
		pDataContents->GetCellData (_T("목차키"),0,m_strConKey ) ;
		pDataContents->GetCellData (_T("목차"),0,m_strCSource ) ;
	    
		pDataContents->GetCellData (_T("목차생성일"),0,m_strCCreateDate ) ;
		pDataContents->GetCellData (_T("목차수정일"),0,m_strCUpdateDate ) ;
		pDataContents->GetCellData (_T("초록키"),0,m_strAbsKey ) ;
	     
		pDataContents->GetCellData (_T("초록"),0,m_strASource ) ;
		pDataContents->GetCellData (_T("초록생성일"),0,m_strACreateDate ) ;
		pDataContents->GetCellData (_T("초록수정일"),0,m_strAUpdateDate ) ;
	   
	
		pDataContents->GetCellData (_T("초록생성일"),0,m_strACreateDate ) ;
		pDataContents->GetCellData (_T("초록수정일"),0,m_strAUpdateDate ) ;
        
	 // pDataContents->GetCellData (_T("목차유무"),0,m_strConYN ) ;
	 //	pDataContents->GetCellData (_T("초록유무"),0,m_strAbsYN ) ;	
	
	    pDataContents->GetCellData (_T("제어번호"),0,m_strControlNo ) ;
		pDataContents->GetCellData (_T("종키"),0,m_strSpeciesKey ) ;
		
	}  

   if(m_nDataRowIdx>=0){
		// 입력일때나 수정인경우 종이 있을때만 셋팅된다.

        GetDataMgrData (m_ParentDM ,_T("IBS_형식구분_코드"),m_strFormCode ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_자료구분_코드"),m_strMatCode ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_이용제한구분_코드"),m_strUseLimit ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_종KEY"),m_strSpeciesKey ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_제어번호"),m_strControlNo ,m_nDataRowIdx ) ;
        GetDataMgrData (m_ParentDM ,_T("BS_목차유무"),m_strConYN ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("BS_초록유무"),m_strAbsYN ,m_nDataRowIdx ) ;
		m_strClassCode=_T("1");
	}

    
   	m_strLibCode=_T("011001");
	if(_ttoi(m_strCommonRecKey.GetBuffer (0))<=0)m_strCommonRecKey=_T("");
	if(_ttoi(m_strConKey.GetBuffer (0))<=0)m_strConKey=_T("");
	if(_ttoi(m_strAbsKey.GetBuffer (0))<=0)m_strAbsKey=_T("");
	if(_ttoi(m_strSpeciesKey.GetBuffer (0))<=0)m_strSpeciesKey=_T("");
	if(m_strConYN==_T("Y") || m_strConYN==_T("y"))m_strConYN==_T("Y"); else m_strConYN=_T("N");
	if(m_strAbsYN==_T("Y") || m_strAbsYN==_T("y"))m_strAbsYN==_T("Y"); else m_strAbsYN=_T("N");

     
/*
	if(m_nDlgType== 0 && m_strConYN == _T("Y"))
	{
		AfxMessageBox(_T("목차가 이미 생성되어 있습니다.목차 수정을 이용하시기 바랍니다."));
		OnCancel() ;
		return 0 ;

	}
*/
	// 3.화면을 셋팅한다.
	if(m_nDlgType==0){
		SetDlgItemText(IDC_eCONTROL_NO,m_strControlNo);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_본표제"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eTITLE,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_저작자"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eAUTHOR,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_발행자"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_ePUBLISHER,strTemp);
	}else{

        if(GetDataMgrData (m_ParentDM ,_T("IBS_제어번호"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eCONTROL_NO,strTemp);
		if(GetDataMgrData (m_ParentDM ,_T("IBS_본표제"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eTITLE,strTemp);
        if(GetDataMgrData (m_ParentDM ,_T("IBS_저작자"),strTemp,m_nDataRowIdx ) == 0)
			SetDlgItemText(IDC_eAUTHOR,strTemp);
        if(GetDataMgrData (m_ParentDM ,_T("IBS_발행자"),strTemp,m_nDataRowIdx ) == 0)
		{	SetDlgItemText(IDC_ePUBLISHER,strTemp);
			SetControlData (_T("CM_CONTENTMGR_02"),_T("발행자"),strTemp,-1) ;
		}


	}

	if(m_nDlgType==0)GetDlgItem(IDC_bSPECIES)->EnableWindow(false);
	else{
		if(m_nDataRowIdx>=0)GetDlgItem(IDC_bSPECIES)->EnableWindow(false);
		else GetDlgItem(IDC_bSPECIES)->EnableWindow(true);
	}

    // 4. 다시 화면을 셋팅한다.
	m_TocEditdlg.SetTocFromString(m_strCSource);
	m_ChorokDlg.SetChorok(m_strASource);

	GetDlgItem(IDC_bBRINGINC)->EnableWindow(true);

	if(m_strSpeciesKey!=_T(""))
	{
		if(m_strConYN==_T("Y") ){
			SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 있음>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
			m_TocEditdlg.SetReadOnly(false);
			SetDlgItemText(IDC_bSAVEC,_T("저  장"));
		}
		else{
			SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 없음>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
			if(m_strUbControlNo!=_T("") && m_strNetConYN==_T("Y")){ 
				m_TocEditdlg.SetReadOnly(true);
				SetDlgItemText(IDC_bSAVEC,_T("복  원"));
				GetDlgItem(IDC_bBRINGINC)->EnableWindow(false);
			}
			else
			{
				SetDlgItemText(IDC_bSAVEC,_T("저  장"));
				m_TocEditdlg.SetReadOnly(false);
			}
		}
	}else{
		if(m_strConKey!=_T("") ){
			SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 있음>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
		}else{
			SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 없음>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
		}
		SetDlgItemText(IDC_bSAVEC,_T("저  장"));
		m_TocEditdlg.SetReadOnly(false);
	}


	
	GetDlgItem(IDC_bBRINGINA)->EnableWindow(true);

	if(m_strSpeciesKey!=_T("")){
		if(m_strAbsYN==_T("Y"))
		{
			SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 있음>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(true);
			SetDlgItemText(IDC_bSAVEA,_T("저  장"));
			m_ChorokDlg.SetReadOnly(false);
		}else{
			// KOLIS 서버에서 삭제
			SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 없음>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(false);
			if(m_strUbControlNo!=_T("") && m_strNetAbsYN==_T("Y")){ 
				m_ChorokDlg.SetReadOnly(true);
				SetDlgItemText(IDC_bSAVEA,_T("복  원"));
				GetDlgItem(IDC_bBRINGINA)->EnableWindow(false);
			}else{
				SetDlgItemText(IDC_bSAVEA,_T("저  장"));
				m_ChorokDlg.SetReadOnly(false);
			}
		}
	}else{
		if(m_strAbsKey!=_T(""))
		{
			SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 있음>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(true);
		}else{
			// KOLIS 서버에서 삭제
			SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 없음>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(false);
		}
		SetDlgItemText(IDC_bSAVEA,_T("저  장"));
		m_ChorokDlg.SetReadOnly(false);
	}

   
    
    CString tmp_Title ,tmp_PubYear ,tmp_Publisher;

    ids = GetDataMgrData (m_ParentDM ,_T("IBS_본표제"),tmp_Title,m_idx);
    ids = GetDataMgrData (m_ParentDM ,_T("IBS_발행년도"),tmp_PubYear,m_idx);
    ids = GetDataMgrData (m_ParentDM ,_T("IBS_발행자"),tmp_Publisher,m_idx); 

	CString strDefualt[8];
    strDefualt[0] = m_strControlNo ;            // 제어번호
	strDefualt[1] = _T("") ;               // 청구기호 
	strDefualt[2] = _T("") ;               // 총페이지수
	strDefualt[3] = tmp_PubYear ;               // 발행년도
	strDefualt[4] = tmp_Publisher ;             // 발행자
	strDefualt[5] = _T("") ;               // 총서명
    strDefualt[6] = tmp_Title ;                 // 서명 
    strDefualt[7] = _T("") ;

	m_TocEditdlg.SetTocDefault (strDefualt) ;
    
	/*
	<id>
	<call>
	<count>
	<date>2003
	<pub>나
	<sertitle>
	<title>기증두 문제있나?
	<body>목차

	</body>
    */
   
    m_arrBringDefault[0] = _T("<id>")+strDefualt[0]+_T("\r\n");
	m_arrBringDefault[1] = _T("<call>")+strDefualt[1]+_T("\r\n"); 
    m_arrBringDefault[2] = _T("<count>")+strDefualt[2]+_T("\r\n") ;
	m_arrBringDefault[3] = _T("<date>")+strDefualt[3]+_T("\r\n");    
	m_arrBringDefault[4] = _T("<pub>")+strDefualt[4]+_T("\r\n") ;
    m_arrBringDefault[5] = _T("<sertitle>")+strDefualt[5]+_T("\r\n") ;
    m_arrBringDefault[6] = _T("<title>")+strDefualt[6]+_T("\r\n") ;
			  
    //m_arrBringDefault[7] = _T("<body>")+strDefualt[7]+_T("\r\n</body>") ;
    
    CString strQuery ;
	CString strConKeyword ,strAbsKeyword ;
	if(m_strSpeciesKey!=_T("") && m_strConYN==_T("Y"))
	{
	     
		strQuery.Format (_T("select IDX_TOC_KEYWORD from IDX_BO_TOC_TBL where species_key = %s"),m_strSpeciesKey);
		pDataContents->GetOneValueQuery (strQuery,strConKeyword);
        ((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->SetWindowText(strConKeyword);

	}

	if(m_strSpeciesKey!=_T("") && m_strAbsYN ==_T("Y"))
	{
        strQuery.Format (_T("select IDX_ABS_KEYWORD from IDX_BO_TOC_TBL where rec_key = %s"),m_strSpeciesKey);
		pDataContents->GetOneValueQuery (strQuery,strAbsKeyword);
        ((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->SetWindowText(strAbsKeyword);

	}
  
	return 0 ;
}


//////////////////////////////////////////////////////////////////////////
//목차 키워드 생성 
//////////////////////////////////////////////////////////////////////////
VOID CCONTENTMGR_02::OnbMOKKEYWORD() 
{
	// TODO: Add your control notification handler code here
	CString sBody ;
	sBody=m_TocEditdlg.GetFinalToc();
	if(m_TocEditdlg.CheckSyntax())return ;	

    CMakeSearchData *pMakeData = NULL;
    pMakeData = new CMakeSearchData(this); 

	INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//한자 한글 변환
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
		return ;
	}

	MakeIndexKeyWord(sBody) ;
   
	LPCTSTR lpszTok = _tcstok(sBody.GetBuffer(0), _T(" "));
	CString tmp_Data;
	CString strKeyWord;
	
	while (lpszTok)
	{   
		tmp_Data.Format (_T("%s"),lpszTok);
		iCh = pMakeData->GetKeyWord( tmp_Data );				//공백, 특수문자 제거
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			return ;
		}

		strKeyWord +=  _T(" ") + tmp_Data;

		lpszTok = _tcstok(NULL, _T(" "));
	}

	strKeyWord.TrimLeft ();

	//중복 키워드 제거
	CArray<CString, CString&> pArrayKeyword;
	strKeyWord += _T(" ");
	INT iPos = strKeyWord.Find( _T(" ") );
	while ( iPos != -1 ) {
		CString sItem = strKeyWord.Left( iPos );
		INT iFind = 0;
		for ( INT i = 0 ; i < pArrayKeyword.GetSize() ; i++ ) {
			if ( sItem == pArrayKeyword.GetAt(i) ) {
				iFind = 1;
				break;
			}
		}
		if ( iFind == 0 ) {
			pArrayKeyword.Add( sItem );
		}
		strKeyWord = strKeyWord.Mid( iPos+1 );
		iPos = strKeyWord.Find( _T(" ") );
	}
	strKeyWord = _T("");

	for ( INT i = 0 ; i < pArrayKeyword.GetSize() ; i++ ) {
		strKeyWord = strKeyWord + _T(" ") + pArrayKeyword.GetAt(i);
	}

	if( strKeyWord.GetLength() > 1)
		strKeyWord = strKeyWord.Mid(1);

    CEdit * pEdit = (CEdit*)GetDlgItem(IDC_eMOK_KEYWORD) ;
	pEdit->SetWindowText (_T(""));
	pEdit->SetWindowText (strKeyWord) ;

	m_KeyWord_update = true ;
	m_strUpdateKeyword = strKeyWord ;
 
	delete pMakeData ;
}

VOID CCONTENTMGR_02::OnbCHOKEYWORD() 
{
	CString sBody ;
	sBody=m_ChorokDlg.GetChorokText();    

    CMakeSearchData *pMakeData = NULL;
    pMakeData = new CMakeSearchData(this); 

	INT iCh = pMakeData->ChangeHanjaToHangul( sBody );	//한자 한글 변환
	if ( 0 != iCh ) {			
		AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
		return ;
	}

	MakeIndexKeyWord(sBody) ;
   
	LPCTSTR lpszTok = _tcstok(sBody.GetBuffer(0), _T(" "));
	CString tmp_Data;
	CString strKeyWord;
	
	while (lpszTok)
	{   
		tmp_Data.Format (_T("%s"),lpszTok);
		iCh = pMakeData->GetKeyWord( tmp_Data );				//공백, 특수문자 제거
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( sBody ) error") );
			return ;
		}

		strKeyWord +=  _T(" ") + tmp_Data;

		lpszTok = _tcstok(NULL, _T(" "));
	}

	strKeyWord.TrimLeft ();

	//중복 키워드 제거
	CArray<CString, CString&> pArrayKeyword;
	strKeyWord += _T(" ");
	INT iPos = strKeyWord.Find( _T(" ") );
	while ( iPos != -1 ) {
		CString sItem = strKeyWord.Left( iPos );
		INT iFind = 0;
		for ( INT i = 0 ; i < pArrayKeyword.GetSize() ; i++ ) {
			if ( sItem == pArrayKeyword.GetAt(i) ) {
				iFind = 1;
				break;
			}
		}
		if ( iFind == 0 ) {
			pArrayKeyword.Add( sItem );
		}
		strKeyWord = strKeyWord.Mid( iPos+1 );
		iPos = strKeyWord.Find( _T(" ") );
	}
	strKeyWord = _T("");

	for ( INT i = 0 ; i < pArrayKeyword.GetSize() ; i++ ) {
		strKeyWord = strKeyWord + _T(" ") + pArrayKeyword.GetAt(i);
	}

	if( strKeyWord.GetLength() > 1)
		strKeyWord = strKeyWord.Mid(1);

    CEdit * pEdit = (CEdit*)GetDlgItem(IDC_eCHO_KEYWORD) ;
	pEdit->SetWindowText (_T(""));
	pEdit->SetWindowText (strKeyWord) ;

    m_KeyWord_update = true ;
	m_strUpdateKeyword = strKeyWord ;

	delete pMakeData ;
}

VOID CCONTENTMGR_02::OnbDCMInput() 
{
	// TODO: Add your control notification handler code here
	
	CString strDBFLAG ;
	strDBFLAG = _T("M");

	CString sWhere = _T("");
	CString strTempContents = _T(""); 
    CString strBackup = _T("");

	CESL_DataMgr *pDM = FindDM(m_ParentDM );
	if(pDM == NULL)
	{
		AfxMessageBox(_T("FindDM() Error~!"));
        return ;
	}
     
	//테스트 후 삭제 할 것 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//m_strSpeciesKey.Format (_T("%d"),60212958) ;

	sWhere.Format (_T("SELECT CONTENTS FROM CO_DCM_TOC_TBL WHERE SPECIES_KEY = %s"),m_strSpeciesKey ) ;

	pDM->GetOneValueQuery (sWhere,strTempContents) ;
	
	if(strTempContents.IsEmpty ())
	{   
		AfxMessageBox(_T("관련 자료가 없습니다."));
		return ;
	}
    else
	{   
		
		strBackup=m_TocEditdlg.GetFinalToc();
        
        CString strDCMStream = _T("");
        //strDCMStream = strTempContents ;
        INT nStart ,nEnd ;
        nStart = strTempContents.Find (_T("<body>"));
        nEnd   = strTempContents.Find (_T("</body>"));
		if(nStart <0 ) return ;

		strDCMStream = strTempContents.Mid (nStart+6, nEnd-nStart-6) ; 
        strDCMStream.TrimRight ();
		strDCMStream.TrimLeft ();

        strTempContents = ReMake_BringInStream(0,strDCMStream) ;
        strTempContents.TrimLeft ();
		strTempContents.TrimRight ();
		m_TocEditdlg.SetTocFromString(strTempContents);

	}
    
    
    // 3. 그대로 진행할것인지 사용자에게 묻는다.
	if(AfxMessageBox(_T("계속하시겠습니까?"),MB_YESNO)==IDNO)
	{
		m_TocEditdlg.SetTocFromString( strBackup );
		return ;
	}

	
}

CString CCONTENTMGR_02::KeywordMake( CString strItemData )
{
EFS_BEGIN
/*
	if ( !strItemData.IsEmpty() ) {		
		INT iCh = m_MakeSearchData->ChangeHanjaToHangul(strItemData);	//한자 한글 변환
		if ( 0 != iCh ) {			
			AfxMessageBox ( _T("pMakeData->ChangeHanjaToHangul( strItemData ) error") );
			return _T("");
		}
		iCh = m_MakeSearchData->GetKeyWord(strItemData);	//공백, 특수문자 제거
		if ( 0 != iCh ) {						
			return _T("");
		}
		*/

		if (strItemData.IsEmpty()) return _T("");
		
		//단어에 포함될 경우 키워드 생성
		INT iWord = SearchWord( strItemData );
		if ( iWord == 1 ) {
			// 키워드 채택
			return strItemData;
		}
		
		while (1)
		{			
			// 불용어에 포함될 경우 키워드를 생성하지 않는다.
			INT iUnlessWord = SearchUnlessWord( strItemData );
			if ( iUnlessWord == 1 ) return _T("");
			
			//조사에 포함될 경우 조사를 삭제하고 키워드 생성
			CString sResult=_T("");
			INT iAuxiliaryWord = SearchAuxiliaryWord( strItemData, sResult );
			if ( iAuxiliaryWord == 1 ) 
			{
				strItemData = sResult;				
			}
			else
			{
				// 키워드 채택
				return strItemData;				
			}
			
		}
	//}

	return _T("");

EFS_END
return _T("");
}


INT CCONTENTMGR_02::SearchWord(CString sWord)
{
EFS_BEGIN

	INT ids;

	ids = SearchInFile(_T("..\\CFG\\CO_WORD_TBL.TXT"), sWord);
	if ( ids ) return ids;	
	return 0;

EFS_END
return -1;

}

INT CCONTENTMGR_02::SearchAuxiliaryWord( CString sWord, CString &sResult )
{
EFS_BEGIN

	// 
	CString strWord;
	INT ids;
	INT i;
	INT nLength;

	nLength = sWord.GetLength();

	for ( i=1 ; i<nLength ; i++ )
	{
		strWord = sWord.Right(nLength-i);
		ids = SearchInFile(_T("..\\CFG\\CO_AUXILIARY_WORD_TBL.TXT"), strWord);
		if ( ids == 1) 
		{
			sResult = sWord.Left(i);
			return ids;
		}
	}	

EFS_END
return -1;

}

//INT CESL_DataMgr::GetCellData(INT rowidx, INT idx, CString &result)
INT CCONTENTMGR_02::SearchUnlessWord( CString sWord )
{
EFS_BEGIN

	INT ids;

	ids = SearchInFile(_T("..\\CFG\\CO_UNLESS_WORD_TBL.TXT"), sWord);
	if ( ids ) return ids;
	return 0;

EFS_END
return -1;

}

INT CCONTENTMGR_02::LoadVolInfo()
{
	CESL_DataMgr* pVolDataMgr;
	CString	      strWhere;
	
	pVolDataMgr = FindDM(_T("DM_CONTENTS_VOL")) ;
	if(pVolDataMgr == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}

	strWhere.Format(_T("SPECIES_KEY=%s AND BOOK_APPENDIX_FLAG = 'B'"), m_strSpeciesKey);
	pVolDataMgr->RefreshDataManager(strWhere);		

	m_pGrid->Display();	

	return 0;
}

BEGIN_EVENTSINK_MAP(CCONTENTMGR_02, CDialog)
    //{{AFX_EVENTSINK_MAP(CCONTENTMGR_BO_EDIT)
	ON_EVENT(CCONTENTMGR_02, IDC_GRID_VOLUME, -600 /* Click */, OnClickGridVolume, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


VOID CCONTENTMGR_02::OnClickGridVolume() 
{
	PrevNextProc(0);
}

INT CCONTENTMGR_02::PrevNextProc(INT nPrevNext)
{	
	CESL_DataMgr* pDataContents;
	CString sMokcha_Check ;
	CString strWhere;
	INT		nTempIndex ;
	INT		ids;

	nTempIndex = m_idx ;	

    pDataContents = FindDM(_T("DM_목차초록")) ;
	if(pDataContents == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}

	CESL_DataMgr* pVolDataMgr;
		
	pVolDataMgr = FindDM(_T("DM_CONTENTS_VOL")) ;
	if(pVolDataMgr == NULL )
	{
		AfxMessageBox(_T("FindDM() Error~!")) ;
		return -1;
	}

	// 1.목차 초록을 가져 온다. 2. 종정보 가져온다.
	// 권별정보의 Vol을 확인
	CString strVolSortNo;
	INT nRow = m_pGrid->GetRow();	
	
	pVolDataMgr->GetCellData(_T("일련번호"), nRow-1, strVolSortNo );	

	GetDataMgrData (m_ParentDM ,_T("IBS_종KEY"),m_strSpeciesKey,m_nDataRowIdx );
	
	if ( strVolSortNo.IsEmpty() || m_str90 != _T("Y"))
	{
		strWhere.Format(_T("toc.SPECIES_KEY=%s"), m_strSpeciesKey);	
	}
	else
	{
		strWhere.Format(_T("toc.SPECIES_KEY=%s AND toc.vol_sort_no=%s"), m_strSpeciesKey, strVolSortNo);	
	}
	
	pDataContents->RefreshDataManager (strWhere) ;

	// 2006.1.2 ADD BY PDJ
	// 종에 대한 목차유무별 판단기준에서 권호별 목차 유무로 판단
	m_strConYN = _T("N");
	m_strAbsYN = _T("N");
	if ( pDataContents->GetRowCount() > 0 )
	{
		// 수정모드
		m_nDlgType = 1;		
	}
	else
	{
		// 입력모드
		m_nDlgType = 0;			
	}

	// 수정이면 목차,초록을 보여준다.
	if ( m_nDlgType == 1 )
	{
		pDataContents->GetCellData (_T("레코드키"),0,m_strCommonRecKey ) ;
		pDataContents->GetCellData (_T("목차키"),0,m_strConKey ) ;
		pDataContents->GetCellData (_T("목차"),0,m_strCSource ) ;
		
		pDataContents->GetCellData (_T("목차생성일"),0,m_strCCreateDate ) ;
		pDataContents->GetCellData (_T("목차수정일"),0,m_strCUpdateDate ) ;
		pDataContents->GetCellData (_T("초록키"),0,m_strAbsKey ) ;
		
		pDataContents->GetCellData (_T("초록"),0,m_strASource ) ;
		pDataContents->GetCellData (_T("초록생성일"),0,m_strACreateDate ) ;
		pDataContents->GetCellData (_T("초록수정일"),0,m_strAUpdateDate ) ;	   		
		
		pDataContents->GetCellData (_T("제어번호"),0,m_strControlNo ) ;
		pDataContents->GetCellData (_T("종키"),0,m_strSpeciesKey ) ;
		
		// 목차 초록 유무
		if ( m_strConKey.IsEmpty() ) m_strConYN = _T("N");
		else m_strConYN = _T("Y");

		if ( m_strAbsKey.IsEmpty() ) m_strAbsYN = _T("N");
		else m_strAbsYN = _T("Y");
	}
	else
	{
		// 입력이면 변수 초기화
		// 1. 공통정보 초기화
		m_strCommonRecKey = _T("");
		m_strConKey = _T("");
		m_strAbsKey = _T("");
		m_strSpeciesKey = _T("");
		
		// 2. 목차정보 초기화		
		m_strCSource = _T("");
		m_strCCreateDate = _T("");
		m_strCUpdateDate = _T("");
		
		// 3. 초록정보 초기화
		m_strASource = _T("");
		m_strACreateDate = _T("");
		m_strAUpdateDate = _T("");		
	}
	
	if(m_nDataRowIdx>=0){
		// 입력일때나 수정인경우 종이 있을때만 셋팅된다.
		
        GetDataMgrData (m_ParentDM ,_T("IBS_형식구분_코드"),m_strFormCode ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_자료구분_코드"),m_strMatCode ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_이용제한구분_코드"),m_strUseLimit ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_종KEY"),m_strSpeciesKey ,m_nDataRowIdx ) ;
		GetDataMgrData (m_ParentDM ,_T("IBS_제어번호"),m_strControlNo ,m_nDataRowIdx ) ;
        //GetDataMgrData (m_ParentDM ,_T("BS_목차유무"),m_strConYN ,m_nDataRowIdx ) ;
		//GetDataMgrData (m_ParentDM ,_T("BS_초록유무"),m_strAbsYN ,m_nDataRowIdx ) ;
		m_strClassCode=_T("1");
	}       			
	
     
	if(m_nDlgType==0)GetDlgItem(IDC_bSPECIES)->EnableWindow(false);
	else{
		if(m_nDataRowIdx>=0)GetDlgItem(IDC_bSPECIES)->EnableWindow(false);
		else GetDlgItem(IDC_bSPECIES)->EnableWindow(true);
	}

    // 4. 다시 화면을 셋팅한다.
	m_TocEditdlg.SetTocFromString(m_strCSource);
	m_ChorokDlg.SetChorok(m_strASource);

	GetDlgItem(IDC_bBRINGINC)->EnableWindow(true);

	if(m_strSpeciesKey!=_T(""))
	{
		if(m_strConYN==_T("Y") ){
			SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 있음>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
			m_TocEditdlg.SetReadOnly(false);
			SetDlgItemText(IDC_bSAVEC,_T("저  장"));
		}
		else{
			SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 없음>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
			if(m_strUbControlNo!=_T("") && m_strNetConYN==_T("Y")){ 
				m_TocEditdlg.SetReadOnly(true);
				SetDlgItemText(IDC_bSAVEC,_T("복  원"));
				GetDlgItem(IDC_bBRINGINC)->EnableWindow(false);
			}
			else
			{
				SetDlgItemText(IDC_bSAVEC,_T("저  장"));
				m_TocEditdlg.SetReadOnly(false);
			}
		}
	}else{
		if(m_strConKey!=_T("") ){
			SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 있음>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(true);
		}else{
			SetDlgItemText(IDC_STATIC_01,_T("<목 차 : 없음>"));
			GetDlgItem(IDC_bCDELETE)->EnableWindow(false);
		}
		SetDlgItemText(IDC_bSAVEC,_T("저  장"));
		m_TocEditdlg.SetReadOnly(false);
	}


	
	GetDlgItem(IDC_bBRINGINA)->EnableWindow(true);

	if(m_strSpeciesKey!=_T("")){
		if(m_strAbsYN==_T("Y"))
		{
			SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 있음>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(true);
			SetDlgItemText(IDC_bSAVEA,_T("저  장"));
			m_ChorokDlg.SetReadOnly(false);
		}else{
			// KOLIS 서버에서 삭제
			SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 없음>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(false);
			if(m_strUbControlNo!=_T("") && m_strNetAbsYN==_T("Y")){ 
				m_ChorokDlg.SetReadOnly(true);
				SetDlgItemText(IDC_bSAVEA,_T("복  원"));
				GetDlgItem(IDC_bBRINGINA)->EnableWindow(false);
			}else{
				SetDlgItemText(IDC_bSAVEA,_T("저  장"));
				m_ChorokDlg.SetReadOnly(false);
			}
		}
	}else{
		if(m_strAbsKey!=_T(""))
		{
			SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 있음>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(true);
		}else{
			// KOLIS 서버에서 삭제
			SetDlgItemText(IDC_STATIC_02,_T("<초 록 : 없음>"));
			GetDlgItem(IDC_bADELETE)->EnableWindow(false);
		}
		SetDlgItemText(IDC_bSAVEA,_T("저  장"));
		m_ChorokDlg.SetReadOnly(false);
	}
    
    CString tmp_Title ,tmp_PubYear ,tmp_Publisher;

    ids = GetDataMgrData (m_ParentDM ,_T("IBS_본표제"),tmp_Title,m_idx);
    ids = GetDataMgrData (m_ParentDM ,_T("IBS_발행년도"),tmp_PubYear,m_idx);
    ids = GetDataMgrData (m_ParentDM ,_T("IBS_발행자"),tmp_Publisher,m_idx); 

	CString strDefualt[8];
    strDefualt[0] = m_strControlNo ;            // 제어번호
	strDefualt[1] = _T("") ;               // 청구기호 
	strDefualt[2] = _T("") ;               // 총페이지수
	strDefualt[3] = tmp_PubYear ;               // 발행년도
	strDefualt[4] = tmp_Publisher ;             // 발행자
	strDefualt[5] = _T("") ;               // 총서명
    strDefualt[6] = tmp_Title ;                 // 서명 
    strDefualt[7] = _T("") ;

	m_TocEditdlg.SetTocDefault (strDefualt) ;
    
	m_arrBringDefault[0] = _T("<id>")+strDefualt[0]+_T("\r\n");
	m_arrBringDefault[1] = _T("<call>")+strDefualt[1]+_T("\r\n"); 
	m_arrBringDefault[2] = _T("<count>")+strDefualt[2]+_T("\r\n") ;
	m_arrBringDefault[3] = _T("<date>")+strDefualt[3]+_T("\r\n");    
	m_arrBringDefault[4] = _T("<pub>")+strDefualt[4]+_T("\r\n") ;
	m_arrBringDefault[5] = _T("<sertitle>")+strDefualt[5]+_T("\r\n") ;
	m_arrBringDefault[6] = _T("<title>")+strDefualt[6]+_T("\r\n") ;

	if ( !m_strCommonRecKey.IsEmpty() )
	{
		CString strQuery ;
		CString strConKeyword ,strAbsKeyword, strValue ;
		CString strConUseLimitCode, strAbsUseLimitCode;
		INT		nOffset, nPos, nIdx;

		strQuery.Format (_T("select IDX_TOC_KEYWORD||'↕'||TOC_USE_LIMIT_CODE||'↕'||IDX_ABS_KEYWORD||'↕'||ABS_USE_LIMIT_CODE from IDX_BO_TOC_TBL where REC_KEY = %s"),m_strCommonRecKey);
		pDataContents->GetOneValueQuery (strQuery,strValue);
		if ( strValue.IsEmpty() )
		{
			strConKeyword=_T("");
			strAbsKeyword=_T("");
			strConUseLimitCode=_T("");
			strAbsUseLimitCode=_T("");		
		}
		else
		{
			nOffset = 0;
			nPos = 0;
			nIdx = 0;

			while ( nOffset >= 0 ) 
			{
				nOffset = strValue.Find(_T("↕"), nPos );
				switch ( nIdx )
				{
				case 0:
					strConKeyword = strValue.Mid(nPos, nOffset-nPos);
					break;
				case 1:
					strConUseLimitCode = strValue.Mid(nPos, nOffset-nPos);
					break;
				case 2:
					strAbsKeyword = strValue.Mid(nPos, nOffset-nPos);
					break;
				case 3:
					strAbsUseLimitCode = strValue.Mid(nPos, strValue.GetLength()-nPos);
					break;
				default:
					break;
				}
				nPos = nOffset+1;
				nIdx++;
			} // END WHILE			
		} // END IF
			
		if( strConUseLimitCode != _T("") )
			SetControlData (_T("CM_CONTENTMGR_02"),_T("목차이용제한구분"),strConUseLimitCode) ;
		if( strAbsUseLimitCode != _T("") )
			SetControlData (_T("CM_CONTENTMGR_02"),_T("초록이용제한구분"),strAbsUseLimitCode) ;	
		((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->SetWindowText(strConKeyword);
		((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->SetWindowText(strAbsKeyword);
	}
	else
	{
//		SetControlData (_T("CM_CONTENTMGR_02"),_T("목차이용제한구분"),_T("")) ;
//		SetControlData (_T("CM_CONTENTMGR_02"),_T("초록이용제한구분"),_T("")) ;
		((CEdit*)GetDlgItem(IDC_eMOK_KEYWORD))->SetWindowText(_T(""));
		((CEdit*)GetDlgItem(IDC_eCHO_KEYWORD))->SetWindowText(_T(""));
	}
	

	return 0;
}


VOID CCONTENTMGR_02::MakeSemiCollection(INT nMode, CESL_DataMgr* pDM, CString strContentsIn )
{
	CString strDate;
	CString strRecKey;
	CString strQuery;	
	
	// 기존 색인 삭제
	// 무조건 지운다.
	// DELETE & INSERT
	// 키가 없다면 최초생성이다. => 지울게 없음.
	if ( m_strSpeciesKey.IsEmpty() || m_strConKey.IsEmpty() ) return;
	strQuery.Format(_T("INSERT INTO IDX_TMP_MBO_TOC_TBL (REC_KEY, MBO_TOC_KEY,IDX_WORK_FLAG,INPUT_DATE) ")
		_T("( SELECT ESL_SEQ.nextval, REC_KEY, 'D', sysdate FROM IDX_MBO_TOC_TBL WHERE CONTENTS_KEY=%s AND SPECIES_KEY = %s );"), 		
		m_strConKey, m_strSpeciesKey);
	pDM->ExecuteQuery(strQuery, FALSE, FALSE );	

	strQuery.Format(_T("DELETE FROM IDX_MBO_TOC_TBL WHERE CONTENTS_KEY=%s AND SPECIES_KEY = %s; "), 
		m_strConKey, m_strSpeciesKey );
	pDM->ExecuteQuery(strQuery, FALSE, FALSE );	

	// 삭제 모드이면 삭제만 하고 return
	if ( nMode == 1 ) 
	{
		return;
	}
			
	CTime t = CTime::GetCurrentTime ();
	strDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());


	CString title,ori_title,author,ori_author, strConSortNo;
	CString OneLine;
	
	// 01.
	INT idx = strContentsIn.Find(_T("<body>"),0);
	// 02.
	idx=strContentsIn.Find(_T("\n"),idx);
	
	// 03.	
	INT s_idx=idx+1,e_idx=idx+1;	
	INT con_sort_no=1;

	CIndex index;	
	
	while(strContentsIn.Mid(s_idx,7)!=_T("</body>"))
	{
		e_idx=strContentsIn.Find(_T("\n"),s_idx);			
		if(e_idx==-1) break;
		OneLine=strContentsIn.Mid(s_idx,e_idx-s_idx);

		INT token_idx=OneLine.Find(_T("//"),0);
		if(token_idx!=-1)
		{
			ori_title=title=OneLine.Mid(0,token_idx);
			ori_author=author=OneLine.Mid(token_idx+2);
			ori_author.Remove('\r');
		}
		else
		{
			ori_title=title=OneLine;
			ori_title.Remove('\r');
			ori_author=author=_T("");
		}

		// 02. TITLE
		index.MakeIndex_NonSpace(title);
		index.ConvertHanjaToHangul(title);
		
		title.Replace(_T("ㅇ"),_T(""));
		title.TrimLeft();
		title.TrimRight();		

		strConSortNo.Format(_T("%d"), con_sort_no);

		// 03. AUTHOR
		index.MakeIndex_NonSpace(author);
		index.ConvertHanjaToHangul(author);	
		
		// DB 작업
		pDM->InitDBFieldData () ;
		pDM->MakeRecKey (strRecKey) ;
		pDM->AddDBFieldData (_T("REC_KEY")					, _T("NUMERIC")	,strRecKey);
		pDM->AddDBFieldData (_T("CONTENTS_KEY")				, _T("NUMERIC")	,m_strConKey);
		pDM->AddDBFieldData (_T("SPECIES_KEY")				, _T("NUMERIC")	,m_strSpeciesKey);
		pDM->AddDBFieldData (_T("CON_SORT_NO")				, _T("NUMERIC")	,strConSortNo);
		pDM->AddDBFieldData (_T("IDX_BO_TOC_TITLE")			, _T("STRING")	,title);
		pDM->AddDBFieldData (_T("IDX_BO_TOC_AUTHOR")		, _T("STRING")	,author);
		pDM->AddDBFieldData (_T("IDX_BO_TOC_TITLE_INFO")	, _T("STRING")	,ori_title);
		pDM->AddDBFieldData (_T("IDX_BO_TOC_AUTHOR_INFO")	, _T("STRING")	,ori_author);
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		ids = pDM->MakeInsertFrame (_T("IDX_MBO_TOC_TBL"));
// 		if(ids<0) {
// 			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 			return ;
// 		}		
		//=====================================================
		
				
		// 2. 세미나 자료 컬렉션 생성 TEMP TABLE : IDX_TMP_MBO_TOC_TBL			
		//=====================================================
		//2009.01.20 REM BY PJW : IDX_TMP_BO_TBL,IDX_TMP_SE_TBL 사용안함 으로 주석처리함
// 		pDM->InitDBFieldData () ;
// 		pDM->AddDBFieldData (_T("REC_KEY")			, _T("NUMERIC")	,strRecKey);
// 		pDM->AddDBFieldData (_T("MBO_TOC_KEY")		, _T("NUMERIC")	,strRecKey);
// 		pDM->AddDBFieldData (_T("IDX_WORK_FLAG")	, _T("STRING")	,_T("I"));				
// 		pDM->AddDBFieldData (_T("INPUT_DATE")		, _T("STRING")	,strDate);	
// 		ids = pDM->MakeInsertFrame (_T("IDX_TMP_MBO_TOC_TBL"));
// 		if(ids<0) {
// 			AfxMessageBox(_T("MakeInsertFrame(IDX_TMP_BO_TBL) Error~!"));
// 			return ;
// 		}		
		//=====================================================
		
		
		s_idx=e_idx+1;
		con_sort_no++;
	}
}



HBRUSH CCONTENTMGR_02::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

