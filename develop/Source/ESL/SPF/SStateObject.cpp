// SStateObject.cpp: implementation of the CSStateObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPF.h"
#include "SStateObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSStateObject::CSStateObject()
{
	m_RelatedToGroup = 0;
	m_ActivatedIndex = 0;

	m_colorText = RGB(0,0,0);

	m_logFont.lfHeight = 8;
	m_logFont.lfWidth = 0;
	m_logFont.lfEscapement =0;
	m_logFont.lfOrientation = 0;
	m_logFont.lfWeight = FW_NORMAL;
	m_logFont.lfItalic = FALSE;
	m_logFont.lfUnderline = FALSE;
	m_logFont.lfStrikeOut = FALSE;
	m_logFont.lfCharSet = DEFAULT_CHARSET;
	m_logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	m_logFont.lfClipPrecision = OUT_CHARACTER_PRECIS;
	m_logFont.lfQuality = DEFAULT_QUALITY;
	m_logFont.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	
	// KOL.UDF.022 시큐어코딩 보완
	m_ActivatedIndex = 0;
	m_LObjectList.RemoveAll();
	m_RObjectList.RemoveAll();
	m_RelatedToPath = 0;


}


CSStateObject::~CSStateObject()
{

}


INT CSStateObject::DrawObject(CDC *pDC)
{

	INT ids;

	//SET PEN or Brush
	if(m_Activated==_T("T"))	
	{
		ids = SetActivatedObject(pDC);
	}
	else if(m_RelatedToGroup)
	{
		ids = SetDrawRelatedObject(pDC);
	}
	else
	{  
		ids = SetDrawObject(pDC);
	}

	/// BODY
	pDC->Ellipse(m_ObjectRect);

    SetBodyCaption(pDC);

    //DrawPickPoint
	if(m_Selected == 1)
	{
		DrawPickPoint(pDC);
	}
    
    //DrawActivatedPickPoint
	if(m_ActivatedIndex != 0)
    {
       DrawActivatedPickPoint(pDC,m_ActivatedIndex);
	}

	//RETURN PEN or Brush  
	ids = ReturnDrawObject(pDC);
	
	return 0;

}

INT CSStateObject::SetBodyCaption(CDC *pDC)
{

    CString TmpAlias;
    TmpAlias = m_ALIAS;
    TmpAlias.MakeLower();
	    
	if(TmpAlias.GetLength() > 10)
	{
       CString tmpalias,tmp1,tmp2;
       tmpalias = m_ALIAS;
	   
       tmp1 = tmpalias.Left(10);
	   tmpalias = m_ALIAS;
	   
       tmp2 = tmpalias.Right(TmpAlias.GetLength() - 10);
     
       CRect HeaderRect1;
       HeaderRect1.top  = m_ObjectRect.top - m_ObjectRect.Width()/2;
       HeaderRect1.bottom  = m_ObjectRect.top - m_ObjectRect.Width()/2 - 12;
       HeaderRect1.left = m_ObjectRect.left + m_ObjectRect.Width()/2 -32;
       HeaderRect1.right = m_ObjectRect.right - m_ObjectRect.Width()/2 +32;

       CRect HeaderRect2;
       HeaderRect2.top  = m_ObjectRect.top - m_ObjectRect.Width()/2+12;
       HeaderRect2.bottom  = m_ObjectRect.top - m_ObjectRect.Width()/2 ;
       HeaderRect2.left = m_ObjectRect.left + m_ObjectRect.Width()/2 -32;
       HeaderRect2.right = m_ObjectRect.right - m_ObjectRect.Width()/2 +32;

       DrawTextWithFont(pDC, tmp1, HeaderRect1, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);
       DrawTextWithFont(pDC, tmp2, HeaderRect2, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);

	
	}
	else
	{
       	CRect HeaderRect;
        HeaderRect.top  = m_ObjectRect.top - m_ObjectRect.Width()/2 + 12;
        HeaderRect.bottom  = m_ObjectRect.top - m_ObjectRect.Width()/2 - 12;
        HeaderRect.left = m_ObjectRect.left + m_ObjectRect.Width()/2 -32;
        HeaderRect.right = m_ObjectRect.right - m_ObjectRect.Width()/2 +32;

        //pDC->Rectangle(HeaderRect);
	
	    pDC->SetTextColor(m_colorText);
	    pDC->SetBkMode(OPAQUE);
	    DrawTextWithFont(pDC, m_ALIAS, HeaderRect, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);

	}
   
	 return 0;


}

/*
INT CSStateObject::DrawOutLine(CDC *pDC)
{

   	CPen TmpPen, *pOldTmpPen;
	
	TmpPen.CreatePen(PS_SOLID,1,RGB(255,0,0));
    pOldTmpPen = (CPen*)pDC->SelectObject(&TmpPen);

	INT x,y;
	CRect rect;

	//1	
	x = m_pt1.x;
	y = m_pt1.y;
	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;
    
	pDC->Rectangle(rect);
	
	pDC->SelectObject(pOldTmpPen);
  	
    TmpPen.DeleteObject();
     
   return 0;
}


INT CSStateObject::UnDrawOutLine(CDC *pDC)
{
   	CPen TmpPen, *pOldTmpPen;
	
	TmpPen.CreatePen(PS_SOLID,1,RGB(255,255,255));
    pOldTmpPen = (CPen*)pDC->SelectObject(&TmpPen);

	INT x,y;
	CRect rect;

	//1	
	x = m_pt1.x;
	y = m_pt1.y;
	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;
    
	pDC->Rectangle(rect);
	
	pDC->SelectObject(pOldTmpPen);
    TmpPen.DeleteObject();
   
   return 0;
}

*/

INT CSStateObject::UnDrawObject(CDC *pDC)
{

	INT ids;

	//SET PEN or Brush
	ids = SetUnDrawObject(pDC);

	/// BODY
	pDC->Ellipse(m_ObjectRect);



	//BODY Caption
//	pDC->DrawText(m_ALIAS , Header , DT_CENTER);
    
	//PickPoint
	DrawPickPoint(pDC);
    
   
	//RETURN PEN or Brush  
	ids = ReturnDrawObject(pDC);

	for(INT i = 0 ; i < m_ObjectList.GetCount() ; i++)
	{

		CSObject * tmp = (CSObject*)m_ObjectList.GetAt(m_ObjectList.FindIndex(i));
		tmp->UnDrawObject(pDC);

	}

	for(i = 0 ; i < m_LObjectList.GetCount() ; i++)
	{

		CSObject * tmp = (CSObject*)m_LObjectList.GetAt(m_LObjectList.FindIndex(i));
		tmp->UnDrawObject(pDC);

	}
	
	for( i = 0 ; i < m_RObjectList.GetCount() ; i++)
	{

		CSObject * tmp = (CSObject*)m_RObjectList.GetAt(m_RObjectList.FindIndex(i));
		tmp->UnDrawObject(pDC);

	}
	
	return 0;

}

INT CSStateObject::MoveObject(CPoint Pos)
{

	//좌표 입력 

	INT m_Index=0,i;
	CRect m_Rect;

	m_Rect = m_ObjectRect;

	m_ObjectRect.top += Pos.y;  
	m_ObjectRect.bottom += Pos.y; 
	m_ObjectRect.left += Pos.x; 
	m_ObjectRect.right += Pos.x;

	for( i = 0 ; i < m_ObjectList.GetCount() ; i++)
	{

		CSObject * tmp = (CSObject*)m_ObjectList.GetAt(m_ObjectList.FindIndex(i));
		tmp->MoveObject(Pos);

	}

	for(i = 0 ; i < m_LObjectList.GetCount() ; i++)
	{
		
		CSObject * tmp = (CSObject*)m_LObjectList.GetAt(m_LObjectList.FindIndex(i));
		m_Index = GetIndex(m_Rect,tmp->m_pt1);

		if( m_Index == 1)
		{
			tmp->m_pt1.x =m_ObjectRect.left + ((m_ObjectRect.right - m_ObjectRect.left)/2) ;
			tmp->m_pt1.y =m_ObjectRect.bottom + 1;
		}else
		if( m_Index == 2)
		{
			tmp->m_pt1.x =m_ObjectRect.right-1;
			tmp->m_pt1.y =m_ObjectRect.top + ((m_ObjectRect.bottom - m_ObjectRect.top)/2);
		}else
		if( m_Index == 3)
		{
			tmp->m_pt1.x =m_ObjectRect.left + ((m_ObjectRect.right - m_ObjectRect.left)/2);
			tmp->m_pt1.y =m_ObjectRect.top - 1;
		}else
		if( m_Index == 4)
		{
			tmp->m_pt1.x =m_ObjectRect.left + 1;
			tmp->m_pt1.y =m_ObjectRect.top + ((m_ObjectRect.bottom - m_ObjectRect.top)/2) ;
		}

	}

	for(i = 0 ; i < m_RObjectList.GetCount() ; i++)
	{

		CSObject * tmp = (CSObject*)m_RObjectList.GetAt(m_RObjectList.FindIndex(i));
//		tmp->MoveObject2(Pos,3);
		m_Index = GetIndex(m_Rect,tmp->m_pt3);

		if( m_Index == 1)
		{
			tmp->m_pt3.x =m_ObjectRect.left + ((m_ObjectRect.right - m_ObjectRect.left)/2) ;
			tmp->m_pt3.y =m_ObjectRect.bottom + 1;
		}else
		if( m_Index == 2)
		{
			tmp->m_pt3.x =m_ObjectRect.right - 1;
			tmp->m_pt3.y =m_ObjectRect.top + ((m_ObjectRect.bottom - m_ObjectRect.top)/2);
		}else
		if( m_Index == 3)
		{
			tmp->m_pt3.x =m_ObjectRect.left + ((m_ObjectRect.right - m_ObjectRect.left)/2);
			tmp->m_pt3.y =m_ObjectRect.top - 1;
		}else
		if( m_Index == 4)
		{
			tmp->m_pt3.x =m_ObjectRect.left + 1;
			tmp->m_pt3.y =m_ObjectRect.top + ((m_ObjectRect.bottom - m_ObjectRect.top)/2) ;
		}

	}


	return 0;
}



INT CSStateObject::IsSeleted(CPoint Cur_Pos)
{
	m_Selected = 0;

	if( OtherGroupFlag ) return 0;

	if( m_ObjectRect.left < Cur_Pos.x && Cur_Pos.x < m_ObjectRect.right)
	{
			if( m_ObjectRect.bottom < Cur_Pos.y && Cur_Pos.y < m_ObjectRect.top)
			{
				MakeObjList();		
				m_Selected = 1;
			}
	}
	
	return m_Selected;
}

INT CSStateObject::DrawPickPoint(CDC *pDC)
{
	INT x,y;
    INT picksize = 5;

	CRect rect;
	
	//1
	x = m_ObjectRect.left + ( m_ObjectRect.Width()/2);
	y = m_ObjectRect.bottom ;

	rect.top = y + picksize; 	
	rect.bottom = y-picksize;
	rect.left = x - picksize;
	rect.right = x+picksize;

	pDC->Rectangle(rect);


	//2
	x = m_ObjectRect.right ;
	y = m_ObjectRect.top + ( m_ObjectRect.Height()/2) ;

	rect.top = y + picksize; 	
	rect.bottom = y-picksize;
	rect.left = x - picksize;
	rect.right = x+picksize;

	pDC->Rectangle(rect);

    
	//3
	x = m_ObjectRect.left + (m_ObjectRect.Width()/2);
	y = m_ObjectRect.top ;

	rect.top = y + picksize; 	
	rect.bottom = y-picksize;
	rect.left = x - picksize;
	rect.right = x+picksize;

	pDC->Rectangle(rect);
	

	//4
	x = m_ObjectRect.left ;
	y = m_ObjectRect.top + ( m_ObjectRect.Height()/2);

	rect.top = y + picksize; 	
	rect.bottom = y-picksize;
	rect.left = x - picksize;
	rect.right = x+picksize;
    
	pDC->Rectangle(rect);

   	return 0;
}


//현재 쓰지 않는 함수 
INT CSStateObject::DrawActivatedPickPoint(CDC *pDC,INT Index)
{
 
    return 0;
}


INT CSStateObject::UnDrawActivatedPickPoint(CDC *pDC)
{
    //index에 해당하는 pickpoint를 활성화 시킨다 
    INT x,y;
    INT picksize = 5;
    CRect rect;
	  
	CPen TmpPen, *pOldTmpPen;
    CBrush TmpBrush, *pOldTmpBrush;

    TmpPen.CreatePen(PS_SOLID,1,RGB(255,255,255));
    pOldTmpPen = (CPen*)pDC->SelectObject(&TmpPen);
	
	TmpBrush.CreateSolidBrush(RGB(255,255,255));
    pOldTmpBrush = (CBrush*)pDC->SelectObject(&TmpBrush);


		x = m_ObjectRect.left + ( m_ObjectRect.Width()/2);
		y = m_ObjectRect.bottom ;

		rect.top = y + picksize; 	
		rect.bottom = y-picksize;
		rect.left = x - picksize;
		rect.right = x+picksize;

		pDC->Rectangle(rect);


       	x = m_ObjectRect.right ;
		y = m_ObjectRect.top + ( m_ObjectRect.Height()/2) ;

		rect.top = y + picksize; 	
		rect.bottom = y-picksize;
		rect.left = x - picksize;
		rect.right = x+picksize;

		pDC->Rectangle(rect);
         

		//3
		x = m_ObjectRect.left + (m_ObjectRect.Width()/2);
		y = m_ObjectRect.top ;

		rect.top = y + picksize; 	
		rect.bottom = y-picksize;
		rect.left = x - picksize;
		rect.right = x+picksize;

		pDC->Rectangle(rect);
	  

		//4
		x = m_ObjectRect.left ;
		y = m_ObjectRect.top + ( m_ObjectRect.Height()/2);

		rect.top = y + picksize; 	
		rect.bottom = y-picksize;
		rect.left = x - picksize;
		rect.right = x+picksize;
    
		pDC->Rectangle(rect);
       
	
    
	pDC->SelectObject(pOldTmpPen);
    pDC->SelectObject(pOldTmpBrush);

    
    TmpPen.DeleteObject();
	TmpBrush.DeleteObject();

    return 0;


}

INT CSStateObject::SetAllEnv(CPoint Cur_Pos)
{
	CSGroupObject * tmp;
    INT count = m_List->GetCount();
    INT RelatedToGroupFlag = 0;

	for( INT i = 0 ; i < count ; i++)
	{
		tmp =(CSGroupObject *)m_List->GetAt(m_List->FindIndex(count-i-1));
	
		if(tmp->m_ObjectType == 'G')
		{
			
			if(tmp->IsRelated(Cur_Pos))
			{
				RelatedToGroupFlag = 1;
				if( m_GROUP_ALIAS != tmp->m_ALIAS || m_GROUP_ALIAS == _T(""))
				{
					if ( AfxMessageBox(_T("선택하신 Group에 추가 하시겠습니까?"), MB_YESNO) == IDYES )
					{
						m_GROUP_ALIAS = tmp->m_ALIAS;
						SaveToDM();
					}
				}
			}
					
		}
	
	   if(RelatedToGroupFlag == 1) break;
	
	}

   
	m_RelatedToGroup  = 0;
        
    if(RelatedToGroupFlag == 0 && m_GROUP_ALIAS.GetLength() != 0)
	{
		if ( AfxMessageBox(_T("Group에서 제거 하시겠습니까?"), MB_YESNO) == IDYES )
		{
			m_GROUP_ALIAS =_T("");
			SaveToDM();
		}
	}	



	return 0;
}


INT CSStateObject::CheckAllEnv(CPoint point)
{
	CSGroupObject * tmp;
    INT count = m_List->GetCount();

	for( INT i = 0 ; i < count ; i++)
	{
		tmp =(CSGroupObject *)m_List->GetAt(m_List->FindIndex(count-i-1));
	    
		if(tmp->m_ObjectType == 'G')
		{
			if(m_GROUP_ALIAS != tmp->m_ALIAS )
			{
				if( tmp->IsRelated(point) )
				{
						m_RelatedToGroup  = 1;
						return 1;
				}
					 m_RelatedToGroup =  0;
			}

		}
	}
	return 0;
}


INT CSStateObject::IsRelated(CPoint Cur_Pos)
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

INT CSStateObject::WhichIndex(CPoint Cur_Pos)
{
            
	   double dis1,dis2,dis3,dis4;

	   //1
	   CPoint pos;
	   pos.x = m_ObjectRect.left + ( m_ObjectRect.Width()/2);
	   pos.y = m_ObjectRect.bottom ;
       
       dis1 = Distance(Cur_Pos,pos);

       //2
       pos.x = m_ObjectRect.right ;
       pos.y = m_ObjectRect.top + ( m_ObjectRect.Height()/2) ;
       
	   dis2 = Distance(Cur_Pos,pos);
      
	   //3
	   pos.x = m_ObjectRect.left + (m_ObjectRect.Width()/2);
	   pos.y = m_ObjectRect.top ;

	   dis3 = Distance(Cur_Pos,pos);

       //4
	   pos.x = m_ObjectRect.left ;
	   pos.y = m_ObjectRect.top + ( m_ObjectRect.Height()/2);

       dis4 = Distance(Cur_Pos,pos);

	   INT result =  GetMinimumIndex(dis1,dis2,dis3,dis4);
	    
	   return result;
	
}

INT CSStateObject::GetIndex(CRect m_Rect,CPoint Cur_Pos)
{
            
	   double dis1,dis2,dis3,dis4;

	   //1
	   CPoint pos;
	   pos.x = m_Rect.left + ( m_Rect.Width()/2);
	   pos.y = m_Rect.bottom ;
       
       dis1 = Distance(Cur_Pos,pos);

       //2
       pos.x = m_Rect.right ;
       pos.y = m_Rect.top + ( m_Rect.Height()/2) ;
       
	   dis2 = Distance(Cur_Pos,pos);
      
	   //3
	   pos.x = m_Rect.left + (m_Rect.Width()/2);
	   pos.y = m_Rect.top ;

	   dis3 = Distance(Cur_Pos,pos);

       //4
	   pos.x = m_Rect.left ;
	   pos.y = m_Rect.top + ( m_Rect.Height()/2);

       dis4 = Distance(Cur_Pos,pos);

	   INT result =  GetMinimumIndex(dis1,dis2,dis3,dis4);
	    
	   return result;
	
}


double CSStateObject::Distance(CPoint pos1,CPoint pos2)
{
    INT dx = abs(pos1.x - pos2.x);
    INT dy = abs(pos1.y - pos2.y);
    
	return sqrt(dx*dx + dy+dy);
}


INT CSStateObject::GetMinimumIndex(double a, double b, double c, double d)
{
    INT result = 0; 
    double Min1,Min2;

	if(a >= b)
	{
	  Min1 = b;
	}
	else
	{
	  Min1 = a;
	}
    
	if(c >= d)
	{
	  Min2 = d;
	}
	else
	{
	  Min2 = c;
	}

	if(Min1 >= Min2)
	{
		Min1 = Min2;
	}
	 
	if(Min1 == a) return 1;
    if(Min1 == b) return 2;
    if(Min1 == c) return 3;
    if(Min1 == d) return 4;

	return 0;
}



INT CSStateObject::IsRelatedToPath(CPoint point)
{
    
	if(IsRelated(point) !=0)
	{
	    m_ActivatedIndex = WhichIndex(point);
	 
		return 1;	
	}    
    else
	{
       m_ActivatedIndex = 0;
	}

	return 0;
}

INT CSStateObject::IsPickPoint(CPoint Cur_Pos)
{

	return 0;
}

INT CSStateObject::SaveToDM()
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
    ids = pDM->AddDBFieldData(_T("GROUP_ALIAS"),_T("STRING"),m_GROUP_ALIAS);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	ids = pDM->AddDBFieldData(_T("TOP"),_T("NUMERIC"),TOP);
	ids = pDM->AddDBFieldData(_T("BOTTOM"),_T("NUMERIC"),BOTTOM);
	ids = pDM->AddDBFieldData(_T("LEFT"),_T("NUMERIC"),LEFT);
	ids = pDM->AddDBFieldData(_T("RIGHT"),_T("NUMERIC"),RIGHT);
	ids = pDM->AddDBFieldData(_T("IDX"),_T("NUMERIC"),m_IDX);
	
	ids = pDM->MakeUpdateFrame(_T("SPF_STATE_TBL"), _T("PK"), _T("NUMERIC"),m_PK);
            

	//DM에 수정된 값을 넣어준다
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		if(pDM->GetCellData(i,0) == m_PK)
		{
			pDM->SetCellData(i,1,m_ALIAS);	 
			pDM->SetCellData(i,2,m_GROUP_ALIAS);
			pDM->SetCellData(i,3,m_DESCRIP);
			pDM->SetCellData(i,4,TOP);	 
			pDM->SetCellData(i,5,BOTTOM);
			pDM->SetCellData(i,6,LEFT);
			pDM->SetCellData(i,7,RIGHT);
			pDM->SetCellData(i,8,m_IDX);

		}
	}
    
   	return 0;

}
INT CSStateObject::InsertToDM()
{
			//DB에 정보저장 
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
		ids = pDM->AddDBFieldData(_T("TOP"),_T("NUMERIC"),TOP);
		ids = pDM->AddDBFieldData(_T("BOTTOM"),_T("NUMERIC"),BOTTOM);
		ids = pDM->AddDBFieldData(_T("LEFT"),_T("NUMERIC"),LEFT);
		ids = pDM->AddDBFieldData(_T("RIGHT"),_T("NUMERIC"),RIGHT);
    	ids = pDM->AddDBFieldData(_T("TASK_ALIAS"),_T("STRING"),m_TASK);

		ids = pDM->AddDBFieldData(_T("IDX"),_T("NUMERIC"),m_IDX);
			
		ids = pDM->MakeInsertFrame(_T("SPF_STATE_TBL"));
		ids = pDM->SendFrame(TRUE);
		ids = pDM->EndFrame();

		//DM에 정보저장
		pDM->InsertRow(-1);
		pDM->SetCellData(pDM->GetRowCount()-1,0,m_PK);
		pDM->SetCellData(pDM->GetRowCount()-1,1,m_ALIAS);
		
		pDM->SetCellData(pDM->GetRowCount()-1,3,m_DESCRIP);
		pDM->SetCellData(pDM->GetRowCount()-1,4,TOP);
		pDM->SetCellData(pDM->GetRowCount()-1,5,BOTTOM);
		pDM->SetCellData(pDM->GetRowCount()-1,6,LEFT); 
		pDM->SetCellData(pDM->GetRowCount()-1,7,RIGHT);

		pDM->SetCellData(pDM->GetRowCount()-1,8,m_IDX);



	return 0;
}

INT CSStateObject::DeleteToDM()
{

	INT ids;
	CString Qry;

	Qry.Format(_T("DELETE FROM SPF_STATE_TBL WHERE PK = %s;"),m_PK);
	pDM->AddFrame(Qry);

	//DM에서 삭제
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		if(pDM->GetCellData(i,0) == m_PK)
		{	
			ids=pDM->DeleteRow(i,pDM->GetColCount());
		}
	}


	///관련 object 의 관계 삭제 
	CSObject *tmp;

	for( i=0; i < m_List->GetCount();i++)
	{
		tmp =(CSObject *)m_List->GetAt(m_List->FindIndex(i));
		if( tmp->m_START_STATE_ALIAS == m_ALIAS)
		{
			tmp->m_START_STATE_ALIAS = _T("");
			tmp->SaveToDM();	
		}

		if( tmp->m_END_STATE_ALIAS == m_ALIAS)
		{
			tmp->m_END_STATE_ALIAS = _T("");
			tmp->SaveToDM();	
		}

		///LIST 에서 삭제 
		if( tmp->m_ALIAS == m_ALIAS)
		{
			m_List->RemoveAt(m_List->FindIndex(i));
		}

	}

	return 0;
}

INT CSStateObject::MakeObjList()
{
	m_ObjectList.RemoveAll();
	m_LObjectList.RemoveAll();
	m_RObjectList.RemoveAll();

	for(INT i=0; i < m_List->GetCount(); i++)
	{
		CSObject *tmp;
	
		tmp =(CSObject *)m_List->GetAt(m_List->FindIndex(i));
		
		///Group Path
		if( tmp->m_START_STATE_ALIAS == m_ALIAS)
		{
			m_LObjectList.AddHead((CObject*)tmp);

			if( tmp->m_END_STATE_ALIAS == m_ALIAS)
			{
				m_LObjectList.RemoveHead();
				m_ObjectList.AddHead((CObject*)tmp);
				continue;
			}

		}
		else
		if( tmp->m_END_STATE_ALIAS == m_ALIAS)
		{
				m_RObjectList.AddHead((CObject*)tmp);	
		}


	}


	return 0;
}