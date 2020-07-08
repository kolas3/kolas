// BO_LOC_38901.cpp : implementation file
//

#include "stdafx.h"
#include "BO_LOC_38901.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3890 dialog


CBO_LOC_3890::CBO_LOC_3890(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3890)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

// 추가 : DeleteObject위한 소멸자 추가 재용 [2008-05-15]
CBO_LOC_3890::~CBO_LOC_3890()
{
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]
}

VOID CBO_LOC_3890::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3890)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3890, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3890)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3890 message handlers

BOOL CBO_LOC_3890::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// 추가 : 다이어로그&스태틱 색변경 재용 [2008-05-15]	
	EnableThemeDialogTexture(GetSafeHwnd()); 

	INT ids = InitESL_Mgr( _T("BO_LOC_3890") );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	/// 메일종류 정보 구성
	ids = InitMailClassInfo();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("OnInitDialog") );

	/// 그리드 콜벡 함수 설정
	CESL_Grid* pGrid = (CESL_Grid*)FindControl(_T("CM_BO_LOC_3890"),_T("메인그리드"));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("OnInitDialog") );

	pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3890::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if( GetSafeHwnd() == NULL ) return ;

	CWnd* pWnd = GetDlgItem(IDC_grid3890_Main);

	if( pWnd == NULL ) return;

	pWnd->MoveWindow( 0 , 0 , cx , cy );
	
}



BOOL CBO_LOC_3890::Create(CWnd* pParentWnd) 
{


	return CDialog::Create(IDD, pParentWnd);


}



INT CBO_LOC_3890::InitMailClassInfo()
{



	INT ids;

	CESL_DataMgr* pMailClassDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pMailClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("InitMailClassInfo") );

	ids = pMailClassDM->RefreshDataManager(_T(""));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("InitMailClassInfo") );

	pMailClassDM->SORT( _T("코드") );

	return 0;




}


INT CBO_LOC_3890::ConvertMailClassCodeToDesc(CString sCode,CString &sDesc)
{


	INT ids;

	CESL_DataMgr* pMailClassDM = FindDM(_T("DM_BO_LOC_3830_MAIL_CLASS"));
	if( pMailClassDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("ConvertMailClassCodeToDesc") );

	CString sTmpCode;
	for( INT i = 0 ; i < pMailClassDM->GetRowCount() ; i++ )
	{
		ids = pMailClassDM->GetCellData( _T("코드") , i , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -2 , _T("ConvertMailClassCodeToDesc") );

		if( sCode.Compare(sTmpCode) == 0 )
		{
			ids = pMailClassDM->GetCellData( _T("설명") , i , sDesc );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -3 , _T("ConvertMailClassCodeToDesc") );
			
			return 0;
		}
	}


	return 1;


}


INT CBO_LOC_3890::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	INT ids;

	if( strDMFieldAlias.Compare(_T("메일종류")) == 0 )
	{
		ids = ConvertMailClassCodeToDesc( str , str );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("GridDisplayEventProc") );
	}
	else if( strDMFieldAlias.Compare(_T("상태")) == 0 )
	{
		if( str.IsEmpty() || str.Compare(_T("N")) == 0 )
			str = _T("발송대기중");
		else if( str.Compare(_T("OK")) == 0 )
			str = _T("발송완료");
		else if( str.Compare(_T("CANCEL")) == 0 )
			str = _T("취소됨");
		else if( str.Compare(_T("S")) == 0 )
			str = _T("발송중지");
		else
			str = _T("발송처리중");
	}


	return 0;


}


/*
@Create
 - 2009.03.25 BY KSJ
@Description
 - 발송대기(STATUS:N)-> 발송중지(STATUS:S)로 변경
@Parameter

@Return Value
 - 
 - 0, 양수 : 성공
 - 음수 : 실패
@Mini Spec
 1.
*/
INT CBO_LOC_3890::StopSendMail()
{

	INT ids;

	// 검색했는지 확인
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3890") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("StopSendMail") );
	}

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("검색 먼저 해주십시오.") );
		return 1;
	}

	// 선정된것이 존재하는지 확인
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3890") , _T("메인그리드") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("StopSendMail") );
	}

	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("중지할 메일을 선정해주십시오.") );
		return 2;
	}

	// 한건 한건 처리해야함
	// 선정된 것을 돌아가면 쿼리를 날려
	// 중지할 수 있는 상태인지 알아본다.

	// 중지가능하면 중지

	// 이미 DB에 존재하지 않는다면 발송됨으로 변경

	pGrid->SelectGetTailPosition();
	INT nSIndex , i;

	INT nSuccesCnt = 0;
	INT nFailCnt = 0;

	CString sTempKey , sStatus;
	CString sSql;

	for( i=0 ; i<nSelectCnt ; i++ )
	{
		nSIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("상태") , nSIndex , sStatus );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("StopSendMail") ); 
		}

		ids = pDM->GetCellData( _T("발송대기목록KEY") , nSIndex , sTempKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("StopSendMail") ); 
		}

		if( sStatus.IsEmpty() || ( sStatus.Compare(_T("N")) == 0 ) )
		{
			sSql.Format( _T("SELECT STATUS FROM MN_MAIL_TEMP_TBL WHERE REC_KEY = %s") , sTempKey );
			ids = pDM->GetOneValueQuery( sSql , sStatus );
			if( ids >= 0 && ( sStatus.IsEmpty() || ( sStatus.Compare(_T("N")) == 0 ) ) )
			{
				pDM->StartFrame();

				pDM->InitDBFieldData();

				pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("S") );

				ids = pDM->MakeUpdateFrame( _T("MN_MAIL_TEMP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sTempKey );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("StopSendMail") );
				}


				ids = pDM->SendFrame();
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("StopSendMail") );
				}

				pDM->EndFrame();

				ids = pDM->SetCellData( _T("상태") , _T("S") , nSIndex  );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("StopSendMail") );
				}

				nSuccesCnt++;
			}
			else
			{
				if( ids < 0 ) 
				{
					ids = pDM->SetCellData( _T("상태") , _T("OK") , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("StopSendMail") );
					}
				}
				else
				{
					ids = pDM->SetCellData( _T("상태") , sStatus , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("StopSendMail") );
					}
				}

				nFailCnt++;
			}

			pGrid->DisplayLine(nSIndex);
		}
		else
		{
			nFailCnt++;
		}

		pGrid->SelectGetPrev();
	}
	

	CString sMsg;
	if( nSuccesCnt == 0 )
	{
		// 2009.03.25 UPDATE BY KSJ : 실패 메세지 변경
		//DEL sMsg.Format(_T("변경할 수 있는 메일이 존재하지 않습니다."));
		sMsg.Format(_T("중지는 발송대기 중인 메일만 가능합니다."));
	}
	else
	{
		sMsg.Format(_T("변경되었습니다.\r\n변경한 메일 수 : %d 건 , 변경할 수 없는 메일 수 : %d 건") , nSuccesCnt , nFailCnt );
	}

	AfxMessageBox( sMsg );

	return 0;
}

/*
@Create
 - 2009.03.25 BY KSJ
@Description
 - 발송대기(STATUS:N),발송중지(STATUS:S) -> MN_MAIL_TEMP_TBL에서 삭제
@Parameter

@Return Value
 - 
 - 0, 양수 : 성공
 - 음수 : 실패
@Mini Spec
 1. 
*/
INT CBO_LOC_3890::CancelSendMail()
{
	INT ids;

	// 검색했는지 확인
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3890") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("CancelSendMail") );
	}

	if( pDM->GetRowCount() == 0 )
	{
		AfxMessageBox( _T("검색 먼저 해주십시오.") );
		return 1;
	}

	// 선정된것이 존재하는지 확인
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3890") , _T("메인그리드") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("CancelSendMail") );
	}

	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("중지할 메일을 선정해주십시오.") );
		return 2;
	}

	// 한건 한건 처리해야함
	// 선정된 것을 돌아가면 쿼리를 날려
	// 취소할 수 있는 상태인지 알아본다.

	// 중지가능하면 중지

	// 이미 DB에 존재하지 않는다면 발송됨으로 변경

	pGrid->SelectGetTailPosition();
	INT		nSIndex , i;
	INT		nSuccesCnt = 0;
	INT		nFailCnt = 0;

	CString sTempKey , sStatus;
	CString sSql;

	for( i=0 ; i<nSelectCnt ; i++ )
	{
		nSIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("상태") , nSIndex , sStatus );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("CancelSendMail") ); 
		}

		ids = pDM->GetCellData( _T("발송대기목록KEY") , nSIndex , sTempKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("CancelSendMail") ); 
		}

		if( (sStatus.Compare(_T("N"))==0)  || (sStatus.Compare(_T("S"))==0) || sStatus.IsEmpty() )
		{
			sSql.Format( _T("SELECT STATUS FROM MN_MAIL_TEMP_TBL WHERE REC_KEY = %s") , sTempKey );
			ids = pDM->GetOneValueQuery( sSql , sStatus );
			if( ids >= 0 && ((sStatus.Compare(_T("N"))==0)  || (sStatus.Compare(_T("S"))==0) || sStatus.IsEmpty() ) )
			{
				pDM->StartFrame();

				ids = pDM->MakeDeleteFrame( _T("MN_MAIL_TEMP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sTempKey );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("CancelSendMail") );
				}


				ids = pDM->SendFrame();
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("CancelSendMail") );
				}

				pDM->EndFrame();

				ids = pDM->SetCellData( _T("상태") , _T("CANCEL") , nSIndex  );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("CancelSendMail") );
				}

				nSuccesCnt++;
			}
			else
			{
				if( ids < 0 ) 
				{
					ids = pDM->SetCellData( _T("상태") , _T("OK") , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("CancelSendMail") );
					}
				}
				else
				{
					ids = pDM->SetCellData( _T("상태") , sStatus , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("CancelSendMail") );
					}
				}

				nFailCnt++;
			}

			pGrid->DisplayLine(nSIndex);
		}
		else
		{
			nFailCnt++;
		}

		pGrid->SelectGetPrev();
	}
	
	CString sMsg;
	if( nSuccesCnt == 0 )
	{
		// 2009.03.25 UPDATE BY KSJ : 실패 메세지 변경
		//DEL sMsg.Format(_T("변경할 수 있는 메일이 존재하지 않습니다."));
		sMsg.Format(_T("취소는 발송대기,발송중지 중인 메일만 가능합니다."));
	}
	else
	{
		sMsg.Format(_T("변경되었습니다.\r\n변경한 메일 수 : %d 건 , 변경할 수 없는 메일 수 : %d 건") , nSuccesCnt , nFailCnt );
	}

	AfxMessageBox( sMsg );

	return 0;
}

/*
@Create
 - 2009.03.25 BY KSJ
@Description
 - 발송중지(STATUS:S) -> 발송대기(STATUS:N)으로 변경
@Parameter

@Return Value
 - 
 - 0, 양수 : 성공
 - 음수 : 실패
@Mini Spec
 1. 
*/
INT CBO_LOC_3890::SendMail()
{
	INT ids;

	// 검색했는지 확인
	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3890") );
	if( NULL == pDM ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("CancelSendMail") );
	}

	INT		nRowCnt	=	pDM->GetRowCount();
	if( 0 == nRowCnt )
	{
		AfxMessageBox( _T("검색 먼저 해주십시오.") );
		return 1;
	}

	// 선정된것이 존재하는지 확인
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3890") , _T("메인그리드") );
	if( NULL == pGrid ) 
	{
		ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("CancelSendMail") );
	}

	pGrid->SelectMakeList();
	INT nSelectCnt = pGrid->SelectGetCount();
	
	if( nSelectCnt == 0 )
	{
		AfxMessageBox( _T("중지할 메일을 선정해주십시오.") );
		return 2;
	}

	// 한건 한건 처리해야함
	// 선정된 것을 돌아가면 쿼리를 날려
	// 발송할 수 있는 상태인지 알아본다.

	// 중지가능하면 중지

	// 이미 DB에 존재하지 않는다면 발송됨으로 변경

	pGrid->SelectGetTailPosition();

	INT nSuccesCnt = 0;
	INT nFailCnt = 0;

	CString sTempKey , sStatus;
	CString sSql;

	INT		i , nSIndex;
	for( i=0 ; i<nSelectCnt ; i++ )
	{
		nSIndex = pGrid->SelectGetIdx();

		ids = pDM->GetCellData( _T("상태") , nSIndex , sStatus );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SendMail") ); 
		}

		ids = pDM->GetCellData( _T("발송대기목록KEY") , nSIndex , sTempKey );
		if( 0 > ids ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SendMail") ); 
		}

		if( (sStatus.Compare(_T("S"))==0) || sStatus.IsEmpty() )
		{
			sSql.Format( _T("SELECT STATUS FROM MN_MAIL_TEMP_TBL WHERE REC_KEY = %s") , sTempKey );
			ids = pDM->GetOneValueQuery( sSql , sStatus );
			if( ids >= 0 && ((sStatus.Compare(_T("S"))==0) || sStatus.IsEmpty() ) )
			{
				pDM->StartFrame();

				pDM->InitDBFieldData();

				pDM->AddDBFieldData( _T("STATUS") , _T("STRING") , _T("N") );

				ids = pDM->MakeUpdateFrame( _T("MN_MAIL_TEMP_TBL") , _T("REC_KEY") , _T("NUMERIC") , sTempKey );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1050 , _T("SendMail") );
				}

				ids = pDM->SendFrame();
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1060 , _T("SendMail") );
				}

				pDM->EndFrame();

				ids = pDM->SetCellData( _T("상태") , _T("N") , nSIndex  );
				if( 0 > ids ) 
				{
					ERROR_MSG_RETURN_ERROR_CODE2( -1070 , _T("SendMail") );
				}


				nSuccesCnt++;
			}
			else
			{
				if( ids < 0 ) 
				{
					ids = pDM->SetCellData( _T("상태") , _T("OK") , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1080 , _T("SendMail") );
					}
				}
				else
				{
					ids = pDM->SetCellData( _T("상태") , sStatus , nSIndex  );
					if( 0 > ids ) 
					{
						ERROR_MSG_RETURN_ERROR_CODE2( -1090 , _T("SendMail") );
					}
				}

				nFailCnt++;
			}

			pGrid->DisplayLine(nSIndex);
		}
		else
		{
			nFailCnt++;
		}

		pGrid->SelectGetPrev();
	}
	

	CString sMsg;
	if( nSuccesCnt == 0 )
	{
		// 2009.03.25 UPDATE BY KSJ : 실패 메세지 변경
		//DEL sMsg.Format(_T("변경할 수 있는 메일이 존재하지 않습니다."));
		sMsg.Format(_T("발송은 발송중지 중인 메일만 가능합니다."));
	}
	else
	{
		sMsg.Format(_T("변경되었습니다.\r\n변경한 메일 수 : %d 건 , 변경할 수 없는 메일 수 : %d 건") , nSuccesCnt , nFailCnt );
	}

	AfxMessageBox( sMsg );

	return 0;
}

HBRUSH CBO_LOC_3890::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	/************************************
	 재용 [2008-05-15]
	다이어로그 & 스태틱 색변경
	 ************************************/
	// 여기서부터 	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
