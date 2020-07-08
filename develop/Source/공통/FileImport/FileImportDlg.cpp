// FileImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"

#include "FileImportDlg.h"
#include "MarcEditDlg.h"
#include "efs.h"
#include "../BO_ACQ_GROUP_MANAGER/Group_Insert_Dlg.h"
#include "../BO_ACQ_GROUP_MANAGER/RequisitionNo_Insert_Dlg.h"
#include "../BO_ACQ_GROUP_MANAGER/DivisionNo_Insert_Dlg.h"
#include "../BO_ACQ_GROUP_MANAGER/GroupPair.h"
#include "../BO_ACQ_GROUP_MANAGER/OtherGroupNo_Insert_Dlg.h"
#include "..\\..\\관리\\목차초록관리\\ContentsMgr_Editor\\CONTENTMGR_BO_EDIT.h"

#ifndef OBM_SIZE
#define	OBM_SIZE		32766

#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileImportDlg dialog


CFileImportDlg::CFileImportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileImportDlg)	
	m_strFilePath = _T("");
	m_bCodeApply = TRUE;
	m_bMultiVol = TRUE;
	m_bTOCImport = TRUE;
	//}}AFX_DATA_INIT

	m_nOpenMode = 1000;
	m_pThreadImport = NULL;
	m_pThreadDisplay = NULL;

	m_bIsThreadRunning = FALSE;
	m_bThreadExecute = FALSE;
	m_pBOSPECIES = NULL;

	m_pDM_Index = NULL;
	m_pDM_Species = NULL;

	m_pGrid_OK = NULL;
	m_pGrid_ERR = NULL;
	m_pGrid_SUCCESS = NULL;

	m_nFileType = 0;
	
	m_hThread= ::CreateEvent(NULL, FALSE, FALSE, _T("ThreadEvent"));

	m_pCM = NULL;
	m_pCM_ClassCode = NULL;
	m_pComboMgr = NULL;
	m_pGroup = NULL;
	m_pGroupCollection = NULL;
	m_pParentGroupCollection = NULL;
	m_pParentDM = NULL;
	m_pParentBookDM = NULL;
	m_pParentGrid = NULL;
	m_nAcqType = ACQ_TYPE_INVALID;

	m_nImportStatusArray = 0;
	m_strImportStatusArray[m_nImportStatusArray][0] = _T("수서");	m_strImportStatusArray[m_nImportStatusArray][1] = _T("반입자료");	m_strImportStatusArray[m_nImportStatusArray][2] = _T("BOT112O");		m_nImportStatusArray++;
	m_strImportStatusArray[m_nImportStatusArray][0] = _T("수서기초");	m_strImportStatusArray[m_nImportStatusArray][1] = _T("기초자료");	m_strImportStatusArray[m_nImportStatusArray][2] = _T("BOT111O");		m_nImportStatusArray++;
	m_strImportStatusArray[m_nImportStatusArray][0] = _T("수서구입");	m_strImportStatusArray[m_nImportStatusArray][1] = _T("구입자료");	m_strImportStatusArray[m_nImportStatusArray][2] = _T("BOA111N");		m_nImportStatusArray++;
	m_strImportStatusArray[m_nImportStatusArray][0] = _T("수서검수");	m_strImportStatusArray[m_nImportStatusArray][1] = _T("검수자료");	m_strImportStatusArray[m_nImportStatusArray][2] = _T("BOA112N");		m_nImportStatusArray++;
	m_strImportStatusArray[m_nImportStatusArray][0] = _T("기증접수");	m_strImportStatusArray[m_nImportStatusArray][1] = _T("기증자료");	m_strImportStatusArray[m_nImportStatusArray][2] = _T("BOA211O");		m_nImportStatusArray++;
	m_strImportStatusArray[m_nImportStatusArray][0] = _T("등록");	m_strImportStatusArray[m_nImportStatusArray][1] = _T("등록인계자료");	m_strImportStatusArray[m_nImportStatusArray][2] = _T("BOR111N");		m_nImportStatusArray++;	
	m_strImportStatusArray[m_nImportStatusArray][0] = _T("정리");	m_strImportStatusArray[m_nImportStatusArray][1] = _T("복본조사대상자료");	m_strImportStatusArray[m_nImportStatusArray][2] = _T("BOC111O");		m_nImportStatusArray++;

	m_strImportStatusGroup = _T("");
	m_strImportStatus = _T("");

	m_bIsExcel = TRUE;
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = FALSE;
	}	

}

CFileImportDlg::~CFileImportDlg()
{
	if(m_pComboMgr)
	{
		delete m_pComboMgr;
		m_pComboMgr = NULL;
	}
	if(m_pGroupCollection)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = NULL;
	}
	if (m_pDM_Index != NULL)
	{
		delete m_pDM_Index;
		m_pDM_Index = NULL;
	}

	if (m_pDM_Species != NULL)
	{
		delete m_pDM_Species;
		m_pDM_Species = NULL;
	}

	if (m_pBOSPECIES != NULL)
	{
		delete m_pBOSPECIES;
		m_pBOSPECIES = NULL;
	}
	CloseHandle(m_hThread);
}

BOOL CFileImportDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CFileImportDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileImportDlg)
	DDX_Control(pDX, IDC_cmbIMPORT_STATUS, m_cImportStatus);
	DDX_Control(pDX, IDC_cmbIMPORTTYPE, m_cImportType);	
	DDX_Text(pDX, IDC_eFILE_PATH, m_strFilePath);
	DDX_Check(pDX, IDC_chkCODE_APPLY, m_bCodeApply);
	DDX_Check(pDX, IDC_chkAPPLY_MULTI_VOL, m_bMultiVol);
	DDX_Check(pDX, IDC_chkTOC, m_bTOCImport);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CFileImportDlg, CDialog)
	//{{AFX_MSG_MAP(CFileImportDlg)
	ON_BN_CLICKED(IDC_bFILE_OPEN, OnbFILEOPEN)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_bIMPORT, OnbIMPORT)
	ON_BN_CLICKED(IDC_bEXPORT, OnbEXPORT)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_IMPORT, OnSelchangeTabImport)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bNORMALIZATION, OnbNORMALIZATION)
	ON_BN_CLICKED(IDC_btnCLOSE, OnbCLOSE)
	ON_CBN_SELCHANGE(IDC_cmbIMPORTTYPE, OnSelchangecmbIMPORTTYPE)
	ON_CBN_SELCHANGE(IDC_cmbFIMPGROUP_INFO, OnSelchangecmbFIMPGROUPINFO)
	ON_CBN_SELCHANGE(IDC_cmbFIMPWORK_NO, OnSelchangecmbFIMPWORKNO)
	ON_BN_CLICKED(IDC_btnFIMPGROUP_CREATE, OnbtnFIMPGROUPCREATE)
	ON_CBN_SELCHANGE(IDC_cmbWORK_CODE, OnSelchangecmbWORKCODE)
	ON_CBN_SELCHANGE(IDC_cmbCLASIFY_CLASS, OnSelchangecmbCLASIFYCLASS)
	ON_CBN_SELCHANGE(IDC_cmbFORM_CLASS, OnSelchangecmbFORMCLASS)
	ON_EN_KILLFOCUS(IDC_edtFIMPACQ_YEAR, OnKillfocusedtFIMPACQYEAR)
	ON_BN_CLICKED(IDC_rTXTFILE, OnrTXTFILE)
	ON_BN_CLICKED(IDC_rMODSFILE, OnrMODSFILE)
	ON_BN_CLICKED(IDC_rTXT, OnrTXT)
	ON_BN_CLICKED(IDC_rEXCEL, OnrEXCEL)	
	ON_BN_CLICKED(IDC_rCSV, OnrCSV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GRID_DISPLAY, MsgDisplayGrid)
	ON_MESSAGE(WM_SORT_GRID, MsgSortGrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileImportDlg 
INT CFileImportDlg::Init()
{
	EFS_BEGIN

	m_pBOSPECIES = new CBO_SPECIES(this);
	m_pBOSPECIES->SetOpenMode(4000);
	m_pBOSPECIES->InitDataMgr();

	CString strDMPK;

	m_pDM_Species = new CESL_DataMgr;
	if (m_pDM_Species == NULL) return -1;

	m_pDM_Species->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Species->InitDMPKFromFile(_T("DMUP_단행_종_종정보"));
	m_pDM_Species->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDM_Index = new CESL_DataMgr;
	if (m_pDM_Index == NULL) return -1;
	m_pDM_Index->SetCONNECTION_INFO(CONNECTION_INFO);
	m_pDM_Index->InitDMPKFromFile(_T("DMUP_단행_종_색인정보"));
	m_pDM_Index->SetMarcMgr(m_pInfo->pMarcMgr);

	m_pDmGridOk = FindDM(_T("DM_BO_FILE_IMPORT_GRID_OK"));
	if( m_pDmGridOk == NULL )
	{
		AfxMessageBox(_T("m_pDmGridOk is null"));
		return -1;
	}

	m_pDmGridError = FindDM(_T("DM_BO_FILE_IMPORT_GRID_ERROR"));
	if( m_pDmGridError == NULL )
	{
		AfxMessageBox(_T("m_pDmGridError is null"));
		return -1;
	}

	m_pDmGridSuccess = FindDM(_T("DM_BO_FILE_IMPORT_GRID_SUCCESS"));
	if( m_pDmGridSuccess == NULL )
	{
		AfxMessageBox(_T("m_pDmGridSuccess is null"));
		return -1;
	}

	m_pCM = FindCM(_T("CM_BO_FILE_IMPORT"));
	m_pCM_ClassCode = FindCM(_T("CM_BO_FILE_IMPORT_구분"));

	m_pGrid_OK = (CESL_Grid*)m_pCM->FindControl(_T("Grid_OK"));
	
	m_pGrid_ERR = (CESL_Grid*)m_pCM->FindControl(_T("Grid_ERR"));
	
	m_pGrid_SUCCESS = (CESL_Grid*)m_pCM->FindControl(_T("Grid_SUCCESS"));

	if (m_pGrid_OK == NULL || m_pGrid_ERR == NULL || m_pGrid_SUCCESS == NULL) return -1;

	CRect rcGrid;
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	
	m_pGrid_OK->MoveWindow(rcGrid);
	m_pGrid_ERR->MoveWindow(rcGrid);
	m_pGrid_SUCCESS->MoveWindow(rcGrid);
	
	this->SetCONNECTION_INFO(m_pDM_Index->CONNECTION_INFO);

	( (CButton*) GetDlgItem (IDC_rKORMARC) )->SetCheck(TRUE);

	return 0;

	EFS_END
	return -1;

}

VOID CFileImportDlg::ButtonStatus(INT nMode)
{
	EFS_BEGIN

	const INT nIDCount = 5;
	INT nArrID[nIDCount] = {
		IDC_bFILE_OPEN, IDC_bMODIFY, IDC_bNORMALIZATION,
		IDC_bIMPORT, IDC_bEXPORT
	};

	for (INT i = 0; i < nIDCount; i++)
	{
		if (nMode == 0)
		{
			GetDlgItem(nArrID[i])->EnableWindow(FALSE);
		}
		else if (nMode == 1)
		{
			GetDlgItem(nArrID[i])->EnableWindow(TRUE);
		}
	}

	if (nMode == 0)
	{
		GetDlgItem(IDC_btnCLOSE)->SetWindowText(_T("중 지"));
	}
	else if (nMode == 1)
	{
		GetDlgItem(IDC_btnCLOSE)->SetWindowText(_T("닫 기"));
	}

	EFS_END
}

VOID CFileImportDlg::ImportMarcFromFile()
{
	EFS_BEGIN

	BeginWaitCursor();
	INT ids;
	
	if (m_strFilePath.IsEmpty()) return;

	CString sFormCode, sMarcGroup;
	GetControlData( _T("CM_BO_FILE_IMPORT_구분"), _T("IBS_형식구분_코드"), sFormCode );
	
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	
	m_marcImpExpMgr.Init(this, m_pInfo->pMarcMgr, m_nFileType, m_strFilePath);
	
	m_marcImpExpMgr.InitDMList();
	m_marcImpExpMgr.AddDMPtr(m_pDM_Species);
	m_marcImpExpMgr.AddDMPtr(m_pDM_Index);
	
	m_marcImpExpMgr.SetBookCountAlias(_T("BS_책수"));
	
	m_marcImpExpMgr.SetErrChkAlias(_T("BS_파일반입오류"));
	
	m_marcImpExpMgr.SetMarcStreamAlias(_T("BS_MARC"));

	INT nCheck;
	nCheck = ( (CButton*) GetDlgItem (IDC_rUSMARC) )->GetCheck();		

	ids = m_marcImpExpMgr.ImportFile(_T(""),nCheck);

	if (ids >= 0)
	{
		m_pThreadDisplay = AfxBeginThread(ThreadFunc_Display, this);
	}
	EndWaitCursor();

	EFS_END
}

INT CFileImportDlg::ExportToTxtFile(CString strFilePath)
{
	EFS_BEGIN

	BeginWaitCursor();

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return -1;

	INT nSel = pTabCtrl->GetCurSel();
	CESL_DataMgr *pDM = NULL;

	if (nSel == 0)
	{
		pDM = m_pDmGridOk;
	}
	else if (nSel == 1)
	{
		pDM = m_pDmGridError;
	}
	else if (nSel == 2)
	{
		pDM = m_pDmGridSuccess;
	}
	
	if (pDM == NULL) 
	{
		return 0;
	}
	
	if (pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("반출할 자료가 없습니다."));
			return 0;
	}

	CStdioFile file;
	
	INT nConvertMode;
	nConvertMode = m_MarcConvertApi.m_nConvertModeChk;
	if(nConvertMode == _UTF8_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		if( 0 == file.GetLength() )
		{
			CHAR  cMul1;
			CHAR  cMul2;
			CHAR  cMul3;
			BYTE byteMul1 = 0xEF;
			BYTE byteMul2 = 0xBB;
			BYTE byteMul3 = 0xBF;
			
			cMul1 = (CHAR)byteMul1;
			cMul2 = (CHAR)byteMul2;
			cMul3 = (CHAR)byteMul3;
			
			file.Write( &cMul1, sizeof(CHAR));
			file.Write( &cMul2, sizeof(CHAR));
			file.Write( &cMul3, sizeof(CHAR));
		}
	}
	else if(nConvertMode == _UNICODE_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}
	}
	else if(nConvertMode == _ANSI_FILE)
	{
		if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		return -1;
	}
	

	CString strErrMarc;
	CString strIndex;
	INT nNeedByte;
	INT nCount = pDM->GetRowCount();
	for (INT i = 0; i < nCount; i++)
	{
		strIndex = pDM->GetCellData(_T("순위"), i);
		strErrMarc = m_pDM_Species->GetCellData(_T("BS_MARC"), _ttoi(strIndex) - 1);

		if (strErrMarc.IsEmpty()) continue;

		m_MarcConvertApi.GetMarcStruct(strErrMarc);
		strErrMarc.Format(m_MarcConvertApi.m_strUniConvertMarc);
		if(nConvertMode == _UTF8_FILE)
		{			
			nNeedByte = m_MarcConvertApi.GetLengthUnicodeToUTF8(strErrMarc);
			m_MarcConvertApi.GetStrUniCodeToUtf8(strErrMarc);	
			CHAR cTemp;
			for( INT i = 0; i < nNeedByte-1 ; i++ )
			{
				cTemp = *(m_MarcConvertApi.m_cpConvertData+i);
				file.Write(&cTemp, sizeof(CHAR));
			}
			CString strTemp;
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
		}
		else if(nConvertMode == _UNICODE_FILE)
		{
			strErrMarc += _T("\r\n");
			file.WriteString(strErrMarc);			
		}
		else if(nConvertMode == _ANSI_FILE)
		{
			nNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(strErrMarc);
			m_MarcConvertApi.GetStrUniCodeToAnsi(strErrMarc);
			CHAR cTemp;
			for( INT i = 0; i < nNeedByte-1 ; i++ )
			{
				cTemp = *(m_MarcConvertApi.m_cpConvertData+i);
				file.Write(&cTemp, sizeof(CHAR));
			}
			CString strTemp;
			cTemp = '\r';
			file.Write(&cTemp, sizeof(CHAR));
			cTemp = '\n';
			file.Write(&cTemp, sizeof(CHAR));
		}		
	}

	file.Close();

	EndWaitCursor();

	AfxMessageBox(_T("반출 완료"));

	return 0;

	EFS_END
	return -1;

}

VOID CFileImportDlg::CodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();

	const INT nCodeAliasCount = 11;
	CString arrStrAlias[nCodeAliasCount] = {
		_T("IBS_이용제한구분_코드"),
		_T("IBS_자료구분_코드"),
		_T("IBS_매체구분_코드"),
		_T("IBS_보조등록구분_코드"),
		_T("IBS_이용대상구분_코드"),
		_T("IBS_형식구분_코드"),
		_T("IBS_분류표구분"),
		_T("IBS_등록구분_코드"),
		_T("IBS_업무구분_코드"),
		_T("IBS_제어자료구분_코드"),
		_T("BS_주제구분_코드")
	};

	CString arrStrData[nCodeAliasCount];
	CString strAlias;

	for (INT i = 0; i < nCodeAliasCount; i++)
	{
		GetControlData(_T("CM_BO_FILE_IMPORT_구분"), arrStrAlias[i], arrStrData[i], -1, 0);
	}
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
	{
		m_pDM_Index->SetCellData(arrStrAlias, nCodeAliasCount, arrStrData, row);
		
	}
	nRowCount = m_pDM_Species->GetRowCount();

	CString strClassNo;
	CString strClassNoType;

	for (row = 0; row < nRowCount; row++)
	{		
		for(i=0 ; i<11 ; i++)
		{
			strAlias = arrStrAlias[i];
			if(strAlias == _T("IBS_제어자료구분_코드"))
			{
				m_pDM_Species->SetCellData(_T("BS_제어자료구분"), arrStrData[i], row);
			}

			if(strAlias == _T("IBS_분류표구분"))
			{
				strClassNoType = arrStrData[i];
			}

			if ( strAlias == _T("BS_주제구분_코드") )
			{
				if ( strClassNoType == _T("1") )
				{
					m_pDM_Species->GetCellData(_T("BS_MARC.KDC분류기호"), row, strClassNo);
					INT nLength;
					TCHAR cData;
					nLength = strClassNo.GetLength();
					for ( INT i = 0 ; i < nLength ; i++ )
					{
						if( strClassNo.IsEmpty() )
						{
							break;
						}

						cData = strClassNo.GetAt(i);

						if( !_istdigit(cData) && cData != '.' )
						{
 							strClassNo = _T("");
							m_pDM_Species->SetCellData(_T("BS_MARC.KDC분류기호"), strClassNo, row);
							break;
						}
					}
				}
				else if ( strClassNoType == _T("2") )
				{
					m_pDM_Species->GetCellData(_T("BS_MARC.DDC분류기호"), row, strClassNo);
					INT nLength;
					TCHAR cData;
					nLength = strClassNo.GetLength();
					for ( INT i = 0 ; i < nLength ; i++ )
					{
						if( strClassNo == _T("적용안함") || strClassNo.IsEmpty() )
						{
							break;
						} 

						cData = strClassNo.GetAt(i);

						if( !isdigit(cData) && cData != '.' )
						{
							strClassNo = _T("");
							m_pDM_Species->SetCellData(_T("BS_MARC.DDC분류기호"), strClassNo, row);
							break;
						}
					}
				}

				if ( strClassNo != _T("") ) 
				{
					arrStrData[i] = strClassNo.Left(1);
					m_pDM_Species->SetCellData(_T("BS_주제구분_코드"), arrStrData[i], row);
				}
				else
				{
					CESL_ControlMultiComboBox *pCM_Subject = (CESL_ControlMultiComboBox *)m_pCM_ClassCode->FindControl(_T("BS_주제구분_코드"));
 					pCM_Subject->GetData(strClassNo,pCM_Subject->GetCurSel(),0);
					arrStrData[i] = strClassNo.Left(1);
					m_pDM_Species->SetCellData(_T("BS_주제구분_코드"), arrStrData[i], row);
				}
			}
			strAlias.Replace(_T("IBS"), _T("BS"));

			m_pDM_Species->SetCellData(strAlias, arrStrData[i], row);
		}
	}

	EndWaitCursor();

	EFS_END
}

VOID CFileImportDlg::WorkCodeApply()
{
	EFS_BEGIN

	BeginWaitCursor();

	const INT nCodeAliasCount = 1;
	CString arrStrAlias[nCodeAliasCount] = {
		_T("IBS_업무구분_코드"),
	};

	CString arrStrData[nCodeAliasCount];
	CString strAlias;

	for (INT i = 0; i < nCodeAliasCount; i++)
	{
		GetControlData(_T("CM_BO_FILE_IMPORT_구분"), arrStrAlias[i], arrStrData[i], -1, 0);
	}
	
	INT nRowCount = m_pDM_Index->GetRowCount();
	for (INT row = 0; row < nRowCount; row++)
	{
		m_pDM_Index->SetCellData(arrStrAlias, nCodeAliasCount, arrStrData, row);
		
	}
	nRowCount = m_pDM_Species->GetRowCount();

	CString strClassNo;
	CString strClassNoType;

	for (row = 0; row < nRowCount; row++)
	{		
		for(i=0 ; i<1 ; i++)
		{
			strAlias = arrStrAlias[i];
			if(strAlias == _T("IBS_제어자료구분_코드"))
			{
				m_pDM_Species->SetCellData(_T("BS_제어자료구분"), arrStrData[i], row);
			}

			if(strAlias == _T("IBS_분류표구분"))
			{
				strClassNoType = arrStrData[i];
			}

			if ( strAlias == _T("BS_주제구분_코드") )
			{
				if ( strClassNoType == _T("1") )
				{
					m_pDM_Species->GetCellData(_T("BS_MARC.KDC분류기호"), row, strClassNo);
					
					INT nLength;
					TCHAR cData;
					nLength = strClassNo.GetLength();
					for ( INT i = 0 ; i < nLength ; i++ )
					{
						if( strClassNo.IsEmpty() )
						{
							break;
						}

						cData = strClassNo.GetAt(i);

						if( !_istdigit(cData) && cData != '.' )
						{
 							strClassNo = _T("");
							m_pDM_Species->SetCellData(_T("BS_MARC.KDC분류기호"), strClassNo, row);
							break;
						}
					}
				}
				else if ( strClassNoType == _T("2") )
				{
					m_pDM_Species->GetCellData(_T("BS_MARC.DDC분류기호"), row, strClassNo);
					INT nLength;
					TCHAR cData;
					nLength = strClassNo.GetLength();
					for ( INT i = 0 ; i < nLength ; i++ )
					{
						if( strClassNo == _T("적용안함") || strClassNo.IsEmpty() )
						{
							break;
						} 

						cData = strClassNo.GetAt(i);

						if( !isdigit(cData) && cData != '.' )
						{
							strClassNo = _T("");
							m_pDM_Species->SetCellData(_T("BS_MARC.DDC분류기호"), strClassNo, row);
							break;
						}
					}
				}

				if ( strClassNo != _T("") ) 
				{
					arrStrData[i] = strClassNo.Left(1);
					m_pDM_Species->SetCellData(_T("BS_주제구분_코드"), arrStrData[i], row);
				}
				else
				{
					CESL_ControlMultiComboBox *pCM_Subject = (CESL_ControlMultiComboBox *)m_pCM_ClassCode->FindControl(_T("BS_주제구분_코드"));
 					pCM_Subject->GetData(strClassNo,pCM_Subject->GetCurSel(),0);
					arrStrData[i] = strClassNo.Left(1);
					m_pDM_Species->SetCellData(_T("BS_주제구분_코드"), arrStrData[i], row);
				}
			}
			strAlias.Replace(_T("IBS"), _T("BS"));

			m_pDM_Species->SetCellData(strAlias, arrStrData[i], row);
		}
	}

	EndWaitCursor();

	EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CFileImportDlg message handlers

BOOL CFileImportDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	CButton *pBtn_ANSI = ( (CButton*) GetDlgItem (IDC_rANSI) );
	pBtn_ANSI->SetCheck(0);
	CButton *pBtn_UTF8 = ( (CButton*) GetDlgItem (IDC_rUTF8) );
	pBtn_UTF8->SetCheck(0);
	CButton *pBtn_UNICODE = ( (CButton*) GetDlgItem (IDC_rUNICODE) );
	pBtn_UNICODE->SetCheck(1);

	if (InitESL_Mgr(_T("BO_FILE_IMPORT")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	if (Init() < 0)
	{
		AfxMessageBox(_T("Initialization is Failed!"));
		EndDialog(IDCANCEL);
		return FALSE;
	}
	
	SetImportTypeArray();

	GetWindowRect(m_rcInitial);

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl)
	{
		pTabCtrl->InsertItem(0, _T("정상"));
		pTabCtrl->InsertItem(1, _T("오류"));
		pTabCtrl->InsertItem(2, _T("성공"));

		pTabCtrl->SetCurFocus(1);
		pTabCtrl->SetCurFocus(0);
	}
	m_wndStatusBarCtrl.Create(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP , CRect(0, 0, 0, 0), this, WM_USER + 1000);
	m_wndStatusBarCtrl.SetMinHeight(22);
	
	if (!m_wndProgressBar.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, CRect(0,0,100,100), &m_wndStatusBarCtrl, 17298))
		return -1;
	m_wndProgressBar.SetFont(GetFont());
	
	const INT nParts = 5;
	CRect rect;
	m_wndStatusBarCtrl.GetClientRect(&rect);
	INT widths[nParts] = { 100, 200, 300, 400, -1 };
	m_wndStatusBarCtrl.SetParts(nParts, widths);
	
	INT nCur = 0;
	m_cImportStatus.ResetContent();
	for (INT i = 0; i < m_nImportStatusArray; i++) {
		if (m_strImportStatusGroup == m_strImportStatusArray[i][0]) {
			m_cImportStatus.AddString(m_strImportStatusArray[i][1]);
			if (m_strImportStatus == m_strImportStatusArray[i][2]) m_cImportStatus.SetCurSel(nCur);
			nCur++;
		}
	}

	CString strValue;
	GetManageValue( _T("기타") , _T("공통") , _T("5163") , _T("5163") , strValue );
	if ( strValue != _T("Y") )
	{
		GetDlgItem(IDC_rTXTFILE)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_rMODSFILE)->ShowWindow(SW_HIDE);
	}
	
	nCur = 0;

	m_cImportType.ResetContent();
	for (i = 0; i < m_nImportTypeArray; i++) {
		if (m_strImportStatusGroup == m_strImportTypeArray[i][0]) {
			m_cImportType.AddString(m_strImportTypeArray[i][1]);
			nCur++;
		}
	}
	m_cImportType.SetCurSel(0);

	m_pComboMgr = new CMulitComboBoxManager(this);
	m_pGroupCollection = new CGroupCollection(this);
	
	CString strFormCode;
	CString strCodeDesc;
	m_pInfo->pCodeMgr->GetDefaultCode(_T("입수구분"), strFormCode);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("입수구분"),strFormCode,strCodeDesc);
	m_cImportType.SelectString(0,strCodeDesc);

	SetCtrlByImportType();
	OnSelchangecmbCLASIFYCLASS();

	( (CButton*) GetDlgItem (IDC_rEXCEL) )->SetCheck(TRUE);
	
	PostMessage(WM_SIZE);

	

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CFileImportDlg::OnbCLOSE() 
{
	EFS_BEGIN

	CString strText;

	GetDlgItem(IDC_btnCLOSE)->GetWindowText(strText);
	strText.Replace(_T(" "), _T(""));

	INT nIdx;
	INT nCombSel;
	CString strCodeDesc;
	m_strExtAcqYear = m_strAcqYear;

	nCombSel = m_cImportType.GetCurSel();
	m_cImportType.GetWindowText(strCodeDesc);
	strCodeDesc.Replace(_T(" "), _T(""));
	if( _T("적용안함") != strCodeDesc )
	{
		CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("그룹정보"));
		nIdx = pCmb->GetCurSel();
		pCmb->GetLBText(nIdx,m_strExtGruopInfo);
	}	

	if (strText == _T("닫기"))
	{
		if (m_bIsThreadRunning)
		{
			m_bIsThreadRunning = FALSE;

			MSG msg;
			while (TRUE)
			{
				if (::PeekMessage(&msg,GetSafeHwnd(), 1, 1, PM_NOREMOVE))
					AfxGetApp()->PumpMessage();

				if (WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0)
					break;
			}
		}
		CDialog::OnOK();
	}
	else if (strText == _T("중지"))
	{
		m_bIsThreadRunning = FALSE;
	}

	m_pParentGrid->SelectMakeList();
	m_pParentGrid->SetProtectSelectColumn(TRUE);
	m_pParentGrid->Display();
	m_pParentGrid->SetProtectSelectColumn(FALSE);

	if(!m_bThreadExecute)
	{
		m_strExtAcqYear.Empty();
		m_strExtGruopInfo.Empty();
	}

	EFS_END
}

VOID CFileImportDlg::OnSelchangeTabImport(NMHDR* pNMHDR, LRESULT* pResult) 
{
	EFS_BEGIN

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CMSHFlexGrid *pGridNormal = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_OK);
	CMSHFlexGrid *pGridError  = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_ERR);
	CMSHFlexGrid *pGridSuccess  = (CMSHFlexGrid*)GetDlgItem(IDC_MSHFLEXGRID_SUCCESS);

	if (pGridNormal == NULL || pGridError == NULL || pGridSuccess == NULL) return;
	
	if (nCurSel == 0)
	{
		pGridNormal->ShowWindow(SW_SHOW);
		pGridError->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 1)
	{
		pGridError->ShowWindow(SW_SHOW);
		pGridNormal->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_HIDE);
	}
	else if (nCurSel == 2)
	{
		pGridError->ShowWindow(SW_HIDE);
		pGridNormal->ShowWindow(SW_HIDE);
		pGridSuccess->ShowWindow(SW_SHOW);
	}
	
	
	*pResult = 0;

	EFS_END
}

VOID CFileImportDlg::OnSize(UINT nType, INT cx, INT cy) 
{
	EFS_BEGIN
		
	CDialog::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL) return;
	
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (rcWnd.Width() < m_rcInitial.Width())
	{
		rcWnd.right = rcWnd.left + m_rcInitial.Width();
		if (rcWnd.Height() < m_rcInitial.Height())
			rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		
		MoveWindow(rcWnd);
		return;
	}
	
	if (rcWnd.Height() < m_rcInitial.Height())
	{
		rcWnd.bottom = rcWnd.top + m_rcInitial.Height();	
		if (rcWnd.Width() < m_rcInitial.Width())
			rcWnd.right = rcWnd.left + m_rcInitial.Width();			
		
		MoveWindow(rcWnd);
		return;
	}
	
	INT i = 0;
	CRect rcClient;
	CWnd *pWnd = NULL;
	
	CRect rcDlg;
	GetClientRect(rcDlg);
	
	pWnd = GetDlgItem(IDC_TAB_IMPORT);
	if (pWnd)
	{
		pWnd->GetWindowRect(rcClient);
		ScreenToClient(rcClient);
		
		rcClient.right = rcDlg.right - 10;
		rcClient.bottom = rcDlg.bottom - 24;
		
		pWnd->MoveWindow(rcClient);
	}
	
	pWnd = GetDlgItem(IDC_STATIC_GRID);
	if (pWnd)
	{
		rcClient.DeflateRect(5, 5);
		rcClient.top += 20;
		pWnd->MoveWindow(rcClient);
	}
	
	const INT nResizeControlCnt = 3;
	INT nArrayResizeControl[nResizeControlCnt] = {
		IDC_MSHFLEXGRID_OK, IDC_MSHFLEXGRID_ERR, 
			IDC_MSHFLEXGRID_SUCCESS, 
	};	
	
	for (i = 0; i < nResizeControlCnt; i++)
	{
		pWnd = GetDlgItem(nArrayResizeControl[i]);
		if (pWnd != NULL)
		{
			pWnd->MoveWindow(rcClient);
		}
	}
	
	if (m_wndStatusBarCtrl.GetSafeHwnd() != NULL)
		m_wndStatusBarCtrl.SendMessage(WM_SIZE, 0, 0);
	
	if(!::IsWindow(m_wndProgressBar.m_hWnd))
        return;
	
	CRect   rc;
    m_wndStatusBarCtrl.GetRect(4,rc);
	rc.top -=1;
	rc.bottom +=5;
    
    m_wndProgressBar.SetWindowPos(NULL,rc.left,
		rc.top-1,
		rc.Width(),
		rc.Height()+2,SWP_NOZORDER | SWP_NOACTIVATE);
	
	m_wndProgressBar.GetClientRect(rc);
	HRGN hrgn = CreateRectRgn(rc.left+2, rc.top+4, rc.right-2, rc.bottom-6);
	m_wndProgressBar.SetWindowRgn(hrgn, TRUE);
	DeleteObject(hrgn);
	EFS_END
}

BOOL CFileImportDlg::PreTranslateMessage(MSG* pMsg) 
{
	EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		return CESL_Mgr::PreTranslateMessage(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);

	EFS_END
	return FALSE;

}

VOID CFileImportDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();	

	EFS_END
}

VOID CFileImportDlg::OnbFILEOPEN() 
{
	EFS_BEGIN

	UpdateData();

	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	ZeroMemory(&OFN, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	if (m_nFileType == 0)
	{
		if (m_bIsExcel != TRUE) 
		{
			OFN.lpstrFilter = _T("한셀파일(*.cell)\0*.cell\0모든파일(*.*)\0*.*\0");
			OFN.lpstrTitle = _T("한셀 파일 저장");
		}	
		else
		{
			OFN.lpstrFilter = _T("XLS파일(*.xls)\0*.xls\0모든파일(*.*)\0*.*\0");
			OFN.lpstrTitle = _T("EXCEL 파일 열기");
		}
	}
	else if( m_nFileType == 1 || m_nFileType == 2 || m_nFileType == 3)
	{
		OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("TEXT 파일 열기");
	}
	else if( m_nFileType == 5) 
	{
		OFN.lpstrFilter = _T("TXT파일 (탭으로 분리된 텍스트) (*.txt)\0*.txt\0CSV파일 (탭으로 분리된 텍스트) (*.csv)\0*.csv\0모든파일 (탭으로 분리된 텍스트)(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("CSV 파일 열기");
	}

	if (GetOpenFileName(&OFN) != 0)
	{
		m_strFilePath = OFN.lpstrFile;
		UpdateData(FALSE);

		m_wndProgressBar.SetWindowText(_T("파일읽기 준비중..."));
		m_wndProgressBar.UpdateWindow();
		
		SetCurrentDirectory(szBuffer);
		ImportMarcFromFile();
	}
	
	EFS_END
}

VOID CFileImportDlg::OnbMODIFY() 
{
	EFS_BEGIN

	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;
	
	INT nCurSel = pTabCtrl->GetCurSel();

	CESL_Grid *pGrid = NULL;
	CESL_DataMgr *pDm = NULL;
	
	if (nCurSel == 0)
	{
		pGrid = m_pGrid_OK;
		pDm   = m_pDmGridOk;
	}
	else if (nCurSel == 1)
	{
		pGrid = m_pGrid_ERR;
		pDm   = m_pDmGridError;
	}

	if (pGrid == NULL) 
	{
		return;
	}

	pGrid->SelectMakeList();
	INT nSelectCnt;
	nSelectCnt = pGrid->SelectGetCount();
	if( 1 > nSelectCnt )
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	CMarcEditDlg dlg(this);
	dlg.SetMarcImpExpMgr(&m_marcImpExpMgr);
	INT nChkMultiBook;
	CButton * pChkGiveAutoRegno = (CButton *)GetDlgItem(IDC_chkAPPLY_MULTI_VOL);
	nChkMultiBook = pChkGiveAutoRegno->GetCheck();	 

	dlg.SetParentInfo(m_pDM_Species, pDm, pGrid, m_nFileType, _T("BS_MARC"), _T("BS_파일반입오류")
		              , nChkMultiBook);

	pGrid->SetProtectSelectColumn(TRUE);

	dlg.DoModal();

	pGrid->Display();
	INT nIdx;
	CString strData;
	nIdx = pGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		strData = pDm->GetCellData(_T("순위"), nIdx);
		pGrid->SetTextMatrix(nIdx+1, 0,strData);
		nIdx = pGrid->SelectGetNext();		
	}
	pGrid->SetProtectSelectColumn(FALSE);

	EFS_END
}

VOID CFileImportDlg::OnbNORMALIZATION() 
{
	EFS_BEGIN

	CString strData;
	CString strMsg;

	INT nRowOK		= 0;
	INT nRowsOK		= 0;
	INT nRecovers	= 0;

	BeginWaitCursor();

	CESL_ControlMgr *pCM = FindCM(_T("CM_BO_FILE_IMPORT"));
	if (pCM == NULL) return;

	INT nIdx;
	INT nSelectCnt;
	INT nColCnt;
	INT nCopyRow;

	nColCnt = m_pDmGridError->GetRefColumnCount();
	
	m_pGrid_ERR->SelectMakeList();
	nSelectCnt = m_pGrid_ERR->SelectGetCount();

	if( 0 < nSelectCnt )
	{
		nIdx = m_pGrid_ERR->SelectGetTailPosition();

		for( INT i = 0 ; i < nSelectCnt ; i++ )
		{
			strData = m_pDmGridError->GetCellData( _T("BS_파일반입오류"), nIdx );
			if( strData.IsEmpty() )
			{	
				nCopyRow = m_pDmGridOk->GetRowCount();
				m_pDmGridOk->InsertRow(-1);
				for( INT j = 0 ; j < nColCnt ; j++)
				{
					strData = m_pDmGridError->GetCellData( nIdx, j );
					m_pDmGridOk->SetCellData( nCopyRow, j, strData );
				}

				m_pDmGridError->DeleteRow(nIdx);
				nRecovers++;
			}	
			nIdx = m_pGrid_ERR->SelectGetPrev();			
		}
		
	}
	else
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return;
	}

	if( 1 > nRecovers )
	{
		AfxMessageBox( _T("오류복귀할자료가 없습니다.") );
		return;
	}
			
	EndWaitCursor();
	
	if( nRecovers > 0 )
	{
		INT nRowCnt;
		nRowCnt = m_pDmGridOk->GetRowCount();
		strData.Format( _T("%d"), nRowCnt );
		m_wndStatusBarCtrl.SetText(_T("정상 : ") + strData, 1, 0);
				
		nRowCnt = m_pDmGridError->GetRowCount();
		strData.Format( _T("%d"), nRowCnt );
		m_wndStatusBarCtrl.SetText(_T("오류 : ") + strData, 2, 0);
		
		strMsg.Format(_T("[ %d ]건 중 [ %d ] 건이 복귀했습니다."), nRowCnt + nRecovers, nRecovers);
		AfxMessageBox(strMsg);

		SortDm(m_pDmGridOk);

		DisplayGrid();
	}
	else
	{
		strMsg.Format(_T("오류 수정된 자료가 없습니다."));
		AfxMessageBox(strMsg);
	}

	EFS_END
}

VOID CFileImportDlg::OnbIMPORT() 
{
	EFS_BEGIN
	
	INT nCount;

	nCount = m_pDmGridOk->GetRowCount();
	if (nCount < 1) 
	{
		AfxMessageBox(_T("반입할 자료가 없습니다."));
		return;
	}

	m_pGrid_OK->SelectMakeList();

	nCount = m_pGrid_OK->SelectGetCount();
	if (nCount < 1) 
	{
		AfxMessageBox(_T("반입할 자료를 선정해 주세요"));
		return;
	}

	INT nIdx;
	INT nLength;
	CString strTemp;
	TCHAR cData;
	if( 0 == m_nFileType )
	{		
		nIdx = m_pGrid_OK->SelectGetHeadPosition();
		for( INT i = 0 ; i < nCount ; i++)
		{
			strTemp = m_pDmGridOk->GetCellData( _T("가격"), nIdx );
			strTemp.Replace(_T(","),_T(""));
			nLength = strTemp.GetLength();
			if( !strTemp.IsEmpty() )
			{
				for( INT j = 0 ; j < nLength ; j++ )
				{
					cData = strTemp.GetAt(j);
					if( !_istdigit(cData) && _T('.') != cData )
					{
						AfxMessageBox(_T("가격정보는 숫자만 입력해주세요."));
						return;
					}
				}
			}
			nIdx = m_pGrid_OK->SelectGetNext();
		}
	}
	INT ids = CheckFormCode();
	if(ids < 0) return ;
	
	UpdateData();
	if (m_bCodeApply)
	{
		CodeApply();
	}
	else
	{
		WorkCodeApply();
	}

	CString strStatusName;
	m_cImportStatus.GetWindowText(strStatusName);
	strStatusName.TrimLeft();			strStatusName.TrimRight();
	FindImportStatus(strStatusName, m_strImportStatus);

	CString strTmpData = m_strImportStatus.Left(3);
	if(strTmpData == _T("BOT") && m_pGroup == NULL) 
	{ 
		ESLAfxMessageBox(_T("그룹정보를 설정해 주세요."));
		return ;
	}
	else if(strTmpData != _T("BOT") && m_nAcqType != ACQ_TYPE_INVALID && m_pGroup == NULL)
	{
		if(m_nAcqType == PURCHASE)
			ESLAfxMessageBox(_T("차수정보를 설정해 주세요."));
		else if(m_nAcqType == DONATE)
			ESLAfxMessageBox(_T("접수번호정보를 설정해 주세요."));
		return ;
	}
	
	CString strAcqCode, strQuery;
	CGroupPair GroupPair(this);
	INT nTmpData;
	if(m_nAcqType == ACQ_TYPE_INVALID && m_pGroup)
	{
		strAcqCode = _T("0");
		nTmpData = GroupPair.GetBookLastSerialNo(m_pDM_Index, strAcqCode, m_pGroup);
		m_pGroup->SetBookLastSerialNo(nTmpData);
	}
	else if(m_nAcqType == PURCHASE)
	{
		strAcqCode = _T("1");
		CString strWorkNo = m_pGroup->GetWorkNo();
		if(strWorkNo == _T("새로생성")) 
		{
			m_pDM_Index->StartFrame();
			m_pGroupCollection->InsertWorkNo(m_pDM_Index);
			if(m_pDM_Index->SendFrame() < 0) return;
			if(m_pDM_Index->EndFrame() < 0) return;
		}
		nTmpData = GroupPair.GetLastWorkNo(m_pDM_Index, m_pGroup);
		m_pGroup->SetLastWorkNo(nTmpData);
		nTmpData = GroupPair.GetBookLastSerialNo(m_pDM_Index, strAcqCode, m_pGroup);
		m_pGroup->SetBookLastSerialNo(nTmpData);
	}
	else if(m_nAcqType == DONATE)
	{
		strAcqCode = _T("2");
		nTmpData = GroupPair.GetBookLastSerialNo(m_pDM_Index, strAcqCode, m_pGroup);
		m_pGroup->SetBookLastSerialNo(nTmpData);
	}
	else if(m_nAcqType == OTHER_ACQ_TYPE)
	{
		m_cImportType.GetWindowText(strAcqCode);
		m_pInfo->pCodeMgr->ConvertDescToCode(_T("입수구분"), strAcqCode, strAcqCode);
		if(m_pGroup->GetGroupInfo() != _T("적용안함"))
		{
			nTmpData = GroupPair.GetBookLastSerialNo(m_pDM_Index, strAcqCode, m_pGroup);
			m_pGroup->SetBookLastSerialNo(nTmpData);
		}
		m_pGroup->SetGroupNo(strAcqCode);		
	}

	if(m_pGroup != NULL) m_pGroup->SetRecKey(strAcqCode, m_pDM_Index);

	m_pThreadImport = AfxBeginThread(ThreadFunc_Import, this);

	EFS_END
}

VOID CFileImportDlg::OnbEXPORT() 
{
	EFS_BEGIN
		
	INT nExportFileType;
	BOOL bCheck;
	INT ids;
	
	bCheck = ( (CButton*) GetDlgItem (IDC_rANSI) )->GetCheck();
	if ( bCheck ) nExportFileType = _ANSI_FILE;

	bCheck = ( (CButton*) GetDlgItem (IDC_rUTF8) )->GetCheck();
	if ( bCheck ) nExportFileType = _UTF8_FILE;

	bCheck = ( (CButton*) GetDlgItem (IDC_rUNICODE) )->GetCheck();
	if ( bCheck ) nExportFileType = _UNICODE_FILE;
	
	ids = m_MarcConvertApi.SetConvertMode(nExportFileType);
	
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_TAB_IMPORT);
	if (pTabCtrl == NULL) return;

	INT nSel = pTabCtrl->GetCurSel();
	CESL_DataMgr *pDM = NULL;

	if (nSel == 0)
	{
		pDM = m_pDmGridOk;
	}
	else if (nSel == 1)
	{
		pDM = m_pDmGridError;
	}
	else if (nSel == 2)
	{
		pDM = m_pDmGridSuccess;
	}
	
	if (pDM == NULL) 
	{
		return;
	}
	
	if (pDM->GetRowCount() < 1)
	{
		AfxMessageBox(_T("반출할 자료가 없습니다."));
			return;
	}

	CString strFilePath;
	
	TCHAR szBuffer[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szBuffer);
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lpstrDefExt = _T("TXT");

	OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT 파일 저장");

	if (GetSaveFileName(&OFN) != 0)
	{
		strFilePath= OFN.lpstrFile;
		ExportToTxtFile(strFilePath);
	}

	SetCurrentDirectory(szBuffer);

	EFS_END
}

BEGIN_EVENTSINK_MAP(CFileImportDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CFileImportDlg)
	ON_EVENT(CFileImportDlg, IDC_MSHFLEXGRID_ERR, -605 /* MouseDown */, OnMouseDownMshflexgridErr, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CFileImportDlg, IDC_MSHFLEXGRID_OK, -605 /* MouseDown */, OnMouseDownMshflexgridOk, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CFileImportDlg::OnMouseDownMshflexgridOk(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	EFS_END
}

VOID CFileImportDlg::OnMouseDownMshflexgridErr(short Button, short Shift, long x, long y) 
{
	EFS_BEGIN

	EFS_END
}

INT CFileImportDlg::MakeTOC(INT nIndex)
{
	EFS_BEGIN
	
	CString strContentsYN;
	CString strAbstractsYN;
	CString strSpeciesKey;
	CString strContentsKey;
	CString strAbstractsKey;
	CString strTocCommonKey;
	CString strSystemDate;
	CString strContentsData;
	CString strAbstractsData;
	INT		ids;
	
	m_pDM_Species->GetCellData(_T("IB_목차유무"), nIndex, strContentsYN);
	m_pDM_Species->GetCellData(_T("IB_초록유무"), nIndex, strAbstractsYN);

	m_pDM_Species->GetCellData(_T("BS_종KEY"), nIndex, strSpeciesKey);

	strContentsKey = _T("");
	strContentsKey = _T("");
	m_pDM_Species->StartFrame();
	m_pDM_Species->InitDBFieldData() ;
	m_pDM_Species->MakeRecKey(strTocCommonKey);
	
	m_pDM_Species->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strTocCommonKey);
	if ( strContentsYN == _T("Y") ) 
	{
		m_pDM_Species->MakeRecKey(strContentsKey);
		m_pDM_Species->AddDBFieldData(_T("CONTENTS_KEY"),_T("NUMERIC"),strContentsKey);
	}
	if ( strAbstractsYN == _T("Y") ) 
	{
		m_pDM_Species->MakeRecKey(strAbstractsKey);
		m_pDM_Species->AddDBFieldData(_T("ABSTRACTS_KEY"),_T("NUMERIC"),strAbstractsKey);
	}
	m_pDM_Species->AddDBFieldData(_T("SPECIES_KEY"),_T("STRING"),strSpeciesKey);
	m_pDM_Species->AddDBFieldData(_T("SPECIES_CLASS"),_T("STRING"),_T("2"));			
	ids = m_pDM_Species->MakeInsertFrame (_T("BO_TOC_COMMON_TBL")) ; 
	if ( ids < 0 ) return ids;
	
	CTime t = CTime::GetCurrentTime ();
	strSystemDate.Format (_T("%d/%d/%d"),t.GetYear (),t.GetMonth (),t.GetDay ());

	if ( strContentsYN == _T("Y") )
	{
		m_pDM_Species->GetCellData(_T("목차"), nIndex, strContentsData);
		m_pDM_Species->InitDBFieldData();
		m_pDM_Species->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strContentsKey);
		m_pDM_Species->AddDBFieldData(_T("CREATE_DATE"),_T("STRING"), strSystemDate);
		m_pDM_Species->AddDBFieldData (_T("TOC_COMMON_KEY"),_T("NUMERIC"),strTocCommonKey);
		m_pDM_Species->AddDBFieldData(_T("CONTENTS"),_T("STRING"),strContentsData,true);
		ids = m_pDM_Species->MakeInsertFrame(_T("BO_CONTENTS_TBL")) ;		
		if ( ids < 0 ) return ids;
	}
	
	if ( strAbstractsYN  == _T("Y") )
	{
		m_pDM_Species->GetCellData(_T("초록"), nIndex, strAbstractsData);
		m_pDM_Species->InitDBFieldData();		
		m_pDM_Species->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strAbstractsKey);
		m_pDM_Species->AddDBFieldData(_T("CREATE_DATE"),_T("STRING"), strSystemDate);
		m_pDM_Species->AddDBFieldData (_T("TOC_COMMON_KEY"),_T("NUMERIC"),strTocCommonKey);
		m_pDM_Species->AddDBFieldData(_T("ABSTRACTS"),_T("STRING"),strAbstractsData,true);
		ids = m_pDM_Species->MakeInsertFrame(_T("BO_ABSTRACTS_TBL")) ;		
		if ( ids < 0 ) return ids;
	}	
	
	CCONTENTMGR_BO_EDIT TocApi(0,this);	

	m_pDM_Species->InitDBFieldData();		
	m_pDM_Species->AddDBFieldData(_T("REC_KEY"),_T("NUMERIC"),strTocCommonKey);
	if ( strContentsYN == _T("Y") )
	{
		TocApi.MakeIndexKeyWord(strContentsData);
		ConvertHanjaToHangul(strContentsData);		
		m_pDM_Species->AddDBFieldData (_T("IDX_TOC_KEYWORD")		, _T("STRING"), strContentsData);
		m_pDM_Species->AddDBFieldData (_T("TOC_YN"),_T("STRING")	, _T("Y") );
		m_pDM_Species->AddDBFieldData (_T("TOC_USE_LIMIT_CODE")	, _T("STRING"), _T("GM") );
		m_pDM_Species->AddDBFieldData (_T("CONTENTS_KEY")		, _T("NUMERIC"), strContentsKey );
		m_pDM_Species->AddDBFieldData (_T("TOC_CREATE_DATE")		, _T("STRING"), strSystemDate);
	}

	if ( strAbstractsYN  == _T("Y") )
	{
		TocApi.MakeIndexKeyWord(strAbstractsData);
		ConvertHanjaToHangul(strContentsData);		
		m_pDM_Species->AddDBFieldData (_T("IDX_ABS_KEYWORD")		, _T("STRING"), strAbstractsData) ;			
		m_pDM_Species->AddDBFieldData (_T("ABS_YN")				, _T("STRING"), _T("Y"));
		m_pDM_Species->AddDBFieldData (_T("ABS_USE_LIMIT_CODE")	, _T("STRING"), _T("GM") ) ;
		m_pDM_Species->AddDBFieldData (_T("ABSTRACTS_KEY")		, _T("NUMERIC"), strAbstractsKey  );			
		m_pDM_Species->AddDBFieldData (_T("ABS_CREATE_DATE")		, _T("STRING"), strSystemDate) ;   			
	}

	m_pDM_Species->AddDBFieldData (_T("SPECIES_KEY")		, _T("NUMERIC"), strSpeciesKey ) ;
	m_pDM_Species->AddDBFieldData (_T("SPECIES_CLASS")	, _T("STRING"), _T("2") ) ;			
	ids = m_pDM_Species->MakeInsertFrame (_T("IDX_BO_TOC_TBL"));
	if ( ids < 0 ) return ids;

	m_pDM_Species->SendFrame(TRUE);
	m_pDM_Species->EndFrame();	

	return 0;

	EFS_END

	return -1;
}

UINT ThreadFunc_Import(LPVOID pParam)
{
	EFS_BEGIN

	CFileImportDlg *pDlg = (CFileImportDlg*)pParam;
	if (pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}

	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->m_bThreadExecute = TRUE;
	pDlg->ButtonStatus(0);

	CESL_DataMgr *pDM_BO_SPECIES = NULL;
	CESL_ControlMgr *pCM = NULL;
	CESL_Grid *pGrid_OK = NULL;

	CESL_DataMgr *pDM_OK		= NULL;
	CESL_DataMgr *pDM_ERR		= NULL;
	CESL_DataMgr *pDM_SUCCESS	= NULL;
	CESL_DataMgr *pDM_TO		= NULL; 

	INT nChkCnt;
	INT nStopCnt = 0;
	INT ids = -1;
	INT idxDM = -1;
	INT nSelectRowCnt = -1;

	INT nColCnt = -1;
	INT iCount = 0;

	INT nRowSuccess = 0;
	INT nRowsSuccess = 0;

	INT nRemoveRowOk = 0;
	INT nRowSuccessCnt = 0;

	INT nSelectIdx;

	CString strData;
	BOOL bError = FALSE;

	CString strErrorMsg;

	INT nImportType = pDlg->m_cImportType.GetCurSel();
	CString strMode;
	if(pDlg->m_nFileType == 0) strMode = _T("EXCEL");
	else strMode = _T("MARC");

	pGrid_OK = pDlg->m_pGrid_OK;
	
	pDM_OK		= pDlg->m_pDmGridOk;
	pDM_ERR		= pDlg->m_pDmGridError;
	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;

	nColCnt = pDM_OK->GetRefColumnCount();
	
	if ( pDM_OK == NULL || pDM_ERR == NULL || pDM_SUCCESS == NULL )
		goto ERR;
	
	pGrid_OK->SelectMakeList();
	nSelectRowCnt = pGrid_OK->SelectGetCount();
	
	pDlg->m_wndProgressBar.SetRange(0, nSelectRowCnt);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));

	if (nSelectRowCnt <= 0)
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}
	nSelectIdx = pGrid_OK->SelectGetHeadPosition();
	for( nChkCnt = 0 ; nChkCnt < nSelectRowCnt ; nChkCnt++ )
	{
		strErrorMsg.Empty();
		bError = FALSE;
		if (!pDlg->m_bIsThreadRunning)
		{
			nStopCnt = nChkCnt;
			break;
		}

		strData = pDM_OK->GetCellData( _T("순위"), nSelectIdx );
		idxDM = _ttoi(strData) - 1;

		strData = pDM_OK->GetCellData( _T("BS_파일반입오류"), nSelectIdx );
		if( strData.IsEmpty() )
		{
			ids = pDlg->m_pBOSPECIES->BO_SPECIES_FILE_IMPORT(
				strMode, 
				pDlg->m_strImportStatus,
				pDlg->m_pDM_Index,
				pDlg->m_pDM_Species,
				idxDM, 
				pDlg->GetParentDM(), 
				pDlg->GetParentBookDM(),
				pDlg->GetParentGrid(),
				pDlg->GetGroupInfo(), 
				(INT)pDlg->GetAcqType(),
				pDlg->m_bMultiVol);
			if (ids < 0)
			{
				bError = TRUE;
			}

			CString strContentsYN;
			CString strAbstractsYN;

			pDlg->m_pDM_Species->GetCellData(_T("IB_목차유무"), idxDM, strContentsYN);
			pDlg->m_pDM_Species->GetCellData(_T("IB_초록유무"), idxDM, strAbstractsYN);
			if ( strContentsYN == _T("Y") || strAbstractsYN == _T("Y") )
			{
				ids = pDlg->MakeTOC(idxDM);
				if ( ids ) 
				{
					bError = TRUE;		
				}
			}
			
			if( bError == TRUE )
			{
				strErrorMsg = pDlg->GetLastError();

				if( strErrorMsg.IsEmpty() )
				{
					pDM_OK->SetCellData(_T("BS_파일반입오류"), strErrorMsg, nSelectIdx);				
				}
				else
				{				
					pDM_OK->SetCellData(_T("BS_파일반입오류"), _T("마크반입 에러"), nSelectIdx);
				}		
			}
		}

		pDlg->m_wndProgressBar.StepIt();
		nSelectIdx = pGrid_OK->SelectGetNext();
	}

	nSelectIdx = pGrid_OK->SelectGetTailPosition();
	if ( !pDlg->m_bIsThreadRunning ) 
	{
		for( INT i = 0 ; i < nSelectRowCnt - nStopCnt ; i++ )
		{
			nSelectIdx = pGrid_OK->SelectGetPrev();
		}

		nSelectRowCnt = nStopCnt;
	}

	for( nChkCnt = 0 ; nChkCnt < nSelectRowCnt ; ++nChkCnt )
	{
		INT nRowsTo;

		strData = pDM_OK->GetCellData( _T("BS_파일반입오류"), nSelectIdx);

		if( strData.IsEmpty() )
		{
			pDM_TO = pDM_SUCCESS;
		}
		else
		{
			pDM_TO = pDM_ERR;
		}

		nRowsTo = pDM_TO->GetRowCount();
		pDM_TO->InsertRow(-1);
		
		for (INT col = 0; col < nColCnt; col++)
 		{	
			strData = pDM_OK->GetCellData( nSelectIdx ,col);
 			pDM_TO->SetCellData( nRowsTo ,col,  strData);

 		}

		pDM_OK->DeleteRow( nSelectIdx);
		nSelectIdx = pGrid_OK->SelectGetPrev();
	}

	INT nRowCnt;
	nRowCnt = pDM_SUCCESS->GetRowCount();
	strData.Format(_T("성공 : %d"),nRowCnt );
	pDlg->m_wndStatusBarCtrl.SetText(strData, 3, 0);
	
	nRowCnt = pDM_ERR->GetRowCount();
	strData.Format(_T("오류 : %d"),nRowCnt );
	pDlg->m_wndStatusBarCtrl.SetText(strData, 2, 0);
	
	nRowCnt = pDM_OK->GetRowCount();
	strData.Format(_T("정상 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strData, 1, 0);

	pDlg->m_wndProgressBar.SetWindowText(_T("반입하였습니다."));
	AfxMessageBox(_T("반입하였습니다."));
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	SetEvent(pDlg->m_hThread);

	::PostMessage(pDlg->GetSafeHwnd(), WM_SORT_GRID, 0, 0);

	return 0;

ERR:
	pDlg->m_wndProgressBar.SetWindowText(_T("반입 실패"));
	AfxMessageBox(_T("반입 실패"));
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	::PostMessage(pDlg->GetSafeHwnd(), WM_SORT_GRID, 0, 0);
	return -1;

	EFS_END
	return -1;
}

UINT ThreadFunc_Display(LPVOID pParam)
{
	EFS_BEGIN	
	
	CFileImportDlg *pDlg = (CFileImportDlg*)pParam;
	if (pDlg == NULL)
	{
		pDlg->m_bIsThreadRunning = FALSE;
		return -1;
	}
	
	pDlg->m_bIsThreadRunning = TRUE;
	pDlg->ButtonStatus(0);
	
	CESL_DataMgr *pDM_Species = NULL;
	CESL_ControlMgr *pCM = NULL;

	CESL_DataMgr *pDM_OK		= NULL;
	CESL_DataMgr *pDM_ERR		= NULL;
	CESL_DataMgr *pDM_SUCCESS	= NULL;
	CESL_DataMgr *pDM_TEMP		= NULL;

	CESL_Grid *pGrid_OK			= NULL;
	
	CString strAlias;
	CStringArray astrAlias;
	CString strFieldName;
	CString strFieldType;
	CString strDataType;
	CString strData;

	INT i = 0;
	INT nTotalCount = -1;
	INT row = 0;
	INT nColCnt = 0;
	INT nDmRowIdx = 0;

	pDM_Species = pDlg->m_pDM_Species;
	if (pDM_Species == NULL)
	{
		goto ERR;
	}
	
	nTotalCount = pDM_Species->GetRowCount();
	
	pCM = pDlg->FindCM(_T("CM_BO_FILE_IMPORT"));
	if (pCM == NULL)
	{
		goto ERR;
	}

	pDlg->m_wndProgressBar.SetRange(0, nTotalCount);
	pDlg->m_wndProgressBar.SetPos(0);
	pDlg->m_wndProgressBar.SetWindowText(_T(""));

	strData.Format(_T("전체 : %d"), nTotalCount);
	pDlg->m_wndStatusBarCtrl.SetText(strData, 0, 0);
		

	pDM_OK		= pDlg->m_pDmGridOk;
	pDM_ERR		= pDlg->m_pDmGridError;
	pDM_SUCCESS	= pDlg->m_pDmGridSuccess;

	if (pDM_OK == NULL || pDM_ERR == NULL || pDM_SUCCESS == NULL)
	{
		goto ERR;
	}

	pDM_OK->FreeData();
	pDM_ERR->FreeData();
	pDM_SUCCESS->FreeData();

	nColCnt = pDM_OK->GetRefColumnCount();
	for( i = 0 ; i < nColCnt ; i++ )
	{	
		pDM_OK->FindColumnFieldName( i , strAlias , strFieldName ,strFieldType,strDataType,0,strData);
		astrAlias.Add(strAlias);
	}
	
	for (row = 0; row < nTotalCount; row++)
	{
		CString sBookCnt;

		pDlg->m_wndProgressBar.StepIt();

		if (pDM_Species->GetCellData( _T("BS_파일반입오류"), row).IsEmpty())
		{
			pDM_TEMP = pDM_OK;
		}
		else
		{
			pDM_TEMP = pDM_ERR;
		}
		nDmRowIdx = pDM_TEMP->GetRowCount();
		pDM_TEMP->InsertRow(-1);
		
		if (!pDlg->m_bIsThreadRunning) 
		{
			break;
		}
	
		for( i = 0 ; i < nColCnt ; i++ )
		{
			strAlias = astrAlias.GetAt(i);
			if( _T("선정") == strAlias ) 
			{
				continue;
			}
			if( _T("순위") == strAlias )
			{
				strData.Format(_T("%d"), row+1);
			}
			else if( _T("BS_파일반입책수") == strAlias )
			{
				CButton * pChkMultiBook = (CButton *)pDlg->GetDlgItem(IDC_chkAPPLY_MULTI_VOL);
				if( pChkMultiBook->GetCheck() == 1)
				{
					strData = pDM_Species->GetCellData(_T("BS_책수"),row);
				}
				else
				{
					strData = _T("1");
				}
			}
			else if( _T("BS_파일반입오류") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_파일반입오류"),row);
			}
			else if( _T("IB_목차유무") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("IB_목차유무"),row);
			}
			else if( _T("IB_초록유무") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("IB_초록유무"),row);
			}
			else if( _T("낱권ISBN") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_MARC.020$a"),row);
			}
			else if( _T("본표제") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_MARC.서명"),row);
			}
			else if( _T("편권차") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_MARC.245$n"),row);
			}
			else if( _T("가격") == strAlias )
			{
				strData = pDM_Species->GetCellData(_T("BS_MARC.950$b"),row);
			}
			else
			{
				pDM_Species->GetCellData(("BS_MARC.") + strAlias, row, strData);
			}
			pDM_TEMP->SetCellData( nDmRowIdx, i, strData);
		}
	}
	
	INT nRowCnt;
	pDlg->m_wndStatusBarCtrl.SetText(_T("성공 : 0"), 3, 0);

	nRowCnt = pDM_ERR->GetRowCount();
	strData.Format(_T("오류 : %d"),nRowCnt );
	pDlg->m_wndStatusBarCtrl.SetText(strData, 2, 0);

	nRowCnt = pDM_OK->GetRowCount();
	strData.Format(_T("정상 : %d"), nRowCnt);
	pDlg->m_wndStatusBarCtrl.SetText(strData, 1, 0);

	pDlg->m_wndProgressBar.SetWindowText(_T("파일읽기 완료"));
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);

	SetEvent(pDlg->m_hThread);

	::PostMessage(pDlg->GetSafeHwnd(), WM_GRID_DISPLAY, 0, 0);
	return 0;
ERR:
	pDlg->m_bIsThreadRunning = FALSE;
	pDlg->ButtonStatus(1);
	return -1;
	EFS_END
	return -1;
}

INT CFileImportDlg::SetBeginImportStatus(CString strGroup, CString strStatus)
{
	EFS_BEGIN

	m_strImportStatusGroup = strGroup;
	m_strImportStatus = strStatus;

	return 0;

	EFS_END
	return -1;

}

INT CFileImportDlg::FindImportStatus(CString strStatusName, CString &strStatus)
{
	EFS_BEGIN

	strStatus = _T("");

	for (INT i = 0; i < m_nImportStatusArray; i++) {
		if (strStatusName == m_strImportStatusArray[i][1]) {
			strStatus = m_strImportStatusArray[i][2];
			return 0;
		}
	}
	return -1;

	EFS_END
	return -1;

}

VOID CFileImportDlg::SetCtrlByImportType()
{
	CString strImportType, strImportStatus, strTmpData;
	m_cImportStatus.GetWindowText(strImportStatus);
	m_cImportType.GetWindowText(strImportType);
	CButton *pBtn1 = (CButton*)GetDlgItem(IDC_strFIMPGROUP_INFO);
	CButton *pBtn2 = (CButton*)GetDlgItem(IDC_stcFIMPWORK_NO);
	CButton *pBtn3 = (CButton*)GetDlgItem(IDC_btnFIMPGROUP_CREATE);
	CComboBox *pCmb = (CComboBox*)GetDlgItem(IDC_cmbFIMPWORK_NO);
	CComboBox *pCmb1 = (CComboBox*)GetDlgItem(IDC_cmbFIMPGROUP_INFO);

	if(strImportType == _T("적용안함")) m_nAcqType = ACQ_TYPE_INVALID;
	else if(strImportType == _T("구입")) m_nAcqType = PURCHASE;
	else if(strImportType == _T("기증") || strImportType == _T("수증") ) m_nAcqType = DONATE;
	else m_nAcqType = OTHER_ACQ_TYPE;
	
	SetGroupCollection(m_nAcqType);

	if( (strImportStatus == _T("기초자료") || strImportStatus == _T("반입자료") )&& strImportType == _T("적용안함"))
	{
		pBtn1->SetWindowText(_T("그룹이름"));
		pBtn2->ShowWindow(SW_HIDE);
		pCmb->ShowWindow(SW_HIDE);
		pBtn3->SetWindowText(_T("그룹생성"));
		CStringArray tmpArray;
		m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
	}
	else if(strImportStatus != _T("반입자료") && strImportType == _T("적용안함"))
	{
		pBtn1->SetWindowText(_T("그룹이름"));
		pBtn2->ShowWindow(SW_HIDE);
		pCmb->ShowWindow(SW_HIDE);
		pCmb1->EnableWindow(FALSE);
		pBtn3->SetWindowText(_T("그룹생성"));
		pBtn3->EnableWindow(FALSE);
		m_pGroup = NULL;
	}
	else if(strImportType == _T("구입"))
	{
		pBtn1->SetWindowText(_T("차수번호"));
		pBtn2->ShowWindow(SW_SHOW);
		pBtn2->SetWindowText(_T("작업번호"));
		pCmb->ShowWindow(SW_SHOW);
		pCmb1->EnableWindow(TRUE);
		pBtn2->EnableWindow(TRUE);
		pBtn3->SetWindowText(_T("차수생성"));		
		pBtn3->EnableWindow(TRUE);
		CStringArray tmpArray;
		m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
		if(m_pGroup != NULL)
			m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), m_pGroup->GetArrayWorkNo(), _T("작업번호"));
	}
	else if(strImportType == _T("기증"))
	{
		pBtn1->SetWindowText(_T("접수번호"));
		pBtn2->ShowWindow(SW_HIDE);
		pCmb->ShowWindow(SW_HIDE);
		pCmb1->EnableWindow(TRUE);
		pBtn3->EnableWindow(TRUE);
		pBtn3->SetWindowText(_T("접수번호생성"));
		CStringArray tmpArray;
		m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
	}
	else
	{
		pBtn1->SetWindowText(_T("그룹번호"));
		pBtn2->ShowWindow(SW_HIDE);
		pCmb->ShowWindow(SW_HIDE);
		pCmb1->EnableWindow(TRUE);
		pBtn3->EnableWindow(TRUE);
		pBtn3->SetWindowText(_T("그룹번호생성"));
		CStringArray tmpArray;
		m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
	}
}

VOID CFileImportDlg::OnSelchangecmbIMPORTTYPE() 
{
	// TODO: Add your control notification handler code here
	SetCtrlByImportType();
}

VOID CFileImportDlg::SetGroupCollection(ACQ_TYPE nAcqType)
{
	if(m_pParentGroupCollection)
		m_pParentGroupCollection->CopyData(m_pGroupCollection);

	if(m_strAcqYear.IsEmpty()) m_strAcqYear = m_pBOSPECIES->GetTodayYear();
	CString strWorkCode, strAcqCode;
	m_pCM->SetControlMgrData(_T("수입년도"), m_strAcqYear);
	m_pCM_ClassCode->GetControlMgrData(_T("IBS_업무구분_코드"), strWorkCode);
	m_cImportType.GetWindowText(strAcqCode);
	m_pInfo->pCodeMgr->ConvertDescToCode(_T("입수구분"), strAcqCode, strAcqCode);

	CString strImportStatus;
	m_cImportStatus.GetWindowText(strImportStatus);

	ACQ_TYPE nTmpType = m_pGroupCollection->GetAcqType();
	if(nAcqType != nTmpType || m_pGroupCollection->GetAcqCode() != strAcqCode)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = new CGroupCollection(this);
		m_pGroupCollection->SetAcqType(nAcqType);
		m_pGroupCollection->SetAcqCode(strAcqCode);
		if(_T("구입자료") == strImportStatus)
			m_pGroupCollection->SetWorkingStatus(_T("구입대상"));
		else if(_T("검수자료") == strImportStatus)
			m_pGroupCollection->SetWorkingStatus(_T("주문"));
		else
			m_pGroupCollection->SetWorkingStatus(_T("검수완료"));
		m_pGroupCollection->MakeGroupInfo(m_strAcqYear, m_strGruopInfo, strWorkCode);
	}
	else if(nAcqType == PURCHASE && m_pGroupCollection->GetWorkCode() != strWorkCode)
	{
		delete m_pGroupCollection;
		m_pGroupCollection = new CGroupCollection(this);
		m_pGroupCollection->SetAcqType(nAcqType);
		if(_T("구입자료") == strImportStatus)
			m_pGroupCollection->SetWorkingStatus(_T("구입대상"));
		else if(_T("검수자료") == strImportStatus)
			m_pGroupCollection->SetWorkingStatus(_T("주문"));
		else
			m_pGroupCollection->SetWorkingStatus(_T("검수완료"));
		m_pGroupCollection->MakeGroupInfo(m_strAcqYear, m_strGruopInfo, strWorkCode);
	}
	else if(m_pGroupCollection->GetGroupCount() == 0)
		m_pGroupCollection->MakeGroupInfo(m_strAcqYear, m_strGruopInfo, strWorkCode);
	
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
}

VOID CFileImportDlg::OnSelchangecmbFIMPGROUPINFO() 
{
	// TODO: Add your control notification handler code here
	CString strGroupName, strLastNo, strImportType;
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("그룹정보"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSel(nCurSel);
	m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
	
	m_cImportType.GetWindowText(strImportType);
	if(strImportType == _T("구입"))
	{
		CStringArray * pArray = m_pGroup->GetArrayWorkNo();
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, m_pGroup->GetWorkNo(-1), pArray, _T("작업번호"));		
	}
}

VOID CFileImportDlg::OnSelchangecmbFIMPWORKNO() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlComboBox * pCmb = (CESL_ControlComboBox*)m_pCM->FindControl(_T("작업번호"));
	INT nCurSel = pCmb->GetCurSel();
	m_pGroupCollection->SetCurSelWorkNo(nCurSel);	
}

VOID CFileImportDlg::OnbtnFIMPGROUPCREATE() 
{
	// TODO: Add your control notification handler code here
	CString strAddGroupNo, strAddWorkNo, strAddLastNo, strAddGroupName, strImportType, strWorkCode;
	if(m_strAcqYear.IsEmpty()) m_strAcqYear = m_pBOSPECIES->GetTodayYear();
	m_cImportType.GetWindowText(strImportType);

	CString strImportStatus;
	m_cImportStatus.GetWindowText(strImportStatus);

	BOOL bIDOK_Flag = FALSE;

	if(m_nAcqType == PURCHASE)
	{
		m_pCM_ClassCode->GetControlMgrData(_T("IBS_업무구분_코드"), strWorkCode)	;
		CDivisionNo_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqYear(m_strAcqYear);
		dlg.SetWorkCode(strWorkCode);
		dlg.SetCONNECTION_INFO(m_pDM_Index->CONNECTION_INFO);

		if(_T("구입자료") == strImportStatus)   dlg.SetWorkStatus(_T("구입대상"));
		else if(_T("검수자료") == strImportStatus)  dlg.SetWorkStatus(_T("주문"));
		else  dlg.SetWorkStatus(_T("검수완료"));

		if(IDOK == dlg.DoModal())
		{
			bIDOK_Flag = TRUE;
			dlg.GetInsertInfo(strAddGroupNo, strAddWorkNo, strAddLastNo, strWorkCode);
			m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupNo, strWorkCode);
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			m_pGroup->SetLastSerialNo(_ttoi(strAddLastNo.GetBuffer(0)));
			CStringArray tmpArray;
			
			INT ids;
			CString strTmpData;
			ids = m_pGroupCollection->MakeGroupInfo(m_strAcqYear, m_strGruopInfo, strWorkCode);
			if(ids < 0) return;
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
			m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
			if(m_pGroupCollection->GetAcqCode() == _T("1"))
			{
				if(m_pGroup != NULL)
					m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), m_pGroup->GetArrayWorkNo(), _T("작업번호"));
			}
		}
	}
	else if(m_nAcqType == DONATE)
	{
		CRequisitionNo_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqYear(m_strAcqYear);
		if(IDOK == dlg.DoModal())
		{
			bIDOK_Flag = TRUE;
			dlg.GetInsertInfo(strAddGroupNo);
			m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupNo);
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			CStringArray tmpArray;

			INT ids;
			CString strTmpData;
			ids = m_pGroupCollection->MakeGroupInfo(m_strAcqYear, m_strGruopInfo, strWorkCode);
			if(ids < 0) return;
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
			m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
		}
	}	
	else if(m_nAcqType == ACQ_TYPE_INVALID)
	{
		CGroup_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqYear(m_strAcqYear);
		if(IDOK == dlg.DoModal())
		{
			bIDOK_Flag = TRUE;
			dlg.GetInsertInfo(strAddGroupNo, strAddGroupName, strAddLastNo);
			m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupName, strAddGroupNo);
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			m_pGroup->SetLastSerialNo(_ttoi(strAddLastNo.GetBuffer(0)));
			m_pGroup->SetHaveLastSerialNo(TRUE);
			CStringArray Array;
						
			INT ids;
			CString strTmpData;
			ids = m_pGroupCollection->MakeGroupInfo(m_strAcqYear, m_strGruopInfo, strWorkCode);
			if(ids < 0) return;
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			m_pGroupCollection->GetGroupInfo(&Array, strTmpData);
			m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &Array, _T("그룹정보"));			
		}
	}
	else if(m_nAcqType == OTHER_ACQ_TYPE)
	{
		CString strAcqCode;
		m_cImportType.GetWindowText(strAcqCode);
		m_pInfo->pCodeMgr->ConvertDescToCode(_T("입수구분"), strAcqCode, strAcqCode);
		COtherGroupNo_Insert_Dlg dlg(this);
		dlg.SetOpenMode(DINSERT);
		dlg.SetAcqYear(m_strAcqYear);
		dlg.SetAcqCode(strAcqCode);
		if(IDOK == dlg.DoModal())
		{
			bIDOK_Flag = TRUE;
			dlg.GetInsertInfo(strAddGroupNo, strAcqCode);
			m_pGroupCollection->AddGroupInfo(m_strAcqYear, strAddGroupNo, strAcqCode);
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			CStringArray tmpArray;

			INT ids;
			CString strTmpData;
			ids = m_pGroupCollection->MakeGroupInfo(m_strAcqYear, m_strGruopInfo, strWorkCode);
			if(ids < 0) return;
			m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
			m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
			m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
		}		
	}

	m_pCM->ControlDisplay(_T("그룹정보"));

}

VOID CFileImportDlg::SetGroupCollection(CGroupCollection * pTmp)
{
	m_pParentGroupCollection = pTmp;
}

VOID CFileImportDlg::OnSelchangecmbWORKCODE() 
{
	// TODO: Add your control notification handler code here
	CString strImportType;
	m_cImportType.GetWindowText(strImportType);

	if(strImportType != _T("구입")) return ;
	SetCtrlByImportType();
}

VOID CFileImportDlg::OnSelchangecmbCLASIFYCLASS() 
{
	// TODO: Add your control notification handler code here
	CESL_ControlMultiComboBox *pCM_WhichCode = (CESL_ControlMultiComboBox *)m_pCM_ClassCode->FindControl(_T("IBS_분류표구분"));
	if (!pCM_WhichCode) return;
    CESL_ControlMultiComboBox *pCM_Subject = (CESL_ControlMultiComboBox *)m_pCM_ClassCode->FindControl(_T("BS_주제구분_코드"));
	if (!pCM_Subject) return;
  
	CString class_name;
	CString strWhichCode;
	pCM_WhichCode->GetData(strWhichCode, pCM_WhichCode->GetCurSel(), 1);
	strWhichCode.TrimLeft();		strWhichCode.TrimRight();
	strWhichCode.MakeUpper();
	class_name = _T("");
	if (strWhichCode == _T("KDC")) class_name = _T("KDC대주제부호관리");
	if (strWhichCode == _T("DDC")) class_name = _T("DDC대주제부호관리");
	if (strWhichCode == _T("LC"))  class_name = _T("LC주제부호");

	pCM_Subject->FreeComboData();
	pCM_Subject->ResetContent();
	pCM_Subject->InitMultiComboBox(2);
	if (class_name.GetLength() == 0) return ;

	INT nCode = 256;
	INT nDefaultValue = 0;
	CString strCode[256], strDesc[256];
	nDefaultValue = 0;
	m_pInfo->pCodeMgr->GetCode(class_name, strCode, strDesc, nCode, nDefaultValue);	
	if (nCode == 0) return ;

	CString strDefaultValue;
	if(m_pCM_ClassCode != NULL) m_pDM_Species->GetCellData(_T("BS_주제구분_코드"), 0, strDefaultValue);
		
	for (INT i = 0; i < nCode; i++) {
		if (strCode[i].GetLength() == 0) strCode[i] = _T("  ");
		
		if (strDefaultValue == strCode[i]) nDefaultValue = i;
	
		pCM_Subject->AddItem(strCode[i], 0, i);
		pCM_Subject->AddItem(strDesc[i], 1, i);
	}

	pCM_Subject->SetCurSel(nDefaultValue);		
}

VOID CFileImportDlg::OnSelchangecmbFORMCLASS() 
{
	// TODO: Add your control notification handler code here
	CheckFormCode();	
}

INT CFileImportDlg::CheckFormCode()
{
	CString strFormCode;
	m_pCM_ClassCode->GetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
	if(strFormCode.IsEmpty())
	{
		ESLAfxMessageBox(_T("형식구분은 필수 설정사항입니다. 올바른 정보를 설정해 주십시요."));
		m_pInfo->pCodeMgr->GetDefaultCode(_T("형식구분"), strFormCode);
		m_pCM_ClassCode->SetControlMgrData(_T("IBS_형식구분_코드"), strFormCode);
		return -1;
	}
	m_pCM_ClassCode->GetControlMgrData(_T("IBS_등록구분_코드"), strFormCode);
	if(strFormCode.IsEmpty())
	{
		ESLAfxMessageBox(_T("등록구분은 필수 설정사항입니다. 올바른 정보를 설정해 주십시요."));
		m_pInfo->pCodeMgr->GetDefaultCode(_T("등록구분"), strFormCode);
		m_pCM_ClassCode->SetControlMgrData(_T("IBS_등록구분_코드"), strFormCode);
		return -1;
	}
	return 0;
}

VOID CFileImportDlg::SetImportTypeArray()
{
	m_nImportTypeArray = 0;
	CStringArray ArrAcqCodeDesc;

	CString codes[6];
	CString code, desc;
	INT startrow = 0;
	INT k=0;
	
	INT codeCnt = m_pInfo->pCodeMgr->GetCodeCount(_T("입수구분"));
	for(INT i=0 ; i<codeCnt ; i++)
	{
		startrow = m_pInfo->pCodeMgr->GetCode(_T("입수구분"), codes, 6, startrow);
		code = codes[2];
		desc = codes[3];
		
		code.TrimLeft(); code.TrimRight();
		desc.TrimLeft(); desc.TrimRight();
		
		startrow++;
		
		ArrAcqCodeDesc.Add(desc);
	}
	
	for(i=0 ; i<ArrAcqCodeDesc.GetSize() ; i++)
	{
		m_strImportTypeArray[m_nImportTypeArray][0] = _T("정리");	m_strImportTypeArray[m_nImportTypeArray][1] = ArrAcqCodeDesc.GetAt(i);		m_nImportTypeArray++;
		m_strImportTypeArray[m_nImportTypeArray][0] = _T("등록");	m_strImportTypeArray[m_nImportTypeArray][1] = ArrAcqCodeDesc.GetAt(i);		m_nImportTypeArray++;
	}
	m_strImportTypeArray[m_nImportTypeArray][0] = _T("수서");	m_strImportTypeArray[m_nImportTypeArray][1] = _T("적용안함");		m_nImportTypeArray++;
	m_strImportTypeArray[m_nImportTypeArray][0] = _T("수서기초");	m_strImportTypeArray[m_nImportTypeArray][1] = _T("적용안함");		m_nImportTypeArray++;
	m_strImportTypeArray[m_nImportTypeArray][0] = _T("수서구입");	m_strImportTypeArray[m_nImportTypeArray][1] = _T("구입");	m_nImportTypeArray++;
	m_strImportTypeArray[m_nImportTypeArray][0] = _T("수서검수");	m_strImportTypeArray[m_nImportTypeArray][1] = _T("구입");	m_nImportTypeArray++;
	m_strImportTypeArray[m_nImportTypeArray][0] = _T("기증접수");	m_strImportTypeArray[m_nImportTypeArray][1] = _T("기증");	m_nImportTypeArray++;
}

VOID CFileImportDlg::OnKillfocusedtFIMPACQYEAR() 
{
	// TODO: Add your control notification handler code here
	INT ids;
	m_pCM->GetControlMgrData(_T("수입년도"), m_strAcqYear);
	if(m_pGroupCollection && m_pGroupCollection->GetAcqYear() != m_strAcqYear)
	{
		CStringArray tmpArray;
		CString strTmpData, strWorkCode;
		m_pCM_ClassCode->GetControlMgrData(_T("IBS_업무구분_코드"), strWorkCode);
		ids = m_pGroupCollection->MakeGroupInfo(m_strAcqYear, m_strGruopInfo, strWorkCode);
		if(ids < 0) return;
		m_pGroup = m_pGroupCollection->GetCurSelGroupInfo();
		m_pGroupCollection->GetGroupInfo(&tmpArray, strTmpData);
		m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), &tmpArray, _T("그룹정보"));
		if(m_pGroupCollection->GetAcqCode() == _T("1"))
		{
			if(m_pGroup != NULL)
				m_pComboMgr->SetSingleComboBoxStrData(m_pCM->CM_ALIAS, _T(""), m_pGroup->GetArrayWorkNo(), _T("작업번호"));
		}
	}
}

INT CFileImportDlg::ConvertHanjaToHangul(CString &strSrc)
{
	EFS_BEGIN	

	if ( strSrc == "" ) return 0;
	
	CHAR		*UTF8String;
	_TCHAR		*OutUTF16;
	INT			ids;
	INT			nLen;
	
	ids = m_cvt.UnicodeToUTF8(strSrc.GetBuffer(0) , &UTF8String, &nLen);
	ids = m_cvt.Hanja2Hangul(UTF8String);
	ids = m_cvt.UTF8ToUnicode(UTF8String, &OutUTF16, &nLen);
	
	strSrc.Format(_T("%s"), OutUTF16 );
	
	free(UTF8String);
	free(OutUTF16);

	
	return 0;

	EFS_END
	return -1;

}

void CFileImportDlg::OnrEXCEL() 
{
	m_nFileType = 0;	
}


void CFileImportDlg::OnrTXT() 
{
	m_nFileType = 1;	
}

void CFileImportDlg::OnrTXTFILE() 
{
	m_nFileType = 2;
	
}

void CFileImportDlg::OnrMODSFILE() 
{
	m_nFileType = 3;
	
}

void CFileImportDlg::OnrCSV() 
{
	m_nFileType = 5;
	
}

HBRUSH CFileImportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

INT CFileImportDlg::DisplayGrid()
{
	m_pGrid_OK->Display();
	m_pGrid_ERR->Display();	
	m_pGrid_SUCCESS->Display();	

	SetGridRank();
	
	return 0 ;
}

INT CFileImportDlg::SetGridRank()
{
	INT nRowCnt;
	INT i;
	CString strData;

 	nRowCnt = m_pDmGridOk->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridOk->GetCellData(_T("순위"),i);
		m_pGrid_OK->SetTextMatrix( i+1, 0, strData);
	}

	nRowCnt = m_pDmGridError->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridError->GetCellData(_T("순위"),i);
		m_pGrid_ERR->SetTextMatrix( i+1, 0, strData);
	}

	nRowCnt = m_pDmGridSuccess->GetRowCount();

	for( i = 0 ; i < nRowCnt ; i++ )
	{
		strData = m_pDmGridSuccess->GetCellData(_T("순위"),i);
		m_pGrid_SUCCESS->SetTextMatrix( i+1, 0, strData);
	}

	return 0 ;
}

INT CFileImportDlg::SortGrid()
{	
	SortDm( m_pDmGridOk );
	SortDm( m_pDmGridError );
	SortDm( m_pDmGridSuccess );
	
	return 0 ;
}

INT CFileImportDlg::SortDm( CESL_DataMgr * pSortDm )
{
	BOOL bChkSwap;

	INT nRowCnt;
	INT i;

	CString strData1;
	CString strData2;
	
	nRowCnt = pSortDm->GetRowCount();
	while(1)
	{
		bChkSwap = FALSE;
		for( i = 0 ; i < nRowCnt-1 ; i++ )
		{
			strData1 = pSortDm->GetCellData( _T("순위"), i );		
			strData2 = pSortDm->GetCellData( _T("순위"), i+1 );
			if( _ttoi(strData1) > _ttoi(strData2) )
			{
				pSortDm->SORT_SwapRow( i , i + 1 );
				bChkSwap = TRUE;
			}
		}

		if( bChkSwap == FALSE )
		{
			break;
		}
	}
	
	return 0 ;
}

INT CFileImportDlg::MsgSortGrid()
{
	SortGrid();
	DisplayGrid();

	return 0 ;
}

INT CFileImportDlg::MsgDisplayGrid()
{
	DisplayGrid();
	
	return 0 ;
}

