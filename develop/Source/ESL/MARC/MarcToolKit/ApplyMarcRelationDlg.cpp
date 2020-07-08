// ApplyMarcRelationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ApplyMarcRelationDlg.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApplyMarcRelationDlg dialog


CApplyMarcRelationDlg::CApplyMarcRelationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CApplyMarcRelationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CApplyMarcRelationDlg)
	m_strDestDir = _T("..\\cfg\\MarcRelation");
	//}}AFX_DATA_INIT

	m_readCfgFile.ReadCfgFile(_T(""));
	m_strConnectionInfo = m_readCfgFile.m_sConnection;
}

BOOL CApplyMarcRelationDlg::Create(CWnd *pParentWnd)
{
	EFS_BEGIN

	return CDialog::Create(IDD, pParentWnd);

	EFS_END
	return FALSE;

}

VOID CApplyMarcRelationDlg::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplyMarcRelationDlg)
	DDX_Text(pDX, IDC_eDEST_DIR, m_strDestDir);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CApplyMarcRelationDlg, CDialog)
	//{{AFX_MSG_MAP(CApplyMarcRelationDlg)
	ON_BN_CLICKED(IDC_bDEST_DIR, OnbDESTDIR)
	ON_BN_CLICKED(IDC_bSTART_APPLY, OnbSTARTAPPLY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplyMarcRelationDlg message handlers

INT CApplyMarcRelationDlg::InitComboBox()
{
	EFS_BEGIN

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMARC_GROUP);
	if (pComboBox == NULL) return -1;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_strConnectionInfo);

	CString strQuery = _T("SELECT MARC_GROUP_CODE FROM MARC_GROUP_TBL ORDER BY MARC_GROUP_CODE");

	dm.RestructDataManager(strQuery);
	INT nRowCount = dm.GetRowCount();

	pComboBox->AddString(_T("======전체======"));

	CString strMarcGroup;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		 strMarcGroup = dm.GetCellData(idx, 0);
		 if (strMarcGroup.IsEmpty()) continue;

		 pComboBox->AddString(strMarcGroup);
	}

	pComboBox->SetCurSel(0);

	return 0;

	EFS_END
	return -1;

}

INT CApplyMarcRelationDlg::ApplyMarcRelation_Simple()
{
	EFS_BEGIN

	UpdateData();
	if (m_strDestDir.IsEmpty()) 
	{
		MessageBox(_T("경로를 설정해 주세요..."));
	}

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_strConnectionInfo);

	CString strQuery = _T("SELECT MARC_GROUP_CODE FROM MARC_GROUP_TBL ORDER BY MARC_GROUP_CODE");

	dm.RestructDataManager(strQuery);
	INT nRowCount = dm.GetRowCount();

	CString strMarcGroup;
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strMarcGroup = dm.GetCellData(idx, 0);
		if (strMarcGroup.IsEmpty()) continue;

		ApplyMarcRelation_Simple(strMarcGroup);
	}

	return 0;

	EFS_END
	return -1;

}

INT CApplyMarcRelationDlg::ApplyMarcRelation_Simple(CString strMarcGroup)
{
	EFS_BEGIN

	BeginWaitCursor();
	UpdateData();
	if (m_strDestDir.IsEmpty()) 
	{
		MessageBox(_T("경로를 설정해 주세요..."));
	}

	if (strMarcGroup.IsEmpty()) return -1;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_strConnectionInfo);

	CString strQuery;
	CString strResult[2];
	strQuery.Format(_T("SELECT PK, VERSION FROM MARC_GROUP_TBL ")
					_T("WHERE MARC_GROUP_CODE = '%s'"), strMarcGroup);
	if (dm.GetValuesQuery(strQuery, strResult, 2) < 0) return -1;

	INT ids = -1;
	CString strFilePath;
	strFilePath.Format(_T("%s\\SimpleRelation_%s.txt"), m_strDestDir, strMarcGroup);
	
	// 마크 그룹의 SimpleRelation을 파일로 다운 받는다.
	m_MarcRelation.FreeInstructionList();
	ids = MakeSimpleMarcRelation_Tag(&m_MarcRelation, strMarcGroup);
	m_MarcRelation.InstructionList_ToTextFile(strFilePath);
	m_MarcRelation.FreeInstructionList();

	// 파일을 BLOB으로 그룹 테이블에 올린다.
	CStdioFile file;
	if (!file.Open(strFilePath, CFile::modeRead | CFile::typeBinary))
	{
		MessageBox(_T("파일을 여는데 실패했습니다."));
		return -1;
	}
	TCHAR cUni;
	file.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		file.SeekToBegin();
	}
	
	INT nFileSize = file.GetLength();
	BYTE *pBuffer = new BYTE[nFileSize * sizeof(BYTE) + 1024];
	ZeroMemory(pBuffer, nFileSize);
	nFileSize = file.ReadHuge(pBuffer, nFileSize);
	file.Close();

	ids = dm.LOBUpdate(_T("MARC_GROUP_TBL"), _T("PK"), _T("NUMERIC"), strResult[0], _T("SIMPLE_FILE_DATA"), pBuffer, nFileSize);
	if (ids < 0)
	{
		MessageBox(_T("변경적용 실패"));
		goto ERR;
	}
	
	// 파일 사이즈, 버전을 업데이트 한다.
	dm.StartFrame();
	strQuery.Format(_T("UPDATE MARC_GROUP_TBL SET VERSION = %d, SIMPLE_FILE_SIZE = %d WHERE PK = %s;"), _ttoi(strResult[1]) + 1, nFileSize, strResult[0]);
	dm.AddFrame(strQuery);
	ids = dm.SendFrame();
	if (ids < 0) 
	{
		MessageBox(_T("변경적용 실패했습니다."));
		goto ERR;
	}
	ids = dm.EndFrame();
	if (ids < 0) 
	{
		MessageBox(_T("변경적용 실패했습니다."));
		goto ERR;
	}

	EndWaitCursor();
	if (pBuffer)
		delete [] pBuffer;
	return 0;
ERR:
	if (pBuffer)
		delete [] pBuffer;

	return -1;

	EFS_END
	return -1;

}

INT CApplyMarcRelationDlg::MakeSimpleMarcRelation_Tag(CMarcRelation *pMarcRelation, CString strMarcGroupCode)
{
	EFS_BEGIN

	if (pMarcRelation == NULL || strMarcGroupCode.IsEmpty()) return -1;

	INT ids = -1;
	CString strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_strConnectionInfo);
	
	strQuery.Format(_T("SELECT ALIAS, TAG_CODE, EXAMPLE, REPEAT_TYPE FROM MARC_TAG_TBL ")
					_T("WHERE MARC_GROUP_CODE = '%s'"), strMarcGroupCode);

	ids = dm.RestructDataManager(strQuery);
	if (ids < 0) return ids;

	CString strTagAlias;
	CString strTagCode;
	CString strExample;
	CString strRepeatType;

	INT nRowCount = dm.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strTagAlias		= dm.GetCellData(idx, 0);
		strTagCode		= dm.GetCellData(idx, 1);
		strExample		= dm.GetCellData(idx, 2);
		strRepeatType	= dm.GetCellData(idx, 3);

		// OPERATION TAG
		pMarcRelation->InstructionList_AddInstruction(strMarcGroupCode, _T("TAG"), strTagCode, strTagCode, strExample);

		// OPERATION REPEAT_TAG
		if (strRepeatType == _T("Y"))
			pMarcRelation->InstructionList_AddInstruction(strMarcGroupCode, _T("REPEAT_TAG"), strTagCode, strTagCode, _T(""));

		ids = MakeSimpleMarcRelation_Indicator(pMarcRelation, strMarcGroupCode, strTagCode, strTagAlias);
		ids = MakeSimpleMarcRelation_Subfield(pMarcRelation, strMarcGroupCode, strTagCode, strTagAlias);
	}

	return 0;

	EFS_END
	return -1;

}

INT CApplyMarcRelationDlg::MakeSimpleMarcRelation_Indicator(CMarcRelation *pMarcRelation, CString strMarcGroupCode, CString strTagCode, CString strTagAlias)
{
	EFS_BEGIN

	if (pMarcRelation == NULL || strMarcGroupCode.IsEmpty() || strTagAlias.IsEmpty()) return -1;
	
	CString strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_strConnectionInfo);
	

	// FIRST
	strQuery.Format(_T("SELECT FIRST.IND_CODE, FIRST.DEFAULT_YN, SECOND.IND_CODE, SECOND.DEFAULT_YN FROM MARC_FIRST_INDICATOR_TBL FIRST, MARC_SECOND_INDICATOR_TBL SECOND ")
					_T("WHERE FIRST.MARC_TAG_ALIAS = '%s' AND SECOND.MARC_TAG_ALIAS = '%s'"), strTagAlias, strTagAlias);
	
	INT ids = dm.RestructDataManager(strQuery);
	if (ids < 0) return ids;

	CString strFirstInd;
	CString strFirstDefaultYN;
	CString strSecondInd;	
	CString strSecondDefaultYN;

	INT nRowCount = dm.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strFirstInd			= dm.GetCellData(idx, 0);
		strFirstDefaultYN	= dm.GetCellData(idx, 1);
		strSecondInd		= dm.GetCellData(idx, 2);
		strSecondDefaultYN	= dm.GetCellData(idx, 3);

		// OPERATION INDICATOR
		pMarcRelation->InstructionList_AddInstruction(strMarcGroupCode, _T("INDICATOR"), strTagCode + strFirstInd + strSecondInd, strTagCode, _T(""));

		// OPERATION DEFAULT INDICATOR
		if (strFirstDefaultYN == _T("V") && strSecondDefaultYN == _T("V"))
		{
			pMarcRelation->InstructionList_AddInstruction(strMarcGroupCode, _T("DEFAULT_INDICATOR"), strTagCode, strFirstInd + strSecondInd, _T(""));
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CApplyMarcRelationDlg::MakeSimpleMarcRelation_Subfield(CMarcRelation *pMarcRelation, CString strMarcGroupCode, CString strTagCode, CString strTagAlias)
{
	EFS_BEGIN

	if (pMarcRelation == NULL || strMarcGroupCode.IsEmpty() || strTagAlias.IsEmpty()) return -1;

	CString strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_strConnectionInfo);

	strQuery.Format(_T("SELECT SUBFIELD_CODE, REPEAT_TYPE, PUNCTUATION_MARK_SET FROM MARC_SUBFIELD_TBL ")
					_T("WHERE MARC_TAG_ALIAS = '%s'"), strTagAlias);

	INT ids = dm.RestructDataManager(strQuery);
	if (ids < 0) return ids;

	CString strSubfieldCode;
	CString strRepeatType;
	CString strPuncMarkSet;
	CString strPuncMark;

	INT nLength = 0;

	INT nRowCount = dm.GetRowCount();
	for (INT idx = 0; idx < nRowCount; idx++)
	{
		strSubfieldCode		= dm.GetCellData(idx, 0);
		strRepeatType		= dm.GetCellData(idx, 1);
		strPuncMarkSet		= dm.GetCellData(idx, 2);

		// OPERATION SUBFILED_CODE
		pMarcRelation->InstructionList_AddInstruction(strMarcGroupCode, _T("SUBFILED_CODE"), strTagCode + strSubfieldCode, strTagCode, _T(""));

		// OPERATION REPEAT_SUBFIELD_CODE
		if (strRepeatType == _T("Y"))
			pMarcRelation->InstructionList_AddInstruction(strMarcGroupCode, _T("REPEAT_SUBFIELD_CODE"), strTagCode + strSubfieldCode, strTagCode, _T(""));
	
		// OPERATION PUNCTUATION_MARK
		nLength = strPuncMarkSet.GetLength();
		if (nLength > 0)
		{
			pMarcRelation->InstructionList_AddInstruction(strMarcGroupCode, _T("PUNCTUATION_MARK"), strTagCode + strSubfieldCode, strTagCode, strPuncMarkSet);
			for (INT i = 0; i < nLength; i++)
			{
				strPuncMark = strPuncMarkSet.GetAt(i);
				if (strPuncMark == _T("|")) continue;
					
				pMarcRelation->InstructionList_AddInstruction(strMarcGroupCode, _T("PUNCTUATION_MARK"), strPuncMark + strTagCode + strSubfieldCode, strTagCode, _T(""));

			}
		}
	}

	return 0;

	EFS_END
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CApplyMarcRelationDlg message handlers

BOOL CApplyMarcRelationDlg::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();

	if (m_strConnectionInfo.IsEmpty())
	{
		m_readCfgFile.ReadCfgFile(_T(""));
		m_strConnectionInfo = m_readCfgFile.m_sConnection;
	}

	InitComboBox();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CApplyMarcRelationDlg::OnbDESTDIR() 
{
	EFS_BEGIN

	CString strDir;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	
	bi.hwndOwner = GetSafeHwnd();
	
	LPITEMIDLIST pidlSelected = SHBrowseForFolder(&bi);
	BOOL bSuccess = SHGetPathFromIDList(pidlSelected, strDir.GetBuffer(MAX_PATH));
	strDir.ReleaseBuffer();

	if (!bSuccess) return;

	m_strDestDir = strDir;
	UpdateData(FALSE);

	EFS_END
}

VOID CApplyMarcRelationDlg::OnbSTARTAPPLY() 
{
	EFS_BEGIN

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_cMARC_GROUP);
	if (pComboBox == NULL) return;

	INT nCurSel = pComboBox->GetCurSel();

	INT ids = -1;
	if (nCurSel == 0)
	{
		ids = ApplyMarcRelation_Simple();
	}
	else
	{
		CString strMarcGroup;
		pComboBox->GetLBText(pComboBox->GetCurSel(), strMarcGroup);
		ids = ApplyMarcRelation_Simple(strMarcGroup);

	}

	if (ids >= 0) MessageBox(_T("적용 완료"));

	EFS_END
}
