// ExcelExporter.cpp: implementation of the CExcelExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExcelExporter.h"
// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
#include "ExportToHCell.h"
//*/ END -------------------------------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcelExporter::CExcelExporter(CESL_Mgr * pMgr, CSIReportManager * pSIMgr, CESL_DataMgr * pDM, CString strRPTFileName)
{
	m_pParentMgr = pMgr;
	m_pSIReportMgr = pSIMgr;
	m_pDM = pDM;
	m_pWorkForm = NULL;
	m_strRPTFileName = strRPTFileName;

	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	m_bIsExcel = TRUE;
	CLSID clsExcelApp;
	if (!FAILED(CLSIDFromProgID(_T("HCell.Application"), &clsExcelApp))) 
	{
		m_bIsExcel = FALSE;	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	}	
//*/ END -------------------------------------------------------------------------------------
}

CExcelExporter::~CExcelExporter()
{
	if(m_pWorkForm)
	{
		delete m_pWorkForm;
		m_pWorkForm = NULL;
	}
}

INT CExcelExporter::ReadExcelExportConfigFile(short &title_size, short &title_bk, short &title_fk, bool &title_bold, bool &title_italic, short &data_size, short &data_bk, short &data_fk, bool &data_bold, bool &data_italic)
{
	CStdioFile fd;
	
	title_size = 9;
	title_bk = 0;		// White
	title_fk = 1;		// Black
	title_bold = FALSE;
	title_italic = FALSE;
	data_size = 9;
	data_bk = 0;		// White
	data_fk = 1;		// Black
	data_bold = FALSE;
	data_italic = FALSE;
	
	if (fd.Open(_T("..\\CFG\\GridExcelExport.CFG"), CStdioFile::modeRead | CFile::typeBinary) == FALSE) return 0;
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString strLine, strTag, strData;
	INT nPos;
	while(TRUE) {
		if (fd.ReadString(strLine) == FALSE) break;
		strLine.TrimLeft();			strLine.TrimRight();
		if (strLine.GetLength() == 0) continue;
		if (strLine.GetAt(0) == '#') continue;
		if (strLine.GetAt(0) == '.') continue;
		
		nPos = strLine.Find('=', 0);
		if (nPos == -1) continue;
		strTag = strLine.Mid(0, nPos);
		strTag.TrimLeft();			strTag.TrimRight();
		strTag.MakeUpper();
		strData = strLine.Mid(nPos+1);
		strData.TrimLeft();			strData.TrimRight();
		
		if (strTag.GetLength() == 0) continue;
		if (strData.GetLength() == 0) continue;
		
		if (strTag == _T("TITLE_FONT_SIZE")) title_size = _ttoi(strData);
		if (strTag == _T("TITLE_BK_COLOR")) title_bk = _ttoi(strData);
		if (strTag == _T("TITLE_FK_COLOR")) title_fk = _ttoi(strData);
		if (strTag == _T("TITLE_BOLD")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) title_bold = TRUE;
			else title_bold = FALSE;
		}
		if (strTag == _T("TITLE_ITALIC")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) title_italic = TRUE;
			else title_italic = FALSE;
		}
		
		if (strTag == _T("DATA_FONT_SIZE")) data_size = _ttoi(strData);
		if (strTag == _T("DATA_BK_COLOR")) data_bk = _ttoi(strData);
		if (strTag == _T("DATA_FK_COLOR")) data_fk = _ttoi(strData);
		if (strTag == _T("DATA_BOLD")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) data_bold = TRUE;
			else data_bold = FALSE;
		}
		if (strTag == _T("DATA_ITALIC")) {
			strData.MakeUpper();
			if (strData == _T("TRUE")) data_italic = TRUE;
			else data_italic = FALSE;
		}
	}
	
	fd.Close();
	
	return 0;
}

INT CExcelExporter::ExeclExport()
{	
	// Excel Export 설정을 읽어온다.
	short title_size, title_bk, title_fk;
	bool title_bold, title_italic;
	short data_size, data_bk, data_fk;
	bool data_bold, data_italic;
	ReadExcelExportConfigFile(title_size,title_bk,title_fk,title_bold,title_italic,data_size,data_bk,data_fk,data_bold,data_italic);

	TCHAR TmpDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,TmpDir);

	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH] = _T("");
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.lpstrFile = lpstrFile;
	OFN.hwndOwner = m_pParentMgr->m_hWnd;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_OVERWRITEPROMPT;

	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	if (m_bIsExcel != TRUE) 
	{
		OFN.lpstrFilter = _T("한셀파일(*.cell)\0*.cell\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("한셀 파일 저장");
	}	
	else
//*/ END -------------------------------------------------------------------------------------
	{
		OFN.lpstrFilter = _T("XLS파일(*.xls)\0*.xls\0모든파일(*.*)\0*.*\0");
		OFN.lpstrTitle = _T("EXCEL 파일 저장");
	}
	
	if (GetSaveFileName(&OFN) == 0)
	{
		SetCurrentDirectory(TmpDir);
        return 0;
	}
/*
	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
                    _T("EXCEL files(*.xls)|All files(*.*)|*.*|"), m_pParentMgr);
	if(dlg.DoModal() != IDOK) {
		//MessageBox(_T("Excel 출력이 취소되었습니다."), _T("Warnning!"), MB_ICONEXCLAMATION);
		SetCurrentDirectory(TmpDir);
        return 0;
	}
*/

	CRect rect;
	m_pParentMgr->GetClientRect(&rect);

	CRect DlgRect;
	DlgRect = rect;
	INT nWidth = DlgRect.Width();
	INT nHeight = DlgRect.Height();
	DlgRect.top = (nHeight - 50) / 2;
	DlgRect.bottom = DlgRect.top + 50;
	DlgRect.left = (nWidth - 175) / 2;
	DlgRect.right = DlgRect.left + 175;
	
	CESLDlg_ExcelExportWait WaitDlg(m_pParentMgr);
	WaitDlg.Create(IDD_DLG_MSG_EXCELEXPORTWAIT, m_pParentMgr);
	WaitDlg.MoveWindow( DlgRect );
	WaitDlg.CenterWindow(NULL);
	WaitDlg.ShowWindow(TRUE);
	
	WaitDlg.m_Progress1.SetPos(0);
/*
    CString sPathName = dlg.GetPathName();
    CString sPath(sPathName);
    CString sTitle(dlg.GetFileTitle());
    sPath = sPath.Left(sPath.GetLength() - dlg.GetFileName().GetLength());
*/

	// 16/03/29 김혜영 : 한셀 적용하기
//*/ ADD -------------------------------------------------------------------------------------
	// 16/04/05 김혜영 : 상호대차 배포를 위해 임시주석처리 -> // 16/04/19 김혜영 : 주석풀음
	if (m_bIsExcel != TRUE) 
	{
		WaitDlg.GetDlgItem(IDC_COM_EXCELEXPORT_STATIC)->SetWindowText(_T("한셀 파일로 저장합니다."));

		// COM 관련 DLL를 로딩해서 COM관련 명령어를 사용할 수 있도록 해준다
		CoInitialize(NULL);
		CExportToHCell *HCellMgr;
		HCellMgr = new CExportToHCell;				

		// 엑셀 관련 제어를 위한 초기화들을 수행한다.
		INT ids = HCellMgr->InitExcel();
		if( ids ) 
		{
			SetCurrentDirectory(TmpDir);
			HCellMgr->Release();
			delete HCellMgr;
			HCellMgr = NULL;				
			return -1;
		}
		
		WaitDlg.m_Progress1.SetPos(10);;		
		if(HCellMgr->Open() != 0) {
			SetCurrentDirectory(TmpDir);
			HCellMgr->Close();
			delete HCellMgr;
			HCellMgr = NULL;	
			return 0;
		}
		
		INT i, j;
		INT nRow = m_pDM->GetRowCount();
		INT nCol ;
		i = m_ArrHeader.GetSize();
		j = m_ArrDMAlias.GetSize();
		if(i == j)	nCol = i;
		else if(i > j) nCol = j;
		else if(i < j) nCol = i;

		HCellMgr->InitArray( nRow, nCol );

		CString strData;
		INT nAlignment, n_ExcelAlignment[256];
		for (i = 0; i < nCol; i++) {
			//INT nColumnSize = GetColWidth(i, 0);
			strData = m_ArrHeader.GetAt(i);
			strData.Replace(_T("\r\n"), _T(""));
			HCellMgr->SetTextToHCell(1, i+1, strData);
			HCellMgr->SetHorizAlign(i+1, 1, i+1, 1, -4108);	//(HCENTER: -4108)
			nAlignment = m_ArrAlignInfo.GetAt(i);
			n_ExcelAlignment[i] = 1;
			if (nAlignment == ALIGN_LEFT) n_ExcelAlignment[i] = -4131;
			else if (nAlignment == ALIGN_HCENTER) n_ExcelAlignment[i] = -4108;
			else if (nAlignment == ALIGN_RIGHT) n_ExcelAlignment[i] = -4152;
		}
		HCellMgr->SetFont(1, 1, nCol+1, 1, title_bold, title_italic, title_fk, title_size); 
		if (title_bk > 0) HCellMgr->SetBkColor(1, 1, nCol+1, 1, title_bk);
		WaitDlg.m_Progress1.SetPos(20);
		CStringArray ArrData;
		INT nProgress;
		for (i = 0; i < nRow; i++) {
			for (j = 0; j < nCol; j++) {
				strData = m_pDM->GetCellData(m_ArrDMAlias.GetAt(j), i);//GetTextMatrix(i, j);
				HCellMgr->FillSafeArray(i, j, strData);		
				HCellMgr->SetHorizAlign(j+1, i+2, j+1, i+2, n_ExcelAlignment[j]);
				//2005.11.23 황정영 : 목록의 소계/총계 라인은 색을 넣어준다.
				if(strData == _T("소계") || strData == _T("총계"))
					HCellMgr->SetBkColor(1, i+2, nCol, i+2, RGB(192,192,192));	
			}			
			nProgress = (INT)((70 * i) / (nRow>=1?nRow-1:1))+20;	//0나누기 오류수정 
			WaitDlg.m_Progress1.SetPos(nProgress);
		}
		HCellMgr->PutAllbyArray( 2, 1, nRow, nCol );		
		
		//2005.11.23 황정영 : 헤더라인에 색을 칠하고, 각 셀의 테두리선을 긋는다.
		HCellMgr->SetBkColor(1, 1, nCol, 1, RGB(192,192,192));						
		HCellMgr->SetBorder(1, 1, nCol, nRow+1, 7, 2);	// 테두리
		HCellMgr->SetFont(1, 2, 1, nRow+1, title_bold, title_italic, title_fk, title_size);

		if (title_bk > 0) HCellMgr->SetBkColor(1, 2, 1, nRow, title_bk);
		HCellMgr->SetFont(2, 2, nCol+1, nRow+1, data_bold, data_italic, data_fk, data_size);
		if (data_bk > 0) HCellMgr->SetBkColor(2, 2, nCol+1, nRow, data_bk);

		WaitDlg.m_Progress1.SetPos(90);

		SetCurrentDirectory(TmpDir);

		// 패스와 파일이름을 설정하여 저장한다.
		CString strPath = OFN.lpstrFile;
		HCellMgr->Save(strPath);
		HCellMgr->VisableExcel(TRUE);

		// 사용한 엑셀 객체를 반환한다.		
		HCellMgr->Release();
		delete HCellMgr;
		HCellMgr = NULL;
		WaitDlg.m_Progress1.SetPos(100);
	}
	else
//*/ END -------------------------------------------------------------------------------------
	{
		C_ExcelManControl m_ExcelCtrl;
		
		// Excel Manager OCX객체를		 연결한다.
		m_ExcelCtrl.Create(_T("Excel Control"), BS_DEFPUSHBUTTON, rect, m_pParentMgr, 8805);
		if (m_ExcelCtrl.m_hWnd == NULL) {
			SetCurrentDirectory(TmpDir);
			m_pParentMgr->ESLAfxMessageBox(_T("Excelmanager.ocx가 설치되어 있지 않습니다. 설치하여 주십시오."), MB_ICONEXCLAMATION);
			return 0;
		}
		WaitDlg.m_Progress1.SetPos(10);
		CExcelManager m_ExcelManager(&m_ExcelCtrl);
		if(m_ExcelManager.OpenManager() == FALSE) {
			SetCurrentDirectory(TmpDir);
			m_pParentMgr->ESLAfxMessageBox(_T("NOT Installed Excelmanager.ocx or No Excel!!!"), MB_ICONEXCLAMATION);
			return 0;
		}
		
		INT i, j;
		INT nRow = m_pDM->GetRowCount();
		INT nCol ;
		i = m_ArrHeader.GetSize();
		j = m_ArrDMAlias.GetSize();
		if(i == j)	nCol = i;
		else if(i > j) nCol = j;
		else if(i < j) nCol = i;

		CString strData;
		INT nAlignment, n_ExcelAlignment[256];
		for (i = 0; i < nCol; i++) {
			//INT nColumnSize = GetColWidth(i, 0);
			strData = m_ArrHeader.GetAt(i);
			strData.Replace(_T("\r\n"), _T(""));
			m_ExcelManager.SetTextMatrix(1, i+1, strData);
			m_ExcelManager.Horizontal_Alignment(i+1, 1, i+1, 1, 3);
					
			nAlignment = m_ArrAlignInfo.GetAt(i);
			n_ExcelAlignment[i] = 1;
			if (nAlignment == ALIGN_LEFT) n_ExcelAlignment[i] = 2;
			if (nAlignment == ALIGN_HCENTER) n_ExcelAlignment[i] = 3;
			if (nAlignment == ALIGN_RIGHT) n_ExcelAlignment[i] = 4;
		}
		m_ExcelManager.Font(1, 1, nCol+1, 1, title_bold, title_italic, title_fk, title_size);
		if (title_bk > 0) m_ExcelManager.Cell_Color(1, 1, nCol+1, 1, title_bk);
		WaitDlg.m_Progress1.SetPos(20);
		CStringArray ArrData;
		INT nProgress;
		for (i = 0; i < nRow; i++) {
			for (j = 0; j < nCol; j++) {
				strData = m_pDM->GetCellData(m_ArrDMAlias.GetAt(j), i);//GetTextMatrix(i, j);
				m_ExcelManager.SetTextMatrix(i+2, j+1, strData);
				m_ExcelManager.Horizontal_Alignment(j+1, i+2, j+1, i+2, n_ExcelAlignment[j]);
				//2005.11.23 황정영 : 목록의 소계/총계 라인은 색을 넣어준다.
				if(strData == _T("소계") || strData == _T("총계"))
					m_ExcelManager.Cell_Color(1, i+2, nCol, i+2, 15);
			}
			nProgress = (INT)((70 * i) / (nRow-1))+20;
			WaitDlg.m_Progress1.SetPos(nProgress);
		}
		
		//2005.11.23 황정영 : 헤더라인에 색을 칠하고, 각 셀의 테두리선을 긋는다.
		m_ExcelManager.Cell_Color(1, 1, nCol, 1, 15);
		//DrawLine(sX, xY, eX, eY, border_style, line_style, line_weight, color )
		m_ExcelManager.DrawLine(1,1,nCol,nRow+1, 1, 7, 2, data_bk);//왼쪽
		m_ExcelManager.DrawLine(1,1,nCol,nRow+1, 2, 7, 2, data_bk);//오른쪽
		m_ExcelManager.DrawLine(1,1,nCol,nRow+1, 3, 7, 2, data_bk);//위쪽
		m_ExcelManager.DrawLine(1,1,nCol,nRow+1, 4, 7, 2, data_bk);//아래쪽

		m_ExcelManager.Font(1, 2, 1, nRow+1, title_bold, title_italic, title_fk, title_size);
		if (title_bk > 0) m_ExcelManager.Cell_Color(1, 2, 1, nRow, title_bk);
		m_ExcelManager.Font(2, 2, nCol+1, nRow+1, data_bold, data_italic, data_fk, data_size);
		if (data_bk > 0) m_ExcelManager.Cell_Color(2, 2, nCol+1, nRow, data_bk);
		WaitDlg.m_Progress1.SetPos(90);

		// 패스와 파일이름을 설정하여 저장한다.
		//m_ExcelManager.SaveAs(sPath + sTitle + _T(".xls"));
		CString strPath = OFN.lpstrFile;
		strPath.Replace(_T(".xls"), _T(""));
		m_ExcelManager.SaveAs(strPath + _T(".xls"));

		// 사용한 엑셀 객체를 반환한다.
		m_ExcelManager.CloseManager();
		WaitDlg.m_Progress1.SetPos(100);	

		//m_pParentMgr->ESLAfxMessageBox(_T("EXCEL로 저장되었습니다."), MB_ICONEXCLAMATION);
		SetCurrentDirectory(TmpDir);
		ShellExecute(NULL, _T("open"), strPath+_T(".xls"), NULL, NULL, NULL);	
	}
	return 0;
}

INT CExcelExporter::LoadWorkForm()
{
	if(m_pSIReportMgr == NULL) return -1;
	if(m_pWorkForm)
	{
		delete m_pWorkForm;
		m_pWorkForm = NULL;
	}

	m_pWorkForm = new CSIReportWorkForm(MASTER_IS_MANAGER);

	CString tmp_filename = _T("rpt_tmp\\tmp_ExcelExportManager.rpt");
	CString PK;
	INT ids = m_pSIReportMgr->GetSIReportFileFromDB(m_strRPTFileName, tmp_filename, PK);
	if(ids < 0) return -1;
	ids = m_pWorkForm->LoadWorkForm(tmp_filename);
	if(ids < 0) return -1;
	return 0;
}

CSIReportObject_Table * CExcelExporter::FindRepeatTableObject()
{
	if(m_pWorkForm == NULL) return NULL;
	
	CSIReportWorkSheet *pSheet = m_pWorkForm->FindSheet(0);
	if(pSheet == NULL) return NULL;

	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return NULL;
	
	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) 
	{
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) 
		{
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (!pTable) continue;
			if (pTable->repeat == 1) return pTable;
		}
	}
	return NULL;
}

CSIReportObject_Table * CExcelExporter::FindTempleteTableObject()
{
	if(m_pWorkForm == NULL) return NULL;
	
	CSIReportWorkSheet *pSheet = m_pWorkForm->FindSheet(0);
	if(pSheet == NULL) return NULL;
	
	INT count = pSheet->ObjList.GetCount();
	if (count == 0) return NULL;
	
	CSIReportObject * pObject;
	POSITION pos;
	INT i;
	pos = pSheet->ObjList.GetHeadPosition();
	for(i = 0; i < count; i++) 
	{
		pObject = NULL;
		pObject = (CSIReportObject*)pSheet->ObjList.GetNext(pos);
		if (!pObject) continue;
		if (pObject->GetObjType() == OBJECT_TABLE) 
		{
			CSIReportObject_Table *pTable = (CSIReportObject_Table *)pObject;
			if (!pTable) continue;
			if (pTable->repeat == 1) continue;
			if(pTable->CellList.GetCount() > 4)
				return pTable;
		}
	}
	return NULL;
}

INT	CExcelExporter::SetRPTLinkedHeaderInfo()
{
	m_ArrHeader.RemoveAll();
	
	CSIReportObject_Table_Cell * pCell = NULL;
	CSIReportObject_Table * pTable = FindTempleteTableObject();
	if(pTable == NULL) return -1;
	CSIReportObject_Text * pText = NULL;

	CString strTmpData;
	POSITION pos = pTable->CellList.GetHeadPosition();
	while(pos)
	{
		pCell = (CSIReportObject_Table_Cell*)pTable->CellList.GetNext(pos);
		if(pCell == NULL) continue;
		strTmpData = pCell->text.text;
		m_ArrHeader.Add(strTmpData);
	}
	
	return 0;	
}

INT	CExcelExporter::SetRPTLinkedDMInfo()
{
	m_ArrDMAlias.RemoveAll();
	m_ArrAlignInfo.RemoveAll();

	CSIReportObject_Table_Cell * pCell = NULL;
	CSIReportObject_Table * pTable = FindRepeatTableObject();
	if(pTable == NULL) return -1;

	CString strTmpData;
	POSITION pos = pTable->CellList.GetHeadPosition();
	while(pos)
	{
		pCell = (CSIReportObject_Table_Cell*)pTable->CellList.GetNext(pos);
		if(pCell == NULL) continue;
		strTmpData = pCell->data_field;
		m_ArrDMAlias.Add(strTmpData);
		m_ArrAlignInfo.Add(pCell->horz_align);
	}

	return 0;	
}

INT CExcelExporter::InitExcelExporter(CString strRPTFileName/*=_T("")*/)
{
	if(m_pParentMgr == NULL || m_pSIReportMgr == NULL || m_pDM == NULL)
		return -1;
	if(!strRPTFileName.IsEmpty()) m_strRPTFileName = strRPTFileName;

	if(m_strRPTFileName.IsEmpty()) return -1;
	//SIReportManager에 Connection Info 설정		
	m_pSIReportMgr->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	//WorkForm로딩
	INT ids = LoadWorkForm();
	if(ids < 0) return -1;
	//헤더정보 추출
	ids = SetRPTLinkedHeaderInfo();
	if(ids < 0) return -1;
	//DM링크/셀 정렬정보 추출
	ids = SetRPTLinkedDMInfo();
	if(ids < 0) return -1;
	return 0;
}
