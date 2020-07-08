// Kolas2up_ViewerControlData.cpp: implementation of the CKolas2up_ViewerControlData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Kolas2up_ViewerControlData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKolas2up_ViewerControlData::CKolas2up_ViewerControlData()
{
	TAG = _T("");
	DATA = _T("");

	cTAG_bk = RGB(255, 223, 206);
	cTAG_fk = RGB(0, 0, 0);

	cDATA_bk = RGB(239, 247, 247);
	cDATA_fk = RGB(0, 0, 0);
}

CKolas2up_ViewerControlData::~CKolas2up_ViewerControlData()
{

}
