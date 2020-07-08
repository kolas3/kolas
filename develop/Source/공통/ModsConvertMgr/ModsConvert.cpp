/**************************************************************************/
// ModsConvert.cpp -- managing MODS Convert created by SICI
//
//         Son Suhyong
//         ECO KOLIS Develope Team
//         Copyright (c) 2007.12 ECO Corporation
//
/**************************************************************************/
#include    "stdafx.h"
#include    "ModsConvert.h"
#include	"ModsInfo.h"

/**************************************************************************/
//    Contructor
/**************************************************************************/
CModsConvert::CModsConvert() : CDialog()
{
	m_ModsLen = 0;
	m_ModsIdx = 0;
}

/**************************************************************************/
//    Destructor
/**************************************************************************/
CModsConvert::~CModsConvert()
{
}

/*********************************************/
// MOSD to MARC Convert...
/*********************************************/
INT CModsConvert::ConvertMODStoMARC	(TCHAR *fi_ModsData, TCHAR *fo_MarcData)
{
	INT			ids;

	MODS_STRU	tMods;

	// 1.Get MODS Info...
	ids = GetParsingMODSStru( fi_ModsData, &tMods );
	if ( ids )
		return ids;

	// 2.Get Marc Data...
	ids = GetMarcData( &tMods, fo_MarcData );
	if ( ids )
		return ids;

	// 3.Free tMods
	ids = FreeMODSStru( &tMods );
	if ( ids )
		return ids;
		
	return 0;
}
/***********************************************************************************************************************/
// MODS PASSING FUNCTION...
/***********************************************************************************************************************/
/*********************************************/
// MOSD to MARC Convert...
/*********************************************/
INT	CModsConvert::GetParsingMODSStru(TCHAR *fi_ModsData, MODS_STRU *fo_tModsStru)
{
	INT				i;
	INT				ids;

	INT				iTemp;
	INT				nState;
	INT				iSubDep;
	INT				iTempDep;

	TCHAR			szNameTemp[32];

	MODS_SUB_STRU	tElement;
	MODS_SUB_STRU	*tSubCurrent;
	MODS_SUB_STRU	*tSubTemp;
	
	// 1.Data Init...
	nState = iTemp = -1;
	iSubDep = iTempDep = -1;
	tSubCurrent = tSubTemp = NULL;
	memset( szNameTemp, 0x00, sizeof(szNameTemp) );
		
	InitModsStru( fo_tModsStru, -1 );
	InitSubModsStru( &tElement, -1 );

	// 2.Get ModsData Length
	m_ModsLen = _tcslen( fi_ModsData );
	if ( m_ModsLen <= 0 )
		return -10;
	
	// 3.Get Element Data
	while ( 1 )
	{
		// nState(-1) : Non Match Element
		// nState( 0) : Element Start
		// nState(>0) : Element End
		ids = GetElementData( fi_ModsData, &tElement, &nState, &iSubDep, FALSE);
		if ( ids > 0 )
			break;
		if ( ids < 0)
			return ids;

		if ( nState == -1 )
		{
			iSubDep--;

			// MODS Info Pass...
			if ( _tcscmp( tElement.szName, _T("xml") ) != 0 || _tcscmp( tElement.szName, _T("mods") ) != 0 )
				continue;
			
			// Non Match Element Pass...
			_tcscpy( szNameTemp, tElement.szName );	//</ElementName>
			
			while(1)	
			{
				// TRUE : PassMods
				ids = GetElementData( fi_ModsData, &tElement, &nState, &iSubDep, TRUE);
				if ( ids > 0 )
					break;
				if ( ids < 0 )
					return ids;
				// Search </ElementName>
				if ( _tcscmp( tElement.szName, szNameTemp ) == 0 )
					break;
			}
		}
		else if ( nState == 0 )
		{
			// Add Sub Element...
			if ( iSubDep == 0 )
			{
				fo_tModsStru->nSubCnt++;
				fo_tModsStru->tModsSub = (MODS_SUB_STRU*) realloc( fo_tModsStru->tModsSub, sizeof(MODS_SUB_STRU) * fo_tModsStru->nSubCnt );
				tSubCurrent = &fo_tModsStru->tModsSub[fo_tModsStru->nSubCnt - 1];
				tSubCurrent->tFrontSub = NULL;
			}
			else if ( iSubDep != 0 )
			{
				tSubTemp = tSubCurrent;
				tSubCurrent->nSubCnt++;
				tSubCurrent->tNextSub = (MODS_SUB_STRU*) realloc( tSubCurrent->tNextSub, sizeof(MODS_SUB_STRU) * (tSubCurrent->nSubCnt) );
				tSubCurrent = &tSubCurrent->tNextSub[tSubCurrent->nSubCnt - 1];
				tSubCurrent->tFrontSub = tSubTemp;
			}

			memset( tSubCurrent->szName, 0x00, sizeof(tSubCurrent->szName) );
			_tcscpy( tSubCurrent->szName, tElement.szName );
			
			// Sub Element Set Type
			if ( tElement.nTypeCnt > 0 )
			{
				tSubCurrent->nTypeCnt = tElement.nTypeCnt;
				tSubCurrent->cType = (ATTRIBUTE_TYPE*) malloc( sizeof(ATTRIBUTE_TYPE) * tElement.nTypeCnt );
				
				for ( i = 0; i < tElement.nTypeCnt; i++ )
				{
					_tcscpy( tSubCurrent->cType[i].szName, tElement.cType[i].szName );
					_tcscpy( tSubCurrent->cType[i].szData, tElement.cType[i].szData );
				}
			}
			else
			{
				tSubCurrent->nTypeCnt = 0;
				tSubCurrent->cType = NULL;
			}
			
			// Sub Element Set SubInfo Init...
			tSubCurrent->nSubCnt = 0;
			tSubCurrent->tNextSub = NULL;
			tSubCurrent->prgData = NULL;

		}
		else if ( nState > 0 )
		{
			// Set Element Data...
			tSubCurrent->prgData = (TCHAR*) malloc( sizeof(TCHAR) * (_tcslen( tElement.prgData ) + 1) );
			_tcsncpy( tSubCurrent->prgData, tElement.prgData, _tcslen( tElement.prgData ) );
			tSubCurrent->prgData[_tcslen( tElement.prgData )] = _T('\0');

			iSubDep--;
			tSubCurrent = tSubCurrent->tFrontSub;
		}
	} // End Wile

	return 0;
}

/*********************************************/
// MODS STRU INIT...
/*********************************************/
INT CModsConvert::InitModsStru(MODS_STRU *fo_ModsStru, INT nTypeCnt)
{
	fo_ModsStru->nSubCnt = 0;
	fo_ModsStru->tModsSub = NULL;

	return 0;
}

/*********************************************/
// MODS SUB ELEMENT STRU INIT...
/*********************************************/
INT CModsConvert::InitSubModsStru(MODS_SUB_STRU *fo_ModsSubStru, INT nTypeCnt)
{
	INT i;

	fo_ModsSubStru->nSubCnt = 0;
	fo_ModsSubStru->nTypeCnt = 0;
	fo_ModsSubStru->tFrontSub = NULL;
	fo_ModsSubStru->tNextSub = NULL;
	memset( fo_ModsSubStru->szName, 0x00, sizeof(fo_ModsSubStru->szName) );

	if ( nTypeCnt == -1 )
	{
		fo_ModsSubStru->cType = NULL;
		fo_ModsSubStru->prgData = NULL;
	}
	else
	{
		if ( fo_ModsSubStru->cType != NULL )
			free( fo_ModsSubStru->cType );
		if ( fo_ModsSubStru->prgData != NULL )
			free( fo_ModsSubStru->prgData );

		fo_ModsSubStru->cType = (ATTRIBUTE_TYPE*)malloc( sizeof(ATTRIBUTE_TYPE) * nTypeCnt );
		fo_ModsSubStru->prgData = NULL;

		for ( i = 0; i < nTypeCnt; i++ )
		{
			memset( fo_ModsSubStru->cType[i].szName, 0x00, sizeof(fo_ModsSubStru->cType[i].szName) );
			memset( fo_ModsSubStru->cType[i].szData, 0x00, sizeof(fo_ModsSubStru->cType[i].szName) );
		}
	}

	return 0;
}

/*********************************************/
// GET ELEMENT DATA...
/*********************************************/
INT	CModsConvert::GetElementData(TCHAR *fi_ModsData, MODS_SUB_STRU *fo_sElementTemp, INT *fo_State, INT *iSubDep, BOOL bPass)
{
	INT		i, j;

	INT		iData;
	INT		nDQCnt;
	INT		idxLeft;
	INT		idxRigth;
	INT		nElementCnt;
	INT		iSElementData;
	INT		iEElementData;

	BOOL	bExist;

	TCHAR	szElementTemp[1024];
	TCHAR	*prgElementData;

	// 1.Data Init...
	bExist = FALSE;
	idxLeft = idxRigth = -1;
	iData = nDQCnt = nElementCnt = iSElementData = iEElementData = 0;
		
	prgElementData = NULL;
	memset( szElementTemp, 0x00, sizeof(szElementTemp) );

	// 2.Check End of Data 
	if ( m_ModsIdx >= m_ModsLen )
		return 10;

// Pass Mode
if ( bPass == FALSE )
{
	// Check Sub Dep...
	if ( fi_ModsData[m_ModsIdx] == _T('<') )
		if ( fi_ModsData[m_ModsIdx + 1] != _T('/') )
			(*iSubDep)++;
}
	// 3.Search ('<')
	iSElementData = m_ModsIdx;
	for ( m_ModsIdx; m_ModsIdx < m_ModsLen; m_ModsIdx++ )
	{
		if ( fi_ModsData[m_ModsIdx] == _T('<') )
		{
			idxLeft = m_ModsIdx;
			break;
		}
	}
	iEElementData = m_ModsIdx - 1;
	m_ModsIdx++;

	if ( idxLeft == -1 )
		return -10;

	// 4.Search ('>')
	for ( m_ModsIdx; m_ModsIdx < m_ModsLen; m_ModsIdx++ )
	{
		if ( fi_ModsData[m_ModsIdx] == _T('>') )
		{
			idxRigth = m_ModsIdx;
			break;
		}
		// Attribute Count
		if ( fi_ModsData[m_ModsIdx] == _T('\"') )
			nDQCnt++;
	}
	m_ModsIdx++;

	if ( idxRigth == -1 || idxRigth <= idxLeft )
		return -10;

	// 5.InitSubModsStru
	if ( nDQCnt == 0 )
		InitSubModsStru( fo_sElementTemp, 0 );
	else
		InitSubModsStru( fo_sElementTemp, (INT)nDQCnt / 2 );

	// 6.Get Elements State
	if ( fi_ModsData[idxLeft + 1] == _T('/') )
		*fo_State = idxLeft - 1;						// </Element>
	else
		*fo_State = 0;									// <Element>

	// 7.Get Element Name
	if ( *fo_State == 0 )
		_tcsncpy( szElementTemp, &fi_ModsData[idxLeft + 1], idxRigth - idxLeft - 1 );
	else
		_tcsncpy( szElementTemp, &fi_ModsData[idxLeft + 2], idxRigth - idxLeft - 2 );

	TrimLeft( szElementTemp, szElementTemp, _T(' ') );
	TrimRight( szElementTemp, szElementTemp, _T(' ') );
	TrimLeft( szElementTemp, szElementTemp, _T('?') );	// ?xml

	for ( i = 0; i < (INT)_tcslen( szElementTemp ); i++ )
	{
		if ( szElementTemp[i] == _T(' ') )
			break;
		else
			fo_sElementTemp->szName[i] = szElementTemp[i];
	}
	fo_sElementTemp->szName[i++] = _T('\0');

	// 8.Get Element Data
	if ( *fo_State > 0 )
	{
		fo_sElementTemp->prgData = (TCHAR*) malloc( sizeof(TCHAR) * (iEElementData - iSElementData + 2) );
		_tcsncpy( fo_sElementTemp->prgData, &fi_ModsData[iSElementData], iEElementData - iSElementData + 1 );
		fo_sElementTemp->prgData[iEElementData - iSElementData + 1] = _T('\0');
	}
// Pass Mode
if ( bPass == FALSE )
{
	// 9.Check Element...
	if ( *iSubDep >= 0 )
	{
		for ( i = 0; i < m_MaxElementCnt; i++ )
		{
			if ( _tcscmp( _T("NULL"), szElementName[i][*iSubDep] ) == 0 )
			{
				bExist = FALSE;
				break;
			}
			if ( _tcscmp( fo_sElementTemp->szName, szElementName[i][*iSubDep] ) == 0 )
			{
				bExist = TRUE;
				break;
			}
		}
	}
	if ( bExist == FALSE )
	{
		*fo_State = -1;
		return 0;
	}

	// 10.Get Attribute
	if ( nDQCnt <= 0 )
		return 0;
	
	fo_sElementTemp->nTypeCnt = nDQCnt / 2;
	i = 0;

	for ( j = 0; j < fo_sElementTemp->nTypeCnt; j++ )
	{
		for ( i; i < (INT)_tcslen( szElementTemp ); i++ )
		{
			if ( szElementTemp[i] == _T(' ') )
				break;
		}
		iData = i;
		
		for ( i; i < (INT)_tcslen( szElementTemp ); i++ )
		{
			if ( szElementTemp[i] == _T('=') )
				break;
		}
		_tcsncpy( fo_sElementTemp->cType[j].szName, &szElementTemp[iData], i - iData );
		TrimAll( fo_sElementTemp->cType[j].szName, fo_sElementTemp->cType[j].szName, _T(' ') );
		
		for ( i; i < (INT)_tcslen( szElementTemp ); i++ )
		{
			if ( szElementTemp[i] == _T('\"') )
				break;
		}
		i++;
		iData = i;
		
		for ( i; i < (INT)_tcslen( szElementTemp ); i++ )
		{
			if ( szElementTemp[i] == _T('\"') )
				break;
		}
		
		_tcsncpy( fo_sElementTemp->cType[j].szData, &szElementTemp[iData], i - iData );
		i++;
	}
} // End Pass Mods

	return 0;
}

/*********************************************/
// Trim ALL
/*********************************************/
VOID CModsConvert::TrimAll(TCHAR* pszSourceString, TCHAR* pszObjectString, TCHAR cTrimCharacter)
{
	INT i = 0;
	INT j = 0;
	
	for ( i = 0; pszSourceString[i] != _T('\0') && pszSourceString[i] != _T('\n'); i++ )
	{
		if ( pszSourceString[i] == cTrimCharacter )
			;
		else
		{
			pszObjectString[j] = pszSourceString[i];
			j++;
		}
	}
	pszObjectString[j] = _T('\0');
}

/*********************************************/
// Trim Left
/*********************************************/
VOID CModsConvert::TrimLeft(TCHAR* pszSourceString, TCHAR* pszObjectString, TCHAR cTrimCharacter)
{
	INT i = 0;
	INT nLen = 0;

	nLen = _tcslen( pszSourceString );

	for ( i = 0; pszSourceString[i] != _T('\0') && pszSourceString[i] != _T('\n'); i++ )
	{
		if ( pszSourceString[i] != cTrimCharacter )
			break;
	}
	_tcsncpy( pszObjectString, &pszSourceString[i], nLen - i );
	
	pszObjectString[nLen - i] = _T('\0');
}

/*********************************************/
// Trim Right
/*********************************************/
VOID CModsConvert::TrimRight(TCHAR* pszSourceString, TCHAR* pszObjectString, TCHAR cTrimCharacter)
{
	INT i = 0;
	INT nLen = 0;

	nLen = _tcslen( pszSourceString );

	for ( i = nLen - 1; i >= 0; i-- )
	{
		if ( pszSourceString[i] == cTrimCharacter )
			;
		else
			break;
	}

	_tcsncpy( pszObjectString, pszSourceString, i + 1 );
	
	pszObjectString[i + 1] = _T('\0');
}

/***********************************************************************************************************************/
// END : MODS PASSING FUNCTION...
/***********************************************************************************************************************/

/***********************************************************************************************************************/
// CREATE MARC FUNCTION...
/***********************************************************************************************************************/
/*********************************************/
// Get MARC Data to MODS Stru...
/*********************************************/
INT	CModsConvert::GetMarcData(MODS_STRU *fi_tMods, TCHAR *fo_MarcData)
{
	INT			ids;
	tMARC_STRU	tMarc;

	// 0.Data Init...
	memset( fo_MarcData, 0x00, sizeof(fo_MarcData) );

	// 1.Marc Stru Init...
	ids = ReadMarcStruFromStreamMarc( _T(""), &tMarc );
	if ( ids )
		return 0;

	// 2.007/008...
	ids = GetTAG_007( fi_tMods, &tMarc );
	ids = GetTAG_008( fi_tMods, &tMarc );
	
	// 3.Get TAG Data...
	ids = GetTAG_020( fi_tMods, &tMarc );
	ids = GetTAG_022( fi_tMods, &tMarc );
	ids = GetTAG_056( fi_tMods, &tMarc );
	ids = GetTAG_080( fi_tMods, &tMarc );
	ids = GetTAG_082( fi_tMods, &tMarc );
	ids = GetTAG_245( fi_tMods, &tMarc );
	ids = GetTAG_260( fi_tMods, &tMarc );
	ids = GetTAG_300( fi_tMods, &tMarc );
	ids = GetTAG_440( fi_tMods, &tMarc );
	ids = GetTAG_500( fi_tMods, &tMarc );
	ids = GetTAG_505( fi_tMods, &tMarc );

	// 4.Get Marc Stream...
	ids = WriteMarcStruToStreamMarc( tMarc, fo_MarcData );
	if ( ids )
		return ids;

	// 5.Get Reader Data...
	ids = GetReader06( fi_tMods, fo_MarcData );
	if ( ids ) 
		return ids;
	ids = GetReader07( fi_tMods, fo_MarcData );
	if ( ids ) 
		return ids;
	
	return 0;
}

/*********************************************/
// SET ELEMENT SEARCH INFO...
/*********************************************/
INT	CModsConvert::SetSearchInfo(MODS_STRU *fo_ModsStru, INT nSubDep, TCHAR *fi_ElementName, TCHAR *TypeName, TCHAR *TypeData)
{
	if ( nSubDep < 0 )
		return -10;

	if ( fo_ModsStru->nSubCnt > nSubDep )
		return -10;

	if ( fo_ModsStru->nSubCnt < nSubDep || fo_ModsStru->nSubCnt < nSubDep == 0 )
	{
		fo_ModsStru->nSubCnt = nSubDep;
		fo_ModsStru->tModsSub = (MODS_SUB_STRU*) realloc( fo_ModsStru->tModsSub, sizeof(MODS_SUB_STRU) * (nSubDep + 1) );
		
		fo_ModsStru->tModsSub[nSubDep].prgData = NULL;
		fo_ModsStru->tModsSub[nSubDep].tFrontSub = NULL;
		fo_ModsStru->tModsSub[nSubDep].tNextSub = NULL;
		fo_ModsStru->tModsSub[nSubDep].nSubCnt = 0;
		fo_ModsStru->tModsSub[nSubDep].nTypeCnt = 0;
		fo_ModsStru->tModsSub[nSubDep].cType = NULL;

		_tcscpy( fo_ModsStru->tModsSub[nSubDep].szName, fi_ElementName );
	}

	if ( TypeName != NULL )
	{
		fo_ModsStru->tModsSub[nSubDep].nTypeCnt++;
		fo_ModsStru->tModsSub[nSubDep].cType = (ATTRIBUTE_TYPE*) malloc( sizeof(ATTRIBUTE_TYPE) * fo_ModsStru->tModsSub[nSubDep].nTypeCnt );

		memset( fo_ModsStru->tModsSub[nSubDep].cType[fo_ModsStru->tModsSub[nSubDep].nTypeCnt - 1].szName, 0x00, sizeof(fo_ModsStru->tModsSub[nSubDep].cType[fo_ModsStru->tModsSub[nSubDep].nTypeCnt - 1].szName) );
		memset( fo_ModsStru->tModsSub[nSubDep].cType[fo_ModsStru->tModsSub[nSubDep].nTypeCnt - 1].szData, 0x00, sizeof(fo_ModsStru->tModsSub[nSubDep].cType[fo_ModsStru->tModsSub[nSubDep].nTypeCnt - 1].szData) );
	
		_tcscpy( fo_ModsStru->tModsSub[nSubDep].cType[fo_ModsStru->tModsSub[nSubDep].nTypeCnt - 1].szName, TypeName );
		_tcscpy( fo_ModsStru->tModsSub[nSubDep].cType[fo_ModsStru->tModsSub[nSubDep].nTypeCnt - 1].szData, TypeData );
	}

	return 0;
}

/*********************************************/
// Search Element Stru to MODS Stru...
/*********************************************/
INT	CModsConvert::GetElementStru(MODS_STRU *fi_tMods, TCHAR *fi_ElementName, INT iSubDep, MODS_STRU *fo_ModsTemp)
{
	INT				i;
	INT				ids;
	INT				iCurrentDep;
	MODS_SUB_STRU	*tCurrent;

	iCurrentDep = -1;
	tCurrent = NULL;

	for ( i = 0; i < fi_tMods->nSubCnt; i++ )
	{
		tCurrent = &fi_tMods->tModsSub[i];

		if ( iSubDep >= 0 )
		{
			ids = GetSubElementStru( tCurrent, &iCurrentDep, fi_ElementName, iSubDep, fo_ModsTemp);
			if ( ids )
				return ids;
		}
		else if ( iSubDep < 0 )
			return -10;

	}

	return 0;
}

INT	CModsConvert::GetElementStru(MODS_STRU *fi_tMods, MODS_STRU *fi_SearchInfoMods, INT iSubDep, MODS_STRU *fo_SearchDataMods)
{
	INT				i;
	INT				ids;
	INT				iCurrentDep;
	MODS_SUB_STRU	*tCurrent;

	iCurrentDep = -1;
	tCurrent = NULL;

	for ( i = 0; i < fi_tMods->nSubCnt; i++ )
	{
		tCurrent = &fi_tMods->tModsSub[i];

		if ( iSubDep >= 0 )
		{
			ids = GetSubElementStru( tCurrent, &iCurrentDep, fi_SearchInfoMods, iSubDep, fo_SearchDataMods);
			if ( ids )
				return ids;
		}
		else if ( iSubDep < 0 )
			return -10;

	}

	return 0;
}

/*********************************************/
// Search Sub Element Stru to MODS Stru...
/*********************************************/
INT	CModsConvert::GetSubElementStru(MODS_SUB_STRU *fi_Current, INT *fi_CurrentDep, TCHAR *fi_ElementName, INT iSubDep, MODS_STRU *fo_ModsTemp)
{
	INT	i;
	INT	ids;

	(*fi_CurrentDep)++;

	if ( (*fi_CurrentDep) > iSubDep )
	{
		(*fi_CurrentDep)--;
		return 0;
	}

	if ( _tcscmp( fi_Current->szName, fi_ElementName ) == 0 )
	{
		ids = CopyElementData(fi_Current, fo_ModsTemp);
		if ( ids )
			return ids;
	}

	if ( fi_Current->nSubCnt <= 0 )
	{
		(*fi_CurrentDep)--;
		return 0;
	}
	else
	{
		for ( i = 0; i < fi_Current->nSubCnt; i++ )
		{
			ids = GetSubElementStru( &fi_Current->tNextSub[i], fi_CurrentDep, fi_ElementName, iSubDep, fo_ModsTemp);
			if ( ids )
				return ids;
		}
	}

	(*fi_CurrentDep) = (*fi_CurrentDep) - 1;
	return 0;
}

INT	CModsConvert::GetSubElementStru(MODS_SUB_STRU *fi_Current, INT *fi_CurrentDep, MODS_STRU *fi_SearchInfoMods, INT iSubDep, MODS_STRU *fo_SearchDataMods)
{
	INT	i;
	INT	ids;
	INT	nSameTypeCnt;

	nSameTypeCnt = 0;

	(*fi_CurrentDep)++;

	if ( (*fi_CurrentDep) > iSubDep )
	{
		(*fi_CurrentDep)--;
		return 0;
	}

	// Compare Element Name
	if ( _tcscmp( fi_Current->szName, fi_SearchInfoMods->tModsSub[*fi_CurrentDep].szName ) != 0 )
	{
		(*fi_CurrentDep)--;
		return 0;
	}

	// Compare Element Type Cnt
	if ( fi_Current->nTypeCnt != fi_SearchInfoMods->tModsSub[*fi_CurrentDep].nTypeCnt )
	{
		(*fi_CurrentDep)--;
		return 0;
	}

	// Compare Element Type Name & Data
	if ( fi_Current->nTypeCnt > 0 )
	{
		for ( i = 0; i < fi_Current->nTypeCnt; i++ )
		{
			if ( _tcscmp( fi_Current->cType[i].szName, fi_SearchInfoMods->tModsSub[*fi_CurrentDep].cType[i].szName ) == 0 &&
				 _tcscmp( fi_Current->cType[i].szData, fi_SearchInfoMods->tModsSub[*fi_CurrentDep].cType[i].szData ) == 0 )
			{
				nSameTypeCnt++;
			}
		}

		if ( fi_Current->nTypeCnt != nSameTypeCnt )
		{
			(*fi_CurrentDep)--;
			return 0;
		}
	}
	
	if ( (*fi_CurrentDep) == iSubDep )
	{
		ids = CopyElementData(fi_Current, fo_SearchDataMods);
		if ( ids )
			return ids;
	}

	if ( fi_Current->nSubCnt <= 0 )
	{
		(*fi_CurrentDep)--;
		return 0;
	}
	else
	{
		for ( i = 0; i < fi_Current->nSubCnt; i++ )
		{
			ids = GetSubElementStru( &fi_Current->tNextSub[i], fi_CurrentDep, fi_SearchInfoMods, iSubDep, fo_SearchDataMods);
			if ( ids )
				return ids;
		}
	}

	(*fi_CurrentDep) = (*fi_CurrentDep) - 1;
	return 0;
}
/*********************************************/
// Copy Element Stru to MODS Stru...
/*********************************************/
INT	CModsConvert::CopyElementData(MODS_SUB_STRU *fi_Element, MODS_STRU *fo_ModsTemp)
{
	INT	i;

	// 1. Data Inti...
	fo_ModsTemp->nSubCnt++;
	fo_ModsTemp->tModsSub = (MODS_SUB_STRU*) realloc( fo_ModsTemp->tModsSub, sizeof(MODS_SUB_STRU) * fo_ModsTemp->nSubCnt );

	// 2.Data Copy... nTypeCnt,cType,prgData
	fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].nSubCnt = 0;
	fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].tFrontSub = NULL;
	fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].tNextSub = NULL;
	// Name
	_tcscpy( fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].szName, fi_Element->szName );
	// Data
	fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].prgData = (TCHAR*) malloc( sizeof(TCHAR) * (_tcslen( fi_Element->prgData ) + 1) );
	_tcscpy( fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].prgData, fi_Element->prgData );
	fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].prgData[_tcslen( fi_Element->prgData )] = _T('\0');
	// Type
	fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].cType = NULL;
	fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].nTypeCnt = fi_Element->nTypeCnt;

	if ( fi_Element->nTypeCnt > 0 )
		fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].cType = (ATTRIBUTE_TYPE*) malloc( sizeof(ATTRIBUTE_TYPE) * fi_Element->nTypeCnt );
	for ( i = 0; i < fi_Element->nTypeCnt; i++ )
	{
		_tcscpy( fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].cType[i].szName, fi_Element->cType[i].szName );
		_tcscpy( fo_ModsTemp->tModsSub[fo_ModsTemp->nSubCnt - 1].cType[i].szData, fi_Element->cType[i].szData );
	}

	return 0;
}

/*************************************************************************************************/
// 007 RULE
// SEL(0) : <genre authority="marc">					007/00, 007/01	DEP(0)	00 = a(LEN:8),d(LEN:6)
// SEL(1) : <physicalDescription><digitalOrigin>		007/00-10		DEP(1)	00 = c(LEN:14)
// SEL(2) : <physicalDescription><reformattingQuality>	007/13			DEP(1)
/*************************************************************************************************/
INT CModsConvert::GetTAG_007(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			ids;
	INT			n007Len;
	TCHAR		c007_00 = _T(' ');
	TCHAR		c007_01 = _T(' ');
	TCHAR		c007_11 = _T(' ');
	TCHAR		c007_13 = _T(' ');
	TCHAR		szFieldData[32];

	MODS_STRU	tInMods;
	MODS_STRU	tOutMods;

	// ===============================================
	// 1.Data Init...
	// ===============================================
	n007Len = 0;
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	InitModsStru( &tInMods, -1 );
	InitModsStru( &tOutMods, -1 );

	// ===============================================
	// 2.SEL(0)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("genre"), _T("authority"), _T("marc") );
	if ( ids )
		return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 0, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("atlas") ) == 0 )
			{
				n007Len = 8; c007_00 = _T('a');	c007_01 = _T('d');
			}
			else if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("globe") ) == 0 )
			{
				n007Len = 6; c007_00 = _T('d');
			}
			else if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("map") ) == 0 )
			{
				n007Len = 8; c007_00 = _T('a');	c007_01 = _T('j');
			}
			else if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("model") ) == 0 )
			{
				n007Len = 8; c007_00 = _T('a');	c007_01 = _T('q');
			}
			else if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("remote sensing image") ) == 0 )
			{
				n007Len = 8; c007_00 = _T('a');	c007_01 = _T('r');
			}
		}
	}
	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	// ===============================================
	// 3.SEL(1)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("physicalDescription"), NULL, NULL );
	if ( ids ) return ids;
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 1, _T("digitalOrigin"), NULL, NULL );
	if ( ids ) return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 1, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("reformatted digital") ) == 0 )
			{
				n007Len = 14; c007_00 = _T('c'); c007_11 = _T('a');
			}
		}
	}
	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	// ===============================================
	// 4.SEL(2)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("physicalDescription"), NULL, NULL );
	if ( ids ) return ids;
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 1, _T("reformattingQuality"), NULL, NULL );
	if ( ids ) return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 1, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("access") ) == 0 )
				c007_13 = _T('a');
			else if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("preservation") ) == 0 )
				c007_13 = _T('p');
			else if ( _tcscmp( tOutMods.tModsSub[i].prgData, _T("replacement") ) == 0 )
				c007_13 = _T('r');
		}
	}
	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	// ===============================================
	// 5.Set 007 Data
	// ===============================================
	if ( n007Len != 0 )
		for ( i = 0; i < n007Len; i++ )
			szFieldData[i] = _T(' ');
	
	if ( n007Len == 6 )
		szFieldData[0] = c007_00;
	else if ( n007Len == 8 )
	{
		szFieldData[0] = c007_00;
		szFieldData[1] = c007_01;
	}
	else if ( n007Len == 14 )
	{
		szFieldData[0] = c007_00;
		for ( i = 1; i < 10; i++ )
			szFieldData[i] = c007_00;
		szFieldData[11] = c007_11;
		if ( c007_13 != _T(' ') )
			szFieldData[13] = c007_13;
	}

	return 0;
}

/*************************************************************************************************/
// 008 RULE
// SEL(0) : <recordCreationDate encoding="marc">					: 008/00-05		DEP(0)
// SEL(1) : <originInfo><dateIssued encoding="marc">				: 008/06,07-10	DEP(1)
// SEL(2) : <originInfo><dateIssued point="start" encoding="marc">	: 008/06,07-10	DEP(1)
// SEL(2) : <originInfo><dateIssued point="end" encoding="marc">	: 008/06,11-14	DEP(1)
// SEL(3) : <originInfo><place><placeTerm type="code" authority="marccountry">	: 008/15-17	DEP(2)
// SEL(3) : <originInfo><copyrightDate>								: 008/06		DEP(1)
// SEL(4) : <targetAudience authority="marctarget">					: 008/22		DEP(0)
// SEL(5) : <physicalDescription><form authority="marcform">		: 008/23,25,29	DEP(1)
// SEL(6) : <genre authority="marc">								: 008/21~33		DEP(0)
// SEL(7) : <language><languageTerm type="code" authority="iso639-2b">	: 008/35-37	DEP(1)
/*************************************************************************************************/
INT CModsConvert::GetTAG_008(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			j;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldData[64];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldData, 0x00, sizeof(szFieldData) );
	for ( i = 0; i < 40; i++ )
		szFieldData[i] = _T(' ');

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("recordCreationDate"), 0, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(0)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("encoding") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("marc") ) == 0 )
				{
					_tcsncpy( szFieldData, tModsTemp.tModsSub[i].prgData, 6 );
				}
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("dateIssued"), 1, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(1)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("encoding") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("marc") ) == 0 )
				{
					if ( szFieldData[6] != _T('m') )
						szFieldData[6] = _T('s');
					_tcsncpy( &szFieldData[7], tModsTemp.tModsSub[i].prgData, 4 );
				}
			}
			// SEL(2)
			else if ( tModsTemp.tModsSub[i].nTypeCnt == 2 )
			{
				for ( j = 0; j < tModsTemp.tModsSub[i].nTypeCnt; j++ )
				{
					if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("point") ) == 0 &&
						_tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("start") ) == 0 )
					{
						nSameCnt++;
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("encoding") ) == 0 &&
						_tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("marc") ) == 0 )
					{
						nSameCnt++;
					}
				}

				if ( tModsTemp.tModsSub[i].nTypeCnt == nSameCnt )
				{
					szFieldData[6] = _T('m');
					_tcsncpy( &szFieldData[7], tModsTemp.tModsSub[i].prgData, 4 );
				}

				nSameCnt = 0;
			}
			// SEL(2)
			else if ( tModsTemp.tModsSub[i].nTypeCnt == 2 )
			{
				for ( j = 0; j < tModsTemp.tModsSub[i].nTypeCnt; j++ )
				{
					if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("point") ) == 0 &&
						_tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("end") ) == 0 )
					{
						nSameCnt++;
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("encoding") ) == 0 &&
						_tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("marc") ) == 0 )
					{
						nSameCnt++;
					}
				}

				if ( tModsTemp.tModsSub[i].nTypeCnt == nSameCnt )
				{
					szFieldData[6] = _T('m');
					_tcsncpy( &szFieldData[11], tModsTemp.tModsSub[i].prgData, 4 );
				}

				nSameCnt = 0;
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("placeTerm"), 0, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(3)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 2 )
			{
				for ( j = 0; j < tModsTemp.tModsSub[i].nTypeCnt; j++ )
				{
					if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("type") ) == 0 &&
						_tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("code") ) == 0 )
					{
						nSameCnt++;
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("authority") ) == 0 &&
						_tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("marccountry") ) == 0 )
					{
						nSameCnt++;
					}
				}

				if ( tModsTemp.tModsSub[i].nTypeCnt == nSameCnt )
				{
					szFieldData[6] = _T('m');
					_tcsncpy( &szFieldData[11], tModsTemp.tModsSub[i].prgData, 4 );
				}

				nSameCnt = 0;
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("copyrightDate"), 1, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(0)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 0 )
			{
				szFieldData[6] = _T('s');
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("targetAudience"), 0, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(4)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("authority") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("marctarget") ) == 0 )
				{
					if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("adolescent") ) == 0 )
						szFieldData[22] = _T('d');
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("adult") ) == 0 )
						szFieldData[22] = _T('e');
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("general") ) == 0 )
						szFieldData[22] = _T('b');
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("juvenile") ) == 0 )
						szFieldData[22] = _T('j');
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("preschool") ) == 0 )
						szFieldData[22] = _T('a');
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("specialized") ) == 0 )
						szFieldData[22] = _T('f');
				}
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("form"), 1, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(4)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("authority") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("marcform") ) == 0 )
				{
					if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("braille") ) == 0 )
					{
						szFieldData[23] = _T('f'); szFieldData[29] = _T('f');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("electronic") ) == 0 )
					{
						szFieldData[23] = _T('s'); szFieldData[29] = _T('s'); szFieldData[25] = _T('m');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("microfiche") ) == 0 )
					{
						szFieldData[23] = _T('b'); szFieldData[29] = _T('b');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("microfilm") ) == 0 )
					{
						szFieldData[23] = _T('a'); szFieldData[29] = _T('a');
					}
				}
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("genre"), 0, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(4)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("authority") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("marcgt") ) == 0 )
				{
					if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("database") ) == 0 )
					{
						szFieldData[21] = _T('d'); szFieldData[25] = _T('e');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("loose-leaf") ) == 0 )
					{
						szFieldData[21] = _T('l');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("newspaper") ) == 0 )
					{
						szFieldData[21] = _T('n');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("periodical") ) == 0 )
					{
						szFieldData[21] = _T('p');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("series") ) == 0 )
					{
						szFieldData[21] = _T('m');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("web site") ) == 0 )
					{
						szFieldData[21] = _T('w');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("abstract or summary") ) == 0 )
					{
						szFieldData[24] = _T('a'); szFieldData[25] = _T('a'); szFieldData[26] = _T('a'); szFieldData[27] = _T('a');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("bibliography") ) == 0 )
					{
						szFieldData[24] = _T('b'); szFieldData[25] = _T('b');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("catalog") ) == 0 )
					{
						szFieldData[24] = _T('c'); szFieldData[25] = _T('c');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("dictionary") ) == 0 )
					{
						szFieldData[24] = _T('d'); szFieldData[25] = _T('d');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("directory") ) == 0 )
					{
						szFieldData[24] = _T('r'); szFieldData[25] = _T('r');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("discography") ) == 0 )
					{
						szFieldData[24] = _T('k'); szFieldData[25] = _T('k');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("encyclopedia") ) == 0 )
					{
						szFieldData[24] = _T('e'); szFieldData[25] = _T('e');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("filmography") ) == 0 )
					{
						szFieldData[24] = _T('q'); szFieldData[25] = _T('q');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("handbook") ) == 0 )
					{
						szFieldData[24] = _T('f'); szFieldData[25] = _T('f');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("index") ) == 0 )
					{
						szFieldData[24] = _T('i'); szFieldData[25] = _T('i');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("law report or digest") ) == 0 )
					{
						szFieldData[24] = _T('w'); szFieldData[25] = _T('w');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("legal article") ) == 0 )
					{
						szFieldData[24] = _T('g'); szFieldData[25] = _T('g');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("legal case and case notes") ) == 0 )
					{
						szFieldData[24] = _T('v'); szFieldData[25] = _T('v');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("legislation") ) == 0 )
					{
						szFieldData[24] = _T('l'); szFieldData[25] = _T('l');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("patent") ) == 0 )
					{
						szFieldData[24] = _T('j'); szFieldData[25] = _T('j');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("programmed text") ) == 0 )
					{
						szFieldData[24] = _T('p'); szFieldData[25] = _T('p');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("review") ) == 0 )
					{
						szFieldData[24] = _T('o'); szFieldData[25] = _T('o');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("statistics") ) == 0 )
					{
						szFieldData[24] = _T('s'); szFieldData[25] = _T('s');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("survey of literature") ) == 0 )
					{
						szFieldData[24] = _T('n'); szFieldData[25] = _T('n');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("technical report") ) == 0 )
					{
						szFieldData[24] = _T('t'); szFieldData[25] = _T('t');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("theses") ) == 0 )
					{
						szFieldData[24] = _T('m'); szFieldData[25] = _T('m');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("treaty") ) == 0 )
					{
						szFieldData[24] = _T('z'); szFieldData[25] = _T('z');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("atlas") ) == 0 )
					{
						szFieldData[25] = _T('e');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("font") ) == 0 )
					{
						szFieldData[25] = _T('f');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("game") ) == 0 )
					{
						szFieldData[25] = _T('g');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("globe") ) == 0 )
					{
						szFieldData[25] = _T('d');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("numeric data") ) == 0 )
					{
						szFieldData[25] = _T('a');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("sound") ) == 0 )
					{
						szFieldData[25] = _T('h'); szFieldData[30] = _T('s'); szFieldData[31] = _T('s');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("conference publication") ) == 0 )
					{
						szFieldData[29] = _T('1'); szFieldData[30] = _T('c'); szFieldData[31] = _T('c');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("autobiography") ) == 0 )
					{
						szFieldData[30] = _T('a'); szFieldData[31] = _T('a'); szFieldData[34] = _T('a');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("biography") ) == 0 )
					{
						szFieldData[30] = _T('b'); szFieldData[31] = _T('b'); szFieldData[34] = _T('d');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("drama") ) == 0 )
					{
						szFieldData[30] = _T('d'); szFieldData[31] = _T('d'); szFieldData[33] = _T('d');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("essay") ) == 0 )
					{
						szFieldData[30] = _T('e'); szFieldData[31] = _T('e'); szFieldData[33] = _T('e');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("festschrift") ) == 0 )
					{
						szFieldData[30] = _T('1');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("fiction") ) == 0 )
					{
						szFieldData[30] = _T('f'); szFieldData[31] = _T('f'); szFieldData[33] = _T('l');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("folktale") ) == 0 )
					{
						szFieldData[30] = _T('o'); szFieldData[31] = _T('o');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("history") ) == 0 )
					{
						szFieldData[30] = _T('h'); szFieldData[31] = _T('h');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("humor, satire") ) == 0 )
					{
						szFieldData[30] = _T('k'); szFieldData[31] = _T('k'); szFieldData[33] = _T('h');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("instruction") ) == 0 )
					{
						szFieldData[30] = _T('i'); szFieldData[31] = _T('i');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("interview") ) == 0 )
					{
						szFieldData[30] = _T('t'); szFieldData[31] = _T('t');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("language instruction") ) == 0 )
					{
						szFieldData[30] = _T('j'); szFieldData[31] = _T('j');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("memoir") ) == 0 )
					{
						szFieldData[30] = _T('m'); szFieldData[31] = _T('m');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("poetry") ) == 0 )
					{
						szFieldData[30] = _T('p'); szFieldData[31] = _T('p'); szFieldData[33] = _T('p');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("rehearsal") ) == 0 )
					{
						szFieldData[30] = _T('r'); szFieldData[31] = _T('r');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("reporting") ) == 0 )
					{
						szFieldData[30] = _T('g'); szFieldData[31] = _T('g');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("speech") ) == 0 )
					{
						szFieldData[30] = _T('l'); szFieldData[31] = _T('l'); szFieldData[33] = _T('s');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("art original") ) == 0 )
					{
						szFieldData[33] = _T('a');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("art reproduction") ) == 0 )
					{
						szFieldData[33] = _T('c');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("chart") ) == 0 )
					{
						szFieldData[33] = _T('n');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("comic strip") ) == 0 )
					{
						szFieldData[33] = _T('c');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("diorama") ) == 0 )
					{
						szFieldData[33] = _T('d');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("filmstrip") ) == 0 )
					{
						szFieldData[33] = _T('f');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("flash card") ) == 0 )
					{
						szFieldData[33] = _T('o');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("graphic") ) == 0 )
					{
						szFieldData[33] = _T('k');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("kit") ) == 0 )
					{
						szFieldData[33] = _T('b');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("letter") ) == 0 )
					{
						szFieldData[33] = _T('i');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("microscope slide") ) == 0 )
					{
						szFieldData[33] = _T('p');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("model") ) == 0 )
					{
						szFieldData[33] = _T('q');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("motion picture") ) == 0 )
					{
						szFieldData[33] = _T('m');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("novel") ) == 0 )
					{
						szFieldData[33] = _T('f');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("picture") ) == 0 )
					{
						szFieldData[33] = _T('i');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("realia") ) == 0 )
					{
						szFieldData[33] = _T('r');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("short story") ) == 0 )
					{
						szFieldData[33] = _T('j');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("slide") ) == 0 )
					{
						szFieldData[33] = _T('s');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("technical drawing") ) == 0 )
					{
						szFieldData[33] = _T('l');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("toy") ) == 0 )
					{
						szFieldData[33] = _T('w');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("transparency") ) == 0 )
					{
						szFieldData[33] = _T('t');
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("videorecording") ) == 0 )
					{
						szFieldData[33] = _T('v');
					}
				}
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("languageTerm"), 1, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(7)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 2 )
			{
				for ( j = 0; j < tModsTemp.tModsSub[i].nTypeCnt; j++ )
				{
					if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("type") ) == 0 &&
						_tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("code") ) == 0 )
					{
						nSameCnt++;
					}
					else if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("authority") ) == 0 &&
						_tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("iso639-2b") ) == 0 )
					{
						nSameCnt++;
					}
				}

				if ( tModsTemp.tModsSub[i].nTypeCnt == nSameCnt )
				{
					_tcsncpy( &szFieldData[35], tModsTemp.tModsSub[i].prgData, 3 );
				}

				nSameCnt = 0;
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}

// ISBN은 무조건 낱권으로 표기 (제1지시기호:b)
/*************************************************************************************************/
// 020 RULE (ISBN)
// SEL(0) : <identifier type="isbn">				: 020 $a ind1=blank ind2=blank	DEP(0)
// SEL(1) : <identifier type="isbn" invalid="yes">	: 020 $z ind1=blank ind2=blank	DEP(0)
/*************************************************************************************************/
INT CModsConvert::GetTAG_020(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			j;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("identifier"), 0, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt <= 0 )
		return 0;

	// 4.Get TAG Data
	for ( i = 0; i < tModsTemp.nSubCnt; i++ )
	{
		// SEL(0)
		if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
		{
			if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("type") ) == 0 &&
				 _tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("isbn") ) == 0 )
			{
				_stprintf( szFieldCode, _T("%ca"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 20, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
		// SEL(1)
		else if ( tModsTemp.tModsSub[i].nTypeCnt == 2 )
		{
			for ( j = 0; j < tModsTemp.tModsSub[i].nTypeCnt; j++ )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("type") ) == 0 &&
				 _tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("isbn") ) == 0 )
				{
					nSameCnt++;
				}
				else if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("invalid") ) == 0 &&
						  _tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("yes") ) == 0 )
				{
					nSameCnt++;
				}
			}

			if ( tModsTemp.tModsSub[i].nTypeCnt == nSameCnt )
			{
				_stprintf( szFieldCode, _T("%cz"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 20, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}

/*************************************************************************************************/
// 022 RULE (ISSN)
// SEL(0) : <identifier type="issn">				: 020 $a ind1=blank ind2=blank	DEP(0)
// SEL(1) : <identifier type="issn" invalid="yes">	: 020 $z ind1=blank ind2=blank	DEP(0)
/*************************************************************************************************/
INT CModsConvert::GetTAG_022(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			j;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("identifier"), 0, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt <= 0 )
		return 0;

	// 4.Get TAG Data
	for ( i = 0; i < tModsTemp.nSubCnt; i++ )
	{
		// SEL(0)
		if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
		{
			if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("type") ) == 0 &&
				 _tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("issn") ) == 0 )
			{
				_stprintf( szFieldCode, _T("%ca"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 22, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
		// SEL(1)
		else if ( tModsTemp.tModsSub[i].nTypeCnt == 2 )
		{
			for ( j = 0; j < tModsTemp.tModsSub[i].nTypeCnt; j++ )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("type") ) == 0 &&
				 _tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("issn") ) == 0 )
				{
					nSameCnt++;
				}
				else if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("invalid") ) == 0 &&
						  _tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("yes") ) == 0 )
				{
					nSameCnt++;
				}
			}

			if ( tModsTemp.tModsSub[i].nTypeCnt == nSameCnt )
			{
				_stprintf( szFieldCode, _T("%cz"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 22, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}
/*************************************************************************************************/
// 056 RULE (KDC분류)
// SEL(0) : <classification authority="kdc">	: 056 $a ind1=blank ind2=blank	DEP(0)
/*************************************************************************************************/
INT CModsConvert::GetTAG_056(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("classification"), 0, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt <= 0 )
		return 0;

	// 4.Get TAG Data
	for ( i = 0; i < tModsTemp.nSubCnt; i++ )
	{
		// SEL(0)
		if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
		{
			if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("authority") ) == 0 &&
				 _tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("kdc") ) == 0 )
			{
				_stprintf( szFieldCode, _T("%ca"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 56, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}
/*************************************************************************************************/
// 080 RULE (국제십진분류)
// SEL(0) : <classification authority="udc">	: 080 $a ind1=blank ind2=blank	DEP(0)
/*************************************************************************************************/
INT CModsConvert::GetTAG_080(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("classification"), 0, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt <= 0 )
		return 0;

	// 4.Get TAG Data
	for ( i = 0; i < tModsTemp.nSubCnt; i++ )
	{
		// SEL(0)
		if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
		{
			if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("authority") ) == 0 &&
				 _tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("udc") ) == 0 )
			{
				_stprintf( szFieldCode, _T("%ca"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 80, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}
// <classification edition="13" authority="ddc">793.3</classification>
/*************************************************************************************************/
// 082 RULE (DDC분류)
// SEL(0) : <classification authority="ddc">					082 $a ind1=0 ind2=blank						DEP(0)
// SEL(1) : <classification edition="content" authority="ddc">	082 $a ind1=0 ind2=blank and Add $2 "content"	DEP(0) 
/*************************************************************************************************/
INT CModsConvert::GetTAG_082(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			j;
	INT			ids;
	INT			iTemp;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = iTemp = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("classification"), 0, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt <= 0 )
		return 0;

	// 4.Get TAG Data
	for ( i = 0; i < tModsTemp.nSubCnt; i++ )
	{
		// SEL(0)
		if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
		{
			if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("authority") ) == 0 &&
				 _tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("ddc") ) == 0 )
			{
				_stprintf( szFieldCode, _T("%ca"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 82, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('0') );
			}
		}
		// SEL(1)
		else if ( tModsTemp.tModsSub[i].nTypeCnt == 2 )
		{
			for ( j = 0; j < tModsTemp.tModsSub[i].nTypeCnt; j++ )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("authority") ) == 0 &&
				 _tcscmp( tModsTemp.tModsSub[i].cType[j].szData, _T("ddc") ) == 0 )
				{
					nSameCnt++;
				}
				else if ( _tcscmp( tModsTemp.tModsSub[i].cType[j].szName, _T("edition") ) == 0 )
				{
					nSameCnt++;
					iTemp = j;
				}
			}

			if ( tModsTemp.tModsSub[i].nTypeCnt == nSameCnt )
			{
				_stprintf( szFieldCode, _T("%ca"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 82, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('0') );

				_stprintf( szFieldCode, _T("%c2"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].cType[iTemp].szData );
				InsertSubFieldDataToMarcStru( 82, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('0') );
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}
/*************************************************************************************************/
// 245 RULE (표제)
// SEL(0) : <titleInfo><title>							245 $a ind1=1 ind2=<nonSort>	DEP(1)
// SEL(1) : <titleInfo><subTitle>						245 $b ind1=1 ind2=<nonSort>	DEP(1)
// SEL(2) : <titleInfo><partNumber>						245 $n ind1=1 ind2=<nonSort>	DEP(1)
// SEL(3) : <titleInfo><partName>						245 $p ind1=1 ind2=<nonSort>	DEP(1)
// SEL(4) : <physicalDescription><form authority="gmd">	245 $h ind1=1 ind2=<nonSort>	DEP(1)
// SEL(5) : <note type="statement of responsibility">	245 $d ind1=1 ind2=<nonSort>	DEP(0)
/*************************************************************************************************/
INT CModsConvert::GetTAG_245(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			ids;
	INT			iTemp;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];

	MODS_STRU	tInMods;
	MODS_STRU	tOutMods;

	// 1.Data Init...
	nSameCnt = iTemp = 0;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	InitModsStru( &tInMods, -1 );
	InitModsStru( &tOutMods, -1 );

	// ===============================================
	// 2.SEL(0)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("titleInfo"), NULL, NULL );
	if ( ids )
		return ids;
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 1, _T("title"), NULL, NULL );
	if ( ids )
		return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 1, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			_stprintf( szFieldCode, _T("%ca"), 31 );
			_tcscpy( szFieldData, tOutMods.tModsSub[i].prgData );
			InsertSubFieldDataToMarcStru( 245, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('1'), _T('0') );
		}
	}
	
	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	// ===============================================
	// 2.SEL(1)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("titleInfo"), NULL, NULL );
	if ( ids )
		return ids;
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 1, _T("subTitle"), NULL, NULL );
	if ( ids )
		return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 1, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			_stprintf( szFieldCode, _T("%cb"), 31 );
			_tcscpy( szFieldData, tOutMods.tModsSub[i].prgData );
			InsertSubFieldDataToMarcStru( 245, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('1'), _T('0') );
		}
	}

	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	// ===============================================
	// 2.SEL(2)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("titleInfo"), NULL, NULL );
	if ( ids )
		return ids;
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 1, _T("partNumber"), NULL, NULL );
	if ( ids )
		return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 1, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			_stprintf( szFieldCode, _T("%cn"), 31 );
			_tcscpy( szFieldData, tOutMods.tModsSub[i].prgData );
			InsertSubFieldDataToMarcStru( 245, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('1'), _T('0') );
		}
	}

	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	// ===============================================
	// 2.SEL(3)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("titleInfo"), NULL, NULL );
	if ( ids )
		return ids;
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 1, _T("partName"), NULL, NULL );
	if ( ids )
		return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 1, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			_stprintf( szFieldCode, _T("%cp"), 31 );
			_tcscpy( szFieldData, tOutMods.tModsSub[i].prgData );
			InsertSubFieldDataToMarcStru( 245, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('1'), _T('0') );
		}
	}

	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	// ===============================================
	// 2.SEL(4)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("physicalDescription"), NULL, NULL );
	if ( ids )
		return ids;
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 1, _T("form"), _T("authority"), _T("gmd") );
	if ( ids )
		return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 1, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			_stprintf( szFieldCode, _T("%ch"), 31 );
			_tcscpy( szFieldData, tOutMods.tModsSub[i].prgData );
			InsertSubFieldDataToMarcStru( 245, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('1'), _T('0') );
		}
	}

	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	// ===============================================
	// 2.SEL(5)
	// ===============================================
	// 1.Set Search Condition
	ids = SetSearchInfo( &tInMods, 0, _T("note"), _T("type"), _T("statement of responsibility") );
	if ( ids )
		return ids;
	
	// 2.Get Data...
	ids = GetElementStru( fi_tMods, &tInMods, 0, &tOutMods);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tOutMods.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tOutMods.nSubCnt; i++ )
		{
			_stprintf( szFieldCode, _T("%cd"), 31 );
			_tcscpy( szFieldData, tOutMods.tModsSub[i].prgData );
			InsertSubFieldDataToMarcStru( 245, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('1'), _T('0') );
		}
	}

	// 5.Data Init...
	FreeTempMODSStru( &tInMods );
	FreeTempMODSStru( &tOutMods );

	return 0;
}
/*************************************************************************************************/
// 260 RULE (발행사항)
// SEL(0) : <originInfo><place><placeTerm type="text">			260 $a ind1=blank ind2=blank	DEP(2)
// SEL(1) : <originInfo><publisher>								260 $b ind1=blank ind2=blank	DEP(1)
// SEL(2) : <originInfo><dateIssued>							260 $c ind1=blank ind2=blank	DEP(1)
// SEL(3) : <originInfo><dateIssued point="start">				260 $c ind1=blank ind2=blank	DEP(1)
// SEL(3) : <originInfo><dateIssued point="end">				260 $c ind1=blank ind2=blank	DEP(1) : Generate a hyphen before date
// SEL(4) : <originInfo><dateIssued qualifier="questionable">	260 $c ind1=blank ind2=blank	DEP(1) : Generate a question mark after the date
// SEL(5) : <originInfo><dateCreated>							260 $g ind1=blank ind2=blank	DEP(1)
// SEL(6) : <originInfo><copyrightDate>							260 $c ind1=blank ind2=blank	DEP(1) : Generate a "c" before the date
/*************************************************************************************************/
INT CModsConvert::GetTAG_260(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("placeTerm"), 2, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(0)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("type") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("text") ) == 0 )
				{
					_stprintf( szFieldCode, _T("%ca"), 31 );
					_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
					InsertSubFieldDataToMarcStru( 260, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
				}
			}
		}
	}
	
	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("publisher"), 1, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(1)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 0 )
			{
				_stprintf( szFieldCode, _T("%cb"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 260, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
	}
	
	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("copyrightDate"), 1, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(6)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 0 )
			{
				_stprintf( szFieldCode, _T("%cc"), 31 );
				szFieldData[0] = _T('c');
				_tcscpy( &szFieldData[1], tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 260, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("dateIssued"), 1, &tModsTemp);
	if ( ids )
		return ids;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("copyrightDate"), 1, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(2)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 0 )
			{
				_stprintf( szFieldCode, _T("%cc"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
			}
			// SEL(4)
			else if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("qualifier") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("questionable") ) == 0 )
				{
					_stprintf( szFieldCode, _T("%cc"), 31 );
					_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
					_tcscpy( &szFieldData[_tcslen(tModsTemp.tModsSub[i].prgData)], _T("?") );
					InsertSubFieldDataToMarcStru( 260, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
				}
				// SEL(3)
				else if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("point") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("start") ) == 0 )
				{
					_stprintf( szFieldCode, _T("%cc"), 31 );
					_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				}
				else if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("point") ) == 0 &&
					_tcscmp( tModsTemp.tModsSub[i].cType[0].szData, _T("end") ) == 0 )
				{
					_stprintf( szFieldCode, _T("%cc"), 31 );
					_tcscpy( &szFieldData[_tcslen(szFieldData)], _T("-") );
					_tcscpy( &szFieldData[_tcslen(szFieldData)], tModsTemp.tModsSub[i].prgData );
				}
			}

		}

		InsertSubFieldDataToMarcStru( 260, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
	}
	
	FreeTempMODSStru( &tModsTemp );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("dateCreated"), 1, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(5)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 0 )
			{
				_stprintf( szFieldCode, _T("%cg"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 260, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}
/*************************************************************************************************/
// 300 RULE (형태사항)
// SEL(0) : <physicalDescription><extent>	300 $a ind1=blank ind2=blank	DEP(1)
/*************************************************************************************************/
INT CModsConvert::GetTAG_300(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("extent"), 1, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt <= 0 )
		return 0;

	// 4.Get TAG Data
	for ( i = 0; i < tModsTemp.nSubCnt; i++ )
	{
		// SEL(0)
		_stprintf( szFieldCode, _T("%ca"), 31 );
		_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
		InsertSubFieldDataToMarcStru( 300, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' ') );
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}
// 문서에 정리 안돼 있음
/*************************************************************************************************/
// 440 RULE (총서)
// SEL(0) : <relatedItem type="series"><titleInfo><partNumber>		440 $n ind1=0 ind2=blank	DEP(2)
// SEL(1) : <relatedItem type="series"><titleInfo><partName>		440 $p ind1=0 ind2=blank	DEP(2)
/*************************************************************************************************/
INT CModsConvert::GetTAG_440(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	return 0;
}
/*************************************************************************************************/
// 500 RULE (일반주기)
// SEL(0) : <note>		500 $a ind1=blank ind2=blank	DEP(0)
/*************************************************************************************************/
INT CModsConvert::GetTAG_500(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("note"), 0, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt <= 0 )
		return 0;

	// 4.Get TAG Data
	for ( i = 0; i < tModsTemp.nSubCnt; i++ )
	{
		// SEL(0)
		if ( tModsTemp.tModsSub[i].nTypeCnt == 0 )
		{
			_stprintf( szFieldCode, _T("%ca"), 31 );
			_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
			InsertSubFieldDataToMarcStru( 500, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('1') );
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}
/*************************************************************************************************/
// 505 RULE (내용주기)
// SEL(0) : <tableOfContents>								500 $a ind1=0 ind2=blank	DEP(0)
// SEL(1) : <tableOfContents xlink:simpleLink="content">	500 $u ind1=0 ind2=blank	DEP(0) // 예측불가 자료
/*************************************************************************************************/
INT CModsConvert::GetTAG_505(MODS_STRU *fi_tMods, tMARC_STRU *fo_MarcStru)
{
	INT			i;
	INT			ids;
	INT			nSameCnt;
	TCHAR		szFieldCode[16];
	TCHAR		szFieldData[10000];
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	nSameCnt = 0;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;
	memset( szFieldCode, 0x00, sizeof(szFieldCode) );
	memset( szFieldData, 0x00, sizeof(szFieldData) );

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("tableOfContents"), 0, &tModsTemp);
	if ( ids )
		return ids;
	
	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(0)
			if ( tModsTemp.tModsSub[i].nTypeCnt == 0 )
			{
				_stprintf( szFieldCode, _T("%ca"), 31 );
				_tcscpy( szFieldData, tModsTemp.tModsSub[i].prgData );
				InsertSubFieldDataToMarcStru( 505, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('0') );
			}
			else if ( tModsTemp.tModsSub[i].nTypeCnt == 1 )
			{
				if ( _tcscmp( tModsTemp.tModsSub[i].cType[0].szName, _T("xlink:simpleLink") ) == 0 )
				{
					_stprintf( szFieldCode, _T("%cu"), 31 );
					_tcscpy( szFieldData, tModsTemp.tModsSub[i].cType[0].szData );
					InsertSubFieldDataToMarcStru( 505, szFieldCode, szFieldData, fo_MarcStru, FALSE, _T(' '), _T('0') );
				}
			}
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}

/*************************************************************************************************/
// READER RULE
// SEL(0) : <typeOfResource>	: READER/06		DEP(0)
/*************************************************************************************************/
INT	CModsConvert::GetReader06(MODS_STRU *fi_tMods, TCHAR *fo_MarcData)
{
	INT			i;
	INT			ids;
	INT			iReader;
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	iReader = 6;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("typeOfResource"), 0, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt <= 0 )
		return 0;

	// 4.Get TAG Data
	for ( i = 0; i < tModsTemp.nSubCnt; i++ )
	{
		// SEL(0)
		if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("text") ) == 0 )
			fo_MarcData[iReader] = _T('a');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("text and attribute manuscript=\"yes\"") ) == 0 )
			fo_MarcData[iReader] = _T('t');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("cartographic") ) == 0 )
			fo_MarcData[iReader] = _T('e');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("cartographic and attribute manuscript=\"yes\"") ) == 0 )
			fo_MarcData[iReader] = _T('f');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("notated music") ) == 0 )
			fo_MarcData[iReader] = _T('c');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("notated music and attribute manuscript=\"yes\"") ) == 0 )
			fo_MarcData[iReader] = _T('d');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("sound recording-nonmusical") ) == 0 )
			fo_MarcData[iReader] = _T('i');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("sound recording-musical") ) == 0 )
			fo_MarcData[iReader] = _T('j');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("still image") ) == 0 )
			fo_MarcData[iReader] = _T('k');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("moving image") ) == 0 )
			fo_MarcData[iReader] = _T('g');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("three dimensional object") ) == 0 )
			fo_MarcData[iReader] = _T('r');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("software, multimedia") ) == 0 )
			fo_MarcData[iReader] = _T('m');
		else if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("mixed material") ) == 0 )
			fo_MarcData[iReader] = _T('p');
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}

/*************************************************************************************************/
// READER RULE
// SEL(0) : <typeOfResource>		: READER/07		DEP(0)
// SEL(0) : <originInfo><issuance>	: READER/07		DEP(1)
/*************************************************************************************************/
INT	CModsConvert::GetReader07(MODS_STRU *fi_tMods, TCHAR *fo_MarcData)
{
	INT			i;
	INT			ids;
	INT			iReader;
	MODS_STRU	tModsTemp;

	// 1.Data Init...
	iReader = 7;
	tModsTemp.nSubCnt = 0;
	tModsTemp.tModsSub = NULL;

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("typeOfResource"), 0, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(0)
			if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("if attribute collection=\"yes\"") ) == 0 )
				fo_MarcData[iReader] = _T('c');
		}
		
		// 1.Data Inti...
		FreeTempMODSStru( &tModsTemp );
	}

	// 2.Get Data...
	ids = GetElementStru( fi_tMods, _T("issuance"), 1, &tModsTemp);
	if ( ids )
		return ids;

	// 3.Check Data Exist...
	if ( tModsTemp.nSubCnt > 0 )
	{
		// 4.Get TAG Data
		for ( i = 0; i < tModsTemp.nSubCnt; i++ )
		{
			// SEL(0)
			if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("continuing") ) == 0 )
				fo_MarcData[iReader] = _T('s');
			if ( _tcscmp( tModsTemp.tModsSub[i].prgData, _T("monographic") ) == 0 )
				fo_MarcData[iReader] = _T('m');
		}
	}

	FreeTempMODSStru( &tModsTemp );

	return 0;
}
/***********************************************************************************************************************/
// END : CREATE MARC FUNCTION...
/***********************************************************************************************************************/

/***********************************************************************************************************************/
// FREE MODS STRU...
/***********************************************************************************************************************/
/*********************************************/
// TEMP MODE FREE...
/*********************************************/
INT	CModsConvert::FreeTempMODSStru( MODS_STRU *tModsTemp )
{
	INT	i;
	
	for ( i = tModsTemp->nSubCnt - 1; i >= 0; i-- )
	{
		if ( tModsTemp->tModsSub[i].cType != NULL )
		{
			free( tModsTemp->tModsSub[i].cType );
			tModsTemp->tModsSub[i].cType = NULL;
		}
		if ( tModsTemp->tModsSub[i].prgData != NULL )
		{
			free( tModsTemp->tModsSub[i].prgData );
			tModsTemp->tModsSub[i].prgData = NULL;
		}
	}
	
	if ( tModsTemp->tModsSub != NULL )
	{
		free( tModsTemp->tModsSub );
		tModsTemp->tModsSub = NULL;
	}

	tModsTemp->nSubCnt = 0;

	return 0;
}

/*********************************************/
// ALL MODS DATA FREE...
/*********************************************/
INT	CModsConvert::FreeMODSStru( MODS_STRU *fi_tMods )
{
	INT				i;
	INT				ids;
	INT				iCurrentDep;
	MODS_SUB_STRU	*tCurrent;

	iCurrentDep = -1;
	tCurrent = NULL;

	for ( i = fi_tMods->nSubCnt - 1; i >= 0 ; i-- )
	{
		tCurrent = &fi_tMods->tModsSub[i];

		ids = FreeMODSSubStru( tCurrent, &iCurrentDep );
		if ( ids )
			return ids;
	}

	return 0;
}

/*********************************************/
// SUB MODE DATA FREE...
/*********************************************/
INT	CModsConvert::FreeMODSSubStru( MODS_SUB_STRU *fi_Current, INT *fi_CurrentDep )
{
	INT	i;
	INT	ids;

	(*fi_CurrentDep)++;

	if ( fi_Current->nSubCnt <= 0 )
	{
		if ( fi_Current->cType != NULL )
		{
			free( fi_Current->cType );
			fi_Current->cType = NULL;
		}
		if ( fi_Current->prgData != NULL )
		{
			free( fi_Current->prgData );
			fi_Current->prgData = NULL;
		}

		(*fi_CurrentDep)--;
		return 0;
	}
	else
	{
		for ( i = fi_Current->nSubCnt - 1; i >= 0; i-- )
		{
			ids = FreeMODSSubStru( &fi_Current->tNextSub[i], fi_CurrentDep );
			if ( ids )
				return ids;
		}
	}

	if ( fi_Current->tNextSub != NULL )
	{
		free( fi_Current->tNextSub );
		fi_Current->tNextSub = NULL;
	}

	(*fi_CurrentDep) = (*fi_CurrentDep) - 1;
	return 0;
}
/***********************************************************************************************************************/
// END : FREE MODS STRU...
/***********************************************************************************************************************/
/*
INT CModsConvert::LoadTopElementsName()
{
	INT		i;
	INT		nTopElementCnt;
	TCHAR	cFileCheck;
	TCHAR	szLine[1024];
	CString	strTemp;
	FILE	*fp;
	
	// 1.Data Init...
	nTopElementCnt = i = 0;
	szTopElementsName = NULL;
	fp = NULL;
	memset( szLine, 0x00, sizeof( szLine ) );

	// 2.Ref File Open
	fp = _tfopen( szTopElementsFileName, _T("rb") );
	if( fp == NULL )
		return -10;

	// 3.Sel File incoding
	cFileCheck = _fgettc( fp );
	if ( 0xfeff != cFileCheck )
		fseek( fp, 0x00, SEEK_SET );		// ANSI

	// 4.Top Element Count
	while( _fgetts( szLine, 1024, fp ) != NULL )
	{
		StrTrimAll( szLine, szLine, _T('\n') );
		if ( _tcscmp( szLine, _T("") ) != 0 )
			nTopElementCnt++;
	}

	// 5.Set File pointer Front
	if ( 0xfeff != cFileCheck )
		fseek( fp, 0x00, SEEK_SET );		// ANSI
	else
		fseek(fp, 0x02, SEEK_SET);			// UNICODE
	
	m_TopElementCnt = nTopElementCnt;
	szTopElementsName = (TCHAR**) malloc ( sizeof(TCHAR*) * nTopElementCnt );

	// 6.Read Top Elements
	while( _fgetts( szLine, 1024, fp ) != NULL ) 
	{
		StrTrimAll( szLine, szLine, _T('\n') );
		StrTrimAll( szLine, szLine, _T('\r') );
		if( _tcscmp( szLine, _T("") ) != 0 ) 
		{
			szTopElementsName[i] = (TCHAR*) malloc ( sizeof(TCHAR) * _tcslen( szLine ) + sizeof(TCHAR) * 3 );
			_tcscpy( szTopElementsName[i], szLine );
			i++;
		}
	}

	fclose(fp);

	return 0;
}
*/

