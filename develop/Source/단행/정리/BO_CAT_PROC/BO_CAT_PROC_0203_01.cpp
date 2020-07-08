// BO_CAT_PROC_0203_01.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0203_01.h"
#include "..\..\..\ESL\SIReportMgr\SIReportManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0203_01 dialog


CBO_CAT_PROC_0203_01::CBO_CAT_PROC_0203_01(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0203_01)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0203_01::~CBO_CAT_PROC_0203_01()
{
}

VOID CBO_CAT_PROC_0203_01::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0203_01)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0203_01, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0203_01)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0203_01 message handlers

BOOL CBO_CAT_PROC_0203_01::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}

BOOL CBO_CAT_PROC_0203_01::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0203_01")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	CButton * pPrint01;
	pPrint01=(CButton *)GetDlgItem(IDC_rLINE_PRINT);
	pPrint01->SetCheck(1);

	CButton * pPIECE_PAPER;
	pPIECE_PAPER=(CButton *)GetDlgItem(IDC_rPIECE_PAPER);
	pPIECE_PAPER->SetCheck(1);

	INT idx;
	SelectMakeList(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	idx = SelectGetHeadPosition(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	if (idx < 0)
	{
		return TRUE;		
	}

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_BO_CAT_PROC_0000 error") );
		return FALSE;
	}

	sRecKey = pDM->GetCellData( _T("BS_종KEY") , idx );
	idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));

	while ( idx != -1 ) {
		CString sRecKeyTmp = pDM->GetCellData( _T("BS_종KEY") , idx );
		sRecKey = sRecKey + _T(", ") + sRecKeyTmp;
		idx = SelectGetNext(_T("CM_BO_CAT_PROC_0000"), _T("ProcGrid"));
	}

	CESL_DataMgr * pDM_0203_01;
	pDM_0203_01 = FindDM( _T("DM_BO_CAT_PROC_0203_01") );
	if ( NULL == pDM_0203_01 ) {
		AfxMessageBox( _T("DM_BO_CAT_PROC_0203_01 error") );
		return FALSE;
	}

	CString strWhere = _T("SPECIES_KEY IN(") + sRecKey + _T(")");
	if (pDM_0203_01->RefreshDataManager(strWhere) < 0)	return false;
	INT iRowCount = pDM_0203_01->GetRowCount();
	if ( pDM_0203_01->GetRowCount() < 1 ) {
		AfxMessageBox (_T("검색된 결과가 없습니다."));
	}
	for ( INT i = pDM_0203_01->GetRowCount()-1 ; i >= 0 ; i-- ) {
		CString sRegNo = pDM_0203_01->GetCellData( _T("BB_등록번호"), i );
		if ( sRegNo == _T("") ) {
			pDM_0203_01->DeleteRow( i );
		}
	}

	CString sLibraryName;
	CString sQuery = _T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME ='도서관명'");
	pDM_0203_01->GetOneValueQuery( sQuery , sLibraryName );

	SetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("도서관명"), sLibraryName );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CBO_CAT_PROC_0203_01::OnOK() 
{
EFS_BEGIN

	CString sRegCode;
	GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("등록구분_코드"), sRegCode );
	if ( sRegCode == _T("") ) {

	}

	CString sRegNoStart, sRegNoEnd;
	GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("등록번호부터"), sRegNoStart );
	GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("등록번호까지"), sRegNoEnd );
	
	if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
		
		sRegNoStart = m_pInfo->MakeRegNo(sRegCode, sRegNoStart );
		sRegNoEnd = m_pInfo->MakeRegNo(sRegCode, sRegNoEnd );		
	}

	CESL_DataMgr *m_pDM = FindDM( _T("DM_BO_CAT_PROC_0203_01") );
	if ( m_pDM == NULL ) {
		AfxMessageBox ( _T("finddm error") );
		return;
	}


	CButton *pLINE_PRINT;
	pLINE_PRINT = (CButton *)GetDlgItem(IDC_rLINE_PRINT);
	CButton *pSERIAL_PRINT;
	pSERIAL_PRINT = (CButton *)GetDlgItem(IDC_rSERIAL_PRINT);
	CButton *pBARCODE;
	pBARCODE = (CButton *)GetDlgItem(IDC_rBARCODE);

	if ( pLINE_PRINT->GetCheck() == 1 || pSERIAL_PRINT->GetCheck() == 1 ) {	//줄바꿔찍기
		CESL_DataMgr *pDm0203;
		pDm0203 = FindDM(_T("DM_BO_CAT_PROC_0203"));
		if ( NULL == pDm0203 ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0203 error") );
			return;
		}

		CESL_DataMgr *pDmLabel;
		pDmLabel = FindDM(_T("DM_BO_CAT_PROC_0203_LABEL"));
		if ( NULL == pDmLabel ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0203_LABEL error") );
			return;
		}
		pDmLabel->FreeData();
		INT i0203Count = pDm0203->GetRowCount();
		INT iLabelCount = pDmLabel->GetRowCount();
		INT iCount = -1;
		if ( pLINE_PRINT->GetCheck() == 1 ) {			
			for ( INT i = 0 ; i < i0203Count ; i++ ) {
				CString sRegNo = pDm0203->GetCellData( _T("BB_등록번호"), i );
				if ( sRegNoStart != _T("") && sRegNoEnd == _T("") ) {
					if ( sRegNo != sRegNoStart ) {
						continue;
					}
				}
				else if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
					if ( sRegNo < sRegNoStart || sRegNo > sRegNoEnd ) {
						continue;
					}
				}
				else if ( sRegNoStart == _T("") && sRegNoEnd != _T("") ) {
					AfxMessageBox( _T("시작등록번호를 입력하세요.") );
					return;
				}

				CString sLabelType = pDm0203->GetCellData( _T("BB_레이블출력구분"), i );
				if ( sLabelType == _T("줄바꿔찍기") ) {
					iCount ++;
					CString sShelfCode = pDm0203->GetCellData( _T("BB_별치기호_코드"), i );
					CString sClassNo = pDm0203->GetCellData( _T("BB_분류기호"), i );
					CString sBookCode = pDm0203->GetCellData( _T("BB_도서기호"), i );
					CString sVolCode = pDm0203->GetCellData( _T("BB_권책기호"), i );
					CString sCopyCode = pDm0203->GetCellData( _T("BB_복본기호"), i );
					CString sRegNo = pDm0203->GetCellData( _T("BB_등록번호"), i );
					CString sResult;
					CString sNewLine;
					if ( sShelfCode != _T("") ) {
						sResult = sShelfCode;
						sNewLine = _T("\r\n");
					}
					else {
						sNewLine = _T("\r\n");
					}
					if ( sClassNo != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("\r\n") + sClassNo;
						}
						else {
							sResult = sResult + sClassNo;
							sNewLine = sNewLine + _T("\r\n");
						}
					}
					else {
						sNewLine = sNewLine + _T("\r\n");
					}
					if ( sBookCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("\r\n") + sBookCode;
						}
						else {
							sResult = sResult + sBookCode;
							sNewLine = sNewLine + _T("\r\n");
						}
					}
					else {
						sNewLine = sNewLine + _T("\r\n");
					}
					if ( sVolCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("\r\n") + sVolCode;
						}
						else {
							sResult = sResult + sVolCode;
							sNewLine = sNewLine + _T("\r\n");
						}						
					}
					else {
						sNewLine = sNewLine + _T("\r\n");
					}
					if ( sCopyCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("\r\n") + sCopyCode;
						}
						else {
							sResult = sResult + sCopyCode;
							sNewLine = sNewLine + _T("\r\n");
						}						
					}
					else {
						sNewLine = sNewLine + _T("\r\n");
					}
					sResult = sResult + sNewLine + _T("\r\n") + sRegNo;
					
					INT iNod = iCount % 6;
					INT iMok =  iCount / 6 ;

					if ( iNod == 0 ) {
						pDmLabel->InsertRow(-1);
					}
					CString sNum;
					sNum.Format( _T("%d") , iNod+1 );
					CString sCallAlias = _T("BB_청구기호") + sNum;
					pDmLabel->SetCellData( sCallAlias, sResult, iMok );
				}
			}
		}
		else {			
			for ( INT i = 0 ; i < i0203Count ; i++ ) {

				CString sRegNo = pDm0203->GetCellData( _T("BB_등록번호"), i );
				if ( sRegNoStart != _T("") && sRegNoEnd == _T("") ) {
					if ( sRegNo != sRegNoStart ) {
						continue;
					}
				}
				else if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
					if ( sRegNo < sRegNoStart || sRegNo > sRegNoEnd ) {
						continue;
					}
				}
				else if ( sRegNoStart == _T("") && sRegNoEnd != _T("") ) {
					AfxMessageBox( _T("시작등록번호를 입력하세요.") );
					return;
				}

				CString sLabelType = pDm0203->GetCellData( _T("BB_레이블출력구분"), i );
				if ( sLabelType == _T("이어찍기") ) {
					iCount ++;
					CString sShelfCode = pDm0203->GetCellData( _T("BB_별치기호_코드"), i );
					CString sClassNo = pDm0203->GetCellData( _T("BB_분류기호"), i );
					CString sBookCode = pDm0203->GetCellData( _T("BB_도서기호"), i );
					CString sVolCode = pDm0203->GetCellData( _T("BB_권책기호"), i );
					CString sCopyCode = pDm0203->GetCellData( _T("BB_복본기호"), i );
					CString sRegNo = pDm0203->GetCellData( _T("BB_등록번호"), i );
					CString sResult = sShelfCode;
						if ( sResult != _T("") ) {
							sResult = sResult + _T(" ") + sClassNo;
						}
						else {
							sResult = sClassNo;
						}
					if ( sBookCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("-") + sBookCode;
						}
						else {
							sResult = sBookCode;
						}
					}
					if ( sVolCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("-") + sVolCode;
						}						
					}
					if ( sCopyCode != _T("") ) {
						if ( sResult != _T("") ) {
							sResult = sResult + _T("=") + sCopyCode;
						}
						else {
							sResult = sCopyCode;
						}
					}

					INT iNod = (iCount+1) % 2;
					INT iMok = iCount / 2;

					if ( iNod == 1 ) {
						pDmLabel->InsertRow(-1);
						pDmLabel->SetCellData( _T("BB_청구기호3"), sRegNo, iMok );
					}
					if ( iNod == 0 ) {
						iNod = 2;
					}
					CString sNum;
					sNum.Format( _T("%d") , iNod );
					CString sCallAlias = _T("BB_청구기호") + sNum;
					pDmLabel->SetCellData( sCallAlias, sResult, iMok );
				}
			}
		}

		if ( iCount < 0 ) {
			AfxMessageBox( _T("출력할 데이터가 없습니다.") );
			return;
		}

		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
			return;
		}
		INT ids;
		pSIReportManager->SetCONNECTION_INFO(pDmLabel->CONNECTION_INFO);
		if ( pSERIAL_PRINT->GetCheck() == 1 ) {
			ids = pSIReportManager->GetSIReportFileFromDB(_T("정리_이어찍기_완성"));
		}
		else {
			ids = pSIReportManager->GetSIReportFileFromDB(_T("정리_줄바꿔찍기"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}

		pSIReportManager->SetDataMgr(0, pDmLabel);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}	
	}

	if ( pBARCODE->GetCheck() == 1 ) {
		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
			return;
		}
		INT ids;
		pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
		ids = pSIReportManager->GetSIReportFileFromDB(_T("정리-바코드출력"));		
		
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}
		
		CString sLibraryName;
		GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("도서관명"), sLibraryName );
		pSIReportManager->SetTempVariable(_T("%BARCODE바코드제목%"), sLibraryName);

		pSIReportManager->SetDataMgr(0, m_pDM);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}	
	}

	CButton *pBOOKCARD;
	pBOOKCARD = (CButton *)GetDlgItem(IDC_rBOOKCARD);
	CButton *pSTICKER;
	pSTICKER = (CButton *)GetDlgItem(IDC_rSTICKER);
	if ( pBOOKCARD->GetCheck() == 1 || pSTICKER->GetCheck() == 1 ) {
		CString sPrintCount;
		GetControlData( _T("CM_BO_CAT_PROC_0203_01"), _T("매폭"), sPrintCount );
		INT iPrintCount = _ttoi ( sPrintCount );
		if ( iPrintCount > 4 || sPrintCount == _T("") ) {
			AfxMessageBox( _T("매폭은 4 이하입니다.") );
			return;
		}
		CESL_DataMgr *pDm0203;
		pDm0203 = FindDM(_T("DM_BO_CAT_PROC_0203"));
		if ( NULL == pDm0203 ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0203 error") );
			return;
		}

		CESL_DataMgr *pDmLabel;
		pDmLabel = FindDM(_T("DM_BO_CAT_PROC_0203_LABEL"));
		if ( NULL == pDmLabel ) {
			AfxMessageBox ( _T("DM_BO_CAT_PROC_0203_LABEL error") );
			return;
		}
		pDmLabel->FreeData();
		INT i0203Count = pDm0203->GetRowCount();
		INT iLabelCount = pDmLabel->GetRowCount();
		INT iCount = -1;
		for ( INT i = 0 ; i < i0203Count ; i++ ) {
			
			CString sRegNo = pDm0203->GetCellData( _T("BB_등록번호"), i );
			if ( sRegNoStart != _T("") && sRegNoEnd == _T("") ) {
				if ( sRegNo != sRegNoStart ) {
					continue;
				}
			}
			else if ( sRegNoStart != _T("") && sRegNoEnd != _T("") ) {
				if ( sRegNo < sRegNoStart || sRegNo > sRegNoEnd ) {
					continue;
				}
			}
			else if ( sRegNoStart == _T("") && sRegNoEnd != _T("") ) {
				AfxMessageBox( _T("시작등록번호를 입력하세요.") );
				return;
			}

			iCount ++;
			CString sShelfCode = pDm0203->GetCellData( _T("BB_별치기호_코드"), i );
			CString sClassNo = pDm0203->GetCellData( _T("BB_분류기호"), i );
			CString sBookCode = pDm0203->GetCellData( _T("BB_도서기호"), i );
			CString sVolCode = pDm0203->GetCellData( _T("BB_권책기호"), i );
			CString sCopyCode = pDm0203->GetCellData( _T("BB_복본기호"), i );
			sRegNo = pDm0203->GetCellData( _T("BB_등록번호"), i );
			CString sResult;

			CString sNewLine;
			if ( sShelfCode != _T("") ) {
				sResult = sShelfCode;
				sNewLine = _T("\r\n");
			}
			else {
				sNewLine = _T("\r\n");
			}
			if ( sClassNo != _T("") ) {
				if ( sResult != _T("") ) {
					sResult = sResult + _T("\r\n") + sClassNo;
				}
				else {
					sResult = sResult + sClassNo;// + _T("\r\n");
					sNewLine = sNewLine + _T("\r\n");
				}
			}
			else {
				sNewLine = sNewLine + _T("\r\n");
			}
			if ( sBookCode != _T("") ) {
				if ( sResult != _T("") ) {
					sResult = sResult + _T("\r\n") + sBookCode;
				}
				else {
					sResult = sResult + sBookCode;
					sNewLine = sNewLine + _T("\r\n");
				}
			}
			else {
				sNewLine = sNewLine + _T("\r\n");
			}
			if ( sVolCode != _T("") ) {
				if ( sResult != _T("") ) {
					sResult = sResult + _T("\r\n") + sVolCode;
				}
				else {
					sResult = sResult + sVolCode;
					sNewLine = sNewLine + _T("\r\n");
				}						
			}
			else {
				sNewLine = sNewLine + _T("\r\n");
			}
			if ( sCopyCode != _T("") ) {
				if ( sResult != _T("") ) {
					sResult = sResult + _T("\r\n") + sCopyCode;
				}
				else {
					sResult = sResult + sCopyCode;
					sNewLine = sNewLine + _T("\r\n");
				}						
			}
			else {
				sNewLine = sNewLine + _T("\r\n");
			}
			sResult = sResult + sNewLine;

			CString sTitle = pDm0203->GetCellData( _T("IBS_본표제"), i );
			sTitle = _T("서  명 : ") + sTitle;
			INT iTitleLen = sTitle.GetLength();
			if ( iTitleLen > 37 ) {
				sTitle = sTitle.Left(36);
			}
			CString sAuthor = pDm0203->GetCellData( _T("IBS_저작자"), i );
			sAuthor = _T("저  자 : ") + sAuthor;
			sResult = sResult + _T("\r\n\r\n\r\n") + sTitle + _T("\r\n\r\n") + sAuthor;
			sResult = sResult + _T("\r\n") + _T("               ") + sRegNo;
			if ( sRegNo == _T("") ) {
				sResult = sResult + _T("\r\n");
			}

			INT iNod = iCount % iPrintCount;
			INT iMok =  iCount / iPrintCount ;

			if ( iNod == 0 ) {
				pDmLabel->InsertRow(-1);
			}
			CString sNum;
			sNum.Format( _T("%d") , iNod+1 );
			CString sCallAlias = _T("BB_청구기호") + sNum;
			pDmLabel->SetCellData( sCallAlias, sResult, iMok );
		}

		if ( iCount < 0 ) {
			AfxMessageBox( _T("출력할 데이터가 없습니다.") );
			return;
		}

		CSIReportManager *pSIReportManager = new CSIReportManager(this);
		pSIReportManager->m_fpEventHandler = ( INT( CWnd::* )( CSIReportEventInfo *EventInfo  ) )SIReportEventHandler;
		if (!pSIReportManager) {
			AfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
			return;
		}
		INT ids;
		pSIReportManager->SetCONNECTION_INFO(pDmLabel->CONNECTION_INFO);
		if ( pBOOKCARD->GetCheck() == 1 ) {
			ids = pSIReportManager->GetSIReportFileFromDB(_T("정리_북카드"));
		}
		else {
			ids = pSIReportManager->GetSIReportFileFromDB(_T("정리_스티커"));
		}
		
		if (ids < 0) {
			AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
			return;
		}

		pSIReportManager->SetDataMgr(0, pDmLabel);
		ids = pSIReportManager->Print();
		if (pSIReportManager) {
			delete pSIReportManager;
			pSIReportManager = NULL;
		}
	}

EFS_END
}

INT CBO_CAT_PROC_0203_01::SIReportEventHandler( CSIReportEventInfo * EventInfo )
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = (CSIReportManager *)EventInfo->m_pSIReportManager;
	if (!pSIReportManager) return -1;
	return 0;

EFS_END
return -1;

}
HBRUSH CBO_CAT_PROC_0203_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
