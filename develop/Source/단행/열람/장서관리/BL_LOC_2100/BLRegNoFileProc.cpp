// BLRegNoFileProc.cpp: implementation of the CBLRegNoFileProc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BLRegNoFileProc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLRegNoFileProc::CBLRegNoFileProc(CESL_Mgr *pWnd)
	: CSPF_Work(pWnd)
{
	m_sTmpRegNoCMAlias = _T("CM_BO_LOC_2110");
	m_sTmpRegNoDMAlias = _T("DM_BO_LOC_2100_REG_NO");
	//m_sTmpRegNoDMAlias = _T("DM_BO_LOC_2110_TMP_REG_NO");
	m_sRegNoInfoCMAlias = _T("CM_BO_LOC_2110_INFO");
	m_sGridAlias = _T("grid");

	m_pTmpRegNoCM = m_pParentMgr->FindCM( m_sTmpRegNoCMAlias );
	if( m_pTmpRegNoCM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd ,  -1 , _T("CBLRegNoFileProc(CESL_Mgr *pWnd)") );
	
	m_pTmpRegNoDM = m_pParentMgr->FindDM( m_sTmpRegNoDMAlias );
	if( m_pTmpRegNoDM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd ,  -2 , _T("CBLRegNoFileProc(CESL_Mgr *pWnd)") );

	m_pRegNoInfoCM = m_pParentMgr->FindCM( m_sRegNoInfoCMAlias );
	if( m_pRegNoInfoCM == NULL ) ERROR_MSG_RETURN_VOID( m_pParentMgr->m_hWnd , -3 , _T("CBLRegNoFileProc(CESL_Mgr *pWnd)") );

	m_pParentSpfWork = new CBLBookCheckProc( m_pParentMgr ); // 부모 SPF

	m_nCheckRegNo = 0;

}

CBLRegNoFileProc::~CBLRegNoFileProc()
{
	if( m_pParentSpfWork != NULL ) delete m_pParentSpfWork;
	m_pParentSpfWork = NULL;
}

INT CBLRegNoFileProc::SPFExcutePath( CString sPathAlias )
{
	EFS_BEGIN

	INT ids;

	if( sPathAlias.Compare( DM_ONE_KEYBORD_ADD ) == 0 )
	{
		ids = AddOneRegNoFromKeybord();
	}
	else if( sPathAlias.Compare( DM_ONE_BARCODE_ADD ) == 0 )
	{
		ids = AddOneRegNoFromBarcode();
	}
	else if( sPathAlias.Compare( DM_CLEAR ) == 0 )
	{
		ids = DMClear();
	}
	else if( sPathAlias.Compare( DM_DELETE_ROW ) == 0 )
	{
		ids = DMDeleteRow();
	}
	else if( sPathAlias.Compare( DM_LOAD_FILE ) == 0 )
	{
		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
		//ids = DMLoadFile();
		ids = DMLoadFile_201809( true );
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
	}
	else if( sPathAlias.Compare( DM_LOAD_FILE2 ) == 0 )
	{
		// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//ids = DMLoadFile2();
		ids = DMLoadFile_201809( false );
		// KOL.RED.2018.201 ---------------------------------------------------------------------------
	}
	else if( sPathAlias.Compare( DM_SAVE_FILE ) == 0 )
	{
		ids = DMSaveFile();
	}
	else if( sPathAlias.Compare( OPEN_REGNO_EDIT ) == 0 )
	{
		ids = OpenRegNoEdit();
	}
	else if( sPathAlias.Compare( SAVE_REGNO_EDIT ) == 0 )
	{
		ids = SaveRegNoEdit();
	}
	else 
	{
		return -1;
	}

	if( ids < 0 ) 
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못하였습니다.") );

	return 0;

	EFS_END
	return -1;

}


// 키보드로부터 등록번호를 추가한다.
INT CBLRegNoFileProc::AddOneRegNoFromKeybord()
{
	EFS_BEGIN

	INT ids;

	// 컨트롤에서 등록번호값을 구해온다.
	CString sRegCode;
	CString sRegNo;

	ids = m_pRegNoInfoCM->GetControlMgrData( _T("등록구분") , sRegCode , -1 , 0 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddOneRegNoFromKeybord()") );

	ids = m_pRegNoInfoCM->GetControlMgrData( _T("등록번호") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AddOneRegNoFromKeybord()") );

	if( sRegNo.IsEmpty() ) return 1;

	sRegNo = sRegCode + sRegNo ;

	// DM에 추가한다.
	ids = AddOneRegNo( sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AddOneRegNoFromKeybord()") );

	return 0;

	EFS_END
	return -1;

}

// 바코드로부터 등록번호를 추가한다.
INT CBLRegNoFileProc::AddOneRegNoFromBarcode()
{
	EFS_BEGIN

	INT ids;
	// 컨트롤에서 등록번호값을 구해온다.
	CString sRegNo;

	ids = m_pRegNoInfoCM->GetControlMgrData( _T("바코드") , sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddOneRegNoFromBarcode()") );

	if( sRegNo.IsEmpty() || sRegNo.GetLength() < 3 ) return 1;


	// DM에 추가한다.
	ids = AddOneRegNo( sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AddOneRegNoFromBarcode()") );

	return 0;

	EFS_END
	return -1;

}

// sRegNo(등록번호)를 DM에 추가한다.
INT CBLRegNoFileProc::AddOneRegNo( CString sRegNo )
{
	EFS_BEGIN

	INT ids;

	// 등록번호를 자리수에 맞춰 0을 입력한다.
	ids = CLocCommonAPI::MakeRegNoSpaceToZero( m_pParentMgr  ,  sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddOneRegNo( CString sRegNo )") ); 

	// DM에 추가한다. - 추가하기 전 Grid의 체크된 부분을 DM에도 옮겨준다.
	// 추가하기전에 중복된 등록번호인지 조사한다.
	ids = CheckDuplicateRegNo( sRegNo );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("AddOneRegNo( CString sRegNo )") ); 
	if( ids > 0 ) return 1;

	m_pTmpRegNoDM->InsertRow(-1);

	ids = m_pTmpRegNoDM->SetCellData( _T("등록번호") , sRegNo , m_pTmpRegNoDM->GetRowCount()-1 );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("AddOneRegNo( CString sRegNo )") ); 

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	cMapData_CheckNum.Add( sRegNo );
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	// 그리드를 다시 보여준다.
	ids = GridDisplay();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("AddOneRegNo( CString sRegNo )") ); 

	return 0;

	EFS_END
	return -1;

}

// 같은 등록번호가 존재하는지 검사
INT CBLRegNoFileProc::CheckDuplicateRegNo( CString sRegNo )
{
	EFS_BEGIN

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	// 해쉬로 찾기
	// nEqualCnt 현재 따로 쓰지는 않는다.
	/*
	INT ids;

	CString sTmpRegNo;
	INT nEqualCnt = 0;

	for( INT i = 0 ; i < m_pTmpRegNoDM->GetRowCount() ; i++ )
	{
		ids = m_pTmpRegNoDM->GetCellData( _T("등록번호") , i , sTmpRegNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("AddOneRegNo( CString sRegNo )") ); 

		if( sRegNo.Compare(sTmpRegNo)==0 )
			nEqualCnt++;
	}

	return nEqualCnt;
	*/
	if( cMapData_CheckNum.Find(sRegNo) != NULL )
	{
		return 1;
	}
	return 0;
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	EFS_END
	return -1;

}

// DM에 같은 등록번호가 존재하면 지워주는 함수
INT CBLRegNoFileProc::DeleteDuplicateRegNo()
{
	EFS_BEGIN

	INT ids;

	INT nRowCount = m_pTmpRegNoDM->GetRowCount();
	CString sRegNo;
	//=====================================================
	//2009.03.03 UPDATE BY PJW : 이중for문으로 데이타양이 많을 경우 시간 오래걸림 비교후 바로 삭제 하도록 수정
// 	CString sRegNo;
// 	
// 	for( INT i = nRowCount-1 ; i > -1 ; i-- )
// 	{
// 		ids = m_pTmpRegNoDM->GetCellData( _T("등록번호") , i , sRegNo );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DeleteDuplicateRegNo( CString sRegNo )") ); 
// 
// 		ids = CheckDuplicateRegNo( sRegNo );
// 		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DeleteDuplicateRegNo( CString sRegNo )") ); 
// 		if( ids > 1 ) m_pTmpRegNoDM->DeleteRow(i);
// 	}
	CString strCompareRegNo;
	m_pTmpRegNoDM->SORT(_T("등록번호"));
	
	for( INT i = nRowCount-1 ; i > 0 ; i-- )
	{
		ids = m_pTmpRegNoDM->GetCellData( _T("등록번호") , i , sRegNo );
		if( ids < 0 ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DeleteDuplicateRegNo( CString sRegNo )") ); 
		}
		ids = m_pTmpRegNoDM->GetCellData( _T("등록번호") , i-1 , strCompareRegNo );
		if( ids < 0 ) 
		{
			ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DeleteDuplicateRegNo( CString strCompareRegNo )") ); 
		}

		if(strCompareRegNo == sRegNo)
		{
			m_pTmpRegNoDM->DeleteRow(i);
		}
	}
	//=====================================================
	
	

	return 0;

	EFS_END
	return -1;

}

// 그리드 보여준다.
INT CBLRegNoFileProc::GridDisplay( BOOL bCheckCopy/* = TRUE */)
{
	EFS_BEGIN

	return 0;
		
	INT ids;
	
	// 그리드의 체크를 DM으로 이동
	if( bCheckCopy )
	{
		ids = MoveGridCheckToDM();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("GridDisplay()") ); 
	}	

	ids = m_pParentMgr->AllControlDisplay( m_sTmpRegNoCMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridDisplay()") ); 

	// 갯수 설정
	INT nCount = m_pTmpRegNoDM->GetRowCount();
	CString sCount;
	sCount.Format(_T("%d") , nCount );
	ids = m_pRegNoInfoCM->SetControlMgrData( _T("개수") , sCount );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("GridDisplay()") ); 

	return 0;

	EFS_END
	return -1;

}

// 그리드의 체크를 DM으로 이동
INT CBLRegNoFileProc::MoveGridCheckToDM()
{
	EFS_BEGIN

	INT ids;

	if( m_pTmpRegNoDM->GetRowCount() < 1 ) return 0;

	CESL_Grid *pGrid = (CESL_Grid*)(m_pTmpRegNoCM->FindControl( m_sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MoveGridCheckToDM()") );

	TCHAR *sDMColumnAlias[1] = { _T("선정") };
	INT nColIndex[1] = { -1 };
	ids = CLocCommonAPI::GetGridColumnIndexs( m_pParentMgr , pGrid , sDMColumnAlias , 1 , nColIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("ChangeBookCheckYN") );
	INT nCheckColIndex = nColIndex[0];

	CString sCheck;
	for( INT i = 0 ; i < pGrid->GetCount() ; i++ )
	{
		ids = pGrid->GetAt( i , nCheckColIndex , sCheck );
		if( ids < 0 )
		{
			CString sMsg;
			sMsg.Format(_T("i : %d , nCheckColIndex : %d , sCheck : %s") , i , nCheckColIndex , sCheck );
			AfxMessageBox( sMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("MoveGridCheckToDM()") ); 

		ids = m_pTmpRegNoDM->SetCellData( _T("선정") , sCheck , i );
		if( ids < 0 )
		{
			CString sMsg;
			sMsg.Format(_T("i : %d , nCheckColIndex : %d , sCheck : %s") , i , nCheckColIndex , sCheck );
			AfxMessageBox( sMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("MoveGridCheckToDM()") );
	}

	return 0;

	EFS_END
	return -1;

}

// 초기화
INT CBLRegNoFileProc::DMClear()
{
	EFS_BEGIN

	INT ids;

	// DM 삭제
	m_pTmpRegNoDM->FreeData();

	// 화면 리로드
	ids = GridDisplay(FALSE);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMClear()") ); 

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	cMapData_CheckNum.Clear();
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	return 0;

	EFS_END
	return -1;

}


// 삭제
// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
// 이함수 안쓰는 거 같음 , BO 쪽에서 직접 지는거 같다.
// KOL.RED.2018.201 ---------------------------------------------------------------------------
INT CBLRegNoFileProc::DMDeleteRow()
{
	EFS_BEGIN

	INT ids;
	
	ids = MoveGridCheckToDM();
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMDeleteRow()") ); 
	
	CString sCheck;
	INT nRowCount = m_pTmpRegNoDM->GetRowCount();
	for( INT i=nRowCount-1 ;i > -1 ; i-- )
	{
		ids = m_pTmpRegNoDM->GetCellData( _T("선정") , i , sCheck );
		if( ids < 0 && ids != -4012 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DMDeleteRow()") ); 

		if( sCheck.Compare( _T("V") ) == 0 ) 
			m_pTmpRegNoDM->DeleteRow(i);
	}

	ids = GridDisplay( FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMDeleteRow()") ); 

	// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
	DeleteGrid_UpdateHash();
	// KOL.RED.2018.201 ---------------------------------------------------------------------------

	return 0;

	EFS_END
	return -1;

}

// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
// 중복 체크용 해쉬 테이블을  다시 만든다.
void	CBLRegNoFileProc::DeleteGrid_UpdateHash()
{
	EFS_BEGIN
	INT ids;
	int i=0;
	cMapData_CheckNum.Clear();
	int	nRowCount = m_pTmpRegNoDM->GetRowCount();
	CString sRegNo;
	for( i = 0 ; i < nRowCount ; i++ )
	{
		ids = m_pTmpRegNoDM->GetCellData( _T("등록번호") , i , sRegNo );
		cMapData_CheckNum.Add( sRegNo );
	}
	EFS_END
}
// KOL.RED.2018.201 ---------------------------------------------------------------------------


// 파일에서 불러오기
INT CBLRegNoFileProc::DMLoadFile()
{
	EFS_BEGIN

	// DM 변경 - 김용배
	// m_pTmpRegNoDM
	
	INT ids = SanFileAPI::RegNoFileLoad( m_pTmpRegNoDM , m_pParentMgr );//, m_nCheckRegNo );
	
	if( ids == 100 ) return 1;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMLoadFile()") );

	// 2008.07.11 REMOVE BY PDW {{++
//	if( CHECK_DUPLICATEREGNO_IS_DONE != ids )
	// 2008.07.11 REMOVE BY PDW --}}
	{
		ids = DeleteDuplicateRegNo();
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("DMLoadFile()") );
	}	

	ids = GridDisplay( FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMLoadFile()") ); 

	return 0;

	EFS_END
	return -1;

}

// 파일에서 불러오기
INT CBLRegNoFileProc::DMLoadFile2()
{
	EFS_BEGIN

	// DM 변경 - 김용배
	// m_pTmpRegNoDM
	
	INT ids = SanFileAPI::RegNoFileLoad( m_pTmpRegNoDM , m_pParentMgr );//, m_nCheckRegNo );
	
	if( ids == 100 ) return 1;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMLoadFile()") );

	ids = GridDisplay( FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3 , _T("DMLoadFile()") ); 

	return 0;

	EFS_END
	return -1;
}

// KOL.RED.2018.201 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
//
// 파일로 등록번호 등록하기 
// bool check_dupl : 기존의 그리드는 유지한체 중복체크여부에 따라 추가할지 체크
// 위 함수 , DMLoadFile ,DMLoadFile2 를 대체한다.
INT CBLRegNoFileProc::DMLoadFile_201809( bool check_dupl )
{
	EFS_BEGIN

	CESL_DataMgr cDM;
	INT ids;
	ids = CLocCommonAPI::MakeCopyDM( m_pParentMgr , m_pTmpRegNoDM , &cDM );

	// KOL.RED.2018.204 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	//ids = SanFileAPI::RegNoFileLoad( &cDM , m_pParentMgr );//, m_nCheckRegNo );
	TCHAR TmpDir[1025];
	GetCurrentDirectory(1024,TmpDir);

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                    _T("엑셀 파일(*.xls)|.xls|한셀 통합 문서(*.cell)|.cell|모든 파일(*.*)|*.*||"), m_pParentMgr);

	INT result_dlg = dlg.DoModal(); 
	SetCurrentDirectory(TmpDir);
	if(result_dlg != IDOK) 
	{
		return 0;
	}

	CString strExt = dlg.GetFileExt();
	if (strExt.CompareNoCase(_T("XLS")) == 0 || strExt.CompareNoCase(_T("CELL")) == 0)
	{
		// BO_LOC_3390 참고.
		/////////////////////////////////////////////////////////////////////////////////
		// 1. 엑셀 파일을 연다
		/////////////////////////////////////////////////////////////////////////////////
		CRect rc;
		m_pParentMgr->GetWindowRect(rc);
		C_ExcelManControl ExcelCtrl;
		
		ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rc, m_pParentMgr, WM_USER + 10000);
		if (ExcelCtrl.m_hWnd == NULL) 
		{
			AfxMessageBox( _T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오.") );
			return 1;
		}

		CExcelManager ExcelMgr;
		ExcelMgr.SetManager(&ExcelCtrl);		
		if(ExcelMgr.OpenManager() == FALSE) 
		{
			AfxMessageBox( _T("NOT Installed Excelmanager.ocx or No Excel!!!") );
			return 2;
		}

		ExcelMgr.SetUseTemplate(TRUE, dlg.GetPathName() );
		if (ExcelMgr.OpenManager() == FALSE)
		{
			AfxMessageBox( _T("Open Failed") );
			return 3;
		}
			
		CString sValue = _T("");
		INT nRow = 0;
		//cDM.SetDataMgrRef( 0 , _T("등록번호") , _T("") , _T("")  , _T("")  , _T("") );
		// 3.2) 엑셀 파일의 끝은 행 전체가 비어 있거나 #EOF가 존재하는 행이다.

		CString log;
		while(TRUE)
		{	
			sValue = ExcelMgr.GetTextMatrix(nRow + 1, 1);
			if( _tcscmp(sValue, _T("#_TEOF")) == 0 )
			{
				break;
			}
			if( sValue.IsEmpty() )
			{
				break;
			}
			nRow = cDM.InsertRow( -1 );
			cDM.SetCellData(nRow, 0, sValue);
			log.Format( _T("excel file  read -> : %d %s \n") , nRow , sValue ) ;
			OutputDebugString( log );
		}

		ExcelMgr.CloseManager();

	}
	else
	{
		ids = SanFileAPI::RegNoFileLoad(&cDM,  m_pParentMgr , dlg.GetPathName());
	}

	// KOL.RED.2018.204 ---------------------------------------------------------------------------	

	if( ids == 100 ) return 1;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -10 , _T("DMLoadFile_201809()") );

	INT nRowCount = cDM.GetRowCount();
	CString sRegNo;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = cDM.GetCellData( _T("등록번호") , i , sRegNo );
		if( check_dupl )
		{
			if( cMapData_CheckNum.Find(sRegNo) != NULL )
			{
				continue;
			}
		}
		m_pTmpRegNoDM->InsertRow(-1);
		ids = m_pTmpRegNoDM->SetCellData( _T("등록번호") , sRegNo , m_pTmpRegNoDM->GetRowCount()-1 );
		cMapData_CheckNum.Add( sRegNo );
	}
	ids = GridDisplay( FALSE );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -20 , _T("DMLoadFile_201809()") ); 

	return 0;
	EFS_END
	return -1;
}
// KOL.RED.2018.201 ---------------------------------------------------------------------------


// DM에 있는 등록번호를 파일에 저장하는 함수
INT CBLRegNoFileProc::DMSaveFile()
{
	EFS_BEGIN

	INT ids;

	// KOL.RED.2018.204 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
	/*
	//=====================================================
	//2009.01.13 UPDATGE BY PJW : 파일 다이얼로그를 열때 파일명도 넘겨 준다.
// 	ids = SanFileAPI::RegNoFileSave( m_pTmpRegNoDM );
	CTime t = CTime::GetCurrentTime();
	CString strFileName;
 	strFileName.Format(_T("(장서관리)등록번호편집_%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	ids = SanFileAPI::RegNoFileSave( m_pTmpRegNoDM,strFileName );
	if( ids == 100 ) return 0;
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("DMLoadFile()") );
	//=====================================================
	*/
	ids = m_pTmpRegNoDM->ExportToExcelWithFileDialog(m_pParentMgr);
	// KOL.RED.2018.204 ---------------------------------------------------------------------------		

	return 0;

	EFS_END
	return -1;

}

// 부모 등록번호 DM에서 자식 등록번호 DM으로 카피하는 함수
INT CBLRegNoFileProc::OpenRegNoEdit()
{
	EFS_BEGIN

	INT ids;

	// 부모의 등록번호 DM의 값을 현재의 등록번호 DM값에 이동
	ids = m_pParentSpfWork->SPFExcutePath(OPEN_REGNO_EDIT);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("OnbtnFileSave()") );	

	ids = GridDisplay(FALSE);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("OnbtnFileSave()") );	

	return 0;

	EFS_END
	return -1;

}

// 자식 등록번호 DM에서 부모 등록번호 DM으로 카피하는 함수
INT CBLRegNoFileProc::SaveRegNoEdit()
{
	EFS_BEGIN

	INT ids;
	ids = m_pParentSpfWork->SPFExcutePath(SAVE_REGNO_EDIT);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd ,  -1 , _T("OnbtnFileSave()") );		

	return 0;

	EFS_END
	return -1;

}

VOID CBLRegNoFileProc::SetCheckRegNoOption( INT flag )
{
	m_nCheckRegNo = flag;
}
