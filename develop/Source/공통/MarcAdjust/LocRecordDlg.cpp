// LocRecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LocRecordDlg.h"

#include "..\MarcAdjustApi\CatApi.h"
#include "..\MarcAdjustApi\CatApi_BO.h"
#include "..\MarcAdjustApi\CatApi_SE.h"

#include "..\..\공동목록\Light_BO_CAT_CENTERUPLOAD\BO_CAT_CENTERUPLOAD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocRecordDlg dialog


CLocRecordDlg::CLocRecordDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CLocRecordDlg)
	m_bCenterUpload = FALSE;
	//}}AFX_DATA_INIT

	m_pDM_Parent = NULL;
	m_pGrid_Parent  = NULL;

	m_pCM = NULL;
	m_pGrid = NULL;

	m_nModeMONO_SERIAL = MODE_MONO;
}

CLocRecordDlg::~CLocRecordDlg()
{
	
}

VOID CLocRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocRecordDlg)
	DDX_Check(pDX, IDC_chkCENTER_UPLOAD_BC, m_bCenterUpload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocRecordDlg, CDialog)
	//{{AFX_MSG_MAP(CLocRecordDlg)
	ON_BN_CLICKED(IDC_bSAVE_BC, OnbSAVE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocRecordDlg message handlers

INT CLocRecordDlg::SetParentInfo(CESL_DataMgr *pDM_Parent, CESL_Grid *pGrid_Parent, CString strSpeciesKeyAlias, BOOL bCatalogComplete )
{
	if (!pDM_Parent || !pGrid_Parent || strSpeciesKeyAlias.IsEmpty()) return -1;

	m_pDM_Parent = pDM_Parent;
	m_pGrid_Parent = pGrid_Parent;
	m_strParentSpeciesKeyAlias = strSpeciesKeyAlias;
	m_bCatalogComplete = bCatalogComplete;

	return 0;
}

INT CLocRecordDlg::Init()
{
	CString strDMAlias, strHD_Book;
	CCatApi *CatApi;

	if (m_nModeMONO_SERIAL == MODE_MONO)
	{
		strDMAlias = _T("DM_단행_정리DM_책정보");
		CatApi = new CCatApi_BO(this);
		strHD_Book = _T("BB");
	}
	else
	{
		strDMAlias = _T("DM_연속_정리DM_책정보");
		CatApi = new CCatApi_SE(this);
		strHD_Book = _T("SB");
	}
	
	CatApi->InitMemberData();

	CESL_DataMgr m_DM;
	m_DM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);
	m_DM.InitDMPKFromFile(strDMAlias);
	m_DM.SetMarcMgr(m_pInfo->pMarcMgr);

	CString strBookWhere;
	CESL_DataMgr *pDM_Where = FindDM(_T("DM_BO_CAT_PROC_WHERE"));
	if (pDM_Where)
		strBookWhere = pDM_Where->GetCellData(_T("책조건절"), 0);

	if (strBookWhere.Find(_T("A.")) >= 0)
	{
		m_DM.TBL_NAME += _T(" , CO_ACCESSION_REC_TBL A ");
	}
	if (strBookWhere.Find(_T("P.")) >= 0)
	{
		m_DM.TBL_NAME += _T(" , BO_PURCHASE_SEQ_NO_TBL p ");
	}
	if (strBookWhere.Find(_T("I.")) >= 0)
	{
		m_DM.TBL_NAME += _T(" , IDX_BO_TBL I ");
		strBookWhere += _T(" AND I.REC_KEY = B.SPECIES_KEY ");
	} 

	INT ids = -1;

	ids = m_pGrid_Parent->SelectMakeList();
	if (ids < 0) return ids;

	INT nRowCount = m_pGrid_Parent->SelectGetCount();
	if (nRowCount <= 0) 
	{
		m_strLastErrMsg = _T("선정된 리스트가 없습니다.");
		return -1;
	}

	CMap<CString, LPCTSTR, INT, INT> mapShelf;

	CString strBookKey;
	CString strSpeciesKey;
	CString strOption;

	CString strShelfLocCode;
	INT nBookCnt;

	INT nIdx = -1;
	INT nSelectedIdx = m_pGrid_Parent->SelectGetHeadPosition();
	while (nSelectedIdx >= 0)
	{
		nIdx = nSelectedIdx;
		nSelectedIdx = m_pGrid_Parent->SelectGetNext();

		ids = m_pDM_Parent->GetCellData(m_strParentSpeciesKeyAlias, nIdx, strSpeciesKey);
		if (ids < 0 || strSpeciesKey.IsEmpty()) continue;

		m_arrStrSpeciesKey.Add(strSpeciesKey);

		strOption.Format(_T("SPECIES_KEY = %s "), strSpeciesKey );

		if ( strBookWhere != _T("") )
			strOption += _T(" AND ") + strBookWhere;

		strOption = strOption + _T(" AND B.SHELF_DATE IS NULL") ;
		ids = m_DM.RefreshDataManager(strOption);
		if (ids < 0) continue;

		nRowCount = m_DM.GetRowCount();
		for (INT idx = 0; idx < nRowCount; idx++)
		{
			nBookCnt = -1;
			strShelfLocCode.Empty();
			
			ids = m_DM.GetCellData( strHD_Book + _T("_배가위치부호"), idx, strShelfLocCode);
			if (ids < 0 || strShelfLocCode.IsEmpty()) continue;
			
			ids = m_DM.GetCellData( strHD_Book + _T("_책KEY"), idx, strBookKey);
			if (ids < 0 || strBookKey.IsEmpty()) continue;

			if (mapShelf.Lookup(strShelfLocCode, nBookCnt))
				mapShelf.SetAt(strShelfLocCode, ++nBookCnt);
			else
				mapShelf.SetAt(strShelfLocCode, 1);			

			m_arrStrBookKey.Add(strBookKey);
		}
	}

	// 그리드에 보여준다.
	nRowCount = mapShelf.GetCount();
	if (nRowCount <= 0) 
	{
		m_strLastErrMsg = _T("배가기록을 할 자료가 없습니다.");
		return -1;
	}

	m_pGrid->SetRows(nRowCount + 1);

	m_pGrid->SetColWidth(0, 0, 0);

	INT idx = 1;
	CString strBookCnt;
	POSITION pos = mapShelf.GetStartPosition();
	while (pos)
	{
		mapShelf.GetNextAssoc(pos, strShelfLocCode, nBookCnt);
		strBookCnt.Format(_T("%d"), nBookCnt);

		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료실구분"), strShelfLocCode, strShelfLocCode);

		m_pGrid->SetTextMatrix(idx, 1, strShelfLocCode);
		m_pGrid->SetTextMatrix(idx, 2, strBookCnt);
		idx++;
	}

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(m_pInfo->CONNECTION_INFO);

	CString sCenterUploadYN;

	CString sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
					 _T("WHERE GROUP_1='L' AND GROUP_2='기타' AND GROUP_3='공통' ")
					 _T("AND CLASS_ALIAS='센터업로드사용여부' AND VALUE_NAME='CENTER_UPLOAD' ")
					 _T("AND MANAGE_CODE=UDF_MANAGE_CODE");

	TmpDM.GetOneValueQuery(sQuery, sCenterUploadYN);

	if ( _T("N") == sCenterUploadYN || _T("DM_연속_정리DM_책정보") == strDMAlias) 
	{
		GetDlgItem( IDC_chkCENTER_UPLOAD_BC )->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->SetCheck(0);
		m_bCenterUpload = FALSE;
	}
	else 
	{
		GetDlgItem( IDC_chkCENTER_UPLOAD_BC )->ShowWindow(TRUE);
		((CButton*)GetDlgItem(IDC_chkCENTER_UPLOAD_BC))->SetCheck(1);
		m_bCenterUpload = TRUE;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CLocRecordDlg message handlers

BOOL CLocRecordDlg::OnInitDialog() 
{
	EFS_BEGIN
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_단행_정리_배가일자기록")) < 0)
	{
		m_strLastErrMsg = _T("다이얼로그 초기화 실패");
		goto ERR;
	}
	
	m_pCM = FindCM(_T("CM_단행_정리_배가일자기록"));
	if (m_pCM == NULL)
	{
		m_strExeToolBarAlias = _T("CM 찾기 실패");
		goto ERR;
	}

	m_pGrid = static_cast<CESL_Grid*>(m_pCM->FindControl(_T("GRID_배가실")));
	if (m_pGrid == NULL)
	{
		m_strExeToolBarAlias = _T("GRID 찾기 실패");
		goto ERR;
	}

	if (Init() < 0) goto ERR;

	EnableThemeDialogTexture(GetSafeHwnd());	
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

ERR:
	MessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;

EFS_END
return FALSE;
}

VOID CLocRecordDlg::OnbSAVE() 
{
	UpdateData();
	if ( m_bCatalogComplete == TRUE)
	{
		EndDialog(IDOK);
		return;
	}

	INT ids = -1;
	INT idx = -1;
	INT nRowCount = -1;
	CString strRecKey;
	CString strQuery;
	CString strShelfDate;

	m_pCM->GetControlMgrData(_T("배가일자"), strShelfDate);

	CCatApi *CatApi;

	if (m_nModeMONO_SERIAL == MODE_MONO)
		CatApi = new CCatApi_BO(this);
	else
		CatApi = new CCatApi_SE(this);
	
	CatApi->InitMemberData();
	
	ids = CatApi->RecordLocDate(m_pDM_Parent, &m_arrStrSpeciesKey, &m_arrStrBookKey, NULL, strShelfDate, TRUE);
	if (ids < 0)
	{
		AfxMessageBox(_T("배가일자를 기록하지 못했습니다."));
		return;
	}
	
	if (m_bCenterUpload)
	{
		CString strBaseDM = _T("DM_CENTERUPLOAD_01");		
		CString strDupDM = _T("DM_CENTERUPLOAD_복본자료");

		if ( m_nModeMONO_SERIAL == MODE_SERIAL ) {
			strBaseDM = _T("DM_SE_CENTERUPLOAD_01");
			strDupDM = _T("DM_SE_CENTERUPLOAD_복본자료");
		}
				
		CESL_DataMgr *pDM_Basic = FindDM(strBaseDM);
		if (pDM_Basic == NULL) return;
		
		CString strWhere;
		nRowCount = m_arrStrSpeciesKey.GetSize();		
		for (idx = 0; idx < nRowCount; idx++)
		{
			strRecKey = m_arrStrSpeciesKey.GetAt(idx);
			if (idx == 0)
				strWhere.Format(_T("I.REC_KEY IN (%s "), strRecKey);
			else
				strWhere += _T(",") + strRecKey;

			if ( idx == 999 ) {				
				break;
			}
		}
		strWhere += _T(")");

		if (pDM_Basic->RefreshDataManager(strWhere) < 0)	return;

		CArray<CString, CString&> aSpeciesKeyList;
		CString sSpeciesKeyList;
		if ( nRowCount > 1000 ) {
			CString strBaseDM1000 = _T("DM_CENTERUPLOAD_01_1000건");

			if ( m_nModeMONO_SERIAL == MODE_SERIAL ) {
				strBaseDM1000 = _T("DM_SE_CENTERUPLOAD_01_1000건");				
			}

			CESL_DataMgr *pDM_Basic1000 = FindDM(strBaseDM1000);
			if (pDM_Basic1000 == NULL) return;

			INT iCount = 0;

			INT iInsertIdx = 0;
			for ( INT i = idx ; i < nRowCount ; i++ ) {				
				strRecKey = m_arrStrSpeciesKey.GetAt(i);
				sSpeciesKeyList = sSpeciesKeyList + _T(", ") + strRecKey;
				iCount++;
				if ( iCount == 1000 ) {
					sSpeciesKeyList = sSpeciesKeyList.Mid(2);
					aSpeciesKeyList.Add( sSpeciesKeyList );

					strWhere.Format(_T("I.REC_KEY IN (%s)"), sSpeciesKeyList );
					if (pDM_Basic1000->RefreshDataManager(strWhere) < 0)	return;
					for ( INT j = 0 ; j < pDM_Basic1000->GetRowCount(); j++ ) {
						iInsertIdx = pDM_Basic->GetRowCount();
						pDM_Basic->InsertRow(-1);						
						for ( INT k = 0 ; k < pDM_Basic1000->GetColCount(); k++ ) {
							CString sData = pDM_Basic1000->GetCellData( j, k );
							pDM_Basic->SetCellData( iInsertIdx, sData, k );
						}
					}

					sSpeciesKeyList = _T("");
					iCount = 0;
				}
			}
			sSpeciesKeyList = sSpeciesKeyList.Mid(2);
			strWhere.Format(_T("I.REC_KEY IN (%s)"), sSpeciesKeyList );
			if (pDM_Basic1000->RefreshDataManager(strWhere) < 0)	return;
			for ( INT j = 0 ; j < pDM_Basic1000->GetRowCount(); j++ ) {
				iInsertIdx = pDM_Basic->GetRowCount();
				pDM_Basic->InsertRow(-1);
				for ( INT k = 0 ; k < pDM_Basic1000->GetColCount(); k++ ) {
					CString sData = pDM_Basic1000->GetCellData( j, k );
					pDM_Basic->SetCellData( iInsertIdx, sData, k );
				}
			}
		}
		
		nRowCount = pDM_Basic->GetRowCount();
		
		CBO_CAT_CENTERUPLOAD dlg(this);
		dlg.SetDMAlias(strBaseDM, strDupDM, _T(""));
		dlg.CenterUpload(FALSE) ;
		
	}

	AfxMessageBox(_T("배가일자를 기록하였습니다."), MB_ICONINFORMATION, MB_OK);

	EndDialog(IDOK);
}

BEGIN_EVENTSINK_MAP(CLocRecordDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CLocRecordDlg)
	ON_EVENT(CLocRecordDlg, IDC_GRID_CAT, -605 /* MouseDown */, OnMouseDownGridCat, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CLocRecordDlg::OnMouseDownGridCat(short Button, short Shift, long x, long y) 
{
	m_pGrid->SetReverse(m_pGrid->GetRow() - 1);	
}

HBRUSH CLocRecordDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
