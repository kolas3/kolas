// DlgRptSave.cpp : implementation file
//

#include "stdafx.h"
#include "sireporteditor.h"
#include "DlgRptSave.h"
#include "SIReportEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgRptSave dialog


CDlgRptSave::CDlgRptSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRptSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRptSave)
	m_strTitle = _T("");
	m_strDescription = _T("");
	//}}AFX_DATA_INIT

	m_nMode = MODE_SAVE_AS;
	
	m_strReportPK= _T("");
	m_strTitle = _T("");
	m_strDescription = _T("");
	m_strSaveFileName = _T("");
	m_strRptGroup = _T("");

	m_pDM = NULL;
	m_pDM = new CESL_DataMgr;

	m_strRptGroup = _T("LIGHT,단행,수서");
}

CDlgRptSave::~CDlgRptSave()
{
	FreeBitmapAliasList();
}

VOID CDlgRptSave::AddBitmapAlias(CString strBitmapAlias)
{
EFS_BEGIN

	CString *pStr = new CString;
	pStr->Format(_T("%s"), strBitmapAlias);

	m_listBitampAlias.AddTail((CObject*)pStr);

EFS_END
}

VOID CDlgRptSave::FreeBitmapAliasList()
{
EFS_BEGIN

	INT nCount = m_listBitampAlias.GetCount();
	if (nCount == 0) return;
	CString *pStr;
	for (INT i = 0; i < nCount; i++) 
	{
		pStr = (CString*)m_listBitampAlias.RemoveHead();
		if (pStr) delete pStr;
	}
	m_listBitampAlias.RemoveAll();
	return;

EFS_END
}


VOID CDlgRptSave::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRptSave)
	DDX_Control(pDX, IDC_cSTEP3, m_cSTEP3);
	DDX_Control(pDX, IDC_cSTEP2, m_cSTEP2);
	DDX_Control(pDX, IDC_cSTEP1, m_cSTEP1);
	DDX_Text(pDX, IDC_eTITLE, m_strTitle);
	DDX_Text(pDX, IDC_eDESCRIPTION, m_strDescription);
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CDlgRptSave, CDialog)
	//{{AFX_MSG_MAP(CDlgRptSave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_bSAVE, OnbSAVE)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_eTITLE, OnChangeeTITLE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRptSave message handlers

BOOL CDlgRptSave::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	m_pDM->SetCONNECTION_INFO(m_sConnection);

	InitBusinessStep();
	UpdateData(FALSE);
	
	if (m_nMode == MODE_SAVE || ((CSIReportEditorView*)GetParentFrame()->GetActiveView())->m_nEditorMode == USER_NORMAL)
		SetTimer(0, 200, NULL);
	else
		ButtonSataus();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

VOID CDlgRptSave::ButtonSataus()
{
EFS_BEGIN

	UpdateData();

	if (m_strTitle.IsEmpty()) 
		GetDlgItem(IDC_bSAVE)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_bSAVE)->EnableWindow(TRUE);

EFS_END
}

VOID CDlgRptSave::ErrorMessageBox(CString msg, INT ids)
{
EFS_BEGIN

	CString errormsg;
	errormsg.Format(_T("%s [%d]"), msg, ids);
	AfxMessageBox(errormsg);

EFS_END
}


VOID CDlgRptSave::OnDestroy() 
{
EFS_BEGIN

	CDialog::OnDestroy();
	
	if (m_pDM) {
		delete m_pDM;
		m_pDM = NULL;
	}

EFS_END
}

VOID CDlgRptSave::OnTimer(UINT nIDEvent) 
{
EFS_BEGIN

	if (nIDEvent == 0) {
		KillTimer(0);
		if(!m_strTitle.IsEmpty())
			OnbSAVE();
		else
		{
			AfxMessageBox(_T("원본 형식이 없으므로 저장할수 없습니다."));
			return;
		}
	}	
	CDialog::OnTimer(nIDEvent);

EFS_END
}

VOID CDlgRptSave::OnbSAVE() 
{
EFS_BEGIN

	UpdateData();
	
	CString strRPTGROUP, strSTEP1, strSTEP2, strSTEP3;
	m_cSTEP1.GetWindowText(strSTEP1);
	m_cSTEP2.GetWindowText(strSTEP2);
	m_cSTEP3.GetWindowText(strSTEP3);
	strSTEP1.TrimLeft();		strSTEP1.TrimRight();		strSTEP1.MakeUpper();
	strSTEP2.TrimLeft();		strSTEP2.TrimRight();		strSTEP2.MakeUpper();
	strSTEP3.TrimLeft();		strSTEP3.TrimRight();		strSTEP3.MakeUpper();
	strRPTGROUP.Format(_T("%s,%s,%s"), strSTEP1, strSTEP2, strSTEP3);

	INT ids;
	CString sQuery;
	CString strResult;

	// Switch mode User Normal, user develper
	if (((CSIReportEditorView*)GetParentFrame()->GetActiveView())->m_nEditorMode == USER_NORMAL)
	{
		sQuery.Format(_T("SELECT PK FROM ESL_SIREPORT_TBL WHERE RPTALIAS = '%s' AND FLAG = 'U'"), m_strTitle);
		m_pDM->GetOneValueQuery(sQuery, strResult);

		if (strResult.IsEmpty())
		{
			ids = m_pDM->MakeRecKey(m_strReportPK, m_sSequence);
			if (ids < 0) 
			{
				ErrorMessageBox(_T("Make Primary Key Fail!"), ids);
				return;
			}

			m_nMode = MODE_SAVE_AS;
		}
		else
		{
			m_strReportPK = strResult;
			m_nMode = MODE_SAVE;
		}
	}

	// Switch Mode Save, Saveas
	if (m_nMode == MODE_SAVE_AS && ((CSIReportEditorView*)GetParentFrame()->GetActiveView())->m_nEditorMode == USER_DEVELOPER)
	{		
		sQuery.Format(_T("SELECT COUNT(*) FROM ESL_SIREPORT_TBL WHERE RPTALIAS = '%s'"), m_strTitle);
		m_pDM->GetOneValueQuery(sQuery, strResult);

		if (strResult != _T("0"))
		{
			AfxMessageBox(_T("제목이 중복이 됩니다. 다시 입력해주세요"));
			GetDlgItem(IDC_eTITLE)->SetFocus();
			return;
		}

		ids = m_pDM->MakeRecKey(m_strReportPK, m_sSequence);
		if (ids < 0) 
		{
			ErrorMessageBox(_T("Make Primary Key Fail!"), ids);
			return;
		}
	}

	// 원본이 존재하는지 체크한다.
	if (m_nMode == MODE_SAVE)
	{
		sQuery.Format(_T("SELECT COUNT(*) FROM ESL_SIREPORT_TBL WHERE RPTALIAS = '%s'"), m_strTitle);
		m_pDM->GetOneValueQuery(sQuery, strResult);

		if (strResult == _T("0"))
		{
			m_nMode = MODE_SAVE_AS;
			ids = m_pDM->MakeRecKey(m_strReportPK, m_sSequence);
			if (ids < 0) 
			{
				ErrorMessageBox(_T("Make Primary Key Fail!"), ids);
				return;
			}
		}
	}

	INT rptsize = 0;
	CFile file;
	if (!file.Open(m_strSaveFileName, CFile::modeRead | CFile::typeBinary)) {
		AfxMessageBox(_T("Save File Open Fail!"));
		return;
	}
	
	rptsize = file.GetLength();
	BYTE * pBuffer = (BYTE *)malloc(rptsize + 1024);
	file.ReadHuge(pBuffer, rptsize);
	file.Close();

	// Make Query by save mode
	CString sCurrentTime;
	CTime t = CTime::GetCurrentTime();
	sCurrentTime.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	
	if (m_nMode == MODE_SAVE)
	{
		sQuery.Format(_T("UPDATE ESL_SIREPORT_TBL SET RPTALIAS='%s', RPTGROUP='%s', ENGINEER='%s', DESCRIP='%s', RPTSIZE=%d, LAST_DATE='%s' WHERE PK=%s;"), m_strTitle, strRPTGROUP, m_sUserID, m_strDescription, rptsize, sCurrentTime, m_strReportPK);
	}
	else if (m_nMode == MODE_SAVE_AS)
	{
		CString strFlag;
		((CSIReportEditorView*)GetParentFrame()->GetActiveView())->m_nEditorMode == USER_NORMAL ? strFlag = _T("U") : strFlag = _T("X");

		sQuery.Format(_T("INSERT INTO ESL_SIREPORT_TBL (PK, FLAG, RPTALIAS, RPTGROUP, ENGINEER, DESCRIP, RPTSIZE, FIRST_DATE, LAST_DATE) "
			_T("VALUES (%s, '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s');")), m_strReportPK, strFlag, m_strTitle, strRPTGROUP, m_sUserID, m_strDescription, rptsize, sCurrentTime, sCurrentTime);
	}
	
	// XML LOB File Update
	ids = m_pDM->ExecuteQuery(sQuery, 1);
	if (ids < 0) {
		ErrorMessageBox(_T("Execute Query Failed!"), ids);
		free (pBuffer);
		return;
	}
	ids = m_pDM->LOBUpdate(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), m_strReportPK, _T("RPTDATA"), pBuffer, rptsize);
	if (ids < 0) {
		ErrorMessageBox(_T("Insert Fail!"), ids);
		free (pBuffer);
		return;
	}
	free(pBuffer);

	// Bitmap LOB File Update
	
		// Remove Old Bitmap LOB data
	sQuery.Format(_T("DELETE FROM ESL_SIREPORT_TBL WHERE FLAG = 'B' AND RPTALIAS = '%s';"), m_strTitle);
	ids = m_pDM->ExecuteQuery(sQuery, 1);
	if (ids < 0) {
		ErrorMessageBox(_T("Execute Query Failed!"), ids);
		free (pBuffer);
		return;
	}

	CString strPK;
	CString *pStrBitmapAlias;

	POSITION pos = m_listBitampAlias.GetHeadPosition();
	while (pos != NULL)
	{
		pStrBitmapAlias = (CString*)m_listBitampAlias.GetNext(pos);
		if (pStrBitmapAlias == NULL) continue;
		
		
		ids = m_pDM->MakeRecKey(strPK, m_sSequence);
		if (ids < 0) 
		{
			ErrorMessageBox(_T("Make Primary Key Fail!"), ids);
			return;
		}

		if (!SaveBitmapFiles(strPK, pStrBitmapAlias))
		{
			EndDialog(IDOK);
			return;
		}

	}
	AfxMessageBox(_T("저장 완료!"));
	
	EndDialog(IDOK);

EFS_END
}

BOOL CDlgRptSave::SaveBitmapFiles(CString strPK, CString* pStrBitmapAlias)
{
EFS_BEGIN

	INT ids;
	
	// Open bitmap file and read file to pBuffer
	CString strBitmapFileName;
	strBitmapFileName.Format(_T("%s\\%s.bmp"), EDITOR_TEMP_DIR, pStrBitmapAlias->GetBuffer(0));
	
	CFile file;
	if (!file.Open(strBitmapFileName, CFile::modeRead | CFile::typeBinary)) {
		AfxMessageBox(_T("Bitmap File Open Fail!"));
		return FALSE;
	}
	
	INT nRptsize = file.GetLength();
	BYTE* pBuffer = (BYTE*)malloc(nRptsize+ 1024);
	file.ReadHuge(pBuffer, nRptsize);
	file.Close();

	//Insert Bitmap data and Update Bitmap LOB data
	CString sCurrentTime;
	CTime t = CTime::GetCurrentTime();
	sCurrentTime.Format(_T("%04d%02d%02d%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	CString strQuery;
	strQuery.Format(_T("INSERT INTO ESL_SIREPORT_TBL (PK, FLAG, RPTALIAS, BITMAP_ALIAS, ENGINEER, DESCRIP, RPTSIZE, FIRST_DATE, LAST_DATE) "
			_T("VALUES (%s, 'B', '%s', '%s', '%s', '%s', %d, '%s', '%s');")), strPK, m_strTitle, pStrBitmapAlias->GetBuffer(0), m_sUserID, m_strDescription, nRptsize, sCurrentTime, sCurrentTime);
	
	ids = m_pDM->ExecuteQuery(strQuery, 1);
	if (ids < 0) {
		ErrorMessageBox(_T("Execute Query Failed!"), ids);
		free (pBuffer);
		return FALSE;
	}

	ids = m_pDM->LOBUpdate(_T("ESL_SIREPORT_TBL"), _T("PK"), _T("NUMERIC"), strPK, _T("RPTDATA"), pBuffer, nRptsize);
	if (ids < 0) {
		ErrorMessageBox(_T("Insert Fail!"), ids);
		free (pBuffer);
		return FALSE;
	}
	free(pBuffer);

	return TRUE;

EFS_END
return FALSE;

}

VOID CDlgRptSave::OnbCANCEL() 
{
EFS_BEGIN

	CDialog::OnCancel();	

EFS_END
}

VOID CDlgRptSave::OnChangeeTITLE() 
{
EFS_BEGIN

	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	ButtonSataus();
	

EFS_END
}

BOOL CDlgRptSave::PreTranslateMessage(MSG* pMsg) 
{
EFS_BEGIN

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnbSAVE();
			return TRUE;
		}
	}	
	
	return CDialog::PreTranslateMessage(pMsg);

EFS_END
return FALSE;

}

INT CDlgRptSave::InitBusinessStep(VOID)
{
EFS_BEGIN

	INT nSTEP1=0, nSTEP2=0, nSTEP3=0;
	CString strSTEP1[32], strSTEP2[32], strSTEP3[32];

	INT i, ids;
	CString sQuery;
	// (START) - 테스트 코드 - 코드관리로 바꿔야 함
	ids = m_pDM->RestructDataManager(_T("SELECT CODE.CODE FROM CD_CLASS_CODE_TBL CLASS, CD_CODE_TBL CODE WHERE CLASS.CLASS_CODE=CODE.CLASS AND CLASS.CLASS_NAME='업무단계1'"));
	nSTEP1 = m_pDM->GetRowCount();
	for (i = 0; i < nSTEP1; i++) {
		strSTEP1[i] = m_pDM->GetCellData(i, 0);
	}
	ids = m_pDM->RestructDataManager(_T("SELECT CODE.CODE FROM CD_CLASS_CODE_TBL CLASS, CD_CODE_TBL CODE WHERE CLASS.CLASS_CODE=CODE.CLASS AND CLASS.CLASS_NAME='업무단계2'"));
	nSTEP2 = m_pDM->GetRowCount();
	for (i = 0; i < nSTEP2; i++) {
		strSTEP2[i] = m_pDM->GetCellData(i, 0);
	}
	ids = m_pDM->RestructDataManager(_T("SELECT CODE.CODE FROM CD_CLASS_CODE_TBL CLASS, CD_CODE_TBL CODE WHERE CLASS.CLASS_CODE=CODE.CLASS AND CLASS.CLASS_NAME='업무단계3'"));
	nSTEP3 = m_pDM->GetRowCount();
	for (i = 0; i < nSTEP3; i++) {
		strSTEP3[i] = m_pDM->GetCellData(i, 0);
	}
	// ( END ) - 테스트 코드 - 코드관리로 바꿔야 함

	CString strTmpSTEP1, strTmpSTEP2, strTmpSTEP3;
	INT pos1=0, pos2=0;
	if (m_strRptGroup.GetLength() > 0) {
		pos2 = m_strRptGroup.Find(_T(","), pos1);
		strTmpSTEP1 = m_strRptGroup.Mid(pos1, pos2-pos1);
		pos1 = pos2+1;

		pos2 = m_strRptGroup.Find(_T(","), pos1);
		strTmpSTEP2 = m_strRptGroup.Mid(pos1, pos2-pos1);
		pos1 = pos2+1;

		pos2 = m_strRptGroup.Find(_T(","), pos1);
		if (pos2 == -1) pos2 = m_strRptGroup.GetLength();
		strTmpSTEP3 = m_strRptGroup.Mid(pos1, pos2-pos1);
		pos1 = pos2+1;
	}

	InitBusinessStepComboBox(&m_cSTEP1, strSTEP1, nSTEP1, strTmpSTEP1);
	InitBusinessStepComboBox(&m_cSTEP2, strSTEP2, nSTEP2, strTmpSTEP2);
	InitBusinessStepComboBox(&m_cSTEP3, strSTEP3, nSTEP3, strTmpSTEP3);

	if (m_strRptGroup.GetLength() == 0) {
		m_cSTEP1.SetCurSel(0);
		m_cSTEP2.SetCurSel(0);
		m_cSTEP3.SetCurSel(0);
	}
	return 0;

EFS_END
return -1;

}

INT CDlgRptSave::InitBusinessStepComboBox(CComboBox *pCMB, CString str[], INT nstr, CString strSelect)
{
EFS_BEGIN

	if (!pCMB) return -1;

	pCMB->ResetContent();
	INT nSelectedRow = -1;
	for (INT i = 0; i < nstr; i++) {
		pCMB->AddString(str[i]);
		if (str[i] == strSelect) nSelectedRow = i;
	}
	if (nSelectedRow >= 0) {
		pCMB->SetCurSel(nSelectedRow);
	}	
	return 0;

EFS_END
return -1;

}
