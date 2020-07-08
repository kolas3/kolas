// ESL_ControlMgr.cpp: implementation of the CESL_ControlMgr class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ESL_ControlMgr.h"
#include "efs.h"
#include "ESL_Grid.h"
#include "ESL_Grid2.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_ControlMgr::CESL_ControlMgr(CWnd* pParent /*=NULL*/)
{
	m_pParent = pParent;
}

CESL_ControlMgr::~CESL_ControlMgr()
{
	FreeControlElementList();
}

CString CESL_ControlMgr::GetAlias(VOID)
{
EFS_BEGIN

	EFS_BEGIN

	return CM_ALIAS;

	EFS_END
	return _T("");


EFS_END
return _T("");

}

VOID CESL_ControlMgr::SetAlias(CString alias)
{
EFS_BEGIN

	EFS_BEGIN

	CM_ALIAS = alias;

	EFS_END

EFS_END
}

INT CESL_ControlMgr::LoadReference(CString pk)
{
EFS_BEGIN

	EFS_BEGIN

	if (pk.GetLength() == 0) return -3001;
	CM_PK = pk;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(CONNECTION_INFO);
	CString query;

	INT ir;
	query.Format(_T("SELECT CTRL_MGR_ALIAS FROM ESL_CTRL_MGR_TBL WHERE PK=%s"), pk);
	ir = dm.RestructDataManager(query);
	if (ir < 0) return -3002;
	CM_ALIAS = dm.GetCellData(0, 0);

	query.Format(_T("SELECT CTRL_ALIAS, CTRL_TYPE, CTRL_ID, CTRL_STYLE, INIT_VALUE, LINK_DATA, LIST_DATA, VALIDATION FROM ESL_CTRL_MGR_SUB_TBL WHERE CTRL_MGR_PK=%s"), CM_PK);
	ir = dm.RestructDataManager(query);
	if (ir < 0) return -3002;
	CESL_Control_Element * newCME;
	INT totalcount = dm.GetRowCount();
	for (INT i = 0; i < totalcount; i++) {
		newCME = NULL;
		newCME = new CESL_Control_Element(m_pParent);
		if (newCME == NULL) continue;

		newCME->CTRL_ALIAS		=	dm.GetCellData(i, 0);
		if (dm.GetCellData(i, 2).GetLength() > 0) {
			newCME->CTRL_ID			= _ttoi(dm.GetCellData(i, 2));
		}
		newCME->CTRL_STYLE		=	dm.GetCellData(i, 3);
		newCME->CTRL_TYPE		=	dm.GetCellData(i, 1);
		newCME->INIT_VALUE		=	dm.GetCellData(i, 4);
		newCME->LINK_DATA		=	dm.GetCellData(i, 5);
		newCME->LIST_DATA		=	dm.GetCellData(i, 6);
		newCME->VALIDATION		=	dm.GetCellData(i, 7);
		newCME->InitControl();
		Control_List.AddTail((CObject*)newCME);
		
	}
	return 0;

	EFS_END
	return -1;


EFS_END
return -1;

}

INT CESL_ControlMgr::LoadReferenceFromMemory(CESL_DataMgr *pDM, CString cm_pk)
{
	EFS_BEGIN
		
		EFS_BEGIN
		
		INT i;
	INT totalcount = pDM->GetRowCount();
	CString szbuf1, szbuf2;

	TCHAR * cell = NULL;
	DWORD *row;
	CString strTmp;

	POSITION pos = pDM->data.GetHeadPosition();

	for (i = 0; i < totalcount; i++) {
		row = NULL;
		row = (DWORD*)pDM->data.GetNext(pos);

		if (cm_pk.Compare((TCHAR*)row[0]) == 0) {
			CM_ALIAS.Format(_T("%s"), (TCHAR *)row[1]);
			break;
		}
	}
	CESL_Control_Element *newCME;
	pos = pDM->data.GetHeadPosition();

	for (i = 0; i < totalcount; i++) {
		row = NULL;
		row = (DWORD*)pDM->data.GetNext(pos);

		newCME = NULL;
		if (cm_pk.Compare((TCHAR*)row[0]) == 0) {
			newCME = new CESL_Control_Element(m_pParent);
			if (newCME == NULL) {
				FreeControlElementList();
				return (-1);
			}
			newCME->CM_ALIAS = CM_ALIAS;
			newCME->CTRL_ALIAS.Format(_T("%s"), (TCHAR *)row[2]);
			newCME->CTRL_TYPE.Format(_T("%s"), (TCHAR *)row[3]);
			newCME->CTRL_ID = _ttoi((TCHAR *)row[4]);
			newCME->CTRL_STYLE.Format(_T("%s"), (TCHAR *)row[5]);
			newCME->INIT_VALUE.Format(_T("%s"), (TCHAR *)row[6]);
			newCME->LINK_DATA.Format(_T("%s"), (TCHAR *)row[8]);
			newCME->LIST_DATA.Format(_T("%s"), (TCHAR *)row[7]);
			newCME->VALIDATION.Format(_T("%s"), (TCHAR *)row[9]);
			newCME->REQUIRECHECK.Format(_T("%s"), (TCHAR *)row[10]);

			newCME->REQUIRECHECK.TrimLeft();		newCME->REQUIRECHECK.TrimRight();
			newCME->InitControl();
			Control_List.AddTail((CObject*)newCME);
		}
	}
	return 0;

	EFS_END
	return -1;


EFS_END
return -1;

}

INT CESL_ControlMgr::LoadReferenceFromFile(CString cm_pk)
{
EFS_BEGIN

	CString strFileName;
	CString strLine;
	CString strTmp;
	strFileName.Format(_T("..\\CFG\\Reference\\CM\\%s.CM"), cm_pk);
	
	///
	CStdioFile fd;
	if (fd.Open(strFileName, CStdioFile::modeRead | CStdioFile::shareDenyWrite | CFile::typeBinary) == 0) return -1;
	TCHAR cUni;
	fd.Read( &cUni, sizeof(TCHAR) );
	if( 0xFEFF != cUni )
	{
		fd.SeekToBegin();
	}
	TCHAR szBuf[16009];
	INT pos;

	fd.ReadString(szBuf, 16000);
	fd.ReadString(szBuf, 16000);
	strLine.Format(_T("%s"), szBuf);
	strLine.TrimLeft();		strLine.TrimRight();
	pos = strLine.Find(_T("="));
	CM_ALIAS.Format(_T("%s"), strLine.Mid(pos+1));

	CESL_Control_Element *newCME;
	INT nStart = 0;
	while(TRUE) {
		if (fd.ReadString(szBuf, 16000) == NULL) break;
		if (szBuf[0] == '#') continue;
		strLine.Format(_T("%s"), szBuf);
		strLine.TrimLeft();			strLine.TrimRight();

		newCME = NULL;
		newCME = new CESL_Control_Element(m_pParent);
		if (newCME == NULL) continue;

		nStart = 0;
		newCME->CM_ALIAS = CM_ALIAS;

		pos = strLine.Find('|', nStart);
		newCME->CTRL_ALIAS = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		newCME->CTRL_TYPE = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		newCME->CTRL_ID = _ttoi(strLine.Mid(nStart, pos-nStart));
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		newCME->CTRL_STYLE = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		newCME->INIT_VALUE = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);

		strTmp = strLine.Mid(nStart, pos-nStart);
		strTmp.Replace(_T("~"),_T("|"));
		newCME->LIST_DATA = strTmp;
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		newCME->LINK_DATA = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		newCME->VALIDATION = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		pos = strLine.Find('|', nStart);
		newCME->REQUIRECHECK = strLine.Mid(nStart, pos-nStart);
		nStart = pos+1;

		newCME->REQUIRECHECK.TrimLeft();		newCME->REQUIRECHECK.TrimRight();
		newCME->InitControl();	

		Control_List.AddTail((CObject*)newCME);
	}
	fd.Close();

	return 0;

EFS_END
return -1;

}

INT CESL_ControlMgr::FreeControlElementList(VOID)
{
EFS_BEGIN

	EFS_BEGIN

	INT controlcount = Control_List.GetCount();
	if (controlcount == 0) return 0;
	CESL_Control_Element * pCE;
	for (INT i = 0; i < controlcount; i++) {
		pCE = (CESL_Control_Element*)Control_List.RemoveHead();
		if (pCE) delete pCE;
	}
	Control_List.RemoveAll();
	return(controlcount);

	EFS_END
	return -1;


EFS_END
return -1;

}

CESL_Control_Element *CESL_ControlMgr::FindControlElement(CString control_alias)
{
EFS_BEGIN

	EFS_BEGIN

	INT cecount = Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)Control_List.GetNext(pos);
		if (pCE) {
			if (pCE->GetAlias() == control_alias) return pCE;
		}
	}
	return (NULL);

	EFS_END
	return NULL;


EFS_END
return NULL;

}

CObject * CESL_ControlMgr::FindControl(CString control_alias)
{
EFS_BEGIN

	EFS_BEGIN

	CESL_Control_Element *pCE = FindControlElement(control_alias);
	if (pCE == NULL) return NULL;
	return pCE->Control;

	EFS_END
	return NULL;


EFS_END
return NULL;

}

INT CESL_ControlMgr::GetControlMgrData(CString alias, CString &data, INT index, INT col)
{
EFS_BEGIN

	EFS_BEGIN

	CESL_Control_Element *pCE = FindControlElement(alias);
	if (pCE == NULL) return -1;
	return pCE->GetControlMgrData(data, index, col);

	EFS_END
	return -1;


EFS_END
return -1;

}


INT CESL_ControlMgr::SetControlMgrData(CString alias, CString data, INT index, INT col)
{
EFS_BEGIN

	EFS_BEGIN

	CESL_Control_Element *pCE = FindControlElement(alias);
	if (pCE == NULL) return -1;
	return pCE->SetControlMgrData(data, index, col);

	EFS_END
	return -1;


EFS_END
return -1;

}

INT CESL_ControlMgr::ControlDisplay(CString alias, INT index)
{
EFS_BEGIN

	EFS_BEGIN

	CESL_Control_Element *pCME = FindControlElement(alias);
	if (pCME == NULL) return -1;
	return pCME->Display(index);

	EFS_END
	return -1;


EFS_END
return -1;

}

INT CESL_ControlMgr::AllControlDisplay(INT index)
{
EFS_BEGIN

	EFS_BEGIN

	INT cecount = Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)Control_List.GetNext(pos);
		if (pCE) {
			pCE->Display(index);
		}
	}
	return 0;

	EFS_END
	return -1;


EFS_END
return -1;

}

INT CESL_ControlMgr::AllControlReadOnly(VOID)
{
EFS_BEGIN

	EFS_BEGIN

	INT cecount = Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)Control_List.GetNext(pos);
		if (pCE) {
			pCE->SetControlReadOnly();
		}
	}
	return 0;

	EFS_END
	return -1;


EFS_END
return -1;

}

INT CESL_ControlMgr::GetAllControlMarcStream(CString m_strMarcGroupName, CMarc *pMarc)
{
EFS_BEGIN

	EFS_BEGIN

	CMarcMgr marcMgr;
	marcMgr.InitMarcMgr(m_strMarcGroupName);

	CString strTmp;
	INT ids;
	INT cecount = Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)Control_List.GetNext(pos);
		if (pCE) {
			ids = pCE->GetControlMgrData(strTmp);
			if (ids == 0) marcMgr.SetItem(pMarc, pCE->CTRL_ALIAS, strTmp);
		}
	}
	return 0;

	EFS_END
	return -1;


EFS_END
return -1;

}

INT CESL_ControlMgr::AllControlClear(VOID)
{
EFS_BEGIN

	EFS_BEGIN

	INT cecount = Control_List.GetCount();
	CESL_Control_Element * pCE = NULL;
	POSITION pos;
	pos = Control_List.GetHeadPosition();
	for(INT i = 0; i < cecount; i++) {
		pCE = NULL;
		pCE = (CESL_Control_Element*)Control_List.GetNext(pos);
		if (pCE) {
			pCE->Clear();
		}
	}
	return 0;

	EFS_END
	return -1;


EFS_END
return -1;

}
