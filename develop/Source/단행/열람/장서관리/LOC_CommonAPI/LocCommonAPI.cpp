// LocCommonAPI.cpp: implementation of the CLocCommonAPI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LocCommonAPI.h"
#include "..\..\..\..\공통\K2UP_Common\MakeSearchData.h"
#include "ReadCfgFile.h"

#include "..\..\..\..\공통\HASH_API\CHash.h"


#include "../../../공통/BO_SPECIES_API/ISBNChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocCommonAPI::CLocCommonAPI(CESL_Mgr *pWnd )
	: CSPF_Work(pWnd)
{

}

CLocCommonAPI::~CLocCommonAPI()
{
}

INT CLocCommonAPI::MakeSql( CString *sGetData , TCHAR **sDBFieldName , INT *nDBFieldType , INT nCnt , CString &sWhere , CESL_Mgr* pEslMgr )
{
	EFS_BEGIN
	
	CMakeSearchData MakeSearchData(pEslMgr);
		
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		sGetData[i].TrimLeft(); sGetData[i].TrimRight();

		switch( nDBFieldType[i] )
		{
		case ONENUMERIC :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += sDBFieldName[i] + CString(_T(" = ")) + sGetData[i] + CString(_T(" "));
			}
			break;
		case TWONUMERIC :
			if( !sGetData[i].IsEmpty() )
			{
				if( !sGetData[i+1].IsEmpty() )
				{
					if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
					sWhere += sDBFieldName[i] + CString(_T(" BETWEEN ")) + sGetData[i] + CString(_T(" AND ")) + sGetData[i+1];
				}
				else
				{
					if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
					sWhere += sDBFieldName[i] + CString(_T(" = ")) + sGetData[i] + CString(_T(" "));
				}
			}
			break;
		case ONESTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
			}
			break;
		case ONELIKESTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += sDBFieldName[i] + CString(_T(" LIKE '%")) + sGetData[i] + CString(_T("%' "));
			}
			break;
		case TWOSTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( sGetData[i+1].IsEmpty() )
				{
					sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i] + CString(_T("' "));
				}
				else
				{
					sWhere += CString(_T("( ")) + sDBFieldName[i] + CString(_T(" BETWEEN '")) + sGetData[i] + CString(_T("' AND '")) + sGetData[i+1] + CString(_T("' ) "));
				}
				i++;
			}
			break;
		case ONEDATE:
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				sWhere += sDBFieldName[i] + 
					CString(_T(" BETWEEN to_date('")) + sGetData[i] + CString(_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS' AND to_date('")) + sGetData[i] +_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')");
			}
			break;
		case TWODATE:
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( sGetData[i+1].IsEmpty() )
				{
					sWhere += sDBFieldName[i] + CString(_T(" BETWEEN to_date('")) + sGetData[i] + CString(_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")) + sGetData[i] +_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')");
				}
				else
				{
					sWhere += sDBFieldName[i] + CString(_T(" BETWEEN to_date('")) + sGetData[i] + CString(_T(" 00:00:00','YYYY/MM/DD HH24:MI:SS') AND to_date('")) + sGetData[i+1] +_T(" 23:59:59','YYYY/MM/DD HH24:MI:SS')");
				}
				i++;
			}
			break;
		case THREESTRING :
			if( !sGetData[i].IsEmpty() )
			{
				if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ") ;
				if( !sGetData[i+1].IsEmpty() )
				{
					CString sAlias = sDBFieldName[i+1];
					if( sAlias.Find(_T("REG_NO")) > -1 )
					{
						CString sRegNo1 , sRegNo2;
						sRegNo1 = sGetData[i]+sGetData[i+1];

						MakeRegNoSpaceToZero( sRegNo1 );

						if( sGetData[i+2].IsEmpty() )
						{
							sWhere += sDBFieldName[i+1] + CString(_T(" = '")) + sRegNo1 + CString(_T("' "));
						}
						else
						{
							sRegNo2 = sGetData[i]+sGetData[i+2];
							MakeRegNoSpaceToZero( sRegNo2 );
							sWhere += CString(_T("( ")) + sDBFieldName[i+1] + CString(_T(" BETWEEN '")) + sRegNo1 +CString(_T("' AND '")) + sRegNo2 + _T("' ) ");
						}

					}
					else
					{
						if( sGetData[i+2].IsEmpty() )
						{
							sWhere += sDBFieldName[i] + CString(_T(" = '")) + sGetData[i]+sGetData[i+1]+ CString(_T("' "));
						}
						else
						{
							sWhere += CString(_T("( ")) + sDBFieldName[i] + CString(_T(" BETWEEN '")) + sGetData[i]+sGetData[i+1] +CString(_T("' AND '")) + sGetData[i]+sGetData[i+2] + _T("' ) ");
						}
					}
				}
				else
				{
					CString sAlias = sDBFieldName[i+1];
					if( sAlias.Find(_T("REG_NO")) < 0  )
						sWhere += sDBFieldName[i] + CString(_T(" LIKE('")) + sGetData[i] + CString(_T("%') "));
					else
						sWhere += _T(" 1=1 ");
				}
				i = i+2;
			}
			break;
		case ONEINDEXSEARCH :
			if( !sGetData[i].IsEmpty() )
			{
				BOOL bIsEqualSearch = FALSE;

				if( sGetData[i].GetAt(0) == '\"' )
					bIsEqualSearch = TRUE;

				MakeSearchData.GetKeyWord(sGetData[i]);
				if( !bIsEqualSearch ) 
					sGetData[i] += _T("*");
				


				if( 1 == pEslMgr->m_pInfo->GetSearchIndexType() )
				{
					continue;
				}
				else
				{
					if( sWhere.GetLength() > 1 ) sWhere += _T(" AND ");
						sWhere += CString(_T(" ( CATSEARCH( ")) + sDBFieldName[i] + CString(_T(" , '")) + sGetData[i] + CString(_T("',NULL) > 0 ) "));
				}
								
			}
			break;
		default:
			break;
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::MakeShelfCode( CString sDMAlias )
{
	EFS_BEGIN

	INT ids;

	ids = MakeShelfCode( m_pParentMgr , sDMAlias );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("MakeShelfCode( CString sDMAlias )") );
	
	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::MakeShelfCode( CESL_Mgr *pEslMgr , CString sDMAlias )
{
	EFS_BEGIN

	INT ids;


	CESL_DataMgr *pDM = pEslMgr->FindDM( sDMAlias );
	if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("CBL_BOSHELF::MakeShelfCode( CString sDMAlias )") );

	INT nRowCount = pDM->GetRowCount();

	CString sShelfCode;
	for( INT i = 0 ; i < nRowCount ; i++ )
	{
		ids = MakeShelfCode( pEslMgr , pDM , i , sShelfCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -2 , _T("CBL_BOSHELF::MakeShelfCode( CString sDMAlias )") );
	}


	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::MakeShelfCode( CESL_Mgr *pEslMgr , CESL_DataMgr* pDM , INT nIndex , CString &sShelfCode )
{
	EFS_BEGIN

	INT ids;

	sShelfCode.Empty();
	CString sTmpCode;
	const INT nCnt = 5;
	TCHAR *sFieldAlias[nCnt] =
	{
		_T("IBS_청구_별치기호_코드") ,	
		_T("IBS_청구_분류기호") ,		
		_T("IBS_청구_도서기호") ,		
		_T("IBS_청구_권책기호") ,		
		_T("IBS_청구_복본기호") ,		
	};
	

	for( INT j = 0 ; j < nCnt ; j++ )
	{
		ids = pDM->GetCellData( sFieldAlias[j] , nIndex , sTmpCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -2 , _T("CBL_BOSHELF::MakeShelfCode( CString sDMAlias )") );
		sTmpCode.TrimLeft(); sTmpCode.TrimRight();
		
		if( j == 0 && !sTmpCode.IsEmpty() )
		{
			pEslMgr->m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("별치기호") , sTmpCode , sTmpCode );
		}
		
		if( !sTmpCode.IsEmpty() ) 
		{
			if( sShelfCode.GetLength() > 1 )
			{
				if( j == 1 )
					sShelfCode += _T(" ");
				else if( j == 4 )
					sShelfCode += _T("=");
				else
					sShelfCode += _T("-");
			}
			sShelfCode += sTmpCode;
		}
	}
	
	ids = pDM->SetCellData( _T("청구기호") , sShelfCode , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -3 , _T("CBL_BOSHELF::MakeShelfCode( CString sDMAlias )") );
	
	return 0;

	EFS_END

	return -1;
	
}


INT CLocCommonAPI::GetWorkingStatusDescToCode( CString &sCodeValue , INT nMode )
{
EFS_BEGIN

	const INT nCnt = 75;

	CString sWorkingStatus[nCnt][2] =
	{
		{_T("BOT111O"),_T("단행기초자료") },
		{_T("BOT112O"),_T("단행반입자료") },
		{_T("BOA111N"),_T("단행구입대상자료") },
		{_T("BOA112N"),_T("단행주문자료(검수대상자료)") },
		{_T("BOA113O"),_T("단행미납품자료") },
		{_T("BOA114O"),_T("단행(구입)인계대상자료") },
		{_T("BOA211O"),_T("단행기증접수자료(인계대상자료)") },
		{_T("BOA212O"),_T("단행기증부당자료") },
		{_T("BOR111N"),_T("단행등록인계자료") },
		{_T("BOR112N"),_T("단행등록자료(정리인계대상자료)") },
		{_T("BOR113O"),_T("단행가원부번호부여자료") },
		{_T("BOC111O"),_T("단행복본조사대상자료") },
		{_T("BOC112O"),_T("단행분류대상자료") },
		{_T("BOC113N"),_T("단행정리대상자료") },
		{_T("BOC114O"),_T("단행열람인계대상자료(정리완료자료)") },
		{_T("BOL111O"),_T("단행열람인계자료(배가대상자료)") },
		{_T("BOL112N"),_T("비치자료") } ,
		{_T("BOL211O"),_T("관외대출자료") } ,    
		{_T("BOL212O"),_T("관내대출자료") } ,    
		{_T("BOL213O"),_T("특별대출자료") } ,    
		{_T("BOL214O"),_T("긴급대출자료") } ,    
		{_T("BOL215O"),_T("배달자료") } ,        
		{_T("BOL216O"),_T("무인반납자료") },		
		{_T("BOL217O"),_T("장기대출자료") } ,		
		{_T("BOL218O"),_T("상호대차대출자료") } ,		
		{_T("BOL219O"),_T("책나래대출자료") } ,
		{_T("BOL113O"),_T("수리제본자료") } ,    
		{_T("BOL252N"),_T("분실자료") } ,        
		{_T("BOL115O"),_T("제적자료") } ,
		{_T("BOL114O"),_T("파손자료") } ,
		{_T("BOC211O"),_T("긴급정리대상자료")},
		{_T("BOL116O"),_T("재정리자료")},		
		{_T("BOL411O"),_T("단행책두레업무진행자료")},	
		{_T("BOL511O"),_T("통합도서타관반납")},
		{_T("BOL611O"),_T("통합도서타관대출")},		
		{_T("SEA111N") ,_T("구입입수자료(인계대상자료)") },
		{_T("SEA112N") ,_T("구입결호자료") },
		{_T("SEA211N") ,_T("기증입수자료(인계대상자료)") },
		{_T("SEA212N") ,_T("기증결호자료") },
		{_T("SEB111N") ,_T("제본인계자료") },
		{_T("SEB112N") ,_T("제본결정자료") },
		{_T("SEB113N") ,_T("제본발주자료") },
		{_T("SEB114N") ,_T("제본검수자료(등록인계대상자료)") },
		{_T("SEL111N") ,_T("실시간인계자료(실시간배가대상자료)") },
		{_T("SEL113O") ,_T("실시간폐기자료") },
		{_T("SER111N") ,_T("등록인계자료") },
		{_T("SER112N") ,_T("등록자료(정리인계대상자료)") },
		{_T("SER113O") ,_T("가원부번호부여자료") },
		{_T("SEC111N") ,_T("정리대상자료") },
		{_T("SEC112O") ,_T("열람인계대상자료(정리완료자료)") },
		{_T("SEL211O") ,_T("열람인계자료(배가대상자료)") },
		{_T("SEL111O"),_T("실시간인계자료")},
		{_T("SEL112N"),_T("실시간배가자료")},
		{_T("SEL113O"),_T("실시간폐기자료")},
		{_T("SER111N"),_T("등록인계자료")},
		{_T("SER112N"),_T("등록자료(정리인계대상자료)")},
		{_T("SER113O"),_T("가원부번호부여자료")},
		{_T("SEC111N"),_T("정리대상자료")},
		{_T("SEC112O"),_T("열람인계대상자료(정리완료자료")},
		{_T("SEL211O"),_T("열람인계자료(배가대상자료)")},
		{_T("SEL212N"),_T("연속비치자료")},
		{_T("SEL213O"),_T("연속수리제본자료")},
		{_T("SEL214O"),_T("연속파손자료")},
		{_T("SEL215O"),_T("연속제적자료")},
		{_T("SEL216O"),_T("연속분실자료")},
		{_T("SEL311O"),_T("연속관외대출자료")},
		{_T("SEL312O"),_T("연속관내대출자료")},
		{_T("SEL313O"),_T("연속특별대출자료")},
		{_T("SEL314O"),_T("연속긴급대출자료")},
		{_T("SEL315O"),_T("연속배달자료")},		
		{_T("SEL317O"),_T("연속장기대출자료")},
		{_T("SEL111N"),_T("실시간인계자료(실시간배가대상자료)")},		
		{_T("SEL217O"),_T("연속재정리자료")},		
		{_T("SEL411O"),_T("연속책두레업무진행자료")},		
		{_T("SEL511O"),_T("통합도서타관반납")}
	};
	
	INT nIndex;
	INT nReturnIdx;

	if( nMode == 0 )
	{
		nIndex = 1;
		nReturnIdx = 0;
	}
	else
	{
		nIndex = 0;
		nReturnIdx = 1;
	}

	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sCodeValue.Compare( sWorkingStatus[i][nIndex] ) == 0 )
		{
			sCodeValue = sWorkingStatus[i][nReturnIdx];
			return 0;
		}
	}
	
	if( sCodeValue.Compare(_T("대출자료")) == 0 )
	{
#ifdef __K2UP__
		sCodeValue = _T("'BOL211O','BOL212O','BOL213O','BOL214O','BOL215O','BOL217O','BOL411O','BOL511O','BOL511O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL317O','SEL411O','SEL511O'");		
#else
		sCodeValue = _T("'BOL221O','BOL222O','BOL223O','BOL224O','BOL225O','BOL411O','BOL511O','BOL611O','SEL311O','SEL312O','SEL313O','SEL314O','SEL315O','SEL411O','SEL511O'");
		
#endif
	}

	return 0;

EFS_END
return -1;
}

INT CLocCommonAPI::MakeRegNoSpaceToZero( CString &sRegNo )
{
EFS_BEGIN
	
	CString sRegCode = _T("");
	CString sRegNumber = _T("");

	sRegNo.TrimLeft();
	sRegNo.TrimRight();
	sRegNo.Replace( _T(" "), _T("") );
	
	if(_T("$") == sRegNo.Left(1))
	{ 
		INT nLength = sRegNo.GetLength();
		if(7 < nLength)
		{ 
			sRegNo = sRegNo.Mid(7, nLength);
		}
	}

	INT nRegCodeLength = 0;
	INT nFind = 0;
	CStdioFile file;
	CString strLine = _T("");
	CString strRegCodeLength = _T("");
	if (file.Open(_T("..\\Cfg\\RegCodeLength.TXT"), CFile::modeRead|CFile::typeBinary))
	{
		TCHAR cUni;
		file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			file.SeekToBegin();
		}
		
		while (file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();

			nFind = strLine.Find(_T("등록구분코드자리수"));
			if (nFind < 0) continue;
			nFind = strLine.Find(_T("|"));

			strRegCodeLength = strLine.Mid(nFind+1);
			strRegCodeLength.TrimLeft(); strRegCodeLength.TrimRight();
			nRegCodeLength = _ttoi(strRegCodeLength);
		}
	}
	if ( sRegNo.GetLength() <= nRegCodeLength ) return 1;

	sRegCode = sRegNo.Mid( 0 , nRegCodeLength );
	sRegNumber = sRegNo.Mid(nRegCodeLength);

	for ( INT i = 0;i < 12 - sRegNo.GetLength();i++ )
		sRegCode += _T("0");
	
	sRegNo = sRegCode + sRegNumber;

	return 0;

EFS_END
	return -1;
}

INT CLocCommonAPI::MakeRegNoSpaceToZeroAuto(CString &strRegNo)
{
	INT i = 0, j = 0, nRegCodeLength = 0;
	CString strRegCode, strRegNumber;

	if(strRegNo.GetLength() > 12)
		return -1;

	for(i = 0; i < strRegNo.GetLength(); i++){
		if(IsNumericText(strRegNo.Mid(i, 1)) == TRUE){
			strRegCode = strRegNo.Left(i);
			strRegNumber = strRegNo.Right(strRegNo.GetLength() - i);
			for(j = 0; j < 12 - strRegNo.GetLength(); j++){
				strRegCode += _T("0");
			}
			strRegNo = strRegCode + strRegNumber;
			break;
		}
	}
	return 0;
}

INT CLocCommonAPI::MakeRegNoSpaceToZero( CESL_Mgr *pEslMgr , CString &sRegNo , CString strManageCode, CPreRegCodeMgr* pPreRegCodeMgr)
{
	EFS_BEGIN

	BOOL bIsFree = FALSE;


	if( pPreRegCodeMgr == NULL ) bIsFree = TRUE;

	if( pPreRegCodeMgr == NULL )
		pPreRegCodeMgr = new CPreRegCodeMgr(pEslMgr);

	pPreRegCodeMgr->IsInPreRegNo( sRegNo , strManageCode, sRegNo );

	INT ids = MakeRegNoSpaceToZero( sRegNo );

	if( bIsFree )
		delete pPreRegCodeMgr;
	
	return ids;

	EFS_END
	return -1;

}
INT CLocCommonAPI::MakeRegNoSpaceToZero( CESL_Mgr *pEslMgr , CString &sRegNo , CPreRegCodeMgr* pPreRegCodeMgr)
{
	DWORD dwPID = ::GetCurrentProcessId();
	CString strCHMfileName;
  	strCHMfileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwPID);
	
	CStdioFile fd;
	if (TRUE == fd.Open(strCHMfileName, CFile::modeRead|CFile::typeBinary)) 
	{
		TCHAR cUni;
		fd.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fd.SeekToBegin();
		}
		CString strLine;
		fd.ReadString(strLine);
		strLine.TrimLeft();			strLine.TrimRight();
		if (strLine.GetLength() > 0)
		{
			strCHMfileName = strLine;
		}
		fd.Close();

		CString strManageCode=strLine;

		if(_T("$") == sRegNo.Left(1))
		{ 
			INT nLength = sRegNo.GetLength();
			if(7 < nLength)
			{ 
				sRegNo = sRegNo.Mid(7, nLength);
			}
		}

		return MakeRegNoSpaceToZero(pEslMgr, sRegNo, strManageCode, pPreRegCodeMgr);
	}	

	return 0;
}

INT CLocCommonAPI::MakeRegNoSpaceToZeroFromLib( CESL_Mgr *pEslMgr , CString &sRegNo , CPreRegCodeMgr* pPreRegCodeMgr, CString strLibCode)
{
	if(_T("$") == sRegNo.Left(1))
	{ 
		INT nLength = sRegNo.GetLength();
		if(7 < nLength)
		{ 
			CString strData = sRegNo;
			strLibCode = strData.Mid(1, 6);
			sRegNo = strData.Mid(7, nLength);
		}
	}

	INT nRegCodeLength = 0;
	CString strPreRegCodeLength = _T("");
	nRegCodeLength = pEslMgr->m_pInfo->nREG_CODE_LENGTH;

	CString strManageCode;
	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));

	CString strQuery;
	strQuery.Format( _T("SELECT MANAGE_CODE FROM MN_LIBINFO2_TBL WHERE LIB_CODE='%s' "), strLibCode );
	INT ids = TmpDM.GetOneValueQuery(strQuery, strManageCode);
	if( 0 > ids && 1 > TmpDM.GetRowCount() ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("CLocCommonAPI::MakeRegNoSpaceToZeroFromLib") );

	strQuery.Format(_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE CLASS_ALIAS = '등록구분코드자리수설정' AND MANAGE_CODE = '%s'"),strManageCode);
	TmpDM.GetOneValueQuery(strQuery, strPreRegCodeLength);
	
	INT nPreRegCodeLength = _ttoi(strPreRegCodeLength);
	if(nPreRegCodeLength != nRegCodeLength)
	{
		SaveRegCodeLengthToFile(pEslMgr, nPreRegCodeLength);
	}

	ids = MakeRegNoSpaceToZero(pEslMgr, sRegNo, strManageCode, pPreRegCodeMgr);

	if(nPreRegCodeLength != nRegCodeLength)
	{		
		SaveRegCodeLengthToFile(pEslMgr, nRegCodeLength);
	}

	return ids;
}

INT CLocCommonAPI::SaveRegCodeLengthToFile(CESL_Mgr *pEslMgr, INT nLength)
{
	pEslMgr->m_pInfo->nREG_CODE_LENGTH = nLength;

	CString strResult[2];
	CStdioFile file;
	if (file.Open(_T("..\\Cfg\\RegCodeLength.TXT"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		if( 0 == file.GetLength() )
		{			
			TCHAR cUni = 0xFEFF;
			file.Write( &cUni, sizeof(TCHAR));
		}

		CString strLine;
		strLine.Format(_T("등록구분코드자리수|%d\r\n"), pEslMgr->m_pInfo->nREG_CODE_LENGTH);
		file.WriteString(strLine);

		file.Close();
	}
	return 0;
}

BOOL CLocCommonAPI::IsNumericText(CString strValue)
{
	INT nLength = strValue.GetLength();
	if(1 > nLength) return FALSE;
	BOOL bCheck = FALSE;
	for(INT i = 0; i < strValue.GetLength(); i++)
	{
		TCHAR cValue = strValue.Mid(i, 1).GetAt(0);
		if(cValue >= '0' && cValue <= '9')
		{
			bCheck = TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return bCheck;
}

INT CLocCommonAPI::CopyDM( CString sSrcDMAlias , CString sDstDMAlias )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = m_pParentMgr->FindDM( sSrcDMAlias );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -1 , _T("CopyDM( CString sSrcDMAlias , CString sDstDMAlias )") );

	CESL_DataMgr *pDstDM = m_pParentMgr->FindDM( sDstDMAlias );
	if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -2 , _T("CopyDM( CString sSrcDMAlias , CString sDstDMAlias )") );

	INT nAliasCnt;
	nAliasCnt = pDstDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDstDM->GetAllAlias( sColAlias , nAliasCnt );

	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -3, _T("CopyDM( CString sSrcDMAlias , CString sDstDMAlias )") );

	CString sResult;

	pDstDM->FreeData();

	for( INT j = 0 ; j < pSrcDM->GetRowCount() ; j++ )
	{
		pDstDM->InsertRow(-1);

		for( INT i = 0 ; i < nAliasCnt ; i++ )
		{
			ids = pSrcDM->GetCellData( sColAlias[i] , j , sResult );
			if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -4 , _T("CopyDM( CString sSrcDMAlias , CString sDstDMAlias )") );
			if( ids == -4012 ) sResult.Empty();
			
			ids = pDstDM->SetCellData( sColAlias[i] , sResult , pDstDM->GetRowCount()-1 );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( m_pParentMgr->m_hWnd , -5 , _T("CopyDM( CString sSrcDMAlias , CString sDstDMAlias )") );
		}
	}

	delete []sColAlias;

	return 0;



	EFS_END
	return -1;

}

INT CLocCommonAPI::AddDM( CString sSrcDMAlias , CString sDstDMAlias , CESL_Mgr *pEslMgr )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr *pSrcDM = pEslMgr->FindDM( sSrcDMAlias );
	if( pSrcDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("AddDM( CString sSrcDMAlias , CString sDstDMAlias )") );

	CESL_DataMgr *pDstDM = pEslMgr->FindDM( sDstDMAlias );
	if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -2 , _T("AddDM( CString sSrcDMAlias , CString sDstDMAlias )") );

	ids = AddDM( pSrcDM , pDstDM , pEslMgr );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -3 , _T("AddDM") );

	return 0;



	EFS_END
	return -1;

}

INT CLocCommonAPI::AddDM( CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM , CESL_Mgr *pEslMgr )
{
	EFS_BEGIN

	INT ids;

	for( INT j = 0 ; j < pSrcDM->GetRowCount() ; j++ )
	{
		ids = AddDM( pSrcDM , pDstDM , j , -1 , pEslMgr );
		if( ids < 0 )  ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1, _T("AddDM") );
	}


	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::AddDM(	CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM , INT nSrcIndex , INT nDstIndex , CESL_Mgr *pEslMgr )
{
	EFS_BEGIN
	
	INT ids;

	INT nAliasCnt;
	nAliasCnt = pDstDM->RefList.GetCount();

	CString *sColAlias;
	sColAlias = new CString[nAliasCnt];
	
	ids = pDstDM->GetAllAlias( sColAlias , nAliasCnt );
	if( pDstDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1, _T("AddDM") );

	if( nDstIndex == -1 )
	{
		pDstDM->InsertRow(-1);
		nDstIndex = pDstDM->GetRowCount()-1;
	}

	CString sResult;
	for( INT i = 0 ; i < nAliasCnt ; i++ )
	{
		ids = pSrcDM->GetCellData( sColAlias[i] , nSrcIndex , sResult );
		if( ids < 0 && (ids != -4012) )
		{
			CString sEMsg;
			sEMsg.Format( _T("sColAlias[i] : %s ") , sColAlias[i] );
			AfxMessageBox( sColAlias[i]);
		}
		if( ids < 0 && (ids != -4012) ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -2 , _T("AddDM") );
		if( ids == -4012 ) sResult.Empty();
		
		ids = pDstDM->SetCellData( sColAlias[i] , sResult , nDstIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -3 , _T("AddDM") );
	}

	delete []sColAlias;

	return 0;

	EFS_END
	return -1;
}

INT CLocCommonAPI::InsertGridRow(  CESL_Mgr *pEslMgr , CString sResultCMAlias , CString sResultGridAlias , INT nDMIndex , INT nGridIndex, INT nRowHeight )
{
	EFS_BEGIN

	INT ids;

	CESL_ControlMgr *pCM = pEslMgr->FindCM( sResultCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("InsertGridRow") ); 

	CESL_Grid *pGrid = (CESL_Grid*)pCM->FindControl(sResultGridAlias);
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -2 , _T("InsertGridRow") );

	ids = InsertGridRow( pEslMgr , pGrid , nDMIndex , nGridIndex , nRowHeight);
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -3 , _T("InsertGridRow") );
	
	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::InsertGridRow(  CESL_Mgr *pEslMgr , CESL_Grid *pGrid , INT nDMIndex , INT nGridIndex , INT nRowHeight )
{
	EFS_BEGIN

	INT ids;

	INT nColCount = pGrid->GetColCount();

	if( nGridIndex == -1 )
		nGridIndex = pGrid->GetCount();

	CString sData;
	CESL_DataMgr *pDM;
	for( INT k = 0 ; k < nColCount ; k++ )
	{
		POSITION pos = pGrid->m_arrColumnInfo.FindIndex(k);
		CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);

		if( pColumnInfo->strColumnDMAlias.IsEmpty() ) continue;
		if( pColumnInfo->strColumnDMFieldAlias.IsEmpty()) continue;

		pDM = pEslMgr->FindDM( pColumnInfo->strColumnDMAlias );
		if( pDM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("InsertGridRow") );

		ids = pEslMgr->GetDataMgrData( pColumnInfo->strColumnDMAlias , pColumnInfo->strColumnDMFieldAlias , sData , nDMIndex );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -2 , _T("InsertGridRow") );

		pGrid->SetAt( nGridIndex , k , sData );

	}

	if( nRowHeight != -1 ) 
		pGrid->SetRowHeight( nGridIndex+1 , nRowHeight );

	CString sIndex;
	sIndex.Format(_T("%d"),nGridIndex+1);
	pGrid->SetTextMatrix( nGridIndex+1 , 0 , sIndex );

	pGrid->m_nRealRowCount = pDM->GetRowCount();
	
	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::GetGridColumnIndexs( CESL_Mgr *pMgr , CESL_Grid *pGrid , TCHAR **sLinkFieldAlias , INT nLinkFieldCnt , INT *nColIndex )
{
	EFS_BEGIN

	INT nColCount = pGrid->GetColCount();
	
	for( INT j = 0 ; j < nLinkFieldCnt ; j++ )
	{
		for( INT k = 0 ; k < nColCount ; k++ )
		{
			POSITION pos = pGrid->m_arrColumnInfo.FindIndex(k);

			CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);
			if( pColumnInfo->strColumnDMFieldAlias.Compare(sLinkFieldAlias[j]) == 0 )
			{
				nColIndex[j] = k;
				break;
			}
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::GetGridColumnIndexs( CESL_Mgr *pMgr , CESL_Grid *pGrid , TCHAR *sLinkFieldAlias , INT &nColIndex )
{
	EFS_BEGIN

	INT nColCount = pGrid->GetColCount();
	
	for( INT k = 0 ; k < nColCount ; k++ )
	{
		POSITION pos = pGrid->m_arrColumnInfo.FindIndex(k);

		CESL_GridColumnInfo *pColumnInfo = (CESL_GridColumnInfo*)pGrid->m_arrColumnInfo.GetAt(pos);
		if( pColumnInfo->strColumnDMFieldAlias.Compare(sLinkFieldAlias) == 0 )
			nColIndex = k;
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::GetGridColumnIndexs( CESL_Mgr *pMgr , CString sCMAlias , CString sGridAlias , TCHAR **sLinkFieldAlias , INT nLinkFieldCnt , INT *nColIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_ControlMgr *pCM = pMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -1 , _T("GetGridColumnIndexs") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -2 , _T("GetGridColumnIndexs") );

	ids = GetGridColumnIndexs( pMgr , pGrid , sLinkFieldAlias , nLinkFieldCnt , nColIndex );
	if( ids < 0 )ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -3 , _T("GetGridColumnIndexs") );

	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::GetGridColumnIndexs( CESL_Mgr *pMgr , CString sCMAlias , CString sGridAlias , TCHAR *sLinkFieldAlias ,INT &nColIndex )
{
	EFS_BEGIN

	INT ids;

	CESL_ControlMgr *pCM = pMgr->FindCM( sCMAlias );
	if( pCM == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -1 , _T("GetGridColumnIndexs") );

	CESL_Grid *pGrid = (CESL_Grid*)(pCM->FindControl( sGridAlias ));
	if( pGrid == NULL ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -2 , _T("GetGridColumnIndexs") );

	ids = GetGridColumnIndexs( pMgr , pGrid , sLinkFieldAlias , nColIndex );
	if( ids < 0 )ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -3 , _T("GetGridColumnIndexs") );

	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::CivilNoCheck( CString sCivilNo1, CString sCivilNo2 )
{
	EFS_BEGIN

	if( 6 != sCivilNo1.GetLength() || 7 != sCivilNo2.GetLength() )
	{
		return -4;
	}

	INT sum, i; 

	sum = 0;

    INT nMonth = _ttoi(sCivilNo1.Mid(2,2));
    if(nMonth < 1 || nMonth > 12)
	{
        return -1;
    }

    INT nDay = _ttoi(sCivilNo1.Mid(4,2));
    if(nMonth < 1 || nMonth > 31)
	{
        return -2;
    }
    
    if( (sCivilNo2.GetAt(0)!='1') && (sCivilNo2.GetAt(0)!= '2') && (sCivilNo2.GetAt(0)!='3') && (sCivilNo2.GetAt(0)!='4') 
		&& (sCivilNo2.GetAt(0)!= '5') && (sCivilNo2.GetAt(0)!= '6') && (sCivilNo2.GetAt(0)!= '7') && (sCivilNo2.GetAt(0)!= '8') )
	{
        return -3;
    }

	if( sCivilNo2.GetAt(0)=='5' || (sCivilNo2.GetAt(0) == '6') || (sCivilNo2.GetAt(0)== '7') || (sCivilNo2.GetAt(0)== '8'))
	{
		INT ids;
		ids = CheckForeignCivilNum( sCivilNo1 + sCivilNo2 );
		if( ids != 0 ) return -1;
	}
	else
	{
		for( i = 0 ; i < 12 ; i++) 
		{ 
			if(i < 6) 
				sum += (sCivilNo1.GetAt(i)-'0') * (i+2); 
			if( i > 5 && i < 8 ) 
				sum += (sCivilNo2.GetAt(i-6)-'0') * (i+2); 
			
			if( i > 7 ) 
				sum += (sCivilNo2.GetAt(i-6)-'0') * (i-6); 
		} 
		
		INT nCheckDigit = 11-(sum%11);
		nCheckDigit = nCheckDigit%10;
		if( nCheckDigit != (sCivilNo2.GetAt(6)-'0') ) 
			return -1;
	}
	
	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::CheckForeignCivilNum( CString civilNum )
{	
	const INT cnt = 13;
	INT num[ cnt ];
	INT sum = 0, odd = 0;
	for( INT i = 0; i < cnt; i++ )
		num[ i ] = _ttoi( civilNum.Mid( i, 1 ) );
	
	odd = num[ 7 ] * 10 + num[ 8 ];
	
	if( odd % 2 != 0 )
		return 1;
	
	INT checkNum[] = { 2, 3, 4, 5, 6, 7, 8, 9, 2, 3, 4, 5 };
	
	for( i = 0, sum = 0; i < 12; i++ )
		sum += ( num[ i ] *= checkNum[ i ] );
	sum = 11 - ( sum % 11 );
	
	if( sum >= 10 )
		sum -= 10;
	sum += 2;
	
	if( sum >= 10 )
		sum -= 10;
	
	if( sum != num[ 12 ] )
		return 3;
	
	return 0;
}

INT CLocCommonAPI::CivilNoCheck( CString sCivilNo )
{
	EFS_BEGIN

	INT ids;
	INT nDashIndex;
	CString sCivilNo1,sCivilNo2;

	if( sCivilNo.GetLength() != 14 )
		return 1;
	
	nDashIndex = sCivilNo.Find(_T("-"));

	if( nDashIndex != 6 ) 
		return 2;

	sCivilNo1 = sCivilNo.Mid(0,nDashIndex);
	sCivilNo2 = sCivilNo.Mid(nDashIndex+1);

	ids = CivilNoCheck( sCivilNo1 , sCivilNo2 );
	if( ids != 0 ) return 3;

	return 0;


	EFS_END
	return -1;

}

INT CLocCommonAPI::SortDM( CESL_Mgr *pMgr , CESL_DataMgr *pDM ,  CArray < CString , CString > &saOrder )
{
	EFS_BEGIN

	INT ids;

	INT nStartRowIdx;
	INT nFinishRowIdx;

	bool bRunSort = FALSE;


	CString sResult;
	CString sTmpResult;
	for( INT i = 0 ; i < saOrder.GetSize() ; i++ )
	{
		for( INT j = 0 ; j < pDM->GetRowCount() ; j++ )
		{
			if( j==0 )
				nStartRowIdx = 0;
			
			if( j!=pDM->GetRowCount()-1 )
			{
				for( INT k = 0 ; k < i ; k++ )
				{					
					ids = pDM->GetCellData( saOrder.GetAt(k) , j , sTmpResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd ,  -2 , _T("SortDM") );
					
					ids = pDM->GetCellData( saOrder.GetAt(k) , j+1 , sResult );
					if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd ,  -3 , _T("SortDM") );
					
					if( sTmpResult.Compare( sResult ) != 0 )
						bRunSort = TRUE;
				}
			}
			else
			{
				bRunSort = TRUE;
			}
			
			
			if( bRunSort )
			{
				nFinishRowIdx = j;
				
				ids = pDM->SORT( saOrder.GetAt(i) , nStartRowIdx , nFinishRowIdx+1 , FALSE );
				if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd ,  -3 , _T("SortDM") );
				
				nStartRowIdx = nFinishRowIdx+1;
				
				bRunSort = FALSE;
			}
		}
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::MakeArrayList( CString sLine , CString sDelimiter , CArray <CString,CString> &asArray )
{
	EFS_BEGIN

	asArray.RemoveAll();

	sLine.TrimLeft(); sLine.TrimRight();

	INT nIndex;
	CString sFieldName;

	
	bool IsStop = FALSE;
	while( TRUE )
	{
		nIndex = sLine.Find(sDelimiter);

		if( nIndex < 0 )
		{
			asArray.Add( sLine );
			IsStop = TRUE;
		}
		else
		{
			sFieldName = sLine.Mid( 0 , nIndex );
			sLine = sLine.Mid( nIndex+1 , sLine.GetLength()-nIndex );
			sFieldName.TrimLeft(); sFieldName.TrimRight();
			asArray.Add( sFieldName );
		}

		if( IsStop ) break;
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::MakeArrayList( CString sLine , CString sDelimiter , CStringArray &asArray )
{
	EFS_BEGIN

	asArray.RemoveAll();

	CArray <CString,CString> asTmpArray;

	MakeArrayList( sLine , sDelimiter , asTmpArray );

	for( INT i = 0 ; i < asTmpArray.GetSize() ; i++ )
	{
		asArray.Add( asTmpArray.GetAt(i) );
	}

	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::GetCellData( CESL_Mgr *pMgr , CESL_DataMgr *pDM , CString sConditionAlias , CString sConditionData , CString sGetAlias , CString &sGetData )
{
	EFS_BEGIN

	INT ids;

	CString sTmpData;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( sConditionAlias , i , sTmpData );
		if( ids < 0 )
		{
			CString sMsg;
			sMsg.Format( _T("ALIAS : %s , Data : %s ") , sConditionAlias , sTmpData );
			AfxMessageBox( sMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -1 , _T("GetCellData") );

		if( sTmpData.Compare( sConditionData ) == 0 )
		{
			ids = pDM->GetCellData( sGetAlias , i , sGetData );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -1 , _T("GetCellData") );

			return 0;
		}
	}

	return 1;

	EFS_END
	return -1;

}


INT CLocCommonAPI::SetCellData( CESL_Mgr *pMgr , CESL_DataMgr *pDM , CString sConditionAlias , CString sConditionData , CString sGetAlias , CString &sGetData )
{
	EFS_BEGIN
		
	INT ids;
	
	CString sTmpData;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		ids = pDM->GetCellData( sConditionAlias , i , sTmpData );
		if( ids < 0 )
		{
			CString sMsg;
			sMsg.Format( _T("ALIAS : %s , Data : %s ") , sConditionAlias , sTmpData );
			AfxMessageBox( sMsg );
		}
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -1 , _T("GetCellData") );
		
		if( sTmpData.Compare( sConditionData ) == 0 )
		{
			ids = pDM->SetCellData( sGetAlias , sGetData , i  );
			if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , -1 , _T("GetCellData") );
			
			return 0;
		}
	}
	
	return 1;
	
	EFS_END
	return -1;
	
}


INT CLocCommonAPI::MakeCopyDM( CESL_Mgr *pEslMgr , CESL_DataMgr *pSrcDM , CESL_DataMgr *pDstDM )
{
	EFS_BEGIN

	INT ids;

	pDstDM->SetCONNECTION_INFO(GetDBConnection(pEslMgr->m_pInfo->MODE));
	pDstDM->TBL_NAME = pSrcDM->TBL_NAME;

	pDstDM->EXTRA_CONDITION = pSrcDM->EXTRA_CONDITION;
	pDstDM->CONDITION = pSrcDM->CONDITION;
	pDstDM->DB_MGR_ALIAS = pSrcDM->DB_MGR_ALIAS;
	

	pDstDM->InitDataMgrRef( pSrcDM->RefList.GetCount() );
	pDstDM->m_nCols = pSrcDM->RefList.GetCount();

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pSrcDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pSrcDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("MakeCopyDM") );

		ids = pDstDM->SetDataMgrRef( k , sFieldAlias , sFieldName , sFieldType  , sInitValue  , sDataType );			
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("MakeCopyDM") );
	}		
	
	return 0;

	EFS_END
	return -1;

}

INT CLocCommonAPI::RefreshDataManager( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sWhere , CProgressCtrl *pProgress/*=NULL*/ )
{
	EFS_BEGIN

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( pEslMgr->m_pInfo->MODE );
	MobileApi.SetDM( pDM );

	INT ids = MobileApi.RefreshDataManager( sWhere , pProgress );
	if( ids < 0 ) return ids;

	return 0;
	
	EFS_END

	return -1;
	
}

INT CLocCommonAPI::RestructDataManager( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sWhere , CProgressCtrl *pProgress/*=NULL*/ )
{
	EFS_BEGIN
		

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( pEslMgr->m_pInfo->MODE );
	MobileApi.SetDM( pDM );

	INT ids = MobileApi.RestructDataManager( sWhere , pProgress );
	if( ids < 0 ) return ids;

	return 0;

	EFS_END
		
	return -1;
	
}

INT CLocCommonAPI::GetOneValueQuery( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sQuery , CString &sValue )
{
	EFS_BEGIN
		

	CKolas2up_MobileApi MobileApi;
	MobileApi.SetMode( pEslMgr->m_pInfo->MODE );
	MobileApi.SetDM( pDM );

	INT ids = MobileApi.GetOneValueQuery( sQuery , sValue );
	if( ids < 0 ) return ids;

	return 0;

	EFS_END
		
	return -1;
		
}

CKolas2up_MobileApi* CLocCommonAPI::InitMobileApi( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM )
{
	EFS_BEGIN
		
	CKolas2up_MobileApi *pMobileApi;
	pMobileApi = new CKolas2up_MobileApi;

	pMobileApi->SetMode( pEslMgr->m_pInfo->MODE );
	pMobileApi->SetDM( pDM );

	return pMobileApi;

	EFS_END
		
	return NULL;
}

INT CLocCommonAPI::MobileApiEndLog( CKolas2up_MobileApi* pMobileApi )
{
	EFS_BEGIN
		
	pMobileApi->EndLog();
	
	if( pMobileApi != NULL )
		delete pMobileApi;
	pMobileApi = NULL;

	return 0;

	EFS_END
		
	return -1;	
}

INT CLocCommonAPI::GetManageValue( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , CString &sValue )
{
EFS_BEGIN
		
	INT ids;

	INT nCnt = pDM->GetRowCount();

	CString sCompareStr;
	INT nIndex;

	ids = FindManageDMIndex( pEslMgr , pDM , sGroup2 , sGroup3 , sAlias , sValueName , nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -5 , _T("GetManageValue") );
	if( ids > 0 )
	{
		sValue.Empty();
		return 0;
	}

	ids = pDM->GetCellData( _T("VALUE_1") , nIndex , sValue );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -5 , _T("GetManageValue") );

	sValue.TrimLeft(); sValue.TrimRight();

	return 0;

EFS_END
return -1;
}

INT CLocCommonAPI::FindManageDMIndex( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sGroup2 , CString sGroup3 , CString sAlias , CString sValueName , INT &nIndex )
{
	EFS_BEGIN
		

	INT ids;

	INT nCnt = pDM->GetRowCount();

	CString sCompareStr;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("GROUP_2") , i , sCompareStr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -1 , _T("GetManageValue") );
		
		if( sCompareStr.Compare( sGroup2 ) != 0 ) continue;

		ids = pDM->GetCellData( _T("GROUP_3") , i , sCompareStr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -2 , _T("GetManageValue") );
		
		if( sCompareStr.Compare( sGroup3 ) != 0 ) continue;

		ids = pDM->GetCellData( _T("CLASS_ALIAS") , i , sCompareStr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -3 , _T("GetManageValue") );
		
		if( sCompareStr.Compare( sAlias ) != 0 ) continue;

		ids = pDM->GetCellData( _T("VALUE_NAME") , i , sCompareStr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -4 , _T("GetManageValue") );
		
		if( sCompareStr.Compare( sValueName ) != 0 ) continue;

		nIndex = i;

		return 0;
	}

	return 1;

	EFS_END
		
	return -1;	
	
}

INT CLocCommonAPI::GetManageValue_Name( CESL_Mgr *pEslMgr, CESL_DataMgr *pDM, CString sGroup2, CString sGroup3, CString sAlias, CString &sValueName )
{
EFS_BEGIN
		
	INT ids;

	INT nCnt = pDM->GetRowCount();

	CString sCompareStr;
	INT nIndex;

	ids = FindManageDMIndex2( pEslMgr, pDM, sGroup2, sGroup3, sAlias, nIndex );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd, -5, _T("GetManageValue_Name") );
	if( ids > 0 )
	{
		sValueName.Empty();
		return 0;
	}

	ids = pDM->GetCellData( _T("VALUE_NAME"), nIndex, sValueName );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd, -5, _T("GetManageValue_Name") );

	sValueName.TrimLeft(); sValueName.TrimRight();

	return 0;

EFS_END
return -1;
}

INT CLocCommonAPI::FindManageDMIndex2( CESL_Mgr *pEslMgr, CESL_DataMgr *pDM, CString sGroup2, CString sGroup3, CString sAlias, INT &nIndex )
{
EFS_BEGIN
		
	INT ids;

	INT nCnt = pDM->GetRowCount();

	CString sCompareStr;
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		ids = pDM->GetCellData( _T("GROUP_2") , i , sCompareStr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -1 , _T("FindManageDMIndex2") );
		
		if( sCompareStr.Compare( sGroup2 ) != 0 ) continue;

		ids = pDM->GetCellData( _T("GROUP_3") , i , sCompareStr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -2 , _T("FindManageDMIndex2") );
		
		if( sCompareStr.Compare( sGroup3 ) != 0 ) continue;

		ids = pDM->GetCellData( _T("CLASS_ALIAS") , i , sCompareStr );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd ,  -3 , _T("FindManageDMIndex2") );
		
		if( sCompareStr.Compare( sAlias ) != 0 ) continue;

		nIndex = i;

		return 0;
	}

	return 1;

EFS_END	
return -1;	
}

INT CLocCommonAPI::GetDMEqualRemoveCnt( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sEqualFieldAlias , CString &sCnt )
{
	EFS_BEGIN
		

	INT ids;

	CStringArray sUniqueValue;

	CString sValue;
	CString sData;
	bool IsEqual;
	for( INT i = 0 ; i < pDM->GetRowCount() ; i++ )
	{
		IsEqual = FALSE;
		ids = pDM->GetCellData( sEqualFieldAlias , i , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("GetDMEqualRemoveCnt") );

		for( INT j = 0 ; j < sUniqueValue.GetSize() ; j++ )
		{
			sValue = sUniqueValue.GetAt(j);
			if( sValue.Compare(sData) == 0 )
				IsEqual = TRUE;

		}

		if( IsEqual ) continue;

		sUniqueValue.Add( sData );
	}

	sCnt.Format( _T("%d") , sUniqueValue.GetSize() );

	return 0;

	EFS_END
		
	return -1;	
	
}

INT CLocCommonAPI::ChangeDMFieldName( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM  , CString sInFieldAlias , CString sInFieldName , CString sInDataType )
{
	EFS_BEGIN
		

	INT ids;

	CString sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType;
	for( INT k = 0 ; k < pDM->RefList.GetCount() ; k++ )
	{
		sFieldAlias = sFieldName = sFieldType = sInitValue = sDataType = _T("");
		ids = pDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("ChangeDMFieldName") );

		if( sFieldAlias.Compare( sInFieldAlias ) != 0 ) continue;

		ids = pDM->SetDataMgrRef( k , sFieldAlias , sInFieldName , sFieldType  , sInitValue  , sInDataType );			
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -2 , _T("ChangeDMFieldName") );

		ids = pDM->GetColumnRefInfo( k , sFieldAlias , sFieldName , sFieldType , sInitValue , sDataType );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -3 , _T("ChangeDMFieldName") );
		
	}		

	return 0;

	EFS_END
	
	return -1;
	
	
}

INT CLocCommonAPI::GetDMEqualRemoveCnt( CESL_Mgr *pEslMgr , CESL_DataMgr *pDM , CString sEqualFieldAlias , CESL_Grid* pGrid , CString &sCnt )
{
	EFS_BEGIN
		

	INT ids;

	pGrid->SelectMakeList();
	pGrid->SelectGetHeadPosition();

	INT nSelectCnt = pGrid->SelectGetCount();

	CStringArray sUniqueValue;

	CString sValue;
	CString sData;
	bool IsEqual;
	INT nIndex;
	for( INT i = 0 ; i < nSelectCnt ; i++ )
	{
		nIndex = pGrid->SelectGetIdx();

		IsEqual = FALSE;
		ids = pDM->GetCellData( sEqualFieldAlias , nIndex , sData );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1 , _T("GetDMEqualRemoveCnt") );

		for( INT j = 0 ; j < sUniqueValue.GetSize() ; j++ )
		{
			sValue = sUniqueValue.GetAt(j);
			if( sValue.Compare(sData) == 0 )
				IsEqual = TRUE;

		}

		if( IsEqual )
		{
			pGrid->SelectGetNext();
			continue;
		}

		sUniqueValue.Add( sData );

		pGrid->SelectGetNext();
	}

	sCnt.Format( _T("%d") , sUniqueValue.GetSize() );

	return 0;

	EFS_END
		
	return -1;
	
	
}

INT CLocCommonAPI::ConvertMailClassCodeToDesc( CESL_Mgr* pEslMgr , CString &sMailClass , INT nMode )
{
	EFS_BEGIN
		
	const INT nCnt = 9;
	CString sWorkingStatus[nCnt][2] =
	{
		{ _T("0") , _T("예약안내(반납시)") } , 
		{ _T("1") , _T("예약안내(비치자료예약)") } , 
		{ _T("2") , _T("반납독촉") } , 
		{ _T("3") , _T("대출안내") } , 
		{ _T("4") , _T("반납안내(연체안된자료)") } , 
		{ _T("5") , _T("반납안내(연체된자료)") } , 
		{ _T("6") , _T("예약취소") } , 
		{ _T("7") , _T("반납연기") } , 		
		{ _T("8") , _T("신규회원안내메일") } 
	};
	
	INT nIndex;
	INT nReturnIdx;
	
	if( nMode == 0 )
	{
		nIndex = 1;
		nReturnIdx = 0;
	}
	else
	{
		nIndex = 0;
		nReturnIdx = 1;
	}
	
	for( INT i = 0 ; i < nCnt ; i++ )
	{
		if( sMailClass.Compare( sWorkingStatus[i][nIndex] ) == 0 )
		{
			sMailClass = sWorkingStatus[i][nReturnIdx];
			return 0;
		}
	}
		
	return 0;

	EFS_END
		
	return -1;
	
}

INT CLocCommonAPI::SortDM( CESL_Mgr *pMgr , CESL_DataMgr *pDM ,  CArray < CString , CString > &saOrder , INT* nCond )
{
	EFS_BEGIN

	INT ids;

	INT nColumnCnt = saOrder.GetSize();
	CString* sAlias = new CString[nColumnCnt];

	for( INT i = 0 ; i < nColumnCnt ; i++ )
	{
		sAlias[i] = saOrder.GetAt(i);
	}

	ids = pDM->SORT( nColumnCnt , sAlias , nCond , ESL_DATAMGR_SORT_ASC );
	if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pMgr->m_hWnd , i , _T("SortDM") );


	delete []sAlias;

	return 0;

	EFS_END

	return -1;
}

CTime CLocCommonAPI::GetCurrentDateTime(CESL_Mgr* pMgr, CString &strCurrentDate)
{
	CTime tCurrent = CTime::GetCurrentTime();
	strCurrentDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), tCurrent.GetYear(), tCurrent.GetMonth(), tCurrent.GetDay(), tCurrent.GetHour(), tCurrent.GetMinute(), tCurrent.GetSecond());

	BOOL bEqual = FALSE;
	CFileFind ff;
	CString strCurrentDateFilePath = _T("..\\cfg\\CurrentDate.cfg");
	if(TRUE == ff.FindFile(strCurrentDateFilePath))
	{
		CStdioFile fFile;
		if(fFile.Open(strCurrentDateFilePath, CFile::modeRead|CFile::typeBinary))
		{
			TCHAR cUni;
			fFile.Read(&cUni, sizeof(TCHAR));
			if(0xFEFF != cUni )
			{
				fFile.SeekToBegin();
			}
			CString strGetData;
			fFile.ReadString(strGetData);
			fFile.Close();
			if(strGetData == strCurrentDate.Left(10))
			{
				bEqual = TRUE;
			}
		}
	}
	ff.Close();
	if(FALSE == bEqual)
	{
		CESL_DataMgr dm;
		dm.SetCONNECTION_INFO(pMgr->m_pInfo->CONNECTION_INFO);
		tCurrent = dm.GetDBTime();
		strCurrentDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), tCurrent.GetYear(), tCurrent.GetMonth(), tCurrent.GetDay(), tCurrent.GetHour(), tCurrent.GetMinute(), tCurrent.GetSecond());
		DeleteFile(strCurrentDateFilePath);
		FILE *fp = _tfopen(strCurrentDateFilePath, _T("w+b"));
		fputc(0xFF, fp); fputc(0xFE, fp);
		_ftprintf(fp, strCurrentDate.Left(10));
		fclose(fp);
	}

	CTime tResult(_ttoi(strCurrentDate.Mid(0, 4)), _ttoi(strCurrentDate.Mid(5, 2)), _ttoi(strCurrentDate.Mid(8, 2)), _ttoi(strCurrentDate.Mid(11, 2)), _ttoi(strCurrentDate.Mid(14, 2)), _ttoi(strCurrentDate.Mid(17, 2)));
	return tResult;
}

CTime CLocCommonAPI::GetDBTime( CESL_Mgr* pEslMgr,CESL_DataMgr* pDM)
{
	EFS_BEGIN



	CTime tCurrent = CTime::GetCurrentTime();
	INT ids;
	CString strtime;
	
	bool bIsDelete = FALSE;
	if( pDM == NULL )
	{
		pDM = new CESL_DataMgr();
		bIsDelete = TRUE;
		pDM->CONNECTION_INFO = GetDBConnection(pEslMgr->m_pInfo->MODE);
		pDM->ParseCONNECTION_INFO();
	}
	
	ids = pDM->GetOneValueQuery(_T("SELECT TO_CHAR(SYSDATE, 'YYYY-MM-DD HH24:MI:SS') FROM DUAL"), strtime);	

	if( bIsDelete )
		delete pDM;

	if (strtime.GetLength() != 19) return tCurrent;
	
	CTime result2(_ttoi(strtime.Mid(0, 4)), _ttoi(strtime.Mid(5, 2)), _ttoi(strtime.Mid(8, 2)), _ttoi(strtime.Mid(11, 2)), _ttoi(strtime.Mid(14, 2)), _ttoi(strtime.Mid(17, 2)));
	
	return result2;

	EFS_END

	return NULL;
}

CString CLocCommonAPI::GetDate( CESL_Mgr* pEslMgr , UINT nCtrlID )
{
	EFS_BEGIN

	CString sDate;
	
	CDateTimeCtrl *pDate = ( CDateTimeCtrl * )pEslMgr->GetDlgItem( nCtrlID );

	SYSTEMTIME tTime;
	
	DWORD dwResult = pDate->GetTime( &tTime );

	if (dwResult != GDT_VALID ) return _T("");

	sDate.Format( _T("%04d/%02d/%02d"), tTime.wYear , tTime.wMonth , tTime.wDay  );
	
	return sDate;

	EFS_END
		
	return _T("");
}


CString CLocCommonAPI::GetDBConnection(INT nMode)
{
	EFS_BEGIN

	CString sPath;
	sPath = _T("");

	CReadCfgFile ReadCfg;	
	ReadCfg.ReadCfgFile(sPath);

	return ReadCfg.m_sConnection;
	
	EFS_END

	return _T("");
}


INT CLocCommonAPI::GetLibInfo( CString ArrFieldAttr[], CString ArrResult[], INT nFieldCount , CESL_Mgr* pEslMgr )
{
	EFS_BEGIN

	INT ids;

	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(pEslMgr->m_pInfo->CONNECTION_INFO);
	
	dm.MANAGE_CODE = pEslMgr->m_pInfo->MANAGE_CODE;
	
 	CString strFieldAttr;
 	for (INT idx = 0; idx < nFieldCount; idx++)
 	{
 		if (ArrFieldAttr[idx].IsEmpty()) continue;
 		
 		strFieldAttr += ArrFieldAttr[idx];
 		if (idx != nFieldCount - 1)
 			strFieldAttr += _T(", ");
 	}
 	CString strQuery;
 	strQuery.Format(_T("SELECT %s FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=UDF_MANAGE_CODE"), strFieldAttr );
 	ids = dm.GetValuesQuery(strQuery, ArrResult, nFieldCount);
 	if (ids < 0) return -1;
 
	return 0;

	EFS_END

	return -1;
}


INT CLocCommonAPI::MakeIdxTmpTblInsertFrame( CESL_DataMgr* pDM ,  CString sSpeciesKey , INT nMode , CESL_Mgr* pEslMgr , CKolas2up_MobileApi* pMobileApi , CString sOption )
{
	EFS_BEGIN

 	CString sTmpTblRecKey;
 	pDM->MakeRecKey( sTmpTblRecKey );

	pDM->InitDBFieldData();

	if( pEslMgr->m_pInfo->GetSearchIndexType() == 1 )	
	{
		CString sUpdateSearch[2] =
		{
			_T("ECO$P_MAKE_BO$I(%s);") , _T("ECO$P_MAKE_SE$I(%s);")
		};

		CString sUpdateEcoSearch;
		sUpdateEcoSearch.Format( sUpdateSearch[nMode] , sSpeciesKey );
		if( pMobileApi != NULL )
			pMobileApi->AddFrame( sUpdateEcoSearch );
		else
			pDM->AddFrame( sUpdateEcoSearch );
	}

	return 0;

	EFS_END

	return -1;
}


INT CLocCommonAPI::ChangeSecuriyCivilNo( CESL_Mgr* pESLMgr , CESL_DataMgr* pDM , INT nIndex , CString sAlias , CString sIsUnityLoanService )
{
	EFS_BEGIN

	INT ids;

	CString sCivilNo;
	INT nStartIndex = 0;
	INT nFinishIndex = pDM->GetRowCount();
	
	if( nIndex != -1 )
	{
		nStartIndex = nIndex;
		nFinishIndex = nIndex + 1;
	}

	for( INT i = nStartIndex ; i < nFinishIndex ; i++ )
	{
		ids = pDM->GetCellData( sAlias , i , sCivilNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pESLMgr->m_hWnd , -1 , _T("ChangeSecuriyCivilNo") );

		ChangeSecuriyCivilNo( pESLMgr , sCivilNo , sIsUnityLoanService );

		pDM->SetCellData( sAlias , sCivilNo , i );
	}


	return 0;

	EFS_END

	return -1;
}

INT CLocCommonAPI::ChangeSecuriyCivilNo( CESL_Mgr* pESLMgr , CString &sCivilNo , CString sIsUnityLoanService )
{
	EFS_BEGIN

	CString strBuf , strCivilNoCopy;

	strCivilNoCopy = sCivilNo;

	strCivilNoCopy.Replace( _T("-") , _T("") );
	strBuf = strCivilNoCopy;
	if( 13 <= strCivilNoCopy.GetLength() )
	{
		strBuf = strCivilNoCopy.Mid(0,6) + _T("-") + strCivilNoCopy.Mid(6);
	}

	INT ids = CivilNoCheck( strBuf );
	if(0 > ids) return -1;

	if( 0 == ids )
	{
		if(_T("Y")==sIsUnityLoanService)
		{
			sCivilNo.Format( _T("%s-%s*****") , strBuf.Left(6) , strBuf.Mid(7,2) );
		}
		else
		{
			sCivilNo.Format( _T("%s-%s******") , strBuf.Left(6) , strBuf.Mid(7,1) );
		}
	}

	return 0;

	EFS_END

	return -1;

}

INT CLocCommonAPI::MakeIdxWokingStatusFrame( CESL_DataMgr* pDM ,  CString sSpeciesKey , INT nMode , CESL_Mgr* pEslMgr , CKolas2up_MobileApi* pMobileApi  )
{
	EFS_BEGIN
		
	CString sTableName[2] =
	{
		_T("IDX_BO_TBL") , _T("IDX_SE_TBL")
	};
	
	CString sFunction[2] = 
	{
		_T("testWorkStatus") , _T("testSEWorkStatus")
	};

	CString sRunFunction;
	sRunFunction.Format( _T("%s(%s)") , sFunction[nMode] , sSpeciesKey );

	pDM->AddDBFieldData( _T("WORKING_STATUS") , _T("NUMERIC") , sRunFunction );

#ifdef __K2UP_INDEX_TBL_LOG_INSERT__
	pDM->AddDBFieldData( _T("LAST_WORK"), _T("STRING"), pEslMgr->GetWorkLogMsg( _T("장서관리"), __WFILE__, __LINE__ ), TRUE );
#endif

	if( pMobileApi != NULL )
		pMobileApi->MakeUpdateFrame( sTableName[nMode] , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	else
	{
		pDM->MakeUpdateFrame( sTableName[nMode] , _T("REC_KEY") , _T("NUMERIC") , sSpeciesKey );
	}
	
	pDM->InitDBFieldData();
	
	MakeIdxTmpTblInsertFrame( pDM , sSpeciesKey , nMode , pEslMgr , pMobileApi );

	return 0;
	
	EFS_END
		
	return -1;
}



INT CLocCommonAPI::GetWorkLogMsg( CESL_Mgr* pEslMgr , CString &sLastMsg , CString sFilePath , INT nFileLine )
{
	EFS_BEGIN

	sLastMsg = pEslMgr->GetWorkLogMsg( _T("") , sFilePath , nFileLine  );
	
	return 0;
	
	EFS_END
		
	return -1;
}

INT CLocCommonAPI::MakeRFIDLog( CString funcInfo, CString workInfo, CString fileName, INT line, INT ids, CString expInfo )
{
	EFS_BEGIN

	CFileFind filefind;
	BOOL bFind = filefind.FindFile(_T("..\\cfg\\_log"));
	if ( bFind == FALSE ) return 0;
	
	CTime time = CTime::GetCurrentTime();
	CString output;
	
	output.Format( 
		_T("[ %04d-%02d-%02d . %02d:%02d:%02d ]\r\n")
		_T("RESULT\t : %d\r\nFUNC\t : %s\r\nWORK\t : %s\r\nFILE\t : %s - LINE : %d\r\n\r\n"),
		time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(),
		ids, funcInfo, workInfo, fileName, line );

	CString logFileName;
	logFileName.Format( _T("..\\tunning\\RFID_LOG-%04d-%02d-%02d.txt"), time.GetYear(), time.GetMonth(), time.GetDay() );

	FILE *fp;
	fp = _tfopen( logFileName.GetBuffer( 0 ), _T("a+b") );
	if( !fp )
	{
		AfxMessageBox( _T("RFID Log 기록에 실패하였습니다.") );
		return -1;
	}
	

	fseek(fp, 0, SEEK_END);		
	if( 0 == ftell(fp) )
	{
		/*UNCHANGE*/fputc(0xFF, fp);
		/*UNCHANGE*/fputc(0xFE, fp);
	}
	
	_ftprintf( fp, _T("%s"), output.GetBuffer( 0 )  );
	fclose( fp );
	
	return 0;

	EFS_END
	return -1;
}
 
INT CLocCommonAPI::MakeHPCorpNumList( CComboBox *pCombo )
{
	EFS_BEGIN
		
	CStringList phoneCorpNumList;
	
	CESL_Mgr* pMgr = (CESL_Mgr*)pCombo->GetParent();
	if( pMgr != NULL )
	{
		INT nCodeCount;
		nCodeCount = pMgr->m_pInfo->pCodeMgr->GetCodeCount(_T("이동통신사국번"));
		if( nCodeCount == -2001 )
		{
			MakeDefaultHPNumList(phoneCorpNumList);
		}
		else if( nCodeCount < 0 )   return nCodeCount;
		else
		{
			CString* pdata = new CString[nCodeCount];
			CString* pDesc = new CString[nCodeCount];
			INT nDefaultValue;

			INT ids;
			ids = pMgr->m_pInfo->pCodeMgr->GetCode(_T("이동통신사국번"), pdata, pDesc, nCodeCount, nDefaultValue);
			if(ids == -2001)
			{				
				MakeDefaultHPNumList(phoneCorpNumList);
			}			
			else
			{
				for(int i = 0; i < nCodeCount; i++)
				{
					CString strBuf = pDesc[i];
					if( strBuf.IsEmpty() ||
						0 == strBuf.Compare(_T("적용안함")) )
					{

					}

					phoneCorpNumList.AddTail( pDesc[i] );
				}
				
			}
			delete []pdata;
			delete []pDesc;
		}
	}
	else
	{
		MakeDefaultHPNumList(phoneCorpNumList);
	}

	INT nSetIdx = -1;
	for( INT i = 0; i < phoneCorpNumList.GetCount(); i++ ) {
		pCombo->InsertString( i, phoneCorpNumList.GetAt( phoneCorpNumList.FindIndex( i ) ) );	
		if (  phoneCorpNumList.GetAt( phoneCorpNumList.FindIndex( i ) ) == _T("적용안함")) nSetIdx = i;
	}
	if ( nSetIdx >= 0) pCombo->SetCurSel( nSetIdx );
	else pCombo->SetCurSel( 0 );		
	
	return 0;

	EFS_END
	return -1;
}

INT CLocCommonAPI::MakeHPCorpNumList(CWnd *pWnd)
{
	EFS_BEGIN
		
	CComboBox *pCombo = ( CComboBox * )pWnd;
	return MakeHPCorpNumList( pCombo );

	EFS_END
	return -1;
}

INT CLocCommonAPI::MakeHPCorpNumList(CWnd *pWnd, UINT cID)
{
	EFS_BEGIN
		
	return MakeHPCorpNumList( pWnd->GetDlgItem( cID ) );

	EFS_END
	return -1;
}

VOID CLocCommonAPI::MakeDefaultHPNumList( CStringList &hpNumList )
{
	EFS_BEGIN
		
	const INT cnt = 9;
	CString phoneCorpNum[ cnt ] = {
		_T("적용안함"), _T("011"), _T("016"), _T("017"), _T("018"), _T("019") , _T("010"), _T("0505"), _T("0502") };
	for( INT i = 0; i <cnt; i++ )
		hpNumList.AddTail( phoneCorpNum[ i ] );

	EFS_END
}

INT CLocCommonAPI::GetHPCorpNumList( CStringList &hpNumList )
{
	EFS_BEGIN
	
	return -1;
		
	CStdioFile fp;
	if( fp.Open( HANDPHONENUM_LIST_FILE_PATH, CFile::modeRead | CFile::typeBinary ) )
	{
		TCHAR cUni;
		fp.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fp.SeekToBegin();
		}

		CString sNum;
		while( fp.ReadString( sNum ) )
			hpNumList.AddTail( sNum );
		fp.Close();
	}
	else
		return -1;

	return 0;

	EFS_END
	return -1;
}

INT CLocCommonAPI::SetPhoneNumCel( CComboBox *pCombo, CString hpNum )
{
	EFS_BEGIN

	CString sNum;
	INT cnt = pCombo->GetCount();
	INT defaultCel = pCombo->GetCurSel();
	hpNum.TrimLeft();
	for( INT i = 0; i < cnt; i++ )
	{
		pCombo->SetCurSel( i );
		pCombo->GetWindowText( sNum );
		if( sNum == hpNum.Left( sNum.GetLength() ) )
			return 0;			
	}
	
	pCombo->SetCurSel( defaultCel );
	return 0;

	EFS_END
	return -1;
}

INT CLocCommonAPI::SetPhoneNumCel(CWnd *pWnd, UINT cID, CString hpNum)
{
	EFS_BEGIN

	return SetPhoneNumCel( pWnd->GetDlgItem( cID ), hpNum );

	EFS_END
	return -1;
}

INT CLocCommonAPI::SetPhoneNumCel( CWnd *pWnd, CString hpNum )
{
	EFS_BEGIN

	CComboBox *pCombo = ( CComboBox * )pWnd;
	return SetPhoneNumCel( pCombo, hpNum );

	EFS_END
	return -1;
}

INT CLocCommonAPI::WriteLogSmartID( CESL_Mgr* pEslMgr , CString strDMAlias , CString strMenuID, CString strMenuAlias , CString strAlias)
{
EFS_BEGIN

	INT ids;
	CString strQuery;
	CESL_DataMgr* pDM = pEslMgr->FindDM(strDMAlias);
	if( NULL == pDM ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1010 , _T("WriteLogSmartID") );

	strQuery.Format( _T("UPDATE ESL_TOOLBAR_STATISTICS_TBL ")
					 _T("SET HIT_COUNT=HIT_COUNT+1, LAST_HIT_DATE=SYSDATE ")
					 _T("WHERE MENU_ID='%s' AND MENU_ALIAS='%s' AND ALIAS='%s' AND MANAGE_CODE=UDF_MANAGE_CODE;")
					 , strMenuID, strMenuAlias , strAlias );
	ids = pDM->ExecuteQuery( strQuery , TRUE );
	if( 0 > ids ) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1020 , _T("WriteLogSmartID") );

	return 0;	

EFS_END
return -1;
}

INT CLocCommonAPI::KL_RealNameCertification( CESL_Mgr* pEslMgr , CString strName , CString strCivilNo , CESL_DataMgr &TmpDM )
{
EFS_BEGIN

	CString strCommand = _T("IL_K01_LOC_B01_SERVICE");
	CFrameWriter Writer(pEslMgr->m_pInfo);
	Writer.setCommand(strCommand);
	Writer.addRecord();
	Writer.addElement(_T("이름"), strName);
	Writer.addElement(_T("주민등록번호"), strCivilNo);

	CFrameReader Reader;
	CFrameSender Sender(pEslMgr->m_pInfo);
	INT ids = Sender.CommMiddleWareDaemon(&Writer, &Reader);
	if(0>ids) return ids;

	Reader.makeDM(&TmpDM);
	CString strTemp = Reader.getCommand();
	if(_T("ERROR")==strTemp) return -1010;
	if(1 > TmpDM.GetRowCount()) return -1020;

	TmpDM.GetCellData(_T("처리결과"), 0, strTemp);
	if(_T("OK") != strTemp)
	{
		return -1030;
	}

	return 0;

EFS_END
return -1;
}

INT CLocCommonAPI::KL_CreateCurrentLoanInfo( CESL_Mgr* pEslMgr , CString strUserKey , CString strUserNo )
{
EFS_BEGIN
	
	INT ids;
	CString strFuncName = _T("KL_CreateCurrentLoanInfo");

	CESL_DataMgr TmpDM;
	TmpDM.SetCONNECTION_INFO(_T(""));
	
	CString strQuery;

	strQuery.Format(_T("SELECT REC_KEY,")
							_T("TO_CHAR(CL.LOAN_DATE,'YYYY/MM/DD HH24:MI:SS'),")
							_T("TO_CHAR(CL.RETURN_PLAN_DATE,'YYYY/MM/DD HH24:MI:SS'),")	
							_T("TO_CHAR(CL.DELAY_RETURN_DATE,'YYYY/MM/DD HH24:MI:SS'),")
							_T("CL.STATUS,")				
							_T("CL.REG_NO,")				
							_T("CL.TITLE,")					
							_T("CL.AUTHOR,")				
							_T("CL.PUBLISHER,")				
							_T("CL.CALL_NO,")				
							_T("(SELECT DECODE(BB.BOOK_APPENDIX_FLAG,'A',NULL,'B',CONCAT(CONCAT(BB.VOL,'/'),BB.VOL_TITLE)) FROM BO_BOOK_TBL BB WHERE BB.REC_KEY=CL.BOOK_KEY) AS VOL,")
							_T("(SELECT NVL(BOOK_APPENDIX_FLAG,'B') FROM BO_BOOK_TBL WHERE REC_KEY=CL.BOOK_KEY) AS A,")
							_T("(SELECT LIB_CODE FROM MN_LIBINFO2_TBL WHERE MANAGE_CODE=CL.MANAGE_CODE) AS LIB_CODE ")
					_T("FROM LS_WORK_T01 CL ")
					_T("WHERE CL.USER_KEY=%s ")					
					_T("AND (SELECT VALUE_1 FROM ESL_MANAGE_TBL WHERE VALUE_NAME = '통합도서서비스' AND MANAGE_CODE = (SELECT MANAGE_CODE FROM BO_BOOK_TBL WHERE REC_KEY=CL.BOOK_KEY)) = 'Y' ")					
					_T("AND L_DEVICE != 'KOLASIII-이동도서관' ")					
					_T("AND CL.STATUS IN ('0','2','L','D') ")
					, strUserKey ); 	
	ids = TmpDM.RestructDataManager(strQuery);
	if(0>ids) ERROR_MSG_RETURN_ERROR_CODE( pEslMgr->m_hWnd , -1010 , strFuncName );
	if(0==TmpDM.GetRowCount()) return 0;
	
	CStringArray astrFrames;
	const int nColCnt = 13;
	CString strResult;
	CString strCLFieldAlias[nColCnt] = 
	{	_T("로컬KEY"),	_T("대출일"),	_T("반납예정일"),	_T("반납연기일"),	_T("상태"),		
		_T("등록번호"),	_T("서명"),		_T("저자"),			_T("발행자"),		_T("청구기호"), 
		_T("편권차"),	_T("부록여부"),	_T("대출도서관")
	};

	CString strCommand = _T("IL_K03_LOC_B01_SERVICE");
	CFrameWriter Writer(pEslMgr->m_pInfo);
	Writer.setCommand(strCommand);

	INT nRowCnt = TmpDM.GetRowCount();
	for(INT i=0; i<nRowCnt; i++)
	{
		Writer.clear();
		Writer.setCommand(strCommand);
		Writer.addRecord();
		Writer.addElement(_T("통합대출자번호"), strUserNo);

		for(INT j=0; j<nColCnt; j++)
		{
			ids = TmpDM.GetCellData(i, j, strResult);
			
			if(strCLFieldAlias[j].Compare(_T("상태")) == 0 && (strResult.Compare(_T("L")) == 0 || strResult.Compare(_T("D")) == 0)){
				if(strResult.Compare(_T("L")) == 0)
					strResult = _T("0");
				else
					strResult = _T("2");

				Writer.addElement(_T("상태"), strResult);
				Writer.addElement(_T("지역내상호대차상태"), _T("Y"));
			}else{
				Writer.addElement(strCLFieldAlias[j], strResult);
			}
		}
		astrFrames.Add(Writer.getFrame());
 	}

	CFrameSender Sender(pEslMgr->m_pInfo);
	ids = Sender.SendTempTbl(astrFrames, pEslMgr->GetWorkLogMsg( _T("통합회원대출현황정보생성"), __WFILE__, __LINE__ ));
	if(0>ids) 
	{
		CString strMsg;
		strMsg.Format(_T("대출현황정보생성(3) ERROR CODE : %d"),ids);
		CESL_DataMgr TmpDM;
		TmpDM.QueryLog(strMsg.GetBuffer(0));
		return ids;
	}
	
	return 0;

EFS_END
return -1;
}

INT CLocCommonAPI::ChangeSecuriyUserNo( CESL_Mgr* pESLMgr , CESL_DataMgr* pDM , INT nIndex , CString sAlias , CString sIsUnityLoanService )
{
	EFS_BEGIN

	INT ids;

	CString sUserNo;
	INT nStartIndex = 0;
	INT nFinishIndex = pDM->GetRowCount();
	
	if( nIndex != -1 )
	{
		nStartIndex = nIndex;
		nFinishIndex = nIndex + 1;
	}

	for( INT i = nStartIndex ; i < nFinishIndex ; i++ )
	{
		ids = pDM->GetCellData( sAlias , i , sUserNo );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE( pESLMgr->m_hWnd , -1 , _T("ChangeSecuriyCivilNo") );

		ChangeSecuriyUserNo( pESLMgr , sUserNo , sIsUnityLoanService );

		pDM->SetCellData( sAlias , sUserNo , i );
		
	}

	return 0;

	EFS_END

	return -1;
}

INT CLocCommonAPI::ChangeSecuriyUserNo( CESL_Mgr* pESLMgr , CString &sUserNo , CString sIsUnityLoanService )
{
	EFS_BEGIN

	CString strBuf;
	strBuf = sUserNo;
	if( 13 <= sUserNo.GetLength() )
	{
		strBuf = sUserNo.Mid(0,6) + _T("-") + sUserNo.Mid(6);
	}

	INT ids = CivilNoCheck( strBuf );
	if(0>ids) return -1;
	if(0==ids)
	{
		if(_T("Y")==sIsUnityLoanService)
		{
			sUserNo.Format( _T("%s%s*****") , strBuf.Left(6) , strBuf.Mid(7,2) );
		}
		else
		{
			sUserNo.Format( _T("%s%s******") , strBuf.Left(6) , strBuf.Mid(7,1) );
		}
	}

	return 0;

	EFS_END

	return -1;
}

VOID CLocCommonAPI::SetMoneyFormat(CString &sMoney)
{
	sMoney.Replace( _T(","), _T("") );

	CString strBuf = sMoney;
	strBuf.Replace( _T(","), _T("") );

	INT nSize = (strBuf.GetLength() * 3) + 100;
	_TCHAR *pszFormattedNumber = new _TCHAR [nSize];

	NUMBERFMT nFmt = { 0, 0, 3, _T("."), _T(","), 0 };

	::GetNumberFormat( NULL, NULL, strBuf, &nFmt, pszFormattedNumber, nSize - 1 );

	sMoney = pszFormattedNumber;

	if ( pszFormattedNumber )
		delete [] pszFormattedNumber;
}

CString CLocCommonAPI::StringToSHA256(CString strInputData)
{
	if(strInputData.IsEmpty()) return _T("");

	CString strOutputData = _T("");
	CHash hashObj;
	hashObj.SetHashFormat(UPPERCASE_NOSPACES);
	hashObj.SetHashOperation(STRING_HASH);
	hashObj.SetHashAlgorithm(SHA2);
	hashObj.SetSHA2Strength(256);
	char ctmp[256];
	WideCharToMultiByte(CP_ACP, 0, (LPCTSTR)strInputData, -1, ctmp, 256, NULL, NULL);
	hashObj.SetHashString(ctmp);
 	WCHAR ctmp2[256];
 	MultiByteToWideChar(CP_ACP, 0, (LPSTR)(LPCTSTR)hashObj.DoHash(), -1, ctmp2, 256);
 	strOutputData = ctmp2;
	return strOutputData;
}

INT CLocCommonAPI::CheckUpperEnglish(CString strPassword) 
{
	char cEng[26];
	INT ids;
	for(INT i = 0; i < 26; i++)
	{
		cEng[i] = 'A' + i;
		ids = strPassword.Find(cEng[i]);
		if(0 <= ids)
		{
			return 1;
		}
	}
	return 0;
}

INT CLocCommonAPI::CheckLowerEnglish(CString strPassword) 
{
	char cEng[26];
	INT ids;
	for(INT i = 0; i < 26; i++)
	{
		cEng[i] = 'a' + i;
		ids = strPassword.Find(cEng[i]);
		if(0 <= ids)
		{
			return 1;
		}
	}
	return 0;
}

INT CLocCommonAPI::CheckNumber(CString strPassword) 
{
	char cNum[10];
	INT ids;
	for(INT i = 0; i < 10; i++)
	{
		cNum[i] = '0' + i;
		ids = strPassword.Find(cNum[i]);
		if(0 <= ids)
		{
			return 1;
		}
	}
	return 0;
}

INT CLocCommonAPI::CheckSpecial(CString strPassword) 
{
	char cNum1[15];
	char cNum2[7];
	char cNum3[6];
	char cNum4[4];
	INT ids;

	for(INT i = 0; i < 15; i++)
	{
		cNum1[i] = '!' + i;
		ids = strPassword.Find(cNum1[i]);
		if(0 <= ids)
		{
			return 1;
		}
	}

	for(i = 0; i < 7; i++)
	{
		cNum2[i] = ':' + i;
		ids = strPassword.Find(cNum2[i]);
		if(0 <= ids)
		{
			return 1;
		}
	}
	
	for(i = 0; i < 6; i++)
	{
		cNum3[i] = '[' + i;
		ids = strPassword.Find(cNum3[i]);
		if(0 <= ids)
		{
			return 1;
		}
	}

	for(i = 0; i < 4; i++)
	{
		cNum4[i] = '{' + i;
		ids = strPassword.Find(cNum4[i]);
		if(0 <= ids)
		{
			return 1;
		}
	}

	return 0;	
}

CString CLocCommonAPI::UrlDecode(LPCTSTR szSource)
{
  int n, nMax = _tcslen(szSource);
  WORD ch;
  CString sFinal, sTemp;
  BYTE z, y, x, w, v, u;
  
  for (n = 0; n < nMax; ++n)
  {
    ch = (WORD)szSource[n];

    if (ch != _T('='))
    {
      sFinal += (TCHAR)ch;
      continue;
    }

    if (n >= nMax - 2) break;
    z = MakeByte(szSource[n+1], szSource[n+2]);

    if (z < 127)
    {
      sFinal += (TCHAR)z;
      n = n + 2;
    }
    else if (z >= 192 && z <= 223)
    {
      if (n >= nMax - 5) break;
      y = MakeByte(szSource[n+4], szSource[n+5]);
      sFinal += (TCHAR)( (z-192)*64 + (y-128) );
      n = n + 5;
    }
    else if (z >= 224 && z <= 239)
    {
      if (n >= nMax - 8) break;
      y = MakeByte(szSource[n+4], szSource[n+5]);
      x = MakeByte(szSource[n+7], szSource[n+8]);
      sFinal += (TCHAR)( (z-224)*4096 + (y-128)*64 + (x-128) );
      n = n + 8;
    }
    else if (z >= 240 && z <= 247)
    {
      if (n >= nMax - 11) break;
      y = MakeByte(szSource[n+4], szSource[n+5]);
      x = MakeByte(szSource[n+7], szSource[n+8]);
      w = MakeByte(szSource[n+10], szSource[n+11]);
      sFinal += (TCHAR)( (z-240)*262144 + (y-128)*4096 + 
             (x-128)*64 + (w-128) );
      n = n + 11;
    }
    else if (z >= 248 && z <= 251)
    {
      if (n >= nMax - 14) break;
      y = MakeByte(szSource[n+4], szSource[n+5]);
      x = MakeByte(szSource[n+7], szSource[n+8]);
      w = MakeByte(szSource[n+10], szSource[n+11]);
      v = MakeByte(szSource[n+13], szSource[n+14]);
      sFinal += (TCHAR)( (z-248)*16777216 + (y-128)*262144 + 
           (x-128)*4096 + (w-128)*64 + (v-128) );
      n = n + 14;
    }
    else if (z >= 252 && z <= 253)
    {
      if (n >= nMax - 17) break;
      y = MakeByte(szSource[n+4], szSource[n+5]);
      x = MakeByte(szSource[n+7], szSource[n+8]);
      w = MakeByte(szSource[n+10], szSource[n+11]);
      v = MakeByte(szSource[n+13], szSource[n+14]);
      u = MakeByte(szSource[n+16], szSource[n+17]);
      sFinal += (TCHAR)( (z-252)*1073741824 + (y-128)*16777216 + 
          (x-128)*262144 + (w-128)*4096 + (v-128)*64 + (u-128) );
      n = n + 17;
    }
  }

  return sFinal;
}


BYTE CLocCommonAPI::MakeByte(TCHAR ch1, TCHAR ch2)
{
  BYTE bt1 = 0, bt2 = 0;

  switch (ch2)
  {
    case _T('0'):
      bt2 = 0x00;
      break;
    case _T('1'):
      bt2 = 0x01;
      break;
    case _T('2'):
      bt2 = 0x02;
      break;
    case _T('3'):
      bt2 = 0x03;
      break;
    case _T('4'):
      bt2 = 0x04;
      break;
    case _T('5'):
      bt2 = 0x05;
      break;
    case _T('6'):
      bt2 = 0x06;
      break;
    case _T('7'):
      bt2 = 0x07;
      break;
    case _T('8'):
      bt2 = 0x08;
      break;
    case _T('9'):
      bt2 = 0x09;
      break;
    case _T('A'):
      bt2 = 0x0A;
      break;
    case _T('B'):
      bt2 = 0x0B;
      break;
    case _T('C'):
      bt2 = 0x0C;
      break;
    case _T('D'):
      bt2 = 0x0D;
      break;
    case _T('E'):
      bt2 = 0x0E;
      break;
    case _T('F'):
      bt2 = 0x0F;
      break;
  }

  switch (ch1)
  {
    case _T('0'):
      bt1 = 0x00;
      break;
    case _T('1'):
      bt1 = 0x10;
      break;
    case _T('2'):
      bt1 = 0x20;
      break;
    case _T('3'):
      bt1 = 0x30;
      break;
    case _T('4'):
      bt1 = 0x40;
      break;
    case _T('5'):
      bt1 = 0x50;
      break;
    case _T('6'):
      bt1 = 0x60;
      break;
    case _T('7'):
      bt1 = 0x70;
      break;
    case _T('8'):
      bt1 = 0x80;
      break;
    case _T('9'):
      bt1 = 0x90;
      break;
    case _T('A'):
      bt1 = 0xA0;
      break;
    case _T('B'):
      bt1 = 0xB0;
      break;
    case _T('C'):
      bt1 = 0xC0;
      break;
    case _T('D'):
      bt1 = 0xD0;
      break;
    case _T('E'):
      bt1 = 0xE0;
      break;
    case _T('F'):
      bt1 = 0xF0;
      break;
  }
  BYTE btFinal = bt2 | bt1;
  return  btFinal;  
}

CString CLocCommonAPI::UrlEncode(LPCTSTR szSource)
{
	WORD ch;
	BYTE bt1, bt2, bt3, bt4, bt5, bt6;
	int n, nMax = _tcslen(szSource);
	CString sFinal, sTemp;
	CString strPase = _T("%");
	for (n = 0; n < nMax; ++n)
	{
		ch = (WORD)szSource[n];
		if (ch == _T('='))
		{
			sTemp.Format(_T("%s%02X"), strPase, ch);
			sFinal += sTemp;
		}
		else if (ch < 128)
		{
			sFinal += szSource[n];
		}
		else if (ch <= 2047)
		{
			 bt1 = (BYTE)(192 + (ch / 64));
			 bt2 = (BYTE)(128 + (ch % 64));
			sTemp.Format(_T("%s%02X%s%02X"), strPase, bt1, strPase, bt2);
			sFinal += sTemp;
		}
		else if (ch <= 65535)
		{
			 bt1 = (BYTE)(224 + (ch / 4096));
			 bt2 = (BYTE)(128 + ((ch / 64) % 64));
			 bt3 = (BYTE)(128 + (ch % 64));
			sTemp.Format(_T("%s%02X%s%02X%s%02X"), strPase, bt1, strPase, bt2, strPase, bt3);		
			sFinal += sTemp;
		}
		else if (ch <= 2097151)
		{
			 bt1 = (BYTE)(240 + (ch / 262144));
			 bt2 = (BYTE)(128 + ((ch / 4096) % 64));
			 bt3 = (BYTE)(128 + ((ch / 64) % 64));
			 bt4 = (BYTE)(128 + (ch % 64));
			sTemp.Format(_T("%s%02X%s%02X%s%02X%s%02X"), strPase, bt1, strPase, bt2, strPase, bt3, strPase, bt4);
			sFinal += sTemp;
		}
		else if (ch <=67108863)
		{
			bt1 = (BYTE)(248 + (ch / 16777216));
			bt2 = (BYTE)(128 + ((ch / 262144) % 64));
			bt3 = (BYTE)(128 + ((ch / 4096) % 64));
			bt4 = (BYTE)(128 + ((ch / 64) % 64));
			bt5 = (BYTE)(128 + (ch % 64));
			sTemp.Format(_T("%s%02X%s%02X%s%02X%s%02X%s%02X"), strPase, bt1, strPase, bt2, strPase, bt3, strPase, bt4, strPase, bt5);
			sFinal += sTemp;
		}
		else if (ch <=2147483647)
		{
			 bt1 = (BYTE)(252 + (ch / 1073741824));
			 bt2 = (BYTE)(128 + ((ch / 16777216) % 64));
			 bt3 = (BYTE)(128 + ((ch / 262144) % 64));
			 bt4 = (BYTE)(128 + ((ch / 4096) % 64));
			 bt5 = (BYTE)(128 + ((ch / 64) % 64));
			 bt6 = (BYTE)(128 + (ch % 64));
			sTemp.Format(_T("%s%02X%s%02X%s%02X%s%02X%s%02X%s%02X"), strPase, bt1, strPase, bt2, strPase, bt3, strPase, bt4, strPase, bt5, strPase, bt6);
			sFinal += sTemp;
		}
	}
	return sFinal;
}

VOID CLocCommonAPI::MakeISBNSearchQuery(CString strINISBN, CString strSpeciesTblAlias, CString& strWhere, INT m_nMode)
{
	CString strAddedISBN, strToISBN, strISBNView;
	INT iResult;
	CISBNChecker isbnChecker;
	strToISBN = _T("");
	if(m_nMode == 2){
		strISBNView = _T("ECO$V_SESTCODE$I");
	}else{
		strISBNView = _T("ECO$V_BOSTCODE$I");
	}

	switch( strINISBN.GetLength() ) {		
	case 10:
		strAddedISBN = _T("978") + strINISBN;
		isbnChecker.ConvertISBN(strAddedISBN.GetBuffer(0), strToISBN.GetBuffer(MAX_PATH), &iResult );
		strToISBN.ReleaseBuffer();
		strToISBN.Remove(_T('-'));
		strWhere.Format(_T("%s IN ( ")
			_T("SELECT DISTINCT ECO_KEY FROM %s ")
			_T("WHERE (TOKEN='%s' OR TOKEN='%s') AND TOKEN_TYPE='F' ) "), strSpeciesTblAlias, strISBNView, strINISBN, strToISBN);		
		break;		
	case 13:
		if( _T("978") == strINISBN.Left(3) ) {
			strAddedISBN = strINISBN.Mid(3);
			isbnChecker.ConvertISBN(strAddedISBN.GetBuffer(0), strToISBN.GetBuffer(MAX_PATH), &iResult );
			strToISBN.ReleaseBuffer();
			strToISBN.Remove(_T('-'));	
			strWhere.Format(_T("%s IN ( ")
				_T("SELECT DISTINCT ECO_KEY FROM %s ")
				_T("WHERE (TOKEN='%s' OR TOKEN='%s') AND TOKEN_TYPE='F' ) "), strSpeciesTblAlias, strISBNView, strINISBN, strToISBN);			
		}
		else {
			strWhere.Format(_T("%s IN ( ")
				_T("SELECT DISTINCT ECO_KEY FROM %s ")
				_T("WHERE TOKEN='%s' AND TOKEN_TYPE='F' ) "), strSpeciesTblAlias, strISBNView, strINISBN );			
		}
		break;
	}
}
