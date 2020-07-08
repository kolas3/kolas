// EquipPrintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EquipPrintDlg.h"
#include "..\MarcAdjustApi\MarcAdjustConstant.h"
#include "..\K2UP_Common\LibInfoMgr.h"
// 2018.11.22 LJY : RFID BOOK TAGGING
#include "..\..\단행\열람\장서관리\RFID_JOB\RFIDTaggingDlg.h"
//---
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


	m_nModeMONO_SERIAL = 0;

	m_pDM_Parent = NULL;
	m_pGrid_Parent = NULL;

	m_pDM = NULL;
	m_pCM = NULL;
	m_pGrid = NULL;
	m_pEditCtrl = NULL;

	m_pParent = pParent;
}

CEquipPrintDlg::~CEquipPrintDlg()
{
	CESL_DataMgr	*m_pSaveDM;
	m_pSaveDM		= FindDM(_T("DM_정리_출력_장비_책_보관"));
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
	ON_BN_CLICKED(IDC_bDeleteDm, OnbDeleteDm)
	ON_WM_DESTROY()
	ON_EN_KILLFOCUS(IDC_EDIT_CALLNO, OnKillfocusEditCallno)
	ON_WM_CTLCOLOR()
	ON_WM_NCACTIVATE()
	ON_BN_CLICKED(IDC_btnBookTagging, OnbtnRFIDBookTagging)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintDlg Operations

INT CEquipPrintDlg::Init()
{
	EFS_BEGIN

	if (!m_pDM_Parent || !m_pGrid_Parent|| m_strSpeciesKeyAlias.IsEmpty()) return -1;
	
	m_pDM = FindDM(_T("DM_정리_출력_장비"));
	m_pCM = FindCM(_T("CM_정리_출력_장비"));
	m_pGrid = reinterpret_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID_LIST")));

	if (!m_pDM || !m_pCM || !m_pGrid) return -1;

	m_pGrid->SetProtectSelectColumn(TRUE);
	m_pGrid->m_fpEventHandler = ( INT( CWnd::* )( CString &strGridAlias, INT nRow, INT nCol, CString &strDMFieldAlias, CString &str ) )GridDisplayEventProc;

	INT ids = -1;
	INT nCount;
	INT nIdx;
	INT idx;
	INT	i;

	if ( m_nPrintType == 0 )
	{	
		ids = m_pGrid_Parent->SelectMakeList();
		if (ids < 0) return ids;
		
		nCount = m_pGrid_Parent->SelectGetCount();
		if (nCount <= 0)
		{
			if ( m_arrStrSpeciesKey.GetSize() < 1 ) {
				m_strLastErrMsg = _T("선정된 자료가 없습니다.");
				return -1;
			}
		}
			
		CStringArray arrStrSpeciesKey;
		
		nIdx = m_pGrid_Parent->SelectGetHeadPosition();

		if (nIdx < 0 && m_arrStrSpeciesKey.GetSize() < 1) return -1;
		
		CString strSpeciesKey;
		while (nIdx >= 0)
		{
			m_pDM_Parent->GetCellData(m_strSpeciesKeyAlias, nIdx, strSpeciesKey);
			if (!strSpeciesKey.IsEmpty())
				arrStrSpeciesKey.Add(strSpeciesKey);
			
			nIdx = m_pGrid_Parent->SelectGetNext();
		}
		
		GetDlgItem(IDC_bDeleteDm)->ShowWindow(SW_HIDE);	
			
		CString strOption = _T("I.REC_KEY IN (");
		nCount = arrStrSpeciesKey.GetSize();
		for (idx = 0; idx < nCount; idx++)
		{
			strOption += arrStrSpeciesKey.GetAt(idx);
			if (idx != nCount - 1)
				strOption += _T(", ");
		}	

		strOption += _T(")");

		CString strAppendix;
		CString strVol;
		CString strPage;
		CString strPublishYear;
		CString strVolTitle;

		if (m_nModeMONO_SERIAL == MODE_MONO)
		{
			m_pDM->TBL_NAME = _T("BO_BOOK_TBL B, IDX_BO_TBL I");
			strAppendix = _T("DECODE(BOOK_APPENDIX_FLAG, 'A', '부록')");
			strVol = _T("B.VOL");
			strPage = _T("B.PAGE");
			strPublishYear = _T("B.PUBLISH_YEAR");
			strVolTitle = _T("B.VOL_TITLE");
		}
		else
		{
			m_pDM->TBL_NAME = _T("SE_BOOK_TBL B, IDX_SE_TBL I");
			strAppendix = _T("DECODE(BOOK_TYPE, 'A', '부록')");
			strVol = _T("B.VOL_CODE");
			strPage = _T("(SELECT PAGE FROM SE_VOL_TBL WHERE B.VOL_KEY = REC_KEY)");
			strPublishYear = _T("(SELECT SUBSTR(PUBLISH_DATE,0,4) FROM SE_VOL_TBL WHERE B.VOL_KEY = REC_KEY)");
			strVolTitle = _T("(SELECT VOL_TITLE FROM SE_VOL_TBL WHERE B.VOL_KEY = REC_KEY)");
		}

		CESL_DataMgr::reference *pRef = NULL;
		POSITION pos = m_pDM->RefList.GetHeadPosition();
		while (pos)
		{
			pRef = (CESL_DataMgr::reference*)m_pDM->RefList.GetNext(pos);
			if (pRef && pRef->FIELD_ALIAS == _T("책부록"))
			{
				pRef->FIELD_NAME = strAppendix;
			}		
			else if (pRef && pRef->FIELD_ALIAS == _T("편권차"))
			{
				pRef->FIELD_NAME = strVol;
			}
			else if (pRef && pRef->FIELD_ALIAS == _T("면장수"))
			{
				pRef->FIELD_NAME = strPage;
			}
			else if (pRef && pRef->FIELD_ALIAS == _T("발행년"))
			{
				pRef->FIELD_NAME = strPublishYear;
			}
			else if (pRef && pRef->FIELD_ALIAS == _T("권서명"))
			{
				pRef->FIELD_NAME = strVolTitle;
			}
		}
		
		CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
		CString bookWhere ;
		pDM_Where->GetCellData (_T("책조건절"),0,bookWhere) ;
    
		CString strFrom = m_pDM->TBL_NAME ;
		
		if(bookWhere.Find(_T("a."))>=0 || bookWhere.Find(_T("A."))>=0)
		{
			strFrom +=  _T(" , co_accession_rec_tbl a ") ;
			strOption = strOption + _T(" AND B.ACCESSION_REC_KEY = A.REC_KEY");
		}
		
		if(bookWhere.Find (_T("p."))>=0 || bookWhere.Find (_T("P."))>=0) 
		{
			strFrom +=  _T(" , BO_PURCHASE_SEQ_NO_TBL p  ") ;
			strOption = strOption + _T(" AND P.MANAGE_CODE = UDF_MANAGE_CODE");
		}
		
		m_pDM->TBL_NAME = strFrom ;
		strOption += _T(" and ") +bookWhere ;		
		ids = m_pDM->RefreshDataManager(strOption);
		if (ids < 0)
		{
			m_strLastErrMsg = _T("데이터베이스와 작업중에 에러가 발생했습니다.");
			return ids;
		}

		CESL_DataMgr * tmpDM = new CESL_DataMgr();

		tmpDM->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		tmpDM->TBL_NAME = m_pDM->TBL_NAME;
		
		tmpDM->EXTRA_CONDITION = m_pDM->EXTRA_CONDITION;
		tmpDM->CONDITION = m_pDM->CONDITION;
		tmpDM->DB_MGR_ALIAS = m_pDM->DB_MGR_ALIAS;
		
		tmpDM->InitDataMgrRef( m_pDM->RefList.GetCount() );
		tmpDM->m_nCols = m_pDM->RefList.GetCount();
		
		CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
		for( INT k = 0 ; k < m_pDM->RefList.GetCount() ; k++ )
		{
			sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
			ids = m_pDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
			if( ids < 0 ) AfxMessageBox(_T("MakeCopdyDM Error"));
			
			ids = tmpDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
			if( ids < 0 ) AfxMessageBox(_T("MakeCopdyDM Error"));
		}
		
		tmpDM->CopyDM(m_pDM);

		INT nCnt = m_pDM->GetRowCount();
		for( INT i = nCnt-1 ; i>=0; i-- )
			m_pDM->DeleteRow(i);
		
		nCnt = m_pDM->GetRowCount();
		nCnt = tmpDM->GetRowCount();
			
		nCount = arrStrSpeciesKey.GetSize();
		for (idx = 0; idx < nCount; idx++)
		{
			strOption = arrStrSpeciesKey.GetAt(idx);
			for( i = 0; i < nCnt; i++ )
			{
				if( strOption == tmpDM->GetCellData(_T("BS_종KEY"),i) )
				{
					CopyDMToDMByAlias(tmpDM,i,m_pDM,-1);
				}
			}
		}
	}
	else
	{	
		CESL_DataMgr *m_pSaveDM;		
		const INT nColCnt = 23;
		
		CString sColAliasData[nColCnt];
		CString sColAlias[nColCnt] =
		{
			_T("도서기호"), _T("분류기호"), _T("복본기호"), _T("레이블출력구분"), _T("등록번호"),
				_T("RFID_SERIAL"), _T("별치기호_코드"), _T("배가코드"), _T("권책기호"), _T("작업상태"), 
				_T("저작자"), _T("발행자"), _T("BS_종KEY"), _T("본표제"), _T("청구기호"), 
				_T("선정"), _T("삭제책KEY"), _T("책부록"), _T("BB_책KEY")			
				,_T("편권차"), _T("면장수"), _T("발행년"), _T("권서명")
		};

		m_pSaveDM = FindDM(_T("DM_정리_출력_장비_책_보관"));
		nCount = m_pSaveDM->GetRowCount();		
		if ( nCount <= 0 )
		{
			m_strLastErrMsg = _T("보관된 자료가 없습니다.");

			return -1;
		}

		m_pDM->FreeData();
		for ( i=0 ; i<nCount ; i++ )
		{
			m_pSaveDM->GetCellData(sColAlias, 23, sColAliasData, i );		
			ids = m_pDM->InsertRow(-1);
			m_pDM->SetCellData( sColAlias , 23, sColAliasData , i );		
		}
	}

	for ( i = m_pDM->GetRowCount()-1 ; i >= 0 ; i-- ) {
		CString sBookKey = m_pDM->GetCellData( _T("BB_책KEY"), i );
		INT iBookCount = m_arrStrDeleteBookKey.GetSize();
		for ( INT j = 0 ; j < m_arrStrDeleteBookKey.GetSize() ; j++ ) {
			if ( sBookKey == m_arrStrDeleteBookKey.GetAt(j) ) {
				m_pDM->DeleteRow(i);
				break;
			}
		}
	}

	CString strCallNo;
	CString strItem;
	nCount = m_pDM->GetRowCount();
	for (idx = 0; idx < nCount; idx++)
	{
		strCallNo = MakeCallNoPrintFormat2(idx);

		ids = m_pDM->SetCellData(_T("청구기호"), strCallNo, idx);

		ids = m_pDM->GetCellData(_T("레이블출력구분"), idx, strItem);

		ids = m_pDM->SetCellData(_T("레이블출력구분"), _T("2"), idx);
	}

	m_pCM->AllControlDisplay();
	SetGridHeight();

	nCount = m_pGrid->GetRows();
	for (idx = 0; idx < nCount - 1; idx++)
	{
		m_pGrid->SetAt(idx, m_pGrid->m_nSelectIdx, m_pGrid->m_sSelectString);
	}

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, INT nTYPE)
{
	EFS_BEGIN

	if (!pDM_Parent || !pGrid_Parent || strSpeciesKeyAlias.IsEmpty()) return -1;

	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strSpeciesKeyAlias = strSpeciesKeyAlias;
	m_nPrintType = nTYPE;
	
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
	
	m_pEditCtrl = new CEdit();
	m_pEditCtrl->Create(WS_CHILD | ES_AUTOHSCROLL | ES_LEFT | ES_MULTILINE | ES_WANTRETURN, CRect(0, 0, 0, 0), this, IDC_EDIT_CALLNO);
	m_pEditCtrl->ShowWindow(SW_HIDE);

	EnableThemeDialogTexture(GetSafeHwnd());
	m_bRFIDYN = RecordRFIDUSEYN();

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
		strCallNo = ChangeCallNoPrintFormat( strPrintFormat, nIdx );
		ids = m_pDM->SetCellData(_T("청구기호"), strCallNo, nIdx);	

		ids = m_pDM->SetCellData(_T("레이블출력구분"), strPrintFormat, nIdx);
		nIdx = m_pGrid->SelectGetNext();
	}

	ids = m_pGrid->Display();
	if ( _T("2") == strPrintFormat )  SetGridHeight();

	EFS_END
}

VOID CEquipPrintDlg::OnbPRINTEQUIP() 
{
	EFS_BEGIN

	CEquipPrintPropertyDlg dlg(this);
	dlg.SetParentInfo(m_pDM, m_pGrid);
	dlg.DoModal();

	EFS_END
}

BOOL CEquipPrintDlg::RecordRFIDUSEYN()
{
EFS_BEGIN
	m_bRFIDYN = FALSE;
	CString strRfidUseYN =_T("N");
	CString sQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND GROUP_3 = '공통' AND CLASS_ALIAS = 'RFID사용여부' AND MANAGE_CODE = UDF_MANAGE_CODE ");
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	TmpDM.GetOneValueQuery( sQuery , strRfidUseYN );	

	if(_T("Y") == strRfidUseYN )
	{
		GetDlgItem(IDC_btnBookTagging)->ShowWindow(TRUE);	
		m_bRFIDYN = TRUE;
	}
	return m_bRFIDYN;

EFS_END
	return FALSE;
}

VOID CEquipPrintDlg::OnbtnRFIDBookTagging()
{
EFS_BEGIN
	if(!m_bRFIDYN) return;

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
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;		
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	INT iRowCount = 1;

	CRFIDTaggingDlg dlg;
	dlg.CONNECTION_INFO = pDM->CONNECTION_INFO;	
	CRFIDBookInfo *pBook;

	CLibInfoMgr mgr(this);
	CString strUserId;
	CLibInfo pLibInfo;
	mgr.GetLibInfo( strUserId, &pLibInfo );
	CString sLibCode = pLibInfo.GetLibCode();	


	CString sRegNo = pDM->GetCellData( _T("등록번호"), idx );
	CString sRegCode, sRegNumber;

	INT nRegCodeLength;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	if ( sRegNo.GetLength() == 12 ) {
		sRegCode = sRegNo.Left(nRegCodeLength);
		sRegNumber = sRegNo.Mid(nRegCodeLength);
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

	pBook = dlg.AddBook();
	if (pBook) {		
		pBook->strAccessionNo = sRegNo;
		pBook->strTitle = sTitle;
		pBook->strRFIDSerialNo = sRfidSerial;
		pBook->strBookKey = sBookKey;
		pBook->strCallNo = sCallNo;
		pBook->strShelfLocCode = sShelfCode;
		pBook->strStatus = sBookStatus;

		if ( m_nModeMONO_SERIAL == MODE_SERIAL )
			pBook->strBoSe = _T("S");
		else
			pBook->strBoSe = _T("B");

	}

	idx = SelectGetNext(strParentCMAlias, strParentGridAlias);
	CString sRecKeySub;
	while ( idx != -1 ) {

		CString sRegNo = pDM->GetCellData( _T("등록번호"), idx );
		CString sRegCode, sRegNumber;

		INT nRegCodeLength;
		nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
		if ( sRegNo.GetLength() == 12 ) {
			sRegCode = sRegNo.Left(nRegCodeLength);
			sRegNumber = sRegNo.Mid(nRegCodeLength);
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
			if ( m_nModeMONO_SERIAL == MODE_SERIAL )
				pBook->strBoSe = _T("S");
			else
				pBook->strBoSe = _T("B");
		}

		idx = SelectGetNext(strParentCMAlias, strParentGridAlias);		
		iRowCount ++ ;
	} 
	
	dlg.GetLibCode( this->m_pInfo->USER_PK , dlg.LIB_CODE );
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

	m_pGrid->MoveWindow(rcGrid);

	EFS_END
}

VOID CEquipPrintDlg::OnbDeleteDm() 
{
	
	CString strParentCMAlias, strParentDMAlias, strParentSaveDMAlias, strParentGridAlias;
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
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;		
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(strParentDMAlias);
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	strParentSaveDMAlias = _T("DM_정리_출력_장비_책_보관");
	CESL_DataMgr *pSaveDM;
	pSaveDM = FindDM(strParentSaveDMAlias);
	if ( NULL == pSaveDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return;
	}

	CString strBookKey;
	CString strBookKey2;
	INT		i, nSaveCount;
	BOOL	bCheck;

	nSaveCount = pSaveDM->GetRowCount();
	idx = SelectGetTailPosition(strParentCMAlias, strParentGridAlias);
	while ( idx != -1 ) 
	{
		pDM->GetCellData(_T("BB_책KEY"), idx, strBookKey );

		bCheck = FALSE;
		for ( i=0 ; i<nSaveCount ; i++ )
		{
			pSaveDM->GetCellData(_T("BB_책KEY"), i, strBookKey2 );
			if ( strBookKey == strBookKey2 ) 
			{
				bCheck = TRUE;
				pSaveDM->DeleteRow( i );
				nSaveCount--;
				break;
			}
		}
		
		m_pGrid->SetTextMatrix(idx+1, 1, _T(""));
		pDM->DeleteRow( idx );

		idx = SelectGetPrev(strParentCMAlias, strParentGridAlias);
	}
	
	m_pCM->AllControlDisplay();
	SetGridHeight();
}

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
	
	CString sQuery = _T("");
	CString sValue = _T("") ;
	sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_1 = 'L' AND GROUP_2 = '정리' AND GROUP_3 = '공통' AND CLASS_ALIAS = '레이블권책기호설정'");
	m_pDM_Parent->GetOneValueQuery( sQuery , sValue );	
	sValue.TrimLeft(); sValue.TrimRight();

	INT ids = m_pDM->GetCellData(_T("별치기호_코드"), idx, strItemData);
	if (!strItemData.IsEmpty())
	{
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
		strCallNo += _T("\r\n") + sValue + strItemData;
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


INT CEquipPrintDlg::ApplyDataChange(LPCTSTR lpszData)
{
	EFS_BEGIN

	if (lpszData == NULL) return -1;


	if (m_pDM == NULL || m_pGrid == NULL) return -1;

	m_pGrid->SetFocus();
	m_pGrid->SetTextMatrix(m_nSelectedGridRow, m_nSelectedGridCol, lpszData);
	
	m_pDM->SetCellData(_T("청구기호"), lpszData, m_nSelectedGridRow - 1);
	
	m_pGrid->Display();
	SetGridHeight();

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
	
	if (m_pEditCtrl != NULL)
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


	ApplyDataChange(strData);

	return ;

	
}


INT CEquipPrintDlg::CopyDMToDMByAlias(CESL_DataMgr *pDM_From, INT nFromIdx, CESL_DataMgr *pDM_To, INT nToIdx)
{
	if (pDM_From == NULL || pDM_To == NULL) return -1;
	
	INT ids = -1;
	
	INT nRowCount = pDM_From->GetRowCount();
	if (nRowCount - 1 < nFromIdx || nFromIdx < 0) return -1;
	
	nRowCount = pDM_To->GetRowCount();
	if (nRowCount - 1 < nToIdx ) return -1;
	
	if (nToIdx < 0)
	{
		ids = pDM_To->InsertRow(-1);
		if (ids < 0) return -1;
		nToIdx = pDM_To->GetRowCount() - 1;
	}	
	
	CString strAlias;
	CString strData, strTmpData;
	
	CESL_DataMgr::reference *pRef = NULL;
	
	POSITION pos = pDM_From->RefList.GetHeadPosition();
	INT startpos = -1;
	while (pos)
	{
		pRef = (CESL_DataMgr::reference*)pDM_From->RefList.GetNext(pos);
		if (pRef == NULL) continue;
		
		strAlias = pRef->FIELD_ALIAS;
				
		ids = pDM_From->GetCellData(strAlias, nFromIdx, strData);
		if (ids < 0) continue;

		ids = pDM_To->SetCellData(strAlias, strData, nToIdx);
		if (ids < 0) continue;
	}
	
	return nToIdx;
	
	return 0;
}

HBRUSH CEquipPrintDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}


LRESULT CEquipPrintDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CEquipPrintDlg::OnNcActivate(BOOL bActive) 
{	
	return CDialog::OnNcActivate(bActive);
}
