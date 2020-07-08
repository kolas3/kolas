// ImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExcelToPMAlias.h"
#include "ImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DBNTWIN32
#include <sqlfront.h>
#include <sqldb.h>

/////////////////////////////////////////////////////////////////////////////
// CImportDlg dialog


CImportDlg::CImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportDlg)
	m_strDBID = _T("sa");
	m_strDBIP = _T("192.168.1.50");
	m_strDBNAME = _T("pm");
	m_strDBPWD = _T("");
	m_strJOBKEY = _T("");
	m_strPROJECTKEY = _T("");
	m_strREFTABLE = _T("ESL_REFERENCE_TBL");
	m_strTABLENAME = _T("");
	//}}AFX_DATA_INIT

	m_nRows = 0;
	m_nCols = 0;
	m_pTableGrid = NULL;
}


VOID CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportDlg)
	DDX_Text(pDX, IDC_eDBID, m_strDBID);
	DDX_Text(pDX, IDC_eDBIP, m_strDBIP);
	DDX_Text(pDX, IDC_eDBNAME, m_strDBNAME);
	DDX_Text(pDX, IDC_eDBPWD, m_strDBPWD);
	DDX_Text(pDX, IDC_eJOBKEY, m_strJOBKEY);
	DDX_Text(pDX, IDC_ePROJECTKEY, m_strPROJECTKEY);
	DDX_Text(pDX, IDC_eREFTABLE, m_strREFTABLE);
	DDX_Text(pDX, IDC_eTABLENAME, m_strTABLENAME);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportDlg, CDialog)
	//{{AFX_MSG_MAP(CImportDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportDlg message handlers

VOID CImportDlg::OnOK() 
{
	UpdateData(TRUE);
	if (m_pTableGrid == NULL) {
		CDialog::OnOK();
		return;
	}
	BOOL bDelete = FALSE;
	CString strRec_Key, strQuery, strDesc;
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_CHECK1);
	if (pButton1) {
		if (pButton1->GetCheck() == TRUE) bDelete = TRUE;
			else bDelete = FALSE;
	}
	if (bDelete == TRUE) {
		strQuery.Format(_T("DELETE FROM %s WHERE PROJECT_KEY=%s AND JOB_KEY=%s AND TBL_NAME='%s';"), m_strREFTABLE, m_strPROJECTKEY, m_strJOBKEY, m_strTABLENAME);
		MSSQL_ExecuteNonSelectQuery(strQuery);
	}
	INT nRows = m_pTableGrid->GetRows() - 1;
	CProgressCtrl *pBar = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	INT nColumnOrder = 10000000;
	for (INT i = 1; i <= nRows; i++) {
		MSSQL_ExecuteSelectQuery(_T("SELECT max(rec_key)+1 FROM ESL_REFERENCE_TBL;"));
		strRec_Key = m_strResult[0][0];

		nColumnOrder = 10000000 + ((i-1)* 1000000);
		strDesc = m_pTableGrid->GetTextMatrix(i, 3);
		strDesc.TrimLeft();		strDesc.TrimRight();
		strDesc.Replace(_T("'"), _T("''"));
		strQuery.Format(_T("INSERT INTO %s (REC_KEY, PROJECT_KEY, JOB_KEY, TBL_NAME, FIELD_NAME, FIELD_TYPE, FIELD_SIZE, DEF_REF_ALIAS, DESCRIP, LASTMODIFIED, COLUMN_ORDER) VALUES ")
						_T("(%s, %s, %s, '%s', '%s', '%s', %s, '%s', '%s', GETDATE(), %d);"),
						m_strREFTABLE,
						strRec_Key, m_strPROJECTKEY, m_strJOBKEY, m_strTABLENAME, 
						m_pTableGrid->GetTextMatrix(i, 0),
						m_pTableGrid->GetTextMatrix(i, 1),
						m_pTableGrid->GetTextMatrix(i, 2),
						m_pTableGrid->GetTextMatrix(i, 0),
						strDesc, 
						nColumnOrder
						);
		MSSQL_ExecuteNonSelectQuery(strQuery);
		if (pBar) pBar->SetPos((INT)((i+1)*100 / nRows));
	}
	if (pBar) pBar->SetPos(100);
	CString strMsg;
	strMsg.Format(_T("Finish! [%s]"), m_strTABLENAME);
	AfxMessageBox(strMsg);
	CDialog::OnOK();
}

VOID CImportDlg::OnCancel() 
{	
	CDialog::OnCancel();
}

BOOL CImportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton *pButton1 = (CButton*)GetDlgItem(IDC_CHECK1);
	if (pButton1) pButton1->SetCheck(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CImportDlg::MSSQL_ExecuteNonSelectQuery(CString strQuery)
{
	PDBPROCESS  dbproc;
	PLOGINREC   login;

	dbinit();
    login = dblogin ();
	DBSETLUSER (login, m_strDBID);
	DBSETLPWD  (login, m_strDBPWD);
    dbproc = dbopen (login, m_strDBIP);
	
	if(!dbproc){
		dbexit();
		return -1;
	}
	BOOL bDead = dbdead (dbproc);
	if (bDead) {
		dbexit();
		return -1;
	}

	dbuse(dbproc, m_strDBNAME);

	dbcmd (dbproc, strQuery);
	dbsqlexec (dbproc);
	INT nResult = dbresults (dbproc);
	dbexit();
	if (nResult == 0) return 0;
	return -1;
}

INT CImportDlg::MSSQL_ExecuteSelectQuery(CString strQuery)
{
	PDBPROCESS  dbproc;
	PLOGINREC   login;
	_TUCHAR result[4][256];

	dbinit();
    login = dblogin ();
	DBSETLUSER (login, m_strDBID);
	DBSETLPWD  (login, m_strDBPWD);
    dbproc = dbopen (login, m_strDBIP);
	
	if(!dbproc){
		dbexit();
		return 0;
	}
	BOOL bDead = dbdead (dbproc);
	if (bDead) {
		dbexit();
		return 0;
	}

	dbuse(dbproc, m_strDBNAME);

	dbcmd (dbproc, strQuery);
    dbsqlexec (dbproc);

	INT nRow = 0;
	INT nCol = 0;
	INT i, j;

	CString tmp0, tmp1, tmp2, tmp3;
	if(dbresults(dbproc) == SUCCEED){	
		nCol = dbnumcols(dbproc);
		for(i = 0 ; i < nCol ; i++){
			dbbind(dbproc, i+1, NTBSTRINGBIND, 0, result[i]);
		}		
        while (dbnextrow (dbproc) != NO_MORE_ROWS)
        {
			for (j = 0; j < nCol; j++) {
				if (result[j] == NULL) {
					m_strResult[nRow][j] = _T("");
					continue;
				}
				m_strResult[nRow][j].Format(_T("%s"), result[j]);
			}
        }
		nRow++;
	}
	dbexit();
	m_nRows = nRow;
	m_nCols = nCol;
	return 0;
}
