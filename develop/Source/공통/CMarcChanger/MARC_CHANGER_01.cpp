// MARC_CHANGER_01.cpp : implementation file
//

#include "stdafx.h"
#include "MARC_CHANGER_01.h"
#include "ECO_FILE_API.h"
#include "ECO_FileIO.h"
#include <direct.h>
#include "MARC_CHANGER_MARC_VIEW.h"
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMARC_CHANGER_01 dialog


CMARC_CHANGER_01::CMARC_CHANGER_01(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(CMARC_CHANGER_01::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMARC_CHANGER_01)
	m_chk049Hold = FALSE;
	m_chk090Hold = FALSE;
	//}}AFX_DATA_INIT
	
	m_strFilePath = _T("");
	pCharSetMgr_RoleWord = NULL;
	m_pIndexTool = NULL;
	m_pMarcChangerGrid = NULL;
	m_pMarcChangeCompareDM = NULL;
	m_pMarcChangerCM = NULL;
}

CMARC_CHANGER_01::~CMARC_CHANGER_01()
{
	if( m_pIndexTool != NULL)
	{
		delete m_pIndexTool;
		m_pIndexTool = NULL;
	}

	if( pCharSetMgr_RoleWord != NULL)
	{
		delete pCharSetMgr_RoleWord;
		pCharSetMgr_RoleWord = NULL;		
	}
}

void CMARC_CHANGER_01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMARC_CHANGER_01)
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
	DDX_Check(pDX, IDC_chk049Hold, m_chk049Hold);
	DDX_Check(pDX, IDC_chk090Hold, m_chk090Hold);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMARC_CHANGER_01, CDialog)
//{{AFX_MSG_MAP(CMARC_CHANGER_01)
ON_BN_CLICKED(IDC_bFILE, OnbFILE)
ON_WM_CTLCOLOR()
ON_WM_SIZE()
ON_BN_CLICKED(IDC_btnMarcView, OnbtnMarcView)
ON_BN_CLICKED(IDC_chk049Hold, Onchk049Hold)
ON_BN_CLICKED(IDC_chk090Hold, Onchk090Hold)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMARC_CHANGER_01 message handlers

BOOL CMARC_CHANGER_01::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	if(InitESL_Mgr(_T("SM_MARC_CHANGER")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return -1;
	}

	m_pMarcChangerCM = FindCM(_T("CM_MARC_CHANGER"));
	m_pMarcChangerGrid = (CESL_Grid*)m_pMarcChangerCM->FindControl(_T("MarcChangerGrid"));
	m_pMarcChangeCompareDM = FindDM(_T("DM_MARC_CHANGE_COMPARE"));
	m_pMaxRegNoDM = FindDM(_T("DM_단행_정리DM_가장큰등록번호"));
	m_pMaxRegNoDM->FreeData();
	
	InitMemberData();
	
	if(m_strParentBookDMAlias == _T("DM_구입자료관리"))
	{
		((CButton*)GetDlgItem(IDC_chk049Hold))->SetCheck(true);
		((CButton*)GetDlgItem(IDC_chk049Hold))->EnableWindow(FALSE);
		m_chk049Hold = TRUE;
	}
	
	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMARC_CHANGER_01::SetParentMgr(CString strPkAlias, CString strCM, CString strGridAlias, CString strDM, CString strMarcAlias, CString strBookDM )
{
	m_strParentCMAlias		= strCM;
	m_strParentDMAlias		= strDM;
	m_strParentBookDMAlias	= strBookDM;
	m_strGridAlias			= strGridAlias;
	m_strParentMarcAlias	= strMarcAlias;
	m_strPkAlias			= strPkAlias;
}

INT CMARC_CHANGER_01::GridDisplay()
{
	INT nRowCnt;
	nRowCnt = m_pMarcChangeCompareDM->GetRowCount();
	m_pMarcChangerGrid->Display();
	for( INT i = 0 ; i < nRowCnt ; ++i )
	{
		m_pMarcChangerGrid->SetRowHeight( i+1 ,700);
	}
	return 0;
}

void CMARC_CHANGER_01::OnbFILE() 
{
	UpdateData();
	
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
	OFN.lpstrFilter = _T("TXT파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TEXT 파일 열기");	
	
	BOOL bSuccess = GetOpenFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
	{
		m_strFilePath = OFN.lpstrFile;
		( (CEdit*) GetDlgItem(IDC_eFILE_NAME) )->SetWindowText(m_strFilePath);
		UpdateData(FALSE);		
		m_ProgressCtrl.UpdateWindow();		
		LoadTargetMarc();
		MakeCompareMarcDM();
		GridDisplay();		
	}
	else
	{
		( (CEdit*) GetDlgItem(IDC_eFILE_NAME) )->SetWindowText(_T(""));
	}
	
}

void CMARC_CHANGER_01::OnOK() 
{
	GetGridSelectCnt();

	m_pMarcChangerGrid->SetProtectSelectColumn(TRUE);
	SelectMarcChange() ;
	GridDisplay();
	m_pMarcChangerGrid->SetProtectSelectColumn(FALSE);
		
}


INT CMARC_CHANGER_01::SelectMarcChange() 
{	
	CESL_Grid		*pGrid;	
	CESL_DataMgr	*pDM;
	CString			strSourceStreamMarc;	
	CString			strTargetStreamMarc;		
	INT				nCount;
	INT				nIndex;	
	INT				nSourceIndex;
	CString			strSourceIndex;
	
	pDM = FindDM(m_strParentDMAlias);
	
	CESL_ControlMgr *pCM = FindCM(m_strParentCMAlias);
	if ( pCM == NULL ) return -100;	
	
	pGrid = (CESL_Grid*)pCM->FindControl(m_strGridAlias);	
	m_pMarcChangerGrid->SelectMakeList();
	nCount = m_pMarcChangerGrid->SelectGetCount();
	if ( nCount <= 0 ) 
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}		
	
	m_ProgressCtrl.SetRange( 0, nCount );
    m_ProgressCtrl.SetStep( 1 );
    m_ProgressCtrl.SetPos( 0 );
	
	CArray<CString, CString&> arrItem;
	CMarc		marc;
	CString		strISBN;	
	INT			nChangeCount;
	INT			nProgress;
	INT			ids;
	CString		frm;
	
	CString strRecKey;
		
	nChangeCount = 0;
	nProgress = 0;
	pDM->StartFrame();	
	nIndex = m_pMarcChangerGrid->SelectGetHeadPosition();
	while ( nIndex > -1 )
	{
		pDM->InitDBFieldData();
		if ( nIndex < 0 ) return nIndex;
		
		strSourceStreamMarc = m_pMarcChangeCompareDM->GetCellData( _T("SOURCE_MARC"), nIndex );
		strTargetStreamMarc = m_pMarcChangeCompareDM->GetCellData( _T("TARGET_MARC") , nIndex );
		if ( strSourceStreamMarc == _T("") || strTargetStreamMarc == _T("") ) 
		{
			nIndex = m_pMarcChangerGrid->SelectGetNext();
			continue;
		}
		
		strRecKey = m_pMarcChangeCompareDM->GetCellData(_T("SOURCE_MARC_REC_KEY"), nIndex );
		strSourceIndex	= m_pMarcChangeCompareDM->GetCellData(_T("MAIN_GRID_IDX"), nIndex );
		
		CString strMessage;
		ids = ChangeMarc(strSourceStreamMarc, strTargetStreamMarc, strMessage );
		if(ids == -2000)
		{
			m_pMarcChangeCompareDM->SetCellData( _T("비고"), strMessage, nIndex );
			nIndex = m_pMarcChangerGrid->SelectGetNext();
			continue;
		}

		pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strTargetStreamMarc );					
		
		ids = pDM->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);					
		
		if ( ids < 0 ) 
		{
			pDM->EndFrame();
			return ids;
		}
		nSourceIndex = _ttoi(strSourceIndex);
		pDM->SetCellData(m_strParentMarcAlias, strTargetStreamMarc, nSourceIndex );
		
		ids = UpdateIndex(nSourceIndex);
		if ( ids < 0 ) 
		{
			pDM->EndFrame();
			return ids;
		}
	
		if ( nProgress > 0 && nProgress%10 == 0 ){
			pDM->SendFrame();
			pDM->EndFrame();
			pDM->StartFrame();
		}
		
		
		nChangeCount++;
		nProgress ++;
		frm.Format( _T("%d / %d"), nProgress, nCount );
		m_ProgressCtrl.SetWindowText(frm);
		m_ProgressCtrl.StepIt();
		m_pMarcChangeCompareDM->SetCellData( _T("비고"), _T("완료"), nIndex );
		nIndex = m_pMarcChangerGrid->SelectGetNext();
	}	
	pDM->SendFrame();
	pDM->EndFrame();

	UpdateRegNo();

	if( nProgress == 0 ) {
		AfxMessageBox(_T("마크교체 대상이 없습니다."));
	}else {
		CString strMsg;
		strMsg.Format(_T(" 총 %d건 중 %d건 완료되었습니다."), nCount, nProgress);
		AfxMessageBox(strMsg);
	}

	return 0;
}

INT CMARC_CHANGER_01::MakeCompareMarcDM() 
{		
	CESL_Grid		*pGrid;	
	CESL_DataMgr	*pDM;
	CString			strSourceStreamMarc;	
	CString			strTargetStreamMarc;		
	INT				nCount;
	INT				nIndex;	
	
	pDM = FindDM(m_strParentDMAlias);
	
	CESL_ControlMgr *pCM = FindCM(m_strParentCMAlias);
	if ( pCM == NULL ) return -100;	
	
	pGrid = (CESL_Grid*)pCM->FindControl(m_strGridAlias);	
	pGrid->SelectMakeList();
	nCount = pGrid->SelectGetCount();
	if ( nCount < 0 ) 
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}		
	
	CArray<CString, CString&> arrItem;
	CMarc		marc;
	CString		strISBN;	
	INT			nItemCount;
	INT			y;
	INT			nChangeCount;
	INT			ids;
	BOOL		bFind;
	CString		frm;
	
	CString strRecKey;
	
	CString strTagData;
	CString str245TagData;
	CString str260TagData;
	CString strCompareTemp;
	CString strTemp1;
	CString strTemp2;
	INT nCompareDmIdx;
	CString strISBNList;
	CString strIsbnCnt;
	
	m_pMarcChangeCompareDM->FreeData();
	
	nChangeCount = 0;
	nCompareDmIdx = 0;
	nIndex = pGrid->SelectGetHeadPosition();
	while ( nIndex > -1 )
	{
		nIndex = pGrid->SelectGetIdx();
		if ( nIndex < 0 ) return nIndex;
		
		strSourceStreamMarc = pDM->GetCellData(m_strParentMarcAlias, nIndex );
		if ( strSourceStreamMarc == _T("") ) continue;
		
		
		m_pInfo->pMarcMgr->Decoding(strSourceStreamMarc, &marc);

		CString strSetISBN;
		ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("세트ISBN"), strSetISBN, &arrItem);		
		ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("020$a"), strISBN, &arrItem);
		nItemCount = arrItem.GetSize();
		
		m_pMarcChangeCompareDM->InsertRow(-1);
		m_pMarcChangeCompareDM->SetCellData(_T("SOURCE_MARC"), strSourceStreamMarc, nCompareDmIdx);
		pDM->GetCellData(_T("IBS_종KEY"), nIndex, strCompareTemp );
		m_pMarcChangeCompareDM->SetCellData(_T("SOURCE_MARC_REC_KEY"), strCompareTemp, nCompareDmIdx);
		strCompareTemp.Format(_T("%d"),nIndex);
		m_pMarcChangeCompareDM->SetCellData(_T("MAIN_GRID_IDX"), strCompareTemp, nCompareDmIdx);
		strTemp1.Empty();
		strTemp2.Empty();
		
		ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("245$a"), strTemp1);
		ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("245$d"), strTemp2);
		
		str245TagData.Format(_T("%s/%s"),strTemp1, strTemp2 );
		
		strTemp1.Empty();
		strTemp2.Empty();
		
		ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("260$b"), strTemp1);
		ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("260$c"), strTemp2);
		
		str260TagData.Format(_T("%s,%s"),strTemp1, strTemp2 );
		
		strIsbnCnt.Empty();
		
		if( strSetISBN.IsEmpty() )
		{
			strIsbnCnt.Format(_T("%d"),nItemCount);
		}
		else
		{
			strIsbnCnt.Format(_T("%d"),nItemCount-1);
		}

		ids = m_pMarcChangeCompareDM->SetCellData(_T("SOURCE_ISBN_CNT"), strIsbnCnt, nCompareDmIdx);

		if (ids >= 0)
		{
			nItemCount = arrItem.GetSize();	
			
			for (y = 0; y < nItemCount; y++)
			{
				strISBN = arrItem.GetAt(y);

				if( strISBN == strSetISBN )
				{
					strISBN.Empty();
					continue;
				}
				
				strISBNList += _T(" ") + strISBN;
			}
			
			for (y = 0; y < nItemCount; y++)
			{
				strISBN = arrItem.GetAt(y);

				bFind = GetMarc(strISBN, strTargetStreamMarc);
				if ( bFind )
				{
					m_pMarcChangeCompareDM->SetCellData(_T("TARGET_MARC"), strTargetStreamMarc, nCompareDmIdx );
					m_pMarcChangeCompareDM->SetCellData(_T("TARGET_TAG_DATA"), m_strTargetTagData, nCompareDmIdx );
					m_pMarcChangeCompareDM->SetCellData(_T("TARGET_ISBN_CNT"), m_strTargetIsbnCnt, nCompareDmIdx );
					
					if( m_strTargetIsbnCnt != strIsbnCnt ) 
					{
						m_pMarcChangeCompareDM->SetCellData(_T("비고"), _T("편권차를 확인해주세요."), nCompareDmIdx);
					}
					break;					
				}
			}
			
			
			if( 0 == nItemCount )
			{
				m_pMarcChangeCompareDM->SetCellData(_T("비고"), _T("선정자료에 ISBN이 없습니다."), nCompareDmIdx);
			}
			else if( !bFind )
			{
				m_pMarcChangeCompareDM->SetCellData(_T("비고"), _T("매칭되는 자료가 없습니다. \n( 기준 : ISBN )"), nCompareDmIdx);
			}
			strISBNList.TrimLeft();
			strTagData.Format( _T("%s\n%s\n%s"), strISBNList, str245TagData, str260TagData );
			m_pMarcChangeCompareDM->SetCellData(_T("SOURCE_TAG_DATA"), strTagData, nCompareDmIdx);
			strISBNList.Empty();
		}
		nIndex = pGrid->SelectGetNext();
		++nCompareDmIdx;
	}	
	
	return 0;
}

INT	CMARC_CHANGER_01::ChangeMarc()
{
	CESL_Grid		*pGrid;	
	CESL_DataMgr	*pDM;
	CString			strSourceStreamMarc;	
	CString			strTargetStreamMarc;		
	INT				nCount;
	INT				nIndex;	
	
	pDM = FindDM(m_strParentDMAlias);
	
	CESL_ControlMgr *pCM = FindCM(m_strParentCMAlias);
	if ( pCM == NULL ) return -100;	
	
	pGrid = (CESL_Grid*)pCM->FindControl(m_strGridAlias);	
	pGrid->SelectMakeList();
	nCount = pGrid->SelectGetCount();
	if ( nCount < 0 ) 
	{
		AfxMessageBox(_T("선정된 자료가 없습니다."));
		return 0;
	}		
	
	m_ProgressCtrl.SetRange( 0, nCount );
    m_ProgressCtrl.SetStep( 1 );
    m_ProgressCtrl.SetPos( 0 );
	
	CArray<CString, CString&> arrItem;
	CMarc		marc;
	CString		strISBN;	
	INT			nItemCount;
	INT			y;
	INT			nChangeCount;
	INT			nProgress;
	INT			ids;
	BOOL		bFind;
	CString		frm;
	
	CString strRecKey;
	
	
	nChangeCount = 0;
	nProgress = 0;
	pDM->StartFrame();	
	nIndex = pGrid->SelectGetHeadPosition();
	while ( nIndex > -1 )
	{
		pDM->InitDBFieldData();
		nIndex = pGrid->SelectGetIdx();
		if ( nIndex < 0 ) return nIndex;
		
		strSourceStreamMarc = pDM->GetCellData(m_strParentMarcAlias, nIndex );
		if ( strSourceStreamMarc == _T("") ) continue;
		
		m_pInfo->pMarcMgr->Decoding(strSourceStreamMarc, &marc);
		
		CString strSetISBN;
		ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("세트ISBN"), strSetISBN,&arrItem);	
		ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("020$a"), strISBN, &arrItem);
		if (ids >= 0)
		{
			nItemCount = arrItem.GetSize();
			for (y = 0; y < nItemCount; y++)
			{
				strISBN = arrItem.GetAt(y);

				if( strISBN == strSetISBN )
				{
					strISBN.Empty();
					continue;
				}
				
				bFind = GetMarc(strISBN, strTargetStreamMarc);
				if ( bFind )
				{
					strRecKey = pDM->GetCellData(m_strPkAlias, nIndex );
					
					pDM->AddDBFieldData(_T("MARC"), _T("STRING"), strTargetStreamMarc );					
					
					ids = pDM->MakeUpdateFrame(_T("BO_SPECIES_TBL"), _T("REC_KEY"), _T("NUMERIC"), strRecKey);					
					
					if ( ids < 0 ) 
					{
						pDM->EndFrame();
						return ids;
					}
					pDM->SetCellData(m_strParentMarcAlias, strTargetStreamMarc, nIndex );
					
					ids = UpdateIndex(nIndex);
					if ( ids < 0 ) 
					{
						pDM->EndFrame();
						return ids;
					}
					
					nChangeCount++;
					break;					
				}
			}			
		}
		nProgress ++;
		frm.Format( _T("%d / %d"), nProgress, nCount );
		m_ProgressCtrl.SetWindowText(frm);
		m_ProgressCtrl.StepIt();		
		nIndex = pGrid->SelectGetNext();
	} 
	pDM->SendFrame();
	pDM->EndFrame();
	
	return 0;
}

VOID CMARC_CHANGER_01::LoadTargetMarc()
{
	CStdioFile file;	
	if( file.Open(m_strFilePath, CFile::modeRead | CFile::typeBinary ) == FALSE )
	{
		AfxMessageBox(_T("파일열기에 실패했습니다."));
		return ;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}

	CString strLineData;
	INT nPos;
	
	CMarc		marc;
	CString		strISBN;
	CString		strISBNList;
	INT			ids;
	INT			nItemCount;

	CString strTagData;
	CString str245TagData;
	CString str260TagData;
	CString strTemp1;
	CString strTemp2;
	CString strIsbnCnt;
	CString strStreamMarc;
	CArray<CString, CString&> arrItem;
	CESL_DataMgr *pDM = FindDM(_T("DM_MARC_CHANGER"));
	INT nIndex = 0;

	while( file.ReadString(strLineData) )
	{
		while( 1 )
		{
			nPos = strLineData.Find(29,0);
			if( 0 > nPos )
			{
				break;
			}
			strStreamMarc = strLineData.Left(nPos);
			strLineData = strLineData.Mid(nPos+1);
			
			strStreamMarc.TrimLeft();	strStreamMarc.TrimRight();			
			strLineData.TrimLeft()	;	strLineData.TrimRight();

			m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);

			CString strSetISBN;
			ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("세트ISBN"), strSetISBN, &arrItem);			
			ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("020$a"), strISBN, &arrItem);		

			strISBNList = _T("");
			if (ids >= 0)
			{
				pDM->InsertRow(-1);
				nItemCount = arrItem.GetSize();
				for (INT i  = 0; i < nItemCount; i++)
				{
					strISBN = arrItem.GetAt(i);
					if( strISBN == strSetISBN )
					{
						strISBN.Empty();
						continue;
					}
					strISBNList += " " + strISBN;
				}
				pDM->SetCellData(_T("MARC"), strStreamMarc, nIndex);
				pDM->SetCellData(_T("ISBN"), strISBNList, nIndex);
				
				strTemp1.Empty();
				strTemp2.Empty();
				
				ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("245$a"), strTemp1);
				ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("245$d"), strTemp2);

				str245TagData.Format(_T("%s/%s"),strTemp1, strTemp2 );

				strTemp1.Empty();
				strTemp2.Empty();
				
				ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("260$b"), strTemp1);
				ids = m_pInfo->pMarcMgr->GetItem(&marc, _T("260$c"), strTemp2);
				
				str260TagData.Format(_T("%s,%s"),strTemp1, strTemp2 );
				strISBNList.TrimLeft();
				strTagData.Format( _T("%s\n%s\n%s"), strISBNList, str245TagData, str260TagData );

				pDM->SetCellData(_T("TAG_DATA"), strTagData, nIndex);
				strIsbnCnt.Empty();
				
				if( strSetISBN.IsEmpty() )
				{
					strIsbnCnt.Format(_T("%d"),nItemCount);
				}
				else
				{
					strIsbnCnt.Format(_T("%d"),nItemCount-1);
				}
				
				pDM->SetCellData(_T("ISBN_CNT"), strIsbnCnt, nIndex);
				nIndex++;
			}
			m_ProgressCtrl.StepIt();
		}
	}

	m_ProgressCtrl.SetPos( 0 );
	file.Close();	
}

BOOL CMARC_CHANGER_01::GetMarc(CString strISBN, CString &strStreamMarc)
{
	INT	nCount;	
	
	CString strISBNList;
	CString strTemp;
	INT nLength;
	
	CESL_DataMgr *pDM = FindDM(_T("DM_MARC_CHANGER"));
	
	strStreamMarc = _T("");	
	nCount = pDM->GetRowCount();
	INT nFind = strISBN.Find(_T("("));
	nLength = strISBN.GetLength();
	if( 0 < nFind )
	{
		strTemp = strISBN.Left(nFind);
		strISBN = strTemp;
		nLength = strISBN.GetLength();	
	}
	
	
	if( 13 == nLength )
	{
		strTemp =  strISBN.Mid( 3, nLength -4 );
		strISBN = strTemp;
	}
	else
	{
		strTemp =  strISBN.Mid( 0, nLength -1 );
		strISBN = strTemp;
	}

	for ( int i=0 ; i<nCount ; i++ )
	{
		strISBNList = pDM->GetCellData(_T("ISBN"), i );
		if ( strISBNList.Find(strISBN) >= 0 )
		{
			strStreamMarc = pDM->GetCellData(_T("MARC"), i );	

			m_strTargetTagData.Empty();
			pDM->GetCellData(	_T("TAG_DATA")	, i	, m_strTargetTagData );
			
			m_strTargetIsbnCnt.Empty();
			pDM->GetCellData(	_T("ISBN_CNT")	, i	, m_strTargetIsbnCnt );	
			
			return TRUE;
		}		
	}
	return FALSE;
}

INT	CMARC_CHANGER_01::ChangeMarc(CString strSourceMarc, CString &strTargetMarc, CString &strMessage)
{
	CMarc	srcMarc;
	CMarc	tarMarc;	
	CString strField;
	CString strItem;
	INT		ids;
	
	m_pInfo->pMarcMgr->Decoding(strSourceMarc, &srcMarc);
	m_pInfo->pMarcMgr->Decoding(strTargetMarc, &tarMarc);	
	
	ids = m_pInfo->pMarcMgr->DeleteField(&tarMarc, _T("001") ); 
	
	if( m_chk090Hold ) {
		ids = m_pInfo->pMarcMgr->DeleteField(&tarMarc, _T("090") );	
	}

	CArray<CString, CString&> arrRegNo;
	CString strTempData;
	CString strQuery;
	CString strResult;
	
	if( 1 == m_chk049Hold )
	{
		ids = m_pInfo->pMarcMgr->DeleteField(&tarMarc, _T("049") );
		
		ids = m_pInfo->pMarcMgr->GetField(&srcMarc, _T("049"), strField ); 
		if ( !strField.IsEmpty() )
		{
			ids = m_pInfo->pMarcMgr->InsertField(&tarMarc, strField);
		}
	}	
	else
	{
		CString strDupRegNo;
		ids = m_pInfo->pMarcMgr->GetItem(&tarMarc, _T("049$l"), strTempData, &arrRegNo);
		INT nRegNoCnt = arrRegNo.GetSize();

		CString str049field,strRegNo;
		ids = m_pInfo->pMarcMgr->GetField(&tarMarc, _T("049"), str049field ); 
		for(INT i=0; i<nRegNoCnt; i++)
		{
			strTempData = arrRegNo.GetAt( i );
			strRegNo = strTempData;
			CLocCommonAPI::MakeRegNoSpaceToZero(strTempData);
			str049field.Replace(strRegNo,strTempData);
			strQuery.Format(_T("SELECT COUNT(1) FROM BO_BOOK_TBL WHERE REG_NO ='%s'"),strTempData);
			m_pMarcChangeCompareDM->GetOneValueQuery(strQuery,strResult);
			if(strResult != _T("0"))
			{
				if(!strDupRegNo.IsEmpty())
						strDupRegNo += _T(",");
				strDupRegNo += strTempData;
			}
		}
		if(!strDupRegNo.IsEmpty())
		{
			strMessage.Format(_T("교체실패\n(중복된 등록번호가 존재합니다.\n%s)"),strDupRegNo);
			return -2000;
		}

		m_pInfo->pMarcMgr->DeleteField(&tarMarc,_T("049"));
		m_pInfo->pMarcMgr->InsertField(&tarMarc,_T("049") + str049field);
	}

	CString strMarcAlias;

	m_pInfo->pMarcMgr->GetItem( &tarMarc, _T("245$c"), strItem);
	if( !strItem.IsEmpty() )
	{
		m_mapMarcAlias.Lookup(_T("부서명"), strMarcAlias);

		ids = m_pInfo->pMarcMgr->SetItem(&tarMarc, strMarcAlias, strItem);
		ids = m_pInfo->pMarcMgr->DeleteItem(&tarMarc, _T("245$c"),strItem);
	}
	
	ids = m_pInfo->pMarcMgr->Encoding(&tarMarc, strTargetMarc);	
	
	return 0;
}

CString CutISBN(CString strISBN)
{
	INT nLength = 0;
	CString strTemp;
	INT nFind = strISBN.Find(_T("("));
	nLength = strISBN.GetLength();
	if( 0 < nFind )
	{
		strTemp = strISBN.Left(nFind);
		strISBN = strTemp;
		nLength = strISBN.GetLength();	
	}
	
	
	if( 13 == nLength )
	{
		strTemp =  strISBN.Mid( 3, nLength -4 );
		strISBN = strTemp;
	}
	else
	{
		strTemp =  strISBN.Mid( 0, nLength -1 );
		strISBN = strTemp;
	}
	return strISBN;
}

BOOL CheckIsDigit(CString strTemp)
{
	if(strTemp.IsEmpty()) return FALSE;
	
	TCHAR * tmp = strTemp.GetBuffer(0);
	INT size = strTemp.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] < '0' || tmp[i] > '9') 
			return FALSE;
	}
	return TRUE;
}

CString GetUpdateISBN(CString strBookISBN,CString str020ISBN)
{
	CString strResult = _T("");
	CString strCutBookISBN,strCut020ISBN;
	strCutBookISBN = CutISBN(strBookISBN);
	strCut020ISBN = CutISBN(str020ISBN);
	
	if(strCutBookISBN == strCut020ISBN)
	{
		if(str020ISBN.GetLength() == 10 || str020ISBN.GetLength()==13)
		{
			strResult = str020ISBN;
		}
	}
	else
	{}
	
	return strResult;
}

VOID GetPriceInfo(CESL_DataMgr * pDM,CString str020Price,CString& strPrice,CString& strCurrency_Code,CString& strExchange_Rate)
{
	if(str020Price.IsEmpty())
	{
		return;
	}
	TCHAR * tmp = str020Price.GetBuffer(0);
	INT size = str020Price.GetLength();
	for(INT i=0 ; i<size ; i++)
	{
		if(tmp[i] >= '0' && tmp[i] <= '9') 
			break;
	}
	
	strCurrency_Code = str020Price.Left(i);
	strPrice = str020Price.Mid(i);
	
	CString strQuery;
	strQuery.Format(_T("SELECT EXCHANGE_RATE FROM CO_EXCHANGE_RATE_TBL WHERE MANAGE_CODE = UDF_MANAGE_CODE AND USER_CODE_MNG_KEY IN (SELECT REC_KEY FROM CD_CODE_TBL WHERE CODE LIKE '%s%%') AND APPLY_YN = 'Y' AND INPUT_DATE >= (SELECT TRUNC(SYSDATE, 'YEAR') FROM DUAL) AND INPUT_DATE < (SELECT ADD_MONTHS(TRUNC(SYSDATE, 'YEAR'), 12) FROM DUAL)")
		, strCurrency_Code);
	pDM->GetOneValueQuery(strQuery,strExchange_Rate);
}

INT CMARC_CHANGER_01::UpdateIndex(INT idx)
{
	if (idx < 0)
		return -1;
	
	INT ids;
	
	CESL_DataMgr *pDM = FindDM(m_strParentDMAlias);
	if (pDM == NULL) return -1;
	
	CESL_DataMgr *pDM_Index = FindDM(_T("DM_단행_정리DM_색인정보"));
	if (pDM_Index == NULL) return -1;
	
	CESL_DataMgr	*pBookDM;
	INT nBookDMIdx;
	INT nBookDMCnt;
	CString strBookDmTemp;
	CString strBookKey;
	CString strBookDmSpeciesKey;
	CString strUpdateBookQuery;
	CString strTempBookQuery;
	CString strVolSortNo;
	
	CString strWorkingStatus;
	CString strBookISBN;
	CString strACQKey;
	BOOL b049Exist = FALSE;
	
	INT nPriceSum=0;
	
	pBookDM = FindDM(m_strParentBookDMAlias);
	if (pBookDM == NULL) return -1;
	strUpdateBookQuery = _T("");
	
	if (idx >= pDM->GetRowCount())
		return -1;
	
	CString strMarc;
	CString strSpeciesKey;
	CString strWhere;
	CString strValue;
	ids = pDM->GetCellData(m_strParentMarcAlias, idx, strMarc);
	if (ids < 0) return -1;
	ids = pDM->GetCellData(m_strPkAlias, idx, strSpeciesKey);
	if (ids < 0) return -1;
	CMarc pMarc;
	m_pInfo->pMarcMgr->Decoding(strMarc, &pMarc);
	
	CString strTempData;
	CArray<CString, CString&> arrTempData;
	CArray<CString, CString&> arr020ISBN;
	CArray<CString, CString&> arr020Price;
	CArray<CString, CString&> arrRegNo;
	CString strUpdateQuery;
	CString strUpdateBookKey;
	CString strUpdateISBN;
	
	ids = m_pInfo->pMarcMgr->GetItem(&pMarc, _T("049$l"), strTempData, &arrRegNo);	
	if ( ids > -1 )
	{
		CString strArrRegNo,strArrTmp;
		int nArrRegNo;
		for (int k = 0 ; k < arrRegNo.GetSize() ; k++ )
		{
			strArrRegNo = arrRegNo.GetAt(k);
			strArrTmp = strArrRegNo.Mid( m_pInfo->nREG_CODE_LENGTH );
			nArrRegNo = _ttoi(strArrTmp);
			SetMaxRegNo( m_pMaxRegNoDM, strArrRegNo.Mid( 0, m_pInfo->nREG_CODE_LENGTH ), nArrRegNo);
		}		
	}	
	
	if(!strTempData.IsEmpty()) b049Exist = TRUE;

	CString strSetISBN;
	ids = m_pInfo->pMarcMgr->GetItem(&pMarc, _T("세트ISBN"), strSetISBN, &arr020ISBN);
	ids = m_pInfo->pMarcMgr->GetItem(&pMarc, _T("020$a"), strTempData, &arr020ISBN);
	ids = m_pInfo->pMarcMgr->GetItem(&pMarc, _T("020$c"), strTempData, &arr020Price);

	CString str056Field;
	CString strSubjectCode;
	INT nFieldidx;
	m_pInfo->pMarcMgr->GetItem(&pMarc, _T("056$a"), str056Field);

	if(!str056Field.IsEmpty())
	{
		strSubjectCode = _T("");
		nFieldidx = str056Field.FindOneOf(_T("a"));		
		strSubjectCode = str056Field.GetAt(0);		
		
	}
	
	INT nRegNoCnt = arrRegNo.GetSize(); 

	nBookDMCnt = pBookDM->GetRowCount();
	INT nCnt = 0;
	INT nRegNoChk = 0;
	
	CString strRegNo;
	CString strRegCode;
	INT nRegCodeLength;
	
	CString strTemp;
	nRegCodeLength = m_pInfo->nREG_CODE_LENGTH;
	CString strRegCodeData;
	CStringArray astrVolSortNo;

	BOOL bVolNortNo;
	CString strOrderPrice;
	INT nOrderPrice = 0;
	CString strDiscRate;

	DOUBLE dOrderPrice = 0;
	DOUBLE dDiscRate = 0;
	DOUBLE dExchangeRate = 0;
	CString strOrderPriceTemp;
	TCHAR *pEndPtr;

	for( INT i = 0 ; i < nBookDMCnt ; i++ )
	{
		bVolNortNo = TRUE;
		pBookDM->GetCellData(_T("IBS_종KEY") , i, strBookDmSpeciesKey );
		pBookDM->GetCellData(_T("BB_책KEY") , i, strUpdateBookKey );
		
		if( strBookDmSpeciesKey == strSpeciesKey )
		{
			pBookDM->GetCellData(_T("BB_낱권ISBN") , i, strBookISBN );
			pBookDM->GetCellData(_T("BP_수서정보KEY") , i, strACQKey );
			INT n020IsbnCnt = arr020ISBN.GetSize();
			
			pBookDM->GetCellData(_T("BB_가격"),i,strTemp);
			pBookDM->GetCellData(_T("BB_권일련번호"),i,strVolSortNo);
			for( INT nVolSortNoIdx = 0 ; nVolSortNoIdx < astrVolSortNo.GetSize() ; ++nVolSortNoIdx )
			{
				if( strVolSortNo == astrVolSortNo.GetAt(nVolSortNoIdx) )
				{
					bVolNortNo = FALSE;
				}
				break;
			}
			if( bVolNortNo )
			{
				astrVolSortNo.Add( strVolSortNo );

			}			

			if( bVolNortNo )
			{
				if(!strTemp.IsEmpty())
				{
					nPriceSum += _ttoi(strTemp);
				}
			}
			if(!strTemp.IsEmpty())
			{
				nOrderPrice += _ttoi(strTemp);
			}
			for(INT nArrIdx=0; nArrIdx<n020IsbnCnt; nArrIdx++)
			{	
				INT nPos;
				strUpdateISBN = arr020ISBN.GetAt(nArrIdx);
				if( strUpdateISBN == strSetISBN )
				{
					strUpdateISBN.Empty();
					continue;
				}
				nPos = strUpdateISBN.Find(_T("("));
				if(nPos != -1)
				{
					strUpdateISBN = strUpdateISBN.Left(nPos);
				}
				strUpdateISBN = GetUpdateISBN(strBookISBN,strUpdateISBN);
				
				if( !strUpdateISBN.IsEmpty() )
				{
					break;
				}
			}
			
			if(!strUpdateISBN.IsEmpty())
			{
				CString strPrice,strCurrency_Code,strExchange_Rate;
				
				pBookDM->GetCellData(_T("BB_가격"),i,strTemp);				

				if( bVolNortNo )
				{
					if(!strTemp.IsEmpty())
					{
						nPriceSum -= _ttoi(strTemp);
					}
				}
				if(!strTemp.IsEmpty())
				{
					nOrderPrice -= _ttoi(strTemp);
				}

				if( arr020Price.GetSize() > 0 ) {
					GetPriceInfo(pDM,arr020Price.GetAt(nArrIdx),strPrice,strCurrency_Code,strExchange_Rate);
					
					pBookDM->SetCellData(_T("BP_화폐구분"),strCurrency_Code,i);
					pBookDM->SetCellData(_T("UDF_화폐구분"),strCurrency_Code,i);
					pBookDM->SetCellData(_T("BB_가격"),strPrice,i);
					pBookDM->SetCellData(_T("UDF_단가"),strPrice,i);
				}
				pBookDM->GetCellData(_T("BP_할인율"), i, strDiscRate);	
				
				if( bVolNortNo )
				{
					if(!strPrice.IsEmpty())	
					{
						nPriceSum += _ttoi(strPrice);
					}
				}
				if(!strTemp.IsEmpty())
				{
					nOrderPrice += _ttoi(strPrice);
				}
				
				if(strPrice.IsEmpty()) strPrice=_T("NULL");
				if(strExchange_Rate.IsEmpty()) strExchange_Rate=_T("NULL");
				
				if(nOrderPrice != 0)
				{
					dOrderPrice = (DOUBLE)nOrderPrice;					
					dDiscRate = _tcstod(strDiscRate.GetBuffer(0), &pEndPtr);
					dExchangeRate = _tcstod(strExchange_Rate.GetBuffer(0), &pEndPtr);
					dOrderPrice = (dOrderPrice - ((dOrderPrice*dDiscRate)/100)) * dExchangeRate;
					strOrderPrice.Format(_T("%.2f"),dOrderPrice);
				}
				CString strPurchaseInfoQuery;
				
				strPurchaseInfoQuery.Format(_T("UPDATE BO_PURCHASEINFO_TBL SET PRICE=%d,CURRENCY_CODE='%s',EXCHANGE_RATE=%s,SUBJECT_CODE='%s', ORDER_PRICE='%s'  WHERE REC_KEY=%s;")
					,nPriceSum,strCurrency_Code,strExchange_Rate, strSubjectCode, strOrderPrice,strACQKey);
				pDM->SetCellData(_T("BP_주제구분"),strSubjectCode,i);
				
				pDM->AddFrame(strPurchaseInfoQuery);
				
				if ( m_chk049Hold == FALSE &&  0 < nRegNoCnt )
				{
					if( nRegNoCnt > nRegNoChk )
					{
						strRegNo = _T("");
						strRegNo = arrRegNo.GetAt( nRegNoChk );
						pBookDM->SetCellData(_T("BB_등록번호"),strRegNo, i);
						pBookDM->SetCellData(_T("시작등록번호"),strRegNo, i);
						strRegCode = strRegNo.Left(nRegCodeLength);
						++nRegNoChk;
					}
					
					pBookDM->GetCellData(_T("BB_자료상태_설명") , i, strWorkingStatus );
					if(strWorkingStatus == _T("등록인계자료") && b049Exist)
					{
						strUpdateQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_CODE = '%s', REG_NO = '%s', WORKING_STATUS='BOR112N', EA_ISBN='%s', PRICE=%s  WHERE REC_KEY = %s;"),
							strRegCode, strRegNo, strUpdateISBN, strPrice,strUpdateBookKey);
						pDM->SetCellData(_T("BB_자료상태_코드"),_T("BOR112N"),i);
						pDM->SetCellData(_T("BB_자료상태_설명"),_T("등록자료"),i);
					}
					else
					{
						strUpdateQuery.Format(_T("UPDATE BO_BOOK_TBL SET REG_CODE = '%s', REG_NO = '%s', EA_ISBN='%s', PRICE=%s WHERE REC_KEY = %s;"),
							strRegCode, strRegNo, strUpdateISBN, strPrice,strUpdateBookKey);
					}	
					pBookDM->SetCellData(_T("BB_등록구분_코드"),strRegCode, i);
					
					CString sQuery,sCount,strNum;
					strNum	= strRegNo.Mid( nRegCodeLength );
					INT nNum = _ttoi(strNum);

					sQuery.Format( _T("DELETE CO_DROP_ACCESSION_NO_TBL WHERE REG_CODE ='%s' AND REG_NO ='%d' AND MANAGE_CODE = UDF_MANAGE_CODE;") , strRegCode , nNum);
					
					pDM->AddFrame(sQuery);
				}
				else
				{
					
					strUpdateQuery.Format(_T("UPDATE BO_BOOK_TBL SET EA_ISBN='%s', PRICE=%s WHERE REC_KEY = %s;"),
						strUpdateISBN, strPrice,strUpdateBookKey);
				}
				
				CString strTempRegCode;
				pBookDM->GetCellData(_T("BB_등록구분_코드"), i, strTempRegCode);
				
				if( strRegCodeData.IsEmpty() ) 
				{
					strRegCodeData = strTempRegCode;
				}
				else
				{
					INT nPos = strRegCodeData.Find(strTempRegCode);
					if( 0 > nPos)
					{
						strRegCodeData += _T(" ") + strTempRegCode;
					}
					
				}				
				
				ids = pDM->AddFrame(strUpdateQuery);
				if (ids < 0) 
				{
					return ids;
				}
				
			}
			
			pBookDM->GetCellData(_T("BB_대표책여부") , i, strBookDmTemp );
			if( strBookDmTemp == _T("Y") )
			{
				pBookDM->GetCellData(_T("BB_권일련번호") , i, strVolSortNo );
				nBookDMIdx = i;
			
			}
	}
	}
	CString strPriceSum;
	if(nPriceSum != 0)
	{
		strPriceSum.Format(_T("%d"),nPriceSum);
		pDM->SetCellData(_T("UDF_종가격"),strPriceSum,idx);
		pDM->SetCellData(_T("UDF_단가"),strPriceSum,idx);		
		pDM->SetCellData(_T("BP_가격"),strPriceSum,idx);
		pDM->SetCellData(_T("BP_납품가격"),strPriceSum,idx);
	}

	CString strBookKeyTemp;
	for( INT nBookIdx = 0 ; nBookIdx < pBookDM->GetRowCount() ;  ++nBookIdx)
	{
		pBookDM->GetCellData(_T("IBS_종KEY") , nBookIdx, strBookKeyTemp );
		
		if( strBookKeyTemp == strSpeciesKey )
		{
			pBookDM->SetCellData(_T("BP_발주가격"),strOrderPrice,nBookIdx);
			pBookDM->SetCellData(_T("BP_가격"),strPriceSum,nBookIdx);
		}
	}
	
	strWhere.Format(_T("REC_KEY = %s"), strSpeciesKey );
	pDM_Index->RefreshDataManager(strWhere);
	
	CString strCallNo;
	CString strBookCode;
	
	if( !m_chk090Hold ) {
		m_pInfo->pMarcMgr->GetItem(&pMarc, _T("090$b"), strBookCode);
		m_pInfo->pMarcMgr->GetItem(&pMarc, _T("090$a"), strCallNo);
		
		if(!str056Field.IsEmpty())pDM_Index->SetCellData(_T("IBS_분류기호KDC"), str056Field, 0);
		if(!strBookCode.IsEmpty())pDM_Index->SetCellData(_T("IBS_도서기호"), strBookCode, 0);
		if(!strCallNo.IsEmpty())pDM_Index->SetCellData(_T("IBS_분류기호"), strCallNo, 0);
	}
	
	ids = -1;
	INT nVolCount = -1;
	
	CString strDMFieldAlias;
	CString strMarcAlias;
	CString strItemData;
	CArray<CString, CString&> arrItemData;
	
	POSITION pos = m_mapCatDMtoMarc.GetStartPosition();
	while (pos)
	{
		m_mapCatDMtoMarc.GetNextAssoc(pos, strDMFieldAlias, strMarcAlias);
		
		ids = m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
		if (ids < 0) continue;
		
		if (strDMFieldAlias.Find(_T("IBS")) >= 0)
		{
			pDM_Index->SetCellData(strDMFieldAlias, strItemData, 0);
			if (strDMFieldAlias==_T("IBS_발행년") )			
			{
				pDM->SetCellData( _T("IBS_발행년도2"), strItemData, idx);
				//LKS
				pDM->SetCellData( _T("IBS_발행년"), strItemData, idx);
			}
			else
			{
				pDM->SetCellData(strDMFieldAlias, strItemData, idx);
			}
		}
	}
	
	POSITION posTag = NULL;
	POSITION posSubfield = NULL;
	
	CString strTitle;
	CTag *pTag = NULL;
	CSubfield *pSubfield = NULL;
	CString strSubfieldCode;
	posTag = pMarc.m_listTag.GetHeadPosition();
	while (posTag)
	{
		pTag = (CTag*)pMarc.m_listTag.GetNext(posTag);
		if (pTag && pTag->GetTagCode() == _T("245"))
		{
			posSubfield = pTag->m_listSubfield.GetHeadPosition();
			while (posSubfield)
			{
				pSubfield = (CSubfield*)pTag->m_listSubfield.GetNext(posSubfield);
				if (!pSubfield) continue;
				
				strSubfieldCode = pSubfield->GetSubfieldCode();
				strSubfieldCode.Replace(SUBFIELD_CODE, '$');
				
				if (strSubfieldCode == _T("$a") ||
					strSubfieldCode == _T("$x") ||
					strSubfieldCode == _T("$b"))
				{
					strTitle += pSubfield->GetPunctuationMark() + pSubfield->GetItemData();
				}
			}
		}
	}
	
	pDM_Index->SetCellData(_T("IBS_본표제"), strTitle, 0);
	pDM->SetCellData(_T("IBS_본표제"), strTitle, idx);	
	
	m_mapMarcAlias.Lookup(_T("저자"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	{
		CString strAuthor = strItemData;
		arrItemData.RemoveAll();
		m_mapMarcAlias.Lookup(_T("공저자"), strMarcAlias);
		ids = m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData, &arrItemData);
		if (ids >= 0)
		{
			INT nCount = arrItemData.GetSize();
			for (INT idx = 0; idx < nCount; idx++)
			{
				strItemData = arrItemData.GetAt(idx);
				if (strItemData.IsEmpty()) continue;
				
				strAuthor += _T(";") + strItemData;
			}
		}
		
		pDM_Index->SetCellData(_T("IBS_저작자"), strAuthor, 0);
		pDM->SetCellData(_T("IBS_저작자"), strAuthor, idx);
	}
	
	CString strPublisher;
	arrItemData.RemoveAll();
	
	m_mapMarcAlias.Lookup(_T("발행자"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData, &arrItemData);	
	{
		strPublisher = strItemData;
		INT nCount = arrItemData.GetSize();
		for (INT idx = 1; idx < nCount; idx++)
		{
			strItemData = arrItemData.GetAt(idx);
			if (strItemData.IsEmpty()) continue;
			
			strPublisher += _T(":") + strItemData;
		}
		
		
		pDM_Index->SetCellData(_T("IBS_발행자"), strPublisher, 0);		
	}
	

	if (m_pIndexTool == NULL)
		m_pIndexTool = new CIndexTool;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("통합마크"));
	m_pMarcMgr = m_pInfo->pMarcMgr;
	m_pIndexTool->Init(m_pMarcMgr, &pMarc);
	
	CString strIndexItem;
	CString strIndexDMFieldAlias;
	CString strIndexToolKitAlias;
	pos = m_mapIndexDMtoIndexToolKit.GetStartPosition();
	while (pos)
	{
		strIndexItem.Empty();
		m_mapIndexDMtoIndexToolKit.GetNextAssoc(pos, strIndexDMFieldAlias, strIndexToolKitAlias);
		
		ids = m_pIndexTool->GetIndex(strIndexToolKitAlias, strIndexItem);
		if (ids < 0) continue;
		
		pDM_Index->SetCellData(strIndexDMFieldAlias, strIndexItem, 0);
	}
	
	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_저작자"), 0, strItemData);
	if (strItemData.IsEmpty())
	{
		m_pInfo->pMarcMgr->GetItem(&pMarc, _T("245$d"), strItemData);
		DeleteRoleWord(strItemData);
		pDM_Index->SetCellData( _T("IBS_저작자"), strItemData, 0);
		
		if (strItemData.IsEmpty()) 
		{
			CString strFormCode;
			pDM_Index->GetCellData( _T("IBS_형식구분_코드"), 0, strFormCode);
			if (strFormCode == _T("BK")) 
			{
				m_pInfo->pMarcMgr->GetItem(&pMarc, _T("100$a"), strItemData);
				pDM_Index->SetCellData( _T("IBS_저작자"), strItemData, 0);
			} 
			else 
			{
				m_pInfo->pMarcMgr->GetItem(&pMarc, _T("700$a"), strItemData);
				pDM_Index->SetCellData( _T("IBS_저작자"), strItemData, 0);
			}
		}
	}
	
	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_본표제"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		pDM_Index->GetCellData( _T("IBS_검색용본표제"), 0, strItemData);
		pDM_Index->SetCellData( _T("IBS_본표제"), strItemData, 0);
	}
	
	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_검색용범위검색발행자"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("발행자"), strItemData);
		pDM_Index->SetCellData( _T("IBS_검색용범위검색발행자"), strItemData, 0);
	}
	
	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_발행자"), 0, strItemData);
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("발행자"), strItemData);
		pDM_Index->SetCellData( _T("IBS_발행자"), strItemData, 0);
	}
	
	strItemData.Empty();
	ids = pDM_Index->GetCellData( _T("IBS_발행년"), 0, strItemData);	
	if (ids < 0 || strItemData.IsEmpty())
	{
		m_pIndexTool->GetIndex(_T("범위검색용발행년도"), strItemData);
		pDM_Index->SetCellData( _T("IBS_발행년"), strItemData, 0);		
	}
	
	strItemData.Empty();
	m_pInfo->pMarcMgr->GetItem( &pMarc , _T("008@15-17"), strItemData );
	pDM_Index->SetCellData( _T("IBS_발행국코드"), strItemData, 0);
	
	CString strIdxTitle;
	CString strIdxAuthor;
	CString strIdxPublisher;
	CString strIdxKeyword;
	CString strIdxSubject;
	
	ids = pDM_Index->GetCellData( _T("IBS_검색용본표제"), 0, strIdxTitle);
	ids = pDM_Index->GetCellData( _T("IBS_검색용저작자"), 0, strIdxAuthor);
	ids = pDM_Index->GetCellData( _T("IBS_검색용발행자"), 0, strIdxPublisher);
	ids = pDM_Index->GetCellData( _T("IBS_검색용키워드"), 0, strIdxKeyword);
	
	
	ids = pDM_Index->GetCellData( _T("IBS_검색용키워드"), 0, strIdxKeyword);
	
	ids = pDM_Index->GetCellData( _T("IBS_주제명파일"), 0, strIdxSubject);	
	
	CString strIdxAllItem = strIdxTitle + _T(" ") + strIdxAuthor + _T(" ") + strIdxPublisher + _T(" ") + strIdxKeyword + _T(" ") +strIdxSubject;
	CStringArray arrStrTarget;
	MakeIndexArray(&arrStrTarget, strIdxAllItem,_T(" "));
	
	INT nCount = arrStrTarget.GetSize();
	
	strIdxAllItem.Empty();
	
	for (INT j = 0 ; j < nCount ; j++)
	{
		strIndexItem = arrStrTarget.GetAt(j);
		
		if(strIdxAllItem.IsEmpty()) 
			strIdxAllItem = strIndexItem;
		else 
			strIdxAllItem += _T(" ") + strIndexItem;
	}
	
	pDM_Index->SetCellData( _T("IBS_전체검색용색인어"), strIdxAllItem, 0);	
	pDM_Index->SetCellData( _T("IBS_등록구분_코드"), strRegCodeData, 0);	
	
	CArray <CString, CString> RemoveAliasList;
	RemoveAliasList.RemoveAll();
	RemoveAliasList.Add( _T("IBS_REC_KEY"));	
	
	pDM->InitDBFieldData();	
	pDM->MakeQueryByDM(RemoveAliasList, pDM_Index, pDM_Index->TBL_NAME, 0, pDM, _T("REC_KEY"), _T("NUMERIC"), strSpeciesKey);	
	
	CString strQuery;
	CString strRecKey;
	ids = pDM->MakeRecKey(strRecKey);
	if (ids < 0) return ids;

	ids = pDM_Index->GetCellData( _T("IBS_저작자"), 0, strItemData);
	strUpdateBookQuery.Format( _T("AUTHOR = '%s'"), strItemData);
	
	ids = pDM_Index->GetCellData( _T("IBS_발행년"), 0, strItemData);
	strTempBookQuery = strUpdateBookQuery; 
	strUpdateBookQuery.Format(_T("%s, PUBLISH_YEAR = '%s'"), strTempBookQuery, strItemData);
	
	m_mapMarcAlias.Lookup(_T("면장수"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	strTempBookQuery = strUpdateBookQuery;
	strUpdateBookQuery.Format(_T("%s, PAGE = '%s'"), strTempBookQuery, strItemData);
	
	m_mapMarcAlias.Lookup(_T("물리적특성"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	strTempBookQuery = strUpdateBookQuery;
	strUpdateBookQuery.Format(_T("%s, PHYSICAL_PROPERTY = '%s'"), strTempBookQuery, strItemData);
	
	m_mapMarcAlias.Lookup(_T("크기"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	strTempBookQuery = strUpdateBookQuery;
	strUpdateBookQuery.Format(_T("%s, BOOK_SIZE = '%s'"), strTempBookQuery, strItemData);
	
	m_mapMarcAlias.Lookup(_T("딸림자료"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	strTempBookQuery = strUpdateBookQuery;
	strUpdateBookQuery.Format(_T("%s, ACCOMP_MAT = '%s'"), strTempBookQuery, strItemData);
	
	m_mapMarcAlias.Lookup(_T("가격잡정보"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	strTempBookQuery = strUpdateBookQuery;
	strUpdateBookQuery.Format(_T("%s, PRICE_OTHER_INFO = '%s'"), strTempBookQuery, strItemData);
	
	m_mapMarcAlias.Lookup(_T("가격성격"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	strTempBookQuery = strUpdateBookQuery;
	strUpdateBookQuery.Format(_T("%s, PRICE_CHARACTER = '%s'"), strTempBookQuery, strItemData);
	
	m_mapMarcAlias.Lookup(_T("가격"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
				
	strItemData.Empty();
	m_mapMarcAlias.Lookup(_T("편권차"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	strTempBookQuery = strUpdateBookQuery;
	strUpdateBookQuery.Format(_T("%s, VOL = '%s'"), strTempBookQuery, strItemData);				

	strItemData.Empty();
	m_mapMarcAlias.Lookup(_T("편제"), strMarcAlias);
	m_pInfo->pMarcMgr->GetItem(&pMarc, strMarcAlias, strItemData);
	strTempBookQuery = strUpdateBookQuery;
	strUpdateBookQuery.Format(_T("%s, VOL_TITLE = '%s'"), strTempBookQuery, strItemData);

	if( !strItemData.IsEmpty() )
	{
		if(strItemData.GetAt(0) < '0' || strItemData.GetAt(0) > '9')
		{
			strItemData = strItemData.Mid(1);
		}

		if( CheckIsDigit(strItemData))
		{
		}

	}
	strQuery.Format(_T("UPDATE BO_BOOK_TBL SET %s WHERE SPECIES_KEY = %s AND VOL_SORT_NO = '%s';"),
		strUpdateBookQuery, strSpeciesKey, strVolSortNo);
	
	ids = pDM->AddFrame(strQuery);
	if (ids < 0) return ids;	

	strQuery.Format(_T("UPDATE BO_SPECIES_TBL SET SUBJECT_CODE='%s'  WHERE REC_KEY=%s;")
		,strSubjectCode,strSpeciesKey);
	ids = pDM->AddFrame(strQuery);
	if (ids < 0) return ids;	
	
	
	return 0;
}


INT CMARC_CHANGER_01::InitMemberData()
{	
	EFS_BEGIN
		
	m_mapMarcAlias.RemoveAll();
	m_mapCatDMtoMarc.RemoveAll();
	m_mapIndexDMtoIndexToolKit.RemoveAll();
	
	m_mapMarcAlias.SetAt(_T("편권차"), _T("245$n"));
	m_mapMarcAlias.SetAt(_T("편제"), _T("245$p"));
	m_mapMarcAlias.SetAt(_T("서명"), _T("245$a"));
	m_mapMarcAlias.SetAt(_T("부서명"), _T("245$b"));
	m_mapMarcAlias.SetAt(_T("대등서명"), _T("245$x"));
	m_mapMarcAlias.SetAt(_T("저자"), _T("245$d"));
	m_mapMarcAlias.SetAt(_T("공저자"), _T("245$e"));
	m_mapMarcAlias.SetAt(_T("발행자"), _T("260$b"));
	m_mapMarcAlias.SetAt(_T("총서표제"), _T("440$a"));
	m_mapMarcAlias.SetAt(_T("총서편차"), _T("490$a"));
	m_mapMarcAlias.SetAt(_T("가격"), _T("950$b"));
	m_mapMarcAlias.SetAt(_T("가격잡정보"), _T("950$c"));
	m_mapMarcAlias.SetAt(_T("면장수"), _T("300$a"));
	m_mapMarcAlias.SetAt(_T("물리적특성"), _T("300$b"));
	m_mapMarcAlias.SetAt(_T("크기"), _T("300$c"));
	m_mapMarcAlias.SetAt(_T("딸림자료"), _T("300$e"));
	m_mapMarcAlias.SetAt(_T("가격성격"), _T("950$a"));
	
	m_mapCatDMtoMarc.SetAt(_T("IBS_판사항"), _T("250$a"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_발행지"), _T("260$a"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_발행자"), _T("260$b"));
	m_mapCatDMtoMarc.SetAt(_T("IBS_발행년"), _T("260$c"));
	
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용저작자"), _T("저작자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_범위검색용발행년도"), _T("범위검색용발행년도"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용범위검색발행자"), _T("범위검색용발행자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용정렬용본표제"), _T("범위검색용본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용키워드"), _T("키워드파일"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용발행자"), _T("발행자"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_검색용본표제"), _T("본표제"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_표제사항"), _T("표제사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_저작자사항"), _T("저작자사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_발행자사항"), _T("발행자사항"));
	m_mapIndexDMtoIndexToolKit.SetAt(_T("IBS_발행년도사항"), _T("발행년도사항"));
	
	
	return 0;
	
	EFS_END
		return -1;
}

INT CMARC_CHANGER_01::MakeIndexArray(CStringArray *pArrStr, 
									 LPCTSTR strSrcIndexField, 
									 LPCTSTR lpszDelimit /*= _T(" ")*/)
{
	EFS_BEGIN
		
		CIndex *pIndex = m_pIndexTool->GetIndex();
	CString strIndexItem;	
	
	LPTSTR lpszToken = _tcstok(const_cast<LPTSTR>(strSrcIndexField), lpszDelimit);
	while (lpszToken)
	{
		strIndexItem.Format(_T("%s"), lpszToken);
		pIndex->MakeIndex(strIndexItem);
		if (!IsStringInArray(pArrStr, strIndexItem))
		{
			pArrStr->Add(strIndexItem);
		}
		
		lpszToken = _tcstok(NULL, lpszDelimit);
	}
	
	return 0;
	
	EFS_END
		return -1;
}

INT CMARC_CHANGER_01::DeleteRoleWord(CString &strSrc)
{
	EFS_BEGIN
		
		if (pCharSetMgr_RoleWord == NULL)
		{
			pCharSetMgr_RoleWord = new CESL_CharSetMgr;
			pCharSetMgr_RoleWord->InitData(_T("역할어"), _T("역할어"));
		}
		
		INT nFind = strSrc.ReverseFind(' ');
		if (nFind > 0)
		{
			CString str1 = strSrc.Mid(nFind);
			CString str2 = str1;
			str1.TrimLeft();
			str1.TrimRight();
			
			if (pCharSetMgr_RoleWord->Find(str1, str2) == 0)
			{
				strSrc.Delete(nFind, strSrc.GetLength() - nFind);
			}
		}
		
		return 0;
		
		EFS_END
			return -1;
}

BOOL CMARC_CHANGER_01::IsStringInArray(CStringArray *pArr, 
									   CString str)
{
	EFS_BEGIN
		
		if (pArr == NULL || str.IsEmpty()) return FALSE;
		
		INT nRowCount = pArr->GetSize();
		if (nRowCount == 0) return FALSE;
		
		for (INT i = 0; i < nRowCount; i++)
		{
			if (pArr->GetAt(i) == str) return TRUE;
		}
		
		return FALSE;
		
		EFS_END
			return FALSE;
}

HBRUSH CMARC_CHANGER_01::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void CMARC_CHANGER_01::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CMSHFlexGrid *pGrid = (CMSHFlexGrid*)GetDlgItem(IDC_MARC_CHANGE_GRID);
	if(pGrid->GetSafeHwnd() == NULL) return;
	pGrid->MoveWindow(6, 85, cx - 10, cy- 90 );			
}

void CMARC_CHANGER_01::OnbtnMarcView() 
{
	INT nSelectCnt;
	INT nSelectIdx;
	CString strRemark;
	nSelectCnt = GetGridSelectCnt();
	
	nSelectIdx = m_pMarcChangerGrid->SelectGetHeadPosition();
	for( INT i = 0 ; i < nSelectCnt ; i++)
	{
		strRemark = m_pMarcChangerGrid->GetTextMatrix( nSelectIdx+1, 4 );
		if( strRemark.Find( _T("ISBN") ) >= 0 )
		{
			m_pMarcChangerGrid->SetTextMatrix(nSelectIdx+1, 1 , _T(""));
		}
		nSelectIdx = m_pMarcChangerGrid->SelectGetNext();		
	}
	nSelectCnt = GetGridSelectCnt();
	
	if( 0 >= nSelectCnt )
	{
		AfxMessageBox(_T("선정된 데이터가 없습니다."));
		return;
	}
	m_pMarcChangerGrid->SetProtectSelectColumn(TRUE);
	CMARC_CHANGER_MARC_VIEW dlg(this);
	
	dlg.SetParentDM(m_pMarcChangeCompareDM);
	dlg.SetParentGrid(m_pMarcChangerGrid);
	
	dlg.DoModal();
	
	GridDisplay();
	m_pMarcChangerGrid->SetProtectSelectColumn(FALSE);
}

INT CMARC_CHANGER_01::GetGridSelectCnt() 
{
	INT nSelectCnt;
	nSelectCnt = 0;
	
	m_pMarcChangerGrid->SelectMakeList();
	nSelectCnt = m_pMarcChangerGrid->SelectGetCount();
	
	return nSelectCnt;
}	

void CMARC_CHANGER_01::Onchk049Hold() 
{
	if ( m_chk049Hold == TRUE )
	{
		m_chk049Hold = FALSE;
	}
	else
	{
		m_chk049Hold = TRUE;
	}
	
}

void CMARC_CHANGER_01::Onchk090Hold() 
{
	
	if ( m_chk090Hold == TRUE )
	{
		m_chk090Hold = FALSE;
	}
	else
	{
		m_chk090Hold = TRUE;
	}
	
}

VOID CMARC_CHANGER_01::SetMaxRegNo( CESL_DataMgr* pDM, CString strRegCode, int RegNo )
{
	if ( pDM == NULL || strRegCode.IsEmpty() || RegNo <= 0 ) return;

	int cnt = pDM->GetRowCount();

	if ( cnt < 0 ) return;

	CString strRegNo = _T("");
	strRegNo.Format(_T("%d"), RegNo );
	
	if ( cnt == 0 )
	{
		pDM->InsertRow(-1);
		pDM->SetCellData( _T("BB_등록구분_코드"), strRegCode, 0);
		pDM->SetCellData( _T("가장큰일련번호"), strRegNo, 0);

		return;
	}

	CString strTmp = _T(""), strTmpNo = _T("");
	BOOL bIsExist = FALSE;
	
	for ( int i = 0 ; i < cnt ; i++)
	{
		pDM->GetCellData( _T("BB_등록구분_코드"), i ,strTmp );
		if ( strTmp.CompareNoCase(strRegCode) == 0 ) bIsExist = TRUE;
	}

	if ( !bIsExist )
	{
		pDM->InsertRow(-1);
		pDM->SetCellData( _T("BB_등록구분_코드"), strRegCode, pDM->GetRowCount() - 1);
		pDM->SetCellData( _T("가장큰일련번호"), strRegNo, pDM->GetRowCount() - 1 );
		
		return;
	}

	for ( int j = 0 ; j <cnt ; j++ )
	{
		pDM->GetCellData( _T("BB_등록구분_코드"), j , strTmp );
		if ( strTmp.CompareNoCase(strRegCode) == 0 ){

			pDM->GetCellData( _T("가장큰일련번호"), j , strTmpNo );
			if ( _ttoi(strRegNo) > _ttoi(strTmpNo) )
			{
				pDM->SetCellData( _T("가장큰일련번호"), strRegNo, j );
			}
		}
	}

}

VOID CMARC_CHANGER_01::UpdateRegNo(){
	
	if ( m_pMaxRegNoDM == NULL || m_pMaxRegNoDM->GetRowCount() <= 0 ) return;
	
	CString strRegNo,strRegCode,strQry;

	m_pMaxRegNoDM->StartFrame();

	for( int i = 0 ; i < m_pMaxRegNoDM->GetRowCount() ; i++){

		m_pMaxRegNoDM->GetCellData( _T("BB_등록구분_코드"), i, strRegCode );
		m_pMaxRegNoDM->GetCellData( _T("가장큰일련번호"), i, strRegNo );

		strQry.Format(
			_T("UPDATE CO_LAST_NUMBER_TBL SET LAST_NUMBER = '%s' , LAST_WORK = '%s' WHERE KIND_CODE='REG_NO' AND DIVISION_VALUE='%s' AND PUBLISH_FORM_CODE='CO' AND MANAGE_CODE = UDF_MANAGE_CODE ")
			_T("AND NOT EXISTS ( ")
			_T("SELECT * FROM CO_LAST_NUMBER_TBL WHERE KIND_CODE='REG_NO' AND PUBLISH_FORM_CODE= 'CO' AND DIVISION_VALUE = '%s' AND LAST_NUMBER > '%s'  AND MANAGE_CODE = UDF_MANAGE_CODE); ")
			, strRegNo, GetWorkLogMsg( _T("마크교체"), __WFILE__, __LINE__ ) , strRegCode, strRegCode, strRegNo);

		m_pMaxRegNoDM->AddFrame(strQry);
	}	
	m_pMaxRegNoDM->SendFrame();
	m_pMaxRegNoDM->EndFrame();

	m_pMaxRegNoDM->FreeData();
}