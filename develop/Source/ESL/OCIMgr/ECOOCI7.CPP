
#include "stdafx.h"
#include "ECOOCI7.h"
#include "OCIMgrEx.h"
#include "except.h"
#include "io.h"
#include "fcntl.h"

/* Gloal Variable Defind */
Lda_Def lda;
Cda_Def cda;
ub1 hda[HDA_SIZE];

struct describe desc[MAX_SELECT_LIST_SIZE];
struct define def [MAX_SELECT_LIST_SIZE];

INT g_nCdaCount = 0;

/* Static Variable Defind */
static BOOL g_bConnect = FALSE;


//--------------------------------------------------------------------------
// 함수명 : OCIError
// 기  능 : Cursor의 Error Message를 Log File에 출력한다
//          Parameter :
//            cda : [I N] Cursor 정보 포인터
//--------------------------------------------------------------------------
VOID OCIError(Cda_Def* cda)
{
ECO_EXCEPTION_BEGIN

text msg[512];

MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> -- ORACLE ERROR --"));

oerhms(&lda, cda->rc, msg, (sword)sizeof(msg));

	DisconnectDB();

MESSAGE(M$WARN, MSG_ARGMENT, _T("%s"), msg);
MESSAGE(M$WARN, MSG_ARGMENT, _T("Processing OCI function %s"), oci_func_tab[cda->fc]);

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : ConnectDB
// 기  능 : Oracle에 Login하고 Cursor 정보를 받는다
//          DB 작업을 하기 위해서는 꼭 이 Function이 선생되어야 한다
//          Parameter :
//            pszUserName : [I N] 사용자ID
//            pszPassword : [I N] 사용자Password
//--------------------------------------------------------------------------
INT ConnectDB(UCHAR* pszUserName, UCHAR* pszPassword)
{
ECO_EXCEPTION_BEGIN

static text username[128];
static text password[128];

	// ID와 Password 변경을 확인한다. 변경되었다면 기존의 연결을 종료하고
	// 새 ID와 Password로 재연결을 해야한다.
if (ECOStrCmpi((TCHAR*) username, (TCHAR*) pszUserName) != 0 ||
ECOStrCmpi((TCHAR*) password, (TCHAR*) pszPassword) != 0)
	{
if (g_bConnect == TRUE) DisconnectDB();
}
	else
	{
if (g_bConnect == TRUE)
		{
return 0;
}
}

	// 로그인한다 (LDA 정보가 세팅됨)
if (orlon(&lda, hda, pszUserName, -1, pszPassword, -1, -1))
	{
		AfxMessageBox(_T("DBWAYI YEONGYEOLI IRUEOJIJI ANHASSSEUBNIDA.\n\nJAMSI HUE DASI SIDOHASIBSIO."), MB_OK|MB_ICONINFORMATION);
MESSAGE(M$WARN, MSG_ARGMENT, _T("Cannot connect as %s."), pszUserName);
return -1;
}

	// Cursor 정보를 세팅한다
if (oopen(&cda, &lda, (text*) 0, -1, -1, (text*) 0, -1))
{
MESSAGE(M$WARN, MSG_ARGMENT, _T("Error opening cursor. Exiting...\n"));
ologof(&lda);
return -2;
}

ECOStrCpy((TCHAR*) username, (TCHAR*) pszUserName);
ECOStrCpy((TCHAR*) password, (TCHAR*) pszPassword);

g_bConnect = TRUE;

MESSAGE(M$INFORM, MSG_ARGMENT, _T("Oracle Connection As %s."), pszUserName);

return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : DisconnectDB
// 기  능 : Oracle과의 연결을 종료한다
//--------------------------------------------------------------------------
INT DisconnectDB()
{
ECO_EXCEPTION_BEGIN

if (g_bConnect == TRUE)
	{
		if (orol (&lda)) MESSAGE(M$WARN, MSG_ARGMENT, _T("Error rollback!") );
if (oclose(&cda)) MESSAGE(M$WARN, MSG_ARGMENT, _T("Error closing cursor!"));
if (ologof(&lda)) MESSAGE(M$WARN, MSG_ARGMENT, _T("Error logging off!") );
g_bConnect = FALSE;
}

return 0;

ECO_EXCEPTION_END
}

//---------------------------------------------------------------------------------------------------
// S E L E C T / I N S E R T / U P D A T E / D E L E T E / E X E C U T E  S Q L
//---------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 함수명 : SelectProc
// 기  능 : SELECT 문을 수행하여 결과 값을 리턴한다
//          Parameter :
//            pszUserID    : [I N] 사용자 ID
//            pszPasswd    : [I N] 사용자 Password
//            pszSQL       : [I N] SELECT SQL
//            nRowCount    : [OUT] 결과 Row Count
//            nColumnCount : [OUT] 결과 Column Count
//            pOutData     : [OUT] 결과 Data
//--------------------------------------------------------------------------
INT SelectProc(
				TCHAR* pszUserID ,	// [IN] SAYONGJAID
TCHAR* pszPasswd ,	// [IN] SAYONGJAPASSWD
TCHAR* pszSQL ,	// [IN] SELECT SQL
INT* nRowCount ,	// [OUT] GYEOLGWA Row Count
INT* nColumnCount ,	// [OUT] GYEOLGWA Column Count
DB_DATA_ARRAY_TYPE** pOutData			// [OUT] GYEOLGWA Data
)
{
ECO_EXCEPTION_BEGIN

INT ids;

	TCHAR szFormat[256];

	sword col, deflen, deftyp, i;

	// DB 연결 확인
	ids = ConnectDB((UCHAR*) pszUserID, (UCHAR*) pszPasswd);
	if (ids)
	{
		return -100;
	}

	// SQL 로그를 남긴다
LoggingProcess(LOG_SQL_FILE_NAME, pszSQL);

	// SQL 무결성 확인
if (oparse(&cda, (text*) pszSQL, (sb4) -1, (sword) PARSE_NO_DEFER, (ub4) PARSE_V7_LNG))
	{
OCIError(&cda);
return -1;
}

	(*nRowCount ) = 0;
(*nColumnCount) = 0;

	// Column Type을 Define 한다
for (col = 0; col < MAX_SELECT_LIST_SIZE; col++)
	{
if (odescr(&cda, col + 1,
&desc[col].dbsize, &desc[col].dbtype ,
&desc[col].buf[0], &desc[col].buflen ,
&desc[col].dsize , &desc[col].precision ,
&desc[col].scale , &desc[col].nullok))
		{
if (cda.rc == VAR_NOT_IN_LIST) break;
else
			{
OCIError(&cda);
return -2;
}
}

		// TYPE에 관계없이 STRING TYPE으로 DB에서 DATA를 가져온다 2000/06/03 수정  by 이종혁

desc[col].defptr = NULL;

		switch (desc[col].dbtype)
		{
		case NUMBER_TYPE :
			desc[col].dbsize = NUMBER_TYPE_LEN;
			break;
		case DATE_TYPE :
			desc[col].dbsize = DATE_TYPE_LEN;
			break;
		case 8:	// LONG_TYPE
			desc[col].dbsize = LONG_TYPE_LEN;
			break;
		case ROWID_TYPE :
			desc[col].dbsize = ROWID_TYPE_LEN;
			break;
		}

deflen = desc[col].dbsize +sizeof(TCHAR);
		deftyp = STRING_TYPE;

desc[col].defptr = (ub1*) malloc(deflen);
if (desc[col].defptr == NULL)
		{
for (i = 0; i < col; i++)
			{
if (desc[i].defptr != NULL)
				{
free(desc[i].defptr); desc[i].defptr = NULL;
}
}
return -3;
}

		wmemset(desc[col].defptr, 0x00, deflen);

if (odefin(&cda, col + 1, desc[col].defptr, deflen, deftyp,
-1, &def[col].indp, (text *) 0, -1, -1,
&def[col].col_retlen, &def[col].col_retcode))
		{
OCIError(&cda);
for (i = 0; i < col; i++)
			{
if (desc[i].defptr != NULL)
				{
free(desc[i].defptr); desc[i].defptr = NULL;
}
}
return -4;
}
}

(*nColumnCount) = col;

pOutData[0] = (DB_DATA_ARRAY_TYPE*) ECOMalloc(sizeof(DB_DATA_ARRAY_TYPE) * (*nColumnCount));
	if (pOutData == NULL)
	{
for (i = 0; i < (*nColumnCount); i++)
		{
if (desc[i].defptr != NULL) ECOFree(desc[i].defptr);
}
		return -5;
	}

	wmemset(pOutData[0], 0x00, sizeof(DB_DATA_ARRAY_TYPE) * (*nColumnCount));

	// SQL 수행한다
if (oexec(&cda))
	{
OCIError(&cda);
for (i = 0; i < (*nColumnCount); i++)
		{
if (desc[i].defptr != NULL) ECOFree(desc[i].defptr);
}
return -6;
}

	// 결과를 받는다
while (TRUE)
	{
if (ofetch(&cda))
		{
if (cda.rc == NO_DATA_FOUND ) break;
if (cda.rc != NULL_VALUE_RETURNED)
			{
OCIError(&cda);
				// Inserted by kck 2000.5.6
for (i = 0; i < (*nColumnCount); i++)
				{
if (desc[i].defptr != NULL) ECOFree(desc[i].defptr);
}
				return -7;
}
}
		else
		{

			if (g_nFetchCount == (*nRowCount))
			{
				CString sMessage;
				sMessage.Format(_T("JARYOREUL GAJIGO OL SU ISSNEUN GEONSUGA %dEURO JEHANDOEEO ISSSEUBNIDA.\n\nDDARASEO JOHOEDOEN JARYONEUN %d BODA MANHEUL SU ISSSEUBNIDA."), g_nFetchCount, g_nFetchCount);
				AfxMessageBox(sMessage, MB_OK|MB_ICONINFORMATION);
				break;
			}

(*nRowCount) += 1;

for (col = 0; col < (*nColumnCount); col++)
			{
				// 저장할 변수 공간을 확보한다
			 pOutData[0][col].pDBDataUnion = (DB_DATA_UNION*) ECORealloc(pOutData[0][col].pDBDataUnion, sizeof(DB_DATA_UNION) * (*nRowCount));
			 if (pOutData[0][col].pDBDataUnion == NULL)
				{
				 FreeDBDataArrayType(pOutData[0], (*nColumnCount));
for (i = 0; i < (*nColumnCount); i++)
					{
if (desc[i].defptr != NULL) ECOFree(desc[i].defptr);
}
				 return -8;
			 }

				//
				if ((*nRowCount) == 1)
				{
					switch (desc[col].dbtype)
					{
					case FLOAT_TYPE :
					case INT_TYPE :
						pOutData[0][col].nDataType = NUMERIC;
						break;
					case DATE_TYPE :
						pOutData[0][col].nDataType = DATE;
						break;
					default :
						pOutData[0][col].nDataType = STRING;
					}
				}

				desc[col].defptr[desc[col].dbsize] = '\0';
str_rtrim((TCHAR *)desc[col].defptr, (TCHAR *)desc[col].defptr, ' ');

				// NUMBER 타입 중 소수점이 들어가는 것을 처리 (2000/06/19 by 이종혁)

				if (desc[col].scale > 0)
				{
					_stprintf(szFormat, _T("%%-%d.%df"), desc[col].precision, desc[col].scale);
					_stprintf((TCHAR*) desc[col].defptr, szFormat, atof((TCHAR*) desc[col].defptr));
					str_rtrim((TCHAR*) desc[col].defptr, (TCHAR*) desc[col].defptr, ' ');
				}

pOutData[0][col].pDBDataUnion[(*nRowCount) - 1].pStringData = (TCHAR*) ECOCopyMalloc(_tcsclen((TCHAR*) desc[col].defptr) +sizeof(TCHAR), desc[col].defptr);
if (pOutData[0][col].pDBDataUnion[(*nRowCount) - 1].pStringData == NULL)
				{
				 FreeDBDataArrayType(pOutData[0], (*nColumnCount));
for (i = 0; i < (*nColumnCount); i++)
					{
if (desc[i].defptr != NULL) ECOFree(desc[i].defptr);
}
return -9;
}

				wmemset(desc[col].defptr, 0x00, desc[col].dbsize);

pOutData[0][col].nRowCount++;
}
}
}

for (i = 0; i < (*nColumnCount); i++)
	{
if (desc[i].defptr != NULL) ECOFree(desc[i].defptr);
}

return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : SelectProcForOneRecord
// 기  능 : 새로운 커서 정보로 SELECT 문을 수행한다
//          Parameter :
//            lpOraHandle  : [I N] Oracle 연결 정보
//            pszUserID    : [I N] 사용자 ID
//            pszPasswd    : [I N] 사용자 Password
//            pszSQL       : [I N] SELECT SQL
//--------------------------------------------------------------------------
INT SelectProcForOneRecord(
ORA_HANDLE* lpOraHandle ,
							TCHAR* pszUserID ,	// [IN] SAYONGJAID
							TCHAR* pszPasswd ,	// [IN] SAYONGJAPASSWD
							TCHAR* szSQL 	// [IN] SELECT SQL
)
{
ECO_EXCEPTION_BEGIN

	INT ids;
sword col, deflen, deftyp, i;

    // DB 연결 확인
	ids = ConnectDB((UCHAR*) pszUserID, (UCHAR*) pszPasswd);
	if (ids)
	{
		return -100;
	}

	// 새로운 커서를 받는다.
ids = ECO_oopen(&(lpOraHandle->cda), &lda, (text*) 0, -1, -1, (text*) 0, -1);
	if (ids)
	{
OCIError(&(lpOraHandle->cda));
MESSAGE(M$WARN, MSG_ARGMENT, _T("Error opening cursor. Exiting...\n"));
return -1;
}

lpOraHandle->nColumnCount = 0;

	// SQL을 검사한다
if (ECO_oparse(&(lpOraHandle->cda), (text*) szSQL, (sb4) -1, (sword) PARSE_NO_DEFER, (ub4) PARSE_V7_LNG))
	{
OCIError(&(lpOraHandle->cda));
return -2;
}


	// Column Type 을 Define한다
for (col = 0; col < MAX_SELECT_LIST_SIZE; col++)
	{
if (ECO_odescr(&(lpOraHandle->cda), col + 1,
&lpOraHandle->desc[col].dbsize, &lpOraHandle->desc[col].dbtype ,
&lpOraHandle->desc[col].buf[0], &lpOraHandle->desc[col].buflen ,
&lpOraHandle->desc[col].dsize , &lpOraHandle->desc[col].precision ,
&lpOraHandle->desc[col].scale , &lpOraHandle->desc[col].nullok))
		{
if (lpOraHandle->cda.rc == VAR_NOT_IN_LIST) break;
else
			{
OCIError(&(lpOraHandle->cda));
return -3;
}
}

		// TYPE에 관계없이 STRING TYPE으로 DB에서 DATA를 가져온다 2000/06/03 수정  by 이종혁

lpOraHandle->desc[col].defptr = NULL;

		switch (lpOraHandle->desc[col].dbtype)
		{
		case NUMBER_TYPE :
			lpOraHandle->desc[col].dbsize = NUMBER_TYPE_LEN;
			break;
		case DATE_TYPE :
			lpOraHandle->desc[col].dbsize = DATE_TYPE_LEN;
			break;
		case 8:	// LONG_TYPE
			lpOraHandle->desc[col].dbsize = LONG_TYPE_LEN;
			break;
		case ROWID_TYPE :
			lpOraHandle->desc[col].dbsize = ROWID_TYPE_LEN;
			break;
		}

deflen = lpOraHandle->desc[col].dbsize + 1;
		deftyp = STRING_TYPE;

lpOraHandle->desc[col].defptr = (ub1*) ECOMalloc(deflen);
if (lpOraHandle->desc[col].defptr == NULL)
		{
for (i = 0; i < col; i++)
			{
				if (lpOraHandle->desc[i].defptr != NULL) ECOFree(lpOraHandle->desc[i].defptr);
}
return -4;
}

		wmemset(lpOraHandle->desc[col].defptr, 0x00, deflen);

if (ECO_odefin(&(lpOraHandle->cda) , col + 1, lpOraHandle->desc[col].defptr, deflen, deftyp,
-1, &lpOraHandle->def[col].indp, (text *) 0, -1, -1,
&lpOraHandle->def[col].col_retlen, &lpOraHandle->def[col].col_retcode))
		{
OCIError(&(lpOraHandle->cda));
for (i = 0; i < col; i++)
			{
if (lpOraHandle->desc[i].dbtype != FLOAT_TYPE && lpOraHandle->desc[i].dbtype != INT_TYPE)
				{
if (lpOraHandle->desc[i].defptr != NULL) ECOFree(lpOraHandle->desc[i].defptr);
}
}
return -5;
}
}

lpOraHandle->nColumnCount = col;

	// SQL을 수행한다
if (ECO_oexec(&(lpOraHandle->cda)))
	{
OCIError(&(lpOraHandle->cda));
for (i = 0; i < col; i++)
		{
if (lpOraHandle->desc[i].dbtype != FLOAT_TYPE && lpOraHandle->desc[i].dbtype != INT_TYPE)
			{
if (lpOraHandle->desc[i].defptr != NULL) ECOFree(lpOraHandle->desc[i].defptr);
}
}
		return -6;
}

	return 0;
ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : FetchOneRecord
// 기  능 : SELECT 한 커서 정보를 가지고 결과값을 리턴받는다
//          Parameter :
//            lpOraHandle  : [I N] Oracle 연결 정보
//            pOutData     : [OUT] 결과 값을 받는 구조체 변수 포인터
//--------------------------------------------------------------------------
INT FetchOneRecord(
ORA_HANDLE* lpOraHandle , //
					DB_DATA_ARRAY_TYPE** pOutData	 // [OUT] GYEOLGWA Data
)
{
ECO_EXCEPTION_BEGIN

	INT col, i , ids;
INT nColumnCount;

	TCHAR szFormat[256];

nColumnCount = lpOraHandle->nColumnCount;

	// 결과값을 리턴받는 구조체 변수의 메모리를 할당한다
pOutData[0] = (DB_DATA_ARRAY_TYPE*) ECO_malloc(sizeof(DB_DATA_ARRAY_TYPE) * nColumnCount);
	if (pOutData == NULL)
	{
		return -1;
	}
	wmemset(pOutData[0], 0x00, sizeof(DB_DATA_ARRAY_TYPE) * nColumnCount);

	// 결과값을 받는다
ids = ECO_ofetch(&(lpOraHandle->cda));
if (ids)
	{
if (lpOraHandle->cda.rc == NO_DATA_FOUND )
		{
			return 0;
		}
		else if (lpOraHandle->cda.rc != NULL_VALUE_RETURNED)
		{
			FreeDBDataArrayType(pOutData[0], nColumnCount);
OCIError(&(lpOraHandle->cda));
			return -2;
}
}
	else
	{
for (col = 0; col < nColumnCount; col++)
		{
			pOutData[0][col].pDBDataUnion = (DB_DATA_UNION *) ECORealloc(pOutData[0][col].pDBDataUnion, sizeof(DB_DATA_UNION) * 1);
			if (pOutData[0][col].pDBDataUnion == NULL)
			{
				FreeDBDataArrayType(pOutData[0], nColumnCount);
				return -3;
			}

			switch (lpOraHandle->desc[col].dbtype)
			{
			case FLOAT_TYPE :
			case INT_TYPE :
				pOutData[0][col].nDataType = NUMERIC;
				break;
			case DATE_TYPE :
				pOutData[0][col].nDataType = DATE;
				break;
			default :
				pOutData[0][col].nDataType = STRING;
			}

			lpOraHandle->desc[col].defptr[lpOraHandle->desc[col].dbsize] = '\0';
str_rtrim((TCHAR*) lpOraHandle->desc[col].defptr, (TCHAR*) lpOraHandle->desc[col].defptr, ' ');

			// NUMBER 타입 중 소수점이 들어가는 것을 처리 (2000/06/19 by 이종혁)

			if (lpOraHandle->desc[col].scale > 0)
			{
				_stprintf(szFormat, _T("%%-%d.%df"), lpOraHandle->desc[col].precision, lpOraHandle->desc[col].scale);
				_stprintf((TCHAR*) lpOraHandle->desc[col].defptr, szFormat, atof((TCHAR*) lpOraHandle->desc[col].defptr));
				str_rtrim((TCHAR*) lpOraHandle->desc[col].defptr, (TCHAR*) lpOraHandle->desc[col].defptr, ' ');
			}

pOutData[0][col].pDBDataUnion[0].pStringData = (TCHAR*) ECOCopyMalloc(_tcsclen((TCHAR*) lpOraHandle->desc[col].defptr)+sizeof(TCHAR), lpOraHandle->desc[col].defptr);
if (pOutData[0][col].pDBDataUnion[0].pStringData == NULL)
			{
				FreeDBDataArrayType(pOutData[0], nColumnCount);
for (i = 0; i < nColumnCount; i++)
				{
if (lpOraHandle->desc[i].defptr != NULL) ECOFree(lpOraHandle->desc[i].defptr);
}
return -4;
}

			wmemset(lpOraHandle->desc[col].defptr, 0x00, lpOraHandle->desc[col].dbsize);

pOutData[0][col].nRowCount = 1;
}
}

return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : DisconnectDBForOneRecord
// 기  능 : 커서의 연결을 종료한다
//          Parameter :
//            lpOraHandle  : [I N] Oracle 연결 정보
//--------------------------------------------------------------------------
INT DisconnectDBForOneRecord(ORA_HANDLE *lpOraHandle)
{
ECO_EXCEPTION_BEGIN

if (ECO_oclose(&(lpOraHandle->cda))) MESSAGE(M$WARN, MSG_ARGMENT, _T("Error closing cursor!"));

	for (INT i = 0; i < lpOraHandle->nColumnCount; i++)
	{
		if (lpOraHandle->desc[i].dbtype != FLOAT_TYPE && lpOraHandle->desc[i].dbtype != INT_TYPE)
		{
			if (lpOraHandle->desc[i].defptr != NULL) ECOFree(lpOraHandle->desc[i].defptr);
		}
	}
return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : UnSelectProc
// 기  능 : SELECT가 아닌 DML 커리를 수행한다
//          Parameter :
//            pszUserID  : [I N] 사용자 ID
//            pszPasswd  : [I N] 사용자 Password
//            pszSQL     : [I N] DML SQL
//--------------------------------------------------------------------------
INT UnSelectProc(
TCHAR* pszUserID ,	// [IN] SAYONGJAID
TCHAR* pszPasswd ,	// [IN] SAYONGJAPASSWD
TCHAR* pszSQL				// [IN] SILHAENGHAL SQL
)
{
ECO_EXCEPTION_BEGIN

INT ids;

TCHAR* pSQL = NULL;

	// 연결 확인
	ids = ConnectDB((UCHAR*) pszUserID, (UCHAR*) pszPasswd);
	if (ids) return -100;

	// PL/SQL형식으로 바꿔준다
pSQL = (TCHAR*) ECOMalloc(sizeof(TCHAR) * _tcsclen(pszSQL) + 32);
if (NULL == pSQL) return -1;

_stprintf(pSQL, _T("begin %s end;"), pszSQL);

	// 수행 SQL에 대해 로그를 남긴다
LoggingProcess (LOG_SQL_FILE_NAME , pszSQL);

	// SQL을 검사한다
if (oparse(&cda, (text*) pSQL, (sb4) -1, (sword) PARSE_NO_DEFER, (ub4) PARSE_V7_LNG))
	{
OCIError(&cda); free(pSQL); return -2;
}

	// 전에 binding된 변수가 있다면 binding 시킨다
	ids = BindingVariable();
	if (ids)
	{
		free(pSQL); return -3;
	}

	// SQL을 수행한다
if (oexec(&cda))
	{
		FreeBindingVariable();
OCIError(&cda); free(pSQL); return -4;
}

	FreeBindingVariable();
free(pSQL);

return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : InsertProc
// 기  능 : INSERT 문을 수행한다
//          Parameter :
//            pszUserID        : [I N] 사용자 ID
//            pszPasswd        : [I N] 사용자 Password
//            pszTableName     : [I N] 테이블명
//            nColumnCount     : [I N] 칼럼수
//            pszAttributeName : [I N] Attribute명
//            pszAttributeData : [I N] Attribute Data
//            pnInDataType     : [I N] Attribute Type
//            bCommit          : [I N] Commit 여부
//--------------------------------------------------------------------------
INT	 InsertProc(
				TCHAR* pszUserID ,	// [IN] SAYONGJAID
TCHAR* pszPasswd ,	// [IN] SAYONGJAPASSWD
				TCHAR* pszTableName ,	// [IN] InsertHAL TEIBEULMYEONG
				INT nColumnCount ,	// [IN] InsertHAL KALREOMSU
				TCHAR** pszAttributeName,	// [IN] InsertHAL AttributeMYEONG
				TCHAR** pszAttributeData,	// [IN] InsertHAL Attribute Data
				INT* pnInDataType ,	// [IN] InsertHAL Attribute Type
				BOOL bCommit				// [IN[ Insert HU Commit YEOBU
			 )
{
ECO_EXCEPTION_BEGIN

INT ids;

	CString sSQLStatement;

	// Insert SQL을 만든다
	ids = MakeInsertSQL(pszTableName, nColumnCount, pszAttributeName, pszAttributeData, pnInDataType, sSQLStatement);
	if (ids)
	{
		FreeBindingVariable();
		return -1;
	}

	// SQL을 실행한다
	ids = UnSelectProc(pszUserID, pszPasswd, sSQLStatement.GetBuffer(0));
if (ids != 0)
	{
		FreeBindingVariable();
MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);
return -2;
}

	// Commit 한다
	if (bCommit == TRUE)
	{
		ids = CommitProc(pszUserID, pszPasswd);
		if (ids)
		{
			FreeBindingVariable();
			return -3;
		}
	}

	return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : UpdateProc
// 기  능 : UPDATE 문을 수행한다
//          Parameter :
//            pszUserID          : [I N] 사용자 ID
//            pszPasswd          : [I N] 사용자 Password
//            pszTableName       : [I N] 테이블명
//            nColumnCount       : [I N] 칼럼수
//            pszAttributeName   : [I N] Attribute명
//            pszAttributeData   : [I N] Attribute Data
//            pnInDataType       : [I N] Attribute Type
//            pszPKAttributeName : [I N] Primary Key Attribute 명
//            pszTablePrimaryKey : [I N] Primary Key 값
//            bCommit            : [I N] Commit 여부
//--------------------------------------------------------------------------
INT	 UpdateProc(
				TCHAR* pszUserID ,		// [IN] SAYONGJAID
TCHAR* pszPasswd ,		// [IN] SAYONGJAPASSWD
				TCHAR* pszTableName ,		// [IN] UpdateHAL TEIBEULMYEONG
				INT nColumnCount ,		// [IN] UpdateHAL KALREOMSU
				TCHAR** pszAttributeName ,		// [IN] UpdateHAL AttributeMYEONG
				TCHAR** pszAttributeData ,		// [IN] UpdateHAL Attribute Data
				INT* pnInDataType ,		// [IN] UpdateHAL Attribute Type
				TCHAR* pszPKAttributeName,		// [IN] UpdateHAL TEIBEULYI Key AttributeMYEONG
				TCHAR* pszTablePrimaryKey,		// [IN] UpdateHAL TEIBEULYI Key GABS
				BOOL bCommit					// [IN] Update HU Commit YEOBU
			 )
{
ECO_EXCEPTION_BEGIN
INT ids;

	CString sSQLStatement;

	// Update SQL을 만든다
	ids = MakeUpdateSQL(pszTableName, nColumnCount, pszAttributeName, pszAttributeData, pnInDataType, pszPKAttributeName, pszTablePrimaryKey, sSQLStatement);
	if (ids) {
		FreeBindingVariable();
		return -1;
	}

	// SQL을 실행한다
	ids = UnSelectProc(pszUserID, pszPasswd, sSQLStatement.GetBuffer(0));
if (ids != 0) {
		FreeBindingVariable();
MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);
return -2;
}

	// Commit 한다

	if (bCommit == TRUE)
	{
		ids = CommitProc(pszUserID, pszPasswd);
		if (ids)
		{
			FreeBindingVariable();
			return -3;
		}
	}

	return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : DeleteProc
// 기  능 : DELETE 문을 수행한다
//          Parameter :
//            pszUserID          : [I N] 사용자 ID
//            pszPasswd          : [I N] 사용자 Password
//            pszTableName       : [I N] 테이블명
//            pszPKAttributeName : [I N] Primary Key Attribute 명
//            pszTablePrimaryKey : [I N] Primary Key 값
//            bCommit            : [I N] Commit 여부
//--------------------------------------------------------------------------
INT	 DeleteProc(
				TCHAR* pszUserID ,		// [IN] SAYONGJAID
TCHAR* pszPasswd ,		// [IN] SAYONGJAPASSWD
				TCHAR* pszTableName ,		// [IN] SAGJE DAESANG TableMYEONG
				TCHAR* pszPKAttributeName,		// [IN] SAGJE DAESANG TableYI Key AttributeMYEONG
				TCHAR* pszTablePrimaryKey,		// [IN] SAGJE DAESANG REKODEUYI Key GABS
				BOOL bCommit					// [IN] SAGJE HU Commit YEOBU
			 )
{
ECO_EXCEPTION_BEGIN

INT ids;

	CString sSQLStatement;

	// 자료의 무결성을 확인한다
	if (_tcsclen(pszTableName) <= 0 || _tcsclen(pszPKAttributeName) <= 0 || _tcsclen(pszTablePrimaryKey) <= 0)
	{
		FreeBindingVariable();
		return -1;
	}

	// SQL문을 만든다

	sSQLStatement.Format(_T("DELETE FROM %s WHERE %s = %s;"), pszTableName, pszPKAttributeName, pszTablePrimaryKey);

	// SQL을 실행한다

	ids = UnSelectProc(pszUserID, pszPasswd, sSQLStatement.GetBuffer(0));
if (ids != 0)
	{
		FreeBindingVariable();
MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);
return -2;
}

	// 4. Commit 한다

	if (bCommit == TRUE)
	{
		ids = CommitProc(pszUserID, pszPasswd);
		if (ids)
		{
			FreeBindingVariable();
			return -3;
		}
	}

	return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : ExecSQLProc
// 기  능 : SQL 문을 수행한다
//          Parameter :
//            pszUserID          : [I N] 사용자 ID
//            pszPasswd          : [I N] 사용자 Password
//            pszSQL             : [I N] 테이블명
//            bCommit            : [I N] Commit 여부
//--------------------------------------------------------------------------
INT	 ExecSQLProc(
				 TCHAR* pszUserID ,		// [IN] SAYONGJAID
TCHAR* pszPasswd ,		// [IN] SAYONGJAPASSWD
				 TCHAR* pszSQL ,		// [IN] SILHAENGHAL SQL
				 BOOL bCommit			// [IN] SILHAENGHU Commit YEOBU
				)
{
ECO_EXCEPTION_BEGIN

INT ids;

	// 자료의 무결성을 확인한다
	if (_tcsclen(pszSQL) <= 0)
	{
		FreeBindingVariable();
		return -1;
	}

	// SQL을 실행한다
	ids = UnSelectProc (pszUserID, pszPasswd, pszSQL);
if (ids != 0)
	{
		FreeBindingVariable();
MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);
return -2;
}

	// Commit 한다

	if (bCommit == TRUE)
	{
		ids = CommitProc(pszUserID, pszPasswd);
		if (ids)
		{
			FreeBindingVariable();
			return -3;
		}
	}

	return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : CommitProc
// 기  능 : Commit 문을 수행한다
//          Parameter :
//            pszUserID          : [I N] 사용자 ID
//            pszPasswd          : [I N] 사용자 Password
//--------------------------------------------------------------------------
INT	 CommitProc(
				TCHAR* pszUserID ,	// [IN] SAYONGJAID
TCHAR* pszPasswd			// [IN] SAYONGJAPASSWD
)
{
ECO_EXCEPTION_BEGIN

INT ids;

	ids = UnSelectProc (pszUserID, pszPasswd, _T("BEGIN COMMIT; END;"));
	if (ids != 0)
	{
		FreeBindingVariable();
		MESSAGE(M$WARN, MSG_ARGMENT, _T("<< DB_Access_Proc >> UnSelectProc Error. [%d]"), ids);
		return -1;
	}

	return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : NextNumberProc
// 기  능 : 번호를 따온다
//          Parameter :
//            pszUserID          : [I N] 사용자 ID
//            pszPasswd          : [I N] 사용자 Password
//            pszSQL             : [I N] 번호 구하기용 SQL
//            pszNumberAlias     : [I N] SQL에 있는 번호 Alias
//            pszNumber          : [OUT] 구해진 번호
//--------------------------------------------------------------------------
INT	 NextNumberProc(
					TCHAR* pszUserID ,		// [I N] SAYONGJAID
					TCHAR* pszPasswd ,		// [I N] SAYONGJAPASSWD
				 	TCHAR* pszSQL ,		// [I N] BEONHO GUHAGIYONG SQL
					TCHAR* pszNumberAlias ,		// [I N] SQLE ISSNEUN BEONHO Alias
					TCHAR* pszNumber				// [OUT] GUHAEJIN BEONHO
				 )
{
ECO_EXCEPTION_BEGIN

INT ids;

	ub2 n_ret_len;
	ub2 n_ret_rcode;
	sb2 n_ret_indp;

	TCHAR* pSQL = NULL;

	NUMBER_DATA_TYPE tNumberData;

	// DB 연결을 확인한다
	ids = ConnectDB((UCHAR*) pszUserID, (UCHAR*) pszPasswd);
	if (ids)
	{
		return -100;
	}

	// 자료의 무결성을 확인한다
	if (_tcsclen(pszSQL) <= 0 || _tcsclen(pszNumberAlias) <= 0)
	{
		FreeBindingVariable();
		return -1;
	}

	pSQL = (TCHAR*) malloc(sizeof(TCHAR) * (_tcsclen(pszSQL) + 36));
	if (pSQL == NULL)
	{
		FreeBindingVariable();
		return -2;
	}

_tcscpy(tNumberData.szNumberAlias, pszNumberAlias);

	_stprintf(pSQL, _T("begin %s end;"), pszSQL);

	// 수행 SQL의 로그를 남긴다
LoggingProcess (LOG_SQL_FILE_NAME , pszSQL);

	// SQL문의 무경설을 검사한다
	if (oparse(&cda, (text*) pszSQL, (sb4) -1, (sword) PARSE_NO_DEFER, (ub4) PARSE_V7_LNG))
	{
FreeBindingVariable(); OCIError(&cda); free(pSQL); return -3;
}

	// 받을 Number 변수에 관해 Binding한다

    // Bind the OUT n_ret using obndra. obndrv could
    // have been used just as well, since no arrays
    // are involved, but it is possible to use obndra
    // for scalars as well.


	n_ret_len = (sizeof (TCHAR) * NUMBER_TYPE_LEN);

	wmemset(tNumberData.szNumberValue, 0x00, NUMBER_TYPE_LEN);

	if (obndra(&cda,
		 (text*) tNumberData.szNumberAlias,
		 -1,
		 (ub1*) &tNumberData.szNumberValue,
		 (sword) (sizeof (TCHAR) * NUMBER_TYPE_LEN),
		 STRING_TYPE,
		 -1,
		 &n_ret_indp,
		 &n_ret_len,
		 &n_ret_rcode,
		 (ub4) 0, /* pass as 0, not 1, when binding a scalar */
		 (ub4*) 0, /* pass as the null pointer when scalar */
		 (text*) 0,
		 -1,
		 -1))
	{
		FreeBindingVariable();
		free(pSQL);
		OCIError(&cda);
		return -4;
	}

	// SQL을수행한다
	if (oexec(&cda))
	{
		FreeBindingVariable();
		free(pSQL);
OCIError(&cda);
return -5;
}

	free(pSQL);

_stprintf(pszNumber, _T("%s"), tNumberData.szNumberValue);

	return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : GetOneFieldValueProc
// 기  능 : SELECT의 첫행 첫열을 구한다
//          Parameter :
//            pszUserID          : [I N] 사용자 ID
//            pszPasswd          : [I N] 사용자 Password
//            pszSQL             : [I N] SELECT SQL
//            pszReturnValue     : [OUT] 첫번째 Row 첫번째 Column의 값
//--------------------------------------------------------------------------
INT GetOneFieldValueProc(
						 TCHAR* pszUserID ,	// [IN] SAYONGJAID
						 TCHAR* pszPasswd ,	// [IN] SAYONGJAPASSWD
						 TCHAR* pszSQL ,	// [IN] SILHAENGHAL SQL
						 TCHAR* pszReturnValue	// [OUT] CEOTBEONJJAE Row CEOTBEONJJAE ColumnYI GABS
)
{
ECO_EXCEPTION_BEGIN

INT ids;
	const INT nMaxBufferSize = 100000;

	TCHAR szBuffer[nMaxBufferSize];

	sb2 ind_name;
	ub2 retc_name;
	ub2 retl_name;

	// DB 연결을 확인한다
	ids = ConnectDB((UCHAR*) pszUserID, (UCHAR*) pszPasswd);
	if (ids)
	{
		return -100;
	}

	// 수행 SQL의 로그를 남긴다
LoggingProcess (LOG_SQL_FILE_NAME , pszSQL);

    // SQL 무결성을 검사한다
if (oparse(&cda, (text*) pszSQL, (sb4) -1, (sword) PARSE_NO_DEFER, (ub4) PARSE_V7_LNG))
	{
OCIError(&cda);
return -1;
}

	// 받아오는 값의 Position을 Define 한다
	wmemset(szBuffer, 0x00, nMaxBufferSize);

	if (odefin(&cda, 1, (ub1*) szBuffer, (sword) nMaxBufferSize, STRING_TYPE, -1, &ind_name, (text*) 0, -1, -1, &retc_name, &retl_name))
	{
		OCIError(&cda);
		return -2;
	}

	// 실행한다
	if (oexec(&cda))
	{
OCIError(&cda);
return -3;
}

	// Fetch 한다
	if (ofetch(&cda))
	{
if (cda.rc == NO_DATA_FOUND )
		{
			_tcscpy(pszReturnValue, _T(""));
			return 0;
		}

if (cda.rc != NULL_VALUE_RETURNED)
		{
OCIError(&cda);
			_tcscpy(pszReturnValue, _T(""));
			return 0;
}
	}
	else
	{
		ECOStrRTrim(szBuffer, szBuffer, ' ');

		_tcscpy(pszReturnValue, szBuffer);
	}

	return 0;

ECO_EXCEPTION_END
}


//--------------------------------------------------------------------------
// 함수명 : BindingVariable
// 기  능 : 저장되어 있던 바인딩 정보를 가지고 바인딩한다
//--------------------------------------------------------------------------
INT		BindingVariable()
{
ECO_EXCEPTION_BEGIN

	INT i;

	for (i = 0; i < g_nBindingCount; i++) {
		if (obndrv(&cda,
					(text *) g_pBindingData[i].szBindingAlias,
					-1,
					(ub1 *) g_pBindingData[i].pBindingValue,
					(sword) sizeof(TCHAR) * (_tcsclen(g_pBindingData[i].pBindingValue) + 1),
					STRING_TYPE,
					-1,
					(sb2 *) 0,
					(text *) 0,
					-1,
					-1))
		{
			OCIError(&cda);
			return -1;
		}
	}

	return 0;

ECO_EXCEPTION_END
}

//--------------------------------------------------------------------------
// 함수명 : FetchFile
// 기  능 : DB로부터 File을 가져온다
//          Parameter :
//            pszUserID      : [I N] 사용자 ID
//            pszPasswd      : [I N] 사용자 Password
//            pszTableName   : [I N] 테이블명
//            pszFieldName   : [I N] Long Raw Type의 필드명
//            pszFileName    : [I N] 생성할 파일명
//            pszRecKey      : [I N] Priamry Key(필드명은 REC_KEY 로 함)
//            nPieceSize     : [I N] Piece Size
//--------------------------------------------------------------------------
// Function FetchFile() : retrieves contents of 'filename'      /
//         from database and (re)stores it back to disk         /
//                                                              /
// Note: If necessary, the context pointer could be used to     /
//          point to a file being used in a piecewise operation./
//          It is not necesssay in this example program, so a   /
//          dummy value is passed instead.                      /

INT FetchFile(
			 TCHAR*			pszUserID ,	// [I N] SAYONGJAID
TCHAR*			pszPasswd ,	// [I N] SAYONGJAPASSWD
			 TCHAR*			pszTableName ,	// [I N] TEIBEULMYEONG
			 TCHAR* pszFieldName , // [I N] PILDEUMYEONG
			 TCHAR*			pszFileName ,	// [I N] PAILMYEONG
			 TCHAR*			pszRecKey , // [I N] PrimaryKey
			 UINT		nPieceSize 		// [I N]
			 )
{
	INT ids;
	text* longbuf;				// buffer to hold long column on insert
	ub4 len_longbuf;			// length of buffer
	INT fd;
	TCHAR* context = _T("context pointer");
	ub1 piece;
	eword iteration = 0;
	eword plsqltable;
	ub1 cont = 1;
	ub2 col_rcode;
	ub2 col_len;
	TCHAR szSQL [512];

	// DB 연결을 확인한다
	ids = ConnectDB((UCHAR*) pszUserID, (UCHAR*) pszPasswd);
	if (ids)
	{
		return -100;
	}

    // 저장할 파일을 연다
	if (!(fd = open(pszFileName, O_WRONLY | O_CREAT | O_BINARY, 511))) return -1;

	// Allocate memory for storage of one piece
	len_longbuf = nPieceSize;
	longbuf = (text*) malloc(len_longbuf);
	if (longbuf == (text*) NULL)
	{
		return -2;
	}

	// SQL문의 무결성 확인
	CWaitCursor wait;

	_stprintf(szSQL, _T("SELECT %s FROM %s WHERE REC_KEY=%s"), pszFieldName, pszTableName, pszRecKey);

	if (oparse(&cda, (text*) szSQL, (sb4)-1, 0, (ub4)VERSION_7))
	{
		OCIError(&cda);
		if (longbuf != NULL) {
			free(longbuf);
			longbuf = NULL;
		}
		close(fd);
		return -3;
	}

	// file 내용을 binding 한다
	if (odefinps(&cda, 0, 1, (ub1*) &context, (ub4) MAX_COL_SIZE,
			 SQLT_LBI, 0, (sb2 *)0, (text *)0, 0, 0,
			 (ub2*) &col_len, (ub2*) &col_rcode, 0, 0, 0, 0))
	{
		OCIError(&cda);
		if (longbuf != NULL)
		{
			free(longbuf);
			longbuf = NULL;
		}
		close(fd);
		return -4;
	}

	// execute SQL statement
	if (oexec(&cda))
	{
		OCIError(&cda);
		if (longbuf != NULL)
		{
			free(longbuf);
			longbuf = NULL;
		}
		close(fd);
		return -5;
	}

	while (cont)						// while pieces remain to be fetched
	{
		ofetch(&cda);
		switch (cda.rc)						// do fetch & switch on return value
		{
		case 0:								// write last piece to buffer
			if (len_longbuf != (ub4) write(fd, longbuf, len_longbuf))
			{
				return -6;
				if (longbuf != NULL)
				{
					free(longbuf);
					longbuf = NULL;
				}
				close(fd);
			}

			cont = 0;
			break;
		case OCI_MORE_FETCH_PIECES:			// ORA-03130 was returned
			ogetpi(&cda, &piece, (VOID **)&context, (UINT *) &iteration, (UINT *) &plsqltable);

			if (piece != OCI_FIRST_PIECE)	// can't write on first fetch
				if (len_longbuf != (ub4) write(fd, longbuf, len_longbuf))
				{
					if (longbuf != NULL)
					{
						free(longbuf);
						longbuf = NULL;
					}
					close(fd);
					return -7;
				}


			osetpi(&cda, piece, longbuf, &len_longbuf);
			break;
		default:
			OCIError(&cda);
			if (longbuf != NULL)
			{
				free(longbuf);
				longbuf = NULL;
			}
			close(fd);
			return -8;						// other value indicates error
		}
	}

	if (longbuf != NULL)
	{
		free(longbuf);
		longbuf = NULL;
	}

	if (close(fd)) return -9;			// close file

	return 0;
}

//--------------------------------------------------------------------------
// 함수명 : InsertFile
// 기  능 : DB로부터 File을 저장한다
//          Parameter :
//            pszUserID      : [I N] 사용자 ID
//            pszPasswd      : [I N] 사용자 Password
//            pszTableName   : [I N] 테이블명
//            pszFieldName   : [I N] Long Raw Type의 필드명
//            pszFileName    : [I N] 입력할 파일명
//            pszRecKey      : [I N] Priamry Key(필드명은 REC_KEY 로 함)
//            nPieceSize     : [I N] Piece Size
//--------------------------------------------------------------------------
// Function InsertFile() : This function loads long raw data    /
//          into a memory buffer from a source file and then    /
//          inserts it piecewise into the database              /
//                                                              /
// Note: If necessary, the context pointer could be used to     /
//          point to a file being used in a piecewise operation./
//          It is not necesssay in this example program, so a   /
//          dummy value is passed instead.                      /

INT InsertFile(
				TCHAR*			pszUserID ,	// [IN] SAYONGJAID
				TCHAR*			pszPasswd ,	// [IN] SAYONGJAPASSWD
				TCHAR*			pszTableName ,	// [IN] TEIBEULMYEONG
				TCHAR* pszFieldName , // [IN] PILDEUMYEONG
				TCHAR*			pszFileName ,	// [IN] PAILMYEONG
				TCHAR*			pszRecKey , // [IN] PrimaryKey
				UINT	nPieceSize
			 )
{
	INT ids;
	text* longbuf = NULL;					// buffer to hold long column on insert
	ub4 len_longbuf;						// length of longbuf
	ub2 col_rcode;						// Column return code
	INT fd;
	TCHAR* context = _T("context pointer");
	ub1 piece;
	ub4 iteration;
	ub4 plsqltable;
	ub1 cont = (ub1)1;
	TCHAR szSQL [512];

	// DB 연결을 확인한다
	ids = ConnectDB((UCHAR*) pszUserID, (UCHAR*) pszPasswd);
	if (ids)
	{
		return -100;
	}

	// 파일을 연다
	if (!(fd = open(pszFileName, O_RDONLY|O_BINARY))) return -1;

	// Allocate memory for storage of one piece
	len_longbuf = nPieceSize;
	longbuf = (text *)malloc(len_longbuf);
	if (longbuf == (text *)NULL) return -2;

	// SQL의 무결성을 확인한다
	CWaitCursor wait;

	_stprintf(szSQL, _T("INSERT INTO %s(REC_KEY, %s) VALUES(%s,:filecontent)")
		, pszTableName, pszFieldName, pszRecKey );

	if (oparse(&cda, (text*)szSQL, (sb4)-1, 0, (ub4)VERSION_7))
	{
		OCIError(&cda);
		if (longbuf != NULL)
		{
			free(longbuf);
			longbuf = NULL;
		}
		close(fd);
		return -3;
	}

	// filecontent를 binding한다
	if (obindps(&cda, 0, (text*) _T(":filecontent"),
			 _tcsclen(_T(":filecontent")), (ub1*) context, len_longbuf,
			 SQLT_LBI, (sword)0, (sb2*) 0,
			 (ub2*) 0, &col_rcode, 0, 0, 0, 0,
			 0, (ub4*) 0, (text *)0, 0, 0))
	{
		OCIError(&cda);
		if (longbuf != NULL)
		{
			free(longbuf);
			longbuf = NULL;
		}
		close(fd);
		return -4;
	}

	while (cont)
	{
		oexec(&cda);
		switch (cda.rc)
		{
		case 0:										// operation is finished
			cont = 0;
			break;
		case OCI_MORE_INSERT_PIECES:				// ORA-03129 was returned
			if ((len_longbuf = read(fd, longbuf, len_longbuf)) == -1) {
				if (longbuf != NULL) {
					free(longbuf);
					longbuf = NULL;
				}
				close(fd);
				return -5;
			}

			ogetpi(&cda, &piece, (dvoid **)&context, &iteration, &plsqltable);

			if (len_longbuf < nPieceSize) // last piece?
				piece = OCI_LAST_PIECE;
			osetpi(&cda, piece, longbuf, &len_longbuf);
			break;
		default:
			OCIError(&cda);
			if (longbuf != NULL)
			{
				free(longbuf);
				longbuf = NULL;
			}
			close(fd);
			return -6;

		}
	}

	if (longbuf != NULL)
	{
		free(longbuf);
		longbuf = NULL;
	}

	if (close(fd)) return -7;						// close file

	return 0;
}

//--------------------------------------------------------------------------
// 함수명 : UpdateFile
// 기  능 : DB로부터 File을 갱신한다
//          Parameter :
//            pszUserID      : [I N] 사용자 ID
//            pszPasswd      : [I N] 사용자 Password
//            pszTableName   : [I N] 테이블명
//            pszFieldName   : [I N] Long Raw Type의 필드명
//            pszFileName    : [I N] 입력할 파일명
//            pszRecKey      : [I N] Priamry Key(필드명은 REC_KEY 로 함)
//            nPieceSize     : [I N] Piece Size
//--------------------------------------------------------------------------
INT UpdateFile(
				TCHAR*			pszUserID ,	// [IN] SAYONGJAID
				TCHAR*			pszPasswd ,	// [IN] SAYONGJAPASSWD
				TCHAR*			pszTableName ,	// [IN] TEIBEULMYEONG
				TCHAR*			pszFieldName , // [IN] PILDEUMYEONG
				TCHAR*			pszFileName ,	// [IN] PAILMYEONG
				TCHAR*			pszRecKey , // [IN] PrimaryKey
				UINT	nPieceSize
			 )
{
	INT ids;
	text* longbuf = NULL;					// buffer to hold long column on insert
	ub4 len_longbuf;						// length of longbuf
	ub2 col_rcode;						// Column return code
	INT fd;
	TCHAR* context = _T("context pointer");
	ub1 piece;
	ub4 iteration;
	ub4 plsqltable;
	ub1 cont = (ub1)1;
	TCHAR szSQL [512];

	// DB 연결을 확인한다
	ids = ConnectDB((UCHAR*) pszUserID, (UCHAR*) pszPasswd);
	if (ids)
	{
		return -100;
	}

	// 파일을 연다

	if (!(fd = open(pszFileName, O_RDONLY|O_BINARY))) return -1;

	// Allocate memory for storage of one piece

	len_longbuf = nPieceSize;
	longbuf = (text *)malloc(len_longbuf);
	if (longbuf == (text *)NULL) return -2;

	// SQL의 무결성을 확인한다
	CWaitCursor wait;

	_stprintf(szSQL, _T("UPDATE %s SET FILECONTENT=:filecontent WHERE REC_KEY=%s")
		, pszTableName, pszRecKey);

	if (oparse(&cda, (text*) szSQL, (sb4)-1, 0, (ub4)VERSION_7))
	{
		OCIError(&cda);
		if (longbuf != NULL) {
			free(longbuf);
			longbuf = NULL;
		}
		close(fd);
		return -3;
	}

	// filecontent를 binding한다

	if (obindps(&cda, 0, (text*) _T(":filecontent"),
			 _tcsclen(_T(":filecontent")), (ub1*) context, len_longbuf,
			 SQLT_LBI, (sword)0, (sb2*) 0,
			 (ub2*) 0, &col_rcode, 0, 0, 0, 0,
			 0, (ub4*) 0, (text *)0, 0, 0))
	{
		OCIError(&cda);
		if (longbuf != NULL)
		{
			free(longbuf);
			longbuf = NULL;
		}
		close(fd);
		return -4;
	}

	while (cont)
	{
		oexec(&cda);
		switch (cda.rc)
		{
		case 0:										// operation is finished
			cont = 0;
			break;
		case OCI_MORE_INSERT_PIECES:				// ORA-03129 was returned
			if ((len_longbuf = read(fd, longbuf, len_longbuf)) == -1)
			{
				if (longbuf != NULL)
				{
					free(longbuf);
					longbuf = NULL;
				}
				close(fd);
				return -5;
			}

			ogetpi(&cda, &piece, (dvoid **)&context, &iteration, &plsqltable);

			if (len_longbuf < nPieceSize) // last piece?
				piece = OCI_LAST_PIECE;
			osetpi(&cda, piece, longbuf, &len_longbuf);
			break;
		default:
			OCIError(&cda);
			if (longbuf != NULL)
			{
				free(longbuf);
				longbuf = NULL;
			}
			close(fd);
			return -6;

		}
	}

	if (longbuf != NULL)
	{
		free(longbuf);
		longbuf = NULL;
	}

	if (close(fd)) return -7;						// close file

	return 0;
}
