// BO_CAT_AUTO_REGNO_GIVE.cpp : implementation file
//

#include "stdafx.h"
//#include _T("marcadjust.h")
#include "BO_CAT_AUTO_REGNO_GIVE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_AUTO_REGNO_GIVE dialog


CBO_CAT_AUTO_REGNO_GIVE::CBO_CAT_AUTO_REGNO_GIVE(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	m_cModeMONO_SERIAL = cMODE_MONO;
	//{{AFX_DATA_INIT(CBO_CAT_AUTO_REGNO_GIVE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_AUTO_REGNO_GIVE::~CBO_CAT_AUTO_REGNO_GIVE()
{
	
}



VOID CBO_CAT_AUTO_REGNO_GIVE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_AUTO_REGNO_GIVE)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_AUTO_REGNO_GIVE, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_AUTO_REGNO_GIVE)
	ON_BN_CLICKED(IDC_bAutoRegnoGive, OnbAutoRegnoGive)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_AUTO_REGNO_GIVE message handlers

BOOL CBO_CAT_AUTO_REGNO_GIVE::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_단행_정리_자동등록번호부여")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CString strDMBook_Alias;
	CString strHD_Book;
	CString strDMLastNo_Alias;
	if ( m_cModeMONO_SERIAL == cMODE_MONO )
	{
		strDMBook_Alias = _T("DM_단행_정리DM_책정보");
		strDMLastNo_Alias = _T("DM_마지막번호");
		strHD_Book = _T("BB");
	}
	else 
	{
		strDMBook_Alias = _T("DM_연속_정리DM_책정보");
		strDMLastNo_Alias = _T("DM_연속_마지막번호");
		strHD_Book = _T("SB");
	}

	INT ids = 0;
	CESL_DataMgr *pDmBook = FindDM(strDMBook_Alias);
	if (pDmBook == NULL) {
		AfxMessageBox( strDMBook_Alias + _T(" 확인") );
		return FALSE;
	}

	CESL_DataMgr *pDmLastNum = FindDM( strDMLastNo_Alias );
	if (pDmLastNum == NULL) {
		AfxMessageBox( strDMLastNo_Alias + _T(" 확인") );
		return -1;
	}

	INT iBookCount = pDmBook->GetRowCount();
	CString sRegCode;
	CString sRegNo;

	pDmBook->SORT( strHD_Book + _T("_등록구분_코드") );

	for ( INT i = 0 ; i < iBookCount ; i++ ) {
		if (pDmBook->GetCellData( strHD_Book + _T("_DB_FLAG"), i )==_T("D"))
			continue;

		sRegNo = pDmBook->GetCellData( strHD_Book + _T("_등록번호"), i );
		if ( sRegNo == _T("") ) {
			sRegCode = pDmBook->GetCellData( strHD_Book + _T("_등록구분_코드"), i );
			if ( sRegCode == _T("") ) continue;
			INT iSwFind = 0;
			for ( INT j = 0 ; j < m_aRegCode.GetSize() ; j++ ) {
				if ( m_aRegCode.GetAt(j) == sRegCode ) {
					INT iTmpCount = m_aBookCount.GetAt(j);
					iTmpCount++;
					m_aBookCount.SetAt( j, iTmpCount );
					iSwFind = 1;
					break;
				}
			}
			if ( iSwFind == 0 ) {
				m_aRegCode.Add( sRegCode );
				m_aBookCount.Add( 1 );
			}
		}
	}

	if (m_cModeMONO_SERIAL == cMODE_SERIAL)
	{
		CESL_DataMgr *pDmAppendix = FindDM( _T("DM_연속_정리DM_부록정보") );
		if (pDmBook == NULL) {
			AfxMessageBox( _T("[DM_연속_정리DM_부록정보] 확인") );
			return FALSE;
		}
		INT iAppendixCount = pDmAppendix->GetRowCount();
		for ( INT i = 0 ; i < iAppendixCount ; i++ ) {
			if (pDmAppendix->GetCellData( strHD_Book + _T("_DB_FLAG"), i )==_T("D"))
				continue;

			sRegNo = pDmAppendix->GetCellData( strHD_Book + _T("_등록번호"), i );
			if ( sRegNo == _T("") ) {
				sRegCode = pDmAppendix->GetCellData( strHD_Book + _T("_등록구분_코드"), i );
				if ( sRegCode == _T("") ) continue;
				INT iSwFind = 0;
				for ( INT j = 0 ; j < m_aRegCode.GetSize() ; j++ ) {
					if ( m_aRegCode.GetAt(j) == sRegCode ) {
						INT iTmpCount = m_aBookCount.GetAt(j);
						iTmpCount++;
						m_aBookCount.SetAt( j, iTmpCount );
						iSwFind = 1;
						break;
					}
				}
				if ( iSwFind == 0 ) {
					m_aRegCode.Add( sRegCode );
					m_aBookCount.Add( 1 );
				}
			}
		}
	}

	INT iBookCount_tot = 0;

	CESL_ControlMgr *pCM = FindCM(_T("CM_단행_정리_자동등록번호부여"));
	if ( pCM == NULL ) {
		AfxMessageBox( _T("CM_BO_CAT_REG error") );
		return FALSE;
	}

	CESL_Grid * pGrid_sub = NULL;
	pGrid_sub = (CESL_Grid*)pCM->FindControl(_T("AutoGiveRegno"));
	pGrid_sub->SetRows( m_aRegCode.GetSize() + 1 );
	pGrid_sub->SetSort(FALSE);

	for ( i = 0 ; i < m_aRegCode.GetSize() ; i++ ) {
		CString sRegCode = m_aRegCode.GetAt( i );
		CString sBookCount;
		sBookCount.Format( _T("%d"), m_aBookCount.GetAt(i) );
		pGrid_sub->SetTextMatrix( i+1, 1, sRegCode);
		pGrid_sub->SetTextMatrix( i+1, 2, sBookCount);

		CString sQuery = _T("SELECT LAST_NUMBER FROM CO_LAST_NUMBER_TBL ")
						 _T("WHERE KIND_CODE='REG_NO' AND DIVISION_VALUE='") + sRegCode + 
						 _T("' AND MANAGE_CODE=UDF_MANAGE_CODE");
	
		CString sLastNum;	
		for ( INT j = 0 ; j < pDmLastNum->GetRowCount() ; j++ ) {
			if ( sRegCode == pDmLastNum->GetCellData(strHD_Book + _T("_등록구분_코드"), j) ) {
				sLastNum = pDmLastNum->GetCellData( _T("마지막번호"), j );
			}
		}
		if ( sLastNum == _T("") ) {
			pDmBook->GetOneValueQuery( sQuery , sLastNum );
		}
		m_aLastNum.Add( sLastNum );

		if ( sLastNum == _T("") ) {
			m_aLastNum.SetAt( i, _T("0") );

			ids = pDmBook->StartFrame();
			if ( 0 > ids ) return -1;
			CString sQuery_reg;
			CTime time = CTime::GetCurrentTime();
			CString sYear;
			sYear.Format( _T("%04d"), time.GetYear() );
			sQuery_reg.Format(	_T("INSERT INTO CO_LAST_NUMBER_TBL ")
								_T("(REC_KEY, PUBLISH_FORM_CODE, KIND_CODE, DIVISION_VALUE, MAKE_YEAR, ")
								_T("LAST_NUMBER, MANAGE_CODE) ")
								_T("VALUES(ESL_SEQ.NEXTVAL, 'CO', 'REG_NO', '%s', '%s', '0', UDF_MANAGE_CODE);") 
								, sRegCode, sYear );

			ids = pDmBook->AddFrame( sQuery_reg );
			if ( 0 > ids ) return -1;

			ids = pDmBook->SendFrame();			
			if (ids < 0) {
				AfxMessageBox(_T("입력사항을 DataBase에 적용하던중 에러가 발생하였습니다(DM_REG)."));
				return FALSE;
			}
			ids = pDmBook->EndFrame();
			if ( 0 > ids ) return -1;
		}

		INT iLastNum = _ttoi( sLastNum.GetBuffer(0) );
		iLastNum ++;		
		sLastNum.Format( _T("%d") , iLastNum );
		
		INT nRegCodeLength;
		nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	
		CString sNum10;
		sNum10.Format ( _T("%d") , iLastNum );

		CString sStartRegNo, sEndRegNo;
		sStartRegNo = m_pInfo->MakeRegNo(sRegCode, sNum10);
		sNum10 = sStartRegNo.Mid(nRegCodeLength);
		
		INT iBookCount;
		iBookCount = _ttoi(sBookCount.GetBuffer(0));
		iLastNum = iLastNum + iBookCount -1 ;
		iBookCount_tot = iBookCount_tot + iBookCount;
		CString sTmp;
		sTmp.Format( _T("%d") , iLastNum );		

		sEndRegNo = m_pInfo->MakeRegNo( sRegCode,sTmp);
		sTmp = sEndRegNo.Mid(nRegCodeLength);		
		sLastNum = sNum10 + _T(" ~ ") + sEndRegNo;
		pGrid_sub->SetTextMatrix( i+1, 3, sRegCode + sLastNum);
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_AUTO_REGNO_GIVE::OnbAutoRegnoGive() 
{
	CString strDMBook_Alias, strDMAppendix_Alias, strDMLastNum_Alias;
	CString strHD_Book;
	if (m_cModeMONO_SERIAL == cMODE_MONO)
	{
		strDMBook_Alias = _T("DM_단행_정리DM_책정보");
		strDMAppendix_Alias = _T("DM_단행_정리DM_부록정보");
		strDMLastNum_Alias = _T("DM_마지막번호");
		strHD_Book = _T("BB");
	}
	else 
	{
		strDMBook_Alias = _T("DM_연속_정리DM_책정보");
		strDMAppendix_Alias = _T("DM_연속_정리DM_부록정보");
		strDMLastNum_Alias = _T("DM_연속_마지막번호");
		strHD_Book = _T("SB");
	}

	CESL_DataMgr *pDmBook = FindDM(strDMBook_Alias);
	if (pDmBook == NULL) {
		AfxMessageBox( strDMBook_Alias + _T(" 확인") );
		return;
	}
	SetRegCodeToBookDM(pDmBook, strHD_Book);

	if (m_cModeMONO_SERIAL == cMODE_SERIAL)
	{
		CESL_DataMgr *pDmAppendix = FindDM(strDMAppendix_Alias);
		if (pDmAppendix == NULL) {
			AfxMessageBox( strDMAppendix_Alias + _T(" 확인") );
			return;
		}
		SetRegCodeToBookDM( pDmAppendix, strHD_Book );
	}

	CESL_DataMgr *pDmLastNum = FindDM( strDMLastNum_Alias );
	if (pDmLastNum == NULL) {
		AfxMessageBox( strDMLastNum_Alias + _T(" 확인") );
		return;
	}

	for ( INT i = 0 ; i < m_aLastNum.GetSize() ; i++ ) {
		CString sLastNum = m_aLastNum.GetAt(i);
		CString sRegCode = m_aRegCode.GetAt(i);
		INT iYes = 0;
		for ( INT j = 0 ; j < pDmLastNum->GetRowCount() ; j++ ) {
			if ( sRegCode == pDmLastNum->GetCellData(strHD_Book + _T("_등록구분_코드"),j) ) {
				pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, j );
				iYes = 1;
				break;
			}
		}
		if ( iYes == 0 ) {
			pDmLastNum->InsertRow(-1);
			pDmLastNum->SetCellData( _T("마지막번호"), sLastNum, i );
			pDmLastNum->SetCellData( strHD_Book + _T("_등록구분_코드"), sRegCode, i );
		}
	}

	EndDialog(IDOK);
}

INT CBO_CAT_AUTO_REGNO_GIVE::SetRegCodeToBookDM(CESL_DataMgr *pDmBook, CString strHD_Book)
{
	INT i,j, k;
	CStringArray aLastNum;
	for ( k = 0 ; k < m_aRegCode.GetSize(); k++ ) {
		CString sRegCode = m_aRegCode.GetAt(k);
		CString sNum = m_aLastNum.GetAt(k);
	}
	for ( i = 0 ; i < pDmBook->GetRowCount() ; i++ ) {

		if (pDmBook->GetCellData( strHD_Book + _T("_DB_FLAG"), i )==_T("D"))
			continue;

		CString sRegNo = pDmBook->GetCellData( strHD_Book + _T("_등록번호"), i );
		CString sRegCode = pDmBook->GetCellData( strHD_Book + _T("_등록구분_코드"), i );
		if ( sRegNo == _T("") && sRegCode != _T("") ) {
			for ( j = 0 ; j < m_aRegCode.GetSize() ; j++ ) {
				if ( sRegCode == m_aRegCode.GetAt(j) ) {
					INT iLastNum = _ttoi( m_aLastNum.GetAt(j) );
					iLastNum++;
					CString sLastNum;
					INT nRegCodeLength;
					nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
					sLastNum.Format( _T("%d"), iLastNum );
					sRegNo = m_pInfo->MakeRegNo(sRegCode,sLastNum);
					sLastNum = sRegNo.Mid(nRegCodeLength);
					pDmBook->SetCellData( strHD_Book + _T("_등록번호"), sRegNo, i );
					m_aLastNum.SetAt( j, sLastNum );
				}
			}
		}
	}
	
	return 0;
}
HBRUSH CBO_CAT_AUTO_REGNO_GIVE::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
