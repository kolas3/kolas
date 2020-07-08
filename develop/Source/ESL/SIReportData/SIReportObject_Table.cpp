// SIReportObject_Table.cpp: implementation of the CSIReportObject_Table class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObject_Table.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CSIReportObject_Table, CSIReportObject)
CSIReportObject_Table::CSIReportObject_Table()
{
	SetObjType(OBJECT_TABLE);
	
	anchor_col = 0;
	anchor_row = 0;
	area = 1;
	bind = 0;
	block_col = 0;
	block_row = 0;
	cursor_row = 0;
	cursor_col = 0;
	horz_line = 1;
	vert_line = 1;
	line_max = 0;
	
	row_count = 1;
	col_count = 1;

	visible = 1;
	
	part_sum = 0;
	total_sum = 0;

	repeat = 0;

	m_bIsResizingCol = FALSE;
	m_bIsResizingRow = FALSE;
	m_nResizingCol1 = 0;
	m_nResizingCol2 = 0;
	m_nResizingRow1 = 0;
	m_nResizingRow2 = 0;
	
	CellList.RemoveAll();
}

VOID CSIReportObject_Table::Clone(CSIReportObject *pObject)
{
EFS_BEGIN

	CSIReportObject::Clone(pObject);

	((CSIReportObject_Table*)pObject)->anchor_col = anchor_col;
	((CSIReportObject_Table*)pObject)->anchor_row = anchor_row;
	((CSIReportObject_Table*)pObject)->area = area;
	((CSIReportObject_Table*)pObject)->bind = bind;
	((CSIReportObject_Table*)pObject)->block_col = block_col;
	((CSIReportObject_Table*)pObject)->block_row = block_row;
	((CSIReportObject_Table*)pObject)->cursor_row = cursor_row;
	((CSIReportObject_Table*)pObject)->cursor_col = cursor_col;
	((CSIReportObject_Table*)pObject)->horz_line = horz_line;
	((CSIReportObject_Table*)pObject)->vert_line = vert_line;
	((CSIReportObject_Table*)pObject)->line_max = line_max;
	((CSIReportObject_Table*)pObject)->row_count = row_count;
	((CSIReportObject_Table*)pObject)->col_count = col_count;
	((CSIReportObject_Table*)pObject)->visible = visible;
	((CSIReportObject_Table*)pObject)->part_sum = part_sum;
	((CSIReportObject_Table*)pObject)->total_sum = total_sum;

	// Cell~
	CSIReportObject_Table_Cell * pCell;
	for (INT i = 0; i < row_count; i++)
	{
		for (INT j = 0; j < col_count; j++)
		{
			pCell = new CSIReportObject_Table_Cell;
			GetCell(i, j)->Clone(pCell);
			pCell->SetParentObject(pObject);
			((CSIReportObject_Table*)pObject)->CellList.AddTail((CObject*)pCell);
		}
	}

EFS_END
}

CSIReportObject_Table::~CSIReportObject_Table()
{
	FreeCell();
}

INT CSIReportObject_Table::InitCell(INT row, INT col)
{
EFS_BEGIN

	FreeCell();

	INT i, j;
		
	CRect rect = GetRect();

	INT nHeight = rect.Height() / row;
	INT nWidth = rect.Width() / col;
		
	CSIReportObject_Table_Cell * pCell;
	for (i = 0; i < row; i++) {
		
		rect.left = GetRect().left;
		rect.bottom = rect.top + nHeight;
		
		for (j = 0; j < col; j++) {

			rect.right = rect.left + nWidth;
			
			pCell = new CSIReportObject_Table_Cell;
			if (pCell == NULL) continue;

			pCell->Create(rect, NULL, this);
			
			CellList.AddTail((CObject*)pCell);
			
			rect.left = rect.right;
		}
		rect.top = rect.bottom;
	}
	
	CRect rectTmp = GetRect();

	rectTmp.right = rect.right;
	rectTmp.bottom = rect.bottom;

	SetRect(rectTmp);

	row_count = row;
	col_count = col;
	
	return 0;

EFS_END
return -1;

}

INT CSIReportObject_Table::FreeCell(VOID)
{
EFS_BEGIN

	INT count = CellList.GetCount();
	if (count == 0) return 0;
	CSIReportObject_Table_Cell * pData;
	for (INT i = 0; i < count; i++) {
		pData = (CSIReportObject_Table_Cell*)CellList.RemoveHead();
		if (pData) {
			delete pData;
			pData = NULL;
		}
	}
	CellList.RemoveAll();
	row_count = 0;
	col_count = 0;
	return 0;

EFS_END
return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CSIReportObject_Table Column & Row Operation
BOOL CSIReportObject_Table::AddRow()
{
EFS_BEGIN
	CSIReportObject_Table_Cell* pCell;
	CSIReportObject_Table_Cell* pSampleCell;

	for (INT i = 0; i < col_count; i++)
	{
		CRect rect;
		pSampleCell = GetCell(row_count - 1, i);
		if (pSampleCell == NULL) 
		{
			rect = GetRect();
		}
		else
			rect = pSampleCell->GetRect();
		
		INT nHeight = rect.Height();
		rect.top += nHeight;
		rect.bottom += nHeight;

		pCell = NULL;
		pCell = new CSIReportObject_Table_Cell;
		if (pCell == NULL) continue;
		pCell->Create(rect, NULL, this);
		CellList.AddTail((CObject*)pCell);
	}
	row_count++;
	SetRect(GetRect());
	return TRUE;

EFS_END
return FALSE;

}

BOOL CSIReportObject_Table::AddColumn()
{
EFS_BEGIN

	CSIReportObject_Table_Cell* pCell;
	CSIReportObject_Table_Cell* pSampleCell;

	for (INT i = row_count - 1; i >= 0; i--)
	{
		pSampleCell = GetCell(i, col_count - 1);
		if (pSampleCell == NULL) return FALSE;

		CRect rect = pSampleCell->GetRect();
		INT nWidth = rect.Width();
		rect.left += nWidth;
		rect.right += nWidth;

		pCell = new CSIReportObject_Table_Cell;
		if (pCell == NULL) continue;

		pCell->Create(rect, NULL, this);
		
		POSITION pos = CellList.FindIndex((i + 1) * col_count -1);
		if (pos == NULL) return FALSE;

		CellList.InsertAfter(pos, (CObject*)pCell);		
	}

	col_count++;
	SetRect(GetRect());

	return TRUE;

EFS_END
return FALSE;

}

BOOL CSIReportObject_Table::RemoveRow()
{
EFS_BEGIN

	if (row_count == 1) return FALSE;

	CSIReportObject_Table_Cell* pCell;
	for (INT i = col_count - 1; i >= 0 ; i--)
	{
		POSITION pos = CellList.FindIndex( (row_count - 1) * col_count + i);
		if (pos == NULL) return FALSE;
		
		pCell = (CSIReportObject_Table_Cell*)CellList.GetAt(pos);
		CellList.RemoveAt(pos);
		if (pCell != NULL)
		{
			delete pCell;
			pCell = NULL;
		}
	}

	row_count--;
	SetRect(GetRect());
	return TRUE;

EFS_END
return FALSE;

}

BOOL CSIReportObject_Table::RemoveColumn()
{
EFS_BEGIN

	if (col_count == 1) return FALSE;
	
	CSIReportObject_Table_Cell* pCell;
	for (INT i = row_count - 1; i >= 0 ; i--)
	{
		POSITION pos = CellList.FindIndex( (i + 1) * col_count - 1);
		if (pos == NULL) return FALSE;
		
		pCell = (CSIReportObject_Table_Cell*)CellList.GetAt(pos);
		CellList.RemoveAt(pos);
		if (pCell != NULL)
		{
			delete pCell;
			pCell = NULL;
		}
	}

	col_count--;
	SetRect(GetRect());
	
	return TRUE;

EFS_END
return FALSE;

}

INT CSIReportObject_Table::InsertRow()
{
EFS_BEGIN

	INT nRow;
	INT nCol;

	CSIReportObject_Table_Cell* pSampleCell;
	pSampleCell = GetFocusCell(nRow, nCol);
	if (pSampleCell == NULL) return FALSE;

	CRect rectSample = pSampleCell->GetRect();
	INT nHeight = rectSample.Height();

	POSITION pos = CellList.FindIndex(nRow * col_count);
	if (pos == NULL) return FALSE;

	// Get Merge Information
	CCellID MasterCellID;
	CCellID SlaveCellID;

	BOOL bIsMergedTable = GetMergeInformation(MasterCellID, SlaveCellID);
	
	if (bIsMergedTable)
	{
		if (nRow <= SlaveCellID.m_nRow)
		{
			CSIReportObject_Table_Cell* pCellMaster = GetCell(MasterCellID);
			if (pCellMaster != NULL)
			{
				pCellMaster->bind_row++;
			}
		}
	}

	// Insert New Cell
	INT row, col;
	for (col = 0; col < col_count; col++)
	{
		CSIReportObject_Table_Cell* pCellNew = new CSIReportObject_Table_Cell;
		pCellNew->Create(rectSample, NULL, this);

		// Determine Merge State
		CCellID NewCellID(nRow, col);
		if (bIsMergedTable)
		{
			if (MasterCellID <= NewCellID && NewCellID <= SlaveCellID)
			{
				if (MasterCellID.m_nRow != nRow)
				{
					pCellNew->bind_status = 2;
					pCellNew->bind_row = MasterCellID.m_nRow;
					pCellNew->bind_col = MasterCellID.m_nCol;
				}
			}
		}

		CellList.InsertBefore(pos, (CObject*)pCellNew);
	}
	row_count++;

	// Update new Cell Rect
	for (col = 0; col < col_count; col++)
	{
		CSIReportObject_Table_Cell* pCell = GetCell(nRow + 1, col);
		if (pCell == NULL) return FALSE;
		
		CRect rect = pCell->GetRect();

		pCell = GetCell(nRow, col);
		if (pCell == NULL) return FALSE;
		
		pCell->SetRect(rect);		
	}

	// Update exist Cell Rect
	for (row = nRow + 1; row < row_count; row++)
	{
		for (col = 0; col < col_count; col++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(row, col);
			if (pCell == NULL) continue;

			CRect rect = pCell->GetRect();
			
			rect.top += nHeight;
			rect.bottom += nHeight;

			pCell->SetRect(rect);
		}
	}
		
	SetRect(GetRect());	

	return TRUE;

EFS_END
return -1;

}

INT CSIReportObject_Table::InsertColumn()
{
EFS_BEGIN

	INT nRow;
	INT nCol;

	CSIReportObject_Table_Cell* pSampleCell = GetFocusCell(nRow, nCol);
	if (pSampleCell == NULL) return FALSE;

	CRect rectSample = pSampleCell->GetRect();
	INT nWidth = rectSample.Width();

	// Get Merge Information
	CCellID MasterCellID;
	CCellID SlaveCellID;

	BOOL bIsMergedTable = GetMergeInformation(MasterCellID, SlaveCellID);
	
	if (bIsMergedTable)
	{
		if (nCol <= SlaveCellID.m_nCol)
		{
			CSIReportObject_Table_Cell* pCellMaster = GetCell(MasterCellID);
			if (pCellMaster != NULL)
			{
				pCellMaster->bind_col++;
			}
		}
	}

	// Insert New Cell
	INT row, col;
	for ( row = row_count - 1; row >= 0; row--)
	{
		POSITION pos = CellList.FindIndex(row * col_count + nCol);
		if (pos == NULL) return FALSE;

		CSIReportObject_Table_Cell* pCellNew = new CSIReportObject_Table_Cell;
		if (pCellNew == NULL) continue;

		pCellNew->Create(rectSample, NULL, this);

		// Determine Merge State
		CCellID NewCellID(row, nCol);
		if (bIsMergedTable)
		{
			if (MasterCellID <= NewCellID && NewCellID <= SlaveCellID)
			{
				if (MasterCellID.m_nCol != nCol)
				{
					pCellNew->bind_status = 2;
					pCellNew->bind_row = MasterCellID.m_nRow;
					pCellNew->bind_col = MasterCellID.m_nCol;
				}
			}
		}

		CellList.InsertBefore(pos, (CObject*)pCellNew);
	}
	col_count++;

	// Update new Cell Rect
	for (row = 0; row < row_count; row++)
	{
		CSIReportObject_Table_Cell* pCell = GetCell(row, nCol + 1);
		if (pCell == NULL) return FALSE;
		
		CRect rect = pCell->GetRect();

		pCell = GetCell(row, nCol);
		if (pCell == NULL) return FALSE;
		
		pCell->SetRect(rect);		
	}

	// Update exist Cell Rect
	for (col = nCol + 1; col < col_count; col++)
	{
		for (row = 0; row < row_count; row++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(row, col);
			if (pCell == NULL) continue;

			CRect rect = pCell->GetRect();
			
			rect.left += nWidth;
			rect.right += nWidth;

			pCell->SetRect(rect);
		}
	}
		
	SetRect(GetRect());	

	return nCol;	

EFS_END
return -1;

}

INT CSIReportObject_Table::DeleteRow(CArray<CString, CString&> *pArrayChildName /*= NULL*/)
{
EFS_BEGIN

	if (row_count == 1) return FALSE;
	INT nRow;
	INT nCol;
	
	CSIReportObject_Table_Cell* pSampleCell;
	pSampleCell = GetFocusCell(nRow, nCol);
	if (pSampleCell == NULL) return FALSE;

	CRect rectSample = pSampleCell->GetRect();
	INT nHeight = rectSample.Height();


	// Get Merge Information
	CCellID MasterCellID;
	CCellID SlaveCellID;

	BOOL bIsMergedTable = GetMergeInformation(MasterCellID, SlaveCellID);
	
	if (bIsMergedTable)
	{
		if (nRow <= SlaveCellID.m_nRow)
		{
			// Change Master cell's bind_col
			CSIReportObject_Table_Cell* pCellMaster = GetCell(MasterCellID);
			if (pCellMaster != NULL)
			{
				// Delete Master Column
				if (MasterCellID.m_nRow == nRow)
				{
					// Set to master next column
					CSIReportObject_Table_Cell* pCellNext = GetCell(MasterCellID.m_nRow + 1, MasterCellID.m_nCol);
					if (pCellNext != NULL)
					{
						// Update slave cells
						for (INT row = MasterCellID.m_nRow + 1; row < SlaveCellID.m_nRow; row ++)
						{
							for (INT col = MasterCellID.m_nCol; col < SlaveCellID.m_nCol; col++)
							{
								CSIReportObject_Table_Cell* pCellSlave = GetCell(row, col);
								if (pCellSlave != NULL && pCellMaster != pCellSlave)
								{
									pCellSlave->bind_col = MasterCellID.m_nRow + 1;
								}
							}
						}

						// Update Master Cells
						pCellNext->bind_status = 1;
						pCellNext->bind_row = pCellMaster->bind_row - 1;
						pCellNext->bind_col = pCellMaster->bind_col;
					}
				}

				// Other all
				pCellMaster->bind_row--;
			}
		}
	}

	// Update Rect
	for (INT row = nRow + 1; row < row_count; row++)
	{
		for (INT col = 0; col < col_count; col++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(row, col);
			if (pCell == NULL) return FALSE;

			CRect rect = pCell->GetRect();

			rect.top -= nHeight;
			rect.bottom -= nHeight;

			pCell->SetRect(rect);
		}
	}

	// Delete row
	for (INT col = col_count -1; col >= 0; col--)
	{
		POSITION pos = CellList.FindIndex( nRow * col_count + col);
		if (pos == NULL) return FALSE;
		
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetAt(pos);
		CellList.RemoveAt(pos);
		if (pCell != NULL)
		{
			if (pArrayChildName != NULL)
			{
				// Check Has Child
				CString strChildName;
				for (INT i = 0; i < pCell->child_count; i++)
				{
					strChildName = pCell->child_name[i];
					if (!strChildName.IsEmpty())
					{
						pArrayChildName->Add(strChildName);
					}
				}
			}

			delete pCell;
			pCell = NULL;
		}		
	}

	row_count--;
	SetRect(GetRect());
	
	return TRUE;

EFS_END
return -1;

}

INT CSIReportObject_Table::DeleteColumn(CArray<CString, CString&> *pArrayChildName /*= NULL*/)
{
EFS_BEGIN

	if (col_count == 1) return FALSE;

	INT nRow;
	INT nCol;
	
	CSIReportObject_Table_Cell* pSampleCell;
	pSampleCell = GetFocusCell(nRow, nCol);
	if (pSampleCell == NULL) return FALSE;

	CRect rectSample = pSampleCell->GetRect();
	INT nWidth = rectSample.Width();

	// Get Merge Information
	CCellID MasterCellID;
	CCellID SlaveCellID;

	BOOL bIsMergedTable = GetMergeInformation(MasterCellID, SlaveCellID);
	
	if (bIsMergedTable)
	{
		if (nCol <= SlaveCellID.m_nCol)
		{
			// Change Master cell's bind_col
			CSIReportObject_Table_Cell* pCellMaster = GetCell(MasterCellID);
			if (pCellMaster != NULL)
			{
				// Delete Master Column
				if (MasterCellID.m_nCol == nCol)
				{
					// Set to master next column
					CSIReportObject_Table_Cell* pCellNext = GetCell(MasterCellID.m_nRow, MasterCellID.m_nCol + 1);
					if (pCellNext != NULL)
					{
						// Update slave cells
						for (INT row = MasterCellID.m_nRow; row < SlaveCellID.m_nRow; row ++)
						{
							for (INT col = MasterCellID.m_nCol + 1; col < SlaveCellID.m_nCol; col++)
							{
								CSIReportObject_Table_Cell* pCellSlave = GetCell(row, col);
								if (pCellSlave != NULL && pCellMaster != pCellSlave)
								{
									pCellSlave->bind_col = MasterCellID.m_nCol + 1;
								}
							}
						}

						// Update Master Cells
						pCellNext->bind_status = 1;
						pCellNext->bind_row = pCellMaster->bind_row;
						pCellNext->bind_col = pCellMaster->bind_col - 1;
					}
				}

				// Other all
				pCellMaster->bind_col--;
			}
		}
	}

	// Update Rect
	for (INT col = nCol + 1; col < col_count; col++)
	{
		for (INT row = 0; row < row_count; row++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(row, col);
			if (pCell == NULL) return FALSE;

			CRect rect = pCell->GetRect();

			rect.left -= nWidth;
			rect.right -= nWidth;

			pCell->SetRect(rect);
		}
	}

	for (INT row = row_count - 1; row >= 0; row--)
	{
		POSITION pos = CellList.FindIndex( row * col_count + nCol);
		if (pos == NULL) return FALSE;
		
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetAt(pos);
		CellList.RemoveAt(pos);
		if (pCell != NULL)
		{
			if (pArrayChildName != NULL)
			{
				// Check Has Child
				CString strChildName;
				for (INT i = 0; i < pCell->child_count; i++)
				{
					strChildName = pCell->child_name[i];
					if (!strChildName.IsEmpty())
					{
						pArrayChildName->Add(strChildName);
					}
				}
			}

			delete pCell;
			pCell = NULL;
		}
	}

	// Delete Column
	col_count--;
	SetRect(GetRect());
	

	return nCol;

EFS_END
return -1;

}

INT CSIReportObject_Table::ChangeColumn(INT nCol1, INT nCol2)
{
EFS_BEGIN

	if ((nCol1 < 0 || nCol1 > col_count - 1) || (nCol2 < 0 || nCol2 > col_count - 1))
		return -1;
	
	POSITION pos1;
	POSITION pos2;

	CSIReportObject_Table_Cell *pCell1;
	CSIReportObject_Table_Cell *pCell2;

	for (INT row = 0; row < row_count; row++)
	{
		pos1 = CellList.FindIndex(row * col_count + nCol1);
		pos2 = CellList.FindIndex(row * col_count + nCol2);

		if (pos1 == NULL || pos2 == NULL) return -1;

		pCell1 = (CSIReportObject_Table_Cell*)CellList.GetAt(pos1);
		pCell2 = (CSIReportObject_Table_Cell*)CellList.GetAt(pos2);

		if (pCell1 == NULL || pCell2 == NULL) return -1;

		CSIReportObject_Table_Cell cell;
		pCell1->Clone(&cell);
		pCell2->Clone(pCell1);
		cell.Clone(pCell2);
		
		// Restore Not chagne attr
		// rect
		CRect rect = pCell1->GetRect();
		pCell1->SetRect(pCell2->GetRect());
		pCell2->SetRect(rect);

		INT nTemp;

		// bind_row
		nTemp = pCell1->bind_row;
		pCell1->bind_row = pCell2->bind_row;
		pCell2->bind_row = nTemp;

		// bind_col
		nTemp = pCell1->bind_col;
		pCell1->bind_col = pCell2->bind_col;
		pCell2->bind_col = nTemp;

		// bind_status
		nTemp = pCell1->bind_status;
		pCell1->bind_status = pCell2->bind_status;
		pCell2->bind_status = nTemp;

		// child_count
		nTemp = pCell1->child_count;
		pCell1->child_count = pCell2->child_count;
		pCell2->child_count = nTemp;

		// child_name
		CString sTemp;
		sTemp = pCell1->child_name[0];
		pCell1->child_name[0] = pCell2->child_name[0];
		pCell2->child_name[0] = sTemp;

		//CellList.SetAt(pos1, (CObject*)pCell2);
		//CellList.SetAt(pos2, (CObject*)pCell1);
	}

	SetRect(GetRect());

	return 0;

EFS_END
return -1;

}

/////////////////////////////////////////////////////////////////////////////
// CSIReportObject_Table Cell Operation
INT CSIReportObject_Table::AddNewCell(CSIReportObject_Table_Cell *pCell)
{
EFS_BEGIN

	if (!pCell) return -1;
	CellList.AddTail((CObject*)pCell);
	
	return 0;

EFS_END
return -1;

}

CSIReportObject_Table_Cell *CSIReportObject_Table::GetCell(CCellID CellID)
{
EFS_BEGIN

	return GetCell(CellID.m_nRow, CellID.m_nCol);

EFS_END
return NULL;

}

CSIReportObject_Table_Cell *CSIReportObject_Table::GetCell(INT row, INT col)
{	
EFS_BEGIN

	if (row >= row_count || col >= col_count) return NULL;
	
	POSITION pos;
	pos = CellList.FindIndex(row * col_count + col);
	if (pos == NULL) return NULL;

	return (CSIReportObject_Table_Cell *)CellList.GetAt(pos);

EFS_END
return NULL;

}

CSIReportObject_Table_Cell *CSIReportObject_Table::GetCell(POINT point)
{
EFS_BEGIN

	POSITION pos = CellList.GetHeadPosition();
	while(pos)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetNext(pos);
		if (pCell)
		{
			if(pCell->PtInSIReportObject(point) && pCell->bind_status != 2)
				return pCell;
		}
	}

	return NULL;

EFS_END
return NULL;

}

INT	CSIReportObject_Table::GetCellLocation(INT &nRow, INT &nCol, CSIReportObject_Table_Cell* pCell)
{
	if (pCell == NULL) return -1;

	INT row;
	INT col;

	for (row = 0; row < row_count; row++)
	{
		for (col = 0; col < col_count; col++)
		{
			if (pCell == GetCell(row, col))
			{
				nRow = row;
				nCol = col;

				return 0;
			}
		}
	}
	return -1;
}

BOOL CSIReportObject_Table::SetCellData(INT nRow, INT nCol, CSIReportObject_Table_Cell* pCellSource)
{
EFS_BEGIN

	if (pCellSource == NULL) return FALSE;
	
	CSIReportObject_Table_Cell* pCell = GetCell(nRow, nCol);
	if (pCell == NULL) return FALSE;

	pCellSource->Clone(pCell);

	return TRUE;

EFS_END
return FALSE;

}

VOID CSIReportObject_Table::SetCellRect(CRect rectTable)
{
EFS_BEGIN

	INT nHeight = rectTable.Height() / row_count;
	INT nWidth = rectTable.Width() / col_count;

	CRect rect = rectTable;
	
	CSIReportObject_Table_Cell * pCell;
	for (INT row = 0; row < row_count; row++)
	{
		rect.left = GetRect().left;
		rect.bottom = rect.top + nHeight;
		for (INT col = 0; col < col_count; col++)
		{
			rect.right = rect.left + nWidth;
			pCell = GetCell(row, col);
			if (pCell == NULL) return;

			pCell->SetRect(rect);
			rect.left = rect.right;			
		}
		rect.top = rect.bottom;
	}

EFS_END
}

VOID CSIReportObject_Table::ReSetCellFocus()
{
EFS_BEGIN

	POSITION pos = CellList.GetHeadPosition();
	while(pos)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetNext(pos);
		if (pCell)
		{
			pCell->focus = FALSE;
		}
	}

EFS_END
}

INT CSIReportObject_Table::SetCellFocus(POINT point)
{
EFS_BEGIN

	// Check Focus
	BOOL bIsCheckFocus = FALSE;

	CSIReportObject_Table_Cell* pCell = GetCell(point);
	if (pCell != NULL)
	{
		if (pCell->focus) return -1;
		if (pCell->bind_status == 2)
		{
			CSIReportObject_Table_Cell* pMasterCell = GetCell(pCell->bind_row, pCell->bind_col);
			pMasterCell->focus = TRUE;
			bIsCheckFocus = TRUE;
		}
		else
		{
			pCell->focus = TRUE;
			bIsCheckFocus = TRUE;
		}
	}

	if (!bIsCheckFocus) return -1;


	// Set Focus in range of select
	INT nStartRow = 0;
	INT nEndRow = 0;
	INT nStartCol = 0;
	INT nEndCol = 0;

	BOOL bFirstFocus = TRUE;

	INT row;
	INT col;

	// Find Range
	for (row = 0; row < row_count; row++)
	{
		for (col = 0; col < col_count; col++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(row, col);
			if (pCell && pCell->focus)
			{
				if (bFirstFocus)
				{
					nStartRow = row;
					nStartCol = col;

					nEndRow = row;
					nEndCol = col;
				}

				if (!bFirstFocus)
				{
					nStartRow > row ? nStartRow = row : NULL;
					nStartCol > col ? nStartCol = col : NULL;

					nEndRow < row ? nEndRow = row : NULL;
					nEndCol < col ? nEndCol = col : NULL;
				}

				bFirstFocus = FALSE;
			}
		}
	}

	if (bFirstFocus) return 1;

	bIsCheckFocus = FALSE;

	// Set range to focused
	for (row = nStartRow; row <= nEndRow; row++)
	{
		for (col = nStartCol; col <= nEndCol; col++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(row, col);
			if (pCell)
			{
				pCell->focus =TRUE;
				bIsCheckFocus = TRUE;
			}
		}
	}

	if (bIsCheckFocus) 
		return 0;
	else
		return -1;

EFS_END
return -1;

}

CSIReportObject_Table_Cell *CSIReportObject_Table::GetFocusCell()
{
EFS_BEGIN

	INT nRow;
	INT nCol;
	
	return GetFocusCell(nRow, nCol);

EFS_END
return NULL;

}

CSIReportObject_Table_Cell* CSIReportObject_Table::GetFocusCell(INT &nRow, INT &nCol)
{
EFS_BEGIN

	for (INT i = 0; i < row_count; i++)
	{
		for (INT j = 0; j < col_count; j++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(i, j);
			if (pCell != NULL && pCell->focus)
			{
				nRow = i;
				nCol = j;
				return pCell;
			}
		}
	}
	return NULL;

EFS_END
return NULL;

}

VOID CSIReportObject_Table::MoveCell(UINT nChar, INT nPixcel /* = 1 */)
{
EFS_BEGIN

	if (GetFocusCell() != NULL) 
	{
		MoveCellFocus(nChar);
		return;
	}
	
	POSITION posCell = CellList.GetHeadPosition();
	while (posCell != NULL)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetNext(posCell);
		if (pCell == NULL) continue;

		CRect rect = pCell->GetRect();

		switch(nChar)
		{
		case VK_LEFT:
			rect.left -= nPixcel;
			rect.right -= nPixcel;
			break;
		case VK_RIGHT:
			rect.left += nPixcel;
			rect.right += nPixcel;
			break;
		case VK_UP:
			rect.top -= nPixcel;
			rect.bottom -= nPixcel;
			break;
		case VK_DOWN:
			rect.top += nPixcel;
			rect.bottom += nPixcel;
			break;
		}
		pCell->SetRect(rect);
	}

EFS_END
}

VOID CSIReportObject_Table::MoveCellFocus(UINT nChar)
{
EFS_BEGIN

	INT nRow;
	INT nCol;
		
	CSIReportObject_Table_Cell *pCell = GetFocusCell(nRow, nCol);
	// move cell focus
	if ( pCell != NULL)
	{
		pCell->focus = FALSE;
		switch(nChar)
		{
			case VK_LEFT:
				if (nCol == 0)
				{
					nCol = col_count -1;
					if (nRow == 0)
						nRow = row_count - 1;
					else
						nRow--;
				}
				else
					nCol--;
				break;
			case VK_RIGHT:
				if (nCol == col_count - 1)
				{
					nCol = 0;
					if (nRow == row_count - 1)
						nRow = 0;
					else
						nRow++;
				}
				else
					nCol++;
				break;
			case VK_UP:
				if (nRow == 0)
					nRow = row_count - 1;
				else
					nRow--;
				break;
			case VK_DOWN:
				if (nRow == row_count -1)
					nRow = 0;
				else
					nRow++;
				break;
		}

		GetCell(nRow, nCol)->focus = TRUE;
	}

EFS_END
}

INT CSIReportObject_Table::GetMergeInformation(CCellID &MasterCellID, CCellID &SlaveCellID)
{
EFS_BEGIN

	for (INT row = 0; row < row_count; row++)
	{
		for (INT col = 0; col < col_count; col++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(row, col);
			if (pCell != NULL) 
			{
				if (pCell->bind_status == 1)
				{
					MasterCellID.m_nRow = row;
					MasterCellID.m_nCol = col;

					SlaveCellID.m_nRow = pCell->bind_row;
					SlaveCellID.m_nCol = pCell->bind_col;

					return TRUE;
				}
			}
		}
	}
	return FALSE;

EFS_END
return -1;

}

VOID CSIReportObject_Table::MergeSelectedCells()
{
EFS_BEGIN

	// Get first forcus rect
	CSIReportObject_Table_Cell* pMasterCell = NULL;
	
	INT nFocusCnt = 0;
	INT row = 0;
	INT col = 0;

	BOOL bFindMasterCell = FALSE;

	for (INT i = 0; i < row_count; i++)
	{
		for (INT j = 0; j < col_count; j++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(i, j);
			if (pCell && pCell->focus)
			{
				if (!bFindMasterCell)
				{
					pMasterCell = pCell;

					if (pMasterCell->bind_status == 1) 
						return;
					pMasterCell->bind_status = 1;
					row = i, col = j;

					nFocusCnt ++;
				}

				if (pCell != pMasterCell && bFindMasterCell)
				{
					if (pCell->bind_status == 1)
					{
						pMasterCell->bind_row = pCell->bind_row;
						pMasterCell->bind_col = pCell->bind_col;
						
					}
					else
					{
						pMasterCell->bind_row = i;
						pMasterCell->bind_col = j;
					}

					pCell->bind_row = row;
					pCell->bind_col = col;

					pCell->bind_status = 2;
					pCell->focus = FALSE;

					nFocusCnt ++;
				}

				bFindMasterCell = TRUE;
			}
		}
	}

	if (nFocusCnt < 2) pMasterCell->bind_status = 0;

EFS_END
}

VOID CSIReportObject_Table::DeMergeSelectedCells()
{
EFS_BEGIN

	for (INT i = 0; i < row_count; i++)
	{
		for (INT j = 0; j < col_count; j++)
		{
			CSIReportObject_Table_Cell* pCell = GetCell(i, j);
			if (pCell && pCell->focus)
			{
				
				if (pCell->bind_status == 1)
				{
					for (INT ii = i; ii <= pCell->bind_row; ii++)
					{
						for (INT jj = j; jj <= pCell->bind_col; jj++)
						{
							CSIReportObject_Table_Cell *pMergedCell = GetCell(ii, jj);
							pMergedCell->bind_status = 0;
							pMergedCell->focus = FALSE;
						}
					}

					pCell->bind_status = 0;
					pCell->focus = TRUE;
				}
			}
		}
	}

EFS_END
}

/////////////////////////////////////////////////////////////////////////////
// CSIReportObject_Table Basic Operation
VOID CSIReportObject_Table::InitDrag(POINT point)
{
EFS_BEGIN

	CSIReportObject::InitDrag(point);

	// Init Cell Dragging
	POSITION pos = CellList.GetHeadPosition();
	while (pos)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetNext(pos);
		if (pCell)
		{
			pCell->InitDrag(point);
		}
	}

EFS_END
}

VOID CSIReportObject_Table::InitResize(INT resizeMode,  POINT point)
{
EFS_BEGIN

	CSIReportObject::InitResize(resizeMode, point);
	
	// Init Cell Resize
	point.x /= col_count;
	point.y /= row_count;

	POSITION pos = CellList.GetHeadPosition();
	while (pos)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetNext(pos);
		if (pCell)
		{
			pCell->InitResize(resizeMode, point);
		}
	}

EFS_END
}

VOID CSIReportObject_Table::SetRect(CRect rect)
{
EFS_BEGIN

	CSIReportObject::SetRect(rect);
	
	
	CRect rectAdjust = rect;
	
	if (m_bIsResizing || m_bIsDragging)
	{
		POSITION pos = CellList.GetHeadPosition();
		while (pos)
		{
			CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetNext(pos);
			if (pCell)
			{
				if (m_bIsResizing)
					pCell->SetRect(pCell->GetResizeRect());
				else if (m_bIsDragging)
					pCell->SetRect(pCell->GetDragRect());
			}
		}
	}
	
	CSIReportObject_Table_Cell *pCell = GetCell(0, 0);
	if (pCell != NULL)
	{
		rectAdjust.left = pCell->GetRect().left;
		rectAdjust.top = pCell->GetRect().top;
	}
	
	pCell = GetCell(row_count - 1, col_count - 1);
	if (pCell != NULL)
	{
		rectAdjust.right = pCell->GetRect().right;
		rectAdjust.bottom = pCell->GetRect().bottom;
	}

	if (m_bIsResizing)
		m_bIsResizing = FALSE;
	else if (m_bIsDragging)
		m_bIsDragging = FALSE;


	CSIReportObject::SetRect(rectAdjust);	

EFS_END
}

INT CSIReportObject_Table::GetMouseMode(POINT point)
{
EFS_BEGIN

	CRect rectClient = GetRect();
	CRect rectWindow = GetRect();
	
	rectClient.DeflateRect(5, 5);
	rectWindow.InflateRect(10, 10);
	
	// MOVE
	if (rectWindow.PtInRect(point) && !rectClient.PtInRect(point))
	{
		return TABLE_MOVE;
	}
	// RESIZING COL
	else if (IsMouseOverColResizeArea(point))
	{
		return RESIZING_COL;
	}
	// RESIZING ROW
	else if (IsMouseOverRowResizeArea(point))
	{
		return RESIZING_ROW;
	}
	// SELECT CELL
	else if (GetRect().PtInRect(point))
	{
		return CELL_OVER;
	}
	
	return MODE_NONE;

EFS_END
return -1;

}

BOOL CSIReportObject_Table::IsMouseOverColResizeArea(POINT point)
{
EFS_BEGIN

	INT nCol1;
	INT nCol2;
	
	return IsMouseOverColResizeArea(point, nCol1, nCol2);

EFS_END
return FALSE;

}
BOOL CSIReportObject_Table::IsMouseOverColResizeArea(POINT point, INT &nCol1, INT &nCol2)
{
EFS_BEGIN

	for (INT i = 0; i < row_count; i++)
	{
		for (INT j = 0; j < col_count; j++)
		{
			CSIReportObject_Table_Cell *pCell = GetCell(i, j);
			if (pCell == NULL) continue;

			CRect rect = pCell->GetRect();

			CRect resizeRect;
			resizeRect.left = rect.right - MOUSE_OVER_AREA;
			resizeRect.top = rect.top;
			resizeRect.bottom = rect.bottom;
			resizeRect.right = rect.right + MOUSE_OVER_AREA;

			if (resizeRect.PtInRect(point) && pCell->bind_status != 2)
			{
				nCol1 = j; 
				nCol2 = j + 1;
				return TRUE;
			}
		}
	}
	return FALSE;

EFS_END
return FALSE;

}
BOOL CSIReportObject_Table::IsMouseOverRowResizeArea(POINT point)
{
EFS_BEGIN

	INT nRow1;
	INT nRow2;
	return IsMouseOverRowResizeArea(point, nRow1, nRow2);

EFS_END
return FALSE;

}
BOOL CSIReportObject_Table::IsMouseOverRowResizeArea(POINT point, INT &nRow1, INT &nRow2)
{
EFS_BEGIN

	for (INT i = 0; i < row_count; i++)
	{
		for (INT j = 0; j < col_count; j++)
		{
			CSIReportObject_Table_Cell *pCell = GetCell(i, j);
			if (pCell == NULL) continue;

			CRect rect = pCell->GetRect();

			CRect resizeRect;
			resizeRect.left = rect.left;
			resizeRect.top = rect.bottom - MOUSE_OVER_AREA;
			resizeRect.bottom = rect.bottom + MOUSE_OVER_AREA;
			resizeRect.right = rect.right;

			if (resizeRect.PtInRect(point) && pCell->bind_status != 2)
			{
				nRow1 = i; 
				nRow2 = i + 1;
				return TRUE;
			}
		}
	}

	return FALSE;

EFS_END
return FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CSIReportObject_Table Draw
VOID CSIReportObject_Table::Draw(CDC *pDC, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	// Cell Drawing
	POSITION pos = CellList.GetHeadPosition();
	while (pos)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetNext(pos);
		if (pCell)
			pCell->Draw(pDC);
	}

EFS_END
}

VOID CSIReportObject_Table::DrawDragImage(CDC *pDC, POINT point, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	CSIReportObject::DrawDragImage(pDC, point, FALSE);
	
	// Draw Cell Drag Image
	POSITION pos = CellList.GetHeadPosition();
	while (pos)
	{
		CSIReportObject_Table_Cell* pCell = (CSIReportObject_Table_Cell*)CellList.GetNext(pos);
		if (pCell)
			pCell->DrawDragImage(pDC, point);
	}

EFS_END
}

VOID CSIReportObject_Table::DrawResizeImage(CDC *pDC, POINT point, BOOL bIsVisible /*= TRUE*/)
{
EFS_BEGIN

	CSIReportObject::DrawResizeImage(pDC, point);

	// Draw Cell Resizing	
	INT row;
	INT col;
	INT nStandard;
		
	// Devide mount by col_count, row_count
	point.x /= col_count;
	point.y /= row_count;
	
	// Resizing
	if (m_ResizeMode == LEFT_MIDDLE ||
		m_ResizeMode == LEFT_TOP ||
		m_ResizeMode == LEFT_BOTTOM)
	{
		for (row = 0; row < row_count; row++)
		{
			nStandard = GetRect().right;
						
			for (col = col_count -1; col >= 0; col--) 
			{
				CSIReportObject_Table_Cell *pCell = GetCell(row, col);
				if (pCell == NULL) return;

				CRect rectResize= pCell->GetResizeRect();
				
				rectResize.right = nStandard;
				rectResize.left = nStandard - pCell->GetResizeRect().Width();
					
				pCell->SetResizeRect(rectResize);
				pCell->DrawResizeImage(pDC, point, FALSE);

				nStandard = pCell->GetResizeRect().left;
			}
		}
	}
	
	if (m_ResizeMode == RIGHT_MIDDLE ||
	    m_ResizeMode == RIGHT_TOP ||
		m_ResizeMode == RIGHT_BOTTOM)
	{
		for (row = 0; row < row_count; row++) 
		{
			nStandard = GetRect().left;
						
			for (col = 0; col < col_count; col++) 
			{
				CSIReportObject_Table_Cell *pCell = GetCell(row, col);
				if (pCell == NULL) return;

				CRect rectResize= pCell->GetResizeRect();
				
				rectResize.left = nStandard;
				rectResize.right = nStandard + pCell->GetResizeRect().Width();
								
				pCell->SetResizeRect(rectResize);
				pCell->DrawResizeImage(pDC, point, FALSE);		
				
				nStandard = pCell->GetResizeRect().right;
			}
		}
	}
	
	if (m_ResizeMode == MIDDLE_TOP ||
		m_ResizeMode == LEFT_TOP ||
		m_ResizeMode == RIGHT_TOP)
	{
		for (col = 0; col < col_count; col++) 
		{
			nStandard = GetRect().bottom;
						
			for (row = row_count - 1; row >= 0 ; row--) 
			{
				CSIReportObject_Table_Cell *pCell = GetCell(row, col);
				if (pCell == NULL) return;

				CRect rectResize = pCell->GetResizeRect();
				
				rectResize.bottom = nStandard;
				rectResize.top = nStandard - pCell->GetResizeRect().Height();
				
				pCell->SetResizeRect(rectResize);
				pCell->DrawResizeImage(pDC, point, FALSE);

				nStandard = pCell->GetResizeRect().top;
			}
		}
	}
	
	if (m_ResizeMode == MIDDLE_BOTTOM ||
		m_ResizeMode == LEFT_BOTTOM ||
		m_ResizeMode == RIGHT_BOTTOM)
	{
		for (col = 0; col < col_count; col++) 
		{
			nStandard = GetRect().top;
						
			for (row = 0; row < row_count; row++) 
			{
				CSIReportObject_Table_Cell *pCell = GetCell(row, col);
				if (pCell == NULL) return;

				CRect rectResize = pCell->GetResizeRect();
				
				rectResize.top = nStandard;
				rectResize.bottom = nStandard + pCell->GetResizeRect().Height();
				
				pCell->SetResizeRect(rectResize);
				pCell->DrawResizeImage(pDC, point, FALSE);

				nStandard = pCell->GetResizeRect().bottom;
			}
		}	
	}

EFS_END
}

VOID CSIReportObject_Table::DrawResizingCol(CDC *pDC, POINT point)
{
EFS_BEGIN

	if (pDC == NULL) return;

	if (!m_bIsResizingCol)
	{
		if (!IsMouseOverColResizeArea(point, m_nResizingCol1, m_nResizingCol2)) 
			return;
		m_pointResizingCol = point;

		// Check Pointer
		CSIReportObject_Table_Cell *pCell1 = GetCell(0, m_nResizingCol1);
		CSIReportObject_Table_Cell *pCell2 = GetCell(row_count - 1, m_nResizingCol1);

		if (pCell1 == NULL || pCell2 == NULL) return;		

		/*

		m_pointTop.x = GetCell(0, m_nResizingCol1)->GetRect().right;
		m_pointTop.y = GetCell(0, m_nResizingCol1)->GetRect().top;

		m_pointBottom.x = GetCell(row_count -1, m_nResizingCol1)->GetRect().right;
		m_pointBottom.y = GetCell(row_count -1, m_nResizingCol1)->GetRect().bottom;

		*/

		m_pointTop.x = pCell1->GetRect().right;
		m_pointTop.y = pCell1->GetRect().top;

		m_pointBottom.x = pCell2->GetRect().right;
		m_pointBottom.y = pCell2->GetRect().bottom;

		m_bIsResizingCol = TRUE;
	}
	else if (m_pointResizingCol != point)
	{
		// Check Pointer
		CSIReportObject_Table_Cell *pCell1 = GetCell(0, m_nResizingCol1);
		CSIReportObject_Table_Cell *pCell2 = GetCell(0, m_nResizingCol2);

		if (pCell1 == NULL || pCell2 == NULL) return;

		// check
		/*
		if ((point.x < GetCell(0, m_nResizingCol1)->GetRect().left + MOUSE_OVER_AREA) 
			|| (point.x > GetCell(0, m_nResizingCol2)->GetRect().right - MOUSE_OVER_AREA))
			return;
			*/
		if ((point.x < pCell1->GetRect().left + MOUSE_OVER_AREA) 
			|| (point.x > pCell2->GetRect().right - MOUSE_OVER_AREA))
			return;

		INT nOldMode = pDC->SetROP2(R2_NOT);
		pDC->MoveTo(m_pointTop);
		pDC->LineTo(m_pointBottom);

		m_pointTop.x = m_pointTop.x - (m_pointResizingCol.x - point.x);
		m_pointBottom.x = m_pointBottom.x - (m_pointResizingCol.x - point.x);

		pDC->MoveTo(m_pointTop);
		pDC->LineTo(m_pointBottom);
		pDC->SetROP2(nOldMode);

		m_pointResizingCol = point;
	}

EFS_END
}

VOID CSIReportObject_Table::DrawResizingRow(CDC *pDC, POINT point)
{
EFS_BEGIN

	if (!m_bIsResizingRow)
	{
		if (!IsMouseOverRowResizeArea(point, m_nResizingRow1, m_nResizingRow2)) 
			return;
		m_pointResizingRow = point;

		// Check Pointer
		CSIReportObject_Table_Cell *pCell1 = GetCell(m_nResizingRow1, 0);
		CSIReportObject_Table_Cell *pCell2 = GetCell(m_nResizingRow1, col_count - 1);
		
		if (pCell1 == NULL || pCell2 == NULL) return;

		m_pointLeft.x = pCell1->GetRect().left;
		m_pointLeft.y = pCell1->GetRect().bottom;

		m_pointRight.x = pCell2->GetRect().right;
		m_pointRight.y = pCell2->GetRect().bottom;

		m_bIsResizingRow = TRUE;				

		/*
		if (GetCell(m_nResizingRow1, 0) == NULL) return;
		if (GetCell(m_nResizingRow1, 0) == NULL) return;
		if (GetCell(m_nResizingRow1, col_count - 1) == NULL) return;
		if (GetCell(m_nResizingRow1, col_count - 1) == NULL) return;
		
		m_pointLeft.x = GetCell(m_nResizingRow1, 0)->GetRect().left;
		m_pointLeft.y = GetCell(m_nResizingRow1, 0)->GetRect().bottom;

		m_pointRight.x = GetCell(m_nResizingRow1, col_count - 1)->GetRect().right;
		m_pointRight.y = GetCell(m_nResizingRow1, col_count - 1)->GetRect().bottom;
		*/
	}
	else if (m_pointResizingRow != point)
	{
		// Check Pointer
		CSIReportObject_Table_Cell *pCell1 = GetCell(m_nResizingRow1, 0);
		CSIReportObject_Table_Cell *pCell2 = GetCell(m_nResizingRow2, 0);
		
		if (pCell1 == NULL || pCell2 == NULL) return;

		// check
		/*
		if ((point.y < GetCell(m_nResizingRow1, 0)->GetRect().top + MOUSE_OVER_AREA) 
			|| (point.y > GetCell(m_nResizingRow2, 0)->GetRect().bottom - MOUSE_OVER_AREA))
			return;
			*/

		if ((point.y < pCell1->GetRect().top + MOUSE_OVER_AREA) 
			|| (point.y > pCell2->GetRect().bottom - MOUSE_OVER_AREA))
			return;


		INT nOldMode = pDC->SetROP2(R2_NOT);
		pDC->MoveTo(m_pointLeft);
		pDC->LineTo(m_pointRight);

		m_pointLeft.y = m_pointLeft.y - (m_pointResizingRow.y - point.y);
		m_pointRight.y = m_pointRight.y - (m_pointResizingRow.y - point.y);

		pDC->MoveTo(m_pointLeft);
		pDC->LineTo(m_pointRight);
		pDC->SetROP2(nOldMode);

		m_pointResizingRow = point;
	}

EFS_END
}

VOID CSIReportObject_Table::DrawResizedCol(CDC *pDC, POINT point)
{
EFS_BEGIN

	if (!m_bIsResizingCol) return;

	for (INT i = 0; i < row_count; i++)
	{
		CRect rectCell;
		
		CSIReportObject_Table_Cell *pCell = GetCell(i, m_nResizingCol1);
		if (pCell == NULL) return;
		
		rectCell = pCell->GetRect();
		rectCell.right = rectCell.right - (rectCell.right - m_pointTop.x);
		pCell->SetRect(rectCell);

		pCell = GetCell(i, m_nResizingCol2);
		if (pCell == NULL) return;

		rectCell = pCell->GetRect();
		rectCell.left = rectCell.left - (rectCell.left - m_pointTop.x);
		pCell->SetRect(rectCell);		
	}

	m_bIsResizingCol = FALSE;

EFS_END
}

VOID CSIReportObject_Table::DrawResizedRow(CDC *pDC, POINT point)
{
EFS_BEGIN

	if (!m_bIsResizingRow) return;

	for (INT i = 0; i < col_count; i++)
	{
		CRect rectCell;
		CSIReportObject_Table_Cell *pCell = GetCell(m_nResizingRow1, i);
		if (pCell == NULL) return;
		
		rectCell = pCell->GetRect();
		rectCell.bottom = rectCell.bottom - (rectCell.bottom - m_pointLeft.y);
		pCell->SetRect(rectCell);

		pCell = GetCell(m_nResizingRow2, i);
		if (pCell == NULL) return;

		rectCell = pCell->GetRect();
		rectCell.top = rectCell.top - (rectCell.top - m_pointLeft.y);
		pCell->SetRect(rectCell);
	}
	
	m_bIsResizingRow = FALSE;

EFS_END
}

VOID CSIReportObject_Table::ResizeCell(UINT nChar, INT nPixcel /* = 1 */)
{
EFS_BEGIN

	INT nRow;
	INT nCol;
	CSIReportObject_Table_Cell *pCell = GetFocusCell(nRow, nCol);
	if (pCell == NULL) return;
	
	/*if (nChar == VK_UP)
	{
		for (INT row = 0; row < nRow; row++)
		{
			for (INT col = 0; col < col_count; col++)
			{
				CSIReportObject_Table_Cell *pResizeCell = GetCell(row, col);
				if (pResizeCell != NULL)
				{
					CRect rect = pResizeCell->GetRect();
					rect.top --;
					rect.bottom --;

					pResizeCell->SetRect(rect);
				}
			}
		}
		for (INT col = 0; col < col_count; col++)
		{
			CSIReportObject_Table_Cell *pResizeCell = GetCell(nRow, col);
			if (pResizeCell != NULL)
			{
				CRect rect = pResizeCell->GetRect();
				rect.top--;
				pResizeCell->SetRect(rect);
			}
		}
	}
	*/
	if (nChar == VK_UP)
	{
		for (INT row = nRow + 1; row < row_count; row++)
		{
			for (INT col = 0; col < col_count; col++)
			{
				CSIReportObject_Table_Cell *pResizeCell = GetCell(row, col);
				if (pResizeCell != NULL)
				{
					CRect rect = pResizeCell->GetRect();
					rect.top -= nPixcel;
					rect.bottom -= nPixcel;

					pResizeCell->SetRect(rect);
				}
			}
		}
		for (INT col = 0; col < col_count; col++)
		{
			CSIReportObject_Table_Cell *pResizeCell = GetCell(nRow, col);
			if (pResizeCell != NULL)
			{
				CRect rect = pResizeCell->GetRect();
				rect.bottom -= nPixcel;
				pResizeCell->SetRect(rect);
			}
		}
	}
	else if (nChar == VK_DOWN)
	{
		for (INT row = nRow + 1; row < row_count; row++)
		{
			for (INT col = 0; col < col_count; col++)
			{
				CSIReportObject_Table_Cell *pResizeCell = GetCell(row, col);
				if (pResizeCell != NULL)
				{
					CRect rect = pResizeCell->GetRect();
					rect.top += nPixcel;
					rect.bottom += nPixcel;

					pResizeCell->SetRect(rect);
				}
			}
		}
		for (INT col = 0; col < col_count; col++)
		{
			CSIReportObject_Table_Cell *pResizeCell = GetCell(nRow, col);
			if (pResizeCell != NULL)
			{
				CRect rect = pResizeCell->GetRect();
				rect.bottom += nPixcel;
				pResizeCell->SetRect(rect);
			}
		}
	}
	else if (nChar == VK_LEFT)
	{
		for (INT col = nCol + 1; col < col_count; col++)
		{
			for (INT row = 0; row < row_count; row++)
			{
				CSIReportObject_Table_Cell *pResizeCell = GetCell(row, col);
				if (pResizeCell != NULL)
				{
					CRect rect = pResizeCell->GetRect();
					rect.left -= nPixcel;
					rect.right -= nPixcel;

					pResizeCell->SetRect(rect);
				}
			}
		}
		for (INT row = 0; row < row_count; row++)
		{
			CSIReportObject_Table_Cell *pResizeCell = GetCell(row, nCol);
			if (pResizeCell != NULL)
			{
				CRect rect = pResizeCell->GetRect();
				rect.right -= nPixcel;
				pResizeCell->SetRect(rect);
			}
		}		
	}
	/*
	else if (nChar == VK_LEFT)
	{
		for (INT col = 0; col < nCol; col++)
		{
			for (INT row = 0; row < row_count; row++)
			{
				CSIReportObject_Table_Cell *pResizeCell = GetCell(row, col);
				if (pResizeCell != NULL)
				{
					CRect rect = pResizeCell->GetRect();
					rect.left--;
					rect.right--;

					pResizeCell->SetRect(rect);
				}
			}
		}
		for (INT row = 0; row < row_count; row++)
		{
			CSIReportObject_Table_Cell *pResizeCell = GetCell(row, nCol);
			if (pResizeCell != NULL)
			{
				CRect rect = pResizeCell->GetRect();
				rect.left--;
				pResizeCell->SetRect(rect);
			}
		}
	}
	*/
	else if (nChar == VK_RIGHT)
	{
		for (INT col = nCol + 1; col < col_count; col++)
		{
			for (INT row = 0; row < row_count; row++)
			{
				CSIReportObject_Table_Cell *pResizeCell = GetCell(row, col);
				if (pResizeCell != NULL)
				{
					CRect rect = pResizeCell->GetRect();
					rect.left++;
					rect.right++;

					pResizeCell->SetRect(rect);
				}
			}
		}
		for (INT row = 0; row < row_count; row++)
		{
			CSIReportObject_Table_Cell *pResizeCell = GetCell(row, nCol);
			if (pResizeCell != NULL)
			{
				CRect rect = pResizeCell->GetRect();
				rect.right++;
				pResizeCell->SetRect(rect);
			}
		}		
	}
	
	SetRect(GetRect());

EFS_END
}

INT CSIReportObject_Table::ReCalcAllRect(INT nLeft, INT nTop)
{
EFS_BEGIN

	m_rect.top += nTop;
	m_rect.bottom += nTop;

	m_rect.left += nLeft;
	m_rect.right += nLeft;

	CSIReportObject_Table_Cell *pCell;
	INT nRow, nCol;
	for (nRow = 0; nRow < row_count; nRow++) {
		for (nCol = 0; nCol < col_count; nCol++) {
			pCell = NULL;
			pCell = GetCell(nRow, nCol);
			if (pCell == NULL) continue;
			pCell->ReCalcAllRect(nLeft, nTop);
		}
	}
	return 0;

EFS_END
return -1;

}

