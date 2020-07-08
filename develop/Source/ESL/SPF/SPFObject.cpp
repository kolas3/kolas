// SPFObject.cpp: implementation of the CSPFObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SPFObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSPFObject::CSPFObject()
{
	m_Activated = _T("");
	m_Selected = 0;

	// KOL.UDF.022 시큐어코딩 보완
	m_ObjectType = ''; 
	m_ALIAS = _T("");
	m_PK = _T("");
	m_DESCRIP = _T("");
    m_ObjectRect = NULL;
	m_StartPos = NULL;
	m_TASK_ALIAS = _T("");
    m_PARENT_GROUP_ALIAS = _T("");
	m_GROUP_ALIAS = _T("");
	m_PATH_TYPE = _T("");
	m_START_STATE_ALIAS = _T("");
    m_END_STATE_ALIAS = _T("");
    m_SOURCE_FILE_NAME = _T("");
	m_SOURCE_FUNCTION_NAME = _T("");
    m_pt1 = NULL;
	m_pt2 = NULL;
	m_pt3 = NULL;
    m_pt1Activated = _T("");
	m_pt3Activated = _T("");
	pOldPen = NULL;
}

CSPFObject::~CSPFObject()
{

}

INT CSPFObject::DrawObject(CDC *pDC)
{

	INT ids;

	//SET PEN or Brush
	if(m_Activated==_T("T"))	
	{
		SetActivatedObject(pDC);
	}
	else
	{  
		SetDrawObject(pDC);
	}


	if(m_ObjectType == 'G')
	{
		ids = SPFDrawGroup(pDC);
	}
	else if(m_ObjectType == 'S')
	{
		ids = SPFDrawState(pDC);
	}
	else if(m_ObjectType == 'P')
	{
	    if(m_PATH_TYPE != _T("B"))
		{
			ids = SPFDrawPath(pDC);
		}
	}
	else
	{
		AfxMessageBox(_T("잘못된 데이터 입니다."));
	}

	//RETURN PEN or Brush  
	ReturnDrawObject(pDC);
	
	return 0;
}

INT CSPFObject::UnDrawObject(CDC *pDC)
{

	INT ids;
	
	if(m_ObjectType == 'G')
	{
		//SET PEN or Brush
        SetUnDrawObject(pDC);
		ids = SPFDrawGroup(pDC);
	}
	else if(m_ObjectType == 'S')
	{
	    //SET PEN or Brush
        SetUnDrawObject(pDC);
	    ids = SPFDrawState(pDC);
	}
	else if(m_ObjectType == 'P')
	{
		//SET PEN or Brush for PATH Object
        SetUnDrawPathObject(pDC);
	    ids = SPFDrawPath(pDC);
	}
	else
	{
		AfxMessageBox(_T("잘못된 데이터 입니다."));
	}

	//RETURN PEN or Brush  
	ReturnDrawObject(pDC);

	return 0;

}

INT CSPFObject::DrawSelectedObject(CDC *pDC)
{
	INT ids;

	ids = DrawPickPoint(pDC);

	return 0;
}

INT CSPFObject::UnDrawSelectedObject(CDC *pDC)
{

	INT ids;

	ids = UnDrawObject(pDC);
	ids = UnDrawPickPoint(pDC);

	return 0;
}

INT CSPFObject::SetActivatedObject(CDC *pDC)
{
    
    m_pen.CreatePen( PS_SOLID , 1 , RGB(0,0,255));
	pOldPen = pDC->SelectObject(&m_pen);
      
	return 0;
}

INT CSPFObject::IsSeleted(CPoint Cur_Pos)
{
	m_Selected = 0;

	if(m_ObjectType == 'G')
	{
		if( m_ObjectRect.left-10 < Cur_Pos.x && Cur_Pos.x < m_ObjectRect.left+10)
		{
			if( m_ObjectRect.bottom-10 < Cur_Pos.y && Cur_Pos.y < m_ObjectRect.top+10 )
				m_Selected = 1;
		}
	    else if( m_ObjectRect.right-10 < Cur_Pos.x && Cur_Pos.x < m_ObjectRect.right+10)
		{
			if( m_ObjectRect.bottom-10 < Cur_Pos.y && Cur_Pos.y < m_ObjectRect.top+10 )
				m_Selected = 1;
		} 
	    else if( m_ObjectRect.left+10 < Cur_Pos.x && Cur_Pos.x < m_ObjectRect.right-10)
		{
			if( m_ObjectRect.top-10 < Cur_Pos.y && Cur_Pos.y < m_ObjectRect.top+10 || m_ObjectRect.bottom-10 < Cur_Pos.y && Cur_Pos.y < m_ObjectRect.bottom+10)
				m_Selected = 1;
		} 
	
	}

	if(m_ObjectType == 'S')
	{
		if( m_ObjectRect.left-5 < Cur_Pos.x && Cur_Pos.x < m_ObjectRect.right+5)
		{
			if( m_ObjectRect.bottom -5< Cur_Pos.y && Cur_Pos.y < m_ObjectRect.top+5 )
				m_Selected = 1;
		}
	}

	if(m_ObjectType == 'P')
	{
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
        else if(IsPathPickPoint(Cur_Pos) != 0)
		{
            m_Selected = 1;
		}
	
	}
	return m_Selected;
}






//자기의 정보를 DM과 DB에 저장한다
INT CSPFObject::InsertToDM(CESL_DataMgr *pDM)
{
    if(m_ObjectType == 'G')
	{
	    InsertToGroupDM(pDM);
	}
	else if(m_ObjectType == 'S')
    {
	    InsertToStateDM(pDM);
	}
	if(m_ObjectType == 'P')
	{
       InsertToPathDM(pDM);
	}	

	return 0;
}


INT CSPFObject::InsertToGroupDM(CESL_DataMgr *pDM)
{
    //DB에 정보저장 
    INT ids;
	CString TOP,BOTTOM,LEFT,RIGHT;
	            
	TOP.Format(_T("%d"),m_ObjectRect.top);
	BOTTOM.Format(_T("%d"),m_ObjectRect.bottom);
	LEFT.Format(_T("%d"),m_ObjectRect.left);
	RIGHT.Format(_T("%d"),m_ObjectRect.right);
	
    pDM->InitDBFieldData();
			
	ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
	ids = pDM->AddDBFieldData(_T("PK"),_T("NUMERIC"),m_PK);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	
	ids = pDM->AddDBFieldData(_T("TOP"),_T("NUMERIC"),TOP);
	ids = pDM->AddDBFieldData(_T("BOTTOM"),_T("NUMERIC"),BOTTOM);
	ids = pDM->AddDBFieldData(_T("LEFT"),_T("NUMERIC"),LEFT);
	ids = pDM->AddDBFieldData(_T("RIGHT"),_T("NUMERIC"),RIGHT);
               
    ids = pDM->MakeInsertFrame(_T("SPF_GROUP_TBL"));
    ids = pDM->SendFrame(TRUE);
    ids = pDM->EndFrame();

    //DM에 정보저장
    pDM->InsertRow(-1);
    pDM->SetCellData(pDM->GetRowCount()-1,0,m_PK);
    pDM->SetCellData(pDM->GetRowCount()-1,1,m_ALIAS);
	
	pDM->SetCellData(pDM->GetRowCount()-1,4,m_DESCRIP);
	pDM->SetCellData(pDM->GetRowCount()-1,5,TOP);
    pDM->SetCellData(pDM->GetRowCount()-1,6,BOTTOM);
    pDM->SetCellData(pDM->GetRowCount()-1,7,LEFT); 
    pDM->SetCellData(pDM->GetRowCount()-1,8,RIGHT);
	
	return 0;
}

INT CSPFObject::InsertToStateDM(CESL_DataMgr *pDM)
{
    //DB에 정보저장 
    INT ids;
	CString TOP,BOTTOM,LEFT,RIGHT;
	            
	TOP.Format(_T("%d"),m_ObjectRect.top);
	BOTTOM.Format(_T("%d"),m_ObjectRect.bottom);
	LEFT.Format(_T("%d"),m_ObjectRect.left);
	RIGHT.Format(_T("%d"),m_ObjectRect.right);
	
    pDM->InitDBFieldData();
			
	ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
	ids = pDM->AddDBFieldData(_T("PK"),_T("NUMERIC"),m_PK);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	
	ids = pDM->AddDBFieldData(_T("TOP"),_T("NUMERIC"),TOP);
	ids = pDM->AddDBFieldData(_T("BOTTOM"),_T("NUMERIC"),BOTTOM);
	ids = pDM->AddDBFieldData(_T("LEFT"),_T("NUMERIC"),LEFT);
	ids = pDM->AddDBFieldData(_T("RIGHT"),_T("NUMERIC"),RIGHT);
               
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

	return 0;
}

INT CSPFObject::InsertToPathDM(CESL_DataMgr *pDM)
{
    //DB에 정보저장 
	INT ids;
	CString PT1_X,PT1_Y,PT2_X,PT2_Y,PT3_X,PT3_Y;
	            
	PT1_X.Format(_T("%d"),m_pt1.x);
	PT1_Y.Format(_T("%d"),m_pt1.y);
	PT2_X.Format(_T("%d"),m_pt2.x);
	PT2_Y.Format(_T("%d"),m_pt2.y);
	PT3_X.Format(_T("%d"),m_pt3.x);
	PT3_Y.Format(_T("%d"),m_pt3.y);
	
	pDM->InitDBFieldData();
				
	ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
	
	if(m_PATH_TYPE == _T("S"))
	{
		ids = pDM->AddDBFieldData(_T("PATH_TYPE"),_T("STRING"),_T("S"));
	}
	else if(m_PATH_TYPE == _T("D"))
	{
		ids = pDM->AddDBFieldData(_T("PATH_TYPE"),_T("STRING"),_T("D"));
	}
	else if(m_PATH_TYPE == _T("B"))
    {
        ids = pDM->AddDBFieldData(_T("PATH_TYPE"),_T("STRING"),_T("B"));
	}
    	
	ids = pDM->AddDBFieldData(_T("PK"),_T("NUMERIC"),m_PK);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	ids = pDM->AddDBFieldData(_T("PT1_X"),_T("NUMERIC"),PT1_X);
	ids = pDM->AddDBFieldData(_T("PT1_Y"),_T("NUMERIC"),PT1_Y);
    ids = pDM->AddDBFieldData(_T("PT2_X"),_T("NUMERIC"),PT2_X);
	ids = pDM->AddDBFieldData(_T("PT2_Y"),_T("NUMERIC"),PT2_Y);
	ids = pDM->AddDBFieldData(_T("PT3_X"),_T("NUMERIC"),PT3_X);
	ids = pDM->AddDBFieldData(_T("PT3_Y"),_T("NUMERIC"),PT3_Y);
	
	ids = pDM->AddDBFieldData(_T("SOURCE_FILE_NAME"),_T("STRING"),m_SOURCE_FILE_NAME);
	ids = pDM->AddDBFieldData(_T("SOURCE_FUNCTION_NAME"),_T("STRING"),m_SOURCE_FUNCTION_NAME);
    
	ids = pDM->MakeInsertFrame(_T("SPF_PATH_TBL"));
	ids = pDM->SendFrame(TRUE);
	ids = pDM->EndFrame();
     
	
	//DM에 정보저장
	pDM->InsertRow(-1);
	pDM->SetCellData(pDM->GetRowCount()-1,0,m_PK);
	pDM->SetCellData(pDM->GetRowCount()-1,1,m_ALIAS);
	
	if(m_PATH_TYPE == _T("S"))
	{
		pDM->SetCellData(pDM->GetRowCount()-1,2,_T("S"));
	}
	else if(m_PATH_TYPE == _T("D"))
	{
		pDM->SetCellData(pDM->GetRowCount()-1,2,_T("D"));
	}
	else if(m_PATH_TYPE == _T("B"))
    {
        pDM->SetCellData(pDM->GetRowCount()-1,2,_T("B"));
	}
	//pDM->SetCellData(pDM->GetRowCount()-1,3,m_START_STATE_ALIAS);			
	//pDM->SetCellData(pDM->GetRowCount()-1,4,m_END_STATE_ALIAS);	
	//pDM->SetCellData(pDM->GetRowCount()-1,5,m_Activated);
    pDM->SetCellData(pDM->GetRowCount()-1,6,m_SOURCE_FILE_NAME); 
    pDM->SetCellData(pDM->GetRowCount()-1,7,m_SOURCE_FUNCTION_NAME);
	pDM->SetCellData(pDM->GetRowCount()-1,8,m_DESCRIP);
	
	pDM->SetCellData(pDM->GetRowCount()-1,9,PT1_X);
	pDM->SetCellData(pDM->GetRowCount()-1,10,PT1_Y);
	pDM->SetCellData(pDM->GetRowCount()-1,11,PT2_X);
	pDM->SetCellData(pDM->GetRowCount()-1,12,PT2_Y);
	pDM->SetCellData(pDM->GetRowCount()-1,13,PT3_X);
	pDM->SetCellData(pDM->GetRowCount()-1,14,PT3_Y);
	
	return 0;

}

//Object에 변화가 일어났을 때, 그 정보를 DM과 DB에 저장
INT CSPFObject::ModifyToDM(CESL_DataMgr *pDM)
{
    if(m_ObjectType == 'G')
	{
	    ModifyToGroupDM(pDM);
	}
	else if(m_ObjectType == 'S')
    {
	    ModifyToStateDM(pDM);
	}
	if(m_ObjectType == 'P')
	{
        ModifyToPathDM(pDM);
	}	

	return 0;
}

INT CSPFObject::ModifyToGroupDM(CESL_DataMgr *pDM)
{
            
	INT ids;
	CString TOP,BOTTOM,LEFT,RIGHT;
	         
	TOP.Format(_T("%d"),m_ObjectRect.top);
	BOTTOM.Format(_T("%d"),m_ObjectRect.bottom);
	LEFT.Format(_T("%d"),m_ObjectRect.left);
	RIGHT.Format(_T("%d"),m_ObjectRect.right);
    
	//DB에 수정된 값을 넣어준다 
	pDM->InitDBFieldData();
	ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
    ids = pDM->AddDBFieldData(_T("TASK_ALIAS"),_T("STRING"),m_TASK_ALIAS);
	ids = pDM->AddDBFieldData(_T("PARENT_GROUP_ALIAS"),_T("STRING"),m_PARENT_GROUP_ALIAS);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	ids = pDM->AddDBFieldData(_T("TOP"),_T("NUMERIC"),TOP);
	ids = pDM->AddDBFieldData(_T("BOTTOM"),_T("NUMERIC"),BOTTOM);
	ids = pDM->AddDBFieldData(_T("LEFT"),_T("NUMERIC"),LEFT);
	ids = pDM->AddDBFieldData(_T("RIGHT"),_T("NUMERIC"),RIGHT);
	
	ids = pDM->MakeUpdateFrame(_T("SPF_GROUP_TBL"), _T("PK"), _T("NUMERIC"),m_PK);

            
	//DM에 수정된 값을 넣어준다
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		if(pDM->GetCellData(i,0) == m_PK)
		{
			pDM->SetCellData(i,1,m_ALIAS);	 
			pDM->SetCellData(i,2,m_TASK_ALIAS);
			pDM->SetCellData(i,3,m_PARENT_GROUP_ALIAS);
			pDM->SetCellData(i,4,m_DESCRIP);
		    pDM->SetCellData(i,5,TOP);	 
			pDM->SetCellData(i,6,BOTTOM);
			pDM->SetCellData(i,7,LEFT);
			pDM->SetCellData(i,8,RIGHT);
			
		}
	}
    
	return 0;
}

INT CSPFObject::ModifyToStateDM(CESL_DataMgr *pDM)
{
    INT ids;
	CString TOP,BOTTOM,LEFT,RIGHT;
	         
	TOP.Format(_T("%d"),m_ObjectRect.top);
	BOTTOM.Format(_T("%d"),m_ObjectRect.bottom);
	LEFT.Format(_T("%d"),m_ObjectRect.left);
	RIGHT.Format(_T("%d"),m_ObjectRect.right);
    
	//DB에 수정된 값을 넣어준다 
	pDM->InitDBFieldData();
	ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
    ids = pDM->AddDBFieldData(_T("GROUP_ALIAS"),_T("STRING"),m_GROUP_ALIAS);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	ids = pDM->AddDBFieldData(_T("TOP"),_T("NUMERIC"),TOP);
	ids = pDM->AddDBFieldData(_T("BOTTOM"),_T("NUMERIC"),BOTTOM);
	ids = pDM->AddDBFieldData(_T("LEFT"),_T("NUMERIC"),LEFT);
	ids = pDM->AddDBFieldData(_T("RIGHT"),_T("NUMERIC"),RIGHT);
	
	ids = pDM->MakeUpdateFrame(_T("SPF_STATE_TBL"), _T("PK"), _T("NUMERIC"),m_PK);
	
            
	//DM에 수정된 값을 넣어준다
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		if(pDM->GetCellData(i,0) == m_PK)
		{
			pDM->SetCellData(i,1,m_ALIAS);	 
			pDM->SetCellData(i,2,m_TASK_ALIAS);
			pDM->SetCellData(i,3,m_PARENT_GROUP_ALIAS);
			pDM->SetCellData(i,4,m_DESCRIP);
		    pDM->SetCellData(i,5,TOP);	 
			pDM->SetCellData(i,6,BOTTOM);
			pDM->SetCellData(i,7,LEFT);
			pDM->SetCellData(i,8,RIGHT);
		}
	}
    
   	return 0;
}

INT CSPFObject::ModifyToPathDM(CESL_DataMgr *pDM)
{
    
    INT ids;
    CString PT1_X,PT1_Y,PT2_X,PT2_Y,PT3_X,PT3_Y;
	            
	PT1_X.Format(_T("%d"),m_pt1.x);
	PT1_Y.Format(_T("%d"),m_pt1.y);
	PT2_X.Format(_T("%d"),m_pt2.x);
	PT2_Y.Format(_T("%d"),m_pt2.y);
	PT3_X.Format(_T("%d"),m_pt3.x);
	PT3_Y.Format(_T("%d"),m_pt3.y);


	//DB에 수정된 값을 넣어준다 
	pDM->InitDBFieldData();
	ids = pDM->AddDBFieldData(_T("ALIAS"),_T("STRING"),m_ALIAS);
    ids = pDM->AddDBFieldData(_T("PATH_TYPE"),_T("STRING"),m_PATH_TYPE);
	ids = pDM->AddDBFieldData(_T("START_STATE_ALIAS"),_T("STRING"),m_START_STATE_ALIAS);
	ids = pDM->AddDBFieldData(_T("END_STATE_ALIAS"),_T("STRING"),m_END_STATE_ALIAS);
	ids = pDM->AddDBFieldData(_T("ISACTIVATED"),_T("STRING"),m_Activated);
	ids = pDM->AddDBFieldData(_T("SOURCE_FILE_NAME"),_T("STRING"),m_SOURCE_FILE_NAME);
	ids = pDM->AddDBFieldData(_T("SOURCE_FUNCTION_NAME"),_T("STRING"),m_SOURCE_FUNCTION_NAME);
	ids = pDM->AddDBFieldData(_T("DESCRIP"),_T("STRING"),m_DESCRIP);
	ids = pDM->AddDBFieldData(_T("PT1_X"),_T("NUMERIC"),PT1_X);
	ids = pDM->AddDBFieldData(_T("PT1_Y"),_T("NUMERIC"),PT1_Y);
    ids = pDM->AddDBFieldData(_T("PT2_X"),_T("NUMERIC"),PT2_X);
	ids = pDM->AddDBFieldData(_T("PT2_Y"),_T("NUMERIC"),PT2_Y);
	ids = pDM->AddDBFieldData(_T("PT3_X"),_T("NUMERIC"),PT3_X);
	ids = pDM->AddDBFieldData(_T("PT3_Y"),_T("NUMERIC"),PT3_Y);
	ids = pDM->MakeUpdateFrame(_T("SPF_PATH_TBL"), _T("ALIAS"), _T("STRING"),m_ALIAS);
	
            
	//DM에 수정된 값을 넣어준다
	for(INT i=0; i < pDM->GetRowCount();i++)
	{
		if(pDM->GetCellData(i,0) == m_PK)
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
		}
	}
    
    return 0;
}


INT CSPFObject::DeleteToDM(CESL_DataMgr *pDM)
{
    if(m_ObjectType == 'G')
	{
	    DeleteToGroupDM(pDM);
	}
	else if(m_ObjectType == 'S')
    {
	    DeleteToStateDM(pDM);
	}
	if(m_ObjectType == 'P')
	{
        DeleteToPathDM(pDM);
	}	
    
	return 0;
}

INT CSPFObject::DeleteToGroupDM(CESL_DataMgr *pDM)
{

	return 0;
}

INT CSPFObject::DeleteToStateDM(CESL_DataMgr *pDM)
{

	return 0;
}

INT CSPFObject::DeleteToPathDM(CESL_DataMgr *pDM)
{

	return 0;
}


INT CSPFObject::SPFDrawGroup(CDC *pDC)
{
	pDC->Rectangle(m_ObjectRect);
	return 0;
}

INT CSPFObject::SPFDrawState(CDC *pDC)
{

	pDC->Ellipse(m_ObjectRect);
	return 0;
}

INT CSPFObject::SPFDrawPath(CDC *pDC)
{
	
	CPathObject path;
    
	path.DrawPathObject(pDC,m_pt1,m_pt2,m_pt3);
    
	return 0;
}


INT CSPFObject::DrawPickPoint(CDC *pDC)
{
	INT ids;

	//SET PEN or Brush
	
	if(m_ObjectType == 'G')
	{
		SetDrawObject(pDC);
		ids = SPFDrawGroupPickPoint(pDC);
	}
	else if(m_ObjectType == 'S')
	{
		SetDrawObject(pDC);
		ids = SPFDrawStatePickPoint(pDC);
	}
	else if(m_ObjectType == 'P')
	{
		SetDrawObject(pDC);	
		ids = SPFDrawPathPickPoint(pDC);
	}
	else
	{
		AfxMessageBox(_T("잘못된 데이터 입니다."));
	}

	//RETURN PEN or Brush  
	ReturnDrawObject(pDC);

	return 0;
}

INT CSPFObject::UnDrawPickPoint(CDC *pDC)
{
	INT ids;

	//SET PEN or Brush
	SetUnDrawObject(pDC);


	
	if(m_ObjectType == 'G')
	{
		ids = SPFDrawGroupPickPoint(pDC);
	}
	else if(m_ObjectType == 'S')
	{
		ids = SPFDrawStatePickPoint(pDC);
	}
	else if(m_ObjectType == 'P')
	{
		ids = SPFDrawPathPickPoint(pDC);
	}
	else
	{
		AfxMessageBox(_T("잘못된 데이터 입니다."));
	}

	//RETURN PEN or Brush  
	ReturnDrawObject(pDC);

	return 0;
}

INT CSPFObject::SPFDrawGroupPickPoint(CDC *pDC)
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

INT CSPFObject::IsPickPoint(CPoint Cur_Pos)
{
	if (m_Selected)
	{
		if(m_ObjectType == 'G')
		{
		    return IsGroupPickPoint(Cur_Pos);
        }
		else if(m_ObjectType == 'S')
		{
            return IsStatePickPoint(Cur_Pos);
		}
		else if(m_ObjectType == 'P')
		{
            return IsPathPickPoint(Cur_Pos);
		}
	}

	return 0;
}

INT CSPFObject::IsGroupPickPoint(CPoint Cur_Pos)
{
    INT x,y;
	CRect rect;

	//1	
	x = m_ObjectRect.left;
	y = m_ObjectRect.top;

   if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 1;
	}

	//2
	x = m_ObjectRect.right;
	y = m_ObjectRect.top;

    if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 2;
	}

	//3
	x = m_ObjectRect.right;
	y = m_ObjectRect.bottom;
     
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 3;
	}
	//4
	x = m_ObjectRect.left;
	y = m_ObjectRect.bottom;
   
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 4;
	}

	return 0;
}

INT CSPFObject::IsStatePickPoint(CPoint Cur_Pos)
{
    	
    INT x,y;

	x = m_ObjectRect.left + ( m_ObjectRect.Width()/2);
	y = m_ObjectRect.top ;
		
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 1;
	}

	x = m_ObjectRect.right;	
	y = m_ObjectRect.top +( m_ObjectRect.Height()/2);
		
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 2;
	}

	x = m_ObjectRect.left + ( m_ObjectRect.Width()/2);
	y = m_ObjectRect.bottom ;
		
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 3;
	}

	x = m_ObjectRect.left;
	y = m_ObjectRect.top +( m_ObjectRect.Height()/2);
		
	if ( x-5 < Cur_Pos.x  &&   Cur_Pos.x < x + 5)
	{
		if( y-5 < Cur_Pos.y  &&   Cur_Pos.y < y + 5)
		return 4;
	}

	return 0;
}

INT CSPFObject::IsPathPickPoint(CPoint Cur_Pos)
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

INT CSPFObject::SPFDrawStatePickPoint(CDC *pDC)
{
	INT x,y;
	double dx,dy;
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
	dx = m_ObjectRect.left + (m_ObjectRect.Width()/2);
	dy = m_ObjectRect.bottom + (m_ObjectRect.Width()/2);

	dx +=  (m_ObjectRect.Width()/2)/1.414;
	dy -=  (m_ObjectRect.Width()/2)/1.414;
		
    x = (INT)dx;
	y = (INT)dy;

	rect.top = y - picksize; 	
	rect.bottom = y + picksize;
	rect.left = x - picksize;
	rect.right = x + picksize;

	pDC->Rectangle(rect);


	//3
	x = m_ObjectRect.right ;
	y = m_ObjectRect.top + ( m_ObjectRect.Height()/2) ;

	rect.top = y + picksize; 	
	rect.bottom = y-picksize;
	rect.left = x - picksize;
	rect.right = x+picksize;

	pDC->Rectangle(rect);

    
	//4
	dx = m_ObjectRect.left + (m_ObjectRect.Width()/2);
	dy = m_ObjectRect.bottom + (m_ObjectRect.Width()/2);
	
	dx +=  (m_ObjectRect.Width()/2)/1.414;
	dy +=  (m_ObjectRect.Width()/2)/1.414;
    
	x = (INT)dx;
	y = (INT)dy;
	
	rect.top = y - picksize; 	
	rect.bottom = y + picksize;
	rect.left = x - picksize;
	rect.right = x + picksize;

	pDC->Rectangle(rect);


	//5
	x = m_ObjectRect.left + (m_ObjectRect.Width()/2);
	y = m_ObjectRect.top ;

	rect.top = y + picksize; 	
	rect.bottom = y-picksize;
	rect.left = x - picksize;
	rect.right = x+picksize;

	pDC->Rectangle(rect);
	
	
	//6
	dx = m_ObjectRect.left + (m_ObjectRect.Width()/2);
	dy = m_ObjectRect.bottom + (m_ObjectRect.Width()/2);
	
	dx -=  (m_ObjectRect.Width()/2)/1.414;
	dy +=  (m_ObjectRect.Width()/2)/1.414;
    
	x = (INT)dx;
	y = (INT)dy;
	
	rect.top = y - picksize; 	
	rect.bottom = y + picksize;
	rect.left = x - picksize;
	rect.right = x + picksize;
	
	pDC->Rectangle(rect);

	//7
	x = m_ObjectRect.left ;
	y = m_ObjectRect.top + ( m_ObjectRect.Height()/2);

	rect.top = y + picksize; 	
	rect.bottom = y-picksize;
	rect.left = x - picksize;
	rect.right = x+picksize;
    
	pDC->Rectangle(rect);

   
	//8
	dx = m_ObjectRect.left + (m_ObjectRect.Width()/2);
	dy = m_ObjectRect.bottom + (m_ObjectRect.Width()/2);
	
	dx -=  (m_ObjectRect.Width()/2)/1.414;
	dy -=  (m_ObjectRect.Width()/2)/1.414;
    
	x = (INT)dx;
	y = (INT)dy;
	
	rect.top = y - picksize; 	
	rect.bottom = y + picksize;
	rect.left = x - picksize;
	rect.right = x + picksize;
    
    pDC->Rectangle(rect);


	return 0;
}



INT CSPFObject::SPFDrawPathPickPoint(CDC *pDC)
{
	
	
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
  
/*
    ReturnDrawObject(pDC);	
	SetActivatedObject(pDC);
   	
	if(m_pt1Activated == _T("T"))
	{
		 //1	
		x = m_pt1.x;
		y = m_pt1.y;

		rect.top = y + 5; 	
		rect.bottom = y-5;
		rect.left = x - 5;
		rect.right = x+5;
    
		pDC->Rectangle(rect);
    }
	
    if(m_pt3Activated == _T("T"))
	{
       
		//3
		x = m_pt3.x;
		y = m_pt3.y;


		rect.top = y + 5; 	
		rect.bottom = y-5;
		rect.left = x - 5;
		rect.right = x+5;
    
		pDC->Rectangle(rect);
	}

*/
	return 0;
}


INT CSPFObject::ReSizeObject(INT PickIndex, CPoint point)
{
    
   if (m_Selected)
	{
		if(m_ObjectType == 'G')
		{
		    ReSizeGroupObject(PickIndex,point);
        }
		else if(m_ObjectType == 'S')
		{
            ReSizeStateObject(PickIndex,point);
		}
		else if(m_ObjectType == 'P')
		{
            ReSizePathObject(PickIndex,point);
		}
	}

	return 0;
}


INT CSPFObject::ReSizeGroupObject(INT PickIndex, CPoint point)
{
    switch(PickIndex)
	{
	case 1:
		m_ObjectRect.top = point.y;
        m_ObjectRect.left = point.x;
    break;
	case 2:
        m_ObjectRect.top = point.y;
        m_ObjectRect.right = point.x;

	break;
	case 3:
        m_ObjectRect.bottom = point.y;
        m_ObjectRect.right = point.x;
    
	break;
	case 4:
	    m_ObjectRect.bottom = point.y;
        m_ObjectRect.left = point.x;
	break;
	}

    return 0;
}


INT CSPFObject::ReSizeStateObject(INT PickIndex, CPoint point)
{
    switch(PickIndex)
	{
	case 1:
		

    break;
	
	case 2:


	break;
	
	case 3:

    
	break;
	
	case 4:
	
	break;
	}



    return 0;
}


INT CSPFObject::ReSizePathObject(INT PickIndex, CPoint point)
{
   switch(PickIndex)
	{
	case 1:
		m_pt1.x = point.x;
		m_pt1.y = point.y;
    break;
	case 2:
        m_pt2.x = point.x;
		m_pt2.y = point.y;
	break;
	case 3:
        m_pt3.x = point.x;
		m_pt3.y = point.y;
 	break;
	}

    return 0;
}

INT CSPFObject::SetDrawObject(CDC *pDC)
{
    m_pen.CreatePen( PS_SOLID , 1 , RGB(0,0,0));
	pOldPen = pDC->SelectObject(&m_pen);
	//TRACE(_T("m_pen.CreatePen(); \n"));

	return 0;
}

INT CSPFObject::SetUnDrawObject(CDC *pDC)
{

    m_pen.CreatePen( PS_SOLID , 1 , RGB(255,255,255));
	pOldPen = pDC->SelectObject(&m_pen);
	//TRACE(_T("m_pen.CreatePen(); \n"));


	return 0;
}

INT CSPFObject::SetUnDrawPathObject(CDC *pDC)
{
    m_pen.CreatePen( PS_SOLID , 2 , RGB(255,255,255));
	pOldPen = pDC->SelectObject(&m_pen);

	return 0;
}


INT CSPFObject::ReturnDrawObject(CDC *pDC)
{

	pDC->SelectObject(&pOldPen);
	m_pen.DeleteObject();
	//TRACE(_T("m_pen.DeleteObject(); \n"));
	return 0;
}

