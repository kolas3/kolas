// EquipPrintPropertyDlg.cpp : implementation file

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
#include "Barcod.h"

#define MAXBUFFERSIZE 250
#define strError _T("GetPrintName Error")
LPTSTR GetPrnName();              //Get Printer Name
LPTSTR szPrinterName = NULL;

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyDlg dialog

CEquipPrintPropertyDlg::CEquipPrintPropertyDlg(CESL_Mgr* pParent /*=NULL*/)
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

	m_lpszFileName = _T("..\\CFG\\정리_장비_출력.cfg");
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
	ON_CONTROL_RANGE(BN_CLICKED, IDC_rPRINT_LINE, IDC_rPRINT_STICKER, OnPrintFormatChange )
	ON_BN_CLICKED(IDC_bFONT, OnbFONT)
	ON_CBN_SELCHANGE(IDC_cREG_NO_PLACE, OnSelchangecREGNOPLACE)
	ON_CBN_SELENDOK(IDC_CobUnit, OnSelendokCobUnit)
	ON_BN_CLICKED(IDC_BtnSerialLabelConfig, OnBtnSerialLabelConfig)
	ON_BN_CLICKED(IDC_CHK_SERIAL, OnChkSerial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyDlg Operations

INT CEquipPrintPropertyDlg::Init()
{
	EFS_BEGIN

	if (!m_pDM_Parent || !m_pGrid_Parent) return -1;

	// 전정된 정보가 있나 보자
	m_pGrid_Parent->SelectMakeList();
	if (m_pGrid_Parent->SelectGetCount() <= 0)
	{
		m_strLastErrMsg = _T("선정된 정보가 없습니다.");
		return -1;
	}

	if (m_pSIReportManager)
		delete m_pSIReportManager;
	m_pSIReportManager = new CSIReportManager(this);

	FreePrintPropertyInfo();

	m_pCM = FindCM(_T("CM_정리_출력_장비_속성"));
	if (!m_pCM) return -1;

	INT ids = -1;

	// 설정파일을 읽어온다.
	ids = LoadPrintPropertyInfo();
	if (ids < 0) return ids;

	// 도서관명 가져오기
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

	// 설정 값 가져오기
	CString strManageValue;
	CString strQuery = _T("SELECT VALUE_NAME FROM ESL_MANAGE_TBL ")
			           _T("WHERE GROUP_1 = 'L' AND GROUP_2 = '기타' AND CLASS_ALIAS = '관리구분별도서관설정'");

	INT ids = dm.GetOneValueQuery(strQuery, strManageValue);
	if (ids < 0) return -1;

	CString strFieldAttr;
	for (INT idx = 0; idx < nFieldCount; idx++)
	{
		if (ArrFieldAttr[idx].IsEmpty()) continue;

		strFieldAttr += ArrFieldAttr[idx];
		if (idx != nFieldCount - 1)
			strFieldAttr += _T(", ");
	}

	CString strManageCode;
	if (strManageValue == _T("Y"))
	{
		strQuery.Format(_T("SELECT MANAGE_CODE FROM MN_USER_TBL WHERE REC_KEY = %s"), this->m_pInfo->USER_PK);
		ids = dm.GetOneValueQuery(strQuery, strManageCode);
		if (ids >= 0)
		{
			if (strManageCode.IsEmpty())
				strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"), strFieldAttr, strManageCode);
			else
				strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE = '%s'"), strFieldAttr, strManageCode);

			ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
		}
	}
	else
	{
		strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE IS NULL"), strFieldAttr);
		INT ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
	}

	return 0;
}

// Printer Name을 얻는 함수
LPTSTR GetPrnName()
{
	OSVERSIONINFO osv;
    TCHAR cBuffer[MAXBUFFERSIZE];
	PRINTER_INFO_2 *lpInfo = NULL;
    DWORD dwSizeNeeded;
	DWORD dwNumItems;
	DWORD dwItem;
	LPTSTR _strResult;

	// Windows 버전 체크
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	
	// Windows 95 or 98의 경우 EnumPrinters를 사용
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
		m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (VOID*&)pInfo);
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
		_stscanf(strLine, _T("%s\t=\t%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%s"),
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
			pInfo->m_strFontFace.GetBuffer(MAX_PATH)
			);
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
		m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (VOID*&)pInfo);
		if (!pInfo) continue;

		strLine.Format(_T("%s\t=\t%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%4d%s\r\n"),
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
			pInfo->m_strFontFace
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

	// 찾아서 없으면 생성한다.
	if (!m_mapPrintPropertyInfo.Lookup(m_strPrintFormatKey, (VOID*&)pInfo))
	{
		pInfo = new CPrintPropertyInfo;
		m_mapPrintPropertyInfo.SetAt(m_strPrintFormatKey, pInfo);
	}

	if (!pInfo) return -1;

	// 프로퍼티를 가져와서 보여준다.
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

	// 폰트
	CString strFont;
	strFont.Format(_T("%s, %d"), pInfo->m_strFontFace, pInfo->m_nFontSize);
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

	// 찾아서 없으면 생성한다.
	if (!m_mapPrintPropertyInfo.Lookup(m_strPrintFormatKey, (VOID*&)pInfo))
	{
		pInfo = new CPrintPropertyInfo;
		m_mapPrintPropertyInfo.SetAt(m_strPrintFormatKey, pInfo);
	}

	// 값을 가져와 세팅한다.
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

	CString sUnit;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CobUnit);	
	pCombo->GetLBText( pCombo->GetCurSel() , sUnit );

	if ( sUnit == _T("밀리미터(mm)") ) {
		// 위쪽여백
		PrintMgr.nStartY = pInfo->m_nMarginUp * 10/3;

		// 양쪽여백
		PrintMgr.nStartX = pInfo->m_nMarginSide * 10/3;

		// 세로간격
		PrintMgr.nTableHeightGap = pInfo->m_nSpaceVert * 10/3;

		// 가로간격
		PrintMgr.nTableWidthGap = pInfo->m_nSpaceHorz * 10/3;

		// 레이블너비
		PrintMgr.nTableWidth = pInfo->m_nLabelWidth * 10/3;	
	}
	else {
		// 위쪽여백
		PrintMgr.nStartY = pInfo->m_nMarginUp;

		// 양쪽여백
		PrintMgr.nStartX = pInfo->m_nMarginSide;

		// 세로간격
		PrintMgr.nTableHeightGap = pInfo->m_nSpaceVert;

		// 가로간격
		PrintMgr.nTableWidthGap = pInfo->m_nSpaceHorz;

		// 레이블너비
		PrintMgr.nTableWidth = pInfo->m_nLabelWidth;
	}

	// 바코드여백
	PrintMgr.m_nBarcodeWidthGap  = pInfo->m_nMarginBarCode;

	// 가로개수
	PrintMgr.m_nRows = pInfo->m_nRowCount;
	
	// 세로개수
	PrintMgr.m_nCols = pInfo->m_nColCount;
	
	// 폰트
	PrintMgr.SetFont(pInfo->m_nFontSize, pInfo->m_strFontFace);

	// 용지 가로 / 세로
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

	// 출력 시작 (공백의 개수가 있으면 그만큼 넘어가자)
	INT nStartPos = 0;
	CString strStartPos;
	m_pCM->GetControlMgrData(_T("시작위치"), strStartPos);
	
	if (!strStartPos.IsEmpty())
	{
		nStartPos = _ttoi(strStartPos);
		nStartPos--;
	}
	
	INT iPrintYesNo = 0;	// 실제출력 여부 리턴(-1:출력 개수가 없다.)

	// 레이블 - 줄바꿔찍기
	if (m_nOption_Print == 0)
	{
		// 등록번호를 어디에 찍을지 결정
		CString strRegNoPlace;
		m_pCM->GetControlMgrData(_T("등록번호위치"), strRegNoPlace);
		if (strRegNoPlace == _T("용지오른쪽"))
		{
			m_nMaxLine = 6;
			double dMaxLine = 6.0;			
			double dTmp;
			if ( sUnit == _T("밀리미터(mm)") ) {
				dTmp = pInfo->m_nLabelHight * 10/3;
			}
			else {
				dTmp = pInfo->m_nLabelHight;
			}
			PrintMgr.nLineHeight = dTmp / dMaxLine;

			// col 수를 1 늘려준다.
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
			PrintEquip_LineExtra(pInfo, &PrintMgr, nStartPos);
		}
	}
	// 이어찍기
	else if (m_nOption_Print == 1)
	{
		// 등록번호를 어디에 찍을지 결정
		CString strRegNoPlace;
		m_pCM->GetControlMgrData(_T("등록번호위치"), strRegNoPlace);
		if (strRegNoPlace == _T("용지오른쪽"))
		{
			m_nMaxLine = PrintMgr.m_nCols;
			if ( sUnit == _T("밀리미터(mm)") ) {
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
			}
			PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;

			// col 수를 1 늘려준다.
			PrintMgr.m_nCols++;

			// 출력
			PrintEquip_SerialNormal(pInfo, &PrintMgr, nStartPos);
		}
		else if (strRegNoPlace == _T("레이블사이"))
		{
			m_nMaxLine = 2;
			if ( sUnit == _T("밀리미터(mm)") ) {
				pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
			}
			PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
			PrintEquip_SerialExtra(pInfo, &PrintMgr, nStartPos);
		}
	}
	// 바코드
	else if (m_nOption_Print == 2)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//기표원일경우 ZEBRA프린트로 출력할 수 있도록 수정.. ㅠㅠ;;
		CString strValue;
		GetManageValue( _T("기타") , _T("공통") , _T("기표원") , _T("기표원") , strValue );
		if ( strValue == _T("Y") )
		{
			PrintEquip_BarCode_KiPyo( nStartPos );
			return 0;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		m_nMaxLine = 4;
		if ( sUnit == _T("밀리미터(mm)") ) {
			pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
		}
		PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
		PrintEquip_BarCode(pInfo, &PrintMgr, nStartPos);
	}
	// 북카드
	else if (m_nOption_Print == 3)
	{
		m_nMaxLine = 8;
		if ( sUnit == _T("밀리미터(mm)") ) {
			pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
		}
		PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
		PrintEquip_BookCard(pInfo, &PrintMgr, nStartPos);
	}
	// 스티커
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
		// 출력물 생성
		m_pSIReportManager->FreeWorkForm();
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
	// 청구기호를 줄바꿔 찍는다
	
	// 별치기호 코드 + CRLF
	// 분류기호 + CRLF
	// 도서기호 + CRLF
	// 권책기호 + CRLF
	// 복본기호 + CRLF
	// 등록번호 + CRLF

	INT ids = -1;
	INT idx = -1;
	INT nCol = 0;
	INT nLine = 0;
	BOOL bAddCard = TRUE;
	CString strItemData;
	CString strRegNo;
	CString strRegNoField;
	CSIReportPrintObject *pObject = NULL;
	INT iTotalPrintCount;
	iTotalPrintCount = m_pDM_Parent->GetRowCount();
	INT iRealPrintCount = 0;	// 실제 출력 개수
	
	INT iLast = 0;
	INT iPrintCount = 0;
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
	INT iStart = 0;
	while (i >= 0)
	{
		
		idx = i;
		 
		if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1))  == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) 
			idx = iTotalPrintCount-1;				

		if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1)) == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1))))
			break;

		// 선정된 것만 출력
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

		// 등록번호를 찍을 곳인지 보자
		if (nCol % pPrintMgr->m_nCols == 0)		
		{
			//if ( pPrintMgr->m_nCols != 3 )
				//i = m_pGrid_Parent->SelectGetPrev();

			if ( iLast == 0 && i == -1 && pPrintMgr->m_nCols == 2) {
				i = nCol/2;
				iLast = 1;
			}

			// 레이블 출력시 왼쪽 등록번호 위치 조절
			INT n = 0;
			strRegNoField.TrimRight();
			if (strRegNoField.IsEmpty()) continue;
			// 그동안 모았던 등록번호를 찍자
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
				// 레이블 출력시 왼쪽 등록번호 위치 조절
				i = m_pGrid_Parent->SelectGetHeadPosition();

				if (nStartPosition > 0) 
				{	
					strRegNoField = _T(" |") + strRegNoField;
				}
				continue;
			}

			// 레이블이 출력구분 체크
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
			if (!IsValidRegNo(strRegNo)) 
			{
				bAddCard = FALSE;
				continue;
			}

			// 실제 출력 개수
			iRealPrintCount++;

// 소정 수정 2004.10.07 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			ids = m_pDM_Parent->GetCellData(_T("청구기호"), idx, strItemData);

			CString strTmp = strItemData;
			while ( strItemData.Find(_T("\r\n"),0) != -1 )
			{
				ids = strItemData.Find(_T("\r\n"),0);
				strTmp = strItemData.Mid(0, ids);
				strItemData = strItemData.Mid(ids+2);
				pObject->SetAt(nLine++, strTmp );
			}
			if ( strItemData != _T("") )
			{
				pObject->SetAt(nLine++, strItemData );
			}

//			ids = m_pDM_Parent->GetCellData(_T("별치기호_코드"), idx, strItemData);
//			if (!strItemData.IsEmpty())
//			{
//				// 별치기호는 설명으로 바꾼다.
//				m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
//				pObject->SetAt(nLine++, strItemData);
//			}
//			
//			ids = m_pDM_Parent->GetCellData(_T("분류기호"), idx, strItemData);
//			if (!strItemData.IsEmpty())
//				pObject->SetAt(nLine++, strItemData);
//			
//			ids = m_pDM_Parent->GetCellData(_T("도서기호"), idx, strItemData);
//			if (!strItemData.IsEmpty())
//				pObject->SetAt(nLine++, strItemData);
//
//			ids = m_pDM_Parent->GetCellData(_T("권책기호"), idx, strItemData);
//			if (!strItemData.IsEmpty())
//				pObject->SetAt(nLine++, strItemData);
//
//			ids = m_pDM_Parent->GetCellData(_T("복본기호"), idx, strItemData);
//			if (!strItemData.IsEmpty())
//				pObject->SetAt(nLine++, _T("c.") + strItemData);
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

			// 레이블 출력시 왼쪽 등록번호 위치 조절 
			if (nCol % pPrintMgr->m_nCols != 1) {
				strRegNoField += _T(" |") + strRegNo;
			}
			else {
				strRegNoField = strRegNo;
			}
		}
	}

	// 실제 찍을 개수가 없으면 리턴
	if ( iRealPrintCount == 0 ) return -1;
	
	// 등록번호 찍을것이 남아있으면 찍는다.
	// 두번째 이상부터
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
				// 그동안 모았던 등록번호를 찍자
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
		//컴퓨터 이름
		GetComputerName(szComName, &len);
		//프린터 이름
		szPrinterName = GetPrnName();
		CString strNameFormat;
		strNameFormat.Format( _T("net use LPT1 \\\\%s"), szComName);

		ids = WinExec(strNameFormat, SW_HIDE);
		if (ids <= 31) {
			AfxMessageBox(_T("Error!"));
			return -2;
		}
		Sleep(400);
		FILE* prn = NULL;
		prn = _tfopen(_T("LPT1"), _T("w"));
		if(prn == NULL)
		{
			AfxMessageBox(_T("프린트 연결을 확인하세요!"));
			return -2;
		}
		fseek(prn, 0, SEEK_END);		
		if( 0 == ftell(prn) )
		{
			/*UNCHANGE*/fputc(0xFF, prn);
			/*UNCHANGE*/fputc(0xFE, prn);
		}
		Sleep(300);

		//파일읽어오기
		CString sPathSerialConfig = _T("..\\CFG\\정리_연속용지_출력.cfg");		
		FILE *fpSerialConfig;
		TCHAR cLineConfig[256];
		fpSerialConfig = _tfopen( sPathSerialConfig , _T("rb") );		
		if ( fpSerialConfig == NULL ) {
			AfxMessageBox( _T("연속용지 설정을 저장하세요!") );
			return -3;
		}
		//++++++++++++++++++++++++++++++++++++++++++
		//Unicode일땐 2byte건너띄기
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
		//------------------------------------------

		INT iColCount = 2;
		INT iLineCount = 5;
		INT iFirstLoc = 10;			
		INT iSecLoc = 30;			
		INT iRegLoc = 60;
		INT iColSpace = 1;
		INT iFirstLineSpaceYN = 1;
		INT iSerialFont = -1;

		_ftprintf( prn, _T("%c%c0"), 27, 114);

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
					_ftprintf( prn, _T("%c%c0"), 28, 107 );
				}
				else if ( sItem == _T("한글고딕체10.5p") ) {
					iSerialFont = 1;
					_ftprintf( prn, _T("%c%c1"), 28, 107 );
				}
				else if ( sItem == _T("한글명조체10.5p") ) {
					iSerialFont = 2;
					_ftprintf( prn, _T("%c%c2"), 28, 107 );
				}
				else if ( sItem == _T("한글고딕체14p") ) {
					iSerialFont = 3;
					_ftprintf( prn, _T("%c%c3"), 28, 107 );
				}
				else if ( sItem == _T("한글샘물체10.5p") ) {
					iSerialFont = 6;
					_ftprintf( prn, _T("%c%c6"), 28, 107 );
				}
				else if ( sItem == _T("한글샘물체14p") ) {
					iSerialFont = 7;
					_ftprintf( prn, _T("%c%c7"), 28, 107 );
				}
				else if ( sItem == _T("한글궁서체10.5p") ) {
					iSerialFont = 9;
					_ftprintf( prn, _T("%c%c9"), 28, 107 );
				}
				else if ( sItem == _T("한글필기체10.5p") ) {
					iSerialFont = 10;
					_ftprintf( prn, _T("%c%cA"), 28, 107 );
				}
				else if ( sItem == _T("한글디나루체10.5p") ) {
					iSerialFont = 11;
					_ftprintf( prn, _T("%c%cB"), 28, 107 );
				}
				else if ( sItem == _T("한글옛체10.5p") ) {
					iSerialFont = 12;
					_ftprintf( prn, _T("%c%cC"), 28, 107 );
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
						while ( sItem.GetLength() < iSecLoc ) {
							sItem = sItem + _T(" ");
						}
					}
					if ( (iColNo % (iColCount+1)) == 1 ) {
						while ( sItem.GetLength() < iRegLoc ) {
							sItem = sItem + _T(" ");
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
			_ftprintf( prn, sPreView );
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
	// 청구기호를 줄바꿔 찍는다
	
	// 별치기호 코드 + CRLF
	// 분류기호 + CRLF
	// 도서기호 + CRLF
	// 권책기호 + CRLF
	// 복본기호 + CRLF
	// 등록번호 + CRLF

	INT ids = -1;
	INT idx = -1;
	INT nLine = 0;
	BOOL bAddCard = TRUE;
	CString strItemData;
	CSIReportPrintObject *pObject = NULL;
		
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

		// 레이블이 출력구분 체크
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

// 소정 수정 2005.01.20 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		ids = m_pDM_Parent->GetCellData(_T("청구기호"), idx, strItemData);

			CString strTmp = strItemData;
			while ( strItemData.Find(_T("\r\n"),0) != -1 )
			{
				ids = strItemData.Find(_T("\r\n"),0);
				strTmp = strItemData.Mid(0, ids);
				strItemData = strItemData.Mid(ids+2);
				pObject->SetAt(nLine++, strTmp );
			}
			if ( strItemData != _T("") )
			{
				pObject->SetAt(nLine++, strItemData );
			}

/*		ids = m_pDM_Parent->GetCellData(_T("별치기호_코드"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			// 별치기호는 설명으로 바꾼다.
			m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호"), strItemData, strItemData);
			pObject->SetAt(nLine++, strItemData);
		}
		
		ids = m_pDM_Parent->GetCellData(_T("분류기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("도서기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, strItemData);

		ids = m_pDM_Parent->GetCellData(_T("권책기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, strItemData);

		ids = m_pDM_Parent->GetCellData(_T("복본기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, _T("c.") + strItemData);
*/
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(6, strItemData, TRUE);
	}

	return 0;

	EFS_END
	return -1;
}

INT CEquipPrintPropertyDlg::PrintEquip_SerialNormal(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

		if (pPrintMgr == NULL) return -1;
	// 청구기호를 이어서 찍는다.
	// 별치기호코드 분류기호-도서기호-권책기호=복본기호

	// 마지지막 col에 등록번호를 찍는다.
	INT iStartPosOri = nStartPosition;

	INT ids = -1;
	INT idx = -1;
	INT nCol = 0;
	INT nLine = 0;
	BOOL bAddCard = TRUE;
	CString strRegNo;
	CString strRegNoField;
	CString strCallNo;
	CSIReportPrintObject *pObject = NULL;
	INT iTotalPrintCount;
	iTotalPrintCount = m_pDM_Parent->GetRowCount();
	INT iRealPrintCount = 0;	// 실제 출력 개수

	INT iLast = 0;
	
	INT iPrintCount = 0;
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
	INT iStart = 0;
	while (i >= 0)
	{
		
		idx = i;
		 
		if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1))  == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) 
			idx = iTotalPrintCount-1;				

		if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1)) == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1))))
			break;

		// 선정된 것만 출력
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

		// 등록번호를 찍을 곳인지 보자
		if (nCol % pPrintMgr->m_nCols == 0)		
		{
			i = m_pGrid_Parent->SelectGetPrev();
			if ( iLast == 0 && i == -1 && pPrintMgr->m_nCols == 2) {
				i = nCol/2;
				iLast = 1;
			}

			// 레이블 출력시 왼쪽 등록번호 위치 조절
			INT n = 0;
			strRegNoField.TrimRight();
			if (strRegNoField.IsEmpty()) continue;
			// 그동안 모았던 등록번호를 찍자
			LPTSTR lpszToken = _tcstok(strRegNoField.GetBuffer(0), _T("|"));
			while (lpszToken)
			{
				pObject->SetAt(n++, lpszToken);
				lpszToken = _tcstok(NULL, _T("|"));
			}
			strRegNoField.Empty();

			//i = m_pGrid_Parent->SelectGetPrev();
			
			if ( i == -1 ) {
				i = m_pGrid_Parent->SelectGetTailPosition();
			}

		}
		else
		{			
			if (--nStartPosition >= 0) 
			{
				i = m_pGrid_Parent->SelectGetHeadPosition();
				continue;
			}

			// 레이블이 출력구분 체크
			if (m_pDM_Parent->GetCellData(_T("레이블출력구분"), idx) != _T("1")) 
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
	if (iRowCount <= 1 ) {
		AfxMessageBox( _T("출력할 자료가 없습니다.") );
		return -1;
	}

	strRegNoField.TrimRight();
	// 등록번호 찍을것이 남아있으면 찍는다.
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
				// 그동안 모았던 등록번호를 찍자
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
		//컴퓨터 이름
		GetComputerName(szComName, &len);
		//프린터 이름
		szPrinterName = GetPrnName();
		CString strNameFormat;
		strNameFormat.Format( _T("net use LPT1 \\\\%s"), szComName);

		ids = WinExec(strNameFormat, SW_HIDE);
		if (ids <= 31) {
			AfxMessageBox(_T("Error!"));
			return -1;
		}
		Sleep(400);
		FILE* prn = _tfopen(_T("LPT1"), _T("wb"));		
		if(prn == NULL)
		{
			AfxMessageBox(_T("프린트 연결을 확인하세요!"));
			return -2;
		}
		fseek(prn, 0, SEEK_END);		
		if( 0 == ftell(prn) )
		{
			/*UNCHANGE*/fputc(0xFF, prn);
			/*UNCHANGE*/fputc(0xFE, prn);
		}
		Sleep(300);

		//파일읽어오기
		CString sPathSerialConfig = _T("..\\CFG\\정리_연속용지_이어찍기_출력.cfg");		
		FILE *fpSerialConfig;
		TCHAR cLineConfig[256];
		fpSerialConfig = _tfopen( sPathSerialConfig , _T("rb") );		
		if ( fpSerialConfig == NULL ) {
			AfxMessageBox( _T("연속용지 이어찍기 설정을 저장하세요!") );
			return -3;
		}
		//++++++++++++++++++++++++++++++++++++++++++
		//Unicode일땐 2byte건너띄기
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
		//------------------------------------------	

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
					while ( sOneLine.GetLength() < iSecLoc ) {
						sOneLine = sOneLine + _T(" ");
					}
				}
				if ( (i+1) % 3 == 2 ) {
					while ( sOneLine.GetLength() < iRegLoc ) {
						sOneLine = sOneLine + _T(" ");
					}
				}

				if ( (i+1) % 3 == 0 ) {
					sItem = pPrintMgr->GetAt(i)->LINE[1];
					CString sSpace;
					while ( sSpace.GetLength() < iRegLoc ) {
						sSpace = sSpace + _T(" ");
					}
					sPreView = sPreView + sOneLine + _T("\r\n") + sSpace + sItem + _T("\r\n\r\n");
					sOneLine = _T("");
				}
			}
		}
		else {
			for( INT i = 0 ; i < iRowCount ; i++ ) {
				//청구기호
				CString sItem;
				ids = m_pDM_Parent->GetCellData(_T("청구기호"), i, sItem);
								
				
				CString sSpace;
				while ( sSpace.GetLength() < iSecLoc ) {
					sSpace = _T(" ") + sSpace;
				}
				sItem = sSpace + sItem;
				while ( sItem.GetLength() < iRegLoc ) {
					sItem = sItem + _T(" ");
				}
				sPreView = sPreView + sItem;
								
				//등록번호
				ids = m_pDM_Parent->GetCellData(_T("등록번호"), i, sItem);

				while ( sSpace.GetLength() < iSecLoc ) {
					sSpace = _T(" ") + sSpace;
				}
				sItem = sSpace + sItem;
				while ( sItem.GetLength() < iRegLoc ) {
					sItem = sItem + _T(" ");
				}

				sPreView = sPreView + sItem + _T("\r\n\r\n");
			}
			sPreView.TrimRight();
		}
		CSerialLabelPreview dlg(this);
		dlg.m_sPreview = sPreView;
		if ( dlg.DoModal() == IDOK ) {
			_ftprintf( prn, sPreView );
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
	// 청구기호를 이어서 찍는다.
	// 별치기호코드 분류기호-도서기호-권책기호=복본기호

	INT ids = -1;
	INT idx = -1;
	BOOL bAddCard = TRUE;
	CString strRegNo;
	CString strCallNo;
	CSIReportPrintObject *pObject = NULL;
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
	while (i >= 0)
	{
		idx = i;
		i = m_pGrid_Parent->SelectGetNext();

		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			if (!pObject) continue;
		}

		bAddCard = TRUE;

		if (--nStartPosition >= 0) 
		{
			i = m_pGrid_Parent->SelectGetHeadPosition();
			continue;
		}

		// 레이블이 출력구분 체크
		if (m_pDM_Parent->GetCellData(_T("레이블출력구분"), idx) != _T("1")) 
		{
			bAddCard = FALSE;
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strRegNo);
		if (!IsValidRegNo(strRegNo)) 
		{
			bAddCard = FALSE;
			continue;
		}

		ids = m_pDM_Parent->GetCellData(_T("청구기호"), idx, strCallNo);
		if (strCallNo.IsEmpty()) continue;
		
		pObject->SetAt(0, strCallNo);
		pObject->SetAt(1, strRegNo, TRUE);
	}


	return 0;

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
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
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

		pObject->SetAt(0, strLibName);
		pObject->SetAt(1, strRegNo);
		pObject->SetAt(2, strRegNo);

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

	//pPrintMgr->m_bFrawOuterLine = TRUE;
	
	// 별치기호 코드 + CRLF
	// 분류기호 + CRLF
	// 도서기호 + CRLF
	// 권책기호 + CRLF
	// 복본기호 + CRLF
	// 등록번호 + CRLF
	// 서명
	// 저작자

	INT ids = -1;
	INT idx = -1;
	INT nLine = 0;
	CString strItemData;
	CSIReportPrintObject *pObject = NULL;
	INT i = m_pGrid_Parent->SelectGetHeadPosition();
	while (i >= 0)
	{
		idx = i;
		i = m_pGrid_Parent->SelectGetNext();

		nLine = 0;

		pObject = pPrintMgr->AddCard(m_nMaxLine);
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
			pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("도서기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, strItemData);

		ids = m_pDM_Parent->GetCellData(_T("권책기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, strItemData);

		ids = m_pDM_Parent->GetCellData(_T("복본기호"), idx, strItemData);
		if (!strItemData.IsEmpty())
		{
			strItemData = _T("c.") +  strItemData ;
			pObject->SetAt(nLine++, strItemData);
		}

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, strItemData);

		ids = m_pDM_Parent->GetCellData(_T("본표제"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, _T("서  명 : ") + strItemData);

		ids = m_pDM_Parent->GetCellData(_T("저작자"), idx, strItemData);
		if (!strItemData.IsEmpty())
			pObject->SetAt(nLine++, _T("저  자 : ") + strItemData);
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
		strStart.Format(_T("%s%010s"), strRegNoCode, m_strRegNoStart);

	if (!m_strRegNoEnd.IsEmpty())
		strEnd.Format(_T("%s%010s"), strRegNoCode, m_strRegNoEnd);

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

/////////////////////////////////////////////////////////////////////////////
// CEquipPrintPropertyDlg message handlers

BOOL CEquipPrintPropertyDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();


	//CString sPathUnit = _T("..\\CFG\\정리_장비_출력.cfg");
	CString sPathUnit = m_lpszFileName;
	//파일읽어오기
	FILE *fpUnit;
	TCHAR cLineUnit[256];
	fpUnit = _tfopen( sPathUnit , _T("rb") );
	if ( fpUnit == NULL ) {
		AfxMessageBox( _T("..\\CFG\\정리_장비_출력.cfg 파일이 없습니다.") );
		return FALSE;
	}	
	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
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
	//------------------------------------------
			
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
	
	return TRUE;  
ERR:
	if (!m_strLastErrMsg.IsEmpty())
		AfxMessageBox(m_strLastErrMsg);
	EndDialog(IDCANCEL);
	return FALSE;

	EFS_END
	return FALSE;
}

VOID CEquipPrintPropertyDlg::OnSelchangecREGNOPLACE() 
{
	OnPrintFormatChange();
}

VOID CEquipPrintPropertyDlg::OnPrintFormatChange()
{
	EFS_BEGIN

	UpdateData();

	// 등록번호 위치를 숨긴다.
	GetDlgItem(IDC_cREG_NO_PLACE)->ShowWindow(SW_SHOW);

	// 바코드 마진 콘트롤 보여주기 / 숨기기
	CWnd *pWndTBarCode = GetDlgItem(IDC_tMARGIN_BARCODE);
	CWnd *pWndEBarCode = GetDlgItem(IDC_eMARGIN_BARCODE);

	CEdit *pWndELibName = (CEdit*)GetDlgItem(IDC_eLIB_NAME_CP);

	pWndTBarCode->ShowWindow(SW_HIDE);
	pWndEBarCode->ShowWindow(SW_HIDE);
	pWndELibName->SetReadOnly(TRUE);

	GetDlgItem(IDC_eLENGTH_COUNT)->EnableWindow(TRUE);

	// 레이블 - 줄바꿔찍기
	if (m_nOption_Print == 0)
	{
		m_strPrintFormatKey = _T("LABEL_LINE");
		// 등록번호 위치를 보여준다.
		GetDlgItem(IDC_cREG_NO_PLACE)->ShowWindow(SW_SHOW);
		
		// 등록번호를 어디에 찍을지 결정
		CString strRegNoPlace;
		m_pCM->GetControlMgrData(_T("등록번호위치"), strRegNoPlace);
		if (strRegNoPlace == _T("용지오른쪽"))
			m_strPrintFormatKey = _T("LABEL_LINE_NORMAL");
		else if (strRegNoPlace == _T("레이블사이"))
			m_strPrintFormatKey = _T("LABEL_LINE_EXTRA");
	}
	// 레이블 - 이어찍기
	else if (m_nOption_Print == 1)
	{
		// 등록번호 위치를 보여준다.
		GetDlgItem(IDC_cREG_NO_PLACE)->ShowWindow(SW_SHOW);
		
		// 등록번호를 어디에 찍을지 결정
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
	// 바코드
	else if (m_nOption_Print == 2)
	{
		m_strPrintFormatKey = _T("BARCODE");
		pWndTBarCode->ShowWindow(SW_SHOW);
		pWndEBarCode->ShowWindow(SW_SHOW);
		pWndELibName->SetReadOnly(FALSE);
	}
	// 북카드
	else if (m_nOption_Print == 3)
	{
		m_strPrintFormatKey = _T("BOOKCARD");
	}
	// 스티커
	else if (m_nOption_Print == 4)
	{
		m_strPrintFormatKey = _T("STICKER");
	}

	DisplayPrintPropertyInfo();

	EFS_END
}

VOID CEquipPrintPropertyDlg::OnbPRINTEQUIP() 
{
	EFS_BEGIN

	CPrintPropertyInfo *pInfo = NULL;
	pInfo = ApplyPrintPropertyInfo();
	if (pInfo == NULL) return;
	
	PrintEquip(pInfo);

	EFS_END
}
VOID CEquipPrintPropertyDlg::OnbSAVECONFIG() 
{
	EFS_BEGIN

	ApplyPrintPropertyInfo();
	SavePrintPropertyInfo();

	EFS_END
}

VOID CEquipPrintPropertyDlg::OnbFONT() 
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

	CFontDialog FontDialog(&logFont);
	if (FontDialog.DoModal() != IDOK)
		return;

	// 수정된 값을 설정한다.

	FontDialog.GetCurrentFont(&logFont);
	pInfo->m_nFontSize = FontDialog.GetSize() / 10;
	pInfo->m_nFontHeight = logFont.lfHeight;
	pInfo->m_strFontFace = logFont.lfFaceName;
	

	DisplayPrintPropertyInfo();

	EFS_END
}

VOID CEquipPrintPropertyDlg::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();

	delete m_pSIReportManager;
	m_pSIReportManager = NULL;

	FreePrintPropertyInfo();	

	EFS_END
}

VOID CEquipPrintPropertyDlg::OnSelendokCobUnit() 
{
	CString sUnit;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CobUnit);	
	pCombo->GetLBText( pCombo->GetCurSel() , sUnit );

	CString strKey;
	CString strLine;
	CPrintPropertyInfo *pInfo = NULL;
	POSITION pos = m_mapPrintPropertyInfo.GetStartPosition();

	if ( sUnit != m_sUnit ) {
		if ( sUnit == _T("밀리미터(mm)") ) {
			while (pos)
			{
				m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (VOID*&)pInfo);
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
			double dValue = atof( sValue ) * 3/10;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eMARGIN_UP)->SetWindowText(sValue);

			GetDlgItem(IDC_eMARGIN_SIDE)->GetWindowText(sValue);
			dValue = atof( sValue ) * 3/10;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eMARGIN_SIDE)->SetWindowText(sValue);

			GetDlgItem(IDC_eSPACE_VERT)->GetWindowText(sValue);
			dValue = atof( sValue ) * 3/10;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eSPACE_VERT)->SetWindowText(sValue);

			GetDlgItem(IDC_eSPACE_HORZ)->GetWindowText(sValue);
			dValue = atof( sValue ) * 3/10;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eSPACE_HORZ)->SetWindowText(sValue);

			GetDlgItem(IDC_eLABEL_HIGHT)->GetWindowText(sValue);
			dValue = atof( sValue ) * 3/10;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eLABEL_HIGHT)->SetWindowText(sValue);

			GetDlgItem(IDC_eLABEL_WIDTH)->GetWindowText(sValue);
			dValue = atof( sValue ) * 3/10;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eLABEL_WIDTH)->SetWindowText(sValue);

		}
		if ( sUnit == _T("픽셀(pixel)") ) {
			while (pos)
			{
				m_mapPrintPropertyInfo.GetNextAssoc(pos, strKey, (VOID*&)pInfo);
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
			double dValue = atof( sValue ) * 10/3;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eMARGIN_UP)->SetWindowText(sValue);

			GetDlgItem(IDC_eMARGIN_SIDE)->GetWindowText(sValue);
			dValue = atof( sValue ) * 10/3;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eMARGIN_SIDE)->SetWindowText(sValue);

			GetDlgItem(IDC_eSPACE_VERT)->GetWindowText(sValue);
			dValue = atof( sValue ) * 10/3;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eSPACE_VERT)->SetWindowText(sValue);

			GetDlgItem(IDC_eSPACE_HORZ)->GetWindowText(sValue);
			dValue = atof( sValue ) * 10/3;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eSPACE_HORZ)->SetWindowText(sValue);

			GetDlgItem(IDC_eLABEL_HIGHT)->GetWindowText(sValue);
			dValue = atof( sValue ) * 10/3;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eLABEL_HIGHT)->SetWindowText(sValue);

			GetDlgItem(IDC_eLABEL_WIDTH)->GetWindowText(sValue);
			dValue = atof( sValue ) * 10/3;	
			sValue.Format( _T("%0.f"), dValue );
			GetDlgItem(IDC_eLABEL_WIDTH)->SetWindowText(sValue);
		}
	}
	m_sUnit = sUnit;
}

VOID CEquipPrintPropertyDlg::OnBtnSerialLabelConfig() 
{
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

VOID CEquipPrintPropertyDlg::OnChkSerial() 
{
	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_CHK_SERIAL);
	if ( pSerial->GetCheck() == 1 ) {
	}
}

VOID CEquipPrintPropertyDlg::PrintEquip_BarCode_KiPyo( INT nStartPos )
{
	//ZEBRA 프린트로 출력하도록 설정.. (기표원만 적용)

	INT ids = -1;
	INT idx = -1;
	INT j;
	//등록번호
	TCHAR        AccessionNo[32];
	CDC         dc;
	DOCINFO     di;
	CBarcod *pBarcod1 = ( CBarcod * )GetDlgItem( IDC_BARCOD1 );
	LONG        s_AccessionNo;

	CPrintDialog dlg(FALSE,  PD_ALLPAGES | PD_NOSELECTION | PD_NOPAGENUMS );
	if(dlg.DoModal() != IDOK) return ;

	//프린트 작업
	CBitmap bitmap;

	dc.Attach(dlg.GetPrinterDC());
	dc.m_bPrinting = TRUE;
	wmemset (&di, 0, sizeof( DOCINFO) );
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = _T("낱장바코드출력");
	if ( dc.StartDoc( &di ) == SP_ERROR ) return ;


	TCHAR s_SpaceBuff[64];
	wmemset( s_SpaceBuff , ' ' , 63);
	s_SpaceBuff[63] = '\0';

	CPrintInfo Info;
	Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));
	
	//도서관이름 BitMap
	bitmap.LoadBitmap(IDB_BITMAP1);
	CSize bitmapSize;
	BITMAP bmInfo;
	bitmap.GetObject(sizeof(bmInfo), &bmInfo);


	INT nGridIdx = m_pGrid_Parent->SelectGetHeadPosition();
	while (nGridIdx >= 0)
	{
		idx = nGridIdx;

		if (--nStartPos >= 0) 
		{
			nGridIdx = m_pGrid_Parent->SelectGetHeadPosition();
			continue;
		}

		dc.StartPage();
		PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), -25, 0 );

		CString sTmp;
		ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, sTmp );
		//if (!IsValidRegNo(AccessionNo)) continue;
		_tcscpy( AccessionNo, sTmp.GetBuffer(0) );
		s_AccessionNo = _ttol( &AccessionNo[2] );

		pBarcod1->SetCaption( AccessionNo );
		pBarcod1->SetPrinterLeft  ( 60 );
		pBarcod1->SetPrinterTop   ( 80  );
		pBarcod1->SetPrinterWidth ( 450 );
		pBarcod1->SetPrinterHeight( 100 );
		pBarcod1->SetPrinterHDC   ( (long)( dc.GetSafeHdc( ) ) );
		
		if( _tcsncmp( AccessionNo , _T("WM") , 2 ) == 0 ){
			CBitmap bitmap;
			bitmap.LoadBitmap(IDB_B_WM);
			CSize bitmapSize;
			BITMAP bmInfo;
			bitmap.GetObject(sizeof(bmInfo), &bmInfo);			
			PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), 100, 190 );
			for( j = 2; j < _tcsclen(AccessionNo) ; j++ ){

				CBitmap bitmap;

				switch( AccessionNo[j] ){
					case '1':						
						bitmap.LoadBitmap(IDB_B1);
						break;
					case '2':						
						bitmap.LoadBitmap(IDB_B2);
						break;
					case '3':						
						bitmap.LoadBitmap(IDB_B3);
						break;
					case '4':						
						bitmap.LoadBitmap(IDB_B4);
						break;
					case '5':						
						bitmap.LoadBitmap(IDB_B5);
						break;
					case '6':						
						bitmap.LoadBitmap(IDB_B6);
						break;
					case '7':
						bitmap.LoadBitmap(IDB_B7);
						break;
					case '8':
						bitmap.LoadBitmap(IDB_B8);
						break;
					case '9':
						bitmap.LoadBitmap(IDB_B9);
						break;
					case '0':
						bitmap.LoadBitmap(IDB_B10);
						break;
				}
				CSize bitmapSize;
				BITMAP bmInfo;
				bitmap.GetObject(sizeof(bmInfo), &bmInfo);
				PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), 120+j*20, 190 );
			}
			nGridIdx = m_pGrid_Parent->SelectGetNext();
		}else
		if( _tcsncmp( AccessionNo , _T("AT") , 2 ) == 0 ){
			CBitmap bitmap;
			bitmap.LoadBitmap(IDB_B_AT);
			CSize bitmapSize;
			BITMAP bmInfo;
			bitmap.GetObject(sizeof(bmInfo), &bmInfo);			
			PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), 100, 190 );
			for( j = 2; j < _tcsclen(AccessionNo) ; j++ ){

				CBitmap bitmap;

				switch( AccessionNo[j] ){
					case '1':						
						bitmap.LoadBitmap(IDB_B1);
						break;
					case '2':						
						bitmap.LoadBitmap(IDB_B2);
						break;
					case '3':						
						bitmap.LoadBitmap(IDB_B3);
						break;
					case '4':						
						bitmap.LoadBitmap(IDB_B4);
						break;
					case '5':						
						bitmap.LoadBitmap(IDB_B5);
						break;
					case '6':						
						bitmap.LoadBitmap(IDB_B6);
						break;
					case '7':
						bitmap.LoadBitmap(IDB_B7);
						break;
					case '8':
						bitmap.LoadBitmap(IDB_B8);
						break;
					case '9':
						bitmap.LoadBitmap(IDB_B9);
						break;
					case '0':
						bitmap.LoadBitmap(IDB_B10);
						break;
				}
				CSize bitmapSize;
				BITMAP bmInfo;
				bitmap.GetObject(sizeof(bmInfo), &bmInfo);			
				PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), 120+j*20, 190 );
			}
			nGridIdx = m_pGrid_Parent->SelectGetNext();
		}
		else
		if( _tcsncmp( AccessionNo , _T("SM") , 2 ) == 0 ){
			CBitmap bitmap;
			bitmap.LoadBitmap(IDB_B_SM);
			CSize bitmapSize;
			BITMAP bmInfo;
			bitmap.GetObject(sizeof(bmInfo), &bmInfo);			
			PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), 100, 190 );
			for( j = 2; j < _tcsclen(AccessionNo) ; j++ ){

				CBitmap bitmap;

				switch( AccessionNo[j] ){
					case '1':						
						bitmap.LoadBitmap(IDB_B1);
						break;
					case '2':						
						bitmap.LoadBitmap(IDB_B2);
						break;
					case '3':						
						bitmap.LoadBitmap(IDB_B3);
						break;
					case '4':						
						bitmap.LoadBitmap(IDB_B4);
						break;
					case '5':						
						bitmap.LoadBitmap(IDB_B5);
						break;
					case '6':						
						bitmap.LoadBitmap(IDB_B6);
						break;
					case '7':
						bitmap.LoadBitmap(IDB_B7);
						break;
					case '8':
						bitmap.LoadBitmap(IDB_B8);
						break;
					case '9':
						bitmap.LoadBitmap(IDB_B9);
						break;
					case '0':
						bitmap.LoadBitmap(IDB_B10);
						break;
				}
				CSize bitmapSize;
				BITMAP bmInfo;
				bitmap.GetObject(sizeof(bmInfo), &bmInfo);			
				PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), 120+j*20, 190 );
			}
			nGridIdx = m_pGrid_Parent->SelectGetNext();
		}
		else
		if( _tcsncmp( AccessionNo , _T("EM") , 2 ) == 0 ){
			CBitmap bitmap;
			bitmap.LoadBitmap(IDB_B_EM);
			CSize bitmapSize;
			BITMAP bmInfo;
			bitmap.GetObject(sizeof(bmInfo), &bmInfo);			
			PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), 100, 190 );
			for( j = 2; j < _tcsclen(AccessionNo) ; j++ ){

				CBitmap bitmap;

				switch( AccessionNo[j] ){
					case '1':						
						bitmap.LoadBitmap(IDB_B1);
						break;
					case '2':						
						bitmap.LoadBitmap(IDB_B2);
						break;
					case '3':						
						bitmap.LoadBitmap(IDB_B3);
						break;
					case '4':						
						bitmap.LoadBitmap(IDB_B4);
						break;
					case '5':						
						bitmap.LoadBitmap(IDB_B5);
						break;
					case '6':						
						bitmap.LoadBitmap(IDB_B6);
						break;
					case '7':
						bitmap.LoadBitmap(IDB_B7);
						break;
					case '8':
						bitmap.LoadBitmap(IDB_B8);
						break;
					case '9':
						bitmap.LoadBitmap(IDB_B9);
						break;
					case '0':
						bitmap.LoadBitmap(IDB_B10);
						break;
				}
				CSize bitmapSize;
				BITMAP bmInfo;
				bitmap.GetObject(sizeof(bmInfo), &bmInfo);			
				PrintDIB( dc.m_hDC, ImageToDIB( &dc, &bitmap ), 120+j*20, 190 );
			}

			nGridIdx = m_pGrid_Parent->SelectGetNext();
	
		}

		if ( dc.EndPage() <= 0 ) {				
			dc.AbortDoc();
			dc.DeleteDC();
			dc.Detach();
			delete dc;
			return ;
		}

	}

	dc.EndDoc();	
	dc.DeleteDC();
	dc.Detach();
	delete dc;

}



















































BOOL CEquipPrintPropertyDlg::PrintDIB( HDC hDC, HANDLE hDib, INT xOrigin, INT yOrigin ) 
{
    // Forget it!!!
    if( 0 == hDib )
    {
        return FALSE;
    }

    INT                 iBits           = 0; 
    WORD                dwPaletteSize   = 0;
    LPBITMAPINFOHEADER  lpDIB           = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
    LPBITMAPINFOHEADER  lpbi            = lpDIB;
    LPBITMAPCOREHEADER  lpbc            = (LPBITMAPCOREHEADER)lpbi;

    //  With the BITMAPINFO format headers, the size of the palette
    //  is in biClrUsed. However, in the BITMAPCORE-style headers, it
    //  depends on the bits per pixel ( = 2 raised to the power of
    //  bits/pixel).
    if( lpbi->biSize != sizeof(BITMAPCOREHEADER) )
    {
       iBits = lpbi->biBitCount;
    }
    else
    {
       iBits = lpbc->bcBitCount;
    }

    if( ( lpbi->biSize != sizeof(BITMAPCOREHEADER) ) &&
        ( 0 != lpbi->biClrUsed ) )
    {
        dwPaletteSize = (WORD)lpbi->biClrUsed;
    }
    else
    {
        switch( iBits )
        {
            case 1:
                dwPaletteSize = 2;
                break;
            case 4:
                dwPaletteSize = 16;
                break;
            case 8:
                dwPaletteSize = 256;
                break;
            default:
                // A 24 bitcount DIB has no color table
                dwPaletteSize = 0;
                break;
        }
    }

    if( lpbi->biSize == sizeof(BITMAPCOREHEADER) )
       dwPaletteSize *= sizeof(RGBTRIPLE);
    else
       dwPaletteSize *= sizeof(RGBQUAD);

    LPCTSTR pBuf = (LPCTSTR)lpbi + (WORD)lpbi->biSize + dwPaletteSize; 

    iBits  = StretchDIBits( hDC, 
                            xOrigin,              // DestX 
                            yOrigin,              // DestY 
                            lpbi->biWidth,        // nDestWidth 
                            abs(lpbi->biHeight),  // nDestHeight 
                            0,                    // x-coord upper-left corner of source
                            0,                    // y-coord upper-left corner of source
                            lpbi->biWidth,        // width of source rectangle
                            abs(lpbi->biHeight),  // height of source rectangle 
                            pBuf,                 // buffer that holds the bits
                            (LPBITMAPINFO) lpbi,  // lpBitsInfo 
                            DIB_RGB_COLORS,       // wUsage 
                            SRCCOPY);             // raster operation code

    if (lpbi != lpDIB) 
    { 
        GlobalFree((HANDLE)lpbi); 
    } 
    GlobalUnlock(hDib);

    return( ( 0 != iBits ) && ( GDI_ERROR != iBits ) ); 
}


HANDLE CEquipPrintPropertyDlg::ImageToDIB( CDC * pDC, CBitmap * bitmap )
{
	CPalette    pal;

	if( pDC->GetDeviceCaps( RASTERCAPS ) & RC_PALETTE ) {
		UINT        nSize   = sizeof(LOGPALETTE) + ( sizeof(PALETTEENTRY) * 256 );
		LOGPALETTE* pLP     = (LOGPALETTE*)new BYTE[nSize];
		pLP->palVersion     = 0x300;

		pLP->palNumEntries = (unsigned short)GetSystemPaletteEntries( *pDC, 0, 255, pLP->palPalEntry );

		// Create the palette
		pal.CreatePalette( pLP );

        // Free memory
		delete[] pLP;
	}

	// Convert the bitmap to a DIB
	return DDBToDIB( *bitmap, BI_RGB, &pal );
}

HANDLE CEquipPrintPropertyDlg::DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal ) 
{
    // Local Variables
    BITMAP                  bm;
    BITMAPINFOHEADER        bi;
    LPBITMAPINFOHEADER      lpbi;
    DWORD                   dwLen;
    HANDLE                  hDIB;
    HDC                     hDC;
    HPALETTE                hPal;

    // Always a safe Handle
    ASSERT( bitmap.GetSafeHandle() );

    // The function has no arg for bitfields
    if( BI_BITFIELDS == dwCompression )
        return NULL;

    // If a palette has not been supplied use defaul palette
    hPal = (HPALETTE)pPal->GetSafeHandle();
    if( NULL == hPal )
    {
        hPal = (HPALETTE)GetStockObject( DEFAULT_PALETTE );
    }

    // If fails no resource available
    ASSERT( NULL != hPal );

    // Get bitmap information
    bitmap.GetObject( sizeof(bm), (LPCTSTR)&bm );

    // Initialize the bitmapinfoheader
    bi.biSize               = sizeof(BITMAPINFOHEADER);
    bi.biWidth              = bm.bmWidth;
    bi.biHeight             = bm.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = (unsigned short)( bm.bmPlanes * bm.bmBitsPixel );
    bi.biCompression        = dwCompression;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    // Compute the size of the  infoheader and the color table
    INT nColors = ( 1 << bi.biBitCount );
    if( 256 < nColors )
    {
        nColors = 0;
    }

    dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

    // We need a device context to get the DIB from
    hDC = ::GetDC( NULL );
    hPal = SelectPalette( hDC, hPal, FALSE );
    RealizePalette( hDC );

    // If fails no resource available
    ASSERT( NULL != hPal );
    ASSERT( NULL != hDC );

    // Allocate enough memory to hold bitmapinfoheader and color table
    hDIB = GlobalAlloc( GMEM_FIXED, dwLen );

    // Memory allocation failed
    if( NULL == hDIB )
    {
        // Get the ErrorCode
//        m_dwLastError = GetLastError();

        SelectPalette( hDC, hPal, FALSE );
        ::ReleaseDC( NULL, hDC );
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)hDIB;
    *lpbi = bi;

    // Call GetDIBits with a NULL lpBits param, so the device driver 
    // will calculate the biSizeImage field 
    GetDIBits( hDC, 
               (HBITMAP)bitmap.GetSafeHandle(), 
               0L, 
               (DWORD)bi.biHeight,
               (LPBYTE)NULL, 
               (LPBITMAPINFO)lpbi, 
               (DWORD)DIB_RGB_COLORS );

    bi = *lpbi;

    // Set the Colorbit as allowed
    if( 1 >= bi.biBitCount )
        bi.biBitCount = 1;
    else if( 4 >= bi.biBitCount )
        bi.biBitCount = 4;
    else if( 8 >= bi.biBitCount )
        bi.biBitCount = 8;
    else
        bi.biBitCount = 24;

    // If the driver did not fill in the biSizeImage field, then compute it
    // Each scan line of the image is aligned on a DWORD (32bit) boundary
    if( 0 == bi.biSizeImage )
    {
        bi.biSizeImage = ( ( ( ( bi.biWidth * bi.biBitCount ) + 31 ) & ~31 ) / 8 ) * bi.biHeight;

        // If a compression scheme is used the result may infact be larger
        // Increase the size to account for this.
        if( BI_RGB != dwCompression )
        {
            bi.biSizeImage = bi.biSizeImage * 3 / 2;
        }
    }

    // Realloc the buffer so that it can hold all the bits
    dwLen += bi.biSizeImage;

    HANDLE locHandle = GlobalReAlloc( hDIB, dwLen, GMEM_MOVEABLE );

    if( NULL == locHandle )
    {
        // Get the ErrorCode
        //m_dwLastError = GetLastError();

        // Free memory
        GlobalFree( hDIB );

        // Reselect the original palette
        SelectPalette( hDC, hPal, FALSE );
        ::ReleaseDC( NULL, hDC );
        return NULL;
    }
    
    // Set new Handle
    hDIB = locHandle;

    // Get the bitmap bits
    lpbi = (LPBITMAPINFOHEADER)hDIB;

    // Finally get the DIB
    BOOL bGotBits = GetDIBits( hDC,                             // DC-Handle
                               (HBITMAP)bitmap.GetSafeHandle(), // Bitmaphandle
                               0L,                              // Start scan line
                               (DWORD)bi.biHeight,              // Number of scan lines
                               (LPBYTE)lpbi + (bi.biSize + nColors * sizeof(RGBQUAD)),// address for bitmap bits
                               (LPBITMAPINFO)lpbi,              // address of bitmapinfo
                               (DWORD)DIB_RGB_COLORS);          // Use RGB for color table

    // The getting failed
    if( FALSE == bGotBits )
    {
        // Free memory
        GlobalFree( hDIB );
        hDIB = NULL;
    }

    // Reselect the original palette
    SelectPalette( hDC, hPal, FALSE );
    ::ReleaseDC( NULL, hDC );
    return hDIB;
}
