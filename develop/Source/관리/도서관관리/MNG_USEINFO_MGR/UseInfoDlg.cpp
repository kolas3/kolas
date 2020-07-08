// UseInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UseInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUseInfoDlg dialog

CUseInfoDlg::CUseInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CUseInfoDlg)
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_pCM = NULL;
	m_bInsert = TRUE;
	AfxInitRichEdit();
	m_HtmlEditor = NULL;

	if ( FAILED(CoInitialize(NULL) ) )
	{
		AfxAbort();
	}
}

CUseInfoDlg::~CUseInfoDlg()
{
	if ( m_HtmlEditor != NULL )
	{
		delete m_HtmlEditor;
	}
	CoUninitialize();
}

VOID CUseInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUseInfoDlg)
	DDX_Control(pDX, IDC_TGroup2, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUseInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CUseInfoDlg)
	ON_CBN_SELCHANGE(IDC_cManage, OnSelchangecManage)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TGroup2, OnSelchangeTGroup2)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()     
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUseInfoDlg message handlers

BOOL CUseInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_MNG_USEINFO_MGR")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Failed!!")); 
	}
	m_pCM = FindCM(_T("CM_MNG_USEINFO_MGR"));
	m_pDM = FindDM(_T("DM_MNG_USEINFO_MGR"));
	m_ctrlTab.InsertItem(0, _T("공지사항"));
	m_ctrlTab.InsertItem(1, _T("이용안내"));
	InitManage();


	CRect rect;

	m_HtmlEditor = new CDlgHtmlEdit;
	CreateHtmlEditor(m_HtmlEditor, this);
	m_HtmlEditor->ShowWindow(true);	
	SetTimer(1, 100, NULL);
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CUseInfoDlg::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd); 
}

VOID CUseInfoDlg::OnModify()
{
	INT type = m_ctrlTab.GetCurSel();
	// 0 : 공지사항
	// 1 : 이용안내

	INT inputMode = m_pDM->GetRowCount() ? 0 : 1;
	// 0 : UPDATE
	// 1 : INSERT

	const INT cnt = 4;
	CString alias[ cnt ] = {
		_T("ANNOUNCEMENT"),						// 공지사항
		_T("ANN_UPDATE_DATE"),					// 공지사항 수정일자
		_T("FIRST_WORK"),						// 최초작업
		_T("MANAGE_CODE") };					// 관리구분
	if( type )
	{
		alias[ 0 ] = _T("GUIDE");				// 이용안내
		alias[ 1 ] = _T("GUI_UPDATE_DATE");		// 이용안내 수정일자
	}
	if( inputMode )
		alias[ 2 ] = _T("LAST_WORK");

	CString data[ cnt ];
	// m_pCM->GetControlMgrData( _T("정보내용"), data[ 0 ] );
	data[0] = m_HtmlEditor->GetHtml();	

	m_pCM->GetControlMgrData( _T("등록일"),   data[ 1 ] );
	data[ 2 ] = GetWorkLogMsg( _T("도서관이용안내"), __WFILE__, __LINE__ );
	m_pCM->GetControlMgrData( _T("관리구분"), data[ 3 ] );// 배열에 잘못된 숫자가 들어가 변경해줌(M_2004-1006_HAN...)

	m_pDM->StartFrame();
	m_pDM->InitDBFieldData();

	CString recKey;
	m_pDM->MakeRecKey( recKey );
/*
	for( INT i = 0; i < cnt; i++ )
		if( !data[ i ].IsEmpty() )
			m_pDM->AddDBFieldData( alias[ i ], _T("STRING"), data[ i ], true );
*/
	// 2003년 12월 23일 조병걸 수정
	for( INT i = 0; i < cnt; i++ )
		if( !data[ i ].IsEmpty() )
		{
			m_pDM->AddDBFieldData( alias[ i ], _T("STRING"), data[ i ], true );
			m_pDM->SetCellData( alias[i] , data[i] , 0 );
		}


	if( inputMode )
	{
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), recKey );
		m_pDM->MakeInsertFrame( _T("MN_LIB_GUIDANCE_TBL") );

		m_pDM->SetCellData( alias, cnt, data, -1 );
		m_pDM->SetCellData( _T("REC_KEY"), recKey, 0 );
	}
	else
		m_pDM->MakeUpdateFrame( _T("MN_LIB_GUIDANCE_TBL"), _T("REC_KEY"), _T("NUMERIC"), m_pDM->GetCellData( _T("REC_KEY"), 0 ) );
	m_pDM->SendFrame();
	m_pDM->EndFrame();

	if( inputMode )
		AfxMessageBox( _T("저장되었습니다.") );
	else
		AfxMessageBox( _T("수정되었습니다.") );
	


	/*
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("관리구분"));
	if (pCMB == NULL) return;

	CArray<CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();

	CString strManage;
//	CString strDate;
	CString strInfo;
	CString query;
	CString strRecKey;
	m_pCM->GetControlMgrData(_T("정보내용"), strInfo);
	if(strInfo.IsEmpty())
	{
		ESLAfxMessageBox(_T("내용을 입력해 주십시요."));
		return ;
	}
	

	if(m_pDM->GetRowCount() == 0)
		m_bInsert = TRUE;
	else 
		m_bInsert = FALSE;


	INT num = m_ctrlTab.GetCurSel(); 
	CTime time = m_pDM->GetDBTime();
	CString date;
	date.Format( _T("%04d/%02d/%02d"), time.GetYear(), time.GetMonth(), time.GetDay() );
	CString workLog = GetWorkLogMsg( _T("도서관이용안내"), __WFILE__, __LINE__ );
	if (num == 0)
	{	
		m_pCM->GetControlMgrData(_T("관리구분"), strManage);
//		m_pCM->GetControlMgrData(_T("등록일"), strDate);
		m_pCM->GetControlMgrData(_T("정보내용"), strInfo);
	
		// 데이터가 없을 경우 입력하는 부분. 	m_bInsert는 pDM->GetRowCount() = 0 일경우 TRUE	
		if(m_bInsert)   
		{
			m_pDM->InsertRow(-1);
			m_pDM->MakeRecKey(strRecKey);
			m_pDM->SetCellData(_T("REC_KEY"), strRecKey, 0);
			m_pDM->SetCellData(_T("FIRST_WORK"), this->GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), 0);
		}
		else
			strRecKey = m_pDM->GetCellData(_T("REC_KEY"), 0);
		m_pDM->SetCellData(_T("MANAGE_CODE"), strManage, 0);
//		m_pDM->SetCellData(_T("ANN_UPDATE_DATE"), strDate, 0);
		m_pDM->SetCellData(_T("ANNOUNCEMENT"), strInfo, 0);
		m_pDM->SetCellData(_T("LAST_WORK"), this->GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), 0);

		m_pDM->StartFrame(); 
				
		m_pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), strManage );
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strInfo );
		m_pDM->AddDBFieldData( _T("ANNOUNCEMENT"), _T("STRING"), strInfo );
		m_pDM->AddDBFieldData( _T("ANN_UPDATE_DATE"), _T("STRING"), date );

		if( m_bInsert )
		{
			m_pDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			m_pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), workLog );
			m_pDM->MakeInsertFrame( _T("MN_LIB_GUIDANCE_TBL") );
		}
		else
		{
			m_pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), workLog );
			m_pDM->MakeUpdateFrame( _T("MN_LIB_GUIDANCE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
		}
		
		
		//if(m_bInsert)
			//m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
		//else
			//m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);
		
	}
	else if (num == 1)
	{
		m_pCM->GetControlMgrData(_T("관리구분"), strManage);
//		m_pCM->GetControlMgrData(_T("등록일"), strDate);
		m_pCM->GetControlMgrData(_T("정보내용"), strInfo);
		
        // 데이터가 없을 경우 입력하는 부분 
		if(m_bInsert)
		{
			m_pDM->InsertRow(-1);
			m_pDM->MakeRecKey(strRecKey);
			m_pDM->SetCellData(_T("REC_KEY"), strRecKey, 0);
			m_pDM->SetCellData(_T("FIRST_WORK"), this->GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), 0);
		}
		else 
			strRecKey = m_pDM->GetCellData(_T("REC_KEY"), 0);
		m_pDM->SetCellData(_T("MANAGE_CODE"), strManage, 0);
//		m_pDM->SetCellData(_T("GUI_UPDATE_DATE"), strDate, 0);
		m_pDM->SetCellData(_T("GUIDE"), strInfo, 0);
		m_pDM->SetCellData(_T("LAST_WORK"), this->GetWorkLogMsg( _T("도서관관리"), __WFILE__, __LINE__ ), 0);

		m_pDM->StartFrame(); 
		
		m_pDM->AddDBFieldData( _T("MANAGE_CODE"), _T("STRING"), strManage );
		m_pDM->AddDBFieldData( _T("REC_KEY"), _T("STRING"), strInfo );
		m_pDM->AddDBFieldData( _T("GUIDE"), _T("STRING"), strInfo );
		m_pDM->AddDBFieldData( _T("GUI_UPDATE_DATE"), _T("STRING"), date );

		if( m_bInsert )
		{
			m_pDM->AddDBFieldData( _T("REC_KEY"), _T("NUMERIC"), _T("ESL_SEQ.NEXTVAL") );
			m_pDM->AddDBFieldData( _T("FIRST_WORK"), _T("STRING"), workLog );
			m_pDM->MakeInsertFrame( _T("MN_LIB_GUIDANCE_TBL") );
		}
		else
		{
			m_pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), workLog );
			m_pDM->MakeUpdateFrame( _T("MN_LIB_GUIDANCE_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey );
		}
		
		
		//if(m_bInsert)
			//m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM);
		//else
			//m_pDM->MakeQueryByDM(RemoveAliasList, m_pDM, m_pDM->TBL_NAME, 0, m_pDM, _T("REC_KEY"), _T("NUMERIC"), strRecKey);	
		
	}

	INT ids = m_pDM->SendFrame();
	if(ids < 0) 
		ESLAfxMessageBox(_T("변경내용을 저장할 수 없습니다."));
	ESLAfxMessageBox(_T("변경내용을 저장하였습니다."));
	return;
	Display();
	*/
}

VOID CUseInfoDlg::OnSelchangecManage() 
{
	CESL_ControlMultiComboBox *pCMB = (CESL_ControlMultiComboBox *)m_pCM->FindControl(_T("관리구분"));
	if (pCMB == NULL) return;

	CString strManage;
	m_pCM->GetControlMgrData(_T("관리구분"), strManage);
	if (strManage.IsEmpty()) 
	{
		pCMB->SetCurSel(m_nMANAGE_CODE);
		AfxMessageBox(_T("[관리구분별도서관설정]이 'Y'일때는 적용안함은 설정할 수 없습니다."));

		m_pCM->SetControlMgrData(_T("관리구분"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
//		return;
	}
	CString strManageCode;
	m_pCM->GetControlMgrData(_T("관리구분"), strManageCode);
	RefreshDM(strManageCode);
	Display();
}

VOID CUseInfoDlg::InitManage()
{
	//++2008.10.22 UPDATE BY PWR {{++
	// 통합시스템 적용
	m_cUseMANAGE_CODE = 'Y';
	m_strUSER_MANAGE_CODE = m_pInfo->MANAGE_CODE;
//	CString strTmp, strQuery;
//	strTmp = _T("");
//	strQuery.Format(_T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' AND CLASS_ALIAS='관리구분별도서관설정' "));
//	m_pDM->GetOneValueQuery(strQuery, strTmp);
//	strTmp.TrimLeft();				strTmp.TrimRight();
//	strTmp.MakeUpper();
//	if (strTmp == _T("N")) 
//		m_cUseMANAGE_CODE = 'N';
//	else 
//		m_cUseMANAGE_CODE = 'Y';   // m_cUseMANAGE_CODE : 관리구분별도서관설정 Y/N
//
//	strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE ID='%s'"), GetUserID());
//	m_pDM->GetOneValueQuery(strQuery, m_strUSER_MANAGE_CODE);    // m_strUSER_MANAGE_CODE : 관리구분별도서관 
//	m_strUSER_MANAGE_CODE.TrimLeft();			m_strUSER_MANAGE_CODE.TrimRight();
	//--2008.10.22 UPDATE BY PWR --}}

	// 관리구분을 가져온다.
	CString strManageCode;
	m_pCM->GetControlMgrData(_T("관리구분"), strManageCode);   // strManageCode : 관리구분별도서관기호
	RefreshDM(strManageCode);
	Display();	
}  

VOID CUseInfoDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	CStatic *pStatic;

	pStatic = ( CStatic * )GetDlgItem( IDC_SGroup2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 9, 3, cx- 16, 45 );

	pStatic = ( CStatic * )GetDlgItem( IDC_TGroup2 );
	if( NULL == pStatic->GetSafeHwnd() )
		return;
	else
		pStatic->MoveWindow( 9, 53, cx - 16, cy - 60);

	CRect rect;

	rect.left = 17;
	rect.top = 80;
	rect.right = cx-17;
	rect.bottom = cy-17;
	
	if ( m_HtmlEditor->GetSafeHwnd() != NULL ) m_HtmlEditor->MoveWindow(rect);	
}

INT CUseInfoDlg::RefreshDM(CString strManageCode)
{
	CString strWhere;
	if(m_cUseMANAGE_CODE == _T("Y"))
	{
		strWhere.Format(_T("MANAGE_CODE = '%s'"), strManageCode);
	}
	else
	{
		strWhere.Format(_T("MANAGE_CODE IS NULL"));
	}

	if(m_pDM == NULL) return -1;
	INT ids = m_pDM->RefreshDataManager(strWhere);
	if(ids < 0) return -1;
	
	CString strCheck;
//	m_pDM->GetCellData(_T("정보내용"), strCheck);
//	if (strCheck.IsEmpty())
	if(m_pDM->GetRowCount() == 0)
		m_bInsert = TRUE;
	else 
		m_bInsert = FALSE;
	return 0;
}

VOID CUseInfoDlg::Display()
{	
	if ( m_HtmlEditor == NULL ) return;

	if(m_cUseMANAGE_CODE == _T("Y"))
		GetDlgItem(IDC_cManage)->EnableWindow(TRUE);
	else 
	{
		GetDlgItem(IDC_cManage)->EnableWindow(FALSE);
		m_pCM->SetControlMgrData(_T("관리구분"), _T(""));
	}
	CString strDate; 
	CTime t = m_pDM->GetDBTime();
	strDate.Format(_T("%04d/%02d/%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	CString strHtml;

	INT nCurSel = m_ctrlTab.GetCurSel();
	if(m_bInsert) 
	{
		if(nCurSel == 0)
		{
			//m_pCM->SetControlMgrData(_T("정보내용"), m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0));
			strHtml = m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0);
			if ( m_HtmlEditor->GetSafeHwnd() != NULL )
			{
				if(_T("") == strHtml) strHtml = _T("<HTML></HTML>");
				m_HtmlEditor->SetHtml(strHtml);
			}
			m_pCM->SetControlMgrData(_T("등록일"), strDate, 0);			
		}
		else
		{
			//m_pCM->SetControlMgrData(_T("정보내용"), m_pDM->GetCellData(_T("GUIDE"), 0));
			strHtml = m_pDM->GetCellData(_T("GUIDE"), 0);
			if ( m_HtmlEditor->GetSafeHwnd() != NULL )
			{
				if(_T("") == strHtml) strHtml = _T("<HTML></HTML>");
				m_HtmlEditor->SetHtml(strHtml);
			}
			m_pCM->SetControlMgrData(_T("등록일"), strDate, 0);			
		}
		return;
	}
	if(nCurSel == 0)
	{
		//m_pCM->SetControlMgrData(_T("정보내용"), m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0));
		strHtml = m_pDM->GetCellData(_T("ANNOUNCEMENT"), 0);
		if ( m_HtmlEditor->GetSafeHwnd() != NULL )
		{
			if(_T("") == strHtml) strHtml = _T("<HTML></HTML>");
			m_HtmlEditor->SetHtml(strHtml);
		}
		m_pCM->SetControlMgrData(_T("등록일"), strDate, 0);
		m_pCM->SetControlMgrData(_T("관리구분"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
	}
	else
	{
		//m_pCM->SetControlMgrData(_T("정보내용"), m_pDM->GetCellData(_T("GUIDE"), 0));
		strHtml = m_pDM->GetCellData(_T("GUIDE"), 0);
		if ( m_HtmlEditor->GetSafeHwnd() != NULL )
		{
			if(_T("") == strHtml) strHtml = _T("<HTML></HTML>");
			m_HtmlEditor->SetHtml(strHtml);
		}
		m_pCM->SetControlMgrData(_T("등록일"), strDate, 0);
		m_pCM->SetControlMgrData(_T("관리구분"), m_pDM->GetCellData(_T("MANAGE_CODE"), 0));
	}
}	

VOID CUseInfoDlg::OnSelchangeTGroup2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	Display();
	*pResult = 0;
}


void CUseInfoDlg::OnInsertObject()
{
	BeginWaitCursor();	
		// Word document를 위한 클래스 ID를 얻는다.
		// 생성할때 사용된다. 

		//if(FAILED(::CLSIDFromProgID(OLESTR("ecom.HtmlEditorControl"),&clsid)))        		

		/*
		// Word 가 삽입된 item 생성하기
		if(!pItem->CreateNewItem(clsid))
		AfxThrowMemoryException(); // 어떤 예외가 발생하면 TRY 문을 탈출하기만 하면 된다.

		// 새로운 CContainerItem이 유효한 것을 확인하라.
		ASSERT_VALID(pItem);

		// 아이템을 수정하기 위해 서버를 실행한다.
		pItem->DoVerb(OLEIVERB_SHOW, this);

		// 임의의 UI 다자인 처럼 마지막으로 삽입된 아이템을 선택하도록 설정한다.
		m_pSelection = pItem;  // 마지막으로 삽입된 아이템을 선택하도록 설정한다.

		pDoc->UpdateAllViews(NULL);

		// embedded object에 대한 포인터를 diapatch하는 질의를 한다. 이경우, Word
		// document이다.
		LPDISPATCH lpDisp;
		lpDisp = pItem->GetDispatch();
		
		// lpDisp를 사용하도록 _Document doc를 설정한다, 실제 document의 IDispatch*		
		m_html.AttachDispatch(lpDisp);
		m_html.setHtmlDoc(_T("Test html...."));	
		m_html.SetLeft(100);
		m_html.SetTop(100);
		m_html.SetWidth(400);
		m_html.SetHeight(400);		
	}  
	CATCH(CException, e)// 예외가 발생하면 clean up을 해준다.
	{
		if (pItem != NULL) {
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH
	*/

	// 작업이 끝났다는 것을 사용자가 알 수 있도록 커서를 보통 상태로 복구한다
	EndWaitCursor();
}

void CUseInfoDlg::OnTimer(UINT nIDEvent) 
{
	if ( nIDEvent == 1 )
	{
		KillTimer(1);
		Display();
	}
	
	CDialog::OnTimer(nIDEvent);
}

HBRUSH CUseInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

