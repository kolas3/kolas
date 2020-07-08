// MNG_SERVICE_1110.cpp : implementation file
//

#include "stdafx.h"
#include "MNG_SERVICE_1110.h"
#include "..\\..\\..\\SMS\\CO_SMS\\CfgFileApi.h"

#include "../../../SMS/CO_SMS/SMS_MakeMsg.h"
#include "../../../SMS/CO_SMS/SMS_Env_Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMNG_SERVICE_1110 dialog


CMNG_SERVICE_1110::CMNG_SERVICE_1110(CESL_Mgr* pParent /*=NULL*/)
	: CMNG_SMS_SERVICE_MIDDLE_DIALOG(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMNG_SERVICE_1110)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMNG_SERVICE_1110::~CMNG_SERVICE_1110()
{	
	
}


VOID CMNG_SERVICE_1110::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMNG_SERVICE_1110)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMNG_SERVICE_1110, CDialog)
	//{{AFX_MSG_MAP(CMNG_SERVICE_1110)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_chkCOM2, OnchkCOM2)
	ON_BN_CLICKED(IDC_chkCOM1, OnchkCOM1)
	ON_BN_CLICKED(IDC_chkCOM3, OnchkCOM3)
	ON_BN_CLICKED(IDC_chkCOM4, OnchkCOM4)
	ON_BN_CLICKED(IDC_chkCOM5, OnchkCOM5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMNG_SERVICE_1110 message handlers

BOOL CMNG_SERVICE_1110::OnInitDialog() 
{
	CDialog::OnInitDialog();

	INT ids = InitESL_Mgr( _T("MNG_SMS_SERVICE_1110") );
	if( 0 > ids )
	{
		AfxMessageBox( _T("InitESL_Mgr") );
	}

	EnableThemeDialogTexture(GetSafeHwnd());
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CMNG_SERVICE_1110::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

VOID CMNG_SERVICE_1110::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if( NULL == GetSafeHwnd() ) 
	{
		return;
	}

	CRect WndPos;
	CWnd* pWnd = GetDlgItem(IDC_MAINGRID);

	if( NULL == pWnd->GetSafeHwnd() )
	{
		return;
	}

	pWnd->GetWindowRect(WndPos);

//	ClientToScreen( WndPos );
	ScreenToClient( WndPos );

	pWnd->MoveWindow( WndPos.left , WndPos.top , WndPos.Width() , cy - WndPos.top - 10 );
}

INT CMNG_SERVICE_1110::ReView()
{
EFS_BEGIN

	// 1. 문자 종류별 편집 내용 화면에 보여주기
	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_SMS_SERVICE_1110") , _T("문자종류별내용") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ReView") );
	}

	pGrid->Display();
	pGrid->m_bGridEdit = TRUE;
	
	// 2. 구성 정보 화면에 표시해주기

	CESL_DataMgr* pDM = GetTotalEnvDM();
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("ReView") );
	}

	INT		nRowCnt = pDM->GetRowCount();
	if( 0 < nRowCnt )
	{
		CString sComponent;
		pDM->GetCellData( _T("문자구성정보") , 0 ,  sComponent );
		ReShowComponentInfo(sComponent);
	}

	// 2009.05.25 ADD BY KSJ : 서명 크기 화면에 표시
	CString		strTitleSize;
	pDM->GetCellData( _T("TITLE_SIZE") , 0 ,  strTitleSize );
	GetDlgItem( IDC_edtBookTitleByte )->SetWindowText( strTitleSize );

	AfxGetApp()->DoWaitCursor(1);
	
	return 0;

EFS_END
return -1;
}


INT CMNG_SERVICE_1110::ReShowComponentInfo( CString sComponent )
{
EFS_BEGIN

	// 구성정보 
	//	5자리로 구성 0이면 문자 생성시 누락 , 1이면 문자 생성시 구성
	//	예 : '00000'
	//	1자리 : 이용자명
	//	2자리 : 기본메시지
	//	3자리 : 반납예정일/연체일/예약만기일
	//	4자리 : 서명
	//	5자리 : 도서관명

	const INT nCnt = 5;
	INT nIDC[nCnt] =
	{
		IDC_chkCOM1 , IDC_chkCOM3 , IDC_chkCOM4 , IDC_chkCOM5 , IDC_chkCOM2
	};

	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		((CButton*)GetDlgItem(nIDC[i]))->SetCheck(FALSE);

		if( i > sComponent.GetLength()-1 ) 
		{
			continue;
		}

		if( '1' == sComponent.GetAt(i) )
		{
			((CButton*)GetDlgItem(nIDC[i]))->SetCheck(TRUE);

			// 2009.05.08 ADD BY KSJ : 서명 Check 되면, 서명 크기 에디트 박스 READONLY FALSE로 
			if( 3 == i )
			{
				CEdit*	pEdtTitleSize = (CEdit*) GetDlgItem ( IDC_edtBookTitleByte );
				DWORD dwStyle = pEdtTitleSize->GetStyle();
				dwStyle &= ES_READONLY;
				if( ES_READONLY == (dwStyle & ES_READONLY) )
				{
					pEdtTitleSize->SetReadOnly( FALSE );
				}
			}
		}
	}

	return 0;

EFS_END
return -1;
}

// TempDM에 저장
INT CMNG_SERVICE_1110::Save( CESL_DataMgr* pTypeEnvDM , CESL_DataMgr* pTotalEnvDM )
{
EFS_BEGIN

	// 구성정보
	CString		sComponent;
	const INT	nCnt = 5;
	INT nIDC[nCnt] =
	{
		IDC_chkCOM1 , IDC_chkCOM3 , IDC_chkCOM4 , IDC_chkCOM5 , IDC_chkCOM2
	};
	
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		if( 1 == ((CButton*)GetDlgItem(nIDC[i]))->GetCheck() )
		{
			sComponent += _T("1");
		}
		else
		{
			sComponent += _T("0");
		}
	}

	pTotalEnvDM->SetCellData( _T("문자구성정보") ,  sComponent , 0  );

	// 2009.04.01 ADD BY KSJ : 서명 크기 저장
	CString		strTitleSize;
	CEdit*	pedtBookTitleByte	=	(CEdit*)GetDlgItem( IDC_edtBookTitleByte );
	pedtBookTitleByte->GetWindowText( strTitleSize );
	INT		nTitleSize	=	_ttoi( strTitleSize );
	if( 8 > nTitleSize || 30 < nTitleSize )
	{/// 서명 크기는 8~30사이만 저장 할 수 있다.
		AfxMessageBox(_T("서명 크기는 8~30 사이의 숫자만 입력하여 주십시요."), MB_ICONWARNING);
		pedtBookTitleByte->SetSel( 0 , -1 );
		return 1;
	}

	pTotalEnvDM->SetCellData( _T("TITLE_SIZE") , strTitleSize , 0  );

	// 문자 종류별 구성정보
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_SMS_SERVICE_1110") , _T("문자종류별내용") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("Save") );
	}

	/// 종류 , 기본문자
	INT			nNameLength , nMessageLength;
	INT			nRowCnt	=	pTypeEnvDM->GetRowCount();
	CString		sName , sMessage;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		pGrid->GetAt( i , 0 , sName );
		pGrid->GetAt( i , 1 , sMessage );

		// 자리수 검사
		nNameLength	=	sName.GetLength();
		if( 20 < nNameLength )
		{
			CString sEMsg;
			sEMsg.Format(_T("문자 종류(%s)가 20byte(한글10자,영문20자)를 초과할수 없습니다.") , sName);
			AfxMessageBox(sEMsg);
			return 1;
		}
		
		// 자리수 검사
		nMessageLength	=	sMessage.GetLength();
		if( 80 < nMessageLength )
		{
			CString sEMsg;
			sEMsg.Format(_T("%s의기본 문자가 80byte(한글40자,영문80자)를 초과할수 없습니다.") , sName);
//			AfxMessageBox(sEMsg);
//			return 2;
		}

		pTypeEnvDM->SetCellData( _T("종류") , sName , i );
		pTypeEnvDM->SetCellData( _T("기본문자") , sMessage , i );
	}
	
	return 0;

EFS_END
return -1;
}

/*
@Create
 - 
@Description
 - 미리보기 문자 구성
@Parameter

@Return Value
 - 
 - 0, 양수 : 성공
 - 음수 : 실패
@Mini Spec
 1. 
*/
INT CMNG_SERVICE_1110::ShowPreView()
{
	EFS_BEGIN

	// 문자 종류별 구성정보
	CESL_Grid*		pGrid = (CESL_Grid*)FindControl( _T("CM_MNG_SMS_SERVICE_1110") , _T("문자종류별내용") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ShowPreView") );
	}

	INT nIndex = pGrid->GetIdx();

	CString		strSMSType;
	GetTypeEnvDM()->GetCellData( _T("타입") , nIndex , strSMSType);

	SMS_MSG_INFO		SMS_MsgInfo;

	// 2009.04.17 ADD BY KSJ : 미리보기 구성시 DB에서 값가져오는 방식에서 DM에서 가져오는 방식으로 변경
//DEL 	SMS_CONNECTION_INFO SMS_ConnectionInfo;
//DEL 	CSMS_Env_Info		SMSEnvInfo(this);
//DEL 	SMSEnvInfo.MakeSMSEnv();
//DEL 	SMSEnvInfo.GetSMSEnv( strSMSType , &SMS_ConnectionInfo , &SMS_MsgInfo );
	GetTotalEnvDM()->GetCellData( _T("회신전화번호"),			0 , SMS_MsgInfo.sCallBackNumber );
	GetTotalEnvDM()->GetCellData( _T("나누어보내기허용여부"),	0 , SMS_MsgInfo.sDivideYN );
	// 2009.10.15 UPDATE BY KSJ : 도서관명 MN_LIBINFO2_TBL의 SMS_LIB_NAME 가져오도록 수정
	//DEL GetTotalEnvDM()->GetCellData( _T("도서관명"),				0 , SMS_MsgInfo.sLibName );
	SMS_MsgInfo.sLibName = m_strLibName;

	CString		strTitleSize;
	GetTotalEnvDM()->GetCellData( _T("TITLE_SIZE") , 0 , strTitleSize );

	INT		nTitleSize				=	_ttoi( strTitleSize );

	SMS_MsgInfo.nTitleSize			=	nTitleSize;
	SMS_MsgInfo.sDefaultMsg			=	_T("");
	SMS_MsgInfo.sTitle				=	_T("");
	SMS_MsgInfo.sMsgComponentInfo	=	_T("");

	const INT nCnt = 5;
	INT nIDC[nCnt] =
	{
		IDC_chkCOM1 , IDC_chkCOM3 , IDC_chkCOM4 , IDC_chkCOM5 , IDC_chkCOM2
	};
	
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		if( ((CButton*)GetDlgItem(nIDC[i]))->GetCheck() == 1 )
		{
			SMS_MsgInfo.sMsgComponentInfo += _T("1");

			// 2009.05.08 ADD BY KSJ : 서명에 Check가 되어 있으면 서명 크기 에디트 박스 ReadOnly FALSE
			if( 3 == i )
			{
				CEdit*	pEdtTitleSize = (CEdit*) GetDlgItem ( IDC_edtBookTitleByte );
				DWORD dwStyle = pEdtTitleSize->GetStyle();
				dwStyle &= ES_READONLY;
				if( ES_READONLY == (dwStyle & ES_READONLY) )
				{
					pEdtTitleSize->SetReadOnly( FALSE );
				}
			}
		}
		else
		{
			SMS_MsgInfo.sMsgComponentInfo += _T("0");

			// 2009.05.08 ADD BY KSJ : 서명에 Check가 되어 있으면 서명 크기 에디트 박스 ReadOnly TRUE
			if( 3 == i )
			{
				CEdit*	pEdtTitleSize = (CEdit*) GetDlgItem ( IDC_edtBookTitleByte );
				DWORD dwStyle = pEdtTitleSize->GetStyle();
				dwStyle &= ES_READONLY;
				if( (dwStyle & ES_READONLY) != ES_READONLY)
				{
					pEdtTitleSize->SetReadOnly( TRUE );// 읽기만 가능
				}
			}
		}
	}
	
	pGrid->GetAt( nIndex , 0  , SMS_MsgInfo.sTitle );
	pGrid->GetAt( nIndex , 1  , SMS_MsgInfo.sDefaultMsg );

	CSMS_MakeMsg MakeSMS(this);

	CString		strMsg;
	MakeSMS.MakeSMSMsg( strSMSType , &SMS_MsgInfo , _T("홍길동") , _T("2003/10/10") , _T("대하소설 의적홍길동") , _T("3") , strMsg );

	GetDlgItem(IDC_edtPREVIEW)->SetWindowText( strMsg );

	// 2009.04.01 ADD BY KSJ : 서명 크기 컨트롤에 값 Setting
	//CString		strTitleSize;
	strTitleSize.Format( _T("%d") , SMS_MsgInfo.nTitleSize );
	GetDlgItem( IDC_edtBookTitleByte )->SetWindowText( strTitleSize );

	return 0;

EFS_END
return -1;
}

BEGIN_EVENTSINK_MAP(CMNG_SERVICE_1110, CDialog)
    //{{AFX_EVENTSINK_MAP(CMNG_SERVICE_1110)
	ON_EVENT(CMNG_SERVICE_1110, IDC_MAINGRID, -600 /* Click */, OnClickMaingrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CMNG_SERVICE_1110::OnClickMaingrid() 
{
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_MNG_SMS_SERVICE_1110") , _T("문자종류별내용") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010 , _T("OnClickMaingrid") );
	}

	if( 1 > pGrid->GetMouseRow() ) 
	{
		return ;
	}

	ShowPreView();
}


HBRUSH CMNG_SERVICE_1110::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

LRESULT CMNG_SERVICE_1110::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// 2009.05.15 ADD BY KSJ : GridSelEditor에서 Enter가 눌러졌을 경우 미리보기 갱신
	if( ESL_GRID_MODIFY == message)
	{
		ShowPreView();
	}

	// 2009.10.15 ADD BY KSJ : 도서관명 Setting
	if( WM_SHOWWINDOW == message )
	{
		if( TRUE == wParam )
		{
			CESL_DataMgr* pDM = GetTotalEnvDM();
			CString strQuery;
			strQuery.Format( _T("SELECT SMS_LIB_NAME FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE") );
			pDM->GetOneValueQuery( strQuery , m_strLibName );

			// 2010.12.22 ADD BY PWR : 회신번호, 도서관명 PC설정값 적용
			CCfgFileApi cfgFileApi;
			CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
			cfgFileApi.SetFile(strCfgFilePath);
			CFileFind ff;
			if(ff.FindFile(strCfgFilePath))
			{
				CString strLibNameTarget;
				cfgFileApi.GetData(_T("도서관명설정적용대상"), strLibNameTarget);
				if(_T("1") == strLibNameTarget)
				{
					CString strValue;
					cfgFileApi.GetData(_T("도서관명"), strValue);
					if(!strValue.IsEmpty()) m_strLibName = strValue;
				}
			}
		}
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}

/*
@Create
 - 2009.05.15 BY KSJ
@Description
 - 구성정보 변경시 미리보기 화면 갱신
@Parameter

@Return Value
 - 
 - 0, 양수 : 성공
 - 음수 : 실패
@Mini Spec
 1. 도서관명
 2. 수신자이름
 3. 기본문자
 4. 반납예정일/예약만기일
 5. 서명
*/
void CMNG_SERVICE_1110::OnchkCOM2() 
{/// 1. 도서관명
	ShowPreView();		
}

void CMNG_SERVICE_1110::OnchkCOM1() 
{/// 2. 수신자이름
	ShowPreView();
}

void CMNG_SERVICE_1110::OnchkCOM3() 
{/// 3. 기본문자
	ShowPreView();
}

void CMNG_SERVICE_1110::OnchkCOM4() 
{/// 4. 반납예정일/예약만기일
	ShowPreView();
}

void CMNG_SERVICE_1110::OnchkCOM5() 
{/// 5. 서명
	ShowPreView();
}
