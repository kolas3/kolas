#include "Stdafx.h"
#include "XLAutomationForJournal.h"

CXLAutomationForJournal::CXLAutomationForJournal(BOOL bExcelUsed)
	: CXLAutomation(TRUE, bExcelUsed)
{
	m_pBackupWorkSheet = m_pdispWorksheet;

	m_pWorkSheets = NULL; 
}

CXLAutomationForJournal::~CXLAutomationForJournal()
{
	m_pdispWorksheet = m_pBackupWorkSheet;

	ReleaseWorkSheets();

	if (m_pWorkSheets)
	{
		m_pWorkSheets->Release();
		m_pWorkSheets = NULL;
	}
}

VOID CXLAutomationForJournal::ReleaseWorkSheets()
{
	INT nCount = m_WorkSheets.GetSize();
	for (INT i=0; i<nCount; i++)
	{
		IDispatch* pdispWorkSheet = m_WorkSheets.GetAt(i);
		if (pdispWorkSheet)
		{
			pdispWorkSheet->Release();
			pdispWorkSheet = NULL;
		}
	}
}

INT CXLAutomationForJournal::CreateJournalWorkSheet(BOOL bWorkObject/*=TRUE*/)
{
	if (NULL == m_pdispExcelApp)
		return -10;
	if (NULL == m_pdispWorkbook)
		return -11;

	if (m_pWorkSheets == NULL)
	{
		CXLAutomation::ClearAllArgs();

		VARIANTARG varg1;
		if (!CXLAutomation::ExlInvoke(m_pdispWorkbook, _T("Worksheets"), &varg1, DISPATCH_PROPERTYGET, 0))
		{
			return -21;
		}

		m_pWorkSheets = varg1.pdispVal;
	}

	CXLAutomation::ClearAllArgs();

	VARIANTARG varg;
	if (!CXLAutomation::ExlInvoke(m_pWorkSheets, _T("Add"), &varg, DISPATCH_METHOD, 0))
	{
		return -22;
	}

	m_pdispWorksheet = varg.pdispVal;

	m_WorkSheets.Add(m_pdispWorksheet);

	return m_WorkSheets.GetSize()-1;
}

INT CXLAutomationForJournal::SetWorkSheetName(CString strName)
{
	if (NULL == m_pdispWorksheet)
		return -10; 

	CXLAutomation::ClearAllArgs();

	CXLAutomation::AddArgumentCString(NULL, 0, strName);

	VARIANTARG varg;
	if (!CXLAutomation::ExlInvoke(m_pdispWorksheet, _T("Name"), &varg, DISPATCH_PROPERTYPUT, 0))
	{
		ASSERT(FALSE);
		return -21; 
	}

	return 0;
}
