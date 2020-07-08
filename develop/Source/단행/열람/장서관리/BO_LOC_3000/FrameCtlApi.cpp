#include "stdafx.h"
#include "FrameCtlApi.h"

// @since   2011.08.25
// @author  PWR
// @memo    MakeFrame Func for LibSecu
INT MakeFrame(CString strSystem, CString strCMD, CString strDataType, CStringArray &astrData, CString &strFrame, CString strNextData/*="0"*/ )
{	
	INT		nArrayCnt;
	CString strBuf;	
	CString strTemp;
	CString strBody;	
	CString strHeader;

	// 1. make body
	// DataCnt | DataLen | DataValue | DataLen | DataValue |.....
	//    6    |    5    |    ...    |    5    |    ...    |.....	
	nArrayCnt = astrData.GetSize();

	for( INT i=0;i<nArrayCnt;i++ )
	{
		strTemp.Empty();
		strBuf = astrData.GetAt(i);
		
		strTemp.Format(_T("%05d%s"), strBuf.GetLength(), strBuf);
		strBody += strTemp;		
	}
	
	if( nArrayCnt )
	{
		strTemp.Empty();
		strTemp.Format(_T("%06d"), nArrayCnt);
		strBody = strTemp + strBody;
	}

	// 2. make header
	// SystemType | Cmd | DataType | BodyLen | NextData
	//      1     |  3  |     1    |    10   |     1
	strTemp.Empty();

	strHeader.Format(_T("%s%s%s%010d%s"), strSystem, strCMD, strDataType, strBody.GetLength(), strNextData);

	// size check
	if( (_SA_SYSTEMTYPE_LEN + 
		 _SA_CMD_LEN + 
		 _SA_DATATYPE_LEN + 
		 _SA_BODYLEN_LEN + 
		 _SA_NEXTDATA_LEN) != strHeader.GetLength() )
		return -2013;

	// 3. add stx, etx
	// _SA_STX		2 
	// _SA_ETX		3 
	strFrame.Empty();

	strFrame.Format(_T("%c%s%s%c"), _SA_STX, strHeader, strBody, _SA_ETX);

	AfxMessageBox(strFrame);

	return 0;
}

// @since   2011.08.25
// @author  PWR
// @memo    AnalyzeFrame Func for LibSecu
INT AnalyzeFrame(CString strFrame, CString &strSystem, CString &strCMD, CString &strDataType, CString &strNextData, CStringArray &astrData)
{	
	CString strHeader;
	INT		nBodyLen;	
	CString strBody;
	INT     nDataCnt;
	CString strBuf;	
	INT     nDataSize;

	// 1. delete stx, etx
	//    stx		First-Idx 0 
	//    etx       Last-Idx  len-1 
	
	// check stx
	if( CString(_SA_STX) != strFrame.Mid(0,1) )
		return -1011;
	
	// check etx
	if( CString(_SA_ETX) != strFrame.Mid(strFrame.GetLength()-1) )
		return -1012;

	// delete stx, etx
	strFrame = strFrame.Mid(1, strFrame.GetLength()-2);

	// min size check
	if( (_SA_SYSTEMTYPE_LEN + 
		 _SA_CMD_LEN + 
		 _SA_DATATYPE_LEN + 
		 _SA_BODYLEN_LEN + 
		 _SA_NEXTDATA_LEN) > strFrame.GetLength() )
		return -1013;

	// 2. analyze header
	// SystemType | Cmd | DataType | BodyLen | NextData
	//      1     |  3  |     1    |    10   |     1
	// _SA_SYSTEMTYPE_LEN   1
	// _SA_CMD_LEN			3
	// _SA_DATATYPE_LEN		1
	// _SA_BODYLEN_LEN		10	
	// _SA_NEXTDATA_LEN     1
	// Header size = _SA_SYSTEMTYPE_LEN + _SA_CMD_LEN + _SA_DATATYPE_LEN + _SA_BODYLEN_LEN + _SA_NEXTDATA_LEN

	strHeader = strFrame.Mid(0, _SA_SYSTEMTYPE_LEN + _SA_CMD_LEN + _SA_DATATYPE_LEN + _SA_BODYLEN_LEN + _SA_NEXTDATA_LEN);
	
	strSystem   = strHeader.Mid(0,_SA_SYSTEMTYPE_LEN);
	strCMD      = strHeader.Mid(_SA_SYSTEMTYPE_LEN,_SA_CMD_LEN);
	strDataType = strHeader.Mid(_SA_SYSTEMTYPE_LEN + _SA_CMD_LEN,_SA_DATATYPE_LEN);	
	nBodyLen    = _ttoi( strHeader.Mid(_SA_SYSTEMTYPE_LEN + _SA_CMD_LEN + _SA_DATATYPE_LEN,_SA_BODYLEN_LEN) );
	strNextData = strHeader.Mid(_SA_SYSTEMTYPE_LEN + _SA_CMD_LEN + _SA_DATATYPE_LEN + _SA_BODYLEN_LEN);

	// 3. analyze body
	// DataCnt | DataLen | DataValue | DataLen | DataValue |.....
	//    6    |    5    |    ...    |    5    |    ...    |.....

	// check body size
	if( nBodyLen )
	{
		strBody = strFrame.Mid(_SA_SYSTEMTYPE_LEN + _SA_CMD_LEN + _SA_DATATYPE_LEN + _SA_BODYLEN_LEN + _SA_NEXTDATA_LEN);
		if(nBodyLen != strBody.GetLength()) return -1021;
		nDataCnt = _ttoi(strBody.Mid(0, _SA_DATACNT_LEN));
		astrData.RemoveAll(); astrData.FreeExtra();
		strBuf = strBody.Mid(_SA_DATACNT_LEN);
		for(INT i = 0; i < nDataCnt; i++)
		{
			nDataSize = _ttoi(strBuf.Mid(0, _SA_DATALEN_LEN));
			astrData.Add(strBuf.Mid(_SA_DATALEN_LEN, nDataSize));
			strBuf = strBuf.Mid(_SA_DATALEN_LEN + nDataSize);
		}
	}

	return 0;
}