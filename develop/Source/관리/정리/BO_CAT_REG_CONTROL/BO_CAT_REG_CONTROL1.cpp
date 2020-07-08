// BO_CAT_REG_CONTROL1.cpp : implementation file
//

#include "stdafx.h"
//#include _T("	\ add additional includes here")
#include "BO_CAT_REG_CONTROL1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_CONTROL dialog


CBO_CAT_REG_CONTROL::CBO_CAT_REG_CONTROL(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_REG_CONTROL)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_REG_CONTROL::~CBO_CAT_REG_CONTROL()
{
}

BOOL CBO_CAT_REG_CONTROL::Create(CWnd* pParentWnd)
{
EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_CONTROL::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_REG_CONTROL)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_REG_CONTROL, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_REG_CONTROL)
	ON_BN_CLICKED(IDC_bOK, OnbOK)
	ON_BN_CLICKED(IDC_bRegTOFILE, OnbRegTOFILE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_REG_CONTROL message handlers

BOOL CBO_CAT_REG_CONTROL::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_REG_CONTROL")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pAccCan;
	pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	pAccCan->SetCheck(1);	

	CButton * pPART;
	pPART=(CButton *)GetDlgItem(IDC_rPART);
	pPART->SetCheck(1);	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_REG_CONTROL::OnbOK() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	if (pDM == NULL) return;
	pDM->FreeData();

	pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	if (pDM == NULL) return;

	CString sControlCode;
	GetControlData( _T("CM_BO_CAT_REG_CONTROL"), _T("BS_자료구분_코드"), sControlCode );
	CString sYear;
	GetControlData( _T("CM_BO_CAT_REG_CONTROL"), _T("년도"), sYear );
	if ( sYear == _T("") ) {
		AfxMessageBox( _T("년도를 입력하세요.") );
		return;
	}
	//CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pDM->CONNECTION_INFO);
	CString sQuery;
	if ( sControlCode == _T("") ) {
		sQuery = _T("SELECT DISTINCT CONTROL_NO FROM BO_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='") + sYear + _T("' AND CONTROL_NO IS NOT NULL ORDER BY CONTROL_NO");
	}
	else {
		sQuery = _T("SELECT DISTINCT CONTROL_NO FROM BO_SPECIES_TBL WHERE SUBSTR(CONTROL_NO,4,4)='") + sYear + _T("' AND SUBSTR(CONTROL_NO,0,3)='") + sControlCode + _T("' AND CONTROL_NO IS NOT NULL ORDER BY CONTROL_NO");
	}
	//ACQUISIT_YEAR

	CButton * pAccCan;
	pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);

	dm.RestructDataManager(sQuery);
	INT iRowCount = dm.GetRowCount();
	if ( iRowCount < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		//return;
	}
	if ( pAccCan->GetCheck() == 1 ) {
		CString sRowCount;
		sRowCount.Format( _T("%d") , iRowCount );
		SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("총종수"), sRowCount);

		CString strRegClass = sControlCode;
		//CString strRegClass = sRegCode;
		CString strRegNo;
		INT		nRegNo = -1;

		INT nStart = -1;
		INT nEnd = -1;

		for ( INT i = 0 ; i < iRowCount ; i ++ ) {		
			//strRegNo = pDM->GetCellData(_T("BB_등록번호"), i);
			strRegNo = dm.GetCellData( i , 0 );
			//strRegNo.Replace(_T("EM"), NULL);
			strRegNo.Replace(strRegClass, NULL);
			nRegNo = _ttoi(strRegNo);

			nStart = nRegNo;
			nEnd = nRegNo;

			// check next val
			while (TRUE)
			{
				//strRegNo = pDM->GetCellData(_T("BB_등록번호"), ++i);
				//i++;
				//i = i-1;
				++i;
				if ( i == iRowCount ) {
					break;
				}
				strRegNo = dm.GetCellData( i , 0 );
				//strRegNo = dm.GetCellData( ++i-1 , 0 );
				//strRegNo.Replace(_T("EM"), NULL);
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);

				if (nEnd + 1 == nRegNo)
				{
					nEnd++;	
				}
				else
				{
					i--;
					break;
				}
			}

			DisplayRegNo(strRegClass, nStart, nEnd);
		}
		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
		pCM->AllControlDisplay();
	}
	else {	//누락제어번호 리스트
		/*
		dm.RestructDataManager(sQuery);
		INT iRowCount = dm.GetRowCount();
		if ( iRowCount < 1 ) {
			AfxMessageBox (_T("검색된 결과가 없습니다."));
			//return;
		}
		*/

		//CString sRowCount;
		//sRowCount.Format( _T("%d") , iRowCount );
		//SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("총종수"), sRowCount);

		CString strRegClass = sControlCode;
		
		CString strRegNo;
		INT		nRegNo = -1;

		INT nStart = -1;
		INT nEnd = -1;
		BOOL bStart = FALSE;

		INT iCount = 0;
		for ( INT i = 0 ; i < iRowCount ; i ++ ) {
			if ( i == 0 ) {
				strRegNo = dm.GetCellData( i , 0 );
				
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);
				CString sStartNum = strRegNo.Right(5);
				INT iStartNum = _ttoi( sStartNum );

				nStart = nRegNo;
				nEnd = nRegNo-1;

				if ( strRegNo != _T("") && i == 0 && iStartNum != 1 ) {
					CString sStart;
					sStart.Format( _T("%s00001"), sYear );
					nStart = _ttoi( sStart );
					DisplayRegNo(strRegClass, nStart, nEnd);
					bStart = true;					
					iCount = iStartNum -1;
					continue;
				}
			}

			nStart = nRegNo;

			// check next val
			while (TRUE)
			{			
				//++i;
				if ( i == iRowCount ) {
					break;
				}
				strRegNo = dm.GetCellData( i , 0 );
				//strRegNo = dm.GetCellData( ++i-1 , 0 );
				//strRegNo.Replace(_T("EM"), NULL);
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);
				//nStart = nRegNo;
				++i;

				if (nStart + 1 == nRegNo)
				{
					//nEnd++;
					nStart++;
					iCount++;
				}
				else
				{
					//i--;
					nStart++;
					i--;
					iCount++;
					break;
				}
			}
			nEnd = nRegNo - 1;

			DisplayRegNo(strRegClass, nStart, nEnd);
		}
		//총수 구하기
		//strRegNo = dm.GetCellData( i-1 , 0 );		
		//strRegNo.Right(5);
		//nRegNo = _ttoi(strRegNo);

		iCount = _ttoi( strRegNo.Right(5) );
		iCount = iCount - iRowCount;
		CString sRowCount;
		sRowCount.Format( _T("%d") , iCount );
		SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("총종수"), sRowCount);

		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
		pCM->AllControlDisplay();
	}

	


EFS_END
}

INT CBO_CAT_REG_CONTROL::DisplayRegNo(CString strRegClass, INT nStart, INT nEnd)
{
EFS_BEGIN

	if (strRegClass.IsEmpty()) return -1;
	if (nStart > nEnd) return -1;

	/*CESL_ControlMgr *pCM = FindCM(_T("CM_BO_CAT_REG_0320"));
	if (pCM == NULL) return -1;

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(_T("GRD_REG_0320"));
	if (pGrid == NULL) return -1;
	*/
	//CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_0320_GRD"));
	CESL_DataMgr *pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	
	if (pDM == NULL) return -1;

	CString strAlias;

	INT nColCount = pDM->RefList.GetCount();
	INT nRowCount = pDM->GetRowCount();

	strAlias.Format(_T("제어번호%d"), nColCount);
	if (nRowCount == 0 || 
		!pDM->GetCellData(strAlias, nRowCount -1).IsEmpty())
	{
		pDM->InsertRow(-1);
		nRowCount++;
	}

	CString strData;
	for (INT i = 0; i < nColCount; i++)
	{
		strAlias.Format(_T("제어번호%d"), i + 1);
		if (pDM->GetCellData(strAlias, nRowCount - 1).IsEmpty())
		{
			// Insert Data
			if (nStart == nEnd)
			{
				strData.Format(_T("%s%09d"), strRegClass, nStart);
			}
			else if (nStart < nEnd)
			{
				//strData.Format(_T("%s%010d ~ %s%010d"), strRegClass, nStart, strRegClass, nEnd);
				strData.Format(_T("%s%09d ~ %s%09d"), strRegClass, nStart, strRegClass, nEnd);
			}
			pDM->SetCellData(strAlias, strData, nRowCount -1);
			break;
		}
	}
	
	return 0;

EFS_END
return -1;

}

VOID CBO_CAT_REG_CONTROL::OnbRegTOFILE() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM;
	//pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	//if (pDM == NULL) return;
	//pDM->FreeData();

	pDM = FindDM(_T("DM_BO_CAT_REG_CONTROL"));
	if (pDM == NULL) return;

	INT iRowCount = pDM->GetRowCount();
	//iRowCount = dm.GetRowCount();
	if ( iRowCount < 1 ) {
		return;
	}

	CString sControlCode;
	GetControlData( _T("CM_BO_CAT_REG_0320"), _T("BS_자료구분_코드"), sControlCode );
	CString sYear;
	GetControlData( _T("CM_BO_CAT_REG_0320"), _T("년도"), sYear );
	CString sTotal;
	GetControlData( _T("CM_BO_CAT_REG_0320"), _T("총종수"), sTotal );

	//파일 만들기
	CFile file;
	if ( ::CreateDirectory( _T("..\\전산실"), NULL ) == FALSE ) {
		//AfxMessageBox( _T("폴더 만들기 실패") );
	}
	
	//CFile *fp;
	FILE *fp;
	//TCHAR cLine[256];
	CString sPath = _T("..\\전산실\\KMO2003_Build.txt");
	CButton * pAccCan;
	pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	if ( pAccCan->GetCheck() == 1 ) {
		sPath = _T("..\\전산실\\") + sControlCode + sYear + _T("_Build.txt");
	}
	else {
		sPath = _T("..\\전산실\\") + sControlCode + sYear + _T("_Omission.txt");
	}
	fp = _tfopen( sPath , _T("wb") );
	if ( fp == NULL ) {
		AfxMessageBox( _T("KMO2003_Build.txt 파일이 없습니다.") );
		return;
	}
	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	
	_ftprintf( fp, _T("-----------------------------------------------------------------------------------\r\n") );
	CString sLine = _T(" ### ") + sControlCode + sYear + _T(" ### \r\n");
	_ftprintf( fp, sLine );
	_ftprintf( fp, _T("-----------------------------------------------------------------------------------\r\n") );
	
	for ( INT i = 0 ; i < iRowCount ; i++ ) {		
		CString sControlNo1 = pDM->GetCellData( _T("제어번호1"), i );
		if ( sControlNo1.GetLength() == 12 ) {
			sControlNo1 = sControlNo1 + _T("               ");
		}
		CString sControlNo2 = pDM->GetCellData( _T("제어번호2"), i );
		if ( sControlNo2.GetLength() == 12 ) {
			sControlNo2 = sControlNo2 + _T("               ");
		}
		CString sControlNo3 = pDM->GetCellData( _T("제어번호3"), i );
		if ( sControlNo3.GetLength() == 12 ) {
			sControlNo3 = sControlNo3 + _T("               ");
		}
		CString sData = sControlNo1 + _T(" ") + sControlNo2 + _T(" ") + sControlNo3 + _T("\r\n");
		_ftprintf( fp, sData );
	}
	_ftprintf( fp, _T("\r\n-----------------------------------------------------------------------------------\r\n") );
	sTotal = _T("  소 계 :  ") + sTotal;
	_ftprintf( fp, sTotal );
	_ftprintf( fp, _T("\r\n-----------------------------------------------------------------------------------\r\n") );
	fclose( fp );

	CString sMessage = _T("[ ") + sPath + _T(" ] 파일에 저장 작업이 완료되었습니다.") ;
	AfxMessageBox( sMessage );

	return;



	//CButton * pAccCan;
	pAccCan=(CButton *)GetDlgItem(IDC_rCONTROLNO_LIST);
	if ( pAccCan->GetCheck() == 1 ) {
		CString sRowCount;
		sRowCount.Format( _T("%d") , iRowCount );
		//SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("총종수"), sRowCount);

		CString strRegClass = sControlCode;
		//CString strRegClass = sRegCode;
		CString strRegNo;
		INT		nRegNo = -1;

		INT nStart = -1;
		INT nEnd = -1;

		for ( INT i = 0 ; i < iRowCount ; i ++ ) {		
			//strRegNo = pDM->GetCellData(_T("BB_등록번호"), i);
			strRegNo = dm.GetCellData( i , 0 );
			//strRegNo.Replace(_T("EM"), NULL);
			strRegNo.Replace(strRegClass, NULL);
			nRegNo = _ttoi(strRegNo);

			nStart = nRegNo;
			nEnd = nRegNo;

			// check next val
			while (TRUE)
			{
				//strRegNo = pDM->GetCellData(_T("BB_등록번호"), ++i);
				//i++;
				//i = i-1;
				++i;
				if ( i == iRowCount ) {
					break;
				}
				strRegNo = dm.GetCellData( i , 0 );
				//strRegNo = dm.GetCellData( ++i-1 , 0 );
				//strRegNo.Replace(_T("EM"), NULL);
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);

				if (nEnd + 1 == nRegNo)
				{
					nEnd++;	
				}
				else
				{
					i--;
					break;
				}
			}

			//DisplayRegNo(strRegClass, nStart, nEnd);
			CString strData;			
			strData.Format(_T("%s%09d ~ %s%09d"), strRegClass, nStart, strRegClass, nEnd);
			_ftprintf( fp, strData );
		}
		//CESL_ControlMgr *pCM;
		//pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
		//pCM->AllControlDisplay();
	}
	else {	//누락제어번호 리스트
		/*
		dm.RestructDataManager(sQuery);
		INT iRowCount = dm.GetRowCount();
		if ( iRowCount < 1 ) {
			AfxMessageBox (_T("검색된 결과가 없습니다."));
			//return;
		}
		*/

		//CString sRowCount;
		//sRowCount.Format( _T("%d") , iRowCount );
		//SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("총종수"), sRowCount);

		CString strRegClass = sControlCode;
		
		CString strRegNo;
		INT		nRegNo = -1;

		INT nStart = -1;
		INT nEnd = -1;
		BOOL bStart = FALSE;

		INT iCount = 0;
		for ( INT i = 0 ; i < iRowCount ; i ++ ) {
			if ( i == 0 ) {
				strRegNo = dm.GetCellData( i , 0 );
				
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);
				CString sStartNum = strRegNo.Right(5);
				INT iStartNum = _ttoi( sStartNum );

				nStart = nRegNo;
				nEnd = nRegNo-1;

				if ( strRegNo != _T("") && i == 0 && iStartNum != 1 ) {
					CString sStart;
					sStart.Format( _T("%s00001"), sYear );
					nStart = _ttoi( sStart );
					DisplayRegNo(strRegClass, nStart, nEnd);
					bStart = true;					
					iCount = iStartNum -1;
					continue;
				}
			}

			nStart = nRegNo;

			// check next val
			while (TRUE)
			{			
				//++i;
				if ( i == iRowCount ) {
					break;
				}
				strRegNo = dm.GetCellData( i , 0 );
				//strRegNo = dm.GetCellData( ++i-1 , 0 );
				//strRegNo.Replace(_T("EM"), NULL);
				strRegNo.Replace(strRegClass, NULL);
				nRegNo = _ttoi(strRegNo);
				//nStart = nRegNo;
				++i;

				if (nStart + 1 == nRegNo)
				{
					//nEnd++;
					nStart++;
					iCount++;
				}
				else
				{
					//i--;
					nStart++;
					i--;
					iCount++;
					break;
				}
			}
			nEnd = nRegNo - 1;

			DisplayRegNo(strRegClass, nStart, nEnd);
		}
		//총수 구하기
		//strRegNo = dm.GetCellData( i-1 , 0 );		
		//strRegNo.Right(5);
		//nRegNo = _ttoi(strRegNo);

		iCount = _ttoi( strRegNo.Right(5) );
		iCount = iCount - iRowCount;
		CString sRowCount;
		sRowCount.Format( _T("%d") , iCount );
		SetControlData(_T("CM_BO_CAT_REG_CONTROL"), _T("총종수"), sRowCount);

		CESL_ControlMgr *pCM;
		pCM = FindCM(_T("CM_BO_CAT_REG_CONTROL"));
		pCM->AllControlDisplay();
	}

	fclose( fp );

EFS_END
}

HBRUSH CBO_CAT_REG_CONTROL::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
