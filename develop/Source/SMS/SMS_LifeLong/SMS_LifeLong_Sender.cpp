// SMS_LifeLong_Sender.cpp : implementation file
//

#include "stdafx.h"
#include "SMS_LifeLong_Sender.h"
#include "..\CXroshotManager\EmmaSMSManager.h"
#include "SMS_LifeLong_Contents_Editor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Sender dialog


CSMS_LifeLong_Sender::CSMS_LifeLong_Sender(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMS_LifeLong_Sender)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCSMS_LifeLong = (CSMS_LifeLong*)pParent;
	m_pDMSend = NULL;
	m_receiverName.RemoveAll();
	m_receiverNum.RemoveAll();
	m_msg.RemoveAll();
	m_sTitle = _T("");
	m_sDefaultMsg = _T("");
	m_sMESSAGE = _T("");
	m_sLibName = _T("");

	// 2006-01-09 ADD BY PDW(Jr) ==============================================
	// 평생학습관 SMS 로그 저장 관련 추가
	m_lstEmmaKey.RemoveAll();;
	m_lstDCnt.RemoveAll();;
	// ========================================================================
}


VOID CSMS_LifeLong_Sender::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMS_LifeLong_Sender)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMS_LifeLong_Sender, CDialog)
	//{{AFX_MSG_MAP(CSMS_LifeLong_Sender)
	ON_WM_DESTROY()
	ON_MESSAGE( SMS_SEND_STATUS, ShowSMSSendStatus )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMS_LifeLong_Sender message handlers

BOOL CSMS_LifeLong_Sender::Create(CWnd* pParentWnd)
{
EFS_BEGIN
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::Create(IDD, pParentWnd);
EFS_END
	return FALSE;
}

BOOL CSMS_LifeLong_Sender::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT ids;
	//////////////////////////////////////////////////////////////////////////////
	// 1. SM 초기화
	//////////////////////////////////////////////////////////////////////////////
	ids = InitESL_Mgr(_T("SMS_DLG_LifeLong_Sender"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::OnInitDialog()") );
	
	//////////////////////////////////////////////////////////////////////////////
	// 2. 프로그레스 초기화
	//////////////////////////////////////////////////////////////////////////////
	ids = InitProgress();
	if( ids < 0 ) return FALSE;

	//////////////////////////////////////////////////////////////////////////////
	// 3. SMS 종류 
	//////////////////////////////////////////////////////////////////////////////
	ids = InitSMSEdit();
	if( ids < 0 ) return FALSE;

	// 2006-01-05 ADD BY PDW(Jr) =================================================
	// 도서관명 정보 설정
	if ( SetLibName() < 0 )
	{
		return FALSE;
	}
	// ===========================================================================

	//////////////////////////////////////////////////////////////////////////////
	// 4. 미리보기 초기화
	//    미리보기는 SMS의 종류가 선택되어 있어야한다.
	//////////////////////////////////////////////////////////////////////////////
	if ( !m_pCSMS_LifeLong->m_sSMSType.IsEmpty() )
	{
		ids = InitPreview();
		if ( ids < 0 ) return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 발송용 DM을 설정한다.
///////////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::SetInfo(CESL_DataMgr *pDM)
{
EFS_BEGIN

	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::SetInfo") );
	m_pDMSend = pDM;

	return 0;
EFS_END
	return -1;
}

VOID CSMS_LifeLong_Sender::OnOK() 
{
	// TODO: Add extra validation here
	SendSMS();
//	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////////////
// [함수설명] 
// 발송
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::SendSMS()
{
EFS_BEGIN
	
	INT ids;
	//////////////////////////////////////////////////////////////////////////////
	// 1. 아레오에 대입할 리스트를 만든다.
	//    받는사람이름
	//    받는사람번호
	//    타이틀
	//    메세지
	//////////////////////////////////////////////////////////////////////////////
	ids = MakeSMSData();
	if (ids < 0) return ids;

	//////////////////////////////////////////////////////////////////////////////
	// 2. 아레오 클래스로 넣는다.
	//////////////////////////////////////////////////////////////////////////////
	CEmmaSMSManager Emma;

	// 2006-01-09 ADD BY PDW(Jr) =================================================
	// 평생학습관 플래그 추가 
	Emma.m_bLifeLong = TRUE;
	// ===========================================================================

	ids = Emma.SendSMSMsg(m_receiverName, m_receiverNum, _T(""), m_msg, _T(""), this);
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::SendSMS") );

	//////////////////////////////////////////////////////////////////////////////
	// 3. 마무리
	//////////////////////////////////////////////////////////////////////////////
	return 0;
EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 아레오 발송용 리스트를 만든다.
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::MakeSMSData()
{
EFS_BEGIN
	INT ids;
	INT nCnt;
	
	//////////////////////////////////////////////////////////////////////////////
	// 1. DM에서 기본데이터를 만든다.
	//////////////////////////////////////////////////////////////////////////////
	
	// 2006-01-05 ADD BY PDW(Jr) =================================================
	// 핸드폰 번호가 NULL인 회원에 대한 처리가 추가됨
	INT nNullHPCnt = 0;

	nCnt = m_pDMSend->GetRowCount();
	if (nCnt < 1) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::MakeSMSData") );

	CString HP      = _T("");
	CString name    = _T("");
	CString program = _T("");
	CString msg     = _T("");

	for (INT i = 0;i < nCnt;i++)
	{		
		m_pDMSend->GetCellData(_T("핸드폰"), i, HP);
		
		if ( HP.IsEmpty() )	
		{
			nNullHPCnt++;
			continue;
		}

		m_pDMSend->GetCellData(_T("회원명"), i, name);
		if (m_pCSMS_LifeLong->m_nDisplayMode == 0)
			m_pDMSend->GetCellData(_T("강좌명"), i, program);

		//////////////////////////////////////////////////////////////////////////
		// 2. 메세지를 만든다.
		//////////////////////////////////////////////////////////////////////////
		ids = MakeMSG(&msg, name, program);
		if (ids < 0) continue;
				
		//////////////////////////////////////////////////////////////////////////
		// 3. 메세지생성까지 성공했다면
		//    리스트에 추가된다.
		//////////////////////////////////////////////////////////////////////////
		m_receiverNum.AddTail(HP);
		m_receiverName.AddTail(name);
		m_msg.AddTail(msg);
	}

	if ( nNullHPCnt )
	{
		CString sResultMsg;
		sResultMsg.Format( _T("핸드폰 번호가 없는 회원정보가 있습니다. ( %d : 건수 )"), nNullHPCnt );
		AfxMessageBox( sResultMsg );
	}

	// ===========================================================================
	return 0;
EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 메세지를 만든다.
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::MakeMSG(CString* psGetMsg, CString sName, CString sProgram)
{
EFS_BEGIN

	INT ids;

	//////////////////////////////////////////////////////////////////////////
	// 1. MN_SMS_TYPE_ENV_TBL 의 DEFAULT_MAESSAGE를 가져온다
	//////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::MakeMSG") );

	CString sQuery  = _T("");
    CString sGetMsg = _T("");

/*	sQuery.Format( _T("SELECT DEFAULT_MESSAGE FROM MN_SMS_TYPE_ENV_TBL WHERE TYPE = '%s'") , 
				   m_pCSMS_LifeLong->m_sSMSType );

	ids = pDM->GetOneValueQuery( sQuery, sGetMsg );
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Sender::MakeMSG") );
*/
	// 메세지 방식 변경
	sGetMsg = m_sMESSAGE;

	//////////////////////////////////////////////////////////////////////////
	// 2. 알리아스를 데이터로 변환한다
	//    [ALIAS]
	//    %수강자% -> 수신자명
	//    %강좌명% -> 강좌명
	//    %개강일% -> 강좌개강일 
	//  ** 소스에서 알리아스 처리를 해준다. 
	//     평생교육원 메일자체가 특수 메일 경우이기 때문에 레퍼런스를 가지지 않는다.
	//////////////////////////////////////////////////////////////////////////
	sGetMsg.Replace(_T("%수강자%"), sName);
	sGetMsg.Replace(_T("%강좌명%"), sProgram);

	// 2.1) 프로그램명이 있다면 개강일을 구할수 있다.
	CString sOpenDay = _T("");
	if ( !sProgram.IsEmpty() )
	{	
		sQuery.Format(_T("SELECT GAEGANGDAY FROM PG_PROGRAM@EMT WHERE PG_NAME = '%s'"), sProgram);
		ids = pDM->GetOneValueQuery( sQuery, sOpenDay );
		if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Sender::MakeMSG") );			
	}

	sGetMsg.Replace(_T("%개강일%"), sOpenDay);
	*psGetMsg = sGetMsg;

	return 0;
EFS_END
	return -1;
}

//////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 프로그레스바를 진행 시킨다.
//////////////////////////////////////////////////////////////////////////////
VOID CSMS_LifeLong_Sender::ShowSMSSendStatus(WPARAM wParam, LPARAM lParam)
{
	CString statusMsg = ( ( CString * )wParam )->GetBuffer( 0 );

	// cnt = 전체 갯수, ids = 현재 위치, nCnt = 성공갯수, error = 플래그
	INT cnt, idx, dCnt, error;
	_stscanf( statusMsg, _T("%d/%d/%d/%d"), &cnt, &idx, &dCnt, &error );
	CProgressCtrl *pProgress = ( CProgressCtrl * )GetDlgItem( IDC_pProgress );
		
	// 2006-01-09 UPDATE BY PDW(Jr) ==========================================
	// 로그 관련부분 추가 됨
	switch( error )
	{
		case 1 :
			pProgress->SetPos( m_pDMSend->GetRowCount() );
			InsertSMSLog();
			AfxMessageBox( _T(" SMS 전송을 완료하였습니다. ") );
			break;
		case 2 :
			InsertSMSLog();
			AfxMessageBox( _T(" SMS 전송이 중단되었습니다. ") );
			break;
		default :
			pProgress->SetPos( idx );
			AddSMSLog( dCnt );
			break;
	}
	// ======================================================================
}

//////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 프로그레스바를 초기화 한다.
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::InitProgress()
{
	CProgressCtrl *pProgress = ( CProgressCtrl * )GetDlgItem( IDC_pProgress );
	if (pProgress == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1, _T("CSMS_LifeLong_Sender::InitProgress") );
	
	INT nCnt = m_pDMSend->GetRowCount();
	pProgress->SetRange( 0, nCnt );
	pProgress->SetPos( 0 );
	return 0;
}

VOID CSMS_LifeLong_Sender::OnDestroy() 
{
	CDialog::OnDestroy();
		
}

//////////////////////////////////////////////////////////////////////////////
// [함수설명]
// SMS 종류 표시
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::InitSMSEdit()
{
	//////////////////////////////////////////////////////////////////////////
	// 1. DM에서 해당 이름을 표시 해준다.
	//////////////////////////////////////////////////////////////////////////
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Sender::OnInitDialog()") );
	pDM->RefreshDataManager( _T("") );

	INT nCnt = pDM->GetRowCount();
	CString sValue = _T("");

	for (INT i = 0;i < nCnt;i++)
	{
		pDM->GetCellData( _T("타입"), i, sValue );

		if ( _tcscmp( m_pCSMS_LifeLong->m_sSMSType, sValue ) == 0 )
		{
			pDM->GetCellData( _T("SMS명"), i, sValue );
			SetDlgItemText(IDC_eMAILNAME, sValue );
			// ADD WORK 기본메세지 저장
			pDM->GetCellData( _T("기본메세지"), i, m_sDefaultMsg );
			break;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
// [함수설명]
// 미리보기 화면 초기화
//////////////////////////////////////////////////////////////////////////////
INT CSMS_LifeLong_Sender::InitPreview()
{
	INT ids;

	CSMS_LifeLong_Contents_Editor Contents(this);
	Contents.m_sSMSType = m_pCSMS_LifeLong->m_sSMSType;
	
	//////////////////////////////////////////////////////////////////////////
	// 1. 설정파일의 값을 불러온다
	//////////////////////////////////////////////////////////////////////////
	CString sContents = _T("");
	Contents.GetCFGContents( &sContents );
	
	//   도서관 이름,      수신자명,        강좌명,           개강일,        기본메세지
	BOOL bLibName = FALSE, bSender = FALSE, bProgram = FALSE, bOpen = FALSE, bDefaultMsg = FALSE;
	
	bLibName    = _ttoi( sContents.Mid(0,1) );
	bSender     = _ttoi( sContents.Mid(1,1) );
	bProgram    = _ttoi( sContents.Mid(2,1) );
	bOpen       = _ttoi( sContents.Mid(3,1) );
	bDefaultMsg = _ttoi( sContents.Mid(4,1) );

	//////////////////////////////////////////////////////////////////////////
	// 2. 설정값에 따른 메세지를 정의한다.
	//////////////////////////////////////////////////////////////////////////
	CString sMsg = _T("");
	
	// 2005-12-22 ADD BY PDW(Jr) =============================================
	// URL이라면 URL도 보여준다.
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSGroup") );
	if (pDM == NULL) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::InitPreview()") );

	CString sData;
	ids = pDM->GetOneValueQuery( _T("SELECT MIN(MSG_GB) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE = '3' AND MANAGE_CODE=UDF_MANAGE_CODE"), sData );
	if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Sender::InitPreview()") );
	if ( sData == _T("5") )
	{
		ids = pDM->GetOneValueQuery( _T("SELECT MIN(URL) FROM MN_SMS_TOTAL_ENV_TBL WHERE SMS_TYPE = '3' AND MANAGE_CODE=UDF_MANAGE_CODE"), sData );
		if (ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CSMS_LifeLong_Sender::InitPreview()") );

		if ( !sData.IsEmpty() )
			sMsg += sData + _T(" ");
	}
	// =======================================================================	

	if ( bLibName && m_sLibName.GetLength() > 0 )
	{		
		sMsg += _T("[") + m_sLibName + _T("]");
	}

	if ( bSender )
	{
		sMsg += _T("%수강자% ");
	}

	if ( bDefaultMsg )
	{	
		CString sDefault = _T("");
		Contents.GetDefaultMsg( &sDefault );
		sMsg += sDefault;
	}

	// 강좌 관련 정보는 필수 조건이 있다.
	// 1. PM에 강좌 정보가 있어야한다. 
	// 2. SMS 종류가 '개강안내', '수강접수확인'일 경우만 가능
	if ( bProgram && m_pCSMS_LifeLong->m_nDisplayMode == 0 && 
			( m_pCSMS_LifeLong->m_sSMSType == _T("LFE01") || m_pCSMS_LifeLong->m_sSMSType == _T("LFE02") ) )
		sMsg += _T(" %강좌명% ");
	if ( bOpen && m_pCSMS_LifeLong->m_nDisplayMode == 0 &&
			( m_pCSMS_LifeLong->m_sSMSType == _T("LFE01") || m_pCSMS_LifeLong->m_sSMSType == _T("LFE02") ) )
		sMsg += _T(" 개강일:%개강일%");

	//////////////////////////////////////////////////////////////////////////
	// 3. 미리보기 화면에 출력한다.
	//    알리아스를 미리보기 출력으로 변경한다.
	//////////////////////////////////////////////////////////////////////////
	m_sMESSAGE = sMsg;
	sMsg.Replace( _T("%수강자%"), _T("'수강자'") );
	sMsg.Replace( _T("%강좌명%"), _T("'강좌명'") );	
	sMsg.Replace( _T("%개강일%"), _T("'개강일'") );		
	
	INT nByte = sMsg.GetLength();
	CString sTitle = _T("");
	sTitle.Format( _T("미리보기 ( %d Byte )"), nByte );

	// 2005-12-12 UPD BY PDW(Jr) ==================================================
	// 문자길이를 16바이트 단위로 자른다.	
	INT nPos = 0;
	INT nLength = sMsg.GetLength();
	
	while ( TRUE )
	{
		// 일반적으로 8글자는 들어간다. KTF기준
		nPos += 16;
		if ( nPos > nLength ) break;
		
		if ( !IsCanCut( sMsg, nPos - 1 ) )	nPos--;	
		
	    sMsg = sMsg.Mid( 0, nPos ) + _T("\n     ") + sMsg.Mid( nPos );
		nPos += 6;
		nLength += 6;
	}

	sMsg = _T("     ") + sMsg;
	//SetDlgItemText( IDC_reSMS, sMsg );
	CRichEditCtrl* edtPreView = (CRichEditCtrl*)GetDlgItem( IDC_reSMS );
	edtPreView->SetWindowText( sMsg );
	edtPreView->SetReadOnly();
	// CRichEditCtrl
	// ============================================================================

	SetDlgItemText( IDC_stPreView, sTitle );
	

	return 0;
}



VOID CSMS_LifeLong_Sender::OnCancel() 
{
	// TODO: Add extra cleanup here
//	this->DestroyWindow();	
	CDialog::OnCancel();
}

BOOL CSMS_LifeLong_Sender::IsCanCut(CString str, INT pos)
{
	INT i, nLastPos = -1;
	//글자수 초과면 FALSE
	if( str.GetLength() <= pos )
		return TRUE;
	
	TCHAR ch = str.GetAt( pos );
	
	//마지막 글자 || 한글이 아니면 TRUE
	if( pos + 1 == str.GetLength() || ( ch & ( 0x80 ) ) != 128 )
		return true;
	
	//첫글자가 1byte문자면 TRUE 아니면 FALSE
	if( !pos )
		if( ( ch & ( 0x80 ) ) != 128 )
			return true;
		else
			return false;

	for( i = 0; i < pos; i++ )
	{
		ch = str.GetAt( i );
		
		//자르는 부분에서 가장 가까운(앞쪽으로)영문의 위치
		if( ( ch & ( 0x80 ) ) != 128 )
			nLastPos = i;
	}
	
	if( nLastPos == -1 && pos % 2 == 1 )
		return true;
	
	if( nLastPos == -1 && pos % 2 == 0 )
		return false;
	
	//짝수개 차이나면 TRUE
	bool result = !( ( pos - nLastPos ) % 2 );
	return result;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-05 ~ 2006-01-05
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   함수설명   ] 
	  도서관명을 설정한다.

   [ RETURN VALUE ]
      0    : 성공 
      음수 : 실패 
--------------------------------------------------------------------------------------------*/
INT CSMS_LifeLong_Sender::SetLibName()
{
	INT ids;
	CESL_DataMgr* pDM = FindDM( _T("DM_SMS_LifeLong_SMSInfo") );
	if ( !pDM ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CSMS_LifeLong_Sender::SetLibName()") );

	// 엠마 정보를 가져온다 
	CString sQuery = _T("");
	sQuery.Format( _T("SMS_TYPE='3'") );
	ids = pDM->RefreshDataManager( sQuery );
	if(ids < 0) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CSMS_LifeLong_Sender::SetLibName()") );

	// 도서관명
	pDM->GetCellData( _T("도서관명"), 0, m_sLibName );	
	return 0;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-09 ~ 2006-01-09
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   함수설명   ] 
	  로그와 관련된 정보설정( 현재 잘라보낸 횟수만 설정 )

   [ RETURN VALUE ]
      0    : 성공 
      음수 : 실패 
--------------------------------------------------------------------------------------------*/
INT CSMS_LifeLong_Sender::AddSMSLog(INT nDCnt)
{
	CString strTemp;
	strTemp.Format( _T("%d"), nDCnt );
	m_lstDCnt.AddTail( strTemp );
	return 0;
}

/*--------------------------------------------------------------------------------------------
   @Work Day : 2006-01-09 ~ 2006-01-09
   @Author   : Park, Daewoo(Jr)   
   @WorkType : New
 	 
   [   함수설명   ] 
	  로그테이블에 기록한다.

   [ RETURN VALUE ]
      0    : 성공 
      음수 : 실패 
--------------------------------------------------------------------------------------------*/
INT CSMS_LifeLong_Sender::InsertSMSLog()
{
	INT ids = 0;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMSSendDlg") );
	
	pDM->StartFrame();
	pDM->InitDBFieldData();

	// 2006-01-09 UPDATE BY PDW(Jr) ======================================================
	// 엠마 이력관리를 위해 필드 추가 됨 ( EMMA_KEY )
	const INT cnt = 14;
	CString fieldName[ cnt ] = {
		_T("REC_KEY"), 
		_T("USER_KEY"), 
		_T("NAME"), 
		_T("SEND_PHONE_NUMBER"), 
		_T("SEND_MESSAGE"), 
		_T("SMS_TYPE"), 
		_T("SEND_DIVIDE_CNT"), 
		_T("SEND_TIME"), 
		_T("SEND_WORKER"), 
		_T("SEND_SUCCESS_YN"), 
		_T("SEND_FAIL_REASON"), 
		_T("FIRST_WORK"), 
		_T("LAST_WORK"),
		_T("EMMA_KEY") };
	CString fieldType[ cnt ] = {
		_T("NUMERIC"), 
		_T("NUMERIC"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("NUMERIC"), 
		_T("NUMERIC"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"), 
		_T("STRING"),
		_T("NUMERIC") };
	
	CString data[ cnt ];	
	CTime time = pDM->GetDBTime();
	data[  0 ] = _T("ESL_SMS_SEQ.NEXTVAL");
	data[  5 ] = m_pCSMS_LifeLong->m_sSMSType;
	// 2006-01-11 UPDATE BY PDW(Jr) ===========================================================
	// 전송시간 SYSDATE로 변경
	/*
	data[  7 ].Format( _T("%d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	*/
	data[  7 ] = _T("SYSDATE");
	// ========================================================================================
	data[  8 ] = GetUserID();
	data[  9 ] = _T("Y");
	data[ 11 ] = GetWorkLogMsg( _T("SMS 발송"), __WFILE__, __LINE__ );
	
	INT rowCnt = m_receiverName.GetCount();

	for( INT i = 0; i < rowCnt; i++ )
	{
		data[ 1 ]  = _T("");														// 수신인 KEY
		data[ 2 ]  = m_receiverName.GetAt( m_receiverName.FindIndex( i ) );	    // 수신인 명
		data[ 3 ]  = m_receiverNum.GetAt( m_receiverNum.FindIndex( i ) );	    // 수신인 HP
		data[ 4 ]  = m_msg.GetAt( m_msg.FindIndex( i ) );	                    // 발송 메세지 
		data[ 6 ]  = m_lstDCnt.GetAt( m_lstDCnt.FindIndex( i ) );				// 발송 횟수
		data[ 13 ] = m_lstEmmaKey.GetAt( m_lstEmmaKey.FindIndex( i ) );			// EMMA KEY

		for( INT j = 0; j < cnt; j++ )
		{
			if( !data[ j ].IsEmpty() )
				pDM->AddDBFieldData( fieldName[ j ], fieldType[ j ], data[ j ], true );
		}

		pDM->MakeInsertFrame( _T("MN_SMS_LOG_TBL") );
	}

	if ( rowCnt > 0 ) 
	{
		pDM->SendFrame();
	}

	pDM->EndFrame();	
	
	m_lstDCnt.RemoveAll();
	m_lstEmmaKey.RemoveAll();

	return 0;
}
