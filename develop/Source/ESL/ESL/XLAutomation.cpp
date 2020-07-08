// XLAutomation.cpp: implementation of the CXLAutomation class.
//This is C++ modification of the AutoXL C-sample from 
//Microsoft Excel97 Developer Kit, Microsoft Press 1997 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XLAutomation.h"
#include <ole2ver.h>
#include <string.h>
#include <winuser.h>
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
 *  Arrays of argument information, which are used to build up the arg list
 *  for an IDispatch call.  These arrays are statically allocated to reduce
 *  complexity, but this code could be easily modified to perform dynamic
 *  memory allocation.
 *
 *  When arguments are added they are placed into these arrays.  The
 *  Vargs array contains the argument values, and the lpszArgNames array
 *  contains the name of the arguments, or a NULL if the argument is unnamed.
 *  Flags for the argument such as NOFREEVARIANT are kept in the wFlags array.
 *
 *  When Invoke is called, the names in the lpszArgNames array are converted
 *  into the DISPIDs expected by the IDispatch::Invoke function.  The
 *  IDispatch::GetIDsOfNames function is used to perform the conversion, and
 *  the resulting IDs are placed in the DispIds array.  There is an additional
 *  slot in the DispIds and lpszArgNames arrays to allow for the name and DISPID
 *  of the method or property being invoked.
 *  
 *  Because these arrays are static, it is important to call the ClearArgs()
 *  function before setting up arguments.  ClearArgs() releases any memory
 *  in use by the argument array and resets the argument counters for a fresh
 *  Invoke.
 */
//INT			m_iArgCount;
//INT			m_iNamedArgCount;
//VARIANTARG	m_aVargs[MAX_DISP_ARGS];
//DISPID		m_aDispIds[MAX_DISP_ARGS + 1];		// one extra for the member name
//LPOLESTR	m_alpszArgNames[MAX_DISP_ARGS + 1];	// used to hold the argnames for GetIDs
//WORD		m_awFlags[MAX_DISP_ARGS];
//////////////////////////////////////////////////////////////////////

CXLAutomation::CXLAutomation()
{
	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	m_bIsExcel = FALSE;
//*/ END -------------------------------------------------------------------------------------

	m_pdispExcelApp = NULL;
	m_pdispWorkbook = NULL;
	m_pdispWorksheet = NULL;
	m_pdispActiveChart = NULL;
	InitOLE();
	StartExcel();
	SetExcelVisible(TRUE);
	CreateWorkSheet();
	//CreateXYChart();
}

CXLAutomation::CXLAutomation(BOOL bVisible)
{
	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	m_bIsExcel = FALSE;
//*/ END -------------------------------------------------------------------------------------

	m_pdispExcelApp = NULL;
	m_pdispWorkbook = NULL;
	m_pdispWorksheet = NULL;
	m_pdispActiveChart = NULL;
	InitOLE();
	StartExcel();
	SetExcelVisible(bVisible);
	CreateWorkSheet();
	//CreateXYChart();
}
// 18.10.30 JMJ KOL.RED.2018.021
CXLAutomation::CXLAutomation(BOOL bVisible, BOOL bIsExcel)
{
	m_bIsExcel = bIsExcel;
	m_pdispExcelApp = NULL;
	m_pdispWorkbook = NULL;
	m_pdispWorksheet = NULL;
	m_pdispActiveChart = NULL;
	InitOLE();
	StartExcel();
	SetExcelVisible(bVisible);
	CreateWorkSheet();
}
CXLAutomation::~CXLAutomation()
{
	//ReleaseExcel();
	ReleaseDispatch();
	OleUninitialize();
	OleInitialize(NULL);  
	
}

BOOL CXLAutomation::InitOLE()
{
	DWORD dwOleVer;

	OleUninitialize();
	
	dwOleVer = CoBuildVersion();
	
	// check the OLE library version
	if (rmm != HIWORD(dwOleVer)) 
	{
		MessageBox(NULL, _T("Incorrect version of OLE libraries."), _T("Failed"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	
	// could also check for minor version, but this application is
	// not sensitive to the minor version of OLE
	
	// initialize OLE, fail application if we can't get OLE to init.
	if (FAILED(OleInitialize(NULL))) 
	{
		MessageBox(NULL, _T("Cannot initialize OLE."), _T("Failed"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	
	return TRUE;
}

BOOL CXLAutomation::StartExcel()
{
	CLSID clsExcelApp;

	// if Excel is already running, return with current instance
	if (m_pdispExcelApp != NULL)
		return TRUE;

	/* Obtain the CLSID that identifies EXCEL.APPLICATION
	 * This value is universally unique to Excel versions 5 and up, and
	 * is used by OLE to identify which server to start.  We are obtaining
	 * the CLSID from the ProgID.
	 */
	// 16/03/29 김혜영 : 한셀 적용하기
/*// BEFORE ----------------------------------------------------------------------------------
	if (FAILED(CLSIDFromProgID(_T("Excel.Application"), &clsExcelApp))) 
	{
		MessageBox(NULL, _T("Cannot obtain CLSID from ProgID"), _T("Failed"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}

	// start a new copy of Excel, grab the IDispatch interface
	if (FAILED(CoCreateInstance(clsExcelApp, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (VOID**)&m_pdispExcelApp))) 
	{
		MessageBox(NULL, _T("Cannot start an instance of Excel for Automation."), _T("Failed"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}
*/// AFTER -----------------------------------------------------------------------------------
	/*
	if (FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) // 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	{
		m_bIsExcel = TRUE;
		if (FAILED(CLSIDFromProgID(_T("Excel.Application"), &clsExcelApp))) 
		{
			MessageBox(NULL, _T("Cannot obtain CLSID from ProgID"), _T("Failed"), MB_OK | MB_ICONSTOP);
			return FALSE;
		}
	}
	*/
	// 18.10.30 JMJ KOL.RED.2018.021
	if(m_bIsExcel == TRUE)
	{
		if (FAILED(CLSIDFromProgID(_T("Excel.Application"), &clsExcelApp))) 
		{
			MessageBox(NULL, _T("Cannot obtain CLSID from ProgID"), _T("Failed"), MB_OK | MB_ICONSTOP);
			return FALSE;
		}
	}
	else
	{
		if (FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp)))
		{
			MessageBox(NULL, _T("Cannot obtain CLSID from ProgID"), _T("Failed"), MB_OK | MB_ICONSTOP);
			return FALSE;
		}
	}

	// start a new copy of Excel, grab the IDispatch interface
	if (FAILED(CoCreateInstance(clsExcelApp, NULL, CLSCTX_LOCAL_SERVER, IID_IDispatch, (VOID**)&m_pdispExcelApp))) 
	{
		CString strMsg;
		strMsg.Format(_T("Cannot start an instance of %s for Automation."), m_bIsExcel?_T("Excel"):_T("HCell"));
		MessageBox(NULL, strMsg, _T("Failed"), MB_OK | MB_ICONSTOP);
		return FALSE;
	}
//*/ END -------------------------------------------------------------------------------------

	return TRUE;
}

/*******************************************************************
 *
 *								INVOKE
 *
 *******************************************************************/

/*
 *  INVOKE
 *
 *  Invokes a method or property.  Takes the IDispatch object on which to invoke,
 *  and the name of the method or property as a String.  Arguments, if any,
 *  must have been previously setup using the AddArgumentXxx() functions.
 *
 *  Returns TRUE if the call succeeded.  Returns FALSE if an error occurred.
 *  A messagebox will be displayed explaining the error unless the DISP_NOSHOWEXCEPTIONS
 *  flag is specified.  Errors can be a result of unrecognized method or property
 *  names, bad argument names, invalid types, or runtime-exceptions defined
 *  by the recipient of the Invoke.
 *
 *  The argument list is reset via ClearAllArgs() if the DISP_FREEARGS flag is
 *  specified.  If not specified, it is up to the caller to call ClearAllArgs().
 *
 *  The return value is placed in pvargReturn, which is allocated by the caller.
 *  If no return value is required, pass NULL.  It is up to the caller to free
 *  the return value (ReleaseVariant()).
 *
 *  This function calls IDispatch::GetIDsOfNames for every invoke.  This is not
 *  very efficient if the same method or property is invoked multiple times, since
 *  the DISPIDs for a particular method or property will remain the same during
 *  the lifetime of an IDispatch object.  Modifications could be made to this code
 *  to cache DISPIDs.  If the target application is always the same, a similar
 *  modification is to statically browse and store the DISPIDs at compile-time, since
 *  a given application will return the same DISPIDs in different sessions.
 *  Eliminating the extra cross-process GetIDsOfNames call can result in a
 *  signficant time savings.
 */


BOOL CXLAutomation::ExlInvoke(IDispatch *pdisp, LPOLESTR szMember, VARIANTARG * pvargReturn,
			WORD wInvokeAction, WORD wFlags)
{
	HRESULT hr = 0;
	DISPPARAMS dispparams;
	UINT uiArgErr = 0;
	EXCEPINFO excep;
	
	// Get the IDs for the member and its arguments.  GetIDsOfNames expects the
	// member name as the first name, followed by argument names (if any).
	m_alpszArgNames[0] = szMember;
	hr = pdisp->GetIDsOfNames( IID_NULL, m_alpszArgNames,
								1 + m_iNamedArgCount, LOCALE_SYSTEM_DEFAULT, m_aDispIds);
	if (FAILED(hr)) 
	{
		if (!(wFlags & DISP_NOSHOWEXCEPTIONS))
			ShowException(szMember, hr, NULL, 0);
		return FALSE;
	}
	
	if (pvargReturn != NULL)
		ClearVariant(pvargReturn);
	
	// if doing a property put(ref), we need to adjust the first argument to have a
	// named arg of DISPID_PROPERTYPUT.
	if (wInvokeAction & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF)) 
	{
		m_iNamedArgCount = 1;
		m_aDispIds[1] = DISPID_PROPERTYPUT;
		pvargReturn = NULL;
	}
	
	dispparams.rgdispidNamedArgs = m_aDispIds + 1;
	dispparams.rgvarg = m_aVargs;
	dispparams.cArgs = m_iArgCount;
	dispparams.cNamedArgs = m_iNamedArgCount;
	
	excep.pfnDeferredFillIn = NULL;
	
	hr = pdisp->Invoke(m_aDispIds[0], IID_NULL, LOCALE_SYSTEM_DEFAULT,
								wInvokeAction, &dispparams, pvargReturn, &excep, &uiArgErr);
	
	if (wFlags & DISP_FREEARGS)
		ClearAllArgs();
	
	if (FAILED(hr)) 
	{
		// display the exception information if appropriate:
		if (!(wFlags & DISP_NOSHOWEXCEPTIONS))
			ShowException(szMember, hr, &excep, uiArgErr);
	
		// free exception structure information
		SysFreeString(excep.bstrSource);
		SysFreeString(excep.bstrDescription);
		SysFreeString(excep.bstrHelpFile);
	
		return FALSE;
	}
	return TRUE;
}

/*
 *  ClearVariant
 *
 *  Zeros a variant structure without regard to current contents
 */
VOID CXLAutomation::ClearVariant(VARIANTARG *pvarg)
{
	pvarg->vt = VT_EMPTY;
	pvarg->wReserved1 = 0;
	pvarg->wReserved2 = 0;
	pvarg->wReserved3 = 0;
	pvarg->lVal = 0;
}

/*
 *  ClearAllArgs
 *
 *  Clears the existing contents of the arg array in preparation for
 *  a new invocation.  Frees argument memory if so marked.
 */
VOID CXLAutomation::ClearAllArgs()
{
	INT i;
	
	for (i = 0; i < m_iArgCount; i++) 
	{
		if (m_awFlags[i] & DISPARG_NOFREEVARIANT)
			// free the variant's contents based on type
			ClearVariant(&m_aVargs[i]);
		else
			ReleaseVariant(&m_aVargs[i]);
	}

	m_iArgCount = 0;
	m_iNamedArgCount = 0;
}

/*
 *  ReleaseVariant
 *
 *  Clears a particular variant structure and releases any external objects
 *  or memory contained in the variant.  Supports the data types listed above.
 */
VOID CXLAutomation::ReleaseVariant(VARIANTARG *pvarg)
{
	VARTYPE vt = 0;
	VARIANTARG *pvargArray = NULL;
	long lLBound = 0, lUBound = 0, l = 0;
	
	vt = pvarg->vt & 0xfff;		// mask off flags
	
	// check if an array.  If so, free its contents, then the array itself.
	if (V_ISARRAY(pvarg)) 
	{
		// variant arrays are all this routine currently knows about.  Since a
		// variant can contain anything (even other arrays), call ourselves
		// recursively.
		if (vt == VT_VARIANT) 
		{
			SafeArrayGetLBound(pvarg->parray, 1, &lLBound);
			SafeArrayGetUBound(pvarg->parray, 1, &lUBound);
			
			if (lUBound > lLBound) 
			{
				lUBound -= lLBound;
				
				SafeArrayAccessData(pvarg->parray, (VOID**)&pvargArray);
				
				for (l = 0; l < lUBound; l++) 
				{
					ReleaseVariant(pvargArray);
					pvargArray++;
				}
				
				SafeArrayUnaccessData(pvarg->parray);
			}
		}
		else 
		{
			MessageBox(NULL, _T("ReleaseVariant: Array contains non-variant type"), _T("Failed"), MB_OK | MB_ICONSTOP);
		}
		
		// Free the array itself.
		SafeArrayDestroy(pvarg->parray);
	}
	else 
	{
		switch (vt) 
		{
			case VT_DISPATCH:
				//(*(pvarg->pdispVal->lpVtbl->Release))(pvarg->pdispVal);
				pvarg->pdispVal->Release();
				break;
				
			case VT_BSTR:
				SysFreeString(pvarg->bstrVal);
				break;
				
			case VT_I2:
			case VT_BOOL:
			case VT_R8:
			case VT_ERROR:		// to avoid erroring on an error return from Excel
				// no work for these types
				break;
				
			default:
				MessageBox(NULL, _T("ReleaseVariant: Unknown type"), _T("Failed"), MB_OK | MB_ICONSTOP);
				break;
		}
	}	
	ClearVariant(pvarg);
}

BOOL CXLAutomation::SetExcelVisible(BOOL bVisible)
{
	if (m_pdispExcelApp == NULL)
		return FALSE;
	
	ClearAllArgs();
	AddArgumentBool(NULL, 0, bVisible);
	return ExlInvoke(m_pdispExcelApp, _T("Visible"), NULL, DISPATCH_PROPERTYPUT, DISP_FREEARGS);
}

/*******************************************************************
 *
 *					   ARGUMENT CONSTRUCTOR FUNCTIONS
 *
 *  Each function adds a single argument of a specific type to the list
 *  of arguments for the current invoke.  If appropriate, memory may be
 *  allocated to represent the argument.  This memory will be
 *  automatically freed the next time ClearAllArgs() is called unless
 *  the NOFREEVARIANT flag is specified for a particular argument.  If
 *  NOFREEVARIANT is specified it is the responsibility of the caller
 *  to free the memory allocated for or contained within the argument.
 *
 *  Arguments may be named.  The name string must be a C-style string
 *  and it is owned by the caller.  If dynamically allocated, the caller
 *  must free the name string.
 *
 *******************************************************************/

/*
 *  Common code used by all variant types for setting up an argument.
 */

VOID CXLAutomation::AddArgumentCommon(LPOLESTR lpszArgName, WORD wFlags, VARTYPE vt)
{
	ClearVariant(&m_aVargs[m_iArgCount]);
	
	m_aVargs[m_iArgCount].vt = vt;
	m_awFlags[m_iArgCount] = wFlags;
	
	if (lpszArgName != NULL) 
	{
		m_alpszArgNames[m_iNamedArgCount + 1] = lpszArgName;
		m_iNamedArgCount++;
	}
}	
	

BOOL CXLAutomation::AddArgumentDispatch(LPOLESTR lpszArgName, WORD wFlags, IDispatch * pdisp)
{
	AddArgumentCommon(lpszArgName, wFlags, VT_DISPATCH);
	m_aVargs[m_iArgCount++].pdispVal = pdisp;
	return TRUE;
}


BOOL CXLAutomation::AddArgumentInt2(LPOLESTR lpszArgName, WORD wFlags, INT i)
{
	AddArgumentCommon(lpszArgName, wFlags, VT_I2);
	m_aVargs[m_iArgCount++].iVal = i;
	return TRUE;
}


BOOL CXLAutomation::AddArgumentBool(LPOLESTR lpszArgName, WORD wFlags, BOOL b)
{
	AddArgumentCommon(lpszArgName, wFlags, VT_BOOL);
	// Note the variant representation of True as -1
	m_aVargs[m_iArgCount++].boolVal = b ? -1 : 0;
	return TRUE;
}

BOOL CXLAutomation::AddArgumentDouble(LPOLESTR lpszArgName, WORD wFlags, double d)
{
	AddArgumentCommon(lpszArgName, wFlags, VT_R8);
	m_aVargs[m_iArgCount++].dblVal = d;
	return TRUE;
}


BOOL CXLAutomation::ReleaseExcel()
{
	if (m_pdispExcelApp == NULL)
		return TRUE;
	
	// Tell Excel to quit, since for automation simply releasing the IDispatch
	// object isn't enough to get the server to shut down.
	
	// Note that this code will hang if Excel tries to display any message boxes.
	// This can occur if a document is in need of saving.  The CreateChart() code
	// always clears the dirty bit on the documents it creates, avoiding this problem.
	ClearAllArgs();
	ExlInvoke(m_pdispExcelApp, _T("Quit"), NULL, DISPATCH_METHOD, 0);
	
	// Even though Excel has been told to Quit, we still need to release the
	// OLE object to account for all memory.
	ReleaseDispatch();
	
	return TRUE;
}

//Create an empty workshet
BOOL CXLAutomation::CreateWorkSheet()
{
	if(NULL == m_pdispExcelApp)
		return FALSE;

	BOOL fResult;
	VARIANTARG varg1, varg2;
	IDispatch *pdispRange = NULL;
	IDispatch *pdispActiveSheet = NULL;
	IDispatch *pdispActiveCell = NULL;
	IDispatch *pdispCrt = NULL;
		
	// Set wb = [application].Workbooks.Add(template := xlWorksheet)
	ClearAllArgs();
	if (!ExlInvoke(m_pdispExcelApp, _T("Workbooks"), &varg1, DISPATCH_PROPERTYGET, 0))
		return FALSE;
	
	ClearAllArgs();
	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	if(m_bIsExcel != TRUE)
	{
		AddArgumentInt2(NULL, 0, 1);
		fResult = ExlInvoke(varg1.pdispVal, _T("Item"), &varg2, DISPATCH_PROPERTYGET, 0);
	}
	else
//*/ END -------------------------------------------------------------------------------------
	{
		AddArgumentInt2(_T("Template"), 0, xlWorksheet);
		fResult = ExlInvoke(varg1.pdispVal, _T("Add"), &varg2, DISPATCH_METHOD, 0);
	}

	ReleaseVariant(&varg1);
	if (!fResult)
		return FALSE;
	m_pdispWorkbook = varg2.pdispVal;
	
	// Set ws = wb.Worksheets(1)
	ClearAllArgs();
	AddArgumentInt2(NULL, 0, 1);
	if (!ExlInvoke(m_pdispWorkbook, _T("Worksheets"), &varg2, DISPATCH_PROPERTYGET, 0))
		goto CreateWsBail;
	m_pdispWorksheet = varg2.pdispVal;

	fResult = TRUE;

CreateWsExit:	
	if (pdispRange != NULL)
		pdispRange->Release();
	if (pdispCrt != NULL)
		pdispCrt->Release();
	return fResult;
	
CreateWsBail:
	fResult = FALSE;
	goto CreateWsExit;
}

/*
 *  OLE and IDispatch use a BSTR as the representation of strings.
 *  This constructor automatically copies the passed-in C-style string
 *  into a BSTR.  It is important to not set the NOFREEVARIANT flag
 *  for this function, otherwise the allocated BSTR copy will probably
 *  get lost and cause a memory leak.
 */

BOOL CXLAutomation::AddArgumentOLEString(LPOLESTR lpszArgName, WORD wFlags, LPOLESTR lpsz)
{
	BSTR b;
	
	b = SysAllocString(lpsz);
	if (!b)
		return FALSE;
	AddArgumentCommon(lpszArgName, wFlags, VT_BSTR);
	m_aVargs[m_iArgCount++].bstrVal = b;
	return TRUE;

}

BOOL CXLAutomation::AddArgumentCString(LPOLESTR lpszArgName, WORD wFlags, CString szStr)
{
	BSTR b;
	
	b = szStr.AllocSysString();
	if (!b)
		return FALSE;
	AddArgumentCommon(lpszArgName, wFlags, VT_BSTR);
	m_aVargs[m_iArgCount++].bstrVal = b;
	
	return TRUE;
}

//Perform Worksheets.Cells(x,y).Value = szStr
BOOL CXLAutomation::SetCellsValueToString(double Column, double Row, CString szStr)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;
	if(szStr.IsEmpty())
		return FALSE;
	long nBuffSize = szStr.GetLength();
	

	VARIANTARG vargRng;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, Column);
	AddArgumentDouble(NULL, 0, Row);
	if(!ExlInvoke(m_pdispWorksheet, _T("Cells"),&vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

    AddArgumentCString(NULL, 0, szStr );
	if (!ExlInvoke(vargRng.pdispVal, _T("Value"), NULL, DISPATCH_PROPERTYPUT, 0))
		return FALSE;
	ReleaseVariant(&vargRng);
	
	
	return TRUE;
}
BOOL CXLAutomation::Create3YChart(INT nX1Column,INT nY1Column)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;

	CString csRange;
	BOOL fResult;
	VARIANTARG varg1, varg2;
	IDispatch *pdispRange = NULL;
	IDispatch *pdispCrt = NULL;


//    Range(_T("B:B,D:D")).Select
	ClearAllArgs();
	CString csFirstRange(_T("B:B,D:D"));
	CString csSecondRange(_T("C:C"));
	/*
	csFirstRange.SetAt(0,'A'+nX1Column-1);
	csFirstRange.SetAt(2,'A'+nX1Column-1);
	csFirstRange.SetAt(4,'A'+nY1Column-1);
	csFirstRange.SetAt(6,'A'+nY1Column-1);
	*/
	csFirstRange.SetAt(0,'A'+0);		
	csFirstRange.SetAt(2,'A'+13);	
	//csFirstRange.SetAt(4,'A'+10);
	//csFirstRange.SetAt(6,'A'+15);
	
	AddArgumentCString(NULL, 0,csFirstRange);
	if (!ExlInvoke(m_pdispWorksheet, _T("Range"), &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		goto CreateChartBail;
	pdispRange = varg2.pdispVal;
	

	//    First range Select
	ClearAllArgs();
	if (!ExlInvoke(pdispRange, _T("Select"), &varg1, DISPATCH_PROPERTYGET, 0))
		goto CreateChartBail;
	
	// set crt = wb.Charts.Add
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, _T("Charts"), &varg1, DISPATCH_PROPERTYGET, 0))
		goto CreateChartBail;
	ClearAllArgs();
	fResult = ExlInvoke(varg1.pdispVal, _T("Add"), &varg2, DISPATCH_METHOD, 0);
	ReleaseVariant(&varg1);
	if (!fResult)
		goto CreateChartBail;
	pdispCrt = varg2.pdispVal;

	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, _T("ActiveChart"), &varg1, DISPATCH_PROPERTYGET, 0))
		goto CreateChartBail;
	m_pdispActiveChart = varg1.pdispVal;

	ClearAllArgs();
	AddArgumentInt2(NULL, 0, xlXYScatterLinesNoMarkers);
	if (!ExlInvoke(m_pdispActiveChart, _T("ChartType"), &varg1, DISPATCH_PROPERTYPUT, 0))
		goto CreateChartBail;

//
	ClearAllArgs();
	AddArgumentInt2(_T("PlotBy"), 0, xlColumns);
	AddArgumentDispatch(_T("Source"), 0, pdispRange);	// will auto-free
	pdispRange = NULL;
	if (!ExlInvoke(m_pdispActiveChart, _T("SetSourceData"), NULL, DISPATCH_METHOD, DISP_FREEARGS))
		goto CreateChartBail;

	
fResult = TRUE;
	

CreateChartExit:
	if (pdispRange != NULL)
		pdispRange->Release();
	if (pdispCrt != NULL)
		pdispCrt->Release();
	return fResult;
	
CreateChartBail:
	fResult = FALSE;
	goto CreateChartExit;

	return TRUE;

}
//Create XY chart. Y values are in column nYColumn.
BOOL CXLAutomation::CreateXYChart(INT nYColumn)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;

	BOOL fResult;
	VARIANTARG varg1, varg2;
	IDispatch *pdispRange = NULL;
	IDispatch *pdispCrt = NULL;

		
	// set sourceRange = ws.Columns(nYColumn)
	ClearAllArgs();
	//AddArgumentDouble(NULL, 0, nYColumn);
	/*
	AddArgumentDouble(NULL, 0, 1);	
	if (!ExlInvoke(m_pdispWorksheet, _T("Columns"), &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		goto CreateChartBail;
	pdispRange = varg2.pdispVal;	
	*/

	ClearAllArgs();
		AddArgumentOLEString(NULL, 0, _T("A1:N7"));
		if (!ExlInvoke(m_pdispWorksheet, _T("Range"), &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
			return FALSE;
		pdispRange = varg2.pdispVal;	

	
	
	// set crt = wb.Charts.Add
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, _T("Charts"), &varg1, DISPATCH_PROPERTYGET, 0))
		goto CreateChartBail;
	ClearAllArgs();
	fResult = ExlInvoke(varg1.pdispVal, _T("Add"), &varg2, DISPATCH_METHOD, 0);
	ReleaseVariant(&varg1);
	if (!fResult)
		goto CreateChartBail;
	pdispCrt = varg2.pdispVal;

	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, _T("ActiveChart"), &varg1, DISPATCH_PROPERTYGET, 0))
		goto CreateChartBail;
	m_pdispActiveChart = varg1.pdispVal;

	ClearAllArgs();
	AddArgumentInt2(NULL, 0, xl3DBar);
	if (!ExlInvoke(m_pdispActiveChart, _T("ChartType"), &varg1, DISPATCH_PROPERTYPUT, 0))
		goto CreateChartBail;

	//Charts.Add
    //ActiveChart.ChartType = xlXYScatterLinesNoMarkers
    //ActiveChart.SetSourceData Source:=Sheets(_T("Sheet1")).Range(_T("A:A, B:B")), PlotBy:= _
    //    xlColumns
	ClearAllArgs();
	AddArgumentInt2(_T("PlotBy"), 0, xlColumns);
	AddArgumentDispatch(_T("Source"), 0, pdispRange);	// will auto-free
	pdispRange = NULL;
	if (!ExlInvoke(m_pdispActiveChart, _T("SetSourceData"), NULL, DISPATCH_METHOD, DISP_FREEARGS))
		goto CreateChartBail;


	fResult = TRUE;

CreateChartExit:
	if (pdispRange != NULL)
		pdispRange->Release();
	if (pdispCrt != NULL)
		pdispCrt->Release();
	return fResult;
	
CreateChartBail:
	fResult = FALSE;
	goto CreateChartExit;

	return TRUE;

}

BOOL CXLAutomation::SetRangeValueDouble(LPOLESTR lpszRef, double d)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;

	VARIANTARG vargRng;
	BOOL fResult;
	
	ClearAllArgs();
	AddArgumentOLEString(NULL, 0, lpszRef);
	if (!ExlInvoke(m_pdispWorksheet, _T("Range"), &vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	
	AddArgumentDouble(NULL, 0, d);
	fResult = ExlInvoke(vargRng.pdispVal, _T("Value"), NULL, DISPATCH_PROPERTYPUT, 0);
	ReleaseVariant(&vargRng);
	
	return fResult;

}

/*
 *  Constructs an 1-dimensional array containing variant strings.  The strings
 *  are copied from an incoming array of C-Strings.
 */
BOOL CXLAutomation::AddArgumentCStringArray(LPOLESTR lpszArgName, WORD wFlags, LPOLESTR *paszStrings, INT iCount)
{
	SAFEARRAY *psa = NULL;
	SAFEARRAYBOUND saBound;
	VARIANTARG *pvargBase = NULL;
	VARIANTARG *pvarg = NULL;
	INT i = 0, j = 0;
	
	saBound.lLbound = 0;
	saBound.cElements = iCount;
	
	psa = SafeArrayCreate(VT_VARIANT, 1, &saBound);
	if (psa == NULL)
		return FALSE;
	
	SafeArrayAccessData(psa, (VOID**) &pvargBase);
	
	pvarg = pvargBase;
	for (i = 0; i < iCount; i++) 
	{
		// copy each string in the list of strings
		ClearVariant(pvarg);
		pvarg->vt = VT_BSTR;
		if ((pvarg->bstrVal = SysAllocString(*paszStrings++)) == NULL) 
		{
			// memory failure:  back out and free strings alloc'ed up to
			// now, and then the array itself.
			pvarg = pvargBase;
			for (j = 0; j < i; j++) 
			{
				SysFreeString(pvarg->bstrVal);
				pvarg++;
			}
			SafeArrayDestroy(psa);
			return FALSE;
		}
		pvarg++;
	}
	
	SafeArrayUnaccessData(psa);

	// With all memory allocated, setup this argument
	AddArgumentCommon(lpszArgName, wFlags, VT_VARIANT | VT_ARRAY);
	m_aVargs[m_iArgCount++].parray = psa;
	return TRUE;

}
//May be new points have been added to the plot data source. Update plot range
BOOL CXLAutomation::UpdatePlotRange(INT nYColumn)
{
	if((NULL == m_pdispWorksheet) || (NULL == m_pdispWorkbook) || (NULL == m_pdispActiveChart))
		return FALSE;

	VARIANTARG varg1;
	IDispatch *pdispRange = NULL;
	IDispatch *pdispActiveChart = NULL;
	BOOL bResult = TRUE;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, nYColumn);
	if (!ExlInvoke(m_pdispWorksheet, _T("Columns"), &varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	pdispRange = varg1.pdispVal;

   //ActiveChart.SetSourceData Source:=Sheets(_T("Sheet1")).Range(_T("A:A, B:B")), PlotBy:= _
   //    xlColumns
	ClearAllArgs();
	AddArgumentInt2(_T("PlotBy"), 0, xlColumns);
	AddArgumentDispatch(_T("Source"), 0, pdispRange);	// will auto-free
	pdispRange = NULL;
	if (!ExlInvoke(m_pdispActiveChart, _T("SetSourceData"), NULL, DISPATCH_METHOD, DISP_FREEARGS))
		bResult = FALSE;

	ClearAllArgs();
	if(NULL != pdispRange)
	{
		pdispRange->Release();
		pdispRange = NULL;
	}
	
	return bResult;
	
}

//Copy string to clipboard and paste it to worksheet
BOOL CXLAutomation::PasteStringToWorksheet(CString pDataBuffer, CString strFileName)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;
	if(pDataBuffer.IsEmpty())
		return FALSE;

	long nBuffSize = pDataBuffer.GetLength(); 
	//Nothing to copy
	if(0 == nBuffSize)
		return FALSE;

	HANDLE hMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, 24);
  
	  if (OpenClipboard(NULL) )
	  {
		HGLOBAL hClipMem;
		TCHAR* buffer;
		BOOL bResult = TRUE;
		
		
		//++2008.06.16 UPDATE BY CJY {{++
		// 유니코드용으로 변경
		EmptyClipboard();
		hClipMem = GlobalAlloc(GMEM_DDESHARE, (nBuffSize+1)*sizeof(TCHAR));
		buffer = (TCHAR*)GlobalLock(hClipMem);
		wmemcpy(buffer, pDataBuffer, nBuffSize);
		GlobalUnlock(hClipMem);
		SetClipboardData(CF_UNICODETEXT, hClipMem);
		CloseClipboard();		
		// 이전소스
		// EmptyClipboard();
		// hClipMem = GlobalAlloc(GMEM_DDESHARE, nBuffSize);
		// buffer = (TCHAR*)GlobalLock(hClipMem);
		// wmemcpy(buffer, pDataBuffer, nBuffSize);
		// GlobalUnlock(hClipMem);
		// SetClipboardData(CF_TEXT, hClipMem);
		// CloseClipboard();		
		//--2008.06.16 UPDATE BY CJY --}}
		
		//Paste data from clipboard
		// set sourceRange = ws.Range(_T("A1"))
		IDispatch* pdispRange = NULL;
		VARIANTARG varg1, varg2;

		ClearAllArgs();
		AddArgumentOLEString(NULL, 0, _T("A1"));
		if (!ExlInvoke(m_pdispWorksheet, _T("Range"), &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
			return FALSE;
		pdispRange = varg2.pdispVal;


		//Force past from clipboard
		//set as = sourceRange.Select
		ClearAllArgs();
		// 16/03/29 김혜영 : 한셀 적용하기 - Select는 속성이 아닌 메소드이므로 변경
/*// BEFORE ----------------------------------------------------------------------------------
		if (!ExlInvoke(pdispRange, _T("Select"), &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
*/// AFTER -----------------------------------------------------------------------------------
		if (!ExlInvoke(pdispRange, _T("Select"), &varg2, DISPATCH_METHOD, DISP_FREEARGS))
//*/ END -------------------------------------------------------------------------------------	
			goto Failed;		

		// set wb.ActiveSheet.Paste
		ClearAllArgs();
		if (!ExlInvoke(m_pdispWorkbook, _T("ActiveSheet"), &varg1, DISPATCH_PROPERTYGET, 0))
			goto Failed;
		ClearAllArgs();		

		// set wb.ActiveSheet.Paste
		ClearAllArgs();
		if (!ExlInvoke(m_pdispWorkbook, _T("ActiveSheet"), &varg1, DISPATCH_PROPERTYGET, 0))
			goto Failed;
		ClearAllArgs();
		bResult = ExlInvoke(varg1.pdispVal, _T("Paste"), &varg2, DISPATCH_METHOD, 0);
		ReleaseVariant(&varg1);
	
		goto Success;

Failed:
		bResult = FALSE;
Success:
		if(NULL != pdispRange)
		{
			pdispRange->Release();
			pdispRange = NULL;
		}

		return bResult;
	  }
	  return FALSE;

}

// 18.10.30 JMJ KOL.RED.2018.021
BOOL CXLAutomation::PasteStringToCSV(CString pDataBuffer, CString strFileName)
{
	CString sTempDirectory = _T("rpt_tmp\\Excel");						//tmp파일위치
	CString sTempFileName = _T("tmp_ExcelExport");
	CString sCurrentFileName = sTempFileName+_T(".csv");				
	CString sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp파일

	CString sErrorMsg, sCnt;
	CFileFind fFind;

	INT nCnt = 0;

	if (!fFind.FindFile(sTempDirectory))
	{
		// 디렉토리 없으면 생성
		if (!CreateDirectory(sTempDirectory, NULL))
		{
			//tmp디렉토리생성실패
			TRACE(_T("Error Occured = tmp디렉토리생성실패\r\n"));
			AfxMessageBox(_T("tmp디렉토리 생성에 실패했습니다."));
			return -1;
		}
	}
	// tmp파일 존재시 삭제
	while(fFind.FindFile(sTempFile))
	{
		CFileException e;
		CFile f;
		f.Open(sTempFile, CFile::modeRead | CFile::typeBinary, &e);
		//if(e.m_cause == CFileException::sharingViolation) // 이미 실행되어있어 lock걸린 상태
		if(e.m_cause != 0) // 예외처리발생
		{
			nCnt++;
			sCnt.Format(_T("%d"), nCnt);
			sCurrentFileName = sTempFileName + sCnt + _T(".csv");		//차이
			sTempFile = sTempDirectory + _T("\\") + sCurrentFileName;	//tmp파일
		}
		else
		{
			f.Close();
			if(!DeleteFile(sTempFile))
			{
				//tmp디렉토리생성실패
				TRACE(_T("Error Occured = tmp파일삭제실패\r\n"));
				AfxMessageBox(_T("기존tmp파일 삭제에 실패했습니다."));
				return -1;
			}
		}
	}
	
	TCHAR TmpDir[_MAX_PATH];
	ZeroMemory(TmpDir, sizeof(TCHAR)*_MAX_PATH);
	GetCurrentDirectory(_MAX_PATH, TmpDir);
	CString sFile;

	CFile fFile;
	sFile.Format(_T("%s\\%s"), TmpDir, sTempFile);
	if( fFile.Open( sFile ,  CFile::modeCreate | CFile::modeWrite ) == FALSE ) 
	{
		AfxMessageBox( sFile );
		return FALSE;
	}
	USHORT nShort = 0xfeff;  // Unicode BOM(Byte Order Mark).
	fFile.Write(&nShort,2);

	fFile.Write(pDataBuffer, pDataBuffer.GetLength()*2);
	fFile.Close();

	if(strFileName.IsEmpty() == FALSE)
	{
		INT ids = FileCopy(sFile, strFileName);
		if(ids<0) 
		{
			TRACE(_T("Error Occured = FileCopy"));
			CString strMsg;
			strMsg.Format(_T("파일복사에 실패 하였습니다.\r\n해당파일의 프로세스가 실행중인지 확인해 주십시오."));
			AfxMessageBox(strMsg);
		}
		else
		{
			sFile = strFileName;
		}
	}

	ShellExecute(NULL,_T("OPEN"), sFile,NULL,NULL,SW_SHOW);
	return TRUE;
}


INT CXLAutomation::FileCopy(CString sStyleFile, CString sTempFile ){
	HANDLE hSource = INVALID_HANDLE_VALUE;
	HANDLE hTarget = INVALID_HANDLE_VALUE;
	DWORD dwFileSize = 0;
	
	try
	{
		// 양식파일(원본) 핸들
		hSource = CreateFile(sStyleFile, GENERIC_READ, 0, NULL,	OPEN_EXISTING, 0, NULL);
		if(hSource == INVALID_HANDLE_VALUE)
			throw MAKE_HRESULT(1, FACILITY_WIN32, GetLastError());		
		
		// 템프파일(복사본) 핸들
		hTarget = CreateFile(sTempFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
		if(hTarget == INVALID_HANDLE_VALUE)
			throw MAKE_HRESULT(1, FACILITY_WIN32, GetLastError());
		
		dwFileSize = GetFileSize(hSource, NULL);
		
		BYTE pBuff[BLOCK_LEN];	
		ZeroMemory(pBuff, sizeof(BYTE)*BLOCK_LEN);
		DWORD dwCopyLength = BLOCK_LEN;
		DWORD dwTransffered = 0;
		DWORD dwReadBytes = 0;
		DWORD dwWroteBytes = 0;
		
		while( dwTransffered < dwFileSize )
		{
			dwCopyLength = dwFileSize - dwTransffered;
			if(dwCopyLength >= BLOCK_LEN)
				dwCopyLength = BLOCK_LEN;
			
			if(!ReadFile(hSource, pBuff, dwCopyLength, &dwReadBytes, NULL) )
				throw MAKE_HRESULT(1, FACILITY_WIN32, GetLastError());
			
			if(!WriteFile(hTarget, pBuff, dwCopyLength, &dwWroteBytes, NULL) )
				throw MAKE_HRESULT(1, FACILITY_WIN32, GetLastError());
			
			dwTransffered += dwCopyLength;
		} // end of while
		
	}
	
	catch (HRESULT hr)
	{
		TRACE1("Error Occured .. %d", hr);
		return -1;
	}
	
	// file handle close
	if(hSource != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hSource);
		hSource = NULL;
	}
	if(hTarget != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hTarget);
		hTarget = NULL;
	}
	
	return 0;
}

//Clean up: release dipatches
VOID CXLAutomation::ReleaseDispatch()
{
	if(NULL != m_pdispExcelApp)
	{
		m_pdispExcelApp->Release();
		m_pdispExcelApp = NULL;
	}

	if(NULL != m_pdispWorksheet)
	{
		m_pdispWorksheet->Release();
		m_pdispWorksheet = NULL;
	}

	if(NULL != m_pdispWorkbook)
	{
		m_pdispWorkbook->Release();
		m_pdispWorkbook = NULL;
	}

	if(NULL != m_pdispActiveChart)
	{
		m_pdispActiveChart->Release();
		m_pdispActiveChart = NULL;
	}
}

VOID CXLAutomation::ShowException(LPOLESTR szMember, HRESULT hr, EXCEPINFO *pexcep, UINT uiArgErr)
{
	TCHAR szBuf[512];
	ZeroMemory(szBuf, sizeof(TCHAR)*512);
	switch (GetScode(hr)) 
	{
		case DISP_E_UNKNOWNNAME:
			wsprintf(szBuf, _T("%s: Unknown name or named argument."), szMember);
			break;
	
		case DISP_E_BADPARAMCOUNT:
			wsprintf(szBuf, _T("%s: Incorrect number of arguments."), szMember);
			break;
			
		case DISP_E_EXCEPTION:
			wsprintf(szBuf, _T("%s: Error %d: "), szMember, pexcep->wCode);
			if (pexcep->bstrDescription != NULL)
				lstrcat(szBuf, (TCHAR*)pexcep->bstrDescription);
			else
				lstrcat(szBuf, _T("<<No Description>>"));
			break;
			
		case DISP_E_MEMBERNOTFOUND:
			wsprintf(szBuf, _T("%s: method or property not found."), szMember);
			break;
		
		case DISP_E_OVERFLOW:
			wsprintf(szBuf, _T("%s: Overflow while coercing argument values."), szMember);
			break;
		
		case DISP_E_NONAMEDARGS:
			wsprintf(szBuf, _T("%s: Object implementation does not support named arguments."),
						szMember);
		    break;
		    
		case DISP_E_UNKNOWNLCID:
			wsprintf(szBuf, _T("%s: The locale ID is unknown."), szMember);
			break;
		
		case DISP_E_PARAMNOTOPTIONAL:
			wsprintf(szBuf, _T("%s: Missing a required parameter."), szMember);
			break;
		
		case DISP_E_PARAMNOTFOUND:
			wsprintf(szBuf, _T("%s: Argument not found, argument %d."), szMember, uiArgErr);
			break;
			
		case DISP_E_TYPEMISMATCH:
			wsprintf(szBuf, _T("%s: Type mismatch, argument %d."), szMember, uiArgErr);
			break;

		default:
			wsprintf(szBuf, _T("%s: Unknown error occured."), szMember);
			break;
	}
	
	MessageBox(NULL, szBuf, _T("OLE Error"), MB_OK | MB_ICONSTOP);

}
//Delete entire line from the current worksheet
//Worksheet.Rows(nLine).Select
//Selection.Delete Shift:=xlUp
BOOL CXLAutomation::DeleteRow(long nRow)
{
	if(NULL == m_pdispWorksheet)
		return FALSE;

	VARIANTARG varg1;
	
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, nRow);
	if (!ExlInvoke(m_pdispWorksheet, _T("Rows"), &varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	ClearAllArgs();
	AddArgumentInt2(_T("Shift"), 0, xlUp);
	if (!ExlInvoke(varg1.pdispVal, _T("Delete"), NULL, DISPATCH_METHOD, DISP_FREEARGS))
		return FALSE;
	
		
	return TRUE;

}

//Save current workbook as an Excel file:
//ActiveWorkbook.SaveAs
//FileName:=szFileName, FileFormat:=xlNormal,
//Password:=szPassword,
//WriteResPassword:=szWritePassword,
//ReadOnlyRecommended:= bReadOnly,
//CreateBackup:= bBackup
BOOL CXLAutomation::SaveAs(CString szFileName, INT nFileFormat, CString szPassword, CString szWritePassword, BOOL bReadOnly, BOOL bBackUp)
{
	if(NULL == m_pdispWorkbook)
		return FALSE;
	ClearAllArgs();
	AddArgumentBool(_T("CreateBackup"), 0, bBackUp);
	AddArgumentBool(_T("ReadOnlyRecommended"), 0, bReadOnly);
	AddArgumentCString(_T("WriteResPassword"), 0, szWritePassword);
	AddArgumentCString(_T("Password"), 0, szPassword);
	AddArgumentCString(_T("FileName"), 0, szFileName);
	if (!ExlInvoke(m_pdispWorkbook, _T("SaveAs"), NULL, DISPATCH_METHOD, DISP_FREEARGS))
		return FALSE;

	return TRUE;
}

//Get Worksheet.Calls(nColumn, nRow).Value
//This method is not fully tested - see code coments 
CString CXLAutomation::GetCellValueCString(INT nColumn, INT nRow)
{
	CString szValue =_T("");
	if(NULL == m_pdispWorksheet)
		return szValue;
	
	VARIANTARG vargRng, vargValue;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, nColumn);
	AddArgumentDouble(NULL, 0, nRow);
	if(!ExlInvoke(m_pdispWorksheet, _T("Cells"),&vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return szValue;
    
	if (!ExlInvoke(vargRng.pdispVal, _T("Value"), &vargValue, DISPATCH_PROPERTYGET, 0))
		return szValue;

	VARTYPE Type = vargValue.vt;
	switch (Type) 
		{
			case VT_UI1:
				{
					_TUCHAR nChr = vargValue.bVal;
					szValue = nChr;
				}
				break;
			case VT_I4:
				{
					long nVal = vargValue.lVal;
					szValue.Format(_T("%i"), nVal);
				}
				break;
			case VT_R4:
				{
					float fVal = vargValue.fltVal;
					szValue.Format(_T("%f"), fVal);
				}
				break;
			case VT_R8:
				{
					double dVal = vargValue.dblVal;
					szValue.Format(_T("%f"), dVal);
				}
				break;
			case VT_BSTR:
				{
					BSTR b = vargValue.bstrVal;
					szValue = b;
				}
				break;
			case VT_BYREF|VT_UI1:
				{
					//Not tested
					_TUCHAR* pChr = (unsigned short*)vargValue.pbVal;
					szValue = *pChr;
				}
				break;
			case VT_BYREF|VT_BSTR:
				{
					//Not tested
					BSTR* pb = vargValue.pbstrVal;
					szValue = *pb;
				}
			case 0:
				{
					//Empty
					szValue = _T("");
				}

				break;
		}
	
		
//	ReleaseVariant(&vargRng);
//	ReleaseVariant(&vargValue);
	
	return szValue;

}
//Insert picture from file szFileName to worksheet
//The left top corner of the picture is position in (Column, nRow)  
//on the worksheet
//Size of the picture in % of original size is given by dPicWidth, dPicHeight
//If dPicWidth = 0.0, dPicHeight = 0.0 or dPicWidth = 1.0, dPicHeight = 1.0
//the picture has default (i.e., original) size 
BOOL CXLAutomation::InsertPictureToWorksheet(CString szFileName, INT Column, INT Row, double dPicWidth, double dPicHeight)
{
	//No file name provided. Leave. 
	if(szFileName.IsEmpty())
		return FALSE;
	if(NULL == m_pdispWorksheet)
		return FALSE;

	//First, select cell where you want copy the picture (i.e., the top left corner of the picture
	//Leave if Column and Row are outside the worksheet
	if((Column < 1) || (Row < 1))
		return FALSE;

	VARIANTARG vargRng, vargActiveCell;
	VARIANTARG varg1, varg2;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, Column);
	AddArgumentDouble(NULL, 0, Row);
	if(!ExlInvoke(m_pdispWorksheet, _T("Cells"),&vargRng, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ClearAllArgs();
	if(!ExlInvoke(vargRng.pdispVal, _T("Select"),&vargActiveCell, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ReleaseVariant(&vargRng);
	ReleaseVariant(&vargActiveCell);

	//Run this macros to incert picture from file 
	//ActiveSheet.Pictures.Insert(_T("c:\mypicture.bmp")).Select
	//or
	//ActiveSheet.Pictures.Insert (_T("C:\mypicture.bmp"))
    //ActiveSheet.Pictures.ShapeRange.ScaleWidth 0.31, msoScaleFromTopLeft
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorksheet, _T("Pictures"), &varg1, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ClearAllArgs();
	AddArgumentCString(NULL, 0, szFileName);
	if (!ExlInvoke(varg1.pdispVal, _T("Insert"), &varg2, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	if((dPicWidth != 0.0) && (dPicHeight != 0.0))
	{

		//Run this macros to resize the picture:
		//Selection.ShapeRange.ScaleWidth dPicWidth, msoFalse, msoScaleFromTopLeft
		//Selection.ShapeRange.ScaleHeight dPicWidth, msoFalse, msoScaleFromTopLeft
		VARIANTARG vargImage;
		ClearAllArgs();
		if (!ExlInvoke(varg1.pdispVal, _T("ShapeRange"), &vargImage, DISPATCH_PROPERTYGET, DISP_FREEARGS))
			return FALSE;

		ClearAllArgs();
		//msoScaleFromTopLeft = 0 - this argument indicate scaling from top left
		AddArgumentInt2(NULL, 0, 0);
		AddArgumentDouble(NULL, 0, dPicWidth);
     	if (!ExlInvoke(vargImage.pdispVal, _T("ScaleWidth"), NULL, DISPATCH_METHOD, 0)) //DISP_FREEARGS))
			return FALSE;
		if (!ExlInvoke(vargImage.pdispVal, _T("ScaleHeight"), NULL, DISPATCH_METHOD, 0)) //DISP_FREEARGS))
			return FALSE;
		ClearAllArgs();
		ReleaseVariant(&vargImage);
		
	}
	ReleaseVariant(&varg1);
	ReleaseVariant(&varg2);
	return TRUE;
}
//Paste image to clipborad
BOOL CXLAutomation::PlaceImageToClipboard(BYTE *pImage)
{
	if(NULL == pImage)
		return FALSE;
	LPBITMAPINFOHEADER pBmpHeader = (LPBITMAPINFOHEADER) pImage;
	long nImageBufferSize;
	//Find the size of buffer to store this image
	long nBitCount = pBmpHeader->biBitCount;
	long nWidth = pBmpHeader->biWidth;
	long nHeight = pBmpHeader->biHeight;


	//We will support only 8-bit and 24-bit images
	//8-bit image has 4x256 = 1024-byte palette table followed after the bitmapinfoheader 
	//24-bit image does not have the palette table
	//When calculating buffer size remember that the BMP image is 32-bit alligned
	if((nBitCount != 8) && (nBitCount != 24))
		return FALSE;
	//calculate number of bytes in one image line
	long nBytePerLine = (nBitCount / 8) * nWidth;
	//Here, for instance 72-bit line should be rounded to 96-bit to be 32-bit aligned 
	nBytePerLine =  4 * ((nBytePerLine + 3) / 4); 
	nImageBufferSize = sizeof(BITMAPINFOHEADER) + (nBytePerLine * nHeight);
	//if image is 8-bit, add the size of palette table
	if(8 == nBitCount)
		nImageBufferSize = nImageBufferSize + 256 * sizeof(RGBQUAD);

	  //Place image to clipboard 	 
	  if (OpenClipboard(NULL) )
	  {
		HGLOBAL hClipMem;
		BYTE* buffer;
		BOOL bResult = TRUE;
		EmptyClipboard();
		hClipMem = GlobalAlloc(GMEM_DDESHARE, nImageBufferSize);
		//hClipMem = GlobalAlloc(GMEM_MOVEABLE, nImageBufferSize);
		buffer = (BYTE*)GlobalLock(hClipMem);
		memcpy(buffer, pImage, nImageBufferSize);
		SetClipboardData(CF_DIB, hClipMem);
		GlobalUnlock(hClipMem);
		CloseClipboard();
		return TRUE;
	  }
	return FALSE;
}
//Insert image to worksheet using clipboard
BOOL CXLAutomation::InsertPictureToWorksheet(BYTE *pImage, INT Column, INT Row, double dPicWidth, double dPicHeight)
{
	//Place image to clipboard
	if(NULL != pImage)
		if(!PlaceImageToClipboard(pImage))
			return FALSE;
	
	//Select cell where you want copy the picture (i.e., the top left corner of the picture
	//Leave if Column and Row are outside the worksheet
	if((Column < 1) || (Row < 1))
		return FALSE;

	VARIANTARG vargCell, vargActiveCell;
	VARIANTARG vargActiveSelection;
	
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, Column);
	AddArgumentDouble(NULL, 0, Row);
	if(!ExlInvoke(m_pdispWorksheet, _T("Cells"),&vargCell, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ClearAllArgs();
	if(!ExlInvoke(vargCell.pdispVal, _T("Select"),&vargActiveCell, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ReleaseVariant(&vargCell);
	ReleaseVariant(&vargActiveCell);

	//Paste image from clipboard
	//by runing:
	//ActiveSheet.Paste
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorksheet, _T("Paste"), &vargActiveSelection, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	ReleaseVariant(&vargActiveSelection);

	//Select image
	if (!ExlInvoke(m_pdispWorksheet, _T("Pictures"), &vargActiveSelection, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	//Resize image
	if((dPicWidth != 0.0) && (dPicHeight != 0.0))
	{
	
		//Run this macros to resize the picture:
		//Selection.ShapeRange.ScaleWidth dPicWidth, msoFalse, msoScaleFromTopLeft
		//Selection.ShapeRange.ScaleHeight dPicWidth, msoFalse, msoScaleFromTopLeft
		VARIANTARG vargImage;
		ClearAllArgs();
		if (!ExlInvoke(vargActiveSelection.pdispVal, _T("ShapeRange"), &vargImage, DISPATCH_PROPERTYGET, DISP_FREEARGS))
			return FALSE;

		ClearAllArgs();
		//msoScaleFromTopLeft = 0 - this argument indicate scaling from top left
		AddArgumentInt2(NULL, 0, 0);
		AddArgumentDouble(NULL, 0, dPicWidth);
     	if (!ExlInvoke(vargImage.pdispVal, _T("ScaleWidth"), NULL, DISPATCH_METHOD, 0)) //DISP_FREEARGS))
			return FALSE;
		if (!ExlInvoke(vargImage.pdispVal, _T("ScaleHeight"), NULL, DISPATCH_METHOD, 0)) //DISP_FREEARGS))
			return FALSE;
		ClearAllArgs();
		ReleaseVariant(&vargImage);
		
	}
	ReleaseVariant(&vargActiveSelection);
	return TRUE;
}
//Open Microsoft Excel file and switch to the firs available worksheet. 
BOOL CXLAutomation::OpenExcelFile(CString szFileName)
{
	//Leave if the file cannot be open
	if(NULL == m_pdispExcelApp)
		return FALSE;
	if(szFileName.IsEmpty())
		return FALSE;

	VARIANTARG varg1, vargWorkbook, vargWorksheet;
	ClearAllArgs();
	if (!ExlInvoke(m_pdispExcelApp, _T("Workbooks"), &varg1, DISPATCH_PROPERTYGET, 0))
		return FALSE;
		
	ClearAllArgs();
	AddArgumentCString(_T("Filename"), 0, szFileName);
	if (!ExlInvoke(varg1.pdispVal, _T("Open"), &vargWorkbook, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	//Now let's get the first worksheet of this workbook
	ClearAllArgs();
	AddArgumentInt2(NULL, 0, 1);
	if (!ExlInvoke(vargWorkbook.pdispVal, _T("Worksheets"), &vargWorksheet, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;

	//Close the empty worksheet
	ClearAllArgs();
	if (!ExlInvoke(m_pdispWorkbook, _T("Close"), NULL, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	//Remember the newly open worksheet 
	m_pdispWorkbook = vargWorkbook.pdispVal;
	m_pdispWorksheet = vargWorksheet.pdispVal;
	return TRUE;
}

BOOL CXLAutomation::SetCellBackgroundColor(long Row, long Column, INT ColorIndex)
{
	//Macro, which we want to execute:
	//Select cell (Row, Column)
	// Selection.Interior.ColorIndex = ColorIndex
//Selection.Interior.Pattern = xlSolid
//Selection.Interior.PatternColorIndex = xlAutomatic

	//Select cell (Row, Column)
	VARIANTARG vargSelection;
	ClearAllArgs();
	AddArgumentDouble(NULL, 0, Column);
	AddArgumentDouble(NULL, 0, Row);
if(!ExlInvoke(m_pdispWorksheet, _T("Cells"),&vargSelection, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	//Here vargSelection exposes Selection object
	
//Now we doing this macro:
		// Selection.Interior.ColorIndex = ColorIndex
//Selection.Interior.Pattern = xlSolid
//Selection.Interior.PatternColorIndex = xlAutomatic
	VARIANTARG vargInterior;
	ClearAllArgs();
if(!ExlInvoke(vargSelection .pdispVal, _T("Interior"),&vargInterior, DISPATCH_PROPERTYGET, DISP_FREEARGS))
		return FALSE;
	//We got vargInterior, which exposes Excelb.s Interior object 
	
//We are doing Selection.Interior.ColorIndex = ColorIndex
ClearAllArgs();
AddArgumentInt2(NULL, 0, ColorIndex);
if(!ExlInvoke(vargInterior .pdispVal, _T("ColorIndex"), NULL, DISPATCH_METHOD, DISP_FREEARGS))
		return FALSE;

//We are doing Selection.Interior.Pattern = xlSolid
ClearAllArgs();
AddArgumentInt2(NULL, 0, 1); // Here xlSolid =  1
if(!ExlInvoke(vargInterior .pdispVal, _T("Pattern"), NULL, DISPATCH_METHOD, DISP_FREEARGS))
		return FALSE;

//We are doing Selection.Interior. PatternColorIndex = xlAutomatic
ClearAllArgs();
AddArgumentInt2(NULL, 0, -4105);  //Here xlAutomatic = -4105
if(!ExlInvoke(vargInterior .pdispVal, _T("PatternColorIndex"), NULL, DISPATCH_METHOD, DISP_FREEARGS))
		return FALSE;

	return TRUE;
}
