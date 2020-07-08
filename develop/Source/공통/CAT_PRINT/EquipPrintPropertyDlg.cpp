// EquipPrintPropertyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EquipPrintPropertyDlg.h"
#include "BO_CAT_SERIAL_LABEL_CONFIG.h"
#include "BO_SERIAL_NORMAL_CONFIG.h"
#include "SerialLabelPreview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <iostream.h>
#include <iomanip.h>

#include <Winspool.h>
#include <Windows.h>
#include <malloc.h>
#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>

#define MAXBUFFERSIZE 250
#define strError _T("GetPrintName Error")
LPTSTR GetPrnName();              //Get Printer Name
LPTSTR szPrinterName = NULL;

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyDlg dialog
CEquipPrintPropertyDlg::CEquipPrintPropertyDlg(CESL_Mgr* pParent /*=NULL*/ , INT nModeFlag /*=0*/ )
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CEquipPrintPropertyDlg)
	m_nOption_Paper = 0;
	m_nOption_Print = 0;
	m_nOption_PaperDir = 1;
	m_strRegNoStart = _T("");
	m_strRegNoEnd = _T("");
	//}}AFX_DATA_INIT

	m_pDM_Parent = NULL;
	m_pGrid_Parent = NULL;

	m_pSIReportManager = NULL;
	m_pCM = NULL;

	m_nMaxLine = 13;

	m_bBarcodeUserno = nModeFlag;

	if ( 1 == m_bBarcodeUserno ) 
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD_SEC );
		m_nIDHelp = IDD_SEC;
		m_lpszFileName = _T("..\\CFG\\대출자번호_장비_출력.cfg");
	}
	else if(2 == m_bBarcodeUserno )
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD_LIB );
		m_nIDHelp = IDD_LIB;
		m_lpszFileName = _T("..\\CFG\\기관부호_장비_출력.cfg");
	}
	else
	{
		m_lpszTemplateName = MAKEINTRESOURCE( IDD );
		m_nIDHelp = IDD;
		m_lpszFileName = _T("..\\CFG\\정리_장비_출력.cfg");
	}
	
	
}


CEquipPrintPropertyDlg::~CEquipPrintPropertyDlg()
{
}

VOID CEquipPrintPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEquipPrintPropertyDlg)
	DDX_Radio(pDX, IDC_rPAPER_SHEET, m_nOption_Paper);
	DDX_Radio(pDX, IDC_rPRINT_LINE, m_nOption_Print);
	DDX_Radio(pDX, IDC_rPAPER_WIDTH, m_nOption_PaperDir);
	DDX_Text(pDX, IDC_eREG_NO_START_CP, m_strRegNoStart);
	DDV_MaxChars(pDX, m_strRegNoStart, 10);
	DDX_Text(pDX, IDC_eREG_NO_END_CP, m_strRegNoEnd);
	DDV_MaxChars(pDX, m_strRegNoEnd, 10);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CEquipPrintPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CEquipPrintPropertyDlg)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP, OnbPRINTEQUIP)
	ON_BN_CLICKED(IDC_bSAVE_CONFIG, OnbSAVECONFIG)
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_rPRINT_LINE, IDC_rPRINT_BARCODE2, OnPrintFormatChange )
	ON_BN_CLICKED(IDC_bFONT, OnbFONT)
	ON_CBN_SELCHANGE(IDC_cREG_NO_PLACE, OnSelchangecREGNOPLACE)
	ON_CBN_SELENDOK(IDC_CobUnit, OnSelendokCobUnit)
	ON_BN_CLICKED(IDC_BtnSerialLabelConfig, OnBtnSerialLabelConfig)
	ON_BN_CLICKED(IDC_CHK_SERIAL, OnChkSerial)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyDlg Operations

INT CEquipPrintPropertyDlg::Init()
{
	EFS_BEGIN

	if(2 != m_bBarcodeUserno)
	{
		if (!m_pDM_Parent || !m_pGrid_Parent) return -1;

		m_pGrid_Parent->SelectMakeList();
		if (m_pGrid_Parent->SelectGetCount() <= 0)
		{
			m_strLastErrMsg = _T("선정된 정보가 없습니다.");
			return -1;
		}
	}

	if (m_pSIReportManager)
		delete m_pSIReportManager;
	m_pSIReportManager = new CSIReportManager(this);

	FreePrintPropertyInfo();

	m_pCM = FindCM(_T("CM_정리_출력_장비_속성"));
	if (!m_pCM) return -1;

	INT ids = -1;

	ids = LoadPrintPropertyInfo();
	if (ids < 0) return ids;
	ids = LoadSelectInfo();

	CString strLibName;
	ids = GetLibInfo(&CString(_T("LIB_NAME")), &strLibName, 1);

	m_pCM->SetControlMgrData(_T("도서관명"), strLibName);

	OnPrintFormatChange();

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::GetLibInfo(CString ArrFieldAttr[], 
						CString ArrResult[], 
						INT nFieldCount)
{
	if (!ArrFieldAttr || !ArrResult || nFieldCount < 1) return -1;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(this->m_pInfo->CONNECTION_INFO);

	CString strFieldAttr;
	for (INT idx = 0; idx < nFieldCount; idx++)
	{
		if (ArrFieldAttr[idx].IsEmpty()) continue;

		strFieldAttr += ArrFieldAttr[idx];
		if (idx != nFieldCount - 1)
			strFieldAttr += _T(", ");
	}

	CString strQuery;
	strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"), strFieldAttr );
	INT ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
	if (ids < 0) return -1;

	return 0;
}

LPTSTR GetPrnName()
{
	OSVERSIONINFO osv;
    TCHAR cBuffer[MAXBUFFERSIZE];
	PRINTER_INFO_2 *lpInfo = NULL;
    DWORD dwSizeNeeded;
	DWORD dwNumItems;
	DWORD dwItem;
	LPTSTR _strResult;

	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	
	if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
	    EnumPrinters( PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &dwSizeNeeded, &dwNumItems );
			
		if (dwSizeNeeded == 0)
		{
			return strError;
		}
		
		lpInfo = (PRINTER_INFO_2 *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSizeNeeded);
		
		if (lpInfo == NULL)
		{
			return strError;
		}
		
		if (EnumPrinters( PRINTER_ENUM_LOCAL,	NULL, 2, (LPBYTE)lpInfo, dwSizeNeeded, &dwSizeNeeded, &dwNumItems) == 0 )
		{
		    return strError;
		}
		
		// display printers 
		for ( dwItem = 0; dwItem < dwNumItems; dwItem++ )
		{
			_strResult = lpInfo[dwItem].pPrinterName;
		}

		HeapFree( GetProcessHeap(), 0, lpInfo );
	}
	// If Windows NT, use the GetDefaultPrinter API for Windows 2000,
	// or GetProfileString for version 4.0 and earlier...
	else if (osv.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (GetProfileString(_T("windows"), _T("device"), _T(",,,"), cBuffer, MAXBUFFERSIZE) <= 0)
		{
			MessageBox(NULL, _T("ERROR"), _T("GetProfileString"), MB_OK);
			return strError;
		}
		_tcstok(cBuffer, _T(","));
		_strResult = cBuffer;
    }

	return _strResult;
}

INT CEquipPrintPropertyDlg::FreePrintPropertyInfo()
{
	EFS_BEGIN

	CString strKey;
	CPrintPropertyInfo *pInfo = NULL;
	POSITION pos = m_mapPrintPropertyInfo.GetStartPosition();
	while (pos)
	{
		m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (void*&)pInfo);
		if (!pInfo) continue;

		delete pInfo;
	}

	m_mapPrintPropertyInfo.RemoveAll();

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::LoadPrintPropertyInfo()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_lpszFileName, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(m_lpszFileName, CFile::modeRead | CFile::modeCreate | CFile::typeBinary))
			return -1;
		else
		{
			if( 0 == file.GetLength() )
			{			
				TCHAR cUni = 0xFEFF;
				file.Write( &cUni, sizeof(TCHAR));
			}
		}
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	CPrintPropertyInfo *pInfo = NULL;

	CString strKey;
	CString strLine;
	while (file.ReadString(strLine))
	{
		strLine.TrimLeft();
		strLine.TrimRight();

		if (strLine.IsEmpty() || strLine.Left(1) == '.' || strLine.Left(1) == '#') continue;

		
		pInfo = new CPrintPropertyInfo;
		_stscanf(strLine, _T("%s\t=\t%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%s%4d"),
			strKey.GetBuffer(MAX_PATH), 
			&pInfo->m_nMarginUp,
			&pInfo->m_nMarginSide,
			&pInfo->m_nSpaceVert,
			&pInfo->m_nSpaceHorz,
			&pInfo->m_nLabelHight,
			&pInfo->m_nLabelWidth,
			&pInfo->m_nRowCount,
			&pInfo->m_nColCount,
			&pInfo->m_nMarginBarCode,
			&pInfo->m_nFontHeight,
			&pInfo->m_nFontSize,
			pInfo->m_strFontFace.GetBuffer(MAX_PATH),
			&pInfo->m_nFontStyle
			);
		
		if ( pInfo->m_nFontStyle < 0 ) pInfo->m_nFontStyle = 400;		

		strKey.ReleaseBuffer();
		pInfo->m_strFontFace.ReleaseBuffer();
		m_mapPrintPropertyInfo.SetAt(strKey, pInfo);
	}

	file.Close();

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::SavePrintPropertyInfo()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_lpszFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		m_strLastErrMsg.Format(_T("[%s] 파일을 열 수 없습니다."), m_lpszFileName);
		return -1;
	}
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}

	CString strKey;
	CString strLine;
	CPrintPropertyInfo *pInfo = NULL;
	POSITION pos = m_mapPrintPropertyInfo.GetStartPosition();
	while (pos)
	{
		m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (void*&)pInfo);
		if (!pInfo) continue;

		strLine.Format(_T("%s\t=\t%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%s%4d\r\n"),
			strKey, 
			pInfo->m_nMarginUp,
			pInfo->m_nMarginSide,
			pInfo->m_nSpaceVert,
			pInfo->m_nSpaceHorz,
			pInfo->m_nLabelHight,
			pInfo->m_nLabelWidth,
			pInfo->m_nRowCount,
			pInfo->m_nColCount,
			pInfo->m_nMarginBarCode,
			pInfo->m_nFontHeight,
			pInfo->m_nFontSize,
			pInfo->m_strFontFace,
			pInfo->m_nFontStyle
			);
		
		file.WriteString(strLine);
	}
	CString sUnit;
	GetDlgItem(IDC_CobUnit)->GetWindowText(sUnit);
	sUnit = _T("UNIT = ") + sUnit;
	file.WriteString(sUnit);

	file.Close();
	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::DisplayPrintPropertyInfo()
{
	EFS_BEGIN

	CPrintPropertyInfo *pInfo = NULL;

	if (!m_mapPrintPropertyInfo.Lookup(m_strPrintFormatKey, (void*&)pInfo))
	{
		pInfo = new CPrintPropertyInfo;
		if(_T("BARCODE2") == m_strPrintFormatKey)
		{			
			pInfo->m_nMarginUp = 50;
			pInfo->m_nMarginSide = 50;
			pInfo->m_nSpaceVert = 15;
			pInfo->m_nSpaceHorz = 20;
			pInfo->m_nLabelHight = 200;
			pInfo->m_nLabelWidth = 200;
			pInfo->m_nRowCount = 4;
			pInfo->m_nColCount = 3;
			pInfo->m_nMarginBarCode = 5;
			pInfo->m_nFontHeight = -12;
			pInfo->m_nFontSize = 9;
			pInfo->m_strFontFace = _T("굴림체");
			pInfo->m_nFontStyle = 400;
		}
		m_mapPrintPropertyInfo.SetAt(m_strPrintFormatKey, pInfo);
	}

	if (!pInfo) return -1;

	CString strData;
	
	strData.Format(_T("%d"), pInfo->m_nMarginUp);
	m_pCM->SetControlMgrData(_T("위쪽여백"), strData);

	strData.Format(_T("%d"), pInfo->m_nMarginSide);
	m_pCM->SetControlMgrData(_T("양쪽여백"), strData);

	strData.Format(_T("%d"), pInfo->m_nSpaceVert);
	m_pCM->SetControlMgrData(_T("세로간격"), strData);

	strData.Format(_T("%d"), pInfo->m_nSpaceHorz);
	m_pCM->SetControlMgrData(_T("가로간격"), strData);

	strData.Format(_T("%d"), pInfo->m_nLabelHight);
	m_pCM->SetControlMgrData(_T("레이블높이"), strData);

	strData.Format(_T("%d"), pInfo->m_nLabelWidth);
	m_pCM->SetControlMgrData(_T("레이블너비"), strData);

	strData.Format(_T("%d"), pInfo->m_nRowCount);
	m_pCM->SetControlMgrData(_T("가로개수"), strData);

	strData.Format(_T("%d"), pInfo->m_nColCount);
	m_pCM->SetControlMgrData(_T("세로개수"), strData);

	strData.Format(_T("%d"), pInfo->m_nMarginBarCode);
	m_pCM->SetControlMgrData(_T("바코드여백"), strData);

	CString strFont;
	CString strStyle;
	if  (pInfo->m_nFontStyle == 400 ) strStyle=_T("보통");
	else if (pInfo->m_nFontStyle == 700 ) strStyle=_T("굵게");
	else  strStyle=_T("보통");

	strFont.Format(_T("%s, %d, %s"), pInfo->m_strFontFace, pInfo->m_nFontSize, strStyle);
	GetDlgItem(IDC_bFONT)->SetWindowText(strFont);	

	return 0;

	EFS_END
	return -1;
}

CPrintPropertyInfo *CEquipPrintPropertyDlg::ApplyPrintPropertyInfo()
{
	EFS_BEGIN

	UpdateData();
	
	CPrintPropertyInfo *pInfo = NULL;

	if (!m_mapPrintPropertyInfo.Lookup(m_strPrintFormatKey, (void*&)pInfo))
	{
		pInfo = new CPrintPropertyInfo;
		m_mapPrintPropertyInfo.SetAt(m_strPrintFormatKey, pInfo);
	}

	CString strData;

	m_pCM->GetControlMgrData(_T("위쪽여백"), strData);
	pInfo->m_nMarginUp = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("양쪽여백"), strData);
	pInfo->m_nMarginSide = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("세로간격"), strData);
	pInfo->m_nSpaceVert = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("가로간격"), strData);
	pInfo->m_nSpaceHorz = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("레이블높이"), strData);
	pInfo->m_nLabelHight = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("레이블너비"), strData);
	pInfo->m_nLabelWidth = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("가로개수"), strData);
	pInfo->m_nRowCount = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("세로개수"), strData);
	pInfo->m_nColCount = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("바코드여백"), strData);
	pInfo->m_nMarginBarCode = _ttoi(strData);

	return pInfo;

	EFS_END
	return NULL;
}

INT CEquipPrintPropertyDlg::PrintEquip(CPrintPropertyInfo *pInfo)
{
	EFS_BEGIN

	if (pInfo == NULL) return -1;

	BeginWaitCursor();

	UpdateData();

	CSIReportPrintObjectMgr PrintMgr;
	PrintMgr.m_strBarcodeStyle = _T("Code 128A");

	CString sUnit;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CobUnit);	
	pCombo->GetLBText( pCombo->GetCurSel() , sUnit );
	INT nPrintCount = 0;

	if ( sUnit == _T("밀리미터(mm)") ) {
		PrintMgr.nStartY = pInfo->m_nMarginUp * 10/3;

		PrintMgr.nStartX = pInfo->m_nMarginSide * 10/3;

		PrintMgr.nTableHeightGap = pInfo->m_nSpaceVert * 10/3;

		PrintMgr.nTableWidthGap = pInfo->m_nSpaceHorz * 10/3;

		PrintMgr.nTableWidth = pInfo->m_nLabelWidth * 10/3;	
	}
	else {
		PrintMgr.nStartY = pInfo->m_nMarginUp;

		PrintMgr.nStartX = pInfo->m_nMarginSide;

		PrintMgr.nTableHeightGap = pInfo->m_nSpaceVert;

		PrintMgr.nTableWidthGap = pInfo->m_nSpaceHorz;

		PrintMgr.nTableWidth = pInfo->m_nLabelWidth;
	}

	PrintMgr.m_nBarcodeWidthGap  = pInfo->m_nMarginBarCode;

	PrintMgr.m_nRows = pInfo->m_nRowCount;
	
	PrintMgr.m_nCols = pInfo->m_nColCount;
	
	PrintMgr.SetFont(pInfo->m_nFontSize, pInfo->m_strFontFace, pInfo->m_nFontStyle);

	if (m_nOption_PaperDir == 1)
	{
		m_pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;
		PrintMgr.nPaper = PAPER_A4_PORTRAIT;
	}
	else
	{
		m_pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
		PrintMgr.nPaper = PAPER_A4_LANDSCAPE;
	}

	INT nStartPos = 0;
	CString strStartPos;
	m_pCM->GetControlMgrData(_T("시작위치"), strStartPos);
	
	if (!strStartPos.IsEmpty())
	{
		nStartPos = _ttoi(strStartPos);
		nStartPos--;
	}

	CString strRegNoPlace;
	m_pCM->GetControlMgrData(_T("등록번호위치"), strRegNoPlace);

	if ( m_nOption_Print == 1 && strRegNoPlace == _T("레이블안쪽") )
	{
		( (CComboBox*) GetDlgItem (IDC_cREG_NO_PLACE) )->SetCurSel(0);
	}
	
	INT iPrintYesNo = 0;	
	
	if (m_nOption_Print == 0)
	{		
		if (strRegNoPlace == _T("용지오른쪽"))
		{
			m_nMaxLine = 6;
			DOUBLE dMaxLine = 6.0;			
			DOUBLE dTmp;
			if ( sUnit == _T("밀리미터(mm)") ) {
				dTmp = pInfo->m_nLabelHight * 10/3;
			}
			else {
				dTmp = pInfo->m_nLabelHight;
			}
			PrintMgr.nLineHeight = dTmp / dMaxLine;
	
			PrintMgr.m_nCols++;
			iPrintYesNo = PrintEquip_LineNormal(pInfo, &PrintMgr, nStartPos);
			if ( iPrintYesNo == -1 ) {
				AfxMessageBox( _T("출력할 데이터가 없습니다.") );
				return -1;
			}
		}
		else if (strRegNoPlace == _T("레이블사이"))
		{
			m_nMaxLine = 6;
			if ( sUnit == _T("밀리미터(mm)") ) {
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
			}
			PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
			nPrintCount = PrintEquip_LineExtra(pInfo, &PrintMgr, nStartPos);
			if ( nPrintCount == 0 ) 
			{
				AfxMessageBox(_T("선정된 자료 중 입력한 등록번호에 해당하는 자료가 없습니다."));
				return 0;
			}
		}
		else if (strRegNoPlace == _T("레이블안쪽"))
		{
			m_nMaxLine = 6;
			if ( sUnit == _T("밀리미터(mm)") ) {
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
			}
			PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
			
			nPrintCount = PrintEquip_LineExtra2(pInfo, &PrintMgr, nStartPos);
			if ( nPrintCount == 0 ) 
			{
				AfxMessageBox(_T("선정된 자료 중 입력한 등록번호에 해당하는 자료가 없습니다."));
				return 0;
			}
			
		}
	}
	else if (m_nOption_Print == 1)
	{		
		if (strRegNoPlace == _T("용지오른쪽"))
		{
			m_nMaxLine = PrintMgr.m_nCols;
			if ( sUnit == _T("밀리미터(mm)") ) {
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
			}
			PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;

			PrintMgr.m_nCols++;
			PrintEquip_SerialNormal(pInfo, &PrintMgr, nStartPos);
		}
		else if (strRegNoPlace == _T("레이블사이"))
		{
			m_nMaxLine = 2;
			if ( sUnit == _T("밀리미터(mm)") ) {
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
			}
			PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;			
			nPrintCount = PrintEquip_SerialExtra(pInfo, &PrintMgr, nStartPos);
			if ( nPrintCount == 0 ) 
			{
				AfxMessageBox(_T("선정된 자료 중 입력한 등록번호에 해당하는 자료가 없습니다."));
				return 0;
			}
		}
	}
	else if (m_nOption_Print == 2 || m_nOption_Print == 5)
	{
		m_nMaxLine = 4;
		if ( sUnit == _T("밀리미터(mm)") ) {
			pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
		}
		PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
		PrintEquip_BarCode(pInfo, &PrintMgr, nStartPos);
	}
	else if (m_nOption_Print == 3)
	{
		m_nMaxLine = 8;
		if ( sUnit == _T("밀리미터(mm)") ) {
			pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
		}
		PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
		PrintEquip_BookCard(pInfo, &PrintMgr, nStartPos);
	}
	else if (m_nOption_Print == 4)
	{
		m_nMaxLine = 8;
		if ( sUnit == _T("밀리미터(mm)") ) {
			pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
		}
		PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
		PrintEquip_Sticker(pInfo, &PrintMgr, nStartPos);
	}

	INT ids = m_pSIReportManager->DeleteTempPageFiles();

	EndWaitCursor();

	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_CHK_SERIAL);
	if ( pSerial->GetCheck() == 0 ) {

		m_pSIReportManager->FreeWorkForm();
		m_pSIReportManager->m_bPrivacyPrintLogRecord = TRUE;
		ids = PrintMgr.Print(m_pSIReportManager);

		ids = m_pSIReportManager->Print2();
	}
	
	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_LineNormal(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN
	INT iStartPosOri = nStartPosition;
	
	if (pPrintMgr == NULL) return -1;
	
	INT ids = -1;
	INT idx = -1;
	INT nCol = 0;
	INT nLine = 0;
	INT nAnsiNeedByte = 0;
	BOOL bAddCard = TRUE;
	CString strItemData;
	CString strRegNo;
	CString strRegNoField;
	CSIReportPrintObject *pObject = NULL;
	INT iTotalPrintCount;
	iTotalPrintCount = m_pDM_Parent->GetRowCount();
	INT iRealPrintCount = 0;
	
	CString strQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_1 = 'L' AND GROUP_2 = '정리' AND GROUP_3 = '공통' AND CLASS_ALIAS = '레이블권책기호설정'");
	CString strValue = _T("");
	m_pDM_Parent->GetOneValueQuery(strQuery, strValue);	
	strValue.TrimLeft(); strValue.TrimRight();

	INT iPrintCount = 0;
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
	INT iStart = 0;
	while (i >= 0)
	{		
		idx = i;		
			if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1))  == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {
				idx = iTotalPrintCount-1;				
			}

			if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1)) > (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {		
				break;
			}

		i = m_pGrid_Parent->SelectGetNext();
		
		nLine = 0;

		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			iPrintCount ++;
			if (!pObject) continue;
			nCol++;
		}

		bAddCard = TRUE;

		if (nCol % pPrintMgr->m_nCols == 0)		
		{
			if (i < 0 ) 
			{
				i = m_pGrid_Parent->SelectGetHeadPosition();
			
			}

			INT n = 0;
			strRegNoField.TrimRight();
			if (strRegNoField.IsEmpty()) continue;

			LPTSTR lpszToken = _tcstok(strRegNoField.GetBuffer(0), _T("|"));
			while (lpszToken)
			{
				pObject->SetAt(n++, lpszToken);
				lpszToken = _tcstok(NULL, _T("|"));
			}

			strRegNoField.Empty();	

			i = m_pGrid_Parent->SelectGetPrev();
			
			if ( i == -1 ) {
				i = m_pGrid_Parent->SelectGetTailPosition();
			}
			
		}
		else
		{
			if (--nStartPosition >= 0) 
			{
				i = m_pGrid_Parent->SelectGetHeadPosition();

				if (nStartPosition > 0) 
				{	
					strRegNoField = _T(" |") + strRegNoField;
				}
				continue;
			}

			if (m_pDM_Parent->GetCellData(_T("레이블출력구분"), idx) != _T("2")) 
			{
				if ( idx != m_pGrid_Parent->SelectGetCount() ) {
					bAddCard = FALSE;
					continue;
				}
				else {
					idx--;
				}
			}		
			ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strRegNo);
			
			if (!strRegNo.IsEmpty())
			{
				INT nRegCodeLength;
				nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
				for ( INT j = nRegCodeLength ; j < strRegNo.GetLength() ; j++ )
				{
					if( strRegNo.Mid(j, 1 ) != _T("0") )
					{
						strRegNo = strRegNo.Left(nRegCodeLength) + strRegNo.Mid ( j, strRegNo.GetLength()-j  );
						break;
					}			
				}						
			}          
			if (!IsValidRegNo(strRegNo)) 
			{
				bAddCard = FALSE;
				continue;
			}

			iRealPrintCount++;

			ids = m_pDM_Parent->GetCellData(_T("별치기호_코드"), idx, strItemData);
			if (!strItemData.IsEmpty())
			{
				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
				pObject->SetAt(nLine++, strItemData);
			}
			else
			{
				pObject->SetAt(nLine++, _T(""));
			}
			
			ids = m_pDM_Parent->GetCellData(_T("분류기호"), idx, strItemData);
			if (!strItemData.IsEmpty())
			{
				pObject->SetAt(nLine++, strItemData);
			}
			else
			{
				pObject->SetAt(nLine++, _T(""));
			}
			
			ids = m_pDM_Parent->GetCellData(_T("도서기호"), idx, strItemData);
			if (!strItemData.IsEmpty())
			{
				pObject->SetAt(nLine++, strItemData);
			}
			else
			{
				pObject->SetAt(nLine++, _T(""));
			}

			ids = m_pDM_Parent->GetCellData(_T("권책기호"), idx, strItemData);
			if (!strItemData.IsEmpty())
				pObject->SetAt(nLine++, strValue + strItemData);
			
			ids = m_pDM_Parent->GetCellData(_T("복본기호"), idx, strItemData);
			if (!strItemData.IsEmpty())
				pObject->SetAt(nLine++, _T("c.") + strItemData);

			if (nCol % pPrintMgr->m_nCols != 1) {
				strRegNoField += _T(" |") + strRegNo;
			}
			else {
				strRegNoField = strRegNo;
			}
		}
	}

	if ( iRealPrintCount == 0 ) return -1;

	strRegNoField.TrimRight();
	if (!strRegNoField.IsEmpty())
	{
		while (TRUE)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			if (!pObject) continue;

			if (++nCol % pPrintMgr->m_nCols == 0)
			{
				INT n = 0;
				strRegNoField.TrimRight();
				if (strRegNoField.IsEmpty()) continue;

				LPTSTR lpszToken = _tcstok(strRegNoField.GetBuffer(0), _T("|"));
				while (lpszToken)
				{
					pObject->SetAt(n++, lpszToken);
					lpszToken = _tcstok(NULL, _T("|"));
				}
				
				iStart = 1;
				break;
			}	
		}
	}

	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_CHK_SERIAL);
	if ( pSerial->GetCheck() == 1 ) {	
		Sleep(300);	
		TCHAR szComName[255];	
		DWORD len;
		len =255;

		GetComputerName(szComName, &len);

		szPrinterName = GetPrnName();
		CString strNameFormat;
		strNameFormat.Format( _T("net use LPT1 \\\\%s"), szComName);
		
		CHAR *pcCmd;
		m_MarcConvertApi.GetStrUniCodeToAnsi(strNameFormat);
		pcCmd = m_MarcConvertApi.m_cpConvertData;
		ids = WinExec(pcCmd, SW_HIDE);
		if (ids <= 31) {
			AfxMessageBox(_T("연속 용지용 프린터 설정을 확인하세요!"));
			return -1;
		}
		
		Sleep(400);
		FILE* prn = NULL;
		prn = _tfopen(_T("LPT1"), _T("wb"));
		if(prn == NULL)
		{
			AfxMessageBox(_T("연속 용지용 프린터 설정을 확인하세요!"));
			return -1;
		}		
		Sleep(300);
		fseek(prn, 0, SEEK_END);
	
		CString sPathSerialConfig = _T("..\\CFG\\정리_연속용지_출력.cfg");		
		FILE *fpSerialConfig;
		TCHAR cLineConfig[256];
		fpSerialConfig = _tfopen( sPathSerialConfig , _T("rb") );		
		if ( fpSerialConfig == NULL ) {
			AfxMessageBox( _T("연속용지 설정을 저장하세요!") );
			return -1;
		}

		FILE* fpUni = fpSerialConfig;
	
		if( 0xFF == fgetc(fpUni) ) 
		{
			if( 0xFE != fgetc(fpUni) )
			{
				fseek(fpUni, 0, SEEK_SET);
			}
		}
		else
		{
			fseek(fpUni, 0, SEEK_SET);
		}

		INT iColCount = 2;
		INT iLineCount = 5;
		INT iFirstLoc = 10;			
		INT iSecLoc = 30;			
		INT iRegLoc = 60;
		INT iColSpace = 1;
		INT iFirstLineSpaceYN = 1;
		INT iSerialFont = -1;
	
		fprintf( prn, "%c%c0", 27, 114);		

		CString sLineConfig;
		while( _fgetts( cLineConfig, 256, fpSerialConfig) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLineConfig );
			INT iFind = sLine.Find( _T(":") );
			CString sItem = sLine.Mid( iFind+1 );		
			sItem.Replace( _T(" "), NULL );
			sItem.Replace( _T("\n"), NULL );

			if ( sLine.Left(1) == _T("1") ) {
				iFirstLoc = _ttoi( sItem );
			}
			else if ( sLine.Left(1) == _T("2") ) {
				iSecLoc = _ttoi( sItem );
			}
			else if ( sLine.Left(1) == _T("3") ) {
				iRegLoc = _ttoi( sItem );
			}
			else if ( sLine.Left(1) == _T("4") ) {
				iColSpace = _ttoi( sItem );
			}
			else if ( sLine.Left(1) == _T("5") ) {

			}
			else if ( sLine.Left(1) == _T("6") ) {
				if ( sItem == _T("출력함") ) {					
				}
				else {					
				}
			}
			else if ( sLine.Left(1) == _T("7") ) {				
				iFirstLineSpaceYN = _ttoi( sItem );
			}
			else if ( sLine.Left(1) == _T("8") ) {
				if ( sItem == _T("기본폰트") ) {
					iSerialFont = 0;
				}
				else if ( sItem == _T("한글명조체10.5p") ) {
					iSerialFont = 0;					
					fprintf( prn, "%c%c0", 28, 107 );
				}
				else if ( sItem == _T("한글고딕체10.5p") ) {
					iSerialFont = 1;					
					fprintf( prn, "%c%c1", 28, 107 );
				}
				else if ( sItem == _T("한글명조체10.5p") ) {
					iSerialFont = 2;
					fprintf( prn, "%c%c2", 28, 107 );					
				}
				else if ( sItem == _T("한글고딕체14p") ) {
					iSerialFont = 3;
					fprintf( prn, "%c%c3", 28, 107 );					
				}
				else if ( sItem == _T("한글샘물체10.5p") ) {
					iSerialFont = 6;
					fprintf( prn, "%c%c6", 28, 107 );					
				}
				else if ( sItem == _T("한글샘물체14p") ) {
					iSerialFont = 7;
					fprintf( prn, "%c%c7", 28, 107 );					
				}
				else if ( sItem == _T("한글궁서체10.5p") ) {
					iSerialFont = 9;
					fprintf( prn, "%c%c9", 28, 107 );
				}
				else if ( sItem == _T("한글필기체10.5p") ) {
					iSerialFont = 10;
					fprintf( prn, "%c%cA", 28, 107 );
				}
				else if ( sItem == _T("한글디나루체10.5p") ) {
					iSerialFont = 11;
					fprintf( prn, "%c%cB", 28, 107 );
				}
				else if ( sItem == _T("한글옛체10.5p") ) {
					iSerialFont = 12;
					fprintf( prn, "%c%cC", 28, 107 );
				}
			}
		}
		
		iRegLoc = iRegLoc - iFirstLoc - iSecLoc;
		INT iRowCount = pPrintMgr->GetCount() / (iColCount+1);
		
		CString sFirstLoc;
		for ( i = 0 ; i < iFirstLoc ; i++ ) {
			sFirstLoc = sFirstLoc + _T(" ");
		}
		
		CString sPreView;
		for ( i = 0 ; i < iFirstLineSpaceYN ; i++ ) {		
			sPreView = sPreView + _T("\r\n");
		}
		
		INT iColNo = 0;
		CString sItem;	
		for( i = 0 ; i < iRowCount ; i++ ) {		
			for ( INT j = 0 ; j < iLineCount ; j++ ) {
				for ( INT k = 0 ; k <= iColCount ; k++ ) {
					iColNo = i * (iColCount+1) + k;
					if ( iColNo >= pPrintMgr->GetCount() ) {
						break;
					}
					sItem = pPrintMgr->GetAt(iColNo)->LINE[j];
					sItem = sItem;				
					
					if ( (iColNo % (iColCount+1)) == 0 ) {
						sItem = sFirstLoc + sItem;
						
						nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sItem);
						while ( nAnsiNeedByte < iSecLoc ) 
						{
							sItem = sItem + _T(" ");
							++nAnsiNeedByte;
						}
					}
					if ( (iColNo % (iColCount+1)) == 1 ) {
						
						nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sItem);
						while ( nAnsiNeedByte < iRegLoc ) 
						{
							sItem = sItem + _T(" ");
							++nAnsiNeedByte;
						}						
					}
					if ( (iColNo % (iColCount+1)) == 2 ) {
										
					}
										
					sPreView = sPreView + sItem;
				}			

				sPreView = sPreView + _T("\r\n");
			}

			for ( j = 1 ; j <= iColSpace ; j++ ) {
				
				sPreView = sPreView + _T("\r\n");
			}
		}	

		CSerialLabelPreview dlg(this);
		dlg.m_sPreview = sPreView;
		if ( dlg.DoModal() == IDOK ) {
			
			CHAR *szpStream;
			szpStream = NULL;
			m_MarcConvertApi.GetStrUniCodeToAnsi(sPreView);
			szpStream = m_MarcConvertApi.m_cpConvertData;
			
			fprintf( prn, "%s", szpStream );

			fflush(prn);						
		}
		fclose(prn);
		fclose(fpSerialConfig);
	}		
	
	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_LineExtra(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;
	
	CString sQuery = _T("");
	CString sValue = _T("") ;
	sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_1 = 'L' AND GROUP_2 = '정리' AND GROUP_3 = '공통' AND CLASS_ALIAS = '레이블권책기호설정'");
	m_pDM_Parent->GetOneValueQuery( sQuery , sValue );	
	sValue.TrimLeft(); sValue.TrimRight();

	INT ids = -1;
	INT idx = -1;
	INT nLine = 0;
	BOOL bAddCard = TRUE;
	CString strItemData;
	CSIReportPrintObject *pObject = NULL;
	INT nPrintCount = 0;
		
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
	while (i >= 0)
	{
		idx = i;
		i = m_pGrid_Parent->SelectGetNext();

		nLine = 0;

		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(7);
			if (!pObject) continue;
		}

		bAddCard = TRUE;

		if (--nStartPosition >= 0) 
		{
			i = m_pGrid_Parent->SelectGetHeadPosition();
			continue;
		}

		if (m_pDM_Parent->GetCellData(_T("레이블출력구분"), idx) != _T("2")) 
		{
			bAddCard = FALSE;
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strItemData);
		if (!IsValidRegNo(strItemData)) 
		{
			bAddCard = FALSE;
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("별치기호_코드"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
			pObject->SetAt(nLine++, strItemData);
		}
		else
		{
			pObject->SetAt(nLine++, _T(""));
		}
		
		ids = m_pDM_Parent->GetCellData(_T("분류기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			pObject->SetAt(nLine++, strItemData);
		}
		else
		{
			pObject->SetAt(nLine++, _T(""));
		}
		
		ids = m_pDM_Parent->GetCellData(_T("도서기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			pObject->SetAt(nLine++, strItemData);
		}
		else
		{
			pObject->SetAt(nLine++, _T(""));
		}

		ids = m_pDM_Parent->GetCellData(_T("권책기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			strItemData = sValue + strItemData;
			pObject->SetAt(nLine++, strItemData);
		}

		ids = m_pDM_Parent->GetCellData(_T("복본기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, _T("c.") + strItemData);

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			INT nRegCodeLength;
			nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
			for ( INT nIdx = nRegCodeLength ; nIdx < strItemData.GetLength() ; nIdx++ )
			{
				if ( strItemData.Mid(nIdx, 1 ) == _T("0") )
					continue;
				else{
					CString strTmp049Data;
					strTmp049Data = strItemData.Left(nRegCodeLength) + strItemData.Mid ( nIdx, strItemData.GetLength()-nIdx  );
					pObject->SetAt(6, strTmp049Data, TRUE);
					break;
				}			
			}		
		}

		nPrintCount ++;
	}

	return nPrintCount;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_LineExtra2(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;
	
	CString sQuery = _T("");
	CString sValue = _T("") ;
	sQuery = _T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND GROUP_1 = 'L' AND GROUP_2 = '정리' AND GROUP_3 = '공통' AND CLASS_ALIAS = '레이블권책기호설정'");
	m_pDM_Parent->GetOneValueQuery( sQuery , sValue );	
	sValue.TrimLeft(); sValue.TrimRight();

	INT ids = -1;
	INT nLine = 0;
	BOOL bAddCard = TRUE;
	CString strItemData;
	CSIReportPrintObject *pObject = NULL;
	
	INT nPrintCnt;
	INT nSelectCnt;
	INT i;
	INT nIdx;
	nPrintCnt = 0;
	nSelectCnt = 0;

	nSelectCnt = m_pGrid_Parent->SelectGetCount();
	nIdx = m_pGrid_Parent->SelectGetHeadPosition();

	for(INT j = 0 ; j < nStartPosition ; j++)
	{
 		if (bAddCard)
 		{
 			pObject = pPrintMgr->AddCard(m_nMaxLine);
 			if (!pObject) continue;
 		}
 
 		bAddCard = TRUE;
	}

	for( i = 0 ; i < nSelectCnt ; i++ )
	{
		nLine = 0;

		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			if (!pObject) 
			{
				nIdx = m_pGrid_Parent->SelectGetNext();
				continue;
			}
		}

		bAddCard = TRUE;

		if (m_pDM_Parent->GetCellData(_T("레이블출력구분"), nIdx) != _T("2")) 
		{
			bAddCard = FALSE;
			nIdx = m_pGrid_Parent->SelectGetNext();
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), nIdx, strItemData);
		if (!IsValidRegNo(strItemData)) 
		{
			bAddCard = FALSE;
			nIdx = m_pGrid_Parent->SelectGetNext();
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("별치기호_코드"), nIdx, strItemData);
		if (!strItemData.IsEmpty())
		{
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
			pObject->SetAt(nLine++, strItemData);
		}
		else
		{
			pObject->SetAt(nLine++, _T(""));
		}
		
		ids = m_pDM_Parent->GetCellData(_T("분류기호"), nIdx, strItemData);
		if (!strItemData.IsEmpty())
		{
			pObject->SetAt(nLine++, strItemData);
		}
		else
		{
			pObject->SetAt(nLine++, _T(""));
		}
		
		ids = m_pDM_Parent->GetCellData(_T("도서기호"), nIdx, strItemData);
		if (!strItemData.IsEmpty())
		{
			pObject->SetAt(nLine++, strItemData);
		}
		else
		{
			pObject->SetAt(nLine++, _T(""));
		}

		ids = m_pDM_Parent->GetCellData(_T("권책기호"), nIdx, strItemData);
		if (!strItemData.IsEmpty())
		{
			strItemData = sValue + strItemData;
			pObject->SetAt(nLine++, strItemData);
		}

		ids = m_pDM_Parent->GetCellData(_T("복본기호"), nIdx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, _T("c.") + strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("등록번호"), nIdx, strItemData);
		if (!strItemData.IsEmpty())
		{
			INT nRegCodeLength;
			nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
			for ( INT j = nRegCodeLength ; j < strItemData.GetLength() ; j++ )
			{
				if( strItemData.Mid(j, 1 ) != _T("0") )
				{
					CString strTmp049Data;
					strTmp049Data = strItemData.Left(nRegCodeLength) + strItemData.Mid ( j, strItemData.GetLength()-j  );
					pObject->SetAt(5, strTmp049Data, TRUE);
					break;
				}			
			}						
		}             
		nPrintCnt++;
		nIdx = m_pGrid_Parent->SelectGetNext();
	}
	return nPrintCnt;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_SerialNormal(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;
	
	INT ids = -1;
	INT idx = -1;
	INT nCol = 0;
	INT nAnsiNeedByte = 0;

	BOOL bAddCard = TRUE;
	CString strRegNo;
	CString strRegNoField;
	CString strCallNo;
	CSIReportPrintObject *pObject = NULL;
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
	INT iLast = 0;
	while (i >= 0)
	{
		
		idx = i;
		i = m_pGrid_Parent->SelectGetNext();

		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			if (!pObject) continue;
			nCol++;
		}

		bAddCard = TRUE;

		if (nCol % pPrintMgr->m_nCols == 0)		
		{
			i = m_pGrid_Parent->SelectGetPrev();
			if ( iLast == 0 && i == -1 && pPrintMgr->m_nCols == 2) {
				i = nCol/2;
				iLast = 1;
			}
			INT n = 0;

			strRegNoField.TrimRight();
			if (strRegNoField.IsEmpty()) continue;

			LPTSTR lpszToken = _tcstok(strRegNoField.GetBuffer(0), _T("|"));
			while (lpszToken)
			{
				pObject->SetAt(n++, lpszToken);
				lpszToken = _tcstok(NULL, _T("|"));
			}

			strRegNoField.Empty();
		}
		else
		{			
			if (--nStartPosition >= 0) 
			{
				i = m_pGrid_Parent->SelectGetHeadPosition();
				continue;
			}

			if (m_pDM_Parent->GetCellData(_T("레이블출력구분"), idx) != _T("1")) 
			{
				bAddCard = FALSE;
				continue;
			}

			ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strRegNo);
			
			if (!strRegNo.IsEmpty())
			{
				INT nRegCodeLength;
				nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
				for ( INT j = nRegCodeLength ; j < strRegNo.GetLength() ; j++ )
				{
					if( strRegNo.Mid(j, 1 ) != _T("0") )
					{
						strRegNo = strRegNo.Left(nRegCodeLength) + strRegNo.Mid ( j, strRegNo.GetLength()-j  );
						break;
					}			
				}						
			}            
			if (!IsValidRegNo(strRegNo)) 
			{
				bAddCard = FALSE;
				continue;
			}

			ids = m_pDM_Parent->GetCellData(_T("청구기호"), idx, strCallNo);
			if (strCallNo.IsEmpty()) continue;
			pObject->SetAt(0, strCallNo);

			strRegNoField += _T("|") + strRegNo;
		}
	}
	INT iRowCount = pPrintMgr->GetCount();

	if (iRowCount == 0 ) {
		AfxMessageBox( _T("출력할 자료가 없습니다.") );
		return -1;
	}

	strRegNoField.TrimRight();

	if (!strRegNoField.IsEmpty())
	{
		while (TRUE)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			if (!pObject) continue;

			if (++nCol % pPrintMgr->m_nCols == 0)
			{
				INT n = 0;
				strRegNoField.TrimRight();
				if (strRegNoField.IsEmpty()) continue;

				LPTSTR lpszToken = _tcstok(strRegNoField.GetBuffer(0), _T("|"));
				while (lpszToken)
				{
					pObject->SetAt(n++, lpszToken);
					lpszToken = _tcstok(NULL, _T("|"));
				}

				break;
			}	
		}
	}

	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_CHK_SERIAL);
	if ( pSerial->GetCheck() == 1 ) {
		Sleep(300);	
		TCHAR szComName[255];	
		DWORD len;
		len =255;

		GetComputerName(szComName, &len);

		szPrinterName = GetPrnName();
		CString strNameFormat;
		strNameFormat.Format( _T("net use LPT1 \\\\%s"), szComName);
		
		CHAR *pcCmd;
		m_MarcConvertApi.GetStrUniCodeToAnsi(strNameFormat);
		pcCmd = m_MarcConvertApi.m_cpConvertData;
		ids = WinExec(pcCmd, SW_HIDE);
		if (ids <= 31) {
			AfxMessageBox(_T("연속 용지용 프린터 설정을 확인하세요!"));
			return -1;
		}

		Sleep(400);
		FILE* prn = _tfopen(_T("LPT1"), _T("wb"));		
		if(prn == NULL)
		{
			AfxMessageBox(_T("연속 용지용 프린터 설정을 확인하세요!"));
			return -1;
		}
		fseek(prn, 0, SEEK_END);		

		Sleep(300);
	
		CString sPathSerialConfig = _T("..\\CFG\\정리_연속용지_이어찍기_출력.cfg");		
		FILE *fpSerialConfig;
		TCHAR cLineConfig[256];
		fpSerialConfig = _tfopen( sPathSerialConfig , _T("rb") );		
		if ( fpSerialConfig == NULL ) {
			AfxMessageBox( _T("연속용지 이어찍기 설정을 저장하세요!") );
			return -1;
		}
		FILE* fpUni = fpSerialConfig;
	
		if( 0xFF == fgetc(fpUni) ) 
		{
			if( 0xFE != fgetc(fpUni) )
			{
				fseek(fpUni, 0, SEEK_SET);
			}
		}
		else
		{
			fseek(fpUni, 0, SEEK_SET);
		}
		
		INT iColCount = 2;
		INT iLineCount = 5;
		INT iFirstLoc = 10;			
		INT iSecLoc = 30;			
		INT iRegLoc = 60;

		INT iColSpace = 1;
		INT iFirstLineSpaceYN = 0;
		INT iSerialFont = -1;

		CString sLineConfig;
		CString sFirstLoc, sSecLoc, sColnoLoc, sColCount;
		while( _fgetts( cLineConfig, 256, fpSerialConfig) ) {
			CString sLine;
			sLine.Format( _T("%s"), cLineConfig );
			INT iFind = sLine.Find( _T(":") );
			CString sItem = sLine.Mid( iFind+1 );		
			sItem.Replace( _T(" "), NULL );
			sItem.Replace( _T("\n"), NULL );

			if ( sLine.Left(1) == _T("1") ) {				
				iSecLoc = _ttoi(sItem);
			}
			else if ( sLine.Left(1) == _T("2") ) {				
				iRegLoc = _ttoi(sItem);
			}		
			else if ( sLine.Left(1) == _T("3") ) {				
				if ( sItem == _T("공백있음") ) {
					iFirstLineSpaceYN = 1;
				}
			}			
		}

		CString sPreView;
		for ( i = 0 ; i < iFirstLineSpaceYN ; i++ ) {			
			sPreView = sPreView + _T("\r\n");
		}
		
		CString sOneLine;
		INT iRowCount = pPrintMgr->GetCount();
		CString sColNo;
		GetDlgItem(IDC_eLENGTH_COUNT)->GetWindowText(sColNo);
		if ( sColNo == _T("2") ) {
			for( INT i = 0 ; i < iRowCount ; i++ ) {
				CString sItem = pPrintMgr->GetAt(i)->LINE[0];

				sOneLine = sOneLine + sItem;
				if ( (i+1) % 3 == 1 ) {
					
					nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sOneLine);
					while ( nAnsiNeedByte < iSecLoc ) 
					{
						sOneLine = sOneLine + _T(" ");
						++nAnsiNeedByte;
					}
				}
				if ( (i+1) % 3 == 2 ) {
					
					nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sOneLine);
					while ( nAnsiNeedByte < iRegLoc ) 
					{
						sOneLine = sOneLine + _T(" ");
						++nAnsiNeedByte;
					}
				}

				if ( (i+1) % 3 == 0 ) {
					sItem = pPrintMgr->GetAt(i)->LINE[1];		
					CString sSpace;
					
					nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sSpace);
					while ( nAnsiNeedByte < iRegLoc ) 
					{
						sSpace = sSpace + _T(" ");
						++nAnsiNeedByte;
					}
					
					sPreView = sPreView + sOneLine + _T("\r\n") + sSpace + sItem + _T("\r\n\r\n");
					sOneLine = _T("");
				}
			}
		}
		else {
			for( INT i = 0 ; i < iRowCount ; i++ ) {

				CString sItem;
				ids = m_pDM_Parent->GetCellData(_T("청구기호"), i, sItem);
								
				
				CString sSpace;

				nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sSpace);
				while ( nAnsiNeedByte < iSecLoc ) 
				{
					sSpace = sSpace + _T(" ");
					++nAnsiNeedByte;
				}

				sItem = sSpace + sItem;
				
				nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sItem);
				while ( nAnsiNeedByte < iRegLoc ) 
				{
					sItem = sItem + _T(" ");
					++nAnsiNeedByte;
				}

				sPreView = sPreView + sItem;
							
				ids = m_pDM_Parent->GetCellData(_T("등록번호"), i, sItem);
				
				if (!sItem.IsEmpty())
				{
					INT nRegCodeLength;
					nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
					for ( INT j = nRegCodeLength ; j < sItem.GetLength() ; j++ )
					{
						if( sItem.Mid(j, 1 ) != _T("0") )
						{
							sItem = sItem.Left(nRegCodeLength) + sItem.Mid ( j, sItem.GetLength()-j  );
							break;
						}			
					}						
				}            
				
				nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sSpace);
				while ( nAnsiNeedByte < iSecLoc ) 
				{
					sSpace = sSpace + _T(" ");
					++nAnsiNeedByte;
				}
				
				sItem = sSpace + sItem;
				
				nAnsiNeedByte = m_MarcConvertApi.GetLengthUnicodeToAnsi(sItem);
				while ( nAnsiNeedByte < iRegLoc ) 
				{
					sItem = sItem + _T(" ");
					++nAnsiNeedByte;
				}				

				sPreView = sPreView + sItem + _T("\r\n\r\n");
			}
			sPreView.TrimRight();
		}
		CSerialLabelPreview dlg(this);
		dlg.m_sPreview = sPreView;
		if ( dlg.DoModal() == IDOK ) {
			
			CHAR *szpStream;
			szpStream = NULL;
			m_MarcConvertApi.GetStrUniCodeToAnsi(sPreView);
			szpStream = m_MarcConvertApi.m_cpConvertData;
			
			fprintf( prn, "%s", szpStream );
			
			fflush(prn);			
		}
		fclose(prn);	
		fclose(fpSerialConfig);		
	}

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_SerialExtra(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;
	
	INT ids = -1;
	INT nIdx = -1;
	BOOL bAddCard = TRUE;
	CString strRegNo;
	CString strCallNo;
	CSIReportPrintObject *pObject = NULL;

	INT nPrintCnt;
	INT nSelectCnt;
	nPrintCnt = 0;
	nSelectCnt = 0;

	nSelectCnt = m_pGrid_Parent->SelectGetCount();
	nIdx = m_pGrid_Parent->SelectGetHeadPosition();

	for(INT j = 0 ; j < nStartPosition ; j++)
	{
 		if (bAddCard)
 		{
 			pObject = pPrintMgr->AddCard(m_nMaxLine);
 			if (!pObject) continue;
 		}
 
 		bAddCard = TRUE;
	}

	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			if (!pObject) 
			{
				nIdx = m_pGrid_Parent->SelectGetNext();
				continue;
			}
		}

		bAddCard = TRUE;

		if (m_pDM_Parent->GetCellData(_T("레이블출력구분"), nIdx) != _T("1")) 
		{
			bAddCard = FALSE;
			nIdx = m_pGrid_Parent->SelectGetNext();
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), nIdx, strRegNo);
		
		if (!strRegNo.IsEmpty())
		{
			INT nRegCodeLength;
			nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
			for ( INT j = nRegCodeLength ; j < strRegNo.GetLength() ; j++ )
			{
				if( strRegNo.Mid(j, 1 ) != _T("0") )
				{
					strRegNo = strRegNo.Left(nRegCodeLength) + strRegNo.Mid ( j, strRegNo.GetLength()-j  );
					break;
				}			
			}						
		}            
		
		if (!IsValidRegNo(strRegNo)) 
		{
			bAddCard = FALSE;
			nIdx = m_pGrid_Parent->SelectGetNext();
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("청구기호"), nIdx, strCallNo);
		if (strCallNo.IsEmpty()) 
		{
			nIdx = m_pGrid_Parent->SelectGetNext();
			continue;
		}
		
		pObject->SetAt(0, strCallNo);
		pObject->SetAt(1, strRegNo, TRUE);

		nPrintCnt++;
		nIdx = m_pGrid_Parent->SelectGetNext();
	}

	return nPrintCnt;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_BarCode(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;

	pPrintMgr->MODE = _T("바코드");

	pPrintMgr->m_nBarcodeHeight = pPrintMgr->nLineHeight * 2;

	INT ids = -1;
	INT idx = -1;
	CString strLibName;
	CString strRegNo;
	CString strCallNo;

	m_pCM->GetControlMgrData(_T("도서관명"), strLibName);

	CSIReportPrintObject *pObject = NULL;
	INT i = 0;
	
	if(2 == m_bBarcodeUserno)
	{
		INT nMax = pInfo->m_nRowCount * pInfo->m_nColCount;
		for(i = 0; i < nMax; i++)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);

			pObject->SetAt(0, strLibName);
			CString strTmp;
			strTmp.Format(_T("#%s"), m_pInfo->LIB_CODE);
			pObject->SetAt(1, strTmp);
			strTmp.TrimLeft(_T("#"));
			pObject->SetAt(2, strTmp);
		}
	}
	else
	{
		i = m_pGrid_Parent->SelectGetHeadPosition();
		while (i >= 0)
		{
			idx = i;
			i = m_pGrid_Parent->SelectGetNext();

			pObject = pPrintMgr->AddCard(m_nMaxLine);
			if (!pObject) continue;

			if (--nStartPosition >= 0) 
			{
				i = m_pGrid_Parent->SelectGetHeadPosition();
				continue;
			}

			ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strRegNo);
			if (!IsValidRegNo(strRegNo)) continue;
							
			if (1 == m_bBarcodeUserno )
			{
				pObject->SetAt( 0, _T("대출자번호_바코드") );
				pObject->SetAt( 1, strRegNo );
				
				ids = m_pDM_Parent->GetCellData(_T("이용자명"), idx, strRegNo);
				pObject->SetAt( 2, strRegNo );
			}
			else if(2 == m_bBarcodeUserno )
			{
				pObject->SetAt(0, strLibName);
				if(5 == m_nOption_Print)
				{
					CString strTmp;
					strTmp.Format(_T("#%s"), m_pInfo->LIB_CODE);				
					pObject->SetAt(1, strTmp);				
					strTmp.TrimLeft(_T("#"));
					pObject->SetAt(2, strTmp);
				}
				else
				{
					pObject->SetAt(1, strRegNo);
					pObject->SetAt(2, strRegNo);
				}
			}
			else
			{		
				pObject->SetAt(0, strLibName);
				if(5 == m_nOption_Print)
				{
					CString strTmp;
					strTmp.Format(_T("$%s%s"), m_pInfo->LIB_CODE, strRegNo);				
					pObject->SetAt(1, strTmp);
					strTmp.TrimLeft(_T("$"));
					pObject->SetAt(2, strTmp);
				}
				else
				{
					pObject->SetAt(1, strRegNo);
					pObject->SetAt(2, strRegNo);
				}
			}
		}
	}

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_BookCard(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;

	return PrintEquip_Sticker(pInfo, pPrintMgr, nStartPosition);

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_Sticker(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;

	CSIReportPrintObject *pObject = NULL;
	CString strYN;
	CString strItemData;	
	CString strTemp;
	CString strTemp2;
	CString strBlank = _T("                                                  ");
	INT ids = -1;
	INT idx = -1;
	INT nLine = 0;
	INT nWidthCharCnt = 0;
	INT nOffset = 0;
	INT nCutPos = 0;
	INT nLinePos = 0;
	INT nLinePos2 = 0;
	INT i,j;
	BOOL bPrintRegNo = FALSE;
	
	i = m_pGrid_Parent->SelectGetHeadPosition();
	
	
	strYN.Empty();
	m_pDM_Parent->GetOneValueQuery(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND CLASS_ALIAS = '국방대'"), strYN );
	if ( strYN == _T("Y") )
	{
		CStdioFile fFile;
		if( fFile.Open( _T("..\\cfg\\국방대북카드.cfg") , CFile::modeRead | CFile::typeBinary ) )
		{
			TCHAR cUni;
			fFile.Read( &cUni, sizeof(TCHAR) );
			if( 0xFEFF != cUni )
			{
				fFile.SeekToBegin();
			}
			
			CString StrLength;
			fFile.ReadString(StrLength);
			fFile.Close();
			StrLength.TrimLeft(_T(" "));
			StrLength.TrimRight(_T(" "));
			
			nWidthCharCnt = _ttoi(StrLength.GetBuffer(0));
		}	
		else
		{
			nWidthCharCnt = 34;
		}
	}

	while (i >= 0)
	{
		idx = i;
		i = m_pGrid_Parent->SelectGetNext();

		nLine = 0;

		if ( strYN == _T("Y") ) 
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine+2);
		}
		else 
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
		}

		if (!pObject) continue;

		if (--nStartPosition >= 0) 
		{
			i = m_pGrid_Parent->SelectGetHeadPosition();
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strItemData);
		if (!IsValidRegNo(strItemData)) continue;		
						
		ids = m_pDM_Parent->GetCellData(_T("별치기호_코드"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("분류기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			if ( strYN == _T("Y") && nLine == 1 )
			{			
				strTemp = strItemData;
				ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strItemData);
				if (!strItemData.IsEmpty())
				{
					strTemp += strBlank.Left( nWidthCharCnt - (strTemp.GetLength()+strItemData.GetLength()) );
					strTemp	+= strItemData;
					pObject->SetAt(nLine++, strTemp);
				}
			}
			else
			{	
					pObject->SetAt(nLine++, strItemData);
			}
		}

		ids = m_pDM_Parent->GetCellData(_T("도서기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			if ( strYN == _T("Y") && nLine == 1 )
			{			
				strTemp = strItemData;
				ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strItemData);
				if (!strItemData.IsEmpty())
				{
					strTemp += strBlank.Left( nWidthCharCnt - (strTemp.GetLength()+strItemData.GetLength()) );
					strTemp	+= strItemData;
					pObject->SetAt(nLine++, strTemp);
				}
			}
			else
			{	
					pObject->SetAt(nLine++, strItemData);
			}
		}

		ids = m_pDM_Parent->GetCellData(_T("권책기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, strItemData);				

		ids = m_pDM_Parent->GetCellData(_T("복본기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			strItemData = _T("c.") +  strItemData ;
			pObject->SetAt(nLine++, strItemData);
		}

		if ( strYN != _T("Y") )
		{
			ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strItemData);
			if (!strItemData.IsEmpty())
				pObject->SetAt(nLine++, strItemData );
		}
			
		if ( strYN == _T("Y") ) 
		{
			strTemp = _T("");
			ids = m_pDM_Parent->GetCellData(_T("저작자"), idx, strItemData);
			if ( !strItemData.IsEmpty() )
			{			
				for ( j = nLine ; j <= 5 ; j++  )
				{
					pObject->SetAt(nLine++, _T(""));
				}
				
				nOffset = strItemData.GetLength() + 9;

				nLinePos = 0;
				if ( nOffset > nWidthCharCnt )
				{
					if ( !IsCanCut(strItemData, nWidthCharCnt-10) ) 
					{
						nLinePos = 1;
					}					
				}				
				
				nLinePos2 = 0;
				
				if ( nOffset > nWidthCharCnt && nOffset+9 > nWidthCharCnt*2 )
				{
					nCutPos = (nWidthCharCnt*2)-21-nLinePos;

					if ( !IsCanCut(strItemData, nCutPos-1 ) )
					{
						nLinePos2 = 1;
					}				
							
					strTemp.Empty();
					nCutPos = nCutPos - nLinePos2;
					
					if ( nLinePos2 ) strTemp2 = _T(" ...");					
					else strTemp2 = _T("...");					
					
					strTemp.Format(_T("%s%s"), strItemData.Left(nCutPos), strTemp2);
					strItemData = strTemp;
				}				
				
				if ( nOffset > nWidthCharCnt )
				{
					pObject->SetAt(nLine++, _T("저  자 : ") + strItemData.Left(nWidthCharCnt-9-nLinePos) );
					pObject->SetAt(nLine++, _T("         ") + strItemData.Mid(nWidthCharCnt-9-nLinePos) );
				}
				else
				{
					
					pObject->SetAt(nLine++, _T("저  자 : ") + strItemData);
					pObject->SetAt(nLine++, _T(""));
				}				
			}

			strTemp = _T("");
			ids = m_pDM_Parent->GetCellData(_T("본표제"), idx, strItemData);
			if ( !strItemData.IsEmpty() )
			{
				for ( j = nLine ; j <= 5 ; j++  )
				{
					pObject->SetAt(nLine++, _T(""));
				}
				
				nOffset = strItemData.GetLength() + 9;
				nLinePos = 0;
				if ( nOffset > nWidthCharCnt )
				{
					if ( !IsCanCut(strItemData, nWidthCharCnt-10) ) 
					{
						nLinePos = 1;
					}
				}
				
				nLinePos2 = 0;							
				if ( nOffset > nWidthCharCnt && nOffset+9 > nWidthCharCnt*2 )
				{
					nCutPos = (nWidthCharCnt*2)-21-nLinePos;
					if ( !IsCanCut(strItemData, nCutPos-1 ) )
					{
						nLinePos2 = 1;
					}				
								
					strTemp.Empty();
					nCutPos = nCutPos - nLinePos2;
					
					if ( nLinePos2 ) strTemp2 = _T(" ...");					
					else strTemp2 = _T("...");					
					
					strTemp.Format(_T("%s%s"), strItemData.Left(nCutPos), strTemp2);
					strItemData = strTemp;
				}				
				
				if ( nOffset > nWidthCharCnt )
				{
					pObject->SetAt(nLine++, _T("서  명 : ") + strItemData.Left(nWidthCharCnt-9-nLinePos) );
					pObject->SetAt(nLine++, _T("         ") + strItemData.Mid(nWidthCharCnt-9-nLinePos) );
				}
				else
				{
					
					pObject->SetAt(nLine++, _T("서  명 : ") + strItemData);
					pObject->SetAt(nLine++, _T(""));
				}
			}
		}
		else
		{
			strTemp = _T("");
			ids = m_pDM_Parent->GetCellData(_T("본표제"), idx, strItemData);
			if ( !strItemData.IsEmpty() )
			{
				pObject->SetAt(nLine++, _T("서  명 : ") + strItemData);				
			}	
			
			strTemp = _T("");
			ids = m_pDM_Parent->GetCellData(_T("저작자"), idx, strItemData);
			if ( !strItemData.IsEmpty() )
			{								
				pObject->SetAt(nLine++, _T("저  자 : ") + strItemData);			
			}
		}
	}
			

	return 0;

	EFS_END
	return -1;
}


BOOL CEquipPrintPropertyDlg::IsValidRegNo(CString &strRegNo)
{
	EFS_BEGIN

	UpdateData();
	CString strRegNoCode;
	m_pCM->GetControlMgrData(_T("등록구분"), strRegNoCode);
	
	CString strStart;
	CString strEnd;
	
	if (!m_strRegNoStart.IsEmpty())
	{
		strStart = m_pInfo->MakeRegNo(strRegNoCode, m_strRegNoStart);
	}

	if (!m_strRegNoEnd.IsEmpty())
	{
		strEnd = m_pInfo->MakeRegNo(strRegNoCode, m_strRegNoEnd);
	}
	if (strStart.IsEmpty() && !strEnd.IsEmpty())
		return TRUE;
	
	if (!strStart.IsEmpty() && strEnd.IsEmpty())
	{
		if (strRegNo != strStart) return FALSE;
	}
	else if (!strStart.IsEmpty() && !strEnd.IsEmpty())
	{
		if (strRegNo < strStart || strRegNo > strEnd)
			return FALSE;
	}
	
	return TRUE;

	EFS_END
	return FALSE;
}

BOOL CEquipPrintPropertyDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	CString strQuery = 
	_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND GROUP_3 = '공통' AND CLASS_ALIAS = '통합도서서비스' AND VALUE_NAME = '통합도서서비스' AND MANAGE_CODE = UDF_MANAGE_CODE");

	CESL_DataMgr DM;
	DM.SetCONNECTION_INFO(_T(""));

	CString strValue;
	DM.GetOneValueQuery(strQuery, strValue);

	if(_T("Y") == strValue)
	{
		GetDlgItem(IDC_rPRINT_BARCODE2)->ShowWindow(SW_SHOW);
	}

	CString sPathUnit = m_lpszFileName;

	FILE *fpUnit;
	TCHAR cLineUnit[256];
	fpUnit = _tfopen( sPathUnit , _T("rb") );
	if ( fpUnit == NULL ) {
		AfxMessageBox( CString( sPathUnit + _T("파일이 없습니다.")) );
		return FALSE;
	}	

	FILE* fpUni = fpUnit;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	
	CString sUnit;
	while( _fgetts( cLineUnit, 256, fpUnit) ) {
		CString sLine;
		sLine.Format( _T("%s"), cLineUnit );
		if ( sLine.Left(4) == _T("UNIT") ) {
			sUnit = sLine.Mid(7);
		}
	}
	fclose(fpUnit);

	CComboBox * pRegCode;	
	CString sRegCode;
	pRegCode=(CComboBox *)GetDlgItem(IDC_CobUnit);

	pRegCode->AddString(_T("픽셀(pixel)"));
	pRegCode->AddString(_T("밀리미터(mm)"));
	if ( sUnit == _T("밀리미터(mm)") ) {
		pRegCode->SetCurSel(0);
		m_sUnit = _T("밀리미터(mm)");
	}
	else {
		pRegCode->SetCurSel(1);
		m_sUnit = _T("픽셀(pixel)");
	}

	if (InitESL_Mgr(_T("SM_정리_출력_장비_속성")) < 0) 
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error!!");
		goto ERR;
	}

	if (Init() < 0) goto ERR;
	
	if ( 1 == m_bBarcodeUserno ) 
	{
		SetUserNoBarcodeScreen();
	}
	else if( 2 == m_bBarcodeUserno)
	{
		( (CButton*) GetDlgItem (IDC_rPRINT_LINE	) )->SetCheck(FALSE);
		( (CButton*) GetDlgItem (IDC_rPRINT_SERIAL	) )->SetCheck(FALSE);
		( (CButton*) GetDlgItem (IDC_rPRINT_BARCODE	) )->SetCheck(TRUE);
		( (CButton*) GetDlgItem (IDC_rPRINT_BOOKCARD) )->SetCheck(FALSE);
		( (CButton*) GetDlgItem (IDC_rPRINT_STICKER	) )->SetCheck(FALSE);
		OnPrintFormatChange();
	}

	EnableThemeDialogTexture(GetSafeHwnd());


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

void CEquipPrintPropertyDlg::OnSelchangecREGNOPLACE() 
{
	OnPrintFormatChange();
}

void CEquipPrintPropertyDlg::OnPrintFormatChange()
{
	EFS_BEGIN

	UpdateData();

	GetDlgItem(IDC_cREG_NO_PLACE)->ShowWindow(SW_SHOW);

	CWnd *pWndTBarCode = GetDlgItem(IDC_tMARGIN_BARCODE);
	CWnd *pWndEBarCode = GetDlgItem(IDC_eMARGIN_BARCODE);

	CEdit *pWndELibName = (CEdit*)GetDlgItem(IDC_eLIB_NAME_CP);

	pWndTBarCode->ShowWindow(SW_HIDE);
	pWndEBarCode->ShowWindow(SW_HIDE);
	pWndELibName->SetReadOnly(TRUE);

	GetDlgItem(IDC_eLENGTH_COUNT)->EnableWindow(TRUE);

	CString strRegNoPlace;
	m_pCM->GetControlMgrData(_T("등록번호위치"), strRegNoPlace);

	if ( m_nOption_Print == 1 && strRegNoPlace == _T("레이블안쪽") )
	{
		( (CComboBox*) GetDlgItem (IDC_cREG_NO_PLACE) )->SetCurSel(0);
		strRegNoPlace = _T("용지오른쪽");
	}

	if (m_nOption_Print == 0)
	{
		m_strPrintFormatKey = _T("LABEL_LINE");

		GetDlgItem(IDC_cREG_NO_PLACE)->ShowWindow(SW_SHOW);
		
		CString strRegNoPlace;
		m_pCM->GetControlMgrData(_T("등록번호위치"), strRegNoPlace);
		if (strRegNoPlace == _T("용지오른쪽"))
			m_strPrintFormatKey = _T("LABEL_LINE_NORMAL");
		else if (strRegNoPlace == _T("레이블사이"))
			m_strPrintFormatKey = _T("LABEL_LINE_EXTRA");
		else if (strRegNoPlace == _T("레이블안쪽"))
			m_strPrintFormatKey = _T("LABEL_LINE_IN");
	}
	else if (m_nOption_Print == 1)
	{
		GetDlgItem(IDC_cREG_NO_PLACE)->ShowWindow(SW_SHOW);
		
		CString strRegNoPlace;
		m_pCM->GetControlMgrData(_T("등록번호위치"), strRegNoPlace);
		if (strRegNoPlace == _T("용지오른쪽")) {
			m_strPrintFormatKey = _T("LABEL_SERIAL_NORMAL");
			GetDlgItem(IDC_eLENGTH_COUNT)->SetWindowText(_T("1"));
			GetDlgItem(IDC_eLENGTH_COUNT)->EnableWindow(FALSE);
		}
		else if (strRegNoPlace == _T("레이블사이"))
			m_strPrintFormatKey = _T("LABEL_SERIAL_EXTRA");
	}
	else if (m_nOption_Print == 2)
	{
		m_strPrintFormatKey = _T("BARCODE");
		pWndTBarCode->ShowWindow(SW_SHOW);
		pWndEBarCode->ShowWindow(SW_SHOW);
		pWndELibName->SetReadOnly(FALSE);
	}
	else if(m_nOption_Print == 5)
	{
		m_strPrintFormatKey = _T("BARCODE2");
		pWndTBarCode->ShowWindow(SW_SHOW);
		pWndEBarCode->ShowWindow(SW_SHOW);
		pWndELibName->SetReadOnly(FALSE);		
	}
	else if (m_nOption_Print == 3)
	{
		m_strPrintFormatKey = _T("BOOKCARD");
	}
	else if (m_nOption_Print == 4)
	{
		m_strPrintFormatKey = _T("STICKER");
	}

	DisplayPrintPropertyInfo();

	EFS_END
}

void CEquipPrintPropertyDlg::OnbPRINTEQUIP() 
{
	EFS_BEGIN

	CPrintPropertyInfo *pInfo = NULL;

	INT nRowIdx;
	INT nRowCnt;
	INT i;
	BOOL bChkData;
	CString strPrintFormat;
	bChkData = FALSE;
		
	if(2 != m_bBarcodeUserno)
	{
		nRowCnt = m_pDM_Parent->GetRowCount();
		m_pGrid_Parent->SelectMakeList();
		nRowCnt = m_pGrid_Parent->SelectGetCount();
		nRowIdx = m_pGrid_Parent->SelectGetHeadPosition();
	}

	if ( m_nOption_Print == 0 )
	{
		for( i = 0 ; i < nRowCnt ; i++ )
		{
			m_pDM_Parent->GetCellData(_T("레이블출력구분"), nRowIdx, strPrintFormat );
			if( strPrintFormat == _T("2"))
			{
				bChkData = TRUE;
				break;
			}

			nRowIdx = m_pGrid_Parent->SelectGetNext();
		}

		if( bChkData == FALSE )
		{
			AfxMessageBox(_T("선정된 자료 중 줄바꿔찍기 할 자료가 없습니다."));
			return ;
		}
	}
	else if ( m_nOption_Print == 1 )
	{
		for( i = 0 ; i < nRowCnt ; i++ )
		{
			m_pDM_Parent->GetCellData(_T("레이블출력구분"), nRowIdx, strPrintFormat );

			if( strPrintFormat == _T("1"))
			{
				bChkData = TRUE;
				break;
			}

			nRowIdx = m_pGrid_Parent->SelectGetNext();
		}

		if( bChkData == FALSE )
		{
			AfxMessageBox(_T("선정된 자료 중 이어찍기 할 자료가 없습니다."));
			return ;
		}
	}
	pInfo = ApplyPrintPropertyInfo();
	if (pInfo == NULL) return;
	
	PrintEquip(pInfo);

	EFS_END
}

void CEquipPrintPropertyDlg::OnbSAVECONFIG() 
{
	EFS_BEGIN

	ApplyPrintPropertyInfo();
	SavePrintPropertyInfo();
	SaveSelectInfo();

	AfxMessageBox(_T("저장되었습니다."));

	EFS_END
}

void CEquipPrintPropertyDlg::OnbFONT() 
{
	EFS_BEGIN

	CPrintPropertyInfo *pInfo = NULL;
	pInfo = ApplyPrintPropertyInfo();
	if (pInfo == NULL) return;

	LOGFONT logFont;
	ZeroMemory(&logFont,sizeof(logFont));
	logFont.lfCharSet = DEFAULT_CHARSET;	
	_tcscpy(logFont.lfFaceName, pInfo->m_strFontFace);
	logFont.lfHeight = pInfo->m_nFontHeight;
	logFont.lfWeight = pInfo->m_nFontStyle;

	CFontDialog FontDialog(&logFont);
	if (FontDialog.DoModal() != IDOK)
		return;

	FontDialog.GetCurrentFont(&logFont);
	pInfo->m_nFontSize = FontDialog.GetSize() / 10;
	pInfo->m_nFontHeight = logFont.lfHeight;
	pInfo->m_strFontFace = logFont.lfFaceName;
	pInfo->m_nFontStyle = logFont.lfWeight;	
	

	DisplayPrintPropertyInfo();

	EFS_END
}

void CEquipPrintPropertyDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();

	delete m_pSIReportManager;
	m_pSIReportManager = NULL;

	FreePrintPropertyInfo();	

	EFS_END
}

void CEquipPrintPropertyDlg::OnSelendokCobUnit() 
{
	// TODO: Add your control notification handler code here
	CString sUnit;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CobUnit);	
	pCombo->GetLBText( pCombo->GetCurSel() , sUnit );

	CString strKey;
	CString strLine;
	CPrintPropertyInfo *pInfo = NULL;
	POSITION pos = m_mapPrintPropertyInfo.GetStartPosition();

	TCHAR* pEndPtr = NULL;

	if ( sUnit != m_sUnit ) {
		if ( sUnit == _T("밀리미터(mm)") ) {
			while (pos)
			{
				m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (void*&)pInfo);
				if (!pInfo) continue;				
				pInfo->m_nMarginUp = pInfo->m_nMarginUp * 3/10;
				pInfo->m_nMarginSide = pInfo->m_nMarginSide * 3/10;
				pInfo->m_nSpaceVert = pInfo->m_nSpaceVert * 3/10;
				pInfo->m_nSpaceHorz = pInfo->m_nSpaceHorz * 3/10;
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 3/10;
				pInfo->m_nLabelWidth = pInfo->m_nLabelWidth * 3/10;
			}					

			CString sValue;
			GetDlgItem(IDC_eMARGIN_UP)->GetWindowText(sValue);

			DOUBLE dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr) * 3/10;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eMARGIN_UP)->SetWindowText(sValue);

			GetDlgItem(IDC_eMARGIN_SIDE)->GetWindowText(sValue);

			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr)* 3/10;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eMARGIN_SIDE)->SetWindowText(sValue);

			GetDlgItem(IDC_eSPACE_VERT)->GetWindowText(sValue);

			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr)* 3/10;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eSPACE_VERT)->SetWindowText(sValue);

			GetDlgItem(IDC_eSPACE_HORZ)->GetWindowText(sValue);

			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr)* 3/10;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eSPACE_HORZ)->SetWindowText(sValue);

			GetDlgItem(IDC_eLABEL_HIGHT)->GetWindowText(sValue);
	
			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr)* 3/10;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eLABEL_HIGHT)->SetWindowText(sValue);

			GetDlgItem(IDC_eLABEL_WIDTH)->GetWindowText(sValue);

			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr)* 3/10;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eLABEL_WIDTH)->SetWindowText(sValue);

		}
		if ( sUnit == _T("픽셀(pixel)") ) {
			while (pos)
			{
				m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (void*&)pInfo);
				if (!pInfo) continue;				
				pInfo->m_nMarginUp = pInfo->m_nMarginUp * 10/3;
				pInfo->m_nMarginSide = pInfo->m_nMarginSide * 10/3;
				pInfo->m_nSpaceVert = pInfo->m_nSpaceVert * 10/3;
				pInfo->m_nSpaceHorz = pInfo->m_nSpaceHorz * 10/3;
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
				pInfo->m_nLabelWidth = pInfo->m_nLabelWidth * 10/3;
			}

			CString sValue;
			GetDlgItem(IDC_eMARGIN_UP)->GetWindowText(sValue);
	
			DOUBLE dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr) * 10/3;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eMARGIN_UP)->SetWindowText(sValue);

			GetDlgItem(IDC_eMARGIN_SIDE)->GetWindowText(sValue);

			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr) * 10/3;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eMARGIN_SIDE)->SetWindowText(sValue);

			GetDlgItem(IDC_eSPACE_VERT)->GetWindowText(sValue);

			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr) * 10/3;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eSPACE_VERT)->SetWindowText(sValue);

			GetDlgItem(IDC_eSPACE_HORZ)->GetWindowText(sValue);

			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr) * 10/3;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eSPACE_HORZ)->SetWindowText(sValue);

			GetDlgItem(IDC_eLABEL_HIGHT)->GetWindowText(sValue);

			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr) * 10/3;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eLABEL_HIGHT)->SetWindowText(sValue);

			GetDlgItem(IDC_eLABEL_WIDTH)->GetWindowText(sValue);
	
			dValue = _tcstod(sValue.GetBuffer(0), &pEndPtr) * 10/3;

			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eLABEL_WIDTH)->SetWindowText(sValue);
		}
	}
	m_sUnit = sUnit;
}

void CEquipPrintPropertyDlg::OnBtnSerialLabelConfig() 
{
	// TODO: Add your control notification handler code here
	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_rPRINT_SERIAL);
	if ( pSerial->GetCheck() == 1 ) {
		CBO_SERIAL_NORMAL_CONFIG dlg(this);
		dlg.DoModal();				
	}
	else {		
		CBO_CAT_SERIAL_LABEL_CONFIG dlg(this);
		dlg.DoModal();
	}
}

void CEquipPrintPropertyDlg::OnChkSerial() 
{
	// TODO: Add your control notification handler code here
	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_CHK_SERIAL);
	if ( pSerial->GetCheck() == 1 ) {
		pSerial->SetCheck(1);
	}
	else
	{
		pSerial->SetCheck(0);
	}
}

INT CEquipPrintPropertyDlg::SaveSelectInfo()
{
	EFS_BEGIN

	CStdioFile file;
	CString strFileName;	
	
	if ( 1 == m_bBarcodeUserno )
	{
		strFileName = _T("..\\CFG\\대출번호바코드_장비_선택.cfg");
	}
	else if( 2 == m_bBarcodeUserno )
	{
		strFileName = _T("..\\CFG\\기관부호_장비_선택.cfg");
	}
	else
	{
		strFileName = _T("..\\CFG\\정리_장비_선택.cfg");
	}
	if (!file.Open(strFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
	{
		m_strLastErrMsg.Format(_T("[%s] 파일을 열 수 없습니다."), strFileName);
		return -1;
	}
	
	if( 0 == file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		file.Write( &cUni, sizeof(TCHAR));
	}
		
	CString strPaperType;
	CString strUseSerialPaper;	
	CString strPrintType;
	CString strRegNoPos;	
	CString strLine;
	BOOL	bCheck;
	INT		nIdx;

	bCheck = ( (CButton*) GetDlgItem (IDC_rPAPER_WIDTH) )->GetCheck();
	if ( bCheck == TRUE ) strPaperType = _T("가로");
	else strPaperType = _T("세로");

	bCheck = ( (CButton*) GetDlgItem (IDC_CHK_SERIAL) )->GetCheck();
	if ( bCheck == TRUE ) strUseSerialPaper = _T("사용함");
	else strUseSerialPaper = _T("사용안함");

	bCheck = ( (CButton*) GetDlgItem (IDC_rPRINT_LINE) )->GetCheck();
	if ( bCheck == TRUE ) strPrintType = _T("줄바꿔찍기");
	
	bCheck = ( (CButton*) GetDlgItem (IDC_rPRINT_SERIAL) )->GetCheck();
	if ( bCheck == TRUE ) strPrintType = _T("이어찍기");

	bCheck = ( (CButton*) GetDlgItem (IDC_rPRINT_BARCODE) )->GetCheck();
	if ( bCheck == TRUE ) strPrintType = _T("바코드");

	bCheck = ( (CButton*) GetDlgItem (IDC_rPRINT_BOOKCARD) )->GetCheck();
	if ( bCheck == TRUE ) strPrintType = _T("북카드");

	bCheck = ( (CButton*) GetDlgItem (IDC_rPRINT_STICKER) )->GetCheck();
	if ( bCheck == TRUE ) strPrintType = _T("스티커");

	nIdx = ( (CComboBox*) GetDlgItem (IDC_cREG_NO_PLACE) )->GetCurSel();
	if ( nIdx == 0 ) strRegNoPos = _T("용지오른쪽");
	else if ( nIdx == 1 ) strRegNoPos = _T("레이블사이");
	else if ( nIdx == 2 ) strRegNoPos = _T("레이블안쪽");

	strLine.Format(_T("# 장비출력 선택항목 설정\r\n")
				   _T("용지방향	 = %s\r\n")
				   _T("# (가로, 세로)\r\n")
		           _T("연속용지	 = %s\r\n")		
				   _T("#  (사용함, 사용안함)\r\n")
				   _T("출력종류	 = %s\r\n")
				   _T("# (줄바꿔찍기, 이어찍기, 바코드, 북카드, 스티커)\r\n")
				   _T("등록번호위치 = %s\r\n")
				   _T("# (용지오른쪽, 레이블사이, 레이블안쪽)\r\n")
				   ,strPaperType, strUseSerialPaper, strPrintType, strRegNoPos );	
		
	file.WriteString(strLine);
	
	file.Close();
	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::LoadSelectInfo()
{
	EFS_BEGIN

	CStdioFile	file;
	CString		strFileName;
	CString		strPaperType;
	CString		strUseSerialPaper;
	CString		strPrintType;
	CString		strRegNoPos;
	CString		strLine;
	CString		sTemp;
	INT			idx;

	if ( 1 == m_bBarcodeUserno )
	{
		strFileName = _T("..\\CFG\\대출번호바코드_장비_선택.cfg");
	}
	else if( 2 == m_bBarcodeUserno )
	{
		strFileName = _T("..\\CFG\\기관부호_장비_선택.cfg");
	}
	else
	{
		strFileName = _T("..\\CFG\\정리_장비_선택.cfg");	
	}

	if(!file.Open (strFileName,CFile::modeRead | CFile::typeBinary) ) 
	{
		m_strLastErrMsg.Format(_T("[%s] 파일을 열 수 없습니다."), strFileName);
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	while(file.ReadString (strLine))
	{
		strLine.TrimLeft();
		if (strLine.Left(1) == _T("#") || strLine.IsEmpty()) continue;		
		if (strLine.Left(1) == _T("[") || strLine.IsEmpty()) continue;		

		idx = strLine.Find(_T("="));
		if (0 <= idx)
		{
			sTemp = strLine.Left(idx);
			sTemp.TrimLeft();
			sTemp.TrimRight();
			if (sTemp == _T("용지방향")) 
			{					
				strPaperType = strLine.Mid(idx+1);
				strPaperType.TrimLeft();
				strPaperType.TrimRight();
			} 
			if (sTemp == _T("연속용지")) 
			{					
				strUseSerialPaper = strLine.Mid(idx+1);
				strUseSerialPaper.TrimLeft();
				strUseSerialPaper.TrimRight();
			} 
			if (sTemp == _T("출력종류")) 
			{	
				strPrintType = strLine.Mid(idx+1);
				strPrintType.TrimLeft();
				strPrintType.TrimRight();
			} 
			if (sTemp == _T("등록번호위치")) 
			{
				strRegNoPos = strLine.Mid(idx+1);
				strRegNoPos.TrimLeft();
				strRegNoPos.TrimRight();
			} 			
		}
	}
	file.Close () ;	

	if ( strPaperType == _T("가로") ) 
	{
		( (CButton*) GetDlgItem (IDC_rPAPER_WIDTH) )->SetCheck(TRUE);
		( (CButton*) GetDlgItem (IDC_rPAPER_LENGTH) )->SetCheck(FALSE);
	}
	else
	{
		( (CButton*) GetDlgItem (IDC_rPAPER_WIDTH) )->SetCheck(FALSE);
		( (CButton*) GetDlgItem (IDC_rPAPER_LENGTH) )->SetCheck(TRUE);
	}

	if ( strUseSerialPaper == _T("사용함") ) 
	{
		( (CButton*) GetDlgItem (IDC_CHK_SERIAL) )->SetCheck(TRUE);
	}
	else
	{
		( (CButton*) GetDlgItem (IDC_CHK_SERIAL) )->SetCheck(FALSE);
	}

	( (CButton*) GetDlgItem (IDC_rPRINT_LINE	) )->SetCheck(FALSE);
	( (CButton*) GetDlgItem (IDC_rPRINT_SERIAL	) )->SetCheck(FALSE);
	( (CButton*) GetDlgItem (IDC_rPRINT_BARCODE	) )->SetCheck(FALSE);
	( (CButton*) GetDlgItem (IDC_rPRINT_BOOKCARD) )->SetCheck(FALSE);
	( (CButton*) GetDlgItem (IDC_rPRINT_STICKER	) )->SetCheck(FALSE);

	if ( strPrintType == _T("줄바꿔찍기") )
	{
		( (CButton*) GetDlgItem (IDC_rPRINT_LINE	) )->SetCheck(TRUE);
	}
	else if ( strPrintType == _T("이어찍기") )
	{
		( (CButton*) GetDlgItem (IDC_rPRINT_SERIAL	) )->SetCheck(TRUE);
	}
	else if ( strPrintType == _T("바코드") )
	{
		( (CButton*) GetDlgItem (IDC_rPRINT_BARCODE	) )->SetCheck(TRUE);
	}
	else if ( strPrintType == _T("북카드") )
	{
		( (CButton*) GetDlgItem (IDC_rPRINT_BOOKCARD) )->SetCheck(TRUE);
	}
	else if ( strPrintType == _T("스티커") )
	{
		( (CButton*) GetDlgItem (IDC_rPRINT_STICKER	) )->SetCheck(TRUE);
	}
	else 
	{
		( (CButton*) GetDlgItem (IDC_rPRINT_LINE	) )->SetCheck(TRUE);
	}
	

	idx = 0;
	if ( strRegNoPos == _T("용지오른쪽")) idx = 0;
	else if ( strRegNoPos == _T("레이블사이")) idx = 1;
	else if ( strRegNoPos == _T("레이블안쪽")) idx = 2;

	( (CComboBox*) GetDlgItem (IDC_cREG_NO_PLACE) )->SetCurSel(idx);

	return 0;

	EFS_END
	return -1;
}

BOOL CEquipPrintPropertyDlg::IsCanCut( CString str, INT pos )
{
	INT i, nLastPos = -1;
	if( str.GetLength() < pos+1 )
		return FALSE;
	
	TCHAR ch = str.GetAt( pos );
	
	if( pos + 1 == str.GetLength() || ( ch & ( 0x80 ) ) != 128 )
		return TRUE;
	
	if( !pos )
		if( ( ch & ( 0x80 ) ) != 128 )
			return TRUE;
		else
			return FALSE;

	for( i = 0; i < pos; i++ )
	{
		ch = str.GetAt( i );
		
		if( ( ch & ( 0x80 ) ) != 128 )
			nLastPos = i;
	}
	
	if( nLastPos == -1 && pos % 2 == 1 )
		return TRUE;
	
	if( nLastPos == -1 && pos % 2 == 0 )
		return TRUE;
	
	bool result = !( ( pos - nLastPos ) % 2 );
	return result;
}

INT CEquipPrintPropertyDlg::SetUserNoBarcodeScreen()
{
	( (CButton*) GetDlgItem (IDC_rPRINT_LINE	) )->SetCheck(FALSE);
	( (CButton*) GetDlgItem (IDC_rPRINT_SERIAL	) )->SetCheck(FALSE);
	( (CButton*) GetDlgItem (IDC_rPRINT_BARCODE	) )->SetCheck(TRUE);
	( (CButton*) GetDlgItem (IDC_rPRINT_BOOKCARD) )->SetCheck(FALSE);
	( (CButton*) GetDlgItem (IDC_rPRINT_STICKER	) )->SetCheck(FALSE);

	OnPrintFormatChange();	
	m_nOption_Print;
	return 0;
}
HBRUSH CEquipPrintPropertyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
