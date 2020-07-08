// MarcRelationToText.cpp : implementation file
//

#include "stdafx.h"
#include "MarcRelationToText.h"
#include "efs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarcRelationToText dialog


CMarcRelationToText::CMarcRelationToText(CWnd* pParent /*=NULL*/)
	: CDialog(CMarcRelationToText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarcRelationToText)
	m_bChkDeep = FALSE;
	m_bChkSimple = FALSE;
	m_strDeepFilePath = _T("");
	m_strSimpleFilePath = _T("");
	//}}AFX_DATA_INIT
}


VOID CMarcRelationToText::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarcRelationToText)
	DDX_Check(pDX, IDC_chkDEEP, m_bChkDeep);
	DDX_Check(pDX, IDC_chkSIMPLE, m_bChkSimple);
	DDX_Text(pDX, IDC_eDEEPFILEPATH, m_strDeepFilePath);
	DDX_Text(pDX, IDC_eSIMPLEFILEPATH, m_strSimpleFilePath);
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CMarcRelationToText, CDialog)
	//{{AFX_MSG_MAP(CMarcRelationToText)
	ON_BN_CLICKED(IDC_bSTART, OnbSTART)
	ON_BN_CLICKED(IDC_bREAD, OnbREAD)
	ON_BN_CLICKED(IDC_chkDEEP, OnchkDEEP)
	ON_BN_CLICKED(IDC_chkSIMPLE, OnchkSIMPLE)
	ON_BN_CLICKED(IDC_bDEEPFILE, OnbDEEPFILE)
	ON_BN_CLICKED(IDC_bSIMPLE, OnbSIMPLE)
	ON_BN_CLICKED(IDC_bDEST_DIR, OnbDESTDIR)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarcRelationToText message handlers

VOID CMarcRelationToText::OnbSTART() 
{
	EFS_BEGIN

	GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);
	UpdateData();
	if (m_bChkSimple)
		//MakeSimpleMarcRelation();
		//MakeSimpleMarcRelation_New();
		MakeSimpleMarcRelation_VersionUp();
	if (m_bChkDeep)
		MakeDeepMarcRelation();

	GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);

	EFS_END
}

BOOL CMarcRelationToText::OnInitDialog() 
{
	EFS_BEGIN

	CDialog::OnInitDialog();
	
	m_readCfgFile.ReadCfgFile(_T(""));

	ButtonStatus();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

	EFS_END
	return FALSE;

}

VOID CMarcRelationToText::MakeDeepMarcRelation()
{
	EFS_BEGIN

	// For Check time
	DWORD START, FINISH, ELA;

	// marc relation
	CMarcRelation MarcRelation;

	// data manager
	CESL_DataMgr dmRelation;
	CESL_DataMgr dmGroup;

	dmRelation.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);
	dmGroup.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);
	
	CString strGroupQuery = _T("SELECT MARC_GROUP_CODE FROM MARC_RELATION_TBL GROUP BY MARC_GROUP_CODE");
	CString strRelationQuery = _T("SELECT MARC_GROUP_CODE, OPERATOR, BASIC_PARAM, PARAM, DESCRIP FROM MARC_RELATION_TBL");
	
	// Select Start...
	START = GetTickCount();

	dmGroup.RestructDataManager(strGroupQuery);
	dmRelation.RestructDataManager(strRelationQuery);
	
	FINISH = GetTickCount();
	ELA = FINISH-START;
	TRACE(_T("MARC SELECT %d.%d sec\n\n"), ELA/1000, ELA%1000);
	// Select End...

	INT nGroupCount = dmGroup.GetRowCount();
	INT nRelationCount = dmRelation.GetRowCount();

	BOOL bTmp[100000];
	for (INT i = 0; i < 100000; i++) 
	{
		bTmp[i] = TRUE;
	}

	CString strBaseGroupName;
	CString strGroupName;
	CString strOperator;
	CString strBasicParam;
	CString strParam;
	CString strDesc;

	// Fetch Start...
	START = GetTickCount();	

	for (INT idx = 0; idx < nGroupCount; idx++)
	{
		dmGroup.GetCellData(idx, 0, strBaseGroupName);
		
		for (INT subIdx = 0; subIdx < nRelationCount; subIdx++)
		{
			if (bTmp[subIdx] == FALSE) continue;
			dmRelation.GetCellData(subIdx, 0, strGroupName);
			if (strBaseGroupName != strGroupName)
				continue;

			bTmp[subIdx] = FALSE;

			dmRelation.GetCellData(subIdx, 1, strOperator);
			dmRelation.GetCellData(subIdx, 2, strBasicParam);
			dmRelation.GetCellData(subIdx, 3, strParam);
			dmRelation.GetCellData(subIdx, 4, strDesc);

			MarcRelation.InstructionList_AddInstruction(strGroupName, strOperator, strBasicParam, strParam, strDesc);
			
		}
	}

	FINISH = GetTickCount();
	ELA = FINISH-START;
	TRACE(_T("MAKE INSTRUCTION LIST %d.%d sec\n\n"), ELA/1000, ELA%1000);
	// Fetch End...

	MarcRelation.InstructionList_ToTextFile(m_strDeepFilePath);
	MarcRelation.FreeInstructionList();

	EFS_END
}

VOID CMarcRelationToText::MakeSimpleMarcRelation_VersionUp()
{
	EFS_BEGIN

	// Destination Dir
	CString strFilePath;

	// For Check time
	//DWORD START, FINISH, ELA;

	// marc relation
	CMarcRelation MarcRelation;

	CESL_DataMgr dmMarcGroup;
	CESL_DataMgr dm;

	dmMarcGroup.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);
	dm.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);

	CString strQuery;

	strQuery = _T("SELECT MARC_GROUP_CODE FROM MARC_GROUP_TBL");

	dmMarcGroup.RestructDataManager(strQuery);

	INT nGroupCnt = dmMarcGroup.GetRowCount();
	
	CString strMarcGroupCode;
	
	INT ids = -1;
	
	// 마크 그룹
	for (INT idx = 0; idx < nGroupCnt; idx ++)
	{
		strMarcGroupCode = dmMarcGroup.GetCellData(idx, 0);
		if (strMarcGroupCode .IsEmpty()) continue;

		// Save File Path
		strFilePath.Format(_T("%s\\SimpleRelation_%s.txt"), m_strSimpleFilePath, strMarcGroupCode);

		ids = MakeSimpleMarcRelation_Tag(&MarcRelation, strMarcGroupCode);

		MarcRelation.InstructionList_ToTextFile(strFilePath);
		MarcRelation.FreeInstructionList();
	}

	EFS_END
}

INT CMarcRelationToText::MakeSimpleMarcRelation_Tag(CMarcRelation *pMarcRelation, CString strMarcGroupCode)
{
	EFS_BEGIN

	if (pMarcRelation == NULL || strMarcGroupCode.IsEmpty()) return -1;

	INT ids = -1;
	CString strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);
	
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

INT CMarcRelationToText::MakeSimpleMarcRelation_Indicator(CMarcRelation *pMarcRelation, CString strMarcGroupCode, CString strTagCode, CString strTagAlias)
{
	EFS_BEGIN

	if (pMarcRelation == NULL || strMarcGroupCode.IsEmpty() || strTagAlias.IsEmpty()) return -1;
	
	CString strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);
	

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

INT CMarcRelationToText::MakeSimpleMarcRelation_Subfield(CMarcRelation *pMarcRelation, CString strMarcGroupCode, CString strTagCode, CString strTagAlias)
{
	EFS_BEGIN

	if (pMarcRelation == NULL || strMarcGroupCode.IsEmpty() || strTagAlias.IsEmpty()) return -1;

	CString strQuery;
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);

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

VOID CMarcRelationToText::MakeSimpleMarcRelation_New()
{
	EFS_BEGIN

	// Destination Dir
	CString strFilePath;

	// For Check time
	DWORD START, FINISH, ELA;
	
	// marc relation
	CMarcRelation MarcRelation;

	// data manager
	CESL_DataMgr dmRelation;
	CESL_DataMgr dmGroup;
	dmRelation.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);
	dmGroup.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);

	CString strGroupQuery = _T("SELECT MARC_GROUP_CODE FROM MARC_TAG_TBL GROUP BY MARC_GROUP_CODE");
	CString strIndRelationQuery = 
		_T("SELECT TAG.MARC_GROUP_CODE, TAG.TAG_CODE, TAG.REPEAT_TYPE, TAG.EXAMPLE, ")
		_T("IND1.IND_CODE, IND2.IND_CODE, ")
		_T("SUBFIELD.SUBFIELD_CODE, SUBFIELD.REPEAT_TYPE, SUBFIELD.PUNCTUATION_MARK_SET ")
		_T("FROM MARC_TAG_TBL TAG, MARC_FIRST_INDICATOR_TBL IND1, MARC_SECOND_INDICATOR_TBL IND2, MARC_SUBFIELD_TBL SUBFIELD ")
		_T("WHERE TAG.ALIAS = IND1.MARC_TAG_ALIAS AND TAG.ALIAS = IND2.MARC_TAG_ALIAS AND TAG.ALIAS = SUBFIELD.MARC_TAG_ALIAS (+)");

	// Select Start...
	START = GetTickCount();
	
	dmGroup.RestructDataManager(strGroupQuery);
	dmRelation.RestructDataManager(strIndRelationQuery);
	
	FINISH = GetTickCount();
	ELA = FINISH-START;
	TRACE(_T("MARC SELECT %d.%d sec\n\n"), ELA/1000, ELA%1000);
	// Select End...

	INT nGroupCount = dmGroup.GetRowCount();
	INT nRelationCount = dmRelation.GetRowCount();
	
	BOOL bTmp[100000];
	for (INT i = 0; i < 100000; i++) 
	{
		bTmp[i] = TRUE;
	}

	CString strBaseGroupName;
	CString strGroupName;
	CString strRepeatType;
	CString strFirstInd;
	CString strSecondInd;
	CString strTagCode;
	CString strExample;
	CString strSubfieldCode;
	CString strPuncMarkSet;
	CString strPuncMark;

	// Fetch Start...
	START = GetTickCount();

	for (INT idx = 0; idx < nGroupCount; idx++)
	{
		dmGroup.GetCellData(idx, 0, strBaseGroupName);

		strFilePath.Format(_T("%s\\SimpleRelation_%s.txt"), m_strSimpleFilePath, strBaseGroupName);
		
		for (INT subIdx = 0; subIdx < nRelationCount; subIdx++)
		{
			if (bTmp[subIdx] == FALSE) continue;
			dmRelation.GetCellData(subIdx, 0, strGroupName);
			if (strBaseGroupName != strGroupName)
				continue;
			bTmp[subIdx] = FALSE;
						
			//CString strTagCode;
			dmRelation.GetCellData(subIdx, 1, strTagCode);

			// OPERATION REPEAT_TAG
			dmRelation.GetCellData(subIdx, 2, strRepeatType);
			if (strRepeatType == _T("Y"))
			{
				MarcRelation.InstructionList_AddInstruction(strGroupName, _T("REPEAT_TAG"), strTagCode, strTagCode, _T(""));
			}

			// OPERATION TAG
			dmRelation.GetCellData(subIdx, 3, strExample);
			MarcRelation.InstructionList_AddInstruction(strGroupName, _T("TAG"), strTagCode, strTagCode, strExample);

			// OPERATION INDICATOR
			dmRelation.GetCellData(subIdx, 4, strFirstInd);
			dmRelation.GetCellData(subIdx, 5, strSecondInd);
			
			MarcRelation.InstructionList_AddInstruction(strGroupName, _T("INDICATOR"), strTagCode + strFirstInd + strSecondInd, strTagCode, _T(""));

			// OPERATION SUBFILED_CODE
			dmRelation.GetCellData(subIdx, 6, strSubfieldCode);
			
			MarcRelation.InstructionList_AddInstruction(strGroupName, _T("SUBFILED_CODE"), strTagCode + strSubfieldCode, strTagCode, _T(""));

			// OPERATION REPEAT_SUBFIELD_CODE
			dmRelation.GetCellData(subIdx, 7, strRepeatType);
			if (strRepeatType == _T("Y"))
			{
				MarcRelation.InstructionList_AddInstruction(strGroupName, _T("REPEAT_SUBFIELD_CODE"), strTagCode + strSubfieldCode, strTagCode, _T(""));
			}			

			// OPERATION PUNCTUATION_MARK
			dmRelation.GetCellData(subIdx, 8, strPuncMarkSet);
			INT nLength = strPuncMarkSet.GetLength();
			if (nLength > 0)
			{
				MarcRelation.InstructionList_AddInstruction(strGroupName, _T("PUNCTUATION_MARK"), strTagCode + strSubfieldCode, strTagCode, strPuncMarkSet);
				for (INT i = 0; i < nLength; i++)
				{
					strPuncMark = strPuncMarkSet.GetAt(i);
					if (strPuncMark == _T("|")) continue;
						
					MarcRelation.InstructionList_AddInstruction(strGroupName, _T("PUNCTUATION_MARK"), strPuncMark + strTagCode + strSubfieldCode, strTagCode, _T(""));

				}
			}
		}
		// Write group instruction list to file
		MarcRelation.InstructionList_ToTextFile(strFilePath);
		MarcRelation.FreeInstructionList();
	}
	FINISH = GetTickCount();
	ELA = FINISH-START;
	TRACE(_T("MAKE INSTRUCTION LIST %d.%d sec\n\n"), ELA/1000, ELA%1000);

	EFS_END
}

VOID CMarcRelationToText::MakeSimpleMarcRelation()
{
	EFS_BEGIN

	// For Check time
	DWORD START, FINISH, ELA;
	
	// marc relation
	CMarcRelation MarcRelation;

	// data manager
	CESL_DataMgr dmRelation;
	CESL_DataMgr dmGroup;
	dmRelation.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);
	dmGroup.SetCONNECTION_INFO(m_readCfgFile.m_sConnection);

	CString strGroupQuery = _T("SELECT MARC_GROUP_CODE FROM MARC_TAG_TBL GROUP BY MARC_GROUP_CODE");
	CString strIndRelationQuery = 
		_T("SELECT TAG.MARC_GROUP_CODE, TAG.TAG_CODE, TAG.REPEAT_TYPE, TAG.EXAMPLE, ")
		_T("IND1.IND_CODE, IND2.IND_CODE, ")
		_T("SUBFIELD.SUBFIELD_CODE, SUBFIELD.REPEAT_TYPE, SUBFIELD.PUNCTUATION_MARK_SET ")
		_T("FROM MARC_TAG_TBL TAG, MARC_FIRST_INDICATOR_TBL IND1, MARC_SECOND_INDICATOR_TBL IND2, MARC_SUBFIELD_TBL SUBFIELD ")
		_T("WHERE TAG.ALIAS = IND1.MARC_TAG_ALIAS AND TAG.ALIAS = IND2.MARC_TAG_ALIAS AND TAG.ALIAS = SUBFIELD.MARC_TAG_ALIAS (+)");

	// Select Start...
	START = GetTickCount();
	
	dmGroup.RestructDataManager(strGroupQuery);
	dmRelation.RestructDataManager(strIndRelationQuery);
	
	FINISH = GetTickCount();
	ELA = FINISH-START;
	TRACE(_T("MARC SELECT %d.%d sec\n\n"), ELA/1000, ELA%1000);
	// Select End...

	INT nGroupCount = dmGroup.GetRowCount();
	INT nRelationCount = dmRelation.GetRowCount();
	
	BOOL bTmp[100000];
	for (INT i = 0; i < 100000; i++) 
	{
		bTmp[i] = TRUE;
	}

	CString strBaseGroupName;
	CString strGroupName;
	CString strRepeatType;
	CString strFirstInd;
	CString strSecondInd;
	CString strTagCode;
	CString strExample;
	CString strSubfieldCode;
	CString strPuncMarkSet;
	CString strPuncMark;

	// Fetch Start...
	START = GetTickCount();

	for (INT idx = 0; idx < nGroupCount; idx++)
	{
		dmGroup.GetCellData(idx, 0, strBaseGroupName);
		
		for (INT subIdx = 0; subIdx < nRelationCount; subIdx++)
		{
			if (bTmp[subIdx] == FALSE) continue;
			dmRelation.GetCellData(subIdx, 0, strGroupName);
			if (strBaseGroupName != strGroupName)
				continue;
			bTmp[subIdx] = FALSE;
						
			//CString strTagCode;
			dmRelation.GetCellData(subIdx, 1, strTagCode);

			// OPERATION REPEAT_TAG
			dmRelation.GetCellData(subIdx, 2, strRepeatType);
			if (strRepeatType == _T("Y"))
			{
				MarcRelation.InstructionList_AddInstruction(strGroupName, _T("REPEAT_TAG"), strTagCode, strTagCode, _T(""));
			}

			// OPERATION TAG
			dmRelation.GetCellData(subIdx, 3, strExample);
			MarcRelation.InstructionList_AddInstruction(strGroupName, _T("TAG"), strTagCode, strTagCode, strExample);

			// OPERATION INDICATOR
			dmRelation.GetCellData(subIdx, 4, strFirstInd);
			dmRelation.GetCellData(subIdx, 5, strSecondInd);
			
			MarcRelation.InstructionList_AddInstruction(strGroupName, _T("INDICATOR"), strTagCode + strFirstInd + strSecondInd, strTagCode, _T(""));

			// OPERATION SUBFILED_CODE
			dmRelation.GetCellData(subIdx, 6, strSubfieldCode);
			
			MarcRelation.InstructionList_AddInstruction(strGroupName, _T("SUBFILED_CODE"), strTagCode + strSubfieldCode, strTagCode, _T(""));

			// OPERATION REPEAT_SUBFIELD_CODE
			dmRelation.GetCellData(subIdx, 7, strRepeatType);
			if (strRepeatType == _T("Y"))
			{
				MarcRelation.InstructionList_AddInstruction(strGroupName, _T("REPEAT_SUBFIELD_CODE"), strTagCode + strSubfieldCode, strTagCode, _T(""));
			}			

			// OPERATION PUNCTUATION_MARK
			dmRelation.GetCellData(subIdx, 8, strPuncMarkSet);
			INT nLength = strPuncMarkSet.GetLength();
			if (nLength > 0)
			{
				MarcRelation.InstructionList_AddInstruction(strGroupName, _T("PUNCTUATION_MARK"), strTagCode + strSubfieldCode, strTagCode, strPuncMarkSet);
				for (INT i = 0; i < nLength; i++)
				{
					strPuncMark = strPuncMarkSet.GetAt(i);
					if (strPuncMark == _T("|")) continue;
						
					MarcRelation.InstructionList_AddInstruction(strGroupName, _T("PUNCTUATION_MARK"), strPuncMark + strTagCode + strSubfieldCode, strTagCode, _T(""));

				}
			}
		}
	}
	FINISH = GetTickCount();
	ELA = FINISH-START;
	TRACE(_T("MAKE INSTRUCTION LIST %d.%d sec\n\n"), ELA/1000, ELA%1000);
	// Fetch End...

	MarcRelation.InstructionList_ToTextFile(m_strSimpleFilePath);
	MarcRelation.FreeInstructionList();

	EFS_END
}


VOID CMarcRelationToText::OnbREAD() 
{
	EFS_BEGIN

	CMarcRelation MarcRelation;

	CString strFilename = _T("MARC2TEXT.TXT");

	MarcRelation.InstructionList_FromTextFile(strFilename);
	AfxMessageBox(_T("ComPLETE"));
	MarcRelation.FreeInstructionList();

	EFS_END
}


VOID CMarcRelationToText::ButtonStatus()
{
	EFS_BEGIN

	UpdateData();
	if (m_bChkSimple && m_bChkDeep)
	{
		GetDlgItem(IDC_bSIMPLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_bDEEPFILE)->EnableWindow(TRUE);

		if (!m_strSimpleFilePath.IsEmpty() && !m_strDeepFilePath.IsEmpty())
			GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);
	}
	else if (m_bChkSimple)
	{
		GetDlgItem(IDC_bSIMPLE)->EnableWindow(TRUE);

		if (!m_strSimpleFilePath.IsEmpty())
			GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);
		else 
			GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);
	}
	else if (m_bChkDeep)
	{
		GetDlgItem(IDC_bDEEPFILE)->EnableWindow(TRUE);

		if (!m_strDeepFilePath.IsEmpty())
			GetDlgItem(IDC_bSTART)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_bSTART)->EnableWindow(FALSE);
		GetDlgItem(IDC_bDEEPFILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_bSIMPLE)->EnableWindow(FALSE);
	}

	EFS_END
}

VOID CMarcRelationToText::OnchkDEEP() 
{
	EFS_BEGIN

	ButtonStatus();

	EFS_END
}

VOID CMarcRelationToText::OnchkSIMPLE() 
{
	EFS_BEGIN

	ButtonStatus();

	EFS_END
}

VOID CMarcRelationToText::OnbDEEPFILE() 
{
	EFS_BEGIN

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = _T("TXT 파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TXT 파일 저장...");
	OFN.lpstrFile = lpstrFile;
	OFN.lpstrDefExt = _T("txt");
	OFN.hwndOwner = m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&OFN) != 0)
	{
		CString strFilePath;
		CString strFileExt;
		m_strDeepFilePath = OFN.lpstrFile;
	}	

	UpdateData(FALSE);
	ButtonStatus();

	EFS_END
}

VOID CMarcRelationToText::OnbSIMPLE() 
{
	EFS_BEGIN

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFilter = _T("TXT 파일(*.txt)\0*.txt\0모든파일(*.*)\0*.*\0");
	OFN.lpstrTitle = _T("TXT 파일 저장...");
	OFN.lpstrFile = lpstrFile;
	OFN.lpstrDefExt = _T("txt");
	OFN.nMaxFile = MAX_PATH;
	OFN.hwndOwner = m_hWnd;
	OFN.Flags = OFN_OVERWRITEPROMPT;
	if (GetSaveFileName(&OFN) != 0)
	{
		CString strFilePath;
		CString strFileExt;
		m_strSimpleFilePath = OFN.lpstrFile;
	}

	UpdateData(FALSE);
	ButtonStatus();

	EFS_END
}

VOID CMarcRelationToText::OnbDESTDIR() 
{
	EFS_BEGIN

	CString strDir;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	
	bi.hwndOwner = GetSafeHwnd();
	
	LPITEMIDLIST pidlSelected = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pidlSelected, strDir.GetBuffer(MAX_PATH));
	strDir.ReleaseBuffer();

	m_strSimpleFilePath = strDir;
	UpdateData(FALSE);

	ButtonStatus();

	EFS_END
}
