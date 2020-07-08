// MarcExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MarcExportDlg.h"
#include <direct.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcExportDlg dialog


CMarcExportDlg::CMarcExportDlg(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcExportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pCM = NULL;

}

CMarcExportDlg::~CMarcExportDlg()
{
}

BOOL CMarcExportDlg::Create(CWnd* pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;
}


VOID CMarcExportDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcExportDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP


	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcExportDlg, CDialog)
	//{{AFX_MSG_MAP(CMarcExportDlg)
	ON_BN_CLICKED(IDC_bFILEOPEN, OnbFILEOPEN)
	ON_BN_CLICKED(IDC_bMAKE_FILE, OnbMAKEFILE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcExportDlg message handlers

BOOL CMarcExportDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_정리_전거마크반출")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		EndDialog(IDCANCEL);
		return FALSE;
	}

	m_pCM = FindCM(_T("CM_정리_전거마크반출"));
	if (m_pCM == NULL) 
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	CString strPath;
	GetFullPathName(_T("..\\BIN\\반출_AV.TXT"), MAX_PATH, strPath.GetBuffer(MAX_PATH), NULL);


	m_pCM->SetControlMgrData(_T("파일명"), strPath );

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;
}

VOID CMarcExportDlg::OnbFILEOPEN() 
{
EFS_BEGIN


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
	OFN.lpstrDefExt = _T("TXT");
	
	BOOL bSuccess = GetSaveFileName(&OFN);
	SetCurrentDirectory(szBuffer);
	if (bSuccess)
		m_pCM->SetControlMgrData(_T("파일명"), OFN.lpstrFile);


EFS_END
}

VOID CMarcExportDlg::OnbMAKEFILE() 
{
EFS_BEGIN

	CString strFileName;
	m_pCM->GetControlMgrData( _T("파일명"), strFileName );
	if ( strFileName.IsEmpty() ) 
	{
		AfxMessageBox( _T("파일명을 입력해야 합니다.") );
		return;
	}

	if ( IDNO == AfxMessageBox( _T("파일을 작성하시겠습니까?") , MB_YESNO) ) return;

	// 경로를 분해(-_-) 하자
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	
	_tsplitpath(strFileName, drive, dir, fname, ext);

	// 자료 검색
	CString strOption;
	INT ids = MakeOptionQuery(strOption);
	if (ids < 0) return;

	CESL_DataMgr *pDM = NULL;
	pDM = FindDM(_T("DM_정리_전거마크반출"));
	if ( pDM == NULL ) return ;


	if (pDM->RefreshDataManager(strOption) < 0) return;
	
	INT nRowCount = pDM->GetRowCount();
	if (nRowCount < 1 ) 
	{
		AfxMessageBox (_T("검색된 결과가 없습니다."));
		return;
	}

	// Make MarcInfo
	ids = MakeMarcInfo(pDM);
	if (ids < 0) return;

	
	//파일자동분할
	INT nFileCount = 1;
	INT nDataCountPerFile = nRowCount;

	CMarc marc;
	CString strMarcStream;
	CString strExpFileName;

	INT idxData = 0;
	INT nDataCount = 0;
	CStdioFile file;
	
	for (INT idxFile = 0; idxFile < nFileCount; idxFile++)
	{
		if (nFileCount > 1)
			strExpFileName.Format(_T("%s%s%s_%05d%s"), drive, dir, fname, idxFile, ext);
		else
			strExpFileName = strFileName;

		if (!file.Open(strExpFileName, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) 
		{
			AfxMessageBox(_T("파일 생성에 실패했습니다."));
			return;
		}
		
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}	

		if (nFileCount > 1)
			idxData = idxFile * nDataCountPerFile;

		nDataCount = idxData + nDataCountPerFile;
		for (; idxData < nDataCount; idxData++)
		{
			if (idxData > nRowCount - 1) break;
			ids = pDM->GetCellData(_T("MARC"), idxData, strMarcStream);
			if (ids < 0 || strMarcStream.IsEmpty()) continue;

			//홍수
			//strMarcStream += _T("\n");
			file.WriteString(strMarcStream);
		}		

		file.Close();
	}
	
	CString sMessage;
	sMessage.Format( _T("검색된 %d건에 대한 파일작성이 완료되었습니다."), nRowCount );
	AfxMessageBox( sMessage );

EFS_END
}


INT CMarcExportDlg::MakeOptionQuery(CString &strOption)
{
EFS_BEGIN

	strOption.Empty();

	CString sACControlStart = _T("");
	CString sACControlEnd = _T("");
	
	m_pCM->GetControlMgrData( _T("전거제어번호부터"), sACControlStart );
	m_pCM->GetControlMgrData( _T("전거제어번호까지"), sACControlEnd );
	
	sACControlStart.TrimLeft();
	sACControlStart.TrimRight();
	sACControlEnd.TrimLeft();
	sACControlEnd.TrimRight();
	
	if ( sACControlStart == _T("") || sACControlEnd == _T("") ) {
		AfxMessageBox( _T("전거제어번호를 입력해 주세요.") );
		return - 1;
	}
	if ( sACControlStart.Left(3) != _T("KAC") || sACControlEnd.Left(3) != _T("KAC") )
	{
		AfxMessageBox( _T("전거제어번호는 KAC로 시작합니다.") );
		return - 1;	
	}

	//자릿수 맞추기
	CString sTmpChar0;
	if ( sACControlStart.GetLength() < 12 )
	{
		sTmpChar0 = _T("");
		for ( INT i = sACControlStart.GetLength(); i < 12 ; i++ )
			sTmpChar0 += _T("0");
		
		sACControlStart = sACControlStart.Left(3) + sTmpChar0 + sACControlStart.Mid(3);		
	}
	
	if ( sACControlEnd.GetLength() < 12 )
	{
		sTmpChar0 = _T("");
		for ( INT i = sACControlEnd.GetLength(); i < 12 ; i++ )
			sTmpChar0 += _T("0");
		
		sACControlEnd = sACControlEnd.Left(3) + sTmpChar0 + sACControlEnd.Mid(3);		
	}

	strOption.Format(_T("AC_CONTROL_NO >= '%s' AND AC_CONTROL_NO <= '%s'"),sACControlStart, sACControlEnd );

	return 0;

EFS_END
	return -1;
}

INT CMarcExportDlg::MakeMarcInfo(CESL_DataMgr *pDM)
{
EFS_BEGIN

	/*
	if (pDM == NULL) return -1;
	INT ids = -1;

	// 자관 정보
	BOOL bMarcInfo = FALSE;
	if (static_cast<CButton*>(GetDlgItem(IDC_chkMarcInfo))->GetCheck() == 1 )  bMarcInfo = TRUE;
	
	//++ 040217_HSS 조건불포함 등록번호 삭제 ++++++++++++++++
	CButton * pDelRegNo;
	pDelRegNo = (CButton *)GetDlgItem(IDC_chCLASS2);

	// 조건 select
	CString sRegCode;
	m_pCM->GetControlMgrData( _T("등록구분"), sRegCode );
	sRegCode.Replace( _T(" "), NULL );

	CString sNoStart, sNoEnd;	
	m_pCM->GetControlMgrData( _T("등록번호부터"), sNoStart );
	m_pCM->GetControlMgrData( _T("등록번호까지"), sNoEnd );
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

	CMarc marc;
	POSITION pos = NULL;
	CTag *pTag = NULL;
	CString strHoldInfo;
	CString strStreamMarc;
	INT nRowCount = pDM->GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		ids = pDM->GetCellData(_T("BS_MARC"), idx, strStreamMarc);
		ids = m_pInfo->pMarcMgr->Decoding(strStreamMarc, &marc);
		if (ids < 0) continue;
		ids = pDM->GetCellData(_T("SH_소장정보"), idx, strHoldInfo);
		if (ids >= 0 && !strHoldInfo.IsEmpty())
		{
			CMarc marcHold;
			ids = m_pInfo->pMarcMgr->Decoding(strHoldInfo, &marcHold);
			if (ids >= 0)
			{
				pos = marcHold.m_listTag.GetHeadPosition();
				while (pos)
				{
					pTag = (CTag*)marcHold.m_listTag.GetNext(pos);
					if (!pTag) continue;
					marc.AddTag(pTag->Clone());
				}
			}
		}

		if (!bMarcInfo)
		{
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("049"));
			m_pInfo->pMarcMgr->DeleteField(&marc, _T("090"));
		}

		//++ 040217_HSS 조건불포함 등록번호 삭제 ++++++++++++++++
		if ( pDelRegNo->GetCheck() == 1 ) {
			CString strRegNo, strRegCode;	// 등록번호,등록구분
			INT nRegNum;					// 등록번호 숫자
			CArray<CString, CString&> p049lArray;
			m_pInfo->pMarcMgr->GetItem( &marc, _T("049$l"), strRegNo, &p049lArray );
			for ( INT j=0 ; j < p049lArray.GetSize() ; j++ ) {
				strRegNo = p049lArray.GetAt(j);
				strRegCode = strRegNo.Mid(0, strRegNo.GetLength()-10);
				nRegNum = _ttoi(strRegNo.Mid(strRegNo.GetLength()-10, 10));

				// 등록구분이 다르거나 번호가 밖인 경우
				if (sRegCode.Compare(strRegCode) 
					|| nRegNum < _ttoi(sNoStart)
					|| nRegNum > _ttoi(sNoEnd) )
				{
					m_pInfo->pMarcMgr->DeleteItem( &marc, _T("049$l"), strRegNo );
				}
			}
		}
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.09.09 이학중 수정 전
		//ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc);
		//ids = pDM->SetCellData(_T("MARC"), strStreamMarc, idx);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++

        //+++++++++++++++++++++++++++++++++++++++++++++++++++++
		//+++++++++++++++++++++++++++++++++++++++++++++++++++++
		//2003.09.09 이학중 
		//MARC 반출시 TAG 정렬 하기 
		ids = m_pInfo->pMarcMgr->Encoding(&marc, strStreamMarc,TRUE);
		ids = pDM->SetCellData(_T("MARC"), strStreamMarc, idx);
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	}
	*/
	return 0;

EFS_END
	return -1;
}

VOID CMarcExportDlg::OnCancel() 
{
EFS_BEGIN
	
	CDialog::OnCancel();

EFS_END
}

HBRUSH CMarcExportDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr; 
}
