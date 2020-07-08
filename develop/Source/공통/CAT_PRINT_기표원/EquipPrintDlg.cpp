// EquipPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EquipPrintDlg.h"
#include "..\MarcAdjustApi\MarcAdjustConstant.h"
#include "EquipPrintPropertyDlg.h"

#include "..\..\SmartID\BarCodeToRFID2\SmartIDScannerDlg.h"
#include "..\..\SmartID\BarCodeToRFID2\BarcodeToRFIDDlg.h"
#include "..\K2UP_Common\LibInfoMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintDlg dialog


CEquipPrintDlg::CEquipPrintDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CEquipPrintDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nModeMONO_SERIAL = MODE_MONO;

	m_pDM_Parent = NULL;
	m_pGrid_Parent = NULL;

	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
}

BOOL CEquipPrintDlg::Create(CWnd *pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}


VOID CEquipPrintDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquipPrintDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CEquipPrintDlg, CDialog)
	//{{AFX_MSG_MAP(CEquipPrintDlg)
	ON_BN_CLICKED(IDC_bCHANGE_PRINT_FORMAT, OnbCHANGEPRINTFORMAT)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP, OnbPRINTEQUIP)
	ON_WM_SIZE()	
	ON_BN_CLICKED(IDC_bRfidWrite, OnbRfidWrite)
	ON_BN_CLICKED(IDC_bDeleteDm, OnbDeleteDm)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDIT_CALLNO, OnKillfocusEditCallno)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintDlg Operations

INT CEquipPrintDlg::Init()
{
	EFS_BEGIN

	if (!m_pDM_Parent || !m_pGrid_Parent|| m_strSpeciesKeyAlias.IsEmpty()) return -1;
	
	// 멤버 정보 세팅
	m_pDM = FindDM(_T("DM_정리_출력_장비"));
	m_pCM = FindCM(_T("CM_정리_출력_장비"));
	m_pGrid = reinterpret_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID_LIST")));

	if (!m_pDM || !m_pCM || !m_pGrid) return -1;

	m_pGrid->SetProtectSelectColumn(TRUE);
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	// 선정 리스트 체크한다.
	INT ids = -1;
	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0) return ids;

	INT nCount = m_pGrid_Parent->SelectGetCount();
	if (nCount <= 0)
	{
		if ( m_arrStrSpeciesKey.GetSize() < 1 ) {
			m_strLastErrMsg = _T("선정된 자료가 없습니다.");
			return -1;
		}
	}

	// 권책기호
	CString sQuery = _T("");
	CString sValue = _T("") ;

	sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '정리' AND GROUP_3 = '공통' AND CLASS_ALIAS = '레이블권책기호설정'");
	m_pDM->GetOneValueQuery( sQuery , sValue );	
	sValue.TrimLeft(); sValue.TrimRight();
	m_strVolCode = sValue;

	// 선정 리스트를 돌면서 종키를 가져온다.
	CStringArray arrStrSpeciesKey;

	INT nIdx = m_pGrid_Parent->SelectGetHeadPosition();
	//if (nIdx < 0) return -1;
	if (nIdx < 0 && m_arrStrSpeciesKey.GetSize() < 1) return -1;

	CString strSpeciesKey;
	while (nIdx >= 0)
	{
		// DM에서 종키를 가져온다.
		m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, nIdx, strSpeciesKey);
		if (!strSpeciesKey.IsEmpty())
			arrStrSpeciesKey.Add(strSpeciesKey);

		nIdx = m_pGrid_Parent->SelectGetNext();
	}

	GetDlgItem(IDC_bDeleteDm)->ShowWindow(SW_HIDE);
	// 출력자료 보관된 내용이 있으면 그 내용을 출력한다.
	INT iSwSave = 0;	// 출력자료 보관된 내용이 있으면 1, 없으면 0
	if ( m_arrStrSpeciesKey.GetSize() > 0 ) {
		arrStrSpeciesKey.RemoveAll();
		for ( INT i = 0 ; i < m_arrStrSpeciesKey.GetSize() ; i++ ) {
			arrStrSpeciesKey.Add( m_arrStrSpeciesKey.GetAt(i) );
		}
		//AfxMessageBox( _T("출력자료 보관된 내용을 출력합니다.") );
		iSwSave = 1;
		GetDlgItem(IDC_bDeleteDm)->ShowWindow(SW_SHOW);
	}

	// 저장된 종키를 가지고 쿼리를 구성한다.
	CString strOption = _T("I.REC_KEY IN (");
	nCount = arrStrSpeciesKey.GetSize();
	for (INT idx = 0; idx < nCount; idx++)
	{
		strOption += arrStrSpeciesKey.GetAt(idx);
		if (idx != nCount - 1)
			strOption += _T(", ");
	}

	strOption += _T(")");

	// 단행 연속에 따라 테이블 이름을 및 쿼리를 변경한다.
	CString strFieldName;
	if (m_nModeMONO_SERIAL == MODE_MONO)
	{
		m_pDM->TBL_NAME = _T("BO_BOOK_TBL B, IDX_BO_TBL I");
		strFieldName = _T("DECODE(BOOK_APPENDIX_FLAG, 'A', '부록')");
	}
	else
	{
		m_pDM->TBL_NAME = _T("SE_BOOK_TBL B, IDX_SE_TBL I");
		strFieldName = _T("DECODE(BOOK_TYPE, 'A', '부록')");
	}

	// 책부록에 대한 내용을 바꾸어준다.
	CESL_DataMgr::reference *pRef = NULL;
	POSITION pos = m_pDM->RefList.GetHeadPosition();
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)m_pDM->RefList.GetNext(pos);
		if (pRef && pRef->FIELD_ALIAS == _T("책부록"))
		{
			pRef->FIELD_NAME = strFieldName;
			break;
		}		
	}

 	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 2003.08.23 이학중 수정
    // 복본추기한 내용의 원문책 정보를 보여 주지 않게 수정.  
    
	//strOption += _T("AND B.DUP_SPECIES_KEY IS NULL ") ;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
    CString bookWhere ;
	pDM_Where->GetCellData (_T("책조건절"),0,bookWhere) ;
    
	CString strFrom = m_pDM->TBL_NAME ;

	if(bookWhere.Find (_T("a."))>=0)
	{
		strFrom +=  _T(" , co_accession_rec_tbl a ") ;
		strOption = strOption + _T(" AND B.ACCESSION_REC_KEY = A.REC_KEY");
	}
	
	if(bookWhere.Find (_T("p."))>=0) 
	{
        strFrom +=  _T(" , BO_PURCHASE_SEQ_NO_TBL p  ") ;
	}
    
	m_pDM->TBL_NAME = strFrom ;
	//ids = m_pDM->RefreshDataManager(strOption);

    if ( iSwSave == 0 ) {	// 출력자료보관된 내용이 없으면 책조건절 포함
		strOption += _T(" and ") +bookWhere ;
	}
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	if ( iSwSave == 1 ) {
		CESL_DataMgr *pDmDeleteBook;
		pDmDeleteBook = FindDM(_T("DM_마지막번호"));
		if ( NULL == pDmDeleteBook ) {
			AfxMessageBox ( _T("pDmDeleteBook error") );
			return -1;
		}
		//삭제책KEY
		CString sDeleteBookKey, sDeleteBookKeyList;
		INT iDeleteCount = pDmDeleteBook->GetRowCount();
		for ( INT i = 0 ; i < iDeleteCount ; i++ ) {
			sDeleteBookKey = pDmDeleteBook->GetCellData( _T("삭제책KEY"), i );
			sDeleteBookKeyList = sDeleteBookKeyList + _T(",") + sDeleteBookKey;
		}
		
		if ( sDeleteBookKeyList != _T("") ) {
			sDeleteBookKeyList = sDeleteBookKeyList.Mid(1);
			strOption = strOption + _T(" AND B.REC_KEY NOT IN (") + sDeleteBookKeyList + _T(")");
		}
	}

	//strOption = strOption + _T(" AND I.REC_KEY = B.SPECIES_KEY AND B.REG_NO = 'EM0000000011'");
	ids = m_pDM->RefreshDataManager(strOption);
	if (ids < 0)
	{
		m_strLastErrMsg = _T("데이터베이스와 작업중에 에러가 발생했습니다.");
		return ids;
	}

	// 본관 삭제된 내용 제외
	for ( INT i = m_pDM->GetRowCount()-1 ; i >= 0 ; i-- ) {
		CString sBookKey = m_pDM->GetCellData( _T("BB_책KEY"), i );
		INT iBookCount = m_arrStrDeleteBookKey.GetSize();
		for ( INT j = 0 ; j < m_arrStrDeleteBookKey.GetSize() ; j++ ) {
			if ( sBookKey == m_arrStrDeleteBookKey.GetAt(j) ) {
				m_pDM->DeleteRow(i);
				break;
			}
		}
	}
	// 출력형태 및 청구기호를 세팅해 준다. (청구기호 : 별치기호 분류기호-도서기호-권책기호=복본기호)
	CString strCallNo;
	CString strItem;
	nCount = m_pDM->GetRowCount();
	for (idx = 0; idx < nCount; idx++)
	{
		// 소정 수정(2004.10.08)
		// 청구기호 생성 Format 2(줄바꿔찍기)
		strCallNo = MakeCallNoPrintFormat2(idx);

		// 청구기호
		ids = m_pDM->SetCellData(_T("청구기호"), strCallNo, idx);

		// 출력형태
		ids = m_pDM->GetCellData(_T("레이블출력구분"), idx, strItem);
		//if (strItem.IsEmpty())
		ids = m_pDM->SetCellData(_T("레이블출력구분"), _T("2"), idx);
	}

	// Display
	m_pCM->AllControlDisplay();
	SetGridHeight();

	// 전체 선정하기
	nCount = m_pGrid->GetRows();
	for (idx = 0; idx < nCount - 1; idx++)
	{
		m_pGrid->SetAt(idx, m_pGrid->m_nSelectIdx, m_pGrid->m_sSelectString);
	}


	//RFID 버튼 보여주기
//	CString sValue;
//	CString sQuery;
	sQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND GROUP_3 = '공통' AND CLASS_ALIAS = 'RFID사용여부' AND MANAGE_CODE = UDF_MANAGE_CODE ");
	m_pDM->GetOneValueQuery( sQuery , sValue );	
	
	if ( sValue == _T("Y") ) {
		GetDlgItem(IDC_bRfidWrite)->ShowWindow(SW_SHOW);
	}	


	return 0;

	EFS_END
	return -1;
}

//INT CEquipPrintDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias)
INT CEquipPrintDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, CStringArray *arrStrSpeciesKey)
{
	EFS_BEGIN

	if (!pDM_Parent || !pGrid_Parent || strSpeciesKeyAlias.IsEmpty()) return -1;

	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;

	for ( INT i = 0 ; i < arrStrSpeciesKey->GetSize() ; i++ ) {
		m_arrStrSpeciesKey.Add( arrStrSpeciesKey->GetAt(i) );
	}	

	return 0;

	EFS_END
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintDlg message handlers

INT CEquipPrintDlg::GridDisplayEventProc(CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str)
{
	EFS_BEGIN

	strGridAlias.TrimLeft();		strGridAlias.TrimRight();
	
	if (strDMFieldAlias == _T("레이블출력구분"))
	{
		if (str == _T("1"))
			str = _T("이어찍기");
		else 
			str = _T("줄바꿔찍기");
	}

	return 0;

	EFS_END
	return -1;
}

BOOL CEquipPrintDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();


	SetIcon(LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);
	
	if (InitESL_Mgr(_T("SM_정리_출력_장비")) < 0) 
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error!!");
		goto ERR;
	}

	if (Init() < 0) goto ERR;	
	
	// 소정 추가 2004.10.08 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_pEditCtrl = new CEdit();
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN, CRect(0, 0, 0, 0), this, IDC_EDIT_CALLNO);
	m_pEditCtrl->ShowWindow(SW_HIDE);
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CEquipPrintDlg::OnbCHANGEPRINTFORMAT() 
{
	EFS_BEGIN

	// 선정된 자료의 출력형태를 변경한다.
	CString strPrintFormat;
	INT ids = m_pCM->GetControlMgrData(_T("출력형태"), strPrintFormat);
	if (ids < 0) return;
	
	if (strPrintFormat == _T("이어찍기"))
		strPrintFormat = _T("1");
	else
		strPrintFormat = _T("2");
	
	ids = m_pGrid->SelectMakeList();
	if (ids < 0) return;

	if (m_pGrid->SelectGetCount() <= 0) return;

	CString strCallNo=_T("");
	INT nIdx = m_pGrid->SelectGetHeadPosition();
	while (nIdx >= 0)
	{
		// 소정 추가 2004.10.08 ++++++++++++++++++++++++++++++++++++++++++++++++++++++
		strCallNo = ChangeCallNoPrintFormat( strPrintFormat, nIdx );
		ids = m_pDM->SetCellData(_T("청구기호"), strCallNo, nIdx);
		// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=		

		ids = m_pDM->SetCellData(_T("레이블출력구분"), strPrintFormat, nIdx);
		nIdx = m_pGrid->SelectGetNext();
	}

	ids = m_pGrid->Display();
	SetGridHeight();

	EFS_END
}

VOID CEquipPrintDlg::OnbPRINTEQUIP() 
{
	EFS_BEGIN

	// 장비자료를 출력한다.
	CEquipPrintPropertyDlg dlg(this);
	dlg.SetParentInfo(m_pDM, m_pGrid);
	dlg.DoModal();

	EFS_END
}

VOID CEquipPrintDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN

	CDialog::OnSize(nType, cx, cy);
	
	if (!GetSafeHwnd()) return;	

	if (!m_pGrid || !m_pGrid->GetSafeHwnd()) return;


	CRect rc;
	GetClientRect(rc);
	rc.DeflateRect(10, 0);

	CRect rcGrid;
	m_pGrid->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);

	rcGrid.left = rc.left;
	rcGrid.right = rc.right;
	rcGrid.bottom = rc.bottom - 10;

	m_pGrid->MoveWindow(rcGrid);

	EFS_END
}

VOID CEquipPrintDlg::OnbRfidWrite() 
{
	// TODO: Add your control notification handler code here
	CString strParentCMAlias, strParentDMAlias, strParentGridAlias;
	if (strParentCMAlias.IsEmpty())
		strParentCMAlias = _T("CM_정리_출력_장비");
	if (strParentDMAlias.IsEmpty())
		strParentDMAlias = _T("DM_정리_출력_장비");
	if (strParentGridAlias.IsEmpty())
		strParentGridAlias = _T("GRID_LIST");

	INT idx;
	SelectMakeList(strParentCMAlias, strParentGridAlias);
	idx = SelectGetHeadPosition(strParentCMAlias, strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		//EndDialog(IDCANCEL);
		//m_bInitDlg = FALSE;
		return;		
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	INT iRowCount = 1;

	
	//RFID 등록번호, 서명 넣기
	CBarcodeToRFIDDlg dlg;
	//dlg.CONNECTION_INFO = _T("kolas2@kolas2/kolas2#[]");
	dlg.CONNECTION_INFO = pDM->CONNECTION_INFO;	
	CRFIDBookInfo *pBook;

	//국가 코드

	//도서관 코드	
	//CLibInfoMgr mgr(pMgr);
	//mgr.GetLibInfo(pMgr->GetUserID(), &m_cLibInfo);
	//CLibInfoMgr mgr(pParent);
	CLibInfoMgr mgr(this);
	CString strUserId;
	CLibInfo pLibInfo;
	mgr.GetLibInfo( strUserId, &pLibInfo );
	CString sLibCode = pLibInfo.GetLibCode();	

// 050421_삭제-자료상태 상관없음
//	if ( pDM->GetCellData( _T("작업상태"), idx ) != _T("BOL112N") )
//	{
//		AfxMessageBox(_T("소장자료만 RFID TAG를 입력하실수 있습니다."));
//		return;
//	}



	CString sRegNo = pDM->GetCellData( _T("등록번호"), idx );
	CString sRegCode, sRegNumber;
	if ( sRegNo.GetLength() == 12 ) {
		sRegCode = sRegNo.Left(2);
		sRegNumber = sRegNo.Mid(2);
	}		
	CString sTitle = pDM->GetCellData( _T("본표제"), idx );
	CString sRfidSerial = pDM->GetCellData( _T("BB_RfidSerial"), idx );	
	CString sBookKey = pDM->GetCellData( _T("BB_책KEY"), idx );
	CString sCallNo = pDM->GetCellData( _T("청구기호"), idx );
	if ( sCallNo.GetLength() > 25 ) {
		sCallNo = sCallNo.Left(25);
	}
	CString sShelfCode = pDM->GetCellData( _T("배가코드") , idx );
	CString sBookStatus = pDM->GetCellData( _T("작업상태"), idx );	

	//첫번째 등록번호, 서명
	pBook = dlg.AddBook();
	if (pBook) {		
		pBook->strAccessionNo = sRegNo;
		pBook->strTitle = sTitle;
		pBook->strRFIDSerialNo = sRfidSerial;
		pBook->strBookKey = sBookKey;
		pBook->strCallNo = sCallNo;
		pBook->strShelfLocCode = sShelfCode;
		pBook->strStatus = sBookStatus;
	}

	idx = SelectGetNext(strParentCMAlias, strParentGridAlias);
	CString sRecKeySub;
	while ( idx != -1 ) {

		CString sRegNo = pDM->GetCellData( _T("등록번호"), idx );
		CString sRegCode, sRegNumber;
		if ( sRegNo.GetLength() == 12 ) {
			sRegCode = sRegNo.Left(2);
			sRegNumber = sRegNo.Mid(2);
		}
		CString sTitle = pDM->GetCellData( _T("본표제"), idx );
		CString sRfidSerial = pDM->GetCellData( _T("BB_RfidSerial"), idx );	
		CString sBookKey = pDM->GetCellData( _T("BB_책KEY"), idx );
		CString sCallNo = pDM->GetCellData( _T("청구기호"), idx );
		if ( sCallNo.GetLength() > 25 ) {
			sCallNo = sCallNo.Left(25);
		}
		CString sShelfCode = pDM->GetCellData( _T("배가코드"), idx );
		CString sBookStatus = pDM->GetCellData( _T("작업상태"), idx );	

		pBook = dlg.AddBook();
		if (pBook) {
			pBook->strAccessionNo = sRegNo;
			pBook->strTitle = sTitle;
			pBook->strRFIDSerialNo = sRfidSerial;
			pBook->strBookKey = sBookKey;
			pBook->strCallNo = sCallNo;
			pBook->strShelfLocCode = sShelfCode;
			pBook->strStatus = sBookStatus;
			
		}

		idx = SelectGetNext(strParentCMAlias, strParentGridAlias);		
		iRowCount ++ ;
	} 
	
	dlg.DoModal();

	return;
}

VOID CEquipPrintDlg::OnbDeleteDm() 
{
	// TODO: Add your control notification handler code here
	CString strParentCMAlias, strParentDMAlias, strParentGridAlias;
	if (strParentCMAlias.IsEmpty())
		strParentCMAlias = _T("CM_정리_출력_장비");
	if (strParentDMAlias.IsEmpty())
		strParentDMAlias = _T("DM_정리_출력_장비");
	if (strParentGridAlias.IsEmpty())
		strParentGridAlias = _T("GRID_LIST");

	INT idx;
	SelectMakeList(strParentCMAlias, strParentGridAlias);
	idx = SelectGetHeadPosition(strParentCMAlias, strParentGridAlias);
	if (idx < 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));	// 에러
		//EndDialog(IDCANCEL);
		//m_bInitDlg = FALSE;
		return;		
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}
	/*
	CESL_DataMgr *pDmDeleteBook;
	pDmDeleteBook = FindDM(_T("DM_단행_정리_검색결과"));
	if ( NULL == pDmDeleteBook ) {
		AfxMessageBox ( _T("pDmDeleteBook error") );
		return;
	}
	*/
	CESL_DataMgr *pDmDeleteBook;
	pDmDeleteBook = FindDM(_T("DM_마지막번호"));
	if ( NULL == pDmDeleteBook ) {
		AfxMessageBox ( _T("pDmDeleteBook error") );
		return;
	}
	
	//삭제책KEY
	CString sDeleteBookKey = pDmDeleteBook->GetCellData( _T("삭제책KEY"), 0 );

	idx = SelectGetTailPosition(strParentCMAlias, strParentGridAlias);
	//idx = SelectGetNext(strParentCMAlias, strParentGridAlias);
	CString sBookKey;
	CString sSpeciesKey;
	while ( idx != -1 ) {
		sBookKey = pDM->GetCellData( _T("BB_책KEY"), idx );
		sSpeciesKey = pDM->GetCellData( _T("BS_종KEY"), idx );
		INT iBookCount = m_arrStrDeleteBookKey.GetSize();		
		if ( sBookKey != _T("") ) {
			m_arrStrDeleteBookKey.Add(sBookKey);
			pDM->DeleteRow( idx );
			pDmDeleteBook->InsertRow(-1);
			INT iDeleteCount = pDmDeleteBook->GetRowCount();
			pDmDeleteBook->SetCellData( _T("삭제책KEY"), sBookKey, iDeleteCount-1 );
			pDmDeleteBook->SetCellData( _T("삭제종KEY"), sSpeciesKey, iDeleteCount-1 );
			/*
			if ( sDeleteBookKey != _T("") ) {
				sDeleteBookKey = sDeleteBookKey + _T(",") + sBookKey;
			}
			else {
				sDeleteBookKey = sBookKey;
			}
			*/
		}
		idx = SelectGetPrev(strParentCMAlias, strParentGridAlias);		
	}
	//if ( sDeleteBookKey != _T("") ) {
	//	sDeleteBookKey = sDeleteBookKey.Mid(1);
	//}
	//pDmDeleteBook->SetCellData( _T("삭제책KEY"), sDeleteBookKey, 0 );
	m_pCM->AllControlDisplay();
}


////////////////////////////////////////////////////////////////////////////////
// 소정 추가 2004.10.07 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
CString CEquipPrintDlg::ChangeCallNoPrintFormat(CString sPrintFormat, INT idx)
{
	if ( _T("1") == sPrintFormat )
		return MakeCallNoPrintFormat1( idx );
	if ( _T("2") == sPrintFormat )
		return MakeCallNoPrintFormat2( idx );

	return _T("");
}
CString CEquipPrintDlg::MakeCallNoPrintFormat1(INT idx)
{
	CString strCallNo = _T("");
	CString strItem = _T("");

	INT ids = m_pDM->GetCellData(_T("별치기호_코드"), idx, strItem);
	if (!strItem.IsEmpty())
	{
		// 별치기호는 설명으로 바꾼다.
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItem, strItem);
	}

	strCallNo = strItem;

	ids = m_pDM->GetCellData(_T("분류기호"), idx, strItem);
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T(" ") + strItem;
	}

	ids = m_pDM->GetCellData(_T("도서기호"), idx, strItem);
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T("-") + strItem;
	}

	ids = m_pDM->GetCellData(_T("권책기호"), idx, strItem);	
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T("-") + strItem;
	}

	ids = m_pDM->GetCellData(_T("복본기호"), idx, strItem);	
	if (!strItem.IsEmpty())
	{
		if (strCallNo.IsEmpty())
			strCallNo = strItem;
		else
			strCallNo += _T("=") + strItem;
	}

	return strCallNo;

}

CString CEquipPrintDlg::MakeCallNoPrintFormat2(INT idx )
{
	CString strCallNo = _T("");
	CString strItemData=_T("");
	INT nHeight;

	nHeight = 0;
	
	INT ids = m_pDM->GetCellData(_T("별치기호_코드"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		// 별치기호는 설명으로 바꾼다.
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
		strCallNo = strItemData + _T("\r\n");		
	}
	
	ids = m_pDM->GetCellData(_T("분류기호"), idx, strItemData);
	if (!strItemData.IsEmpty())		
	{
		strCallNo +=  strItemData;		
	}
	
	ids = m_pDM->GetCellData(_T("도서기호"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		strCallNo += _T("\r\n") + strItemData;
	}

	ids = m_pDM->GetCellData(_T("권책기호"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		// 050128_권책기호 관리에 설정값으로 SET
		//strCallNo += _T("\r\n") + strItemData;
		strCallNo += _T("\r\n") + m_strVolCode;//권책기호
		strCallNo += strItemData;
	}

	ids = m_pDM->GetCellData(_T("복본기호"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
		strCallNo += _T("\r\nc.") + strItemData;
	}

	

	return strCallNo;
}

INT CEquipPrintDlg::SetGridHeight() 
{
	INT nHeight, ids;
	CString strTmp;


	for ( INT i = 0 ; i < m_pDM->GetRowCount() ; i++ ) 
	{
		strTmp = m_pDM->GetCellData(_T("청구기호"), i);
		nHeight = 1;
		while ( strTmp.Find(_T("\n"),0) != -1 )
		{
			nHeight++;
			ids = strTmp.Find(_T("\n"),0);
			strTmp = strTmp.Mid(ids+1);
		}
		nHeight++;
		m_pGrid->SetRowHeight( i+1, 200 * nHeight );
	}

	return 0;
}


INT CEquipPrintDlg::ShowControl()
{
	EFS_BEGIN


	INT nCol = m_pGrid->GetCol();
	INT nRow = m_pGrid->GetRow();

	CString sOldGirdData = m_pGrid->GetTextMatrix(nRow, nCol);

	CString strText;
	strText.Format(_T("%s"), m_pGrid->GetTextMatrix(0, nCol));

	if ( strText != _T("청구기호") ) return -1;

	
	// 콤보가 아니면	
	// 에디트 박스를 보여줄것
	ShowEditBox(m_pGrid);

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintDlg::ShowEditBox(CESL_Grid *pGrid)
{
	EFS_BEGIN



	m_pEditCtrl->GetSafeHwnd();

	if (pGrid == NULL) return -1;

	INT nRow = pGrid->GetRow();
	INT nCol = pGrid->GetCol();



	if (nRow < 1 || nCol < 1) return -1;

	CRect rcCell;
	GetCellRect(pGrid, rcCell);

	// 텍스트 설정
	m_pEditCtrl->SetWindowText(pGrid->GetTextMatrix(nRow, nCol));

	m_pEditCtrl->MoveWindow(rcCell);
	m_pEditCtrl->ShowWindow(SW_SHOW);
	m_pEditCtrl->SetFocus();
	m_pEditCtrl->BringWindowToTop();

	m_nSelectedGridRow = nRow;
	m_nSelectedGridCol = nCol;

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintDlg::GetCellRect(CESL_Grid *pGrid, CRect &rect)
{
	EFS_BEGIN

	if (pGrid == NULL) return -1;

	CRect rcGrid;
	CRect rcClient;

	pGrid->GetWindowRect(rcGrid);

	GetClientRect(rcClient);

	ClientToScreen((POINT*)&(rcClient.left));

	rcGrid.top -= rcClient.top;
	rcGrid.left -= rcClient.left;

	rect.left	= rcGrid.left	+ (long)((float)pGrid->GetCellLeft() / 15.0f);
	rect.top	= rcGrid.top	+ (long)((float)pGrid->GetCellTop() / 15.0f);
	rect.right	= rect.left		+ (long)((float)pGrid->GetCellWidth() / 15.0f) - 1;
	rect.bottom = rect.top		+ (long)((float)pGrid->GetCellHeight() / 15.0f) - 1;

	return 0;

	EFS_END
	return -1;
}


INT CEquipPrintDlg::ApplyDataChange(LPCTSTR lpszData, INT nTopRow /* = 0 */)
{
	EFS_BEGIN

	if (lpszData == NULL) return -1;


	if (m_pDM == NULL || m_pGrid == NULL) return -1;

	// 그리드에 변경을 적용시킨다.
	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(m_nSelectedGridRow, m_nSelectedGridCol, lpszData);
	

	// 데이터 메니저에 변경을 적용시킨다.
	m_pDM->SetCellData(_T("청구기호"), lpszData, m_nSelectedGridRow - 1);
	
	m_pGrid->Display(m_nSelectedGridRow);
	SetGridHeight();

	m_pGrid->SetTopRow(nTopRow);
	return 0;

	EFS_END
	return -1;
}



BEGIN_EVENTSINK_MAP(CEquipPrintDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CEquipPrintDlg)
	ON_EVENT(CEquipPrintDlg, IDC_GRID_EQUIP_BOOK_LIST, -601 /* DblClick */, OnDblClickGridEquipBookList, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CEquipPrintDlg::OnDblClickGridEquipBookList() 
{
	if (m_pGrid->GetMouseRow() == 0) return;
	ShowControl();
	
}

VOID CEquipPrintDlg::OnDestroy() 
{

	CDialog::OnDestroy();
	
	if (m_pEditCtrl)
	{
		delete m_pEditCtrl;
		m_pEditCtrl = NULL;
	}
	
	
}

VOID CEquipPrintDlg::OnKillfocusEditCallno() 
{
	SetFocus();

	if (m_pEditCtrl == NULL) return ;
	
	if (!::IsWindowVisible(m_pEditCtrl->GetSafeHwnd())) return ;

	m_pEditCtrl->ShowWindow(SW_HIDE);
			

	CString strData;
	m_pEditCtrl->GetWindowText(strData);

	INT nTopRow;

	nTopRow = m_pGrid->GetTopRow();

	ApplyDataChange(strData, nTopRow);

	return ;

	
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
