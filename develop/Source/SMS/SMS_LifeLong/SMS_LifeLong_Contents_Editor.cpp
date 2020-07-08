// SMS_LifeLong_Contents_Editor.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_LifeLong_Contents_Editor.h"
#include "SMS_LifeLong1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Contents_Editor dialog


CSMS_LifeLong_Contents_Editor::CSMS_LifeLong_Contents_Editor(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_LifeLong_Contents_Editor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CSMS_LifeLong_Contents_Editor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_LifeLong_Contents_Editor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_LifeLong_Contents_Editor, CDialog)
	//{{AFX_MSG_MAP(CSMS_LifeLong_Contents_Editor)
	ON_CBN_SELCHANGE(IDC_cSMS_TYPE, OnSelchangecSMSTYPE)
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_EN_CHANGE(IDC_eSMS_CONTENTS, OnChangeeSMSCONTENTS)
	ON_BN_CLICKED(IDC_chkLibName, OnchkLibName)
	ON_BN_CLICKED(IDC_chkReciever, OnchkReciever)
	ON_BN_CLICKED(IDC_chkProgram, OnchkProgram)
	ON_BN_CLICKED(IDC_chkOpen, OnchkOpen)
	ON_BN_CLICKED(IDC_chkDefaultMsg, OnchkDefaultMsg)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_EN_CHANGE(IDC_eURL, OnChangeeURL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Contents_Editor message handlers

BOOL CSMS_LifeLong_Contents_Editor::Create(CWnd* pParentWnd) 
{
EFS_BEGIN	
	return CDialog::Create(IDD, pParentWnd);
EFS_END
	return FALSE;
}

BOOL CSMS_LifeLong_Contents_Editor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	/////////////////////////////////////////////////////////////////////////
	// 0. 개발자화면 감추기
	/////////////////////////////////////////////////////////////////////////
	FILE* pf = _tfopen( _T("..\\CFG\\_debug"), _T("rb") );
	if ( pf == NULL )
	{
		CRect rc;
		this->GetClientRect(rc);		
		this->MoveWindow( rc.left,rc.top, rc.Width() - 310, rc.Height() + 25 );
	}

	/////////////////////////////////////////////////////////////////////////
	// 1. SMS 콤보박스를 초기화 한다.
	/////////////////////////////////////////////////////////////////////////
	ids = InitSMSGroup();
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::OnInitDialog") );

	/////////////////////////////////////////////////////////////////////////
	// 2. 문자 구성 체크박스 초기화 
	/////////////////////////////////////////////////////////////////////////
//	ids =InitContent();
//	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::OnInitDialog") );
	/////////////////////////////////////////////////////////////////////////
	// 3. 추가 정보 초기화 
	/////////////////////////////////////////////////////////////////////////
	ids = InitSMSExInfo();
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Contents_Editor::OnInitDialog") );
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// SMS 종류 콤보박스 초기화
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::InitSMSGroup()
{
EFS_BEGIN
	
	INT ids;

	///////////////////////////////////////////////////////////////////////////////////
	// 1. SMS DM 초기화 
	///////////////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::InitSMSGroup") );
	ids = pDM->RefreshDataManager( _T("") );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::InitSMSGroup") );

	///////////////////////////////////////////////////////////////////////////////////
	// 2. 콤보박스에 입력
	///////////////////////////////////////////////////////////////////////////////////
	INT nCnt = pDM->GetRowCount();
	CString sTemp = _T("");
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cSMS_TYPE);

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("SMS명"), i, sTemp);
		sTemp.TrimLeft();sTemp.TrimRight();
		if ( !sTemp.IsEmpty() )
			pCB->AddString(sTemp);		
	}
	
	return 0;
EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// SMS 종류 선택이벤트
//////////////////////////////////////////////////////////////////////////////////////
VOID CSMS_LifeLong_Contents_Editor::OnSelchangecSMSTYPE() 
{
	INT ids;

	///////////////////////////////////////////////////////////////////////////////////
	// 1. 현재 선택된 값을 읽는다.
	///////////////////////////////////////////////////////////////////////////////////
	CString sSMSName = _T("");
	CString sErrMsg  = _T("");
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_cSMS_TYPE);
	pCB->GetLBText(pCB->GetCurSel(), sSMSName);
	if ( sSMSName.IsEmpty() )
	{
		sErrMsg.Format( _T("선택된 SMS종류가 없습니다.") );
		AfxMessageBox(sErrMsg);
		return;
	}
	
	///////////////////////////////////////////////////////////////////////////////////
	// 2. 선택된 값의 DEFAULT_MESSAGE를 화면에 뿌린다.
	///////////////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) 
	{
		sErrMsg.Format( _T("ERROR POSITION : CSMS_LifeLong_Contents_Editor::OnSelchangecSMSTYPE()") );
		AfxMessageBox(sErrMsg);
		return;
	}

	INT nCnt = pDM->GetRowCount();
	CString sTemp = _T("");

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("SMS명"), i, sTemp );
		if ( _tcscmp(sTemp, sSMSName) == 0 )
		{
			pDM->GetCellData( _T("타입"), i, m_sSMSType );
			GetDefaultMsg( &sTemp );
			CEdit* pEdit = ( CEdit* )GetDlgItem( IDC_eSMS_CONTENTS );
			pEdit->SetWindowText( sTemp );
			
			// ADD WORK 체크박스 비사용 결정
			if ( m_sSMSType == _T("LFE03") || m_sSMSType == _T("LFE04") )
			{
				GetDlgItem( IDC_chkProgram )->EnableWindow( FALSE );
				GetDlgItem( IDC_chkOpen )->EnableWindow( FALSE );
			}
			else
			{	
				GetDlgItem( IDC_chkProgram )->EnableWindow( TRUE );
				GetDlgItem( IDC_chkOpen )->EnableWindow( TRUE );
			}			

			// ADD WORK 체크박스 변화
			ids = SetChkContents( m_sSMSType );
			if (ids < 0) 
			{
				sErrMsg.Format( _T("ERROR POSITION : CSMS_LifeLong_Contents_Editor::OnSelchangecSMSTYPE()") );
				AfxMessageBox(sErrMsg);
				return;
			}

			break;
		}
	}

	SetContentByte();
	///////////////////////////////////////////////////////////////////////////////////
	// 3. 개발자 정보
	///////////////////////////////////////////////////////////////////////////////////
	pDM->GetCellData( _T("REC_KEY"), i, sTemp);
	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_TypeInfo1);
	pEdt->SetWindowText( sTemp );

	pEdt = (CEdit*)GetDlgItem(IDC_TypeInfo2);
	pEdt->SetWindowText( m_sSMSType );

	pDM->GetCellData( _T("자동발송"), i, sTemp);
	pEdt = (CEdit*)GetDlgItem(IDC_TypeInfo3);
	pEdt->SetWindowText( sTemp );

	pDM->GetCellData( _T("SMS명"), i, sTemp );	
	pEdt = (CEdit*)GetDlgItem(IDC_TypeInfo4);
	pEdt->SetWindowText( sTemp );
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// CFG 파일의 설정에 맞게 체크 박스 표시
// ** 사용안함 
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::InitContent()
{
	CString sErrMsg = _T("");
	///////////////////////////////////////////////////////////////////////////////////
	// 1. 값에 따라 체크박스 표시
	//    도서관명 
	//    수신자
	//    강자명
	//    개강일
	//    기본메세지
	///////////////////////////////////////////////////////////////////////////////////
	CStdioFile File;
	if ( !File.Open(FILE_PATH, CFile::modeRead | CFile::typeBinary) )
	{
		sErrMsg.Format( _T("%s 파일을 찾을수 없습니다."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	TCHAR cUni;
	File.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		File.SeekToBegin();
	}

	CString sTemp = _T("");

	while ( File.ReadString( sTemp ) )
	{
		if ( _tcscmp( sTemp.Left(1) , _T("#") ) == 0 )	continue;
		else										break;
	}

	File.Close();
	
	if ( sTemp.GetLength() != 5 )
	{
		sErrMsg.Format( _T("데이터형식이 잘못되어 있습니다."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	INT nChk;
	CButton* pBT;
	// 도서관명
	nChk = _ttoi( sTemp.Mid(0,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkLibName);
	pBT->SetCheck(nChk);
	// 수신자명
	nChk = _ttoi( sTemp.Mid(1,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkReciever);
	pBT->SetCheck(nChk);
	// 강좌명
	nChk = _ttoi( sTemp.Mid(2,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkProgram);
	pBT->SetCheck(nChk);
	// 개강일
	nChk = _ttoi( sTemp.Mid(3,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkOpen);
	pBT->SetCheck(nChk);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 추가 사항
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::InitSMSExInfo()
{
	INT ids;

	///////////////////////////////////////////////////////////////////////////////////
	// 1. SMS 추가사항 DM 초기화 
	///////////////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSInfo") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::InitSMSExInfo") );

	// 엠마 정보를 가져온다 
	CString sQuery = _T("");
	sQuery.Format( _T("SMS_TYPE='3'") );
	ids = pDM->RefreshDataManager( sQuery );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::InitSMSExInfo") );

	///////////////////////////////////////////////////////////////////////////////////
	// 2. 정보를 표시한다. 
	//    1번 로우 정보가 유효하다고 가정한다.
	///////////////////////////////////////////////////////////////////////////////////
	// 2.1) 회신번호
	CString sGetValue = _T("");
	pDM->GetCellData( _T("회신번호"), 0, sGetValue );
	SetDlgItemText(IDC_eRECEIVE_NUMBER, sGetValue);
	// 2.2) URL
	pDM->GetCellData( _T("URL"), 0, sGetValue );
	SetDlgItemText(IDC_eURL, sGetValue);
	// ADD WORK MSG_GB 추가
	// 평생은 엠마로 고정한다.
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COMBO1);
	pCB->AddString(_T("SMS"));
	pCB->AddString(_T("URL"));

	pDM->GetCellData( _T("MSG_GB"), 0, sGetValue );
	SetDlgItemText(IDC_eURL, sGetValue);
	if ( _tcscmp( sGetValue, _T("0") ) == 0 )
		pCB->SetCurSel(0);
	else if ( _tcscmp( sGetValue, _T("5") ) == 0 )
		pCB->SetCurSel(1);

	// 이하로 개발자 정보이다. ////////////////////////////////////////////////////////
	// 2.3) REC_KEY
	pDM->GetCellData( _T("REC_KEY"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo1, sGetValue);
	// 2.4) 잘라보내기
	pDM->GetCellData( _T("잘라보내기"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo2, sGetValue);
	// 2.5) 바인드ID
	pDM->GetCellData( _T("바인드ID"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo3, sGetValue);
	// 2.6) 바인드PWD
	pDM->GetCellData( _T("바인드PW"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo4, sGetValue);
	// 2.7) SMS_TYPE
	pDM->GetCellData( _T("타입"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo5, sGetValue);
	// 2.8) MSG_GB
	pDM->GetCellData( _T("MSG_GB"), 0, sGetValue );
	SetDlgItemText(IDC_TotalInfo6, sGetValue);

	// 2006-01-04 ADD BY PDW(Jr) =====================================================
	// 2.9) 도서관명
	pDM->GetCellData( _T("도서관명"), 0, sGetValue );
	SetDlgItemText(IDC_edtLibName, sGetValue);
	// ===============================================================================
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// CFG 파일의 설정에 맞게 체크 박스 표시
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::SetChkContents(CString sType)
{
	///////////////////////////////////////////////////////////////////////////////////
	// 1. 값에 따라 체크박스 표시
	//    도서관명 
	//    수신자
	//    강자명
	//    개강일
	///////////////////////////////////////////////////////////////////////////////////
	CString sTemp = _T("");
	GetCFGContents( &sTemp );

	INT nChk;
	CButton* pBT;
	// 도서관명
	nChk = _ttoi( sTemp.Mid(0,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkLibName);
	pBT->SetCheck(nChk);
	// 수신자명
	nChk = _ttoi( sTemp.Mid(1,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkReciever);
	pBT->SetCheck(nChk);
	// 강좌명
	nChk = _ttoi( sTemp.Mid(2,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkProgram);
	pBT->SetCheck(nChk);
	// 개강일
	nChk = _ttoi( sTemp.Mid(3,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkOpen);
	pBT->SetCheck(nChk);
	// 기본메세지
	nChk = _ttoi( sTemp.Mid(4,1) );
	pBT = (CButton*)GetDlgItem(IDC_chkDefaultMsg);
	pBT->SetCheck(nChk);

	return 0;
}

VOID CSMS_LifeLong_Contents_Editor::OnbSAVE() 
{
	INT ids;

	ids = UpdateInDB();
	if (ids < 0)
	{
		AfxMessageBox( _T("저장에 실패 했습니다.") );
		return;
	}

	AfxMessageBox( _T("저장 했습니다.") );

	SetContentByte();
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 화면 정보를 DB에 반영한다.
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::UpdateInDB()
{	
	INT ids;

	///////////////////////////////////////////////////////////////////////////////////
	// 1. SMS가 선택되어 있는지 확인한다.
	///////////////////////////////////////////////////////////////////////////////////
	CString sRecKey = _T("");
	GetDlgItemText( IDC_TypeInfo1, sRecKey );
	if ( sRecKey.IsEmpty() )
	{
		CString sErrMsg = _T("");
		sErrMsg.Format( _T("선택된 SMS 종류가 없습니다.") );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	
	ids = UpdateTypeInfo();
	if (ids < 0) return ids;

	ids = UpdateTotalInfo();
	if (ids < 0) return ids;

	ids = UpdateContentInfo();
	if (ids < 0) return ids;
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// MN_SMS_TYPE_ENV_TBL UPDATE
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::UpdateTypeInfo()
{
	INT ids;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );
	
	const INT nTypeCnt = 4;

	////////////////////////////////////////////////////////////////////////////////
	// 1. 필드 알리아스 
	////////////////////////////////////////////////////////////////////////////////
	CString sFieldAlias[nTypeCnt] = { _T("DEFAULT_MESSAGE"),  _T("TYPE"),   _T("NAME"),   _T("AUTO_SEND_YN") };
 
	////////////////////////////////////////////////////////////////////////////////
	// 2. 필드 타입
	////////////////////////////////////////////////////////////////////////////////
	CString sFieldType[nTypeCnt]  = { _T("STRING"),           _T("STRING"), _T("STRING"), _T("STRING") };

	////////////////////////////////////////////////////////////////////////////////
	// 3. 화면 데이터
	////////////////////////////////////////////////////////////////////////////////
	CString sUpdateData[nTypeCnt];
	GetDlgItemText( IDC_eSMS_CONTENTS, sUpdateData[0] );
	GetDlgItemText( IDC_TypeInfo2,     sUpdateData[1] );
	GetDlgItemText( IDC_TypeInfo4,     sUpdateData[2] );
	GetDlgItemText( IDC_TypeInfo3,	   sUpdateData[3] );

	////////////////////////////////////////////////////////////////////////////////
	// 4. REC_KEY 구하기 
	////////////////////////////////////////////////////////////////////////////////
	/*	화면상에 뿌려준 REC_KEY를 가져온다
	CString sQuery = T(_T(""));
	sQuery.Format ( _T("SELECT MIN(REC_KEY) FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE = '%s'"), sFieldType[0] );

	CString sRecKey = _T("");
	ids = pDM->GetOneValueQuery( sQuery , sRecKey );	
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );

	ids = pDM->StartFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );
	*/	
	CString sRecKey = _T("");
	GetDlgItemText( IDC_TypeInfo1, sRecKey );

	////////////////////////////////////////////////////////////////////////////////
	// 5. 프레임 생성
	////////////////////////////////////////////////////////////////////////////////
	ids = pDM->StartFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );

	pDM->InitDBFieldData();	
	for (INT i = 0;i < nTypeCnt;i++)
	{
		ids = pDM->AddDBFieldData( sFieldAlias[i]	, sFieldType[i] , sUpdateData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );
	}
			
	ids = pDM->MakeUpdateFrame( _T("MN_SMS_TYPE_ENV_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );
	
	pDM->EndFrame();

	ids = pDM->RefreshDataManager( _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("CSMS_LifeLong_Contents_Editor::UpdateTypeInfo") );

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// MN_SMS_TOTAL_ENV_TBL UPDATE
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::UpdateTotalInfo()
{
	INT ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_LifeLong_SMSInfo") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );
	
	const INT nTotalCnt = 7;

	////////////////////////////////////////////////////////////////////////////////
	// 1. 필드 알리아스
	////////////////////////////////////////////////////////////////////////////////
	CString sFieldAlias[nTotalCnt] = { _T("CALLBACK_PHONE"),  _T("URL"),       _T("SEND_DIVIDE_YN"),
		                               _T("BIND_ID"),         _T("BIND_PWD"),  _T("SMS_TYPE")      ,
									   _T("MSG_GB")													};
 
	////////////////////////////////////////////////////////////////////////////////
	// 2. 필드 타입
	////////////////////////////////////////////////////////////////////////////////
	CString sFieldType[nTotalCnt]  = { _T("STRING"),          _T("STRING"),    _T("STRING"), 
		                               _T("STRING"),          _T("STRING"),    _T("STRING"),
									   _T("STRING")										    };

	////////////////////////////////////////////////////////////////////////////////
	// 3. 화면 데이터
	////////////////////////////////////////////////////////////////////////////////
	CString sUpdateData[nTotalCnt];
	GetDlgItemText(IDC_eRECEIVE_NUMBER, sUpdateData[0]);
	GetDlgItemText(IDC_eURL,            sUpdateData[1]);
	GetDlgItemText(IDC_TotalInfo2,      sUpdateData[2]);
	GetDlgItemText(IDC_TotalInfo3,      sUpdateData[3]); 
	GetDlgItemText(IDC_TotalInfo4,      sUpdateData[4]); 
	GetDlgItemText(IDC_TotalInfo5,      sUpdateData[5]);
	
	CString sTemp = _T("");
	GetDlgItemText( IDC_COMBO1, sTemp );
	if ( _tcscmp( sTemp , _T("SMS") ) == 0 )
		sUpdateData[6] = _T("0");
	else if ( _tcscmp( sTemp , _T("URL") ) == 0 )
		sUpdateData[6] = _T("5");
	////////////////////////////////////////////////////////////////////////////////
	// 4. REC_KEY 구하기 
	////////////////////////////////////////////////////////////////////////////////
	CString sRecKey = _T("");
	GetDlgItemText(IDC_TotalInfo1, sRecKey);

	////////////////////////////////////////////////////////////////////////////////
	// 5. 프레임 생성
	////////////////////////////////////////////////////////////////////////////////
	ids = pDM->StartFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );

	pDM->InitDBFieldData();	
	for (INT i = 0;i < nTotalCnt;i++)
	{
		ids = pDM->AddDBFieldData( sFieldAlias[i]	, sFieldType[i] , sUpdateData[i] );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );
	}

	ids = pDM->MakeUpdateFrame( _T("MN_SMS_TOTAL_ENV_TBL") , _T("REC_KEY") , _T("NUMERIC") , sRecKey );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );

	ids = pDM->SendFrame();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );
	
	pDM->EndFrame();
	
	ids = pDM->RefreshDataManager( _T("") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("CSMS_LifeLong_Contents_Editor::UpdateTotalInfo") );

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// CFG파일 정보 UPDATE
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::UpdateContentInfo()
{
	//////////////////////////////////////////////////////////////////////////////////
	// 1. 현재 체크 정보를 구한다.
	//////////////////////////////////////////////////////////////////////////////////
	CString sCurStatus = _T("");
	GetCurContents(&sCurStatus);

	//////////////////////////////////////////////////////////////////////////////////
	// 2. 파일을 열어서 타입이 같다면 버프에 변환해서 넣는다.
	//////////////////////////////////////////////////////////////////////////////////
	CStdioFile File;
	if ( !File.Open(FILE_PATH, CFile::modeRead | CFile::typeBinary) )
	{
		CString sErrMsg =_T("");
		sErrMsg.Format( _T("%s 파일을 찾을수 없습니다."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	TCHAR cUni;
	File.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		File.SeekToBegin();
	}

	CString sType = _T("");
	GetDlgItemText(IDC_TypeInfo2, sType);

	CString sLine = _T("");
	CString sBuf  = _T("");

	while ( File.ReadString( sLine ) )
	{
		if ( sLine.Find( sType ) != -1 )
			sLine = sType + _T(",") + sCurStatus;
		
		sLine += _T("\n");
		sBuf += sLine;
	}
		
	File.Close();
	
	if ( !File.Open(FILE_PATH, CFile::modeWrite) )
	{
		CString sErrMsg =_T("");
		sErrMsg.Format( _T("%s 파일을 찾을수 없습니다."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	File.WriteString(sBuf);
	File.Close();

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 화면에 설정된 CHK의 상태를 반환한다.
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::GetCurContents(CString *sResult)
{
	//////////////////////////////////////////////////////////////////////////////////
	// 1. 체크박스의 현재상태를 구한다.
	//////////////////////////////////////////////////////////////////////////////////
	INT nChk1 = 0, nChk2 = 0, nChk3 = 0, nChk4 = 0, nChk5 = 0;
	CButton* pBT;
	pBT = (CButton*)GetDlgItem(IDC_chkLibName);
	nChk1 = pBT->GetCheck();
	
	pBT = (CButton*)GetDlgItem(IDC_chkReciever);
	nChk2 = pBT->GetCheck();

	pBT = (CButton*)GetDlgItem(IDC_chkProgram);
	nChk3 = pBT->GetCheck();

	pBT = (CButton*)GetDlgItem(IDC_chkOpen);
	nChk4 = pBT->GetCheck();

	pBT = (CButton*)GetDlgItem(IDC_chkDefaultMsg);
	nChk5 = pBT->GetCheck();

	//////////////////////////////////////////////////////////////////////////////////
	// 2. 스트링으로 반환한다.
	//////////////////////////////////////////////////////////////////////////////////
	sResult->Format(_T("%d%d%d%d%d"), nChk1, nChk2, nChk3, nChk4, nChk5);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 문자 바이트 표현
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::SetContentByte()
{
/*	CString sTemp = _T("");
	GetDlgItemText( IDC_eSMS_CONTENTS, sTemp);
	INT nByte = sTemp.GetLength();
	CString sTitle = _T("");
	sTitle.Format( _T("문자 내용 ( %d Byte )"), nByte );
	SetDlgItemText(	IDC_sSMS_CONTENTS , sTitle );
*/
	INT nCurPayByte = 0;
	INT nCurMAXByte = 80;

/*	  1. URL 크기를 구한다.( MAX 값 설정 )
	  2. 기본 형식 바이트를 더한다.
	  3. 문자 구성을 구한다.	  
      4. 문자 BYTE를 표현한다. 
*/
	
	// 1. URL 크기를 구한다.( MAX 값 설정 )
	CString sSMSType;
	GetDlgItemText( IDC_COMBO1 ,sSMSType );
	sSMSType.TrimLeft();sSMSType.TrimRight();
	if ( sSMSType.Replace( _T("URL") , _T("") )  > 0 )
	{
		CEdit* pEdtURL = ( CEdit* )GetDlgItem( IDC_eURL );
		if ( !pEdtURL ) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CSMS_LifeLong_Contents_Editor::SetContentByte()") );	
			
		// 공백을 위해 추가로 1바이트가 더 빠진다.
		if ( pEdtURL->GetWindowTextLength() > 0 )
		{
			nCurMAXByte -= ( pEdtURL->GetWindowTextLength() + 1 );	
		}			 
	}
	
	// 2. 기본 형식 바이트를 더한다.
	//    기본문자가 변경되어야한다.
/*	if ( GetDlgItem( IDC_cSMS_TYPE )->GetWindowTextLength() > 0 )
	{
		nCurPayByte += GetDlgItem( IDC_cSMS_TYPE )->GetWindowTextLength() + 2;
	}
*/
	// 3. 문자 구성을 구한다.
	// able 되어 있는 구성 && 체크되어 있는 값만을 계산한다.
	const INT nEleCnt = 5;
	CButton* pBtn     = NULL;
	                     // 도서관명       이용자명         강좌명          날짜정보     기본메세지 
	INT nID[ nEleCnt ] = { IDC_chkLibName, IDC_chkReciever, IDC_chkProgram, IDC_chkOpen, IDC_chkDefaultMsg };
	for ( INT i = 0;i < nEleCnt;i++ )
	{		
		pBtn = (CButton*)GetDlgItem( nID[ i ] );

		if ( pBtn->IsWindowEnabled() && pBtn->GetCheck() )
		{
			//    0 : 도서관명, 1 : 수신자명, 2 : 강좌명, 3 : 개강일, 4 :기본메세지
			switch( i )
			{
				case 0:		// 도서관명
					{						
						// 형식 2바이트 추가 
						nCurPayByte += GetDlgItem( IDC_edtLibName )->GetWindowTextLength() + 2;
					}
					break;
				case 1:		// 수신자명
					{
						// 가변 길이 적용되지 않음
					}
					break;
				case 2:		// 강좌명
					{
						// 가변 길이 적용되지 않음
					}
					break;
				case 3:		// 날짜정보
					{
						nCurPayByte += 18; // 형식이 고정되어 있다. ',개강일:2005/12/12'
					}
					break;
				case 4:		// 기본메세지 
					{
						nCurPayByte += GetDlgItem(IDC_eSMS_CONTENTS)->GetWindowTextLength();
					}
					break;
			}
		}
	}

	// 4. 문자 바이트를 표현한다.
	CString sBYTE;
	sBYTE.Format( _T("%d / %d"), nCurPayByte, nCurMAXByte);
	SetDlgItemText(	IDC_sSMS_CONTENTS , sBYTE );

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 기본 메세지를 구한다.
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::GetDefaultMsg(CString *sGetMsg)
{
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Contents_Editor::GetDefaultMsg") );
	
	INT nCnt = pDM->GetRowCount();
	if (nCnt < 1) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Contents_Editor::GetDefaultMsg") );

	CString sTemp = _T("");
	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("타입"), i, sTemp );
		if ( _tcscmp(sTemp, m_sSMSType) == 0 )
		{
			pDM->GetCellData( _T("기본메세지"), i, *sGetMsg );
			break;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// CFG의 값을 리턴한다.
//////////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Contents_Editor::GetCFGContents(CString* psResult)
{
	CString sErrMsg = _T("");
	CStdioFile File;
	if ( !File.Open(FILE_PATH, CFile::modeRead | CFile::typeBinary) )
	{
		sErrMsg.Format( _T("%s 파일을 찾을수 없습니다."), FILE_PATH );
		AfxMessageBox( sErrMsg );
		return -1;
	}
	TCHAR cUni;
	File.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		File.SeekToBegin();
	}

	CString sTemp = _T("");

	while ( File.ReadString( sTemp ) )
	{
		if ( _tcscmp( sTemp.Mid(0,1) , _T("#") ) == 0 )	continue;
		if ( sTemp.Replace( m_sSMSType, _T("") ) > 0 )		break;
	}

	File.Close();

	sTemp.Replace( _T(","), _T("") );

	if ( sTemp.GetLength() != 5 )
	{
		sErrMsg.Format( _T("데이터형식이 잘못되어 있습니다.") );
		AfxMessageBox( sErrMsg );
		return -1;
	}

	*psResult = sTemp;

	return 0;
}

VOID CSMS_LifeLong_Contents_Editor::OnCancel() 
{
	// TODO: Add extra cleanup here
//	this->DestroyWindow();	
	CDialog::OnCancel();
}

VOID CSMS_LifeLong_Contents_Editor::OnChangeeSMSCONTENTS() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkLibName() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkReciever() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkProgram() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkOpen() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnchkDefaultMsg() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnSelchangeCombo1() 
{
	SetContentByte();
}

VOID CSMS_LifeLong_Contents_Editor::OnChangeeURL() 
{
	SetContentByte();
}
