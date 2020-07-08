// ExportToHCell.cpp : implementation file

#include "stdafx.h"
#include "ExportToHCell.h"

/////////////////////////////////////////////////////////////////////////////
// CExportToExcel

// 오토메이션 관련 제어에 필요한 클래스들 선언.
COleVariant covTRUE((short)TRUE),
			covFALSE((short)FALSE),
			covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);


CExportToHCell::CExportToHCell()
{
	EnableAutomation();
}

CExportToHCell::~CExportToHCell()
{
}

void CExportToHCell::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CListCtrl::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CExportToHCell, CListCtrl)
	//{{AFX_MSG_MAP(CExportToExcel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CExportToHCell, CListCtrl)
	//{{AFX_DISPATCH_MAP(CExportToExcel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IExportToHCell to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3BEF26D9-5712-4CF9-9E2C-2F2CFAEED53B}
static const IID IID_IExportToHCell =
{ 0x3bef26d9, 0x5712, 0x4cf9, { 0x9e, 0x2c, 0x2f, 0x2c, 0xfa, 0xee, 0xd5, 0x3b } };

BEGIN_INTERFACE_MAP(CExportToHCell, CListCtrl)
	INTERFACE_PART(CExportToHCell, IID_IExportToHCell, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportToHCell message handlers

// 엑셀 관련 제어를 위한 초기화들을 수행한다.
INT CExportToHCell::InitExcel()
{
	// Start HCcel and get Application object...
	// 한셀 어플리케이션을 디스패치 시킨다.
	if(!m_app.CreateDispatch(_T("HCell.Application")))
	{
		AfxMessageBox(_T("한셀이 설치되어 있지 않습니다."));
		return 10;
	}

	// 한셀 저장 할것인지 등의 경고 뜨게/안뜨게 (TRUE/FALSE) 하기
	m_app.put_DisplayAlerts(TRUE);	
	
	m_nRow = 1;
	m_nColumn = 0;
	m_nCheckSheet = 1;

	return 0;
}

INT CExportToHCell::Open(CString sFileName)
{
	//오픈할때 파일명이 있다면 해당 엑셀파일을 열고
	//파일명이 없다면 빈 시트 생성한다.	
	
	if( !sFileName.IsEmpty() )
	{		
		/*
		// 기존국중소스
		m_lpDisp = m_app.get_Workbooks();

		if (m_lpDisp == NULL) 
			return -1;

		m_objBooks.AttachDispatch(m_lpDisp);
		
		// 엑셀파일오픈
		m_lpDisp = m_objBooks.Open( COleVariant(sFileName),
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional);
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objBook.AttachDispatch(m_lpDisp);

		m_lpDisp = m_objBook.get_Worksheets();
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objSheets.AttachDispatch(m_lpDisp);
		
		m_lpDisp = m_objSheets.get_Item(COleVariant((short)1));
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objSheet.AttachDispatch(m_lpDisp);
		// 시트 생성		
		*/
		m_objBooks	= m_app.get_Workbooks();	

		LPDISPATCH l_lpDisp;
		// 엑셀파일오픈
		l_lpDisp= m_objBooks.Open( COleVariant(sFileName),
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional, covOptional,
								covOptional, covOptional);

		m_objBook.AttachDispatch(l_lpDisp);
		l_lpDisp = m_objBook.get_Sheets();
		m_objSheets.AttachDispatch(l_lpDisp);
		m_objSheets	= m_objBook.get_Worksheets();
		m_objSheet	= m_objSheets.get_Item(COleVariant((short)1));
		m_lpDisp = l_lpDisp;
	}
	else
	{
		m_lpDisp = m_app.get_Workbooks();

		if (m_lpDisp == NULL) 
			return -1;

		m_objBooks.AttachDispatch(m_lpDisp);
		
		m_lpDisp = m_app.get_ActiveWorkbook();
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objBook.AttachDispatch(m_lpDisp);

		m_lpDisp = m_objBook.get_Worksheets();
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objSheets.AttachDispatch(m_lpDisp);
		
		m_lpDisp = m_objSheets.get_Item(COleVariant((short)1));
		if (m_lpDisp == NULL) 
			return -1;
		
		m_objSheet.AttachDispatch(m_lpDisp);
	}

	return 0;
}

// 파일 경로와 이름을 받고 그 이름으로 저장을 시키는 함수.
INT CExportToHCell::SaveAs()
{
	SelectLocation();
	Save();
	return 0;
}

// 시트를 다음 시트로 넘겨준다. 부르는 곳에서 이함수가 호출되면 로우를 1로 셋팅해야한다.
INT CExportToHCell::NextSheet()
{
	VARIANT     varValue;
	COleVariant varCount(m_objSheets.get_Count());
	V_VT(&varValue)       = VT_DISPATCH;
	V_DISPATCH(&varValue) = m_objSheets.get_Item(varCount);
	
	if ( m_nCheckSheet != m_objSheets.get_Count())
	{
		m_nCheckSheet++;
		m_objSheets	= m_objSheets.get_Item(COleVariant((short)m_nCheckSheet));
	}			
	else if( m_nCheckSheet == m_objSheets.get_Count())
	{
		m_objSheets = m_objSheets.Add(covOptional, varValue, covOptional, covOptional);
		m_nCheckSheet++; 
	}
	else
		return -15;
	
	return 0;
}

// 파일경로를 받아 m_strFileName에 저장하는 함수.
INT CExportToHCell::SelectLocation()
{
	CString strDir;

	// 파일경로 창 뛰워서 경로를 포함한 이름 정보 받기.
	CFileDialog dlg(FALSE, NULL, _T("*.cell"), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
		_T("한셀 통합 문서(*.cell)|*.cell|모든 파일|*.*||"), this);
	if(dlg.DoModal() != IDOK)
	{
		MessageBox(_T("한셀 출력이 취소되었습니다."), _T("Warnning!"), MB_ICONEXCLAMATION);
		SetCurrentDirectory(strDir);
        return -5;
	}
	
	// 받은 경로 m_strFileName에 저장.
	m_strFileName.Format(_T("%s"), dlg.GetPathName(), dlg.GetFileName());
	return 0;
}

// m_strFileName 이름으로 엑셀저장하는 함수.
INT CExportToHCell::Save()
{
	// m_strFileName 이름으로 저장하기.
	m_objBook.SaveAs(COleVariant(m_strFileName), 
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		0,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional); 

	return 0;
}

INT CExportToHCell::Save(CString szFileName)
{
	RangeSelect(1,1);
	m_strFileName = szFileName;
	szFileName.MakeLower();

	int nFileFormat = -4143;  // HCell Format : -4143, Excel Format : 43
	if (szFileName.Right(3) == _T("xls"))
	{
		nFileFormat = 43;
	}

	m_app.put_DisplayAlerts(FALSE);

	// szFileName 이름으로 저장하기.
	m_objBook.SaveAs(COleVariant(szFileName), 
		COleVariant((short)nFileFormat),
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		1,
		covOptional,
		covOptional,
		covOptional,
		covOptional,
		covOptional); 

	m_app.put_DisplayAlerts(TRUE);
	return 0;
}

INT CExportToHCell::SetTextToHCell(INT nExRow, INT nExCol, CString strData, CString DataType, CListCtrl *pListCtrl)
{
	if ( _EXCEL_MAXCOLUMN < nExCol )
		return 10;	

	RangeSelect(nExCol, nExRow);

	m_objRange.put_Value(covOptional, COleVariant(strData));

	m_lpDisp = m_objRange.get_Font();
	if (m_lpDisp == NULL) 
		return -1;

	CFont0 objFont;
	objFont.AttachDispatch(m_lpDisp);
	objFont.put_Bold(covTRUE);
	objFont.ReleaseDispatch();

	m_objRange.put_ColumnWidth(COleVariant(short(pListCtrl->GetColumnWidth(nExCol) / 9)));
	m_objRange.put_Value(covOptional, COleVariant(strData));
	return 0;
}

INT CExportToHCell::SetTextToHCell(INT nExRow, INT nExCol, CString strData, CString DataType, INT ColmnWidth)
{
	if ( _EXCEL_MAXCOLUMN < nExCol )
		return 10;	

	RangeSelect(nExCol, nExRow);

	m_lpDisp = m_objRange.get_Font();
	if (m_lpDisp == NULL) 
		return -1;
	
	CFont0 objFont;
	objFont.AttachDispatch(m_lpDisp);
	objFont.put_Bold(covTRUE);
	objFont.ReleaseDispatch();

	m_objRange.put_ColumnWidth(COleVariant(short(ColmnWidth)));
	m_objRange.put_Value(covOptional, COleVariant(strData));

	return 0;
}

INT CExportToHCell::SetTextToHCell(INT nExRow, INT nExCol, CString strData)
{
	if ( _EXCEL_MAXCOLUMN < nExCol )
		return 10;	

	RangeSelect(nExCol, nExRow);

	m_objRange.put_Value(covOptional, COleVariant(strData));

	return 0;
}

INT CExportToHCell::SetTextToHCell(CString sPoint, CString strData)
{
	RangeSelect(sPoint);
	m_objRange.put_Value(covOptional, COleVariant(strData));

	return 0;
}

// colesafearray형태의 m_objSafeArray를 생성하고 초기화한다. 
INT CExportToHCell::InitArray(INT nMaxRow, INT nMaxCol)
{	
	DWORD numElements[]={nMaxRow,nMaxCol};   //element array
    m_objSafeArray.Create(VT_BSTR, 2, numElements);
	return 0;
}

// colesafearray형태의 m_objSafeArray 의 모든 인자를 엑셀에 저장한다.
INT CExportToHCell::PutAllbyArray(INT nExRow, INT nExCol, INT nMaxRow, INT nMaxCol)
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

	m_lpDisp = m_objSheet.get_Range(COleVariant(m_strLocation), COleVariant(strMax));
	if (m_lpDisp == NULL) 
		return -1;
	m_objRange.AttachDispatch(m_lpDisp);
	m_objRange.put_Value(covOptional, COleVariant(m_objSafeArray));

	m_objSafeArray.Detach();
	m_objSafeArray.Destroy();

	// 셀의 크기를 데이터에 맞춘다. (컬럼에서 컬럼 크기를 받아 처리하였음)
//	Range colRange(m_objRange.GetColumns());
//	colRange.AutoFit();
	return 0;
}

// colesafearray형태의 m_objSafeArray 에 인자를 저장한다.
INT CExportToHCell::FillSafeArray(INT nRow, INT nCol, CString strValue)
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

// 폰트 지정
// DESC : 영역을 받아서 nBold, nItalic, clrForeColor, nSize로 폰트를 지정한다.
INT CExportToHCell::SetFont(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, BOOL bBold, BOOL bItalic, COLORREF clrForeColor, INT nSize)
{	
	if(nMaxCol > 0 && nMaxRow > 0)
		RangeSelect(nMinCol, nMinRow, nMaxCol, nMaxRow);
	else
		RangeSelect(nMinCol, nMinRow);

	// 폰트 지정	
	m_lpDisp = m_objRange.get_Font();
	if (m_lpDisp == NULL) 
		return -1;

	CFont0 objFont;
	objFont.AttachDispatch(m_lpDisp);
	objFont.put_Bold(COleVariant((short)bBold));
	objFont.put_Italic(COleVariant((short)bItalic));
	objFont.put_Color(COleVariant((double)clrForeColor));
	objFont.put_Size(COleVariant((short)nSize));
	objFont.ReleaseDispatch();

	return 0;
}

// 테두리 설정
// DESC : 영역을 받아서 nLineStyle(선종류)와 nWeight(굵기)로 테두리를 그린다.
INT CExportToHCell::SetBorder(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, INT nLineStyle, INT nWeight)
{
	RangeSelect(nMinCol, nMinRow, nMaxCol, nMaxRow);

	// 테두리표시
	m_lpDisp = m_objRange.get_Borders();
	if (m_lpDisp == NULL) 
		return -1;

	CBorders objBorders;
	objBorders.AttachDispatch(m_lpDisp);
	objBorders.put_LineStyle(COleVariant((short)nLineStyle));
	objBorders.put_Weight(COleVariant((short)nWeight));
	objBorders.ReleaseDispatch();

	return 0;
}

// 수평 정렬
// DESC : 영역을 받아서 nAlign(정렬)로 수평정렬을 한다. (Gerneral:1, Left:-4131, Center:-4108, Rignt:-4152)
INT CExportToHCell::SetHorizAlign(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, INT nAlign)
{
	RangeSelect(nMinCol, nMinRow, nMaxCol, nMaxRow);
	
	// 수평정렬	
	m_objRange.put_HorizontalAlignment(COleVariant((short)nAlign));

	return 0;
}

// 셀 배경색 지정
// DESC : 영역을 받아서 clrBackColr(배경색)로 셀 배경을 채운다. 
INT CExportToHCell::SetBkColor(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow, COLORREF clrBkColor)
{
	RangeSelect(nMinCol, nMinRow, nMaxCol, nMaxRow);

	// 배경색 지정
	m_lpDisp = m_objRange.get_Interior();
	if (m_lpDisp == NULL) 
		return -1;

	Cnterior objInterior;
	objInterior.AttachDispatch(m_lpDisp);
	objInterior.put_Color(COleVariant((double)clrBkColor));
	objInterior.ReleaseDispatch();

	return 0;
}

// 틀고정
INT CExportToHCell::SetFreezePanes(INT nCol, INT nRow, BOOL bValue)
{
	RangeSelect(nCol, nRow);

	m_lpDisp = m_app.get_ActiveWindow();
	if (m_lpDisp == NULL) 
		return -1;

	CWindow0 objWindow(m_lpDisp);	
	objWindow.put_FreezePanes(bValue);
	objWindow.ReleaseDispatch();

	return 0;
}

// 특정셀의 내용 가져오기. 
CString CExportToHCell::GetValue(INT nCol, INT nRow)
{	
	RangeSelect(nCol, nRow);
	
	COleVariant sTemp(m_objRange.get_Text());	//한셀추가수정
	CString sData(sTemp.bstrVal);

	return sData;
}

// Sheet 갯수를 구한다.
long CExportToHCell::GetSheetCount()
{
	if(m_objSheets==NULL)
		return 0;

	return m_objSheets.get_Count();
}

// nIndex의 Sheet이름을 구한다.
CString CExportToHCell::GetSheetName(INT nIndex)
{
	if(nIndex < 1 || GetSheetCount() < nIndex)
		return CString(_T("FAILED"));

	m_lpDisp = m_objSheets.get_Item(COleVariant((short)(nIndex)));
	if (m_lpDisp == NULL) 
		return CString(_T("FAILED"));
	m_objSheet.AttachDispatch(m_lpDisp);
	return m_objSheet.get_Name();
}

// nIndex의 Sheet이동
VOID CExportToHCell::SetActiveSheet(INT nIndex)
{
	if(nIndex < 1 || GetSheetCount() < nIndex)
		return;
	
	m_lpDisp = m_objSheets.get_Item(COleVariant((short)(nIndex)));
	if (m_lpDisp == NULL) 
		return;
	m_objSheet.AttachDispatch(m_lpDisp);
	m_objSheet.Activate();
}

// 치환하기
BOOL CExportToHCell::Replace(CString sWhat, CString sReplacement)
{
	RangeSelect(1, 1);

	return m_objRange.Replace(COleVariant(sWhat), COleVariant(sReplacement), 
		COleVariant((short)2), COleVariant((short)1), COleVariant((short)0),COleVariant((short)0),
		COleVariant((short)0),COleVariant((short)0));
}

// 파일복사
// DESC : sStyleFile(양식파일)을 sTempFile(템프파일)에 복사.
INT CExportToHCell::FileCopy(CString sStyleFile, CString sTempFile ){
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

// 메모 저장
// DESC: nCol, nRow (저장할위치)
INT CExportToHCell::AddComment(INT nCol, INT nRow, CString sValue)
{
	//한셀지원하지않음
	/* 
	RangeSelect(nCol, nRow);
	LPDISPATCH l_lpDisp(m_objRange.GetComment());
	m_objComment.AttachDispatch(l_lpDisp);
	//m_objComment.SetComment(sValue);
	*/
	return 0;
}

// 메모 가져오기
// DESC: nCol, nRow (가져올 메모위치)
CString CExportToHCell::GetComment(INT nCol, INT nRow)
{
	m_lpDisp = m_objSheet.get_Comments();
	if (m_lpDisp == NULL) 
		return _T("");

	m_objComments.AttachDispatch(m_lpDisp);

	if (m_lpDisp == NULL) 
		return _T("");
		
	int nComments = (int)m_objComments.get_Count();
	if(nComments <= 0) 
		return _T("");

	RangeSelect(nCol, nRow);
	
	m_lpDisp = m_objRange.get_Comment();
	if (m_lpDisp == NULL) 
		return _T("");

	m_objComment.AttachDispatch(m_lpDisp);
	return m_objComment.Text(covOptional, covOptional, covOptional);
}

// 메모지우기
INT CExportToHCell::ClearComments(INT nCol, INT nRow)
{
	RangeSelect(nCol, nRow);
	m_objRange.ClearComments();

	return 0;
}

// 영역설정
INT CExportToHCell::GetRange(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow)
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

		m_lpDisp = m_objSheet.get_Range(COleVariant(m_strLocation), COleVariant(strMax));
		if (m_lpDisp == NULL) 
			return -1;
		m_objRange.AttachDispatch(m_lpDisp);
	}

	return 0;
}

// 셀선택
INT CExportToHCell::RangeSelect(INT nMinCol, INT nMinRow, INT nMaxCol, INT nMaxRow)
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
		strMax.Format(_T("%c%d"), 'A' + nMinCol + nMaxCol-2, nMaxRow);

	// 영역설정
    m_lpDisp = m_objSheet.get_Range(COleVariant(m_strLocation), COleVariant(strMax));
	if (m_lpDisp == NULL) 
		return -1;

	m_objRange.AttachDispatch(m_lpDisp);
	m_objRange.Select();

	return 0;
}

INT CExportToHCell::RangeSelect(INT nCol, INT nRow)
{
	// A1~ IV65536 까지의 넣을 위치를 스트링으로 저장한다. 
	if ( _EXCEL_NALPHAMAX_ < nCol-1 )
		m_strLocation.Format(_T("%c%c%d"), 'A' + (nCol-1/_EXCEL_NALPHAMAX_), 'A' + (nCol-1%_EXCEL_NALPHAMAX_), nRow);
	else
		m_strLocation.Format(_T("%c%d"), 'A' + nCol-1, nRow);
	
	m_lpDisp = m_objSheet.get_Range(COleVariant(m_strLocation), COleVariant(m_strLocation));
	if (m_lpDisp == NULL) 
		return -1;
	
	m_objRange.AttachDispatch(m_lpDisp);
	m_objRange.Select();
	
	return 0;
}

INT CExportToHCell::RangeSelect(CString sValue)
{
	m_lpDisp = m_objSheet.get_Range(COleVariant(sValue), COleVariant(sValue));
	if (m_lpDisp == NULL) 
		return -1;

	m_objRange.AttachDispatch(m_lpDisp);
	m_objRange.Select();

	return 0;
}

// 모든 세션들을 닫아주고 디스패치를 릴리즈 해주는 곳.
INT CExportToHCell::Close()
{
	// 북을 닫아주고
	m_objBook.Close(covOptional, COleVariant(m_strFileName), covOptional); 
	m_objBooks.Close();
	
	// 릴리즈 디스패치를 해준다.
	m_objComment.ReleaseDispatch();
	m_objComments.ReleaseDispatch();	
	m_objRange.ReleaseDispatch();
	m_objSheet.ReleaseDispatch();
	m_objSheets.ReleaseDispatch();
	m_objBook.ReleaseDispatch();
	m_objBooks.ReleaseDispatch();
	
	// 어플리케이션 종료.
	m_app.Quit();
	m_app.ReleaseDispatch();	

	return 0;
}

INT CExportToHCell::Release()
{
	// 릴리즈 디스패치를 해준다.
	m_objComment.ReleaseDispatch();
	m_objComments.ReleaseDispatch();		
	m_objRange.ReleaseDispatch();
	m_objSheet.ReleaseDispatch();
	m_objSheets.ReleaseDispatch();
	m_objBook.ReleaseDispatch();
	m_objBooks.ReleaseDispatch();
	m_app.ReleaseDispatch();	
	return 0;
}

// 한셀을 화면에 띄워 보이게 해주는 함수.
INT CExportToHCell::VisableExcel(BOOL bValue)
{
	// 엑셀을 연다. 보이게 해준다.
	if(bValue)
		m_app.put_Visible(TRUE);
	else
		m_app.put_Visible(FALSE);

	return 0;
}