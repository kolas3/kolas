// RFIDTaggingDlg.cpp: implementation of the CRFIDTaggingDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RFIDTaggingDlg.h"
#include "RFID_Interface.h"
#include "CharConvert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRFIDTaggingDlg::CRFIDTaggingDlg(CWnd* pParent /*=NULL*/)	
	: CDialog(CRFIDTaggingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRFIDTaggingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	BOOK_LIST.RemoveAll();
	m_bStartUp = FALSE;
	m_nIndex = -1;

	m_pBookList = NULL;
	m_nBookListRowHeight = 35;
	m_nBookListColWidth = 160;

	m_pBookInfo = NULL;
	m_nBookInfoColWidth1 = 100;
	m_nBookInfoColWidth2 = 200;
	m_nBookInfoRowHeight1 = 35;
	m_nBookInfoRowHeight2 = 17;

	m_pOwnerWnd = NULL;
	m_fpEventHandlerBefore = NULL;
	m_fpEventHandlerAfter = NULL;

	m_nTaggingTotalCnt = 0;	
	m_bDelay = FALSE;

	m_pCodeMgr = NULL;
	m_bIsAlreadySave = FALSE;

}


BOOL CRFIDTaggingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_pBookList = new CESL_Grid(NULL);
	m_pBookList->SubclassDlgItem(IDC_GRID_BOOK_LIST, this);
	m_pBookInfo = new CESL_Grid(NULL);
	m_pBookInfo->SubclassDlgItem(IDC_GRID_BOOK_INFO, this);

	InitBookList();
	InitBookInfo();
	
	CWinApp* pApp = AfxGetApp();
	UINT nData = pApp->GetProfileInt(_T("Style"), _T("Value"), 0);	
	if ( nData == 1 ) m_bDelay = TRUE;
		
	CRFID_Interface RFID;

	if (RFID.GET_STATUS() != 0) {
		m_bStartUp = FALSE;
		SetTimer(1, 300, NULL);
	} else {
		m_bStartUp = TRUE;
	}

	if ( 0 < this->GetBookListCount() )
	{		
		POSITION       pos   = BOOK_LIST.GetHeadPosition();
		CRFIDBookInfo* pBook = NULL;

		for ( INT i = 0;i < this->GetBookListCount();i++ )
		{
			pBook = (CRFIDBookInfo*)BOOK_LIST.GetNext(pos);

			if ( pBook )
			{
				if ( 0 > this->GetBookInfoFromDB( pBook->strAccessionNo , pBook ) )
				{
					pBook->strTemp = _T("");
				}
			}
		}		
	}

	ShowBookList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CRFIDTaggingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRFIDTaggingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CRFIDTaggingDlg::DestroyWindow() 
{
		
	FreeBookList();

	if ( m_pBookList ) 
	{
		delete m_pBookList;
		m_pBookList = NULL;
	}

	if ( m_pBookInfo ) 
	{
		delete m_pBookInfo;
		m_pBookInfo = NULL;
	}

	return CDialog::DestroyWindow();
}

BEGIN_MESSAGE_MAP(CRFIDTaggingDlg, CDialog)
	//{{AFX_MSG_MAP(CRFIDTaggingDlg)
	ON_BN_CLICKED(IDC_btnSave, OnbtnSAVE)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbtnCLOSE)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CRFIDTaggingDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CRFIDTaggingDlg)
	ON_EVENT(CRFIDTaggingDlg, IDC_GRID_BOOK_LIST, -600 /* Click */, OnClickGridBookList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRFIDTaggingDlg message handlers

VOID CRFIDTaggingDlg::OnClickGridBookList() 
{
	INT nRow = m_pBookList->GetRow()-1;
	m_nIndex = nRow;
	ShowBookInfo();
}
VOID CRFIDTaggingDlg::OnbtnCLOSE() 
{
	CDialog::OnCancel();
}

VOID CRFIDTaggingDlg::OnbtnSAVE()
{
	if(TRUE == m_bIsAlreadySave) return;

	m_bIsAlreadySave = TRUE;

	CRFID_Interface RFID;

	SAVE(NULL);

	m_nTaggingTotalCnt++;

	m_bIsAlreadySave = FALSE;
}

VOID CRFIDTaggingDlg::FreeBookList(VOID)
{
	INT             nData;
	CRFIDBookInfo * pData;

	nData = BOOK_LIST.GetCount();
	if ( !nData ) return;
	
	for (INT i = 0; i < nData; i++) 
	{
		pData = (CRFIDBookInfo*)BOOK_LIST.RemoveHead();
		if (pData) delete pData;
	}
	BOOK_LIST.RemoveAll();	
	
	return;
}

INT CRFIDTaggingDlg::SAVE(TCHAR *pSerial) 
{
	INT			   ids;
	TCHAR          szSerial[32];
	CString        strMessage;
	POSITION       pos;
	CRFIDBookInfo* pBook;
	INT            ids_before;
	INT            ids_after;

	if ( _T("불가") == m_pBookList->GetTextMatrix(m_nIndex+1, 1) )
	{
		AfxMessageBox( _T("RFID에 입력할 수 없는 자료입니다."), MB_ICONSTOP );		
		return -4;
	}

	if ( !pSerial ) 
	{		
		if ( !GetFirstTagSerial(szSerial) ) 
			return -1;
	} 
	else 
	{
		_stprintf(szSerial, _T("%s\0"), pSerial);
	}
	
	strMessage.Format(_T("Tag[%s] 작업 中"), szSerial);
	ShowMessage(strMessage);

	pos = BOOK_LIST.FindIndex(m_nIndex);
	if ( !pos ) return -2;
	pBook = (CRFIDBookInfo*)BOOK_LIST.GetAt(pos);
	if ( !pBook ) return -3;

	if( pBook->strRFIDSerialNo.GetLength() && 0 != _tcscmp( szSerial, pBook->strRFIDSerialNo ) )
	{
		CString strMessage;
		strMessage.Format(_T("RFID_SERIAL이 [%s]인 태그에 저장된 데이터입니다.")
			              _T(" 그래도 저장하시겠습니까?\r\n\r\n")
						  _T("RFID 리더기 위의 시리얼[%s]"), pBook->strRFIDSerialNo, szSerial);
		if ( IDNO == AfxMessageBox( strMessage, MB_YESNO ) )
		{
			ShowMessage(_T("취소되었습니다."));
			return 0;	
		}
	}
	
	ShowMessage(_T("RFID Serial을 DB에 저장중입니다."));
 	ids = SaveRFIDSerialToBook(pBook, szSerial);		
	if(-1 > ids)
	{
		ShowMessage(_T("DB저장에 실패하였습니다."));
		return ids;
	}
	if(0 < ids)
	{
		ShowMessage(_T("취소되었습니다."));
		return ids;
	}
	ShowMessage(_T("RFID Serial을 DB에 저장하였습니다."));
	pBook->strRFIDSerialNo.Format(_T("%s"), szSerial);

	ids_before = RFIDSaveEventBeforeProc(pBook, ids);

	if (ids_before < 0) {
		AfxMessageBox( _T("Tag 저장 이전작업에서 에러"), MB_ICONSTOP );
		ShowMessage(_T("RFID에 입력이 실패하였습니다."));
		return ids_before;
	}

	ids = SaveBookInfoToRFID(pBook);
	if (ids == 0) 
	{
		ShowMessage(_T("RFID에 성공적으로 저장하였습니다."));
		m_pBookList->SetTextMatrix(m_nIndex+1, 1, _T("저장"));
		ShowBookInfo();
	}
	else
	{
		ShowMessage(_T("RFID에 입력이 실패하였습니다."));
		m_pBookList->SetTextMatrix(m_nIndex+1, 1, _T("실패"));
	}

	ids_after = RFIDSaveEventAfterProc(pBook, ids);

	return ids_after;
}

BOOL CRFIDTaggingDlg::GetFirstTagSerial(TCHAR* pSerialNo)
{
	INT				ids;
	CRFID_Interface RFID;
	INT				nTagCount = 0;
	CStringArray    DSFID_LIST;
	CStringArray    SERIAL_LIST;
	CString			strErrorMsg;


	if ( 0 != ( ids = RFID.GetTagSerial(SERIAL_LIST) ) )
	{
		ShowMessage( _T("책을 리더기에 올려 주십시오.") );
		return FALSE;
	}
	
	if ( 0 == ( nTagCount = SERIAL_LIST.GetSize() ) ) 
	{
		ShowMessage( _T("책을 리더기에 올려 주십시오.") );
		return FALSE;
	}
	else if ( 1 < nTagCount ) 
	{
		strErrorMsg.Format(_T("리더기에 책을 한권만 올려 주십시오.[%d]"), nTagCount);
		AfxMessageBox(strErrorMsg);
		ShowMessage( strErrorMsg ); 
		return FALSE;
	}

	_tcscpy( pSerialNo, SERIAL_LIST.GetAt(0) );
	return TRUE;
}

INT CRFIDTaggingDlg::SaveRFIDSerialToBook(CRFIDBookInfo *pBook, TCHAR *pNewSerial)
{
	int ids = 0;
	if (pBook == NULL) return -1010;
	if (pNewSerial == NULL) return -1020;

	CString strTblName;
	if (pBook->strBoSe == _T("S") || pBook->strBoSe == _T("s"))
	{
		strTblName = _T("SE_BOOK_TBL");
	}
	else
	{
		strTblName = _T("BO_BOOK_TBL");
	}


	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(CONNECTION_INFO);
	TmpDM.CheckManageCode();

	CString strQuery;
	strQuery.Format( 
		_T("SELECT BB.MANAGE_CODE, '단행테이블 : ' || IB.TITLE_INFO || '(' || BB.REG_NO || ')' , NULL FROM BO_BOOK_TBL BB, IDX_BO_TBL IB WHERE BB.SPECIES_KEY = IB.REC_KEY AND BB.WORKNO = '%s' AND BB.REC_KEY <> %s ")
		_T("UNION ")
		_T("SELECT SB.MANAGE_CODE, '연속테이블 : ' || SS.TITLE_INFO || '(' || SB.REG_NO || ')' , NULL FROM SE_BOOK_TBL SB, IDX_SE_TBL SS WHERE SB.SPECIES_KEY = SS.REC_KEY AND SB.WORKNO = '%s' AND SB.REC_KEY <> %s ")
		_T("UNION ")
		_T("SELECT CLU.MANAGE_CODE, '이용자테이블 : ' || CLU.NAME || '(' || CLU.USER_NO || ')' , CLU.WORKNO FROM CO_LOAN_USER_TBL CLU WHERE CLU.WORKNO = '%s' ")
		,pNewSerial, pBook->strBookKey, pNewSerial, pBook->strBookKey, pNewSerial
		);
	TmpDM.RestructDataManager(strQuery);

	int nRowCnt = 0;	
	const INT ROW_COUNT = TmpDM.GetRowCount();
	const INT COL_CNT = 3;
		
	CString strSerial;
	CString strManageCode;
	CString strSerialList;

	TmpDM.StartFrame();
	if(0 < ROW_COUNT)
	{
		for(nRowCnt = 0; nRowCnt < ROW_COUNT; nRowCnt++)
		{
			ids = TmpDM.GetCellData(nRowCnt, 0, strManageCode);
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1010 , _T("SaveRFIDSerialToBook") );
			}

			ids = TmpDM.GetCellData(nRowCnt, 1, strSerial);
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1020 , _T("SaveRFIDSerialToBook") );
			}

			if(strManageCode != TmpDM.MANAGE_CODE)
			{
				CString strMsg;
				strMsg.Format(_T("이 태그는 다른 도서관에서 등록된 태그이기 때문에 기록할 수 없습니다."));
				AfxMessageBox(strMsg);

				TmpDM.EndFrame();
				return 1010;
			}
			
			strSerialList = strSerialList + _T("\r\n") + strSerial;
		}

		CString strMsg;
		strMsg.Format(_T("RFID_SERIAL 이 중복되는 데이타가 발견되었습니다. 그래도 저장하시겠습니까? %s"), strSerialList);

		if (IDYES == AfxMessageBox(strMsg, MB_YESNO)) 
		{
			strQuery.Format(
				_T("UPDATE BO_BOOK_TBL SET WORKNO = NULL WHERE WORKNO='%s' AND REC_KEY<>%s;"), 
				pNewSerial, pBook->strBookKey);
			TmpDM.AddFrame(strQuery);

			strQuery.Format(
				_T("UPDATE SE_BOOK_TBL SET WORKNO = NULL WHERE WORKNO='%s' AND REC_KEY<>%s;"), 
				pNewSerial, pBook->strBookKey);
			TmpDM.AddFrame(strQuery);
			
			CString strWorkNo;
			ids = TmpDM.GetCellData( 0 , 2 , strWorkNo );
			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1030 , _T("SaveRFIDSerialToBook") );
			}

			if( 0 > ids )
			{
				ERROR_MSG_RETURN_ERROR_CODE2( -1040 , _T("SaveRFIDSerialToBook") );
			}

			if( pNewSerial == strWorkNo )
			{
				strQuery.Format( _T("UPDATE CO_LOAN_USER_TBL SET WORKNO = NULL , CARD_PASSWORD = NULL WHERE WORKNO = '%s';") , strWorkNo );
				TmpDM.AddFrame(strQuery);
			}
		}
		else
		{
			TmpDM.EndFrame();
			return 1020;
		}
	}

	strQuery.Format(_T("UPDATE %s SET WORKNO='%s' WHERE REC_KEY=%s;"), strTblName, pNewSerial, pBook->strBookKey);
	TmpDM.AddFrame(strQuery);

	ids = TmpDM.SendFrame();
	TmpDM.EndFrame();

	if (ids < 0)
	{		
		return -1000;
	}
	
	return 0;
}

// 
INT CRFIDTaggingDlg::SaveBookInfoToRFID(CRFIDBookInfo *pBook)
{
	INT				ids;
	CRFID_Interface RFID;
	TAGDATASTRU		pTagStru;
	CString			strErrorMsg;
	CString			strRegCode=_T(""), strRegSerial=_T("");


	INT nRegCodeLength;
	INT nFind;
	CStdioFile file;
	CString strLine;
	CString strRegCodeLength;
	if (file.Open(_T("..\\Cfg\\RegCodeLength.TXT"), CFile::modeRead|CFile::typeBinary))
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();

			nFind = strLine.Find(_T("등록구분코드자리수"));
			if (nFind < 0) continue;
			nFind = strLine.Find(_T("|"));

			strRegCodeLength = strLine.Mid(nFind+1);
			strRegCodeLength.TrimLeft(); strRegCodeLength.TrimRight();
			nRegCodeLength = _ttoi(strRegCodeLength);
		}
	}

 	if (pBook->strAccessionNo.GetLength() > nRegCodeLength ) {
 		strRegCode = pBook->strAccessionNo.Left(nRegCodeLength);
 		strRegSerial = pBook->strAccessionNo.Mid(nRegCodeLength);
 	}

	memset( &pTagStru, 0x00, sizeof(TAGDATASTRU) );
	
	CharConvert cv;
	char*       szTmp = NULL;
	INT         len   = 0;

	cv.UnicodeToMultiByte( pBook->strRFIDSerialNo.GetBuffer(0), &szTmp, &len );
	if( 0 < len)
	{
		strcpy((char*)pTagStru.strSerial ,szTmp);
		delete [] szTmp;		
	}
		
	if( !pBook->strLoan.GetLength() ) pBook->strLoan = _T("0");
	if( !pBook->strSecurity.GetLength() ) pBook->strSecurity = _T("1");
		
	cv.UnicodeToMultiByte( pBook->strLibCode.GetBuffer(0), &szTmp, &len );
	if( 0 < len )
	{
		strcpy((char*)pTagStru.strLibCode ,szTmp);
		delete [] szTmp;
	}

	cv.UnicodeToMultiByte( strRegCode.GetBuffer(0), &szTmp, &len );
	if( 0 < len )
	{
		strcpy((char*)pTagStru.strRegCode ,szTmp);
		delete [] szTmp;
	}

	cv.UnicodeToMultiByte( strRegSerial.GetBuffer(0), &szTmp, &len );
	if( 0 < len )
	{
		strcpy((char*)pTagStru.strRegNo ,szTmp);
		delete [] szTmp;
	}
		
	ShowMessage(_T("RFID에 저장중입니다."));
	for(int nRetry = 0; nRetry < 3; nRetry++)
	{
		ids = RFID.PutTagAllDataByUID( &pTagStru );
		if(0 == ids) break;
	}
	if (ids)
	{
		CString str;
		str.Format(_T("책에 RFID 정보를 저장하던중 에러가 발생하였습니다.[%d]"), ids);
		AfxMessageBox( str, MB_ICONSTOP );
		return -1;
	}
	return ids;
}

VOID CRFIDTaggingDlg::ShowMessage(TCHAR *szMsg)
{
	ShowMessage( CString( szMsg ) );

	return;
}

VOID CRFIDTaggingDlg::ShowMessage(CString strMsg)
{
	CWnd* pWnd = GetDlgItem(IDC_STATIC_RFID_MSG);
	if (pWnd) 
		pWnd->SetWindowText(strMsg);
	return;
}

VOID CRFIDTaggingDlg::ShowBookInfo()
{

	POSITION        pos;
	CRFIDBookInfo * pBook;

	InitBookInfo();

	if ( 0 > m_nIndex || GetBookListCount() <= m_nIndex ) return;

	pos = BOOK_LIST.FindIndex(m_nIndex);
	if ( !pos ) return;
	pBook = (CRFIDBookInfo*)BOOK_LIST.GetAt(pos);
	if ( !pBook ) return;
	
	m_pBookInfo->SetTextMatrix(0, 1, pBook->strTitle);
	m_pBookInfo->SetTextMatrix(1, 1, pBook->strAccessionNo);
	m_pBookInfo->SetTextMatrix(2, 1, pBook->strAuthor);
	m_pBookInfo->SetTextMatrix(3, 1, pBook->strPublisher);
	m_pBookInfo->SetTextMatrix(4, 1, pBook->strRFIDSerialNo);
	
	return;
}

VOID CRFIDTaggingDlg::InitBookInfo()
{
	INT nRow = 0;

	m_pBookInfo->m_bFroceReverse = FALSE;
	m_pBookInfo->RemoveAll();
	m_pBookInfo->SetFixedRows(0);
	m_pBookInfo->SetFixedCols(0);
	m_pBookInfo->SetRows(6);
	m_pBookInfo->SetCols(0, 2);
	m_pBookInfo->SetColWidth(0, 0, m_nBookInfoColWidth1 * 15);
	m_pBookInfo->SetColWidth(1, 0, m_nBookInfoColWidth2 * 15);
	m_pBookInfo->SetRowHeight(-1, m_nBookInfoRowHeight2 * 15);
	m_pBookInfo->SetRowHeight(0, m_nBookInfoRowHeight1 * 15);

	COLORREF tag_bk1  = RGB(242, 222, 176);
	COLORREF tag_fk1  = RGB(0, 0, 0);
	COLORREF data_bk1 = RGB(237, 237, 237);
	COLORREF data_fk1 = RGB(0, 0, 0);

	COLORREF tag_bk2  = RGB(247, 238, 218);
	COLORREF tag_fk2  = RGB(0, 0, 0);
	COLORREF data_bk2 = RGB(255, 255, 255);
	COLORREF data_fk2 = RGB(0, 0, 0);

	nRow = 0;
	SetGridCellColor(m_pBookInfo, nRow, 0, tag_bk1, tag_fk1);	SetGridCellColor(m_pBookInfo, nRow, 1, data_bk1, data_fk1);
	m_pBookInfo->SetTextMatrix(nRow, 0, _T("서명"));

	nRow = 1;
	SetGridCellColor(m_pBookInfo, nRow, 0, tag_bk2, tag_fk2);	SetGridCellColor(m_pBookInfo, nRow, 1, data_bk2, data_fk2);
	m_pBookInfo->SetTextMatrix(nRow, 0, _T("등록번호"));

	nRow = 2;
	SetGridCellColor(m_pBookInfo, nRow, 0, tag_bk1, tag_fk1);	SetGridCellColor(m_pBookInfo, nRow, 1, data_bk1, data_fk1);
	m_pBookInfo->SetTextMatrix(nRow, 0, _T("저자"));

	nRow = 3;
	SetGridCellColor(m_pBookInfo, nRow, 0, tag_bk2, tag_fk2);	SetGridCellColor(m_pBookInfo, nRow, 1, data_bk2, data_fk2);
	m_pBookInfo->SetTextMatrix(nRow, 0, _T("출판사"));

	nRow = 4;
	SetGridCellColor(m_pBookInfo, nRow, 0, tag_bk1, tag_fk1);	SetGridCellColor(m_pBookInfo, nRow, 1, data_bk1, data_fk1);
	m_pBookInfo->SetTextMatrix(nRow, 0, _T("BOOK RFID"));

	nRow = 5;
	SetGridCellColor(m_pBookInfo, nRow, 0, tag_bk2, tag_fk2);	SetGridCellColor(m_pBookInfo, nRow, 1, data_bk2, data_fk2);
	m_pBookInfo->SetTextMatrix(nRow, 0, _T("READER RFID"));

	m_pBookInfo->SetGridLines(0);

	return;
}

VOID CRFIDTaggingDlg::InitBookList()
{
	m_pBookList->RemoveAll();
	m_pBookList->SetFixedRows(1);
	m_pBookList->SetFixedCols(1);
	m_pBookList->SetRows(2);
	m_pBookList->SetCols(0, 3);
	m_pBookList->SetColWidth(0, 0, 30*15);
	m_pBookList->SetColWidth(1, 0, 30*15);
	m_pBookList->SetTextMatrix(0, 1, _T("상태"));
	m_pBookList->SetTextMatrix(0, 2, _T("정보"));
	return;
}

VOID CRFIDTaggingDlg::SetGridCellColor(CESL_Grid *pGrid, INT nRow, INT nCol, COLORREF bk, COLORREF fk)
{
	if ( pGrid )
	{
		pGrid->SetRow(nRow);
		pGrid->SetCol(nCol);
		pGrid->SetCellBackColor(bk);
		pGrid->SetCellForeColor(fk);
	}

	return;
}

VOID CRFIDTaggingDlg::ShowBookList(VOID)
{
	INT             nRows;
	POSITION        pos;
	CRFIDBookInfo * pBook;
	CString         strText;
	COLORREF tag_bk1  = RGB(255, 255, 255);
	COLORREF tag_fk1  = RGB(0, 0, 0);
	InitBookList();

	nRows = GetBookListCount();
	if ( !nRows ) return;

	m_pBookList->SetRows( nRows + 1 );
	pos = BOOK_LIST.GetHeadPosition();
	
	for ( INT i = 0;i < nRows;i++ ) 
	{
		strText.Format( _T("%d"), i + 1 );
		m_pBookList->SetTextMatrix( i + 1, 0, strText );	

		pBook = NULL;
		pBook = (CRFIDBookInfo*)BOOK_LIST.GetNext(pos);		

		if( pBook->strRFIDSerialNo.GetLength() )
		{
			m_pBookList->SetTextMatrix( i + 1, 1, _T("저장") );
		}
		else
		{
			m_pBookList->SetTextMatrix( i + 1, 1, _T("대기") );
		}
		SetGridCellColor(m_pBookList, i+1, 1, tag_bk1, tag_fk1);

		if ( !pBook ) continue;
		if ( !pBook->strAccessionNo.GetLength() ) 
		{
			strText.Format( _T("%s"), pBook->strTitle );
		} else 
		{
			strText.Format( _T("등록번호:%15s %s"), pBook->strAccessionNo, pBook->strTitle );
		}

		m_pBookList->SetTextMatrix( i+1, 2, strText );
		SetGridCellColor(m_pBookList, i+1, 2, tag_bk1, tag_fk1);
	}
	m_pBookList->SetRowHeight( -1, m_nBookListRowHeight * 15 );
	m_pBookList->SetRowHeight( 0, 17 * 15 );
	m_pBookList->SetColWidth( 2, 0, m_nBookListColWidth * 15 );

	m_pBookList->SetRow( 1 );
	m_pBookList->SetCol( 2 );
	m_pBookList->SetReverse( 0 );

	m_nIndex = 0;

	ShowBookInfo();
	return;
}

INT CRFIDTaggingDlg::GetBookInfoFromDB(CString strREG_NO, CRFIDBookInfo* pBook/*=NULL*/)
{
	INT		     ids;	
	CString      strQuery;
	CString      strErrorMsg;
	CString      strBookCount;	
	CString      strPublishForm = _T("N");
	CESL_DataMgr DM;

	DM.SetCONNECTION_INFO( CONNECTION_INFO );

	strQuery.Format( _T("SELECT COUNT(*) FROM BO_BOOK_TBL ")
					 _T("WHERE REG_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strREG_NO );

	ids = DM.GetOneValueQuery( strQuery, strBookCount );
	strBookCount.TrimLeft();strBookCount.TrimRight();
	if ( 1 == _ttoi( strBookCount ) )
	{
		strPublishForm = _T("B");
	}
	else if ( 1 < _ttoi( strBookCount ) )
	{
		strErrorMsg.Format( _T("해당 등록번호('%s')를 가진 책이 단행에서 [%d]권이 발견되었습니다."), strREG_NO, _ttoi( strBookCount ) );
		AfxMessageBox( strErrorMsg );
		return -2;
	}

	strQuery.Format( _T("SELECT COUNT(*) FROM SE_BOOK_TBL ")
					 _T("WHERE REG_NO='%s' AND MANAGE_CODE=UDF_MANAGE_CODE"), strREG_NO );
	ids = DM.GetOneValueQuery( strQuery, strBookCount );
	strBookCount.TrimLeft();strBookCount.TrimRight();
	if ( 1 == _ttoi( strBookCount ) )
	{
		strPublishForm = _T("S");
	}
	else if ( 1 < _ttoi( strBookCount ) ) 
	{
		strErrorMsg.Format( _T("해당 등록번호('%s')를 가진 책이 연속에서 [%d]권이 발견되었습니다."), strREG_NO, _ttoi( strBookCount ) );
		AfxMessageBox( strErrorMsg );
		return -2;
	}

	if ( _T("B") != strPublishForm && _T("S") != strPublishForm ) 
	{
		strErrorMsg.Format( _T("등록되지 않은 등록 번호 입니다.") );
		AfxMessageBox( strErrorMsg );
		return -1;
	}

	if( _T("B") == strPublishForm )
	{                       
		strQuery.Format(_T("SELECT BB.REC_KEY, ")
		                _T("BB.REG_CODE, ")		
						_T("BB.REG_NO, ")		
						_T("BB.SHELF_LOC_CODE, ")
						_T("BB.WORKING_STATUS, ")
						_T("BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE, ")
						_T("BB.AUTHOR, ")		
						_T("BB.SPECIES_KEY, ")	
						_T("IB.TITLE, ")		
						_T("IB.PUBLISHER, ")	
						_T("BB.WORKNO, ")		
						_T("BB.EPC,	")			
						_T("BB.EA_ISBN, ")		
						_T("BS.CONTROL_NO, ")	
						_T("BS.MARC, ")			
						_T("PUBLISH_YEAR ")		
						_T("FROM IDX_BO_TBL IB, BO_SPECIES_TBL BS, BO_BOOK_TBL BB ")
						_T("WHERE IB.REC_KEY = BS.REC_KEY ")
						_T("AND BS.REC_KEY = BB.SPECIES_KEY ")
						_T("AND BB.REG_NO='%s' ")
						_T("AND IB.MANAGE_CODE=UDF_MANAGE_CODE"), strREG_NO );
	}
	else if( _T("S") == strPublishForm )
	{
		strQuery.Format(_T("SELECT BB.REC_KEY, ")
			            _T("BB.REG_CODE, ")		
						_T("BB.REG_NO, ")		
						_T("BB.SHELF_LOC_CODE, ")
						_T("BB.WORKING_STATUS, ")
						_T("BB.SEPARATE_SHELF_CODE, BB.CLASS_NO, BB.BOOK_CODE, BB.VOL_CODE, BB.COPY_CODE, ")
						_T("'', ")				
						_T("BB.SPECIES_KEY, ")	
						_T("IB.TITLE, ")		
						_T("IB.PUBLISHER, ")	
						_T("BB.WORKNO, ")		
						_T("'', ")				
						_T("IB.ST_ISSN, ")		
						_T("'', ")				
						_T("'', ")				
						_T("IB.PUB_YEAR ")		
						_T("FROM IDX_SE_TBL IB , SE_BOOK_TBL BB ")
						_T("WHERE IB.REC_KEY = BB.SPECIES_KEY ")
						_T("AND BB.REG_NO='%s' ")
						_T("AND IB.MANAGE_CODE=UDF_MANAGE_CODE"), strREG_NO ); 
	}
	
	ids = DM.RestructDataManager( strQuery );
	if ( ids ) 
	{
		strErrorMsg.Format( _T("책정보를 읽어오는데 실패하였습니다.") );
		AfxMessageBox( strErrorMsg );
		return -4;
	}

	pBook->strLocale       = _T("01");
	pBook->strLibCode      = LIB_CODE;
	pBook->strRegCode      = DM.GetCellData(0, 1);
	pBook->strRegNo        = DM.GetCellData(0, 2);
	pBook->strAccessionNo  = DM.GetCellData(0, 2);
	pBook->strShelfLocCode = DM.GetCellData(0, 3);
	pBook->strBoSe         = strPublishForm;
	GetLoanSecurityStatus(DM.GetCellData(0, 4), pBook);
	pBook->strControlNo	   = DM.GetCellData(0, 17);
	pBook->strAuthor       = DM.GetCellData(0, 10);
	pBook->strTitle		   = DM.GetCellData(0, 12);
	pBook->strPublisher    = DM.GetCellData(0, 13);
	pBook->strRFIDSerialNo = DM.GetCellData(0, 14);
	pBook->strBookKey      = DM.GetCellData(0, 0);	
	pBook->strSpeciesKey   = DM.GetCellData(0, 11);
	pBook->strTemp		   = strREG_NO;
	return 0;
}

INT CRFIDTaggingDlg::GetLoanSecurityStatus(CString strWORKING_STATUS, CRFIDBookInfo *pBookInfo)
{
	if ( !pBookInfo ) return -1;
	
	strWORKING_STATUS.TrimLeft();			
	strWORKING_STATUS.TrimRight();
	strWORKING_STATUS.MakeUpper();

	if( 
		(strWORKING_STATUS.Compare(_T("BOL211O")) == 0 ) || 
		(strWORKING_STATUS.Compare(_T("BOL212O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("BOL213O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("BOL214O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("BOL215O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("BOL113O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("BOL411O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("BOL511O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL213O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL311O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL312O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL411O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL511O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL313O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL314O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL315O")) == 0 ) 
		|| 
		(strWORKING_STATUS.Compare(_T("SEL317O")) == 0 ) ||
		(strWORKING_STATUS.Compare(_T("SEL318O")) == 0 ) 
	)
	{
		pBookInfo->strLoan     = _T("1");
		pBookInfo->strSecurity = _T("0");
	}
	else
	{
		pBookInfo->strLoan     = _T("0");
		pBookInfo->strSecurity = _T("1");		
	}

	return 0;
}

INT CRFIDTaggingDlg::GetBookListCount()
{
	return BOOK_LIST.GetCount();
}

INT CRFIDTaggingDlg::RFIDSaveEventBeforeProc(CRFIDBookInfo *pBook, INT nIds)
{
	if (m_pOwnerWnd == NULL) return 0;
	if (m_fpEventHandlerBefore == NULL) return 0;
	INT ids = ( m_pOwnerWnd->*m_fpEventHandlerBefore )(pBook, nIds);
	return ids;
}

INT CRFIDTaggingDlg::RFIDSaveEventAfterProc(CRFIDBookInfo *pBook, INT nIds)
{
	if (m_pOwnerWnd == NULL) return 0;
	if (m_fpEventHandlerAfter == NULL) return 0;
	INT ids = ( m_pOwnerWnd->*m_fpEventHandlerAfter )(pBook, nIds);
	return ids;
}

INT CRFIDTaggingDlg::GetLibCode(CString strUSER_KEY, CString& strLIB_CODE)
{

	INT ids;
	CESL_DataMgr DM;
	CString strQuery;
	CString strTmp;
	CString strManageCode;
	CString strResult;

	DM.SetCONNECTION_INFO( CONNECTION_INFO );


	strQuery.Format( _T("SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE") );
	ids = DM.GetOneValueQuery( strQuery, strResult );	
	strResult.TrimLeft();strResult.TrimRight();
	strLIB_CODE = strResult;
	
	return 0;
}

CRFIDBookInfo * CRFIDTaggingDlg::AddBook(VOID)
{
	CRFIDBookInfo* pData = new CRFIDBookInfo;
	if ( !pData ) return NULL;
	BOOK_LIST.AddTail((CObject*)pData);
		
	return pData;	
}
