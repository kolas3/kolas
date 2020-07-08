#include "stdafx.h"
#include "BO_LOC_3195.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBO_LOC_3195::CBO_LOC_3195(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CBO_LOC_3195)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_LOC_3195::~CBO_LOC_3195()
{

}
VOID CBO_LOC_3195::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_LOC_3195)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBO_LOC_3195, CDialog)
	//{{AFX_MSG_MAP(CBO_LOC_3195)
	ON_BN_CLICKED(IDC_btn3100_CLOSE, OnbtnCLOSE)
	ON_BN_CLICKED(IDC_btn3100_SAVE, OnbtnSAVE)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_LOC_3195 message handlers

BOOL CBO_LOC_3195::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

BOOL CBO_LOC_3195::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	EnableThemeDialogTexture(GetSafeHwnd()); 
	
	SetIcon(LoadIcon(::AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME)),FALSE );	
	
	INT ids;
	ids = InitESL_Mgr(_T("BO_LOC_3195"));
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("OnInitDialog") );

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3195") );
	
	CESL_ControlMgr *pCM = FindCM( _T("CM_BO_LOC_3195") );	
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -5 , _T("CBO_LOC_3183::LoadData()") );

	ids = pDM->RefreshDataManager(_T(""));
	if(ids) ERROR_MSG_RETURN_ERROR_CODE2( -7 , _T("CBO_LOC_3195::RefreshDataManager()") );
	ids = pCM->AllControlDisplay();
	if(ids)  ERROR_MSG_RETURN_ERROR_CODE2( -9 , _T("CBO_LOC_3195::AllControlDisplay()") );
	
	LoadShelfLocCode(&m_strArray);
	SetShelfLocCode();

	return TRUE;
EFS_END
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CBO_LOC_3195::OnbtnCLOSE() 
{
	EFS_BEGIN

	CDialog::OnCancel();	

	EFS_END
}

INT CBO_LOC_3195::LoadShelfLocCode(CStringArray	*strArray )
{
EFS_BEGIN
	CString	strTemp;
	TCHAR	szLineBuffer[1024];
	TCHAR	*ptoken;	
	INT		nLen;
	
	strArray->RemoveAll();

	FILE *fp = _tfopen( LOAN_RETURN_SHELFLOCCODE_FILEPATH, _T("rb") );
	if ( fp == NULL ) return 0;
	
	if ( get_line(fp, szLineBuffer, &nLen ) == NULL )
	{
        ptoken = _tcstok( szLineBuffer, _T(" "));
        while( ptoken != NULL ) 
		{
			strTemp.Format(_T("%s"), ptoken);
			strArray->Add(strTemp);
			ptoken = _tcstok(NULL, _T(" "));			
		}
	}	

	fclose(fp);
	return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3195::SetShelfLocCode()
{
EFS_BEGIN			
	
	CString	strCode;
	INT		i, y, nCount, nRowCount;
	INT		ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3195") );	
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE2( -100 , _T("CBO_LOC_3183::SetShelfLocCode()") );
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3195") , _T("SHELF_GRID") );
	nRowCount = pDM->GetRowCount();
	nCount = m_strArray.GetSize();

	for ( y=0 ; y<nCount ; y++ )
	{		
		for ( i=0 ; i<nRowCount ; i++ )
		{				
			ids = pDM->GetCellData( _T("CODE") , i , strCode);
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( ids , _T("CBO_LOC_3195::SetShelfLocCode()") );
			if ( m_strArray.GetAt(y) == strCode )
			{
				pGrid->SetAt(i, 0, _T("V"));
			}
		}
	}
	
	return 0;
EFS_END
	return -1;
	
}

INT CBO_LOC_3195::get_line(
		                    FILE    *fp     ,
		                    TCHAR    *s_line ,
		                    INT     *s_cnt
				         )
{
 EFS_BEGIN
	SHORT   col, c;

    for ( col = 0; col < 9999 && (c=_fgettc(fp)) != -1 && c != '\n'; col++)  s_line[col] = (TCHAR)c;

    *s_cnt = (INT)col;
    if ( c == '\n' ) s_line[col++] = (TCHAR)c;
    s_line[col]= '\0';

    if (c == -1 && (*s_cnt) == 0) return -1;

    return( 0 );
EFS_END
}

INT CBO_LOC_3195::next_line (
							 FILE   *fp          ,
							 TCHAR   *ctmp        ,
							 INT    *line_length
							)
{
 EFS_BEGIN
	ctmp[0] = ' ';
    do {
        if ( get_line(fp, ctmp, (INT *)line_length) == -1 ) return -1;
        if ( ctmp[0] == '.' || *(INT *)line_length  == 0  ) continue;
        else break;
    } while (1);

    return 0;
EFS_END
	return -1;
}

INT CBO_LOC_3195::SaveShelfLocCode()
{
EFS_BEGIN
	
	CString	strShelfLocCode;
	CString	strCheck;
	INT		i, ids;

	CESL_DataMgr* pDM = FindDM( _T("DM_BO_LOC_3195") );	
	CESL_Grid* pGrid = (CESL_Grid*)FindControl( _T("CM_BO_LOC_3195") , _T("SHELF_GRID") );
	FILE *fp = _tfopen( LOAN_RETURN_SHELFLOCCODE_FILEPATH, _T("wb") );	
	
		fseek(fp, 0, SEEK_END);		
		if( 0 == ftell(fp) )
		{
			/*UNCHANGE*/fputc(0xFF, fp);
			/*UNCHANGE*/fputc(0xFE, fp);
		}
	strShelfLocCode.Empty();
	for ( i=0; i<pDM->GetRowCount() ; i++ )
	{
		strCheck = pGrid->GetAt(i,0);
		if ( strCheck == _T("V") )
		{		
			ids = pDM->GetCellData( _T("CODE") , i, strCheck );
			strShelfLocCode += strCheck + _T(" ");
		}
	}

	_ftprintf(fp, strShelfLocCode);
	fclose(fp);

	AfxMessageBox(_T("저장되었습니다."));
	

	return 0;
EFS_END
	return -1;
}

VOID CBO_LOC_3195::OnbtnSAVE() 
{	
EFS_BEGIN
	INT	ids;

	ids = SaveShelfLocCode();
	//if( ids ) ERROR_MSG_RETURN_ERROR_CODE(_T("CBO_LOC_3195::SaveShelfLocCode()") );
	
	return;
EFS_END
	return;
}

HBRUSH CBO_LOC_3195::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}
