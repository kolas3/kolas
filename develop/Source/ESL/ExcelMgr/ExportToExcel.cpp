// ExportToExcel.cpp : implementation file
// MAKE BY PWR 2008.09

#include "stdafx.h"
#include "ExportToExcel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportToExcel

// 오토메이션 관련 제어에 필요한 클래스들 선언.
COleVariant covTrue((short)TRUE),
			covFalse((short)FALSE),
			VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

CExportToExcel::CExportToExcel()
{
	EnableAutomation();
}

CExportToExcel::~CExportToExcel()
{
}

void CExportToExcel::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CListCtrl::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CExportToExcel, CListCtrl)
	//{{AFX_MSG_MAP(CExportToExcel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CExportToExcel, CListCtrl)
	//{{AFX_DISPATCH_MAP(CExportToExcel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IExportToExcel to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3BEF26D9-5712-4CF9-9E2C-2F2CFAEED52B}
static const IID IID_IExportToExcel =
{ 0x3bef26d9, 0x5712, 0x4cf9, { 0x9e, 0x2c, 0x2f, 0x2c, 0xfa, 0xee, 0xd5, 0x2b } };

BEGIN_INTERFACE_MAP(CExportToExcel, CListCtrl)
	INTERFACE_PART(CExportToExcel, IID_IExportToExcel, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportToExcel message handlers

// 엑셀 관련 제어를 위한 초기화들을 수행한다.
INT CExportToExcel::InitExcel()
{
	// Start Excel and get Application object...
	// 엑셀 어플리케이션을 디스패치 시킨다.
	if(!m_app.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("엑셀이 설치되어 있지 않습니다."));
		return 10;
	}

	// 엑셀 저장 할것인지 등의 경고 뜨게/안뜨게 (TRUE/FALSE) 하기
	// m_app.SetDisplayAlerts(false);
	
	
	
	
	m_nRow = 1;
	m_nColumn = 0;
	m_nCheckSheet = 1;

	return 0;
}

INT CExportToExcel::Open(CString sFileName)
{
	//오픈할때 파일명이 있다면 해당 엑셀파일을 열고
	//파일명이 없다면 빈 시트 생성한다.
	if( !sFileName.IsEmpty() )
	{
		// Instantiate Excel and start a new workbook.
		m_objbooks	= m_app.GetWorkbooks();	

		LPDISPATCH l_lpDisp;
		// 엑셀파일오픈
		l_lpDisp= m_objbooks.Open( sFileName,
								VOptional, VOptional, VOptional,
								VOptional, VOptional, VOptional,
								VOptional, VOptional, VOptional,
								VOptional, VOptional, VOptional );

		m_objbook.AttachDispatch(l_lpDisp);
		l_lpDisp = m_objbook.GetSheets();
		m_objsheets.AttachDispatch(l_lpDisp);
		m_objsheets	= m_objbook.GetWorksheets();
		m_objsheet	= m_objsheets.GetItem(COleVariant((short)1));
		// 시트 생성
	}
	else
	{
		m_objbooks	= m_app.GetWorkbooks();	
		// 시트 생성
		m_objbook	= m_objbooks.Add(VOptional);
		m_objsheets	= m_objbook.GetWorksheets();
		m_objsheet	= m_objsheets.GetItem(COleVariant((short)1));
	}

	return 0;
}

// 파일 경로와 이름을 받고 그 이름으로 저장을 시키는 함수.
INT CExportToExcel::SaveAs()
{
	SelectLocation();
	Save();
	return 0;
}

// 모든 세션들을 닫아주고 디스패치를 릴리즈 해주는 곳.
INT CExportToExcel::Close()
{
	// 북을 닫아주고
	m_objbook.Close(VOptional, COleVariant(m_strFileName), VOptional); 
	m_objbooks.Close();
	
	// 릴리즈 디스패치를 해준다.
	m_objComment.ReleaseDispatch();
	m_objComments.ReleaseDispatch();
	
	m_objBorders.ReleaseDispatch();
	m_objRange.ReleaseDispatch();
	m_objsheet.ReleaseDispatch();
	m_objsheets.ReleaseDispatch();
	m_objbook.ReleaseDispatch();
	m_objbooks.ReleaseDispatch();
	m_app.ReleaseDispatch();

	// 어플리케이션 종료.
	m_app.Quit();
	return 0;
}

INT CExportToExcel::Release()
{
	// 릴리즈 디스패치를 해준다.
	m_objComment.ReleaseDispatch();
	m_objComments.ReleaseDispatch();
	
	m_objBorders.ReleaseDispatch();
	m_objRange.ReleaseDispatch();
	m_objsheet.ReleaseDispatch();
	m_objsheets.ReleaseDispatch();
	m_objbook.ReleaseDispatch();
	m_objbooks.ReleaseDispatch();
	m_app.ReleaseDispatch();
	
	return 0;
}

// 엑셀을 화면에 띄워 보이게 해주는 함수.
INT CExportToExcel::VisableExcel(BOOL bValue)
{
	// 엑셀을 연다. 보이게 해준다.
	if(bValue)
		m_app.SetVisible(TRUE);
	else
		m_app.SetVisible(FALSE);

	return 0;
}

INT CExportToExcel::SetTextToExcel(INT nExRow, INT nExCol, CString strData, CString DataType, CListCtrl *pListCtrl)
{
	if ( _EXCEL_MAXCOLUMN < nExCol )
		return 10;	

	RangeSelect(nExCol, nExRow);

	if ( 0 == _tcscmp(DataType , _EXCEL_NUMERIC_ ))
		;
	else if ( 0 == _tcscmp(DataType , _EXCEL_DATE_ ))		
		m_objRange.SetNumberFormat(COleVariant(_T("@"))); 
	else if ( 0 == _tcscmp(DataType , _EXCEL_STRING_ ))
		m_objRange.SetNumberFormat(COleVariant(_T("@"))); 
	else
		return -25;
	
	m_objRange.SetValue(COleVariant(strData));
	m_odjFont = m_objRange.GetFont();
	m_odjFont.SetBold(covTrue);

	m_objRange.SetColumnWidth(COleVariant(short(pListCtrl->GetColumnWidth(nExCol) / 9)));

	return 0;
}

INT CExportToExcel::SetTextToExcel(INT nExRow, INT nExCol, CString strData, CString DataType, INT ColmnWidth)
{
	if ( _EXCEL_MAXCOLUMN < nExCol )
		return 10;	

	RangeSelect(nExCol, nExRow);

	if ( 0 == _tcscmp(DataType , _EXCEL_NUMERIC_ ))
		;
	else if ( 0 == _tcscmp(DataType , _EXCEL_DATE_ ))		
		m_objRange.SetNumberFormat(COleVariant(_T("@"))); 
	else if ( 0 == _tcscmp(DataType , _EXCEL_STRING_ ))
		m_objRange.SetNumberFormat(COleVariant(_T("@"))); 
	else
		return -25;
	
	m_objRange.SetValue(COleVariant(strData));
	m_odjFont = m_objRange.GetFont();
	m_odjFont.SetBold(covTrue);

	m_objRange.SetColumnWidth(COleVariant(short(ColmnWidth)));

	return 0;
}
INT CExportToExcel::SetTextToExcel(CString sPoint, CString strData)
{
	RangeSelect(sPoint);
	m_objRange.SetValue(COleVariant(strData));

	return 0;
}

// 파일경로를 받아 m_strFileName에 저장하는 함수.
INT CExportToExcel::SelectLocation()
{
	CString strDir;

	// 파일경로 창 뛰워서 경로를 포함한 이름 정보 받기.
	// 16/03/29 김혜영 : 한셀 적용하기 - 파일 저장형식이 잘못 작성되어 있어서 수정
/*// BEFORE ----------------------------------------------------------------------------------
	CFileDialog dlg(FALSE, NULL, _T("*.xls"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		_T("EXCEL files(*.xls)|All files(*.*)|*.*|"), this);
*/// AFTER -------------------------------------------------------------------------------
	CFileDialog dlg(FALSE, NULL, _T("*.xls"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		_T("EXCEL files(*.xls)|.xls|All files(*.*)|*.*||"), this);
//*/ END -------------------------------------------------------------------------------------
	
	if(dlg.DoModal() != IDOK)
	{
		MessageBox(_T("Excel 출력이 취소되었습니다."), _T("Warnning!"), MB_ICONEXCLAMATION);
		SetCurrentDirectory(strDir);
        return -5;
	}
	
	// 받은 경로 m_strFileName에 저장.
	m_strFileName.Format(_T("%s"), dlg.GetPathName(), dlg.GetFileName());
	return 0;
}

// m_strFileName 이름으로 엑셀저장하는 함수.
INT CExportToExcel::Save()
{
	// m_strFileName 이름으로 저장하기.
	m_objbook.SaveAs(COleVariant(m_strFileName), 
		VOptional,
		VOptional,
		VOptional,
		VOptional,
		VOptional,
		0,
		VOptional,
		VOptional,
		VOptional,
		VOptional); 

	return 0;
}

INT CExportToExcel::Save(CString szFileName)
{
	m_strFileName = szFileName;
	// szFileName 이름으로 저장하기.
	m_objbook.SaveAs(COleVariant(szFileName), 
		VOptional,
		VOptional,
		VOptional,
		VOptional,
		VOptional,
		0,
		VOptional,
		VOptional,
		VOptional,
		VOptional); 

	return 0;
}

// 시트를 다음 시트로 넘겨준다. 부르는 곳에서 이함수가 호출되면 로우를 1로 셋팅해야한다.
INT CExportToExcel::NextSheet()
{
	VARIANT     varValue;
	COleVariant varCount(m_objsheets.GetCount());
	V_VT(&varValue)       = VT_DISPATCH;
	V_DISPATCH(&varValue) = m_objsheets.GetItem(varCount);
	
	if ( m_nCheckSheet != m_objsheets.GetCount())
	{
		m_nCheckSheet++;
		m_objsheet	= m_objsheets.GetItem(COleVariant((short)m_nCheckSheet));
	}			
	else if( m_nCheckSheet == m_objsheets.GetCount())
	{
		m_objsheet = m_objsheets.Add(VOptional, varValue, VOptional, VOptional);
		m_nCheckSheet++; 
	}
	else
		return -15;
	
	return 0;
}

// 차트 위자드를 이용한 차트를 만드는 함수 
INT CExportToExcel::MakeChart(INT ChartType, CString StartPoint, CString EndPoint, INT Left, INT Top, INT Width, INT Height )
{
	lpDisp = m_objsheet.GetRange(COleVariant(StartPoint), COleVariant(EndPoint));
					 // The range containing the data to be charted.
	ASSERT(lpDisp);
	m_objRange.AttachDispatch(lpDisp);

	VARIANT var; // ChartWizard needs a Variant for the Source range.
	var.vt = VT_DISPATCH; // .vt is the usable member of the tagVARIANT
						 // Struct. Its value is a union of options.
	var.pdispVal = lpDisp; // Assign IDispatch pointer
						  // of the Source range to var.
	
	m_chartobjects = m_objsheet.ChartObjects(VOptional);
	ChartObject chartobject = m_chartobjects.Add(Left, Top, Width, Height);
	m_chart.AttachDispatch(chartobject.GetChart());

	m_chart.ChartWizard(var,                    // Source.
					COleVariant((short)ChartType),  // Gallery: 3d Column.
					VOptional,    // Format, use default.
					COleVariant((short)1),   // PlotBy: xlRows.
					COleVariant((short)0),   // CategoryLabels.
					COleVariant((short)1),   // SeriesLabels.
					COleVariant((short)TRUE),  // HasLegend.
					COleVariant(_T("KOLASIII")),  // Title.
					COleVariant(_T("Graph")),    // CategoryTitle.
					COleVariant(_T("Value")),  // ValueTitles.
					VOptional              // ExtraTitle.
					);
	return 0;
}

// colesafearray형태의 m_objSafeArray 의 모든 인자를 엑셀에 저장한다.
INT CExportToExcel::PutAllbyArray(INT nExRow, INT nExCol, INT nMaxRow, INT nMaxCol)
{
	CString strMax;
	
	// A1~ IV65536 까지의 넣을 위치를 스트링으로 저장한다. 
	// 시작 위치
	if ( _EXCEL_NALPHAMAX_ < nExCol-1 )
		m_strLocation.Format(_T("%c%c%d"), 'A' + (nExCol-1/_EXCEL_NALPHAMAX_), 'A' + (nExCol-1%_EXCEL_NALPHAMAX_), nExRow);
	else
		m_strLocation.Format(_T("%c%d"), 'A' + nExCol-1, nExRow);

	// 마지막 위치
	if ( _EXCEL_NALPHAMAX_ < nExCol + nMaxCol-2 )
		strMax.Format(_T("%c%c%d"), 'A' + (nExCol + nMaxCol-2/_EXCEL_NALPHAMAX_), 'A' + (nExCol + nMaxCol-2%_EXCEL_NALPHAMAX_), nExRow + nMaxRow-1);
	else
		strMax.Format(_T("%c%d"), 'A' + nExCol + nMaxCol-2, nExRow + nMaxRow-1);

    m_objRange = m_objsheet.GetRange(COleVariant(m_strLocation, VT_BSTR), COleVariant(strMax, VT_BSTR));
    m_objRange.SetValue(m_objSafeArray);
	m_objSafeArray.Detach();
//	m_objSafeArray.Clear();
	m_objSafeArray.Destroy();

	// 셀의 크기를 데이터에 맞춘다. (컬럼에서 컬럼 크기를 받아 처리하였음)
//	Range colRange(m_objRange.GetColumns());
//	colRange.AutoFit();
	return 0;
}

// colesafearray형태의 m_objSafeArray 에 인자를 저장한다.
INT CExportToExcel::FillSafeArray(INT nRow, INT nCol, CString strValue)
{
	if ( _EXCEL_MAXCOLUMN < nCol + 1 )
		return 10;

	VARIANT vTemp;
    long index[2]; 

    index[0] = nRow;
    index[1] = nCol;	
	
    VariantInit(&vTemp);
    vTemp.vt = VT_BSTR;
	vTemp.bstrVal = SysAllocString(strValue);

    m_objSafeArray.PutElement(index, vTemp.bstrVal);

    SysFreeString(vTemp.bstrVal);

    VariantClear(&vTemp);
	return 0;
}

// colesafearray형태의 m_objSafeArray를 생성하고 초기화한다. 
INT CExportToExcel::InitArray(INT nMaxRow, INT nMaxCol)
{	
	DWORD numElements[]={nMaxRow,nMaxCol};   //element array
    m_objSafeArray.Create(VT_BSTR, 2, numElements);
	return 0;
}

// 차트생성 여부를 물어 보는 함수
INT CExportToExcel::MakeChartQuestion(INT nChartType, CString strStartCell, CString strEndCell, INT nTop, INT nLeft, INT nWidth, INT nHeight)
{
	if(IDYES == MessageBox(_T("차트를 만드시겠습니까?"), NULL, MB_YESNO))
		MakeChart(nChartType, strStartCell, strEndCell, nTop, nLeft, nWidth, nHeight);
	return 0;	
}

// 테두리 설정
// DESC : 영역을 받아서 nLineStyle(선종류)와 nWeight(굵기)로 테두리를 그린다.
INT CExportToExcel::SetBorder(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, INT nLineStyle, INT nWeight)
{
	CString strMax;
	
	// A1~ IV65536 까지의 넣을 위치를 스트링으로 저장한다. 
	// 시작 위치
	if ( _EXCEL_NALPHAMAX_ < nMinCol-1 )
		m_strLocation.Format(_T("%c%c%d"), 'A' + (nMinCol-1/_EXCEL_NALPHAMAX_), 'A' + (nMinCol-1%_EXCEL_NALPHAMAX_), nMinRow);
	else
		m_strLocation.Format(_T("%c%d"), 'A' + nMinCol-1, nMinRow);

	// 마지막 위치 (시작위치부터 Max만큼더)
	if ( _EXCEL_NALPHAMAX_ < nMinCol + nMaxCol-2 )
		strMax.Format(_T("%c%c%d"), 'A' + (nMinCol + nMaxCol-2/_EXCEL_NALPHAMAX_), 'A' + (nMinCol + nMaxCol-2%_EXCEL_NALPHAMAX_), nMinRow + nMaxRow-1);
	else
		strMax.Format(_T("%c%d"), 'A' + nMinCol + nMaxCol-2, nMinRow + nMaxRow-1);

	// 영역설정
    m_objRange = m_objsheet.GetRange(COleVariant(m_strLocation, VT_BSTR), COleVariant(strMax, VT_BSTR));

	// 테두리표시
	m_objBorders = m_objRange.GetBorders();
	m_objBorders.SetLineStyle(COleVariant((short)nLineStyle));
	m_objBorders.SetWeight(COleVariant((short)nWeight));
	return 0;
}

// 특정셀의 내용 가져오기. 
CString CExportToExcel::GetValue(INT nCol, INT nRow)
{	
	RangeSelect(nCol, nRow);
    COleVariant sTemp(m_objRange.GetValue());
	CString sData(sTemp.bstrVal);

	return sData;
}

// Sheet 갯수를 구한다.
long CExportToExcel::GetSheetCount()
{
	if(m_objsheets==NULL)
		return 0;

	return m_objsheets.GetCount();
}

// nIndex의 Sheet이름을 구한다.
CString CExportToExcel::GetSheetName(INT nIndex)
{
	if(nIndex < 1 || GetSheetCount() < nIndex)
		return CString(_T("FAILED"));

	LPDISPATCH l_lpDisp = m_objsheets.GetItem(COleVariant((short)(nIndex)));
	m_objsheet.AttachDispatch(l_lpDisp);
	return m_objsheet.GetName();
}

// nIndex의 Sheet이동
VOID CExportToExcel::SetActiveSheet(INT nIndex)
{
	if(nIndex < 1 || GetSheetCount() < nIndex)
		return;
	
	LPDISPATCH l_lpDisp = m_objsheets.GetItem(COleVariant((short)(nIndex)));
	m_objsheet.AttachDispatch(l_lpDisp);
	m_objsheet.Activate();
}

// 치환하기
BOOL CExportToExcel::Replace(CString sWhat, CString sReplacement)
{
	RangeSelect(1, 1);

	return m_objRange.Replace(COleVariant(sWhat), COleVariant(sReplacement), 
		COleVariant((short)2), COleVariant((short)1), COleVariant((short)0),COleVariant((short)0),
		COleVariant((short)0),COleVariant((short)0),COleVariant((short)0),COleVariant((short)0));
}

// 파일복사
// DESC : sStyleFile(양식파일)을 sTempFile(템프파일)에 복사.
INT CExportToExcel::FileCopy(CString sStyleFile, CString sTempFile ){
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

// 틀고정
INT CExportToExcel::SetFreezePanes(INT nCol, INT nRow, BOOL bValue)
{
	RangeSelect(nCol, nRow);

	LPDISPATCH l_lpDisp = m_app.GetActiveWindow();
	m_objWindow.AttachDispatch(l_lpDisp);
	m_objWindow.Activate();
    //m_objWindow = m_objbook.GetWindows();
		
	m_objWindow.SetFreezePanes(bValue);
	m_objWindow.ReleaseDispatch();
	return 0;
}

// 메모 저장
// DESC: nCol, nRow (저장할위치)
INT CExportToExcel::AddComment(INT nCol, INT nRow, CString sValue)
{
	RangeSelect(nCol, nRow);
	LPDISPATCH l_lpDisp(m_objRange.GetComment());
	m_objComment.AttachDispatch(l_lpDisp);
	//m_objComment.SetComment(sValue);

	return 0;
}

// 메모 가져오기
// DESC: nCol, nRow (가져올 메모위치)
CString CExportToExcel::GetComment(INT nCol, INT nRow)
{
	m_objComments = m_objsheet.GetComments();
	if(m_objComments.GetCount() <= 0) return _T("");

	RangeSelect(nCol, nRow);
	LPDISPATCH l_lpDisp(m_objRange.GetComment());
	m_objComment.AttachDispatch(l_lpDisp);
	return m_objComment.Text(VOptional, VOptional, VOptional);
}

// 메모지우기
INT CExportToExcel::ClearComments(INT nCol, INT nRow)
{
	RangeSelect(nCol, nRow);
	m_objRange.ClearComments();

	return 0;
}

// 영역설정
INT CExportToExcel::GetRange(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow)
{
	// 셀 한칸
	if(nMaxCol == 0 || nMaxRow == 0)
	{
		RangeSelect(nMinCol, nMinRow);
	}
	else//셀 영역
	{
		CString strMax;
		
		// A1~ IV65536 까지의 넣을 위치를 스트링으로 저장한다. 
		// 시작 위치
		if ( _EXCEL_NALPHAMAX_ < nMinCol-1 )
			m_strLocation.Format(_T("%c%c%d"), 'A' + (nMinCol-1/_EXCEL_NALPHAMAX_), 'A' + (nMinCol-1%_EXCEL_NALPHAMAX_), nMinRow);
		else
			m_strLocation.Format(_T("%c%d"), 'A' + nMinCol-1, nMinRow);

		// 마지막 위치 (시작위치부터 Max만큼더)
		if ( _EXCEL_NALPHAMAX_ < nMinCol + nMaxCol-2 )
			strMax.Format(_T("%c%c%d"), 'A' + (nMinCol + nMaxCol-2/_EXCEL_NALPHAMAX_), 'A' + (nMinCol + nMaxCol-2%_EXCEL_NALPHAMAX_), nMinRow + nMaxRow-1);
		else
			strMax.Format(_T("%c%d"), 'A' + nMinCol + nMaxCol-2, nMinRow + nMaxRow-1);

		m_objRange = m_objsheet.GetRange(COleVariant(m_strLocation, VT_BSTR), COleVariant(strMax, VT_BSTR));
	}

	return 0;
}

// 셀선택
INT CExportToExcel::RangeSelect(INT nCol, INT nRow)
{
	// A1~ IV65536 까지의 넣을 위치를 스트링으로 저장한다. 
	if ( _EXCEL_NALPHAMAX_ < nCol-1 )
		m_strLocation.Format(_T("%c%c%d"), 'A' + (nCol-1/_EXCEL_NALPHAMAX_), 'A' + (nCol-1%_EXCEL_NALPHAMAX_), nRow);
	else
		m_strLocation.Format(_T("%c%d"), 'A' + nCol-1, nRow);

	m_objRange = m_objsheet.GetRange(COleVariant(m_strLocation), COleVariant(m_strLocation));
	m_objRange.Select();

	return 0;
}
INT CExportToExcel::RangeSelect(CString sValue)
{
	m_objRange = m_objsheet.GetRange(COleVariant(sValue), COleVariant(sValue));
	m_objRange.Select();

	return 0;
}