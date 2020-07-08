// ESL_DataVerifyMgr.cpp: implementation of the CESL_DataVerifyMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include _T("esl.h")
#include "ESL_DataVerifyMgr.h"
#include "ESL_Mgr.h"
#include "ESL_DVM_VIEWER.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_DataVerifyMgr::CESL_DataVerifyMgr(CESL_Mgr * pWnd)
{
	m_pWnd = (CESL_Mgr *)pWnd;
	
	OldDM = new CESL_DataMgr();
	NewDM = new CESL_DataMgr();
	TmpDM = new CESL_DataMgr();
	Viewer = new CESL_DVM_VIEWER(this);
	
	ViewFlag = false;
	
}



CESL_DataVerifyMgr::~CESL_DataVerifyMgr()
{
    FreeDVMRef();
    
	delete OldDM;
	OldDM = NULL;

    delete NewDM;
	NewDM = NULL;
    
	delete TmpDM;
	TmpDM = NULL;

	delete Viewer;
	Viewer = NULL;
	
}

INT CESL_DataVerifyMgr::InitDVMAliasFromMemory(CString dvm_alias)
{
     
	CESL_DataMgr *pDM;
	
	pDM = m_pWnd->m_pInfo->pESLDataVerifyMgr;
	INT totalcount = pDM->GetRowCount();
	CString szbuf1, szbuf2;
		
	if (pDM == NULL) return -1; 
	
	for (INT i = 0; i < totalcount; i++) {
		szbuf1 = pDM->GetCellData(i, 1);
		
		if (szbuf1 == dvm_alias) {
			
			DB_VERIFY_MGR_ALIAS = szbuf1;

			szbuf2 = pDM->GetCellData(i, 0);
			PK = szbuf2;				
			szbuf2 = pDM->GetCellData(i, 2);	
			SetCONNECTION_INFO(szbuf2);			
			szbuf2 = pDM->GetCellData(i, 3);
			TBL_NAME = szbuf2;
			szbuf2 = pDM->GetCellData(i, 4);
			CONDITION = szbuf2;
			szbuf2 = pDM->GetCellData(i, 5);
			EXTRA_CONDITION = szbuf2;
			break;
		}
	}
	
	FreeDVMRef();
	
	reference * pRef;
	
	for (i = 0; i < totalcount; i++) {
		pRef = NULL;
		szbuf1 = pDM->GetCellData(i, 1);
	
		if (szbuf1 == dvm_alias) {
			
			pRef = new reference;
			
			if (!pRef) {
				FreeDVMRef();
				return -2;
			}
			
			pRef->FIELD_ALIAS = pDM->GetCellData(i, 6);
			pRef->FIELD_NAME = pDM->GetCellData(i, 7);
			pRef->FIELD_TYPE = pDM->GetCellData(i, 8);
			pRef->INIT_VALUE = pDM->GetCellData(i, 9);
			pRef->DATA_TYPE = pDM->GetCellData(i, 10);
            pRef->FILTER1 = pDM->GetCellData(i, 11);
			pRef->FILTER2 = pDM->GetCellData(i, 12);
            
			RefList.AddTail((CObject*)pRef);
		}
	}
	
    InitRefDM();

	m_strHtmlFormat = _T("");

	return 0;
	        
}

INT CESL_DataVerifyMgr::FreeDVMRef()
{
   INT refcount = RefList.GetCount();
	if (refcount == 0) return 0;
	reference * pRef;
	for (INT i = 0; i < refcount; i++) {
		pRef = (reference*)RefList.RemoveHead();
		if (pRef)
		{
			delete pRef;
			pRef = NULL;
		}
	}
	RefList.RemoveAll();
	return(refcount);

}

VOID CESL_DataVerifyMgr::SetCONNECTION_INFO(CString info)
{
	if (info.GetLength() == 0) return;
	CONNECTION_INFO = info;
	ParseCONNECTION_INFO();
}


VOID CESL_DataVerifyMgr::ParseCONNECTION_INFO(VOID)
{
	if (CONNECTION_INFO.GetLength() == 0) return;
	INT curpos=0, newpos=0;
	CString strbuf;

	newpos = CONNECTION_INFO.Find(_T("@"), curpos);
	m_dbid = CONNECTION_INFO.Mid(curpos, newpos-curpos);
	curpos = newpos+1;

	newpos = CONNECTION_INFO.Find(_T("/"), curpos);
	m_dbservice = CONNECTION_INFO.Mid(curpos, newpos-curpos);
	curpos = newpos+1;
	
	newpos = CONNECTION_INFO.Find(_T("["), curpos);
	m_dbpwd = CONNECTION_INFO.Mid(curpos, newpos-curpos);
	curpos = newpos+1;

	newpos = CONNECTION_INFO.Find(_T("]"), curpos);
	m_dbhost = CONNECTION_INFO.Mid(curpos, newpos-curpos);
}


INT CESL_DataVerifyMgr::InitRefDM()
{
	
	OldDM->InitDMPKFromMemory(m_pWnd->m_pInfo->pESLDataVerifyMgr,PK);
	NewDM->InitDMPKFromMemory(m_pWnd->m_pInfo->pESLDataVerifyMgr,PK);
    TmpDM->InitDMPKFromMemory(m_pWnd->m_pInfo->pESLDataVerifyMgr,PK);
   
	
	OldDM->m_nCols = OldDM->RefList.GetCount();
    NewDM->m_nCols = NewDM->RefList.GetCount();    
    	
	DoRollBack = FALSE;

	ModifyCount=0;
	return 0;
}

INT	CESL_DataVerifyMgr::StartPointCheck()
{
   TmpDM->RefreshDataManager(TmpQry);   
   
   for(INT i=0; i<TmpDM->GetRowCount(); i++)
   {
       OldDM->InsertRow(-1);
	   for(INT j=0; j<TmpDM->GetColCount(); j++)
	   {
         OldDM->SetCellData(i+ModifyCount,j,TmpDM->GetCellData(i,j));
	   }
   }

   return 0;
}

INT	CESL_DataVerifyMgr::EndPointCheck()
{
	TmpDM->RefreshDataManager(TmpQry);
	
   for(INT i=0; i<TmpDM->GetRowCount(); i++)
   {
	    NewDM->InsertRow(-1);
		for(INT j=0; j<TmpDM->GetColCount(); j++)
	   {
         NewDM->SetCellData(i+ModifyCount,j,TmpDM->GetCellData(i,j));
	   }
   }

    ModifyCount++;
     	
	return 0;

}

INT CESL_DataVerifyMgr::SetKeyField(CString Field , CString Type, CString Data )
{
    TmpQry = _T("");

	CString TmpStr;
	Type.MakeUpper();

	if ( TmpQry == _T("") )
	{
		if (Type == _T("STRING")) TmpQry.Format(_T(" %s = '%s' "),  Field , Data);
		if (Type == _T("NUMERIC")) TmpQry.Format(_T(" %s = %s "),  Field , Data);
	}
	else
	{
		if (Type == _T("STRING")) TmpStr.Format(_T(" %s = '%s' "),  Field , Data);
		if (Type == _T("NUMERIC")) TmpStr.Format(_T(" %s = %s "),  Field , Data);

		TmpQry += _T(" OR ");
		TmpQry += TmpStr;
	}

	return 0;
}

INT CESL_DataVerifyMgr::FilteringData(CESL_DataMgr * NewDM)
{
     	
	COLORREF blue,red;
			
	blue = RGB(228,243,251);
    red = RGB(255,200,200);
	
	reference *TmpRef;
	CString TmpData,TmpFilter;
    CString filtermessage;
    CString TmpOldData;
	CString tmpHtmlData;
	
	for(INT i=0 ; i < NewDM->GetRowCount() ; i++)
	{
		m_strHtmlFormat += _T("<TR>");

		for(INT j=0 ; j < NewDM->GetColCount() ; j++)
		{

			TmpData = NewDM->GetCellData(i,j);
			TmpOldData = OldDM->GetCellData(i,j);


			if(TmpData == TmpOldData)
				tmpHtmlData.Format(_T("<TD>%s"),TmpData);
			else
				tmpHtmlData.Format(_T("<TD BGCOLOR = \"E4F3FB\">%s"),TmpData);	



			TmpRef = (reference *)RefList.GetAt(RefList.FindIndex(j));
			TmpFilter = TmpRef->FILTER1;
	        
            		
			if(TmpFilter == _T("testfilter") && TmpData.IsEmpty() == TRUE)
			{
			        
				tmpHtmlData.Format(_T("<TD BGCOLOR = \"FFC8C8\">%s"),TmpData);


				Viewer->m_grid_new.SetRow(i+2);
				Viewer->m_grid_new.SetCol(j+1);
				Viewer->m_grid_new.SetCellBackColor(red);
                Viewer->m_grid_new.SetRow(1);
				Viewer->m_grid_new.SetCellBackColor(red);
				    
				if(DoRollBack != TRUE) DoRollBack = TRUE;
			
			}
              
			m_strHtmlFormat += tmpHtmlData;

        }

    }
    return 0;

}

CString CESL_DataVerifyMgr::MakeUpdateQueryStr(CString FieldName, CString TmpTblName,CString Data, CString Condition)
{
             
		CString Qry;
        if(Data == _T(""))
		{ 
		Qry.Format(_T("update %s set %s = NULL where %s;"),TmpTblName,FieldName,Condition);
        }
		else
		{
		Qry.Format(_T("update %s set %s = '%s' where %s;"),TmpTblName,FieldName,Data,Condition);
        }
		
		return Qry;

}
 
CString CESL_DataVerifyMgr::MakeUpdateQueryInt(CString FieldName,CString TmpTblName, CString Data, CString Condition)
{
             
       	CString Qry;
        if(Data == _T(""))
		{ 
		Qry.Format(_T("update %s set %s = NULL where %s;"),TmpTblName,FieldName,Condition); 
        }
		else
		{
		Qry.Format(_T("update %s set %s = %s where %s;"),TmpTblName,FieldName,Data,Condition);
        }

        return Qry;

} 

INT CESL_DataVerifyMgr::DoRollBackData()
{
    	
	reference *TmpRef;
	CString OldData,TmpFieldName,TmpFieldType,AddFrameQry,TmpTblName,TmpCondition,TmpTblAlias;
    CString rollbackmessage;

	OldDM->StartFrame();
	
	for(INT i=0 ; i < OldDM->GetRowCount() ; i++)
	{
		for(INT j=0 ; j < OldDM->GetColCount() ; j++)
		{	
		            TmpRef = (reference *)RefList.GetAt(RefList.FindIndex(j));
		            
			        OldData = OldDM->GetCellData(i,j);
					TmpFieldName =  Field_Name_Parsing(TmpRef->FIELD_NAME);
                    TmpFieldType = TmpRef->FIELD_TYPE;
                    TmpTblName = Tbl_Name_Parsing(TmpRef->FIELD_NAME);
                    TmpTblAlias =  Tbl_Alias_Parsing(TmpRef->FIELD_NAME);
                    
                    TmpCondition = GetUpdateCondition(TmpTblAlias,i);  
                                        
					TmpFieldType.MakeUpper();

                    if(TmpFieldType == _T("STR"))
					{
					 AddFrameQry = MakeUpdateQueryStr(TmpFieldName,TmpTblName, OldData,TmpCondition);
					}
					else
					{
                     AddFrameQry = MakeUpdateQueryInt(TmpFieldName,TmpTblName, OldData,TmpCondition);
					}
                     
					OldDM->AddFrame(AddFrameQry);		
		}
	}
	
	OldDM->SendFrame();
    OldDM->EndFrame();
    	
	rollbackmessage.Format(_T("모든 데이터를 이전상태로 돌려놓았습니다"));  
	AfxMessageBox(rollbackmessage);

	return 0;
}

CString CESL_DataVerifyMgr::Field_Name_Parsing(CString FieldName)
{
     
     INT curpos=0;
     INT newpos=0; 
	 INT length;

	 CString buf;
     
	 FieldName.TrimLeft();
	 FieldName.TrimRight();

	 newpos = FieldName.Find(_T("."),curpos);
     if(newpos == -1) return FieldName;
     
	 length = FieldName.GetLength();
	 buf = FieldName.Mid(newpos+1,length-newpos);
     buf.TrimLeft();
     buf.TrimRight();

	 return buf;
	
}

CString CESL_DataVerifyMgr::Tbl_Alias_Parsing(CString FieldName)
{
     
     INT curpos=0;
     INT newpos=0; 
	

	 CString buf = _T("");
     
	 FieldName.TrimLeft();
	 FieldName.TrimRight();

	 newpos = FieldName.Find(_T("."),curpos);
     if(newpos == -1) return buf;
    	
	 buf = FieldName.Left(newpos+1);
     buf.TrimLeft();
     buf.TrimRight();

	 return buf;
	
}

CString CESL_DataVerifyMgr::Tbl_Name_Parsing(CString FieldName)
{
     	 	
	 INT pos,length;
	 INT curpos=0;
     INT newpos=0; 
	 INT startpos=0;
	 INT endpos=0;
	 CString alias,tmpalias,buf,tblname;
     TBL_NAME.TrimLeft();
	 TBL_NAME.TrimRight();
	 length = TBL_NAME.GetLength();
 
	 newpos = FieldName.Find(_T("."),curpos);
     if(newpos == -1) return TBL_NAME;
     	 	 
	 alias = FieldName.Left(newpos);
     alias.TrimLeft();
     alias.TrimRight();
     tmpalias.Format(_T(" %s"),alias);
     
     
     endpos = TBL_NAME.Find(_T(","),startpos);
     
	 
	 while(1)
	 {
     
	 if(endpos == -1) break;
	 
	 buf = TBL_NAME.Mid(startpos,endpos-startpos);
     buf.TrimLeft();
     buf.TrimRight();
     pos = buf.Find(tmpalias,0);
     
	 if(pos>0) 
	 {
        pos = buf.Find(_T(" "),0);
        tblname = buf.Mid(0,pos);
        return tblname;     
      
	 }
     

	 startpos = endpos+1;
         
	 endpos = TBL_NAME.Find(_T(","),startpos);
     
	 if(endpos == -1) { 
	    
		 buf = TBL_NAME.Mid(startpos,length-startpos);
	     buf.TrimLeft();
         buf.TrimRight();
         pos = buf.Find(tmpalias,0);
     
		 if(pos>0) 
		 {
			pos = buf.Find(_T(" "),0);
			tblname = buf.Mid(0,pos);
			return tblname;     
        		 
		 }
	 	 
	 }
	    
	 }
     
	 return TBL_NAME;

}

INT CESL_DataVerifyMgr::EndDVM()
{
    	
	if( ViewFlag ) 
	{
		delete Viewer;
		Viewer = NULL;
		Viewer = new CESL_DVM_VIEWER(this);
	}

	Viewer->Create(m_pWnd);
	Viewer->ShowWindow(SW_SHOW);
    
	FilteringData(NewDM);
	
	Viewer->UpdateWindow();
	
	CString strHtml;
	GetHtmlFormatData(strHtml);
    FileLog(strHtml);
   

	if(DoRollBack == TRUE) return -1;

	return 0;

}

CString CESL_DataVerifyMgr::GetUpdateCondition(CString TblAlias, INT rowidx)
{
    CString WhereQry,PKField,PKValue,PKType;
		
	if(TblAlias.IsEmpty() == TRUE)
	{
         	  	
      reference *TmpRef;
      INT i = 0;
            
	  for(i=0; i<RefList.GetCount(); i++)
	  {
	  
	      TmpRef = (reference *)RefList.GetAt(RefList.FindIndex(i));
		  if(TmpRef->FIELD_ALIAS == _T("PK")) 
		  { 
			   PKField =TmpRef->FIELD_NAME;
			   PKValue = OldDM->GetCellData(_T("PK"),rowidx);
			   PKType = TmpRef->FIELD_TYPE;  
			   PKType.MakeUpper();  
		  }
      
	  }
      
	  if(PKField.IsEmpty() == TRUE) AfxMessageBox(_T("해당 DVM에서 PK란 Field Alias를 찾을 수 없습니다"));
      
	  if (PKType == _T("STR")) 
	  {
		  WhereQry.Format(_T(" %s = '%s' "),  PKField , PKValue);
	  }
	  else
	  {
		  WhereQry.Format(_T(" %s = %s "),  PKField , PKValue);
	  }
      
	  
	  return WhereQry;
    }
    else 
	{
      
	  CString FieldAlias; 
      FieldAlias.Format(_T("%sPK"),TblAlias);
      

      reference *TmpRef;
      INT j = 0;
            
	  for(j=0; j<RefList.GetCount(); j++)
	  {
	      
	      TmpRef = (reference *)RefList.GetAt(RefList.FindIndex(j));
		  if(TmpRef->FIELD_ALIAS == FieldAlias) 
		  { 
			   PKField = Field_Name_Parsing(TmpRef->FIELD_NAME);
			   PKValue = OldDM->GetCellData(FieldAlias,rowidx);
			   PKType = TmpRef->FIELD_TYPE;  
			   PKType.MakeUpper();  
		  }
      
	  }
      
      if(PKField.IsEmpty() == TRUE) AfxMessageBox(_T("해당 DVM에서 PK란 Field Alias를 찾을 수 없습니다"));

      if (PKType == _T("STR")) 
	  {
		  WhereQry.Format(_T(" %s = '%s' "),  PKField , PKValue);
	  }
	  else
	  {
		  WhereQry.Format(_T(" %s = %s "),  PKField , PKValue);
	  }

	  return WhereQry;
	}
   
}


INT CESL_DataVerifyMgr::GetHtmlFormatData(CString &strResult)
{
	
	CString strTmpOldData;
	CString strTmpAlias;
	CString strTmpFilter;
	CString strTmp;

	strTmpOldData = _T("");
	strTmpAlias = _T("<TR>");
	strTmpFilter = _T("<TR>");
    strTmp = _T("");

	strResult = _T("<TABLE>");
	reference *TmpRef;

	for(INT i=0;i<RefList.GetCount();i++)
	{
		TmpRef = (reference *)RefList.GetAt(RefList.FindIndex(i));
			
		strTmp.Format(_T("<TD>%s"),TmpRef->FIELD_ALIAS);
		strTmpAlias += strTmp;
      		
		strTmp.Format(_T("<TD>%s"),TmpRef->FILTER1);
		strTmpFilter += strTmp;

		strTmp = _T("");

	}

	for(i=0 ; i < OldDM->GetRowCount() ; i++)
	{
		strTmpOldData += _T("<TR>");
		for(INT j=0 ; j < OldDM->GetColCount() ; j++)
		{
			
			strTmp.Format(_T("<TD>%s"),OldDM->GetCellData(i,j));
			strTmpOldData += strTmp;
			strTmp = _T("");
		}
	}

	strResult += strTmpAlias;
	strResult += strTmpOldData;
	strResult += strTmpFilter;
	strResult += m_strHtmlFormat;
	strResult += _T("</TABLE>");

	return 0;
}

VOID CESL_DataVerifyMgr::FileLog(CString strMsg)
{
	FILE *fd = _tfopen(_T("D:\\Kolas2up\\bin\\DVM_HTML.TXT"), _T("a+b"));
	if(!fd) return;
	CTime t = CTime::GetCurrentTime();

		fseek(fd, 0, SEEK_END);		
		if( 0 == ftell(fd) )
		{
			fputc(0xFF, fd);
			fputc(0xFE, fd);
		}

	_ftprintf(fd, _T("[%d/%d/%d - %d:%d:%d] %s\r\n"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), strMsg);
	fclose(fd);
	return;
}