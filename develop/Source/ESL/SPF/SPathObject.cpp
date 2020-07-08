// SPathObject.cpp: implementation of the CSPathObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPF.h"
#include "SPathObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSPathObject::CSPathObject()
{
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
    m_pt1Activated = 0;
	m_pt3Activated = 0;
    m_Related = 0;
	m_pt1RelatedToState = 0;
    m_pt3RelatedToState = 0;
	pBackPath = NULL;
}

CSPathObject::~CSPathObject()
{

}

INT CSPathObject::DrawObject(CDC *pDC)
{
    INT ids;
  
	if( m_PATH_TYPE == _T("B")) return 0;

	CPathObject path; 

	if(m_Activated==_T("T"))	
	{
		ids = SetActivatedObject(pDC);
	    
		/// BODY
		path.DrawPathObject(pDC,m_pt1,m_pt2,m_pt3);

		//headercaption
        SetBodyCaption(pDC,1);
	
	}
	else
	{  
		ids = SetDrawObject(pDC);
	    
		/// BODY
		path.DrawPathObject(pDC,m_pt1,m_pt2,m_pt3);
   
		//headercaption
        SetBodyCaption(pDC,0);
	
	}
  

    
	path.DrawPathObject(pDC,m_pt1,m_pt2,m_pt3);


	//DrawPickPoint
	if(m_Selected == 1)
		DrawPickPoint(pDC);

	
	//RETURN PEN or Brush  
	ids = ReturnDrawObject(pDC);
	
	return 0;

}

INT CSPathObject::UnDrawObject(CDC *pDC)
{
    INT ids;
	INT caption_height_size = 24;
	INT caption_width_size = 100;
	
	
	if( m_PATH_TYPE == _T("B")) return 0;
	//SET PEN or Brush
	ids = SetUnDrawObject(pDC);

	/// BODY
	CPathObject path;
    path.DrawPathObject(pDC,m_pt1,m_pt2,m_pt3);

    ///Headercaption	
    CRect HeaderRect;
    HeaderRect.top  = m_pt2.y - 7;
    HeaderRect.bottom  = m_pt2.y - caption_height_size- 7;
    HeaderRect.left = m_pt2.x -caption_width_size/2;
    HeaderRect.right = m_pt2.x +caption_width_size/2;
         
	pDC->Rectangle(HeaderRect);
   
    ///UnDrawPickPoint
	UnDrawPickPoint(pDC);

	//RETURN PEN or Brush  
	ids = ReturnDrawObject(pDC);
	
	return 0;
}

INT CSPathObject::SetBodyCaption(CDC *pDC, INT Activated)
{
    INT caption_height_size = 24;
	INT caption_width_size = 100;
 	
	CString TmpAlias;
    TmpAlias = m_ALIAS;
    
	if(1==Activated)
	{
       CString tmp;
	   tmp.Format(_T("**%s"),m_ALIAS);
	   TmpAlias = tmp;
	}
   
	TmpAlias.MakeLower();
    
	if(TmpAlias.GetLength() > 12)
	{
       CString tmpalias,tmp1,tmp2;
       CString TmpActivated;
	   
       if(1==Activated)
	   {
    	   TmpActivated.Format(_T("**%s"),m_ALIAS);
	       tmp1 = TmpActivated.Left(12);

	       TmpActivated.Format(_T("**%s"),m_ALIAS);
           tmp2 = TmpActivated.Right(TmpAlias.GetLength() - 12);
	   
	   }
       else
	   {
           tmpalias = m_ALIAS;
	 	   tmp1 = tmpalias.Left(12);
	   
	       tmpalias = m_ALIAS;
	       tmp2 = tmpalias.Right(TmpAlias.GetLength() - 12);
       }

       CRect HeaderRect1;
       HeaderRect1.top  = m_pt2.y - caption_height_size/2 -7;
       HeaderRect1.bottom  = m_pt2.y - caption_height_size- 7;
       HeaderRect1.left = m_pt2.x - caption_width_size/2;
       HeaderRect1.right = m_pt2.x + caption_width_size/2;

       CRect HeaderRect2;
       HeaderRect2.top  = m_pt2.y - 7;
       HeaderRect2.bottom  = m_pt2.y - caption_height_size/2 -7;
       HeaderRect2.left = m_pt2.x -caption_width_size/2;
       HeaderRect2.right = m_pt2.x +caption_width_size/2;

       DrawTextWithFont(pDC, tmp1, HeaderRect1, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);
       DrawTextWithFont(pDC, tmp2, HeaderRect2, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);
	
	}
	else
	{
       CString tmp3;
	   
	   if(1==Activated)
	   {
    	   tmp3.Format(_T("**%s"),m_ALIAS);
	 
	   }
       else
	   {
           tmp3 = m_ALIAS;
	   }
		
	   CRect HeaderRect;
       HeaderRect.top  = m_pt2.y - 7;
       HeaderRect.bottom  = m_pt2.y - caption_height_size- 7;
       HeaderRect.left = m_pt2.x -caption_width_size/2;
       HeaderRect.right = m_pt2.x +caption_width_size/2;

	
	   pDC->SetTextColor(m_colorText);
	   pDC->SetBkMode(OPAQUE);
	   DrawTextWithFont(pDC, tmp3, HeaderRect, m_logFont, DT_VCENTER|DT_CENTER |DT_SINGLELINE);

	}
   
	 return 0;

}


INT CSPathObject::MoveObject(CPoint Pos)
{
	/*
	m_START_STATE_ALIAS.TrimLeft();
	m_START_STATE_ALIAS.TrimRight();
	m_END_STATE_ALIAS.TrimLeft();
	m_END_STATE_ALIAS.TrimRight();
	*/
	if(!m_START_STATE_ALIAS.GetLength())
	{
		m_pt1.x += Pos.x; 
	    m_pt1.y += Pos.y; 
	}
	
	m_pt2.x += Pos.x;
	m_pt2.y += Pos.y;

	if(!m_END_STATE_ALIAS.GetLength())
	{
		m_pt3.x += Pos.x;
		m_pt3.y += Pos.y;
	}

	return 0;
}

INT CSPathObject::ReSizeObject(CPoint Pos , INT PickIndex)
{


   switch(PickIndex)
	{
	case 1:
		m_pt1.x = Pos.x;
		m_pt1.y = Pos.y;
    break;
	case 2:
        m_pt2.x = Pos.x;
		m_pt2.y = Pos.y;
	break;
	case 3:
        m_pt3.x = Pos.x;
		m_pt3.y = Pos.y;
 	break;
	}

    return 0;

}

INT CSPathObject::IsPickPoint(CPoint Cur_Pos)
{
    INT x,y;
	CRect rect;
	
	//1	
	x = m_pt1.x;
	y = m_pt1.y;
	
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 1;
	}

    //2	
	x = m_pt2.x;
	y = m_pt2.y;
    
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 2;
	}
    
	//3
	x = m_pt3.x;
	y = m_pt3.y;

    if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 3;
	}

	return 0;
}



INT CSPathObject::IsSeleted(CPoint Cur_Pos)
{
	 if( m_PATH_TYPE == _T("B")) return 0;
	m_Selected = 0;

	if( m_pt1.x < Cur_Pos.x && Cur_Pos.x < m_pt2.x)
	{
		double a,b,c,d;
		a = (m_pt1.y-m_pt2.y);
		b = (m_pt2.x-m_pt1.x);
		c = (m_pt2.x-Cur_Pos.x);
	    d = a/b*c + m_pt2.y;
              
		if( (INT)d  < Cur_Pos.y +20 )
		{
			if( Cur_Pos.y-20 < (INT)d)
			{
				m_Selected = 1;
		
			}
		}
	}
	else if( m_pt2.x < Cur_Pos.x && Cur_Pos.x < m_pt3.x)
	{
        double a,b,c,d;
		a = (m_pt3.y-m_pt2.y);
		b = (m_pt3.x-m_pt2.x);
		c = (Cur_Pos.x-m_pt2.x);
	    d = a/b*c + m_pt2.y;
              
		if( (INT)d  < Cur_Pos.y +20 )
		{
			if( Cur_Pos.y-20 < (INT)d)
			{
				m_Selected = 1;
		
			}
		}
   
	}
    else if(IsPickPoint(Cur_Pos) != 0)
	{
        m_Selected = 1;
	}
	

	return m_Selected;
}



INT CSPathObject::DrawPickPoint(CDC *pDC)
{
	
	CPen TmpPen1, TmpPen2 , TmpPen3;
    CPen *pOldTmpPen1,*pOldTmpPen2,*pOldTmpPen3;
	
	CBrush TmpBrush1, TmpBrush2, TmpBrush3;
	CBrush *pOldTmpBrush1,*pOldTmpBrush2,*pOldTmpBrush3;

 ////////////////////////////////////////////////////////////   
	TmpPen1.CreatePen(PS_SOLID,1,RGB(0,0,0));
    pOldTmpPen1 = (CPen*)pDC->SelectObject(&TmpPen1);
	
	TmpBrush1.CreateSolidBrush(RGB(0,255,0));
    pOldTmpBrush1 = (CBrush*)pDC->SelectObject(&TmpBrush1);

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
	
	pDC->SelectObject(pOldTmpPen1);
    pDC->SelectObject(pOldTmpBrush1);
	
    TmpPen1.DeleteObject();
    TmpBrush1.DeleteObject();
/////////////////////////////////////////////////////	
	TmpPen2.CreatePen(PS_SOLID,1,RGB(0,0,0));
    pOldTmpPen2 = (CPen*)pDC->SelectObject(&TmpPen2);
	
	TmpBrush2.CreateSolidBrush(RGB(255,0,0));
    pOldTmpBrush2 = (CBrush*)pDC->SelectObject(&TmpBrush2);


	//2	
	x = m_pt2.x;
	y = m_pt2.y;

	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;
    
	pDC->Ellipse(rect);
    
	pDC->SelectObject(pOldTmpPen2);
    pDC->SelectObject(pOldTmpBrush2);
		
	TmpPen2.DeleteObject();
	TmpBrush2.DeleteObject();
////////////////////////////////////////////////////////
    TmpPen3.CreatePen(PS_SOLID,1,RGB(0,0,0));
    pOldTmpPen3 = (CPen*)pDC->SelectObject(&TmpPen3);
	
	TmpBrush3.CreateSolidBrush(RGB(0,0,255));
    pOldTmpBrush3 = (CBrush*)pDC->SelectObject(&TmpBrush3);

	//3
	x = m_pt3.x;
	y = m_pt3.y;


	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;
    
	pDC->Rectangle(rect);

    pDC->SelectObject(pOldTmpPen3);
    pDC->SelectObject(pOldTmpBrush3);

    TmpPen3.DeleteObject();
	TmpBrush3.DeleteObject();

    return 0;
}


INT CSPathObject::UnDrawPickPoint(CDC *pDC)
{
    
    CPen TmpPen, *pOldTmpPen;
    CBrush TmpBrush, *pOldTmpBrush;

    TmpPen.CreatePen(PS_SOLID,1,RGB(255,255,255));
    pOldTmpPen = (CPen*)pDC->SelectObject(&TmpPen);
	
	TmpBrush.CreateSolidBrush(RGB(255,255,255));
    pOldTmpBrush = (CBrush*)pDC->SelectObject(&TmpBrush);

	INT x,y;
	CRect rect;

	//1	
	x = m_pt1.x;
	y = m_pt1.y;
	rect.top = y + 7; 	
	rect.bottom = y-7;
	rect.left = x - 7;
	rect.right = x+ 7;
    
	pDC->Ellipse(rect);
	
	
	//2	
	x = m_pt2.x;
	y = m_pt2.y;

	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;
    
	pDC->Rectangle(rect);
    
	//3
	x = m_pt3.x;
	y = m_pt3.y;


	rect.top = y + 5; 	
	rect.bottom = y-5;
	rect.left = x - 5;
	rect.right = x+5;
    
	pDC->Rectangle(rect);

    pDC->SelectObject(pOldTmpPen);
    pDC->SelectObject(pOldTmpBrush);

    
    TmpPen.DeleteObject();
	TmpBrush.DeleteObject();



    return 0;
}



INT CSPathObject::SetAllEnv(CPoint Cur_Pos)
{

   	
   	CSStateObject * tmp;
    INT pt1RelatedFlag = 0;
    INT pt3RelatedFlag = 0;
	INT count = m_List->GetCount();
    CPoint m_Pos1;
    CPoint m_Pos3;
	m_Pos1.x = m_pt1.x;
	m_Pos1.y = m_pt1.y;
    m_Pos3.x = m_pt3.x;
	m_Pos3.y = m_pt3.y;
	
    

	for( INT i =0 ; i < count ; i++)
	{
		tmp =(CSStateObject *)m_List->GetAt(m_List->FindIndex(count-1-i));
		
		if(tmp->m_ObjectType == 'S')
		{
           ////////////////////////////////////m_pt1의 경우 			
		   	 
			if(tmp->IsRelated(m_Pos1))
			 {
			   
				pt1RelatedFlag = 1;
				
				if(tmp->m_ALIAS != m_START_STATE_ALIAS || m_START_STATE_ALIAS == _T(""))		
				{
                          
                        TRACE(_T("START: %s\n"),m_START_STATE_ALIAS);
                        CString MSG1;
						MSG1.Format(_T("%s를 START_STATE로 하시겠습니까?"),tmp->m_ALIAS);


	                    if(AfxMessageBox(MSG1, MB_YESNO) == IDYES )
						{
							m_START_STATE_ALIAS = tmp->m_ALIAS;
							SaveToDM();
						}		
			
				}
			 }
			
			  ////////////////////////////////////m_pt3의 경우 
             if(tmp->IsRelated(m_Pos3))
			 {
			    pt3RelatedFlag = 1;
				
				 if(tmp->m_ALIAS != m_END_STATE_ALIAS || m_END_STATE_ALIAS == _T(""))		
				{
                          m_END_STATE_ALIAS = tmp->m_ALIAS;
					      TRACE(_T("                               END: %s\n"),m_END_STATE_ALIAS);
                          
						  CString MSG2;
						  MSG2.Format(_T("%s를 END_STATE로 하시겠습니까?"),tmp->m_ALIAS);

                        if(AfxMessageBox(MSG2, MB_YESNO) == IDYES )
						{
							m_END_STATE_ALIAS = tmp->m_ALIAS;
							SaveToDM();
						}		
			
				}
			 }
		}
	   
		if(pt1RelatedFlag == 1 && pt3RelatedFlag == 1) break;
	
	}



    if(pt1RelatedFlag == 0 && m_START_STATE_ALIAS.GetLength() != 0)
	{
		
		TRACE(_T("DELETE_START: %s\n"),m_START_STATE_ALIAS);
		CString Msg1;
        Msg1.Format(_T("%s를 START_STATE로  사용하지 않으시겠습니까?"),m_START_STATE_ALIAS);

		if ( AfxMessageBox(Msg1, MB_YESNO) == IDYES )
		{
			m_START_STATE_ALIAS =_T("");
			SaveToDM();
		}
	}

    if(pt3RelatedFlag == 0 && m_END_STATE_ALIAS.GetLength() != 0)
	{
		
		TRACE(_T("                                        DELETE_END: %s\n"),m_END_STATE_ALIAS);
		CString Msg2;
        Msg2.Format(_T("%s를 END_STATE로  사용하지 않으시겠습니까?"),m_END_STATE_ALIAS);
		
		if ( AfxMessageBox(Msg2, MB_YESNO) == IDYES )
		{
			m_END_STATE_ALIAS =_T("");
			SaveToDM();
		}
	}

	return 0;
}



INT CSPathObject::CheckAllEnv(CPoint Cur_Pos)
{

	
	CSStateObject * tmp;
    CPoint m_Pos1;
    CPoint m_Pos3;
	m_Pos1.x = m_pt1.x;
	m_Pos1.y = m_pt1.y;
    m_Pos3.x = m_pt3.x;
	m_Pos3.y = m_pt3.y;
    INT count = m_List->GetCount();
    m_pt1RelatedToState=0;
    m_pt3RelatedToState=0;


	for( INT i =0 ; i < count ; i++)
	{
		tmp =(CSStateObject *)m_List->GetAt(m_List->FindIndex(count-1-i));
		
		if(tmp->m_ObjectType == 'S')
		{
		 
			 if(tmp->IsRelatedToPath(m_Pos1) == 1 && m_pt1RelatedToState == 0)
			 {
			     m_pt1RelatedToState = 1;
				
				if( tmp->m_ActivatedIndex == 1)
				{
				   m_pt1.x = tmp->m_ObjectRect.left + ( tmp->m_ObjectRect.Width()/2);
				   m_pt1.y = tmp->m_ObjectRect.bottom + 1; //해당 rect에 안쪽영역에 들어가야함 
					
				}
				if( tmp->m_ActivatedIndex == 2)
				{
				   m_pt1.x = tmp->m_ObjectRect.right - 1 ;
				   m_pt1.y = tmp->m_ObjectRect.top + ( tmp->m_ObjectRect.Height()/2) ;

				}
				if( tmp->m_ActivatedIndex == 3)
				{
				   m_pt1.x = tmp->m_ObjectRect.left + (tmp->m_ObjectRect.Width()/2);
				   m_pt1.y = tmp->m_ObjectRect.top - 1;

				}
				if( tmp->m_ActivatedIndex == 4)
				{
				   m_pt1.x = tmp->m_ObjectRect.left + 1;
				   m_pt1.y = tmp->m_ObjectRect.top + ( tmp->m_ObjectRect.Height()/2);

				}
			    				
			
			 }
		     
			 
			 if(tmp->IsRelatedToPath(m_Pos3) == 1 && m_pt3RelatedToState == 0)
			 {
                m_pt3RelatedToState = 1;
				
				if( tmp->m_ActivatedIndex == 1)
				{
				   m_pt3.x = tmp->m_ObjectRect.left + ( tmp->m_ObjectRect.Width()/2);
				   m_pt3.y = tmp->m_ObjectRect.bottom + 1;
					
				}
				if( tmp->m_ActivatedIndex == 2)
				{
				   m_pt3.x = tmp->m_ObjectRect.right -1;
				   m_pt3.y = tmp->m_ObjectRect.top + ( tmp->m_ObjectRect.Height()/2) ;

				}
				if( tmp->m_ActivatedIndex == 3)
				{
				   m_pt3.x = tmp->m_ObjectRect.left + (tmp->m_ObjectRect.Width()/2);
				   m_pt3.y = tmp->m_ObjectRect.top -1;

				}
				if( tmp->m_ActivatedIndex == 4)
				{
				   m_pt3.x = tmp->m_ObjectRect.left +1;
				   m_pt3.y = tmp->m_ObjectRect.top + ( tmp->m_ObjectRect.Height()/2);

				}				
						  
			}
		 
		   if(m_pt1RelatedToState == 1 && m_pt3RelatedToState == 1) return 1;
			
		 
	    // tmp->m_RelatedToPath  = 0;
	    // m_Related =  0;
       

		}
	}

	return 0;

}



INT CSPathObject::SaveToDM()
{
	INT ids;
    CString PT1_X,PT1_Y,PT2_X,PT2_Y,PT3_X,PT3_Y,m_IDX;
	            
	PT1_X.Format(_T("%d"),m_pt1.x);
	PT1_Y.Format(_T("%d"),m_pt1.y);
	PT2_X.Format(_T("%d"),m_pt2.x);
	PT2_Y.Format(_T("%d"),m_pt2.y);
	PT3_X.Format(_T("%d"),m_pt3.x);
	PT3_Y.Format(_T("%d"),m_pt3.y);
	m_IDX.Format(_T("%d"),IDX);


	//DB에 수정된 값을 넣어준다 

	CString Qry,tmp;

	Qry.Format(_T("UPDATE SPF_PATH_TBL SET "));

	tmp.Format(_T(" ALIAS = '%s', ") ,m_ALIAS);
	Qry +=tmp; 	

	tmp.Format(_T(" PATH_TYPE = '%s',") ,m_PATH_TYPE);
	Qry +=tmp; 	
	tmp.Format(_T(" START_STATE_ALIAS = '%s',"),m_START_STATE_ALIAS);
	Qry +=tmp; 	
	tmp.Format(_T(" END_STATE_ALIAS = '%s',") ,m_END_STATE_ALIAS);
	Qry +=tmp; 	
	tmp.Format(_T(" ISACTIVATED = '%s', "),m_Activated);
	Qry +=tmp; 	
	tmp.Format(_T(" DEVELOPER = '%s' ,"),m_Developer);
	Qry +=tmp; 	
	tmp.Format(_T(" DVM_ALIAS = '%s', "),m_DvmAlias);
	Qry +=tmp; 	
	tmp.Format(_T(" SOURCE_FILE_NAME = '%s',") ,m_SOURCE_FILE_NAME);
	Qry +=tmp; 	
	tmp.Format(_T(" SOURCE_FUNCTION_NAME = '%s', "),m_SOURCE_FUNCTION_NAME);
	Qry +=tmp; 	
	tmp.Format(_T(" DESCRIP = '%s' ,"),m_DESCRIP);
	Qry +=tmp; 	
	tmp.Format(_T(" PT1_X = %s ,"),PT1_X);
	Qry +=tmp; 	
	tmp.Format(_T(" PT1_Y = %s ,"),PT1_Y);
	Qry +=tmp; 	
	tmp.Format(_T(" PT2_X = %s ,"),PT2_X);
	Qry +=tmp; 	
	tmp.Format(_T(" PT2_Y = %s ,"),PT2_Y);
	Qry +=tmp; 	
	tmp.Format(_T(" PT3_X = %s ,"),PT3_X);
	Qry +=tmp; 	
	tmp.Format(_T(" PT3_Y = %s ,"),PT3_Y);
	Qry +=tmp; 	
	tmp.Format(_T(" IDX = %d "),IDX);
	Qry +=tmp; 	

	tmp.Format(_T(" WHERE PK = %s and PATH_TYPE = '%s' ;"),m_PK, m_PATH_TYPE);
	Qry +=tmp; 	

	ids= pDM->AddFrame(Qry);
            
	//DM에 수정된 값을 넣어준다
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		//back path를 고려해 alias로 row를 찾는다 
		if(pDM->GetCellData(i,0) == m_PK && pDM->GetCellData(i,2) == m_PATH_TYPE )
		{
			pDM->SetCellData(i,1,m_ALIAS);
			pDM->SetCellData(i,2,m_PATH_TYPE);
			pDM->SetCellData(i,3,m_START_STATE_ALIAS);
			pDM->SetCellData(i,4,m_END_STATE_ALIAS);
			pDM->SetCellData(i,5,m_Activated);

			pDM->SetCellData(i,6,m_SOURCE_FILE_NAME);
			pDM->SetCellData(i,7,m_SOURCE_FUNCTION_NAME);
			pDM->SetCellData(i,8,m_DESCRIP);
		    pDM->SetCellData(i,9,PT1_X);	 
			pDM->SetCellData(i,10,PT1_Y);
			pDM->SetCellData(i,11,PT2_X);
			pDM->SetCellData(i,12,PT2_Y);
            pDM->SetCellData(i,13,PT3_X);
			pDM->SetCellData(i,14,PT3_Y);
			pDM->SetCellData(i,15,m_Developer);
			pDM->SetCellData(i,16,m_DvmAlias);
			pDM->SetCellData(i,17,m_IDX);

		}
	}
    
	if( m_PATH_TYPE == _T("D"))
	{
		pBackPath->m_START_STATE_ALIAS = m_END_STATE_ALIAS;
		pBackPath->m_END_STATE_ALIAS = m_START_STATE_ALIAS;

		pBackPath->m_pt1 =m_pt1;
		pBackPath->m_pt2 =m_pt2;
		pBackPath->m_pt3 =m_pt3;

		pBackPath->SaveToDM();
		
	}

//    TRACE(_T("        %s\n"),m_START_STATE_ALIAS);
//    TRACE(_T("%s\n"),m_END_STATE_ALIAS);


    return 0;

}

INT CSPathObject::InsertToDM()
{
	//DB에 정보저장 
	INT ids;
	CString PT1_X,PT1_Y,PT2_X,PT2_Y,PT3_X,PT3_Y,m_IDX;
				
	PT1_X.Format(_T("%d"),m_pt1.x);
	PT1_Y.Format(_T("%d"),m_pt1.y);
	PT2_X.Format(_T("%d"),m_pt2.x);
	PT2_Y.Format(_T("%d"),m_pt2.y);
	PT3_X.Format(_T("%d"),m_pt3.x);
	PT3_Y.Format(_T("%d"),m_pt3.y);
	m_IDX.Format(_T("%d"),IDX);
			
	pDM->InitDBFieldData();
						
	ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
	ids = pDM->AddDBFieldData(_T("PATH_TYPE"),_T("STRING"),m_PATH_TYPE);

	ids = pDM->AddDBFieldData(_T("TASK_ALIAS"),_T("STRING"),m_TASK);
					
	ids = pDM->AddDBFieldData(_T("PK"),_T("NUMERIC"),m_PK);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	ids = pDM->AddDBFieldData(_T("ISACTIVATED"),_T("STRING"),m_Activated);
	ids = pDM->AddDBFieldData(_T("PT1_X"),_T("NUMERIC"),PT1_X);
	ids = pDM->AddDBFieldData(_T("PT1_Y"),_T("NUMERIC"),PT1_Y);
	ids = pDM->AddDBFieldData(_T("PT2_X"),_T("NUMERIC"),PT2_X);
	ids = pDM->AddDBFieldData(_T("PT2_Y"),_T("NUMERIC"),PT2_Y);
	ids = pDM->AddDBFieldData(_T("PT3_X"),_T("NUMERIC"),PT3_X);
	ids = pDM->AddDBFieldData(_T("PT3_Y"),_T("NUMERIC"),PT3_Y);

	ids = pDM->AddDBFieldData(_T("IDX"),_T("NUMERIC"),m_IDX);


	ids = pDM->AddDBFieldData(_T("DEVELOPER"),_T("STRING"),m_Developer);
	ids = pDM->AddDBFieldData(_T("DVM_ALIAS"),_T("STRING"),m_DvmAlias);

	ids = pDM->AddDBFieldData(_T("SOURCE_FILE_NAME"),_T("STRING"),m_SOURCE_FILE_NAME);
	ids = pDM->AddDBFieldData(_T("SOURCE_FUNCTION_NAME"),_T("STRING"),m_SOURCE_FUNCTION_NAME);
    
	ids = pDM->MakeInsertFrame(_T("SPF_PATH_TBL"));
	ids = pDM->SendFrame(TRUE);
	ids = pDM->EndFrame();
     
				
	//DM에 정보저장
	pDM->InsertRow(-1);
	pDM->SetCellData(pDM->GetRowCount()-1,0,m_PK);
	pDM->SetCellData(pDM->GetRowCount()-1,1,m_ALIAS);
				
	pDM->SetCellData(pDM->GetRowCount()-1,2,m_PATH_TYPE);
	//pDM->SetCellData(pDM->GetRowCount()-1,3,m_START_STATE_ALIAS);			
	//pDM->SetCellData(pDM->GetRowCount()-1,4,m_END_STATE_ALIAS);	
	pDM->SetCellData(pDM->GetRowCount()-1,5,m_Activated);
	pDM->SetCellData(pDM->GetRowCount()-1,6,m_SOURCE_FILE_NAME); 
	pDM->SetCellData(pDM->GetRowCount()-1,7,m_SOURCE_FUNCTION_NAME);
	pDM->SetCellData(pDM->GetRowCount()-1,8,m_DESCRIP);
				
	pDM->SetCellData(pDM->GetRowCount()-1,9,PT1_X);
	pDM->SetCellData(pDM->GetRowCount()-1,10,PT1_Y);
	pDM->SetCellData(pDM->GetRowCount()-1,11,PT2_X);
	pDM->SetCellData(pDM->GetRowCount()-1,12,PT2_Y);
	pDM->SetCellData(pDM->GetRowCount()-1,13,PT3_X);
	pDM->SetCellData(pDM->GetRowCount()-1,14,PT3_Y);
	pDM->SetCellData(pDM->GetRowCount()-1,15,m_Developer);
	pDM->SetCellData(pDM->GetRowCount()-1,16,m_DvmAlias);
	pDM->SetCellData(pDM->GetRowCount()-1,17,m_IDX);

	return 0;
}

INT CSPathObject::DeleteToDM()
{

	INT ids;
	CString Qry;

	Qry.Format(_T("DELETE FROM SPF_PATH_TBL WHERE PK = %s;"),m_PK);
	pDM->AddFrame(Qry);

	//DM에서 삭제
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		if(pDM->GetCellData(i,0) == m_PK)
		{	
			ids=pDM->DeleteRow(i,pDM->GetColCount());
			i=i-1;
		}
	}


	CSObject *tmp;

	for( i=0; i < m_List->GetCount();i++)
	{
		///LIST 에서 삭제 
		tmp =(CSObject *)m_List->GetAt(m_List->FindIndex(i));

		if( tmp->m_ALIAS == m_ALIAS)
		{
			m_List->RemoveAt(m_List->FindIndex(i));
		}

	}

	///관련 ReSource 의 관계 삭제 

	Qry.Format(_T("DELETE FROM SPF_PATH_RESOURCE_TBL WHERE PATH_ALIAS = '%s';"),m_ALIAS);
	pDM->AddFrame(Qry);

//	pDM->SendFrame(TRUE);
	return 0;
}

CSObject * CSPathObject::GetBackPath()
{
	CSObject *tmp;

	for(INT i=0; i < m_List->GetCount();i++)
	{
		///LIST 에서 삭제 
		tmp =(CSObject *)m_List->GetAt(m_List->FindIndex(i));

		if( tmp->m_ObjectType == _T("P") && tmp->m_PATH_TYPE == _T("B") && tmp->m_PK == m_PK)
		{
			return tmp;

		}
	}

	return tmp;
}

INT CSPathObject::MakeObjList()
{

	return 0;
}

INT CSPathObject::MoveObject2(CPoint Pos, INT PickIndex)
{

	if( PickIndex == 1)
	{
		m_pt1.x += Pos.x; 
		m_pt1.y += Pos.y; 
	}

	if( PickIndex == 3)

	{
		m_pt3.x += Pos.x;
		m_pt3.y += Pos.y;
	}

	return 0;
}
