// ReserveLinkBookInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ReserveLinkBookInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReserveLinkBookInfoDlg dialog
CReserveInfo::CReserveInfo()
{
}

CReserveLinkBookInfoDlg::CReserveLinkBookInfoDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CReserveLinkBookInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nSelectIdx = -1;

	m_nReserveInfoMode	= 0;
	m_nMode				= 0;

	m_pParentMgr = pParent;
}

CReserveLinkBookInfoDlg::~CReserveLinkBookInfoDlg()
{
}


VOID CReserveLinkBookInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReserveLinkBookInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReserveLinkBookInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CReserveLinkBookInfoDlg)
	ON_BN_CLICKED(IDC_btnVR_CLOSE, OnbtnVRCLOSE)
	ON_BN_CLICKED(IDC_btnVR_SELECT, OnbtnVRSELECT)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReserveLinkBookInfoDlg message handlers

BOOL CReserveLinkBookInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 	

	INT ids;

	HICON shit = AfxGetApp()->LoadIcon( IDR_BL_LOC_VOL_RESERVE_MNG_MAINFRAME );

	SetIcon( shit, FALSE );


	ids = InitESL_Mgr(_T("BL_RESERVE_LINK_BOOK_INFO_DLG"));
	if( ids < 0 ) 
	{
		AfxMessageBox( _T("InitESL_Mgr(BL_RESERVE_LINK_BOOK_INFO_DLG) ERROR!!") );
		return FALSE;
	}

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BL_RESERVE_LINK_BOOK_INFO_DLG") , _T("책정보") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog > FindControl") );
	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	


	ids = LoadBookInfoFromDB();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( FALSE , _T("OnInitDialog > LoadBookInfoFromDB") );

	CESL_DataMgr* pDM = FindDM(_T("DM_BL_RESERVE_LINK_BOOK_INFO_DLG"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AutoComplete") );

	INT			nRCnt = pDM->GetRowCount();
	INT			nReserveMarkCnt = 0;
	CString		sValue;
	for(INT i = 0; i < nRCnt; i++)
	{
		pDM->GetCellData(_T("지정여부"), i, sValue);
		if( sValue == _T("X") ) nReserveMarkCnt++;
	}
	if( 1 > nRCnt || 1 > nReserveMarkCnt )
	{
		EndDialog(5);
		return TRUE;
	}

	ids = AllControlDisplay(_T("CM_BL_RESERVE_LINK_BOOK_INFO_DLG"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("LoadBookInfoFromDB") ); 

	ids = AutoComplete();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( FALSE , _T("OnInitDialog > LoadBookInfoFromDB") );

	if( (ids == 0) || (ids == 2) || (ids == 3) ) EndDialog(ids);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CReserveLinkBookInfoDlg::LoadBookInfoFromDB()
{

EFS_BEGIN

	INT ids;
	CString sSQuery;

	if( m_RI.sPublishFormCode.Compare(_T("MO")) == 0 )
	{
		sSQuery.Format( _T("SELECT B.REG_NO , ")
						_T("CL.RETURN_PLAN_DATE , ")
						_T("NULL,")
						_T("B.WORKING_STATUS,")
						_T("B.SEPARATE_SHELF_CODE,")
						_T("B.CLASS_NO,")
						_T("B.BOOK_CODE,")
						_T("B.VOL_CODE,") 
						_T("B.COPY_CODE,")
						_T("DECODE((SELECT MAX(CL2.REG_NO) FROM LS_WORK_T01 CL2 WHERE CL2.STATUS = '3' AND CL2.BOOK_KEY = B.REC_KEY),NULL,'X','O'),")
						_T("B.SPECIES_KEY,")
						_T("B.REC_KEY,")
						_T("B.VOL_SORT_NO,")
						_T("'MO',")
						_T("B.SHELF_LOC_CODE,")
						_T("B.CLASS_NO_TYPE,")
						_T("B.CLASS_NO,")
						_T("B.MANAGE_CODE,")
						_T("B.REG_CODE ,")
						_T("B.PREV_WORKING_STATUS ,")
						_T("B.WORKNO ")		// 2005-12-28 UPD BY PDW(Jr)   RFID_SERIAL -> WORKNO 변경
						_T("FROM BO_BOOK_TBL B , ")
						_T("(SELECT BOOK_KEY,RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE STATUS IN ('0','2','5')) CL ")
						_T("WHERE B.REC_KEY = CL.BOOK_KEY(+) AND B.SPECIES_KEY = %s AND B.MANAGE_CODE='%s' AND ")
						_T("B.WORKING_STATUS IN ('BOL112N') AND B.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ') AND ") // 2009.10.09 UPDATE BY PWR : 이용제한구분(특수,열람,사서,귀중) 제외되도록 수정
						, m_RI.sSpeciesKey, m_pInfo->MANAGE_CODE );

			if( m_RI.sVolInfo.IsEmpty() )
			{
				sSQuery += _T(" B.VOL_SORT_NO IS NULL ");
			}
			else
			{
				sSQuery += _T(" B.VOL_SORT_NO = ") + m_RI.sVolInfo;
			}
	}
	else
	{
		sSQuery.Format( _T("SELECT B.REG_NO , ")
						_T("CL.RETURN_PLAN_DATE , ")
						_T("NULL,")
						_T("B.WORKING_STATUS,")
						_T("B.SEPARATE_SHELF_CODE,")
						_T("B.CLASS_NO,")
						_T("B.BOOK_CODE,")
						_T("B.VOL_CODE,") 
						_T("B.COPY_CODE,")
						_T("DECODE((SELECT MAX(CL2.REG_NO) FROM LS_WORK_T01 CL2 WHERE CL2.STATUS = '3' AND CL2.BOOK_KEY = B.REC_KEY),NULL,'X','O'),")
						_T("B.SPECIES_KEY,")
						_T("B.REC_KEY,")
						_T("B.VOL_KEY,")
						_T("'MO',")
						_T("B.SHELF_LOC_CODE,")
						_T("B.CLASS_NO_TYPE,")
						_T("B.CLASS_NO,")
						_T("B.MANAGE_CODE,")
						_T("B.REG_CODE ,")
						_T("B.PREV_WORKING_STATUS ,")
						_T("B.WORKNO ")		// 2005-12-28 UPD BY PDW(Jr)   RFID_SERIAL -> WORKNO 변경
						_T("FROM SE_BOOK_TBL B , ")
						_T("(SELECT BOOK_KEY,RETURN_PLAN_DATE FROM LS_WORK_T01 WHERE STATUS IN ('0','2','5')) CL ")
						_T("WHERE B.REC_KEY = CL.BOOK_KEY(+)  AND B.SPECIES_KEY = %s AND B.MANAGE_CODE='%s' AND ") 
						_T("B.WORKING_STATUS IN ('SEL112N') AND B.USE_LIMIT_CODE NOT IN ('CL','CD','CA','IZ') AND ") // 2009.10.09 UPDATE BY PWR : 이용제한구분(특수,열람,사서,귀중) 제외되도록 수정
						, m_RI.sSpeciesKey, m_pInfo->MANAGE_CODE );

			if( m_RI.sVolInfo.IsEmpty() )
			{
				sSQuery += _T(" B.VOL_KEY IS NULL ");
			}
			else
			{
				sSQuery += _T(" B.VOL_KEY = ") + m_RI.sVolInfo;
			}
	}

	CESL_DataMgr* pDM = FindDM(_T("DM_BL_RESERVE_LINK_BOOK_INFO_DLG"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("LoadBookInfoFromDB") );

	CESL_DataMgr TmpDM;
	CLocCommonAPI::MakeCopyDM( this , pDM , &TmpDM );

	ids = TmpDM.RestructDataManager( sSQuery );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("LoadBookInfoFromDB") ); 

	const INT nCnt = 20;
	CString sDMFieldAlias[nCnt] =
	{
		_T("등록번호"),	_T("반납예정일") , _T("청구기호") , _T("자료상태") , _T("IBS_청구_별치기호_코드") , 
		_T("IBS_청구_분류기호") , _T("IBS_청구_도서기호") , _T("IBS_청구_권책기호") , _T("IBS_청구_복본기호") ,_T("지정여부") , 
		_T("종KEY") , _T("책KEY") , _T("권호정보KEY") , _T("발행형태구분") , _T("자료실구분") , 
		_T("분류기호구분") , _T("분류기호") , _T("관리구분") , _T("등록구분") , _T("이전자료상태") 

	};

	INT nRowCnt = TmpDM.GetRowCount();
	CString sData;
	pDM->FreeData();
	for( INT i = 0 ; i < nRowCnt ; i++ )
	{	
		pDM->InsertRow(-1);
		for( INT j = 0 ; j < nCnt ; j++ )
		{
			ids = TmpDM.GetCellData( i , j , sData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("LoadBookInfoFromDB") ); 

			ids = pDM->SetCellData( sDMFieldAlias[j] ,  sData , i );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("LoadBookInfoFromDB") ); 
		}
	}

	CLocCommonAPI::MakeShelfCode( this , _T("DM_BL_RESERVE_LINK_BOOK_INFO_DLG") );//++2009.01.08 UPDATE BY KSJ {{++


	return 0;

EFS_END
return -1;
}

INT CReserveLinkBookInfoDlg::AutoComplete()
{
EFS_BEGIN

	INT ids;

	if( m_RI.sBookKey.IsEmpty() ) return 1;

	CESL_DataMgr* pDM = FindDM(_T("DM_BL_RESERVE_LINK_BOOK_INFO_DLG"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("AutoComplete") );

	INT nRCnt = pDM->GetRowCount();
	
	INT nSIndex=-1;

	CString sBookKey;
	for( INT i = 0 ; i < nRCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("책KEY") , i , sBookKey );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("AutoComplete") );

		if( sBookKey.Compare(m_RI.sBookKey) == 0 )
			nSIndex = i;
	}

	if( nSIndex == -1 ) return 2;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BL_RESERVE_LINK_BOOK_INFO_DLG") , _T("책정보") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("AutoComplete") );

	pGrid->SetRowSel(nSIndex+1);  
	pGrid->SetReverse(nSIndex);

	ids = CheckSelect();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -4 , _T("AutoComplete") );

	if( m_nSelectIdx == -1 )
	{
		return 3;
	}

	return 0;

EFS_END
return -1;
}

VOID CReserveLinkBookInfoDlg::OnbtnVRCLOSE() 
{
EFS_BEGIN

	OnCancel();	

EFS_END
}

VOID CReserveLinkBookInfoDlg::OnbtnVRSELECT() 
{
EFS_BEGIN

	INT ids;

	ids = CheckSelect();
	if( ids < 0 ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnbtnRSELECT") );


	if( m_nSelectIdx == -1 )
	{
		return ;
	}

	OnCancel();	

EFS_END
}

VOID CReserveLinkBookInfoDlg::OnDblClickVrBookList() 
{
EFS_BEGIN

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BL_RESERVE_LINK_BOOK_INFO_DLG") , _T("책정보") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_VOID2( -1 , _T("OnDblClickRbookList") );

	if( pGrid->GetMouseRow() <= 0 ) return;

	OnbtnVRSELECT();

EFS_END
}

VOID CReserveLinkBookInfoDlg::OnSize(UINT nType, INT cx, INT cy) 
{
EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);

	if( GetSafeHwnd() == NULL ) return ;

	CWnd* pWnd = GetDlgItem( IDC_VR_BOOK_LIST );
	if( pWnd->GetSafeHwnd() == NULL ) return ;
	pWnd->MoveWindow( 3, 40 , cx-6 , cy - 50);
	

EFS_END
}

INT CReserveLinkBookInfoDlg::SetInfo(CReserveInfo &RI )
{
EFS_BEGIN

	m_RI = RI;

	return 0;

EFS_END
return -1;
}

INT CReserveLinkBookInfoDlg::GetInfo(CReserveInfo &RI)
{
EFS_BEGIN

	if( m_nSelectIdx == -1 ) return 1;

	RI = m_RI;
	return 0;

EFS_END
return -1;
}

INT CReserveLinkBookInfoDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
EFS_BEGIN

	INT ids = 0;

	if( strDMFieldAlias.Compare( _T("자료상태") ) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( str , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 ,  _T("GridDisplayEventProc > GetWorkingStatusDescToCode")  );
	}
	else if( strDMFieldAlias.Compare( _T("자료실구분") ) == 0 )
	{
		m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("자료실구분") , str , str );
	}
	else if( strDMFieldAlias.Compare( _T("이전자료상태") ) == 0 )
	{
		ids = CLocCommonAPI::GetWorkingStatusDescToCode( str , 1 );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 ,  _T("GridDisplayEventProc > GetWorkingStatusDescToCode")  );
	}
	else if( strDMFieldAlias.Compare( _T("발행형태구분") ) == 0 )
	{
		if( str.Compare(_T("MO")) == 0 ) 
			str = _T("단행");
		else
			str = _T("연속");
	}

	return 0;
EFS_END
return -1;

}

INT CReserveLinkBookInfoDlg::CheckSelect()
{
EFS_BEGIN

	INT ids;

	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BL_RESERVE_LINK_BOOK_INFO_DLG") , _T("책정보") );
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("CheckSelect") );

	INT nIndex = pGrid->GetIdx();

	CESL_DataMgr* pDM = FindDM(_T("DM_BL_RESERVE_LINK_BOOK_INFO_DLG"));
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("CheckSelect") );

	if( pDM->GetRowCount() == 0 ) return 1;

	CString sRComplete;
	ids = pDM->GetCellData( _T("지정여부")  , nIndex , sRComplete );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("CheckSelect") );

	if( sRComplete.Compare(_T("O")) == 0 )
	{
		AfxMessageBox( _T("선택한 자료는 이미 지정된 자료이기 때문에 지정할 수 없습니다.") );
		return 1;
	}

	m_nSelectIdx = nIndex;
	ids = MakeRI();

	return 0;

EFS_END
return -1;
}

INT CReserveLinkBookInfoDlg::MakeRI()
{
EFS_BEGIN

	INT ids;

	const INT nCnt = 17;
	CString sFieldAlias[nCnt] =
	{
		_T("등록번호"),	_T("반납예정일"),	_T("청구기호"),		_T("자료상태"),		_T("종KEY"), 
		_T("책KEY"),	_T("권호정보KEY"),	_T("발행형태구분"),	_T("자료실구분"),	_T("분류기호구분"), 
		_T("분류기호"),	_T("관리구분"),		_T("등록구분"),		_T("이전자료상태"),	_T("IBS_청구_권책기호"), 
		_T("RFID시리얼"), _T("IBS_청구_별치기호_코드")
	};

	CString sData;

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = GetDataMgrData( _T("DM_BL_RESERVE_LINK_BOOK_INFO_DLG") , sFieldAlias[i] , sData , m_nSelectIdx );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("MakeRI") ); 

		switch(i)
		{
		case 0:
			m_RI.sRegNo = sData;
			break;
		case 1:
			break;
		case 2:
			m_RI.sCallNo = sData;
			break;
		case 3:
			m_RI.sWorkingStatus = sData;
			break;
		case 4:
			m_RI.sSpeciesKey = sData;
			break;
		case 5:
			m_RI.sBookKey = sData;
			break;
		case 6:
			m_RI.sVolInfo = sData;
			break;
		case 7:
			m_RI.sPublishFormCode = sData;
			break;
		case 8:
			m_RI.sShelfLocCode = sData;
			break;
		case 9:
			m_RI.sClassNoType = sData;
			break;
		case 10:
			m_RI.sClassNo = sData;
			break;
		case 11:
			m_RI.sManageCode = sData;
			break;
		case 12:
			m_RI.sRegCode = sData;
			break;
		case 13:
			m_RI.sPrevWorkingStatus = sData;
			break;
		case 14:
			m_RI.sVolCode = sData;
			break;
		case 15:
			m_RI.sRFIDSerial = sData;
			break;
		case 16:
			m_RI.sSeparateShelfCode = sData;
			break;
		};
	}

	return 0;

EFS_END
return -1;
}

BEGIN_EVENTSINK_MAP(CReserveLinkBookInfoDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CReserveLinkBookInfoDlg)
	ON_EVENT(CReserveLinkBookInfoDlg, IDC_VR_BOOK_LIST, -601 /* DblClick */, OnDblClickVrBookList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

HBRUSH CReserveLinkBookInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	
	return hbr; 
}

BOOL CReserveLinkBookInfoDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CReserveLinkBookInfoDlg::SetIsAutoSetReservationDay(CString sIsAutoSetReservationDay)
{
	m_sIsAutoSetReservationDay	=		sIsAutoSetReservationDay;
}
