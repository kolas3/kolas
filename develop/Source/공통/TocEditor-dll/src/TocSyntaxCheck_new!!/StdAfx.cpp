// stdafx.cpp : source file that includes just the standard includes
//	TocSyntaxCheck.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CString ConvertID2Chinese(UINT nID)	// 중국어 출력 위해 추가
{
	CString stmp;
	stmp.LoadString(nID);
	return stmp;
}



