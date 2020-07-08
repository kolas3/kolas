// SE_LOC_1599.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1599.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599 dialog

#define MAX_LINE 13

CSE_LOC_1599::CSE_LOC_1599(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1599)
	m_nOption_Paper = 0;
	m_nOption_Print = 0;
	m_nOption_PaperDir = 1;
	m_strRegNoStart = _T("");
	m_strRegNoEnd = _T("");
	//}}AFX_DATA_INIT
	m_pDM_Parent = NULL;
	m_pSIReportManager = NULL;
	m_pCM = NULL;
}


VOID CSE_LOC_1599::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1599)
	DDX_Radio(pDX, IDC_rPAPER_SHEET, m_nOption_Paper);
	DDX_Radio(pDX, IDC_rPRINT_LINE, m_nOption_Print);
	DDX_Radio(pDX, IDC_rPAPER_WIDTH, m_nOption_PaperDir);
	DDX_Text(pDX, IDC_eREG_NO_START_CP, m_strRegNoStart);
	DDX_Text(pDX, IDC_eREG_NO_END_CP, m_strRegNoEnd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1599, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1599)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP, OnbPRINTEQUIP)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP2, OnbPRINTEQUIP2)
	ON_BN_CLICKED(IDC_bSAVE_CONFIG, OnbSAVECONFIG)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_bFONT, OnbFONT)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_rPRINT_LINE, IDC_rPRINT_STICKER, OnPrintFormatChange )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599 message handlers



INT CSE_LOC_1599::Init()
{
	EFS_BEGIN

	if (!m_pDM_Parent) return -1;

	if (m_pSIReportManager)
		delete m_pSIReportManager;
	m_pSIReportManager = new CSIReportManager(this);

	FreePrintPropertyInfo();

	m_pCM = FindCM(_T("CM_정리_출력_장비_속성"));
	if (!m_pCM) return -1;

	m_lpszFileName = _T("..\\CFG\\정리_장비_출력.cfg");

	INT ids = -1;

	// 설정파일을 읽어온다.
	ids = LoadPrintPropertyInfo();
	if (ids < 0) return ids;

	// 도서관명 가져오기
/*
	CString strLibName;
	m_pDM_Parent->GetOneValueQuery(_T("SELECT VALUE FROM MN_LIBINFO_TBL WHERE VALUE_NAME ='도서관명'"), strLibName);
*/
	CString sFieldName[1] = { _T("LIB_NAME") };
	CString strLibName[1];
	CLocCommonAPI::GetLibInfo( sFieldName , strLibName , 1 , this );

	m_pCM->SetControlMgrData(_T("도서관명"), strLibName[0]);


	OnPrintFormatChange();

	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599::FreePrintPropertyInfo()
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

INT CSE_LOC_1599::LoadPrintPropertyInfo()
{
	EFS_BEGIN

	CStdioFile file;
	if (!file.Open(m_lpszFileName, CFile::modeRead | CFile::typeBinary))
	{
		if (!file.Open(m_lpszFileName, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary))
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
			&pInfo->m_nWidthCount,
			&pInfo->m_nLengthCount,
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

INT CSE_LOC_1599::SavePrintPropertyInfo()
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
			pInfo->m_nWidthCount,
			pInfo->m_nLengthCount,
			pInfo->m_nMarginBarCode,
			pInfo->m_nFontHeight,
			pInfo->m_nFontSize,
			pInfo->m_strFontFace
			);
		
		file.WriteString(strLine);
	}

	file.Close();
	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599::DisplayPrintPropertyInfo()
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

	strData.Format(_T("%d"), pInfo->m_nWidthCount);
	m_pCM->SetControlMgrData(_T("가로개수"), strData);

	strData.Format(_T("%d"), pInfo->m_nLengthCount);
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

CPrintPropertyInfo *CSE_LOC_1599::ApplyPrintPropertyInfo()
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
	pInfo->m_nWidthCount = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("세로개수"), strData);
	pInfo->m_nLengthCount = _ttoi(strData);

	m_pCM->GetControlMgrData(_T("바코드여백"), strData);
	pInfo->m_nMarginBarCode = _ttoi(strData);

	return pInfo;

	EFS_END
	return NULL;
}

INT CSE_LOC_1599::PrintEquip(CPrintPropertyInfo *pInfo)
{
	EFS_BEGIN

	if (pInfo == NULL) return -1;

	UpdateData();

	CSIReportPrintObjectMgr PrintMgr;

	// 위쪽여백
	PrintMgr.nStartX = pInfo->m_nMarginUp;

	// 양쪽여백
	PrintMgr.nStartY = pInfo->m_nMarginSide;

	// 세로간격
	PrintMgr.nTableHeightGap = pInfo->m_nSpaceVert;

	// 가로간격
	PrintMgr.nTableWidthGap = pInfo->m_nSpaceHorz;

	// 레이블높이
	
	// 바코드 높이가 너무 짧다고 바코드 높이를 수정 할 수 있도록 김용배가 수정했음!
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eBARCODE_HEIGHT );
	CString barcodeHeight;
	pEdt->GetWindowText( barcodeHeight );
	//pInfo->m_nLabelHight += 100;
	pInfo->m_nLabelHight += _ttoi( barcodeHeight );	
	
	PrintMgr.nLineHeight = pInfo->m_nLabelHight / MAX_LINE;
	
	// 레이블너비
	PrintMgr.nTableWidth = pInfo->m_nLabelWidth;

	// 가로개수
	PrintMgr.m_nRows = pInfo->m_nWidthCount;
	
	// 세로개수
	PrintMgr.m_nCols = pInfo->m_nLengthCount;

	// 바코드여백
	PrintMgr.m_nBarcodeWidthGap  = pInfo->m_nMarginBarCode;

	// 폰트
	PrintMgr.SetFont(pInfo->m_nFontSize, pInfo->m_strFontFace);

	// 용지 가로 / 세로
	if (m_nOption_PaperDir == 0)
	{
		m_pSIReportManager->m_nDefaultPaperType = PAPER_A4_PORTRAIT;
		PrintMgr.nPaper = PAPER_A4_PORTRAIT;
	}
	else
	{
		m_pSIReportManager->m_nDefaultPaperType = PAPER_A4_LANDSCAPE;
		PrintMgr.nPaper = PAPER_A4_LANDSCAPE;
	}

	// 출력 시작 (공백의 개수가 있으면 그만큼 넘어가자
	INT nStartPos = 0;
	CString strStartPos;
	m_pCM->GetControlMgrData(_T("시작위치"), strStartPos);
	
	if (!strStartPos.IsEmpty())
		nStartPos = _ttoi(strStartPos);
	nStartPos--;

	// 레이블 - 줄바꿔찍기
	if (m_nOption_Print == 0)
		PrintEquip_Line(&PrintMgr, nStartPos);
	// 이어찍기
	else if (m_nOption_Print == 1)
		PrintEquip_Serial(&PrintMgr, nStartPos);
	// 바코드
	else if (m_nOption_Print == 2)
		PrintEquip_BarCode(&PrintMgr, nStartPos);
	
	INT ids = m_pSIReportManager->DeleteTempPageFiles();

	// 출력물 생성
	m_pSIReportManager->FreeWorkForm();
	ids = PrintMgr.Print(m_pSIReportManager);

	ids = m_pSIReportManager->Print2();
	
	return 0;

	EFS_END
	return -1;
}


INT CSE_LOC_1599::PrintEquip_Line(CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;
	// 실시간 배가 자료의 청구기호를 줄바꿔찍기한다.
	
	// 함번호 + CRLF
	// 서명 + CRLF
	// 권호명 + CRLF

	INT ids = -1;
	INT nLine = 0;
	CString strItemData;
	CSIReportPrintObject *pObject = NULL;

	
	CESL_Grid* pGrid = GetParentGridPtr();
	if( pGrid == NULL ) return -2;

	pGrid->SelectMakeList();

	/// nMode = 0 -> 그리드에 선택된 행이 존재한다. , nMode = 1 -> 전체를 다 출력한다.
	INT nMode = 0;
	INT nRowCount = pGrid->SelectGetCount();
	if( nRowCount == 0 )
	{
		nMode = 1;
		nRowCount = m_pDM_Parent->GetRowCount();
	}
	else
		pGrid->SelectGetHeadPosition();

	INT nIndex;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		if( nMode == 0 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = idx;


		nLine = 0;

		pObject = pPrintMgr->AddCard(MAX_LINE);
		if (!pObject) continue;

		if (--nStartPosition >= 0) 
		{
			idx--;
			continue;
		}

		// 레이블이 출력구분 체크
		ids = m_pDM_Parent->GetCellData(_T("함번호"), nIndex, strItemData);
		pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("서명"), nIndex, strItemData);
		pObject->SetAt(nLine++, strItemData);

		ids = m_pDM_Parent->GetCellData(_T("권호명"), nIndex , strItemData);
		pObject->SetAt(nLine++, strItemData);

		if( nMode == 0 )
			pGrid->SelectGetNext();
	}

	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599::PrintEquip_Serial(CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;
	// 청구기호를 이어서 찍는다.
	// 별치기호코드 분류기호-도서기호-권책기호=복본기호

	INT ids = -1;
	CString strItemData;
	CString strRegNo;
	CString strCallNo;
	CSIReportPrintObject *pObject = NULL;
	INT nRowCount = m_pDM_Parent->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pObject = pPrintMgr->AddCard(MAX_LINE);
		if (!pObject) continue;

		if (--nStartPosition >= 0) 
		{
			idx--;
			continue;
		}

		// 레이블이 출력구분 체크
		if (m_pDM_Parent->GetCellData(_T("레이블출력구분"), idx) != _T("2")) continue;

		ids = m_pDM_Parent->GetCellData(_T("등록번호"), idx, strRegNo);

		ids = m_pDM_Parent->GetCellData(_T("청구기호"), idx, strCallNo);
		
		strItemData.Format(_T("%-50s   %s"), strCallNo, strRegNo);
		
		pObject->SetAt(0, strItemData);
	}


	return 0;

	EFS_END
	return -1;
}



/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599 message handlers

BOOL CSE_LOC_1599::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	CEdit *pEdt = ( CEdit * )GetDlgItem( IDC_eBARCODE_HEIGHT );
	pEdt->SetWindowText( _T("100") );
	
	
	if (InitESL_Mgr(_T("SM_정리_출력_장비_속성")) < 0) 
	{
		m_strLastErrMsg = _T("Init ESL Mgr Error!!");
		goto ERR;
	}

	if (Init() < 0) goto ERR;
	
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

VOID CSE_LOC_1599::OnPrintFormatChange()
{
	EFS_BEGIN

	UpdateData();

	// 바코드 마진 콘트롤 보여주기 / 숨기기
	CWnd *pWndTBarCode = GetDlgItem(IDC_tMARGIN_BARCODE);
	CWnd *pWndEBarCode = GetDlgItem(IDC_eMARGIN_BARCODE);

	CEdit *pWndELibName = (CEdit*)GetDlgItem(IDC_eLIB_NAME_CP);

	pWndTBarCode->ShowWindow(SW_HIDE);
	pWndEBarCode->ShowWindow(SW_HIDE);
//	pWndELibName->SetReadOnly(TRUE);

	// 레이블 - 줄바꿔찍기
	if (m_nOption_Print == 0)
	{
		m_strPrintFormatKey = _T("LABEL_LINE");
	}
	// 레이블 - 이어찍기
	else if (m_nOption_Print == 1)
	{
		m_strPrintFormatKey = _T("LABEL_SERIAL");
	}

	DisplayPrintPropertyInfo();

	EFS_END
}

VOID CSE_LOC_1599::OnbPRINTEQUIP() 
{
	EFS_BEGIN

	CESL_DataMgr *pDM = FindDM( _T("DM_SE_LOC_1500") );
	if( !pDM->GetRowCount() )
	{
		MessageBox( _T("출력할 목록이 존재하지 않습니다."), _T("KOLAS II Upgrade"), MB_OK );
		return;
	}

	CPrintPropertyInfo *pInfo = NULL;
	pInfo = ApplyPrintPropertyInfo();
	if (pInfo == NULL) return;
	
	PrintEquip(pInfo);

	EFS_END
}

VOID CSE_LOC_1599::OnbSAVECONFIG() 
{
	EFS_BEGIN

	ApplyPrintPropertyInfo();
	SavePrintPropertyInfo();

	EFS_END
}

VOID CSE_LOC_1599::OnbFONT() 
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

VOID CSE_LOC_1599::OnDestroy() 
{
	EFS_BEGIN

	CDialog::OnDestroy();

	delete m_pSIReportManager;
	m_pSIReportManager = NULL;

	FreePrintPropertyInfo();	

	EFS_END
}


VOID CSE_LOC_1599::OnbPRINTEQUIP2() 
{
	OnCancel();	
}



INT CSE_LOC_1599::PrintEquip_BarCode(CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN
	
	if (pPrintMgr == NULL) return -1;
	
	pPrintMgr->MODE = _T("바코드");
	
	pPrintMgr->m_nBarcodeHeight = pPrintMgr->nLineHeight * 2;
	
	INT ids = -1;

	CString strLibName;
	CString strRegNo;
	CString strCallNo;

	m_pCM->GetControlMgrData(_T("도서관명"), strLibName);
	
	CSIReportPrintObject *pObject = NULL;

	CESL_Grid* pGrid = GetParentGridPtr();
	if( pGrid == NULL ) return -2;
	
	pGrid->SelectMakeList();
	
	/// nMode = 0 -> 그리드에 선택된 행이 존재한다. , nMode = 1 -> 전체를 다 출력한다.
	INT nMode = 0;
	INT nRowCount = pGrid->SelectGetCount();
	if( nRowCount == 0 )
	{
		nMode = 1;
		nRowCount = m_pDM_Parent->GetRowCount();
	}
	else
		pGrid->SelectGetHeadPosition();
	
	INT nIndex;
	
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		pObject = pPrintMgr->AddCard(MAX_LINE);
		if (!pObject) continue;
		
		if( nMode == 0 )
			nIndex = pGrid->SelectGetIdx();
		else
			nIndex = idx;

		if (--nStartPosition >= 0) 
		{
			idx--;
			continue;
		}
		
		ids = m_pDM_Parent->GetCellData(_T("가등록번호"), nIndex, strRegNo);
		
		pObject->SetAt(0, strLibName);
		pObject->SetAt(1, strRegNo);
		pObject->SetAt(2, strRegNo);

		if( nMode == 0 )
			pGrid->SelectGetNext();
		
	}
	
	return 0;
	
	EFS_END
	return -1;
}




CESL_Grid* CSE_LOC_1599::GetParentGridPtr()
{
	EFS_BEGIN

	CESL_ControlMgr* pCM = FindCM( _T("CM_SE_LOC_1500") );
	if( pCM == NULL ) return NULL;

	CESL_Grid* pGrid = (CESL_Grid*)pCM->FindControl(_T("메인그리드"));

	return pGrid;

	EFS_END

	return NULL;
}
