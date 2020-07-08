// MSheetDesigner.cpp : implementation file
//

#include "stdafx.h"
#include "SIReportEditor.h"
#include "MSheetDesigner.h"

#include "MemDC.h"

#include "SIReportEditorView.h"

#include "DlgTableProperty.h"
#include "DlgColumnChange.h"

#include "ESL_DataMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "efs.h"

/////////////////////////////////////////////////////////////////////////////
// CMSheetDesigner

CMSheetDesigner::CMSheetDesigner(CSIReportXMLIOMgr* pIOMgr)
{
	m_pIOMgr = pIOMgr;

	m_bIsReadOnly = FALSE;
	m_bDoubleBuffering = TRUE;

	m_bHistory = TRUE;
	// rect tracker
	m_nBmpIdx = 0;

	m_rectTracker.m_rect = CRect(0, 0, 0, 0);
	m_rectTracker.m_nStyle = CRectTracker::dottedLine | CRectTracker::resizeOutside;

	m_sizePickPoint = CSize(10, 10);

	m_nResizeDir = 0;

	m_MouseMode = MOUSE_NOTHING;
	m_szMouseCursor = IDC_ARROW;

	m_pWorkSheet = NULL;
	m_pDlgProperty = NULL;

	m_pEditWnd = NULL;

	m_nZoomMode = 0;
}

INT CMSheetDesigner::InitMSheetDesinger(CSIReportWorkSheet *pWorSheet, BOOL bIsReadOnly /* = FALSE */)
{
EFS_BEGIN

	if (!pWorSheet)
	{
		AfxMessageBox(_T("Initalize worksheet error!!"));
		return -1;
	}

	FreeUndoList();
	FreeRedoList();
	m_listSelectedObject.RemoveAll();
	
	m_pWorkSheet = pWorSheet;

	SetReadOnly(bIsReadOnly);
	
	Invalidate();
	UpdateWindow();
	return 0;

EFS_END
return -1;

}

CMSheetDesigner::~CMSheetDesigner()
{
	FreeSelectedObjectList();
	FreeUndoList();
	FreeRedoList();

	if (m_pDlgProperty != NULL)
	{
		delete m_pDlgProperty;
		m_pDlgProperty = NULL;
	}

	if (m_pEditWnd != NULL)
	{
		delete m_pEditWnd;
		m_pEditWnd = NULL;
	}
}

VOID CMSheetDesigner::FreeUndoList()
{
EFS_BEGIN

	INT nCount = m_listUndo.GetCount();
	if (nCount == 0) return;
	CSIReportWorkSheet* pSheet;
	for (INT i = 0; i < nCount; i++) 
	{
		pSheet = (CSIReportWorkSheet*)m_listUndo.RemoveHead();
		if (pSheet) delete pSheet;
	}
	m_listUndo.RemoveAll();
	return;

EFS_END
}

VOID CMSheetDesigner::FreeRedoList()
{
EFS_BEGIN

	INT nCount = m_listRedo.GetCount();
	if (nCount == 0) return;
	CSIReportWorkSheet* pSheet;
	for (INT i = 0; i < nCount; i++) 
	{
		pSheet = (CSIReportWorkSheet*)m_listRedo.RemoveHead();
		if (pSheet) delete pSheet;
	}
	m_listRedo.RemoveAll();
	return;

EFS_END
}

VOID CMSheetDesigner::FreeSelectedObjectList()
{
EFS_BEGIN

	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while(pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
			((CSIReportObject_Table*)pObject)->ReSetCellFocus();
	}

	m_listSelectedObject.RemoveAll();
	return;

EFS_END
}

BEGIN_MESSAGE_MAP(CMSheetDesigner, CWnd)
	//{{AFX_MSG_MAP(CMSheetDesigner)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_EN_KILLFOCUS( ID_eCELLTEXT, OnEditLostFocus )
	ON_EN_CHANGE( ID_eCELLTEXT, OnEditChange )	
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(ID_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMSheetDesigner message handlers

BOOL CMSheetDesigner::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

EFS_END
return FALSE;

}

INT CMSheetDesigner::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
EFS_BEGIN

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_oleDropTarget.Register(this))
	{
		AfxMessageBox(_T("Ole Register Drop Target Failed"));
	}
		
	m_nOleFormat = RegisterClipboardFormat(_T("SIREPORT_EDITOR"));

	// create property dialog
	m_pDlgProperty = new CDlgProperty;
	if (!m_pDlgProperty->Create(this))
	{
		TRACE0("Failed to create propertyList\n");
		return FALSE;      // fail to create
	}
	CRect frmRect;
	CRect dlgRect;

	GetParentFrame()->GetWindowRect(frmRect);
	m_pDlgProperty->GetWindowRect(dlgRect);
	m_pDlgProperty->ShowWindow(SW_HIDE);
	m_pDlgProperty->MoveWindow(frmRect.right - dlgRect.Width(), frmRect.bottom - dlgRect.Height(), dlgRect.Width() + 1, dlgRect.Height());

	// create dynamic edit box
	m_pEditWnd = new CEdit();
	m_pEditWnd->Create(WS_CHILD | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_MULTILINE  , CRect(1, 1, 1, 1), this, ID_eCELLTEXT);
	
	return 0;

EFS_END
return -1;

}

/////////////////////////////////////////////////////////////////////////////
// Main Operations

VOID CMSheetDesigner::AddObject(INT nObjType, POINT point)
{
EFS_BEGIN

	// Check ReadOnly
	if (m_bIsReadOnly) return;

	CPaintDC dc(this);
	GetIsotropicDC(dc);
	dc.DPtoLP(&point);

	CRect rect;
	rect.left = point.x;
	rect.top = point.y;
	rect.right = rect.left + 100;
	rect.bottom = rect.top + 100;

	CSIReportObject *pObject = NULL;
	
	// create object by object type
	if (nObjType == OBJECT_TABLE)
	{
		CDlgTableProperty dlg;
		if (dlg.DoModal() == IDOK)
		{
			pObject = new CSIReportObject_Table;
			pObject->SetRect(rect);
			pObject->m_strName = dlg.m_strName;
			
			((CSIReportObject_Table*)pObject)->visible = dlg.visible;
			((CSIReportObject_Table*)pObject)->repeat = dlg.repeat;
			((CSIReportObject_Table*)pObject)->part_sum = dlg.part_sum;
			((CSIReportObject_Table*)pObject)->total_sum = dlg.total_sum;
			((CSIReportObject_Table*)pObject)->horz_line = dlg.horz_line;
			((CSIReportObject_Table*)pObject)->vert_line = dlg.vert_line;
			((CSIReportObject_Table*)pObject)->InitCell(dlg.row_count, dlg.col_count);
		}
	}
	else if (nObjType == OBJECT_TEXT_BOX)
	{
		pObject = new CSIReportObject_Text_Box;
	}
	else if (nObjType == OBJECT_LINE)
	{
		pObject = new CSIReportObject_Line;
	}
	else if (nObjType == OBJECT_BARCODE)
	{
		pObject = new CSIReportObject_Barcode;
	}
	else if (nObjType == OBJECT_BITMAP)
	{
		pObject = new CSIReportObject_Bitmap;
		
		CString strTemp;
		strTemp.Format(_T("BITMAP_%03d"), ++m_nBmpIdx);
		pObject->m_strName = strTemp;		
	}
	
	// add object to objectlist
	if(pObject != NULL) 
	{
		EditSaveHistory();

		pObject->Create(rect, this);
		m_pWorkSheet->ObjList.AddTail((CObject*)pObject);
	}

EFS_END
}

VOID CMSheetDesigner::AddSelectObject(CObject* pObject)
{
EFS_BEGIN

	if (m_bIsReadOnly) return;
	m_listSelectedObject.AddTail(pObject);

EFS_END
}

VOID CMSheetDesigner::AddSelectObject(CObList* pList)
{
EFS_BEGIN

	if (m_bIsReadOnly) return;
	m_listSelectedObject.AddTail(pList);

EFS_END
}

VOID CMSheetDesigner::DeleteObject()
{
EFS_BEGIN

	EditSaveHistory();
	
	POSITION posSelected = m_listSelectedObject.GetHeadPosition();
	while (posSelected != NULL)
	{
		CSIReportObject *pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetNext(posSelected);
		if (pSelectedObject == NULL) continue;
		
		POSITION posObject = m_pWorkSheet->ObjList.GetTailPosition();
		while (posObject != NULL)
		{
			POSITION oldPos = posObject;
			CSIReportObject *pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetPrev(posObject);
			if (pObject != NULL && pSelectedObject == pObject)
			{
				// Modify Parent Information
				if (pObject->GetParentObject() != NULL && !pObject->m_strParentName.IsEmpty())
				{
					if (GetObject(pObject->m_strParentName) != NULL)
					{	
						CSIReportObject_Table_Cell *pCell = FindParentCell(pObject->m_strParentName, pObject->GetName());
						//CSIReportObject_Table_Cell *pCell = (CSIReportObject_Table_Cell*)pObject->GetParentObject();
						if (pCell != NULL)
						{
							pCell->child_name[0] = _T("");
							pCell->child_count--;
						}
					}
					else
					{
						pObject->SetParentObject(NULL);
						pObject->m_strParentName = _T("");
					}
				}
				// Modify Child Information
				else if (pObject->GetObjType() == OBJECT_TABLE)
				{
					CSIReportObject_Table *pTable = FindChildTable((CSIReportObject_Table*)pObject);
					if (pTable != NULL)
					{
						pTable->SetParentName(_T(""));
						pTable->SetParentObject(NULL);
					}
				}
				delete pObject;
				pObject = NULL;
				m_pWorkSheet->ObjList.RemoveAt(oldPos);
				break;
			}
		}
	}

	m_listSelectedObject.RemoveAll();
	
	ShowPropertyWindow();
	Invalidate();
	UpdateWindow();

EFS_END
}


CSIReportObject *CMSheetDesigner::GetObject(CPoint point, BOOL bReverse /* = TRUE */)
{
EFS_BEGIN

	POSITION pos;
	if (bReverse) 
		pos = m_pWorkSheet->ObjList.GetTailPosition();
	else 
		pos = m_pWorkSheet->ObjList.GetHeadPosition();
	
	while (pos)
	{
		CSIReportObject *pObject;
		if (bReverse)
			pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetPrev(pos);
		else
			pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);

		if (pObject != NULL && pObject->PtInSIReportObject(point))
			return pObject;
	}

	return NULL;

EFS_END
return NULL;

}

CSIReportObject *CMSheetDesigner::GetObject(CString strObjectName)
{
EFS_BEGIN

	POSITION pos = m_pWorkSheet->ObjList.GetHeadPosition();
	while (pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);
		if (pObject && pObject->m_strName == strObjectName)
			return pObject;
	}
	return NULL;

EFS_END
return NULL;

}

VOID CMSheetDesigner::ShowPropertyWindow()
{
EFS_BEGIN

	// show property window when one object selected
	if (m_listSelectedObject.GetCount() != 1) 
	{
		m_pDlgProperty->ShowWindow(SW_HIDE);
		return;
	}
	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject == NULL) return;
	
	CString strTemp;
	m_pDlgProperty->Init();

	// LINE PROPERTY
	if (pObject->GetObjType() == OBJECT_LINE)
	{
		CSIReportObject_Line* pLine = (CSIReportObject_Line*)pObject;

		m_pDlgProperty->SetWindowText(_T("LINE PROPERTY"));
		m_pDlgProperty->AddTabItem(_T("LINE"));
		m_pDlgProperty->SetTabFocus(0);
		
		m_pDlgProperty->AddProperty(_T("LINE"), _T("라인 이름 : "), _T("m_strName"), ID_PROPERTY_TEXT, pLine->m_strName);
		strTemp.Format(_T("%d"), pLine->thick);
		m_pDlgProperty->AddProperty(_T("LINE"), _T("선 굵기 : "), _T("thick"), ID_PROPERTY_TEXT, strTemp);
		m_pDlgProperty->AddProperty(_T("LINE"), _T("선 스타일 : "), _T("style"), ID_PROPERTY_COMBO_LIST, _T("PS_SOLID!PS_DASH!PS_DOT!PS_DASHDOTDOT!PS_NULL!PS_INSIDEFRAME"), pLine->style);

		m_pDlgProperty->ShowWindow(SW_SHOW);

	}
	// TEXT BOX
	else if (pObject->GetObjType() == OBJECT_TEXT_BOX)
	{
		CSIReportObject_Text_Box* pTextBox = (CSIReportObject_Text_Box*)pObject;

		m_pDlgProperty->SetWindowText(_T("TEXT BOX PROPERTY"));
		m_pDlgProperty->AddTabItem(_T("TEXT_BOX"));
		m_pDlgProperty->SetTabFocus(0);

		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("텍스트 박스 이름 : "), _T("m_strName"), ID_PROPERTY_TEXT, pTextBox->m_strName);

		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("수평선 보여주기 : "), _T("horz_line"), ID_PROPERTY_BOOL, _T("FALSE!TRUE"), pTextBox->horz_line);
		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("수직선 보여주기: "), _T("vert_line"), ID_PROPERTY_BOOL, _T("FALSE!TRUE"), pTextBox->vert_line);
		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("첫페이지만 보이기: "), _T("m_nOnlyShowFirstPage"), ID_PROPERTY_BOOL, _T("FALSE!TRUE"), pTextBox->m_nOnlyShowFirstPage);
		
		strTemp.Format(_T("%d"), pTextBox->indent_len);
		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("여백 : "), _T("indent_len"), ID_PROPERTY_TEXT, strTemp);
		
		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("가로 맞춤: "), _T("horz_align"), ID_PROPERTY_COMBO_LIST, _T("왼쪽 맞춤!가운데 맞춤!오른쪽 맞춤"), pTextBox->horz_align);
		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("세로 맞춤: "), _T("vert_align"), ID_PROPERTY_COMBO_LIST, _T("위쪽 맞춤!세로 가운데 맞춤!아래쪽 맞춤"), pTextBox->vert_align);
		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("폰트: "), _T("font"), pTextBox->text.font, pTextBox->text.color);
		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("배경색: "), _T("fill_color"), pTextBox->fill_color);
		m_pDlgProperty->AddProperty(_T("TEXT_BOX"), _T("텍스트: "), _T("text"), ID_PROPERTY_TEXT_MULTI, pTextBox->text.text);

		m_pDlgProperty->ShowWindow(SW_SHOW);

	}
	// BITAMP
	else if (pObject->GetObjType() == OBJECT_BITMAP)
	{
		CSIReportObject_Bitmap* pBitmap = (CSIReportObject_Bitmap*)pObject;

		m_pDlgProperty->SetWindowText(_T("BITMAP PROPERTY"));
		m_pDlgProperty->AddTabItem(_T("BITMAP"));
		m_pDlgProperty->SetTabFocus(0);
		
		strTemp.Format(_T("%s!Bitmap Files (*.BMP)|*.bmp|All Files (*.*)|*.*|"), pBitmap->file_name == _T("") ? _T("*.BMP") : pBitmap->file_name);

		m_pDlgProperty->AddProperty(_T("BITMAP"), _T("비트맵 이름 : "), _T("m_strName"), ID_PROPERTY_STATIC, pBitmap->m_strName);
		m_pDlgProperty->AddProperty(_T("BITMAP"), _T("BITMAP 파일경로: "), _T("file_name"), ID_PROPERTY_PATH, strTemp);

		m_pDlgProperty->ShowWindow(SW_SHOW);
	}
	// BARCODE
	else if (pObject->GetObjType() == OBJECT_BARCODE)
	{
		CSIReportObject_Barcode* pBarcode = (CSIReportObject_Barcode*)pObject;
		m_pDlgProperty->SetWindowText(_T("BARCODE PROPERTY"));
		m_pDlgProperty->AddTabItem(_T("BARCODE"));
		m_pDlgProperty->SetTabFocus(0);
		
		m_pDlgProperty->AddProperty(_T("BARCODE"), _T("바코드 이름: "), _T("m_strName"), ID_PROPERTY_TEXT, pBarcode->m_strName);
		
		CString strClass;
		INT i = 0;
		for (i = 0; i < pBarcode->m_nStyles - 1; i++)
		{
			strClass += pBarcode->m_strStyles[i][2] + _T("!");
		}
		strClass += pBarcode->m_strStyles[i][3];

		m_pDlgProperty->AddProperty(_T("BARCODE"), _T("코드 값: "), _T("m_strCaption"), ID_PROPERTY_TEXT, pBarcode->m_strCaption);
		m_pDlgProperty->AddProperty(_T("BARCODE"), _T("클래스: "), _T("m_nStyle"), ID_PROPERTY_COMBO_LIST, strClass, pBarcode->m_nStyle - 1);

		m_pDlgProperty->ShowWindow(SW_SHOW);
	}
	// TABLE PROPERTY
	else if (pObject->GetObjType() == OBJECT_TABLE)
	{
		
		// set table property
		CSIReportObject_Table *pTable = ((CSIReportObject_Table*)pObject);
		
		// cell
		CSIReportObject_Table_Cell *pCell = pTable->GetFocusCell();
	
		// property list test...
		if (m_pDlgProperty)
		{
			m_pDlgProperty->SetWindowText(_T("TABLE PROPERTY"));
			m_pDlgProperty->AddTabItem(_T("TABLE"));
			m_pDlgProperty->SetTabFocus(0);
			
			
			// Table property
			m_pDlgProperty->AddProperty(_T("TABLE"), _T("테이블 이름 : "), _T("m_strName"), ID_PROPERTY_TEXT, pTable->m_strName);
			
			strTemp.Format(_T("%d"), pTable->row_count);
			m_pDlgProperty->AddProperty(_T("TABLE"), _T("행 : "), _T("row_count"), ID_PROPERTY_STATIC, strTemp);
			strTemp.Format(_T("%d"), pTable->col_count);
			m_pDlgProperty->AddProperty(_T("TABLE"), _T("열 : "), _T("row_count"), ID_PROPERTY_STATIC, strTemp);
			
			m_pDlgProperty->AddProperty(_T("TABLE"), _T("소계 : "), _T("part_sum"), ID_PROPERTY_BOOL, _T("FALSE!TRUE"), pTable->part_sum);
			m_pDlgProperty->AddProperty(_T("TABLE"), _T("합계 : "), _T("total_sum"), ID_PROPERTY_BOOL, _T("FALSE!TRUE"), pTable->total_sum);
			m_pDlgProperty->AddProperty(_T("TABLE"), _T("반복 : "), _T("repeat"), ID_PROPERTY_BOOL, _T("FALSE!TRUE"), pTable->repeat);
			m_pDlgProperty->AddProperty(_T("TABLE"), _T("수평선 보여주기 : "), _T("horz_line"), ID_PROPERTY_BOOL, _T("FALSE!TRUE"), pTable->horz_line);
			m_pDlgProperty->AddProperty(_T("TABLE"), _T("수직선 보여주기: "), _T("vert_line"), ID_PROPERTY_BOOL, _T("FALSE!TRUE"), pTable->vert_line);
			
			// Cell property
			if (pCell != NULL)
			{
				m_pDlgProperty->AddTabItem(_T("CELL"));
				m_pDlgProperty->SetTabFocus(1);
				
				if (pCell->data_mgr_name == _T(""))
				{
					pCell->data_mgr_name = pTable->GetCell(0, 0)->data_mgr_name;
					if (pCell->data_mgr_name == _T(""))
					{
						pCell->data_mgr_name = GetWorkSheet()->m_strDefaultDMAlias;
					}
				}
				
				INT nDMMode;
				if (((CSIReportEditorView*)GetParent())->m_nEditorMode == USER_NORMAL)
					nDMMode = ID_PROPERTY_STATIC;
				else
					nDMMode = ID_PROPERTY_TEXT;


				m_pDlgProperty->AddProperty(_T("CELL"), _T("데이터 메니저이름 : "), _T("data_mgr_name"), nDMMode, pCell->data_mgr_name);
				

				// Get Data Field List from DB
				CString strDataFieldList;
				//INT nSelectedDataField = GetDataFieldList(pCell->data_mgr_name, pCell->data_field, strDataFieldList);
				INT nSelectedDataField = GetDataFieldList(pCell->data_mgr_name, pCell->data_field_user, strDataFieldList);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("데이터 필드  이름 : "), _T("data_field_user"), ID_PROPERTY_COMBO_LIST, strDataFieldList, nSelectedDataField);
				
				m_pDlgProperty->AddProperty(_T("CELL"), _T("소계필드 : "), _T("part_sum"), ID_PROPERTY_COMBO_LIST, _T("없음!정수!실수"), pCell->part_sum);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("합계필드: "), _T("total_sum"), ID_PROPERTY_COMBO_LIST, _T("없음!정수!실수"), pCell->total_sum);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("사선: "), _T("slash"), ID_PROPERTY_COMBO_LIST, _T("없음!＼!／"), pCell->slash);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("폰트: "), _T("font"), pCell->text.font, pCell->text.color);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("배경색: "), _T("fill_color"), pCell->fill_color);

				strTemp.Format(_T("%d"), pCell->char_len);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("라인당 글자수 : "), _T("char_len"), ID_PROPERTY_TEXT, strTemp);
				strTemp.Format(_T("%d"), pCell->char_max);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("최대 글자수 : "), _T("char_max"), ID_PROPERTY_TEXT, strTemp);
				strTemp.Format(_T("%d"), pCell->indent_len);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("여백 : "), _T("indent_len"), ID_PROPERTY_TEXT, strTemp);

				m_pDlgProperty->AddProperty(_T("CELL"), _T("가로 맞춤: "), _T("horz_align"), ID_PROPERTY_COMBO_LIST, _T("왼쪽 맞춤!가운데 맞춤!오른쪽 맞춤"), pCell->horz_align);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("세로 맞춤: "), _T("vert_align"), ID_PROPERTY_COMBO_LIST, _T("위쪽 맞춤!세로 가운데 맞춤!아래쪽 맞춤"), pCell->vert_align);
				INT nOrientation;
				
				if (pCell->orientation == 1)
					nOrientation = 0;
				else if (pCell->orientation == -1)
					nOrientation = 1;
					
				m_pDlgProperty->AddProperty(_T("CELL"), _T("텍스트 방향 : "), _T("orientation"), ID_PROPERTY_COMBO_LIST, _T("가로방향!세로방향"), nOrientation);
				m_pDlgProperty->AddProperty(_T("CELL"), _T("텍스트: "), _T("text"), ID_PROPERTY_TEXT_MULTI, pCell->text.text);

			}

			m_pDlgProperty->ShowWindow(SW_SHOW);
		}
	}
	else
		m_pDlgProperty->ShowWindow(SW_HIDE);

	SetFocus();

EFS_END
}


INT	 CMSheetDesigner::GetDataFieldList(CString strDM, CString strDataField, CString &strDataFieldList)
{
	if (strDM.IsEmpty())
	{
		strDataFieldList = _T(" ");
		return 0;
	}

	INT nResult = 0;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(((CSIReportEditorView*)GetParent())->m_ReadCfgFile.m_sConnection);
		
	CStdioFile fd;
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\REFERENCE\\DM\\%s.UDM"), strDM);
	if (fd.Open(strFileName, CStdioFile::modeRead | CFile::typeBinary) == FALSE) return 0;

	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString strLine, strTemp;
	INT nPos;
	strDataFieldList = _T(" ");
	INT nLineCount = 0;
	for (INT i = 0; TRUE; i++)
	{
		if (fd.ReadString(strLine) == FALSE) break;
		strLine.TrimLeft();				strLine.TrimRight();
		if (strLine.GetLength() == 0) continue;
		if (strLine.GetAt(0) == '#') continue;
		nPos = strLine.Find('|', 0);
		if (nPos == -1) continue;

		strTemp = strLine.Mid(0, nPos);
		if (strTemp.IsEmpty()) continue;

		nLineCount++;

		strDataFieldList += _T("!") + strTemp;
		if (strTemp == strDataField)
			nResult = nLineCount;
	}
	fd.Close();

	return nResult;
}

/////////////////////////////////////////////////////////////////////////////
// Edit

VOID CMSheetDesigner::EditProc(UINT nChar)
{
EFS_BEGIN

	// determine edit procedure
	
	// count selected object list;

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount == 0) return;

	if (nCount == 1) 
		EditResizeObject(nChar);
	else
		EditAlignObject(nChar);


EFS_END
}

VOID CMSheetDesigner::EditMoveObject(UINT nChar)
{
EFS_BEGIN

	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportObject* pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;

		CRect rect = pObject->GetRect();

		switch(nChar)
		{
		case VK_LEFT:
			rect.left--;
			rect.right--;
			break;
		case VK_RIGHT:
			rect.left++;
			rect.right++;
			break;
		case VK_UP:
			rect.top--;
			rect.bottom--;
			break;
		case VK_DOWN:
			rect.top++;
			rect.bottom++;
			break;
		}
				
		// Table
		if (pObject->GetObjType() == OBJECT_TABLE)
		{
			((CSIReportObject_Table*)pObject)->MoveCell(nChar);
			ShowPropertyWindow();
		}

		pObject->SetRect(rect);
	}

	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditResizeObject(UINT nChar)
{
EFS_BEGIN

	// check select object count == 1
	if (m_listSelectedObject.GetCount() != 1) 
		return;
	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	if (pObject == NULL) return;
	if (pObject->GetObjType() == OBJECT_TABLE) 
	{
		((CSIReportObject_Table*)pObject)->ResizeCell(nChar);
	}
	else
	{
		CRect rect = pObject->GetRect();

		switch(nChar)
		{
		case VK_LEFT:
			rect.right--;
			break;
		case VK_RIGHT:
			rect.right++;
			break;
		case VK_UP:
			rect.bottom--;
			break;
		case VK_DOWN:
			rect.bottom++;
			break;
		}
		
		pObject->SetRect(rect);
	}
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditAlignObject(UINT nChar)
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount == 0) return;

	CSIReportObject *pStandardObject = (CSIReportObject*)m_listSelectedObject.GetAt(m_listSelectedObject.FindIndex(0));
	
	CRect rectStandard = pStandardObject->GetRect();

	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportObject* pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;

		CRect rect = pObject->GetRect();
		INT nPixcel = 0;

		switch(nChar)
		{
		case VK_LEFT:
			nPixcel = rect.left - rectStandard.left;
			rect.left -= nPixcel;
			rect.right -= nPixcel;
			break;
		case VK_UP:
			nPixcel = rect.top - rectStandard.top;
			rect.top -= nPixcel;
			rect.bottom -= nPixcel;
			break;
		case VK_RIGHT:
			nPixcel = rectStandard.right - rect.right;
			rect.left += nPixcel;
			rect.right += nPixcel;
			break;
		case VK_DOWN:
			nPixcel = rectStandard.bottom - rect.bottom;
			rect.top += nPixcel;
			rect.bottom += nPixcel;
			break;
		}

		// Table
		if (pObject->GetObjType() == OBJECT_TABLE)
		{
			((CSIReportObject_Table*)pObject)->MoveCell(nChar, nPixcel);
			//ShowPropertyWindow();
		}

		pObject->SetRect(rect);
	}

	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditCenterObject(UINT nChar)
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount == 0) return;

	CRect rectClient;
	GetClientRect(rectClient);

	CPaintDC dc(this);
	GetIsotropicDC(dc);

	dc.DPtoLP(rectClient);
	
	CRect rect;
	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportObject* pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;

		CRect rectObject = pObject->GetRect();

		rect.UnionRect(rect, rectObject);
	}

	INT nVertMove = (rectClient.top + (rectClient.Height() / 2)) - ((rect.top + rect.Height() / 2));
	INT nHorzMove = (rectClient.left + (rectClient.Width() / 2)) - ((rect.left + rect.Width() / 2));


	INT nPixcel = 0;
	pos = m_listSelectedObject.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportObject* pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;

		CRect rectObject = pObject->GetRect();
		
		if (GetKeyState(VK_SHIFT) & 0x8000 || nChar == VK_SHIFT)
		{
			rectObject.right += nHorzMove;
			rectObject.left += nHorzMove;

			// Table
			if (pObject->GetObjType() == OBJECT_TABLE)
			{
				((CSIReportObject_Table*)pObject)->MoveCell(VK_RIGHT, nHorzMove);
				//ShowPropertyWindow();
			}

		}
		else
		{
			rectObject.top += nVertMove;
			rectObject.bottom += nVertMove;

			// Table
			if (pObject->GetObjType() == OBJECT_TABLE)
			{
				((CSIReportObject_Table*)pObject)->MoveCell(VK_DOWN, nVertMove);
				//ShowPropertyWindow();
			}
		}

		pObject->SetRect(rectObject);
	}

	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditMakeSameSize(INT nMode)
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount == 0) return;

	CSIReportObject *pStandardObject = (CSIReportObject*)m_listSelectedObject.GetAt(m_listSelectedObject.FindIndex(0));
	
	CRect rectStandard = pStandardObject->GetRect();

	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportObject* pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;

		CRect rect = pObject->GetRect();

		if (nMode == MAKE_SAME_SIZE || nMode == MAKE_SAME_WIDTH)
			rect.right = rect.left + rectStandard.Width();
		if (nMode == MAKE_SAME_SIZE || nMode == MAKE_SAME_HEIGHT)
			rect.bottom = rect.top + rectStandard.Height();

		// Table
		if (pObject->GetObjType() == OBJECT_TABLE)
			((CSIReportObject_Table*)pObject)->SetCellRect(rect);
		
		pObject->SetRect(rect);
	}

	Invalidate();
	UpdateWindow();	

EFS_END
}

VOID CMSheetDesigner::EditBringToFront()
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount != 1) return;
	
	CSIReportObject *pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetAt(m_listSelectedObject.FindIndex(0));
	if (pSelectedObject == NULL) return;

	POSITION pos = m_pWorkSheet->ObjList.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION oldPos = pos;
		CSIReportObject* pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);
		if (pObject != NULL && pObject == pSelectedObject)
		{
			m_pWorkSheet->ObjList.RemoveAt(oldPos);
			m_pWorkSheet->ObjList.AddTail(pSelectedObject);
			break;
		}
	}

	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditSendToBack()
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount != 1) return;
	
	CSIReportObject *pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetAt(m_listSelectedObject.FindIndex(0));
	if (pSelectedObject == NULL) return;

	POSITION pos = m_pWorkSheet->ObjList.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION oldPos = pos;
		CSIReportObject* pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);
		if (pObject != NULL && pObject == pSelectedObject)
		{
			m_pWorkSheet->ObjList.RemoveAt(oldPos);
			m_pWorkSheet->ObjList.AddHead(pSelectedObject);
			break;
		}
	}

	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditBringForward()
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount != 1) return;
	
	CSIReportObject *pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetAt(m_listSelectedObject.FindIndex(0));
	if (pSelectedObject == NULL) return;

	POSITION pos = m_pWorkSheet->ObjList.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION oldPos = pos;
		CSIReportObject* pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);
		if (pObject != NULL && pObject == pSelectedObject)
		{
			if (pos == NULL) break;

			CSIReportObject* pNextObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetAt(pos);
			m_pWorkSheet->ObjList.SetAt(oldPos, pNextObject);
			m_pWorkSheet->ObjList.SetAt(pos, pSelectedObject);
			break;
		}
	}

	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditSendBackward()
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount != 1) return;
	
	CSIReportObject *pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetAt(m_listSelectedObject.FindIndex(0));
	if (pSelectedObject == NULL) return;

	POSITION pos = m_pWorkSheet->ObjList.GetTailPosition();
	while (pos != NULL)
	{
		POSITION oldPos = pos;
		CSIReportObject* pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetPrev(pos);
		if (pObject != NULL && pObject == pSelectedObject)
		{
			if (pos == NULL) break;

			CSIReportObject* pPrevObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetAt(pos);
			m_pWorkSheet->ObjList.SetAt(oldPos, pPrevObject);
			m_pWorkSheet->ObjList.SetAt(pos, pSelectedObject);
			break;
		}
	}

	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditSelectAll()
{
EFS_BEGIN

	FreeSelectedObjectList();
	//m_listSelectedObject.AddTail(&m_pWorkSheet->ObjList);
	AddSelectObject(&m_pWorkSheet->ObjList);
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditSelectNextObject()
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount == 0) 
	{
		if (m_pWorkSheet->ObjList.GetCount() > 0)
		{
			CSIReportObject* pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetAt(m_pWorkSheet->ObjList.FindIndex(0));
			if (pObject != NULL)
				//m_listSelectedObject.AddTail((CObject*)pObject);
				AddSelectObject((CObject*)pObject);
		}

		return;
	}

	CSIReportObject* pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetAt(m_listSelectedObject.FindIndex(nCount - 1));
	if (pSelectedObject == NULL) return;

	POSITION pos = m_pWorkSheet->ObjList.GetHeadPosition();
	while (pos != NULL)
	{
		CSIReportObject* pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);
		if (pObject != NULL && pObject == pSelectedObject)
		{
			FreeSelectedObjectList();
			if (pos == NULL)
			{
				//m_listSelectedObject.AddTail((CSIReportObject*)m_pWorkSheet->ObjList.GetAt(m_pWorkSheet->ObjList.FindIndex(0)));
				AddSelectObject((CSIReportObject*)m_pWorkSheet->ObjList.GetAt(m_pWorkSheet->ObjList.FindIndex(0)));
			}
			else
			{
				//m_listSelectedObject.AddTail((CSIReportObject*)m_pWorkSheet->ObjList.GetAt(pos));
				AddSelectObject((CSIReportObject*)m_pWorkSheet->ObjList.GetAt(pos));
			}
			
			break;
		}
	}

	Invalidate();
	UpdateWindow();


EFS_END
}

VOID CMSheetDesigner::EditCutObject()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() == 0) return;

	EditCopyObject();
	DeleteObject();


EFS_END
}

VOID CMSheetDesigner::EditCopyObject()
{
EFS_BEGIN

	INT nCount = m_listSelectedObject.GetCount();
	if (nCount == 0) return;
		
	CString strClipboardString;
	INT ids = m_pIOMgr->CopyObject(&m_listSelectedObject, strClipboardString);
	if (ids < 0) return;

	// Copy object XML string to a global memory block.
    LPTSTR lpszObject;
	HANDLE hData = ::GlobalAlloc (GMEM_MOVEABLE, strClipboardString.GetLength() + 1);
	
	lpszObject = (LPTSTR)GlobalLock(hData);
	_tcscpy(lpszObject, strClipboardString.GetBuffer(0));
	
	::GlobalUnlock(hData);

	OpenClipboard();
	EmptyClipboard();
	UINT nFormat = ((CSIReportEditorApp*) AfxGetApp ())->GetClipboardFormat ();
	SetClipboardData(nFormat, hData);
	CloseClipboard();

EFS_END
}

VOID CMSheetDesigner::EditPastObject()
{
EFS_BEGIN

	if (!OpenClipboard()) return;
	
	UINT nFormat = ((CSIReportEditorApp*) AfxGetApp ())->GetClipboardFormat ();

	HANDLE hClipData;
	if (!(hClipData = GetClipboardData(nFormat)))
	{
		CloseClipboard();
		return;
	}

	HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, GlobalSize(hClipData));
	
	LPTSTR lpClipData = (LPTSTR)GlobalLock(hClipData);
	
	CString strClipboardString;
	strClipboardString.Format(_T("%s"), lpClipData);
	
	::GlobalUnlock(hData);
	CloseClipboard();

	if (m_pIOMgr->PastObject(m_pWorkSheet, strClipboardString))
	{
		Invalidate();
		UpdateWindow();
	}

EFS_END
}

// redo /undo
VOID CMSheetDesigner::EditUndo()
{
EFS_BEGIN

	INT nCount = m_listUndo.GetCount();
	if (nCount == 0) return;

	EditSaveHistory(MODE_REDO);
	
	// Get Undo sheet
	CSIReportWorkSheet* pSheet = (CSIReportWorkSheet*)m_listUndo.RemoveTail();
	if (pSheet == NULL) return;
	
	// Replace sheet
	FreeSelectedObjectList();
	m_pWorkSheet->FreeObjects();
	pSheet->Clone(m_pWorkSheet);
	delete pSheet;

	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditRedo()
{
EFS_BEGIN

	INT nCount = m_listRedo.GetCount();
	if (nCount == 0) return;

	EditSaveHistory();

	CSIReportWorkSheet* pSheet = (CSIReportWorkSheet*)m_listRedo.RemoveTail();
	if (pSheet == NULL) return;

	FreeSelectedObjectList();
	m_pWorkSheet->FreeObjects();
	pSheet->Clone(m_pWorkSheet);
	delete pSheet;
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditRemoveTableRow()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
		((CSIReportObject_Table*)pObject)->RemoveRow();
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditRemoveTableColumn()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
		((CSIReportObject_Table*)pObject)->RemoveColumn();
	
	Invalidate();
	UpdateWindow();

EFS_END
}

// Table
VOID CMSheetDesigner::EditText()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	if (pObject == NULL) return;
	
	if (pObject->GetObjType() != OBJECT_TABLE && pObject->GetObjType() != OBJECT_TEXT_BOX) return;

	CString strText;
	CRect rect;

	if (pObject->GetObjType() == OBJECT_TABLE) 
	{
	
		CSIReportObject_Table_Cell* pCell = ((CSIReportObject_Table*)pObject)->GetFocusCell();
		if (pCell == NULL) return;
	
		rect = pCell->GetRect();
		strText = pCell->text.text;
	}
	else if (pObject->GetObjType() == OBJECT_TEXT_BOX)
	{
		rect = pObject->GetRect();
		strText = ((CSIReportObject_Text_Box*)pObject)->text.text;
	}

	CPaintDC dc(this);
	GetIsotropicDC(dc);

	dc.LPtoDP(rect);
	rect.DeflateRect(2, 2);

	m_pEditWnd->MoveWindow(rect);
	m_pEditWnd->SetWindowText(strText);
	m_pEditWnd->ShowWindow(SW_SHOW);
	m_pEditWnd->SetSel(0, -1);
	m_pEditWnd->SetFocus();

EFS_END
}

VOID CMSheetDesigner::EditTableDeleteRow()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
	{
		CArray<CString, CString&> arrayDeletedChildName;
		((CSIReportObject_Table*)pObject)->DeleteRow(&arrayDeletedChildName);

		INT nDeleteChildCount = arrayDeletedChildName.GetSize();
		if (nDeleteChildCount  > 0)
		{
			for (INT i = 0; i < nDeleteChildCount; i++)
			{
				CSIReportObject *pChildObject = GetObject(arrayDeletedChildName.GetAt(i));
				if (pChildObject == NULL) continue;

				pChildObject->SetParentName(_T(""));
				pChildObject->SetParentObject(NULL);
			}
		}
	}
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditTableDeleteColumn()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
	{
		CArray<CString, CString&> arrayDeletedChildName;
		INT nCol = ((CSIReportObject_Table*)pObject)->DeleteColumn(&arrayDeletedChildName);

		INT nDeleteChildCount = arrayDeletedChildName.GetSize();
		if (nDeleteChildCount  > 0)
		{
			for (INT i = 0; i < nDeleteChildCount; i++)
			{
				CSIReportObject *pChildObject = GetObject(arrayDeletedChildName.GetAt(i));
				if (pChildObject == NULL) continue;

				pChildObject->SetParentName(_T(""));
				pChildObject->SetParentObject(NULL);
			}
		}
		

		// Find Child table and change column
		CSIReportObject_Table* pChildTable = FindChildTable((CSIReportObject_Table*)pObject);
		if (pChildTable != NULL)
		{
			CSIReportObject_Table_Cell *pCell = pChildTable->GetCell(0, nCol);
			if (pCell != NULL)
			{
				pCell->focus = TRUE;
				pChildTable->DeleteColumn();
			}
		}
	}
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditTableInsertRow()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
		((CSIReportObject_Table*)pObject)->InsertRow();
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditTableInsertColumn()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
	{
		INT nCol = ((CSIReportObject_Table*)pObject)->InsertColumn();
		// Find Child table and change column
		CSIReportObject_Table* pChildTable = FindChildTable((CSIReportObject_Table*)pObject);
		if (pChildTable != NULL)
		{
			CSIReportObject_Table_Cell *pCell = pChildTable->GetCell(0, nCol);
			if (pCell != NULL)
			{
				pChildTable->GetCell(0, nCol)->focus = TRUE;
				pChildTable->InsertColumn();
			}
		}

		Invalidate();
		UpdateWindow();
	}

EFS_END
}

VOID CMSheetDesigner::EditTableAddRow()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
		((CSIReportObject_Table*)pObject)->AddRow();
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditTableAddColumn()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
		((CSIReportObject_Table*)pObject)->AddColumn();
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditTableChangeColumn()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	EditSaveHistory();
	
	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
	{
		INT nRow;
		INT nCol;
		CSIReportObject_Table* pTable = (CSIReportObject_Table*)pObject;
		if (pTable->GetFocusCell(nRow, nCol) == NULL) return;
		
		CDlgColumnChange dlg;
		if (dlg.DoModal() == IDOK)
		{
			pTable->ChangeColumn(nCol, dlg.m_nColumn - 1);

			// Find Child table and change column
			CSIReportObject_Table* pChildTable = FindChildTable(pTable);
			if (pChildTable != NULL)
				pChildTable->ChangeColumn(nCol, dlg.m_nColumn - 1);
		}
		else
			return;
	}
	
	Invalidate();
	UpdateWindow();


EFS_END
}

// Merge / DeMerge
VOID CMSheetDesigner::EditTableMergeCells()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;

	EditSaveHistory();
	
	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
		((CSIReportObject_Table*)pObject)->MergeSelectedCells();
	
	Invalidate();
	UpdateWindow();

EFS_END
}

VOID CMSheetDesigner::EditTableDeMergeCells()
{
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return;

	EditSaveHistory();

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	
	if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
		((CSIReportObject_Table*)pObject)->DeMergeSelectedCells();
	
	Invalidate();
	UpdateWindow();

EFS_END
}

CSIReportObject_Table* CMSheetDesigner::FindChildTable(CSIReportObject_Table* pTableParent)
{
EFS_BEGIN

	if (!pTableParent->m_strParentName.IsEmpty()) return NULL;

	POSITION posCell = pTableParent->CellList.GetHeadPosition();
	while (posCell != NULL)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)pTableParent->CellList.GetNext(posCell);
		if (pCell == NULL) continue;
		if (pCell->child_count == 0) continue;

		POSITION posObject = m_pWorkSheet->ObjList.GetHeadPosition();
		while (posObject != NULL)
		{
			CSIReportObject* pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(posObject);
			if (pObject == NULL) continue;

			if (pObject->GetObjType() == OBJECT_TABLE && pObject->m_strName == pCell->child_name[0])
				return (CSIReportObject_Table*)pObject;
		}
	}

	return NULL;

EFS_END
return NULL;

}

CSIReportObject_Table_Cell* CMSheetDesigner::FindParentCell(CString strParentName, CString strChildName)
{
EFS_BEGIN

	POSITION pos = m_pWorkSheet->ObjList.GetHeadPosition();
	while(pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);
		if (pObject != NULL && pObject->GetObjType() == OBJECT_TABLE)
		{
			if (pObject->GetName() == strParentName)
			{
				CSIReportObject_Table *pTable = (CSIReportObject_Table*)pObject;
				POSITION posCell = pTable->CellList.GetHeadPosition();
				while (posCell)
				{
					CSIReportObject_Table_Cell *pCell = (CSIReportObject_Table_Cell*)pTable->CellList.GetNext(posCell);
					if (pCell == NULL) continue;

					// Check name;
					if (pCell->child_count > 0 && pCell->child_name[0] == strChildName)
					{
						return pCell;
					}
				}
			}
		}
	}

	return NULL;

EFS_END
return NULL;

}

VOID CMSheetDesigner::EditSaveHistory(INT nMode /* = MODE_UNDO */)
{
EFS_BEGIN

	if (!m_bHistory) return;
	return;
	
	CSIReportWorkSheet* pSheet = m_pWorkSheet->Clone(NULL);

	// pSheet to undo list
	INT nCount;

	if (nMode == MODE_UNDO)
	{
		m_listUndo.AddTail((CObject*)pSheet);
		nCount = m_listUndo.GetCount();
	}
	else if (nMode == MODE_REDO)
	{
		m_listRedo.AddTail((CObject*)pSheet);
		nCount = m_listRedo.GetCount();
	}
	
	// Circuler
	if (nCount > MAX_UNDO)
	{
		CSIReportWorkSheet* pDeleteSheet;
		
		if (nMode == MODE_UNDO)
			pDeleteSheet = (CSIReportWorkSheet*)m_listUndo.RemoveHead();
		else if (nMode == MODE_REDO)
			pDeleteSheet = (CSIReportWorkSheet*)m_listRedo.RemoveHead();

		if (pDeleteSheet != NULL)
		{
			delete pDeleteSheet;
			pDeleteSheet = NULL;
		}
	}
	return;

EFS_END
}

VOID CMSheetDesigner::EditZoomIn()
{
EFS_BEGIN

	if (m_nZoomMode == 2) return;

	CRect rect;
	GetClientRect(rect);

	m_nZoomMode++;

	rect.right = rect.left + rect.Width() * 2;
	rect.bottom = rect.top + rect.Height() * 2;

	MoveWindow(rect, FALSE);

EFS_END
}

VOID CMSheetDesigner::EditZoomOut()
{
EFS_BEGIN

	if (m_nZoomMode == 0) return;

	CRect rect;
	GetClientRect(rect);

	m_nZoomMode--;

	rect.right = rect.left + rect.Width() / 2;
	rect.bottom = rect.top + rect.Height() / 2;
	
	MoveWindow(rect, FALSE);

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// Draw
VOID CMSheetDesigner::GetIsotropicDC(CDC &dc)
{
EFS_BEGIN

	CRect rect;
	GetClientRect(rect);

	dc.SetBkMode(TRANSPARENT);
	
	dc.SetMapMode(MM_ANISOTROPIC);
	CSize size = dc.GetViewportExt();
	CSize size2 = dc.GetWindowExt();

	// determine zoom mode
	if (m_nZoomMode == 0)
	{
		dc.SetWindowExt(rect.Width(), rect.Height());	
	}
	else if (m_nZoomMode > 0)
	{
		dc.SetWindowExt(rect.Width() / (m_nZoomMode * 2), rect.Height() / (m_nZoomMode * 2));
	}
	
	//dc.SetWindowExt(rect.Width() / m_nZoom, rect.Height() / m_nZoom);

	dc.SetViewportExt(rect.Width(), rect.Height());
	//dc.SetViewportOrg(10, 10);

EFS_END
}

VOID CMSheetDesigner::OnPaint() 
{
EFS_BEGIN

	CPaintDC dc(this); // device context for painting
	GetIsotropicDC(dc);

	if (m_bDoubleBuffering)
	{
		CMemDC memDC(&dc);
		OnDraw(&memDC);
		DrawObject(&memDC);
	}
	else
	{
		OnDraw(&dc);
		DrawObject(&dc);
	}
	
	// Do not call CWnd::OnPaint() for painting messages

EFS_END
}

VOID CMSheetDesigner::OnDraw(CDC *pDC)
{
EFS_BEGIN

	CRect rect;
	GetClientRect(rect);
	
	CBrush fg_brush;
	fg_brush.CreateSolidBrush(RGB(255, 255, 255));
	
	pDC->DPtoLP(rect);
	pDC->FillRect(rect, &fg_brush);

EFS_END
}

VOID CMSheetDesigner::DrawObject(CDC *pDC)
{
EFS_BEGIN

	// Draw Object
	POSITION pos = m_pWorkSheet->ObjList.GetHeadPosition();
	while (pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);
		if (pObject == NULL) continue;
		
		pObject->Draw(pDC);
	}

	// Draw Selected Item
	pos = m_listSelectedObject.GetHeadPosition();
	while (pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;
		
		CSize sizePickPoint = m_sizePickPoint;
		pDC->DPtoLP(&sizePickPoint);
		pObject->DrawSelected(pDC, sizePickPoint);
	}

EFS_END
}

BOOL CMSheetDesigner::OnEraseBkgnd(CDC* pDC) 
{
EFS_BEGIN

	return TRUE;
	GetIsotropicDC(*pDC);
	CRect rect;
	GetClientRect(rect);
	
	CBrush fg_brush;
	fg_brush.CreateSolidBrush(RGB(255, 255, 255));
	
	pDC->DPtoLP(rect);
	pDC->FillRect(rect, &fg_brush);

	return TRUE;	
	//return CWnd::OnEraseBkgnd(pDC);

EFS_END
return FALSE;

}

VOID CMSheetDesigner::DrawResizeImage(POINT point)
{
EFS_BEGIN

	CClientDC dc(this);
	GetIsotropicDC(dc);

	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while(pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;
			
		pObject->DrawResizeImage(&dc, point);
	}

EFS_END
}

VOID CMSheetDesigner::DrawResizedImage(POINT point)
{
EFS_BEGIN

	CClientDC dc(this);
	GetIsotropicDC(dc);

	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while(pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;
		
		pObject->DrawResizeImage(&dc, point);
		pObject->SetRect(pObject->GetResizeRect());
	}

EFS_END
}

VOID CMSheetDesigner::DrawDragImage(POINT point)
{
EFS_BEGIN

	CClientDC dc(this);
	GetIsotropicDC(dc);

	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while(pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;
		
		pObject->DrawDragImage(&dc, point);
	}

EFS_END
}

VOID CMSheetDesigner::DrawDropImage(POINT point)
{
EFS_BEGIN

	CClientDC dc(this);
	GetIsotropicDC(dc);

	POSITION pos = m_listSelectedObject.GetHeadPosition();
	while(pos)
	{
		CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
		if (pObject == NULL) continue;
		
		pObject->DrawDragImage(&dc, point);
		pObject->SetRect(pObject->GetDragRect());
	}

EFS_END
}

VOID CMSheetDesigner::DrawResizingCol(POINT point)
{
EFS_BEGIN

	CClientDC dc(this);
	GetIsotropicDC(dc);

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	POSITION pos = m_listSelectedObject.FindIndex(0);
	
	CSIReportObject_Table *pTable = (CSIReportObject_Table*)m_listSelectedObject.GetAt(pos);
	if (pTable != NULL)
		pTable->DrawResizingCol(&dc, point);

EFS_END
}

VOID CMSheetDesigner::DrawResizingRow(POINT point)
{
EFS_BEGIN

	CClientDC dc(this);
	GetIsotropicDC(dc);

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	POSITION pos = m_listSelectedObject.FindIndex(0);

	CSIReportObject_Table *pTable = (CSIReportObject_Table*)m_listSelectedObject.GetAt(pos);
	if (pTable != NULL)
		pTable->DrawResizingRow(&dc, point);

EFS_END
}

VOID CMSheetDesigner::DrawResizedCol(POINT point)
{
EFS_BEGIN

	CClientDC dc(this);
	GetIsotropicDC(dc);

	if (m_listSelectedObject.GetCount() != 1) 
		return;
	POSITION pos = m_listSelectedObject.FindIndex(0);

	CSIReportObject_Table *pTable = (CSIReportObject_Table*)m_listSelectedObject.GetAt(pos);
	if (pTable != NULL)
	{
		pTable->DrawResizingCol(&dc, point);
		pTable->DrawResizedCol(&dc, point);
	}

EFS_END
}

VOID CMSheetDesigner::DrawResizedRow(POINT point)
{
EFS_BEGIN

	CClientDC dc(this);
	GetIsotropicDC(dc);
	
	if (m_listSelectedObject.GetCount() != 1) 
		return;
	POSITION pos = m_listSelectedObject.FindIndex(0);

	CSIReportObject_Table *pTable = (CSIReportObject_Table*)m_listSelectedObject.GetAt(pos);
	if (pTable != NULL)
	{
		pTable->DrawResizingRow(&dc, point);
		pTable->DrawResizedRow(&dc, point);
	}

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// Mouse message handlers

VOID CMSheetDesigner::OnMouseMove(UINT nFlags, CPoint point) 
{
EFS_BEGIN

	// Check ReadOnly
	if (m_bIsReadOnly) return;

	CWnd::OnMouseMove(nFlags, point);

	SetCursor(AfxGetApp()->LoadStandardCursor(m_szMouseCursor));
	
	CPaintDC dc(this);
	GetIsotropicDC(dc);
	dc.DPtoLP(&point);

	// not click mouse left button
	if (nFlags != MK_LBUTTON)
	{
		m_szMouseCursor = IDC_ARROW;
		m_MouseMode = MOUSE_NOTHING;
		
		// 1. check drag item
		CSIReportObject* pObject = GetObject(point);
		if (pObject != NULL)
		{			
			m_szMouseCursor = IDC_SIZEALL;

			if (pObject->GetObjType() == OBJECT_TABLE)
			{
				INT nMouseMode = ((CSIReportObject_Table*)pObject)->GetMouseMode(point);

				if (nMouseMode == CELL_OVER)
				{
					m_szMouseCursor = IDC_IBEAM;
					m_MouseMode = MOUSE_SELECT_CELLS;
				}
				else if (nMouseMode == RESIZING_COL)
				{
					m_szMouseCursor = IDC_SIZEWE;
					m_MouseMode = MOUSE_OVER_COL_DIVIDE;
				}
				else if (nMouseMode == RESIZING_ROW)
				{
					m_szMouseCursor = IDC_SIZENS;
					m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
				}
			}
		}
		
		// select selected Item for resizing or draging
		if (m_MouseMode != MOUSE_SELECT_CELLS && m_MouseMode != MOUSE_OVER_COL_DIVIDE && m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
		{			
			POSITION pos = m_listSelectedObject.GetHeadPosition();
			while(pos != NULL)
			{		
				CSIReportObject* pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
				if (pObject != NULL && pSelectedObject != NULL && pObject == pSelectedObject)
				{
					// show resizing arrow
					m_nResizeDir = pSelectedObject->PtInResizeRect(point);
					if (m_nResizeDir)
					{
						switch(m_nResizeDir)
						{
						case CSIReportObject::LEFT_TOP:
						case CSIReportObject::RIGHT_BOTTOM:
							m_szMouseCursor = IDC_SIZENWSE;
							break;

						case CSIReportObject::RIGHT_TOP:
						case CSIReportObject::LEFT_BOTTOM:
							m_szMouseCursor = IDC_SIZENESW;
							break;

						case CSIReportObject::MIDDLE_TOP:
						case CSIReportObject::MIDDLE_BOTTOM:
							m_szMouseCursor = IDC_SIZENS;
							break;
						
						case CSIReportObject::LEFT_MIDDLE:
						case CSIReportObject::RIGHT_MIDDLE:
							m_szMouseCursor = IDC_SIZEWE;
							break;					
						}
						m_MouseMode = MOUSE_PREPARE_RESIZING;
					}
					else
						m_MouseMode = MOUSE_PREPARE_DRAG;
				}
			}
		}
	}
	// CELL SELECTING
	else if (m_MouseMode == MOUSE_SELECT_CELLS)
	{
		if (m_listSelectedObject.GetCount() > 1) return;

		POSITION pos = m_listSelectedObject.GetHeadPosition();
		while (pos != NULL)
		{
			CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
			if (pObject == NULL) continue;

			if (pObject->GetObjType() == OBJECT_TABLE)
			{
				if (((CSIReportObject_Table*)pObject)->SetCellFocus(point) == 0)
				{
					Invalidate();
					UpdateWindow();
				}
			}
		}
	}
	// RESIZING
	else if (m_MouseMode == MOUSE_RESIZING)
	{
		m_szMouseCursor = IDC_CROSS;
		DrawResizeImage(point);
	}
	// DRAGGING
	else if (m_MouseMode == MOUSE_DRAGGING)
	{
		DrawDragImage(point);
	}
	// Resiziing col
	else if (m_MouseMode == MOUSE_SIZING_COL)
	{
		DrawResizingCol(point);
	}
	// Resizing row
	else if (m_MouseMode == MOUSE_SIZING_ROW)
	{
		DrawResizingRow(point);
	}

EFS_END
}

VOID CMSheetDesigner::OnLButtonDown(UINT nFlags, CPoint point) 
{
EFS_BEGIN

	// Check ReadOnly
	if (m_bIsReadOnly) return;

	CWnd::OnLButtonDown(nFlags, point);
	
	CPaintDC dc(this);
	GetIsotropicDC(dc);
	CPoint dp = point;
	dc.DPtoLP(&point);

	if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
		SetFocus();

	SetCursor(AfxGetApp()->LoadStandardCursor(m_szMouseCursor));

	if (m_MouseMode == MOUSE_PREPARE_RESIZING)
	{
		// Initialize resize
		POSITION pos = m_listSelectedObject.GetHeadPosition();
		while(pos != NULL)
		{
			CSIReportObject *pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
			if (pSelectedObject == NULL) continue;
			
			pSelectedObject->InitResize(m_nResizeDir, point);
		}
		SetCapture();
		m_MouseMode = MOUSE_RESIZING;
	}
	else if (nFlags & MK_SHIFT)
	{
		CSIReportObject *pObject = GetObject(point);	
		if (pObject != NULL)
		{
			BOOL bAddTail = TRUE;
			POSITION pos = m_listSelectedObject.GetTailPosition();
			while(pos != NULL)
			{
				POSITION posOld = pos;
				if (pObject == (CSIReportObject*)m_listSelectedObject.GetPrev(pos))
				{
					m_listSelectedObject.RemoveAt(posOld);
					bAddTail = FALSE;
				}
			}
			if (bAddTail)
				//m_listSelectedObject.AddTail((CObject*)pObject);
				AddSelectObject((CObject*)pObject);
		}
	}
	else if (m_MouseMode == MOUSE_NOTHING ||
			m_MouseMode == MOUSE_SELECT_CELLS || 
			m_MouseMode == MOUSE_OVER_COL_DIVIDE ||
			m_MouseMode == MOUSE_OVER_ROW_DIVIDE)
	{
		FreeSelectedObjectList();

		CSIReportObject *pObject = GetObject(point);
		if (pObject != NULL)
		{
			//m_listSelectedObject.AddTail((CObject*)pObject);
			AddSelectObject((CObject*)pObject);
			
			// select cells
			if (pObject->GetObjType() == OBJECT_TABLE && m_MouseMode == MOUSE_SELECT_CELLS)
			{
				((CSIReportObject_Table*)pObject)->ReSetCellFocus();
				((CSIReportObject_Table*)pObject)->SetCellFocus(point);
			}
			else if (m_MouseMode == MOUSE_OVER_COL_DIVIDE)
				m_MouseMode = MOUSE_SIZING_COL;
			else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE)
				m_MouseMode = MOUSE_SIZING_ROW;
			else 
				m_MouseMode = MOUSE_PREPARE_DRAG;
		}
		// Rect tracker
		else
		{
			m_rectTracker.TrackRubberBand(this, dp, TRUE);
			
			CRect rect;
			m_rectTracker.GetTrueRect(rect);

			CPaintDC dc(this);
			GetIsotropicDC(dc);
			dc.DPtoLP(rect);
			
			POSITION pos = m_pWorkSheet->ObjList.GetHeadPosition();
			while(pos)
			{
				pObject = (CSIReportObject*)m_pWorkSheet->ObjList.GetNext(pos);
				if (pObject == NULL) continue;

				CRect rectObject = pObject->GetRect();
				
				if (rectObject.Width() == 0 || rectObject.Height() == 0)
					rectObject.InflateRect(5, 5);

				CRect rectIntersect = rectObject;

				rectIntersect.IntersectRect(rectObject, rect);
				if (rectObject == rectIntersect)
					//m_listSelectedObject.AddTail((CObject*)pObject);
					AddSelectObject((CObject*)pObject);
			}
		}
	}

	ShowPropertyWindow();
	
	Invalidate();
	UpdateWindow();
	
	if (m_MouseMode == MOUSE_PREPARE_DRAG)
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));

		// Initialize drag
		POSITION pos = m_listSelectedObject.GetHeadPosition();
		while(pos)
		{
			CSIReportObject *pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetNext(pos);
			if (pSelectedObject)
				pSelectedObject->InitDrag(point);
		}
		
		SetCapture();
		m_MouseMode = MOUSE_DRAGGING;
		DrawDragImage(point);
	}
	
	if (m_MouseMode != MOUSE_NOTHING && m_MouseMode != MOUSE_SELECT_CELLS)
	{
		EditSaveHistory();
	}

EFS_END
}

VOID CMSheetDesigner::OnLButtonUp(UINT nFlags, CPoint point) 
{
EFS_BEGIN

	// Check ReadOnly
	if (m_bIsReadOnly) return;

	CWnd::OnLButtonUp(nFlags, point);

	CPaintDC dc(this);
	GetIsotropicDC(dc);
	dc.DPtoLP(&point);

	if (m_MouseMode == MOUSE_DRAGGING)
	{
		if (m_listSelectedObject.GetCount() == 1)
		{
			POSITION pos = m_listSelectedObject.FindIndex(0);
			if (pos != NULL)
			{
				CSIReportObject *pSelectedObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
				if (pSelectedObject != NULL && pSelectedObject->GetObjType() == OBJECT_TABLE)
				{
					CSIReportObject *pObject = GetObject(point);
					if (pObject != NULL && (pObject != pSelectedObject) && (pObject->GetObjType() == OBJECT_TABLE))
					{
						CSIReportObject_Table_Cell *pCell = ((CSIReportObject_Table*)pObject)->GetCell(point);
						if (pCell != NULL && pSelectedObject->m_strParentName.IsEmpty())
						{
							if (MessageBox(_T("객체를 붙이겠습니까?"), _T("객체 붙이기"), MB_ICONQUESTION | MB_YESNO) == IDYES)
							{
								/// TODO : Attach selected object
								pSelectedObject->SetParentObject(pCell);
								pSelectedObject->m_strParentName = pCell->GetParentObject()->m_strName;

								pCell->child_name[pCell->child_count] = pSelectedObject->m_strName;
								pCell->child_count++;
							}
						}
					}
				}
			}
		}
		
		DrawDropImage(point);
	}
	else if (m_MouseMode == MOUSE_RESIZING)
	{
		DrawResizedImage(point);
	}
	else if (m_MouseMode == MOUSE_SIZING_COL)
	{
		DrawResizedCol(point);
	}
	else if (m_MouseMode == MOUSE_SIZING_ROW)
	{
		DrawResizedRow(point);
	}

	if (m_MouseMode != MOUSE_NOTHING)
	{
		Invalidate();
		UpdateWindow();		
	}
		
	m_MouseMode = MOUSE_NOTHING;
	ReleaseCapture();

EFS_END
}

VOID CMSheetDesigner::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
EFS_BEGIN

	if (Acceclerator(nChar)) return;	
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);

EFS_END
}

BOOL CMSheetDesigner::Acceclerator(UINT nChar)
{
EFS_BEGIN

	if (GetAsyncKeyState(VK_CONTROL))
	{
		switch(nChar)
		{
		case VK_LEFT:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_UP:
			EditProc(nChar);
			return TRUE;
		case VK_F3:
			EditTableInsertRow();
			return TRUE;
		case VK_F4:
			EditTableInsertColumn();
			return TRUE;
		case VK_F5:
			EditTableDeleteRow();
			return TRUE;
		case VK_F6:
			EditTableDeleteColumn();
			return TRUE;
		case VK_F9:
			EditCenterObject(nChar);
			return TRUE;
		/*
		// Select All
		case 0x41:
			EditSelectAll();
			return TRUE;

		// Undo
		case 0x5A:
			EditUndo();
			return TRUE;

		// Redo
		case 0x59:
			EditRedo();
			return TRUE;

		// Copy
		case 0x43:
			EditCopyObject();
			return TRUE;

		// Cut
		case 0x58:
			EditCutObject();
			return TRUE;

		// Past
		case 0x56:
			EditPastObject();
			return TRUE;
		*/
		default:
			return FALSE;
			
		}
	}
	else
	{
		switch(nChar)
		{
		case VK_F2:
			ShowPropertyWindow();
			return TRUE;
		case VK_F3:
			EditTableAddRow();
			return TRUE;
		case VK_F4:
			EditTableAddColumn();
			return TRUE;
		case VK_F5:
			EditTableDeleteRow();
			return TRUE;
		case VK_F6:
			EditTableDeleteColumn();
			return TRUE;
		case VK_F7:
			EditTableMergeCells();
			return TRUE;
		case VK_F8:
			EditTableDeMergeCells();
			return TRUE;
		case VK_F11:
			EditTableChangeColumn();
			return TRUE;
		case VK_DELETE:
			DeleteObject();
			return TRUE;
		case VK_LEFT:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_UP:
			EditMoveObject(nChar);
			return TRUE;
		case VK_TAB:
			EditSelectNextObject();
			return TRUE;
		case VK_RETURN:
			EditText();
		default:
			return FALSE;
		}
	}
	return FALSE;

EFS_END
return FALSE;

}

LONG CMSheetDesigner::OnPropertyChanged(UINT wItemChanged, LONG lPropertyType)
{	
EFS_BEGIN

	if (m_listSelectedObject.GetCount() != 1) 
		return 0;
	POSITION pos = m_listSelectedObject.FindIndex(0);

	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);
	if (pObject == NULL) return 0;

	EditSaveHistory();
	
	CPropertyListCtrl *pCtrl = m_pDlgProperty->GetCurrentPropertyListCtrl();

	// get changed property item
	INT			nSelectedItem = 0;
	CString		strText;
	BOOL		bValue = FALSE;
	COLORREF	color;
	LOGFONT		logfont;
	INT			nFontSize = 0;
	COLORREF	colorFont;

	switch (lPropertyType)
	{
	case ID_PROPERTY_TEXT:
	case ID_PROPERTY_TEXT_MULTI:
	case ID_PROPERTY_PATH:
		pCtrl->GetProperty(wItemChanged, &strText);
		break;
	case ID_PROPERTY_BOOL:
		pCtrl->GetProperty(wItemChanged, &bValue);
		break;
	case ID_PROPERTY_COLOR:
		pCtrl->GetProperty(wItemChanged, &color);
		break;
	case ID_PROPERTY_FONT:
		pCtrl->GetProperty(wItemChanged, &logfont, &nFontSize, &colorFont);
		break;
	case ID_PROPERTY_COMBO_LIST:
		pCtrl->GetProperty(wItemChanged, &nSelectedItem, &strText);
		break;
	}
	
	// set property item to object
	CString strAlias = pCtrl->GetAlias();
	CString strItemAlias = pCtrl->GetItemAlias(wItemChanged);
	if (strItemAlias.IsEmpty()) return 0;
	
	if (strAlias == _T("LINE"))
	{
		CSIReportObject_Line* pLine = (CSIReportObject_Line*)pObject;

		if (strItemAlias == _T("thick"))
		{
			pLine->thick = _ttoi(strText);
		}
		else if (strItemAlias == _T("style"))
		{
			pLine->style = nSelectedItem;
		}

	}
	else if (strAlias == _T("TEXT_BOX"))
	{
		CSIReportObject_Text_Box* pTextBox = (CSIReportObject_Text_Box*)pObject;
		if (strItemAlias == _T("m_strName"))
		{	
			pTextBox->m_strName = strText;
		}
		else if (strItemAlias == _T("horz_line"))
		{
			pTextBox->horz_line = bValue;
		}
		else if (strItemAlias == _T("vert_line"))
		{
			pTextBox->vert_line = bValue;
		}
		else if (strItemAlias == _T("fill_color"))
		{
			pTextBox->fill_color = color;
		}
		else if (strItemAlias == _T("indent_len"))
		{
			pTextBox->indent_len = _ttoi(strText);
		}
		else if (strItemAlias == _T("horz_align"))
		{
			pTextBox->horz_align = nSelectedItem;
		}
		else if (strItemAlias == _T("vert_align"))
		{
			pTextBox->vert_align = nSelectedItem;
		}
		else if (strItemAlias == _T("text"))
		{
			pTextBox->text.text = strText;
		}
		else if (strItemAlias == _T("font"))
		{
			pTextBox->text.font = logfont;
			pTextBox->text.size = nFontSize;
			pTextBox->text.color = colorFont;
		}
		else if (strItemAlias == _T("m_nOnlyShowFirstPage"))
		{
			pTextBox->m_nOnlyShowFirstPage = bValue;
		}
	}
	else if (strAlias == _T("BITMAP"))
	{
		CSIReportObject_Bitmap* pBitmap = (CSIReportObject_Bitmap*)pObject;
		if (strItemAlias == _T("m_strName"))
		{
			pBitmap->m_strName = strText;
		}
		else if (strItemAlias == _T("file_name"))
		{
			if (pBitmap->file_name != strText)
			{
				pBitmap->FreeBitmap();
				pBitmap->file_name = strText;
				pBitmap->LoadBitmap();
			}
		}
	}
	else if (strAlias == _T("BARCODE"))
	{
		CSIReportObject_Barcode* pBarcode = (CSIReportObject_Barcode*)pObject;
		if (strItemAlias == _T("m_strName"))
		{
			pBarcode->m_strName = strText;
		}
		else if (strItemAlias == _T("m_strCaption"))
		{
			if (!strText.IsEmpty() && pBarcode->m_nStyle == 2) 
			{
				for (INT i = 0; i <= 9; i++)
				{
					CString strTemp;
					strTemp.Format(_T("%d"), i);
					if (strText.Find(strTemp) >= 0)
					{
						AfxMessageBox(_T("이 형식에서는 숫자를 사용할 수 없습니다."));
						ShowPropertyWindow();
						return 0;
					}
				}
			}
			pBarcode->SetCaption(strText);
		}

		else if (strItemAlias == _T("m_nStyle"))
		{
			if (!pBarcode->m_strCaption.IsEmpty() && strText == _T("Interleaved 2 of 5"))
			{
				for (INT i = 0; i <= 9; i++)
				{
					CString strTemp;
					strTemp.Format(_T("%d"), i);
					if (pBarcode->m_strCaption.Find(strTemp) >= 0)
					{
						AfxMessageBox(_T("이 형식에서는 숫자를 사용할 수 없습니다."));
						ShowPropertyWindow();
						return 0;
					}
				}
			}

			pBarcode->SetStyle(strText);
		}
	}
	else if (strAlias == _T("TABLE"))
	{
		CSIReportObject_Table* pTable = (CSIReportObject_Table*)pObject;

		if (strItemAlias == _T("m_strName"))
		{
			// Check Table Is Child
			if (!pTable->GetParentName().IsEmpty())
			{
				// Update child name to Parent table cell
				CSIReportObject_Table_Cell *pCell = (CSIReportObject_Table_Cell*)FindParentCell(pTable->GetParentName(), pTable->GetName());
				if (pCell != NULL)
				{
					pCell->child_name[0] = strText;
				}
			}

			// Check Table Is Parent
			CSIReportObject_Table *pChildTable = FindChildTable(pTable);
			if (pChildTable != NULL)
			{
				pChildTable->SetParentName(strText);
				pChildTable->SetParentObject(pTable);
			}

			// change table name
			pTable->m_strName = strText;
			
		}
		else if (strItemAlias == _T("repeat"))
		{
			pTable->repeat = bValue;
		}
		else if (strItemAlias == _T("part_sum"))
		{
			pTable->part_sum = bValue;
		}
		else if (strItemAlias == _T("total_sum"))
		{
			pTable->total_sum = bValue;
		}
		else if (strItemAlias == _T("horz_line"))
		{
			pTable->horz_line = bValue;
		}
		else if (strItemAlias == _T("vert_line"))
		{
			pTable->vert_line = bValue;
		}
	}
	else if (strAlias == _T("CELL"))
	{
		CSIReportObject_Table* pTable = (CSIReportObject_Table*)pObject;
		
		CSIReportObject_Table_Cell* pCell = pTable->GetFocusCell();

		if (strItemAlias == _T("data_mgr_name"))
		{
			pCell->data_mgr_name = strText;
			ShowPropertyWindow();
		}
		// User Alias Add ...
		else if (strItemAlias == _T("data_field_user"))
		{
			pCell->data_field_user = strText;
			
			// 유저 에일리어스에 맞는 데이터 필드를 찾는다.
			CESL_DataMgr dm;
			dm.SetCONNECTION_INFO(((CSIReportEditorView*)GetParent())->m_ReadCfgFile.m_sConnection);

			pCell->data_field = GetDataMgrFieldAliasByUserAlias(pCell->data_mgr_name, pCell->data_field_user);
		}

		// adjust property to selected cells but data_mgr_name , data_field
		POSITION pos = pTable->CellList.GetHeadPosition();
		while (pos != NULL)
		{
			pCell = (CSIReportObject_Table_Cell*)pTable->CellList.GetNext(pos);
			if (pCell == NULL) continue;
			if (!pCell->focus) continue;

			if (pCell == NULL) return 0;
		
			if (strItemAlias == _T("part_sum"))
			{
				pCell->part_sum = nSelectedItem;
			}
			else if (strItemAlias == _T("total_sum"))
			{
				pCell->total_sum = nSelectedItem;
			}
			else if (strItemAlias == _T("slash"))
			{
				pCell->slash = nSelectedItem;
			}
			else if (strItemAlias == _T("font"))
			{
				pCell->text.font = logfont;
				pCell->text.size = nFontSize;
				pCell->text.color = colorFont;
			}
			else if (strItemAlias == _T("fill_color"))
			{
				pCell->fill_color = color;
			}
			else if (strItemAlias == _T("char_len"))
			{
				pCell->char_len = _ttoi(strText);
			}
			else if (strItemAlias == _T("char_max"))
			{
				pCell->char_max = _ttoi(strText);
			}
			else if (strItemAlias == _T("indent_len"))
			{
				pCell->indent_len = _ttoi(strText);
			}
			else if (strItemAlias == _T("horz_align"))
			{
				pCell->horz_align = nSelectedItem;
			}
			else if (strItemAlias == _T("vert_align"))
			{
				pCell->vert_align = nSelectedItem;
			}
			else if (strItemAlias == _T("char_max"))
			{
				pCell->char_max = _ttoi(strText);
			}
			else if (strItemAlias == _T("orientation"))
			{
				if (nSelectedItem == 0)
					pCell->orientation = 1;
				else 
					pCell->orientation = -1;
			}
			else if (strItemAlias == _T("text"))
			{
				pCell->text.text = strText;
			}
		}
	}
	
	Invalidate();
	UpdateWindow();

	return 0;

EFS_END
return -1;

}

VOID CMSheetDesigner::OnEditChange()
{
EFS_BEGIN

	

EFS_END
}

VOID CMSheetDesigner::OnEditLostFocus()
{
EFS_BEGIN

	if (m_pEditWnd == NULL) return;

	if (m_listSelectedObject.GetCount() != 1) 
		return;

	m_pEditWnd->ShowWindow(SW_HIDE);

	POSITION pos = m_listSelectedObject.FindIndex(0);
	CSIReportObject *pObject = (CSIReportObject*)m_listSelectedObject.GetAt(pos);

	if (pObject == NULL) return;
	if (pObject->GetObjType() == OBJECT_TABLE) 
	{
	
		CSIReportObject_Table_Cell* pCell = ((CSIReportObject_Table*)pObject)->GetFocusCell();
		if (pCell == NULL) return;
		m_pEditWnd->GetWindowText(pCell->text.text);
	}
	else if (pObject->GetObjType() == OBJECT_TEXT_BOX)
	{
		m_pEditWnd->GetWindowText(((CSIReportObject_Text_Box*)pObject)->text.text);
	}	

EFS_END
}

VOID CMSheetDesigner::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
EFS_BEGIN

	EditText();	
	CWnd::OnLButtonDblClk(nFlags, point);

EFS_END
}

CString CMSheetDesigner::GetDataMgrFieldAliasByUserAlias(CString strDMAlias, CString strUserAlias)
{
	CString strFileName;
	strFileName.Format(_T("..\\CFG\\REFERENCE\\DM\\%s.UDM"), strDMAlias);
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CFile::typeBinary) == FALSE) return _T("");

	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	
	CString strLine;
	CString strTmp1, strTmp2;
	INT nPos1, nPos2;
	while(TRUE) {
		if (fd.ReadString(strLine) == FALSE) break;
		strLine.TrimLeft();			strLine.TrimRight();
		if (strLine.GetLength() == 0) continue;
		if (strLine.GetAt(0) == '#') continue;

		nPos1 = strLine.Find('|', 0);
		if (nPos1 == -1) continue;
		nPos2 = strLine.Find('|', nPos1+1);
		if (nPos2 == -1) continue;

		strTmp1 = strLine.Mid(0, nPos1);
		strTmp1.TrimLeft();			strTmp1.TrimRight();
		strTmp2 = strLine.Mid(nPos1+1, nPos2-nPos1-1);
		strTmp2.TrimLeft();			strTmp2.TrimRight();

		if (strTmp1 == strUserAlias) {
			fd.Close();
			return strTmp2;
		}
	}
	fd.Close();
	return _T("");
}