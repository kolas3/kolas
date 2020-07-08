// ControlObjectMgr.cpp: implementation of the CControlObjectMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ControlObjectMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlObjectMgr::CControlObjectMgr()
{
	pObjectList = NULL;
	m_nObjectCnt = 0;
}

CControlObjectMgr::~CControlObjectMgr()
{
	DeleteObjectList();
}




INT CControlObjectMgr::DeleteObjectList()
{
	for( INT i = 0 ; i < m_nObjectCnt ; i++ )
	{
		delete pObjectList[i];
	}
	delete []pObjectList;

	pObjectList = NULL;
	m_nObjectCnt = 0;

	return 0;
}



INT CControlObjectMgr::MakeControlObjectList( CESL_DataMgr* pCardDM , INT nMode )
{
	if( pObjectList != NULL )
		DeleteObjectList();

	m_nObjectCnt = pCardDM->GetRowCount();

	pObjectList = new CControlObject*[m_nObjectCnt];

	CString sType , strWidth , strHeight;
	for( INT i = 0 ; i < m_nObjectCnt ; i++ )
	{
		pCardDM->GetCellData( _T("타입") , i , sType );

		// 2009.10.07 ADD BY KSJ : 넓이/높이의 값이 없거나 0일 경우 사진은 출력 안 한다
		pCardDM->GetCellData( _T("넓이") , i , strWidth );
		pCardDM->GetCellData( _T("높이") , i , strHeight );
		if(		_T("0") == strWidth		|| TRUE == strWidth.IsEmpty()
			||	_T("0") == strHeight	|| TRUE == strHeight.IsEmpty()	)
		{
			pCardDM->SetCellData( _T("파일경로") , _T("") , i );
		}

		// 객체 생성
		pObjectList[i] = CreateInstanceFormType( sType );
		
		// 알맞는 값 집어넣기
		InsertValueFromDM( pObjectList[i] , pCardDM , i , nMode );
	}

	return 0;
}


CControlObject* CControlObjectMgr::CreateInstanceFormType( CString sType )
{
	if( sType.Compare(TYPE_LINE) == 0 ) 
		return new CLine;
	else if( sType.Compare(TYPE_TEXTBOX) == 0 ) 
		return new CTextBox;
	else if( sType.Compare(TYPE_BARCODE) == 0 ) 
		return new CBarCode;
	else if( sType.Compare(TYPE_BITMAP) == 0 ) 
		return new CBitMap;
	else if( sType.Compare(TYPE_BACKGROUND) == 0 ) 
		return new CBackGround;
	
	return NULL;
}

INT CControlObjectMgr::InsertValueFromDM( CControlObject* pObject , CESL_DataMgr* pCardDM , INT nIndex , INT nMode )
{
	pObject->InsertValue( pCardDM , nIndex , nMode );

	return 0;
}




INT CControlObjectMgr::DrawDC( CDC* pDC )
{
	for( INT i = 0 ; i < m_nObjectCnt ; i++ )
	{
		pObjectList[i]->DrawDC( pDC );
	}

	return 0;
}