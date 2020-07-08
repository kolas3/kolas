// SMSContentsEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SMSContentsEditDlg.h"
#include "..\\CO_SMS\\CfgFileApi.h"
#include "imm.h"
#include "..\\SMS_04\\SMS_4100.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SMS_MAXBYTE		80
#define LMS_MAXBYTE		2000

CSMSContentsEditDlg::CSMSContentsEditDlg(CESL_Mgr* pParent, CStringList &SMSList, INT SMSType )
	: CESL_Mgr( IDD, pParent)
{
	//{{AFX_DATA_INIT(CSMSContentsEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nSMSType = SMSType;
	m_currentApplication = FALSE;

	POSITION pos = SMSList.GetHeadPosition();
	while( pos )
	{
		m_SMSList.AddTail( SMSList.GetNext( pos ) );
	}

	m_nSMS03PublishFormCode		=	0;

	m_strNightLoanReservationUseYn = _T("");

	m_pParent = pParent;

	m_str_SmsYearLength = _T("");
}

CSMSContentsEditDlg::~CSMSContentsEditDlg()
{ 
}

BOOL CSMSContentsEditDlg::Create(CWnd* pParentWnd)
{	
	return CDialog::Create(IDD, pParentWnd);  
}	

VOID CSMSContentsEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSMSContentsEditDlg)
	DDX_Control(pDX, IDC_COMBO_SMS1, m_ctlSmsSendType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSMSContentsEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSMSContentsEditDlg)
	ON_EN_CHANGE(IDC_eSMS_CONTENTS, OnChangeeSMSCONTENTS)
	ON_CBN_SELCHANGE(IDC_cSMS_TYPE, OnSelchangecSMSTYPE)
	ON_BN_CLICKED(IDC_bSMS_SAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_CHECK5, OnChkLibName)
	ON_BN_CLICKED(IDC_CHECK1, OnChkUserName)
	ON_BN_CLICKED(IDC_CHECK2, OnChkDefContents)
	ON_BN_CLICKED(IDC_CHECK3, OnChkDateInfo)
	ON_BN_CLICKED(IDC_CHECK4, OnChkTitle)
	ON_EN_CHANGE(IDC_eURL, OnChangeeURL)
	ON_CBN_SELCHANGE(IDC_COMBO_SMS1, OnSelchangeCombo1)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_eSMS_CONTENTS, OnSetfocuseSMSCONTENTS)
	ON_BN_CLICKED(IDC_btnSMSContentsSend, OnbtnSMSContentsSend)
	ON_EN_CHANGE(IDC_edtLIB_NAME, OnChangeedtLIBNAME)
	ON_EN_SETFOCUS(IDC_edtLIB_NAME, OnSetfocusedtLIBNAME)
	ON_EN_CHANGE(IDC_edtTitleSize, OnChangeedtTitleSize)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSMSContentsEditDlg message handlers

BOOL CSMSContentsEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("SMSContentsEditDlg") ) < 0 )
	{
		AfxMessageBox( _T("InitESL_Mgr Error : SMSContentsEditDlg") );
		return false;
	}
	
	CESL_DataMgr pDM;
	pDM.SetCONNECTION_INFO(pParentMgr->m_pInfo->CONNECTION_INFO);
	CString strWhere = _T("");
	strWhere.Format( 
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS='SMS연도표기방식' AND value_name='연도자릿수설정' AND MANAGE_CODE='%s'"),pParentMgr->m_pInfo->MANAGE_CODE);
	pDM.GetOneValueQuery(strWhere, m_str_SmsYearLength);
	
	pDM.FreeData();

	InitScreen();
	
	SearchSMSInfo();
	MakeSMSComboBox();
	
	SearchSMSDefaulgInfo();
	ViewSMSComposition();
	ViewCallbackNum();
	ViewLibName();
	ViewTitleSize();

	ViewSMSContentsBytes();

	( (CComboBox*) GetDlgItem (IDC_cSMS_TYPE) )->SetCurSel(0);

	( (CButton*) GetDlgItem ( IDC_radSMSContentsSelect ) )->SetCheck(1);
	( (CButton*) GetDlgItem ( IDC_radSMSContentsAll ) )->SetCheck(0);

	OnSelchangecSMSTYPE();

	SetMsgPreview();
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH CSMSContentsEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

VOID CSMSContentsEditDlg::OnChangeeSMSCONTENTS() 
{
	CEdit *pEdtContents = ( CEdit * )GetDlgItem( IDC_eSMS_CONTENTS );
	if( NULL == pEdtContents || NULL == pEdtContents->GetSafeHwnd() )
	{
		return;
	}

	pEdtContents->GetWindowText( m_strDefaultMsg );
	SetMsgPreview();

	ViewSMSContentsBytes();
}

void CSMSContentsEditDlg::OnChangeedtLIBNAME() 
{
	CEdit *pEdtLibName = ( CEdit * )GetDlgItem( IDC_edtLIB_NAME );
	if( NULL == pEdtLibName || NULL == pEdtLibName->GetSafeHwnd() )
	{
		return;
	}

	pEdtLibName->GetWindowText( m_strLibName );
	SetMsgPreview();	

	ViewSMSContentsBytes();
}

void CSMSContentsEditDlg::OnChangeedtTitleSize() 
{
	CEdit *pEdtTitleSize = ( CEdit * )GetDlgItem( IDC_edtTitleSize );
	if( NULL == pEdtTitleSize || NULL == pEdtTitleSize->GetSafeHwnd() )
	{
		return;
	}

	CString		strTitleSize;
	pEdtTitleSize->GetWindowText( strTitleSize );
	m_nTitleSize	=	_ttoi( strTitleSize );
}

VOID CSMSContentsEditDlg::ViewSMSContentsBytes()
{
	INT nCurPayByte = 0;
	INT nCurMAXByte = SMS_MAXBYTE;

	if ( _T("1") == m_strSMSSystemInfo || _T("3") == m_strSMSSystemInfo || _T("4") == m_strSMSSystemInfo ) 
	{
		CString strSMSType;
		m_ctlSmsSendType.GetWindowText( strSMSType );

		if ( 0 < strSMSType.Replace( _T("URL"), _T("") ) )
		{
			CEdit* pEdtURL = ( CEdit* )GetDlgItem( IDC_eURL );
			if ( NULL == pEdtURL ) 
			{
				ERROR_MSG_RETURN_VOID2( -1010, _T("ViewSMSContentsBytes") );	
			}
			
			if ( 0 < pEdtURL->GetWindowTextLength() )
			{
				nCurMAXByte -= ( pEdtURL->GetWindowTextLength() + 1 );	
			}			 
		}
	}


	const INT nEleCnt = 5;
	CButton* pBtn     = NULL;
	wchar_t *strUni = NULL;
	CString sContents = _T(""), sTemp = _T("");
	INT		nID[ nEleCnt ] = { IDC_CHECK5, IDC_CHECK1, IDC_CHECK2, IDC_CHECK3, IDC_CHECK4 };
	INT		i;
	for ( i=0 ; i<nEleCnt; i++ )
	{		
		pBtn = (CButton*)GetDlgItem( nID[ i ] );

		if ( pBtn->IsWindowEnabled() && pBtn->GetCheck() )
		{
			switch( i )
			{
				case 0:
					{												
						GetDlgItem(IDC_edtLIB_NAME)->GetWindowText(sContents);
						strUni = (wchar_t*)sContents.GetBuffer(0);
						int nLen = WideCharToMultiByte(CP_ACP,0,strUni,lstrlenW(strUni),NULL,0,NULL,NULL);
						nLen +=2 ;
						nCurPayByte += nLen;
					}
					break;
				case 1:
					{
					}
					break;
				case 2:	
					{
						GetDlgItem(IDC_eSMS_CONTENTS)->GetWindowText(sContents);
						strUni = (wchar_t*)sContents.GetBuffer(0);
						int nLen = WideCharToMultiByte(CP_ACP,0,strUni,lstrlenW(strUni),NULL,0,NULL,NULL);
												
						nCurPayByte += nLen;
					}
					break;
				case 3:
					{
						if ( m_str_SmsYearLength == _T("Y") )
							nCurPayByte += 20;
						else
							nCurPayByte += 22;
					}
					break;
				case 4:
					{			
					}
					break;
			}
		}
	}
	
	CStatic* pStatic = ( CStatic* )GetDlgItem( IDC_staSMSByte );
	if ( NULL == pStatic ) 
	{
		ERROR_MSG_RETURN_VOID2( -1020, _T("ViewSMSContentsBytes") );
	}

	CString strDisplayStatus, strDisplayByte;

	if(m_IsLMSUse == TRUE && nCurPayByte > 160){
		nCurMAXByte = LMS_MAXBYTE;
		strDisplayStatus.Format(_T("상태 : LMS 장문발송"));
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("상태 : LMS 장문발송"));
	}else if(m_IsLMSUse == TRUE && nCurPayByte <= 160 && nCurPayByte > 80){
		nCurMAXByte = SMS_MAXBYTE;
		strDisplayStatus.Format(_T("상태 : SMS 2건발송"));
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("상태 : SMS 2건발송"));
	}else{
		nCurMAXByte = SMS_MAXBYTE;
		strDisplayStatus.Format(_T("상태 : SMS 단문발송"));
		GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(_T("상태 : SMS 단문발송"));
	}

	if(m_IsLMSUse == TRUE && nCurPayByte > LMS_MAXBYTE){
		strDisplayStatus.Format(_T("%d Byte를 넘을 수 없습니다."), LMS_MAXBYTE);
		GetDlgItem(IDC_btnSMSContentsSend)->EnableWindow(FALSE);
	}else{
		GetDlgItem(IDC_btnSMSContentsSend)->EnableWindow(TRUE);
	}
	
	GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strDisplayStatus);

	strDisplayByte.Format( _T("%d / %d BYTE"), nCurPayByte, nCurMAXByte );
	pStatic->SetWindowText( strDisplayByte );

	GetDlgItem(IDC_edtAllDataSize)->SetWindowText(strDisplayByte);
}

VOID CSMSContentsEditDlg::SearchSMSInfo()
{	
	CString where, tmp;
	
	if( m_SMSList.GetCount() )
	{
		where = _T("TYPE IN( ");
		POSITION pos = m_SMSList.GetHeadPosition();
		while( pos )
		{
			tmp.Format( _T("'%s', "), m_SMSList.GetNext( pos ) );
			where += tmp;
		}

		where = where.Left( where.GetLength() - 2 );
		where += _T(" )");
	}
	
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg") );
	pDM->RefreshDataManager( where );

	INT		i , nRowCnt;
	nRowCnt		=	pDM->GetRowCount();
	for( i=0 ; i<nRowCnt , i<SMS_COUNT ; i++ )
	{
		m_SMSContent[ i ] = pDM->GetCellData( _T("문자종류"), i );
	}

	SetMode();
}

VOID CSMSContentsEditDlg::MakeSMSComboBox()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg") );
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cSMS_TYPE );	
	
	INT nRowCnt = pDM->GetRowCount();
	INT		i;
	for( i=0 ; i<nRowCnt ; i++ )
	{
		pCombo->InsertString( i, pDM->GetCellData( _T("문자이름"), i ) );
	}
	
	if( nRowCnt -1 < m_nSMSType )
	{
		m_nSMSType = 0;
	}

	pCombo->SetCurSel( m_nSMSType );

	CString strTmp;
	CString strPhone, strURL, strMSGType;
	CString strWhere;	
	CString strAppendWhere;
	
	if ( TRUE == m_strSMSSystemInfo.IsEmpty() )
	{
		strAppendWhere = _T(" WHERE MANAGE_CODE=UDF_MANAGE_CODE");
	}
	else
	{
		strAppendWhere.Format(_T(" WHERE SMS_TYPE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), m_strSMSSystemInfo );
	}


	strWhere.Format(_T("SELECT LMS_USE_YN FROM MN_SMS_TOTAL_ENV_TBL "));
	strWhere+=strAppendWhere;
	pDM->GetOneValueQuery(strWhere, strTmp);
	
	m_IsLMSUse = FALSE;
	if(strTmp == _T("Y"))
		m_IsLMSUse = TRUE;
	
	strWhere.Format(_T("SELECT URL FROM MN_SMS_TOTAL_ENV_TBL "));
	strWhere+=strAppendWhere;
	pDM->GetOneValueQuery(strWhere, strTmp);
	strURL = strTmp;
	
	strWhere.Format(_T("SELECT MSG_GB FROM MN_SMS_TOTAL_ENV_TBL"));
	strWhere+=strAppendWhere;
	pDM->GetOneValueQuery(strWhere, strTmp);
	strMSGType = strTmp;

	( (CEdit*) GetDlgItem(IDC_eURL) )->SetWindowText(strURL);

	m_ctlSmsSendType.ResetContent();
	if ( _T("1") == m_strSMSSystemInfo )
	{
		m_ctlSmsSendType.AddString(_T("SMS"));
		m_ctlSmsSendType.AddString(_T("URL"));
		m_ctlSmsSendType.AddString(_T("해외SMS"));
		m_ctlSmsSendType.AddString(_T("해외URL"));

		if ( strMSGType == _T("A") )
		{
			m_ctlSmsSendType.SetCurSel(0);
		}
		else if ( strMSGType == _T("B") )
		{
			m_ctlSmsSendType.SetCurSel(1);
		}
		else if ( strMSGType == _T("C") )
		{
			m_ctlSmsSendType.SetCurSel(2);
		}
		else if ( strMSGType == _T("D") )
		{
			m_ctlSmsSendType.SetCurSel(3);
		}
		else
		{
			m_ctlSmsSendType.SetCurSel(0);
		}
	}
	else if ( _T("3") == m_strSMSSystemInfo )
	{
		m_ctlSmsSendType.AddString(_T("SMS"));
		m_ctlSmsSendType.AddString(_T("URL"));

		if ( strMSGType == _T("0")  )
		{
			m_ctlSmsSendType.SetCurSel(0);
		}
		else if ( strMSGType == _T("5") )
		{
			m_ctlSmsSendType.SetCurSel(1);
		}
		else
		{
			m_ctlSmsSendType.SetCurSel(0);
		}
	}
	else if ( _T("4") == m_strSMSSystemInfo )
	{
		m_ctlSmsSendType.AddString(_T("SMS"));
		m_ctlSmsSendType.AddString(_T("URL"));
		if ( strMSGType == _T("0")  )
		{
			m_ctlSmsSendType.SetCurSel(0);
		}
		else if ( strMSGType == _T("1") )
		{
			m_ctlSmsSendType.SetCurSel(1);
		}
	}
}

VOID CSMSContentsEditDlg::OnSelchangecSMSTYPE() 
{
	// TODO: Add your control notification handler code here
	
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cSMS_TYPE );
	m_nSMSType = pCombo->GetCurSel();
	ViewSMSContent();
	EnableCompositioin();
	ViewReceiverNumEdt();
}

VOID CSMSContentsEditDlg::ViewSMSContent()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg") );
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eSMS_CONTENTS );
	if( NULL == pEdt || NULL == pEdt->GetSafeHwnd() )
	{
		return;
	}

	m_strDefaultMsg = pDM->GetCellData( _T("기본메세지"), m_nSMSType );
	pEdt->SetWindowText( m_strDefaultMsg );

	SetMsgPreview();
	
	ViewSMSContentsBytes();
}	

VOID CSMSContentsEditDlg::SearchSMSDefaulgInfo()
{
	CString strWhere;
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg_SMS") );

	if ( TRUE == m_strSMSSystemInfo.IsEmpty() )
	{
		strWhere.Format(_T("SMS_TYPE = '' OR SMS_TYPE IS NULL"));
	}
	else
	{
		strWhere.Format(_T("SMS_TYPE = '%s'"), m_strSMSSystemInfo );
	}

	pDM->RefreshDataManager( strWhere );

	INT ids;

	CString query;
	query.Format(	_T("SELECT SMS_CALLBACK_PHONE, SMS_LIB_NAME ")
					_T("FROM MN_LIBINFO2_TBL ")
		            _T("WHERE MANAGE_CODE=UDF_MANAGE_CODE") );		

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO( pDM->CONNECTION_INFO );
	ids = TmpDM.RestructDataManager( query );
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1010 , _T("GetSMSConnectionEnv") );
	}

	CString strCallBack = TmpDM.GetCellData(0,0);
	CString strLibName = TmpDM.GetCellData(0,1);
	
	CCfgFileApi cfgFileApi;
	CString strCfgFilePath = _T("..\\cfg\\SMS_Callback.cfg");
	cfgFileApi.SetFile(strCfgFilePath);
	CFileFind ff;
	if(ff.FindFile(strCfgFilePath))
	{
		CString strCallbackTarget;
		cfgFileApi.GetData(_T("회신번호설정적용대상"), strCallbackTarget);
		CString strValue;
		if(_T("1") == strCallbackTarget)
		{
			cfgFileApi.GetData(_T("회신번호"), strValue);
			if(!strValue.IsEmpty()) strCallBack = strValue;
		}
		CString strLibNameTarget;
		cfgFileApi.GetData(_T("도서관명설정적용대상"), strLibNameTarget);
		if(_T("1") == strLibNameTarget)
		{
			cfgFileApi.GetData(_T("도서관명"), strValue);
			if(!strValue.IsEmpty()) strLibName = strValue;
		}
	}

	ids = pDM->SetCellData(_T("CALLBACK_PHONE"), strCallBack, 0);
	if( 0 > ids )
	{
		ERROR_MSG_RETURN_VOID2( -1020 , _T("GetSMSConnectionEnv") );
	}

	ids = pDM->SetCellData(_T("LIB_NAME"), strLibName, 0);
	if( 0 > ids ) 
	{
		ERROR_MSG_RETURN_VOID2( -1030 , _T("GetSMSConnectionEnv") );
	}
}

VOID CSMSContentsEditDlg::ViewSMSComposition()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg_SMS") );
	CString strComposition = pDM->GetCellData( _T("SMS_MAKE_COMPONENT"), 0 );

	const INT nCnt = 5;
	UINT id[ nCnt ] = { IDC_CHECK1 , IDC_CHECK2 , IDC_CHECK3 , IDC_CHECK4 , IDC_CHECK5 };

	CButton *pBtn = NULL;
	INT		i , nComposition;
	for( i=0; i<nCnt ; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );

		if (TRUE == strComposition.IsEmpty()) {
			pBtn->SetCheck( false );
		} else {
			nComposition	=	_ttoi( strComposition.Mid( i, 1 ) );
			if( 1 == nComposition )
			{
				pBtn->SetCheck( true );
			}
			else
			{
				pBtn->SetCheck( false );
			}
		}

	}
}

VOID CSMSContentsEditDlg::ViewCallbackNum()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg_SMS") );
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eRECEIVE_NUMBER );
	pEdt->SetWindowText( pDM->GetCellData( _T("CALLBACK_PHONE"), 0 ) );
}

INT CSMSContentsEditDlg::ViewLibName()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg_SMS") );
	CString strLibName = pDM->GetCellData( _T("LIB_NAME"), 0 );

	CEdit* pEdt = (CEdit*)GetDlgItem(IDC_edtLIB_NAME);
	pEdt->SetWindowText( strLibName );

	return 0;
}

INT CSMSContentsEditDlg::ViewTitleSize()
{
EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg_SMS") );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("ViewTitleSize") );
	}
	CString strTitleSize = pDM->GetCellData( _T("TITLE_SIZE"), 0 );

	CEdit*		pEdt = (CEdit*)GetDlgItem( IDC_edtTitleSize );
	pEdt->SetWindowText( strTitleSize );

	INT	nTitleSize	=	_ttoi( strTitleSize );
	m_nTitleSize	=	nTitleSize;	

	return 0;

EFS_END
return -1;
}

VOID CSMSContentsEditDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	OnbSAVE();

	SaveSMSContents();
	SaveSMSInfo();
	AfxMessageBox( _T("저장되었습니다.") );
	
}

INT CSMSContentsEditDlg::SaveSMSContents()
{
	if( -1 == m_nSMSType )
	{
		return -1;
	}
	
	CString strContents;
	CEdit	*pEdt = ( CEdit * )GetDlgItem( IDC_eSMS_CONTENTS );
	pEdt->GetWindowText( strContents );

	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg") );
	CString query;
	query.Format( _T("UPDATE MN_SMS_TYPE_ENV_TBL SET DEFAULT_MESSAGE = '%s' WHERE REC_KEY = %s;"), strContents , pDM->GetCellData( _T("REC_KEY"), m_nSMSType ) );
	pDM->ExecuteQuery( query, 1 );
	
	return 0;
}

INT CSMSContentsEditDlg::SaveSMSInfo()
{
	CString query, callbackNum;
	CString strURL, strMSGGB;	

	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eRECEIVE_NUMBER );
	pEdt->GetWindowText( callbackNum );

	( (CEdit*) GetDlgItem(IDC_eURL) )->GetWindowText(strURL);
	if ( !strURL.IsEmpty() )
	{
	}

	( (CComboBox*) GetDlgItem(IDC_COMBO_SMS1) )->GetWindowText(strMSGGB);
	if ( strMSGGB == _T("SMS") )
	{
		if ( m_strSMSSystemInfo == _T("1") ) strMSGGB=_T("A");
		else if ( m_strSMSSystemInfo == _T("3") ) strMSGGB=_T("0");
		else if ( m_strSMSSystemInfo == _T("4") ) strMSGGB=_T("0");
	}
	else if ( strMSGGB == _T("URL") )
	{
		if ( m_strSMSSystemInfo == _T("1") ) strMSGGB=_T("B");
		else if ( m_strSMSSystemInfo == _T("3") ) strMSGGB=_T("5");
		else if ( m_strSMSSystemInfo == _T("4") ) strMSGGB=_T("1");
	}
	else if ( strMSGGB == _T("해외SMS") )
	{
		strMSGGB=_T("C");
	}
	else if ( strMSGGB == _T("해외URL") )
	{
		strMSGGB=_T("D");
	}

	CString strAppendWhere;
	if ( m_strSMSSystemInfo.IsEmpty() )
	{
		strAppendWhere=_T(" WHERE MANAGE_CODE=UDF_MANAGE_CODE");
	}
	else
	{
		strAppendWhere.Format(_T("WHERE SMS_TYPE='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), m_strSMSSystemInfo);
	}

	query.Format( _T("UPDATE MN_SMS_TOTAL_ENV_TBL SET SMS_MAKE_COMPONENT = '%s', URL='%s', MSG_GB='%s' %s;"), GetSMSComposition(),  strURL, strMSGGB, strAppendWhere );
	
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg_SMS") );
	pDM->ExecuteQuery( query, 1 );
	
	INT ids = 0;

	query.Format(_T("UPDATE MN_LIBINFO2_TBL SET SMS_CALLBACK_PHONE='%s' ")					  
		         _T("WHERE MANAGE_CODE=UDF_MANAGE_CODE;"), callbackNum );		

	pDM->ExecuteQuery( query, 1 );

	return 0;
}

CString CSMSContentsEditDlg::GetSMSComposition()
{
	const INT cnt = 5;
	UINT id[ cnt ] = {
		IDC_CHECK1, IDC_CHECK2, IDC_CHECK3, IDC_CHECK4, IDC_CHECK5 };

	CString composition, tmp;
	CButton *pBtn = NULL;
	for( INT i = 0; i < cnt; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		if( pBtn->GetCheck() )
			tmp = _T("1");
		else
			tmp = _T("0");

		composition += tmp;
	}

	return composition;
}

VOID CSMSContentsEditDlg::OnbSAVE() 
{
	// TODO: Add your control notification handler code here
	
	m_composition = GetSMSComposition();
	CEdit *pEdt = NULL;
	CComboBox *pCombo = ( CComboBox * )GetDlgItem( IDC_cSMS_TYPE );
	pEdt = ( CEdit * )GetDlgItem( IDC_eRECEIVE_NUMBER );
	pEdt->GetWindowText( m_callbackNum );
	pEdt = ( CEdit * )GetDlgItem( IDC_eSMS_CONTENTS );
	pEdt->GetWindowText( m_SMSContent[ _ttoi( m_SMSList.GetAt( m_SMSList.FindIndex( pCombo->GetCurSel() ) ).Right( 2 ) ) - 1 ] );	
	
	m_currentApplication = true;
}

INT CSMSContentsEditDlg::GetSMSInfo( CString type, SMS_MSG_INFO &SMS_MsgInfo, SMS_CONNECTION_INFO &SMS_ConnectionInfo, INT nMode )
{
	CSMS_Env_Info SMSEnvInfo( this );
	SMSEnvInfo.MakeSMSEnv();
	SMSEnvInfo.GetSMSEnv( type , &SMS_ConnectionInfo , &SMS_MsgInfo );

	if(nMode == 0)
	{
		CString		strDefaultMsg , strLibCode , strCallBackNumber , strTitleSize;
		((CEdit*)GetDlgItem( IDC_eSMS_CONTENTS ))->GetWindowText( strDefaultMsg );
		SMS_MsgInfo.sDefaultMsg		=	strDefaultMsg;

		((CEdit*)GetDlgItem( IDC_eRECEIVE_NUMBER ))->GetWindowText( strCallBackNumber );
		SMS_MsgInfo.sCallBackNumber	=	strCallBackNumber;

		((CEdit*)GetDlgItem( IDC_edtTitleSize ))->GetWindowText( strTitleSize );
		SMS_MsgInfo.sCallBackNumber	=	strCallBackNumber;
	}
	
	return 0;
}

VOID CSMSContentsEditDlg::EnableCompositioin()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg") );
	CString type = pDM->GetCellData( _T("문자종류"), m_nSMSType );

	const INT nCnt = 5;
	BOOL bFlag[ nCnt ] = { TRUE , TRUE , TRUE , TRUE , TRUE };

	switch( _ttoi( type.Right( 2 ) ) )
	{
		case 4 :
		case 7 :
		case 8 :
			bFlag[ 3 ] = FALSE;
			break;
		case 5 :
		case 6 :
		case 9 :
			bFlag[ 3 ] = FALSE;
			bFlag[ 4 ] = FALSE;
			break;
		default :
			return;
	}

	CButton *pBtn = NULL;
	UINT	id[ nCnt ] = { IDC_CHECK5 , IDC_CHECK1 , IDC_CHECK2 , IDC_CHECK3 , IDC_CHECK4 };
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		pBtn = ( CButton * )GetDlgItem( id[ i ] );
		pBtn->EnableWindow( bFlag[ i ] );
	}
}

VOID CSMSContentsEditDlg::ViewReceiverNumEdt()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg") );
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eRECEIVE_NUMBER2 );
	CStatic *pStatic = ( CStatic * )GetDlgItem( IDC_sRECEIVER_NUM );
	
	BOOL bFlag = FALSE;
	if( _T("SMS09") == pDM->GetCellData( _T("문자종류"), m_nSMSType ) )
	{
		bFlag = TRUE;
	}

	pStatic->ShowWindow( bFlag );
	pEdt->ShowWindow( bFlag );
}

CString CSMSContentsEditDlg::GetReceiverNum()
{
	CString receiverNum;
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eRECEIVE_NUMBER2 );
	pEdt->GetWindowText( receiverNum );

	return receiverNum;
}

VOID CSMSContentsEditDlg::InitScreen()
{
	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg") );
	CString strWhere;
	const INT nMovePitch = 85;
	
	strWhere=	_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
				_T("WHERE CLASS_ALIAS='SMS시스템정보' AND MANAGE_CODE=UDF_MANAGE_CODE");

	pDM->GetOneValueQuery(strWhere, m_strSMSSystemInfo);
}

VOID CSMSContentsEditDlg::OnChkLibName() 
{
	ViewSMSContentsBytes();

	SetMsgPreview();
}

VOID CSMSContentsEditDlg::OnChkUserName() 
{
	ViewSMSContentsBytes();

	SetMsgPreview();
}

VOID CSMSContentsEditDlg::OnChkDefContents() 
{
	ViewSMSContentsBytes();

	SetMsgPreview();
}

VOID CSMSContentsEditDlg::OnChkDateInfo() 
{
	ViewSMSContentsBytes();

	SetMsgPreview();
}

VOID CSMSContentsEditDlg::OnChkTitle() 
{
	ViewSMSContentsBytes();

	SetMsgPreview();
}

VOID CSMSContentsEditDlg::OnChangeeURL() 
{
	ViewSMSContentsBytes();

	SetMsgPreview();
}

VOID CSMSContentsEditDlg::OnSelchangeCombo1() 
{
	ViewSMSContentsBytes();

	SetMsgPreview();
}

void CSMSContentsEditDlg::OnSetfocuseSMSCONTENTS() 
{
EFS_BEGIN

	CEdit	*pEditSMSContents;
	pEditSMSContents	=	( CEdit*) GetDlgItem (IDC_eSMS_CONTENTS);
		
	HIMC	hIME;
	DWORD	dwConversion, dwSentence;
	
	hIME = ::ImmGetContext( pEditSMSContents->m_hWnd );
	if( NULL != hIME ) 
	{
		ImmGetConversionStatus( hIME , &dwConversion , &dwSentence );
		
		dwConversion	=	IME_CMODE_HANGUL;
		
		ImmSetConversionStatus( hIME , dwConversion , dwSentence );
		
		ImmReleaseContext( pEditSMSContents->m_hWnd , hIME ); 
	} 

EFS_END		
}

void CSMSContentsEditDlg::OnSetfocusedtLIBNAME() 
{
EFS_BEGIN

	CEdit	*pEditLibName;
	pEditLibName	=	( CEdit*) GetDlgItem (IDC_edtLIB_NAME);
		
	HIMC	hIME;
	DWORD	dwConversion, dwSentence;
	
	hIME = ::ImmGetContext( pEditLibName->m_hWnd );
	if( NULL != hIME ) 
	{
		ImmGetConversionStatus( hIME , &dwConversion , &dwSentence );
		
		dwConversion	=	IME_CMODE_HANGUL;
		
		ImmSetConversionStatus( hIME , dwConversion , dwSentence );
		
		ImmReleaseContext( pEditLibName->m_hWnd , hIME ); 
	} 

EFS_END		
}

VOID CSMSContentsEditDlg::SetMode()
{
	if( _T("SMS01") == m_SMSContent[0] || _T("SMS02") == m_SMSContent[0] || _T("SMS03") == m_SMSContent[0] || _T("NRL03") == m_SMSContent[0] )
	{
		m_nMenuMode		=	1;
		((CComboBox*)GetDlgItem( IDC_cSMS_TYPE ))->EnableWindow( FALSE );
	}
	else if( _T("SMS04") == m_SMSContent[0] ||  _T("SMS18") == m_SMSContent[0] ||  _T("SMS19") == m_SMSContent[0])
	{
		m_nMenuMode		=	2;
		((CComboBox*)GetDlgItem( IDC_cSMS_TYPE ))->EnableWindow( FALSE );
	}
	else if( _T("SMS07") == m_SMSContent[0] ||  _T("SMS08") == m_SMSContent[0] )
	{
		m_nMenuMode		=	3;
	}
	else if(  _T("SMS05") == m_SMSContent[0] || _T("SMS06") == m_SMSContent[0] || _T("SMS09") == m_SMSContent[0] )
	{
		m_nMenuMode		=	4;
	}
	else
	{
		m_nMenuMode		=	0;
	}
}

INT CSMSContentsEditDlg::SetMsgPreview()
{
EFS_BEGIN
	
	BOOL		bEnabled , bChecked;
	CString		strMsg , strData , strTemp;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMSContentsEditDlg_SMS") );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SetMsgPreview") );
	}

	const INT	nCnt = 5;
	CButton*	pBtn[nCnt];
	UINT	id[ nCnt ] = { IDC_CHECK5 , IDC_CHECK1 , IDC_CHECK2 , IDC_CHECK3 , IDC_CHECK4 };
	INT		i;
	for( i=0 ; i<nCnt ; i++ )
	{
		pBtn[i] = (CButton*)GetDlgItem( id[ i ] );
		if( NULL == pBtn[i] || NULL == pBtn[i]->GetSafeHwnd() )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SetMsgPreview") );
		}
	}

	CEdit*	pEdtLibName = (CEdit*) GetDlgItem ( IDC_edtLIB_NAME );
	if( NULL == pEdtLibName || NULL == pEdtLibName->GetSafeHwnd() )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SetMsgPreview") );
	}

	bEnabled	=	pBtn[0]->IsWindowEnabled();
	bChecked	=	pBtn[0]->GetCheck();
	if( TRUE == bEnabled && TRUE == bChecked )
	{
		strMsg		=	_T("[") + m_strLibName + _T("]");
		m_bUseMsg[0]	=	TRUE;
		
		DWORD dwStyle = pEdtLibName->GetStyle();
		dwStyle &= ES_READONLY;
		if( (dwStyle & ES_READONLY) == ES_READONLY)
		{
			pEdtLibName->SetReadOnly( FALSE );
		}
	}
	else
	{
		m_bUseMsg[0]	=	FALSE;
		
		DWORD dwStyle = pEdtLibName->GetStyle();
		dwStyle &= ES_READONLY;
		if( (dwStyle & ES_READONLY) != ES_READONLY)
		{
			pEdtLibName->SetReadOnly( TRUE );
		}
	}

	bEnabled	=	pBtn[1]->IsWindowEnabled();
	bChecked	=	pBtn[1]->GetCheck();
	if( TRUE == bEnabled && TRUE == bChecked )
	{
		strMsg		+=	_T("홍길동");
		m_bUseMsg[1]	=	TRUE;
	}
	else
	{
		m_bUseMsg[1]	=	FALSE;
	}

	CEdit*	pEdtDefaultMsg = (CEdit*) GetDlgItem ( IDC_eSMS_CONTENTS );
	if( NULL == pEdtDefaultMsg || NULL == pEdtDefaultMsg->GetSafeHwnd() )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SetMsgPreview") );
	}

	bEnabled	=	pBtn[2]->IsWindowEnabled();
	bChecked	=	pBtn[2]->GetCheck();
	if( TRUE == bEnabled && TRUE == bChecked )
	{
		strMsg		+=	m_strDefaultMsg;
		m_bUseMsg[2]	=	TRUE;
		
		DWORD dwStyle = pEdtDefaultMsg->GetStyle();
		dwStyle &= ES_READONLY;
		if( (dwStyle & ES_READONLY) == ES_READONLY )
		{
			pEdtDefaultMsg->SetReadOnly( FALSE );
		}
	}
	else
	{
		m_bUseMsg[2]	=	FALSE;
		
		DWORD dwStyle = pEdtDefaultMsg->GetStyle();
		dwStyle &= ES_READONLY;
		if( (dwStyle & ES_READONLY) != ES_READONLY)
		{
			pEdtDefaultMsg->SetReadOnly( TRUE );
		}
	}

	bEnabled	=	pBtn[3]->IsWindowEnabled();
	bChecked	=	pBtn[3]->GetCheck();
	if( TRUE == bEnabled && TRUE == bChecked )
	{
		if( _T("Y") == m_strNightLoanReservationUseYn )
		{
			if( 0 == m_nSMSType || 3 == m_nSMSType )
			{
				if (m_str_SmsYearLength == _T("Y"))
					strMsg		+=	_T(",예약만기일:OO/OO/OO");
				else
					strMsg		+=	_T(",예약만기일:OOOO/OO/OO");
			}
			else
			{
				if (m_str_SmsYearLength == _T("Y"))
					strMsg		+=	_T(",반납예정일:OO/OO/OO");
				else
					strMsg		+=	_T(",반납예정일:OOOO/OO/OO");
			}
		}
		else
		{
			if( 2 == m_nSMSType )
			{
				if (m_str_SmsYearLength == _T("Y"))
					strMsg		+=	_T(",예약만기일:OO/OO/OO");
				else
					strMsg		+=	_T(",예약만기일:OOOO/OO/OO");
			}
			else
			{
				if (m_str_SmsYearLength == _T("Y"))
					strMsg		+=	_T(",반납예정일:OO/OO/OO");
				else
					strMsg		+=	_T(",반납예정일:OOOO/OO/OO");
			}
		}

		
		
		m_bUseMsg[3]	=	TRUE;
	}
	else
	{
		m_bUseMsg[3]	=	FALSE;
	}

	CEdit*	pEdtTitleSize = (CEdit*) GetDlgItem ( IDC_edtTitleSize );
	if( NULL == pEdtTitleSize || NULL == pEdtTitleSize->GetSafeHwnd() )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SetMsgPreview") );
	}

	bEnabled	=	pBtn[4]->IsWindowEnabled();
	bChecked	=	pBtn[4]->GetCheck();
	if( TRUE == bEnabled && TRUE == bChecked )
	{
		strMsg		+=	_T("(서명)");
		m_bUseMsg[4]	=	TRUE;

		DWORD dwStyle = pEdtTitleSize->GetStyle();
		dwStyle &= ES_READONLY;
		if( (dwStyle & ES_READONLY) == ES_READONLY )
		{
			pEdtTitleSize->SetReadOnly( FALSE );
		}
	}
	else
	{
		m_bUseMsg[4]	=	FALSE;

		DWORD dwStyle = pEdtTitleSize->GetStyle();
		dwStyle &= ES_READONLY;
		if( (dwStyle & ES_READONLY) != ES_READONLY)
		{
			pEdtTitleSize->SetReadOnly( TRUE );
		}
	}

	GetDlgItem( IDC_edtSMSContentsPreview )->SetWindowText( strMsg );

	return 0;

EFS_END
return -1;
}


void CSMSContentsEditDlg::OnbtnSMSContentsSend() 
{
EFS_BEGIN

	SMS_CONNECTION_INFO		SMS_ConnectionInfo;
	SMS_MSG_INFO	SMS_MsgInfo;
	CSMS_MakeMsg MakeSMS( this );
	GetSMSInfo( m_strSMSType , SMS_MsgInfo , SMS_ConnectionInfo );

	if( SMS_MsgInfo.CheckUseKatok() == 0 )
	{
		if( 8 > m_nTitleSize || 30 < m_nTitleSize )
		{
			AfxMessageBox(_T("서명 크기는 8~30 사이의 숫자만 입력하여 주십시요."), MB_ICONWARNING);
			CEdit*	pedtBookTitleSize	=	(CEdit*)GetDlgItem( IDC_edtTitleSize );
			pedtBookTitleSize->SetFocus();
			pedtBookTitleSize->SetSel( 0 , -1 );
			return;
		}
	}

	GetSMSType();
	if( 1 == m_nMenuMode )
	{
		SendSMS_01();
	}
	else if( 2 == m_nMenuMode )
	{
		SendSMS_02();
	}
	else if( 3 == m_nMenuMode )
	{
		SendSMS_03();
	}
	else if( 4 == m_nMenuMode )
	{
		SendSMS_04();
	}

EFS_END
}

INT CSMSContentsEditDlg::SendSMS_01()
{
EFS_BEGIN

	INT		ids;

	CComboBox*	pCmb	=	((CComboBox*)GetDlgItem( IDC_cSMS_TYPE ));
	pCmb->EnableWindow(FALSE);

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_01") );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SendSMS_01") );
	}

	INT		nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자가 없습니다.") , MB_ICONINFORMATION);
		return 1010;
	}

	CArray< INT, INT > aCheckedRow;
	ids		=	GetCheckedGridRow( _T("CM_SMS_DLG_01") , _T("GRID") , aCheckedRow );
	if( 0 != ids )
	{
		return 1020;
	}

	SMS_CONNECTION_INFO		SMS_ConnectionInfo;
	SMS_MSG_INFO	SMS_MsgInfo;
	
	CSMS_MakeMsg MakeSMS( this );
	GetSMSInfo( m_strSMSType , SMS_MsgInfo , SMS_ConnectionInfo );
	CSMS_Env_Info SMSDMSender( this );

	MakeSMS.m_nType_SendPlatform = SMS_MsgInfo.CheckUseKatok();

	SMSDMSender.InitSMSFieldData();
	CString		strDateAlias = _T("반납예정일");
	if( 3 == _ttoi( m_strSMSType.Right( 2 ) ) )
	{
		strDateAlias = _T("예약만기일");
	}

	CString			strUserKey , strLoanKey , strData , strName, strPhoneNum, strBookTitle, strMsg, strDate, strBookCnt , strSMSReceiptYN;
	CStringList		slName , slPhonNum , slContent , slUserKey , slLoanKey , slSMSReceiptYN;
	CStringArray	asPositionCode , asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo;

	CString			strContent_Katok;
	CStringArray	asContent_Katok;

	const INT		nExAliasCnt = 6;
	CString			sExAlias[nExAliasCnt] =
	{
		_T("소속"),		_T("직급"),	_T("회원상태코드"), 
		_T("이용자ID"), _T("생일"), _T("주민번호"),
	};

	INT		nRowCnt , nIdx , i , j;
	nRowCnt		=	aCheckedRow.GetSize();
	for( i=0 ; i<nRowCnt ; i++ )
	{
		nIdx = aCheckedRow.GetAt( i );

		strSMSReceiptYN	=	pDM->GetCellData( _T("SMS_USE_YN"), nIdx );
		
		strName = pDM->GetCellData( _T("대출자명"), nIdx );
		if( 2 == _ttoi( m_strSMSType.Right( 2 ) ) )
		{
			strBookCnt = pDM->GetCellData( _T("연체책수"), nIdx );
		}
		else if( 1 == _ttoi( m_strSMSType.Right( 2 ) ) )
		{
			strBookCnt = pDM->GetCellData( _T("대출책수"), nIdx );
		}
		else if( 3 == _ttoi( m_strSMSType.Right( 2 ) ) )
		{
			strBookCnt = _T("1");
		}
		strDate = pDM->GetCellData( strDateAlias , nIdx );
		strBookTitle = pDM->GetCellData( _T("서명"), nIdx );		
		strPhoneNum = pDM->GetCellData( _T("휴대폰"), nIdx );
		strPhoneNum.TrimLeft();		strPhoneNum.TrimRight();
		strPhoneNum.Replace( _T("-"), _T("") );
		strPhoneNum.Replace( _T(" "), _T("") );

		strUserKey	=	pDM->GetCellData( _T("USER_KEY"), nIdx );
		strLoanKey	=	pDM->GetCellData( _T("대출이력KEY"), nIdx );
		strSMSReceiptYN	=	pDM->GetCellData( _T("SMS_USE_YN"), nIdx );

		SetMsgInfo( SMS_MsgInfo , strName , strDate , strBookTitle );

		MakeSMS.MakeSMSMsg( m_strSMSType , &SMS_MsgInfo , strName , strDate , strBookTitle , strBookCnt , strMsg , &strContent_Katok );

		slName.AddTail( strName );
		slPhonNum.AddTail( strPhoneNum );
		slContent.AddTail( strMsg );
		slUserKey.AddTail( strUserKey );
		slLoanKey.AddTail( strLoanKey );
		slSMSReceiptYN.AddTail( strSMSReceiptYN );
		asContent_Katok.Add(strContent_Katok);

		for( j=0 ; j<nExAliasCnt ; j++ )
		{
			ids = pDM->GetCellData( sExAlias[j] , nIdx , strData );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SendSMS_01") );
			}

			switch(j)
			{
			case 0:
				asPositionCode.Add(strData);
				break;
			case 1:
				asClassCode.Add(strData);
				break;
			case 2:
				asInferiorClass.Add(strData);
				break;
			case 3:
				asUserID.Add(strData);
				break;
			case 4:
				asBirthDay.Add(strData);
				break;
			case 5:
				asCivilNo.Add(strData);
				break;
			}
		}
	}

	CSMSSendDlg dlg( this, SMS_ConnectionInfo, m_strSMSType , SMS_MsgInfo.sCallBackNumber, 
					slUserKey , slLoanKey , slName , slPhonNum , slContent , slSMSReceiptYN , 0 );
	dlg.SetInfoEx(  asPositionCode ,	asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo );

	dlg.m_asContent_Katok.Append( asContent_Katok );
	dlg.m_pSMS_MSG_INFO = &SMS_MsgInfo;
	dlg.DoModal();

	if( _T("NRL03") == m_strSMSType )
	{
		INT nIdx = dlg.m_gridIdx.GetSize();
		if( nIdx )
		{
			CString strLoanKey;
			for( i=0 ; i<nIdx ; i++ )
			{				
				pDM->GetCellData( _T("대출이력KEY"), dlg.m_gridIdx.GetAt(i)-1, strLoanKey );

				if( TRUE != strLoanKey.IsEmpty() )
				{
					pDM->StartFrame();
					pDM->InitDBFieldData();
					pDM->AddDBFieldData( _T("NIGHT_RESERVATION_SMS_SEND_YN"), _T("STRING"), _T("Y") );
					pDM->MakeUpdateFrame( _T("LS_WORK_T01"), _T("REC_KEY"), _T("NUMERIC"), strLoanKey );
					pDM->SendFrame();
					pDM->EndFrame();
				}
			}
		}
	}

	return 0;

EFS_END
return -1;
}

INT CSMSContentsEditDlg::SendSMS_02()
{
EFS_BEGIN
	
	INT		ids;
	
	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_02") );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SendSMS_02") );
	}

	INT		nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자가 없습니다.") , MB_ICONINFORMATION);
		return 1010;
	}

	CArray< INT, INT > aCheckedRow;
	ids		=	GetCheckedGridRow( _T("CM_SMS_DLG_02") , _T("GRID") , aCheckedRow );
	if( 0 != ids )
	{
		return 1020;
	}

	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;

	CSMS_MakeMsg MakeSMS( this );
 	GetSMSInfo( m_strSMSType , SMS_MsgInfo , SMS_ConnectionInfo );
	CSMS_Env_Info SMSDMSender( this );

	MakeSMS.m_nType_SendPlatform = SMS_MsgInfo.CheckUseKatok();

	SMSDMSender.InitSMSFieldData();
	
	CStringList		slName, slPhonNum, slContent, slUserKey, slLoanKey, slSMSReceiptYN;
	CString			strName, strPhoneNum, strDate , strBookTitle, strMsg, strUserKey, strSMSReceiptYN;

	CString			strContent_Katok;
	CStringArray	asContent_Katok;

	CString			strUserNo, strQuery;
	CESL_DataMgr	TmpDM;
	TmpDM.SetCONNECTION_INFO( pDM->CONNECTION_INFO );

	INT		nRowCnt , nIdx , i;
	nRowCnt		=	aCheckedRow.GetSize();
	for( i=0 ; i<nRowCnt ; i++ )
	{
		nIdx = aCheckedRow.GetAt( i );

		strSMSReceiptYN	=	pDM->GetCellData( _T("SMS_USE_YN"), nIdx );

		ids	=	pDM->GetCellData( _T("신청자") , nIdx , strName );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SendSMS_02") );
		}
	
		ids	=	pDM->GetCellData( _T("표제"), nIdx , strBookTitle );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SendSMS_02") );
		}

		ids =	pDM->GetCellData( _T("전화번호"), nIdx , strPhoneNum );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SendSMS_02") );
		}

		ids =	pDM->GetCellData( _T("대출자번호"), nIdx , strUserNo );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SendSMS_02") );
		}
		strQuery.Format( _T("SELECT REC_KEY FROM CO_LOAN_USER_TBL ")
						 _T("WHERE USER_NO='%s' "), strUserNo);
		TmpDM.GetOneValueQuery(strQuery, strUserKey);

		strPhoneNum.TrimLeft();		strPhoneNum.TrimRight();
		strPhoneNum.Replace( _T("-"), _T("") );
		strPhoneNum.Replace( _T(" "), _T("") );
		
		SetMsgInfo( SMS_MsgInfo , strName , strDate , strBookTitle );

		MakeSMS.MakeSMSMsg( m_strSMSType , &SMS_MsgInfo , strName , _T("") , strBookTitle , _T("") , strMsg , &strContent_Katok );
		
		slName.AddTail( strName );
		slPhonNum.AddTail( strPhoneNum );
		slContent.AddTail( strMsg );

		strSMSReceiptYN	=	pDM->GetCellData( _T("SMS_USE_YN"), nIdx );
		slUserKey.AddTail( strUserKey );
		slLoanKey.AddTail( _T("0") );
		slSMSReceiptYN.AddTail( strSMSReceiptYN );

		asContent_Katok.Add(strContent_Katok);

	}

	CSMSSendDlg dlg( this , SMS_ConnectionInfo, m_strSMSType , SMS_MsgInfo.sCallBackNumber, 
					slUserKey , slLoanKey , slName , slPhonNum , slContent , slSMSReceiptYN , 0 );
	dlg.m_asContent_Katok.Append( asContent_Katok );
	dlg.m_pSMS_MSG_INFO = &SMS_MsgInfo;
	dlg.DoModal();

	return 0;

EFS_END
return -1;
}

INT CSMSContentsEditDlg::SendSMS_03()
{
EFS_BEGIN

	INT		ids;

	CESL_DataMgr *pDM = FindDM( _T("DM_SMS_DLG_03_USER") );
	if( NULL == pDM )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SendSMS_03") );
	}
	
	INT	nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자가 없습니다.") , MB_ICONINFORMATION );
		return 1010;
	}

	CArray< INT, INT > aCheckedRow;
	ids		=	GetCheckedGridRow( _T("CM_SMS_DLG_03") , _T("GRID_USER") , aCheckedRow );
	if( 0 != ids )
	{
		return 1020;
	}
	
	CString		strBookTitle;
	ids		=	GetBookTitle( strBookTitle );
	if( 0 != ids )
	{
		return 1030;
	}

	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;

	CSMS_MakeMsg MakeSMS( this );
 	GetSMSInfo( m_strSMSType , SMS_MsgInfo , SMS_ConnectionInfo );
	CSMS_Env_Info SMSDMSender( this );

	MakeSMS.m_nType_SendPlatform = SMS_MsgInfo.CheckUseKatok();

	// 5. SMS 내용 구성
	SMSDMSender.InitSMSFieldData();

	CString			strName, strPhoneNum, strTitle, strMsg, strDate, strUserKey, strLoanKey, strSMSReceiptYN;
	CStringList		slName, slPhonNum, slContent, slUserKey, slLoanKey, slSMSReceiptYN;
	CStringArray	asPositionCode,asClassCode,asInferiorClass,asUserID,asBirthDay,asCivilNo;

	CString			strContent_Katok;
	CStringArray	asContent_Katok;


	const INT nExAliasCnt = 6;
	CString sExAlias[nExAliasCnt] =
	{
		_T("소속") , _T("직급") , _T("회원상태코드") , _T("이용자ID") , _T("생일") , _T("주민번호")
	};

	CString		strData;
	INT			nRowCnt , nIdx , i , j;
	nRowCnt		=	aCheckedRow.GetSize();
	for( i=0 ; i<nRowCnt ; i++ )
	{
		nIdx = aCheckedRow.GetAt( i );

		strSMSReceiptYN	=	pDM->GetCellData( _T("SMS_USE_YN"), nIdx );

		ids		=	pDM->GetCellData( _T("대출자명") , nIdx , strName );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SendSMS_02") );
		}

		ids		=	pDM->GetCellData( _T("휴대폰") , nIdx , strPhoneNum );
		if( 0 > ids )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SendSMS_02") );
		}

		strPhoneNum.TrimLeft();		strPhoneNum.TrimRight();
		strPhoneNum.Replace( _T("-"), _T("") );
		strPhoneNum.Replace( _T(" "), _T("") );

		SetMsgInfo( SMS_MsgInfo , strName , strDate , strBookTitle );
		
		MakeSMS.MakeSMSMsg( m_strSMSType , &SMS_MsgInfo , strName , _T("") , strBookTitle , _T("") , strMsg , &strContent_Katok );

		slName.AddTail( strName );
		slPhonNum.AddTail( strPhoneNum );
		slContent.AddTail( strMsg );

		strUserKey		= pDM->GetCellData( _T("USER_KEY") , nIdx );
		strSMSReceiptYN	= pDM->GetCellData( _T("SMS_USE_YN") , nIdx );
		slUserKey.AddTail( strUserKey );
		slLoanKey.AddTail( _T("0") );
		slSMSReceiptYN.AddTail( strSMSReceiptYN );

		asContent_Katok.Add(strContent_Katok);

		for( j=0 ; j<nExAliasCnt ; j++ )
		{
			ids = pDM->GetCellData( sExAlias[j] , nIdx , strData );
			if( 0 > ids ) 
			{
				AfxMessageBox( _T("ERROR") );
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SendSMS_03") );
			}

			switch(j)
			{
				case 0:
					asPositionCode.Add(strData);
					break;
				case 1:
					asClassCode.Add(strData);
					break;
				case 2:
					asInferiorClass.Add(strData);
					break;
				case 3:
					asUserID.Add(strData);
					break;
				case 4:
					asBirthDay.Add(strData);
					break;
				case 5:
					asCivilNo.Add(strData);
					break;
			}
		}
	}
	
	CSMSSendDlg dlg( this, SMS_ConnectionInfo, m_strSMSType , SMS_MsgInfo.sCallBackNumber, 
					slUserKey , slLoanKey , slName , slPhonNum , slContent , slSMSReceiptYN , 0 );
	dlg.SetInfoEx(  asPositionCode ,	asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo );
	dlg.m_asContent_Katok.Append( asContent_Katok );
	dlg.m_pSMS_MSG_INFO = &SMS_MsgInfo;
	dlg.DoModal();		
	
	return 0;

EFS_END
return -1;
}

INT CSMSContentsEditDlg::SendSMS_04()
{
EFS_BEGIN

	INT ids;

	CESL_DataMgr* pDM = FindDM(_T("DM_SMS_4100"));
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SendSMS_04") );
	}

	INT	nDMRowCnt	=	pDM->GetRowCount();
	if( 1 > nDMRowCnt )
	{
		AfxMessageBox( _T("문자 발송 대상자가 없습니다.") , MB_ICONINFORMATION );
		return 1010;
	}

	CArray< INT, INT > aCheckedRow;
	ids		=	GetCheckedGridRow( _T("CM_SMS_4100") , _T("검색된이용자그리드") , aCheckedRow );
	if( 0 != ids )
	{
		return 1020;
	}

	SMS_CONNECTION_INFO SMS_ConnectionInfo;
	SMS_MSG_INFO SMS_MsgInfo;
	
	CSMS_MakeMsg MakeSMS( this );
 	GetSMSInfo( m_strSMSType , SMS_MsgInfo , SMS_ConnectionInfo );
	CSMS_Env_Info SMSDMSender( this );
 
	MakeSMS.m_nType_SendPlatform = SMS_MsgInfo.CheckUseKatok();

	CString			strUserName , strPhoneNum , strMsg , strDate , strUserKey , strBookTitle , strData , strSMSReceiptYN;
	CStringList		slName , slPhonNum , slContent , slUserKey , slLoanKey, slSMSReceiptYN;
	CStringArray	asPositionCode , asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo;

	CString			strContent_Katok;
	CStringArray	asContent_Katok;

	const INT nExAliasCnt = 6;
	CString sExAlias[nExAliasCnt] =
	{
		_T("대출소속정보"),	_T("대출직급정보"),	_T("회원상태코드") , 
		_T("이용자ID"),		_T("생일"),			_T("주민등록번호")
	};
	
	INT		i , j , nRowCnt , nIdx;
	nRowCnt		=	aCheckedRow.GetSize();
	for( i=0 ; i<nRowCnt ; i++ )
	{
		nIdx = aCheckedRow.GetAt( i );

		strSMSReceiptYN	=	pDM->GetCellData( _T("SMS_USE_YN"), nIdx );

		ids = pDM->GetCellData( _T("이용자명") , nIdx , strUserName );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SendSMSFromTempDM") );
		}

		ids = pDM->GetCellData( _T("휴대폰번호") , nIdx , strPhoneNum );
		if( 0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SendSMSFromTempDM") );
		}

		strPhoneNum.TrimLeft();		strPhoneNum.TrimRight();
		strPhoneNum.Replace( _T("-"), _T("") );
		strPhoneNum.Replace( _T(" "), _T("") );
		
		SetMsgInfo( SMS_MsgInfo , strUserName , strDate , strBookTitle );		

		MakeSMS.MakeSMSMsg( m_strSMSType , &SMS_MsgInfo , strUserName , _T("") , _T("") , _T("0") , strMsg , &strContent_Katok );

		slName.AddTail( strUserName );
		slPhonNum.AddTail( strPhoneNum );
		slContent.AddTail( strMsg );
		
		ids = pDM->GetCellData( _T("이용자KEY"), nIdx , strUserKey );
		if( 0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SendSMSFromTempDM") );
		}

		ids = pDM->GetCellData( _T("SMS_USE_YN"), nIdx , strSMSReceiptYN );
		if( 0 > ids)
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SendSMSFromTempDM") );
		}

		slUserKey.AddTail( strUserKey );
		slLoanKey.AddTail( _T("0") );
		slSMSReceiptYN.AddTail( strSMSReceiptYN );

		asContent_Katok.Add(strContent_Katok);

		for( j=0 ; j<nExAliasCnt ; j++ )
		{
			ids = pDM->GetCellData( sExAlias[j] , nIdx , strData );
			if( 0 > ids ) 
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SendSMSFromTempDM") );
			}

			switch(j)
			{
			case 0:
				asPositionCode.Add( strData );
				break;
			case 1:
				asClassCode.Add( strData );
				break;
			case 2:
				asInferiorClass.Add( strData );
				break;
			case 3:
				asUserID.Add( strData );
				break;
			case 4:
				asBirthDay.Add( strData );
				break;
			case 5:
				asCivilNo.Add( strData );
				break;
			}
		}
	}

	CSMSSendDlg dlg( this, SMS_ConnectionInfo, m_strSMSType , SMS_MsgInfo.sCallBackNumber, 
					slUserKey , slLoanKey , slName , slPhonNum , slContent , slSMSReceiptYN , 0 );
	dlg.SetInfoEx(  asPositionCode ,	asClassCode , asInferiorClass , asUserID , asBirthDay , asCivilNo );
	dlg.m_asContent_Katok.Append( asContent_Katok );
	dlg.m_pSMS_MSG_INFO = &SMS_MsgInfo;
	dlg.DoModal();		

	return 0;

EFS_END
return -1;
}

INT CSMSContentsEditDlg::GetCheckedGridRow(  CString strCMAlias , CString strCtrlAlias , CArray< INT, INT > &aCheckedRow )
{
EFS_BEGIN

	CESL_Grid*		pGrid	=	(CESL_Grid*)FindControl( strCMAlias , strCtrlAlias );
	if( NULL == pGrid || NULL == pGrid->GetSafeHwnd() )
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetCheckedGridRow") );
	}
	
	INT		i , nRowCnt;
	nRowCnt		=	pGrid->GetRows();
	if( 1 > nRowCnt )
	{
		AfxMessageBox( _T("검색 된 SMS 발송 대상자가 없습니다.") , MB_ICONINFORMATION);
		return 1010;
	}

	aCheckedRow.RemoveAll();
	if( 1 == ((CButton*)GetDlgItem( IDC_radSMSContentsSelect ))->GetCheck() )
	{
		for( i=1; i<nRowCnt ; i++ )
		{
			if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
			{
				aCheckedRow.Add( i - 1 );	
			}
		}

		INT		nArrayCnt;
		nArrayCnt	=	aCheckedRow.GetSize();
		if( 1 > nArrayCnt )
		{
			AfxMessageBox( _T("SMS 발송 대상자를 선택하여 주십시요.") , MB_ICONINFORMATION);
			return 1020;
		}
	}
	else
	{
		for( i=1; i<nRowCnt ; i++ )
		{
			aCheckedRow.Add( i - 1 );
		}		
	}
	
	return 0;
	
EFS_END
return -1;
}

VOID CSMSContentsEditDlg::SetSearchType( INT nSMSType )
{
	m_nSMSType	=	nSMSType;
	CComboBox*	pCmb	=	((CComboBox*)GetDlgItem( IDC_cSMS_TYPE ));
	pCmb->SetCurSel( m_nSMSType );

	ViewReceiverNumEdt();

	SetMsgPreview();
	ViewSMSContent();
}

VOID CSMSContentsEditDlg::SetMsgInfo( SMS_MSG_INFO &SMS_MsgInfo , CString &strName , CString &strDate , CString &strBookTitle )
{
	CString		strComponentInfo;
	
	if( TRUE == m_bUseMsg[1] )
	{
		strComponentInfo	=	_T("1");
	}
	else
	{
		strComponentInfo	=	_T("0");
	}
	
	if( TRUE == m_bUseMsg[2] )
	{
		strComponentInfo	+=	_T("1");
	}
	else
	{
		strComponentInfo	+=	_T("0");
	}

	if( TRUE == m_bUseMsg[3] )
	{
		strComponentInfo	+=	_T("1");
	}
	else
	{
		strDate =	_T("");
		strComponentInfo	+=	_T("0");
	}

	if( TRUE == m_bUseMsg[4] )
	{
		strComponentInfo	+=	_T("1");
	}
	else
	{
		strComponentInfo	+=	_T("0");
	}

	if( TRUE == m_bUseMsg[0] )
	{
		SMS_MsgInfo.sLibName	=	m_strLibName;
		strComponentInfo	+=	_T("1");
	}
	else
	{
		strComponentInfo	+=	_T("0");
	}
	
	SMS_MsgInfo.sMsgComponentInfo	=	strComponentInfo;

	SMS_MsgInfo.nTitleSize			=	m_nTitleSize;
}

INT CSMSContentsEditDlg::GetBookTitle(CString &strBookTitle)
{
EFS_BEGIN

	CMSHFlexGrid *pGrid = NULL;
	CESL_DataMgr *pDM = NULL;

	if( 0 == m_nSMS03PublishFormCode )
	{
		pGrid	=	(CMSHFlexGrid*)FindControl( _T("CM_SMS_DLG_03") , _T("GRID_BO_BOOK") );
		if( NULL == pGrid && NULL == pGrid->GetSafeHwnd() )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("GetBookTitle") );
		}

		pDM		=	FindDM( _T("DM_SMS_DLG_03_BO_BOOK") );
		if( NULL == pDM )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("GetBookTitle") );
		}
	}
	else if( 1 == m_nSMS03PublishFormCode )
	{
		pGrid	=	(CMSHFlexGrid*)FindControl( _T("CM_SMS_DLG_03") , _T("GRID_SE_BOOK") );
		if( NULL == pGrid && NULL == pGrid->GetSafeHwnd() )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("GetBookTitle") );
		}

		pDM = FindDM( _T("DM_SMS_DLG_03_SE_BOOK") );
		if( NULL == pDM )
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("GetBookTitle") );
		}
	}
	else
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("GetBookTitle") );
	}
	
	INT		i , nRowCnt;
	nRowCnt		=	pGrid->GetRows();
	for( i=1 ; i<nRowCnt; i++ )
	{
		if( _T("V") == pGrid->GetTextMatrix( i, 1 ) )
		{
			pDM->GetCellData( _T("표제") , i-1 , strBookTitle );
			return 0;
		}
	}

	AfxMessageBox( _T("선정 된 자료가 없습니다.") , MB_ICONINFORMATION );
	return 1020;

EFS_END
return -1;
}

VOID CSMSContentsEditDlg::GetSMSType()
{
EFS_BEGIN

	CComboBox*	pCmb	=	((CComboBox*)GetDlgItem( IDC_cSMS_TYPE ));
	INT			nCurSel	=	pCmb->GetCurSel();
	POSITION	pos		=	m_SMSList.FindIndex( nCurSel );
	m_strSMSType		=	m_SMSList.GetAt( pos );

EFS_END
}

void CSMSContentsEditDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(TRUE == bShow)
	{
		SearchSMSDefaulgInfo();
		ViewSMSComposition();
		ViewCallbackNum();
		ViewLibName();
		ViewTitleSize();
	}
}
