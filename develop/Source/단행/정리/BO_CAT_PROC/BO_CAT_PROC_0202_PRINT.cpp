// BO_CAT_PROC_0202_PRINT.cpp : implementation file
//

#include "stdafx.h"
#include "BO_CAT_PROC_0202_PRINT.h"

#include <iostream.h>
#include <iomanip.h>

#include <Winspool.h>
#include <Windows.h>
#include <malloc.h>
#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0202_PRINT dialog


CBO_CAT_PROC_0202_PRINT::CBO_CAT_PROC_0202_PRINT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_CAT_PROC_0202_PRINT)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

VOID CBO_CAT_PROC_0202_PRINT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0202_PRINT)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0202_PRINT, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0202_PRINT)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_bPRINT, OnbPRINT)
	ON_BN_CLICKED(IDC_chDEFAULT, OnchDEFAULT)
	ON_BN_CLICKED(IDC_chTITLE, OnchTITLE)
	ON_BN_CLICKED(IDC_chAUTHOR, OnchAUTHOR)
	ON_BN_CLICKED(IDC_chSUBJECT, OnchSUBJECT)
	ON_BN_CLICKED(IDC_chCLASSIFY, OnchCLASSIFY)
	ON_BN_CLICKED(1008, OnSerialPrint)
	ON_BN_CLICKED(IDC_rPRINT_PAPER, OnMonoPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0202_PRINT message handlers

BOOL CBO_CAT_PROC_0202_PRINT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_iAllBuchulCnt = 0;

	m_iAllCardListCnt = 0;

	m_iCurentCardListCnt = 0;

	m_list->FreeData();
	m_list->nTableGap = 10;

	
	
	m_pInfo->pMarcMgr->Decoding(m_strStreamMarc, &m_Marc);

	if ( m_strParentMode == _T("") ) return FALSE;
	
	if ( m_strParentMode == _T("카드목록") )
	{
		CButton *pSerial;
		pSerial = (CButton *)GetDlgItem(IDC_rCARD_PRINT);
		pSerial->SetCheck(IDC_rCARD_PRINT);

		GetDlgItem(IDC_rADDENTRY_PRINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_eADDENTRY_CNT)->EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC2)->EnableWindow(FALSE);

		pSerial = (CButton *)GetDlgItem(IDC_rPRINT_PAPER);
		pSerial->SetCheck(IDC_rPRINT_PAPER);

		pSerial = (CButton *)GetDlgItem(IDC_chDEFAULT);
		pSerial->SetCheck(IDC_chDEFAULT);

		SetDlgItemText(IDC_eDEFAULT_CNT, _T("0"));
		SetDlgItemText(IDC_eTITLE_CNT, _T("0"));
		SetDlgItemText(IDC_eAUTHOR_CNT, _T("0"));
		SetDlgItemText(IDC_eSUBJECT_CNT, _T("0"));
		SetDlgItemText(IDC_eCLASSIFY_CNT, _T("0"));
		
		OnchDEFAULT();
		OnchTITLE();
		OnchAUTHOR();
		OnchSUBJECT();
		OnchCLASSIFY();
	}
	else if ( m_strParentMode == _T("부출지시서") )
	{
		CButton *pSerial;
		pSerial = (CButton *)GetDlgItem(IDC_rADDENTRY_PRINT);
		pSerial->SetCheck(IDC_rADDENTRY_PRINT);

		GetDlgItem(IDC_rCARD_PRINT)->EnableWindow(FALSE);

		GetDlgItem(IDC_chDEFAULT)->EnableWindow(FALSE);
		GetDlgItem(IDC_chTITLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_chAUTHOR)->EnableWindow(FALSE);
		GetDlgItem(IDC_chSUBJECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_chCLASSIFY)->EnableWindow(FALSE);

		GetDlgItem(IDC_eDEFAULT_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_eTITLE_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_eAUTHOR_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_eSUBJECT_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_eCLASSIFY_CNT)->EnableWindow(FALSE);

		GetDlgItem(IDC_STATIC3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC4)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC5)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC6)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC7)->EnableWindow(FALSE);

		pSerial = (CButton *)GetDlgItem(IDC_rPRINT_PAPER);
		pSerial->SetCheck(IDC_rPRINT_PAPER);

		SetDlgItemText(IDC_eADDENTRY_CNT, _T("0"));
	}

	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_rPRINT_PAPER);
	if ( pSerial->GetCheck() == 1 ) 
		m_iPrintType = MODE_MONO;
	else
		m_iPrintType = MODE_SERIAL;
	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_CAT_PROC_0202_PRINT::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

VOID CBO_CAT_PROC_0202_PRINT::OnbPRINT() 
{
EFS_BEGIN

	// TODO: Add your control notification handler code here
	if ( m_strParentMode == _T("") ) return;

	if ( m_strParentMode == _T("카드목록") )
	{
		if ( !GetAllPrintCardListCnt() ) return;
		
		if ( !PrePrintCardList() ) 
		{
			AfxMessageBox(_T("카드목록 출력중 에러가 발생하였습니다."));
			return;		
		}
	}else 
	if ( m_strParentMode == _T("부출지시서") )
	{
		if ( !GetAllPrintBuchulCnt() )  return;

		if ( !ExecutePrintBuchul() ) 
		{
			AfxMessageBox(_T("부출지시서 출력중 에러가 발생하였습니다."));
			return;		
		}
	}


EFS_END
return;	
}

BOOL CBO_CAT_PROC_0202_PRINT::MakePrintCardList(CString sAddStringForBuchul)
{
EFS_BEGIN

	if ( m_list == NULL || m_sCard == _T("") )
		return FALSE;
	
	CString sSearchCard = m_sCard;

	INT iPosCut = m_sCard.Find( _T("\n") );

	m_pCard = m_list->AddCard();			
	if (m_pCard == NULL) return FALSE;

	INT iPrnLine = -1;
	while ( iPosCut != -1 ) {
		CString sLeftStr;
		sLeftStr= m_sCard.Left(iPosCut-1);
		sLeftStr.TrimLeft();
		sLeftStr.TrimRight();
		iPrnLine++;

		if ( iPrnLine > 15 ) {
			iPrnLine = 0;
			m_pCard = m_list->AddCard();
			if (m_pCard == NULL) continue;
		}
		if ( iPrnLine == 1 )
		{
			sLeftStr = sLeftStr + _T("        ") + sAddStringForBuchul;
		}
		if ( iPrnLine > 3 && iPrnLine < 14 ) {
			if ( iPrnLine == 13 )
				sLeftStr = _T("                                    ") + sLeftStr;
			else 
			{
				if ( IsCheckRegNo(sLeftStr) )
					sLeftStr = sLeftStr;
				else 
				sLeftStr = _T("           ") + sLeftStr;
			}
		}
		if ( iPrnLine == 14 ) {
			iPrnLine++;
		}
		if ( iPrnLine == 15) {
			sLeftStr = _T("                                ") + sLeftStr;
		}		
		
		sLeftStr.TrimRight();
		m_pCard->SetAt( iPrnLine, sLeftStr);
		m_sCard = m_sCard.Mid(iPosCut+1);


		INT iPosEnter = m_sCard.Find( _T("\r\n") );
		if ( iPosEnter != 0 && sLeftStr != _T("") )
			m_sCard.TrimLeft();

		iPosCut = m_sCard.Find( _T("\n") );

		sLeftStr.TrimLeft();
	}	

	iPrnLine++;
	if ( iPrnLine > 3 && iPrnLine < 13) {
		m_sCard.TrimLeft();
		m_sCard.TrimRight();
		
		if ( m_sCard.GetLength() == 12)
			m_sCard = m_sCard;
		else
			m_sCard = _T("           ") + m_sCard;
	}
	if ( iPrnLine == 13 ) {
		iPrnLine++;
	}
	if ( iPrnLine == 14) {
		m_sCard = _T("                                ") + m_sCard;
	}
	
	if  ( iPrnLine != 16 ) 
	{
		m_pCard->SetAt( iPrnLine, m_sCard);
	}
	m_sCard = sSearchCard;

	m_iCurentCardListCnt++;

	if ( m_iCurentCardListCnt ==  m_iAllCardListCnt)
		ExecutePrintCardList();

	return TRUE;
EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0202_PRINT::ExecutePrintBuchul()
{
EFS_BEGIN

	CString strTmpItem;
	GetDlgItemText(IDC_eADDENTRY_CNT, strTmpItem);

	CString sSearchCard = m_sCard;
	m_pCard = m_list->AddCard();
	INT iPrnLine = -1;

	for (INT i = 0; i < _ttoi(strTmpItem) ; i++ )
	{
				
		if (m_pCard == NULL) return FALSE;

		INT iPosCut = m_sCard.Find( _T("\n") );
		
		while ( iPosCut != -1 ) {

			CString sLeftStr;
			sLeftStr= m_sCard.Left(iPosCut-1);
			sLeftStr.TrimLeft();
			sLeftStr.TrimRight();
			iPrnLine++;
		
			if ( iPrnLine > 15 ) {
				iPrnLine = 0;
				m_pCard = m_list->AddCard();
			}

			
			m_pCard->SetAt( iPrnLine, sLeftStr);
			m_sCard = m_sCard.Mid(iPosCut+1);
			
			
			iPosCut = m_sCard.Find( _T("\n") );
			
			INT iPosEnter = m_sCard.Find( _T("\r\n") );
			
			
			sLeftStr.TrimLeft();
			
			if ( iPosEnter != 0 && sLeftStr != _T("") )
				m_sCard.TrimLeft();

		}
		m_sCard = sSearchCard;
	}


	ExecutePrintCardList();

	return TRUE;

EFS_END
return FALSE;
}

VOID CBO_CAT_PROC_0202_PRINT::OnchDEFAULT() 
{
EFS_BEGIN

	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_chDEFAULT);
	if ( pSerial->GetCheck() == 1 ) {
		GetDlgItem(IDC_eDEFAULT_CNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC3)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_eDEFAULT_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC3)->EnableWindow(FALSE);	
	}
EFS_END
}

VOID CBO_CAT_PROC_0202_PRINT::OnchTITLE() 
{
EFS_BEGIN
	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_chTITLE);
	if ( pSerial->GetCheck() == 1 ) {
		GetDlgItem(IDC_eTITLE_CNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC4)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_eTITLE_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC4)->EnableWindow(FALSE);	
	}
EFS_END
	
}

VOID CBO_CAT_PROC_0202_PRINT::OnchAUTHOR() 
{
EFS_BEGIN	
	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_chAUTHOR);
	if ( pSerial->GetCheck() == 1 ) {
		GetDlgItem(IDC_eAUTHOR_CNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC5)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_eAUTHOR_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC5)->EnableWindow(FALSE);	
	}
	EFS_END	
}

VOID CBO_CAT_PROC_0202_PRINT::OnchSUBJECT() 
{
EFS_BEGIN		
	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_chSUBJECT);
	if ( pSerial->GetCheck() == 1 ) {	
		GetDlgItem(IDC_eSUBJECT_CNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC6)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_eSUBJECT_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC6)->EnableWindow(FALSE);	
	}
	EFS_END		
}

VOID CBO_CAT_PROC_0202_PRINT::OnchCLASSIFY() 
{
EFS_BEGIN	
	CButton *pSerial;
	pSerial = (CButton *)GetDlgItem(IDC_chCLASSIFY);
	if ( pSerial->GetCheck() == 1 ) {
		GetDlgItem(IDC_eCLASSIFY_CNT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC7)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_eCLASSIFY_CNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC7)->EnableWindow(FALSE);	
	}
	EFS_END		
}

BOOL CBO_CAT_PROC_0202_PRINT::PrePrintCardList()
{
EFS_BEGIN

	if ( m_list == NULL || m_sCard == _T("") ||  m_pCard == NULL )
		return FALSE;
	
	CButton *pSerial;
	
	pSerial = (CButton *)GetDlgItem(IDC_chDEFAULT);	
	if ( pSerial->GetCheck() == 1 ) {
		PrnCardList_Default();
	}

	pSerial = (CButton *)GetDlgItem(IDC_chTITLE);	
	if ( pSerial->GetCheck() == 1 ) {
		PrnCardList_Title();
	}
	
	pSerial = (CButton *)GetDlgItem(IDC_chAUTHOR);	
	if ( pSerial->GetCheck() == 1 ) {
		PrnCardList_Author();
	}

	pSerial = (CButton *)GetDlgItem(IDC_chSUBJECT);	
	if ( pSerial->GetCheck() == 1 ) {
		PrnCardList_Subject();
	}

	pSerial = (CButton *)GetDlgItem(IDC_chCLASSIFY);	
	if ( pSerial->GetCheck() == 1 ) {
		PrnCardList_Classify();
	}

	return TRUE;

	
EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0202_PRINT::PrnCardList_Default()
{
EFS_BEGIN

	CString sPrnCount;
	GetDlgItemText(IDC_eDEFAULT_CNT, sPrnCount);

	CString sAddStringForBuchul;
	
	for ( INT i = 0; i < _ttoi(sPrnCount); i++ )
	{
		if ( m_iPrintType == MODE_MONO )
			MakePrintCardList(sAddStringForBuchul);
		else if ( m_iPrintType == MODE_SERIAL )
			MakePrintCardList_SERIAL(sAddStringForBuchul);
	}

	return TRUE;

	
EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0202_PRINT::PrnCardList_Title()
{
EFS_BEGIN

	CString sPrnCount;
	GetDlgItemText(IDC_eTITLE_CNT, sPrnCount);

	CString sAddStringForBuchul;

	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("245$a"), sAddStringForBuchul );
	
	
	for ( INT i = 0; i < _ttoi(sPrnCount); i++ )
	{
		if ( m_iPrintType == MODE_MONO )
			MakePrintCardList(sAddStringForBuchul);
		else if ( m_iPrintType == MODE_SERIAL )
			MakePrintCardList_SERIAL(sAddStringForBuchul);
	}

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0202_PRINT::PrnCardList_Author()
{
EFS_BEGIN

	CString sPrnCount;
	GetDlgItemText(IDC_eAUTHOR_CNT, sPrnCount);

	CString sAddStringForBuchul;

	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("100$a"), sAddStringForBuchul );

	if ( sAddStringForBuchul.IsEmpty() ) 
		m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("700$a"), sAddStringForBuchul );
		
	for ( INT i = 0; i < _ttoi(sPrnCount); i++ )
	{
		if ( m_iPrintType == MODE_MONO )
			MakePrintCardList(sAddStringForBuchul);
		else if ( m_iPrintType == MODE_SERIAL )
			MakePrintCardList_SERIAL(sAddStringForBuchul);
	}

	return TRUE;

EFS_END
return FALSE;
}
BOOL CBO_CAT_PROC_0202_PRINT::PrnCardList_Subject()
{
EFS_BEGIN

	CString sPrnCount;
	GetDlgItemText(IDC_eSUBJECT_CNT, sPrnCount);

	CString sAddStringForBuchul, sTmpItem;

	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("600$a"), sTmpItem );
	if ( ! sAddStringForBuchul.IsEmpty() )
		sAddStringForBuchul = sAddStringForBuchul + sTmpItem + _T(" ");

	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("610$a"), sTmpItem );
	if ( ! sAddStringForBuchul.IsEmpty() )
		sAddStringForBuchul = sAddStringForBuchul + sTmpItem + _T(" ");

	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("611$a"), sTmpItem );
	if ( ! sAddStringForBuchul.IsEmpty() )
		sAddStringForBuchul = sAddStringForBuchul + sTmpItem + _T(" ");

	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("630$a"), sTmpItem );
	if ( ! sAddStringForBuchul.IsEmpty() )
		sAddStringForBuchul = sAddStringForBuchul + sTmpItem + _T(" ");

	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("650$a"), sTmpItem );
	if ( ! sAddStringForBuchul.IsEmpty() )
		sAddStringForBuchul = sAddStringForBuchul + sTmpItem + _T(" ");
	
	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("651$a"), sTmpItem );
	if ( ! sAddStringForBuchul.IsEmpty() )
		sAddStringForBuchul = sAddStringForBuchul + sTmpItem + _T(" ");
	
	for ( INT i = 0; i < _ttoi(sPrnCount); i++ )
	{
		if ( m_iPrintType == MODE_MONO )
			MakePrintCardList(sAddStringForBuchul);
		else if ( m_iPrintType == MODE_SERIAL )
			MakePrintCardList_SERIAL(sAddStringForBuchul);
	}

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0202_PRINT::PrnCardList_Classify()
{
EFS_BEGIN

	CString sPrnCount;
	GetDlgItemText(IDC_eCLASSIFY_CNT, sPrnCount);

	CString sAddStringForBuchul;
	
	m_pInfo->pMarcMgr->GetItem( &m_Marc, _T("090$a"), sAddStringForBuchul );

	for ( INT i = 0; i < _ttoi(sPrnCount); i++ )
	{
		if ( m_iPrintType == MODE_MONO )
			MakePrintCardList(sAddStringForBuchul);
		else if ( m_iPrintType == MODE_SERIAL )
			MakePrintCardList_SERIAL(sAddStringForBuchul);
	}

	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0202_PRINT::ExecutePrintCardList()
{
EFS_BEGIN

	CSIReportManager *pSIReportManager = new CSIReportManager(this);

	
	m_iCurentCardListCnt = 0;

	INT ids;
	ids = pSIReportManager->DeleteTempPageFiles();

	m_list->m_nPageCount = 0;

	ids = m_list->Print(pSIReportManager);

	ids = pSIReportManager->Print2();
	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}
	m_list->FreeData();
	return TRUE;
EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0202_PRINT::GetAllPrintCardListCnt()
{
EFS_BEGIN

	CString strTmpItem;
	CButton *pSerial;

	m_iAllCardListCnt = 0;

	pSerial = (CButton *)GetDlgItem(IDC_chDEFAULT);
	if ( pSerial->GetCheck() == 1 ) 
	{
		GetDlgItemText(IDC_eDEFAULT_CNT, strTmpItem);
		m_iAllCardListCnt = m_iAllCardListCnt + _ttoi(strTmpItem); 
	}

	pSerial = (CButton *)GetDlgItem(IDC_chTITLE);
	if ( pSerial->GetCheck() == 1 ) 
	{
		GetDlgItemText(IDC_eTITLE_CNT, strTmpItem);
		m_iAllCardListCnt = m_iAllCardListCnt + _ttoi(strTmpItem); 
	}

	pSerial = (CButton *)GetDlgItem(IDC_chAUTHOR);
	if ( pSerial->GetCheck() == 1 ) 
	{
		GetDlgItemText(IDC_eAUTHOR_CNT, strTmpItem);
		m_iAllCardListCnt = m_iAllCardListCnt + _ttoi(strTmpItem); 
	}

	pSerial = (CButton *)GetDlgItem(IDC_chSUBJECT);
	if ( pSerial->GetCheck() == 1 ) 
	{
		GetDlgItemText(IDC_eSUBJECT_CNT, strTmpItem);
		m_iAllCardListCnt = m_iAllCardListCnt + _ttoi(strTmpItem); 
	}
	
	pSerial = (CButton *)GetDlgItem(IDC_chCLASSIFY);
	if ( pSerial->GetCheck() == 1 ) 
	{
		GetDlgItemText(IDC_eCLASSIFY_CNT, strTmpItem);
		m_iAllCardListCnt = m_iAllCardListCnt + _ttoi(strTmpItem); 
	}
	
	if ( m_iAllCardListCnt == 0 )
	{
		AfxMessageBox(_T("카드목록 출력 매수를 확인해 주세요"));
		return FALSE;
	}
	
	return TRUE;

EFS_END
return FALSE;
}

BOOL CBO_CAT_PROC_0202_PRINT::GetAllPrintBuchulCnt()
{
EFS_BEGIN

	CButton *pSerial;
	CString strTmpItem;

	pSerial = (CButton *)GetDlgItem(IDC_rADDENTRY_PRINT);
	if ( pSerial->GetCheck() == 1 ) 
	{
		GetDlgItemText(IDC_eADDENTRY_CNT, strTmpItem);
		m_iAllBuchulCnt = m_iAllBuchulCnt + _ttoi(strTmpItem); 
	}

	if ( m_iAllBuchulCnt == 0)
	{
		AfxMessageBox(_T("부출지시서 출력 매수를 확인해 주세요."));
		return FALSE;
	}
	
	return TRUE;

EFS_END
return FALSE;
}


BOOL CBO_CAT_PROC_0202_PRINT::MakePrintCardList_SERIAL(CString sAddStringForBuchul)
{
EFS_BEGIN

	if ( m_list == NULL || m_sCard == _T("") )
		return FALSE;

	Sleep(300);	
	TCHAR szComName[255];	
	DWORD len;
	len =255;

	GetComputerName(szComName, &len);

	CString strNameFormat;
	strNameFormat.Format( _T("net use LPT1 \\\\%s"), szComName);
	
	INT ids = WinExec((LPSTR)(LPCTSTR)strNameFormat, SW_HIDE);
	if (ids <= 31) {
		AfxMessageBox(_T("Print Error!"));
		return -1;
	}
	Sleep(400);
	FILE* prn = _tfopen(_T("LPT1"), _T("wb"));
	if(prn == NULL)
	{
		AfxMessageBox(_T("Prn Open Error!"));
		return -1;
	}
	
		/*UNCHANGE*/fputc(0xFF, prn);
		/*UNCHANGE*/fputc(0xFE, prn);
		
	Sleep(300);
	CString sPrintString;
	CString sSearchCard = m_sCard;

	INT iPosCut = m_sCard.Find( _T("\r\n") );
	
	INT iPrnLine = -1;

	_ftprintf( prn, m_sCard );
	fflush(prn);

	fclose(prn);	

	m_sCard = sSearchCard;
	return TRUE;


EFS_END
return FALSE;
}

VOID CBO_CAT_PROC_0202_PRINT::OnSerialPrint() 
{
EFS_BEGIN

	m_iPrintType = MODE_SERIAL;
	
EFS_END	
}

VOID CBO_CAT_PROC_0202_PRINT::OnMonoPrint() 
{
EFS_BEGIN

	m_iPrintType = MODE_MONO;

EFS_END
}

CBO_CAT_PROC_0202_PRINT::~CBO_CAT_PROC_0202_PRINT()
{
}

BOOL CBO_CAT_PROC_0202_PRINT::IsCheckRegNo(CString sLeftStr)
{
EFS_BEGIN
	if ( sLeftStr == _T("") ) return FALSE;

	if ( sLeftStr.GetLength() != 12 ) return FALSE;
	else return TRUE;
		
EFS_END
return FALSE;
}

HBRUSH CBO_CAT_PROC_0202_PRINT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
