// DCM_MANAGER.cpp : implementation file
//

#include "stdafx.h"
#include "DCM_MANAGER.h"
#include "DCM_ENVIRONMENT_MGR.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCM_MANAGER dialog


CDCM_MANAGER::CDCM_MANAGER(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CDCM_MANAGER::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDCM_MANAGER)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pConnection = NULL;
//     m_pFileFind = NULL;
	m_strDbFlag = _T("M");
}

CDCM_MANAGER::~CDCM_MANAGER()
{
	if (m_pConnection != NULL) 
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

//	if (m_pFileFind != NULL) 
//	{
//        delete m_pFileFind;
//        m_pFileFind = NULL;
//    }

}

void CDCM_MANAGER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDCM_MANAGER)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDCM_MANAGER, CDialog)
	//{{AFX_MSG_MAP(CDCM_MANAGER)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnEnv, OnbtnEnv)
	ON_BN_CLICKED(IDC_btnDcmAdd, OnbtnDcmAdd)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnDcmDel, OnbtnDcmDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCM_MANAGER message handlers

HBRUSH CDCM_MANAGER::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDCM_MANAGER::OnInitDialog() 
{
	CDialog::OnInitDialog();

	EnableThemeDialogTexture(GetSafeHwnd()); 

	if( _T("M") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_BO_DCM")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}

		m_pCM = FindCM(_T("CM_BO_DCM"));
		m_pDM = FindDM(_T("DM_BO_DCM"));
		m_pFtpDM = FindDM(_T("DM_BO_DCM_FTP_INFO"));
	}
	else if( _T("Y") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_SE_DCM")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}

		m_pCM = FindCM(_T("CM_SE_DCM"));
		m_pDM = FindDM(_T("DM_SE_DCM"));
		m_pFtpDM = FindDM(_T("DM_SE_DCM_FTP_INFO"));
	}	
	else if( _T("A") == m_strDbFlag )
	{
		if (InitESL_Mgr(_T("SM_SE_ATICLE_DCM")) < 0)
		{
			ESLAfxMessageBox(_T("InitESL_Mgr Error"));
			return FALSE;
		}

		m_pCM = FindCM(_T("CM_SE_ATICLE_DCM"));
		m_pDM = FindDM(_T("DM_SE_ATICLE_DCM"));
		m_pFtpDM = FindDM(_T("DM_SE_ATICLE_DCM_FTP_INFO"));
	}
	else
	{
		AfxMessageBox(_T("DB FLAG를 설정해주세요."));
		return FALSE;
	}

	m_pGrid = (CESL_Grid*)m_pCM->FindControl(_T("DcmMgrGrid"));

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileTypeSet);
	pCmb->SelectString(0,_T("PDF"));
// 	pCmb->EnableWindow(FALSE);

	
	if(!Init())
	{
		ESLAfxMessageBox(_T("관리화면을 초기화 할 수 없습니다."));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDCM_MANAGER::Init()
{
	CString strData;
	CString strSpeciesKey;
	CString strOption;
	INT nParentSelectCnt;
	INT nIdx;
	INT nIds;
	

	if( _T("M") == m_strDbFlag )
	{
		m_pParentGrid->SelectMakeList();
		nParentSelectCnt = m_pParentGrid->SelectGetCount();
		nIdx = m_pParentGrid->SelectGetHeadPosition();	
		for( INT i = 0 ; i < nParentSelectCnt ; i++ )
		{
			strData = m_pParentDM->GetCellData( _T("BS_종KEY"), nIdx );
			if(strSpeciesKey.IsEmpty())
			{
				strSpeciesKey = strData;
			}
			else
			{
				strSpeciesKey += _T(", ") + strData;
			}

			nIdx = m_pParentGrid->SelectGetNext();
		}
		//단행에서 이용할 조건
		//제어번호와 발행년이 존재하는 자료만 원부추가 할수있다.
		strOption.Format( _T("B.REC_KEY IN (SELECT MAX(REC_KEY) FROM BO_BOOK_TBL WHERE SPECIES_KEY IN( %s ) AND BOOK_APPENDIX_FLAG = 'B' GROUP BY SPECIES_KEY,VOL_SORT_NO) AND I.CONTROL_NO IS NOT NULL AND I.PUB_YEAR IS NOT NULL ")
			             , strSpeciesKey );
	}
	else if( _T("Y") == m_strDbFlag )
	{
		m_pParentGrid->SelectMakeList();
		nParentSelectCnt = m_pParentGrid->SelectGetCount();
		nIdx = m_pParentGrid->SelectGetHeadPosition();	
		for( INT i = 0 ; i < nParentSelectCnt ; i++ )
		{
			strData = m_pParentDM->GetCellData( _T("SS_종KEY"), nIdx );
			if(strSpeciesKey.IsEmpty())
			{
				strSpeciesKey = strData;
			}
			else
			{
				strSpeciesKey += _T(", ") + strData;
			}

			nIdx = m_pParentGrid->SelectGetNext();
		}

		//연속에서 이용할 조건
		//제어번호가 존재하는 자료만 원부추가 할수있다.
		strOption.Format( _T("B.REC_KEY IN (SELECT MAX(REC_KEY) FROM SE_BOOK_TBL WHERE SPECIES_KEY IN( %s ) GROUP BY SPECIES_KEY,VOL_KEY) AND I.CONTROL_NO IS NOT NULL")
						 , strSpeciesKey );
	}
	else if( _T("A") == m_strDbFlag )
	{
		strData = m_pParentDM->GetCellData( _T("권호키"), 0 );
		//기사에서 이용할 조건
// 		strOption.Format( _T("B.REC_KEY IN (SELECT MAX(REC_KEY) FROM SE_BOOK_TBL WHERE VOL_KEY IN( %s ) GROUP BY SPECIES_KEY, VOL_KEY)")
		strOption.Format( _T("V.REC_KEY IN ( %s )")
						 , strData );
	}
	
	nIds = m_pDM->RefreshDataManager(strOption);
	if(nIds < 0) 
	{
		return FALSE;
	}

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileTypeSet);
	pCmb->GetLBText( pCmb->GetCurSel() , strData  );

	strOption.Format(_T("ACTIVE_FLAG = 'Y' AND TYPE = '%s'"), strData );
	nIds = m_pFtpDM->RefreshDataManager(strOption);
	if(nIds < 0)
	{
		return FALSE;
	}
	
	m_pGrid->Display();
	m_pGrid->SetReverse(0);

	return TRUE;
}


void CDCM_MANAGER::OnbtnEnv() 
{
	CString strData;
	CString strFileType;
	CString strOption;
	BOOL bActiveChage;
	INT nIds;

	CDCM_ENVIRONMENT_MGR dlg(this);
	dlg.SetDbFlag(m_strDbFlag);

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileTypeSet);
	pCmb->GetLBText( pCmb->GetCurSel() , strFileType  );

 	dlg.SetParentFileType(strFileType);

	dlg.DoModal();

	bActiveChage = dlg.GetActiveChange();

	if( bActiveChage )
	{
		strOption.Format(_T("ACTIVE_FLAG = 'Y' AND TYPE = '%s'"), strFileType );
		nIds = m_pFtpDM->RefreshDataManager(strOption);
		if(nIds < 0)
		{
			return ;
		}
	}
}

void CDCM_MANAGER::OnbtnDcmAdd() 
{
	CString strData;
	CString strOption;
	CString strMsg;
	INT nSelectCnt;
	INT nIdx;
	INT nIds;
	INT nCurPos;
	INT nDmRowCnt;

	BOOL bMsgViewChk;
	bMsgViewChk = FALSE;
	
	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileTypeSet);
	pCmb->GetLBText( pCmb->GetCurSel() , strData  );

	strOption.Format(_T("ACTIVE_FLAG = 'Y' AND TYPE = '%s'"), strData );
	nIds = m_pFtpDM->RefreshDataManager(strOption);

	if( nIds < 0 )
	{
		strMsg.Format( _T("서버의 정보를 가져오는데 실패했습니다.") );
		AfxMessageBox( strMsg );
		return ;
	}
	
	nDmRowCnt = m_pFtpDM->GetRowCount();
	if( 1 != nDmRowCnt )
	{
		strMsg.Format( _T("FTP서버를 설정해 주세요.") );
		AfxMessageBox( strMsg );
		return ;
	}

	nCurPos = m_pGrid->GetIdx();

	m_pGrid->SelectMakeList();
	nSelectCnt = m_pGrid->SelectGetCount();
	if( 0 < nSelectCnt )
	{
		nIds = m_pDM->StartFrame();
		if ( nIds < 0 )
  		{
			strMsg.Format(_T("StartFrame 작업이 실패했습니다."));
			AfxMessageBox( strMsg );
			return;
  		}

		nIdx = m_pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			CString strData;

			strData = m_pDM->GetCellData( _T( "파일명" ), nIdx );
			if( !strData.IsEmpty() )
			{
				if( bMsgViewChk == FALSE )
				{
					strMsg.Format(_T("원문이 존재하지 않는 자료만 업로드 가능합니다."));
					AfxMessageBox( strMsg );
					bMsgViewChk = TRUE;
				}
				nIdx = m_pGrid->SelectGetNext();
				continue;
			}

			strData = m_pDM->GetCellData( _T( "UDF_TAGET원문" ), nIdx );
			if( strData.IsEmpty() )
			{
				nIdx = m_pGrid->SelectGetNext();
				continue;
			}

			nIds = OnConnect( nIdx );
			if( nIds == -1000 )
			{	
				strMsg.Format(_T("FTP연결에 실패했습니다."));
				AfxMessageBox( strMsg );
				return;
			}
			else if ( nIds == -1001 )
			{
				strMsg.Format(_T("FTP전송에 실패했습니다."));
				AfxMessageBox( strMsg );
				continue;
			}

			nIds = MakeInsertQuery(nIdx);						

			nIdx = m_pGrid->SelectGetNext();
		}

		nIds = m_pDM->SendFrame();
		if ( nIds < 0 )
  		{
  			strMsg.Format(_T("SendFrame 작업이 실패했습니다."));
			AfxMessageBox( strMsg );
			//strErrMsg.Format
			return;
  		}

		nIds = m_pDM->EndFrame();
		if ( nIds < 0 )
  		{
  			strMsg.Format(_T("EndFrame 작업이 실패했습니다."));
			AfxMessageBox( strMsg );
			//strErrMsg.Format
			return;
  		}		

		AfxMessageBox(_T("원문 업로드를 완료했습니다."));
		m_pGrid->SetProtectSelectColumn(TRUE);
		m_pGrid->Display();
		m_pGrid->SetReverse(nCurPos);
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
	else
	{
		AfxMessageBox(_T("선정된 데이터가 없습니다."));
	}
}

VOID CDCM_MANAGER::OnDcmAdd() 
{
	EFS_BEGIN

	UpdateData();

	CString strFileType;
	CString strData;
	TCHAR szBuffer[MAX_PATH];

	_tgetcwd(szBuffer, MAX_PATH);

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileTypeSet);
	pCmb->GetLBText( pCmb->GetCurSel() , strData  );
	strFileType.Format(_T("파일형식(*.%s)|*.%s|모든파일(*.*)|*.*||"), strData, strData);
	CFileDialog OFN(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFileType );
	if (OFN.DoModal() != IDOK) 
	{
		return ;
	}
	
	_tchdir(szBuffer);
	m_strFilePath = OFN.GetPathName();

	EFS_END
}

void CDCM_MANAGER::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	//===================================================
	//2009.04.21 UPDATE BY PJW : 현재는 PDF만 사용함으로 콤보박스는 보여줄 필요없다.
//	CButton *pBtnFileTypeSet = (CButton*)GetDlgItem(IDC_cmbFileTypeSet);
//	if(pBtnFileTypeSet->GetSafeHwnd() == NULL) return;
//	pBtnFileTypeSet->MoveWindow( 7, cy - 29, 87, 21);
//
//	CButton *pBtnEnv = (CButton*)GetDlgItem(IDC_btnEnv);
//	if(pBtnEnv->GetSafeHwnd() == NULL) return;
//	pBtnEnv->MoveWindow(102, cy - 29, 87, 21);
//
//	CButton *pBtnDcmAdd = (CButton*)GetDlgItem(IDC_btnDcmAdd);
//	if(pBtnDcmAdd->GetSafeHwnd() == NULL) return;
//	pBtnDcmAdd->MoveWindow(197, cy - 29, 87, 21);
//
//	CButton *pBtnDcmDel = (CButton*)GetDlgItem(IDC_btnDcmDel);
//	if(pBtnDcmDel->GetSafeHwnd() == NULL) return;
//	pBtnDcmDel->MoveWindow(292, cy - 29, 87, 21);

	CButton *pBtnFileTypeSet = (CButton*)GetDlgItem(IDC_cmbFileTypeSet);
	if(pBtnFileTypeSet->GetSafeHwnd() == NULL) return;
	pBtnFileTypeSet->MoveWindow( 7, cy - 29, 87, 21);

	CButton *pBtnEnv = (CButton*)GetDlgItem(IDC_btnEnv);
	if(pBtnEnv->GetSafeHwnd() == NULL) return;
	pBtnEnv->MoveWindow(7, cy - 29, 87, 21);

	CButton *pBtnDcmAdd = (CButton*)GetDlgItem(IDC_btnDcmAdd);
	if(pBtnDcmAdd->GetSafeHwnd() == NULL) return;
	pBtnDcmAdd->MoveWindow(102, cy - 29, 87, 21);

	CButton *pBtnDcmDel = (CButton*)GetDlgItem(IDC_btnDcmDel);
	if(pBtnDcmDel->GetSafeHwnd() == NULL) return;
	pBtnDcmDel->MoveWindow(197, cy - 29, 87, 21);
	//===================================================

	CButton *pBtnCANCEL = (CButton*)GetDlgItem(IDCANCEL);
	if(pBtnCANCEL->GetSafeHwnd() == NULL) return;
	pBtnCANCEL->MoveWindow(cx - 92, cy - 29, 87, 21);

	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_gridDCM_MANAGER);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(0, 0, cx, cy - 40);
		
}

BEGIN_EVENTSINK_MAP(CDCM_MANAGER, CDialog)
    //{{AFX_EVENTSINK_MAP(CDCM_MANAGER)
	ON_EVENT(CDCM_MANAGER, IDC_gridDCM_MANAGER, -601 /* DblClick */, OnDblClickgridDCMMANAGER, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDCM_MANAGER::OnDblClickgridDCMMANAGER() 
{
	if(m_pGrid->GetMouseRow() == 0) return;

	if(m_pDM->GetRowCount() == 0) return;

	INT nRow = m_pGrid->GetRow() - 1;
	INT nCol = m_pGrid->GetCol();

	INT nColIdx;
	INT nGridColCnt;
	CString strTmpData;

	nGridColCnt = m_pGrid->GetColCount();
	for( INT i = 0 ; i < nGridColCnt+1 ; i++ )
	{
		strTmpData = m_pGrid->GetTextMatrix( 0, i );
		if( strTmpData == _T("원문위치") ) 
		{
			nColIdx = i;
			break;
		}
	}

	if( nCol == nColIdx ) 
	{
 		OnDcmAdd();
		m_pDM->SetCellData(_T("UDF_TAGET원문"), m_strFilePath, nRow);	

		m_pGrid->SetProtectSelectColumn(TRUE);
		m_pGrid->Display();
		m_pGrid->SetReverse(nRow);
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
}



//===================================================
//2009.04.09 UPDATE BY PJW : 아래는 FTP 파일 제거함수 함수
//===================================================
INT CDCM_MANAGER::OnRemoveConnect(INT nIdx) 
{
	CString strDir;
	CString strPubYear;
	CString strPassive;
	CString strRemoteDir;
	CString strFtpSeverFileName;
	CString strFtpSeverFilePath;
	CString strOption;

	BOOL bPassive;
	
	if (m_pConnection != NULL) 
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	CString strIP;
	CString strPort;
	CString strID;
	CString strPass;
	CString strRootDir;
	CString strDbFlag;
	CString strData;

	INT nPos;
	INT nIds;

	strData = m_pDM->GetCellData(_T("서버ID"), nIdx);
	strOption.Format(_T("SERVER_ID = %s"), strData);
	nIds = m_pFtpDM->RefreshDataManager( strOption );

	if( nIds < 0 )
	{
		AfxMessageBox( _T("서버의 정보를 가져오는데 실패했습니다.") );
		return -1;
	}

	strFtpSeverFilePath = m_pDM->GetCellData(_T("파일PATH"), nIdx);
	strFtpSeverFileName = m_pDM->GetCellData(_T("파일명"), nIdx);
	strFtpSeverFilePath.Replace(_T("\\"), _T("/"));

	if( _T("M") == m_strDbFlag )		// 단행본
	{
		strDbFlag.Format(_T("DAN"));	
	}
	else if( _T("Y") == m_strDbFlag )
	{
		strDbFlag.Format(_T("YON"));	// 연속
	}
	else if( _T("A") == m_strDbFlag )
	{
		strDbFlag.Format(_T("NOTES"));	// 기사
	}

	strIP = m_pFtpDM->GetCellData(_T("내부서버IP"), 0);
	strPort = m_pFtpDM->GetCellData(_T("서버PORT"), 0);
	strID = m_pFtpDM->GetCellData(_T("USERID"), 0);
	strPass = m_pFtpDM->GetCellData(_T("PASSWD"), 0);
	strRootDir = m_pFtpDM->GetCellData(_T("ROOT_DIR"), 0);
	strPassive = m_pFtpDM->GetCellData(_T("PASSIVE모드"), 0);
	if( _T("Y") == strPassive )
	{
		bPassive = TRUE;
	}
	else
	{
		bPassive = FALSE;
	}

	m_pConnection = m_Session.GetFtpConnection( strIP, strID, strPass, _ttoi(strPort), bPassive);
		
    if (!m_pConnection) 
	{
        m_pConnection = NULL;
        AfxMessageBox(_T("m_Session.GetFtpConnection() failed"));
    }

	m_pConnection->GetCurrentDirectory(strRemoteDir);
// 	strRemoteDir.Replace(_T("\\"), _T("/"));
	
	strDir = strRootDir;
	if(!m_pConnection->SetCurrentDirectory(strDir))
	{
		AfxMessageBox(_T("삭제할 원문의 폴더가 존재하지 않습니다."));
		m_pConnection->Close();
		return -1;
	}
		
	while(1)
	{
		
		nPos = strFtpSeverFilePath.Find(_T("/"));
		if( 0 > nPos)
		{
			strDir = strFtpSeverFilePath;
			break;
		}
		else if( 0 == nPos )
		{
			strFtpSeverFilePath = strFtpSeverFilePath.Mid( nPos + 1 );
			continue;
		}
		
		nPos = strFtpSeverFilePath.Find(_T("/"));
		strDir = strFtpSeverFilePath.Left(nPos);
		strDir.Replace(_T("/"),_T(""));
		strFtpSeverFilePath = strFtpSeverFilePath.Mid( nPos + 1 );

		if(!m_pConnection->SetCurrentDirectory(strDir))
		{
			AfxMessageBox(_T("삭제할 원문의 폴더가 존재하지 않습니다."));
			m_pConnection->Close();
			return -1;
		}
	}

	strDir.Replace(_T("/"),_T(""));
	if( !strDir.IsEmpty() )
	{
		if(!m_pConnection->SetCurrentDirectory(strDir))
		{
			AfxMessageBox(_T("삭제할 원문의 폴더가 존재하지 않습니다."));
			m_pConnection->Close();
			return -1;
		}
	}

	
	if( !m_pConnection->Remove( strFtpSeverFileName ) )
	{
		AfxMessageBox(_T("원문 삭제하는데  실패했습니다.."));
		m_pConnection->Close();
		return -1;
	}

	m_pConnection->GetCurrentDirectory( strFtpSeverFilePath );
	strFtpSeverFilePath.Replace(_T("\\"), _T("/"));

// 	strDir.Replace( _T("\\"), _T("/") );

	CString strCurrentSeverPath;
	while(1)
	{
		if( strFtpSeverFilePath.IsEmpty() )
		{
			break;
		}

		nPos = strFtpSeverFilePath.ReverseFind('/');
		strDir = strFtpSeverFilePath.Mid( nPos + 1 );
		strDir.Replace( _T("/"), _T("") );
		strFtpSeverFilePath = strFtpSeverFilePath.Left( nPos );

		strCurrentSeverPath = strFtpSeverFilePath + _T("/");
		m_pConnection->SetCurrentDirectory( strCurrentSeverPath );
		if( !m_pConnection->RemoveDirectory( strDir ) )
		{
			break;
		}
	}

	m_pConnection->Close();
 
	return 0 ;
}

//===================================================
//2009.04.09 UPDATE BY PJW : 아래는 FTP UPDATE함수
//===================================================
INT CDCM_MANAGER::OnConnect(INT nIdx) 
{
	CString strDir;
	CString strTemp;
	CString strControlNo;
	CString strVolSortNo;
	CString strFileType;
	CString strArticleKey;
	CString strLocalFilePath;
	CString strPubYear;
	CString strPassive;
	CString strRemoteDir;
	CString strFtpSeverFileName;
	CString strFtpSeverFilePath;

	// KOL.UDF.022 시큐어코딩 보완
	INT nIds = -1;

	BOOL bPassive;
	
	if (m_pConnection != NULL) 
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}

	CString strIP;
	CString strPort;
	CString strID;
	CString strPass;
	CString strRootDir;
	CString strDbFlag;

	if( _T("M") == m_strDbFlag )		// 단행본
	{
		strDbFlag.Format(_T("DAN"));	
	}
	else if( _T("Y") == m_strDbFlag )
	{
		strDbFlag.Format(_T("YON"));	// 연속
	}
	else if( _T("A") == m_strDbFlag )
	{
		strDbFlag.Format(_T("NOTES"));	// 기사
	}

	strIP = m_pFtpDM->GetCellData(_T("내부서버IP"), 0);
	strPort = m_pFtpDM->GetCellData(_T("서버PORT"), 0);
	strID = m_pFtpDM->GetCellData(_T("USERID"), 0);
	strPass = m_pFtpDM->GetCellData(_T("PASSWD"), 0);
	strRootDir = m_pFtpDM->GetCellData(_T("ROOT_DIR"), 0);
	strPassive = m_pFtpDM->GetCellData(_T("PASSIVE모드"), 0);
	if( _T("Y") == strPassive )
	{
		bPassive = TRUE;
	}
	else
	{
		bPassive = FALSE;
	}

	m_pConnection = m_Session.GetFtpConnection( strIP, strID, strPass, _ttoi(strPort), bPassive);
		
    if (!m_pConnection) 
	{
        m_pConnection = NULL;
		if( 0 == nIds )
		{
			return -1000 ;
		}
//         AfxMessageBox(_T("m_Session.GetFtpConnection() failed"));
    }

	m_pConnection->GetCurrentDirectory(strRemoteDir);
// 	strRemoteDir.Replace(_T("\\"), _T("/"));
	
	strDir = strRootDir;
	if(!m_pConnection->SetCurrentDirectory(strDir))
	{
		m_pConnection->CreateDirectory(strDir);
		m_pConnection->SetCurrentDirectory(strDir);
	}

	CComboBox* pCmb = (CComboBox*)GetDlgItem(IDC_cmbFileTypeSet);
	pCmb->GetLBText( pCmb->GetCurSel() , strFileType  );
	strDir = strFileType;

	if(!m_pConnection->SetCurrentDirectory(strDir))
	{
		m_pConnection->CreateDirectory(strDir);
		m_pConnection->SetCurrentDirectory(strDir);
	}
// 	strFtpSeverFilePath += _T("\\") + strDir;
	strFtpSeverFilePath += _T("/") + strDir;

	strDir = strDbFlag;
	if(!m_pConnection->SetCurrentDirectory(strDir))
	{
		m_pConnection->CreateDirectory(strDir);
		m_pConnection->SetCurrentDirectory(strDir);
	}
	strFtpSeverFilePath += _T("/") + strDir;

	if( _T("M") == m_strDbFlag )		// 단행본
	{
		strPubYear = m_pDM->GetCellData(_T("발행년"), nIdx);
		strDir = strPubYear;
		if(!m_pConnection->SetCurrentDirectory(strDir))
		{
			m_pConnection->CreateDirectory(strDir);
			m_pConnection->SetCurrentDirectory(strDir);
		}
		strFtpSeverFilePath += _T("/") + strDir;

		strControlNo = m_pDM->GetCellData(_T("제어번호"), nIdx);
		strVolSortNo = m_pDM->GetCellData(_T("권일련번호"), nIdx);
		strDir.Format(_T("%s_%s"), strControlNo, strVolSortNo);

		if(!m_pConnection->SetCurrentDirectory(strDir))
		{
			m_pConnection->CreateDirectory(strDir);
			m_pConnection->SetCurrentDirectory(strDir);
		}
		strFtpSeverFilePath += _T("/") + strDir;
		
		m_pConnection->GetCurrentDirectory(strDir);
// 		strDir.Replace(_T("\\"), _T("/"));

		strLocalFilePath = m_pDM->GetCellData(_T("UDF_TAGET원문"), nIdx);
		strFtpSeverFileName.Format( _T("%s_%s.%s"), strControlNo, strVolSortNo, strFileType );
	}
	else if( _T("Y") == m_strDbFlag )
	{
		strControlNo = m_pDM->GetCellData(_T("제어번호"), nIdx);
		strDir = strControlNo;
		if(!m_pConnection->SetCurrentDirectory(strDir))
		{
			m_pConnection->CreateDirectory(strDir);
			m_pConnection->SetCurrentDirectory(strDir);
		}
		strFtpSeverFilePath += _T("/") + strDir;

		strVolSortNo = m_pDM->GetCellData(_T("권일련번호"), nIdx);
		strDir.Format(_T("%s_%s"), strControlNo, strVolSortNo);

		if(!m_pConnection->SetCurrentDirectory(strDir))
		{
			m_pConnection->CreateDirectory(strDir);
			m_pConnection->SetCurrentDirectory(strDir);
		}
		strFtpSeverFilePath += _T("/") + strDir;
		
		m_pConnection->GetCurrentDirectory(strDir);
// 		strDir.Replace(_T("\\"), _T("/"));
		
		strLocalFilePath = m_pDM->GetCellData(_T("UDF_TAGET원문"), nIdx);
		strFtpSeverFileName.Format( _T("%s_%s.%s"), strControlNo, strVolSortNo, strFileType );
	}	
	else if( _T("A") == m_strDbFlag )
	{
		strControlNo = m_pDM->GetCellData(_T("제어번호"), nIdx);
		strDir = strControlNo;
		if(!m_pConnection->SetCurrentDirectory(strDir))
		{
			m_pConnection->CreateDirectory(strDir);
			m_pConnection->SetCurrentDirectory(strDir);
		}
		strFtpSeverFilePath += _T("/") + strDir;

		strControlNo = m_pDM->GetCellData(_T("제어번호"), nIdx);
		strVolSortNo = m_pDM->GetCellData(_T("권일련번호"), nIdx);
		strArticleKey = m_pDM->GetCellData(_T("기사KEY"), nIdx);
		strDir.Format(_T("%s_%s_%s"), strControlNo, strVolSortNo, strArticleKey);

		if(!m_pConnection->SetCurrentDirectory(strDir))
		{
			m_pConnection->CreateDirectory(strDir);
			m_pConnection->SetCurrentDirectory(strDir);
		}
		strFtpSeverFilePath += _T("/") + strDir;
		
		m_pConnection->GetCurrentDirectory(strDir);
// 		strDir.Replace(_T("\\"), _T("/"));
		
		strLocalFilePath = m_pDM->GetCellData(_T("UDF_TAGET원문"), nIdx);

		strFtpSeverFileName.Format( _T("%s_%s_%s.%s"), strControlNo, strVolSortNo, strArticleKey, strFileType );
	}	
	
	nIds = m_pConnection->PutFile(strLocalFilePath,strFtpSeverFileName);	
	m_pConnection->Close();
	if( 0 == nIds )
	{
		return -1001 ;
	}

	m_pDM->SetCellData(_T("파일명"), strFtpSeverFileName, nIdx);
	m_pDM->SetCellData(_T("파일PATH"), strFtpSeverFilePath, nIdx);

	return 0 ;
}


INT CDCM_MANAGER::MakeInsertQuery(INT nIdx)
{
	CString strData;
	CString strServerId;
	CString strActiveFlag;
	CString strContentsType;
	CString strQuery;

	m_pDM->InitDBFieldData() ;

	m_pDM->MakeRecKey(strData);
// 	m_pDM->AddDBFieldData( _T("REC_KEY")   ,_T("NUMERIC"), _T("ESL_SEQ.NEXTVAL"));
	m_pDM->AddDBFieldData( _T("REC_KEY")   ,_T("NUMERIC"), strData);
	m_pDM->SetCellData(_T("원문KEY"), strData, nIdx);

	strData = m_pDM->GetCellData(_T("종KEY"), nIdx);
	m_pDM->AddDBFieldData( _T("SPECIES_KEY")   ,_T("NUMERIC"), strData);

	//===================================================
	//DM이 연속 기사용일 경우만 사용한다.
	if( _T("A") == m_strDbFlag )
	{
		strData = m_pDM->GetCellData(_T("기사KEY"), nIdx);
		m_pDM->AddDBFieldData( _T("ARTICLE_KEY")     ,_T("NUMERIC") , strData );
	}
	//===================================================	

	strData = m_pDM->GetCellData(_T("권일련번호"), nIdx);
	m_pDM->AddDBFieldData( _T("VOL_SORT_NO")     ,_T("NUMERIC"), strData );

	m_pDM->AddDBFieldData( _T("DB_FLAG")    ,_T("STRING"), m_strDbFlag );

	strData = m_pDM->GetCellData(_T("파일PATH"), nIdx);
	m_pDM->AddDBFieldData( _T("PATH")	   ,_T("STRING"), strData );

	strData = m_pDM->GetCellData(_T("파일명"), nIdx);
	m_pDM->AddDBFieldData( _T("FILENAME")	   ,_T("STRING"), strData );

	strData = m_pFtpDM->GetCellData(_T("CONTENTS_TYPE"), 0);
	m_pDM->AddDBFieldData( _T("TYPE") ,_T("STRING"), strData );

	strData = m_pFtpDM->GetCellData(_T("서버ID"), 0);
	m_pDM->AddDBFieldData( _T("SERVER_ID")      ,_T("NUMERIC"), strData );
	m_pDM->SetCellData(_T("서버ID"), strData, nIdx);

	strData = m_pFtpDM->GetCellData(_T("REG_DATE"), 0);

	CTime t = CTime::GetCurrentTime();
	strData.Format( _T("%04d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay() );
	m_pDM->AddDBFieldData( _T("REG_DATE") ,_T("STRING"), strData );

	m_pDM->MakeInsertFrame( _T("DL_CONTENTS_MAP_TBL") );
	

	return 0 ;
}

void CDCM_MANAGER::OnbtnDcmDel() 
{
	INT nSelectCnt;
	INT nIdx;
	INT nIds;
	INT nCurPos;

	nCurPos = m_pGrid->GetIdx();

	m_pGrid->SelectMakeList();
	nSelectCnt = m_pGrid->SelectGetCount();
	if( 0 < nSelectCnt )
	{
		nIds = m_pDM->StartFrame();
		if ( nIds < 0 )
  		{
  			AfxMessageBox(_T("StartFrame 작업이 실패했습니다."));
			return;
  		}

		nIdx = m_pGrid->SelectGetHeadPosition();
		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			CString strData;
			strData = m_pDM->GetCellData( _T( "파일명" ), nIdx );
			if( strData.IsEmpty() )
			{
				nIdx = m_pGrid->SelectGetNext();
				continue;
			}

			nIds = OnRemoveConnect( nIdx );
			if( 0 <= nIds )
			{
				nIds = MakeDeleteQuery( nIdx );
			}

			nIdx = m_pGrid->SelectGetNext();
		}

		nIds = m_pDM->SendFrame();
		if ( nIds < 0 )
  		{
  			AfxMessageBox(_T("SendFrame 작업이 실패했습니다."));
			return;
  		}

		nIds = m_pDM->EndFrame();
		if ( nIds < 0 )
  		{
  			AfxMessageBox(_T("EndFrame 작업이 실패했습니다."));
			return;
  		}		

		AfxMessageBox(_T("원문 삭제를 완료했습니다."));
		m_pGrid->SetProtectSelectColumn(TRUE);
		m_pGrid->Display();
		m_pGrid->SetReverse(nCurPos);
		m_pGrid->SetProtectSelectColumn(FALSE);
	}
	else
	{
		AfxMessageBox(_T("선정된 데이터가 없습니다."));
	}
}

INT CDCM_MANAGER::MakeDeleteQuery(INT nIdx)
{
	CString strData;
	CString strQuery;

	strData = m_pDM->GetCellData(_T("원문KEY"), nIdx);
	m_pDM->MakeDeleteFrame(_T("DL_CONTENTS_MAP_TBL"), _T("REC_KEY"), _T("NUMERIC"), strData);
	m_pDM->SetCellData(_T("원문KEY"), _T(""), nIdx);
	m_pDM->SetCellData(_T("서버ID"), _T(""), nIdx);
	m_pDM->SetCellData(_T("파일명"), _T(""), nIdx);
	m_pDM->SetCellData(_T("파일PATH"), _T(""), nIdx);

	return 0;
}

