// RfidTagView.cpp : implementation file
//

#include "stdafx.h"
#include "RfidTagView.h"

#include "EFS.h"

#include "RFID_Change.h"

#include "CharConvert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRfidTagView dialog


CRfidTagView::CRfidTagView(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD , pParent)
{
	//{{AFX_DATA_INIT(CRfidTagView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bSecurityMode = false;
}
CRfidTagView::~CRfidTagView()
{
}
VOID CRfidTagView::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRfidTagView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CRfidTagView, CDialog)
	//{{AFX_MSG_MAP(CRfidTagView)
	ON_BN_CLICKED(IDC_btnRICLOSE, OnbtnRICLOSE)
	ON_BN_CLICKED(IDC_btnRIREFRESH, OnbtnRIREFRESH)
	ON_BN_CLICKED(IDC_btnSET_SECURITY_MOD, OnbtnSETSECURITYMOD)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_tabTAGINFO, OnSelchangetabTAGINFO)
	ON_BN_CLICKED(IDC_btnSET_SECURITY_ON, OnbtnSETSECURITYON)
	ON_BN_CLICKED(IDC_btnSET_SECURITY_OFF, OnbtnSETSECURITYOFF)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRfidTagView message handlers

BOOL CRfidTagView::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	InitESL_Mgr( _T("SM_RFID_VIEW") );

	m_ViewerControl.InitViewer( IDC_GridtagINFO , this );
	m_ViewerControl.SetViewerWidth( 25 , 74 );	
	m_ViewerControl.SetFixedCols(0);
	m_ViewerControl.SetFixedRows(0);

	GatherShowTagInfo();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

INT CRfidTagView::GatherShowTagInfo()
{
	EFS_BEGIN

	INT	ids;
	ClearInfo();

	ids = GatherTagInfo();
	if ( ids == 100 ) 
	{
		OnCancel();	
		return 0;
	}
	ViewTagInfo();

	ViewSecurityMode();
	
	return 0;

	EFS_END
	return -1;

}

INT CRfidTagView::ClearInfo()
{
	EFS_BEGIN

	CESL_DataMgr* pDM = FindDM(_T("DM_RFID_TAG_INFO_LIST"));
	pDM->FreeData();

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTAGINFO);
	pTab->DeleteAllItems();

	pTab->InsertItem( 0 , _T("태그 정보가 존재하지 않습니다.") );

	const INT nFieldCnt = 8;
	CString sFieldAlias[nFieldCnt] = 
	{
		_T("시리얼번호") , _T("보안상태") , _T("등록번호") , _T("서명") , _T("관리구분") ,  _T("배가위치") , _T("대출상태")  , _T("예약건수")
	};

	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		m_ViewerControl.SetControlData( sFieldAlias[i] , _T("") );
	}

	m_ViewerControl.Display();

	return 0;

	EFS_END
	return -1;

}
INT CRfidTagView::GatherTagInfo()
{
EFS_BEGIN
	INT				ids;
	CRFID_Interface	RFID( this );
	INT				nStatus;
	INT				nTagCnt;
	CStringArray	SERIAL_LIST;
	CString			strFuncInfo;
	TAGDATASTRU		pTagStru;
	
	nStatus = RFID.GET_STATUS();
	CLocCommonAPI::MakeRFIDLog( _T("RFID_GetStatus()"), _T("RFID 동기화"), __WFILE__, __LINE__, nStatus );
		
	if( 0 != nStatus )
	{		
		AfxMessageBox(_T("발견된 리더기가 없습니다."));
		return 100;
	}
	
	ids = RFID.GetTagSerial( SERIAL_LIST );
	nTagCnt = SERIAL_LIST.GetSize();

	strFuncInfo.Format( _T("RFID_GetTagSerials( SerialList, %d )"), nTagCnt );
	CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID Tag 동기화"), __WFILE__, __LINE__, ids );
	
	for( INT i = 0;i < nTagCnt;i++ )
	{		
		strFuncInfo.Format( _T("RFID_ToString( SerialList[i] , sSerialString )") );
		CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID DB 동기화"), __WFILE__, __LINE__, ids );

		memset(&pTagStru, 0, sizeof(TAGDATASTRU));

		if(RFID.GetSerialCountDB(SERIAL_LIST.GetAt(i)) == 1) continue;
		
		RFID.GetTagAllDataByUID( SERIAL_LIST.GetAt(i) , &pTagStru );		

		InsertDMFromRFID( &pTagStru );
	}
	
	UINT nShowFlag = SW_HIDE;
	if(1 < nTagCnt)
	{
		nShowFlag = SW_SHOW;
	}
	{
		CWnd* pWnd = GetDlgItem(IDC_btnSET_SECURITY_OFF);
		if(NULL != pWnd)
		{
			pWnd->ShowWindow(nShowFlag);
		}

		pWnd = GetDlgItem(IDC_btnSET_SECURITY_ON);
		if(NULL != pWnd)
		{
			pWnd->ShowWindow(nShowFlag);
		}
	}

	return 0;	
EFS_END
	return -1;
}

INT CRfidTagView::InsertDMFromRFID( TAGDATASTRU* pTagStru )
{
EFS_BEGIN
	
	INT		  ids = 0;
	CString	  sRCntQ , sRCnt, sValue;
	CString		strQuery, strResult, strBookTable, strIDXTable;

	const INT nFieldCnt = 2;
	CString   sFieldAlias[nFieldCnt] = 
	{
		_T("시리얼번호") , _T("보안상태")
	};
	CString   sData[nFieldCnt];
	CString   strFuncInfo;
	CString   sTmpShelfLocCode;

	strFuncInfo.Format( _T("RFID_ToString( pTagData->Serial , suSerial )") );
	CLocCommonAPI::MakeRFIDLog( strFuncInfo, _T("RFID DB 동기화"), __WFILE__, __LINE__, ids );

	CharConvert cv;
	TCHAR*      szTmp = NULL;
	INT         len   = 0;

	if( 0 == cv.MultiByteToUnicode((char*)pTagStru->strSerial ,&szTmp,&len) )
	{
		sData[0] = CString(szTmp);		
		delete [] szTmp;
	}

	if( !pTagStru->bValue)
	{
		sData[1] = _T("보안해제(0)");
		m_bSecurityMode = false;
	}
	else
	{
		sData[1] = _T("보안상태(1)");
		m_bSecurityMode = true;
	}

	CESL_DataMgr* pDM = FindDM( _T("DM_RFID_TAG_INFO_LIST") );	
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	strQuery.Format(_T("SELECT 'BO_BOOK_TBL' FROM BO_BOOK_TBL WHERE WORKNO = '%s' ")
					_T("UNION ")
					_T("SELECT 'SE_BOOK_TBL' FROM SE_BOOK_TBL WHERE WORKNO = '%s' "),
					sData[0], sData[0]);
	ids = TmpDM.GetOneValueQuery( strQuery, strBookTable );
	if( ids < 0 && ids != -4007 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InsertDMFromRFID") );

	if(strBookTable.IsEmpty() == TRUE){
		strBookTable = _T("BO_BOOK_TBL");
	}
	if(strBookTable.Compare(_T("SE_BOOK_TBL")) == 0){
		strIDXTable = _T("IDX_SE_TBL");
	}else{
		strIDXTable = _T("IDX_BO_TBL");
	}

	strQuery.Format( _T("SELECT REG_NO, ")
					 _T("(SELECT TITLE_INFO FROM %s WHERE REC_KEY = BB.SPECIES_KEY) AS TITLE_INFO, ")
					 _T("ECO$F_GET_CODE2DESC('1',MANAGE_CODE) || ' (' || MANAGE_CODE || ')' AS MANAGE_CODE, ")
					 _T("ECO$F_GET_CODE2DESC('19',SHELF_LOC_CODE) || ' (' || SHELF_LOC_CODE || ')' AS SHELF_LOC_CODE, ")
					 _T("ECO$F_GET_CODE2DESC('WS',WORKING_STATUS) || ' (' || WORKING_STATUS || ')' AS WORKING_STATUS, ")
					 _T("(SELECT COUNT(RESERVATION_DATE) || '건' FROM LS_WORK_T01 WHERE STATUS = '3' AND BOOK_KEY = BB.REC_KEY) AS RESERVATION_CNT ")
					 _T("FROM %s BB WHERE WORKNO = '%s' ")
					 , strIDXTable, strBookTable, sData[0] );
	ids = TmpDM.RestructDataManager(strQuery);
	if( ids < 0 && ids != -4007 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InsertDMFromRFID") );

	pDM->InsertRow(-1);
	INT nInsertIndex = pDM->GetRowCount() - 1;
	for( INT i = 0 ; i < nFieldCnt  ; i++ )
	{
		ids = pDM->SetCellData( sFieldAlias[i] , sData[i] , nInsertIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("InsertDMFromRFID") );
	}

	sValue = TmpDM.GetCellData(0, 0);
	ids = pDM->SetCellData( _T("등록번호") , sValue , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("InsertDMFromRFID") );

	sValue = TmpDM.GetCellData(0, 1);
	ids = pDM->SetCellData( _T("서명") , sValue , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("InsertDMFromRFID") );

	sValue = TmpDM.GetCellData(0, 2);
	ids = pDM->SetCellData( _T("관리구분") , sValue , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -6 , _T("InsertDMFromRFID") );

	sValue = TmpDM.GetCellData(0, 3);
	ids = pDM->SetCellData( _T("배가위치") , sValue , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("InsertDMFromRFID") );

	sValue = TmpDM.GetCellData(0, 4);
	ids = pDM->SetCellData( _T("대출상태") , sValue , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -8 , _T("InsertDMFromRFID") );

	sValue = TmpDM.GetCellData(0, 5);
	ids = pDM->SetCellData( _T("예약건수") , sValue , nInsertIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("InsertDMFromRFID") );

	return 0;

EFS_END
	return -1;
}

INT CRfidTagView::ViewTagInfo()
{
	EFS_BEGIN


	CESL_DataMgr* pDM = FindDM( _T("DM_RFID_TAG_INFO_LIST") );

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox(_T("보여줄 정보가 존재하지 않습니다.") );
		return 1;
	}

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTAGINFO);
	pTab->DeleteAllItems();

	CString sTitle , sSerial , sRegNo;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
  		pDM->GetCellData( _T("시리얼번호") , i , sSerial );
		pDM->GetCellData( _T("등록번호") , i , sRegNo );
  		sTitle = sSerial + _T(":") + sRegNo;
		pTab->InsertItem( i , sTitle );
	}

	DisplayGrid();
	return 0;

	EFS_END

	return -1;


}

INT CRfidTagView::DisplayGrid()
{
	EFS_BEGIN


	const INT nFieldCnt = 8;
	CString sFieldAlias[nFieldCnt] = 
	{
		_T("시리얼번호") , _T("보안상태") , _T("등록번호") , _T("서명") , _T("관리구분") ,  _T("배가위치") , _T("대출상태")  , _T("예약건수")
	};
	
	CESL_DataMgr* pDM = FindDM( _T("DM_RFID_TAG_INFO_LIST") );

	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTAGINFO);

	INT nIndex = pTab->GetCurSel();

	if( pDM->GetRowCount() - 1 < nIndex )
		return 0;

	CString sData;
	for( INT i = 0 ; i < nFieldCnt ; i++ )
	{
		sData.Empty();
		pDM->GetCellData( sFieldAlias[i] , nIndex , sData );

		if(_T("보안상태") == sFieldAlias[i])
		{
			if(_T("보안상태(1)") == sData || _T("1") == sData)
			{
				m_bSecurityMode = true;
			}
			else
			{
				m_bSecurityMode = false;
			}
			ViewSecurityMode();
		}

		m_ViewerControl.SetControlData( sFieldAlias[i] , sData );
	}

	m_ViewerControl.Display();

	return 0;

	EFS_END

	return -1;


}

VOID CRfidTagView::OnbtnRICLOSE() 
{
	EFS_BEGIN

	OnCancel();	

	EFS_END
}

VOID CRfidTagView::OnbtnRIREFRESH() 
{
	EFS_BEGIN

	GatherShowTagInfo();	

	EFS_END
}

VOID CRfidTagView::ViewSecurityMode()
{
	CButton *pBtn = ( CButton * )GetDlgItem( IDC_btnSET_SECURITY_MOD );
	if( m_bSecurityMode )
		pBtn->SetWindowText( _T("보안해제") );
	else
		pBtn->SetWindowText( _T("보안설정") );
}

VOID CRfidTagView::OnbtnSETSECURITYMOD() 
{
	int ids = 0;
	CRFID_Interface RFID(this);
	CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_tabTAGINFO);
	INT nIndex = pTab->GetCurSel();

	CString strSerial;
	CESL_DataMgr* pDM = FindDM(_T("DM_RFID_TAG_INFO_LIST"));
	ids = pDM->GetCellData(_T("시리얼번호"), nIndex, strSerial);
		
	if(TRUE == m_bSecurityMode)
	{
		ids = RFID.ChangeTagLoanProcDataByUID(strSerial, 1);
	}
	else
	{
		ids = RFID.ChangeTagLoanProcDataByUID(strSerial, 0);
	}
	if(0 == ids)
	{
		AfxMessageBox( _T("변경되었습니다.") );
	}
	else
	{
		AfxMessageBox( _T("변경할 자료가 존재하지 않습니다.") );
	}	
	GatherShowTagInfo();
	ViewSecurityMode();
}

HBRUSH CRfidTagView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}

void CRfidTagView::OnSelchangetabTAGINFO(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DisplayGrid();
	
	*pResult = 0;
}

void CRfidTagView::OnbtnSETSECURITYON() 
{
	// TODO: Add your control notification handler code here
	CRFID_Change RFID;
	INT ids;
 	
	ids = RFID.Change( RFID_CHANGE_SECURITY_ON );
 
 	if( ids )
	{
 		AfxMessageBox( _T("변경할 자료가 존재하지 않습니다.") );
	}
 	else
	{
 		AfxMessageBox( _T("변경되었습니다.") );
	}

	GatherShowTagInfo();
	ViewSecurityMode();
}

void CRfidTagView::OnbtnSETSECURITYOFF() 
{
	// TODO: Add your control notification handler code here
	CRFID_Change RFID;
	INT ids;
 	
	ids = RFID.Change( RFID_CHANGE_SECURITY_OFF );
 
 	if( ids )
	{
 		AfxMessageBox( _T("변경할 자료가 존재하지 않습니다.") );
	}
 	else
	{
 		AfxMessageBox( _T("변경되었습니다.") );
	}	

	GatherShowTagInfo();
	ViewSecurityMode();
}

