#include "stdafx.h"
#include "RotatingLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif




// [6/4/2002] =============================================================
// [6/4/2002]	C O N S T R U C T I O N / D E S T R U C T I O N 
// [6/4/2002] =============================================================


//////////////////////////////////////////////////////////////////////
//
// Constructor
//    Constructs the CRotatingLog class, and opens the log file.
//
// Parameters
//    File       [in] - the name of the log file
//    MaxLines   [in] - the maximum number of lines the file is to contain
//    LineLength [in] - the length of each line in the file
//
// Returns
//    Nothing
//
// Note
//    Throws a CFileException if unable to open the file 'File'
//
//////////////////////////////////////////////////////////////////////

CRotatingLog::CRotatingLog(const CString &File, BOOL bMakeDateFilename/*=FALSE*/, UINT nOpenFlags/*=CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite*/, UINT MaxLines/*=9999*/, UINT LineLength/*=1024*/)
{
	this->SetRotateLineLimit(MaxLines, LineLength);
	
	if (bMakeDateFilename)	this->m_FilePath = this->MakeDateFilename(File);
	else					this->m_FilePath = File;

	this->m_bFileOpen = FALSE;

	// KOL.UDF.022 시큐어코딩 보완
	m_bForward = FALSE;
	m_linelength = 0;
	m_maxlines = 0;
	m_bFileOpen = FALSE;
}

//////////////////////////////////////////////////////////////////////
//
// Destructor
//    Closes the file
//
// Parameters
//    None
//
// Returns
//    Nothing
//
//////////////////////////////////////////////////////////////////////

CRotatingLog::~CRotatingLog()
{
	StopLogging();
}


// [6/4/2002] =============================================================
// [6/4/2002]	O P E R A T I O N
// [6/4/2002] =============================================================

BOOL CRotatingLog::StopLogging()
{
	if (this->m_bFileOpen) {
		this->m_file.Close();	
		this->m_bFileOpen = FALSE;
	}
	return TRUE;
}
BOOL CRotatingLog::StartLogging()
{
	this->Open();
	return TRUE;
}

BOOL CRotatingLog::Open()
{
	this->m_bFileOpen = FALSE;
	CFileException e;
	if (!this->m_file.Open(this->m_FilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite | CFile::shareDenyWrite | CFile::typeBinary, &e))
	{
		TRACE (_T("CRotatingLog::CRotatingLog - Unable to open file\n"));
		AfxThrowFileException(e.m_cause, e.m_lOsError, e.m_strFileName);
		this->m_file.m_hFile = NULL;
		return FALSE;
	}
	
	this->m_file.SeekToBegin();
	if( 0 == this->m_file.GetLength() )
	{			
		TCHAR cUni = 0xFEFF;
		this->m_file.Write( &cUni, sizeof(TCHAR));
	}
	
	this->m_bFileOpen = TRUE;
	return TRUE;
}

CString CRotatingLog::MakeDateFilename(const CString &Filename)
{
	CString sPath;
	CString sDateFilename;
	INT		nBackSlashIndex;

    long	ltime;
    tm		*t;

	nBackSlashIndex = Filename.ReverseFind('\\');
	if (0<nBackSlashIndex) sPath = Filename.Left(nBackSlashIndex+1);
	
    time(&ltime);
    t = localtime (&ltime);

	sDateFilename.Format(_T("(%04d%02d%02d)"), 1900+t->tm_year, t->tm_mon+1, t->tm_mday);
	sDateFilename += Filename.Mid(nBackSlashIndex+1);

	return (sPath+sDateFilename);
}

//////////////////////////////////////////////////////////////////////
//
// AddLine
//    Adds a line of text to the file
//
// Parameters
//    Line [in] - the text string to be added to the file
//
// Returns
//    TRUE on success, FALSE on failure. Use GetLastError() to get 
//    error information on failure.
//
// Note
//    If the 'Line' is longer than m_linelength, it will be truncated
//
//////////////////////////////////////////////////////////////////////

BOOL CRotatingLog::AddLine(const CString& Line, BOOL bWriteDateTime/*=TIME_LOG*/, BOOL bNewLine/*=NEW_LINE*/)
{
	BOOL returnvalue = TRUE;

	if (this->m_bFileOpen) 
	{
		TCHAR *buffer = new TCHAR[this->m_linelength + 2];
		wmemset(buffer, ' ', this->m_linelength);
		buffer[this->m_linelength] = '\n';
		buffer[this->m_linelength + 1] = 0x00;
		
		try {
			
			//if (this->m_bFileOpen) {
				CString sLog;
				UINT lastline = 0;
/*
				this->m_file.SeekToBegin();
				if (this->m_file.ReadString(sLog)) {
					lastline = _ttoi(sLog);
					if (0 == lastline) 
						AfxThrowFileException(CFileException::generic, ERROR_FILE_CORRUPT);
				}
				lastline++;
				if (lastline>this->m_maxlines) lastline = 1;	//Rotary

				long position = this->m_file.Seek( (lastline-1) * (this->m_linelength+2) + 6, CFile::begin );
				this->m_file.WriteString(buffer);
*/
				if (bWriteDateTime) {
					long	ltime;
					tm		*t;
					time(&ltime);
					t = localtime (&ltime);
					sLog.Format(_T("[%04d:%02d:%02d:%02d:%02d:%02d] "), 
								t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
				} else {
					sLog.Empty();
				}

				//this->m_file.Seek(position, CFile::begin);
				this->m_file.SeekToEnd();
				sLog += Line.Left(this->m_linelength-sLog.GetLength());
				this->m_file.WriteString(sLog);

				if (bNewLine) this->m_file.WriteString(_T("\n"));
				
				//this->m_file.SeekToBegin();
				//sLog.Format(_T("%04d\n"), lastline);
				//this->m_file.WriteString(sLog);
			//}
		
		} catch (CFileException *e) {

			SetLastError(e->m_lOsError);
			e->Delete();
			delete [] buffer;
			returnvalue = FALSE;
		}
	
		delete [] buffer;
	}
	
	return returnvalue;
}

BOOL CRotatingLog::StartAddLineStop(const CString& Line, BOOL bWriteDateTime/*=TIME_LOG*/, BOOL bNewLine/*=NEW_LINE*/)
{
	INT rc;
	this->StartLogging();
	rc = this->AddLine(Line, bWriteDateTime, bNewLine);
	this->StopLogging();
	return rc;
}

BOOL CRotatingLog::StartAddLine(const CString& Line, BOOL bWriteDateTime/*=TIME_LOG*/, BOOL bNewLine/*=NEW_LINE*/)
{
	this->StartLogging();
	return this->AddLine(Line, bWriteDateTime, bNewLine);
}

BOOL CRotatingLog::AddLineStop(const CString& Line, BOOL bWriteDateTime/*=TIME_LOG*/, BOOL bNewLine/*=NEW_LINE*/)
{
	INT rc;
	rc = this->AddLine(Line, bWriteDateTime, bNewLine);
	this->StopLogging();
	return rc;
}


//////////////////////////////////////////////////////////////////////
//
// GetDumpStart
//    Gets the line count of the oldest line in the file. Use this value
//    as the intial 'count' in GetDumpLine. bForward sets the direction 
//    GetDumpLine() goes to retrieve lines from the file
//
// Parameters
//    bForward [in] - the order to retrieve lines :
//                    TRUE  - oldest to newest
//                    FALSE - newest to oldest
//                                  
//
// Returns
//    The line count
//    zero if the file is empty
//    -1 if an error occurs, use GetLastError() to get error information.
//
//////////////////////////////////////////////////////////////////////

UINT CRotatingLog::GetDumpStart(BOOL bForward)
{
	m_bForward = bForward;
	UINT line = 0;
	
	try {
				
		this->m_file.SeekToBegin();
		TCHAR cUni;
		this->m_file.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			this->m_file.SeekToBegin();
		}
		CString str;
		if (this->m_file.ReadString(str)) {
			line = _ttoi(str);
			if (line == 0) AfxThrowFileException(CFileException::generic, ERROR_FILE_CORRUPT);
			if (m_bForward) {
				line++;
				if (line>this->m_maxlines) {
					line = 1;
				} else {
					UINT pos = line * (this->m_linelength + 2) + 6;
					if (pos>this->m_file.GetLength()) line = 1;
				}
			}
		}
	
	} catch (CFileException *e) {

		SetLastError(e->m_lOsError);
		e->Delete();
		line = -1;
		
	}

	return line;
}

//////////////////////////////////////////////////////////////////////
//
// GetDumpLine
//    Gets the line of text at the 'count', and updates count to point
//    to the next line.
//
// Parameters
//    count [in/out] - in - the line to get
//                     out - the next line to get
//    line      [in] - a CString to receive the line of text
//
// Returns
//    TRUE on success, FALSE on failure. Use GetLastError() to get error
//    information on failure
//
// Note
//    The count will loop back to the beginning, so you will have gone
//    through the file when count equals the value returned from GetDumpStart 
//
//////////////////////////////////////////////////////////////////////

BOOL CRotatingLog::GetDumpLine(UINT &count, CString &line)
{
	ASSERT (count != 0);
	try 
	{
		this->m_file.Seek((count - 1) * (this->m_linelength + 2) + 6, CFile::begin);
		this->m_file.ReadString(line);
		line.TrimRight();
		
		if (m_bForward) 
		{
			count++;
			if (count>this->m_maxlines) 
			{
				count = 1;
			} 
			else 
			{
				UINT pos = count * (this->m_linelength + 2) + 6;
				if (pos > this->m_file.GetLength()) count = 1;
			}
		} 
		else 
		{
			count--;
			if (count == 0) count = (this->m_file.GetLength() - 6) / (this->m_linelength + 2);
		}
	
	} catch (CFileException *e) {
		SetLastError(e->m_lOsError);
		e->Delete();
		return FALSE;
	}

	return TRUE;
}


// [6/4/2002] =============================================================
// [6/4/2002]	P R O P E R T I E S
// [6/4/2002] =============================================================

//////////////////////////////////////////////////////////////////////
//
// SetRotateLineLimit
//
// Parameters
//    MaxLines   [in] - the maximum number of lines the file is to contain
//    LineLength [in] - the length of each line in the file
//
// Returns
//    Nothing
//
//////////////////////////////////////////////////////////////////////

VOID CRotatingLog::SetRotateLineLimit(UINT MaxLines, UINT LineLength)
{
	this->m_maxlines = MaxLines;
	if (1>this->m_maxlines)		this->m_maxlines = 1;
	if (9999<this->m_maxlines)	this->m_maxlines = 9999;
	this->m_linelength = LineLength;
}
