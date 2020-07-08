// SGroupObject.cpp: implementation of the CSGroupObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPF.h"
#include "SGroupObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSGroupObject::CSGroupObject()
{
	m_Related =0;

    m_colorText = RGB(0,0,0);

	m_logFont.lfHeight = 12;
    m_logFont.lfWidth = 0;
	m_logFont.lfEscapement =0;
	m_logFont.lfOrientation = 0;
	m_logFont.lfWeight = FW_BOLD;
	m_logFont.lfItalic = FALSE;
	m_logFont.lfUnderline = FALSE;
	m_logFont.lfStrikeOut = FALSE;
	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_logFont.lfClipPrecision = OUT_CHARACTER_PRECIS;
    m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;

}

CSGroupObject::~CSGroupObject()
{

}

INT CSGroupObject::DrawObject(CDC *pDC)
{

	INT ids;

	//SET PEN or Brush
	//TRACE(_T("%s\n"),m_GROUP_TYPE);
	
	
	if(m_GROUP_TYPE.GetLength())	
	{
		ids = SetActivatedObject(pDC);
	}
	else if(m_Related)
	{
		ids = SetDrawRelatedObject(pDC);
	}
	else
	{  
		ids = SetDrawObject(pDC);
	}

	/// BODY
	pDC->Rectangle(m_ObjectRect);

	//headerline
	CRect HeaderRect;
	
	HeaderRect = m_ObjectRect; 
	HeaderRect.top = m_ObjectRect.bottom + GROUP_HEADER_SIZE ;
    pDC->Rectangle(HeaderRect);
    
	///headercaption	 
	pDC->SetTextColor(m_colorText);
	pDC->SetBkMode(OPAQUE);
	DrawTextWithFont(pDC, m_ALIAS, HeaderRect, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);

	//DrawPickPoint
	if(m_Selected == 1)
		DrawPickPoint(pDC);

	//RETURN PEN or Brush  
	ids = ReturnDrawObject(pDC);
	
	return 0;

}

INT CSGroupObject::UnDrawObject(CDC *pDC)
{

	INT ids;

	//SET PEN or Brush
	ids = SetUnDrawObject(pDC);

	/// BODY
	pDC->Rectangle(m_ObjectRect);

	/// Header Line	
	CRect Header;
	CPoint Tmp;
	Header= m_ObjectRect;
	Header.top = Header.bottom + GROUP_HEADER_SIZE ; 
	pDC->Rectangle(Header);

	Tmp.x  = Header.left;
	Tmp.y  = Header.top;
	pDC->MoveTo(Tmp);

	Tmp.x  = Header.right;
	pDC->LineTo(Tmp);

	//picpoint
	DrawPickPoint(pDC);

	
	//RETURN PEN or Brush  
    ids = ReturnDrawObject(pDC);


	for(INT i = 0 ; i < m_ObjectList.GetCount() ; i++)
	{

		CSObject * tmp = (CSObject*)m_ObjectList.GetAt(m_ObjectList.FindIndex(i));
		tmp->UnDrawObject(pDC);

	}

	
	return 0;

}

INT CSGroupObject::SetDrawObject(CDC *pDC)
{
    
	m_pen.CreatePen( PS_SOLID , 2 , RGB(0,0,0));
	pOldPen = pDC->SelectObject(&m_pen);
   
	return 0;

}

INT CSGroupObject::SetUnDrawObject(CDC *pDC)
{
    m_pen.CreatePen( PS_SOLID , 3 , RGB(255,255,255));
	pOldPen = pDC->SelectObject(&m_pen);

	return 0;


}
INT CSGroupObject::MoveObject(CPoint Pos)
{

	//좌표 입력 
	m_ObjectRect.top += Pos.y;  
	m_ObjectRect.bottom += Pos.y; 
	m_ObjectRect.left += Pos.x; 
	m_ObjectRect.right += Pos.x;

	for(INT i = 0 ; i < m_ObjectList.GetCount() ; i++)
	{

		CSObject * tmp = (CSObject*)m_ObjectList.GetAt(m_ObjectList.FindIndex(i));
		tmp->MoveObject(Pos);

	}

	return 0;
}

INT CSGroupObject::ReSizeObject(CPoint Pos , INT PickIndex)
{
	INT tmp;

//	TRACE(_T("Pick INDEX: %d , ReSize X:%d , Y:%d \n"),PickIndex, Pos.x , Pos.y);

    switch(PickIndex)
	{
		case 1:

			m_ObjectRect.bottom = Pos.y;
			m_ObjectRect.left = Pos.x;
//			TRACE(_T("ReSize X:%d , Y:%d \n"),  Pos.x , Pos.y);

			if( m_ObjectRect.top < m_ObjectRect.bottom)
			{
				tmp = m_ObjectRect.top;
				m_ObjectRect.top = m_ObjectRect.bottom;
				m_ObjectRect.bottom = tmp;
				
				TRACE(_T("m_ObjectRect.top < m_ObjectRect.bottom \n"));

			}
			if( m_ObjectRect.right < m_ObjectRect.left)
			{
				tmp = m_ObjectRect.left;
				m_ObjectRect.left = m_ObjectRect.right;
				m_ObjectRect.right = tmp;

//				TRACE(_T("m_ObjectRect.right < m_ObjectRect.left \n"));
			
			}

			break;

		case 2:

			m_ObjectRect.bottom = Pos.y;
			m_ObjectRect.right = Pos.x;

			if( m_ObjectRect.top < m_ObjectRect.bottom)
			{
				tmp = m_ObjectRect.top;
				m_ObjectRect.top = m_ObjectRect.bottom;
				m_ObjectRect.bottom = tmp;
				
//				TRACE(_T("m_ObjectRect.top < m_ObjectRect.bottom \n"));

			}
			if( m_ObjectRect.right < m_ObjectRect.left)
			{
				tmp = m_ObjectRect.left;
				m_ObjectRect.left = m_ObjectRect.right;
				m_ObjectRect.right = tmp;

//				TRACE(_T("m_ObjectRect.right < m_ObjectRect.left \n"));
			
			}

			break;
		
		case 3:
			m_ObjectRect.top = Pos.y;
			m_ObjectRect.right = Pos.x;

			if( m_ObjectRect.top < m_ObjectRect.bottom)
			{
				tmp = m_ObjectRect.top;
				m_ObjectRect.top = m_ObjectRect.bottom;
				m_ObjectRect.bottom = tmp;
				
//				TRACE(_T("m_ObjectRect.top < m_ObjectRect.bottom \n"));

			}
			if( m_ObjectRect.right < m_ObjectRect.left)
			{
				tmp = m_ObjectRect.left;
				m_ObjectRect.left = m_ObjectRect.right;
				m_ObjectRect.right = tmp;

//				TRACE(_T("m_ObjectRect.right < m_ObjectRect.left \n"));
			
			}

			break;
		
		case 4:
			m_ObjectRect.top = Pos.y;
			m_ObjectRect.left = Pos.x;
			if( m_ObjectRect.top < m_ObjectRect.bottom)
			{
				tmp = m_ObjectRect.top;
				m_ObjectRect.top = m_ObjectRect.bottom;
				m_ObjectRect.bottom = tmp;
				
//				TRACE(_T("m_ObjectRect.top < m_ObjectRect.bottom \n"));

			}
			if( m_ObjectRect.right < m_ObjectRect.left)
			{
				tmp = m_ObjectRect.left;
				m_ObjectRect.left = m_ObjectRect.right;
				m_ObjectRect.right = tmp;

//				TRACE(_T("m_ObjectRect.right < m_ObjectRect.left \n"));
			
			}

			break;
	}


    return 0;

}

INT CSGroupObject::IsPickPoint(CPoint Cur_Pos)
{
    INT x,y;
	CRect rect;

	//1	
	x = m_ObjectRect.left;
	y = m_ObjectRect.bottom;

   if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 1;
	}

	//2
	x = m_ObjectRect.right;
	y = m_ObjectRect.bottom;

    if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 2;
	}

	//3
	x = m_ObjectRect.right;
	y = m_ObjectRect.top;
     
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 3;
	}
	//4
	x = m_ObjectRect.left;
	y = m_ObjectRect.top;
   
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 4;
	}

	return 0;
}

INT CSGroupObject::IsSeleted(CPoint Cur_Pos)
{
	m_Selected = 0;

	if( m_ObjectRect.left < Cur_Pos.x && Cur_Pos.x < m_ObjectRect.right)
		{
			if( m_ObjectRect.bottom < Cur_Pos.y && Cur_Pos.y < m_ObjectRect.bottom + GROUP_HEADER_SIZE )
			{
				m_Selected = 1;
				MakeObjList();
			}
		}
	
	return m_Selected;
}

INT CSGroupObject::MakeObjList()
{

	m_ObjectList.RemoveAll();

	for(INT i=0; i < m_List->GetCount(); i++)
	{
		CSObject *tmp;
	
		tmp =(CSObject *)m_List->GetAt(m_List->FindIndex(i));
		
		///Group Path
		if( tmp->m_PARENT_GROUP_ALIAS == m_ALIAS )
		{
			tmp->MakeObjList();
			m_ObjectList.AddHead((CObject*)tmp);
		}
		///State Path
		if( tmp->m_GROUP_ALIAS == m_ALIAS )
		{
			tmp->MakeObjList();
			m_ObjectList.AddHead((CObject*)tmp);
		}



	}


	return 0;

}

INT CSGroupObject::DrawPickPoint(CDC *pDC)
{
	INT x,y;
	CRect rect;
	//1	
	x = m_ObjectRect.left;
	y = m_ObjectRect.top;

	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;


	pDC->Rectangle(rect);

	//2
	x = m_ObjectRect.left;
	y = m_ObjectRect.bottom ;

	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;


	pDC->Rectangle(rect);


	//3
	x = m_ObjectRect.right;
	y = m_ObjectRect.top;

	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;


	pDC->Rectangle(rect);


	//4
	x = m_ObjectRect.right;
	y = m_ObjectRect.bottom;

	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;


	pDC->Rectangle(rect);


	return 0;
}
INT CSGroupObject::SetAllEnv(CPoint Cur_Pos)
{

   	CSGroupObject * tmp;
    INT RelatedFlag = 0;

	for( INT i =0 ; i < m_List->GetCount() ; i++)
	{
		tmp =(CSGroupObject *)m_List->GetAt(m_List->FindIndex(i));
		
		if(tmp->m_ObjectType == 'G')
		{
		
			if( tmp->m_ALIAS != m_ALIAS)
			{
	       			if( tmp->IsRelated(Cur_Pos) )
					{
						RelatedFlag = 1;
						if( m_PARENT_GROUP_ALIAS != tmp->m_ALIAS || m_PARENT_GROUP_ALIAS == _T("")  )
						{
							if ( AfxMessageBox(_T("선택하신 Group에 추가 하시겠습니까?"), MB_YESNO) == IDYES )
							{
								m_PARENT_GROUP_ALIAS = tmp->m_ALIAS;
								SaveToDM();
							}
						}
						 
					}
					
					tmp->m_Related  = 0;
					m_Related  = 0;
			}
		}
	}

    if(RelatedFlag == 0 && m_PARENT_GROUP_ALIAS.GetLength() != 0)
	{
		if ( AfxMessageBox(_T("Group에서 제거 하시겠습니까?"), MB_YESNO) == IDYES )
		{
			m_PARENT_GROUP_ALIAS =_T("");
			SaveToDM();
		}
	}

	return 0;
}

INT CSGroupObject::CheckAllEnv(CPoint point)
{

	CSGroupObject * tmp;

	for( INT i =0 ; i < m_List->GetCount() ; i++)
	{
		tmp =(CSGroupObject *)m_List->GetAt(m_List->FindIndex(i));
		
		if(tmp->m_ObjectType == 'G')
		{
			if( tmp->m_ALIAS != m_ALIAS)
			{
				if( m_PARENT_GROUP_ALIAS != tmp->m_ALIAS )
				{
					if( tmp->IsRelated(point) )
					{
						m_Related  = 1;
						return 1;
					}
					tmp->m_Related  = 0;
					m_Related =  0;
				}
			}
		}
	}

	return 0;

}


INT CSGroupObject::IsRelated(CPoint Cur_Pos)
{

	if(  m_ObjectRect.left < Cur_Pos.x && Cur_Pos.x < m_ObjectRect.right)	
	{
		if( m_ObjectRect.bottom < Cur_Pos.y && Cur_Pos.y <m_ObjectRect.top)
		{
			return 1;
		}
	}
	return 0;
}


INT CSGroupObject::SaveToDM()
{

	INT ids;
	CString TOP,BOTTOM,LEFT,RIGHT,m_IDX;
	         
	TOP.Format(_T("%d"),m_ObjectRect.top);
	BOTTOM.Format(_T("%d"),m_ObjectRect.bottom);
	LEFT.Format(_T("%d"),m_ObjectRect.left);
	RIGHT.Format(_T("%d"),m_ObjectRect.right);
	m_IDX.Format(_T("%d"),IDX);
    
	//DB에 수정된 값을 넣어준다 
	pDM->InitDBFieldData();
	ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
    ids = pDM->AddDBFieldData(_T("TASK_ALIAS"),_T("STRING"),m_TASK);
	ids = pDM->AddDBFieldData(_T("PARENT_GROUP_ALIAS"),_T("STRING"),m_PARENT_GROUP_ALIAS);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	ids = pDM->AddDBFieldData(_T("TOP"),_T("NUMERIC"),TOP);
	ids = pDM->AddDBFieldData(_T("BOTTOM"),_T("NUMERIC"),BOTTOM);
	ids = pDM->AddDBFieldData(_T("LEFT"),_T("NUMERIC"),LEFT);
	ids = pDM->AddDBFieldData(_T("RIGHT"),_T("NUMERIC"),RIGHT);

	ids = pDM->AddDBFieldData(_T("IDX"),_T("NUMERIC"),m_IDX);
	ids = pDM->AddDBFieldData(_T("GROUP_TYPE"),_T("STRING"),m_GROUP_TYPE);
	
	ids = pDM->MakeUpdateFrame(_T("SPF_GROUP_TBL"), _T("PK"), _T("NUMERIC"),m_PK);

            
	//DM에 수정된 값을 넣어준다
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		if(pDM->GetCellData(i,0) == m_PK)
		{
			pDM->SetCellData(i,1,m_ALIAS);	 
			pDM->SetCellData(i,2,m_TASK);
			pDM->SetCellData(i,3,m_PARENT_GROUP_ALIAS);
			pDM->SetCellData(i,4,m_DESCRIP);
		    pDM->SetCellData(i,5,TOP);	 
			pDM->SetCellData(i,6,BOTTOM);
			pDM->SetCellData(i,7,LEFT);
			pDM->SetCellData(i,8,RIGHT);
			pDM->SetCellData(i,9,m_IDX);
			pDM->SetCellData(i,10,m_GROUP_TYPE);

			
		}
	}
    

	return 0;


}

INT CSGroupObject::InsertToDM()
{
		//DB에 정보 저장
		INT ids;

		CString TOP,BOTTOM,LEFT,RIGHT,m_IDX;

					
		TOP.Format(_T("%d"),m_ObjectRect.top);
		BOTTOM.Format(_T("%d"),m_ObjectRect.bottom);
		LEFT.Format(_T("%d"),m_ObjectRect.left);
		RIGHT.Format(_T("%d"),m_ObjectRect.right);
		m_IDX.Format(_T("%d"),IDX);	

		pDM->InitDBFieldData();
				
		ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
		ids = pDM->AddDBFieldData(_T("PK"),_T("NUMERIC"),m_PK);
		ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
		ids = pDM->AddDBFieldData(_T("TASK_ALIAS"),_T("STRING"),m_TASK);
		
		ids = pDM->AddDBFieldData(_T("TOP"),_T("NUMERIC"),TOP);
		ids = pDM->AddDBFieldData(_T("BOTTOM"),_T("NUMERIC"),BOTTOM);
		ids = pDM->AddDBFieldData(_T("LEFT"),_T("NUMERIC"),LEFT);
		ids = pDM->AddDBFieldData(_T("RIGHT"),_T("NUMERIC"),RIGHT);

		ids = pDM->AddDBFieldData(_T("IDX"),_T("NUMERIC"),m_IDX);
		
		ids = pDM->MakeInsertFrame(_T("SPF_GROUP_TBL"));
		ids = pDM->SendFrame(TRUE);
		ids = pDM->EndFrame();

		//DM에 정보저장
		pDM->InsertRow(-1);
		pDM->SetCellData(pDM->GetRowCount()-1,0,m_PK);
		pDM->SetCellData(pDM->GetRowCount()-1,1,m_ALIAS);
		pDM->SetCellData(pDM->GetRowCount()-1,2,m_TASK);
		
		pDM->SetCellData(pDM->GetRowCount()-1,4,m_DESCRIP);
		pDM->SetCellData(pDM->GetRowCount()-1,5,TOP);
		pDM->SetCellData(pDM->GetRowCount()-1,6,BOTTOM);
		pDM->SetCellData(pDM->GetRowCount()-1,7,LEFT); 
		pDM->SetCellData(pDM->GetRowCount()-1,8,RIGHT);
		pDM->SetCellData(pDM->GetRowCount()-1,9,m_IDX);

		pDM->SetCellData(pDM->GetRowCount()-1,10,m_GROUP_TYPE);

		return 0;
}

INT CSGroupObject::DeleteToDM()
{

	CString Qry;
	INT ids;

	Qry.Format(_T("DELETE FROM SPF_GROUP_TBL WHERE PK = %s;"),m_PK);
	pDM->AddFrame(Qry);

	//DM에서 삭제
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		if(pDM->GetCellData(i,0) == m_PK)
		{
			ids = pDM->DeleteRow(i,pDM->GetColCount());
		}
	}


	///관련 object 의 관계 삭제 
	CSObject *tmp;

	for( i=0; i < m_List->GetCount();i++)
	{
		tmp =(CSObject *)m_List->GetAt(m_List->FindIndex(i));
		if( tmp->m_PARENT_GROUP_ALIAS == m_ALIAS || tmp->m_GROUP_ALIAS == m_ALIAS )
		{
			tmp->m_PARENT_GROUP_ALIAS = _T("");
			tmp->m_GROUP_ALIAS = _T("");
			tmp->SaveToDM();	
		}

		if( tmp->m_ALIAS == m_ALIAS)
		{
			m_List->RemoveAt(m_List->FindIndex(i));
		}

	}


	return 0;
}