// SE_LOC_1599_EX.cpp : implementation file
//

#include "stdafx.h"
#include "SE_LOC_1599_EX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599_EX dialog


CSE_LOC_1599_EX::CSE_LOC_1599_EX(CESL_Mgr* pParent)
	: CEquipPrintPropertyDlg(pParent)
{
	//{{AFX_DATA_INIT(CSE_LOC_1599_EX)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


VOID CSE_LOC_1599_EX::DoDataExchange(CDataExchange* pDX)
{
	CEquipPrintPropertyDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_LOC_1599_EX)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_LOC_1599_EX, CDialog)
	//{{AFX_MSG_MAP(CSE_LOC_1599_EX)
	ON_BN_CLICKED(IDC_bPRINT_EQUIP, OnbPRINTEQUIP)
	ON_BN_CLICKED(IDC_bSAVE_CONFIG, OnbSAVECONFIG)
	ON_BN_CLICKED(IDC_bFONT, OnbFONT)
	ON_CBN_SELCHANGE(IDC_cREG_NO_PLACE, OnSelchangecREGNOPLACE)
	ON_CBN_SELENDOK(IDC_CobUnit, OnSelendokCobUnit)
	ON_WM_DESTROY()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_rPRINT_LINE, IDC_rPRINT_STICKER, OnPrintFormatChange )		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_LOC_1599_EX message handlers

BOOL CSE_LOC_1599_EX::OnInitDialog() 
{
	m_lpszFileName = _T("..\\CFG\\연속_실시간열람_장비_출력.cfg");

	CEquipPrintPropertyDlg::OnInitDialog();

	/// 컨트롤이 안보이게 숨긴다.
	InitControl();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CSE_LOC_1599_EX::InitControl()
{
	const INT nUnVisibleCnt = 12;

	UINT nUnVisbleIDC[nUnVisibleCnt] =
	{
		IDC_rPRINT_SERIAL , IDC_rPRINT_BOOKCARD , IDC_rPRINT_STICKER , IDC_staBOOKCARD , IDC_staREG_NO , 
		IDC_cREG_CODE_CP , IDC_eREG_NO_START_CP , IDC_eREG_NO_END_CP , IDC_staSLASH , IDC_rPRINT_SERIAL ,
		IDC_CHK_SERIAL , IDC_BtnSerialLabelConfig
	};

//	for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
	for( INT i = 0 ; i < nUnVisibleCnt ; i++ )
	{
		GetDlgItem(nUnVisbleIDC[i])->ShowWindow(SW_HIDE);
	}

	/// 컨트롤을 초기화해준다.
	CString sText;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_cREG_NO_PLACE);
	for( i = 0 ; i < pCombo->GetCount() ; i++ )
	{
		pCombo->GetLBText(i,sText);
		if( sText.Compare(_T("레이블사이")) == 0 )
		{
			pCombo->SetCurSel(i);
		}
	}

	((CButton*)GetDlgItem(IDC_CHK_SERIAL))->SetCheck(FALSE);

	((CComboBox*)GetDlgItem(IDC_cREG_NO_PLACE))->ShowWindow(SW_HIDE);

	return 0;
}


BOOL CSE_LOC_1599_EX::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CEquipPrintPropertyDlg::PreTranslateMessage(pMsg);
}


VOID CSE_LOC_1599_EX::OnbPRINTEQUIP() 
{
	EFS_BEGIN
/*
	CEquipPrintPropertyDlg::OnbPRINTEQUIP();
*/
	
	/// 청구기호가 아니고 함번호로 출력하기 때문에 다르다.
	CPrintPropertyInfo *pInfo = NULL;
	pInfo = CEquipPrintPropertyDlg::ApplyPrintPropertyInfo();
	if (pInfo == NULL) return;
	
	this->PrintEquip(pInfo);
	
	


	EFS_END
}



VOID CSE_LOC_1599_EX::OnbSAVECONFIG() 
{
	EFS_BEGIN
		
	CEquipPrintPropertyDlg::OnbSAVECONFIG();
	
	EFS_END
}


VOID CSE_LOC_1599_EX::OnPrintFormatChange()
{
	EFS_BEGIN
	
	CEquipPrintPropertyDlg::OnPrintFormatChange();

	InitControl();
	
	EFS_END
}


VOID CSE_LOC_1599_EX::OnbFONT() 
{
	EFS_BEGIN
		
	CEquipPrintPropertyDlg::OnbFONT();
	
	EFS_END
}


VOID CSE_LOC_1599_EX::OnSelchangecREGNOPLACE() 
{
	CEquipPrintPropertyDlg::OnSelchangecREGNOPLACE();
}



VOID CSE_LOC_1599_EX::OnSelendokCobUnit() 
{
	CEquipPrintPropertyDlg::OnSelendokCobUnit();
}

VOID CSE_LOC_1599_EX::OnDestroy() 
{
	CEquipPrintPropertyDlg::OnDestroy();
}


INT CSE_LOC_1599_EX::PrintEquip(CPrintPropertyInfo *pInfo)
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
	
	//INT ids = 0;	// 실제출력 여부 리턴(-1:출력 개수가 없다.)
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
			//PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
			
			//PrintMgr.nLineHeight = 13.5;

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
	/// 모든 출력물이 똑같다.
	else if (m_nOption_Print == 2)
	{
		m_nMaxLine = 4;
		if ( sUnit == _T("밀리미터(mm)") ) {
			pInfo->m_nLabelHight = pInfo->m_nLabelHight * 10/3;
		}
		PrintMgr.nLineHeight = pInfo->m_nLabelHight / m_nMaxLine;
		CEquipPrintPropertyDlg::PrintEquip_BarCode(pInfo, &PrintMgr, nStartPos);
	}

	INT ids = m_pSIReportManager->DeleteTempPageFiles();

	EndWaitCursor();

	// 출력물 생성
	m_pSIReportManager->FreeWorkForm();
	//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
	m_pSIReportManager->m_bPrivacyPrintLogRecord = TRUE;
	ids = PrintMgr.Print(m_pSIReportManager);

	ids = m_pSIReportManager->Print2();
	
	return 0;

	EFS_END
	return -1;
}



INT CSE_LOC_1599_EX::PrintEquip_LineNormal(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN
	INT iStartPosOri = nStartPosition;
	
	if (pPrintMgr == NULL) return -1;

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

	m_pGrid_Parent->SelectMakeList();

	INT nSelectedCnt = m_pGrid_Parent->SelectGetCount();

	
	INT i;

	i = m_pGrid_Parent->SelectGetHeadPosition();
	
	INT iPrintCount = 0;
	INT iStart = 0;


	while (i >= 0)
	{
		
		idx = i;
		 
		//if ( iPrintCount - pPrintMgr->m_nCols == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {
		if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1))  == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {
			idx = iTotalPrintCount-1;				
		}
		//if ( iPrintCount - pPrintMgr->m_nCols > (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {			
		//if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1)) > (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {			
		if ( iPrintCount - (iTotalPrintCount /(pPrintMgr->m_nCols -1)) == (iTotalPrintCount + iStartPosOri + (iStartPosOri / (pPrintMgr->m_nCols -1)))) {			
			break;
		}

		//i = m_pGrid_Parent->SelectGetNext();
		if ( i == m_pGrid_Parent->SelectGetCount() ) {
			i = m_pGrid_Parent->SelectGetCount();
		}
		else {
			i = m_pGrid_Parent->SelectGetNext();
		}
		
		
		nLine = 0;


		if (bAddCard)
		{
			pObject = pPrintMgr->AddCard(m_nMaxLine);
			iPrintCount ++;
			if (!pObject) continue;
			nCol++;
		}

		bAddCard = TRUE;

		if (--nStartPosition >= 0) 
		{
			i = m_pGrid_Parent->SelectGetHeadPosition();
		}


		// 실제 출력 개수
		iRealPrintCount++;
		// 실시간 열람 자료에 맞게 수정
		// 레이블이 출력구분 체크
		ids = m_pDM_Parent->GetCellData(_T("함번호"), idx, strItemData);
		pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("서명"), idx, strItemData);
		pObject->SetAt(nLine++, strItemData);
		
		ids = m_pDM_Parent->GetCellData(_T("권호명"), idx , strItemData);
		pObject->SetAt(nLine++, strItemData);
			

	}

	// 실제 찍을 개수가 없으면 리턴
	if ( iRealPrintCount == 0 ) return -1;
	
	return 0;

	EFS_END
	return -1;
}

INT CSE_LOC_1599_EX::PrintEquip_LineExtra(CPrintPropertyInfo *pInfo, CSIReportPrintObjectMgr *pPrintMgr, INT nStartPosition)
{
	EFS_BEGIN

	if (pPrintMgr == NULL) return -1;

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
		ids = m_pDM_Parent->GetCellData(_T("함번호"), idx, strItemData);
		pObject->SetAt(nLine++, strItemData);
		ids = m_pDM_Parent->GetCellData(_T("서명"), idx, strItemData);
		pObject->SetAt(nLine++, strItemData);
		ids = m_pDM_Parent->GetCellData(_T("권호명"), idx , strItemData);
		pObject->SetAt(nLine++, strItemData);
		

	}

	return 0;

	EFS_END
	return -1;
}
