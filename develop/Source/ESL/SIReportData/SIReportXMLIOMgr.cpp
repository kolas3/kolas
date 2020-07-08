// SIReportXMLIOMgr.cpp: implementation of the CSIReportXMLIOMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportXMLIOMgr.h"
#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSIReportXMLIOMgr::CSIReportXMLIOMgr(INT MASTER_FLAG)
{
	this->MASTER_FLAG = MASTER_FLAG;

	CoInitialize(NULL);
}	

CSIReportXMLIOMgr::~CSIReportXMLIOMgr()
{
	CoUninitialize();
}

//////////////////////////////////////////////////////////////////////
// CSIReportXMLIOMgr Operations
INT CSIReportXMLIOMgr::LoadWorkForm(CSIReportWorkForm *pWorkForm, CString strFileName)
{
EFS_BEGIN

	//CoInitialize(NULL);
	IXMLDOMDocumentPtr pDom(_T("Microsoft.XMLDOM"), NULL, CLSCTX_INPROC_SERVER);
	pDom->async = FALSE;

	try
	{
		CComVariant varFileName = (LPCTSTR)strFileName;
		if (!pDom->load(varFileName))
		{
			throw(0);
		}

		IXMLDOMNodeListPtr pChildNodeList = pDom->childNodes;
		
		FetchWorkForm(pChildNodeList, pWorkForm);
		FetchScript(pChildNodeList, pWorkForm);

		pChildNodeList.Release();
		pDom.Release();
	}
	catch(...)
	{
		LONG errCode;
		CComBSTR sourceString, reasonString;
		IXMLDOMParseErrorPtr errObj;
		pDom->get_parseError(&errObj);
		errObj->get_errorCode(&errCode);
		errObj->get_srcText(&sourceString);
		errObj->get_reason(&reasonString);
		CString strResult;
		strResult.Format(_T("Exception occured : %d, %s, %s"), errCode, CString(sourceString), CString(reasonString));
		AfxMessageBox(strResult);
		CoUninitialize();
		return -1;
	}

	return 0;

EFS_END
return -1;

}

VOID CSIReportXMLIOMgr::FetchWorkForm(MSXML2::IXMLDOMNodeListPtr pNodeList, CSIReportWorkForm *pWorkForm)
{
EFS_BEGIN

	for (LONG idx = 0; idx < pNodeList->length; idx++)
	{
		IXMLDOMNodePtr pNode = pNodeList->item[idx];

		CComBSTR text;
		CComBSTR nameString;

		pNode->get_baseName(&nameString);
		pNode->get_text(&text);

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		// work form
		if (strNameString == _T("WORKFORM"))
		{
			IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;
			for (LONG idx = 0; idx < NodeListPtr->length; idx++)
			{
				IXMLDOMNodePtr pNodeWorkForm = NodeListPtr->item[idx];
				
				CComBSTR text;
				CComBSTR nameString;

				pNodeWorkForm->get_baseName(&nameString);
				pNodeWorkForm->get_text(&text);

				CString strNameString(nameString);
				CString strText(text);

				nameString.Empty();
				text.Empty();
			

				if (strNameString == _T("VERSION"))
					pWorkForm->VERSION = _ttoi(strText);
				else if (strNameString == _T("ALIAS"))
					pWorkForm->alias = CString(text);
				else if (strNameString == _T("SHEET_HEIGHT"))
					pWorkForm->sheet_height = _ttoi(strText);
				else if (strNameString == _T("SHEET_SIZE"))
					pWorkForm->sheet_size = _ttoi(strText);
				else if (strNameString == _T("PAPER"))
					pWorkForm->paper = _ttoi(strText);

			}

			IXMLDOMNodePtr pNextSibling = pNode->nextSibling;
			if (pNextSibling)
			{
				FetchWorkForm(pNextSibling->childNodes, pWorkForm);
				pNextSibling.Release();
			}
			else return;
		}	
		// work sheet
		else if (strNameString == _T("WORKSHEET"))
		{
			CSIReportWorkSheet *pSheet = new CSIReportWorkSheet;
			FetchWorkSheet(pNode, pSheet);
			
			pWorkForm->AddNewSheet(pSheet);

			IXMLDOMNodePtr pNextSibling = pNode->nextSibling;
			if (pNextSibling)
			{
				FetchWorkForm(pNextSibling->childNodes, pWorkForm);
				pNextSibling.Release();
			}
			else return;
		}
		else
		{
			IXMLDOMNodeListPtr pChlidList = pNode->childNodes;
			FetchWorkForm(pChlidList, pWorkForm);
			pChlidList.Release();
		}

		pNode.Release();
	}

EFS_END
}

VOID CSIReportXMLIOMgr::FetchScript(MSXML2::IXMLDOMNodeListPtr pNodeList, CSIReportWorkForm *pWorkForm)
{
EFS_BEGIN

	for (LONG idx = 0; idx < pNodeList->length; idx++)
	{
		IXMLDOMNodePtr pNode = pNodeList->item[idx];

		CComBSTR text;
		CComBSTR nameString;

		pNode->get_baseName(&nameString);
		pNode->get_text(&text);

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();		
		
		if (strNameString == _T("SCRIPT"))
		{
			IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;
			
			for (LONG idx = 0; idx < NodeListPtr->length; idx++)
			{
				IXMLDOMNodePtr pNodeScript = NodeListPtr->item[idx];
				
				CComBSTR text;
				CComBSTR nameString;
			
				pNodeScript->get_baseName(&nameString);
				pNodeScript->get_text(&text);

				CString strNameString(nameString);
				CString strText(text);

				nameString.Empty();
				text.Empty();		

				if (strNameString == _T("INCLUDE"))
				{
					POSITION pos = pWorkForm->WorkSheetList.GetHeadPosition();
					while (pos)
					{
						CSIReportWorkSheet *pSheet = (CSIReportWorkSheet*)pWorkForm->WorkSheetList.GetNext(pos);
						if (pSheet == NULL) continue;

						if (pSheet->Alias == CString(strText))
							pWorkForm->IncludeList.AddTail((CObject*)pSheet);
					}
				}

				pNodeScript.Release();
			}
			NodeListPtr.Release();

			return;
		}
		else
		{
			nameString.Empty();
			text.Empty();

			IXMLDOMNodeListPtr pChlidList = pNode->childNodes;
			FetchScript(pChlidList, pWorkForm);
			pChlidList.Release();
		}
	}

EFS_END
}

VOID CSIReportXMLIOMgr::FetchWorkSheet(IXMLDOMNodePtr pNode, CSIReportWorkSheet* pSheet)
{
EFS_BEGIN

	CSIReportObject *pObject;
	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;

	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr pSheetNode = NodeListPtr->item[idx];

		CComBSTR text;
		CComBSTR nameString;		
		TESTHR(pSheetNode->get_baseName(&nameString));
		TESTHR(pSheetNode->get_text(&text));
		
		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("IDX"))
			pSheet->Idx = _ttoi(strText);
		else if (strNameString == _T("ALIAS"))
			pSheet->Alias = strText;
		else if (strNameString == _T("DEFAULT_DM"))
			pSheet->m_strDefaultDMAlias = strText;			
		else if (strNameString == _T("MASTER_SHEET"))
			pSheet->master_sheet = _ttoi(strText);
		// Object fetch
		else if (strNameString == _T("OBJECT_LINE"))
		{
			pObject = new CSIReportObject_Line;
			FetchLine(pSheetNode, (CSIReportObject_Line*)pObject);
			pSheet->AddNewObject(pObject);
		}
		else if (strNameString == _T("OBJECT_TEXT_BOX"))
		{
			pObject = new CSIReportObject_Text_Box;
			FetchTextBox(pSheetNode, (CSIReportObject_Text_Box*)pObject);
			pSheet->AddNewObject(pObject);
		}
		else if (strNameString == _T("OBJECT_BITMAP"))
		{
			pObject = new CSIReportObject_Bitmap;
			FetchBitmap(pSheetNode, (CSIReportObject_Bitmap*)pObject);
			pSheet->AddNewObject(pObject);
		}
		else if (strNameString == _T("OBJECT_BARCODE"))
		{
			pObject = new CSIReportObject_Barcode;
			FetchBarcode(pSheetNode, (CSIReportObject_Barcode*)pObject);
			pSheet->AddNewObject(pObject);
		}
		else if (strNameString == _T("OBJECT_TABLE"))
		{
			pObject = new CSIReportObject_Table;
			FetchTable(pSheetNode, (CSIReportObject_Table*)pObject);
			pSheet->AddNewObject(pObject);
		}

		pSheetNode.Release();
	}

	NodeListPtr.Release();

EFS_END
}

VOID CSIReportXMLIOMgr::FetchLine(IXMLDOMNodePtr pNode, CSIReportObject_Line *pObject)
{
EFS_BEGIN

	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;

	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr  pObjectNode = NodeListPtr->item[idx];
		
		CComBSTR text;
		CComBSTR nameString;

		TESTHR(pObjectNode->get_baseName(&nameString));
		TESTHR(pObjectNode->get_text(&text));
		
		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("NAME"))
			pObject->m_strName = strText;
		else if (strNameString == _T("RECT"))
			pObject->SetRect(FetchRect(pObjectNode));
		else if (strNameString == _T("PARENT_NAME"))
			pObject->m_strParentName = strText;
		else if (strNameString == _T("BLOCK_ROW"))
			pObject->block_row = _ttoi(strText);
		else if (strNameString == _T("BLOCK_COL"))
			pObject->block_col = _ttoi(strText);
		else if (strNameString == _T("VISIBLE"))
			pObject->visible = _ttoi(strText);
		else if (strNameString == _T("THICK"))
			pObject->thick = _ttoi(strText);
		else if (strNameString == _T("STYLE"))
			pObject->style = _ttoi(strText);

		pObjectNode.Release();
	}

	NodeListPtr.Release();

EFS_END
}

VOID CSIReportXMLIOMgr::FetchTextBox(IXMLDOMNodePtr pNode, CSIReportObject_Text_Box* pObject)
{
EFS_BEGIN

	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;

	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr  pObjectNode = NodeListPtr->item[idx];
				
		CComBSTR text;
		CComBSTR nameString;
		
		TESTHR(pObjectNode->get_baseName(&nameString));
		TESTHR(pObjectNode->get_text(&text));
		
		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("NAME"))
			pObject->m_strName = strText;
		else if (strNameString == _T("RECT"))
			pObject->SetRect(FetchRect(pObjectNode));
		else if (strNameString == _T("PARENT_NAME"))
			pObject->m_strParentName = strText;
		else if (strNameString == _T("FOCUS"))
			pObject->focus = _ttoi(strText);
		else if (strNameString == _T("FILL_COLOR"))
			FetchColor(pObjectNode, pObject->fill_color);
		else if (strNameString == _T("CHAR_LEN"))
			pObject->char_len = _ttoi(strText);
		else if (strNameString == _T("CHAR_MAX"))
			pObject->char_max = _ttoi(strText);
		else if (strNameString == _T("INDENT_LEN"))
			pObject->indent_len = _ttoi(strText);
		else if (strNameString == _T("VERT_LINE"))
			pObject->vert_line = _ttoi(strText);
		else if (strNameString == _T("HORZ_LINE"))
			pObject->horz_line = _ttoi(strText);
		else if (strNameString == _T("INDENT_LEN"))
			pObject->indent_len = _ttoi(strText);
		else if (strNameString == _T("HORZ_ALIGN"))
			pObject->horz_align = _ttoi(strText);
		else if (strNameString == _T("VERT_ALIGN"))
			pObject->vert_align = _ttoi(strText);
		else if (strNameString == _T("TEXT"))
			FetchText(pObjectNode, pObject->text);
		else if (strNameString == _T("ONLYSHOWFIRSTPAGE"))
			pObject->m_nOnlyShowFirstPage = _ttoi(strText);

		pObjectNode.Release();
	}

	NodeListPtr.Release();

EFS_END
}

VOID CSIReportXMLIOMgr::FetchBitmap(IXMLDOMNodePtr pNode, CSIReportObject_Bitmap* pObject)
{
EFS_BEGIN

	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;

	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr  pObjectNode = NodeListPtr->item[idx];

		CComBSTR text;
		CComBSTR nameString;
			
		TESTHR(pObjectNode->get_baseName(&nameString));
		TESTHR(pObjectNode->get_text(&text));

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("NAME"))
		{
			pObject->m_strName = strText;
			CString strBmpFileName;
			if (MASTER_FLAG == MASTER_IS_EDITOR) 
			{
				strBmpFileName.Format(_T("%s\\%s.bmp"), EDITOR_TEMP_DIR, strText);
			} else {
				strBmpFileName.Format(_T("%s\\%s.bmp"), MANAGER_TEMP_DIR, strText);
			}
			pObject->file_name = strBmpFileName;
		}
		else if (strNameString == _T("RECT"))
			pObject->SetRect(FetchRect(pObjectNode));
		else if (strNameString == _T("PARENT_NAME"))
			pObject->m_strParentName = strText;
		else if (strNameString == _T("BLOCK_ROW"))
			pObject->block_row = _ttoi(strText);
		else if (strNameString == _T("BLOCK_COL"))
			pObject->block_col = _ttoi(strText);
		else if (strNameString == _T("VISIBLE"))
			pObject->visible = _ttoi(strText);
		
		pObjectNode.Release();
	}

	NodeListPtr.Release();

EFS_END
}

VOID CSIReportXMLIOMgr::FetchBarcode(IXMLDOMNodePtr pNode, CSIReportObject_Barcode* pObject)
{
EFS_BEGIN

	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;

	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr  pObjectNode = NodeListPtr->item[idx];
		
		CComBSTR text;
		CComBSTR nameString;

		TESTHR(pObjectNode->get_baseName(&nameString));
		TESTHR(pObjectNode->get_text(&text));

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("NAME"))
			pObject->m_strName = strText;
		else if (strNameString == _T("RECT"))
			pObject->SetRect(FetchRect(pObjectNode));
		else if (strNameString == _T("PARENT_NAME"))
			pObject->m_strParentName = strText;
		else if (strNameString == _T("STYLE"))
			pObject->SetStyle(_ttoi(strText));
		else if (strNameString == _T("CAPTION"))
			pObject->SetCaption(strText);

		pObjectNode.Release();
	}
	NodeListPtr.Release();

EFS_END
}

VOID CSIReportXMLIOMgr::FetchTable(IXMLDOMNodePtr pNode, CSIReportObject_Table* pObject)
{
EFS_BEGIN

	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;

	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr  pTableNode = NodeListPtr->item[idx];

		CComBSTR text;
		CComBSTR nameString;
			
		TESTHR(pTableNode->get_baseName(&nameString));
		TESTHR(pTableNode->get_text(&text));

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("NAME"))
			pObject->m_strName = strText;
		else if (strNameString == _T("RECT"))
			pObject->SetRect(FetchRect(pTableNode));
		else if (strNameString == _T("PARENT_NAME"))
			pObject->m_strParentName = strText;
		else if (strNameString == _T("BLOCK_ROW"))
			pObject->block_row = _ttoi(strText);
		else if (strNameString == _T("BLOCK_COL"))
			pObject->block_col = _ttoi(strText);
		else if (strNameString == _T("VISIBLE"))
			pObject->visible = _ttoi(strText);
		else if (strNameString == _T("AREA"))
			pObject->area = _ttoi(strText);
		else if (strNameString == _T("CURSOR_ROW"))
			pObject->cursor_row = _ttoi(strText);
		else if (strNameString == _T("CURSOR_COL"))
			pObject->cursor_col = _ttoi(strText);
		else if (strNameString == _T("BIND"))
			pObject->bind = _ttoi(strText);
		else if (strNameString == _T("ANCHOR_ROW"))
			pObject->anchor_row = _ttoi(strText);
		else if (strNameString == _T("ANCHOR_COL"))
			pObject->anchor_col = _ttoi(strText);
		else if (strNameString == _T("HORZ_LINE"))
			pObject->horz_line = _ttoi(strText);
		else if (strNameString == _T("VERT_LINE"))
			pObject->vert_line = _ttoi(strText);
		else if (strNameString == _T("LINE_MAX"))
			pObject->line_max = _ttoi(strText);
		else if (strNameString == _T("ROW_COUNT"))
			pObject->row_count = _ttoi(strText);
		else if (strNameString == _T("COL_COUNT"))
			pObject->col_count = _ttoi(strText);
		else if (strNameString == _T("REPEAT"))
			pObject->repeat = _ttoi(strText);
		else if (strNameString == _T("PART_SUM"))
			pObject->part_sum = _ttoi(strText);
		else if (strNameString == _T("TOTAL_SUM"))
			pObject->total_sum = _ttoi(strText);
		else if (strNameString == _T("CELL"))
		{
			CSIReportObject_Table_Cell *pCell = new CSIReportObject_Table_Cell;
			FetchTableCell(pTableNode, pObject, pCell);
			pObject->AddNewCell(pCell);
		}

		pTableNode.Release();
	}

	NodeListPtr.Release();

EFS_END
}

VOID CSIReportXMLIOMgr::FetchTableCell(IXMLDOMNodePtr pNode, CSIReportObject_Table *pTable, CSIReportObject_Table_Cell* pCell)
{
EFS_BEGIN

	pCell->SetParentObject(pTable);

	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;

	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr  pCellNode = NodeListPtr->item[idx];

		CComBSTR text;
		CComBSTR nameString;
			
		TESTHR(pCellNode->get_baseName(&nameString));
		TESTHR(pCellNode->get_text(&text));

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("RECT"))
			pCell->SetRect(FetchRect(pCellNode));
		else if (strNameString == _T("FOCUS"))
			pCell->focus = _ttoi(strText);
		else if (strNameString == _T("FILL_COLOR"))
			FetchColor(pCellNode, pCell->fill_color);
		else if (strNameString == _T("CHAR_LEN"))
			pCell->char_len = _ttoi(strText);
		else if (strNameString == _T("CHAR_MAX"))
			pCell->char_max = _ttoi(strText);
		else if (strNameString == _T("INDENT_LEN"))
			pCell->indent_len = _ttoi(strText);
		else if (strNameString == _T("HORZ_ALIGN"))
			pCell->horz_align = _ttoi(strText);
		else if (strNameString == _T("VERT_ALIGN"))
			pCell->vert_align = _ttoi(strText);
		else if (strNameString == _T("DATA_MGR_NAME"))
			pCell->data_mgr_name = strText;
		else if (strNameString == _T("DATA_FIELD"))
			pCell->data_field = strText;
		else if (strNameString == _T("DATA_FIELD_USER"))
			pCell->data_field_user = strText;
		else if (strNameString == _T("ORIENTATION"))
			pCell->orientation = _ttoi(strText);
		else if (strNameString == _T("SLASH"))
			pCell->slash = _ttoi(strText);
		else if (strNameString == _T("BIND_STATUS"))
			pCell->bind_status = _ttoi(strText);
		else if (strNameString == _T("BIND_ROW"))
			pCell->bind_row = _ttoi(strText);
		else if (strNameString == _T("BIND_COL"))
			pCell->bind_col = _ttoi(strText);
		else if (strNameString == _T("ORIGIN_WIDTH"))
			pCell->orign_width = _ttoi(strText);
		else if (strNameString == _T("ORIGIN_HEIGHT"))
			pCell->orign_height = _ttoi(strText);
		else if (strNameString == _T("CHILD_COUNT"))
			pCell->child_count = _ttoi(strText);
		else if (strNameString == _T("CHILD_NAME"))
			pCell->child_name[0] = strText;
		else if (strNameString == _T("PART_SUM"))
			pCell->part_sum = _ttoi(strText);
		else if (strNameString == _T("TOTAL_SUM"))
			pCell->total_sum = _ttoi(strText);
		else if (strNameString == _T("TEXT"))
			FetchText(pCellNode, pCell->text);
		
		pCellNode.Release();		
	}

	NodeListPtr.Release();
	pNode.Release();

EFS_END
}

VOID CSIReportXMLIOMgr::FetchColor(IXMLDOMNodePtr pNode, COLORREF &rColor)
{
EFS_BEGIN

	INT nRed = 255;
	INT nGreen = 255;
	INT nBlue = 255;

	IXMLDOMNamedNodeMapPtr pNodeMap = pNode->attributes;
	
	for (LONG idx = 0; idx < pNodeMap->length; idx++)
	{
		IXMLDOMNodePtr pItemNode;
		pNodeMap->get_item(idx, &pItemNode);

		CComBSTR text;
		CComBSTR nameString;
		
		TESTHR(pItemNode->get_baseName(&nameString));
		TESTHR(pItemNode->get_text(&text));

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("R"))
			nRed = _ttoi(strText);
		else if (strNameString == _T("G"))
			nGreen = _ttoi(strText);
		else if (strNameString == _T("B"))
			nBlue = _ttoi(strText);

		pItemNode.Release();
	}

	pNodeMap.Release();
	pNode.Release();
	
	rColor = RGB(nRed, nGreen, nBlue);

EFS_END
}

CRect CSIReportXMLIOMgr::FetchRect(IXMLDOMNodePtr pNode)
{
	CRect rect;

	IXMLDOMNamedNodeMapPtr pNodeMap = pNode->attributes;
	for (LONG idx = 0; idx < pNodeMap->length; idx++)
	{
		IXMLDOMNodePtr pItemNode;
		pNodeMap->get_item(idx, &pItemNode);

		CComBSTR text;
		CComBSTR nameString;
		
		TESTHR(pItemNode->get_baseName(&nameString));
		TESTHR(pItemNode->get_text(&text));

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("LEFT"))
			rect.left = _ttoi(strText);
		else if (strNameString == _T("TOP"))
			rect.top = _ttoi(strText);
		else if (strNameString == _T("RIGHT"))
			rect.right = _ttoi(strText);
		else if (strNameString == _T("BOTTOM"))
			rect.bottom = _ttoi(strText);

		pItemNode.Release();
	}

	pNodeMap.Release();
	pNode.Release();

	return rect;
}

VOID CSIReportXMLIOMgr::FetchText(IXMLDOMNodePtr pNode, CSIReportObject_Text &rText)
{
EFS_BEGIN

	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;

	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr  pTextNode = NodeListPtr->item[idx];
		
		CComBSTR text;
		CComBSTR nameString;
		
		TESTHR(pTextNode->get_baseName(&nameString));
		TESTHR(pTextNode->get_text(&text));

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();
		
		if (strNameString == _T("TEXT"))
		{
			CString strTemp = strText;
			INT nStrLength = strText.GetLength();
			TCHAR cFIRST, cLAST;
			if (nStrLength >= 2) {		
				cFIRST = strTemp.GetAt(0);
				cLAST = strTemp.GetAt(nStrLength-1);
				if (cFIRST == 39 && cLAST == 39) {
					strTemp = strText.Mid(1, nStrLength-2);
				}
			}
			strTemp.Replace(_T("\n"), _T("\r\n"));
			rText.text = strTemp;
			/*
			CString strTemp = strText;
			strTemp.Replace(_T("\n"), _T("\r\n"));
			rText.text = strTemp;
			*/
		}
		else if (strNameString == _T("COLOR"))
		{
			FetchColor(pTextNode, rText.color);
		}
		else if (strNameString == _T("SIZE"))
		{
			rText.size = _ttol(strText);
		}
		else if (strNameString == _T("FONT"))
		{
			IXMLDOMNamedNodeMapPtr pNodeMap = pTextNode->attributes;
			
			LOGFONT logfont;
			ZeroMemory(&logfont,sizeof(logfont));

			for (LONG idx = 0; idx < pNodeMap->length; idx++)
			{
				IXMLDOMNodePtr pItemNode;
				pNodeMap->get_item(idx, &pItemNode);

				CComBSTR text;
				CComBSTR nameString;
				
				TESTHR(pItemNode->get_baseName(&nameString));
				TESTHR(pItemNode->get_text(&text));

				CString strNameString(nameString);
				CString strText(text);

				nameString.Empty();
				text.Empty();

				if (strNameString == _T("HEIGHT"))
					logfont.lfHeight = _ttoi(strText);
				else if (strNameString == _T("WIDTH"))
					logfont.lfWidth = _ttoi(strText);
				else if (strNameString == _T("ESCAPEMENT"))
					logfont.lfEscapement = _ttoi(strText);
				else if (strNameString == _T("ORIENTATION"))
					logfont.lfOrientation = _ttoi(strText);
				else if (strNameString == _T("WEIGHT"))
					logfont.lfWeight = _ttoi(strText);
				else if (strNameString == _T("ITALIC"))
					logfont.lfItalic = _ttoi(strText);
				else if (strNameString == _T("UNDERLINE"))
					logfont.lfUnderline = _ttoi(strText);
				else if (strNameString == _T("STRIKEOUT"))
					logfont.lfStrikeOut = _ttoi(strText);
				else if (strNameString == _T("CHARSET"))
					logfont.lfCharSet = _ttoi(strText);
				else if (strNameString == _T("OUTPRECISION"))
					logfont.lfOutPrecision = _ttoi(strText);
				else if (strNameString == _T("CLIPPRECISION"))
					logfont.lfClipPrecision = _ttoi(strText);
				else if (strNameString == _T("QUALITY"))
					logfont.lfQuality = _ttoi(strText);
				else if (strNameString == _T("PITCHANDFAMILY"))
					logfont.lfPitchAndFamily = _ttoi(strText);
				else if (strNameString == _T("FACENAME"))
					_tcscpy(logfont.lfFaceName,strText);

				pItemNode.Release();
			}
			rText.font = logfont;
			pNodeMap.Release();
		}

		pTextNode.Release();
	}

	NodeListPtr.Release();
	pNode.Release();

EFS_END
}

INT CSIReportXMLIOMgr::SaveToXML(CSIReportWorkForm *pWorkForm, CString strFileName)
{
EFS_BEGIN

	//HRESULT hr = CoInitialize(NULL);
	IXMLDOMDocumentPtr pDom (_T("Microsoft.XMLDOM"), NULL, CLSCTX_INPROC_SERVER);
	
	POSITION pos;

	try
	{
		IXMLDOMProcessingInstructionPtr pPI= pDom->createProcessingInstruction(_T("xml"), _T("version='1.0' encoding='utf-8'"));
		pDom->appendChild(pPI);
		
		IXMLDOMElementPtr pRootNode = pDom->createElement(_T("SIREPORT"));

		// Work form
		IXMLDOMElementPtr pWorkFormNode = pDom->createElement(_T("WORKFORM"));
		pWorkFormNode->appendChild(MakeTextElement(pDom, _T("VERSION"), pWorkForm->VERSION));
		pWorkFormNode->appendChild(MakeTextElement(pDom, _T("ALIAS"), pWorkForm->alias));
		pWorkFormNode->appendChild(MakeTextElement(pDom, _T("SHEET_HEIGHT"), pWorkForm->sheet_height));
		pWorkFormNode->appendChild(MakeTextElement(pDom, _T("SHEET_SIZE"), pWorkForm->sheet_size));
		pWorkFormNode->appendChild(MakeTextElement(pDom, _T("PAPER"), pWorkForm->paper));

		pRootNode->appendChild(pWorkFormNode);

		// script
		IXMLDOMElementPtr pScriptNode = pDom->createElement(_T("SCRIPT"));

		pos = pWorkForm->IncludeList.GetHeadPosition();
		while (pos != NULL)
		{
			CSIReportWorkSheet *pSheet = (CSIReportWorkSheet*)pWorkForm->IncludeList.GetNext(pos);
			if (pSheet == NULL) continue;
			
			pScriptNode->appendChild(MakeTextElement(pDom, _T("INCLUDE"), pSheet->Alias));
		}
		pRootNode->appendChild(pScriptNode);

		// work sheet
		pos = pWorkForm->WorkSheetList.GetHeadPosition();
		while (pos != NULL)
		{
			CSIReportWorkSheet *pSheet = (CSIReportWorkSheet*)pWorkForm->WorkSheetList.GetNext(pos);
			if (pSheet == NULL) continue;

			IXMLDOMElementPtr pWorkSheetNode = pDom->createElement(_T("WORKSHEET"));
	
			MakeWorkSheetElement(pDom, pWorkSheetNode, pSheet);

			pRootNode->appendChild(pWorkSheetNode);
		}
				
		pDom->appendChild(pRootNode);

		TESTHR(pDom->save((LPCTSTR)strFileName));

		pRootNode.Release();
		pDom.Release();
	}
	catch(...)
	{
		LONG errCode;
		CComBSTR sourceString, reasonString;
		IXMLDOMParseErrorPtr errObj;
		pDom->get_parseError(&errObj);
		errObj->get_errorCode(&errCode);
		errObj->get_srcText(&sourceString);
		errObj->get_reason(&reasonString);
		CString strResult;
		strResult.Format(_T("Exception occured : %d, %s, %s"), errCode, CString(sourceString), CString(reasonString));
		AfxMessageBox(strResult);
		CoUninitialize();
	}

	return 0;

EFS_END
return -1;

}

VOID CSIReportXMLIOMgr::MakeWorkSheetElement(MSXML2::IXMLDOMDocument *pDom, MSXML2::IXMLDOMElementPtr pWorkSheetNode, CSIReportWorkSheet* pSheet)
{
EFS_BEGIN

	pWorkSheetNode->appendChild(MakeTextElement(pDom, _T("IDX"), pSheet->Idx));
	pWorkSheetNode->appendChild(MakeTextElement(pDom, _T("ALIAS"), pSheet->Alias));
	pWorkSheetNode->appendChild(MakeTextElement(pDom, _T("DEFAULT_DM"), pSheet->m_strDefaultDMAlias));
	pWorkSheetNode->appendChild(MakeTextElement(pDom, _T("MASTER_SHEET"), pSheet->master_sheet));

	// object
	IXMLDOMElementPtr pObjectNode;
	POSITION posObject = pSheet->ObjList.GetHeadPosition();
	while (posObject != NULL)
	{
		CSIReportObject *pObject = (CSIReportObject*)pSheet->ObjList.GetNext(posObject);
		if (pObject == NULL) continue;
		
		pWorkSheetNode->appendChild(MakeObjectElement(pDom, pObject));
	}

EFS_END
}

IXMLDOMElementPtr CSIReportXMLIOMgr::MakeObjectElement(MSXML2::IXMLDOMDocument *pDom, CSIReportObject* pObject)
{
	IXMLDOMElementPtr pObjectNode = NULL;
	if (pObject->GetObjType() == OBJECT_LINE)
	{
		pObjectNode = pDom->createElement(_T("OBJECT_LINE"));
		MakeObjectLineElement(pDom, pObjectNode, (CSIReportObject_Line*)pObject);
	}
	else if (pObject->GetObjType() == OBJECT_TEXT_BOX)
	{
		pObjectNode = pDom->createElement(_T("OBJECT_TEXT_BOX"));
		MakeObjectTextBoxElement(pDom, pObjectNode, (CSIReportObject_Text_Box*)pObject);
	}
	else if (pObject->GetObjType() == OBJECT_BITMAP)
	{
		pObjectNode = pDom->createElement(_T("OBJECT_BITMAP"));
		MakeObjectBitmapElement(pDom, pObjectNode, (CSIReportObject_Bitmap*)pObject);
	}
	else if (pObject->GetObjType() == OBJECT_BARCODE)
	{
		pObjectNode = pDom->createElement(_T("OBJECT_BARCODE"));
		MakeObjectBarcodeElement(pDom, pObjectNode, (CSIReportObject_Barcode*)pObject);
	}
	else if (pObject->GetObjType() == OBJECT_TABLE)
	{
		pObjectNode = pDom->createElement(_T("OBJECT_TABLE"));
		MakeObjectTableElement(pDom, pObjectNode, (CSIReportObject_Table*)pObject);
	}

	return pObjectNode;
}

VOID CSIReportXMLIOMgr::MakeObjectLineElement(MSXML2::IXMLDOMDocument *pDom, MSXML2::IXMLDOMElementPtr pLineNode, CSIReportObject_Line* pLine)
{
EFS_BEGIN

	pLineNode->appendChild(MakeTextElement(pDom, _T("NAME"), pLine->m_strName));
	pLineNode->appendChild(MakeRectElement(pDom, _T("RECT"), pLine->GetRect()));
	pLineNode->appendChild(MakeTextElement(pDom, _T("PARENT_NAME"), pLine->m_strParentName));
	pLineNode->appendChild(MakeTextElement(pDom, _T("BLOCK_ROW"), pLine->block_row));
	pLineNode->appendChild(MakeTextElement(pDom, _T("BLOCK_COL"), pLine->block_col));
	pLineNode->appendChild(MakeTextElement(pDom, _T("VISIBLE"), pLine->visible));
	pLineNode->appendChild(MakeTextElement(pDom, _T("THICK"), pLine->thick));
	pLineNode->appendChild(MakeTextElement(pDom, _T("STYLE"), pLine->style));


EFS_END
}

VOID CSIReportXMLIOMgr::MakeObjectTextBoxElement(MSXML2::IXMLDOMDocument *pDom, MSXML2::IXMLDOMElementPtr pTexBoxNode, CSIReportObject_Text_Box* pTextBox)
{
EFS_BEGIN

	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("NAME"), pTextBox->m_strName));
	pTexBoxNode->appendChild(MakeRectElement(pDom, _T("RECT"), pTextBox->GetRect()));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("PARENT_NAME"), pTextBox->m_strParentName));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("FOCUS"), pTextBox->focus));
	pTexBoxNode->appendChild(MakeColorElement(pDom, _T("FILL_COLOR"), pTextBox->fill_color));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("CHAR_LEN"), pTextBox->char_len));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("CHAR_MAX"), pTextBox->char_max));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("INDENT_LEN"), pTextBox->indent_len));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("VERT_LINE"), pTextBox->vert_line));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("HORZ_LINE"), pTextBox->horz_line));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("HORZ_ALIGN"), pTextBox->horz_align));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("VERT_ALIGN"), pTextBox->vert_align));
	pTexBoxNode->appendChild(MakeTextElement(pDom, _T("ONLYSHOWFIRSTPAGE"), pTextBox->m_nOnlyShowFirstPage));

	// text
	IXMLDOMElementPtr pTextNode = pDom->createElement(_T("TEXT"));
	MakeObjectTextElement(pDom, pTextNode, pTextBox->text);
	pTexBoxNode->appendChild(pTextNode);

EFS_END
}

VOID CSIReportXMLIOMgr::MakeObjectBitmapElement(MSXML2::IXMLDOMDocument *pDom, MSXML2::IXMLDOMElementPtr pBitmapNode, CSIReportObject_Bitmap *pBitmap)
{
EFS_BEGIN

	pBitmapNode->appendChild(MakeTextElement(pDom, _T("NAME"), pBitmap->m_strName));
	pBitmapNode->appendChild(MakeRectElement(pDom, _T("RECT"), pBitmap->GetRect()));
	pBitmapNode->appendChild(MakeTextElement(pDom, _T("PARENT_NAME"), pBitmap->m_strParentName));
	pBitmapNode->appendChild(MakeTextElement(pDom, _T("BLOCK_ROW"), pBitmap->block_row));
	pBitmapNode->appendChild(MakeTextElement(pDom, _T("BLOCK_COL"), pBitmap->block_col));
	pBitmapNode->appendChild(MakeTextElement(pDom, _T("VISIBLE"), pBitmap->visible));
	pBitmapNode->appendChild(MakeTextElement(pDom, _T("FILE_NAME"), pBitmap->file_name));

EFS_END
}

VOID CSIReportXMLIOMgr::MakeObjectBarcodeElement(MSXML2::IXMLDOMDocument *pDom, MSXML2::IXMLDOMElementPtr pBarcodeNode, CSIReportObject_Barcode *pBarcode)
{
EFS_BEGIN

	pBarcodeNode->appendChild(MakeTextElement(pDom, _T("NAME"), pBarcode->m_strName));
	pBarcodeNode->appendChild(MakeRectElement(pDom, _T("RECT"), pBarcode->GetRect()));
	pBarcodeNode->appendChild(MakeTextElement(pDom, _T("PARENT_NAME"), pBarcode->m_strParentName));
	pBarcodeNode->appendChild(MakeTextElement(pDom, _T("STYLE"), pBarcode->m_nStyle));
	pBarcodeNode->appendChild(MakeTextElement(pDom, _T("CAPTION"), pBarcode->m_strCaption));

EFS_END
}

VOID CSIReportXMLIOMgr::MakeObjectTableElement(MSXML2::IXMLDOMDocument *pDom, MSXML2::IXMLDOMElementPtr pTableNode, CSIReportObject_Table* pTable)
{
EFS_BEGIN

	pTableNode->appendChild(MakeTextElement(pDom, _T("NAME"), pTable->m_strName));
	pTableNode->appendChild(MakeRectElement(pDom, _T("RECT"), pTable->GetRect()));
	pTableNode->appendChild(MakeTextElement(pDom, _T("PARENT_NAME"), pTable->m_strParentName));
	pTableNode->appendChild(MakeTextElement(pDom, _T("BLOCK_ROW"), pTable->block_row));
	pTableNode->appendChild(MakeTextElement(pDom, _T("BLOCK_COL"), pTable->block_col));
	pTableNode->appendChild(MakeTextElement(pDom, _T("VISIBLE"), pTable->visible));
	pTableNode->appendChild(MakeTextElement(pDom, _T("AREA"), pTable->area));
	pTableNode->appendChild(MakeTextElement(pDom, _T("CURSOR_ROW"), pTable->cursor_row));
	pTableNode->appendChild(MakeTextElement(pDom, _T("CURSOR_COL"), pTable->cursor_col));
	pTableNode->appendChild(MakeTextElement(pDom, _T("BIND"), pTable->bind));
	pTableNode->appendChild(MakeTextElement(pDom, _T("ANCHOR_ROW"), pTable->anchor_row));
	pTableNode->appendChild(MakeTextElement(pDom, _T("ANCHOR_COL"), pTable->anchor_col));
	pTableNode->appendChild(MakeTextElement(pDom, _T("HORZ_LINE"), pTable->horz_line));
	pTableNode->appendChild(MakeTextElement(pDom, _T("VERT_LINE"), pTable->vert_line));
	pTableNode->appendChild(MakeTextElement(pDom, _T("LINE_EXPAND"), _T("")));
	pTableNode->appendChild(MakeTextElement(pDom, _T("CHAR_CX"), _T("")));
	pTableNode->appendChild(MakeTextElement(pDom, _T("CHAR_CY"), _T("")));
	pTableNode->appendChild(MakeTextElement(pDom, _T("LINE_MAX"), pTable->line_max));
	pTableNode->appendChild(MakeTextElement(pDom, _T("ROW_COUNT"), pTable->row_count));
	pTableNode->appendChild(MakeTextElement(pDom, _T("COL_COUNT"), pTable->col_count));
	pTableNode->appendChild(MakeTextElement(pDom, _T("REPEAT"), pTable->repeat));
	pTableNode->appendChild(MakeTextElement(pDom, _T("PART_SUM"), pTable->part_sum));
	pTableNode->appendChild(MakeTextElement(pDom, _T("TOTAL_SUM"), pTable->total_sum));

	
	// Cell
	POSITION posCell = pTable->CellList.GetHeadPosition();
	while (posCell != NULL)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)pTable->CellList.GetNext(posCell);
		if (pCell == NULL) continue;

		IXMLDOMElementPtr pCellNode = pDom->createElement(_T("CELL"));
		MakeObjectCellElement(pDom, pCellNode, pCell);

		pTableNode->appendChild(pCellNode);
	}
	

EFS_END
}

VOID CSIReportXMLIOMgr::MakeObjectCellElement(MSXML2::IXMLDOMDocument *pDom, MSXML2::IXMLDOMElementPtr pCellNode, CSIReportObject_Table_Cell *pCell)
{
EFS_BEGIN

	pCellNode->appendChild(MakeRectElement(pDom, _T("RECT"), pCell->GetRect()));
	//pCellNode->appendChild(MakeTextElement(pDom, _T("FOCUS"), pCell->focus));
	pCellNode->appendChild(MakeTextElement(pDom, _T("FOCUS"), 0));
	pCellNode->appendChild(MakeColorElement(pDom, _T("FILL_COLOR"), pCell->fill_color));

	// TCHAR len...
	pCellNode->appendChild(MakeTextElement(pDom, _T("CHAR_LEN"), pCell->char_len));
	pCellNode->appendChild(MakeTextElement(pDom, _T("CHAR_MAX"), pCell->char_max));
	pCellNode->appendChild(MakeTextElement(pDom, _T("INDENT_LEN"), pCell->indent_len));
	pCellNode->appendChild(MakeTextElement(pDom, _T("HORZ_ALIGN"), pCell->horz_align));
	pCellNode->appendChild(MakeTextElement(pDom, _T("VERT_ALIGN"), pCell->vert_align));
	pCellNode->appendChild(MakeTextElement(pDom, _T("DATA_MGR_NAME"), pCell->data_mgr_name));
	pCellNode->appendChild(MakeTextElement(pDom, _T("DATA_FIELD"), pCell->data_field));
	pCellNode->appendChild(MakeTextElement(pDom, _T("DATA_FIELD_USER"), pCell->data_field_user));
	pCellNode->appendChild(MakeTextElement(pDom, _T("ORIENTATION"), pCell->orientation));
	pCellNode->appendChild(MakeTextElement(pDom, _T("SLASH"), pCell->slash));
	pCellNode->appendChild(MakeTextElement(pDom, _T("BIND_STATUS"), pCell->bind_status));
	pCellNode->appendChild(MakeTextElement(pDom, _T("BIND_ROW"), pCell->bind_row));
	pCellNode->appendChild(MakeTextElement(pDom, _T("BIND_COL"), pCell->bind_col));
	pCellNode->appendChild(MakeTextElement(pDom, _T("ORIGIN_WIDTH"), pCell->orign_width));
	pCellNode->appendChild(MakeTextElement(pDom, _T("ORIGIN_HEIGHT"), pCell->orign_height));
	pCellNode->appendChild(MakeTextElement(pDom, _T("CHILD_COUNT"), pCell->child_count));
	pCellNode->appendChild(MakeTextElement(pDom, _T("CHILD_NAME"), pCell->child_name[0]));
	pCellNode->appendChild(MakeTextElement(pDom, _T("PART_SUM"), pCell->part_sum));
	pCellNode->appendChild(MakeTextElement(pDom, _T("TOTAL_SUM"), pCell->total_sum));

	// text
	IXMLDOMElementPtr pTextNode = pDom->createElement(_T("TEXT"));
	MakeObjectTextElement(pDom, pTextNode, pCell->text);
	pCellNode->appendChild(pTextNode);

EFS_END
}

VOID CSIReportXMLIOMgr::MakeObjectTextElement(MSXML2::IXMLDOMDocument *pDom, MSXML2::IXMLDOMElementPtr pTextNode, CSIReportObject_Text text)
{
EFS_BEGIN

	CString str;
	str.Format(_T("'%s'"), text.text);
	pTextNode->appendChild(MakeTextElement(pDom, _T("TEXT"), str));
	pTextNode->appendChild(MakeColorElement(pDom, _T("COLOR"), text.color));
	pTextNode->appendChild(MakeTextElement(pDom, _T("SIZE"), text.size));
	pTextNode->appendChild(MakeLogfontElement(pDom, _T("FONT"), text.font));	

EFS_END
}

//Otehr Make Tool
IXMLDOMElementPtr CSIReportXMLIOMgr::MakeTextElement(MSXML2::IXMLDOMDocument *pDom, CString strTagName, INT nData)
{
	CString strData;
	strData.Format(_T("%d"), nData);
	
    return MakeTextElement(pDom, strTagName, strData);
}

IXMLDOMElementPtr CSIReportXMLIOMgr::MakeTextElement(MSXML2::IXMLDOMDocument *pDom, CString strTagName, CString strData)
{
	IXMLDOMElementPtr pNode = pDom->createElement((LPCTSTR)strTagName);
    pNode->appendChild(pDom->createTextNode((LPCTSTR)strData));
    return pNode;
}

IXMLDOMElementPtr CSIReportXMLIOMgr::MakeRectElement(MSXML2::IXMLDOMDocument *pDom, CString strTagName, CRect rect)
{
	IXMLDOMElementPtr pNode = pDom->createElement((LPCTSTR)strTagName);

	pNode->setAttribute(_T("LEFT"), rect.left);
	pNode->setAttribute(_T("TOP"), rect.top);
	pNode->setAttribute(_T("RIGHT"), rect.right);
	pNode->setAttribute(_T("BOTTOM"), rect.bottom);

	return pNode;
}

IXMLDOMElementPtr CSIReportXMLIOMgr::MakeColorElement(MSXML2::IXMLDOMDocument *pDom, CString strTagName, COLORREF color)
{
	IXMLDOMElementPtr pNode = pDom->createElement((LPCTSTR)strTagName);

	pNode->setAttribute(_T("R"), GetRValue(color));
	pNode->setAttribute(_T("G"), GetGValue(color));
	pNode->setAttribute(_T("B"), GetBValue(color));

	return pNode;
}

IXMLDOMElementPtr CSIReportXMLIOMgr::MakeLogfontElement(MSXML2::IXMLDOMDocument *pDom, CString strTagName, LOGFONT logfont)
{
	IXMLDOMElementPtr pNode = pDom->createElement((LPCTSTR)strTagName);
	
	pNode->setAttribute(_T("HEIGHT"), logfont.lfHeight);
	pNode->setAttribute(_T("WIDTH"), logfont.lfWidth);
	pNode->setAttribute(_T("ESCAPEMENT"), logfont.lfEscapement);
	pNode->setAttribute(_T("ORIENTATION"), logfont.lfOrientation);
	pNode->setAttribute(_T("WEIGHT"), logfont.lfWeight);
	pNode->setAttribute(_T("ITALIC"), logfont.lfItalic);
	pNode->setAttribute(_T("UNDERLINE"), logfont.lfUnderline);
	pNode->setAttribute(_T("STRIKEOUT"), logfont.lfStrikeOut);
	pNode->setAttribute(_T("CHARSET"), logfont.lfCharSet);
	pNode->setAttribute(_T("OUTPRECISION"), logfont.lfOutPrecision);
	pNode->setAttribute(_T("CLIPPRECISION"), logfont.lfClipPrecision);
	pNode->setAttribute(_T("QUALITY"), logfont.lfQuality);
	pNode->setAttribute(_T("PITCHANDFAMILY"), logfont.lfPitchAndFamily);
	pNode->setAttribute(_T("FACENAME"), logfont.lfFaceName);
	
	return pNode;
}

INT CSIReportXMLIOMgr::MMtoPixel(DOUBLE dMM)
{
EFS_BEGIN

	return (980 * (INT)dMM) / 297;

EFS_END
return -1;

}

DOUBLE CSIReportXMLIOMgr::PixelToMM(INT nPixel)
{
	return (297.0 * ((double)nPixel / 980.0)) * 100.0;
}


// For Copy & Pase
INT CSIReportXMLIOMgr::CopyObject(CObList* pList, CString &strClipboardString)
{
EFS_BEGIN

	IXMLDOMDocumentPtr pDom (_T("Microsoft.XMLDOM"), NULL, CLSCTX_INPROC_SERVER);

	try
	{
		IXMLDOMProcessingInstructionPtr pPI= pDom->createProcessingInstruction(_T("xml"), _T("version='1.0' encoding='utf-16'"));
		pDom->appendChild(pPI);
		IXMLDOMElementPtr pRootNode = pDom->createElement(_T("OBJECT"));
		
		POSITION pos = pList->GetHeadPosition();
		while (pos != NULL)
		{
			CSIReportObject* pObject = (CSIReportObject*)pList->GetNext(pos);
			if (pObject == NULL) continue;

			pRootNode->appendChild(MakeObjectElement(pDom, pObject));
		}
		
		pDom->appendChild(pRootNode);
		
		CComBSTR text;
		pDom->get_xml(&text);
		strClipboardString = text;
		text.Empty();	

		pRootNode.Release();
		pDom.Release();
	}
	catch(...)
	{
		LONG errCode;
		CComBSTR sourceString, reasonString;
		IXMLDOMParseErrorPtr errObj;
		pDom->get_parseError(&errObj);
		errObj->get_errorCode(&errCode);
		errObj->get_srcText(&sourceString);
		errObj->get_reason(&reasonString);
		CString strResult;
		strResult.Format(_T("Exception occured : %d, %s, %s"), errCode, CString(sourceString), CString(reasonString));
		AfxMessageBox(strResult);
		CoUninitialize();
		return -1;
	}

	return 0;

EFS_END
return -1;

}

INT  CSIReportXMLIOMgr::PastObject(CSIReportWorkSheet* pSheet, CString &strClipboardString)
{
EFS_BEGIN

	IXMLDOMDocumentPtr pDom (_T("Microsoft.XMLDOM"), NULL, CLSCTX_INPROC_SERVER);

	try
	{
		pDom->loadXML(strClipboardString.GetBuffer(0));
		IXMLDOMNodeListPtr NodeListPtr = pDom->childNodes;
						
		for (LONG idx = 0; idx < NodeListPtr->length; idx++)
		{
			IXMLDOMNodePtr pNode = NodeListPtr->item[idx];

			CComBSTR text;
			CComBSTR nameString;
		
			pNode->get_baseName(&nameString);

			CString strName = nameString;
			
			nameString.Empty();
			if (strName == _T("OBJECT"))
			{
				FetchObject(pNode, pSheet);
				pDom.Release();
				pNode.Release();
				return TRUE;
			}

			pNode.Release();
		}

		pDom.Release();
	}
	catch(...)
	{
		LONG errCode;
		CComBSTR sourceString, reasonString;
		IXMLDOMParseErrorPtr errObj;
		pDom->get_parseError(&errObj);
		errObj->get_errorCode(&errCode);
		errObj->get_srcText(&sourceString);
		errObj->get_reason(&reasonString);
		CString strResult;
		strResult.Format(_T("Exception occured : %d, %s, %s"), errCode, CString(sourceString), CString(reasonString));
		AfxMessageBox(strResult);
		CoUninitialize();
	}
	return FALSE;

EFS_END
return -1;

}

VOID CSIReportXMLIOMgr::FetchObject(IXMLDOMNodePtr pNode, CSIReportWorkSheet* pSheet)
{
EFS_BEGIN

	IXMLDOMNodeListPtr NodeListPtr = pNode->childNodes;
	
	CSIReportObject *pObject;
	
	for (LONG idx = 0; idx < NodeListPtr->length; idx++)
	{
		IXMLDOMNodePtr pObjectNode = NodeListPtr->item[idx];
		
		CComBSTR text;
		CComBSTR nameString;

		TESTHR(pObjectNode->get_baseName(&nameString));
		TESTHR(pObjectNode->get_text(&text));

		CString strNameString(nameString);
		CString strText(text);

		nameString.Empty();
		text.Empty();

		if (strNameString == _T("OBJECT_LINE"))
		{
			pObject = new CSIReportObject_Line;
			FetchLine(pObjectNode, (CSIReportObject_Line*)pObject);
			pSheet->AddNewObject(pObject);
		}
		else if (strNameString == _T("OBJECT_TEXT_BOX"))
		{
			pObject = new CSIReportObject_Text_Box;
			FetchTextBox(pObjectNode, (CSIReportObject_Text_Box*)pObject);
			pSheet->AddNewObject(pObject);
		}
		else if (strNameString == _T("OBJECT_BITMAP"))
		{
			pObject = new CSIReportObject_Bitmap;
			FetchBitmap(pObjectNode, (CSIReportObject_Bitmap*)pObject);
			pSheet->AddNewObject(pObject);
		}
		else if (strNameString == _T("OBJECT_BARCODE"))
		{
			pObject = new CSIReportObject_Barcode;
			FetchBarcode(pObjectNode, (CSIReportObject_Barcode*)pObject);
			pSheet->AddNewObject(pObject);
		}
		else if (strNameString == _T("OBJECT_TABLE"))
		{
			pObject = new CSIReportObject_Table;
			FetchTable(pObjectNode, (CSIReportObject_Table*)pObject);
			pSheet->AddNewObject(pObject);
		}

		pObjectNode.Release();
	}

	NodeListPtr.Release();

EFS_END
}

INT CSIReportXMLIOMgr::GetMaxTmpPageNum(VOID)
{
EFS_BEGIN
	CFile fd;
	INT nPages = 0;
	CString strTmp;
	for (INT i = 1; i < 99999; i++) {
		strTmp.Format(_T("rpt_tmp\\page%04d.xml"), i);
		if (fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false) break;
		fd.Close();
		nPages = i;
	}
	return nPages;
EFS_END
return -1;

}
