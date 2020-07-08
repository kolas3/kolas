// SIReportObjManager.cpp: implementation of the CSIReportObjManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportObjManager.h"
#include "LoanCardPrinter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSIReportObjManager::CSIReportObjManager( VOID *parent )
{
	m_pParent = parent;
	m_pWorkForm = NULL;
	m_pWorkSheet = NULL;
}

CSIReportObjManager::~CSIReportObjManager()
{
	FreeWorkForm();
}

INT CSIReportObjManager::Print( CSIReportManager *pSIReportManager )
{
	INT userIndex = 0;
	for( INT i = 0; i < m_pageCnt; i++ )
	{
		InitWorkForm();

		for( INT j = 0; j < m_cardCntOnPage && userIndex < m_userCnt; j++, userIndex++ )
			for( INT k = 0; k < m_objCntOnCard; k++ )
				AddObjectToSheet( userIndex, j, k );

		SaveToFile( i );
	}

	return 0;
}

VOID CSIReportObjManager::InitWorkForm()
{
	FreeWorkForm();
	
	m_pWorkForm = new CSIReportWorkForm( MASTER_IS_MANAGER );
	m_pWorkForm->AddNewSheet();
	m_pWorkSheet = m_pWorkForm->GetWorkSheet( 0 );

	m_pWorkForm->paper = PAPER_A4_PORTRAIT;
}

INT CSIReportObjManager::SaveToFile( INT pageIdx )
{
	if( !m_pWorkForm )
		return -1;
	
	CSIReportXMLIOMgr *pXMLIOMgr = new CSIReportXMLIOMgr( MASTER_IS_MANAGER );
	if( !pXMLIOMgr )
		return -2;

	CString fileName;
	fileName.Format( _T("rpt_tmp\\page%04d.xml"), ++pageIdx );
	
	pXMLIOMgr->SaveToXML( m_pWorkForm, fileName );
	if( pXMLIOMgr )
	{
		delete pXMLIOMgr;
		pXMLIOMgr = NULL;
	}

	return 0;
}

VOID CSIReportObjManager::FreeWorkForm()
{
	if( m_pWorkForm )
	{
		delete m_pWorkForm;
		m_pWorkForm = NULL;
	}
}

INT CSIReportObjManager::AddObjectToSheet( INT userIndex, INT locate, INT objIndex )
{
	CLoanCardPrinter *pParent = ( CLoanCardPrinter * )m_pParent;

	CSIReportObject *pObj = ( CSIReportObject * )m_pWorkSheet->AddNewObject( pParent->GetObjType( objIndex ) );
	pParent->SetObjectInfo( pObj, userIndex, locate, objIndex );
	
	return 0;
}
