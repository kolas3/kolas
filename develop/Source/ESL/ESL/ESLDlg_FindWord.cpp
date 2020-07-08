// ESLDlg_FindWord.cpp : implementation file
//

#include "stdafx.h"
#include "ESLDlg_FindWord.h"
#include "ESL_Mgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_FindWord dialog


CESLDlg_FindWord::CESLDlg_FindWord(CWnd* pParent /*=NULL*/)
	: CDialog(CESLDlg_FindWord::IDD, pParent)
{
	m_pParentGrid = NULL;

	m_nPrevFindIdx = -1;
}

CESLDlg_FindWord::~CESLDlg_FindWord()
{
}

void CESLDlg_FindWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESLDlg_FindWord)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESLDlg_FindWord, CDialog)
	//{{AFX_MSG_MAP(CESLDlg_FindWord)
	ON_WM_SHOWWINDOW()
	ON_EN_CHANGE(IDC_COM_eFINDWORD, OnChangeFindWord)
	ON_BN_CLICKED(IDC_COM_bCANCEL, OnbtnCANCEL)
	ON_BN_CLICKED(IDC_COM_bFIND, OnbtnFIND)
	ON_BN_CLICKED(IDC_COM_bALL, OnbtnALL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESLDlg_FindWord message handlers

BOOL CESLDlg_FindWord::Create(CWnd* pParentWnd) 
{	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CESLDlg_FindWord::OnInitDialog() 
{
	CDialog::OnInitDialog();

	/// 초기값 아래로 선택
	GetDlgItem(IDC_COM_rDOWN)->SendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED);
		
	return TRUE;
}

BOOL CESLDlg_FindWord::PreTranslateMessage(MSG* pMsg) 
{	
	if( WM_KEYDOWN == pMsg->message )
	{
		///@{{	hotkey 
		if( 0x80 & (BYTE)GetKeyState(VK_CONTROL) )
		{
			/// 찾을 내용 포커스 이동	Ctrl + N
			if( 0x4E == pMsg->wParam )
			{
				GetDlgItem(IDC_COM_eFINDWORD)->SetFocus();
				return TRUE;
			}
			/// 아래로 선택				Ctrl + D
			else if( 0x44 == pMsg->wParam )
			{		
				GetDlgItem(IDC_COM_rUPPER)->SendMessage(BM_SETCHECK, (WPARAM)BST_UNCHECKED);
				GetDlgItem(IDC_COM_rDOWN)->SendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED);
				return TRUE;
			}
			/// 위로 선택				Ctrl + U
			else if( 0x55 == pMsg->wParam )
			{
				GetDlgItem(IDC_COM_rUPPER)->SendMessage(BM_SETCHECK, (WPARAM)BST_CHECKED);
				GetDlgItem(IDC_COM_rDOWN)->SendMessage(BM_SETCHECK, (WPARAM)BST_UNCHECKED);				
				return TRUE;
			}
			/// 다음 찾기				Ctrl + F
			else if( 0x46 == pMsg->wParam )
			{
				if( GetDlgItem(IDC_COM_bFIND)->IsWindowEnabled() )
				{
					OnbtnFIND();				
					return TRUE;
				}
			}
			/// 모두 선택				Ctrl + A
			else if( 0x41 == pMsg->wParam )
			{
				if( GetDlgItem(IDC_COM_bALL)->IsWindowEnabled() )
				{
					OnbtnALL();				
					return TRUE;
				}
			}
		}
		///@}}	
		
		if( VK_RETURN == pMsg->wParam )
		{
			if( GetDlgItem(IDC_COM_bALL)->IsWindowEnabled() )
			{
				OnbtnALL();
				OnbtnCANCEL();
			}

			return TRUE;
			
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}

void CESLDlg_FindWord::OnChangeFindWord() 
{
	///@{{ 찾을 내용 입력 유무 확인
	CString strData;
	GetDlgItemText(IDC_COM_eFINDWORD, strData);

	if( strData.GetLength() )
	{
		GetDlgItem(IDC_COM_bFIND)->EnableWindow(TRUE);
		GetDlgItem(IDC_COM_bALL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_COM_bFIND)->EnableWindow(FALSE);
		GetDlgItem(IDC_COM_bALL)->EnableWindow(FALSE);
	}
	///@}}
}

void CESLDlg_FindWord::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	/// 컬럼 구성이 변경되어 있을 수 있다.
	if( bShow )
	{
		InitColumnInfo();

		/// 검색을 다시하기 위해 
		m_nPrevFindIdx = -1;

		/// 입력창에 포커스를 위치 시킨다
		GetDlgItem(IDC_COM_eFINDWORD)->SetFocus();		
	}
}

///@{{ 콤보박스 관련
INT CESLDlg_FindWord::InitColumnInfo()
{
EFS_BEGIN
	
	INT		ids;

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COM_cTARGET);
	if( NULL == pBox )	return -1011;	

	/// 선정 기능이 있는 그리드라면 
	/// 선정 컬럼을 제외해야 한다.
	INT	nSelectColIdx = -1;
	if( pParentGrid->m_bCheck && 'S' == pParentGrid->m_cSelectType )
	{
		nSelectColIdx = pParentGrid->m_nSelectIdx;	
		GetDlgItem(IDC_COM_bALL)->ShowWindow(SW_SHOW);
	}
	/// 선정 그리드가 아니라면
	/// 모두 선택 버튼을 감춘다.
	else
	{
		GetDlgItem(IDC_COM_bALL)->ShowWindow(SW_HIDE);
	}

	/// 기존에 있다면 다시 생성한다.
	if( pBox->GetCount() )
	{
		pBox->ResetContent();
	}

	/// 기본값 입력 
	ids = pBox->AddString( _T("적용안함") );
	if( CB_ERR == ids || CB_ERRSPACE == ids )	return -1021;

	/// 그리드 컬럼명을 콤보박스에 적용한다.
	INT	nColCount = pParentGrid->GetColCount();

	CString strColumn;
	INT		nWidth;
	INT		nAlignment;

	for( INT i=0;i<nColCount;i++ )
	{
		if( i == nSelectColIdx )
			continue;

		ids = pParentGrid->GetColumnInfo( i, strColumn, nWidth, nAlignment );
		if( 0 > ids ) continue;

		ids = pBox->AddString( strColumn );
		if( CB_ERR == ids || CB_ERRSPACE == ids )	return -1021;
	}
	
	pBox->SetCurSel(0);
	pBox->UpdateWindow();

	return 0;
EFS_END
return -1;
}

INT	CESLDlg_FindWord::GetIndexColumn(INT nIndex, CString &strColumn)
{
EFS_BEGIN

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COM_cTARGET);
	if( NULL == pBox )	return -1001;
	
	pBox->GetLBText(nIndex, strColumn);
	return 0;

EFS_END
return -1;
}
INT CESLDlg_FindWord::GetSelectColumn(CString &strColumn)
{
EFS_BEGIN

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COM_cTARGET);
	if( NULL == pBox )	return -1001;
		
	return GetIndexColumn(pBox->GetCurSel(), strColumn);

EFS_END
return -1;
}

INT CESLDlg_FindWord::GetColumnCount()
{
EFS_BEGIN

	CComboBox* pBox = (CComboBox*)GetDlgItem(IDC_COM_cTARGET);
	if( NULL == pBox )	return -1001;
		
	return pBox->GetCount();

EFS_END
return -1;
}
///@}}

void CESLDlg_FindWord::OnbtnCANCEL() 
{
EFS_BEGIN

	CDialog::OnOK();	

EFS_END
}

void CESLDlg_FindWord::OnbtnFIND() 
{
EFS_BEGIN
	
	INT		ids;
	
	ids = FindWordProc();
	if( 0 > ids )	
	{
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못했습니다"), MB_ICONSTOP);
	}
	else if( 100 == ids )
	{
		AfxMessageBox(_T("찾을 결과가 없습니다"), MB_ICONINFORMATION);
	}
	else if( 200 == ids )
	{
		AfxMessageBox(_T("찾을 내용을 입력해야 합니다"), MB_ICONINFORMATION);
	}

	/// 입력창에 포커스를 위치 시킨다
	GetDlgItem(IDC_COM_eFINDWORD)->SetFocus();

EFS_END	
}

void CESLDlg_FindWord::OnbtnALL() 
{
EFS_BEGIN

	INT		ids;

	ids = AllSelectProc();	
	if( 0 > ids )	
	{
		AfxMessageBox(_T("기능을 정상적으로 수행하지 못했습니다"), MB_ICONSTOP);
	}
	else if( 100 == ids )
	{
		AfxMessageBox(_T("찾을 결과가 없습니다"), MB_ICONINFORMATION);
	}
	else if( 200 == ids )
	{
		AfxMessageBox(_T("찾을 내용을 입력해야 합니다"), MB_ICONINFORMATION);
	}

	/// 입력창에 포커스를 위치 시킨다
	GetDlgItem(IDC_COM_eFINDWORD)->SetFocus();

EFS_END	
}

///@{{ 문자열 검색
/// 연결 DM의 ALIAS와 FIELD를 가져온다.
INT	CESLDlg_FindWord::GetLinkAlias(CString strColumn, CString &strDM_ALIAS, CString &strFIELD_ALIAS)
{
EFS_BEGIN

	INT	nColIdx = GetColIdx(strColumn);
	if( 0 > nColIdx )	return -3001;

	if( 0 > GetLinkAlias(nColIdx, strDM_ALIAS, strFIELD_ALIAS) )	return -3001;

	return 0;
EFS_END	
return -1;
}

/// idx에 해당하는 DM의 ALIAS와 FIELD를 가져온다.
INT CESLDlg_FindWord::GetLinkAlias(INT nColIdx, CString &strDM_ALIAS, CString &strFIELD_ALIAS)
{
EFS_BEGIN

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;
	
	POSITION pos;
	pos = pParentGrid->m_arrColumnInfo.FindIndex(nColIdx);
	if( NULL == pos )	return -2001;
		
	CESL_GridColumnInfo* pColumnInfo = (CESL_GridColumnInfo*)pParentGrid->m_arrColumnInfo.GetAt(pos);
	if( NULL == pColumnInfo ) return -2011;

	strDM_ALIAS    = pColumnInfo->strColumnDMAlias;
	strFIELD_ALIAS = pColumnInfo->strColumnDMFieldAlias;

	return 0;
EFS_END	
return -1;
}

/// 컬럼명에 해당하는 idx를 가져온다.
INT CESLDlg_FindWord::GetColIdx(CString strColumn)
{
EFS_BEGIN

	INT		ids;

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	/// 선정 기능이 있는 그리드라면 
	/// 선정 컬럼을 제외해야 한다.
	INT	nSelectColIdx = -1;
	if( pParentGrid->m_bCheck && 'S' == pParentGrid->m_cSelectType )
		nSelectColIdx = pParentGrid->m_nSelectIdx;

	INT	nColCount = pParentGrid->GetColCount();

	CString strColumnInfo;
	INT		nWidth;
	INT		nAlignment;

	for( INT i=0;i<nColCount;i++ )
	{
		if( i == nSelectColIdx )
			continue;

		ids = pParentGrid->GetColumnInfo( i, strColumnInfo, nWidth, nAlignment );
		if( 0 > ids ) continue;

		if( strColumnInfo == strColumn )
			return i;
	}

	return -1;
EFS_END	
return -1;
}

/// 검색
/// 속도 향상을 위해 함수화를 하지 않았다.
INT	CESLDlg_FindWord::FindWordProc()
{
EFS_BEGIN
	
	INT		ids;

	/// 그리드 결과가 없다면 
	/// 검색하지 않는다
	INT	nGridRowCount;
	nGridRowCount = GetGridRowCount();
	if( 0 > nGridRowCount )  return -1001;
	if( 0 == nGridRowCount ) return 100;

	/// 검색어를 가져온다.
	CString strFindWord;
	ids = GetFindWord(strFindWord);
	if( 0 > ids )  return -1001;

	strFindWord.TrimLeft();strFindWord.TrimRight();
	if( 0 == strFindWord.GetLength() )	return 200;

	/// 검색 방향 설정
	INT		nFindDirection = GetFindDirection();
	if( 0 > nFindDirection )	return -1021;

	/// 검색 처음 시작 위치를 정한다.
	INT		nCurFindIdx = GetCurGridRowIdx();
	if( 0 > nCurFindIdx )	return -1011;

	/// 처음 검색이라면
	/// 또는 이전 검색에서 찾지 못했다면
	/// 현재 선택된 로우가 검색 위치이다.

	/// 이전검색 결과가 있다면 
	/// 선택된 로우의 다음 대상이 검색 위치이다.
	if( -1 != m_nPrevFindIdx )
	{		
		/// 선택된 위치와 이전 위치가 똑같다면 이어서 검색한다.
		/// 다르다면 선택된 위치부터 검색한다.
		if( m_nPrevFindIdx == nCurFindIdx )
			nCurFindIdx = GetNextFindIdx(nFindDirection, m_nPrevFindIdx);		

		if( !IsValidGridIdx(nCurFindIdx) )
		{
			nCurFindIdx = m_nPrevFindIdx;
		}
	}

	/// 검색대상 컬럼명을 가져온다.
	CString strColumn;
	ids = GetSelectColumn(strColumn);
	if( 0 > ids )	return -1022;
	
	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	CESL_GridData* pData = (CESL_GridData*)pParentGrid->m_pData;
	if( NULL == pData )	return -1000;

	CESL_Mgr* pMgr = pData->pMgr;
	if( NULL == pMgr )	return -1000;

	CESL_DataMgr* pDM = NULL;

	CString strDM_ALIAS;
	CString strFIELD_ALIAS;

	/// 적용안함이라면 전체 컬럼을 대상으로 해야한다.
	if( _T("적용안함") == strColumn )
	{
		INT	nColumnCount = GetColumnCount();
		if( 0 > nColumnCount )	return -1023;

		/// 적용안함을 제외한다.
		CStringArray arDM_ALIAS;
		CStringArray arFIELD_ALIAS;

		for( INT i=1;i<nColumnCount;i++ )
		{
			ids = GetIndexColumn( i, strColumn );
			if( 0 > ids )	return -2001;		

			ids = GetLinkAlias( strColumn, strDM_ALIAS, strFIELD_ALIAS );
			if( 0 > ids )	return -2001;
			
			if( 0 == arDM_ALIAS.GetSize() )
			{
				arDM_ALIAS.Add(strDM_ALIAS);
			}
			else
			{
				if( strDM_ALIAS != arDM_ALIAS.GetAt(0) )
				{
					AfxMessageBox(_T("'적용안함'을 지원하지 않는 그리드입니다.\r\n")
								  _T("'검색정보'를 선택해 주십시오"), MB_ICONINFORMATION);
					return 300;
				}
			}
			arFIELD_ALIAS.Add(strFIELD_ALIAS);
		}
		
		pDM = pMgr->FindDM(strDM_ALIAS);
		if( NULL == pDM )	return -1000;

		do
		{
			CString strData;

			for( INT j=0;j<arFIELD_ALIAS.GetSize();j++ )
			{
				ids = pDM->GetCellData(arFIELD_ALIAS.GetAt(j), nCurFindIdx, strData);
				if( 0 > ids )	return -3001;

				strData.TrimLeft();strData.TrimRight();

				/// 찾았다면
				/// 찾은 그리드의 로우를 반전한다
				/// m_nPrevFindIdx를 업뎃한다.
				if( -1 != strData.Find(strFindWord) )
				{
					/// 그리드 포지션 이동					
					pParentGrid->SendMessage( WM_VSCROLL, SB_TOP );
					for( INT k=0;k<nCurFindIdx;k++ )
						pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );

					m_nPrevFindIdx = nCurFindIdx;
					pParentGrid->SetReverse(m_nPrevFindIdx);

					return 0;
				}
			}

			/// 찾지 못했다면 다음을 검색한다.
			nCurFindIdx = GetNextFindIdx(nFindDirection, nCurFindIdx);				
		}
		while( 0 <= nCurFindIdx && nGridRowCount > nCurFindIdx );		
	}
	else
	{
		ids = GetLinkAlias( strColumn, strDM_ALIAS, strFIELD_ALIAS );
		if( 0 > ids )	return -2001;		

		pDM = pMgr->FindDM(strDM_ALIAS);
		if( NULL == pDM )	return -1000;

		do
		{
			CString strData;
			ids = pDM->GetCellData(strFIELD_ALIAS, nCurFindIdx, strData);
			if( 0 > ids )	return -3001;

			strData.TrimLeft();strData.TrimRight();

			/// 찾았다면
			/// 찾은 그리드의 로우를 반전한다
			/// m_nPrevFindIdx를 업뎃한다.
			if( -1 != strData.Find(strFindWord) )
			{				
				/// 그리드 포지션 이동					
				pParentGrid->SendMessage( WM_VSCROLL, SB_TOP );
				for( INT k=0;k<nCurFindIdx;k++ )
					pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );

				m_nPrevFindIdx = nCurFindIdx;
				pParentGrid->SetReverse(m_nPrevFindIdx);
				
				return 0;
			}

			/// 찾지 못했다면 다음을 검색한다.
			nCurFindIdx = GetNextFindIdx(nFindDirection, nCurFindIdx);			
		}
		while( 0 <= nCurFindIdx && nGridRowCount > nCurFindIdx );		
	}

	/// 만약 검색 idx가 끝까지 도달했다면		
	CString strMessage;
	strMessage.Format(_T("\"%s\"을(를) 찾을 수 없습니다."), strFindWord);
	AfxMessageBox(strMessage, MB_ICONINFORMATION);

	return 0;
EFS_END	
return -1;
}

/// 모두선택
/// 모두선택은 방향에 상관없이
/// 처음부터 끝까지 비교한다.
INT	CESLDlg_FindWord::AllSelectProc()
{
EFS_BEGIN
	
	INT		ids;
	
	/// 그리드 결과가 없다면 
	/// 검색하지 않는다
	INT	nGridRowCount;
	nGridRowCount = GetGridRowCount();
	if( 0 > nGridRowCount )  return -1001;
	if( 0 == nGridRowCount ) return 100;

	/// 검색어를 가져온다.
	CString strFindWord;
	ids = GetFindWord(strFindWord);
	if( 0 > ids )  return -1001;

	strFindWord.TrimLeft();strFindWord.TrimRight();
	if( 0 == strFindWord.GetLength() )	return 200;

	/// 검색 처음 시작 위치를 정한다.
	INT		nCurFindIdx = 0;
	
	/// 검색대상 컬럼명을 가져온다.
	CString strColumn;
	ids = GetSelectColumn(strColumn);
	if( 0 > ids )	return -1022;
	
	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	CESL_GridData* pData = (CESL_GridData*)pParentGrid->m_pData;
	if( NULL == pData )	return -1000;

	CESL_Mgr* pMgr = pData->pMgr;
	if( NULL == pMgr )	return -1000;

	CESL_DataMgr* pDM = NULL;

	CString strDM_ALIAS;
	CString strFIELD_ALIAS;
	
	INT nFindCount = 0;

	/// 최초 검색 위치로 그리드 포인트를 이동시키기 위해
	INT	nFirstFindIdx = -1;
	pParentGrid->SendMessage( WM_VSCROLL, SB_TOP );
	
	/// 시작하기전에 그리드에 선택된 모든 체크를 풀어준다.
	pParentGrid->UnSelectAll();

	/// 적용안함이라면 전체 컬럼을 대상으로 해야한다.
	if( _T("적용안함") == strColumn )
	{
		INT	nColumnCount = GetColumnCount();
		if( 0 > nColumnCount )	return -1023;

		/// 적용안함을 제외한다.
		CStringArray arDM_ALIAS;
		CStringArray arFIELD_ALIAS;

		for( INT i=1;i<nColumnCount;i++ )
		{
			ids = GetIndexColumn( i, strColumn );
			if( 0 > ids )	return -2001;		

			ids = GetLinkAlias( strColumn, strDM_ALIAS, strFIELD_ALIAS );
			if( 0 > ids )	return -2001;
			
			if( 0 == arDM_ALIAS.GetSize() )
			{
				arDM_ALIAS.Add(strDM_ALIAS);
			}
			else
			{
				if( strDM_ALIAS != arDM_ALIAS.GetAt(0) )
				{
					AfxMessageBox(_T("'적용안함'을 지원하지 않는 그리드입니다.\r\n")
								  _T("'검색정보'를 선택해 주십시오"), MB_ICONINFORMATION);
					return 300;
				}
			}
			arFIELD_ALIAS.Add(strFIELD_ALIAS);
		}
		
		pDM = pMgr->FindDM(strDM_ALIAS);
		if( NULL == pDM )	return -1000;

		do
		{
			CString strData;

			for( INT j=0;j<arFIELD_ALIAS.GetSize();j++ )
			{
				ids = pDM->GetCellData(arFIELD_ALIAS.GetAt(j), nCurFindIdx, strData);
				if( 0 > ids )	return -3001;

				strData.TrimLeft();strData.TrimRight();

				/// 찾았다면
				/// 찾은 그리드를 체크한다.
				if( -1 != strData.Find(strFindWord) )
				{			
					nFindCount++;
					pParentGrid->SetAt(nCurFindIdx, pParentGrid->m_nSelectIdx, pParentGrid->m_sSelectString);

					if( -1 == nFirstFindIdx )
						nFirstFindIdx = nCurFindIdx;
					break;
				}
			}

			/// 찾지 못했다면 다음을 검색한다.
			nCurFindIdx ++;
			/// 그리드 포지션 이동
//			pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );
				
		}
		while( 0 <= nCurFindIdx && nGridRowCount > nCurFindIdx );		
	}
	else
	{
		ids = GetLinkAlias( strColumn, strDM_ALIAS, strFIELD_ALIAS );
		if( 0 > ids )	return -2001;		

		pDM = pMgr->FindDM(strDM_ALIAS);
		if( NULL == pDM )	return -1000;

		do
		{
			CString strData;
			ids = pDM->GetCellData(strFIELD_ALIAS, nCurFindIdx, strData);
			if( 0 > ids )	return -3001;

			strData.TrimLeft();strData.TrimRight();

			/// 찾았다면
			/// 찾은 그리드의 로우를 반전한다
			/// m_nPrevFindIdx를 업뎃한다.
			if( -1 != strData.Find(strFindWord) )
			{
				nFindCount++;
				pParentGrid->SetAt(nCurFindIdx, pParentGrid->m_nSelectIdx, pParentGrid->m_sSelectString);
				if( -1 == nFirstFindIdx )
					nFirstFindIdx = nCurFindIdx;
			}

			/// 찾지 못했다면 다음을 검색한다.
			nCurFindIdx ++;
			/// 그리드 포지션 이동
//			pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );
			
		}
		while( 0 <= nCurFindIdx && nGridRowCount > nCurFindIdx );		
	}

	/// 만약 검색 idx가 끝까지 도달했다면		
	CString strMessage;
	if( 0 == nFindCount )
	{
		strMessage.Format(_T("\"%s\"을(를) 찾을 수 없습니다."), strFindWord);
	}
	else
	{
		strMessage.Format(_T("'%d' 건을 선택하였습니다."), nFindCount);

		/// 그리드 이동
		for( INT k=0;k<nFirstFindIdx;k++ )
			pParentGrid->SendMessage( WM_VSCROLL, SB_LINEDOWN );

	}
//	AfxMessageBox(strMessage, MB_ICONINFORMATION);

	return 0;
EFS_END	
return -1;
}

/// 사용자 입력값을 가져온다.
INT	CESLDlg_FindWord::GetFindWord(CString &strFindWord)
{
EFS_BEGIN

	return GetDlgItemText(IDC_COM_eFINDWORD, strFindWord);

EFS_END	
return -1;
}

/// 검색 방향
/// 0  아래로 
/// 1  위로 
INT	CESLDlg_FindWord::GetFindDirection()
{
EFS_BEGIN

	CButton* pBtn = (CButton*)GetDlgItem(IDC_COM_rUPPER);
	if( NULL == pBtn )	return -1001;

	return pBtn->GetCheck();

EFS_END	
return -1;
}

/// 다음 검색 위치
INT CESLDlg_FindWord::GetNextFindIdx(INT nFindDirection, INT nCurFindIdx)
{
EFS_BEGIN
	
	/// 아래로
	if( 0 == nFindDirection )	nCurFindIdx++;
	/// 위로
	else if( 1 == nFindDirection )	nCurFindIdx--;
	
	return nCurFindIdx;
EFS_END	
return -1;
}
INT CESLDlg_FindWord::GetNextFindIdx(INT nCurFindIdx)
{
EFS_BEGIN
	
	INT	nFindDirection;
	nFindDirection = GetFindDirection();
	if( 0 > nFindDirection )	return nFindDirection;

	return GetNextFindIdx(nFindDirection, nCurFindIdx);	

EFS_END	
return -1;
}
///@}}

///@{{ 그리드 제어
/// 현재 선택된 그리드의 로우를 가져온다.
INT	CESLDlg_FindWord::GetCurGridRowIdx()
{
EFS_BEGIN

	/// 그리드에 출력된 로우가 없다면 
	if( 0 == GetGridRowCount() )	return -100;

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1001;

	/// 선택된 로우가 없다면 
	if(0 == pParentGrid->GetMouseRow()) return 0;

	INT nRowIdx = pParentGrid->GetRow()-1;
	return nRowIdx < 0 ? 0 : nRowIdx;

EFS_END	
return -1;
}

INT CESLDlg_FindWord::GetGridRowCount()
{
EFS_BEGIN

	CESL_Grid* pParentGrid = (CESL_Grid*)m_pParentGrid;
	if( NULL == pParentGrid )	return -1000;

	return pParentGrid->m_nRealRowCount;

EFS_END	
return -1;
}

/// 입력 로우 idx가 유효한지 확인해준다.
BOOL CESLDlg_FindWord::IsValidGridIdx(INT nGridIdx)
{
EFS_BEGIN

	if( 0 > nGridIdx )
		return FALSE;

	if( GetGridRowCount() <= nGridIdx )
		return FALSE;

	return TRUE;
EFS_END	
return -1;
}
///@}}
